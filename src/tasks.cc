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
#include "ui_confdockwidget.h"
#include "functions.h"
#include "status.h"
#include "processingExternal/errordialog.h"
#include "datadir.h"
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QMetaObject>
#include <QGenericReturnArgument>

void MainWindow::deleteDummyLineEdits(QLineEdit *le1, QLineEdit *le2)
{
    // set the pointers to the null pointer
    le1 = nullptr;
    le2 = nullptr;
    delete le1;
    delete le2;
}

QStringList MainWindow::taskHDUreformat(bool &stop, QString mode)
{
    QString taskBasename = "HDUreformat";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    handleDataDirs(goodDirList, ui->setupBiasLineEdit, le1, le2, statusString, mode);
    handleDataDirs(goodDirList, ui->setupDarkLineEdit, le1, le2, statusString, mode);
    handleDataDirs(goodDirList, ui->setupFlatoffLineEdit, le1, le2, statusString, mode);
    handleDataDirs(goodDirList, ui->setupFlatLineEdit, le1, le2, statusString, mode);
    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, statusString, mode);
    handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, statusString, mode);
    handleDataDirs(goodDirList, ui->setupStandardLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskProcessbias(bool &stop, QString mode)
{
    QString taskBasename = "Processbias";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;   
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    handleDataDirs(goodDirList, ui->setupBiasLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskProcessdark(bool &stop, QString mode)
{
    QString taskBasename = "Processdark";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    handleDataDirs(goodDirList, ui->setupDarkLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskProcessflatoff(bool &stop, QString mode)
{
    QString taskBasename = "Processflatoff";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;    
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    handleDataDirs(goodDirList, ui->setupFlatoffLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskProcessflat(bool &stop, QString mode)
{
    QString taskBasename = "Processflat";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;    
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    // Do we use the FLAT OFF/DARK or BIAS (in case one or more are defined);
    // FLAT OFF preferred over BIAS
    QString bias = ui->setupBiasLineEdit->text();
    QString flatoff = ui->setupFlatoffLineEdit->text();
    if (!flatoff.isEmpty()) le1 = ui->setupFlatoffLineEdit;
    else if (!bias.isEmpty()) le1 = ui->setupBiasLineEdit;
    else le1->setText("theli_nobias");

    handleDataDirs(goodDirList, ui->setupFlatLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskProcessscience(bool &stop, QString mode)
{
    QString taskBasename = "Processscience";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;    
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    // Do we use the FLAT and / or BIAS (in case one or more are defined);
    QString bias = ui->setupBiasLineEdit->text();
    QString dark = ui->setupDarkLineEdit->text();
    QString flat = ui->setupFlatLineEdit->text();
    if (!dark.isEmpty()) le1 = ui->setupDarkLineEdit;
    else if (!bias.isEmpty()) le1 = ui->setupBiasLineEdit;
    else le1->setText("theli_nobias");

    if (!flat.isEmpty()) le2 = ui->setupFlatLineEdit;
    else le2->setText("theli_noflat");

    // append DT_x to tell the internal processor which Data structure we have to work on
    // For FLAT and FLAT_OFF we have separate taskBasenames, for SCIENCE we don't.
    // I should revisit this and make it more consistent
    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, "theli_DT_SCIENCE", mode);
    handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, "theli_DT_SKY", mode);
    handleDataDirs(goodDirList, ui->setupStandardLineEdit, le1, le2, "theli_DT_STANDARD", mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskChopnod(bool &stop, QString mode)
{
    QString taskBasename = "Chopnod";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;    
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, statusString, mode);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskBackground(bool &stop, QString mode)
{
    QString taskBasename = "Background";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;    
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    // The first argument gets replaced by the sky dir (if any)
    QString sky = ui->setupSkyLineEdit->text();
    if (!sky.isEmpty()) le1 = ui->setupSkyLineEdit;
    else le1->setText("noskydir");
    // Standard stars don't have separate sky entries (at least we don't support it)
    auto *nosky = new QLineEdit();
    nosky->setText("noskydir");

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, statusString, mode);

    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, statusString, mode);
    handleDataDirs(goodDirList, ui->setupStandardLineEdit, nosky, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskCollapse(bool &stop, QString mode)
{
    QString taskBasename = "Collapse";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;    
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, statusString, mode);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, statusString, mode);
    handleDataDirs(goodDirList, ui->setupStandardLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskBinnedpreview(bool &stop, QString mode)
{
    QString taskBasename = "Binnedpreview";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;    
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, statusString, mode);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskGlobalweight(bool &stop, QString mode)
{
    QString taskBasename = "Globalweight";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;    
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    // Do we use the FLAT and / or BIAS (in case one or more are defined);
    QString bias = ui->setupBiasLineEdit->text();
    QString dark = ui->setupDarkLineEdit->text();
    QString flat = ui->setupFlatLineEdit->text();
    if (!flat.isEmpty()) le1 = ui->setupFlatLineEdit;
    else le1->setText("theli_noflat");

    if (!dark.isEmpty()) le2 = ui->setupDarkLineEdit;
    else if (!bias.isEmpty()) le2 = ui->setupBiasLineEdit;
    else le2->setText("theli_nobias");
    // We don't need the statusString here
    statusString = "";

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, statusString, mode);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskIndividualweight(bool &stop, QString mode)
{
    QString taskBasename = "Individualweight";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;    
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, statusString, mode);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, statusString, mode);
    handleDataDirs(goodDirList, ui->setupStandardLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskSeparate(bool &stop, QString mode)
{
    QString taskBasename = "Separate";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;    
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, statusString, mode);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskAbsphotindirect(bool &stop, QString mode)
{
    QString taskBasename = "Absphotindirect";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;    
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskCreatesourcecat(bool &stop, QString mode)
{
    QString taskBasename = "Createsourcecat";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;    
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, statusString, mode);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskAstromphotom(bool &stop, QString mode)
{
    QString taskBasename = "Astromphotom";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;    
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, statusString, mode);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskGetCatalogFromWEB(bool &stop, QString mode)
{
    QString taskBasename = "GetCatalogFromWEB";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;    
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskGetCatalogFromIMAGE(bool &stop, QString mode)
{
    QString taskBasename = "GetCatalogFromIMAGE";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;    
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskRestoreHeader(bool &stop, QString mode)
{
    QString taskBasename = "RestoreHeader";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;    
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskSkysub(bool &stop, QString mode)
{
    QString taskBasename = "Skysub";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;    
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, statusString, mode);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskCoaddition(bool &stop, QString mode)
{
    QString taskBasename = "Coaddition";
    QString statusString = status.predictStatus(taskBasename);
    QStringList goodDirList;    
    auto *le1 = new QLineEdit();
    auto *le2 = new QLineEdit();

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, statusString, mode);
    deleteDummyLineEdits(le1, le2);

    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

// TODO: go through these tasks and clean up those we don't need anymore, or info therein we don't need
QStringList MainWindow::taskResolveTarget(bool &stop, QString mode)
{
    QString taskBasename = "ResolveTarget";
    QStringList goodDirList;
    // Replace blanks by underscores, and push the target name onto 'gooddirlist', which carries no other information
    QString target = cdw->ui->ARCtargetresolverLineEdit->text();
    target = target.replace(" ","_");

    cdw->ui->ARCraLineEdit->clear();
    cdw->ui->ARCdecLineEdit->clear();
    goodDirList << target;
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}
