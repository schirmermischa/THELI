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

#include <QPainter>

#include <math.h>

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
//    binnedGraphicsView->setRenderHint(QPainter::Antialiasing);
    //    ui->navigatorStackedWidget->setContentsMargins(0,0,0,0);
    ui->binnedGraphicsLayout->insertWidget(0, binnedGraphicsView);
    ui->magnifiedGraphicsLayout->insertWidget(1, magnifiedGraphicsView);
    navigator_binned_nx = ui->binnedFrame->width();
    navigator_binned_ny = ui->binnedFrame->height();
    navigator_magnified_nx = ui->magnifiedFrame->width();
    navigator_magnified_ny = ui->magnifiedFrame->height();
    binnedGraphicsView->nx = navigator_binned_nx;
    binnedGraphicsView->ny = navigator_binned_ny;

    ui->quitPushButton->hide();

    connect(binnedGraphicsView, &MyBinnedGraphicsView::currentMousePos, this, &IvConfDockWidget::showCurrentMousePosBinned);
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

// receiver from mouse pos hovering in binned vew
void IvConfDockWidget::showCurrentMousePosBinned(QPointF currentMousePos)
{
    QPointF qpoint = iview->binnedToQimage(currentMousePos);
    long i = qpoint.x();
    long j = iview->naxis2 - qpoint.y();
    ui->xposLabel->setText("x = "+QString::number(i));
    ui->yposLabel->setText("y = "+QString::number(j));
    // display sky coords
    iview->xy2sky(qpoint.x(), iview->naxis2 - qpoint.y());

    // Display pixel values: respect image boundaries
    if (i<0 || j<0 || i>= iview->naxis1 || j>= iview->naxis2) {
        if (iview->displayMode == "FITSmonochrome" || iview->displayMode == "MEMview") {
            ui->valueLabel->setText("Value = ");
        }
        else if (iview->displayMode == "FITScolor") {
            ui->valueLabel->setText("Value R = ");
            ui->valueGreenLabel->setText("Value G = ");
            ui->valueBlueLabel->setText("Value B = ");
        }
    }
    else {
        if (iview->displayMode == "FITSmonochrome" || iview->displayMode == "MEMview") {
            ui->valueLabel->setText("Value = "+QString::number(iview->fitsData[i+iview->naxis1*j]));
        }
        else if (iview->displayMode == "FITScolor") {
            ui->valueLabel->setText("Value R = "+QString::number(iview->fitsDataR[i+iview->naxis1*j]));
            ui->valueGreenLabel->setText("Value G = "+QString::number(iview->fitsDataG[i+iview->naxis1*j]));
            ui->valueBlueLabel->setText("Value B = "+QString::number(iview->fitsDataB[i+iview->naxis1*j]));
        }
    }
}

void IvConfDockWidget::updateNavigatorMagnifiedReceived(QGraphicsPixmapItem *magnifiedPixmapItem, qreal magnification, float dx, float dy)
{
    magnifiedGraphicsView->resetMatrix();
    magnifiedScene->clear();
    magnifiedScene->addItem(magnifiedPixmapItem);
    magnifiedGraphicsView->setScene(magnifiedScene);
    magnifiedGraphicsView->scale(magnification, magnification);
    magnifiedGraphicsView->centerOn(magnifiedPixmapItem);
    QPen pen(QColor("#00ffff"));
    pen.setCosmetic(true);
    float x1 = navigator_magnified_nx / magnification / 2.;
    float y1 = navigator_magnified_ny / magnification / 2.;
    QPoint p1 = QPoint(x1, y1);
    QPoint p2 = QPoint(x1, y1);

    // dx and dy intended to realign pixmap when cursor moves out of image boundary, but it is not working.
    //    magnifiedGraphicsView->centerOn(x1+dx, y1+dy);
    magnifiedScene->addRect(QRect(p1,p2), pen);
    magnifiedGraphicsView->show();
    //    ui->navigatorStackedWidget->setCurrentIndex(1);
}

void IvConfDockWidget::clearMagnifiedSceneReceiver()
{
    magnifiedScene->clear();
    magnifiedGraphicsView->show();
}

void IvConfDockWidget::updateNavigatorBinnedReceived(QGraphicsPixmapItem *binnedPixmapItem)
{
    //    binnedGraphicsView->resetMatrix();
    binnedScene->clear();
    binnedScene->addItem(binnedPixmapItem);
    // needed to prevent binnedView from scrolling
    binnedScene->setSceneRect(binnedPixmapItem->boundingRect());
    // panning movement restricted to fov, half offset, if the following line is commented. Unclear why
    binnedGraphicsView->binnedSceneRect = binnedScene->sceneRect();

    binnedGraphicsView->setScene(binnedScene);
    binnedGraphicsView->show();
}

// Receiver for the event when the mouse enters the main graphics view
void IvConfDockWidget::mouseEnteredViewReceived()
{
    //    ui->navigatorStackedWidget->setCurrentIndex(1);
}

// Receiver for the event when the mouse leaves the main graphics view
void IvConfDockWidget::mouseLeftViewReceived()
{
    // when the mouse cursor leaves the scene
    magnifiedScene->clear();
    magnifiedGraphicsView->show();
    //    ui->navigatorStackedWidget->setCurrentIndex(0);
}

// whenever the user scrolls or zooms in the main window , this slot is invoked
void IvConfDockWidget::updateNavigatorBinnedViewportReceived(QRect rect)
{
    // leave if the user scrolls by dragging the fov rectangle in the binned window
    if (binnedGraphicsView->fovBeingDragged) return;

    // delete the old display of the rect
    QList<QGraphicsItem*> items = binnedScene->items();
    for (auto &it : items) {
        QGraphicsRectItem *rect = qgraphicsitem_cast<QGraphicsRectItem*>(it);
        if (rect) binnedScene->removeItem(it);
    }

    // draw the new rect
    QPen pen(QColor("#00ffff"));
    pen.setCosmetic(true);
    pen.setWidth(0);
    binnedGraphicsView->fovRectItem = new MyQGraphicsRectItem(rect, nullptr);
    binnedGraphicsView->fovRectItem->setPen(pen);
    binnedGraphicsView->fovRectItem->setFlags(QGraphicsEllipseItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
    binnedScene->addItem(binnedGraphicsView->fovRectItem);
//    binnedGraphicsView->fovRectItem->setFlags(QGraphicsEllipseItem::ItemIsMovable | QGraphicsRectItem::ItemSendsGeometryChanges);
//    binnedGraphicsView->fovRectItem->setFlags(QGraphicsRectItem::ItemSendsGeometryChanges);

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
    int x1 = navigator_magnified_nx/2-0.5;
    int x2 = navigator_magnified_nx/2+0.5;
    int y1 = navigator_magnified_ny/2-0.5;
    int y2 = navigator_magnified_ny/2+0.5;
    QPoint p1 = magnifiedGraphicsView->mapFromScene(QPoint(x1, y1));
    QPoint p2 = magnifiedGraphicsView->mapFromScene(QPoint(x2, y2));
    magnifiedScene->addRect(QRect(p1, p2), pen);
    magnifiedGraphicsView->setScene(magnifiedScene);
    magnifiedGraphicsView->show();
}

void IvConfDockWidget::receiveCDmatrix(double* cd)
{
    cd11 = cd[0];
    cd12 = cd[1];
    cd21 = cd[2];
    cd22 = cd[3];

    if (cd11 == 0. && cd12 == 0. && cd21 == 0. && cd22 == 0.) return;

    drawCompass();
}

void IvConfDockWidget::drawCompass()
{
    // delete old compass elements
    QList<QGraphicsItem*> items = binnedScene->items();
    for (auto &it : items) {
        QGraphicsLineItem *line = qgraphicsitem_cast<QGraphicsLineItem*>(it);
        if (line) binnedScene->removeItem(it);
        QGraphicsTextItem *text = qgraphicsitem_cast<QGraphicsTextItem*>(it);
        if (text) binnedScene->removeItem(it);
    }

    double norm = sqrt(cd11*cd11+cd12*cd12);
    double length = navigator_binned_nx / 5.;

    QPointF start = iview->qimageToBinned(QPointF(iview->naxis1/2, iview->naxis2/2));

    QPen pen(QColor("#ffff00"));
    pen.setCosmetic(true);
    pen.setWidth(0);

    // Arrows
    QPointF eastEnd;
    QPointF northEnd;
    eastEnd.setX(start.x() + cd11/norm * length);
    eastEnd.setY(start.y() - cd12/norm * length);    // y axis is inverted in QImage wrt FITS, hence -cd12
    northEnd.setX(start.x() + cd21/norm * length);
    northEnd.setY(start.y() - cd22/norm * length);   // y axis is inverted in QImage wrt FITS, hence -cd22

    QLineF eastArrow = QLineF(start, eastEnd);
    QLineF northArrow = QLineF(start, northEnd);
    binnedScene->addLine(eastArrow, pen);
    binnedScene->addLine(northArrow, pen);

    // Labels
    QGraphicsTextItem *labelEast = binnedScene->addText("E");
    QGraphicsTextItem *labelNorth = binnedScene->addText("N");
    labelEast->setDefaultTextColor(QColor("#ffff00"));
    labelNorth->setDefaultTextColor(QColor("#ffff00"));
    int exoffset = labelEast->boundingRect().width()/2;
    int eyoffset = labelEast->boundingRect().height()/2;
    int nxoffset = labelNorth->boundingRect().width()/2;
    int nyoffset = labelNorth->boundingRect().height()/2;
    int exspace = 0.3*(eastEnd.x() - start.x());
    int eyspace = 0.3*(eastEnd.y() - start.y());
    int nxspace = 0.3*(northEnd.x() - start.x());
    int nyspace = 0.3*(northEnd.y() - start.y());
    labelEast->setPos(eastEnd.x()-exoffset+exspace, eastEnd.y()-eyoffset+eyspace);
    labelNorth->setPos(northEnd.x()-nxoffset+nxspace, northEnd.y()-nyoffset+nyspace);

    binnedGraphicsView->show();
}
