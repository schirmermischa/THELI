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

#include "swarpworker.h"
#include "../functions.h"

#include <QProcess>
#include <QTest>

SwarpWorker::SwarpWorker(QString command, QString dir, QString type, QObject *parent) : Worker(parent)
{
    swarpCommand = command;
    coaddDirName = dir;
    swarpType = type;
//    qDebug() << "SW1" << coaddDirName;
}

void SwarpWorker::runSwarp()
{
    extProcess = new QProcess();
    //    connect(&extProcess, &QProcess::readyReadStandardOutput, this, &Controller::processExternalStdout);
    connect(extProcess, &QProcess::readyReadStandardError, this, &SwarpWorker::processExternalStderr);
    QTest::qWait(300);   // If I don't do this, the GUI crashes. It seems the process produces an output faster than the connection can be made ...
    extProcess->setWorkingDirectory(coaddDirName);
    extProcess->start("/bin/sh -c \""+swarpCommand+"\"");
    extProcess->waitForFinished(-1);

    if (swarpType == "swarpPreparation") emit finishedPreparation();
    if (swarpType == "swarpResampling") emit finishedResampling(threadID);
    if (swarpType == "swarpCoaddition") emit finishedCoaddition();

    emit finished();
    // stdout and stderr channels are slotted into the monitor's plainTextEdit
}

void SwarpWorker::abort()
{
    // First, kill the children
    long pid = extProcess->processId();
    killProcessChildren(pid);

    // The kill the process that invokes the commandline task
    extProcess->kill();
    emit finished();
}

void SwarpWorker::processExternalStderr()
{
    QProcess *process = qobject_cast<QProcess*>(sender());

    // have to remove the '\x1B[1A' and '\x1B[1M' escape sequences.
    QString stderr(process->readAllStandardError());
    stderr.remove(QRegExp("[^a-zA-Z\\d\\s\\.\\:\\-\\_/]"));
    stderr.remove("1A1M ");
    stderr.remove("1A ");
    stderr.remove("1M ");
    stderr = stderr.simplified();
    stderr.replace(" Center:", "<br>Center:");
    stderr.replace(" Gain:", "<br>Gain:");
    stderr.replace(" Scale:", "<br>Scale:");
    stderr.replace(" Background:", "<br>Background:");
    stderr.replace(" RMS:", "<br>RMS:");
    stderr.replace(" Weight scale:", "<br>Weight scale:");
    stderr.replace("... Looking for ", "<br>Looking for ");
    stderr.replace(" Examining input data ...", "<br>Examining input data");
    stderr.replace(" Creating NEW output image ", "<br>Creating NEW output image ");
    stderr.replace(" Creating NEW weight-map ", "<br>Creating NEW weight-map ");
    stderr.replace(" ----- SWarp ", "<br>----- SWarp ");
    stderr.replace(" ------- Output File coadd.fits:", "<br>------- Output File coadd.fits:");
    stderr.replace(" Flux scaling astrom/photom:", "<br>Flux scaling astrom/photom:");
    stderr.replace(" -------------- File", "<br>-------------- File");
    stderr.remove(QRegExp("^1A"));
    stderr.remove(" 0m ");
    stderr.remove(" 7m ");

    QStringList warnings;
    warnings << "WARNING: Astrometric approximation too inaccurate for this re-projection";
    warnings << "Significant inaccuracy likely to occur in projection";
    warnings << "inaccuracies likely to occur";

    QStringList errors;
    errors << "Error:";
    errors << "Null or negative global weighting factor";
    errors << "Not enough memory";
    errors << "Buffer overflow detected";
    errors << "core dumped";
    errors << "has flux scale = 0: I will take 1 instead";
    errors << "Could not allocate memory";

    QString type = "normal";
    for (auto &warning : warnings) {
        if (stderr.contains(warning)) {
            type = "warning";
            break;
        }
    }
    for (auto &error : errors) {
        if (stderr.contains(error)) {
            type = "error";
            emit errorFound();
            break;
        }
    }
    emit messageAvailable(stderr.simplified(), type);
}
