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
#include "dockwidgets/ivstatisticsdockwidget.h"
#include "dockwidgets/ivfinderdockwidget.h"
#include "ui_ivconfdockwidget.h"
#include "ui_ivcolordockwidget.h"
#include "ui_ivstatisticsdockwidget.h"
#include "ui_ivfinderdockwidget.h"

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
        ui->actionMaskingMode->setChecked(false);
        myGraphicsView->middleMouseMode = "SkyMode";
        hideWCSdockWidget();
        icdw->ui->quitPushButton->show();
    }
    else if (mode == "DragMode") {
        ui->actionDragMode->setChecked(true);
        ui->actionSkyMode->setChecked(false);
        ui->actionWCSMode->setChecked(false);
        ui->actionMaskingMode->setChecked(false);
        middleMouseMode = "DragMode";
        hideWCSdockWidget();
        icdw->ui->quitPushButton->hide();
    }
    else if (mode == "WCSMode") {
        ui->actionWCSMode->setChecked(true);
        ui->actionSkyMode->setChecked(false);
        ui->actionDragMode->setChecked(false);
        ui->actionMaskingMode->setChecked(false);
        middleMouseMode = "WCSMode";
        showWCSdockWidget();
        icdw->ui->quitPushButton->hide();
    }
    else if (mode == "MaskingMode") {
        ui->actionMaskingMode->setChecked(true);
        ui->actionSkyMode->setChecked(false);
        ui->actionDragMode->setChecked(false);
        ui->actionWCSMode->setChecked(false);
        middleMouseMode = "MaskingMode";
        hideWCSdockWidget();
        icdw->ui->quitPushButton->hide();
    }
}

void IView::sendWCStoWCSdockWidget()
{
    if (!wcsInit) return;

    // Catching spectroscopic exposures or other exposures with invalid "imaging" WCS
    if (!wcs) {
        wcsdw->init();
        return;
    }

    // Copy the CD matrix to the WCS dock widget and init()
    wcsdw->cd11_orig = wcs->cd[0];
    wcsdw->cd12_orig = wcs->cd[1];
    wcsdw->cd21_orig = wcs->cd[2];
    wcsdw->cd22_orig = wcs->cd[3];
    wcsdw->crpix1_orig = wcs->crpix[0];
    wcsdw->crpix2_orig = wcs->crpix[1];
    wcsdw->crval1_orig = wcs->crval[0];
    wcsdw->crval2_orig = wcs->crval[1];
    wcsdw->init();
}

void IView::showWCSdockWidget()
{
    if (!wcs || ! wcsInit) {
        QMessageBox msgBox;
        msgBox.setText("WCS not found");
        msgBox.setInformativeText("This image does not contain any WCS information. The WCS dialog will not be shown.\n\n");
        msgBox.addButton(tr("Ok"), QMessageBox::YesRole);
        msgBox.exec();
        wcsdw->setDisabled(true);
        ui->actionWCSMode->setChecked(false);
        return;
    }

    if (wcsdw->isVisible()) return;

    sendWCStoWCSdockWidget();

    addDockWidget(Qt::LeftDockWidgetArea, wcsdw);
    wcsdw->setFloating(false);
    wcsdw->raise();
    wcsdw->show();

    if (!ui->actionRefCat->isChecked()) {
        // otherwise showReferenceCat() exits
        ui->actionRefCat->setChecked(true);
    }
    showReferenceCat();
    if (refCatItems.isEmpty()) {
        QMessageBox msgBox;
        msgBox.setText("No reference sources available");
        msgBox.setInformativeText("You must retrieve a reference catalog first before using the WCS module.\n\n");
        msgBox.addButton(tr("Ok"), QMessageBox::YesRole);
        msgBox.exec();
        wcsdw->setDisabled(true);
        ui->actionRefCat->setChecked(false);
        return;
    }
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
    if (filterName.isEmpty() || filterLineEdit->text().isEmpty()) {
        if (!displayMode.contains("SCAMP")) filterName = "*.fits";
        else filterName = "*.png";
        filterLineEdit->setText(filterName);
    }
    setImageList(filterName);
    currentId = imageList.indexOf(QFileInfo(filename).fileName());
    if (currentId == -1) {
        qDebug() << "IView::getCurrentId(): Image not found in list." << filterName << numImages << filename;
    }
}

void IView::loadImage()
{
    QString filter = filterLineEdit->text();
    if (filter.isEmpty()
            || !filter.contains(".fit")
            || !filter.contains("*")) {
        filter = "*.fits";
        filterLineEdit->setText(filter);
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
    filterLineEdit->setText(filter);

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
    scene->removeCrosshair();
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
    icdw->clearBinnedSceneReceiver();
    icdw->clearMagnifiedSceneReceiver();
}

void IView::loadFITSexternal(QString fileName, QString filter)
{
    if (!fileName.isEmpty()) {
        switchMode("FITSmonochrome");
        filterName = filter;
        filterLineEdit->setText(filterName);
        loadFITS(fileName);
    }
    else {
        switchMode("CLEAR");
        filterName = filter;
        filterLineEdit->setText(filterName);
    }
}

// invoked when clicking a data point in image statistics
void IView::loadFITSexternalRAM(int index)
{
    switchMode("FITSmonochrome");
    bool sourcecatShown = sourcecatSourcesShown;
    bool refcatShown = refcatSourcesShown;
    clearItems();

    loadFromRAM(myImageList[index], 0);
    currentId = index;
    setCatalogOverlaysExternally(sourcecatShown, refcatShown);
    redrawSkyCirclesAndCats();
}

void IView::loadPNG(QString filename, int currentId)
{
    if (imageList.isEmpty() || dirName.isEmpty()) {
        qDebug() << __func__ << " No scamp checkplots found!";
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
    pageLabel->setText(QString::number(currentId+1)+" / "+QString::number(numImages));

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
        //        myGraphicsView->centerOn(pixmapItem);
        myGraphicsView->show();
        myGraphicsView->setMinimumSize(200,200);
        myGraphicsView->setMaximumSize(10000,10000);

        QString prependPath = dirName;
        removeLastCharIf(prependPath, '/');
        QString path = getLastWord(prependPath,'/');
        this->setWindowTitle("iView ---   "+path+"/ ---  "+showName);
        pageLabel->setText(QString::number(currentId+1)+" / "+QString::number(numImages));

        // Load the binned image to the navigator
        emit updateNavigatorBinned(binnedPixmapItem);       // binnedPixmapItem created in mapFITS()
        emit updateNavigatorBinnedWCS(wcs, wcsInit);
        emit newImageLoaded();
    }
    else {
        // At end of file list, or file does not exist anymore.
        // Update file list
        filterLineEdit_textChanged("dummy");
        return;
    }
}

/*
void IView::loadFromRAMlist(const QModelIndex &index)
{
    loadFromRAM(myImageList[index.row()], index.column());

    // Get the center image poststamp; copy() refers to the top left corner, and then width and height
    QPixmap magnifiedPixmap = pixmapItem->pixmap().copy(naxis1/2.-icdw->navigator_magnified_nx/2.,
                                                        naxis2/2.-icdw->navigator_magnified_ny/2.,
                                                        icdw->navigator_magnified_nx, icdw->navigator_magnified_ny);
    magnifiedPixmapItem = new QGraphicsPixmapItem(magnifiedPixmap);

    // Update the navigator magnified window with the center image poststamp
    emit updateNavigatorMagnified(magnifiedPixmapItem, icdw->zoom2scale(zoomLevel)*magnify, 0., 0.);
}
*/

// Receiver of Navigator currentMousePos Eevent
void IView::updateNavigatorMagnifiedReceived(QPointF point)
{
    // Cap the magnification
    qreal magnification = icdw->zoom2scale(zoomLevel)*magnify;
    if (magnification > magnify) magnification = magnify;

    float dx = 0.;
    float dy = 0.;

    if (point.x() < 0 || point.x() > naxis1 || point.y() < 0 || point.y() > naxis2) {
        emit clearMagnifiedScene();
        return;
    }

    if (displayMode == "FITSmonochrome" || displayMode == "MEMview") {
        //        qDebug() << point.x() + 1. - icdw->navigator_magnified_nx/2./magnification << point.y() + 1. - icdw->navigator_magnified_ny/2./magnification <<
        //                icdw->navigator_magnified_nx/magnification << icdw->navigator_magnified_ny/magnification;
        if (point.x() + 1. - icdw->navigator_magnified_nx/2./magnification < 0) dx = point.x() + 1. - icdw->navigator_magnified_nx/2./magnification;
        if (point.x() + 1. - icdw->navigator_magnified_nx/2./magnification >= naxis1) dx = point.x() + 1. - icdw->navigator_magnified_nx/2./magnification;
        QPixmap magnifiedPixmap = pixmapItem->pixmap().copy(point.x() + 1. - icdw->navigator_magnified_nx/2./magnification,
                                                            point.y() + 1. - icdw->navigator_magnified_ny/2./magnification,
                                                            icdw->navigator_magnified_nx/magnification, icdw->navigator_magnified_ny/magnification);
        magnifiedPixmapItem = new QGraphicsPixmapItem(magnifiedPixmap);
        //        qDebug() << magnifiedPixmap.width() << magnifiedPixmap.height() << icdw->navigator_magnified_nx/magnification << icdw->navigator_magnified_ny/magnification;
    }
    else if (displayMode == "FITScolor") {
        QPixmap magnifiedPixmap = pixmapItem->pixmap().copy(point.x() + 1. - icdw->navigator_magnified_nx/2/magnification,
                                                            point.y() + 1. - icdw->navigator_magnified_ny/2/magnification,
                                                            icdw->navigator_magnified_nx/magnification, icdw->navigator_magnified_ny/magnification);
        magnifiedPixmapItem = new QGraphicsPixmapItem(magnifiedPixmap);
    }
    else {
        // Do not emit signal for PNG mode
        return;
    }
    emit updateNavigatorMagnified(magnifiedPixmapItem, magnification, dx, dy);
}

void IView::loadFromRAM(MyImage *it, int indexColumn)
{
    scene->removeCrosshair();
    clearAxes();

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
    finderdw->dateObs = it->dateobs;
    finderdw->geoLon = it->geolon;
    finderdw->geoLat = it->geolat;

    sendWCStoWCSdockWidget();
    this->setWindowTitle("iView --- Memory viewer : "+it->chipName);

    connect(currentMyImage, &MyImage::WCSupdated, this, &IView::WCSupdatedReceived);

    // Get the dynamic range
    // Normal viewer mode
    if (dataIntSet) {
        delete[] dataInt;
        dataInt = nullptr;
    }
    dataInt = new unsigned char[naxis1*naxis2];
    dataIntSet = true;
    getGlobalImageStatistics();
    // AUTO
    if (icdw->ui->minLineEdit->text().isEmpty()
            || icdw->ui->maxLineEdit->text().isEmpty()
            || icdw->ui->autocontrastPushButton->isChecked()) {
        autoContrast();
    }
    // MANUAL
    else {
        // get background statisics (median and sd)
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
    currentFileName = currentMyImage->baseName+".fits";

    // Get the center image poststamp; copy() refers to the top left corner, and then width and height
    QPixmap magnifiedPixmap = pixmapItem->pixmap().copy(naxis1/2.-icdw->navigator_magnified_nx/2.,
                                                        naxis2/2.-icdw->navigator_magnified_ny/2.,
                                                        icdw->navigator_magnified_nx, icdw->navigator_magnified_ny);
    magnifiedPixmapItem = new QGraphicsPixmapItem(magnifiedPixmap);
    // Update the navigator magnified window with the center image poststamp
    emit updateNavigatorMagnified(magnifiedPixmapItem, icdw->zoom2scale(zoomLevel)*magnify, 0., 0.);

    // Update the navigator binned window with the binned poststamp
    emit updateNavigatorBinned(binnedPixmapItem);
    emit updateNavigatorBinnedWCS(wcs, wcsInit);
    emit newImageLoaded();
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
        qDebug() << __func__ << "One or more of the three color channels could not be read!";
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

    icdw->ui->quitPushButton->hide();

    // Update the navigator binned window with the binned poststamp
    emit updateNavigatorBinned(binnedPixmapItem);
    emit updateNavigatorBinnedWCS(wcs, wcsInit);
}

bool IView::loadFITSdata(QString filename, QVector<float> &data, QString colorMode)
{
    if (displayMode.contains("SCAMP") || displayMode == "CLEAR") {
        return false;
    }

    scene->removeCrosshair();
    clearAxes();

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
    finderdw->dateObs = currentMyImage->dateobs;
    finderdw->geoLon = currentMyImage->geolon;
    finderdw->geoLat = currentMyImage->geolat;
    (void) wcsset(wcs);
    wcsInit = true;
    sendWCStoWCSdockWidget();

    connect(currentMyImage, &MyImage::WCSupdated, this, &IView::WCSupdatedReceived);

    // Move the data from the transient MyImage over to the class member.
    data.swap(currentMyImage->dataCurrent);        // 'fitsData' in the rest of the code

    // Get the dynamic range
    // Normal viewer mode
    if (displayMode == "FITSmonochrome") {
        if (dataIntSet) {
            delete[] dataInt;
            dataInt = nullptr;
        }
        dataInt = new unsigned char[naxis1*naxis2];
        dataIntSet = true;
        // determine best dynamic range for display (not yet applied)
        // AUTO
        getGlobalImageStatistics();
        if (icdw->ui->minLineEdit->text().isEmpty()
                || icdw->ui->maxLineEdit->text().isEmpty()
                || icdw->ui->autocontrastPushButton->isChecked()) {
            autoContrast();
        }
        // MANUAL
        else {
            // get background statisics (medVal and rmsVal)
            dynRangeMin = icdw->ui->minLineEdit->text().toFloat();
            dynRangeMax = icdw->ui->maxLineEdit->text().toFloat();
        }
    }
    else if (displayMode == "FITScolor") {
        if (colorMode == "redChannel" && dataIntRSet) {
            delete[] dataIntR;
            dataIntR = nullptr;
        }
        if (colorMode == "greenChannel" && dataIntGSet) {
            delete[] dataIntG;
            dataIntG = nullptr;
        }
        if (colorMode == "blueChannel" && dataIntBSet) {
            delete[] dataIntB;
            dataIntB = nullptr;
        }
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
        getGlobalImageStatistics(colorMode);
        autoContrast();
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

    //**************************************************
    // ADDITIONAL SECTION TO TEST TRANSFORMATIONS
    //    QTransform *transform = nullptr; // transformation maxtrix
    if(wcs) {
        // Transformation prameters, to be filled explicitly from WCS matrix
        // Must be computed with respect to global reference system valid for all images. Not sure how to do this
        /*
        qreal scale = 1.; // scaling factor
        qreal phi = 0.;   // rotation angle
        qreal dx = 0.;    // translation
        qreal dy = 0.;    // translation
        transform = new QTransform();

        // Adjust according to CD matrix
        qreal matrix[][3] = {
            {cd11, cd21, 0.0},
            {cd12, cd22, 0.0},
            {crpix1, crpix2, 1.0},
        };

        qreal matrix[][3] = {
            {1.0, 0.0, 0.0},
            {0.0, 1.0, 0.0},
            {0.0, 0.0, 1.0},
        };
        transform->setMatrix(matrix[0][0], matrix[0][1], matrix[0][2],
                matrix[1][0], matrix[1][1], matrix[1][2],
                matrix[2][0], matrix[2][1], matrix[2][2]);

        transform->rotate(0.);
        dx = naxis1/2. - (transform->m11()*naxis1/2.+transform->m21()*naxis2/2.);
        dy = naxis2/2. - (transform->m12()*naxis1/2.+transform->m22()*naxis2/2.);
        transform->translate(-wcs->crpix[0], -wcs->crpix[1]);
        */
    }
    else {
        // A WCS lock toggle button should be deactivated
    }

    // the scene MUST have it's final size BEFORE we add an item
    // WARNING: with some images (large coadds) then these images appear shifted to the right and cannot be viewed completely.
    //    if(scene->width() < 1 || scene->height() < 1)
    //        scene->setSceneRect( 0, 0, naxis1, naxis2);

    // end additional section
    //**************************************************

    clearItems();
    if (displayMode == "FITSmonochrome" || displayMode == "MEMview") {
        compressDynrange(fitsData, dataInt);
        QImage fitsImage(dataInt, naxis1, naxis2, naxis1, QImage::Format_Grayscale8);
        fitsImage = fitsImage.mirrored(false, true);
        // apply transformation if there is one
        // if(transform) fitsImage = fitsImage.transformed(*transform,Qt::SmoothTransformation);
        pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(fitsImage));
        // while transformation the translations gets lost because the image is cropped to its contents, so we have to translate afterwards
        //  if(transform) pixmapItem->setOffset(transform->m31(), transform->m32());
        //       qDebug() << transform->m11() << transform->m12() << transform->m13();
        //       qDebug() << transform->m21() << transform->m22() << transform->m23();
        //       qDebug() << transform->m31() << transform->m32() << transform->m33();
        scene->clear();
        scene->addItem(pixmapItem);
        myGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        myGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
    else if (displayMode == "FITScolor") {
        compressDynrange(fitsDataR, dataIntR, colordw->colorFactorApplied[0]);
        compressDynrange(fitsDataG, dataIntG, colordw->colorFactorApplied[1]);
        compressDynrange(fitsDataB, dataIntB, colordw->colorFactorApplied[2]);

        QImage colorFitsImage(naxis1, naxis2, QImage::Format_ARGB32);
        for (long i=0; i<naxis1*naxis2; ++i) {
            QRgb argb = qRgba( dataIntR[i], dataIntG[i], dataIntB[i], 255);
            QRgb* rowData = (QRgb*) colorFitsImage.scanLine(i/naxis1);
            rowData[i%naxis1] = argb;
        }
        colorFitsImage = colorFitsImage.mirrored(false, true);
        // if (transform) colorFitsImage = colorFitsImage.transformed(*transform,Qt::SmoothTransformation);
        // QGraphicsPixmapItem *item = new QGraphicsPixmapItem( QPixmap::fromImage(colorFitsImage));
        // CHECK if that works
        pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(colorFitsImage));
        // if(transform) pixmapItem->setOffset(transform->m31(), transform->m32());
        scene->clear();
        scene->addItem(pixmapItem);
    }
    else {
        qDebug() << __func__ << "Invalid mode in mapFITS()";
    }

    binnedPixmapItem = new QGraphicsPixmapItem(pixmapItem->pixmap().scaled(icdw->navigator_binned_nx, icdw->navigator_binned_ny, Qt::KeepAspectRatio, Qt::FastTransformation));

    /*
    if (transform) {
        delete transform;
        transform = nullptr;
    }
    */

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
    finderdw->bypassResolver();
}

void IView::compressDynrange(const QVector<float> &fitsdata, unsigned char *intdata, float colorCorrectionFactor)
{
    float rescale = 255. / (dynRangeMax - dynRangeMin);
    float tmpdata;
    // NOT THREADSAFE!
    //#pragma omp parallel for
    long i=0;
    for (auto &it : fitsdata) {
        // Truncate dynamic range
        float fitsdataCorrected = it * colorCorrectionFactor;
        if (fitsdataCorrected > dynRangeMax) tmpdata = dynRangeMax;
        else if (fitsdataCorrected < dynRangeMin) tmpdata = dynRangeMin;
        else tmpdata = fitsdataCorrected;
        // Compress to uchar (not thread safe)
        intdata[i] = (unsigned char) ((tmpdata-dynRangeMin) * rescale);
        ++i;
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
        int symbolSize = 15;
        if (!readRaDecCatalog(dirName+"/cat/refcat/theli_mystd.iview", refCatItems, symbolSize, width, color)) {
            // Perhaps we are looking at a coadded image. Then go one directory up first
            if (!readRaDecCatalog(dirName+"/../cat/refcat/theli_mystd.iview", refCatItems, symbolSize, width, color)) {
                // No overlap with data field? Let the user provide one manually:
                QString refcatFileName =
                        QFileDialog::getOpenFileName(this, tr("Select reference catalog (theli_mystd.iview)"), dirName,
                                                     "theli_mystd.iview");
                if (QFile(refcatFileName).exists()) {
                    if (!readRaDecCatalog(refcatFileName, refCatItems, symbolSize, width, color)) {
                        qDebug() << __func__ << " : Could not read manually provided reference catalog.";
                        // Remove any previous catalog display.
                        if (!refCatItems.isEmpty()) {
                            for (auto &it: refCatItems) scene->removeItem(it);
                            refCatItems.clear();
                        }
                    }
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
        wcsdw->setDisabled(true);
    }
    if (!refCatItems.isEmpty()) {
        refcatSourcesShown = true;
        wcsdw->setEnabled(true);
    }
    else {
        refcatSourcesShown = false;
        wcsdw->setDisabled(true);
    }

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
            if (it.contains("ATLAS-REFCAT2")) readRaDecCatalog(G2referencePathName+it, G2refCatItems, 19, width, QColor("#00ee00"));
            if (it.contains("SKYMAPPER")) readRaDecCatalog(G2referencePathName+it, G2refCatItems, 29, width, QColor("#eeee00"));
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

void IView::showAbsPhotReferences(bool checked)
{
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;

    if (checked) {
        // Clear previous items
        if (!AbsPhotRefCatItems.isEmpty()) {
            for (auto &it: AbsPhotRefCatItems) scene->removeItem(it);
            AbsPhotRefCatItems.clear();
        }

        QString dirName = AbsPhotReferencePathName;
        QString catNameMatched = "ABSPHOT_sources_matched.iview";
        QString catNameDownloaded = "ABSPHOT_sources_downloaded.iview";

        if (AbsPhotReferencePathName.isEmpty()) {
            qDebug() << __func__ << "No path name defined for absolute reference catalog. AbsPhot references will not be displayed.";
            return;
        }

        // Clear the item list
        AbsPhotRefCatItems.clear();
        int width = 2;
        readRaDecCatalog(dirName+"/"+catNameDownloaded, AbsPhotRefCatItems, 20, width, QColor("#ee0000"));
        //        readRaDecCatalog(dirName+"/"+catNameDownloaded, AbsPhotRefCatItems, 18, width, QColor("#000000"));
        readRaDecCatalog(dirName+"/"+catNameMatched, AbsPhotRefCatItems, 20, width, QColor("#eeee00"));
        //        readRaDecCatalog(dirName+"/"+catNameMatched, AbsPhotRefCatItems, 18, width, QColor("#000000"));
    }
    else {
        // Remove any previous catalog display.
        if (!AbsPhotRefCatItems.isEmpty()) {
            for (auto &it: AbsPhotRefCatItems) scene->removeItem(it);
            AbsPhotRefCatItems.clear();
        }
    }

    myGraphicsView->setScene(scene);
    myGraphicsView->show();
}

bool IView::readRaDecCatalog(QString fileName, QList<QGraphicsRectItem*> &items, double size, int width, QColor color)
{
    QString line;
    QStringList lineList;
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
            double ra = lineList.at(0).toDouble();
            double dec = lineList.at(1).toDouble();
            double x = 0.;
            double y = 0.;
            sky2xyQImage(ra, dec, x, y);
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
        // error handling in caller function
        return false;
    }
}

// returns coordinates in QImage system!
void IView::sky2xyQImage(double alpha, double delta, double &x, double &y)
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

// returns coordinates in FITS
void IView::sky2xyFITS(double alpha, double delta, double &x, double &y)
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
    y = pixcrd[1];
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

void IView::getGlobalImageStatistics(QString colorMode)
{
    // Quasi-random sampling an array at every dim_small pixel
    int ranStep = 2./3.*naxis1 + sqrt(naxis1);

    QVector<double> subSample;
    if (displayMode == "FITSmonochrome" || displayMode == "MEMview") {
        get_array_subsample(fitsData, subSample, ranStep);
        // GLOBAL statistics
        globalMedian = medianMask_T(subSample, QVector<bool>(), "ignoreZeroes");
        globalMean = meanMask_T(subSample, QVector<bool>());
        globalRMS = 1.486*madMask_T(subSample, QVector<bool>(), "ignoreZeroes");
    }
    else if (displayMode == "FITScolor") {
        QVector<double> subSampleR;
        QVector<double> subSampleG;
        QVector<double> subSampleB;
        if (colorMode == "redChannel") {
            get_array_subsample(fitsDataR, subSampleR, ranStep);
            globalMedianR = medianMask_T(subSampleR, QVector<bool>(), "ignoreZeroes");
        }
        if (colorMode == "greenChannel") {
            get_array_subsample(fitsDataG, subSampleG, ranStep);
            globalMedianG = medianMask_T(subSampleG, QVector<bool>(), "ignoreZeroes");
        }
        if (colorMode == "blueChannel") {
            get_array_subsample(fitsDataB, subSampleB, ranStep);
            globalMedianB = medianMask_T(subSampleB, QVector<bool>(), "ignoreZeroes");
            // In addition, compute overall statistics across all three bands (once the blue is read, we have all of them available)
            get_array_subsample(fitsDataR, subSampleR, ranStep);
            get_array_subsample(fitsDataG, subSampleG, ranStep);
            for (long i=0; i<subSampleR.length(); ++i) {
                subSampleR[i] += (subSampleG[i] + subSampleB[i]);
            }
            globalMedian = medianMask_T(subSampleR, QVector<bool>(), "ignoreZeroes");
            globalMean = meanMask_T(subSampleR, QVector<bool>());
            globalRMS = 1.486*madMask_T(subSampleR, QVector<bool>(), "ignoreZeroes");
        }
    }
}

void IView::autoContrast()
{
    dynRangeMin = globalMedian - 2.*globalRMS;
    dynRangeMax = globalMedian + 10.*globalRMS;
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
        clearAxes();
        scene->removeCrosshair();
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

        emit updateNavigatorBinned(binnedPixmapItem);
        emit updateNavigatorBinnedWCS(wcs, wcsInit);
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
    scene->zoomScale = icdw->zoom2scale(zoomLevel);
    finderdw->bypassResolver();
}

void IView::zoomFitPushButton_clicked_receiver(bool checked)
{
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;

    if (checked) {
        myGraphicsView->fitInView(scene->items(Qt::AscendingOrder).at(0), Qt::KeepAspectRatio);
        double scale = myGraphicsView->transform().m11();
        QString scaleFactor;
        if (scale > 1.) scaleFactor = QString::number(scale,'f',2)+":1";
        else scaleFactor = "1:"+QString::number(1./scale,'f',2);
        //        icdw->ui->zoomValueLabel->setText(scaleFactor);
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
    clearAxes();
    dynRangeMin = rangeMin.toFloat();
    dynRangeMax = rangeMax.toFloat();
    mapFITS();
    redrawUpdateAxes();  // redshift axes
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

void IView::filterLineEdit_textChanged(const QString &arg1)
{
    QString filter = arg1;

    if (filter.isEmpty()
            || !filter.contains(".fits")
            || !filter.contains("*")) {
        filter = "*.fits";
        filterLineEdit->setText(filter);
    }

    setImageList(filter);
    numImages = imageList.length();

    pageLabel->setText("? / "+QString::number(numImages));

    // Rewind
    //    iview->startAction_triggered();
}

void IView::fovCenterChangedReceiver(QPointF newCenter)
{
    QPointF center = binnedToQimage(newCenter);
    //    qDebug() << newCenter << center;
    myGraphicsView->centerOn(center);
}

void IView::targetResolvedReceived(QString alphaStr, QString deltaStr)
{
    if (alphaStr.contains(":")) alphaStr = hmsToDecimal(alphaStr);
    if (deltaStr.contains(":")) deltaStr = dmsToDecimal(deltaStr);

    double x = 0.;
    double y = 0.;
    sky2xyQImage(alphaStr.toDouble(), deltaStr.toDouble(), x, y);
    if (x > 0. && x < naxis1-1 && y > 0. && y < naxis2-1) {
        myGraphicsView->centerOn(QPointF(x,y));
        scene->addCrosshair(x,y);
    }
    else {
/*
 *         if (sender() == finderdw->ui->targetresolverToolButton) {
            finderdw->ui->targetNameNonsiderealLineEdit->setText("outside f.o.v");
        }
        if (sender() == finderdw->ui->MPCresolverToolButton) {
            finderdw->ui->targetNameSiderealLineEdit->setText("outside f.o.v");
        }
*/
    }
}

void IView::WCSupdatedReceived()
{
    sendWCStoWCSdockWidget();
    icdw->receiveWCS(wcs, wcsInit);
}
