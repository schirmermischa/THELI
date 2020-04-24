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

#ifndef MYIMAGE_H
#define MYIMAGE_H

#include "../myfits/myfits.h"
#include "../tools/detectedobject.h"
#include "../threading/sexworker.h"
#include "../threading/anetworker.h"
#include "../processingStatus/processingStatus.h"

#include <gsl/gsl_vector.h>

#include <omp.h>
#include <wcs.h>

#include <QObject>

// This class keeps track of an individual image, its memory/disk state,
// processing status, previous processing states, file name, FITS file handles,
// associated weight and mask images, geometry, filter, exposure time, seeing
// and RZP
class MyImage : public QObject
{
    Q_OBJECT

private:

    void transferDataToMyImage();
    void transferMetadataToMyImage();
    void stayWithinBounds(QVector<long> &vertices);
    float polynomialSum(float x, QVector<float> coefficients);

    // ================= BACKGROUND MODELING ===========================
    long nGridPoints = 0;           // Number of grid points
    int gridStep = 0;               // Linear distance between grid points
    int filterSize = 0;             // spatial scale on which the image should be filtered
    //   int subSample = 3;         // SPLINTER The factor by which the grid should be subSampled
    int subSample = 1;              // GSL The factor by which the grid should be subSampled; at least twice the kernel width
    int n_grid = 0;                 // number of grid points along naxis1
    int m_grid = 0;                 // number of grid points along naxis2
    int pad_l = 0;                  // number of pad pixels left edge;     same as padWidth w
    int pad_b = 0;                  // number of pad pixels bottom edge;   same as padWidth w
    int pad_r = 0;                  // number of pad pixels right edge;    same as padWidth w unless +1 to make even dimension
    int pad_t = 0;                  // number of pad pixels top edge;      same as padWidth w unless +1 to make even dimension
    long n_pad = 0;                 // NAXIS1 of the padded image
    long m_pad = 0;                 // NAXIS2 of the padded image
    QString splineMode = "smooth";  // "smooth" or "interpolate"
    QString padMode = "normal";     // Whether the image is padded "normal" or "dyadic"
    QVector<float> backStatsGrid;   // the statistics for the grid (background value for each grid point)
    QVector<float> rmsStatsGrid;    // the statistics for the grid (rms value for each grid point)
    //    QVector<float> data_padded;     // padded image data
    //    QVector<bool> mask_padded;      // padded mask data
    QVector<QVector<long>> grid;    // the grid on which we calculate statistics
    QVector<long> padDims;          // contains left, bottom, right, top pad dimensions, and then overall n_pad and m_pad

    void planGrid();
    void getGridStatistics();
    void filterGridStatistics();
    //    void fitBackgroundSPLINTER();
    void fitBackgroundGSL();
    void getPadDimensions();
    void padImage();
    void padCorner(int width, QString corner, long ipadmin, long ipadmax, long jpadmin, long jpadmax);
    void padEdgeLR(int width, QString edge, long ipadmin, long ipadmax, long jpadmin, long jpadmax);
    void padEdgeBT(int width, QString edge, long ipadmin, long ipadmax, long jpadmin, long jpadmax);
    int padCorr(int i, int w, int max);
    void replaceBlankGridStatistics();
    void clearBackgroundMemory();
    // void backgroundModel(int filtersize, QString splinemode);    public function
    // =================================================================

    // ================== Image segmentation ===========================
    void thresholdImage();
    bool insideImage(QPoint p);
    void floodFill(const QPoint startPoint, QList<long> &objectPixelIndices, long &objID,
                   QVector<long> &dataSegmentation, const long DMIN);
    QVector<float> directConvolve(QVector<float> &data);
    void writeSegmentation(QString fileName);
    void writeObjectMask(QString fileName);

    // =================================================================

    void transferWCS();
    QVector<long> locateSkyNode(const double alpha, const double delta, const double radius);

    void printCfitsioError(QString funcName, int status);

    void median2D(const QVector<float> &data_in, QVector<float> &data_out, int filtersize);
    void pushDownToL3();
    void pushDownToL2();
    void pushDownToL1(QString backupDir);
    void pullUpFromL1();
    void pullUpFromL2();
    void pullUpFromL3();
    void wipeL1();
    void wipeL2();
    void wipeL3();
    void readImageBackupL1();
    void replaceCardInFullHeaderString(QString keyname, double value);
public:
    explicit MyImage(QString pathname, QString filename, QString statusString, int chipnumber,
                     const QVector<bool> &mask, int *verbose, bool makebackup = true,
                     QObject *parent = nullptr);
    explicit MyImage(QString fullPathName, const QVector<bool> &mask, int *verbose, QObject *parent = nullptr);
    explicit MyImage(QObject *parent = nullptr);

    // A series of flags telling the status of a MyImage
    enum active_type {
        ACTIVE,           // The image is active (will be used in coaddition and all processing steps)
        INACTIVE,         // Image is inactive (for whatever reason, parked in inactive/)
        BADSTATS,         // Image was flagged as having bad seeing/ throughput etc (parked in inactive/badStatistics/)
        BADBACK,          // Image was flagged because the background could not be modeled (parked in inactive/badBackground/)
        LOWDETECTION,     // Image was flagged because it has insufficiently many source detections for astrometry
        DELETED,          // Image was manually deleted by the user (cannot be found anymore on disk)
    };

    active_type activeState = ACTIVE;
    active_type oldState = activeState;   // needed by memoryViewer;

    ~MyImage();
    struct wcsprm *wcs;
    bool wcsInit = false;
    char *fullheader = nullptr;

    ProcessingStatus *processingStatus;

    int maxCPU = 1;       // In case we work on a single image, e.g. for abszeropoint, we can speed up calculations

    QList<QVector<double>> skyPolyfitNodes;

    QVector<float> apertures;

    QThread *workerThread;
    SexWorker *sexWorker;
    AnetWorker *anetWorker;

    // Defining data
    QString path = "";
    QString pathBackupL1 = "";
    QString pathBackupL2 = "";
    QString pathBackupL3 = "";
    QString pathExtension = "";
    QString weightPath = path+"../WEIGHTS/";
    QString weightName = "";
    QString name = "";
    QString baseName = "";   // "image_1AB" in case of image_1AB.fits
    QString rootName = "";   // "image" in case of image_1AB.fits
    QString chipName = "";   // "image_1" in case of image_1AB.fits
    QString filter = "";
//    QString statusCurrent = "";
    QString statusBackupL1 = "";
    QString statusBackupL2 = "";
    QString statusBackupL3 = "";
    QString baseNameBackupL1 = "";
    QString baseNameBackupL2 = "";
    QString baseNameBackupL3 = "";
    MyFITS *imageFITS = nullptr;
    MyFITS *weightFITS = nullptr;
    bool makeBackup = true;  // used when reading an image for the first time in the pipeline context
    bool metadataTransferred = false;
    int groupNumber = -1; // The association of images this image belongs to (an overlapping group on sky)

    bool minimizeMemoryUsage = false;
    bool headerInfoProvided = false;
    bool processingFinished = false;          // Reset to false every time a process starts; If finised, external jobs use this flag to force a
                                              // dump to drive to be able to release memory
    bool isTaskRepeated = false;

    QList<DetectedObject*> objectList;

    QVector<QString> header;

    QString sexCommand;
    QString anetCommand;

    // Static image characteristics
    // Once running, the chipNumber always starts with 1,
    // contrary to the 'chip' variable which starts at 0
    int chipNumber = 0;
    int naxis1 = 0;
    int naxis2 = 0;

    float matchingTolerance = 2./3600.;   // default matching tolerance = 2 arcsec;

    long dim;
    // Status flags
    bool imageInMemory = false;
    bool backupL1InMemory = false;
    bool backupL2InMemory = false;
    bool backupL3InMemory = false;
    bool backupL1OnDrive = false;
    bool backupL2OnDrive = false;
    bool backupL3OnDrive = false;
    bool imageOnDrive = false;
    bool weightInMemory = false;
    bool weightOnDrive = false;
    bool headerRead = false;
    bool modeDetermined = false;
    bool hasMJDread = false;
    bool validFile = true;         // Is the file valid
    bool validMode = true;         // Is the statistical mode within accepted range
    bool validBackground = true;   // Is the image accepted to contribute to a background model (no if e.g. bright star)
    bool validDetector = true;     // Is the detector alive
    // Data characteristics
    QString dateobs = "";
    double crval1 = 0.;
    double crval2 = 0.;
    double mjdobs = 0;
    float fwhm = -1.0;             // FWHM estimate based on GAIA point sources
    float fwhm_est = -1.0;         // median FWHM of all detected sources in an image
    float RZP = 0;                 // SCAMP output
    float exptime = 0;
    float airmass = 1.0;
    float ellipticity = -1.0;      // estimate based on GAIA point sources
    float ellipticity_est = -1.0;  // median ellipticity based on all detected sources
    float FLXSCALE = 0.;           // SCAMP output
    float skyValue = -1e9;         // previously set to zero
    float skySigma = -1;
    float gain = 1.0;              // Read from raw fits header; fallback: taken from instrument config file
    float plateScale = 0.;
    float gainNormalization = 1.0;
    int bitpix = -32;
    // Processing stages
    QVector<float> dataCurrent;    // Contains the current state of processing. Processing is done on it, will then be pushed down into first backup level
    QVector<float> dataBackupL1;   // First backup level
    QVector<float> dataBackupL2;   // Second backup level
    QVector<float> dataBackupL3;   // Third backup level
    QVector<float> dataMeasure;    // temporary (for object detection)
    const QVector<bool> &globalMask;      // Global mask (e.g. vignetting, permanently bad pixels; same for all images)
//    QVector<bool> globalMask;      // Global mask (e.g. vignetting, permanently bad pixels; same for all images)
    QVector<bool> objectMask;      // Object mask (used for background modeling and sky subtraction)
    bool backgroundPushedDown = false;  // Used to detect whether a backup copy was made already during twopass background correction
    bool globalMaskAvailable = true;    // Unless we load an external image, e.g. for absolute zeropoint

    // Memory stages (to decide what can be deleted and what not)
    bool dataWeight_deletable = false;
    bool dataCurrent_deletable = false;
    bool dataBackupL1_deletable = false;
    bool dataBackupL2_deletable = true;  // should always be true (apart from when we populate it)
    bool dataBackupL3_deletable = true;  // should always be true (apart from when we populate it)
    bool dataBackground_deletable = false;

    bool successProcessing = true;   // A flag that is updated everytime we do something to the image
    int backgroundBlock = 0;

    QString procInfo = "";

    float meanExposureBackground = 0.;

    // Flags to decide whether the image is used in a current background calculation or not
    bool useForBackground = false;              // Overall flag
    bool useForBackgroundSequence = false;      // Whether the image can be used from a 'LIRIS-style' sequence splitting
    bool useForBackgroundWindowed = false;      // Whether the image can be used because it falls within the current group, statically or dynamically
    bool useForBackgroundStars = false;         // Whether the image can be used because it does not contain bright sources
    bool hasBrightStars = false;

    int *verbosity;
    omp_lock_t backgroundLock;
    omp_lock_t objectLock;

    // The image corners in RA/DEC (e.g. to check whether a bright star is inside or outside)
    double alpha_ul = 0.;
    double alpha_ur = 0.;
    double alpha_ll = 0.;
    double alpha_lr = 0.;
    double delta_ul = 0.;
    double delta_ur = 0.;
    double delta_ll = 0.;
    double delta_lr = 0.;
    double alpha_ctr = 0.;   // Image center coords
    double delta_ctr = 0.;

    // Associated data
    QVector<float> dataWeight;
    QVector<float> dataWeightSmooth;
    //    QVector<uint> mask;
    float maskValue = 0.;
    float saturationValue = 1.e9;

    bool allocatedImageFITS = false;

    // =============== Astrometric solution ===================
    QVector<float> dataXcorr;

    // ================= BACKGROUND MODELING ===========================
    QVector<float> dataBackground;  // The background model
    void backgroundModel(int filtersize, QString splinemode);
    void releaseBackgroundMemory(QString mode = "");
    void releaseBackgroundMemoryBackgroundModel();
    bool backupCopyBackgroundMade = false;
    bool hasBrightStarsChecked = false;
    // =================================================================

    // ================== Image segmentation ===========================
    QVector<long> dataSegmentation;
    void segmentImage(QString DTstring, QString DMINstring, bool convolution, bool writeSegImage = false);
    void maskExpand(QString expFactor, bool writeObjectmaskImage = false);
    void transferObjectsToMask();
    void releaseAllDetectionMemory();
    void releaseDetectionPixelMemory();
    bool backgroundModelDone = false;
    bool segmentationDone = false;
    bool maskExpansionDone = false;
    bool objectMaskDone = false;
    bool objectMaskDonePass1 = false;
    bool objectMaskDonePass2 = false;
    bool leftBackgroundWindow = false;      // used to mark the memory for deletion
    bool enteredBackgroundWindow = false;
    // =================================================================

    void add(float value);
    void appendToScampCatalogInternal(fitsfile *fptr, QString minFWHM_string, QString maxFlag_string);
    void appendToScampCatalogSExtractor(fitsfile *fptr);
    void applyBackgroundModel(MyImage *backgroundImage, QString mode, bool rescaleFlag);
    void applyMask();
    void applyPolygons(int chip);
    void backupOrigHeader(int chip);
    void buildSexCommand();
    void buildAnetCommand();
    void cornersToRaDec();
    void createSextractorCatalog();
    void collapseCorrection(QString threshold, QString direction);
    void cosmicsFilter(QString aggressiveness);
    void checkBrightStars(QList<QVector<double>> &brightStarList, float safetyDistance, float plateScale);
    void divide(float value);
    void divideFlat(MyImage *flatImage);
    void emitModelUpdateNeeded();
    void estimateMatchingTolerance();
    void evaluateSkyNodes(const QVector<double> alpha, const QVector<double> delta, const QVector<double> radius);
    QVector<double> extractCDmatrix();
    QVector<float> extractPixelValues(long xmin, long xmax, long ymin, long ymax);
    void filterSextractorCatalog(QString minFWHM, QString maxFlag);
    void freeAll();
    void freeData(QVector<float> &data);
    void freeAncillaryData(QVector<float> &data);
    void freeData();
    float freeData(QString type);
    void protectMemory();
    void freeWeight();
    QString getKeyword(QString key);
    void getMode(bool determineMode = true);
    void illuminationCorrection(int chip, QString thelidir, QString instName, QString filter);
    void initWeightfromGlobalWeight(const QList<MyImage *> &gwList);
    void laplaceFilter(QVector<float> &dataFiltered);
    void makeCutout(long xmin, long xmax, long ymin, long ymax);
    void makeDriveBackup(QString backupDirName, QString statusOld);
    void makeMemoryBackup();
    void makeXcorrData();
    void maskBloomingSpike(QString instType, QString range, QString minVal, bool requested, QString bloomMin = "", QString bloomMax = "");
    void mergeObjectWithGlobalMask();
    void multiply(float value);
    void normalizeFlat();
    void provideHeaderInfo();
    void readImage(bool determineMode = true);
    void readWeight();
    void resetObjectMasking();
    QVector<float> retainUnmaskedData(int sampleDensity = 1);
    QVector<float> retainUnmaskedDataThresholded(float minVal, float maxVal, int sampleDensity = 1);
    void roundEdgeOfWeight(float edge, bool roundEdge);
    bool scanAstromHeader(int chip, QString mode);
    void setModeFlag(QString min, QString max);
    void setupDataInMemorySimple(bool determineMode);
    void setupData(bool isTaskRepeated, bool createBackup, bool determineMode, QString backupDir = "");
    void setupBackgroundData(const bool &isTaskRepeated, const QString &backupDir);
    void setupBackgroundData_newParallel(const bool &isTaskRepeated, const QString &backupDir);
    void setupCoaddMode();
    void setupCalibDataInMemory(bool createBackup, bool determineMode, bool mustRereadFromDisk);
    void setActiveState(active_type state);
    void sexcatToIview();
    void showProcInfo();
    void sky2xy(const double alpha, const double delta, double &x, double &y);
    void subtract(float value);
    void subtractBias(MyImage *biasImage, QString dataType);
    void subtractSkyFit(int order, gsl_vector *c, bool saveSkyModel);
    void subtractPolynomialSkyFit(gsl_vector *c, int order);
    void subtractBackgroundModel();
    void transferDataToMyWeight();
    void toTIFF(int bit, float minthresh, float maxthresh, bool zscaleing = false, float grey = 0., float gamma = 1.0);
    void thresholdWeight(QString imageMin, QString imageMax);
    void unprotectMemory();
    void updateKeywordInHeader(QString key, QString value);
    void updateMode();
    void updateCRVALinHeaderOnDrive();
    void updateCRVALCDinHeaderOnDrive();
    void updateInactivePath();
    void updateProcInfo(QString text);
    void updateSaturation(QString saturation);
    void updateZeroOrderOnDrive(QString updateMode);
//    void updateZeroOrderInMemory();
    // Not sure I need the optional argument feature for the following two
    void writeImage(QString fileName = "", QString filter = "", float exptime = -1.0, bool addGain = false);
    void writeConstSkyImage(float constValue, QString filter = "", float exptime = -1.0, bool addGain = false);
    void writeImageDebayer(bool addGain = false);
    void writeWeight(QString fileName = "");
    void writeCatalog(QString minFWHM_string, QString maxFlag_string);
    void writeWeightSmoothed(QString fileName);
    void xy2sky(const double x, const double y, double &alpha, double &delta);
    void xtalk(int method, float scale, QString direction = "", int nsection = -1);
    void updateHeaderValue(QString keyName, float keyValue);
    void updateHeaderValue(QString keyName, QString keyValue);
    void updateHeaderValueInFITS(QString keyName, QString keyValue);
    void calcMedianSeeingEllipticity();
    QVector<double> collectObjectParameter(QString paramName);
    void collectSeeingParameters(QVector<QVector<double> > &outputParams, QVector<double> &outputMag, int goodChip);
    void setDeletable();
//    void setDeletable(QString dataX, bool deletable);
    void createMyFITSinstance();
    void pushNameToFITS();
    void writeBackgroundModel();
    void getMeanBackground();
    void pushDown(QString backupDir);
    void pullUp();
    void createSextractorCatalog_old();
    bool makeL1Current();
    bool makeL2Current();
    bool makeL3Current();
    void makeBackgroundBackup();
    void checkTaskRepeatStatus(QString taskBasename);
    void readImageBackupL1Launch();
    void setObjectLock(bool locked);
    void setBackgroundLock(bool locked);
//    void updateCRPIXOnDrive();
    void removeSourceCatalogs();
    void dumpToDriveIfPossible();
    bool containsRaDec(QString alphaStr, QString deltaStr);
    double getPlateScale();
    void calcMedianSeeingEllipticitySex();
    void updateCRVALinHeader();
    void updateCRVALCDinHeader();
    void unprotectMemoryWeight();
    void checkWCSsanity();
    void buildAnetCommand(QString pixscale_maxerr);
    void runAnetCommand();
    QString extractAnetOutput();
    void sexcatToAnet();
    void writeImageBackupL1();
    void writeImageBackupL2();
    void writeImageBackupL3();
signals:
    void modelUpdateNeeded(QString baseName, QString chipName);
    void messageAvailable(QString message, QString type);
    void setMemoryLock(bool locked);
    void setWCSLock(bool locked);
    void critical();
    void warning();
    void errorOccurred();

public slots:
    void messageAvailableReceived(QString message, QString type);
    void anetOutputReceived(QString message, QString type);

private slots:
    void errorFoundReceived();
};

#endif // MYIMAGE_H
