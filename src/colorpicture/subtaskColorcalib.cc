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
    photcatresult[2].catname = "SKYMAPPER";
    photcatresult[3].catname = "APASS";
    photcatresult[4].catname = "AVGWHITE";

    ui->redFactorLineEdit->setText("");
    ui->greenFactorLineEdit->setText("1.000");
    ui->blueFactorLineEdit->setText("");
    ui->redErrorLineEdit->setText("");
    ui->greenErrorLineEdit->setText("0.000");
    ui->blueErrorLineEdit->setText("");

    if (ui->DTLineEdit->text().isEmpty()) ui->DTLineEdit->setText("3.0");
    if (ui->DMINLineEdit->text().isEmpty()) ui->DMINLineEdit->setText("5.0");

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
    // get platescale
    if (croppedList.isEmpty()) return;
    croppedList.at(0)->loadHeader();     // get access to platescale and radius

    QList<Query*> queryList;
    if (croppedList.at(0)->plateScale > 3. || croppedList.at(0)->radius > 1.5) {
        queryList = {APASSquery};
        ui->resultPANSTARRSPushButton->setDisabled(true);
        ui->resultSDSSPushButton->setDisabled(true);
        ui->resultSKYMAPPERPushButton->setDisabled(true);
        ui->numPANSTARRSLabel->setText("0 stars");
        ui->numSDSSLabel->setText("0 stars");
        ui->numSKYMAPPERLabel->setText("0 stars");
        emit messageAvailable("PANSTARRS, SDSS and SKYMAPPER queries deactivated, field of view is too large.", "warning");
    }
    else {
        queryList = {PANSTARRSquery, SDSSquery, SKYMAPPERquery, APASSquery};
    }

    // Retrieve the photometric reference catalogs and identify solar type analogs

    // nested parallelism within colorCalibSegmentImages();
    //#pragma omp parallel sections
    //    {
    //#pragma omp section
    //        {
    // Detect sources in all images
    colorCalibSegmentImages();
    //        }
    // retrieve catalogs
    colorCalibRetrieveCatalogs(queryList);

    // Extract solar analogs
    filterSolarTypeStars(queryList);
    //        }
    //#pragma omp section
    //        {
    //        }
    //    }

    // Match the object catalogs with the reference catalogs, and calculate the various correction factors
    colorCalibMatchCatalogs();
}

void ColorPicture::colorCalibMatchCatalogs()
{
    emit messageAvailable("Matching object and reference catalogs ...", "ignore");

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

    // if filter order is changed, must update rCorr and bCorr below!
    match2D(objDatR, objDatG, matchedRG, tolerance, multipleR, multipleG, 0);

    // Match R+G with B
    tolerance = (imageR->matchingTolerance + imageB->matchingTolerance) / 2.;
    QVector<QVector<double>> matchedRGB;
    match2D(matchedRG, objDatB, matchedRGB, tolerance, multipleR, multipleB, maxCPU);

    QString tolB = QString::number(imageB->matchingTolerance*3600,'f',1) + "\" (" + QString::number(imageB->matchingTolerance*3600/imageB->plateScale,'f',1) + " pix)";
    QString tolG = QString::number(imageG->matchingTolerance*3600,'f',1) + "\" (" + QString::number(imageG->matchingTolerance*3600/imageG->plateScale,'f',1) + " pix)";
    QString tolR = QString::number(imageR->matchingTolerance*3600,'f',1) + "\" (" + QString::number(imageR->matchingTolerance*3600/imageR->plateScale,'f',1) + " pix)";
    emit messageAvailable("RGB matching tolerances:  " + tolB + " --- " + tolG + " --- " +tolR, "ignore");
    emit messageAvailable("RGB # of matched sources: " + QString::number(matchedRGB.length()), "ignore");

    // Extract AVGWHITE color correction factors
    QVector<double> rCorr;   // red correction factors wrt. green channel
    QVector<double> bCorr;   // blue correction factors wrt. green channel
    rCorr.clear();
    bCorr.clear();
    rCorr.reserve(matchedRGB.length());
    bCorr.reserve(matchedRGB.length());
    for (auto &obj : matchedRGB) {
        // 'obj' contains RA, DEC, flux_b, flux_g, flux_r
        // CAREFUL: mag order depends on which filter is loaded into match2D as source and which one as reference
        rCorr.append(obj[3] / obj[4]);
        bCorr.append(obj[3] / obj[2]);
    }
    double num = rCorr.length();

    // copy result for AVGWHITE into photcatresult
    int nrefcat = 4;
    photcatresult[nrefcat].rfac    = QString::number(medianMask_T(rCorr),'f',3);
    photcatresult[nrefcat].rfacerr = QString::number(medianerrMask(rCorr) / sqrt(num),'f',3);
    photcatresult[nrefcat].gfac    = "1.000";
    photcatresult[nrefcat].gfacerr = "0.000";
    photcatresult[nrefcat].bfac    = QString::number(medianMask_T(bCorr),'f',3);
    photcatresult[nrefcat].bfacerr = QString::number(medianerrMask(bCorr) / sqrt(num),'f',3);
    photcatresult[nrefcat].nstars = QString::number(num);
    emit updateNrefStars("AVGWHITE", rCorr.length());

    // Match with reference catalogs
    tolerance = (imageR->matchingTolerance + imageG->matchingTolerance + imageB->matchingTolerance) / 3.;

    filterReferenceCatalog(PANSTARRS, imageG);
    colorCalibMatchReferenceCatalog(matchedRGB, PANSTARRS, tolerance);

    filterReferenceCatalog(SDSS, imageG);
    colorCalibMatchReferenceCatalog(matchedRGB, SDSS, tolerance);

    filterReferenceCatalog(SKYMAPPER, imageG);
    colorCalibMatchReferenceCatalog(matchedRGB, SKYMAPPER, tolerance);

    filterReferenceCatalog(APASS, imageG);
    colorCalibMatchReferenceCatalog(matchedRGB, APASS, tolerance);
}

// Remove objects outside the actual field of view (simply for clarity)
// We need to test a single exposure, only. Does not matter whether it is the red, green or blue channel
void ColorPicture::filterReferenceCatalog(RefCatData *REFCAT, MyImage *channelImage)
{
    long countG2inside = 0;
    for (int i=0; i<REFCAT->ra.length(); ++i) {
        // is the reference source within the FITS image?
        if (channelImage->containsRaDec(REFCAT->ra[i], REFCAT->de[i])) {
            // if yes, is it covered by valid pixels (rejecting empty image borders)
            double x = 0.;
            double y = 0.;
            channelImage->sky2xy(REFCAT->ra[i], REFCAT->de[i], x, y);
            long n = channelImage->naxis1;
            if (channelImage->dataCurrent[x + n * y] != 0.) ++countG2inside;
        }
    }
    if (REFCAT->ra.length() > 0) {
        emit messageAvailable(REFCAT->name + " : " + QString::number(countG2inside) + " of the G2 references located inside pixel area", "note");
    }
}

void ColorPicture::colorCalibMatchReferenceCatalog(const QVector<QVector<double>> &matchedRGB, RefCatData *REFCAT, float tolerance)
{
    int index = 0;
    if (REFCAT->name == "PANSTARRS") index = 0;
    else if (REFCAT->name == "SDSS") index = 1;
    else if (REFCAT->name == "SKYMAPPER") index = 2;
    else if (REFCAT->name == "APASS") index = 3;

    if (matchedRGB.isEmpty() || REFCAT->ra.isEmpty()) {
        photcatresult[index].rfac    = "1.000";
        photcatresult[index].rfacerr = "0.000";
        photcatresult[index].gfac    = "1.000";
        photcatresult[index].gfacerr = "0.000";
        photcatresult[index].bfac    = "1.000";
        photcatresult[index].bfacerr = "0.000";
        photcatresult[index].nstars = "0";

        QString nstars = "0 stars";
        if (REFCAT->ra.isEmpty()) {
            if (REFCAT->name == "PANSTARRS") ui->numPANSTARRSLabel->setText(nstars);
            else if (REFCAT->name == "SDSS") ui->numSDSSLabel->setText(nstars);
            else if (REFCAT->name == "SKYMAPPER") ui->numSKYMAPPERLabel->setText(nstars);
            else if (REFCAT->name == "APASS") ui->numAPASSLabel->setText(nstars);
        }
        return;
    }

    QVector<QVector<double>> refDat;
    QVector<QVector<double>> matchedREFCAT;
    int dummy1;
    int dummy2;
    for (int i=0; i<REFCAT->ra.length(); ++i) {
        QVector<double> refdata;
        // Add the two coords, and a dummy magnitude of 0.0 (required by match2D algorithm)
        refdata << REFCAT->de[i] << REFCAT->ra[i] << 0.0;
        refDat.append(refdata);
    }

    match2D(matchedRGB, refDat, matchedREFCAT, tolerance, dummy1, dummy2, maxCPU);

    if (matchedREFCAT.isEmpty()) {
        emit messageAvailable("None of the G2-like sources in " + REFCAT->name + " were detected in your image.", "warning");
        photcatresult[index].rfac    = "1.000";
        photcatresult[index].rfacerr = "0.000";
        photcatresult[index].gfac    = "1.000";
        photcatresult[index].gfacerr = "0.000";
        photcatresult[index].bfac    = "1.000";
        photcatresult[index].bfacerr = "0.000";
        photcatresult[index].nstars = "0";
        writeG2refcat(REFCAT->name, matchedREFCAT);
        QString nstars = "0 stars";
        if (REFCAT->name == "PANSTARRS") ui->numPANSTARRSLabel->setText(nstars);
        else if (REFCAT->name == "SDSS") ui->numSDSSLabel->setText(nstars);
        else if (REFCAT->name == "SKYMAPPER") ui->numSKYMAPPERLabel->setText(nstars);
        else if (REFCAT->name == "APASS") ui->numAPASSLabel->setText(nstars);
        return;
    }
    // Calculate the color correction factors
    // matchRGB contains magnitudes in the order BGR, i.e. matchREFCAT contains: magref-B-G-R
    QVector<float> rCorr;   // red correction factors wrt. green channel
    QVector<float> bCorr;   // cblue orrection factors wrt. green channel
    rCorr.reserve(matchedREFCAT.length());
    bCorr.reserve(matchedREFCAT.length());
    for (auto &obj : matchedREFCAT) {
        // 'obj' contains RA, DEC, 0.0, flux_b, flux_g, flux_r          // 0.0 is the dummy magnitude
        rCorr.append(obj[4] / obj[5]);
        bCorr.append(obj[4] / obj[3]);
    }
    photcatresult[index].rfac    = QString::number(meanMask_T(rCorr),'f',3);
    photcatresult[index].rfacerr = QString::number(rmsMask_T(rCorr),'f',3);
    photcatresult[index].gfac    = "1.000";
    photcatresult[index].gfacerr = "0.000";
    photcatresult[index].bfac    = QString::number(meanMask_T(bCorr),'f',3);
    photcatresult[index].bfacerr = QString::number(rmsMask_T(bCorr),'f',3);
    photcatresult[index].nstars = QString::number(rCorr.length());

    writeG2refcat(REFCAT->name, matchedREFCAT);

    QString nstars = QString::number(matchedREFCAT.length()) + " stars";
    if (REFCAT->name == "PANSTARRS") ui->numPANSTARRSLabel->setText(nstars);
    else if (REFCAT->name == "SDSS") ui->numSDSSLabel->setText(nstars);
    else if (REFCAT->name == "SKYMAPPER") ui->numSKYMAPPERLabel->setText(nstars);
    else if (REFCAT->name == "APASS") ui->numAPASSLabel->setText(nstars);
    else if (REFCAT->name == "AVGWHITE") ui->numAVGWHITELabel->setText(nstars);

    QString type = "note";
    if (matchedREFCAT.length() == 0) type = "warning";
    emit messageAvailable(REFCAT->name + " : "+QString::number(matchedREFCAT.length()) + " of them were detected in the image.", type);
}

void ColorPicture::writeG2refcat(const QString refcatName, const QVector<QVector<double>> matchedREFCAT)
{
    // The iView catalog (ASCII)
    QString outpath = mainDir+"/color_theli/";
    QDir outdir(outpath);
    if (!outdir.exists()) outdir.mkpath(outpath);
    QFile outcat_iview(outpath+"/PHOTCAT_calibration/PHOTCAT_sources_matched_"+refcatName+".iview");
    QTextStream stream_iview(&outcat_iview);
    if( !outcat_iview.open(QIODevice::WriteOnly)) {
        emit messageAvailable("Query::processPhotomCatalog(): ERROR writing "+outpath+outcat_iview.fileName()+" : "+outcat_iview.errorString(), "error");
        emit criticalReceived();
        return;
    }

    // Write iView catalog
    for (auto &source : matchedREFCAT) {
        // RA first, then DEC!  (matching is done with DEC in first column)
        stream_iview << QString::number(source[1], 'f', 9) << " " << QString::number(source[0], 'f', 9) << "\n";
    }
    outcat_iview.close();
    outcat_iview.setPermissions(QFile::ReadUser | QFile::WriteUser);
}

void ColorPicture::colorCalibRetrieveCatalogs(QList<Query*> queryList)
{
    emit messageAvailable("Querying reference sources, this may take a while ...", "ignore");

    // Collector for meta data from the queries
    QStringList queryResult;
    for (int i=0; i<queryList.length(); ++i) queryResult << "";

    // It appears that the parallel query is not threadsafe.
    // Results in not reproducible errors, at least when run through Qt5 debugger (but when run through valgrind)
    //#pragma omp parallel for num_threads(maxCPU)
    for (int i=0; i<queryList.length(); ++i) {
        auto &query = queryList[i];
        query->photomDir = ui->dirLineEdit->text() + "/color_theli/";
        query->photomImage = croppedList[0];
        query->doColorCalibQueryFromWeb();
        query->identifySolarTypeStars();
        QString info = "G2 sources " + query->refcatName + " : " + QString::number(query->numG2sources) + " (out of : " + QString::number(query->numSources) + ")";
        queryResult[i] = info;
        emit messageAvailable(info, "append");
        //        emit updateNrefStars(query->refcatName, query->numG2sources);
    }
}

void ColorPicture::filterSolarTypeStars(QList<Query*> queryList)
{
    PANSTARRS->clear();
    SDSS->clear();
    SKYMAPPER->clear();
    APASS->clear();

    for (auto &query : queryList) {
        if (query->numSources == 0) continue;
        for (long k=0; k<query->mag1_out.length(); ++k) {
            if (!query->G2type[k]) continue;
            if (query->refcatName == "PANSTARRS") {
                PANSTARRS->ra.append(query->ra_out[k]);
                PANSTARRS->de.append(query->de_out[k]);
            }
            if (query->refcatName == "SDSS") {
                SDSS->ra.append(query->ra_out[k]);
                SDSS->de.append(query->de_out[k]);
            }
            if (query->refcatName == "SKYMAPPER") {
                SKYMAPPER->ra.append(query->ra_out[k]);
                SKYMAPPER->de.append(query->de_out[k]);
            }
            if (query->refcatName == "APASS") {
                APASS->ra.append(query->ra_out[k]);
                APASS->de.append(query->de_out[k]);
            }
        }
    }
}

void ColorPicture::colorCalibSegmentImages()
{
    emit messageAvailable("Detecting sources ...", "ignore");

    // Create object catalogs, get matching tolerance
    QString DT = ui->DTLineEdit->text();
    QString DMIN = ui->DMINLineEdit->text();
#pragma omp parallel for num_threads(maxCPU)
    for (int i=0; i<croppedList.length(); ++i) {
        auto &it = croppedList[i];
        // only do source detection if the image is also present in one of the RGB channels:
        if (it->name != ui->redComboBox->currentText() &&
                it->name != ui->greenComboBox->currentText() &&
                it->name != ui->blueComboBox->currentText()) {
            continue;
        }
        // Do nothing if we have the catalog already
        if (it->segmentationDone) continue;
        // Obtain catalog
        //        emit messageAvailable("Detecting sources in " + it->baseName +" ...", "ignore");
        it->maxCPU = maxCPU / croppedList.length();
        it->resetObjectMasking();
        it->readImage(it->path + "/" +it->name);
        it->readWeight();
        it->backgroundModel(100, "interpolate");
        it->segmentImage(DT, DMIN, true, false);
        it->estimateMatchingTolerance();
        it->matchingTolerance *= 3.;
        it->releaseBackgroundMemory();
        it->releaseDetectionPixelMemory();

        QString channelName = "";
        if (it->name == ui->redComboBox->currentText()) channelName = "R";
        if (it->name == ui->greenComboBox->currentText()) channelName = "G";
        if (it->name == ui->blueComboBox->currentText()) channelName = "B";
        emit messageAvailable(channelName + " : " + QString::number(it->objectList.length()) + " sources", "ignore");
    }
}

void ColorPicture::updateNrefStarsReceived(QString name, long number)
{
    QString nstars = QString::number(number) + " stars";
    if (name == "PANSTARRS") ui->numPANSTARRSLabel->setText(nstars);
    else if (name == "SDSS") ui->numSDSSLabel->setText(nstars);
    else if (name == "SKYMAPPER") ui->numSKYMAPPERLabel->setText(nstars);
    else if (name == "APASS") ui->numAPASSLabel->setText(nstars);
    else if (name == "AVGWHITE") ui->numAVGWHITELabel->setText(nstars);
}
