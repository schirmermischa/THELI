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
#include "../tools/polygon.h"
#include "../processingInternal/data.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QString>

void MyImage::readWeight()
{
    dataWeight_deletable = false;
    // Leave if image is already loaded
    if (weightInMemory) return;
    else {
        int status = 0;
        fitsfile *fptr = nullptr;
        QString fileName = weightPath+"/"+weightName+".fits";
        initFITS(&fptr, fileName, &status);
        readDataWeight(&fptr, &status);
        fits_close_file(fptr, &status);

        if (status) {
            printCfitsioError("MyImage::readWeight()", status);
            successProcessing = false;
            weightInMemory = false;
        }
        else {
            weightInMemory = true;
            weightOnDrive = true;
            successProcessing = true;
        }
    }
}

void MyImage::initWeightfromGlobalWeight(const QList<MyImage*> &gwList)
{
    if (!successProcessing) return;

    if (*verbosity > 1) emit messageAvailable(chipName + " : Init weight from global weight ...", "image");

    // Load the matching global weight
    bool loadSuccess = false;
    for (auto &gw: gwList) {
        if (gw->filter == filter && filter == "RGB") {
            emit messageAvailable("MyImage::initWeightFromGlobalWeight(): You must first apply the flatfield to the science data!", "error");
            emit critical();
            successProcessing = false;
            loadSuccess = false;
            break;
        }
        if (gw->filter == filter) {
            if (!gw->imageInMemory) {
//                gw->lockForInitWCSneeded = false;
                gw->readImage();
//                gw->lockForInitWCSneeded = true;
            }
            dataWeight = gw->dataCurrent;
            loadSuccess = true;
            weightInMemory = true;
            break;
        }
    }
    if (!loadSuccess) {
        emit messageAvailable(chipName + " : MyImage::initWeightFromGlobalWeight(): Did not find the globalweight for filter " + filter, "error");
        emit critical();
        successProcessing = false;
    }
    // An oddball, not sure that will ever happen
    if (loadSuccess && dataWeight.length() != dataCurrent.length()) {
        emit messageAvailable(chipName + " : MyImage::initWeightFromGlobalWeight(): weight and science image do not have the same size!", "error");
        emit critical();
        successProcessing = false;
    }
}

void MyImage::thresholdWeight(QString imageMin, QString imageMax)
{
    if (imageMin.isEmpty() && imageMax.isEmpty()) return;
    if (imageMin == imageMax) return;

    if (*verbosity > 1) emit messageAvailable(chipName + " : Thresholding weight if image is outside ["+imageMin+","+imageMax+"] ...", "image");
    float minVal = imageMin.toFloat();
    float maxVal = imageMax.toFloat();
    if (imageMin.isEmpty()) minVal = -1.e9;
    if (imageMax.isEmpty()) maxVal = 1.e9;

    if (minVal > maxVal) std::swap(minVal, maxVal);

    long i=0;
    for (auto &pixel : dataWeight) {
        if (dataCurrent.at(i) < minVal) pixel = 0.;
        if (dataCurrent.at(i) > maxVal) pixel = 0.;
        ++i;
    }
}

void MyImage::roundEdgeOfWeight(float edge, bool roundEdge)
{
    dataWeightSmooth = dataWeight;

    for (long j=0; j<naxis2; ++j) {
        float dymin = (j <= naxis2-j) ? j : naxis2-j;
        for (long i=0; i<naxis1; ++i) {
            float dxmin = (i <= naxis1-i) ? i : naxis1-i;
            float d = (dxmin <= dymin) ? dxmin : dymin;
            // Optionally, round the edge
            if (roundEdge) {
                if (dymin <= edge && dxmin <= edge) {
                    d = edge - sqrt((edge-dxmin) * (edge-dxmin) + (edge-dymin) * (edge-dymin));
                    if (d < 0.) d = 0.;
                }
            }
            if (d <= edge) {
                dataWeightSmooth[i+naxis1*j] = 0.5*(-cos(d/edge*3.14159) +1.) * dataWeight.at(i+naxis1*j);
            }
        }
    }
}

void MyImage::applyPolygons(int chip)
{
    if (!successProcessing) return;

    //    QString rootName = baseName;
    //    rootName.truncate(rootName.lastIndexOf('_'));
    QString regionFileName = path+"/reg/"+chipName+".reg";
    QFile regionFile(regionFileName);
    if (!regionFile.exists()) return;

    if (*verbosity > 1) emit messageAvailable(chipName + " : Mask found, applying mask to weight ...", "image");
    addRegionFilesToWeight(naxis1, naxis2, regionFileName, dataWeight);
}

void MyImage::freeWeight()
{
    emit setMemoryLock(true);
    if (minimizeMemoryUsage) {
        dataWeight.clear();
        dataWeight.squeeze();
        weightInMemory = false;
    }
    else {
        dataWeight_deletable = true;
    }
    emit setMemoryLock(false);
}

void MyImage::maskBloomingSpike(QString instType, QString range, QString minVal, bool requested, QString bloomMin, QString bloomMax)
{
    if (!successProcessing) return;
    if (!requested) return;
    if (instType != "OPT") return;

    if (*verbosity > 1) emit messageAvailable(chipName + " : Detecting blooming spikes ...", "image");

    float histstep = 1000.;
    float maxval = maxVec_T(dataCurrent);

    float minval = minVal.toFloat();
    if (minVal.isEmpty()) minval = meanMask_T(dataCurrent) + 10.*rmsMask_T(dataCurrent);

    if (maxval > 200000.) maxval = 200000.;

    QVector<bool> mask(naxis1*naxis2, true);  // false if pixel is good, true if pixel is "bloomed"

    // masking
    long l = 0;
    long k = 0;
    for (auto &pixel : dataCurrent) {
        if (pixel < minval) mask[k] = false;
        else ++l;
        ++k;
    }

    long npix1 = l;  // number of potentially bloomed pixels
    if (npix1 == 0) {
        if (*verbosity > 1) emit messageAvailable(chipName + " : No pixels found above blooming threshold ...", "image");
        return;
    }

    // detect value of blooming spike
    QVector<float> pixels(npix1,0);
    QVector<long> pixelcoords(npix1,0);

    l = 0;
    k = 0;
    for (auto &pixel : dataCurrent) {
        if (mask[k]) {
            pixels[l] = pixel;
            pixelcoords[l] = k;
            ++l;
        }
        ++k;
    }

    // The count thresholds for blooming spikes
    float bloommin = 0.;
    float bloommax = 0.;
    if (!bloomMin.isEmpty() && !bloomMax.isEmpty()) {
        // manually given; NOT IMPLEMENTED in GUI. I think the auto mode works just fine
        bloommin = bloomMin.toFloat();
        bloommax = bloomMax.toFloat();
    }
    else {
        // auto

        long nhist = ceil( (maxval-minval) / histstep);
        QVector<float> hist(nhist,0);

        for (long j=0; j<npix1; ++j) {
            for (k=0; k<nhist; ++k) {
                if (pixels[j] >= minval + histstep * k &&
                        pixels[j] < minval + histstep * (k+1)) {
                    ++hist[k];
                }
            }
        }

        float maxhist = maxVec_T(hist);
        long bloommaxindex = maxIndex(hist);

        float bloomval = minval + bloommaxindex * histstep;

        // detect a reasonable dynamic range for the blooming spike
        long max_int = bloommaxindex;
        long i = 0;  // was i=1; dont understand why; fails if the highest value is at the last position in hist, hence i=0
        long maxindex_int = 0;
        while (hist[max_int] > 0.1 * maxhist && max_int < nhist) {
            max_int = bloommaxindex + i;
            if (max_int > nhist-1) {
                max_int = nhist-1;
                break;
            }
            maxindex_int = max_int;
            ++i;
        }

        long min_int = bloommaxindex;
        i = 0;  // was i=1; dont understand why; fails if the highest value is at the last position in hist, hence i=0
        long minindex_int = 0;
        while (hist[min_int] > 0.1 * maxhist && min_int > 0) {
            min_int = bloommaxindex - i;
            if (min_int < 0) {
                min_int = 0;
                break;
            }
            minindex_int = min_int;
            ++i;
        }

        float rangeval = range.toFloat();
        bloommin = minval + (minindex_int - 1) * histstep - rangeval;
        bloommax = minval + (maxindex_int + 1) * histstep + rangeval;

        bloommax = bloomval + 1.5 * (bloommax - bloomval);
    }

    // qDebug() << bloommin << bloommax;
    // Blooming range:  bloommin bloommax

    // keep only pixels with values within the blooming range
    l = 0;
    for (long i=0; i<npix1; ++i) {
        if (pixels[i] >= bloommin && pixels[i] <= bloommax) {
            ++l;
        }
    }

    long npix2 = l;     // Number of bloomed pixels

    QVector<float> pixels2;
    QVector<long> pixelcoords2;
    pixels2.reserve(npix2);
    pixelcoords2.reserve(npix2);

    for (long i=0; i<npix1; ++i) {
        if (pixels[i] >= bloommin && pixels[i] <= bloommax) {
            pixels2.append(pixels[i]);
            pixelcoords2.append(pixelcoords[i]);
        }
    }

    // Mask the bloomed pixels in the weight
    for (long i=0; i<npix2; ++i) {
        dataWeight[pixelcoords2[i]] = 0.;
    }

    QString bloomRange = "["+QString::number(long(bloommin))+","+QString::number(long(bloommax))+"]";
    if (*verbosity > 1) emit messageAvailable(chipName + " : Bloomed pixels masked. Dynamic range: "+bloomRange, "image");
}

void MyImage::laplaceFilter(QVector<float> &dataFiltered)
{
    // Laplace kernel
    QVector<float> kernel = {-1.,  -2., -1.,
                             -2., +12., -2.,
                             -1.,  -2., -1.};

    // Laplace filtering using direct convolution, to enhance local defects
    long n = naxis1;
    long m = naxis2;
    float datatmp = 0.;

    float baseLevel = medianMask(dataCurrent);

    for (long j=1; j<m-1; ++j) {
        for (long i=1; i<n-1; ++i) {
            long k = i+n*j;
            float sum = 0.;
            int l = 0;
            for (int jt=j-1; jt<=j+1; ++jt) {
                for (int it=i-1; it<=i+1; ++it) {
                    long t = it+n*jt;
                    datatmp = dataCurrent.at(t) - baseLevel + 1000.; // make sure the image has a positive background
                    sum += datatmp;
                    dataFiltered[k] += datatmp * kernel[l];
                    ++l;
                }
            }
            // the filtered image is flux-normalized.
            // Purely empirical, suppresses stars much better (when afterwards subtracting a local median of the laplace filtered image)
            // than if a local median or mean background is subtracted instead.
            // Since we divide, we must make sure the local level is significantly larger than zero, hence the +1000.
            dataFiltered[k] /= sum;
        }
    }
}

void MyImage::median2D(const QVector<float> &data_in, QVector<float> &data_out, int filtersize)
{
    int s = 2*filtersize+1;
    int n = naxis1;
    int m = naxis2;
    s = s*s;
    QVector<float> chunk(s,0);

    for (int j=0; j<m; ++j) {
        for (int i=0; i<n; ++i) {
            if (!globalMask.isEmpty()) {
                if (globalMask[i+n*j]) continue;
            }
            long k = 0;
            for (int jt=j-filtersize; jt<=j+filtersize; ++jt) {
                for (int it=i-filtersize; it<=i+filtersize; ++it) {
                    long t = it+n*jt;
                    if (!globalMask.isEmpty()) {
                        if (it>=0 && jt>=0 && it<n && jt<m && !globalMask[t]) {
                            chunk[k++] = data_in[t];
                        }
                    }
                    else {
                        if (it>=0 && jt>=0 && it<n && jt<m) {
                            chunk[k++] = data_in[t];
                        }
                    }
                }
            }
            data_out[i+n*j] = straightMedianInline(chunk);
        }
    }
}

// Turns out this algorithm is similar to "LAcosmic" (http://www.astro.yale.edu/dokkum/lacosmic/)
void MyImage::cosmicsFilter(QString aggressiveness)
{
    if (!successProcessing) return;
    if (aggressiveness.isEmpty()) return;

    float aggressiveFactor = aggressiveness.toFloat();
    if (aggressiveFactor == 0.) return;

    if (*verbosity > 1) emit messageAvailable(chipName + " : Filtering spurious pixels ...", "image");

    long n = naxis1;
    long m = naxis2;
    QVector<float> dataLaplace(n*m);
    QVector<float> dataMedian(n*m);

    // Laplace filter, then median filter the Laplace filtered image
    laplaceFilter(dataLaplace);             // CHECK: hogging some memory
    median2D(dataLaplace, dataMedian, 1);   // CHECK: hogging some memory

    // Remove the median image from the Laplace image.
    // Suppresses residuals from bright but unsaturated stars
    long k = 0;
    for (auto &pixel : dataLaplace) {
        pixel -= dataMedian[k];
        ++k;
    }

    float rms = 1.48 * madMask_T(dataLaplace, globalMask);
    float thresh = 8.0 / aggressiveFactor; // user-adjusted threshold; the higher, the lower the threshold. Default: 8 sigma detection
    float cutoff = thresh*rms;

    // Initiate cosmics mask
    QVector<bool> cosmicsMask(n*m, false);

    // Thresholding step 1
    // Everything deviating by more than 'cutoff'
    k = 0;
    for (auto &pixel : dataLaplace) {
        // not masking negative outliers because of the compensation of the laplace kernel
        if (pixel > cutoff) cosmicsMask[k] = true;
        ++k;
    }

    // Thresholding step 2
    // If pixels above and below, or left and right are masked, then the current pixel gets also masked
    QVector<bool> mask_tmp = cosmicsMask;
    for (int j=1; j<m-1; ++j) {
        for (int i=1; i<n-1; ++i) {
            if ((mask_tmp[i-1+n*j] && mask_tmp[i+1+n*j])
                    || (mask_tmp[i+n*(j-1)] && mask_tmp[i+n*(j+1)])) {
                cosmicsMask[i+n*j] = true;
            }
        }
    }

    // Thresholding step 3
    // If at least 2 of the 8 surrounding pixels deviate by more than 'cutoff', and are not in the same row or column,
    // then the current pixel gets also masked.
    for (int j=1; j<m-1; ++j) {
        for (int i=1; i<n-1; ++i) {
            int count = 0;
            int isum = 0;
            int jsum = 0;
            for (int jt=j-1; jt<=j+1; ++jt) {
                for (int it=i-1; it<=i+1; ++it) {
                    long t = it+n*jt;
                    if (dataLaplace[t] > cutoff) {
                        isum += it;
                        jsum += jt;
                        ++count;
                    }
                }
            }
            // Using the mean i and j to make sure pixels are not in the same column;
            // This is to avoid that a single bad column/row gets tripled in width.
            float meani = float(isum)/float(count);
            float meanj = float(jsum)/float(count);
            if (count >= 2 && fabs(meani-i) < 1. && fabs(meanj-j) < 1. ) cosmicsMask[i+n*j] = true;
        }
    }

    // Thresholding step 4
    // If at least 4 out of the 8 surrounding pixels deviate by more than 0.5*cutoff, then the current pixel gets also masked
    for (int j=1; j<m-1; ++j) {
        for (int i=1; i<n-1; ++i) {
            int count = 0;
            for (int jt=j-1; jt<=j+1; ++jt) {
                for (int it=i-1; it<=i+1; ++it) {
                    long t = it+n*jt;
                    if (dataLaplace[t] > 0.5*cutoff) ++count;
                }
            }
            if (count >= 4) cosmicsMask[i+n*j] = true;
        }
    }

    // Apply the cosmics mask to the weight map
    k = 0;
    for (auto &pixel : dataWeight) {
        if (cosmicsMask[k]) pixel = 0.;
        ++k;
    }
}
