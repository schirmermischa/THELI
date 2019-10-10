#include "colorpicture.h"
#include "ui_colorpicture.h"
#include "../functions.h"
#include "../tools/tools.h"
#include "../query/query.h"
#include "../myfits/myfits.h"
#include "../tools/cfitsioerrorcodes.h"

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
