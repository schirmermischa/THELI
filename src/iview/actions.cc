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

void IView::previousAction_triggered()
{
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;

    if (currentId > 0) {
        currentId --;
        if (displayMode == "FITSmonochrome") loadFITS("", currentId);
        else if (displayMode == "CLEAR") loadFITS("", currentId);
        else if (displayMode == "SCAMP") loadPNG("", currentId);
        else if (displayMode == "MEMview") loadFromRAM(myImageList[currentId], 0);
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
}

void IView::nextAction_triggered()
{
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;

    if (currentId < numImages-1) {
        currentId ++;
        if (displayMode == "FITSmonochrome") loadFITS("", currentId);
        else if (displayMode == "CLEAR") loadFITS("", currentId);
        else if (displayMode == "SCAMP") loadPNG("", currentId);
        else if (displayMode == "MEMview") loadFromRAM(myImageList[currentId], 0);
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
        else if (displayMode == "SCAMP") loadPNG("", currentId);
        else if (displayMode == "MEMview") loadFromRAM(myImageList[currentId], 0);
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
        else if (displayMode == "SCAMP") loadPNG("", currentId);
        else if (displayMode == "MEMview") loadFromRAM(myImageList[currentId], 0);
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
}

void IView::startAction_triggered()
{
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;

    currentId = 0;
    if (displayMode == "FITSmonochrome") loadFITS("", currentId);
    else if (displayMode == "CLEAR") loadFITS("", currentId);
    else if (displayMode == "SCAMP") loadPNG("", currentId);
    else if (displayMode == "MEMview") loadFromRAM(myImageList[currentId], 0);
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
}

void IView::endAction_triggered()
{
    // Leave if no image is displayed
    if (scene->items().isEmpty()) return;

    currentId = numImages - 1;
    if (displayMode == "FITSmonochrome") loadFITS("", currentId);
    else if (displayMode == "CLEAR") loadFITS("", currentId);
    else if (displayMode == "SCAMP") loadPNG("", currentId);
    else if (displayMode == "MEMview") loadFromRAM(myImageList[currentId], 0);
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
}
