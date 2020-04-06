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

#ifndef MYFITS_H
#define MYFITS_H

#include <QMap>
#include <QObject>
#include <QVector>
//#include "mywcs.h"
#include "fitsio2.h"

using namespace std;

class MyFITS : public QObject
{
    Q_OBJECT
public:
    explicit MyFITS(QObject *parent = nullptr);
    explicit MyFITS(long n, long m, QObject *parent = nullptr);
//    explicit MyFITS(MyFITS &myfits, QObject *parent = nullptr);
    explicit MyFITS(const MyFITS &myfits, QObject *parent = nullptr);
    explicit MyFITS(QString fileName, QString mode = "", QObject *parent = nullptr);
    explicit MyFITS(QString fileName, const long n, const long m, QVector<float> &, QObject *parent = nullptr);
    explicit MyFITS(QString fileName, const long n, const long m, QVector<long> &, QObject *parent = nullptr);
    explicit MyFITS(QString fileName, const long n, const long m, float value, QObject *parent = nullptr);

    ~MyFITS();

    QString name = "";
    QString weightName = "";
    QString headerRef = "";
    QString fullHeaderString = "";
    QString dateobs = "";
    bool hasWCS = false;
    bool hasMJDread = false;
    bool hasTHELIheader = false;
    bool addGainNormalization = false;
    float gainNormalization = 1.0;
    int numExt = 0;
    double crpix1 = 0.0;
    double crpix2 = 0.0;
    double crval1 = 0.0;
    double crval2 = 0.0;
    double cd1_1 = 0.0;
    double cd1_2 = 0.0;
    double cd2_1 = 0.0;
    double cd2_2 = 0.0;
    float gain = 1.0;
    float equinox = 0.;
    double plateScale = 0.0;
    float exptime = -1.0;
    double mjdobs = 0.0;
    float airmass = 1.0;
    float ellipticity = -1.0;
    float fwhm = -1.0;
    float RZP = -1.0;
    float skyValue = -1e9;
    QString filter;
    long naxis1 = 0;
    long naxis2 = 0;
    int bitpix = 0;
    QVector<float> data;
    QVector<QString> header;
    bool errorMapPopulated = false;

    float constValue;

    char *fullheader = nullptr;
    int numHeaderKeys = 0;
    bool fullheaderAllocated = false;

//    MyWCS myWCS;

    bool inMemory = false;

    bool headerInfoProvided = false;

    void erase();
    void getMJD();
    bool informSwarpfilter(long &naxis1, long &naxis2, double &crpix1, double &crpix2, double &sky, double &fluxscale);
    void initFITS(fitsfile **fptr, int *status);
    bool loadData();
    void loadDataSection(long xmin, long xmax, long ymin, long ymax, QVector<float> &dataSect);
    void loadDataSection(long xmin, long xmax, long ymin, long ymax, float *dataSect);
//    MyWCS loadWCS();
    void propagateHeader(fitsfile *fptr, QVector<QString> header);
    QString readFILTER();
    void stayWithinBounds(QVector<long> &vertices);
    bool write(QString history, float exptime = -1.0, QString filter = "",
               QVector<QString> header = QVector<QString>());
    bool write(QString history, QString something);
    bool writeDebayer(QString history, float exptime, QString filter, double mjdobs, QVector<QString> header = QVector<QString>());
    bool writeLong();
    bool provideHeaderInfo();
    void printerror(int status);
    bool writeConstImage(QString history, float exptime, QString filter, QVector<QString> header);

signals:
    void messageAvailable(QString message, QString type);

private:
    void stayWithinBounds(long &coord, QString axis);   // Overloaded function, see above
    void populateCfitsioErrorKeyMap();
//    void getWCSheaderKeys(fitsfile **fptr, int *status);
    void readHeader(fitsfile **fptr, int *status);
    void readData(fitsfile **fptr, int *status);
    void initWCS(int *status);
    void extractKeywordDouble(QString card, QString key,  double &value);
    void extractKeywordFloat(QString card, QString key, float &value);
    void extractKeywordLong(QString card, QString key, long &value);
    void extractKeywordInt(QString card, QString key, int &value);
    void extractKeywordString(QString card, QString key,  QString &value);
    void initTHELIheader(int *status);
    void checkTHELIheader(int *status);
public slots:
};

#endif // MYFITS_H
