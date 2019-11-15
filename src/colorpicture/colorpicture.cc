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
#include "../iview/iview.h"
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

ColorPicture::ColorPicture(instrumentDataType *instrumentData, QString main, QWidget *parent) :
    QMainWindow(parent),
    mainDir(main),
    ui(new Ui::ColorPicture)
{
    ui->setupUi(this);
    initEnvironment(thelidir, userdir, tmpdir);
    instData = instrumentData;

    // Model views
    coaddDirModel = new QStringListModel(this);
    coaddImageModel = new QStringListModel(this);
    ui->coaddDirListView->setModel(coaddDirModel);
    ui->coaddImageListView->setModel(coaddImageModel);
    ui->redComboBox->setModel(coaddImageModel);
    ui->greenComboBox->setModel(coaddImageModel);
    ui->blueComboBox->setModel(coaddImageModel);
    ui->broadbandComboBox ->setModel(coaddImageModel);
    ui->narrowbandComboBox->setModel(coaddImageModel);

    ui->dirLineEdit->setText(mainDir);
    paintPathLineEdit(ui->dirLineEdit, mainDir, "dir");
    addDirectories();

    connect(ui->blueFactorLineEdit, &QLineEdit::textChanged, this, &ColorPicture::validate );
    connect(ui->redFactorLineEdit, &QLineEdit::textChanged, this, &ColorPicture::validate );
    connect(ui->filterRatioLineEdit, &QLineEdit::textChanged, this, &ColorPicture::validate );
    connect(ui->narrowbandWeightLineEdit, &QLineEdit::textChanged, this, &ColorPicture::validate );

    connect(ui->resultSDSSPushButton, &QPushButton::clicked, this, &ColorPicture::toggleCalibResult );
    connect(ui->resultPANSTARRSPushButton, &QPushButton::clicked, this, &ColorPicture::toggleCalibResult );
    connect(ui->resultAPASSPushButton, &QPushButton::clicked, this, &ColorPicture::toggleCalibResult );
    connect(ui->resultAVGWHITEPushButton, &QPushButton::clicked, this, &ColorPicture::toggleCalibResult );

    connect(ui->redFactorLineEdit, &QLineEdit::textEdited, this, &ColorPicture::sendColorFactors);
    connect(ui->blueFactorLineEdit, &QLineEdit::textEdited, this, &ColorPicture::sendColorFactors);
    connect(ui->resultSDSSPushButton, &QPushButton::clicked, this, &ColorPicture::sendColorFactors);
    connect(ui->resultPANSTARRSPushButton, &QPushButton::clicked, this, &ColorPicture::sendColorFactors);
    connect(ui->resultAPASSPushButton, &QPushButton::clicked, this, &ColorPicture::sendColorFactors);
    connect(ui->resultAVGWHITEPushButton, &QPushButton::clicked, this, &ColorPicture::sendColorFactors);

    connect(ui->getStatisticsPushButton, &QPushButton::clicked, this, &ColorPicture::on_previewCalibPushButton_clicked);

    connect(this, &ColorPicture::messageAvailable, this, &ColorPicture::displayMessage);

    connect(this, &ColorPicture::updateNrefStars, this, &ColorPicture::updateNrefStarsReceived);

    ui->tabWidget->setCurrentIndex(0);

    resultButtonGroup->setExclusive(true);
    resultButtonGroup->addButton(ui->resultSDSSPushButton);
    resultButtonGroup->addButton(ui->resultPANSTARRSPushButton);
    resultButtonGroup->addButton(ui->resultAPASSPushButton);
    resultButtonGroup->addButton(ui->resultAVGWHITEPushButton);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor("#c6c6c6"));
    ui->statusbar->setPalette(palette);

    setWindowIcon(QIcon(":/images/color.png"));

    loadPreferences();

    // Init queries
    SDSSquery->refcatName = "SDSS";
    APASSquery->refcatName = "APASS";
    PANSTARRSquery->refcatName = "PANSTARRS";
    connect(SDSSquery, &Query::messageAvailable, this, &ColorPicture::displayMessage);
    connect(APASSquery, &Query::messageAvailable, this, &ColorPicture::displayMessage);
    connect(PANSTARRSquery, &Query::messageAvailable, this, &ColorPicture::displayMessage);
}

ColorPicture::~ColorPicture()
{
    delete SDSSquery;
    delete APASSquery;
    delete PANSTARRSquery;

    delete ui;
}

void ColorPicture::updateVerbosity(int verbosityLevel)
{
    verbosity = verbosityLevel;
}

void ColorPicture::loadPreferences()
{
    QSettings settings("THELI", "PREFERENCES");
    maxCPU = settings.value("prefCPUSpinBox").toInt();
    //    verbosity = settings.value("prefVerbosityComboBox").toInt();
    verbosity = 0;
}

void ColorPicture::displayMessage(QString message, QString type)
{
    if (type == "error") {
        ui->processingTextEdit->appendHtml("<font color=\"#ee0000\">ERROR: " + message + "</font>");
    }
    else if (type == "warning") {
        ui->processingTextEdit->appendHtml("<font color=\"#ee5500\">" + message + "</font>");
    }
    else if (type == "info") {
        ui->processingTextEdit->appendHtml("<font color=\"#0000dd\">" + message + "</font>");
    }
    else if (type == "append") {
        ui->processingTextEdit->moveCursor(QTextCursor::End);
        ui->processingTextEdit->appendHtml(" "+message);
        ui->processingTextEdit->moveCursor(QTextCursor::End);
    }
    else ui->processingTextEdit->appendHtml(message);
}

void ColorPicture::criticalReceived()
{
    emit finished();
}

void ColorPicture::checkCalibrationFactor(QLineEdit *le)
{
    if (le->text().isEmpty()) le->setText("1.000");
}

void ColorPicture::sendColorFactors()
{
    checkCalibrationFactor(ui->redFactorLineEdit);
    checkCalibrationFactor(ui->blueFactorLineEdit);
    // green is always 1.0
    float redFactor = ui->redFactorLineEdit->text().toFloat();
    float blueFactor = ui->blueFactorLineEdit->text().toFloat();
    emit colorFactorChanged(redFactor, blueFactor);
}

void ColorPicture::on_selectDirPushButton_clicked()
{
    // get parent directory

    QFileDialog qfd(this);
    qfd.setFileMode(QFileDialog::DirectoryOnly);
    qfd.setOption(QFileDialog::ShowDirsOnly);
    qfd.setDirectory(mainDir);
    qfd.setWindowTitle(tr("Select directory"));
    QString dirName;
    if (qfd.exec()) dirName = qfd.selectedFiles().at(0);

    ui->dirLineEdit->setText(dirName);
    paintPathLineEdit(ui->dirLineEdit, dirName, "dir");

    addDirectories();
}

void ColorPicture::addDirectories()
{
    QString dirName = ui->dirLineEdit->text();
    if (!QDir(dirName).exists()) return;

    // find all coadd_xxx subdirectories
    QStringList subDirList;
    findRecursion(dirName, &subDirList);

    QStringList coaddList;
    // Keep coadd_xxx only
    for (auto &it : subDirList) {
        QString tmp = it.remove(0, dirName.length());
        if (tmp.startsWith('/')) tmp = tmp.remove(0,1);
        if (tmp.startsWith("coadd_") || tmp.contains("/coadd_")) {
            coaddList.append(tmp);
        }
    }
    coaddDirModel->setStringList(coaddList);
}

void ColorPicture::findRecursion(const QString &path, QStringList *result)
{
    // recursive
    QDir currentDir(path);
    const QString prefix = path + QLatin1Char('/');
    foreach (const QString &match, currentDir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot))
        result->append(prefix + match);
    foreach (const QString &dir, currentDir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot))
        findRecursion(prefix + dir, result);
}

void ColorPicture::refreshComboBoxes()
{
    QStringList filters;
    filters << "*cropped.fits";
    QString dirName = ui->dirLineEdit->text();
    QStringList fileList = QDir(dirName+"/color_theli/").entryList(filters);
    coaddImageModel->setStringList(fileList);
    coaddImageModel->sort(0);

    ui->redComboBox->setCurrentIndex(0);
    ui->greenComboBox->setCurrentIndex(0);
    ui->blueComboBox->setCurrentIndex(0);
    ui->broadbandComboBox->setCurrentIndex(0);
    ui->narrowbandComboBox->setCurrentIndex(0);
}

void ColorPicture::taskFinished(QString taskname)
{
    if (taskname == "CropCoadds") updateImageListView();
    if (taskname == "ColorCalib") updateCalibFactors();
    if (taskname == "BBNBratio") updateFilterRatio();
    if (taskname == "BBNBcombine") updateBBNBcombine();
    if (taskname == "Fits2Tiff") updateTiff();
}

void ColorPicture::updateTiff()
{
    ui->createTiffPushButton->setEnabled(true);
    showDone(ui->createTiffPushButton, "(4) Create TIFFs");
    emit messageAvailable("Done.", "ignore");
}

void ColorPicture::updateImageListView()
{
    // Repopulate the comboboxes
    refreshComboBoxes();

    buttonPalette.setColor(QPalette::Button, QColor("#44d8cc"));
    ui->getCoaddsPushButton->setPalette(buttonPalette);
    ui->getCoaddsPushButton->setEnabled(true);
    showDone(ui->getCoaddsPushButton, "Get selected coadded images");
}

void ColorPicture::on_clearCoaddsPushButton_clicked()
{
    coaddImageModel->removeRows(0, coaddImageModel->rowCount());
}

void ColorPicture::validate()
{
    // Floating point validators
    QRegExp rf( "^[-]{0,1}[0-9]+[.]{0,1}[0-9]+" );
    QValidator* validator = new QRegExpValidator( rf, this );
    ui->blueFactorLineEdit->setValidator( validator );
    ui->redFactorLineEdit->setValidator( validator );
    ui->filterRatioLineEdit->setValidator( validator );
    ui->narrowbandWeightLineEdit->setValidator( validator );
}


void ColorPicture::showDone(QPushButton *pushButton, QString text)
{
    //    pushButton->setText("Done");
    //    QTest::qWait(1000);
    pushButton->setText(text);
}

void ColorPicture::updateCalibFactors()
{
    // Select the best result (the ones with the most matches, other than AVGWHITE
    int maxStars = 0;
    int maxIndex = -1;
    for (int i=0; i<4; ++i) {
        if (photcatresult[i].nstars.toInt() > maxStars) {
            maxStars = photcatresult[i].nstars.toInt();
            maxIndex = i;
        }
    }
    if (maxIndex == 0) ui->resultSDSSPushButton->setChecked(true);
    else if (maxIndex == 1) ui->resultPANSTARRSPushButton->setChecked(true);
    else if (maxIndex == 2) ui->resultAPASSPushButton->setChecked(true);
    else {
        // Fallback onto AVGWHITE solution
        maxIndex = 3;
        ui->resultAVGWHITEPushButton->setChecked(true);
    }

    toggleCalibResult();

    buttonPalette.setColor(QPalette::Button, QColor("#44d8cc"));
    ui->calibratePushButton->setPalette(buttonPalette);
    ui->calibratePushButton->setEnabled(true);
    showDone(ui->calibratePushButton, "(1) Calibrate");
    emit messageAvailable("Done.", "ignore");

    ui->redComboBox->setEnabled(true);
    ui->greenComboBox->setEnabled(true);
    ui->blueComboBox->setEnabled(true);

    // send the new color factors to iview (if opened)
    sendColorFactors();

    // If no reference stars were retrieved
    if (photcatresult[maxIndex].nstars.toInt() == 0 || maxIndex == 3) {
        emit messageAvailable("No G2-type references could be matched for this field. Falling back on 'average white'.", "warning");
    }
}

// Given a name, find the corresponding MyImage* in memory
MyImage* ColorPicture::identifyCroppedMyImage(QString name)
{
    for (auto &myImage : croppedList) {
        if (myImage->name == name) return myImage;
    }
    emit messageAvailable("Could not identify "+name+"<br>in the list of cropped images!", "error");
    return nullptr;
}

QVector<QVector<double>> ColorPicture::getObjectData(MyImage *myImage)
{
    QVector<QVector<double>> objData;

    objData.reserve(myImage->objectList.length());
    if (myImage->objectList.length() == 0) {
        emit messageAvailable("No objects detected in "+myImage->baseName+" !", "error");
    }
    else {
        for (auto &object : myImage->objectList) {
            QVector<double> tmp;
            if (object->FLAGS == 0 && object->FLUX_AUTO > 0.) {
                // DEC comes first in the catalogs, because the matching alg sorts the vectors for DEC
                tmp << object->DELTA_J2000 << object->ALPHA_J2000 << object->FLUX_AUTO;
                objData.append(tmp);
            }
        }
    }
    return objData;
}

void ColorPicture::updateFilterRatio()
{
    // Read results
    QString ratio = QString::number(bbnbFluxRatio, 'f', 3);
    QString ratioError = QString::number(bbnbFluxRatioError, 'f', 3);
    ui->filterRatioLineEdit->setText(ratio+" +/- "+ratioError);

    ui->getRatioPushButton->setText("(1) Estimate flux ratio");
    ui->getRatioPushButton->setEnabled(true);
    ui->narrowbandComboBox->setEnabled(true);
    ui->broadbandComboBox->setEnabled(true);
    ui->BBNBcombinePushButton->setEnabled(true);
}

void ColorPicture::updateBBNBcombine()
{
    QString oldBB = ui->broadbandComboBox->currentText();
    QString oldNB = ui->narrowbandComboBox->currentText();

    // Update the model (which refreshes all combo boxes)
    refreshComboBoxes();

    // Restore previous combobox selection
    ui->broadbandComboBox->setCurrentText(oldBB);
    ui->narrowbandComboBox->setCurrentText(oldNB);

    ui->BBNBcombinePushButton->setEnabled(true);
    ui->BBNBcombinePushButton->setText("(2) Combine images");
}

void ColorPicture::toggleCalibResult()
{
    int i;
    if (ui->resultSDSSPushButton->isChecked()) i = 0;
    else if (ui->resultPANSTARRSPushButton->isChecked()) i = 1;
    else if (ui->resultAPASSPushButton->isChecked()) i = 2;
    else if (ui->resultAVGWHITEPushButton->isChecked()) i = 3;
    else i = 4;

    if (i<4) {
        ui->redFactorLineEdit->setText(photcatresult[i].rfac);
        ui->redErrorLineEdit->setText(photcatresult[i].rfacerr);
        ui->greenFactorLineEdit->setText("1.000");
        ui->greenErrorLineEdit->setText("0.000");
        ui->blueFactorLineEdit->setText(photcatresult[i].bfac);
        ui->blueErrorLineEdit->setText(photcatresult[i].bfacerr);
    }
    else {
        ui->redFactorLineEdit->clear();
        ui->redErrorLineEdit->clear();
        ui->greenFactorLineEdit->setText("1.000");
        ui->greenErrorLineEdit->setText("0.000");
        ui->blueFactorLineEdit->clear();
        ui->blueErrorLineEdit->clear();
    }
}

void ColorPicture::readCalibResults(QString filename, int index)
{
    QString path = mainDir+"/color_theli/PHOTCAT_calibration/";
    QFile file(path+"/"+filename);
    QStringList list;
    QString line;
    int i=0;

    if ( file.open(QIODevice::ReadOnly)) {
        QTextStream stream( &file );
        while ( !stream.atEnd() ) {
            line = stream.readLine().simplified();
            list = line.split(" ");
            if (!line.isEmpty()) {
                if (i==0) photcatresult[index].nstars = line;
                if (i==1) {
                    photcatresult[index].bfac    = list.at(0);
                    photcatresult[index].bfacerr = list.at(1);
                }
                if (i==2) {
                    photcatresult[index].gfac    = list.at(0);
                    photcatresult[index].gfacerr = list.at(1);
                }
                if (i==3) {
                    photcatresult[index].rfac    = list.at(0);
                    photcatresult[index].rfacerr = list.at(1);
                }
                ++i;
            }
        }
        file.close();
    }
    else {
        QMessageBox::warning(this, tr("Could not read color calibration result."),
                             tr("The file ")+path+"/"+filename+tr(" could not be opened."),
                             QMessageBox::Ok);
    }
}

void ColorPicture::readFilterRatioResults(QString filename)
{
    QString path = mainDir+"/color_theli/PHOTCAT_calibration/";
    QFile file(path+"/"+filename);
    QStringList list;
    QString line;
    QString ratio = "";
    QString ratio_error = "";

    if ( file.open(QIODevice::ReadOnly)) {
        QTextStream stream( &file );
        while ( !stream.atEnd() ) {
            line = stream.readLine().simplified();
            list = line.split(" ");
            if (!line.isEmpty() && list.length() == 2) {
                ratio = list.at(0);
                ratio_error = list.at(1);
            }
        }
        file.close();
        if (!ratio.isEmpty() && !ratio_error.isEmpty()) {
            ui->filterRatioLineEdit->setText(ratio+" +/- "+ratio_error);
        }
        else {
            ui->filterRatioLineEdit->clear();
        }
    }
    else {
        QMessageBox::warning(this, tr("Could not read filter ratio result."),
                             tr("The file ")+path+"/"+filename+tr(" could not be opened."),
                             QMessageBox::Ok);
    }
}

void ColorPicture::updateColorFactorsExternal(QString redFactor, QString blueFactor)
{
    ui->redFactorLineEdit->setText(redFactor);
    ui->blueFactorLineEdit->setText(blueFactor);
    resetResultButtonGroup();
}

// Invoked by the preview button, as well as by the "get statistics" push button
void ColorPicture::on_previewCalibPushButton_clicked()
{
    if (iViewOpen) {
        iView->raise();
        return;
    }

    // Load the image viewer
    checkCalibrationFactor(ui->redFactorLineEdit);
    checkCalibrationFactor(ui->blueFactorLineEdit);

    iView = new IView("FITScolor", mainDir+"/color_theli/",
                      ui->redComboBox->currentText(),
                      ui->greenComboBox->currentText(),
                      ui->blueComboBox->currentText(),
                      ui->redFactorLineEdit->text().toFloat(),
                      ui->blueFactorLineEdit->text().toFloat(),
                      this);

    connect(this, &ColorPicture::colorFactorChanged, iView, &IView::updateColorViewExternal);
    connect(iView, &IView::colorFactorChanged, this, &ColorPicture::updateColorFactorsExternal);
    connect(iView, &IView::closed, this, &ColorPicture::updateIviewStatus);
    connect(iView, &IView::statisticsRequested, this, &ColorPicture::measureStatistics);

    iViewOpen = true;
    iView->show();

    if (sender() == ui->getStatisticsPushButton) {
        QMessageBox msgBox;
        msgBox.setText("Left-click into a blank region of the image to obtain image statistics. "
                       "Ideally, this should be the darkest region of the image with few stars.\n\n"
                       "Statistics will be measured in a 9x9 pixel area.");
        msgBox.exec();
    }
}

void ColorPicture::updateIviewStatus()
{
    iViewOpen = false;
}

void ColorPicture::measureStatistics(long x, long y)
{
    ui->statisticsTableWidget->clear();
    int nrows = coaddImageModel->rowCount();
    int ncols = 5;
    ui->statisticsTableWidget->setRowCount(nrows);
    ui->statisticsTableWidget->setColumnCount(ncols);
    QStringList header;
    header << "Image" << "median" << "rms" << "min (black point)" << "max (white point)";
    ui->statisticsTableWidget->setHorizontalHeaderLabels(header);

    // Get the min and max values first
    QTableWidgetItem *item0 = 0;
    QTableWidgetItem *item1 = 0;
    QTableWidgetItem *item2 = 0;
    QTableWidgetItem *item3 = 0;
    QTableWidgetItem *item4 = 0;

    QVector<float> minValues;
    QVector<float> maxValues;

    for (int i=0; i<nrows; ++i) {
        QString name = coaddImageModel->index(i,0).data().toString();
        for (auto &myImage : croppedList) {
            if (myImage->name == name) {
                long xmin = x - 1 - 4;
                long xmax = x - 1 + 4;
                long ymin = y - 1 - 4;
                long ymax = y - 1 + 4;
                QVector<float> data = myImage->extractPixelValues(xmin, xmax, ymin, ymax);
                //  To load from drive instead of memory
                //  MyFITS image(dirName + name);
                //  image.loadDataSection(xmin, xmax, ymin, ymax, data);
                float medVal = medianMask_T(data);
                float rmsVal = madMask_T(data)*1.486;
                float minVal = medVal - 6.*rmsVal;      // an estimate for the black point
                float maxVal = medVal + 250.*rmsVal;    // an estimate for the white point
                minValues.append(minVal);
                maxValues.append(maxVal);
                item0 = new QTableWidgetItem(name);
                item1 = new QTableWidgetItem(QString::number(medVal, 'f', 3));
                item2 = new QTableWidgetItem(QString::number(rmsVal, 'f', 2));
                item0->setFlags(item0->flags() ^ Qt::ItemIsEditable);
                item1->setFlags(item1->flags() ^ Qt::ItemIsEditable);
                item2->setFlags(item2->flags() ^ Qt::ItemIsEditable);
                ui->statisticsTableWidget->setItem(i, 0, item0);
                ui->statisticsTableWidget->setItem(i, 1, item1);
                ui->statisticsTableWidget->setItem(i, 2, item2);
            }
        }
    }

    // Black and white points should be the same for all imagesto preserve colors
    float commonMin = minVec_T(minValues);
    float commonMax = maxVec_T(maxValues);
    for (int i=0; i<nrows; ++i) {
        item3 = new QTableWidgetItem(QString::number(commonMin, 'f', 3));
        item4 = new QTableWidgetItem(QString::number(commonMax, 'f', 2));
        ui->statisticsTableWidget->setItem(i, 3, item3);
        ui->statisticsTableWidget->setItem(i, 4, item4);
    }

    ui->statisticsTableWidget->resizeColumnsToContents();
    statisticsRetrieved = true;
}

void ColorPicture::resetResultButtonGroup(QString resetLabels)
{
    resultButtonGroup->setExclusive(false);
    ui->resultSDSSPushButton->setChecked(false);
    ui->resultPANSTARRSPushButton->setChecked(false);
    ui->resultAPASSPushButton->setChecked(false);
    ui->resultAVGWHITEPushButton->setChecked(false);
    resultButtonGroup->setExclusive(true);

    if (!resetLabels.isEmpty()) {
        ui->numSDSSLabel->setText("? stars");
        ui->numPANSTARRSLabel->setText("? stars");
        ui->numAPASSLabel->setText("? stars");
        ui->numAVGWHITELabel->setText("? stars");
    }
}

void ColorPicture::on_actionClose_triggered()
{
    this->close();
}


void ColorPicture::on_redComboBox_currentIndexChanged(int index)
{
    if (iViewOpen) {
        iView->close();
        on_previewCalibPushButton_clicked();
    }
}

void ColorPicture::on_greenComboBox_currentIndexChanged(int index)
{
    if (iViewOpen) {
        iView->close();
        on_previewCalibPushButton_clicked();
    }
}

void ColorPicture::on_blueComboBox_currentIndexChanged(int index)
{
    if (iViewOpen) {
        iView->close();
        on_previewCalibPushButton_clicked();
    }
}

void ColorPicture::on_narrowbandComboBox_currentTextChanged(const QString &arg1)
{
    ui->filterRatioLineEdit->setText("0");
}

void ColorPicture::on_broadbandComboBox_currentTextChanged(const QString &arg1)
{
    ui->filterRatioLineEdit->setText("0");
}


void ColorPicture::on_abortPushButton_clicked()
{
    workerThread->quit();
    workerThread->wait();

}
