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

#ifndef INSTRUMENTDATA_H
#define INSTRUMENTDATA_H

#include <QVector>
#include <QFile>
#include <QMap>

typedef struct {
    int numChips;
    QString name = "";
    QString shortName = "";
    QString nameFullPath = "";
    float obslat = 0.;
    float obslon = 0.;
    QString bayer;
    QString type;
    QString flip = "";
    float pixscale = 1.0; // in arcsec
//    float gain = 1.0;
    float radius = 0.1;   // exposure coverage radius in degrees
    float storage = 0;    // MB used for a single image


    float storageExposure = 0.; // MB used for the entire (multi-chip) exposure
    int numUsedChips;
    long nGlobal = 1;     // Overall focal plane size in x-direction
    long mGlobal = 1;     // Overall focal plane size in x-direction
    QVector<int> badChips;
    QVector<int> goodChips;
    int validChip = -1;
    QMap<int, int> chipMap;      // in case of bad detectors, we need to map e.g. chip #4 to index #3 (e.g. if data from chip #2 is missing)

    QVector<int> overscan_xmin;
    QVector<int> overscan_xmax;
    QVector<int> overscan_ymin;
    QVector<int> overscan_ymax;
    QVector<int> cutx;
    QVector<int> cuty;
    QVector<int> sizex;
    QVector<int> sizey;
    QVector<int> crpix1;
    QVector<int> crpix2;
} instrumentDataType;

#endif // INSTRUMENTDATA_H
