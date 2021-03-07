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
#include "cfitsioerrorcodes.h"

#include <QString>
#include <QStringList>
#include <QVector>
#include <QFile>
#include <QDir>

/*
// TODO

Instrument slike FORS1/2 come with different collimators, changing the plate scale but not the detector geometries.
What to do with PIXSCALE in this respect/ Do we still need it in the instrument.ini files, or is it better derived from the CD matrix directly?

Binning: include as separate option? need to check various instruments whether their overscan and gain stay the same or not.
Could alternatively offer separate overscan / size vectors in the .ini files. Or even better: automatic detection of the binning mode

Take CRPIX from .ahead file for multi-chip cameras. Don't take CD matrix from ahead file because of changing position angles ...
*/


void Splitter::buildTheliHeader()
{
    if (!successProcessing) return;

    // The following keywords must be present in the THELI headers,
    // AND the values taken from the raw data
    QStringList mandatoryKeys = {"OBJECT"};

    // Loop over all mandatory keywords and try to find a match.
    // Certainly not the most efficient way of doing it, but the overhead should be small
    // as we operate on QStringLists and not FITS files

    QString fallback = "";
    for (auto &mandatoryKey : mandatoryKeys) {
        // Search key in primary and extension header, and append it to 'headerTHELI' if found
        bool keyFound = searchKey(mandatoryKey, headerDictionary.value(mandatoryKey), headerTHELI);

        // Default fallback values
        if (mandatoryKey == "OBJECT" && !keyFound)  fallback = "OBJECT  = 'Unknown'";

        if (!keyFound) {
            if (*verbosity > 1) emit messageAvailable(fileName + " : Could not determine keyword: "+mandatoryKey+", using default: "+fallback, "ignore");
            fallback.resize(80, ' ');
            headerTHELI.append(fallback);
        }
    }

    // Append geographic coordinates
    QString geolon = "GEOLON  = "+QString::number(instData.obslon, 'f', 4);
    QString geolat = "GEOLAT  = "+QString::number(instData.obslat, 'f', 4);
    geolon.resize(80, ' ');
    geolat.resize(80, ' ');
    headerTHELI.append(geolon);
    headerTHELI.append(geolat);

    // Instrument-specific optional keys
    bool keyFoundOptional = true;
    QString instSpecificKey = "";
    if (instData.name == "SPARTAN@SOAR") instSpecificKey = "ROTATOR";
    if (!instSpecificKey.isEmpty()) keyFoundOptional = searchKey(instSpecificKey, {instSpecificKey}, headerTHELI);
    if (!keyFoundOptional) emit messageAvailable(fileName + " : Could not determine instrument-specific keyword: "+instSpecificKey, "warning");

    // Propagate Bayer matrix ID for color CCD chips
    if (!instData.bayer.isEmpty()) {
        QString card = "BAYER   = '"+instData.bayer+"'";
        card.resize(80, ' ');
        headerTHELI.append(card);
    }

    // Append DUMMY keywords
    headerTHELI.append(dummyKeys);

    // Append the THELIPRO keyword to indicate that this  FITS file underwent initial THELI processing
    QString card = "THELIPRO= 1                    / Processed by THELI";
    card.resize(80, ' ');
    headerTHELI.append(card);
}

void Splitter::buildTheliHeaderMJDOBS()
{
    if (!successProcessing) return;

    // List of instruments for which MJD-OBS is not reliable and should be constructed from DATE-OBS
    QStringList instruments = {"GSAOI@GEMINI", "GSAOI_CHIP1@GEMINI", "GSAOI_CHIP2@GEMINI", "GSAOI_CHIP3@GEMINI", "GSAOI_CHIP4@GEMINI",
                               "FLAMINGOS2@GEMINI",
                               "SAMI_2x2@SOAR"          // integer
                              };
    if (instruments.contains(instData.name)) {
        if (!dateObsValue.isEmpty()) {
            mjdobsValue = dateobsToMJD();
            QString mjdCard = "MJD-OBS = "+QString::number(mjdobsValue,'f',7);
            mjdCard.resize(80, ' ');
            headerTHELI.append(mjdCard);
        }
        else {
            mjdobsValue = 58849.000000;
            QString mjdCard = "MJD-OBS = 58849.000000";
            mjdCard.resize(80, ' ');
            headerTHELI.append(mjdCard);
            emit messageAvailable(fileName + " : Could not determine keyword: MJD-OBS, set to 58849.000000 (2020-01-01)<br>"+
                                  "Some processing tasks (background modeling, proper motion correction) will not work correctly.", "warning");
            emit warning();
        }
        return;
    }

    // Search key in primary and extension header, and append it to 'headerTHELI' if found
    bool keyFound = searchKey("MJD-OBS", headerDictionary.value("MJD-OBS"), headerTHELI);
    if (keyFound) return;

    // Calculate MJD-OBS from DATE-OBS if not found
    if (!keyFound && !dateObsValue.isEmpty()) {
        mjdobsValue = dateobsToMJD();
        QString mjdCard = "MJD-OBS = "+QString::number(mjdobsValue,'f',7);
        mjdCard.resize(80, ' ');
        headerTHELI.append(mjdCard);
    }
    else {
        mjdobsValue = 58849.000000;
        QString mjdCard = "MJD-OBS = 58849.000000";
        mjdCard.resize(80, ' ');
        headerTHELI.append(mjdCard);
        emit messageAvailable(fileName + " : Could not determine keyword: MJD-OBS, set to 58849.000000 (2020-01-01)<br>"+
                              "Some processing tasks (background modeling, proper motion correction) will not work correctly.", "warning");
        emit warning();
    }
}

// UNUSED
void Splitter::buildTheliHeaderSATURATION()
{
    QString card = "SATURATE= " + QString::number(saturationValue, 'f', 6) + " / Saturation level for this image";
    card.resize(80, ' ');
    headerTHELI.append(card);
}

void Splitter::buildTheliHeaderWCS(int chip)
{
    if (!successProcessing) return;

    // Each of the following handles their own exceptions for specific instruments
    // (which don't carry a proper and consistent FITS header)

    WCSbuildCTYPE();

    WCSbuildCRVAL();

    WCSbuildCRPIX(chip);

    WCSbuildCDmatrix(chip);

    WCSbuildRADESYS();

    WCSbuildEQUINOX();
}

void Splitter::WCSbuildCRPIX(int chip)
{
    if (!successProcessing) return;

    // Exceptions. Return if successful.
    if (individualFixCRPIX(chip)) return;

    QStringList headerWCS;

    // CRPIXi: Rely on instrument.ini (Todo: scan .ahead files directly for multi-chip cameras)
    QString crpix1_card = "CRPIX1  = "+QString::number(instData.crpix1[chip]);
    QString crpix2_card = "CRPIX2  = "+QString::number(instData.crpix2[chip]);
    crpix1_card.resize(80, ' ');
    crpix2_card.resize(80, ' ');
    headerWCS.append(crpix1_card);
    headerWCS.append(crpix2_card);

    headerTHELI.append(headerWCS);
}

// Instrument dependent
void Splitter::WCSbuildCRVAL()
{
    if (!successProcessing) return;

    // Exceptions. Return if successful.
    if (individualFixCRVAL()) return;

    QStringList headerWCS;

    // Use dedicated lookup
    searchKeyCRVAL("CRVAL1", headerDictionary.value("CRVAL1"), headerWCS);
    searchKeyCRVAL("CRVAL2", headerDictionary.value("CRVAL2"), headerWCS);

    headerTHELI.append(headerWCS);
}

void Splitter::WCSbuildCTYPE()
{
    if (!successProcessing) return;

    QStringList headerWCS;

    // CTYPEi are fixed no matter what!
    QString ctype1_card = "CTYPE1  = 'RA---TAN'";
    QString ctype2_card = "CTYPE2  = 'DEC--TAN'";
    ctype1_card.resize(80, ' ');
    ctype2_card.resize(80, ' ');
    headerWCS.append(ctype1_card);
    headerWCS.append(ctype2_card);

    headerTHELI.append(headerWCS);
}

// Instrument dependent
void Splitter::WCSbuildCDmatrix(int chip)
{
    if (!successProcessing) return;

    // Exceptions. Return if successful.
    if (individualFixCDmatrix(chip)) return;

    QStringList headerWCS;
    QString fallback = "";

    QStringList wcsKeys = {"CD1_1", "CD1_2", "CD2_1", "CD2_2"};

    float flipcd11 = 1.0;
    float flipcd22 = 1.0;
    if (instData.flip == "FLIPX") flipcd11 = -1.0;
    else if (instData.flip == "FLIPY") flipcd22 = -1.0;
    else if (instData.flip == "ROT180") {
        flipcd11 = -1.0;
        flipcd22 = -1.0;
    }

    for (auto &wcsKey : wcsKeys) {
        bool keyFound = searchKey(wcsKey, headerDictionary.value(wcsKey), headerWCS);
        // default values if failed
        if (!keyFound && wcsKey == "CD1_1") {
            bool found = searchKey("CD1_1", headerDictionary.value("CDELT1"), headerWCS);  // first argument, CD1_1, is taken to form the new header card. Value is taken from CDELT1
            if (!found) fallback = "CD1_1   = "+QString::number(-1.*flipcd11*instData.pixscale/3600., 'g', 6);
        }
        if (!keyFound && wcsKey == "CD2_2") {
            bool found = searchKey("CD2_2", headerDictionary.value("CDELT2"), headerWCS);
            if (!found) fallback = "CD2_2   = "+QString::number(flipcd22*instData.pixscale/3600., 'g', 6);
        }
        if (!keyFound && wcsKey == "CD1_2") fallback = "CD1_2   = 0.0";
        if (!keyFound && wcsKey == "CD2_1") fallback = "CD2_1   = 0.0";

        if (!keyFound) {
            if (*verbosity > 1) emit messageAvailable(fileName + " : Could not determine keyword: "+wcsKey+", using default: "+fallback, "ignore");
            fallback.resize(80, ' ');
            headerWCS.append(fallback);
        }
    }

    headerTHELI.append(headerWCS);
}

void Splitter::WCSbuildRADESYS()
{
    if (!successProcessing) return;

    QStringList headerWCS;
    QString wcsKey = "RADESYS";
    bool keyFound = searchKey(wcsKey, headerDictionary.value(wcsKey), headerWCS);
    if (!keyFound) {
        QString card = "RADESYS = 'ICRS'";
        if (*verbosity > 1) emit messageAvailable(fileName + " : Could not determine keyword: "+wcsKey+", using default: "+card, "ignore");
        card.resize(80, ' ');
        headerWCS.append(card);
    }
    headerTHELI.append(headerWCS);
}

void Splitter::WCSbuildEQUINOX()
{
    if (!successProcessing) return;

    if (individualFixEQUINOX()) return;

    QStringList headerWCS;
    QString wcsKey = "EQUINOX";
    bool keyFound = searchKey(wcsKey, headerDictionary.value(wcsKey), headerWCS);
    if (!keyFound) {
        QString card = "EQUINOX = 2000.0";
        if (*verbosity > 1) emit messageAvailable(fileName + " : Could not determine keyword: "+wcsKey+", using default: "+card, "ignore");
        card.resize(80, ' ');
        headerWCS.append(card);
    }
    headerTHELI.append(headerWCS);
}

bool Splitter::individualFixEQUINOX()
{
    if (!successProcessing) return false;

    bool individualFixDone = false;

    QStringList headerWCS;
    QString equinoxCard = "";

    if (instData.name == "SITe3_SWOPE@LCO") {
        equinoxCard = equinoxCard = "EQUINOX = 2000.0";            // SITe3_SWOPE@LCO has EPOCH written to EQUINOX, which makes scamp fail since the coords are in J2000.
        individualFixDone = true;
    }

    if (individualFixDone) {
        equinoxCard.resize(80, ' ');
        headerWCS.append(equinoxCard);
        headerTHELI.append(headerWCS);
    }

    return individualFixDone;
}

bool Splitter::individualFixCRVAL()
{
    if (!successProcessing) return false;

    bool individualFixDone = false;

    QStringList headerWCS;
    QString crval1_card = "";
    QString crval2_card = "";
    QString crval1 = "";
    QString crval2 = "";

    // List of instruments that we have to consider
    QStringList list = {"WFC@INT", "SUSI1@NTT"};

    // Leave if no individual fix is required.
    if (!list.contains(instData.name)) return false;

    // Prepare fix.
    // First, read coords and fix format (sometimes we have 'HH MM SS' instead of 'HH:MM:SS')
    // Convert to decimal format if necessary
    searchKeyValue(headerDictionary.value("CRVAL1"), crval1);
    searchKeyValue(headerDictionary.value("CRVAL2"), crval2);
    crval1.replace(' ', ':');
    crval2.replace(' ', ':');
    if (crval1.contains(':')) crval1 = hmsToDecimal(crval1);
    if (crval2.contains(':')) crval2 = dmsToDecimal(crval2);

    // Here are the individual fixes
    if (instData.name == "WFC@INT") {
        double alpha = crval1.toDouble();
        double delta = crval2.toDouble();
        // reset the coordinates such that scamp does not get confused (optical axis != crpix by ~4 arcminutes)
        alpha = alpha - 0.0733 / cos(delta*rad);
        if (alpha > 360.) alpha -= 360.;
        if (alpha < 0.) alpha += 360.;
        delta = delta - 0.02907;
        crval1_card = "CRVAL1  = "+QString::number(alpha, 'f', 6);
        crval2_card = "CRVAL2  = "+QString::number(delta, 'f', 6);
        individualFixDone = true;
    }
    if (instData.name == "SUSI1@NTT") {
        searchKeyValue(QStringList() << "RA", crval1);
        searchKeyValue(QStringList() << "DEC", crval2);
        crval1_card = "CRVAL1  = "+crval1;
        crval2_card = "CRVAL2  = "+crval2;
        individualFixDone = true;
    }

    if (individualFixDone) {
        crval1_card.resize(80, ' ');
        crval2_card.resize(80, ' ');
        headerWCS.append(crval1_card);
        headerWCS.append(crval2_card);
        headerTHELI.append(headerWCS);
    }

    return individualFixDone;
}

bool Splitter::individualFixCRPIX(int chip)
{
    if (!successProcessing) return false;

    bool individualFixDone = false;

    QStringList headerWCS;
    QString crpix1_card = "";
    QString crpix2_card = "";
    float crpix1 = 0.;
    float crpix2 = 0.;

    // Leave if no individual fix is required.
    if (!multiChannelMultiExt.contains(instData.name)) return false;

    // Prepare fix.
    // Convert to decimal format if necessary
    searchKeyValue(headerDictionary.value("CRPIX1"), crpix1);
    searchKeyValue(headerDictionary.value("CRPIX2"), crpix2);

    // Solutions per individual channel (will be ignored)
    crpix1_card = "CRPIX1  = "+QString::number(crpix1, 'f', 2);
    crpix2_card = "CRPIX2  = "+QString::number(crpix2, 'f', 2);

    // Here are the individual fixes
    if (instData.name == "GMOS-N-HAM_1x1@GEMINI" || instData.name == "GMOS-S-HAM_1x1@GEMINI") {
        if (chip == 3) crpix1_card = "CRPIX1  = 3180";
        if (chip == 7) crpix1_card = "CRPIX1  = 1088";
        if (chip == 11) crpix1_card = "CRPIX1  = -1004";
        crpix2_card = "CRPIX2  = 2304";
        individualFixDone = true;
    }

    if (instData.name == "GMOS-N-HAM@GEMINI" || instData.name == "GMOS-S-HAM@GEMINI") {
        if (chip == 3) crpix1_card = "CRPIX1  = 1589";
        if (chip == 7) crpix1_card = "CRPIX1  = 544";
        if (chip == 11) crpix1_card = "CRPIX1  = -502";
        crpix2_card = "CRPIX2  = 1152";
        individualFixDone = true;
    }

    if (instData.name == "SOI@SOAR") {
        if (chip == 1) crpix1_card = "CRPIX1  = 1051";
        if (chip == 3) crpix1_card = "CRPIX1  = -26";
        crpix2_card = "CRPIX2  = 1024";
        individualFixDone = true;
    }

    if (instData.name == "LRIS_BLUE@KECK") {
        if (chip == 1) crpix1_card = "CRPIX1  = 2099";
        if (chip == 3) crpix1_card = "CRPIX1  = -52";
        crpix2_card = "CRPIX2  = 2052";
        individualFixDone = true;
    }

    if (instData.name == "LRIS_RED@KECK") {
        if (chip == 1) crpix1_card = "CRPIX1  = -126";
        if (chip == 3) crpix1_card = "CRPIX1  = 1835";
        crpix2_card = "CRPIX2  = 1260";
        individualFixDone = true;
    }

    if (instData.name == "MOSAIC-II_16@CTIO") {
        if (chip == 1) crpix1_card = "CRPIX1  = 4219";
        if (chip == 3) crpix1_card = "CRPIX1  = 2078";
        if (chip == 5) crpix1_card = "CRPIX1  = -33";
        if (chip == 7) crpix1_card = "CRPIX1  = -2166";
        if (chip == 9) crpix1_card = "CRPIX1  = 4221";
        if (chip == 11) crpix1_card = "CRPIX1  = 2081";
        if (chip == 13) crpix1_card = "CRPIX1  = -31";
        if (chip == 15) crpix1_card = "CRPIX1  = -2169";
        if (chip == 1) crpix2_card = "CRPIX2  = 4148";
        if (chip == 3) crpix2_card = "CRPIX2  = 4121";
        if (chip == 5) crpix2_card = "CRPIX2  = 4119";
        if (chip == 7) crpix2_card = "CRPIX2  = 4136";
        if (chip == 9) crpix2_card = "CRPIX2  = -34";
        if (chip == 11) crpix2_card = "CRPIX2  = -24";
        if (chip == 13) crpix2_card = "CRPIX2  = -26";
        if (chip == 15) crpix2_card = "CRPIX2  = -56";
        individualFixDone = true;
    }
    if (instData.name == "PISCO@LCO") {
        if (chip == 1) crpix1_card = "CRPIX1  = 1500";
        if (chip == 3) crpix1_card = "CRPIX1  = 1500";
        if (chip == 5) crpix1_card = "CRPIX1  = 1500";
        if (chip == 7) crpix1_card = "CRPIX1  = 1500";
        if (chip == 1) crpix2_card = "CRPIX2  = 3000";
        if (chip == 3) crpix2_card = "CRPIX2  = 3000";
        if (chip == 5) crpix2_card = "CRPIX2  = 3000";
        if (chip == 7) crpix2_card = "CRPIX2  = 3000";
        individualFixDone = true;
    }
    if (instData.name == "MOSAIC-III_4@KPNO_4m") {
        if (chip == 3) crpix1_card = "CRPIX1  = 4219";
        if (chip == 7) crpix1_card = "CRPIX1  = 2078";
        if (chip == 11) crpix1_card = "CRPIX1  = -33";
        if (chip == 15) crpix1_card = "CRPIX1  = -2166";
        if (chip == 3) crpix2_card = "CRPIX2  = 4148";
        if (chip == 7) crpix2_card = "CRPIX2  = 4121";
        if (chip == 11) crpix2_card = "CRPIX2  = 4119";
        if (chip == 15) crpix2_card = "CRPIX2  = 4136";
        individualFixDone = true;
    }
    if (instData.name == "SAMI_2x2@SOAR") {
        if (chip == 3) {
            crpix1_card = "CRPIX1  = 1024";
            crpix1_card = "CRPIX1  = 1024";
        }
        individualFixDone = true;
    }

    // Append only when all channels of one chip have been read (i.e., 'individualFixDone' == true)
    if (individualFixDone) {
        crpix1_card.resize(80, ' ');
        crpix2_card.resize(80, ' ');
        headerWCS.append(crpix1_card);
        headerWCS.append(crpix2_card);
        headerTHELI.append(headerWCS);
    }

    return individualFixDone;
}

bool Splitter::individualFixCDmatrix(int chip)
{
    if (!successProcessing) return false;

    bool individualFixDone = false;

    QStringList headerWCS;

    QString cd11_card = "";
    QString cd12_card = "";
    QString cd21_card = "";
    QString cd22_card = "";

    if (instData.name == "Direct_2k_DUPONT@LCO") {
        cd11_card = "CD1_1   =  0.0";
        cd12_card = "CD1_2   =  -7.2273e-5";
        cd21_card = "CD2_1   =  -7.2273e-5";
        cd22_card = "CD2_2   =  0.0";
        individualFixDone = true;
    }
    if (instData.name == "SITe3_SWOPE@LCO") {
        cd11_card = "CD1_1   =  -1.2083e-4";
        cd12_card = "CD1_2   =  0.0";
        cd21_card = "CD2_1   =  0.0";
        cd22_card = "CD2_2   =  -1.2083e-4";
        individualFixDone = true;
    }
    if (instData.name == "Direct_4k_SWOPE@LCO") {
        if (chip == 0) {
            cd11_card = "CD1_1   =  8.839493716E-07";
            cd12_card = "CD1_2   =  -1.209379295E-04";
            cd21_card = "CD2_1   =  -1.209379295E-04";
            cd22_card = "CD2_2   =  -8.839493716E-07";
        }
        if (chip == 1) {
            cd11_card = "CD1_1   =  -8.861927404E-07";
            cd12_card = "CD1_2   =  -1.209344868E-04";
            cd21_card = "CD2_1   =  1.209344868E-04";
            cd22_card = "CD2_2   =  -8.861927404E-07";
        }
        if (chip == 2) {
            cd11_card = "CD1_1   =  -7.788683939E-07";
            cd12_card = "CD1_2   =  1.211132665E-04";
            cd21_card = "CD2_1   =  1.211132665E-04";
            cd22_card = "CD2_2   =  7.788683939E-07";
        }
        if (chip == 3) {
            cd11_card = "CD1_1   =  8.755109826E-07";
            cd12_card = "CD1_2   =  1.208828016E-04";
            cd21_card = "CD2_1   =  -1.208828016E-04";
            cd22_card = "CD2_2   =  8.755109826E-07";
        }
        individualFixDone = true;
    }
    if (instData.name == "LDSS3_from201402@LCO") {
        if (chip == 0) {
            cd11_card = "CD1_1   =  -5.213e-5";
            cd12_card = "CD1_2   =  0.0";
            cd21_card = "CD2_1   =  0.0";
            cd22_card = "CD2_2   =  5.213e-5";
        }
        if (chip == 1) {
            cd11_card = "CD1_1   =  5.230e-5";
            cd12_card = "CD1_2   =  0.0";
            cd21_card = "CD2_1   =  0.0";
            cd22_card = "CD2_2   =  5.230e-5";
        }
        individualFixDone = true;
    }
    if (instData.name == "WFI@MPGESO") {
        cd11_card = "CD1_1   =  -6.611e-5";
        cd12_card = "CD1_2   =  0.0";
        cd21_card = "CD2_1   =  0.0";
        cd22_card = "CD2_2   =  6.611e-5";
        individualFixDone = true;
    }
    if (instData.name == "DEIMOS_2AMP@KECK") {
        if (chip %2 == 1) {
            cd11_card = "CD1_1   =  0.0";
            cd12_card = "CD1_2   =  3.29167e-5";
            cd21_card = "CD2_1   =  3.29167e-5";
            cd22_card = "CD2_2   =  0.0";
        }
        else {
            cd11_card = "CD1_1   =  0.0";
            cd12_card = "CD1_2   =  3.29167e-5";
            cd21_card = "CD2_1   =  -3.29167e-5";
            cd22_card = "CD2_2   =  0.0";
        }
        individualFixDone = true;
    }
    if (instData.name == "DEIMOS_1AMP@KECK") {
        cd11_card = "CD1_1   =  0.0";
        cd12_card = "CD1_2   =  3.28000e-5";
        cd21_card = "CD2_1   =  3.28000e-5";
        cd22_card = "CD2_2   =  0.0";
        individualFixDone = true;
    }
    if (instData.name == "LRIS_BLUE@KECK") {
        cd11_card = "CD1_1   =  0.0";
        cd12_card = "CD1_2   =  -3.76e-5";
        cd21_card = "CD2_1   =  3.76e-5";
        cd22_card = "CD2_2   =  0.0";
        individualFixDone = true;
    }
    if (instData.name == "LRIS_RED@KECK") {
        cd11_card = "CD1_1   =  -3.76e-5";
        cd12_card = "CD1_2   =  0.0";
        cd21_card = "CD2_1   =  0.0";
        cd22_card = "CD2_2   =  -3.76e-5";
        individualFixDone = true;
    }
    if (instData.name == "ESI@KECK") {
        cd11_card = "CD1_1   =  -4.28333e-5";
        cd12_card = "CD1_2   =  0.0";
        cd21_card = "CD2_1   =  0.0";
        cd22_card = "CD2_2   =  -4.28333e-5";
        individualFixDone = true;
    }
    if (instData.name.contains("WFI_2x2") && instData.name.contains("MPGESO") ) {
        cd11_card = "CD1_1   =  -1.322e-4";
        cd12_card = "CD1_2   =  0.0";
        cd21_card = "CD2_1   =  0.0";
        cd22_card = "CD2_2   =  1.322e-4";
        individualFixDone = true;
    }
    if (instData.name == "WFC@INT") {
        if (chip == 0) {
            cd11_card = "CD1_1   =  -1.186589131599E-06";
            cd12_card = "CD1_2   =  -9.208350034543E-05";
            cd21_card = "CD2_1   =  -9.202558574925E-05";
            cd22_card = "CD2_2   =   9.373099270996E-07";
        }
        if (chip == 1) {
            cd11_card = "CD1_1   =   9.158969785153E-05";
            cd12_card = "CD1_2   =   1.000429584977E-07";
            cd21_card = "CD2_1   =  -8.707577386754E-08";
            cd22_card = "CD2_2   =  -9.204121646891E-05";
        }
        if (chip == 2) {
            cd11_card = "CD1_1   =  -1.101867868104E-06";
            cd12_card = "CD1_2   =  -9.186460105657E-05";
            cd21_card = "CD2_1   =  -9.119982231051E-05";
            cd22_card = "CD2_2   =   1.393090409586E-06";
        }
        if (chip == 3) {
            cd11_card = "CD1_1   =  -9.862265741128E-07";
            cd12_card = "CD1_2   =  -9.221689418834E-05";
            cd21_card = "CD2_1   =  -9.224461667406E-05";
            cd22_card = "CD2_2   =   1.077599414761E-06";
        }
        individualFixDone = true;
    }
    if (instData.name == "FourStar@LCO") {     // FourStar has no CD matrix in the header
        if (!searchKeyValue(QStringList() << "ROTANGLE", positionAngle)) {
            emit messageAvailable(name + " : Could not find ROTANGLE keyword, set to zero! CD matrix might have wrong orientation.", "warning");
            emit warning();
            positionAngle = 0.0;
        }
        double cd11 = -1.*instData.pixscale / 3600.;
        double cd12 = 0.0;
        double cd21 = 0.0;
        double cd22 = instData.pixscale / 3600.;
        rotateCDmatrix(cd11, cd12, cd21, cd22, positionAngle);
        cd11_card = "CD1_1   =  "+QString::number(cd11, 'g', 6);
        cd12_card = "CD1_2   =  "+QString::number(cd12, 'g', 6);
        cd21_card = "CD2_1   =  "+QString::number(cd21, 'g', 6);
        cd22_card = "CD2_2   =  "+QString::number(cd22, 'g', 6);
        individualFixDone = true;
    }
    if (instData.name.contains("IMACS")) {     // IMACS has no CD matrix in the header
        if (!searchKeyValue(QStringList() << "ROTATORE", positionAngle)) {
            emit messageAvailable(name + " : Could not find ROTANGLE keyword, set to zero! CD matrix might have wrong orientation.", "warning");
            emit warning();
            positionAngle = 0.0;
        }

        double cd11 = instData.pixscale / 3600.;
        double cd12 = 0.;
        double cd21 = 0.;
        double cd22 = -instData.pixscale / 3600.;
        // Chips 1-4 are rotated by 180 degrees
        if (chip >= 4) rotateCDmatrix(cd11, cd12, cd21, cd22, positionAngle);
        else rotateCDmatrix(cd11, cd12, cd21, cd22, positionAngle+180.);
        cd11_card = "CD1_1   =  "+QString::number(cd11, 'g', 6);
        cd12_card = "CD1_2   =  "+QString::number(cd12, 'g', 6);
        cd21_card = "CD2_1   =  "+QString::number(cd21, 'g', 6);
        cd22_card = "CD2_2   =  "+QString::number(cd22, 'g', 6);
        individualFixDone = true;
    }
    if (instData.name == "LIRIS@WHT" || instData.name == "LIRIS_POL@WHT") {     // LIRIS has no CD matrix in the header
        if (!searchKeyValue(QStringList() << "ROTSKYPA", positionAngle)) {
            emit messageAvailable(name + " : Could not find ROTSKYPA keyword, set to zero! CD matrix might have wrong orientation.", "warning");
            emit warning();
            positionAngle = 0.0;
        }
        double cd11 = -1.*instData.pixscale / 3600.;
        double cd12 = 0.0;
        double cd21 = 0.0;
        double cd22 = instData.pixscale / 3600.;
        rotateCDmatrix(cd11, cd12, cd21, cd22, positionAngle);
        cd11_card = "CD1_1   =  "+QString::number(cd11, 'g', 6);
        cd12_card = "CD1_2   =  "+QString::number(cd12, 'g', 6);
        cd21_card = "CD2_1   =  "+QString::number(cd21, 'g', 6);
        cd22_card = "CD2_2   =  "+QString::number(cd22, 'g', 6);
        individualFixDone = true;
    }
    if (instData.name == "SUSI1@NTT") {
        double cd11 = -3.611e-5;
        double cd12 = 0.;
        double cd21 = 0.;
        double cd22 = 3.611e-5;
        cd11_card = "CD1_1   =  "+QString::number(cd11, 'g', 6);
        cd12_card = "CD1_2   =  "+QString::number(cd12, 'g', 6);
        cd21_card = "CD2_1   =  "+QString::number(cd21, 'g', 6);
        cd22_card = "CD2_2   =  "+QString::number(cd22, 'g', 6);
        individualFixDone = true;
    }
    if (instNameFromData == "GROND_OPT@MPGESO") {                 // GROND optical data has both the NIR and OPT CD matrices in the header.
        double cd11 = 0.;                                         // With the current scheme, the NIR matrix in the HDU gets picked over
        double cd12 = 0.;                                         // OPT matrix in the extension
        double cd21 = 0.;
        double cd22 = 0.;
        searchKeyValue(QStringList() << "CD1_1", cd11);
        searchKeyValue(QStringList() << "CD1_2", cd12);
        searchKeyValue(QStringList() << "CD2_1", cd21);
        searchKeyValue(QStringList() << "CD2_2", cd22);
        cd11_card = "CD1_1   =  "+QString::number(cd11, 'g', 6);
        cd12_card = "CD1_2   =  "+QString::number(cd12, 'g', 6);
        cd21_card = "CD2_1   =  "+QString::number(cd21, 'g', 6);
        cd22_card = "CD2_2   =  "+QString::number(cd22, 'g', 6);
        individualFixDone = true;
    }
    if (instNameFromData == "GROND_NIR@MPGESO") {                 // just double tapping
        double cd11 = 0.;
        double cd12 = 0.;
        double cd21 = 0.;
        double cd22 = 0.;
        searchKeyValue(QStringList() << "J_CD1_1", cd11);
        searchKeyValue(QStringList() << "J_CD1_2", cd12);
        searchKeyValue(QStringList() << "J_CD2_1", cd21);
        searchKeyValue(QStringList() << "J_CD2_2", cd22);
        cd11_card = "CD1_1   =  "+QString::number(cd11, 'g', 6);
        cd12_card = "CD1_2   =  "+QString::number(cd12, 'g', 6);
        cd21_card = "CD2_1   =  "+QString::number(cd21, 'g', 6);
        cd22_card = "CD2_2   =  "+QString::number(cd22, 'g', 6);
        individualFixDone = true;
    }
    if (instData.name == "PFC_old@WHT") {
        double cd11 = 6.55e-5;
        double cd12 = 5.0e-7;
        double cd21 = 5.0e-7;
        double cd22 = -6.55e-5;
        cd11_card = "CD1_1   =  "+QString::number(cd11, 'g', 6);
        cd12_card = "CD1_2   =  "+QString::number(cd12, 'g', 6);
        cd21_card = "CD2_1   =  "+QString::number(cd21, 'g', 6);
        cd22_card = "CD2_2   =  "+QString::number(cd22, 'g', 6);
        individualFixDone = true;
    }

    if (individualFixDone) {
        cd11_card.resize(80, ' ');
        cd12_card.resize(80, ' ');
        cd21_card.resize(80, ' ');
        cd22_card.resize(80, ' ');

        headerWCS.append(cd11_card);
        headerWCS.append(cd12_card);
        headerWCS.append(cd21_card);
        headerWCS.append(cd22_card);

        headerTHELI.append(headerWCS);
    }

    return individualFixDone;
}


// Build the EXPTIME keyword
void Splitter::buildTheliHeaderEXPTIME()
{
    if (!successProcessing) return;

    // List of instruments that have separate DITs and NDITs
    QStringList nditInstruments = {"GSAOI@GEMINI", "GSAOI_CHIP1@GEMINI", "GSAOI_CHIP2@GEMINI", "GSAOI_CHIP3@GEMINI", "GSAOI_CHIP4@GEMINI",
                                   "HAWKI@VLT", "INGRID@WHT", "IRCS_HIGHRES@SUBARU", "IRCS_LOWRES@SUBARU", "ISAAC@VLT", "ISPI@CTIO",
                                   "LIRIS@WHT", "LIRIS_POL@WHT", "MOIRCS@SUBARU", "MOSFIRE@KECK", "NEWFIRM@CTIO", "NICS@TNG", "NIRC2@KECK",
                                   "NIRI@GEMINI", "PISCES@LBT", "SOFI@NTT", "VIRCAM@VISTA"};

    // The following instruments also have DITs and NDITs, but they are coadded instead of coaveraged and thus can be treated normally:
    // MOIRCS

    QString exptimeKey;

    // Instruments for which we don't have to do anything special
    if (!nditInstruments.contains(instData.name)) {
        bool keyFound = searchKey("EXPTIME", headerDictionary.value("EXPTIME"), headerTHELI);
        if (!keyFound) {
            exptimeKey = "EXPTIME = 1.0";
            exptimeKey.resize(80, ' ');
            headerTHELI.append(exptimeKey);
            emit messageAvailable(fileName + " : Could not determine keyword: EXPTIME, set to 1.0", "warning");
            emit warning();
        }
        return;
    }

    // Instruments (see list above) for which we have to determine the true total EXPTIME keyword, and rescale the data.
    // In THELI, EXPTIME always represents the total effective integration time. If images are averaged from several coadds,
    // they also need to be rescaled.

    float dit = -1.0;
    float ndit = -1.0;

    bool foundDIT = searchKeyValue(headerDictionary.value("DIT"), dit);
    bool foundNDIT = searchKeyValue(headerDictionary.value("NDIT"), ndit);

    // default values if failed
    if (!foundDIT) {
        emit messageAvailable(fileName + " : Could not determine keyword: DIT, set to 1.0", "warning");
        dit = 1.0;
    }
    if (!foundNDIT) {
        emit messageAvailable(fileName + " : Could not determine keyword: NDIT, set to 1", "warning");
        ndit = 1.0;
    }
    if (!foundDIT || !foundNDIT) {
        emit messageAvailable("This is a serious issue with data from "+instData.name+" .<br>The true exposure time is unknown."+
                              "You can continue, but a correct calibration of the stacked image is not guaranteed.", "warning");
        emit warning();
    }

    exptimeValue = dit*ndit;
    exptimeKey      = "EXPTIME = "+QString::number(exptimeValue, 'f', 5);
    QString ditKey  = "DIT     = "+QString::number(dit, 'f', 5);
    QString nditKey = "NDIT    = "+QString::number(int(ndit));

    exptimeKey.resize(80, ' ');
    ditKey.resize(80, ' ');
    nditKey.resize(80, ' ');
    headerTHELI.append(exptimeKey);
    headerTHELI.append(ditKey);
    headerTHELI.append(nditKey);

    // Rescale the pixels. NDIT is actually an integer and must be >= 1.
    // The following cameras DO NOT average the NDITs but add them directly, hence we must not rescale them.
    // TODO: this list must be verified!
    QStringList directCoaddition = {"IRCS_HIGHRES@SUBARU", "IRCS_LOWRES@SUBARU", "ISPI@CTIO", "MOSFIRE@KECK", "NIRC2@KECK",
                                    "NIRI@GEMINI", "PISCES@LBT", "VIRCAM@VISTA"};

    if (!directCoaddition.contains(instData.name)) {
        if (ndit > 1.) {
            for (auto &pixel : dataCurrent) pixel *= ndit;
        }
    }
}

// Build the DATE-OBS keyword
void Splitter::buildTheliHeaderDATEOBS()
{
    if (!successProcessing) return;

    if (individualFixDATEOBS()) return;

    bool found = false;
    // Loop over all possible dateobs keywords, and break once we found one that is valid.
    // This is different from the general strategy to go over all possible keyword variants and take the first one that exists
    for (auto &keyword : headerDictionary.value("DATE-OBS")) {
        dateObsValue = "";
        searchKeyValue(QStringList() << keyword, dateObsValue);
        if (!dateObsValue.isEmpty() && checkFormatDATEOBS()) {
            found = true;
            break;
        }
    }

    //    bool keyFound = searchKey("DATE-OBS", headerDictionary.value("DATE-OBS"), headerTHELI);
    //    if (keyFound && checkFormatDATEOBS()) return;

    // Fallback: Try and reconstruct DATE-OBS keyword from other keywords
    // DATE-OBS has not been appended yet by searchKey() child function if format is wrong
    if (!found) {
        QString dateValue;
        QString timeValue;
        bool foundDATE = searchKeyValue(headerDictionary.value("DATE"), dateValue);
        bool foundTIME = searchKeyValue(headerDictionary.value("TIME"), timeValue);
        if (foundDATE && foundTIME) {
            if (timeValue.contains(" ")) timeValue.replace(" ",":");         // E.g. SITe_SWOPE@LCO has a time stamp without colons
            if (dateValue.contains("-") && timeValue.contains(":")) {
                dateObsValue = dateValue+"T"+timeValue;
            }
            else {
                // Construct a unique dummy DATE-OBS keyword, incremented by 0.1 seconds.
#pragma omp critical
                {
                    *dateObsIncrementor += 0.1;
                    QString timeStamp = decimalSecondsToHms(*dateObsIncrementor);
                    dateObsValue = "2020-01-01T"+timeStamp;
                }
                emit messageAvailable(fileName + " : Could not determine keyword: DATE-OBS, set to "+dateObsValue, "warning");
                emit warning();
                //                dateObsValue = "2020-01-01T00:00:00.000";
                //                dateObsValid = false;
            }
        }
    }

    QString card = "DATE-OBS= '"+dateObsValue+"'";
    card.resize(80, ' ');
    headerTHELI.append(card);
}

bool Splitter::individualFixDATEOBS()
{
    bool individualFixDone = false;

    if (instData.name == "WHIRC@WIYN") {
        double mjdValue = 0;
        bool foundMJDOBS = searchKeyValue(headerDictionary.value("MJD-OBS"), mjdValue);
        // DATE-OBS has the start time of a sequence
        if (foundMJDOBS) {
            dateObsValue = mjdobsToDATEOBS(mjdValue);
            individualFixDone = true;
        }
    }

    if (instData.name == "PISCO@LCO") {   // wrong order of DATE and DATEOBS keyword
        QString dateValue;
        QString timeValue;
        bool foundDATE = searchKeyValue(QStringList() << "DATEOBS", dateValue);
        bool foundTIME = searchKeyValue(QStringList() << "TELUT", timeValue);
        if (foundDATE && foundTIME) {
            if (dateValue.contains("-") && timeValue.contains(":")) {
                dateObsValue = dateValue+"T"+timeValue;
            }
            else {
                // Construct a unique dummy DATE-OBS keyword, incremented by 0.1 seconds.
#pragma omp critical
                {
                    *dateObsIncrementor += 0.1;
                    QString timeStamp = decimalSecondsToHms(*dateObsIncrementor);
                    dateObsValue = "2020-01-01T"+timeStamp;
                }
                emit messageAvailable(fileName + " : Could not determine keyword: DATE-OBS, set to "+dateObsValue, "warning");
                emit warning();
            }
        }
        individualFixDone = true;
    }

    if (individualFixDone) {
        QString card = "DATE-OBS= '"+dateObsValue+"'";
        card.resize(80, ' ');
        headerTHELI.append(card);
    }

    return individualFixDone;
}

// Build the GAIN keyword
void Splitter::buildTheliHeaderGAIN(int chip)
{
    if (!successProcessing) return;

    // Exceptions. Return if successful.
    if (individualFixGAIN(chip)) return;

    // normal cases
    chipGain = 1.0;
    if (!searchKeyValue(headerDictionary.value("GAIN"), chipGain)) {
        //        if (instData.name != "GROND_NIR@MPGESO") {    // GROND: gain determined in writeImageIndividual()
        if (instNameFromData != "GROND_NIR@MPGESO") {    // GROND: gain determined in writeImageIndividual()
            emit messageAvailable(fileName + " : Could not determine keyword: GAIN, set to 1.0.", "warning");
            emit warning();
        }
        chipGain = 1.0;
    }

    // Consistency checks
    if (chipGain < 0.02 || chipGain > 30.) {
        emit messageAvailable(fileName + " : GAIN keyword outside plausible range (0.02-30 e-/ADU): " + QString::number(chipGain)+", set to 1.0.", "warning");
        emit warning();
        chipGain = 1.0;
    }

    QString card1 = "GAINORIG= "+QString::number(chipGain, 'f', 6) + " / Original gain in the raw data for this image";
    QString card2 = "GAIN    = 1.0     / ADUs were converted to e- in this image using GAINORIG";
    card1.resize(80, ' ');
    card2.resize(80, ' ');
    headerTHELI.append(card1);
    headerTHELI.append(card2);

    gainForSaturation = chipGain;
    gain[chip] = chipGain;   // used to convert the pixel data from ADU to electrons
}

bool Splitter::individualFixGAIN(int chip)
{
    if (!successProcessing) return false;

    bool individualFixDone = false;

    chipGain = 1.0;
    if (instData.name == "HAWKI@VLT") {           // https://www.eso.org/sci/facilities/paranal/instruments/hawki/inst.html
        if (chip == 0) chipGain = 1.705;
        if (chip == 1) chipGain = 1.870;
        if (chip == 2) chipGain = 1.735;
        if (chip == 3) chipGain = 2.110;
        individualFixDone = true;
    }
    if (instData.name == "NEWFIRM@KPNO_4m") {    // https://www.noao.edu/ets/newfirm/documents/ORION_SCA_lab_tests_final.pdf
        chipGain = 7.6;                          // same for all 4 chips
        individualFixDone = true;
    }
    if (instData.name == "HDI@KPNO_0.9m") {      // https://www.noao.edu/0.9m/observe/hdi/hdi_manual.html
        chipGain = 1.3;
        individualFixDone = true;
    }
    if (instData.name == "WHIRC@WIYN") {         // https://www.noao.edu/kpno/manuals/whirc/whirc.user.html
        chipGain = 3.4;
        individualFixDone = true;
    }
    else if (instData.name == "NIRI@GEMINI") {    // https://www.gemini.edu/sciops/instruments/niri/imaging/detector-array
        chipGain = 12.3;                          // No gain keyword in FITS header
        individualFixDone = true;
    }
    /*
     * UPDATE: The gain in the LIRIS headers follows the actual gain setting, might not be accurate to the decimal, though.
     * (email from R. Karjalainen, ING, 2019-11-07)
    else if (instData.name == "LIRIS@WHT" || instData.name == "LIRIS_POL@WHT") {    // http://www.ing.iac.es/astronomy/instruments/liris/detector.html
        chipGain = 3.6;                           // Wrong gain in header
        individualFixDone = true;
    }
    */
    else if (instData.name == "MOIRCS_200807-201505@SUBARU") {  // https://www.naoj.org/Observing/Instruments/MOIRCS/OLD/inst_detector_oldMOIRCS.html
        if (chip == 0) chipGain = 3.50;           // Wrong in headers between August 2008 and April 2010
        if (chip == 1) chipGain = 3.30;
        individualFixDone = true;
    }
    else if (instData.name.contains("WFI") && instData.name.contains("MPGESO")) {  // http://www.ls.eso.org:8081/sci/facilities/lasilla/sciops/CCDs/WFI/qc_suite/plots/plot1.png
        if (chip == 0) chipGain = 1.99;        // Does not have GAIN keywords for all detectors
        if (chip == 1) chipGain = 2.02;
        if (chip == 2) chipGain = 2.29;
        if (chip == 3) chipGain = 2.68;
        if (chip == 4) chipGain = 2.24;
        if (chip == 5) chipGain = 2.25;
        if (chip == 6) chipGain = 2.16;
        if (chip == 7) chipGain = 2.03;
        individualFixDone = true;
    }
    else if (instData.name == "DEIMOS_1AMP@KECK") {
        if (chip == 0) chipGain = 1.206;
        if (chip == 1) chipGain = 1.200;
        if (chip == 2) chipGain = 1.167;
        if (chip == 3) chipGain = 1.217;
        individualFixDone = true;
    }
    else if (instData.name == "DEIMOS_2AMP@KECK") {  // https://www2.keck.hawaii.edu/inst/obsdata/inst/deimos/www/detector_data/deimos_detector_data.html
        if (chip == 0) chipGain = 1.206;
        if (chip == 1) chipGain = 1.221;
        if (chip == 2) chipGain = 1.200;
        if (chip == 3) chipGain = 1.188;
        if (chip == 4) chipGain = 1.167;
        if (chip == 5) chipGain = 1.250;
        if (chip == 6) chipGain = 1.217;
        if (chip == 7) chipGain = 1.228;
        individualFixDone = true;
    }
    else if (instData.name == "SuprimeCam_200808-201705@SUBARU") {
        chipGain = harmonicGain(multiportGains);
        individualFixDone = true;
    }
    else if (instData.name.contains("GMOS-N-HAM") || instData.name.contains("GMOS-S-HAM")) {
        chipGain = harmonicGain(multiportGains);
        individualFixDone = true;
    }
    else if (instData.name.contains("SOI@SOAR")) {
        chipGain = harmonicGain(multiportGains);
        individualFixDone = true;
    }
    else if (instData.name == "LRIS_BLUE@KECK") {
        chipGain = harmonicGain(multiportGains);
        individualFixDone = true;
    }
    else if (instData.name == "LRIS_RED@KECK") {
        chipGain = harmonicGain(multiportGains);
        individualFixDone = true;
    }
    else if (instData.name == "MOSAIC-II_16@CTIO") {
        chipGain = harmonicGain(multiportGains);
        individualFixDone = true;
    }
    else if (instData.name == "PISCO@LCO") {
        chipGain = harmonicGain(multiportGains);
        individualFixDone = true;
    }
    else if (instData.name == "MOSAIC-III_4@KPNO_4m") {
        chipGain = harmonicGain(multiportGains);
        individualFixDone = true;
    }
    else if (instData.name == "SAMI_2x2@SOAR") {
        chipGain = harmonicGain(multiportGains);
        individualFixDone = true;
    }
    else if (instData.name == "FORS1_199904-200703@VLT" || instData.name == "FORS2_200004-200203@VLT") {
        // 1-port read mode or 4-port read mode?
        numReadoutChannels = 0;
        if (!searchKeyValue(QStringList() << "HIERARCH ESO DET OUTPUTS", numReadoutChannels)) {
            emit messageAvailable(baseName + " : Could not determine number of readout channels!", "error");
            emit critical();
            successProcessing = false;
        }
        else {
            if (numReadoutChannels == 4) {
                float gain1 = 0.0;
                float gain2 = 0.0;
                float gain3 = 0.0;
                float gain4 = 0.0;
                searchKeyValue(QStringList() << "HIERARCH ESO DET OUT1 CONAD", gain1);
                searchKeyValue(QStringList() << "HIERARCH ESO DET OUT2 CONAD", gain2);
                searchKeyValue(QStringList() << "HIERARCH ESO DET OUT3 CONAD", gain3);
                searchKeyValue(QStringList() << "HIERARCH ESO DET OUT4 CONAD", gain4);
                channelGains.clear();
                channelGains << gain1 << gain2 << gain3 << gain4;
                chipGain = harmonicGain(channelGains);
                individualFixDone = true;
            }
        }
    }

    if (individualFixDone) {
        QString card1 = "GAINORIG= "+QString::number(chipGain, 'f', 6) + " / Original gain in the raw data for this image";
        QString card2 = "GAIN    = 1.0     / ADUs were converted to e- in this image using GAINORIG";
        card1.resize(80, ' ');
        card2.resize(80, ' ');
        headerTHELI.append(card1);
        headerTHELI.append(card2);

        if (instData.name.contains("GMOS-N-HAM")
                || instData.name.contains("GMOS-S-HAM")
                || instData.name.contains("SAMI")
                || instData.name == "MOSAIC-II_16@CTIO"
                || instData.name == "PISCO@LCO"
                || instData.name == "MOSAIC-III_4@KPNO_4m") {
            gain[chip/numAmpPerChip] = chipGain;
        }
        else {
            gain[chip] = chipGain;        // not applied for e.g. SuprimeCam_200808-201705
        }

        gainForSaturation = chipGain;
    }

    return individualFixDone;
}

// Build the AIRMASS keyword
void Splitter::buildTheliHeaderAIRMASS()
{
    if (!successProcessing) return;

    bool keyFound = searchKey("AIRMASS", headerDictionary.value("AIRMASS"), headerTHELI);
    if (keyFound) return;

    // Fallback: Calculate airmass from RA, DEC, OBSLAT and LST
    bool foundLST = searchKeyLST(headerDictionary.value("LST"));
    if (!foundLST) lstValue = dateobsToLST();

    double airmass = 1.0;
    if (foundLST && lstValue != 58849.0000) airmass = localSiderealTimeToAirmass();  // numeric value indicates that dateobs is unknown
    else airmass = 1.0;
    QString card = "AIRMASS = "+QString::number(airmass, 'f', 4);
    card.resize(80, ' ');
    headerTHELI.append(card);

    if (!foundLST || lstValue != 58849.0000) {
        if (dataType == "SCIENCE"
                || dataType == "SKY"
                || dataType == "STD") {
            emit messageAvailable(fileName + " : Could not determine keyword: AIRMASS, set to 1.0", "warning");
        }
    }
}

void Splitter::buildTheliHeaderFILTER(int chip)
{
    // Exceptions. Return if successful.
    if (individualFixFILTER(chip)) return;

    QStringList filterKeywordList;
    QStringList possibleKeyNames = headerDictionary.value("FILTER");

    QList<QStringList> headers = {primaryHeader, extHeader};

    bool keyFound = false;
    bool clearFound = false;
    bool darkFound = false;
    // Loop over headers
    for (auto &header : headers) {
        for (auto &possibleKey : possibleKeyNames) {
            for (auto &card : header) {
                QString keyName = card.split("=")[0].simplified();
                // Loop over list of possible key names to find match
                if (keyName == possibleKey) {
                    QString filterName = card.split("=")[1];
                    if (filterName.contains("'")) {           // FILTER keyword is a string starting with a single quote
                        filterName = filterName.split("'").at(1);
                    }
                    else {
                        int slashPosition = filterName.lastIndexOf('/');
                        // TODO: the slash might occur further in front! In particular for HIERARCH ESO cards
                        if (slashPosition > 12) filterName.truncate(slashPosition);
                    }
                    filterName = filterName.simplified();
                    // Clean the string
                    filterName.remove("'");
                    filterName.remove("#");
                    filterName.remove("[");
                    filterName.remove("]");
                    filterName.remove("(");
                    filterName.remove(")");
                    filterName.remove("/");
                    filterName.remove(";");
                    filterName.remove("$");
                    filterName.remove(" ");
                    // Skip if filter name suggests that the slot was empty
                    if (filterName.contains("clear", Qt::CaseInsensitive)
                            || filterName.contains("empty", Qt::CaseInsensitive)
                            || filterName.contains("clr", Qt::CaseInsensitive)
                            || filterName.contains("csl", Qt::CaseInsensitive)          // MOIRCS
                            || filterName.contains("hole", Qt::CaseInsensitive)         // MOIRCS
                            || filterName.contains("unavailable", Qt::CaseInsensitive)  // SAMI
                            || filterName.contains("open", Qt::CaseInsensitive)) {
                        clearFound = true;
                        continue;
                    }
                    // Skip if filter name suggests that a dark was taken
                    if (filterName.contains("dark", Qt::CaseInsensitive)
                            || filterName.contains("close", Qt::CaseInsensitive)
                            || filterName.contains("blocked", Qt::CaseInsensitive)) {
                        darkFound = true;
                        continue;
                    }
                    // Seems we found a valid FILTER keyword
                    filterKeywordList.append(filterName);
                    keyFound = true;
                }
            }
        }
    }

    QString filterCard = "";
    if (!keyFound && darkFound) filterCard = "FILTER  = 'Dark'";
    else if (!keyFound && !darkFound && clearFound) filterCard = "FILTER  = 'Clear'";
    else if (!keyFound && !darkFound && !clearFound) {
        if (dataType != "BIAS" && dataType != "DARK") {
            if (*verbosity > 1) emit messageAvailable(fileName + " : Could not determine keyword: FILTER, set to 'Unknown'", "warning");
        }
        filter = "Unknown";
        filterCard = "FILTER  = '"+filter+"'";
    }
    else {
        filterKeywordList.removeDuplicates();
        filter = filterKeywordList.join("+");
        // Replace by short filter name (if mapped)
        QString replacement = filterDictionary.value(filter);
        if (!replacement.isEmpty()) filter = replacement;
        filterCard = "FILTER  = '"+filter+"'";
    }
    filterCard.resize(80, ' ');
    headerTHELI.append(filterCard);
}

bool Splitter::individualFixFILTER(int chip)
{
    if (!successProcessing) return false;

    bool individualFixDone = false;

    QString filterCard = "";

    if (instData.name == "PISCO@LCO") {
        if (chip == 0 || chip == 1)  filter = "g";
        if (chip == 2 || chip == 3) filter = "r";
        if (chip == 4 || chip == 5) filter = "i";
        if (chip == 6 || chip == 7) filter = "z";
        filterCard = "FILTER  = '"+filter+"'";
        individualFixDone = true;
    }

    if (instData.name == "LRIS_BLUE@KECK") {
        searchKeyValue(QStringList() << "BLUFILT", filter);
        filterCard = "FILTER  = '"+filter+"'";
        individualFixDone = true;
    }

    if (instData.name == "LRIS_RED@KECK") {
        searchKeyValue(QStringList() << "REDFILT", filter);
        filterCard = "FILTER  = '"+filter+"'";
        individualFixDone = true;
    }

    if (individualFixDone) {
        filterCard.resize(80, ' ');
        headerTHELI.append(filterCard);
    }

    return individualFixDone;
}

bool Splitter::checkFormatDATEOBS()
{
    // dateobs format: YYYY-MM-DDTHH:MM:SS.sss
    if (!dateObsValue.contains("T")
            || !dateObsValue.contains(":")
            || !dateObsValue.contains("-")) {
        return false;
    }

    QStringList list = dateObsValue.split("T");
    QString date = list[0];
    QString time = list[1];
    QStringList datelist = date.split("-");
    QStringList timelist = time.split(":");
    if (datelist.length() != 3
            || timelist.length() != 3) {
        return false;
    }

    return true;
}

QString Splitter::mjdobsToDATEOBS(double mjd)
{
    // Code taken and adjusted from:
    // https://api.kde.org/4.12-api/kdeedu-apidocs/marble/html/astrolib_8cpp_source.html

    // This program is free software licensed under the GNU LGPL. You can
    // find a copy of this license in LICENSE.txt in the top directory of
    // the source code.
    //
    // Copyright : Gerhard HOLTKAMP          11-JAN-2012

    long jd0 = long(mjd + 2400001.0);
    long b = long ((jd0 - 1867216.25) / 36524.25);     // Gregorian calendar
    long c = jd0 + b - (b/4) + 1525;                   // Gregorian calendar
    long d = long ((c - 122.1) / 365.25);
    long e = 365 * d + (d/4);
    long f = long ((c - e) / 30.6001);
    int day = c - e - long(30.6001 * f);
    int month = f - 1 - 12 * (f / 14);
    int year = d - 4715 - ((7 + month) / 10);
    double hour = 24.0 * (mjd - floor(mjd));

    QString date = QString::number(year)+"-"+QString::number(month)+"-"+QString::number(day)+"T";
    QString time = decimalToDms(hour);

    return date+time;
}

double Splitter::dateobsToMJD()
{
    if (!checkFormatDATEOBS()) {
        emit messageAvailable(fileName + " : Splitter::dateobsToMJD(): Invalid DATE-OBS format:" + dateObsValue
                              + "Setting MJD-OBS to 58849.000000 (2020-01-01).<br>"
                              + "Background modeling and proper motion correction will not work correctly.", "warning");
        emit warning();
        return 58849.000000;
    }

    QStringList list = dateObsValue.split("T");
    QString date = list[0];
    QString time = list[1];
    QStringList datelist = date.split("-");
    QStringList timelist = time.split(":");

    double year = datelist[0].toDouble();
    double month = datelist[1].toDouble();
    double day = datelist[2].toDouble();
    double hh = timelist[0].toDouble();
    double mm = timelist[1].toDouble();
    double ss = timelist[2].toDouble();

    // Explanation: See http://aa.usno.navy.mil/faq/docs/JD_Formula.html
    // UT in decimal hours
    double ut = hh + mm / 60.0 + ss / 3600.0;

    double A = year * 367.0;
    double B = floor((month + 9.0) / 12.0);
    double C = floor(((year + B) * 7.0) / 4.0);
    double D = floor((275.0 * month) / 9.0);
    double E = day + 1721013.5 + (ut / 24.0);
    double F = (((100.0 * year) + month - 190002.5) >= 0) ? 1.0 : -1.0;
    double julian_date = A - C + D + E - (0.5 * F) + 0.5;
    double mjd = julian_date - 2400000.5;

    return mjd;
}

double Splitter::dateobsToLST()
{
    if (!checkFormatDATEOBS()) {
        emit messageAvailable(fileName + " : Splitter::dateobsToMJD(): Invalid DATE-OBS format:" + dateObsValue
                              + "Setting MJD-OBS to 58849.000000 (2020-01-01).<br>"
                              + "Background modeling and proper motion correction will not work correctly.", "warning");
        emit warning();
        return 58849.000000;
    }

    QStringList list = dateObsValue.split("T");
    QString date = list[0];
    QString time = list[1];
    QStringList datelist = date.split("-");
    QStringList timelist = time.split(":");

    double year = datelist[0].toDouble();
    double month = datelist[1].toDouble();
    double day = datelist[2].toDouble();
    double hh = timelist[0].toDouble();
    double mm = timelist[1].toDouble();
    double ss = timelist[2].toDouble();

    // Explanation: See http://www.xylem.f2s.com/kepler/index.html#top
    // UT in decimal hours
    double ut = hh + mm/60. + ss/3600.;

    // The integer and fractional days from J2000
    double dwhole = 367 * year - (int)(7*(year+(int)((month+9)/12))/4) + (int)(275*month/9) + day - 730531.5;
    double dfrac = ut / 24.;
    double d = dwhole + dfrac;

    double lst = 100.46 + 0.985647*d + instData.obslon + 15.*ut;

    // LST must be between 0 and 360 degrees
    int idummy = (int)(lst/360.);
    if(lst > 0) lst -= (float)(idummy*360.);
    else lst -= (float)((idummy-1)*360.);

    // Convert to seconds
    lst = (lst/15.)*3600.;

    return lst;
}

double Splitter::localSiderealTimeToAirmass()
{
    double LSTbegin = lstValue;
    double LSTmiddle = lstValue + 0.5 * exptimeValue;
    double LSTend = lstValue + exptimeValue;
    // double lst = lstValue * RAD;
    double hourangle_begin = (LSTbegin/240. - crval1) * rad;
    double hourangle_middle = (LSTmiddle/240. - crval1) * rad;
    double hourangle_end = (LSTend/240. - crval1) * rad;

    // The effective airmass is estimated using the 'mean airmass' estimator in
    // "Some Factors Affecting the Accuracy of Stellar Photometry with CCDs" (P. Stetson, DAO preprint, September 1988)

    double airmass_begin = calcAirmass(hourangle_begin);
    double airmass_middle = calcAirmass(hourangle_middle);
    double airmass_end = calcAirmass(hourangle_end);
    double airmass = (airmass_begin + 4.0*airmass_middle + airmass_end) / 6.0;

    return airmass;
}

// ahourangle is already in [rad]
double Splitter::calcAirmass(double ahourangle)
{
    double sh = sin(ahourangle);
    double ch = cos(ahourangle);
    double sd = sin(crval2 * rad);
    double cd = cos(crval2 * rad);
    double sp = sin(instData.obslat * rad);
    double cp = cos(instData.obslat * rad);
    double x = ch*cd*sp - sd*cp;
    double y = sh*cd;
    double z = ch*cd*cp + sd*sp;
    double zn = sqrt(x*x+y*y);
    double zf = zn/z;
    double zd = atan(zf);
    double seczm = 1.0 / (cos(std::min(1.52, zd))) - 1.0;

    // Convert zenith distance to airmass following
    // "R.H. Hardie in _Astronomical Techniques_ (W.A. Hiltner, ed.) (Chicago: U. Chicago Press), p. 180 (1962)."
    double airmass = 1.0 + seczm * (0.9981833 - seczm * (0.002875 + seczm * 0.008083));

    return airmass;
}
