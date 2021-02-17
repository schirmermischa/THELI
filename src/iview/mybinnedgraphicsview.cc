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
#include "wcs.h"

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

    if (leftButtonPressed) {
        // scroll main view only if rectangle is within binnedPixMap
        qreal x1;
        qreal x2;
        qreal y1;
        qreal y2;
        binnedSceneRect.getCoords(&x1, &y1, &x2, &y2);

        qreal halfSceneWidth = (x2-x1) / 2.;
        qreal halfSceneHeight = (y2-y1) / 2.;

        qreal xcen = fovRectItem->scenePos().x() + nx / 2.;
        qreal ycen = fovRectItem->scenePos().y() + ny / 2.;
        qreal xcen1 = fovRectItem->scenePos().x() + halfSceneWidth;
        qreal ycen1 = fovRectItem->scenePos().y() + halfSceneHeight;
        qreal xr1 = xcen1 - fovRectItem->rect().width() / 2.;
        qreal xr2 = xcen1 + fovRectItem->rect().width() / 2.;
        qreal yr1 = ycen1 - fovRectItem->rect().height() / 2.;
        qreal yr2 = ycen1 + fovRectItem->rect().height() / 2.;
        qreal vxmin = x1 <= x2 ? x1 : x2;
        qreal vxmax = x1 >= x2 ? x1 : x2;
        qreal vymin = y1 <= y2 ? y1 : y2;
        qreal vymax = y1 >= y2 ? y1 : y2;
        if (xr1 > vxmin && xr2 < vxmax && yr1 > vymin && yr2 < vymax) {
            fovBeingDragged = true;
            fovRectItem->setFlags(QGraphicsEllipseItem::ItemIsMovable);
            QPoint rectCenter = QPoint(xcen, ycen);
            QPointF rectCenterQimage = mapToScene(rectCenter);
            emit fovRectCenterChanged(rectCenterQimage);
        }
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
        fovBeingDragged = false;
//        old_x = fovRectItem->scenePos().x() + nx/2.;
//        old_y = fovRectItem->scenePos().y() + ny/2.;
//        qDebug() << "OLD" << old_x << old_y;
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
