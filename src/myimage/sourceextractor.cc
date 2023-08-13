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


// This file deals with running Source Extractor as an external tool

#include "myimage.h"
#include "../functions.h"
#include "../tools/polygon.h"
#include "../tools/tools.h"
#include "../tools/cfitsioerrorcodes.h"
#include "../processingInternal/data.h"
#include "../threading/sourceextractorworker.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QString>
#include <QProcess>
#include <QTest>

void MyImage::buildSourceExtractorCommand()
{
    if (!successProcessing) return;

    // Create the 'cat/' sub-directory if it does not exist yet.
    mkAbsDir(path+"/cat/iview/");

    QString sourceExtractor = findExecutableName("source-extractor");
    sourceExtractorCommand = sourceExtractor + " ";
    sourceExtractorCommand += path + "/" + chipName + processingStatus->statusString + ".fits";
    sourceExtractorCommand += " -CATALOG_NAME " + path+"/cat/" + chipName + ".cat";
    sourceExtractorCommand += " -WEIGHT_IMAGE " + weightPath + "/" + chipName + ".weight.fits";

    // NOTE: further options are appended in controller::detectionSourceExtractor()

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
void MyImage::createSourceExtractorCatalog_old()
{
    if (!successProcessing) return;

    QProcess process;
    process.start("/bin/sh -c \""+sourceExtractorCommand+"\"");
    process.waitForFinished(-1);
}

// start in new thread
void MyImage::createSourceExtractorCatalog()
{
    if (!successProcessing) return;

    if (*verbosity >= 2) emit messageAvailable("Running the following command in " + path + " : <br>"+sourceExtractorCommand, "image");

    // Run the SourceExtractor command
    workerThread = new QThread();
    sourceExtractorWorker = new SourceExtractorWorker(sourceExtractorCommand, path);
    sourceExtractorWorker->moveToThread(workerThread);
    connect(workerThread, &QThread::started, sourceExtractorWorker, &SourceExtractorWorker::runSourceExtractor);
    connect(sourceExtractorWorker, &SourceExtractorWorker::errorFound, this, &MyImage::errorFoundReceived);
    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);
    // Direct connection required, otherwise the task hangs after the first SourceExtractor command
    // (does not proceed to the next step in the controller's for loop)
    connect(sourceExtractorWorker, &SourceExtractorWorker::finished, workerThread, &QThread::quit, Qt::DirectConnection);
    connect(sourceExtractorWorker, &SourceExtractorWorker::finished, sourceExtractorWorker, &QObject::deleteLater);
    connect(sourceExtractorWorker, &SourceExtractorWorker::messageAvailable, this, &MyImage::messageAvailableReceived);
    workerThread->start();
    workerThread->wait();
}

void MyImage::errorFoundReceived()
{
    successProcessing = false;
}

void MyImage::filterSourceExtractorCatalog(QString minFWHM, QString maxFlag, QString maxEll)
{
    if (!successProcessing) return;

    fitsfile *fptr;
    int status = 0;

    QString filterString = "";

    if (!maxFlag.isEmpty() ) filterString = "FLAGS <= "+ maxFlag;
    if (!minFWHM.isEmpty()) {
        if (filterString.isEmpty()) filterString = "FWHM_IMAGE >= "+minFWHM;
        else filterString += " && FWHM_IMAGE >= "+minFWHM;
    }
    if (!maxEll.isEmpty()) {
        if (filterString.isEmpty()) filterString = "ELLIPTICITY <= "+maxEll;
        else filterString += " && ELLIPTICITY <= "+maxEll;
    }
    if (filterString.isEmpty()) return;

    QString catName = path+"/cat/"+chipName+".cat";
    fits_open_file(&fptr, catName.toUtf8().data(), READWRITE, &status);
    char tblname[100] = "LDAC_OBJECTS";
    int extver = 0;
    fits_movnam_hdu(fptr, BINARY_TBL, tblname, extver, &status);
    fits_select_rows(fptr, fptr, filterString.toUtf8().data(), &status);
    fits_close_file(fptr, &status);

    printCfitsioError(__func__, status);

    // Cannot do that, creating a child for a parent in a different thread;
    // should push the message directly to monitor instead;
    // printCfitsioError(__func__, status);
}

void MyImage::calcMedianSeeingEllipticitySex(QString catName, int extnum)
{
    if (!successProcessing) return;

    fitsfile *fptr;
    int status = 0;

    if (catName.isEmpty()) catName = path+"/cat/"+chipName+".cat";
    fits_open_file(&fptr, catName.toUtf8().data(), READONLY, &status);

    // Move to the LDAC_OBJECTS table
    if (extnum == 0) {       // working on single source extractor catalogs before being merged into scamp catalog
        char tblname[100] = "LDAC_OBJECTS";
        int extver = 0;
        fits_movnam_hdu(fptr, BINARY_TBL, tblname, extver, &status);
    }
    else {
        int hduType = 0;
        fits_movabs_hdu(fptr, extnum, &hduType, &status);
    }

    long nrows = 0;
    int fwhmColNum = -1;
    int ellColNum = -1;

    fits_get_num_rows(fptr, &nrows, &status);
    char fwhmName[100] = "FWHM_IMAGE";
    char ellName[100] = "ELLIPTICITY";
    fits_get_colnum(fptr, CASESEN, fwhmName, &fwhmColNum, &status);
    fits_get_colnum(fptr, CASESEN, ellName, &ellColNum, &status);
    int firstrow = 1;
    int firstelem = 1;
    int anynul = 0;
    double *fwhm = new double[nrows];
    double *ell = new double[nrows];

    fits_read_col(fptr, TDOUBLE, fwhmColNum, firstrow, firstelem, nrows, NULL, fwhm, &anynul, &status);
    fits_read_col(fptr, TDOUBLE, ellColNum, firstrow, firstelem, nrows, NULL, ell, &anynul, &status);
    fits_close_file(fptr, &status);

    printCfitsioError(__func__, status);

    // Put into a vector so we can do calculations
    QVector<double> fwhmVec(nrows);
    QVector<double> ellVec(nrows);
    for (long i=0; i<nrows; ++i) {
        fwhmVec[i] = fwhm[i];
        ellVec[i] = ell[i];
    }

    fwhm_est = straightMedianInline(fwhmVec) * plateScale;
    ellipticity_est = straightMedianInline(ellVec);
    updateHeaderValueInFITS("FWHMEST", QString::number(fwhm_est, 'f', 2));
    updateHeaderValueInFITS("ELLIPEST", QString::number(ellipticity_est, 'f', 3));

    delete [] fwhm;
    delete [] ell;
    fwhm = nullptr;
    ell = nullptr;
}

void MyImage::sourceExtractorCatToIview()
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

    printCfitsioError(__func__, status);

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
        emit messageAvailable("MyImage::sourceExtractorCatToIview(): Could not write to "+iviewName, "error");
        emit critical();
        successProcessing = false;
    }

    delete [] xwin;
    delete [] ywin;
    delete [] awin;
    delete [] bwin;
    delete [] thetawin;
    xwin = nullptr;
    ywin = nullptr;
    awin = nullptr;
    bwin = nullptr;
    thetawin = nullptr;
}

void MyImage::appendToScampCatalogSourceExtractor(fitsfile *fptr)
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
    printCfitsioError(__func__, status);
}

void MyImage::sourceExtractorCatToAnet()
{
    if (!successProcessing) return;

    // Copy the XWIN_IMAGE, YWIN_IMAGE and MAG_AUTO columns to a new FITS table
    fitsfile *fptrAnet;
    int statusAnet = 0;
    char x[100] = "X";
    char y[100] = "Y";
    char mag[100] = "MAG";
    char *ttype[3] = {x, y, mag};
    char tf1[10] = "1D";
    char tf2[10] = "1D";
    char tf3[10] = "1E";
    char *tform[3] = {tf1, tf2, tf3};
    QString anetName = path+"/cat/"+chipName+".anet";
    fits_create_file(&fptrAnet, anetName.toUtf8().data(), &statusAnet);

    fitsfile *fptrSex;
    int statusSex = 0;
    int hduType = 0;
    int xColNum = -1;
    int yColNum = -1;
    int magColNum = -1;
    long nrows = -1;
    char xName[100] = "XWIN_IMAGE";
    char yName[100] = "YWIN_IMAGE";
    char magName[100] = "MAG_AUTO";
    QString filename = path+"/cat/"+chipName+".cat";
    fits_open_file(&fptrSex, filename.toUtf8().data(), READONLY, &statusSex);
    fits_movabs_hdu(fptrSex, 3, &hduType, &statusSex);
    fits_get_colnum(fptrSex, CASESEN, xName, &xColNum, &statusSex);
    fits_get_colnum(fptrSex, CASESEN, yName, &yColNum, &statusSex);
    fits_get_colnum(fptrSex, CASESEN, magName, &magColNum, &statusSex);

    fits_get_num_rows(fptrSex, &nrows, &statusSex);
    fits_create_tbl(fptrAnet, BINARY_TBL, nrows, 3, ttype, tform, nullptr, "OBJECTS", &statusAnet);

    fits_copy_col(fptrSex, fptrAnet, xColNum, 1, FALSE, &statusSex);
    fits_copy_col(fptrSex, fptrAnet, yColNum, 2, FALSE, &statusSex);
    fits_copy_col(fptrSex, fptrAnet, magColNum, 3, FALSE, &statusSex);
    fits_close_file(fptrSex, &statusSex);
    printCfitsioError(__func__, statusSex);

    fits_close_file(fptrAnet, &statusAnet);
    printCfitsioError(__func__, statusAnet);
}
