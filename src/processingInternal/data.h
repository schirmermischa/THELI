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

// The 'data' class keeps the references to all FITS images
// encountered in a single directory.
// It triggers a 'read-from-disk' when an image is required for some task.
// It dumps images to disk if memory runs low or if the user requests it.

// It defines the various processing functions that can operate on the data.
// The processing functions read and update the successProcessing flag for each chip
// (similar to the cfitsio status flag)

#ifndef DATA_H
#define DATA_H

#include "mask.h"
#include "../myimage/myimage.h"
#include "../instrumentdata.h"
#include "../processingStatus/processingStatus.h"

#include <omp.h>

#include <QObject>
#include <QMap>
#include <QDir>
#include <QString>
#include <QVector>
#include <QLineEdit>

class Data : public QObject
{
    Q_OBJECT
public:
    explicit Data(instrumentDataType *instrumentData, Mask *detectorMask, QString maindirname,
                  QString subdirname, int *verbose, QObject *parent = nullptr);

    ~Data();

    struct ImageInfo {
        QStringList fullName;
        QStringList baseName;
        QStringList filter;
        QList<int> chip;
        QList<int> naxis1;
        QList<int> naxis2;
        QList<double> mjdobs;
    };
    ImageInfo imageInfo;

    Mask *mask;
    ProcessingStatus *processingStatus = nullptr;

    instrumentDataType *instData;

    bool successProcessing = true;

    QDir dir;
    QString dirName = "";
    QString status = "";
    QString statusAfterProcessing = "";
    QString mainDirName = "";
    QString subDirName = "";
    QString dataType = "";
    QMap<QString,int> mapName;
    QMap<int,int> mapNaxis1;
    QMap<int,int> mapNaxis2;

    bool userYield = false;
    bool userStop = false;
    bool userKill = false;

    omp_lock_t progressLock;

    float *progress;
    long numActiveImages = 0;
    float progressStepSize = 0.;
    float progressCombinedStepSize = 0.;

    // Pointing characteristics
    double RAcenter = -100.;
    double DECcenter = -100.;
    double searchRadius = -100.;

    // Multi-threading; params set externally by 'controller'
    int maxCPU = 1;                    // Overall, maximum number of CPUs to be used
    int maxExternalThreads = 1;        // Maximum number of threads working on chips; equal or smaller than maxCPU
    int maxInternalThreads = 1;        // Maximum number of threads working on images; equal or smaller than maxCPU - maxExternalThreads
    int maxThreadsIO = 1;              // Maximum number of IO threads
    long maxRAM = 0;                   // Max amount of RAM available for processing (MB)
    int currentExternalThreads = 0;    // The current number of external running threads (over different chips)
    int currentInternalThreads = 0;    // The current number of internal running threads (over images of the same chips)
    bool useGPU = false;
    int *verbosity;

    bool dataInitialized = false;
    bool isTaskRepeated = false;
    QVector<bool> staticModelDone;

    bool currentlyDebayering = false;

    // A 2D list, 1st axis has one entry for each chip.
    // The 2nd axis keeps references to all FITS files of that chip (in memory or on disk)
    QVector<QList<MyImage*>> myImageList;        // A list of all images per chip    [chip][image]
    QVector<QList<MyImage*>> bayerList;          // A list of all debayered images    (only needed for memory calculations)
    QVector<QList<MyImage*>> exposureList;       // A list of all images per exposure   [exposure][chip]

    // A list of master calibration files, one for each chip (if applicable for this structure)
    QVector<MyImage*> combinedImage;

    QString pathBackupL1 = "";
    QString pathBackupL2 = "";
    QString pathBackupL3 = "";
    QString statusBackupL1 = "";
    QString statusBackupL2 = "";
    QString statusBackupL3 = "";

    float maskValue = -1e9;
    long numImages = 0;
    long numMasterCalibs = 0;
    bool hasAllMasterCalibs = false;
    QVector<int> uniqueChips;  // length: numChips
//    QString checkGeometry();
    bool hasImages();
//    bool getHeaders();
    void clearImageInfo();
//    QStringList collectNamesForChip(int chip);
//    QStringList collectNamesForFilter(QString filter);
//    void maskInit(Mask &mask, long n, long m);
//    QVector<long> getOverscanArea(QString axis, int chip);
//    QString getMasterFilename(QString type, int chip);

    // Processing functions
    void combineImagesCalib(int chip, float (*combineFunction_ptr) (const QVector<float> &, const QVector<bool> &, long), QString nlow, QString nhigh,
                            QString dirName, QString subDirName, QString dataType);
    void combineImages(const int chip, const QString nlowString, const QString nhighString, const QString currentImage, const QString mode,
                       const QString dirName, const QString subDirName, QVector<bool> &dataStaticModelDone);
    void combineImages_newParallel(int chip, MyImage *masterCombined, QList<MyImage *> &backgroundList, QString nlow, QString nhigh, QString currentImage, QString mode, const QString subDirName);
//    void deleteMyImageList();
//    void forceStatus(int chip, QString status);
    void loadCombinedImage(int chip);
    void populate(QString statusString);
    void repopulate(int chip, QList<MyImage *> replacementList);
//    void resetDataCurrentToBackup(int chip);
//    bool setModeFlag(int chip, QString min, QString max);
    void writeCombinedImage(int chip);
//    bool writeImages(int chip, QString statusString);
    void resetUserAbort();

    // Flags that control whether a task is actually executed (or skipped)
    // These are set externally by the controller
    //   bool overscanFlag = false;
    //   bool biasFlag = false;
    //   bool darkFlag = false;
    //   bool flatoffFlag = false;
    //   bool flatFlag = false;
    bool rescaleFlag = false;    // Whether the images in this group have to be rescaled to the same mode when combining them  (FLAT: yes, BIAS: no)

    // Memory functions
//    float memoryCurrentFootprint(bool globalweights = false);
//    float memoryNeeded(int chip);
//    void memoryFreeDataX(int chip, QString dataX);
    void memorySetDeletable(int chip, QString dataX, bool deletable);
    void getGainNormalization();
    bool getPointingCharacteristics();
    void initGlobalWeight(int chip, Data *flatData, QString filter, bool sameWeight, QString flatMin, QString flatMax);
    void resetGlobalWeight(QString filter);
    void thresholdGlobalWeight(int chip, Data *comparisonData, QString filter, QString threshMin, QString threshMax);
    void detectDefects(int chip, Data *flatData, QString scienceFilter, bool sameWeight,
                       QString defectKernel, QString defectRowTol, QString defectColTol, QString defectClusTol);    
    void writeGlobalWeights(int chip, QString filter);
    bool collectMJD();

    // Reset functions for the taskInternal processes
    void resetProcessbackground();
    void resetObjectMasking();

    void populateExposureList();
    void getModeCombineImages(int chip);
    QVector<double> getKeyFromAllImages(QString key);

    void checkPresenceOfMasterCalibs();
    void reportModeCombineImages();
    void countUnsavedImages(long &numUnsavedLatest, long &numUnsavedAll);
//    bool containsUnsavedImages();
    void writeUnsavedImagesToDrive(bool includeBackup);

    void broadcastNumberOfFiles();
    int identifyClusters(QString toleranceString);
    void setSuccess(bool state);
    void resetSuccessProcessing();
    float releaseMemory(float RAMneededThisThread, float RAMneededAllThreads, float currentTotalmemoryUsed, QString mode = "");
    void protectMemory();
    void unprotectMemory(int chip);
    void releaseAllMemory();
    void restoreBackupLevel(QString backupDirName);
    void transferBackupInfo();
    void restoreFromDirectory(QString backupDirName);
    bool restoreFromBackupLevel(QString level, QString &newStatusRAM);
    void restoreRAWDATA();
    bool checkStatusConsistency();
    void emitStatusChanged();
    bool hasMatchingPartnerFiles(QString testDirName, QString suffix);
    bool checkTaskRepeatStatus(QString taskBasename);
    void resetStaticModel();
    void writeBackgroundModel(const int &chip, const QString &mode, const QString &basename, bool &staticImageWritten);
//    void getModeCombineImagesBackground(int chip, MyImage *image);
    void writeBackgroundModel_newParallel(int chip, MyImage *combinedBackgroundImage, QString mode, QString basename,
                                          int threadID, omp_lock_t &backLock, bool &staticImageWritten);
    void cleanBackgroundModelStatus();
    bool checkForRawData();
    void applyMask(int chip, QString filter);
    void resetBackupInfo();
    void removeCatalogs();
    bool doesCoaddContainRaDec(const QString &refRA, const QString &refDEC);
public slots:
    void setMemoryLockReceived(bool locked);
    void setWCSLockReceived(bool locked);
    void modelUpdateReceiver(QString basename, QString chipName);
    void pushMessageAvailable(QString message, QString type);
    void pushWarning();
    void pushCritical();
    void criticalFromQueryReceived();
//    void pushErrorOccurred();

private:
    QString thelidir;
    QString userdir;

    bool workerInit = false;
    bool workerThreadInit = false;

    float maskVal = -1.e9;
//    void incrementCurrentThreads(int &currentThreads, omp_lock_t &lock);
//    void decrementCurrentThreads(int &currentThreads, omp_lock_t &lock);
//    void getFixedHeaderKeys(QString filename, QStringList &badImages);
    QVector<float> getNormalizedRescaleFactors(int chip, QVector<long> &goodIndex, QString mode);
//    void printCfitsioError(QString funcName, int status);
    void doImagesOverlap(MyImage &imgRef, MyImage &imgTest, float tolerance);
    bool checkForUnassignedImages(int &groupNumber);
    void findOverlappingImages(MyImage *img, float tolerance);
    void releaseMemoryCombined(float &RAMfreed, const float RAMneededThisThread);
    void releaseMemoryIndividual(const QStringList &datalist, float &RAMfreed, const float RAMneededThisThread);
    void removeCurrentFITSfiles();
    void releaseMemoryDebayer(float &RAMfreed, const float RAMneededThisThread);

private slots:

signals:
    void modelUpdateNeeded(QString basename, QString chipName);
    // Passed through from MyImage:
    void messageAvailable(QString message, QString type);
    void showMessageBox(QString trigger, QString part1, QString part2);
    void appendOK();
    void critical();
    void warning();
    void progressUpdate(float progress);
    void addToProgressBar(float differential);
    void resetProgressBar();
    void setMemoryLock(bool locked);
    void setWCSLock(bool locked);
    void globalModelUpdateNeeded();
    void updateModelHeaderLine();
    void statusChanged(QString newStatus);
    void errorOccurredInMyImage();
};

#endif // DATA_H
