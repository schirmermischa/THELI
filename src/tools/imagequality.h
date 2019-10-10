#ifndef IMAGEQUALITY_H
#define IMAGEQUALITY_H

#include "../query/query.h"
#include "../processingInternal/data.h"
#include "../instrumentdata.h"

#include <QObject>

class ImageQuality : public QObject
{
    Q_OBJECT
public:
    explicit ImageQuality(Data *scienceData, instrumentDataType *instrumentData, QString maindirname, QObject *parent = nullptr);

    bool getSeeingFromGaia();
    bool getSeeingFromRhMag();

    double matchingTolerance = 1./3600;   // 1.0 arcsec
    QString baseName;
    double fwhm = -1.0;
    double ellipticity = -1.0;

    QVector<double> source;
    QVector<QVector<double>> sourceCat;
    QVector<QVector<double>> refCat;
    QVector<double> sourceMag;

    QVector<double> refRA;
    QVector<double> refDEC;

private:
    instrumentDataType *instData;
    Data *scienceData;
    QString mainDirName;

signals:
    void messageAvailable(QString message, QString type);
    void critical();

public slots:
private slots:
    void displayMessageReceived(QString message, QString type);
    void criticalReceived();
};

#endif // IMAGEQUALITY_H
