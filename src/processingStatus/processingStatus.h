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

#ifndef PROCESSINGSTATUS_H
#define PROCESSINGSTATUS_H

#include <QObject>
#include <QDir>
#include <QString>

class ProcessingStatus : public QObject
{
    Q_OBJECT
public:
    explicit ProcessingStatus(QString dirName, QObject *parent = nullptr);

    bool HDUreformat = false;
    bool Processscience = false;
    bool Chopnod = false;
    bool Background = false;
    bool Collapse = false;
    bool Starflat = false;
    bool Skysub = false;

    QString statusString = "";

    void writeToDrive();       // Used by Data class only
    void readFromDrive();      // Used by Data class only
    void reset();
    void statusToBoolean(QString status);
    QString getStatusString();
    void inferStatusFromFilenames();
    bool doesStatusFileExist();
    void deleteFromDrive();
    QString whatIsStatusOnDrive();

private:
    QDir dir;
    QString dirName = "";

    QString extractStatusFromFilename(QString &filename);

signals:
    void processingStatusChanged();
    void messageAvailable(QString messageString, QString code);
    void critical();

public slots:
};

#endif // PROCESSINGSTATUS_H
