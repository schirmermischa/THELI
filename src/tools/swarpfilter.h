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

#ifndef SWARPFILTER_H
#define SWARPFILTER_H

#include "../instrumentdata.h"
#include "../myimage/myimage.h"

#include <omp.h>

#include <QVector>
#include <QObject>
#include <QDir>

class SwarpFilter : public QObject
{
    Q_OBJECT
public:
    SwarpFilter(QString coadddirname, QString kappaString, QString clustersizeString,
                QString borderwidthString, int maxCPU, int *verbose, QObject *parent = nullptr);
    ~SwarpFilter();

    float *progress;

    void runCosmicFilter();
    void init();
signals:
    void messageAvailable(QString message, QString type);
    void progressUpdate(float progress);

private:

    omp_lock_t lock;
    float progressStepSize = 0;

    int nthreads = 1;

    QVector<MyImage*> images;
    QVector<MyImage*> weights;

    long coadd_naxis1;
    long coadd_naxis2;
    float coadd_crpix1;
    float coadd_crpix2;

    long num_images = 0;       // Number of resampled images
    float kappa = 4.0;         // Detection threshold (outliers in units of sigma)
    int clusterSize = 1;       // Minimum size of a cluster of bad pixels to trigger masking
    int maskWidth = 0;         // Width of an extra border around a detected bad pixel cluster
    long blocksize = 0;        // Number of lines read at a time
    long chunksize = 0;        // Number of pixels analysed in one step
    long nblocks = 0;          // The total number of blocks to process

    QVector<long> naxis1;
    QVector<long> naxis2;
    QVector<float> crpix1;     // CRPIX is integer if stacked with swarp
    QVector<float> crpix2;     // CRPIX is integer if stacked with swarp
    QVector<float> fluxscale;
    QVector<float> sky;        // rescaled sky values (corrected for relative transparency by 'fluxscale')
    QVector<long> xoffset;     // number of pixels between the left border of coadd.fits and the resampled image
    QVector<long> yoffset;     // number of pixels between the lower border of coadd.fits and the resampled image

    QVector< QVector<long> > badpixindex;
    QVector< QVector<long> > block_coadd_index;
    QVector< QVector<float> > block_coadd_image;

    QDir coaddDir;
    QString coaddDirName = "";
    QString thelidir;
    QString userdir;
    QString tmpdir;

    int *verbosity;

    void freeMemoryBlocks();
    void freeMemoryVectors();
    void getCoaddInfo();
    void getGeometries();
    void getImages();
    void getBlocksize();
    bool get_coaddblock(const int index, const long block, QVector<float> &resampledData, QVector<long> &resampledCoaddIndex);
    void identify_bad_pixels(const QVector<float> &gooddata, const QVector<long> &gooddataind, const long &currentpixel,
                             const long &ngoodweight, QVector<pair<long,long>> &bpp);
    void initStorage();
    void stackfilter_rejectmax(const QVector<float> &gooddata, float &meanval, float &rmsval);
    void stackfilter_rejectminmax(const QVector<float> &gooddata, float &meanval, float &rmsval);
    void updateBadPixelIndex(const QVector<pair<long, long> > bpp);
    void writeWeight();
};

#endif // SWARPFILTER_H
