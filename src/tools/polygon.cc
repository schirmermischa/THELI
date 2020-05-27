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

#include "polygon.h"

#include <QVector>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>

// Split a ds9 region POLYGON string into x- and y vertice arrays
void polygon2vertices(QString polystring, QVector<float> &vertx, QVector<float> &verty)
{
    // extract vertices
    polystring = polystring.split("(")[1].remove(")").simplified();

    // split the polygon string into its vertices
    QStringList list = polystring.split(',');

    // number of vertices
//    int nvert = (polystring.count(',') + 1) / 2;

    // populate
    for (int i=0; i<list.length(); i+=2) {
        vertx.append(list[i].toFloat());
        verty.append(list[i+1].toFloat());
    }
}

void region2circle(QString circlestring, float &x, float &y, float &r)
{
    // extract circle parameters Directory not found in Data class
    circlestring = circlestring.split('(')[1].remove(')').simplified();

    // midpoint and the radius
    QStringList list = circlestring.split(',');
    x = list[0].toFloat();
    y = list[1].toFloat();
    r = list[2].toFloat();
}

/*
void addPolygon_bool(const long n, const long m, const QVector<float> &vertx, const QVector<float> &verty, const QString senseMode, QVector<bool> &mask)
{
    float x, y;

    // apply the polygon mask
    // NOT THREADSAFE!
    // #pragma omp parallel for firstprivate(vertx, verty, senseMode)
    for (long j=0; j<m; ++j) {
        // we have to add +1 to compensate for arrays in C starting with 0
        // because the polygon system starts with 1, not 0
        y = (float) j + 1;
        for (long i=0; i<n; ++i) {
            x = (float) i + 1;
            // if a pixel is masked already (value = true) then it must remain masked no matter what.
            // that is, we only check whether still unmasked pixels need to be masked
            if (!mask.at(i+n*j)) {
                // test if the pixel is inside or outside the polygon
                bool polytest = pnpoly_T(vertx, verty, x, y);
                // Mask pixels inside or outside the polygon
                if (senseMode == "in") {
                    // Pixels inside the polygon are good
                    // pnpoly() returns 0 if a pixel is outside the polygon
                    if (polytest == 0) {
                        mask[i+n*j] = true;
                    }
                }
                else {
                    if (polytest == 1) {
                        mask[i+n*j] = true;
                    }
                }
            }
        }
    }
}
*/

void addPolygon_bool(const long n, const long m, const QVector<float> &vertx, const QVector<float> &verty, const QString senseMode, QVector<bool> &mask)
{
    float x, y;

    // apply the polygon mask
    // NOT THREADSAFE!
    // #pragma omp parallel for firstprivate(vertx, verty, senseMode)
    for (long j=0; j<m; ++j) {
        // we have to add +1 to compensate for arrays in C starting with 0
        // because the polygon system starts with 1, not 0
        y = (float) j + 1;
        for (long i=0; i<n; ++i) {
            x = (float) i + 1;
            // if a pixel is masked already (value = true) then it must remain masked no matter what.
            // that is, we only check whether still unmasked pixels need to be masked
//            if (!mask.at(i+n*j)) {
                // test if the pixel is inside or outside the polygon
                bool polytest = pnpoly_T(vertx, verty, x, y);
                // Mask pixels inside or outside the polygon
                if (senseMode == "in") {        // everything masked by default in the beginning
                    // Pixels inside the polygon are good
                    // pnpoly() returns 0 if a pixel is outside the polygon
                    if (polytest == 1) {
                        mask[i+n*j] = false;
                    }
                }
                else {         // everything unmasked by default in the beginning
                    // Pixels outside the polygon are good
                    if (polytest == 1) {   // pixels inside the polygon must remain masked
                        mask[i+n*j] = true;
                    }
                }
//            }
        }
    }
}

void addPolygon_float(const long n, const long m, const QVector<float> &vertx, const QVector<float> &verty, QString senseMode, QVector<float> &weight)
{
    float x, y;

    // apply the polygon mask
    // NOT THREADSAFE
    //#pragma omp parallel for
    for (long j=0; j<m; ++j) {
        // we have to add +1 to compensate for arrays in C starting with 0
        // because the polygon system starts with 1, not 0
        y = (float) j + 1;
        for (long i=0; i<n; ++i) {
            x = (float) i + 1;
            // if a pixel has weight zero already then it must retain that weight no matter what.
            // that is, we check only pixels with weight > 0
            if (weight[i+n*j] > 0.) {
                // test if the pixel is inside or outside the polygon
                bool polytest = pnpoly_T(vertx, verty, x, y);
                // Mask pixels inside or outside the polygon
                if (senseMode == "in") {
                    // Pixels inside the polygon are bad
                    // pnpoly() returns 0 if a pixel is outside the polygon
                    if (polytest == 1) weight[i+n*j] = 0.;
                }
                else {
                    if (polytest == 0) weight[i+n*j] = 0;
                }
            }
        }
    }
}

void addCircle_bool(const long n, const long m, float x, float y, float r, QString senseMode, QVector<bool> &mask)
{
    float ii, jj, d;

    // NOT THREADSAFE
// #pragma omp parallel for
    for (long j=0; j<m; ++j) {
        jj = (float) j;
        for (long i=0; i<n; ++i) {
            ii = (float) i;
            // if a pixel is masked already (value = true) then it must remain masked no matter what.
            // that is, we only check whether still unmasked pixels need to be masked
            if (!mask[i+n*j]) {
                // Mask pixels inside or outside the circle
                d = (ii-x) * (ii-x) + (jj-y) * (jj-y);
                if (senseMode == "in") {
                    // mask pixels outside the circle
                    if (d >= r*r) mask[i+n*j] = true;
                }
                else {
                    // mask pixels inside the circle
                    if (d <= r*r) mask[i+n*j] = true;
                }
            }
        }
    }
}

void addCircle_float(const long n, const long m, float x, float y, float r, QString senseMode, QVector<float> &weight)
{
    float ii, jj, d;
// NOT THREADSAFE
// #pragma omp parallel for
    for (long j=0; j<m; ++j) {
        jj = (float) j;
        for (long i=0; i<n; ++i) {
            ii = (float) i;
            // if a pixel has weight zero already then it must retain that weight no matter what.
            // that is, we check only pixels with weight > 0
            if (weight[i+n*j] > 0.) {
                // Mask pixels inside or outside the circle
                d = (ii-x) * (ii-x) + (jj-y) * (jj-y);
                if (senseMode == "in") {
                    // mask pixels inside the circle
                    if (d <= r*r) weight[i+n*j] = 0.;
                }
                else {
                    // mask pixels inside the circle
                    if (d >= r*r) weight[i+n*j] = 0.;
                }
            }
        }
    }
}

void addRegionFilesToMask(const long n, const long m, QString regionFile, QVector<bool> &mask, bool &isChipMasked)
{
    QFile file(regionFile);
    if (!file.exists()) return;
    if ( !file.open(QIODevice::ReadOnly)) {
        qDebug() << "polygon::addRegionFilesToWeight: could not open "+regionFile;
        return;
    }

    isChipMasked = false;

    QString senseMode = "in";
    QString combineMode = "or"; // not used

    QTextStream in(&(file));
    while(!in.atEnd()) {
        QString line = in.readLine().simplified();
        if (line.isEmpty()) continue;
        if (line.contains("# Sense: ")) {
            senseMode = line.split(":")[1].simplified();
            if (senseMode == "in") mask.fill(true);     // default, everything masked. Only keep pixels inside polygons and circles
            else mask.fill(false);                      // default, everything unmasked. Only keep pixels inside polygons and circles
        }
        //  if (line.contains("# Combine: ")) combineMode = line.split(":")[1].simplified();

        // There can be an arbitrary sequence of polygons and circles
        // The masks will be combined in the sense that a pixel will be masked if it is masked at least once.

        // Mask a polygon
        if (line.contains("polygon(") || line.contains("POLYGON(")) {
            QVector<float> vertx;
            QVector<float> verty;
            polygon2vertices(line, vertx, verty);
            addPolygon_bool(n, m, vertx, verty, senseMode, mask);
            isChipMasked = true;
        }

        // Mask a circle
        if (line.contains("circle(") || line.contains("CIRCLE(")) {
            float x = 0.;
            float y = 0.;
            float r = 0.;
            region2circle(line, x, y, r);
            addCircle_bool(n, m, x, y, r, senseMode, mask);
            isChipMasked = true;
        }
    }
    file.close();
}

void addRegionFilesToWeight(const long n, const long m, QString regionFile, QVector<float> &weight)
{
    QFile file(regionFile);
    if (!file.exists()) return;
    if ( !file.open(QIODevice::ReadOnly)) {
        qDebug() << "QDEBUG: polygon::addRegionFilesToWeight: could not open "+regionFile;
        return;
    }

    QString senseMode = "in";
    QString combineMode = "or"; // not used


    QTextStream in(&(file));
    while(!in.atEnd()) {
        QString line = in.readLine().simplified();
        if (line.isEmpty()) continue;
        if (line.contains("# Sense: ")) senseMode = line.split(":")[1].simplified();
        //  if (line.contains("# Combine: ")) combineMode = line.split(":")[1].simplified();

        // There can be an arbitrary sequence of polygons and circles
        // The masks will be combined in the sense that a pixel will be masked if it is masked at least once.

        // Mask a polygon
        if (line.contains("polygon(") || line.contains("POLYGON(")) {
            QVector<float> vertx;
            QVector<float> verty;
            polygon2vertices(line, vertx, verty);
            addPolygon_float(n, m, vertx, verty, senseMode, weight);
        }

        // Mask a circle
        if (line.contains("circle(") || line.contains("CIRCLE(")) {
            float x = 0.;
            float y = 0.;
            float r = 0.;
            region2circle(line, x, y, r);
            addCircle_float(n, m, x, y, r, senseMode, weight);
        }
    }
    file.close();
}
