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

#ifndef CORRELATOR_H
#define CORRELATOR_H

#include "../myimage/myimage.h"
#include <fftw3.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

#include <QObject>


class Correlator : public QObject
{
    Q_OBJECT
public:
    explicit Correlator(const MyImage *refimg, const MyImage *comimg, QObject *parent = nullptr);

    int numThreads = 1;

    void xcorrelate();
    QVector<float> find_peak();

private:

    int naxis1;
    int naxis2;
    int n_pad;
    int m_pad;
    QVector<float> dataRef;
    QVector<float> dataCom;
    QVector<float> dataCorrelated;

    bool successProcessing = true;

    void padImages(const QVector<float> &dataref, const QVector<float> &datacom);
signals:

public slots:
};

#endif // CORRELATOR_H
