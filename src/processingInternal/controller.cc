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

#include "controller.h"
#include "instrumentdata.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "../dockwidgets/confdockwidget.h"
#include "ui_confdockwidget.h"
#include "ui_memoryviewer.h"
#include "../dockwidgets/monitor.h"
#include "../myimage/myimage.h"
#include "../functions.h"
#include "../preferences.h"
#include "../tools/cfitsioerrorcodes.h"

#include <omp.h>
#include <QMainWindow>
#include <QString>
#include <QList>
#include <QSettings>
#include <QTest>
#include <QMutex>
#include <QTimer>
#include <QProgressBar>

#include <unistd.h>

Controller::Controller(const instrumentDataType *instrumentData, QString statusold, ConfDockWidget *confDockWidget,
                       Monitor *processMonitor, MainWindow *parent) :
    QMainWindow(parent),
    cdw(confDockWidget),
    instData(instrumentData)
{
    mainGUI = parent;
    //    cdw = confDockWidget;
    //    instData = instrumentData;
    statusOld = statusold;
    monitor = processMonitor;

    // Load detector info only if an instrument has been selected
    // In some instances, e.g. deleting all manually defined user insts, this command would fail
    // Same for the masks

    if (!instrumentData->nameFullPath.isEmpty()) {
        getDetectorSections();
        mask = new Mask(instData, this);
    }

    // Before populating all Data objects
    omp_init_lock(&lock);
    omp_init_lock(&memoryLock);
    omp_init_lock(&wcsLock);
    omp_init_lock(&genericLock);
    omp_init_lock(&progressLock);
    omp_init_lock(&backgroundLock);

    loadPreferences();

    // Populate data tree
    mapDataTree();

    // Tell DT_x about the memory preference
    updateMemoryPreference(minimizeMemoryUsage);

    // Initialization
    initEnvironment(thelidir, userdir);
    instrument_dir = mainGUI->instrument_dir;

    //    loadPreferences();

    externalProcess = new QProcess(this);
    //    connect(externalProcess, &QProcess::readyReadStandardOutput, this, &Controller::processExternalStdout);
    //    connect(externalProcess, &QProcess::readyReadStandardError, this, &Controller::processExternalStderr);
    stdoutByteArray = new QByteArray();
    stderrByteArray = new QByteArray();
    stdoutByteArray->reserve(10000);
    stderrByteArray->reserve(10000);

    //    setupExternalProcesses();

    populateCommentMap();

    // Done at top level, so it is not repeated many times by the Splitter class
    populateHeaderDictionary();
    populateFilterDictionary();
    populateInstrumentDictionary();

    // Daisy-chaining scamp, and the coadditions
    connect(this, &Controller::showScampCheckPlots, this, &Controller::showScampCheckPlotsReceived);
    connect(this, &Controller::swarpStartResampling, this, &Controller::coaddResample);
    connect(this, &Controller::swarpStartSwarpfilter, this, &Controller::coaddSwarpfilter);
    connect(this, &Controller::swarpStartCoaddition, this, &Controller::coaddCoaddition);
    connect(this, &Controller::swarpStartUpdate, this, &Controller::coaddUpdate);

    progressTimer = new QTimer(this);
    connect(progressTimer, SIGNAL(timeout()), SLOT(displayProgress()));
    progressTimer->start(100);
}

void Controller::getNumberOfActiveImages(Data *&data)
{
    numActiveImages = 0;
    progress = 0.;
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        for (auto &it : data->myImageList.at(chip)) {
            if (it->activeState == MyImage::ACTIVE) ++numActiveImages;
        }
    }

    progressStepSize = 100./(float(numActiveImages));
    progressHalfStepSize = 0.5*progressStepSize;

    // Last 5% are reserved for mode determination and writing to drive
    // 45% are in Data::combineImagesCalib() for combination
    progressCombinedStepSize = 5./instData->numUsedChips;
}

Controller::~Controller()
{
    for (auto &DT_x : masterListDT) {
        for (auto &it : DT_x) {
            delete it;
            it = nullptr;
        }
    }

    delete GLOBALWEIGHTS;
    GLOBALWEIGHTS = nullptr;

    //    mask->reset();
    delete mask;
    mask = nullptr;
    omp_destroy_lock(&lock);
    omp_destroy_lock(&memoryLock);
    omp_destroy_lock(&genericLock);
    omp_destroy_lock(&progressLock);
    omp_destroy_lock(&backgroundLock);

    for (auto &it : externalProcesses) {
        delete it;
        it = nullptr;
    }

    //    delete externalProcess;
    delete stdoutByteArray;
    delete stderrByteArray;
    stdoutByteArray = nullptr;
    stderrByteArray = nullptr;
}

// Reads whatever is defined in the Setup data tree section, and maps it onto the various lists
void Controller::mapDataTree()
{
    if (QDir(mainDirName) == QDir::home()) {
        emit messageAvailable("For safety reasons, your home directory is not permitted as the main directory.", "error");
        criticalReceived();
        return;
    }

    dataTreeUpdateOngoing = true;
    omp_set_lock(&memoryLock);
    emit clearMemoryView();
    mainDirName = mainGUI->ui->setupMainLineEdit->text();
    recurseCounter = 0;
    parseDataDir(mainGUI->ui->setupBiasLineEdit, DT_BIAS);
    recurseCounter = 0;
    parseDataDir(mainGUI->ui->setupDarkLineEdit, DT_DARK);
    recurseCounter = 0;
    parseDataDir(mainGUI->ui->setupFlatoffLineEdit, DT_FLATOFF);
    recurseCounter = 0;
    parseDataDir(mainGUI->ui->setupFlatLineEdit, DT_FLAT);
    recurseCounter = 0;
    parseDataDir(mainGUI->ui->setupScienceLineEdit, DT_SCIENCE);
    recurseCounter = 0;
    parseDataDir(mainGUI->ui->setupSkyLineEdit, DT_SKY);
    recurseCounter = 0;
    parseDataDir(mainGUI->ui->setupStandardLineEdit, DT_STANDARD);
    recurseCounter = 0;

    updateMasterList();

    if (QDir(mainDirName+"/GLOBALWEIGHTS/").exists()) {
        GLOBALWEIGHTS = new Data(instData, mask, mainDirName, "GLOBALWEIGHTS", &verbosity);
        GLOBALWEIGHTS->maxExternalThreads = maxExternalThreads;
        GLOBALWEIGHTS->progress = &progress;
        GLOBALWEIGHTS->dataType = "GLOBALWEIGHT";
        connect(GLOBALWEIGHTS, &Data::statusChanged, mainGUI, &MainWindow::statusChangedReceived);
        connect(GLOBALWEIGHTS, &Data::messageAvailable, this, &Controller::messageAvailableReceived);
        connect(GLOBALWEIGHTS, &Data::appendOK, this, &Controller::appendOKReceived);
        connect(GLOBALWEIGHTS, &Data::critical, this, &Controller::criticalReceived);
        connect(GLOBALWEIGHTS, &Data::warning, this, &Controller::warningReceived);
        connect(GLOBALWEIGHTS, &Data::showMessageBox, this, &Controller::showMessageBoxReceived);
        connect(GLOBALWEIGHTS, &Data::setMemoryLock, this, &Controller::setMemoryLockReceived, Qt::DirectConnection);
        connect(GLOBALWEIGHTS, &Data::setWCSLock, this, &Controller::setWCSLockReceived, Qt::DirectConnection);
        //        if (memoryViewer != nullptr) {
        //            connect(GLOBALWEIGHTS, &Data::statusChanged, memoryViewer, &MemoryViewer::updateStatusCheckBoxesReceived);
        //        }
        GLOBALWEIGHTS->setParent(this);
        globalweights_created = true;
    }

    emit populateMemoryView();
    omp_unset_lock(&memoryLock);
    dataTreeUpdateOngoing = false;
}

// Parse the data tree entries
// Construct the Data entities
void Controller::parseDataDir(QLineEdit *le, QList<Data *> &DT_x)
{
    DT_x.clear();

    if (!QDir(mainDirName).exists()
            || mainDirName.isEmpty()
            || le->text().isEmpty()) return;

    bool successFileScan = true;
    QString badDirName = "";

    QStringList dirs = le->text().replace(","," ").simplified().split(" ");
    for (auto &it : dirs) {
        // do not accept any wrong entries; empty the list
        if (!QDir(mainDirName+"/"+it).exists()) {
            DT_x.clear();
            return;
        }
        if (QDir(mainDirName) == QDir::home()) {
            emit messageAvailable("For safety reasons, your home directory is not permitted as the main directory.", "error");
            criticalReceived();
            return;
        }

        Data *data = new Data(instData, mask, mainDirName, it, &verbosity);
        connect(data, &Data::statusChanged, mainGUI, &MainWindow::statusChangedReceived);
        connect(data, &Data::messageAvailable, this, &Controller::messageAvailableReceived);
        connect(data, &Data::appendOK, this, &Controller::appendOKReceived);
        connect(data, &Data::critical, this, &Controller::criticalReceived);
        connect(data, &Data::warning, this, &Controller::warningReceived);
        connect(data, &Data::showMessageBox, this, &Controller::showMessageBoxReceived);
        connect(data, &Data::setMemoryLock, this, &Controller::setMemoryLockReceived, Qt::DirectConnection);
        connect(data, &Data::setWCSLock, this, &Controller::setWCSLockReceived, Qt::DirectConnection);
        connect(data, &Data::addToProgressBar, this, &Controller::addToProgressBarReceived);
        connect(data, &Data::errorOccurredInMyImage, this, &Controller::criticalReceived);
        //        if (memoryViewer != nullptr) {
        //            connect(data, &Data::statusChanged, memoryViewer, &MemoryViewer::updateStatusCheckBoxesReceived);
        //        }
        data->setParent(this);
        data->maxExternalThreads = maxExternalThreads;
        data->progress = &progress;
        if (le == mainGUI->ui->setupBiasLineEdit) data->dataType = "BIAS";
        else if (le == mainGUI->ui->setupDarkLineEdit) data->dataType = "DARK";
        else if (le == mainGUI->ui->setupFlatoffLineEdit) data->dataType = "FLATOFF";
        else if (le == mainGUI->ui->setupFlatLineEdit) data->dataType = "FLAT";
        else if (le == mainGUI->ui->setupScienceLineEdit) data->dataType = "SCIENCE";
        else if (le == mainGUI->ui->setupSkyLineEdit) data->dataType = "SKY";
        else if (le == mainGUI->ui->setupStandardLineEdit) data->dataType = "STANDARD";
        else {
            // nothing yet
        }
        // Check status consistency

        successFileScan = data->checkStatusConsistency();

        if (!successFileScan) {
            badDirName = data->dirName;
            break;
        }

        // Needs to know the dataType, hence here and not further up
        data->broadcastNumberOfFiles();
        DT_x << data;
    }

    if (!successFileScan) {
        emit messageAvailable(badDirName + " : Inconsistency detected between FITS file names and recorded processing status.<br>Inferring status from file names ...", "warning");

        /*
        if (recurseCounter > 1) {
            qDebug() << badDirName << ": The FITS files found do not match the recorded status";
            qDebug() << "Either restore the files manually, or use the memory viewer to set the correct status.";
            qDebug() << "Restart recommended.";

                // TODO: the signal emitted is received by the controller and re-emitted, but not received by MainWindow. very odd. hence the message Available();
                // Edit: even that signal is not received!
                emit messageAvailable("<br>" + badDirName + tr(": The FITS files found do not match the recorded status.\n")+
                                      tr("Either restore the files manually, or use the 'Processing status' menu to reflect the current status.<br> Restart recommended."), "error");
                emit criticalReceived();
                emit showMessageBox("Data::INCONSISTENT_DATA_STATUS", badDirName, "status");
                return;

        }
        */
        // RECURSIVE
        // must repeat everything (simplest implementation), at max once

        if (recurseCounter == 0) {
            ++recurseCounter;   // Must set before entering same function again!
            parseDataDir(le, DT_x);
        }
    }
}

void Controller::updateMasterList()
{
    // A full refresh of the master list, e.g. if a data directory was edited in the UI (triggering a parse);
    masterListDT.clear();
    masterListDT << DT_BIAS << DT_DARK << DT_FLATOFF << DT_FLAT << DT_SCIENCE << DT_SKY << DT_STANDARD;
}

void Controller::rereadScienceDataDirReceived()
{
    dataTreeUpdateOngoing = true;
    omp_set_lock(&memoryLock);
    emit clearMemoryView();

    for (auto &data: DT_SCIENCE) {
        delete data;
        data = nullptr;
    }
    DT_SCIENCE.clear();

    parseDataDir(mainGUI->ui->setupScienceLineEdit, DT_SCIENCE);

    updateMasterList();

    emit populateMemoryView();
    omp_unset_lock(&memoryLock);
    dataTreeUpdateOngoing = false;

    // And do the final GUI updates to signal processing has finished
    if (successProcessing) {
        emit progressUpdate(100);
        pushEndMessage(taskBasename, "SCIENCE");
    }
}

// Receiving end from setMemoryLock calls within other classes
void Controller::setMemoryLockReceived(bool locked)
{
    if (locked) omp_set_lock(&memoryLock);
    else omp_unset_lock(&memoryLock);
}

// Receiving end from setWCSLock calls within other classes
void Controller::setWCSLockReceived(bool locked)
{
    if (locked) omp_set_lock(&wcsLock);
    else omp_unset_lock(&wcsLock);
}

long Controller::makeListofAllImages(QList<MyImage*> &allMyImages, Data *data)
{
    allMyImages.clear();
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        //        incrementCurrentThreads(lock);
        for (auto &it : data->myImageList[chip]) {
            allMyImages.append(it);
        }
    }
    return allMyImages.length();
}

// Rescan the data tree if a LineEdit was successfully edited
void Controller::dataTreeEditedReceived()
{
    // Do not do anything while loading a new project (causes crashes in memoryviewer)
    if (mainGUI->readingSettings) return;

    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender());

    // CASE 1: The main data dir has changed. Must update everything.
    QString maindir = mainGUI->ui->setupMainLineEdit->text();
    if (lineEdit == mainGUI->ui->setupMainLineEdit) {
        QDir dir(maindir);
        if (dir.exists()) mapDataTree();
        return;
    }

    // CASE 2: A single element of the data tree has changed
    QStringList list = datadir2StringList(lineEdit);

    // Loop over the directories and check whether they exist
    for (int i=0; i<list.size(); ++i) {
        // check if path is absolute, if not, prepend main path
        QString dirname = list.at(i);
        QDir dir(dirname);
        if (!dir.isAbsolute()) {
            dirname = maindir + "/" + dirname;
            dir = QDir(dirname);
        }
        if (!dir.exists()) {
            return;
        }
    }

    dataTreeUpdateOngoing = true;
    omp_set_lock(&memoryLock);
    emit clearMemoryView();

    QString name = sender()->objectName();
    if (name == "setupBiasLineEdit") parseDataDir(lineEdit, DT_BIAS);
    else if (name == "setupDarkLineEdit") parseDataDir(lineEdit, DT_DARK);
    else if (name == "setupFlatoffLineEdit") parseDataDir(lineEdit, DT_FLATOFF);
    else if (name == "setupFlatLineEdit") parseDataDir(lineEdit, DT_FLAT);
    else if (name == "setupScienceLineEdit") parseDataDir(lineEdit, DT_SCIENCE);
    else if (name == "setupSkyLineEdit") parseDataDir(lineEdit, DT_SKY);
    else if (name == "setupStandardLineEdit") parseDataDir(lineEdit, DT_STANDARD);

    updateMasterList();

    emit populateMemoryView();
    omp_unset_lock(&memoryLock);
    dataTreeUpdateOngoing = false;
}

void Controller::checkForUnsavedImages(long &numUnsavedLatest, long &numUnsavedBackup)
{
    numUnsavedLatest = 0;
    numUnsavedBackup = 0;
    for (auto &DT_x : masterListDT) {
        for (auto &data : DT_x) {
            data->countUnsavedImages(numUnsavedLatest, numUnsavedBackup);
        }
    }
}

void Controller::writeUnsavedImagesToDrive(bool includeBackup)
{
    emit messageAvailable("Writing images to disk", "controller");
    criticalReceived();

    // Do not need to loop over globalweights (they are always saved)
    for (auto &DT_x : masterListDT) {
        for (auto &data : DT_x) {
            data->writeUnsavedImagesToDrive(includeBackup);
        }
    }

    emit messageAvailable("Done writing images ...", "controller");
}

void Controller::wipeDataTree()
{
    // Lock, so it doesn't interfere with memory progress bar
    dataTreeUpdateOngoing = true;
    omp_set_lock(&memoryLock);
    emit clearMemoryView();

    releaseAllMemory();

    // The call above just releases the processed pixels kept in memory, it does NOT clear the image lists as such
    for (auto &DT_x : masterListDT) {
        if (!DT_x.isEmpty()) {
            for (auto &it : DT_x) {
                it->dataInitialized = false;
                it->processingStatus = nullptr;
                it->myImageList.clear();
                delete it;
                it = nullptr;
            }
            DT_x.clear();
        }
    }

    if (globalweights_created) {
        delete GLOBALWEIGHTS;
        GLOBALWEIGHTS = nullptr;
        globalweights_created = false;
    }

    masterListDT.clear();
    emit clearMemoryView();

    omp_unset_lock(&memoryLock);
    dataTreeUpdateOngoing = false;
}

void Controller::newProjectLoadedReceived()
{
    // wipeDataTree();
    // Done in MainWindow::on_setupProjectLoadPushButton_clicked();
    // Must be done then because e.g. the number of chips changes.

    // Infer the status of the project
    statusOld = mainGUI->status.getStatusFromHistory();

    // Re-read full data tree
    mapDataTree();
}

void Controller::displayProgress()
{
    emit progressUpdate(progress);
}

void Controller::updateAll()
{
    // Reset all masks
    mask->reset();
    mask->instData = instData;
    mask->initMasks();

    // For GROND (and other multi-channel instruments)
    provideAlternativeMask();

    // instData member is a pointer and does not need updating when a new instrument is selected (true? must check!)
    mapDataTree();   // updates the memory view
}

// so far called in MainGUIWorker::runTask(); should be done better by signal emission
void Controller::loadPreferences()
{
    // Start with max number of CPUs (updated with user preference below)
    QSettings settings("THELI", "PREFERENCES");
    maxCPU = settings.value("prefCPUSpinBox", QThread::idealThreadCount()).toInt();
    useGPU = settings.value("prefGPUCheckBox", false).toBool();
    maxRAM = 0.75 * settings.value("prefMemorySpinBox").toInt();                   // 75% of RAM
    verbosity = settings.value("prefVerbosityComboBox", 1).toInt();
    if (settings.value("prefIntermediateDataComboBox", "If necessary") == "If necessary") alwaysStoreData = false;
    else alwaysStoreData = true;
    minimizeMemoryUsage = settings.value("prefMemoryCheckBox", false).toBool();

    availableThreads = maxCPU;

    verbosity = settings.value("prefVerbosityComboBox").toInt();

    if (settings.status() != QSettings::NoError) {
        emit messageAvailable("WARNING: Could not retrieve CPU and GPU parameters from preferences. Parallelization deactivated.", "warning");
        maxCPU = 1;
        maxThreadsIO = 1;
        maxExternalThreads = 1;
        maxInternalThreads = 1;
        maxRAM = 1024;
        useGPU = false;
        verbosity = 1;
    }

    // We have maxExternalThreads, which is the max number of threads working on independent detectors.
    // If more CPUs than detectors, limit this number so that we have left-over threads for further parallelization
    // (internal threads in Data class)
    if (maxCPU > instData->numUsedChips) {
        //        maxExternalThreads = instData->numUsedChips;      // NO! The loops skipped for a bad chip will just wait until another CPU becomes available.
        maxExternalThreads = instData->numChips;                    // keep full number of CPUs instead
        maxInternalThreads = maxCPU - maxExternalThreads + 1;
    }
    else {
        maxExternalThreads = maxCPU;
        maxInternalThreads = 1;
    }

    pushParallelizationToData(DT_BIAS);
    pushParallelizationToData(DT_DARK);
    pushParallelizationToData(DT_FLATOFF);
    pushParallelizationToData(DT_FLAT);
    pushParallelizationToData(DT_SCIENCE);
    pushParallelizationToData(DT_SKY);
    pushParallelizationToData(DT_STANDARD);

    if (GLOBALWEIGHTS != nullptr) pushParallelizationToData(GLOBALWEIGHTS);

    // For Swarp coaddition
    externalProcesses.resize(maxCPU);
    for (int i=0; i<maxCPU; ++i) {
        QProcess *process = new QProcess();
        //        connect(process, &QProcess::readyReadStandardOutput, this, &Controller::processExternalStdout);
        //        connect(process, &QProcess::readyReadStandardError, this, &Controller::processExternalStderr);
        externalProcesses[i] = process;
    }
    swarpCommands.resize(maxCPU);
    threadsFinished.fill(false, maxCPU);
    swarpWorkers.resize(maxCPU);
    workerThreads.resize(maxCPU);
}

void Controller::pushParallelizationToData(QList<Data*> DT_x)
{
    if (DT_x.isEmpty()) return;

    for (auto &it : DT_x) {
        if (it == nullptr) continue;
        it->maxCPU = maxCPU;
        it->maxExternalThreads = maxExternalThreads;
        it->maxThreadsIO = maxThreadsIO;
        it->useGPU = useGPU;
        it->maxRAM = maxRAM;
    }
}

void Controller::pushParallelizationToData(Data *data)
{
    if (data == nullptr) return;

    data->maxCPU = maxCPU;
    data->maxExternalThreads = maxExternalThreads;
    data->maxThreadsIO = maxThreadsIO;
    data->useGPU = useGPU;
    data->maxRAM = maxRAM;
}

void Controller::addToProgressBarReceived(const float differential)
{
    omp_set_lock(&progressLock);
    progress += differential;
    emit progressUpdate(progress);
    omp_unset_lock(&progressLock);
}

void Controller::releaseAllMemory()
{
    // memory lock set in caller

    // Globalweights
    if (GLOBALWEIGHTS != nullptr) {
        GLOBALWEIGHTS->releaseAllMemory();
    }
    // Pass through normal data tree
    for (auto &DT_x : masterListDT) {
        for (auto &data : DT_x) {
            if (data == nullptr) continue;
            data->releaseAllMemory();
        }
    }
}

void Controller::doDataFitInRAM(const long nImages, const long storageSize)
{
    if (alwaysStoreData) return;    // we are good

    if (nImages*storageSize > maxRAM) {
        alwaysStoreData = true;
        QSettings settings("THELI", "PREFERENCES");
        settings.setValue("prefIntermediateDataComboBox", "Always");
        if (settings.status() != QSettings::NoError) {
            emit messageAvailable("Could not update preferences concerning intermediate data storage", "warning");
        }
        else {
            emit messageAvailable("******************************************************", "note");
            emit messageAvailable("High memory use expected. Will write FITS images to drive on the fly.", "note");
            emit messageAvailable("******************************************************<br>", "note");
        }
    }
}

void Controller::releaseMemory(float RAMneededThisThread, int numThreads, QString mode)
{
    // Requested by several threads, hence this must be locked
    omp_set_lock(&memoryLock);
    float RAMfreed = 0.;

    bool RAMwasReallyReleased = false;
    float currentTotalMemoryUsed = mainGUI->myRAM->getRAMload();
    // Globalweights
    if (GLOBALWEIGHTS != nullptr && globalweights_created) {
        GLOBALWEIGHTS->releaseMemory(RAMneededThisThread, RAMneededThisThread*numThreads, currentTotalMemoryUsed, mode);
    }
    // Pass through normal data tree
    for (auto &DT_x : masterListDT) {
        for (auto &data : DT_x) {
            if (data == nullptr) continue;
            // memviewer is updated by signals emitted by MyImage class
            float released = data->releaseMemory(RAMneededThisThread, RAMneededThisThread*numThreads, currentTotalMemoryUsed, mode);
            if (released >= 0.) {
                RAMfreed += released;
                RAMwasReallyReleased = true;
            }
        }
    }

    //    float RAMstillneeded = RAMneededThisThread - RAMfreed;

    if (RAMfreed < RAMneededThisThread
            && RAMwasReallyReleased
            && currentTotalMemoryUsed > 0.
            && RAMfreed < currentTotalMemoryUsed - 100                        // 100 is to suppress insignificant warnings
            && RAMneededThisThread > maxRAM - currentTotalMemoryUsed) {
        //            && !swapWarningShown) {
        if (verbosity > 1) {
            emit messageAvailable(QString::number(long(RAMneededThisThread)) + " MB requested, " + QString::number(long(RAMfreed))
                                  + " MB released. Try fewer CPUs to avoid swapping.", "warning");
        }
        swapWarningShown = true;
    }

    //    emit messageAvailable("Released "+QString::number(long(RAMfreed)) + " MB", "note");
    if (RAMfreed >= RAMneededThisThread && RAMwasReallyReleased) {
        if (verbosity >= 2) emit messageAvailable("Released "+QString::number(long(RAMfreed)) + " MB", "note");
    }

    omp_unset_lock(&memoryLock);
}

// This function is called after each task to respect the maximum amount of memory allowed by the user.
// Actual use may overshoot during processing
void Controller::satisfyMaxMemorySetting()
{
    // Requested by several threads, hence this must be locked
    omp_set_lock(&memoryLock);

    float RAMfreed = 0.;

    bool RAMwasReallyReleased = false;
    float currentTotalMemoryUsed = mainGUI->myRAM->getRAMload();
    float mustReleaseRAM = currentTotalMemoryUsed - maxRAM;
    // Globalweights
    if (GLOBALWEIGHTS!= nullptr) {
        GLOBALWEIGHTS->releaseMemory(mustReleaseRAM, mustReleaseRAM, currentTotalMemoryUsed);
    }
    // Pass through normal data tree
    for (auto &DT_x : masterListDT) {
        for (auto &data : DT_x) {
            float released = data->releaseMemory(mustReleaseRAM, mustReleaseRAM, currentTotalMemoryUsed);
            if (released >= 0.) {
                RAMfreed += released;
                RAMwasReallyReleased = true;
            }
        }
    }

    if (RAMfreed < mustReleaseRAM
            && RAMwasReallyReleased
            && !swapWarningShown) {
        emit messageAvailable("Tried to release "+QString::number(long(mustReleaseRAM)) + " MB, " + QString::number(long(RAMfreed))
                              + " MB actually released.", "warning");
        swapWarningShown = true;
    }

    if (RAMfreed >= mustReleaseRAM && RAMwasReallyReleased) {
        if (verbosity >= 2) emit messageAvailable("Released "+QString::number(long(RAMfreed)) + " MB", "note");
    }

    omp_unset_lock(&memoryLock);
}

void Controller::checkSuccessProcessing(const Data *data)
{
    if (userStop || userKill) {
        emit messageAvailable("Aborted", "stop");
        return;
    }

    if (!data->successProcessing) {
        successProcessing = false;
        emit messageAvailable("Data processing in " + data->dirName + " unsuccessful.", "error");
        criticalReceived();
    }
}

// Shows a message box where the user can optionally trigger a clearance of the error state.
// The task that triggers this
bool Controller::testResetDesire(const Data *data)
{
    if (!data->successProcessing) {
        emit showMessageBox("Controller::RESET_REQUESTED", data->subDirName, "");
        return false;
    }
    return true;
}

void Controller::getDetectorSections()
{
    overscanX.clear();
    overscanY.clear();
    dataSection.clear();

    overscanX.resize(instData->numChips);
    overscanY.resize(instData->numChips);
    dataSection.resize(instData->numChips);

    QVector<int> xmin = instData->overscan_xmin;
    QVector<int> xmax = instData->overscan_xmax;
    QVector<int> ymin = instData->overscan_ymin;
    QVector<int> ymax = instData->overscan_ymax;

    // to stay safe, i include chips that are not used
    for (int chip=0; chip<instData->numChips; ++chip) {
        // Overscan X
        QVector<long> overscanxRegion;
        if (!xmin.isEmpty() && !xmax.isEmpty()) overscanxRegion << xmin[chip] << xmax[chip];
        overscanX[chip] << overscanxRegion;

        // Overscan Y
        QVector<long> overscanyRegion;
        if (!ymin.isEmpty() && !ymax.isEmpty()) overscanyRegion << ymin[chip] << ymax[chip];
        overscanY[chip] << overscanyRegion;

        // Data Section
        QVector<long> section;
        section << instData->cutx[chip];
        section << instData->cutx[chip] + instData->sizex[chip] - 1;   // sizex is not a coordinate, but the number of pixels along this axis. Hence -1
        section << instData->cuty[chip];
        section << instData->cuty[chip] + instData->sizey[chip] - 1;   // sizey is not a coordinate, but the number of pixels along this axis. Hence -1
        dataSection[chip] << section;
    }
}

QList<QVector<float>> Controller::getNonlinearityCoefficients()
{
    QList<QVector<float>> coeffs;
    QString coeffsFileName = instrument_dir+"/"+instData->name+".nldat";
    QFile coeffsFile(coeffsFileName);

    // Read the coefficients
    if( !coeffsFile.open(QIODevice::ReadOnly)) {
        // return empty list if file does not exist
        return coeffs;
    }

    QTextStream in(&coeffsFile);
    while(!in.atEnd()) {
        QString line = in.readLine().simplified();
        if (line.isEmpty() || line.contains("#")) continue;
        // Extract the coefficients and put them into a QVector<float>
        line = line.simplified();
        QStringList values = line.split(" ");
        QVector<float> vecData;
        for (auto &it : values) {
            vecData.push_back(it.toFloat());
        }
        coeffs.append(vecData);
    }
    coeffsFile.close();

    if (coeffs.length() != instData->numChips) {
        QMessageBox::warning(this, tr("Inconsistent number of nonlinearity coefficients"),
                             tr("The file with nonlinearity coefficients,\n")+coeffsFileName+
                             tr("contains entries (lines) for")+QString::number(coeffs.length())+" detectors.\n"+
                             tr("However, ")+instData->name+tr(" has")+QString::number(instData->numChips)+tr(" detectors.")+
                             tr("Data processing will continue without non-linearity correction."),
                             QMessageBox::Ok);
        coeffs.clear();
    }

    return coeffs;
}

void Controller::resetErrorStatusReceived(QString dirName)
{
    Data *data = getDataAll(dirName);

    if (data != nullptr) {
        data->resetSuccessProcessing();
    }
}

QVector<QString> Controller::getBackgroundThresholds(const int loop, const bool twoPass, const QString DT, const QString DMIN, bool &doSourceDetection)
{
    QVector<QString> thresholds;

    // No thresholds specified, or in first loop of twoPass mode: don't do source detection
    if (DT.isEmpty()
            || DMIN.isEmpty()
            || (twoPass && loop == 0)) {
        doSourceDetection = false;
        thresholds << "" << "";   // not evaluated later-on
        return thresholds;
    }
    // Thresholds specified: detection depends on whether we are in twopass mode or not
    else if (!twoPass || loop == 1) {        // loop == 1 implies twoPass = true
        doSourceDetection = true;
        thresholds << DT << DMIN;
        return thresholds;
    }
    else {
        // should be cought by previous 'else if'
        emit messageAvailable("Controller::getBackgroundThresholds(): Code should never enter here!", "error");
        criticalReceived();
        successProcessing = false;
        doSourceDetection = false;
        thresholds << DT << DMIN;
        return thresholds;
    }
}

// Updates the processing status, and also creates a backup file on drive (if the FITS file exists)
void Controller::updateImageAndData(MyImage *image, Data *data)
{
    bool *s = nullptr;

    if (taskBasename == "HDUreformat") s = &image->processingStatus->HDUreformat;
    else if (taskBasename == "Processscience") s = &image->processingStatus->Processscience;
    else if (taskBasename == "Chopnod") s = &image->processingStatus->Chopnod;
    else if (taskBasename == "Background") s = &image->processingStatus->Background;
    else if (taskBasename == "Collapse") s = &image->processingStatus->Collapse;
    else if (taskBasename == "Starflat") s = &image->processingStatus->Starflat;
    else if (taskBasename == "Skysub") s = &image->processingStatus->Skysub;
    else {
        emit messageAvailable("Controller::updateProcessingStatus(): Invalid taskBasename. This is a bug!", "error");
        criticalReceived();
        return;
    }

    // Status of Data class becomes immediately false if the status of a single image is false
    if (image->successProcessing) {
        // Update the status (processing was successful)
        *s = true;
        QString statusNew = image->processingStatus->getStatusString();
        // Update members in MyImage class
        image->processingStatus->statusString = statusNew;
        image->baseName = image->chipName + statusNew;
        // New pixel data are not yet on drive
        image->imageOnDrive = false;
    }
    else {
        *s = false;
        data->successProcessing = false;
    }
}

// The top level entry function for MainWindow to initiate a task
void Controller::runTask()
{
    if (!successProcessing) return;

    // Reset the process progress bar
    emit resetProgressBar();

    // call the function by its string representation (needs a const char *)
    bool test = true;
    if (taskBasename == "processScience") {
        // Calls
        test = QMetaObject::invokeMethod(this, ("taskInternal"+taskBasename).toStdString().c_str(),
                                         Qt::DirectConnection, Q_ARG(QList<Data*>, DT_SCIENCE));
    }
    else {
        test = QMetaObject::invokeMethod(this, ("taskInternal"+taskBasename).toStdString().c_str(),
                                         Qt::DirectConnection);
    }

    if (!test) {
        emit messageAvailable("Controller::runTask(): Could not evaluate QMetaObject for " +taskBasename, "error");
        criticalReceived();
        return;
    }
}

// Identify the Data class that corresponds to directory dirName, and return a link to that class
Data* Controller::getData(QList<Data *> DT_x, QString dirName)
{
    for (auto &it : DT_x) {
        if (QDir::cleanPath(it->dirName) == QDir::cleanPath(mainDirName + "/" + dirName)) {
            // check if it contains mastercalib data
            it->checkPresenceOfMasterCalibs();
//            if (it->isEmpty()) return nullptr;
            if (it->hasAllMasterCalibs) return it;    // otherwise a dir with only the master calibs will not be accepted
            if (it->isEmpty()) return nullptr;
            else return it;
        }
    }

    emit messageAvailable("Controller::getData(): Directory " + dirName + " not found in Data class for " + DT_x[0]->dirName, "error");
    criticalReceived();

    return nullptr;
}

// Identify the Data class that corresponds to directory dirName, and return a link to that class
// Used if we don't know the top level category of that directory
// TODO: Check what happens if we use e.g. the same science directory as a science directory as well as a flat directory. Prohibit this at software level?
Data* Controller::getDataAll(QString dirName)
{
    for (auto &DT_x : masterListDT) {
        for (auto &it : DT_x) {
            if (QDir::cleanPath(it->dirName) == QDir::cleanPath(mainDirName + "/" + dirName)) {
                // check if it contains mastercalib data
                it->checkPresenceOfMasterCalibs();
                return it;
            }
        }
    }

    emit messageAvailable("Controller::getDataAll(): Directory " + dirName + " not found in Data classes.", "error");
    criticalReceived();
    return nullptr;
}

// Decide which data can (or cannot) be deleted from memory
void Controller::memoryDecideDeletableStatus(Data *data, bool deletable)
{
    for (int chip=0; chip<instData->numChips; ++chip) {
        data->memorySetDeletable(chip, "dataBackupL1", deletable);
    }
}

QString Controller::getUserParamLineEdit(const QLineEdit *le)
{
    QString value = le->text();
    if (value == "") value = cdw->defaultMap[le->objectName()];
    return value;
}

QString Controller::getUserParamCheckBox(const QCheckBox *cb)
{
    if (cb->isChecked()) return "Y";
    else return "N";
}

QString Controller::getUserParamComboBox(const QComboBox *cb)
{
    return cb->currentText();
}

void Controller::pushBeginMessage(const QString idstring, const QString targetdir)
{
    QString message = commentMap.value(idstring);
    emit messageAvailable("<br>##############################################", "output");
    emit messageAvailable(message+" "+targetdir, "controller");
    emit messageAvailable("##############################################<br>\n", "output");
}

void Controller::pushEndMessage(const QString idstring, const QString targetdir)
{
    QString message = commentMap.value(idstring);
    emit messageAvailable("<br>##############################################", "output");
    emit messageAvailable(message+" "+targetdir+"... DONE.", "controller");
    emit messageAvailable("##############################################<br>\n", "output");
}

void Controller::updateMemoryPreference(bool isRAMminimized)
{
    minimizeMemoryUsage = isRAMminimized;

    sendMemoryPreferenceToImages(DT_BIAS);
    sendMemoryPreferenceToImages(DT_DARK);
    sendMemoryPreferenceToImages(DT_FLATOFF);
    sendMemoryPreferenceToImages(DT_FLAT);
    sendMemoryPreferenceToImages(DT_SCIENCE);
    sendMemoryPreferenceToImages(DT_SKY);
    sendMemoryPreferenceToImages(DT_STANDARD);

    if (GLOBALWEIGHTS != nullptr) sendMemoryPreferenceToImages(DT_STANDARD);
}

void Controller::updateIntermediateDataPreference(QString intermediateDataPreference)
{
    if (intermediateDataPreference == "Always") alwaysStoreData = true;
    else alwaysStoreData = false;
}

void Controller::criticalReceived()
{
    abortProcess = true;
    emit messageAvailable("Abort.", "stop");
    monitor->raise();
}

void Controller::warningReceived()
{
    monitor->raise();
}

void Controller::messageAvailableReceived(QString message, QString type)
{
    emit messageAvailable(message, type);
}

void Controller::appendOKReceived()
{
    emit appendOK();
}

void Controller::showMessageBoxReceived(QString trigger, QString part1, QString part2)
{
    emit showMessageBox(trigger, part1, part2);
}

void Controller::updateVerbosity(int verbosityLevel)
{
    verbosity = verbosityLevel;
}

void Controller::sendMemoryPreferenceToImages(QList<Data*> DT_x)
{
    if (DT_x.isEmpty()) return;

    // including chips the user does not want to use
    for (auto &data : DT_x) {
        for (int chip=0; chip<data->instData->numChips; ++chip) {
            for (auto &it : data->myImageList[chip]) {
                it->minimizeMemoryUsage = minimizeMemoryUsage;
            }
        }
    }
}

void Controller::restoreAllRawData()
{
    // memory lock set in caller; NO IT IS NOT!
    // restore button deactivated while task is running
#pragma omp parallel for num_threads(maxCPU)
    for (int i=0; i<masterListDT.length(); ++i) {
        auto &DT_x = masterListDT[i];
        for (auto &it : DT_x) {
            // Just a paranoid safety barrier, in case the user provides incomplete data and then clicks on 'restore'
            if (it->dir.absolutePath() == QDir::homePath()) {
                QMessageBox::warning( this, "Dangerous data tree",
                                      "The full path to " + it->dirName+"is identical to your home directory!\nTHELI will not delete / restore any data in this directory.");
                continue;
            }
            else {
                it->restoreRAWDATA();
            }
        }
    }

    if (GLOBALWEIGHTS != nullptr) GLOBALWEIGHTS->releaseAllMemory();
    QDir globalweightsDir(mainDirName+"/GLOBALWEIGHTS/");
    if (globalweightsDir.exists()) {
        emit messageAvailable("Deleting " + mainDirName+"/GLOBALWEIGHTS/" + "...", "controller");
        globalweightsDir.removeRecursively();
    }

    QDir weightsDir(mainDirName+"/WEIGHTS/");
    if (weightsDir.exists()) {
        emit messageAvailable("Deleting " + mainDirName+"/WEIGHTS/" + "...", "controller");
        weightsDir.removeRecursively();
    }

    updateMasterList();

    mainGUI->status.clearAllCheckBoxes();

    cdw->ui->ARCpmRALineEdit->clear();
    cdw->ui->ARCpmDECLineEdit->clear();
}

void Controller::printCfitsioError(QString funcName, int status)
{
    if (status) {
        CfitsioErrorCodes *errorCodes = new CfitsioErrorCodes(this);
        emit messageAvailable("Controller::"+funcName+":<br>" + errorCodes->errorKeyMap.value(status), "error");
        criticalReceived();
        successProcessing = false;
    }
}

// Cannot be done above when data classes are defined (because memoryviewer has not been declared in mainwindow.cc when the controller is defined.
void Controller::connectDataWithMemoryViewer()
{
    for (auto &DT_x : masterListDT) {
        if (!DT_x.isEmpty()) {
            for (auto &it : DT_x) {
                connect(it, &Data::statusChanged, memoryViewer, &MemoryViewer::updateStatusCheckBoxesReceived);
            }
        }
    }

    if (globalweights_created && GLOBALWEIGHTS != nullptr) {
        connect(GLOBALWEIGHTS, &Data::statusChanged, memoryViewer, &MemoryViewer::updateStatusCheckBoxesReceived);
    }
}

void Controller::activationWarningReceived(QString imagestatus, QString drivestatus)
{
    QMessageBox::warning( this, "THELI: Incoherent processing states",
                          "This image has a different processing status (" + imagestatus + ") "
                          +"than the currently active images (" + drivestatus + "). It cannot be reactivated like this.\n"
                          +"To include this image in the processing, you must either restore the corresponding processing status for all active images, "
                           "or close THELI and manually restore all suitable images, or start from the raw data.");
}

bool Controller::isExposureActive(QList<MyImage*> exposure)
{
    bool active = false;
    for (auto &it : exposure) {
        if (it->activeState == MyImage::ACTIVE) {
            active = true;
            break;
        }
    }
    return active;
}

/*
void Controller::decrementCurrentThreads(omp_lock_t &lock)
{
    omp_set_lock(&lock);
    //    --currentExternalThreads;
    --currentThreadsRunning;
    if (availableThreads < maxCPU) ++availableThreads;
    omp_unset_lock(&lock);
}
*/

/*
void Controller::incrementCurrentThreads(omp_lock_t &lock)
{
    omp_set_lock(&lock);
    //    ++currentExternalThreads;
    ++currentThreadsRunning;
    if (availableThreads > 0) --availableThreads;
    omp_unset_lock(&lock);
}
*/

/*
int Controller::reserveAvailableThreads(omp_lock_t &lock)
{
    int numInternalThreads = 1;
    // Consume remaining available threads.
    while (availableThreads > 0) {
        omp_set_lock(&lock);
        ++numInternalThreads;
        --availableThreads;
        omp_unset_lock(&lock);
        // Give the other threads a chance
        QTest::qWait(100);
    }
    return numInternalThreads;
}
*/

/*
void Controller::makeThreadsAvailable(omp_lock_t &lock, int numberThreadsBlocked)
{
    omp_set_lock(&lock);
    availableThreads += numberThreadsBlocked;
    omp_unset_lock(&lock);
}
*/

/*
void Controller::restoreData(MyImage *myImage, QString backupDirName)
{
    // Moves an image from the backupDir to the current Dir
    QString currentPath = myImage->path;
    QString backupPath = myImage->path + "/" + backupDirName + "/";    // DO NOT use append() because it will also change the variable one appends to.
    QFile image(backupPath+"/"+myImage->name);
    if (!image.exists()) return;
    if (!image.rename(currentPath+"/"+myImage->name)) {
        QString command = "mv " + backupPath+"/"+myImage->name + currentPath+"/"+myImage->name;
        emit messageAvailable("CTRL: restoreData(): Could not execute the following operation: <br> " + command, "error");
        criticalReceived();
    }
}
*/

/*
int Controller::getInternalThreads(int chip)
{
    QVector<QString> PARA(instData->numChips);
    maxExternalThreads;
    int i=1;
    while (i<=instData->numChips) {
        PARA[i] = "";
        ++i;
    }

    // Assign chips to CPUs
    int k = 1;
    while (k <= maxExternalThreads) {
        int ACTUPROC = 1;
        while (ACTUPROC <= instData->numChips && k <= maxExternalThreads) {
            PARA[ACTUPROC].append(QString::number(k));
            ++k;
            ++ACTUPROC;
        }
    }

    k = 1;
    QString nthread = "";
    while (k <= instData->numChips) {
       int numthread = PARA[k].split(" ").length();
       if (numthread == 0) numthread = 1;
       nthread.append(numthread);
        ++k;
    }

    return nthread.split(" ").at(chip);
}
*/

/*
void Controller::incrementProgress()
{
    omp_set_lock(&progressLock);
    progress += progressStepSize;
    emit progressUpdate(progress);
    omp_unset_lock(&progressLock);
}
*/

/*
void Controller::incrementProgressHalfStep()
{
    //    omp_set_lock(&progressLock);
    progress += progressStepSize/2.;
    emit progressUpdate(progress);
    //    omp_unset_lock(&progressLock);
}
*/

/*
void Controller::incrementProgressCombinedStep()
{
    //    omp_set_lock(&progressLock);
    progress += progressCombinedStepSize;
    emit progressUpdate(progress);
    //    omp_unset_lock(&progressLock);
}
*/

/*
void Controller::rereadDataDir(QLineEdit *le, QList<Data *> &DT_x)
{
    dataTreeUpdateOngoing = true;
    omp_set_lock(&memoryLock);
    emit clearMemoryView();

    for (auto &data: DT_x) {
        delete data;
    }
    DT_x.clear();

    parseDataDir(le, DT_x);

    updateMasterList();

    emit populateMemoryView();
    omp_unset_lock(&memoryLock);
    dataTreeUpdateOngoing = false;
}
*/

/*
void Controller::displayDriveSpace()
{
    // Storage space in the main/home directory
    QString maindir = mainGUI->ui->setupMainLineEdit->text();

    double GBtotal_data, GBfree_data, GBused_data;
    double GBtotal_home, GBfree_home, GBused_home;

    QStorageInfo storage_home(QDir::homePath());
    if (storage_home.isValid() && storage_home.isReady()) {
        GBtotal_home = storage_home.bytesTotal()/1024./1024./1024.;
        GBfree_home = storage_home.bytesAvailable()/1024./1024./1024.;
        GBused_home = GBtotal_home - GBfree_home;
    }
    else {
        emit messageAvailable("Controller::displayDriveSpace(): Cannot determine home directory!", "error");
        return;
    }

    QStorageInfo storage_data(maindir);
    if (storage_data.isValid() && storage_data.isReady()) {
        GBtotal_data = storage_data.bytesTotal()/1024./1024./1024.;
        GBfree_data = storage_data.bytesAvailable()/1024./1024./1024.;
        GBused_data = GBtotal_data - GBfree_data;
    }
    else {
        GBtotal_data = GBtotal_home;
        GBfree_data = GBfree_home;
        GBused_data = GBused_home;
    }

    mainGUI->driveProgressBar->setRange(0, GBtotal_data);

    QString datadiskstring = QString::number(GBfree_data,'f',2) + " GB left";

    // check if the data disk warning should be activated
    if (GBfree_data <= mainGUI->diskwarnPreference/1024.) {           // preference is given in MB
        if (!mainGUI->datadiskspace_warned) {
            mainGUI->datadiskspace_warned = true;
            if (maindir.isEmpty()) maindir = QDir::homePath();
            QMessageBox::warning( this, "THELI: DATA DISK SPACE LOW",
                                  "The remaining disk space on\n\n"
                                  + maindir+"\n\nis less than your warning threshold of "
                                  + QString::number(mainGUI->diskwarnPreference)+" MB.\n"
                                                                                 "The threshold can be set under Edit->Preferences in the main menu. "
                                                                                 "This warning will not be shown anymore in this session, "
                                                                                 "unless you update the threshold to a new value.");
        }
    }
    else mainGUI->datadiskspace_warned = false;

    if (GBfree_home <= 0.1) {
        if (!mainGUI->homediskspace_warned) {
            mainGUI->homediskspace_warned = true;
            QMessageBox::warning( this, "THELI: HOME DISK SPACE LOW",
                                  "THELI: You are running low (<100 MB) on disk space in your home directory!\n");
        }
    }
    else mainGUI->homediskspace_warned = false;

    mainGUI->driveProgressBar->setFormat("Drive: "+datadiskstring);
    mainGUI->driveProgressBar->setValue(GBused_data);
}
*/


/*
void Controller::processExternalStdout()
{
    QProcess *process = qobject_cast<QProcess*>(sender());
    QString stdout(process->readAllStandardOutput());
    //    QString stdout(externalProcess->readLine());
    //    emit messageAvailable(stdout, "normal");
}

void Controller::processExternalStderr()
{
    QProcess *process = qobject_cast<QProcess*>(sender());
    QString stderr(process->readAllStandardError());
    emit messageAvailable(stderr, "normal");
}
*/


/*
void Controller::splitterMemoryReceived(long memoryUsed)
{
#pragma omp atomic
    splitterMemoryUsed += memoryUsed;

    emit updateMemoryProgressBar(splitterMemoryUsed / 1024 / 1024);
}
*/

/*
void Controller::displayCPUload()
{
    //    int CPUload = myCPU->getCPUload();
    float CPUload = myCPU->getCurrentValue();

    QString CPUstring = QString::number(int(CPUload)) + " %";
    mainGUI->cpuProgressBar->setFormat("CPU: "+CPUstring);
    mainGUI->cpuProgressBar->setValue(int(CPUload));
}

void Controller::displayRAMload()
{
    float RAMload = myRAM->getCurrentValue();

    QString RAMstring = QString::number(long(RAMload)) + " MB";
    mainGUI->memoryProgressBar->setFormat("RAM: %p% ("+RAMstring+")");
    mainGUI->memoryProgressBar->setValue(int(RAMload));
}
*/

/*
void Controller::displayMemoryTotalUsed()
{
    // CHECK: in principle, the memoryLock should be sufficient to avoid crashes
    if (dataTreeUpdateOngoing) return;

    // Interferes with releaseMemory(), e.g. when memoryCurrentFootprint() evaluates the
    // capacity of a vector while it is being squeezed at the same time.
    // Cannot do this inside getMemoryTotalUsed() itself, because that is called by releasememory(), which in turn sets the lock
    omp_set_lock(&memoryLock);
    float totalMemory = getMemoryTotalUsed();
    omp_unset_lock(&memoryLock);

    QString memoryString = QString::number(long(totalMemory)) + " MB";
    mainGUI->memoryProgressBar->setFormat("RAM: %p% ("+memoryString+")");
    mainGUI->memoryProgressBar->setValue(long(totalMemory));
}
*/

/*
// crashes when used right at startup
// CHECK: still crashes?
QStringList Controller::getFilterList(QString scienceDir)
{
    Data *scienceData = getData(DT_SCIENCE, scienceDir);

    QStringList filterList;

    QList<MyImage*> allMyImages;
    long numMyImages = makeListofAllImages(allMyImages, scienceData);

#pragma omp parallel for num_threads(maxCPU)
    for (int k=0; k<numMyImages; ++k) {
        auto &it = allMyImages[k];
        it->loadHeader();
#pragma omp critical
        {
            if (!filterList.contains(it->filter)) filterList << it->filter;
        }
    }

    return filterList;
}
*/

/*
void Controller::progressUpdateReceived(float progress)
{
    emit progressUpdate(progress);
}
*/


// UNUSED
/*
void Controller::updateSingle()
{
    dataTreeUpdateOngoing = true;
    omp_set_lock(&memoryLock);
    emit clearMemoryView();

    QLineEdit *le = qobject_cast<QLineEdit*>(sender());
    if (le == mainGUI->ui->setupMainLineEdit) mainDirName = le->text();
    else if (le == mainGUI->ui->setupBiasLineEdit) parseDataDir(le, DT_BIAS);
    else if (le == mainGUI->ui->setupDarkLineEdit) parseDataDir(le, DT_DARK);
    else if (le == mainGUI->ui->setupFlatLineEdit) parseDataDir(le, DT_FLATOFF);
    else if (le == mainGUI->ui->setupFlatoffLineEdit) parseDataDir(le, DT_FLAT);
    else if (le == mainGUI->ui->setupScienceLineEdit) parseDataDir(le, DT_SCIENCE);
    else if (le == mainGUI->ui->setupSkyLineEdit) parseDataDir(le, DT_SKY);
    else if (le == mainGUI->ui->setupStandardLineEdit) parseDataDir(le, DT_STANDARD);

    updateMasterList();

    emit populateMemoryView();
    omp_unset_lock(&memoryLock);
    dataTreeUpdateOngoing = false;
}
*/
