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

#ifndef MEMORYVIEWER_H
#define MEMORYVIEWER_H

#include "../processingInternal/controller.h"
#include "../processingInternal/data.h"
#include "../datamodel/datamodel.h"
#include "../threading/memoryworker.h"
#include "../iview/iview.h"

#include <QDockWidget>
#include <QMainWindow>
#include <QList>
#include <QThread>
#include <QComboBox>

class MainWindow;   // forward declaration to access GUI members
class MemoryWorker; // Not sure why I need this forward declaration (otherwise compiler error; the controller doesn't require it, otoh)
class Controller;   // only because of including memoryworker
class DataModel;    // only because of including memoryworker

namespace Ui {
class MemoryViewer;
}

class MemoryViewer : public QDockWidget
{
    Q_OBJECT

public:
    explicit MemoryViewer(Controller *ctrl, MainWindow *parent = 0);
    ~MemoryViewer();

    MainWindow *mainGUI;
    Controller *controller;
    Ui::MemoryViewer *ui;  // public, so that we can access its members from outside
    QList<DataModel*> dataModelList;
    QThread *workerThread;
    MemoryWorker *worker;
    bool iViewOpen = false;

private:
    void addData(const QList<Data *> &DT_x, const QString &type);
    bool workerInit = false;
    bool workerThreadInit = false;
    IView *iView;

    DataModel *emptyModel = new DataModel(this);    
    void addGlobalWeights();
    void addDataWeights(const QList<Data *> &DT_x);
    void addBackupDirs(const QString &dirName);
    void showhideStatusCheckBoxes(QString type);
    void updateStatusCheckBoxes(Data *data);
    void repopulateRestoreComboBox();
    void statusDataToCheckBox(Data *data);
    void statusCheckBoxToData();
    Data *getDataClassThisModel();
    void hideStatusCheckBoxes();

public slots:
    void populate();
    void clearMemoryViewReceived();
    void populateMemoryViewReceived();
    void addBackupDirReceived(QString scienceDir);
    void updateStatusCheckBoxesReceived(QString statusString);
    void projectResetReceived();

private slots:
    void on_datadirComboBox_currentIndexChanged(int index);
    void on_downloadToolButton_clicked();
//    void activeStateCheckBoxClicked(QModelIndex index);
    void writeCheckBoxClicked(const QModelIndex &index);
    void on_memoryTableView_clicked(const QModelIndex &index);
    void currentlyDisplayedIndex_received(int currentId);
    void on_restorePushButton_clicked();
    void updateProcessingStatusOnDriveAndInData();
    void updateStatusTipRestoreButton();
    void on_restoreComboBox_currentTextChanged(const QString &arg1);
    void iViewClosed_received();

signals:
    void beginResetModel();
    void endResetModel();
    void reraiseMemoryViewer();

};

#endif // MEMORYVIEWER_H
