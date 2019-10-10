#ifndef MYWCS_H
#define MYWCS_H

#include <QString>

class MyWCS
{
public:
    MyWCS();

    double crval1 = -99.;
    double crval2 = -99.;
    double crpix1 = -1e9;
    double crpix2 = -1e9;
    double cd1_1 = -99.;
    double cd1_2 = -99.;
    double cd2_1 = -99.;
    double cd2_2 = -99.;
    double plateScale;
    float equinox = 0.;
    long naxis1 = 0;
    long naxis2 = 0;
    bool isValid = false;
    void xy2sky(double x, double y, double &alpha, double &delta);
    void sky2xy(double alpha, double delta, double &x, double &y);
    double getPlateScale();
    bool containsRaDec(QString alphaStr, QString deltaStr);
    void checkValidity();
};

#endif // MYWCS_H
