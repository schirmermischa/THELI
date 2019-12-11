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
#include "../functions.h"
#include "../qcustomplot.h"
#include "../iview/iview.h"

#include <QVector>
#include <QList>
#include <QStringList>

// Controls what happens when a key is pressed
void ImageStatistics::keyReleaseEvent(QKeyEvent *event)
{
    // 'delete' key: park selected images from one chip in inactive/badStatistics/
    // 'a' key: park selected images (including all chips from that exposure) in inactive/badStatistics/
    // Left / right key: move selection forward / backward
    if (!ui->statPlot->hasFocus()) return;
    if (event->key() != Qt::Key_Delete
            && event->key() != Qt::Key_A
            && event->key() != Qt::Key_Left
            && event->key() != Qt::Key_Right) return;

    if (selection.isEmpty()) return;
    if (!scienceDir.exists()) return;

    QVector<int> badIndexes;

    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_A) {
        // make a "badStatistics" sub-directory
        QString badStatsDirName = scienceDirName+"/inactive/badStatistics/";
        QDir badStatsDir(badStatsDirName);
        badStatsDir.mkpath(badStatsDirName);
        // move selected images to badstats
        foreach (QCPDataRange dataRange, selection.dataRanges()) {
     //       QCPGraphDataContainer::const_iterator itBegin = seeingGraph->data()->at(dataRange.begin());
     //       QCPGraphDataContainer::const_iterator itEnd = seeingGraph->data()->at(dataRange.end());
            int begin = dataRange.begin();
            int end = dataRange.end();
            for (int i=begin; i<end; ++i) {
                badIndexes.append(i);
                imgSelectedName = dataName[i];

                // 'Delete' key pressed (actually: released)
                // Park selected image
                if (event->key() == Qt::Key_Delete) {
                    if (!badStatsList.contains(imgSelectedName)) {
                        badStatsList << imgSelectedName;
                    }
                    QFile badImage(scienceDirName+"/"+imgSelectedName+statusString+".fits");
                    if (!badImage.rename(badStatsDirName+imgSelectedName+statusString+".fits")) {
                        qDebug() << "QDEBUG: Could not execute the following operation:";
                        qDebug() << "mv " << scienceDirName+"/"+imgSelectedName+statusString+".fits" << badStatsDirName+imgSelectedName+statusString+".fits";
                    }
                }

                // 'A' key pressed
                // Park selected exposure (all chips belonging to that exposure)
                if (event->key() == Qt::Key_A) {
                    QString base = removeLastWords(imgSelectedName, 1, '_');
                    QStringList baseFilter(base+"_*.fits");
                    QStringList baseList = scienceDir.entryList(baseFilter);
                    for (auto & it : baseList) {
                        if (!badStatsList.contains(it)) {
                            badStatsList << it;
                        }
                        QFile badImage(scienceDirName+"/"+it);
                        if (!badImage.rename(badStatsDirName+it)) {
                            qDebug() << "QDEBUG: Could not execute the following operation:";
                            qDebug() << "mv " << scienceDirName+"/"+it << badStatsDirName+it;
                        }
                    }
                }
            }
        }
        dataImageNr = removeVectorItems_T(dataImageNr, badIndexes);
        dataSky = removeVectorItems_T(dataSky, badIndexes);
        dataAirmass = removeVectorItems_T(dataAirmass, badIndexes);
        dataFWHM = removeVectorItems_T(dataFWHM, badIndexes);
        dataRZP = removeVectorItems_T(dataRZP, badIndexes);
        dataEllipticity = removeVectorItems_T(dataEllipticity, badIndexes);
        numObj = dataSky.length();
        clearSelection();
        readStatisticsData();
        plot();
        if (iViewOpen) iView->clearAll();
    }

    // Cycle the selection forth or back by one or more data points,
    // depending on previous click types
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) {
        if (lastDataPointClicked == -1) return;
        int currentDataPoint;
        if (initKeySelection) {
            clearSelection();
            initKeySelection = false;
        }
        if (event->key() == Qt::Key_Left) {
            if (lastDataPointClicked <= 0) return;
            currentDataPoint = lastDataPointClicked-1;
            if (lastKeyPressed == Qt::Key_Right) directionSwitched = "RightToLeft";
            else directionSwitched = "noSwitch";
            lastKeyPressed = Qt::Key_Left;
        }
        else if (event->key() == Qt::Key_Right) {
            if (lastDataPointClicked >= numObj-1) return;
            currentDataPoint = lastDataPointClicked+1;
            if (lastKeyPressed == Qt::Key_Left) directionSwitched = "LeftToRight";
            else directionSwitched = "noSwitch";
            lastKeyPressed = Qt::Key_Right;
        }
        else {
            // middle mouse click etc
            return;
        }
        lastDataPointClicked = currentDataPoint;
        QCPDataRange rangeSelected;
        if (directionSwitched == "noSwitch") {
            rangeSelected.setBegin(currentDataPoint);
            rangeSelected.setEnd(currentDataPoint+1);
        }
        if (directionSwitched == "RightToLeft") {
            rangeSelected.setBegin(currentDataPoint);
            rangeSelected.setEnd(currentDataPoint+2);
        }
        if (directionSwitched == "LeftToRight") {
            rangeSelected.setBegin(currentDataPoint-1);
            rangeSelected.setEnd(currentDataPoint+1);
        }
        QCPDataSelection dpClicked;
        dpClicked.addDataRange(rangeSelected);
        // Previously right click: add (or remove) previous or next data point to selection
        if (lastButtonClicked == Qt::RightButton) {
            if (!selection.contains(dpClicked)) selection.operator +=(rangeSelected);
            else selection.operator -=(rangeSelected);
        }
        // Previously left click: select previous or next data point
        if (lastButtonClicked == Qt::LeftButton) {
            clearSelection();
            selection = dpClicked;
        }
        selection.simplify();
        plotSelection(currentDataPoint);

        // Reflect the image in iView
        if (iViewOpen) {
            imgSelectedName = dataName[currentDataPoint];
            if (!selection.isEmpty()) {
                QString filter = ui->filterLineEdit->text();
                //                emit imageSelected(scienceDirName+'/'+imgSelectedName, filter);
                emit imageSelected(lastDataPointClicked);
            }
            else {
                if (iViewOpen) iView->clearAll();
                imgSelected = false;
                lastDataPointClicked = -1;
            }
        }
    }
}
