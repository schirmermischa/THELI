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

#ifndef CPU_H
#define CPU_H

#include <QObject>
#include <QFile>
#include <QVector>
#include <QTextStream>

class CPU : public QObject
{
    Q_OBJECT
public:
    explicit CPU(QObject *parent = nullptr);
    ~CPU();

    int getCPUload();
    float getCurrentValue();

signals:

private:

    bool CPUbarDeactivated = false;
    double tot1 = 0.;
    double tot2 = 0.;
    double idle1 = 0.;
    double idle2 = 0.;

    void readStatsCPU(double &totval, double &idleval);

    QFile file;
    QTextStream instream;
    int maxCPU = 1;

    long long lastTotal = 0;
    long long lastTotalActive = 0;
    long long lastTotalIdle = 0;

    void init();
public slots:

};

#endif // CPU_H
