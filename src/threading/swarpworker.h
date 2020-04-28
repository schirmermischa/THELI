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

#ifndef SWARPWORKER_H
#define SWARPWORKER_H

#include "worker.h"

#include <QObject>
#include <QProcess>

class SwarpWorker : public Worker
{
    Q_OBJECT

public:
    explicit SwarpWorker(QString command, QString dir, QString type, QObject *parent = nullptr);
    QString swarpCommand;
    QString coaddDirName;
    QString swarpType;
    int threadID;
    QProcess *extProcess = nullptr;

    void abort();

public slots:
    void runSwarp();

signals:
    void finishedPreparation();
    void errorFound();
    void finishedResampling(int threadID);
    void finishedCoaddition();
    void messageAvailable(QString message, QString type);

private slots:
    void processExternalStderr();
};

#endif // SWARPWORKER_H
