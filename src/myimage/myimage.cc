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

#include "myimage.h"
#include "../functions.h"
#include "../tools/polygon.h"
#include "../tools/tools.h"
#include "../tools/cfitsioerrorcodes.h"
#include "../processingInternal/data.h"
#include "../processingStatus/processingStatus.h"
#include "wcs.h"
#include "wcshdr.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QString>
#include <QTest>

// C'tor
MyImage::MyImage(QString pathname, QString filename, QString statusString, int chipnumber,
                 const QVector<bool> &mask, int *verbose, bool makebackup, QObject *parent) : QObject(parent), globalMask(mask)
{
    path = pathname;
    name = filename;
    chipNumber = chipnumber;
    QFileInfo fi(path+"/"+name);
    if (globalMask.isEmpty()) globalMaskAvailable = false;

    baseName = fi.completeBaseName();
    rootName = baseName;
    rootName.truncate(rootName.lastIndexOf('_'));
    chipName = rootName+"_"+QString::number(chipNumber);
    makeBackup = makebackup;
    weightName = chipName+".weight";

    processingStatus = new ProcessingStatus(path, this);
    processingStatus->statusString = statusString;
    processingStatus->statusToBoolean(processingStatus->statusString);

    if (! QFile(path+"/"+filename).exists()) {
        // A new file is being created
        validFile = false;
        validMode = false;
        validBackground = false;
        validDetector = false;
    }
    else {
        // An existing file is being read
        validFile = true;
        validMode = true;
        validBackground = true;
        validDetector = true;
        weightPath = path+"/../WEIGHTS/";
    }

    verbosity = verbose;

    wcs = new wcsprm();
    wcsInit = true;

    omp_init_lock(&backgroundLock);
    omp_init_lock(&objectLock);
}

void MyImage::checkTaskRepeatStatus(QString taskBasename)
{
    isTaskRepeated = false;

    if (taskBasename == "HDUreformat" && processingStatus->HDUreformat) isTaskRepeated = true;
    else if (taskBasename == "Processscience" && processingStatus->Processscience) isTaskRepeated = true;
    else if (taskBasename == "Chopnod" && processingStatus->Chopnod) isTaskRepeated = true;
    else if (taskBasename == "Background" && processingStatus->Background) isTaskRepeated = true;
    else if (taskBasename == "Collapse" && processingStatus->Collapse) isTaskRepeated = true;
    else if (taskBasename == "Starflat" && processingStatus->Starflat) isTaskRepeated = true;
    else if (taskBasename == "Skysub" && processingStatus->Skysub) isTaskRepeated = true;
}

MyImage::MyImage(QString fullPathName, const QVector<bool> &mask, int *verbose, QObject *parent) :
    QObject(parent), globalMask(mask)
{
    QFileInfo fi(fullPathName);

    path = fi.absolutePath();
    name = fi.fileName();
    chipNumber = 1;
    baseName = fi.completeBaseName();

    rootName = baseName;
    rootName.truncate(rootName.lastIndexOf('_'));
    chipName = rootName+"_"+QString::number(chipNumber);

    makeBackup = false;
    weightName = baseName+".weight";

    if (globalMask.isEmpty()) globalMaskAvailable = false;

    processingStatus = new ProcessingStatus(path);
    processingStatus->statusString = "";
    processingStatus->statusToBoolean(processingStatus->statusString);

    if (! QFile(fullPathName).exists()) {
        // A new file is being created
        validFile = false;
        validMode = false;
        validBackground = false;
        validDetector = false;
    }
    else {
        // An existing file is being read
        validFile = true;
        validMode = true;
        validBackground = true;
        validDetector = true;
        // Create the FITS instance, but do not open and read it yet.
        weightPath = path;
    }

    verbosity = verbose;

    wcs = new wcsprm();
    wcsInit = true;

    omp_init_lock(&backgroundLock);
    omp_init_lock(&objectLock);
}

MyImage::~MyImage()
{
    for (auto &object : objectList) {
        delete object;
    }

    if (wcsInit) wcsfree(wcs);
    if (wcsInit) delete wcs;  // valgrind does not like that

    int status = 0;
    if (fullheaderAllocated) fits_free_memory(fullheader, &status);

    omp_destroy_lock(&backgroundLock);
    omp_destroy_lock(&objectLock);
}

void MyImage::setObjectLock(bool locked)
{
    if (locked) omp_set_lock(&objectLock);
    else omp_unset_lock(&objectLock);
}

void MyImage::setBackgroundLock(bool locked)
{
    if (locked) omp_set_lock(&backgroundLock);
    else omp_unset_lock(&backgroundLock);
}

void MyImage::updateProcInfo(QString text)
{
    if (procInfo.isEmpty()) procInfo.append(text);
    else procInfo.append(", "+text);
}

void MyImage::showProcInfo()
{
    if (*verbosity > 1) emit messageAvailable(chipName + " : " + procInfo, "image");
}

void MyImage::readImage(bool determineMode)
{
    dataCurrent_deletable = false;
    dataWeight_deletable = false;

    // Leave if image is already loaded
    if (imageInMemory) {
        updateProcInfo("Already in memory");
        if (*verbosity > 2) emit messageAvailable(chipName+" : Already in memory", "image");
        return;
    }
    else {
        if (loadData()) {
            getMode(determineMode);
            imageInMemory = true;
            imageOnDrive = true;
            successProcessing = true;
            headerInfoProvided = true;
            updateProcInfo("Loaded");
            if (*verbosity > 2) emit messageAvailable(chipName+" : Loaded", "image");

        }
        else {
            emit messageAvailable("MyImage::readImage(): Could not load " + baseName + ".fits", "error");
            emit critical();
            successProcessing = false;
        }
        emit modelUpdateNeeded(baseName, chipName);
    }
}

// Used by iview when loading directly from FITS files, by swarpfilter when reading weights,
// and by color picture when cropping images
void MyImage::readImage(QString loadFileName)
{
    dataCurrent_deletable = false;
    dataWeight_deletable = false;

    // Leave if image is already loaded
    if (imageInMemory) {
        updateProcInfo("Already in memory");
        if (*verbosity > 2) emit messageAvailable(chipName+" : Already in memory", "image");
        return;
    }
    else {
        if (loadData(loadFileName)) {
            imageInMemory = true;
            imageOnDrive = true;
            successProcessing = true;
            headerInfoProvided = true;
            updateProcInfo("Loaded");
            if (*verbosity > 2) emit messageAvailable(chipName+" : Loaded", "image");
        }
        else {
            emit messageAvailable("MyImage::readImage(): Could not load " + baseName + ".fits", "error");
            emit critical();
            successProcessing = false;
        }
        emit modelUpdateNeeded(baseName, chipName);
    }
}

// When having to read from a backup file right after launch (task repeated)
void MyImage::readImageBackupL1Launch()
{
    dataCurrent_deletable = false;
    dataWeight_deletable = false;
    dataBackupL1_deletable = false;

    bool determineMode = true;

    QString loadFileName = pathBackupL1 + "/" + baseNameBackupL1 + ".fits";
    if (loadData(loadFileName)) {
        getMode(determineMode);
        dataBackupL1 = dataCurrent;
        imageInMemory = true;
        backupL1OnDrive = true;
        backupL1InMemory = true;
        successProcessing = true;
        headerInfoProvided = true;
        updateProcInfo("Loaded");
        if (*verbosity > 2) emit messageAvailable(baseNameBackupL1+" : Loaded", "image");
    }
    else {
        emit messageAvailable("MyImage::readImageBackupL1Launch(): Could not load " + baseNameBackupL1, "error");
        emit critical();
        successProcessing = false;
    }
    emit modelUpdateNeeded(baseName, chipName);
}

// when MyImage has been read before and all members are setup correctly, just the pixel data are missing.
// This function is used if the pixel data were discarded due to memory constraints, and now they are needed again
void MyImage::readImageBackupL1()
{
    dataCurrent_deletable = false;
    dataBackupL1_deletable = false;

    // Leave if image is already loaded
    if (backupL1InMemory) {
        updateProcInfo("Already in memory");
        if (*verbosity > 2) emit messageAvailable(baseNameBackupL1+" : Already in memory", "image");
        return;
    }
    else {
        int status = 0;
        long nelements = naxis1*naxis2;
        float *buffer = new float[nelements];
        float nullval = 0.;
        int anynull;
        long fpixel = 1;
        fitsfile *fptr = nullptr;
        QString backupName = pathBackupL1 + "/" + baseNameBackupL1 + ".fits";
        QFile backupFile(backupName);
        if (!backupFile.exists()) {
            emit messageAvailable(baseName + " : Could not read data backup file " + pathBackupL1 + "/" + baseNameBackupL1 + " (does not exist)!", "error");
            emit critical();
            successProcessing = false;
            return;
        }
        fits_open_file(&fptr, backupName.toUtf8().data(), READONLY, &status);
        fits_read_img(fptr, TFLOAT, fpixel, nelements, &nullval, buffer, &anynull, &status);
        fits_close_file(fptr, &status);
        printCfitsioError("readImageBackupL1()", status);

        dataBackupL1.resize(nelements);
        for (long i=0; i<nelements; ++i) {
            dataBackupL1[i] = buffer[i];
        }
        delete [] buffer;
        backupL1InMemory = true;
        dataCurrent = dataBackupL1;     // probably unnecessary, as we operate on databackupL1, updating dataCurrent
        dataBackupL1.squeeze();  // shed excess memory
        dataCurrent.squeeze();  // shed excess memory
        imageInMemory = true;
    }
}

// Retrieve the zero order solution from the currently created header file, or restore it from the original backup copy;
// This includes FLXSCALE and RZP
bool MyImage::scanAstromHeader(int chip, QString mode)
{
    QFile file;
    if (mode == "inBackupDir") {
        file.setFileName(path+"/.origheader_backup/"+rootName+"_"+QString::number(chip+1)+".origastrom.head");
        if(!file.open(QIODevice::ReadOnly)) {
            emit messageAvailable("MyImage::scanAstromHeader(): " + file.fileName() + " " + file.errorString(), "error");
            emit critical();
            return false;
        }
    }
    else {
        // mode == "inHeadersDir"
        file.setFileName(path+"/headers/"+rootName+"_"+QString::number(chip+1)+".head");
        if(!file.open(QIODevice::ReadOnly)) {
            emit messageAvailable("MyImage::scanAstromHeader(): " + file.fileName() + " " + file.errorString(), "error");
            emit critical();
            return false;
        }
    }

    double cd11_orig = wcs->cd[0];
    double cd12_orig = wcs->cd[1];
    double cd21_orig = wcs->cd[2];
    double cd22_orig = wcs->cd[3];

    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = in.readLine().simplified();
        if (line.isEmpty()) continue;
        QStringList list = line.split(" ");
        if (list.length() < 3) continue;
        if (line.contains("CRVAL1")) wcs->crval[0] = list[2].toDouble();
        if (line.contains("CRVAL2")) wcs->crval[1] = list[2].toDouble();
        if (line.contains("CRPIX1")) wcs->crpix[0] = list[2].toFloat();
        if (line.contains("CRPIX2")) wcs->crpix[1] = list[2].toFloat();
        if (line.contains("CD1_1")) wcs->cd[0] = list[2].toDouble();
        if (line.contains("CD1_2")) wcs->cd[1] = list[2].toDouble();
        if (line.contains("CD2_1")) wcs->cd[2] = list[2].toDouble();
        if (line.contains("CD2_2")) wcs->cd[3] = list[2].toDouble();
        if (line.contains("FLXSCALE")) FLXSCALE = list[2].toFloat();
        if (line.contains("RZP")) RZP = list[2].toFloat();
    }

    file.close();

    // Do not update the WCS matrix if it is significantly flawed
    if (sanityCheckWCS(wcs).isEmpty()) {
        wcs->flag = 0;  // Trigger recomputation
        cornersToRaDec();
        return true;
    }
    else {
        // Restore original values
        wcs->cd[0] = cd11_orig;
        wcs->cd[1] = cd12_orig;
        wcs->cd[2] = cd21_orig;
        wcs->cd[3] = cd22_orig;
        cornersToRaDec();
        return false;
    }
}

void MyImage::checkWCSsanity()
{
    // Do not update the WCS matrix if it is significantly flawed
    if (sanityCheckWCS(wcs).isEmpty()) return;
    else {
        wcs->cd[0] = -1.*plateScale/3600.;
        wcs->cd[1] = 0.;
        wcs->cd[2] = 0.;
        wcs->cd[3] = plateScale/3600.;
        wcs->flag = 0;
        updateCRVALCDinHeader();
        updateCRVALCDinHeaderOnDrive();
        emit messageAvailable("Singular or skewed CD matrix detected, reset to default values!", "warning");
        emit warning();
    }
}

void MyImage::backupOrigHeader(int chip)
{
    QDir dir(path+ "/.origheader_backup/");
    dir.mkpath(path+ "/.origheader_backup/");

    // DO NOT OVERWRITE EXISTING BACKUP COPY
    QFile file(path+"/.origheader_backup/"+rootName+"_"+QString::number(chip+1)+".origastrom.head");
    if (file.exists()) return;

    // Backup copy does not exist, create it
    if(!file.open(QIODevice::WriteOnly)) {
        emit messageAvailable("MyImage::backupOrigHeader(): " + file.fileName() + " " + file.errorString(), "error");
        emit critical();
        return;
    }
    QTextStream stream(&file);

    fitsfile *fptr;
    int status = 0;
    int nkeys = 0;
    int keypos = 0;
    char card[FLEN_CARD];
    // Must read from pathBackupL1
    QString filename = pathBackupL1+"/"+baseNameBackupL1+".fits";
    //    QString filename = path+"/"+baseName+".fits";
    fits_open_file(&fptr, filename.toUtf8().data(), READWRITE, &status);
    fits_get_hdrpos(fptr, &nkeys, &keypos, &status); // get number of keywords
    for (int i=1; i<=nkeys; ++i) {
        fits_read_record(fptr, i, card, &status);
        stream << card << "\n";
    }
    if (status == END_OF_FILE) status = 0;
    else {
        printCfitsioError("backupOrigHeader()", status);
    }
    fits_close_file(fptr, &status);
    file.close();
    file.setPermissions(QFile::ReadUser | QFile::WriteUser);
}


void MyImage::replaceCardInFullHeaderString(QString keyname, double value)
{
    QString card = keyname;
    card.resize(8, ' ');
    card.append("= ");
    card.append(QString::number(value, 'g', 12));
    card.resize(80, ' ');

    long dim = strlen(fullheader);
    for (long i=0; i<dim-9; ++i) {
        if (fullheader[i] == card.at(0)
                &&  fullheader[i+1] == card.at(1)
                &&  fullheader[i+2] == card.at(2)
                &&  fullheader[i+3] == card.at(3)
                &&  fullheader[i+4] == card.at(4)
                &&  fullheader[i+5] == card.at(5)
                &&  fullheader[i+6] == card.at(6)
                &&  fullheader[i+7] == card.at(7)
                &&  fullheader[i+8] == card.at(8)) {
            for (int j=0; j<80; ++j) fullheader[i+j] = card.toUtf8().data()[j];
        }
    }
}

void MyImage::updateMode()
{
    if (!successProcessing) return;

    // Force an update of the mode
    skyValue = modeMask(dataCurrent, "stable", globalMask)[0];
    modeDetermined = true;
    QString skyvalue = "SKYVALUE= "+QString::number(skyValue);
    if (*verbosity > 1) emit messageAvailable(chipName + " : " + skyvalue, "image");
    skyvalue.resize(80,' ');
    // header is empty when running equalizeBayerFlat
    if (!header.contains("SKYVALUE=") && !header.isEmpty()) {
        header.insert(header.end()-1, skyvalue);
    }
}

void MyImage::updateHeaderValue(QString keyName, float keyValue)
{
    if (!successProcessing) return;

    QString card = keyName;
    card.resize(8, ' ');
    card.append("= ");
    QString keyword = card;
    card.append(QString::number(keyValue));
    card.resize(80, ' ');
    if (!header.contains(keyword) && !header.isEmpty()) {
        header.insert(header.end()-1, card);
    }
    else {
        for (auto &existingCard : header) {
            if (existingCard.contains(keyword)) {
                // Replace old entry
                existingCard = card;
                break;
            }
        }
    }
}

void MyImage::updateHeaderValue(QString keyName, QString keyValue)
{
    if (!successProcessing) return;

    QString card = keyName;
    card.resize(8, ' ');
    card.append("= ");
    QString keyword = card;
    card.append(keyValue);
    card.resize(80, ' ');
    if (!header.contains(keyword) && !header.isEmpty()) {
        header.insert(header.end()-1, card);
    }
    else {
        for (auto &existingCard : header) {
            if (existingCard.contains(keyword)) {
                // Replace old entry
                existingCard = card;
                break;
            }
        }
    }
}

void MyImage::updateHeaderValueInFITS(QString keyName, QString keyValue)
{
    if (!successProcessing) return;

    // Don't do anything if file doesn't exist on drive
    QFile file(path+"/"+name);
    if (!file.exists()) return;

    int status = 0;
    fitsfile *fptr = nullptr;
    fits_open_file(&fptr, (path+"/"+name).toUtf8().data(), READWRITE, &status);
    fits_update_key_str(fptr, keyName.toUtf8().data(), keyValue.toUtf8().data(), nullptr, &status);
    fits_close_file(fptr, &status);
    printCfitsioError("updateHeaderValueInFITS", status);
}

double MyImage::getPlateScale()
{
    if (!wcsInit) {
        if (*verbosity > 2) emit messageAvailable(chipName + " : No WCS. PlateScale set to 1.0", "info");
        return 1.0;
    }

    // replace with sth more sophisticated
    plateScale = sqrt(wcs->cd[0] * wcs->cd[0] + wcs->cd[2] * wcs->cd[2]) * 3600.;
    if (plateScale == 0.) plateScale = 1.0;
    return plateScale;
}

void MyImage::getMode(bool determineMode)
{
    if (!successProcessing) return;

    // Get the mode only if requested, and measure it only if it hasn't been measured already
    // (in which case it is available as the SKYVALUE header keyword)
    if (determineMode && !modeDetermined) {
        skyValue = modeMask(dataCurrent, "stable", globalMask)[0];
        modeDetermined = true;
        QString skyvalue = "SKYVALUE= "+QString::number(skyValue);
        if (*verbosity > 1) emit messageAvailable(chipName + " : " + skyvalue, "image");
        skyvalue.resize(80,' ');
        if (!header.isEmpty()) {
            if (!header.contains("SKYVALUE=")) {
                header.insert(header.end()-1, skyvalue);
            }
        }
        else {
            emit messageAvailable("MyImage::getMode():" + baseName + " : header vector is empty.", "error");
            successProcessing = false;
        }
    }
}

// Flag the image if its mode is outside the acceptable range
void MyImage::setModeFlag(QString min, QString max)
{
    if (!successProcessing) return;
    if (min.isEmpty() && max.isEmpty()) return;

    if (!modeDetermined) {
        // TODO: implement mode() for integer FITS files, and measure it here (raw data, e.g. darks or flats)
        return;
    }
    else {
        validMode = true;
        if (!min.isEmpty() && skyValue < min.toFloat()) validMode = false;
        if (!max.isEmpty() && skyValue > max.toFloat()) validMode = false;
        if (!validMode) {
            if (*verbosity > 1) emit messageAvailable(chipName + " : Mode = " + QString::number(skyValue,'f',3)
                                                      + " outside user-defined limits ["+min+","+max+"]", "output");
        }
        successProcessing = true;
    }
}

// does the same as "updateHeaderValue()"
void MyImage::updateKeywordInHeader(QString key, QString value)
{
    for (auto &it : header) {
        if (!it.contains("=")) continue;
        QStringList list = it.split("=");
        QString keyword = list[0];
        //        QString value = list[1].simplified();
        if (keyword.simplified() == key) {
            it = keyword+"= "+value;
            // pad with empty chars until 80 chars long
            while (it.length() < 80) {
                it.append(' ');
            }
            return;
        }
    }
}

// Here, 'biasData' may also refer to a dark or flatoff image
void MyImage::subtractBias(MyImage *biasImage, QString dataType)
{
    if (!successProcessing) return;
    if (!validMode) return;
    // We have verified "above" that the bias was successfully loaded

    long i = 0;
    for (auto &pixel : dataCurrent) {
        pixel -= biasImage->dataCurrent[i];
        ++i;
    }

    QString mode = "";
    if (dataType == "BIAS") mode = "bias subtracted";
    else if (dataType == "DARK") mode = "dark subtracted";
    else if (dataType == "FLATOFF") mode = "flatoff subtracted";
    else {
        // nothing
    }

    if (*verbosity > 1) emit messageAvailable(chipName + " : "+mode, "image");
    successProcessing = true;
}

void MyImage::divideFlat(MyImage *flatImage)
{
    if (!successProcessing) return;
    if (!validMode) return;
    // We have verified "above" that the flat was successfully loaded

    long i = 0;
    for (auto &pixel : dataCurrent) {
        // Divide by flat, and correct for gain differences
        pixel /= (flatImage->dataCurrent[i] * flatImage->gainNormalization);
        // NaN pixels slow down SourceExtractor enourmously (and make it fail).
        // we can probably get rid of this once we have completely thrown out SourceExtractor
        if (std::isnan(pixel) || std::isinf(pixel)) pixel = 0.;
        ++i;
    }

    if (*verbosity > 1) emit messageAvailable(chipName + " : Flat fielded", "image");
    successProcessing = true;
}

void MyImage::applyBackgroundModel(MyImage *backgroundImage, QString mode, bool rescaleFlag)
{
    if (!successProcessing) return;

    long i = 0;
    if (mode == "Subtract model" ) {
        float rescale = 1.0;
        if (rescaleFlag) rescale = skyValue / backgroundImage->skyValue;
        for (auto &pixel : dataCurrent) {
            if (backgroundImage->dataCurrent[i] != 0.) {
                pixel = dataBackupL1[i] - backgroundImage->dataCurrent[i] * rescale;
            }
            else {
                pixel = 0.;
            }
            ++i;
        }
        QString img = " IMG = "+QString::number(skyValue, 'f', 3) + ";";
        QString back = " BACK = "+QString::number(backgroundImage->skyValue, 'f', 3) + ";";
        QString fac = " rescale = "+QString::number(rescale, 'f', 3);
        if (*verbosity > 1) emit messageAvailable(chipName + " : Background model subtracted;"+img+back+fac, "image");
    }
    else if (mode == "Divide model") {
        // rescaling switched off. Background image is always normalized to its own mode
        for (auto &pixel : dataCurrent) {
            if (backgroundImage->dataCurrent[i] != 0) {
                pixel = dataBackupL1[i] / (backgroundImage->dataCurrent[i] / backgroundImage->skyValue);
            }
            else {
                pixel = backgroundImage->skyValue;
            }
            ++i;
        }
        QString img = " IMG = "+QString::number(skyValue, 'f', 3) + ";";
        QString back = " BACK = "+QString::number(backgroundImage->skyValue, 'f', 3) + ";";
        if (*verbosity > 1) emit messageAvailable(chipName + " : Divided by normalized background model;"+img+back, "image");
    }
    successProcessing = true;
}

// Normalize Flat to one. Gain differences are corrected separately.
void MyImage::normalizeFlat()
{
    if (!successProcessing) return;

    if (*verbosity > 1) emit messageAvailable(chipName + " : Normalizing flat field, dividing by "+QString::number(skyValue, 'f', 3), "image");
    for (auto &pixel : dataCurrent) {
        pixel /= skyValue;
    }

    successProcessing = true;
}

// Normalize Flat to one. Gain differences are corrected separately.
void MyImage::illuminationCorrection(int chip, QString thelidir, QString instName, QString filter)
{
    if (!successProcessing) return;

    // It is ok to look up and read the illum correction files in MyImage (instead of Controller)
    // because it is applied to the master flat, and therefore executed only once.
    // If we applied it to every science frame, then it would be better to implement it in Controller
    // so it gets executed only once
    // TODO: must provide in distribution?
    QString illumcorrPath = thelidir+"/ExtCal/"+instName+"/illumcorr/";
    QString illumcorrFileName = "illumcorr_"+filter+"_"+QString::number(chip)+".fits";
    if (QFile(illumcorrPath+illumcorrFileName).exists()) {
        if (*verbosity > 1) emit messageAvailable(chipName + " : External illumination correction : <br>" + illumcorrPath+illumcorrFileName, "image");
        MyImage *illumCorrFlat = new MyImage(illumcorrPath, illumcorrFileName, "", chip+1, QVector<bool>(), verbosity);
        illumCorrFlat->readImage();
        if (naxis1 != illumCorrFlat->naxis1 || naxis2 != illumCorrFlat->naxis2 ) {
            emit messageAvailable("MyImage::illuminationCorrection(): " + baseName + " : illumination correction image does not have the same size as the master flat!", "error");
            emit critical();
            successProcessing = false;
        }
        else {
            long i = 0;
            for (auto &pixel : dataCurrent) {
                pixel *= illumCorrFlat->dataCurrent[i];
                ++i;
            }
            successProcessing = true;
        }
        delete illumCorrFlat;
    }
}

void MyImage::collapseCorrection(QString threshold, QString direction)
{
    if (!successProcessing) return;

    if (*verbosity > 1) emit messageAvailable(chipName + " : Collapse correction along " + direction, "image");

    if (direction == "x") {
        static_cast<void> (collapse_x(dataCurrent, globalMask, objectMask, threshold.toFloat(), naxis1, naxis2, "2Dsubtract"));
    }
    else if (direction == "y") {
        static_cast<void> (collapse_y(dataCurrent, globalMask, objectMask, threshold.toFloat(), naxis1, naxis2, "2Dsubtract"));
    }
    else if (direction == "xy") {
        static_cast<void> (collapse_x(dataCurrent, globalMask, objectMask, threshold.toFloat(), naxis1, naxis2, "2Dsubtract"));
        static_cast<void> (collapse_y(dataCurrent, globalMask, objectMask, threshold.toFloat(), naxis1, naxis2, "2Dsubtract"));
    }
    else if (direction == "yx") {
        static_cast<void> (collapse_y(dataCurrent, globalMask, objectMask, threshold.toFloat(), naxis1, naxis2, "2Dsubtract"));
        static_cast<void> (collapse_x(dataCurrent, globalMask, objectMask, threshold.toFloat(), naxis1, naxis2, "2Dsubtract"));
    }
    else {
        static_cast<void> (collapse_quad(dataCurrent, globalMask, objectMask, threshold.toFloat(), naxis1, naxis2, direction, "2Dsubtract"));
    }
}

QVector<float> MyImage::extractPixelValues(long xmin, long xmax, long ymin, long ymax)
{
    // CHECK: if not in memory then load from drive

    long nsub = xmax - xmin + 1;
    long msub = ymax - ymin + 1;

    QVector<float> section;
    /*
    section.reserve(nsub*msub);

    for (long j=ymin; j<=ymax; ++j) {
        for (long i=xmin; i<=xmax; ++i) {
            section.append(dataCurrent[i+naxis1*j]);
        }
    }
    */

    section.resize(nsub*msub);
    long k = 0;
    for (long j=ymin; j<=ymax; ++j) {
        for (long i=xmin; i<=xmax; ++i) {
            section[k] = dataCurrent[i+naxis1*j];
            ++k;
        }
    }

    return section;
}

void MyImage::makeCutout(long xmin, long xmax, long ymin, long ymax)
{
    long nsub = xmax - xmin + 1;
    long msub = ymax - ymin + 1;

    QVector<float> dataCut;

    /*
    dataCut.reserve(nsub*msub);
    for (long j=ymin; j<=ymax; ++j) {
        for (long i=xmin; i<=xmax; ++i) {
            dataCut.append(dataCurrent[i+naxis1*j]);
        }
    }
    */

    long k = 0;
    dataCut.resize(nsub*msub);
    for (long j=ymin; j<=ymax; ++j) {
        for (long i=xmin; i<=xmax; ++i) {
            dataCut[k] = dataCurrent[i+naxis1*j];
            ++k;
        }
    }

    naxis1 = nsub;
    naxis2 = msub;
    if (wcsInit) {
        wcs->crpix[0] = wcs->crpix[0] - xmin + 1;
        wcs->crpix[1] = wcs->crpix[1] - ymin + 1;
        wcs->flag = 0;
    }

    dataCurrent.swap(dataCut);
}

// UNUSED
float MyImage::polynomialSum(float x, QVector<float> coefficients)
{
    // Coefficients contains the coeffs of a polynomial, starting with the lowest term
    // e.g., for p(x) = c0 + c1*x + c2*x*x  we have coefficients = [c0, c1, c2]
    float sum = 0.;
    int k = 0;
    for (auto & it : coefficients) {
        sum += it * pow(x, k);
        ++k;
    }
    return sum;
}


void MyImage::removeSourceCatalogs()
{
    QString catName = path+"/cat/"+chipName+".cat";
    QFile catFile1(catName);
    if (catFile1.exists()) catFile1.remove();

    catName = path+"/cat/"+chipName+".anet";
    QFile catFile2(catName);
    if (catFile2.exists()) catFile2.remove();
}

// This routine is used when reading an external image
// (and possibly its existing weight in the same directory, such as a pair of coadd.fits and coadd.weight.fits)
void MyImage::setupCoaddMode()
{
    readImage();

    // Setup an empty dummy globalMask
    //    globalMask = QVector<bool>();
    globalMaskAvailable = false;

    // Load a matching weight image, if one exists
    weightName.append(".fits");

    QFile file(path+"/"+weightName);
    if (!file.exists()) return;

    MyImage *myWeight = new MyImage(path, weightName, "", 1, QVector<bool>(), verbosity, false);
    myWeight->readImage();
    dataWeight.swap(myWeight->dataCurrent);

    delete myWeight;
}

void MyImage::updateZeroOrderOnDrive(QString updateMode)
{
    if (!successProcessing) return;
    if (!imageOnDrive) return;

    // Must write file to disk (scamp reads the header information) if it does not exist yet
    //    QFile file(path+ "/" + baseName + ".fits");
    //    if (!file.exists()) writeImage(path+ "/" + baseName + ".fits");

    int status = 0;
    char zerohead[80] = {0};
    QString zeroheadString = "";
    fitsfile *fptr = nullptr;
    fits_open_file(&fptr, (path+"/"+chipName+processingStatus->statusString+".fits").toUtf8().data(), READWRITE, &status);
    fits_read_key_str(fptr, "ZEROHEAD", zerohead, nullptr, &status);
    if (status > 0) {
        // Add the key if it doesn't exist
        // Not used for anything, just informative
        fits_update_key_str(fptr, "ZEROHEAD", "NO", "Astrometric header update", &status); // reset the update flag
        zeroheadString = "NO";
        status = 0;
    }
    else zeroheadString.fromLatin1(zerohead);

    fits_update_key_dbl(fptr, "CRVAL1", wcs->crval[0], 9, nullptr, &status);
    fits_update_key_dbl(fptr, "CRVAL2", wcs->crval[1], 9, nullptr, &status);
    fits_update_key_flt(fptr, "CRPIX1", wcs->crpix[0], 3, nullptr, &status);
    fits_update_key_flt(fptr, "CRPIX2", wcs->crpix[1], 3, nullptr, &status);
    fits_update_key_dbl(fptr, "CD1_1", wcs->cd[0], 9, nullptr, &status);
    fits_update_key_dbl(fptr, "CD1_2", wcs->cd[1], 9, nullptr, &status);
    fits_update_key_dbl(fptr, "CD2_1", wcs->cd[2], 9, nullptr, &status);
    fits_update_key_dbl(fptr, "CD2_2", wcs->cd[3], 9, nullptr, &status);
    if (std::isnan(RZP)) {
        RZP = 0.;
        FLXSCALE = 0.;
        emit messageAvailable(chipName + " : Scamp could not determine the relative zeropoint. Set to 0!", "warning");
    }
    fits_update_key_flt(fptr, "RZP", RZP, 3, nullptr, &status);
    fits_update_key_flt(fptr, "FLXSCALE", FLXSCALE, 3, nullptr, &status);
    if (updateMode == "restore") {
        fits_update_key_str(fptr, "ZEROHEAD", "NO", "Astrometric header update", &status); // reset the update flag
    }
    else {
        // updateMode == "update"
        fits_update_key_str(fptr, "ZEROHEAD", "YES", "Astrometric header update", &status); // reset the update flag
    }

    fits_close_file(fptr, &status);
    printCfitsioError("updateZeroOrderOnDrive()", status);
}

/*
        void MyImage::updateZeroOrderInMemory()
        {
            if (!successProcessing) return;

            // Update the data in memory
            // WCSLIB threading issue (probably solved by wcsLock in initWCS()
            // Can be removed if we don't see this again
            if (wcs->naxis != 2) {
                emit messageAvailable(chipName + " : MyImage::updateZeroOrder(): Incompatible NAXIS WCS dimension: "
                                      + QString::number(wcs->naxis) + ", attempting reload (actually, this is a bug!)", "warning");
                imageInMemory = false;
                readImage(false);
                if (wcs->naxis != 2) {
                    emit messageAvailable(chipName + " : Reload failed!", "error");
                    return;
                }
                else {
                    emit messageAvailable(chipName + " : Reload success!", "note");
                }
            }

            cornersToRaDec();
        }
        */

// Update the 'header' QVector passed onto any newly ritten FITS file
void MyImage::updateCRVALinHeader()
{
    updateHeaderValue("CRVAL1", wcs->crval[0]);
    updateHeaderValue("CRVAL2", wcs->crval[1]);
}

// Update the 'header' QVector passed onto any newly ritten FITS file
void MyImage::updateCRVALCDinHeader()
{
    updateHeaderValue("CRVAL1", wcs->crval[0]);
    updateHeaderValue("CRVAL2", wcs->crval[1]);
    updateHeaderValue("CD1_1", wcs->cd[0]);
    updateHeaderValue("CD1_2", wcs->cd[1]);
    updateHeaderValue("CD2_1", wcs->cd[2]);
    updateHeaderValue("CD2_2", wcs->cd[3]);
}

void MyImage::updateCRVALinHeaderOnDrive()
{
    // Must write file to drive (scamp reads the header information) if it does not exist yet
    QString outfile = path+"/"+chipName+processingStatus->statusString+".fits";
    //  QFile file(path+ "/" + baseName + ".fits");
    QFile file(outfile);
    if (!file.exists()) writeImage(outfile);

    int status = 0;
    fitsfile *fptr = nullptr;
    fits_open_file(&fptr, (outfile).toUtf8().data(), READWRITE, &status);
    fits_update_key_dbl(fptr, "CRVAL1", wcs->crval[0], 6, nullptr, &status);
    fits_update_key_dbl(fptr, "CRVAL2", wcs->crval[1], 6, nullptr, &status);
    fits_close_file(fptr, &status);
    printCfitsioError("updateCRVALinHeaderOnDrive()", status);
}

void MyImage::updateCRVALCDinHeaderOnDrive()
{
    // Must write file to disk (scamp reads the header information) if it does not exist yet
    QString outfile = path+"/"+chipName+processingStatus->statusString+".fits";
    QFile file(outfile);
    if (!file.exists()) writeImage(outfile);

    int status = 0;
    fitsfile *fptr = nullptr;
    fits_open_file(&fptr, (outfile).toUtf8().data(), READWRITE, &status);
    fits_update_key_dbl(fptr, "CRVAL1", wcs->crval[0], 6, nullptr, &status);
    fits_update_key_dbl(fptr, "CRVAL2", wcs->crval[1], 6, nullptr, &status);
    fits_update_key_flt(fptr, "CD1_1", wcs->cd[0], 6, nullptr, &status);
    fits_update_key_flt(fptr, "CD1_2", wcs->cd[1], 6, nullptr, &status);
    fits_update_key_flt(fptr, "CD2_1", wcs->cd[2], 6, nullptr, &status);
    fits_update_key_flt(fptr, "CD2_2", wcs->cd[3], 6, nullptr, &status);
    fits_close_file(fptr, &status);
    printCfitsioError("updateCRVALCDinHeaderOnDrive()", status);
}

void MyImage::updateInactivePath()
{
    if (activeState == MyImage::ACTIVE) pathExtension = "";
    else if (activeState == MyImage::INACTIVE) pathExtension = "/inactive/";
    else if (activeState == MyImage::BADSTATS) pathExtension = "/inactive/badStatistics/";
    else if (activeState == MyImage::BADBACK) pathExtension = "/inactive/badBackground/";
    else if (activeState == MyImage::LOWDETECTION) pathExtension = "/inactive/lowDetections/";
    else if (activeState == MyImage::DELETED) pathExtension = "/inactive/";
}

void MyImage::applyMask()
{
    // Leave if the chip has no mask (save some CPU cycles)
    if (!globalMaskAvailable) return;

    if (globalMask.length() != dataCurrent.length()) {
        emit messageAvailable("MyImage::applyMask(): " + baseName + " : inconsistent sizes of mask and image", "error");
        emit critical();
        return;
    }

    long i=0;
    for (auto &it : dataCurrent) {
        if (globalMask[i]) it = maskValue;
        ++i;
    }
}

QString MyImage::getKeyword(QString key)
{
    QString value;
    key.resize(8,' ');
    for (auto &it : header) {
        if (it.contains(key)) {
            value = it.split("=")[1].split("/")[0].simplified().remove("'");
            break;
        }
    }
    return value;
}


void MyImage::messageAvailableReceived(QString message, QString type)
{
    if (type == "error" || type == "stop") {
        emit messageAvailable(message, type);
        emit critical();
    }
    else {
        if (*verbosity > 1) emit messageAvailable(message, type);
    }
}

void MyImage::anetOutputReceived(QString message, QString type)
{
    if (type == "error" || type == "stop") {
        emit messageAvailable(message, type);
        emit critical();
    }
    else {
        if (*verbosity >= 1) emit messageAvailable(message, type);
    }
}


QVector<double> MyImage::extractCDmatrix()
{
    QVector<double> cd;
    cd.push_back(getKeyword("CD1_1").toDouble());
    cd.push_back(getKeyword("CD1_2").toDouble());
    cd.push_back(getKeyword("CD2_1").toDouble());
    cd.push_back(getKeyword("CD2_2").toDouble());
    return cd;
}

// TODO: update to use wcslib functions
void MyImage::checkBrightStars(QList<QVector<double>> &brightStarList, float safetyDistance, float plateScale)
{
    if (!successProcessing) return;

    hasBrightStars = false;

    // Loop over all bright stars and check whether they are inside the chip
    for (auto &it : brightStarList) {
        QVector<double> raVec;
        QVector<double> decVec;
        // order is important! we don't want a line crossing in the polygon line
        raVec << alpha_ll << alpha_lr << alpha_ur << alpha_ul;
        decVec << delta_ll << delta_lr << delta_ur << delta_ul;
        double alpha = it.at(0);
        double delta = it.at(1);
        if (pnpoly_T(raVec, decVec, alpha, delta)) {
            hasBrightStars = true;
            return;
        }
    }

    // if we are still here, then check for bright stars outside the chip
    // within the minimum safety distance

    safetyDistance *= (60. / plateScale); // convert to pixel
    for (auto &it : brightStarList) {
        double xstar;
        double ystar;
        double alpha = it.at(0);
        double delta = it.at(1);
        sky2xy(alpha, delta, xstar, ystar);
        if (xstar > -1*safetyDistance
                && xstar < naxis1+safetyDistance
                && ystar > -1*safetyDistance
                && ystar < naxis2+safetyDistance) {
            hasBrightStars = true;
            return;
        }
    }
}

bool MyImage::containsRaDec(QString alphaStr, QString deltaStr)
{
    double alpha_ul;
    double alpha_ur;
    double alpha_ll;
    double alpha_lr;
    double delta_ul;
    double delta_ur;
    double delta_ll;
    double delta_lr;

    // Convert the cartesian image vertices to RA/DEC
    xy2sky(1, 1, alpha_ll, delta_ll);
    xy2sky(naxis1, 1, alpha_lr, delta_lr);
    xy2sky(1, naxis2, alpha_ul, delta_ul);
    xy2sky(naxis1, naxis2, alpha_ur, delta_ur);

    // Check if the sky coordinates are contained in this picture frame
    QVector<double> raVec;
    QVector<double> decVec;
    // order is important! we don't want a line crossing in the polygon line
    raVec << alpha_ll << alpha_lr << alpha_ur << alpha_ul;
    decVec << delta_ll << delta_lr << delta_ur << delta_ul;

    // Convert to decimal if required
    if (alphaStr.contains(":")) alphaStr = hmsToDecimal(alphaStr);
    if (deltaStr.contains(":")) deltaStr = dmsToDecimal(deltaStr);
    return pnpoly_T(raVec, decVec, alphaStr.toDouble(), deltaStr.toDouble());
}

void MyImage::cornersToRaDec()
{
    xy2sky(1, 1, alpha_ll, delta_ll);
    xy2sky(naxis1, 1, alpha_lr, delta_lr);
    xy2sky(1, naxis2, alpha_ul, delta_ul);
    xy2sky(naxis1, naxis2, alpha_ur, delta_ur);
    xy2sky(naxis1/2, naxis2/2, alpha_ctr, delta_ctr);
}

QVector<float> MyImage::retainUnmaskedDataThresholded(float minVal, float maxVal, int sampleDensity)
{
    QVector<float> dataThresholded;
    long n = dataCurrent.length();
    dataThresholded.reserve(n/sampleDensity);
    if (!globalMask.isEmpty()) {
        for (long i=0; i<n; i+=sampleDensity) {
            float it = dataCurrent[i];
            if (it > minVal && it < maxVal && !globalMask[i]) {
                dataThresholded.append(it);
            }
        }
    }
    else {
        for (long i=0; i<n; i+=sampleDensity) {
            float it = dataCurrent[i];
            if (it > minVal && it < maxVal) {
                dataThresholded.append(it);
            }
        }
    }

    return dataThresholded;
}

QVector<float> MyImage::retainUnmaskedData(int sampleDensity)
{
    QVector<float> dataThresholded;
    long n = dataCurrent.length();
    dataThresholded.reserve(n/sampleDensity);
    if (!globalMask.isEmpty()) {
        for (long i=0; i<n; i+=sampleDensity) {
            if (!globalMask[i]) dataThresholded.append(dataCurrent[i]);
        }
    }
    else {
        if (sampleDensity == 1) dataThresholded = dataCurrent;
        else {
            for (long i=0; i<n; i+=sampleDensity) {
                dataThresholded.append(dataCurrent[i]);
            }
        }
    }

    return dataThresholded;
}

void MyImage::mergeObjectWithGlobalMask()
{
    if (objectMask.isEmpty()) objectMask = globalMask;
    else {
        long i=0;
        for (auto &pixel : objectMask) {
            pixel *= globalMask[i];
            ++i;
        }
    }
}

void MyImage::subtract(float value)
{
    for (auto &it : dataCurrent) {
        it -= value;
    }
}

void MyImage::add(float value)
{
    for (auto &it : dataCurrent) {
        it += value;
    }
}

void MyImage::multiply(float value)
{
    for (auto &it : dataCurrent) {
        it *= value;
    }
}

void MyImage::divide(float value)
{
    if (value == 0.) {
        emit messageAvailable("MyImage::divide(): " + baseName + " : Division by zero encountered!", "error");
        emit critical();
        return;
    }
    for (auto &it : dataCurrent) {
        it /= value;
    }
}

void MyImage::sky2xy(const double alpha, const double delta, double &x, double &y)
{
    double world[2];
    double phi;
    double theta;
    double imgcrd[2];
    double pixcrd[2];
    world[0] = alpha;
    world[1] = delta;
    int stat[1];
    wcss2p(wcs, 1, 2, world, &phi, &theta, imgcrd, pixcrd, stat);
    x = pixcrd[0];
    y = pixcrd[1];
}

void MyImage::xy2sky(const double x, const double y, double &alpha, double &delta)
{
    double world[2];
    double phi;
    double theta;
    double imgcrd[2];
    double pixcrd[2];
    pixcrd[0] = x;
    pixcrd[1] = y;
    int stat[1];
    wcsp2s(wcs, 1, 2, pixcrd, imgcrd, &phi, &theta, world, stat);
    alpha = world[0];
    delta = world[1];
}

void MyImage::emitModelUpdateNeeded()
{
    emit modelUpdateNeeded(baseName, chipName);
}

// For use from within the memory viewer
void MyImage::setActiveState(active_type state)
{
    // Always us this function when setting the active status
    activeState = state;

    // TODO: DOES NOT WORK if restoring images. CurrentPath is already the full path including the "inactive" part.
    // file is moved on top of itself (and deleted beforehand).
    // Move the image accordingly
    QString currentPath = path + pathExtension;      // The path where the image is currently located (if on disk)
    updateInactivePath();                            // Update pathextension according to the set state
    QString newPath = path + pathExtension;   // The path where the image should go
    if (!imageOnDrive) return;
    moveFile(baseName+".fits", currentPath, newPath);
    // TODO: must do a modelUpdate
}

void MyImage::printCfitsioError(QString funcName, int status)
{
    if (status) {
        CfitsioErrorCodes *errorCodes = new CfitsioErrorCodes(this);
        emit messageAvailable("MyImage::"+funcName+":<br>" + baseName + " : " + errorCodes->errorKeyMap.value(status), "error");
        emit critical();
        successProcessing = false;
    }
}
