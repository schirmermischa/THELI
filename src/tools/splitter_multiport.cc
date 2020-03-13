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
#include "../myfits/myfits.h"
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
// For others, where multiple channels are pasted into a single FITS extension, we must reply on the headers and do individual implementations
void Splitter::getMultiportInformation(int chip)
{
    multiportOverscanSections.clear();
    multiportIlluminatedSections.clear();
    multiportChannelSections.clear();
    multiportGains.clear();
    multiportOverscanDirections.clear();
    bool individualFixDone = false;

    naxis1 = dataSection[chip][1] - dataSection[chip][0] + 1;
    naxis2 = dataSection[chip][3] - dataSection[chip][2] + 1;

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
        individualFixDone = true;
        return;
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
        individualFixDone = true;
        return;
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
        channelGains.clear();
        channelGains << 1.0 << 1.0 << 1.0 << 1.0;   // dummy; not used anywhere else for SuprimeCam_200808-201705
        individualFixDone = true;
        return;
    }

    if (instData.name == "LIRIS_POL@WHT") {
        naxis1 = 1024;
        naxis2 = 1024;
        multiportOverscanDirections << "dummy";
        QVector<long> section = {0, naxis1-1, 0, naxis2-1};   // all four channels at the same time. Cutting happens separately during writeImage();
        multiportIlluminatedSections << section;
        multiportChannelSections << section;
        multiportGains << 1.0;
        individualFixDone = true;
        return;
    }

    if (multiportGains.length() != multiportOverscanSections.length()
            || multiportGains.length() != multiportIlluminatedSections.length()) {
        emit messageAvailable("Splitter::getMultiportInformation : Inconsistent number of channels for gain, overscan and data section: "
                              + QString::number(channelGains.length()) + " "
                              + QString::number(multiportOverscanSections.length()) + " "
                              + QString::number(multiportIlluminatedSections.length()), "error");
        emit critical();
        successProcessing = false;
    }

    // What to do for detectors that are not split up by several readout channels and overscans
    if (!individualFixDone) {
        // Append the overscan strips from the instrument.ini files, and padd the missing dimension
        QVector<long> overscan;
        if (!overscanX[chip].isEmpty()) {
            overscan << overscanX[chip][0] << overscanX[chip][1] << 0 << naxis2Raw-1;     // last two elements not used
            multiportOverscanDirections << "vertical";
        }
        if (!overscanY[chip].isEmpty()) {
            overscan << 0 << naxis1Raw-1 << overscanY[chip][2] << overscanY[chip][3];     // first two elements not used
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

void Splitter::pasteMultiportIlluminatedSections(int chip)
{
    // Paste the data sections into a single image of dimensions naxis1, naxis2
    dataCurrent.resize(naxis1*naxis2);
    //    long k = 0;   // the running 1D index in the pasted image
    int channel = 0;
    for (auto &section : multiportIlluminatedSections) {
        if (section.length() != 4) continue; // skip wrong vertices, for whatever reason they might be here
        pasteSubArea(dataCurrent, dataRaw, multiportIlluminatedSections[channel], multiportGains[channel], naxis1, naxis2, naxis1Raw, naxis2Raw);
        ++channel;
    }
}

// paste a subarea 'sector' (xmin, xmax, ymin, ymax) from source image "dataS" to target image "dataT",
// Offsets are calculated internally, ssuming that data sections have equal sizes
void Splitter::pasteSubArea(QVector<float> &dataT, const QVector<float> &dataS, const QVector<long> &section, const float corrFactor,
                            const long nT, const long mT, const long nS, const long mS)
{
    long iminS = section[0];
    long imaxS = section[1];
    long jminS = section[2];
    long jmaxS = section[3];

    // The x- and y-offsets for the illuminated sections. Calculated by taking the lower x (or y) value,
    // and checking how many equally sized sections can fit (safe, as we have usually not more than 4 sections, and the overscan areas are comparatively small)
    long sizex = section[1] - section[0] + 1;     // section x-size
    long sizey = section[3] - section[2] + 1;     // section y-size
    long nsecx = section[0] / sizex;              // number of sections found to the left, using integer division
    long nsecy = section[2] / sizey;              // number of sections found below the botton, using integer division
    long offx = nsecx * sizex;                // The offset for the current section in the pasted output geometry
    long offy = nsecy * sizey;                // The offset for the current section in the pasted output geometry

    for (long jS=jminS; jS<=jmaxS; ++jS) {
        for (long iS=iminS; iS<=imaxS; ++iS) {
            long iT = offx+iS-iminS;
            long jT = offy+jS-jminS;
            if (iT>=nT || iT<0 || jT>=mT || jT<0) continue;   // don't paste pixels falling outside target area
            dataT[iT+nT*jT] = dataS[iS+nS*jS] * corrFactor;   // correcting for gain differences
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
