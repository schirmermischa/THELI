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

#ifndef IMAGEQUALITY_H
#define IMAGEQUALITY_H

#include "../query/query.h"
#include "../processingInternal/data.h"
#include "../instrumentdata.h"

#include <QObject>

class ImageQuality : public QObject
{
    Q_OBJECT
public:
    explicit ImageQuality(Data *scienceData, instrumentDataType *instrumentData, QString maindirname, QObject *parent = nullptr);

    bool getSeeingFromGaia();
    bool getSeeingFromRhMag();

    double matchingTolerance = 1./3600;   // 1.0 arcsec starting value
    QString baseName;
    double fwhm = -1.0;
    double ellipticity = -1.0;
    long numSources = 0;

    QVector<double> source;
    QVector<QVector<double>> sourceCat;
    QVector<QVector<double>> refCat;
    QVector<double> sourceMag;

    QVector<double> refRA;
    QVector<double> refDEC;

private:
    instrumentDataType *instData;
    Data *scienceData;
    QString mainDirName;

signals:
    void messageAvailable(QString message, QString type);
    void critical();

public slots:
private slots:
    void displayMessageReceived(QString message, QString type);
    void criticalReceived();
};

#endif // IMAGEQUALITY_H
