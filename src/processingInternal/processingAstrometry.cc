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
    scampDir = scampScienceDir+"/astrom_photom_scamp/";
    scampPlotsDir = mainDirName+"/"+scienceDir + "/plots/";
    scampHeadersDir = mainDirName+"/"+scienceDir + "/headers/";
    scampScienceData = scienceData;
    anetDir = scampScienceDir+"/astrom_photom_anet/";

    pushBeginMessage(taskBasename, scienceDir);
    pushConfigAstromphotom();

    // DAISY-CHAINING. The end of the scamp run triggers the iView display of the checkplots,
    // which in turn triggers the header update

    if (cdw->ui->ASTmethodComboBox->currentText() == "Scamp") {
        QString catDirName = mainDirName + "/" + scienceDir + "/cat/";
        if (!scienceData->hasMatchingPartnerFiles(catDirName, ".scamp", false)) {
            emit messageAvailable("Not all exposures have complete .scamp catalogs. They will be skipped.", "warning");
            // return;
        }

        if (cdw->ui->ASTmatchMethodComboBox->currentText() == "Astrometry.net") {
            if (!scienceData->hasMatchingPartnerFiles(catDirName, ".anet", false)) {
                emit messageAvailable("Not all exposures have complete .anet catalogs. They will be skipped.", "warning");
                // return;
            }
            prepareAnetRun(scienceData);
            progress = 0.;
            runAnet(scienceData);
        }

        // run scamp and retrieval of point source catalog in parallel
#pragma omp parallel sections
        {
#pragma omp section
            {
                // Prepare scamp directories and perform consistency checks
                prepareScampRun(scienceData);
                // Collect scamp input catalogs
                long totNumObjects = 0;
                long numCats = prepareScampCats(scienceData, totNumObjects);
                //              if (numCats == 0) return;      // no exiting in openMP construct
                if (numCats > 0) {

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
                    connect(scampWorker, &ScampWorker::errorFound, this, &Controller::scampErrorFoundReceived);
                    connect(scampWorker, &ScampWorker::finishedScamp, this, &Controller::finishedScampReceived);
                    // Need the direct connection if we want the thread to actually return control to the main thread (activating the start button again).
                    // But then sky sub would start before checkplots are evaluated.
                    // CORRECT WAY: call workerThread->quit() at the end of the image quality analysis
                    //       connect(scampWorker, &ScampWorker::finished, workerThread, &QThread::quit, Qt::DirectConnection);
                    //       connect(scampWorker, &ScampWorker::finished, workerThread, &QThread::quit);
                    connect(scampWorker, &ScampWorker::finished, scampWorker, &QObject::deleteLater);
                    connect(scampWorker, &ScampWorker::messageAvailable, monitor, &Monitor::displayMessage);
                    connect(scampWorker, &ScampWorker::fieldMatched, this, &Controller::fieldMatchedReceived);
                    workerThread->start();
                    workerThread->wait();
                }
            }
#pragma omp section
            {
                downloadGaiaCatalog(scampScienceData); // Point sources
            }
        }
    }

    // Not implemented (greyed out)
    if (cdw->ui->ASTmethodComboBox->currentText() == "Astrometry.net") {
        QString catDirName = mainDirName + "/" + scienceDir + "/cat/";
        if (!scienceData->hasMatchingPartnerFiles(catDirName, ".anet")) return;

        // Prepare anet directories and perform consistency checks
        prepareAnetRun(scienceData);

        /*
        // Release memory (assuming it is the same for a.net)
        // 140 bytes per detection
        // "a few 10 kB" per FITS table
        //
        long totBytes = 0;
        int degrees = cdw->ui->ASTdistortLineEdit->text().toInt();
        totBytes += totNumObjects * 140 + (2*50000*instData->numChips)*numCats;
        long Nt = numCats * 1 * (degrees*degrees+2) + numCats*6;
        totBytes += 8*Nt*Nt;
        totBytes = 2*totBytes/ 1024. / 1024.;
        if (verbosity >= 1) emit messageAvailable("Astrometry.net memory estimate: " + QString::number(long(totBytes)) + " MB", "controller");
        releaseMemory(totBytes, 1);
        */

        progress = 0.;

        runAnet(scienceData);
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

void Controller::scampErrorFoundReceived()
{
    successProcessing = false;
    criticalReceived();
    if (workerThread) workerThread->quit();
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
        if (workerThread) workerThread->quit();
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
        if (instData->validChip == -1) {
            successProcessing = false;
            return;
        }
        for (auto &it : scampScienceData->myImageList[instData->validChip]) {
            it->loadHeader();
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
        int headCount = 0;
        while (inStream.readLineInto(&line)) {
            if (i==0) {
                int chip = instData->chipMap.key(headCount) + 1;      // dealing with user defined bad chips
                HEAD.setFileName(scampHeadersDir+"/"+MEFinfo.completeBaseName()+"_"+QString::number(chip)+".head");
                outStream.setDevice(&HEAD);
                if (!HEAD.open(QIODevice::WriteOnly)) {
                    success = false;
                    break;
                }
            }
            // THELI style flux scaling
            if (line.contains("FLXSCALE=")) line.replace("FLXSCALE=","FLSCALE =");        // copying the original FLXSCALE keyword to FLSCALE (original will be replaced below)
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
                ++headCount;
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
        if (workerThread) workerThread->quit();
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
        if (workerThread) workerThread->quit();
        successProcessing = true;
        return;
    }

#pragma omp parallel for num_threads(maxExternalThreads)
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (abortProcess || !successProcessing || instData->badChips.contains(chip)) continue;
        for (auto &it : scampScienceData->myImageList[chip]) {
            if (abortProcess) break;
            it->setupDataInMemorySimple(false);
            if (it->activeState != MyImage::ACTIVE) continue;

            if (!it->successProcessing) {
                abortProcess = true;
                continue;
            }
            //            it->backupOrigHeader(chip);            // Create a backup copy of the original FITS headers if it doesn't exist yet
            if (it->scanAstromHeader(chip, "inHeadersDir")) {    // reads the header, and updates the wcs struct in MyImage class (i.e. the memory)
                it->updateZeroOrderOnDrive("update");            // Overwrite 0-th order solution in FITS header (if on drive)
                //               it->updateZeroOrderInMemory();                   // Overwrite 0-th order solution in memory
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

void Controller::prepareAnetRun(Data *scienceData)
{
    if (!successProcessing) return;

    if (verbosity > 0) emit messageAvailable("Setting up astrometry.net run ...", "controller");

    QString scienceDir = mainDirName+"/"+scienceData->subDirName;
    QString headersPath = scienceDir+"/headers/";

    // Clean-up and recreate the anet directory trees
    QDir dir(anetDir);
    dir.removeRecursively();
    dir.mkpath(anetDir);

    QDir headersDir(headersPath);
    headersDir.removeRecursively();
    headersDir.mkpath(headersPath);

    // Check if the reference catalog exists
    QFile refcat(scienceDir+"/cat/refcat/theli_mystd.index");
    if (!refcat.exists()) {
        emit messageAvailable("The astrometric reference catalog does not exist, or was not created!", "error");
        successProcessing = false;
        monitor->raise();
        return;
    }

    // Write the config file for the solver
    QFile backendConfig(scienceDir+"/astrom_photom_anet/backend.cfg");
    if (backendConfig.exists()) backendConfig.remove();
    QTextStream stream(&backendConfig);
    if( !backendConfig.open(QIODevice::WriteOnly)) {
        emit messageAvailable("Controller::prepareAnetRun(): ERROR writing "+scienceDir+"/astrom_photom_anet/backend.cfg : "+backendConfig.errorString(), "error");
        emit criticalReceived();
        return;
    }
    stream << "#inparallel" << "\n";
    stream << "#depths 10 20 30 40 50 60 70 80 90 100" << "\n";
    stream << "#cpulimit 300" << "\n";
    stream << "add_path " << scienceDir+"/cat/refcat/" << "\n";
    stream << "index theli_mystd.index" << "\n";
    backendConfig.close();
    backendConfig.setPermissions(QFile::ReadUser | QFile::WriteUser);
}

void Controller::runAnet(Data *scienceData)
{
    QList<MyImage*> allMyImages;
    long numMyImages = makeListofAllImages(allMyImages, scienceData);

    float nimg = 1;  // wild guess, don't know the memory needs of a.net
    releaseMemory(nimg*instData->storage*maxCPU, 1);
    scienceData->protectMemory();

    QString pixscaleMaxerr = cdw->ui->ASTpixscaleLineEdit->text();

    scienceData->populateExposureList();
    progressStepSize = 100. / float(numMyImages);

    QStringList failedImages = QStringList();
    QStringList failedExposures = QStringList();

#pragma omp parallel for num_threads(maxCPU)
    for (int k=0; k<numMyImages; ++k) {
        if (abortProcess || !successProcessing) continue;

        auto &it = allMyImages[k];
        int chip = it->chipNumber - 1;
        if (!it->successProcessing) continue;
        if (instData->badChips.contains(chip)) continue;

        releaseMemory(nimg*instData->storage, maxCPU);

        if (verbosity >= 1) emit messageAvailable(it->chipName + " : Running astrometry.net ...", "data");
        it->loadHeader();         // don't need pixels, but metadata
        it->checkWCSsanity();
        it->buildAnetCommand(pixscaleMaxerr);
        it->runAnetCommand();
        it->unprotectMemory();
        if (minimizeMemoryUsage) {
            it->freeAll();
        }
#pragma omp critical
        {
           if (!it->anetSolved) {
               failedImages.append(it->baseName);
               failedExposures.append(it->rootName);
           }
           progress += progressStepSize;
        }
    }

    emit messageAvailable("<br>Astrometry.net summary : ", "data");

    if (!failedImages.isEmpty()) {
        emit messageAvailable("The following images did not solve:", "warning");
        for (auto &it : failedImages) {
            emit messageAvailable(it, "warning");
        }
        emit warningReceived();
    }


    // merge the anet output of multi-chip cameras, and extract relevant keywords, only
    for (long i=0; i<scienceData->exposureList.length(); ++i) {
        if (abortProcess || !successProcessing) continue;

        // open stream for merged output file
        QString aheaderName = anetDir+"/"+scienceData->exposureList[i][0]->rootName+".ahead";
        QFile aheaderFile(aheaderName);
        QTextStream stream(&aheaderFile);
        if( !aheaderFile.open(QIODevice::WriteOnly)) {
            emit messageAvailable("Could not write astrometry.net reformatted output to "+aheaderFile.fileName(), "error");
            emit messageAvailable(aheaderFile.errorString(), "error");
            emit criticalReceived();
            successProcessing = false;
            break;
        }

        // Loop over chips and extract anet header data
        bool consistent = false;
        int count = 0;
        for (auto &it : scienceData->exposureList[i]) {
            stream << it->extractAnetOutput();
            ++count;
        }
        if (count == instData->numUsedChips) consistent = true;
        aheaderFile.close();
        aheaderFile.setPermissions(QFile::ReadUser | QFile::WriteUser);
        if (!consistent) {
            aheaderFile.remove();
            emit messageAvailable(scienceData->exposureList[i][0]->rootName + " : Controller::runAnet(): Inconsistent number of detectors", "warning");
        }
    }

    // Collect a list of the header files for scamp
    makeAnetHeaderList(scienceData);
}

void Controller::doImageQualityAnalysis()
{
    if (!successProcessing) return;

    // Only do this for cameras which have a meaningful plate scale for this purpose, i.e. where there is
    // a risk that background galaxies contaminate a global statistical seeing and ellipticity measurement done during 'create source cat'.
    // Same for huge cameras such as DECam, but here simply because the catalog download would take forever.
    if (instData->pixscale > 2.0 || instData->radius > 1.0) {
        if (workerThread) workerThread->quit();
        successProcessing = true;
        return;
    }

    pushBeginMessage("ImageQuality", scampScienceDir);

    // Todo: replace with GAIA catalog if that was the reference catalog already!
    //    downloadGaiaCatalog(scampScienceData); // Point sources

    QList<MyImage*> allMyImages;
    long numMyImages = makeListofAllImages(allMyImages, scampScienceData);

#pragma omp parallel for num_threads(maxCPU) firstprivate(mainDirName, gaiaQuery, instData)
    for (int k=0; k<numMyImages; ++k) {
        if (abortProcess || !successProcessing) continue;
        auto &it = allMyImages[k];
        int chip = it->chipNumber - 1;
        if (!it->successProcessing) continue;
        if (it->activeState != MyImage::ACTIVE) continue;
        if (instData->badChips.contains(chip)) continue;

        it->setupDataInMemorySimple(false);
        if (!it->successProcessing) {
            abortProcess = true;
            continue;
        }
        // Setup seeing measurement
        it->estimateMatchingTolerance();
        ImageQuality *imageQuality = new ImageQuality(instData, mainDirName);
        imageQuality->matchingTolerance = it->matchingTolerance;
        imageQuality->baseName = it->chipName;
        // pass the reference data
        collectGaiaRaDec(it, gaiaQuery->de_out, gaiaQuery->ra_out, imageQuality->refCat);
        // pass the source data (dec, ra, fwhm, ell on one hand, and mag separately)
        it->collectSeeingParameters(imageQuality->sourceCat, imageQuality->sourceMag, instData->chipMap.value(chip));
        // match
        static_cast<void> (imageQuality->getSeeingFromGaia());
        it->fwhm = imageQuality->fwhm * instData->pixscale;                                // Updating MyImage fwhm parameter, in arcsec
        it->updateHeaderValue("FWHM", it->fwhm);                                           // Updating MyImage header string
        it->updateHeaderValue("ELLIP", imageQuality->ellipticity);
        it->updateHeaderValueInFITS("FWHM", QString::number(it->fwhm, 'f', 3));            // Updating the current FITS image on drive
        it->updateHeaderValueInFITS("ELLIP", QString::number(imageQuality->ellipticity, 'f', 3));
        //        if (!gaia) imageQuality->getSeeingFromRhMag();      TODO: Not yet implemented
        if (verbosity > 1) emit messageAvailable(it->chipName + " : FWHM / Ellipticity / # stars = "
                                                 + QString::number(it->fwhm, 'f', 3) + " / "
                                                 + QString::number(imageQuality->ellipticity, 'f', 3) + " / "
                                                 + QString::number(imageQuality->numSources), "ignore");
        delete imageQuality;
        imageQuality = nullptr;
        it->unprotectMemory();
        if (minimizeMemoryUsage) {
            it->freeAll();
        }
    }

    if (workerThread) workerThread->quit();

    delete gaiaQuery;
    gaiaQuery = nullptr;

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
            if (it->activeState != MyImage::ACTIVE) continue;
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
    QString headersPath = scienceDir+"/headers/";
    QString plotsPath = scienceDir+"/plots/";

    // Clean-up and recreate the scamp directory trees
    QDir dir(scampDir);
    dir.removeRecursively();
    dir.mkpath(scampDir);

    QDir plotsDir(plotsPath);
    plotsDir.removeRecursively();
    plotsDir.mkpath(plotsPath);

    QDir headersDir(headersPath);
    headersDir.removeRecursively();
    headersDir.mkpath(headersPath);

    QFile file(scampDir+"/scamp_global.ahead");
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

    if (verbosity > 0) emit messageAvailable("Linking scamp catalogs to "+scampDir+" ...", "controller");

    // Prepare a file that contains all scamp catalogs (only those for which an image is currently present in scienceDir);
    QFile catFile(scampDir+"/scamp_cats");
    catFile.remove();
    QTextStream stream(&catFile);
    if( !catFile.open(QIODevice::WriteOnly)) {
        emit messageAvailable("Writing list of scamp catalogs to "+scampDir+catFile.fileName(), "error");
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

    QDir catDir(scienceDir+"/cat/");
    QStringList catList = catDir.entryList(QStringList("*.scamp"));
    for (auto &cat : catList) {
        QString catbase = cat;
        totNumObjects += getNumObjectsScampCat(scienceDir+"/cat/"+catbase);
        catbase.remove(".scamp");
        for (auto &image : imageList) {
            if (image.contains(catbase)) {
                QFile catFile(scienceDir+"/cat/"+cat);
                catFile.link(scampDir+"/"+cat);
                stream << scampDir+"/"+cat+"\n";
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

long Controller::makeAnetHeaderList(Data *scienceData)
{
    if (!successProcessing) return 0;

    // Prepare a list of all anet .ahead files (only those for which an image is currently present);
    QFile aheadFile(anetDir+"/anet_headers");
    aheadFile.remove();
    QTextStream stream(&aheadFile);
    if( !aheadFile.open(QIODevice::WriteOnly)) {
        emit messageAvailable("Writing list of anet headers to "+anetDir+aheadFile.fileName(), "error");
        emit messageAvailable(aheadFile.errorString(), "error");
        successProcessing = false;
        monitor->raise();
        return 0;
    }

    // Link anet headers (only those for currently active images)
    long numCat = 0;
    QStringList imageList;
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->badChips.contains(chip)) continue;
        for (auto &it : scienceData->myImageList[chip]) {
            if (it->activeState == MyImage::ACTIVE) imageList << it->chipName;
        }
    }

    QDir aheadDir(anetDir);
    QStringList aheadList = aheadDir.entryList(QStringList("*.ahead"));
    for (auto &ahead : aheadList) {
        QString aheadbase = ahead;
        int numChips = getNumAnetChips(anetDir+"/"+aheadbase);
        if (numChips != instData->numUsedChips) continue;          // skip bad entry. Error triggered by getNumAnetChips()
        aheadbase.remove(".ahead");
        for (auto &image : imageList) {
            if (image.contains(aheadbase)) {
                stream << anetDir+"/"+ahead+"\n";
                ++numCat;
                break;
            }
        }
    }
    aheadFile.close();
    aheadFile.setPermissions(QFile::ReadUser | QFile::WriteUser);

    if (numCat == 0) {
        QString scienceDir = mainDirName+"/"+scienceData->subDirName;
        emit messageAvailable("No *.ahead catalogs were found matching the exposures in "+scienceDir, "error");
        monitor->raise();
        successProcessing = false;
        return 0;
    }

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

    QString out = __func__;
    out.append( " :<br> " +cat);
    printCfitsioError(out, status);

    return nobj;
}

long Controller::getNumAnetChips(QString ahead)
{
    // Count the number of "END" strings
    QFile file(ahead);
    QTextStream inStream(&file);
    if (!file.open(QIODevice::ReadOnly)) {
        emit messageAvailable("Controller::getNumAnetChips(): Could not open "+ahead+" " + file.errorString(), "error");
        emit criticalReceived();
        monitor->raise();
        successProcessing = false;
        return 0;
    }

    QString line;
    int numChips = 0;
    while (inStream.readLineInto(&line)) {
        if (line == "END") ++numChips;
    }
    file.close();

    if (numChips == instData->numUsedChips) return numChips;
    else {
        emit messageAvailable(ahead + ":<br>Expected "+QString::number(instData->numUsedChips)+" chips, but found "+QString::number(numChips), "error");
        emit criticalReceived();
        monitor->raise();
        successProcessing = false;
        return 0;
    }
}

long Controller::getNumObjectsSourceExtractorCat(QString cat)
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

    printCfitsioError(__func__, status);

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
    scampCommand += " @"+scampDir+"/scamp_cats";
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

    if (cdw->ui->ASTmatchMethodComboBox->currentText() == "Astrometry.net") {
        scampCommand += " -AHEADER_NAME @"+anetDir+"/anet_headers";
    }

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
