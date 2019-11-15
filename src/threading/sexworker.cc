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

#include "sexworker.h"

#include <QProcess>
#include <QTest>

SexWorker::SexWorker(QString command, QString dir, QObject *parent) : Worker(parent)
{
    sexCommand = command;
    sexDirName = dir;
}

void SexWorker::runSex()
{
    QProcess extProcess;
    connect(&extProcess, &QProcess::readyReadStandardOutput, this, &SexWorker::processExternalStdout);
    connect(&extProcess, &QProcess::readyReadStandardError, this, &SexWorker::processExternalStderr);
    QTest::qWait(300);   // If I don't do this, the GUI crashes. It seems the process produces an output faster than the connection can be made ...
    extProcess.setWorkingDirectory(sexDirName);
    extProcess.start("/bin/sh -c \""+sexCommand+"\"");
    extProcess.waitForFinished(-1);

    emit finished();
    // stdout and stderr channels are slotted into the monitor's plainTextEdit
}

void SexWorker::processExternalStdout()
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

void SexWorker::processExternalStderr()
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
            emit messageAvailable("SExtractor: " + stderr.simplified(), "stop");
            break;
        }
    }
}
