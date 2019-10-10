#include "mywcs.h"
#include "functions.h"
#include "tools/polygon.h"

MyWCS::MyWCS()
{
    crval1 = 0.0;
    crval2 = 0.0;
    crpix1 = 0.0;
    crpix2 = 0.0;
    cd1_1 = 0.0;
    cd1_2 = 0.0;
    cd2_1 = 0.0;
    cd2_2 = 0.0;
    naxis1 = 0;
    naxis2 = 0;
}

bool MyWCS::containsRaDec(QString alphaStr, QString deltaStr)
{
    double alpha_ul;
    double alpha_ur;
    double alpha_ll;
    double alpha_lr;
    double delta_ul;
    double delta_ur;
    double delta_ll;
    double delta_lr;

    // Convert the cartesian image vertices to RA/DEC
    xy2sky(1, 1, alpha_ll, delta_ll);
    xy2sky(naxis1, 1, alpha_lr, delta_lr);
    xy2sky(1, naxis2, alpha_ul, delta_ul);
    xy2sky(naxis1, naxis2, alpha_ur, delta_ur);

    // Check if the sky coordinates are contained in this picture frame
    QVector<double> raVec;
    QVector<double> decVec;
    // order is important! we don't want a line crossing in the polygon line
    raVec << alpha_ll << alpha_lr << alpha_ur << alpha_ul;
    decVec << delta_ll << delta_lr << delta_ur << delta_ul;

    // Convert to decimal if required
    if (alphaStr.contains(":")) alphaStr = hmsToDecimal(alphaStr);
    if (deltaStr.contains(":")) deltaStr = dmsToDecimal(deltaStr);
    return pnpoly_T(raVec, decVec, alphaStr.toDouble(), deltaStr.toDouble());
}

void MyWCS::xy2sky(double x, double y, double &alpha, double &delta)
{
    double pi = 3.14159265;
    double dx = x - crpix1;
    double dy = y - crpix2;
    double xt = cd1_1*dx + cd2_1*dy;
    double yt = cd1_2*dx + cd2_2*dy;
    delta = crval2 + yt;
    alpha = crval1 + xt/cos(delta*pi/180.);
}

void MyWCS::sky2xy(double alpha, double delta, double &x, double &y)
{
    double pi = 3.14159265;
    double cosd = cos(delta * pi/180.);
    double cd12 = cd1_2;
    double cd21 = cd2_1;
    double cd11 = cd1_1;
    double cd22 = cd2_2;
    double detCD = (cd11 * cd22 - cd12 * cd21);
    x = ( (alpha-crval1)*cd22*cosd + cd12*cd21*crpix1 + cd11*cd22*crpix1 + (crval2-delta)*cd12) / detCD;
    y = ( (crval1-alpha)*cd21*cosd + cd12*cd21*crpix2 + cd11*cd22*crpix2 + (delta-crval2)*cd11) / detCD;
}

void MyWCS::checkValidity()
{
    if (crval1 != -99.
            && crval2 != -99.
            && crpix1 > -1e9
            && crpix2 > -1e9
            && cd1_1 != -99.
            && cd1_2 != -99.
            && cd2_1 != -99.
            && cd2_2 != -99.
            && naxis1 > 0
            && naxis2 > 0
            && equinox > 0.) {
        isValid = true;
    }
    else {
        isValid = false;
    }
}

double MyWCS::getPlateScale()
{
    plateScale = sqrt(cd1_1 * cd1_1 + cd2_1 * cd2_1) * 3600.;
    if (plateScale == 0.) plateScale = 1.0;
    return plateScale;
}
