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

#include "query.h"
#include "../functions.h"
#include "../tools/tools.h"
#include "../myimage/myimage.h"
#include "../processingInternal/data.h"
#include "../tools/cfitsioerrorcodes.h"

#include "wcs.h"
#include "wcshdr.h"

#include <QMetaObject>
#include <QVector>
#include <QStringList>
// #include <QSettings>
#include <QProcess>
#include <QStandardPaths>

Query::Query(int *verbose)
{
    // Initialization
    initEnvironment(thelidir, userdir);

//    QSettings settings("THELI", "PREFERENCES");
//    QString server = settings.value("prefServerComboBox").toString();
//    downloadServer = translateServer(server);

    pythonExecutable = findExecutableName("python");

    // reserve 1MB for the downloaded catalog
    byteArray.reserve(1e6);

    verbosity = verbose;
}

Query::~Query()
{

}

void Query::doBrightStarQueryFromWeb()
{
    initAstromQuery();
    getMedianEpoch();
    buildQuerySyntaxAstrom();
    runCommand(queryCommand);
    processBrightStarCatalog();
}

void Query::doAstromQueryFromWeb()
{
    initAstromQuery();
    getMedianEpoch();
    buildQuerySyntaxAstrom();
    runCommand(queryCommand);
    processAstromCatalog();
}

// Used for image quality, a pure stellar catalog
void Query::doGaiaQuery()
{
    initGaiaQuery();
    getMedianEpoch();
    buildQuerySyntaxGaia();
    runCommand(queryCommand);
    processGaiaCatalog();
}

void Query::doPhotomQueryFromWeb()
{
    initPhotomQuery();
    // getMedianEpoch();
    buildQuerySyntaxPhotom();
    runCommand(queryCommand);
    processPhotomCatalog();
}

void Query::doColorCalibQueryFromWeb()
{
    initColorCalibQuery();
    buildQuerySyntaxColorCalib();
    runCommand(queryCommand);
    processColorCalibCatalog();
}

void Query::getCatalogSearchLocationAstrom()
{
    if (!successProcessing) return;

    QVector<double> crval1 = scienceData->getKeyFromAllImages("CRVAL1");
    QVector<double> crval2 = scienceData->getKeyFromAllImages("CRVAL2");

    if (crval1.isEmpty() || crval2.isEmpty()) {
        emit messageAvailable("Query::getCatalogSearchLocationAstrom(): CRVAL vectors are empty", "error");
        emit critical();
        successProcessing = false;
        return;
    }

    // Use median to calculate field center (avoiding issues with RA=0|360 deg boundary)
    // Do not average central two elements if number of data points is even
    alpha = straightMedian_T(crval1, 0, false);
    delta = straightMedian_T(crval2, 0, false);

    // Convert to string (because we pass this to 'vizquery'
    alpha_string = QString::number(alpha, 'f', 6);
    delta_string = QString::number(delta, 'f', 6);
}

void Query::getCatalogSearchRadiusAstrom()
{
    if (!successProcessing) return;

    QVector<double> corners_crval1 = scienceData->getKeyFromAllImages("CORNERS_CRVAL1");
    QVector<double> corners_crval2 = scienceData->getKeyFromAllImages("CORNERS_CRVAL2");

    if (corners_crval1.isEmpty() || corners_crval2.isEmpty()) {
        emit messageAvailable("Query::getCatalogSearchRadiusAstrom(): Corner vectors are empty", "error");
        emit critical();
        successProcessing = false;
        return;
    }

    // Search radius in DEC in arcmin
    double crval2_min = minVec_T(corners_crval2);
    double crval2_max = maxVec_T(corners_crval2);
    double crval2_radius = 60.*(crval2_max - crval2_min)*0.5;

    // Search radius in RA in arcmin, take into account possible pass over 0|360 deg boundary
    double crval1_min = minVec_T(corners_crval1);
    double crval1_max = maxVec_T(corners_crval1);
    if (crval1_max - crval1_min > 350.) {
        for (auto &ra : corners_crval1) {
            if (ra > 180.) ra -= 360.;
        }
        // Calculate min / max once more
        crval1_min = minVec_T(corners_crval1);
        crval1_max = maxVec_T(corners_crval1);
    }
    double crval1_radius = 60.*(crval1_max - crval1_min)*0.5*cos(delta*3.14159/180.);

    if (!radius_manual.isEmpty()) radius_string = radius_manual;
    else {
        // Include a 10% safety margin in the search radius
        radius = 1.1*sqrt(crval1_radius*crval1_radius + crval2_radius*crval2_radius);

        // safeguarding against a potential threading issue with wcslib:
        if (radius > 5.*scienceData->instData->radius*60) {
            radius = 2.*scienceData->instData->radius*60;
            // TODO: this is probably not necessary anymore because of the wcslock in MyImage::initWCS()
            emit messageAvailable("Truncating the search radius to "+QString::number(radius, 'f', 1) + " arcmin", "warning");
            emit messageAvailable("Do you have different targets collected in the same directory?", "warning");
            qDebug() << "RA corner vertices" << corners_crval1;
            qDebug() << "DEC corner vertices" << corners_crval2;
        }
        radius_string = QString::number(radius, 'f', 3);
    }
}

void Query::getCatalogSearchLocationPhotom()
{
    if (!successProcessing) return;

    // Calculate RA/DEC of image center
    photomImage = new MyImage(photomDir, photomImageName, "", 1, QVector<bool>(), verbosity);
    photomImage->loadHeader();
    naxis1 = photomImage->naxis1;
    naxis2 = photomImage->naxis2;
    radius = 1.1 * sqrt(naxis1*naxis1+naxis2*naxis2) / 2. * photomImage->plateScale / 60.;

    // Convert to string (because we pass this to 'vizquery'
    alpha_string = QString::number(photomImage->alpha_ctr, 'f', 6);
    delta_string = QString::number(photomImage->delta_ctr, 'f', 6);
    radius_string = QString::number(radius, 'f', 3);
}

void Query::getCatalogSearchLocationColorCalib()
{
    if (!successProcessing) return;

    // Calculate RA/DEC of image center
    photomImage->loadHeader();
    naxis1 = photomImage->naxis1;
    naxis2 = photomImage->naxis2;
    radius = 1.1 * sqrt(naxis1*naxis1+naxis2*naxis2) / 2. * photomImage->plateScale / 60.;

    // Convert to string (because we pass this to 'vizquery'
    alpha_string = QString::number(photomImage->alpha_ctr, 'f', 6);
    delta_string = QString::number(photomImage->delta_ctr, 'f', 6);
    radius_string = QString::number(radius, 'f', 3);
}

void Query::getMedianEpoch()
{
    if (!successProcessing) return;

    // Get median observation epoch (for GAIA)
    QVector<double> epochs = scienceData->getKeyFromAllImages("DATE-OBS");
    if (epochs.isEmpty() && refcatName.contains("GAIA")) {
        emit messageAvailable("Query::getMedianEpoch():<br>The DATE-OBS keyword was not found in the FITS headers, but is required for the GAIA reference catalog.<br>You must choose a different reference catalog.", "error");
        emit critical();
        successProcessing = false;
        return;
    }
    epoch = straightMedian_T(epochs);
}

void Query::initAstromQuery()
{
    if (!successProcessing) return;

    // Reformat if necessary
    if (alpha_manual.contains(":")) alpha_manual = hmsToDecimal(alpha_manual);
    if (delta_manual.contains(":")) delta_manual = dmsToDecimal(delta_manual);

    // Get location. Manual or automatic mode
    if (magLimit_string.isEmpty()) magLimit_string = "25";
    if (alpha_manual.isEmpty() || delta_manual.isEmpty()) {
        getCatalogSearchLocationAstrom();
    }
    else {
        alpha_string = alpha_manual;
        delta_string = delta_manual;
    }

    // Get search radius
    getCatalogSearchRadiusAstrom();

    // Make declination explicitly positive (sometimes the cdsclient wants that, or wanted that)
    if (!delta_string.contains('-') && !delta_string.contains('+') ) delta_string.prepend('+');

    // Vizier does not deal correctly with integer coords
    if (!alpha_string.contains(".")) alpha_string.append(".0");
    if (!delta_string.contains(".")) delta_string.append(".0");
}

void Query::initGaiaQuery()
{
    if (!successProcessing) return;

    getCatalogSearchLocationAstrom();
    // search radius is provided explicitly only by the coaddition. Otherwise, we have to determine it
    if (radius_string.isEmpty()) getCatalogSearchRadiusAstrom();

    // Make declination explicitly positive (sometimes the cdsclient wants that, or wanted that)
    if (!delta_string.contains('-') && !delta_string.contains('+') ) delta_string.prepend('+');

    // Vizier does not deal correctly with integer coords
    if (!alpha_string.contains(".")) alpha_string.append(".0");
    if (!delta_string.contains(".")) delta_string.append(".0");
}

void Query::initPhotomQuery()
{
    if (!successProcessing) return;

    getCatalogSearchLocationPhotom();
    // Make declination explicitly positive (sometimes the cdsclient wants that, or wanted that)
    if (!delta_string.contains('-') && !delta_string.contains('+') ) delta_string.prepend('+');
}

void Query::initColorCalibQuery()
{
    if (!successProcessing) return;

    getCatalogSearchLocationColorCalib();
    // Make declination explicitly positive (sometimes the cdsclient wants that, or wanted that)
    if (!delta_string.contains('-') && !delta_string.contains('+') ) delta_string.prepend('+');
}

void Query::buildQuerySyntaxAstrom()
{
    if (!successProcessing) return;

    // Vizier queries
    queryCommand = pythonExecutable + " " + thelidir+"/python/vizquery.py ";
    queryCommand.append("-mime=tsv -out.max=170000 ");        // TODO: More than ~170000 crashes writeAstromScamp(), for unknown reasons
    //        queryCommand.append("-site="+downloadServer+" ");
    queryCommand.append("-c.rm="+radius_string+" ");
    queryCommand.append("-c='"+alpha_string+delta_string+"' ");

    if (refcatName.contains("ASCC")) queryCommand.append("-out=_RAJ -out=_DEJ -source=I/280B -out=Vmag Vmag=0.."+magLimit_string);
    else if (refcatName.contains("TYC"))  queryCommand.append("-out=_RAJ -out=_DEJ -source=TYC/tyc_main -out=Vmag Vmag=0.."+magLimit_string);
    else if (refcatName.contains("2MASS")) queryCommand.append("-out=_RAJ -out=_DEJ -source=2MASS-PSC -out='Jmag,Hmag,Kmag' Hmag=0.."+magLimit_string);
    else if (refcatName.contains("UCAC")) queryCommand.append("-out=RAgaia -out=DEgaia -source=I/340 -out='Gmag,RMag' -out=pmRA -out=pmDE Rmag=0.."+magLimit_string);
    else if (refcatName.contains("GAIA")) queryCommand.append("-out=RA_ICRS -out=DE_ICRS -source=I/345/gaia2 -out=Gmag -out=pmRA -out=pmDE Gmag=0.."+magLimit_string);
    else if (refcatName.contains("SDSS")) queryCommand.append("-out=RA_ICRS -out=DE_ICRS -source=V/147 -out=gmag,rmag,imag rmag=0.."+magLimit_string);
    else if (refcatName.contains("PANSTARRS")) queryCommand.append("-out=_RAJ -out=_DEJ -source=II/349/ps1 -out=gmag,rmag,imag rmag=0.."+magLimit_string);
}

void Query::buildQuerySyntaxGaia()
{
    if (!successProcessing) return;

    // Vizier query for GAIA point sources. Point sources are identified by means of proper motion, in extractRaDecGaia();
    queryCommand = pythonExecutable + " " + thelidir+"/python/vizquery.py ";
    queryCommand.append("-mime=tsv -out.max=1000000 ");
    queryCommand.append("-c.rm="+radius_string+" ");
    queryCommand.append("-c='"+alpha_string+delta_string+"' ");
    queryCommand.append("-out=RA_ICRS -out=DE_ICRS -source=I/345/gaia2 -out=Gmag -out=pmRA -out=pmDE -out=e_pmRA -out=e_pmDE");
}

void Query::buildQuerySyntaxPhotom()
{
    if (!successProcessing) return;

    // Vizier queries

    queryCommand = pythonExecutable + " " + thelidir+"/python/vizquery.py ";
    queryCommand.append("-mime=tsv -out.max=1000000 ");
    //        queryCommand.append("-site="+downloadServer+" ");
    queryCommand.append("-c.rm="+radius_string+" ");
    queryCommand.append("-c='"+alpha_string+delta_string+"' ");

    QString filter1 = filterStringToVizierName(refcatFilter1);
    QString filter2 = filterStringToVizierName(refcatFilter2);

    if (refcatName.contains("2MASS")) queryCommand.append("-out=_RAJ -out=_DEJ -source=2MASS-PSC ");
    else if (refcatName.contains("APASS")) queryCommand.append("-out=_RAJ -out=_DEJ -source=II/336 ");
    else if (refcatName.contains("UKIDSS")) queryCommand.append("-out=RA_ICRS -out=DE_ICRS -source=II/319 ");
    else if (refcatName.contains("SDSS")) queryCommand.append("-out=RA_ICRS -out=DE_ICRS -source=V/147 ");
    else if (refcatName.contains("PANSTARRS")) queryCommand.append("-out=_RAJ -out=_DEJ -source=II/349/ps1 ");

    queryCommand.append("-out=" + filter1 + " -out=" + filter2 + " -out=e_" + filter1 + " -out=e_" + filter2);    //  mags and their errors
}

void Query::buildQuerySyntaxColorCalib()
{
    if (!successProcessing) return;

    // Vizier queries

    queryCommand = pythonExecutable + " " + thelidir+"/python/vizquery.py ";
    queryCommand.append("-mime=tsv -out.max=1000000 ");
    queryCommand.append("-c.rm="+radius_string+" ");
    queryCommand.append("-c='"+alpha_string+delta_string+"' ");

    QString filter1;
    QString filter2;
    QString filter3;
    QString filter4;
    QString filter5;

    if (refcatName == "SDSS") {
        filter1 = filterStringToVizierName("u");
        filter2 = filterStringToVizierName("g");
        filter3 = filterStringToVizierName("r");
        filter4 = filterStringToVizierName("i");
        filter5 = filterStringToVizierName("z");
    }
    if (refcatName == "APASS") {
        filter1 = filterStringToVizierName("B");
        filter2 = filterStringToVizierName("V");
        filter3 = filterStringToVizierName("g");
        filter4 = filterStringToVizierName("r");
        filter5 = filterStringToVizierName("i");
    }
    if (refcatName == "PANSTARRS") {
        filter1 = filterStringToVizierName("g");
        filter2 = filterStringToVizierName("r");
        filter3 = filterStringToVizierName("i");
        filter4 = filterStringToVizierName("z");
        filter5 = "";
    }

    if (refcatName.contains("APASS")) queryCommand.append("-out=_RAJ -out=_DEJ -source=II/336 ");
    else if (refcatName.contains("SDSS")) queryCommand.append("-out=RA_ICRS -out=DE_ICRS -source=V/147 ");
    else if (refcatName.contains("PANSTARRS")) queryCommand.append("-out=_RAJ -out=_DEJ -source=II/349/ps1 ");

    queryCommand.append("-out=" + filter1
                        + " -out=" + filter2
                        + " -out=" + filter3
                        + " -out=" + filter4
                        + " -out=" + filter5);
}

QString Query::filterStringToVizierName(QString filter)
{
    if (!successProcessing) return "";

    if (refcatName.contains("APASS") && filter == "B") return "Bmag";
    if (refcatName.contains("APASS") && filter == "V") return "Vmag";
    if (refcatName.contains("APASS") && filter == "g") return "g\\'mag";
    if (refcatName.contains("APASS") && filter == "r") return "r\\'mag";
    if (refcatName.contains("APASS") && filter == "i") return "i\\'mag";

    if (refcatName.contains("SDSS") && filter == "u") return "umag";
    if (refcatName.contains("SDSS") && filter == "g") return "gmag";
    if (refcatName.contains("SDSS") && filter == "r") return "rmag";
    if (refcatName.contains("SDSS") && filter == "i") return "imag";
    if (refcatName.contains("SDSS") && filter == "z") return "zmag";

    if (refcatName.contains("PANSTARRS") && filter == "g") return "gmag";
    if (refcatName.contains("PANSTARRS") && filter == "r") return "rmag";
    if (refcatName.contains("PANSTARRS") && filter == "i") return "imag";
    if (refcatName.contains("PANSTARRS") && filter == "z") return "zmag";
    if (refcatName.contains("PANSTARRS") && filter == "y") return "ymag";

    if (refcatName.contains("2MASS") && filter == "J") return "Jmag";
    if (refcatName.contains("2MASS") && filter == "H") return "Hmag";
    if (refcatName.contains("2MASS") && filter == "Ks") return "Kmag";

    if (refcatName.contains("VHS") && filter == "Y") return "Ymag4";
    if (refcatName.contains("VHS") && filter == "J") return "Jmag4";
    if (refcatName.contains("VHS") && filter == "H") return "Hmag4";
    if (refcatName.contains("VHS") && filter == "Ks") return "Ksmag4";

    if (refcatName.contains("UKIDSS") && filter == "") return "Ymag";
    if (refcatName.contains("UKIDSS") && filter == "") return "Jmag1";
    if (refcatName.contains("UKIDSS") && filter == "") return "Hmag";
    if (refcatName.contains("UKIDSS") && filter == "") return "Kmag";

    return "";
}

QString Query::resolveTarget(QString target)
{
    if (!successProcessing) return "Unresolved";

    queryCommand = pythonExecutable + " " + thelidir+"/python/resolvetargets.py "+target;

    runCommand(queryCommand);

    QTextStream stream(&byteArray, QIODevice::ReadOnly);

    QString line;
    while (stream.readLineInto(&line)) {
        line = line.simplified();
        if (line.contains("Traceback")) return "Unresolved";
        else {
            // In case python2 is used instead of python3, we have to remove parentheses and commas from the output
            line = line.remove("(");
            line = line.remove(")");
            line = line.replace(",", " ");
            line = line.simplified();
            QStringList coordList = line.split(' ');
            targetAlpha = coordList[0];
            targetDelta = coordList[1];
            // Convert to HHMMSS and DDMMSS
            if (!targetAlpha.contains(":")) targetAlpha = decimalToHms(targetAlpha.toDouble());
            if (!targetDelta.contains(":")) targetDelta = decimalToDms(targetDelta.toDouble());
            return "Resolved";
        }
    }
    return "Unresolved";
}

/*
// Extract 'value' in a line <tag>value</tag>
QString Query::parseXML(QString &line, const QString &tag)
{
    line.remove("<"+tag+">");
    line.remove("</"+tag+">");
    return line.simplified();
}

// Extract 'value' in a line <tag>value</tag>
void Query::parseXML2(QString &line, const QString &tag, QString &val1, QString &val2)
{
    line.remove("<"+tag+">");
    line.remove("</"+tag+">");
    QStringList list = line.simplified().split(" ");
    val1 = list[0];
    val2 = list[1];
}
*/

void Query::clearAstrom()
{
    raMotions.clear();
    deMotions.clear();
    numSources = 0;

    ra_out.clear();
    de_out.clear();
    mag1_out.clear();
}

void Query::clearGaia()
{
    raMotions.clear();
    deMotions.clear();
    raMotions_err.clear();
    deMotions_err.clear();
    numSources = 0;

    ra_out.clear();
    de_out.clear();
    mag1_out.clear();
}

void Query::clearPhotom()
{
    numSources = 0;

    ra_out.clear();
    de_out.clear();
    mag1_out.clear();
    mag2_out.clear();
    mag1err_out.clear();
    mag2err_out.clear();
}

void Query::clearColorCalib()
{
    numSources = 0;

    ra_out.clear();
    de_out.clear();
    mag1_out.clear();
    mag2_out.clear();
    mag3_out.clear();
    mag4_out.clear();
    mag5_out.clear();
}

void Query::processAstromCatalog()
{
    if (!successProcessing) return;

    QTextStream stream(&byteArray, QIODevice::ReadOnly);

    clearAstrom();

    // The iView catalog (ASCII)
    QString outpath = mainDirName+"/"+scienceData->subDirName+"/cat/refcat/";
    mkAbsDir(outpath);

    QFile outcat_iview(outpath+"/theli_mystd.iview");
    QTextStream stream_iview(&outcat_iview);
    if( !outcat_iview.open(QIODevice::WriteOnly)) {
        emit messageAvailable("Query::processAstromCatalog(): ERROR writing "+outpath+outcat_iview.fileName()+" : "+outcat_iview.errorString(), "error");
        emit critical();
        return;
    }

    // Write iView catalog, prepare scamp and anet
    QString line;
    long i=0;
    while (stream.readLineInto(&line)) {
        if (line.contains("database is not currently reachable")) {
            successfulDataBaseAccess = false;
            emit messageAvailable("Query:: The CDS database is currently not reachable", "error");
            emit critical();
            break;
        }
        if (line.contains("#")
                || line.isEmpty()
                || line.contains("deg")
                || line.contains("RA")
                || line.contains("Content-Type")
                || line.contains("Content-Disposition")
                || line.contains("DocumentRef")
                || line.contains("---")) continue;
        QString result = extractRaDecMagAstrom(line);  // also prepares scamp and anet
        if (!result.isEmpty()) {
            stream_iview << result << "\n";
            ++numSources;
        }
        ++i;
    }
    outcat_iview.close();
    outcat_iview.setPermissions(QFile::ReadUser | QFile::WriteUser);

    measureBulkMotion();       // display mean bulk motion in that field
    pushNumberOfSources();     // display number of refcat sources

    writeAstromScamp();
    writeAstromANET();         // Can be lengthy if 100000 sources or more are retrieved.

    dumpRefcatID();            // writes refcat/.refcatID, used to check whether the catalog needs to be recreated
}

void Query::processGaiaCatalog()
{
    if (!successProcessing) return;

    QTextStream stream(&byteArray, QIODevice::ReadOnly);

    clearGaia();

    // The iView catalog (ASCII)
    QString outpath = mainDirName+"/"+scienceData->subDirName+"/cat/refcat/";
    mkAbsDir(outpath);

    QFile outcat_iview(outpath+"/theli_pointsources.iview");
    QTextStream stream_iview(&outcat_iview);
    if( !outcat_iview.open(QIODevice::WriteOnly)) {
        emit messageAvailable("Query::processGaiaCatalog(): "+outpath+outcat_iview.fileName()+" : "+outcat_iview.errorString(), "error");
        emit critical();
        return;
    }

    // Write iView catalog
    QString line;
    long i=0;
    while (stream.readLineInto(&line)) {
        if (line.contains("database is not currently reachable")) {
            successfulDataBaseAccess = false;
            emit messageAvailable("Query:: The CDS database is currently not reachable", "error");
            emit critical();
            break;
        }
        if (line.contains("#")
                || line.isEmpty()
                || line.contains("deg")
                || line.contains("RA")
                || line.contains("Content-Type")
                || line.contains("Content-Disposition")
                || line.contains("DocumentRef")
                || line.contains("---")) continue;
        QString result = extractRaDecGaia(line);         // includes point source filtering
        if (!result.isEmpty()) {
            stream_iview << result << "\n";
            ++numSources;
        }
        ++i;
    }
    outcat_iview.close();
    outcat_iview.setPermissions(QFile::ReadUser | QFile::WriteUser);

    //    pushNumberOfSources();     // display number of refcat sources

    dumpRefcatID();            // writes refcat/.refcatID, used to check whether the catalog needs to be recreated
}

// used externally when modeling the background.
// difference: does not dump catalogs to drive
void Query::processBrightStarCatalog()
{
    if (!successProcessing) return;

    QTextStream stream(&byteArray, QIODevice::ReadOnly);

    clearAstrom();

    // Process the stream
    QString line;
    long i=0;
    while (stream.readLineInto(&line)) {
        if (line.contains("database is not currently reachable")) {
            successfulDataBaseAccess = false;
            emit messageAvailable("Query:: The CDS database is currently not reachable", "error");
            emit critical();
            break;
        }
        if (line.contains("#")
                || line.isEmpty()
                || line.contains("deg")
                || line.contains("RA")
                || line.contains("Content-Type")
                || line.contains("Content-Disposition")
                || line.contains("DocumentRef")
                || line.contains("---")) continue;
        QString result = extractRaDecMagAstrom(line);
        if (!result.isEmpty()) ++numSources;
        ++i;
    }
}

void Query::processPhotomCatalog()
{
    if (!successProcessing) return;

    QTextStream stream(&byteArray, QIODevice::ReadOnly);

    clearPhotom();

    // The iView catalog (ASCII)
    QString outpath = photomDir+"/";
    QDir outdir(outpath);
    if (!outdir.exists()) outdir.mkpath(outpath);
    QFile outcat_iview(outpath+"/refcatPhotom.iview");
    QTextStream stream_iview(&outcat_iview);
    if( !outcat_iview.open(QIODevice::WriteOnly)) {
        emit messageAvailable("Query::processPhotomCatalog(): ERROR writing "+outpath+outcat_iview.fileName()+" : "+outcat_iview.errorString(), "error");
        emit critical();
        return;
    }

    // Write iView catalog
    QString line;
    long i=0;
    while (stream.readLineInto(&line)) {
        if (line.contains("database is not currently reachable")) {
            successfulDataBaseAccess = false;
            emit messageAvailable("Query:: The CDS database is currently not reachable", "error");
            emit critical();
            break;
        }
        if (line.contains("#")
                || line.isEmpty()
                || line.contains("deg")
                || line.contains("RA")
                || line.contains("Content-Type")
                || line.contains("Content-Disposition")
                || line.contains("DocumentRef")
                || line.contains("---")) continue;
        QString result = extractRaDecMagPhotom(line);
        if (!result.isEmpty()) {
            stream_iview << result << "\n";
            ++numSources;
        }
        ++i;
    }
    outcat_iview.close();
    outcat_iview.setPermissions(QFile::ReadUser | QFile::WriteUser);
}

void Query::processColorCalibCatalog()
{
    if (!successProcessing) return;

    QTextStream stream(&byteArray, QIODevice::ReadOnly);

    clearColorCalib();

    // The iView catalog (ASCII)
    QString outpath = photomDir+"/";
    QDir outdir(outpath);
    if (!outdir.exists()) outdir.mkpath(outpath);
    QFile outcat_iview(outpath+"/PHOTCAT_calibration/PHOTCAT_sources_"+refcatName+".iview");
    QTextStream stream_iview(&outcat_iview);
    if( !outcat_iview.open(QIODevice::WriteOnly)) {
        emit messageAvailable("Query::processPhotomCatalog(): ERROR writing "+outpath+outcat_iview.fileName()+" : "+outcat_iview.errorString(), "error");
        emit critical();
        return;
    }

    // Write iView catalog
    QString line;
    long i=0;
    while (stream.readLineInto(&line)) {
        if (line.contains("database is not currently reachable")) {
            successfulDataBaseAccess = false;
            emit messageAvailable("Query:: The CDS database is currently not reachable", "error");
            emit critical();
            break;
        }
        if (line.contains("#")
                || line.isEmpty()
                || line.contains("deg")
                || line.contains("RA")
                || line.contains("Content-Type")
                || line.contains("Content-Disposition")
                || line.contains("DocumentRef")
                || line.contains("---")) continue;
        QString result = extractRaDecMagColorCalib(line);
        if (!result.isEmpty()) {
            stream_iview << result << "\n";
            ++numSources;
        }
        ++i;
    }
    outcat_iview.close();
    outcat_iview.setPermissions(QFile::ReadUser | QFile::WriteUser);
}

void Query::pushNumberOfSources()
{
    QString messageString = "";
    if (numSources > 0) {
        if (!fromImage) {
            messageString = QString::number(numSources) + " " + refcatName + " reference sources retrieved at this location:"
                    //                    + "<br>RA  = " + QString::number(alpha, 'f', 5)
                    //                    + "<br>DEC = " + QString::number(delta, 'f', 5)
                    + "<br>RA  = " + alpha_string
                    + "<br>DEC = " + delta_string
                    + "<br>radius = " + radius_string + "'";
            emit messageAvailable(messageString, "ignore");
        }
        else {
            messageString = QString::number(numSources) + " " + refcatName + " reference sources detected in the image.";
            emit messageAvailable(messageString, "ignore");
        }
    }
    else {
        if (!successfulDataBaseAccess) {
            emit messageAvailable("Query:: The CDS database is currently not reachable", "stop");
            emit critical();
            successfulDataBaseAccess = true;
        }
        if (!suppressCatalogWarning) {
            messageString = "WARNING: No reference sources retrieved! <br>"
                            "-- try a different catalog <br>"
                            "-- check your internet connection <br>"
                            "-- adjust mag limit and radius";
            emit messageAvailable(messageString, "stop");
            emit critical();
        }
    }
}

void Query::identifySolarTypeStars()
{
    if (!successProcessing) return;

    if (refcatName == "SDSS") {
        c1min = sunUG;
        c1max = sunUG;
        c2min = sunGR;
        c2max = sunGR;
        c3min = sunRI;
        c3max = sunRI;
        c1tol = sunUGtol;
        c2tol = sunGRtol;
        c3tol = sunRItol;
    }
    if (refcatName == "APASS") {
        c1min = sunBV;
        c1max = sunBV;
        c2min = sunGR;
        c2max = sunGR;
        c3min = sunRI;
        c3max = sunRI;
        c1tol = sunBVtol;
        c2tol = sunGRtol;
        c3tol = sunRItol;
    }
    if (refcatName == "PANSTARRS") {
        c1min = sunGR;
        c1max = sunGR;
        c2min = sunRI;
        c2max = sunRI;
        c3min = sunIZ;
        c3max = sunIZ;
        c1tol = sunGRtol;
        c2tol = sunRItol;
        c3tol = sunIZtol;
    }

    QVector<float> tolerances = {1.0, 1.5, 2.0};

    // We use the first three colors, only
    // SDSS: u-g, g-r, r-i
    // APASS: B-V, g-r, r-i
    // PANSTARRS: g-r, r-i, i-z
    for (auto &tol : tolerances) {
        G2type.fill(false, mag1_out.length());
        int nmatch = 0;
        float c1min0 = c1min - tol*c1tol;
        float c1max0 = c1max + tol*c1tol;
        float c2min0 = c2min - tol*c2tol;
        float c2max0 = c2max + tol*c2tol;
        float c3min0 = c3min - tol*c3tol;
        float c3max0 = c3max + tol*c3tol;

        for (long k=0; k<mag1_out.length(); ++k) {
            float col1 = 0.;
            float col2 = 0.;
            float col3 = 0.;
            if (refcatName == "PANSTARRS" || refcatName == "SDSS") {
                col1 = mag1_out[k] - mag2_out[k];
                col2 = mag2_out[k] - mag3_out[k];
                col3 = mag3_out[k] - mag4_out[k];
            }
            else if (refcatName == "APASS") {
                col1 = mag1_out[k] - mag2_out[k];    // "B-V"
                col2 = mag3_out[k] - mag4_out[k];    // "g-r"
                col3 = mag4_out[k] - mag5_out[k];    // "r-i"
            }
            if (col1 >= c1min0 && col1 <= c1max0
                    && col2 >= c2min0 && col2 <= c2max0
                    && col3 >= c3min0 && col3 <= c3max0) {
                G2type[k] = true;
                ++nmatch;
            }
        }
        if (nmatch > 0) {
            numG2sources = nmatch;
            break;
        }
    }
}

void Query::measureBulkMotion()
{
    if (!successProcessing) return;

    if (!refcatName.contains("GAIA")
            && !refcatName.contains("UCAC5")) return;

    raBulkMotion = straightMedian_T(raMotions);
    deBulkMotion = straightMedian_T(deMotions);
    if (raMotions.length() > 1) {
        raErrBulkMotion = rmsMask_T(raMotions) / sqrt(raMotions.length());
        deErrBulkMotion = rmsMask_T(deMotions) / sqrt(deMotions.length());
    }
    else {
        raErrBulkMotion = 0.;
        deErrBulkMotion = 0.;
    }

    QString pmRA_GUI = QString::number(raBulkMotion, 'f', 3) + " +/- " + QString::number(raErrBulkMotion, 'f', 3);
    QString pmDE_GUI = QString::number(deBulkMotion, 'f', 3) + " +/- " + QString::number(deErrBulkMotion, 'f', 3);

    emit bulkMotionObtained(pmRA_GUI, pmDE_GUI);
}


QString Query::extractRaDecMagAstrom(QString &line)
{
    if (!successProcessing) return "";

    // 'line' contains RA, DEC, and an arbitrary number of magnitudes (which we average)
    // Fields are separated by semi-colons
    QString result = "";
    //    line = line.simplified();
    QStringList list = line.split('\t');
    int length = list.length();
    if (refcatName.contains("GAIA") && length < 5) return result;
    if (!refcatName.contains("GAIA") && length < 3) return result;

    QString ra = list[0].simplified();
    QString dec = list[1].simplified();
    if (ra.isEmpty()) return "";
    QVector<float> magnitudes;
    magLimit = magLimit_string.toFloat();

    // without proper motions
    if (!refcatName.contains("GAIA")
            && !refcatName.contains("UCAC")) {
        for (int i=2; i<length; ++i) {
            QString mag = list[i].simplified();
            if (!mag.isEmpty()) magnitudes << mag.toFloat();
        }
        float meanMag = meanMask(magnitudes);
        result = ra+" "+dec+" "+QString::number(meanMag, 'f', 3);
        ra_out.append(ra.toDouble());
        de_out.append(dec.toDouble());
        mag1_out.append(meanMag);
    }

    // with proper motions
    else {
        for (int i=2; i<length-2; ++i) {
            QString mag = list[i].simplified();
            if (!mag.isEmpty()) magnitudes << mag.toFloat();
        }
        float meanMag = meanMask(magnitudes);

        double ra_obj = ra.toDouble();
        double dec_obj = dec.toDouble();
        QString pmRA_string = list[length-2].simplified();
        QString pmDE_string = list[length-1].simplified();
        float pmRA = pmRA_string.toFloat();
        float pmDE = pmDE_string.toFloat();
        raMotions << pmRA;
        deMotions << pmDE;
        float pm = sqrt(pmRA*pmRA+pmDE*pmDE);
        if (!maxProperMotion_string.isEmpty()) maxProperMotion = maxProperMotion_string.toFloat();
        // Correct coordinates for proper motion.
        double pmRA_rescaled = pmRA/1000./3600./cos(dec_obj*3.14159265/180.);
        double pmDE_rescaled = pmDE/1000./3600.;
        ra_obj = ra_obj + pmRA_rescaled * (epoch - epochReference);
        dec_obj = dec_obj + pmDE_rescaled * (epoch - epochReference);
        ra = QString::number(ra_obj, 'f', 9);
        dec = QString::number(dec_obj, 'f', 9);

        // keep the reference source if it is brighter than the limiting magnitude, and its proper motion is less than the max allowed proper motion
        if (pm < maxProperMotion) {
            result = ra+" "+dec+" "+QString::number(meanMag, 'f', 3);
            ra_out.append(ra_obj);
            de_out.append(dec_obj);
            mag1_out.append(meanMag);
        }
    }

    return result;
}

QString Query::extractRaDecGaia(QString &line)
{
    if (!successProcessing) return "";

    // 'line' contains RA, DEC, pmRA, pmDE, e_pmRA, e_pmDE
    // Fields are separated by semi-colons
    QString result = "";
    QStringList list = line.split('\t');
    int length = list.length();
    // Need RA, DEC, pmRA, pmDE, e_pmRA, e_pmDE
    if (length < 6) return "";

    QString ra = list[0].simplified();
    QString dec = list[1].simplified();
    QString pmRA_string = list[2].simplified();
    QString pmDE_string = list[3].simplified();
    QString e_pmRA_string = list[4].simplified();
    QString e_pmDE_string = list[5].simplified();

    if (ra.isEmpty()) return "";

    double ra_obj = ra.toDouble();
    double dec_obj = dec.toDouble();
    float pmRA = pmRA_string.toFloat();
    float pmDE = pmDE_string.toFloat();
    float e_pmRA = e_pmRA_string.toFloat();
    float e_pmDE = e_pmDE_string.toFloat();
    raMotions << pmRA;
    deMotions << pmDE;
    raMotions_err << e_pmRA;
    deMotions_err << e_pmDE;
    float pm = sqrt(pmRA*pmRA+pmDE*pmDE);
    float e_pm = 1./pm * sqrt(pow(pmRA*e_pmRA,2) + pow(pmDE*e_pmDE,2));   // propagation of uncertainty
    float sn;
    if (e_pm <= 0.) sn = 0.;
    else sn = pm / e_pm;
    // Minimum SN for the proper motion detection: 95% confidence, and therefore it must be a point source (stars move, distant galaxies don't)
    if (sn < 2.) return "";

    // Correct coordinates for proper motion.
    double pmRA_rescaled = pmRA/1000./3600./cos(dec_obj*3.14159265/180.);
    double pmDE_rescaled = pmDE/1000./3600.;
    ra_obj = ra_obj + pmRA_rescaled * (epoch - epochReference);
    dec_obj = dec_obj + pmDE_rescaled * (epoch - epochReference);
    ra = QString::number(ra_obj, 'f', 9);
    dec = QString::number(dec_obj, 'f', 9);

    result = ra+" "+dec;
    ra_out.append(ra_obj);
    de_out.append(dec_obj);

    return result;
}

QString Query::extractRaDecMagPhotom(QString &line)
{
    if (!successProcessing) return "";

    // 'line' contains RA, DEC, and 2 magnitudes, and their errors
    // Fields are separated by semi-colons
    QString result = "";
    //    line = line.simplified();
    QStringList list = line.split('\t');
    int length = list.length();
    if (length != 6) return result;

    QString ra = list[0].simplified();
    QString dec = list[1].simplified();
    QString mag1 = list[2].simplified();
    QString mag2 = list[3].simplified();
    QString mag1err = list[4].simplified();
    QString mag2err = list[5].simplified();
    if (ra.isEmpty() || dec.isEmpty()
            || mag1.isEmpty() || mag2.isEmpty()
            || mag1err.isEmpty() || mag2err.isEmpty()) return result;

    result = ra+" "+dec+" "+mag1+" "+mag2; // we do not propagate the errors into the output, just keep them in the variables
    ra_out.append(ra.toDouble());
    de_out.append(dec.toDouble());
    mag1_out.append(mag1.toFloat());
    mag2_out.append(mag2.toFloat());
    mag1err_out.append(mag1err.toFloat());
    mag2err_out.append(mag2err.toFloat());

    return result;
}

QString Query::extractRaDecMagColorCalib(QString &line)
{
    if (!successProcessing) return "";

    // 'line' contains RA, DEC, and 4-5 magnitudes
    // Fields are separated by semi-colons
    QString result = "";
    QStringList list = line.split('\t');
    int length = list.length();
    if (length < 5) return result;

    QString ra = list[0].simplified();
    QString dec = list[1].simplified();
    QString mag1 = list[2].simplified();
    QString mag2 = list[3].simplified();
    QString mag3 = list[4].simplified();
    QString mag4 = list[5].simplified();
    QString mag5;
    if (refcatName == "PANSTARRS") mag5 = mag1;
    else {
        if (list.length() == 7) mag5 = list[6].simplified();
        else {
            emit messageAvailable("Query::extractRaDecMagColorCalib(): Unexpected format: " + line+"<br>Ignoring PANSTARRS y-band magnitude", "warning");
            mag5 = mag1;
        }
    }
    if (ra.isEmpty() || dec.isEmpty()
            || mag1.isEmpty() || mag2.isEmpty()
            || mag3.isEmpty() || mag4.isEmpty()
            || mag5.isEmpty()) return result;

    result = ra+" "+dec+" "+mag1+" "+mag2+" "+mag3+" "+mag4+" "+mag5; // we do not propagate the errors into the output, just keep them in the variables
    ra_out.append(ra.toDouble());
    de_out.append(dec.toDouble());
    mag1_out.append(mag1.toFloat());
    mag2_out.append(mag2.toFloat());
    mag3_out.append(mag3.toFloat());
    mag4_out.append(mag4.toFloat());
    mag5_out.append(mag5.toFloat());

    return result;
}

// Retrieves the multi-line output of a query to one of the reference catalog servers
void Query::runCommand(QString command)
{
    if (!successProcessing) return;

    if (*verbosity > 1) {
        if (command.contains("vizquery")) emit messageAvailable("vizquery command: <br>"+queryCommand, "ignore");
    }

    byteArray.clear();
    QProcess process;
    process.start("/bin/sh -c \""+command+"\"");
    process.waitForFinished(-1);
    byteArray = process.readAllStandardOutput();
}

/*
// needed if the data isn't in memory yet (e.g. immediately after starting the main GUI)
void Query::provideHeaderInfo()
{
    for (int chip=0; chip<scienceData->instData->numChips; ++chip) {
        for (auto &it : scienceData->myImageList[chip]) {
            it->loadHeader();
        }
    }
}
*/

void Query::writeAstromScamp()
{
    if (!successProcessing) return;

    // Don't write an empty catalog (so that tasks checking for failure (non-existence of the catalog) can succeed)
    if (ra_out.length() == 0) return;

    char xworld[100] = "X_WORLD";
    char yworld[100] = "Y_WORLD";
    char mag[100] = "MAG";
    char magerr[100] = "MAGERR";
    char erra[100] = "ERRA_WORLD";
    char errb[100] = "ERRB_WORLD";
    char errt[100] = "ERRTHETA_WORLD";    // scamp does not complain if this key is absent
    char flags[100] = "FLAGS";
    char obsdate[100] = "OBSDATE";
    //    char field[100] = "FIELD_POS";
    char *ttype[9] = {xworld, yworld, mag, magerr, erra, errb, errt, flags, obsdate};
    //    char *ttype[8] = {"X_WORLD", "Y_WORLD", "MAG", "MAGERR", "ERRA_WORLD", "ERRB_WORLD", "ERRTHETA_WORLD", "FIELD_POS"};   // invalid in C++
    char tf1[10] = "1D";
    char tf2[10] = "1D";
    char tf3[10] = "1E";
    char tf4[10] = "1E";
    char tf5[10] = "1E";
    char tf6[10] = "1E";
    char tf7[10] = "1E";
    char tf8[10] = "1I";
    char tf9[10] = "1E";
    //    char tf10[10] = "1I";
    char *tform[9] = {tf1, tf2, tf3, tf4, tf5, tf6, tf7, tf8, tf9};

    // Init numSources if called from outside Query
    if (numSources == 0) numSources = ra_out.length();

    double ra_arr[numSources];
    double de_arr[numSources];
    float mag_arr[numSources];
    float magErr_arr[numSources];
    float aErr_arr[numSources];
    float bErr_arr[numSources];
    float thetaErr_arr[numSources];
    int flags_arr[numSources];
    float obsdate_arr[numSources];
    //    short fieldpos[numSources];

    float dateobs = 2000.0; // dummy, for most catalogs
    if (refcatName.contains("GAIA")) dateobs = 2015.5;

    for (long i=0; i<numSources; ++i) {
        ra_arr[i] = ra_out[i];
        de_arr[i] = de_out[i];
        mag_arr[i] = mag1_out[i];
        magErr_arr[i] = 0.1;
        aErr_arr[i] = 0.00003;
        bErr_arr[i] = 0.00003;
        thetaErr_arr[i] = 0.1;
        flags_arr[i] = 0;
        obsdate_arr[i] = dateobs;
        //        fieldpos[i] = 1;
    }

    int status = 0;
    fitsfile *fptr;
    long firstrow  = 1;
    long firstelem = 1;
    int tfields = 9;
    long nrows = numSources;

    QString outpath = mainDirName+"/"+scienceData->subDirName+"/cat/refcat/";
    mkAbsDir(outpath);

    QString filename = outpath + "/theli_mystd.scamp";
    filename = "!"+filename;
    fits_create_file(&fptr, filename.toUtf8().data(), &status);
    fits_create_tbl(fptr, BINARY_TBL, nrows, tfields, ttype, tform, nullptr, "LDAC_OBJECTS", &status);
    fits_write_col(fptr, TDOUBLE, 1, firstrow, firstelem, nrows, ra_arr, &status);
    fits_write_col(fptr, TDOUBLE, 2, firstrow, firstelem, nrows, de_arr, &status);
    fits_write_col(fptr, TFLOAT, 3, firstrow, firstelem, nrows, mag_arr, &status);
    fits_write_col(fptr, TFLOAT, 4, firstrow, firstelem, nrows, magErr_arr, &status);
    fits_write_col(fptr, TFLOAT, 5, firstrow, firstelem, nrows, aErr_arr, &status);
    fits_write_col(fptr, TFLOAT, 6, firstrow, firstelem, nrows, bErr_arr, &status);
    fits_write_col(fptr, TFLOAT, 7, firstrow, firstelem, nrows, thetaErr_arr, &status);
    fits_write_col(fptr, TSHORT, 8, firstrow, firstelem, nrows, flags_arr, &status);
    fits_write_col(fptr, TFLOAT, 9, firstrow, firstelem, nrows, obsdate_arr, &status);
    //    fits_write_col(fptr, TSHORT, 8, firstrow, firstelem, nrows, fieldpos, &status);
    fits_close_file(fptr, &status);

    printCfitsioError("writeAstromScamp()", status);

    QFile file(filename);
    file.setPermissions(QFile::ReadUser | QFile::WriteUser);
}

void Query::writeAstromANET()
{
    if (!successProcessing) return;

    // STEP 1: write a FITS table

    char xworld[100] = "RA";
    char yworld[100] = "DEC";
    char mag[100] = "MAG";
    char *ttype[3] = {xworld, yworld, mag};
    char tf1[10] = "1D";
    char tf2[10] = "1D";
    char tf3[10] = "1E";
    char *tform[3] = {tf1, tf2, tf3};

    // Init numSources if called from outside Query
    if (numSources == 0) numSources = ra_out.length();

    double ra_arr[numSources];
    double de_arr[numSources];
    float mag_arr[numSources];
    for (long i=0; i<numSources; ++i) {
        ra_arr[i] = ra_out[i];
        de_arr[i] = de_out[i];
        mag_arr[i] = mag1_out[i];
    }

    int status = 0;
    fitsfile *fptr;
    long firstrow  = 1;
    long firstelem = 1;
    int tfields = 3;
    long nrows = numSources;

    if (numSources > 50000) {
        QString messageString = "NOTE: More than 50000 sources were retrieved.\n"
                                "The astrometry.net index is not being built as it is very time consuming.\n"
                                "If you want to use astrometry.net, reduce the catalog size by imposing\n"
                                "a (tighter) magnitude limit.";
        emit messageAvailable(messageString, "warning");
        return;
    }

    QString outpath = mainDirName+"/"+scienceData->subDirName+"/cat/refcat/";
    mkAbsDir(outpath);

    QString filename1 = outpath+"/theli_mystd_anet.cat";
    filename1 = "!"+filename1;
    fits_create_file(&fptr, filename1.toUtf8().data(), &status);
    fits_create_tbl(fptr, BINARY_TBL, nrows, tfields, ttype, tform, nullptr, "OBJECTS", &status);
    fits_write_col(fptr, TDOUBLE, 1, firstrow, firstelem, nrows, ra_arr, &status);
    fits_write_col(fptr, TDOUBLE, 2, firstrow, firstelem, nrows, de_arr, &status);
    fits_write_col(fptr, TFLOAT, 3, firstrow, firstelem, nrows, mag_arr, &status);
    fits_close_file(fptr, &status);

    printCfitsioError("writeAstromANET()", status);

    // STEP 2: build the anet index

    // The diameter of one chip in arcmin
    float diameter = sqrt(naxis1*naxis1+naxis2*naxis2)*pixscale/60.;

    // The anet scale number
    float x = 2./log(2.)*log(diameter/6.);
    int scaleNumber;
    if (x-int(x) < 0.5) scaleNumber = int(x);
    else scaleNumber = int(x) + 1;
    if (scaleNumber<0) scaleNumber = 0;
    if (scaleNumber>19) scaleNumber = 19;

    filename1 = outpath+"/theli_mystd_anet.cat"; // without the exclamation mark
    QString filename2 = outpath + "/theli_mystd.index";
    QString buildIndexCommand = "build-astrometry-index ";
    QString anetCheck = QStandardPaths::findExecutable("build-astrometry-index");
    if (anetCheck.isEmpty()) return;

    buildIndexCommand.append("-i "+filename1+" ");
    buildIndexCommand.append("-o "+filename2+" ");
    buildIndexCommand.append("-E -M -S MAG -j 0.1 -I 1 ");
    buildIndexCommand.append("-t "+outpath+" ");
    buildIndexCommand.append("-P "+QString::number(scaleNumber));

    runCommand(buildIndexCommand);
    QFile file(filename2);
    file.setPermissions(QFile::ReadUser | QFile::WriteUser);
    QFile file1(filename1);
    file1.remove();
}

void Query::writeAstromIview()
{
    if (!successProcessing) return;

    // The iView catalog (ASCII)
    QString outpath = mainDirName+"/"+scienceData->subDirName+"/cat/refcat/";
    mkAbsDir(outpath);

    QFile outcat_iview(outpath+"/theli_mystd.iview");
    QTextStream stream_iview(&outcat_iview);
    if( !outcat_iview.open(QIODevice::WriteOnly)) {
        emit messageAvailable("Query::writeRefCatxxx(): ERROR writing "+outpath+outcat_iview.fileName()+" : "+outcat_iview.errorString(), "error");
        emit critical();
        return;
    }

    // Write iView catalog
    long dim = ra_out.length();
    stream_iview.setRealNumberPrecision(9);
    for (long i=0; i<dim; ++i) {
        stream_iview << QString::number(ra_out[i], 'f', 9) << " "
                     << QString::number(de_out[i], 'f', 9) << " "
                     << QString::number(mag1_out[i], 'f', 3) << "\n";
    }
    outcat_iview.close();
    outcat_iview.setPermissions(QFile::ReadUser | QFile::WriteUser);
}

void Query::dumpRefcatID()
{
    if (!successProcessing) return;

    QString outpath = mainDirName+"/"+scienceData->subDirName+"/cat/refcat/";
    mkAbsDir(outpath);

    QFile file(outpath+"/.refcatID");
    QTextStream stream(&file);
    if( !file.open(QIODevice::WriteOnly)) {
        emit messageAvailable("Query::dumpRefcatID(): ERROR writing "+outpath+file.fileName()+" : "+file.errorString(), "error");
        emit critical();
        return;
    }
    if (!refcatName.contains("GAIA")) {
        stream << refcatName+"_"+alpha_manual+"_"+delta_manual+"_"+magLimit_string+"_"+radius_manual << "\n";
    }
    else {
        stream << refcatName+"_"+alpha_manual+"_"+delta_manual+"_"+magLimit_string+"_"+radius_manual+"_"+maxProperMotion_string << "\n";
    }
    file.close();
}

void Query::printCfitsioError(QString funcName, int status)
{
    if (status) {
        CfitsioErrorCodes *errorCodes = new CfitsioErrorCodes(this);
        emit messageAvailable("Query::"+funcName+":<br>" + errorCodes->errorKeyMap.value(status), "error");
        emit critical();
        successProcessing = false;
    }
}
