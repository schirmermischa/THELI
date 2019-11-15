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

#ifndef FILEPROGRESSCOUNTER_H
#define FILEPROGRESSCOUNTER_H

#include <QObject>
#include <QDir>
#include <QTimer>
#include <QFile>

class FileProgressCounter : public QObject
{
    Q_OBJECT
public:
    explicit FileProgressCounter(QString dirname, QString filter, int numtotimages, float *progressDep,
                                 QObject *parent = nullptr);
    explicit FileProgressCounter(QString dirname, long finalsize, float *progressDep, QObject *parent = nullptr);

    QDir dir;
    QString dirName;
    QStringList filterList;
    int numTotImages;
    QTimer *timer;
    QFileInfo fi;
    long finalSize;
    float *progress;

signals:
    void progressUpdate(float progress);

public slots:
    void numberFileProgress();
    void sizeFileProgress();
    void stopFileProgressTimerReceived();

};

#endif // FILEPROGRESSCOUNTER_H
