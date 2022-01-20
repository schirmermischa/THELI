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

#include "datamodel.h"
#include <QDebug>
#include <QString>

#include "../myimage/myimage.h"

DataModel::DataModel(Data *someData, QObject *parent)
    : QAbstractTableModel(parent)
{
    myData = someData;

    if (myData->dataType == "BIAS"
            || myData->dataType == "DARK"
            || myData->dataType == "FLATOFF"
            || myData->dataType == "FLAT") {
        modelType = "calib";
    }
    else {
        if (myData->dataType == "GLOBALWEIGHT") modelType = "globalweight";
        else modelType = "science";
    }

    // Create the image list
    updateImageList();

    connect(myData, &Data::modelUpdateNeeded, this, &DataModel::modelUpdateReceiver);
    connect(myData, &Data::globalModelUpdateNeeded, this, &DataModel::updateImageList);
    connect(myData, &Data::updateModelHeaderLine, this, &DataModel::updateHeaderLineGlobal);
}

DataModel::DataModel(Data *someData, QString weight, QObject *parent)
    : QAbstractTableModel(parent)
{
    myData = someData;

    modelType = weight;         // must be "weight"

    // Create the image list
    updateImageList();

    connect(myData, &Data::modelUpdateNeeded, this, &DataModel::modelUpdateReceiver);
}

// Empty data model (displayed when switching projects)
DataModel::DataModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    myData = nullptr;
    imageList.clear();
}

void DataModel::updateImageList()
{
    if (myData == nullptr) return;

    imageList.clear();

    // Master calibrators
    if (modelType == "calib" && !myData->combinedImage.isEmpty()) {
        for (auto &image: myData->combinedImage) {
            if (image == nullptr) break;
            if (!image->baseName.isEmpty()) {
                imageList.append(image);
            }
        }
    }
    // Individual images
    if (!myData->myImageList.isEmpty()) {
        for (int chip=0; chip<myData->instData->numChips; ++chip) {
            // Exposure list
            if (chip == 0) {
                for (auto &image: myData->myImageList[chip]) {
                    exposureList.append(image);
                }
            }
            // All images list
            for (auto &image: myData->myImageList[chip]) {
                imageList.append(image);
            }
        }
    }
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Image");
        case 1:
            return QString("Active");
        case 2:
            return QString("On\nDrive");
        case 3:
        {
            QString add = "";
            if (myData != nullptr) add = myData->processingStatus->statusString;
            return QString("L0 in\nRAM\n("+add+")");
        }
        case 4:
        {
            QString add = "";
            if (myData != nullptr) add = myData->statusBackupL1;
            return QString("L1 in\nRAM\n("+add+")");
        }
        case 5:
        {
            QString add = "";
            if (myData != nullptr) add = myData->statusBackupL2;
            return QString("L2 in\nRAM\n("+add+")");
        }
        case 6:
        {
            QString add = "";
            if (myData != nullptr) add = myData->statusBackupL3;
            return QString("L3 in\nRAM\n("+add+")");
        }
        case 7:
            return QString("Comment");
        }
    }
    return QVariant();
}

int DataModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || myData == nullptr) return 0;

    // Total length is given by the images and master calibration files

    // Master cals only exist for calibration data, hence the check against nullptr
    long numMasterCals = myData->numMasterCalibs;

    // Single exposures
    long numExposures = 0;
    if (myData->myImageList.isEmpty()) return numMasterCals;

    for (int chip=0; chip<myData->instData->numChips; ++chip) {
        numExposures += myData->myImageList[chip].length();
    }
    return numMasterCals + numExposures;
}

int DataModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return 8;
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || imageList.isEmpty()) return QVariant();

    int row = index.row();
    int col = index.column();

    if (row >= imageList.length()) {
        //    qDebug() << "DataModel::data(): invalid row index" << row <<", max num images =" << imageList.length();
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole:
        // First column contains the image name. Blanks are added for a little extra space
        if (modelType == "weight") {
            if (col == 0) return imageList[row]->rootName + "_" + QString::number(imageList[row]->chipNumber) + ".weight  ";
            // Y/N indicators
            if (col==2) {
                if (!imageList[row]->weightOnDrive) return "N";
                else return "Y";
            }
            else if (col==3) {
                if (!imageList[row]->weightInMemory) return "N";
                else return "Y";
            }
            else if (col==4 || col == 5 || col == 6) return "N";
            else if (col == 7) return "";
        }
        if (modelType == "globalweight") {
            if (col == 0) return imageList[row]->rootName + "_" + QString::number(imageList[row]->chipNumber) + "  ";
            // Y/N indicators
            if (col==2) {
                if (!imageList[row]->imageOnDrive) return "N";
                else return "Y";
            }
            else if (col==3) {
                if (!imageList[row]->imageInMemory) return "N";
                else return "Y";
            }
            else if (col==4 || col == 5 || col == 6) return "N";
            else if (col == 7) return "";
        }
        else {
            if (col == 0) return imageList[row]->rootName + "_" + QString::number(imageList[row]->chipNumber) + "  ";
            // Y/N indicators
            if (col==2) {
                if (!imageList[row]->imageOnDrive) return "N";
                else return "Y";
            }
            else if (col==3) {
                if (!imageList[row]->imageInMemory) return "N";
                else return "Y";
            }
            else if (col==4) {
                if (!imageList[row]->backupL1InMemory) return "N";
                else return "Y";
            }
            else if (col==5) {
                if (!imageList[row]->backupL2InMemory) return "N";
                else return "Y";
            }
            else if (col==6) {
                if (!imageList[row]->backupL3InMemory) return "N";
                else return "Y";
            }
            else if (col==7) {
                if (imageList[row]->activeState == MyImage::ACTIVE) return "";
                else if (imageList[row]->activeState == MyImage::INACTIVE) return "Deactivated";
                else if (imageList[row]->activeState == MyImage::BADBACK) return "Poor background model";
                else if (imageList[row]->activeState == MyImage::BADSTATS) return "Poor data quality";
                else if (imageList[row]->activeState == MyImage::LOWDETECTION) return "Insufficient sources for astrometry";
                else if (imageList[row]->activeState == MyImage::DELETED) return "Image not found or deleted";
                else return "";
            }
        }
        break;
    case Qt::BackgroundRole:
        // Color coding
        if (modelType == "weight") {
            if (col==0) return QBrush(white);
            if (col==1) return QBrush(turquois);
            if (col==2) {
                if (!imageList[row]->weightOnDrive) return QBrush(red);
                else return QBrush(green);
            }
            else if (col==3) {
                if (!imageList[row]->weightInMemory) return QBrush(orange);
                else return QBrush(blue);
            }
            if (col == 4 || col == 5 || col == 6) return QBrush(orange);
        }
        else if (modelType == "globalweight") {
            if (col==0) return QBrush(white);
            if (col==1) return QBrush(turquois);
            if (col==2) {
                if (!imageList[row]->imageOnDrive) return QBrush(red);
                else return QBrush(green);
            }
            else if (col==3) {
                if (!imageList[row]->imageInMemory) return QBrush(orange);
                else return QBrush(blue);
            }
            if (col == 4 || col == 5 || col == 6) return QBrush(orange);
        }
        else {
            if (col==0) {
                if (imageList[row]->activeState != MyImage::ACTIVE) return QBrush(grey);
                else return QBrush(white);
            }
            if (col==1) {
                if (imageList[row]->activeState != MyImage::ACTIVE) return QBrush(grey);
                else return QBrush(turquois);
            }
            if (col==2) {
                if (!imageList[row]->imageOnDrive) return QBrush(red);
                else return QBrush(green);
            }
            else if (col==3) {
                if (!imageList[row]->imageInMemory) return QBrush(orange);
                else return QBrush(blue);
            }
            else if (col==4) {
                if (!imageList[row]->backupL1InMemory) return QBrush(orange);
                else return QBrush(blue);
            }
            else if (col==5) {
                if (!imageList[row]->backupL2InMemory) return QBrush(orange);
                else return QBrush(blue);
            }
            else if (col==6) {
                if (!imageList[row]->backupL3InMemory) return QBrush(orange);
                else return QBrush(blue);
            }
        }
        break;
    case Qt::TextAlignmentRole:
        if (col == 0 || col == 7) return Qt::AlignLeft;
        else return Qt::AlignCenter;
        break;
    case Qt::CheckStateRole:
        if (modelType == "weight") {
            if (col == 1) return Qt::Checked;
            if (col == 2) {
                if (!imageList[row]->weightOnDrive) return Qt::Unchecked;
                else return Qt::Checked;
            }
        }
        else if (modelType == "globalweight") {
            if (col == 1) return Qt::Checked;
            if (col == 2) {
                if (!imageList[row]->imageOnDrive) return Qt::Unchecked;
                else return Qt::Checked;
            }
        }
        else {
            if (col == 1) {
                if (imageList[row]->activeState != MyImage::ACTIVE) return Qt::Unchecked;
                else return Qt::Checked;
            }
            if (col == 2) {
                if (!imageList[row]->imageOnDrive) return Qt::Unchecked;
                else return Qt::Checked;
            }
        }
        break;
    }
    return QVariant();
}

Qt::ItemFlags DataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) return Qt::NoItemFlags;

    // Inherit and append mode
    Qt::ItemFlags result = QAbstractTableModel::flags(index);

    if (modelType == "weight" || modelType == "globalweight") {
        // Toggle active / inactive state (triggers physical move of file on disk)
        if (index.column() == 1) {
            result = Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
        }
        // Clicking on an unchecked box will check it (dumps data to disk)
        if (index.column() == 2) {
            result = Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
        }
        if (index.column() == 3) result = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
        if (index.column() > 3) result = Qt::ItemIsEnabled |Qt::ItemNeverHasChildren;
    }
    else {
        // Toggle active / inactive state (triggers physical move of file on disk)
        if (index.column() == 1) {
            result = Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemNeverHasChildren;
        }
        // Clicking on an unchecked box will check it (dumps data to disk)
        if (index.column() == 2) {
            result = Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemNeverHasChildren;
        }
        if (index.column() > 2) result = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
    }

    return result;
}

bool DataModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (!index.isValid()) return false;

    if (modelType == "weight" || modelType == "globalweight") return false;

    // Activate / deactivate images
    if (role == Qt::CheckStateRole && index.column() == 1) {
        // CONFUSING! Example:
        // The user clicks on a checked 'active' box to deactivate the image.
        // The 'active' checkbox state e.g. changes from checked to unchecked,
        // but only THEN this slot is invoked, at which point the checkbox is already unchecked.
        if ((Qt::CheckState)value.toInt() == Qt::Checked) {
            // the user clicked an unchecked box, which is now checked, hence we must activate the image
            // ONLY do this if the deactivated image has the same processing state as the active images!
            QString imagestatus = imageList[index.row()]->processingStatus->statusString;
            QString drivestatus = imageList[index.row()]->processingStatus->whatIsStatusOnDrive();
            if (imagestatus == drivestatus) {
                imageList[index.row()]->oldState = imageList[index.row()]->activeState;
                imageList[index.row()]->setActiveState(MyImage::ACTIVE); // Setting this value automatically moves the image on drive accordingly!
            }
            else {
                emit activationWarning(imagestatus, drivestatus);
            }
        }
        else {
            // the user clicked an checked box, which is now unchecked, hence we must deactivate the image
            imageList[index.row()]->oldState = imageList[index.row()]->activeState;
            imageList[index.row()]->setActiveState(MyImage::INACTIVE); // Setting this value automatically moves the image on drive accordingly!
        }
        emit dataChanged(index,index);
        return true;
    }
    // Force write to drive
    if (role == Qt::CheckStateRole && index.column() == 2) {
        if ((Qt::CheckState)value.toInt() == Qt::Checked) {
            MyImage *it = imageList[index.row()];
            if (!it->imageOnDrive) {
                it->writeImage();
                it->imageOnDrive = true;
            }
        }
        emit dataChanged(index,index);
        return true;
    }
    return false;
}

void DataModel::modelUpdateReceiver(QString chipName)
{
    // Find the index of the MyImage that has emitted the original signal
    QModelIndexList list = match(index(0,0), Qt::DisplayRole, chipName);
    if (list.isEmpty()) {
        // An image was removed
        // Consistency check (required for debayering, which adds images that were not there previously)
        updateImageList();
        return;
    }

    // retrieve the indices of the various cells for the current exposure
    long row = list[0].row();
    QModelIndex indexImage = index(row, 0);
    QModelIndex indexState = index(row, 1);
    QModelIndex indexDrive = index(row, 2);
    QModelIndex indexL0 = index(row, 3);
    QModelIndex indexL1 = index(row, 4);
    QModelIndex indexL2 = index(row, 5);
    QModelIndex indexL3 = index(row, 6);
    // QModelIndex indexComment = index(row, 7);         // unused

    // Update the background colors
    setData(indexImage, getColorImage(imageList[row]->activeState), Qt::BackgroundRole);
    setData(indexState, getColorState(imageList[row]->activeState), Qt::BackgroundRole);
    setData(indexDrive, getColorDrive(imageList[row]->imageOnDrive), Qt::BackgroundRole);
    setData(indexL0, getColorRAM(imageList[row]->imageInMemory), Qt::BackgroundRole);
    setData(indexL1, getColorRAM(imageList[row]->backupL1InMemory), Qt::BackgroundRole);
    setData(indexL2, getColorRAM(imageList[row]->backupL2InMemory), Qt::BackgroundRole);
    setData(indexL3, getColorRAM(imageList[row]->backupL3InMemory), Qt::BackgroundRole);

    // Update the Y/N chars
    setData(indexDrive, getIndicator(imageList[row]->imageOnDrive), Qt::DisplayRole);
    setData(indexL0, getIndicator(imageList[row]->imageInMemory), Qt::DisplayRole);
    setData(indexL1, getIndicator(imageList[row]->backupL1InMemory), Qt::DisplayRole);
    setData(indexL2, getIndicator(imageList[row]->backupL2InMemory), Qt::DisplayRole);
    setData(indexL3, getIndicator(imageList[row]->backupL3InMemory), Qt::DisplayRole);

    updateHeaderLine(row);

    // Inform the view that the model has changed
    emit dataChanged(indexDrive,indexL3);
}

void DataModel::updateheaderLineExternal()
{
    updateHeaderLine(0);
    emit dataChanged(index(0, 0), index(0,5));
}

void DataModel::updateHeaderLine(long row)
{
    setHeaderData(3, Qt::Horizontal, "L0 in\nRAM\n("+imageList[row]->processingStatus->statusString+")");
    setHeaderData(4, Qt::Horizontal, "L1 in\nRAM\n("+imageList[row]->statusBackupL1+")");
    setHeaderData(5, Qt::Horizontal, "L2 in\nRAM\n("+imageList[row]->statusBackupL2+")");
    setHeaderData(6, Qt::Horizontal, "L3 in\nRAM\n("+imageList[row]->statusBackupL3+")");
}

void DataModel::updateHeaderLineGlobal()
{
    setHeaderData(3, Qt::Horizontal, "L0 in\nRAM\n("+myData->processingStatus->statusString+")");
    setHeaderData(4, Qt::Horizontal, "L1 in\nRAM\n("+myData->statusBackupL1+")");
    setHeaderData(5, Qt::Horizontal, "L2 in\nRAM\n("+myData->statusBackupL2+")");
    setHeaderData(6, Qt::Horizontal, "L3 in\nRAM\n("+myData->statusBackupL3+")");
}

void DataModel::beginResetModelReceived()
{
    beginResetModel();
}

void DataModel::endResetModelReceived()
{
    endResetModel();
}

QColor DataModel::getColorImage(MyImage::active_type state)
{
    if (state == MyImage::ACTIVE) return white;
    else return grey;
}

QColor DataModel::getColorState(MyImage::active_type state)
{
    if (state == MyImage::ACTIVE) return turquois;
    else return grey;
}

QColor DataModel::getColorDrive(bool flag)
{
    if (flag) return green;
    else return red;
}

QColor DataModel::getColorRAM(bool flag)
{
    if (flag) return blue;
    else return orange;
}

QString DataModel::getIndicator(bool flag)
{
    if (flag) return "Y";
    else return "N";
}

// E.g. master calibrations are created
bool DataModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    QVector<bool> dummyMask;
    dummyMask.clear();
    MyImage *newImage = new MyImage("path", "file", "", 1, dummyMask, myData->verbosity);
    connect(newImage, &MyImage::modelUpdateNeeded, this, &DataModel::modelUpdateReceiver);
    imageList.insert(row, newImage);
    endInsertRows();
    return true;
}

bool DataModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // Nothing to be done
    endInsertColumns();
    return true;
}

// E.g. input images after they were debayered
bool DataModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    imageList.removeAt(row);
    endRemoveRows();
    return true;
}

bool DataModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // Nothing to be done
    endRemoveColumns();
    return true;
}
