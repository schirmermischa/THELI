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

#ifndef MEMORYWORKER_H
#define MEMORYWORKER_H

#include "worker.h"
#include "../processingInternal/data.h"
#include "../dockwidgets/memoryviewer.h"

class MemoryViewer;

class MemoryWorker : public Worker
{
    Q_OBJECT

public:
    explicit MemoryWorker(MemoryViewer *memViewer, QObject *parent = 0);
    explicit MemoryWorker(Data *datadir, QObject *parent = 0);
    MyImage *myImage = nullptr;
    QString path = "";

    Data *data = nullptr;

public slots:
    void MemoryViewerDumpImagesToDrive();
    void DataDumpImagesToDrive();
    void dumpImageToDrive();
//    void processActiveStatusChanged();

private:
    MemoryViewer *memoryViewer = nullptr;
//    Data *data;

    omp_lock_t progressLock;
    float progress = 0.;
    float progressStepSize = 0.;

    void incrementProgress();

signals:
    void progressUpdate(float progress);
    void resetProgressBar();

};

#endif // MEMORYWORKER_H
