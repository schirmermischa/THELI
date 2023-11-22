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

#include "processingStatus.h"
#include "../functions.h"

#include <QObject>
#include <QDir>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QTextStream>

ProcessingStatus::ProcessingStatus(QString dirname, QObject *parent) : QObject(parent)
{
    dirName = dirname;
    dir.setPath(dirname);
    if (!dir.exists()) {
        emit messageAvailable("ProcessingStatus::ProcessingStatus(): Directory " + dirName + " does not exist", "error");
        emit critical();
        statusString = "";
        return;
    }
}

void ProcessingStatus::writeToDrive()
{
    QFile file(dirName + "/.processingStatus");
    QTextStream stream(&file);
    if( !file.open(QIODevice::WriteOnly)) {
        emit messageAvailable("ProcessingStatus::write(): Could not write "+dirName + "/.processingStatus "+file.errorString(), "error");
        emit critical();
        return;
    }

    // Write status
    stream << getStatusString() << "\n";
    file.close();
    file.setPermissions(QFile::ReadUser | QFile::WriteUser);
}

void ProcessingStatus::deleteFromDrive()
{
    QFile file(dirName + "/.processingStatus");
    file.remove();
}

void ProcessingStatus::readFromDrive()
{
    QFile file(dirName + "/.processingStatus");

    if (!file.exists()) {
        inferStatusFromFilenames();
        statusToBoolean(statusString);
        return;
    }

    if(!file.open(QIODevice::ReadOnly)) {
        emit messageAvailable("ProcessingStatus::read(): Could not open "+dirName + "/.processingStatus "+file.errorString(), "error");
        emit critical();
        return;
    }

    QTextStream stream(&file);
    statusString = stream.readLine();
    statusToBoolean(statusString);
    file.close();
}

// same as above, just returning the string
QString ProcessingStatus::whatIsStatusOnDrive()
{
    QFile file(dirName + "/.processingStatus");

    if (!file.exists()) {
        inferStatusFromFilenames();
        statusToBoolean(statusString);
        return "";
    }

    if(!file.open(QIODevice::ReadOnly)) {
        emit messageAvailable("ProcessingStatus::read(): Could not open "+dirName + "/.processingStatus "+file.errorString(), "error");
        emit critical();
        return "";
    }

    QTextStream stream(&file);
    QString tmp = stream.readLine();
    file.close();
    return tmp;
}

QString ProcessingStatus::getStatusString()
{
    statusString = "";
    if (HDUreformat) statusString.append("P");
    if (Processscience) statusString.append("A");
    if (Chopnod) statusString.append("M");
    if (Background) statusString.append("B");
    if (Collapse) statusString.append("C");
    if (Starflat) statusString.append("D");
    if (Skysub) statusString.append("S");

    return statusString;
}

void ProcessingStatus::statusToBoolean(QString status)
{
    reset();

    if (status.contains("P")) HDUreformat = true;
    if (status.contains("A")) Processscience = true;
    if (status.contains("M")) Chopnod = true;
    if (status.contains("B")) Background = true;
    if (status.contains("C")) Collapse = true;
    if (status.contains("D")) Starflat = true;
    if (status.contains("S")) Skysub = true;

    // WARNING: This signal is not connected anywhere!
    emit processingStatusChanged();
}

void ProcessingStatus::reset()
{
    HDUreformat = false;
    Processscience = false;
    Chopnod = false;
    Background = false;
    Collapse = false;
    Starflat = false;
    Skysub = false;
}

// If the .processingStatus file is absent when initialising the Data class, then infer the status from the FITS file names
void ProcessingStatus::inferStatusFromFilenames()
{
    statusString = "";
    QStringList statusList;

    QStringList fileList = dir.entryList(QStringList() << "*.fits");
    for (auto &it : fileList) {
        QString status = extractStatusFromFilename(it);
        if (!statusList.contains(status)) statusList.append(status);
    }

    if (statusList.length() == 1) statusString = statusList.at(0);
    else {
        QString joined = "";
        for (auto &it : statusList) {
            joined.append(it+" ");
        }
        joined = joined.simplified();
        QString message = "Could not infer unambiguous processing status from the FITS file names in "
                + dirName + ".<br>The following statuses were identified: "+ joined
                + "<br>Cleanup the directory manually to establish a consistent status. Restart required.";
        qDebug() << message;
        emit messageAvailable(QString(__func__)+message, "error");  // For some reason this signal is never received by the controller, even though 'Data' re-emits it.
        emit critical();
        statusString = "";
    }
}

// This does not work if raw data have e.g. the form fsr_1075_03_c2.fits (FourStar).
// It would return 'c'. Or in case of Gemini, e.g. S20191231S0495.fits, it would return 'SS'
// Hence this function must not be called if no processing has taken place yet.
// UPDATE: This function IS called when initializing the Data class. But inside that initialization, we check for RAW data,
// and if any are found, the status string and internal data are reset.
QString ProcessingStatus::extractStatusFromFilename(QString &filename)
{
    QString id = getLastWord(filename, '_');
    QString status = id.remove(".fits");
    status.remove(QRegExp("[0123456789]"));
    return status;
}

bool ProcessingStatus::doesStatusFileExist()
{
    QFile file(dirName + "/.processingStatus");
    if( !file.exists()) return false;
    else return true;
}


