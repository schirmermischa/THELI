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

#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsEllipseItem>

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    MyGraphicsScene();
    void leaveEvent(QEvent *event);

    QGraphicsEllipseItem *crosshairCircleItem = nullptr;
    QGraphicsEllipseItem *crosshairCircleBlackItem = nullptr;
    QGraphicsLineItem *crosshairLineNItem = nullptr;
    QGraphicsLineItem *crosshairLineSItem = nullptr;
    QGraphicsLineItem *crosshairLineEItem = nullptr;
    QGraphicsLineItem *crosshairLineWItem = nullptr;

    bool crosshairShown = false;
    double zoomScale = 1.0;

signals:
    void itemDeleted();
    void mouseLeftScene();

protected:
    void keyReleaseEvent(QKeyEvent * keyEvent);

public slots:
    void removeCrosshair();
    void addCrosshair(double x, double y);
};

#endif // MYGRAPHICSSCENE_H
