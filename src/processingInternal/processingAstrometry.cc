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
#include "../dockwidgets/monitor.h"
#include "../tools/tools.h"
#include "../tools/fitting.h"
#include "../tools/imagequality.h"
#include "../tools/correlator.h"
#include "photinst.h"
#include "ui_confdockwidget.h"

#include <QMetaObject>
#include <QVector>
#include <QStringList>
#include <QTextStream>
#include <QStandardPaths>
#include <QProgressBar>
#include <QTest>


void Controller::taskInternalCreatesourcecat()
{
    QString scienceDir = instructions.split(" ").at(1);
    QString updateMode = instructions.split(" ").at(2);
    Data *scienceData = getData(DT_SCIENCE, scienceDir);
    if (scienceData == nullptr) return;      // Error triggered by getData();
    if (!testResetDesire(scienceData)) return;

    currentData = scienceData;
    currentDirName = scienceDir;

    pushBeginMessage(taskBasename, scienceDir);
    pushConfigCreatesourcecat();

    memoryDecideDeletableStatus(scienceData, false);

    // Update coordinates if necessary. Leave if cancelled by the user

    bool check = manualCoordsUpdate(scienceData, updateMode);
    if (!check) return;

    QString minFWHM = cdw->ui->CSCFWHMLineEdit->text();
    QString maxFlag = cdw->ui->CSCmaxflagLineEdit->text();

    getNumberOfActiveImages(scienceData);

    // Cleanup the catalog directory from the results of a previous run
    scienceData->removeCatalogs();

    // INTERNAL
    if (cdw->ui->CSCMethodComboBox->currentText() == "THELI") {
        detectionInternal(scienceData, minFWHM, maxFlag);
        emit resetProgressBar();
        progressStepSize = 100. / float(scienceData->exposureList.length());
        mergeInternal(scienceData, minFWHM, maxFlag);
    }

    // EXTERNAL (SExtractor)
    else {
        detectionSExtractor(scienceData, minFWHM, maxFlag);
        emit resetProgressBar();
        progressStepSize = 100. / float(scienceData->exposureList.length());
        mergeSExtractor(scienceData);
    }

    checkSuccessProcessing(scienceData);
    satisfyMaxMemorySetting();

    if (successProcessing) {
        emit progressUpdate(100);
        //        pushEndMessage(taskBasename, scienceDir);
    }
}

void Controller::detectionInternal(Data *scienceData, QString minFWHM, QString maxFlag)
{
    // Parameters for source detection
    QString DT = cdw->ui->CSCDTLineEdit->text();
    QString DMIN = cdw->ui->CSCDMINLineEdit->text();
    QString background = cdw->ui->CSCbackgroundLineEdit->text();
    bool convolution = cdw->ui->CSCconvolutionCheckBox->isChecked();
    QString saturation = cdw->ui->CSCsaturationLineEdit->text();

    // Create source catalogs for each exposure (keep them in memory!)
    /*
#pragma omp parallel for num_threads(maxExternalThreads)
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : scienceData->myImageList[chip]) {
            if (verbosity >=0 ) emit messageAvailable(it->baseName + " : Creating source catalog ...", "image");
            it->setupDataInMemory(isTaskRepeated, true, true);
            it->backgroundModel(256, "interpolate");
            it->segmentImage(DT, DMIN, convolution, false);
            it->writeCatalog(minFWHM, maxFlag);  // filters out objects that don't match flag or fwhm
            incrementProgress();
        }
    }
*/

    QString warningLevel = "";
    QString stopLevel = "";
    long numExpRejected = 0;
    long numImgRejected = 0;

    QList<MyImage*> allMyImages;
    long numMyImages = makeListofAllImages(allMyImages, scienceData);

    float nimg = 4;  // detection
    releaseMemory(nimg*instData->storage*maxCPU, 1);
    scienceData->protectMemory();

    doDataFitInRAM(numMyImages*instData->numUsedChips, instData->storage);

#pragma omp parallel for num_threads(maxCPU)
    for (int k=0; k<numMyImages; ++k) {
        if (abortProcess || !successProcessing) continue;

        auto &it = allMyImages[k];
        int chip = it->chipNumber - 1;

        if (!it->successProcessing) continue;
        if (instData->badChips.contains(chip)) continue;

        releaseMemory(nimg*instData->storage, maxCPU);

        if (verbosity > 1 ) emit messageAvailable(it->chipName + " : Creating source catalog ...", "image");
        it->setupDataInMemorySimple(true);
        if (!it->successProcessing) {
            abortProcess = true;
            continue;
        }
        it->readWeight();
        it->backgroundModel(256, "interpolate");
        it->updateSaturation(saturation);
        if (it->dataBackground.capacity() == 0) {
            if (successProcessing) emit messageAvailable(it->chipName + " : Background vector has zero capacity!", "error");
            criticalReceived();
            successProcessing = false;
        }
        it->segmentImage(DT, DMIN, convolution, false);
        it->releaseBackgroundMemory();
        it->releaseDetectionPixelMemory();
        it->calcMedianSeeingEllipticity();   // Also propagate to FITS header if file is on drive
        it->writeCatalog(minFWHM, maxFlag);  // filters out objects that don't match flag or fwhm
        it->unprotectMemory();
        if (minimizeMemoryUsage) {
            it->freeAll();
        }
        if (it->successProcessing) {
            long nobj = it->objectList.length();
            emitSourceCountMessage(nobj, it->chipName, warningLevel, stopLevel);
            if (!cdw->ui->CSCrejectExposureLineEdit->text().isEmpty()) {
                long nReject = cdw->ui->CSCrejectExposureLineEdit->text().toLong();
                if (nobj < nReject) {
                    it->setActiveState(MyImage::LOWDETECTION);
                    it->emitModelUpdateNeeded();
                    it->removeSourceCatalogs();
                }
            }
        }
        else {
            scienceData->successProcessing = false;
        }
#pragma omp atomic
        progress += progressStepSize;
    }

    // Deactivate exposures with low detections
    if (!cdw->ui->CSCrejectExposureLineEdit->text().isEmpty()) {
        flagLowDetectionImages(scienceData, numExpRejected, numImgRejected);
        if (numImgRejected > 0) {
            QString addedString = "";
            if (instData->numChips > 1) addedString = "("+QString::number(numImgRejected)+ " detector images)";
            emit messageAvailable("<br>"+QString::number(numExpRejected) + " exposures "+addedString+ " deactivated " +
                                  "(low source count). Corresponding FITS files were moved to <br>" +
                                  scienceData->subDirName+"/inactive/lowDetections/<br>", "warning");
            emit warningReceived();
        }
    }
}

void Controller::detectionSExtractor(Data *scienceData, QString minFWHM, QString maxFlag)
{
    buildSexCommandOptions();
    // Create source catalogs for each exposure

    /*
#pragma omp parallel for num_threads(maxExternalThreads)
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : scienceData->myImageList[chip]) {
            if (verbosity >=0 ) emit messageAvailable(it->baseName + " : Creating SExtractor source catalog ...", "image");
            it->setupDataInMemory(isTaskRepeated, true, true);
            it->buildSexCommand();
            it->sexCommand.append(sexCommandOptions);
            it->createSextractorCatalog();
            it->filterSextractorCatalog(minFWHM, maxFlag);
            it->sexcatToIview();
            incrementProgress();
        }
    }
*/

    QString warningLevel = "";
    QString stopLevel = "";
    long numExpRejected = 0;
    long numImgRejected = 0;

    QList<MyImage*> allMyImages;
    long numMyImages = makeListofAllImages(allMyImages, scienceData);

    float nimg = 4;  // some breathing space for SExtractor
    releaseMemory(nimg*instData->storage*maxCPU, 1);
    scienceData->protectMemory();

#pragma omp parallel for num_threads(maxCPU)
    for (int k=0; k<numMyImages; ++k) {
        if (abortProcess || !successProcessing) continue;

        auto &it = allMyImages[k];
        int chip = it->chipNumber - 1;
        if (!it->successProcessing) continue;
        if (instData->badChips.contains(chip)) continue;

        releaseMemory(nimg*instData->storage, maxCPU);

        if (verbosity > 1) emit messageAvailable(it->chipName + " : Creating source catalog ...", "image");
        it->setupDataInMemorySimple(true);
        if (!it->successProcessing) {
            abortProcess = true;
            continue;
        }
        it->buildSexCommand();
        it->sexCommand.append(sexCommandOptions);
        if (!it->imageOnDrive) it->writeImage();         // Must be on drive for sextractor
        it->createSextractorCatalog();
        it->filterSextractorCatalog(minFWHM, maxFlag);
        it->calcMedianSeeingEllipticitySex();
        it->sexcatToIview();
        it->unprotectMemory();
        if (minimizeMemoryUsage) {
            it->freeAll();
        }
        if (it->successProcessing) {
            long nobj = getNumObjectsSexCat(it->path+"/cat/"+it->chipName+".cat");
            emitSourceCountMessage(nobj, it->chipName, warningLevel, stopLevel);
            if (!cdw->ui->CSCrejectExposureLineEdit->text().isEmpty()) {
                long nReject = cdw->ui->CSCrejectExposureLineEdit->text().toLong();
                if (nobj < nReject) {
                    it->setActiveState(MyImage::LOWDETECTION);
                    it->emitModelUpdateNeeded();
                    it->removeSourceCatalogs();
                }
            }
        }
        else {
            scienceData->successProcessing = false;
            successProcessing = false;
        }
#pragma omp atomic
        progress += progressStepSize;
    }

    // Deactivate exposures with low detections
    if (!cdw->ui->CSCrejectExposureLineEdit->text().isEmpty()) {
        flagLowDetectionImages(scienceData, numExpRejected, numImgRejected);
        if (numImgRejected > 0) {
            QString addedString = "";
            if (instData->numChips > 1) addedString = "("+QString::number(numImgRejected)+ " detector images)";
            emit messageAvailable("<br>"+QString::number(numExpRejected) + " exposures "+addedString+ " deactivated " +
                                  "(low source count). Corresponding FITS files were moved to <br>" +
                                  scienceData->subDirName+"/inactive/lowDetections/", "warning");
            emit warningReceived();
        }
    }
}

void Controller::emitSourceCountMessage(long &nobj, QString baseName, QString &warningLevel, QString &stopLevel)
{
    // Color-coding output lines
    QString detStatus = "";
    QString level = "image";
    if (nobj<10 && nobj>3) {
        detStatus = " (low source count)";
        level = "warning";
    }
    if (nobj<=3) {
        detStatus = " (very low source count)";
        level = "stop";
    }
    emit messageAvailable(baseName + " : " + QString::number(nobj) + " sources detected..."+detStatus, level);
}

void Controller::flagLowDetectionImages(Data *scienceData, long &numExpRejected, long &numImgRejected)
{
    numExpRejected = 0;
    numImgRejected = 0;

    scienceData->populateExposureList();
    int numExposures = scienceData->exposureList.length();
    for (long i=0; i<numExposures; ++i) {
        if (abortProcess || !successProcessing) continue;
        // Test if a single chip has low detection numbers
        bool exposureIsBad = false;
        for (auto &it : scienceData->exposureList[i]) {
            if (it->activeState == MyImage::LOWDETECTION) {
                exposureIsBad = true;
                break;
            }
        }
        // Reject all chips if a single one is bad
        if (exposureIsBad) {
            for (auto &it : scienceData->exposureList[i]) {
                ++numImgRejected;
                if (it->activeState != MyImage::LOWDETECTION) {
                    it->setActiveState(MyImage::LOWDETECTION);
                    it->emitModelUpdateNeeded();
                    it->removeSourceCatalogs();
                }
                it->emitModelUpdateNeeded();
            }
            ++numExpRejected;
        }
    }
}

void Controller::mergeInternal(Data *scienceData, QString minFWHM, QString maxFlag)
{
    scienceData->populateExposureList();
    emit messageAvailable("Merging source catalogs ...", "controller");
    for (long i=0; i<scienceData->exposureList.length(); ++i) {
        // if one detector has been rejected due to low source counts, then skip this exposure
        bool lowCounts = false;
        for (auto &it : scienceData->exposureList[i]) {
            if (it->activeState == MyImage::LOWDETECTION) lowCounts = true;
        }
        if (lowCounts) continue;

        fitsfile *fptr;
        int status = 0;
        QString filename = scienceData->dirName+"/cat/"+scienceData->exposureList[i][0]->rootName+".scamp";
        filename = "!"+filename;
        fits_create_file(&fptr, filename.toUtf8().data(), &status);
        int counter=0;
        for (auto &it : scienceData->exposureList[i]) {
            // Could exclude catalogs due to any activeState
            //            if (it->activeState != MyImage::LOWDETECTION) {
            it->appendToScampCatalogInternal(fptr, minFWHM, maxFlag);
            ++counter;
            //            }
        }
        //        if (counter != instData->numChips) {
        if (counter != instData->numUsedChips) {
            emit messageAvailable(scienceData->exposureList[i][0]->rootName + " : Merged only " + QString::number(counter)
                    + " out of " + QString::number(instData->numUsedChips) + " catalogs for scamp!", "error");
            emit criticalReceived();
        }

        fits_close_file(fptr, &status);

        printCfitsioError("mergeInternal()", status);

#pragma omp atomic
        progress += progressStepSize;
    }
}

void Controller::mergeSExtractor(Data *scienceData)
{
    // Merge Sextractor catalogs to final scamp catalog
    // Loop over exposures
    scienceData->populateExposureList();
    emit messageAvailable("Merging source catalogs ...", "controller");

    progress = 0.;
    progressStepSize = 100./float(scienceData->exposureList.length());

#pragma omp parallel for num_threads(maxCPU)
    for (long i=0; i<scienceData->exposureList.length(); ++i) {
        if (abortProcess || !successProcessing) continue;
        // if one detector has been rejected due to low source counts, then skip this exposure
        bool lowCounts = false;
        for (auto &it : scienceData->exposureList[i]) {
            if (it->activeState == MyImage::LOWDETECTION) lowCounts = true;
        }
        if (lowCounts) continue;

        fitsfile *fptr;
        int status = 0;
        QString filename = scienceData->dirName+"/cat/"+scienceData->exposureList[i][0]->rootName+".scamp";
        filename = "!"+filename;
        fits_create_file(&fptr, filename.toUtf8().data(), &status);
        for (auto &it : scienceData->exposureList[i]) {
            it->appendToScampCatalogSExtractor(fptr);
            if (!it->successProcessing) scienceData->successProcessing = false;
        }
        fits_close_file(fptr, &status);

        printCfitsioError("mergeSExtractor()", status);

#pragma omp atomic
        progress += progressStepSize;
    }
}

void Controller::buildSexCommandOptions()
{
    // This builds the major part of the sextractor command based on the chosen GUI settings.
    // The MyImage class adds the variable rest (catalog name, weight name, image name)

    if (!successProcessing) return;

    sexCommandOptions = " -PARAMETERS_NAME " + thelidir + "/src/config/default.param";
    sexCommandOptions += " -FILTER_NAME "     + thelidir + "/src/config/default.conv";
    sexCommandOptions += " -STARNNW_NAME "    + thelidir + "/src/config/default.nnw";
    sexCommandOptions += " -DETECT_MINAREA "  + getUserParamLineEdit(cdw->ui->CSCDMINLineEdit);
    sexCommandOptions += " -DETECT_THRESH "   + getUserParamLineEdit(cdw->ui->CSCDTLineEdit);
    sexCommandOptions += " -ANALYSIS_THRESH " + getUserParamLineEdit(cdw->ui->CSCDTLineEdit);
    sexCommandOptions += " -FILTER "          + getUserParamCheckBox(cdw->ui->CSCconvolutionCheckBox);
    sexCommandOptions += " -DEBLEND_MINCONT " + getUserParamLineEdit(cdw->ui->CSCmincontLineEdit);
    sexCommandOptions += " -SATUR_LEVEL "     + getUserParamLineEdit(cdw->ui->CSCsaturationLineEdit);
    sexCommandOptions += " -CATALOG_TYPE FITS_LDAC";
    sexCommandOptions += " -WEIGHT_TYPE MAP_WEIGHT ";
    sexCommandOptions += " -PIXEL_SCALE 0.0 ";

    QString backString = cdw->ui->CSCbackgroundLineEdit->text();
    if (backString.isEmpty()) sexCommandOptions += " -BACK_TYPE AUTO " ;
    else sexCommandOptions += " -BACK_TYPE MANUAL -BACK_VALUE "+backString;
}

bool Controller::manualCoordsUpdate(Data *scienceData, QString mode)
{
    if (!successProcessing) return false;
    if (mode == "Cancel") return false;
    if (mode.isEmpty() || mode == "empty") return true;

    QString targetAlpha = cdw->ui->ARCraLineEdit->text();
    QString targetDelta = cdw->ui->ARCdecLineEdit->text();
    if (targetAlpha.contains(':')) targetAlpha = hmsToDecimal(targetAlpha);
    if (targetDelta.contains(':')) targetDelta = dmsToDecimal(targetDelta);

    emit messageAvailable("Updating WCS:", "controller");
    if (mode == "crval") {
        emit messageAvailable("CRVAL1 = "+targetAlpha + "<br>" +
                              "CRVAL2 = "+targetDelta, "data");
    }
    else if (mode == "crval+cd") {
        emit messageAvailable("CRVAL1 = "+targetAlpha + "<br>" +
                              "CRVAL2 = "+targetDelta + "<br>" +
                              "CD1_1 = "+QString::number(-1.*instData->pixscale/3600.) + "<br>" +
                              "CD1_2 = 0.0<br>" +
                              "CD2_1 = 0.0<br>" +
                              "CD2_2 = "+QString::number(instData->pixscale/3600.), "data");
    }

    getNumberOfActiveImages(scienceData);

    QList<MyImage*> allMyImages;
    long numMyImages = makeListofAllImages(allMyImages, scienceData);

#pragma omp parallel for num_threads(maxCPU)
    for (int k=0; k<numMyImages; ++k) {
        auto &it = allMyImages[k];
        int chip = it->chipNumber - 1;
        if (instData->badChips.contains(chip)) continue;

        if (!it->successProcessing) continue;
        it->setupDataInMemorySimple(false);
        if (!it->successProcessing) {
            abortProcess = true;
            continue;
        }
        // TODO: should be sufficient, but crashes when executed right after launch
        // it->provideHeaderInfo();
        if (mode == "crval") {
            it->wcs->crval[0] = targetAlpha.toDouble();
            it->wcs->crval[1] = targetDelta.toDouble();
            it->wcs->flag = 0;
            it->updateCRVALinHeader();
            it->updateCRVALinHeaderOnDrive();
        }
        if (mode == "crval+cd") {
            it->wcs->crval[0] = targetAlpha.toDouble();
            it->wcs->crval[1] = targetDelta.toDouble();
            it->wcs->cd[0] = -1.*it->plateScale/3600.;
            it->wcs->cd[1] = 0.;
            it->wcs->cd[2] = 0.;
            it->wcs->cd[3] = it->plateScale/3600.;
            it->wcs->flag = 0;
            it->updateCRVALCDinHeader();
            it->updateCRVALCDinHeaderOnDrive();
        }
        if (!it->successProcessing) scienceData->successProcessing = false;
#pragma omp atomic
        progress += progressStepSize;
    }

    successProcessing = true;

    emit messageAvailable("<br>Now running source detection ...", "controller");

    //    emit appendOK();
    return true;
}

void Controller::taskInternalAstromphotom()
{
    QString scienceDir = instructions.split(" ").at(1);
    Data *scienceData = getData(DT_SCIENCE, scienceDir);
    if (scienceData == nullptr) return;      // Error triggered by getData();
    if (!testResetDesire(scienceData)) return;

    currentData = scienceData;
    currentDirName = scienceDir;

    if (!scienceData->successProcessing) return;

    mainGUI->ui->processProgressBar->setDisabled(true);

    scampScienceDir = mainDirName+"/"+scienceDir;
    scampDir = scampScienceDir+"/astrom_photom_scamp/cat/";
    scampPlotsDir = mainDirName+"/"+scienceDir + "/plots/";
    scampHeadersDir = mainDirName+"/"+scienceDir + "/headers/";
    scampScienceData = scienceData;

    pushBeginMessage(taskBasename, scienceDir);
    pushConfigAstromphotom();

    // DAISY-CHAINING. The end of the scamp run triggers the iView display of the checkplots,
    // which in turn triggers the header update

    if (cdw->ui->ASTmethodComboBox->currentText() == "Scamp") {
        QString catDirName = mainDirName + "/" + scienceDir + "/cat/";
        if (!scienceData->hasMatchingPartnerFiles(catDirName, ".scamp")) return;

        // Prepare scamp directories and perform consistency checks
        prepareScampRun(scienceData);
        // Collect scamp input catalogs
        long totNumObjects = 0;
        long numCats = prepareScampCats(scienceData, totNumObjects);
        if (numCats == 0) return;

        // Release memory:
        // 140 bytes per detection
        // "a few 10 kB" per FITS table
        //
        long totBytes = 0;
        int degrees = cdw->ui->ASTdistortLineEdit->text().toInt();
        totBytes += totNumObjects * 140 + (2*50000*instData->numChips)*numCats;
        long Nt = numCats * 1 * (degrees*degrees+2) + numCats*6;
        totBytes += 8*Nt*Nt;
        totBytes = 2*totBytes/ 1024. / 1024.;
        if (verbosity >= 1) emit messageAvailable("Scamp memory estimate: " + QString::number(long(totBytes)) + " MB", "controller");
        releaseMemory(totBytes, 1);

        progress = 0.;
        progressStepSize = 80. / (float) numCats;  // 80%, leaving some space for distortion solution.
        // Build the scamp command
        buildScampCommand(scienceData);

        // Run the Scamp command
        workerThread = new QThread();
        scampWorker = new ScampWorker(scampCommand, scampDir, instData->shortName);
        workerInit = true;
        workerThreadInit = true;
        scampWorker->moveToThread(workerThread);

        connect(workerThread, &QThread::started, scampWorker, &ScampWorker::runScamp);
        // Qt::DirectConnection is bad here, because this task runs in a different thread than the main controller,
        // meaning that if e.g. sky sub is activated as well it will start immediately even before the scamp checkplots are shown
        // connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater, Qt::DirectConnection);
        // connect(scampWorker, &ScampWorker::finished, workerThread, &QThread::quit, Qt::DirectConnection);
        // connect(scampWorker, &ScampWorker::finished, scampWorker, &QObject::deleteLater, Qt::DirectConnection);
        connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);
        connect(scampWorker, &ScampWorker::errorFound, this, &Controller::errorFoundReceived);
        connect(scampWorker, &ScampWorker::finishedScamp, this, &Controller::finishedScampReceived);
        // Need the direct connection if we want the thread to actually return control to the main thread (activating the start button again).
        // But then sky sub would start before checkplots are evaluated.
        // CORRECT WAY: call workerThread->quit() at the end of the image quality analysis
        //        connect(scampWorker, &ScampWorker::finished, workerThread, &QThread::quit, Qt::DirectConnection);
        //       connect(scampWorker, &ScampWorker::finished, workerThread, &QThread::quit);
        connect(scampWorker, &ScampWorker::finished, scampWorker, &QObject::deleteLater);
        connect(scampWorker, &ScampWorker::messageAvailable, monitor, &Monitor::displayMessage);
        connect(scampWorker, &ScampWorker::fieldMatched, this, &Controller::fieldMatchedReceived);
        workerThread->start();
        workerThread->wait();
    }
    else if (cdw->ui->ASTmethodComboBox->currentText() == "Cross-correlation") {

    }
    else if (cdw->ui->ASTmethodComboBox->currentText() == "Header") {
        // TODO
    }
    else {
        // No other method yet
    }
}

int Controller::getMaxPhotInst()
{
    if (!successProcessing) return 0;

    // Parse the output headers and extract fluxscales and exposure times
    QDir dir(scampDir);
    QStringList filterList("*.head");
    QStringList MEFheaders = dir.entryList(filterList);

    int maxPhotInst = 0;
    for (auto &it : MEFheaders) {
        QFile MEF(scampDir+"/"+it);
        QTextStream inStream(&MEF);
        if (!MEF.open(QIODevice::ReadOnly)) {
            continue;
        }
        QString line;
        int photinst = 0;
        while (inStream.readLineInto(&line)) {
            if (line.contains("PHOTINST=")) photinst = line.split("=")[1].split("/")[0].simplified().toInt();
        }
        MEF.close();
        if (photinst > maxPhotInst) maxPhotInst = photinst;
    }
    return maxPhotInst;
}

void Controller::scampCalcFluxscale()
{
    if (!successProcessing) return;

    // Parse the output headers and extract fluxscales and exposure times
    QDir dir(scampDir);
    QStringList filterList("*.head");
    QStringList MEFheaders = dir.entryList(filterList);
    if (MEFheaders.isEmpty()) {
        successProcessing = false;
        emit messageAvailable("Could not find scamp output headers.", "error");
        criticalReceived();
        return;
    }

    bool success = true;

    int maxPhotInst = getMaxPhotInst();
    for (int i=0; i<maxPhotInst; ++i) {
        PhotInst *photInst = new PhotInst(this);
        photInstruments.append(photInst);
    }

    long MEFindex = 0;
    for (auto &it : MEFheaders) {
        QFile MEF(scampDir+"/"+it);
        QFileInfo MEFinfo(MEF);
        QString MEFbasename = MEFinfo.completeBaseName();
        QTextStream inStream(&MEF);
        if (!MEF.open(QIODevice::ReadOnly)) {
            success = false;
            continue;
        }
        QString line;
        double fluxscale = -1.;
        double exptime = -1.;
        QString basename = "";
        int photinst_idx = 0;
        while (inStream.readLineInto(&line)) {
            if (line.contains("FLXSCALE=")) fluxscale = line.split("=")[1].split("/")[0].simplified().toDouble();
            if (line.contains("PHOTINST=")) photinst_idx = line.split("=")[1].split("/")[0].simplified().toInt() - 1;
        }
        MEF.close();

        // Find the associated exposure time
        for (auto &it : scampScienceData->myImageList[0]) {
            it->provideHeaderInfo();
            if (it->baseName.contains(MEFbasename)) {
                exptime = it->exptime;
                basename = it->baseName;
                break;
            }
        }
        photInstruments[photinst_idx]->fluxScale.append(fluxscale);
        photInstruments[photinst_idx]->expTime.append(exptime);
        photInstruments[photinst_idx]->baseName.append(basename);
        // Mapping the running overall MEF file index to the running index of MEF file index for the current phot instrument
        photInstruments[photinst_idx]->indexMap.insert(MEFindex, photInstruments[photinst_idx]->fluxScale.length()-1);
        ++MEFindex;
    }

    // Calculate RZP and FLXSCALE for each photometric instrument
    for (auto &pi : photInstruments) {
        pi->getRZP();
    }

    if (!success) {
        successProcessing = false;
    }
}

void Controller::finishedScampReceived()
{
    if (!successProcessing) return;

    // Move the checkplots to their final place
    moveFiles("*.png", scampDir, scampPlotsDir);

    // Calculate the mean RZP and FLXSCALE per photometric instrument
    scampCalcFluxscale();

    // Split the scamp headers into individual chips, adding RZP and FLXSCALE
    splitScampHeaders();

    mainGUI->ui->processProgressBar->setEnabled(true);
    emit progressUpdate(100);
    //    pushEndMessage(taskBasename, scampScienceDir);

    photInstruments.clear();

    emit showScampCheckPlots();
}

void Controller::fieldMatchedReceived()
{
#pragma omp atomic
    progress += progressStepSize;
}

void Controller::splitScampHeaders()
{
    if (!successProcessing) return;

    // Split the MEF scamp header files into individual chips and move them to their final place
    QDir dir(scampDir);
    QStringList filterList("*.head");
    QStringList MEFheaders = dir.entryList(filterList);
    bool success = true;
    long MEFindex = 0;
    for (auto &it : MEFheaders) {
        int chip = 1;
        QFile MEF(scampDir+"/"+it);
        QFileInfo MEFinfo(MEF);
        QTextStream inStream(&MEF);
        if (!MEF.open(QIODevice::ReadOnly)) {
            success = false;
            break;
        }
        QFile HEAD;
        QTextStream outStream;
        QString line;
        int i=0;
        while (inStream.readLineInto(&line)) {
            if (i==0) {
                if (instData->badChips.contains(chip-1)) ++chip;
                HEAD.setFileName(scampHeadersDir+"/"+MEFinfo.completeBaseName()+"_"+QString::number(chip)+".head");
                outStream.setDevice(&HEAD);
                if( !HEAD.open(QIODevice::WriteOnly)) {
                    success = false;
                    break;
                }
            }
            // THELI style flux scaling
            if (line.contains("FLXSCALE=")) line.replace("FLXSCALE=","FLSCALE =");        // preserve the original FLXSCALE value
            if (line.contains("PHOTINST")) {
                int photinst_idx = line.split("=")[1].split("/")[0].simplified().toInt() - 1;
                long idx = photInstruments[photinst_idx]->indexMap.value(MEFindex);
                QString line1 = "FLXSCALE= "+QString::number(photInstruments[photinst_idx]->fluxScale[idx],'g',12) + " / THELI relative flux scale";
                QString line2 = "RZP     = "+QString::number(photInstruments[photinst_idx]->RZP[idx],'g',12) + " / THELI relative zeropoint";
                outStream << line1 << "\n";
                outStream << line2 << "\n";
            }
            outStream << line << "\n";
            ++i;
            if (line.simplified() == "END") {
                HEAD.close();
                HEAD.setPermissions(QFile::ReadUser | QFile::WriteUser);
                i=0;
                ++chip;
            }
        }
        MEF.close();
        ++MEFindex;
    }
    if (!success) {
        successProcessing = false;
        emit messageAvailable("Could not parse one or more of the scamp output header files.", "error");
        criticalReceived();
    }
}

// unused, done in copy_zero_order()
/*
void Controller::updateMyImagesWithScampSolution(Data *scienceData)
{
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : scienceData->myImageList[chip]) {
            // header file
            QFile headerFile(scienceData->dirName + "/headers/" + it->chipName + ".head");
            QString RZP = get_fileHeaderParameter(&headerFile, "RZP");
            it->RZP = RZP.toFloat();
            it->updateHeaderValue("RZP", RZP);  // like that it will be included in the FITS file when written
            it->updateHeaderValueInFITS("RZP", RZP);   // Optionally, updates the FITS file as well (if on drive)
        }
    }
}
*/

void Controller::showScampCheckPlotsReceived()
{
    if (!successProcessing) return;

    emit messageAvailable("<br>*** Inspect the astrometric solution and accept or reject it. ***<br>", "warning");

    IView *checkplotViewer = new IView("SCAMP", scampPlotsDir, this);
    // blocks until seeing determination is done, which is silly and i don't understand why
    // checkplotViewer->setWindowModality(Qt::ApplicationModal);
    checkplotViewer->show();
    connect(checkplotViewer, &IView::solutionAcceptanceState, this, &Controller::registerScampSolutionAcceptance);
    // Avoid THELI hogging memory during the session with every call to iview
    connect(checkplotViewer, &IView::closed, checkplotViewer, &IView::deleteLater);
    connect(checkplotViewer, &IView::closed, this, &Controller::continueWithCopyZeroOrder);
}

void Controller::registerScampSolutionAcceptance(bool scampSolutionAccepted)
{
    scampSolutionAcceptanceState = scampSolutionAccepted;

    if (scampSolutionAccepted) {
        emit messageAvailable("*** Scamp solution accepted ***<br>", "note");
    }
    else {
        emit messageAvailable("*** Scamp solution rejected ***<br>", "warning");
    }
}

void Controller::continueWithCopyZeroOrder()
{
    if (!successProcessing) return;

    if (scampSolutionAcceptanceState) copyZeroOrder();
    else {
        monitor->raise();
        successProcessing = false;
        workerThread->quit();
        successProcessing = true;
    }
}

void Controller::copyZeroOrder()
{
    if (!successProcessing) return;

    pushBeginMessage("CopyZeroOrder", scampScienceDir);

    if (!QDir(scampHeadersDir).exists()) {
        emit messageAvailable("Could not update images with 0th order WCS solution.<br>The headers/ subdirectory does not exist in<br>"+scampScienceDir, "error");
        successProcessing = false;
        monitor->raise();
        workerThread->quit();
        successProcessing = true;
        return;
    }

#pragma omp parallel for num_threads(maxExternalThreads)
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (abortProcess || !successProcessing || instData->badChips.contains(chip)) continue;
        for (auto &it : scampScienceData->myImageList[chip]) {
            if (abortProcess) break;
            it->setupDataInMemorySimple(false);
            if (!it->successProcessing) {
                abortProcess = true;
                continue;
            }
            //            it->backupOrigHeader(chip);                        // Create a backup copy of the original FITS headers if it doesn't exist yet
            if (it->scanAstromHeader(chip, "inHeadersDir")) {
                it->updateZeroOrderOnDrive("update");            // Overwrite 0-th order solution in FITS header (if on drive)
                it->updateZeroOrderInMemory();                   // Overwrite 0-th order solution in memory
            }
            // The following is done by scanAstromheader and updateZeroOrderOnDrive
            /*
            // Copy RZP from header file
            QFile headerFile(scampScienceData->dirName + "/headers/" + it->chipName + ".head");
            QString RZP = get_fileHeaderParameter(&headerFile, "RZP");
            it->RZP = RZP.toFloat();
            it->updateHeaderValue("RZP", RZP);                   // like that it will be included in the FITS file when written
            it->updateHeaderValueInFITS("RZP", RZP);             // Optionally, updates the FITS file as well (if on drive)
            */
            it->unprotectMemory();
            if (minimizeMemoryUsage) {
                it->freeAll();
            }
            if (!it->successProcessing) successProcessing = false;
        }
    }

    //    pushEndMessage("CopyZeroOrder", scampScienceDir);

    doImageQualityAnalysis();
}

void Controller::doImageQualityAnalysis()
{
    if (!successProcessing) return;

    pushBeginMessage("ImageQuality", scampScienceDir);

    // Todo: replace with GAIA catalog if that was the reference catalog already!
    downloadGaiaCatalog(scampScienceData); // Point sources

    QList<MyImage*> allMyImages;
    long numMyImages = makeListofAllImages(allMyImages, scampScienceData);

#pragma omp parallel for num_threads(maxCPU) firstprivate(mainDirName, gaiaQuery, instData)
    for (int k=0; k<numMyImages; ++k) {
        if (abortProcess || !successProcessing) continue;
        auto &it = allMyImages[k];
        int chip = it->chipNumber - 1;
        if (!it->successProcessing) continue;
        if (instData->badChips.contains(chip)) continue;

        it->setupDataInMemorySimple(false);
        if (!it->successProcessing) {
            abortProcess = true;
            continue;
        }
        // Setup seeing measurement
        it->estimateMatchingTolerance();
        ImageQuality *imageQuality = new ImageQuality(scampScienceData, instData, mainDirName);
        imageQuality->matchingTolerance = it->matchingTolerance;
        imageQuality->baseName = it->chipName;
        // pass the reference data
        collectGaiaRaDec(it, gaiaQuery->de_out, gaiaQuery->ra_out, imageQuality->refCat);
        // pass the source data (dec, ra, fwhm, ell on one hand, and mag separately)
        it->collectSeeingParameters(imageQuality->sourceCat, imageQuality->sourceMag, instData->chipMap.value(chip));
        // match
        bool gaia = imageQuality->getSeeingFromGaia();
        it->fwhm = imageQuality->fwhm;                                                     // Updating MyImage fwhm parameter
        it->updateHeaderValue("FWHM", imageQuality->fwhm);                                 // Updating MyImage header string
        it->updateHeaderValue("ELLIP", imageQuality->ellipticity);
        it->updateHeaderValueInFITS("FWHM", QString::number(imageQuality->fwhm, 'f', 3));  // Updating the current FITS image on drive
        it->updateHeaderValueInFITS("ELLIP", QString::number(imageQuality->ellipticity, 'f', 3));
        //        if (!gaia) imageQuality->getSeeingFromRhMag();      TODO: Not yet implemented
        if (verbosity > 1) emit messageAvailable(it->chipName + " : FWHM / Ellipticity / # stars = "
                                                 + QString::number(imageQuality->fwhm, 'f', 3) + " / "
                                                 + QString::number(imageQuality->ellipticity, 'f', 3) + " / "
                                                 + QString::number(imageQuality->numSources), "ignore");
        delete imageQuality;
        it->unprotectMemory();
        if (minimizeMemoryUsage) {
            it->freeAll();
        }
    }

    workerThread->quit();

    delete gaiaQuery;

//    emit messageAvailable("You can display the results of the image quality analysis in the statistics module", "note");

    successProcessing = true;
    //    pushEndMessage("ImageQuality", scampScienceDir);
}

// X-correlation works only for instruments with a single chip!
void Controller::doCrossCorrelation(Data *scienceData)
{
    if (!successProcessing) return;

    QString DT = cdw->ui->ASTxcorrDTLineEdit->text();
    QString DMIN = cdw->ui->ASTxcorrDMINLineEdit->text();

    if (DT.isEmpty()) DT = "3.0";
    if (DMIN.isEmpty()) DMIN = "5";

    //#pragma omp parallel for num_threads(maxExternalThreads)
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (abortProcess || !successProcessing || instData->badChips.contains(chip)) continue;
        for (auto &it : scienceData->myImageList[chip]) {
            if (abortProcess) break;
            if (!it->successProcessing) continue;
            emit messageAvailable(it->baseName + " : Building xcorrelation pixel map ...", "controller");
            it->setupDataInMemorySimple(false);
            if (!it->successProcessing) {
                abortProcess = true;
                continue;
            }
            it->readWeight();
            it->resetObjectMasking();
            it->backgroundModel(64, "interpolate");
            it->segmentImage(DT, DMIN, true, false);
            it->makeXcorrData();
            if (!it->successProcessing) successProcessing = false;
            // TODO
            // write image
            // run python script instead of fftw
            /*
            // Correlate image with reference image (first in series)
            Correlator correlator(it, scienceData->myImageList[chip][0]);
            correlator.xcorrelate();
            QVector<float> peakPos = correlator.find_peak();
            if (peakPos.isEmpty()) it->successProcessing = false;
            */
            it->releaseAllDetectionMemory();
        }
    }
    satisfyMaxMemorySetting();
}

void Controller::prepareScampRun(Data *scienceData)
{
    if (!successProcessing) return;

    if (verbosity > 0) emit messageAvailable("Setting up Scamp run ...", "controller");

    QString scienceDir = mainDirName+"/"+scienceData->subDirName;
    QString path = scienceDir+"/astrom_photom_scamp/";
    QString headersPath = scienceDir+"/headers/";
    QString plotsPath = scienceDir+"/plots/";

    // Clean-up and recreate the scamp directory trees
    QDir dir(path);
    dir.removeRecursively();
    dir.mkpath(path);
    dir.mkdir("cat");
    dir.mkdir("headers");
    dir.mkdir("plots");
    dir.mkdir("backup");

    QDir plotsDir(plotsPath);
    plotsDir.removeRecursively();
    plotsDir.mkpath(plotsPath);

    QDir headersDir(headersPath);
    headersDir.removeRecursively();
    headersDir.mkpath(headersPath);

    QFile file(tmpdir+"/scamp_global.ahead");
    file.remove();

    // Check if the reference catalog exists
    QFile refcat(scienceDir+"/cat/refcat/theli_mystd.scamp");
    if (!refcat.exists()) {
        emit messageAvailable("The astrometric reference catalog does not exist, or was not created!", "error");
        successProcessing = false;
        monitor->raise();
        return;
    }

    //    emit appendOK();
}

long Controller::prepareScampCats(Data *scienceData, long &totNumObjects)
{
    if (!successProcessing) return 0;

    QString scienceDir = mainDirName+"/"+scienceData->subDirName;
    QString path = scienceDir+"/astrom_photom_scamp/";

    if (verbosity > 0) emit messageAvailable("Linking scamp catalogs to "+path+"/cat/ ...", "controller");

    // Prepare a file that contains all scamp catalogs (only those for which an image is currently present in scienceDir);
    QFile catFile(tmpdir+"/scamp_cats");
    catFile.remove();
    QTextStream stream(&catFile);
    if( !catFile.open(QIODevice::WriteOnly)) {
        emit messageAvailable("Writing list of scamp catalogs to "+tmpdir+catFile.fileName(), "error");
        emit messageAvailable(catFile.errorString(), "error");
        successProcessing = false;
        monitor->raise();
        return 0;
    }

    // Link scamp catalogs (only those for currently active images)
    long numCat = 0;
    QStringList imageList;
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        for (auto &it : scienceData->myImageList[chip]) {
            if (it->activeState == MyImage::ACTIVE) imageList << it->chipName;
        }
    }

    //    QDir imageDir(scienceDir);
    //    statusOld = scienceData->status;
    //    QStringList imageList = imageDir.entryList(QStringList("*"+statusOld+".fits"));
    QDir catDir(scienceDir+"/cat/");
    QStringList catList = catDir.entryList(QStringList("*.scamp"));
    for (auto &cat : catList) {
        QString catbase = cat;
        totNumObjects += getNumObjectsScampCat(scienceDir+"/cat/"+catbase);
        catbase.remove(".scamp");
        for (auto &image : imageList) {
            if (image.contains(catbase)) {
                QFile catFile(scienceDir+"/cat/"+cat);
                catFile.link(path+"/cat/"+cat);
                stream << path+"/cat/"+cat+"\n";
                ++numCat;
                break;
            }
        }
    }
    catFile.close();
    catFile.setPermissions(QFile::ReadUser | QFile::WriteUser);

    if (numCat == 0) {
        emit messageAvailable("No cat/*.scamp catalogs were found matching the exposures in "+scienceDir+"<br>Did you create the source catalogs?", "error");
        monitor->raise();
        successProcessing = false;
        return 0;
    }

    //    emit appendOK();

    return numCat;
}

long Controller::getNumObjectsScampCat(QString cat)
{
    int status = 0;
    fitsfile *fptr;
    fits_open_file(&fptr, cat.toUtf8().data(), READONLY, &status);
    // LDAC_OBJECTS tables are found in extensions 3, 5, 7, ..., internally referred to as 2, 4, 6, ...
    int hduType = 0;
    long nobj = 0;
    for (int chip=1; chip<=instData->numUsedChips; ++chip) {
        fits_movabs_hdu(fptr, 2*chip+1, &hduType, &status);
        long nrows = 0;
        fits_get_num_rows(fptr, &nrows, &status);
        nobj += nrows;
    }
    fits_close_file(fptr, &status);

    printCfitsioError("getNumObjectsScampCat():<br>" + cat, status);

    return nobj;
}

long Controller::getNumObjectsSexCat(QString cat)
{
    int status = 0;
    fitsfile *fptr;
    fits_open_file(&fptr, cat.toUtf8().data(), READONLY, &status);
    // LDAC_OBJECTS table is found in extensions 3, internally referred to as 2
    int hduType = 0;
    fits_movabs_hdu(fptr, 3, &hduType, &status);
    long nrows = 0;
    fits_get_num_rows(fptr, &nrows, &status);
    fits_close_file(fptr, &status);

    printCfitsioError("getNumObjectsSexCat():", status);

    return nrows;
}

void Controller::buildScampCommand(Data *scienceData)
{
    if (!successProcessing) return;

    QString refcat = mainDirName+"/"+scienceData->subDirName;
    refcat.append("/cat/refcat/theli_mystd.scamp");

    QString distGroups = "1,1";
    QString distGroupsUser = cdw->ui->ASTdistortgroupsLineEdit->text();
    if (!distGroupsUser.isEmpty()) distGroups.append(","+distGroupsUser);

    QString distKeys = "XWIN_IMAGE,YWIN_IMAGE";
    QString distKeysUser = cdw->ui->ASTdistortkeysLineEdit->text();
    if (!distKeysUser.isEmpty()) distKeys.append(","+distKeysUser);

    scampCommand = findExecutableName("scamp");
    scampCommand += " @"+tmpdir+"/scamp_cats";
    scampCommand += " -NTHREADS " + QString::number(maxCPU);
    scampCommand += " -ASTREF_CATALOG FILE";
    scampCommand += " -ASTREFCAT_NAME "  + refcat;
    scampCommand += " -ASTREF_WEIGHT "   + getUserParamLineEdit(cdw->ui->ASTastrefweightLineEdit);
    scampCommand += " -ASTRINSTRU_KEY "  + getUserParamLineEdit(cdw->ui->ASTastrinstrukeyLineEdit);
    scampCommand += " -CROSSID_RADIUS "  + getUserParamLineEdit(cdw->ui->ASTcrossidLineEdit);
    scampCommand += " -DISTORT_DEGREES " + getUserParamLineEdit(cdw->ui->ASTdistortLineEdit);
    scampCommand += " -DISTORT_GROUPS "  + distGroups;
    scampCommand += " -DISTORT_KEYS "    + distKeys;
    scampCommand += " -PIXSCALE_MAXERR " + getUserParamLineEdit(cdw->ui->ASTpixscaleLineEdit);
    scampCommand += " -POSANGLE_MAXERR " + getUserParamLineEdit(cdw->ui->ASTposangleLineEdit);
    scampCommand += " -POSITION_MAXERR " + getUserParamLineEdit(cdw->ui->ASTpositionLineEdit);
    scampCommand += " -SN_THRESHOLDS "   + getUserParamLineEdit(cdw->ui->ASTsnthreshLineEdit);
    scampCommand += " -STABILITY_TYPE "  + getUserParamComboBox(cdw->ui->ASTstabilityComboBox);
    scampCommand += " -MOSAIC_TYPE "     + getUserParamComboBox(cdw->ui->ASTmosaictypeComboBox);
    scampCommand += " -MATCH_FLIPPED "   + getUserParamCheckBox(cdw->ui->ASTmatchflippedCheckBox);
    scampCommand += " -CHECKPLOT_RES "   + getUserParamLineEdit(cdw->ui->ASTresolutionLineEdit);

    QString value = cdw->ui->ASTastrinstrukeyLineEdit->text();
    if (value == "") value = "NONE";
    scampCommand += " -ASTRINSTRU_KEY "  + value;

    value = cdw->ui->ASTphotinstrukeyLineEdit->text();
    if (value == "") value = "NONE";
    scampCommand += " -PHOTINSTRU_KEY " + value;

    QString matching = "Y";
    if (cdw->ui->ASTmatchMethodComboBox->currentIndex() > 0) matching = "N";
    scampCommand += " -MATCH " + matching;

    if (verbosity >= 1) emit messageAvailable("Executing the following scamp command :<br><br>"+scampCommand+"<br><br>in directory: <br><br>"+scampDir+"<br>", "info");
    if (verbosity >= 1) emit messageAvailable("<br>Scamp output<br>", "ignore");
}
