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

#include <fitsio.h>
#include <omp.h>

#include "swarpfilter.h"
#include "../myimage/myimage.h"
#include "../functions.h"

#include <QVector>
#include <QDebug>
#include <QTextStream>

SwarpFilter::SwarpFilter(QString coadddirname, QString kappaString,
                         QString clustersizeString, QString borderwidthString,
                         int maxCPU, int *verbose)
{
    // Probably not needed, unless to case distinct linux and mac
    initEnvironment(thelidir, userdir);

    coaddDirName = coadddirname;
    nthreads = maxCPU;
    verbosity = verbose;

    if (!kappaString.isEmpty()) kappa = kappaString.toInt();
    if (!borderwidthString.isEmpty()) maskWidth = borderwidthString.toInt();
    if (!clustersizeString.isEmpty()) clusterSize = clustersizeString.toInt();
    if (clusterSize > 9) clusterSize = 9; // upper limit

    coaddDir.setPath(coaddDirName);
    if (!coaddDir.exists()) {
        qDebug() << "QDEBUG: SwarpFilter(): Could not find coadd directory" << coaddDirName;
        return;
    }

    omp_init_lock(&lock);
}

void SwarpFilter::init()
{
    // Collect information about the coadded image and the resampled images
    getImages();
    getCoaddInfo();
    getGeometries();

    // RAM considerations
    getBlocksize();

    // Set the size for the containers
    initStorage();

}

SwarpFilter::~SwarpFilter()
{
    omp_destroy_lock(&lock);
}

void SwarpFilter::freeMemoryVectors()
{
    images.clear();
    weights.clear();
    naxis1.clear();
    naxis2.clear();
    crpix1.clear();
    crpix2.clear();
    sky.clear();
    fluxscale.clear();
    xoffset.clear();
    yoffset.clear();

    images.squeeze();
    weights.squeeze();
    naxis1.squeeze();
    naxis2.squeeze();
    crpix1.squeeze();
    crpix2.squeeze();
    sky.squeeze();
    fluxscale.squeeze();
    xoffset.squeeze();
    yoffset.squeeze();
}

void SwarpFilter::freeMemoryBlocks()
{
    for (long i=0; i<num_images; ++i) {
        block_coadd_image[i].clear();
        block_coadd_index[i].clear();
        block_coadd_image[i].squeeze();
        block_coadd_index[i].squeeze();
    }
    block_coadd_image.clear();
    block_coadd_index.clear();
    block_coadd_image.squeeze();
    block_coadd_index.squeeze();
}

// This does not read the data sections, it just creates MyImage pointers to the resampled images and weights
void SwarpFilter::getImages()
{
    // Fill the list of MyImage types of individual images and master calibration files in this data directory
    QStringList filter;
    filter << "*resamp.fits";
    QStringList resampledImages = coaddDir.entryList(filter);
    num_images = resampledImages.length();

    for (auto &it : resampledImages) {
        QString base = it.remove("resamp.fits");
        QFile img(coaddDirName+"/"+base+"resamp.fits");
        QFile wgt(coaddDirName+"/"+base+"resamp.weight.fits");
        if (!img.exists() || !wgt.exists()) {
            emit messageAvailable("ERROR: SwarpFilter::getImages(): Image " + base + " resamp.[weight].fits does not exist!", "error");
            return;
        }
        // global masks don't apply here because of different image geometry. Hence passing QVector<bool>()
        QVector<bool> dummyMask;
        dummyMask.clear();
        MyImage *myImage = new MyImage(coaddDirName, base+"resamp.fits", "", 0, dummyMask, verbosity);
        MyImage *myWeight = new MyImage(coaddDirName, base+"resamp.weight.fits", "", 0, dummyMask, verbosity);
        images.append(myImage);
        weights.append(myWeight);
    }

    emit messageAvailable("Including "+QString::number(num_images)+" images ...", "config");
}

void SwarpFilter::getCoaddInfo()
{
    QFile coaddHead(coaddDirName+"/coadd.head");

    if (!coaddHead.open(QIODevice::ReadOnly)) {
        emit messageAvailable("ERROR: SwarpFilter::getCoaddInfo(): "+coaddHead.fileName() + " " +coaddHead.errorString(), "error");
        return;
    }

    QTextStream in(&coaddHead);

    while(!in.atEnd()) {
        QString line = in.readLine();
        if (line.contains("NAXIS1  =")) coadd_naxis1 = line.split("=")[1].simplified().split(" ")[0].toLong();
        if (line.contains("NAXIS2  =")) coadd_naxis2 = line.split("=")[1].simplified().split(" ")[0].toLong();
        if (line.contains("CRPIX1  =")) coadd_crpix1 = line.split("=")[1].simplified().split(" ")[0].toFloat();
        if (line.contains("CRPIX2  =")) coadd_crpix2 = line.split("=")[1].simplified().split(" ")[0].toFloat();
    }
    coaddHead.close();
    emit messageAvailable("coadd.head found ...", "config");
}

void SwarpFilter::getGeometries()
{
    double crpix1tmp = 0.;
    double crpix2tmp = 0.;
    long naxis1tmp = 0;
    long naxis2tmp = 0;
    double skytmp = 0.;
    double fluxscaletmp = 0.;

    crpix1.reserve(num_images);
    crpix2.reserve(num_images);
    naxis1.reserve(num_images);
    naxis2.reserve(num_images);
    sky.reserve(num_images);
    fluxscale.reserve(num_images);
    for (auto &it : images) {
        if (!it->informSwarpfilter(naxis1tmp, naxis2tmp, crpix1tmp, crpix2tmp, skytmp, fluxscaletmp)) {
            emit messageAvailable("ERROR: SwarpFilter::loadGeometries(): Could not read the geometries of the resampled images", "error");
            return;
        }
        naxis1.append(naxis1tmp);
        naxis2.append(naxis2tmp);
        crpix1.append(crpix1tmp);
        crpix2.append(crpix2tmp);
        sky.append(skytmp * fluxscaletmp);        // rescaled sky values (corrected for relative transparency)
        fluxscale.append(fluxscaletmp);
        xoffset.append(coadd_crpix1 - crpix1tmp);
        yoffset.append(coadd_crpix2 - crpix2tmp);
    }
    emit messageAvailable("Geometries of the resampled images loaded ...", "config");
}

// The maximum number of lines that can be read without filling up the RAM
void SwarpFilter::getBlocksize()
{
    long systemRAM = 1024 * get_memory();

    // maxmimum memory used: 50% of the available RAM
    blocksize = 0.5 * systemRAM / (coadd_naxis1 * sizeof(float) * num_images * 2);
    blocksize = blocksize > coadd_naxis2 ? coadd_naxis2 : blocksize;  // upper limit

    if (blocksize < 1) {
        emit messageAvailable("ERROR: SwarpFilter::getBlocksize(): Not enough memory available for SwarpFilter", "error");
        return;
    }

    // TODO: introduce upper limit, like naxis2[i]? Variable between images.
    // What happens if larger than naxis2[i]?
    if (blocksize > 300) blocksize = 300;  // upper limit (can probably be dropped after some testing)
    chunksize = coadd_naxis1 * blocksize;
    nblocks = long(coadd_naxis2 / blocksize + 1.);
    emit messageAvailable("Block size = "+QString::number(blocksize) + ", Number of blocks = "+QString::number(nblocks), "config");
}

void SwarpFilter::initStorage()
{
    block_coadd_image.resize(num_images);
    block_coadd_index.resize(num_images);
    sky.resize(num_images);
    badpixindex.resize(num_images);

    for (long i=0; i<num_images; ++i) {
        badpixindex[i].reserve(10000);
    }
}


//******************************************************************
void SwarpFilter::runCosmicFilter()
{
    // Doing the init here, so that the signals get heard outside (connections are made after the constructor).
    init();

    progressStepSize = 66. / nblocks;

    emit messageAvailable("Identifying outliers ...", "output");
    // load the images and identify bad pixels
    for (long block=0; block<nblocks; ++block) {

        QVector<long> presentImages;
        presentImages.reserve(num_images);

        if (*verbosity >= 2) emit messageAvailable("Processing block " + QString::number(block+1) + "/" + QString::number(nblocks)+" ...", "output");

        // read a chunk of data from the images
#pragma omp parallel num_threads(nthreads)
        {
            // two private temporary containers
            QVector<float> resampledData;
            QVector<long> resampledCoaddIndex;

            // loop over resampled images
#pragma omp for
            for (long i=0; i<num_images; ++i) {
                // read a chunk of data overlapping with the current block
                bool isImagePresent = get_coaddblock(i, block, resampledData, resampledCoaddIndex);

                // transfer of pixel data must be sequential
#pragma omp critical (updateBlockData)
                {
                    if (isImagePresent) presentImages.append(i);
                    block_coadd_image[i].swap(resampledData);
                    block_coadd_index[i].swap(resampledCoaddIndex);
                }
            }
        }

        // if this block does not contain resampled images (bottom or top borders of coadded image)
        if (presentImages.isEmpty()) continue;

        // find the bad pixels
#pragma omp parallel num_threads(nthreads)
        {
            // bad pixel pair (index in the coadded image and index in the individual frame)
            QVector<std::pair<long,long>> bpp;
            bpp.reserve(100000);
            QVector<float> gooddata;           // The pixel values of the images contributing to a coadded pixel
            QVector<long> gooddataind;         // The index of an image contributing to a coadded pixel
            gooddata.reserve(num_images);      // maximally num_images will contribute to a coadded pixel
            gooddataind.reserve(num_images);

#pragma omp for
            for (long l=0; l<chunksize; ++l) {                 // loop over coadded pixels
                for (auto &pi : presentImages) {               // Loop over all images present at the current coadded pixel
                    long pixpos = block_coadd_index[pi][l];    // The index of the current coadded pixel in the coord system of the resampled image
                    if (pixpos >= 0) {                         // Negative if image does not overlap with current pixel
                        float value = block_coadd_image[pi][pixpos];
                        if (value != 0.) {                     // If a pixel has value zero then it was most likely masked
                            gooddata.append(value);            // pixel value
                            gooddataind.append(pi);            // image index
                        }
                    }
                }
                long ngoodweight = gooddata.length();  // Number of pixels contributing to a coadded pixel
                long currentpixel = l + block*chunksize;
                identify_bad_pixels(gooddata, gooddataind, currentpixel, ngoodweight, bpp);
                gooddata.clear();
                gooddataind.clear();
            }
#pragma omp critical (updateBadPixels)
            {
                updateBadPixelIndex(bpp);
            }
        }

        // Reset the block data. Not sure why this is necessary. Perhaps caused by swap()?
        for (long i=0; i<num_images; ++i) {
            block_coadd_image[i].clear();
            block_coadd_index[i].clear();
        }
#pragma omp atomic
        *progress += progressStepSize;
        emit progressUpdate(*progress);
    }

    // The badpixindex must be sorted!
    for (auto &bp : badpixindex) {
        std::sort(bp.begin(), bp.end());
    }

    freeMemoryBlocks();

    // Writing results
    writeWeight();

    freeMemoryVectors();
}

void SwarpFilter::updateBadPixelIndex(const QVector<std::pair<long,long>> bpp)
{
    for (auto &pair : bpp) {
        badpixindex[pair.first].append(pair.second);
    }
}

//***************************************************************************************
// Load a section of one of the resampled images
// (a block naxis1 wide and some rows high)
//***************************************************************************************
bool SwarpFilter::get_coaddblock(const int index, const long block, QVector<float> &resampledData, QVector<long> &resampledCoaddIndex)
{
    // index == current image
    long bbs0 = block * blocksize;
    long bbs1 = bbs0 + blocksize;

    // nothing to do if the image is entirely below or above the current coadd block
    if (yoffset[index] + naxis2[index] <= bbs0 || yoffset[index] >= bbs1) {
        return false;
    }

    // The image overlaps with the current coadd block

    // assume that the image covers the block entirely:
    long firstline2read = bbs0 - yoffset[index];           // we start counting at 0!
    long lastline2read  = firstline2read + blocksize - 1;  // we start counting at 0!

    // if the image does not cover the lower part of the block
    // but the upper part entirely
    if (firstline2read < 0 && yoffset[index] + naxis2[index] >= bbs1) {
        lastline2read  = bbs1 - yoffset[index] - 1;
        firstline2read = 0;
    }

    // if the image does not cover the upper part of the block
    // but the lower part entirely
    if (yoffset[index] + naxis2[index] < bbs1 && yoffset[index] <= bbs0) {
        lastline2read  = naxis2[index] - 1;
        firstline2read = bbs0 - yoffset[index];
    }

    // if the image is entirely contained in the block
    // without touching its borders
    if (yoffset[index] > bbs0 && yoffset[index] + naxis2[index] < bbs1) {
        firstline2read = 0;
        lastline2read  = naxis2[index] - 1;
    }

    int nsub = naxis1[index];
    int msub = lastline2read - firstline2read + 1;

    float *data_in = new float[nsub*msub];

    // Load the data sections;
    // One could load the weights too, but that would double the memory load for very little return
    // We later on reject image pixels with zero value; likely they have zero weight; what would be missed is manually masked areas, such as satellites.
    // But the algorithm is supposed to detect them anyway, so no harm done by skipping the weights.
    images[index]->loadDataSection(0, naxis1[index]-1, firstline2read, lastline2read, data_in);
    long l = 0;  // the running index of the image
    long k = 0;  // the running index of the coadd [0,chunksize)

    resampledData.reserve(naxis1[index]*(lastline2read-firstline2read+1)); // Contains valid data points, only
    resampledCoaddIndex.resize(coadd_naxis1*blocksize); // Must stretch over the entire chunksize
    for (auto &it: resampledCoaddIndex) it = -1; // Initialize with invalid index

    // Map the image pixels onto the coadded image block.
    // Save some CPU cycles and do array calculations outside the loop
    long xoff = xoffset[index];
    long yoff = yoffset[index];
    long nax1 = naxis1[index];
    long nax2 = naxis2[index];
    float fluxcorr = fluxscale[index];
    for (long j=0; j<blocksize; ++j) {
        long yi = bbs0 + j - yoff;
        for (long i=0; i<coadd_naxis1; ++i) {
            long xi = i - xoff;
            // if the image overlaps with the coadd block
            if (xi >= 0 && yi >= 0 && xi < nax1 && yi < nax2) {
                resampledData.append(data_in[l] * fluxcorr);
                resampledCoaddIndex[k] = l;
                ++l;
            }
            ++k;
        }
    }

    if (l==0) {
        emit messageAvailable("SwarpFilter:getCoaddBlock(): No overlap was found<br>"
                              +images[index]->name + " : First line / last line: "
                              + QString::number(firstline2read) +  " " + QString::number(lastline2read), "error");
        return false;
    }

    delete [] data_in;

    return true;
}

//***************************************************************************************
// Identify the bad pixels in a stack
//***************************************************************************************
void SwarpFilter::identify_bad_pixels(const QVector<float> &gooddata, const QVector<long> &gooddataind,
                                      const long &currentpixel, const long &ngoodweight, QVector<std::pair<long,long>> &bpp)
{
    // no rejection if less than 2 pixels contributing to a coadded pixel
    if (ngoodweight < 2 ) return;

    float mean = 0.;
    float rms = 0.;
    float thresh = 0.;

    // we could determine ngoodweight from gooddata itself, but the length is already known
    // and thus it is cheaper to just take it as an argument
    // long ngoodweight = gooddata.length();

    // NOTE: I tried to compare (it-mean)^2 against thresh*thresh
    // instead of fabs(it-mean) against thresh, to avoid the expensive sqrt().
    // However, the overall performance gain is on the order of 2-3%, hence I drop this for code clarity

    // 2 pixels in the stack: use poisson rms estimated from sky noise
    if (ngoodweight == 2) {
        float sum = gooddata[0] + gooddata[1];
        mean = 0.5 * sum;
        rms = sum + (sky[gooddataind[0]] + sky[gooddataind[1]]);
        if (rms > 0.) {
            thresh = sqrt(0.5 * rms);
        }
        else return;  // no rejection of bad pixels
    }

    // 3-4 pixels in the stack: reject max pixel
    else if (ngoodweight <= 4) {
        stackfilter_rejectmax(gooddata, mean, rms);
        if (rms > 0.) {
            mean /= ngoodweight - 1;
            rms = sqrt(((ngoodweight - 1) / (ngoodweight - 2)) * (rms / (ngoodweight - 1) - mean * mean));
            thresh = 6. * kappa / ngoodweight * rms;
        }
        else return;  // no rejection of bad pixels
    }

    // 5 or more pixels in the stack: reject min and max pixel
    else {
        stackfilter_rejectminmax(gooddata, mean, rms);
        if (rms > 0.) {
            mean /= ngoodweight - 2;
            rms = sqrt(((ngoodweight - 2) / (ngoodweight - 3)) * (rms / (ngoodweight - 2) - mean * mean));
            // adaptive rms threshold
            if (ngoodweight < 6) thresh = 6. * kappa / ngoodweight * rms;
            else thresh = kappa * rms;
        }
        else return;  // no rejection of bad pixels
    }

    if (ngoodweight > 2) {
        long i=0;
        for (auto &it : gooddata) {
            if (fabs(it - mean) > thresh) {
                bpp.append(std::make_pair(gooddataind[i], currentpixel));
            }
            ++i;
        }
    }
    else {
        long i=0;
        for (auto &it : gooddata) {
            if (fabs(it - mean) > thresh) {
                // reject pixel only if it is the brighter one
                if ((i == 0 && gooddata[0] > gooddata[1]) ||
                        (i == 1 && gooddata[1] > gooddata[0])) {
                    bpp.append(std::make_pair(gooddataind[i], currentpixel));
                }
            }
            ++i;
        }
    }
}

//***************************************************************************************
// ID the max value in the stack, reject it, and calculate meansq and rmssq
//***************************************************************************************
void SwarpFilter::stackfilter_rejectmax(const QVector<float> &gooddata, float &meanval, float &rmsval)
{
    float maxval = gooddata[0];
    meanval = 0.;
    rmsval = 0.;

    for (auto &it : gooddata) {
        if (it > maxval) maxval = it;
        meanval += it;
        rmsval += it*it;
    }

    meanval -= maxval;
    rmsval -= (maxval*maxval);
}

//***************************************************************************************
// ID the min and max values in the stack, reject them, and calculate meansq and rmssq
//***************************************************************************************
void SwarpFilter::stackfilter_rejectminmax(const QVector<float> &gooddata, float &meanval, float &rmsval)
{
    float minval = gooddata[0];
    float maxval = gooddata[0];
    meanval = 0.;
    rmsval = 0.;

    for (auto &it : gooddata) {
        if (it < minval) minval = it;
        else if (it > maxval) maxval = it;
        meanval += it;
        rmsval += it*it;
    }

    meanval -= (minval + maxval);
    rmsval -= (minval*minval + maxval*maxval);
}

//**************************************************************
void SwarpFilter::writeWeight()
{

    // cout << "This is thread " << args->th << endl;
    emit messageAvailable("Writing updated weight maps ...", "output");

    progressStepSize = 34. / num_images;

#pragma omp parallel for num_threads(nthreads)
    for (long i=0; i<num_images; ++i) {

        long n = naxis1[i];
        long m = naxis2[i];
        QVector<char> weight_out(n*m, 1);   // 1 = pixel is good, 0 = pixel is bad
        QVector<char> weight_cpy(n*m, 1);

        long s = 0;
        long t = 0;
        long nbadpix = badpixindex[i].size();
        // mask all bad pixels found
        for (long j=yoffset[i]; j<(yoffset[i] + m); ++j) {
            if (t==nbadpix) break;
            for (long l=xoffset[i]; l<(xoffset[i] + n); ++l) {
                if (t==nbadpix) break;
                long k = l + coadd_naxis1 * j;
                // WARNING: THIS IS FAST, BUT ASSUMES A SORTED BADPIX INDEX!
                if (k == badpixindex[i][t]) {
                    weight_out[s] = 0;
                    if (clusterSize < 2) weight_cpy[s] = 0;
                    ++t;
                }
                ++s;
            }
        }

        // NOTE:
        // There could be many more masked pixels in badpixindex[i] than visibly marked in the end,
        // if the clustersize is larger than 1.

        // mask only clusters consisting of at least 'clustersize' pixels
        if (clusterSize > 1) {
            // we ignore the 1 pixel wide border of the image
            for (long j=1; j<m-1; ++j) {
                for (long k=1; k<n-1; ++k) {
                    if (weight_out[k+n*j] == 0) {
                        long clustercount = 1;
                        for (long l=j-1; l<=j+1; ++l) {
                            for (long o=k-1; o<=k+1; ++o) {
                                clustercount += (weight_out[o+(l*n)] ^ 1);
                            }
                        }
                        if (clustercount >= clusterSize) {
                            for (long l=j-1; l<=j+1; ++l) {
                                for (long o=k-1; o<=k+1; ++o) {
                                    weight_cpy[o+(l*n)] = weight_out[o+(l*n)];
                                }
                            }
                        }
                    }
                }
            }
            // copy the vectors
            weight_out = weight_cpy;
        }

        // if a border of width 'width' pixels should be masked around a
        // bad pixel, too
        if (maskWidth > 0) {
            for (long j=0; j<m; ++j) {
                for (long k=0; k<n; ++k) {
                    if (weight_cpy[k+n*j] == 0) {
                        long jmW0 = j - maskWidth;
                        long kmW0 = k - maskWidth;
                        long jmW1 = j + maskWidth;
                        long kmW1 = k + maskWidth;
                        long masklinemin = jmW0 < 0 ? 0 : jmW0;
                        long maskcolmin  = kmW0 < 0 ? 0 : kmW0;
                        long masklinemax = jmW1 > (m - 1) ? (m - 1) : jmW1;
                        long maskcolmax  = kmW1 > (n - 1) ? (n - 1) : kmW1;
                        for (long l=masklinemin; l <= masklinemax; ++l) {
                            for (long o=maskcolmin; o <= maskcolmax; ++o) {
                                weight_out[o+(l*n)] = 0;
                            }
                        }
                    }
                }
            }
        }

        QString outName = weights[i]->path + "/" + weights[i]->name;
        // Modify the resampled weight and write it
        weights[i]->readImage(outName);
        long k=0;
        for (auto &pixel : weights[i]->dataCurrent) {
            pixel *= weight_out[k];
            ++k;
        }
        weights[i]->writeImage(outName);
        weights[i]->freeData();

#pragma omp atomic
        *progress += progressStepSize;
        emit progressUpdate(*progress);
    }
}
