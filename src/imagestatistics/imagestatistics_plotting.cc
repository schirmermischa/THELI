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

#include "imagestatistics.h"
#include "ui_imagestatistics.h"
#include "../instrumentdata.h"
#include "../functions.h"
#include "../qcustomplot.h"
#include "../iview/iview.h"
#include "../myimage/myimage.h"

#include <QSettings>
#include <QValidator>
#include <QTest>
#include <QVector>
#include <QList>
#include <QStringList>
#include <QRegExpValidator>

void ImageStatistics::plot(QString init)
{
    if (numObj == 0 && init.isEmpty()) {
        clearAll();
        ui->statPlot->replot();
        ui->statPlot->update();
        return;
    }

    ui->statPlot->clearItems();
    ui->statPlot->clearGraphs();
    ui->statPlot->plotLayout()->clear();
    // create a sub layout that we'll place in first row:
    QCPLayoutGrid *subLayout = new QCPLayoutGrid();
    ui->statPlot->plotLayout()->addElement(0, 0, subLayout);

    QCPAxisRect *rectSky = new QCPAxisRect(ui->statPlot);
    QCPAxisRect *rectSeeing = new QCPAxisRect(ui->statPlot);
    QCPAxisRect *rectRZP = new QCPAxisRect(ui->statPlot);
    QCPAxisRect *rectAirmass = new QCPAxisRect(ui->statPlot);
    QCPAxisRect *rectEllipticity = new QCPAxisRect(ui->statPlot);
    //    rectSeeing->axis(QCPAxis::atRight)->setVisible(false);

    QList< QCPAxisRect* > rectList;
    rectList << rectSky << rectSeeing << rectRZP << rectAirmass << rectEllipticity;

    for (auto &it : rectList) {
        foreach (QCPAxis *axis, it->axes()) {
            axis->setLayer("axes");
            axis->grid()->setLayer("grid");
        }
        it->setupFullAxesBox(true);
        it->axis(QCPAxis::atBottom)->setLabel("Image #");
    }

    rectSky->axis(QCPAxis::atLeft)->setLabel("Background [ e- ]");
    rectAirmass->axis(QCPAxis::atLeft)->setLabel("Airmass");
    rectAirmass->axis(QCPAxis::atLeft)->setRangeReversed(true);
    QString unit = ui->fwhmunitsComboBox->currentText();
    if (seeingFromGaia || !seeingData) rectSeeing->axis(QCPAxis::atLeft)->setLabel("Stellar FWHM [ "+unit+" ]");
    else rectSeeing->axis(QCPAxis::atLeft)->setLabel("Median FWHM [ "+unit+" ]");

    //    rectSeeing->addAxis(QCPAxis::atRight)->setLabel("Seeing [ pixel ]");
    //    QCPAxis *seeingPixelAxis = rectSeeing->axis(QCPAxis::atRight);
    rectRZP->axis(QCPAxis::atLeft)->setLabel("Relative zeropoint [ mag ]");
    if (seeingFromGaia || !ellipticityData) rectEllipticity->axis(QCPAxis::atLeft)->setLabel("Stellar ellipticity [ % ]");
    else rectEllipticity->axis(QCPAxis::atLeft)->setLabel("Median ellipticity [ % ]");

    subLayout->addElement(0, 0, rectSky);
    subLayout->addElement(0, 1, rectAirmass);
    subLayout->addElement(0, 2, rectSeeing);
    subLayout->addElement(1, 0, rectRZP);
    subLayout->addElement(1, 1, rectEllipticity);

    // The graphs
    graphList.clear();
    skyGraph = ui->statPlot->addGraph(rectSky->axis(QCPAxis::atBottom), rectSky->axis(QCPAxis::atLeft));
    seeingGraph = ui->statPlot->addGraph(rectSeeing->axis(QCPAxis::atBottom), rectSeeing->axis(QCPAxis::atLeft));
    airmassGraph = ui->statPlot->addGraph(rectAirmass->axis(QCPAxis::atBottom), rectAirmass->axis(QCPAxis::atLeft));
    rzpGraph = ui->statPlot->addGraph(rectRZP->axis(QCPAxis::atBottom), rectRZP->axis(QCPAxis::atLeft));
    ellipticityGraph = ui->statPlot->addGraph(rectEllipticity->axis(QCPAxis::atBottom), rectEllipticity->axis(QCPAxis::atLeft));
    graphList << skyGraph << seeingGraph << airmassGraph << rzpGraph << ellipticityGraph;

    // Leave here if we are just initializing the plot panels

    if (init == "initialize") return;

    QCPSelectionDecorator *decoratorSky = new QCPSelectionDecorator();
    QCPSelectionDecorator *decoratorRZP = new QCPSelectionDecorator();
    QCPSelectionDecorator *decoratorAirmass = new QCPSelectionDecorator();
    QCPSelectionDecorator *decoratorSeeing = new QCPSelectionDecorator();
    QCPSelectionDecorator *decoratorEllipticity = new QCPSelectionDecorator();
    // change the last argument to a larger number if you want bigger data points
    QCPScatterStyle scatterstyle(QCPScatterStyle::ScatterShape::ssDisc, QColor("#ff3300"), QColor("#ff3300"), 6);
    decoratorSky->setPen(QPen(QColor("#000000")));
    decoratorSky->setScatterStyle(scatterstyle, QCPScatterStyle::spAll);
    decoratorRZP->setPen(QPen(QColor("#000000")));
    decoratorRZP->setScatterStyle(scatterstyle, QCPScatterStyle::spAll);
    decoratorSeeing->setPen(QPen(QColor("#000000")));
    decoratorSeeing->setScatterStyle(scatterstyle, QCPScatterStyle::spAll);
    decoratorAirmass->setPen(QPen(QColor("#000000")));
    decoratorAirmass->setScatterStyle(scatterstyle, QCPScatterStyle::spAll);
    decoratorEllipticity->setPen(QPen(QColor("#000000")));
    decoratorEllipticity->setScatterStyle(scatterstyle, QCPScatterStyle::spAll);
    double xmin = minVec_T(dataImageNr);
    double xmax = maxVec_T(dataImageNr);
    double dx = (xmax - xmin) * 0.05;

    QVector<double> dataPtr;

    for (auto &it : graphList) {
        it->setSelectable(QCP::stMultipleDataRanges);
        it->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, QPen(Qt::blue), QBrush(Qt::blue), 6));
        it->setLineStyle(myLineStyle);
        if (it == skyGraph) {
            dataPtr = dataSky;
            it->setSelectionDecorator(decoratorSky);
            it->setData(dataImageNr, dataPtr);
        }
        else if (it == seeingGraph) {
            float unitsRescale = 1.0;
            if (ui->fwhmunitsComboBox->currentText() == "arcsec") unitsRescale = 1.0;
            else unitsRescale = instData->pixscale;
            dataPtr = dataFWHM;
            for (auto &it : dataPtr) it /= unitsRescale;
            it->setSelectionDecorator(decoratorSeeing);
            //            if (seeingData)
            it->setData(dataImageNr, dataPtr);
            //            else {
            //                showNoData(it, 0.50, 1.80);
            //            }
        }
        else if (it == airmassGraph) {
            dataPtr = dataAirmass;
            it->setSelectionDecorator(decoratorAirmass);
            //            if (airmassData)
            it->setData(dataImageNr, dataPtr);
            //            else {
            //                showNoData(it, 1.80, 0.50);
            //            }
        }
        else if (it == rzpGraph) {
            dataPtr = dataRZP;
            it->setSelectionDecorator(decoratorRZP);
            //            if (rzpData)
            it->setData(dataImageNr, dataPtr);
            //            else {
            //                showNoData(it, 1.8, 1.8);
            //            }
        }
        else if (it == ellipticityGraph) {
            dataPtr = dataEllipticity;
            it->setSelectionDecorator(decoratorEllipticity);
            //            if (ellipticityData)
            it->setData(dataImageNr, dataPtr);
            //            else {
            //                showNoData(it, 2.0, 2.0);
            //            }
        }
        double ymin = minVec_T(dataPtr);
        double ymax = maxVec_T(dataPtr);
        double dy = (ymax - ymin) * 0.05;
        it->keyAxis()->setRange(xmin-dx, xmax+dx);
        it->valueAxis()->setRange(ymin-dy, ymax+dy);
    }
    ui->statPlot->replot();
    ui->statPlot->update();

    if (numObj > 0) statisticsDataDisplayed = true;
    else statisticsDataDisplayed = false;
}

void ImageStatistics::showNoData(QCPGraph *graph, float xpos, float ypos)
{
    QCPItemText *noData = new QCPItemText(ui->statPlot);
    noData->position->setType(QCPItemPosition::ptAxisRectRatio);
    noData->setPositionAlignment(Qt::AlignLeft|Qt::AlignCenter);
    noData->position->setCoords(xpos, ypos);
    noData->setClipToAxisRect(false);
    noData->setSelectable(false);
    noData->setText("no data");
    noData->setTextAlignment(Qt::AlignLeft);
    noData->setFont(QFont(font().family(), 9));
    noData->setPadding(QMargins(4, 0, 4, 0));
    noData->setPen(QPen(Qt::black));
}

void ImageStatistics::plotSelection(int index)
{
    for (auto &it : graphList) {
        it->setSelection(selection);
    }
    ui->statPlot->replot();
    ui->statPlot->update();
}

void ImageStatistics::dataPointClicked(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event)
{
    // Clicking on a data point may select or deselect it

    imgSelectedName = dataName[dataIndex];
    imgSelected = true;
    lastDataPointClicked = dataIndex;

    // We build the selection by successively clicking into different graphs.
    // Last index points to point above data range, hence must be +1 to select single data point;
    QCPDataRange rangeSelected(dataIndex, dataIndex+1);
    QCPDataSelection dpClicked;
    dpClicked.addDataRange(rangeSelected);
    // Right click:
    // Include/exclude a data point in the selection.
    // Multiple data points can be selected (we accumulate them in 'selection')
    if (event->button() == Qt::RightButton) {
        initKeySelection = true;
        if (!selection.contains(dpClicked)) selection.operator +=(rangeSelected);
        else selection.operator -=(rangeSelected);
    }
    // Left click: single selection/deselection only
    if (event->button() == Qt::LeftButton) {
        if (!selection.contains(dpClicked)) selection = dpClicked;
        else clearSelection();
    }
    lastButtonClicked = event->button();
    selection.simplify();
    plotSelection(dataIndex);
    //    ui->statPlot->setStatusTip(imgSelectedName);
    QString filter = ui->filterLineEdit->text();
    if (filter.isEmpty()) filter = "*.fits";
    if (!selection.isEmpty()) {
        //      emit imageSelected(scienceDirName+'/'+imgSelectedName, filter);
        emit imageSelected(lastDataPointClicked);
    }
    else {
        if (iViewOpen) iView->clearAll();
        imgSelected = false;
        lastDataPointClicked = -1;
    }
}

void ImageStatistics::on_invertSelectionPushButton_clicked()
{
    QCPDataRange outerRange(0, numObj);
    selection = selection.inverse(outerRange);
    plotSelection(0);
}

void ImageStatistics::numericSelection()
{
    if (numObj == 0) return;

    numSelection.clear();
    QVector<int> numericIndexes;

    QString skyMin = ui->skyMinLineEdit->text();
    QString skyMax = ui->skyMaxLineEdit->text();
    QString airmassMin = ui->airmassMinLineEdit->text();
    QString airmassMax = ui->airmassMaxLineEdit->text();
    QString seeingMin = ui->seeingMinLineEdit->text();
    QString seeingMax = ui->seeingMaxLineEdit->text();
    QString rzpMin = ui->rzpMinLineEdit->text();
    QString rzpMax = ui->rzpMaxLineEdit->text();
    QString ellMin = ui->ellMinLineEdit->text();
    QString ellMax = ui->ellMaxLineEdit->text();
    QString imageMin = ui->imageMinLineEdit->text();
    QString imageMax = ui->imageMaxLineEdit->text();

    for (int i=0; i<numObj; ++i) {
        if (!skyMin.isEmpty() && dataSky[i] < skyMin.toDouble()) numericIndexes.push_back(i);
        if (!skyMax.isEmpty() && dataSky[i] > skyMax.toDouble()) numericIndexes.push_back(i);
        if (!airmassMin.isEmpty() && dataAirmass[i] < airmassMin.toDouble()) numericIndexes.push_back(i);
        if (!airmassMax.isEmpty() && dataAirmass[i] > airmassMax.toDouble()) numericIndexes.push_back(i);
        if (!seeingMin.isEmpty() && dataFWHM[i] < seeingMin.toDouble()) numericIndexes.push_back(i);
        if (!seeingMax.isEmpty() && dataFWHM[i] > seeingMax.toDouble()) numericIndexes.push_back(i);
        if (!rzpMin.isEmpty() && dataRZP[i] < rzpMin.toDouble()) numericIndexes.push_back(i);
        if (!rzpMax.isEmpty() && dataRZP[i] > rzpMax.toDouble()) numericIndexes.push_back(i);
        if (!ellMin.isEmpty() && dataEllipticity[i] < ellMin.toDouble()) numericIndexes.push_back(i);
        if (!ellMax.isEmpty() && dataEllipticity[i] > ellMax.toDouble()) numericIndexes.push_back(i);
        if (!imageMin.isEmpty() && i < imageMin.toInt()) numericIndexes.push_back(i);
        if (!imageMax.isEmpty() && i > imageMax.toInt()) numericIndexes.push_back(i);
    }

    for (auto &it : numericIndexes) {
        QCPDataRange range(it,it+1);
        numSelection.addDataRange(range);
    }
    numSelection.simplify();
    QList<QCPDataRange> rangeList = numSelection.dataRanges();
    // operator += automatically simplifies the selection
    for (auto &it : rangeList) {
        selection.operator +=(it);
    }
    plotSelection(0);
}


void ImageStatistics::on_connectDataPointsCheckBox_clicked()
{
    if (ui->connectDataPointsCheckBox->isChecked()) myLineStyle = QCPGraph::lsLine;
    else myLineStyle = QCPGraph::lsNone;
    plot();
}

void ImageStatistics::on_clearSelectionPushButton_clicked()
{
    clearSelection();
    for (auto &it : graphList) {
        it->setSelection(selection);
    }
    ui->statPlot->replot();
    ui->statPlot->update();
}

void ImageStatistics::on_restoreDataPushButton_clicked()
{
    clearData();
    clearSelection();
    badStatsList.clear();
    moveFiles("*.fits", scienceDirName+"/inactive/badStatistics/", scienceDirName);
    readStatisticsData();
    plot();
}
