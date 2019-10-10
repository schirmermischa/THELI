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
    QPointF middleDragStartPos;
    QPointF middleDragCurrentPos;
    bool rightButtonPressed = false;
    bool leftButtonPressed = false;
    bool middleButtonPressed = false;
    QPoint previousMousePoint;

    bool _pan = false;
    int _panStartX, _panStartY;

public:
    explicit MyGraphicsView(QWidget *parent = nullptr);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    QScrollBar *sxbar;
    QScrollBar *sybar;
    QString middleMouseMode = "DragMode";

    int x;
    int y;

signals:
    void currentMousePos(QPointF);
    void rightDragTravelled(QPointF);
    void middleDragTravelled(QPointF pointStart, QPointF pointEnd);
    void leftDragTravelled(QPointF pointStart, QPointF pointEnd);
    void rightPress();
    void leftPress(QPointF pointStart);
    void middlePress(QPointF point);
    void leftButtonReleased();
    void rightButtonReleased();
    void middleButtonReleased();

public slots:
    void updateMiddleMouseMode(QString mode);
};

#endif // MYGRAPHICSVIEW_H
