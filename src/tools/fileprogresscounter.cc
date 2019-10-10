#include "fileprogresscounter.h"

#include <QDir>
#include <QString>
#include <QDebug>

FileProgressCounter::FileProgressCounter(QString dirname, QString filter, int numtotimages,
                                         float *progressDep, QObject *parent) : QObject(parent)
{
    dir.setPath(dirname);
    filterList << filter;
    numTotImages = numtotimages;
    timer = new QTimer(this);
    progress = progressDep;
}

FileProgressCounter::FileProgressCounter(QString dirname, long finalsize,
                                         float *progressDep, QObject *parent) : QObject(parent)
{
    dirName = dirname;
    timer = new QTimer(this);
    finalSize = finalsize;
    progress = progressDep;
}

void FileProgressCounter::numberFileProgress()
{
    int numImages = dir.entryList(filterList).length();
    *progress = 100. * float(numImages) / float(numTotImages);

//    emit progressUpdate(progress);
}

void FileProgressCounter::sizeFileProgress()
{
    fi.setFile(dirName+"/coadd.fits");
    long currentSize = fi.size();
    *progress = 100. * float(currentSize) / float(finalSize);

    //   emit progressUpdate(progress);
}

void FileProgressCounter::stopFileProgressTimerReceived()
{
    timer->stop();
}

