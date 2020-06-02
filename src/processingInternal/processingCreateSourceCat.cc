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

    // Ensure that the output path exists
    QString outpath = mainDirName+"/"+scienceData->subDirName+"/cat/";
    mkAbsDir(outpath);

    pushBeginMessage(taskBasename, scienceDir);
    pushConfigCreatesourcecat();

    memoryDecideDeletableStatus(scienceData, false);

    if (!scienceData->hasMatchingPartnerFiles(mainDirName+"/WEIGHTS/", ".weight.fits")) return;

    // Update coordinates if necessary. Leave if cancelled by the user

    bool check = manualCoordsUpdate(scienceData, updateMode);
    if (!check) return;

    QString minFWHM = cdw->ui->CSCFWHMLineEdit->text();
    QString maxFlag = cdw->ui->CSCmaxflagLineEdit->text();

    getNumberOfActiveImages(scienceData);

    // Cleanup the catalog directory from the results of a previous run
    scienceData->removeCatalogs();

    // Check if exposures have unambiguous MJD-OBS keywords, otherwise the construction of scamp catalogs will fail
    if (!scienceData->collectMJD()) {
        emit messageAvailable("Duplicate MJD-OBS entries found!", "error");
        emit criticalReceived();
        return;
    }

    emit messageAvailable("Running source extraction ...", "data");

    // INTERNAL
    if (cdw->ui->CSCMethodComboBox->currentText() == "THELI") {
        detectionInternal(scienceData, minFWHM, maxFlag);
        emit resetProgressBar();
        progressStepSize = 100. / float(scienceData->exposureList.length());
        mergeInternal(scienceData, minFWHM, maxFlag);
    }

    // EXTERNAL (SourceExtractor)
    else {
        detectionSourceExtractor(scienceData, minFWHM, maxFlag);
        emit resetProgressBar();
        progressStepSize = 100. / float(scienceData->exposureList.length());
        mergeSourceExtractor(scienceData);
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
        if (it->activeState != MyImage::ACTIVE) continue;

        releaseMemory(nimg*instData->storage, maxCPU);

        if (verbosity > 1 ) emit messageAvailable(it->chipName + " : Creating source catalog ...", "image");
        it->setupDataInMemorySimple(true);
        if (!it->successProcessing) {
            abortProcess = true;
            continue;
        }
        it->checkWCSsanity();
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
            emitSourceCountMessage(nobj, it->chipName);
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

void Controller::detectionSourceExtractor(Data *scienceData, QString minFWHM, QString maxFlag)
{
    buildSourceExtractorCommandOptions();
    // Create source catalogs for each exposure

    /*
#pragma omp parallel for num_threads(maxExternalThreads)
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : scienceData->myImageList[chip]) {
            if (verbosity >=0 ) emit messageAvailable(it->baseName + " : Creating SourceExtractor catalog ...", "image");
            it->setupDataInMemory(isTaskRepeated, true, true);
            it->buildSourceExtractorCommand();
            it->sourceExtractorCommand.append(sourceExtractorCommandOptions);
            it->createSourceExtractorCatalog();
            it->filterSourceExtractorCatalog(minFWHM, maxFlag);
            it->sourceExtractorCatToIview();
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

    float nimg = 4;  // some breathing space for SourceExtractor
    releaseMemory(nimg*instData->storage*maxCPU, 1);
    scienceData->protectMemory();

#pragma omp parallel for num_threads(maxCPU)
    for (int k=0; k<numMyImages; ++k) {
        if (abortProcess || !successProcessing) continue;

        auto &it = allMyImages[k];
        int chip = it->chipNumber - 1;
        if (!it->successProcessing) continue;
        if (it->activeState != MyImage::ACTIVE) continue;
        if (instData->badChips.contains(chip)) continue;

        releaseMemory(nimg*instData->storage, maxCPU);

        if (verbosity > 1) emit messageAvailable(it->chipName + " : Creating source catalog ...", "image");
        it->setupDataInMemorySimple(true);
        if (!it->successProcessing) {
            abortProcess = true;
            continue;
        }
        it->checkWCSsanity();
        it->buildSourceExtractorCommand();
        it->sourceExtractorCommand.append(sourceExtractorCommandOptions);
        if (!it->imageOnDrive) it->writeImage();         // Must be on drive for SourceExtractor
        it->createSourceExtractorCatalog();
        it->filterSourceExtractorCatalog(minFWHM, maxFlag);
        it->calcMedianSeeingEllipticitySex();
        it->sourceExtractorCatToIview();
        it->sourceExtractorCatToAnet();
        it->unprotectMemory();
        if (minimizeMemoryUsage) {
            it->freeAll();
        }
        if (it->successProcessing) {
            long nobj = getNumObjectsSourceExtractorCat(it->path+"/cat/"+it->chipName+".cat");
            emitSourceCountMessage(nobj, it->chipName);
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

void Controller::emitSourceCountMessage(long &nobj, QString baseName)
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
        int inactiveCounter = 0;
        for (auto &it : scienceData->exposureList[i]) {
            if (it->activeState == MyImage::LOWDETECTION) lowCounts = true;
            if (it->activeState == MyImage::INACTIVE) ++inactiveCounter;
        }
        if (lowCounts || inactiveCounter > 0) continue;       // WARNING: skipping images where some detectors have no data / deactivated

        fitsfile *fptr;
        int status = 0;
        QString filename = scienceData->dirName+"/cat/"+scienceData->exposureList[i][0]->rootName+".scamp";
        filename = "!"+filename;
        fits_create_file(&fptr, filename.toUtf8().data(), &status);
        int counter = 0;
        for (auto &it : scienceData->exposureList[i]) {
            // Exclude inactive exposures
//            if (it->activeState == MyImage::INACTIVE) continue;          // could exclude more flags
            it->appendToScampCatalogInternal(fptr, minFWHM, maxFlag);
            ++counter;
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

void Controller::mergeSourceExtractor(Data *scienceData)
{
    // Merge SourceExtractor catalogs to final scamp catalog
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
            it->appendToScampCatalogSourceExtractor(fptr);
            if (!it->successProcessing) scienceData->successProcessing = false;
        }
        fits_close_file(fptr, &status);

        printCfitsioError("mergeSourceExtractor()", status);

#pragma omp atomic
        progress += progressStepSize;
    }
}

void Controller::buildSourceExtractorCommandOptions()
{
    // This builds the major part of the SourceExtractor command based on the chosen GUI settings.
    // The MyImage class adds the variable rest (catalog name, weight name, image name)

    if (!successProcessing) return;

    sourceExtractorCommandOptions = " -PARAMETERS_NAME "  + thelidir + "/config/default.param";
    sourceExtractorCommandOptions += " -FILTER_NAME "     + thelidir + "/config/default.conv";
    sourceExtractorCommandOptions += " -STARNNW_NAME "    + thelidir + "/config/default.nnw";
    sourceExtractorCommandOptions += " -DETECT_MINAREA "  + getUserParamLineEdit(cdw->ui->CSCDMINLineEdit);
    sourceExtractorCommandOptions += " -DETECT_THRESH "   + getUserParamLineEdit(cdw->ui->CSCDTLineEdit);
    sourceExtractorCommandOptions += " -ANALYSIS_THRESH " + getUserParamLineEdit(cdw->ui->CSCDTLineEdit);
    sourceExtractorCommandOptions += " -FILTER "          + getUserParamCheckBox(cdw->ui->CSCconvolutionCheckBox);
    sourceExtractorCommandOptions += " -DEBLEND_MINCONT " + getUserParamLineEdit(cdw->ui->CSCmincontLineEdit);
    sourceExtractorCommandOptions += " -SATUR_LEVEL "     + getUserParamLineEdit(cdw->ui->CSCsaturationLineEdit);
    sourceExtractorCommandOptions += " -CATALOG_TYPE FITS_LDAC";
    sourceExtractorCommandOptions += " -WEIGHT_TYPE MAP_WEIGHT ";
    sourceExtractorCommandOptions += " -PIXEL_SCALE 0.0 ";

    QString backString = cdw->ui->CSCbackgroundLineEdit->text();
    if (backString.isEmpty()) sourceExtractorCommandOptions += " -BACK_TYPE AUTO " ;
    else sourceExtractorCommandOptions += " -BACK_TYPE MANUAL -BACK_VALUE "+backString;
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

        it->loadHeader();

        if (!it->successProcessing) {
            abortProcess = true;
            continue;
        }

        // TODO: should be sufficient, but crashes when executed right after launch
        // it->loadHeader();
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

    //    emit appendOK();
    return true;
}

