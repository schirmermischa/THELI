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
#include "myimage.h"

#include <gsl/gsl_math.h>
#include <gsl/gsl_interp2d.h>
#include <gsl/gsl_spline2d.h>

#include <vector>

#include <QDebug>
#include <QMessageBox>


void MyImage::backgroundModel(int filtersize, QString splinemode)
{
    // Currently, we always do an interpolation, independently of 'splinemode'
    if (!successProcessing) return;
    if (backgroundModelDone) return;

    if (*verbosity > 1) emit messageAvailable(chipName + " : Background mapping ... ", "image");

    filterSize = filtersize;
    splineMode = splinemode;
    gridStep = filterSize / subSample;

    dataBackground_deletable = false;

    // QVector<bool> maskData(naxis1*naxis2, false);

    // The dimensions of the padded image (margins)
    getPadDimensions();

    // Pad the image
    // padImage();

    // Place a grid over the image
    planGrid();

    // Calculate background value at each grid point
    getGridStatistics();

    // replace blank grid points with local estimate
    replaceBlankGridStatistics();

    // Spatial median filter of the statistics
    filterGridStatistics();

    // Do the spline fit
    fitBackgroundGSL();

    backgroundModelDone = true;
}

void MyImage::getMeanBackground()
{
    if (!successProcessing) return;

    float skysum = 0.;
    for (auto &pixel : dataBackground) {
        skysum += pixel;
    }
    meanExposureBackground = skysum / (naxis1*naxis2);
}

void MyImage::fitBackgroundGSL()
{
    if (!successProcessing) return;

    double *xa = new double[n_grid];
    double *ya = new double[m_grid];
    double *za = new double[nGridPoints];

    // Prepare the input data
    for (int i=1; i<=n_grid; ++i) xa[i-1] = i*gridStep;
    for (int j=1; j<=m_grid; ++j) ya[j-1] = j*gridStep;
    long k=0;
    for (int j=0; j<m_grid; ++j) {
        for (int i=0; i<n_grid; ++i) {
            za[k] = backStatsGrid[k];
            ++k;
        }
    }

    const gsl_interp2d_type *T = gsl_interp2d_bicubic;
    gsl_spline2d *spline = gsl_spline2d_alloc(T, n_grid, m_grid);
    gsl_interp_accel *xacc = gsl_interp_accel_alloc();
    gsl_interp_accel *yacc = gsl_interp_accel_alloc();

    // initialize interpolation
    gsl_spline2d_init(spline, xa, ya, za, n_grid, m_grid);

    // interpolate; remove padding at the same time
    dataBackground.clear();
    dataBackground.resize(naxis1*naxis2);
//    dataBackground.squeeze();             // deactivated; causes random crash I don't understand (perhaps not anymore after reading memory directly from /proc/meminfo
    for (long j=pad_b; j<m_pad-pad_t; ++j) {
        double y = double(j);
        for (long i=pad_l; i<n_pad-pad_r; ++i) {
            if (i-pad_l < 0 || i-pad_l >= naxis1 || j-pad_b < 0 || j-pad_b >= naxis2) qDebug() << "MyImage::background: GSLFIT: padding error";
            double x = double(i);
            dataBackground[i-pad_l+naxis1*(j-pad_b)] = gsl_spline2d_eval(spline, x, y, xacc, yacc);
        }
    }

    gsl_spline2d_free(spline);
    gsl_interp_accel_free(xacc);
    gsl_interp_accel_free(yacc);

    delete [] xa;
    delete [] ya;
    delete [] za;
    xa = nullptr;
    ya = nullptr;
    za = nullptr;
}

void MyImage::getGridStatistics()
{
    if (!successProcessing) return;

    // Calculate modes by stepping through grid points
    backStatsGrid.clear();
    backStatsGrid.fill(-1e9, nGridPoints);
    rmsStatsGrid.clear();
    rmsStatsGrid.fill(-1., nGridPoints);

    long sampleSize = gridStep * gridStep / 4;       // This many pixels are evaluated at each grid point
    QVector<float> backgroundSample;
    backgroundSample.reserve(sampleSize);
    // for (long index=0; index<nGridPoints; ++index) {

    // TODO: grid size must be at least twice smaller than the smoothing kernel!

    for (long jg=1; jg<m_grid-1; ++jg) {
        for (long ig=1; ig<n_grid-1; ++ig) {
            long index = ig+n_grid*jg;
            // Select data points in a square around the current grid point
            for (long j=grid[1][index]-gridStep/2; j<grid[1][index]+gridStep/2; ++j) {
                for (long i=grid[0][index]-gridStep/2; i<grid[0][index]+gridStep/2; ++i) {
                    //                    if (!mask_padded[i+n_pad*j]) backgroundSample.append(data_padded[i+n_pad*j]);
                    if (i-pad_l < 0 || i-pad_l >= naxis1 || j-pad_b < 0 || j-pad_b>=naxis2) continue;
                    // Take into account global mask, and possibly object masks as well (if defined)

                    // With global mask
                    long ii = i-pad_l+naxis1*(j-pad_b);
                    if (globalMaskAvailable && !globalMask[ii]) {
                        if (!weightInMemory) {
                            if (!objectMaskDone || !objectMask[ii]) {   // !objectMask[ii] implies objectMaskDone = true
                                backgroundSample.append(dataCurrent[ii]);
                            }
                        }
                        else {
                            if (dataWeight[ii] > 0.
                                    && (!objectMaskDone || !objectMask[ii])) {
                                backgroundSample.append(dataCurrent[ii]);
                            }
                        }
                    }
                    // without global mask: external image, e.g. for absZP
                    else {
                        if (!weightInMemory) {
                            if (!objectMaskDone || !objectMask[ii]) {
                                backgroundSample.append(dataCurrent[ii]);
                            }
                        }
                        else {
                            if (dataWeight[ii] > 0.
                                    && (!objectMaskDone || !objectMask[ii])) {
                                backgroundSample.append(dataCurrent[ii]);
                            }
                        }
                    }
                }
            }

            QVector<float> sky = modeMask(backgroundSample, "stable");
            if (sky[1] > 0.) backStatsGrid[index] = sky[0];  // Histogram peak location (if rms could be evaluated, or data were present)
            if (sky[1] > 0.) rmsStatsGrid[index] = sky[1];
            backgroundSample.clear();
        }
    }

    // calculate average stdev (ignoring the padded border)
    skySigma = 0.;
    float count = 0.;
    for (auto &sigma : rmsStatsGrid) {
        if (sigma > 0.) {
            skySigma += sigma;
            ++count;
        }
    }
    skySigma /= count;

    //    data_padded.clear();
    //    data_padded.squeeze();
    //    mask_padded.clear();
    //    mask_padded.squeeze();
}

// replace a grid point that could not be evaluated with the mean of its closest neighbors
void MyImage::replaceBlankGridStatistics()
{
    if (!successProcessing) return;

    QVector<float> backup = backStatsGrid;

    long gmax = n_grid < m_grid ? m_grid : n_grid;

    for (long j=0; j<m_grid; ++j) {
        for (long i=0; i<n_grid; ++i) {
            if (backup[i+n_grid*j] == -1e9) {
                // found a blank grid point
                long s = 0;
                float nfound = 0.;
                float sum = 0.;
                // search around this point with increasing radius
                while (nfound < 1. && s<gmax) {
                    int imin = i-s < 0 ? 0 : i-s;
                    int jmin = j-s < 0 ? 0 : j-s;
                    int imax = i+s >= n_grid ? n_grid-1 : i+s;
                    int jmax = j+s >= m_grid ? m_grid-1 : j+s;
                    sum = 0.;
                    nfound = 0.;
                    for (int jj=jmin; jj<=jmax; ++jj) {
                        for (int ii=imin; ii<=imax; ++ii) {
                            float gridVal = backup[ii+n_grid*jj];
                            if (gridVal > -1e9) {
                                sum += gridVal;
                                ++nfound;
                            }
                        }
                    }                    ++s;
                }
                backStatsGrid[i+n_grid*j] = sum / nfound;
            }
        }
    }
    if (*verbosity > 1) {
        emit messageAvailable(chipName + " : mean / stdev = "
                              + QString::number(meanMask_T(backStatsGrid), 'f', 3) + " / "
                              + QString::number(skySigma, 'f', 3), "image");
    }
}

void MyImage::filterGridStatistics()
{
    if (!successProcessing) return;

    // WARNING: this function alters the values of 'backStatsGrid'
    // Create a copy
    QVector<float> backStatsGridOrig = backStatsGrid;

    // Median filter
    QVector<float> medianSample;
    medianSample.reserve(9);     // maximally 3x3
    for (long j=0; j<m_grid; ++j) {
        // Stay within bounds
        // It would be good to include more grid points for the statistics, in particular in the corners
        int jmin = j-1 < 0 ? 0 : j-1;
        int jmax = j+1 >= m_grid ? m_grid-1 : j+1;
        for (long i=0; i<n_grid; ++i) {
            int imin = i-1 < 0 ? 0 : i-1;
            int imax = i+1 >= n_grid ? n_grid-1 : i+1;
            for (int jj=jmin; jj<=jmax; ++jj) {
                for (int ii=imin; ii<=imax; ++ii) {
                    medianSample.push_back(backStatsGridOrig[ii+n_grid*jj]);
                }
            }
            backStatsGrid[i+n_grid*j] = straightMedian_T(medianSample);
            medianSample.clear();
        }
    }
}

void MyImage::getPadDimensions()
{
    if (!successProcessing) return;

    // The actual padding width is twice as large as the gridStep
    // to ensure we have enough space to evaluate the grid without having to care for boundaries
    int w = 2 * gridStep;

    // Output contains left, bottom, right, top pad widths, and overall dimensions
    padDims.clear();
    pad_r = w; // same as padWidth w unless +1 to make even dimension
    pad_t = w;
    pad_l = w;
    pad_b = w;

    if (padMode == "normal") {
        // make an image with even dimensions to avoid unclear quadrant swapping (if we FFT the padded image elsewhere)
        if ( naxis1 % 2 != 0) pad_r++;
        if ( naxis2 % 2 != 0) pad_t++;
        n_pad = naxis1 + pad_l + pad_r;
        m_pad = naxis2 + pad_b + pad_t;
    }
    else {   // padMode = "dyadic"
        // pad first, then make dyadic image size
        n_pad = pow(2, ceilf( logf(naxis1+2*w) / logf(2.)));
        m_pad = pow(2, ceilf( logf(naxis2+2*w) / logf(2.)));
        pad_l = (n_pad - naxis1) / 2;
        pad_b = (m_pad - naxis2) / 2;
        pad_r = n_pad - naxis1 - pad_l;
        pad_t = m_pad - naxis2 - pad_b;
    }
}

/*
// The following code pads the image with data. However, we don't need that.
// The grid positions in the padded data are filled using known grid points inside the image.
// Code is kept in case I need it in the future



// Pads an image with a given border width. If mode="dyadic", expands it to the next largest power of 2
// We use twice the grid step so that we have sufficient space to comfortably place a grid over the image including the boundaries.
// By making it twice as large, we don't need to introduce boundary conditions when evaluating histograms outside the nominal image area.
void MyImage::padImage()
{
    // resize the vector correspondingly
    data_padded.resize(n_pad*m_pad);
    mask_padded.fill(true, n_pad*m_pad);  // everything is masked per default, then unmask the true image area

    long n = naxis1;
    long m = naxis2;
    // copy image to center of padded image
    for (long j=pad_b; j<m+pad_b; ++j) {
        for (long i=pad_l; i<n+pad_l; ++i) {
            data_padded[i+n_pad*j] = dataCurrent[(i-pad_l)+n*(j-pad_b)];
            mask_padded[i+n_pad*j] = globalMask[(i-pad_l)+n*(j-pad_b)];
        }
    }

    int width = 100;  // size of the moving window (in both dimensions)

    // pad edges
    padEdgeLR(width, "l", 0, pad_l, pad_b, m+pad_b);
    padEdgeLR(width, "r", n+pad_l, n_pad, pad_b, m+pad_b);
    padEdgeBT(width, "b", pad_l, n+pad_l, 0, pad_b);
    padEdgeBT(width, "t", pad_l, n+pad_l, m+pad_b, m_pad);

    // pad corners
    padCorner(width, "ll", 0, pad_l, 0, pad_b);
    padCorner(width, "lr", n+pad_l, n_pad, 0, pad_b);
    padCorner(width, "ul", 0, pad_l, m+pad_b, m_pad);
    padCorner(width, "ur", n+pad_l, n_pad, m+pad_b, m_pad);
}

// This function provides a small correction when approaching the lower and upper boundary of an array
// with a sliding window. The size of the sliding window is maintained;
// Example:
// i=0:   0 1 2 3 4
// i=1:   0 1 2 3 4
// i=2:   0 1 2 3 4
// i=3:   1 2 3 4 5
// i=4:   2 3 4 5 6
// i=5:   3 4 5 6 7
// i=6:   4 5 6 7 8
// i=7:   5 6 7 8 9
// i=8:   5 6 7 8 9
// i=9:   5 6 7 8 9
int MyImage::padCorr(int i, int w, int max)
{
    if (w-i>0) return w-i;
    else if (w+i>=max) return max-w-i-1;
    else return 0;
}

// Calculate a local moving median around the left and right edge
void MyImage::padEdgeLR(int width, QString edge, long ipadmin, long ipadmax, long jpadmin, long jpadmax)
{
    long n = naxis1;
    long m = naxis2;

    long imin;
    long imax;
    if (edge == "l") {
        imin = 0;
        imax = width;
    }
    else if (edge == "r") {
        imin = n-width;
        imax = n;
    }
    else return;

    // Containers for the median edge values, the mask, and the sample
    QVector<float> edgeData;
    QVector<bool> edgeMask;
    QVector<float> sample;
    edgeData.reserve(m);
    edgeMask.reserve(m);
    sample.reserve(width*width);

    // Calculate the padded values
    int wh = width / 2;
    int step = 3; // accelerator (if larger than 1; not implemented in GUI)
    for (long j=0; j<m; ++j) {                                                       // run up the vertical edge
        for (long jj=j-wh+padCorr(j,wh,m); jj<=j+wh+padCorr(j,wh,m); jj+=step) {   // local samples along NAXIS2
            for (long i=imin; i<imax; i+=step) {                                         // local samples along NAXIS1
                if (!globalMask[i+n*jj]) sample.push_back(dataCurrent[i+n*jj]);
            }
        }
        edgeData.push_back(straightMedian_T(sample));
        if (sample.length() == 0) edgeMask.push_back(true);
        else edgeMask.push_back(false);
        sample.clear();
    }

    // Update the padded image
    for (long j=jpadmin; j<jpadmax; ++j) {
        for (long i=ipadmin; i<ipadmax; ++i) {
            data_padded[i+n_pad*j] = edgeData[j-pad_b];
            mask_padded[i+n_pad*j] = edgeMask[j-pad_b];
        }
    }
}

// Calculate a local moving median around the bottom and top edge
void MyImage::padEdgeBT(int width, QString edge, long ipadmin, long ipadmax, long jpadmin, long jpadmax)
{
    long n = naxis1;
    long m = naxis2;

    long jmin;
    long jmax;
    if (edge == "b") {
        jmin = 0;
        jmax = width;
    }
    else if (edge == "t") {
        jmin = m-width;
        jmax = m;
    }
    else return;

    // Containers for the median edge values, the mask, and the sample
    QVector<float> edgeData;
    QVector<bool> edgeMask;
    QVector<float> sample;
    edgeData.reserve(n);
    edgeMask.reserve(n);
    sample.reserve(width*width);

    // Calculate the padded values
    int wh = width / 2;
    int step = 3; // accelerator (if larger than 1; sparse sampling)
    for (long i=0; i<n; ++i) {                                                     // run along the horizontal edge
        for (long ii=i-wh+padCorr(i,wh,n); ii<i+wh+padCorr(i,wh,n); ii+=step) {    // local samples along NAXIS1
            for (long j=jmin; j<jmax; j+=step) {                                   // local samples along NAXIS2
                if (!globalMask[ii+n*j]) sample.push_back(dataCurrent[ii+n*j]);
            }
        }
        edgeData.push_back(straightMedian_T(sample));
        if (sample.length() == 0) edgeMask.push_back(true);
        else edgeMask.push_back(false);
        sample.clear();
    }

    // Update the padded image
    for (long j=jpadmin; j<jpadmax; ++j) {
        for (long i=ipadmin; i<ipadmax; ++i) {
            data_padded[i+n_pad*j] = edgeData[i-pad_l];
            mask_padded[i+n_pad*j] = edgeMask[i-pad_l];
        }
    }
}

// Calculate a local median for every image corner
void MyImage::padCorner(int width, QString corner, long ipadmin, long ipadmax, long jpadmin, long jpadmax)
{
    long n = naxis1;
    long m = naxis2;

    long imin = 0;
    long imax = 0;
    long jmin = 0;
    long jmax = 0;

    if (corner == "ll") {
        imin = 0;
        imax = width;
        jmin = 0;
        jmax = width;
    }
    else if (corner == "lr") {
        imin = n-width;
        imax = n;
        jmin = 0;
        jmax = width;
    }
    else if (corner == "ul") {
        imin = 0;
        imax = width;
        jmin = m-width;
        jmax = m;
    }
    else {  // ur
        imin = n-width;
        imax = n;
        jmin = m-width;
        jmax = m;
    }

    // Container for the statistical sample
    QVector<float> sample;
    sample.reserve(width*width);
    for (long j=jmin; j<jmax; ++j) {
        for (long i=imin; i<imax; ++i) {
            if (!globalMask[i+n*j]) sample.push_back(dataCurrent[i+n*j]);
        }
    }

    float cornerData = straightMedian_T(sample);
    bool cornerMask;
    if (sample.length() == 0) cornerMask = true;
    else cornerMask = false;

    // Update the padded image with the corner values
    for (long j=jpadmin; j<jpadmax; ++j) {
        for (long i=ipadmin; i<ipadmax; ++i) {
            data_padded[i+n_pad*j] = cornerData;
            mask_padded[i+n_pad*j] = cornerMask;
        }
    }
}
*/


// Calculate grid points for a (padded) image
// The grid "starts" gridSize inside each dimension (so we don't have to consider boundaries)
void MyImage::planGrid()
{
    if (!successProcessing) return;

    // Reset the grid
    grid.clear();

    // Number of grid points in each dimension and overall number of grid points
    n_grid = n_pad / gridStep;
    m_grid = m_pad / gridStep;

    // The first grid point in one dimension is located gridStep pixels in that dimension.
    // The last point must be at least half a gridStep away from the border
    // Some integer math is at work here
    if (n_pad - n_grid*gridStep < gridStep/2) n_grid--;
    if (m_pad - m_grid*gridStep < gridStep/2) m_grid--;

    nGridPoints = n_grid * m_grid;

    QVector<long> grid_x(nGridPoints);
    QVector<long> grid_y(nGridPoints);

    long count = 0;
    for (int j=1; j<=m_grid; ++j) {
        long jgrid = j*gridStep;
        for (int i=1; i<=n_grid; ++i) {
            long igrid = i*gridStep;
            grid_x[count] = igrid;     // points along NAXIS1
            grid_y[count] = jgrid;     // points along NAXIS2
            ++count;
        }
    }

    grid << grid_x << grid_y;
}

void MyImage::releaseBackgroundMemory(QString mode)
{
    emit setMemoryLock(true);
    if (minimizeMemoryUsage || mode == "entirely") {
        dataBackground.clear();
        dataBackground.squeeze();
        backgroundModelDone = false;
    }
    else {
        dataBackground_deletable = true;
    }

    grid.clear();
    grid.squeeze();
    backStatsGrid.clear();
    backStatsGrid.squeeze();
    rmsStatsGrid.clear();
    rmsStatsGrid.squeeze();
    emit setMemoryLock(false);
}

void MyImage::releaseBackgroundMemoryBackgroundModel()
{
    emit setMemoryLock(true);
    if (leftBackgroundWindow) {
        dataBackground.clear();
        dataBackground.squeeze();
        grid.clear();
        grid.squeeze();
        backStatsGrid.clear();
        backStatsGrid.squeeze();
        rmsStatsGrid.clear();
        rmsStatsGrid.squeeze();
        backgroundModelDone = false;
    }
    emit setMemoryLock(false);
}

void MyImage::subtractBackgroundModel()
{
    if (!successProcessing) return;

    long i=0;
    for (auto &pixel : dataCurrent) {
        if (!globalMask.isEmpty()) {
            if (!globalMask[i]) pixel -= dataBackground[i];
        }
        else {
            pixel -= dataBackground[i];
        }
        ++i;
    }
}
