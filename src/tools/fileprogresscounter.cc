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

