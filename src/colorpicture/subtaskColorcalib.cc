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

#include "colorpicture.h"
#include "ui_colorpicture.h"
#include "../functions.h"
#include "../tools/tools.h"
#include "../query/query.h"
#include "../myfits/myfits.h"
#include "../tools/cfitsioerrorcodes.h"

#include <omp.h>
#include <QFileDialog>
#include <QStringList>
#include <QStringListModel>
#include <QDateTime>
#include <QTest>
#include <QSettings>
#include <QMessageBox>

void ColorPicture::on_calibratePushButton_clicked()
{
    photcatresult[0].catname = "PANSTARRS";
    photcatresult[1].catname = "SDSS";
    photcatresult[2].catname = "APASS";
    photcatresult[3].catname = "AVGWHITE";

    ui->redFactorLineEdit->setText("");
    ui->greenFactorLineEdit->setText("1.000");
    ui->blueFactorLineEdit->setText("");
    ui->redErrorLineEdit->setText("");
    ui->greenErrorLineEdit->setText("0.000");
    ui->blueErrorLineEdit->setText("");

    QDir colorCalibDir(mainDir+"/color_theli/PHOTCAT_calibration/");
    if (! colorCalibDir.exists()) {
        if (!colorCalibDir.mkdir(mainDir+"/color_theli/PHOTCAT_calibration/")) {
            emit messageAvailable("Could not create <br>"+mainDir+"/color_theli/PHOTCAT_calibration/", "error");
            return;
        }
    }

    ui->calibratePushButton->setText("Running ...");
    ui->calibratePushButton->setDisabled(true);
    ui->redComboBox->setDisabled(true);
    ui->greenComboBox->setDisabled(true);
    ui->blueComboBox->setDisabled(true);
    resetResultButtonGroup("alsoResetLabels");

    QTest::qWait(50);

    workerThread = new QThread();
    colorpictureWorker = new ColorPictureWorker(this, "ColorCalib");
    colorpictureWorker->moveToThread(workerThread);
    connect(workerThread, &QThread::started, colorpictureWorker, &ColorPictureWorker::runTask);
    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater, Qt::DirectConnection);
    connect(colorpictureWorker, &ColorPictureWorker::finished, workerThread, &QThread::quit, Qt::DirectConnection);
    connect(colorpictureWorker, &ColorPictureWorker::finished, colorpictureWorker, &QObject::deleteLater, Qt::DirectConnection);
    connect(colorpictureWorker, &ColorPictureWorker::messageAvailable, this, &ColorPicture::displayMessage);
    connect(colorpictureWorker, &ColorPictureWorker::tasknameReturned, this, &ColorPicture::taskFinished);
    workerThread->start();
}

void ColorPicture::taskInternalColorCalib()
{
    QList<Query*> queryList = {PANSTARRSquery, SDSSquery, APASSquery};

    // Retrieve the photometric reference catalogs and identify solar type analogs
    colorCalibRetrieveCatalogs(queryList);

    // Extract solar analogs
    filterSolarTypeStars(queryList);

    // Detect sources in all images
    colorCalibSegmentImages();

    // Match the object catalogs with the reference catalogs, and calculate the various correction factors
    colorCalibMatchCatalogs();
}

void ColorPicture::colorCalibMatchCatalogs()
{
    emit messageAvailable("Matching object catalogs ...", "ignore");

    MyImage *imageR = identifyCroppedMyImage(ui->redComboBox->currentText());
    MyImage *imageG = identifyCroppedMyImage(ui->greenComboBox->currentText());
    MyImage *imageB = identifyCroppedMyImage(ui->blueComboBox->currentText());
    if (imageR == nullptr || imageG == nullptr || imageB == nullptr) {
        emit messageAvailable("Could not identify the three RGB channel images!", "error");
        return;
    }

    QVector<QVector<double>> objDatR = getObjectData(imageR);
    QVector<QVector<double>> objDatG = getObjectData(imageG);
    QVector<QVector<double>> objDatB = getObjectData(imageB);

    // Match R with G
    int multipleR = 0;
    int multipleG = 0;
    int multipleB = 0;
    double tolerance;
    tolerance = (imageR->matchingTolerance + imageG->matchingTolerance) / 2.;
    QVector<QVector<double>> matchedRG;
    match2D(objDatR, objDatG, matchedRG, tolerance, multipleR, multipleG, maxCPU);

    // Match R+G with B
    tolerance = (imageR->matchingTolerance + imageB->matchingTolerance) / 2.;
    QVector<QVector<double>> matchedRGB;
    match2D(matchedRG, objDatB, matchedRGB, tolerance, multipleR, multipleB, maxCPU);

    // Extract AVGWHITE color correction factors
    QVector<float> rCorr;   // red correction factors wrt. green channel
    QVector<float> bCorr;   // cblue orrection factors wrt. green channel
    rCorr.reserve(matchedRGB.length());
    bCorr.reserve(matchedRGB.length());
    for (auto &obj : matchedRGB) {
        // 'obj' contains RA, DEC, flux_r, flux_g, flux_b
        rCorr.append(obj[3] / obj[2]);
        bCorr.append(obj[3] / obj[4]);
    }
    float num = rCorr.length();
    photcatresult[3].rfac    = QString::number(meanMask_T(rCorr),'f',3);
    photcatresult[3].rfacerr = QString::number(rmsMask_T(rCorr) / sqrt(num),'f',3);
    photcatresult[3].gfac    = "1.000";
    photcatresult[3].gfacerr = "0.000";
    photcatresult[3].bfac    = QString::number(meanMask_T(bCorr),'f',3);
    photcatresult[3].bfacerr = QString::number(rmsMask_T(bCorr) / sqrt(num),'f',3);
    photcatresult[3].nstars = QString::number(num);
    emit updateNrefStars("AVGWHITE", rCorr.length());

    // Match with reference catalogs
    tolerance = (imageR->matchingTolerance + imageG->matchingTolerance + imageB->matchingTolerance) / 3.;

    colorCalibMatchReferenceCatalog(matchedRGB, SDSS, tolerance);
    colorCalibMatchReferenceCatalog(matchedRGB, APASS, tolerance);
    colorCalibMatchReferenceCatalog(matchedRGB, PANSTARRS, tolerance);
}

void ColorPicture::colorCalibMatchReferenceCatalog(const QVector<QVector<double>> &matchedRGB, RefCatData *REFCAT, float tolerance)
{
    int index = 0;
    if (REFCAT->name == "PANSTARRS") index = 0;
    else if (REFCAT->name == "SDSS") index = 1;
    else if (REFCAT->name == "APASS") index = 2;

    if (matchedRGB.isEmpty() || REFCAT->ra.isEmpty()) {
        photcatresult[index].rfac    = "1.000";
        photcatresult[index].rfacerr = "0.000";
        photcatresult[index].gfac    = "1.000";
        photcatresult[index].gfacerr = "0.000";
        photcatresult[index].bfac    = "1.000";
        photcatresult[index].bfacerr = "0.000";
        photcatresult[index].nstars = "0";
        return;
    }

    QVector<QVector<double>> refDat;
    QVector<QVector<double>> matchedREFCAT;
    int dummy1;
    int dummy2;
    for (int i=0; i<REFCAT->ra.length(); ++i) {
        QVector<double> refdata;
        // Add the two coords, and a dummy magnitude of 0.0
        refdata << REFCAT->de[i] << REFCAT->ra[i] << 0.0;
        refDat.append(refdata);
    }
    match2D(matchedRGB, refDat, matchedREFCAT, tolerance, dummy1, dummy2, maxCPU);

    if (matchedREFCAT.isEmpty()) {
        emit messageAvailable("None of the G2-like sources in " + REFCAT->name + " were detected in your image.", "warning");

    }
    // Calculate the color correction factors
    QVector<float> rCorr;   // red correction factors wrt. green channel
    QVector<float> bCorr;   // cblue orrection factors wrt. green channel
    rCorr.reserve(matchedREFCAT.length());
    bCorr.reserve(matchedREFCAT.length());
    for (auto &obj : matchedREFCAT) {
        // 'obj' contains RA, DEC, flux_r, flux_g, flux_b
        rCorr.append(obj[3] / obj[2]);
        bCorr.append(obj[3] / obj[4]);
    }
    photcatresult[index].rfac    = QString::number(meanMask_T(rCorr),'f',3);
    photcatresult[index].rfacerr = QString::number(rmsMask_T(rCorr),'f',3);
    photcatresult[index].gfac    = "1.000";
    photcatresult[index].gfacerr = "0.000";
    photcatresult[index].bfac    = QString::number(meanMask_T(bCorr),'f',3);
    photcatresult[index].bfacerr = QString::number(rmsMask_T(bCorr),'f',3);
    photcatresult[index].nstars = QString::number(rCorr.length());
}

void ColorPicture::colorCalibRetrieveCatalogs(QList<Query*> queryList)
{
    // It appears that the parallel query is not threadsafe.
    // Results in not reproducible errors, at least when run through Qt5 debugger (but when run through valgrind)
//#pragma omp parallel for num_threads(maxCPU)
    for (int i=0; i<queryList.length(); ++i) {
        auto &query = queryList[i];
        emit messageAvailable("Querying "+query->refcatName + " ...", "ignore");
        query->photomDir = ui->dirLineEdit->text() + "/color_theli/";
        query->photomImage = croppedList[0];
        query->doColorCalibQueryFromWeb();
        query->identifySolarTypeStars();
        QString info = QString::number(query->numSources) + " sources found, "
                + QString::number(query->numG2sources) + " of which have G2-like colors";
        emit messageAvailable(info, "append");
        emit updateNrefStars(query->refcatName, query->numG2sources);
    }
}

void ColorPicture::filterSolarTypeStars(QList<Query*> queryList)
{
    SDSS->clear();
    APASS->clear();
    PANSTARRS->clear();

    for (auto &query : queryList) {
        if (query->numSources == 0) continue;
        for (long k=0; k<query->mag1_out.length(); ++k) {
            if (!query->G2type[k]) continue;
            if (query->refcatName == "SDSS") {
                SDSS->ra.append(query->ra_out[k]);
                SDSS->de.append(query->de_out[k]);
            }
            if (query->refcatName == "PANSTARRS") {
                PANSTARRS->ra.append(query->ra_out[k]);
                PANSTARRS->de.append(query->de_out[k]);
            }
            if (query->refcatName == "APASS") {
                APASS->ra.append(query->ra_out[k]);
                APASS->de.append(query->de_out[k]);
            }
//            qDebug() << PANSTARRS->ra << PANSTARRS->de << APASS->ra << APASS->de;
        }
    }
}

void ColorPicture::colorCalibSegmentImages()
{
    // Create object catalogs, get matching tolerance
#pragma omp parallel for num_threads(maxCPU)
    for (int i=0; i<croppedList.length(); ++i) {
        auto &it = croppedList[i];
        // Do nothing if we have the catalog already
        if (it->segmentationDone) continue;
        // Obtain catalog
        emit messageAvailable("Detecting sources in " + it->baseName +" ...", "ignore");
        it->maxCPU = maxCPU / croppedList.length();
        it->resetObjectMasking();
        it->readImage();
        it->readWeight();
        it->backgroundModel(100, "interpolate");
        it->segmentImage("5", "5", true, false);
        it->estimateMatchingTolerance();
        it->releaseBackgroundMemory();
        it->releaseDetectionPixelMemory();
        emit messageAvailable(it->baseName +" : " + QString::number(it->objectList.length()) + " sources found", "ignore");
    }
}

void ColorPicture::updateNrefStarsReceived(QString name, long number)
{
    QString nstars = QString::number(number) + " stars";
    if (name == "SDSS") ui->numSDSSLabel->setText(nstars);
    else if (name == "APASS") ui->numAPASSLabel->setText(nstars);
    else if (name == "PANSTARRS") ui->numPANSTARRSLabel->setText(nstars);
    else if (name == "AVGWHITE") ui->numAVGWHITELabel->setText(nstars);
}
