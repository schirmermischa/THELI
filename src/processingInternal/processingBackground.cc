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
#include "../tools/fitting.h"
#include "ui_confdockwidget.h"
#include "ui_monitor.h"
#include "../query/query.h"

#include <QMetaObject>
#include <QVector>
#include <QStringList>
#include <QProgressBar>

void Controller::taskInternalBackground()
{
    QString scienceDir = instructions.split(" ").at(1);
    QString skyDir = instructions.split(" ").at(2);
    Data *scienceData = getData(DT_SCIENCE, scienceDir);
    if (scienceData == nullptr) return;      // Error triggered by getData();
    if (!testResetDesire(scienceData)) return;

    currentData = scienceData;
    currentDirName = scienceDir;

    pushBeginMessage(taskBasename, scienceDir);
    pushConfigBackground();

    // Need to fill myImageList to get Filter keyword (if the user starts fresh with this task after launching THELI)
    if (scienceData->myImageList[instData->validChip].isEmpty()) scienceData->populate(scienceData->processingStatus->statusString);
    if (!scienceData->hasImages()) return;
    if (!scienceData->collectMJD()) return;    // Leave if identical MJD entries are found (or no MJD entries at all)
    scienceData->resetProcessbackground();
    scienceData->resetObjectMasking();

    Data *skyData = nullptr;
    if (skyDir == "noskydir") skyData = scienceData;  // The background is calculated either from science or from sky images
    else {
        skyData = getData(DT_SKY, skyDir);
        if (skyData == nullptr) return;      // Error triggered by getData();
        if (!skyData->hasImages()) return;
        if (!skyData->collectMJD()) return;
        skyData->resetProcessbackground();
        skyData->resetObjectMasking();
    }
    skyData->rescaleFlag = true;   // Background images must be rescaled before combination into the final model

    memoryDecideDeletableStatus(scienceData, false);

    backupDirName = scienceData->processingStatus->getStatusString() + "_IMAGES";

    // Static or dynamic mode ?
    scienceData->resetStaticModel();
    skyData->resetStaticModel();
    QString window = cdw->ui->BACwindowLineEdit->text();
    QString mode = "dynamic";
    if (window.isEmpty() || window.toInt() == 0) {
        mode = "static";
    }

    scienceData->checkModeIsPresent();
    skyData->checkModeIsPresent();

    // Flag images with bright stars, leave if definitely too few images left
    QList<QVector<double>> brightStarList;
    retrieveBrightStars(skyData, brightStarList);
    if (!idChipsWithBrightStars(skyData, brightStarList)) return;

    bool success = scienceData->checkTaskRepeatStatus(taskBasename);
    if (!success) return;

    getNumberOfActiveImages(scienceData);
    QVector<QString> numBackExpList(instData->numChips);

    float windowsize;
    if (window.isEmpty() || window == "0") windowsize = scienceData->myImageList[instData->validChip].length();
    else windowsize = window.toInt();
    float nimg = 7 + windowsize;  // image, combined image, new image, background, measure, segment, mask + window data; modify for SKY images?
    releaseMemory(nimg*instData->storage*maxExternalThreads, 1);
    // Protect the rest, will be unprotected as needed
    scienceData->protectMemory();
    skyData->protectMemory();

    doDataFitInRAM(scienceData->myImageList[instData->validChip].length()*instData->numUsedChips, instData->storage);

    QString dt = cdw->ui->BACDTLineEdit->text();
    QString dmin = cdw->ui->BACDMINLineEdit->text();
    QString expFactor = cdw->ui->BACmefLineEdit->text();
    QString nlow1 = cdw->ui->BAC1nlowLineEdit->text();
    QString nhigh1 = cdw->ui->BAC1nhighLineEdit->text();
    QString nlow2 = cdw->ui->BAC2nlowLineEdit->text();
    QString nhigh2 = cdw->ui->BAC2nhighLineEdit->text();
    bool twoPass = cdw->ui->BAC2passCheckBox->isChecked();
    bool convolution = cdw->ui->BACconvolutionCheckBox->isChecked();
    bool rescaleModel = cdw->ui->BACrescaleCheckBox->isChecked();
    int nGroups = cdw->ui->SPSnumbergroupsLineEdit->text().toInt();
    int nLength = cdw->ui->SPSlengthLineEdit->text().toInt();

    QVector<bool> staticImagesWritten(instData->numChips);
    for (auto &it : staticImagesWritten) it = false;

    // ****************************************
    // OLD PARALLELIZATION SCHEME (good if numCPU <= numChips)
    // ****************************************
    processBackground(scienceData, skyData, nimg, numBackExpList, dt, dmin, expFactor, nlow1, nhigh1,
                      nlow2, nhigh2, twoPass, convolution, rescaleModel, nGroups, nLength, mode, staticImagesWritten);

    // ****************************************
    // NEW PARALLELIZATION SCHEME (good if numCPU > numChips);    still not thread-safe
    // ****************************************
    /*
    if (mode == "static") {
        processBackgroundStatic(scienceData, skyData, nimg, numBackExpList, dt, dmin, expFactor, nlow1, nhigh1,
                                nlow2, nhigh2, twoPass, convolution, rescaleModel, nGroups, nLength, staticImagesWritten);
    }
    else {
        processBackgroundDynamic(scienceData, skyData, nimg, numBackExpList, dt, dmin, expFactor, nlow1, nhigh1,
                                 nlow2, nhigh2, twoPass, convolution, rescaleModel, nGroups, nLength, staticImagesWritten);
    }
    */

    if (!successProcessing) return;

    emit messageAvailable("<br>", "output");
    emit messageAvailable("Number of images used in the background models:<br>", "controller");
    for (auto &str : numBackExpList) {
        emit messageAvailable(str, "ignore");
    }
    checkSuccessProcessing(scienceData);
    satisfyMaxMemorySetting();

    // Clean-up, otherwise interference with any source detection / masking task
    scienceData->cleanBackgroundModelStatus();
    skyData->cleanBackgroundModelStatus();

    if (successProcessing) {
        scienceData->processingStatus->Background = true;
        scienceData->processingStatus->writeToDrive();
        scienceData->transferBackupInfo();
        scienceData->emitStatusChanged();
        emit addBackupDirToMemoryviewer(scienceDir);
        emit progressUpdate(100);
        emit refreshMemoryViewer();      // Update TableView header
    }
}

//void Controller::processBackground(Data *scienceData, Data *skyData, const float nimg, QVector<QString> &numBackExpList,
//                                   const QString dt, const QString dmin, const QString expFactor, const QString nlow1,
//                                   const QString nhigh1, const QString nlow2, const QString nhigh2,
//                                   const bool twoPass, const bool convolution, const bool rescaleModel,
//                                   const int nGroups, const int nLength, const QString mode, QVector<bool> &staticImagesWritten)
void Controller::processBackground(Data *scienceData, Data *skyData, const float nimg, QVector<QString> &numBackExpList,
                                   QString dt, QString dmin, QString expFactor, QString nlow1,
                                   QString nhigh1, QString nlow2, QString nhigh2,
                                   const bool twoPass, const bool convolution, const bool rescaleModel,
                                   const int nGroups, const int nLength, QString mode, QVector<bool> &staticImagesWritten)
{
    QString dataDirName = scienceData->dirName;
    QString dataSubDirName = scienceData->subDirName;
    QVector<bool> dataStaticModelDone = skyData->staticModelDone;
#pragma omp parallel for num_threads(maxExternalThreads) firstprivate(dt, dmin, expFactor, nlow1, nhigh1, nlow2, nhigh2, mode, dataDirName, dataSubDirName, dataStaticModelDone)
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (abortProcess || !successProcessing || instData->badChips.contains(chip)) continue;
        int currentExposure = 0;   // only relevant for LIRIS@WHT-type detectors where we need to select specific images
        QString backExpList = "";
        bool pass2staticDone = false;
        for (auto &it : scienceData->myImageList[chip]) {
            if (abortProcess) break;
            if (!it->successProcessing) continue;
            releaseMemory(nimg*instData->storage, maxExternalThreads);
            if (verbosity >= 0) emit messageAvailable(it->chipName + " : Modeling background ...", "image");

            it->processingStatus->Background = false;
            it->setupBackgroundData(scienceData->isTaskRepeated, backupDirName);     // Put original flat-fielded data in dataBackupL1;

            // The list of background images.
            // Each image has a flag whether it contributes to the model. Initially, all are set to 'false'
            if (!setupBackgroundList(chip, skyData, it->chipName)) continue;          // cannot use break in OMP loop

            if (!filterBackgroundList(chip, skyData, it, backExpList, nGroups, nLength, currentExposure, mode)) {
                continue;   // cannot use break in OMP loop
            }

            // PASS 1:
            sendBackgroundMessage(mode, dataStaticModelDone[chip], it->chipName, 1);
            maskObjectsInSkyImagesPass1(chip, skyData, scienceData, twoPass, dt, dmin, convolution, expFactor);
            skyData->combineImages(chip, nlow1, nhigh1, it->chipName, mode, dataDirName, dataSubDirName, dataStaticModelDone);
            skyData->combinedImage[chip]->modeDetermined = false;   // must redetermine!
            skyData->getModeCombineImages(chip);

            // PASS 2:
            if (twoPass) {
                sendBackgroundMessage(mode, dataStaticModelDone[chip], it->chipName, 2);
                maskObjectsInSkyImagesPass2(chip, skyData, scienceData, twoPass, dt, dmin, convolution, expFactor, rescaleModel);
                if (mode == "static" && !pass2staticDone) dataStaticModelDone[chip] = false;    // must recalculate static model (dynamic model will always be recalculated)
                skyData->combineImages(chip, nlow2, nhigh2, it->chipName, mode, dataDirName, dataSubDirName, dataStaticModelDone);
                pass2staticDone = true;
                skyData->combinedImage[chip]->modeDetermined = false;   // must redetermine!
                skyData->getModeCombineImages(chip);
            }

            skyData->writeBackgroundModel(chip, mode, it->baseName, staticImagesWritten[chip]);
            if (mode == "static") dataStaticModelDone[chip] = true;
            it->applyBackgroundModel(skyData->combinedImage[chip], cdw->ui->BACapplyComboBox->currentText(), rescaleModel);

            updateImageAndData(it, scienceData);

            if (alwaysStoreData) {
                it->writeImage();
                // DO NOT UNPROTECT MEMORY HERE (could be needed elsewhere)
            }

//            if (mode == "dynamic") it->releaseMemoryForBackground();
            if (mode == "dynamic") scienceData->unprotectMemoryForBackground(chip);

#pragma omp atomic
            progress += progressStepSize;
            ++currentExposure;
        }

        // In critical section because length of QString backexpList is variable, and Qvector<QString> is probably not thread safe because of this
#pragma omp critical
        {
            numBackExpList[chip] = backExpList;
        }
        // L1 always contains the data before any modification, hence we do not need to create a backup copy.
        if (scienceData->successProcessing) {
            for (auto &it : scienceData->myImageList[chip]) {
                it->makeBackgroundBackup();      // just a FITS file operation if necessary
            }
            scienceData->unprotectMemory(chip);
            skyData->unprotectMemory(chip);
        }
    }
}

/*
//void Controller::processBackgroundStatic(Data *scienceData, Data *skyData, const float &nimg, QVector<QString> &numBackExpList,
//                                         const QString &dt, const QString &dmin, const QString &expFactor, const QString &nlow1,
//                                         const QString &nhigh1, const QString &nlow2, const QString &nhigh2,
//                                         const bool &twoPass, const bool &convolution, const bool &rescaleModel,
//                                         const int &nGroups, const int &nLength, QVector<bool> &staticImagesWritten)
void Controller::processBackgroundStatic(Data *scienceData, Data *skyData, const float nimg, QVector<QString> &numBackExpList,
                                         QString dt,  QString dmin,  QString expFactor,  QString nlow1,
                                         QString nhigh1,  QString nlow2,  QString nhigh2,
                                         const bool twoPass, const bool convolution, const bool rescaleModel,
                                         const int nGroups, const int nLength, QVector<bool> &staticImagesWritten)
{
    // The following is needed for the new scheme
    QVector<QString> backExpList(maxCPU);
    QVector<int> currentExposure(instData->numChips);   // relevant only for LIRIS@WHT-type detectors
    QVector<bool> staticImagesCombined(instData->numChips);
    QVector<MyImage*> combinedBackgroundImages(maxCPU);
    for (auto &it : backExpList) it = "";
    for (auto &it : backExpListRescaleFactors) it = "";
    for (auto &it : currentExposure) it = 0;
    for (auto &it : staticImagesCombined) it = false;
    for (auto &it : combinedBackgroundImages) it = nullptr;

    QList<MyImage*> allMyImages;
    long numMyImages = makeListofAllImages(allMyImages, scienceData);

    QString dataSubDirName = scienceData->subDirName;

#pragma omp parallel for num_threads(maxCPU) firstprivate(dt, dmin, expFactor, nlow1, nhigh1, nlow2, nhigh2, dataSubDirName)
    for (int k=0; k<numMyImages; ++k) {
        if (abortProcess || !successProcessing) continue;
        int threadID = omp_get_thread_num();
        auto &it = allMyImages[k];
        if (!it->successProcessing) continue;
        int chip = it->chipNumber - 1;
        if (instData->badChips.contains(chip)) continue;
        releaseMemory(nimg*instData->storage, maxExternalThreads);

        if (verbosity >= 0) emit messageAvailable(it->chipName + " : Modeling background ...", "image");

        it->processingStatus->Background = false;
        it->setupBackgroundData_newParallel(scienceData->isTaskRepeated, backupDirName);      // Put original flat-fielded data in dataBackupL1; internal lock

        // The list of background images. Each image has a flag whether it contributes to the model. No locking required
        QList<MyImage*> backgroundList;
        if (!filterBackgroundList(chip, skyData, it, backExpList[threadID], backgroundList, nGroups, nLength, currentExposure[chip], "static")) {
            continue;   // cannot use break in OMP loop
        }

        // PASS 1:
        sendBackgroundMessage("static", skyData->staticModelDone[chip], it->chipName, 1);
        maskObjectsInSkyImagesPass1_newParallel(skyData, scienceData, backgroundList, twoPass, dt, dmin, convolution, expFactor, threadID);

        MyImage *masterCombined = new MyImage(dirName, "dummy.fits", "", chip+1, skyData->mask->globalMask[chip], skyData->mask->isChipMasked[chip], &verbosity);
        // do this only once per chip
        omp_set_lock(&backgroundLock);
        if (!staticImagesCombined[chip]) {
            skyData->combineImages_newParallel(chip, masterCombined, backgroundList, nlow1, nhigh1, it->chipName, "static", dataSubDirName);
            skyData->combinedImage[chip] = masterCombined;
            skyData->combinedImage[chip]->naxis1 = it->naxis1;
            skyData->combinedImage[chip]->naxis2 = it->naxis2;
            skyData->combinedImage[chip]->dataCurrent.swap(masterCombined->dataCurrent);
            skyData->getModeCombineImages(chip);
            staticImagesCombined[chip] = true;
        }
        omp_unset_lock(&backgroundLock);

        // PASS 2:
        if (twoPass) {
            sendBackgroundMessage("static", skyData->staticModelDone[chip], it->chipName, 2);
            maskObjectsInSkyImagesPass2_newParallel(skyData, scienceData, masterCombined, backgroundList, twoPass, dt, dmin,
                                                    convolution, expFactor, chip, rescaleModel, threadID, "static");
            // do this only once per chip
            omp_set_lock(&backgroundLock);
            // CHECK: must recalculate 2nd-pass static model (?)
            // also, in pass2_newparalell, must check for maskObjectsDonePass2
            if (!staticImagesCombined[chip]) {
                skyData->combineImages_newParallel(chip, skyData->combinedImage[chip], backgroundList, nlow2, nhigh2, it->chipName, "static", dataSubDirName);
                skyData->getModeCombineImages(chip);
            }
            omp_unset_lock(&backgroundLock);
        }
        skyData->writeBackgroundModel_newParallel(chip, skyData->combinedImage[chip], "static", it->baseName, threadID, backgroundLock, staticImagesWritten[chip]);
        skyData->staticModelDone[chip] = true;
        it->applyBackgroundModel(skyData->combinedImage[chip], cdw->ui->BACapplyComboBox->currentText(), rescaleModel, backExpListRescaleFactors);

        updateImageAndData(it, scienceData);

        if (alwaysStoreData) {
            it->writeImage();
            // DO NOT UNPROTECT MEMORY HERE (could be needed elsewhere)
        }

#pragma omp atomic
        progress += progressStepSize;
        ++currentExposure[chip];

        // In critical section because length of QString backexpList is variable, and Qvector<QString> is probably not thread safe because of this
#pragma omp critical
        {
            // Join the number of exposures with the rescale factors
            QVector<QString> joinedList(maxCPU);

            numBackExpList[chip] = backExpList[chip];
        }
        // L1 always contains the data before any modification, hence we do not need to create a backup copy.
        if (scienceData->successProcessing) {
            for (auto &it : scienceData->myImageList[chip]) {
                it->makeBackgroundBackup();      // just a FITS file operation if necessary
            }
            scienceData->unprotectMemory(chip);
            skyData->unprotectMemory(chip);
        }
    }
}

//void Controller::processBackgroundDynamic(Data *scienceData, Data *skyData, const float &nimg, QVector<QString> &numBackExpList,
//                                          const QString &dt, const QString &dmin, const QString &expFactor, const QString &nlow1,
//                                          const QString &nhigh1, const QString &nlow2, const QString &nhigh2,
//                                          const bool &twoPass, const bool &convolution, const bool &rescaleModel,
//                                          const int &nGroups, const int &nLength, QVector<bool> &staticImagesWritten)
void Controller::processBackgroundDynamic(Data *scienceData, Data *skyData, const float nimg, QVector<QString> &numBackExpList,
                                          QString dt,  QString dmin,  QString expFactor,  QString nlow1,
                                          QString nhigh1,  QString nlow2,  QString nhigh2,
                                          const bool twoPass, const bool convolution, const bool rescaleModel,
                                          const int nGroups, const int nLength, QVector<bool> &staticImagesWritten)
{
    // The following is needed for the new scheme
    QVector<QString> backExpList(maxCPU);
    QVector<int> currentExposure(instData->numChips);   // relevant only for LIRIS@WHT-type detectors
    QVector<bool> staticImagesCombined(instData->numChips);
    QVector<MyImage*> combinedBackgroundImages(maxCPU);
    for (auto &it : backExpList) it = "";
    for (auto &it : currentExposure) it = 0;
    for (auto &it : staticImagesCombined) it = false;
    for (auto &it : combinedBackgroundImages) it = nullptr;

    QList<MyImage*> allMyImages;
    long numMyImages = makeListofAllImages(allMyImages, scienceData);

    QString dataSubDirName = scienceData->subDirName;

#pragma omp parallel for num_threads(maxCPU
        backExpRescaleFactors.append("1.000; no rescaling when dividing model");) firstprivate(dt, dmin, expFactor, nlow1, nhigh1, nlow2, nhigh2, backExpList, allMyImages, dataSubDirName)
    for (int k=0; k<numMyImages; ++k) {
        if (abortProcess || !successProcessing) continue;
        int threadID = omp_get_thread_num();
        auto &it = allMyImages[k];
        if (!it->successProcessing) continue;
        int chip = it->chipNumber - 1;
        if (instData->badChips.contains(chip)) continue;
        releaseMemory(nimg*instData->storage, maxExternalThreads);

        if (verbosity >= 0) emit messageAvailable(it->chipName + " : Modeling background ...", "image");

        it->processingStatus->Background = false;
        it->setupBackgroundData_newParallel(scienceData->isTaskRepeated, backupDirName);      // Put original flat-fielded data in dataBackupL1; internal lock

        // The list of background images. Each image has a flag whether it contributes to the model. No locking required
        QList<MyImage*> backgroundList;
        if (!filterBackgroundList(chip, skyData, it, backExpList[threadID], backgroundList, nGroups, nLength, currentExposure[chip], "dynamic")) {
            continue;   // cannot use break in OMP loop
        }

        // PASS 1:
        sendBackgroundMessage("dynamic", skyData, it->chipName, 1);
        maskObjectsInSkyImagesPass1_newParallel(skyData, scienceData, backgroundList, twoPass, dt, dmin, convolution, expFactor, threadID);

        MyImage *masterCombined = new MyImage(dirName, "dummy.fits", "", chip+1, skyData->mask->globalMask[chip], skyData->mask->isChipMasked[chip], &verbosity);
        skyData->combineImages_newParallel(chip, masterCombined, backgroundList, nlow1, nhigh1, it->chipName, "dynamic", dataSubDirName);
        skyData->getModeCombineImagesBackground(chip, masterCombined);
        masterCombined->naxis1 = it->naxis1;
        masterCombined->naxis2 = it->naxis2;
        combinedBackgroundImages[threadID] = masterCombined;

        // PASS 2:
        if (twoPass) {
            sendBackgroundMessage("dynamic", skyData, it->chipName, 2);
            maskObjectsInSkyImagesPass2_newParallel(skyData, scienceData, masterCombined, backgroundList, twoPass, dt, dmin,
                                                    convolution, expFactor, chip, rescaleModel, threadID, "dynamic");
            skyData->combineImages_newParallel(chip, combinedBackgroundImages[threadID], backgroundList, nlow2, nhigh2, it->chipName, "dynamic", dataSubDirName);
            skyData->getModeCombineImagesBackground(chip, combinedBackgroundImages[threadID]);
        }
        skyData->writeBackgroundModel_newParallel(chip, combinedBackgroundImages[threadID], "dynamic", it->baseName, threadID, backgroundLock, staticImagesWritten[chip]);
        it->applyBackgroundModel(combinedBackgroundImages[threadID], cdw->ui->BACapplyComboBox->currentText(), rescaleModel);

        updateImageAndData(it, scienceData);

        if (alwaysStoreData) {
            it->writeImage();
            // DO NOT UNPROTECT MEMORY HERE (could be needed elsewhere)
        }

#pragma omp atomic
        progress += progressStepSize;
        ++currentExposure[chip];

        // In critical section because length of QString backexpList is variable, and Qvector<QString> is probably not thread safe because of this
#pragma omp critical
        {
            numBackExpList[chip] = backExpList[chip];
        }
        // L1 always contains the data before any modification, hence we do not need to create a backup copy.
        if (scienceData->successProcessing) {
            for (auto &it : scienceData->myImageList[chip]) {
                it->makeBackgroundBackup();
        backExpRescaleFactors.append("1.000; no rescaling when dividing model");      // just a FITS file operation if necessary
            }
            scienceData->unprotectMemory(chip);
            skyData->unprotectMemory(chip);
        }
    }
}
*/

void Controller::maskObjectsInSkyImagesPass1(const int chip, Data *skyData, Data *scienceData, const bool twoPass,
                                             const QString dt, const QString dmin, const bool convolution, const QString expFactor)
{
    // Loop over the list of valid background images and mask the objects
    bool doSourceDetection = false;
    QVector<QString> thresholds = getBackgroundThresholds(0, twoPass, dt, dmin, doSourceDetection);
    QString DT = thresholds[0];
    QString DMIN = thresholds[1];
    for (auto &back : skyData->myImageList[chip]) {
        if (!back->successProcessing) break;
        if (!back->useForBackground) continue;        // that should never be the case because the backgroundlist contains 'valid' images, only, at this point
        //        if (!back->useForBackground) {
        //            back->unprotectMemory();  // TODO: check if possibly dangerous. Can we do this here? Or elsewhere?
        //            continue;
        //        }
        // reads from dataBackupL1; if not then from disk and creates backup in L1, measures the mode if not yet available
        back->setupBackgroundData(isTaskRepeated, backupDirName);       // Already in memory if skyData == scienceData
        if (doSourceDetection && !twoPass && !back->objectMaskDonePass1) {   // only detect if requested and not yet done; objectMaskDone set to false in skydata outside loops
            if (verbosity >= 2) emit messageAvailable(back->chipName + " : Detecting and masking sources ...", "image");
            back->backgroundModel(256, "interpolate");
            back->segmentImage(DT, DMIN, convolution, false);
            back->transferObjectsToMask();
            back->maskExpand(expFactor, false);
            back->objectMaskDonePass1 = true;
        }
        if (!back->successProcessing) {
            skyData->successProcessing = false;
            scienceData->successProcessing = false;
        }
    }
}

void Controller::maskObjectsInSkyImagesPass2(const int chip, Data *skyData, Data *scienceData, const bool twoPass, const QString dt, const QString dmin,
                                             const bool convolution, const QString expFactor, const bool rescaleModel)
{
    bool doSourceDetection = false;
    QVector<QString> thresholds = getBackgroundThresholds(1, twoPass, dt, dmin, doSourceDetection);
    QString DT = thresholds[0];
    QString DMIN = thresholds[1];
    for (auto &back : skyData->myImageList[chip]) {
        if (!back->successProcessing) break;
        if (!back->useForBackground) continue;
        if (doSourceDetection && !back->objectMaskDonePass2) {
            if (verbosity >= 2) emit messageAvailable(back->chipName + " : Detecting and masking sources ...", "image");
            // Mask objects if not yet done for this sky (science) image
            // No masking has taken place in PASS 1 if we are in twopass mode!
            // Subtract 1st pass model: dataCurrent = dataBackupL1 - 1stPassModel
            back->applyBackgroundModel(skyData->combinedImage[chip], cdw->ui->BACapplyComboBox->currentText(), rescaleModel);
            //          back->backgroundModelDone = false;                  // IMPORTANT, otherwise background model step will be skipped
            back->backgroundModel(256, "interpolate");          // create background model
            back->segmentImage(DT, DMIN, convolution, false);   // detect sources (if requested), do not write seg image
            back->transferObjectsToMask();                      // sets objectMaskDone to true
            back->maskExpand(expFactor, false);                 // expand the object mask (if requested)
            back->objectMaskDonePass2 = true;
        }
        if (!back->successProcessing) {
            skyData->successProcessing = false;
            scienceData->successProcessing = false;
        }
    }
}

bool Controller::setupBackgroundList(int chip, Data *skyData, const QString &chipName)
{
    if (!successProcessing) return false;

    // First, collect all images of this chip, and reset their usability for background modeling to 'false' (or 'true' depending which ones is easier to code)
    for (auto &it : skyData->myImageList[chip]) {
        it->useForBackground = false;
        it->useForBackgroundSequence = true;
        it->useForBackgroundWindowed = false;
        it->useForBackgroundStars = true;
    }

    if (skyData->myImageList[chip].length() < 2) {
        emit messageAvailable(chipName + " : At least two images are required for background modeling.", "error");
        emit criticalReceived();
        successProcessing = false;
        return false;
    }

    return true;
}

bool Controller::filterBackgroundList(const int chip, Data *skyData, MyImage *it,  QString &backExpList, const int nGroups, const int nLength,
                                      const int currentExposure, const QString mode)
{
    if (!successProcessing) return false;
    if (!it->successProcessing) return false;

    selectImagesFromSequence(skyData->myImageList[chip], nGroups, nLength, currentExposure);                 // Update flag: Select every n-th image, only, if requested
//    for (auto &back : skyData->myImageList[chip]) qDebug() << back->baseName << back->useForBackgroundSequence;
//    qDebug() << "";

    if (mode == "dynamic") selectImagesDynamically(skyData->myImageList[chip], it->mjdobs);    // Update flag: dynamic or static mode
    else selectImagesStatically(skyData->myImageList[chip], it);                 // Already sets BADBACK flag if necessary

//    for (auto &back : skyData->myImageList[chip]) qDebug() << back->baseName << back->useForBackgroundSequence << back->useForBackgroundWindowed;
//    qDebug() << "";

    flagImagesWithBrightStars(skyData->myImageList[chip]);                       // Update flag: Exclude images affected by bright stars
    if (!it->successProcessing) return false;                        // Leave if not sufficiently many images found for background modeling

    // Combine the flags from all three tests
    combineAllBackgroundUsabilityFlags(skyData->myImageList[chip]);

//    for (auto &back : skyData->myImageList[chip]) qDebug() << back->baseName << back->useForBackground;
//    qDebug() << "";

    int nback = countBackgroundImages(skyData->myImageList[chip], it->chipName);
    QString outstring = it->chipName + " : " + QString::number(nback) + "<br>";
    if (nback < 4) backExpList.append("<font color=#ee5500>" + outstring + "</font>");   // color coding to highlight potentially poor images
    else backExpList.append(outstring);

    if (!successProcessing || nback < 2 || !it->successProcessing) {
        emit messageAvailable(it->chipName + " : No (or not sufficiently many) suitable background images found", "warning");
        it->activeState = MyImage::BADBACK;
        it->successProcessing = false;
        return false;
    }

    return true;
}

/*
void Controller::maskObjectsInSkyImagesPass1_newParallel(Data *skyData, Data *scienceData, const QList<MyImage*> &backgroundList, const bool twoPass,
                                                         const QString dt, const QString dmin, const bool convolution, const QString expFactor,
                                                         const int threadID)
{
    // Loop over the list of valid background images and calculate the model
    bool doSourceDetection = false;
    QVector<QString> thresholds = getBackgroundThresholds(0, twoPass, dt, dmin, doSourceDetection);
    QString DT = thresholds[0];
    QString DMIN = thresholds[1];
    for (auto &back : backgroundList) {
        if (!back->successProcessing) break;
        //        if (!back->useForBackground) {
        //            back->unprotectMemory();  // TODO: check if possibly dangerous. Can we do this here? Or elsewhere?
        //            continue;
        //        }
        // reads from dataBackupL1; if not then from disk and creates backup in L1, measures the mode if not yet available
        back->setupBackgroundData_newParallel(isTaskRepeated, backupDirName);      // Already in memory if skyData == scienceData
        if (doSourceDetection && !twoPass) {  // only detect if requested and not yet done previously; objectMaskDone set to false in skydata outside loops
            back->setObjectLock(true);
            if (!back->objectMaskDone) {
                back->backgroundModel(256, "interpolate");
                back->segmentImage(DT, DMIN, convolution, false);
                back->transferObjectsToMask();
                back->maskExpand(expFactor, false);
            }
            back->setObjectLock(false);
        }
        if (!back->successProcessing) {
            skyData->successProcessing = false;
            scienceData->successProcessing = false;
        }
    }
}

void Controller::maskObjectsInSkyImagesPass2_newParallel(Data *skyData, Data *scienceData, MyImage *combinedImage, const QList<MyImage*> &backgroundList,
                                                         const bool twoPass, const QString dt, const QString dmin, const bool convolution,
                                                         const QString expFactor, const int chip, const bool rescaleModel, const int threadID, const QString mode)
{
    bool doSourceDetection = false;
    QVector<QString> thresholds = getBackgroundThresholds(1, twoPass, dt, dmin, doSourceDetection);
    QString DT = thresholds[0];
    QString DMIN = thresholds[1];
    for (auto &back : backgroundList) {
        if (!back->successProcessing) break;
        if (!back->useForBackground) continue;
        if (doSourceDetection) {
            back->setObjectLock(true);

            // Mask objects if not yet done for this sky (science) image
            // No masking has taken place in PASS 1 if we are in twopass mode!
            // Subtract 1st pass model: dataCurrent = dataBackupL1 - 1stPassModel
            if (!back->objectMaskDone) {
                if (mode == "dynamic") back->applyBackgroundModel(combinedImage, cdw->ui->BACapplyComboBox->currentText(), rescaleModel);
                else back->applyBackgroundModel(skyData->combinedImage[chip], cdw->ui->BACapplyComboBox->currentText(), rescaleModel);
                //              back->backgroundModelDone = false;                  // IMPORTANT, otherwise background model step will be skipped
                back->backgroundModel(256, "interpolate");          // create background model
                back->segmentImage(DT, DMIN, convolution, false);   // detect sources (if requested), do not write seg image
                back->transferObjectsToMask();                      // sets objectMaskDone to true
                back->maskExpand(expFactor, false);                 // expand the object mask (if requested)
            }
            back->setObjectLock(false);
        }
        if (!back->successProcessing) {
            skyData->successProcessing = false;
            scienceData->successProcessing = false;
        }
    }
}
*/

void Controller::sendBackgroundMessage(const QString mode, const bool staticmodeldone, const QString basename, const int pass)
{
    if (verbosity >= 0) {
        if ( (mode == "static" && !staticmodeldone)
             || mode == "dynamic") {
            if (pass == 1) emit messageAvailable(basename + " : Image combination 1st pass ...", "image");
            if (pass == 2) emit messageAvailable(basename + " : Image combination 2nd pass ...", "image");
        }
    }
}

// Select the 'windowSize' images that are closest in time to the targetMJD
void Controller::selectImagesDynamically(const QList<MyImage*> &backgroundList, const double &mjd_ref)
{
    if (!successProcessing) return;

    if (verbosity == 3) emit messageAvailable("Entering dynamic image selection ...", "image");

    QList<QPair<MyImage*, double>> imageListAbs;   // sorted with respect to fabs(mjd_diff)
    QList<QPair<MyImage*, double>> imageListDiff;  // sorted with respect to mjd_diff
    // Reset, and map data onto a list
    for (auto &it : backgroundList) {
        //        qDebug() << qSetRealNumberPrecision(12) << it->baseName << it->mjdobs;
        imageListAbs.append(qMakePair(it,fabs(it->mjdobs-mjd_ref)));
        imageListDiff.append(qMakePair(it,it->mjdobs-mjd_ref));
    }

    // Sort with respect to mjd difference
    std::sort(imageListAbs.begin(), imageListAbs.end(), QPairSecondComparer());
    std::sort(imageListDiff.begin(), imageListDiff.end(), QPairSecondComparer());

    // Mark the first 'windowSize' images, using fabs(mjd_diff), provided they passed the 'spread sequence' test
    int selected = 0;
    int windowSize = cdw->ui->BACwindowLineEdit->text().toInt();

    if (windowSize > backgroundList.length()) {
        QString part1 = QString::number(windowSize);
        QString part2 = QString::number(backgroundList.length());
        emit showMessageBox("Controller::WINDOWSIZE_TOO_LARGE", part1, part2);
        successProcessing = false;
        return;
    }

    // Here's the selection function
    for (auto &it : imageListAbs) {
        // Do not use the current image to contribute to its own background model (it->useForBackgroundWindowed remains 'false')
        if (it.second > 0.) {
            if (selected < windowSize && it.first->activeState == MyImage::ACTIVE && it.first->useForBackgroundSequence) {
                it.first->useForBackgroundWindowed = true;
                it.first->enteredBackgroundWindow = true;
                ++selected;
            }
            else {
                it.first->useForBackgroundWindowed = false;       // redundant. Set to false by default when entering this function
                // free RAM
                if (it.first->enteredBackgroundWindow && !it.first->leftBackgroundWindow) {
                    it.first->leftBackgroundWindow = true;
                    it.first->releaseBackgroundMemoryBackgroundModel();
                    it.first->releaseAllDetectionMemory();
                    //                    if (minimizeMemoryUsage) {
                    //                        it.first->freeAll();
                    //                    }
                }
            }
        }
    }

    // Check if there is a gap larger than the max gap size in the window
    QString maxGapString = cdw->ui->BACgapsizeLineEdit->text();
    if (!maxGapString.isEmpty()) {
        int count = 0;
        double mjd_previous = 0.;
        bool gapViolated = false;
        double currentGap = 0.;
        double maxGap = maxGapString.toDouble() / 24.;  // convert from hours to days (MJD)
        for (auto &it : imageListDiff) {
            if (it.first->useForBackgroundWindowed) {
                if (count == 0) {
                    mjd_previous = it.first->mjdobs;
                    ++count;
                }
                else {
                    currentGap = it.first->mjdobs - mjd_previous;
                    mjd_previous = it.first->mjdobs;
                    ++count;
                    if (currentGap > maxGap) {
                        gapViolated = true;
                        break;
                    }
                }
            }
        }
        if (gapViolated) {
            QString part1 = QString::number(currentGap*24,'f',3);
            emit showMessageBox("Controller::GAP_DYNAMIC_FOUND", part1, maxGapString);
            successProcessing = false;
        }
    }

    QString minWindowSizeString = cdw->ui->BACminWindowLineEdit->text();
    int minWindowSize;
    if (minWindowSizeString.isEmpty()) minWindowSize = windowSize;
    else minWindowSize = minWindowSizeString.toInt();
    if (selected < minWindowSize) {
        QString part1 = QString::number(selected);
        QString part2 = QString::number(minWindowSize);
        emit showMessageBox("Controller::INSUFFICIENT_BACKGROUND_NUMBER", part1, part2);
        successProcessing = false;
    }
}

// Select the images that are closest in time to the targetMJD and within a valid block defined by gap sizes
void Controller::selectImagesStatically(const QList<MyImage*> &backgroundList, MyImage *scienceImage)
{
    if (!successProcessing) return;
    if (!scienceImage->successProcessing) return;

    double mjd_ref = scienceImage->mjdobs;

    if (verbosity == 3) emit messageAvailable("Entering static image selection ...", "image");

    QList<QPair<MyImage*, double>> imageListDiff;  // sorted with respect to mjd_diff
    // Reset, and map data onto a list
    for (auto &it : backgroundList) {
        //        it->useForBackground = false;
        imageListDiff.append(qMakePair(it, it->mjdobs - mjd_ref));
    }

    // Sort with respect to mjd difference
    std::sort(imageListDiff.begin(), imageListDiff.end(), QPairSecondComparer());

    // Identify blocks (if a gap was defined)
    // Use a giant maxGap (longer than an observer's lifetime) if no gap was defined
    QString maxGapString = cdw->ui->BACgapsizeLineEdit->text();
    double maxGap = 1e9;
    if (!maxGapString.isEmpty()) maxGap = maxGapString.toDouble() / 24.;

    int blockCount = 0;
    double mjd_previous = imageListDiff[0].first->mjdobs;
    for (auto &it : imageListDiff) {
        double currentGap = it.first->mjdobs - mjd_previous;
        if (currentGap > maxGap) ++blockCount;
        it.first->backgroundBlock = blockCount;
        mjd_previous = it.first->mjdobs;
    }

    // Decide which block is the best (if any) to correct the image with mjd_ref
    int scienceBlockId = -1;

    int nSky = imageListDiff.length();
    for (int i=0; i<nSky; ++i) {
        double mjd_obs1;
        double mjd_obs2;
        if (i<nSky-1) {
            mjd_obs1 = imageListDiff[i].first->mjdobs;
            mjd_obs2 = imageListDiff[i+1].first->mjdobs;
        }
        else {
            mjd_obs1 = imageListDiff[i].first->mjdobs;
            mjd_obs2 = imageListDiff[i-1].first->mjdobs;
        }
        double d1 = fabs(mjd_ref - mjd_obs1);
        double d2 = fabs(mjd_ref - mjd_obs2);
        int blockID1;
        int blockID2;
        if (i<nSky-1) {
            blockID1 = imageListDiff[i].first->backgroundBlock;
            blockID2 = imageListDiff[i+1].first->backgroundBlock;
        }
        else {
            blockID1 = imageListDiff[i].first->backgroundBlock;
            blockID2 = imageListDiff[i-1].first->backgroundBlock;
        }
        // Cases where a science exposure has a valid sky block
        if (i==0 && mjd_ref < mjd_obs1 && d1 < maxGap) scienceBlockId = blockID1;       // image before first sky exposure
        if (i==nSky-1 && mjd_ref > mjd_obs1 && d1 < maxGap) scienceBlockId = blockID1;  // image after last sky exposure
        if (mjd_ref == mjd_obs1) scienceBlockId = blockID1;                             // image identical to sky image
        if (scienceBlockId != -1) break;                                                // Leave if matching block was found
        if (i==nSky-1) break;                                                           // stay within bounds for mjd_obs2, d2, and blockID2

        // Image must have been taken within the sky sequence

        // science image between two sky exposures
        //        if (mjd_ref > mjd_obs1 && mjd_ref < mjd_obs2) {       // excluding the science image from the model
        if (mjd_ref >= mjd_obs1 && mjd_ref <= mjd_obs2) {               // including the science image from the model
            // within a block or between blocks
            if (blockID1 == blockID2) scienceBlockId = blockID1;
            else {
                // Assign the closer block, if within maxGap
                if (d1 <= d2 && d1 < maxGap) scienceBlockId = blockID1;
                if (d2 < d1 && d2 < maxGap) scienceBlockId = blockID2;
            }
        }
        if (scienceBlockId != -1) break;
    }

    // Set the flag for all sky exposures that have the same block ID as the science exposure, and passed the 'spread sequence' test
    // Remember, imageListDiff just contains pointers into backgroundList, so we
    // are indeed updating the flags in the backgroundList
    int countSky = 0;  // the number of sky images used for correction
    for (auto &it : imageListDiff) {
        if (it.first->backgroundBlock == scienceBlockId && it.first->activeState == MyImage::ACTIVE && it.first->useForBackgroundSequence) {
            it.first->useForBackgroundWindowed = true;
            ++countSky;
        }
    }

    // Always use all images for the static model: comment out the following
    /*
    // Do not use the current image to contribute to its own background model
    for (auto &it : imageListDiff) {
        if (it.second == 0.) {
            it.first->useForBackgroundWindowed = false;
            --countSky;
        }
    }
    */

    if (scienceBlockId == -1) {
        emit messageAvailable(scienceImage->chipName + " : Could not identify suitable sky images. Image deactivated.", "warning");
        emit warningReceived();
        scienceImage->activeState = MyImage::BADBACK;
        scienceImage->successProcessing = false;
        if (scienceImage->imageOnDrive) {
            moveFile(scienceImage->baseName+".fits", scienceImage->path, scienceImage->path+"/inactive/badBackground/");
            scienceImage->path = scienceImage->path+"/inactive/badBackground/";
            scienceImage->emitModelUpdateNeeded();
        }
        return;
    }

    if (countSky < 3) {
        emit messageAvailable(scienceImage->chipName + " : Less than three images found for background modeling. Image deactivated.", "warning");
        emit warningReceived();
        scienceImage->activeState = MyImage::BADBACK;
        scienceImage->successProcessing = false;
        if (scienceImage->imageOnDrive) {
            moveFile(scienceImage->baseName+".fits", scienceImage->path, scienceImage->path+"/inactive/badBackground/");
            scienceImage->path = scienceImage->path+"/inactive/badBackground/";
            scienceImage->emitModelUpdateNeeded();
        }
        return;
    }

    successProcessing = true;
}

// If several exposures were taken at the same dither position before an offset,
// then sometimes the first and perhaps 2nd image must be corrected separately
// from all other first or second exposures.
void Controller::selectImagesFromSequence(QList<MyImage*> &backgroundList, const int &nGroups, const int &nLength, const int &currentExp)
{
    if (!successProcessing) return;

    // Nothing to be done
    if (nGroups == 0 || nLength == 0) return;        // returning with all flags set to 'true', i.e. no contraints

    // set all flags to false for a start
    for (auto &it : backgroundList) {
        it->useForBackgroundSequence = false;
    }

    // To which group does the current image belong
    int groupReference = currentExp % nLength;
    if (groupReference >= nGroups) groupReference = nGroups - 1;

    // Assign group numbers to background images
    int group = 0;
    int count = 0;
    for (auto &it : backgroundList) {
        if (group == groupReference) {
            it->useForBackgroundSequence = true;       // image usable for background modeling
        }
        //        qDebug() << it->chipName << group << count << groupReference << it->useForBackground;
//        qDebug() << "AAA" << it->chipName << group << count << groupReference << it->useForBackgroundSequence << currentExp;
        if (group < nGroups-1) ++group;
        ++count;
        if (count == nLength) {
            count = 0;
            group = 0;
        }
    }

    // Keep only background images that belong to the same group as the science exposure
    /*
    QList<MyImage*>::iterator it = backgroundList.begin();
    while (it != backgroundList.end()) {
        if (! (*it)->useForBackground)
            it = backgroundList.erase(it);
        else
            ++it;
    }
    */

    /*
    for (auto &it : backgroundList) {
        qDebug() << it->chipName;
    }
    qDebug() << " " ;
    */
}

// Obtain a list of bright stars from the UCAC catalog.
// This is done before we enter the big loops that process the images (because we want to retrieve it only once)
void Controller::retrieveBrightStars(Data *skyData, QList<QVector<double>> &brightStarList)
{
    // Leave if not requested
    QString brightMag = cdw->ui->BACmagLineEdit->text();
    if (brightMag.isEmpty()) return;

    QString mag = "";
    QString refcat = "";
    if (instData->type == "OPT") {
        refcat = "UCAC5";
        mag = "Rmag";
    }
    else {
        refcat = "2MASS";
        mag = "Hmag";
    }

    emit messageAvailable("Retrieving stars brighter than "+mag+" = "+brightMag+" from "+refcat+" ...", "controller");

    skyData->getPointingCharacteristics();

    Query *query = new Query(&verbosity);
//    connect(query, &Query::bulkMotionObtained, cdw, &ConfDockWidget::updateGaiaBulkMotion);
    connect(query, &Query::messageAvailable, mainGUI, &MainWindow::processMessage);
    connect(query, &Query::messageAvailable, monitor, &Monitor::displayMessage);
    query->scienceData = skyData;
    query->mainDirName = mainDirName;
    query->refcatName = refcat;
    query->alpha_manual = QString::number(skyData->RAcenter, 'f', 4);   // the query uses QStrings because we execute vizquery.py
    query->delta_manual = QString::number(skyData->DECcenter, 'f', 4);
    query->radius_manual = QString::number(skyData->searchRadius, 'f', 2);
    query->magLimit_string = cdw->ui->BACmagLineEdit->text();
    query->maxProperMotion_string = cdw->ui->ARCmaxpmLineEdit->text();
    query->doBrightStarQueryFromWeb();

    brightStarList.reserve(query->numSources);
    for (long i=0; i<query->numSources; ++i) {
        QVector<double> star;
        star << query->ra_out[i];
        star << query->de_out[i];
        star << query->mag1_out[i];
        brightStarList << star;
    }

    QString numstars = QString::number(query->numSources) + " bright stars found ";
    QString pointing = "within r = "+query->radius_manual+"'"+" of RA = "+query->alpha_manual +", DEC = "+query->delta_manual;
    emit messageAvailable(numstars+pointing, "controller");

    delete query;
    query = nullptr;
}

bool Controller::idChipsWithBrightStars(Data *skyData, QList<QVector<double>> &brightStarList)
{
    QString safetyDistanceString = cdw->ui->BACdistLineEdit->text();
    QString magLimit = cdw->ui->BACmagLineEdit->text();
    if (magLimit.isEmpty()) return true;

    float safetyDistance = safetyDistanceString.toFloat();

    int numImagesAffected = 0;
    QString imagesAffected = "";

    QVector<int> numChipsRejected(instData->numChips, 0);

    /*
    // TODO: when collapsing loops, make sure each chip has the same number of exposures
#pragma omp parallel for num_threads(maxExternalThreads)
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : skyData->myImageList[chip]) {
            it->loadHeader();
            it->checkBrightStars(brightStarList, safetyDistance, instData->pixscale);
            if (it->hasBrightStars) {
                ++numImagesAffected;
                ++numChipsRejected[chip];
                imagesAffected.append(it->baseName + "<br>");
            }
        }
    }
    */

    QList<MyImage*> allMyImages;
    long numMyImages = makeListofAllImages(allMyImages, skyData);

#pragma omp parallel for num_threads(maxCPU)
    for (int k=0; k<numMyImages; ++k) {
        auto &it = allMyImages[k];
        int chip = it->chipNumber - 1;
        if (instData->badChips.contains(chip)) continue;
        it->loadHeader();
        it->checkBrightStars(brightStarList, safetyDistance, instData->pixscale);
        if (it->hasBrightStars) {
#pragma omp critical
            {
                ++numImagesAffected;
                ++numChipsRejected[chip];
                imagesAffected.append(it->chipName + "<br>");
            }
        }
    }

    if (numImagesAffected > 0) {
        emit messageAvailable("Excluding "+QString::number(numImagesAffected)
                              + " images from entering the background model due to bright stars:", "controller");
        emit messageAvailable(imagesAffected, "ignore");
    }

    bool critical = false;
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        int chipsAvailable = skyData->myImageList[chip].length() - numChipsRejected[chip];
        if (chipsAvailable == 0) {
            emit messageAvailable("Chip "+QString::number(chip+1)
                                  + " : No images available for background modeling after bright star filtering.", "error");
            critical = true;
        }
        else if (chipsAvailable == 1) {
            emit messageAvailable("Chip "+QString::number(chip+1)
                                  + " : Only 1 image available for background modeling after bright star filtering.", "error");
            critical = true;
        }
        else if (chipsAvailable <= 4) {
            emit messageAvailable("Chip "+QString::number(chip+1)
                                  + " : Only " + QString::number(chipsAvailable)
                                  + " images available for background modeling after bright star filtering,<br>"+
                                  "expecting poor performance. Ideally, at least 5 images should remain.", "warning");
        }
    }
    if (critical) {
        criticalReceived();
        successProcessing = false;
        return false;
    }
    else return true;
}

void Controller::flagImagesWithBrightStars(const QList<MyImage*> &backgroundList)
{
    for (auto &it : backgroundList) {
        if (it->hasBrightStars) it->useForBackgroundStars = false;
    }
}

// Set an image as usable for background correction only if everything holds
void Controller::combineAllBackgroundUsabilityFlags(const QList<MyImage*> &backgroundList)
{
    for (auto &it : backgroundList) {
        if (it->useForBackgroundSequence
                && it->useForBackgroundWindowed
                && it->useForBackgroundStars) {
            it->useForBackground = true;
        }
    }
}

int Controller::countBackgroundImages(QList<MyImage*> list, QString baseName)
{
    if (!successProcessing) return 0;

    int count = 0;
    for (auto &it : list) {
        if (it->useForBackground) ++count;
    }

    if (count < 2) {
        emit messageAvailable(baseName + " : Less than two images ("
                              + QString::number(count) + ") were found to create the background model.", "warning");
        warningReceived();
        successProcessing = false;
        return count;
    }
    if (count < 4) {
        emit messageAvailable(baseName + " : Only " + QString::number(count)
                              + " images contribute to the background model, expecting poor performance.", "warning");
        warningReceived();
        return count;
    }

    return count;
}
