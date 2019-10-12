#include "controller.h"
#include "../mainwindow.h"
#include "../tools/tools.h"
#include "../tools/fitting.h"
#include "ui_confdockwidget.h"
#include "ui_monitor.h"

#include <QMetaObject>
#include <QVector>
#include <QStringList>
#include <QProgressBar>


void Controller::taskInternalCollapse()
{
    QString scienceDir = instructions.split(" ").at(1);
    Data *scienceData = getData(DT_SCIENCE, scienceDir);
    currentData = scienceData;
    currentDirName = scienceDir;

    pushBeginMessage(taskBasename, scienceDir);

    memoryDecideDeletableStatus(scienceData, false);

    backupDirName = scienceData->processingStatus->getStatusString() + "_IMAGES";

    // Parameters for collapse correction
    QString DT = cdw->ui->COCDTLineEdit->text();
    QString DMIN = cdw->ui->COCDMINLineEdit->text();
    QString expFactor = cdw->ui->COCmefLineEdit->text();
    QString direction = cdw->ui->COCdirectionComboBox->currentText();
    QString threshold = cdw->ui->COCrejectLineEdit->text();

    scienceData->checkTaskRepeatStatus(taskBasename);

    getNumberOfActiveImages(scienceData);

    /*
#pragma omp parallel for num_threads(maxExternalThreads)
    for (int chip=0; chip<instData->numChips; ++chip) {
        incrementCurrentThreads(lock);
        for (auto &it : scienceData->myImageList[chip]) {
            if (verbosity >= 0) emit messageAvailable(it->baseName + " : Collapse correction ...", "controller");
            it->setupDataInMemory(isTaskRepeated, true, true);
            it->backgroundModel(256, "interpolate");
            it->segmentImage(DT, DMIN, true, false);
            it->transferObjectsToMask();
            it->maskExpand(expFactor, false);
            it->collapseCorrection(threshold, direction);
            it->makeMemoryBackup();
            it->statusCurrent = statusNew;
            it->baseName = it->chipName + statusNew;
            it->imageOnDrive = false;
            if (alwaysStoreData) it->writeImage();
            incrementProgress();
        }

        if (scienceData->successProcessing) {
            for (auto &it : scienceData->myImageList[chip]) {
                // it->makeMemoryBackup();
                it->makeDriveBackup(statusOld+"_IMAGES", statusOld);
                scienceData->memorySetDeletable(chip, "dataBackupL1", true);
            }
        }

        decrementCurrentThreads(lock);
    }
*/

    QList<MyImage*> allMyImages;
    long numMyImages = makeListofAllImages(allMyImages, scienceData);

    // Release as much memory as maximally necessary
    float nimg = 7; // old, new, background, segmentation, measure, mask, margin
    releaseMemory(nimg*instData->storage*maxCPU, 1);
    // Protect the rest, will be unprotected as needed
    scienceData->protectMemory();

#pragma omp parallel for num_threads(maxCPU)
    for (int k=0; k<numMyImages; ++k) {
        if (abortProcess || !successProcessing) continue;

        releaseMemory(nimg*instData->storage, maxCPU);

        auto &it = allMyImages[k];
        if (verbosity >= 0) emit messageAvailable(it->chipName + " : Collapse correction ...", "image");
        it->processingStatus->Collapse = false;
        it->setupData(scienceData->isTaskRepeated, true, true, backupDirName);  // CHECK: why do we determine the mode here?
        it->backgroundModel(256, "interpolate");
        it->segmentImage(DT, DMIN, true, false);
        it->transferObjectsToMask();
        it->maskExpand(expFactor, false);
        it->collapseCorrection(threshold, direction);
        it->releaseAllDetectionMemory();
        it->releaseBackgroundMemory("entirely");

        updateImageAndData(it, scienceData);

        if (alwaysStoreData) {
            it->writeImage();
            it->unprotectMemory();
        }
#pragma omp atomic
        progress += progressStepSize;
    }

    /*
    // Just file movements (if on drive), does not require parallelization
    for (int chip=0; chip<instData->numChips; ++chip) {
        if (scienceData->successProcessing) {
            for (auto &it : scienceData->myImageList[chip]) {
                // it->makeMemoryBackup();
                it->makeDriveBackup(statusOld+"_IMAGES", statusOld);
            }
        }
    }
    */

    checkSuccessProcessing(scienceData);
    satisfyMaxMemorySetting();

    if (successProcessing) {
        scienceData->processingStatus->Collapse = true;
        scienceData->processingStatus->writeToDrive();
        scienceData->transferBackupInfo();
        scienceData->emitStatusChanged();
        emit addBackupDirToMemoryviewer(scienceDir, backupDirName);
        emit progressUpdate(100);
        //        pushEndMessage(taskBasename, scienceDir);
    }
}

void Controller::taskInternalBinnedpreview()
{
    QString scienceDir = instructions.split(" ").at(1);
    Data *scienceData = getData(DT_SCIENCE, scienceDir);
    currentData = scienceData;
    currentDirName = scienceDir;

    pushBeginMessage(taskBasename, scienceDir);

    // Collect CD matrices (from first exposure in the list)
    // The transformation matrix is the CD matrix with rotation angle and plate scale stripped
    QVector< QVector<double>> CDmatrices;
    QVector< QVector<int>> Tmatrices;
    for (int chip=0; chip<instData->numChips; ++chip) {
        scienceData->myImageList[chip][0]->readImage(false);
        QVector<double> CDmatrix = scienceData->myImageList[chip][0]->extractCDmatrix();
        CDmatrices << CDmatrix;
        Tmatrices << CDmatrixToTransformationMatrix(CDmatrix);
    }

    // Determine overall image size
    int nGlobal;
    int mGlobal;
    int xminOffset;
    int yminOffset;
    int binFactor = cdw->ui->BIPSpinBox->value();
    getBinnedSize(instData, Tmatrices, nGlobal, mGlobal, binFactor, xminOffset, yminOffset);

    memoryDecideDeletableStatus(scienceData, false);

    int numExp = scienceData->myImageList[0].length();

    getNumberOfActiveImages(scienceData);

    float nimg = (1 + 1./(binFactor*binFactor)) * instData->numChips;
    releaseMemory(nimg*instData->storage*maxCPU, 1);
    scienceData->protectMemory();

    QString statusString = scienceData->processingStatus->statusString;
    // Loop over all exposures
#pragma omp parallel for num_threads(maxCPU) firstprivate(statusString)
    for (int img=0; img<numExp; ++img) {
        if (abortProcess || !successProcessing) continue;

        releaseMemory(nimg*instData->storage, maxCPU);

        // Loop over all chips
        if (verbosity >= 0) emit messageAvailable("Working on chips in exposure " + QString::number(img)
                                                  + " / " + QString::number(numExp), "controller");
        QVector<float> dataBinnedGlobal(nGlobal*mGlobal,0);
        for (int chip=0; chip<instData->numChips; ++chip) {
            if (abortProcess) break;
            scienceData->myImageList[chip][img]->setupDataInMemorySimple(false);
            int n = scienceData->myImageList[chip][img]->naxis1;
            int m = scienceData->myImageList[chip][img]->naxis2;
            int crpix1 = scienceData->myImageList[chip][img]->getKeyword("CRPIX1").toFloat() / binFactor;
            int crpix2 = scienceData->myImageList[chip][img]->getKeyword("CRPIX2").toFloat() / binFactor;
            int nb = floor(n/binFactor);
            int mb = floor(m/binFactor);
            QVector<float> dataBinned(nb*mb,0);
            // Bin the image; map it onto the output binned image;
            if (verbosity >= 1) emit messageAvailable(scienceData->myImageList[chip][img]->chipName + " : Binning ...", "image");
            binData(scienceData->myImageList[chip][img]->dataCurrent, dataBinned, n, m, nb, mb, binFactor, binFactor);
            mapBinnedData(dataBinnedGlobal, dataBinned, Tmatrices[chip], nGlobal, mGlobal,
                          nb, mb, crpix1, crpix2, xminOffset, yminOffset);
            scienceData->myImageList[chip][img]->unprotectMemory();
#pragma omp atomic
            progress += progressStepSize;
        }
        QString outName = scienceData->myImageList[0][img]->rootName;
        outName.append("_"+statusString+".mosaic.fits");
        QString outDirName = scienceData->dirName+"/BINNED/";
        QDir outDir(outDirName);
        if (!outDir.exists()) outDir.mkdir(outDirName);
        QFile out(outDirName+outName);
        if (out.exists()) out.remove();
        MyImage *myBinnedImage = new MyImage(outDirName, outName, "", 1, QVector<bool>(), false, &verbosity);
        connect(myBinnedImage, &MyImage::critical, this, &Controller::criticalReceived);
        connect(myBinnedImage, &MyImage::messageAvailable, this, &Controller::messageAvailableReceived);
        connect(myBinnedImage, &MyImage::warning, this, &Controller::warningReceived);
        connect(myBinnedImage, &MyImage::setMemoryLock, this, &Controller::setMemoryLockReceived, Qt::DirectConnection);
        myBinnedImage->naxis1 = nGlobal;
        myBinnedImage->naxis2 = mGlobal;
        myBinnedImage->dataCurrent.swap(dataBinnedGlobal);
        myBinnedImage->writeImage(outDirName+outName);
        // TODO: preserve for iView; set memory flags accordingly; could create myImageList and construct iView accordingly
        delete myBinnedImage;
    }

    checkSuccessProcessing(scienceData);
    satisfyMaxMemorySetting();

    if (successProcessing) {
        emit progressUpdate(100);
        //        pushEndMessage(taskBasename, scienceDir);
    }
    // Load the plot viewer
    emit loadViewer(scienceData->dirName+"/BINNED/", "*_"+scienceData->processingStatus->statusString+".mosaic.fits", "DragMode");
}
