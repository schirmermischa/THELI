#ifndef MASK_H
#define MASK_H

#include "../instrumentdata.h"

#include <QObject>

// This class is used to create boolean masks for images (one mask per detector).
// Value == false: good pixel (unmasked)
// Value == true: bad pixel (masked)

class Mask : public QObject
{
    Q_OBJECT
public:
    explicit Mask(instrumentDataType *instrumentData, QObject *parent = nullptr);

    QVector< QVector<bool> > globalMask;
    QVector<bool> isChipMasked;
    void addImage(int chip, QVector<float> segmentationMap, bool invert);
    void invert();
    void reset();
    void initMasks();
    instrumentDataType *instData;

private:
    // See polygon.h for further functionality
    //    void addRegionFiles(int chip, QString regionFile);
    void addRectangle(int chip, Mask &mask, QVector<long> rect, bool invert);
//    void addCircle(int chip, float x, float y, float r, QString senseMode);
//    void addPolygon(int chip, QVector<float> &vertx, QVector<float> &verty, QString senseMode);
//    void polygon2vertices(QString polystring, QVector<float> &vertx, QVector<float> &verty);
//    void region2circle(QString circlestring, float &x, float &y, float &r);

signals:

public slots:
};

#endif // MASK_H
