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
#include "../tools/swarpfilter.h"
#include "../tools/fitting.h"
#include "../tools/fileprogresscounter.h"
#include "ui_confdockwidget.h"
#include "../threading/scampworker.h"
#include "../readmes/swarpreadme.h"
#include "../tools/imagequality.h"

#include "unistd.h"   // To query max number of open files

#include <QMetaObject>
#include <QVector>
#include <QStringList>
#include <QStandardPaths>
#include <QTextStream>
#include <QTest>
#include <QTimer>
#include <QProgressBar>

void Controller::taskInternalCoaddition()
{
    coaddScienceDir = instructions.split(" ").at(1);
    QString filterArg = instructions.split(" ").at(2);

    coaddScienceData = getData(DT_SCIENCE, coaddScienceDir);
    if (coaddScienceData == nullptr) return;      // Error triggered by getData();

    currentData = coaddScienceData;
    currentDirName = coaddScienceDir;

    memoryDecideDeletableStatus(coaddScienceData, false);
    QString coaddSubDir = "coadd_"+filterArg;
    QString coaddUniqueID = cdw->ui->COAuniqueidLineEdit->text();
    if (!coaddUniqueID.isEmpty()) coaddSubDir.append("_"+coaddUniqueID);
    coaddDirName = mainDirName + "/" + coaddScienceDir + "/" + coaddSubDir;
    QString headerDirName = mainDirName + "/" + coaddScienceDir + "/headers/";

    if (!coaddScienceData->hasMatchingPartnerFiles(headerDirName, ".head")) return;

    //    qDebug() << "taskInternalCoaddition() : " << filterArg;

    // TODO
    // The following is very fast and does not need a parallel loop on top.

    // If coming here right after launch:
    // #pragma omp parallel for num_threads(maxExternalThreads)
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : coaddScienceData->myImageList[chip]) {
            it->provideHeaderInfo();
        }
    }

    // The various coadd tasks are daisy-chained through signals and slots
    coaddSmoothEdge();

    QTest::qWait(1000);

    coaddPrepare(filterArg);   // entry point to daisy chain
}

void Controller::coaddSmoothEdge()
{
    if (!successProcessing) return;

    QString edgeString = cdw->ui->COAedgesmoothingLineEdit->text();
    if (edgeString.isEmpty()) return;
    float edge = edgeString.toFloat();

    bool roundEdge = true;

    pushBeginMessage("CoaddSmoothWeights", coaddScienceData->subDirName);

    getNumberOfActiveImages(coaddScienceData);

    /*
#pragma omp parallel for num_threads(maxExternalThreads)
    for (int chip=0; chip<instData->numChips; ++chip) {
        incrementCurrentThreads(lock);
        for (auto &it : coaddScienceData->myImageList[chip]) {
            emit messageAvailable(it->baseName + " : Smoothing weight edge ...", "controller");
            it->readWeight();
            it->roundEdgeOfWeight(edge, roundEdge);
            it->writeWeightSmoothed(mainDirName+ "/WEIGHTS/" + it->chipName + ".weightsmooth.fits");
            it->weightName = it->chipName + ".weight";
            QVector<float>().swap(it->dataWeightSmooth);  // immediately release the memory
            incrementProgress();
        }
    }
    */

    QList<MyImage*> allMyImages;
    long numMyImages = makeListofAllImages(allMyImages, coaddScienceData);
    progressStepSize = 100. / float(numMyImages);

    float nimg = 2;  // wild guess
    releaseMemory(nimg*instData->storage*maxCPU, 1);
    coaddScienceData->protectMemory();

#pragma omp parallel for num_threads(maxCPU) firstprivate(mainDirName)
    for (int k=0; k<numMyImages; ++k) {
        if (abortProcess || !successProcessing) continue;

        releaseMemory(nimg*instData->storage, maxCPU);

        auto &it = allMyImages[k];
        if (!it->successProcessing) continue;
        int chip = it->chipNumber - 1;
        if (instData->badChips.contains(chip)) continue;
        emit messageAvailable(it->baseName + " : Smoothing weight edge ...", "controller");
        it->readWeight();
        it->roundEdgeOfWeight(edge, roundEdge);
        it->writeWeightSmoothed(mainDirName+ "/WEIGHTS/" + it->chipName + ".weightsmooth.fits");
        it->weightName = it->chipName + ".weight";
        // immediately release the memory
        it->dataWeightSmooth.clear();
        it->dataWeightSmooth.squeeze();
        it->unprotectMemory();
        if (it->successProcessing) coaddScienceData->successProcessing = false;
#pragma omp atomic
        progress += progressStepSize;
    }

    checkSuccessProcessing(coaddScienceData);
    satisfyMaxMemorySetting();
    if (successProcessing) {
        coaddScienceData->status = statusNew;
        emit progressUpdate(100);
        //        pushEndMessage("CoaddSmoothWeights", coaddScienceData->subDirName);
    }
}

void Controller::coaddPrepare(QString filterArg)
{
    if (!successProcessing) return;

    pushBeginMessage("CoaddPrepare", coaddScienceDir);
    pushConfigCoadd();

    emit resetProgressBar();

    // Release at least as much RAM as nCPU*nchip*2
    releaseMemory(maxCPU*instData->numChips*2*instData->storage, 1);

    QString ident = cdw->ui->COAuniqueidLineEdit->text();
    QString chips = cdw->ui->COAchipsLineEdit->text();
    QVector<int> chipList;
    QStringList chipStringList = chips.replace(","," ").simplified().split(" ");
    if (!chips.isEmpty()) {
        for (auto &chip : chipStringList) chipList.append(chip.toInt()-1);
    }
    else {
        // include all chips
        for (int chip=0; chip<instData->numChips; ++chip) {
            if (instData->badChips.contains(chip)) continue;
            chipList.append(chip);
        }
    }

    QString edgeSmooth = cdw->ui->COAedgesmoothingLineEdit->text();

    // Check if coaddDir exists. Delete everything in it if yes. Then recreate it.
    QDir coaddDir(coaddDirName);
    if (coaddDir.exists()) coaddDir.removeRecursively();
    coaddDir.mkpath(coaddDirName);

    // Calculate median RA / DEC if not given manually
    QString refRA = cdw->ui->COAraLineEdit->text();
    QString refDE = cdw->ui->COAdecLineEdit->text();
    if (!refRA.isEmpty() && refRA.contains(":")) refRA = hmsToDecimal(refRA);
    if (!refDE.isEmpty() && refDE.contains(":")) refDE = dmsToDecimal(refDE);
    if (refRA.isEmpty() || refDE.isEmpty()) {
        QVector<double> alpha;
        QVector<double> delta;
        for (int chip=0; chip<instData->numChips; ++chip) {
            if (instData->badChips.contains(chip)) continue;
            for (auto &it : coaddScienceData->myImageList[chip]) {
                if (!it->successProcessing) continue;
                alpha.append(it->alpha_ctr);
                delta.append(it->delta_ctr);
            }
        }
        double alphaMedian = straightMedian_T(alpha, 0, false); // do not average center two elements because of the RA 0|360 deg boundary
        double deltaMedian = straightMedian_T(delta);
        refRA = QString::number(alphaMedian, 'f', 9);
        refDE = QString::number(deltaMedian, 'f', 9);
    }

    // Progress bar is updated some fraction of a second. We just need to set the variable.
    progress = 30.;
    //    emit progressUpdate(progress);

    // For proper motion correction we need the MJD-OBS of the first exposure, taken from the first chip
    QVector<double> mjdobsData;
    for (auto &it : coaddScienceData->myImageList[0]) {
        if (!it->successProcessing) continue;
        it->provideHeaderInfo();
        mjdobsData.append(it->mjdobs);
    }
    double mjdobsZero = minVec_T(mjdobsData);
    mjdStart = mjdobsZero;
    mjdEnd = maxVec_T(mjdobsData);
    mjdMedian = straightMedian_T(mjdobsData, 0, false);
    bool doPMupdate = false;
    if (!cdw->ui->COApmraLineEdit->text().isEmpty() && !cdw->ui->COApmdecLineEdit->text().isEmpty()) doPMupdate = true;
    if (mjdobsZero == 0. && doPMupdate) {
        emit showMessageBox("Controller::NO_MJDOBS_FOR_PM", "", "");
        successProcessing = false;
        return;
    }

    // Copy the headers (might be modified), link the images and weights, if they match the filterArg
    float numexp = 0;
    QList<QString> filterNames;
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (!chipList.contains(chip) || instData->badChips.contains(chip)) continue;        // Skip chips that should not be coadded
        for (auto &it : coaddScienceData->myImageList[chip]) {
            if (!it->successProcessing) continue;
            it->provideHeaderInfo();
            QFile image(it->path + "/" + it->baseName + ".fits");
            QFile weight(it->weightPath + "/" + it->weightName + ".fits");
            if (!edgeSmooth.isEmpty()) weight.setFileName(it->weightPath + "/" + it->weightName + "smooth.fits");
            QString headerNewName = coaddDirName+"/" + it->baseName + ".head";
            QFile header(it->path + "/headers/" + it->chipName + ".head");
            if (filterArg == "all") {
                image.link(coaddDirName+"/" + it->baseName + ".fits");
                weight.link(coaddDirName+"/" + it->baseName + ".weight.fits");
                if (!doPMupdate) header.copy(headerNewName);
                else coaddPrepareProjectPM(header, headerNewName, refRA, refDE, mjdobsZero, it->mjdobs);
                coaddTexptime += it->exptime;
                coaddSkyvalue += it->skyValue / it->exptime;
                ++numexp;
                if (!filterNames.contains(it->filter)) filterNames.append(it->filter);
            }
            else {
                if (it->filter == filterArg) {
                    image.link(coaddDirName+"/" + it->baseName + ".fits");
                    weight.link(coaddDirName+"/" + it->baseName + ".weight.fits");
                    if (!doPMupdate) header.copy(coaddDirName+"/" + it->baseName + ".head");
                    else coaddPrepareProjectPM(header, headerNewName, refRA, refDE, mjdobsZero, it->mjdobs);
                    if (!filterNames.contains(it->filter)) filterNames.append(it->filter);
                }
                coaddTexptime += it->exptime;
                coaddSkyvalue += it->skyValue / it->exptime;
                ++numexp;
            }
        }
    }

    coaddTexptime /= instData->numUsedChips;
    coaddSkyvalue /= numexp;
    for (auto &filter : filterNames) {
        coaddFilter.append(filter);
        coaddFilter.append("+");
    }
    coaddFilter.truncate(coaddFilter.length()-1);
    coaddGain = coaddTexptime;

    // Check if Swarp will open more file handles than the system currently allows
    long maxOpenFiles = sysconf(_SC_OPEN_MAX);
    if (2*numexp > maxOpenFiles) {
        successProcessing = false;
        SwarpReadme *swarpReadme = new SwarpReadme(2*numexp, maxOpenFiles, this);
        swarpReadme->show();
        return;
    }

    // Run swarp to create coadd.head
    //    tmpCoaddDir = coaddDirName;
    //    tmpCoaddData = scienceData;

    progress = 50.;
    //   emit progressUpdate(progress);

    coaddPrepareBuildSwarpCommand(refRA, refDE);

    // Run the Swarp command
    workerThreadPrepare = new QThread();
    swarpWorker = new SwarpWorker(swarpCommand, coaddDirName, "swarpPreparation");
    workerInit = true;
    workerThreadInit = true;
    swarpWorker->moveToThread(workerThreadPrepare);

    connect(workerThreadPrepare, &QThread::started, swarpWorker, &SwarpWorker::runSwarp, Qt::DirectConnection);
    //    connect(workerThreadPrepare, &QThread::finished, workerThreadPrepare, &QThread::deleteLater, Qt::DirectConnection);
    connect(workerThreadPrepare, &QThread::finished, workerThreadPrepare, &QThread::deleteLater);
    connect(swarpWorker, &SwarpWorker::errorFound, this, &Controller::errorFoundReceived);
    connect(swarpWorker, &SwarpWorker::finishedPreparation, this, &Controller::finishedPreparationReceived);
    // MUST NOT connect to quit, otherwise control returns immediately to the controller, which might start another coaddition
    // right away in case of multi-color data
    //    connect(swarpWorker, &SwarpWorker::finished, workerThreadPrepare, &QThread::quit, Qt::DirectConnection);

    //    connect(swarpWorker, &SwarpWorker::finished, swarpWorker, &QObject::deleteLater, Qt::DirectConnection);
    //    connect(swarpWorker, &SwarpWorker::finished, workerThreadPrepare, &QThread::quit);
    connect(swarpWorker, &SwarpWorker::finished, swarpWorker, &QObject::deleteLater);
    connect(swarpWorker, &SwarpWorker::messageAvailable, monitor, &Monitor::displayMessage);

    //qDebug() << "coaddPrepare(): before threadstart" << coaddDirName;

    workerThreadPrepare->start();
    workerThreadPrepare->wait();

    // finishedXXX signal triggers next step
}


void Controller::errorFoundReceived()
{
    successProcessing = false;
}


// UNUSED

void Controller::finishedPreparationReceived()
{
    progress = 70.;
    //   emit progressUpdate(progress);

    foldCoaddFits();

    if (!successProcessing) return;

    // If requested, rotate coadd.head to user-specified position angle
    coaddPrepareProjectRotation();

    progress = 100.;
    //   emit progressUpdate(progress);

    // Trigger resampling

    emit swarpStartResampling();
}

void Controller::coaddPrepareProjectPM(QFile &headerFileOld, QString newHeaderName, QString refRA, QString refDE, double mjdobsZero, double mjdobsNow)
{
    if (!successProcessing) return;

    double pmRA = cdw->ui->COApmraLineEdit->text().toDouble();
    double pmDE = cdw->ui->COApmdecLineEdit->text().toDouble();
    int unitIndex = cdw->ui->COApmComboBox->currentIndex();
    double timeConversion = 1.;
    if (unitIndex == 0) timeConversion = 1./60.;       // arcsec / sec
    else if (unitIndex == 1) timeConversion = 1.;      // arcsec / min
    else if (unitIndex == 2) timeConversion = 60.;     // arcsec / hr
    else if (unitIndex == 3) timeConversion = 1440.;   // arcsec / day

    // Calculate speed in CRVAL1/2 per minute
    double crval1 = refRA.toDouble();
    double crval2 = refDE.toDouble();
    double crval1Speed = pmRA / 3600. / timeConversion / cos(crval2*3.14159/180.);
    double crval2Speed = pmDE / 3600. / timeConversion;

    // How many minutes passed since the first image was taken
    double timeDiff = (mjdobsNow - mjdobsZero) * 1440.;

    // New CRVAL1/2 for the current exposure
    //    double crval1New = crval1 + timeDiff * crval1Speed;
    //    double crval2New = crval2 + timeDiff * crval2Speed;

    // Update CRVAL1/2 (and write to a new header file)
    QTextStream streamIn(&headerFileOld);
    if( !headerFileOld.open(QIODevice::ReadOnly)) {
        QString part1 = headerFileOld.fileName();
        QString part2 = headerFileOld.errorString();
        emit showMessageBox("Controller::CANNOT_UPDATE_HEADER_WITH_PM_READ", part1, part2);
        successProcessing = false;
        return;
    }
    QFile headerFileNew(newHeaderName);
    QTextStream streamOut(&headerFileNew);
    if( !headerFileNew.open(QIODevice::WriteOnly)) {
        QString part1 = headerFileNew.fileName();
        QString part2 = headerFileNew.errorString();
        emit showMessageBox("Controller::CANNOT_UPDATE_HEADER_WITH_PM_WRITE", part1, part2);
        successProcessing = false;
        return;
    }


    // CHECK CHECK CHECK:
    // MPC: what is the difference between coordinate motion and sky motion?
    // It seems I got it right, but for a target at DEC=3. Higher declinations? don't need to divide crval1speed by cosine dec(?)

    while(!streamIn.atEnd()) {
        QString line = streamIn.readLine();
        if (!line.contains("CRVAL")) streamOut << line << "\n";
        else {
            QString value = line.split("=").at(1);
            int slashPosition = value.lastIndexOf('/');  // (can be -1 if not found, treated as 0 by truncate(), i.e. entire string is set to empty
            if (slashPosition > 10) value.truncate(slashPosition);
            double crval1New = value.simplified().toDouble() - timeDiff * crval1Speed;
            double crval2New = value.simplified().toDouble() - timeDiff * crval2Speed;
            QString crval1String = "CRVAL1  = " + QString::number(crval1New, 'f', 12);
            QString crval2String = "CRVAL2  = " + QString::number(crval2New, 'f', 12);
            crval1String.resize(79, ' ');  // Must be 80 chars long to conform with FITS standard.
            crval2String.resize(79, ' ');  // We use 79 here because the read in stream also contains a carriage return!
            if (line.contains("CRVAL1")) streamOut << crval1String << "\n";
            if (line.contains("CRVAL2")) streamOut << crval2String << "\n";
        }
    }
    headerFileOld.close();
    headerFileNew.close();
    headerFileNew.setPermissions(QFile::ReadUser | QFile::WriteUser);
}

void Controller::coaddPrepareProjectRotation()
{
    if (!successProcessing) return;
    if (cdw->ui->COAskypaLineEdit->text().isEmpty()) return;

    // Rename coadd.head
    QFile coaddHeadOld(coaddDirName+"/coadd.head");
    coaddHeadOld.rename(coaddDirName+"/coadd.head_orig");

    // Read the CD matrix in coadd.head
    if(!coaddHeadOld.open(QIODevice::ReadOnly)) {
        QString part1 = coaddHeadOld.fileName();
        QString part2 = coaddHeadOld.errorString();
        emit showMessageBox("Controller::CANNOT_UPDATE_HEADER_WITH_PA", part1, part2);
        successProcessing = false;
        return;
    }

    double cd11_coadd = 0.;
    double cd12_coadd = 0.;
    double cd21_coadd = 0.;
    double cd22_coadd = 0.;
    long naxis1_coadd = 0;
    long naxis2_coadd = 0;
    QTextStream streamIn(&coaddHeadOld);
    while(!streamIn.atEnd()) {
        QString line = streamIn.readLine();
        if (line.contains("NAXIS1  =")) naxis1_coadd = line.split("=")[1].split("/")[0].simplified().toLong();
        if (line.contains("NAXIS2  =")) naxis2_coadd = line.split("=")[1].split("/")[0].simplified().toLong();
        if (line.contains("CD1_1   =")) cd11_coadd = line.split("=")[1].split("/")[0].simplified().toDouble();
        if (line.contains("CD1_2   =")) cd12_coadd = line.split("=")[1].split("/")[0].simplified().toDouble();
        if (line.contains("CD2_1   =")) cd21_coadd = line.split("=")[1].split("/")[0].simplified().toDouble();
        if (line.contains("CD2_2   =")) cd22_coadd = line.split("=")[1].split("/")[0].simplified().toDouble();
    }

    // PA of the original data (taken from first image)

    double cd11 = coaddScienceData->myImageList[0][0]->wcs->cd[0];
    double cd12 = coaddScienceData->myImageList[0][0]->wcs->cd[1];
    double cd21 = coaddScienceData->myImageList[0][0]->wcs->cd[2];
    double cd22 = coaddScienceData->myImageList[0][0]->wcs->cd[3];
    double PAold = -1. * getPosAnglefromCD(cd11, cd12, cd21, cd22);
    double PAnew = cdw->ui->COAskypaLineEdit->text().toDouble();

    rotateCDmatrix(cd11_coadd, cd12_coadd, cd21_coadd, cd22_coadd, PAnew);
    long naxis1New = 0;
    long naxis2New = 0;
    get_rotimsize(naxis1_coadd, naxis2_coadd, PAold, PAnew, naxis1New, naxis2New);

    // Updated header lines:
    QString str_naxis1 = "NAXIS1  = "+QString::number(naxis1New);
    QString str_naxis2 = "NAXIS2  = "+QString::number(naxis1New);
    QString str_crpix1 = "CRPIX1  = "+QString::number(naxis1New/2);
    QString str_crpix2 = "CRPIX2  = "+QString::number(naxis1New/2);
    QString str_cd11 = "CD1_1   = "+QString::number(cd11_coadd, 'f', 12);
    QString str_cd12 = "CD1_2   = "+QString::number(cd12_coadd, 'f', 12);
    QString str_cd21 = "CD2_1   = "+QString::number(cd21_coadd, 'f', 12);
    QString str_cd22 = "CD2_2   = "+QString::number(cd22_coadd, 'f', 12);
    str_naxis1.resize(80, ' ');
    str_naxis2.resize(80, ' ');
    str_crpix1.resize(80, ' ');
    str_crpix2.resize(80, ' ');
    str_cd11.resize(80, ' ');
    str_cd12.resize(80, ' ');
    str_cd21.resize(80, ' ');
    str_cd22.resize(80, ' ');

    // Open new coadd.head
    QFile coaddHeadNew(coaddDirName+"/coadd.head");
    if(!coaddHeadNew.open(QIODevice::WriteOnly)) {
        QString part1 = coaddHeadNew.fileName();
        QString part2 = coaddHeadNew.errorString();
        emit showMessageBox("Controller::CANNOT_UPDATE_HEADER_WITH_PA", part1, part2);
        successProcessing = false;
        return;
    }

    // Reset the old stream, and scan the file once more
    streamIn.seek(0);
    QTextStream streamOut(&coaddHeadNew);
    while(!streamIn.atEnd()) {
        QString line = streamIn.readLine();
        if (line.contains("NAXIS1")) streamOut << str_naxis1 << "\n";
        else if (line.contains("NAXIS2")) streamOut << str_naxis2 << "\n";
        else if (line.contains("CRPIX1")) streamOut << str_crpix1 << "\n";
        else if (line.contains("CRPIX2")) streamOut << str_crpix2 << "\n";
        else if (line.contains("CD1_1")) streamOut << str_cd11 << "\n";
        else if (line.contains("CD1_2")) streamOut << str_cd12 << "\n";
        else if (line.contains("CD2_1")) streamOut << str_cd21 << "\n";
        else if (line.contains("CD2_2")) streamOut << str_cd22 << "\n";
        else streamOut << line << "\n";
    }

    coaddHeadOld.close();
    coaddHeadNew.close();
    coaddHeadOld.remove();
    coaddHeadNew.setPermissions(QFile::ReadUser | QFile::WriteUser);
}

void Controller::coaddPrepareBuildSwarpCommand(QString refRA, QString refDE)
{
    if (!successProcessing) return;

    QString coaddSize = "";
    QString naxis1 = cdw->ui->COAsizexLineEdit->text();
    QString naxis2 = cdw->ui->COAsizeyLineEdit->text();
    if (!naxis1.isEmpty() && !naxis2.isEmpty()) coaddSize = "-IMAGE_SIZE "+naxis1+","+naxis2;

    QString pixelScale = cdw->ui->COApixscaleLineEdit->text();
    if (pixelScale.isEmpty()) pixelScale = QString::number(instData->pixscale, 'f', 9);

    statusOld = coaddScienceData->processingStatus->statusString;

    QString swarp = findExecutableName("swarp");
    swarpCommand = swarp + " *"+statusOld+".fits";
    swarpCommand += " -NTHREADS " + QString::number(maxCPU);
    swarpCommand += " -CENTER "+refRA+","+refDE;
    swarpCommand += " -CENTER_TYPE MANUAL";
    swarpCommand += " -VERBOSE_TYPE FULL";
    swarpCommand += " -HEADER_ONLY Y";
    // only in v2.40.1, not in earlier versions. Current latest official release: 2.38
    // swarpCommand += " -HEADEROUT_NAME coadd.head";
    swarpCommand += " -PIXELSCALE_TYPE MANUAL";
    swarpCommand += " -GAIN_KEYWORD GAIN";
    swarpCommand += " -PIXEL_SCALE "+pixelScale;
    swarpCommand += " -PROJECTION_TYPE " + cdw->ui->COAprojectionComboBox->currentText();
    swarpCommand += " -CELESTIAL_TYPE " + cdw->ui->COAcelestialtypeComboBox->currentText();
    swarpCommand += " -RESAMPLING_TYPE " + cdw->ui->COAkernelComboBox->currentText();
    swarpCommand += coaddSize;

    if (verbosity > 1) emit messageAvailable("Executing the following swarp command :<br><br>"+swarpCommand+"<br><br>in directory: <br><br>"+coaddDirName+"<br>", "info");
    if (verbosity > 1) emit messageAvailable("<br>Swarp output<br>", "ignore");
}

void Controller::coaddResampleBuildSwarpCommand(QString imageList, int i)
{
    if (!successProcessing) return;

    QString swarpCommand;   // shadowing the class member 'swarpCommand' for a moment

    QString pixelScale = cdw->ui->COApixscaleLineEdit->text();
    if (pixelScale.isEmpty()) pixelScale = QString::number(instData->pixscale, 'f', 9);

    QString rescaleWeights = "N";
    if (cdw->ui->COArescaleweightsCheckBox->isChecked()) rescaleWeights = "Y";

    statusOld = coaddScienceData->processingStatus->statusString;

    QString swarp = findExecutableName("swarp");
    swarpCommand = swarp +" @"+imageList;
    swarpCommand += " -NTHREADS 1";  // Launching maxCPU externally
    //    swarpCommand += " -NTHREADS " + QString::number(maxExternalThreads);
    swarpCommand += " -RESAMPLE Y";
    swarpCommand += " -RESAMPLE_SUFFIX ."+coaddUniqueID+"resamp.fits";
    swarpCommand += " -COMBINE N";
    swarpCommand += " -VERBOSE_TYPE FULL";
    swarpCommand += " -WEIGHT_TYPE MAP_WEIGHT";
    swarpCommand += " -GAIN_KEYWORD GAIN";
    swarpCommand += " -SUBTRACT_BACK N";
    swarpCommand += " -PROJECTION_TYPE " + cdw->ui->COAprojectionComboBox->currentText();
    swarpCommand += " -CELESTIAL_TYPE " + cdw->ui->COAcelestialtypeComboBox->currentText();
    swarpCommand += " -RESAMPLING_TYPE " + cdw->ui->COAkernelComboBox->currentText();
    swarpCommand += " -COMBINE_TYPE " + cdw->ui->COAcombinetypeComboBox->currentText();
    swarpCommand += " -RESCALE_WEIGHTS " + rescaleWeights;
    swarpCommand += " -COPY_KEYWORDS OBJECT,SKYVALUE";

    swarpCommands[i] = swarpCommand;

    if (verbosity > 1) emit messageAvailable("Executing the following swarp command :<br><br>"+swarpCommands[i]+"<br><br>in directory: <br><br>"+coaddDirName+"<br>", "info");
    if (verbosity > 1) emit messageAvailable("<br>Swarp output<br>", "ignore");
}

void Controller::coaddCoadditionBuildSwarpCommand(QString imageList)
{
    if (!successProcessing) return;

    QString pixelScale = cdw->ui->COApixscaleLineEdit->text();
    if (pixelScale.isEmpty()) pixelScale = QString::number(instData->pixscale, 'f', 9);

    QString rescaleWeights = "N";
    if (cdw->ui->COArescaleweightsCheckBox->isChecked()) rescaleWeights = "Y";

    statusOld = coaddScienceData->processingStatus->statusString;

    QString swarp = findExecutableName("swarp");
    swarpCommand = swarp +" @"+imageList;
    swarpCommand += " -NTHREADS " + QString::number(maxCPU);
    swarpCommand += " -RESAMPLE N";
    swarpCommand += " -COMBINE Y";
    swarpCommand += " -VERBOSE_TYPE FULL";
    swarpCommand += " -WEIGHT_TYPE MAP_WEIGHT";
    swarpCommand += " -BLANK_BADPIXELS Y";
    swarpCommand += " -SUBTRACT_BACK N";
    swarpCommand += " -GAIN_KEYWORD GAIN";
    swarpCommand += " -PROJECTION_TYPE " + cdw->ui->COAprojectionComboBox->currentText();
    swarpCommand += " -CELESTIAL_TYPE " + cdw->ui->COAcelestialtypeComboBox->currentText();
    swarpCommand += " -RESAMPLING_TYPE " + cdw->ui->COAkernelComboBox->currentText();
    swarpCommand += " -COMBINE_TYPE " + cdw->ui->COAcombinetypeComboBox->currentText();
    swarpCommand += " -RESCALE_WEIGHTS " + rescaleWeights;
    swarpCommand += " -COPY_KEYWORDS OBJECT";

    if (verbosity > 1) emit messageAvailable("Executing the following swarp command :<br><br>"+swarpCommand+"<br><br>in directory: <br><br>"+coaddDirName+"<br>", "info");
    if (verbosity > 1) emit messageAvailable("<br>Swarp output<br>", "ignore");
}

void Controller::coaddResample()
{
    if (!successProcessing) return;

    progress = 0.;
    emit resetProgressBar();

    pushBeginMessage("CoaddResample", coaddScienceDir);

    statusOld = coaddScienceData->processingStatus->statusString;

    // List of all images
    QDir coaddDir(coaddDirName);
    QStringList filter("*"+statusOld+".fits");
    QStringList imageList = coaddDir.entryList(filter);

    // Lists containing output files and streams
    QString listName;
    QFile file;
    QTextStream stream;
    QStringList listNames;
    int i = 0;
    int fileCount = 0;
    int highGroup = imageList.length() % maxCPU - 1;
    for (auto &imageName : imageList) {
        if (i==0) {
            listName = tmpdir+"/coaddList_"+QString::number(fileCount);
            listNames << listName;
            file.setFileName(listName);
            stream.setDevice(&file);
            if (!file.open(QIODevice::WriteOnly)) {
                QString part1 = listName;
                QString part2 = file.errorString();
                emit showMessageBox("Controller::CANNOT_WRITE_RESAMPLE_LIST", part1, part2);
                successProcessing = false;
                return;
            }
        }
        stream << imageName << "\n";
        ++i;
        /*
        if (i == imageList.length() / maxCPU) {
            i = 0;
            ++fileCount;
            file.close();
            file.setPermissions(QFile::ReadUser | QFile::WriteUser);
        }
        */
        // Spread images as evenly as possible across the threads
        if (fileCount <= highGroup && i == imageList.length() / maxCPU + 1) {
            i = 0;
            ++fileCount;
            file.close();
            file.setPermissions(QFile::ReadUser | QFile::WriteUser);
        }
        if (fileCount > highGroup && i == imageList.length() / maxCPU) {
            i = 0;
            ++fileCount;
            file.close();
            file.setPermissions(QFile::ReadUser | QFile::WriteUser);
        }
    }

    if (file.isOpen()) {
        file.close();
        file.setPermissions(QFile::ReadUser | QFile::WriteUser);
    }

    numberFileCounter = new FileProgressCounter(coaddDirName, "*resamp.fits", imageList.length(), &progress, this);
    connect(numberFileCounter->timer, &QTimer::timeout, numberFileCounter, &FileProgressCounter::numberFileProgress);
    //   connect(numberFileCounter, &FileProgressCounter::progressUpdate, mainGUI, &MainWindow::progressUpdateReceived, Qt::DirectConnection);
    numberFileCounter->timer->start(2000);

    // We can use at most as many threads as we have files in listname:
    localMaxCPU = maxCPU > listNames.length() ? listNames.length() : maxCPU;

#pragma omp parallel for num_threads(localMaxCPU) firstprivate(coaddDirName)
    for (int i=0; i<localMaxCPU; ++i) {
        coaddResampleBuildSwarpCommand(listNames[i], i);

        // Run the Swarp command
#pragma omp critical
        {
            workerThreads[i] = new QThread();
            swarpWorkers[i] = new SwarpWorker(swarpCommands[i], coaddDirName, "swarpResampling");
            workersInit = true;
            workerThreadsInit = true;
            swarpWorkers[i]->threadID = i;
            swarpWorkers[i]->moveToThread(workerThreads[i]);
        }
        connect(workerThreads[i], &QThread::started, swarpWorkers[i], &SwarpWorker::runSwarp);
        connect(workerThreads[i], &QThread::finished, workerThreads[i], &QThread::deleteLater);
        //        connect(workerThreads[i], &QThread::finished, workerThreads[i], &QThread::deleteLater);
        connect(swarpWorkers[i], &SwarpWorker::finishedResampling, this, &Controller::waitForResamplingThreads);
        //       connect(swarpWorkers[i], &SwarpWorker::finished, workerThreads[i], &QThread::quit);
        connect(swarpWorkers[i], &SwarpWorker::finished, swarpWorkers[i], &QObject::deleteLater);
        connect(swarpWorkers[i], &SwarpWorker::finished, workerThreads[i], &QThread::quit);
        //        connect(swarpWorkers[i], &SwarpWorker::finished, swarpWorkers[i], &QObject::deleteLater);
        connect(swarpWorkers[i], &SwarpWorker::messageAvailable, monitor, &Monitor::displayMessage);

        workerThreads[i]->start();
        //       workerThreads[i]->wait();
    }

    //    emit progressUpdate(100);
}

void Controller::waitForResamplingThreads(int threadID)
{
    omp_set_lock(&genericLock);

    threadsFinished[threadID] = true;
    // Trigger Swarpfilter if all resampling threads have finished
    bool allTrue = true;
    for (int i=0; i<localMaxCPU; ++i) {
        if (threadsFinished[i] == false) allTrue = false;
    }
    if (allTrue) {
        progress = 100.;
        //        emit progressUpdate(100);
        emit swarpStartSwarpfilter();
        numberFileCounter->timer->stop();
        // We don't have to delete the swarpWorkers because they are "movedTo" the threads
        //        for (auto &it : workerThreads) {
        //            delete it;
        //       }
    }

    omp_unset_lock(&genericLock);
}

void Controller::coaddSwarpfilter()
{
    if (!successProcessing) return;

    QString kappa = cdw->ui->COAoutthreshLineEdit->text();
    QString clustersize = cdw->ui->COAoutsizeLineEdit->text();
    QString borderwidth = cdw->ui->COAoutborderLineEdit->text();

    if (kappa.isEmpty()) {
        // Trigger coaddition
        emit swarpStartCoaddition();
        return;
    }

    progress = 0.;
    emit resetProgressBar();

    pushBeginMessage("CoaddSwarpFilter", coaddScienceDir);
    SwarpFilter swarpFilter(coaddDirName, kappa, clustersize, borderwidth, maxCPU, &verbosity);
    swarpFilter.progress = &progress;
    connect(&swarpFilter, &SwarpFilter::messageAvailable, monitor, &Monitor::displayMessage);
    connect(&swarpFilter, &SwarpFilter::progressUpdate, mainGUI, &MainWindow::progressUpdateReceived);
    swarpFilter.runCosmicFilter();

    // TODO: introduce successProcessing to swarpfilter();

    // TODO: swarpfilter blocks the update of the CPU and memory progress bar (normal progress bar increases as intended). Check timers?
    checkSuccessProcessing(coaddScienceData);
    if (successProcessing) {
        emit progressUpdate(100);
    }

    // Trigger coaddition
    emit swarpStartCoaddition();
}

void Controller::coaddCoaddition()
{
    if (!successProcessing) return;

    emit resetProgressBar();

    statusOld = coaddScienceData->processingStatus->statusString;

    pushBeginMessage("CoaddCoadd", coaddScienceDir);

    // List of all images
    QDir coaddDir(coaddDirName);
    QStringList filter("*"+statusOld+"*resamp.fits");
    QStringList imageList = coaddDir.entryList(filter);
    QString listName = tmpdir+"/coaddList";
    QFile file(listName);
    QTextStream stream(&file);
    if (!file.open(QIODevice::WriteOnly)) {
        QString part1 = listName;
        QString part2 = file.errorString();
        emit showMessageBox("Controller::CANNOT_WRITE_RESAMPLE_LIST", part1, part2);
        successProcessing = false;
        return;
    }

    for (auto &imageName : imageList) {
        stream << imageName << "\n";
    }
    file.close();
    file.setPermissions(QFile::ReadUser | QFile::WriteUser);

    sizeFileCounter = new FileProgressCounter(coaddDirName, coaddCoadditionGetSize(), &progress, this);
    connect(sizeFileCounter->timer, &QTimer::timeout, sizeFileCounter, &FileProgressCounter::sizeFileProgress);
    connect(sizeFileCounter, &FileProgressCounter::progressUpdate, mainGUI, &MainWindow::progressUpdateReceived);
    connect(this, &Controller::stopFileProgressTimer, sizeFileCounter, &FileProgressCounter::stopFileProgressTimerReceived);
    sizeFileCounter->timer->start(1000);

    coaddCoadditionBuildSwarpCommand(listName);

    // delete a previous Thread from the preparation.
    //    workerThreadPrepare->quit();
    //    delete workerThreadPrepare;

    // Run the Swarp command
    workerThreadCoadd = new QThread();
    swarpWorker = new SwarpWorker(swarpCommand, coaddDirName, "swarpCoaddition");
    workerInit = true;
    workerThreadInit = true;
    swarpWorker->moveToThread(workerThreadCoadd);
    connect(workerThreadCoadd, &QThread::started, swarpWorker, &SwarpWorker::runSwarp);
    connect(workerThreadCoadd, &QThread::finished, workerThreadCoadd, &QThread::deleteLater);
    //    connect(workerThreadCoadd, &QThread::finished, workerThreadCoadd, &QThread::deleteLater);
    connect(swarpWorker, &SwarpWorker::finishedCoaddition, this, &Controller::coaddUpdate, Qt::DirectConnection);
    connect(swarpWorker, &SwarpWorker::finished, workerThreadCoadd, &QThread::quit);
    connect(swarpWorker, &SwarpWorker::finished, swarpWorker, &QObject::deleteLater);
    //connect(swarpWorker, &SwarpWorker::finished, workerThreadCoadd, &QThread::quit);
    //connect(swarpWorker, &SwarpWorker::finished, swarpWorker, &QObject::deleteLater);
    connect(swarpWorker, &SwarpWorker::messageAvailable, monitor, &Monitor::displayMessage);
    workerThreadCoadd->start();
    //    workerThreadCoadd->wait();

    // TODO: does not continue with next filter if scheduled as "separately"! QTconnection issue?

}

long Controller::coaddCoadditionGetSize()
{
    // Read NAXIS1/2 in coadd.head
    QFile coaddHead(coaddDirName+"/coadd.head");
    if(!coaddHead.open(QIODevice::ReadOnly)) {
        QString part1 = coaddHead.fileName();
        QString part2 = coaddHead.errorString();
        emit showMessageBox("Controller::CANNOT_UPDATE_HEADER_WITH_PA", part1, part2);
        successProcessing = false;
        return 0;
    }
    long naxis1_coadd = 0;
    long naxis2_coadd = 0;
    QTextStream streamIn(&coaddHead);
    while(!streamIn.atEnd()) {
        QString line = streamIn.readLine();
        if (line.contains("NAXIS1  =")) naxis1_coadd = line.split("=")[1].split("/")[0].simplified().toLong();
        if (line.contains("NAXIS2  =")) naxis2_coadd = line.split("=")[1].split("/")[0].simplified().toLong();
    }
    return 4 * naxis1_coadd * naxis2_coadd;
}

void Controller::coaddUpdate()
{
    if (!successProcessing) return;
    pushBeginMessage("CoaddUpdate", coaddScienceDir);

    // Stop "measuring" the file size of coadd.fits
    emit stopFileProgressTimer();

    progress = 0.;
    emit resetProgressBar();

    emit messageAvailable("coadd.fits : Downloading GAIA point sources ...", "image");

    emit loadViewer(coaddDirName, "coadd.fits", "DragMode");

    QString pixelScale = cdw->ui->COApixscaleLineEdit->text();
    if (pixelScale.isEmpty()) pixelScale = QString::number(instData->pixscale, 'f', 9);

    long naxis1 = 0;
    long naxis2 = 0;
    fitsfile *fptr0 = nullptr;
    int status = 0;
    QString filename = coaddDirName+ "/coadd.fits";
    fits_open_file(&fptr0, filename.toUtf8().data(), READONLY, &status);
    float radius = 1.0;
    if (!status) {
        fits_read_key_lng(fptr0, "NAXIS1", &naxis1, NULL, &status);
        fits_read_key_lng(fptr0, "NAXIS2", &naxis2, NULL, &status);
        radius = sqrt(naxis1*naxis1+naxis2*naxis2) / 2. * pixelScale.toFloat() / 60.;
        // Enforce an upper limit to the download catalog (the query also has a built-in limit of # sources < 1e6)
        if (radius > 60) radius = 60.;
    }
    fits_close_file(fptr0, &status);

    downloadGaiaCatalog(coaddScienceData, QString::number(radius, 'f', 1)); // Point sources

    // Measure the seeing if we have reference sources
    float seeing_world = 0.;
    float seeing_image = 0.;

    float maxVal = 100;

    if (gaiaQuery->numSources != 0) {

        // Measure the seeing
        MyImage *coadd = new MyImage(coaddDirName, "coadd.fits", "", 1, QVector<bool>(), false, &verbosity, false);
        connect(coadd, &MyImage::critical, this, &Controller::criticalReceived);
        connect(coadd, &MyImage::messageAvailable, this, &Controller::messageAvailableReceived);
        connect(coadd, &MyImage::warning, this, &Controller::warningReceived);
        connect(coadd, &MyImage::setMemoryLock, this, &Controller::setMemoryLockReceived, Qt::DirectConnection);
        coadd->chipName = "coadd.fits";
        emit messageAvailable("coadd.fits : Loading image data ...", "image");
        coadd->setupData(false, false, true);
        coadd->globalMask = QVector<bool>();
        coadd->globalMaskAvailable = false;
        coadd->objectMaskDone = false;
        coadd->weightInMemory = false;
        coadd->gain = 1.0;
        maxVal = maxVec_T(coadd->dataCurrent);
        emit messageAvailable("coadd.fits : Modeling background ...", "image");
        coadd->backgroundModel(256, "interpolate");
        emit messageAvailable("coadd.fits : Detecting sources ...", "image");
        coadd->segmentImage("10", "3", true, false);
        coadd->estimateMatchingTolerance();

        ImageQuality *imageQuality = new ImageQuality(coaddScienceData, instData, mainDirName);
        imageQuality->matchingTolerance = coadd->matchingTolerance;
        imageQuality->baseName = "coadd.fits";
        connect(imageQuality, &ImageQuality::messageAvailable, this, &Controller::messageAvailableReceived);
        // pass the reference data
        collectGaiaRaDec(coadd, gaiaQuery->de_out, gaiaQuery->ra_out, imageQuality->refCat);
        // pass the source data (dec, ra, fwhm, ell on one hand, and mag separately)
        coadd->collectSeeingParameters(imageQuality->sourceCat, imageQuality->sourceMag, 0);
        // match
        bool gaia = imageQuality->getSeeingFromGaia();
//        if (!gaia) imageQuality->getSeeingFromRhMag();      TODO: not yet implemented
        seeing_world = imageQuality->fwhm;
        seeing_image = seeing_world / pixelScale.toFloat();
        emit messageAvailable("coadd.fits : FWHM = " + QString::number(seeing_world, 'f', 2) + "\"  ("
                              + QString::number(seeing_image, 'f', 2) + " pixel)", "image");
        emit messageAvailable("coadd.fits : Ellipticity = " + QString::number(imageQuality->ellipticity, 'f', 3), "image");

        coadd->releaseAllDetectionMemory();
        coadd->releaseBackgroundMemory("entirely");
        coadd->freeAll();
        delete coadd;
    }
    else {
        emit messageAvailable("Seeing will not be determined because no reference point sources were retrieved.", "warning");
    }

    delete gaiaQuery;

    /*
    // TODO: that should be redone using the new gaia matching method
    QVector<float> fluxRadius(coadd->objectList.length());
    for (auto &obj : coadd->objectList) {
        if (obj->FLAGS == 0) fluxRadius.append(obj->FLUX_RADIUS);
    }
    float seeing_image = modeMask(fluxRadius, "classic", QVector<bool>(), false)[0];
    float seeing_world = seeing_image * instData->pixscale;
    emit messageAvailable("coadd.fits : FWHM = " + QString::number(seeing_world, 'f', 2) + "\"  ("
                          + QString::number(seeing_image, 'f', 2) + " pixel)"
    */

    // Update header keywords
    fitsfile *fptr = nullptr;
    QString name = coaddDirName+"/coadd.fits";
    //    int status = 0;    // declared above
    if (name.isNull() || name.isEmpty()) {
        status = 1;
        emit messageAvailable("Controller::coaddUpdate(): file name empty or not initialized!", "error");
        emit criticalReceived();
        return;
    }
    fits_open_file(&fptr, name.toUtf8().data(), READWRITE, &status);
    fits_update_key_str(fptr, "FILTER", coaddFilter.toUtf8().data(), NULL, &status);
    fits_update_key_flt(fptr, "EXPTIME", 1.0, 1, "Exposure time is normalized to 1s", &status);
    fits_update_key_flt(fptr, "TEXPTIME", coaddTexptime, 3, "Total exposure time [s]", &status);
    fits_update_key_flt(fptr, "SKYVALUE", coaddSkyvalue, 3, "Effective background value [e-/s]", &status);
    fits_update_key_flt(fptr, "GAIN", coaddGain, 3, "Effective gain", &status);
    fits_update_key_str(fptr, "UNITS", "e-/s", "Pixels are photo-electrons / second", &status);
    fits_update_key_flt(fptr, "SATURATE", 1000., 3, "Currently undetermined (e-)", &status);
    fits_update_key_flt(fptr, "FWHM_I", seeing_image, 3, "FWHM (pixel)", &status);
    fits_update_key_flt(fptr, "FWHM_W", seeing_world, 3, "FWHM (arcsec)", &status);
    fits_update_key_dbl(fptr, "MJDSTART", mjdStart, 12, "Begin of observation (MJD)", &status);
    fits_update_key_dbl(fptr, "MJDEND", mjdEnd, 12, "End of observation (MJD)", &status);
    fits_update_key_dbl(fptr, "MJDMED", mjdMedian, 12, "Median MJD of observations", &status);
    fits_update_key_str(fptr, "THELIRUN", mainGUI->ui->setupProjectLineEdit->text().toUtf8().data(), "THELI project name", &status);
    fits_close_file(fptr, &status);

    printCfitsioError("coaddUpdate():", status);

    emit progressUpdate(100);

    //    pushEndMessage(taskBasename, coaddScienceDir);

    // Now we can quit the first coaddition thread (which spawned all the other threads).
    // This will return control to mainGUI() (i.e. enable the start button again)
    //   workerThreadPrepare->quit();

    // Finally, do flux calibration if requested
    if (cdw->ui->COAfluxcalibCheckBox->isChecked()) {
        emit messageAvailable("coadd.fits : Loading flux calibration module ...", "image");
        emit loadAbsZP(coaddDirName+"/coadd.fits", instData, maxVal);
    }
    else {
        // Now we can quit the first coaddition thread (which spawned all the other threads).
        // This will return control to mainGUI() (i.e. enable the start button again)
        workerThreadPrepare->quit();
    }
}

void Controller::absZeroPointCloseReceived()
{
    workerThreadPrepare->quit();
}

// write the initial empty coadd.fits into a coadd.head
void Controller::foldCoaddFits()
{
    QFile fileIn(coaddDirName+"/coadd.fits");
    QFile fileOut(coaddDirName+"/coadd.head");
    QTextStream streamIn(&fileIn);
    QTextStream streamOut(&fileOut);
    if (!fileIn.open(QIODevice::ReadOnly)) {
        QString part1 = coaddDirName+"/coadd.fits";
        QString part2 = fileIn.errorString();
        emit showMessageBox("Controller::CANNOT_OPEN_FILE", part1, part2);
        successProcessing = false;
        return;
    }
    if (!fileOut.open(QIODevice::WriteOnly)) {
        QString part1 = coaddDirName+"/coadd.head";
        QString part2 = fileIn.errorString();
        emit showMessageBox("Controller::CANNOT_OPEN_FILE", part1, part2);
        successProcessing = false;
        return;
    }

    QString line;
    int cardLength = 80;
    while (streamIn.readLineInto(&line)) {
        if (line.isEmpty()) continue;
        long length = line.length();
        if (length<80) return;
        for (long i=0; i<=length-cardLength; i+=cardLength) {
            QStringRef subString(&line, i, cardLength);
            QString string = subString.toString();
            streamOut << string << "\n";
        }
    }

    fileIn.close();
    fileOut.close();
    fileOut.setPermissions(QFile::ReadUser | QFile::WriteUser);

    fileIn.remove();
}
