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
#include "../mainwindow.h"
#include "../tools/tools.h"
#include "ui_confdockwidget.h"
#include "ui_monitor.h"

#include <QMetaObject>
#include <QVector>
#include <QStringList>
#include <QProgressBar>

void Controller::taskInternalGlobalweight()
{
    QString scienceDir = instructions.split(" ").at(1);
    QString flatDir = instructions.split(" ").at(2);
    QString biasDir = instructions.split(" ").at(3);
    Data *scienceData = getData(DT_SCIENCE, scienceDir);
    if (scienceData == nullptr) return;      // Error triggered by getData();

    Data *biasData = nullptr;  // Can also point to a dark
    Data *flatData = nullptr;
    currentData = scienceData;
    currentDirName = scienceDir;

    pushBeginMessage(taskBasename, scienceDir);
    pushConfigGlobalweight();

    // Link with the correct bias data, if any; dark preferred over bias
    // I have to select dark / bias again even though it is done in tasks.cc already
    if (!mainGUI->ui->setupDarkLineEdit->text().isEmpty()) {
        biasData = getData(DT_DARK, biasDir);
    }
    else if (!mainGUI->ui->setupBiasLineEdit->text().isEmpty()) {
        biasData = getData(DT_BIAS, biasDir);
    }
    // Same for the flat
    if (!mainGUI->ui->setupFlatLineEdit->text().isEmpty()) {
        flatData = getData(DT_FLAT, flatDir);
        // Should we check the filter string?
    }
    // Check if calib data exist)
    if (biasData != nullptr && !biasData->hasAllMasterCalibs) {
        QString part1 = biasData->dirName+"/"+biasData->subDirName+"_?.fits\n";
        emit showMessageBox("Controller::MASTER_BIAS_NOT_FOUND_GLOBW", part1, "");
        successProcessing = false;
        return;
    }
    if (flatData != nullptr && !flatData->hasAllMasterCalibs) {
        QString part1 = flatData->dirName+"/"+flatData->subDirName+"_?.fits\n";
        emit showMessageBox("Controller::MASTER_FLAT_NOT_FOUND_GLOBW", part1, "");
        successProcessing = false;
        return;
    }

    memoryDecideDeletableStatus(scienceData, false);
    // Need to fill myImageList to get Filter keyword (if the user starts fresh with this task after launching THELI)
    if (scienceData->myImageList[0].isEmpty()) scienceData->populate(scienceData->processingStatus->statusString);

    // Get the filter name, first exposure in SCIENCE (assuming same filter for all exposures)
    QString filter = "";
    for (auto &it : scienceData->myImageList[0]) {
        if (!it->filter.isEmpty()) {
            filter = it->filter;
            break;
        }
        else {
            filter = it->imageFITS->readFILTER();
            if (!filter.isEmpty()) break;
        }
    }

    QDir globalweightDir(mainDirName+"/GLOBALWEIGHTS/");
    if (!globalweightDir.exists()) {
        globalweightDir.mkpath(mainDirName+"/GLOBALWEIGHTS/");
        GLOBALWEIGHTS = new Data(instData, mask, mainDirName, "GLOBALWEIGHTS", &verbosity);
        connect(GLOBALWEIGHTS, &Data::messageAvailable, this, &Controller::messageAvailableReceived);
        connect(GLOBALWEIGHTS, &Data::appendOK, this, &Controller::appendOKReceived);
        connect(GLOBALWEIGHTS, &Data::critical, this, &Controller::criticalReceived);
        connect(GLOBALWEIGHTS, &Data::warning, this, &Controller::warningReceived);
        connect(GLOBALWEIGHTS, &Data::showMessageBox, this, &Controller::showMessageBoxReceived);
        connect(GLOBALWEIGHTS, &Data::setMemoryLock, this, &Controller::setMemoryLockReceived, Qt::DirectConnection);
        GLOBALWEIGHTS->progress = &progress;
        GLOBALWEIGHTS->dataType = "GLOBALWEIGHT";
        // GLOBALWEIGHTS->myImageList is empty at this point
        // TODO: should we not make all the connections here for Data class?
    }
    else {
        // remove the globalweight for the present filter only, in this science directory (and then re-create it)
        GLOBALWEIGHTS->resetGlobalWeight(filter);
    }

    bool sameWeight = cdw->ui->CGWsameweightCheckBox->isChecked();
    QString flatMin = cdw->ui->CGWflatminLineEdit->text();
    QString flatMax = cdw->ui->CGWflatmaxLineEdit->text();
    QString threshMin = cdw->ui->CGWdarkminLineEdit->text();
    QString threshMax = cdw->ui->CGWdarkmaxLineEdit->text();
    QString defectKernel = cdw->ui->CGWflatsmoothLineEdit->text();
    QString defectRowTol = cdw->ui->CGWflatrowtolLineEdit->text();
    QString defectColTol = cdw->ui->CGWflatcoltolLineEdit->text();
    QString defectClusTol = cdw->ui->CGWflatclustolLineEdit->text();

    progressStepSize = 100./(float(instData->numChips));

    float nimg = 6;  // bias, flat, 4 for image detection back, seg, measure, mask)
    releaseMemory(nimg*instData->storage*maxExternalThreads, 1);

    // Only need as many threads as chips, max
#pragma omp parallel for num_threads(maxExternalThreads)
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (abortProcess || !successProcessing || instData->badChips.contains(chip)) continue;

        releaseMemory(nimg*instData->storage, maxExternalThreads);

#pragma omp critical
        {
            if (biasData != nullptr) biasData->loadCombinedImage(chip);  // skipped if already in memory
            if (flatData != nullptr) flatData->loadCombinedImage(chip);  // skipped if already in memory
        }

        GLOBALWEIGHTS->initGlobalWeight(chip, flatData, filter, sameWeight, flatMin, flatMax);
        GLOBALWEIGHTS->thresholdGlobalWeight(chip, biasData, filter, threshMin, threshMax);
        GLOBALWEIGHTS->detectDefects(chip, flatData, filter, sameWeight, defectKernel, defectRowTol, defectColTol, defectClusTol);
        GLOBALWEIGHTS->applyMask(chip, filter);
        GLOBALWEIGHTS->writeGlobalWeights(chip, filter);
        if (biasData != nullptr) biasData->unprotectMemory(chip);
        if (flatData != nullptr) flatData->unprotectMemory(chip);
        GLOBALWEIGHTS->myImageList[chip][0]->unprotectMemory();   // List with one entry per chip, only
#pragma omp atomic
        progress += progressStepSize;
    }

    checkSuccessProcessing(GLOBALWEIGHTS);
    satisfyMaxMemorySetting();

    if (successProcessing) {
        emit progressUpdate(100);
        //        pushEndMessage(taskBasename, scienceDir);
    }
}

void Controller::taskInternalIndividualweight()
{
    QString scienceDir = instructions.split(" ").at(1);
    Data *scienceData = getData(DT_SCIENCE, scienceDir);
    if (scienceData == nullptr) return;      // Error triggered by getData();

    currentData = scienceData;
    currentDirName = scienceDir;

    pushBeginMessage(taskBasename, scienceDir);
    pushConfigIndividualweight();

    memoryDecideDeletableStatus(scienceData, false);

    QDir globalweightDir(mainDirName+"/GLOBALWEIGHTS/");
    if (!globalweightDir.exists()) {
        emit messageAvailable("The global weight maps must be created before the individual weight maps.", "error");
        emit criticalReceived();
        successProcessing = false;
        return;
    }
    QDir individualweightDir(mainDirName+"/WEIGHTS/");
    individualweightDir.mkdir(mainDirName+"/WEIGHTS/");

    QString imageMin = cdw->ui->CIWminaduLineEdit->text();
    QString imageMax = cdw->ui->CIWmaxaduLineEdit->text();
    QString range = cdw->ui->CIWbloomRangeLineEdit->text();
    QString minVal = cdw->ui->CIWbloomMinaduLineEdit->text();
    QString aggressiveness = cdw->ui->CIWaggressivenessLineEdit->text();

    getNumberOfActiveImages(scienceData);

    /*
#pragma omp parallel for num_threads(maxExternalThreads)
    for (int chip=0; chip<instData->numChips; ++chip) {
        incrementCurrentThreads(lock);
        for (auto &it : scienceData->myImageList[chip]) {
            if (verbosity >= 0) emit messageAvailable(it->baseName + " : Creating weight map ...", "controller");
            it->readImage(false);  // no dataBackupL1 required; just use dataCurrent, or read from disk if not yet in memory
            it->initWeightfromGlobalWeight(GLOBALWEIGHTS->myImageList[chip]);
            it->thresholdWeight(imageMin, imageMax);
            it->applyPolygons(chip);
            it->maskBloomingSpike(instData->type, range, minVal, cdw->ui->CIWmaskbloomingCheckBox->isChecked());
            incrementProgress();
            it->cosmicsFilter(aggressiveness);
            it->writeWeight(mainDirName+ "/WEIGHTS/" + it->chipName + ".weight.fits");
            it->weightName = it->chipName + ".weight";
            incrementProgress();
        }
    }
    */

    QList<MyImage*> allMyImages;
    long numMyImages = makeListofAllImages(allMyImages, scienceData);

    float nimg = 4;  // weight, global weight, margins
    releaseMemory(nimg*instData->storage*maxCPU, 1);
    scienceData->protectMemory();

    QString instType = instData->type;

#pragma omp parallel for num_threads(maxCPU) firstprivate(instType, mainDirName)
    for (int k=0; k<numMyImages; ++k) {
        if (abortProcess || !successProcessing) continue;

        auto &it = allMyImages[k];

        if (!it->successProcessing) continue;
        int chip = it->chipNumber - 1;
        if (instData->badChips.contains(chip)) continue;
        releaseMemory(nimg*instData->storage, maxCPU);

        if (verbosity >= 0) emit messageAvailable(it->chipName + " : Creating weight map ...", "image");
        it->setupDataInMemorySimple(false);
        // Locking, otherwise the same globalweight will be initialized several times (image geometry not threadsafe)
#pragma omp critical
        {
            it->initWeightfromGlobalWeight(GLOBALWEIGHTS->myImageList[chip]);
        }
        it->thresholdWeight(imageMin, imageMax);
        it->applyPolygons(chip);
        it->maskBloomingSpike(instType, range, minVal, cdw->ui->CIWmaskbloomingCheckBox->isChecked());
#pragma omp atomic
        progress += progressHalfStepSize;
        it->cosmicsFilter(aggressiveness);
        // Must write weights to drive for swarp
        it->writeWeight(mainDirName+ "/WEIGHTS/" + it->chipName + ".weight.fits");
        it->weightOnDrive = true;
        it->weightName = it->chipName + ".weight";
        it->unprotectMemory();
        if (minimizeMemoryUsage) {
            it->freeAll();
        }
        GLOBALWEIGHTS->myImageList[chip][0]->unprotectMemory();   // List with one entry per chip, only; could also do:
        // GLOBALWEIGHTS->unprotectMemory(chip);
#pragma omp atomic
        progress += progressHalfStepSize;
    }
    checkSuccessProcessing(scienceData);
    satisfyMaxMemorySetting();

    if (successProcessing) {
        emit progressUpdate(100);
        //        pushEndMessage(taskBasename, scienceDir);
    }
}

void Controller::taskInternalSeparate()
{
    // TODO:
    // Shouldn't there be a successprocessing step at the beginning of every task?

    QString scienceDir = instructions.split(" ").at(1);
    Data *scienceData = getData(DT_SCIENCE, scienceDir);
    if (scienceData == nullptr) return;      // Error triggered by getData();

    currentData = scienceData;
    currentDirName = scienceDir;

    pushBeginMessage(taskBasename, scienceDir);

    memoryDecideDeletableStatus(scienceData, false);

    getNumberOfActiveImages(scienceData);

    QString tolerance = cdw->ui->separateTargetLineEdit->text();
    int numGroups = scienceData->identifyClusters(tolerance);
    if (numGroups == 0) return;

    if (verbosity >= 0) emit messageAvailable("Moving images ...", "controller");

    QStringList newSubDirNames;
    QString dataSubDirName = scienceData->subDirName;
    QString statusString = scienceData->processingStatus->statusString;

    // Delete previously created directories, then add files in another loop
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        for (auto &it : scienceData->myImageList[chip]) {
            QString subDirName = dataSubDirName+"_"+QString::number(it->groupNumber);
            QString pathNew = mainDirName+"/"+subDirName;
            QDir groupDir(pathNew);
            if (groupDir.exists()) groupDir.removeRecursively();
        }
    }

#pragma omp parallel for num_threads(maxExternalThreads) firstprivate(dataSubDirName, mainDirName, statusString)
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        for (auto &it : scienceData->myImageList[chip]) {
            if (!it->successProcessing) continue;
            QString pathOld = it->path;
            QString subDirName = dataSubDirName+"_"+QString::number(it->groupNumber);
#pragma omp critical
            {
                if (!newSubDirNames.contains(subDirName)) newSubDirNames.append(subDirName);
            }
            QString pathNew = mainDirName+"/"+subDirName;
            QDir groupDir(pathNew);
            groupDir.mkpath(pathNew);
            it->path = pathNew;
            it->provideHeaderInfo();
            // Always write the new image, even if until now it was kept in memory.
            if (it->imageOnDrive) {
                moveFile(it->chipName+statusString+".fits", pathOld, it->path);
            }
            else {
                it->writeImage();
                it->unprotectMemory();
            }
#pragma omp atomic
            progress += progressStepSize;
        }
    }
    checkSuccessProcessing(scienceData);
    satisfyMaxMemorySetting();

    // Now we have to create new 'Data' instances for each image association
    emit messageAvailable("Setting up "+QString::number(numGroups+1)+" image associations ...", "controller");

    // First, update the line edit
    // In case several dirs are entered, we must replace the current one, only
    QString newDirs = newSubDirNames.join(" "); // New dirs forming one blank-separated string
    QStringList dirList = mainGUI->ui->setupScienceLineEdit->text().split(" ");
    for (auto &dir : dirList) {
        if (dir == scienceData->subDirName) {
            // replace old dir with new dirs
            dir = newDirs;
        }
    }
    QString allDirs = dirList.join(" ");
    scienceData->myImageList.clear();

    // We run in a processing thread inside the controller. To change anything "outside" we need signals and slots
    // Trigger an update of the science dir LineEdit. The MainWindow will then trigger the controller to parse the new data dirs.
    // For that we need to have access to the status (of which the controller knows nothing, only the Data class)
    statusAtDistribute = scienceData->processingStatus->statusString;
    emit scienceDataDirUpdated(allDirs);

    /*
     if (successProcessing) {
        emit progressUpdate(100);
//        pushEndMessage(taskBasename, scienceDir);
    }
    */
}
