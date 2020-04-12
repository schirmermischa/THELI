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


// This file deals with running anettractor as an external tool

#include "myimage.h"
#include "../functions.h"
#include "../tools/polygon.h"
#include "../tools/tools.h"
#include "../tools/cfitsioerrorcodes.h"
#include "../processingInternal/data.h"
#include "../threading/anetworker.h"
#include "wcs.h"
#include "wcshdr.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QString>
#include <QProcess>
#include <QTest>

void MyImage::buildAnetCommand(QString pixscale_maxerr, QString thelidir)
{
    if (!successProcessing) return;

    QString refcat = path + "/cat/refcat/theli_mystd.anet";

    anetCommand = findExecutableName("solve-field");
    anetCommand += " -w "+QString::number(naxis1);
    anetCommand += " -e "+QString::number(naxis2);
    anetCommand += " -L "+QString::number(plateScale / pixscale_maxerr.toFloat(), 'f', 5);
    anetCommand += " -H "+QString::number(plateScale * pixscale_maxerr.toFloat(), 'f', 5);
    anetCommand += " -O";                         // suppress output
    anetCommand += " -p";                         // suppress output plots
    anetCommand += " -u app";                     // units arcsec per pixel
    anetCommand += " -l 5";                       // CPU timeout after 5 seconds
    anetCommand += " -T";                         // don't compute SIP polynomials
    anetCommand += " -s MAG";                     // sort column
    anetCommand += " -a ";                        // sort order: ascending (bright sources first)
    anetCommand += " -b "+thelidir+"/config/anet.backend.cfg";        // config file
    anetCommand += " -R none";
    anetCommand += " -B none";
    anetCommand += " -S none";
    anetCommand += " -M none";
    anetCommand += " -W " + path + "/cat/"+chipName+".wcs";   // output file
    anetCommand += path + "/cat/"+chipName+".anet";

    if (*verbosity >= 1) emit messageAvailable("Executing the following astrometry.net command :<br><br>"+anetCommand+"<br><br>", "info");
    if (*verbosity >= 1) emit messageAvailable("<br>astrometry.net output<br>", "ignore");
}

// start in new thread
void MyImage::runAnetCommand()
{
    if (!successProcessing) return;

    if (*verbosity >= 2) emit messageAvailable("Running the following command in " + path + " : <br>"+anetCommand, "image");

    // Run the solve-field command
    workerThread = new QThread();
    anetWorker = new AnetWorker(anetCommand, path);
    anetWorker->moveToThread(workerThread);
    connect(workerThread, &QThread::started, anetWorker, &AnetWorker::runAnet);
    connect(anetWorker, &AnetWorker::errorFound, this, &MyImage::errorFoundReceived);
    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);
    // Direct connection required, otherwise the task hangs after the first solve-field command
    // (does not proceed to the next step in the controller's for loop)
    connect(anetWorker, &AnetWorker::finished, workerThread, &QThread::quit, Qt::DirectConnection);
    connect(anetWorker, &AnetWorker::finished, anetWorker, &QObject::deleteLater);
    connect(anetWorker, &AnetWorker::messageAvailable, this, &MyImage::messageAvailableReceived);
    workerThread->start();
    workerThread->wait();
}
