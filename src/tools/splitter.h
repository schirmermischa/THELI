#ifndef SPLITTER_H
#define SPLITTER_H

#include "fitsio.h"
#include "libraw/libraw.h"

#include "../instrumentdata.h"
#include "../processingInternal/mask.h"
#include "../processingInternal/data.h"
#include "../dockwidgets/confdockwidget.h"

#include <QObject>
#include <QMap>

class Splitter : public QObject
{
    Q_OBJECT
public:
    explicit Splitter(instrumentDataType &instrumentData, Mask *detectorMask, Data *someData, QString datatype,
                      ConfDockWidget *confDockWidget, QString maindirname,
                      QString subdirname, QString filename, int *verbose, QObject *parent = nullptr);

    void extractImages();
    void determineFileFormat();

    // Pixel processing
    // x-talk
    float xtalkNorAmp = 0.;  // normal xtalk amplitude
    float xtalkRowAmp = 0.;  // row xtalk amplitude
    float xtalkKappa;   // outlier rejection
    QString xtalkDirection = "x";
    int xtalkNsection = 32;
    QString xtalkNorAmpString = "";
    QString xtalkRowAmpString = "";
    QString dataType = "";
    bool doXtalkNor = false;
    bool doXtalkRow = false;
    int xtalkNorMethod = -1;
    int xtalkRowMethod = -1;
    // Nonlinearity
    QList<QVector<float>> nonlinearityCoefficients;
    // Overscan
    // Function pointer (updated externally by the controller)
    float (*combineOverscan_ptr) (const QVector<float> &, const QVector<bool> &, long);


    Mask *mask;
    Data *data;

    QMap<QString, QStringList> headerDictionary;   // Defined once in the controller, we just link to it; NOPE! Local copy, thread safety!
    QMap<QString, QString> filterDictionary;       // Defined once in the controller, we just link to it
    QStringList dummyKeys;                         // Defined once in the controller, we just link to it

    QStringList headerTHELI;

    bool successProcessing = true;

    float progressStepSize = 0.;
    float *progress;
    omp_lock_t *progressLock;
    omp_lock_t *genericLock;

private:
    fitsfile *rawFptr = nullptr;
    LibRaw rawProcessor;

    int rawStatus = 0;

    int numExt = 1;   // Number of FITS extensions in a 2D MEF
    int numSlice = 1; // Number of slices in a 3D cube
    int numHDU = 1;   // Number of HDUs in a FITS file
    int *verbosity;

    long naxis1Raw = 0;  // Before trimming overscan regions
    long naxis2Raw = 0;  // Before trimming overscan regions
    long naxis3Raw = 0;  // Before trimming overscan regions
    long naxis1 = 0;
    long naxis2 = 0;
    long naxis3 = 0;

    QVector<QVector<long>> overscanX;
    QVector<QVector<long>> overscanY;
    QVector<QVector<long>> dataSection;
    QVector<float> gain;

    instrumentDataType instData;   // No pointer, because it is not thread safe (QVectors, QStrings)
    ConfDockWidget *cdw;
    QString name;
    QString fileName;
    QString path;
    QString mainDirName;
    QString subDirName;
    QString baseName;
    QString fitsType;  // SINGLE, MEF or CUBE
    QString dataFormat;  // FITS or RAW
    QString bayerPattern = "";
    QString filter = "Unknown";

    QString dateObsValue = "";
    double crval1 = 0.0;
    double crval2 = 0.0;
    double positionAngle = 0.0;
    float exptimeValue = 0.0;   // header keyword value to calculate other keywords
    float lstValue = 0.0;       // header keyword value to calculate other keywords
    double mjdobsValue = 0.0;   // header keyword value to calculate other keywords
    float sensorTemp = -1000.;
    float cameraTemp = -1000.;
    QString isoSpeed = "";
    bool dateObsValid = true;   // Used to see if we can safely and unambiguously rename files

    QVector<float> dataRaw;
    QVector<float> dataCubeRaw;
    QVector<float> dataCurrent;
    QVector<float> dataXcorr;

    QString fullExtHeaderString;
    QString fullPrimaryHeaderString;
    QStringList extHeader;      // The header of the currently read extension
    QStringList primaryHeader;

    QString uniqueID = "";     // If dateobs cannot be determined unambiguously

    // Processing parameters

    void consistencyChecks();
    void descrambleLiris();
    void getCurrentExtensionData();
    void getDetectorSections();
    float polynomialSum(float x, QVector<float> coefficients);
    void printCfitsioError(QString funcName, int status);
    void readPrimaryHeader();
    void readExtHeader();
    void xtalk_method1();
    void xtalk_method2();
    void xtalk_method3();
    void xtalk_method4();
    void xtalk_method5();
    void xtalk_method6();
    void xtalk_method7();
    void xtalk_method8();
    void xtalk_method9();
    void xtalk_method10();
    void writeImage(int chipMapped);

    void retainMandatoryKeywords();
    void buildTheliHeader();
    void buildTheliHeaderWCS(int chip);
    void buildTheliHeaderFILTER();
    void buildTheliHeaderEXPTIME();
    void buildTheliHeaderMJDOBS();
    void buildTheliHeaderAIRMASS();
    void buildTheliHeaderDATEOBS();
    void buildTheliHeaderGAIN(int chip);

    bool searchKey(const QString &searchKeyName, const QStringList &possibleKeyNames, QStringList &outputHeader);
    bool searchKeyInHeader(const QString &searchKey, const QStringList &possibleKeyNames, const QStringList &inputHeader, QStringList &outputHeader);
    bool searchKeyInHeaderValue(const QStringList &possibleKeyNames, const QStringList &inputHeader, int &value);
    bool searchKeyInHeaderValue(const QStringList &possibleKeyNames, const QStringList &inputHeader, float &value);
    bool searchKeyInHeaderValue(const QStringList &possibleKeyNames, const QStringList &inputHeader, double &value);
    bool searchKeyInHeaderValue(const QStringList &possibleKeyNames, const QStringList &inputHeader, QString &keyValue);
    bool searchKeyValue(const QStringList &possibleKeyNames, int &value);
    bool searchKeyValue(const QStringList &possibleKeyNames, float &value);
    bool searchKeyValue(const QStringList &possibleKeyNames, double &value);
    bool searchKeyValue(const QStringList &possibleKeyNames, QString &value);
    double dateobsToMJD();
    double dateobsToLST();
    double calcAirmass(double ahourangle);
    double localSiderealTimeToAirmass();
    bool checkFormatDATEOBS();
    bool searchKeyInHeaderCRVAL(const QString searchKey, const QStringList &possibleKeyNames, const QStringList &inputHeader, QStringList &outputHeader);
    void searchKeyCRVAL(const QString searchKey, const QStringList &possibleKeyNames, QStringList &outputHeader);
    bool searchKeyLST(const QStringList &possibleKeyNames);
    bool searchKeyInHeaderLST(const QStringList &possibleKeyNames, const QStringList &inputHeader);
    void importRAW();
    void buildHeaderRAW();
    void extractImagesFITS();
    void extractImagesRAW();
    void overwriteCameraIniRAW();
    void backupRawFile();
    bool checkChipGeometry();
    int getNorXtalkMethod();
    int getRowXtalkMethod();
    void correctXtalk();
    void correctNonlinearity(int chip);
    void correctOverscan(float (*combineFunction_ptr)(const QVector<float> &, const QVector<bool> &, long), QVector<long> overscanXArea, QVector<long> overscanYArea);
    void cropDataSection(QVector<long> dataSection);
    void initMyImage(int chip);
    void getDataInFirstCubeSlice();
    void getDataInCube();
    void sliceCube(long slice);
    void writeImageSlice(int chip, long slice);
    void stackCube();
    void manualWCSfix(int chip);

    void WCSbuildCRVAL();
    void WCSbuildCDmatrix(int chip);
    void WCSbuildRADESYS();
    void WCSbuildEQUINOX();
    void WCSbuildCRPIX(int chip);
    void WCSbuildCTYPE();

    bool individualFixCDmatrix(int chip);
    bool individualFixCRVAL();
    void uncompress();
    int inferChipID(int chip);
    void convertToElectrons(int chip);
    float geometricGain(QVector<float> detectorGains);

    bool individualFixGAIN(int chip);
    bool isDetectorAlive(int &chipMapped);
    void individualFixOutName(QString &outname, const int chipID);
signals:
    void messageAvailable(QString message, QString type);
    void warning();
    void critical();
    void showMessageBox(QString trigger, QString part1, QString part2);

public slots:
};

#endif // SPLITTER_H
