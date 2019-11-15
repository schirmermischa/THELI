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

#ifndef ABSZPWORKER_H
#define ABSZPWORKER_H

#include "../abszp/abszeropoint.h"
#include "worker.h"

#include <QObject>

class AbsZeroPoint;

class AbszpWorker : public Worker
{
    Q_OBJECT

public:
    explicit AbszpWorker(AbsZeroPoint *abszeropoint, QObject *parent = nullptr);

    QString abszpCommand;
    QString abszpDirName;
    AbsZeroPoint *absZeroPoint;

    bool yield = false;

public slots:
    void runTask();

private slots:
    void processExternalStdout();
    void processExternalStderr();

signals:

};

#endif // ABSZPWORKER_H
