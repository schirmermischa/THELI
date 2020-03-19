/*
Copyright (C) 2019 Mischa Schirmer

This file is part of THELI.

THELI is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program in the LICENSE file.
If not, see https://www.gnu.org/licenses/ .
*/


// This file deals with running sextractor as an external tool

#include "myimage.h"
#include "../functions.h"
#include "../tools/polygon.h"
#include "../tools/tools.h"
#include "../tools/cfitsioerrorcodes.h"
#include "../processingInternal/data.h"
#include "../threading/sexworker.h"
#include "wcs.h"
#include "wcshdr.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QString>
#include <QProcess>
#include <QTest>

void MyImage::buildSexCommand()
{
    if (!successProcessing) return;

    // Create the 'cat/' sub-directory if it does not exist yet.
    mkAbsDir(path+"/cat/iview/");

    QString sex = findExecutableName("sex");
    sexCommand = sex + " ";
    sexCommand += path + "/" + chipName + processingStatus->statusString + ".fits";
    sexCommand += " -CATALOG_NAME " + path+"/cat/" + chipName + ".cat";
    sexCommand += " -WEIGHT_IMAGE " + weightPath + "/" + chipName + ".weight.fits";

    // NOTE: further options are appended in controller::detectionSExtractor()

    // Check if weight exists
    QFile weight(weightPath+"/"+chipName+".weight.fits");
    if (!weight.exists()) {
        emit messageAvailable(baseName + " : associated weight map not found:<br>"
                              + weightPath + "/" + chipName + ".weight.fits<br>"
                              + "The weight is required for the creation of source catalogs.", "error");
        emit critical();
        successProcessing = false;
    }
}

// start in same thread
void MyImage::createSextractorCatalog_old()
{
    if (!successProcessing) return;

    QProcess process;
    process.start("/bin/sh -c \""+sexCommand+"\"");
    process.waitForFinished(-1);
}

// start in new thread
void MyImage::createSextractorCatalog()
{
    if (!successProcessing) return;

    if (*verbosity >= 2) emit messageAvailable("Running the following command in " + path + " : <br>"+sexCommand, "image");

    // Run the SExtractor command
    workerThread = new QThread();
    sexWorker = new SexWorker(sexCommand, path);
    sexWorker->moveToThread(workerThread);
    connect(workerThread, &QThread::started, sexWorker, &SexWorker::runSex);
    connect(sexWorker, &SexWorker::errorFound, this, &MyImage::errorFoundReceived);
    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);
    // Direct connection required, otherwise the task hangs after the first sextractor command
    // (does not proceed to the next step in the controller's for loop)
    connect(sexWorker, &SexWorker::finished, workerThread, &QThread::quit, Qt::DirectConnection);
    connect(sexWorker, &SexWorker::finished, sexWorker, &QObject::deleteLater);
    connect(sexWorker, &SexWorker::messageAvailable, this, &MyImage::messageAvailableReceived);
    workerThread->start();
    workerThread->wait();
}

void MyImage::errorFoundReceived()
{
    successProcessing = false;
}

void MyImage::filterSextractorCatalog(QString minFWHM, QString maxFlag)
{
    if (!successProcessing) return;

    fitsfile *fptr;
    int status = 0;

//    QString filterString = "Flag <= "+maxFlag + " && FWHM_IMAGE <= "+minFWHM;
    QString filterString = "FLAGS <= "+ maxFlag + " && FWHM_IMAGE >= "+minFWHM;
    QString catName = path+"/cat/"+chipName+".cat";
    fits_open_file(&fptr, catName.toUtf8().data(), READWRITE, &status);
    char tblname[100] = "LDAC_OBJECTS";
    int extver = 0;
    fits_movnam_hdu(fptr, BINARY_TBL, tblname, extver, &status);
    fits_select_rows(fptr, fptr, filterString.toUtf8().data(), &status);
    fits_close_file(fptr, &status);

    printCfitsioError("MyImage::filterSextractorCatalog()", status);

    // Cannot do that, creating a child for a parent in a different thread;
    // should push the message directly to monitor instead;
    // printCfitsioError("MyImage::filterSextractorCatalog()", status);
}

void MyImage::sexcatToIview()
{
    if (!successProcessing) return;

    fitsfile *fptr;
    int status = 0;

    QString catName = path+"/cat/"+chipName+".cat";
    fits_open_file(&fptr, catName.toUtf8().data(), READONLY, &status);

    // Move to the LDAC_OBJECTS table
    char tblname[100] = "LDAC_OBJECTS";
    int extver = 0;
    fits_movnam_hdu(fptr, BINARY_TBL, tblname, extver, &status);

    long nrows = 0;
    int xwinColNum = -1;
    int ywinColNum = -1;
    int awinColNum = -1;
    int bwinColNum = -1;
    int thetawinColNum = -1;

    fits_get_num_rows(fptr, &nrows, &status);
    char xwinName[100] = "XWIN_IMAGE";
    char ywinName[100] = "YWIN_IMAGE";
    char awinName[100] = "AWIN_IMAGE";
    char bwinName[100] = "BWIN_IMAGE";
    char thetawinName[100] = "THETAWIN_IMAGE";
    fits_get_colnum(fptr, CASESEN, xwinName, &xwinColNum, &status);
    fits_get_colnum(fptr, CASESEN, ywinName, &ywinColNum, &status);
    fits_get_colnum(fptr, CASESEN, awinName, &awinColNum, &status);
    fits_get_colnum(fptr, CASESEN, bwinName, &bwinColNum, &status);
    fits_get_colnum(fptr, CASESEN, thetawinName, &thetawinColNum, &status);
    int firstrow = 1;
    int firstelem = 1;
    int anynul = 0;
    double *xwin = new double[nrows];
    double *ywin = new double[nrows];
    float *awin = new float[nrows];
    float *bwin = new float[nrows];
    float *thetawin = new float[nrows];

    fits_read_col(fptr, TDOUBLE, xwinColNum, firstrow, firstelem, nrows, NULL, xwin, &anynul, &status);
    fits_read_col(fptr, TDOUBLE, ywinColNum, firstrow, firstelem, nrows, NULL, ywin, &anynul, &status);
    fits_read_col(fptr, TFLOAT, awinColNum, firstrow, firstelem, nrows, NULL, awin, &anynul, &status);
    fits_read_col(fptr, TFLOAT, bwinColNum, firstrow, firstelem, nrows, NULL, bwin, &anynul, &status);
    fits_read_col(fptr, TFLOAT, thetawinColNum, firstrow, firstelem, nrows, NULL, thetawin, &anynul, &status);
    fits_close_file(fptr, &status);

    printCfitsioError("MyImage::sexcatToIview())", status);

    QString iviewName = path+"/cat/iview/"+chipName+".iview";
    QFile file(iviewName);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream outputStream(&file);
        for (int i=0; i<nrows; ++i) {
            outputStream.setRealNumberPrecision(9);
            outputStream << xwin[i] << " "
                         << ywin[i] << " "
                         << awin[i] << " "
                         << bwin[i] << " "
                         << thetawin[i] << "\n";
        }
        file.close();
    }
    else {
        emit messageAvailable("MyImage::sexcatToIview(): Could not write to "+iviewName, "error");
        emit critical();
        successProcessing = false;
    }

    delete xwin;
    delete ywin;
    delete awin;
    delete bwin;
    delete thetawin;
}

void MyImage::appendToScampCatalogSExtractor(fitsfile *fptr)
{
    if (!successProcessing) return;

    // Copy the LDAC_IMHEAD and LDAC_OBJECTS tables to fptr
    fitsfile *fptrSex;
    int status = 0;
    int hduType = 0;
    QString filename = path+"/cat/"+chipName+".cat";
    fits_open_file(&fptrSex, filename.toUtf8().data(), READONLY, &status);
    fits_movabs_hdu(fptrSex, 2, &hduType, &status);
    fits_copy_hdu(fptrSex, fptr, 0, &status);
    fits_movabs_hdu(fptrSex, 3, &hduType, &status);
    fits_copy_hdu(fptrSex, fptr, 0, &status);
    fits_close_file(fptrSex, &status);
    printCfitsioError("appendToScampCatalogSExtractor", status);
}
