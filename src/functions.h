#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <valarray>
#include "fitsio2.h"
#include <QString>
#include <QFile>
#include <QDir>
#include <QComboBox>
#include <QLineEdit>
#include <QVector>
#include <QDebug>
#include <QProcess>
#include <QProgressBar>
#include <QPlainTextEdit>
#include <QStringList>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

struct dataGSL {
    size_t nx, ny;
    double* z; // pointer to values of 2D function
    double* sigma; // dito for sigmas
};

QString boolToString(bool test);
double getPosAnglefromCD(double cd11, double cd12, double cd21, double cd22);
void rotateCDmatrix(double &cd11, double &cd12, double &cd21, double &cd22, double PAnew);
void get_rotimsize(long naxis1, long naxis2, double PAold, double PAnew, long &Nnew, long &Mnew);
QStringList datadir2StringList(QLineEdit *lineEdit);
QString get_fileparameter(QFile *, QString, QString warn = "");
QString get_fileHeaderParameter(QFile *file, QString parametername);
QString get_fileparameter_vector(QFile *, QString, QString warn = "");
QVector<int> get_fileparameter_FullVector(QFile *file, QString parametername);
long get_memory();
void showLogfile(QString logname, QString line = "");
void fill_combobox(QComboBox *, QString);
void paintPathLineEdit(QLineEdit *, QString, QString check = "dir");
bool listContains(QStringList stringList, QString string);
void exec_system_command(QString);
QVector<float> getSmallSample(const QVector<float> &data, const QVector<bool> &mask = QVector<bool>());
void initEnvironment(QString &thelidir, QString &userdir, QString &tmpdir);
void listSwapLastPairs(QStringList &stringlist, int n);
long numFilesDir(QString path, QString filter);
QString countReferenceSources(QString catalog);
bool hasMatchingPartnerFiles(QString dirname1, QString suffix1, QString dirname2, QString suffix2, QString infomessage);
QString removeLastWords(QString string, int n=1, const QChar sep=' ');
QString getNthWord(QString string, int n, const QChar sep=' ');
QString read_system_command(QString);
QString read_MultipleLines_system_command(QString shell_command);
QString translateServer(QString downloadServer);
QString truncateImageList(QStringList list, int dim);
void message(QPlainTextEdit *pte, QString text, QString type="");
void selectFirstActiveItem(QComboBox *cb);
void get_array_subsample(QVector<float> &data, QVector<double> &datasub, int stepSize);
double medianerrMask(const QVector<double> &vector_in, const QVector<bool> &mask = QVector<bool>());
double madMask(const QVector<double> &vector_in, const QVector<bool> &mask = QVector<bool>(), QString ignoreZeroes = "");
float meanIterative(QVector<float> data, float kappa, int iterMax);
QString hmsToDecimal(QString hms);
QString dmsToDecimal(QString dms);
double dateobsToDecimal(QString dateobs);
QString getLastWord(QString string, const QChar sep);
QString get2ndLastWord(QString string, const QChar sep);
void removeLastCharIf(QString &string, const QChar character);
bool moveFiles(QString filter, QString sourceDirName, QString targetDirName);
bool moveFile(QString filename, QString sourceDirPath, QString targetDirPath, bool skipNonExistingFile = false);
bool deleteFile(QString fileName, QString path);
QVector<float> modeMask(QVector<float> &data, QString mode, const QVector<bool> &mask = QVector<bool>(), bool smooth = true);
int modeMask_sampleDensity(long numDataPoints, int numBins, float SNdesired);
QVector<float> modeMask_clipData(const QVector<float> &data, const QVector<bool> &mask, int sampleDensity = 1, float minVal = 0., float maxVal = 0.);
QVector<long> modeMask_buildHistogram(QVector<float> &data, float &rescale, const int numBins, const float minVal,
                                      const float maxVal, const float madVal, const bool smooth = true);
void modeMask_classic(const QVector<long> histogram, float &skyValue);
void modeMask_gaussian(QVector<long> histogram, float &skyValue);
void modeMask_stable(const QVector<long> histogram, float &skyValue);
float meanMask(const QVector<float> &data, const QVector<bool> &mask = QVector<bool>(), long maxLength = 0);
float medianMask(const QVector<float> &data, const QVector<bool> &mask = QVector<bool>(), long maxLength = 0);
float straightMedian_MinMax(QVector<float> &data, const int nlow, const int nhigh);
float straightMedian_MinMax(QList<float> &data, const int nlow, const int nhigh);
bool readData3D(QString path, QVector<double> &x, QVector<double> &y, QVector<double> &z);
long remainingDataDriveSpace(QString maindir);
int gauss_f(const gsl_vector *x, void *params, gsl_vector *f);
int gauss_df(const gsl_vector *x, void *params, gsl_matrix *J);
int gauss_fdf(const gsl_vector *x, void *params, gsl_vector *f, gsl_matrix *J);
void mkAbsDir(QString absDirName);

// template functions end in "_T"
// This is to distinguish them from the same functions
// which are needed explicitly because they are select through functors
// Test if a RA/DEC coordinate is contained within an image


//***************************************************************
// Template for matrix multiplication.
// Stores the result in the elements of the second matrix
//***************************************************************
template<class T1, class T2>
void matrixMult_T(T1 a11, T1 a12, T1 a21, T1 a22,
                  T2 &b11, T2 &b12, T2 &b21, T2 &b22)
{
    T2 c11, c12, c21, c22;

    c11 = a11 * b11 + a12 * b21;
    c12 = a11 * b12 + a12 * b22;
    c21 = a21 * b11 + a22 * b21;
    c22 = a21 * b12 + a22 * b22;

    b11 = c11;
    b12 = c12;
    b21 = c21;
    b22 = c22;
}

template<class T>
QVector<T> removeVectorItems_T(QVector<T> data, QVector<int> badIndexes)
{
    int i=0;
    QVector<bool> removeFlag = QVector<bool>(data.length(), false);
    for (i=0; i<badIndexes.length(); ++i) {
        if (i==10000) qDebug() << "10000";
        if (i==100000) qDebug() << "100000";
        if (i==1500000) qDebug() << "1500000";
        removeFlag.operator[](i) = true;
    }

    QVector<T> filtered = QVector<T>();
    for (i=0; i<removeFlag.length(); ++i) {
        if (!removeFlag.at(i)) filtered.push_back(data[i]);
    }
    return filtered;
}

template<class T>
T minVec_T(QVector<T> const &qv)
{
    if (qv.length() == 0) {
        qDebug() << "QDEBUG: minVec(): vector has zero length.";
        return 0;
    }
    T extreme = qv.at(0);
    for ( const auto &i : qv) {
        if (i < extreme) extreme = i;
    }
    return (extreme);
}

template<class T>
T maxVec_T(QVector<T> const &qv)
{
    if (qv.length() == 0) {
        qDebug() << "QDEBUG: minVec(): vector has zero length.";
        return 0;
    }
    T extreme = qv.at(0);
    for ( const auto &i : qv) {
        if (i > extreme) extreme = i;
    }
    return (extreme);
}

template<class T>
T meanQuantile_T(QVector<float> const &data, const long start, const long end)
{
    T mean = 0.;
    long numpixels = 0;
    long naned = 0;

    for (long i=start; i<end; ++i) {
        if (!std::isnan(data[i])) {
            mean += data[i];
            ++numpixels;
        }
        else ++naned;
    }

    if (numpixels != 0.) mean /= numpixels;
    else mean = 0.;

    if (naned > (end-start) / 2)
        mean = sqrt(-1); // return NaN

    return (mean);
}

template<class T>
T straightMedian_T(const QVector<T> &vector_in, long maxLength = 0, bool center = true)
{
    long maxDim = vector_in.length();
    if (maxDim == 0) return 0.;

    // Usually, we take all elements in a vector.
    // However, in some cases it may be that elements get masked and the vector is not entirely filled.
    // In this case, an optional maxLength argument is provided
    if (maxLength > 0 && maxLength < maxDim) maxDim = maxLength;

    QVector<T> vector(maxDim);
    for (long i=0; i<maxDim; ++i) vector[i] = vector_in[i];
    std::sort(vector.begin(), vector.end());
    T med;
    if (center) {
        // Calculate average of central two elements if number is even
        med = vector.size() % 2
                ? vector[vector.size() / 2.]
                : ((T)vector[vector.size() / 2. - 1] + vector[vector.size() / 2.]) * .5;
    }
    else {
        // Do not calculate average (needed when calculate median CRVAL1 to avoid issues when crossing the 360/0 degree boundary)
        med = vector[vector.size() / 2.];
    }
    return med;
}

// A faster implementation that can be used if the input data is already "clean", and does not need to be preserved
template<class T>
T straightMedianInline(QVector<T> &data)
{
    long dim = data.length();
    if (dim == 0) return 0.;

    std::sort(data.begin(), data.end());
    T med;
    med = dim % 2
            ? data[dim/2]
            : ((T)data[dim/2-1] + data[dim/2]) * .5;

    return med;
}

// Masked median
template<class T>
T medianMask_T(QVector<T> vector_in, QVector<bool> mask = QVector<bool>(), QString ignoreZeroes = "")
{
    long maxDim = vector_in.length();
    if (maxDim == 0) return 0.;

    // fast-track
    if (mask.isEmpty() && ignoreZeroes == "") {
        return straightMedian_T(vector_in);
    }

    // slower median with masks, and potentially pixel with value zero that are to be ignored
    if (mask.isEmpty()) {
        mask.fill(false, maxDim);
    }

    QVector<T> vector;
    vector.reserve(maxDim);
    int i = 0;
    for (auto &it: vector_in) {
        if (!mask[i] && ignoreZeroes == "") vector.append(it);
        if (!mask[i] && ignoreZeroes != "" && it != 0.) vector.append(it);
        ++i;
    }

    if (vector.size() == 0) return 0.;
    else {
        return straightMedian_T(vector);
    }
}

// Masked mean
template<class T>
T meanMask_T(QVector<T> &vector_in, QVector<bool> mask = QVector<bool>())
{
    long maxDim = vector_in.length();
    if (maxDim == 0) return 0.;

    // fast-track
    if (maxDim == 0) return 0.;
    if (mask.isEmpty()) {
        return std::accumulate(vector_in.begin(), vector_in.end(), .0) / vector_in.size();
    }
    else {
        QVector<T> vector;
        vector.reserve(maxDim);
        int i = 0;
        for (auto &it: vector_in) {
            if (!mask[i]) vector.append(it);
            ++i;
        }

        if (vector.size() == 0) return 0.;
        else
            return std::accumulate(vector.begin(), vector.end(), .0) / vector.size();
    }
}

template<class T>
T rmsMask_T(QVector<T> &vector_in, QVector<bool> mask = QVector<bool>())
{
    long maxDim = vector_in.length();
    if (maxDim == 0) return 0.;

    if (mask.isEmpty()) {
        mask.fill(false, maxDim);
    }

    QVector<T> vector;
    vector.reserve(maxDim);
    int i = 0;
    for (auto &it: vector_in) {
        if (!mask[i]) vector.append(it);
        ++i;
    }

    T rmsval = 0.;
    T meanval = meanMask_T(vector_in, mask);
    if (vector.size() <= 1) return 0.;
    else {
        for (auto &it: vector) {
            rmsval += (meanval - it) * (meanval - it);
        }
        return std::sqrt(rmsval / (vector.size()-1) );
    }
}

// Masked mad
template<class T>
T madMask_T(QVector<T> vector_in, const QVector<bool> mask = QVector<bool>(), QString ignoreZeroes = "")
{
    long maxDim = vector_in.length();
    if (maxDim == 0) return 0.;

    QVector<T> vector;
    vector.reserve(maxDim);
    long i = 0;
    if (!mask.isEmpty()) {
        for (auto &it: vector_in) {
            if (!mask[i] && ignoreZeroes == "") vector.append(it);
            if (!mask[i] && ignoreZeroes != "" && it != 0.) vector.append(it);
            ++i;
        }
    }
    else {
        for (auto &it: vector_in) {
            if (ignoreZeroes == "") vector.append(it);
            if (ignoreZeroes != "" && it != 0.) vector.append(it);
            ++i;
        }
    }

    QVector<T> diff;
    diff.reserve(maxDim);
    if (vector.size() == 0) return 0.;
    else {
        T med = straightMedian_T(vector);
        for (int i=0; i<vector.size(); ++i) {
            diff.append(fabs(vector.at(i)-med));
        }
        return straightMedian_T(diff);
    }
}

template <typename T> int sgn(T x) {
    if (x >= 0) return 1;
    else return -1;
}

// Check a QVector for duplicates
template<class T>
bool hasDuplicates_T(QVector<T> data)
{
    std::sort(data.begin(), data.end());
    T old = data[0];
    long j = 0;
    long nDuplicates = 0;
    for (auto &it : data) {
        if (j>0 && it == old) ++nDuplicates;
        old = it;
        ++j;
    }
    if (nDuplicates > 0) return true;
    else return false;
}

// The index of the entry with the highest value
template<class T>
T maxIndex(QVector<T> const &data)
{
    long index = 0;
    T maxval = data[0];

    long i=0;
    for (auto &pixel : data) {
        if (pixel > maxval) {
            maxval = pixel;
            index = i;
        }
        ++i;
    }

    return index;
}

// Convolve a 1D array with a Gaussian of width sigma (units of array elements)
// The footprint of the Gaussian is +/- 1 sigma
template<class T1, class T2>
void smooth_array_T(QVector<T1> &data, T2 sigma)
{
    float ss2 = 2.*sigma*sigma;
    long dim = data.length();
    float footprint = 1.0;
    QVector<float> tmp(dim);
    for (long i=0; i<dim; ++i) {
        float wsum = 0.;
        long jmin = i-footprint*sigma < 0 ? 0 : i-footprint*sigma;
        long jmax = i+footprint*sigma >= dim ? dim : i+footprint*sigma;
        for (long j=jmin; j<jmax; ++j) {
            float r = float(i-j);
            float weight = exp(-r*r/ss2);
            tmp[i] += (float(data[j]) * weight);
            wsum += weight;
        }
        tmp[i] /= wsum;
    }

    long i=0;
    for (auto &it : data) {
        it = T1(tmp[i]);
        ++i;
    }

    //data = tmp;
}

template<class T1, class T2>
void matrix_mult_T(T1 a11, T1 a12, T1 a21, T1 a22,
                   T2 &b11, T2 &b12, T2 &b21, T2 &b22)
{
    T2 c11, c12, c21, c22;

    c11 = a11 * b11 + a12 * b21;
    c12 = a11 * b12 + a12 * b22;
    c21 = a21 * b11 + a22 * b21;
    c22 = a21 * b12 + a22 * b22;

    b11 = c11;
    b12 = c12;
    b21 = c21;
    b22 = c22;
}

#endif // FUNCTIONS_H