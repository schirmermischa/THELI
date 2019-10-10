#ifndef PHOTINST_H
#define PHOTINST_H

#include <QObject>
#include <QVector>
#include <QMap>

class PhotInst : public QObject
{
    Q_OBJECT
public:
    explicit PhotInst(QObject *parent = nullptr);

    // Scamp photometric instruments
    QVector<double> fluxScale;
    QVector<double> expTime;
    QVector<double> RZP;
    QStringList baseName;
    QMap<long,long> indexMap;

    void getRZP();

signals:

private:
    long numExp = 0;

public slots:
};

#endif // PHOTINST_H
