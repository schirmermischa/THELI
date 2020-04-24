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
#include "../instrumentdata.h"
#include "../preferences.h"
#include "../tools/tools.h"
#include "../myfits/myfits.h"
#include "../tools/cfitsioerrorcodes.h"

#include <omp.h>
#include <QStringList>
#include <QStringListModel>
#include <QDateTime>
#include <QTest>
#include <QMessageBox>

void ColorPicture::on_getRatioPushButton_clicked()
{
    ui->filterRatioLineEdit->setText("");

    QDir colorCalibDir(mainDir+"/color_theli/PHOTCAT_calibration/");
    if (! colorCalibDir.exists()) {
        if (!colorCalibDir.mkdir(mainDir+"/color_theli/PHOTCAT_calibration/")) {
            emit messageAvailable("Could not create "+mainDir+"/color_theli/PHOTCAT_calibration/", "error");
            return;
        }
    }

    ui->getRatioPushButton->setText("Running ...");
    ui->getRatioPushButton->setDisabled(true);
    ui->narrowbandComboBox->setDisabled(true);
    ui->broadbandComboBox->setDisabled(true);
    ui->BBNBcombinePushButton->setDisabled(true);

    QTest::qWait(50);

    QString n_image = ui->narrowbandComboBox->currentText();
    QString b_image = ui->broadbandComboBox->currentText();

    // leave conditions
    if (n_image == b_image) {
        emit messageAvailable("You must choose different images to combine.", "warning");
        ui->getRatioPushButton->setText("Estimate flux ratio");
        ui->getRatioPushButton->setEnabled(true);
        ui->narrowbandComboBox->setEnabled(true);
        ui->broadbandComboBox->setEnabled(true);
        ui->BBNBcombinePushButton->setEnabled(true);
        return;
    }

    workerThread = new QThread();
    colorpictureWorker = new ColorPictureWorker(this, "BBNBratio");
    colorpictureWorker->moveToThread(workerThread);
    connect(workerThread, &QThread::started, colorpictureWorker, &ColorPictureWorker::runTask);
    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater, Qt::DirectConnection);
    connect(colorpictureWorker, &ColorPictureWorker::finished, workerThread, &QThread::quit, Qt::DirectConnection);
    connect(colorpictureWorker, &ColorPictureWorker::finished, colorpictureWorker, &QObject::deleteLater, Qt::DirectConnection);
    connect(colorpictureWorker, &ColorPictureWorker::messageAvailable, this, &ColorPicture::displayMessage);
    connect(colorpictureWorker, &ColorPictureWorker::tasknameReturned, this, &ColorPicture::taskFinished);
    workerThread->start();
}

void ColorPicture::taskInternalBBNBratio()
{
    MyImage *bbImage = identifyCroppedMyImage(ui->broadbandComboBox->currentText());
    MyImage *nbImage = identifyCroppedMyImage(ui->narrowbandComboBox->currentText());

    if (bbImage == nullptr || nbImage == nullptr) return;   // This should never happen

    QList<MyImage*> bbnbList;
    bbnbList << bbImage << nbImage;

    // Create object catalogs
#pragma omp parallel for num_threads(maxCPU)
    for (int i=0; i<bbnbList.length(); ++i) {
        auto &it = bbnbList[i];
        // Do nothing if we have the catalog already
        if (it->segmentationDone) continue;
        // Obtain catalog
        emit messageAvailable("Detecting sources in " + it->baseName +" ...", "ignore");
        it->maxCPU = maxCPU / bbnbList.length();
        it->resetObjectMasking();
        it->readImage();
        it->readWeight();
        it->backgroundModel(100, "interpolate");
        it->segmentImage("5","5", true, false);
        it->estimateMatchingTolerance();
        it->releaseBackgroundMemory();
        it->releaseAllDetectionMemory();
    }

    emit messageAvailable("Matching broadband and narrowband catalogs ...", "ignore");
    QVector<QVector<double>> objDataBB = getObjectData(bbImage);
    QVector<QVector<double>> objDataNB = getObjectData(nbImage);
    QVector<QVector<double>> matched;
    float tolerance = (bbImage->matchingTolerance + nbImage->matchingTolerance) / 2.;
    int dummy1;
    int dummy2;
    match2D(objDataNB, objDataBB, matched, tolerance, dummy1, dummy2, maxCPU);

    // Calculate mean flux ratio
    QVector<float> fluxRatios;   // compute flux_narrowband / flux_broadband
    fluxRatios.reserve(matched.length());
    for (auto &obj : matched) {
        // Skip objects with negative fluxes (could be OK though if background is oversubtracted)
        // CAREFUL: ORDER in 'obj': RA DE FLUX_BB FLUX_NB
        // match2D inserts the properties of the reference catalog 'in between'
        if (obj[2] <= 0. || obj[3] <= 0.) continue;
        fluxRatios.append(obj[3] / obj[2]);
    }
    if (fluxRatios.length() <= 1) {
        bbnbFluxRatio = 1.0;
        bbnbFluxRatioError = 0.0;
    }
    bbnbFluxRatio = straightMedian_T(fluxRatios);
    bbnbFluxRatioError = rmsMask_T(fluxRatios) / sqrt(fluxRatios.length());
    emit messageAvailable("Done.", "ignore");
}

void ColorPicture::on_BBNBcombinePushButton_clicked()
{
    QString BBimage = ui->broadbandComboBox->currentText();
    QString NBimage = ui->narrowbandComboBox->currentText();

    // leave conditions
    if (BBimage == NBimage) {
        emit messageAvailable("You must choose two different images for this task.", "warning");
        ui->getRatioPushButton->setText("Estimate flux ratio");
        ui->getRatioPushButton->setEnabled(true);
        ui->narrowbandComboBox->setEnabled(true);
        ui->broadbandComboBox->setEnabled(true);
        ui->BBNBcombinePushButton->setEnabled(true);
        return;
    }

    ui->BBNBcombinePushButton->setText("Running...");
    ui->BBNBcombinePushButton->setDisabled(true);
    QTest::qWait(50);

    workerThread = new QThread();
    colorpictureWorker = new ColorPictureWorker(this, "BBNBcombine");
    colorpictureWorker->moveToThread(workerThread);
    connect(workerThread, &QThread::started, colorpictureWorker, &ColorPictureWorker::runTask);
    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater, Qt::DirectConnection);
    connect(colorpictureWorker, &ColorPictureWorker::finished, workerThread, &QThread::quit, Qt::DirectConnection);
    connect(colorpictureWorker, &ColorPictureWorker::finished, colorpictureWorker, &QObject::deleteLater, Qt::DirectConnection);
    connect(colorpictureWorker, &ColorPictureWorker::messageAvailable, this, &ColorPicture::displayMessage);
    connect(colorpictureWorker, &ColorPictureWorker::tasknameReturned, this, &ColorPicture::taskFinished);
    workerThread->start();
}

void ColorPicture::taskInternalBBNBcombine()
{
    MyImage *bbImage = identifyCroppedMyImage(ui->broadbandComboBox->currentText());
    MyImage *nbImage = identifyCroppedMyImage(ui->narrowbandComboBox->currentText());

    if (bbImage == nullptr || nbImage == nullptr) return;   // This should never happen

    emit messageAvailable("Combining narrowband and broadband images into: "
                          +bbImage->baseName + nbImage->baseName + "_cropped.fits", "ignore");

    long n = bbImage->naxis1;
    long m = bbImage->naxis2;

    QString path = bbImage->path + "/";

    MyImage *combinedImage = new MyImage(path, bbImage->name, "", 1, QVector<bool>(), &verbosity, false);
    MyImage *combinedWeight = new MyImage(path, bbImage->name, "", 1, QVector<bool>(), &verbosity, false);
    combinedImage->provideHeaderInfo();
    combinedWeight->provideHeaderInfo();

    // Compute and write the combined image as follows:
    // newImage = (1 - weight * width(NB) / width(BB)) * BB + weight * NB;
    float weight = ui->narrowbandWeightLineEdit->text().toFloat();
    if (ui->narrowbandWeightLineEdit->text().isEmpty()) weight = 1.0;
    float scale = 1. - weight * bbnbFluxRatio;
    combinedImage->dataCurrent.resize(n*m);
    combinedWeight->dataCurrent.resize(n*m);
    for (long i=0; i<n*m; ++i) {
        combinedImage->dataCurrent[i] = scale * bbImage->dataCurrent[i] + weight * nbImage->dataCurrent[i];
        combinedWeight->dataCurrent[i] = bbImage->dataWeight[i] + nbImage->dataWeight[i];
    }
    combinedImage->writeImage(path+bbImage->baseName + nbImage->baseName + "_cropped.fits");
    combinedWeight->writeImage(path+bbImage->baseName + nbImage->baseName + "_cropped.weight.fits");

    delete combinedImage;
    delete combinedWeight;
}
