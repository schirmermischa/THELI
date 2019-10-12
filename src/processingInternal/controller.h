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
    QString tmpdir;
    QString instrument_dir;

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

    CPU *myCPU;

    QString swarpCommand;
    QString coaddDirName;
    QString coaddScienceDir;
    QString scampScienceDir;
    QString scampHeadersDir;
    QString scampPlotsDir;
    QString coaddUniqueID;
    QString scampCommand;
    QString sexCommandOptions;
    QString scampDir;
    int processID;

    QVector<PhotInst*> photInstruments;

    QVector<QVector<long>> overscanX;
    QVector<QVector<long>> overscanY;
    QVector<QVector<long>> dataSection;

    bool globalweights_created = false;

    QString statusAtDistribute = "";

    QMap<QString,QStringList> headerDictionary;
    QMap<QString,QString> filterDictionary;

    int recurseCounter = 0;

    // Processing functions
    void checkSuccessProcessing(Data *data);
    void decrementCurrentThreads(omp_lock_t &lock);
    QVector<QString> getBackgroundThresholds(const int loop, const bool twoPass, const QString DT, const QString DMIN, bool &doSourceDetection);
    void getDetectorSections();
    QList<QVector<float> > getNonlinearityCoefficients();
    void stayWithinBounds();
    void incrementCurrentThreads(omp_lock_t &lock);
    void memoryDecideDeletableStatus(Data *data, bool deletable);
    void parseDataDir(QLineEdit *le, QList<Data *> &DT_x);
    void pushParallelizationToData(QList<Data *> DT_x);
    void pushParallelizationToData(Data *data);

    //    void rejectChipsWithBrightStars(QList<MyImage *> backgroundList, QList<QVector<double> > &brightStarList);
    int reserveAvailableThreads(omp_lock_t &lock);
    void retrieveBrightStars(Data *data, QList<QVector<double>> &brightStarList);
    void makeThreadsAvailable(omp_lock_t &lock, int numberThreadsBlocked);
    void selectImagesDynamically(QList<MyImage *> backgroundList, const double &mjd_ref);
    void selectImagesStatically(QList<MyImage *> backgroundList, const double &mjd_ref);
    void selectImagesFromSequence(QList<MyImage *> backgroundList, const int &nGroups, const int &nLength, const int &currentExp);

    // Data handling
    void restoreData(MyImage *myImage, QString backupDir);

    // Sky subtraction
    void skysubModel(Data *scienceData, QString DT, QString DMIN, QString expFactor, QString kernelWidth);
    void skysubPolynomial();
    void skysubPolynomialFit(Data *scienceData);
    QList<QVector<double> > getBlankSkyPositions(QString path);
    QList<MyImage*> measureSkyInBlankRegions(Data *scienceData, QString method = "");
    void skysubConstantFromArea(Data *scienceData);
    void skysubConstantReferenceChip(Data *scienceData, QString DT, QString DMIN, QString expFactor, QString kernelWidth);
    void skysubConstantEachChip(Data *scienceData, QString DT, QString DMIN, QString expFactor, QString kernelWidth);

    // Coaddition
    void coaddPrepare(QString filterArg);
    void coaddSmoothEdge();
    void coaddPrepareProjectPM(QFile &headerFileOld, QString newHeaderName, QString refRA, QString refDE, double mjdobsZero, double mjdobsNow);
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

    // Astrometry
    bool manualCoordsUpdate(Data *scienceData, QString mode);
    QString checkManualCoordsUpdate(QString scienceDir);
    void prepareScampRun(Data *scienceData);
    long prepareScampCats(Data *scienceData, long &totNumObjects);
    QString getUserParamLineEdit(QLineEdit *le);
    QString getUserParamCheckBox(QCheckBox *cb);
    QString getUserParamComboBox(QComboBox *cb);
    void buildScampCommand(Data *scienceData);
    void sendMemoryPreferenceToImages(QList<Data*> DT_x);
    int countBackgroundImages(QList<MyImage *> list, QString baseName);
    //    int getInternalThreads(int chip);

    void populateCommentMap();
    void pushBeginMessage(QString idstring, QString targetdir);
    void pushConfigCoadd();
    void pushConfigAstromphotom();
    void pushConfigGetCatalogFromWeb();
    void pushConfigGetCatalogFromImage();
    void pushConfigCreatesourcecat();
    void pushEndMessage(QString idstring, QString targetdir);
    bool idChipsWithBrightStars(Data *skyData, QList<QVector<double> > &brightStarList);
    void flagImagesWithBrightStars(QList<MyImage *> &backgroundList);
    void getNumberOfActiveImages(Data *&data);
    void incrementProgress();
    void incrementProgressHalfStep();
    void incrementProgressCombinedStep();
    void rereadDataDir(QLineEdit *le, QList<Data *> &DT_x);
    void doCrossCorrelation(Data *scienceData);
    long coaddCoadditionGetSize();
    void splitScampHeaders();
    void scampCalcFluxscale();
    int getMaxPhotInst();
    void provideHeaderInfo(Data *scienceData);
    void buildSexCommandOptions();
    void detectionInternal(Data *scienceData, QString minFWHM, QString maxFlag);
    void detectionSExtractor(Data *scienceData, QString minFWHM, QString maxFlag);
    void mergeInternal(Data *scienceData, QString minFWHM, QString maxFlag);
    void mergeSExtractor(Data *scienceData);
    void populateHeaderDictionary();
    void populateFilterDictionary();
    long makeListofAllImages(QList<MyImage *> &allMyImages, Data *data);

    //    void updateMyImagesWithScampSolution(Data *scienceData);
    void doImageQualityAnalysis();
    void collectGaiaRaDec(QVector<double> &dec, QVector<double> &ra, QVector<QVector<double> > &output);
    void releaseMemory(float RAMneeded, int numThreads, QString mode = "");
    long getNumObjectsScampCat(QString cat);
    void satisfyMaxMemorySetting();
    long getNumObjectsSexCat(QString cat);
    void emitSourceCountMessage(long &nobj, QString baseName);
    void printCfitsioError(QString funcName, int status);
    void updateImageAndData(MyImage *image, Data *data);
    Data *setupDataDir();
    void maskObjectsInSkyImagesPass1(Data *skyData, Data *scienceData, const QList<MyImage *> &backgroundList,
                                     const bool twoPass, const QString dt, const QString dmin, const bool convolution,
                                     const QString expFactor);
    void maskObjectsInSkyImagesPass2(Data *skyData, Data *scienceData, const QList<MyImage*> &backgroundList,
                                     const bool twoPass, const QString dt, const QString dmin, const bool convolution,
                                     const QString expFactor, const int chip, const bool rescaleModel);
    bool filterBackgroundList(const int chip, const Data *skyData, const MyImage *it, QString &backExpList, QList<MyImage*> &backgroundList,
                              const int nGroups, const int nLength, const int currentExposure, const QString mode);
    void maskObjectsInSkyImagesPass1_newParallel(Data *skyData, Data *scienceData, const QList<MyImage *> &backgroundList,
                                                 const bool twoPass, const QString dt, const QString dmin, const bool convolution,
                                                 const QString expFactor, const int threadID);
    void maskObjectsInSkyImagesPass2_newParallel(Data *skyData, Data *scienceData, MyImage *combinedImage, const QList<MyImage *> &backgroundList,
                                                 const bool twoPass, const QString dt, const QString dmin, const bool convolution,
                                                 const QString expFactor, const int chip, const bool rescaleModel,
                                                 const int threadID, const QString mode);
    void sendBackgroundMessage(const int &chip, const QString &mode, const Data *skyData, const QString &basename, const int pass);
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
    void processBackground(Data *scienceData, Data *skyData, const float nimg, QVector<QString> &numBackExpList,
                           const QString dt, const QString dmin, const QString expFactor, const QString nlow1,
                           const QString nhigh1, const QString nlow2, const QString nhigh2,
                           const bool twoPass, const bool convolution, const bool rescaleModel,
                           const int nGroups, const int nLength, QString mode, QVector<bool> &staticImagesWritten);

private slots:
    // The following can also be under 'private', but then the declaration must be preceeded like this:
    // Q_INVOKABLE QString taskHDUreformat();

    void displayMemoryTotalUsed();
    void displayCPUload();
    void displayProgress();
    void displayDriveSpace();
    float getMemoryTotalUsed();

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
    void taskInternalResolveTarget();
    void taskInternalRestoreHeader();
    //    void taskInternalCopyZeroOrder();
    void taskInternalAstromphotom();
    void taskInternalSeparate();

    void processExternalStdout();
    void processExternalStderr();
    void finishedPreparationReceived();
    void waitForResamplingThreads(int threadID);
    void finishedScampReceived();
    void fieldMatchedReceived();
    void showScampCheckPlotsReceived();
    void checkScampSolutionAcceptance(bool scampSolutionAccepted);
    void copyZeroOrder();
    void errorFoundReceived();
    void addToProgressBarReceived(float differential);
    void absZeroPointCloseReceived();

public:
    explicit Controller(instrumentDataType *instrumentData, QString statusold, ConfDockWidget *cdw, Monitor *processMonitor,
                        MainWindow *parent = nullptr);
    ~Controller();

    // Multi-threading (accessed from e.g. memory viewer)
    omp_lock_t lock;
    omp_lock_t memoryLock;
    omp_lock_t wcsLock;
    omp_lock_t genericLock;
    omp_lock_t progressLock;
    omp_lock_t backgroundLock;

    QTimer *memTimer;
    QTimer *cpuTimer;
    QTimer *progressTimer;
    QTimer *driveTimer;

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

    QString mainDirName;

    Query *gaiaQuery;

    bool alwaysStoreData = false;
    bool minimizeMemoryUsage = false;

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

    instrumentDataType *instData;
    Mask *mask = nullptr;
    MainWindow *mainGUI;
    ConfDockWidget *cdw;
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

    QThread *workerThreadPrepare;
    QThread *workerThreadCoadd;
    QThread *workerThread;
    SwarpWorker *swarpWorker;
    ScampWorker *scampWorker;
    bool scampSolutionAccepted = false;

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

    long checkForUnsavedImages();
    void wipeDataTree();
    void writeUnsavedImagesToDrive();
    //    void coaddPrepareStep2();
    QStringList getFilterList(QString scienceDir);
    void downloadGaiaCatalog(Data *scienceData);
    void releaseAllMemory();
    void restoreAllRawData();
    void updateMasterList();
    void connectDataWithMemoryViewer();

signals:
    void loadViewer(QString dirname, QString filter, QString mode);
    void messageAvailable(QString message, QString type);
    void appendOK();
    void showMessageBox(QString trigger, QString part1, QString part2);
    void progressUpdate(float progress);
    void targetResolved(QString alpha, QString delta);
    void resetProgressBar();
    void endThread();
    void swarpStartResampling();
    void swarpStartCoaddition();
    void swarpStartSwarpfilter();
    void swarpStartUpdate();
    void swarpGetAbsZP();
    void showScampCheckPlots();
    void scienceDataDirUpdated(QString allDir);
    void clearMemoryView();
    void populateMemoryView();
    void stopFileProgressTimer();
    void addBackupDirToMemoryviewer(QString scienceDir, QString backupDirName);

public slots:
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
    void progressUpdateReceived(float progress);
    void dataTreeEditedReceived();
    void newProjectLoadedReceived();
    void coaddResample();
    void coaddSwarpfilter();
    void coaddCoaddition();
    void coaddUpdate();
    void rereadScienceDataDirReceived();
    void setMemoryLockReceived(bool locked);
    void setWCSLockReceived(bool locked);
};

#endif // CONTROLLER_H