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

#include "tiffio.h"

#include <omp.h>
#include <QStringList>
#include <QStringListModel>
#include <QDateTime>
#include <QTest>
#include <QMessageBox>

void ColorPicture::on_createTiffPushButton_clicked()
{
    ui->createTiffPushButton->setText("Running ...");
    ui->createTiffPushButton->setDisabled(true);
    QTest::qWait(50);

    workerThread = new QThread();
    colorpictureWorker = new ColorPictureWorker(this, "Fits2Tiff");
    colorpictureWorker->moveToThread(workerThread);
    connect(workerThread, &QThread::started, colorpictureWorker, &ColorPictureWorker::runTask);
    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater, Qt::DirectConnection);
    connect(colorpictureWorker, &ColorPictureWorker::finished, workerThread, &QThread::quit, Qt::DirectConnection);
    connect(colorpictureWorker, &ColorPictureWorker::finished, colorpictureWorker, &QObject::deleteLater, Qt::DirectConnection);
    connect(colorpictureWorker, &ColorPictureWorker::messageAvailable, this, &ColorPicture::displayMessage);
    connect(colorpictureWorker, &ColorPictureWorker::tasknameReturned, this, &ColorPicture::taskFinished);
    workerThread->start();
}

void ColorPicture::taskInternalFits2Tiff()
{
    int nrows = ui->statisticsTableWidget->rowCount();
    int ncols = ui->statisticsTableWidget->columnCount();
    // Leave if the table hasn't been initialized
    if (ncols != 5 || !statisticsRetrieved) return;

    QString bfac = ui->blueFactorLineEdit->text();
    QString rfac = ui->redFactorLineEdit->text();
    if (bfac.isEmpty()) bfac = "1.0";
    if (rfac.isEmpty()) rfac = "1.0";

    // retrieve the black and white points for the RGB channels (must have the same range)
    float maxMax = 0.;
    float minMin = 0.;
    float rescale = 1.0;
    for (int i=0; i<nrows; ++i) {
        QString name = ui->statisticsTableWidget->item(i,0)->data(Qt::DisplayRole).toString();
        float min = ui->statisticsTableWidget->item(i,3)->data(Qt::DisplayRole).toFloat();
        float max = ui->statisticsTableWidget->item(i,4)->data(Qt::DisplayRole).toFloat();
        if (name == ui->redComboBox->currentText()) rescale = rfac.toFloat();
        if (name == ui->blueComboBox->currentText()) rescale = bfac.toFloat();
        // Clip only the RGB images to the same min/max values
        if (name == ui->redComboBox->currentText()
                || name == ui->greenComboBox->currentText()
                || name == ui->blueComboBox->currentText()) {
            if (rescale*max > maxMax) maxMax = rescale*max;
            if (rescale*min < minMin) minMin = rescale*min;
        }
    }

    // Convert FITS to TIFF
#pragma omp parallel for num_threads(maxCPU)
    for (int i=0; i<nrows; ++i) {
        QString name = coaddImageModel->index(i,0).data().toString();
        MyImage *myImage = identifyCroppedMyImage(name);
        if (myImage == nullptr) continue;
        float factor = 1.0;
        if (name == ui->redComboBox->currentText()) factor = ui->redFactorLineEdit->text().toFloat();
        else if (name == ui->blueComboBox->currentText()) factor = ui->blueFactorLineEdit->text().toFloat();
        else factor = 1.0;
        float medVal = ui->statisticsTableWidget->item(i,1)->data(Qt::DisplayRole).toFloat();
        myImage->subtract(medVal);
        myImage->multiply(factor);
        myImage->toTIFF(16, minMin, maxMax);
        myImage->writeImage(myImage->path + "/"+myImage->baseName + "_2tiff.fits");
        emit messageAvailable("TIFF written to " + myImage->path + "/"+myImage->baseName + ".tiff", "info");
    }
}

// Implementation not finished.
void ColorPicture::writeRGBTIFF(QVector<float> &R, QVector<float> &G, QVector<float> &B, long n, long m, float min, float max, QString path)
{
    emit messageAvailable("Creating RGB.tiff ...", "ignore");

    // Clipping min and max values
    QVector<QVector<float>> RGBlist;
    RGBlist << R << G << B;

#pragma omp parallel for
    for (int i=0; i<3; ++i) {
        for (auto &pixel : RGBlist[i]) {
            if (pixel <= min) pixel = min;
            if (pixel >= max) pixel = max;
        }
    }

    float grey = 0.;  // inactive
    grey = grey / 100. * 65000.;
    float blowup = (65000. - grey) / (max - min);

    std::vector< std::vector<long> > imtiff(n);
    for (long i=0; i<n; ++i) {
        imtiff[i].resize(m,0);
    }

#pragma omp parallel for
    for (int img=0; img<3; ++img) {
        for (long i=0; i<n; ++i)  {
            for (long j=0; j<m; ++j)  {
                RGBlist[img][i+n*j] = blowup * (RGBlist[img][i+n*j] - min) + grey;
                // flipping TIFF in y dir
                imtiff[i][m-j-1] = (long) RGBlist[img][i+n*j];
            }
        }
    }

    QString outname = path+"/RGB.tiff";
    TIFF     *outtiff;             // pointer to the TIFF file, defined in tiffio.h
    outtiff = TIFFOpen(outname.toUtf8().data(), "w");
    TIFFSetField(outtiff, TIFFTAG_IMAGEWIDTH, n);
    TIFFSetField(outtiff, TIFFTAG_IMAGELENGTH, m);
    TIFFSetField(outtiff, TIFFTAG_COMPRESSION, 1);
    TIFFSetField(outtiff, TIFFTAG_BITSPERSAMPLE, 16);
    TIFFSetField(outtiff, TIFFTAG_SAMPLESPERPIXEL, 3);
    TIFFSetField(outtiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(outtiff, TIFFTAG_PLANARCONFIG, 1);       // store pixel values as sequential RGB RGB RGB
    TIFFSetField(outtiff, TIFFTAG_SOFTWARE, "THELI");
    TIFFSetField(outtiff, TIFFTAG_IMAGEDESCRIPTION, "Created by THELI");

    uint16 *outbuf;
    uint16 *outb;
    outbuf = (uint16 *)_TIFFmalloc(TIFFScanlineSize(outtiff));
    for (long row=0; row<m; ++row) {
        outb = outbuf;
        for (long column=0; column<n; ++column) {
            *outb++ = (uint16) (imtiff[column][row]);
        }
        TIFFWriteScanline(outtiff, outbuf, row, 0);
    }
    TIFFClose(outtiff);
    _TIFFfree(outbuf);
}
