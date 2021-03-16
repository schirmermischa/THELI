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

QStringList MainWindow::taskHDUreformat(bool &stop, const QString mode)
{
    QString taskBasename = "HDUreformat";
    QStringList goodDirList;
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    handleDataDirs(goodDirList, ui->setupBiasLineEdit, le1, le2, "", success);
    handleDataDirs(goodDirList, ui->setupDarkLineEdit, le1, le2, "", success);
    handleDataDirs(goodDirList, ui->setupFlatoffLineEdit, le1, le2, "", success);
    handleDataDirs(goodDirList, ui->setupFlatLineEdit, le1, le2, "", success);
    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, "", success);
    handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, "", success);
    handleDataDirs(goodDirList, ui->setupStandardLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskProcessbias(bool &stop, const QString mode)
{
    QString taskBasename = "Processbias";
    QStringList goodDirList;   
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    handleDataDirs(goodDirList, ui->setupBiasLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskProcessdark(bool &stop, const QString mode)
{
    QString taskBasename = "Processdark";
    QStringList goodDirList;
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    handleDataDirs(goodDirList, ui->setupDarkLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskProcessflatoff(bool &stop, const QString mode)
{
    QString taskBasename = "Processflatoff";
    QStringList goodDirList;    
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    handleDataDirs(goodDirList, ui->setupFlatoffLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskProcessflat(bool &stop, const QString mode)
{
    QString taskBasename = "Processflat";
    QStringList goodDirList;    
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    // Do we use the FLAT OFF/DARK or BIAS (in case one or more are defined);
    // FLAT OFF preferred over BIAS
    QString bias = ui->setupBiasLineEdit->text();
    QString flatoff = ui->setupFlatoffLineEdit->text();
    if (!flatoff.isEmpty()) le1 = ui->setupFlatoffLineEdit;
    else if (!bias.isEmpty()) le1 = ui->setupBiasLineEdit;
    else le1->setText("theli_nobias");

    handleDataDirs(goodDirList, ui->setupFlatLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskProcessscience(bool &stop, const QString mode)
{
    QString taskBasename = "Processscience";
    QStringList goodDirList;    
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

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
    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, "theli_DT_SCIENCE", success);
    handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, "theli_DT_SKY", success);
    handleDataDirs(goodDirList, ui->setupStandardLineEdit, le1, le2, "theli_DT_STANDARD", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskChopnod(bool &stop, const QString mode)
{
    QString taskBasename = "Chopnod";
    QStringList goodDirList;    
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, "", success);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskBackground(bool &stop, const QString mode)
{
    QString taskBasename = "Background";
    QStringList goodDirList;    
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    // The first argument gets replaced by the sky dir (if any)
    QString sky = ui->setupSkyLineEdit->text();
    if (!sky.isEmpty()) le1 = ui->setupSkyLineEdit;
    else le1->setText("noskydir");
    // Standard stars don't have separate sky entries (at least we don't support it)
    auto *nosky = new QLineEdit(controller);
    nosky->setText("noskydir");

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, "", success);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, "", success);
    handleDataDirs(goodDirList, ui->setupStandardLineEdit, nosky, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskCollapse(bool &stop, const QString mode)
{
    QString taskBasename = "Collapse";
    QStringList goodDirList;
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, "", success);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, "", success);
    handleDataDirs(goodDirList, ui->setupStandardLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskBinnedpreview(bool &stop, const QString mode)
{
    QString taskBasename = "Binnedpreview";
    QStringList goodDirList;    
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, "", success);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskGlobalweight(bool &stop, const QString mode)
{
    QString taskBasename = "Globalweight";
    QStringList goodDirList;    
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    // Do we use the FLAT and / or BIAS (in case one or more are defined);
    QString bias = ui->setupBiasLineEdit->text();
    QString dark = ui->setupDarkLineEdit->text();
    QString flat = ui->setupFlatLineEdit->text();
    if (!flat.isEmpty()) le1 = ui->setupFlatLineEdit;
    else le1->setText("theli_noflat");

    if (!dark.isEmpty()) le2 = ui->setupDarkLineEdit;
    else if (!bias.isEmpty()) le2 = ui->setupBiasLineEdit;
    else le2->setText("theli_nobias");

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, "", success);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskIndividualweight(bool &stop, const QString mode)
{
    QString taskBasename = "Individualweight";
    QStringList goodDirList;    
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, "", success);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, "", success);
    handleDataDirs(goodDirList, ui->setupStandardLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskSeparate(bool &stop, const QString mode)
{
    QString taskBasename = "Separate";
    QStringList goodDirList;    
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, "", success);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskAbsphotindirect(bool &stop, const QString mode)
{
    QString taskBasename = "Absphotindirect";
    QStringList goodDirList;    
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskCreatesourcecat(bool &stop, const QString mode)
{
    QString taskBasename = "Createsourcecat";
    QStringList goodDirList;    
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, "", success);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskAstromphotom(bool &stop, const QString mode)
{
    QString taskBasename = "Astromphotom";
    QStringList goodDirList;    
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, "", success);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskGetCatalogFromWEB(bool &stop, const QString mode)
{
    QString taskBasename = "GetCatalogFromWEB";
    QStringList goodDirList;    
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskGetCatalogFromIMAGE(bool &stop, const QString mode)
{
    QString taskBasename = "GetCatalogFromIMAGE";
    QStringList goodDirList;    
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskRestoreHeader(bool &stop, const QString mode)
{
    QString taskBasename = "RestoreHeader";
    QStringList goodDirList;    
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskSkysub(bool &stop, const QString mode)
{
    QString taskBasename = "Skysub";
    QStringList goodDirList;    
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, "", success);
    if (processSkyImages) handleDataDirs(goodDirList, ui->setupSkyLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

QStringList MainWindow::taskCoaddition(bool &stop, const QString mode)
{
    QString taskBasename = "Coaddition";
    QStringList goodDirList;    
    auto *le1 = new QLineEdit(controller);
    auto *le2 = new QLineEdit(controller);
    bool success = true;

    handleDataDirs(goodDirList, ui->setupScienceLineEdit, le1, le2, "", success);

    if (!success) return QStringList();
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}

// TODO: go through these tasks and clean up those we don't need anymore, or info therein we don't need
QStringList MainWindow::taskResolveTargetSidereal(bool &stop, const QString mode)
{
    QString taskBasename = "ResolveTargetSidereal";
    QStringList goodDirList;
    // Replace blanks by underscores, and push the target name onto 'gooddirlist', which carries no other information
    QString target = cdw->ui->ARCtargetresolverLineEdit->text();
    target = target.replace(" ","_");

    cdw->ui->ARCraLineEdit->clear();
    cdw->ui->ARCdecLineEdit->clear();
    goodDirList << target;
    return createCommandlistBlock(taskBasename, goodDirList, stop, mode);
}
