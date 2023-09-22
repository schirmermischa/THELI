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

// The controller class keeps track of all data defined in the data directory tree,
// and whether it is kept in memory or on disk, throughout the processing.
// It also defines all the processing functions, kept separately in processing.cc

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <omp.h>

#include "data.h"
#include "mask.h"
#include "dockwidgets/memoryviewer.h"
#include "dockwidgets/confdockwidget.h"
#include "dockwidgets/monitor.h"
#include "../threading/scampworker.h"
#include "../threading/swarpworker.h"
#include "../tools/fileprogresscounter.h"
#include "photinst.h"
#include "../iview/iview.h"
#include "../query/query.h"
#include "../tools/cpu.h"
#include "../tools/ram.h"

#include <QObject>
#include <QList>
#include <QMainWindow>
#include <QDir>
#include <QLineEdit>
#include <QTextStream>
#include <QCheckBox>
#include <QComboBox>
#include <QProcess>
#include <QMap>

class MainWindow;   // forward declaration to access GUI members
class Data;         // circular include directives triggered by memoryworker
class Query;        // circular include directives triggered by memoryworker
class MemoryViewer;

class Controller : public QMainWindow
{
    Q_OBJECT

private:
    bool taskRunning = false;

    QString thelidir;
    QString userdir;

    QMap<QString,QString> commentMap;

    struct QPairSecondComparer
    {
        template<typename T1, typename T2>
        bool operator()(const QPair<T1,T2> & a, const QPair<T1,T2> & b) const
        {
            return a.second < b.second;
        }
    };

    QDir dir;
    QString dirName;

    QString tmpCoaddDir;
    Data *tmpCoaddData;
    //    IView *checkplotViewer;

    FileProgressCounter *numberFileCounter;
    FileProgressCounter *sizeFileCounter;

    long coaddNumImages = 0;
    QString swarpCommand;
    QString coaddDirName;
    QString coaddScienceDir;
    QString scampScienceDir;
    QString anetDir;
    QString scampHeadersDir;
    QString scampPlotsDir;
    QString coaddUniqueID;
    QString scampCommand;
    QString sourceExtractorCommandOptions;
    QString scampDir;
    int processID;
    bool scampSolutionAcceptanceState = false; 

    QVector<PhotInst*> photInstruments;

    QVector<QVector<long>> overscanX;
    QVector<QVector<long>> overscanY;
    QVector<QVector<long>> dataSection;

    bool globalweights_created = false;

    long splitterMemoryUsed = 0;

    double rad = 3.1415926535 / 180.;

    float minCoaddSaturationValue = 1.e12;

    QString statusAtDistribute = "";

    instrumentDataType altInstData;   // For instruments with e.g. simultaneous optical and NIR detectors requiring different masks during HDU reformatting

    QMap<QString,QStringList> headerDictionary;
    QMap<QString,QString> filterDictionary;
    QMap<QString,QString> instrumentDictionary;

    int recurseCounter = 0;

    // Processing functions
    void checkSuccessProcessing(const Data *data);
//    void decrementCurrentThreads(omp_lock_t &lock);
    QVector<QString> getBackgroundThresholds(const int loop, const bool twoPass, const QString DT, const QString DMIN, bool &doSourceDetection);
    void getDetectorSections();
    QList<QVector<float> > getNonlinearityCoefficients();
//    void incrementCurrentThreads(omp_lock_t &lock);
    void memoryDecideDeletableStatus(Data *data, bool deletable);
    void parseDataDir(QLineEdit *le, QList<Data *> &DT_x);
    void pushParallelizationToData(QList<Data *> DT_x);
    void pushParallelizationToData(Data *data);

    //    void rejectChipsWithBrightStars(QList<MyImage *> backgroundList, QList<QVector<double> > &brightStarList);
//    int reserveAvailableThreads(omp_lock_t &lock);
    void retrieveBrightStars(Data *data, QList<QVector<double>> &brightStarList);
//    void makeThreadsAvailable(omp_lock_t &lock, int numberThreadsBlocked);
    void selectImagesDynamically(const QList<MyImage *> &backgroundList, const double &mjd_ref);
    void selectImagesStatically(const QList<MyImage *> &backgroundList, MyImage *scienceImage);
    void selectImagesFromSequence(QList<MyImage *> &backgroundList, const int &nGroups, const int &nLength, const int &currentExp);

    // Data handling
//    void restoreData(MyImage *myImage, QString backupDir);

    // Sky subtraction
    void skysubModel(Data *scienceData, QString DT, QString DMIN, QString expFactor, QString kernelWidth);
    void skysubPolynomialFit(Data *scienceData);
    QList<MyImage*> measureSkyInBlankRegions(Data *scienceData, QString method = "");
    void skysubConstantFromArea(Data *scienceData);
    void skysubConstantReferenceChip(Data *scienceData, QString DT, QString DMIN, QString expFactor, QString kernelWidth);
    void skysubConstantEachChip(Data *scienceData, QString DT, QString DMIN, QString expFactor, QString kernelWidth);

    // Coaddition
    void coaddPrepare(QString filterArg);
    void coaddSmoothEdge();
    void coaddPrepareProjectPM(QFile &headerFileOld, QString newHeaderName, QString refDE, double mjdobsZero, double mjdobsNow);
    void coaddPrepareProjectRotation();
    void coaddPrepareBuildSwarpCommand(QString refRA, QString refDE);
    void coaddResampleBuildSwarpCommand(QString imageList, int i);
    void coaddCoadditionBuildSwarpCommand(QString imageList);
    void foldCoaddFits();
    float coaddTexptime = 0.;
    float coaddSkyvalue = 0.;
    QString coaddFilter = "";
    float coaddGain = 0.;
    double mjdStart = 0.;
    double mjdEnd = 0.;
    double mjdMedian = 0.;
    QString minDateObs = "";   // for the coadded image
    QString maxDateObs = "";   // for the coadded image

    // Astrometry
    bool manualCoordsUpdate(Data *scienceData, QString mode);
    void prepareScampRun(Data *scienceData);
    long prepareScampCats(Data *scienceData, long &totNumObjects);
    QString getUserParamLineEdit(const QLineEdit *le);
    QString getUserParamCheckBox(const QCheckBox *cb);
    QString getUserParamComboBox(const QComboBox *cb);
    void buildScampCommand(Data *scienceData);
    void sendMemoryPreferenceToImages(QList<Data*> DT_x);
    int countBackgroundImages(QList<MyImage *> list, QString baseName);
    //    int getInternalThreads(int chip);

    void populateCommentMap();
    void pushBeginMessage(const QString idstring, const QString targetdir);
    void pushConfigCoadd();
    void pushConfigAstromphotom();
    void pushConfigGetCatalogFromWeb();
    void pushConfigGetCatalogFromImage();
    void pushConfigCreatesourcecat();
    void pushEndMessage(QString idstring, const QString targetdir);
    bool idChipsWithBrightStars(Data *skyData, QList<QVector<double> > &brightStarList);
    void flagImagesWithBrightStars(const QList<MyImage *> &backgroundList);
    void getNumberOfActiveImages(Data *&data);
//    void incrementProgress();
//    void incrementProgressHalfStep();
//    void incrementProgressCombinedStep();
//    void rereadDataDir(QLineEdit *le, QList<Data *> &DT_x);
    void doCrossCorrelation(Data *scienceData);
    long coaddCoadditionGetSize();
    void splitScampHeaders();
    void scampCalcFluxscale();
    int getMaxPhotInst();
//    void provideHeaderInfo(Data *scpublic QMainWindowienceData);
    void buildSourceExtractorCommandOptions();
    void detectionInternal(Data *scienceData, QString minFWHM, QString maxFlag, QString maxEll);
    void detectionSourceExtractor(Data *scienceData, QString minFWHM, QString maxFlag, QString maxEll);
    void mergeInternal(Data *scienceData, QString minFWHM, QString maxFlag, QString maxEll);
    void mergeSourceExtractor(Data *scienceData);
    void populateHeaderDictionary();
    void populateFilterDictionary();
    long makeListofAllImages(QList<MyImage *> &allMyImages, Data *data);

    //    void updateMyImagesWithScampSolution(Data *scienceData);
    void doImageQualityAnalysis();
    void collectGaiaRaDec(MyImage *image, QVector<double> &dec, QVector<double> &ra, QVector<QVector<double> > &output);
    void releaseMemory(float RAMneeded, int numThreads, QString mode = "");
    long getNumObjectsScampCat(QString cat);
    void satisfyMaxMemorySetting();
    long getNumObjectsSourceExtractorCat(QString cat);
    void emitSourceCountMessage(long &nobj, QString baseName);
    void printCfitsioError(QString funcName, int status);
    void printCfitsioWarning(QString funcName, int status);
    void updateImageAndData(MyImage *image, Data *data);
    void maskObjectsInSkyImagesPass1(const int chip, Data *skyData, Data *scienceData, const bool twoPass, const QString dt,
                                     const QString dmin, const bool convolution, const QString expFactor);
    void maskObjectsInSkyImagesPass2(const int chip, Data *skyData, Data *scienceData, const bool twoPass, const QString dt,
                                     const QString dmin, const bool convolution, const QString expFactor,
                                     const bool rescaleModel);
    bool filterBackgroundList(const int chip, Data *skyData, MyImage *it,  QString &backExpList, const int nGroups, const int nLength,
                              const int currentExposure, const QString mode);
//    void maskObjectsInSkyImagesPass1_newParallel(Data *skyData, Data *scienceData, const QList<MyImage *> &backgroundList,
//                                                 const bool twoPass, const QString dt, const QString dmin, const bool convolution,
//                                                 const QString expFactor, const int threadID);
//    void maskObjectsInSkyImagesPass2_newParallel(Data *skyData, Data *scienceData, MyImage *combinedImage, const QList<MyImage *> &backgroundList,
//                                                 const bool twoPass, const QString dt, const QString dmin, const bool convolution,
//                                                 const QString expFactor, const int chip, const bool rescaleModel,
//                                                 const int threadID, const QString mode);
    void sendBackgroundMessage(const QString mode, const bool staticmodeldone, const QString basename, const int pass);
    /*
    void processBackgroundStatic(Data *scienceData, Data *skyData, const float nimg, QVector<QString> &numBackExpList,
                                 QString dt, QString dmin, QString expFactor, QString nlow1,
                                 QString nhigh1, QString nlow2, QString nhigh2,
                                 const bool twoPass, const bool convolution, const bool rescaleModel,
                                 const int nGroups, const int nLength, QVector<bool> &staticImagesWritten);
    void processBackgroundDynamic(Data *scienceData, Data *skyData, const float nimg, QVector<QString> &numBackExpList,
                                  QString dt, QString dmin, QString expFactor, QString nlow1,
                                  QString nhigh1, QString nlow2, QString nhigh2,
                                  const bool twoPass, const bool convolution, const bool rescaleModel,
                                  const int nGroups, const int nLength, QVector<bool> &staticImagesWritten);
    */
    void processBackground(Data *scienceData, Data *skyData, const float nimg, QVector<QString> &numBackExpList,
                           const QString dt, const QString dmin, const QString expFactor, const QString nlow1,
                           const QString nhigh1, const QString nlow2, const QString nhigh2,
                           const bool twoPass, const bool convolution, const bool rescaleModel,
                           const int nGroups, const int nLength, QString mode, QVector<bool> &staticImagesWritten);

    void uniformMJDOBS(QDir &dir);
    void finalizeSplitter(Data *data);
    bool updateDataDirs(Data *data);

    void initAltInstrumentData(QString instrumentNameFullPath);
    void resetAltInstrumentData();
    void testOverscan(QVector<int> &overscan);
    void provideAlternativeMask();
    void pushConfigHDUreformat();
    void pushConfigProcessbias();
    void pushConfigProcessdark();
    void pushConfigProcessflat();
    void pushConfigProcessflatoff();
    void pushConfigBackground();
    void pushConfigCollapse();
    void pushConfigBinnedpreview();
    void pushConfigGlobalweight();
    void pushConfigIndividualweight();
    void pushConfigSkysubModel();
    void pushConfigSkysubConst();
    void pushConfigSkysubPoly();
    void flagLowDetectionImages(Data *scienceData, long &numExpRejected, long &numImgRejected);
    void doDataFitInRAM(const long nImages, const long storageSize);
    bool testResetDesire(const Data *data);
    void runAnet(Data *scienceData);
    void prepareAnetRun(Data *scienceData);
    long getNumAnetChips(QString ahead);
    long makeAnetHeaderList(Data *scienceData);
    bool setupBackgroundList(int chip, Data *skyData, const QString &chipName);
    void combineAllBackgroundUsabilityFlags(const QList<MyImage *> &backgroundList);
    void getMinimumSaturationValue();
    bool isExposureActive(QList<MyImage *> exposure);

private slots:
//    void displayRAMload();
//    void displayMemoryTotalUsed();
//    void displayCPUload();
    void displayProgress();
//    void displayDriveSpace();

    // The following can also be under 'private', but then the declaration must be preceded like this:
    // Q_INVOKABLE QString taskHDUreformat();
    void taskInternalProcessbias();
    void taskInternalProcessdark();
    void taskInternalProcessflatoff();
    void taskInternalProcessflat();
    void taskInternalProcessscience();
    void taskInternalBackground();
    void taskInternalCollapse();
    void taskInternalBinnedpreview();
    void taskInternalGlobalweight();
    void taskInternalIndividualweight();
    void taskInternalCreatesourcecat();
    void taskInternalSkysub();
    void taskInternalCoaddition();
    void taskInternalGetCatalogFromWEB();
    void taskInternalGetCatalogFromIMAGE();
    void taskInternalHDUreformat();
    void taskInternalResolveTargetSidereal();
    void taskInternalRestoreHeader();
    //    void taskInternalCopyZeroOrder();
    void taskInternalAstromphotom();
    void taskInternalSeparate();

//    void processExternalStdout();
//    void processExternalStderr();
    void finishedPreparationReceived();
    void waitForResamplingThreads(int threadID);
    void finishedScampReceived();
    void fieldMatchedReceived();
    void showScampCheckPlotsReceived();
    void registerScampSolutionAcceptance(bool scampSolutionAccepted);
    void continueWithCopyZeroOrder();
    void copyZeroOrder();
    void swarpErrorFoundReceived();
    void scampErrorFoundReceived();
    void addToProgressBarReceived(const float differential);
//    void splitterMemoryReceived(long memoryUsed);

public:
    explicit Controller(const instrumentDataType *instrumentData, QString statusold, ConfDockWidget *cdw, Monitor *processMonitor,
                        MainWindow *parent = nullptr);
    ~Controller();

    // Multi-threading (accessed from e.g. memory viewer)
    omp_lock_t lock;
    omp_lock_t memoryLock;
    omp_lock_t wcsLock;
    omp_lock_t genericLock;
    omp_lock_t progressLock;
    omp_lock_t backgroundLock;

    // QTimer *ramTimer;
    // QTimer *cpuTimer;
    QTimer *progressTimer;
    // QTimer *driveTimer;

    int maxCPU = 1;                    // Overall, maximum number of CPUs to be used
    int localMaxCPU = 1;               // depending on how the parallelization is made, and how many images we have, we may run with fewer threads
    int maxExternalThreads = 1;        // Maximum number of threads working on chips; equal or smaller than maxCPU
    int maxInternalThreads = 1;        // Maximum number of threads working on images; equal or smaller than maxCPU - maxExternalThreads
    int maxThreadsIO = 1;              // Maximum number of IO threads
    long maxRAM = 512;                 // Max RAM in MB available for processing
    int currentExternalThreads = 0;    // The current number of external running threads (over different chips)
    int currentInternalThreads = 0;    // The current number of internal running threads (over images of the same chip)
    int currentThreadsRunning = 0;
    int availableThreads = 1;
    bool useGPU = false;
    int verbosity = 0;
    bool userYield = false;
    bool userStop = false;
    bool userKill = false;
    bool abortProcess = false;     // Triggered by critical() signals emitted anywhere
    bool swapWarningShown = false;
    bool dataTreeUpdateOngoing = false;

    QString currentSwarpProcess = "";
    QString mainDirName;
    QString instrument_dir;

    Query *gaiaQuery;
    bool gaiaQueryDone = false;

    bool alwaysStoreData = false;
    bool minimizeMemoryUsage = false;

    bool homeDirWarningShowed = false;

    float progress = 0.;
    long numActiveImages = 0;
    float progressStepSize = 0.;
    float progressHalfStepSize = 0.;
    float progressCombinedStepSize = 0.;

    QString taskBasename;
    QString instructions;
    QString backupDirName = "";
    QString statusOld = "";
    QString statusNew = "";

    QString lastTaskExecuted = "";  // needed to detect whether a task is repeated or not (must restore memory accordingly)
    bool isTaskRepeated = false;

    // DT = "DataTree"
    // The lists contain multiple entries, e.g. if three different BIAS dirs are defined
    QList<Data*> DT_BIAS;
    QList<Data*> DT_DARK;
    QList<Data*> DT_FLAT;
    QList<Data*> DT_FLATOFF;
    QList<Data*> DT_SCIENCE;
    QList<Data*> DT_SKY;
    QList<Data*> DT_STANDARD;

    QList<QList<Data*>> masterListDT;
    Data* GLOBALWEIGHTS = nullptr;
    Mask *mask = nullptr;
    Mask *altMask = nullptr;         // for GROND types with different simultaneous detectors
    MainWindow *mainGUI;
    ConfDockWidget *cdw;
    const instrumentDataType *instData;
    Monitor *monitor;
    MemoryViewer *memoryViewer;
    bool successProcessing = true;   // Reset externally to 'true' when clicking the 'start' button

    void mapDataTree();
    void taskProcessbias();
    void runTask();
    Data* getData(QList<Data*> DT_x, QString dirName);
    Data* getDataAll(QString dirName);

    QString manualCoordUpdateMode = "";

    QVector<QThread*> workerThreads;
    QVector<SwarpWorker*> swarpWorkers;
    QVector<bool> threadsFinished;
    bool workersInit = false;
    bool workerThreadsInit = false;

    QThread *workerThreadPrepare = nullptr;
    QThread *workerThreadCoadd = nullptr;
    QThread *workerThread = nullptr;
    SwarpWorker *swarpWorker = nullptr;
    ScampWorker *scampWorker = nullptr;

    bool workerInit = false;
    bool workerThreadInit = false;

    Data *coaddScienceData;
    Data *scampScienceData;
    Data *currentData = nullptr;
    QString currentDirName = "";

    QVector<QProcess*> externalProcesses;
    QVector<QString> swarpCommands;
    QProcess *externalProcess;
    QByteArray *stdoutByteArray;
    QByteArray *stderrByteArray;

    // Function pointers;
    // Updated externally by MainWindow::updateControllerFunctors()
    float (*combineOverscan_ptr) (const QVector<float> &, const QVector<bool> &, long);
    float (*combineBias_ptr) (const QVector<float> &, const QVector<bool> &, long);
    float (*combineDark_ptr) (const QVector<float> &, const QVector<bool> &, long);
    float (*combineFlatoff_ptr) (const QVector<float> &, const QVector<bool> &, long);
    float (*combineFlat_ptr) (const QVector<float> &, const QVector<bool> &, long);
    float (*combineBackground_ptr) (const QVector<float> &, const QVector<bool> &, long);

    void checkForUnsavedImages(long &numUnsavedLatest, long &numUnsavedAll);
    void wipeDataTree();
    void writeUnsavedImagesToDrive(bool includeBackup);
//    QStringList getFilterList(QString scienceDir);
    void downloadGaiaCatalog(Data *scienceData);
    void downloadGaiaCatalog(Data *scienceData, QString radius);
    void releaseAllMemory();
    void restoreAllRawData();
    void updateMasterList();
    void connectDataWithMemoryViewer();
    QLineEdit *getDataTreeLineEdit(Data *data);
    void getFieldCenter(Data *data, QString &alphaCenter, QString &deltaCenter);
    bool isMainDirHome();
    void populateInstrumentDictionary();

    bool isImageTooBig(QString name);
signals:
    void loadViewer(QString dirname, QString filter, QString mode);
    void messageAvailable(QString message, QString type);
    void appendOK();
    void showMessageBox(QString trigger, QString part1, QString part2);
    void progressUpdate(float progress);
    void targetResolved(QString alpha, QString delta);
    void resetProgressBar();
    void swarpStartResampling();
    void swarpStartCoaddition();
    void swarpStartSwarpfilter();
    void swarpStartUpdate();
    void showScampCheckPlots();
    void scienceDataDirUpdated(QString allDir);
    void clearMemoryView();
    void populateMemoryView();
    void stopFileProgressTimer();
    void addBackupDirToMemoryviewer(QString scienceDir);
    void loadAbsZP(QString coaddImagePath, float maxVal);
    void updateMemoryProgressBar(long splitterMemory);
    void forceFinish();
    void refreshMemoryViewer();

public slots:
    void activationWarningReceived(QString imagestatus, QString drivestatus);
    void updateAll();
    //    void updateSingle();
    void loadPreferences();
    void updateMemoryPreference(bool isRAMminimized);
    void updateIntermediateDataPreference(QString intermediateDataPreference);
    void criticalReceived();
    void warningReceived();
    void messageAvailableReceived(QString message, QString type);
    void appendOKReceived();
    void showMessageBoxReceived(QString trigger, QString part1, QString part2);
    void updateVerbosity(int verbosityLevel);
//    void progressUpdateReceived(float progress);
    void dataTreeEditedReceived();
    void newProjectLoadedReceived();
    void coaddResample();
    void coaddSwarpfilter();
    void coaddCoaddition();
    void coaddUpdate();
    void rereadScienceDataDirReceived();
    void setMemoryLockReceived(bool locked);
    void setWCSLockReceived(bool locked);
    void absZeroPointCloseReceived();
    void resetErrorStatusReceived(QString dirName);
};


#endif // CONTROLLER_H
