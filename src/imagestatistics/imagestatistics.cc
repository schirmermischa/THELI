#include "imagestatistics.h"
#include "ui_imagestatistics.h"
#include "../instrumentdata.h"
#include "../functions.h"
#include "../qcustomplot.h"
#include "../iview/iview.h"
#include "../myfits/myfits.h"
#include "../myimage/myimage.h"
#include "../mywcs.h"

#include <QSettings>
#include <QValidator>
#include <QTest>
#include <QVector>
#include <QList>
#include <QStringList>
#include <QRegExpValidator>

ImageStatistics::ImageStatistics(QVector<QList<MyImage*>> &imlist, QString main, QString sciencedir,
                                 instrumentDataType *instrumentData, QWidget *parent):
    QMainWindow(parent),
    mainDir(main),
    ui(new Ui::ImageStatistics)
{
    ui->setupUi(this);
    initEnvironment(thelidir, userdir, tmpdir);

    instData = instrumentData;

    myImageList = imlist;

    allMyImages.clear();
    for (int k=0; k<myImageList.length(); ++k) {
        for (int chip=0; chip<instData->numChips; ++chip) {
            auto &it = myImageList[k][chip];
            it->provideHeaderInfo();
            allMyImages.append(it);
        }
    }

    ui->dirLineEdit->setText(mainDir+"/"+sciencedir);
    paintPathLineEdit(ui->dirLineEdit, mainDir, "dir");
    scienceDir = sciencedir;
    scienceDirName = mainDir + "/" + sciencedir;
    scienceDir.setPath(scienceDirName);

    numericThresholdList << ui->skyMinLineEdit << ui->skyMaxLineEdit
                         << ui->airmassMinLineEdit << ui->airmassMaxLineEdit
                         << ui->seeingMinLineEdit << ui->seeingMaxLineEdit
                         << ui->rzpMinLineEdit << ui->rzpMaxLineEdit
                         << ui->ellMinLineEdit << ui->ellMaxLineEdit
                         << ui->imageMinLineEdit << ui->imageMaxLineEdit;
    makeConnections();

    //    if (!myImageList[0]->isEmpty()) statusString = myImageList[0].
    //    else statusString = "";
    //    ui->filterLineEdit->setText("*_1"+statusString+".fits");
    ui->statPlot->setMultiSelectModifier(Qt::ControlModifier);

    //    ui->statPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->statPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iMultiSelect);
    ui->statPlot->plotLayout()->clear();

    setWindowIcon(QIcon(":/images/sigma.png"));

    // Plot data right away
    on_statisticsPushButton_clicked();

//    plot("initialize");

    initFromList = true;
}

ImageStatistics::~ImageStatistics()
{
    delete ui;
}

void ImageStatistics::makeConnections()
{
    // For a mouse-click in the plot
    connect(ui->statPlot, &QCustomPlot::plottableClick, this, &ImageStatistics::dataPointClicked);

    // Validators
    connect(ui->raLineEdit, &QLineEdit::textChanged, this, &ImageStatistics::validate);
    connect(ui->decLineEdit, &QLineEdit::textChanged, this, &ImageStatistics::validate);
    for (auto &it : numericThresholdList) {
        connect(it, &QLineEdit::textChanged, this, &ImageStatistics::validate);
        connect(it, &QLineEdit::editingFinished, this, &ImageStatistics::numericSelection);
    }

    // Connections for iView can only be made when iView is instantiated further below.
}

void ImageStatistics::on_selectDirPushButton_clicked()
{
    QString filter = ui->filterLineEdit->text();
    if (filter.isEmpty()) filter = "*.fits";
    QStringList filterList(filter);

    QFileDialog qfd(this);
    qfd.setFileMode(QFileDialog::DirectoryOnly);
    qfd.setDirectory(mainDir);
    qfd.setWindowTitle(tr("Select directory"));

    if (qfd.exec()) scienceDirName = qfd.selectedFiles().at(0);

    scienceDir.setPath(scienceDirName);
    ui->dirLineEdit->setText(scienceDirName);
    paintPathLineEdit(ui->dirLineEdit, scienceDirName, "dir");
    clearAll();
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
    plot("initialize");
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

    QString filter = ui->filterLineEdit->text();
    if (filter.isEmpty()) filter = "*.fits";

    coordImageList.clear();
    // Filter for sky coordinates
    QString ra = ui->raLineEdit->text();
    QString dec = ui->decLineEdit->text();

    if (!ra.isEmpty() && !dec.isEmpty()) {
        for (int k=0; k<allMyImages.length(); ++k) {
            auto &it = allMyImages[k];
            if (it->myWCS.containsRaDec(ra,dec)) coordImageList << it->chipName;
        }
    }
    else {
        for (int k=0; k<allMyImages.length(); ++k) {
            auto &it = allMyImages[k];
            coordImageList << it->chipName;
        }
    }

    numObj = coordImageList.size();
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

    coordImageList.removeDuplicates();
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
        //        for (auto &it : myImageList[chip]) {
        // skip bad images
        if (it->activeState != MyImage::ACTIVE) continue;
        if (badStatsList.contains(it->chipName)) continue;
        // only include images that match the sky coordinate filter
        // (always true if no sky coords given)
        if (!coordImageList.contains(it->chipName)) continue;

        dataName.append(it->chipName);
        dataImageNr.append(numObj+1);

        // Default values and flags if data not available
        if (it->skyValue == 0.0) {      // poor default of -1e9 in myimage.h
            seeingData = false;
            dataSky.append(0.0);
        }
        else dataSky.append(it->skyValue);

        if (it->fwhm == -1.0) {
            seeingData = false;
            dataFWHM.append(0.0);
        }
        else dataFWHM.append(it->fwhm);

        if (it->airmass == 0.0) {
            airmassData = false;
            dataAirmass.append(2.5);
        }
        else dataAirmass.append(it->airmass);

        if (it->ellipticity == -1.0) {
            ellipticityData = false;
            dataEllipticity.append(-0.1);
        }
        else dataEllipticity.append(it->ellipticity);

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

    QString name = "statistics_"+ui->filterLineEdit->text()+".png";
    name = name.replace("*","all");
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
        QString filter = ui->filterLineEdit->text();
        if (filter.isEmpty()) filter = "*.fits";
        if (!imgSelected && !statisticsDataDisplayed) {
            iView = new IView("FITSmonochrome", scienceDirName, filter, this);
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
            connect(iView, &IView::currentlyDisplayedIndex, this, &ImageStatistics::currentlyDisplayedIndexReceived);
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

void ImageStatistics::currentlyDisplayedIndexReceived(int currentId)
{
    // indicate displayed image, e.g. by a ring around the data point
}

void ImageStatistics::on_ClearPlotPushButton_clicked()
{
    clearAll();
}

void ImageStatistics::on_dirLineEdit_textChanged(const QString &arg1)
{
    paintPathLineEdit(ui->dirLineEdit, arg1, "dir");
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
}

void ImageStatistics::on_readmePushButton_clicked()
{
    imstatsReadme = new ImstatsReadme(this);
    imstatsReadme->show();
}
