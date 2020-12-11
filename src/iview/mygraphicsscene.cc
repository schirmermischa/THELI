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
#include <QGraphicsItem>
#include <QList>

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
