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

#include "anetworker.h"
#include "../functions.h"

#include <QProcess>
#include <QTest>

AnetWorker::AnetWorker(QString command, QString dir, QObject *parent) : Worker(parent)
{
    anetCommand = command;
    anetDirName = dir;
}

void AnetWorker::runAnet()
{
    extProcess = new QProcess();
    connect(extProcess, &QProcess::readyReadStandardOutput, this, &AnetWorker::processExternalStdout);
    connect(extProcess, &QProcess::readyReadStandardError, this, &AnetWorker::processExternalStderr);
    QTest::qWait(300);   // If I don't do this, the GUI crashes. It seems the process produces an output faster than the connection can be made ...
    extProcess->setWorkingDirectory(anetDirName);
    extProcess->start("/bin/sh -c \""+anetCommand+"\"");
    extProcess->waitForFinished(-1);

    emit finished();
    // stdout and stderr channels are slotted into the monitor's plainTextEdit
}

void AnetWorker::processExternalStdout()
{
    QProcess *process = qobject_cast<QProcess*>(sender());
    QString stdout(process->readAllStandardOutput());

    stdout.replace(" Field", "<br>Field");
    stdout.replace("\nField", "<br>Field");
    stdout.replace("Hit/miss:   Hit/miss:", "<br>Hit/miss:");

    QStringList errors;
    errors << "Did not solve (or no WCS file was written)." << "Error" << "ERROR";

    // Highlight a successful solve
    // Sometimes neighboring lines are included in one push to stdout, and then they are colored green as well.
    //    if (stdout.contains("solved with index theli_mystd.index")) {
//        emit messageAvailable(stdout.simplified(), "note");
//        return;
//    }


    // And a field that did not solve
    for (auto &error : errors) {
        if (stdout.contains(error)) {
            emit errorFound();
            emit messageAvailable(stdout.simplified(), "stop");
            break;
        }
    }
    emit messageAvailable(stdout.simplified(), "image");
}

void AnetWorker::processExternalStderr()
{
    QProcess *process = qobject_cast<QProcess*>(sender());
    QString stderr(process->readAllStandardError());

    stderr.replace("> *Error*:", "<br><br>> *Error*:");
    stderr.replace("> *ERROR*:", "<br><br>> *ERROR*:");

    QStringList errors;
    errors << "*Error*:" << "*ERROR*:";

    for (auto &error : errors) {
        if (stderr.contains(error)) {
            emit errorFound();
            emit messageAvailable("Astrometry.net: " + stderr.simplified(), "stop");
            break;
        }
    }
}

void AnetWorker::abort()
{
    // First, kill the children
    long pid = extProcess->processId();
    killProcessChildren(pid);

    // The kill the process that invokes the commandline task
    extProcess->kill();

    emit finished();
}
