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

#ifndef IVCONFDOCKWIDGET_H
#define IVCONFDOCKWIDGET_H

#include "../mybinnedgraphicsview.h"
#include "../mymagnifiedgraphicsview.h"
#include "../mygraphicsscene.h"
#include <QDockWidget>

namespace Ui {
class IvConfDockWidget;
}

class IView;  // Forward declaration to access members

class IvConfDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit IvConfDockWidget(IView *parent = 0);
    ~IvConfDockWidget();
    Ui::IvConfDockWidget *ui;

    IView *iview;

    MyMagnifiedGraphicsView *magnifiedGraphicsView;
    MyBinnedGraphicsView *binnedGraphicsView;
    MyGraphicsScene *magnifiedScene = new MyGraphicsScene();
    MyGraphicsScene *binnedScene = new MyGraphicsScene();

    int navigator_nx = 0;   // the width and height of the magnifyer window;
    int navigator_ny = 0;

    QRect currentFov;       // the currently displayed field of view
    QList<QGraphicsRectItem*> currentFovList;

    void switchMode(QString mode);
    double zoom2scale(int zoomlevel);

private slots:
    void on_zoomOutPushButton_clicked();
    void on_zoomInPushButton_clicked();
    void on_minLineEdit_returnPressed();
    void on_maxLineEdit_returnPressed();
    void on_autocontrastPushButton_toggled(bool checked);
    void on_quitPushButton_clicked();

public slots:
    void on_zoomFitPushButton_clicked();
    void on_zoomZeroPushButton_clicked();
    void updateNavigatorMagnifiedReceived(QGraphicsPixmapItem *magnifiedPixmapItem, qreal magnification, float dx, float dy);
    void updateNavigatorBinnedReceived(QGraphicsPixmapItem *binnedPixmapItem);
    void mouseEnteredViewReceived();
    void mouseLeftViewReceived();
    void updateNavigatorBinnedViewportReceived(QRect rect);
    void updateNavigatorMagnifiedViewportReceived();

signals:
    void autoContrastPushButton_toggled(bool checked);
    void minmaxLineEdit_returnPressed(QString rangeMin, QString rangeMax);
    void zoomFitPushButton_clicked(bool checked);
    void zoomInPushButton_clicked();
    void zoomOutPushButton_clicked();
    void zoomZeroPushButton_clicked();
    void closeIview();
};

#endif // IVCONFDOCKWIDGET_H
