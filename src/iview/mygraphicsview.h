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

#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QDebug>
#include <QMouseEvent>
#include <QScrollBar>

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT

private:
    QPointF rightDragStartPos;
    QPointF rightDragCurrentPos;
    QPointF rightDragEndPos;
    QPointF leftDragStartPos;
    QPointF leftDragCurrentPos;
    QPointF leftDragEndPos;        // unused
    QPointF middleDragStartPos;
    QPointF middleDragCurrentPos;
    QPointF middleDragEndPos;
    bool rightButtonPressed = false;
    bool leftButtonPressed = false;
    bool middleButtonPressed = false;
    QPoint previousMousePoint;

    bool _pan = false;
    int _panStartX = 0;
    int _panStartY = 0;
    bool _wcs = false;
    QPoint wcsStart;

public:
    explicit MyGraphicsView();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
    void enterEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
    QScrollBar *sxbar = nullptr;
    QScrollBar *sybar = nullptr;
    QString middleMouseMode = "DragMode";

    int x = 0;
    int y = 0;

signals:
    void currentMousePos(QPointF);
    void rightDragTravelled(QPointF);
    void middleSkyDragTravelled(QPointF pointStart, QPointF pointEnd);
    void middleMaskingDragTravelled(QPointF pointStart, QPointF pointEnd);
    void middleWCSTravelled(QPointF pointStart, QPointF pointEnd);
    void middleWCSreleased();
    void leftDragTravelled(QPointF pointStart, QPointF pointEnd);
    void rightPress();
    void leftPress(QPointF pointStart);
    void middlePress(QPointF point);
    void leftButtonReleased();
    void rightButtonReleased();
    void middleButtonReleased();
    void middlePressResetCRPIX();
    void mouseLeftView();
    void mouseEnteredView();
    void viewportChanged(QRect viewport_rect);

public slots:
    void updateMiddleMouseMode(QString mode);
};

#endif // MYGRAPHICSVIEW_H
