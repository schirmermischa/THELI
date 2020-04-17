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

#include "correlator.h"
#include "../myimage/myimage.h"

#include <fftw3.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlin.h>
#include <gsl/gsl_matrix.h>

// #include "Array.h"
// #include "fftw++.h"

#include <QDebug>

Correlator::Correlator(const MyImage *refimg, const MyImage *comimg, QObject *parent) : QObject(parent)
{
    naxis1 = refimg->naxis1;
    naxis2 = refimg->naxis2;

    if (refimg->naxis1 != comimg->naxis1 || refimg->naxis2 != comimg->naxis2) {
        qDebug() << "ERROR: Correlator(): images have different geometries!";
        successProcessing = false;
    }

    padImages(refimg->dataCurrent, comimg->dataCurrent);
}

void Correlator::padImages(const QVector<float> &dataref, const QVector<float> &datacom)
{
    if (! successProcessing) return;

    // Zeropad data, to accomodate very large offsets
    // Max offset is the full field of view, thus we double the image size
    n_pad = 2*naxis1-1;
    m_pad = 2*naxis2-1;

    dataRef.resize(n_pad*m_pad);
    dataCom.resize(n_pad*m_pad);
    dataCorrelated.resize(n_pad*m_pad);

    int left = naxis1/2;
    int right = naxis1/2;
    int bottom = naxis2/2;
    int top = naxis2/2;

    for (int j=0; j<m_pad; ++j) {
        for (int i=0; i<n_pad; ++i) {
            if (i>=left && i<n_pad-right && j>=bottom && j<m_pad-top) {
                dataRef[i+n_pad*j] = dataref[i-left + naxis1*(j-bottom)];
                dataCom[i+n_pad*j] = datacom[i-left + naxis1*(j-bottom)];
            }
            else {
                dataRef[i+n_pad*j] = 0.;
                dataCom[i+n_pad*j] = 0.;
            }
        }
    }
}

void Correlator::xcorrelate()
{
    /*
    int n = n_pad;
    int m = m_pad;

    int mp = m_pad/2 + 1;
    size_t align = sizeof(Complex);

    Array::array2<double> fa(n,m,align), fb(n,m,align);
    Array::array2<Complex> ga(n,mp,align), gb(n,mp,align);

    fftwpp::rcfft2d Forward_a(n,m,fa,ga);
    fftwpp::rcfft2d Forward_b(n,m,fb,gb);
    fftwpp::crfft2d Backward(n,m,ga,fa);

    // write the data into arrays
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            fa(i,j) = dataRef[i+n*j];
            fb(i,j) = dataCom[i+n*j];
        }
    }

    // Forward FFT
    Forward_a.fft0(fa,ga);
    Forward_b.fft0(fb,gb);

    Complex scale = 1.0/(n*m);

    // cross-correlation
    for (int i=0; i<n; i++) {
        for (int j=0; j<mp; j++) {
            ga(i,j) *= conj(gb(i,j)) * scale;
        }
    }

    // Backward FFT
    Backward.fft0Normalized(ga,fa);

    // put things back into a vector
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            dataCorrelated[i+n*j] = fa(i,j);
        }
    }

    // swap upper half with lower half
    for (int i=0; i<n*m/2; i++) {
        float tmp = dataCorrelated[i];
        dataCorrelated[i] = dataCorrelated[i+n*m/2];
        dataCorrelated[i+n*m/2] = tmp;
    }

    // the cross-correlation image is symmetric, hence we don't need
    // to swap the left and right halves
    */
}


//***************************************************************
QVector<float> Correlator::find_peak()
{
    if (! successProcessing) return QVector<float>();

    /*
    long ipos = 0;
    long jpos = 0;
    double max = 0.;

    // ignore the boundary (FFT may put spurious pixels there)
    // we choose a 3 pixel wide boundary, so there won't be any boundary issues when assigning the 7x7 subarray
    for (int j=3; j<m_pad-3; ++j) {
        for (int i=3; i<n_pad-3; ++i) {
            if (dataCorrelated[i+n_pad*j] >= max) {
                ipos = i;
                jpos = j;
                max = (double) dataCorrelated[i+n_pad*j];
            }
        }
    }

    double xpos = (double) ipos;
    double ypos = (double) jpos;

    // we fit a 7x7 pixel area centred on the maximum pixel
    const size_t nx = 7, ny = 7, n = 7*7;
    const gsl_multifit_fdfsolver_type *T;
    gsl_multifit_fdfsolver *s;
    int status;
    const size_t p = 4;

    gsl_matrix *covar = gsl_matrix_alloc (p, p);
    double z[n], sigma[n];
    struct dataGSL d = { nx, ny, z, sigma };

    gsl_multifit_function_fdf f;
    const gsl_rng_type *type;
    gsl_rng *r;

    gsl_rng_env_setup();

    type = gsl_rng_default;
    r = gsl_rng_alloc (type);

    f.f = &gauss_f;
    f.df = &gauss_df;
    f.fdf = &gauss_fdf;
    f.n = n;
    f.p = p;
    f.params = &d;

    // The data in which we search the maximum
    unsigned int k = 0;
    for (int j=-3; j<=3; ++j) {
        for (int i=-3; i<=3; ++i) {
            z[k] = dataCorrelated[ipos+i+n_pad*(jpos+j)] / max;
            sigma[k++] = 1.;  // same weight for all pixels
        }
    }

    // the initial guess has to be as close as possible (totally sucks...)
    double x_init[4] = { 1.0, 3.0, 3.0, 2.0 };

    gsl_vector_view x = gsl_vector_view_array (x_init, p);

    T = gsl_multifit_fdfsolver_lmsder;
    s = gsl_multifit_fdfsolver_alloc (T, n, p);
    gsl_multifit_fdfsolver_set (s, &f, &x.vector);

    unsigned int iter = 0;
    do {
        ++iter;
        status = gsl_multifit_fdfsolver_iterate(s);
        if (status) break;
        status = gsl_multifit_test_delta (s->dx, s->x, 1e-4, 1e-4);
    }
    while (status == GSL_CONTINUE && iter < 50000);

    //  gsl_multifit_covar (s->J, 0.0, covar);

#define FIT(i) gsl_vector_get(s->x, i)
    // #define ERR(i) sqrt(gsl_matrix_get(covar,i,i))

    QVector<float> result;
    result.append(FIT(1)+xpos-3.0+1.0);
    result.append(FIT(2)+ypos-3.0+1.0);

    gsl_multifit_fdfsolver_free(s);
    gsl_matrix_free(covar);
    gsl_rng_free(r);

    return result
    */
    return QVector<float>();
}
