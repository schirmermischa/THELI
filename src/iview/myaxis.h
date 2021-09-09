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

#ifndef MYAXIS_H
#define MYAXIS_H


#include <math.h>

#include <QObject>
#include <QLineF>
#include <QStringList>
#include <QPen>
#include <QColor>
#include <QVector>

class MyAxis : public QObject
{
    Q_OBJECT
    double wavelengthToPixel(double lambda);
    void initSpectrumTickmarks();

public:
    explicit MyAxis(QObject *parent = nullptr);

    QLineF axisLine;
    QList<QLineF> tickMarks;
    QStringList tickLabels;

    QString tickDirection = "down";

    QPen pen;


    float finesse = 2000.;
    long naxis1 = 0;
    long naxis2 = 0;
    double crval = 0.;
    double cd = 1.0;
    float z = 0.;               // redshift
    float z_0 = 0.;             // the previous value held by z;
    float x_0 = 0.;             // The x coordinate of the mouse cursor when the left button is clicked
    float lambdaMin = 0.;       // wavelength range maximally covered by the axis
    float lambdaMax = 0.;
    int tickLength = 10;
    int ybase = 0.;             // The y coordinate at which the axis is drawn.
    int axisOffset = 0;         // draw the observed axis line this much below the center of the image
    int tickstep = 100;         // Wavelength interval at which tickmarks are drawn

    void init(double crval_in, double cd_in, long naxis1_in, long naxis2_in, int offset, QString direction, QColor color, QString type = "");
    void addSpecies(const QList<QPair<QString, float> > &list);
    void clear();
    void getTickmarks();

signals:
    void redshiftRecomputed();
    void redshiftUpdated(float z_0);

public slots:
    double pixelToWavelength(double x);
    void getWavelengthRange();
    void redshiftChangedReceiver(QString zstring);
    void initRedshift(QPointF pointStart);
    void redshiftSpectrum(QPointF pointStart, QPointF currentPoint);
    void closeRedshift();
};

#endif // MYAXIS_H
