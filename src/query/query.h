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

#ifndef QUERY_H
#define QUERY_H

#include "../processingInternal/data.h"
#include "../myimage/myimage.h"

#include "fitsio2.h"

#include <omp.h>
#include <QString>
#include <QTextStream>
#include <QByteArray>
#include <QObject>
#include <QMap>

class Data;         // circular include directives triggered by myworker

class Query : public QObject
{
    Q_OBJECT

public:
    explicit Query(int *verbose);
    ~Query();

    Data *scienceData = nullptr;

    MyImage *photomImage = nullptr;
    QString photomDir = "";
    QString photomImageName = "";

    bool suppressCatalogWarning = false;

    int numChips = 1;

    QString mainDirName;
    QString refcatName = "";
    QString alpha_manual = "";
    QString delta_manual = "";
    QString radius_manual = "";
    QString magLimit_string = "";
    QString maxProperMotion_string = "";
    QString targetAlpha = "";
    QString targetDelta = "";
    QString refcatFilter1 = "";
    QString refcatFilter2 = "";
    QString refcatFilter3 = "";
    QString refcatFilter4 = "";
    QString refcatFilter5 = "";

    long numSources = 0;
    int numG2sources = 0;

    float naxis1 = 0.;       // needed for building the anet index
    float naxis2 = 0.;       // needed for building the anet index
    float pixscale = 0.;     // needed for building the anet index

    float radius = 0.;
    QString radius_string = "";

    bool fromImage = false;

    QVector<double> ra_out;
    QVector<double> de_out;
    QVector<float> mag1_out;
    QVector<float> mag2_out;
    QVector<float> mag3_out;
    QVector<float> mag4_out;
    QVector<float> mag5_out;
    QVector<bool> G2type;
    QVector<float> mag1err_out;
    QVector<float> mag2err_out;

    void initAstromQuery();
    void doBrightStarQueryFromWeb();
    void doAstromQueryFromWeb();
    void doPhotomQueryFromWeb();
    void doColorCalibQueryFromWeb();
    void doGaiaQuery();
    QString resolveTarget(QString target);
    void writeAstromScamp();
    void writeAstromANET();
    void writeAstromIview();
    void pushNumberOfSources();
    void identifySolarTypeStars();
signals:
    void bulkMotionObtained(QString pmRA_GUI, QString pmDE_GUI);
    void messageAvailable(QString messageString, QString code);
    void critical();

private:
    QString path;
    QString thelidir;
    QString userdir;
    QString instrument_dir;
    QString pythonExecutable = "";

    // The location and radius for the search, and their string representations
    double alpha = 0.;
    double delta = 0.;
    float magLimit = 25.;
    float maxProperMotion = 1.e9;
    QString alpha_string = "";
    QString delta_string = "";

    double rad = 3.1415926535 / 180.;

    int *verbosity;

    bool successProcessing = true;

    QVector<double> raMotions;     // Contains the proper motions in RA
    QVector<double> deMotions;     // Contains the proper motions in DEC
    QVector<double> raMotions_err; // Contains the proper motion errors in RA
    QVector<double> deMotions_err; // Contains the proper motion errors in DEC
    double raBulkMotion = 0.;
    double deBulkMotion = 0.;
    double raErrBulkMotion = 0.;
    double deErrBulkMotion = 0.;
    double epoch = 0;
    double epochReference = 2016.0;      // Reference epoch for GAIA-EDR3. WARNING: might change with newer GAIA releases!

//    QString downloadServer = "";
    QString queryCommand = "";
    QByteArray byteArray;
    bool successfulDataBaseAccess = true;

    int firstUsableRow = 0;   // The first line in the downloaded catalog that contains usable data (not counting #-ed lines)

    // G2 references. These ranges roughly span G0V - G5V
    const float sunUG = 1.43;  // u-g
    const float sunGR = 0.44;  // g-r
    const float sunRI = 0.11;  // r-i
    const float sunIZ = 0.03;  // i-z
    const float sunBV = 0.65;  // B-V
    const float sunVR = 0.36;  // V-R
    const float sunUGtol = 0.08;    // tolerance
    const float sunGRtol = 0.05;    // tolerance
    const float sunRItol = 0.05;    // tolerance
    const float sunIZtol = 0.05;    // tolerance
    const float sunBVtol = 0.05;    // tolerance
    float c1min = 0.;
    float c1max = 0.;
    float c2min = 0.;
    float c2max = 0.;
    float c3min = 0.;
    float c3max = 0.;
    float c1tol = 0.;
    float c2tol = 0.;
    float c3tol = 0.;

    void getCatalogSearchLocationAstrom();
    void getCatalogSearchLocationPhotom();
    void getCatalogSearchRadiusAstrom();
    void getCatalogSearchLocationColorCalib();
    void buildQuerySyntaxAstrom();
    void buildQuerySyntaxGaia();
    void buildQuerySyntaxPhotom();
    void buildQuerySyntaxColorCalib();
    QString extractRaDecMagAstrom(QString &line);
    void processAstromCatalog();
    void processGaiaCatalog();
    void processPhotomCatalog();
    void processColorCalibCatalog();
    void processBrightStarCatalog();
    void getMedianEpoch();
//    void provideHeaderInfo();
    void measureBulkMotion();
    void clearAstrom();
    void clearPhotom();
    void clearColorCalib();
    void clearGaia();
    void runCommand(QString command);
    void dumpRefcatID();
    void initPhotomQuery();
    void initGaiaQuery();
    void initColorCalibQuery();
    QString filterStringToVizierName(QString filter);
    QString extractRaDecMagPhotom(QString &line);
    QString extractRaDecGaia(QString &line);
//    QString parseXML(QString &line, const QString &tag);
//    void parseXML2(QString &line, const QString &tag, QString &val1, QString &val2);
    QString extractRaDecMagColorCalib(QString &line);
    void printCfitsioError(QString funcName, int status);
};

#endif // QUERY_H
