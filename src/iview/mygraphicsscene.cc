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

#include "mygraphicsscene.h"

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QList>
#include <QDebug>

MyGraphicsScene::MyGraphicsScene()
{

}

// Override key event to delete selected items with the backspace or delete key
void MyGraphicsScene::keyReleaseEvent(QKeyEvent * keyEvent)
{
    if(keyEvent->key() == Qt::Key_Backspace || keyEvent->key() == Qt::Key_Delete)
    {
        QList<QGraphicsItem*> selectedItems = this->selectedItems(); // get list of selected items
        foreach(QGraphicsItem* item, selectedItems)
        {
            removeItem(item);
            delete item;
            item = nullptr;
        }
        emit itemDeleted();
    }
}

void MyGraphicsScene::leaveEvent(QEvent *event)
{
    if (event->type() == QEvent::Leave) {
        emit mouseLeftScene();
        QGraphicsScene::event(event);
    }
}

void MyGraphicsScene::addCrosshair(double x, double y)
{
    removeCrosshair();

    QPen pen(QColor("#00ffaa"));
    pen.setWidth(0);
    QPen penBlack(QColor("#000000"));
    penBlack.setWidth(0);

    double width = 20/zoomScale;
    double widthBlack = (width - 2/zoomScale);
    crosshairCircleItem = addEllipse(x-width/2, y-width/2, width, width, pen);
    crosshairCircleBlackItem = addEllipse(x-widthBlack/2, y-widthBlack/2, widthBlack, widthBlack, penBlack);
    crosshairLineNItem = addLine(x, y-width/2, x, y-width, pen);
    crosshairLineSItem = addLine(x, y+width/2, x, y+width, pen);
    crosshairLineEItem = addLine(x-width/2, y, x-width, y, pen);
    crosshairLineWItem = addLine(x+width/2, y, x+width, y, pen);
    crosshairShown = true;
}

void MyGraphicsScene::removeCrosshair()
{
    if (crosshairCircleItem != nullptr) {
        removeItem(crosshairCircleItem);
        crosshairCircleItem = nullptr;
    }
    if (crosshairCircleBlackItem != nullptr) {
        removeItem(crosshairCircleBlackItem);
        crosshairCircleBlackItem = nullptr;
    }
    if (crosshairLineNItem != nullptr) {
        removeItem(crosshairLineNItem);
        crosshairLineNItem = nullptr;
    }
    if (crosshairLineSItem != nullptr) {
        removeItem(crosshairLineSItem);
        crosshairLineSItem = nullptr;
    }
    if (crosshairLineEItem != nullptr) {
        removeItem(crosshairLineEItem);
        crosshairLineEItem = nullptr;
    }
    if (crosshairLineWItem != nullptr) {
        removeItem(crosshairLineWItem);
        crosshairLineWItem = nullptr;
    }

    crosshairShown = false;
}
