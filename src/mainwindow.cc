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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"
#include "status.h"
#include "iview/iview.h"
#include "colorpicture/colorpicture.h"
#include "instrumentdefinition.h"
#include "tools/tools.h"
#include "instrumentdata.h"
#include "processingInternal/controller.h"
#include "processingInternal/data.h"
#include "dockwidgets/confdockwidget.h"
#include "dockwidgets/monitor.h"
#include "dockwidgets/memoryviewer.h"
#include "tools/cpu.h"
#include "ui_confdockwidget.h"
#include "ui_memoryviewer.h"

#include <QDebug>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QThread>
#include <QProgressBar>
#include <QStorageInfo>
#include <QTimer>
#include <QPixmap>

MainWindow::MainWindow(QString pid, QWidget *parent) :
    QMainWindow(parent),
    mainPID(pid),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // WARNING! THIS IS A VERY CAREFULLY BALANCED SETUP
    // TO DO THINGS IN THE RIGHT ORDER.

    // some of the routines called here are also used later on, e.g.
    // changing settings parameters and so on. For some of them we don't want
    // them to execute all of their code, for example when they are called during the first init of the GUI.
    doingInitialLaunch = true;

    // Setup the status object, and hide some GUI elements we don't need
    // for the user (but internally, to simplify code)
    status.init();

    initProcessingStatus();

    initEnvironment(thelidir, userdir, tmpdir);
    instrument_userDir = userdir+"/instruments_user/";
    instrument_dir = thelidir+"/src/config/instruments/";

    // Setup hardware
    QSysInfo *sysInfo = new QSysInfo;
    kernelType = sysInfo->kernelType(); // returns "linux" or "darwin"
    productName = sysInfo->prettyProductName().replace(" ","_");
    systemRAM = get_memory();
    delete sysInfo;

    QPalette palette;
    palette.setColor(QPalette::Base,QColor("#ffffff"));
    palette.setColor(QPalette::Background,QColor("#cccccc"));
    ui->statusBar->setPalette(palette);
    ui->statusBar->setAutoFillBackground(true);
    ui->menuBar->setPalette(palette);
    ui->menuBar->setAutoFillBackground(true);

    addProgressBars();

    establish_connections();

    // Meaningful overview comments shown when running a task
    populateTaskCommentMap();

    // must populate the instrument listview before reading the settings
    QString projectname;
    readPreferenceSettings(projectname);

    // the next one can be adjusted when manually loading a project file
    // actually, think it is not needed here because it must also be invoked by readGUIsettings() below
    // fill_setupInstrumentComboBox();

    // Create dock widgets (before reading settings)
    cdw = new ConfDockWidget(this);
    monitor = new Monitor(this);

    // perhaps before initializeGuiLayout?
    // Also calls fill_setupInstrumentComboBox()
    readGUISettings(projectname);

    // Update some confdockwidget displays according to settings
    cdw->toggle_skyparams();
    cdw->toggle_skyparamThresholds("");

    // If the user launches THELI the very first time, then on_setupInstrumentComboBox_clicked does
    // not get triggered (don't know why not); do it manually:
    if (instData.name.isEmpty()) on_setupInstrumentComboBox_clicked();

    // Fix some paint events triggered by readSettings().
    // The reason is that the settings are not written in order.
    // Settings change the dir names, triggering a paint event,
    // which results in red background for some elements because
    // the main dir is not set first. Hence the repaint.
    repaintDataDirs();

    // Initial configuration of MainWindow
    initGUI();

    // Repaint background of previously executed tasks.
    // initGUI loads the style sheet, thus defaulting to backgrounds
    status.history2checkbox();

    // this->setStyleSheet("QComboBox:hover { background-color: #99ccff };");

    // wipe the tmp directory upon every launch
    wipeTmpDir();

    // The entity that keeps track of the data, incl connections
    QString statusOld = status.getStatusFromHistory();

    // NOTE: setting up the controller may take a while (hen creating object masks)
    controller = new Controller(&instData, statusOld, cdw, monitor, this);
    // LineEdit changes update controller
    for (auto &it : status.listDataDirs) {
        //        connect(it, &QLineEdit::textChanged, controller, &Controller::updateSingle);
        connect(it, &QLineEdit::editingFinished, controller, &Controller::dataTreeEditedReceived);
        // editing finished not emitted if dirs are empty (because of the validator)
        connect(it, &QLineEdit::textChanged, this, &MainWindow::emitEditingFinished);
    }

    // Another dock widget, must be done after instantiating the controller
    memoryViewer = new MemoryViewer(controller, this);
    controller->memoryViewer = memoryViewer;
    controller->connectDataWithMemoryViewer();
    connect(controller, &Controller::clearMemoryView, memoryViewer, &MemoryViewer::clearMemoryViewReceived, Qt::DirectConnection);
    connect(controller, &Controller::populateMemoryView, memoryViewer, &MemoryViewer::populateMemoryViewReceived);
    connect(controller, &Controller::addBackupDirToMemoryviewer, memoryViewer, &MemoryViewer::addBackupDirReceived);

    // Combine mode changes update controller
    connect(cdw->ui->overscanMethodComboBox, &QComboBox::currentTextChanged, this, &MainWindow::updateControllerFunctors);
    connect(cdw->ui->biasMethodComboBox, &QComboBox::currentTextChanged, this, &MainWindow::updateControllerFunctors);
    connect(cdw->ui->darkMethodComboBox, &QComboBox::currentTextChanged, this, &MainWindow::updateControllerFunctors);
    connect(cdw->ui->flatoffMethodComboBox, &QComboBox::currentTextChanged, this, &MainWindow::updateControllerFunctors);
    connect(cdw->ui->flatMethodComboBox, &QComboBox::currentTextChanged, this, &MainWindow::updateControllerFunctors);
    connect(ui->setupInstrumentComboBox, &QComboBox::currentTextChanged, this, &MainWindow::updateController);
    // Initiate the controller functors;
    updateControllerFunctors("");
    // Internal processing
    connect(controller, &Controller::loadViewer, this, &MainWindow::launchViewer);
    connect(controller, &Controller::messageAvailable, monitor, &Monitor::displayMessage);
    connect(controller, &Controller::appendOK, monitor, &Monitor::appendOK);
    connect(controller, &Controller::showMessageBox, this, &MainWindow::showMessageBoxReceived);
    connect(controller, &Controller::progressUpdate, this, &MainWindow::progressUpdateReceived);
    connect(controller, &Controller::resetProgressBar, this, &MainWindow::resetProgressBarReceived);
    connect(controller, &Controller::targetResolved, cdw, &ConfDockWidget::targetResolvedReceived);
    connect(controller, &Controller::scienceDataDirUpdated, this, &MainWindow::scienceDataDirUpdatedReceived);
    connect(this, &MainWindow::rereadScienceDataDir, controller, &Controller::rereadScienceDataDirReceived);
    connect(this, &MainWindow::newProjectLoaded, controller, &Controller::newProjectLoadedReceived);
    connect(this, &MainWindow::messageAvailable, monitor, &Monitor::displayMessage);

    connect(ui->setupMainLineEdit, &QLineEdit::textChanged, cdw, &ConfDockWidget::updateAPIsolutions);
    connect(ui->setupStandardLineEdit, &QLineEdit::textChanged, cdw, &ConfDockWidget::updateAPIsolutions);
    connect(cdw, &QDockWidget::dockLocationChanged, this, &MainWindow::cdw_dockLocationChanged);
    connect(cdw, &QDockWidget::topLevelChanged, this, &MainWindow::cdw_topLevelChanged);
    // Must be done after readGUIsettings()
    addDockWidgets();

    doingInitialLaunch = false;

    /*
    QVector<bool> maskit(2410236, false);
    MyImage bla("/data1/hannah_0101.A-0694/BIN_2/FLAT/sextest/", "sub.fits", "", 0, maskit, false);
    bla.readImage(false);
    bla.backgroundModel(100, "interpolate");
    bla.segmentImage("5", "5", true, false);
    for (auto &object : bla.objectList) {
        qDebug() << object->X << object->Y << object->FLUX_RADIUS << object->MAG_AUTO << object->ALPHA_J2000
            << object->DELTA_J2000 << object->A << object->B << object->THETA << object->XWIN << object->YWIN << object->CXX << object->CYY << object->CXY;
    }
    */

    memoryViewer->populate();
    // Need this after connecting status dirs to paint checkboxes correctly.
    // Doesn't work if done further above, not sure why

    // Does the right thing (repaint check boxes), but it seems random at best. Only updates tabwidet shown
    status.updateStatus();

    update();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    bool accept = true;

    QString filepath = QDir::homePath()+"/.config/THELI/"+ui->setupProjectLineEdit->text()+".conf";

    if (writePreferenceSettings() != QSettings::NoError) {
        qDebug() << "closeEvent: Could not save preference settings.";
    }

    if (writeGUISettings() == QSettings::NoError) accept = accept && true;
    else if (writeGUISettings() == QSettings::AccessError) {
        const QMessageBox::StandardButton ret
                = QMessageBox::warning(this, tr("Application"),
                                       tr("A file access error occurred while writing the project configuration to\n")+
                                       filepath+"\n",
                                       QMessageBox::Ok | QMessageBox::Cancel);
        switch (ret) {
        case QMessageBox::Ok:
            accept = accept && true;
            break;
        case QMessageBox::Cancel:
            accept = accept && false;
            break;
        default:
            break;
        }
    }
    else if (writeGUISettings() == QSettings::FormatError) {
        const QMessageBox::StandardButton ret
                = QMessageBox::warning(this, tr("Application"),
                                       tr("A formatting error occurred when writing the project configuration to\n")+
                                       filepath+"\n",
                                       QMessageBox::Ok | QMessageBox::Cancel);
        switch (ret) {
        case QMessageBox::Ok:
            accept = accept && true;
            break;
        case QMessageBox::Cancel:
            accept = accept && false;
            break;
        default:
            break;
        }
    }

    // write out data in memory
    long numUnsavedImages = controller->checkForUnsavedImages();
    if (numUnsavedImages > 0) {
        long mBytes = numUnsavedImages*instData.storage;

        QMessageBox msgBox;
        msgBox.setModal(true);
        msgBox.setInformativeText(tr("Unsaved images detected.\n") +
                                  tr("The current project keeps ") +
                                  QString::number(numUnsavedImages) +
                                  tr(" processed images in memory that have not yet been saved to drive.\n\n"));
        QAbstractButton *pButtonSave = msgBox.addButton(tr("Save images (")+QString::number(mBytes)+" MB) and close", QMessageBox::YesRole);
        QAbstractButton *pButtonContinue = msgBox.addButton(tr("Close without saving"), QMessageBox::YesRole);
        QAbstractButton *pButtonCancel = msgBox.addButton(tr("Cancel"), QMessageBox::YesRole);
        msgBox.exec();
        if (msgBox.clickedButton() == pButtonSave) {
            if (sufficientSpaceAvailable(mBytes)) {
                controller->writeUnsavedImagesToDrive();
                accept = accept && true;
            }
            else {
                // sufficientSpaceAvailable() displays a warning message
                accept = accept && false;
            }
        }
        else if (msgBox.clickedButton() == pButtonContinue) {
            accept = accept && true;
        }
        else accept = accept && false;
    }

    if (accept) event->accept();
    else event->ignore();
}

void MainWindow::emitEditingFinished(const QString &arg1)
{
    QLineEdit* lineedit = qobject_cast<QLineEdit*>(sender());
    if (arg1.isEmpty()) lineedit->editingFinished();
}

MainWindow::~MainWindow()
{
    // delete class variables that have been assigned with 'new':
    delete instrument_model;
    delete ui;
}

void MainWindow::addProgressBars()
{
    // Add diskspace progress bar to the status bar and connect it to a timer that updates it repeatedly.
    // Don't have to delete the statusBar separately because it is a child of 'ui' and constructed after 'ui'
    driveProgressBar = new QProgressBar(this);
    driveProgressBar->setMinimumWidth(200);
    driveProgressBar->setMaximumWidth(200);
    cpuProgressBar = new QProgressBar(this);
    cpuProgressBar->setMinimumWidth(200);
    cpuProgressBar->setMaximumWidth(200);
    memoryProgressBar = new QProgressBar(this);
    memoryProgressBar->setMinimumWidth(200);
    memoryProgressBar->setMaximumWidth(200);
    ui->processProgressBar->setMinimumWidth(200);
    ui->processProgressBar->setMaximumWidth(200);

    // progress bars cannot be added to menuBar under MacOs (at least in 2011)
    /*
    if (kernelType == "darwin") ui->statusBar->addPermanentWidget(driveProgressBar, 0);
    else {
        // bar doesnt show under linux, either
        QWidgetAction* actionProgressBar = new QWidgetAction(this);
        actionProgressBar->setDefaultWidget(driveProgressBar);
        ui->menuBar->addAction(actionProgressBar);
    }
    */

    QSysInfo *sysInfo = new QSysInfo;
    QString kernelType = sysInfo->kernelType();

    // Push the progress bars to the right, with little spacers in between;
    QWidget* empty = new QWidget();
    QWidget* empty1 = new QWidget();
    QWidget* empty2 = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    empty1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    empty2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    empty1->setMinimumWidth(10);
    empty2->setMinimumWidth(10);
    if (kernelType == "linux") {
        ui->toolBar->addWidget(empty);
        ui->toolBar->addWidget(driveProgressBar);
        ui->toolBar->addWidget(empty1);
        ui->toolBar->addWidget(cpuProgressBar);
        ui->toolBar->addWidget(empty2);
        ui->toolBar->addWidget(memoryProgressBar);
    }
    else if (kernelType == "darwin") {        // Progress Bars not supported in macOS in the top toolBar
        ui->statusBar->addWidget(empty);
        ui->statusBar->addWidget(driveProgressBar);
        ui->statusBar->addWidget(empty1);
        ui->statusBar->addWidget(cpuProgressBar);
        ui->statusBar->addWidget(empty2);
        ui->statusBar->addWidget(memoryProgressBar);
    }
    delete sysInfo;
}

void MainWindow::addDockWidgets()
{
    addDockWidget(Qt::RightDockWidgetArea, cdw);
    cdw->setFloating(false);

    addDockWidget(Qt::RightDockWidgetArea, monitor);
    monitor->setFloating(false);

    addDockWidget(Qt::RightDockWidgetArea, memoryViewer);
    memoryViewer->setFloating(false);

    // Create tabbed dock widgets
    tabifyDockWidget(cdw, monitor);
    tabifyDockWidget(monitor, memoryViewer);

    cdw->raise();
}

void MainWindow::wipeTmpDir()
{
    QDir tmp(tmpdir);
    // SAFEGUARD
    if (tmp.exists() && tmpdir.contains(".theli/tmp")) {
        QStringList filter("*");
        QStringList fileList = tmp.entryList(filter);
        for (auto &it: fileList) {
            tmp.remove(it);
        }
    }
}

void MainWindow::resetProgressBarReceived()
{
    ui->processProgressBar->setValue(0);
}

void MainWindow::updateController()
{
    controller->instData = &instData;
    controller->updateAll();
}

void MainWindow::updateControllerFunctors(QString text)
{
    if (cdw->ui->overscanMethodComboBox->currentText() == "Line median") controller->combineOverscan_ptr = &medianMask;
    else if (cdw->ui->overscanMethodComboBox->currentText() == "Line mean") controller->combineOverscan_ptr = &meanMask;
    else controller->combineOverscan_ptr = nullptr;

    if (cdw->ui->biasMethodComboBox->currentText() == "Median")
        controller->combineBias_ptr = &medianMask;
    else controller->combineBias_ptr = &meanMask;

    if (cdw->ui->darkMethodComboBox->currentText() == "Median")
        controller->combineDark_ptr = &medianMask;
    else controller->combineDark_ptr = &meanMask;

    if (cdw->ui->flatoffMethodComboBox->currentText() == "Median")
        controller->combineFlatoff_ptr = &medianMask;
    else controller->combineFlatoff_ptr = &meanMask;

    if (cdw->ui->flatMethodComboBox->currentText() == "Median")
        controller->combineFlat_ptr = &medianMask;
    else controller->combineFlat_ptr = &meanMask;

    if (cdw->ui->BACmethodComboBox->currentText() == "Median")
        controller->combineBackground_ptr = &medianMask;
    else controller->combineBackground_ptr = &meanMask;
}

// currently not in use
bool MainWindow::maybeSave()
{
    const QMessageBox::StandardButton ret
            = QMessageBox::question(this, tr("Application"),
                                    tr("Reduced enough data ?"),
                                    QMessageBox::Ok | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Ok:
        return true;
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MainWindow::initGUI()
{
    QFile file(":/qss/default.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QString::fromLatin1(file.readAll());
    qApp->setStyleSheet(styleSheet);

    QIcon key(":/images/key.png");
    QIcon redoarrow(":/images/redoarrow.png");
    QIcon projectLoad(":/images/open_file.png");
    QIcon projectReset(":/images/reset.png");
    QIcon projectDataReset(":/images/db_reset.png");
    // configuration dialog

    ui->HDUreformatConfigureToolButton->setIcon(key);
    ui->calibratorConfigureToolButton->setIcon(key);
    ui->BACconfigureToolButton->setIcon(key);
    ui->chopnodConfigureToolButton->setIcon(key);
    ui->COCconfigureToolButton->setIcon(key);
    ui->binnedPreviewConfigureToolButton->setIcon(key);
    ui->globalweightConfigureToolButton->setIcon(key);
    ui->individualweightConfigureToolButton->setIcon(key);
    ui->separateConfigureToolButton->setIcon(key);
    ui->absphotomindirectConfigureToolButton->setIcon(key);
    ui->absphotomdirectConfigureToolButton->setIcon(key);
    ui->astromphotomConfigureToolButton->setIcon(key);
    ui->createsourcecatConfigureToolButton->setIcon(key);
    ui->skysubConfigureToolButton->setIcon(key);
    ui->coadditionConfigureToolButton->setIcon(key);

    ui->setupProjectLoadToolButton->setIcon(projectLoad);
    ui->setupProjectResetToolButton->setIcon(projectReset);
    ui->setupProjectResetDataToolButton->setIcon(projectDataReset);

    this->setWindowTitle("THELI GUI "+GUIVERSION+"      Project: "+ui->setupProjectLineEdit->text());

    // Check the status of currently selected tasks (if any; simulator mode)
    // and push a suitable message to plainTextedit
    on_startPushButton_clicked();

    // Mandatory checkboxes have yellow background
    // Don't know the parameter referring to the background color of the checkbox marker alone
    /*
    QList<QCheckBox*> mandatoryCheckboxList;
    mandatoryCheckboxList.append(ui->applyHDUreformatCheckBox);
    mandatoryCheckboxList.append(ui->applyGlobalweightCheckBox);
    mandatoryCheckboxList.append(ui->applyIndividualweightCheckBox);
    mandatoryCheckboxList.append(ui->applyCreatesourcecatCheckBox);
    mandatoryCheckboxList.append(ui->applyAstromphotomCheckBox);
    mandatoryCheckboxList.append(ui->applyCoadditionCheckBox);
    for (auto &it : mandatoryCheckboxList) {
        it->setStyleSheet("base: #fff669;");
    }
    */

    QIcon yield(":/images/yield_final.png");
    QIcon stop(":/images/stop_final.png");
    ui->yieldToolButton->setIcon(yield);
    ui->stopToolButton->setIcon(stop);
}

void MainWindow::establish_connections()
{
    // Other, 'singular' connectors

    // Data dirs
    for (auto &it : status.listDataDirs) {
        // LineEdits adjust their background color
        connect(it, &QLineEdit::textChanged, this, &MainWindow::repaintDataDirs);
        // LineEdits editing finished must update GUI settings
        connect(it, &QLineEdit::editingFinished, this, &MainWindow::writeGUISettings);
    }
    // TODO: move to confdockwidget
    // connect(ui->ARCselectimageLineEdit, &QLineEdit::textChanged, this, &MainWindow::repaintDataDirs);

    // Close the main GUI
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::shutDown);

    // Connect configureToolButtons with the confStackedWidget
    for (auto &it : status.listToolButtons) {
        connect(it, &QToolButton::clicked, this, &MainWindow::link_ConfToolButtons_confStackedWidget);
    }

    // Connect task checkboxes (load corresponding config page, and run simulator mode)
    for (auto &it : status.listCheckBox) {
        connect(it, &QCheckBox::clicked, this, &MainWindow::link_taskCheckBoxes_confStackedWidget);
        connect(it, &QCheckBox::clicked, this, &MainWindow::on_startPushButton_clicked);
    }

    // Connect actions
    connect(ui->actionPreferences, &QAction::triggered, this, &MainWindow::loadPreferences);
    connect(ui->actionAbsZP, &QAction::triggered, this, &MainWindow::load_dialog_abszeropoint);
    connect(ui->actioniView, &QAction::triggered, this, &MainWindow::loadIView);
    connect(ui->actionNew_instrument, &QAction::triggered, this, &MainWindow::load_dialog_newinst);
    connect(ui->actionColor_picture, &QAction::triggered, this, &MainWindow::load_dialog_colorpicture);
    connect(ui->actionImage_statistics, &QAction::triggered, this, &MainWindow::load_dialog_imageStatistics);

    // Connect validators
    connect_validators();

    connect(ui->setupProjectResetToolButton, &QPushButton::clicked, this, &MainWindow::reset);
    connect(ui->setupProjectResetDataToolButton, &QPushButton::clicked, this, &MainWindow::restoreOriginalData);
}

void MainWindow::cdw_dockLocationChanged(const Qt::DockWidgetArea &area)
{
    if (cdw->isFloating()) this->adjustSize();
}

void MainWindow::cdw_topLevelChanged(bool topLevel)
{
    // Minimum size for the MainWindow after DockWidget goes floating
    if (topLevel) this->adjustSize();
}

void MainWindow::updateDiskspaceWarning(int newLimit)
{
    diskwarnPreference = newLimit;
    datadiskspace_warned = false;
}

void MainWindow::updateServer(QString server)
{
    downloadServer = server;
}

void MainWindow::updateNumcpu(int cpu)
{
    numCPU = cpu;
}

void MainWindow::link_ConfToolButtons_confStackedWidget()
{
    QObject* obj = sender();
    cdw->raise();
    if (obj == ui->HDUreformatConfigureToolButton) cdw->ui->confStackedWidget->setCurrentIndex(0);
    else if (obj == ui->calibratorConfigureToolButton) cdw->ui->confStackedWidget->setCurrentIndex(0);
    else if (obj == ui->chopnodConfigureToolButton) cdw->ui->confStackedWidget->setCurrentIndex(1);
    else if (obj == ui->BACconfigureToolButton) cdw->ui->confStackedWidget->setCurrentIndex(1);
    else if (obj == ui->COCconfigureToolButton) cdw->ui->confStackedWidget->setCurrentIndex(2);
    else if (obj == ui->binnedPreviewConfigureToolButton) cdw->ui->confStackedWidget->setCurrentIndex(2);
    else if (obj == ui->globalweightConfigureToolButton) cdw->ui->confStackedWidget->setCurrentIndex(3);
    else if (obj == ui->individualweightConfigureToolButton) cdw->ui->confStackedWidget->setCurrentIndex(3);
    else if (obj == ui->separateConfigureToolButton) cdw->ui->confStackedWidget->setCurrentIndex(3);
    else if (obj == ui->absphotomindirectConfigureToolButton) cdw->ui->confStackedWidget->setCurrentIndex(4);
    else if (obj == ui->absphotomdirectConfigureToolButton) cdw->ui->confStackedWidget->setCurrentIndex(5);
    else if (obj == ui->createsourcecatConfigureToolButton) cdw->ui->confStackedWidget->setCurrentIndex(5);
    else if (obj == ui->astromphotomConfigureToolButton) cdw->ui->confStackedWidget->setCurrentIndex(6);
    else if (obj == ui->skysubConfigureToolButton) cdw->ui->confStackedWidget->setCurrentIndex(7);
    else if (obj == ui->coadditionConfigureToolButton) cdw->ui->confStackedWidget->setCurrentIndex(8);
}

void MainWindow::link_taskCheckBoxes_confStackedWidget()
{
    QObject* obj = sender();
    if (obj == ui->applyHDUreformatCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(0);
    else if (obj == ui->applyProcessbiasCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(0);
    else if (obj == ui->applyProcessdarkCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(0);
    else if (obj == ui->applyProcessflatoffCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(0);
    else if (obj == ui->applyProcessflatCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(0);
    else if (obj == ui->applyProcessscienceCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(0);
    else if (obj == ui->applyChopnodCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(1);
    else if (obj == ui->applyBackgroundCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(1);
    else if (obj == ui->applyCollapseCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(2);
    else if (obj == ui->applyBinnedpreviewCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(2);
    else if (obj == ui->applyGlobalweightCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(3);
    else if (obj == ui->applyIndividualweightCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(3);
    else if (obj == ui->applySeparateCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(3);
    else if (obj == ui->applyAbsphotindirectCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(4);
    else if (obj == ui->applyStarflatCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(5);
    else if (obj == ui->applyCreatesourcecatCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(5);
    else if (obj == ui->applyAstromphotomCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(6);
    else if (obj == ui->applySkysubCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(7);
    else if (obj == ui->applyCoadditionCheckBox) cdw->ui->confStackedWidget->setCurrentIndex(8);

    // Show the configuration tab (in case e.g. currently the monitor is displayed)
    cdw->raise();
}

void MainWindow::checkPaths()
{
    // Cast sender to LineEdit because we know it's a LineEdit.
    // Then we have access to its content.
    QLineEdit* lineedit = qobject_cast<QLineEdit*>(sender());
    checkPathsLineEdit(lineedit);
}

bool MainWindow::checkPathsLineEdit(QLineEdit *lineEdit)
{
    // Checks whether the entries in this lineEdit exist as directories.
    // The return value is ignored in some cases.
    // Turn the lineEdit to a stringList
    QStringList list = datadir2StringList(lineEdit);
    QString dirname = lineEdit->text();
    // The following does not make much sense if we have multiple entries in a LineEdit.
    // But we catch that with the following two ifs
    QDir dir = QDir(dirname);
    // Process path fields if
    // -- unrelated to the data tree
    // -- the main data dir itself = QFile(
    // -- they are absolute paths
    // -- baddir string is empty
    if (lineEdit == cdw->ui->ARCselectimageLineEdit) {
        paintPathLineEdit(lineEdit, dirname, "file");
        return true;
    }
    if (dir.isAbsolute() || dirname.isEmpty() || lineEdit == ui->setupMainLineEdit) {
        paintPathLineEdit(lineEdit, dirname);
        return true;
    }

    // Process the remaining data paths (BIAS, FLAT etc.), all relative.
    QString baddir_summary = "";
    QString main = ui->setupMainLineEdit->text();
    // Loop over the directories and check whether they are good or bad.
    // If a bad one is found, we paint the background red after the loop, otherwise green.
    for (int i=0; i<list.size(); ++i) {
        // check if path is absolute, if not, prepend main path
        dirname = list.at(i);
        dir = QDir(dirname);
        if (!dir.isAbsolute()) {
            dirname = main + "/" + dirname;
            dir = QDir(dirname);
        }
        if (!dir.exists()) baddir_summary = baddir_summary + ", " + dirname;
    }

    // Paint LineEdit accordingly:
    // paintPathLineEdit() does not change or read the text in a lineEdit;
    // It merely evaluates if the second arg (a QString) exists as a QDir or not, and
    // paints the LineEdit accordingly.
    // For the purpose here we just use either an existing or a non-existing directory.
    if (!baddir_summary.isEmpty()) {
        paintPathLineEdit(lineEdit, baddir_summary);
        return false;
    }
    else {
        paintPathLineEdit(lineEdit, dirname);
        return true;
    }
}

// Can be called from a slot, hence sender() exists, but is also called directly
// in the MainWindow constructor
void MainWindow::repaintDataDirs()
{
    // do not repaint while reading GUI settings (unnecessary load due to self-triggered calls)
    if (doingInitialLaunch && readingSettings) return;
    bool test = false;
    if (sender() == 0 || sender() == ui->setupMainLineEdit) {
        // Check subdirs as well
        for ( auto &it : status.listDataDirs) {
            test = checkPathsLineEdit(it);
            // Simulate a run if maindir exists (to repaint the command lists)
            if (test && sender() !=0 && it == ui->setupMainLineEdit) {
                on_startPushButton_clicked();
            }
        }
    }
    else {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender());
        if (lineEdit != 0) {
            test = checkPathsLineEdit(lineEdit);
            if (test) on_startPushButton_clicked();
        }
        else {
            qDebug() << "QDEBUG: repaintDataDirs(): bad LineEdit!";
        }
    }
}

// invoked when changing the instrument category in the preference dialog
void MainWindow::linkPrefInst_with_MainInst(int index)
{
    instrumentPreference = index;
    fill_setupInstrumentComboBox();
}

// invoked when reading a project requires a change of category in the preference dialog
// (the inst combobox is populated correctly already)
void MainWindow::updatePreferences()
{
    QSettings settings("THELI", "PREFERENCES");
    settings.setValue("setupProjectLineEdit", ui->setupProjectLineEdit->text());
    settings.sync();
    if (settings.status() != QSettings::NoError) {
        qDebug() << "QDEBUG: An error occurred updating the inst category preference:" << settings.status();
    }
}

void MainWindow::updateFontSize(int index)
{
    QFont currentFont = this->font();
    currentFont.setPointSize(index);
    this->setFont(currentFont);
}

void MainWindow::updateFont(QFont font)
{
    this->setFont(font);
}

void MainWindow::resetInstrumentData()
{
    instData.numChips = 1;
    instData.name = "";
    instData.shortName = "";
    instData.nameFullPath = "";
    instData.obslat = 0.;
    instData.obslong = 0.;
    instData.bayer = "";
    instData.type = "OPT";
    instData.pixscale = 1.0; // in arcsec
    instData.gain = 1.0;
    instData.radius = 0.1;   // exposure coverage radius in degrees
    instData.storage = 0;    // MB used for a single image
    instData.storageExposure = 0.; // MB used for the entire (multi-chip) exposure

    instData.overscan_xmin.clear();
    instData.overscan_xmax.clear();
    instData.overscan_ymin.clear();
    instData.overscan_ymax.clear();
    instData.cutx.clear();
    instData.cuty.clear();
    instData.sizex.clear();
    instData.sizey.clear();
    instData.crpix1.clear();
    instData.crpix2.clear();
}

void MainWindow::initInstrumentData(QString instrumentNameFullPath)
{
    resetInstrumentData();

    QFile instDataFile(instrumentNameFullPath);
    instDataFile.setFileName(instrumentNameFullPath);
    instData.nameFullPath = instrumentNameFullPath;

    // read the instrument specific data
    if( !instDataFile.open(QIODevice::ReadOnly)) {
        emit messageAvailable("MainWindow::initInstrumentData(): "+instrumentNameFullPath+" "+instDataFile.errorString(), "error");
        return;
    }

    bool bayerFound = false;
    QTextStream in(&(instDataFile));
    while(!in.atEnd()) {
        QString line = in.readLine().simplified();
        if (line.isEmpty() || line.contains("#")) continue;

        // scalars
        if (line.contains("INSTRUMENT=")) instData.name = line.split("=")[1];
        if (line.contains("INSTSHORT=")) instData.shortName = line.split("=")[1];
        if (line.contains("NCHIPS=")) instData.numChips = line.split("=")[1].toInt();
        if (line.contains("TYPE=")) instData.type = line.split("=")[1];
        if (line.contains("BAYER=")) {
            // BAYER is not mandatory; if not found, we must set it to blank
            instData.bayer = line.split("=")[1];
            bayerFound = true;
        }
        if (line.contains("OBSLAT=")) instData.obslat = line.split("=")[1].toFloat();
        if (line.contains("OBSLONG=")) instData.obslong = line.split("=")[1].toFloat();
        if (line.contains("PIXSCALE=")) instData.pixscale = line.split("=")[1].toFloat();
        if (line.contains("GAIN=")) instData.gain = line.split("=")[1].toFloat();

        // vectors
        if (line.contains("OVSCANX1=")
                || line.contains("OVSCANX2=")
                || line.contains("OVSCANY1=")
                || line.contains("OVSCANY2=")
                || line.contains("CUTX=")
                || line.contains("CUTY=")
                || line.contains("SIZEX=")
                || line.contains("SIZEY=")
                || line.contains("REFPIXX=")
                || line.contains("REFPIXY=")) {
            line = line.replace('=',' ').replace(')',' ').replace(')',"");
            line = line.simplified();
            QStringList values = line.split(" ");
            QVector<int> vecData;
            // NOTE: already subtracting -1 to make it conform with C++ indexing
            // (apart from SIZEX/Y, which is the actual number of pixels per axis and not a coordinate)
            for (int i=2; i<values.length(); i=i+2) {
                if (line.contains("SIZE")) vecData.push_back(values.at(i).toInt());
                else vecData.push_back(values.at(i).toInt() - 1);
                //     vecData.push_back(values.at(i).toInt() - 1);
            }
            if (line.contains("OVSCANX1")) instData.overscan_xmin = vecData;
            if (line.contains("OVSCANX2")) instData.overscan_xmax = vecData;
            if (line.contains("OVSCANY1")) instData.overscan_ymin = vecData;
            if (line.contains("OVSCANY2")) instData.overscan_ymax = vecData;
            if (line.contains("CUTX")) instData.cutx = vecData;
            if (line.contains("CUTY")) instData.cuty = vecData;
            if (line.contains("SIZEX")) instData.sizex = vecData;
            if (line.contains("SIZEY")) instData.sizey = vecData;
            if (line.contains("REFPIXX")) instData.crpix1 = vecData;
            if (line.contains("REFPIXY")) instData.crpix2 = vecData;
        }
    }

    if (!bayerFound) instData.bayer = "";
    // Backwards compatibility:
    if (instData.type.isEmpty()) instData.type = "OPT";
    QString shortstring = instData.name.split('@').at(0);
    if (instData.shortName.isEmpty()) instData.shortName = shortstring;

    instDataFile.close();

    // The overscan needs special treatment:
    // if it is consistently -1, or the string wasn't found,
    // then the vector must be empty
    testOverscan(instData.overscan_xmin);
    testOverscan(instData.overscan_xmax);
    testOverscan(instData.overscan_ymin);
    testOverscan(instData.overscan_ymax);

    // Determine the radius covered by a single exposure
    int nGlobal;
    int mGlobal;
    int xminOffset;
    int yminOffset;
    int binFactor = 1;
    QVector< QVector<int>> Tmatrices;
    for (int chip=0; chip<instData.numChips; ++chip) {
        QVector<int> tm = {1,0,0,1};
        Tmatrices << tm;
    }
    getBinnedSize(&instData, Tmatrices, nGlobal, mGlobal, binFactor, xminOffset, yminOffset);    // used to retrieve nglobal and mglobal
    instData.radius = 0.5 * sqrt(nGlobal*nGlobal + mGlobal*mGlobal)*instData.pixscale / 3600.;   // in degrees

    // Lastly, how many MB does a single detector occupy
    instData.storage = instData.sizex[0]*instData.sizey[0]*sizeof(float) / 1024. / 1024.;
    instData.storageExposure = instData.storage * instData.numChips;
}

void MainWindow::testOverscan(QVector<int> &overscan)
{
    if (overscan.isEmpty()) return;

    // if the overscan is consistently -1, then the vector must be empty
    bool flag = true;
    for (auto &it : overscan) {
        if (it != -1) {
            flag = false;
            break;
        }
    }
    if (flag) overscan.clear();
}

void MainWindow::fill_setupInstrumentComboBox()
{
    // UNCOMMENT to see how the GUI is initialized
    // qDebug() << "fill_setupInstrumentComboBox";
    QStringList instDirList;
    instDirList << thelidir+"/src/config/instruments/"
                << QDir::homePath()+"/.theli/instruments_user/";

    // Loop over pre-defined and user-defined instruments
    instrument_model = new MyStringListModel;
    QStringList allInstrumentList;

    for (auto &instDirName : instDirList) {
        QDir instDir = QDir(instDirName);
        QStringList filter("*.ini");
        instDir.setNameFilters(filter);
        instDir.setSorting(QDir::Name);
        QStringList currentInstrumentList = instDir.entryList();
        if (currentInstrumentList.isEmpty()) continue;
        currentInstrumentList.replaceInStrings(".ini","");
        allInstrumentList.append(currentInstrumentList);
        // instrument_model only used here, but it needs to exist throughpout the lifetime of the GUI.
        if (instDirName.contains("instruments_user")) instrument_model->instrument_userDir = instDirName;
        else instrument_model->instrument_dir = instDirName;
    }

    if (allInstrumentList.isEmpty()) {
        QMessageBox::critical(this, tr("THELI"),
                              tr("No instrument configurations were found! Did you set the THELIDIR environment variable correctly?\n")
                              +tr("THELIDIR points to the installation directory (where you find the src/ sub-directory)."),
                              QMessageBox::Ok);
        return;
    }

    //    allInstrumentList.sort();
    instrument_model->setStringList(allInstrumentList);

    // CAREFUL! the following line also triggers
    // on_setupInstrumentComboBox_clicked()
    // and repaintDataDirs() (why??)
    ui->setupInstrumentComboBox->setModel(instrument_model);
    ui->setupInstrumentComboBox->setCurrentIndex(0);

    // We always do a software click as well. Unnecessary?
    // on_setupInstrumentComboBox_clicked();

    // Override user's desktop preferences, to avoid a ComboBox to fill the entire screen
    // from top to bottom. Only show 20 items max (designer setting)
    ui->setupInstrumentComboBox->setStyleSheet("combobox-popup: 0;");
}

void MainWindow::updateInstrumentComboBoxBackgroundColor()
{
    QPalette palette;
    if (instrument_type == "OPT") palette.setColor(QPalette::Button,QColor("#40acff"));
    else if (instrument_type == "NIR") palette.setColor(QPalette::Button,QColor("#70ffd4"));
    else if (instrument_type == "NIRMIR") palette.setColor(QPalette::Button,QColor("#ffe167"));
    else if (instrument_type == "MIR") palette.setColor(QPalette::Button,QColor("#ff616b"));
    else palette.setColor(QPalette::Button,QColor("#40acff"));  // backwards compatibility; if undefined, use optical color
    ui->setupInstrumentComboBox->setPalette(palette);
}

void MainWindow::toggleButtonsWhileRunning()
{
    if (!ui->startPushButton->isEnabled()) {
        ui->setupInstrumentComboBox->setDisabled(true);
        ui->setupProjectLineEdit->setDisabled(true);
        ui->setupProjectLoadToolButton->setDisabled(true);
        ui->setupProjectResetToolButton->setDisabled(true);
        ui->setupProjectResetDataToolButton->setDisabled(true);
        cdw->ui->ARCgetcatalogPushButton->setDisabled(true);
        cdw->ui->restoreHeaderPushButton->setDisabled(true);
    }
    else {
        ui->setupInstrumentComboBox->setEnabled(true);
        ui->setupProjectLineEdit->setEnabled(true);
        ui->setupProjectLoadToolButton->setEnabled(true);
        ui->setupProjectResetToolButton->setEnabled(true);
        ui->setupProjectResetDataToolButton->setEnabled(true);
        cdw->ui->ARCgetcatalogPushButton->setEnabled(true);
        cdw->ui->restoreHeaderPushButton->setEnabled(true);
    }
}

QString MainWindow::getInstDir(QString instname)
{
    instrument_name = ui->setupInstrumentComboBox->currentText();
    QFile file;
    file.setFileName(instrument_dir+instname);
    if (file.exists()) return instrument_dir;
    else {
        file.setFileName(instrument_userDir+instname);
        if (file.exists()) return instrument_userDir;
        else {
            qDebug() << "does not exist" << instrument_userDir+instname;
            qDebug() << "ERROR: MainWindow::getInstDir(): Could not identify instrument directory!";
        }
    }
    return "";
}

void MainWindow::checkMemoryConstraints()
{
    // systemRAM is in kB;
    if (instData.storageExposure >= systemRAM / 1024.) {
        QMessageBox::warning( this, "Low physical RAM detected",
                              "A single "+instData.name + " exposure occupies "
                              + QString::number(long(instData.storageExposure)) + " MB in memory.\n"
                              + "Your machine has only "+QString::number(long(systemRAM/1024)) + " MB installed. "
                              + "Processing might be fine, but performance will be significantly reduced.");
    }
}

void MainWindow::on_setupInstrumentComboBox_clicked()
{
    // Read some instrument parameters and the file path
    instrument_name = ui->setupInstrumentComboBox->currentText();
    if (instrument_name.isEmpty()) return; // can happen if e.g. there are no user-defined instruments
    QString matchingInstDir = getInstDir(instrument_name+".ini");
    instrument_file.setFileName(matchingInstDir+instrument_name+".ini");
    instrument_bayer = get_fileparameter(&instrument_file, "BAYER");
    instrument_type = get_fileparameter(&instrument_file, "TYPE");
    instrument_nchips = get_fileparameter(&instrument_file, "NCHIPS").toInt();

    // Pass on to ConfDockWidget
    cdw->instrument_dir = matchingInstDir;
    cdw->instrument_name = instrument_name;
    cdw->instrument_type = instrument_type;
    cdw->instrument_bayer = instrument_bayer;   // UNUSED?
    cdw->instrument_nchips = instrument_nchips;
    cdw->instrument_file.setFileName(matchingInstDir+instrument_name+".ini");

    // The structure holding the content of the instrument file
    initInstrumentData(matchingInstDir+instrument_name+".ini");

    updateInstrumentComboBoxBackgroundColor();

    if (!readingSettings) {
        // TODO: rather, look into the image headers and calculate the most likely pixel scale (dropping into 1x1 or 2x2 binning mode)
        cdw->ui->COApixscaleLineEdit->setText(get_fileparameter(&instrument_file, "PIXSCALE"));
    }

    if (instrument_type == "MIR") {
        // UNCOMMENT to see how the GUI is initialized
        // qDebug() << "U0 - MIR setup";
        ui->applyChopnodCheckBox->show();
        ui->chopnodConfigureToolButton->show();
        ui->applyBackgroundCheckBox->hide();
        ui->BACconfigureToolButton->hide();
        cdw->ui->overscanCheckBox->setDisabled(true);
    }
    else if (instrument_type == "NIR") {
        // UNCOMMENT to see how the GUI is initialized
        // qDebug() << "U0 - NIR setup";
        ui->applyChopnodCheckBox->hide();
        ui->chopnodConfigureToolButton->hide();
        ui->applyBackgroundCheckBox->show();
        ui->BACconfigureToolButton->show();
        cdw->ui->overscanCheckBox->setDisabled(true);
    }
    else if (instrument_type == "NIRMIR") {
        // UNCOMMENT to see how the GUI is initialized
        // qDebug() << "U0 - NIRMIR setup";
        ui->applyChopnodCheckBox->show();
        ui->chopnodConfigureToolButton->show();
        ui->applyBackgroundCheckBox->show();
        ui->BACconfigureToolButton->show();
        cdw->ui->overscanCheckBox->setDisabled(true);
    }
    else if (instrument_type == "OPT" || instrument_type == "") {
        // UNCOMMENT to see how the GUI is initialized
        // qDebug() << "U0 - OPT setup";
        ui->applyChopnodCheckBox->hide();
        ui->chopnodConfigureToolButton->hide();
        ui->applyBackgroundCheckBox->show();
        ui->BACconfigureToolButton->show();
        cdw->ui->overscanCheckBox->setEnabled(true);
    }

    // Update settings
    writeGUISettings();

    checkMemoryConstraints();
}

void MainWindow::scienceDataDirUpdatedReceived(QString allDirs)
{
    ui->setupScienceLineEdit->setText(allDirs);
    emit rereadScienceDataDir();
}

void MainWindow::progressUpdateReceived(float progressReceived)
{
    controller->progress = progressReceived;
    ui->processProgressBar->setValue(int(progressReceived));
}


bool MainWindow::sufficientSpaceAvailable(long spaceNeeded)
{
    QString unitsNeeded = " MB";
    QString unitsFree = " MB";
    long spaceFree = remainingDataDriveSpace(ui->setupMainLineEdit->text());  // in MBytes

    QString spaceNeededString = QString::number(spaceNeeded) + unitsNeeded;
    QString spaceFreeString = QString::number(spaceFree) + unitsFree;

    // Unit conversions
    if (spaceNeeded > 1024.) {
        float spaceNeededFloat = spaceNeeded / 1024.;
        unitsNeeded = " GB";
        spaceNeededString = QString::number(spaceNeededFloat, 'f', 1) + unitsNeeded;
    }

    if (spaceFree > 1024.) {
        float spaceFreeFloat = spaceFree / 1024.;
        unitsFree = " GB";
        spaceFreeString = QString::number(spaceFreeFloat, 'f', 1) + unitsFree;
    }

    if (spaceNeeded > spaceFree) {
        QMessageBox::warning( this, tr("Insufficient space on drive"),
                              tr("The current operation needs to write ") + spaceNeededString +
                              tr(" to drive, however only") + spaceFreeString +
                              tr(" are available on ")+ui->setupMainLineEdit->text());
        return false;
    }

    return true;
}


void MainWindow::on_setupProjectLoadToolButton_clicked()
{
    // First of all, check if we have unsaved images in memory
    long numUnsavedImages = controller->checkForUnsavedImages();
    if (numUnsavedImages > 0) {
        long mBytes = numUnsavedImages*(instData.sizex[0]*instData.sizey[0]*4./1024/1024);

        QMessageBox msgBox;
        msgBox.setModal(true);
        msgBox.setInformativeText(tr("A new project is about to be loaded.\n") +
                                  tr("The current project keeps ") +
                                  QString::number(numUnsavedImages) +
                                  tr(" processed images in memory that have not yet been saved to drive.\n\n"));
        QAbstractButton *pButtonSave = msgBox.addButton(tr("Save images (")+QString::number(mBytes)+" MB)", QMessageBox::YesRole);
        QAbstractButton *pButtonContinue = msgBox.addButton(tr("Continue without saving"), QMessageBox::YesRole);
        QAbstractButton *pButtonCancel = msgBox.addButton(tr("Cancel loading new project"), QMessageBox::YesRole);
        msgBox.exec();
        if (msgBox.clickedButton() == pButtonSave) {
            if (sufficientSpaceAvailable(mBytes)) {
                controller->writeUnsavedImagesToDrive();
            }
            else {
                // sufficientSpaceAvailable() displays a warning message
                return;
            }
        }
        else if (msgBox.clickedButton() == pButtonContinue) {
            // nothing yet
        }
        else return;
    }

    QString projectname;
    if (kernelType == "linux") {
        projectname = QFileDialog::getOpenFileName(this, tr("Select THELI Project"), QDir::homePath()+"/.config/THELI/",
                                                   tr("THELI projects (*.conf)"));
    }
    else {
        // kernelType == "darwin"
        projectname = QFileDialog::getOpenFileName(this, tr("Select THELI Project"), QDir::homePath()+"/Library/Preferences/THELI/",
                                                   tr("THELI projects (*.plist)"));
    }

    // Truncate the suffix (OS dependent; Linux: .conf; MacOS Mojave: com.theli.<project>.plist)
    QFileInfo projectFileInfo(projectname);
    QString projectBaseName = projectFileInfo.completeBaseName();
    if (kernelType == "darwin") projectBaseName.remove("com.theli.");
    if ( projectBaseName.isEmpty() ) {
        return;
    }

    // Before wiping the data tree, we must switch away from the memory view (if currently raised).
    // Even though we display an empty model, it still tries to access the old ones, even though everything should be protected.
    // So, we just switch to the config view and then back (until I sorted this out)
    bool viewSwitched = false;
    if (memoryViewer->isVisible()) {
        cdw->raise();
        viewSwitched = true;
    }
    controller->wipeDataTree();

    // Now load the new GUI settings
    ui->setupProjectLineEdit->setText(projectBaseName);
    int settingsStatus = readGUISettings(projectBaseName);
    if (settingsStatus == 0) {
        // everything is OK
    }
    else if (settingsStatus == 1) {
        QMessageBox::warning( this, "THELI: Access error",
                              "An access error occurred while reading the configuration file for project '"+projectBaseName+"'!\n");
    }
    else if (settingsStatus == 2) {
        QMessageBox::warning( this, "THELI: Format error",
                              "A format error occurred while reading the configuration file for project '"+projectBaseName+"'!\n");
    }

    // Could also directly call controller->newProjectLoadedReceived() instead
    emit newProjectLoaded();

    // Show memory view again (if it was hidden before)
    if (viewSwitched) memoryViewer->raise();
}

void MainWindow::loadPreferences()
{
    // send instrument name and project name (why?)
    updatePreferences();
    // would have to set combobox, because the dialog exists already
    bool taskRunning;
    if (ui->startPushButton->isEnabled()) taskRunning = false;
    else taskRunning = true;
    preferences = new Preferences(taskRunning, this);
    connect(preferences, &Preferences::instPreferenceChanged, this, &MainWindow::linkPrefInst_with_MainInst);
    connect(preferences, &Preferences::fontSizeChanged, this, &MainWindow::updateFontSize);
    connect(preferences, &Preferences::fontChanged, this, &MainWindow::updateFont);
    connect(preferences, &Preferences::diskspacewarnChanged, this, &MainWindow::updateDiskspaceWarning);
    connect(preferences, &Preferences::serverChanged, this, &MainWindow::updateServer);
    connect(preferences, &Preferences::numcpuChanged, this, &MainWindow::updateNumcpu);
    connect(preferences, &Preferences::memoryUsageChanged, controller, &Controller::updateMemoryPreference);
    connect(preferences, &Preferences::switchProcessMonitorChanged, this, &MainWindow::updateSwitchProcessMonitorPreference);
    connect(preferences, &Preferences::intermediateDataChanged, controller, &Controller::updateIntermediateDataPreference);
    connect(preferences, &Preferences::verbosityLevelChanged, controller, &Controller::updateVerbosity);
    connect(preferences, &Preferences::preferencesUpdated, controller, &Controller::loadPreferences);
    connect(this, &MainWindow::runningStatusChanged, preferences, &Preferences::updateParallelization);
    connect(this, &MainWindow::sendingDefaultFont, preferences, &Preferences::receiveDefaultFont);
    QFont defaultFont = this->font();
    emit sendingDefaultFont(defaultFont);  // to preferences
    preferences->show();
}

void MainWindow::updateSwitchProcessMonitorPreference(bool switchToMonitor)
{
    switchProcessMonitorPreference = switchToMonitor;
}

void MainWindow::statusChangedReceived(QString newStatus)
{
    status.statusstringToHistory(newStatus);
}

void MainWindow::load_dialog_abszeropoint()
{
    AbsZeroPoint *abszeropoint = new AbsZeroPoint("", &instData, this);
    // crashes, probably because 'preferences' doesn't exist yet.
    //   connect(preferences, &Preferences::verbosityLevelChanged, abszeropoint, &AbsZeroPoint::updateVerbosity);
    abszeropoint->show();
}

void MainWindow::load_dialog_imageStatistics()
{
    QString mainDir = ui->setupMainLineEdit->text();
    Data *scienceData = nullptr;

    if (controller->DT_SCIENCE.length() == 1) {
        scienceData = controller->DT_SCIENCE.at(0);
    }
    else {
        QMessageBox msgBox;
        msgBox.setInformativeText(tr("Image statistics: Choose SCIENCE data\n\n") +
                                  tr("The current SCIENCE data tree contains several entries. ") +
                                  tr("Select one for the statistics module:\n\n"));
        for (auto &data : controller->DT_SCIENCE) {
            QAbstractButton *button = msgBox.addButton(data->subDirName, QMessageBox::YesRole);
        }
        QAbstractButton *pCancel = msgBox.addButton(tr("Cancel"), QMessageBox::NoRole);
        msgBox.exec();
        QString choice = msgBox.clickedButton()->text();
        if (msgBox.clickedButton()== pCancel) return;

        for (auto &data : controller->DT_SCIENCE) {
            if (data->subDirName == choice) scienceData = data;
            break;
        }
    }

    scienceData->populateExposureList();
    ImageStatistics *imagestatistics = new ImageStatistics(scienceData->exposureList, mainDir, scienceData->subDirName, &instData, this);
    imagestatistics->show();
}

void MainWindow::load_dialog_colorpicture()
{
    ColorPicture *colorPicture = new ColorPicture(&instData,  ui->setupMainLineEdit->text(), this);
    connect(colorPicture, &ColorPicture::showMessageBox, this, &MainWindow::showMessageBoxReceived);
    colorPicture->show();
}

void MainWindow::load_dialog_newinst()
{
    instrument->show();
}

QString MainWindow::getStatusForSettings()
{
    QString statusString = "";
    for (auto &it : status.listHistory) {
        if (it) statusString.append("1");
        else statusString.append("0");
    }
    return statusString;
}

void MainWindow::setStatusFromSettings(QString statusString)
{
    // Enforce that the status string is no longer than the maximum string
    // (this is in case I change the number of task checkboxes at some point)
    statusString.truncate(19);
    QStringList list = statusString.split("");
    // The call above results in empty strings at the first and last place of the list.
    // We need to remove them:
    list.removeFirst();
    list.removeLast();
    int i = 0;
    for (auto &it : list) {
        if (it == "0") status.listHistory.operator[](i) = false;
        else status.listHistory.operator[](i) = true;
        ++i;
    }
    status.updateStatus();
}

void MainWindow::loadIView()
{
    QString main = ui->setupMainLineEdit->text();
    QString science = ui->setupScienceLineEdit->text().split(" ").at(0);
    QString dirname = main+"/"+science;
    if (!QDir(dirname).exists()) dirname = QDir::homePath();

    IView *iView = new IView("FITSmonochrome", dirname, "*.fits", this);
    iView->show();
}

// Used for displaying:
// (*) binned mosaics
void MainWindow::launchViewer(QString dirname, QString filter, QString mode)
{
    // Load the FITS viewer
    IView *iView = new IView("FITSmonochrome", dirname, filter, this);
    connect(iView, &IView::closed, iView, &IView::deleteLater);
    iView->show();
    iView->setMiddleMouseMode(mode);
}

/*
void MainWindow::launchCoaddFluxcal(QString coaddImage)
{
    AbsZeroPoint *abszeropoint = new AbsZeroPoint(coaddImage, &instData, this);
    abszeropoint->show();
}
*/

void MainWindow::reset()
{
    // First of all, check if we have unsaved images in memory
    long numUnsavedImages = controller->checkForUnsavedImages();
    if (numUnsavedImages > 0) {
        long mBytes = numUnsavedImages*(instData.sizex[0]*instData.sizey[0]*4./1024/1024);

        QMessageBox msgBox;
        msgBox.setModal(true);
        msgBox.setInformativeText(tr("You are about to reset the current project, which keeps \n") +
                                  QString::number(numUnsavedImages) +
                                  tr(" processed images in memory that have not yet been saved to drive.\n\n"));
        QAbstractButton *pButtonSave = msgBox.addButton(tr("Save images (")+QString::number(mBytes)+" MB)", QMessageBox::YesRole);
        QAbstractButton *pButtonContinue = msgBox.addButton(tr("Continue without saving"), QMessageBox::YesRole);
        QAbstractButton *pButtonCancel = msgBox.addButton(tr("Cancel"), QMessageBox::YesRole);
        msgBox.exec();
        if (msgBox.clickedButton() == pButtonSave) {
            if (sufficientSpaceAvailable(mBytes)) {
                controller->writeUnsavedImagesToDrive();
            }
            else {
                // sufficientSpaceAvailable() displays a warning message
                return;
            }
        }
        else if (msgBox.clickedButton() == pButtonContinue) {
            // nothing yet
        }
        else return;
    }

    controller->wipeDataTree();

    cdw->loadDefaults();

    for (auto &it : status.listHistory) {
        it = false;
    }
    status.updateStatus();
    writeGUISettings();
}

void MainWindow::populateTaskCommentMap()
{
    taskCommentMap.clear();
    taskCommentMap.insert("HDUreformat", "HDU reformatting");
    taskCommentMap.insert("Processbias", "Processing biases in");
    taskCommentMap.insert("Processdark", "Processing darks in");
    taskCommentMap.insert("Processflatoff", "Processing flat off/darks in");
    taskCommentMap.insert("Processflat", "Processing flats in");
    taskCommentMap.insert("Processscience", "Debiasing and flatfielding data in");
    taskCommentMap.insert("Chopnod", "Doing chopnod background correction for");
    taskCommentMap.insert("Background", "Applying background model to");
    taskCommentMap.insert("Collapse", "Applying collapse correction to");
    taskCommentMap.insert("Binnedpreview", "Creating binned previews for");
    taskCommentMap.insert("Globalweight", "Creating global weights for");
    taskCommentMap.insert("Individualweight", "Creating individual weights for");
    taskCommentMap.insert("Separate", "Separating different targes in");
    taskCommentMap.insert("Absphotindirect", "Performing indirect absolute photometry");
    taskCommentMap.insert("Createsourcecat", "Creating source catalogs for");
    taskCommentMap.insert("Astromphotom", "Calculating astrometric solution for");
    taskCommentMap.insert("Skysub", "Subtracting the sky for");
    taskCommentMap.insert("Coaddition", "Performing coaddition for");
    taskCommentMap.insert("GetCatalogFromWEB", "Downloading astrometric reference catalog for");
    taskCommentMap.insert("GetCatalogFromIMAGE", "Extracting astrometric reference catalog for");
    taskCommentMap.insert("Resolvetarget", "Resolving target coordinates for");
}

void MainWindow::initProcessingStatus()
{
    // Initialize the status' collective widget lists
    // the order is important!!!
    status.listCheckBox.append(ui->applyHDUreformatCheckBox);
    status.listCheckBox.append(ui->applyProcessbiasCheckBox);
    status.listCheckBox.append(ui->applyProcessdarkCheckBox);
    status.listCheckBox.append(ui->applyProcessflatoffCheckBox);
    status.listCheckBox.append(ui->applyProcessflatCheckBox);
    status.listCheckBox.append(ui->applyProcessscienceCheckBox);
    status.listCheckBox.append(ui->applyChopnodCheckBox);
    status.listCheckBox.append(ui->applyBackgroundCheckBox);
    status.listCheckBox.append(ui->applyCollapseCheckBox);
    status.listCheckBox.append(ui->applyBinnedpreviewCheckBox);
    status.listCheckBox.append(ui->applyGlobalweightCheckBox);
    status.listCheckBox.append(ui->applyIndividualweightCheckBox);
    status.listCheckBox.append(ui->applySeparateCheckBox);
    status.listCheckBox.append(ui->applyAbsphotindirectCheckBox);
    status.listCheckBox.append(ui->applyCreatesourcecatCheckBox);
    status.listCheckBox.append(ui->applyAstromphotomCheckBox);
    status.listCheckBox.append(ui->applyStarflatCheckBox);
    status.listCheckBox.append(ui->applySkysubCheckBox);
    status.listCheckBox.append(ui->applyCoadditionCheckBox);

    status.listToolButtons.append(ui->HDUreformatConfigureToolButton);
    status.listToolButtons.append(ui->calibratorConfigureToolButton);
    status.listToolButtons.append(ui->BACconfigureToolButton);
    status.listToolButtons.append(ui->chopnodConfigureToolButton);
    status.listToolButtons.append(ui->COCconfigureToolButton);
    status.listToolButtons.append(ui->binnedPreviewConfigureToolButton);
    status.listToolButtons.append(ui->globalweightConfigureToolButton);
    status.listToolButtons.append(ui->individualweightConfigureToolButton);
    status.listToolButtons.append(ui->separateConfigureToolButton);
    status.listToolButtons.append(ui->absphotomindirectConfigureToolButton);
    status.listToolButtons.append(ui->absphotomdirectConfigureToolButton);
    status.listToolButtons.append(ui->astromphotomConfigureToolButton);
    status.listToolButtons.append(ui->createsourcecatConfigureToolButton);
    status.listToolButtons.append(ui->skysubConfigureToolButton);
    status.listToolButtons.append(ui->coadditionConfigureToolButton);

    status.listDataDirs.append(ui->setupMainLineEdit);
    status.listDataDirs.append(ui->setupBiasLineEdit);
    status.listDataDirs.append(ui->setupDarkLineEdit);
    status.listDataDirs.append(ui->setupFlatoffLineEdit);
    status.listDataDirs.append(ui->setupFlatLineEdit);
    status.listDataDirs.append(ui->setupScienceLineEdit);
    status.listDataDirs.append(ui->setupSkyLineEdit);
    status.listDataDirs.append(ui->setupStandardLineEdit);

    // Populate the map that associates CheckBoxes and their texts
    // (so that we can lookup the CheckBox based on its string representation,
    // needed for some messages being sent to plainTextEdit when executing tasks.
    // Also, populate the index map;
    QString taskBasename;
    int i=0;
    for (auto &it: status.listCheckBox) {
        taskBasename = it->objectName().remove("apply").remove("CheckBox");
        checkboxMap.insert(it, taskBasename);
        status.indexMap.insert(taskBasename, i);
        ++i;
    }

    // List name contains the taskBasename;
    // The Boolean list goes to false,
    // the value list to empy strings
    QString name;
    for (int i=0; i<status.numtasks; ++i) {
        name = status.listCheckBox.at(i)->objectName();
        name.remove("apply");
        name.remove("CheckBox");
        status.listName << name;
        status.listHistory << false;
        status.listCurrentValue << " ";
        status.listFixedValue << " ";        // just to create a QList with 'numtasks' elements
    }
    for (int i=0; i<status.numtasks; ++i) {
        // HARDCODING!!! This must be updated if the order of tasks
        // is changed, or if new task checkboxes are added to the GUI
        if (i==0) status.listFixedValue.operator[](i) = "P";
        else if (i==5) status.listFixedValue.operator[](i) = "A";
        else if (i==6) status.listFixedValue.operator[](i) = "M";
        else if (i==7) status.listFixedValue.operator[](i) = "B";
        else if (i==8) status.listFixedValue.operator[](i) = "C";
        else if (i==16) status.listFixedValue.operator[](i) = "D";
        else if (i==17) status.listFixedValue.operator[](i) = "S";
    }
}

void MainWindow::on_setupReadmePushButton_clicked()
{
    multidirReadme = new MultidirReadme(this);
    multidirReadme->show();
}

void MainWindow::on_setupInstrumentComboBox_currentTextChanged(const QString &arg1)
{
    // UNCOMMENT to see how the GUI is built up
    // qDebug() << "on_setupInstrumentComboBox_currentTextChanged()";
    on_setupInstrumentComboBox_clicked();
}

void MainWindow::on_processingTabWidget_currentChanged(int index)
{
    if (index == 0 || index == 1) {
        cdw->ui->confStackedWidget->setCurrentIndex(0);
        cdw->ui->confBackwardPushButton->setDisabled(true);
    }
    //    else if (index == 2) {
    //        cdw->ui->confStackedWidget->setCurrentIndex(1);
    //        cdw->ui->confBackwardPushButton->setEnabled(true);
    //    }
    else {
        cdw->ui->confStackedWidget->setCurrentIndex(3);
        cdw->ui->confBackwardPushButton->setEnabled(true);
        // Fill the coadd filter combobox with values
    }
    writeGUISettings();
    update();
}

void MainWindow::shutDown()
{
    this->close();
}


void MainWindow::on_actionAdd_new_instrument_configuration_triggered()
{
    load_dialog_newinst();
}

void MainWindow::on_actionEdit_preferences_triggered()
{
    loadPreferences();
}

void MainWindow::restoreOriginalData()
{
    QMessageBox msgBox;
    msgBox.setInformativeText(tr("Restore raw data?\n\n") +
                              tr("This button restores all raw data from their RAWDATA/ directories.\n") +
                              tr("All previously processed data will be lost (full data reset).\n\n"));
    QAbstractButton *okButton = msgBox.addButton("OK", QMessageBox::YesRole);
    QAbstractButton *cancelButton = msgBox.addButton(tr("Cancel"), QMessageBox::NoRole);
    msgBox.exec();
    if (msgBox.clickedButton()== cancelButton) return;

    // Restore data
    controller->restoreAllRawData();

    QString newStatus = "";
    status.statusstringToHistory(newStatus);
    // Reset status
    for (auto &it : status.listHistory) {
        it = false;
    }
    status.updateStatus();
}

void MainWindow::on_actionLicense_triggered()
{
    license = new License(this);
    license->show();
}

void MainWindow::on_actionAcknowledging_triggered()
{
    acknowledging = new Acknowledging(this);
    acknowledging->show();
}
