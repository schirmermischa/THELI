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

// THELI internal source detection, (mostly) using the definitions and nomenclature from
// https://sextractor.readthedocs.io/en/latest/index.html

#include "detectedobject.h"

#include <cmath>
#include "functions.h"
#include <QVector>

#include "wcs.h"
#include "wcshdr.h"


//  ========  WARNING  ==========================================
//
// Image position parameters X, Y, XWIN and YWIN are zero-indexed
// (must add +1 to be consistent with FITS convention)
//
// ==============================================================

DetectedObject::DetectedObject(const QList<long> &objectIndices, const QVector<float> &data, const QVector<float> &background, const QVector<float> &weight,
                               const QVector<bool> &_mask, bool weightinmemory, const long nax1, const long nax2, const long objid,
                               const float satVal, const float gainval, wcsprm &wcsImage, QObject *parent) : QObject(parent),
    dataMeasure(data),
    dataBackground(background),
    dataWeight(weight),
    mask(_mask),
    weightInMemory(weightinmemory),
    wcs(wcsImage),
    saturationValue(0.98*satVal),    // 98%: some margin against effective saturation, which might occur at slightly lower values already
    gain(gainval)                    // GAIN is 1.0 always as we convert ADU to electrons during HDU reformatting, already. Kept for clarity
{
    area = objectIndices.length();
    objID = objid;
    pixels_x.reserve(area);
    pixels_y.reserve(area);
    pixels_flux.reserve(area);
    pixels_back.reserve(area);
    pixels_weight.reserve(area);
    naxis1 = nax1;
    naxis2 = nax2;
    bitflags.resize(16);

    /* bit flags
    0: at least 10% of object area is masked
    1: object has been deblended (not implemented)
    2: at least one pixel is saturated
    3: the isophotal footprint of the detected object is truncated (too close to an image boundary)
    4: the windowed footprint of the detected object is truncated (too close to an image boundary)
    5: at least one photometric aperture has masked pixels or is truncated by the image boundary or has zero weight pixels
    6: at least one pixel has zero weight
    7: spurious or corrupted detection. Could be e.g. a bad column
    8:
    */

    // copy the pixel position and pixel value for all pixels comprising this object
    for (auto &index : objectIndices) {
        pixels_x.append(index % naxis1);
        pixels_y.append(index / naxis1);
        pixels_flux.append(data.at(index));
        pixels_back.append(background.at(index));
        if (weightInMemory) pixels_weight.append(weight.at(index));
        else pixels_weight.append(1.0);
    }
}

DetectedObject::~DetectedObject()
{
    remove();
}

void DetectedObject::remove()
{
    pixels_flux.clear();
    pixels_back.clear();
    pixels_weight.clear();
    pixelsAper_back.clear();
    pixelsAper_flux.clear();
    pixelsAper_weight.clear();
    pixelsWin_back.clear();
    pixelsWin_flux.clear();
    pixelsWin_weight.clear();
    pixelsWin_x.clear();
    pixelsWin_y.clear();
    apertures.clear();
    pixelsAper_x.clear();
    pixelsAper_y.clear();
    pixels_x.clear();
    pixels_y.clear();

    pixels_flux.squeeze();
    pixels_back.squeeze();
    pixels_weight.squeeze();
    pixels_x.squeeze();
    pixels_y.squeeze();
    pixelsAper_back.squeeze();
    pixelsAper_flux.squeeze();
    pixelsAper_weight.squeeze();
    pixelsWin_back.squeeze();
    pixelsWin_flux.squeeze();
    pixelsWin_weight.squeeze();
    pixelsWin_x.squeeze();
    pixelsWin_y.squeeze();
    apertures.squeeze();
    pixelsAper_x.squeeze();
    pixelsAper_y.squeeze();
}

void DetectedObject::computeObjectParams()
{
    // Must be done in a specific order

    // ISOPHOTAL
    calcFlux();
    calcMoments();
    calcVariance();
    calcEllipticity();
    calcMomentsErrors();
    calcApertureMagnitudes();

    // WINDOWED
    calcFluxRadius();
    calcFWHM();
    calcMagAuto();
    calcWindowedMoments();
    calcWindowedEllipticity();
    calcWindowedMomentsErrors();
    filterSpuriousDetections();

    // Applied only when writing catalogs to disk for scamp
    // correctOriginOffset();
    calcSkyCoords();
}

void DetectedObject::calcFlux()
{
    if (badDetection) return;

    FLUX_ISO = std::accumulate(pixels_flux.begin(), pixels_flux.end(), .0);
    if (FLUX_ISO < 0.) {
        bitflags.setBit(7,true);
        badDetection = true;
        return;
    }
    MAG_ISO = -2.5*log10(FLUX_ISO) + ZP;
}

QVector<double> DetectedObject::calcFluxAper(float aperture)
{
    // get aperture pixels
    long xminAper = floor(X-0.5*aperture);
    long xmaxAper = ceil(X+0.5*aperture);
    long yminAper = floor(Y-0.5*aperture);
    long ymaxAper = ceil(Y+0.5*aperture);

    // Check truncation by image frame
    if (isTruncated(xminAper, xmaxAper, yminAper, ymaxAper)) bitflags.setBit(5,true);

    // Truncate if necessary
    xminAper = xminAper < 0 ? 0 : xminAper;
    xmaxAper = xmaxAper >=naxis1 ? naxis1-1 : xmaxAper;
    yminAper = yminAper < 0 ? 0 : yminAper;
    ymaxAper = ymaxAper >=naxis2 ? naxis2-1 : ymaxAper;

    long npixAper = (xmaxAper-xminAper+1) * (ymaxAper-yminAper+1);
    pixelsAper_flux.resize(npixAper);
    pixelsAper_back.resize(npixAper);
    pixelsAper_weight.fill(1.0, npixAper);     // fill vector with dummy weight (if weight map is not available, e.g. during background object masking)
    pixelsAper_x.resize(npixAper);
    pixelsAper_y.resize(npixAper);
    long k = 0;
    for (long j=yminAper; j<=ymaxAper; ++j) {
        for (long i=xminAper; i<=xmaxAper; ++i) {
            pixelsAper_flux[k] = dataMeasure.at(i+naxis1*j);
            pixelsAper_back[k] = dataBackground.at(i+naxis1*j);
            if (weightInMemory) pixelsAper_weight[k] = dataWeight.at(i+naxis1*j);           // update weight if weight map is available
            pixelsAper_x[k] = i;
            pixelsAper_y[k] = j;
            ++k;
        }
    }

    // We use a 4 times sub-sampled grid for finer resolution for compact objects smaller than 10 pixels
    int s = 0;
    if (XMAX-XMIN <= 10 && YMAX - YMIN <= 10) s = 4;
    else s = 1;

    // The measurement window is defined by the aperture
    long nn = (xmaxAper-xminAper+1) * s;
    long mm = (ymaxAper-yminAper+1) * s;
    QVector<double> dataSub(nn*mm, 0);
    QVector<double> backSub(nn*mm, 0);
    QVector<double> weightSub(nn*mm, 0);

    // Subsample the original pixel data. The window is the minimum rectangular envelope
    long dim = pixelsAper_flux.length();
    for (long k=0; k<dim; ++k) {
        double flux = pixelsAper_flux.at(k);
        double back = pixelsAper_back.at(k);
        double weight = pixelsAper_weight.at(k);
        double spx = s*(pixelsAper_x.at(k)-xminAper);
        double spy = s*(pixelsAper_y.at(k)-yminAper);
        for (int ss1=0; ss1<s; ++ss1) {
            long jj = spy + ss1;
            for (int ss2=0; ss2<s; ++ss2) {
                long ii = spx + ss2;
                dataSub[ii+nn*jj] = flux;
                backSub[ii+nn*jj] = back;
                weightSub[ii+nn*jj] = weight;
            }
        }
    }

    // The object centroid in the subsampled coordinate system
    double xcen = (X-xminAper) * double(s);
    double ycen = (Y-yminAper) * double(s);

    double fluxAper = 0.;
    double fluxErrAper = 0.;
    for (long j=0; j<=ycen*2; ++j) {
        double jj = double(j);
        for (long i=0; i<=xcen*2; ++i) {
            double ii = double(i);
            if (ii<0 || ii>=nn || jj<0 || jj>=mm) continue;
            double rsq = (xcen-ii)*(xcen-ii) + (ycen-jj)*(ycen-jj);
            if (rsq <= 0.25*aperture*aperture*s*s) {
                fluxAper += dataSub[ii+nn*jj];
                fluxErrAper += backSub[ii+nn*jj]/gain + dataSub[ii+nn*jj]/gain;
                if (weightSub[ii+nn*jj] == 0.) bitflags.setBit(5,true);
                if (dataSub[ii+nn*jj] > saturationValue) bitflags.setBit(2,true);
                dataSub[ii+nn*jj] = 0.;     // just making sure we don't count a pixel twice
            }
        }
    }

    // correct for subsampling of aperture
    fluxAper = fluxAper / (s*s);
    fluxErrAper = fluxErrAper / (s*s);

    fluxErrAper = sqrt(fluxErrAper);
    double magAper = -2.5*log10(fluxAper) + ZP;
    double magErrAper = 99.;
    if (fluxAper > 0.) magErrAper = 2.5*log10(1.+fluxErrAper/fluxAper);
    QVector<double> result;
    result << fluxAper << fluxErrAper << magAper << magErrAper;
    if ((fluxAper < 0. || std::isnan(magErrAper))) {
        bitflags.setBit(7,true);
        badDetection = true;
    }

    return result;
}

void DetectedObject::calcApertureMagnitudes()
{
    if (badDetection) return;

    if (apertures.isEmpty()) return;

    FLUX_APER.reserve(apertures.length());
    MAG_APER.reserve(apertures.length());
    FLUXERR_APER.reserve(apertures.length());
    MAGERR_APER.reserve(apertures.length());

    QVector<double> result;
    for (auto &aperture : apertures) {
        result = calcFluxAper(aperture);
        FLUX_APER.append(result[0]);
        FLUXERR_APER.append(result[1]);
        MAG_APER.append(result[2]);
        MAGERR_APER.append(result[3]);
    }
}

void DetectedObject::calcMoments()
{
    if (badDetection) return;

    double xsum = 0.;
    double ysum = 0.;
    double xxsum = 0.;
    double yysum = 0.;
    double xysum = 0.;
    double xysumsq = 0.;
    for (long i=0; i<area; ++i) {
        double pi = pixels_flux.at(i);
        double px = pixels_x.at(i);
        double py = pixels_y.at(i);
        if (pi >= saturationValue) bitflags.setBit(2,true);
        if (pixels_weight.at(i) == 0.) bitflags.setBit(6,true);
        xsum += px*pi;
        ysum += py*pi;
        xxsum += px*px*pi;
        yysum += py*py*pi;
        xysum += px*py*pi;
        xysumsq += px*px*py*py*pi;
    }
    X = xsum / FLUX_ISO;
    Y = ysum / FLUX_ISO;

    if (X < 0.
            || X > naxis1
            || Y < 0.
            || Y > naxis2) {
        bitflags.setBit(7,true);
        badDetection = true;
        return;
    }

    X2 = xxsum / FLUX_ISO - X*X;
    Y2 = yysum / FLUX_ISO - Y*Y;
    XY = xysum / FLUX_ISO - X*Y;
    X2Y2 = xysumsq / FLUX_ISO - X*X*Y*Y;

    // Handling of infinitely thin objects (pathological cases, such as bad columns)
    double rho = 1/12.;
    if (X2*Y2 - XY*XY < rho*rho) {
        bitflags.setBit(7,true);
        badDetection = true;
        return;
    }

    XMIN = minVec_T(pixels_x);
    XMAX = maxVec_T(pixels_x);
    YMIN = minVec_T(pixels_y);
    YMAX = maxVec_T(pixels_y);

    double s1 = (X2+Y2) / 2.;
    double s2 = sqrt((X2-Y2) * (X2-Y2) / 4. + XY*XY);

    if (s1<s2) {
        bitflags.setBit(7,true);
        badDetection = true;
    }
}

// weighted variance
// https://www.itl.nist.gov/div898/software/dataplot/refman2/ch2/weighvar.pdf
void DetectedObject::calcVariance()
{
    if (badDetection) return;

    double xsum = 0.;
    double ysum = 0.;
    double psum = 0.;
    double n_nonzero = 0;
    if (area>1) {
        for (long i=0; i<area; ++i) {
            double pi = pixels_flux.at(i);
            double px = pixels_x.at(i);
            double py = pixels_y.at(i);
            xsum += pi*pow(px - X,2);
            ysum += pi*pow(py - Y,2);
            if (pi!= 0.) ++n_nonzero;
            psum += pi;
        }
        XVAR = xsum / ( (n_nonzero-1.) / n_nonzero * psum);
        YVAR = ysum / ( (n_nonzero-1.) / n_nonzero * psum);
    }
    else {
        XVAR = 1.;
        YVAR = 1.;
    }
}

void DetectedObject::calcWindowedMoments()
{
    if (badDetection) return;

    getWindowedPixels();

    XWIN = X;
    YWIN = Y;
    if (FLUX_RADIUS>10.) return;  // Windowed parameters useful for small objects, only

    double dsq = 4.*FLUX_RADIUS*FLUX_RADIUS / (8.*log(2.));
    double rmaxsq = 4.*FLUX_RADIUS*FLUX_RADIUS;
    double eps = 2e-4;
    long iter = 0;
    long iterMax = 10;   // verified that this converges indeed within less than 10 iterations, even for oddities like cosmics
    double diff = 1.;

    // Iterate to obtain first moments
    while (iter < iterMax && diff > eps) {
        double xwsum = 0.;
        double ywsum = 0.;
        double wsum = 0.;
        double XWIN0 = XWIN;
        double YWIN0 = YWIN;
        long i=0;
        for (auto &pixel : pixelsWin_flux) {
            double rsq = pow((pixelsWin_x.at(i) - XWIN0),2.) + pow((pixelsWin_y.at(i) - YWIN0),2.);
            if (rsq < rmaxsq) {
                double w = exp(-rsq / (2.*dsq));
                xwsum += w*pixel*(pixelsWin_x.at(i)-XWIN0);
                ywsum += w*pixel*(pixelsWin_y.at(i)-YWIN0);
                wsum += w*pixel;
            }
            ++i;
        }
        XWIN = XWIN0 + 2.*xwsum/wsum;      // didn't have the prefactor 2, but doesn't seem to make a difference; probably faster conversion like this
        YWIN = YWIN0 + 2.*xwsum/wsum;
        diff = sqrt( pow(XWIN-XWIN0,2) + pow(YWIN-YWIN0,2));
        ++iter;
    }

    if (XWIN < 0.
            || XWIN > naxis1
            || YWIN < 0.
            || YWIN > naxis2) {
        bitflags.setBit(7,true);
        badDetection = true;
        return;
    }

    // Second moments
    double xxwsum = 0.;
    double yywsum = 0.;
    double xywsum = 0.;
    double wsum = 0.;
    long i=0;
    for (auto &pixel : pixelsWin_flux) {
        double px = pixelsWin_x.at(i);
        double py = pixelsWin_y.at(i);
        double rsq = pow((px - XWIN),2.) + pow((py - YWIN),2.);
        if (rsq < rmaxsq) {
            double w = exp(-rsq / (2.*dsq));
            xxwsum += w*pixel*(px-XWIN)*(px-XWIN);
            yywsum += w*pixel*(py-YWIN)*(py-YWIN);
            xywsum += w*pixel*(px-XWIN)*(py-YWIN);
            wsum += w*pixel;
        }
        ++i;
    }
    X2WIN = xxwsum / wsum;
    Y2WIN = yywsum / wsum;
    XYWIN = xywsum / wsum;
}

void DetectedObject::calcMomentsErrors()
{
    if (badDetection) return;

    double xsum = 0.;
    double ysum = 0.;
    double xysum = 0.;
    double psum = 0.;
    for (long i=0; i<area; ++i) {
        double pi = pixels_flux.at(i);
        double px = pixels_x.at(i);
        double py = pixels_y.at(i);
        double sisq = sigma_back*sigma_back + pi / gain;
        xsum += sisq*(px-X)*(px-X);
        ysum += sisq*(py-Y)*(py-Y);
        xysum += sisq*(px-X)*(py-Y);
        psum += pi;
    }
    ERRX2 = xsum / (psum*psum);
    ERRY2 = ysum / (psum*psum);
    ERRXY = xysum / (psum*psum);

    ERRA = 0.5*(ERRX2+ERRY2) + sqrt( 0.25*(ERRX2-ERRY2)*(ERRX2-ERRY2) + ERRXY*ERRXY);
    ERRB = 0.5*(ERRX2+ERRY2) - sqrt( 0.25*(ERRX2-ERRY2)*(ERRX2-ERRY2) + ERRXY*ERRXY);
    ERRA = sqrt(ERRA);
    ERRB = sqrt(ERRB);

    ERRTHETA = 2.*ERRXY / (ERRX2-ERRY2);
    ERRTHETA = fabs(0.5*atan(ERRTHETA)/rad);

    // noise floor
    if (ERRTHETA < 0.01) ERRTHETA = 0.01;
}

void DetectedObject::calcWindowedMomentsErrors()
{
    if (badDetection) return;

    if (FLUX_RADIUS>10.) return;  // Windowed parameters useful for small objects, only

    double dsq = 4.*FLUX_RADIUS*FLUX_RADIUS / (8.*log(2.));
    double rmaxsq = 4.*FLUX_RADIUS*FLUX_RADIUS;
    double xwsum = 0.;
    double ywsum = 0.;
    double xywsum = 0.;
    double wsum = 0.;
    long i=0;
    for (auto &pixel : pixelsWin_flux) {
        double px = pixelsWin_x.at(i);
        double py = pixelsWin_y.at(i);
        double rsq = pow((px - XWIN),2.) + pow((py - YWIN),2.);
        double sisq = sigma_back*sigma_back + pixel / gain;  // I think this is wrong, should divide sigma_back by gain, too!
        if (rsq < rmaxsq) {
            double w = exp(-rsq / (2.*dsq));
            xwsum += w*w*sisq*(px-XWIN)*(px-XWIN);
            ywsum += w*w*sisq*(py-YWIN)*(py-YWIN);
            xywsum += w*w*sisq*(px-XWIN)*(py-YWIN);
            wsum += w*w*pixel*pixel;
        }
        ++i;
    }

    if (wsum  > 0.) {
        ERRX2WIN = 4.*xwsum/wsum;
        ERRY2WIN = 4.*ywsum/wsum;
        ERRXYWIN = 4.*xywsum/wsum;
        ERRAWIN = 0.5*(ERRX2WIN+ERRY2WIN) + sqrt( 0.25*(ERRX2WIN-ERRY2WIN)*(ERRX2WIN-ERRY2WIN) + ERRXYWIN*ERRXYWIN);
        ERRBWIN = 0.5*(ERRX2WIN+ERRY2WIN) - sqrt( 0.25*(ERRX2WIN-ERRY2WIN)*(ERRX2WIN-ERRY2WIN) + ERRXYWIN*ERRXYWIN);
        ERRAWIN = sqrt(ERRAWIN);
        ERRBWIN = sqrt(ERRBWIN);
        ERRTHETAWIN = 2.*ERRXYWIN / (ERRX2WIN-ERRY2WIN);
        ERRTHETAWIN = fabs(0.5*atan(ERRTHETAWIN)/rad);
    }
    else {
        ERRX2WIN = 0.;
        ERRY2WIN = 0.;
        ERRXYWIN = 0.;
        ERRAWIN = 0.;
        ERRBWIN = 0.;
        ERRTHETAWIN = 0.;
        bitflags.setBit(7,true);
        badDetection = true;
    }

    // noise floor
    if (ERRTHETAWIN < 0.01) ERRTHETAWIN = 0.01;
}

void DetectedObject::calcSkyCoords()
{
    if (badDetection) return;

    double world[2];
    double phi;
    double theta;
    double imgcrd[2];
    double pixcrd[2];
    // CAREFUL! wcslib starts pixels counting at 1, hence must add +1 to zero-indexed C++ vectors
    pixcrd[0] = XWIN + 1.;
    pixcrd[1] = YWIN + 1.;
    int stat[1];
    wcsp2s(&wcs, 1, 2, pixcrd, imgcrd, &phi, &theta, world, stat);
    ALPHA_J2000 = world[0];
    DELTA_J2000 = world[1];
}

void DetectedObject::calcEllipticity()
{
    //  if (badDetection) return;

    if (XY == 0. || X2 == Y2) THETA = 0.;
    else {
        THETA = atan(2.*XY / (X2-Y2)) / 2. / rad;
        // THETA should have the same sign as XY
        if ( (THETA<0 && XY>0)
             || (THETA>0 && XY<0)) THETA *= -1.;
    }

    double s1 = (X2+Y2) / 2.;
    double s2 = sqrt((X2-Y2) * (X2-Y2) / 4. + XY*XY);
    if (s1<s2) {
        bitflags.setBit(7,true);
        badDetection = true;
        //        return;
    }

    A = sqrt(s1+s2);
    B = sqrt(s1-s2);

    XERR = sqrt( pow(A*cos(THETA*rad), 2) + pow(B*sin(THETA*rad), 2));
    YERR = sqrt( pow(A*sin(THETA*rad), 2) + pow(B*cos(THETA*rad), 2));

    // This is what the printed source extractor documentation says (v. 2.13), but not the online doc https://sextractor.readthedocs.io/ (The latter seems wrong)
    CXX = Y2 / s2;
    CYY = X2 / s2;
    CXY = -2. * XY / s2;

    ELLIPTICITY = 1. - B/A;
}

void DetectedObject::calcWindowedEllipticity()
{
    if (badDetection) return;

    if (XYWIN == 0. || X2WIN == Y2WIN) THETAWIN = 0.;
    else {
        THETAWIN = atan(2.*XYWIN / (X2WIN-Y2WIN)) / 2. / rad;
        // THETA should have the same sign as XY
        if ( (THETAWIN<0 && XYWIN>0)
             || (THETAWIN>0 && XYWIN<0)) THETAWIN *= -1.;
    }

    double s1 = (X2WIN+Y2WIN) / 2.;
    double s2 = sqrt((X2WIN-Y2WIN) * (X2WIN-Y2WIN) / 4. + XYWIN*XYWIN);
    if (s1<s2) {
        bitflags.setBit(7,true);
        badDetection = true;
        return;
    }

    AWIN = sqrt(s1+s2);
    BWIN = sqrt(s1-s2);

    XWINERR = sqrt( pow(AWIN*cos(THETAWIN*rad), 2) + pow(BWIN*sin(THETAWIN*rad), 2));
    YWINERR = sqrt( pow(AWIN*sin(THETAWIN*rad), 2) + pow(BWIN*cos(THETAWIN*rad), 2));

    CXXWIN = Y2WIN / s2;
    CYYWIN = X2WIN / s2;
    CXYWIN = -2.*XYWIN / s2;
}

void DetectedObject::calcFluxRadius()
{
    if (badDetection) return;

    if (FLUX_ISO == 0.) return;

    // We use a 5 times sub-sampled grid for finer resolution
    // for compact objects smaller than 10 pixels, and 3 times sub-sampled if smaller than 20 pixels
    int s = 0;
    if (XMAX-XMIN <= 10 && YMAX - YMIN <= 10) s = 5;
    else if (XMAX-XMIN <= 20 && YMAX - YMIN <= 20) s = 3;
    else if (XMAX-XMIN>100 || YMAX-YMIN > 100) {
        // don't calculate flux radius for very large objects (algorithm is very inefficient)
        FLUX_RADIUS = sqrt((XMAX-XMIN)*(XMAX-XMIN) + (YMAX-YMIN)*(YMAX-YMIN));
        return;
    }
    else s = 1;

    long nn = (XMAX-XMIN+1) * s;
    long mm = (YMAX-YMIN+1) * s;

    QVector<double> dataSub(nn*mm, 0);
    // Subsample the original pixel data. The window is the minimum rectangular envelope
    for (long k=0; k<area; ++k) {
        double flux = pixels_flux[k];
        double spx = s*(pixels_x[k]-XMIN);
        double spy = s*(pixels_y[k]-YMIN);
        for (int ss1=0; ss1<s; ++ss1) {
            long jj = spy + ss1;
            for (int ss2=0; ss2<s; ++ss2) {
                long ii = spx + ss2;
                dataSub[ii+nn*jj] = flux;
            }
        }
    }

    // The object centroid in the subsampled coordinate system
    double xcen = (X-XMIN) * double(s);
    double ycen = (Y-YMIN) * double(s);

    // Integrate outwards until we reach 50% of the total flux
    // Really inefficient, because we do the same calculations again and again.
    double fluxInt = 0.;
    double step = 1.;
    double maxStep = sqrt(nn*nn+mm*mm) / 2.; // to avoid runaway loop (pathological cases, not that I know what that would be)
    /*
                              while (fluxInt < 0.5*flux*s*s && step < maxStep) {
                                  double r = step;
                                  fluxInt = 0.;
                                  for (long jj=0; jj<mm; ++jj) {
                                      double dy2 = (ycen - jj)*(ycen-jj);
                                      for (long ii=0; ii<nn; ++ii) {
                                          double dx2 = (xcen - ii)*(xcen-ii);
                                          if (dx2+dy2 <= r*r) fluxInt += dataSub[ii+nn*jj];
                                      }
                                  }
                                  FLUX_RADIUS = r;
                                  ++step;
                              }
                              */

    FLUX_RADIUS = 0.;
    long ixcen = long(xcen);
    long iycen = long(ycen);
    double flux_previous = 0.;
    double flux_current = 0.;
    while (fluxInt < 0.5*FLUX_ISO*s*s && step < maxStep) {
        // from the centroid, stepping outside in quadratic "rings"
        flux_previous = flux_current;
        for (long jj=iycen-step; jj<=iycen+step; ++jj) {
            for (long ii=ixcen-step; ii<=ixcen+step; ++ii) {
                if (jj == iycen-step
                        || jj == iycen+step
                        || ii == ixcen-step
                        || ii == ixcen+step) {     // running along horizontal border
                    if (ii<0 || ii>=nn || jj<0 || jj>=mm) continue;
                    fluxInt += dataSub[ii+nn*jj];
                    dataSub[ii+nn*jj] = 0.;     // making sure we don't count a pixel twice
                }
            }
        }
        flux_current = fluxInt;
        FLUX_RADIUS = step;
        ++step;
    }
    FLUX_RADIUS /= double(s);

    // Interpolate 0.1 pixel step size
    double intp = (0.5-flux_previous) / (flux_current - 0.5) / s;
    FLUX_RADIUS += intp;
}

void DetectedObject::calcFWHM()
{
    //    FWHM = 1.995*FLUX_RADIUS; // For a theoretical 2D gaussian
    FWHM = 2.355*sqrt(XVAR+YVAR);
}

void DetectedObject::getWindowedPixels()
{
    if (badDetection) return;

    long xminWin = floor(X-2.*FLUX_RADIUS);
    long xmaxWin = ceil(X+2.*FLUX_RADIUS);
    long yminWin = floor(Y-2.*FLUX_RADIUS);
    long ymaxWin = ceil(Y+2.*FLUX_RADIUS);

    // Check truncation by image frame
    if (isTruncated(xminWin, xmaxWin, yminWin, ymaxWin)) bitflags.setBit(4,true);

    // Truncate if necessary
    xminWin = xminWin < 0 ? 0 : xminWin;
    xmaxWin = xmaxWin >=naxis1 ? naxis1-1 : xmaxWin;
    yminWin = yminWin < 0 ? 0 : yminWin;
    ymaxWin = ymaxWin >=naxis2 ? naxis2-1 : ymaxWin;

    long npixWin = (xmaxWin-xminWin+1) * (ymaxWin-yminWin+1);
    pixelsWin_flux.reserve(npixWin);
    pixelsWin_x.reserve(npixWin);
    pixelsWin_y.reserve(npixWin);
    for (long j=yminWin; j<=ymaxWin; ++j) {
        double jj = double(j);
        for (long i=xminWin; i<=xmaxWin; ++i) {
            double ii = double(i);
            double rsq = (X-ii)*(X-ii) + (Y-jj)*(Y-jj);
            if (rsq <= 4.*FLUX_RADIUS*FLUX_RADIUS) {
                if (weightInMemory && dataWeight.at(i+naxis1*j) == 0.) {
                    bitflags.setBit(6,true);
                    continue;
                }
                pixelsWin_flux.append(dataMeasure.at(i+naxis1*j));
                pixelsWin_x.append(i);
                pixelsWin_y.append(j);
            }
        }
    }
}

bool DetectedObject::isTruncated(const long xmin, const long xmax, const long ymin, const long ymax)
{
    bool truncation = false;
    if (xmin < 0) truncation = true;
    if (ymin < 0) truncation = true;
    if (xmax >= naxis1) truncation = true;
    if (ymax >= naxis2) truncation = true;

    return truncation;
}

void DetectedObject::calcMagAuto()
{
    if (badDetection) return;

    // TODO: check that this parameterisation actually encompasses 6 times the best fit isophote
    long imin = floor(X - 6.*A);
    long imax = ceil(X + 6.*A);
    long jmin = floor(Y - 6.*A);
    long jmax = ceil(Y + 6.*A);

    imin = imin < 0 ? 0 : imin;
    imax = imax >=naxis1 ? naxis1-1 : imax;
    jmin = jmin < 0 ? 0 : jmin;
    jmax = jmax >=naxis2 ? naxis2-1 : jmax;

    // Loop over the rectangular subset of pixels that encompass the object
    double rkron = 0.;
    double fsum = 0.;
    for (long j=jmin; j<=jmax; ++j) {
        float dy = j - Y;
        for (long i=imin; i<=imax; ++i) {
            float dx = i - X;
            // Work on pixels within 6x the ellipse
            double rsq = CXX*dx*dx + CYY*dy*dy + CXY*dx*dy;
            if (rsq <= 36.*A*A) {
                rkron += sqrt(rsq) * dataMeasure.at(i+naxis1*j);
                fsum += dataMeasure.at(i+naxis1*j);
            }
        }
    }

    /* debugging
    // objects close to the detection limit may not fulfil the "rsq <= 36.*A*A" requirement
    if (fsum == 0.) {
        for (long j=jmin; j<=jmax; ++j) {
            float dy = j - Y;
            for (long i=imin; i<=imax; ++i) {
                float dx = i - X;
                double rsq = CXX*dx*dx + CYY*dy*dy + CXY*dx*dy;
                qDebug() << i << j << dx << dy << CXX << CYY << CXY << rsq << 36.*A*A << dataMeasure.at(i+naxis1*j);
            }
        }
    }
    */

    if (fsum == 0.) rkron = 3.5;
    else rkron /= fsum;

    // enforce a minimum radius of 3.5 pixels for noisy objects
    rkron = rkron < 3.5 ? 3.5 : rkron;

    double auto_radius = 2.5*rkron;

    imin = floor(X - auto_radius);
    imax = ceil(X + auto_radius);
    jmin = floor(Y - auto_radius);
    jmax = ceil(Y + auto_radius);

    imin = imin < 0 ? 0 : int(imin);
    imax = imax >=naxis1 ? naxis1-1 : int(imax);
    jmin = jmin < 0 ? 0 : int(jmin);
    jmax = jmax >=naxis2 ? naxis2-1 : int(jmax);
    if (isTruncated(imin, imax, jmin, jmax)) bitflags.setBit(3,true);

    FLUX_AUTO = 0;
    FLUXERR_AUTO = 0;
    float numMasked = 0.;
    float numTot = 0.;
    for (long j=jmin; j<=jmax; ++j) {
        float dy = Y - j;
        for (long i=imin; i<=imax; ++i) {
            float dx = X - i;
            // Work on pixels within auto_radius
            if (dx*dx + dy*dy < auto_radius*auto_radius) {
                if (weightInMemory && dataWeight.at(i+naxis1*j) == 0.) bitflags.setBit(6,true);
                // With global mask
                if (globalMaskAvailable) {
                    if (!mask.at(i+naxis1*j)) {
                        FLUX_AUTO += dataMeasure.at(i+naxis1*j);
                        FLUXERR_AUTO += dataBackground.at(i+naxis1*j)/gain + dataMeasure.at(i+naxis1*j)/gain;   // neglecting readout noise
                    }
                    else {
                        ++numMasked;
                    }
                }
                // without global mask
                else {
                    FLUX_AUTO += dataMeasure.at(i+naxis1*j);
                    FLUXERR_AUTO += dataBackground.at(i+naxis1*j)/gain + dataMeasure.at(i+naxis1*j)/gain;   // neglecting readout noise
                }
                ++numTot;
            }
        }
    }

    if (FLUX_AUTO < 0.) {
        bitflags.setBit(7,true);
        badDetection = true;
        MAGERR_AUTO = 99.;
        MAG_AUTO = 99.;
        FLUXERR_AUTO = 0.;
    }
    else {
        FLUXERR_AUTO = sqrt(FLUXERR_AUTO);
        MAG_AUTO = -2.5*log10(FLUX_AUTO) + ZP;
        MAGERR_AUTO = 2.5*log10(1.+FLUXERR_AUTO/FLUX_AUTO);
    }

    if (numMasked / numTot > 0.1) bitflags.setBit(1,true);
}

void DetectedObject::filterSpuriousDetections()
{
    if (std::isnan(XWIN)
            || std::isnan(YWIN)
            || std::isnan(AWIN)
            || std::isnan(BWIN)
            || std::isnan(ERRAWIN)
            || std::isnan(ERRBWIN)
            || std::isnan(ERRTHETAWIN)
            || std::isnan(FLUX_RADIUS)
            || XWIN < 1.
            || YWIN < 1.
            || XWIN > naxis1-1
            || YWIN > naxis2-1
            || (AWIN==0 && BWIN==0)) {
        bitflags.setBit(7,true);
        badDetection = true;
    }

    FLAGS = 0;
    for (int i=0; i<bitflags.size(); ++i) {
        if (bitflags.testBit(i)) FLAGS += pow(2,i);
    }

    //        qDebug() << X << XWIN << Y << YWIN << AWIN << BWIN << ERRAWIN << ERRBWIN << ERRTHETAWIN << FLUX_RADIUS << ERRX2WIN << ERRY2WIN << ERRXYWIN << badDetection;

}

// In C++, arrays start counting a 0.
// However, for object detection in FITS images, we start counting at 1.
// Verified by comparing with Source Extractor positions

// UNUSED. Offset applied when writing source catalogs to disk for scamp
void DetectedObject::correctOriginOffset()
{
    X += 1.;
    Y += 1.;
    XWIN += 1.;
    YWIN += 1.;
}
