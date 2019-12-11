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

#ifndef ABSPHOT_H
#define ABSPHOT_H

#include <QVector>

class AbsPhot
{
public:
    AbsPhot();
    QVector<double> qv_RA = QVector<double>();
    QVector<double> qv_DEC = QVector<double>();
    QVector<double> qv_ZP = QVector<double>();                // the median ZP per aperture
    QVector<double> qv_ZPerr = QVector<double>();             // the median ZP err per aperture
    QVector<double> qv_mag1ref = QVector<double>();           // the reference magnitude per object
    QVector<double> qv_mag2ref = QVector<double>();           // the reference magnitude to construct the color term per object
    QVector<double> qv_mag1errref = QVector<double>();        // the reference magnitude error per object
    QVector<double> qv_mag2errref = QVector<double>();        // the reference magnitude error to construct the color term per object
    QVector<double> qv_colorIndividual = QVector<double>();   // individual color index per object
    QVector<double> qv_colorErrIndividual = QVector<double>();   // individual color index error per object
    QVector<double> qv_ZPIndividual = QVector<double>();      // individual ZP per object (magref - magobj)
    QVector<double> qv_ZPerrIndividual = QVector<double>();   // individual ZP error per object (magref - magobj)
    QVector<double> qv_colorIndividualOutlier = QVector<double>();   // individual color term per object that did not survive the fit
    QVector<double> qv_ZPIndividualOutlier = QVector<double>();      // individual ZP per object (magref - magobj) that did not survive the fit
    QVector<double> qv_colorManualOutlier = QVector<double>();   // individual color term per object that was manually rejected
    QVector<double> qv_ZPManualOutlier = QVector<double>();      // individual ZP per object (magref - magobj) that was manually rejected
    QVector<double> qv_magauto = QVector<double>();           // MAG_AUTO per object
    QVector<double> qv_magerrauto = QVector<double>();        // MAGERR_AUTO per object
    QVector<double> qv_apertures = QVector<double>();         // aperture sizes
    QVector<bool>   qv_fitMask = QVector<bool>();             // whether an individual object is masked for a fit iteration
    QVector<bool>   qv_fitMask_old = QVector<bool>();         // whether an individual object is masked for a fit iteration previously
    QVector<bool>   qv_ManualMask = QVector<bool>();          // whether an individual object was manually selected for exclusion
    QVector<bool>   qv_ManualMask_old = QVector<bool>();      // whether an individual object was previously manually selected for exclusion
    // dims of magaper: numObj vectors, each vector conaining numAper apertures
    QVector< QVector<double> > qv_magaper = QVector< QVector<double> >();      // contains all individual apertures per object, per aperture
    QVector< QVector<double> > qv_magerraper = QVector< QVector<double> >();   // contains all individual apertures per object, per aperture
    QString filter = "";
    QString colorfilter = "";
    QString color = "";
    double slope;
    double cutoff;

    int numAper = 0;
    int numObj = 0;
    int num_outliers = 0;
    int num_ManualOutliers = 0;
    QVector<float> fitParams = QVector<float>(4,0.);
    QVector<float> fitParamsErr = QVector<float>(4,0.);
    bool initialized = false;

    double ZPauto;
    double ZPautoerr;

    QString ZPSelected;
    QString ZPerrSelected;
    QString Color1Selected;
    QString Color2Selected;
    QString Color3Selected;
    QString ColorErr1Selected;
    QString ColorErr2Selected;
    QString ColorErr3Selected;

    void regressionLinfit();              // using an iterative linear fit
    bool regression(int fitOrder);        // Using a GSL polynomial fit
    void clear();

    void getColor();
    void getZP();

private:
    long setupFitMask();
};

#endif // ABSPHOT_H
