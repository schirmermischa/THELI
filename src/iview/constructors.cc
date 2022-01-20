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
#include "ui_ivconfdockwidget.h"
#include "ui_ivcolordockwidget.h"
#include "ui_ivredshiftdockwidget.h"
#include "dockwidgets/ivscampdockwidget.h"
#include "dockwidgets/ivcolordockwidget.h"
#include "dockwidgets/ivredshiftdockwidget.h"

#include "../tools/tools.h"

#include "fitsio2.h"

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
#include <QScreen>

// Various constructors

// Open the window without loading an image
// Not sure I'm using that anywhere
// mode = CLEAR
IView::IView(QWidget *parent) :
    QMainWindow(parent),
    displayMode("CLEAR"),
    ui(new Ui::IView)
{

    //    qDebug() << "mode 1";
    ui->setupUi(this);
    initGUI();

    icdw->zoom2scale(zoomLevel);

    // get a list of all FITS files in the current directory
    QDir dir = dir.current();
    dirName = dir.absolutePath();

    filterName = "*.fits";
    setImageList(filterName);
    makeConnections();
    switchMode();
    initGUIstep2();
}

// MEMview, single MyImage
IView::IView(QString mode, QWidget *parent) :
    QMainWindow(parent),
    displayMode(mode),         // "MEMview", set externally
    ui(new Ui::IView)
{
    ui->setupUi(this);
    initGUI();

    // Hide redshift action
    ui->actionRedshift->setVisible(false);

    //    qDebug() << "mode memview single image:" << mode;

    icdw->zoom2scale(zoomLevel);
    /*
    // get a list of all FITS files in the current directory
    QDir dir = dir.current();
    dirName = dir.absolutePath();

    filterName = "*.fits";
    setImageList(filterName);
    */
    makeConnections();
    switchMode();
    initGUIstep2();

    fromMemory = true;

    // Can't use these in memory mode (ensuring internal consistency, and avoiding very complex code)
    // filterLabel->hide();
    // filterLineEdit->hide();
}

// MEMview, list of MyImages
IView::IView(QString mode, QList<MyImage*> &list, QString dirname, QWidget *parent) :
    QMainWindow(parent),
    displayMode(mode),
    ui(new Ui::IView)
{
    ui->setupUi(this);
    initGUI();

    // Hide redshift action
    ui->actionRedshift->setVisible(false);

    //    qDebug() << "mode memview list of images:" << mode;

    icdw->zoom2scale(zoomLevel);

    myImageList = list;
    numImages = myImageList.length();
    setImageListFromMemory();
    dirName = dirname;

    fromMemory = true;

    // 'currentId' is initialised externally in memoryViewer
    /*
    // get a list of all FITS files in the current directory
    QDir dir = dir.current();
    dirName = dir.absolutePath();

    filterName = "*.fits";
    setImageList(filterName);
    */
    makeConnections();
    switchMode();
    initGUIstep2();

    // Can't use these in memory mode (ensuring internal consistency, and avoiding very complex code)
    // filterLabel->hide();
    // filterLineEdit->hide();
}

// FITS viewer, commandline invocation, with or without an argument
// mode == FITSmonochrome
IView::IView(QString mode, QString name, QWidget *parent) :
    QMainWindow(parent),
    displayMode(mode),
    ui(new Ui::IView)
{
    ui->setupUi(this);
    initGUI();

    if (displayMode == "FITSmonochrome") {
        icdw->zoom2scale(zoomLevel);
        QFileInfo fileInfo(name);
        QString suffix = fileInfo.suffix();
        QDir dir;
        if (suffix == "") dir = dir.current();
        else dir = fileInfo.absoluteDir();
        dirName = dir.absolutePath();

        filterName = "*.fits";
        setImageList(filterName);
        makeConnections();
        switchMode();
        currentFileName = name;
        if (!name.isEmpty()) loadFITS(name);
    }
    else if (displayMode == "SCAMP") {
        dirName = name;
        scampInteractiveMode = true;

        filterName = "*.png";
        setImageList(filterName);
        makeConnections();
        switchMode();

        currentId = 0;
        loadPNG("", currentId);
    }
    else if (displayMode == "SCAMP_VIEWONLY") {          // just to display the plots, whenever
        dirName = name;
        scampInteractiveMode = false;

        filterName = "*.png";
        setImageList(filterName);
        makeConnections();
        switchMode();

        currentId = 0;
        loadPNG("", currentId);
    }

    // TODO: if no refcat subdir found then hide the sourcecat / refcat buttons

    initGUIstep2();
}

/*
// FITS viewer, commandline invocation, with or without an argument
// mode == FITSmonochrome
IView::IView(QString mode, QString fileName, QWidget *parent) :
    QMainWindow(parent),
    displayMode(mode),
    ui(new Ui::IView)
{
    ui->setupUi(this);
    initGUI();

    QFileInfo fileInfo(fileName);
    QString suffix = fileInfo.suffix();
    QDir dir;
    if (suffix == "") dir = dir.current();
    else dir = fileInfo.absoluteDir();
    dirName = dir.absolutePath();

    filterName = "*.fits";
    setImageList(filterName);
    makeConnections();
    switchMode();

    if (!fileName.isEmpty()) loadFITS(fileName);

    // TODO: if no refcat subdir found then hide the sourcecat / refcat buttons

    initGUIstep2();
}
*/

/*
// Scamp checkplot viewer
// mode == "SCAMP"
IView::IView(QString mode, QString dirname, QWidget *parent) :
    QMainWindow(parent),
    dirName(dirname),
    displayMode(mode),
    scampInteractiveMode(true),
    ui(new Ui::IView)
{
    ui->setupUi(this);
    initGUI();

    // get a list of all PNG files in this directory
    filterName = "*.png";
    setImageList(filterName);
    makeConnections();

    currentId = 0;
    switchMode();
    loadPNG("", currentId);

    initGUIstep2();
}
*/

// Used by imstatistics (opening iview without data point selection)
// mode = FITSmonochrome
IView::IView(QString mode, QString dirname, QString filter, QWidget *parent) :
    QMainWindow(parent),
    dirName(dirname),
    filterName(filter),
    displayMode(mode),
    ui(new Ui::IView)
{
    ui->setupUi(this);

    initGUI();
    filterLineEdit->setText(filterName);

    // Hide redshift action
    ui->actionRedshift->setVisible(false);

    icdw->zoom2scale(zoomLevel);

    // get a list of all FITS files in this directory
    setImageList(filterName);

    pageLabel->setText("? / "+QString::number(numImages));
    makeConnections();
    switchMode();

    if (numImages > 0) {
        currentFileName = imageList[0];
        loadFITS(dirName+'/'+imageList[0]);
    }
    initGUIstep2();
}

// Used by imstatistics (opening iview with a data point selected)
// mode = FITSmonochrome
IView::IView(QString mode, QString dirname, QString fileName, QString filter, QWidget *parent) :
    QMainWindow(parent),
    dirName(dirname),
    filterName(filter),
    currentFileName(fileName),
    displayMode(mode),
    ui(new Ui::IView)
{
    ui->setupUi(this);
    initGUI();

    // Hide redshift action
    ui->actionRedshift->setVisible(false);

    // qDebug() << "mode imstatistics 2:" << mode << filterName;

    filterLineEdit->setText(filterName);
    icdw->zoom2scale(zoomLevel);

    // get a list of all FITS files in this directory
    setImageList(filterName);
    pageLabel->setText("? / "+QString::number(numImages));

    makeConnections();
    switchMode();
    loadFITS(dirName+'/'+fileName);
    initGUIstep2();
}


// Displaying color images
// mode = FITScolor
IView::IView(QString mode, QString dirname, QString rChannel, QString gChannel, QString bChannel,
             float factorR, float factorB, QWidget *parent) :
    QMainWindow(parent),
    dirName(dirname),
    filterName("*.fits"),
    displayMode(mode),
    ui(new Ui::IView),
    ChannelR(rChannel),
    ChannelG(gChannel),
    ChannelB(bChannel)
{
    ui->setupUi(this);
    initGUI();

    // Hide redshift action
    ui->actionRedshift->setVisible(false);

    // The list of the three FITS images
    imageList << ChannelR << ChannelG << ChannelB;
    numImages = imageList.length();

    // Update the dockwidget
    colordw->colorFactorZeropoint[0] = factorR;
    colordw->colorFactorZeropoint[1] = 1.0;
    colordw->colorFactorZeropoint[2] = factorB;
    colordw->colorFactorApplied[0] = factorR;
    colordw->colorFactorApplied[1] = 1.0;
    colordw->colorFactorApplied[2] = factorB;
    colordw->ui->redFactorLineEdit->setText(QString::number(factorR, 'f', 3));
    colordw->ui->blueFactorLineEdit->setText(QString::number(factorB, 'f', 3));

    makeConnections();
    switchMode();
    loadColorFITS(icdw->zoom2scale(zoomLevel));
    initGUIstep2();

    /*
    ui->toolBar->addWidget(redResetPushButton);
    ui->toolBar->addWidget(redFactorLineEdit);
    ui->toolBar->addWidget(redSlider);
    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(blueResetPushButton);
    ui->toolBar->addWidget(blueFactorLineEdit);
    ui->toolBar->addWidget(blueSlider);

    redSlider->setMinimum(0);
    redSlider->setMaximum(sliderSteps);
    redSlider->setValue(sliderSteps/2);
    redSlider->setMinimumWidth(150);
    redSlider->setMaximumWidth(250);
    redFactorLineEdit->setText(QString::number(factorR, 'f', 3));
    redFactorLineEdit->setMinimumWidth(120);
    redFactorLineEdit->setMaximumWidth(120);
    redResetPushButton->setText("Reset red factor");
    blueSlider->setMinimum(0);
    blueSlider->setMaximum(sliderSteps);
    blueSlider->setValue(sliderSteps/2);
    blueSlider->setMinimumWidth(150);
    blueSlider->setMaximumWidth(250);
    blueFactorLineEdit->setText(QString::number(factorB, 'f', 3));
    blueFactorLineEdit->setMinimumWidth(120);
    blueFactorLineEdit->setMaximumWidth(120);
    blueResetPushButton->setText("Reset blue factor");
    */
}

// Force the user to make a decision in case we are in interactive mode
// viewing the scamp checkplots, and the user wants to close the dialog
// by closing the window directly, not by accepting or rejecting a solution
void IView::closeEvent(QCloseEvent *event)
{
    //    qDebug() << fitsData.capacity();
    //    fitsData.clear();
    //    fitsData.squeeze();

    timer->stop();

    if (scampInteractiveMode && !scampCorrectlyClosed) {
        QMessageBox msgBox;
        msgBox.setText("Accept or reject the astrometric solution.");
        msgBox.setInformativeText("THELI needs to know whether to update the FITS headers with "
                                  "the astrometric solution or not. If in doubt, click reject. "
                                  "You can re-run the astrometric solution at any time. \n\n");
        QAbstractButton *pButtonAccept = msgBox.addButton(tr("Accept"), QMessageBox::YesRole);
        QAbstractButton *pButtonReject = msgBox.addButton(tr("Reject"), QMessageBox::YesRole);
        QAbstractButton *pButtonAgain = msgBox.addButton(tr("Show me again"), QMessageBox::YesRole);
        msgBox.exec();
        if (msgBox.clickedButton()==pButtonAccept) emit solutionAcceptanceState(true);
        else if (msgBox.clickedButton()==pButtonReject) emit solutionAcceptanceState(false);
        else if (msgBox.clickedButton()==pButtonAgain) {
            event->ignore();
            return;
        }
    }

    writePreferenceSettings();
    emit closed();

    event->accept();
}

IView::~IView()
{
    if (dataIntSet) {
        delete [] dataInt;
        dataInt = nullptr;
    }
    if (dataIntRSet) {
        delete [] dataIntR;
        dataIntR = nullptr;
    }
    if (dataIntGSet) {
        delete [] dataIntG;
        dataIntG = nullptr;
    }
    if (dataIntBSet) {
        delete [] dataIntB;
        dataIntB = nullptr;
    }
    if (dataBinnedIntSet) {
        delete [] dataBinnedInt;
        dataBinnedInt = nullptr;
    }
    if (dataBinnedIntRSet) {
        delete [] dataBinnedIntR;
        dataBinnedIntR = nullptr;
    }
    if (dataBinnedIntGSet) {
        delete [] dataBinnedIntG;
        dataBinnedIntG = nullptr;
    }
    if (dataBinnedIntBSet) {
        delete [] dataBinnedIntB;
        dataBinnedIntB = nullptr;
    }
    // It appears that 'scene' does not take ownership of the pixmapitem.
    // nullptr if e.g. closing the GUI without saving the images. Don't know how that can be related, but here we go ...
    if (pixmapItem != nullptr) {
        delete pixmapItem;
        pixmapItem = nullptr;
    }
    //    if (binnedPixmapItem != nullptr) {
    //        delete binnedPixmapItem;
    //        binnedPixmapItem = nullptr;
    //    }
    if (magnifiedPixmapItem != nullptr) {
        //        delete magnifiedPixmapItem;
        //        magnifiedPixmapItem = nullptr;
    }
    delete ui;
    //    if (icdwDefined) delete icdw;
    //    if (scampdwDefined) delete scampdw;

    if (!fromMemory) {
        if (wcsInit) wcsfree(wcs);     //  wcs is a pointer to MyImage if loaded from memory, hence we must not delete it here!
    }
    // delete wcs;
    delete myGraphicsView;
    delete scene;
    myGraphicsView = nullptr;
    scene = nullptr;
}

void IView::makeConnections()
{
    connect(ui->actionClose, &QAction::triggered, this, &IView::close);
    connect(ui->actionLoadImageFromDrive, &QAction::triggered, this, &IView::loadImage);
    connect(ui->actionStart, &QAction::triggered, this, &IView::startAction_triggered);
    connect(ui->actionEnd, &QAction::triggered, this, &IView::endAction_triggered);
    connect(ui->actionPrevious, &QAction::triggered, this, &IView::previousAction_triggered);
    connect(ui->actionNext, &QAction::triggered, this, &IView::nextAction_triggered);
    connect(ui->actionForward, &QAction::triggered, this, &IView::forwardAction_triggered);
    connect(ui->actionBack, &QAction::triggered, this, &IView::backAction_triggered);
    connect(ui->actionSourceCat, &QAction::triggered, this, &IView::showSourceCat);
    connect(ui->actionRefCat, &QAction::triggered, this, &IView::showReferenceCat);

    //    myGraphicsView = new MyGraphicsView(this);
    myGraphicsView = new MyGraphicsView();
    QPalette backgroundPalette;
    backgroundPalette.setColor(QPalette::Base, QColor("#000000"));
    myGraphicsView->setPalette(backgroundPalette);
    myGraphicsView->setMouseTracking(true);

    ui->graphicsLayout->addWidget(myGraphicsView);

    connect(myGraphicsView, &MyGraphicsView::currentMousePos, this, &IView::showCurrentMousePos);
    connect(myGraphicsView, &MyGraphicsView::currentMousePos, this, &IView::collectLocalStatisticsSample);
    connect(myGraphicsView, &MyGraphicsView::rightDragTravelled, this, &IView::adjustBrightnessContrast);
    connect(myGraphicsView, &MyGraphicsView::leftDragTravelled, this, &IView::drawSeparationVector);
    connect(myGraphicsView, &MyGraphicsView::middleSkyDragTravelled, this, &IView::drawSkyCircle);
    connect(myGraphicsView, &MyGraphicsView::middleMaskingDragTravelled, this, &IView::drawMaskingPolygon);
    connect(myGraphicsView, &MyGraphicsView::middleWCSTravelled, this, &IView::updateCRPIX);
    connect(myGraphicsView, &MyGraphicsView::middleWCSreleased, this, &IView::updateCRPIXFITS);
    connect(myGraphicsView, &MyGraphicsView::middlePressResetCRPIX, this, &IView::middlePressResetCRPIXreceived);
    connect(myGraphicsView, &MyGraphicsView::rightPress, this, &IView::initDynrangeDrag);
    connect(myGraphicsView, &MyGraphicsView::leftPress, this, &IView::initSeparationVector);
    connect(myGraphicsView, &MyGraphicsView::leftPress, this, &IView::sendStatisticsCenter);
    connect(myGraphicsView, &MyGraphicsView::leftButtonReleased, this, &IView::clearSeparationVector);
    connect(myGraphicsView, &MyGraphicsView::middleButtonReleased, this, &IView::appendSkyCircle);
    connect(myGraphicsView, &MyGraphicsView::rightButtonReleased, this, &IView::redrawSkyCirclesAndCats);
    connect(myGraphicsView, &MyGraphicsView::leftButtonReleased, this, &IView::updateSkyCircles);
    connect(myGraphicsView, &MyGraphicsView::mouseEnteredView, icdw, &IvConfDockWidget::mouseEnteredViewReceived);
    connect(myGraphicsView, &MyGraphicsView::mouseLeftView, icdw, &IvConfDockWidget::mouseLeftViewReceived);
    connect(myGraphicsView, &MyGraphicsView::viewportChanged, this, &IView::viewportChangedReceived);
    connect(myGraphicsView, &MyGraphicsView::currentMousePos, this, &IView::updateNavigatorMagnifiedReceived);
    connect(myGraphicsView, &MyGraphicsView::currentMousePos, this, &IView::showWavelength);
    connect(myGraphicsView, &MyGraphicsView::leftDragTravelled, &restframeAxis, &MyAxis::redshiftSpectrum);
    connect(myGraphicsView, &MyGraphicsView::leftDragTravelled, &spectrumAxis, &MyAxis::redshiftSpectrum);
    connect(myGraphicsView, &MyGraphicsView::leftButtonReleased, &restframeAxis, &MyAxis::closeRedshift);
    connect(myGraphicsView, &MyGraphicsView::leftButtonReleased, &spectrumAxis, &MyAxis::closeRedshift);

    connect(scene, &MyGraphicsScene::itemDeleted, this, &IView::updateSkyCircles);
    connect(scene, &MyGraphicsScene::mouseLeftScene, icdw, &IvConfDockWidget::mouseLeftViewReceived);

    connect(wcsdw, &IvWCSDockWidget::CDmatrixChanged, this, &IView::updateCDmatrix);
    connect(wcsdw, &IvWCSDockWidget::CDmatrixChangedFITS, this, &IView::updateCDmatrixFITS);
    connect(wcsdw, &IvWCSDockWidget::CDmatrixChanged, icdw, &IvConfDockWidget::drawCompass);

    connect(statdw, &IvStatisticsDockWidget::visibilityChanged, this, &IView::updateStatisticsButton);
    connect(zdw, &IvRedshiftDockWidget::visibilityChanged, this, &IView::updateRedshiftButton);
    connect(finderdw, &IvFinderDockWidget::visibilityChanged, this, &IView::updateFinderButton);
    connect(finderdw, &IvFinderDockWidget::targetResolved, this, &IView::targetResolvedReceived);
    connect(finderdw, &IvFinderDockWidget::clearTargetResolved, scene, &MyGraphicsScene::removeCrosshair);

    connect(timer, &QTimer::timeout, this, &IView::forwardAction_triggered);
    connect(timer, &QTimer::timeout, this, &IView::backAction_triggered);

    connect(this, &IView::middleMouseModeChanged, myGraphicsView, &MyGraphicsView::updateMiddleMouseMode);
    connect(this, &IView::updateNavigatorMagnified, icdw, &IvConfDockWidget::updateNavigatorMagnifiedReceived);
    connect(this, &IView::updateNavigatorBinned, icdw, &IvConfDockWidget::updateNavigatorBinnedReceived);
    connect(this, &IView::updateNavigatorBinnedViewport, icdw, &IvConfDockWidget::updateNavigatorBinnedViewportReceived);
    connect(this, &IView::statisticsSampleAvailable, statdw, &IvStatisticsDockWidget::statisticsSampleReceiver);
    connect(this, &IView::statisticsSampleColorAvailable, statdw, &IvStatisticsDockWidget::statisticsSampleColorReceiver);
    connect(filterLineEdit, &QLineEdit::textChanged, this, &IView::filterLineEdit_textChanged);

    connect(icdw->binnedGraphicsView, &MyBinnedGraphicsView::fovRectCenterChanged, this, &IView::fovCenterChangedReceiver);
    connect(this, &IView::updateNavigatorBinnedWCS, icdw, &IvConfDockWidget::receiveWCS);
    connect(this, &IView::clearMagnifiedScene, icdw, &IvConfDockWidget::clearMagnifiedSceneReceiver);

    connect(this, &IView::newImageLoaded, this, &IView::updateAxes);
    connect(this, &IView::newImageLoaded, this, &IView::resetRedshift);
    connect(this, &IView::wavelengthUpdated, zdw, &IvRedshiftDockWidget::showWavelength);

    connect(&restframeAxis, &MyAxis::redshiftRecomputed, this, &IView::updateAxes);
    connect(&spectrumAxis, &MyAxis::redshiftRecomputed, this, &IView::updateAxes);
    connect(&spectrumAxis, &MyAxis::redshiftUpdated, zdw, &IvRedshiftDockWidget::redshiftUpdatedReceiver);
}

void IView::switchMode(QString mode)
{
    if (!mode.isEmpty()) displayMode = mode;

    ui->actionClose->setVisible(true);
    if (displayMode.contains("SCAMP")) {
        this->setWindowTitle("iView --- Scamp check plots");
        // Disable file menu. Must stay within interactive mode. Also, 'close' will cause segfault.
        ui->menuFile->setVisible(false);
        middleMouseActionGroup->setVisible(false);
        // many of the settings below are handled by the modeStackedWidget already!
        ui->actionSourceCat->setVisible(false);
        ui->actionRefCat->setVisible(false);
        ui->actionBack->setVisible(false);
        ui->actionForward->setVisible(false);
        pageLabel->show();
        speedLabel->hide();
        speedSpinBox->hide();
        ui->actionBack->setEnabled(true);
        ui->actionForward->setEnabled(true);
        ui->actionPrevious->setEnabled(true);
        ui->actionNext->setEnabled(true);
        ui->actionStart->setEnabled(true);
        ui->actionEnd->setEnabled(true);
    }
    else if (displayMode == "FITSmonochrome") {
        this->setWindowTitle("iView --- FITS file viewer");
        middleMouseActionGroup->setVisible(true);
        ui->actionSourceCat->setVisible(true);
        ui->actionRefCat->setVisible(true);
        pageLabel->show();
        speedLabel->show();
        speedSpinBox->show();
        ui->actionBack->setEnabled(true);
        ui->actionForward->setEnabled(true);
        ui->actionPrevious->setEnabled(true);
        ui->actionNext->setEnabled(true);
        ui->actionStart->setEnabled(true);
        ui->actionEnd->setEnabled(true);
    }
    else if (displayMode == "MEMview") {
        this->setWindowTitle("iView --- Memory viewer");
        middleMouseActionGroup->setVisible(true);
        ui->actionSourceCat->setVisible(true);
        ui->actionRefCat->setVisible(true);
        pageLabel->show();
        speedLabel->show();
        speedSpinBox->show();
        ui->actionLoadImageFromDrive->setDisabled(true);
        ui->actionBack->setEnabled(true);
        ui->actionForward->setEnabled(true);
        ui->actionPrevious->setEnabled(true);
        ui->actionNext->setEnabled(true);
        ui->actionStart->setEnabled(true);
        ui->actionEnd->setEnabled(true);
    }
    else if (displayMode == "FITScolor") {
        this->setWindowTitle("iView --- FITS file viewer");
        middleMouseActionGroup->setVisible(false);
        ui->actionSourceCat->setVisible(false);
        ui->actionRefCat->setVisible(false);
        pageLabel->hide();
        speedLabel->hide();
        speedSpinBox->hide();
        ui->actionBack->setVisible(false);
        ui->actionForward->setVisible(false);
        ui->actionPrevious->setVisible(false);
        ui->actionNext->setVisible(false);
        ui->actionStart->setVisible(false);
        ui->actionEnd->setVisible(false);
    }
    else if (displayMode == "CLEAR") {
        this->setWindowTitle("iView --- FITS file viewer");
        middleMouseActionGroup->setVisible(true);
        ui->actionBack->setDisabled(true);
        ui->actionForward->setDisabled(true);
        ui->actionPrevious->setDisabled(true);
        ui->actionNext->setDisabled(true);
        ui->actionStart->setDisabled(true);
        ui->actionEnd->setDisabled(true);
        ui->actionSourceCat->setDisabled(true);
        ui->actionRefCat->setDisabled(true);
    }

    // Also adjust the dockwidgets
    // NOT in SCAMP mode, because there it is not initialized!
    if (!displayMode.contains("SCAMP")) icdw->switchMode(displayMode);
}

void IView::initGUI()
{
    addDockWidgets();  // Widgets must be alive early on

    //    QRect rec = QApplication::desktop()->screenGeometry();  // deprecated in Qt5.14
    QScreen *screen = QGuiApplication::screens().at(0);
    QRect rec = screen->geometry();
    screenHeight = rec.height();
    screenWidth = rec.width();

    middleMouseActionGroup->setExclusive(true);
    middleMouseActionGroup->addAction(ui->actionDragMode);
    middleMouseActionGroup->addAction(ui->actionSkyMode);
    middleMouseActionGroup->addAction(ui->actionMaskingMode);
    middleMouseActionGroup->addAction(ui->actionWCSMode);
    ui->actionDragMode->setChecked(true);
    wcsdw->hide();

    if (displayMode.contains("SCAMP")) {
        middleMouseActionGroup->setVisible(false);     // NOTE: must add actions above before we can "hide" them simply by hiding the group
    }
    else {
        middleMouseActionGroup->setVisible(true);
    }

    readPreferenceSettings();
    setWindowIcon(QIcon(":/icons/iview.png"));

    //    QFile file(":/qss/default.qss");
    //    file.open(QFile::ReadOnly);
    //    QString styleSheet = QString::fromLatin1(file.readAll());
    //    qApp->setStyleSheet(styleSheet);

    this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
}

void IView::initGUIstep2()
{
    // update the startDirName
    if (!startDirNameSet) {
        startDirName = dirName;
        startDirNameSet = true;
    }

    if (!displayMode.contains("SCAMP")) {
        //        ui->toolBar->addWidget(speedLabel);
        //        speedLabel->setText(" Frame rate");
        ui->toolBar->addWidget(speedSpinBox);
        speedSpinBox->setValue(2);
        speedSpinBox->setMinimum(1);
        speedSpinBox->setMaximum(10);
        speedSpinBox->setSuffix(" Hz");
        speedSpinBox->setToolTip("Frame rate for forward / backward playing");

        ui->toolBar->addWidget(filterLabel);
        filterLabel->setText("  Filter");
        ui->toolBar->addWidget(filterLineEdit);
        filterLineEdit->setStatusTip("Only these images will be selected when using the yellow navigation buttons at the top.");
    }
    pageLabel->setMaximumWidth(200);
    pageLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    ui->toolBar->addWidget(pageLabel);
    ui->toolBar->addSeparator();
    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred);
    ui->toolBar->addWidget(empty);
}

void IView::addDockWidgets()
{
    setDockOptions(DockOption::AllowNestedDocks);

    if (displayMode == "FITSmonochrome" || displayMode == "MEMview") {
        icdw = new IvConfDockWidget(this);
        addDockWidget(Qt::LeftDockWidgetArea, icdw);
        icdw->setFloating(false);
        icdw->raise();
    }

    else if (displayMode == "FITScolor") {
        icdw = new IvConfDockWidget(this);
        addDockWidget(Qt::LeftDockWidgetArea, icdw);
        icdw->setFloating(false);
        icdw->raise();

        colordw = new IvColorDockWidget(this);
        addDockWidget(Qt::LeftDockWidgetArea, colordw);
        colordw->setFloating(false);
        colordw->raise();
    }

    else if (displayMode == "CLEAR") {
        icdw = new IvConfDockWidget(this);
        addDockWidget(Qt::LeftDockWidgetArea, icdw);
        icdw->setFloating(false);
        icdw->raise();
    }

    else if (displayMode == "SCAMP") {
        scampdw = new IvScampDockWidget(this);
        icdw = new IvConfDockWidget(this);
        addDockWidget(Qt::LeftDockWidgetArea, scampdw);
        scampdw->setFloating(false);
        icdw->hide();
        scampdw->raise();
        finderdw->hide();
        statdw->hide();
        zdw->hide();
    }
    else if (displayMode == "SCAMP_VIEWONLY") {
        icdw = new IvConfDockWidget(this);
        icdw->hide();
        finderdw->hide();
        statdw->hide();
        zdw->hide();
    }

    // Connections
    if (displayMode == "SCAMP") {
        scampdwDefined = true;
        connect(scampdw, &IvScampDockWidget::solutionAcceptanceState, this, &IView::solutionAcceptanceStateReceived);
    }
    else if (displayMode == "SCAMP_VIEWONLY") {
        scampdwDefined = false;
    }
    else {
        icdwDefined = true;
        connect(icdw, &IvConfDockWidget::autoContrastPushButton_toggled, this, &IView::autoContrastPushButton_toggled_receiver);
        connect(icdw, &IvConfDockWidget::minmaxLineEdit_returnPressed, this, &IView::minmaxLineEdit_returnPressed_receiver);
        connect(icdw, &IvConfDockWidget::minmaxLineEdit_returnPressed, this, &IView::minmaxLineEdit_returnPressed_receiver);
        connect(icdw, &IvConfDockWidget::zoomFitPushButton_clicked, this, &IView::zoomFitPushButton_clicked_receiver);
        connect(icdw, &IvConfDockWidget::zoomInPushButton_clicked, this, &IView::zoomInPushButton_clicked_receiver);
        connect(icdw, &IvConfDockWidget::zoomOutPushButton_clicked, this, &IView::zoomOutPushButton_clicked_receiver);
        connect(icdw, &IvConfDockWidget::zoomZeroPushButton_clicked, this, &IView::zoomZeroPushButton_clicked_receiver);
        connect(icdw, &IvConfDockWidget::zoomFitPushButton_clicked, this, &IView::updateAxes);
        connect(icdw, &IvConfDockWidget::zoomInPushButton_clicked, this, &IView::updateAxes);
        connect(icdw, &IvConfDockWidget::zoomOutPushButton_clicked, this, &IView::updateAxes);
        connect(icdw, &IvConfDockWidget::zoomZeroPushButton_clicked, this, &IView::updateAxes);
        connect(icdw, &IvConfDockWidget::closeIview, this, &IView::close);
    }

    if (!displayMode.contains("SCAMP")) {
        addDockWidget(Qt::LeftDockWidgetArea, statdw);
        statdw->hide();

        addDockWidget(Qt::LeftDockWidgetArea, finderdw);
        finderdw->hide();

        addDockWidget(Qt::LeftDockWidgetArea, zdw);
        zdw->hide();
    }
}

void IView::updateStatisticsButton()
{
    if (statdw->isVisible()) ui->actionImage_statistics->setChecked(true);
    else ui->actionImage_statistics->setChecked(false);
}

void IView::updateRedshiftButton()
{
    if (zdw->isVisible()) ui->actionRedshift->setChecked(true);
    else ui->actionRedshift->setChecked(false);
}

void IView::updateFinderButton()
{
    if (finderdw->isVisible()) {
        ui->actionFinder->setChecked(true);
    }
    else {
        ui->actionFinder->setChecked(false);
        scene->removeCrosshair();
    }
}
