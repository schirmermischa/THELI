#include "mygraphicsellipseitem.h"
#include <QGraphicsSceneMouseEvent>

#include <QGraphicsEllipseItem>

MyGraphicsEllipseItem::MyGraphicsEllipseItem()
{
    this->setAcceptedMouseButtons(Qt::LeftButton);
}

void MyGraphicsEllipseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        this->setSelected(true);
    }
    QGraphicsEllipseItem::mousePressEvent(event);
}
