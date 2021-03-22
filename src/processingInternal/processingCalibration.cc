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

#include <QMetaObject>
#include <QVector>
#include <QStringList>
#include <QProgressBar>


void Controller::taskInternalProcessbias()
{
    if (!successProcessing) return;

    QString biasDir = instructions.split(" ").at(1);
    Data *biasData = getData(DT_BIAS, biasDir);
    if (biasData == nullptr) return;      // Error triggered by getData();
    if (!testResetDesire(biasData)) return;

    QString min = cdw->ui->biasMinLineEdit->text();
    QString max = cdw->ui->biasMaxLineEdit->text();
    QString nlow = cdw->ui->biasNlowLineEdit->text();
    QString nhigh = cdw->ui->biasNhighLineEdit->text();
    currentData = biasData;
    currentDirName = biasDir;

    memoryDecideDeletableStatus(biasData, false);

    pushBeginMessage(taskBasename, biasDir);
    pushConfigProcessbias();

    // TODO: The following line is needed only as long as we are handling splitting of raw data by scripts.
    // The biasData imagelist is empty after splitting because the Data constructor only looks for *_chip.fits, not for raw files.
    if (biasData->myImageList[instData->validChip].isEmpty()) {
        biasData->populate("");
        emit populateMemoryView();
    }

    getNumberOfActiveImages(biasData);

    // Release as much memory as maximally necessary
    float nimg = biasData->myImageList[instData->validChip].length() + 1;  // The number of images one thread keeps in memory
    releaseMemory(nimg*instData->storage*maxExternalThreads, 1, "calibrator");
    // Protect the rest, will be unprotected as needed
    biasData->protectMemory();
    QString dataDirName = biasData->dirName;             // copies for thread safety
    QString dataSubDirName = biasData->subDirName;       // copies for thread safety
    QString dataDataType = biasData->dataType;

    doDataFitInRAM(nimg*instData->numUsedChips, instData->storage);

    // Loop over all chips
    // NOTE: QString is not threadsafe, must create copies for threads!
    // NOTE: a 'bad' chip will 'continue', but openMP waits until at least one of the other threads has finished

#pragma omp parallel for num_threads(maxExternalThreads) firstprivate(nlow, nhigh, min, max, dataDirName, dataSubDirName)
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (abortProcess || !successProcessing || instData->badChips.contains(chip)) continue;
        float nimg = biasData->myImageList[chip].length() + 1;  // The number of images we must keep in memory
        // Release memory cannot touch any dataCurrent read by MyImage::readImage, because we 'protected' it outside the loop.
        // Initially, this call might not do anything because everything is protected. On systems with less RAM than
        // a single exposure this might cause swapping. We test for this elsewhere (when loading images).
        releaseMemory(nimg*instData->storage, maxExternalThreads, "calibrator");

        for (auto &it : biasData->myImageList[chip]) {
            if (abortProcess) break;
            if (!it->successProcessing) continue;
            it->setupCalibDataInMemory(false, true, false);   // Read image (if not already in memory), do not create backup, do get mode
            it->checkCorrectMaskSize(instData);
            it->setModeFlag(min, max);                        // Flag the image if its mode is outside a user-provided acceptable range
#pragma omp atomic
            progress += progressHalfStepSize;
        }

        if (biasData->myImageList[chip].length() == 0) {
            biasData->successProcessing = false;
            successProcessing = false;
            emit messageAvailable("Could not find data for chip "+QString::number(chip+1)+ " in "+biasData->subDirName, "error");
            emit criticalReceived();
            continue;
        }
        biasData->combineImagesCalib(chip, combineBias_ptr, nlow, nhigh, dataDirName, dataSubDirName, dataDataType);  // Combine images
        biasData->getModeCombineImages(chip);
        biasData->writeCombinedImage(chip);
        biasData->unprotectMemory(chip);
        if (minimizeMemoryUsage) {
            for (auto &it : biasData->myImageList[chip]) {
                it->freeAll();
            }
        }
        biasData->combinedImage[chip]->emitModelUpdateNeeded();
        if (!biasData->successProcessing) successProcessing = false;
#pragma omp atomic
        progress += progressCombinedStepSize;
    }
    if (!successProcessing) {
        emit progressUpdate(100);
        return;
    }
    biasData->reportModeCombineImages();

    checkSuccessProcessing(biasData);
    satisfyMaxMemorySetting();
    if (successProcessing) {
        emit progressUpdate(100);
        //        pushEndMessage(taskBasename, biasDir);
    }
}

void Controller::taskInternalProcessdark()
{
    if (!successProcessing) return;

    QString darkDir = instructions.split(" ").at(1);
    Data *darkData = getData(DT_DARK, darkDir);
    if (darkData == nullptr) return;      // Error triggered by getData();
    if (!testResetDesire(darkData)) return;

    QString min = cdw->ui->darkMinLineEdit->text();
    QString max = cdw->ui->darkMaxLineEdit->text();
    QString nlow = cdw->ui->darkNlowLineEdit->text();
    QString nhigh = cdw->ui->darkNhighLineEdit->text();
    currentData = darkData;
    currentDirName = darkDir;

    memoryDecideDeletableStatus(darkData, false);

    pushBeginMessage(taskBasename, darkDir);
    pushConfigProcessdark();

    getNumberOfActiveImages(darkData);

    // TODO: The following line is needed only as long as we are handling splitting of raw data by scripts.
    if (darkData->myImageList[instData->validChip].isEmpty()) {
        darkData->populate("");
        emit populateMemoryView();
    }

    // Release as much memory as maximally necessary
    float nimg = darkData->myImageList[instData->validChip].length() + 1;  // The number of images one thread keeps in memory
    releaseMemory(nimg*instData->storage*maxExternalThreads, 1, "calibrator");
    // Protect the rest, will be unprotected as needed
    darkData->protectMemory();
    QString dataDirName = darkData->dirName;             // copies for thread safety
    QString dataSubDirName = darkData->subDirName;       // copies for thread safety
    QString dataDataType = darkData->dataType;

    doDataFitInRAM(nimg*instData->numUsedChips, instData->storage);

    // Loop over all chips
#pragma omp parallel for num_threads(maxExternalThreads) firstprivate(nlow, nhigh, min, max, dataDirName, dataSubDirName)
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (abortProcess || !successProcessing || instData->badChips.contains(chip)) continue;

        float nimg = darkData->myImageList[chip].length() + 1;  // The number of images we must keep in memory
        releaseMemory(nimg*instData->storage, maxExternalThreads, "calibrator");

        for (auto &it : darkData->myImageList[chip]) {
            if (abortProcess) break;
            if (!it->successProcessing) continue;
            it->setupCalibDataInMemory(false, true, false);
            it->checkCorrectMaskSize(instData);
            it->setModeFlag(min, max);
#pragma omp atomic
            progress += progressHalfStepSize;
        }
        if (darkData->myImageList[chip].length() == 0) {
            darkData->successProcessing = false;
            successProcessing = false;
            emit messageAvailable("Could not find data for chip "+QString::number(chip+1)+ " in "+darkData->subDirName, "error");
            emit criticalReceived();
            continue;
        }
        darkData->combineImagesCalib(chip, combineDark_ptr, nlow, nhigh, dataDirName, dataSubDirName, dataDataType);
        darkData->getModeCombineImages(chip);
        darkData->writeCombinedImage(chip);
        darkData->unprotectMemory(chip);
        if (minimizeMemoryUsage) {
            for (auto &it : darkData->myImageList[chip]) it->freeAll();
        }
        if (!darkData->successProcessing) successProcessing = false;
#pragma omp atomic
        progress += progressCombinedStepSize;
    }
    if (!successProcessing) {
        emit progressUpdate(100);
        return;
    }

    darkData->reportModeCombineImages();

    checkSuccessProcessing(darkData);
    satisfyMaxMemorySetting();
    if (successProcessing) {
        emit progressUpdate(100);
        //        pushEndMessage(taskBasename, darkDir);
    }
}

void Controller::taskInternalProcessflatoff()
{
    QString flatoffDir = instructions.split(" ").at(1);
    Data *flatoffData = getData(DT_FLATOFF, flatoffDir);
    if (flatoffData == nullptr) return;      // Error triggered by getData();
    if (!testResetDesire(flatoffData)) return;

    QString min = cdw->ui->flatoffMinLineEdit->text();
    QString max = cdw->ui->flatoffMaxLineEdit->text();
    QString nlow = cdw->ui->flatoffNlowLineEdit->text();
    QString nhigh = cdw->ui->flatoffNhighLineEdit->text();
    currentData = flatoffData;
    currentDirName = flatoffDir;

    memoryDecideDeletableStatus(flatoffData, false);

    pushBeginMessage(taskBasename, flatoffDir);
    pushConfigProcessflatoff();

    // TODO: The following line is needed only as long as we are handling splitting of raw data by scripts.
    if (flatoffData->myImageList[instData->validChip].isEmpty()) {
        flatoffData->populate("");
        emit populateMemoryView();
    }

    getNumberOfActiveImages(flatoffData);

    // Release as much memory as maximally necessary
    float nimg = flatoffData->myImageList[instData->validChip].length() + 1;  // The number of images one thread keeps in memory
    releaseMemory(nimg*instData->storage*maxExternalThreads, 1, "calibrator");
    // Protect the rest, will be unprotected as needed
    flatoffData->protectMemory();
    QString dataDirName = flatoffData->dirName;             // copies for thread safety
    QString dataSubDirName = flatoffData->subDirName;       // copies for thread safety
    QString dataDataType = flatoffData->dataType;

    doDataFitInRAM(nimg*instData->numUsedChips, instData->storage);

    // Loop over all chips
#pragma omp parallel for num_threads(maxExternalThreads) firstprivate(nlow, nhigh, min, max, dataDirName, dataSubDirName)
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (abortProcess || !successProcessing || instData->badChips.contains(chip)) continue;

        float nimg = flatoffData->myImageList[chip].length() + 1;  // The number of images we must keep in memory
        releaseMemory(nimg*instData->storage, maxExternalThreads, "calibrator");

        for (auto &it : flatoffData->myImageList[chip]) {
            if (abortProcess) break;
            if (!it->successProcessing) continue;
            it->setupCalibDataInMemory(false, true, false);
            it->checkCorrectMaskSize(instData);
            it->setModeFlag(min, max);
#pragma omp atomic
            progress += progressHalfStepSize;
        }
        if (flatoffData->myImageList[chip].length() == 0) {
            flatoffData->successProcessing = false;
            successProcessing = false;
            emit messageAvailable("Could not find data for chip "+QString::number(chip+1)+ " in "+flatoffData->subDirName, "error");
            emit criticalReceived();
            continue;
        }
        flatoffData->combineImagesCalib(chip, combineFlatoff_ptr, nlow, nhigh, dataDirName, dataSubDirName, dataDataType);
        flatoffData->getModeCombineImages(chip);
        flatoffData->writeCombinedImage(chip);
        flatoffData->unprotectMemory(chip);
        if (minimizeMemoryUsage) {
            for (auto &it : flatoffData->myImageList[chip]) it->freeAll();
        }
        if (!flatoffData->successProcessing) successProcessing = false;
#pragma omp atomic
        progress += progressCombinedStepSize;
    }
    if (!successProcessing) {
        emit progressUpdate(100);
        return;
    }

    flatoffData->reportModeCombineImages();

    checkSuccessProcessing(flatoffData);
    satisfyMaxMemorySetting();
    if (successProcessing) {
        emit progressUpdate(100);
        //        pushEndMessage(taskBasename, flatoffDir);
    }
}

void Controller::taskInternalProcessflat()
{
    QString flatDir = instructions.split(" ").at(1);
    QString biasDir = instructions.split(" ").at(2);
    Data *flatData = getData(DT_FLAT, flatDir);
    if (flatData == nullptr) return;      // Error triggered by getData();
    if (!testResetDesire(flatData)) return;

    Data *biasData = nullptr;
    QString min = cdw->ui->flatMinLineEdit->text();
    QString max = cdw->ui->flatMaxLineEdit->text();
    QString nlow = cdw->ui->flatNlowLineEdit->text();
    QString nhigh = cdw->ui->flatNhighLineEdit->text();
    currentData = flatData;
    currentDirName = flatDir;

    pushBeginMessage(taskBasename, flatDir);
    pushConfigProcessflat();

    // flatoff, or bias, or no bias; we call it "bias" for simplicity
    if (!mainGUI->ui->setupFlatoffLineEdit->text().isEmpty()) {
        biasData = getData(DT_FLATOFF, biasDir);
    }
    else if (!mainGUI->ui->setupBiasLineEdit->text().isEmpty()) {
        biasData = getData(DT_BIAS, biasDir);
    }

    // Must rescale flats when combining them
    flatData->rescaleFlag = true;

    if (biasData != nullptr && !biasData->hasAllMasterCalibs) {
        QString part1 = biasData->dirName+"/"+biasData->subDirName+"_*.fits\n";
        emit showMessageBox("Controller::MASTER_FLATOFF_NOT_FOUND", part1, "");
        successProcessing = false;
        return;
    }

    // Check if all flats have the same filter
    // TODO: imageInfo.filter is not yet populated at this point!
    // Actually, as far as I can see it never gets populated at all!
    /*
    QStringList filters = flatData->imageInfo.filter;
    filters.removeDuplicates();
    if (filters.length() > 1) {
        QString filtersJoined = filters.join(' ');
        QMessageBox::warning(this, tr("Non-identical filters found."),
                             tr("The flat-field directory ")+flatData->dirName+tr(" contains several different filters:\n")+
                             filtersJoined+"\n"+
                             tr("You must first remove the unwanted images."),
                             QMessageBox::Ok);
        return;
    }
    QString filter = filters[0];
    */

    // Retrieve nonlinearity information (checks internally if available, otherwise returns empty list)
    QList<QVector<float>> nonlinearityCoefficients;
    if (cdw->ui->nonlinearityCheckBox->isChecked()) {
        nonlinearityCoefficients = getNonlinearityCoefficients();
    }

    memoryDecideDeletableStatus(flatData, false);

    // TODO: The following line is needed only as long as we are handling splitting of raw data by scripts.
    if (flatData->myImageList[instData->validChip].isEmpty()) {
        flatData->populate("");
        emit populateMemoryView();
    }

    getNumberOfActiveImages(flatData);

    // Release as much memory as maximally necessary
    float nimg = flatData->myImageList[instData->validChip].length() + 2;  // The number of images one thread keeps in memory
    releaseMemory(nimg*instData->storage*maxExternalThreads, 1, "calibrator");
    // Protect the rest, will be unprotected as needed
    flatData->protectMemory();
    QString dataDirName = flatData->dirName;             // copies for thread safety
    QString dataSubDirName = flatData->subDirName;       // copies for thread safety
    QString dataDataType = flatData->dataType;
    QString biasDataType;
    if (biasData != nullptr) biasDataType = biasData->dataType;

    if (biasData != nullptr) biasData->protectMemory();

    doDataFitInRAM(nimg*instData->numUsedChips, instData->storage);

    // Loop over all chips
#pragma omp parallel for num_threads(maxExternalThreads) firstprivate(nlow, nhigh, min, max, dataDirName, dataSubDirName)
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (abortProcess || !successProcessing || instData->badChips.contains(chip)) continue;

        float nimg = flatData->myImageList[chip].length() + 2;  // The number of images we must keep in memory
        releaseMemory(nimg*instData->storage, maxExternalThreads, "calibrator");

        QString message = "";
        if (biasData != nullptr) {
            biasData->loadCombinedImage(chip);
            message = biasData->subDirName;
        }

        for (auto &it : flatData->myImageList[chip]) {
            if (abortProcess) break;
            if (!it->successProcessing) continue;
            if (verbosity >= 0 && !message.isEmpty()) emit messageAvailable(it->chipName + " : Correcting with "+message+"_"+QString::number(chip+1)+".fits", "image");
            // careful with the booleans, they make sure the data is correctly reread from disk or memory if task is repeated
            it->setupCalibDataInMemory(true, true, true);    // read from backupL1, if not then from disk. Makes backup copy if not yet done
            it->checkCorrectMaskSize(instData);
            it->setModeFlag(min, max);                       // Must set mode flags before subtracting dark component (flatoffs can have really high levels in NIR data)
            if (biasData != nullptr && biasData->successProcessing) { // cannot pass nullptr to subtractBias()
                //                it->subtractBias(ref, biasDataType);
                it->subtractBias(biasData->combinedImage[chip], biasDataType);
                //                  it->subtractBias(biasData->combinedImage[chip]->dataCurrent, biasDataType);
                it->skyValue -= biasData->combinedImage[chip]->skyValue;
                it->saturationValue -= biasData->combinedImage[chip]->skyValue;
            }
            //            it->getMode(true);
            //            it->setModeFlag(min, max);
            if (!it->successProcessing) flatData->successProcessing = false;  // does not need to be threadsafe
# pragma omp atomic
            progress += progressHalfStepSize;
        }
        if (biasData != nullptr) biasData->unprotectMemory(chip);
        if (flatData->myImageList[chip].length() == 0) {
            flatData->successProcessing = false;
            successProcessing = false;
            emit messageAvailable("Could not find data for chip "+QString::number(chip+1)+ " in "+flatData->subDirName, "error");
            emit criticalReceived();
            continue;
        }
        flatData->combineImagesCalib(chip, combineFlat_ptr, nlow, nhigh, dataDirName, dataSubDirName, dataDataType);
        // Remove Bayer intensity variations within a 2x2 superpixel
        if (!instData->bayer.isEmpty()) equalizeBayerFlat(flatData->combinedImage[chip]);
        flatData->getModeCombineImages(chip);
        // Individual images may be released; must keep master calib for further below
        flatData->memorySetDeletable(chip, "dataCurrent", true);
        for (auto &it : flatData->myImageList[chip]) it->freeData("dataCurrent");     // Bias subtracted pixels, not needed anymore
        flatData->memorySetDeletable(chip, "dataBackupL1", true);
        if (minimizeMemoryUsage) {
            for (auto &it : flatData->myImageList[chip]) it->freeAll();
            if (biasData != nullptr) biasData->combinedImage[chip]->freeAll();
        }

        if (!flatData->successProcessing) successProcessing = false;
#pragma omp atomic
        progress += progressCombinedStepSize;
        // Apply illumination correction (if available, e.g. for DECam; does nothing if not available)
        // TODO: can be uncommented once the filter is known (see commented block above)
        // flatData->combinedImage[chip]->illuminationCorrection(chip, thelidir, instData->name, filter);
    }

    if (!successProcessing) {
        emit progressUpdate(100);
        return;
    }

    // Obtain gain normalizations. Must be done once all images have been combined.
    flatData->getGainNormalization();

    // Normalize flats to one. Gain corrections are stored in member variable for later use
#pragma omp parallel for num_threads(maxExternalThreads)
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        flatData->combinedImage[chip]->normalizeFlat();
        flatData->combinedImage[chip]->applyMask();
        flatData->writeCombinedImage(chip);
        flatData->unprotectMemory(chip);
    }
    flatData->reportModeCombineImages();

    checkSuccessProcessing(flatData);
    satisfyMaxMemorySetting();
    if (successProcessing) {
        emit progressUpdate(100);
        //        pushEndMessage(taskBasename, flatDir);
    }
}

void Controller::taskInternalProcessscience()
{
    QString scienceDir = instructions.split(" ").at(1);
    QString biasDir = instructions.split(" ").at(2);
    QString flatDir = instructions.split(" ").at(3);
    QString scienceMode = instructions.split(" ").at(4);
    Data *scienceData;
    if (scienceMode == "theli_DT_SCIENCE") scienceData = getData(DT_SCIENCE, scienceDir);
    else if (scienceMode == "theli_DT_SKY") scienceData = getData(DT_SKY, scienceDir);
    else if (scienceMode == "theli_DT_STANDARD") scienceData = getData(DT_STANDARD, scienceDir);
    else {
        return;
    }

    if (scienceData == nullptr) return;      // Error triggered by getData();
    if (!testResetDesire(scienceData)) return;

    Data *biasData = nullptr;  // Can also point to a dark
    Data *flatData = nullptr;
    currentData = scienceData;
    currentDirName = scienceDir;

    pushBeginMessage(taskBasename, scienceDir);

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
    }

    if (biasData == nullptr && flatData == nullptr) {
        emit messageAvailable("No Bias / Dark or Flat calibrators defined. Nothing will be done.", "warning");
        return;
    }

    // Check if calib data exist (at least chip 1)
    if (biasData != nullptr && !biasData->hasAllMasterCalibs) {
        QString part1 = biasData->dirName+"/"+biasData->subDirName+"_*.fits\n";
        emit showMessageBox("Controller::MASTER_BIAS_NOT_FOUND", part1, "");
        successProcessing = false;
        return;
    }
    if (flatData != nullptr && !flatData->hasAllMasterCalibs) {
        QString part1 = flatData->dirName+"/"+flatData->subDirName+"_*.fits\n";
        emit showMessageBox("Controller::MASTER_FLAT_NOT_FOUND", part1, "");
        successProcessing = false;
        return;
    }

    memoryDecideDeletableStatus(scienceData, false);
    // TODO: The following line is needed only as long as we are handling splitting of raw data by scripts.
    if (scienceData->myImageList[instData->validChip].isEmpty()) {
        scienceData->populate("");
        emit populateMemoryView();
    }

    // Loop over all chips
    backupDirName = scienceData->processingStatus->getStatusString() + "_IMAGES";

    bool success = scienceData->checkTaskRepeatStatus(taskBasename);
    if (!success) return;

    getNumberOfActiveImages(scienceData);

    scienceData->bayerList.clear();
    scienceData->bayerList.resize(instData->numChips);

    QList<MyImage*> allMyImages;
    long numMyImages = makeListofAllImages(allMyImages, scienceData);
    QVector<long> numProcessedImages;
    numProcessedImages.fill(0, instData->numChips);

    // Release as much memory as maximally necessary
    float nimg;
    if (instData->bayer.isEmpty()) nimg = 4;  // old, new, bias, flat
    else nimg = 6;  // old, 3 new, bias, flat

    releaseMemory(nimg*instData->storage*maxCPU, 1);
    // Protect the rest, will be unprotected as needed
    scienceData->protectMemory();
    if (biasData != nullptr) biasData->protectMemory();
    if (flatData != nullptr) flatData->protectMemory();

    QString dataDirName = scienceData->dirName;             // copies for thread safety
    QString biasDataType;
    if (biasData != nullptr) biasDataType = biasData->dataType;

    // get rid of bad detectors, should they still be here
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (!instData->badChips.contains(chip)) continue;  // skip good detectors
        for (auto &it : scienceData->myImageList[chip]) {
            if (it->imageOnDrive) {                            // delete FITS file for bad detectors (if still present after splitting) so they cannot interfere
                deleteFile(it->baseName+".fits", it->path);
                it->imageOnDrive = false;
                it->activeState = MyImage::DELETED;
            }
        }
    }
    if (instData->bayer.isEmpty()) doDataFitInRAM(numMyImages*instData->numUsedChips, instData->storage);
    else {
        scienceData->currentlyDebayering = true;
        doDataFitInRAM(4*numMyImages*instData->numUsedChips, instData->storage);
    }

    if (instData->bayer.isEmpty()) {
        QStringList filters;
        for (auto &it : allMyImages) {
            filters.append(it->filter);
        }
        filters.removeDuplicates();
        if (filters.length() > 1) {
            QString filtersJoined = filters.join(", ");
            emit messageAvailable("Files with different FILTER keywords found in "+scienceData->dirName+" :", "error");
            emit messageAvailable(filtersJoined, "error");
            emit warningReceived();
            return;
        }
    }

    // Must keep track of memory consumption
    scienceData->bayerList.clear();
    scienceData->bayerList.resize(instData->numChips);

#pragma omp parallel for num_threads(maxCPU) firstprivate(dataDirName, biasDataType)
    for (int k=0; k<numMyImages; ++k) {
        if (abortProcess || !successProcessing) continue;

        auto &it = allMyImages[k];
        int chip = it->chipNumber - 1;

        if (!it->successProcessing) continue;
        if (it->activeState != MyImage::ACTIVE) continue;
        if (instData->badChips.contains(chip)) continue;     // redundant. Image not even in allMyImages[k];
        releaseMemory(nimg*instData->storage, maxCPU);

        // Don't remember why we need a lock here. I think it had to do with the headers. Will crash otherwise
        // TODO: probably not needed anymore with latest memory scheme
        QString message;
        // #pragma omp critical
        //        {
        if (biasData != nullptr) {
            biasData->loadCombinedImage(chip);  // skipped if already in memory
            message.append(biasData->subDirName);
        }
        if (flatData != nullptr) {
            flatData->loadCombinedImage(chip);  // skipped if already in memory
            if (message.isEmpty()) message.append(flatData->subDirName);
            else message.append(" and " + flatData->subDirName);
        }
        //        }
        if (verbosity >= 0) {
            if (!message.isEmpty() && instData->bayer.isEmpty()) emit messageAvailable(it->chipName + " : Correcting with "+message, "image");
            if (!message.isEmpty() && !instData->bayer.isEmpty()) emit messageAvailable(it->chipName + " : Correcting with "+message+", debayering", "image");
            if (message.isEmpty() && !instData->bayer.isEmpty()) emit messageAvailable(it->chipName + " : Debayering", "image");
        }
        it->processingStatus->Processscience = false;

        it->setupData(scienceData->isTaskRepeated, true, false, backupDirName);
        it->checkCorrectMaskSize(instData);

        if (!it->successProcessing) {
            abortProcess = true;
            continue;
        }

        // TODO: check if we can just pass the data structure and chip number,
        // and test internally for nullptr and 'successProcessing'.
        // Then the "if" could go away
        if (biasData != nullptr && biasData->successProcessing) {
            it->subtractBias(biasData->combinedImage[chip], biasDataType);
        }
        if (flatData != nullptr && flatData->successProcessing) {
            it->divideFlat(flatData->combinedImage[chip]);
        }
        it->bitpix = -32;  // so that mode calculations later on use the right algorithm

        // Without Bayer pattern
        if (instData->bayer.isEmpty()) {
            it->getMode(true);
            it->applyMask();
            it->backupOrigHeader(chip);             // Write out the zero order solution (before everything is kept in memory). This requires a FITS file
            updateImageAndData(it, scienceData);
            if (alwaysStoreData) {
                it->writeImage();
                it->unprotectMemory();
                if (minimizeMemoryUsage) {
                    it->freeAll();
                }
            }
        }
        else {
            // Create 3 new MyImages for R, G, and B
            MyImage *debayerB = new MyImage(dataDirName, it->baseName, "P", chip+1, mask->globalMask[chip], &verbosity);
            MyImage *debayerG = new MyImage(dataDirName, it->baseName, "P", chip+1, mask->globalMask[chip], &verbosity);
            MyImage *debayerR = new MyImage(dataDirName, it->baseName, "P", chip+1, mask->globalMask[chip], &verbosity);
            debayer(chip, it, debayerB, debayerG, debayerR);
            it->unprotectMemory();
            it->freeAll();
#pragma omp critical
            {
                // The order in which we insert the images here is important for data::writeGlobalWeights()!
                scienceData->bayerList[chip] << debayerB << debayerG << debayerR;
            }
            QList<MyImage*> list; // Contains the current 3 debayered images

            list << debayerB << debayerG << debayerR;
            for (auto &it_deb: list) {
                if (abortProcess) break;
                connect(it_deb, &MyImage::modelUpdateNeeded, scienceData, &Data::modelUpdateReceiver);
                connect(it_deb, &MyImage::critical, this, &Controller::criticalReceived);
                connect(it_deb, &MyImage::warning, this, &Controller::warningReceived);
                connect(it_deb, &MyImage::messageAvailable, this, &Controller::messageAvailableReceived);
                connect(it_deb, &MyImage::setMemoryLock, this, &Controller::setMemoryLockReceived, Qt::DirectConnection);
                it_deb->getMode(true);
                it_deb->applyMask();
                it_deb->backupOrigHeader(chip);
                it_deb->imageInMemory = true;
                it_deb->backupL1InMemory = true;
                it_deb->processingStatus->HDUreformat = true;

                updateImageAndData(it_deb, scienceData);

                // Must provide filter string explicitly (and therefore also the full path and file name.
                // Always store debayered images
                //                it->writeImageDebayer(it->path + "/" + it->chipName + "PA.fits", it->filter, it->exptime, it->mjdobs);
                it_deb->writeImageDebayer();
                it_deb->unprotectMemory();
                if (minimizeMemoryUsage) {
                    it_deb->freeAll();
                }
            }
        }
#pragma omp atomic
        progress += progressStepSize;
        ++numProcessedImages[chip];
        if ((minimizeMemoryUsage && instData->numChips > 1)
                || numProcessedImages[chip] == numMyImages/instData->numUsedChips) {
            // all images of this chip have been processed, and the calib data can be set deletable
            if (biasData != nullptr) biasData->unprotectMemory(chip);
            if (flatData != nullptr) flatData->unprotectMemory(chip);
        }
    }

    // Clear memory from pre-debayered data
    if (!instData->bayer.isEmpty()) {
        for (int chip=0; chip<instData->numChips; ++chip) {
            if (instData->badChips.contains(chip)) continue;
            for (auto &it : scienceData->myImageList[chip]) {
                it->freeAll();
            }
        }
    }

    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        // Update image list; remove bayer images, insert debayered images
        if (!instData->bayer.isEmpty()) {
            scienceData->repopulate(chip, scienceData->bayerList[chip]);
            emit populateMemoryView();
        }
        /*
        if (scienceData->successProcessing) {
            for (auto &it : scienceData->myImageList[chip]) {
                // it->makeMemoryBackup();
                //                qDebug() << "taskInternalProcessscience():" << scienceData->successProcessing << instData->bayer;
                if (instData->bayer != "Y") it->makeDriveBackup("P_IMAGES", statusOld);
                //                scienceData->memorySetDeletable(chip, "dataBackupL1", true);
            }
        }
        */
        // Double tapping ...
        if (biasData != nullptr) biasData->unprotectMemory(chip);
        if (flatData != nullptr) flatData->unprotectMemory(chip);
        if (minimizeMemoryUsage) {
            if (biasData != nullptr) biasData->releaseAllMemory();
            if (flatData != nullptr) flatData->releaseAllMemory();
        }
    }

    if (!instData->bayer.isEmpty()) scienceData->currentlyDebayering = false;
    scienceData->bayerList.clear();

    checkSuccessProcessing(scienceData);
    satisfyMaxMemorySetting();
    if (successProcessing) {
        scienceData->processingStatus->Processscience = true;
        scienceData->processingStatus->writeToDrive();
        scienceData->transferBackupInfo();
        scienceData->emitStatusChanged();
        emit addBackupDirToMemoryviewer(scienceDir);
        emit progressUpdate(100);
        //        pushEndMessage(taskBasename, scienceDir);
    }
}
