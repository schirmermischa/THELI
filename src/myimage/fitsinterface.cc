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
#include <omp.h>
#include "../tools/cfitsioerrorcodes.h"
#include "wcs.h"
#include "wcshdr.h"

#include "../functions.h"
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

// Extract the FILTER keyword from a yet unopened FITS file
void MyImage::readFILTER(QString loadFileName)
{
    if (loadFileName.isEmpty()) loadFileName = path + "/" + baseName + ".fits";
    int status = 0;
    fitsfile *fptr = nullptr;
    initFITS(&fptr, loadFileName, &status);
    char filterchar[80];
    fits_read_key_str(fptr, "FILTER", filterchar, NULL, &status);
    fits_close_file(fptr, &status);
    QString filterstring(filterchar);
    printCfitsioError("MyImage::readFILTER()", status);

    filter = filterstring.simplified();
}

// Extract the NAXIS1, NAXIS2, CRPIX1, CRPIX2, SKYVALUE keywords from a yet unopened FITS file for swarpfilter()
bool MyImage::informSwarpfilter(long &naxis1, long &naxis2, double &crpix1, double &crpix2, double &sky, double &fluxscale)
{
    QString loadFileName = path + "/" + name;
    int status = 0;
    fitsfile *fptr = nullptr;
    initFITS(&fptr, loadFileName, &status);
    fits_read_key_dbl(fptr, "CRPIX1", &crpix1, NULL, &status);
    fits_read_key_dbl(fptr, "CRPIX2", &crpix2, NULL, &status);
    fits_read_key_lng(fptr, "NAXIS1", &naxis1, NULL, &status);
    fits_read_key_lng(fptr, "NAXIS2", &naxis2, NULL, &status);
    fits_read_key_dbl(fptr, "SKYVALUE", &sky, NULL, &status);
    fits_read_key_dbl(fptr, "FLXSCALE", &fluxscale, NULL, &status);
    fits_close_file(fptr, &status);
    printCfitsioError("MyImage::informSwarpfilter()", status);
    if (status) return false;
    else return true;
}

void MyImage::initFITS(fitsfile **fptr, QString loadFileName, int *status)
{
    if (loadFileName.isNull() || loadFileName.isEmpty()) {
        *status = 1;
        emit messageAvailable("MyImage::initFITS(): file name empty or not initialized!", "error");
        return;
    }
    // Bypassing a memory leak in cfitsio
    QFile file(loadFileName);
    if (!file.exists()) {
        emit messageAvailable(name+" MyImage::initFITS(): file name empty or not initialized!", "error");
        *status = 104;
        return;
    }
    fits_open_file(fptr, loadFileName.toUtf8().data(), READONLY, status);
    fits_get_num_hdus(*fptr, &numExt, status);
    if (numExt > 1) {
        QMessageBox msgBox;
        msgBox.setText(name+" is a multi-extension FITS file, which is currently not supported.");
        msgBox.exec();
        *status = 1;
    }
}

void MyImage::readHeader(fitsfile **fptr, int *status)
{
    if (*status) return;
    // Read the entire header. This should always work!
    fits_hdr2str(*fptr, 0, NULL, 0, &fullheader, &numHeaderKeys, status);

    fullheaderAllocated = true;
    if (*status) return;

    fullHeaderString = QString::fromUtf8(fullheader);

    // Map the header onto a QVector<QString>
    int cardLength = 80;
    long length = fullHeaderString.length();
    if (length<80) return;
    header.clear();
    for (long i=0; i<=length-cardLength; i+=cardLength) {
        QStringRef subString(&fullHeaderString, i, cardLength);
        QString string = subString.toString();
        header.push_back(string);
    }
}

void MyImage::readData(fitsfile **fptr, int *status)
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
        dataCurrent.resize(nelements);
        for (long i=0; i<nelements; ++i) {
            // if (isinf(buffer[i])) data[i] = 0.;
            // else data[i] = buffer[i];
            dataCurrent[i] = buffer[i];
        }
    }
    dataCurrent.squeeze(); // shed excess memory

    delete [] buffer;
}

void MyImage::readDataWeight(fitsfile **fptr, int *status)
{
    if (*status) return;
    long naxis[2];

    // Get image geometry
    fits_get_img_size(*fptr, 2, naxis, status);

    // Read the data block
    long nax1 = naxis[0];
    long nax2 = naxis[1];
    long nelements = nax1*nax2;
    float *buffer = new float[nelements];
    float nullval = 0.;
    int anynull;
    long fpixel = 1;
    fits_read_img(*fptr, TFLOAT, fpixel, nelements, &nullval, buffer, &anynull, status);

    if (! *status) {
        dataWeight.resize(nelements);
        for (long i=0; i<nelements; ++i) {
            // if (isinf(buffer[i])) data[i] = 0.;
            // else data[i] = buffer[i];
            dataWeight[i] = buffer[i];
        }
    }
    dataWeight.squeeze(); // shed excess memory

    delete [] buffer;
}

bool MyImage::loadData(QString loadFileName)
{
    if (loadFileName.isEmpty()) loadFileName = path + "/" + chipName+processingStatus->statusString+".fits";

//    qDebug() << "loadData():" << path + "/" + chipName+processingStatus->statusString+".fits";

    int status = 0;

    fitsfile *fptr = nullptr;
    initFITS(&fptr, loadFileName, &status);
    readHeader(&fptr, &status);
    readData(&fptr, &status);
    initWCS();
    initTHELIheader(&status);
    checkTHELIheader(&status);
    cornersToRaDec();
    fits_close_file(fptr, &status);

    printCfitsioError("MyImage::loadData()", status);

    if (!status) headerInfoProvided = true;
    else headerInfoProvided = false;

    if (status) return false;
    else return true;
}

// Setup the WCS
void MyImage::initWCS()
{
    if (!successProcessing) return;

    emit setWCSLock(true);          // It appears that not everything in the wcslib is threadsafe
    int nreject;
    int nwcs;
    int check = wcspih(fullheader, numHeaderKeys, 0, 0, &nreject, &nwcs, &wcs);
    if (check > 1) {
        emit messageAvailable("MyImage::initWCS(): " + baseName + ": wcspih() returned" + QString::number(check), "error" );
        emit critical();
        successProcessing = false;
        wcsInit = false;
        emit setWCSLock(false);
        return;
    }
    if (nwcs == 0 || check == 1) {
        // OK state, e.g. for master calibrators which don't have a valid WCS
        if (*verbosity > 2) emit messageAvailable(chipName + " : No WCS representation found", "image");
        wcsInit = false;
        emit setWCSLock(false);
        return;
    }
    int wcsCheck = wcsset(wcs);
    if (wcsCheck > 0) {
        QString wcsError = "";
        if (wcsCheck == 1) wcsError = "Null wcsprm pointer passed";                   // Should be caught by 'if' conditions above
        if (wcsCheck == 2) wcsError = "Memory allocation failed";
        if (wcsCheck == 3) wcsError = "Linear transformation matrix is singular";
        if (wcsCheck == 4) wcsError = "Inconsistent or unrecognized coordinate axis types";
        if (wcsCheck == 5) wcsError = "Invalid parameter value";
        if (wcsCheck == 6) wcsError = "Invalid coordinate transformation parameters";
        if (wcsCheck == 7) wcsError = "Ill-conditioned coordinate transformation parameters";
        emit messageAvailable("MyImage::initWCS(): wcsset() error : " + wcsError, "error");
        emit critical();
        successProcessing = false;
        wcsInit = false;
        emit setWCSLock(false);
        return;
    }
    wcsInit = true;
    plateScale = sqrt(wcs->cd[0] * wcs->cd[0] + wcs->cd[2] * wcs->cd[2]) * 3600.;       // in arcsec
    if (plateScale == 0.) plateScale = 1.0;

    if (*verbosity > 2) {
        emit messageAvailable(chipName + " : RA / DEC = "
                              + QString::number(wcs->crval[0], 'f', 6) + " "
                + QString::number(wcs->crval[1], 'f', 6), "image");
    }
    emit setWCSLock(false);
}

// If header data are needed, but not (yet) the data block.
// E.g. if the GUI is started, and the first task is to download the reference catalog or to update the zero-th order solution
void MyImage::loadHeader(QString loadFileName)
{
    if (headerInfoProvided) return;
    if (loadFileName.isEmpty()) loadFileName = path+"/"+baseName+".fits";
    int status = 0;
    fitsfile *fptr = nullptr;
    initFITS(&fptr, loadFileName, &status);
    readHeader(&fptr, &status);
#pragma omp critical
    {
        initWCS();
    }
    initTHELIheader(&status);
    checkTHELIheader(&status);
    cornersToRaDec();

    radius = sqrt(naxis1*naxis1 + naxis2*naxis2)/2. * plateScale / 3600;         // image radius in degrees. Must be determined after initWCS and initTHELIheader when naxis_i/j are known

    fits_close_file(fptr, &status);

    printCfitsioError("MyImage::loadHeader()", status);

    if (!status) headerInfoProvided = true;
    else headerInfoProvided = false;
}

void MyImage::getMJD()
{
    if (hasMJDread) return;

    fitsfile *fptr = nullptr;

    QString fileName = path+"/"+chipName+processingStatus->statusString+".fits";

    int status = 0;
    fits_open_file(&fptr, fileName.toUtf8().data(), READONLY, &status);
    fits_read_key_dbl(fptr, "MJD-OBS", &mjdobs, NULL, &status);
    fits_close_file(fptr, &status);

    printCfitsioError("getMJD()", status);
    if (status) hasMJDread = false;
    else hasMJDread = true;
}

void MyImage::initTHELIheader(int *status)
{
    if (*status) return;
    for (auto &it : header) {
        extractKeywordLong(it, "NAXIS1", naxis1);
        extractKeywordLong(it, "NAXIS2", naxis2);
        extractKeywordDouble(it, "MJD-OBS", mjdobs);
        extractKeywordString(it, "FILTER", filter);
        extractKeywordString(it, "DATE-OBS", dateobs);
        extractKeywordFloat(it, "EXPTIME", exptime);
        extractKeywordFloat(it, "GAINCORR", gainNormalization);
        extractKeywordFloat(it, "SKYVALUE", skyValue);
        extractKeywordInt(it, "BITPIX", bitpix);
        extractKeywordFloat(it, "SKYVALUE", skyValue);
        extractKeywordFloat(it, "AIRMASS", airmass);
        extractKeywordFloat(it, "FWHM", fwhm);
        extractKeywordFloat(it, "FWHMEST", fwhm_est);
        extractKeywordFloat(it, "GAIN", gain);
        extractKeywordFloat(it, "ELLIP", ellipticity);
        extractKeywordFloat(it, "ELLIPEST", ellipticity_est);
        extractKeywordFloat(it, "RZP", RZP);
    }
    if (mjdobs != 0.0) hasMJDread = true;
    else hasMJDread = false;

    filter = filter.simplified();
    dim = naxis1*naxis2;
    if (skyValue != -1e9) modeDetermined = true;
    else modeDetermined = false;

    metadataTransferred = true;
}

void MyImage::checkTHELIheader(int *status)
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
            && fullHeaderString.contains("GAIN    =")
            && fullHeaderString.contains("OBJECT  =")
            && fullHeaderString.contains("MJD-OBS =")) {
        hasTHELIheader = true;
    }
    else {
        hasTHELIheader = false;
    }
}

void MyImage::extractKeywordDouble(QString card, QString key, double &value)
{
    // Make keys unique (e.g. EXPTIME vs TEXPTIME) by constructing full keyword
    key.resize(8, ' ');
    key.append("=");
    if (card.contains(key)) value = card.split("=")[1].split("/")[0].simplified().remove("'").toDouble();
}

void MyImage::extractKeywordFloat(QString card, QString key, float &value)
{
    key.resize(8, ' ');
    key.append("=");
    if (card.contains(key)) value = card.split("=")[1].split("/")[0].simplified().remove("'").toFloat();
}

void MyImage::extractKeywordLong(QString card, QString key, long &value)
{
    key.resize(8, ' ');
    key.append("=");
    if (card.contains(key)) value = card.split("=")[1].split("/")[0].simplified().remove("'").toLong();
}

void MyImage::extractKeywordInt(QString card, QString key, int &value)
{
    key.resize(8, ' ');
    key.append("=");
    if (card.contains(key)) value = card.split("=")[1].split("/")[0].simplified().remove("'").toInt();
}

void MyImage::extractKeywordString(QString card, QString key, QString &value)
{
    key.resize(8, ' ');
    key.append("=");
    if (card.contains(key)) value = card.split("=")[1].split("/")[0].simplified().remove("'");
}

void MyImage::propagateHeader(fitsfile *fptr, QVector<QString> header)
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


void MyImage::stayWithinBounds(long &coord, QString axis)
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
void MyImage::stayWithinBounds(QVector<long> &vertices)
{
    if (vertices[0] < 0) vertices[0] = 0;
    if (vertices[1] > naxis1-1) vertices[1] = naxis1-1;
    if (vertices[2] < 0) vertices[2] = 0;
    if (vertices[3] > naxis2-1) vertices[3] = naxis2-1;
}

// used in swarpfilter (using arrays instead of vectors, for performance reasons; unnecessary data copying)
void MyImage::loadDataSection(long xmin, long xmax, long ymin, long ymax, float *dataSect)
{
    QString fileName = path + "/" + name;

    int status = 0;
    fitsfile *fptr = nullptr;
    initFITS(&fptr, fileName, &status);
    fits_read_key_lng(fptr, "NAXIS1", &naxis1, NULL, &status);
    fits_read_key_lng(fptr, "NAXIS2", &naxis2, NULL, &status);

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
        emit messageAvailable("MyImage::loadDataSection() / swarpfilter: image size was modified!", "error");
        return;
    }

    float nullval = 0.;
    int anynull = 0;
    long fpixel[2] = {xmin+1, ymin+1};   // cfitsio starts counting at 1, at least here
    long lpixel[2] = {xmax+1, ymax+1};   // cfitsio starts counting at 1, at least here
    long strides[2] = {1, 1};
    fits_read_subset(fptr, TFLOAT, fpixel, lpixel, strides, &nullval, dataSect, &anynull, &status);
    fits_close_file(fptr, &status);

    printCfitsioError("MyImage::stayWithinBounds()", status);
}

