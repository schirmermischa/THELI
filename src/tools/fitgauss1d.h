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

#ifndef FITGAUSS1D_H
#define FITGAUSS1D_H

#include <QVector>
#include <QDebug>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlinear.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

struct fitData
{
  double *t;
  double *y;
  long n;
};

double gaussian(const double a, const double b, const double c, const double t);
int func_f(const gsl_vector *x, void *params, gsl_vector *f);
int func_df(const gsl_vector *x, void *params, gsl_matrix *J);
int func_fvv(const gsl_vector *x, const gsl_vector *v, void *params, gsl_vector *fvv);
void solve_system(gsl_vector *x, gsl_multifit_nlinear_fdf *fdf, gsl_multifit_nlinear_parameters *params);

template<class T1>
QVector<float> fitGauss1D(const QVector<T1> dataY, double amp0, double mean0, double sigma0, const QVector<T1> dataX = QVector<T1>())
{
    long n = dataY.length();
    int p = 3; // number of parameters for the 1D Gaussian

    // The arrays holding the data (GSL doesn't know anything about vectors)
    double *xarr = new double[n];
    double *yarr = new double[n];

    // Copy the data
    for (long i=0; i<n; ++i) {
        if (dataX.isEmpty()) xarr[i] = double(i);
        else xarr[i] = double(dataX[i]);
        yarr[i] = double(dataY[i]);
    }

    struct fitData fit_data;
    fit_data.n = n;
    fit_data.t = xarr;
    fit_data.y = yarr;

    delete [] xarr;
    delete [] yarr;

    const gsl_rng_type * T = gsl_rng_default;
    gsl_vector *f = gsl_vector_alloc(n);
    gsl_vector *x = gsl_vector_alloc(p);
    gsl_multifit_nlinear_fdf fdf;
    gsl_multifit_nlinear_parameters fdf_params = gsl_multifit_nlinear_default_parameters();
    gsl_rng *r;
    size_t i;

    gsl_rng_env_setup();
    r = gsl_rng_alloc(T);

    // define function to be minimized
    fdf.f = func_f;
    fdf.df = func_df;
    fdf.fvv = func_fvv;
    fdf.n = n;
    fdf.p = p;
    fdf.params = &fit_data;

    // starting point
    gsl_vector_set(x, 0, amp0);
    gsl_vector_set(x, 1, mean0);
    gsl_vector_set(x, 2, sigma0);

    fdf_params.trs = gsl_multifit_nlinear_trs_lmaccel;

    QVector<float> result;      // contains amplitude, mean, sigma
    solve_system(x, &fdf, &fdf_params);
    result << gsl_vector_get(x, 0) << gsl_vector_get(x, 1) << gsl_vector_get(x, 2);

    // debug:  print data and model
    /*
        double A = gsl_vector_get(x, 0);
        double B = gsl_vector_get(x, 1);
        double C = gsl_vector_get(x, 2);
        for (i=0; i<n; ++i) {
            double ti = fit_data.t[i];
            double yi = fit_data.y[i];
            double fi = gaussian(A, B, C, ti);
            qDebug() << ti << yi << fi;
        }
    */

    gsl_vector_free(f);
    gsl_vector_free(x);
    gsl_rng_free(r);

    return result;
}

#endif // FITGAUSS1D_H
