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
