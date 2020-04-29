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

void ColorPicture::on_getCoaddsPushButton_clicked()
{
    // FIRST, fetch the selected images. This is fast because we link only, and hence we don't need an external thread.
    QItemSelectionModel *selectionModel = ui->coaddDirListView->selectionModel();
    QStringList list = coaddDirModel->stringList();
    QStringList imageList;
    QString dirName = ui->dirLineEdit->text();
    for (int i=0; i<ui->coaddDirListView->model()->rowCount(); ++i) {
        QModelIndex index = ui->coaddDirListView->model()->index(i,0);
        if (selectionModel->isSelected(index)) imageList.append(list.at(i));
    }
    // Stop if no images were selected
    if (imageList.length() < 2) {
        emit messageAvailable("At least two images must be chosen from the list of coadded images.", "warning");
        return;
    }

    emit messageAvailable("Linking selected coadded images to "+dirName+"/color_theli/", "info");

    QPalette palette;
    palette.setColor(QPalette::Base,QColor("#a9ffe6"));
    ui->getCoaddsPushButton->setText("Running ...");
    ui->getCoaddsPushButton->setPalette(palette);
    ui->getCoaddsPushButton->setDisabled(true);
    QTest::qWait(50);

    // Create the directory containing the images
    QDir colorTheli(dirName+"/color_theli");
    if (colorTheli.exists()) {
        colorTheli.rename(dirName+"/color_theli",
                          dirName+"/color_theli_"+QDateTime::currentDateTime().toString(Qt::ISODate));
    }
    colorTheli.mkdir(dirName+"/color_theli");

    // Link the images
    QString coaddImageName;
    QString coaddWeightName;
    QFile coaddImage;
    QFile coaddWeight;
    QString identifier;

    QStringList coaddImageList;

    for (auto &it : imageList) {
        if (it.contains("coadd_")) {
            identifier = it.split("coadd_").at(1);
        }
        else continue;
        coaddImageName = dirName+"/"+it+"/coadd.fits";
        coaddWeightName = dirName+"/"+it+"/coadd.weight.fits";
        coaddImage.setFileName(coaddImageName);
        coaddWeight.setFileName(coaddWeightName);
        if (coaddImage.exists()) {
            coaddImage.link(dirName+"/color_theli/"+identifier+".fits");
            coaddImageList.append(identifier+".fits");
            // populate the coaddImage listView
            coaddImageModel->setStringList(coaddImageList);
        }
        if (coaddWeight.exists()) coaddWeight.link(dirName+"/color_theli/"+identifier+".weight.fits");
    }

    coaddImageModel->sort(0);

    // Next, crop the images to their maximum overlap
    // Reset the button now, so we don't have to do it from within another thread
    buttonPalette = ui->getCoaddsPushButton->palette();
    ui->getCoaddsPushButton->setText("Cropping images, please wait ...");
    ui->getCoaddsPushButton->setDisabled(true);

    QTest::qWait(50);

    workerThread = new QThread();
    colorpictureWorker = new ColorPictureWorker(this, "CropCoadds");
    colorpictureWorker->moveToThread(workerThread);
    connect(workerThread, &QThread::started, colorpictureWorker, &ColorPictureWorker::runTask);
    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater, Qt::DirectConnection);
    connect(colorpictureWorker, &ColorPictureWorker::finished, workerThread, &QThread::quit, Qt::DirectConnection);
    connect(colorpictureWorker, &ColorPictureWorker::finished, colorpictureWorker, &QObject::deleteLater, Qt::DirectConnection);
    connect(colorpictureWorker, &ColorPictureWorker::messageAvailable, this, &ColorPicture::displayMessage);
    connect(colorpictureWorker, &ColorPictureWorker::tasknameReturned, this, &ColorPicture::taskFinished);
    workerThread->start();
}

void ColorPicture::taskInternalCropCoadds()
{
    QString dirName = ui->dirLineEdit->text() + "/color_theli/";
    QDir colorTheli(dirName);
    QStringList filter("*.fits");
    QStringList imageList = colorTheli.entryList(filter);
    coaddList.clear();
    for (auto &it : imageList) {
        QVector<bool> dummyMask;
        dummyMask.clear();
        MyImage *myImage = new MyImage(dirName+it, dummyMask, &verbosity);
        connect(myImage, &MyImage::messageAvailable, this, &ColorPicture::displayMessage);
        connect(myImage, &MyImage::critical, this, &ColorPicture::criticalReceived);
        myImage->globalMaskAvailable = false;
        myImage->maxCPU = maxCPU / imageList.length();
        myImage->provideHeaderInfo();
        coaddList.append(myImage);
    }

    QVector<float> crpix1;
    QVector<float> crpix2;
    QVector<float> naxis1;
    QVector<float> naxis2;
    QVector<float> d1;
    QVector<float> d2;
    for (int i=0; i<coaddList.length(); ++i) {
        auto &it = coaddList[i];
        crpix1 << it->wcs->crpix[0];
        crpix2 << it->wcs->crpix[1];
        naxis1 << it->naxis1;
        naxis2 << it->naxis2;
        d1 << it->naxis1 - it->wcs->crpix[0];
        d2 << it->naxis2 - it->wcs->crpix[1];
    }
    float xlow = -1.*minVec_T(crpix1);
    float ylow = -1.*minVec_T(crpix2);
    float xhigh = minVec_T(d1);
    float yhigh = minVec_T(d2);

    CfitsioErrorCodes *errorCodes = new CfitsioErrorCodes();

#pragma omp parallel for num_threads(maxCPU)
    for (int i=0; i<coaddList.length(); ++i) {
        auto &it = coaddList[i];
        emit messageAvailable("Cropping "+it->name, "ignore");
        // crop image
        float xlowNew = xlow + crpix1[i];
        float ylowNew = ylow + crpix2[i];
        float xhighNew = xhigh + crpix1[i] - 1;
        float yhighNew = yhigh + crpix2[i] - 1;
        QString tmpName = it->name;
        QString name;
        if (it->name.contains(".weight.fits")) {
            tmpName.remove(".weight.fits");
            name = tmpName + "_cropped.weight.fits";
            it->baseName = tmpName + "_cropped.weight.fits";
        }
        else {
            name = it->baseName + "_cropped.fits";
        }
        it->readImage(false);
        it->makeCutout(xlowNew, xhighNew, ylowNew, yhighNew);
        it->writeImage(dirName + name);
        it->name = name;
        // update fits header
        int status = 0;
        float crpix1New = crpix1[i] - xlowNew + 1;
        float crpix2New = crpix2[i] - ylowNew + 1;
        fitsfile *fptr = nullptr;
        fits_open_file(&fptr, (dirName+name).toUtf8().data(), READWRITE, &status);
        fits_update_key_flt(fptr, "CRPIX1", crpix1New, 6, nullptr, &status);
        fits_update_key_flt(fptr, "CRPIX2", crpix2New, 6, nullptr, &status);
        fits_close_file(fptr, &status);

        QString code = errorCodes->errorKeyMap.value(status);
        if (status) {
            emit messageAvailable("CFITSIO: Could not update CRPIX keywords in "+name+":<br>"+code, "error");
        }
    }
    delete errorCodes;

    // Prepare for segmentation
    // Move the cropped weights over into the cropped coadd
    croppedList.clear();
    for (auto &coadd : coaddList) {
        if (coadd->name.contains("weight.fits")) continue;
        QString baseCoadd = coadd->name;
        baseCoadd.remove("_cropped.fits");
        for (auto &weight : coaddList) {
            if (!weight->name.contains("weight.fits")) continue;
            QString baseWeight = weight->name;
            baseWeight.remove("_cropped.weight.fits");
            if (baseCoadd == baseWeight) {
                coadd->dataWeight.swap(weight->dataCurrent);
                coadd->weightInMemory = true;
                coadd->globalMaskAvailable = false;
                croppedList.append(coadd);
            }
        }
    }
    emit messageAvailable("Done.", "ignore");
}

