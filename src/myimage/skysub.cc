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

#include "../functions.h"
#include "myimage.h"

#include "wcs.h"

#include <gsl/gsl_vector.h>

#include <QDebug>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>

// subtract a polynomial fit
// UNUSED?
void MyImage::subtractPolynomialSkyFit(gsl_vector* c, int order)
{
    if (!successProcessing) return;

    QVector<double> ra;
    QVector<double> dec;
    QVector<double> radius;
    //    readSkyPositions(ra, dec, sky);
    //    calculateSkyFit(ra, dec, sky, order);
}

// Needed for polynomial fit
QVector<long> MyImage::locateSkyNode(const double alpha, const double delta, const double radius)
{
    double xpos = 0.;
    double ypos = 0.;
    sky2xy(alpha, delta, xpos, ypos);
    long xcen = xpos;
    long ycen = ypos;

    // maximally inscribe a square, of half width 'w' into the circular aperture
    long w = radius*0.7 / plateScale;
    if (w<1) return QVector<long>();

    QVector<long> aperture;
    long xmin = xcen - w < 0 ? 0 : xcen - w;
    long ymin = ycen - w < 0 ? 0 : ycen - w;
    long xmax = xcen + w >= naxis1 ? naxis1-1 : xcen + w;
    long ymax = ycen + w >= naxis2 ? naxis2-1 : ycen + w;

    long numPixels = (xmax-xmin+1)*(ymax-ymin+1);
    if (numPixels < 10) return QVector<long>();     // return if insufficient number of data points

    aperture << xmin << xmax << ymin << ymax;
    return aperture;
}

// Needed for polynomial fit and constant sky sub
void MyImage::evaluateSkyNodes(const QVector<double> alpha, const QVector<double> delta,
                               const QVector<double> radius)
{
    if (alpha.length() != delta.length()
            || alpha.length() != radius.length()
            || alpha.isEmpty()) {
        return;
    }

    skyPolyfitNodes.clear();
    for (long k=0; k<alpha.length(); ++k) {
        QVector<long> aperture = locateSkyNode(alpha[k], delta[k], radius[k]);
        if (aperture.isEmpty()) continue;
        QVector<float> sample;
        long xmin = aperture[0];
        long xmax = aperture[1];
        long ymin = aperture[2];
        long ymax = aperture[3];
        sample.reserve((xmax-xmin+1)*(ymax-ymin+1));
        for (long j=ymin; j<=ymax; ++j) {
            for (long i=xmin; i<=xmax; ++i) {
                if (globalMaskAvailable) {
                    if (!globalMask[i+naxis1*j]) sample.append(dataCurrent[i+naxis1*j]);
                }
                else {
                    sample.append(dataCurrent[i+naxis1*j]);
                }
            }
        }
        float sky;
        if (sample.length() < 10) continue;                          // insufficient number of data points
        if (sample.length() < 1000) sky = straightMedian_T(sample);  // Mode won't work if sample is small.
        else sky = modeMask(sample, "stable")[0];

        QVector<double> node = {alpha[k], delta[k], sky};
        skyPolyfitNodes.append(node);
    }
}

void MyImage::subtractSkyFit(int order, gsl_vector *c, bool saveSkyModel)
{
    QVector<float> skymodel;
    if (saveSkyModel) skymodel.resize(naxis1*naxis2);

    float skysum = 0.;

    if (*verbosity > 1) emit messageAvailable(baseName + " : Subtracting polynomial fit ...", "image");
    long t = 0;
    for (long j=0; j<naxis2; ++j) {
        for (long i=0; i<naxis1; ++i) {
            // Calculate RA / DEC from linear WCS model
            // double ra_pix  = wcs->cd[0] * (double(i)-wcs->crpix[0]) + wcs->cd[1] * (double(j)-wcs->crpix[1]) + wcs->crval[0];
            // double dec_pix = wcs->cd[2] * (double(i)-wcs->crpix[0]) + wcs->cd[3] * (double(j)-wcs->crpix[1]) + wcs->crval[1];

            // Potentially very slow!
            double ra_pix;
            double dec_pix;
            xy2sky(double(i), double(j), ra_pix, dec_pix);

            // Evaluate the background model
            double s = 0;
            double sky = gsl_vector_get(c, s++);
            // Do not combine the following two for loops, because of the s++
            for (int k=1; k<=order; ++k) {
                sky += gsl_vector_get(c, s++) * pow(ra_pix, k);
            }
            for (int k=1; k<=order; ++k) {
                sky += gsl_vector_get(c, s++) * pow(dec_pix, k);
            }
            dataCurrent[i+naxis1*j] -= sky;
            skysum += sky;
            if (saveSkyModel) {
                skymodel[t] = sky;
                ++t;
            }
        }
    }

    meanExposureBackground = skysum / (naxis1*naxis2);

    if (saveSkyModel) {
        mkAbsDir(path + "/SKY_IMAGES");
        MyFITS out(path+"/SKY_IMAGES/"+baseName+".sky.fits", naxis1, naxis2, skymodel);
        out.write("");
    }
}
