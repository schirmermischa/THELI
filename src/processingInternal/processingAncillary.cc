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

#include "controller.h"
#include "../mainwindow.h"
#include "../tools/tools.h"
#include "../query/query.h"
#include "dockwidgets/confdockwidget.h"
#include "ui_confdockwidget.h"

#include <QMetaObject>
#include <QVector>
#include <QStringList>
#include <QProgressBar>

void Controller::taskInternalGetCatalogFromWEB()
{
    QString scienceDir = instructions.split(" ").at(1);
    Data *scienceData = getData(DT_SCIENCE, scienceDir);
    if (scienceData == nullptr) return;      // Error triggered by getData();

    if (scienceData->myImageList[0].isEmpty()) {
        emit messageAvailable("No images found for "+scienceData->subDirName+". Could not determine sky coordinates for catalog download.", "warning");
        emit warningReceived();
        return;
    }

    pushBeginMessage(taskBasename, scienceDir);
    pushConfigGetCatalogFromWeb();

    Query *query = new Query(&verbosity);
    connect(query, &Query::bulkMotionObtained, cdw, &ConfDockWidget::updateGaiaBulkMotion);
    connect(query, &Query::messageAvailable, monitor, &Monitor::displayMessage);
    connect(query, &Query::critical, this, &Controller::criticalReceived);
    connect(query, &Query::critical, scienceData, &Data::criticalFromQueryReceived);
    query->mainDirName = mainDirName;
    query->refcatName = cdw->ui->ARCcatalogComboBox->currentText();
    query->alpha_manual = cdw->ui->ARCraLineEdit->text();
    query->delta_manual = cdw->ui->ARCdecLineEdit->text();
    query->radius_manual = cdw->ui->ARCradiusLineEdit->text();
    query->magLimit_string = cdw->ui->ARCminmagLineEdit->text();
    query->maxProperMotion_string = cdw->ui->ARCmaxpmLineEdit->text();
    query->scienceData = scienceData;
    query->naxis1 = instData->sizex[0];
    query->naxis2 = instData->sizey[0];
    query->pixscale = instData->pixscale;
    query->doAstromQueryFromWeb();
    delete query;
    query = nullptr;

    //    pushEndMessage(taskBasename, scienceDir);
}

// UNUSED
/*
void Controller::provideHeaderInfo(Data *scienceData)
{
    if (verbosity>1) emit messageAvailable("Collecting metadata from FITS files ...", "controller");

    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        for (auto &it : scienceData->myImageList[chip]) {
            it->loadHeader();
        }
    }
}
*/

void Controller::downloadGaiaCatalog(Data *scienceData)
{
    //    int verbosity = 0;
    gaiaQuery = new Query(&verbosity);
    connect(gaiaQuery, &Query::messageAvailable, this, &Controller::messageAvailableReceived);
    connect(gaiaQuery, &Query::critical, this, &Controller::criticalReceived);
    gaiaQuery->mainDirName = mainDirName;
    gaiaQuery->scienceData = scienceData;
    gaiaQuery->naxis1 = instData->sizex[0];
    gaiaQuery->naxis2 = instData->sizey[0];
    gaiaQuery->pixscale = instData->pixscale;
    gaiaQuery->suppressCatalogWarning = true;

    emit messageAvailable("Querying point source catalog from GAIA ...", "ignore");
    gaiaQuery->doGaiaQuery();
    emit messageAvailable(QString::number(gaiaQuery->numSources) + " point sources retrieved for analysis of image quality ...", "ignore");

    // DO NOT 'delete' gaia query here. It is still used elsewhere
}

// Overload, to set the image size from the coadded image, and not from the images in the Data class
void Controller::downloadGaiaCatalog(Data *scienceData, QString radius)
{
    //    int verbosity = 0;
    gaiaQuery = new Query(&verbosity);
    connect(gaiaQuery, &Query::messageAvailable, this, &Controller::messageAvailableReceived);
    connect(gaiaQuery, &Query::critical, this, &Controller::criticalReceived);
    gaiaQuery->mainDirName = mainDirName;
    gaiaQuery->scienceData = scienceData;
    gaiaQuery->radius_string = radius;
    gaiaQuery->suppressCatalogWarning = true;

    emit messageAvailable("Querying point source catalog from GAIA ...", "ignore");
    gaiaQuery->doGaiaQuery();
    emit messageAvailable(QString::number(gaiaQuery->numSources) + " point sources retrieved for analysis of image quality ...", "ignore");

    // DO NOT 'delete' gaia query here. It is still used elsewhere
}

void Controller::collectGaiaRaDec(MyImage *image, QVector<double> &dec, QVector<double> &ra, QVector<QVector<double>> &output)
{
    long dim = dec.length();
    output.reserve(dim);
    QVector<double> alpha;
    QVector<double> delta;
    alpha.reserve(4);
    delta.reserve(4);
    alpha << image->alpha_ll << image->alpha_lr << image->alpha_ul<< image->alpha_ur;
    delta << image->delta_ll << image->delta_lr << image->delta_ul<< image->delta_ur;
    double alpha_min = minVec_T(alpha);
    double alpha_max = maxVec_T(alpha);
    double delta_min = minVec_T(delta);
    double delta_max = maxVec_T(delta);

    for (long i=0; i<dim; ++i) {
        // Only include refcat sources that are within the footprint of the image.
        // Otherwise, with very large multichip cameras, there is a severe overhead for every chip
        if (ra[i] >= alpha_min && ra[i] <= alpha_max && dec[i] >= delta_min && dec[i] <= delta_max) {
            QVector<double> result(3);
            result[0] = dec[i];
            result[1] = ra[i];
            result[2] = 0.0;   // dummy magnitude
            output << result;
        }
    }
}

void Controller::taskInternalGetCatalogFromIMAGE()
{
    QString scienceDir = instructions.split(" ").at(1);
    Data *scienceData = getData(DT_SCIENCE, scienceDir);
    if (scienceData == nullptr) return;      // Error triggered by getData();

    pushBeginMessage(taskBasename, scienceDir);
    pushConfigGetCatalogFromImage();

    QString DT = cdw->ui->ARCDTLineEdit->text();
    QString DMIN = cdw->ui->ARCDMINLineEdit->text();
    if (DT == "") DT = cdw->defaultMap["ARCDTLineEdit"];
    if (DMIN == "") DMIN = cdw->defaultMap["ARCMINLineEdit"];

    QString image = cdw->ui->ARCselectimageLineEdit->text();
    QFileInfo fi;
    fi.setFile(image);
    QString imagePath = fi.absolutePath();
    QString imageName = fi.fileName();
    QVector<bool> dummyMask;
    dummyMask.clear();
    MyImage *detectionImage = new MyImage(imagePath, imageName, "", 1, dummyMask, &verbosity);
    connect(detectionImage, &MyImage::critical, this, &Controller::criticalReceived);
    connect(detectionImage, &MyImage::messageAvailable, this, &Controller::messageAvailableReceived);
    detectionImage->setupCoaddMode();    // Read image, add a dummy global mask, and add a weight map if any
    detectionImage->backgroundModel(256, "interpolate");
    detectionImage->segmentImage(DT, DMIN, true);

    Query *query = new Query(&verbosity);
    connect(query, &Query::messageAvailable, monitor, &Monitor::displayMessage);
    connect(query, &Query::critical, this, &Controller::criticalReceived);
    query->mainDirName = mainDirName;
    query->scienceData = scienceData;
    query->fromImage = true;
    for (auto &object : detectionImage->objectList) {
        query->ra_out.append(object->ALPHA_J2000);
        query->de_out.append(object->DELTA_J2000);
        query->mag1_out.append(object->MAG_AUTO);
    }
    query->writeAstromScamp();
    query->writeAstromANET();
    query->writeAstromIview();
    query->pushNumberOfSources();
    delete query;
    query = nullptr;
    detectionImage->releaseAllDetectionMemory();
    detectionImage->releaseBackgroundMemory("entirely");

    delete detectionImage;
    detectionImage = nullptr;

    // dump reference catalog ID
    QString outpath = mainDirName+"/"+scienceData->subDirName+"/cat/refcat/";
    mkAbsDir(outpath);

    QFile file(outpath+"/.refcatID");
    QTextStream stream(&file);
    if( !file.open(QIODevice::WriteOnly)) {
        emit messageAvailable("Query::dumpRefcatID(): ERROR writing "+outpath+file.fileName()+" : "+file.errorString(), "error");
        emit criticalReceived();
        return;
    }

    stream << image+"_"+cdw->ui->ARCDTLineEdit->text()+"_"+cdw->ui->ARCDMINLineEdit->text() << "\n";
    file.close();
}

void Controller::taskInternalResolveTargetSidereal()
{
    cdw->ui->ARCpmRALineEdit->clear();
    cdw->ui->ARCpmDECLineEdit->clear();

    QString targetName = cdw->ui->ARCtargetresolverLineEdit->text();
    if (targetName.isEmpty()) return;
    targetName = targetName.simplified().replace(" ", "_");

    Query *query = new Query(&verbosity);
    connect(query, &Query::messageAvailable, monitor, &Monitor::displayMessage);
    connect(query, &Query::critical, this, &Controller::criticalReceived);
    QString check = query->resolveTargetSidereal(targetName);
    if (check == "Resolved") emit targetResolved(query->targetAlpha, query->targetDelta);
    else if (check == "Unresolved") emit showMessageBox("Controller::TARGET_UNRESOLVED", cdw->ui->ARCtargetresolverLineEdit->text(), "");
    else {
        // nothing yet.
    }

    delete query;
    query = nullptr;
}

void Controller::taskInternalRestoreHeader()
{
    QString scienceDir = instructions.split(" ").at(1);
    Data *scienceData = getData(DT_SCIENCE, scienceDir);
    if (scienceData == nullptr) return;      // Error triggered by getData();

    pushBeginMessage(taskBasename, scienceDir);

    QDir headerDir;
    headerDir.setPath(mainDirName+"/"+scienceDir+"/headers/");
    if (!headerDir.exists()) {
        emit messageAvailable("Could not restore the original WCS solution. The headers/ subdirectory does not exist in<br>"+scienceDir, "error");
        successProcessing = false;
        monitor->raise();
        return;
    }

    QDir origDir;
    origDir.setPath(mainDirName+"/"+scienceDir+"/.origheader_backup/");
    if (!origDir.exists()) {
        emit messageAvailable("Cannot restore headers. The backup copy was not found.", "error");
        successProcessing = false;
        monitor->raise();
        return;
    }

#pragma omp parallel for num_threads(maxExternalThreads)
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        for (auto &it : scienceData->myImageList[chip]) {
            it->loadHeader();
            // Restore the header if a backup exists
            if (it->scanAstromHeader(chip, "inBackupDir")) {
                it->updateZeroOrderOnDrive("restore");
//                it->updateZeroOrderInMemory();
            }
        }
    }
    successProcessing = true;
    //    pushEndMessage(taskBasename, scienceDir);
}

// Cloned from Query::getCatalogSearchLocationAstrom()
void Controller::getFieldCenter(Data *data, QString &alphaCenter, QString &deltaCenter)
{
    if (!successProcessing) return;

    QVector<double> crval1 = data->getKeyFromAllImages("CRVAL1");
    QVector<double> crval2 = data->getKeyFromAllImages("CRVAL2");

    if (crval1.isEmpty() || crval2.isEmpty()) {
        emit messageAvailable("Query::getCatalogSearchLocationAstrom(): CRVAL vectors are empty", "error");
        emit criticalReceived();
        successProcessing = false;
        return;
    }

    // Use median to calculate field center (avoiding issues with RA=0|360 deg boundary)
    // Do not average central two elements if number of data points is even
    alphaCenter = QString::number(straightMedian_T(crval1, 0, false), 'f', 3);
    deltaCenter = QString::number(straightMedian_T(crval2, 0, false), 'f', 3);
}
