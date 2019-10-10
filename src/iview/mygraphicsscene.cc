#include "mygraphicsscene.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QList>

MyGraphicsScene::MyGraphicsScene(QWidget *parent)
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
        }
        emit itemDeleted();
    }
}
