#ifndef POLYGON_H
#define POLYGON_H

#include <QVector>

void polygon2vertices(QString polystring, QVector<float> &vertx, QVector<float> &verty);
void addCircle_bool(long n, long m, float x, float y, float r, QString senseMode, QVector<bool> &mask);
void addPolygon_bool(const long n, const long m, const QVector<float> &vertx, const QVector<float> &verty, const QString senseMode, QVector<bool> &mask);
void addPolygon_float(long n, long m, QVector<float> &vertx, QVector<float> &verty, QString senseMode, QVector<float> &weight);
void addRegionFilesToMask(long n, long m, QString regionFile, QVector<bool> &mask, bool &isChipMasked);
void addRegionFilesToWeight(long n, long m, QString regionFile, QVector<float> &mask);
void region2circle(QString circlestring, float &x, float &y, float &r);

/*
Polygon tester (pnpoly)

Copyright (c) 1970-2003, Wm. Randolph Franklin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

(1) Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimers.
(2) Redistributions in binary form must reproduce the above copyright notice in
the documentation and/or other materials provided with the distribution.
(3) The name of W. Randolph Franklin may not be used to endorse or promote
products derived from this Software without specific prior written permission.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

template<class T>
bool pnpoly_T(const QVector<T> &vertx, const QVector<T> &verty, const T testx, const T testy)
{
    long i, j;
    long nvert = vertx.length();
    bool c = false;
    for (i=0, j=nvert-1; i<nvert; j=i++) {
        if ( ((verty[i]>testy) != (verty[j]>testy)) &&
             (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
            c = !c;
    }
    return c; // false if point is outside polygon, true if inside, true or false if exactly on the connecting line
}


#endif // POLYGON_H
