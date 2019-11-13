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
    data->numImages = 0;

    pushBeginMessage(taskBasename, dataDir);

    // Obtain a list of files in this directory, including non-FITS files
    QString path = mainDirName + "/" + dataDir;
    QDir dir(path);
    QStringList filter;
    filter << "*.*";
    QStringList files = dir.entryList(filter, QDir::Files);
    numActiveImages = files.length();
    progressStepSize = 100./(float(numActiveImages) * instData->numChips);
    progress = 0.;

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

    // Loop over all chips
#pragma omp parallel for num_threads(maxCPU) firstprivate(mainDirName, dataDir, dummyKeys, nonlinearityCoefficients, headerDictionary, filterDictionary, dataType)
    for (int i=0; i<numActiveImages; ++i) {
        if (!successProcessing) continue;
        if (userStop || userKill || abortProcess) continue;  // Only place we do it this way, because Data class is not yet instantiated
        // Setup the splitter class for the current file
        QString fileName = files.at(i);
        emit messageAvailable(fileName + " : HDU reformatting, low-level pixel processing ...", "image");
        Splitter *splitter = new Splitter(*instData, mask, data, dataType, cdw, mainDirName, dataDir, fileName, &verbosity);
        splitter->headerDictionary = headerDictionary;
        splitter->filterDictionary = filterDictionary;
        splitter->dummyKeys = dummyKeys;
        splitter->combineOverscan_ptr = combineOverscan_ptr;       // set by MainWindow::updateControllerFunctors()
        splitter->nonlinearityCoefficients = nonlinearityCoefficients;
        splitter->progressStepSize = progressStepSize;
        splitter->progressLock = &progressLock;
        splitter->genericLock = &genericLock;
        splitter->progress = &progress;
        connect(splitter, &Splitter::messageAvailable, this, &Controller::messageAvailableReceived);
        connect(splitter, &Splitter::critical, this, &Controller::criticalReceived);
        connect(splitter, &Splitter::warning, this, &Controller::warningReceived);
        connect(splitter, &Splitter::showMessageBox, this, &Controller::showMessageBoxReceived);

        // Extract images. This also handles all low-level pixel processing.
        splitter->determineFileFormat();
        splitter->extractImages();
        if (!splitter->successProcessing) successProcessing = false;
        // splitter handles the progress counter
        // No memory management needed, splitter simply runs out of scope
    }

    checkSuccessProcessing(data);

    if (successProcessing) {
        uniformMJDOBS(dir);              // rename file and update MJDOBS for a few specific instruments, only
        if (!updateDataDirs(data)) {     // Some instruments need to split the original data dir into several more, e.g. GROND and LIRIS_POL
            finalizeSplitter(data);
        }
        emit progressUpdate(100);
    }
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
    //        emit populateMemoryView();
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
        QList<Data*> *DT_x;
        if (data->dataType == "BIAS") DT_x = &DT_BIAS;
        else if (data->dataType == "DARK") DT_x = &DT_DARK;
        else if (data->dataType == "FLATOFF") DT_x = &DT_FLATOFF;
        else if (data->dataType == "FLAT") DT_x = &DT_FLAT;
        else if (data->dataType == "SCIENCE") DT_x = &DT_SCIENCE;
        else if (data->dataType == "SKY") DT_x = &DT_SKY;
        else if (data->dataType == "STD") DT_x = &DT_STANDARD;
        else {
            // nothing
        }
        updateMasterList();
        emit populateMemoryView();
        omp_unset_lock(&memoryLock);
        dataTreeUpdateOngoing = false;
        for (auto &data : *DT_x) {
            finalizeSplitter(data);
        }

        updateDataDirDone = true;
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
