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
#include "../tools/tools.h"
#include "../tools/cfitsioerrorcodes.h"
#include "../processingStatus/processingStatus.h"
#include "wcs.h"
#include "wcshdr.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QString>


void MyImage::writeImage(QString fileName, QString filter, float exptime, bool addGain)
{
    if (!successProcessing) return;

    if (fileName.isEmpty()) {
        fileName = path+"/"+chipName+processingStatus->statusString+".fits";
    }

    if (addGain) addGainNormalization = true;
    else addGainNormalization = false;

    bool success = write(fileName, dataCurrent, exptime, filter, header);
    if (success) imageOnDrive = true;
    else imageOnDrive = false;

    emit modelUpdateNeeded(chipName);
}

// same as above, just writes the dataTIFF vector instead
void MyImage::writeImageTIFF(QString fileName, QString filter, float exptime, bool addGain)
{
    if (!successProcessing) return;

    if (addGain) addGainNormalization = true;
    else addGainNormalization = false;

    static_cast<void> (write(fileName, dataTIFF, exptime, filter, header));
}

void MyImage::writeImageBackupL1()
{
    if (!successProcessing) return;

    QString fileName = pathBackupL1+"/"+chipName+statusBackupL1+".fits";

    addGainNormalization = true;

    bool success = write(fileName, dataBackupL1, exptime, filter, header);
    if (success) backupL1OnDrive = true;
    else backupL1OnDrive = false;

    // emit modelUpdateNeeded(chipName);
}

void MyImage::writeImageBackupL2()
{
    if (!successProcessing) return;

    QString fileName = pathBackupL2+"/"+chipName+statusBackupL2+".fits";

    addGainNormalization = true;

    bool success = write(fileName, dataBackupL2, exptime, filter, header);
    if (success) backupL2OnDrive = true;
    else backupL2OnDrive = false;

    //    emit modelUpdateNeeded(chipName);
}

void MyImage::writeImageBackupL3()
{
    if (!successProcessing) return;

    QString fileName = pathBackupL3+"/"+chipName+statusBackupL3+".fits";

    addGainNormalization = true;

    bool success = write(fileName, dataBackupL3, exptime, filter, header);
    if (success) backupL3OnDrive = true;
    else backupL3OnDrive = false;

    //    emit modelUpdateNeeded(chipName);
}

void MyImage::writeBackgroundModel()
{
    if (!successProcessing) return;

    mkAbsDir(path + "/SKY_IMAGES");
    QString fileName = path + "/SKY_IMAGES/" + baseName+".sky.fits";

    write(fileName, dataBackground, exptime, filter, header);
}

void MyImage::writeWeight(QString fileName)
{
    if (!successProcessing) return;

    bool success = write(fileName, dataWeight, exptime, filter, header);
    if (success) weightOnDrive = true;
    else weightOnDrive = false;
}

void MyImage::writeWeightSmoothed(QString fileName)
{
    if (!successProcessing) return;

    bool success = write(fileName, dataWeightSmooth, exptime, filter, header);
    if (success) weightOnDrive = true;
    else weightOnDrive = false;
}

void MyImage::writeConstSkyImage(float constValue)
{
    if (!successProcessing) return;

    mkAbsDir(path+"/SKY_IMAGES");

    QString fileName = path+"/SKY_IMAGES/"+baseName+".sky.fits";

    writeConstImage(fileName, constValue, header);
}

void MyImage::writeImageDebayer(bool addGain)
{
    if (!successProcessing) return;

    QString fileName = path+"/"+chipName+processingStatus->statusString+".fits";

    if (addGain) addGainNormalization = true;
    else addGainNormalization = false;

    bool success = write(fileName, dataCurrent, exptime, filter, header);
    if (success) imageOnDrive = true;
    else imageOnDrive = false;

    emit modelUpdateNeeded(chipName);
}

// If the 'headerRef' member is set, the header from that image will be copied.
bool MyImage::write(QString fileName, const QVector<float> &data, const float exptime,
                    const QString filter, const QVector<QString> header)
{
    // The new output file
    fitsfile *fptr;
    int status = 0;
    long fpixel = 1;
    int bitpix = FLOAT_IMG;
    long naxis = 2;
    long naxes[2] = {naxis1, naxis2};
    long nelements = naxis1*naxis2;
    float *array = new float[nelements];
    for (long i=0; i<nelements; ++i) {
        array[i] = data[i];
    }

    // Overwrite file if it exists
    fileName = "!"+fileName;
    fits_create_file(&fptr, fileName.toUtf8().data(), &status);
    fits_create_img(fptr, bitpix, naxis, naxes, &status);
    fits_write_img(fptr, TFLOAT, fpixel, nelements, array, &status);

    // header stuff
    updateHeaderValue("SATURATE", saturationValue, 'e');      // Could be done explicitly every time saturation is changed
    if (!header.isEmpty()) propagateHeader(fptr, header);

    if (exptime >= 0.) {
        fits_write_key_flt(fptr, "EXPTIME", exptime, 6, nullptr, &status);
    }

    if (!filter.isEmpty()) {
        fits_update_key_str(fptr, "FILTER", filter.toUtf8().data(), nullptr, &status);
    }

    if (addGainNormalization) {
        fits_update_key_flt(fptr, "GAINCORR", gainNormalization, 6, nullptr, &status);
    }

    fits_update_key_dbl(fptr, "MJD-OBS", mjdobs, 15, nullptr, &status);

    // BZERO should be 0 after THELI processing. Pixels are scaled by cfitsio already when loading images.
    fits_update_key_flt(fptr, "BZERO", 0.0, 6, nullptr, &status);

    // This image has been processed by THELI
    fits_update_key_lng(fptr, "THELIPRO", 1, "Indicates that this is a THELI FITS file", &status);
    fits_close_file(fptr, &status);

    delete [] array;
    array = nullptr;

    if (status) {
        printCfitsioError(__func__, status);
        successProcessing = false;
        return false;
    }
    else {
        successProcessing = true;
        if (*verbosity > 1) emit messageAvailable(fileName + " : Written to drive.", "image");
        return true;
    }

    // If requested, copy a reference header. This assumes that the image geometries are identical!
    /*
    if (!headerRef.isEmpty()) {
        fitsfile *reference_fptr = nullptr;
        int status_ref = 0;
        fits_open_file(&reference_fptr, headerRef.toUtf8().data(), READONLY, &status_ref);
        fits_copy_header(reference_fptr, fptr, &status_ref);
        fits_close_file(reference_fptr, &status_ref);
        printCfitsioError(__func__, status_ref);
    }
    */
}

void MyImage::writeSegmentation(QString fileName)
{
    if (!successProcessing) return;

    if (*verbosity > 1) emit messageAvailable(chipName + " : Writing segmentation map ("+fileName+") ...", "image");

    fitsfile *fptr;
    int status = 0;
    long fpixel = 1;
    int bitpix = LONG_IMG;
    long naxis = 2;
    long naxes[2] = {naxis1, naxis2};
    long nelements = naxis1*naxis2;
    long *array = new long[nelements];
    for (long i=0; i<nelements; ++i) {
        array[i] = dataSegmentation[i];
    }

    // Overwrite file if it exists
    fileName = "!"+fileName;
    fits_create_file(&fptr, fileName.toUtf8().data(), &status);
    fits_create_img(fptr, bitpix, naxis, naxes, &status);
    fits_write_img(fptr, TLONG, fpixel, nelements, array, &status);
    fits_close_file(fptr, &status);

    delete [] array;
    array = nullptr;

    printCfitsioError(__func__, status);
}

void MyImage::writeObjectMask(QString fileName)
{
    if (!successProcessing) return;

    if (*verbosity > 1) emit messageAvailable(chipName + " : Writing object mask ("+fileName+") ...", "image");

    fitsfile *fptr;
    int status = 0;
    long fpixel = 1;
    int bitpix = LONG_IMG;
    long naxis = 2;
    long naxes[2] = {naxis1, naxis2};
    long nelements = naxis1*naxis2;
    long *array = new long[nelements];
    for (long i=0; i<nelements; ++i) {
        if (objectMask[i]) array[i] = 0;
        else array[i] = 1;
    }

    // Overwrite file if it exists
    fileName = "!"+fileName;
    fits_create_file(&fptr, fileName.toUtf8().data(), &status);
    fits_create_img(fptr, bitpix, naxis, naxes, &status);
    fits_write_img(fptr, TLONG, fpixel, nelements, array, &status);
    fits_close_file(fptr, &status);
    delete [] array;
    array = nullptr;

    printCfitsioError(__func__, status);
}


// If the 'headerRef' member is set, the header from that image will be copied.
void MyImage::writeConstImage(QString fileName, float constValue, const QVector<QString> header)
{
    // The new output file
    fitsfile *fptr;
    int status = 0;
    long fpixel = 1;
    int bitpix = FLOAT_IMG;
    long naxis = 2;
    long naxes[2] = {naxis1, naxis2};
    long nelements = naxis1*naxis2;
    float *array = new float[nelements];
    for (long i=0; i<nelements; ++i) {
        array[i] = constValue;
    }

    // Overwrite file if it exists
    fileName = "!"+fileName;
    fits_create_file(&fptr, fileName.toUtf8().data(), &status);
    fits_create_img(fptr, bitpix, naxis, naxes, &status);
    fits_write_img(fptr, TFLOAT, fpixel, nelements, array, &status);

    // Header stuff
    if (!header.isEmpty()) propagateHeader(fptr, header);

    if (exptime >= 0.) {
        fits_write_key_flt(fptr, "EXPTIME", exptime, 6, nullptr, &status);
    }

    if (!filter.isEmpty()) {
        fits_update_key_str(fptr, "FILTER", filter.toUtf8().data(), nullptr, &status);
    }

    if (addGainNormalization) {
        fits_update_key_flt(fptr, "GAINCORR", gainNormalization, 6, nullptr, &status);
    }

    // BZERO should be 0 after THELI processing. Pixels are scaled by cfitsio already when loading images.
    fits_update_key_flt(fptr, "BZERO", 0.0, 6, nullptr, &status);

    // This image has been processed by THELI
    fits_update_key_lng(fptr, "THELIPRO", 1, "Indicates that this is a THELI FITS file", &status);
    fits_close_file(fptr, &status);

    delete [] array;
    array = nullptr;

    printCfitsioError(__func__, status);
}

/*
// same as write(), but also adds MJDOBS
bool MyImage::writeDebayer(QString fileName, const float exptime, const QString filter,
                           const QVector<QString> header)
{
    // The new output file
    fitsfile *fptr;
    int status = 0;
    long fpixel = 1;
    int bitpix = FLOAT_IMG;
    long naxis = 2;
    long naxes[2] = {naxis1, naxis2};
    long nelements = naxis1*naxis2;
    float *array = new float[nelements];
    for (long i=0; i<nelements; ++i) {
        array[i] = dataCurrent[i];
    }

    // Overwrite file if it exists
    fileName = "!"+fileName;
    fits_create_file(&fptr, fileName.toUtf8().data(), &status);
    fits_create_img(fptr, bitpix, naxis, naxes, &status);
    fits_write_img(fptr, TFLOAT, fpixel, nelements, array, &status);

    // header stuff
    if (!header.isEmpty()) propagateHeader(fptr, header);

    if (exptime >= 0.) {
        fits_write_key_flt(fptr, "EXPTIME", exptime, 6, nullptr, &status);
    }

    if (!filter.isEmpty()) {
        fits_update_key_str(fptr, "FILTER", filter.toUtf8().data(), nullptr, &status);
    }

    if (addGainNormalization) {
        fits_update_key_flt(fptr, "GAINCORR", gainNormalization, 6, nullptr, &status);
    }

    fits_update_key_dbl(fptr, "MJD-OBS", mjdobs, 15, nullptr, &status);

    // BZERO should be 0 after THELI processing. Pixels are scaled by cfitsio already when loading images.
    fits_update_key_flt(fptr, "BZERO", 0.0, 6, nullptr, &status);

    // This image has been processed by THELI
    fits_update_key_lng(fptr, "THELIPRO", 1, "Indicates that this is a THELI FITS file", &status);
    fits_close_file(fptr, &status);

    delete [] array;
    array = nullptr;

    if (status) {
        printCfitsioError(__func__, status);
        successProcessing = false;
        return false;
    }
    else {
        successProcessing = true;
        if (*verbosity > 1) emit messageAvailable(fileName + " : Written to drive.", "image");
        return true;
    }
}
*/
