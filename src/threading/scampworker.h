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

#ifndef SCAMPWORKER_H
#define SCAMPWORKER_H

#include "worker.h"

#include <QObject>
#include <QProcess>

class ScampWorker : public Worker
{
    Q_OBJECT

public:
    explicit ScampWorker(QString command, QString dir, QString shortname, QObject *parent = nullptr);

    QString scampCommand;
    QString scampDirName;
    QString shortName;
    QProcess *extProcess;

    void abort();

public slots:
    void runScamp();

private slots:
//    void processExternalStdout();
    void processExternalStderr();

signals:
    void errorFound();
    void fieldMatched();
    void finishedScamp();
    void messageAvailable(QString message, QString type);

};

#endif // SCAMPWORKER_H
