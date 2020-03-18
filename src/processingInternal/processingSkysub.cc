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
#include "../dockwidgets/monitor.h"
#include "../tools/tools.h"
#include "../tools/fitting.h"
#include "ui_confdockwidget.h"

#include <QMetaObject>
#include <QVector>
#include <QStringList>
#include <QStandardPaths>
#include <QProgressBar>

void Controller::taskInternalSkysub()
{
    QString scienceDir = instructions.split(" ").at(1);
    Data *scienceData = getData(DT_SCIENCE, scienceDir);
    currentData = scienceData;
    currentDirName = scienceDir;

    memoryDecideDeletableStatus(scienceData, false);

    if (!scienceData->hasMatchingPartnerFiles(mainDirName+"/WEIGHTS/", ".weight.fits")) return;

    // Loop over all chips

    backupDirName = scienceData->processingStatus->getStatusString() + "_IMAGES";

    // Parameters for sky subtraction
    QString DT = cdw->ui->skyDTLineEdit->text();
    QString DMIN = cdw->ui->skyDMINLineEdit->text();
    QString expFactor = cdw->ui->skyMefLineEdit->text();
    QString kernelWidth = cdw->ui->skyKernelLineEdit->text();

    // Polyfit: read the sky values here, do the polynomial fit, store the gsl_vector with the solution
    // and evaluate inside the function

    getNumberOfActiveImages(scienceData);

    scienceData->cleanBackgroundModelStatus();

    scienceData->checkTaskRepeatStatus(taskBasename);

    if (cdw->ui->skyModelRadioButton->isChecked()) {
        skysubModel(scienceData, DT, DMIN, expFactor, kernelWidth);
    }
    else if (cdw->ui->skyPolynomialRadioButton->isChecked()) {
        skysubPolynomialFit(scienceData);
    }
    else if (cdw->ui->skyConstsubRadioButton->isChecked()) {
        if (cdw->ui->skyAreaComboBox->currentIndex() == 0) {
            skysubConstantFromArea(scienceData);
        }
        else if (cdw->ui->skyAreaComboBox->currentIndex() == 1) {
            skysubConstantEachChip(scienceData, DT, DMIN, expFactor, kernelWidth);
        }
        else {
            skysubConstantReferenceChip(scienceData, DT, DMIN, expFactor, kernelWidth);
        }
    }
    else {
        // Other methods to be implemented
    }

    checkSuccessProcessing(scienceData);
    satisfyMaxMemorySetting();

    if (successProcessing) {
        scienceData->processingStatus->Skysub = true;
        scienceData->processingStatus->writeToDrive();
        scienceData->transferBackupInfo();
        scienceData->emitStatusChanged();
        emit addBackupDirToMemoryviewer(scienceDir, backupDirName);
        emit progressUpdate(100);
        //        pushEndMessage(taskBasename, scienceDir);
    }
}

void Controller::skysubPolynomialFit(Data *scienceData)
{
    if (!successProcessing) return;

    pushBeginMessage("SkysubPoly", scienceData->subDirName);

    scienceData->populateExposureList();

    // Measure the sky in all blank regions, update the list of sky nodes in each chip
    QList<MyImage*> ignoreReturnValue = measureSkyInBlankRegions(scienceData);

    // Loop over all exposures
    int order = cdw->ui->skyPolynomialSpinBox->value();
    int numExposures = scienceData->exposureList.length();

    // Overriding getNumberOfActiveImages()
    progressStepSize = 100. / (float) numExposures;

    float nimg = 4;  // wild guess
    releaseMemory(nimg*instData->storage*maxCPU, 1);
    scienceData->protectMemory();

#pragma omp parallel for num_threads(maxCPU) firstprivate(backupDirName)
    for (long i=0; i<numExposures; ++i) {
        if (abortProcess || !successProcessing) continue;

        emit messageAvailable("Building polynomial fit for exposure " + QString::number(i) + " / "
                              + QString::number(numExposures), "controller");
        QList<QVector<double>> skyPolyfitNodes;
        // Collect sky nodes from all images belonging to that exposure
        for (auto &it : scienceData->exposureList[i]) {
            if (!it->successProcessing) continue;
            skyPolyfitNodes.append(it->skyPolyfitNodes);
        }
        // Fit a polynomial to the nodes, subtract the model from the images
        Fitting skyFit;
        skyFit.makePolynomialFit2D(order, skyPolyfitNodes);
        // TODO: large multi-chip cameras would profit from an internal parallelization
        // I THINK a simple '#pragma omp for' would take care of this automatically
        for (auto &it : scienceData->exposureList[i]) {
            if (abortProcess) break;
            if (!it->successProcessing) continue;

            releaseMemory(nimg*instData->storage, maxCPU);

            // Already done in measureSkyInBlankRegions;
            // Does nothing if image is still in memory.
            // If not in memory, will just read it again from drive
            it->processingStatus->Skysub = false;
            it->setupData(scienceData->isTaskRepeated, true, false, backupDirName);
            it->subtractSkyFit(order, skyFit.c, cdw->ui->skySavemodelCheckBox->isChecked());

            updateImageAndData(it, scienceData);

            // Must write for SWarp!
            it->writeImage();
            it->unprotectMemory();
            if (minimizeMemoryUsage) {
                it->freeAll();
            }
            emit messageAvailable(it->chipName + " : &lt;sky&gt; = " + QString::number(it->meanExposureBackground,'f',2) + " e-", "image");
#pragma omp atomic
            progress += progressStepSize;
        }
    }
}

void Controller::skysubConstantFromArea(Data *scienceData)
{
    if (!successProcessing) return;

    pushBeginMessage("SkysubConst", scienceData->subDirName);

    //    scienceData->populateExposureList();

    // Measure the sky in all blank regions, update the list of sky nodes in each chip
    QList<MyImage*> listOfAllImages = measureSkyInBlankRegions(scienceData);

    progressStepSize = 50. / float(scienceData->exposureList.length());
    // Loop over all exposures (consisting of n chips)
#pragma omp parallel for num_threads(maxCPU)
    for (long i=0; i<scienceData->exposureList.length(); ++i) {
        if (abortProcess || !successProcessing) continue;
        // Calculate the mean sky background for the exposure
        QVector<float> skyBackground;
        for (auto &it : scienceData->exposureList[i]) {
            if (!it->successProcessing) continue;
            for (auto &position : it->skyPolyfitNodes) {
                skyBackground.append(position[2]);
            }
        }
        float meanExposureBackground;
        if (skyBackground.isEmpty()) {
            QString part1 = scienceData->exposureList[i][0]->rootName;
            emit showMessageBox("Controller::NO_OVERLAP_WITH_SKYAREA", part1, "");
            meanExposureBackground = 0.;
            successProcessing = false;
            continue;
        }
        else {
            meanExposureBackground = meanMask(skyBackground);
        }
        for (auto &it : scienceData->exposureList[i]) {
            if (!it->successProcessing) continue;
            it->meanExposureBackground = meanExposureBackground;
            emit messageAvailable(it->chipName + " : &lt;sky&gt; = " + QString::number(it->meanExposureBackground,'f',2) + " e-", "image");
            // sky subtraction outsourced below for faster parallelisation
        }
#pragma omp atomic
        progress += progressStepSize;
    }

    if (!successProcessing) return;

    progressStepSize = 50. / float(listOfAllImages.length());
    float nimg = 3;  // wild guess
    releaseMemory(nimg*instData->storage*maxCPU, 1);
    scienceData->protectMemory();

    // In principle this could be done in nicer form by including it in the last for-loop above
#pragma omp parallel for num_threads(maxCPU) firstprivate(backupDirName)
    for (long i=0; i<listOfAllImages.length(); ++i) {
        if (abortProcess || !successProcessing) continue;

        releaseMemory(nimg*instData->storage, maxCPU);

        auto &it = listOfAllImages[i];
        if (!it->successProcessing) continue;
        // Already done in measureSkyInBlankRegions;
        // Does nothing if image is still in memory.
        // If not in memory, will just read it again from drive
        it->processingStatus->Skysub = false;
        it->setupData(scienceData->isTaskRepeated, true, false, backupDirName);
        it->subtract(it->meanExposureBackground);
        updateImageAndData(it, scienceData);

        // Must write for SWarp!
        it->writeImage();
        if (cdw->ui->skySavemodelCheckBox->isChecked()) {
            it->writeConstSkyImage(it->meanExposureBackground);
        }
        it->unprotectMemory();
        if (minimizeMemoryUsage) {
            it->freeAll();
        }
        emit messageAvailable(it->chipName + " : &lt;sky&gt; = " + QString::number(it->meanExposureBackground,'f',2) + " e-", "image");
#pragma omp atomic
        progress += progressStepSize;
    }
}

void Controller::skysubConstantReferenceChip(Data *scienceData, QString DT, QString DMIN, QString expFactor, QString kernelWidth)
{
    if (!successProcessing) return;

    pushBeginMessage("SkysubConst", scienceData->subDirName);

    scienceData->populateExposureList();

    // TODO: add a mode that does not require blank sky fields (i.e., if no coords file found, use entire chip)
    // Measure the sky in all blank regions, update the list of sky nodes in each chip
    QList<MyImage*> listOfAllImages = measureSkyInBlankRegions(scienceData, "listOnly");

    int referenceChip = cdw->ui->skyAreaComboBox->currentIndex();
    if (referenceChip < 2) {
        emit messageAvailable("Controller::skysubConstantReferenceChip(): invalid chip index returned. This is a bug.", "error");
        criticalReceived();
        successProcessing = false;
        return;
    }
    // Calculate the corrrect chip number (subtract the first two entries of the combo box)
    referenceChip -= 2;

    // Loop over all exposures (consisting of n chips)
    int kernel = kernelWidth.toInt();

    // Create object masks (does not change pixels)
    progressStepSize = 50. / float(scienceData->exposureList.length());

    float nimg = 5;  // wild guess
    releaseMemory(nimg*instData->storage*maxCPU, 1);
    scienceData->protectMemory();

#pragma omp parallel for num_threads(maxCPU) firstprivate(DT, DMIN, expFactor)
    for (long i=0; i<scienceData->exposureList.length(); ++i) {
        if (abortProcess || !successProcessing) continue;

        releaseMemory(nimg*instData->storage, maxCPU);

        // Measure the sky background from the reference chip
        auto &it = scienceData->exposureList[i][referenceChip];
        if (!it->successProcessing) continue;
        // Already done in measureSkyInBlankRegions;
        // Does nothing if image is still in memory.
        // If not in memory, will just read it again from drive
        it->setupData(scienceData->isTaskRepeated, false, false);
        it->resetObjectMasking();
        it->backgroundModelDone = false;
        it->readWeight();
        it->backgroundModel(kernel, "interpolate");
        it->segmentImage(DT, DMIN, true, false);
        it->transferObjectsToMask();
        it->maskExpand(expFactor);
        it->mergeObjectWithGlobalMask();
        if (!it->successProcessing) scienceData->successProcessing = false;
        float meanExposureBackground = modeMask(it->dataCurrent, "stable", it->objectMask)[0];
        emit messageAvailable(scienceData->exposureList[0][0]->rootName + " : Exposure background = " + QString::number(meanExposureBackground,'f',2), "image");
        for (auto &it : scienceData->exposureList[i]) {
            it->meanExposureBackground = meanExposureBackground;
        }
        it->unprotectMemory();
        if (minimizeMemoryUsage) {
            it->freeAll();
        }
        it->releaseBackgroundMemory("entirely");
        it->releaseAllDetectionMemory();
#pragma omp atomic
        progress += progressStepSize;
    }

    if (!successProcessing) return;

    // Subtract sky (does change pixels)
    progressStepSize = 50. / float(listOfAllImages.length());
#pragma omp parallel for num_threads(maxCPU) firstprivate(backupDirName)
    for (long i=0; i<listOfAllImages.length(); ++i) {
        if (abortProcess || !successProcessing) continue;
        auto &it = listOfAllImages[i];
        if (!it->successProcessing) continue;
        emit messageAvailable(it->chipName + " : &lt;sky&gt; = " + QString::number(it->meanExposureBackground,'f',2) + " e-", "image");
        it->processingStatus->Skysub = false;
        it->setupData(scienceData->isTaskRepeated, true, false, backupDirName);
        it->subtract(it->meanExposureBackground);

        updateImageAndData(it, scienceData);

        // Must write for SWarp!
        it->writeImage();
        if (cdw->ui->skySavemodelCheckBox->isChecked()) {
            it->writeConstSkyImage(it->meanExposureBackground);
        }
        it->unprotectMemory();
        if (minimizeMemoryUsage) {
            it->freeAll();
        }
#pragma omp atomic
        progress += progressStepSize;
    }
}

void Controller::skysubConstantEachChip(Data *scienceData, QString DT, QString DMIN, QString expFactor, QString kernelWidth)
{
    if (!successProcessing) return;

    // Loop over all exposures (consisting of n chips)
    int kernel = kernelWidth.toInt();

    /*
#pragma omp parallel for num_threads(maxExternalThreads)
    for (int chip=0; chip<instData->numChips; ++chip) {
        incrementCurrentThreads(lock);
        for (auto &it : scienceData->myImageList[chip]) {
            it->setupDataInMemory(isTaskRepeated, true, true);
            it->resetObjectMasking();
            it->backgroundModel(kernel, "interpolate");
            it->segmentImage(DT, DMIN, true, false);
            it->transferObjectsToMask();
            it->maskExpand(expFactor);
            it->mergeObjectWithGlobalMask();
            float meanExposureBackground = modeMask(it->dataCurrent, "stable", it->objectMask)[0];
            it->subtract(meanExposureBackground);
            it->statusCurrent = statusNew;
            it->baseName = it->chipName + statusNew;
            it->writeImage();
            emit messageAvailable(it->baseName + " : sky = "+QString::number(meanExposureBackground,'f',2), "controller");
            incrementProgress();
        }
        if (scienceData->successProcessing) {
            for (auto &it : scienceData->myImageList[chip]) {
                // it->makeMemoryBackup();
                it->makeDriveBackup(statusOld+"_IMAGES", statusOld);
                scienceData->memorySetDeletable(chip, "dataBackupL1", true);
            }
        }
        decrementCurrentThreads(lock);
    }
    */

    QList<MyImage*> allMyImages;
    long numMyImages = makeListofAllImages(allMyImages, scienceData);
    progressStepSize = 100. / float(numMyImages);

    float nimg = 5;  // wild guess
    releaseMemory(nimg*instData->storage*maxCPU, 1);
    scienceData->protectMemory();

#pragma omp parallel for num_threads(maxCPU) firstprivate(DT, DMIN, expFactor, backupDirName)
    for (int k=0; k<numMyImages; ++k) {
        if (abortProcess || !successProcessing) continue;

        releaseMemory(nimg*instData->storage, maxCPU);

        auto &it = allMyImages[k];
        if (!it->successProcessing) continue;
        it->processingStatus->Skysub = false;

        it->setupData(scienceData->isTaskRepeated, true, false, backupDirName);
        it->resetObjectMasking();
        it->backgroundModelDone = false;
        it->readWeight();
        it->backgroundModel(kernel, "interpolate");
        it->segmentImage(DT, DMIN, true, false);
        it->transferObjectsToMask();
        it->maskExpand(expFactor);
        it->mergeObjectWithGlobalMask();
        float meanExposureBackground = modeMask(it->dataCurrent, "stable", it->objectMask)[0];
        it->subtract(meanExposureBackground);

        updateImageAndData(it, scienceData);

        // Must write for SWarp!
        it->writeImage();
        if (cdw->ui->skySavemodelCheckBox->isChecked()) {
            it->writeConstSkyImage(meanExposureBackground);
        }
        it->unprotectMemory();
        it->releaseBackgroundMemory("entirely");
        it->releaseAllDetectionMemory();
        if (minimizeMemoryUsage) {
            it->freeAll();
        }
        emit messageAvailable(it->chipName + " : &lt;sky&gt; = " + QString::number(meanExposureBackground,'f',2) + " e-", "image");
#pragma omp atomic
        progress += progressStepSize;
    }

    /*
    // Just file moving, no parallelization required
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (scienceData->successProcessing) {
            for (auto &it : scienceData->myImageList[chip]) {
                // it->makeMemoryBackup();
                it->makeDriveBackup(statusOld+"_IMAGES", statusOld);
                //                scienceData->memorySetDeletable(chip, "dataBackupL1", true);
            }
        }
    }
    */
}

void Controller::skysubModel(Data *scienceData, QString DT, QString DMIN, QString expFactor, QString kernelWidth)
{
    if (!successProcessing) return;

    pushBeginMessage("SkysubModel", scienceData->subDirName);

    int kernel = kernelWidth.toInt();

    /*
#pragma omp parallel for num_threads(maxExternalThreads)
    for (int chip=0; chip<instData->numChips; ++chip) {
        incrementCurrentThreads(lock);
        for (auto &it : scienceData->myImageList[chip]) {
            emit messageAvailable(it->baseName + " : Modeling the sky ...", "controller");
            it->setupDataInMemory(isTaskRepeated, true, true);
            it->readWeight();
            // Model background so we can detect objects
            it->resetObjectMasking();
            it->backgroundModel(kernel, "interpolate");
            it->segmentImage(DT, DMIN, true, false);
            it->transferObjectsToMask();

            // Model background once more, including object masks, and detect objects
            it->backgroundModel(kernel, "interpolate");
            it->resetObjectMasking();
            it->segmentImage(DT, DMIN, true, false);
            it->transferObjectsToMask();
            it->maskExpand(expFactor);

            // Model background once more, including object masks, and subtract it from the image
            it->backgroundModel(kernel, "interpolate");
            it->subtractBackgroundModel();
            it->freeAncillaryData(it->dataBackground); // We don't need that anymore
            it->statusCurrent = statusNew;
            it->baseName = it->chipName + statusNew;
            // it->freeAncillaryData(it->dataSegmentation); // We don't need that anymore  // boolean
            it->writeImage();
            incrementProgress();
        }

        if (scienceData->successProcessing) {
            for (auto &it : scienceData->myImageList[chip]) {
                // it->makeMemoryBackup();
                it->makeDriveBackup(statusOld+"_IMAGES", statusOld);
                scienceData->memorySetDeletable(chip, "dataBackupL1", true);
                scienceData->memorySetDeletable(chip, "databackgroundL1", true);  // potential double free? see 10 lines above
            }
        }
        decrementCurrentThreads(lock);
    }
    */

    QList<MyImage*> allMyImages;
    long numMyImages = makeListofAllImages(allMyImages, scienceData);
    progressStepSize = 100. / float(numMyImages);

    float nimg = 5;  // wild guess
    releaseMemory(nimg*instData->storage*maxCPU, 1);
    scienceData->protectMemory();

    emit messageAvailable(" Calculating sky models ...", "controller");

#pragma omp parallel for num_threads(maxCPU) firstprivate(DT, DMIN, expFactor, backupDirName)
    for (int k=0; k<numMyImages; ++k) {
        if (abortProcess || !successProcessing) continue;

        releaseMemory(nimg*instData->storage, maxCPU);

        auto &it = allMyImages[k];
        if (!it->successProcessing) continue;
        //        emit messageAvailable(it->baseName + " : Modeling the sky ...", "controller");
        it->processingStatus->Skysub = false;
        it->setupData(scienceData->isTaskRepeated, true, true, backupDirName);
        it->readWeight();
        // Model background so we can detect objects
        it->resetObjectMasking();
        it->backgroundModelDone = false;
        it->backgroundModel(kernel, "interpolate");
        it->segmentImage(DT, DMIN, true, false);
        it->transferObjectsToMask();

        // Model background once more, including object masks, and detect objects
        it->backgroundModelDone = false;
        it->backgroundModel(kernel, "interpolate");
        it->resetObjectMasking();
        it->segmentImage(DT, DMIN, true, false);
        it->transferObjectsToMask();           // overwrites previous mask
        it->maskExpand(expFactor, false);       // false -> true to write the mask FITS files

        // Model background once more, including object masks, and subtract it from the image
        it->backgroundModelDone = false;
        it->backgroundModel(kernel, "interpolate");
        it->subtractBackgroundModel();
        if (cdw->ui->skySavemodelCheckBox->isChecked()) {
            it->writeBackgroundModel();
        }
        it->getMeanBackground();
        if (!isnan(it->meanExposureBackground)) {
            emit messageAvailable(it->chipName + " : &lt;sky&gt; = " + QString::number(it->meanExposureBackground,'f',2) + " e-", "image");
        }
        else {
            emit messageAvailable(it->chipName + " : &lt;sky&gt; = " + QString::number(it->meanExposureBackground,'f',2) + " e-", "warning");
            warningReceived();
            it->setActiveState(MyImage::BADBACK);
            continue;
        }
        it->releaseAllDetectionMemory();
        it->releaseBackgroundMemory("entirely");

        updateImageAndData(it, scienceData);

        // Must write for SWarp
        it->writeImage();
        it->unprotectMemory();
        if (minimizeMemoryUsage) {
            it->freeAll();
        }
#pragma omp atomic
        progress += progressStepSize;
    }

    /*
    // Just file moving, no parallelization required
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (scienceData->successProcessing) {
            for (auto &it : scienceData->myImageList[chip]) {
                // it->makeMemoryBackup();
                it->makeDriveBackup(statusOld+"_IMAGES", statusOld);
                //                scienceData->memorySetDeletable(chip, "dataBackupL1", true);
                //                scienceData->memorySetDeletable(chip, "databackgroundL1", true);  // potential double free? see 10 lines above
            }
        }
    }
    */
}

QList<MyImage*> Controller::measureSkyInBlankRegions(Data *scienceData, QString method)
{
    QList<MyImage*> listOfAllImages;

    if (!successProcessing) return listOfAllImages;

    emit messageAvailable("Measuring sky level in user-defined areas ...", "controller");

    // Measure background at all sky positions
    // For full parallelisation, I collapse the loops. Since some images from some detectors might
    // have been removed, the loops are not prefectly rectangular
    // (one could use a 'continue' statement, though, but would have to determine the max number of exposures per chip
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : scienceData->myImageList[chip]) {
            if (!it->successProcessing) continue;
            listOfAllImages.append(it);
        }
    }

    if (listOfAllImages.isEmpty()) {
        successProcessing = false;
        return listOfAllImages;
    }

    if (method.isEmpty()) {
        QVector<double> alpha;
        QVector<double> delta;
        QVector<double> radius;

        // Retrieve sky positions from maindir/subdir/skysamples.dat
        if (verbosity > 1) emit messageAvailable("Reading blank sky positions from " + scienceData->dirName+"/skysamples.dat", "controller");
        if (!readData3D(scienceData->dirName+"/skysamples.dat", alpha, delta, radius) || alpha.isEmpty()) {
            if (verbosity > 1) emit messageAvailable("Fallback: Reading blank sky positions from " + mainDirName+"/skysamples.dat", "controller");
            if (!readData3D(mainDirName+"/skysamples.dat", alpha, delta, radius) || alpha.isEmpty()) {
                QString part1 = scienceData->dirName;
                emit showMessageBox("Controller::SKY_FILE_NOT_FOUND", part1, "");
                successProcessing = false;
                return listOfAllImages;
            }
        }
#pragma omp parallel for num_threads(maxCPU) firstprivate(backupDirName, alpha, delta, radius)
        for (int i=0; i<listOfAllImages.length(); ++i) {
            listOfAllImages[i]->setupData(scienceData->isTaskRepeated, false, true, backupDirName);     // 'false': do not move images to backup dir yet
            listOfAllImages[i]->evaluateSkyNodes(alpha, delta, radius);
            listOfAllImages[i]->unprotectMemory();
            listOfAllImages[i]->freeAll();
        }
    }

    // Get all images that belong to one exposure (number could vary from exposure to exposure)
    scienceData->populateExposureList();

    return listOfAllImages;
}
