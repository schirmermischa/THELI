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

#include "myimage.h"
#include "../functions.h"
#include "../tools/polygon.h"
#include "../tools/tools.h"
#include "../tools/cfitsioerrorcodes.h"
#include "../processingInternal/data.h"
#include "../processingStatus/processingStatus.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QString>
#include <QTest>

// same as pushdown(), without file movement
void MyImage::makeMemoryBackup()
{
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images

    // TODO: implement a finer granularity memory check (sufficient RAM available?)
    if (!minimizeMemoryUsage) {
        dataBackupL3 = dataBackupL2;
        dataBackupL2 = dataBackupL1;
    }
    dataBackupL1 = dataCurrent;

    statusBackupL3 = statusBackupL2;
    statusBackupL2 = statusBackupL1;
    statusBackupL1 = processingStatus->statusString;

    emit modelUpdateNeeded(baseName, chipName);

    // implement:
    // processingStatus->statusString = statusCurrentNew;
}

// UNUSED?
void MyImage::makeDriveBackup(QString backupDirName, QString statusOld)
{
    if (!successProcessing) return;
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images

    // Current and backup paths
    QString currentPath = path;
    QString backupPath = path+"/"+backupDirName+"/";
    QString oldName = chipName+statusOld+".fits";
    QFile image(currentPath+"/"+oldName);

    // Do nothing if the image does not exist on drive (i.e. is in memory only)
    if (!image.exists()) return;

    moveFile(oldName, currentPath, backupPath);
}

void MyImage::makeBackgroundBackup()
{
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images

    // make backup file (if the FITS file exists)
    mkAbsDir(pathBackupL1);
    moveFile(baseNameBackupL1+".fits", path, pathBackupL1, true);
    backupL1OnDrive = true;

    emit modelUpdateNeeded(baseName, chipName);
}

// Push data one step into the backup structure
void MyImage::pushDown(QString backupDir)
{
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images

    // MEMORY
    pushDownToL3();
    pushDownToL2();
    pushDownToL1(backupDir);

    // DRIVE
    // make backup file (if the FITS file exists)
    mkAbsDir(pathBackupL1);
    moveFile(baseNameBackupL1+".fits", path, pathBackupL1, true);
    backupL1OnDrive = true;

    emit modelUpdateNeeded(baseName, chipName);
}

// Push dataCurrent to backupL1
void MyImage::pushDownToL1(QString backupDir)
{
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images

    // MEMORY
    dataBackupL1 = dataCurrent;
    backupL1InMemory = imageInMemory;
    statusBackupL1 = processingStatus->statusString;
    dataBackupL1_deletable = dataCurrent_deletable;
    pathBackupL1 = path+"/"+backupDir;
    baseNameBackupL1 = baseName;
}

// Push dataCurrent to backupL2
void MyImage::pushDownToL2()
{
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images

    if (!minimizeMemoryUsage) {
        dataBackupL2 = dataBackupL1;
        backupL2InMemory = backupL1InMemory;
        statusBackupL2 = statusBackupL1;
        pathBackupL2 = pathBackupL1;
        baseNameBackupL2 = baseNameBackupL1;
        dataBackupL2_deletable = dataBackupL1_deletable;
    }
}

// Push dataCurrent to backupL3
void MyImage::pushDownToL3()
{
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images

    if (!minimizeMemoryUsage) {
        dataBackupL3 = dataBackupL2;
        backupL3InMemory = backupL2InMemory;
        statusBackupL3 = statusBackupL2;
        pathBackupL3 = pathBackupL2;
        baseNameBackupL3 = baseNameBackupL2;
        dataBackupL3_deletable = dataBackupL2_deletable;
    }
}

// Pull data up one step from the backup structure
void MyImage::pullUp()
{
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images

    // DRIVE
    deleteFile(baseName+".fits", path);
    // restore backup FITS file
    moveFile(baseNameBackupL1+".fits", pathBackupL1, path, true);

    // MEMORY
    pullUpFromL1();
    pullUpFromL2();
    pullUpFromL3();

    emit modelUpdateNeeded(baseName, chipName);
}

// Pull data up one step from the backup structure
bool MyImage::makeL1Current()
{
    if (activeState != ACTIVE) return true;    // Don't change location of deactivated images; don't trigger error

    bool success = true;
    // DRIVE
    success *= deleteFile(baseName+".fits", path);
    // restore backup FITS file
    success *= moveFile(baseNameBackupL1+".fits", pathBackupL1, path, true);

    // L1 to L0
    dataCurrent = dataBackupL1;
    processingStatus->statusString = statusBackupL1;
    processingStatus->statusToBoolean(processingStatus->statusString);
    dataCurrent_deletable = false;
    baseName = baseNameBackupL1;
    imageInMemory = backupL1InMemory;
    imageOnDrive = backupL1OnDrive;

    // MEMORY
    pullUpFromL2();
    pullUpFromL3();

    wipeL3();

    emit modelUpdateNeeded(baseName, chipName);
    return success;
}

// Make L2 current
bool MyImage::makeL2Current()
{
    if (activeState != ACTIVE) return true;    // Don't change location of deactivated images; don't trigger error

    bool success = true;
    // DRIVE
    success *= deleteFile(baseName+".fits", path);
    // restore backup FITS file
    success *= moveFile(baseNameBackupL2+".fits", pathBackupL2, path, true);

    // L2 to L0
    dataCurrent = dataBackupL2;
    processingStatus->statusString = statusBackupL2;
    processingStatus->statusToBoolean(processingStatus->statusString);
    dataCurrent_deletable = false;
    baseName = baseNameBackupL2;
    imageInMemory = backupL2InMemory;
    imageOnDrive = backupL2OnDrive;

    // L3 to L2
    pullUpFromL3();

    // wipe L1
    wipeL1();
    wipeL3();

    emit modelUpdateNeeded(baseName, chipName);
    return success;
}

// make L3 current
bool MyImage::makeL3Current()
{
    if (activeState != ACTIVE) return true;    // Don't change location of deactivated images; don't trigger error

    bool success = true;
    // DRIVE
    success *= deleteFile(baseName+".fits", path);
    // restore backup FITS file
    success *= moveFile(baseNameBackupL3+".fits", pathBackupL3, path, true);

    // L3 to L0
    dataCurrent = dataBackupL3;
    processingStatus->statusString = statusBackupL3;
    processingStatus->statusToBoolean(processingStatus->statusString);
    dataCurrent_deletable = false;
    baseName = baseNameBackupL3;
    imageInMemory = backupL3InMemory;
    imageOnDrive = backupL3OnDrive;

    // wipe L1 and L2
    wipeL1();
    wipeL2();

    emit modelUpdateNeeded(baseName, chipName);
    return success;
}

void MyImage::pullUpFromL3()
{
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images

    dataBackupL2 = dataBackupL3;
    statusBackupL2 = statusBackupL3;
    pathBackupL2 = pathBackupL3;
    dataBackupL2_deletable = dataBackupL3_deletable;
    baseNameBackupL2 = baseNameBackupL3;
    backupL2InMemory = backupL3InMemory;
    dataBackupL3_deletable = true;
    dataBackupL3.clear();
    dataBackupL3.squeeze();
}

void MyImage::pullUpFromL2()
{
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images

    dataBackupL1 = dataBackupL2;
    statusBackupL1 = statusBackupL2;
    pathBackupL1 = pathBackupL2;
    dataBackupL1_deletable = dataBackupL2_deletable;
    baseNameBackupL1 = baseNameBackupL2;
    backupL1InMemory = backupL2InMemory;
}

void MyImage::pullUpFromL1()
{
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images

    dataCurrent = dataBackupL1;
    processingStatus->statusString = statusBackupL1;
    processingStatus->statusToBoolean(processingStatus->statusString);
    dataCurrent_deletable = false;
    baseName = baseNameBackupL1;
    imageInMemory = backupL1InMemory;
}

void MyImage::wipeL1()
{
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images

    dataBackupL1.clear();
    dataBackupL1.squeeze();
    statusBackupL1 = "";
    pathBackupL1 = "";
    dataBackupL1_deletable = true;
    baseNameBackupL1 = "";
    backupL1InMemory = false;
}

void MyImage::wipeL2()
{
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images

    dataBackupL2.clear();
    dataBackupL2.squeeze();
    statusBackupL2 = "";
    pathBackupL2 = "";
    dataBackupL2_deletable = true;
    baseNameBackupL2 = "";
    backupL2InMemory = false;
}

void MyImage::wipeL3()
{
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images

    dataBackupL3.clear();
    dataBackupL3.squeeze();
    statusBackupL3 = "";
    pathBackupL3 = "";
    dataBackupL3_deletable = true;
    baseNameBackupL3 = "";
    backupL3InMemory = false;
}

// For some tasks (e.g. source cat creation) we do not need to push down data into the backup levels
void MyImage::setupDataInMemorySimple(bool determineMode)
{
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images

    // Load the image if not in yet memory
    readImage(determineMode);

    emit modelUpdateNeeded(baseName, chipName);
}

// CHECK: Last argument probably not needed
void MyImage::setupData(bool isTaskRepeated, bool createBackup, bool determineMode, QString backupDir)
{
    if (!successProcessing) return;
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images

    // Protect dataCurrent
    dataCurrent_deletable = false;

    //    if (createBackup && backupDir.isEmpty()) emit messageAvailable("MyImage::setupData(): Cannot restore backup data from backup dir!", "error");

    // CASE 1: The task has not been executed before
    if (!isTaskRepeated) {
        readImage(determineMode);
        // Push down data by one level (at all levels) if required
        if (createBackup) pushDown(backupDir);
    }

    // CASE 2: The task has been executed before and we re-do it: Restore level 1 backup
    if (isTaskRepeated) {
        // Restoring from memory
        if (backupL1InMemory) {
            if (*verbosity > 2) emit messageAvailable(baseName + " : Task repeated, restoring data from RAM (backup level L1)", "image");
            pullUpFromL1();
        }
        // Restoring from drive
        else {
            if (*verbosity > 2) emit messageAvailable(baseName + " : Task repeated, restoring data from backup FITS in "+pathBackupL1, "image");
            // Right after launch, nothing is initialized and we must do that first using the full approach. Otherwise, we just read the data array.
            if (!headerInfoProvided) readImageBackupL1Launch();
            else readImageBackupL1();
            imageInMemory = true;
            backupL1InMemory = true;
        }
    }

    emit modelUpdateNeeded(baseName, chipName);
}

// If a task is repeated, replace dataCurrent with the backup copy.
// Otherwise continue with data current, and push down the backup data one level
void MyImage::setupBackgroundData(const bool &isTaskRepeated, const QString &backupDir)
{
    if (!successProcessing) return;
    if (backgroundPushedDown) return;
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images

    dataCurrent_deletable = false;
    dataBackupL1_deletable = false;

    // CASE 1: The task has not been executed before
    if (!isTaskRepeated) {
        // Nothing is in backupL1 yet, either after restart, or because RAM is low.
        // No push-down has happened yet
        if (!backgroundPushedDown) {
            if (!minimizeMemoryUsage) {
                pushDownToL3();
                pushDownToL2();
            }
            readImage(true);                  // if not yet in memory
            pushDownToL1(backupDir);          // Create a safe copy of the non-subtracted data
            dataBackupL1_deletable = false;   // set to 'true' in pushDownToL1(), but we must protect it
            backgroundPushedDown = true;
        }
    }

    // CASE 2: The task has been executed before (we have PAB images present in dataCurrent and PA images in backupL1)
    else {
        // Restoring from drive if not yet in memory
        if (!backupL1InMemory) {
            if (*verbosity > 2) emit messageAvailable(baseName + " : Task repeated, restoring data from backup FITS in "+pathBackupL1, "image");
            if (!headerInfoProvided) readImageBackupL1Launch();    // Right after launch, nothing is initialized and we must do that first using the full approach.
            else readImageBackupL1();                              // Otherwise, we just read the data array.
            backgroundPushedDown = true;
        }
    }

    emit modelUpdateNeeded(baseName, chipName);
}

void MyImage::setupBackgroundData_newParallel(const bool &isTaskRepeated, const QString &backupDir)
{
    if (!successProcessing) return;
    if (backgroundPushedDown) return;
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images


    omp_set_lock(&backgroundLock);  // Must not be done simultaneously

    dataCurrent_deletable = false;
    dataBackupL1_deletable = false;

    // CASE 1: The task has not been executed before
    if (!isTaskRepeated) {
        // Nothing is in backupL1 yet, either after restart, or because RAM is low.
        // No push-down has happened yet
        if (!backgroundPushedDown) {
            if (!minimizeMemoryUsage) {
                pushDownToL3();
                pushDownToL2();
            }
            readImage(true);                  // if not yet in memory
            pushDownToL1(backupDir);          // Create a safe copy of the non-subtracted data
            dataBackupL1_deletable = false;   // set to 'true' in pushDownToL1()
            backgroundPushedDown = true;
        }
    }

    // CASE 2: The task has been executed before (we have PAB images present in dataCurrent and PA images in backupL1)
    else {
        // Restoring from drive if not yet in memory
        if (!backupL1InMemory) {
            if (*verbosity > 2) emit messageAvailable(baseName + " : Task repeated, restoring data from backup FITS in "+pathBackupL1, "image");
            if (!headerInfoProvided) readImageBackupL1Launch();    // Right after launch, nothing is initialized and we must do that first using the full approach.
            else readImageBackupL1();                              // Otherwise, we just read the data array.
            backgroundPushedDown = true;
        }
    }

    emit modelUpdateNeeded(baseName, chipName);

    omp_unset_lock(&backgroundLock);
}

// If a task is repeated, replace dataCurrent with the backup copy.
// Otherwise continue with data current, and push down the backup data one level
void MyImage::setupCalibDataInMemory(bool createBackup, bool determineMode, bool mustRereadFromDisk)
{
    if (!successProcessing) return;
    if (activeState != ACTIVE) return;    // Don't change location of deactivated images


    // Restore level 1 backup (in case we reprocess the data).
    // Either get it from memory, or read it from disk
    // Backup only needed for FLATS (biases, darks and flatoff are not 'processed' at this point, but flats are bias-subtracted)
    if (backupL1InMemory) {
        // we are here only for flats; bias/darks are created without backup copy
        // TODO: this is not very safe. it would be better if we pass the Data type (BIAS, FLAT etc) along, and then
        // make the case distinctions concerning this value
        dataCurrent = dataBackupL1;
        imageInMemory = backupL1InMemory;
    }
    else {
        // In case of flats, the pixels were bias-subtracted. If the machine has low RAM, then "backupL1inMemory == false"
        // but it could be that "imageInMemory == true", then readImage() does not actually restore the original pixel values.
        // In this case, we must force a reread to refresh dataCurrent, but only for flats)
        if (mustRereadFromDisk) imageInMemory = false;
        readImage(determineMode);
        // Create backup copy, unless we don't need it (e.g. swarpfilter, or for bias/dark/flatoff)
        if (createBackup) {              // true if not specified
            dataBackupL1 = dataCurrent;
            backupL1InMemory = true;
        }
    }

    emit modelUpdateNeeded(baseName, chipName);
}

// unused
void MyImage::dumpToDriveIfPossible()
{
    if (processingFinished && !imageOnDrive && imageInMemory) {
        writeImage();
        unprotectMemory();
    }
}

void MyImage::freeData(QVector<float> &data)
{
    data.clear();
    data.squeeze();
    if (&data == &dataCurrent) imageInMemory = false;
    else if (&data == &dataWeight) weightInMemory = false;
    else if (&data == &dataBackupL1) backupL1InMemory = false;
    else if (&data == &dataBackupL2) backupL2InMemory = false;
    else if (&data == &dataBackupL3) backupL3InMemory = false;

    emit modelUpdateNeeded(baseName, chipName);
}

void MyImage::freeData()
{
    dataCurrent.clear();
    dataCurrent.squeeze();
    imageInMemory = false;
    emit modelUpdateNeeded(baseName, chipName);
}

// this happens only inside memoryLock set in the controller
// MUST USE MEMORY LOCCK!
float MyImage::freeData(QString type)
{
    bool released = false;

    // If the image has never been loaded, this function will crash in several places.
    // Why is not clear to me, perhaps because some strings and e.g. databackground are not initialized;

    if (type == "dataBackground" && dataBackground_deletable && dataBackground.capacity() > 0) {
        // TODO / CHECK: comment these if causing problems
        dataBackground.clear();
        dataBackground.squeeze();
        released = true;
    }
    else if (type == "dataBackupL1" && dataBackupL1_deletable && dataBackupL1.capacity() > 0) {
        dataBackupL1.clear();
        dataBackupL1.squeeze();
        backupL1InMemory = false;
        released = true;
    }
    else if (type == "dataBackupL2" && dataBackupL2_deletable && dataBackupL2.capacity() > 0) {
        dataBackupL2.clear();
        dataBackupL2.squeeze();
        backupL2InMemory = false;
        released = true;
    }
    else if (type == "dataBackupL3" && dataBackupL3_deletable && dataBackupL3.capacity() > 0) {
        dataBackupL3.clear();
        dataBackupL3.squeeze();
        backupL3InMemory = false;
        released = true;
    }
    else if (type == "dataWeight" && dataWeight_deletable && dataWeight.capacity() > 0) {
        // weights are always writtwen to drive (for swarp)
        dataWeight.clear();
        dataWeight.squeeze();
        weightInMemory = false;
        released = true;
    }
    else if (type == "dataCurrent" && dataCurrent_deletable && dataCurrent.capacity() > 0) {
        // Must write image to drive if not yet the case
        if (!imageOnDrive) {
            writeImage();
            imageOnDrive = true;
        }
        dataCurrent.clear();
        dataCurrent.squeeze();
        imageInMemory = false;
        released = true;
    }
    else if (type == "all") {
        // used if a project is changed; release all memory
        if (dataBackground.capacity() > 0) {
            dataBackground.clear();
            dataBackground.squeeze();
        }
        if (dataBackupL1.capacity() > 0) {
            dataBackupL1.clear();
            dataBackupL1.squeeze();
            backupL1InMemory = false;
        }
        if (dataBackupL2.capacity() > 0) {
            dataBackupL2.clear();
            dataBackupL2.squeeze();
            backupL2InMemory = false;
        }
        if (dataBackupL3.capacity() > 0) {
            dataBackupL3.clear();
            dataBackupL3.squeeze();
            backupL3InMemory = false;
        }
        if (dataWeight.capacity() > 0) {
            // weights are always writtwen to drive (for swarp)
            dataWeight.clear();
            dataWeight.squeeze();
            weightInMemory = false;
        }
        if (dataCurrent.capacity() > 0) {
            dataCurrent.clear();
            dataCurrent.squeeze();
            imageInMemory = false;
        }
    }
    emit modelUpdateNeeded(baseName, chipName);

    if (released) return naxis1*naxis2*sizeof(float) / 1024. / 1024.;
    else return 0.;
}

void MyImage::protectMemory()
{
    // Nothing we might change during nominal processing may be touched
    dataCurrent_deletable = false;
    dataBackground_deletable = false;
    dataBackupL1_deletable = false;
    dataWeight_deletable = false;
}


// After an image was written to drive (or isn't needed right away elsewhere) we can set all memory to deletable
void MyImage::unprotectMemory()
{
    // Memory is up for grabs
    dataCurrent_deletable = true;
    dataBackground_deletable = true;
    dataBackupL1_deletable = true;
    dataBackupL2_deletable = true;
    dataBackupL3_deletable = true;
    dataWeight_deletable = true;
}

void MyImage::unprotectMemoryWeight()
{
    // Memory is up for grabs
    dataWeight_deletable = true;
}

// UNUSED
/*
// Selective deletable status
void MyImage::setDeletable(QString dataX, bool deletable)
{
    if (dataX == "dataCurrent") dataCurrent_deletable = deletable;
    else if (dataX == "dataBackupL1") dataBackupL1_deletable = deletable;
    else if (dataX == "dataBackupL2") dataBackupL2_deletable = deletable;
    else if (dataX == "dataBackupL3") dataBackupL3_deletable = deletable;
    else if (dataX == "dataWeight") dataWeight_deletable = deletable;
    else if (dataX == "dataBackground") dataBackground_deletable = deletable;
}
*/

void MyImage::releaseMemoryForBackground()
{
    if (enteredBackgroundWindow && leftBackgroundWindow) {
        if (!backupL1OnDrive) writeImageBackupL1();
        freeData("dataBackupL1");
        if (minimizeMemoryUsage) freeAll();
    }
}

void MyImage::unprotectMemoryForBackground()
{
    if (enteredBackgroundWindow && leftBackgroundWindow) {
        dataBackupL1_deletable = true;
        dataBackground_deletable = true;
        if (minimizeMemoryUsage) freeAll();        // implies that we write the FITS image before calling this function!
    }
}

void MyImage::freeAncillaryData(QVector<float> &data)
{
    QVector<float>().swap(data);
}

void MyImage::freeAll()
{
    emit setMemoryLock(true);
    freeData(dataBackupL1);
    freeData(dataBackupL2);
    freeData(dataBackupL3);
    freeData(dataCurrent);
    freeData(dataWeight);
    emit modelUpdateNeeded(baseName, chipName);
    emit setMemoryLock(false);
}
