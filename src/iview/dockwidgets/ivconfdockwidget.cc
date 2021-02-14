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

#include "ivconfdockwidget.h"
#include "ui_ivconfdockwidget.h"
#include "../iview.h"

IvConfDockWidget::IvConfDockWidget(IView *parent) :
    QDockWidget(parent),
    ui(new Ui::IvConfDockWidget)
{
    ui->setupUi(this);
    iview = parent;

    // Populate the navigator widget
    QPalette backgroundPalette;
    backgroundPalette.setColor(QPalette::Base, QColor("#000000"));
    magnifiedGraphicsView = new MyMagnifiedGraphicsView();
    magnifiedGraphicsView->setPalette(backgroundPalette);
    magnifiedGraphicsView->setMouseTracking(true);
    magnifiedGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    magnifiedGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    binnedGraphicsView = new MyBinnedGraphicsView();
    binnedGraphicsView->setPalette(backgroundPalette);
    binnedGraphicsView->setMouseTracking(true);
    binnedGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    binnedGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    ui->navigatorStackedWidget->setContentsMargins(0,0,0,0);
    ui->binnedGraphicsLayout->insertWidget(0, binnedGraphicsView);
    ui->magnifiedGraphicsLayout->insertWidget(1, magnifiedGraphicsView);
    navigator_nx = ui->binnedFrame->width();
    navigator_ny = ui->binnedFrame->height();

    ui->quitPushButton->hide();

//    connect(magnifyGraphicsView, &MyBinnedGraphicsView::currentMousePos, this, &IvConfDockWidget::showCurrentMousePos);
//    connect(magnifyGraphicsView, &MyBinnedGraphicsView::leftDragTravelled, this, &IView::drawSeparationVector);
//    connect(magnifyGraphicsView, &MyBinnedGraphicsView::leftPress, this, &IView::initSeparationVector);
//    connect(magnifyGraphicsView, &MyBinnedGraphicsView::leftPress, this, &IView::sendStatisticsCenter);
//    connect(magnifyGraphicsView, &MyBinnedGraphicsView::leftButtonReleased, this, &IView::clearSeparationVector);
//    connect(magnifyGraphicsView, &MyBinnedGraphicsView::leftButtonReleased, this, &IView::updateSkyCircles);
}

IvConfDockWidget::~IvConfDockWidget()
{
    delete ui;
}

void IvConfDockWidget::switchMode(QString mode)
{
    if (mode == "FITSmonochrome" || mode == "MEMview") {
        ui->valueGreenLabel->hide();
        ui->valueBlueLabel->hide();
    }
    else if (mode == "FITScolor") {
        ui->valueGreenLabel->show();
        ui->valueBlueLabel->show();
    }
    else if (mode == "CLEAR") {
        ui->xposLabel->setText("x = ");
        ui->yposLabel->setText("y = ");
        ui->alphaDecLabel->setText("R.A. = ");
        ui->alphaHexLabel->setText("R.A. = ");
        ui->deltaDecLabel->setText("Dec  = ");
        ui->deltaHexLabel->setText("Dec  = ");
        ui->valueLabel->setText("Value = ");
//        ui->zoomValueLabel->setText("");
        ui->valueGreenLabel->hide();
        ui->valueBlueLabel->hide();
    }
}

double IvConfDockWidget::zoom2scale(int zoomlevel)
{
    // Translates the integer zoom level to a scaling factor.
    // Update the zoom label
    if (zoomlevel > 0) {
//        ui->zoomValueLabel->setText(QString::number(zoomlevel+1)+":1");
        return zoomlevel+1.;
    }
    else if (zoomlevel == 0) {
//        ui->zoomValueLabel->setText("1:1");
        return 1.;
    }
    else {
//        ui->zoomValueLabel->setText("1:"+QString::number(-zoomlevel+1));
        return 1./(-zoomlevel+1.);
    }
}

void IvConfDockWidget::on_zoomInPushButton_clicked()
{
    ui->zoomFitPushButton->setChecked(false);
    emit zoomInPushButton_clicked();
}

void IvConfDockWidget::on_zoomOutPushButton_clicked()
{
    ui->zoomFitPushButton->setChecked(false);
    emit zoomOutPushButton_clicked();
}

void IvConfDockWidget::on_zoomZeroPushButton_clicked()
{
    ui->zoomFitPushButton->setChecked(false);
    emit zoomZeroPushButton_clicked();
}

void IvConfDockWidget::on_zoomFitPushButton_clicked()
{
    emit zoomFitPushButton_clicked(ui->zoomFitPushButton->isChecked());
}

void IvConfDockWidget::on_minLineEdit_returnPressed()
{
    ui->autocontrastPushButton->setChecked(false);
    emit minmaxLineEdit_returnPressed(ui->minLineEdit->text(), ui->maxLineEdit->text());
}

void IvConfDockWidget::on_maxLineEdit_returnPressed()
{
    //QPixmap magnifiedPixmap = QPixmap("/home/mischa/euclid_logo.png");
    //icdw->magnifiedGraphicsView->resetMatrix();
    //QGraphicsPixmapItem *newItem = new QGraphicsPixmapItem(magnifiedPixmap);
    ui->autocontrastPushButton->setChecked(false);
    emit minmaxLineEdit_returnPressed(ui->minLineEdit->text(), ui->maxLineEdit->text());
}

void IvConfDockWidget::on_autocontrastPushButton_toggled(bool checked)
{
    emit autoContrastPushButton_toggled(checked);
}

void IvConfDockWidget::on_quitPushButton_clicked()
{
    emit closeIview();
}

void IvConfDockWidget::updateNavigatorMagnifiedReceived(QGraphicsPixmapItem *magnifiedPixmapItem, qreal magnification)
{
    magnifiedGraphicsView->resetMatrix();
    magnifiedScene->clear();
    magnifiedScene->addItem(magnifiedPixmapItem);
    magnifiedGraphicsView->setScene(magnifiedScene);
    magnifiedGraphicsView->scale(magnification, magnification);
    magnifiedGraphicsView->centerOn(magnifiedPixmapItem);
    QPen pen(QColor("#00ffff"));
    pen.setCosmetic(true);
//    int x1 = magnifiedScene->width()/2+1;//-0.5;
//    int x2 = magnifiedScene->width()/2+1;//+0.5;
//    float x1 = magnifiedScene->width()/2.+1.;
//    float x2 = magnifiedScene->width()/2.+1.;
//    float y1 = magnifiedScene->height()/2.+1.;//-0.5;
//    float y2 = magnifiedScene->height()/2.+1.;//+0.5;
    float x1 = magnifiedScene->width()/2.;
    float x2 = magnifiedScene->width()/2.;
    float y1 = magnifiedScene->height()/2.;//-0.5;
    float y2 = magnifiedScene->height()/2.;//+0.5;
    QPoint p1 = QPoint(x1, y1);
    QPoint p2 = QPoint(x2, y2);
//    qDebug() << x1 << x2 << y1 << y2;
    magnifiedScene->addRect(QRect(p1,p2), pen);
    magnifiedGraphicsView->show();
//    ui->navigatorStackedWidget->setCurrentIndex(1);
}

void IvConfDockWidget::updateNavigatorBinnedReceived(QGraphicsPixmapItem *binnedPixmapItem)
{
    binnedGraphicsView->resetMatrix();
    binnedScene->clear();
    binnedScene->addItem(binnedPixmapItem);
    binnedGraphicsView->setScene(binnedScene);
    magnifiedGraphicsView->centerOn(binnedPixmapItem);
    binnedGraphicsView->show();
//    ui->navigatorStackedWidget->setCurrentIndex(0);
}

// Receiver for the event when the mouse enters the main graphics view
void IvConfDockWidget::mouseEnteredViewReceived()
{
//    ui->navigatorStackedWidget->setCurrentIndex(1);
}

// Receiver for the event when the mouse leaves the main graphics view
void IvConfDockWidget::mouseLeftViewReceived()
{
//    ui->navigatorStackedWidget->setCurrentIndex(0);
}

void IvConfDockWidget::updateNavigatorBinnedViewportReceived(QRect rect)
{
    return;

    // remove old rects
    QList<QGraphicsItem*> items = binnedScene->items();
    for (auto &it : items) {
       QGraphicsRectItem *rectcast = qgraphicsitem_cast<QGraphicsRectItem*>(it);
       if (rectcast) binnedScene->removeItem(it);
    }

    QPen pen(QColor("#00ffff"));
    pen.setCosmetic(true);
    pen.setWidth(0);
    QRect rectNew;
    int x1;
    int x2;
    int y1;
    int y2;
    rect.getCoords(&x1, &x2, &y1, &y2);
    QPoint p1 = binnedGraphicsView->mapFromScene(QPoint(x1, y1));
    QPoint p2 = binnedGraphicsView->mapFromScene(QPoint(x2, y2));
    binnedScene->addRect(QRect(p1, p2), pen);

    binnedGraphicsView->setScene(binnedScene);
    binnedGraphicsView->show();
}

void IvConfDockWidget::updateNavigatorMagnifiedViewportReceived()
{
    return;

    // remove old rects
    QList<QGraphicsItem*> items = magnifiedScene->items();
    for (auto &it : items) {
       QGraphicsRectItem *rectcast = qgraphicsitem_cast<QGraphicsRectItem*>(it);
       if (rectcast) magnifiedScene->removeItem(it);
    }

    QPen pen(QColor("#aaffff"));
    pen.setCosmetic(true);
    pen.setWidth(0);
    int x1 = navigator_nx/2-0.5;
    int x2 = navigator_nx/2+0.5;
    int y1 = navigator_ny/2-0.5;
    int y2 = navigator_ny/2+0.5;
    QPoint p1 = magnifiedGraphicsView->mapFromScene(QPoint(x1, y1));
    QPoint p2 = magnifiedGraphicsView->mapFromScene(QPoint(x2, y2));
    magnifiedScene->addRect(QRect(p1, p2), pen);
    magnifiedGraphicsView->setScene(magnifiedScene);
    magnifiedGraphicsView->show();
}
