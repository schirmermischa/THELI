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
                 const QVector<bool> &mask, bool masked, int *verbose, bool makebackup,
                 QObject *parent) : QObject(parent)
{
    path = pathname;
    name = filename;
    chipNumber = chipnumber;
    QFileInfo fi(path+"/"+name);
    if (mask.isEmpty()) isMasked = false;
    else {
        globalMask = mask; // This is the globalMask
        isMasked = masked;
    }
    baseName = fi.completeBaseName();
    rootName = baseName;
    rootName.truncate(rootName.lastIndexOf('_'));
    chipName = rootName+"_"+QString::number(chipNumber);
    makeBackup = makebackup;
    weightName = chipName+".weight";

    processingStatus = new ProcessingStatus(path);
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
        // Create the FITS instance, but do not open and read it yet.
        weightPath = path+"/../WEIGHTS/";
        imageFITS = new MyFITS(path+"/"+name);
        weightFITS = new MyFITS(weightPath+"/"+weightName+".fits");
        // setParent() didn't work elsewhere (?)
        imageFITS->setParent(this);
        weightFITS->setParent(this);
        allocatedImageFITS = true;
        connect(imageFITS, &MyFITS::messageAvailable, this, &MyImage::messageAvailableReceived);
        connect(weightFITS, &MyFITS::messageAvailable, this, &MyImage::messageAvailableReceived);
    }

    verbosity = verbose;

    // When creating a master calibrator, we must also instantiate the MyFITS member.
    // On systems with little RAM, MyFITS must exist so that the data can be read from the FITS file on the drive that was created wehn computing the master calibrator.
    // Otherwise, MyFITS is a nullpointer for MyImage::readImage().
    if (!allocatedImageFITS) {
        imageFITS = new MyFITS(path+"/"+name);
        imageFITS->setParent(this);
        allocatedImageFITS = true;
        connect(imageFITS, &MyFITS::messageAvailable, this, &MyImage::messageAvailableReceived);
    }

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

MyImage::MyImage(QString fullPathName, int *verbose, QObject *parent) : QObject(parent)
{
    QFileInfo fi(fullPathName);

    path = fi.absolutePath();
    name = fi.fileName();
    chipNumber = 1;
    isMasked = false;
    baseName = fi.completeBaseName();
    makeBackup = false;
    weightName = baseName+".weight";

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
        imageFITS = new MyFITS(path+"/"+name);
        weightFITS = new MyFITS(weightPath+"/"+weightName+".fits");
        //        imageFITS->setParent(this);
        //        weightFITS->setParent(this);
        allocatedImageFITS = true;
        connect(imageFITS, &MyFITS::messageAvailable, this, &MyImage::messageAvailableReceived);
        connect(weightFITS, &MyFITS::messageAvailable, this, &MyImage::messageAvailableReceived);
    }

    verbosity = verbose;

    // When creating a master calibrator, we must also instantiate the MyFITS member.
    // On systems with little RAM, MyFITS must exist so that the data can be read from the FITS file on the drive that was created wehn computing the master calibrator.
    // Otherwise, MyFITS is a nullpointer for MyImage::readImage().
    if (!allocatedImageFITS) {
        imageFITS = new MyFITS(path+"/"+name);
        imageFITS->setParent(this);
        allocatedImageFITS = true;
        connect(imageFITS, &MyFITS::messageAvailable, this, &MyImage::messageAvailableReceived);
    }

    wcs = new wcsprm();
    wcsInit = true;

    omp_init_lock(&backgroundLock);
    omp_init_lock(&objectLock);
}

MyImage::~MyImage()
{
    if (allocatedImageFITS) delete imageFITS;
    if (wcsInit) wcsfree(wcs);
    if (wcsInit) delete wcs;  // valgrind does not like that

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

// UNUSED
// When creating a master calibrator, we must also instantiate the MyFITS member.
// On systems with little RAM, MyFITS must exist so that the data can be read from the FITS file on the drive that was created wehn computing the master calibrator.
// Otherwise, MyFITS is a nullpointer for MyImage::readImage().
void MyImage::createMyFITSinstance()
{
    if (allocatedImageFITS) return;

    imageFITS = new MyFITS(path+"/"+name);
    imageFITS->setParent(this);
    allocatedImageFITS = true;
    connect(imageFITS, &MyFITS::messageAvailable, this, &MyImage::messageAvailableReceived);
}

void MyImage::updateProcInfo(QString text)
{
    if (procInfo.isEmpty()) procInfo.append(text);
    else procInfo.append(", "+text);
}

void MyImage::showProcInfo()
{
    if (*verbosity > 1) emit messageAvailable(baseName + " : " + procInfo, "image");
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
        // TODO: delete the following. This should never happen
        if (imageFITS == nullptr) {
            emit messageAvailable("MyImage::readImage(): " + baseName + " : imageFITS member is nullptr", "error");
            emit critical();
            successProcessing = false;
            return;
        }
        // Attempt to read image. Order of calls is important!
        if (imageFITS->loadData()) {
            transferDataToMyImage();
            transferMetadataToMyImage();
            transferWCS();
            cornersToRaDec();
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

// When having to read from a backup file right after launch (task repeated)
void MyImage::readImageBackupL1Launch()
{
    dataCurrent_deletable = false;
    dataWeight_deletable = false;
    dataBackupL1_deletable = false;

    bool determineMode = true;

    // Attempt to read image. Order of calls is important!
    imageFITS->name = pathBackupL1 + "/" + baseNameBackupL1 + ".fits";
    if (imageFITS->loadData()) {
        transferDataToMyImage();
        transferMetadataToMyImage();
        transferWCS();
        cornersToRaDec();
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

// when MyImage has been read before and all members are setup correctly
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
            dataCurrent[i] = buffer[i];
        }
        delete [] buffer;
        backupL1InMemory = true;
        dataCurrent = dataBackupL1;     // probably unnecessary, as we operate on databackupL1, updating dataCurrent
        imageInMemory = true;
    }
}

void MyImage::pushNameToFITS()
{
    imageFITS->name = path+"/"+baseName+".fits";
}

// needed if we need access to header information, but not (yet) the data block,
// e.g. if the GUI is started, and the first task is to download the reference catalog,
// or if we update the zero-th order solution
void MyImage::provideHeaderInfo()
{
    /*
    QFile file(name);
    if (!file.exists()) {
        qDebug() << "ERROR: MyImage::provideHeaderInfo(): " + name + " does not exist!";
        successProcessing = false;
        return;
    }
    */
    if (!headerInfoProvided) {
        if (!imageFITS->provideHeaderInfo()) {
            emit messageAvailable(baseName + " : provideHeaderInfo(): ERROR getting FITS header data", "error");
            emit critical();
            return;
        }
        transferMetadataToMyImage();
        transferWCS();
        cornersToRaDec();
        if (*verbosity > 2) emit messageAvailable(baseName + " : Image header loaded", "image");
    }
    else {
        if (*verbosity > 2) emit messageAvailable(baseName + " : Header already in memory", "image");
    }
    headerInfoProvided = true;
}

void MyImage::transferMetadataToMyImage()
{
    // IF THERE ARE PROBLEMS WITH CORRUPTED HEADERS, REMOVE THE IF CONDITION HERE TO TEST
    if (!headerInfoProvided) header = imageFITS->header;
    //    header = imageFITS->header;
    filter = imageFITS->filter.simplified();
    mjdobs = imageFITS->mjdobs;
    naxis1 = imageFITS->naxis1;
    naxis2 = imageFITS->naxis2;
    bitpix = imageFITS->bitpix;
    crval1 = imageFITS->crval1;
    crval2 = imageFITS->crval2;
    exptime = imageFITS->exptime;
    airmass = imageFITS->airmass;
    fwhm = imageFITS->fwhm;
    ellipticity = imageFITS->ellipticity;
    RZP = imageFITS->RZP;
    dateobs = imageFITS->dateobs;
    plateScale = imageFITS->plateScale;
    gainNormalization = imageFITS->gainNormalization;
    hasMJDread = imageFITS->hasMJDread;
    /*
    if (processingStatus->statusString.isEmpty()) {
        // We are reading a split image. Store its size (before overscan trimming)
        naxis1_split = naxis1;
        naxis2_split = naxis2;
    }
    */
    dim = naxis1*naxis2;
    myWCS = imageFITS->myWCS;
    skyValue = imageFITS->skyValue;
    if (skyValue != -1e9) modeDetermined = true;
    else modeDetermined = false;

    metadataTransferred = true;
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

    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = in.readLine().simplified();
        if (line.isEmpty()) continue;
        QStringList list = line.split(" ");
        if (list.length() < 3) continue;
        if (line.contains("CRVAL1")) astromCRVAL1 = list[2].toDouble();
        if (line.contains("CRVAL2")) astromCRVAL2 = list[2].toDouble();
        if (line.contains("CRPIX1")) astromCRPIX1 = list[2].toFloat();
        if (line.contains("CRPIX2")) astromCRPIX2 = list[2].toFloat();
        if (line.contains("CD1_1")) astromCD11 = list[2].toDouble();
        if (line.contains("CD1_2")) astromCD12 = list[2].toDouble();
        if (line.contains("CD2_1")) astromCD21 = list[2].toDouble();
        if (line.contains("CD2_2")) astromCD22 = list[2].toDouble();
        if (line.contains("FLXSCALE")) FLXSCALE = list[2].toFloat();
        if (line.contains("RZP")) RZP = list[2].toFloat();
    }
    file.close();
    return true;
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

// Setup the WCS
void MyImage::transferWCS()
{
    if (!successProcessing) return;

    emit setWCSLock(true);
    fullheader = imageFITS->fullheader;
    int nreject;
    int nwcs;
    //    struct wcsprm *wcstmp = nullptr;
    int check = wcspih(fullheader, imageFITS->numHeaderKeys, 0, 0, &nreject, &nwcs, &wcs);
    if (check > 1) {
        emit messageAvailable("MyImage::transferWCS(): " + baseName + ": wcspih() returned" + QString::number(check), "error" );
        emit critical();
        return;
    }
    (void) wcsset(wcs);
    wcsInit = true;
    if (*verbosity > 2) {
        emit messageAvailable(chipName + " : RA / DEC = "
                              + QString::number(wcs->crval[0], 'f', 6) + " "
                + QString::number(wcs->crval[1], 'f', 6), "image");
    }
    emit setWCSLock(false);
}

// Takes the long fullheaderString and converts it to a QVector
void MyImage::mapHeader(QString fullheaderString)
{
    if (!successProcessing) return;

    int cardLength = 80;
    long length = fullheaderString.length();
    if (length<80) return;
    for (long i=0; i<=length-cardLength; i+=cardLength) {
        QStringRef subString(&fullheaderString, i, cardLength);
        QString string = subString.toString();
        if (string.contains("SIMPLE  =")
                || string.contains("BITPIX  =")
                || string.contains("NAXIS   =")
                || string.contains("NAXIS1  =")
                || string.contains("NAXIS2  =")
                || string.contains("EXTEND  =")
                || string.contains("END      ")) continue;
        header.push_back(subString.toString());
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

void MyImage::getMode(bool determineMode)
{
    if (!successProcessing) return;

    // Get the mode only if requested, and measure it only if it hasn't been measured already
    // (in which case it is available as the SKYVALUE header keyword)
    if (determineMode && !modeDetermined) {
        skyValue = modeMask(dataCurrent, "stable", globalMask)[0];
        modeDetermined = true;
        QString skyvalue = "SKYVALUE= "+QString::number(skyValue);
        if (*verbosity > 1) emit messageAvailable(baseName + " : " + skyvalue, "image");
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

// Potentially not safe with Controller::displayTotalMemoryUsed(); might require lock (or deactivate memory progress bar)
// Take the data array in MyFITS->data and move it over to MyImage
void MyImage::transferDataToMyImage()
{
    if (!successProcessing) return;

    // Transfer MyFITS->data to MyImage->data
    dataCurrent.swap(imageFITS->data);
    imageFITS->data.clear();
    imageFITS->data.squeeze();
}

// Take the data array in MyFITS->data and move it over to MyImage
void MyImage::transferDataToMyWeight()
{
    if (!successProcessing) return;

    // Transfer MyFITS->data to MyImage->data
    dataWeight.swap(weightFITS->data);
    weightFITS->data.clear();
    weightFITS->data.squeeze();
}

/*
void MyImage::resetDataCurrent(QString statusBackup)
{
    if (!successProcessing) return;

    // Return if image isn't yet in memory (will crash otherwise).
    // This function is only called after MyImage->readImage(), hence the image should always be in memory
    if (!imageInMemory) {
        qDebug() << "QDEBUG: MyImage::resetDataCurrent(): Image not loaded. This should never happen!";
        return;
    }

    // Reset the status to backup status
    processingStatus->statusString = statusBackupL1;

    // Copy level 1 backup data into dataCurrent
    if (processingStatus->statusString.isEmpty()) {
        if (naxis1_split == 0) {
            qDebug() << "QDEBUG: MyImage::ERROR: image size for split image not defined. Cannot restore previous state!";
        }
        else {
            naxis1 = naxis1_split;
            naxis2 = naxis2_split;
        }
    }
    dataCurrent = dataBackupL1;
}
*/

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
            if (*verbosity > 1) emit messageAvailable(baseName + " : Mode = " + QString::number(skyValue,'f',3)
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

    if (*verbosity > 1) emit messageAvailable(baseName + " : "+mode, "image");
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
        // NaN pixels slow down sextractor enourmously (and make it fail).
        // we can probably get rid of this once we are done with sextractor
        if (isnan(pixel) || isinf(pixel)) pixel = 0.;
        ++i;
    }

    if (*verbosity > 1) emit messageAvailable(baseName + " : Flat fielded", "image");
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
            if (backgroundImage->dataCurrent[i] != 0) {
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
        if (*verbosity > 1) emit messageAvailable(baseName + " : Background model subtracted;"+img+back+fac, "image");
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
        if (*verbosity > 1) emit messageAvailable(baseName + " : Divided by normalized background model;"+img+back, "image");
    }
    successProcessing = true;
}

// Normalize Flat to one. Gain differences are corrected separately.
void MyImage::normalizeFlat()
{
    if (!successProcessing) return;

    if (*verbosity > 1) emit messageAvailable(baseName + " : Normalizing flat field, dividing by "+QString::number(skyValue, 'f', 3), "image");
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
        if (*verbosity > 1) emit messageAvailable(baseName + " : External illumination correction : <br>" + illumcorrPath+illumcorrFileName, "image");
        MyImage *illumCorrFlat = new MyImage(illumcorrPath, illumcorrFileName, "", chip+1, QVector<bool>(), false, verbosity);
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

    if (*verbosity > 1) emit messageAvailable(baseName + " : Collapse correction along " + direction, "image");

    if (direction == "x") {
        static_cast<void> (collapse_x(dataCurrent, globalMask, objectMask, threshold.toFloat(), naxis1, naxis2, "2Dsubtract"));
    }
    else if (direction == "y") {
        static_cast<void> (collapse_y(dataCurrent, globalMask, objectMask, threshold.toFloat(), naxis1, naxis2, "2Dsubtract"));
    }
    else {
        static_cast<void> (collapse_quad(dataCurrent, globalMask, objectMask, threshold.toFloat(), naxis1, naxis2, direction, "2Dsubtract"));
    }
}

QVector<float> MyImage::extractPixelValues(long xmin, long xmax, long ymin, long ymax)
{
    long nsub = xmax - xmin + 1;
    long msub = ymax - ymin + 1;

    QVector<float> section;
    section.reserve(nsub*msub);

    for (long j=ymin; j<=ymax; ++j) {
        for (long i=xmin; i<=xmax; ++i) {
            section.append(dataCurrent[i+naxis1*j]);
        }
    }

    return section;
}

void MyImage::makeCutout(long xmin, long xmax, long ymin, long ymax)
{
    long nsub = xmax - xmin + 1;
    long msub = ymax - ymin + 1;

    QVector<float> dataCut;

    dataCut.reserve(nsub*msub);
    for (long j=ymin; j<=ymax; ++j) {
        for (long i=xmin; i<=xmax; ++i) {
            dataCut.append(dataCurrent[i+naxis1*j]);
        }
    }

    naxis1 = nsub;
    naxis2 = msub;
    myWCS.crpix1 = myWCS.crpix1 - xmin + 1;
    myWCS.crpix2 = myWCS.crpix2 - ymin + 1;
    wcs->crpix[0] = myWCS.crpix1;
    wcs->crpix[1] = myWCS.crpix2;

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

void MyImage::writeImage(QString fileName, QString filter, float exptime, bool addGain)
{
    if (!successProcessing) return;

    if (fileName.isEmpty()) {
        // Dump dataCurrent to a FITS file at the default location with the latest status string;
        fileName = path+"/"+chipName+processingStatus->statusString+".fits";
    }
    MyFITS out(fileName, naxis1, naxis2, dataCurrent);
    if (addGain) {
        out.addGainNormalization = true;
        out.gainNormalization = gainNormalization;
    }

    QString history = "";
    bool success = out.write(history, exptime, filter, header);
    if (success) {
        imageOnDrive = true;
        successProcessing = true;
        if (*verbosity > 1) emit messageAvailable(baseName + " : Written to drive.", "image");
    }
    else {
        imageOnDrive = false;
        successProcessing = false;
        emit messageAvailable("MyImage::writeImage(): " + baseName + " : Could not write file to drive!", "error");
        emit critical();
        return;
    }
    emit modelUpdateNeeded(baseName, chipName);
}

void MyImage::writeConstSkyImage(float constValue, QString filter, float exptime, bool addGain)
{
    if (!successProcessing) return;

    mkAbsDir(path+"/SKY_IMAGES");
    QString fileName = path+"/SKY_IMAGES/"+baseName+".sky.fits";
    MyFITS out(fileName, naxis1, naxis2, constValue);
    if (addGain) {
        out.addGainNormalization = true;
        out.gainNormalization = gainNormalization;
    }

    QString history = "";
    bool success = out.writeConstImage(history, exptime, filter, header);
    if (success) {
        if (*verbosity > 1) emit messageAvailable(baseName + " : Sky image written to drive.", "image");
    }
    else {
        emit messageAvailable("MyImage::writeConstImage(): " + baseName + " : Could not write sky image to drive!", "error");
        emit critical();
        return;
    }
}

void MyImage::writeImageDebayer(bool addGain)
{
    if (!successProcessing) return;

    QString fileName = path+"/"+chipName+processingStatus->statusString+".fits";

    MyFITS out(fileName, naxis1, naxis2, dataCurrent);
    if (addGain) {
        out.addGainNormalization = true;
        out.gainNormalization = gainNormalization;
    }

    QString history = "";
    bool success = out.writeDebayer(history, exptime, filter, mjdobs, header);
    if (success) {
        imageOnDrive = true;
        successProcessing = true;
        if (*verbosity > 1) emit messageAvailable(baseName + " : Written to drive.", "image");
    }
    else {
        imageOnDrive = false;
        successProcessing = false;
        emit messageAvailable("MyImage::writeImageDebayer(): " + baseName + " : Could not write file to drive!", "error");
        emit critical();
        return;
    }
    emit modelUpdateNeeded(baseName, chipName);
}

// This routine is used when reading an external image
// (and possibly its existing weight in the same directory, such as a pair of coadd.fits and coadd.weight.fits)
void MyImage::setupCoaddMode()
{
    readImage();

    // Setup an empty dummy globalMask
    globalMask = QVector<bool>();
    globalMaskAvailable = false;

    // Load a matching weight image, if one exists
    weightName.append(".fits");

    QFile file(path+"/"+weightName);
    if (!file.exists()) return;

    MyImage *myWeight = new MyImage(path, weightName, "", 1, QVector<bool>(), false, verbosity, false);
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
    fits_open_file(&fptr, (path+"/"+name).toUtf8().data(), READWRITE, &status);
    fits_read_key_str(fptr, "ZEROHEAD", zerohead, nullptr, &status);
    if (status > 0) {
        // Add the key if it doesn't exist
        // Not used for anything, just informative
        fits_update_key_str(fptr, "ZEROHEAD", "NO", "Astrometric header update", &status); // reset the update flag
        zeroheadString = "NO";
        status = 0;
    }
    else zeroheadString.fromLatin1(zerohead);

    // TODO: sometimes we crash here!!
    fits_update_key_dbl(fptr, "CRVAL1", astromCRVAL1, 9, nullptr, &status);
    fits_update_key_dbl(fptr, "CRVAL2", astromCRVAL2, 9, nullptr, &status);
    fits_update_key_flt(fptr, "CRPIX1", astromCRPIX1, 3, nullptr, &status);
    fits_update_key_flt(fptr, "CRPIX2", astromCRPIX2, 3, nullptr, &status);
    fits_update_key_dbl(fptr, "CD1_1", astromCD11, 9, nullptr, &status);
    fits_update_key_dbl(fptr, "CD1_2", astromCD12, 9, nullptr, &status);
    fits_update_key_dbl(fptr, "CD2_1", astromCD21, 9, nullptr, &status);
    fits_update_key_dbl(fptr, "CD2_2", astromCD22, 9, nullptr, &status);
    if (isnan(RZP)) {
        RZP = 0.;
        FLXSCALE = 0.;
        emit messageAvailable(baseName + " : Scamp could not determine the relative zeropoint. Set to 0!", "warning");
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

void MyImage::updateZeroOrderInMemory()
{
    if (!successProcessing) return;

    // Update the data in memory
    // TODO: if no more issues with wcslib, we can remove all use of myWCS
    myWCS.crval1 = astromCRVAL1;
    myWCS.crval2 = astromCRVAL2;
    myWCS.crpix1 = astromCRPIX1;
    myWCS.crpix2 = astromCRPIX2;
    myWCS.cd1_1 = astromCD11;
    myWCS.cd1_2 = astromCD12;
    myWCS.cd2_1 = astromCD21;
    myWCS.cd2_2 = astromCD22;

    // WCSLIB threading issue (probably solved by wcsLock in transferWCS()
    // Can be removed if we don't see this again
    if (wcs->naxis != 2) {
        emit messageAvailable(baseName + " : MyImage::updateZeroOrder(): Incompatible NAXIS WCS dimension: "
                              + QString::number(wcs->naxis) + ", attempting reload (actually, this is a bug!)", "warning");
        imageInMemory = false;
        readImage(false);
        if (wcs->naxis  != 2) {
            emit messageAvailable(baseName + " : Reload failed!", "error");
            return;
        }
        else {
            emit messageAvailable(baseName + " : Reload success!", "note");
        }
    }

    // TODO: could be moved into scanAstromheader()
    wcs->crval[0] = astromCRVAL1;
    wcs->crval[1] = astromCRVAL2;
    wcs->crpix[0] = astromCRPIX1;
    wcs->crpix[1] = astromCRPIX2;
    wcs->cd[0] = astromCD11;
    wcs->cd[1] = astromCD12;
    wcs->cd[2] = astromCD21;
    wcs->cd[3] = astromCD22;

    cornersToRaDec();
}

void MyImage::updateCRVALinHeaderOnDrive()
{
    // Must write file to drive (scamp reads the header information) if it does not exist yet
    QFile file(path+ "/" + baseName + ".fits");
    if (!file.exists()) writeImage(path+ "/" + baseName + ".fits");

    int status = 0;
    fitsfile *fptr = nullptr;
    fits_open_file(&fptr, (path+"/"+name).toUtf8().data(), READWRITE, &status);
    fits_update_key_dbl(fptr, "CRVAL1", crval1, 6, nullptr, &status);
    fits_update_key_dbl(fptr, "CRVAL2", crval2, 6, nullptr, &status);
    fits_close_file(fptr, &status);
    printCfitsioError("updateCRVALinHeaderOnDrive()", status);
}

void MyImage::updateCRVALCDinHeaderOnDrive()
{
    // Must write file to disk (scamp reads the header information) if it does not exist yet
    QFile file(path+ "/" + baseName + ".fits");
    if (!file.exists()) writeImage(path+ "/" + baseName + ".fits");

    int status = 0;
    fitsfile *fptr = nullptr;
    fits_open_file(&fptr, (path+"/"+name).toUtf8().data(), READWRITE, &status);
    fits_update_key_dbl(fptr, "CRVAL1", crval1, 6, nullptr, &status);
    fits_update_key_dbl(fptr, "CRVAL2", crval2, 6, nullptr, &status);
    fits_update_key_flt(fptr, "CD1_1", myWCS.cd1_1, 6, nullptr, &status);
    fits_update_key_flt(fptr, "CD1_2", myWCS.cd1_2, 6, nullptr, &status);
    fits_update_key_flt(fptr, "CD2_1", myWCS.cd2_1, 6, nullptr, &status);
    fits_update_key_flt(fptr, "CD2_2", myWCS.cd2_2, 6, nullptr, &status);
    fits_close_file(fptr, &status);
    printCfitsioError("updateCRVALCDinHeaderOnDrive()", status);
}

void MyImage::updateInactivePath()
{
    if (activeState == MyImage::ACTIVE) pathExtension = "";
    else if (activeState == MyImage::INACTIVE) pathExtension = "/inactive/";
    else if (activeState == MyImage::BADSTATS) pathExtension = "/inactive/badStatistics/";
    else if (activeState == MyImage::BADBACK) pathExtension = "/inactive/badBackground/";
    else if (activeState == MyImage::DELETED) pathExtension = "/inactive/";
}

void MyImage::applyMask()
{
    // Leave if the chip has no mask (save some CPU cycles)
    if (!isMasked) return;

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

    if (!hasWCS) myWCS = imageFITS->loadWCS();
    if (myWCS.naxis1 == 0) {
        emit messageAvailable("MyImage::checkBrightStars(): " + baseName + " : Could not load WCS!", "error");
        emit critical();
        successProcessing = false;
        return;
    }

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

// TODO: check against wcslib whether valid or not
void MyImage::cornersToRaDec()
{
    if (myWCS.isValid) {
        // Convert the cartesian image vertices to RA/DEC
        /*
        myWCS.xy2sky(1, 1, alpha_ll, delta_ll);
        myWCS.xy2sky(naxis1, 1, alpha_lr, delta_lr);
        myWCS.xy2sky(1, naxis2, alpha_ul, delta_ul);
        myWCS.xy2sky(naxis1, naxis2, alpha_ur, delta_ur);
        */
        // using wcslib
        //        QTest::qWait(100);
        // without the qwait(), sometimes wcslib stumbles and does not calculate the values correctly in multi-threaded mode.
        // correct way would be a global omp_lock, but i could not get that initialized correctly
        // Update: setting wcsLock in transferWCS()
        xy2sky(1, 1, alpha_ll, delta_ll);
        xy2sky(naxis1, 1, alpha_lr, delta_lr);
        xy2sky(1, naxis2, alpha_ul, delta_ul);
        xy2sky(naxis1, naxis2, alpha_ur, delta_ur);
        xy2sky(naxis1/2, naxis2/2, alpha_ctr, delta_ctr);
        //        qDebug() << baseName << alpha_ctr << delta_ctr << alpha_ul << alpha_ll << delta_ul << delta_ll;
    }
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

    // Move the image accordingly
    QString currentPath = path;                      // The path where the image is currently located (if on disk)
    updateInactivePath();                            // Update the inactive path component according to the set state
    QString newPath = currentPath + pathExtension;   // The path where the image should go
    if (!imageOnDrive) return;
    moveFile(baseName+".fits", currentPath, newPath);
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
