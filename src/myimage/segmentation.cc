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

#include "instrumentdata.h"
#include "../functions.h"
#include "../tools/detectedobject.h"
#include "myimage.h"

#include <QDebug>
#include <QMessageBox>
#include <QQueue>

void MyImage::resetObjectMasking()
{
    // The background will always be the same, hence let's leave it in place
    // backgroundModelDone = false;
    // dataBackground.clear();
    // dataBackground.squeeze();
    segmentationDone = false;
    maskExpansionDone = false;
    objectMaskDone = false;
    objectMaskDonePass1 = false;
    objectMaskDonePass2 = false;
    for (auto object : objectList) {
        object->pixels_flux.clear();
        object->pixels_x.clear();
        object->pixels_y.clear();
        object->pixels_flux.squeeze();
        object->pixels_x.squeeze();
        object->pixels_y.squeeze();
    }
    objectList.clear();
    objectMask.clear();
    objectMask.squeeze();
}

void MyImage::updateSaturation(QString saturation)
{
    if (!saturation.isEmpty()) {
        saturationValue = saturation.toFloat();
    }
}

void MyImage::segmentImage(QString DTstring, QString DMINstring, bool convolution, bool writeSegImage)
{
    if (*verbosity > 1) emit messageAvailable(chipName + " : Detecting objects ... ", "image");

    // Start fresh
    resetObjectMasking();

    if (!successProcessing) return;
    if (DTstring.isEmpty() || DMINstring.isEmpty()) return;

    // Update the rms Value if it hasn't been determined yet
    // This is a fallback, because we get it through background modeling already at an earlier step.
    if (skySigma < 0.) {
        emit messageAvailable("MyImage::segmentImage(): sky noise not yet estimated, re-computing", "warning");
        emit critical();
        skySigma = modeMask(dataCurrent, "stable", QVector<bool>())[1];
    }

    // Noise clipping
    // if this doesn't work, then make controller a member of each myimage
    emit setMemoryLock(true);
    dataSegmentation.resize(naxis1*naxis2);
    dataMeasure.clear();   // Must clear (if we run the detection twice, e.g. for sky modeling)
    dataMeasure.reserve(naxis1*naxis2);
    emit setMemoryLock(false);

    QList<long> allObjectPixelIndices;
    allObjectPixelIndices.reserve(naxis1*naxis2/2); // Avoid initial memory reallocations; assume half the pixels belong to objects

    // Noise filter (for detection only)
    QVector<float> dataConv;
    if (convolution) dataConv = directConvolve(dataCurrent);
    else dataConv = dataCurrent;

    float DT = DTstring.toFloat();
    float threshold = DT*skySigma;
    float maxWeight = 1.0;
    // Adapt threshold concerning the weight, if available
    if (weightInMemory) {
        maxWeight = maxVec_T(dataWeight); // Could sort a sub-sample and use the 90% highest value or sth like that, more stable
    }
    long i=0;
    for (auto &pixel: dataCurrent) {
        // subtract background model
        float dorig = pixel - dataBackground[i];
        float dconv = dataConv[i] - dataBackground[i];
        dataMeasure.append(dorig);
        // Initialize objects in the segmentation map with a negative value (still unprocessed)
        // WARNING: Using the globalMask is essential, otherwise the floodfill alg seems to run forever. Really?
        // With global mask
        if (globalMaskAvailable) {
            if (!weightInMemory) {
                if (dconv > threshold && !globalMask[i]) {
                    dataSegmentation[i] = -1;
                    allObjectPixelIndices.append(i);
                }
            }
            else {
                float rescale = sqrt(maxWeight/dataWeight[i]);
                if (dataWeight[i] > 0. && !globalMask[i] && dconv > threshold*rescale) {
                    dataSegmentation[i] = -1;
                    allObjectPixelIndices.append(i);
                }
            }
        }
        // Without global mask
        else {
            if (!weightInMemory) {
                if (dconv > threshold) {
                    dataSegmentation[i] = -1;
                    allObjectPixelIndices.append(i);
                }
            }
            else {
                float rescale = sqrt(maxWeight/dataWeight[i]);
                if (dataWeight[i] > 0. && dconv > threshold*rescale) {
                    dataSegmentation[i] = -1;
                    allObjectPixelIndices.append(i);
                }
            }
        }
        ++i;
    }

    if (allObjectPixelIndices.isEmpty()) {
        emit messageAvailable(chipName + " : No objects detected!" , "error");
        emit critical();
        successProcessing = false;
        return;
    }

    // write the image in which we detect sources
    //    MyFITS out1(path + "/" + baseName+".detect.fits", naxis1, naxis2, dataMeasure);
    //    out1.write("");
    //    writeSegmentation(path + "/" + baseName+".seg1.fits");

    // Create segmentation map
    objectList.clear();
    bool scanning = true;
    long startindex = allObjectPixelIndices[0];
    long DMIN = DMINstring.toLong();
    QPoint startPoint(startindex % naxis1, startindex / naxis1);
    long objID = 1; // Number of the first object; we start counting at 1 not 0

    while (scanning) {
        // Flood fill the current object belonging to that pixel, and update dataSegmentation and allObjectPixelIndices
        floodFill(startPoint, allObjectPixelIndices, objID, dataSegmentation, DMIN);
        // Retrieve next non-zero pixel index from 'objectPixelIndices'
        if (!allObjectPixelIndices.isEmpty()) {
            long nextIndex = allObjectPixelIndices.first();
            startPoint.setX(nextIndex % naxis1);
            startPoint.setY(nextIndex / naxis1);
        }
        else {
            scanning = false;
        }
    }

    segmentationDone = true;

    // Lastly, compute object parameters (which we need for masking)
    if (*verbosity > 1) emit messageAvailable(chipName + " : Measuring object parameters ... ", "image");

    // #pragma omp parallel for num_threads(maxCPU)
    for (long i=0; i<objectList.length(); ++i) {
        objectList[i]->apertures = apertures;
        objectList[i]->computeObjectParams();
        objectList[i]->remove();
    }

    if (*verbosity > 1) emit messageAvailable(chipName + " : " + QString::number(objectList.length()) + " objects detected.", "image");

    segmentationDone = true;
    if (writeSegImage) writeSegmentation(path + "/" + baseName+".seg.fits");
}

// getting a rough first estimate ion image quality; refined after astrometric solution
void MyImage::calcMedianSeeingEllipticity()
{
    QVector<double> fwhmVec;
    QVector<double> ellipticityVec;
    fwhmVec.reserve(objectList.length());
    ellipticityVec.reserve(objectList.length());
    for (auto &object : objectList) {
        if (object->FLAGS == 0) {
            fwhmVec.append(float(object->FWHM));
            ellipticityVec.append(float(object->ELLIPTICITY));
        }
    }
    fwhm_est = straightMedianInline(fwhmVec) * plateScale;
    ellipticity_est = straightMedianInline(ellipticityVec);
    updateHeaderValueInFITS("FWHMEST", QString::number(fwhm_est, 'f', 2));
    updateHeaderValueInFITS("ELLIPEST", QString::number(ellipticity_est, 'f', 3));
}

// Flood fill an individual object with the objectID and calculate object area
void MyImage::floodFill(const QPoint startPoint, QList<long> &allObjectPixelIndices, long &objID, QVector<long> &dataSegmentation, const long DMIN)
{
    if (!insideImage(startPoint)) return;

    // Did we process this point already? If so, remove it from the list of indices
    long index = startPoint.x() + naxis1*startPoint.y();
    if (dataSegmentation[index] != -1) {
        allObjectPixelIndices.pop_front();
        return;
    }

    // temporary storage to mark pixels for deletion (if area less than DMIN)
    QList<long> currentObjectPixels;
    currentObjectPixels.reserve(10000);

    QQueue<QPoint> pointQueue;
    pointQueue.enqueue(startPoint);

    // This is the actual flood filling
    long area = 0;
    while (!pointQueue.isEmpty()) {
        QPoint p = pointQueue.first();
        pointQueue.pop_front();
        if (insideImage(p)) {
            if (dataSegmentation[p.x()+naxis1*p.y()] == -1) {
                dataSegmentation[p.x()+naxis1*p.y()] = objID;
                // queue the edges
                pointQueue.enqueue(QPoint(p.x() + 1, p.y()));
                pointQueue.enqueue(QPoint(p.x() - 1, p.y()));
                pointQueue.enqueue(QPoint(p.x(), p.y() + 1));
                pointQueue.enqueue(QPoint(p.x(), p.y() - 1));
                // queue the corners (8-connectivity)
                pointQueue.enqueue(QPoint(p.x()-1, p.y()-1));
                pointQueue.enqueue(QPoint(p.x()+1, p.y()-1));
                pointQueue.enqueue(QPoint(p.x()-1, p.y()+1));
                pointQueue.enqueue(QPoint(p.x()+1, p.y()+1));
                currentObjectPixels.push_back(p.x()+naxis1*p.y());
            }
        }
    }

    area = currentObjectPixels.length();
    // remove object from segmentation map if too small
    if (area < DMIN) {
        for (auto &pixel: currentObjectPixels) {
            dataSegmentation[pixel] = 0;
        }
        return;
    }

    // Initialize the object and add it to the list of detected sources for this image
    DetectedObject *newObject = new DetectedObject(currentObjectPixels, dataMeasure, dataBackground, dataWeight,
                                                   globalMask, weightInMemory, naxis1, naxis2, ++objID,
                                                   saturationValue, gain, *wcs);
    newObject->globalMaskAvailable = globalMaskAvailable;
    objectList.append(newObject);

    // Remove the current pixel we just processed,
    // and potentially a few more if they happen to be at the beginning of the list
    while (!allObjectPixelIndices.isEmpty()
           && !currentObjectPixels.isEmpty()
           && allObjectPixelIndices.first() == currentObjectPixels.first()) {
        allObjectPixelIndices.pop_front();
        currentObjectPixels.pop_front();
    }
}

bool MyImage::insideImage(QPoint p)
{
    if (p.rx() >= 0
            && p.rx() < naxis1
            && p.ry() >= 0
            && p.ry() < naxis2) return true;
    else return false;
}

// convolve with a general purpose noise filter
QVector<float> MyImage::directConvolve(QVector<float> &data)
{
    float kernel[] = { 1./16., 2./16., 1./16.,
                       2./16., 4./16., 2./16.,
                       1./16., 2./16., 1./16.};

    long n = naxis1;
    long m = naxis2;
    QVector<float> dataConv(n*m);

    // direct convolution. We ignore the border
    long s = 1; // half the kernel size - 1
    for (long j=1; j<naxis2-1; ++j) {
        for (long i=1; i<naxis1-1; ++i) {
            for (long ii=-s; ii<=s; ++ii) {
                for (long jj=-s; jj<=s; ++jj) {
                    dataConv[i+n*j] += data[i+ii + n*(j+jj)] * kernel[ii+s + (2*s+1)*(jj+s)];
                }
            }
        }
    }

    return dataConv;
}

// Transfer the detections to the object mask
void MyImage::transferObjectsToMask()
{
    if (!successProcessing) return;
    objectMask.fill(false, naxis1*naxis2);

    if (objectList.isEmpty()) return;

    long i=0;
    for (auto &segment : dataSegmentation) {
        if (segment > 0.) objectMask[i] = true;
        ++i;
    }
    objectMaskDone = true;
}

void MyImage::estimateMatchingTolerance()
{
    QVector<float> sizes;
    for (auto &object : objectList) {
        if (object->FLAGS == 0) sizes.append(object->FLUX_RADIUS);
    }

    if (sizes.isEmpty()) {
        matchingTolerance = 5.*plateScale/3600.;  // 5 pixel
        if (*verbosity > 2) emit messageAvailable(chipName + " : IQ matching tolerance defaulted to "+QString::number(5.*plateScale, 'f', 1)+" arcsec (5 pixel)", "image");
        emit warning();
    }
    else {
        float seeing_image = 2. * meanMask_T(sizes);
        matchingTolerance = seeing_image * plateScale / 3600.;
        if (*verbosity > 2) emit messageAvailable(chipName + " : IQ matching tolerance = "+QString::number(matchingTolerance*3600, 'f', 1)+" arcsec", "image");
    }
}

// Unused
QVector<double> MyImage::collectObjectParameter(QString paramName)
{
    QVector<double> param;
    param.reserve(objectList.length());

    if (paramName == "RA") {
        for (auto &object : objectList) {
            param << object->ALPHA_J2000;
        }
    }
    if (paramName == "DEC") {
        for (auto &object : objectList) {
            param << object->DELTA_J2000;
        }
    }
    if (paramName == "FWHM") {
        for (auto &object : objectList) {
            param << object->FLUX_RADIUS;
        }
    }
    if (paramName == "ELLIPTICITY") {
        for (auto &object : objectList) {
            param << object->ELLIPTICITY;
        }
    }

    return param;
}

void MyImage::collectSeeingParameters(QVector<QVector<double>> &outputParams, QVector<double> &outputMag, int goodChip)
{
    QVector<double> param;
    param.reserve(4);
    outputParams.clear();     // Must clear because each image will fill this data repeatedly
    outputMag.clear();

    // Try memory access in MyImage
    if (!objectList.isEmpty()) {
        outputParams.reserve(objectList.length());
        outputMag.reserve(objectList.length());
        for (auto &object : objectList) {
            // Must recalculate RA and DEC after astrometry
            double raNew;
            double decNew;
            xy2sky(object->XWIN, object->YWIN, raNew, decNew);
            // must pass dec first for tools::match2D() method
            if (object->FLAGS == 0) {
                param << decNew << raNew << object->FWHM << object->ELLIPTICITY;
                outputParams << param;
                outputMag << object->MAG_AUTO;
                param.clear();
            }
        }
        return;
    }

    // Not present (if GUI launched at this position, or sextractor was used to compute catalogs)
    // Try external catalog

    else {
        QString catalogName = "cat/" + rootName+".scamp";
        QString fullCatalogName = path + "/" + catalogName;
        QFile catalog(fullCatalogName);
        if (!catalog.exists()) {
            emit messageAvailable(chipName + " : Could not find catalog for seeing measurement:<br>" + catalogName, "warning");
            emit warning();
        }

        int status = 0;
        fitsfile *fptr;
        fits_open_file(&fptr, fullCatalogName.toUtf8().data(), READONLY, &status);
        // LDAC_OBJECTS tables are found in extensions 3, 5, 7, ..., internally referred to as 2, 4, 6, ...
        int hduType = 0;
        fits_movabs_hdu(fptr, 2*(goodChip+1)+1, &hduType, &status);
        // fits_movnam_hdu(fptr, BINARY_TBL, tblname, extver, &status);
        long nrows = 0;
        int xwinColNum = -1;
        int ywinColNum = -1;
        int alphaColNum = -1;
        int deltaColNum = -1;
        int fwhmColNum = -1;
        int ellColNum = -1;
        int magColNum = -1;
        int flagColNum = -1;
        fits_get_num_rows(fptr, &nrows, &status);
        char xwinName[100] = "XWIN_IMAGE";
        char ywinName[100] = "YWIN_IMAGE";
        char alphaName[100] = "ALPHA_J2000";
        char deltaName[100] = "DELTA_J2000";
        char fwhmName[100] = "FWHM_IMAGE";
        char ellName[100] = "ELLIPTICITY";
        char magName[100] = "MAG_AUTO";
        char flagName[100] = "FLAGS";
        fits_get_colnum(fptr, CASESEN, xwinName, &xwinColNum, &status);
        fits_get_colnum(fptr, CASESEN, ywinName, &ywinColNum, &status);
        fits_get_colnum(fptr, CASESEN, alphaName, &alphaColNum, &status);
        fits_get_colnum(fptr, CASESEN, deltaName, &deltaColNum, &status);
        fits_get_colnum(fptr, CASESEN, fwhmName, &fwhmColNum, &status);
        fits_get_colnum(fptr, CASESEN, ellName, &ellColNum, &status);
        fits_get_colnum(fptr, CASESEN, magName, &magColNum, &status);
        fits_get_colnum(fptr, CASESEN, flagName, &flagColNum, &status);
        int firstrow = 1;
        int firstelem = 1;
        int anynul = 0;
        double *xwin = new double[nrows];
        double *ywin = new double[nrows];
        double *alpha = new double[nrows];
        double *delta = new double[nrows];
        float *fwhm = new float[nrows];
        float *ell = new float[nrows];
        float *mag = new float[nrows];
        int *flag = new int[nrows];

        QString missing = "";
        fits_read_col(fptr, TDOUBLE, xwinColNum, firstrow, firstelem, nrows, NULL, xwin, &anynul, &status);
        if (status != 0) missing.append("XWIN_IMAGE, ");
        status = 0;
        fits_read_col(fptr, TDOUBLE, ywinColNum, firstrow, firstelem, nrows, NULL, ywin, &anynul, &status);
        if (status != 0) missing.append("YWIN_IMAGE, ");
        status = 0;
        fits_read_col(fptr, TDOUBLE, alphaColNum, firstrow, firstelem, nrows, NULL, alpha, &anynul, &status);
        if (status != 0) missing.append("ALPHA_J2000, ");
        status = 0;
        fits_read_col(fptr, TDOUBLE, deltaColNum, firstrow, firstelem, nrows, NULL, delta, &anynul, &status);
        if (status != 0) missing.append("DELTA_J2000, ");
        status = 0;
        fits_read_col(fptr, TFLOAT, fwhmColNum, firstrow, firstelem, nrows, NULL, fwhm, &anynul, &status);
        if (status != 0) missing.append("FWHM_IMAGE, ");
        status = 0;
        fits_read_col(fptr, TFLOAT, ellColNum, firstrow, firstelem, nrows, NULL, ell, &anynul, &status);
        if (status != 0) missing.append("ELLIPTICITY, ");
        status = 0;
        fits_read_col(fptr, TFLOAT, magColNum, firstrow, firstelem, nrows, NULL, mag, &anynul, &status);
        if (status != 0) missing.append("MAG_AUTO, ");
        status = 0;
        fits_read_col(fptr, TINT, flagColNum, firstrow, firstelem, nrows, NULL, flag, &anynul, &status);
        if (status != 0) missing.append("FLAGS, ");
        status = 0;

        fits_close_file(fptr, &status);

        if (!missing.isEmpty()) {
            emit messageAvailable(chipName + " : Could not find " + missing + " in " + catalogName, "warning");
        }

        outputParams.reserve(nrows);
        outputMag.reserve(nrows);
        for (long i=0; i<nrows; ++i) {
            // Must recalculate RA and DEC after astrometry
            double raNew;
            double decNew;
            xy2sky(xwin[i], ywin[i], raNew, decNew);
            // must pass dec first for tools::match2D() method
            // Only clean detections wanted!
            if (flag[i] == 0) {
                param << decNew << raNew << fwhm[i] << ell[i];
                outputParams << param;
                outputMag << mag[i];
                param.clear();
            }
        }
    }
}

void MyImage::maskExpand(QString expFactor, bool writeObjectmaskImage)
{
    if (!successProcessing) return;
    if (maskExpansionDone) return;      // Don't redo the mask expansion (when calculating a dynamic model, revisiting the same image)
    if (expFactor.isEmpty()) {
        if (writeObjectmaskImage) writeObjectMask(path + "/" + baseName+".mask.fits");
        return;
    }

    float factor = expFactor.toFloat();
    if (factor <= 1.0) {
        emit messageAvailable(chipName + " : Mask expansion factor less than 1.0; ignored ...", "warning");
        emit warning();
        return;
    }

    if (*verbosity > 1) emit messageAvailable(chipName + " : Expanding object mask ...", "image");

    factor *= 3.; // We need an extra factor of 3 to encompass the outer isophote

    long n = naxis1;
    long m = naxis2;
    // Loop over all objects
    for (auto &object : objectList) {
        // Do not mask expand extremely small objects (hot pixels etc)
        if (object->B < 1.0) continue;
        // Do not mask expand very large and extremely elongated objects (bad columns, saturation spikes)
        if (object->ELLIPTICITY > 0.9 && object->A > 50) continue;
        float oX = object->X;
        float oY = object->Y;
        float oAf = object->A * factor;
        long imin = (oX - 1.5*oAf > 0) ? int(oX - 1.5*oAf) : 0;
        long imax = (oX + 1.5*oAf < n) ? int(oX + 1.5*oAf) : n-1;
        long jmin = (oY - 1.5*oAf > 0) ? int(oY - 1.5*oAf) : 0;
        long jmax = (oY + 1.5*oAf < m) ? int(oY + 1.5*oAf) : m-1;
        // Loop over the rectangular subset of pixels that encompass the object
        for (long j=jmin; j<=jmax; ++j) {
            float dy = oY - j;
            for (long i=imin; i<=imax; ++i) {
                float dx = oX - i;
                if (object->CXX*dx*dx
                        + object->CYY*dy*dy
                        + object->CXY*dx*dy <= oAf*oAf) {
                    objectMask[i+naxis1*j] = true;
                }
            }
        }
    }

    maskExpansionDone = true;

    if (writeObjectmaskImage) writeObjectMask(path + "/" + baseName+".mask.fits");
}

void MyImage::writeSegmentation(QString fileName)
{
    if (!successProcessing) return;

    if (*verbosity > 1) emit messageAvailable(chipName + " : Writing segmentation map to drive ("+fileName+") ...", "image");

    name = fileName;
    // The new output file
    fitsfile *fptr;
    int status = 0;
    long fpixel = 1;
    long nelements = naxis1*naxis2;
    long *array = new long[nelements];
    for (long i=0; i<nelements; ++i) {
        array[i] = dataSegmentation[i];
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

    printCfitsioError("MyImage::writeSegmentation()", status);
}

void MyImage::writeObjectMask(QString fileName)
{
    if (!successProcessing) return;

    if (*verbosity > 1) emit messageAvailable(chipName + " : Writing object mask to drive ("+fileName+") ...", "image");

    name = fileName;
    // The new output file
    fitsfile *fptr;
    int status = 0;
    long fpixel = 1;
    long nelements = naxis1*naxis2;
    long *array = new long[nelements];
    for (long i=0; i<nelements; ++i) {
        if (objectMask[i]) array[i] = 0;
        else array[i] = 1;
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

    printCfitsioError("MyImage::writeObjectMask()", status);
}

void MyImage::releaseAllDetectionMemory()
{
    for (auto &object : objectList) {
        delete object;
    }
    objectList.clear();

    releaseDetectionPixelMemory();
}

void MyImage::releaseDetectionPixelMemory()
{
    emit setMemoryLock(true);
    dataSegmentation.clear();
    dataSegmentation.squeeze();
    dataMeasure.clear();
    dataMeasure.squeeze();
    objectMask.clear();
    objectMask.squeeze();
    objectMaskDone = false;
    objectMaskDonePass1 = false;
    objectMaskDonePass2 = false;
    segmentationDone = false;
    emit setMemoryLock(false);
}

void MyImage::writeCatalog(QString minFWHM_string, QString maxFlag_string)
{
    QString outpath = path+"/cat/iview/";
    QDir outdir(outpath);
    if (!outdir.exists()) outdir.mkpath(outpath);

    float minFWHM = minFWHM_string.toFloat();
    float maxFlag = maxFlag_string.toFloat();
    if (maxFlag_string.isEmpty()) maxFlag = 100;

    // Write iview catalog
    QFile file(path+"/cat/iview/"+chipName+".iview");
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream outputStream(&file);
        for (auto &object : objectList) {
            outputStream.setRealNumberPrecision(9);
            if (3.*object->AWIN >= minFWHM && object->FLAGS <= maxFlag && object->FLUX_AUTO > 0.) {
                // MUST APPLY ORIGIN OFFSET CORRECTION (+1), because calculations were done starting counting at 0 (in FITS files we start at 1)
                outputStream << object->XWIN + 1. << " "
                             << object->YWIN + 1. << " "
                             << object->AWIN << " "
                             << object->BWIN << " "
                             << object->THETAWIN << "\n";
            }
        }
        file.close();
    }

    // Write anet catalog
    char x[100] = "X";
    char y[100] = "Y";
    char mag[100] = "MAG";
    char *ttype[3] = {x, y, mag};
    char tf1[10] = "1D";
    char tf2[10] = "1D";
    char tf3[10] = "1E";
    char *tform[3] = {tf1, tf2, tf3};

    long numSources = objectList.length();
    long numSourcesRetained = 0.;
    for (long i=0; i<numSources; ++i) {
        if (3.*objectList[i]->AWIN >= minFWHM && objectList[i]->FLAGS <= maxFlag && objectList[i]->FLUX_AUTO > 0.) {
            ++numSourcesRetained;
        }
    }

    long nrows = numSourcesRetained;
    float x_arr[nrows];
    float y_arr[nrows];
    float mag_arr[nrows];
    long k = 0;
    for (long i=0; i<numSources; ++i) {
        if (3.*objectList[i]->AWIN >= minFWHM && objectList[i]->FLAGS <= maxFlag && objectList[i]->FLUX_AUTO > 0.) {
            // MUST APPLY ORIGIN OFFSET CORRECTION (+1), because calculations were done starting counting at 0 (in FITS files we start at 1)
            x_arr[k] = objectList[i]->XWIN + 1.;
            y_arr[k] = objectList[i]->YWIN + 1.;
            mag_arr[k] = objectList[i]->MAG_AUTO;
            ++k;
        }
    }
    int status = 0;
    fitsfile *fptr;
    long firstrow  = 1;
    long firstelem = 1;
    int tfields = 3;
    QString filename = path+"/cat/"+chipName+".anet";
    filename = "!"+filename;
    fits_create_file(&fptr, filename.toUtf8().data(), &status);
    fits_create_tbl(fptr, BINARY_TBL, nrows, tfields, ttype, tform, nullptr, "OBJECTS", &status);
    fits_write_col(fptr, TDOUBLE, 1, firstrow, firstelem, nrows, x_arr, &status);
    fits_write_col(fptr, TDOUBLE, 2, firstrow, firstelem, nrows, y_arr, &status);
    fits_write_col(fptr, TFLOAT, 3, firstrow, firstelem, nrows, mag_arr, &status);
    fits_close_file(fptr, &status);

    printCfitsioError("MyImage::writeCatalog()", status);
}

void MyImage::makeXcorrData()
{
    dataXcorr = dataCurrent;

    // Xcorrelation works best if we mask all unrelevant pixels
    long i = 0;
    for (auto &pixel: dataXcorr) {
        if (globalMaskAvailable && globalMask[i]) pixel = 0.;
        if (dataSegmentation[i] == -1) pixel = 0.;
        if (weightInMemory && dataWeight[i] == 0.) pixel = 0.;
        ++i;
    }
}

// Appends a new binary table to an already opened FITS file (handled by Controller class)
void MyImage::appendToScampCatalogInternal(fitsfile *fptr, QString minFWHM_string, QString maxFlag_string)
{
    float minFWHM = minFWHM_string.toFloat();
    float maxFlag = maxFlag_string.toFloat();
    if (maxFlag_string.isEmpty()) maxFlag = 100;
    if (minFWHM_string.isEmpty()) minFWHM = 0.01;

    // STEP 1: LDAC_IMHEAD table, containing the FITS header
    int status = 0;
    long firstrow  = 1;
    long firstelem = 1;
    int tfields = 1;
    long nrows = 1;
    char name[100] = "Field Header Card";
    char *ttype1[1] = {name};
    // Awkward C-stuff. This is the only way I could get this to work.
    // If the tf0 string is built differently, if strcmp says they are identical, the TTYPE1 keyword is not written properly.
    // And it cannot be udpated properly with fits_update_key() either
    long headerLength = strlen(fullheader);
    char tf0[100];
    sprintf(tf0, "%ldA", headerLength);
    char *tform1[1] = {tf0};
    const char *headerstring[1];
    headerstring[0] = fullheader;
    fits_create_tbl(fptr, BINARY_TBL, nrows, tfields, ttype1, tform1, nullptr, "LDAC_IMHEAD", &status);
    fits_write_col(fptr, TSTRING, 1, firstrow, firstelem, nrows, headerstring, &status);

    // STEP 2: LDAC_OBJECTS table
    char xwin[100] = "XWIN_IMAGE";
    char ywin[100] = "YWIN_IMAGE";
    char erra[100] = "ERRAWIN_IMAGE";
    char errb[100] = "ERRBWIN_IMAGE";
    char errt[100] = "ERRTHETAWIN_IMAGE";
    char flux[100] = "FLUX_AUTO";
    char fluxerr[100] = "FLUXERR_AUTO";
    char flags[100] = "FLAGS";
    char alpha[100] = "ALPHA_J2000";
    char delta[100] = "DELTA_J2000";
    char fwhm[100] = "FWHM_IMAGE";
    char mag[100] = "MAG_AUTO";
    char ell[100] = "ELLIPTICITY";
    char *ttype2[13] = {xwin, ywin, erra, errb, errt, flux, fluxerr, flags, alpha, delta, fwhm, mag, ell};

    char tf1[10] = "1E";
    char tf2[10] = "1E";
    char tf3[10] = "1E";
    char tf4[10] = "1E";
    char tf5[10] = "1E";
    char tf6[10] = "1E";
    char tf7[10] = "1E";
    char tf8[10] = "1I";
    char tf9[10] = "1D";
    char tf10[10] = "1D";
    char tf11[10] = "1E";
    char tf12[10] = "1E";
    char tf13[10] = "1E";
    char *tform2[13] = {tf1, tf2, tf3, tf4, tf5, tf6, tf7, tf8, tf9, tf10, tf11, tf12, tf13};

    long numSources = objectList.length();
    long numSourcesRetained = 0;
    for (long i=0; i<numSources; ++i) {
        if (3.*objectList[i]->AWIN >= minFWHM && objectList[i]->FLAGS <= maxFlag && objectList[i]->FLUX_AUTO > 0.) {
            ++numSourcesRetained;
        }
    }

    nrows = numSourcesRetained;  // one row per source
    float xwin_arr[nrows];
    float ywin_arr[nrows];
    float erra_arr[nrows];
    float errb_arr[nrows];
    float errt_arr[nrows];
    float flux_arr[nrows];
    float fluxerr_arr[nrows];
    short flags_arr[nrows];
    double alpha_arr[nrows];
    double delta_arr[nrows];
    float fwhm_arr[nrows];
    float mag_arr[nrows];
    float ell_arr[nrows];

    long k = 0;
    for (long i=0; i<numSources; ++i) {
        if (3.*objectList[i]->AWIN >= minFWHM && objectList[i]->FLAGS <= maxFlag && objectList[i]->FLUX_AUTO > 0.) {
            // MUST APPLY ORIGIN OFFSET CORRECTION (+1), because calculations were done starting counting at 0 (in FITS files we start at 1)
            xwin_arr[k] = objectList[i]->XWIN + 1.;
            ywin_arr[k] = objectList[i]->YWIN + 1.;
            erra_arr[k] = objectList[i]->ERRAWIN;
            errb_arr[k] = objectList[i]->ERRBWIN;
            errt_arr[k] = objectList[i]->ERRTHETAWIN;
            flux_arr[k] = objectList[i]->FLUX_AUTO;
            fluxerr_arr[k] = sqrt(objectList[i]->FLUX_AUTO);
            flags_arr[k] = objectList[i]->FLAGS;
            // The following are not needed by scamp. They are just for completeness.
            alpha_arr[k] = objectList[i]->ALPHA_J2000;
            delta_arr[k] = objectList[i]->DELTA_J2000;
            fwhm_arr[k] = objectList[i]->FWHM;
            mag_arr[k] = objectList[i]->MAG_AUTO;
            ell_arr[k] = objectList[i]->ELLIPTICITY;
            ++k;
        }
    }

    firstrow  = 1;
    firstelem = 1;
    tfields = 13;
    fits_create_tbl(fptr, BINARY_TBL, nrows, tfields, ttype2, tform2, nullptr, "LDAC_OBJECTS", &status);
    fits_write_col(fptr, TFLOAT, 1, firstrow, firstelem, nrows, xwin_arr, &status);
    fits_write_col(fptr, TFLOAT, 2, firstrow, firstelem, nrows, ywin_arr, &status);
    fits_write_col(fptr, TFLOAT, 3, firstrow, firstelem, nrows, erra_arr, &status);
    fits_write_col(fptr, TFLOAT, 4, firstrow, firstelem, nrows, errb_arr, &status);
    fits_write_col(fptr, TFLOAT, 5, firstrow, firstelem, nrows, errt_arr, &status);
    fits_write_col(fptr, TFLOAT, 6, firstrow, firstelem, nrows, flux_arr, &status);
    fits_write_col(fptr, TFLOAT, 7, firstrow, firstelem, nrows, fluxerr_arr, &status);
    fits_write_col(fptr, TSHORT, 8, firstrow, firstelem, nrows, flags_arr, &status);
    fits_write_col(fptr, TDOUBLE, 9, firstrow, firstelem, nrows, alpha_arr, &status);
    fits_write_col(fptr, TDOUBLE, 10, firstrow, firstelem, nrows, delta_arr, &status);
    fits_write_col(fptr, TFLOAT, 11, firstrow, firstelem, nrows, fwhm_arr, &status);
    fits_write_col(fptr, TFLOAT, 12, firstrow, firstelem, nrows, mag_arr, &status);
    fits_write_col(fptr, TFLOAT, 13, firstrow, firstelem, nrows, ell_arr, &status);
    //    fits_write_col(fptr, TSHORT, 8, firstrow, firstelem, nrows, fieldpos, &status);

    // Color-coding output lines
    QString detStatus = "";
    QString level = "image";
    if (nrows<10 && nrows>3) {
        detStatus = " (low source count)";
        level = "warning";
    }
    if (nrows<=3) {
        detStatus = " (very low source count)";
        level = "stop";
    }

    messageAvailable(rootName + " : " + QString::number(nrows) + " sources after filtering..." +detStatus, level);

    printCfitsioError("MyImage::appendToScampCatalogInternal())", status);
}
