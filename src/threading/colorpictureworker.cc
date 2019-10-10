#include "colorpictureworker.h"
#include "../colorpicture/colorpicture.h"

#include <QTest>

ColorPictureWorker::ColorPictureWorker(ColorPicture *colorpicture, QString workmode, QObject *parent) : Worker(parent)
{
    colorPicture = colorpicture;
    workMode = workmode;
}

void ColorPictureWorker::runTask()
{
    QTest::qWait(50);
    if (yield) {
        emit finished();
        return;
    }

    if (workMode == "CropCoadds") colorPicture->taskInternalCropCoadds();
    else if (workMode == "ColorCalib") colorPicture->taskInternalColorCalib();
    else if (workMode == "Fits2Tiff") colorPicture->taskInternalFits2Tiff();
    else if (workMode == "BBNBratio") colorPicture->taskInternalBBNBratio();
    else if (workMode == "BBNBcombine") colorPicture->taskInternalBBNBcombine();
    else {
        // nothing yet
    }

    emit tasknameReturned(workMode);
}
