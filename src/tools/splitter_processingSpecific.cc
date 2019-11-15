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

#include "splitter.h"
#include "tools.h"
#include "../instrumentdata.h"
#include "../myimage/myimage.h"
#include "../myfits/myfits.h"
#include "../functions.h"

#include "fitsio.h"
#include <QString>
#include <QVector>
#include <QFile>
#include <QDir>

// This source file contains instrument-specific processing routines.
// Mainly to deal with peculiarities of the data

void Splitter::descrambleLiris()
{
    if (!successProcessing) return;

    int n = naxis1Raw;        // Must use Raw size because we trim it in polarimetry mode
    int m = naxis2Raw;
    QVector<QVector<float>> image2D(n);
    QVector<QVector<float>> image2Dmod(n);

    int i = 0;
    int j = 0;

    for(i=0; i<n; ++i) {
        image2D[i].fill(0,m);
        image2Dmod[i].fill(0,m);
    }

    // Put the image data into the 2D array
    for (j=0; j<m; ++j) {
        for (i=0; i<n; ++i) {
            image2D[i][j] = dataRaw[i+n*j];
        }
    }

    // correct 1st quadrant
    // shift lower left corner to the upper right
    image2Dmod[510][511] = image2D[0][0];
    image2Dmod[511][511] = image2D[1][0];
    // shift main block two columns to the left
    for (i=0; i<=509; ++i)  {
        for (j=0; j<=511; ++j)  {
            image2Dmod[i][j] = image2D[i+2][j];
        }
    }
    // shift left two columns to the right and one row down
    for (j=1; j<=511; ++j)  {
        image2Dmod[510][j-1] = image2D[0][j];
        image2Dmod[511][j-1] = image2D[1][j];
    }

    // correct 2nd quadrant
    // shift lower left corner to the upper right
    image2Dmod[1023][511] = image2D[512][0];
    // shift main block one column to the left
    for (i=512; i<=1022; ++i)  {
        for (j=0; j<=511; ++j)  {
            image2Dmod[i][j] = image2D[i+1][j];
        }
    }
    // shift left column to the right and one row down
    for (j=1; j<=511; ++j)  {
        image2Dmod[1023][j-1] = image2D[512][j];
    }

    // correct 3rd quadrant
    // shift lower left corner to the upper right
    image2Dmod[511][1023] = image2D[0][512];
    // shift main block one column to the left
    for (i=0; i<=510; ++i)  {
        for (j=512; j<=1023; ++j)  {
            image2Dmod[i][j] = image2D[i+1][j];
        }
    }
    // shift left column to the right and one row down
    for (j=513; j<=1023; ++j)  {
        image2Dmod[511][j-1] = image2D[0][j];
    }

    // correct 4th quadrant
    // shift lower left corner to the upper right
    image2Dmod[1023][1023] = image2D[512][512];
    // shift main block one column to the left
    for (i=512; i<=1022; ++i)  {
        for (j=513; j<=1023; ++j)  {
            image2Dmod[i][j] = image2D[i+1][j];
        }
    }
    // shift left column to the right and one row down
    for (j=513; j<=1023; ++j)  {
        image2Dmod[1023][j-1] = image2D[512][j];
    }

    // Write the result
    for (j=0; j<m; ++j) {
        for (i=0; i<n; ++i) {
            dataRaw[i+n*j] = image2Dmod[i][j];
        }
    }
}
