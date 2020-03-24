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

#include "absphot.h"
#include "../functions.h"
#include "../instrumentdata.h"

#include <gsl/gsl_multifit.h>

#include <QDebug>
#include <QMessageBox>

AbsPhot::AbsPhot()
{

}

void AbsPhot::clear()
{
    qv_ZP.clear();
    qv_mag1ref.clear();
    qv_mag2ref.clear();
    qv_mag1errref.clear();
    qv_mag2errref.clear();
    qv_colorIndividual.clear();
    qv_colorErrIndividual.clear();
    qv_ZPIndividual.clear();
    qv_ZPerrIndividual.clear();
    qv_colorIndividualOutlier.clear();
    qv_ZPIndividualOutlier.clear();
    qv_colorManualOutlier.clear();
    qv_ZPManualOutlier.clear();
    qv_magauto.clear();
    qv_magerrauto.clear();
    qv_magaper.clear();
    qv_magerraper.clear();
    qv_apertures.clear();
    qv_fitMask.clear();
    qv_fitMask_old.clear();
    qv_ManualMask.clear();
    qv_ManualMask_old.clear();
    filter = "";
    colorfilter = "";
    color = "";
    initialized = false;
    numAper = 0;
    numObj = 0;
    ZPauto = 0.;
    ZPautoerr = 0.;
    ZPSelected = "";
    ZPerrSelected = "";
    Color1Selected = "";
    Color2Selected = "";
    Color3Selected = "";
}

void AbsPhot::getColor()
{
    QString filter1 = color.split("-").at(0);
    QString filter2 = color.split("-").at(1);

    qv_colorIndividual.clear();
    qv_colorErrIndividual.clear();
    qv_ZPIndividual.clear();

    for (int i=0; i<numObj; ++i) {
        if (filter1 == filter) qv_colorIndividual.append(qv_mag1ref[i] - qv_mag2ref[i]);
        else if (filter2 == filter) qv_colorIndividual.append(qv_mag2ref[i] - qv_mag1ref[i]);
        else {
            qDebug() << "AbsPhot::getColor(): Error, this should not happen.";
            break;
        }
        qv_colorErrIndividual.append(sqrt(qv_mag1errref[i]*qv_mag1errref[i] + qv_mag2errref[i]*qv_mag2errref[i]));
        qv_ZPerrIndividual.append(sqrt(qv_mag1errref[i]*qv_mag1errref[i] + qv_magerrauto[i]*qv_magerrauto[i]));
        qv_ZPIndividual.append(qv_mag1ref[i] - qv_magauto[i]);
    }
}

void AbsPhot::getZP()
{
    qv_ZP.clear();
    qv_ZPerr.clear();

    // Calculate the ZP for each MAG_APER
    QVector<double> dummyZPaper;
    for (int i=0; i<numAper; ++i) {
        for (int j=0; j<numObj; ++j) {
            // include color term correction
            dummyZPaper.append(qv_mag1ref[j] - qv_magaper[j][i] - slope*qv_colorIndividual[j]);
        }
        double tmp_mederr = medianerrMask(dummyZPaper, qv_fitMask);
        // Add (roughly) error contribution from individual sources
        if (numObj > 0) {
            double tmperr = rmsMask_T(qv_ZPerrIndividual, qv_fitMask) / sqrt(numObj);
            tmp_mederr = sqrt(tmp_mederr*tmp_mederr + tmperr*tmperr);
        }
        qv_ZP.append(medianMask_T(dummyZPaper, qv_fitMask));
        qv_ZPerr.append(tmp_mederr);
        dummyZPaper.clear();
    }

    // Calculate the ZP for MAG_AUTO
    QVector<double> dummyZPauto;
    for (int j=0; j<numObj; ++j) {
        dummyZPauto.append(qv_mag1ref[j] - qv_magauto[j] - slope*qv_colorIndividual[j]);
    }

    ZPauto = medianMask_T(dummyZPauto, qv_fitMask);
    ZPautoerr = medianerrMask(dummyZPauto, qv_fitMask);
    // Add (roughly) error contribution from individual sources
    if (numObj > 0) {
        double tmperr = rmsMask_T(qv_ZPerrIndividual, qv_fitMask) / sqrt(numObj);
        ZPautoerr = sqrt(ZPautoerr*ZPautoerr + tmperr*tmperr);
    }
}

long AbsPhot::setupFitMask()
{
    // Initially, allow all objects to enter the fit;
    // Also initializes qv_fitMask for the first time;
    qv_fitMask.fill(false, numObj);
    qv_fitMask_old.fill(false, numObj);

    // Exclude objects which have been manually rejected
    long count = 0;
    for (long i=0; i<numObj; ++i) {
        if (qv_ManualMask[i]) {
            qv_fitMask[i] = true;
            qv_fitMask_old[i] = true;
        }
        else ++count;
    }
    return count;  // the number of good data points that will enter the fit
}

// Iterative regression
void AbsPhot::regressionLinfit()
{
    double sum_col_ZP = 0.;
    double sum_col_col = 0.;
    double meanColor = 0.;
    double meanZP = 0.;
    int iter_count = 0;
    int iter_max = 10;

    // Initialize object masks for fitting
    setupFitMask();

    // Iterative straight line fit
    while (iter_count <= iter_max) {
        sum_col_ZP = 0.;
        sum_col_col = 0.;
        meanColor = meanMask_T(qv_colorIndividual, qv_fitMask);
        meanZP = meanMask_T(qv_ZPIndividual, qv_fitMask);
        // Calculate the regression coefficients
        for (long i=0; i<numObj; ++i) {
            if (!qv_fitMask[i]) {
                sum_col_ZP += (qv_colorIndividual[i] - meanColor) * (qv_ZPIndividual[i] - meanZP);
                sum_col_col += (qv_colorIndividual[i] - meanColor) * (qv_colorIndividual[i] - meanColor);
            }
        }
        slope = sum_col_ZP / sum_col_col;
        cutoff = meanZP - slope * meanColor;

        // Check for 2.5 sigma outliers
        double diff = 0.;
        double fitval = 0.;
        double kappa = 2.5;
        // Residuals
        QVector<double> residuals = QVector<double>();
        for (long i=0; i<numObj; ++i) {
            fitval = slope * qv_colorIndividual[i] + cutoff;
            diff = qv_ZPIndividual[i] - fitval;
            residuals.append(diff);
        }
        // Outlier? Set flag if yes
        double rmsval = rmsMask_T(residuals, qv_fitMask);
        for (long i=0; i<numObj; ++i) {
            if (fabs(residuals[i]) > kappa*rmsval) qv_fitMask[i] = true;
            else qv_fitMask[i] = false;
        }
        // Compare with previous flag set
        int check = 1;
        for (long i=0; i<numObj; ++i) {
            if (qv_fitMask[i] != qv_fitMask_old[i]) check *= 0;
        }
        if (check == 1) break; // converged
        else {
            // Update "previous" flag set
            for (long i=0; i<numObj; ++i) {
                qv_fitMask_old[i] = qv_fitMask[i];
            }
        }
        // However, exclude objects which have been manually rejected
        for (int i=0; i<numObj; ++i) {
            if (qv_ManualMask[i]) {
                qv_fitMask[i] = true;
                qv_fitMask_old[i] = true;
            }
        }
        iter_count++;
    }

    // count outliers
    num_outliers = 0;
    for (long i=0; i<numObj; ++i) {
        if (qv_fitMask[i]) {
            num_outliers++;
            qv_colorIndividualOutlier.append(qv_colorIndividual[i]);
            qv_ZPIndividualOutlier.append(qv_ZPIndividual[i]);
        }
    }

    qv_colorManualOutlier.clear();
    qv_ZPManualOutlier.clear();
    num_ManualOutliers = 0;
    for (long i=0; i<numObj; ++i) {
        if (qv_ManualMask[i]) {
            num_ManualOutliers++;
            qv_colorManualOutlier.append(qv_colorIndividual[i]);
            qv_ZPManualOutlier.append(qv_ZPIndividual[i]);
        }
    }

    // Store the result, after resetting all parameters (un-used params for a lower order fit are explicitly set to 0)
    fitParams[0] = cutoff;
    fitParams[1] = slope;
    fitParamsErr[0] = 0.;
    fitParamsErr[1] = 0.;
}

bool AbsPhot::regression(int fitOrder)
{
    // Initialize object masks for fitting
    long n = setupFitMask();

    // Reset the solution
    for (auto &it : fitParams) it = 0.;
    for (auto &it : fitParamsErr) it = 0.;

    int p = fitOrder + 1;

    if (n <= p) return false;

    gsl_matrix *X, *cov;
    gsl_vector *y, *w, *c, *yerr, *w_orig, *r;
    X = gsl_matrix_alloc(n, p);    // input
    y = gsl_vector_alloc(n);       // input
    yerr = gsl_vector_alloc(n);    // input
    w = gsl_vector_alloc(n);       // input
    w_orig = gsl_vector_alloc(n);  // input
    r = gsl_vector_alloc(n);       // input
    c = gsl_vector_alloc(p);       // output
    cov = gsl_matrix_alloc(p, p);  // output

    long i = 0;
    for (auto &mask : qv_fitMask) {
        // add data points and (total) errors if not masked by the user
        if (!mask) {
            float xi = qv_colorIndividual[i];
            float yi = qv_ZPIndividual[i];
            float xierr = qv_colorErrIndividual[i];
            float yierr = qv_ZPerrIndividual[i];
            float ei = sqrt(xierr*xierr + yierr*yierr);
            for (int k=0; k<=fitOrder; ++k) {
                gsl_matrix_set(X, i, k, pow(xi, double(k)));
            }
            gsl_vector_set(y, i, yi);
            gsl_vector_set(yerr, i, yierr);
            gsl_vector_set(w, i, 1./(ei*ei));
            gsl_vector_set(w_orig, i, 1./(ei*ei));
            ++i;
        }
    }

    double chisq;
    gsl_multifit_linear_workspace *work = gsl_multifit_linear_alloc (n, p);

    // iterative fit
    int iter = 0;
    int iterMax = 3;
    while (iter < iterMax) {
        gsl_multifit_wlinear(X, w, y, c, cov, &chisq, work);
        gsl_multifit_linear_residuals(X, y, c, r);
        // Reset the weights to the original weights
        for (size_t i=0; i<r->size; ++i) {
            gsl_vector_set(w, i, gsl_vector_get(w_orig, i));
        }
        // 3 sigma outlier rejection (setting the weights to zero for the next fit)
        for (size_t i=0; i<r->size; ++i) {
            if (gsl_vector_get(r, i) > 3.*gsl_vector_get(yerr, i)) {
                gsl_vector_set(w, i, 0.);
            }
        }
        ++iter;
    }
    gsl_multifit_linear_free(work);

    // Store the result, after resetting all parameters (un-used params for a lower order fit are explicitly set to 0)
    for (int k=0; k<p; ++k) {
        fitParams[k] = gsl_vector_get(c,(k));
        fitParamsErr[k] = sqrt(gsl_matrix_get(cov,(k),(k)));
    }

    gsl_matrix_free(X);
    gsl_vector_free(y);
    gsl_vector_free(w);
    gsl_vector_free(c);
    gsl_matrix_free(cov);

    return true;
}
