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

#include "iview.h"
#include "ui_iview.h"

#include "../functions.h"
#include "mygraphicsview.h"
#include "mygraphicsellipseitem.h"
#include "mygraphicsscene.h"
#include "dockwidgets/ivconfdockwidget.h"
#include "dockwidgets/ivscampdockwidget.h"
#include "dockwidgets/ivcolordockwidget.h"
#include "dockwidgets/ivwcsdockwidget.h"
#include "ui_ivconfdockwidget.h"
#include "ui_ivcolordockwidget.h"

#include "../tools/tools.h"

#include "../myimage/myimage.h"

#include "fitsio2.h"
#include "wcs.h"
#include "wcshdr.h"
#include <omp.h>

#include <QDir>
#include <QSettings>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QGraphicsGridLayout>
#include <QDebug>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QToolBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPointF>
#include <QScrollBar>
#include <QTimer>
#include <QModelIndex>


void IView::setMiddleMouseMode(QString mode)
{
    if (mode == "SkyMode") {
        ui->actionSkyMode->setChecked(true);
        ui->actionDragMode->setChecked(false);
        ui->actionWCSMode->setChecked(false);
        myGraphicsView->middleMouseMode = "SkyMode";
        hideWCSdockWidget();
    }
    else if (mode == "DragMode") {
        ui->actionDragMode->setChecked(true);
        ui->actionSkyMode->setChecked(false);
        ui->actionWCSMode->setChecked(false);
        middleMouseMode = "DragMode";
        hideWCSdockWidget();
    }
    else if (mode == "WCSMode") {
        ui->actionWCSMode->setChecked(true);
        ui->actionSkyMode->setChecked(false);
        ui->actionDragMode->setChecked(false);
        middleMouseMode = "WCSMode";
        showWCSdockWidget();
    }
}

void IView::showWCSdockWidget()
{
    if (wcsdw->isVisible()) return;

    // Copy the CD matrix to the WCS dock widget and init()
    wcsdw->cd11_orig = wcs->cd[0];
    wcsdw->cd12_orig = wcs->cd[1];
    wcsdw->cd21_orig = wcs->cd[2];
    wcsdw->cd22_orig = wcs->cd[3];
    wcsdw->init();

    addDockWidget(Qt::LeftDockWidgetArea, wcsdw);
    wcsdw->setFloating(false);
    wcsdw->raise();
    wcsdw->show();
}

void IView::hideWCSdockWidget()
{
    removeDockWidget(wcsdw);
    wcsdw->hide();
}

void IView::resizeEvent(QResizeEvent * event)
{
    event->accept();
    if (icdw->ui->zoomFitPushButton->isChecked()) icdw->on_zoomFitPushButton_clicked();
}

void IView::sendStatisticsCenter(QPointF point)
{
    if (displayMode.contains("SCAMP") || displayMode == "CLEAR") return;
    long x = point.x();
    long y = naxis2 - point.y();
    emit statisticsRequested(x, y);
}

QString IView::getVectorLabel(double separation)
{
    QString units;
    if (separation < 60) {
        units = "\"";
    }
    else if (separation>=60. && separation < 3600.) {
        separation /= 60.;
        units = "\'";
    }
    else if (separation > 3600.) {
        separation /= 3600.;
        units = "°";
    }
    return QString::number(separation,'f',2)+" "+units;
}

QRect IView::adjustGeometry()
{
    QRect geometry = myGraphicsView->geometry();
    // if the graphics is larger than what the screen can accomodate:
    int minMargin = 150;
    myGraphicsView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    myGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    myGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    if (naxis2 > screenHeight-minMargin) {
        myGraphicsView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//        myGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        geometry.setHeight(screenHeight-minMargin);
    }
    else {
//        myGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    if (naxis1 > screenWidth-minMargin) {
        myGraphicsView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//        myGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        geometry.setWidth(screenWidth-minMargin);
    }
    else {
//        myGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    return geometry;
}

void IView::setImageList(QString filter)
{
    QDir dir(dirName);
    QStringList filterList = filter.split(" ");
    if (dir.exists()) {
        //        filterList << filter;
        dir.setNameFilters(filterList);
        imageList = dir.entryList();
    }
    numImages = imageList.length();

    // we also need a list of chip names (without the status string)
    imageListToChipName();
}

void IView::setImageListFromMemory()
{
    imageList.clear();
    imageListChipName.clear();
    for (auto &it : myImageList) {
        QString imageName = it->chipName + it->processingStatus->statusString + ".fits";
        imageList.append(imageName);
        imageListChipName.append(it->chipName);
    }
}

void IView::imageListToChipName()
{
    imageListChipName.clear();
    for (auto &it : imageList) {
        QString rootName = it;
        rootName.truncate(rootName.lastIndexOf('_'));
        QStringList list = it.split('_');
        QString chipnr = list.last().remove(".fits");
        chipnr.remove(QRegExp("[A-Z]"));
        rootName.append("_"+chipnr);
        imageListChipName.append(rootName);
    }
}

void IView::setCurrentId(QString filename)
{
    // The number of the file in the list of all images (PNG or FITS) in this directory
    if (filterName.isEmpty() || icdw->ui->filterLineEdit->text().isEmpty()) {
        if (!displayMode.contains("SCAMP")) filterName = "*.fits";
        else filterName = "*.png";
        icdw->ui->filterLineEdit->setText(filterName);
    }
    setImageList(filterName);
    currentId = imageList.indexOf(QFileInfo(filename).fileName());
    if (currentId == -1) {
        qDebug() << "IView::getCurrentId(): Image not found in list." << filterName << numImages << filename;
    }
}

void IView::loadImage()
{
    QString filter = icdw->ui->filterLineEdit->text();
    if (filter.isEmpty()
            || !filter.contains(".fit")
            || !filter.contains("*")) {
        filter = "*.fits";
        icdw->ui->filterLineEdit->setText(filter);
    }

    if (!QDir(dirName).exists()) dirName = QDir::homePath();
    if (displayMode.contains("SCAMP")) {
        currentFileName =
                QFileDialog::getOpenFileName(this, tr("Select image"), dirName,
                                             tr("Images and Scamp checkplots (")+filter+" *.png)");
    }
    else {
        filter = "*.fits *.fit";
        currentFileName =
                QFileDialog::getOpenFileName(this, tr("Select image"), dirName,
                                             tr("Images ")+filter);
    }

    if (currentFileName.isEmpty()) return;

    // Identify file type by suffix
    QFileInfo fi(currentFileName);
    QString suffix = fi.suffix();

    // update the dirname
    dirName = fi.absolutePath();

    // Update the filter string
    filterName = "*."+suffix;
    icdw->ui->filterLineEdit->setText(filter);

    if (suffix == "fits") {
        switchMode("FITSmonochrome");
        // Delete catalog displays, if any
        clearItems();
        // reset the startDirname if in PNG or CLEAR mode previously
        if (displayMode.contains("SCAMP") || displayMode == "CLEAR") startDirNameSet = false;
        loadFITS(currentFileName);
    }
    else if (suffix == "png") {
        switchMode("SCAMP");
        loadPNG(currentFileName);
        myGraphicsView->fitInView(scene->items(Qt::AscendingOrder).at(0), Qt::KeepAspectRatio);
        icdw->on_zoomZeroPushButton_clicked();
    }
    else {
        switchMode("CLEAR");
    }

    // update the startDirName
    if (!startDirNameSet) {
        startDirName = dirName;
        startDirNameSet = true;
    }
}

void IView::clearItems() {
    // Delete any catalog displays
    if (!sourceCatItems.isEmpty()) {
        for (auto &it: sourceCatItems) scene->removeItem(it);
        sourceCatItems.clear();
        sourcecatSourcesShown = false;
        ui->actionSourceCat->setChecked(false);
    }
    if (!refCatItems.isEmpty()) {
        for (auto &it: refCatItems) scene->removeItem(it);
        refCatItems.clear();
        refcatSourcesShown = false;
        ui->actionRefCat->setChecked(false);
    }
    if (!acceptedSkyCircleItems.isEmpty()) {
        for (auto &it: acceptedSkyCircleItems) scene->removeItem(it);
        acceptedSkyCircleItems.clear();
    }
}

void IView::setCatalogOverlaysExternally(bool sourcecatShown, bool refcatShown)
{
    ui->actionSourceCat->setChecked(sourcecatShown);
    ui->actionRefCat->setChecked(refcatShown);
    sourcecatSourcesShown = sourcecatShown;
    refcatSourcesShown = refcatShown;
}

// Used by imagestatistics
void IView::clearAll()
{
    clearItems();
    scene->clear();
    myGraphicsView->setScene(scene);
    myGraphicsView->show();
    this->setWindowTitle("iView");
    pageLabel->clear();
    switchMode("CLEAR");
}

void IView::loadFITSexternal(QString fileName, QString filter)
{
    if (!fileName.isEmpty()) {
        switchMode("FITSmonochrome");
        filterName = filter;
        icdw->ui->filterLineEdit->setText(filterName);
        loadFITS(fileName);
    }
    else {
        switchMode("CLEAR");
        filterName = filter;
        icdw->ui->filterLineEdit->setText(filterName);
    }
}

void IView::loadFITSexternalRAM(int index)
{
    switchMode("FITSmonochrome");
    loadFromRAM(myImageList[index], 0);
}

void IView::loadPNG(QString filename, int currentId)
{
    if (imageList.isEmpty() || dirName.isEmpty()) {
        qDebug() << "IView::loadPNG(): No scamp checkplots found!";
        return;
    }

    if (filename.isEmpty()) filename = dirName+"/"+imageList.at(currentId);
    else {
        setCurrentId(filename);
        if (currentId == -1) return;
    }

    QFileInfo fi(filename);
    QString showName = fi.fileName();

    QPixmap pixmap = QPixmap(filename);
    naxis1 = pixmap.width();
    naxis2 = pixmap.height();

    zoomLevel = 0;
    //    icdw->zoom2scale(zoomLevel);   // uninitialized
    myGraphicsView->resetMatrix();

    //    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(pixmap);
    pixmapItem = new QGraphicsPixmapItem(pixmap);

    // Update the view
    //    QRect geometry = adjustGeometry();
    scene->clear();
    scene->addItem(pixmapItem);
    myGraphicsView->setScene(scene);
    myGraphicsView->scale(1.0,1.0);
    //    myGraphicsView->setGeometry(geometry);
    myGraphicsView->show();

    QString prependPath = dirName;
    removeLastCharIf(prependPath, '/');
    QString path = get2ndLastWord(prependPath,'/')+"/"+getLastWord(prependPath,'/');
    this->setWindowTitle("iView ---   "+path+"/ ---   "+showName);
    pageLabel->setText(" Image "+QString::number(currentId+1)+" / "+QString::number(numImages));

    //  icdw->zoom2scale(zoomLevel);
    myGraphicsView->resetMatrix();

    myGraphicsView->setMinimumSize(naxis1,naxis2);
    myGraphicsView->setMaximumSize(naxis1,naxis2);
    myGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    myGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    myGraphicsView->resize(naxis1,naxis2);
}

void IView::loadFITS(QString filename, int currentId, qreal scaleFactor)
{
    if (filename.isEmpty()) {
        filename = dirName+"/"+imageList.at(currentId);
        currentFileName = imageList.at(currentId);
    }
    else {
        setCurrentId(filename);
        if (currentId == -1) return;
    }

    QFileInfo fi(filename);
    QString showName = fi.fileName();

    if (loadFITSdata(filename, fitsData)) {
        // Map dynamic range to INT, update the scene
        mapFITS();

        // Update the view
        QRect geometry = adjustGeometry();
        myGraphicsView->setScene(scene);
        myGraphicsView->scale(scaleFactor, scaleFactor);
        myGraphicsView->setGeometry(geometry);
        myGraphicsView->show();
        myGraphicsView->setMinimumSize(200,200);
        myGraphicsView->setMaximumSize(10000,10000);

        QString prependPath = dirName;
        removeLastCharIf(prependPath, '/');
        QString path = getLastWord(prependPath,'/');
        this->setWindowTitle("iView ---   "+path+"/ ---  "+showName);
        pageLabel->setText(" Image "+QString::number(currentId+1)+" / "+QString::number(numImages));

        // Show the magnified area in the zoom window
        //        QPixmap zoomedPixmap = copy(scene->items().at(0),
        //        zoomedPixmap.
        //ui->zoomGraphicsView->setScene(zoomScene);
        //ui->zoomGraphicsView->scale(scaleFactor*5, scaleFactor*5);
        //ui->zoomGraphicsView->show();
    }
    else {
        // At end of file list, or file does not exist anymore.
        // Update file list
        icdw->on_filterLineEdit_textChanged("dummy");
        return;
    }
}

void IView::loadFromRAMlist(const QModelIndex &index)
{

    loadFromRAM(myImageList[index.row()], index.column());
}

void IView::loadFromRAM(MyImage *it, int indexColumn)
{
    currentMyImage = it;
    if (indexColumn == 0 || indexColumn == 3) {
        // Load image into memory if not yet present
        if (!weightMode) {
            it->readImage(false);
            fitsData = it->dataCurrent;
        }
        else {
            it->readWeight();
            fitsData = it->dataWeight;
            it->weightInMemory = true;
        }
    }
    else if (indexColumn == 4 && it->backupL1InMemory) fitsData = it->dataBackupL1;
    else if (indexColumn == 5 && it->backupL2InMemory) fitsData = it->dataBackupL2;
    else if (indexColumn == 6 && it->backupL3InMemory) fitsData = it->dataBackupL3;
    else {
        return;
    }
    naxis1 = it->naxis1;
    naxis2 = it->naxis2;
    plateScale = it->plateScale;
    naxis1 = it->naxis1;
    naxis2 = it->naxis2;
    wcs = it->wcs;
    wcsInit = it->wcsInit;
    this->setWindowTitle("iView --- Memory viewer : "+it->chipName);

    // Get the dynamic range
    // Normal viewer mode
    if (dataIntSet) delete[] dataInt;
    dataInt = new unsigned char[naxis1*naxis2];
    dataIntSet = true;
    // AUTO
    if (icdw->ui->minLineEdit->text().isEmpty()
            || icdw->ui->maxLineEdit->text().isEmpty()
            || icdw->ui->autocontrastPushButton->isChecked()) {
        autoContrast();
    }
    // MANUAL
    else {
        // get background statisics (medVal and rmsVal)
        getImageStatistics();
        dynRangeMin = icdw->ui->minLineEdit->text().toFloat();
        dynRangeMax = icdw->ui->maxLineEdit->text().toFloat();
    }

    mapFITS();

    // Update the view
    QRect geometry = adjustGeometry();
    myGraphicsView->setScene(scene);
    myGraphicsView->scale(icdw->zoom2scale(0),icdw->zoom2scale(0));
    myGraphicsView->setGeometry(geometry);
    myGraphicsView->show();
    myGraphicsView->setMinimumSize(200,200);
    myGraphicsView->setMaximumSize(10000,10000);

    currentMyImage = it;   // For later use, in particular when updating CRPIX1/2
}

void IView::loadColorFITS(qreal scaleFactor)
{
    QFile redFile(dirName+'/'+ChannelR);
    QFile greenFile(dirName+'/'+ChannelG);
    QFile blueFile(dirName+'/'+ChannelB);
    if (!redFile.exists()
            || !greenFile.exists()
            || !blueFile.exists())
        return;

    QString showName = "Color calibrated preview";

    bool testR = loadFITSdata(dirName+'/'+ChannelR, fitsDataR, "redChannel");
    bool testG = loadFITSdata(dirName+'/'+ChannelG, fitsDataG, "greenChannel");
    bool testB = loadFITSdata(dirName+'/'+ChannelB, fitsDataB, "blueChannel");
    if (!testR || !testG || !testB) {
        qDebug() << "IView::loadColorFits: One or more of the three color channels could not be read!";
        qDebug() << ChannelR << ChannelG << ChannelB;
        return;
    }

    allChannelsRead = true;
    // Map dynamic range to INT, update the scene
    mapFITS();

    // Update the view
    QRect geometry = adjustGeometry();
    myGraphicsView->setScene(scene);
    myGraphicsView->scale(scaleFactor, scaleFactor);
    myGraphicsView->setGeometry(geometry);
    myGraphicsView->show();
    myGraphicsView->setMinimumSize(200,200);
    myGraphicsView->setMaximumSize(10000,10000);

    QString prependPath = dirName;
    removeLastCharIf(prependPath, '/');
    QString path = getLastWord(prependPath,'/');
    this->setWindowTitle("iView ---   "+path+"/ ---    "+showName);
}

bool IView::loadFITSdata(QString filename, QVector<float> &data, QString colorMode)
{
    if (displayMode.contains("SCAMP") || displayMode == "CLEAR") {
        qDebug() << "IView::loadFitsData(): Invalid mode";
        return false;
    }

    if (weightMode) {
        filename.replace(".fits", ".weight.fits");
    }

    // Setup the MyImage
    int verbose = 0;
    if (currentMyImage != nullptr) {
        delete currentMyImage;
        currentMyImage = nullptr;
    }
    QVector<bool> dummyMask;
    dummyMask.clear();
    currentMyImage = new MyImage(filename, dummyMask, &verbose);
    currentMyImage->readImage(filename);
    plateScale = currentMyImage->plateScale;
    naxis1 = currentMyImage->naxis1;
    naxis2 = currentMyImage->naxis2;
    fullheader = currentMyImage->fullheader;
    wcs = currentMyImage->wcs;
    (void) wcsset(wcs);
    wcsInit = true;

    // Move the data from the transient MyImage over to the class member.
    data.swap(currentMyImage->dataCurrent);

    // Get the dynamic range
    // Normal viewer mode
    if (displayMode == "FITSmonochrome") {
        if (dataIntSet) delete[] dataInt;
        dataInt = new unsigned char[naxis1*naxis2];
        dataIntSet = true;
        // AUTO
        if (icdw->ui->minLineEdit->text().isEmpty()
                || icdw->ui->maxLineEdit->text().isEmpty()
                || icdw->ui->autocontrastPushButton->isChecked()) {
            autoContrast();
        }
        // MANUAL
        else {
            // get background statisics (medVal and rmsVal)
            getImageStatistics();
            dynRangeMin = icdw->ui->minLineEdit->text().toFloat();
            dynRangeMax = icdw->ui->maxLineEdit->text().toFloat();
        }
    }
    else if (displayMode == "FITScolor") {
        if (colorMode == "redChannel" && dataIntRSet) delete[] dataIntR;
        if (colorMode == "greenChannel" && dataIntGSet) delete[] dataIntG;
        if (colorMode == "blueChannel" && dataIntBSet) delete[] dataIntB;
        if (colorMode == "redChannel") {
            dataIntR = new unsigned char[naxis1*naxis2];
            dataIntRSet = true;
        }
        if (colorMode == "greenChannel") {
            dataIntG = new unsigned char[naxis1*naxis2];
            dataIntGSet = true;
        }
        if (colorMode == "blueChannel") {
            dataIntB = new unsigned char[naxis1*naxis2];
            dataIntBSet = true;
        }
        // Loading a color view of the RGB FITS channels
        // (only executes fully once all channels have been read)
        icdw->ui->autocontrastPushButton->setChecked(true);
        autoContrast(colorMode);
    }
    return true;
}

void IView::mapFITS()
{
    // record the source/ref catalog states
    bool sourceCatShown = ui->actionSourceCat->isChecked();
    bool refCatShown = ui->actionRefCat->isChecked();
    bool G2shown = false;
    if (displayMode == "FITScolor") {
        G2shown = colordw->ui->G2referencesPushButton->isChecked();
    }

    clearItems();
    if (displayMode == "FITSmonochrome" || displayMode == "MEMview") {
        compressDynrange(fitsData, dataInt);
        QImage fitsImage(dataInt, naxis1, naxis2, naxis1, QImage::Format_Grayscale8);
        fitsImage = fitsImage.mirrored(false, true);
        pixmapItem = new QGraphicsPixmapItem( QPixmap::fromImage(fitsImage));
        scene->clear();
        scene->addItem(pixmapItem);
    }
    else if (displayMode == "FITScolor") {
        compressDynrange(fitsDataR, dataIntR, colordw->colorFactorApplied[0]);
        compressDynrange(fitsDataG, dataIntG, colordw->colorFactorApplied[1]);
        compressDynrange(fitsDataB, dataIntB, colordw->colorFactorApplied[2]);

        QImage colorFitsImage(naxis1, naxis2, QImage::Format_ARGB32 );
        for (long i=0; i<naxis1*naxis2; ++i) {
            QRgb argb = qRgba( dataIntR[i], dataIntG[i], dataIntB[i], 255);
            QRgb* rowData = (QRgb*) colorFitsImage.scanLine(i/naxis1);
            rowData[i%naxis1] = argb;
        }
        colorFitsImage = colorFitsImage.mirrored(false, true);
        // QGraphicsPixmapItem *item = new QGraphicsPixmapItem( QPixmap::fromImage(colorFitsImage));
        // CHECK if that works
        pixmapItem = new QGraphicsPixmapItem( QPixmap::fromImage(colorFitsImage));

        scene->clear();
        scene->addItem(pixmapItem);
    }
    else {
        qDebug() << "IView::mapFITS(): Invalid mode in mapFITS()";
    }
    // Replot the source and reference catalogs (if the corresponding actions are checked)
    if (sourceCatShown && ui->actionSourceCat->isVisible()) {
        ui->actionSourceCat->setChecked(true);
        showSourceCat();
    }
    if (refCatShown && ui->actionRefCat->isVisible()) {
        ui->actionRefCat->setChecked(true);
        showReferenceCat();
    }
    if (displayMode == "FITScolor") {
        if (G2shown) {
            colordw->ui->G2referencesPushButton->setChecked(true);
            showG2References(true);
        }
    }
}

void IView::compressDynrange(const QVector<float> &fitsdata, unsigned char *intdata, float colorCorrectionFactor)
{
    float rescale = 255. / (dynRangeMax - dynRangeMin);
    float tmpdata;
    float fitsdataCorrected;
    // NOT THREADSAFE!
    //#pragma omp parallel for
    for (long i=0; i<naxis1*naxis2; ++i) {
        // Truncate dynamic range
        fitsdataCorrected = fitsdata.at(i) * colorCorrectionFactor;
        if (fitsdataCorrected > dynRangeMax) tmpdata = dynRangeMax;
        else if (fitsdataCorrected < dynRangeMin) tmpdata = dynRangeMin;
        else tmpdata = fitsdataCorrected;
        // Compress to uchar (not thread safe)
        intdata[i] = (unsigned char) ((tmpdata-dynRangeMin) * rescale);
    }
}

void IView::updateColorViewExternal(float redFactor, float blueFactor)
{
    colordw->colorFactorZeropoint[0] = redFactor;
    colordw->colorFactorZeropoint[1] = 1.0;
    colordw->colorFactorZeropoint[2] = blueFactor;
    QString red = QString::number(redFactor, 'f', 3);
    QString blue = QString::number(blueFactor, 'f', 3);
    colordw->ui->redFactorLineEdit->setText(red);
    colordw->ui->blueFactorLineEdit->setText(blue);
    colordw->textToSlider(red, "red");
    colordw->textToSlider(blue, "blue");
    updateColorViewInternal(redFactor, blueFactor);
}

void IView::updateColorViewInternal(float redFactor, float blueFactor)
{
    if (displayMode != "FITScolor") return;
    colordw->colorFactorApplied[0] = redFactor;
    colordw->colorFactorApplied[1] = 1.0;
    colordw->colorFactorApplied[2] = blueFactor;
    mapFITS();
}

void IView::showSourceCat()
{
    // Leave if no image is displayed

    sourcecatSourcesShown = false;

    if (scene->items().isEmpty()) return;

    if (ui->actionSourceCat->isChecked()) {
        /*
                QString imageName = imageList.at(currentId);
                QFileInfo fi(imageName);
                QString baseName = fi.completeBaseName();
                // The catalog is also valid for skysubtracted images
                if (baseName.endsWith(".sub")) baseName.chop(4);
                */

        QString chipName = imageListChipName.at(currentId);
        QFile catalog(dirName+"/cat/iview/"+chipName+".iview");
        QString line;
        QStringList lineList;

        qreal x;
        qreal y;
        qreal size;
        QPen pen(QColor("#00ff66"));
        int penWidth = 2./icdw->zoom2scale(zoomLevel);
        penWidth  = penWidth < 1 ? 1 : penWidth;
        pen.setWidth(penWidth);
        QPoint point;

        // Refresh item list
        sourceCatItems.clear();
        // Read all source positions
        if ( catalog.open(QIODevice::ReadOnly)) {
            QTextStream stream( &catalog );
            while ( !stream.atEnd() ) {
                line = stream.readLine().simplified();
                // skip header lines
                if (line.contains("#")) continue;
                lineList = line.split(" ");
                x = lineList.at(0).toFloat();
                // must flip y
                y = naxis2 - lineList.at(1).toFloat();
                size = 10.*lineList.at(2).toFloat();   // (factor 3 if using flux radius)
                // correct for an offset introduced by where the scene draws the ellipses
                if (size<5.) size = 5.;   // Lower limit for symbol size
                if (size>20.) size = 20.; // Upper limit for symbol size
                point.setX(x-0.5*size);
                // Not sure where the +1 comes from. Perhaps from the flip and counting from 0 or one?
                point.setY(y-0.5*size+1.);
                myGraphicsView->mapToScene(point); // Not sure this is needed; Uncomment if objects are not plotted in the right position
                sourceCatItems.append(scene->addEllipse(point.x(), point.y(), size, size, pen));

                /*
                        // Does not draw ellipses in the right position. Some offset...
                        qreal aell = 6.*lineList.at(2).toFloat();
                        qreal bell = 6.*lineList.at(3).toFloat();
                        qreal theta = lineList.at(4).toFloat();
                        QGraphicsEllipseItem *ellipse = scene->addEllipse(point.x(), point.y(), aell, bell, pen);
                        ellipse->setTransformOriginPoint(x+1,y+1.);
                        ellipse->setRotation(-theta);
                        sourceCatItems.append(ellipse);
                        */
            }
            catalog.close();
        }
    }
    else {
        if (!sourceCatItems.isEmpty()) {
            for (auto &it: sourceCatItems) scene->removeItem(it);
            sourceCatItems.clear();
        }
    }
    if (!sourceCatItems.isEmpty()) sourcecatSourcesShown = true;
    else sourcecatSourcesShown = false;

    myGraphicsView->setScene(scene);
    myGraphicsView->show();
}

void IView::showReferenceCat()
{
    refcatSourcesShown = false;
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;

    int symbolSize = 15;
    QColor color = QColor("#ff3300");
    int width = 2. / icdw->zoom2scale(zoomLevel);
    width = width < 1 ? 1 : width;

    if (!refCatItems.isEmpty()) {
        for (auto &it: refCatItems) scene->removeItem(it);
        refCatItems.clear();
    }

    if (ui->actionRefCat->isChecked()) {
        refCatItems.clear();
        // Try and read the reference catalog in the standard relative position
        if (!readRaDecCatalog(dirName+"/cat/refcat/theli_mystd.iview", refCatItems, symbolSize, width, color)) {
            // Perhaps we are looking at a coadded image. Then go one directory up first
            if (!readRaDecCatalog(dirName+"/../cat/refcat/theli_mystd.iview", refCatItems, symbolSize, width, color)) {
                // No overlap with data field? Let the user provide one manually:
                QString refcatFileName =
                        QFileDialog::getOpenFileName(this, tr("Select reference catalog (theli_mystd.iview)"), dirName,
                                                     "theli_mystd.iview");
                if (QFile(refcatFileName).exists()) {
                    readRaDecCatalog(refcatFileName, refCatItems, symbolSize, width, color);
                }
            }
        }
    }
    else {
        // Remove any previous catalog display.
        if (!refCatItems.isEmpty()) {
            for (auto &it: refCatItems) scene->removeItem(it);
            refCatItems.clear();
        }
    }
    if (!refCatItems.isEmpty()) refcatSourcesShown = true;
    else refcatSourcesShown = false;

    myGraphicsView->setScene(scene);
    myGraphicsView->show();
}

void IView::showG2References(bool checked)
{
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;
    if (G2referencePathName.isEmpty()) return;

    if (checked) {
        QDir calibDir(G2referencePathName);
        QStringList calibSourcesList = calibDir.entryList(QStringList("PHOTCAT_sources_matched*.iview"));

        // Clear the item list
        G2refCatItems.clear();
        // Read the catalogs, append to the item list with different symbols
        int width = 2;
        for (auto &it : calibSourcesList) {
            if (it.contains("SDSS")) readRaDecCatalog(G2referencePathName+it, G2refCatItems, 29, width, QColor("#ee0000"));
            if (it.contains("PANSTARRS")) readRaDecCatalog(G2referencePathName+it, G2refCatItems, 24, width, QColor("#00eeee"));
            if (it.contains("APASS")) readRaDecCatalog(G2referencePathName+it, G2refCatItems, 19, width, QColor("#00ee00"));
        }
    }
    else {
        // Remove any previous catalog display.
        if (!G2refCatItems.isEmpty()) {
            for (auto &it: G2refCatItems) scene->removeItem(it);
            G2refCatItems.clear();
        }
    }

    myGraphicsView->setScene(scene);
    myGraphicsView->show();
}

bool IView::readRaDecCatalog(QString fileName, QList<QGraphicsRectItem*> &items, double size, int width, QColor color)
{
    QString line;
    QStringList lineList;
    double ra;
    double dec;
    double x;
    double y;
    QPen pen(color);
    pen.setWidth(width);
    QPoint point;

    // Read all source positions
    QFile file(fileName);
    if ( file.open(QIODevice::ReadOnly)) {
        QTextStream stream( &file );
        while ( !stream.atEnd() ) {
            line = stream.readLine().simplified();
            // skip header lines
            if (line.contains("#")) continue;
            lineList = line.split(" ");
            ra = lineList.at(0).toDouble();
            dec = lineList.at(1).toDouble();
            sky2xy(ra, dec, x, y);
            // correct for an offset introduced by where the scene draws the ellipses (or rectangles)
            point.setX(x-0.5*size);
            point.setY(y-0.5*size);
            myGraphicsView->mapToScene(point);
            //            qDebug() << point.x() << naxis1 << point.y() << naxis2 << ra << dec << x << y << size;
            // only show reference sources within the image boundaries
            if (point.x() >= 0 && point.x() <= naxis1
                    && point.y() >= 0 && point.y() <= naxis2) {
                //               qDebug() << x << point.x() << y << point.y() << ra << dec;
                items.append(scene->addRect(point.x(), point.y(), size, size, pen));
                //                qDebug() << "itemAdded";
            }
        }
        file.close();
        if (!items.isEmpty()) return true;
        else {
            return false;
        }
    }
    else {
        //        qDebug() << "IView::readRaDecCatalog: could not open" << fileName;
        return false;
    }
}

void IView::sky2xy(double alpha, double delta, double &x, double &y)
{
    if (!wcsInit) return;

    double world[2];
    double phi;
    double theta;
    double imgcrd[2];
    double pixcrd[2];
    world[0] = alpha;
    world[1] = delta;
    int stat[1];

    wcss2p(wcs, 1, 2, world, &phi, &theta, imgcrd, pixcrd, stat);
    x = pixcrd[0];
    y = naxis2 - pixcrd[1];
}

QString IView::dec2hex(double angle)
{
    double hh;
    double mm;
    double ss;
    int sign;

    sign = (angle < 0.0 ? -1 : 1);
    angle *= sign;
    angle = 60.0 * modf(angle, &hh);
    ss = 60.0 * modf(angle, &mm);
    QString h = QString::number(hh, 'f', 0);
    QString m = QString::number(mm, 'f', 0);
    QString s = QString::number(ss, 'f', 3);
    if (hh < 10) h.prepend("0");
    if (mm < 10) m.prepend("0");
    if (ss < 10) s.prepend("0");
    QString p;
    if (sign > 0) p = "+";
    else p = "-";
    return p+h+":"+m+":"+s;
}

void IView::getImageStatistics(QString colorMode)
{
    // Quasi-random sampling an array at every dim_small pixel
    int ranStep = 2./3.*naxis1 + sqrt(naxis1);
    QVector<double> subSample;
    if (displayMode == "FITSmonochrome" || displayMode == "MEMview") get_array_subsample(fitsData, subSample, ranStep);
    else if (displayMode == "FITScolor") {
        // Must have read the red and green channel already
        if (colorMode == "blueChannel") {
            QVector<float> fitsDataSum(naxis1*naxis2);
            // NOT THREADSAFE
            // #pragma omp parallel for
            for (long i=0; i<naxis1*naxis2; ++i) {
                fitsDataSum[i] = fitsDataR.at(i) + fitsDataG.at(i) + fitsDataB.at(i);
            }
            get_array_subsample(fitsDataSum, subSample, ranStep);
        }
        else return;
    }
    else {
        qDebug() << "IView::getImageStatistics(): Invalid mode";
    }

    medVal = medianMask_T(subSample, QVector<bool>(), "ignoreZeroes");
    rmsVal = madMask_T(subSample, QVector<bool>(), "ignoreZeroes");

    int validDigits = 4-log(fabs(medVal))/log(10);
    if (validDigits < 0) validDigits = 0;
    icdw->ui->medianLabel->setText("Median = "+QString::number(medVal, 'f', validDigits));
    icdw->ui->rmsLabel->setText("stdev  = "+QString::number(rmsVal*1.486, 'f', validDigits));
}

void IView::autoContrast(QString colorMode)
{
    // set medVal and rmsVal;
    if (displayMode == "FITSmonochrome" || displayMode == "MEMview") getImageStatistics();
    else if (displayMode == "FITScolor") {
        // Must have read the red and green channel already
        if (colorMode == "blueChannel" || allChannelsRead)
            getImageStatistics(colorMode);
        else return;
    }
    else {
        qDebug() << "IView::autoContrast(): Invalid displayMode:" << displayMode;
    }

    dynRangeMin = medVal - 2.*rmsVal;
    dynRangeMax = medVal + 10.*rmsVal;
    int validDigits = 3-log(fabs(dynRangeMax))/log(10);
    if (validDigits<0) validDigits = 0;
    icdw->ui->minLineEdit->setText(QString::number(dynRangeMin,'f',validDigits));
    icdw->ui->maxLineEdit->setText(QString::number(dynRangeMax,'f',validDigits));
}


void IView::writePreferenceSettings()
{
    QSettings settings("IVIEW", "PREFERENCES");
    settings.setValue("zoomFitPushButton", icdw->ui->zoomFitPushButton->isChecked());
    settings.setValue("autocontrastPushButton", icdw->ui->autocontrastPushButton->isChecked());
}

// TODO: make this dependent on which dockwidget is shown, otherwise we'll get valgrind issues
void IView::readPreferenceSettings()
{
    QSettings settings("IVIEW", "PREFERENCES");
    if (!displayMode.contains("SCAMP")) {
        icdw->ui->zoomFitPushButton->setChecked(settings.value("zoomFitPushButton").toBool());
        icdw->ui->autocontrastPushButton->setChecked(settings.value("autocontrastPushButton").toBool());
    }
}

void IView::on_actionDragMode_triggered()
{
    setMiddleMouseMode("DragMode");             // exclusive button group in c'tor does not work!
    emit middleMouseModeChanged("DragMode");
}

void IView::on_actionSkyMode_triggered()
{
    setMiddleMouseMode("SkyMode");              // exclusive button group in c'tor does not work!
    emit middleMouseModeChanged("SkyMode");
}

void IView::on_actionWCSMode_triggered()
{
    setMiddleMouseMode("WCSMode");              // exclusive button group in c'tor does not work!
    emit middleMouseModeChanged("WCSMode");
}

void IView::solutionAcceptanceStateReceived(bool state)
{
    emit solutionAcceptanceState(state);
    scampCorrectlyClosed = true;
    this->close();
}

void IView::autoContrastPushButton_toggled_receiver(bool checked)
{
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;

    if (checked) {
        autoContrast();

        // Must clear all items (sky circles) on the scene, then redraw afterwards
        if (!skyCircleItems.isEmpty()) skyCircleItems.clear();
        if (!acceptedSkyCircleItems.isEmpty()) {
            for (auto &it : acceptedSkyCircleItems) {
                scene->removeItem(it);
            }
        }
        mapFITS();
        myGraphicsView->setScene(scene);
        myGraphicsView->show();
        redrawSkyCirclesAndCats();
    }
}

void IView::replotCatalogAfterZoom()
{
    bool sourceCatShown = ui->actionSourceCat->isChecked();
    bool refCatShown = ui->actionRefCat->isChecked();
    clearItems();
    if (sourceCatShown && ui->actionSourceCat->isVisible()) {
        ui->actionSourceCat->setChecked(true);
        showSourceCat();
    }
    if (refCatShown && ui->actionRefCat->isVisible()) {
        ui->actionRefCat->setChecked(true);
        showReferenceCat();
    }
}

void IView::zoomFitPushButton_clicked_receiver(bool checked)
{
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;

    if (checked) {
        myGraphicsView->fitInView(scene->items(Qt::AscendingOrder).at(0), Qt::KeepAspectRatio);
        double scale = myGraphicsView->transform().m11();
        QString scaleFactor;
        if (scale > 1.) scaleFactor = "Zoom level: "+QString::number(scale,'f',2)+":1";
        else scaleFactor = "Zoom level: 1:"+QString::number(1./scale,'f',2);
        icdw->ui->zoomLabel->setText(scaleFactor);
    }

    replotCatalogAfterZoom();
}

void IView::zoomInPushButton_clicked_receiver()
{
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;

    ++zoomLevel;
    myGraphicsView->resetMatrix();
    myGraphicsView->scale(icdw->zoom2scale(zoomLevel), icdw->zoom2scale(zoomLevel));

    replotCatalogAfterZoom();
}

void IView::zoomOutPushButton_clicked_receiver()
{
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;

    --zoomLevel;
    myGraphicsView->resetMatrix();
    myGraphicsView->scale(icdw->zoom2scale(zoomLevel), icdw->zoom2scale(zoomLevel));

    replotCatalogAfterZoom();
}

void IView::zoomZeroPushButton_clicked_receiver()
{
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;

    zoomLevel = 0;
    // Do this to update the zoom label
    icdw->zoom2scale(zoomLevel);
    myGraphicsView->resetMatrix();

    replotCatalogAfterZoom();
}

void IView::minmaxLineEdit_returnPressed_receiver(QString rangeMin, QString rangeMax)
{
    dynRangeMin = rangeMin.toFloat();
    dynRangeMax = rangeMax.toFloat();
    mapFITS();
    myGraphicsView->setScene(scene);
    myGraphicsView->show();
}

void IView::colorFactorChanged_receiver(QString redFactor, QString blueFactor)
{
    // Re-emit the signal from the dock widget to the color calibration dialog
    emit colorFactorChanged(redFactor, blueFactor);

    // Apply color factors:
    mapFITS();
}
