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

#include "mask.h"
#include "../functions.h"
#include "../tools/polygon.h"
#include <omp.h>

#include <QTextStream>
#include <QString>

Mask::Mask(const instrumentDataType *instrumentData, QObject *parent) : QObject(parent),
    instData(instrumentData)
{
    // Initialize the masks
    initMasks();
}

void Mask::initMasks()
{
    // We need as many masks as we have chips

    // Masks are fully populated, even for chips the user does not want to use, because the latter may change,
    // but the mask is initialized when the instrument is selected, so we need it for all chips.

    globalMask.resize(instData->numChips);
    isChipMasked.resize(instData->numChips);

    // Resize each mask, and set it to false
    for (int chip=0; chip<instData->numChips; ++chip) {
        long n = instData->sizex[chip];      // naxis1 after overscan trimming
        long m = instData->sizey[chip];      // naxis2 after overscan trimming
        globalMask[chip].fill(false, n*m);   // Initiate all pixels to be unmasked ("masked = false")
        isChipMasked[chip] = false;
        globalMask[chip].squeeze();          // shed excess memory
    }

    // The basename of the region mask.
    // There can be a global mask (without chip number), ending in .reg which is valid for all chips.
    // And there can be additional, individual masks, ending in_chip.reg

    QString baseName = instData->nameFullPath;
    baseName = baseName.remove(".ini");

    // Global mask: create the mask for chip 1, then copy it to all other chips.
    // This assumes that all chips have identical geometries!
    QString globalMaskName = baseName+".reg";
    long n_ref = instData->sizex[0];
    long m_ref = instData->sizey[0];
    QFile file(globalMaskName);
    if (file.exists()) {
        addRegionFilesToMask(n_ref, m_ref, globalMaskName, globalMask[0], isChipMasked[0]);
        for (int chip=1; chip<instData->numChips; ++chip) {
            long n = instData->sizex[chip];
            long m = instData->sizey[chip];
            if (n != n_ref || m != m_ref) {
                qDebug() << QString(__func__) + " Error: chip geometries must be identical for all chips for a global mask.";
                break;
            }
            isChipMasked[chip] = true;
            globalMask[chip] = globalMask[0];
       }
    }

    // Individual mask (addRegionFiles exists immediately if file does not exist)
#pragma omp parallel for
  // NOT parallelized internally (polygon::addPolygon_bool()
    for (int chip=0; chip<instData->numChips; ++chip) {
        QString individualMaskName = baseName+"_"+QString::number(chip+1)+".reg";
        long n = instData->sizex[chip];
        long m = instData->sizey[chip];
        addRegionFilesToMask(n, m, individualMaskName, globalMask[chip], isChipMasked[chip]);
    }
}

void Mask::invert()
{
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : globalMask[chip]) {
            it = !it;
        }
    }
}

/*
void Mask::addRectangle(int chip, QVector<long> rect, bool invert)
{
    long n = instData->sizex[chip]; // naxis1 after overscan trimming
    long m = instData->sizey[chip]; // naxis2 after overscan trimming
    // Add rectangular area to mask, possibly inverted
    long xmin = rect[0];
    long xmax = rect[1];
    long ymin = rect[2];
    long ymax = rect[3];
    long i, j;
    if (xmin >= 0 && xmax >= 0 && ymin >= 0 && ymax >= 0) {
        if (!invert) {
            for (j=0; j<m; ++j) {
                for (i=0; i<n; ++i) {
                    if (i >= xmin && i <= xmax && j >= ymin && j <= ymax) globalMask[chip][i+n*j] = true;
                }
            }
        }
        else {
            for (j=0; j<m; ++j) {
                for (i=0; i<n; ++i) {
                    if (!(i >= xmin && i <= xmax && j >= ymin && j <= ymax)) globalMask[chip][i+n*j] = true;
                }
            }
        }
    }
}
*/

void Mask::addImage(int chip, QVector<float> segmentationMap, bool invert)
{
    long n = instData->sizex[chip]; // naxis1 after overscan trimming
    long m = instData->sizey[chip]; // naxis2 after overscan trimming

    // segmentation map (zero for good p Directory not found in Data classixels, i.e. "no object")
    for (long i=0; i<n*m; ++i) {
        if (!invert) {
            if (segmentationMap[i] != 0.) globalMask[chip][i] = true;
        }
        else {
            if (segmentationMap[i] == 0.) globalMask[chip][i] = true;
        }
    }
}

void Mask::reset()
{
    // Warning: after instrument change, instData->numchips is already the new number of chips, while
    // the globalMask vector still has the old number!
    for (auto &it : globalMask) {
        it.clear();
        it.squeeze();
    }

    globalMask.resize(0);
}
