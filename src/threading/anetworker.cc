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
    // Ignoring stdout
    /*
    QProcess *process = qobject_cast<QProcess*>(sender());
    QString stdout(process->readAllStandardOutput());
    //    QString stdout(externalProcess->readLine());
    stdout.remove(QRegExp("[^a-zA-Z.:-\\d\\s]"));
    emit messageAvailable(stdout.simplified(), "normal");
    */
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
