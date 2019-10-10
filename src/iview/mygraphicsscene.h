#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QKeyEvent>

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MyGraphicsScene(QWidget *parent = nullptr);

signals:
    void itemDeleted();

protected:
    void keyReleaseEvent(QKeyEvent * keyEvent);
};

#endif // MYGRAPHICSSCENE_H
