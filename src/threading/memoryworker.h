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
    MyImage *myImage;
    QString path = "";

    Data *data;

public slots:
    void MemoryViewerDumpImagesToDrive();
    void DataDumpImagesToDrive();
    void dumpImageToDrive();
//    void processActiveStatusChanged();

private:
    MemoryViewer *memoryViewer;
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
