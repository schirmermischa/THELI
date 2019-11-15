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

#include "memoryworker.h"
#include "functions.h"
#include "../dockwidgets/memoryviewer.h"
#include "../processingInternal/data.h"
#include "ui_memoryviewer.h"
#include "../myimage/myimage.h"

#include <omp.h>
#include <QObject>
#include <QTest>

MemoryWorker::MemoryWorker(MemoryViewer *memViewer, QObject *parent) : Worker(parent)
{
    memoryViewer = memViewer;
}

MemoryWorker::MemoryWorker(Data *datadir, QObject *parent) : Worker(parent)
{
    data = datadir;
}

/*
 *  Done internally to MyImage by calling setActiveStatus()
void MemoryWorker::processActiveStatusChanged()
{
    if (state == PAUSED) state = RUNNING;  // Treat as resume
    if (state == RUNNING) return;

    state = RUNNING;
    emit started();

    // Previously active image is now inactive
    if (myImage->oldState == MyImage::ACTIVE && myImage->activeState == MyImage::INACTIVE) {
        moveFile(myImage->baseName+".fits", path, path+"/inactive/");
    }
    // Restore inactive images
    QString inactivePath = path+"/inactive/";
    if (myImage->activeState == MyImage::BADSTATS) inactivePath.append("/badStatistics/");
    if (myImage->activeState == MyImage::BADBACK) inactivePath.append("/badBackground/");
    if (myImage->activeState != MyImage::ACTIVE
            && myImage->activeState != MyImage::DELETED) {
        moveFile(myImage->baseName+".fits", inactivePath, path);
    }

    emit finished();
}
*/

void MemoryWorker::MemoryViewerDumpImagesToDrive()
{
    emit resetProgressBar();

    if (state == PAUSED) state = RUNNING;  // Treat as resume
    if (state == RUNNING) return;

    state = RUNNING;
    emit started();

    memoryViewer->ui->downloadToolButton->setText("Wait ");
    memoryViewer->ui->downloadToolButton->setDisabled(true);
    QTest::qWait(50);

    int index = memoryViewer->ui->datadirComboBox->currentIndex();
    if (index == -1) return;

    int numImages = memoryViewer->dataModelList[index]->imageList.length();

    progressStepSize = 1./float(numImages);

#pragma omp parallel for num_threads(memoryViewer->controller->maxExternalThreads)
    for (int i=0; i<numImages; ++i) {
        MyImage *it = memoryViewer->dataModelList[index]->imageList[i];
        if (!it->imageOnDrive) {
            it->writeImage();
            it->imageOnDrive = true;
            it->emitModelUpdateNeeded();
            incrementProgress();
            emit progressUpdate(progress);
        }
    }
    memoryViewer->ui->downloadToolButton->setText("Write");
    memoryViewer->ui->downloadToolButton->setEnabled(true);

    emit progressUpdate(100.);

    emit finished();
}


void MemoryWorker::DataDumpImagesToDrive()
{
    emit resetProgressBar();

    if (state == PAUSED) state = RUNNING;  // Treat as resume
    if (state == RUNNING) return;

    state = RUNNING;
    emit started();

//    memoryViewer->ui->downloadToolButton->setText("Wait ");
//    memoryViewer->ui->downloadToolButton->setDisabled(true);
    QTest::qWait(50);

    progressStepSize = 1. / float(data->numImages);

    emit progressUpdate(0.);
    data->populateExposureList();

#pragma omp parallel for num_threads(data->maxExternalThreads)
    for (long i=0; i<data->exposureList.length(); ++i) {
        for (auto &it : data->exposureList[i]) {
            if (!it->imageOnDrive) {
                it->writeImage();
                it->imageOnDrive = true;
                it->emitModelUpdateNeeded();
                incrementProgress();
                emit progressUpdate(progress);
            }
        }
    }
//    memoryViewer->ui->downloadToolButton->setText("Write");
//    memoryViewer->ui->downloadToolButton->setEnabled(true);

    emit progressUpdate(100.);

    emit finished();
}

void MemoryWorker::incrementProgress()
{
    omp_set_lock(&progressLock);
    progress += progressStepSize;
    emit progressUpdate(progress);
    omp_unset_lock(&progressLock);
}

void MemoryWorker::dumpImageToDrive()
{
    if (state == PAUSED) state = RUNNING;  // Treat as resume
    if (state == RUNNING) return;

    state = RUNNING;
    emit started();

    myImage->writeImage();
    myImage->imageOnDrive = true;

    emit finished();
}
