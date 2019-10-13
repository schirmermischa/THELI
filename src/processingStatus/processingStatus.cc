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

// If the .status file is absent when initialising the Data class, then infer the status from the FITS file names
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
        emit messageAvailable("ProcessingStatus::inferStatus(): Could not infer unambiguous processing status from the FITS file names in "
                              + dirName + ".<br>The following statuses were identified: "+ joined
                              + "<br>Cleanup the directory manually to establish a consistent status. Restart required.", "error");
        emit critical();
        statusString = "";
    }
}

QString ProcessingStatus::extractStatusFromFilename(QString &filename)
{
    QString id = getLastWord(filename, '_');
    QString status = id.remove(".fits");
    status.remove(QRegExp("[0123456789]"));
    return status;
}



