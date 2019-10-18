#include "splitter.h"
#include "../instrumentdata.h"
#include "../myimage/myimage.h"
#include "../myfits/myfits.h"
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
            fallback.resize(80, ' ');
            headerTHELI.append(fallback);
        }

        if (!keyFound) {
            if (*verbosity > 1) emit messageAvailable(fileName + " : Could not determine keyword: "+mandatoryKey+", using default value", "ignore");
        }
    }

    // Propagate bayer matrix ID for color CCD chips
    if (!instData.bayer.isEmpty()) {
        QString card = "BAYER   = '"+instData.bayer+"'";
        card.resize(80, ' ');
        headerTHELI.append(card);
    }

    // Append DUMMY keywords
    headerTHELI.append(dummyKeys);
}

void Splitter::buildTheliHeaderMJDOBS()
{
    if (!successProcessing) return;

    // List of instruments for which MJD-OBS is not reliable and should be constructed from DATE-OBS
    QStringList instruments = {"GSAOI@GEMINI", "GSAOI_CHIP1@GEMINI", "GSAOI_CHIP2@GEMINI", "GSAOI_CHIP3@GEMINI", "GSAOI_CHIP4@GEMINI",
                               "FLAMINGOS2@GEMINI"};
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
    // Exceptions
    if (instData.name == "WFC@INT") {
        individualFixCRVAL();
        return;
    }

    QStringList headerWCS;

    // Use dedicated lookup
    searchKeyCRVAL("CRVAL1", headerDictionary.value("CRVAL1"), headerWCS);
    searchKeyCRVAL("CRVAL2", headerDictionary.value("CRVAL2"), headerWCS);

    headerTHELI.append(headerWCS);
}

void Splitter::WCSbuildCTYPE()
{
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
    // Exceptions
    if (instData.name == "WFC@INT") {
        individualFixCDmatrix(chip);
        return;
    }

    QStringList headerWCS;
    QString fallback = "";

    QStringList wcsKeys = {"CD1_1", "CD1_2", "CD2_1", "CD2_2"};

    for (auto &wcsKey : wcsKeys) {
        bool keyFound = searchKey(wcsKey, headerDictionary.value(wcsKey), headerWCS);
        // default values if failed
        if (!keyFound && wcsKey == "CD1_1") {
            bool found = searchKey("CD1_1", headerDictionary.value("CDELT1"), headerWCS);  // first argument, CD1_1, is taken to form the new header card. Value is taken from CDELT1
            if (!found) fallback = "CD1_1   = "+QString::number(-1.*instData.pixscale/3600., 'g', 6);
        }
        if (!keyFound && wcsKey == "CD2_2") {
            bool found = searchKey("CD2_2", headerDictionary.value("CDELT2"), headerWCS);
            if (!found) fallback = "CD2_2   = "+QString::number(instData.pixscale/3600., 'g', 6);
        }
        if (!keyFound && wcsKey == "CD1_2") fallback = "CD1_2   = 0.0";
        if (!keyFound && wcsKey == "CD2_1") fallback = "CD2_1   = 0.0";

        if (!keyFound) {
            fallback.resize(80, ' ');
            headerWCS.append(fallback);
            if (*verbosity > 1) emit messageAvailable(fileName + " : Could not determine keyword: "+wcsKey+", using default value", "ignore");
        }
    }

    headerTHELI.append(headerWCS);
}

void Splitter::WCSbuildRADESYS()
{
    QStringList headerWCS;
    QString wcsKey = "RADESYS";
    bool keyFound = searchKey(wcsKey, headerDictionary.value(wcsKey), headerWCS);
    if (!keyFound) {
        QString card = "RADESYS = 'ICRS'";
        card.resize(80, ' ');
        headerWCS.append(card);
        if (*verbosity > 1) emit messageAvailable(fileName + " : Could not determine keyword: "+wcsKey+", using default value", "ignore");
    }
    headerTHELI.append(headerWCS);
}

void Splitter::WCSbuildEQUINOX()
{
    QStringList headerWCS;
    QString wcsKey = "EQUINOX";
    bool keyFound = searchKey(wcsKey, headerDictionary.value(wcsKey), headerWCS);
    if (!keyFound) {
        QString card = "EQUINOX = 2000.0";
        card.resize(80, ' ');
        headerWCS.append(card);
        if (*verbosity > 1) emit messageAvailable(fileName + " : Could not determine keyword: "+wcsKey+", using default value", "ignore");
    }
    headerTHELI.append(headerWCS);
}

void Splitter::individualFixCRVAL()
{
    QStringList headerWCS;

    QString crval1_card = "";
    QString crval2_card = "";

    QString crval1;
    QString crval2;
    searchKeyValue(headerDictionary.value("CRVAL1"), crval1);
    searchKeyValue(headerDictionary.value("CRVAL2"), crval2);

    // Fix format (sometimes we have 'HH MM SS' instead of 'HH:MM:SS')
    crval1.replace(' ', ':');
    crval2.replace(' ', ':');

    // Convert to decimal format if necessary
    if (crval1.contains(':')) crval1 = hmsToDecimal(crval1);
    if (crval2.contains(':')) crval2 = dmsToDecimal(crval2);

    if (instData.name == "WFC@INT") {
        double alpha = crval1.toDouble();
        double delta = crval2.toDouble();
        // reset the coordinates such that scamp does not get confused (optical axis != crpix by ~4 arcminutes)
        alpha = alpha - 0.0733/cos(delta*3.14159/180.);
        if (alpha > 360.) alpha -= 360.;
        if (alpha < 0.) alpha += 360.;
        delta = delta - 0.02907;
        crval1_card = "CRVAL1  = "+QString::number(alpha, 'f', 6);
        crval2_card = "CRVAL2  = "+QString::number(delta, 'f', 6);
    }

    crval1_card.resize(80, ' ');
    crval2_card.resize(80, ' ');

    headerWCS.append(crval1_card);
    headerWCS.append(crval2_card);

    headerTHELI.append(headerWCS);
}

void Splitter::individualFixCDmatrix(int chip)
{
    QStringList headerWCS;

    QString cd11_card = "";
    QString cd12_card = "";
    QString cd21_card = "";
    QString cd22_card = "";

    if (instData.name == "WFC@INT") {
        if (chip == 0 || chip == 2 || chip == 3) {
            cd11_card = "CD1_1   = 0.0";
            cd12_card = "CD1_2   = -0.0000919444";
            cd21_card = "CD2_1   = -0.0000919444";
            cd22_card = "CD2_2   = 0.0";
        }
        else {
            cd11_card = "CD1_1   = -0.0000919444";
            cd12_card = "CD1_2   = 0.0";
            cd21_card = "CD2_1   = 0.0";
            cd22_card = "CD2_2   = -0.0000919444";
        }
    }

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

// Build the EXPTIME keyword
void Splitter::buildTheliHeaderEXPTIME()
{
    if (!successProcessing) return;

    // List of instruments that have separate DITs and NDITs
    QStringList nditInstruments = {"GSAOI@GEMINI", "GSAOI_CHIP1@GEMINI", "GSAOI_CHIP2@GEMINI", "GSAOI_CHIP3@GEMINI", "GSAOI_CHIP4@GEMINI",
                                   "HAWKI@VLT", "INGRID@WHT", "IRCS_HIGHRES@SUBARU", "IRCS_LOWRES@SUBARU", "ISAAC@VLT", "ISPI@CTIO",
                                   "LIRIS@WHT", "LIRIS_POL@WHT", "MOIRCS@SUBARU", "MOSFIRE@KECK", "NEWFIRM@CTIO", "NICS@TNG", "NIRC2@KECK",
                                   "NIRI@GEMINI", "PISCES@LBT", "SOFI@NTT", "VIRCAM@VISTA"};

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

    // Instruments for which we have to determine the true total EXPTIME keyword, and rescale the data.
    // In THELI, EXPTIME always represents the total effective integration time. If images are averaged from several coadds,
    // they also need to be rescaled.

    bool foundDIT = false;
    bool foundNDIT = false;
    float dit = -1.0;
    float ndit = -1.0;

    foundDIT = searchKeyValue(headerDictionary.value("DIT"), dit);
    foundNDIT = searchKeyValue(headerDictionary.value("NDIT"), ndit);

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

    bool keyFound = searchKey("DATE-OBS", headerDictionary.value("DATE-OBS"), headerTHELI);
    if (keyFound && checkFormatDATEOBS()) return;

    // Fallback: Try and reconstruct DATE-OBS keyword from other keywords
    QString dateValue;
    QString timeValue;
    bool foundDATE = searchKeyValue(headerDictionary.value("DATE"), dateValue);
    bool foundTIME = searchKeyValue(headerDictionary.value("TIME"), timeValue);
    if (foundDATE && foundTIME) {
        if (dateValue.contains("-") && timeValue.contains(":")) {
            dateObsValue = dateValue+"T"+timeValue;
        }
        else {
            emit messageAvailable(fileName + " : Could not determine keyword: DATE-OBS, set to 2020-01-01T00:00:00.000", "warning");
            emit warning();
            dateObsValue = "2020-01-01T00:00:00.000";
            dateObsValid = false;
        }
    }

    QString card = "DATE-OBS= '"+dateObsValue+"'";
    card.resize(80, ' ');
    headerTHELI.append(card);
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
            emit messageAvailable(fileName + " : Insufficient data to determine AIRMASS, set to 1.0", "warning");
        }
    }
}

void Splitter::buildTheliHeaderFILTER()
{
    QStringList filterKeywordList;
    QStringList possibleKeyNames = headerDictionary.value("FILTER");

    QList<QStringList> headers = {primaryHeader, extHeader};

    bool keyFound = false;
    bool clearFound = false;
    bool darkFound;
    // Loop over headers
    for (auto &header : headers) {
        for (auto &possibleKey : possibleKeyNames) {
            for (auto &card : header) {
                QString keyName = card.split("=")[0].simplified();
                // Loop over list of possible key names to find match
                if (keyName == possibleKey) {
                    QString filterName = card.split("=")[1];
                    filterName.truncate(filterName.lastIndexOf('/'));  // Some filter names contain a slash
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
                            || filterName.contains("open", Qt::CaseInsensitive)) {
                        clearFound = true;
                        continue;
                    }
                    // Skip if filter name suggests that the slot was empty
                    if (filterName.contains("dark", Qt::CaseInsensitive)
                            || filterName.contains("close", Qt::CaseInsensitive)) {
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

    double lst = 100.46 + 0.985647*d + instData.obslong + 15.*ut;

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
    const double RAD = 3.14159265/180.;
    double LSTbegin = lstValue;
    double LSTmiddle = lstValue + 0.5 * exptimeValue;
    double LSTend = lstValue + exptimeValue;
    double lst = lstValue * RAD;
    double hourangle_begin = (LSTbegin/240.) * RAD - crval1 * RAD;
    double hourangle_middle = (LSTmiddle/240.) * RAD - crval1 * RAD;
    double hourangle_end = (LSTend/240.) * RAD - crval1 * RAD;

    // The effective airmass is estimated using the 'mean airmass' estimator in
    // "Some Factors Affecting the Accuracy of Stellar Photometry with CCDs" (P. Stetson, DAO preprint, September 1988)

    double airmass_begin = calcAirmass(hourangle_begin);
    double airmass_middle = calcAirmass(hourangle_middle);
    double airmass_end = calcAirmass(hourangle_end);
    double airmass = (airmass_begin + 4.0*airmass_middle + airmass_end) / 6.0;

    return airmass;
}

double Splitter::calcAirmass(double ahourangle)
{
    const double RAD = 3.14159265/180.;

    double sh = sin(ahourangle);
    double ch = cos(ahourangle);
    double sd = sin(crval2 * RAD);
    double cd = cos(crval2 * RAD);
    double sp = sin(instData.obslat * RAD);
    double cp = cos(instData.obslat * RAD);
    double x = ch*cd*sp - sd*cp;
    double y = sh*cd;
    double z = ch*cd*cp + sd*sp;
    double zn = sqrt(x*x+y*y);
    double zf = zn/z;
    double zd = atan(zf);
    double seczm = 1.0 /(cos(min(1.52, zd))) - 1.0;

    // Convert zenith distance to airmass following
    // "R.H. Hardie in _Astronomical Techniques_ (W.A. Hiltner, ed.) (Chicago: U. Chicago Press), p. 180 (1962)."
    double airmass = 1.0 + seczm * (0.9981833 - seczm * (0.002875 + seczm * 0.008083));

    return airmass;
}
