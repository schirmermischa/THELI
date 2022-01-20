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

#include "iview.h"
#include "ui_iview.h"

#include "ui_ivredshiftdockwidget.h"
#include "ui_ivconfdockwidget.h"

#include "../functions.h"
#include "mygraphicsview.h"
#include "mygraphicsscene.h"

#include <QTransform>

void IView::updateAxesHelper()
{
    observedAxis.init(currentMyImage->wcs->crval[0], currentMyImage->wcs->cd[0], currentMyImage->naxis1, currentMyImage->naxis2, 75, "down", QColor("#00aaff"));
    restframeAxis.init(currentMyImage->wcs->crval[0], currentMyImage->wcs->cd[0], currentMyImage->naxis1, currentMyImage->naxis2, 25, "down", QColor("#ffdd00"));
    spectrumAxis.init(currentMyImage->wcs->crval[0], currentMyImage->wcs->cd[0], currentMyImage->naxis1, currentMyImage->naxis2, -25, "down", QColor("#ffdd00"), "spectrum");
//    observedAxis.init(currentMyImage->crval1, currentMyImage->header., currentMyImage->naxis1, currentMyImage->naxis2, 75, "down", QColor("#00aaff"));
//    restframeAxis.init(currentMyImage->crval1, currentMyImage->wcs->cd[0], currentMyImage->naxis1, currentMyImage->naxis2, 25, "down", QColor("#ffdd00"));
//    spectrumAxis.init(currentMyImage->crval1, currentMyImage->wcs->cd[0], currentMyImage->naxis1, currentMyImage->naxis2, -25, "down", QColor("#ffdd00"), "spectrum");

    // Populate the spectrum axis with emission lines
    spectrumAxis.tickLength = 20.;
    if (zdw->ui->HCheckBox->isChecked())  spectrumAxis.addSpecies(zdw->HList);
    if (zdw->ui->NCheckBox->isChecked())  spectrumAxis.addSpecies(zdw->NList);
    if (zdw->ui->OCheckBox->isChecked())  spectrumAxis.addSpecies(zdw->OList);
    if (zdw->ui->SCheckBox->isChecked())  spectrumAxis.addSpecies(zdw->SList);
    if (zdw->ui->CCheckBox->isChecked())  spectrumAxis.addSpecies(zdw->CList);
    if (zdw->ui->HeCheckBox->isChecked()) spectrumAxis.addSpecies(zdw->HeList);
    if (zdw->ui->ArCheckBox->isChecked()) spectrumAxis.addSpecies(zdw->ArList);
    if (zdw->ui->NeCheckBox->isChecked()) spectrumAxis.addSpecies(zdw->NeList);
    if (zdw->ui->MgCheckBox->isChecked()) spectrumAxis.addSpecies(zdw->MgList);
    if (zdw->ui->NaCheckBox->isChecked()) spectrumAxis.addSpecies(zdw->NaList);
    if (zdw->ui->CaCheckBox->isChecked()) spectrumAxis.addSpecies(zdw->CaList);
    if (zdw->ui->FeCheckBox->isChecked()) spectrumAxis.addSpecies(zdw->FeList);
}

void IView::updateAxes()
{
    if (!zdw->isVisible()) return;

    // This slot is invoked (among others) everytime a new image is loaded
    clearAxes();
    updateAxesHelper();

    // Do not show axes if redshift Dockwidget is not visible
    if (!zdw->isVisible()) return;
    showAxes();
}

// Invoked e.g. after right-click dragging. Must not redraw graphics scene, as this is done in main iView
void IView::redrawUpdateAxes()
{
    if (!zdw->isVisible()) return;

    updateAxesHelper();
    showAxesHelper(observedAxisLineItems, observedAxisMainLineItems, observedAxisTextItems, observedAxis, "observed");
    showAxesHelper(restframeAxisLineItems, restframeAxisMainLineItems, restframeAxisTextItems, restframeAxis, "restframe");
    showAxesHelper(spectrumAxisLineItems, spectrumAxisMainLineItems, spectrumAxisTextItems, spectrumAxis, "spectrum");
}

void IView::showAxes()
{
    if (!zdw->isVisible()) return;

    showAxesHelper(observedAxisLineItems, observedAxisMainLineItems, observedAxisTextItems, observedAxis, "observed");
    showAxesHelper(restframeAxisLineItems, restframeAxisMainLineItems, restframeAxisTextItems, restframeAxis, "restframe");
    showAxesHelper(spectrumAxisLineItems, spectrumAxisMainLineItems, spectrumAxisTextItems, spectrumAxis, "spectrum");

    myGraphicsView->setScene(scene);
    myGraphicsView->show();
}

void IView::showAxesHelper(QList<QGraphicsLineItem*> &lineItemList, QList<QGraphicsLineItem*> &mainLineItemList, QList<QGraphicsTextItem*> &textItemList, const MyAxis &axis, QString type)
{
    // Remove all previously displayed items
    // Tickmarks
    if (!lineItemList.isEmpty()) {
        for (auto &it: lineItemList) scene->removeItem(it);
        lineItemList.clear();
    }
    // Main axis
    if (!mainLineItemList.isEmpty()) {
        for (auto &it: mainLineItemList) scene->removeItem(it);
        mainLineItemList.clear();
    }

    // Add the main axis line if requested (not for the emission line tickmarks)
    if (type != "spectrum") mainLineItemList.append(scene->addLine(axis.axisLine));

    for (auto &it: axis.tickMarks) {
        lineItemList.append(scene->addLine(it));
    }
    for (auto &it: lineItemList) {
        it->setPen(axis.pen);
    }
    for (auto &it: mainLineItemList) {
        it->setPen(axis.pen);
    }

    // Add the labels; this is in the same order as the line items, so the matching is preserved
    for (auto &it : axis.tickLabels) {
        textItemList.append(scene->addText(it));
    }
    // Setup the font
    QFont currentFont = this->font();
    qreal rescale;
    if (!icdw->ui->zoomFitPushButton->isChecked()) rescale = icdw->zoom2scale(zoomLevel);
    else rescale = myGraphicsView->transform().m11();
    currentFont.setPointSize(int(float(currentFont.pointSize())/rescale));
    int i=0;
    QTransform transform;
    transform.rotate(-45);
    for (auto &it : textItemList) {
        it->setFont(currentFont);
        QString plainText = it->toPlainText();
        // use a semi-transparent background
        if (type != "observed") it->setHtml(QString("<div style='background:rgba(255, 220, 0, 50%);'>" + QString("&nbsp;") + plainText + QString("&nbsp;") + QString("</div>") ));
        else it->setHtml(QString("<div style='background:rgba(0, 200, 255, 50%);'>" + QString("&nbsp;") + plainText + QString("&nbsp;") + QString("</div>") ));
        it->setDefaultTextColor(QColor("#000000"));
        qreal xpos = lineItemList.at(i)->line().x1();
        qreal ypos = lineItemList.at(i)->line().y1();
        ++i;
        int offset = 0;
        if (type == "observed") offset = 5;
        if (type == "restframe") offset = 5;
        if (type == "spectrum") offset = -10;
        it->setPos(xpos-20/icdw->zoom2scale(zoomLevel), ypos+offset/icdw->zoom2scale(zoomLevel));
        if (type == "spectrum") it->setTransform(transform);
    }
}

void IView::clearAxes()
{
    if (!zdw->isVisible()) return;

    observedAxis.clear();
    restframeAxis.clear();
    spectrumAxis.clear();

    clearAxesHelper(observedAxisLineItems, observedAxisMainLineItems, observedAxisTextItems);
    clearAxesHelper(restframeAxisLineItems, restframeAxisMainLineItems, restframeAxisTextItems);
    clearAxesHelper(spectrumAxisLineItems, spectrumAxisMainLineItems, spectrumAxisTextItems);

    myGraphicsView->setScene(scene);
    myGraphicsView->show();
}

void IView::clearAxesHelper(QList<QGraphicsLineItem*> &lineItemList, QList<QGraphicsLineItem*> &mainLineItemList, QList<QGraphicsTextItem*> &textItemList)
{
    // Remove all previously displayed items
    if (!lineItemList.isEmpty()) {
        for (auto &it: lineItemList) scene->removeItem(it);
        lineItemList.clear();
    }
    if (!mainLineItemList.isEmpty()) {
        for (auto &it: mainLineItemList) scene->removeItem(it);
        mainLineItemList.clear();
    }
    if (!textItemList.isEmpty()) {
        for (auto &it: textItemList) scene->removeItem(it);
        textItemList.clear();
    }
}

// Sends the wavelength under the cursor to redshiftDockWidget
void IView::showWavelength(QPointF point)
{
    if (!zdw->isVisible()) return;

    // This function receives the image coords from a mouseMoveEvent
    // and translates them to wavelengths

    // constant offsets required to show same coordinates as in skycat / ds9
    float x_cursor = point.x() + 0.5;

    double lambdaObs = observedAxis.pixelToWavelength(x_cursor);
    double lambdaRest = restframeAxis.pixelToWavelength(x_cursor);

    emit wavelengthUpdated(QString::number(lambdaObs, 'f', 1), QString::number(lambdaRest, 'f', 1));
}

void IView::updateFinesse(int value)
{
    restframeAxis.finesse = 2000.*pow(2,value-5)/(1.+restframeAxis.z);
    spectrumAxis.finesse = 2000.*pow(2,value-5)/(1.+restframeAxis.z);
}

void IView::resetRedshift()
{
    restframeAxis.z = 0.;
    restframeAxis.z_0 = 0.;
    spectrumAxis.z = 0.;
    spectrumAxis.z_0 = 0.;
    zdw->ui->zLineEdit->setText("0.000");
}

void IView::redshiftChanged(QString text)
{
    float z = 0.;
    if (text.isEmpty()) z = 0.;
    else z = text.toFloat();
    restframeAxis.z = z;
    spectrumAxis.z = z;
    restframeAxis.z_0 = z;
    spectrumAxis.z_0 = z;
    updateAxes();
}
