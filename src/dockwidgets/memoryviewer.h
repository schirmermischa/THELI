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

public slots:
    void populate();
    void clearMemoryViewReceived();
    void populateMemoryViewReceived();
    void addBackupDirReceived(QString scienceDir, QString backupDirName);
    void updateStatusCheckBoxesReceived(QString statusstring);

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

signals:
    void beginResetModel();
    void endResetModel();

};

#endif // MEMORYVIEWER_H
