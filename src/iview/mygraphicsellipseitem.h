#ifndef MYGRAPHICSELLIPSEITEM_H
#define MYGRAPHICSELLIPSEITEM_H

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsEllipseItem>

class MyGraphicsEllipseItem : public QGraphicsEllipseItem
{
public:
    MyGraphicsEllipseItem();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MYGRAPHICSELLIPSEITEM_H
