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

#ifndef REFCATDATA_H
#define REFCATDATA_H

#include <QObject>
#include <QVector>

class RefCatData : public QObject
{
    Q_OBJECT
public:
    explicit RefCatData(QString refcatname, QObject *parent = nullptr);

    QVector<double> ra;
    QVector<double> de;
    QVector<float> mag1;
    QVector<float> mag2;
    QVector<float> mag3;
    QVector<float> mag4;
    QVector<float> mag5;
    long numRefSources = 0;
    long numG2Sources = 0;
    QString name = "";

    void clear();

signals:

public slots:
};

#endif // REFCATDATA_H
