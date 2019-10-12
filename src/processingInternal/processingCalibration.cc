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
    QString min = cdw->ui->biasMinLineEdit->text();
    QString max = cdw->ui->biasMaxLineEdit->text();
    QString nlow = cdw->ui->biasNlowLineEdit->text();
    QString nhigh = cdw->ui->biasNhighLineEdit->text();
    currentData = biasData;
    currentDirName = biasDir;

    memoryDecideDeletableStatus(biasData, false);

    pushBeginMessage(taskBasename, biasDir);

    // TODO: The following line is needed only as long as we are handling splitting of raw data by scripts.
    // The biasData imagelist is empty after splitting because the Data constructor only looks for *_chip.fits, not for raw files.
    if (biasData->myImageList[0].isEmpty()) {
        biasData->populate("");
        emit populateMemoryView();
    }

    getNumberOfActiveImages(biasData);

    // Release as much memory as maximally necessary
    float nimg = biasData->myImageList[0].length() + 1;  // The number of images one thread keeps in memory
    releaseMemory(nimg*instData->storage*maxExternalThreads, 1, "calibrator");
    // Protect the rest, will be unprotected as needed
    biasData->protectMemory();
    QString dataDirName = biasData->dirName;             // copies for thread safety
    QString dataSubDirName = biasData->subDirName;       // copies for thread safety
    QString dataDataType = biasData->dataType;

    // Loop over all chips
    // NOTE: QString is not threadsafe, must create copies for threads!
#pragma omp parallel for num_threads(maxExternalThreads) firstprivate(nlow, nhigh, min, max, dataDirName, dataSubDirName)
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (abortProcess || !successProcessing) continue;
        float nimg = biasData->myImageList[chip].length() + 1;  // The number of images we must keep in memory
        // Release memory cannot touch any dataCurrent read by MyImage::readImage, because we 'protected' it outside the loop.
        // Initially, this call might not do anything because everything is protected. On systems with less RAM than
        // a single exposure this might cause swapping. We test for this elsewhere (when loading images).
        releaseMemory(nimg*instData->storage, maxExternalThreads, "calibrator");

        for (auto &it : biasData->myImageList[chip]) {
            if (abortProcess) break;
            it->setupCalibDataInMemory(false, true, false);   // Read image (if not already in memory), do not create backup, do get mode
            it->setModeFlag(min, max);                        // Flag the image if its mode is outside acceptable ranges
#pragma omp atomic
            progress += progressHalfStepSize;
        }
        biasData->combineImagesCalib(chip, combineBias_ptr, nlow, nhigh, dataDirName, dataSubDirName, dataDataType);  // Combine images
        biasData->getModeCombineImages(chip);
        biasData->writeCombinedImage(chip);
        biasData->unprotectMemory(chip);
        biasData->combinedImage[chip]->emitModelUpdateNeeded();
        if (!biasData->successProcessing) successProcessing = false;
#pragma omp atomic
        progress += progressCombinedStepSize;
    }

    /*
    QList<MyImage*> allMyImages;
    long numMyImages = makeListofAllImages(allMyImages, biasData);

#pragma omp parallel for num_threads(maxCPU)
    for (int k=0; k<numMyImages; ++k) {
        auto &it = allMyImages[k];
        int chip = it->chipNumber - 1;
        for (auto &it : biasData->myImageList[chip]) {
            it->setupCalibDataInMemory(false, true, false);    // Read image (if not already in memory), do not create backup copy, do not read or measure the mode
            it->setModeFlag(min, max);                 // Flag the image if its mode is outside acceptable ranges
            incrementProgressHalfStep();
        }
        biasData->combineImagesCalib(chip, combineBias_ptr, "forCalibration", nlow, nhigh);  // Combine images
        biasData->getModeCombineImages(chip);
        biasData->writeCombinedImage(chip);                                // Write combined image
        biasData->memorySetDeletable(chip, "dataCurrent", true);           // Split data may be deleted from memory
        incrementProgressCombinedStep();
    }
    */

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
    QString min = cdw->ui->darkMinLineEdit->text();
    QString max = cdw->ui->darkMaxLineEdit->text();
    QString nlow = cdw->ui->darkNlowLineEdit->text();
    QString nhigh = cdw->ui->darkNhighLineEdit->text();
    currentData = darkData;
    currentDirName = darkDir;

    memoryDecideDeletableStatus(darkData, false);

    pushBeginMessage(taskBasename, darkDir);

    getNumberOfActiveImages(darkData);

    // TODO: The following line is needed only as long as we are handling splitting of raw data by scripts.
    if (darkData->myImageList[0].isEmpty()) {
        darkData->populate("");
        emit populateMemoryView();
    }

    // Release as much memory as maximally necessary
    float nimg = darkData->myImageList[0].length() + 1;  // The number of images one thread keeps in memory
    releaseMemory(nimg*instData->storage*maxExternalThreads, 1, "calibrator");
    // Protect the rest, will be unprotected as needed
    darkData->protectMemory();
    QString dataDirName = darkData->dirName;             // copies for thread safety
    QString dataSubDirName = darkData->subDirName;       // copies for thread safety
    QString dataDataType = darkData->dataType;

    // Loop over all chips
#pragma omp parallel for num_threads(maxExternalThreads) firstprivate(nlow, nhigh, min, max, dataDirName, dataSubDirName)
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (abortProcess || !successProcessing) continue;

        float nimg = darkData->myImageList[chip].length() + 1;  // The number of images we must keep in memory
        releaseMemory(nimg*instData->storage, maxExternalThreads, "calibrator");

        for (auto &it : darkData->myImageList[chip]) {
            if (abortProcess) break;
            it->setupCalibDataInMemory(false, true, false);
            it->setModeFlag(min, max);
#pragma omp atomic
            progress += progressHalfStepSize;
        }
        darkData->combineImagesCalib(chip, combineDark_ptr, nlow, nhigh, dataDirName, dataSubDirName, dataDataType);
        darkData->getModeCombineImages(chip);
        darkData->writeCombinedImage(chip);
        darkData->unprotectMemory(chip);
        if (!darkData->successProcessing) successProcessing = false;
#pragma omp atomic
        progress += progressCombinedStepSize;
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
    QString min = cdw->ui->flatoffMinLineEdit->text();
    QString max = cdw->ui->flatoffMaxLineEdit->text();
    QString nlow = cdw->ui->flatoffNlowLineEdit->text();
    QString nhigh = cdw->ui->flatoffNhighLineEdit->text();
    currentData = flatoffData;
    currentDirName = flatoffDir;

    memoryDecideDeletableStatus(flatoffData, false);

    pushBeginMessage(taskBasename, flatoffDir);

    // TODO: The following line is needed only as long as we are handling splitting of raw data by scripts.
    if (flatoffData->myImageList[0].isEmpty()) {
        flatoffData->populate("");
        emit populateMemoryView();
    }

    getNumberOfActiveImages(flatoffData);

    // Release as much memory as maximally necessary
    float nimg = flatoffData->myImageList[0].length() + 1;  // The number of images one thread keeps in memory
    releaseMemory(nimg*instData->storage*maxExternalThreads, 1, "calibrator");
    // Protect the rest, will be unprotected as needed
    flatoffData->protectMemory();
    QString dataDirName = flatoffData->dirName;             // copies for thread safety
    QString dataSubDirName = flatoffData->subDirName;       // copies for thread safety
    QString dataDataType = flatoffData->dataType;

    // Loop over all chips
#pragma omp parallel for num_threads(maxExternalThreads) firstprivate(nlow, nhigh, min, max, dataDirName, dataSubDirName)
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (abortProcess || !successProcessing) continue;

        float nimg = flatoffData->myImageList[chip].length() + 1;  // The number of images we must keep in memory
        releaseMemory(nimg*instData->storage, maxExternalThreads, "calibrator");

        for (auto &it : flatoffData->myImageList[chip]) {
            if (abortProcess) break;
            it->setupCalibDataInMemory(false, true, false);
            it->setModeFlag(min, max);
#pragma omp atomic
            progress += progressHalfStepSize;
        }
        flatoffData->combineImagesCalib(chip, combineFlatoff_ptr, nlow, nhigh, dataDirName, dataSubDirName, dataDataType);
        flatoffData->getModeCombineImages(chip);
        flatoffData->writeCombinedImage(chip);
        flatoffData->unprotectMemory(chip);
        if (!flatoffData->successProcessing) successProcessing = false;
#pragma omp atomic
        progress += progressCombinedStepSize;
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
    Data *biasData = nullptr;
    QString min = cdw->ui->flatMinLineEdit->text();
    QString max = cdw->ui->flatMaxLineEdit->text();
    QString nlow = cdw->ui->flatNlowLineEdit->text();
    QString nhigh = cdw->ui->flatNhighLineEdit->text();
    currentData = flatData;
    currentDirName = flatDir;

    pushBeginMessage(taskBasename, flatDir);

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
        QString part1 = biasData->dirName+"/"+biasData->subDirName+"_?.fits\n";
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
    if (flatData->myImageList[0].isEmpty()) {
        flatData->populate("");
        emit populateMemoryView();
    }

    getNumberOfActiveImages(flatData);

    // Release as much memory as maximally necessary
    float nimg = flatData->myImageList[0].length() + 2;  // The number of images one thread keeps in memory
    releaseMemory(nimg*instData->storage*maxExternalThreads, 1, "calibrator");
    // Protect the rest, will be unprotected as needed
    flatData->protectMemory();
    QString dataDirName = flatData->dirName;             // copies for thread safety
    QString dataSubDirName = flatData->subDirName;       // copies for thread safety
    QString dataDataType = flatData->dataType;
    QString biasDataType;
    if (biasData != nullptr) biasDataType = biasData->dataType;

    if (biasData != nullptr) biasData->protectMemory();

    // Loop over all chips
#pragma omp parallel for num_threads(maxExternalThreads) firstprivate(nlow, nhigh, min, max, dataDirName, dataSubDirName)
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (abortProcess || !successProcessing) continue;

        float nimg = flatData->myImageList[chip].length() + 2;  // The number of images we must keep in memory
        releaseMemory(nimg*instData->storage, maxExternalThreads, "calibrator");

        if (biasData != nullptr) biasData->loadCombinedImage(chip);

        for (auto &it : flatData->myImageList[chip]) {
            if (abortProcess) break;
            if (verbosity >= 0) emit messageAvailable(it->chipName + " : Applying bias (or dark) correction ...", "image");
            // careful with the booleans, they make sure the data is correctly reread from disk or memory if task is repeated
            it->setupCalibDataInMemory(true, false, true);    // read from backupL1, if not then from disk. Makes backup copy if not yet done
            if (biasData != nullptr && biasData->successProcessing) { // cannot pass nullptr to subtractBias()
                it->subtractBias(biasData->combinedImage[chip], biasDataType);
            }
            it->getMode(true);
            it->setModeFlag(min, max);
            if (!it->successProcessing) flatData->successProcessing = false;  // does not need to be threadsafe
# pragma omp atomic
            progress += progressHalfStepSize;
        }
        if (biasData != nullptr) biasData->unprotectMemory(chip);
        flatData->combineImagesCalib(chip, combineFlat_ptr, nlow, nhigh, dataDirName, dataSubDirName, dataDataType);
        // Remove Bayer intensity variations within a 2x2 superpixel
        if (instData->bayer == "Y") equalizeBayerFlat(flatData->combinedImage[chip]);
        flatData->getModeCombineImages(chip);
        // Individual images may be released; must keep master calib for further below
        flatData->memorySetDeletable(chip, "dataCurrent", true);
        flatData->memorySetDeletable(chip, "dataBackupL1", true);
        if (!flatData->successProcessing) successProcessing = false;
#pragma omp atomic
        progress += progressCombinedStepSize;
        // Apply illumination correction (if available, e.g. for DECam; does nothing if not available)
        // TODO: can be uncommented once the filter is known (see commented block above)
        // flatData->combinedImage[chip]->illuminationCorrection(chip, thelidir, instData->name, filter);
    }

    // Obtain gain normalizations. Must be done once all images have been combined.
    flatData->getGainNormalization();

    // Normalize flats to one. Gain corrections are stored in member variable for later use
#pragma omp parallel for num_threads(maxExternalThreads)
    for (int chip=0; chip<instData->numChips; ++chip) {
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

    // Check if calib data exist (at least chip 1)
    if (biasData != nullptr && !biasData->hasAllMasterCalibs) {
        QString part1 = biasData->dirName+"/"+biasData->subDirName+"_?.fits\n";
        emit showMessageBox("Controller::MASTER_BIAS_NOT_FOUND", part1, "");
        successProcessing = false;
        return;
    }
    if (flatData != nullptr && !flatData->hasAllMasterCalibs) {
        QString part1 = flatData->dirName+"/"+flatData->subDirName+"_?.fits\n";
        emit showMessageBox("Controller::MASTER_FLAT_NOT_FOUND", part1, "");
        successProcessing = false;
        return;
    }

    memoryDecideDeletableStatus(scienceData, false);
    // TODO: The following line is needed only as long as we are handling splitting of raw data by scripts.
    if (scienceData->myImageList[0].isEmpty()) {
        scienceData->populate("");
        emit populateMemoryView();
    }

    // Loop over all chips
    backupDirName = scienceData->processingStatus->getStatusString() + "_IMAGES";

    scienceData->checkTaskRepeatStatus(taskBasename);

    getNumberOfActiveImages(scienceData);

    /*
#pragma omp parallel for num_threads(maxExternalThreads)
    for (int chip=0; chip<instData->numChips; ++chip) {
        incrementCurrentThreads(lock);
        if (biasData != nullptr) biasData->loadCombinedImage(chip);  // skipped if already in memory
        if (flatData != nullptr) flatData->loadCombinedImage(chip);  // skipped if already in memory

        QList<MyImage*> bayerList;
        for (auto &it : scienceData->myImageList[chip]) {
            if (verbosity >= 0) emit messageAvailable(it->baseName + " : Preprocessing ...", "controller");
            it->setupDataInMemory(isTaskRepeated, true, false); // pass statusOld? pushes split_images into dataBackupL1
            if (biasData != nullptr && biasData->successProcessing) {
                it->subtractBias(biasData->combinedImage[chip], biasData->dataType);
            }
            if (flatData != nullptr && flatData->successProcessing) {
                it->divideFlat(flatData->combinedImage[chip]);
            }
            it->bitpix = -32;  // so that mode calculations later on use the right algorithm
            // Without Bayer pattern
            if (instData->bayer != "Y") {
                it->getMode(true);
                it->applyMask();
                it->imageOnDrive = false;
                it->statusCurrent = statusNew;
                it->baseName = it->chipName + statusNew;
                if (alwaysStoreData) it->writeImage();
                it->makeMemoryBackup();
            }
            else {
                // Create 3 new MyImages for R, G, and B
                QString oldName = it->baseName;
                MyImage *debayerR = new MyImage(scienceData->dirName, it->baseName, "", chip+1, QVector<bool>(), false, &verbosity);
                MyImage *debayerG = new MyImage(scienceData->dirName, it->baseName, "", chip+1, QVector<bool>(), false, &verbosity);
                MyImage *debayerB = new MyImage(scienceData->dirName, it->baseName, "", chip+1, QVector<bool>(), false, &verbosity);
                debayer(chip, it, debayerR, debayerG, debayerB);
                QList<MyImage*> list; // Contains the current 3 debayered images
                connect(debayerR, &MyImage::critical, this, &Controller::criticalReceived);
                connect(debayerG, &MyImage::critical, this, &Controller::criticalReceived);
                connect(debayerB, &MyImage::critical, this, &Controller::criticalReceived);
                connect(debayerR, &MyImage::messageAvailable, this, &Controller::messageAvailableReceived);
                connect(debayerG, &MyImage::messageAvailable, this, &Controller::messageAvailableReceived);
                connect(debayerB, &MyImage::messageAvailable, this, &Controller::messageAvailableReceived);
                list << debayerR << debayerG << debayerB;
                for (auto &it: list) {
                    it->getMode(true);
                    it->applyMask();
                    it->baseName = it->chipName + statusNew;
                    // status settings etc are done in the debayer() call further above
                    updateDebayerMemoryStatus(it);
                    // Must provide filter string explicitly (and therefore also the full path and file name)
                    if (alwaysStoreData) it->writeImage(it->path + "/" + it->chipName + statusNew + ".fits", it->filter);
                    it->makeDriveBackupDebayer("P_IMAGES", oldName, statusOld);
                }
                bayerList[chip] << debayerR << debayerG << debayerB;  // contains ALL debayered images
            }
            incrementProgress();
        }
        //        makeThreadsAvailable(lock, numInternalThreads);
        // Update image list; remove bayer images, insert debayered images
        if (instData->bayer == "Y") scienceData->repopulate(chip, bayerList);

        if (scienceData->successProcessing) {
            for (auto &it : scienceData->myImageList[chip]) {
                // it->makeMemoryBackup();
                if (instData->bayer != "Y") it->makeDriveBackup("P_IMAGES", statusOld);
                scienceData->memorySetDeletable(chip, "dataBackupL1", true);
            }
        }

        decrementCurrentThreads(lock);
    }
    */

    QVector<QList<MyImage*>> bayerList;
    bayerList.resize(instData->numChips);

    QList<MyImage*> allMyImages;
    long numMyImages = makeListofAllImages(allMyImages, scienceData);
    QVector<long> numProcessedImages;
    numProcessedImages.fill(0, instData->numChips);

    // Release as much memory as maximally necessary
    float nimg;
    if (instData->bayer != "Y") nimg = 4;  // old, new, bias, flat
    else nimg = 6;  // old, 3 new, bias, flat

    releaseMemory(nimg*instData->storage*maxCPU, 1);
    // Protect the rest, will be unprotected as needed
    scienceData->protectMemory();
    if (biasData != nullptr) biasData->protectMemory();
    if (flatData != nullptr) flatData->protectMemory();

    QString dataDirName = scienceData->dirName;             // copies for thread safety
    QString biasDataType;
    if (biasData != nullptr) biasDataType = biasData->dataType;

#pragma omp parallel for num_threads(maxCPU) firstprivate(dataDirName, biasDataType)
    for (int k=0; k<numMyImages; ++k) {
        if (abortProcess || !successProcessing) continue;

        auto &it = allMyImages[k];
        int chip = it->chipNumber - 1;

        releaseMemory(nimg*instData->storage, maxCPU);

        // Don't remember why we need a lock here. I think it had to do with the headers. Will crash otherwise
        // TODO: probably not needed anymore with latest memory scheme
#pragma omp critical
        {
            if (biasData != nullptr) biasData->loadCombinedImage(chip);  // skipped if already in memory
            if (flatData != nullptr) flatData->loadCombinedImage(chip);  // skipped if already in memory
        }

        if (verbosity >= 0) emit messageAvailable(it->chipName + " : Applying bias/dark/flat correction ...", "image");
        it->processingStatus->Processscience = false;

        it->setupData(scienceData->isTaskRepeated, true, false, backupDirName);

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
        if (instData->bayer != "Y") {
            it->getMode(true);
            it->applyMask();
            it->backupOrigHeader(chip);             // Write out the zero order solution (before everything is kept in memory). This requires a FITS file
            updateImageAndData(it, scienceData);
            if (alwaysStoreData) {
                it->writeImage();
                it->unprotectMemory();
            }
        }
        else {
            // Create 3 new MyImages for R, G, and B
            MyImage *debayerR = new MyImage(dataDirName, it->baseName, "", chip+1, QVector<bool>(), false, &verbosity);
            MyImage *debayerG = new MyImage(dataDirName, it->baseName, "", chip+1, QVector<bool>(), false, &verbosity);
            MyImage *debayerB = new MyImage(dataDirName, it->baseName, "", chip+1, QVector<bool>(), false, &verbosity);
            debayer(chip, it, debayerR, debayerG, debayerB);
            QList<MyImage*> list; // Contains the current 3 debayered images

            list << debayerR << debayerG << debayerB;
            for (auto &it: list) {
                if (abortProcess) break;
                connect(it, &MyImage::modelUpdateNeeded, scienceData, &Data::modelUpdateReceiver);
                connect(it, &MyImage::critical, this, &Controller::criticalReceived);
                connect(it, &MyImage::warning, this, &Controller::warningReceived);
                connect(it, &MyImage::messageAvailable, this, &Controller::messageAvailableReceived);
                connect(it, &MyImage::setMemoryLock, this, &Controller::setMemoryLockReceived, Qt::DirectConnection);
                it->getMode(true);
                it->applyMask();
                it->backupOrigHeader(chip);
                it->imageInMemory = true;
                it->backupL1InMemory = true;

                updateImageAndData(it, scienceData);

                // Must provide filter string explicitly (and therefore also the full path and file name.
                // Always store debayered images
//                it->writeImageDebayer(it->path + "/" + it->chipName + "PA.fits", it->filter, it->exptime, it->mjdobs);
                it->writeImageDebayer();
                it->unprotectMemory();
            }
#pragma omp critical
            {
                bayerList[chip] << debayerR << debayerG << debayerB;  // contains ALL debayered images
            }
        }
#pragma omp atomic
        progress += progressStepSize;
        ++numProcessedImages[chip];
        if ((minimizeMemoryUsage && instData->numChips > 1)
                || numProcessedImages[chip] == instData->numChips) {
            // all images of this chip have been processed, and the calib data can be set deletable
            if (biasData != nullptr) biasData->unprotectMemory(chip);
            if (flatData != nullptr) flatData->unprotectMemory(chip);
        }
    }

    for (int chip=0; chip<instData->numChips; ++chip) {
        // Update image list; remove bayer images, insert debayered images
        if (instData->bayer == "Y") scienceData->repopulate(chip, bayerList[chip]);
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
    }

    checkSuccessProcessing(scienceData);
    satisfyMaxMemorySetting();
    if (successProcessing) {
        scienceData->processingStatus->Processscience = true;
        scienceData->processingStatus->writeToDrive();
        scienceData->transferBackupInfo();
        scienceData->emitStatusChanged();
        emit addBackupDirToMemoryviewer(scienceDir, backupDirName);
        emit progressUpdate(100);
//        pushEndMessage(taskBasename, scienceDir);
    }
}