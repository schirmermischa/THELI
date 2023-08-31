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

    // Append BUNIT
    QString bunit = "BUNIT   = 'e-'";
    bunit.resize(80, ' ');
    headerTHELI.append(bunit);

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
                               "FLAMINGOS2@GEMINI", "SAMI_2x2@SOAR"          // integer
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
                                  "Some processing tasks (background modeling, proper motion correction, catalog creation) will not work correctly.", "warning");
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

    if (chip >= instData.crpix1.length()) {
        QString nfound = QString::number(chip+1);
        QString nexpected = QString::number(instData.numChips);
        emit messageAvailable(fileName + ": " + nfound + " detectors found, " + nexpected + " expected. Check instrument selection." , "error");
        emit critical();
        successProcessing = false;
        return;
    }

    QStringList headerWCS;

    // Use dedicated lookup
    if (instData.name == "HSC@SUBARU") {
        searchKeyCRVAL("CRPIX1", headerDictionary.value("CRPIX1"), headerWCS);
        searchKeyCRVAL("CRPIX2", headerDictionary.value("CRPIX2"), headerWCS);
    }
//    if (instData.name == "VIS@EUCLID") {
//        searchKeyCRVAL("CRPIX1", headerDictionary.value("CRPIX1"), headerWCS);
//        searchKeyCRVAL("CRPIX2", headerDictionary.value("CRPIX2"), headerWCS);
//    }
    else {
        // CRPIXi: Rely on instrument.ini (Todo: scan .ahead files directly for multi-chip cameras)
        QString crpix1_card = "CRPIX1  = "+QString::number(instData.crpix1[chip]);
        QString crpix2_card = "CRPIX2  = "+QString::number(instData.crpix2[chip]);
        crpix1_card.resize(80, ' ');
        crpix2_card.resize(80, ' ');
        headerWCS.append(crpix1_card);
        headerWCS.append(crpix2_card);
    }
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
/*
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
        // fallback
        double cd = 0.0;
        if (!keyFound && wcsKey == "CD1_1") {
            bool success = CDfromCDELTandPC("CDELT1", "PC1_1", cd);
            if (success) fallback = "CD1_1   = "+QString::number(cd, 'g', 6);
            else fallback = "CD1_1   = "+QString::number(-1.*flipcd11*instData.pixscale/3600., 'g', 6);
        }
        if (!keyFound && wcsKey == "CD2_2") {
            bool success = CDfromCDELTandPC("CDELT2", "PC2_2", cd);
            if (success) fallback = "CD2_2   = "+QString::number(cd, 'g', 6);
            else fallback = "CD2_2   = "+QString::number(flipcd22*instData.pixscale/3600., 'g', 6);
        }
        if (!keyFound && wcsKey == "CD1_2") {
            bool success = CDfromCDELTandPC("CDELT2", "PC1_2", cd);
            if (success) fallback = "CD1_2   = "+QString::number(cd, 'g', 6);
            else fallback = "CD1_2   = 0.0";
        }
        if (!keyFound && wcsKey == "CD2_1") {
            bool success = CDfromCDELTandPC("CDELT1", "PC2_1", cd);
            if (success) fallback = "CD2_1   = "+QString::number(cd, 'g', 6);
            else fallback = "CD2_1   = 0.0";
        }

        if (!keyFound) {
            if (*verbosity > 1) emit messageAvailable(fileName + " : Could not determine keyword: "+wcsKey+", using default: "+fallback, "ignore");
            fallback.resize(80, ' ');
            headerWCS.append(fallback);
        }
    }

    headerTHELI.append(headerWCS);
}
*/

// Instrument dependent
void Splitter::WCSbuildCDmatrix(int chip)
{
    if (!successProcessing) return;

    // Exceptions. Return if successful.
    if (individualFixCDmatrix(chip)) return;

    QStringList headerWCS;
    QString fallback = "";

    QStringList wcsKeys = {"CD1_1", "CD1_2", "CD2_1", "CD2_2"};

    // Create fallback CD matrix from CDELT keywords, if later determination of CDij fails
    QVector<double> cd_fallback;
    cd_fallback = CDfromCDELT();

    // Try reading CDij matrix directly, sue fallback if failing
    for (auto &wcsKey : wcsKeys) {
        bool keyFound = searchKey(wcsKey, headerDictionary.value(wcsKey), headerWCS);

        // fallback
        if (!keyFound && wcsKey == "CD1_1") fallback = "CD1_1   = "+QString::number(cd_fallback[0], 'g', 6);
        if (!keyFound && wcsKey == "CD1_2") fallback = "CD1_2   = "+QString::number(cd_fallback[1], 'g', 6);
        if (!keyFound && wcsKey == "CD2_1") fallback = "CD2_1   = "+QString::number(cd_fallback[2], 'g', 6);
        if (!keyFound && wcsKey == "CD2_2") fallback = "CD2_2   = "+QString::number(cd_fallback[3], 'g', 6);
        if (!keyFound) {
            if (*verbosity > 1) emit messageAvailable(fileName + " : Could not determine keyword: "+wcsKey+", using default: "+fallback, "ignore");
            fallback.resize(80, ' ');
            headerWCS.append(fallback);
        }
    }

    headerTHELI.append(headerWCS);
}

/*
bool Splitter::CDfromCDELTandPC(const QString cdeltstr, const QString pcstr, double &cd)
{
    double cdelt = -1.0;
    double pc = -1.0;
    bool cdtest = searchKeyValue(headerDictionary.value(cdeltstr), cdelt);
    bool pctest = searchKeyValue(headerDictionary.value(pcstr), pc);
    if (cdtest && pctest) {
        cd = cdelt*pc;
        if (fabs(cd) > 0.5) cd *= instData.pixscale/3600.;   // recover plate scale if not available
        return true;
    }
    else if (cdtest && !pctest) {
        cd = cdelt;
        if (fabs(cd) > 0.5) cd *= instData.pixscale/3600.;
        qDebug() << cdeltstr << cd;
        return true;
    }
    else {
        cd = 0.;
        return false;
    }
}
*/

QVector<double> Splitter::CDfromCDELT()
{
    double cdelt1 = 0.;
    double cdelt2 = 0.;
    double pc11 = 1.; // matching crota2 = 0.
    double pc12 = 0.; // matching crota2 = 0.
    double pc21 = 0.; // matching crota2 = 0.
    double pc22 = 1.; // matching crota2 = 0.
    double crota2 = 0.;

    bool cdelt1test = searchKeyValue(headerDictionary.value("CDELT1"), cdelt1);
    bool cdelt2test = searchKeyValue(headerDictionary.value("CDELT2"), cdelt2);
    bool crota2test = searchKeyValue(headerDictionary.value("CROTA2"), crota2);
    bool pc11test = searchKeyValue(headerDictionary.value("PC1_1"), pc11);
    bool pc12test = searchKeyValue(headerDictionary.value("PC1_2"), pc12);
    bool pc21test = searchKeyValue(headerDictionary.value("PC2_1"), pc21);
    bool pc22test = searchKeyValue(headerDictionary.value("PC2_2"), pc22);
    bool pctest = true;
    pctest = pc11test & pc12test & pc21test & pc22test;
    if (!pctest) {
        // check for really old convention
        pc11test = searchKeyValue(headerDictionary.value("PC001001"), pc11);
        pc12test = searchKeyValue(headerDictionary.value("PC001002"), pc12);
        pc21test = searchKeyValue(headerDictionary.value("PC002001"), pc21);
        pc22test = searchKeyValue(headerDictionary.value("PC002002"), pc22);
        pctest = pc11test & pc12test & pc21test & pc22test;
    }

    // Only if no CD matrix information is present in the raw FITS headers:
    float flipcd11 = 1.0;
    float flipcd22 = 1.0;
    if (instData.flip == "FLIPX") flipcd11 = -1.0;
    else if (instData.flip == "FLIPY") flipcd22 = -1.0;
    else if (instData.flip == "ROT180") {
        flipcd11 = -1.0;
        flipcd22 = -1.0;
    }

    double cd11 = 0.;
    double cd12 = 0.;
    double cd21 = 0.;
    double cd22 = 0.;

    // Now build the CD matrix
    QVector<double> cd;

    // No information whatsoever: return standard "North up East left"
    if (!cdelt1test || !cdelt2test) {
        cd11 = -flipcd11*instData.pixscale/3600.;
        cd22 = flipcd22*instData.pixscale/3600.;
        cd << cd11 << cd12 << cd21 << cd22;
        return cd;
    }

    // CDELT yes, but no information about sky position angle
    if (!pctest && !crota2test) {
        cd11 = cdelt1;
        cd22 = cdelt2;
        cd << cd11 << cd12 << cd21 << cd22;
        return cd;
    }

    // CROTA2 but no PC matrix
    else if (!pctest && crota2test) {
        pc11 = cos(crota2*rad);
        pc12 = -sin(crota2*rad);
        pc21 = sin(crota2*rad);
        pc22 = cos(crota2*rad);
    }
    // PC matrix and CROTA2
    // According to the FITS standard, this combination is not allowed, but still some headers have it
    else if (pctest && crota2test) {
        pc11 = cos(crota2*rad);
        pc12 = -sin(crota2*rad);
        pc21 = sin(crota2*rad);
        pc22 = cos(crota2*rad);
        emit messageAvailable(name + " : PC matrix and CROTA2 found. Using CROTA2 to compute CD matrix.", "warning");
    }
    else {
        // (pctest && !crota2test) {
        // already handled above by determining the PCij elements directly from FITS header
    }

    // Compute the CD matrix
    cd11 = cdelt1 * pc11;
    cd12 = cdelt2 * pc12;
    cd21 = cdelt1 * pc21;
    cd22 = cdelt2 * pc22;

    // safety check
    if (fabs(cd11) > 0.5
            || fabs(cd11) > 0.5
            || fabs(cd21) > 0.5
            || fabs(cd22) > 0.5) {
        cd11 = -flipcd11*instData.pixscale/3600.;
        cd22 = flipcd22*instData.pixscale/3600.;
        cd12 = 0.;
        cd21 = 0.;
    }

    cd << cd11 << cd12 << cd21 << cd22;
    return cd;
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

    if (instData.name == "VIS@EUCLID") {
        equinoxCard = equinoxCard = "EQUINOX = 2000.0";
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
    QStringList list = {"WFC@INT", "WFC_2x2@INT", "SUSI1@NTT", "PISCO@LCO"};

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
    if (instData.name == "WFC@INT" || instData.name == "WFC_2x2@INT") {
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
    if (instData.name == "PISCO@LCO") {
        searchKeyValue(QStringList() << "TELRA", crval1);    // CRVALij in FITS extensions are highly inaccurate
        searchKeyValue(QStringList() << "TELDC", crval2);
        if (crval1.contains(":")) crval1 = hmsToDecimal(crval1);
        if (crval2.contains(":")) crval2 = dmsToDecimal(crval2);
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

    if (instData.name == "GMOS-S-HAM_4x4@GEMINI") {
        if (chip == 3) crpix1_card = "CRPIX1  = 794";
        if (chip == 7) crpix1_card = "CRPIX1  = 272";
        if (chip == 11) crpix1_card = "CRPIX1  = -251";
        crpix2_card = "CRPIX2  = 576";
        individualFixDone = true;
    }

    if (instData.name == "SOI@SOAR") {
        if (chip == 1) crpix1_card = "CRPIX1  = 1051";
        if (chip == 3) crpix1_card = "CRPIX1  = -26";
        crpix2_card = "CRPIX2  = 1024";
        individualFixDone = true;
    }

    if (instData.name == "DEIMOS_2AMP@KECK") {
        if (chip == 1) crpix1_card = "CRPIX1  = 4253";
        if (chip == 3) crpix1_card = "CRPIX1  = 2097";
        if (chip == 5) crpix1_card = "CRPIX1  = -53";
        if (chip == 7) crpix1_card = "CRPIX1  = -2200";
        crpix2_card = "CRPIX2  = 1301";
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

    double cd11 = 0.;
    double cd12 = 0.;
    double cd21 = 0.;
    double cd22 = 0.;

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
    if (instData.name == "DEIMOS_1AMP@KECK" || instData.name == "DEIMOS_2AMP@KECK") {
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
    if (instData.name == "WFC_2x2@INT") {
        if (chip == 0) {
            cd11_card = "CD1_1   =  -2.187595402632E-06";
            cd12_card = "CD1_2   =  -1.836774933953E-04";
            cd21_card = "CD2_1   =  -1.836774933953E-04";
            cd22_card = "CD2_2   =   2.196693471198E-06";
        }
        if (chip == 1) {
            cd11_card = "CD1_1   =   1.838979962523E-04";
            cd12_card = "CD1_2   =  -1.530619041314E-08";
            cd21_card = "CD2_1   =  -7.465373169364E-08";
            cd22_card = "CD2_2   =  -1.837749990795E-04";
        }
        if (chip == 2) {
            cd11_card = "CD1_1   =  -2.526051008644E-06";
            cd12_card = "CD1_2   =  -1.839716072858E-04";
            cd21_card = "CD2_1   =  -1.839315570910E-04";
            cd22_card = "CD2_2   =   2.415393914242E-06";
        }
        if (chip == 3) {
            cd11_card = "CD1_1   =  -2.049772957061E-06";
            cd12_card = "CD1_2   =  -1.847202572582E-04";
            cd21_card = "CD2_1   =  -1.847377035556E-04";
            cd22_card = "CD2_2   =   2.053889958265E-06";
        }
        individualFixDone = true;
    }
    if (instData.name == "VIS@EUCLID") {
        if (chip == 0) {
            cd11_card = "CD1_1   =   1.485336085825E-05";
            cd12_card = "CD1_2   =   2.373020263256E-05";
            cd21_card = "CD2_1   =   2.362023502688E-05";
            cd22_card = "CD2_2   =  -1.476723307336E-05";
        }
        if (chip == 1) {
            cd11_card = "CD1_1   =   1.484838705414E-05";
            cd12_card = "CD1_2   =   2.371946211532E-05";
            cd21_card = "CD2_1   =   2.359023016259E-05";
            cd22_card = "CD2_2   =  -1.475883937263E-05";
        }
        if (chip == 2) {
            cd11_card = "CD1_1   =   1.484493267117E-05";
            cd12_card = "CD1_2   =   2.372961666401E-05";
            cd21_card = "CD2_1   =   2.360527033961E-05";
            cd22_card = "CD2_2   =  -1.478049990387E-05";
        }
        if (chip == 3) {
            cd11_card = "CD1_1   =   1.485549671799E-05";
            cd12_card = "CD1_2   =   2.373830735312E-05";
            cd21_card = "CD2_1   =   2.363212771872E-05";
            cd22_card = "CD2_2   =  -1.479105693637E-05";
        }
        if (chip == 4) {
            cd11_card = "CD1_1   =   1.482322577805E-05";
            cd12_card = "CD1_2   =   2.371507215907E-05";
            cd21_card = "CD2_1   =   2.355093676088E-05";
            cd22_card = "CD2_2   =  -1.474000475642E-05";
        }
        if (chip == 5) {
            cd11_card = "CD1_1   =   1.481650650650E-05";
            cd12_card = "CD1_2   =   2.370735654076E-05";
            cd21_card = "CD2_1   =   2.351796332508E-05";
            cd22_card = "CD2_2   =  -1.473037411308E-05";
        }
        if (chip == 6) {
            cd11_card = "CD1_1   =   1.480737929504E-05";
            cd12_card = "CD1_2   =   2.371652791694E-05";
            cd21_card = "CD2_1   =   2.353124251720E-05";
            cd22_card = "CD2_2   =  -1.474913230516E-05";
        }
        if (chip == 7) {
            cd11_card = "CD1_1   =   1.482929756570E-05";
            cd12_card = "CD1_2   =   2.372790077018E-05";
            cd21_card = "CD2_1   =   2.356524319612E-05";
            cd22_card = "CD2_2   =  -1.476447491535E-05";
        }
        if (chip == 8) {
            cd11_card = "CD1_1   =   1.479396480012E-05";
            cd12_card = "CD1_2   =   2.370220372208E-05";
            cd21_card = "CD2_1   =   2.347900007563E-05";
            cd22_card = "CD2_2   =  -1.470872804752E-05";
        }
        if (chip == 9) {
            cd11_card = "CD1_1   =   1.477691388604E-05";
            cd12_card = "CD1_2   =   2.369248796008E-05";
            cd21_card = "CD2_1   =   2.343738090092E-05";
            cd22_card = "CD2_2   =  -1.469482038591E-05";
        }
        if (chip == 10) {
            cd11_card = "CD1_1   =   1.476551327445E-05";
            cd12_card = "CD1_2   =   2.370156642127E-05";
            cd21_card = "CD2_1   =   2.345080483108E-05";
            cd22_card = "CD2_2   =  -1.471937620807E-05";
        }
        if (chip == 11) {
            cd11_card = "CD1_1   =   1.478379590078E-05";
            cd12_card = "CD1_2   =   2.371207468444E-05";
            cd21_card = "CD2_1   =   2.348982580685E-05";
            cd22_card = "CD2_2   =  -1.473464502900E-05";
        }
        if (chip == 12) {
            cd11_card = "CD1_1   =   1.487203121520E-05";
            cd12_card = "CD1_2   =   2.373002440295E-05";
            cd21_card = "CD2_1   =   2.363536510299E-05";
            cd22_card = "CD2_2   =  -1.483722108352E-05";
        }
        if (chip == 13) {
            cd11_card = "CD1_1   =   1.485657138737E-05";
            cd12_card = "CD1_2   =   2.372771830310E-05";
            cd21_card = "CD2_1   =   2.359808207965E-05";
            cd22_card = "CD2_2   =  -1.482579682551E-05";
        }
        if (chip == 14) {
            cd11_card = "CD1_1   =   1.484828058745E-05";
            cd12_card = "CD1_2   =   2.373047734707E-05";
            cd21_card = "CD2_1   =   2.360957766250E-05";
            cd22_card = "CD2_2   =  -1.484199198313E-05";
        }
        if (chip == 15) {
            cd11_card = "CD1_1   =   1.486613575122E-05";
            cd12_card = "CD1_2   =   2.373662212645E-05";
            cd21_card = "CD2_1   =   2.363997591790E-05";
            cd22_card = "CD2_2   =  -1.485285912782E-05";
        }
        if (chip == 16) {
            cd11_card = "CD1_1   =   1.482235929435E-05";
            cd12_card = "CD1_2   =   2.373403750698E-05";
            cd21_card = "CD2_1   =   2.357839940013E-05";
            cd22_card = "CD2_2   =  -1.479552989005E-05";
        }
        if (chip == 17) {
            cd11_card = "CD1_1   =   1.480298048702E-05";
            cd12_card = "CD1_2   =   2.372386460655E-05";
            cd21_card = "CD2_1   =   2.354054106341E-05";
            cd22_card = "CD2_2   =  -1.478343953883E-05";
        }
        if (chip == 18) {
            cd11_card = "CD1_1   =   1.479332688673E-05";
            cd12_card = "CD1_2   =   2.372739845340E-05";
            cd21_card = "CD2_1   =   2.355246989508E-05";
            cd22_card = "CD2_2   =  -1.480296647578E-05";
        }
        if (chip == 19) {
            cd11_card = "CD1_1   =   1.481176557493E-05";
            cd12_card = "CD1_2   =   2.373576401162E-05";
            cd21_card = "CD2_1   =   2.358932226099E-05";
            cd22_card = "CD2_2   =  -1.481249922856E-05";
        }
        if (chip == 20) {
            cd11_card = "CD1_1   =   1.476899848422E-05";
            cd12_card = "CD1_2   =   2.372049602314E-05";
            cd21_card = "CD2_1   =   2.350902412408E-05";
            cd22_card = "CD2_2   =  -1.475631705174E-05";
        }
        if (chip == 21) {
            cd11_card = "CD1_1   =   1.474866589659E-05";
            cd12_card = "CD1_2   =   2.371075877642E-05";
            cd21_card = "CD2_1   =   2.347150980654E-05";
            cd22_card = "CD2_2   =  -1.474852920014E-05";
        }
        if (chip == 22) {
            cd11_card = "CD1_1   =   1.474244947610E-05";
            cd12_card = "CD1_2   =   2.371605247177E-05";
            cd21_card = "CD2_1   =   2.348366279118E-05";
            cd22_card = "CD2_2   =  -1.476632120557E-05";
        }
        if (chip == 23) {
            cd11_card = "CD1_1   =   1.476081058159E-05";
            cd12_card = "CD1_2   =   2.372464651027E-05";
            cd21_card = "CD2_1   =   2.352366201702E-05";
            cd22_card = "CD2_2   =  -1.477616621816E-05";
        }
        if (chip == 24) {
            cd11_card = "CD1_1   =   1.482490632217E-05";
            cd12_card = "CD1_2   =   2.376345948269E-05";
            cd21_card = "CD2_1   =   2.367364145233E-05";
            cd22_card = "CD2_2   =  -1.483692107467E-05";
        }
        if (chip == 25) {
            cd11_card = "CD1_1   =   1.480842187087E-05";
            cd12_card = "CD1_2   =   2.374972514562E-05";
            cd21_card = "CD2_1   =   2.364037948810E-05";
            cd22_card = "CD2_2   =  -1.482872011652E-05";
        }
        if (chip == 26) {
            cd11_card = "CD1_1   =   1.479868376454E-05";
            cd12_card = "CD1_2   =   2.375868783686E-05";
            cd21_card = "CD2_1   =   2.365930970970E-05";
            cd22_card = "CD2_2   =  -1.484174797878E-05";
        }
        if (chip == 27) {
            cd11_card = "CD1_1   =   1.481338629988E-05";
            cd12_card = "CD1_2   =   2.375896252243E-05";
            cd21_card = "CD2_1   =   2.367629258239E-05";
            cd22_card = "CD2_2   =  -1.484820604855E-05";
        }
        if (chip == 28) {
            cd11_card = "CD1_1   =   1.479771156173E-05";
            cd12_card = "CD1_2   =   2.373660473100E-05";
            cd21_card = "CD2_1   =   2.360461508886E-05";
            cd22_card = "CD2_2   =  -1.482336515784E-05";
        }
        if (chip == 29) {
            cd11_card = "CD1_1   =   1.477160669198E-05";
            cd12_card = "CD1_2   =   2.372752267865E-05";
            cd21_card = "CD2_1   =   2.356955675634E-05";
            cd22_card = "CD2_2   =  -1.482064997568E-05";
        }
        if (chip == 30) {
            cd11_card = "CD1_1   =   1.476300152064E-05";
            cd12_card = "CD1_2   =   2.372729560742E-05";
            cd21_card = "CD2_1   =   2.358270538891E-05";
            cd22_card = "CD2_2   =  -1.483080761587E-05";
        }
        if (chip == 31) {
            cd11_card = "CD1_1   =   1.478245341966E-05";
            cd12_card = "CD1_2   =   2.373768659001E-05";
            cd21_card = "CD2_1   =   2.360701711570E-05";
            cd22_card = "CD2_2   =  -1.483381351690E-05";
        }
        if (chip == 32) {
            cd11_card = "CD1_1   =   1.476071214974E-05";
            cd12_card = "CD1_2   =   2.371434903027E-05";
            cd21_card = "CD2_1   =   2.352579693693E-05";
            cd22_card = "CD2_2   =  -1.481092847704E-05";
        }
        if (chip == 33) {
            cd11_card = "CD1_1   =   1.473804741719E-05";
            cd12_card = "CD1_2   =   2.370013163467E-05";
            cd21_card = "CD2_1   =   2.348240115738E-05";
            cd22_card = "CD2_2   =  -1.480444602931E-05";
        }
        if (chip == 34) {
            cd11_card = "CD1_1   =   1.472321857718E-05";
            cd12_card = "CD1_2   =   2.369680176884E-05";
            cd21_card = "CD2_1   =   2.349400065458E-05";
            cd22_card = "CD2_2   =  -1.482086342218E-05";
        }
        if (chip == 35) {
            cd11_card = "CD1_1   =   1.474940269291E-05";
            cd12_card = "CD1_2   =   2.370719980710E-05";
            cd21_card = "CD2_1   =   2.353372033718E-05";
            cd22_card = "CD2_2   =  -1.483043777321E-05";
        }
        if (chip == 36) {
            cd11_card = "CD1_1   =   1.483266562799E-05";
            cd12_card = "CD1_2   =   2.373474981797E-05";
            cd21_card = "CD2_1   =   2.366690010902E-05";
            cd22_card = "CD2_2   =  -1.488938895944E-05";
        }
        if (chip == 37) {
            cd11_card = "CD1_1   =   1.481706063147E-05";
            cd12_card = "CD1_2   =   2.372187197666E-05";
            cd21_card = "CD2_1   =   2.363968637556E-05";
            cd22_card = "CD2_2   =  -1.488787750055E-05";
        }
        if (chip == 38) {
            cd11_card = "CD1_1   =   1.480442864514E-05";
            cd12_card = "CD1_2   =   2.371637909172E-05";
            cd21_card = "CD2_1   =   2.364701924466E-05";
            cd22_card = "CD2_2   =  -1.489374132529E-05";
        }
        if (chip == 39) {
            cd11_card = "CD1_1   =   1.482328765591E-05";
            cd12_card = "CD1_2   =   2.372643224791E-05";
            cd21_card = "CD2_1   =   2.366930385093E-05";
            cd22_card = "CD2_2   =  -1.490091264662E-05";
        }
        if (chip == 40) {
            cd11_card = "CD1_1   =   1.478409043333E-05";
            cd12_card = "CD1_2   =   2.371827610991E-05";
            cd21_card = "CD2_1   =   2.360276399711E-05";
            cd22_card = "CD2_2   =  -1.488004579365E-05";
        }
        if (chip == 41) {
            cd11_card = "CD1_1   =   1.476844233775E-05";
            cd12_card = "CD1_2   =   2.370633000480E-05";
            cd21_card = "CD2_1   =   2.357446147361E-05";
            cd22_card = "CD2_2   =  -1.486253816396E-05";
        }
        if (chip == 42) {
            cd11_card = "CD1_1   =   1.475583824840E-05";
            cd12_card = "CD1_2   =   2.369774429479E-05";
            cd21_card = "CD2_1   =   2.358530136273E-05";
            cd22_card = "CD2_2   =  -1.487411315137E-05";
        }
        if (chip == 43) {
            cd11_card = "CD1_1   =   1.477268993505E-05";
            cd12_card = "CD1_2   =   2.370487618427E-05";
            cd21_card = "CD2_1   =   2.361235945727E-05";
            cd22_card = "CD2_2   =  -1.488510059522E-05";
        }
        if (chip == 44) {
            cd11_card = "CD1_1   =   1.473979955025E-05";
            cd12_card = "CD1_2   =   2.369486167104E-05";
            cd21_card = "CD2_1   =   2.354838495889E-05";
            cd22_card = "CD2_2   =  -1.485818169386E-05";
        }
        if (chip == 45) {
            cd11_card = "CD1_1   =   1.471233560797E-05";
            cd12_card = "CD1_2   =   2.368244946054E-05";
            cd21_card = "CD2_1   =   2.350467678519E-05";
            cd22_card = "CD2_2   =  -1.484601375457E-05";
        }
        if (chip == 46) {
            cd11_card = "CD1_1   =   1.469545025106E-05";
            cd12_card = "CD1_2   =   2.366460966794E-05";
            cd21_card = "CD2_1   =   2.351267625062E-05";
            cd22_card = "CD2_2   =  -1.485458627996E-05";
        }
        if (chip == 47) {
            cd11_card = "CD1_1   =   1.472638787796E-05";
            cd12_card = "CD1_2   =   2.368447426808E-05";
            cd21_card = "CD2_1   =   2.355104392732E-05";
            cd22_card = "CD2_2   =  -1.486633411282E-05";
        }
        if (chip == 48) {
            cd11_card = "CD1_1   =   1.480265297552E-05";
            cd12_card = "CD1_2   =   2.371613147278E-05";
            cd21_card = "CD2_1   =   2.368134767313E-05";
            cd22_card = "CD2_2   =  -1.489496866550E-05";
        }
        if (chip == 49) {
            cd11_card = "CD1_1   =   1.478155552655E-05";
            cd12_card = "CD1_2   =   2.370347313527E-05";
            cd21_card = "CD2_1   =   2.365697115492E-05";
            cd22_card = "CD2_2   =  -1.489785441643E-05";
        }
        if (chip == 50) {
            cd11_card = "CD1_1   =   1.476994240617E-05";
            cd12_card = "CD1_2   =   2.369086672177E-05";
            cd21_card = "CD2_1   =   2.365742117490E-05";
            cd22_card = "CD2_2   =  -1.489378997731E-05";
        }
        if (chip == 51) {
            cd11_card = "CD1_1   =   1.478492533105E-05";
            cd12_card = "CD1_2   =   2.369055900304E-05";
            cd21_card = "CD2_1   =   2.368652501783E-05";
            cd22_card = "CD2_2   =  -1.489526501757E-05";
        }
        if (chip == 52) {
            cd11_card = "CD1_1   =   1.472595270652E-05";
            cd12_card = "CD1_2   =   2.370078550371E-05";
            cd21_card = "CD2_1   =   2.363383299694E-05";
            cd22_card = "CD2_2   =  -1.487286301875E-05";
        }
        if (chip == 53) {
            cd11_card = "CD1_1   =   1.471547802237E-05";
            cd12_card = "CD1_2   =   2.368522505315E-05";
            cd21_card = "CD2_1   =   2.360387905790E-05";
            cd22_card = "CD2_2   =  -1.486062510217E-05";
        }
        if (chip == 54) {
            cd11_card = "CD1_1   =   1.470190584653E-05";
            cd12_card = "CD1_2   =   2.367817302493E-05";
            cd21_card = "CD2_1   =   2.361064765619E-05";
            cd22_card = "CD2_2   =  -1.486269179826E-05";
        }
        if (chip == 55) {
            cd11_card = "CD1_1   =   1.472319671133E-05";
            cd12_card = "CD1_2   =   2.368552191383E-05";
            cd21_card = "CD2_1   =   2.363984180456E-05";
            cd22_card = "CD2_2   =  -1.487300688192E-05";
        }
        if (chip == 56) {
            cd11_card = "CD1_1   =   1.471419851256E-05";
            cd12_card = "CD1_2   =   2.366675182697E-05";
            cd21_card = "CD2_1   =   2.355729554680E-05";
            cd22_card = "CD2_2   =  -1.487664514811E-05";
        }
        if (chip == 57) {
            cd11_card = "CD1_1   =   1.467365082641E-05";
            cd12_card = "CD1_2   =   2.364986175970E-05";
            cd21_card = "CD2_1   =   2.351806476181E-05";
            cd22_card = "CD2_2   =  -1.486822153269E-05";
        }
        if (chip == 58) {
            cd11_card = "CD1_1   =   1.465859549935E-05";
            cd12_card = "CD1_2   =   2.363040265601E-05";
            cd21_card = "CD2_1   =   2.352625206882E-05";
            cd22_card = "CD2_2   =  -1.487553301948E-05";
        }
        if (chip == 59) {
            cd11_card = "CD1_1   =   1.468979257510E-05";
            cd12_card = "CD1_2   =   2.365012028980E-05";
            cd21_card = "CD2_1   =   2.355435253574E-05";
            cd22_card = "CD2_2   =  -1.487992213247E-05";
        }
        if (chip == 60) {
            cd11_card = "CD1_1   =   1.477385612500E-05";
            cd12_card = "CD1_2   =   2.367300762014E-05";
            cd21_card = "CD2_1   =   2.368195293828E-05";
            cd22_card = "CD2_2   =  -1.489190111272E-05";
        }
        if (chip == 61) {
            cd11_card = "CD1_1   =   1.474612980621E-05";
            cd12_card = "CD1_2   =   2.366677021901E-05";
            cd21_card = "CD2_1   =   2.365736450435E-05";
            cd22_card = "CD2_2   =  -1.489060092810E-05";
        }
        if (chip == 62) {
            cd11_card = "CD1_1   =   1.473964150124E-05";
            cd12_card = "CD1_2   =   2.364014974408E-05";
            cd21_card = "CD2_1   =   2.366313413875E-05";
            cd22_card = "CD2_2   =  -1.489646713107E-05";
        }
        if (chip == 63) {
            cd11_card = "CD1_1   =   1.476122894456E-05";
            cd12_card = "CD1_2   =   2.365815860275E-05";
            cd21_card = "CD2_1   =   2.369404662738E-05";
            cd22_card = "CD2_2   =  -1.489493372553E-05";
        }
        if (chip == 64) {
            cd11_card = "CD1_1   =   1.472869858669E-05";
            cd12_card = "CD1_2   =   2.365251305151E-05";
            cd21_card = "CD2_1   =   2.363363556931E-05";
            cd22_card = "CD2_2   =  -1.489349814040E-05";
        }
        if (chip == 65) {
            cd11_card = "CD1_1   =   1.469835417334E-05";
            cd12_card = "CD1_2   =   2.363529590844E-05";
            cd21_card = "CD2_1   =   2.360275139313E-05";
            cd22_card = "CD2_2   =  -1.488993845526E-05";
        }
        if (chip == 66) {
            cd11_card = "CD1_1   =   1.468254065174E-05";
            cd12_card = "CD1_2   =   2.361133010803E-05";
            cd21_card = "CD2_1   =   2.360460257948E-05";
            cd22_card = "CD2_2   =  -1.488945433594E-05";
        }
        if (chip == 67) {
            cd11_card = "CD1_1   =   1.470394028686E-05";
            cd12_card = "CD1_2   =   2.363340760562E-05";
            cd21_card = "CD2_1   =   2.363343400795E-05";
            cd22_card = "CD2_2   =  -1.488666545689E-05";
        }
        if (chip == 68) {
            cd11_card = "CD1_1   =   1.466161048713E-05";
            cd12_card = "CD1_2   =   2.362368550307E-05";
            cd21_card = "CD2_1   =   2.357517911142E-05";
            cd22_card = "CD2_2   =  -1.488370966528E-05";
        }
        if (chip == 69) {
            cd11_card = "CD1_1   =   1.462759963111E-05";
            cd12_card = "CD1_2   =   2.360365943094E-05";
            cd21_card = "CD2_1   =   2.353580424207E-05";
            cd22_card = "CD2_2   =  -1.488123767256E-05";
        }
        if (chip == 70) {
            cd11_card = "CD1_1   =   1.462216984283E-05";
            cd12_card = "CD1_2   =   2.357819440271E-05";
            cd21_card = "CD2_1   =   2.354540087739E-05";
            cd22_card = "CD2_2   =  -1.488328900453E-05";
        }
        if (chip == 71) {
            cd11_card = "CD1_1   =   1.463710540116E-05";
            cd12_card = "CD1_2   =   2.358986242328E-05";
            cd21_card = "CD2_1   =   2.358209539028E-05";
            cd22_card = "CD2_2   =  -1.487236335446E-05";
        }
        if (chip == 72) {
            cd11_card = "CD1_1   =   1.460167143932E-05";
            cd12_card = "CD1_2   =   2.362113331512E-05";
            cd21_card = "CD2_1   =   2.313226618970E-05";
            cd22_card = "CD2_2   =  -1.461953920748E-05";
        }
        if (chip == 73) {
            cd11_card = "CD1_1   =   1.463049691728E-05";
            cd12_card = "CD1_2   =   2.363587710052E-05";
            cd21_card = "CD2_1   =   2.319256805457E-05";
            cd22_card = "CD2_2   =  -1.463738521476E-05";
        }
        if (chip == 74) {
            cd11_card = "CD1_1   =   1.464835579722E-05";
            cd12_card = "CD1_2   =   2.362485221778E-05";
            cd21_card = "CD2_1   =   2.316946563998E-05";
            cd22_card = "CD2_2   =  -1.461103509427E-05";
        }
        if (chip == 75) {
            cd11_card = "CD1_1   =   1.462160818792E-05";
            cd12_card = "CD1_2   =   2.362044454073E-05";
            cd21_card = "CD2_1   =   2.312230016690E-05";
            cd22_card = "CD2_2   =  -1.457968969807E-05";
        }
        if (chip == 76) {
            cd11_card = "CD1_1   =   1.467158766951E-05";
            cd12_card = "CD1_2   =   2.364793975825E-05";
            cd21_card = "CD2_1   =   2.324792846003E-05";
            cd22_card = "CD2_2   =  -1.465793571435E-05";
        }
        if (chip == 77) {
            cd11_card = "CD1_1   =   1.469756788593E-05";
            cd12_card = "CD1_2   =   2.366201864040E-05";
            cd21_card = "CD2_1   =   2.329843908245E-05";
            cd22_card = "CD2_2   =  -1.467202245220E-05";
        }
        if (chip == 78) {
            cd11_card = "CD1_1   =   1.471167380870E-05";
            cd12_card = "CD1_2   =   2.365627525284E-05";
            cd21_card = "CD2_1   =   2.328363706462E-05";
            cd22_card = "CD2_2   =  -1.465166335715E-05";
        }
        if (chip == 79) {
            cd11_card = "CD1_1   =   1.468569911282E-05";
            cd12_card = "CD1_2   =   2.364459595314E-05";
            cd21_card = "CD2_1   =   2.323250396221E-05";
            cd22_card = "CD2_2   =  -1.462883644723E-05";
        }
        if (chip == 80) {
            cd11_card = "CD1_1   =   1.472197483927E-05";
            cd12_card = "CD1_2   =   2.367811618709E-05";
            cd21_card = "CD2_1   =   2.335250814351E-05";
            cd22_card = "CD2_2   =  -1.469135278831E-05";
        }
        if (chip == 81) {
            cd11_card = "CD1_1   =   1.475627418500E-05";
            cd12_card = "CD1_2   =   2.368523756432E-05";
            cd21_card = "CD2_1   =   2.340381802311E-05";
            cd22_card = "CD2_2   =  -1.471132856216E-05";
        }
        if (chip == 82) {
            cd11_card = "CD1_1   =   1.476470901503E-05";
            cd12_card = "CD1_2   =   2.367849772818E-05";
            cd21_card = "CD2_1   =   2.338060017038E-05";
            cd22_card = "CD2_2   =  -1.468103334798E-05";
        }
        if (chip == 83) {
            cd11_card = "CD1_1   =   1.473647663282E-05";
            cd12_card = "CD1_2   =   2.367022179581E-05";
            cd21_card = "CD2_1   =   2.333680383602E-05";
            cd22_card = "CD2_2   =  -1.466669276512E-05";
        }
        if (chip == 84) {
            cd11_card = "CD1_1   =   1.459583891284E-05";
            cd12_card = "CD1_2   =   2.360468676475E-05";
            cd21_card = "CD2_1   =   2.315136210467E-05";
            cd22_card = "CD2_2   =  -1.471182524167E-05";
        }
        if (chip == 85) {
            cd11_card = "CD1_1   =   1.462802881900E-05";
            cd12_card = "CD1_2   =   2.361573941147E-05";
            cd21_card = "CD2_1   =   2.320216679244E-05";
            cd22_card = "CD2_2   =  -1.473120251213E-05";
        }
        if (chip == 86) {
            cd11_card = "CD1_1   =   1.464145002837E-05";
            cd12_card = "CD1_2   =   2.362334275379E-05";
            cd21_card = "CD2_1   =   2.319131313402E-05";
            cd22_card = "CD2_2   =  -1.469912460557E-05";
        }
        if (chip == 87) {
            cd11_card = "CD1_1   =   1.461209059743E-05";
            cd12_card = "CD1_2   =   2.360317586365E-05";
            cd21_card = "CD2_1   =   2.313445824869E-05";
            cd22_card = "CD2_2   =  -1.468736955297E-05";
        }
        if (chip == 88) {
            cd11_card = "CD1_1   =   1.463437481455E-05";
            cd12_card = "CD1_2   =   2.364952986949E-05";
            cd21_card = "CD2_1   =   2.327685788792E-05";
            cd22_card = "CD2_2   =  -1.471731941297E-05";
        }
        if (chip == 89) {
            cd11_card = "CD1_1   =   1.466768996471E-05";
            cd12_card = "CD1_2   =   2.366421464210E-05";
            cd21_card = "CD2_1   =   2.333461458667E-05";
            cd22_card = "CD2_2   =  -1.473193340030E-05";
        }
        if (chip == 90) {
            cd11_card = "CD1_1   =   1.468231121721E-05";
            cd12_card = "CD1_2   =   2.366551503785E-05";
            cd21_card = "CD2_1   =   2.332012688928E-05";
            cd22_card = "CD2_2   =  -1.470382462918E-05";
        }
        if (chip == 91) {
            cd11_card = "CD1_1   =   1.465185282869E-05";
            cd12_card = "CD1_2   =   2.365754379977E-05";
            cd21_card = "CD2_1   =   2.327205981300E-05";
            cd22_card = "CD2_2   =  -1.469446074606E-05";
        }
        if (chip == 92) {
            cd11_card = "CD1_1   =   1.469171647638E-05";
            cd12_card = "CD1_2   =   2.367871061499E-05";
            cd21_card = "CD2_1   =   2.337904156740E-05";
            cd22_card = "CD2_2   =  -1.474686984779E-05";
        }
        if (chip == 93) {
            cd11_card = "CD1_1   =   1.472119201091E-05";
            cd12_card = "CD1_2   =   2.369107932912E-05";
            cd21_card = "CD2_1   =   2.343173663283E-05";
            cd22_card = "CD2_2   =  -1.476324598712E-05";
        }
        if (chip == 94) {
            cd11_card = "CD1_1   =   1.473300460551E-05";
            cd12_card = "CD1_2   =   2.369390254272E-05";
            cd21_card = "CD2_1   =   2.341481631303E-05";
            cd22_card = "CD2_2   =  -1.473265062469E-05";
        }
        if (chip == 95) {
            cd11_card = "CD1_1   =   1.470735143725E-05";
            cd12_card = "CD1_2   =   2.368044608439E-05";
            cd21_card = "CD2_1   =   2.336644349089E-05";
            cd22_card = "CD2_2   =  -1.472299248630E-05";
        }
        if (chip == 96) {
            cd11_card = "CD1_1   =   1.453006346410E-05";
            cd12_card = "CD1_2   =   2.359887760184E-05";
            cd21_card = "CD2_1   =   2.319625302791E-05";
            cd22_card = "CD2_2   =  -1.474321432668E-05";
        }
        if (chip == 97) {
            cd11_card = "CD1_1   =   1.456844614546E-05";
            cd12_card = "CD1_2   =   2.362361286477E-05";
            cd21_card = "CD2_1   =   2.325262227103E-05";
            cd22_card = "CD2_2   =  -1.475820545534E-05";
        }
        if (chip == 98) {
            cd11_card = "CD1_1   =   1.458144581497E-05";
            cd12_card = "CD1_2   =   2.362707666313E-05";
            cd21_card = "CD2_1   =   2.324160624225E-05";
            cd22_card = "CD2_2   =  -1.473312328235E-05";
        }
        if (chip == 99) {
            cd11_card = "CD1_1   =   1.455019852321E-05";
            cd12_card = "CD1_2   =   2.360971083963E-05";
            cd21_card = "CD2_1   =   2.318569887528E-05";
            cd22_card = "CD2_2   =  -1.471748287733E-05";
        }
        if (chip == 100) {
            cd11_card = "CD1_1   =   1.459466798577E-05";
            cd12_card = "CD1_2   =   2.363835279112E-05";
            cd21_card = "CD2_1   =   2.331021658366E-05";
            cd22_card = "CD2_2   =  -1.476555865262E-05";
        }
        if (chip == 101) {
            cd11_card = "CD1_1   =   1.462889578684E-05";
            cd12_card = "CD1_2   =   2.365455717479E-05";
            cd21_card = "CD2_1   =   2.336691549319E-05";
            cd22_card = "CD2_2   =  -1.478140488864E-05";
        }
        if (chip == 102) {
            cd11_card = "CD1_1   =   1.465097909728E-05";
            cd12_card = "CD1_2   =   2.366082213840E-05";
            cd21_card = "CD2_1   =   2.335481236225E-05";
            cd22_card = "CD2_2   =  -1.476240665323E-05";
        }
        if (chip == 103) {
            cd11_card = "CD1_1   =   1.461348335527E-05";
            cd12_card = "CD1_2   =   2.364297143664E-05";
            cd21_card = "CD2_1   =   2.329920005989E-05";
            cd22_card = "CD2_2   =  -1.474878459863E-05";
        }
        if (chip == 104) {
            cd11_card = "CD1_1   =   1.467529271185E-05";
            cd12_card = "CD1_2   =   2.366308936433E-05";
            cd21_card = "CD2_1   =   2.340746995323E-05";
            cd22_card = "CD2_2   =  -1.480404342127E-05";
        }
        if (chip == 105) {
            cd11_card = "CD1_1   =   1.470429597672E-05";
            cd12_card = "CD1_2   =   2.368083243432E-05";
            cd21_card = "CD2_1   =   2.344817511825E-05";
            cd22_card = "CD2_2   =  -1.481562362596E-05";
        }
        if (chip == 106) {
            cd11_card = "CD1_1   =   1.471378714922E-05";
            cd12_card = "CD1_2   =   2.368480705449E-05";
            cd21_card = "CD2_1   =   2.344528329644E-05";
            cd22_card = "CD2_2   =  -1.479264705253E-05";
        }
        if (chip == 107) {
            cd11_card = "CD1_1   =   1.469039646450E-05";
            cd12_card = "CD1_2   =   2.366622872944E-05";
            cd21_card = "CD2_1   =   2.339280811626E-05";
            cd22_card = "CD2_2   =  -1.478679123857E-05";
        }
        if (chip == 108) {
            cd11_card = "CD1_1   =   1.448456124781E-05";
            cd12_card = "CD1_2   =   2.356870480893E-05";
            cd21_card = "CD2_1   =   2.322672350865E-05";
            cd22_card = "CD2_2   =  -1.478620501272E-05";
        }
        if (chip == 109) {
            cd11_card = "CD1_1   =   1.451624810953E-05";
            cd12_card = "CD1_2   =   2.359201431979E-05";
            cd21_card = "CD2_1   =   2.328267364599E-05";
            cd22_card = "CD2_2   =  -1.479878593260E-05";
        }
        if (chip == 110) {
            cd11_card = "CD1_1   =   1.453821329730E-05";
            cd12_card = "CD1_2   =   2.360556424422E-05";
            cd21_card = "CD2_1   =   2.327366929494E-05";
            cd22_card = "CD2_2   =  -1.478074454091E-05";
        }
        if (chip == 111) {
            cd11_card = "CD1_1   =   1.450171049883E-05";
            cd12_card = "CD1_2   =   2.358652838198E-05";
            cd21_card = "CD2_1   =   2.321395066344E-05";
            cd22_card = "CD2_2   =  -1.476656298734E-05";
        }
        if (chip == 112) {
            cd11_card = "CD1_1   =   1.456535647000E-05";
            cd12_card = "CD1_2   =   2.360732791496E-05";
            cd21_card = "CD2_1   =   2.332848426836E-05";
            cd22_card = "CD2_2   =  -1.481636744939E-05";
        }
        if (chip == 113) {
            cd11_card = "CD1_1   =   1.459539293765E-05";
            cd12_card = "CD1_2   =   2.362392642047E-05";
            cd21_card = "CD2_1   =   2.337757042428E-05";
            cd22_card = "CD2_2   =  -1.482275905507E-05";
        }
        if (chip == 114) {
            cd11_card = "CD1_1   =   1.461737683576E-05";
            cd12_card = "CD1_2   =   2.363458505163E-05";
            cd21_card = "CD2_1   =   2.336816423325E-05";
            cd22_card = "CD2_2   =  -1.481251124587E-05";
        }
        if (chip == 115) {
            cd11_card = "CD1_1   =   1.458199845963E-05";
            cd12_card = "CD1_2   =   2.361519968791E-05";
            cd21_card = "CD2_1   =   2.331733143272E-05";
            cd22_card = "CD2_2   =  -1.479661337260E-05";
        }
        if (chip == 116) {
            cd11_card = "CD1_1   =   1.463924696974E-05";
            cd12_card = "CD1_2   =   2.364079310047E-05";
            cd21_card = "CD2_1   =   2.343309376212E-05";
            cd22_card = "CD2_2   =  -1.483434678051E-05";
        }
        if (chip == 117) {
            cd11_card = "CD1_1   =   1.465806282379E-05";
            cd12_card = "CD1_2   =   2.365727606928E-05";
            cd21_card = "CD2_1   =   2.347474552836E-05";
            cd22_card = "CD2_2   =  -1.484455269629E-05";
        }
        if (chip == 118) {
            cd11_card = "CD1_1   =   1.467449621510E-05";
            cd12_card = "CD1_2   =   2.366915437751E-05";
            cd21_card = "CD2_1   =   2.346164367254E-05";
            cd22_card = "CD2_2   =  -1.483091767604E-05";
        }
        if (chip == 119) {
            cd11_card = "CD1_1   =   1.464607810124E-05";
            cd12_card = "CD1_2   =   2.365246911953E-05";
            cd21_card = "CD2_1   =   2.341976251802E-05";
            cd22_card = "CD2_2   =  -1.482166603044E-05";
        }
        if (chip == 120) {
            cd11_card = "CD1_1   =   1.444335830848E-05";
            cd12_card = "CD1_2   =   2.352213652959E-05";
            cd21_card = "CD2_1   =   2.324912548309E-05";
            cd22_card = "CD2_2   =  -1.482173634290E-05";
        }
        if (chip == 121) {
            cd11_card = "CD1_1   =   1.447437555213E-05";
            cd12_card = "CD1_2   =   2.354212082960E-05";
            cd21_card = "CD2_1   =   2.329125788986E-05";
            cd22_card = "CD2_2   =  -1.483333581566E-05";
        }
        if (chip == 122) {
            cd11_card = "CD1_1   =   1.450406249551E-05";
            cd12_card = "CD1_2   =   2.357194888358E-05";
            cd21_card = "CD2_1   =   2.328733246547E-05";
            cd22_card = "CD2_2   =  -1.482635384992E-05";
        }
        if (chip == 123) {
            cd11_card = "CD1_1   =   1.446887435936E-05";
            cd12_card = "CD1_2   =   2.355132541950E-05";
            cd21_card = "CD2_1   =   2.323652214511E-05";
            cd22_card = "CD2_2   =  -1.481003758771E-05";
        }
        if (chip == 124) {
            cd11_card = "CD1_1   =   1.452207857252E-05";
            cd12_card = "CD1_2   =   2.356246000914E-05";
            cd21_card = "CD2_1   =   2.334874579102E-05";
            cd22_card = "CD2_2   =  -1.484438600811E-05";
        }
        if (chip == 125) {
            cd11_card = "CD1_1   =   1.455472502248E-05";
            cd12_card = "CD1_2   =   2.358120746189E-05";
            cd21_card = "CD2_1   =   2.339514205919E-05";
            cd22_card = "CD2_2   =  -1.484757631193E-05";
        }
        if (chip == 126) {
            cd11_card = "CD1_1   =   1.457348396577E-05";
            cd12_card = "CD1_2   =   2.360195486445E-05";
            cd21_card = "CD2_1   =   2.339133293538E-05";
            cd22_card = "CD2_2   =  -1.483895066403E-05";
        }
        if (chip == 127) {
            cd11_card = "CD1_1   =   1.454008195769E-05";
            cd12_card = "CD1_2   =   2.358410900742E-05";
            cd21_card = "CD2_1   =   2.334275603655E-05";
            cd22_card = "CD2_2   =  -1.483080248943E-05";
        }
        if (chip == 128) {
            cd11_card = "CD1_1   =   1.458901562113E-05";
            cd12_card = "CD1_2   =   2.359178549552E-05";
            cd21_card = "CD2_1   =   2.344306428878E-05";
            cd22_card = "CD2_2   =  -1.486337104280E-05";
        }
        if (chip == 129) {
            cd11_card = "CD1_1   =   1.462175986402E-05";
            cd12_card = "CD1_2   =   2.361733887704E-05";
            cd21_card = "CD2_1   =   2.348607546703E-05";
            cd22_card = "CD2_2   =  -1.486513424075E-05";
        }
        if (chip == 130) {
            cd11_card = "CD1_1   =   1.463529593407E-05";
            cd12_card = "CD1_2   =   2.364178867895E-05";
            cd21_card = "CD2_1   =   2.348601948871E-05";
            cd22_card = "CD2_2   =  -1.485575720965E-05";
        }
        if (chip == 131) {
            cd11_card = "CD1_1   =   1.460986224421E-05";
            cd12_card = "CD1_2   =   2.361203114887E-05";
            cd21_card = "CD2_1   =   2.343949597677E-05";
            cd22_card = "CD2_2   =  -1.485566411533E-05";
        }
        if (chip == 132) {
            cd11_card = "CD1_1   =   1.439902433134E-05";
            cd12_card = "CD1_2   =   2.345629610913E-05";
            cd21_card = "CD2_1   =   2.325740535484E-05";
            cd22_card = "CD2_2   =  -1.485596236971E-05";
        }
        if (chip == 133) {
            cd11_card = "CD1_1   =   1.443298642922E-05";
            cd12_card = "CD1_2   =   2.347635387290E-05";
            cd21_card = "CD2_1   =   2.330621667414E-05";
            cd22_card = "CD2_2   =  -1.486033282241E-05";
        }
        if (chip == 134) {
            cd11_card = "CD1_1   =   1.445781151314E-05";
            cd12_card = "CD1_2   =   2.350565643289E-05";
            cd21_card = "CD2_1   =   2.330167260556E-05";
            cd22_card = "CD2_2   =  -1.484873085712E-05";
        }
        if (chip == 135) {
            cd11_card = "CD1_1   =   1.441809713258E-05";
            cd12_card = "CD1_2   =   2.348311689798E-05";
            cd21_card = "CD2_1   =   2.325596352129E-05";
            cd22_card = "CD2_2   =  -1.484304513657E-05";
        }
        if (chip == 136) {
            cd11_card = "CD1_1   =   1.447546727940E-05";
            cd12_card = "CD1_2   =   2.350013156095E-05";
            cd21_card = "CD2_1   =   2.336394669080E-05";
            cd22_card = "CD2_2   =  -1.486967033189E-05";
        }
        if (chip == 137) {
            cd11_card = "CD1_1   =   1.450311305828E-05";
            cd12_card = "CD1_2   =   2.352760541293E-05";
            cd21_card = "CD2_1   =   2.341159788465E-05";
            cd22_card = "CD2_2   =  -1.487196208818E-05";
        }
        if (chip == 138) {
            cd11_card = "CD1_1   =   1.453072035878E-05";
            cd12_card = "CD1_2   =   2.354794773212E-05";
            cd21_card = "CD2_1   =   2.339943611848E-05";
            cd22_card = "CD2_2   =  -1.487102562063E-05";
        }
        if (chip == 139) {
            cd11_card = "CD1_1   =   1.450500310877E-05";
            cd12_card = "CD1_2   =   2.352971109315E-05";
            cd21_card = "CD2_1   =   2.335309015628E-05";
            cd22_card = "CD2_2   =  -1.486063065637E-05";
        }
        if (chip == 140) {
            cd11_card = "CD1_1   =   1.454752168874E-05";
            cd12_card = "CD1_2   =   2.354646194800E-05";
            cd21_card = "CD2_1   =   2.345742103297E-05";
            cd22_card = "CD2_2   =  -1.487783136191E-05";
        }
        if (chip == 141) {
            cd11_card = "CD1_1   =   1.458005675672E-05";
            cd12_card = "CD1_2   =   2.356162882797E-05";
            cd21_card = "CD2_1   =   2.350611783105E-05";
            cd22_card = "CD2_2   =  -1.487779943882E-05";
        }
        if (chip == 142) {
            cd11_card = "CD1_1   =   1.459564236634E-05";
            cd12_card = "CD1_2   =   2.359293940495E-05";
            cd21_card = "CD2_1   =   2.349497570450E-05";
            cd22_card = "CD2_2   =  -1.487429906181E-05";
        }
        if (chip == 143) {
            cd11_card = "CD1_1   =   1.456847619401E-05";
            cd12_card = "CD1_2   =   2.356804605434E-05";
            cd21_card = "CD2_1   =   2.346290171106E-05";
            cd22_card = "CD2_2   =  -1.487358881329E-05";
        }
        individualFixDone = true;
    }
    if (instData.name == "FourStar@LCO") {     // FourStar has no CD matrix in the header
        if (!searchKeyValue(QStringList() << "ROTANGLE", positionAngle)) {
            emit messageAvailable(name + " : Could not find ROTANGLE keyword, set to zero! CD matrix might have wrong orientation.", "warning");
            emit warning();
            positionAngle = 0.0;
        }
        cd11 = -1.*instData.pixscale / 3600.;
        cd12 = 0.0;
        cd21 = 0.0;
        cd22 = instData.pixscale / 3600.;
        rotateCDmatrix(cd11, cd12, cd21, cd22, positionAngle);
        cd11_card = "CD1_1   =  "+QString::number(cd11, 'g', 6);
        cd12_card = "CD1_2   =  "+QString::number(cd12, 'g', 6);
        cd21_card = "CD2_1   =  "+QString::number(cd21, 'g', 6);
        cd22_card = "CD2_2   =  "+QString::number(cd22, 'g', 6);
        individualFixDone = true;
    }
    if (instData.name == "NISP@EUCLID") {     // NISP has no CD matrix in the header
        cd11 = instData.pixscale / 3600.;
        cd12 = 0.0;
        cd21 = 0.0;
        cd22 = instData.pixscale / 3600.;
        cd11_card = "CD1_1   =  "+QString::number(cd11, 'g', 6);
        cd12_card = "CD1_2   =  "+QString::number(cd12, 'g', 6);
        cd21_card = "CD2_1   =  "+QString::number(cd21, 'g', 6);
        cd22_card = "CD2_2   =  "+QString::number(cd22, 'g', 6);
        individualFixDone = true;
    }
    if (instData.name.contains("IMACS_F2")) {     // IMACS has no CD matrix in the header
        if (!searchKeyValue(QStringList() << "ROTATORE", positionAngle)) {
            emit messageAvailable(name + " : Could not find ROTANGLE keyword, set to zero! CD matrix might have wrong orientation.", "warning");
            emit warning();
            positionAngle = 0.0;
        }

        if (chip<=3) {
            cd11 = 0.;
            cd12 = -instData.pixscale / 3600.;
            cd21 = instData.pixscale / 3600.;
            cd22 = 0.;
        }
        if (chip>=4) {
            cd11 = 0.;
            cd12 = instData.pixscale / 3600.;
            cd21 = -instData.pixscale / 3600.;
            cd22 = 0.;
        }
        cd11_card = "CD1_1   =  "+QString::number(cd11, 'g', 6);
        cd12_card = "CD1_2   =  "+QString::number(cd12, 'g', 6);
        cd21_card = "CD2_1   =  "+QString::number(cd21, 'g', 6);
        cd22_card = "CD2_2   =  "+QString::number(cd22, 'g', 6);
        individualFixDone = true;
    }
    if (instData.name.contains("IMACS_F4")) {     // IMACS has no CD matrix in the header
        if (!searchKeyValue(QStringList() << "ROTATORE", positionAngle)) {
            emit messageAvailable(name + " : Could not find ROTANGLE keyword, set to zero! CD matrix might have wrong orientation.", "warning");
            emit warning();
            positionAngle = 0.0;
        }

        cd11 = instData.pixscale / 3600.;
        cd12 = 0.;
        cd21 = 0.;
        cd22 = -instData.pixscale / 3600.;
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
        cd11 = -1.*instData.pixscale / 3600.;
        cd12 = 0.0;
        cd21 = 0.0;
        cd22 = instData.pixscale / 3600.;
        rotateCDmatrix(cd11, cd12, cd21, cd22, positionAngle);
        cd11_card = "CD1_1   =  "+QString::number(cd11, 'g', 6);
        cd12_card = "CD1_2   =  "+QString::number(cd12, 'g', 6);
        cd21_card = "CD2_1   =  "+QString::number(cd21, 'g', 6);
        cd22_card = "CD2_2   =  "+QString::number(cd22, 'g', 6);
        individualFixDone = true;
    }
    if (instData.name == "SUSI1@NTT") {
        cd11 = -3.611e-5;
        cd12 = 0.;
        cd21 = 0.;
        cd22 = 3.611e-5;
        cd11_card = "CD1_1   =  "+QString::number(cd11, 'g', 6);
        cd12_card = "CD1_2   =  "+QString::number(cd12, 'g', 6);
        cd21_card = "CD2_1   =  "+QString::number(cd21, 'g', 6);
        cd22_card = "CD2_2   =  "+QString::number(cd22, 'g', 6);
        individualFixDone = true;
    }
    if (instNameFromData == "GROND_OPT@MPGESO") {
        // GROND optical data has both the NIR and OPT CD matrices in the header.
        // With the current scheme, the NIR matrix in the HDU gets picked over
        // OPT matrix in the extension
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
        cd11 = 6.55e-5;
        cd12 = 5.0e-7;
        cd21 = 5.0e-7;
        cd22 = -6.55e-5;
        cd11_card = "CD1_1   =  "+QString::number(cd11, 'g', 6);
        cd12_card = "CD1_2   =  "+QString::number(cd12, 'g', 6);
        cd21_card = "CD2_1   =  "+QString::number(cd21, 'g', 6);
        cd22_card = "CD2_2   =  "+QString::number(cd22, 'g', 6);
        individualFixDone = true;
    }
    if (instData.name == "PISCO@LCO") {
        cd11 = -instData.pixscale / 3600.;
        cd12 = 0.;
        cd21 = 0.;
        cd22 = -instData.pixscale / 3600.;
        // headers appear unreliable (pixel scale wrong by a factor of two)
        /*
        searchKeyValue(QStringList() << "CD1_1", cd11);
        searchKeyValue(QStringList() << "CD1_2", cd12);
        searchKeyValue(QStringList() << "CD2_1", cd21);
        searchKeyValue(QStringList() << "CD2_2", cd22);
        // readout amplifiers are flipped, depending on chip
        if (chip == 1 || chip == 2) {cd11 *= -1.; cd21 *= -1.;}
        if (chip == 4 || chip == 7) {cd12 *= -1.; cd22 *= -1.;}
        if (chip == 5 || chip == 6) {cd11 *= -1.; cd21 *= -1.; cd12 *= -1.; cd22 *= -1.;}
        */
        cd11_card = "CD1_1   =  "+QString::number(cd11, 'g', 6);
        cd12_card = "CD1_2   =  "+QString::number(cd12, 'g', 6);
        cd21_card = "CD2_1   =  "+QString::number(cd21, 'g', 6);
        cd22_card = "CD2_2   =  "+QString::number(cd22, 'g', 6);
        individualFixDone = true;
    }
    if (instData.name == "90Prime@BOK2.3m") {
        // CD matrix appears to assume a plate scale of 1.0 instead of 0.452 arcsec/pixel. It is also rotated by 90 degrees and flipped.
        // We fix it manually, and hope that it doesn't change.
        cd11_card = "CD1_1   = 0.";
        cd22_card = "CD2_2   = 0.";
        if (chip == 0 || chip == 4 || chip == 11 || chip == 15) {
            cd12_card = "CD1_2   =   1.258E-04 ";
            cd21_card = "CD2_1   =   1.258E-04 ";
        }
        if (chip == 1 || chip == 5 || chip == 10 || chip == 14) {
            cd12_card = "CD1_2   =   1.258E-04 ";
            cd21_card = "CD2_1   =  -1.258E-04 ";
        }
        if (chip == 2 || chip == 6 || chip == 9 || chip == 13) {
            cd12_card = "CD1_2   =  -1.258E-04 ";
            cd21_card = "CD2_1   =   1.258E-04 ";
        }
        if (chip == 3 || chip == 7 || chip == 8 || chip == 12) {
            cd12_card = "CD1_2   =  -1.258E-04 ";
            cd21_card = "CD2_1   =  -1.258E-04 ";
        }
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
    if (instData.name == "SOFI@NTT") {           // https://www.eso.org/sci/facilities/lasilla/instruments/sofi/inst/setup/Detector_characteristic.html
        chipGain = 5.3;
        individualFixDone = true;
    }
    if (instData.name == "NISP@EUCLID") {
        chipGain = 2.0;
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
    if (instData.name == "INOLA@INO") {          // https://www.sbig.de/stf-8300/stf-8300-techdat.pdf
        chipGain = 0.37;
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
    // multi-amp cameras where the gain is NOT available in the header
    else if (instData.name == "DEIMOS_2AMP@KECK") {  // https://www2.keck.hawaii.edu/inst/obsdata/inst/deimos/www/detector_data/deimos_detector_data.html
        if (chip == 0) chipGain = 1.206;
        if (chip == 1) chipGain = 1.221;
        if (chip == 2) chipGain = 1.200;
        if (chip == 3) chipGain = 1.188;
        if (chip == 4) chipGain = 1.167;
        if (chip == 5) chipGain = 1.250;
        if (chip == 6) chipGain = 1.217;
        if (chip == 7) chipGain = 1.228;
        chipGain = harmonicGain(multiportGains);
        individualFixDone = true;
    }
    else if (instData.name == "VIRCAM@VISTA") {  // https://www.eso.org/sci/facilities/paranal/instruments/vircam/doc/VIS-MAN-ESO-06000-0002_v108.pdf
        if (chip == 0) chipGain = 3.7;
        if (chip == 1) chipGain = 4.2;
        if (chip == 2) chipGain = 4.0;
        if (chip == 3) chipGain = 4.2;
        if (chip == 4) chipGain = 4.2;
        if (chip == 5) chipGain = 4.1;
        if (chip == 6) chipGain = 3.9;
        if (chip == 7) chipGain = 4.2;
        if (chip == 8) chipGain = 4.6;
        if (chip == 9) chipGain = 4.0;
        if (chip == 10) chipGain = 4.6;
        if (chip == 11) chipGain = 4.0;
        if (chip == 12) chipGain = 5.8;
        if (chip == 13) chipGain = 4.8;
        if (chip == 14) chipGain = 4.0;
        if (chip == 15) chipGain = 5.0;
        chipGain = harmonicGain(multiportGains);
        individualFixDone = true;
    }
    // multi-amp cameras where the gain is available in the header
    else if (instData.name == "SuprimeCam_200808-201705@SUBARU"
             || instData.name == "HSC@SUBARU"
             || instData.name.contains("GMOS-N-HAM")
             || instData.name.contains("GMOS-S-HAM")
             || instData.name.contains("SAMI")          // have not added yet 1x1 confog
             || instData.name.contains("SOI@SOAR")
             || instData.name == "LRIS_BLUE@KECK"
             || instData.name == "LRIS_RED@KECK"
             || instData.name == "MOSAIC-II_16@CTIO"
             || instData.name == "MOSAIC-III_4@KPNO_4m"
             || instData.name == "PISCO@LCO"
             ) {
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
    else if (instData.name.contains("NOTcam")) {
        float gain1 = 0.0;
        float gain2 = 0.0;
        float gain3 = 0.0;
        float gain4 = 0.0;
        searchKeyValue(QStringList() << "GAIN1", gain1);
        searchKeyValue(QStringList() << "GAIN2", gain2);
        searchKeyValue(QStringList() << "GAIN3", gain3);
        searchKeyValue(QStringList() << "GAIN4", gain4);
        channelGains.clear();
        channelGains << gain1 << gain2 << gain3 << gain4;
        chipGain = harmonicGain(channelGains);
        individualFixDone = true;
    }
    else if (instData.name == "90Prime@BOK2.3m") {
        //        QString gainkeyword = "GAIN"+QString::number(chip+1);
        //        searchKeyValue(QStringList() << gainkeyword, chipGain);
        if (chip == 0)  {searchKeyValue(QStringList() << "GAIN1", chipGain);  chipGain /= 1.0000;}
        if (chip == 1)  {searchKeyValue(QStringList() << "GAIN2", chipGain);  chipGain /= 1.0033;}
        if (chip == 2)  {searchKeyValue(QStringList() << "GAIN3", chipGain);  chipGain /= 1.0113;}
        if (chip == 3)  {searchKeyValue(QStringList() << "GAIN4", chipGain);  chipGain /= 0.9681;}
        if (chip == 4)  {searchKeyValue(QStringList() << "GAIN5", chipGain);  chipGain /= 1.0000;}
        if (chip == 5)  {searchKeyValue(QStringList() << "GAIN6", chipGain);  chipGain /= 0.9406;}
        if (chip == 6)  {searchKeyValue(QStringList() << "GAIN7", chipGain);  chipGain /= 0.9823;}
        if (chip == 7)  {searchKeyValue(QStringList() << "GAIN8", chipGain);  chipGain /= 0.9504;}
        if (chip == 8)  {searchKeyValue(QStringList() << "GAIN9", chipGain);  chipGain /= 1.0000;}
        if (chip == 9)  {searchKeyValue(QStringList() << "GAIN10", chipGain); chipGain /= 0.9729;}
        if (chip == 10) {searchKeyValue(QStringList() << "GAIN11", chipGain); chipGain /= 1.0204;}
        if (chip == 11) {searchKeyValue(QStringList() << "GAIN12", chipGain); chipGain /= 0.9789;}
        if (chip == 12) {searchKeyValue(QStringList() << "GAIN13", chipGain); chipGain /= 1.0000;}
        if (chip == 13) {searchKeyValue(QStringList() << "GAIN14", chipGain); chipGain /= 0.9982;}
        if (chip == 14) {searchKeyValue(QStringList() << "GAIN15", chipGain); chipGain /= 1.0366;}
        if (chip == 15) {searchKeyValue(QStringList() << "GAIN16", chipGain); chipGain /= 0.9660;}
        individualFixDone = true;
    }
    else if (instData.name == "VIS@EUCLID") {
        if (chip == 0) chipGain = 3.559;
        if (chip == 1) chipGain = 3.556;
        if (chip == 2) chipGain = 3.537;
        if (chip == 3) chipGain = 3.515;
        if (chip == 4) chipGain = 3.525;
        if (chip == 5) chipGain = 3.561;
        if (chip == 6) chipGain = 3.486;
        if (chip == 7) chipGain = 3.445;
        if (chip == 8) chipGain = 3.579;
        if (chip == 9) chipGain = 3.520;
        if (chip == 10) chipGain = 3.477;
        if (chip == 11) chipGain = 3.496;
        if (chip == 12) chipGain = 3.531;
        if (chip == 13) chipGain = 3.621;
        if (chip == 14) chipGain = 3.448;
        if (chip == 15) chipGain = 3.503;
        if (chip == 16) chipGain = 3.418;
        if (chip == 17) chipGain = 3.425;
        if (chip == 18) chipGain = 3.619;
        if (chip == 19) chipGain = 3.513;
        if (chip == 20) chipGain = 3.580;
        if (chip == 21) chipGain = 3.548;
        if (chip == 22) chipGain = 3.454;
        if (chip == 23) chipGain = 3.471;
        if (chip == 24) chipGain = 3.387;
        if (chip == 25) chipGain = 3.416;
        if (chip == 26) chipGain = 3.440;
        if (chip == 27) chipGain = 3.449;
        if (chip == 28) chipGain = 3.483;
        if (chip == 29) chipGain = 3.469;
        if (chip == 30) chipGain = 3.477;
        if (chip == 31) chipGain = 3.457;
        if (chip == 32) chipGain = 3.430;
        if (chip == 33) chipGain = 3.446;
        if (chip == 34) chipGain = 3.464;
        if (chip == 35) chipGain = 3.483;
        if (chip == 36) chipGain = 3.406;
        if (chip == 37) chipGain = 3.514;
        if (chip == 38) chipGain = 3.437;
        if (chip == 39) chipGain = 3.476;
        if (chip == 40) chipGain = 3.575;
        if (chip == 41) chipGain = 3.571;
        if (chip == 42) chipGain = 3.582;
        if (chip == 43) chipGain = 3.489;
        if (chip == 44) chipGain = 3.345;
        if (chip == 45) chipGain = 3.515;
        if (chip == 46) chipGain = 3.527;
        if (chip == 47) chipGain = 3.431;
        if (chip == 48) chipGain = 3.539;
        if (chip == 49) chipGain = 3.476;
        if (chip == 50) chipGain = 3.513;
        if (chip == 51) chipGain = 3.501;
        if (chip == 52) chipGain = 3.535;
        if (chip == 53) chipGain = 3.504;
        if (chip == 54) chipGain = 3.593;
        if (chip == 55) chipGain = 3.520;
        if (chip == 56) chipGain = 3.597;
        if (chip == 57) chipGain = 3.626;
        if (chip == 58) chipGain = 3.645;
        if (chip == 59) chipGain = 3.630;
        if (chip == 60) chipGain = 3.597;
        if (chip == 61) chipGain = 3.657;
        if (chip == 62) chipGain = 3.586;
        if (chip == 63) chipGain = 3.627;
        if (chip == 64) chipGain = 3.466;
        if (chip == 65) chipGain = 3.477;
        if (chip == 66) chipGain = 3.471;
        if (chip == 67) chipGain = 3.504;
        if (chip == 68) chipGain = 3.467;
        if (chip == 69) chipGain = 3.466;
        if (chip == 70) chipGain = 3.439;
        if (chip == 71) chipGain = 3.442;
        if (chip == 72) chipGain = 3.389;
        if (chip == 73) chipGain = 3.372;
        if (chip == 74) chipGain = 3.393;
        if (chip == 75) chipGain = 3.448;
        if (chip == 76) chipGain = 3.443;
        if (chip == 77) chipGain = 3.409;
        if (chip == 78) chipGain = 3.405;
        if (chip == 79) chipGain = 3.454;
        if (chip == 80) chipGain = 3.395;
        if (chip == 81) chipGain = 3.409;
        if (chip == 82) chipGain = 3.405;
        if (chip == 83) chipGain = 3.465;
        if (chip == 84) chipGain = 3.396;
        if (chip == 85) chipGain = 3.412;
        if (chip == 86) chipGain = 3.463;
        if (chip == 87) chipGain = 3.343;
        if (chip == 88) chipGain = 3.491;
        if (chip == 89) chipGain = 3.438;
        if (chip == 90) chipGain = 3.573;
        if (chip == 91) chipGain = 3.463;
        if (chip == 92) chipGain = 3.344;
        if (chip == 93) chipGain = 3.385;
        if (chip == 94) chipGain = 3.498;
        if (chip == 95) chipGain = 3.378;
        if (chip == 96) chipGain = 3.526;
        if (chip == 97) chipGain = 3.484;
        if (chip == 98) chipGain = 3.534;
        if (chip == 99) chipGain = 3.480;
        if (chip == 100) chipGain = 3.517;
        if (chip == 101) chipGain = 3.456;
        if (chip == 102) chipGain = 3.559;
        if (chip == 103) chipGain = 3.556;
        if (chip == 104) chipGain = 3.665;
        if (chip == 105) chipGain = 3.679;
        if (chip == 106) chipGain = 3.586;
        if (chip == 107) chipGain = 3.493;
        if (chip == 108) chipGain = 3.553;
        if (chip == 109) chipGain = 3.501;
        if (chip == 110) chipGain = 3.627;
        if (chip == 111) chipGain = 3.556;
        if (chip == 112) chipGain = 3.583;
        if (chip == 113) chipGain = 3.566;
        if (chip == 114) chipGain = 3.529;
        if (chip == 115) chipGain = 3.545;
        if (chip == 116) chipGain = 3.498;
        if (chip == 117) chipGain = 3.567;
        if (chip == 118) chipGain = 3.585;
        if (chip == 119) chipGain = 3.540;
        if (chip == 120) chipGain = 3.500;
        if (chip == 121) chipGain = 3.444;
        if (chip == 122) chipGain = 3.541;
        if (chip == 123) chipGain = 3.578;
        if (chip == 124) chipGain = 3.578;
        if (chip == 125) chipGain = 3.582;
        if (chip == 126) chipGain = 3.548;
        if (chip == 127) chipGain = 3.577;
        if (chip == 128) chipGain = 3.572;
        if (chip == 129) chipGain = 3.535;
        if (chip == 130) chipGain = 3.509;
        if (chip == 131) chipGain = 3.491;
        if (chip == 132) chipGain = 3.440;
        if (chip == 133) chipGain = 3.365;
        if (chip == 134) chipGain = 3.436;
        if (chip == 135) chipGain = 3.459;
        if (chip == 136) chipGain = 3.525;
        if (chip == 137) chipGain = 3.479;
        if (chip == 138) chipGain = 3.453;
        if (chip == 139) chipGain = 3.463;
        if (chip == 140) chipGain = 3.673;
        if (chip == 141) chipGain = 3.599;
        if (chip == 142) chipGain = 3.563;
        if (chip == 143) chipGain = 3.636;
        individualFixDone = true;
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
                || instData.name.contains("SOI@SOAR")
                || instData.name == "LRIS_BLUE@KECK"
                || instData.name == "LRIS_RED@KECK"
                || instData.name == "MOSAIC-II_16@CTIO"
                || instData.name == "MOSAIC-III_4@KPNO_4m"
                ) {
            gain[chip/numAmpPerChip] = chipGain;
        }
        // multichannel imagers have less chips than gain.length() ('chip' counts higher than gain.length() )
        else if (instData.name == "PISCO@LCO") {
            gain[0] = chipGain;
            gain[1] = chipGain;
        }
        else {
            gain[chip] = chipGain;        // not applied for e.g. SuprimeCam_200808-201705 and the others (why?)
        }

        gainForSaturation = chipGain;
    }

    return individualFixDone;
}

// Build the AIRMASS keyword
void Splitter::buildTheliHeaderAIRMASS()
{
    if (!successProcessing) return;

    if (instData.name == "NISP@EUCLID" || instData.name == "VIS@EUCLID") {
        QString card = "AIRMASS = 1.000";    // Not sure what happens if I set it to zero
        card.resize(80, ' ');
        headerTHELI.append(card);
        return;   // without errors
    }

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
        if (chip == 0 || chip == 1) filter = "g";
        if (chip == 2 || chip == 3) filter = "r";
        if (chip == 4 || chip == 5) filter = "i";
        if (chip == 6 || chip == 7) filter = "z";
        filterCard = "FILTER  = '"+filter+"'";
        individualFixDone = true;
    }

    if (instData.name == "VIS@EUCLID") {
        filter = "IE";
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

    if (instData.name == "NISP@EUCLID") {
        QString fpos = "";
        QString gpos = "";
        searchKeyValue(QStringList() << "FWA_POS", fpos);
        searchKeyValue(QStringList() << "GWA_POS", gpos);
        if (fpos == "CLOSED") filter = "DARK";
        else if (fpos == "OPEN") filter = gpos;
        else if (gpos == "OPEN" && fpos != "OPEN") filter = fpos;
        else {
            // nothing yet
        }
        filter = filter.remove("FW-");
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
