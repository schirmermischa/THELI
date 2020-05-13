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

#include "fitting.h"

#include <gsl/gsl_math.h>
#include <gsl/gsl_interp2d.h>
#include <gsl/gsl_spline2d.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_multifit.h>
#include <gsl/gsl_vector.h>

#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QDebug>

Fitting::Fitting()
{

}

Fitting::~Fitting()
{

}


// 2D surface fit using polynomial of arbitrary degree
// Weights are optional
void Fitting::makePolynomialFit2D(const int order, const QVector<double> x_in, const QVector<double> y_in,
                                  const QVector<double> z_in, QVector<double> w_in)
{
    if (!FITSUCCESS) return;

    long lx = x_in.length();
    long ly = y_in.length();
    long lz = y_in.length();
    long lw = w_in.length();

    if (lx != ly
            || lx != lz
            || ( lx != lw && lw > 0)) {
        qDebug() << "ERROR: Fitting::makePolynomialFit2D(): Inconsistent size of input arrays" << lx << ly << lz << lw;
        FITSUCCESS = false;
        return;
    }

    // Number of measurement positions
    long N = lx;

    // weights (optional)
    if (w_in.isEmpty()) w_in.fill(1.0, N);

    // Number of free parameters
    int P = 2*order + 1;

    if (N < P) {
        qDebug() << "ERROR: Fitting::makePolynomialFit2D(): Insufficient number of data points (" << N << ") to do a fit of degree" << order;
        FITSUCCESS = false;
        return;
    }


    // we don't use cross-terms
    // Linear combinations of various functions
    // order == 1:
    // z = p0 + p1 x + p2 y
    // order == 2:
    // z = p0 + p1 x + p2 x^2 + p3 y + p3 y^2
    // etc ...

    gsl_vector *y; // observed data ()
    gsl_vector *w; // data weights
    gsl_matrix *X; // data used to predict: x, y

    // allocate space for the matrices and vectors
    X = gsl_matrix_alloc(N, P);   // this is an input
    y = gsl_vector_alloc(N);      // this is an input
    w = gsl_vector_alloc(N);      // this is an input
    c = gsl_vector_alloc(P);      // this is an output
    cov = gsl_matrix_alloc(P, P); // this is an output

    // Put the data into the X matrix, row by row
    int ii = 0;
    for (int i=0; i<N; ++i) {
        int s = 0;
        gsl_matrix_set(X, ii, s++, 1.0);
        // Do not merge these for loops (incrementing s++)
        for (int k=1; k<=order; ++k) {
            gsl_matrix_set(X, ii, s++, pow(x_in[i], k));
        }
        for (int k=1; k<=order; ++k) {
            gsl_matrix_set(X, ii, s++, pow(y_in[i], k));
        }
        ++ii;
    }

    // fill vector of observed data
    ii = 0;
    for (int i=0; i<N; ++i) {
        gsl_vector_set(w, ii, w_in[i]);
        gsl_vector_set(y, ii, z_in[i]);
        ++ii;
    }

    // allocate temporary work space for gsl
    gsl_multifit_linear_workspace *work;
    work = gsl_multifit_linear_alloc(N, P);

    // calculate the fit
    double chisq;
    gsl_multifit_wlinear(X, w, y, c, cov, &chisq, work);

    // deallocate
    gsl_matrix_free(X);
    gsl_vector_free(y);
    gsl_vector_free(w);
    gsl_multifit_linear_free(work);
}

// Overloaded
void Fitting::makePolynomialFit2D(const int order, QList<QVector<double>> nodes)
{
    long numNodes = nodes.length();
    QVector<double> x_in;
    QVector<double> y_in;
    QVector<double> z_in;
    x_in.reserve(numNodes);
    y_in.reserve(numNodes);
    z_in.reserve(numNodes);

    for (auto &it : nodes) {
        x_in.append(it[0]);
        y_in.append(it[1]);
        z_in.append(it[2]);
    }

    makePolynomialFit2D(order, x_in, y_in, z_in);
}
