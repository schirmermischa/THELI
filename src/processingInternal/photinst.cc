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

#include "photinst.h"
#include "../functions.h"

#include <QVector>

PhotInst::PhotInst(QObject *parent) : QObject(parent)
{
    fluxScale.clear();
    expTime.clear();
    RZP.clear();
    indexMap.clear();
    baseName.clear();
}

void PhotInst::getRZP()
{
    numExp = fluxScale.length();

    for (int j=0; j<numExp; ++j) {
        RZP << -2.5*log10(fluxScale[j] * expTime[j]);
    }

    double meanRZP = meanMask_T(RZP);

    // Mean relative ZP
    for (auto &it : RZP) {
        it -= meanRZP;
    }

    // THELI FLXSCALE
    for (int j=0; j<numExp; ++j) {
        fluxScale[j] = pow(10., -0.4*RZP[j]) / expTime[j];
    }
}
