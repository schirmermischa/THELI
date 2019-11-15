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

#ifndef COLORPICTUREWORKER_H
#define COLORPICTUREWORKER_H

#include "../colorpicture/colorpicture.h"
#include "worker.h"

#include <QObject>

// Forward declaration, because 'include colorpciture.h' includes this worker
class ColorPicture;

class ColorPictureWorker : public Worker
{
    Q_OBJECT

public:
    explicit ColorPictureWorker(ColorPicture *colorpicture, QString workmode, QObject *parent = nullptr);

    ColorPicture *colorPicture;
    QString workMode;
    bool yield = false;

public slots:
    void runTask();

signals:
    void tasknameReturned(QString workmode);

};

#endif // COLORPICTUREWORKER_H
