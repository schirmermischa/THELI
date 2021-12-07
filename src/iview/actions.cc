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
#include "ui_ivfinderdockwidget.h"

void IView::checkFinder()
{
    if (!finderdw->isVisible()) return;

    // Both name fields empty
    if (finderdw->ui->targetNameSiderealLineEdit->text().isEmpty()
            && finderdw->ui->targetNameNonsiderealLineEdit->text().isEmpty()
            && !finderdw->ui->targetAlphaLineEdit->text().isEmpty()
            && !finderdw->ui->targetDeltaLineEdit->text().isEmpty() ) {
        finderdw->bypassResolver();
    }

    // sidereal field empty, nonsidereal not empty
    if (!finderdw->ui->targetNameNonsiderealLineEdit->text().isEmpty()
            && finderdw->ui->targetNameSiderealLineEdit->text().isEmpty()) {
        finderdw->on_MPCresolverToolButton_clicked();
    }

    // sidereal field not empty, nonsidereal empty, coords empty
    if (!finderdw->ui->targetNameSiderealLineEdit->text().isEmpty()
            && finderdw->ui->targetNameNonsiderealLineEdit->text().isEmpty()) {
        if (finderdw->ui->targetAlphaLineEdit->text().isEmpty()
                ||finderdw->ui->targetDeltaLineEdit->text().isEmpty()) {
            finderdw->on_locatePushButton_clicked();
        }
        if (!finderdw->ui->targetAlphaLineEdit->text().isEmpty()
                && !finderdw->ui->targetDeltaLineEdit->text().isEmpty()) {
            finderdw->bypassResolver();
        }
    }
}

void IView::checkFinderBypass()
{
    if (!finderdw->isVisible()) return;

    finderdw->bypassResolver();
}

void IView::previousAction_triggered()
{
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;

    if (currentId > 0) {
        currentId --;
        if (displayMode == "FITSmonochrome") loadFITS("", currentId);
        else if (displayMode == "CLEAR") loadFITS("", currentId);
        else if (displayMode.contains("SCAMP")) loadPNG("", currentId);
        else if (displayMode == "MEMview") {
            // Check if the selected image is active. If not, search for one
            while (myImageList[currentId]->activeState == MyImage::INACTIVE && currentId > 0) --currentId;
            loadFromRAM(myImageList[currentId], 0);
        }
        ui->actionNext->setEnabled(true);
        ui->actionEnd->setEnabled(true);
        ui->actionForward->setEnabled(true);
    }
    if (currentId == 0) {
        ui->actionPrevious->setDisabled(true);
        ui->actionStart->setDisabled(true);
        ui->actionBack->setDisabled(true);
    }

    showReferenceCat();
    showSourceCat();

    timer->stop();
    ui->actionBack->setChecked(false);
    ui->actionForward->setChecked(false);

    emit currentlyDisplayedIndex(currentId);

    checkFinder();
}

void IView::nextAction_triggered()
{
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;

    if (currentId < numImages-1) {
        currentId ++;
        if (displayMode == "FITSmonochrome") loadFITS("", currentId);
        else if (displayMode == "CLEAR") loadFITS("", currentId);
        else if (displayMode.contains("SCAMP")) loadPNG("", currentId);
        else if (displayMode == "MEMview") {
            // Check if the selected image is active. If not, search for one
            while (myImageList[currentId]->activeState == MyImage::INACTIVE && currentId < numImages-1) ++currentId;
            loadFromRAM(myImageList[currentId], 0);
        }
        ui->actionPrevious->setEnabled(true);
        ui->actionStart->setEnabled(true);
        ui->actionBack->setEnabled(true);
    }
    if (currentId == numImages-1) {
        ui->actionNext->setDisabled(true);
        ui->actionEnd->setDisabled(true);
        ui->actionForward->setDisabled(true);
    }

    showReferenceCat();
    showSourceCat();

    timer->stop();
    ui->actionBack->setChecked(false);
    ui->actionForward->setChecked(false);

    emit currentlyDisplayedIndex(currentId);

    checkFinder();
}

void IView::forwardAction_triggered()
{
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;

    if (ui->actionForward->isChecked()) timer->start(1000/speedSpinBox->value());
    else return;

    if (currentId < numImages - 1 && ui->actionForward->isChecked()) {
        ui->actionBack->setChecked(false);
        currentId ++;
        if (displayMode == "FITSmonochrome") loadFITS("", currentId);
        else if (displayMode == "CLEAR") loadFITS("", currentId);
        else if (displayMode.contains("SCAMP")) loadPNG("", currentId);
        else if (displayMode == "MEMview") {
            // Check if the selected image is active. If not, search for one
            while (myImageList[currentId]->activeState == MyImage::INACTIVE && currentId < numImages-1) ++currentId;
            loadFromRAM(myImageList[currentId], 0);
        }
        ui->actionPrevious->setEnabled(true);
        ui->actionStart->setEnabled(true);
    }
    if (currentId == numImages-1) {
        timer->stop();
        ui->actionBack->setChecked(false);
        ui->actionBack->setEnabled(true);
        ui->actionForward->setChecked(false);
        ui->actionForward->setDisabled(true);
        ui->actionNext->setDisabled(true);
        ui->actionEnd->setDisabled(true);
    }
    showReferenceCat();
    showSourceCat();

    emit currentlyDisplayedIndex(currentId);

    checkFinder();
}

void IView::backAction_triggered()
{
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;

    if (ui->actionBack->isChecked()) timer->start(1000/speedSpinBox->value());
    else return;

    if (currentId > 0 && ui->actionBack->isChecked()) {
        ui->actionForward->setChecked(false);
        currentId --;
        if (displayMode == "FITSmonochrome") loadFITS("", currentId);
        else if (displayMode == "CLEAR") loadFITS("", currentId);
        else if (displayMode.contains("SCAMP")) loadPNG("", currentId);
        else if (displayMode == "MEMview") {
            // Check if the selected image is active. If not, search for one
            while (myImageList[currentId]->activeState == MyImage::INACTIVE && currentId > 0) --currentId;
            loadFromRAM(myImageList[currentId], 0);
        }
        ui->actionNext->setEnabled(true);
        ui->actionEnd->setEnabled(true);
    }
    if (currentId == 0) {
        timer->stop();
        ui->actionBack->setChecked(false);
        ui->actionBack->setDisabled(true);
        ui->actionForward->setChecked(false);
        ui->actionForward->setEnabled(true);
        ui->actionPrevious->setDisabled(true);
        ui->actionStart->setDisabled(true);
    }
    showReferenceCat();
    showSourceCat();

    emit currentlyDisplayedIndex(currentId);

    checkFinder();
}

void IView::startAction_triggered()
{
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;

    currentId = 0;
    if (displayMode == "FITSmonochrome") loadFITS("", currentId);
    else if (displayMode == "CLEAR") loadFITS("", currentId);
    else if (displayMode.contains("SCAMP")) loadPNG("", currentId);
    else if (displayMode == "MEMview") {
        // Check if the selected image is active. If not, search for one
        while (myImageList[currentId]->activeState == MyImage::INACTIVE && currentId < numImages-1) ++currentId;
        loadFromRAM(myImageList[currentId], 0);
    }
    ui->actionNext->setEnabled(true);
    ui->actionForward->setEnabled(true);
    ui->actionEnd->setEnabled(true);
    ui->actionStart->setDisabled(true);
    ui->actionPrevious->setDisabled(true);
    ui->actionBack->setDisabled(true);

    showReferenceCat();
    showSourceCat();

    timer->stop();
    ui->actionBack->setChecked(false);
    ui->actionForward->setChecked(false);

    emit currentlyDisplayedIndex(currentId);

    checkFinder();
}

void IView::endAction_triggered()
{
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;

    currentId = numImages - 1;
    if (displayMode == "FITSmonochrome") loadFITS("", currentId);
    else if (displayMode == "CLEAR") loadFITS("", currentId);
    else if (displayMode.contains("SCAMP")) loadPNG("", currentId);
    else if (displayMode == "MEMview") {
        // Check if the selected image is active. If not, search for one
        while (myImageList[currentId]->activeState == MyImage::INACTIVE && currentId > 0) --currentId;
        loadFromRAM(myImageList[currentId], 0);
    }
    ui->actionNext->setDisabled(true);
    ui->actionForward->setDisabled(true);
    ui->actionEnd->setDisabled(true);
    ui->actionStart->setEnabled(true);
    ui->actionPrevious->setEnabled(true);
    ui->actionBack->setEnabled(true);

    showReferenceCat();
    showSourceCat();

    timer->stop();
    ui->actionBack->setChecked(false);
    ui->actionForward->setChecked(false);

    emit currentlyDisplayedIndex(currentId);

    checkFinder();
}

void IView::on_actionDragMode_triggered()
{
    setMiddleMouseMode("DragMode");             // exclusive button group in c'tor does not work!
    emit middleMouseModeChanged("DragMode");
}

void IView::on_actionSkyMode_triggered()
{
    setMiddleMouseMode("SkyMode");              // exclusive button group in c'tor does not work!
    emit middleMouseModeChanged("SkyMode");
}

void IView::on_actionWCSMode_triggered()
{
    setMiddleMouseMode("WCSMode");              // exclusive button group in c'tor does not work!
    emit middleMouseModeChanged("WCSMode");
}

void IView::on_actionMaskingMode_triggered()
{
    setMiddleMouseMode("MaskingMode");              // exclusive button group in c'tor does not work!
    emit middleMouseModeChanged("MaskingMode");
}


void IView::on_actionImage_statistics_triggered()
{
    if (statdw->isVisible()) {
        removeDockWidget(statdw);
        statdw->hide();
    }
    else {
        statdw->init();
        statdw->setFloating(false);
        statdw->raise();
        statdw->show();
    }
}

void IView::on_actionFinder_triggered()
{
    if (finderdw->isVisible()) {
        removeDockWidget(finderdw);
        finderdw->hide();
    }
    else {
        finderdw->setFloating(false);
        finderdw->raise();
        finderdw->show();
        finderdw->ui->targetNameSiderealLineEdit->setFocus();
    }
}

void IView::on_actionRedshift_triggered()
{
    if (zdw->isVisible()) {
        clearAxes();
        removeDockWidget(zdw);
        zdw->hide();
    }
    else {
        // Do not display the dialog if no image is loaded
        if (!currentMyImage) {
            ui->actionRedshift->setChecked(false);
            return;
        }
        zdw->setFloating(false);
        zdw->raise();
        zdw->show();
        zdw->init();
        updateAxes();
    }
}
