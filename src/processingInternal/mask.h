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

#ifndef MASK_H
#define MASK_H

#include "../instrumentdata.h"

#include <QObject>

// This class is used to create boolean masks for images (one mask per detector).
// Value == false: good pixel (unmasked)
// Value == true: bad pixel (masked)

class Mask : public QObject
{
    Q_OBJECT
public:
    explicit Mask(const instrumentDataType *instrumentData, QObject *parent = nullptr);

    QVector< QVector<bool> > globalMask;
    QVector<bool> isChipMasked;
    void addImage(int chip, QVector<float> segmentationMap, bool invert);
    void invert();
    void reset();
    void initMasks();
    const instrumentDataType *instData;

private:
    // See polygon.h for further functionality
    //    void addRegionFiles(int chip, QString regionFile);
//    void addRectangle(int chip, QVector<long> rect, bool invert);
//    void addCircle(int chip, float x, float y, float r, QString senseMode);
//    void addPolygon(int chip, QVector<float> &vertx, QVector<float> &verty, QString senseMode);
//    void polygon2vertices(QString polystring, QVector<float> &vertx, QVector<float> &verty);
//    void region2circle(QString circlestring, float &x, float &y, float &r);

signals:

public slots:
};

#endif // MASK_H
