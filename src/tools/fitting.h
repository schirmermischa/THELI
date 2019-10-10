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

class Fitting
{
public:
    Fitting();
    ~Fitting();

    bool FITSUCCESS = true;
    void makePolynomialFit2D(const int order, QList<QVector<double>> nodes);
    void makePolynomialFit2D(const int order, const QVector<double> x_in, const QVector<double> y_in,
                             const QVector<double> z_in, QVector<double> w_in = QVector<double>());

    gsl_vector *c;      // e.g. for polynomial coefficients (fit result)
    gsl_matrix *cov;


};

#endif // FITTING_H
