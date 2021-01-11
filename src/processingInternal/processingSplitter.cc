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
#include "../tools/splitter.h"
#include "ui_confdockwidget.h"

#include "fitsio.h"
#include <QMetaObject>
#include <QVector>
#include <QStringList>
#include <QProgressBar>


void Controller::taskInternalHDUreformat()
{
    if (!successProcessing) return;

    QString dataDir = instructions.split(" ").at(1);
    currentDirName = dataDir;

    Data *data = getDataAll(dataDir);
    if (data == nullptr) return;      // Error triggered by getDataAll();

    data->numImages = 0;

    pushBeginMessage(taskBasename, dataDir);
    pushConfigHDUreformat();

    // Obtain a list of files in this directory, including non-FITS files
    QString path = mainDirName + "/" + dataDir;
    QDir dir(path);
    QStringList filter;
    filter << "*.*";
    QStringList files = dir.entryList(filter, QDir::Files);
    numActiveImages = files.length();
    progressStepSize = 100./(float(numActiveImages) * instData->numChips);
    progress = 0.;

    // Saturation value
    float userSaturationValue = 0.;
    if (!cdw->ui->saturationLineEdit->text().isEmpty()) {
        userSaturationValue = cdw->ui->saturationLineEdit->text().toFloat();
    }

    // Retrieve nonlinearity information (checks internally if available, otherwise returns empty list)
    QList<QVector<float>> nonlinearityCoefficients;
    if (cdw->ui->nonlinearityCheckBox->isChecked()) {
        nonlinearityCoefficients = getNonlinearityCoefficients();
    }

    // DUMMY keywords, and others needed later-on
    QStringList dummyKeys;
    QString newCard;
    newCard = "FWHM    = -1.0";
    newCard.resize(80, ' ');
    dummyKeys.append(newCard);
    newCard = "ELLIP   = -1.0";
    newCard.resize(80, ' ');
    dummyKeys.append(newCard);
    newCard = "RZP     = -1.0";
    newCard.resize(80, ' ');
    dummyKeys.append(newCard);
    for (int i=0; i<20; ++i) {
        QString newCard = "DUMMY"+QString::number(i+1);
        newCard.resize(8, ' ');
        newCard.append("=                  0.0 / Placeholder card");
        newCard.resize(80, ' ');
        dummyKeys.append(newCard);
    }

    QString dataType = data->dataType;

    // Stop memory bar. Updated internally because data classes are not yet available
    // Must use signals instead, timer lives in a different thread
    // memTimer->stop();

    float dateObsIncrementor = 0;

    bool commaDetected = false;

    // Loop over all chips
#pragma omp parallel for num_threads(maxCPU) firstprivate(mainDirName, dataDir, dummyKeys, nonlinearityCoefficients, headerDictionary, filterDictionary, dataType)
    for (int i=0; i<numActiveImages; ++i) {
        if (!successProcessing) continue;
        if (userStop || userKill || abortProcess) continue;  // Only place we do it this way, because Data class is not yet instantiated
        // Setup the splitter class for the current file
        QString fileName = files.at(i);
        Splitter *splitter = new Splitter(*instData, mask, altMask, data, dataType, cdw, mainDirName, dataDir, fileName, &verbosity);
        splitter->headerDictionary = headerDictionary;
        splitter->filterDictionary = filterDictionary;
        splitter->instrumentDictionary = instrumentDictionary;
        splitter->dummyKeys = dummyKeys;
        splitter->combineOverscan_ptr = combineOverscan_ptr;       // set by MainWindow::updateControllerFunctors()
        splitter->nonlinearityCoefficients = nonlinearityCoefficients;
        splitter->progressStepSize = progressStepSize;
        splitter->progressLock = &progressLock;
        splitter->genericLock = &genericLock;
        splitter->progress = &progress;
        splitter->dateObsIncrementor = &dateObsIncrementor;
        splitter->userSaturationValue = userSaturationValue;
        splitter->compileNumericKeys();
        connect(splitter, &Splitter::messageAvailable, this, &Controller::messageAvailableReceived);
        connect(splitter, &Splitter::critical, this, &Controller::criticalReceived);
        connect(splitter, &Splitter::warning, this, &Controller::warningReceived);
        connect(splitter, &Splitter::showMessageBox, this, &Controller::showMessageBoxReceived);
        // Extract images. This also handles all low-level pixel processing.
        splitter->determineFileFormat();
        splitter->extractImages();
        if (splitter->commaDetected) commaDetected = true;
        if (!splitter->successProcessing) successProcessing = false;
        delete splitter;     // Hogging lots of memory otherwise!
        splitter = nullptr;

        // splitter handles the progress counter
        // No memory management needed, splitter simply runs out of scope
    }

    checkSuccessProcessing(data);

    if (verbosity > 1 && commaDetected) {
        emit messageAvailable("Decimal commas found in raw data, converted to decimal dots in output.", "warning");
        emit warningReceived();
    }

    if (successProcessing) {
        uniformMJDOBS(dir);              // rename file and update MJDOBS for a few specific instruments, only
        if (!updateDataDirs(data)) {     // Some instruments need to split the original data dir into several more, e.g. GROND and LIRIS_POL
            finalizeSplitter(data);
        }
        emit progressUpdate(100);
    }

    // Restart memory bar
 //   memTimer->start(1000);
}

void Controller::finalizeSplitter(Data *data)
{
    data->processingStatus->HDUreformat = true;
    data->processingStatus->writeToDrive();
    if (!data->dataInitialized) {
        data->dataInitialized = true;
        data->populate("P");
    }
    for (int chip=0; chip<instData->numChips; ++chip) {
        for (auto &it : data->myImageList[chip]) {
            it->processingStatus->HDUreformat = true;
        }
    }
    data->emitStatusChanged();
    emit populateMemoryView();
}

// Multi-chip cameras must have uniform MJD-OBS, otherwise "exposures" cannot be identified unambiguously
// The following instruments do not have this
void Controller::uniformMJDOBS(QDir &dir)
{
    if (instData->name == "VIMOS@VLT"                  // VIMOS has no unique DATE-OBS
            || instData->name.contains("MOIRCS")) {    // MOIRCS has no unique DATE-OBS
        QString path = dir.absolutePath();
        QStringList filter = {instData->shortName+"*.fits"};
        dir.setNameFilters(filter);
        dir.setSorting(QDir::Name);
        QStringList fileNames = dir.entryList();
        char filterChip1[80];
        char dateObsChip1[80];
        double mjdObsChip1 = 0.;
        for (auto &fileName : fileNames) {
            QFileInfo fi(path+"/"+fileName);
            QString baseName = fi.completeBaseName();
            int pos = baseName.lastIndexOf('_');
            QString chipNumber = baseName.remove(0,pos).remove('_').remove("P");
            fitsfile *fptr;
            int status = 0;
            QString completeName = path+"/"+fileName;
            fits_open_file(&fptr, completeName.toUtf8().data(), READWRITE, &status);
            // Extract DATE-OBS and MJD-OBS from chip 1
            if (chipNumber == "1") {
                fits_read_key_str(fptr, "DATE-OBS", dateObsChip1, nullptr, &status);
                fits_read_key_str(fptr, "FILTER", filterChip1, nullptr, &status);
                fits_read_key_dbl(fptr, "MJD-OBS", &mjdObsChip1, nullptr, &status);
            }
            // Project DATE-OBS and MJD-OBS from chip 1 onto other chips
            else {
                fits_update_key_str(fptr, "DATE-OBS", dateObsChip1, nullptr, &status);
                fits_update_key_dbl(fptr, "MJD-OBS", mjdObsChip1, -13, nullptr, &status);   // the '-' enforces floating point notation over exponential notation
            }
            if (chipNumber == "1") {
                // make double sure that in chip one we have exactly the same format and number of digits as in the other chips!
                fits_update_key_dbl(fptr, "MJD-OBS", mjdObsChip1, -13, nullptr, &status);
            }
            fits_close_file(fptr, &status);
            printCfitsioError("uniformMJDOBS()", status);

            // Rename image file to THELI standard
            QString dateObsString(dateObsChip1);
            QString filterString(filterChip1);
            QFile file(path+"/"+fileName);
            file.rename(path+"/"+instData->shortName+"."+filterString+"."+dateObsString+"_"+chipNumber+"P.fits");
        }
    }
}

bool Controller::updateDataDirs(Data *data)
{
    bool updateDataDirDone = false;
    QLineEdit *le = nullptr;
    QString newDirs = "";
    QString d = data->subDirName;

    // Erase and update the data tree LineEdits
    if (instData->name == "LIRIS_POL@WHT") {
        newDirs = d+"_PA000 ";
        newDirs.append(d+"_PA045 ");
        newDirs.append(d+"_PA090 ");
        newDirs.append(d+"_PA135");
        le = getDataTreeLineEdit(data);
    }

    if (instData->name == "GROND_OPT@MPGESO") {
        newDirs = d+"_g ";
        newDirs.append(d+"_r ");
        newDirs.append(d+"_i ");
        newDirs.append(d+"_z ");
        le = getDataTreeLineEdit(data);
    }

    if (instData->name == "GROND_NIR@MPGESO") {
        newDirs = d+"_J ";
        newDirs.append(d+"_H ");
        newDirs.append(d+"_K");
        le = getDataTreeLineEdit(data);
    }

    if (le != nullptr) {
        le->clear();
        le->setText(newDirs);
        if (!mainGUI->checkPathsLineEdit(le)) {
            le->clear();
            return false;  // return if some data do not exist (e.g. optical GROND might not have darks, but NIR does)
        }
        dataTreeUpdateOngoing = true;
        omp_set_lock(&memoryLock);
        //        emit clearMemoryView();     // Manipulate sobject in different thread despite just emitting a signal. Strange
        mainDirName = mainGUI->ui->setupMainLineEdit->text();
        recurseCounter = 0;
        QList<Data*> *DT_x; // = new QList<Data*>();
        if (data->dataType == "BIAS") DT_x = &DT_BIAS;
        else if (data->dataType == "DARK") DT_x = &DT_DARK;
        else if (data->dataType == "FLATOFF") DT_x = &DT_FLATOFF;
        else if (data->dataType == "FLAT") DT_x = &DT_FLAT;
        else if (data->dataType == "SCIENCE") DT_x = &DT_SCIENCE;
        else if (data->dataType == "SKY") DT_x = &DT_SKY;
        else {
            // data->dataType == "STD")
            DT_x = &DT_STANDARD;
        }
        updateMasterList();
        emit populateMemoryView();
        omp_unset_lock(&memoryLock);
        dataTreeUpdateOngoing = false;
        for (auto &data : *DT_x) {
            finalizeSplitter(data);
        }

        updateDataDirDone = true;
//        delete DT_x;
    }

    return updateDataDirDone;
}

QLineEdit* Controller::getDataTreeLineEdit(Data *data)
{
    QLineEdit *le = nullptr;
    if (data->dataType == "BIAS") le = mainGUI->ui->setupBiasLineEdit;
    else if (data->dataType == "DARK") le = mainGUI->ui->setupDarkLineEdit;
    else if (data->dataType == "FLATOFF") le = mainGUI->ui->setupFlatoffLineEdit;
    else if (data->dataType == "FLAT") le = mainGUI->ui->setupFlatLineEdit;
    else if (data->dataType == "SCIENCE") le = mainGUI->ui->setupScienceLineEdit;
    else if (data->dataType == "SKY") le = mainGUI->ui->setupSkyLineEdit;
    else if (data->dataType == "STD") le = mainGUI->ui->setupStandardLineEdit;
    else {
        // nothing to be done
    }

    return le;
}

// copied from MainWindow
void Controller::resetAltInstrumentData()
{
    altInstData.numChips = 1;
    altInstData.numUsedChips = 1;
    altInstData.name = "";
    altInstData.shortName = "";
    altInstData.nameFullPath = "";
    altInstData.obslat = 0.;
    altInstData.obslong = 0.;
    altInstData.bayer = "";
    altInstData.type = "OPT";
    altInstData.pixscale = 1.0; // in arcsec
//    altInstData.gain = 1.0;
    altInstData.radius = 0.1;   // exposure coverage radius in degrees
    altInstData.storage = 0;    // MB used for a single image
    altInstData.storageExposure = 0.; // MB used for the entire (multi-chip) exposure

    altInstData.overscan_xmin.clear();
    altInstData.overscan_xmax.clear();
    altInstData.overscan_ymin.clear();
    altInstData.overscan_ymax.clear();
    altInstData.cutx.clear();
    altInstData.cuty.clear();
    altInstData.sizex.clear();
    altInstData.sizey.clear();
    altInstData.crpix1.clear();
    altInstData.crpix2.clear();
}

// copied and modified (shortened) from MainWindow, to handle the two GROND detector types
void Controller::initAltInstrumentData(QString instrumentNameFullPath)
{
    resetAltInstrumentData();

    QFile altInstDataFile(instrumentNameFullPath);
    altInstDataFile.setFileName(instrumentNameFullPath);
    altInstData.nameFullPath = instrumentNameFullPath;

    // read the instrument specific data
    if( !altInstDataFile.open(QIODevice::ReadOnly)) {
        emit messageAvailable("Controller::initAltInstrumentData(): "+instrumentNameFullPath+" "+altInstDataFile.errorString(), "error");
        return;
    }

    bool bayerFound = false;
    QTextStream in(&(altInstDataFile));
    while(!in.atEnd()) {
        QString line = in.readLine().simplified();
        if (line.isEmpty() || line.contains("#")) continue;

        // scalars
        if (line.contains("INSTRUMENT=")) altInstData.name = line.split("=")[1];
        if (line.contains("INSTSHORT=")) altInstData.shortName = line.split("=")[1];
        if (line.contains("NCHIPS=")) altInstData.numChips = line.split("=")[1].toInt();
        if (line.contains("TYPE=")) altInstData.type = line.split("=")[1];
        if (line.contains("BAYER=")) {
            // BAYER is not mandatory; if not found, we must set it to blank
            altInstData.bayer = line.split("=")[1];
            bayerFound = true;
        }
        if (line.contains("OBSLAT=")) altInstData.obslat = line.split("=")[1].toFloat();
        if (line.contains("OBSLONG=")) altInstData.obslong = line.split("=")[1].toFloat();
        if (line.contains("PIXSCALE=")) altInstData.pixscale = line.split("=")[1].toFloat();
//        if (line.contains("GAIN=")) altInstData.gain = line.split("=")[1].toFloat();

        // vectors
        if (line.contains("OVSCANX1=")
                || line.contains("OVSCANX2=")
                || line.contains("OVSCANY1=")
                || line.contains("OVSCANY2=")
                || line.contains("CUTX=")
                || line.contains("CUTY=")
                || line.contains("SIZEX=")
                || line.contains("SIZEY=")
                || line.contains("REFPIXX=")
                || line.contains("REFPIXY=")) {
            line = line.replace('=',' ').replace(')',' ').replace(')',"");
            line = line.simplified();
            QStringList values = line.split(" ");
            QVector<int> vecData;
            // NOTE: already subtracting -1 to make it conform with C++ indexing
            // (apart from SIZEX/Y, which is the actual number of pixels per axis and not a coordinate)
            for (int i=2; i<values.length(); i=i+2) {
                if (line.contains("SIZE")) vecData.push_back(values.at(i).toInt());
                else vecData.push_back(values.at(i).toInt() - 1);
                //     vecData.push_back(values.at(i).toInt() - 1);
            }
            if (line.contains("OVSCANX1")) altInstData.overscan_xmin = vecData;
            if (line.contains("OVSCANX2")) altInstData.overscan_xmax = vecData;
            if (line.contains("OVSCANY1")) altInstData.overscan_ymin = vecData;
            if (line.contains("OVSCANY2")) altInstData.overscan_ymax = vecData;
            if (line.contains("CUTX")) altInstData.cutx = vecData;
            if (line.contains("CUTY")) altInstData.cuty = vecData;
            if (line.contains("SIZEX")) altInstData.sizex = vecData;
            if (line.contains("SIZEY")) altInstData.sizey = vecData;
            if (line.contains("REFPIXX")) altInstData.crpix1 = vecData;
            if (line.contains("REFPIXY")) altInstData.crpix2 = vecData;
        }
    }

    if (!bayerFound) altInstData.bayer = "";
    // Backwards compatibility:
    if (altInstData.type.isEmpty()) altInstData.type = "OPT";
    QString shortstring = altInstData.name.split('@').at(0);
    if (altInstData.shortName.isEmpty()) altInstData.shortName = shortstring;

    altInstDataFile.close();

    // The overscan needs special treatment:
    // if it is consistently -1, or the string wasn't found,
    // then the vector must be empty
    testOverscan(altInstData.overscan_xmin);
    testOverscan(altInstData.overscan_xmax);
    testOverscan(altInstData.overscan_ymin);
    testOverscan(altInstData.overscan_ymax);
}

// copied from MainWindow
void Controller::testOverscan(QVector<int> &overscan)
{
    if (overscan.isEmpty()) return;

    // if the overscan is consistently -1, then the vector must be empty
    bool flag = true;
    for (auto &it : overscan) {
        if (it != -1) {
            flag = false;
            break;
        }
    }
    if (flag) overscan.clear();
}

// Runs outside the processing thread, but kept here for consistency
void Controller::provideAlternativeMask()
{
    // If the NIR detector config is chosen for GROND, then we must create separate masks for the optical data.
    // Not the other way round, as the masking for the NIR data is handled elsewhere.
    if (instData->name == "GROND_NIR@MPGESO") {
        initAltInstrumentData(instrument_dir+"/GROND_OPT@MPGESO.ini");
        altMask = new Mask(&altInstData, this);
    }
}
