#include "datadir.h"
#include <QDebug>

// Constructor
DataDir::DataDir(QString data, QString main)
{
    setPaths(data, main);
}

DataDir::DataDir()
{
    name = "";
    subdirname = "";
    numFITS = 0;
    numCHIP1 = 0;
}

void DataDir::setPaths(QString data, QString main)
{
    if (data.isEmpty()) {
        qDebug() << "QDEBUG: DataDir: sub-directory cannot be an empty string!";
        return;
    }
    if (!main.isEmpty()) {
        QDir maindir(main);
        if (!maindir.exists()) {
            qDebug() << "QDEBUG: DataDir: main directory does not exist!";
            return;
        }
        if (!maindir.isAbsolute()) {
            qDebug() << "QDEBUG: DataDir: main directory must be absolute!";
            return;
        }
    }

    // If main was not provided but data contains a blank (e.g. MAINDIR SCIENCE)
    if (data.contains(" ")) {
        QStringList list = data.split(" ");
        main = list.at(0);
        data = list.at(1);
    }

    // Concatenate maindir and datadir, collapse multiple slashes etc
    name = QDir::cleanPath(main+"/"+data+"/");
    subdirname = data;
    dir.setPath(name);

    if (dir.exists()) {
        QStringList fileList;

        // How many FITS files do we have in total
        QStringList filterFITS;
        QStringList filterRAW;
        filterFITS << "*.fits" << "*.fit" << "*.FIT" << "*.FITS";
        dir.setNameFilters(filterFITS);
        fileList = dir.entryList();
        numFITS = fileList.length();
        if (numFITS == 0) {
            // Check for RAW files instead
            filterFITS.clear();
            filterFITS << "*.cr2" << "*.CR2";
            dir.setNameFilters(filterFITS);
            fileList = dir.entryList();
            numFITS = fileList.length();
        }

        // How many FITS files do we have of chip 1 (i.e., number of exposures)
        // (needed to create master calibrators)
        QStringList filterCHIP1;
        filterCHIP1 << "*_1P.fits";
        dir.setNameFilters(filterCHIP1);
        fileList = dir.entryList();
        numCHIP1 = fileList.length();
    }
    else {
        numFITS = 0;
        numCHIP1 = 0;
    }
}

bool DataDir::hasMaster()
{
    // This is to check if a BIAS, DARK etc directory already contains a master BIAS, DARK, etc
    QString cleanname = subdirname.remove("/");
    QStringList filter;
    filter << cleanname+"_1.fits";
    dir.setNameFilters(filter);
    QStringList fileList = dir.entryList();
    if (fileList.length() == 1) {
        return true;
    }
    else {
        return false;
    }
}

bool DataDir::isEmpty()
{
    if (numFITS == 0) {
        return true;
    }
    else {
        return false;
    }
}

bool DataDir::exists()
{
    return dir.exists();
}

long DataDir::numEXT(QString type)
{
    // How many FITS files of a certain type do we have of chip 1 (e.g. "_1AB.fits")
    QStringList filter;
    filter << "*_1"+type+".fits";
    dir.setNameFilters(filter);
    QStringList fileList = dir.entryList();
    return fileList.length();
}

bool DataDir::hasType(QString type) {
    // Do we have FITS files of a certain type (e.g. "_1AB.fits")
    QStringList filter;
    filter << "*_1"+type+".fits";
    dir.setNameFilters(filter);
    QStringList fileList = dir.entryList();
    if (fileList.length() > 0) {
        return true;
    } else {
        return false;
    }
}
