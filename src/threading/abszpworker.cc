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

#include "abszpworker.h"
#include "../abszp/abszeropoint.h"

#include <QProcess>
#include <QTest>

AbszpWorker::AbszpWorker(AbsZeroPoint *abszeropoint, QObject *parent) : Worker(parent)
{
//    abszpCommand = command;
//    abszpDirName = dir;
    absZeroPoint = abszeropoint;
}

void AbszpWorker::runTask()
{
    QTest::qWait(50);
    if (yield) {
        emit finished();
        return;
    }
    absZeroPoint->taskInternalAbszeropoint();

    emit finished();
    // stdout and stderr channels are slotted into the monitor's plainTextEdit
}

void AbszpWorker::processExternalStdout()
{
    QProcess *process = qobject_cast<QProcess*>(sender());
    QString stdout(process->readAllStandardOutput());
    //    QString stdout(externalProcess->readLine());
    stdout.remove(QRegExp("[^a-zA-Z.:-\\d\\s]"));
    emit messageAvailable(stdout.simplified(), "normal");
}

void AbszpWorker::processExternalStderr()
{
    QProcess *process = qobject_cast<QProcess*>(sender());
    QString stderr(process->readAllStandardError());
    stderr.remove(QRegExp("[^a-zA-Z\\d\\s]"));
    emit messageAvailable(stderr.simplified(), "normal");
}
