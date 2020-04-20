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

#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QString>
#include <QProcess>
#include <QTest>

void MyImage::buildAnetCommand(QString pixscale_maxerr)
{
    if (!successProcessing) return;

    anetCommand = findExecutableName("solve-field");
    anetCommand += " -w "+QString::number(naxis1);
    anetCommand += " -e "+QString::number(naxis2);
    anetCommand += " -L "+QString::number(plateScale / pixscale_maxerr.toFloat(), 'f', 5);
    anetCommand += " -H "+QString::number(plateScale * pixscale_maxerr.toFloat(), 'f', 5);
    anetCommand += " -O";                        // suppress output
    anetCommand += " -p";                        // suppress output plots
    anetCommand += " -u app";                    // units arcsec per pixel
    anetCommand += " -l 5";                      // CPU timeout after 5 seconds
    anetCommand += " -T";                        // don't compute SIP polynomials
    anetCommand += " -s MAG";                    // sort column
    anetCommand += " -a ";                       // sort order: ascending (bright sources first)
    anetCommand += " -b " + path + "/astrom_photom_anet/backend.cfg";        // config file
    anetCommand += " -R none";                   // suppress output
    anetCommand += " -B none";                   // suppress output
    anetCommand += " -S none";                   // suppress output
    anetCommand += " -M none";                   // suppress output
    anetCommand += " -U none";                   // suppress output
    anetCommand += " --temp-axy";                // suppress output
    anetCommand += " -W " + path + "/astrom_photom_anet/"+chipName+".wcs";   // output file
    anetCommand += " " + path + "/cat/"+chipName+".anet";

    if (*verbosity > 1) emit messageAvailable("Executing the following astrometry.net command :<br><br>"+anetCommand+"<br>", "info");
}

// start in new thread
void MyImage::runAnetCommand()
{
    if (!successProcessing) return;

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
    connect(anetWorker, &AnetWorker::messageAvailable, this, &MyImage::anetOutputReceived);
    workerThread->start();
    workerThread->wait();
}

QString MyImage::extractAnetOutput()
{
    QString anetOutput = path + "/astrom_photom_anet/"+chipName+".wcs";
    QFile wcsOutput(anetOutput);
    if (!wcsOutput.exists()) {
        emit messageAvailable(chipName + " : Did not solve!", "error");
        emit critical();
        return "";
    }
    else {
       emit messageAvailable(chipName + " : Successfully solved", "note");
    }

    QString header;

    fitsfile *fptr;
    int status = 0;
    int nkeys = 0;
    int keypos = 0;
    char card[FLEN_CARD];
    fits_open_file(&fptr, anetOutput.toUtf8().data(), READONLY, &status);
    fits_get_hdrpos(fptr, &nkeys, &keypos, &status);
    for (int jj = 1; jj <= nkeys; ++jj)  {
        fits_read_record(fptr, jj, card, &status);
        QString cardString(card);
        if (cardString.contains("EQUINOX =")
                || cardString.contains("RADESYS =")
                || cardString.contains("CTYPE1  =")
                || cardString.contains("CTYPE2  =")
                || cardString.contains("CUNIT1  =")
                || cardString.contains("CUNIT2  =")
                || cardString.contains("CRVAL1  =")
                || cardString.contains("CRVAL2  =")
                || cardString.contains("CRPIX1  =")
                || cardString.contains("CRPIX2  =")
                || cardString.contains("CD1_1   =")
                || cardString.contains("CD1_2   =")
                || cardString.contains("CD2_1   =")
                || cardString.contains("CD2_2   =")) {
            header.append(cardString);
            header.append("\n");
        }
    }
    header.append("END\n");

    if (status == END_OF_FILE) status = 0;
    else printCfitsioError("MyImage::reformatAnetOutput()", status);
    fits_close_file(fptr, &status);

    return header;
}
