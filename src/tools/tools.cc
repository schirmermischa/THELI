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

#include "tools.h"
#include "instrumentdata.h"
#include "../functions.h"
#include "../myimage/myimage.h"

#include <omp.h>
#include <QDebug>
#include <QMessageBox>

// Get the max size of a mosaic-ed exposure
void getBinnedSize(const instrumentDataType *instData, const QVector<QVector<int>> Tmatrices,
                   int &n_bin, int &m_bin, int binFactor, int &xminOffset, int &yminOffset)
{
    // The extreme vertices of a mosaic-ed image
    QVector<long> xLimits;
    QVector<long> yLimits;

    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        QVector<int> T = Tmatrices[chip];
        int x0 = -instData->crpix1[chip] + 0;
        int x1 = -instData->crpix1[chip] + instData->sizex[chip];
        int y0 = -instData->crpix2[chip] + 0;
        int y1 = -instData->crpix2[chip] + instData->sizey[chip];
        // Push the x and y coords of the four corners
        xLimits.push_back(T[0]*x0 + T[2]*y0);  // lower left
        xLimits.push_back(T[0]*x1 + T[2]*y0);  // lower right
        xLimits.push_back(T[0]*x0 + T[2]*y1);  // upper left
        xLimits.push_back(T[0]*x1 + T[2]*y1);  // upper right
        yLimits.push_back(T[1]*x0 + T[3]*y0);  // lower left
        yLimits.push_back(T[1]*x1 + T[3]*y0);  // lower right
        yLimits.push_back(T[1]*x0 + T[3]*y1);  // upper left
        yLimits.push_back(T[1]*x1 + T[3]*y1);  // upper right
    }

    // We need these for later use outside this function
    xminOffset = minVec_T(xLimits) / binFactor;
    yminOffset = minVec_T(yLimits) / binFactor;

    // The size of the binned image, with a safety margin of 10 pixels on each side
    n_bin = (maxVec_T(xLimits) - minVec_T(xLimits)) / binFactor + 20;
    m_bin = (maxVec_T(yLimits) - minVec_T(yLimits)) / binFactor + 20;
}

// Remove the plate scale and position angle component from a CD matrix
// (required to create binned images)
QVector<int> CDmatrixToTransformationMatrix(QVector<double> CD, QString instName)
{
    if (instName != "WFC@INT" &&
            instName != "WFC_2x2@INT") rotateCDmatrix(CD, 0.);
    double pscale = sqrt(CD[0]*CD[0] + CD[2]*CD[2]);

    QVector<int> T;
    T.push_back(round(CD[0]/pscale));
    T.push_back(round(CD[1]/pscale));
    T.push_back(round(CD[2]/pscale));
    T.push_back(round(CD[3]/pscale));

    if (instName != "WFC@INT" &&
            instName != "WFC_2x2@INT") {
        // Take out any flips. Only rotations are allowed for binned mosaics
        if (T[0] == -1 && T[3] == 1) T[0] = 1;
        else if (T[0] == 1 && T[3] == -1) T[3] = 1;
        else if (T[1] == -1 && T[2] == 1) T[1] = 1;
        else if (T[1] == 1 && T[2] == -1) T[2] = 1;
    }
    return T;
}

// Rotate a CD matrix to a new position angle
// input angle in [deg]
void rotateCDmatrix(QVector<double> &CD, float pa_new)
{
    double cd11 = CD[0];
    double cd12 = CD[1];
    double cd21 = CD[2];
    double cd22 = CD[3];

    double rad = 3.1415926535 / 180.;

    // is the matrix flipped (det(CD) > 0 if flipped)
    double det = cd11*cd22 - cd12*cd21;
    double f11;
    if (det < 0) f11 = 1.;
    else f11 = -1.;
    double f12 = 0.;
    double f21 = 0.;
    double f22 = 1.;

    // unflip the matrix
    matrixMult_T(f11, f12, f21, f22, cd11, cd12, cd21, cd22);

    // rotate the matrix to the new position angle
    // the current position angle of the CD matrix
    double pa_old = posangle(CD);  // in [deg]
    double pa_diff = rad * (pa_new - pa_old);
    matrixMult_T(cos(pa_diff), -sin(pa_diff), sin(pa_diff), cos(pa_diff), cd11, cd12, cd21, cd22);

    // flip the matrix
    matrixMult_T(f11, f12, f21, f22, cd11, cd12, cd21, cd22);

    CD[0] = cd11;
    CD[1] = cd12;
    CD[2] = cd21;
    CD[3] = cd22;
}

// Get the position angle of a CD matrix [deg]
float posangle(const QVector<double> CD)
{
    double cd11 = CD[0];
    double cd12 = CD[1];
    double cd21 = CD[2];
    double cd22 = CD[3];

    // the pixel scale
    double pscale1 = sqrt(cd11 * cd11 + cd21 * cd21);
    double pscale2 = sqrt(cd12 * cd12 + cd22 * cd22);

    // take out the pixel scale
    cd11 /= pscale1;
    cd12 /= pscale2;
    cd21 /= pscale1;
    cd22 /= pscale2;

    // sqrt(CD elements) is very close to one, but not perfectly
    // as coordinate system is not necessarily orthogonal (shear + contraction)
    double nfac1 = sqrt(cd11 * cd11 + cd21 * cd21);
    double nfac2 = sqrt(cd12 * cd12 + cd22 * cd22);

    // make sure sqrt(CD elements) = 1,
    // so that we can do the inverse trig functions
    cd11 /= nfac1;
    cd21 /= nfac1;
    cd12 /= nfac2;
    cd22 /= nfac2;

    // due to flipping the rotation angle is ambiguous.

    // possibly, the following could be simplified with det(CD),
    // but at the moment I don't see how to identify the additional 2*PI easily
    double pa;
    double PI = 3.1415926535;

    // normal
    if      (cd11 <  0 && cd12 <= 0 && cd21 <= 0 && cd22 >  0) pa = acos(-cd11);       //   0 <= phi <  90
    else if (cd11 >= 0 && cd12 <  0 && cd21 <  0 && cd22 <= 0) pa = acos(-cd11);       //  90 <= phi < 180
    else if (cd11 >  0 && cd12 >= 0 && cd21 >= 0 && cd22 <  0) pa = 2.*PI-acos(-cd11); // 180 <= phi < 270
    else if (cd11 <= 0 && cd12 >  0 && cd21 >  0 && cd22 >= 0) pa = 2.*PI-acos(-cd11); // 270 <= phi < 360

    // flipped
    else if (cd11 >= 0 && cd12 >= 0 && cd21 <= 0 && cd22 >= 0) pa = acos(cd11);       //   0 <= phi <  90
    else if (cd11 <  0 && cd12 >  0 && cd21 <  0 && cd22 <  0) pa = acos(cd11);       //  90 <= phi < 180
    else if (cd11 <  0 && cd12 <= 0 && cd21 >= 0 && cd22 <  0) pa = 2.*PI-acos(cd11); // 180 <= phi < 270
    else if (cd11 >= 0 && cd12 <  0 && cd21 >  0 && cd22 >= 0) pa = 2.*PI-acos(cd11); // 270 <= phi < 360

    else {
        // we are very likely close to 0, 90, 180 or 270 degrees, and the CD matrix is slightly non-orthogonal.
        // In this case, lock onto 0, 90, 180 or 270 degrees. Otherwise, exit with an error.
        double cd11cd12 = fabs(cd11/cd12);
        double cd22cd21 = fabs(cd22/cd21);

        if (cd11cd12 > 20. && cd22cd21 > 20.) {
            if (cd11 > 0. && cd22 > 0.) pa = 0.*PI/2.;
            if (cd11 < 0. && cd22 > 0.) pa = 0.*PI/2.;
            if (cd11 > 0. && cd22 < 0.) pa = 2.*PI/2.;
            if (cd11 < 0. && cd22 < 0.) pa = 2.*PI/2.;
        }

        else if (cd11cd12 < 0.05 && cd22cd21 < 0.05) {
            if (cd12 > 0. && cd21 < 0.) pa = 1.*PI/2.;
            if (cd12 < 0. && cd21 < 0.) pa = 1.*PI/2.;
            if (cd12 > 0. && cd21 > 0.) pa = 3.*PI/2.;
            if (cd12 < 0. && cd21 > 0.) pa = 3.*PI/2.;
        }

        else {
            qDebug() << "Tools::posangle(): ERROR: Could not determine position angle from CD matrix!";
            pa = 0.;
        }
    }

    double rad = PI / 180.;
    return pa / rad;
}

// Calculate binned image (using median filter)
void binData(const QVector<float> &data, QVector<float> &dataBinned, const int n,
             const int nb, const int mb, const int binX, const int binY)
{
    long bsq = binX * binY;
    QVector<float> chunk(bsq,0);

    // Do the binning
    long k;
    for (long j=0; j<mb; ++j) {
        for (long i=0; i<nb; ++i) {
            k = 0;
            for (int jt=j*binY;jt<(j+1)*binY;++jt) {
                for (int it=i*binX;it<(i+1)*binX;++it) {
                    chunk[k++] = data[it+n*jt];
                }
            }
            // Median filter
            dataBinned[i+nb*j] = medianMask(chunk);
        }
    }
}

// Map an individual binned image onto a global output image, using
// the transformation matrix
void mapBinnedData(QVector<float> &dataBinnedGlobal, const QVector<float> &dataBinnedIndividual,
                   const QVector<int> T, const int nGlobal, const int mGlobal, const int nInd, const int mInd,
                   const long crpix1, const long crpix2, const int xminOffset, const int yminOffset, const QString instName)
{
    // Transformation matrix, derived from CD matrix, for relative positioning
    int T0 = T[0];
    int T1 = T[1];
    int T2 = T[2];
    int T3 = T[3];

    for (int j=0; j<mInd; ++j) {
        int jj = j - crpix2;
        for (int i=0; i<nInd; ++i) {
            int ii = i - crpix1;
            // The output pixel position
            int iBin = T0*ii + T2*jj - xminOffset + 10;
            int jBin = T1*ii + T3*jj - yminOffset + 10;
            // The following 4 lines should never be evaluated.
            // If the binned images always look fine, then this can be removed.
            if (iBin >= nGlobal) iBin = nGlobal - 1;
            if (jBin >= mGlobal) jBin = mGlobal - 1;
            if (iBin < 0) iBin = 0;
            if (jBin < 0) jBin = 0;
            if (!instName.contains("MOIRCS")) {
                dataBinnedGlobal[iBin+nGlobal*jBin] = dataBinnedIndividual[i+nInd*j];
            }
            else {
                // "Adding" instead of replacing. MOIRCS detectors are partially masked and these areas
                // do overlap because of the beam splitter (masking valid pixels)
                dataBinnedGlobal[iBin+nGlobal*jBin] += dataBinnedIndividual[i+nInd*j];
            }
        }
    }
}

QVector<float> collapse_x(QVector<float> &data, const QVector<bool> &globalMask, QVector<bool> &objectMask,
                          const float kappa, const long n, const long m, const QString returnMode)
{
    long i, j;
    int iterMax = 5;

    // average columns / rows
    QVector<float> row;
    QVector<float> col;
    row.reserve(n);
    col.reserve(m);

    // Object mask can be empty if defect detection is the first step after launching the GUI
    if (objectMask.isEmpty()) objectMask.fill(false, n*m);

    // extract representative line
    for (j=0; j<m; ++j) {
        for (i=0; i<n; ++i) {
            if (!globalMask[i+n*j] && !objectMask[i+n*j]) row.append(data[i+n*j]);
        }
        col.push_back( meanIterative(row, kappa, iterMax) );
        row.clear();
    }

    // Return 1D collapsed profile
    if (returnMode == "1Dmodel") {
        return col;
    }
    // Return 2D collapsed image
    else if (returnMode == "2Dmodel") {
        QVector<float> collapsed(n*m);
        for (i=0; i<n; ++i) {
            for (j=0; j<m; ++j) {
                collapsed[i+n*j] = col[j];
            }
        }
        return collapsed;
    }
    else {
        // "2Dsubtract" : Subtract 2D model from data
        for (i=0; i<n; ++i) {
            for (j=0; j<m; ++j) {
                if (!globalMask.at(i+n*j)) data[i+n*j] -= col[j];
                else data[i+n*j] = 0.;
            }
        }
        return QVector<float>();  // return value ignored. Updating data directly
    }
}

// Keeping a functor version as comments for future reference
QVector<float> collapse_y(QVector<float> &data, const QVector<bool> &globalMask, QVector<bool> &objectMask,
                          //                                float (*statisticsMode)(QVector<float>, float, int),
                          const float kappa, const long n, const long m, const QString returnMode)
{
    long i, j;
    int iterMax = 5;

    // average columns / rows
    QVector<float> row;
    QVector<float> col;
    row.reserve(n);
    col.reserve(m);

    if (objectMask.isEmpty()) objectMask.fill(false, n*m);

    // extract representative line
    for (i=0; i<n; ++i) {
        for (j=0; j<m; ++j) {
            if (!globalMask[i+n*j] && !objectMask[i+n*j]) col.append(data[i+n*j]);
        }
        //        row.push_back( (*statisticsMode)(col, kappa, iterMax) );
        row.append( meanIterative(col, kappa, iterMax) );
        col.clear();
    }

    // Return 1D collapsed profile
    if (returnMode == "1Dmodel") {
        return row;
    }
    // Return 2D collapsed image
    else if (returnMode == "2Dmodel") {
        QVector<float> collapsed(n*m);
        for (j=0; j<m; ++j) {
            for (i=0; i<n; ++i) {
                collapsed[i+n*j] = row[i];
            }
        }
        return collapsed;
    }
    else {
        // "2Dsubtract" : Subtract 2D model from data
        for (j=0; j<m; ++j) {
            for (i=0; i<n; ++i) {
                if (!globalMask.at(i+n*j)) data[i+n*j] -= row[i];
                else data[i+n*j] = 0.;
            }
        }
        return QVector<float>();  // return value ignored. Updating data directly
    }
}

// collapse along vhhv (vertical, horizontal, horizontal,
// vertical readout quadrants) or hvvh, hhhh, vvvv directions
QVector<float> collapse_quad(QVector<float> &data, const QVector<bool> &globalMask, QVector<bool> &objectMask,
                             const float kappa, const long n, const long m, const QString direction, const QString returnMode)
{
    long i, j;

    if (n % 2 != 0 || m % 2 != 0) {
        qDebug() << "ERROR: image must have even numbered dimensions for this operation!\n";
        qDebug() << "       Dimensions: " << n << "x" << m;
        return QVector<float>();
    }

    long n1 = 0;
    long m1 = 0;
    long nh = n/2;
    long mh = m/2;

    QVector<float> quadrant(nh*mh, 0);
    QVector<float> collquad(nh*mh, 0);
    QVector<float> collapsed2D(n*m, 0);
    QVector<bool> globalMaskquad(nh*mh, false);
    QVector<bool> objectMaskquad(nh*mh, false);

    if (objectMask.isEmpty()) objectMask.fill(false, n*m);

    // do the four quadrants
    long loop = 0;

    while (loop <= 3) {
        // the four quadrants
        // lower left
        if (loop == 0) {
            n1 = 0;
            m1 = 0;
        }
        // lower right
        if (loop == 1) {
            n1 = nh;
            m1 = 0;
        }
        // upper left
        if (loop == 2) {
            n1 = 0;
            m1 = mh;
        }
        // upper right
        if (loop == 3) {
            n1 = nh;
            m1 = mh;
        }

        // copy the quadrant and the mask
        for (j=0; j<mh; ++j) {
            for (i=0; i<nh; ++i) {
                quadrant[i+nh*j] = data[i+n1+n*(j+m1)];
                globalMaskquad[i+nh*j] = globalMask[i+n1+n*(j+m1)];
                objectMaskquad[i+nh*j] = objectMask[i+n1+n*(j+m1)];
            }
        }

        // collapse the quadrant
        if (direction == "yxxy") {
            if (loop == 0) collquad = collapse_y(quadrant, globalMaskquad, objectMaskquad, kappa, nh, mh, "2Dmodel");
            if (loop == 1) collquad = collapse_x(quadrant, globalMaskquad, objectMaskquad, kappa, nh, mh, "2Dmodel");
            if (loop == 2) collquad = collapse_x(quadrant, globalMaskquad, objectMaskquad, kappa, nh, mh, "2Dmodel");
            if (loop == 3) collquad = collapse_y(quadrant, globalMaskquad, objectMaskquad, kappa, nh, mh, "2Dmodel");
        }
        else if (direction == "xyyx") {
            if (loop == 0) collquad = collapse_x(quadrant, globalMaskquad, objectMaskquad, kappa, nh, mh, "2Dmodel");
            if (loop == 1) collquad = collapse_y(quadrant, globalMaskquad, objectMaskquad, kappa, nh, mh, "2Dmodel");
            if (loop == 2) collquad = collapse_y(quadrant, globalMaskquad, objectMaskquad, kappa, nh, mh, "2Dmodel");
            if (loop == 3) collquad = collapse_x(quadrant, globalMaskquad, objectMaskquad, kappa, nh, mh, "2Dmodel");
        }
        else if (direction == "xxxx") {
            if (loop == 0) collquad = collapse_x(quadrant, globalMaskquad, objectMaskquad, kappa, nh, mh, "2Dmodel");
            if (loop == 1) collquad = collapse_x(quadrant, globalMaskquad, objectMaskquad, kappa, nh, mh, "2Dmodel");
            if (loop == 2) collquad = collapse_x(quadrant, globalMaskquad, objectMaskquad, kappa, nh, mh, "2Dmodel");
            if (loop == 3) collquad = collapse_x(quadrant, globalMaskquad, objectMaskquad, kappa, nh, mh, "2Dmodel");
        }
        else if (direction == "yyyy") {
            if (loop == 0) collquad = collapse_y(quadrant, globalMaskquad, objectMaskquad, kappa, nh, mh, "2Dmodel");
            if (loop == 1) collquad = collapse_y(quadrant, globalMaskquad, objectMaskquad, kappa, nh, mh, "2Dmodel");
            if (loop == 2) collquad = collapse_y(quadrant, globalMaskquad, objectMaskquad, kappa, nh, mh, "2Dmodel");
            if (loop == 3) collquad = collapse_y(quadrant, globalMaskquad, objectMaskquad, kappa, nh, mh, "2Dmodel");
        }
        else {
            qDebug() << "QDEBUG: collapseQuad(): Invalid collapse direction" << direction;
        }

        // reconstruct the full (n x m) sized image
        for (j=0; j<mh; ++j) {
            for (i=0; i<nh; ++i) {
                collapsed2D[i+n1+n*(j+m1)] = collquad[i+nh*j];
            }
        }
        ++loop;
    }

    if (returnMode == "2Dmodel") return collapsed2D;
    else {
        // subtract 2D model
        long i = 0;
        for (auto &pixel : data) {
            if (!globalMask.at(i)) pixel -= collapsed2D.at(i);
            else pixel = 0.;
            ++i;
        }
        return QVector<float>();
    }
}

// Sort a 2D QVector by its first element
void sort2DVector(QVector<QVector<double>> data)
{
    std::sort(data.begin(), data.end(),
              [](const QVector<double>& left, const QVector<double>& right)->bool {
        if (left.empty() && right.empty()) return false;
        if (left.empty()) return true;
        if (right.empty()) return false;
        return left.first() < right.first();
    }
    );
}

// returns distance between two points on the sky [deg]
double haversine(double ra1, double ra2, double dec1, double dec2)
{
    double rad = 3.1415926535 / 180.;
    ra1 *= rad;
    ra2 *= rad;
    dec1 *= rad;
    dec2 *= rad;
    double ddec = dec2 - dec1;
    double dra = ra2 - ra1;
    return 2. * asin( sqrt( pow(sin(ddec/2.),2) + cos(dec1) * cos(dec2) * pow(sin(dra/2.),2) ) ) / rad;
}


/*
// Copy magnitudes and mag errors from a {DEC, RA, <MAG>, <MAGERR>} vector to another {DEC, RA, <MAG>, <MAGERR>} vector (matching)
// <MAG> can be one or more numbers, e.g. 2 ref mags, or several aperture mags
// tolerance is in [deg]
void match2D(const QVector<QVector<double>> vec1, const QVector<QVector<double>> vec2, QVector<QVector<double>> &matched,
             double tolerance, int &multiple1, int &multiple2, int nthreads)
{
    if (vec1.isEmpty() || vec2.isEmpty()) return;

    // Only the reference vector needs to be sorted, using the first column (DEC)
    sort2DVector(vec2);

    long dim1 = vec1.length();  // Objects
    long dim2 = vec2.length();  // Reference sources

    matched.reserve(dim1);

    QVector<int> numMatched1;
    QVector<int> numMatched2;
    numMatched1.fill(0, dim1);  // How many times an object got matched with different reference sources
    numMatched2.fill(0, dim2);  // How many times a reference source got matched with different objects

    omp_lock_t lock;
    omp_init_lock(&lock);

    // First pass: identify ambiguous sources and references
    //#pragma omp parallel for num_threads(nthreads)
    for (long i=0; i<dim1; ++i) {
        bool inside_previous = false;
        bool inside_current = false;
        for (long j=0; j<dim2; ++j) {
            // calc DEC offset and see if we are "within reach" (list is sorted with respect to DEC)
            double ddec = fabs(vec2.at(j)[0] - vec1.at(i)[0]);
            if (ddec < tolerance) inside_current = true;
            else inside_current = false;

            // Leave if we passed the plausible matching zone ("zone" refers to the range in the sorted list that could match)
            //           if (inside_previous && !inside_current) break;

            if (inside_current) {
                // distance between the two points (in [deg])
                double distance = haversine(vec2.at(j)[1], vec1.at(i)[1], vec2.at(j)[0], vec1.at(i)[0]);
                if (distance < tolerance) {
                    ++numMatched1[i];
                    ++numMatched2[j];
                }
            }
            inside_previous = inside_current;
        }
    }

    multiple1 = 0;
    multiple2 = 0;
    for (auto &mult : numMatched1) {
        if (mult>1) ++multiple1;
    }
    for (auto &mult : numMatched2) {
        if (mult>1) ++multiple2;
    }

    // Second pass, match unambiguous sources, only
#pragma omp parallel for num_threads(nthreads)
    for (long i=0; i<dim1; ++i) {
        //        if (numMatched1[i] > 1) continue;
        bool inside_previous = false;
        bool inside_current = false;
        QVector<double> dummy;
        for (long j=0; j<dim2; ++j) {
            // distance between the two points (in [deg])
            double distance = haversine(vec2.at(j)[1], vec1.at(i)[1], vec2.at(j)[0], vec1.at(i)[0]);
//            if (nthreads == 0) qDebug() << qSetRealNumberPrecision(10) << vec2.at(j)[1] << vec1.at(i)[1] << vec2.at(j)[0] << vec1.at(i)[0] << distance << tolerance;
            if (distance < tolerance) {
                dummy << vec1.at(i)[0]; // RA OBJ
                dummy << vec1.at(i)[1]; // DEC OBJ
                for (int k=2; k<vec2.at(j).length(); ++k) {
                    dummy << vec2.at(j)[k]; // MAG and MAGERR for reference sources
                }
                for (int k=2; k<vec1.at(i).length(); ++k) {
                    dummy << vec1.at(i)[k]; // MAG and MAGERR for objects
                }
#pragma omp critical
                {
                    matched.append(dummy);
                }
            }
            inside_previous = inside_current;
        }
    }

    omp_destroy_lock(&lock);
}
*/

// Copy magnitudes and mag errors from a {DEC, RA, <MAG>, <MAGERR>} vector to another {DEC, RA, <MAG>, <MAGERR>} vector (matching)
// <MAG> can be one or more numbers, e.g. 2 ref mags, or several aperture mags
// tolerance is in [deg]
void match2D(const QVector<QVector<double>> vec1, const QVector<QVector<double>> vec2, QVector<QVector<double>> &matched,
             double tolerance, int &multiple1, int &multiple2, int nthreads)
{
    if (vec1.isEmpty() || vec2.isEmpty()) return;

    // Only the reference vector needs to be sorted, using the first column (DEC)
    sort2DVector(vec2);

    long dim1 = vec1.length();  // Objects
    long dim2 = vec2.length();  // Reference sources

    matched.reserve(dim1);

    QVector<int> numMatched1;
    QVector<int> numMatched2;
    numMatched1.fill(0, dim1);  // How many times an object got matched with different reference sources
    numMatched2.fill(0, dim2);  // How many times a reference source got matched with different objects

    omp_lock_t lock;
    omp_init_lock(&lock);

    // First pass: identify ambiguous sources and references
    //#pragma omp parallel for num_threads(nthreads)
    for (long i=0; i<dim1; ++i) {
        bool inside_previous = false;
        bool inside_current = false;
        for (long j=0; j<dim2; ++j) {
            // calc DEC offset and see if we are "within reach" (list is sorted with respect to DEC)
            double ddec = fabs(vec2.at(j)[0] - vec1.at(i)[0]);
            if (ddec < tolerance) inside_current = true;
            else inside_current = false;

            // Leave if we passed the plausible matching zone ("zone" refers to the range in the sorted list that could match)
            if (inside_previous && !inside_current) break;

            if (inside_current) {
                // distance between the two points (in [deg])
                double distance = haversine(vec2.at(j)[1], vec1.at(i)[1], vec2.at(j)[0], vec1.at(i)[0]);
                if (distance < tolerance) {
                    ++numMatched1[i];
                    ++numMatched2[j];
                }
            }
            inside_previous = inside_current;
        }
    }

    multiple1 = 0;
    multiple2 = 0;
    for (auto &mult : numMatched1) {
        if (mult>1) ++multiple1;
    }
    for (auto &mult : numMatched2) {
        if (mult>1) ++multiple2;
    }

//    qDebug() << multiple1 << multiple2;

    // Second pass, match unambiguous sources, only
#pragma omp parallel for num_threads(nthreads)
    for (long i=0; i<dim1; ++i) {
        if (numMatched1[i] > 1) continue;
        bool inside_previous = false;
        bool inside_current = false;
        QVector<double> dummy;
        for (long j=0; j<dim2; ++j) {
            if (numMatched2[j] > 1) continue;
            // calc DEC offset and see if we are "within reach"
            double ddec = fabs(vec2.at(j)[0] - vec1.at(i)[0]);
            if (ddec < tolerance) inside_current = true;
            else inside_current = false;

            // Leave if we passed the plausible matching zone
            if (inside_previous && !inside_current) break;

            if (inside_current) {
                // distance between the two points (in [deg])
                double distance = haversine(vec2.at(j)[1], vec1.at(i)[1], vec2.at(j)[0], vec1.at(i)[0]);
                if (distance < tolerance) {
                    dummy << vec1.at(i)[0]; // RA OBJ
                    dummy << vec1.at(i)[1]; // DEC OBJ
                    for (int k=2; k<vec2.at(j).length(); ++k) {
                        dummy << vec2.at(j)[k]; // MAG and MAGERR for reference sources
                    }
                    for (int k=2; k<vec1.at(i).length(); ++k) {
                        dummy << vec1.at(i)[k]; // MAG and MAGERR for objects
                    }
#pragma omp critical
                    {
                        matched.append(dummy);
                    }
                }
            }
            inside_previous = inside_current;
        }
    }

    omp_destroy_lock(&lock);
}

// same as match2d, just uses the reference coordinates in the output catalog
void match2D_refcoords(const QVector<QVector<double>> vec1, const QVector<QVector<double>> vec2, QVector<QVector<double>> &matched,
             double tolerance, int &multiple1, int &multiple2, int nthreads)
{
    if (vec1.isEmpty() || vec2.isEmpty()) return;

    // Only the reference vector needs to be sorted, using the first column (DEC)
    sort2DVector(vec2);

    long dim1 = vec1.length();  // Objects
    long dim2 = vec2.length();  // Reference sources

    matched.reserve(dim1);

    QVector<int> numMatched1;
    QVector<int> numMatched2;
    numMatched1.fill(0, dim1);  // How many times an object got matched with different reference sources
    numMatched2.fill(0, dim2);  // How many times a reference source got matched with different objects

    omp_lock_t lock;
    omp_init_lock(&lock);

    // First pass: identify ambiguous sources and references
    //#pragma omp parallel for num_threads(nthreads)
    for (long i=0; i<dim1; ++i) {
        bool inside_previous = false;
        bool inside_current = false;
        for (long j=0; j<dim2; ++j) {
            // calc DEC offset and see if we are "within reach" (list is sorted with respect to DEC)
            double ddec = fabs(vec2.at(j)[0] - vec1.at(i)[0]);
            if (ddec < tolerance) inside_current = true;
            else inside_current = false;

            // Leave if we passed the plausible matching zone ("zone" refers to the range in the sorted list that could match)
            if (inside_previous && !inside_current) break;

            if (inside_current) {
                // distance between the two points (in [deg])
                double distance = haversine(vec2.at(j)[1], vec1.at(i)[1], vec2.at(j)[0], vec1.at(i)[0]);
                if (distance < tolerance) {
                    ++numMatched1[i];
                    ++numMatched2[j];
                }
            }
            inside_previous = inside_current;
        }
    }

    multiple1 = 0;
    multiple2 = 0;
    for (auto &mult : numMatched1) {
        if (mult>1) ++multiple1;
    }
    for (auto &mult : numMatched2) {
        if (mult>1) ++multiple2;
    }

//    qDebug() << multiple1 << multiple2;

    // Second pass, match unambiguous sources, only
#pragma omp parallel for num_threads(nthreads)
    for (long i=0; i<dim1; ++i) {
        if (numMatched1[i] > 1) continue;
        bool inside_previous = false;
        bool inside_current = false;
        QVector<double> dummy;
        for (long j=0; j<dim2; ++j) {
            if (numMatched2[j] > 1) continue;
            // calc DEC offset and see if we are "within reach"
            double ddec = fabs(vec2.at(j)[0] - vec1.at(i)[0]);
            if (ddec < tolerance) inside_current = true;
            else inside_current = false;

            // Leave if we passed the plausible matching zone
            if (inside_previous && !inside_current) break;

            if (inside_current) {
                // distance between the two points (in [deg])
                double distance = haversine(vec2.at(j)[1], vec1.at(i)[1], vec2.at(j)[0], vec1.at(i)[0]);
                if (distance < tolerance) {
                    dummy << vec2.at(j)[0]; // RA OBJ
                    dummy << vec2.at(j)[1]; // DEC OBJ
                    for (int k=2; k<vec2.at(j).length(); ++k) {
                        dummy << vec2.at(j)[k]; // MAG and MAGERR for reference sources
                    }
                    for (int k=2; k<vec1.at(i).length(); ++k) {
                        dummy << vec1.at(i)[k]; // MAG and MAGERR for objects
                    }
#pragma omp critical
                    {
                        matched.append(dummy);
                    }
                }
            }
            inside_previous = inside_current;
        }
    }

    omp_destroy_lock(&lock);
}
