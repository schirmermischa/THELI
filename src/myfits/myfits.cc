#include "myfits.h"
#include "mywcs.h"
#include <omp.h>
#include "../tools/cfitsioerrorcodes.h"

#include "../functions.h"
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

using namespace std;

// Ctor
MyFITS::MyFITS(QObject *parent) : QObject(parent)
{

}

// Ctor
MyFITS::MyFITS(long n, long m, QObject *parent) : QObject(parent)
{
    name = "";
    naxis1 = n ;
    naxis2 = m;
    data.resize(n*m);
}

MyFITS::~MyFITS()
{
    int status = 0;
    if (fullheaderAllocated) fits_free_memory(fullheader, &status);
}

// Ctor to prepare FITS for reading, or just to init
MyFITS::MyFITS(QString fileName, QString mode, QObject *parent) : QObject(parent)
{
    // Empty constructor
    name = fileName;
    QString path = QFileInfo(fileName).absolutePath()+'/';
    weightName = path+QFileInfo(fileName).completeBaseName()+".weight.fits";

    // If the data block should be read
    if (QFile(fileName).exists() && mode == "Read") {
        if (!loadData()) {
            QMessageBox msgBox;
            msgBox.setText("Could not load "+fileName);
            msgBox.exec();
        }
    }
}

// Ctor to prepare FITS for writing
MyFITS::MyFITS(QString fileName, const long n, const long m, QVector<float> &dataOut,
               QObject *parent) : QObject(parent)
{
    name = fileName;
    naxis1 = n;
    naxis2 = m;
    data = dataOut;
}

// Ctor to prepare FITS for writing
MyFITS::MyFITS(QString fileName, const long n, const long m, float value, QObject *parent) : QObject(parent)
{
    name = fileName;
    naxis1 = n;
    naxis2 = m;
    constValue = value;
}

// copy Ctor
MyFITS::MyFITS(const MyFITS &myfits, QObject *parent) : QObject(parent)
{
    name = myfits.name;
    weightName = myfits.weightName;
    headerRef = myfits.headerRef;
    numExt = myfits.numExt;
    plateScale = myfits.plateScale;
    hasWCS = myfits.hasWCS;
    naxis1 = myfits.naxis1;
    naxis2 = myfits.naxis2;
    crval1 = myfits.crval1;
    crval2 = myfits.crval2;
    crpix1 = myfits.crpix1;
    crpix2 = myfits.crpix2;
    cd1_1 = myfits.cd1_1;
    cd1_2 = myfits.cd1_2;
    cd2_1 = myfits.cd2_1;
    cd2_2 = myfits.cd2_2;
    data.resize(naxis1*naxis2);
    data = myfits.data;
}

// Extract the FILTER keyword from a yet unopened FITS file
QString MyFITS::readFILTER()
{
    int status = 0;
    fitsfile *fptr = nullptr;
    initFITS(&fptr, &status);
    char filter[80];
//    fits_open_file(&fptr, name.toUtf8().data(), READONLY, &status);
    fits_read_key_str(fptr, "FILTER", filter, NULL, &status);
    fits_close_file(fptr, &status);
    QString filterString(filter);

    printerror(status);
    return filterString.simplified();
}

// Extract the NAXIS1, NAXIS2, CRPIX1, CRPIX2, SKYVALUE keywords from a yet unopened FITS file for swarpfilter()
bool MyFITS::informSwarpfilter(long &naxis1, long &naxis2, double &crpix1, double &crpix2, double &sky, double &fluxscale)
{
    int status = 0;
    fitsfile *fptr = nullptr;
    initFITS(&fptr, &status);
//    fits_open_file(&fptr, name.toUtf8().data(), READONLY, &status);
    fits_read_key_dbl(fptr, "CRPIX1", &crpix1, NULL, &status);
    fits_read_key_dbl(fptr, "CRPIX2", &crpix2, NULL, &status);
    fits_read_key_lng(fptr, "NAXIS1", &naxis1, NULL, &status);
    fits_read_key_lng(fptr, "NAXIS2", &naxis2, NULL, &status);
    fits_read_key_dbl(fptr, "SKYVALUE", &sky, NULL, &status);
    fits_read_key_dbl(fptr, "FLXSCALE", &fluxscale, NULL, &status);
    fits_close_file(fptr, &status);
    printerror(status);
    if (status) return false;
    else return true;
}

void MyFITS::initFITS(fitsfile **fptr, int *status)
{
    if (name.isNull() || name.isEmpty()) {
        *status = 1;
        qDebug() << "QDEBUG: ERROR: MyFITS::initFITS(): file name empty or not initialized!";
        return;
    }
    // Bypassing a memory leak in cfitsio
    QFile file(name);
    if (!file.exists()) {
        qDebug() << "QDEBUG: ERROR: MyFITS:initFITS(): " + name + " : file does not exist";
        *status = 104;
        return;
    }
    fits_open_file(fptr, name.toUtf8().data(), READONLY, status);
    fits_get_num_hdus(*fptr, &numExt, status);
    if (numExt > 1) {
        QMessageBox msgBox;
        msgBox.setText(name+"is a multi-extension FITS file, which are currently not supported.");
        msgBox.exec();
        *status = 1;
    }
}

void MyFITS::readHeader(fitsfile **fptr, int *status)
{
    if (*status) return;
    // Read the entire header. This should always work!
//    char *fullheader = nullptr;
    fits_hdr2str(*fptr, 0, NULL, 0, &fullheader, &numHeaderKeys, status);

    fullheaderAllocated = true;
    if (*status) return;

    fullHeaderString = QString::fromUtf8(fullheader);
    // freed in destructor
    //    fits_free_memory(fullheader, status);

    // Map the header onto a QVector<QString>
    int cardLength = 80;
    long length = fullHeaderString.length();
    if (length<80) return;
    header.clear();
    for (long i=0; i<=length-cardLength; i+=cardLength) {
        QStringRef subString(&fullHeaderString, i, cardLength);
        QString string = subString.toString();

        // Omitting a few keywords (otherwise they end up duplicated in the processed science data; dunno why)
        /*
        if (string.contains("SIMPLE  =")
                || string.contains("BITPIX  =")
                || string.contains("NAXIS   =")
                || string.contains("NAXIS1  =")
                || string.contains("NAXIS2  =")
                || string.contains("EXTEND  =")
                || string.contains("END      ")) continue;
        */
        header.push_back(string);
    }
}

void MyFITS::readData(fitsfile **fptr, int *status)
{
    if (*status) return;
    long naxis[2];

    // Get image geometry
    fits_get_img_size(*fptr, 2, naxis, status);

    // Read the data block
    naxis1 = naxis[0];
    naxis2 = naxis[1];
    long nelements = naxis1*naxis2;
    float *buffer = new float[nelements];
    float nullval = 0.;
    int anynull;
    long fpixel = 1;
    fits_read_img(*fptr, TFLOAT, fpixel, nelements, &nullval, buffer, &anynull, status);

    if (! *status) {
        data.resize(nelements);
        for (long i=0; i<nelements; ++i) {
            // if (isinf(buffer[i])) data[i] = 0.;
            // else data[i] = buffer[i];
            data[i] = buffer[i];
        }
    }
    delete [] buffer;
}

bool MyFITS::loadData()
{
    int status = 0;
    fitsfile *fptr = nullptr;
    initFITS(&fptr, &status);
    readHeader(&fptr, &status);
    readData(&fptr, &status);
    initWCS(&status);
    initTHELIheader(&status);
    checkTHELIheader(&status);
    fits_close_file(fptr, &status);

    headerInfoProvided = true;

    printerror(status);
    if (status) return false;
    else return true;
}

bool MyFITS::provideHeaderInfo()
{
    if (headerInfoProvided) return true;

    int status = 0;
    fitsfile *fptr = nullptr;
    initFITS(&fptr, &status);
    readHeader(&fptr, &status);
    initWCS(&status);
    initTHELIheader(&status);
    checkTHELIheader(&status);
    fits_close_file(fptr, &status);

    headerInfoProvided = true;

    printerror(status);
    if (status) return false;
    else return true;
}

void MyFITS::getMJD()
{
    fitsfile *fptr = nullptr;
    int status = 0;
    fits_open_file(&fptr, name.toUtf8().data(), READONLY, &status);
    fits_read_key_dbl(fptr, "MJD-OBS", &mjdobs, NULL, &status);
    fits_close_file(fptr, &status);

    printerror(status);
    if (status) hasMJDread = false;
    else hasMJDread = true;
}

// Free memory (does not destroy the object as such)
void MyFITS::erase()
{
    data.clear();
    data.squeeze();
}

MyWCS MyFITS::loadWCS()
{
    int status = 0;
    fitsfile *fptr = nullptr;
    initFITS(&fptr, &status);
    getWCSheaderKeys(&fptr, &status);
    fits_close_file(fptr, &status);

    printerror(status);
    if (status) {
        hasWCS = false;
        plateScale = 1.0;
        return myWCS;
    }
    else {
        hasWCS = true;
        plateScale = myWCS.getPlateScale();
        return myWCS;
    }
}

void MyFITS::initWCS(int *status)
{
    if (*status) return;
    for (auto &it : header) {
        extractKeywordLong(it, "NAXIS1", myWCS.naxis1);
        extractKeywordLong(it, "NAXIS2", myWCS.naxis2);
        extractKeywordDouble(it, "CRPIX1", myWCS.crpix1);
        extractKeywordDouble(it, "CRPIX2", myWCS.crpix2);
        extractKeywordDouble(it, "CRVAL1", myWCS.crval1);
        extractKeywordDouble(it, "CRVAL2", myWCS.crval2);
        extractKeywordDouble(it, "CD1_1", myWCS.cd1_1);
        extractKeywordDouble(it, "CD1_2", myWCS.cd1_2);
        extractKeywordDouble(it, "CD2_1", myWCS.cd2_1);
        extractKeywordDouble(it, "CD2_2", myWCS.cd2_2);
        extractKeywordFloat(it, "EQUINOX", myWCS.equinox);
    }
    myWCS.checkValidity();
    myWCS.getPlateScale();
    hasWCS = myWCS.isValid;
    plateScale = myWCS.plateScale;

    // redundant, but needed (e.g. by Iview, which does not read the wcs object
    crpix1 = myWCS.crpix1;
    crpix2 = myWCS.crpix2;
    crval1 = myWCS.crval1;
    crval2 = myWCS.crval2;
    cd1_1 = myWCS.cd1_1;
    cd1_2 = myWCS.cd1_2;
    cd2_1 = myWCS.cd2_1;
    cd2_2 = myWCS.cd2_2;
    equinox = myWCS.equinox;
    if (naxis1 == 0) naxis1 = myWCS.naxis1;
    if (naxis2 == 0) naxis2 = myWCS.naxis2;
}

void MyFITS::initTHELIheader(int *status)
{
    if (*status) return;
    for (auto &it : header) {
        extractKeywordDouble(it, "MJD-OBS", mjdobs);
        extractKeywordString(it, "FILTER", filter);
        extractKeywordString(it, "DATE-OBS", dateobs);
        extractKeywordFloat(it, "EXPTIME", exptime);
        extractKeywordFloat(it, "GAINCORR", gainNormalization);
        extractKeywordInt(it, "BITPIX", bitpix);
        extractKeywordFloat(it, "SKYVALUE", skyValue);
        extractKeywordFloat(it, "AIRMASS", airmass);
        extractKeywordFloat(it, "FWHM", fwhm);
        extractKeywordFloat(it, "ELLIP", ellipticity);
        extractKeywordFloat(it, "RZP", RZP);
    }
    if (mjdobs != 0.0) hasMJDread = true;
    else hasMJDread = false;
}

void MyFITS::checkTHELIheader(int *status)
{
    if (*status) return;
    if (fullHeaderString.contains("NAXIS1  =")
            && fullHeaderString.contains("NAXIS2  =")
            && fullHeaderString.contains("CRPIX1  =")
            && fullHeaderString.contains("CRPIX2  =")
            && fullHeaderString.contains("CRVAL1  =")
            && fullHeaderString.contains("CRVAL2  =")
            && fullHeaderString.contains("CD1_1   =")
            && fullHeaderString.contains("CD1_2   =")
            && fullHeaderString.contains("CD2_1   =")
            && fullHeaderString.contains("CD2_2   =")
            && fullHeaderString.contains("EQUINOX =")
            && fullHeaderString.contains("EXPTIME =")
            && fullHeaderString.contains("FILTER  =")
            && fullHeaderString.contains("OBJECT  =")
            && fullHeaderString.contains("MJD-OBS =")) {
        hasTHELIheader = true;
    }
    else {
        hasTHELIheader = false;
    }
}


void MyFITS::extractKeywordDouble(QString card, QString key, double &value)
{
    if (card.contains(key)) value = card.split("=")[1].split("/")[0].simplified().remove("'").toDouble();
}

void MyFITS::extractKeywordFloat(QString card, QString key, float &value)
{
    if (card.contains(key)) value = card.split("=")[1].split("/")[0].simplified().remove("'").toFloat();
}

void MyFITS::extractKeywordLong(QString card, QString key, long &value)
{
    if (card.contains(key)) value = card.split("=")[1].split("/")[0].simplified().remove("'").toLong();
}

void MyFITS::extractKeywordInt(QString card, QString key, int &value)
{
    if (card.contains(key)) value = card.split("=")[1].split("/")[0].simplified().remove("'").toInt();
}

void MyFITS::extractKeywordString(QString card, QString key, QString &value)
{
    if (card.contains(key)) value = card.split("=")[1].split("/")[0].simplified().remove("'");
}

void MyFITS::getWCSheaderKeys(fitsfile **fptr, int *status)
{
    if (*status) return;
    fits_read_key_dbl(*fptr, "CRPIX1", &myWCS.crpix1, NULL, status);
    fits_read_key_dbl(*fptr, "CRPIX2", &myWCS.crpix2, NULL, status);
    fits_read_key_dbl(*fptr, "CRVAL1", &myWCS.crval1, NULL, status);
    fits_read_key_dbl(*fptr, "CRVAL2", &myWCS.crval2, NULL, status);
    fits_read_key_dbl(*fptr, "CD1_1", &myWCS.cd1_1, NULL, status);
    fits_read_key_dbl(*fptr, "CD1_2", &myWCS.cd1_2, NULL, status);
    fits_read_key_dbl(*fptr, "CD2_1", &myWCS.cd2_1, NULL, status);
    fits_read_key_dbl(*fptr, "CD2_2", &myWCS.cd2_2, NULL, status);
    fits_read_key_lng(*fptr, "NAXIS1", &myWCS.naxis1, NULL, status);
    fits_read_key_lng(*fptr, "NAXIS2", &myWCS.naxis2, NULL, status);
    fits_read_key_dbl(*fptr, "MJD-OBS", &mjdobs, NULL, status);
}

bool MyFITS::loadGeometry(int &naxis1, int &naxis2)
{
    int status = 0;
    int naxis[2];
    fitsfile *fptr = nullptr;
    initFITS(&fptr, &status);

    // Get the linear WCS and other parameters
    fits_get_img_dim(fptr, naxis, &status);
    fits_close_file(fptr, &status);

    printerror(status);
    if (status) {
        naxis1 = 0;
        naxis2 = 0;
        QMessageBox msgBox;
        msgBox.setText("ERROR: Could not determine image geometry for "+name);
        msgBox.exec();
        return false;
    }
    else {
        naxis1 = naxis[0];
        naxis2 = naxis[1];
        qDebug() << "CHECK: MyFITS:loadGeometry()" << naxis1 << naxis2;
        return true;
    }
}

void MyFITS::propagateHeader(fitsfile *fptr, QVector<QString> header)
{
    if (header.isEmpty()) return;

    int status = 0;
    // DO NOT COPY BITPIX, NAXIS, NAXIS1/2, EXTEND keywords
    for (int i=0; i<header.length(); ++i) {
        QString key = header[i].split("=")[0].simplified();
        if (key == "SIMPLE"
                || key == "END"
                || key == "BZERO"
                || key.contains("NAXIS")
                || key.contains("BITPIX")
                || key.contains("EXTEND")
                || key.contains("END")) {
            continue;
        }
        fits_write_record(fptr, header[i].toUtf8().constData(), &status);
    }
}

// If the 'headerRef' member is set, the header from that image will be copied.
bool MyFITS::write(QString history, float exptime, QString filter, QVector<QString> header)
{
    // The new output file
    fitsfile *fptr;
    int status = 0;
    long fpixel = 1;
    long nelements = naxis1*naxis2;
    float *array = new float[nelements];
    for (long i=0; i<nelements; ++i) {
        array[i] = data[i];
    }

    int bitpix = FLOAT_IMG;
    long naxis = 2;
    long naxes[2] = { naxis1, naxis2 };

    // THIS IS CONFUSING SHOULD BE WRITTEN MORE CLEANLY

    // Overwrite file if it exists
    name = "!"+name;
    fits_create_file(&fptr, name.toUtf8().data(), &status);
    // If requested, copy a reference header.
    // WARNING: This assumes that the image geometries are identical!
    if (!headerRef.isEmpty()) {
        fitsfile *reference_fptr = nullptr;
        int status_ref = 0;
        fits_open_file(&reference_fptr, headerRef.toUtf8().data(), READONLY, &status_ref);
        fits_copy_header(reference_fptr, fptr, &status_ref);
        // REMOVE BITPIX, NAXIS?, EXTEND etc keywords (already present)
        /*
        fits_delete_key(reference_fptr, "SIMPLE", &status_ref);
        fits_delete_key(reference_fptr, "BITPIX", &status_ref);
        fits_delete_key(reference_fptr, "NAXIS", &status_ref);
        fits_delete_key(reference_fptr, "NAXIS1", &status_ref);
        fits_delete_key(reference_fptr, "NAXIS2", &status_ref);
        fits_delete_key(reference_fptr, "EXTEND", &status_ref);
        */
        fits_close_file(reference_fptr, &status_ref);
        printerror(status_ref);
    }

    fits_create_img(fptr, bitpix, naxis, naxes, &status);
    fits_write_img(fptr, TFLOAT, fpixel, nelements, array, &status);
    if (exptime >= 0.) {
        fits_write_key_flt(fptr, "EXPTIME", exptime, 6, nullptr, &status);
    }

    if (!filter.isEmpty()) {
        // Write a series of keywords (so that a master calibration file
        // does not trigger any header errors)
        // The filter keyword is written after the header update
        /*
        fits_write_key_str(fptr, "DATE-OBS", "2000-01-01T00:00:00.000", nullptr, &status);
        fits_write_key_dbl(fptr, "MJD-OBS", 0.0, 9, nullptr, &status);
        fits_write_key_dbl(fptr, "CRVAL1", 0.0, 9, nullptr, &status);
        fits_write_key_dbl(fptr, "CRVAL2", 0.0, 9, nullptr, &status);
        fits_write_key_flt(fptr, "CRPIX1", 0.0, 6, nullptr, &status);
        fits_write_key_flt(fptr, "CRPIX2", 0.0, 6, nullptr, &status);
        fits_write_key_flt(fptr, "CD1_1", 0.0, 6, nullptr, &status);
        fits_write_key_flt(fptr, "CD1_2", 0.0, 6, nullptr, &status);
        fits_write_key_flt(fptr, "CD2_1", 0.0, 6, nullptr, &status);
        fits_write_key_flt(fptr, "CD2_2", 0.0, 6, nullptr, &status);
        */
    }

    // Copy the original header (plus possible extensions)
    if (headerRef.isEmpty() && !header.isEmpty()) {
        propagateHeader(fptr, header);
    }
    if (!filter.isEmpty()) {
        fits_update_key_str(fptr, "FILTER", filter.toUtf8().data(), nullptr, &status);
    }
    if (addGainNormalization) {
        fits_update_key_flt(fptr, "GAINCORR", gainNormalization, 6, nullptr, &status);
    }
    if (!history.isEmpty()) {
        fits_write_history(fptr, history.toUtf8().data(), &status);
    }
    // BZERO should be 0 after THELI loaded and processed images (data values are scaled by cfitsio when loaded by MyFITS)
    fits_update_key_flt(fptr, "BZERO", 0.0, 6, nullptr, &status);
    fits_close_file(fptr, &status);

    delete [] array;

    printerror(status);
    if (status) {
        qDebug() << "MyFITS::write(): Error writing " << name;
        return false;
    }
    else return true;
}

// If the 'headerRef' member is set, the header from that image will be copied.
bool MyFITS::writeConstImage(QString history, float exptime, QString filter, QVector<QString> header)
{
    // The new output file
    fitsfile *fptr;
    int status = 0;
    long fpixel = 1;
    long nelements = naxis1*naxis2;
    float *array = new float[nelements];
    for (long i=0; i<nelements; ++i) {
        array[i] = constValue;
    }

    int bitpix = FLOAT_IMG;
    long naxis = 2;
    long naxes[2] = { naxis1, naxis2 };

    // THIS IS CONFUSING SHOULD BE WRITTEN MORE CLEANLY

    // Overwrite file if it exists
    name = "!"+name;
    fits_create_file(&fptr, name.toUtf8().data(), &status);

    // If requested, copy a reference header.
    // WARNING: This assumes that the image geometries are identical!
    if (!headerRef.isEmpty()) {
        fitsfile *reference_fptr = nullptr;
        int status_ref = 0;
        fits_open_file(&reference_fptr, headerRef.toUtf8().data(), READONLY, &status_ref);
        fits_copy_header(reference_fptr, fptr, &status_ref);
        // REMOVE BITPIX, NAXIS?, EXTEND etc keywords (already present)
        /*
        fits_delete_key(reference_fptr, "SIMPLE", &status_ref);
        fits_delete_key(reference_fptr, "BITPIX", &status_ref);
        fits_delete_key(reference_fptr, "NAXIS", &status_ref);
        fits_delete_key(reference_fptr, "NAXIS1", &status_ref);
        fits_delete_key(reference_fptr, "NAXIS2", &status_ref);
        fits_delete_key(reference_fptr, "EXTEND", &status_ref);
        */
        fits_close_file(reference_fptr, &status_ref);
        printerror(status_ref);
    }

    fits_create_img(fptr, bitpix, naxis, naxes, &status);
    fits_write_img(fptr, TFLOAT, fpixel, nelements, array, &status);
    if (exptime >= 0.) {
        fits_write_key_flt(fptr, "EXPTIME", exptime, 6, nullptr, &status);
    }

    if (!filter.isEmpty()) {
        // Write a series of keywords (so that a master calibration file
        // does not trigger any header errors)
        // The filter keyword is written after the header update
        /*
        fits_write_key_str(fptr, "DATE-OBS", "2000-01-01T00:00:00.000", nullptr, &status);
        fits_write_key_dbl(fptr, "MJD-OBS", 0.0, 9, nullptr, &status);
        fits_write_key_dbl(fptr, "CRVAL1", 0.0, 9, nullptr, &status);
        fits_write_key_dbl(fptr, "CRVAL2", 0.0, 9, nullptr, &status);
        fits_write_key_flt(fptr, "CRPIX1", 0.0, 6, nullptr, &status);
        fits_write_key_flt(fptr, "CRPIX2", 0.0, 6, nullptr, &status);
        fits_write_key_flt(fptr, "CD1_1", 0.0, 6, nullptr, &status);
        fits_write_key_flt(fptr, "CD1_2", 0.0, 6, nullptr, &status);
        fits_write_key_flt(fptr, "CD2_1", 0.0, 6, nullptr, &status);
        fits_write_key_flt(fptr, "CD2_2", 0.0, 6, nullptr, &status);
        */
    }

    // Copy the original header (plus possible extensions)
    if (headerRef.isEmpty() && !header.isEmpty()) {
        propagateHeader(fptr, header);
    }
    if (!filter.isEmpty()) {
        fits_update_key_str(fptr, "FILTER", filter.toUtf8().data(), nullptr, &status);
    }
    if (addGainNormalization) {
        fits_update_key_flt(fptr, "GAINCORR", gainNormalization, 6, nullptr, &status);
    }
    if (!history.isEmpty()) {
        fits_write_history(fptr, history.toUtf8().data(), &status);
    }
    // BZERO should be 0 after THELI loaded and processed images (data values are scaled by cfitsio when loaded by MyFITS)
    fits_update_key_flt(fptr, "BZERO", 0.0, 6, nullptr, &status);
    fits_close_file(fptr, &status);

    delete [] array;

    printerror(status);
    if (status) {
        qDebug() << "QDEBUG::MyFITS::write(): Error writing " << name;
        return false;
    }
    else return true;
}

bool MyFITS::writeDebayer(QString history, float exptime, QString filter, double mjdobs, QVector<QString> header)
{
    // The new output file
    fitsfile *fptr;
    int status = 0;
    long fpixel = 1;
    long nelements = naxis1*naxis2;
    float *array = new float[nelements];
    for (long i=0; i<nelements; ++i) {
        array[i] = data[i];
    }

    int bitpix = FLOAT_IMG;
    long naxis = 2;
    long naxes[2] = { naxis1, naxis2 };

    // Overwrite file if it exists
    name = "!"+name;
    fits_create_file(&fptr, name.toUtf8().data(), &status);
    fits_create_img(fptr, bitpix, naxis, naxes, &status);
    fits_write_img(fptr, TFLOAT, fpixel, nelements, array, &status);
    if (exptime >= 0.) {
        fits_write_key_flt(fptr, "EXPTIME", exptime, 6, nullptr, &status);
    }

    // Copy the original header (plus possible extensions)
    if (headerRef.isEmpty() && !header.isEmpty()) {
        propagateHeader(fptr, header);
    }
    if (!filter.isEmpty()) {
        fits_update_key_str(fptr, "FILTER", filter.toUtf8().data(), nullptr, &status);
    }
    fits_update_key_dbl(fptr, "MJD-OBS", mjdobs, 15, nullptr, &status);
    if (addGainNormalization) {
        fits_update_key_flt(fptr, "GAINCORR", gainNormalization, 6, nullptr, &status);
    }
    if (!history.isEmpty()) {
        fits_write_history(fptr, history.toUtf8().data(), &status);
    }
    // BZERO should be 0 after THELI loaded and processed images (data values are scaled by cfitsio when loaded by MyFITS)
    fits_update_key_flt(fptr, "BZERO", 0.0, 6, nullptr, &status);
    fits_close_file(fptr, &status);

    delete [] array;

    printerror(status);
    if (status) {
        qDebug() << "QDEBUG::MyFITS::write(): Error writing " << name;
        return false;
    }
    else return true;
}

// If the 'headerRef' member is set, the header from that image will be copied.
bool MyFITS::writeLong()
{
    // The new output file
    fitsfile *fptr;
    int status = 0;
    long fpixel = 1;
    long nelements = naxis1*naxis2;
    long *array = new long[nelements];
    for (long i=0; i<nelements; ++i) {
        array[i] = data[i];
    }

    int bitpix = LONG_IMG;
    long naxis = 2;
    long naxes[2] = { naxis1, naxis2 };

    // Overwrite file if it exists
    name = "!"+name;
    fits_create_file(&fptr, name.toUtf8().data(), &status);
    fits_create_img(fptr, bitpix, naxis, naxes, &status);
    fits_write_img(fptr, TLONG, fpixel, nelements, array, &status);
    fits_close_file(fptr, &status);

    delete [] array;

    printerror(status);
    if (status) {
        qDebug() << "QDEBUG::MyFITS::write(): Error writing " << name;
        return false;
    }
    else return true;
}

void MyFITS::stayWithinBounds(long &coord, QString axis)
{
    if (coord < 0) coord = 0;
    if (axis == "x") {
        if (coord >= naxis1) coord = naxis1 - 1;
    }
    else {
        if (coord >= naxis2) coord = naxis2 - 1;
    }
}

// Make sure a vector with xmin xmax ymin ymax stays within the image boundaries
void MyFITS::stayWithinBounds(QVector<long> &vertices)
{
    if (vertices[0] < 0) vertices[0] = 0;
    if (vertices[1] > naxis1-1) vertices[1] = naxis1-1;
    if (vertices[2] < 0) vertices[2] = 0;
    if (vertices[3] > naxis2-1) vertices[3] = naxis2-1;
}

// used in colorimage on a MyFITS which hasn't its data read yet
void MyFITS::loadDataSection(long xmin, long xmax, long ymin, long ymax, QVector<float> &dataSect)
{
    int status = 0;
    fitsfile *fptr = nullptr;
    initFITS(&fptr, &status);

    // DOES NOT WORK. WHY?
    // int naxis[2];
    // fits_get_img_dim(fptr, &naxis, &status);
    //  naxis1 = naxis[0];
    //  naxis2 = naxis[1];

    // DOES WORK
    fits_read_key_lng(fptr, "NAXIS1", &naxis1, NULL, &status);
    fits_read_key_lng(fptr, "NAXIS2", &naxis2, NULL, &status);
    if (status) {
        printerror(status);
        return;
    }

    stayWithinBounds(xmin, "x");
    stayWithinBounds(xmax, "x");
    stayWithinBounds(ymin, "y");
    stayWithinBounds(ymax, "y");

    vector<long> vertex1(2);
    vector<long> vertex2(2);
    vector<long> stride(2);
    vertex1[0] = xmin;
    vertex1[1] = ymin;
    vertex2[0] = xmax;
    vertex2[1] = ymax;
    stride[0]  = 1;
    stride[1]  = 1;

    int nsub = xmax - xmin + 1;
    int msub = ymax - ymin + 1;
    float nullval = 0.;
    int anynull = 0;
    //float *subarray = new float[nsub*msub];
    float subarray[nsub*msub];
    long fpixel[2] = {xmin+1, ymin+1};   // cfitsio starts counting at 1, at least here
    long lpixel[2] = {xmax+1, ymax+1};   // cfitsio starts counting at 1, at least here
    long strides[2] = {stride[0],stride[1]};
    fits_read_subset(fptr, TFLOAT, fpixel, lpixel, strides, &nullval, subarray, &anynull, &status);
    fits_close_file(fptr, &status);

    dataSect.reserve(nsub*msub);
    for (long i=0; i<nsub*msub; ++i) dataSect.push_back(subarray[i]);

    // delete[] subarray;

    if (status) {
        printerror(status);
        return;
    }
}

// used in swarpfilter (using arrays instead of vectors, for performance reasons; unnecessary data copying)
void MyFITS::loadDataSection(long xmin, long xmax, long ymin, long ymax, float *dataSect)
{
    int status = 0;
    fitsfile *fptr = nullptr;
    initFITS(&fptr, &status);

    // DOES NOT WORK. WHY?
    // int naxis[2];
    // fits_get_img_dim(fptr, &naxis, &status);
    //  naxis1 = naxis[0];
    //  naxis2 = naxis[1];

    // DOES WORK
    fits_read_key_lng(fptr, "NAXIS1", &naxis1, NULL, &status);
    fits_read_key_lng(fptr, "NAXIS2", &naxis2, NULL, &status);
    if (status) {
        printerror(status);
        return;
    }

    long xmin_old = xmin;
    long xmax_old = xmax;
    long ymin_old = ymin;
    long ymax_old = ymax;
    stayWithinBounds(xmin, "x");
    stayWithinBounds(xmax, "x");
    stayWithinBounds(ymin, "y");
    stayWithinBounds(ymax, "y");
    if (xmin != xmin_old
            || xmax != xmax_old
            || ymin != ymin_old
            || ymax != ymax_old) {
        qDebug() << "QDEBUG: ERROR: MyFITS::loadDataSection / swarpfilter: image size was modified!";
        return;
    }

    vector<long> vertex1(2);
    vector<long> vertex2(2);
    vector<long> stride(2);
    vertex1[0] = xmin;
    vertex1[1] = ymin;
    vertex2[0] = xmax;
    vertex2[1] = ymax;
    stride[0]  = 1;
    stride[1]  = 1;

    float nullval = 0.;
    int anynull = 0;
    long fpixel[2] = {xmin+1, ymin+1};   // cfitsio starts counting at 1, at least here
    long lpixel[2] = {xmax+1, ymax+1};   // cfitsio starts counting at 1, at least here
    long strides[2] = {stride[0],stride[1]};
    fits_read_subset(fptr, TFLOAT, fpixel, lpixel, strides, &nullval, dataSect, &anynull, &status);
    fits_close_file(fptr, &status);

    if (status) {
        printerror(status);
        return;
    }
}

void MyFITS::printerror(int status)
{
    if (status) {
        CfitsioErrorCodes *errorCodes = new CfitsioErrorCodes(this);
        emit messageAvailable("cfitsio: " + errorCodes->errorKeyMap.value(status), "error");
    }
}
