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

#ifndef MYWCS_H
#define MYWCS_H

#include <QString>

class MyWCS
{
public:
    MyWCS();

    double crval1 = -99.;
    double crval2 = -99.;
    double crpix1 = -1e9;
    double crpix2 = -1e9;
    double cd1_1 = -99.;
    double cd1_2 = -99.;
    double cd2_1 = -99.;
    double cd2_2 = -99.;
    double plateScale;
    float equinox = 0.;
    long naxis1 = 0;
    long naxis2 = 0;
    bool isValid = false;
    void xy2sky(double x, double y, double &alpha, double &delta);
    void sky2xy(double alpha, double delta, double &x, double &y);
    double getPlateScale();
    bool containsRaDec(QString alphaStr, QString deltaStr);
    void checkValidity();
};

#endif // MYWCS_H
