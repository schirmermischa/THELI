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

#include "data.h"
#include "mask.h"
#include "../myimage/myimage.h"
#include "../functions.h"
#include "../tools/tools.h"
#include "../tools/cfitsioerrorcodes.h"
#include "../preferences.h"
#include "../instrumentdata.h"
#include "../threading/memoryworker.h"
#include "../processingStatus/processingStatus.h"

#include "fitsio.h"
#include <omp.h>
#include <QDir>
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QPushButton>
#include <QSettings>
#include <QMainWindow>
#include <QVector>
#include <QProgressBar>

// Ctor, given an absolute path name (dirName), status string, and optionally chip number
// It creates a list of all matching images in that directory, and also reads some
// essential FITS header keywords.
Data::Data(const instrumentDataType *instrumentData, Mask *detectorMask, QString maindirname,
           QString subdirname, int *verbose) : instData(instrumentData)
{
    emit messageAvailable("DATA: Setting up "+subdirname, "data");

    initEnvironment(thelidir, userdir);

    resetSuccessProcessing();

    omp_init_lock(&progressLock);

    mainDirName = maindirname;
    subDirName = subdirname;

    dirName = mainDirName + "/" + subDirName;
    myImageList.resize(instData->numChips);
    combinedImage.resize(instData->numChips);
    dataInitialized = false;

    verbosity = verbose;

    // globalMask.resize(instData->numChips);

    // Pointer to the detector masks defined in the top-level controller;
    // We don't need a separate entity for each 'Data' instance
    mask = detectorMask;

    // Get the recorded processing status from the .processingStatus file (if any)
    processingStatus = new ProcessingStatus(dirName, this);
    processingStatus->readFromDrive();

    QString backupStatus = processingStatus->statusString;
    backupStatus.chop(1);
    pathBackupL1 = dirName + "/" + backupStatus + "_IMAGES";

    dir.setPath(dirName);
    if (!dir.exists()) {
        emit messageAvailable("DATA: Could not create Data structure for "+mainDirName+"/"+subDirName, "error");
        emit critical();
        return;
    }

    // start fresh
    clearImageInfo();

    // Check whether this directory contains RAW data (and thus must be processed by HDUreformat first)
    if (subDirName != "GLOBALWEIGHTS") {    // crashes otherwise when creating globalweights
        if (checkForRawData()) return;
    }

    if (subDirName == "GLOBALWEIGHTS") {
        numMasterCalibs = 0;
        numImages = 0;
        for (int chip=0; chip<instData->numChips; ++chip) {
            // The list is fully populated, even for chips the user might decide not to use
            myImageList[chip].clear(); // in case we run globalweights several times
            QStringList filter;
            filter << "globalweight*_"+QString::number(chip+1)+".fits";
            QStringList fitsFiles = dir.entryList(filter);
            for (auto &it : fitsFiles) {
                MyImage *myImage = new MyImage(dirName, it, "", chip+1, mask->globalMask[chip], verbosity);
                myImage->setParent(this);
                myImage->readFILTER(dirName+"/"+it);
                myImage->imageOnDrive = true;
                myImageList[chip].append(myImage);
                ++numImages;
                connect(myImage, &MyImage::modelUpdateNeeded, this, &Data::modelUpdateReceiver);
                connect(myImage, &MyImage::critical, this, &Data::pushCritical);
                connect(myImage, &MyImage::warning, this, &Data::pushWarning);
                connect(myImage, &MyImage::errorOccurred, this, &Data::errorOccurredInMyImage);
                connect(myImage, &MyImage::messageAvailable, this, &Data::pushMessageAvailable);
                // Use a direct connection to execute the slot in the signaler's thread, not the receiver's thread
                connect(myImage, &MyImage::setMemoryLock, this, &Data::setMemoryLockReceived, Qt::DirectConnection);
                connect(myImage, &MyImage::setWCSLock, this, &Data::setWCSLockReceived, Qt::DirectConnection);
            }
        }
        if (numImages < instData->numChips && numImages > 0) {
            emit messageAvailable("DATA: " + QString::number(numImages) + " found, expected "+QString::number(instData->numChips), "warning");
            emit warning();
        }
        dataInitialized = true;
        return;
    }

    // Fill the list of MyImage types of individual images and master calibration files in this data directory
    QStringList filter;
    filter << "*.fits";
    QStringList allFitsFiles = dir.entryList(filter);
    numImages = allFitsFiles.length();
    numMasterCalibs = 0;
    // The master calibration FITS files (if any).          (i.e. if we are reading a calibration directory)
    for (int chip=0; chip<instData->numChips; ++chip) {
        QStringList filter;
        filter << subDirName+"_"+QString::number(chip+1)+".fits";
        QStringList fitsFiles = dir.entryList(filter);
        if (!fitsFiles.isEmpty()) {
            // Only one entry in this QStringList because it is the master
            MyImage *myImage = new MyImage(dirName, fitsFiles.at(0), "", chip+1, mask->globalMask[chip], verbosity);
            myImage->setParent(this);
            myImage->imageOnDrive = true;
            combinedImage[chip] = myImage;
            ++numMasterCalibs;
            connect(myImage, &MyImage::modelUpdateNeeded, this, &Data::modelUpdateReceiver);
            connect(myImage, &MyImage::critical, this, &Data::pushCritical);
            connect(myImage, &MyImage::warning, this, &Data::pushWarning);
            connect(myImage, &MyImage::messageAvailable, this, &Data::pushMessageAvailable);
            connect(myImage, &MyImage::setMemoryLock, this, &Data::setMemoryLockReceived, Qt::DirectConnection);
            connect(myImage, &MyImage::setWCSLock, this, &Data::setWCSLockReceived, Qt::DirectConnection);
        }
    }
    if (numMasterCalibs == instData->numChips) {
        hasAllMasterCalibs = true;
        dataInitialized = true;
    }
    if (numMasterCalibs < instData->numChips && numMasterCalibs > 0) {
        emit messageAvailable("DATA: " + QString::number(numMasterCalibs) + " found, expected "+QString::number(instData->numChips), "warning");
    }

    // The other images (minus master calibs etc), i.e. the science exposures

    numImages = 0;
    for (int chip=0; chip<instData->numChips; ++chip) {
        QStringList filter;
        filter << "*_"+QString::number(chip+1)+processingStatus->statusString+".fits";
        QStringList fitsFiles = dir.entryList(filter);

        // TODO: when splitting data, we must use a filter that does not contain the _chip string (raw data)
        // if list == empty then reset string and reload
        for (auto &it : fitsFiles) {
            bool skip = false;
            // skip master calibs
            if (it == subDirName+"_"+QString::number(chip+1)+".fits") skip = true;
            if (skip) continue;
            MyImage *myImage = new MyImage(dirName, it, processingStatus->statusString, chip+1, mask->globalMask[chip], verbosity);
            myImage->setParent(this);
            myImage->readFILTER(dirName+"/"+it);
            myImage->imageOnDrive = true;
            myImage->pathBackupL1 = pathBackupL1;
            myImage->baseNameBackupL1 = myImage->chipName + backupStatus;
            QFile weight(myImage->weightPath+"/"+myImage->weightName+".fits");
            if (weight.exists()) myImage->weightOnDrive = true;
            myImageList[chip].append(myImage);
            if (!uniqueChips.contains(chip+1)) uniqueChips.push_back(chip+1);
            connect(myImage, &MyImage::modelUpdateNeeded, this, &Data::modelUpdateReceiver);
            connect(myImage, &MyImage::critical, this, &Data::pushCritical);
            connect(myImage, &MyImage::warning, this, &Data::pushWarning);
            connect(myImage, &MyImage::messageAvailable, this, &Data::pushMessageAvailable);
            connect(myImage, &MyImage::setMemoryLock, this, &Data::setMemoryLockReceived, Qt::DirectConnection);
            connect(myImage, &MyImage::setWCSLock, this, &Data::setWCSLockReceived, Qt::DirectConnection);
        }
        numImages += myImageList[chip].length();
    }
    if (numImages > 0) dataInitialized = true;

    // Sort the vector with the chip numbers (no particular reason, yet)
    std::sort(uniqueChips.begin(), uniqueChips.end());

    // Parallelization is set externally by controller;

    // Listen to images
    //    establish_connections();
}

Data::~Data()
{
    for (auto &it: combinedImage) {
        delete it;
        it = nullptr;
    }

    deleteMyImageList();

    omp_destroy_lock(&progressLock);
}

// Upon launch, check if the status on record matches the FITS files on drive (using chip 1 as a reference)
bool Data::checkStatusConsistency()
{
    // only do this
    if (!processingStatus->doesStatusFileExist()) return true;

    if (dataType == "SCIENCE" || dataType == "SKY" || dataType == "STD") {
        QStringList expectedFileList = dir.entryList(QStringList() << "*_1"+processingStatus->statusString+".fits");
        QStringList observedFileList = dir.entryList(QStringList() << "*_1*.fits");

        // write found status to disk. Controller will then retry if necessary
        if (expectedFileList.isEmpty() && !observedFileList.isEmpty()) {
            processingStatus->inferStatusFromFilenames();
            processingStatus->statusToBoolean(processingStatus->statusString);
            processingStatus->writeToDrive();
            return false;
        }
    }
    return true;
}

bool Data::checkForRawData()
{
    // Only checking .fits files. Everything else ("*.fit", "*.cr2", "*.fz" etc) it is clear that we have raw data (or other files)
    QStringList filter = {"*.fits"};
    QStringList filter2 = {"*.fits", "*.fit", "*.FIT", "*.fz", "*.cr2", "*.CR2", "*.arw", "*.ARW", "*.dng", "*.DNG", "*.nef", "*.NEF"};
    dir.setFilter(QDir::Files);
    QStringList fileNames = dir.entryList(filter);          // Contains processed images and potential raw data
    QStringList allFileNames = dir.entryList(filter2);      // Everything
    int numRawFiles = 0;
    for (QString &allFileName : allFileNames) {
        if (!allFileName.endsWith(".fits")) ++numRawFiles;  // Does not matter whether imaging data or other stuff
    }

    int numProcessedFiles = 0;
    for (auto &fileName : fileNames) {
        fitsfile *fptr;
        int status = 0;
        QString name = dirName + "/" + fileName;
        fits_open_file(&fptr, name.toUtf8().data(), READONLY, &status);
        long thelipro = 0;
        fits_read_key_lng(fptr, "THELIPRO", &thelipro, nullptr, &status);
        if (status == KEY_NO_EXIST) {
            ++numRawFiles;
            status = 0;
        }
        else {
            ++numProcessedFiles;
        }
        fits_close_file(fptr, &status);
    }

    if (numProcessedFiles == 0 && numRawFiles > 0) {
        processingStatus->deleteFromDrive();
        processingStatus->reset();
        return true;
    }
    else if (numProcessedFiles == 0 && numRawFiles == 0) return true;
    else if (numProcessedFiles > 0 && numRawFiles == 0) return false;
    else {
        processingStatus->deleteFromDrive();
        processingStatus->reset();
        // TODO: this does not show!
        emit showMessageBox("DATA::RAW_AND_PROCESSED_FOUND", dirName, "");
        //        qDebug() << "Data::checkForRawData(): " << subDirName << numProcessedFiles << numRawFiles;
        //        qDebug() << "Both processed and RAW files were found. This status is not allowed";
        // (numProcessedFiles > 0 && numRawFiles > 0) {
        // TODO: this does not show!
        emit messageAvailable(dirName + " : Both processed and RAW files were found. This status is not allowed. You must clean the directory manually.", "error");
        emit critical();
        successProcessing = false;
        return false;
    }
}

void Data::broadcastNumberOfFiles()
{
    emit messageAvailable(subDirName + ": " + QString::number(numImages)+" images initialized", "data");
    if (dataType == "BIAS" || dataType == "DARK" || dataType == "FLATOFF" || dataType == "FLAT") {
        emit messageAvailable(subDirName + ": " + QString::number(numMasterCalibs)+" master "+dataType+" images initialized", "data");
    }
}

// Needed for aborting a task
void Data::setSuccess(bool state)
{
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : myImageList[chip]) {
            it->successProcessing = state;
        }
        successProcessing = state;
    }
}

void Data::writeUnsavedImagesToDrive(bool includeBackup)
{
    emit resetProgressBar();

    /*
     * TODO: Run it as a separate process
     * CRASHES, because processes are started by the controller, which then immediately calls the destructor.
     * Better if we can wait for the processes and then delete 'data', e.g. if handled by controller directly

    workerThread = new QThread(this);
    worker = new MyWorker(this);
    workerInit = true;
    workerThreadInit = true;

    worker->moveToThread(workerThread);
    QObject::connect(workerThread, &QThread::started, worker, &MyWorker::DataDumpImagesToDrive);
    QObject::connect(worker, &MyWorker::finished, worker, &QObject::deleteLater);
    QObject::connect(worker, &QObject::destroyed, workerThread, &QThread::quit);

    workerThread->start();
    workerThread->wait();
    */

    // freezes the GUI while running ...
    progressStepSize = 1. / float(numImages);

    emit resetProgressBar();
    populateExposureList();

#pragma omp parallel for num_threads(maxCPU)
    for (long i=0; i<exposureList.length(); ++i) {
        for (auto &it : exposureList[i]) {
            if (!it->imageOnDrive) {
                it->writeImage();
                it->imageOnDrive = true;
                it->emitModelUpdateNeeded();
                if (includeBackup) {
                    it->writeImageBackupL1();
                    it->writeImageBackupL2();
                    it->writeImageBackupL3();
                }
#pragma omp atomic
                *progress += progressStepSize;
            }
        }
    }

    emit progressUpdate(100.);
}

bool Data::checkTaskRepeatStatus(QString taskBasename)
{
    isTaskRepeated = false;

    // Check the Data class. If the current task matches the status, then the task is being repeated
    if (taskBasename == "HDUreformat" && processingStatus->HDUreformat) isTaskRepeated = true;
    else if (taskBasename == "Processscience" && processingStatus->Processscience) isTaskRepeated = true;
    else if (taskBasename == "Chopnod" && processingStatus->Chopnod) isTaskRepeated = true;
    else if (taskBasename == "Background" && processingStatus->Background) isTaskRepeated = true;
    else if (taskBasename == "Collapse" && processingStatus->Collapse) isTaskRepeated = true;
    else if (taskBasename == "Starflat" && processingStatus->Starflat) isTaskRepeated = true;
    else if (taskBasename == "Skysub" && processingStatus->Skysub) isTaskRepeated = true;

    // Check that all images have the same status
    myImageList[0][0]->checkTaskRepeatStatus(taskBasename);
    bool imageStatus = myImageList[0][0]->isTaskRepeated;
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : myImageList[chip]) {
            it->checkTaskRepeatStatus(taskBasename);
            if (imageStatus != it->isTaskRepeated) {
                emit messageAvailable(dirName + " : Data::checkTaskRepeatStatus(): Inconsistent processing status detected among images!<br>You must clean-up the data directory manually. Restart recommended.", "error");
                emit critical();
                it->successProcessing = false;
                successProcessing = false;
                return false;
            }
        }
    }

    // If the Data class status does not match the image status, then map the image status onto the Data class
    if (isTaskRepeated != imageStatus) {
        // Map the imaging status onto the Data structure
        emit messageAvailable(dirName + " : Data::checkTaskRepeatStatus(): Inconsistent processing status detected between images and Data class. Reset to image status", "warning");
        emit warning();
        processingStatus->statusToBoolean(myImageList[0][0]->processingStatus->statusString);
        processingStatus->getStatusString();
    }

    // Now check if we can actually retrieve all the data with the previous status
    bool success = true;
    if (isTaskRepeated) {
        for (int chip=0; chip<instData->numChips; ++chip) {
            for (auto &it : myImageList[chip]) {
                // If the image is not in memory, check if it is on disk
                if (!it->backupL1InMemory) {
                    QString fileName = it->pathBackupL1 + "/" + it->baseNameBackupL1 + ".fits";
                    QFile file(fileName);
                    if (it->backupL1OnDrive && !file.exists()) success = false;
                    else if (!it->backupL1OnDrive && !file.exists()) success = false;
                    else if (!it->backupL1OnDrive && file.exists()) {
                        it->backupL1OnDrive = true;
                        if (*verbosity > 2) emit messageAvailable(dirName + " : Data::checkTaskRepeatStatus(): Fixed wrong backup status flag, file was found on drive.", "warning");
                    }
                }
            }
        }

        if (!success) {
            emit showMessageBox("Data::BACKUP_DATA_NOT_FOUND", "","");
        }
    }

    return success;
}

void Data::checkPresenceOfMasterCalibs()
{
    // Check if master calibration FITS files are present
    int numMasterCalibs = 0;
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        QStringList filter;
        filter << subDirName+"_"+QString::number(chip+1)+".fits";
        QStringList fitsFiles = dir.entryList(filter);
        if (!fitsFiles.isEmpty()) ++numMasterCalibs;
    }
    if (numMasterCalibs == instData->numUsedChips) hasAllMasterCalibs = true;
    else hasAllMasterCalibs = false;
}


void Data::populateExposureList()
{
    if (*verbosity > 0) emit messageAvailable(subDirName + " : Collecting metadata from images ...", "data");
    // Create a list of unique MJDOBS
    QVector<double> mjdList;
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        for (auto &it : myImageList[chip]) {
            // TODO: this is only necessary if the GUI is launched and files have not been read yet!
            // There should be a member boolean that keeps track of this
            if (!it->headerInfoProvided) it->loadHeader();
            if (!mjdList.contains(it->mjdobs)) mjdList.append(it->mjdobs);
        }
    }
    // Sort list of mjdobs
    std::sort(mjdList.begin(), mjdList.end());

    exposureList.clear();
    exposureList.resize(mjdList.length());

    // Collect all MyImages that belong to one mjdobs
    long expNumber = 0;
    for (auto &mjdobs : mjdList) {
        for (int chip=0; chip<instData->numChips; ++chip) {
            if (instData->badChips.contains(chip)) continue;
            for (auto &it : myImageList[chip]) {
                if (it->mjdobs == mjdobs) exposureList[expNumber].append(it);
            }
        }
        ++expNumber;
    }
    emit appendOK();
}

void Data::resetGlobalWeight(QString filter)
{
    // CHECK: not sure whether i need to exclude badChips here
    if (myImageList.isEmpty()) return;
    for (int chip=0; chip<instData->numChips; ++chip) {
        int removeIndex = 0;
        int i = 0;
        bool remove = false;
        for (auto &it: myImageList[chip]) {
            if (filter == it->filter) {
                removeIndex = i;
                remove = true;
                break;
            }
            ++i;
        }
        if (remove) {
            myImageList[chip][removeIndex]->freeAll();
            myImageList[chip].removeAt(removeIndex);
        }
    }
}

void Data::loadCombinedImage(const int chip)
{
    if (!successProcessing) return;

    if (userStop || userKill) return;

    if (*verbosity > 0 && !combinedImage.at(chip)->imageInMemory) {
        emit messageAvailable("Chip " + QString::number(chip+1) + " : Loading master "+ subDirName + " ...", "data");
    }

    bool determineMode = false;

    // Must be threadsafe only if running process science as a first taskafter GUI re-launch
    combinedImage[chip]->readImageThreadSafe(determineMode);

    // Do we have the mode already (i.e. this function was executed previously for this chip)
    if (!combinedImage[chip]->modeDetermined) {
        // the mode determination will fail if we have mostly integer values (e.g. darks)
        if (dataType == "BIAS" || dataType == "DARK" || dataType == "FLATOFF") {
            // mode doesn't work, as all values are very very narrowly distributed around a single value (which might be integer on top of that)
            combinedImage[chip]->skyValue = meanMask(combinedImage[chip]->dataCurrent, mask->globalMask[chip]);
        }
        else {
            combinedImage[chip]->skyValue = modeMask(combinedImage[chip]->dataCurrent, "stable", mask->globalMask[chip])[0];
        }
        if (*verbosity > 0) {
            emit messageAvailable(subDirName + " : Master calib mean : "+QString::number(combinedImage[chip]->skyValue, 'f', 3), "data");
        }
    }
    combinedImage[chip]->modeDetermined = true;
    combinedImage[chip]->imageInMemory = true;

    successProcessing = combinedImage[chip]->successProcessing;
}

// Usually, the mode / SKYVALUE is computed when applying the master BIAS/FLAT
// However, a user might skip these steps, and then the mode remains undetermined
void Data::checkModeIsPresent()
{
    if (!successProcessing) return;

#pragma omp parallel for num_threads(maxCPU)
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : myImageList[chip]) {
            it->getMode(true);
        }
    }
}

void Data::resetSuccessProcessing()
{
    successProcessing = true;

    if (!dataInitialized) return;

    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : myImageList[chip]) {
            it->successProcessing = true;
        }
    }
}


// Used for creating master calibrators
void Data::combineImagesCalib(int chip, float (*combineFunction_ptr) (const QVector<float> &, const QVector<bool> &, long),
                              const QString nlowString, const QString nhighString, const QString dirName, const QString subDirName,
                              const QString dataType)
{
    if (!successProcessing) return;

    if (userStop || userKill) return;

    if (*verbosity > 0) emit messageAvailable(subDirName + " : Combining images for chip "+QString::number(chip+1)+" ...", "data");

    int nlow = nlowString.toInt();    // returns 0 for empty string (desired)
    int nhigh = nhighString.toInt();  // returns 0 for empty string (desired)
    int numImages = myImageList.at(chip).length();

    // Delete an old master calibration frame if it exists
    if (!deleteFile(subDirName+"_"+QString::number(chip+1)+".fits", dirName)) {
        emit messageAvailable(subDirName + " : Data::combineImgesCalib(): Could not delete old master calibration file!", "error");
        emit critical();
        successProcessing = false;
        return;
    }

    if (nhigh+nlow >= numImages) {
        emit messageAvailable("Number of low and high pixels rejected is equal or larger than the number of available images.", "error");
        emit critical();
        successProcessing = false;
        return;
    }

    if (numImages <= 3) {
        emit messageAvailable("Only "+QString::number(numImages)+" exposures are used to compute the combined master calibration file. More are recommended.", "warning");
        emit warning();
    }

    // Get image geometry from first image in list
    long n = myImageList.at(chip).at(0)->naxis1;
    long m = myImageList.at(chip).at(0)->naxis2;
    if (n == 0 || m == 0) {
        emit messageAvailable(subDirName + " : Data::combineImgesCalib(): Could not determine size of combined image.", "error");
        emit critical();
        successProcessing = false;
        return;
    }
    long dim = n*m;

    // Container for the temporary pixel stack
    // Instantiate a MyImage object for the combined set of images.
    // It does not create a FITS object yet.
    // Delete the instance if it exists already from a previous run of this task to not (re)create it
    //    if (combinedImage[chip] != nullptr) delete combinedImage[chip];

    if (combinedImage.at(chip) == nullptr) {
        MyImage *combinedImageDummy = new MyImage(dirName, subDirName+"_"+QString::number(chip+1)+".fits", "", chip+1,
                                                  mask->globalMask[chip], verbosity);
        connect(combinedImageDummy, &MyImage::modelUpdateNeeded, this, &Data::modelUpdateReceiver);
        connect(combinedImageDummy, &MyImage::critical, this, &Data::pushCritical);
        connect(combinedImageDummy, &MyImage::warning, this, &Data::pushWarning);
        connect(combinedImageDummy, &MyImage::messageAvailable, this, &Data::pushMessageAvailable);
        connect(combinedImageDummy, &MyImage::setMemoryLock, this, &Data::setMemoryLockReceived, Qt::DirectConnection);
        connect(combinedImageDummy, &MyImage::setWCSLock, this, &Data::setWCSLockReceived, Qt::DirectConnection);
        combinedImage[chip] = combinedImageDummy;
    }
    combinedImage[chip]->naxis1 = n;
    combinedImage[chip]->naxis2 = m;
    combinedImage[chip]->dataCurrent.resize(dim);
    combinedImage[chip]->dataCurrent.squeeze();    // shed excess memory

    // loop over all pixels, and images; rescaling is optional
    QVector<long> goodIndex;
    goodIndex.reserve(numImages);
    // Also checks whether the mode is within valid range (for calibrators):
    QVector<float> rescaleFactors = getNormalizedRescaleFactors(chip, goodIndex, "forCalibration");
    if (rescaleFactors.isEmpty()) {
        // we should never enter here. Error handling in getNormalizedRescaleFactors()
        successProcessing = false;
        return;
    }
    long ngood = goodIndex.length();

    if (nhigh+nlow >= ngood) {
        emit messageAvailable("The number of nlow and nhigh rejected pixels is equal to or larger than the number of input exposures. Using zero instead", "warning");
        nlow = 0;
        nhigh = 0;
    }

    QString goodImages;
    int k = 0;
    for (auto &gi : goodIndex) {
        goodImages.append(myImageList[chip][gi]->baseName + ": "
                          + QString::number(myImageList[chip][gi]->skyValue, 'f', 3) + " e-, rescaled with "
                          + QString::number(rescaleFactors[k], 'f', 3));
        if (k<goodIndex.length()-1) goodImages.append("<br>");
        ++k;
    }

    QString rescaled = "";
    if (!rescaleFlag) rescaled = ", without rescaling, ";
    if (*verbosity > 0) emit messageAvailable(subDirName + " : Calculating master "+dataType+" for chip "+QString::number(chip+1)
                                              + rescaled + " from : <br>"+goodImages, "image");
    if (*verbosity > 0) emit messageAvailable(subDirName + " : Median combination running ...", "data");

    // 45% of the progress counter is reserved for combining the images. We update the progress bar for every 10% of these 45%
    float localProgressStepSize = 0.45 / 10. / instData->numUsedChips * 100.;
    int progCount = 1;  // runs from 1 to 10;
    long progCountComparison = dim / 10;

    // works on dataCurrent
    dim = combinedImage.at(chip)->dataCurrent.length();
    // Crashed by stack.append(), but not when running through valgrind (no multi-threading)?
    // Update: does not crash anymore ... (at least for single-chip cameras)
    // TODO: more efficient with semaphore if numcpu and numchips divide with remainder
    //#pragma omp parallel for num_threads(maxCPU) if (instData->numChips == 1)

    //int localMaxThreads = maxCPU/instData->numChips;
    // if (instData->numChips > maxCPU) localMaxThreads = 1;
    //  QList<MyImage*> imglist = myImageList[chip];
    omp_set_nested(false);  // somehow not yet threadsafe!
    // we still parallelise though for single-chip cameras:
    int localMaxThreads = 1;
    // NOT THREADSAFE
    //    if (instData->numChips == 1) localMaxThreads = maxCPU;
#pragma omp parallel for num_threads(localMaxThreads) firstprivate(rescaleFactors) // firstprivate(imglist, goodIndex, rescaleFactors)
    for (long i=0; i<dim; ++i) {
        QVector<float> stack;
        stack.reserve(ngood);
        long k = 0;
        for (auto &gi : goodIndex) {
            // if clause: needed because objectmask can be empty and the lookup will segfault

            if (myImageList[chip][gi]->objectMaskDone) {
                if (!myImageList[chip][gi]->objectMask[i]) {
                    stack.append(myImageList.at(chip).at(gi)->dataCurrent.at(i) * rescaleFactors[k]);
                }
            }
            else {
                stack.append(myImageList.at(chip).at(gi)->dataCurrent.at(i) * rescaleFactors[k]);
            }

            /*
            if (imglist[gi]->objectMaskDone) {
                if (!imglist[gi]->objectMask[i]) {
                    stack.append(imglist[gi]->dataCurrent[i] * rescaleFactors[k]);
                }
            }
            else {
                stack.append(imglist[gi]->dataCurrent[i] * rescaleFactors[k]);
            }
            */

            ++k;
        }
        //        auto &stackedPixel = combinedImage[chip]->dataCurrent[pix];
        combinedImage[chip]->dataCurrent[i] = straightMedian_MinMax(stack, nlow, nhigh);

        // stackedPixel = combineFunction_ptr(stack, QVector<bool>(), ngood);

        // Increment the progressBar every 10 %
        if (i+1 == progCount * progCountComparison) {
#pragma omp atomic
            *progress += localProgressStepSize;
            //                emit addToProgressBar(localProgressStepSize);
            ++progCount;
        }
    }

    combinedImage[chip]->imageInMemory = true;
    successProcessing = true;
    combinedImage[chip]->emitModelUpdateNeeded();
}

void Data::resetStaticModel()
{
    staticModelDone.clear();
    staticModelDone.resize(instData->numChips);
    for (auto &it : staticModelDone) it = false;
}

// Used for creating a background model
void Data::combineImages(const int chip, const QString nlowString, const QString nhighString, const QString currentImage,
                         const QString mode, const QString dirName, const QString subDirName, QVector<bool> &dataStaticModelDone)
{
    if (!successProcessing) return;
    if (userStop || userKill) return;
    if (mode == "static" && dataStaticModelDone[chip]) return;

    QString rescaled = "";
    if (!rescaleFlag) rescaled = ", without rescaling, ";
    if (*verbosity > 0) emit messageAvailable(subDirName + " : Combining these images" +rescaled+" for " +currentImage + " : ", "data");

    // Get image geometry from first image in list that has its size measured
    long n = 0;
    long m = 0;
    for (auto &back : myImageList.at(chip)) {
        if (back->useForBackground) {
            n = back->naxis1;
            m = back->naxis2;
            break;
        }
    }
    if (n == 0 || m == 0) {
        emit messageAvailable(subDirName + " : Data::combineImges(): Could not determine size of combined image.", "error");
        emit critical();
        successProcessing = false;
        return;
    }
    long dim = n*m;

    int nlow = nlowString.toInt();    // returns 0 for empty string
    int nhigh = nhighString.toInt();  // returns 0 for empty string

    int numImages = myImageList.at(chip).length();

    if (nhigh+nlow >= numImages) {
        emit messageAvailable("Number of low and high pixels rejected is equal or larger than the number of available images.", "error");
        emit critical();
        successProcessing = false;
        return;
    }

    // Instantiate a MyImage object for the combined set of images. It does not create a FITS object yet.
    // if (combinedImage[chip] != nullptr) delete combinedImage[chip];
    if (combinedImage.at(chip) == nullptr) {       // re-using previously created object
        MyImage *masterCombined = new MyImage(dirName, currentImage, "", chip+1, mask->globalMask[chip], verbosity);
        connect(masterCombined, &MyImage::modelUpdateNeeded, this, &Data::modelUpdateReceiver);
        connect(masterCombined, &MyImage::critical, this, &Data::pushCritical);
        connect(masterCombined, &MyImage::warning, this, &Data::pushWarning);
        connect(masterCombined, &MyImage::messageAvailable, this, &Data::pushMessageAvailable);
        connect(masterCombined, &MyImage::setMemoryLock, this, &Data::setMemoryLockReceived, Qt::DirectConnection);
        connect(masterCombined, &MyImage::setWCSLock, this, &Data::setWCSLockReceived, Qt::DirectConnection);
        combinedImage[chip] = masterCombined;
    }
    combinedImage[chip]->naxis1 = n;
    combinedImage[chip]->naxis2 = m;
    combinedImage[chip]->dataCurrent.resize(dim);
    combinedImage[chip]->dataCurrent.squeeze();

    // loop over all pixels, and images; rescaling is optional
    QVector<long> goodIndex;
    goodIndex.reserve(numImages);
    QVector<float> rescaleFactors = getNormalizedRescaleFactors(chip, goodIndex, "forBackground");
    if (rescaleFactors.isEmpty()) {
        // we should never enter here. Error handling in getNormalizedRescaleFactors()
        successProcessing = false;
        return;
    }

    QString goodImages;
#pragma omp critical         // QString thread safety  it->chipname could be accessed simultaneously
    {
        int k = 0;
        for (auto &gi : goodIndex) {
            goodImages.append(myImageList[chip][gi]->chipName + ": "
                              + QString::number(myImageList[chip][gi]->skyValue, 'f', 3) + " e-, rescaled with "
                              + QString::number(rescaleFactors[k], 'f', 3));
            if (k<goodIndex.length()-1) goodImages.append("<br>");
            ++k;
        }
    }

    if (*verbosity > 0) emit messageAvailable(goodImages, "image");
    if (*verbosity > 0) emit messageAvailable(subDirName + " : Median combination running ...", "data");

    // works on dataBackupLx (?)

    long ngood = goodIndex.length();

    if (nhigh+nlow >= ngood) {
        emit messageAvailable("The number of nlow and nhigh rejected pixels is equal to or larger than the number of input exposures. Using zero instead", "warning");
        nlow = 0;
        nhigh = 0;
    }

    //    int localMaxThreads = maxCPU/instData->numChips;
    //    if (instData->numChips > maxCPU) localMaxThreads = 1;

    if (instData->numChips > 1) {
        QList<float> stack;
        // Not thread safe
        //    long ngood = goodIndex.length();
        //    stack.reserve(ngood);
        // #pragma omp parallel for num_threads(localMaxThreads)
        for (long i=0; i<dim; ++i) {
            // not thread safe
            //        QVector<float> stack;
            //        stack.reserve(ngood);
            long k = 0;
            for (auto &gi : goodIndex) {
                if (myImageList[chip][gi]->objectMaskDone) {         // needed because objectmask can be empty and the lookup will segfault
                    if (!myImageList[chip][gi]->objectMask[i]) {
                        stack.append(myImageList.at(chip).at(gi)->dataBackupL1.at(i) * rescaleFactors[k]);
                    }
                }
                else {
                    stack.append(myImageList.at(chip).at(gi)->dataBackupL1.at(i) * rescaleFactors[k]);
                }
                ++k;
            }
            combinedImage[chip]->dataCurrent[i] = straightMedian_MinMax(stack, nlow, nhigh);
            stack.clear();
        }
    }

    // Single chip: we can use inner parallelization!
    // UPDATE: identical to the section above, just 'stack' is declared inside the for loop and not outside
    else {
        // NOPE! we cannot, still crashing sometimes
        // #pragma omp parallel for num_threads(localMaxThreads) firstprivate(rescaleFactors)
        for (long i=0; i<dim; ++i) {
            QList<float> stack;
            long k = 0;
            for (auto &gi : goodIndex) {
                if (myImageList[chip][gi]->objectMaskDone) {         // needed because objectmask can be empty and the lookup will segfault
                    if (!myImageList[chip][gi]->objectMask[i]) {
                        stack.append(myImageList.at(chip).at(gi)->dataBackupL1.at(i) * rescaleFactors[k]);
                    }
                }
                else {
                    stack.append(myImageList.at(chip).at(gi)->dataBackupL1.at(i) * rescaleFactors[k]);
                }
                ++k;
            }
            combinedImage[chip]->dataCurrent[i] = straightMedian_MinMax(stack, nlow, nhigh);
        }
    }
    if (mode == "static") dataStaticModelDone[chip] = true;

    combinedImage[chip]->imageInMemory = true;
    successProcessing = true;
}

/*
// Used for creating a background model
void Data::combineImages_newParallel(int chip, MyImage *masterCombined, QList<MyImage*> &backgroundList, QString nlowString,
                                     QString nhighString, QString currentImage, QString mode, const QString subDirName)
{
    if (!successProcessing) return;

    if (userStop || userKill) return;

    if (mode == "static" && staticModelDone[chip]) return;

    masterCombined->setBackgroundLock(true);

    QString rescaled = "";
    if (!rescaleFlag) rescaled = ", without rescaling, ";
    if (*verbosity > 0) emit messageAvailable(subDirName + " : Combining these images" +rescaled+" for " +currentImage + " : ", "data");

    int nlow = nlowString.toInt();    // returns 0 for empty string (desired)
    int nhigh = nhighString.toInt();  // returns 0 for empty string (desired)

    // Get image geometry from first image in list that has its size measured
    long n = 0;
    long m = 0;
    for (auto &back : backgroundList) {
        if (back->useForBackground) {
            n = back->naxis1;
            m = back->naxis2;
            break;
        }
    }
    if (n == 0 || m == 0) {
        emit messageAvailable(subDirName + " : Data::combineImges(): Could not determine size of combined image.", "error");
        emit critical();
        successProcessing = false;
        return;
    }
    long dim = n*m;

    // Container for the temporary pixel stack
    int numImages = backgroundList.length();
    // Instantiate a MyImage object for the combined set of images.
    // It does not create a FITS object yet.
    // Delete the instance if it exists already from a previous run of this task to not (re)create it
    // if (combinedImage[chip] != nullptr) delete combinedImage[chip];

    connect(masterCombined, &MyImage::modelUpdateNeeded, this, &Data::modelUpdateReceiver);
    connect(masterCombined, &MyImage::critical, this, &Data::pushCritical);
    connect(masterCombined, &MyImage::warning, this, &Data::pushWarning);
    connect(masterCombined, &MyImage::messageAvailable, this, &Data::pushMessageAvailable);
    connect(masterCombined, &MyImage::setMemoryLock, this, &Data::setMemoryLockReceived, Qt::DirectConnection);
    connect(masterCombined, &MyImage::setWCSLock, this, &Data::setWCSLockReceived, Qt::DirectConnection);
    masterCombined->naxis1 = n;
    masterCombined->naxis2 = m;
    masterCombined->dataCurrent.resize(dim);
    masterCombined->dataCurrent.squeeze();

    // loop over all pixels, and images; rescaling is optional
    QVector<long> goodIndex;
    goodIndex.reserve(numImages);
    // Also checks whether the mode is within valid range (for calibrators):
    QVector<float> rescaleFactors = getNormalizedRescaleFactors(chip, goodIndex, "forBackground");
    if (rescaleFactors.isEmpty()) {
        // we should never enter here. Error handling in getNormalizedRescaleFactors()
        successProcessing = false;
        return;
    }

    QString goodImages;
    int k = 0;
    for (auto &gi : goodIndex) {
        MyImage *it = backgroundList[gi];
        goodImages.append(it->chipName + ": " + QString::number(it->skyValue,'f',3) + " e-, rescaled with "
                          + QString::number(rescaleFactors[k],'f',3));
        if (k<goodIndex.length()-1) goodImages.append("<br>");
        ++k;
    }

    if (*verbosity > 0) emit messageAvailable(goodImages, "image");
    if (*verbosity > 0) emit messageAvailable(subDirName + " : Median combination running ...", "data");

    // works on dataBackupLx (?)

    dim = masterCombined->dataCurrent.length();
    //    int localMaxThreads = maxCPU/instData->numChips;
    //    if (instData->numChips > maxCPU) localMaxThreads = 1;

    // for some unknown reason this parallelization results in a massive memory chaos. somewhere, something is overflowing and I just can't figure out where.
    QList<float> stack;
    //    long ngood = goodIndex.length();
    //    stack.reserve(ngood);
    // #pragma omp parallel for num_threads(localMaxThreads)
    for (long i=0; i<dim; ++i) {
        //        QVector<float> stack;
        //        stack.reserve(ngood);
        long k = 0;
        for (auto &gi : goodIndex) {
            // Not sure why I have included the requirement on dataBackupL1[i] != 0.
            // For flat-fielded data (not sky subtracted), the pixel values should always be positive. Commented out for the time being
//            if (!backgroundList[gi]->objectMask[i] && backgroundList[gi]->dataBackupL1[i] > 0.) {
//                    stack.append(backgroundList[gi]->dataBackupL1[i] * rescaleFactors[k]);
//                }
//            }
//            else {
//                if (backgroundList[gi]->dataBackupL1[i] > 0.) {
//                    stack.append(backgroundList[gi]->dataBackupL1[i] * rescaleFactors[k]);
//                }
//            }

            // Crash caused by dataBackgroundL1. Stack, rescaleFactors, backgroundList[gi] MyImages are all fine.
            // It appears to be the actual data vectors, but the debugger shows that everything gets mixed up.
            if (backgroundList[gi]->objectMaskDone) {         // needed because objectmask can be empty and the lookup will segfault
                if (!backgroundList[gi]->objectMask[i]) {
                    stack.append(backgroundList[gi]->dataBackupL1[i] * rescaleFactors[k]);
                }
            }
            else {
                stack.append(backgroundList[gi]->dataBackupL1[i] * rescaleFactors[k]);
            }

            ++k;
        }
        masterCombined->dataCurrent[i] = straightMedian_MinMax(stack, nlow, nhigh);
        stack.clear();
        // stackedPixel = combineFunction_ptr(stack, QVector<bool>(), ngood);
    }

    masterCombined->imageInMemory = true;
    successProcessing = true;
    masterCombined->setBackgroundLock(false);
}
*/

void Data::getModeCombineImages(int chip)
{
    if (!successProcessing) return;

    if (userStop || userKill) return;

    if (combinedImage[chip]->modeDetermined) return;

    // Get the mean / mode of the combined image
    if (dataType == "BIAS" || dataType == "DARK" || dataType == "FLATOFF") {
        // mode doesn't work, as all values are very very narrowly distributed around a single value (which might be integer on top of that)
        combinedImage[chip]->skyValue = meanMask(combinedImage[chip]->dataCurrent, mask->globalMask[chip]);
        //       if (*verbosity > 0) emit messageAvailable("Mean of chip "+QString::number(chip+1) + " for master "+dataType + " : "
        //                                                + QString::number(combinedImage[chip]->skyValue) + " e-", "data");
    }
    else {
        combinedImage[chip]->skyValue = modeMask(combinedImage[chip]->dataCurrent, "stable", mask->globalMask[chip])[0];
        //       if (*verbosity > 0) emit messageAvailable("Mode of chip "+QString::number(chip+1) + " for master "+dataType + " : "
        //                                                + QString::number(combinedImage[chip]->skyValue) + " e-", "data");
    }
    combinedImage[chip]->modeDetermined = true;

    successProcessing = true;
}

void Data::reportModeCombineImages()
{
    // Report the mean / mode of the combined image
    QString report = "";
    if (dataType == "BIAS" || dataType == "DARK" || dataType == "FLATOFF") {
        report = "Statistics (mean) for master "+dataType+" : <br>";
    }
    else {
        report = "Statistics (mode) for master "+dataType+" : <br>";
    }
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (!successProcessing) {
            if (!userStop && !userKill) {
                report.append("Chip " + QString::number(chip+1) + " : failed<br>");
            }
        }
        else {
            if (instData->badChips.contains(chip)) {
                report.append("Chip " + QString::number(chip+1) + " : Bad detector, skipped.<br>");
            }
            else {
                report.append("Chip " + QString::number(chip+1) + " : "
                              + QString::number(combinedImage[chip]->skyValue, 'f', 3) + " e-<br>");
            }
        }
    }
    emit messageAvailable(report, "data");
}

QVector<float> Data::getNormalizedRescaleFactors(int chip, QVector<long> &goodIndex, QString mode)
{
    long nImages = myImageList[chip].length();
    QVector<float> rescaleFactors;
    rescaleFactors.reserve(nImages);
    goodIndex.reserve(nImages);
    if (userStop || userKill) return rescaleFactors;

    if (mode == "forBackground") {
        for (auto &it: myImageList[chip]) {
            if (it->useForBackground && !it->modeDetermined) {
                emit messageAvailable("Data::getNormalizedRescaleFactors(): Mode should have been determined in taskInternal()!", "error");
                emit critical();
                successProcessing = false;
                return rescaleFactors;  // empty
            }
        }
    }

    if (!rescaleFlag) {
        // Combining a BIAS or DARK
        long j=0;
        for (auto &it : myImageList.at(chip)) {
            if (it->validMode) {
                rescaleFactors.append(1.0);
                goodIndex.append(j);
            }
            else {
                if (*verbosity > 0) emit messageAvailable(it->chipName + " : Not used, mode outside user limits : "
                                                          + QString::number(it->skyValue), "warning");
            }
            ++j;
        }
        return rescaleFactors;
    }

    else if (mode == "forCalibration") {
        // Combining a FLAT
        long j=0;
        for (auto &it : myImageList.at(chip) ) {
            if (it->validMode) {
                rescaleFactors.append(it->skyValue);
                goodIndex.append(j);
            }
            else {
                if (*verbosity > 0) emit messageAvailable(it->chipName + " : Not used, mode outside user limits : "
                                                          + QString::number(it->skyValue), "warning");
            }
            ++j;
        }
    }

    else if (mode == "forBackground") {
        // Combining a SCIENCE / SKY image
        long j=0;
        for (auto &it : myImageList.at(chip) ) {
            if (it->useForBackground) {
                rescaleFactors.append(it->skyValue);
                goodIndex.append(j);
            }
            ++j;
        }
    }

    else {
        emit messageAvailable("Data::getNormalizedRescaleFactors(): Invalid operating mode: " + mode, "error");
        emit critical();
    }

    // Rescale modes relative to mean mode
    float meanMode = meanMask(rescaleFactors);
    for (auto &it : rescaleFactors) it = meanMode / it;

    return rescaleFactors;
}

// copy the master flat field, threshold it
void Data::initGlobalWeight(int chip, Data *flatData, QString filter, bool sameWeight, QString flatMin, QString flatMax)
{
    if (!successProcessing) return;

    //    myImageList[chip].clear();

    QString globalWeightName = "globalweight_"+instData->name+"_"+filter+"_"+QString::number(chip+1)+".fits";
    MyImage *myImage = new MyImage(mainDirName, globalWeightName, "", chip+1, mask->globalMask[chip], verbosity);
    //    myImage->setParent(this);
    myImage->filter = filter;
    myImage->path = mainDirName+"/GLOBALWEIGHTS/";
    myImage->naxis1 = instData->sizex[chip];
    myImage->naxis2 = instData->sizey[chip];
    connect(myImage, &MyImage::modelUpdateNeeded, this, &Data::modelUpdateReceiver);
    connect(myImage, &MyImage::critical, this, &Data::pushCritical);
    connect(myImage, &MyImage::warning, this, &Data::pushWarning);
    connect(myImage, &MyImage::messageAvailable, this, &Data::pushMessageAvailable);
    connect(myImage, &MyImage::setMemoryLock, this, &Data::setMemoryLockReceived, Qt::DirectConnection);
    connect(myImage, &MyImage::setWCSLock, this, &Data::setWCSLockReceived, Qt::DirectConnection);

    // Potential masking
    long i=0;
    float flatMinVal = 0.;
    float flatMaxVal = 1.e9;
    QString thresholds = ", no thresholding";
    if (!flatMin.isEmpty() && !flatMax.isEmpty()) {
        flatMinVal = flatMin.toFloat();
        flatMaxVal = flatMax.toFloat();
        thresholds = ", thresholds = ["+flatMin+", "+flatMax+"]";
    }
    else if (!flatMin.isEmpty()) {
        flatMinVal = flatMin.toFloat();
        thresholds = ", thresholds = ["+flatMin+", unlimited]";
    }
    else if (!flatMax.isEmpty()) {
        flatMaxVal = flatMax.toFloat();
        thresholds = ", thresholds = [unlimited, "+flatMax+"]";
    }

    // Initiate with the flat field, or unity everywhere
    if (!sameWeight) {
        if (flatData == nullptr) {
            emit messageAvailable("No FLAT data defined. Initializing globalweight for chip " + QString::number(chip+1) + " with constant value 1.0", "warning");
            emit warning();
            long dim = instData->sizex[chip] * instData->sizey[chip];
            myImage->dataCurrent.fill(1.0, dim);
        }
        else {
            if (flatData->successProcessing) {
                if (*verbosity > 0) emit messageAvailable("Initializing globalweight for chip " + QString::number(chip+1) + " from master flat"+thresholds, "data");
                // done before calling this function
                //               if (!flatData->combinedImage[chip]->imageInMemory) flatData->combinedImage[chip]->setupDataInMemorySimple(true);
                myImage->dataCurrent = flatData->combinedImage[chip]->dataCurrent;  // shallow copy
            }
        }
    }
    else {
        if (*verbosity > 0) emit messageAvailable("Initializing globalweight for chip " + QString::number(chip+1) + " with constant value 1.0", "data");
        long dim = instData->sizex[chip] * instData->sizey[chip];
        myImage->dataCurrent.fill(1.0, dim);
        myImage->dataCurrent.resize(dim);      // CHECK: do we still need that given the previous line?
        myImage->dataCurrent.squeeze();        // shed excess memory
    }

    for (auto &it: myImage->dataCurrent) {
        if (it < flatMinVal
                || it > flatMaxVal
                || mask->globalMask[chip][i]) {
            it = 0.;
        }
        ++i;
    }

    myImage->imageInMemory = true;

    myImageList[chip].append(myImage);
}

// Threshold the global weight based on clipping values for the current combined image
void Data::thresholdGlobalWeight(int chip, const Data *comparisonData, const QString filter, const QString threshMin, const QString threshMax)
{
    if (!successProcessing) return;

    if (comparisonData == nullptr) return;
    if (!comparisonData->successProcessing) return;
    if (threshMin.isEmpty() && threshMax.isEmpty()) return;
    if (userStop || userKill) return;

    float threshMinVal = -1.e9;
    float threshMaxVal = 1.e9;

    QString thresholds = "";
    if (!threshMin.isEmpty() && !threshMax.isEmpty()) {
        threshMinVal = threshMin.toFloat();
        threshMaxVal = threshMax.toFloat();
        thresholds = "[threshMin, threshMax]";
    }
    else if (!threshMin.isEmpty()) {
        threshMinVal = threshMin.toFloat();
        thresholds = "["+threshMin+", unlimited]";
    }
    else if (!threshMax.isEmpty()) {
        threshMaxVal = threshMax.toFloat();
        thresholds = "[unlimited, "+threshMax+"]";
    }

    if (*verbosity > 0) emit messageAvailable("Thresholding chip "+QString::number(chip+1)+ " of globalweight, "
                                              +comparisonData->dataType + " must be within "+ thresholds, "data");

    for (auto &it: myImageList[chip]) {
        // Only process global weights that match the current science filter
        if (filter == it->filter) {
            // done before calling this function
            //            if (!comparisonData->combinedImage[chip]->imageInMemory) comparisonData->combinedImage[chip]->setupDataInMemorySimple(true);
            long k = 0;
            for (auto &jt: it->dataCurrent) {
                float compVal = comparisonData->combinedImage[chip]->dataCurrent[k];
                if (compVal < threshMinVal || compVal > threshMaxVal) {
                    jt = 0.;
                }
                ++k;
            }
        }
    }
}

void Data::detectDefects(int chip, Data *comparisonData, const QString filter, const bool sameWeight,
                         const QString defectKernel, const QString defectRowTol, const QString defectColTol, const QString defectClusTol)
{
    if (!successProcessing) return;

    if (sameWeight) return;
    if (defectKernel.isEmpty()) return;
    if (defectRowTol.isEmpty() && defectColTol.isEmpty() && defectClusTol.isEmpty()) return;
    if (!comparisonData->successProcessing) return;
    if (userStop || userKill) return;

    QString params = "kernel size = "+defectKernel;
    if (!defectClusTol.isEmpty()) params += (" clusTol = "+defectClusTol);
    if (!defectRowTol.isEmpty()) params += (" rowTol = "+defectRowTol);
    if (!defectColTol.isEmpty()) params += (" colTol = "+defectColTol);

    if (*verbosity > 0) emit messageAvailable("Defect detection for chip "+QString::number(chip+1)+": "+params, "data");

    // Smooth the flatfield
    int filtersize = defectKernel.toInt();
    QString splinemode = "spline";
    comparisonData->combinedImage[chip]->backgroundModel(filtersize, splinemode);

    // Normalize the flat field by the smoothed image
    long n = comparisonData->combinedImage.at(chip)->naxis1;
    long m = comparisonData->combinedImage.at(chip)->naxis2;
    QVector<float> divImage(n*m);
    long i = 0;
    for (auto &it : divImage) {
        it = comparisonData->combinedImage.at(chip)->dataCurrent.at(i) / comparisonData->combinedImage.at(chip)->dataBackground.at(i);
        ++i;
    }
    comparisonData->combinedImage[chip]->releaseBackgroundMemory();

    // Bad clusters
    if (!defectClusTol.isEmpty()) {
        for (auto &gw: myImageList[chip]) {
            // Only process global weights that match the current science filter
            if (filter == gw->filter) {
                i = 0;
                for (auto &it : divImage) {
                    if ( fabs(it-1.0) > defectClusTol.toFloat()) gw->dataCurrent[i] = 0.;
                    ++i;
                }
            }
        }
    }

    // Bad rows
    float kappa = 1.5;

    if (!defectRowTol.isEmpty()) {
        QVector<float> column1D = collapse_x(divImage, comparisonData->combinedImage[chip]->globalMask,
                                             comparisonData->combinedImage[chip]->objectMask, kappa, n, m, "1Dmodel");
        for (auto &gw: myImageList[chip]) {
            // Only process global weights that match the current science filter
            if (filter == gw->filter) {
                long badIndex = 0;
                // Set all rows to zero where the row index in column1D is outside the thresholds
                for (auto &it : column1D) {
                    if ( fabs(it-1.0) > defectRowTol.toFloat()) {
                        for (long i=0; i<n; ++i) gw->dataCurrent[i+n*badIndex] = 0.;
                    }
                    ++badIndex;
                }
            }
        }
    }

    // Bad cols
    if (!defectColTol.isEmpty()) {
        QVector<float> row1D = collapse_y(divImage, comparisonData->combinedImage[chip]->globalMask,
                                          comparisonData->combinedImage[chip]->objectMask, kappa, n, m, "1Dmodel");
        for (auto &gw: myImageList[chip]) {
            // Only process global weights that match the current science filter
            if (filter == gw->filter) {
                long badIndex = 0;
                // Set all columns to zero where the column index in row1D is outside the thresholds
                for (auto &it : row1D) {
                    if ( fabs(it-1.0) > defectColTol.toFloat()) {
                        for (long j=0; j<m; ++j) gw->dataCurrent[badIndex+n*j] = 0.;
                    }
                    ++badIndex;
                }
            }
        }
    }
}

void Data::applyMask(int chip, QString filter)
{
    for (auto &it: myImageList[chip]) {
        // Only process global weights that match the current science filter
        if (filter == it->filter) {
            long i = 0;
            for (auto &jt: it->dataCurrent) {
                if (mask->globalMask.at(chip).at(i)) {
                    jt = 0.;
                }
                ++i;
            }
        }
    }
}

void Data::writeGlobalWeights(int chip, QString filter)
{
    if (!successProcessing) return;

    if (*verbosity > 0) emit messageAvailable("Writing globalweight for chip " + QString::number(chip+1), "data");

    if (instData->bayer.isEmpty()) {
        for (auto &gw: myImageList[chip]) {
            if (filter == gw->filter) {
                gw->writeImage(gw->path + "/" + gw->name, filter);
            }
        }
    }
    else {
        QString name;
        // WARNING! The following assumes that "B" is the filter of the first image in scienceData->myImageList.
        // The order in which we insert the images in processingCalibration is therefore important!
        for (auto &gw: myImageList[chip]) {
            // Create 3 FITS files
            name = "globalweight_"+instData->name+"_B_"+QString::number(chip+1)+".fits";
            gw->writeImage(gw->path + "/" + name, "B");
            name = "globalweight_"+instData->name+"_G_"+QString::number(chip+1)+".fits";
            gw->writeImage(gw->path + "/" + name, "G");
            name = "globalweight_"+instData->name+"_R_"+QString::number(chip+1)+".fits";
            gw->writeImage(gw->path + "/" + name, "R");
        }
        // Duplicate the gw image twice in myImageList:
        if (myImageList[chip].length() == 1) {
            MyImage *gw2 = new MyImage(mainDirName+"/GLOBALWEIGHTS/",
                                       "globalweight_"+instData->name+"_G_"+QString::number(chip+1)+".fits",
                                       "", chip+1, mask->globalMask[chip], verbosity);
            gw2->filter = "G";
            gw2->dataCurrent = myImageList[chip][0]->dataCurrent;
            connect(gw2, &MyImage::modelUpdateNeeded, this, &Data::modelUpdateReceiver);
            connect(gw2, &MyImage::critical, this, &Data::pushCritical);
            connect(gw2, &MyImage::warning, this, &Data::pushWarning);
            connect(gw2, &MyImage::messageAvailable, this, &Data::pushMessageAvailable);
            connect(gw2, &MyImage::setMemoryLock, this, &Data::setMemoryLockReceived, Qt::DirectConnection);
            connect(gw2, &MyImage::setWCSLock, this, &Data::setWCSLockReceived, Qt::DirectConnection);
            MyImage *gw3 = new MyImage(mainDirName+"/GLOBALWEIGHTS/",
                                       "globalweight_"+instData->name+"_R_"+QString::number(chip+1)+".fits",
                                       "", chip+1, mask->globalMask[chip], verbosity);
            gw3->filter = "R";
            gw3->dataCurrent = myImageList[chip][0]->dataCurrent;
            connect(gw3, &MyImage::modelUpdateNeeded, this, &Data::modelUpdateReceiver);
            connect(gw3, &MyImage::critical, this, &Data::pushCritical);
            connect(gw3, &MyImage::warning, this, &Data::pushWarning);
            connect(gw3, &MyImage::messageAvailable, this, &Data::pushMessageAvailable);
            connect(gw3, &MyImage::setMemoryLock, this, &Data::setMemoryLockReceived, Qt::DirectConnection);
            connect(gw3, &MyImage::setWCSLock, this, &Data::setWCSLockReceived, Qt::DirectConnection);
            myImageList[chip].append(gw2);
            myImageList[chip].append(gw3);
        }
    }
}

void Data::getGainNormalization()
{
    if (!successProcessing) return;

    if (userStop || userKill) return;

    // Calculate the gain normalization factor.
    // The gains are normalized to the chip with the lowest effective gain
    // (the one with brightest image in a FLAT)
    QVector<float> gainNormalization;
    QVector<float> tmpNormalizationData;
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) {
            gainNormalization << 1.0;   // a dummy value to maintain the vector's structure (need one entry per chip)
            continue;
        }
        if (combinedImage[chip]->modeDetermined) {
            gainNormalization << combinedImage[chip]->skyValue;
            tmpNormalizationData << combinedImage[chip]->skyValue;
        }
        else {
            emit messageAvailable("Controller::getGainNormalization(): mode was not determined in flat!", "error");
            emit critical();
            successProcessing = false;
            return;
        }
    }
    float maxVal = maxVec_T(gainNormalization);
    if (*verbosity > 0 && instData->numChips>1) emit messageAvailable("Gain normalization factors (multi-chip cameras):", "data");
    for (int chip=0; chip<instData->numChips; ++chip) {
        QString space = " ";
        if (chip > 9) space = "";
        if (instData->badChips.contains(chip)) {
            if (*verbosity > 0) emit messageAvailable("Chip "+QString::number(chip+1) + space + " : Bad detector, skipped.", "ignore");
            continue;
        }
        gainNormalization[chip] /= maxVal;
        combinedImage[chip]->gainNormalization = gainNormalization[chip];
        if (*verbosity > 0) emit messageAvailable("Chip "+QString::number(chip+1) + space + " : "
                                                  + QString::number(gainNormalization[chip], 'f', 6), "ignore");
    }
}


void Data::protectMemory()
{
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        for (auto &it : myImageList[chip]) {
            it->protectMemory();
        }
        if (combinedImage[chip] != nullptr) {
            combinedImage[chip]->protectMemory();
        }
    }
}

void Data::unprotectMemory(int chip)
{
    for (auto &it : myImageList[chip]) {
        it->unprotectMemory();
    }
    if (combinedImage[chip] != nullptr) {
        combinedImage[chip]->unprotectMemory();
    }
}

void Data::unprotectMemoryForBackground(int chip)
{
    for (auto &it : myImageList[chip]) {
        it->unprotectMemoryForBackground();
    }
}

float Data::releaseMemory(float RAMneededThisThread, float RAMneededAllThreads, float currentTotalMemoryUsed, QString mode)
{
    // Return if we have enough RAM available
    //    qDebug() << RAMneededThisThread << RAMneededAllThreads << maxRAM << currentTotalMemoryUsed;
    if (RAMneededAllThreads < maxRAM - currentTotalMemoryUsed) return -1.;

    // Free RAM
    // Simply loop over data structure and free everything that is set to deletable, irrespective of chip, starting with lowest priority

    QStringList datalist;
    datalist << "dataBackground" << "dataBackupL3" << "dataBackupL2" << "dataBackupL1" << "dataWeight" << "dataCurrent";
    float RAMfreed = 0.;

    // We release memory in a different order, depending on the process:
    // If creating master calibrators, we release the previous master calibs and then the raw data.
    // If we calibrate the science data, we release the raw data in the calibrators first, and then the combined images
    if (mode == "calibrator") {
        releaseMemoryCombined(RAMfreed, RAMneededThisThread);
        releaseMemoryIndividual(datalist, RAMfreed, RAMneededThisThread);
    }
    else {
        releaseMemoryIndividual(datalist, RAMfreed, RAMneededThisThread);
        releaseMemoryDebayer(RAMfreed, RAMneededThisThread);
        releaseMemoryCombined(RAMfreed, RAMneededThisThread);
    }

    return RAMfreed;
}

void Data::releaseAllMemory()
{
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : myImageList[chip]) {
            if (it == nullptr) continue;
            else it->freeData("all");
        }
        if (combinedImage[chip] == nullptr) continue;
        else combinedImage[chip]->freeData("all");
    }
    emit globalModelUpdateNeeded();
}

void Data::releaseMemoryIndividual(const QStringList &datalist, float &RAMfreed, const float RAMneededThisThread)
{
    for (auto &datatype : datalist) {
        for (int chip=0; chip<instData->numChips; ++chip) {
            for (auto &it : myImageList[chip]) {
                RAMfreed += it->freeData(datatype);
                if (RAMfreed > RAMneededThisThread) break;
            }
            if (RAMfreed > RAMneededThisThread) break;
        }
        if (RAMfreed > RAMneededThisThread) break;
    }
}

void Data::releaseMemoryDebayer(float &RAMfreed, const float RAMneededThisThread)
{
    if (!currentlyDebayering) return;
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : bayerList[chip]) {
            RAMfreed += it->freeData("dataCurrent");
            if (RAMfreed > RAMneededThisThread) break;
        }
        if (RAMfreed > RAMneededThisThread) break;
    }
}

void Data::releaseMemoryCombined(float &RAMfreed, const float RAMneededThisThread)
{
    // Release memory occupied by combined images
    for (int chip=0; chip<instData->numChips; ++chip) {
        //        if (dataType == "BIAS" || dataType == "DARK" || dataType == "FLATOFF" || dataType == "FLAT") {
        // If executed without data structure having loaded a combined image (or if it wasn't created yet)
        // it would crash, because none of the class members were instantiated
        if (combinedImage[chip] == nullptr) continue;
        RAMfreed += combinedImage[chip]->freeData("dataBackground");
        RAMfreed += combinedImage[chip]->freeData("dataBackupL1");
        RAMfreed += combinedImage[chip]->freeData("dataCurrent");
        //        }
        if (RAMfreed > RAMneededThisThread) break;
    }
}

void Data::setMemoryLockReceived(bool locked)
{
    emit setMemoryLock(locked);
}

void Data::setWCSLockReceived(bool locked)
{
    emit setWCSLock(locked);
}

// Set which data can (or cannot) be deleted from memory
void Data::memorySetDeletable(int chip, QString dataX, bool deletable)
{
    if (!successProcessing) return;

    if (*verbosity > 1) emit messageAvailable(dataX + " for chip " + QString::number(chip+1) + " marked 'deletable'", "data");
    for (auto &it : myImageList[chip]) {
        if (dataX == "dataCurrent") it->dataCurrent_deletable = deletable;
        else if (dataX == "dataBackupL1") it->dataBackupL1_deletable = deletable;
        else if (dataX == "dataBackupL2") it->dataBackupL2_deletable = deletable;
        else if (dataX == "dataBackupL3") it->dataBackupL3_deletable = deletable;
        else if (dataX == "dataWeight") it->dataWeight_deletable = deletable;
    }

    successProcessing = true;
}

void Data::writeCombinedImage(int chip)
{
    if (!successProcessing) return;
    if (userStop || userKill) return;

    QString name = dirName+"/"+subDirName+"_"+QString::number(chip+1)+".fits";
    float exptime = myImageList[chip][0]->exptime;
    QString filter = myImageList[chip][0]->filter;

    bool addGainNormalization = true;
    if (*verbosity > 0) emit messageAvailable("Writing "+subDirName+"_"+QString::number(chip+1)+".fits", "data");
    combinedImage[chip]->writeImage(name, filter, exptime, addGainNormalization);
    successProcessing = combinedImage[chip]->successProcessing;
}

void Data::writeBackgroundModel(const int &chip, const QString &mode, const QString &basename, bool &staticImageWritten)
{
    if (!successProcessing) return;
    if (userStop || userKill) return;
    if (mode == "static" && staticImageWritten) return;

    QDir backgroundDir(dirName+"/BACKGROUND/");
    backgroundDir.mkdir(dirName+"/BACKGROUND/");

    float exptime = myImageList[chip][0]->exptime;
    QString filter = myImageList[chip][0]->filter;
    bool addGainNormalization = false;

    QString shortName;
    if (mode == "static") shortName = subDirName+"/BACKGROUND/"+subDirName+"_"+QString::number(chip+1)+".fits";
    else shortName = subDirName+"/BACKGROUND/"+basename+".back.fits";
    QString name = mainDirName+"/"+shortName;

    if (*verbosity > 0) emit messageAvailable("Writing "+mode+" background model "+shortName, "data");
    combinedImage[chip]->writeImage(name, filter, exptime, addGainNormalization);
    if (mode == "static") staticImageWritten = true;

    successProcessing = combinedImage[chip]->successProcessing;
}

/*
void Data::writeBackgroundModel_newParallel(int chip, MyImage *combinedBackgroundImage, QString mode, QString basename, int threadID,
                                            omp_lock_t &backLock, bool &staticImageWritten)
{
    if (!successProcessing) return;
    if (userStop || userKill) return;
    if (mode == "static" && staticImageWritten) return;

    QDir backgroundDir(dirName+"/BACKGROUND/");
    backgroundDir.mkdir(dirName+"/BACKGROUND/");

    float exptime = myImageList[chip][0]->exptime;
    QString filter = myImageList[chip][0]->filter;
    bool addGainNormalization = false;

    if (mode == "dynamic") {
        QString shortName = subDirName+"/BACKGROUND/"+basename+".back.fits";
        QString name = mainDirName+"/"+shortName;
        if (*verbosity > 0) emit messageAvailable("Writing "+mode+" background model "+shortName, "data");
        combinedBackgroundImage->writeImage(name, filter, exptime, addGainNormalization);
        successProcessing = combinedBackgroundImage->successProcessing;
    }

    else {  // static mode
        // We need a lock so that the model for a given chip is written only once
        // Some paranoia with checks concerning the state of the model and thread safety.
        if (staticImageWritten) return;
        omp_set_lock(&backLock);
        if (!staticImageWritten) {
            QString shortName = subDirName+"/BACKGROUND/"+subDirName+"_"+QString::number(chip+1)+".fits";
            QString name = mainDirName+"/"+shortName;
            if (*verbosity > 0) emit messageAvailable("Writing "+mode+" background model "+shortName, "data");
            combinedBackgroundImage->writeImage(name, filter, exptime, addGainNormalization);
            successProcessing = combinedBackgroundImage->successProcessing;
            staticImageWritten = true;
        }
        omp_unset_lock(&backLock);
    }
}
*/

void Data::resetUserAbort()
{
    userYield = false;
    userStop = false;
    userKill = false;
}

void Data::resetObjectMasking()
{
    // Reset object masks from a potential previous pass
    if (*verbosity > 0) emit messageAvailable("Resetting object masks ...", "data");
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : myImageList[chip]) {
            it->resetObjectMasking();
        }
    }
}

// only used for debayering
void Data::repopulate(int chip, QList<MyImage*> replacementList)
{
    for (auto &it : myImageList[chip]) {
        it->freeAll();
        //        delete it;       // Must not delete in case of debayering!!! will cause crash in DataModel
    }
    myImageList[chip].clear();
    for (auto &it : replacementList) {
        myImageList[chip].append(it);
    }
}

void Data::populate(QString statusString)
{
    if (*verbosity > 2) emit messageAvailable(subDirName + " : Initializing images ...", "data");
    // Read either raw or processed images (master calibs are handled in the c'tor)

    numImages = 0;
#pragma omp parallel for num_threads(maxCPU) firstprivate(dirName, subDirName, statusString)
    for (int chip=0; chip<instData->numChips; ++chip) {
        QStringList filter;
        filter << "*_"+QString::number(chip+1)+statusString+".fits";
        QStringList fitsFiles = dir.entryList(filter);
        // TODO: when splitting data, we must use a filter that does not contain the _chip string (raw data)
        // if list == empty then reset string and reload
        myImageList[chip].clear();
        for (auto &it : fitsFiles) {
            bool skip = false;
            // skip master calibs and normalized flats
            if (it == subDirName+"_"+QString::number(chip+1)+".fits") skip = true;
            if (skip) continue;
            MyImage *myImage = new MyImage(dirName, it, statusString, chip+1, mask->globalMask[chip], verbosity);
            //            myImage->setParent(this);
            connect(myImage, &MyImage::modelUpdateNeeded, this, &Data::modelUpdateReceiver);
            connect(myImage, &MyImage::critical, this, &Data::pushCritical);
            connect(myImage, &MyImage::warning, this, &Data::pushWarning);
            connect(myImage, &MyImage::messageAvailable, this, &Data::pushMessageAvailable);
            connect(myImage, &MyImage::setMemoryLock, this, &Data::setMemoryLockReceived, Qt::DirectConnection);
            connect(myImage, &MyImage::setWCSLock, this, &Data::setWCSLockReceived, Qt::DirectConnection);
            myImage->imageOnDrive = true;
            myImageList[chip].append(myImage);
#pragma omp critical
            {
                if (!uniqueChips.contains(chip+1)) uniqueChips.push_back(chip+1);
            }
        }
#pragma omp atomic
        numImages += myImageList[chip].length();
    }

    // Sort the vector with the chip numbers (no particular reason, yet)
    std::sort(uniqueChips.begin(), uniqueChips.end());
    if (*verbosity > 0) emit messageAvailable(subDirName + " : " + QString::number(numImages) + " Images initialized ...", "data");
}

void Data::countUnsavedImages(long &numUnsavedLatest, long &numUnsavedBackup)
{
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        for (auto &it : myImageList.at(chip)) {
            if (!it->imageOnDrive) ++numUnsavedLatest;
            if (it->backupL1InMemory && !it->backupL1OnDrive) ++numUnsavedBackup;
            if (it->backupL2InMemory && !it->backupL2OnDrive) ++numUnsavedBackup;
            if (it->backupL3InMemory && !it->backupL3OnDrive) ++numUnsavedBackup;
        }
    }
}

void Data::clearImageInfo()
{
    imageInfo.baseName.clear();
    imageInfo.fullName.clear();
    imageInfo.naxis1.clear();
    imageInfo.naxis2.clear();
    imageInfo.filter.clear();
    imageInfo.mjdobs.clear();
    imageInfo.chip.clear();
}

bool Data::collectMJD()
{
    if (!successProcessing) return false;

    // Get the MJD for all images in all chips if it hasn't been read yet
    // (e.g., if the user starts THELI and continues some processing)
    // The FITS handles are always present in MyImage, even if it hasn't been read yet

    if (*verbosity > 0) emit messageAvailable("Retrieving Modified Julian Dates ...<br>", "data");

    bool duplicateFound = false;

    // The MJD is the same for all chips, hence we could just test it for chip 1.
    // But if one of the images of chip 1 was removed because it was bad, then this would break down;
    // Therefore, read it for every chip in every exposure
#pragma omp parallel for num_threads(maxExternalThreads) firstprivate(subDirName)
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        QVector<double> mjdData;
        mjdData.reserve(myImageList[chip].length());
        if (duplicateFound) continue;
        for (auto &it : myImageList[chip]) {
            //            if (!it->hasMJDread) {
            //                it->imageFITS->getMJD();
            it->getMJD();
            //                it->mjdobs = it->imageFITS->mjdobs;
            //                it->hasMJDread = true;
            //            }
            if (*verbosity == 3) emit messageAvailable(it->chipName + " : MJD-OBS = " +QString::number(it->mjdobs, 'f', 12), "image");
            mjdData.append(it->mjdobs);
        }
        if (!duplicateFound && hasDuplicates_T(mjdData)) {
            // QmessageBox displayed only once (cannot break from loop because of omp parallel)
            emit showMessageBox("Data::DUPLICATE_MJDOBS", subDirName, "");
            duplicateFound = true;
        }
    }

    if (duplicateFound) return false;
    else return true;
}

// The background header correction tasks needs a number of header keywords
// before processing the actual exposures
bool Data::getPointingCharacteristics()
{
    if (!successProcessing) return false;

    if (*verbosity > 0) emit messageAvailable("Retrieving pointing characteristics for "+subDirName + " ...", "data");

    QVector<double> crval1Exposure;
    QVector<double> crval2Exposure;
    QVector<double> crval1Vertex;
    QVector<double> crval2Vertex;

    // we include unused chips here because otherwise any centroids our boundaries might be biased
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : myImageList[chip]) {
            it->loadHeader();
            crval1Exposure << it->wcs->crval[0];
            crval2Exposure << it->wcs->crval[1];
            crval1Vertex << it->alpha_ll;
            crval1Vertex << it->alpha_lr;
            crval1Vertex << it->alpha_ul;
            crval1Vertex << it->alpha_ur;
            crval2Vertex << it->delta_ll;
            crval2Vertex << it->delta_lr;
            crval2Vertex << it->delta_ul;
            crval2Vertex << it->delta_ur;
        }
    }

    RAcenter = straightMedian_T(crval1Exposure, false);
    DECcenter = straightMedian_T(crval2Exposure, false);

    // Declination
    double crval2Min = minVec_T(crval2Vertex);
    double crval2Max = maxVec_T(crval2Vertex);
    double crval2Radius = 60.*(crval2Max - crval2Min) / 2.;

    // Right ascension
    double crval1Min = minVec_T(crval1Vertex);
    double crval1Max = maxVec_T(crval1Vertex);
    // Did we cross the origin (qualitative check)?
    // If yes, then we need to correct the values
    if (crval1Max - crval1Min > 350.) {
        for (auto &it : crval1Vertex) {
            if (it > 180.) it -= 180.;
        }
        crval1Min = minVec_T(crval1Vertex);
        crval1Max = maxVec_T(crval1Vertex);
    }
    double crval1Radius = 60.*(crval1Max - crval1Min) / 2. * cos(DECcenter*rad);

    // Maximum search radius, plus 10% safety margin
    searchRadius = 1.1*sqrt(crval1Radius*crval1Radius + crval2Radius*crval2Radius);

    if (RAcenter == -100. || DECcenter == -100. || searchRadius == -100.) return false;
    else return true;
}

bool Data::hasImages()
{
    if (numImages == 0) {
        emit showMessageBox("Data::IMAGES_NOT_FOUND", processingStatus->statusString, dirName);
        emit critical();
        return false;
    }
    else return true;
}

void Data::resetProcessbackground()
{
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : myImageList[chip]) {
            it->resetObjectMasking();
            it->hasBrightStarsChecked = false;
            it->backgroundModelDone = false;
            it->segmentationDone = false;
            it->maskExpansionDone = false;
            it->backupCopyBackgroundMade = false;
            it->leftBackgroundWindow = false;
            // possibly squeeze some of the data vectors, but then we'd just need to reserve them again [...]
        }
    }
}

void Data::cleanBackgroundModelStatus()
{
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : myImageList[chip]) {
            it->resetObjectMasking();
            it->backgroundModelDone = false;
            it->segmentationDone = false;
            it->maskExpansionDone = false;
        }
    }
}

QVector<double> Data::getKeyFromAllImages(const QString key)
{
    if (*verbosity > 1) emit messageAvailable("Retrieving key " + key + " for images in " + subDirName + " ...", "data");

    // dumping everything to a double vector
    QVector<double> data;
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : myImageList[chip]) {
            if (!it->metadataTransferred) it->loadHeader();  // needed if data requested immediately after launch
            if (key == "CRVAL1") data << it->wcs->crval[0];
            else if (key == "CRVAL2") data << it->wcs->crval[1];
            else if (key == "CRPIX1") data << it->wcs->crpix[0];
            else if (key == "CRPIX2") data << it->wcs->crpix[1];
            else if (key == "NAXIS1") data << it->naxis1;
            else if (key == "NAXIS2") data << it->naxis2;
            else if (key == "CORNERS_CRVAL1") {
                data << it->alpha_ll;
                data << it->alpha_lr;
                data << it->alpha_ul;
                data << it->alpha_ur;
            }
            else if (key == "CORNERS_CRVAL2") {
                data << it->delta_ll;
                data << it->delta_lr;
                data << it->delta_ul;
                data << it->delta_ur;
            }
            else if (key == "DATE-OBS") {
                double tmp = dateobsToDecimal(it->dateobs);
                // Only append if dateobs is valid (we won't have data taken before the year 1000)
                if (tmp > 1000.) data << tmp;
            }
        }
    }
    return data;
}

void Data::modelUpdateReceiver(QString chipName)
{
    emit modelUpdateNeeded(chipName);
}

void Data::pushMessageAvailable(QString message, QString type)
{
    emit messageAvailable(message, type);
}

void Data::pushCritical()
{
    emit critical();
}

void Data::pushWarning()
{
    emit warning();
}

int Data::identifyClusters(QString toleranceString)
{
    if (!successProcessing) return 0;

    // This routine detects associations of images that are overlapping, and assigns a unique number

    int groupNumber = 0;
    int groupNumberOld = 0;

    float tolerance = 0.;
    if (!toleranceString.isEmpty()) tolerance = toleranceString.toFloat() / 60.;

    // Initiate the first of all images
    myImageList[0][0]->groupNumber = groupNumber;

    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : myImageList[chip]) {
            it->loadHeader();
        }
    }

    bool unassigned = true;
    int loop = 0;
    while (unassigned) {
        // Loop over all images to see if they overlap with the current image
        for (int chip=0; chip<instData->numChips; ++chip) {
            for (auto &it : myImageList[chip]) {
                // Jump over images that have been assigned already
                if (loop > 0 && it->groupNumber == groupNumberOld) continue;
                // Check distance of all other images to the current image, and update their groupNumber if matching
                findOverlappingImages(it, tolerance);
            }
        }
        // Check if images remain unassigned
        groupNumberOld = groupNumber;
        unassigned = checkForUnassignedImages(groupNumber);
        ++loop;
    }

    if (*verbosity >= 0) {
        if (groupNumber == 0) {
            emit messageAvailable("A single image association was identified. It is left unchanged.", "data");
        }
        else {
            emit messageAvailable(QString::number(groupNumber+1)+" image associations identified ...", "data");
        }
    }

    return groupNumber;
}

void Data::doImagesOverlap(const MyImage &imgRef, MyImage &imgTest, const float tolerance)
{
    if (!successProcessing) return;

    double alpha1 = rad * imgRef.wcs->crval[0];
    double delta1 = rad * imgRef.wcs->crval[1];
    double alpha2 = rad * imgTest.wcs->crval[0];
    double delta2 = rad * imgTest.wcs->crval[1];
    double dDelta = delta2 - delta1;
    double dAlpha = alpha2 - alpha1;

    // Haversine formula to calculate angular distance between two points on a sphere
    double distance = 2.*asin( sqrt( pow(sin(dDelta/2.),2) + cos(delta1)*cos(delta2)*pow(sin(dAlpha/2.),2))) / rad;

    if (distance <= 2.* instData->radius + tolerance) imgTest.groupNumber = imgRef.groupNumber;
}

void Data::findOverlappingImages(const MyImage *img, const float tolerance)
{
    if (!successProcessing) return;

    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : myImageList[chip]) {
            // If unassigned, check overlap and if positive, assign same group number
            if (it->groupNumber == -1) {
                doImagesOverlap(*img, *it, tolerance);
            }
        }
    }
}

bool Data::checkForUnassignedImages(int &groupNumber)
{
    if (!successProcessing) return false;

    bool unassigned = false;
    for (int chip=0; chip<instData->numChips; ++chip) {
        bool breaked = false;
        for (auto &it : myImageList[chip]) {
            if (it->groupNumber == -1) {
                unassigned = true;
                // Another group exists, increase group counter, and start group
                ++groupNumber;
                it->groupNumber = groupNumber;
                breaked = true;
                break;
            }
        }
        if (breaked) break;
    }
    return unassigned;
}

// Restores FITS images from backupDirName; replaces dataCurrent with dataBackup (if in memory)
void Data::restoreBackupLevel(QString backupDirName)
{
    emit messageAvailable("Restoring "+backupDirName + " images ...", "data");

    if (backupDirName == "_IMAGES") {
        emit messageAvailable("Inconsistency detected in backup structure. This is likely a bug.", "error");
        emit critical();
        return;
    }

    QDir backupDir(dirName+"/"+backupDirName);
    // Qt 5.7 compatibility
    backupDir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    if (!backupDir.exists() || backupDir.count() == 0) {
        emit messageAvailable(dirName+"/"+backupDirName+": No backup FITS files found, restoring memory only.", "data");
    }
    /*
 * QT5.9
    if (!backupDir.exists() || backupDir.isEmpty()) {
        emit messageAvailable(dirName+"/"+backupDirName+": No backup FITS files found, restoring memory only.", "data");
    }
*/

    // Remove all currently present FITS files
    removeCurrentFITSfiles();

    // CASE 1: Restore the raw data
    if (backupDirName == "RAWDATA") {
        restoreRAWDATA();
        return;
    }

    // CASE 2: Data in memory (and on disk)
    // Comparing output from QDir::absolutePath() because it removes double // and trailing /
    QDir d1(pathBackupL1);
    QDir d2(pathBackupL2);
    QDir d3(pathBackupL3);
    QDir dc(dirName+"/"+backupDirName);
    bool success = true;
    QString newStatusRAM = "";

    if (d1.absolutePath() == dc.absolutePath()) success = success && restoreFromBackupLevel("L1", newStatusRAM);
    else if (d2.absolutePath() == dc.absolutePath()) success = success && restoreFromBackupLevel("L2", newStatusRAM);
    else if (d3.absolutePath() == dc.absolutePath()) success = success && restoreFromBackupLevel("L3", newStatusRAM);

    //    qDebug() << "success = " << success << newStatusRAM;

    // Leave if there was an error during file operations
    if (!success) {
        emit messageAvailable("Data::restoreBackupLevel(): Error restoring FITS files from " + backupDirName, "error");
        emit critical();
        return;
    }
    else {
        // If newStatusRAM has changed then the operation was successful, and we can remove the directory
        if (!newStatusRAM.isEmpty()) {
            backupDir.removeRecursively();
            emit statusChanged(newStatusRAM);
            emit updateModelHeaderLine();
            return;
        }
    }

    //    qDebug() << "BLEVEL CASE3";

    // CASE 3: We are still here. That means the user selected a backup dir on disk that is not mapped in one of the backup levels
    restoreFromDirectory(backupDirName);
}

void Data::removeCurrentFITSfiles()
{
    QStringList fitsFileNames = dir.entryList(QStringList() << "*.fits");
    for (auto &it : fitsFileNames) {
        QFile fitsFile(dirName+"/"+it);
        if (!fitsFile.remove()) {
            emit messageAvailable("Could not delete " + it + "! Manual cleanup required.", "error");
            emit critical();
        }
    }
}

void Data::removeCatalogs()
{
    // Remove all scamp, SourceExtractor and .anet catalogs. Keep the iview catalogs
    // Used when recreating source catalogs
    QDir catalogDir(dirName + "/cat/");
    QStringList catFileNames = catalogDir.entryList(QStringList() << "*.cat" << "*.scamp" << "*.anet" );
    for (auto &it : catFileNames) {
        QFile catFile(dirName+"/cat/"+it);
        if (!catFile.remove()) {
            emit messageAvailable("Could not delete " + it, "warning");
            emit warning();
        }
    }
}

// Reflecting backup parameters in Data after successful processing
void Data::transferBackupInfo()
{
    if (!successProcessing) return;

    MyImage *it = myImageList[0][0];
    pathBackupL1 = it->pathBackupL1;
    pathBackupL2 = it->pathBackupL2;
    pathBackupL3 = it->pathBackupL3;
    statusBackupL1 = it->statusBackupL1;
    statusBackupL2 = it->statusBackupL2;
    statusBackupL3 = it->statusBackupL3;
}

// Reset status (when restoring original data)
void Data::resetBackupInfo()
{
    processingStatus->statusString = "";
    pathBackupL1 = "";
    pathBackupL2 = "";
    pathBackupL3 = "";
    statusBackupL1 = "";
    statusBackupL2 = "";
    statusBackupL3 = "";
}

// Used if data is restored from a backup dir alone, i.e. data are not present in RAM in any backup level
void Data::restoreFromDirectory(QString backupDirName)
{
    // Check if any data can be restored
    QDir backupDir(dirName+"/"+backupDirName);
    if (!backupDir.exists()) {
        emit messageAvailable("Data::restoreFromDirectory(): " + subDirName+"/"+backupDirName+" does not exist, no data were restored or deleted.", "error");
        emit critical();
        return;
    }
    backupDir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);  // Qt 5.7 compatibility
    //    if (backupDir.isEmpty()) {             Qt 5.9
    if (backupDir.count() == 0) {
        emit messageAvailable(subDirName+"/"+backupDirName+" is empty, no data were restored or deleted.", "error");
        emit critical();
        return;
    }

    // Remove all currently present FITS files
    removeCurrentFITSfiles();

    QString newStatusDrive = "";
    if (backupDirName == "RAWDATA") newStatusDrive = "";
    else newStatusDrive = backupDirName.split("_").at(0);
    processingStatus->statusToBoolean(newStatusDrive);
    processingStatus->getStatusString();
    processingStatus->writeToDrive();

    if (!moveFiles("*.fits", dirName+"/"+backupDirName, dirName)) {
        emit messageAvailable(backupDirName + " : Restoring failed for some or all FITS files", "error");
        emit critical();
        return;
    }
    else {
        // Paranoia check (afaik this can never happen, but nonetheless ...)
        if (backupDirName == QDir::homePath()) {
            emit messageAvailable("Data::restoreFromDirectory(): STOP: Was about to remove your home directory! This should never happen.", "error");
            emit critical();
            return;
        }
        if (!backupDir.removeRecursively()) {
            emit messageAvailable("Data::restoreFromDirectory(): Could not completely remove " + backupDirName, "error");
            emit critical();
            return;
        }
    }
    emit statusChanged(newStatusDrive);
    emit updateModelHeaderLine();
}

// Returns false if anything goes wrong with the file operations
bool Data::restoreFromBackupLevel(QString level, QString &newStatusRAM)
{
    long i = 0;
    bool success = true;
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        for (auto &it: myImageList[chip]) {
            // Continue if no backup data in RAM
            if (level == "L1") success = success && it->makeL1Current();
            else if (level == "L2") success = success && it->makeL2Current();
            else if (level == "L3") success = success && it->makeL3Current();

            emit it->modelUpdateNeeded(it->chipName);

            if (i==0) newStatusRAM = it->processingStatus->statusString;
            QString statusTmp2 = it->processingStatus->statusString;
            if (statusTmp2 != newStatusRAM) {
                emit messageAvailable("Data::restoreBackupLevel(): Inconsistent processing status detected in RAM", "error");
                emit critical();
                return false;
            }
            ++i;
        }
    }
    transferBackupInfo();
    processingStatus->statusToBoolean(newStatusRAM);
    processingStatus->getStatusString();
    processingStatus->writeToDrive();
    return success;
}

void Data::restoreRAWDATA()
{
    QDir rawdataDir(dirName+"/RAWDATA");
    if (!rawdataDir.exists()) {
        emit messageAvailable(subDirName+"/RAWDATA does not exist, "+subDirName+" remains unmodified.", "note");
        emit warning();
        return;
    }
    rawdataDir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);  // Qt 5.7 compatibility
    //    if (rawdataDir.isEmpty()) {             Qt 5.9
    if (rawdataDir.count() == 0) {
        emit messageAvailable(subDirName+"/RAWDATA is empty, "+subDirName+" remains unmodified.", "note");
        emit warning();
        return;
    }

    // Protect RAWDATA by moving it
    if (!rawdataDir.rename(dirName+"/RAWDATA", mainDirName+"/"+subDirName+"_rawdata")) {
        emit messageAvailable("Could not create temporary directory "+mainDirName+"/"+subDirName+"_rawdata!", "error");
        emit critical();
        return;
    }

    emit messageAvailable("Restoring " + dirName+"/RAWDATA ...", "data");
    // Paranoia check (afaik this can never happen, but nonetheless ...)
    if (dirName == QDir::homePath()) {
        emit messageAvailable("Data::restoreRAWDATA(): STOP: Was about to remove your home directory! This should never happen.", "error");
        emit critical();
        return;
    }
    // remove current dir
    dir.removeRecursively();
    // restore
    rawdataDir.rename(mainDirName+"/"+subDirName+"_rawdata", dirName);

    emit setMemoryLock(true);
    releaseAllMemory();
    resetBackupInfo();
    myImageList.clear();
    combinedImage.clear();
    myImageList.resize(instData->numChips);
    combinedImage.resize(instData->numChips);
    dataInitialized = false;
    emit globalModelUpdateNeeded();   // CHECK: not necessarily threadsafe, depending on the thread in which the slot gets executed
    emit setMemoryLock(false);
    QString newStatus = "";
    processingStatus->deleteFromDrive();
    processingStatus->reset();
    //    processingStatus->writeToDrive();
    emit statusChanged(newStatus);
    emit updateModelHeaderLine();
}

void Data::emitStatusChanged()
{
    emit statusChanged(processingStatus->statusString);
    emit updateModelHeaderLine();
}

bool Data::hasMatchingPartnerFiles(QString testDirName, QString suffix)
{
    if (!successProcessing) return false;

    // This function checks whether e.g. all science exposures have a weight, or astrometric header

    // List of images
    QStringList imageList;
    if (suffix == ".scamp") {
        for (int chip=0; chip<instData->numChips; ++chip) {
            for (auto &it : myImageList[chip]) {
                if (!imageList.contains(it->rootName)
                        && it->activeState == MyImage::ACTIVE) {
                    imageList.append(it->rootName);
                }
            }
        }
    }
    else {
        for (int chip=0; chip<instData->numChips; ++chip) {
            for (auto &it : myImageList[chip]) {
                if (it->activeState == MyImage::ACTIVE) imageList.append(it->chipName);
            }
        }
    }

    // List of reference data (weights, catalogs, headers, etc ...)
    QDir testDir(testDirName);
    QStringList notMatched;
    QStringList filter("*"+suffix);
    QStringList testList = testDir.entryList(filter);
    testList.replaceInStrings(suffix,"");

    imageList.sort();
    testList.sort();

    // If entries are equal, or list1 is fully contained in list2, then we can leave

    // Equal?
    if (imageList.operator ==(testList)) {
        return true;
    }
    else {
        // Fully contained?
        for (auto & it : imageList) {
            // If not contained in list2, add it to the list of missing items
            if (!testList.contains(it)) notMatched << it;
        }
    }

    if (notMatched.isEmpty()) return true;
    else {
        QString missingItems;
        int i = 0;
        for (auto &it: notMatched) {
            if (i>19) {
                missingItems.append("...\n");
                break;  // Do not show more than 20 items
            }
            missingItems.append(it);
            //            missingItems.append(suffix);
            missingItems.append("\n");
            ++i;
        }
        emit messageAvailable(subDirName + " : Not all images have matching "+suffix+ " files!", "error");
        emit critical();
        successProcessing = false;

        long nbad = notMatched.length();
        if (suffix.contains("weight")) {
            emit showMessageBox("Data::WEIGHT_DATA_NOT_FOUND", QString::number(nbad), missingItems);
        }
        else if (suffix.contains("head")) {
            emit showMessageBox("Data::HEADER_DATA_NOT_FOUND", QString::number(nbad), missingItems);
        }
        else if (suffix.contains("scamp")) {
            emit showMessageBox("Data::SCAMP_CATS_NOT_FOUND", QString::number(nbad), missingItems);
        }
        return false;
    }
}

void Data::criticalFromQueryReceived()
{
    successProcessing = false;
}


bool Data::doesCoaddContainRaDec(const QString &refRA, const QString &refDEC)
{
    if (refRA.isEmpty() && refDEC.isEmpty()) return true;

    // Start assuming that no chip of a multi-chip camera contains this coordinate. If a single chip does contain it, we can break and exit
    bool containsRADEC = false;
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : myImageList.at(chip)) {
            if (it->containsRaDec(refRA, refDEC)) {
                containsRADEC = true;
                break;
            }
        }
    }
    return containsRADEC;
}

// Data is listening to all its MyImages
/*
void Data::establish_connections()
{
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &image : myImageList[chip]) {
            connect(image, &MyImage::modelUpdateNeeded, this, &Data::modelUpdateReceiver);
        }
    }
}
*/

void Data::deleteMyImageList()
{
    if (myImageList.isEmpty()) return;
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it: myImageList[chip]) {
            delete it;
            it = nullptr;
        }
    }
}

/*
void Data::forceStatus(int chip, QString status)
{
    for (auto &it : myImageList[chip]) {
        it->processingStatus->statusString = status;
    }
}
*/

/*
void Data::getFixedHeaderKeys(QString filename, QStringList &badImages)
{
    if (userStop || userKill) return;

    long naxis1 = 0;
    long naxis2;
    QString filter = "UNKNOWN";
    double mjdobs = 0.;

    fitsfile *fptr = nullptr;
    int status = 0;

    fits_open_file(&fptr, filename.toUtf8().data(), READONLY, &status);
    if (!status) {
        char *filterName = new char(filename.length()+10);
        fits_read_key_lng(fptr, "NAXIS1", &naxis1, NULL, &status);
        fits_read_key_lng(fptr, "NAXIS2", &naxis2, NULL, &status);
        fits_read_key_dbl(fptr, "MJD-OBS", &mjdobs, NULL, &status);
        fits_read_key_str(fptr, "FILTER", filterName, NULL, &status);
        if (status && !badImages.contains(filename)) {
            badImages.push_back(filename);
            imageInfo.naxis1.push_back(0);
            imageInfo.naxis2.push_back(0);
            imageInfo.mjdobs.push_back(0.);
            imageInfo.filter.push_back("UNKNOWN");
        }
        else {
            filter = QString(filterName);
            imageInfo.naxis1.push_back(naxis1);
            imageInfo.naxis2.push_back(naxis2);
            imageInfo.mjdobs.push_back(mjdobs);
            imageInfo.filter.push_back(filter);
            //          qDebug() << qSetRealNumberPrecision(12) << mjdobs;
        }
        delete filterName;
        filterName = nullptr;
    }

    fits_close_file(fptr, &status);

    if (status) {
        emit messageAvailable(subDirName + " : Data::getFixedHeaderKeys(): Could not read NAXIS1/2, MJD-OBS, and/or FILTER in" + filename, "error");
        printCfitsioError("getFixedHeaderKeys()", status);
    }
}
*/

// Reads relevant keywords from the headers
/*
bool Data::getHeaders()
{
    QStringList badImages;

    for (auto &it : imageInfo.fullName) {
        getFixedHeaderKeys(dirName+"/"+it, badImages);
    }
    if (!badImages.isEmpty()) {
        // Truncate (in case of lots of images)
        QString summary = truncateImageList(badImages,10);
        emit messageAvailable(tr("Could not read one or more of the following keywords:<br>NAXIS1, NAXIS2, FILTER, MJD-OBS")+
                              tr("<br>Add the keywords manually, or the remove the corrupted images.<br>")+summary, "error");
        emit critical();
        emit showMessageBox("Data::CANNOT_READ_HEADER_KEYS", summary, "");
        return false;
    }
    else {
        return true;
    }
}
*/

// Calculates the median geometry for the images of a chip.
// Identifies outliers and moves them to a 'badGeometry/' sub-directory
// UNUSED
/*
QString Data::checkGeometry()
{
    if (!successProcessing) return "";

    QStringList badImages;

    for (auto &chip : uniqueChips) {
        // Find images matching that chip, extract their geometries
        QStringList imageList = collectNamesForChip(chip);
        QVector<int> tmp1;
        QVector<int> tmp2;
        for (auto &it : imageList) {
            tmp1.push_back(imageInfo.naxis1[mapName.value(it)]);
            tmp2.push_back(imageInfo.naxis2[mapName.value(it)]);
        }
        // Median image geometry for chip 'chip':
        int medianNaxis1 = straightMedian_T(tmp1);
        int medianNaxis2 = straightMedian_T(tmp2);
        mapNaxis1.insert(chip, medianNaxis1);
        mapNaxis2.insert(chip, medianNaxis2);
        // Collect images whose geometry deviates from the median geometry
        for (auto &it : imageList) {
            if (imageInfo.naxis1[mapName.value(it)] != medianNaxis1
                    || imageInfo.naxis2[mapName.value(it)] != medianNaxis2) {
                badImages << it;
            }
        }
    }

    if (badImages.isEmpty()) return "success";

    // What to do with the bad images
    QString summary = truncateImageList(badImages, 10);

    emit messageAvailable(subDirName + " : WARNING: The 2D dimensions of the following images deviate from the median dimension:<br>"
                          +summary+"<br>"+
                          "This can be a result of inconsistent binning or readout modes.<br> "
                          "THELI can move these images to a 'badGeometries/' sub-directory "
                          "and re-attempt the process. Alternatively, you can cancel and inspect the data yourself "
                          "before trying again.\n", "warning");

    QMessageBox msgBox;
    msgBox.setText(tr("ERROR: The 2D dimensions of the following images deviate from the median dimension:"));
    msgBox.setInformativeText(summary+
                              "This can be a result of inconsistent binning or readout modes. "
                              "THELI can move these images to a 'badGeometries/' sub-directory "
                              "and re-attempt the process. Alternatively, you can cancel and inspect the data yourself "
                              "before trying again.\n");
    QAbstractButton *pButtonOk = msgBox.addButton(tr("Move images and re-try"), QMessageBox::YesRole);
    QAbstractButton *pButtonCancel = msgBox.addButton(tr("Cancel"), QMessageBox::YesRole);
    msgBox.exec();

    if (msgBox.clickedButton()==pButtonOk) {
        // make a "badGeometry" sub-directory
        QString badDirName = dirName+"/badGeometry/";
        QDir badDir(badDirName);
        badDir.mkpath(badDirName);
        // move selected images to badstats
        for (auto &it : badImages ) {
            QFile badFile(dirName+"/"+it);
            if (!badFile.rename(badDirName+"/"+it)) {
                emit messageAvailable(subDirName + " : Data::checkGeometry(): Could not execute the following operation:<br>"+
                                      "mv " + dirName+"/"+it + badDirName+"/"+it, "error");
                emit critical();
                return "cancel";
            }
        }
        return "recurse";
    }
    else {
        return "cancel";
    }
}
*/

/*
void Data::incrementCurrentThreads(int &currentThreads, omp_lock_t &lock)
{
    omp_set_lock(&lock);
    ++currentThreads;
    omp_unset_lock(&lock);
}

void Data::decrementCurrentThreads(int &currentThreads, omp_lock_t &lock)
{
    omp_set_lock(&lock);
    --currentThreads;
    omp_unset_lock(&lock);
}
*/

/*
void Data::getModeCombineImagesBackground(int chip, MyImage *image)
{
    if (!successProcessing) return;

    if (userStop || userKill) return;

    if (image->modeDetermined) return;

    // Get the mean / mode of the combined image
    if (dataType == "BIAS" || dataType == "DARK" || dataType == "FLATOFF") {
        // mode doesn't work, as all values are very very narrowly distributed around a single value (which might be integer on top of that)
        image->skyValue = meanMask(image->dataCurrent, mask->globalMask[chip]);
    }
    else {
        image->skyValue = modeMask(image->dataCurrent, "stable", mask->globalMask[chip])[0];
    }
    image->modeDetermined = true;

    successProcessing = true;
}
*/

/*
void Data::writeGlobalflags(int chip, QString filter)
{
    if (*verbosity > 0) emit messageAvailable("Writing globalflag for chip " + QString::number(chip+1), "data");

    // The new output file
    fitsfile *fptr;
    int status = 0;
    long fpixel = 1;
    long naxis1 = instData->sizex[chip];
    long naxis2 = instData->sizey[chip];
    long nelements = naxis1*naxis2;
    unsigned int *array = new unsigned int[nelements];
    for (long i=0; i<nelements; ++i) {
        array[i] = 0;
    }

    int bitpix = BYTE_IMG;
    long naxis = 2;
    long naxes[2] = { naxis1, naxis2 };

    QString globalFlagName = mainDirName+"/GLOBALWEIGHTS/globalflag_"+instData->name+"_"+filter+"_"+QString::number(chip+1)+".fits";
    // Overwrite file if it exists
    globalFlagName = "!"+globalFlagName;
    fits_create_file(&fptr, globalFlagName.toUtf8().data(), &status);
    fits_create_img(fptr, bitpix, naxis, naxes, &status);
    fits_write_img(fptr, TBYTE, fpixel, nelements, array, &status);
    fits_close_file(fptr, &status);

    delete [] array;
    array = nullptr;

    printCfitsioError("writeGlobalflags()", status);
}
*/

/*
float Data::memoryNeeded(int chip)
{
    float footprint = 0;

    for (auto &it: myImageList[chip]) {
        footprint += it->dataCurrent.size() * sizeof(float);
    }
    return footprint / 1024. / 1024.;
}
*/

/*
float Data::memoryCurrentFootprint(bool globalweights)
{
    float footprint = 0.;    // Storage (in MB) used for all images currently held in memory

    // CHECK: crashes here when clicking the project reset button right after launching the GUI.
    // still crashes despite this if-clause. MyImageList[chip] seems undefined (innermost for loop)

    if (processingStatus == nullptr
            || !processingStatus->HDUreformat
            || !dataInitialized) return 0.;   // MyImageList undefined, or no data loaded yet

    // For an unknown reason, I cannot access myImageList for GLOBALWEIGHTS when changing a project; memoryprogressbar then crashes the UI
    if (!globalweights) {
        if (!myImageList.isEmpty()) {  // e.g. if RAWDATA are restored
            for (int chip=0; chip<instData->numChips; ++chip) {
                if (instData->badChips.contains(chip)) continue;
                for (auto &it: myImageList[chip]) {
                    footprint += it->dataCurrent.capacity() * sizeof(float);
                    footprint += it->dataBackupL1.capacity() * sizeof(float);
                    footprint += it->dataBackupL2.capacity() * sizeof(float);
                    footprint += it->dataBackupL3.capacity() * sizeof(float);
                    footprint += it->dataMeasure.capacity() * sizeof(float);
                    footprint += it->objectMask.capacity() * sizeof(bool);
                    footprint += it->dataWeight.capacity() * sizeof(float);
                    footprint += it->dataWeightSmooth.capacity() * sizeof(float);
                    footprint += it->dataBackground.capacity() * sizeof(float);
                    footprint += it->dataSegmentation.capacity() * sizeof(long);
                }
            }
        }
        // Also check for debayered images that are not yet in the nominal myimage list
        if (!bayerList.isEmpty()) {
            for (int chip=0; chip<instData->numChips; ++chip) {
                if (instData->badChips.contains(chip)) continue;
                for (auto &it: bayerList[chip]) {
                    footprint += it->dataCurrent.capacity() * sizeof(float);
                }
            }
        }
    }

    if (!combinedImage.isEmpty()) {
        for (int chip=0; chip<instData->numChips; ++chip) {
            if (instData->badChips.contains(chip)) continue;
            // Crashes if equal to nullptr
            if (combinedImage[chip] != nullptr) {
                footprint += combinedImage[chip]->dataCurrent.capacity() * sizeof(float);
            }
        }
    }
    return footprint /= (1024 * 1024);
}
*/

// UNUSED
// Force free
// should use the controller's 'lock'
/*
void Data::memoryFreeDataX(int chip, QString dataX)
{
    //    if (!successProcessing) return;

    QVector<float> empty {};

    if (dataX != "combined") {
        long memReleased = myImageList[chip][0]->dataCurrent.length()*4*myImageList[chip].length();
        if (*verbosity > 1) emit messageAvailable("Freeing "+QString::number(memReleased) + " MB", "data");
        for (auto &it : myImageList[chip]) {
            if (dataX == "dataCurrent") it->dataCurrent.swap(empty);
            else if (dataX == "dataBackupL1") it->dataBackupL1.swap(empty);
            else if (dataX == "dataBackupL2") it->dataBackupL2.swap(empty);
            else if (dataX == "dataBackupL3") it->dataBackupL3.swap(empty);
            else if (dataX == "dataWeight") it->dataWeight.swap(empty);
        }
    }
    else {
        long memReleased = combinedImage[chip]->dataCurrent.length()*4;
        if (*verbosity > 1) emit messageAvailable("Freeing "+QString::number(memReleased) + " MB", "data");
        combinedImage[chip]->dataCurrent.swap(empty);
    }

    // successProcessing = true;
}
*/


/*
bool Data::setModeFlag(int chip, QString min, QString max)
{
    if (!successProcessing) return false;

    QString thresholds = "[" + min + "," + max + "]";
    for (auto &it : myImageList[chip] ) {
        // Try and read image; readImage() returns true immediately if image is already in memory
        //        if (!(it->readImage())) return false;
        //        if (!it->modeDetermined) {
        //            qDebug() << "QDEBUG: Data::setModeFlag(): Error: Mode should have been determined.";
        //            return false;
        //        }
        if (!min.isEmpty()) {
            if (it->skyValue < min.toFloat()) {
                it->validMode = false;
            }
        }
        if (!max.isEmpty()) {
            if (it->skyValue > max.toFloat()) it->validMode = false;
        }
        if (!it->validMode) {
            if (*verbosity > 0) emit messageAvailable(it->baseName + " : Mode "+it->skyValue+" is outside user-defined thresholds "
                                                      + thresholds + ". Image will be ignored when calculating master calibs.", "data");
        }
    }
    return true;
}
*/


// UNUSED
/*
bool Data::writeImages(int chip, QString statusString)
{
    if (!successProcessing) return false;
    if (userStop || userKill) return false;

    bool success = true;  // unused
    for (auto &it : myImageList[chip]) {
        it->processingStatus->statusString = statusString;
        it->writeImage();
    }
    return success;
}
*/


/*
QString Data::getMasterFilename(QString type, int chip)
{
    return mainDirName+"/"+type+"/"+type+"_"+QString::number(chip+1)+".fits";
}
*/

/*
QVector<long> Data::getOverscanArea(QString axis, int chip)
{
    QVector<long> overscanArea;

    // X-axis
    if (axis == "x") {
        QVector<int> min = instData->overscan_xmin;
        QVector<int> max = instData->overscan_xmax;
        if (!min.isEmpty() && !max.isEmpty()) {
            overscanArea << min[chip];
            overscanArea << max[chip];
        }
    }

    // Y-axis
    if (axis == "y") {
        QVector<int> min = instData->overscan_ymin;
        QVector<int> max = instData->overscan_ymax;
        if (!min.isEmpty() && !max.isEmpty()) {
            overscanArea << min[chip];
            overscanArea << max[chip];
        }
    }

    return overscanArea;
}
*/


/*
QStringList Data::collectNamesForChip(int chip)
{
    QStringList names;
    long k = 0;
    for (auto &it: imageInfo.chip) {
        if (it == chip) names << imageInfo.fullName[k];
        ++k;
    }
    return names;
}
*/

// UNUSED
/*
bool Data::containsUnsavedImages()
{
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        for (auto &it : myImageList[chip]) {
            if (!it->imageOnDrive) return false;
        }
    }

    return true;
}
*/

/*
QStringList Data::collectNamesForFilter(QString filter)
{
    QStringList names;
    long k = 0;
    for (auto &it: imageInfo.filter) {
        if (it == filter) names << imageInfo.fullName[k];
        ++k;
    }
    return names;
}
*/


/*
void Data::pushErrorOccurred()
{
    emit errorOccurredInMyImage();
}
*/

/*
void Data::printCfitsioError(QString funcName, int status)
{
    if (status) {
        CfitsioErrorCodes *errorCodes = new CfitsioErrorCodes(this);
        emit messageAvailable("Data::"+funcName+":<br>" + subDirName + " : " + errorCodes->errorKeyMap.value(status), "error");
        emit critical();
    }
}
*/
