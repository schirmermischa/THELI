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
#include "../instrumentdata.h"
#include "../myimage/myimage.h"
#include "../functions.h"

#include "libraw/libraw.h"
#include "libraw/libraw_version.h"

#include <QString>
#include <QStringList>
#include <QVector>
#include <QFile>
#include <QDir>

// Handles RAW files from CMOS-type cameras

void Splitter::importRAW()
{

    if (!successProcessing) return;

    // The file has been opened successfully already by Splitter::determineFileType()
#define S rawProcessor.imgdata.sizes

    // TODO: check what that means. Do we have to return?
    if (!(rawProcessor.imgdata.idata.filters || rawProcessor.imgdata.idata.colors == 1)) {
        emit messageAvailable(fileName + " : Only Bayer-pattern RAW files supported.", "warning");
        emit warning();
        //   return;
    }

    // Extract pixels
    naxis1Raw = S.raw_width;
    naxis2Raw = S.raw_height;

    naxis1 = S.iwidth;
    naxis2 = S.iheight;

//    if (!checkChipGeometry()) return;

    long dim = naxis1Raw*naxis2Raw;
    dataRaw.clear();
    dataRaw.resize(dim);
    dataRaw.squeeze();
    for (int i=0; i<dim; ++i) {
        dataRaw[i] = rawProcessor.imgdata.rawdata.raw_image[i];
    }

    // Extract metadata
#define P1 rawProcessor.imgdata.idata
#define P2 rawProcessor.imgdata.other

#if defined(LIBRAW_MAJOR_VERSION) && LIBRAW_MAJOR_VERSION == 0 && defined(LIBRAW_MINOR_VERSION) && LIBRAW_MINOR_VERSION >= 20
#define P3 rawProcessor.imgdata.makernotes.common
#endif

    QString timeStamp = ctime(&(P2.timestamp));
    timeStamp = timeStamp.simplified();
    QStringList timeStampList = timeStamp.split(' ');
    if (timeStampList.length() < 5) {
        dateObsValue = "2020-01-01T00:00:00.0";
    }
    else {
        QString yy = timeStampList[4];
        QString mm = timeStampList[1];
        QString dd = timeStampList[2];
        QString hours = timeStampList[3];

        if (mm == "Jan") mm = "01";
        else if (mm == "Feb") mm = "02";
        else if (mm == "Mar") mm = "03";
        else if (mm == "Apr") mm = "04";
        else if (mm == "May") mm = "05";
        else if (mm == "Jun") mm = "06";
        else if (mm == "Jul") mm = "07";
        else if (mm == "Aug") mm = "08";
        else if (mm == "Sep") mm = "09";
        else if (mm == "Oct") mm = "10";
        else if (mm == "Nov") mm = "11";
        else if (mm == "Dec") mm = "12";
        dateObsValue = yy+"-"+mm+"-"+dd+"T"+hours;
        if (!checkFormatDATEOBS()) dateObsValue = "2020-01-01T00:00:00.0";
    }

    mjdobsValue = dateobsToMJD();
    exptimeValue = P2.shutter;
#if defined(LIBRAW_MAJOR_VERSION) && LIBRAW_MAJOR_VERSION == 0 && defined(LIBRAW_MINOR_VERSION) && LIBRAW_MINOR_VERSION >= 20
    sensorTemp = P3.SensorTemperature;
    cameraTemp = P3.CameraTemperature;
#else
    sensorTemp = P2.SensorTemperature;
    cameraTemp = P2.CameraTemperature;
#endif
    isoSpeed = QString::number(int(P2.iso_speed));

    // From RAW data (unreliable, because of potential trimming)
    /*
    bayerPattern = "";
    if (P1.filters) {
        if (!P1.cdesc[3]) P1.cdesc[3] = 'G';
        for (int i=0; i<4; ++i) {
            bayerPattern.append(P1.cdesc[rawProcessor.fcol(i >> 1, i & 1)]);
        }
    }
    */

    // Rather, take user-defined setup
    bayerPattern = instData.bayer;
}

// UNUSED
// For RAW files we force the SIZE settings in the camera.ini
bool Splitter::checkChipGeometry()
{
    if (!successProcessing) return false;

    for (int chip=0; chip<instData.numChips; ++chip) {
        if (instData.sizex[chip] != naxis1 || instData.sizey[chip] != naxis2) {
            emit showMessageBox("Splitter::IncompatibleSizeRAW", instData.name, QString::number(naxis1)+" "+QString::number(naxis2));
            emit messageAvailable("Splitter::checkChipGeometry(): Inconsistent detector geometry", "error");
            emit critical();
            successProcessing = false;
            return false;
        }
    }
    return true;
}

void Splitter::buildHeaderRAW()
{
    if (!successProcessing) return;

    float flipcd11 = 1.0;
    float flipcd22 = 1.0;
    if (instData.flip == "FLIPX") flipcd11 = -1.0;
    else if (instData.flip == "FLIPY") flipcd22 = -1.0;
    else if (instData.flip == "ROT180") {
        flipcd11 = -1.0;
        flipcd22 = -1.0;
    }

    QStringList cards;
    cards.append("OBJECT  = 'Unknown'");
    cards.append("CTYPE1  = 'RA---TAN'");
    cards.append("CTYPE2  = 'DEC--TAN'");
    cards.append("CRVAL1  = 0.0");
    cards.append("CRVAL2  = 0.0");
    cards.append("CRPIX1  = "+QString::number(naxis1/2));
    cards.append("CRPIX2  = "+QString::number(naxis2/2));
    cards.append("CD1_1   = "+QString::number(-1.*flipcd11*instData.pixscale/3600.));
    cards.append("CD1_2   = 0.0");
    cards.append("CD2_1   = 0.0");
    cards.append("CD2_2   = "+QString::number(flipcd22*instData.pixscale/3600.));
    cards.append("EQUINOX = 2000.0");
    cards.append("RADESYS = 'ICRS'");
    cards.append("EXPTIME = "+QString::number(exptimeValue));
    cards.append("DATE-OBS= "+dateObsValue);
    cards.append("MJD-OBS = "+QString::number(mjdobsValue, 'f', 12));
    cards.append("FILTER  = 'RGB'");
    cards.append("AIRMASS = 1.0");
    cards.append("THELIPRO= 1");
    cards.append("BAYER   = '"+bayerPattern+"'");
    if (sensorTemp > -1000.) cards.append("DET_TEMP= "+QString::number(sensorTemp, 'f', 2));
    if (cameraTemp > -1000.) cards.append("CAM_TEMP= "+QString::number(cameraTemp, 'f', 2));
    if (!isoSpeed.isEmpty()) cards.append("ISOSPEED= "+isoSpeed);

    for (auto &card : cards) {
        card.resize(80, ' ');
    }

    headerTHELI.clear();
    headerTHELI.append(cards);
    QString card1 = "GAINORIG= 1.0     / Original gain in the raw data for this image";
    QString card2 = "GAIN    = 1.0     / ADUs were converted to e- in this image using GAINORIG";
    card1.resize(80, ' ');
    card2.resize(80, ' ');
    headerTHELI.append(card1);
    headerTHELI.append(card2);
    headerTHELI.append(dummyKeys);
}

void Splitter::extractImagesRAW()
{
    if (!successProcessing) return;

    // Keeping multi-chip code structure for consistency, even though we have just a single chip
    for (int chip=0; chip<instData.numChips; ++chip) {
        importRAW();
        overwriteCameraIniRAW();
        getDetectorSections();
        getMultiportInformation(chip);
        correctOverscan();
        cropDataSection(dataSection[chip]);
        correctXtalk();
        correctNonlinearity(chip);
        applyMask(chip);
        buildHeaderRAW();
        writeImage(chip);

#pragma omp atomic
        *progress += progressStepSize;
    }
}

void Splitter::overwriteCameraIniRAW()
{
    if (!successProcessing) return;

    // For the sake of generality I keep the multi-chip architecture.
    // RAW files can contain multiple extensions (which are not supported yet by THELI)

    // WARNING: we OVERWRITE the overscan and data section entries in camera.ini
    // Trusting that metadata in RAW files are correct

    // No overscan correction if less than 5 pixels wide
    for (int chip=0; chip<instData.numChips; ++chip) {
        // If overscan == 0 in camera.ini, the overscan_xmin etc vectors are set to empty to indicate no overscan shall be done for FITS files.
        // However, for RAW files, we do want an overscan if available.
        instData.overscan_xmin.resize(1);
        instData.overscan_xmax.resize(1);
        instData.overscan_ymin.resize(1);
        instData.overscan_ymax.resize(1);
        instData.overscan_xmin[chip] = 0;
        instData.overscan_xmax[chip] = S.left_margin < 5 ? 0 : S.left_margin - 1;
        instData.overscan_ymin[chip] = 0;
        instData.overscan_ymax[chip] = S.top_margin < 5 ? 0 : S.top_margin - 1;
        // Deactivate overscan if area is less than 5 pixels wide
        if (instData.overscan_xmin[chip] == 0 && instData.overscan_xmax[chip] == 0) {
            instData.overscan_xmin.clear();
            instData.overscan_xmax.clear();
        }
        if (instData.overscan_ymin[chip] == 0 && instData.overscan_ymax[chip] == 0) {
            instData.overscan_ymin.clear();
            instData.overscan_ymax.clear();
        }
        instData.cutx[chip] = S.left_margin;
        instData.cuty[chip] = S.top_margin;
        instData.sizex[chip] = S.iwidth;
        instData.sizey[chip] = S.iheight;
        if (S.iwidth % 2 != 0) {
            instData.cutx[chip] += 1;
            instData.sizex[chip] -= 1;
        }
        if (S.iheight % 2 != 0) {
            instData.cuty[chip] += 1;
            instData.sizey[chip] -= 1;
        }
    }

    // Don't need RAW data and metadata anymore
    rawProcessor.recycle();
}
