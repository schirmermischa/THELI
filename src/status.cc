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

#include "status.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

// Constructor
Status::Status()
{

}

void Status::init()
{
    // connect the statusChanged signal with the corresponding slot
//    connect(this, &Status::statusChanged, this, &Status::updateStatus);
}

// Set the background of the checkboxes according to the processing history
void Status::history2checkbox()
{
    int i=0;
    QPalette palette;
    for (auto &checked : listHistory) {
        if (checked) palette.setColor(QPalette::Button,QColor("#33e3cc"));
        else palette.setColor(QPalette::Button,QColor("#eeffff"));

        // Paint checkbox background
        listCheckBox.operator[](i)->setPalette(palette);
        listCheckBox.operator[](i)->setAutoFillBackground(true);
        ++i;
    }
}

// UNUSED
/*
// Set the background of the checkboxes according to the processing history
void Status::history2actions()
{
    for (int i=0; i<numtasks; ++i) {
        if (listHistory.at(i)) (listStatusActions.operator[](i))->setChecked(true);
        else (listStatusActions.operator[](i))->setChecked(false);
    }
}
*/

// Triggered by mainguiworker (everytime a task is finished)
void Status::updateStatusReceived(QString taskBasename, bool success)
{
    int index = indexMap.value(taskBasename,-1);
    if (index == -1) return;
    else {
        listHistory.operator[](index) = success;
        updateStatus();
    }
}

// Triggered by user actions in the main window.
// Repaints the task checkboxes (among others)
void Status::updateStatus()
{
    int i=0;
    QPalette palette;

    for (auto &checked : listHistory) {
        if (checked) palette.setColor(QPalette::Button, QColor("#33e3cc"));
        else palette.setColor(QPalette::Button, QColor("#eeffff"));

        // Paint checkbox background
        listCheckBox.operator[](i)->setPalette(palette);
        listCheckBox.operator[](i)->setAutoFillBackground(true);

        // Only UNCHECK a task checkbox if the history changed to "executed", never CHECK it
        // because we don't know whether the user wants to redo it.
        if (checked) listCheckBox.operator[](i)->setChecked(false);
        ++i;
    }
}

void Status::clearAllCheckBoxes()
{
    for (auto &cb : listCheckBox) cb->setChecked(false);
}

QString Status::getStatusFromHistory(bool keepblanks)
{
    QString statusString;
    for (int i=0; i<numtasks; ++i) {
        if (listHistory.at(i)) statusString.append(listFixedValue.at(i));
        else statusString.append(" ");
    }

    // Return the status string with or without blanks
    if (keepblanks) return statusString;
    else return statusString.remove(" ");
}

// Find the ID of the last task executed that could have changed the processing status string.
// One could just hardcode the ABCDM chars and see what is the last char in the status, but
// it's better to keep this generic. Too hard to maintain otherwise.
int Status::lastExecutedTaskId()
{
    QString statusString = getStatusFromHistory(true);
    int lastId = 0;
    if (statusString.length() != numtasks) {
        qDebug() << "QDEBUG: The length of the status string must match the number of tasks!";
        return 0;
    }
    for (int i=0; i<numtasks; ++i) {
        if (statusString.at(i) != " ") lastId = i;
    }
    return lastId;
}

// Predicts the status string at a given checkbox based on previous checkbox states
QString Status::predictStatus(QString taskBasename)
{
    // Get the current processing status, including all blanks
    QString predictedStatus = getStatusFromHistory(true);

    // We also need the processing status when the user clicks on the separate "get catalog" button.
    // We use the same status as if astrometry was run
    if (taskBasename == "GetCatalogFromWEB") taskBasename = "Astromphotom";

    // What was the ID of the last task that changed the processing status?
    int lastId = lastExecutedTaskId();
    int currentCheckbox = indexMap.value(taskBasename);
    int upper;
    if (lastId < currentCheckbox) upper = lastId;
    else upper = currentCheckbox;

    // Erase everything after lastId or later than currently to be executed checkbox (whichever is smaller)
    // with blanks
    for (int i=upper+1; i<numtasks; ++i) predictedStatus = predictedStatus.replace(i, 1, ' ');

    // Step from lastId+1 up to (excluding) taskBasename,
    // and set the corresponding task character if the checkbox is set
    // We also have some special task names that are not reflected by means of explicit task checkboxes.
    // In these cases, return an empty string (these tasks don't need the status string)
    upper = indexMap.value(taskBasename,-1);
    if (upper == -1) {
        return "";
    }
    for (int i=lastId+1; i<upper; ++i) {
        if (listCheckBox.at(i)->isChecked())
            predictedStatus = predictedStatus.replace(i,1,listFixedValue.at(i));
    }
    // BIAS / FLAT is a mandatory step (because it truncates the overscan (if any)
    // even if no bias / flat is applied. HARDCODING index.
    predictedStatus = predictedStatus.replace(5, 1, listFixedValue.at(5));

    // Truncate everything that comes after the current checkbox index
    predictedStatus.truncate(currentCheckbox);
    predictedStatus.remove(" ");
    return predictedStatus.simplified();
}

// Predicts the status string after a given Checkbox has been executed
QString Status::predictStatusAfter(QString taskBasename)
{
    QString statusstring = predictStatus(taskBasename);
    statusstring.append(listFixedValue[indexMap.value(taskBasename)]);
    return statusstring.simplified();
}

// If the status changed, e.g. by restoring a previous processing stage, then
// we must reflect this in the history, the checkboxes, and the action status
void Status::statusstringToHistory(QString &statusstring)
{
    // HARDCODING !!!
    if (statusstring.contains("P")) listHistory.operator[](0) = true;
    else listHistory.operator[](0) = false;

    if (statusstring.contains("A")) listHistory.operator[](5) = true;
    else listHistory.operator[](5) = false;

    if (statusstring.contains("M")) listHistory.operator[](6) = true;
    else listHistory.operator[](6) = false;

    if (statusstring.contains("B")) listHistory.operator[](7) = true;
    else listHistory.operator[](7) = false;

    if (statusstring.contains("C")) listHistory.operator[](8) = true;
    else listHistory.operator[](8) = false;

    if (statusstring.contains("D")) listHistory.operator[](16) = true;
    else listHistory.operator[](16) = false;

    if (statusstring.contains("S")) listHistory.operator[](17) = true;
    else listHistory.operator[](17) = false;

    // Update checkboxes and actions
    updateStatus();
}
