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

#include "imagestatistics.h"
#include "ui_imagestatistics.h"
#include "../instrumentdata.h"
#include "../functions.h"
#include "../qcustomplot.h"
#include "../iview/iview.h"
#include "../myimage/myimage.h"
#include "../processingStatus/processingStatus.h"

#include <QSettings>
#include <QValidator>
#include <QTest>
#include <QVector>
#include <QList>
#include <QStringList>
#include <QRegExpValidator>

// ImageStatistics::ImageStatistics(QVector<QList<MyImage*>> &imlist, const QString main, const QString sciencedir,
//                                 const instrumentDataType *instrumentData, QWidget *parent):
ImageStatistics::ImageStatistics(QList<Data*> &datalist, const QString main, const QString sciencedirname,
                                 const instrumentDataType *instrumentData, QWidget *parent):
    QMainWindow(parent),
    mainDir(main),
    instData(instrumentData),
    dataList(datalist),
    ui(new Ui::ImageStatistics)
{
    ui->setupUi(this);
    initEnvironment(thelidir, userdir);

    makeConnections();

    QStringList dirnameList;
    for (auto &it : dataList) {
        dirnameList.append(it->subDirName);
    }
    ui->scienceComboBox->insertItems(0, dirnameList);

    scienceDirName = mainDir + "/" + sciencedirname;
    scienceDir.setPath(scienceDirName);

    ui->scienceComboBox->setCurrentText(sciencedirname);

    Data *scienceData = getData(dataList, sciencedirname);            // dataList corresponds to DT_SCIENCE in the Controller class
    scienceData->populateExposureList();
    myExposureList = scienceData->exposureList;

    //    if (!myExposureList[instData->validChip]->isEmpty()) statusString = myExposureList[instData->validChip].
    //    else statusString = "";
    //    ui->filterLineEdit->setText("*_1"+statusString+".fits");
    ui->statPlot->setMultiSelectModifier(Qt::ControlModifier);

    //    ui->statPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->statPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iMultiSelect);

    setWindowIcon(QIcon(":/icons/sigma.png"));

    init();
}

void ImageStatistics::init()
{
    allMyImages.clear();
    for (int k=0; k<myExposureList.length(); ++k) {
        for (int chip=0; chip<instData->numChips; ++chip) {
            if (instData->badChips.contains(chip)) continue;
            auto &it = myExposureList[k][chip];
            it->loadHeader();       // if not yet in memory
            it->getMode(true);
            allMyImages.append(it);
        }
    }

    processingStatus = new ProcessingStatus(scienceDirName);
    processingStatus->readFromDrive();
    statusString = processingStatus->statusString;

    ui->statPlot->plotLayout()->clear();

    // Plot data right away
    on_statisticsPushButton_clicked();
}

ImageStatistics::~ImageStatistics()
{
    delete processingStatus;
    processingStatus = nullptr;

    delete ui;
}

// cloned from Controller class
Data* ImageStatistics::getData(QList<Data *> DT_x, QString dirName)
{
    for (auto &it : DT_x) {
        if (QDir::cleanPath(it->dirName) == QDir::cleanPath(mainDir + "/" + dirName)) {
            // check if it contains mastercalib data
            it->checkPresenceOfMasterCalibs();
            return it;
        }
    }

    return nullptr;
}

void ImageStatistics::makeConnections()
{

    numericThresholdList << ui->skyMinLineEdit << ui->skyMaxLineEdit
                         << ui->airmassMinLineEdit << ui->airmassMaxLineEdit
                         << ui->seeingMinLineEdit << ui->seeingMaxLineEdit
                         << ui->rzpMinLineEdit << ui->rzpMaxLineEdit
                         << ui->ellMinLineEdit << ui->ellMaxLineEdit
                         << ui->imageMinLineEdit << ui->imageMaxLineEdit;

    // For a mouse-click in the plot
    connect(ui->statPlot, &QCustomPlot::plottableClick, this, &ImageStatistics::dataPointClicked);

    connect(ui->chipsLineEdit, &QLineEdit::editingFinished, this, &ImageStatistics::plot);

    // Validators
    connect(ui->raLineEdit, &QLineEdit::textChanged, this, &ImageStatistics::validate);
    connect(ui->decLineEdit, &QLineEdit::textChanged, this, &ImageStatistics::validate);
    for (auto &it : numericThresholdList) {
        connect(it, &QLineEdit::textChanged, this, &ImageStatistics::validate);
        connect(it, &QLineEdit::editingFinished, this, &ImageStatistics::numericSelection);
    }

    // Connections for iView can only be made when iView is instantiated further below.
}

void ImageStatistics::clearAll()
{
    //    if (numObj == 0) return;
    clearData();
    clearSelection();
    badStatsList.clear();
    ui->statPlot->clearItems();
    ui->statPlot->clearGraphs();
    ui->statPlot->plotLayout()->clear();
    plot();
    ui->statPlot->replot();
    numObj = 0;
    statisticsDataDisplayed = false;
}

void ImageStatistics::on_statisticsPushButton_clicked()
{
    if (!scienceDir.exists()) {
        QMessageBox::warning( this, "Bad selection",
                              "The selected directory does not exist.\n");
        return;
    }

    QString chips = ui->chipsLineEdit->text();       // Could be any text, but we just make it a list of detector IDs
    chips = chips.replace(',', ' ').simplified();
    QStringList chipList = chips.split(' ');
    QVector<int> chipID;
    if (!chips.isEmpty()) {
        for (auto &it : chipList) {
            //    instData = instrumentData;
            chipID.append(it.toInt());
        }
    }

    filteredImageList.clear();
    // Filter for sky coordinates
    QString ra = ui->raLineEdit->text();
    QString dec = ui->decLineEdit->text();

    for (int k=0; k<allMyImages.length(); ++k) {
        if (isImageSelected(allMyImages[k], ra, dec, chipID)) filteredImageList << allMyImages[k]->chipName;
    }

    numObj = filteredImageList.size();
    if (numObj == 0) {
        QMessageBox::warning( this, "No images found",
                              "No images found, check the directory path\n"
                              "and, if provided, your name filter or sky coordinates.\n");
        ui->statisticsPushButton->setEnabled(true);
        ui->statisticsPushButton->setText("Get statistics ...");
        return;
    }

    readStatisticsData();

    plot();
}


bool ImageStatistics::isImageSelected(MyImage *myImage, const QString &ra, const QString &dec, const QVector<int> &chipID)
{
    // RA / DEC filtering
    if (!ra.isEmpty() && !dec.isEmpty()) {
        if (!myImage->containsRaDec(ra,dec)) return false;
    }

    if (chipID.isEmpty()) return true;

    // Chip filtering
    bool correctChip = false;
    for (auto &chip : chipID) {
        if (chip == myImage->chipNumber) correctChip = true;
    }

    if (correctChip) return true;
    else return false;
}

void ImageStatistics::makeListOfBadImages()
{
    // check for bad data on drive
    QDir badStatsDir(scienceDirName+"/inactive/badStatistics/");
    if (badStatsList.isEmpty()) {
        QStringList filter;
        filter << "*.fits";
        badStatsList = badStatsDir.entryList(filter);
    }

    // check for bad data in memory (should always be the same as on drive, but nonetheless)
    for (int k=0; k<allMyImages.length(); ++k) {
        auto &it = allMyImages[k];
        if (it->activeState == MyImage::BADSTATS) badStatsList << it->chipName;
    }

    filteredImageList.removeDuplicates();
}

void ImageStatistics::readStatisticsData()
{
    // Must flag bad images
    makeListOfBadImages();

    clearData();

    // Compile the statistics data
    numObj = 0;
    // check for bad data in memory (should always be the same as on drive, but nonetheless)
    for (auto &it : allMyImages) {
        //        for (auto &it : myExposureList[chip]) {
        // skip bad images
        if (it->activeState != MyImage::ACTIVE) continue;
        if (badStatsList.contains(it->chipName)) continue;
        // only include images that match the sky coordinate filter
        // (always true if no sky coords given)
        if (!filteredImageList.contains(it->chipName)) continue;

        dataName.append(it->chipName);
        dataImageNr.append(numObj+1);

        // Default values and flags if data not available

        // Background
        if (it->skyValue == 0.0) {      // poor default of -1e9 in myimage.h
            skyData = false;
            dataSky.append(0.0);
        }
        else dataSky.append(it->skyValue);

        // In case GUI is launched, catalogs are present, and statistics is requested,
        // we need to fetch the data from the catalogs (or the MyImages)
        if (it->fwhm == -1.0 || it->ellipticity == -1.0) {
            if (it->objectList.length() > 0) it->calcMedianSeeingEllipticity();
            else {
                QString catName = it->path+"/cat/"+it->rootName+".scamp";
                QFile cat;
                cat.setFileName(catName);
                if (cat.exists()) {
                    it->calcMedianSeeingEllipticitySex(catName, 2*it->chipNumber + 1);     // LDAC_OBJECTS table in extensions 3, 5, 7, etc
                }
            }
        }

        // Do we have GAIA seeing? we should have, at least if catalogs exist
        if (it->fwhm == -1.0) {
            // No. Do we have median estimate?
            if (it->fwhm_est == -1.0) {
                // No
                seeingData = false;
                dataFWHM.append(0.0);
            }
            else {
                dataFWHM.append(it->fwhm_est);
                seeingFromGaia = false;
            }
        }
        else {
            dataFWHM.append(it->fwhm);
            seeingFromGaia = true;
        }

        // Do we have GAIA ellipticity?
        if (it->ellipticity == -1.0) {
            // No. Do we have median estimate?
            if (it->ellipticity_est == -1.0) {
                // No
                ellipticityData = false;
                dataEllipticity.append(-0.1);
            }
            else {
                dataEllipticity.append(it->ellipticity_est);
                seeingFromGaia = false;
            }
        }
        else {
            dataEllipticity.append(it->ellipticity);
            seeingFromGaia = true;
        }

        // Airmass
        if (it->airmass == 0.0) {
            airmassData = false;
            dataAirmass.append(2.5);
        }
        else dataAirmass.append(it->airmass);

        // Relative zeropoint
        if (it->RZP == -1.0) {
            rzpData = false;
            dataRZP.append(-0.5);
        }
        else dataRZP.append(it->RZP);

        ++numObj;
    }
}

void ImageStatistics::clearData()
{
    dataName.clear();
    dataImageNr.clear();
    dataSky.clear();
    dataFWHM.clear();
    dataRZP.clear();
    dataAirmass.clear();
    dataEllipticity.clear();
    numObj = 0;
}

void ImageStatistics::on_exportPushButton_clicked()
{
    if (numObj == 0) return;
    if (!scienceDir.exists()) return;

    QString chips = ui->chipsLineEdit->text();       // Could be any text, but we just make it a list of detector IDs
    chips = chips.replace(',', ' ').simplified();
    chips = chips.replace(' ', '_');
    QString chipString = "";
    if (!chips.isEmpty()) chipString = "_chip_"+chips;

    QString name = "statistics"+chipString+".png";
    QString saveFileName =
            QFileDialog::getSaveFileName(this, tr("Save .png Image"),
                                         scienceDirName+"/"+name,
                                         tr("Graphics file (*.png)"));

    if ( saveFileName.isEmpty() ) return;
    QFileInfo graphicsInfo(saveFileName);
    if (graphicsInfo.suffix() != "png") {
        QMessageBox::information(this, tr("Invalid graphics type"),
                                 tr("Only .png format is currently allowed."),
                                 QMessageBox::Ok);
        return;
    }
    else {
        ui->statPlot->savePng(saveFileName,0,0,1.25);
    }
}

void ImageStatistics::on_showPushButton_clicked()
{
    if (!scienceDir.exists()) return;
    if (!ui->showPushButton->isChecked() && iViewOpen) {
        iView->close();
        iViewOpen = false;
        return;
    }

    if (ui->showPushButton->isChecked() && !iViewOpen) {
        if (!imgSelected && !statisticsDataDisplayed) {
            iView = new IView("FITSmonochrome", scienceDirName, "*.fits", this);
            iView->show();
            //            iView->clearAll();
        }
        else if (!imgSelected && statisticsDataDisplayed) {
            QCPDataRange range(0,1);
            selection.addDataRange(range);
            imgSelectedName = dataName[0];
            imgSelected = true;
            lastButtonClicked = Qt::LeftButton;
            plotSelection(0);
            iView = new IView("MEMview", allMyImages, scienceDirName, this); // dirname is needed to overlay catalogs
            iViewOpen = true;
            // To be implemented
            //            connect(iView, &IView::currentlyDisplayedIndex, this, &ImageStatistics::currentlyDisplayedIndexReceived);
            iView->scene->clear();
            iView->show();
        }
        else {
            if (lastDataPointClicked != -1) {
                iView = new IView("MEMview", allMyImages, scienceDirName, this); // dirname is needed to overlay catalogs
                iView->loadFromRAM(allMyImages[lastDataPointClicked], 0);   // '0' refers to dataCurrent
                iView->currentId = lastDataPointClicked;
                iView->show();
            }
            else {
                qDebug() << "QDEBUG: ImageStatistics::on_showPushButton_clicked(): BUG: invalid selection";
            }
        }
        iViewOpen = true;
        connect(this, &ImageStatistics::imageSelected, iView, &IView::loadFITSexternalRAM);
    }
    else if (ui->showPushButton->isChecked() && iViewOpen) {
        iView->show();
        connect(this, &ImageStatistics::imageSelected, iView, &IView::loadFITSexternalRAM);
        iView->raise();
        iViewOpen = true;
    }

    // "uncheck" the iView PushButton when the viewer is closed externally
    connect(iView, &IView::closed, this, &ImageStatistics::uncheckIviewPushButton);
}

// To be implemented
/*
void ImageStatistics::currentlyDisplayedIndexReceived(int currentId)
{
    // indicate displayed image, e.g. by a ring around the data point
}
*/

void ImageStatistics::on_ClearPlotPushButton_clicked()
{
    clearAll();
}

void ImageStatistics::uncheckIviewPushButton()
{
    if (ui->showPushButton->isChecked()) ui->showPushButton->setChecked(false);
    iViewOpen = false;
}

void ImageStatistics::clearSelection()
{
    selection.clear();
    imgSelected = false;
    for (auto &it : numericThresholdList) {
        it->clear();
    }
}

void ImageStatistics::validate()
{
    QRegExp RA( "[0-9.:]+" );
    QRegExp DEC( "^[-]{0,1}[0-9.:]+" );
    QRegExp ripos( "^[0-9]+" );
    QRegExp rf( "^[-]{0,1}[0-9]*[.]{0,1}[0-9]+" );
    QRegExp rfpos( "^[0-9]*[.]{0,1}[0-9]+" );
    QRegExp ricommablank( "^[0-9,\\s]+" );

    QValidator* validator_int_pos_comma_blank = new QRegExpValidator( ricommablank, this );
    QValidator* validator_ra = new QRegExpValidator( RA, this );
    QValidator* validator_dec = new QRegExpValidator( DEC, this );
    QValidator* validator_float = new QRegExpValidator(rf, this );
    QValidator* validator_float_pos = new QRegExpValidator(rfpos, this );
    QValidator* validator_int_pos = new QRegExpValidator( ripos, this );
    ui->raLineEdit->setValidator(validator_ra);
    ui->decLineEdit->setValidator(validator_dec);
    ui->skyMinLineEdit->setValidator(validator_float);
    ui->skyMaxLineEdit->setValidator(validator_float);
    ui->airmassMinLineEdit->setValidator(validator_float_pos);
    ui->airmassMaxLineEdit->setValidator(validator_float_pos);
    ui->seeingMinLineEdit->setValidator(validator_float_pos);
    ui->seeingMaxLineEdit->setValidator(validator_float_pos);
    ui->rzpMinLineEdit->setValidator(validator_float);
    ui->rzpMaxLineEdit->setValidator(validator_float);
    ui->ellMinLineEdit->setValidator(validator_float_pos);
    ui->ellMaxLineEdit->setValidator(validator_float_pos);
    ui->imageMinLineEdit->setValidator(validator_int_pos);
    ui->imageMaxLineEdit->setValidator(validator_int_pos);
    ui->chipsLineEdit->setValidator( validator_int_pos_comma_blank );
}

void ImageStatistics::on_readmePushButton_clicked()
{
    imstatsReadme = new ImstatsReadme(this);
    imstatsReadme->show();
}

void ImageStatistics::on_fwhmunitsComboBox_currentIndexChanged(const QString &arg1)
{
    plot();
}

void ImageStatistics::on_actionClose_triggered()
{
    close();
}

void ImageStatistics::on_scienceComboBox_activated(const QString &arg1)
{

    scienceDirName = mainDir + "/" + arg1;
    scienceDir.setPath(scienceDirName);

    Data *scienceData = getData(dataList, arg1);            // dataList corresponds to DT_SCIENCE in the Controller class
    scienceData->populateExposureList();
    myExposureList = scienceData->exposureList;

    init();
}
