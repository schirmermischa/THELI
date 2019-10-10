#ifndef CORRELATOR_H
#define CORRELATOR_H

#include "../myimage/myimage.h"
#include <fftw3.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

#include <QObject>


class Correlator : public QObject
{
    Q_OBJECT
public:
    explicit Correlator(const MyImage *refimg, const MyImage *comimg, QObject *parent = nullptr);

    int numThreads = 1;

    void xcorrelate();
    QVector<float> find_peak();

private:

    int naxis1;
    int naxis2;
    int n_pad;
    int m_pad;
    QVector<float> dataRef;
    QVector<float> dataCom;
    QVector<float> dataCorrelated;

    bool successProcessing = true;

    void padImages(const QVector<float> &dataref, const QVector<float> &datacom);
signals:

public slots:
};

#endif // CORRELATOR_H
