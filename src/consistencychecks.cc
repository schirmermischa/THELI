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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dockwidgets/confdockwidget.h"
#include "ui_confdockwidget.h"
#include "functions.h"
#include "status.h"
#include "datadir.h"
#include <QDebug>
#include <QMessageBox>
#include <QDir>

bool MainWindow::areAllPathsValid() {
    bool test = checkPathsLineEdit(ui->setupBiasLineEdit);
    test *= checkPathsLineEdit(ui->setupDarkLineEdit);
    test *= checkPathsLineEdit(ui->setupFlatoffLineEdit);
    test *= checkPathsLineEdit(ui->setupFlatLineEdit);
    test *= checkPathsLineEdit(ui->setupScienceLineEdit);
    test *= checkPathsLineEdit(ui->setupSkyLineEdit);
    test *= checkPathsLineEdit(ui->setupStandardLineEdit);
    return test;
}

bool MainWindow::checkMultipledirConsistency(QString mode)
{
    int nbias = datadir2StringList(ui->setupBiasLineEdit).length();
    int ndark = datadir2StringList(ui->setupDarkLineEdit).length();
    int nflatoff = datadir2StringList(ui->setupFlatoffLineEdit).length();
    int nflat = datadir2StringList(ui->setupFlatLineEdit).length();
    int nscience = datadir2StringList(ui->setupScienceLineEdit).length();
    int nsky = datadir2StringList(ui->setupSkyLineEdit).length();
    int nstandard = datadir2StringList(ui->setupStandardLineEdit).length();
    int ncorr; // can represent various calibrators

    if (mode == "HDUreformat") return true;
    if (mode == "ProcessBias") return true;
    if (mode == "ProcessDark") return true;

    // Flat processing (and a few other checks so we don't have to repeat them)
    bool flatcheck = false;
    if (mode == "ProcessFlat" || mode == "ProcessScience" || mode == "ProcessSky" || mode == "ProcessStandard") {
        // Check if we have a consistent setup to process the flats
        // flatoff takes precedence over bias if both are defined:
        if (nflatoff > 0 && nbias > 0) ncorr = nflatoff;
        else if (nflatoff > 0 && nbias == 0) ncorr = nflatoff;
        else {  // (nflatoff == 0 && nbias > 0 or  both == 0)
            ncorr = nbias;
        }
        // Now check if the setup is consistent:
        if (nflat == ncorr) flatcheck = true;
        else if (nflat > 1 && ncorr <= 1) flatcheck = true;
        else flatcheck = false;
        if (mode == "ProcessFlat") {
            if (flatcheck) return true;
            else {
                message(ui->plainTextEdit, "STOP: Inconsistent number of directories for FLAT processing.");
                return false;
            }
        }
    }

    if ((mode == "ProcessScience" || mode == "ProcessBackground") && nsky > 0) {
        if (nscience != nsky && nsky != 1) {
            message(ui->plainTextEdit, "STOP: Inconsistent number of SKY directories for SCIENCE.");
            message(ui->plainTextEdit, "Must be equal to that of SCIENCE, or one.");
            return false;
        }
    }

    if (mode == "ProcessScience" || mode == "ProcessSky" || mode == "ProcessStandard") {
        if (mode == "ProcessSky") nscience = nsky;
        if (mode == "ProcessStandard") nscience = nstandard;

        // We have checked the consistency for flat processing.
        if (!flatcheck) {
            message(ui->plainTextEdit, "STOP: Inconsistent number of directories for FLAT processing.");
            return false;
        }

        // If darks were specified, then they take precedence over biases!
        if ( ndark > 0 && nbias > 0) ncorr = ndark;
        else if (ndark > 0 && nbias == 0) ncorr = ndark;
        else {   // ndark == 0 && nbias > 0 or both == 0
            ncorr = nbias;
        }

        // What remains is to check the consistency science -> flat and science ->bias/dark
        // where 'science' can also be 'sky' or 'standard'
        if (nscience == nflat && nscience == ncorr) return true;
        else if (nscience > 1 && nflat <= 1 && ncorr <= 1) return true;
        else if (nscience > 1 && nflat <= 1 && ncorr == nscience) return true;
        else if (nscience > 1 && nflat == nscience && ncorr <= 1) return true;
        else {
            if (mode == "ProcessScience")
                message(ui->plainTextEdit, "STOP: Inconsistent number of directories for SCIENCE processing.");
            if (mode == "ProcessSky")
                message(ui->plainTextEdit, "STOP: Inconsistent number of directories for SKY processing.");
            if (mode == "ProcessStandard")
                message(ui->plainTextEdit, "STOP: Inconsistent number of directories for STANDARD processing.");

            // should we set the 'stop' flag here?
            return false;
        }
    }

    // In all other cases (later on during processing) we don't care anymore, hence:
    return true;
}

void MainWindow::hasDirCurrentData(DataDir *datadir, bool &stop)
{
    QString currentStatus = status.getStatusFromHistory();
    // The task does not change the status string. We expect the status to be current
    if (!datadir->hasType(currentStatus)) {
        message(ui->plainTextEdit, "STOP: '"+datadir->subdirname+"' does not contain expected images (*"+currentStatus+".fits).");
        stop = true;
    }
}

QString MainWindow::estimateStatusFromFilename(DataDir *datadir)
{
    // Get the name of a single fits file
    QStringList filterList("*.fits");
    QDir dir(datadir->name);
    QStringList imageList = dir.entryList(filterList);
    QString image;
    if (!imageList.isEmpty()) image = imageList.at(0);
    // The image extension (e.g. A, or AB)
    image = getLastWord(image,'_').remove(".fits").remove(QRegExp("[0-9]{1,3}"));
    return image;
}

void MainWindow::check_taskHDUreformat(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{
    QString taskName = checkboxMap.key(taskBasename)->text();

    if (datadir->isEmpty()) {
        if (mode != "execute") message(ui->plainTextEdit, "'"+datadir->subdirname+"' contains no images. Skipping...", "note");
        skip = true;
    }
    if (datadir->hasType("P")) {
        if (mode != "execute") message(ui->plainTextEdit, "'"+datadir->subdirname+"' already contains HDU reformatted images. Skipping...", "note");
        skip = true;
    }
    if (datadir->numEXT("PA*") > 0) {
        if (mode != "execute") message(ui->plainTextEdit, "'"+datadir->subdirname+"' contains preprocessed images. Skipping...", "note");
        skip = true;
    }
}

void MainWindow::check_taskProcessbias(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{
    QString taskName = checkboxMap.key(taskBasename)->text();
    if (datadir->isEmpty()) {
        if (mode != "execute") message(ui->plainTextEdit, "'"+datadir->subdirname+"' contains no images. Skipping...", "note");
        skip = true;
    }
    if (!datadir->hasType("P")
            && !ui->applyHDUreformatCheckBox->isChecked()) {
        message(ui->plainTextEdit, "STOP: '"+datadir->subdirname+"' does not contain HDU reformatted images.");
        stop = true;
    }
    if (!stop
            && datadir->numCHIP1 < 3
            && !ui->applyHDUreformatCheckBox->isChecked()) {
        message(ui->plainTextEdit, "STOP: Need at least 3 exposures in '"+datadir->subdirname+"'.");
        stop = true;
    }
}

void MainWindow::check_taskProcessdark(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{
    QString taskName = checkboxMap.key(taskBasename)->text();
    if (datadir->isEmpty()) {
        if (mode != "execute") message(ui->plainTextEdit, "'"+datadir->subdirname+"' contains no images. Skipping...", "note");
        skip = true;
    }
    if (!datadir->hasType("P")
            && !ui->applyHDUreformatCheckBox->isChecked()) {
        message(ui->plainTextEdit, "STOP: '"+datadir->subdirname+"' does not contain HDU reformatted images.");
        stop = true;
    }
    if (!stop
            && datadir->numCHIP1 < 3
            && !ui->applyHDUreformatCheckBox->isChecked()) {
        message(ui->plainTextEdit, "STOP: Need at least 3 exposures in '"+datadir->subdirname+"'.");
        stop = true;
    }
}

void MainWindow::check_taskProcessflatoff(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{
    QString taskName = checkboxMap.key(taskBasename)->text();
    if (datadir->isEmpty()) {
        if (mode != "execute") message(ui->plainTextEdit, "'"+datadir->subdirname+"' contains no images. Skipping...", "note");
        skip = true;
    }
    if (!datadir->hasType("P")
            && !ui->applyHDUreformatCheckBox->isChecked()) {
        message(ui->plainTextEdit, "STOP: '"+datadir->subdirname+"' does not contain HDU reformatted images.");
        stop = true;
    }
    if (!stop
            && datadir->numCHIP1 < 3
            && !ui->applyHDUreformatCheckBox->isChecked()) {
        message(ui->plainTextEdit, "STOP: Need at least 3 exposures in '"+datadir->subdirname+"'.");
        stop = true;
    }
}

void MainWindow::check_taskProcessflat(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{
    QString taskName = checkboxMap.key(taskBasename)->text();
    if (datadir->isEmpty()) {
        if (mode != "execute") message(ui->plainTextEdit, "'"+datadir->subdirname+"' contains no images. Skipping...", "note");
        skip = true;
    }
    if (!datadir->hasType("P")
            && !ui->applyHDUreformatCheckBox->isChecked()) {
        message(ui->plainTextEdit, "STOP: '"+datadir->subdirname+"' does not contain HDU reformatted images.");
        stop = true;
    }
    if (!stop
            && datadir->numCHIP1 < 2
            && !ui->applyHDUreformatCheckBox->isChecked()) {
        message(ui->plainTextEdit, "STOP: Need at least 3 exposures in '"+datadir->subdirname+"'.");
        stop = true;
    }
}

void MainWindow::check_taskProcessscience(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{
    if (datadir->isEmpty()) {
        if (mode != "execute") message(ui->plainTextEdit, "'"+datadir->subdirname+"' contains no images. Skipping...", "note");
        skip = true;
    }
    if (!datadir->hasType("P")
            && !datadir->hasType("PA")
            && !ui->applyHDUreformatCheckBox->isChecked()) {
        message(ui->plainTextEdit, "STOP: '"+datadir->subdirname+"' does not contain HDU reformatted images.");
        stop = true;
    }
}

void MainWindow::check_taskChopnod(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{

}

void MainWindow::check_taskBackground(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{

}

void MainWindow::check_taskCollapse(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{

}

void MainWindow::check_taskBinnedpreview(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{

}

void MainWindow::check_taskGlobalweight(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{

}

void MainWindow::check_taskIndividualweight(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{

}

void MainWindow::check_taskAbsphotindirect(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{
    QString standardDir = ui->setupStandardLineEdit->text();
    if (standardDir.isEmpty()) {
        if (mode != "execute") message(ui->plainTextEdit, "No standard star directory was defined.", "stop");
        stop = true;
    }
}

void MainWindow::check_taskSeparate(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{

}

void MainWindow::check_taskCreatesourcecat(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{

}

void MainWindow::check_taskAstromphotom(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{

}

void MainWindow::check_taskGetCatalogFromWEB(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{

}

void MainWindow::check_taskGetCatalogFromIMAGE(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{

}

void MainWindow::check_taskRestoreHeader(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{
    hasDirCurrentData(datadir, stop);
    // Check that original headers exist
}

void MainWindow::check_taskResolveTarget(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{
    QString target = cdw->ui->ARCtargetresolverLineEdit->text();
    if (target.isEmpty()) {
        message(ui->plainTextEdit, "A target name must be provided for coordinate resolution.", "stop");
        stop = true;
        cdw->ui->ARCraLineEdit->clear();
        cdw->ui->ARCdecLineEdit->clear();
    }
}

void MainWindow::check_taskSkysub(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{

}

void MainWindow::check_taskCoaddition(DataDir *datadir, QString taskBasename, bool &stop, bool &skip, const QString mode)
{

}
