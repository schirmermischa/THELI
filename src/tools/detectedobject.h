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

#ifndef DETECTEDOBJECT_H
#define DETECTEDOBJECT_H

#include <QObject>
#include <QVector>
#include<QBitArray>

#include "wcs.h"

class DetectedObject : public QObject
{
    Q_OBJECT
    void calcFlux();
    void calcMoments();
    void calcMomentsErrors();
    void calcVariance();
    void calcEllipticity();
    void calcFluxRadius();
    void calcWindowedMoments();
    void calcWindowedMomentsErrors();
    void calcMagAuto();
    void getWindowedPixels();
    void calcSkyCoords();
    void calcWindowedEllipticity();
    void filterSpuriousDetections();
    QVector<double> calcFluxAper(float aperture);
    void calcApertureMagnitudes();
    void correctOriginOffset();
    void calcFWHM();
    bool isTruncated(const long xmin, const long xmax, const long ymin, const long ymax);

public:
    explicit DetectedObject(const QList<long> &objectIndices, const QVector<float> &data, const QVector<float> &background,
                            const QVector<float> &weight, const QVector<bool> &mask, bool weightinmemory,
                            const long naxis1, const long naxis2, const long objid, const float satVal, const float gainval,
                            wcsprm &wcsImage, QObject *parent = nullptr);
    ~DetectedObject();

    const QVector<float> &dataMeasure;
    const QVector<float> &dataBackground;
    const QVector<float> &dataWeight;
    const QVector<bool> &mask;

    bool badDetection = false;   // set if anything goes wrong with an object parameter's calculation
    bool globalMaskAvailable = true;   // the default for internal processing (but not for external images, e.g. abs zeropoint)
    bool weightInMemory = true;

    wcsprm &wcs;

    // Moments and shapes
    // Using Source Extractor naming style for easier recognition

    // ISOPHOTAL
    long XMIN = 0;      // min and max positions (used in calculations)
    long XMAX = 0;
    long YMIN = 0;
    long YMAX = 0;
    double X = 0.;      // 1st moment          // WARNING: zero-indexing, must add +1 if used externally
    double Y = 0.;      // 1st moment          // WARNING: zero-indexing, must add +1 if used externally
    double X2 = 0.;     // 2nd moment
    double Y2 = 0.;     // 2nd moment
    double XY = 0.;     // 2nd moment
    double X2Y2 = 0.;   // helper variable
    double A = 0;       // major axis
    double B = 0;       // minor axis
    double THETA = 0.;  // position angle
    double CXX = 0;     // cartesian ellipticity parameterization
    double CYY = 0.;    // cartesian ellipticity parameterization
    double CXY = 0.;    // cartesian ellipticity parameterization
    double XERR = 0.;
    double YERR = 0.;
    double ERRX2 = 0.;
    double ERRY2 = 0;
    double ERRXY = 0.;
    double ERRA = 0.;
    double ERRB = 0.;
    double ERRTHETA = 0.;
    double MAG_ISO = 0.;
    double FLUX_ISO = 0.;
    double XVAR = 0.;
    double YVAR = 0.;

    // WINDOWED
    double XWIN = 0.;   // windowed 1st moment          // WARNING: zero-indexing, must add +1 if used externally
    double YWIN = 0.;   // windowed 1st moment          // WARNING: zero-indexing, must add +1 if used externally
    double X2WIN = 0.;
    double Y2WIN = 0.;
    double XYWIN = 0.;
    double AWIN = 0.;
    double BWIN = 0.;
    double THETAWIN = 0.;
    double CXXWIN = 0;
    double CYYWIN = 0.;
    double CXYWIN = 0.;
    double ERRX2WIN = 0.;
    double ERRY2WIN = 0.;
    double ERRXYWIN = 0.;
    double ERRAWIN = 0.;
    double ERRBWIN = 0.;
    double ERRTHETAWIN = 0.;
    double XWINERR = 0.;
    double YWINERR = 0.;
    double MAG_AUTO = 0.;
    double MAGERR_AUTO = 0.;
    double FLUX_AUTO = 0.;
    double FLUXERR_AUTO = 0.;
    double FLUX_RADIUS = 0.;
    double FWHM = 0.;
    double ELLIPTICITY = 0.;

    // APERTURE
    QVector<double> FLUX_APER;
    QVector<double> MAG_APER;
    QVector<double> FLUXERR_APER;
    QVector<double> MAGERR_APER;

    float saturationValue = 1.e9;
    double sigmai = 0.;
    double sigmab = 0.;
    double ZP = 0;
    double ALPHA_J2000 = 0.;
    double DELTA_J2000 = 0.;
    double gain = 1.0;
    double sigma_back = 0.;

    double crval1 = 0.;
    double crval2 = 0.;
    long objID = 0;
    long area = 0;
    int FLAGS = 0;
    bool aperFlagSetAlready = false;
    QBitArray bitflags;

    QVector<long> pixels_x;
    QVector<long> pixels_y;
    QVector<float> pixels_flux;
    QVector<float> pixels_back;
    QVector<float> pixels_weight;

    QVector<long> pixelsWin_x;
    QVector<long> pixelsWin_y;
    QVector<float> pixelsWin_flux;
    QVector<float> pixelsWin_back;
    QVector<float> pixelsWin_weight;

    QVector<float> apertures;
    QVector<long> pixelsAper_x;
    QVector<long> pixelsAper_y;
    QVector<float> pixelsAper_flux;
    QVector<float> pixelsAper_back;
    QVector<float> pixelsAper_weight;

    long naxis1 = 0;  // image geometry, needed to respect boundaries
    long naxis2 = 0;

    double rad = 3.14159265/180.;

    void computeObjectParams();
    void remove();
signals:

public slots:
};

#endif // DETECTEDOBJECT_H
