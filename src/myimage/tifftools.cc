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

#include "tiffio.h"

// nothing
#include <QDebug>

void MyImage::toTIFF(int bit, float minthresh, float maxthresh, bool zscaleing, float grey, float gamma)
{
    /*
    // If no dynamic range provided, use the data min/max values
    if (!zscaleing) {
        minthresh = minVec_T(dataCurrent);
        maxthresh = maxVec_T(dataCurrent);
    }
    */

    if (*verbosity >= 2) emit messageAvailable("Creating "+baseName+".tiff ...", "ignore");

    if (dataTIFF.isEmpty()) dataTIFF = dataCurrent;

    long n = naxis1;
    long m = naxis2;
    long dim = n*m;

    // If z-scale is requested
    if (zscaleing) {
        float medVal = medianMask_T(dataTIFF);
        float rmsVal = rmsMask_T(dataTIFF);
        minthresh = medVal - 2.*rmsVal;
        maxthresh = medVal + 10.*rmsVal;
    }

    // If gamma correction if requested
    if ( gamma != 1.0 ) {
        gamma = 1./gamma;
        double gmaxlevel = pow(maxthresh, gamma);
#pragma omp parallel for
        for (long k=0; k<dim; ++k) {
            dataTIFF[k] = pow(dataTIFF[k], gamma) / gmaxlevel * maxthresh;
        }
    }

    // Clipping min and max values
#pragma omp parallel for
    for (long k=0; k<dim; k++) {
        if (dataTIFF[k] <= minthresh) dataTIFF[k] = minthresh;
        if (dataTIFF[k] >= maxthresh) dataTIFF[k] = maxthresh;
    }

    float blowup = 0.;
    if (bit == 8) {
        grey = grey / 100. * 253.;
        blowup = (253. - grey) / (maxthresh - minthresh);
    }
    else {
        grey = grey / 100. * 65000.;
        blowup = (65000. - grey) / (maxthresh - minthresh);
    }

    std::vector< std::vector<long> > imtiff(n);
    for (long i=0; i<n; ++i) {
        imtiff[i].resize(m,0);
    }

#pragma omp parallel for
    for (long i=0; i<n; ++i)  {
        for (long j=0; j<m; ++j)  {
            dataTIFF[i+n*j] = blowup * (dataTIFF[i+n*j] - minthresh) + grey;
            // flipping TIFF in y dir
            imtiff[i][naxis2-j-1] = (long) dataTIFF[i+n*j];
        }
    }

    QString outname = path+"/"+baseName+".tiff";
    TIFF     *outtiff;             // pointer to the TIFF file, defined in tiffio.h
    outtiff = TIFFOpen(outname.toUtf8().data(), "w");
    TIFFSetField(outtiff, TIFFTAG_IMAGEWIDTH, n);
    TIFFSetField(outtiff, TIFFTAG_IMAGELENGTH, m);
    TIFFSetField(outtiff, TIFFTAG_COMPRESSION, 1);
    if (bit == 8)  TIFFSetField(outtiff, TIFFTAG_BITSPERSAMPLE, 8);
    if (bit == 16) TIFFSetField(outtiff, TIFFTAG_BITSPERSAMPLE, 16);
    TIFFSetField(outtiff, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(outtiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(outtiff, TIFFTAG_PLANARCONFIG, 1);
    TIFFSetField(outtiff, TIFFTAG_SOFTWARE, "THELI");
    TIFFSetField(outtiff, TIFFTAG_IMAGEDESCRIPTION, "Created by THELI");

    if (bit == 8) {
        uint8 *outbuf = (uint8 *)_TIFFmalloc(TIFFScanlineSize(outtiff));
        for (long row=0; row<m; ++row) {
            uint8 *outb = outbuf;
            for (long column=0; column<n; ++column) {
                *outb++ = (uint8) (imtiff[column][row]);
            }
            TIFFWriteScanline(outtiff, outbuf, row, 0);
        }
        TIFFClose(outtiff);
        _TIFFfree(outbuf);
    }
    else {
        // bit == 16
        uint16 *outbuf = (uint16 *)_TIFFmalloc(TIFFScanlineSize(outtiff));
        for (long row=0; row<m; ++row) {
            uint16 *outb = outbuf;
            for (long column=0; column<n; ++column) {
                *outb++ = (uint16) (imtiff[column][row]);
            }
            TIFFWriteScanline(outtiff, outbuf, row, 0);
        }
        TIFFClose(outtiff);
        _TIFFfree(outbuf);
    }
}
