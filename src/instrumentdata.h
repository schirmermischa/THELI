#ifndef INSTRUMENTDATA_H
#define INSTRUMENTDATA_H

#include <QVector>
#include <QFile>

typedef struct {
    int numChips;
    QString name = "";
    QString shortName = "";
    QString nameFullPath = "";
    float obslat = 0.;
    float obslong = 0.;
    QString bayer;
    QString type;
    QFile file;
    float pixscale = 1.0; // in arcsec
    float gain = 1.0;
    float radius = 0.1;   // exposure coverage radius in degrees
    float storage = 0;    // MB used for a single image
    float storageExposure = 0.; // MB used for the entire (multi-chip) exposure

    QVector<int> overscan_xmin;
    QVector<int> overscan_xmax;
    QVector<int> overscan_ymin;
    QVector<int> overscan_ymax;
    QVector<int> cutx;
    QVector<int> cuty;
    QVector<int> sizex;
    QVector<int> sizey;
    QVector<int> crpix1;
    QVector<int> crpix2;
} instrumentDataType;

#endif // INSTRUMENTDATA_H
