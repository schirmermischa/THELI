#include "controller.h"
#include "../mainwindow.h"
#include "../tools/tools.h"
#include "../tools/splitter.h"
#include "ui_confdockwidget.h"

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
        emit messageAvailable(fileName + " : Splitting and low level processing ...", "image");
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
        emit progressUpdate(100);
    }
}
