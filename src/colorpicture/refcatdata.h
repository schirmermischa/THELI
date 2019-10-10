#ifndef REFCATDATA_H
#define REFCATDATA_H

#include <QObject>
#include <QVector>

class RefCatData : public QObject
{
    Q_OBJECT
public:
    explicit RefCatData(QString refcatname, QObject *parent = nullptr);

    QVector<double> ra;
    QVector<double> de;
    QVector<float> mag1;
    QVector<float> mag2;
    QVector<float> mag3;
    QVector<float> mag4;
    QVector<float> mag5;
    long numRefSources = 0;
    long numG2Sources = 0;
    QString name = "";

    void clear();

signals:

public slots:
};

#endif // REFCATDATA_H
