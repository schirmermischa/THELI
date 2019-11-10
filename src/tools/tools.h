#ifndef TOOLS_H
#define TOOLS_H

#include <QObject>
#include <QVector>
#include "instrumentdata.h"
#include "../myimage/myimage.h"

void binData(const QVector<float> &data, QVector<float> &dataBinned, int n, int m,
              int nb, int mb, int binX, int binY);
QVector<int> CDmatrixToTransformationMatrix(QVector<double> CD, QString instName);
void debayer(int chip, MyImage *image, MyImage *imageB, MyImage *imageG, MyImage *imageR);
int direction(float N, float E, float W, float S);
void equalizeBayerFlat(MyImage *image);
void getBinnedSize(instrumentDataType *instData, QVector<QVector<int> > Tmatrices,
                   int &n_bin, int &m_bin, int binFactor, int &xminOffset, int &yminOffset);
float hue_transit(float l1, float l2, float l3, float v1, float v3);
void mapBinnedData(QVector<float> &dataBinnedGlobal, const QVector<float> &dataBinnedIndividual, QVector<int> T,
                   int nGlobal, int mGlobal, int nInd, int mInd, long crpix1, long crpix2, int xminOffset,
                   int yminOffset, const QString instName);
float posangle(QVector<double> CDmatrix);
void rotateCDmatrix(QVector<double> &CDin, float pa_new);
void updateDebayerMemoryStatus(MyImage *image);
QVector<float> collapse_x(QVector<float> &data, const QVector<bool> &globalMask, QVector<bool> &objectMask,
                          float kappa, long n, long m, QString returnMode);
QVector<float> collapse_y(QVector<float> &data, const QVector<bool> &globalMask, QVector<bool> &objectMask,
                          float kappa, long n, long m, QString returnMode);
QVector<float> collapse_quad(QVector<float> &data, const QVector<bool> &globalMask, QVector<bool> &objectMask,
                             float kappa, long n, long m, QString direction, QString returnMode);
void match2D(QVector<QVector<double>> vec1, QVector<QVector<double>> vec2, QVector<QVector<double>> &matched,
             double tolerance, int &multiple1, int &multiple2, int nthreads);

#endif // TOOLS_H
