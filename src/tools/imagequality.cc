#include "imagequality.h"
#include "../myimage/myimage.h"
#include "../instrumentdata.h"
#include "../processingInternal/data.h"
#include "../tools/tools.h"
#include "../functions.h"
#include <QDebug>
#include <QString>
#include <QFile>

ImageQuality::ImageQuality(Data *data, instrumentDataType *instrumentData, QString maindirname, QObject *parent) : QObject(parent)
{
    scienceData = data;
    instData = instrumentData;
    mainDirName = maindirname;
}

void ImageQuality::displayMessageReceived(QString message, QString type)
{
    emit messageAvailable(message, type);
}

void ImageQuality::criticalReceived()
{
    emit critical();
}

bool ImageQuality::getSeeingFromGaia()
{
    // Match GAIA point sources with detections in the image

    // refCat and sourceCat are populated externally in Controller::doImageQualityAnalysis()

    int dummy1;
    int dummy2;
    QVector<QVector<double>> matchedCat;
    int maxCPU = 1;   // external parallelization
    match2D(sourceCat, refCat, matchedCat, matchingTolerance, dummy1, dummy2, maxCPU);

    if (matchedCat.isEmpty()) {
        emit messageAvailable(baseName + " : No reference point sources identified for IQ analysis. Using rh-mag method ...", "warning");
        fwhm = -1.0;
        ellipticity = -1.0;
        return false;
    }

    emit messageAvailable(baseName + " : Matched "+QString::number(matchedCat.length()) + " sources identified for IQ analysis. Using rh-mag method ...", "image");
    QVector<double> fwhmVec;
    QVector<double> ellipticityVec;
    fwhmVec.reserve(matchedCat.length());
    ellipticityVec.reserve(matchedCat.length());
    for (auto &source : matchedCat) {
        fwhmVec << source[3];        // matched vector contains: RA, DEC, MAG, FWHM, ELL
        ellipticityVec << source[4];
    }
//    fwhm = meanMask_T(fwhmVec, QVector<bool>()) * instData->pixscale;
//    ellipticity = meanMask_T(ellipticityVec, QVector<bool>());
    fwhm = straightMedian_T(fwhmVec) * instData->pixscale;
    ellipticity = straightMedian_T(ellipticityVec);

    return true;
}

bool ImageQuality::getSeeingFromRhMag()
{
    return true;
}