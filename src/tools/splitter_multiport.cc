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
#include "../dockwidgets/confdockwidget.h"
#include "ui_confdockwidget.h"
#include "../functions.h"
#include "fitsio.h"
#include <QString>
#include <QVector>
#include <QFile>
#include <QDir>


// The functions below make a fork of the overscan / cropping part for detectors with multiple readout channels
// Ultimately, this should become generic enough to also work with "normal" detectors

/*
void Splitter::pasteMultiPortDataSections(QVector<long> dataSect)
{
    if (!successProcessing) return;

    if (instData.name == "LIRIS_POL@WHT") {
        naxis1 = naxis1Raw;
        naxis2 = naxis2Raw;
        dataCurrent = dataRaw;
        return;
    }

    // The image that enters this function has the same geometry as the raw data, including all overscan areas.

    // New image geometry; here, naxis1/2 refers to the total size of the image (after pasting one or more readout channels)
    // This should match what is given in instrument.ini
    naxis1 = dataSect[1] - dataSect[0] + 1;
    naxis2 = dataSect[3] - dataSect[2] + 1;
    if (*verbosity > 1) emit messageAvailable(baseName + " : Cropping to data section, size = "
                                              + QString::number(naxis1)+"x"+QString::number(naxis2)+" pixel", "image");
    dataCurrent.resize(naxis1*naxis2);

    // Loop over readout channels
    int numChannels = multiportOverscanDirections.length();       // the number of readout channels per detector
    for (int channel=0; channel<numChannels; ++channel) {

    }

    long k = 0;
    for (long j=dataSect[2]; j<=dataSect[3]; ++j) {
        for (long i=dataSect[0]; i<=dataSect[1]; ++i) {
            dataCurrent[k] = dataRaw[i+naxis1Raw*j];
            ++k;
        }
    }
}
*/

/*
void Splitter::computeMultiportDataOffsets()
{
    for (int channel=0; channel<multiportGains.length(); ++channel) {
        long xoffset = multiportDataSections[channel][0] - multiportImageSections[channel][0];
        long yoffset = multiportDataSections[channel][2] - multiportImageSections[channel][2];
    }
}
*/


// Collects information where in an image the overscan and illuminated pixel areas are located.
// For "normal" cameras which have only a single readout channel, or where the differences between readout channels are negligible,
// we just take the information from the camera.ini file.
// For others, where multiple channels are pasted into a single FITS extension, or where different channels are in different FITS extensions,
// we must rely on the headers and do individual implementations
void Splitter::getMultiportInformation(int chip)
{
    if (!successProcessing) return;

    multiportOverscanSections.clear();
    multiportIlluminatedSections.clear();
    multiportChannelSections.clear();
    multiportGains.clear();
    multiportOverscanDirections.clear();
    bool individualFixDone = false;

    //    if (instData.name == "GROND_OPT@MPGESO") {
    if (instNameFromData == "GROND_OPT@MPGESO") {
        naxis1 = 2048;
        naxis2 = 2050;
        multiportOverscanDirections << "vertical" << "vertical";
        multiportOverscanSections << extractVerticesFromKeyword("BSECA");
        multiportOverscanSections << extractVerticesFromKeyword("BSECB");
        multiportIlluminatedSections << extractVerticesFromKeyword("DSECA");
        multiportIlluminatedSections << extractVerticesFromKeyword("DSECB");
        multiportChannelSections << extractVerticesFromKeyword("ASECA");
        multiportChannelSections << extractVerticesFromKeyword("ASECB");

        float gainValue1 = 0.0;
        float gainValue2 = 0.0;
        searchKeyValue(QStringList() << "GAIN", gainValue1);
        // Nominally, the 2 channels in the optical GROND detectors have the same gain, but that's not exactly true
        QString detector = "";
        searchKeyValue(QStringList() << "EXTNAME", detector);
        if (detector == "CCDg") gainValue2 = gainValue1 * 1.06947;
        if (detector == "CCDr") gainValue2 = gainValue1 * 1.00766;
        if (detector == "CCDi") gainValue2 = gainValue1 * 1.00001;
        if (detector == "CCDz") gainValue2 = gainValue1 * 0.88904;
        multiportGains << gainValue1 << gainValue2;
//        minGainValue = minVec_T(multiportGains);
        individualFixDone = true;
    }

    //   if (instData.name == "GROND_NIR@MPGESO") {
    if (instNameFromData == "GROND_NIR@MPGESO") {
        naxis1 = 3072;
        naxis2 = 1024;
        multiportOverscanDirections << "dummy";
        QVector<long> section = {0, naxis1-1, 0, naxis2-1};   // all three channels at the same time. Cutting happens separately during writeImage();
        multiportIlluminatedSections << section;
        multiportChannelSections << section;
        multiportGains << 1.0;
//        minGainValue = minVec_T(multiportGains);
        individualFixDone = true;
    }

    if (instData.name == "SuprimeCam_200808-201705@SUBARU") {
        // contains 4 vertical data slices, separated by overscans, in a single FITS extension
        naxis1 = 2048;
        naxis2 = 4224;
        // NOTE: The y coordinates of the illuminated sections and overscan sections in the SuprimeCam raw data are wrong.
        // They are normally contained in keywords "S_EFMN11", "S_EFMX11", "S_EFMN12", "S_EFMX12" for channel 1, etc
        multiportOverscanDirections << "vertical" << "vertical" << "vertical" << "vertical";
        multiportOverscanSections << extractReducedOverscanFromKeyword("S_OSMN11", "S_OSMX11", 1, naxis2Raw);
        multiportOverscanSections << extractReducedOverscanFromKeyword("S_OSMN21", "S_OSMX21", 1, naxis2Raw);
        multiportOverscanSections << extractReducedOverscanFromKeyword("S_OSMN31", "S_OSMX31", 1, naxis2Raw);
        multiportOverscanSections << extractReducedOverscanFromKeyword("S_OSMN41", "S_OSMX41", 1, naxis2Raw);

        /*
        multiportIlluminatedSections << extractVerticesFromKeyword("S_EFMN11", "S_EFMX11", "S_EFMN12", "S_EFMX12");
        multiportIlluminatedSections << extractVerticesFromKeyword("S_EFMN21", "S_EFMX21", "S_EFMN22", "S_EFMX22");
        multiportIlluminatedSections << extractVerticesFromKeyword("S_EFMN31", "S_EFMX31", "S_EFMN32", "S_EFMX32");
        multiportIlluminatedSections << extractVerticesFromKeyword("S_EFMN41", "S_EFMX41", "S_EFMN42", "S_EFMX42");
        */

        int ymin = 49;   // in pixel coords (not accounting for C++ indexing starting at 0)
        int ymax = 4273; // in pixel coords (not accounting for C++ indexing starting at 0)
        if (chip == 3 || chip == 4 || chip == 5 || chip == 8 || chip == 9) {
            ymin = 1;
            ymax = 4225;
        }
        multiportIlluminatedSections << extractReducedIlluminationFromKeyword("S_EFMN11", "S_EFMX11", ymin, ymax);
        multiportIlluminatedSections << extractReducedIlluminationFromKeyword("S_EFMN21", "S_EFMX21", ymin, ymax);
        multiportIlluminatedSections << extractReducedIlluminationFromKeyword("S_EFMN31", "S_EFMX31", ymin, ymax);
        multiportIlluminatedSections << extractReducedIlluminationFromKeyword("S_EFMN41", "S_EFMX41", ymin, ymax);

        if (chip == 0 || chip == 1 || chip == 2 || chip == 6 || chip == 7) {
            multiportChannelSections << QVector<long>({0*naxis1Raw/4, 1*naxis1Raw/4-1, 0, naxis2Raw-1});     // These nunmbers are not directly accessible in the FITS headers
            multiportChannelSections << QVector<long>({1*naxis1Raw/4, 2*naxis1Raw/4-1, 0, naxis2Raw-1});
            multiportChannelSections << QVector<long>({2*naxis1Raw/4, 3*naxis1Raw/4-1, 0, naxis2Raw-1});
            multiportChannelSections << QVector<long>({3*naxis1Raw/4, 4*naxis1Raw/4-1, 0, naxis2Raw-1});
        }
        else {
            multiportChannelSections << QVector<long>({3*naxis1Raw/4, 4*naxis1Raw/4-1, 0, naxis2Raw-1});     // reversed order in FITS headers
            multiportChannelSections << QVector<long>({2*naxis1Raw/4, 3*naxis1Raw/4-1, 0, naxis2Raw-1});
            multiportChannelSections << QVector<long>({1*naxis1Raw/4, 2*naxis1Raw/4-1, 0, naxis2Raw-1});
            multiportChannelSections << QVector<long>({0*naxis1Raw/4, 1*naxis1Raw/4-1, 0, naxis2Raw-1});
        }
        float gainValue1 = 1.0;
        float gainValue2 = 1.0;
        float gainValue3 = 1.0;
        float gainValue4 = 1.0;
        searchKeyValue(QStringList() << "S_GAIN1", gainValue1);
        searchKeyValue(QStringList() << "S_GAIN2", gainValue2);
        searchKeyValue(QStringList() << "S_GAIN3", gainValue3);
        searchKeyValue(QStringList() << "S_GAIN4", gainValue4);
        multiportGains << gainValue1 << gainValue2 << gainValue3 << gainValue4;
//        minGainValue = minVec_T(multiportGains);
        channelGains.clear();
        channelGains << 1.0 << 1.0 << 1.0 << 1.0;   // dummy; not used anywhere else for SuprimeCam_200808-201705
        individualFixDone = true;
    }

    if (instData.name == "LIRIS_POL@WHT") {
        naxis1 = 1024;
        naxis2 = 1024;
        multiportOverscanDirections << "dummy";
        QVector<long> section = {0, naxis1-1, 0, naxis2-1};   // all four channels at the same time. Cutting happens separately during writeImage();
        multiportIlluminatedSections << section;
        multiportChannelSections << section;
        multiportGains << 1.0;
//        minGainValue = minVec_T(multiportGains);
        individualFixDone = true;
    }

    // All Hamamatsu Gemini GMOS configurations. Single channel in single FITS extension
    if (multiChannelMultiExt.contains(instData.name) && instData.name.contains("GMOS")) {
        int binning = 1;
        QString binString = "";
        searchKeyValue(QStringList() << "CCDSUM", binString);
        if (binString.simplified() == "1 1") binning = 1;
        else if (binString.simplified() == "2 2") binning = 2;
        else {
            emit messageAvailable(fileName + ": Invalid binning encountered: "+binString.simplified(), "error");
            emit critical();
            successProcessing = false;
            return;
        }
        naxis1 = 2048 / binning;
        naxis2 = 4224 / binning;

        int naxis1channel = 0;
        int naxis2channel = 0;
        searchKeyValue(QStringList() << "NAXIS1", naxis1channel);
        searchKeyValue(QStringList() << "NAXIS2", naxis2channel);
        multiportOverscanDirections << "vertical";
        multiportOverscanSections << extractVerticesFromKeyword("BIASSEC");      // given in binned units in the header
        multiportIlluminatedSections << extractVerticesFromKeyword("DATASEC");   // given in binned units in the header
        QVector<long> channelSection;
        channelSection << 0 << naxis1channel - 1 << 0 << naxis2channel - 1;
        multiportChannelSections << channelSection;                              // "DETSEC" has the illuminated section in CCD coordinates
        //        for (auto &section : multiportChannelSections) {
        //            for (auto &it : section) it /= binning;                              // maapping unbinned pixel space to binned pixel space
        //        }
        if (chip % numAmpPerChip == 0) dataPasted.resize(naxis1 * naxis2);

        float gainValue = 1.0;
        if (instData.name == "GMOS-S-HAM@GEMINI" || instData.name == "GMOS-S-HAM_1x1@GEMINI") {
            // Accurate amplifier gains are not available in the GMOS FITS headers
            if (mjdobsValue < 57265.999988) {           // before 2015-08-31        // WARNING: these are the LOW gain modes. High gain mode is hardly used
                if (chip == 0)       gainValue = 1.626;
                else if (chip == 1)  gainValue = 1.700;
                else if (chip == 2)  gainValue = 1.720;
                else if (chip == 3)  gainValue = 1.652;
                else if (chip == 4)  gainValue = 1.739;
                else if (chip == 5)  gainValue = 1.673;
                else if (chip == 6)  gainValue = 1.691;
                else if (chip == 7)  gainValue = 1.664;
                else if (chip == 8)  gainValue = 1.613;
                else if (chip == 9)  gainValue = 1.510;
                else if (chip == 10) gainValue = 1.510;
                else if (chip == 11) gainValue = 1.519;
            }
            else {                                      // after 2015-08-31
                if (chip == 0)       gainValue = 1.834;
                else if (chip == 1)  gainValue = 1.874;
                else if (chip == 2)  gainValue = 1.878;
                else if (chip == 3)  gainValue = 1.852;
                else if (chip == 4)  gainValue = 1.908;
                else if (chip == 5)  gainValue = 1.933;
                else if (chip == 6)  gainValue = 1.840;
                else if (chip == 7)  gainValue = 1.878;
                else if (chip == 8)  gainValue = 1.813;
                else if (chip == 9)  gainValue = 1.724;
                else if (chip == 10) gainValue = 1.761;
                else if (chip == 11) gainValue = 1.652;
            }
        }
        if (instData.name == "GMOS-N-HAM@GEMINI" || instData.name == "GMOS-N-HAM_1x1@GEMINI") {
            if (chip == 0)       gainValue = 1.66;
            else if (chip == 1)  gainValue = 1.63;
            else if (chip == 2)  gainValue = 1.62;
            else if (chip == 3)  gainValue = 1.57;
            else if (chip == 4)  gainValue = 1.68;
            else if (chip == 5)  gainValue = 1.65;
            else if (chip == 6)  gainValue = 1.64;
            else if (chip == 7)  gainValue = 1.68;
            else if (chip == 8)  gainValue = 1.61;
            else if (chip == 9)  gainValue = 1.63;
            else if (chip == 10) gainValue = 1.58;
            else if (chip == 11) gainValue = 1.65;
        }
        multiportGains << gainValue;
//        updateMinGainValue(gainValue);
        channelGains.clear();
        channelGains << 1.0;   // dummy;
        individualFixDone = true;
    }

    if (instData.name == "SOI@SOAR") {
        naxis1 = 1024;
        naxis2 = 2048;

        int naxis1channel = 0;
        int naxis2channel = 0;
        searchKeyValue(QStringList() << "NAXIS1", naxis1channel);
        searchKeyValue(QStringList() << "NAXIS2", naxis2channel);
        multiportOverscanDirections << "vertical";
        multiportOverscanSections << extractVerticesFromKeyword("BIASSEC");      // given in binned units in the header
        multiportIlluminatedSections << extractVerticesFromKeyword("DATASEC");   // given in binned units in the header
        QVector<long> channelSection;
        channelSection << 0 << naxis1channel - 1 << 0 << naxis2channel - 1;
        multiportChannelSections << channelSection;                              // "DETSEC" has the illuminated section in CCD coordinates
        if (chip % numAmpPerChip == 0) dataPasted.resize(naxis1 * naxis2);

        float gainValue = 2.0;
        // Accurate amplifier gains are not available in the FITS headers
        if (chip == 0)       gainValue = 2.0;
        else if (chip == 1)  gainValue = 2.0;
        else if (chip == 2)  gainValue = 2.0;
        else if (chip == 3)  gainValue = 2.0;

        multiportGains << gainValue;
//        minGainValue = minVec_T(multiportGains);
        channelGains.clear();
        channelGains << 1.0;   // dummy;
        individualFixDone = true;
    }

    if (instData.name == "MOSAIC-II_16@CTIO") {
        naxis1 = 2048;
        naxis2 = 4096;

        int naxis1channel = 0;
        int naxis2channel = 0;
        searchKeyValue(QStringList() << "NAXIS1", naxis1channel);
        searchKeyValue(QStringList() << "NAXIS2", naxis2channel);
        multiportOverscanDirections << "vertical";
        multiportOverscanSections << extractVerticesFromKeyword("BIASSEC");      // given in binned units in the header
        multiportIlluminatedSections << extractVerticesFromKeyword("DATASEC");   // given in binned units in the header
        QVector<long> channelSection;
//        updateMinGainValue(gainValue);
        channelSection << 0 << naxis1channel - 1 << 0 << naxis2channel - 1;
        multiportChannelSections << channelSection;
        if (chip % numAmpPerChip == 0) dataPasted.resize(naxis1 * naxis2);

        float gainValue = 1.0;
        searchKeyValue(QStringList() << "GAIN", gainValue);
        multiportGains << gainValue;
//        minGainValue = minVec_T(multiportGains);
        channelGains.clear();
        channelGains << 1.0;   // dummy;
        individualFixDone = true;
    }

    if (instData.name == "MOSAIC-III_4@KPNO_4m") {
        naxis1 = 4112;
        naxis2 = 4096;

        int naxis1channel = 0;
        int naxis2channel = 0;
        searchKeyValue(QStringList() << "NAXIS1", naxis1channel);
        searchKeyValue(QStringList() << "NAXIS2", naxis2channel);
        multiportOverscanDirections << "vertical";
        multiportOverscanSections << extractVerticesFromKeyword("BIASSEC");      // given in binned units in the header
        multiportIlluminatedSections << extractVerticesFromKeyword("DATASEC");   // given in binned units in the header
        QVector<long> channelSection;
        channelSection << 0 << naxis1channel - 1 << 0 << naxis2channel - 1;
        multiportChannelSections << channelSection;
        if (chip % numAmpPerChip == 0) dataPasted.resize(naxis1 * naxis2);

        float gainValue = 1.0;
        searchKeyValue(QStringList() << "GAIN", gainValue);
        multiportGains << gainValue;
//        updateMinGainValue(gainValue);
        channelGains.clear();
        channelGains << 1.0;   // dummy;
        individualFixDone = true;
    }

    if (instData.name == "SAMI_2x2@SOAR") {
        naxis1 = 2048;
        naxis2 = 2056;

        int naxis1channel = 0;
        int naxis2channel = 0;
        searchKeyValue(QStringList() << "NAXIS1", naxis1channel);
        searchKeyValue(QStringList() << "NAXIS2", naxis2channel);
        multiportOverscanDirections << "vertical";
        multiportOverscanSections << extractVerticesFromKeyword("BIASSEC");      // given in binned units in the header
        multiportIlluminatedSections << extractVerticesFromKeyword("DATASEC");   // given in binned units in the header
        QVector<long> channelSection;
        channelSection << 0 << naxis1channel - 1 << 0 << naxis2channel - 1;
        multiportChannelSections << channelSection;
        if (chip % numAmpPerChip == 0) dataPasted.resize(naxis1 * naxis2);

        float gainValue = 2.1;          // http://www.soartelescope.org/soar/sites/default/files/SAM/archive/sami-manual.pdf
        searchKeyValue(QStringList() << "GAIN", gainValue);
        if (gainValue == 0.) {
            if (chip == 0) gainValue = 2.10000;
            if (chip == 1) gainValue = 2.20456;
            if (chip == 2) gainValue = 2.09845;
            if (chip == 3) gainValue = 2.19494;
        }

        multiportGains << gainValue;
//        updateMinGainValue(gainValue);
        channelGains.clear();
        channelGains << 1.0;   // dummy;
        individualFixDone = true;
    }

    // If any instrument-specific stuff happened above, then we do a consistency check
    if (individualFixDone) {
        //        if (multiportGains.length() != multiportOverscanSections.length()              // crashes GROND-NIR data
        if (multiportGains.length() != multiportIlluminatedSections.length()) {
            emit messageAvailable("Splitter::getMultiportInformation : Inconsistent number of channels for gain, overscan and data section: "
                                  + QString::number(multiportGains.length()) + " "
                                  + QString::number(multiportIlluminatedSections.length()), "error");
            emit critical();
            successProcessing = false;
        }
//        saturationValue *= minGainValue;
    }
    else {
        // What to do for detectors that are not split up by several readout channels and overscans
        naxis1 = dataSection[chip][1] - dataSection[chip][0] + 1;
        naxis2 = dataSection[chip][3] - dataSection[chip][2] + 1;
        // Append the overscan strips from the instrument.ini files, and padd the missing dimension
        QVector<long> overscan;
        if (!overscanX[chip].isEmpty()) {
            overscan << overscanX[chip][0] << overscanX[chip][1] << 0 << naxis2Raw-1;
            multiportOverscanDirections << "vertical";
        }
        if (!overscanY[chip].isEmpty()) {
            overscan << 0 << naxis1Raw-1 << overscanY[chip][0] << overscanY[chip][1];
            multiportOverscanDirections << "horizontal";
        }
        multiportOverscanSections << overscan;

        multiportIlluminatedSections << dataSection[chip];
        // image section = data section minus the left and bottom overscan pixels (right and upper overscan pixels not counted)
        //        QVector<long> imageSection = {0, dataSection[chip][1] - dataSection[chip][0], 0, dataSection[chip][3] - dataSection[chip][2]};
        QVector<long> channelSection = {0, naxis1Raw - 1, 0, naxis2Raw - 1};
        multiportChannelSections << channelSection;
        multiportGains << 1.0;       // i.e. leave gain unchanged
    }
}

// UNUSED
void Splitter::updateMinGainValue(float gainValue)
{
    if (gainValue < minGainValue) minGainValue = gainValue;
}

void Splitter::pasteMultiportIlluminatedSections(int chip)
{
    if (!successProcessing) return;

    // NOTE: multiportGains[channel] does not contain a Vector, just a scalar. Pasting is done one amp at a time

    // Paste the data sections into a single image of dimensions naxis1, naxis2
    if (!multiChannelMultiExt.contains(instData.name)) {
        dataCurrent.resize(naxis1*naxis2);
        //    long k = 0;   // the running 1D index in the pasted image
        int channel = 0;
        for (auto &section : multiportIlluminatedSections) {
            if (section.length() != 4) continue; // skip wrong vertices, for whatever reason they might be here
            pasteSubArea(dataCurrent, dataRaw, multiportIlluminatedSections[channel], multiportGains[channel], naxis1, naxis2, naxis1Raw);
            ++channel;
        }
    }
    // Individual exceptions (currently: GMOS, SOI, MOSAIC-II only)
    else {
        int channel = 0;

        for (auto &section : multiportIlluminatedSections) {
            if (section.length() != 4) continue; // skip wrong vertices, for whatever reason they might be here
            long offx = 0;
            long offy = 0;
            // detectors where the amps form two or more vertical stripes from left to right
            if (instData.name == "SOI@SOAR"
                    || instData.name.contains("GMOS")
                    || instData.name == "MOSAIC-II_16@CTIO") {
                offx = (chip % numAmpPerChip) * naxis1 / numAmpPerChip;
                offy = 0;
            }
            if (instData.name == "MOSAIC-III_4@KPNO_4m") {
                QVector<long> ampsec;
                ampsec << extractVerticesFromKeyword("CCDSEC");       // unused
                if (chip == 0) {offx = naxis1 / 2; offy = 0;}
                if (chip == 1) {offx = 0; offy = 0;}
                if (chip == 2) {offx = naxis1 / 2; offy = naxis2 / 2;}
                if (chip == 3) {offx = 0; offy = naxis2 / 2;}
                if (chip == 4) {offx = naxis1 / 2; offy = 0;}
                if (chip == 5) {offx = 0; offy = 0;}
                if (chip == 6) {offx = naxis1 / 2; offy = naxis2 / 2;}
                if (chip == 7) {offx = 0; offy = naxis2 / 2;}
                if (chip == 8) {offx = 0; offy = naxis2 / 2;}
                if (chip == 9) {offx = naxis1 / 2; offy = naxis2 / 2;}
                if (chip == 10) {offx = 0; offy = 0;}
                if (chip == 11) {offx = naxis1/2; offy = 0;}
                if (chip == 12) {offx = 0; offy = naxis2 / 2;}
                if (chip == 13) {offx = naxis1 / 2; offy = naxis2 / 2;}
                if (chip == 14) {offx = 0; offy = 0;}
                if (chip == 15) {offx = naxis1 / 2; offy = 0;}
                /*
                if (chip % 4 == 0) {
                    offx = naxis1 / 2;
                    offy = 0;
                }
                if (chip % 4 == 1) {
                    offx = 0;
                    offy = 0;
                }
                if (chip % 4 == 2) {
                    offx = 0;
                    offy = naxis2 / 2;
                }
                if (chip % 4 == 3) {
                    offx = naxis1 / 2;
                    offy = naxis2 / 2;
                }
                */
            }
            if (instData.name == "SAMI_2x2@SOAR") {
                QVector<long> ampsec;
                ampsec << extractVerticesFromKeyword("CCDSEC");     //unused
                if (chip == 0) {offx = 0; offy = 0;}
                if (chip == 1) {offx = naxis1 / 2; offy = 0;}
                if (chip == 2) {offx = 0; offy = naxis2 / 2;}
                if (chip == 3) {offx = naxis1 / 2; offy = naxis2 / 2;}
            }
            pasteSubArea(dataPasted, dataRaw, multiportIlluminatedSections[channel], multiportGains[channel],
                         offx, offy, naxis1, naxis2, naxis1Raw);
            ++channel;
        }
        if ( (chip+1) % numAmpPerChip == 0) {
            // all channels have been pasted. Transfer the data to dataCurrent.
            MEFpastingFinished = true;
            dataCurrent.swap(dataPasted);
            dataPasted.clear();
            dataPasted.squeeze();
            dataCurrent.squeeze();
        }
    }
}

// paste a subarea 'sector' (xmin, xmax, ymin, ymax) from source image "dataS" to target image "dataT",
// Offsets are calculated internally, assuming that data sections have equal sizes
void Splitter::pasteSubArea(QVector<float> &dataT, const QVector<float> &dataS, const QVector<long> &section, const float corrFactor,
                            const long nT, const long mT, const long nS)
{
    long iminS = section.at(0);
    long imaxS = section.at(1);
    long jminS = section.at(2);
    long jmaxS = section.at(3);

    long dimS = dataS.length();
    long dimT = dataT.length();

    // The x- and y-offsets for the illuminated sections. Calculated by taking the lower x (or y) value,
    // and checking how many equally sized sections can fit (safe, as we have usually not more than 4 sections, and the overscan areas are comparatively small)
    long sizex = section.at(1) - section.at(0) + 1;   // section x-size
    long sizey = section.at(3) - section.at(2) + 1;   // section y-size
    long nsecx = section.at(0) / sizex;               // number of sections found to the left, using integer division
    long nsecy = section.at(2) / sizey;               // number of sections found below the botton, using integer division
    long offx = nsecx * sizex;                        // The offset for the current section in the pasted output geometry
    long offy = nsecy * sizey;                        // The offset for the current section in the pasted output geometry

    for (long jS=jminS; jS<=jmaxS; ++jS) {
        for (long iS=iminS; iS<=imaxS; ++iS) {
            long iT = offx+iS-iminS;
            long jT = offy+jS-jminS;
            long sIndex = iS+nS*jS;
            long tIndex = iT+nT*jT;
            if (sIndex >= dimS || tIndex >= dimT) {
                emit messageAvailable("Inconsistent image geometry. " + instData.name + " was probably not yet tested in THELI v3.", "error");
                emit critical();
                successProcessing = false;
                break;
            }
            if (iT>=nT || iT<0 || jT>=mT || jT<0) continue;   // don't paste pixels falling outside target area
            dataT[tIndex] = dataS[sIndex] * corrFactor;   // correcting for gain differences
        }
    }
}

// paste a subarea 'sector' (xmin, xmax, ymin, ymax) from source image "dataS" to target image "dataT",
// offsets dx and dy are given explicitly
void Splitter::pasteSubArea(QVector<float> &dataT, const QVector<float> &dataS, const QVector<long> &section, const float corrFactor,
                            const long offx, const long offy, const long nT, const long mT, const long nS)
{
    long iminS = section[0];
    long imaxS = section[1];
    long jminS = section[2];
    long jmaxS = section[3];

    long dimS = dataS.length();
    long dimT = dataT.length();

    for (long jS=jminS; jS<=jmaxS; ++jS) {
        for (long iS=iminS; iS<=imaxS; ++iS) {
            long iT = offx+iS-iminS;
            long jT = offy+jS-jminS;
            if (iT>=nT || iT<0 || jT>=mT || jT<0) continue;   // don't paste pixels falling outside target area
            long sIndex = iS+nS*jS;
            long tIndex = iT+nT*jT;
            if (sIndex >= dimS || tIndex >= dimT) {
                emit messageAvailable("Inconsistent image geometry. " + instData.name + " was probably not yet tested in THELI v3.", "error");
                emit critical();
                successProcessing = false;
                break;
            }
            dataT[tIndex] = dataS[sIndex] * corrFactor;   // correcting for gain differences
        }
    }
}

// splitting "[xmin:xmax,ymin:ymax]"
QVector<long> Splitter::extractVertices(QString vertexString)
{
    vertexString = vertexString.replace(":"," ");
    vertexString = vertexString.replace(","," ");
    vertexString = vertexString.replace("[","");
    vertexString = vertexString.replace("]","");
    vertexString = vertexString.replace("'","");
    vertexString = vertexString.simplified();
    QStringList list = vertexString.split(" ");
    QVector<long> vertices;
    // for loop also works for one-dimensional vertices
    for (int i=0; i<list.length(); ++i) {
        vertices << list[i].toFloat() - 1;   // The -1 accounts for C++ indexing starting at zero
    }
    return vertices;
}

QVector<long> Splitter::extractVerticesFromKeyword(QString keyword)
{
    QString section = "";
    searchKeyValue(QStringList() << keyword, section);
    return extractVertices(section);
}

QVector<long> Splitter::extractVerticesFromKeyword(QString keyword1, QString keyword2, QString keyword3, QString keyword4)
{
    int value1 = 0;
    int value2 = 0;
    int value3 = 0;
    int value4 = 0;
    searchKeyValue(QStringList() << keyword1, value1);
    searchKeyValue(QStringList() << keyword2, value2);
    searchKeyValue(QStringList() << keyword3, value3);
    searchKeyValue(QStringList() << keyword4, value4);

    QVector<long> vertices;
    vertices << value1 - 1 << value2 - 1 << value3 - 1 << value4 - 1;   // The -1 accounts for C++ indexing starting at zero

    return vertices;
}

// For SuprimeCam_200808-201705, only
QVector<long> Splitter::extractReducedOverscanFromKeyword(QString keyword1, QString keyword2, int value3, int value4)
{
    int value1 = 0;
    int value2 = 0;
    searchKeyValue(QStringList() << keyword1, value1);
    searchKeyValue(QStringList() << keyword2, value2);

    QVector<long> vertices;
    vertices << value1 - 1 << value2 - 1 << value3 - 1 << value4 - 1;   // The -1 accounts for C++ indexing starting at zero

    return vertices;
}

// For SuprimeCam_200808-201705, only
QVector<long> Splitter::extractReducedIlluminationFromKeyword(QString keyword1, QString keyword2, int value3, int value4)
{
    int value1 = 0;
    int value2 = 0;
    searchKeyValue(QStringList() << keyword1, value1);
    searchKeyValue(QStringList() << keyword2, value2);

    QVector<long> vertices;
    vertices << value1 - 1 << value2 - 1 << value3 - 1 << value4 - 1;   // The -1 accounts for C++ indexing starting at zero

    return vertices;
}

void Splitter::updateMEFpastingStatus(int chip)
{
    MEFpastingFinished = false;

    if (!multiChannelMultiExt.contains(instData.name)) {
        MEFpastingFinished = true;
        return;
    }

    // The following instruments store the multiple readout channels of their detectors in separate FITS extensions, and must be pasted back together:
    // Once the last channel has been read, we can paste the chips together
    if (instData.name.contains("GMOS")) {
        if ( (chip + 1) % numAmpPerChip == 0) MEFpastingFinished = true;
        //        if (chip == 3 || chip == 7 || chip == 11) MEFpastingFinished = true;
    }
    if (instData.name == "SOI@SOAR"
            || instData.name == "MOSAIC-II_16@CTIO") {
        if ( (chip +1) % numAmpPerChip == 0) MEFpastingFinished = true;
    }
}
