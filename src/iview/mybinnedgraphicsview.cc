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

#include "mybinnedgraphicsview.h"
#include "iview.h"

MyBinnedGraphicsView::MyBinnedGraphicsView() : QGraphicsView()
{
    QBrush brush(QColor("#000000"));
    brush.setStyle(Qt::SolidPattern);
    this->setBackgroundBrush(brush);
}

void MyBinnedGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QPoint currentPos = event->pos();
    // Display the current pixel coordinates under the cursor
    QPointF currentPoint = mapToScene(currentPos);
    emit currentMousePos(currentPoint);

    // Also, if the right mouse button is pressed while moving,
    // adjust brightness and contrast
    if (rightButtonPressed) {
        emit rightDragTravelled(rightDragStartPos - currentPos);
    }
    if (middleButtonPressed) {
        //        emit middleDragTravelled(previousMousePoint - currentPos);
        //        emit middleDragTravelled(currentPos);
        if (middleMouseMode == "DragMode") {
            if (_pan) {
                horizontalScrollBar()->setValue(horizontalScrollBar()->value() - 2.*(event->x() - _panStartX));
                verticalScrollBar()->setValue(verticalScrollBar()->value() - 2.*(event->y() - _panStartY));
                _panStartX = event->x();
                _panStartY = event->y();
                //    event->accept();
                //    return;
            }
        }
    }
    if (leftButtonPressed) {
        QPointF pointStart = mapToScene(leftDragStartPos.x(), leftDragStartPos.y());
        emit leftDragTravelled(pointStart, currentPoint);
    }
    QGraphicsView::mouseMoveEvent(event);
}

void MyBinnedGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        rightDragStartPos = event->pos();
        rightButtonPressed = true;
        emit rightPress();
    }
    if (event->button() == Qt::LeftButton) {
        leftDragStartPos = event->pos();
        QPointF pointStart = mapToScene(leftDragStartPos.x(), leftDragStartPos.y());
        leftButtonPressed = true;
        emit leftPress(pointStart);
    }
    if (event->button() == Qt::MiddleButton) {
        //        setDragMode(QGraphicsView::ScrollHandDrag);
        //        previousMousePoint = event->pos();
        if (middleMouseMode == "DragMode") {
            middleButtonPressed = true;
            _pan = true;
            _panStartX = event->x();
            _panStartY = event->y();
            setCursor(Qt::OpenHandCursor);
            //    event->accept();
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void MyBinnedGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        rightButtonPressed = false;
        emit rightButtonReleased();
    }
    if (event->button() == Qt::LeftButton) {
        leftButtonPressed = false;
        emit leftButtonReleased();
    }
    if (event->button() == Qt::MiddleButton) {
        middleButtonPressed = false;
        if (middleMouseMode == "DragMode") {
            _pan = false;
            setCursor(Qt::ArrowCursor);
            //        event->accept();
            //      return;
        }
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void MyBinnedGraphicsView::updateMiddleMouseMode(QString mode)
{
    middleMouseMode = mode;
}

