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

#ifndef FITTING_H
#define FITTING_H

#include "../functions.h"

#include <gsl/gsl_math.h>
#include <gsl/gsl_interp2d.h>
#include <gsl/gsl_spline2d.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_multifit.h>
#include <gsl/gsl_vector.h>

#include <QVector>
#include <QObject>

class Fitting : public QObject
{
    Q_OBJECT
public:
    Fitting();
    ~Fitting();

    bool FITSUCCESS = true;
    void makePolynomialFit2D(const int order, QList<QVector<double>> nodes);
    void makePolynomialFit2D(const int order, const QVector<double> x_in, const QVector<double> y_in,
                             const QVector<double> z_in, QVector<double> w_in = QVector<double>());

    gsl_vector *c = nullptr;      // e.g. for polynomial coefficients (fit result)
    gsl_matrix *cov = nullptr;

signals:
    void messageAvailable(QString message, QString type);
    void critical();
    void warning();
};

#endif // FITTING_H
