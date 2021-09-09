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

#include "myaxis.h"
#include <QDebug>

MyAxis::MyAxis(QObject *parent) : QObject(parent)
{

}

void MyAxis::init(double crval_in, double cd_in, long naxis1_in, long naxis2_in, int offset, QString direction, QColor color, QString type)
{
    pen.setColor(color);
    pen.setWidth(0);
    axisOffset = offset;
    tickDirection = direction;
    crval = crval_in;
    cd = cd_in;
    naxis1 = naxis1_in;
    naxis2 = naxis2_in;

    getWavelengthRange();

    ybase = naxis2/2 + axisOffset;
    axisLine.setLine(1, ybase, naxis1, ybase);

    if (type.isEmpty()) getTickmarks();
    if (type == "spectrum") initSpectrumTickmarks();
}

double MyAxis::pixelToWavelength(double x)
{
    return (crval + cd * x) / (1.+z_0);
}

double MyAxis::wavelengthToPixel(double lambda)
{
    return (lambda*(1.+z_0) - crval) / cd;
}

void MyAxis::getTickmarks()
{
    tickLabels.clear();
    tickMarks.clear();

    // Step size depends on resolution
    if (cd < 1.0) tickstep = 100;
    else if (cd < 2.0 && cd >= 1.0) tickstep = 200;
    else if (cd < 4.0 && cd >= 2.0) tickstep = 500;
    else tickstep = 1000;

    // Redshift correction, and lock onto even numbered tick values
    tickstep /= (1.+z);
    if (tickstep < 50) tickstep = 50;
    else if (tickstep >= 50 && tickstep < 150) tickstep = 100;
    else if (tickstep >= 150 && tickstep < 250) tickstep = 200;
    else if (tickstep >= 250 && tickstep < 750) tickstep = 500;
    else tickstep = 1000;
    int start = floor(lambdaMin/tickstep) * tickstep;
    int stop = ceil(lambdaMax/tickstep) * tickstep;

    // The following takes into account that the y-axis in QPixmap is flipped.
    int flip = -1;
    if (tickDirection == "up") flip = 1;

    for (int i=start; i<=stop; i=i+tickstep) {
        float xpos = wavelengthToPixel(i);
        if (i>=lambdaMin && i<=lambdaMax) {
            tickLabels.append(QString::number(i));
            QLineF line = QLineF(xpos, ybase, xpos, ybase+flip*tickLength);
            tickMarks.append(line);
        }
    }
}

void MyAxis::initSpectrumTickmarks()
{
    tickLabels.clear();
    tickMarks.clear();
}

void MyAxis::addSpecies(const QList<QPair<QString, float> > &list)
{
    // The following takes into account that the y-axis in QPixmap is flipped.
    int flip = -1;
    if (tickDirection == "up") flip = 1;

    for (auto &it : list) {
        float lambda = it.second;
        float xpos = wavelengthToPixel(it.second);
        if (lambda >= lambdaMin && lambda <= lambdaMax) {
            tickLabels.append(it.first + " " + QString::number(it.second, 'f', 0));
            QLineF line = QLineF(xpos, ybase, xpos, ybase-flip*tickLength);
            tickMarks.append(line);
        }
    }
}

// must be updated everytime z changes
void MyAxis::getWavelengthRange()
{
    float lambda1 = pixelToWavelength(0);
    float lambda2 = pixelToWavelength(naxis1);

    lambdaMin = lambda1 < lambda2 ? lambda1 : lambda2;
    lambdaMax = lambda1 > lambda2 ? lambda1 : lambda2;
}

void MyAxis::redshiftChangedReceiver(QString zstring)
{
     z = zstring.toFloat();
}

void MyAxis::clear()
{
    tickLabels.clear();
    tickMarks.clear();
}

void MyAxis::initRedshift(QPointF pointStart)
{
    z_0 = z;
}

void MyAxis::closeRedshift()
{
    z = z_0;
}

void MyAxis::redshiftSpectrum(QPointF pointStart, QPointF currentPoint)
{
    float dx = currentPoint.x() - pointStart.x();
    float dz = dx / finesse * (1.+z);
    float z_tmp = z + dz;
    if (z_tmp < 0) z_tmp = 0.;

    z_0 = z_tmp;

    getWavelengthRange();

    emit redshiftRecomputed();
    emit redshiftUpdated(z_0);
}

