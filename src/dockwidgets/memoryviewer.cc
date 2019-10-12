#include "memoryviewer.h"
#include "ui_memoryviewer.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "../myimage/myimage.h"
#include "functions.h"
#include "../processingInternal/controller.h"
#include "../processingInternal/data.h"
#include "../datamodel/datamodel.h"
#include "../threading/memoryworker.h"

#include <omp.h>

#include <QList>
#include <QTest>
#include <QThread>
#include <QTableView>
#include <QString>
#include <QStringList>

MemoryViewer::MemoryViewer(Controller *ctrl, MainWindow *parent) :
    QDockWidget(parent),
    ui(new Ui::MemoryViewer)
{
    ui->setupUi(this);
    mainGUI = parent;
    controller = ctrl;

//    connect(ui->memoryTableView, &QTableView::clicked, this, &MemoryViewer::writeCheckBoxClicked);
    connect(ui->procstatusHDUreformatCheckbox, &QCheckBox::clicked, this, &MemoryViewer::updateProcessingStatusOnDriveAndInData);
    connect(ui->procstatusProcessscienceCheckbox, &QCheckBox::clicked, this, &MemoryViewer::updateProcessingStatusOnDriveAndInData);
    connect(ui->procstatusChopnodCheckbox, &QCheckBox::clicked, this, &MemoryViewer::updateProcessingStatusOnDriveAndInData);
    connect(ui->procstatusBackgroundCheckbox, &QCheckBox::clicked, this, &MemoryViewer::updateProcessingStatusOnDriveAndInData);
    connect(ui->procstatusCollapseCheckbox, &QCheckBox::clicked, this, &MemoryViewer::updateProcessingStatusOnDriveAndInData);
    connect(ui->procstatusStarflatCheckbox, &QCheckBox::clicked, this, &MemoryViewer::updateProcessingStatusOnDriveAndInData);
    connect(ui->procstatusSkysubCheckbox, &QCheckBox::clicked, this, &MemoryViewer::updateProcessingStatusOnDriveAndInData);

    QFile file(":/qss/default.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QString::fromLatin1(file.readAll());
    qApp->setStyleSheet(styleSheet);

    QIcon key(":/images/key.png");
    QIcon download(":/images/download.png");
    QIcon upload(":/images/upload.png");
    ui->downloadToolButton->setIcon(download);
    ui->uploadToolButton->setIcon(upload);

    // Not sure what this button actually would do (that cannot be done with the checkboxes)
    ui->uploadToolButton->hide();
}

MemoryViewer::~MemoryViewer()
{
    /*
    for (auto &dataModel : dataModelList) {
        if (dataModel != nullptr) delete dataModel;
    }
    if (workerInit) delete worker;
    */
    if (workerThreadInit) {
        workerThread->quit();
        workerThread->wait();
        //        delete workerThread;
    }

    delete ui;
}

void MemoryViewer::populate()
{
    ui->datadirComboBox->clear();
    ui->restoreComboBox->clear();
    dataModelList.clear();

    // Add data from the defined data directories
    addData(controller->DT_SCIENCE, "SCIENCE");
    addData(controller->DT_SKY, "SKY");
    addData(controller->DT_BIAS, "BIAS");
    addData(controller->DT_DARK, "DARK");
    addData(controller->DT_FLATOFF, "FLATOFF");
    addData(controller->DT_FLAT, "FLAT");
    addData(controller->DT_STANDARD, "STD");

    addDataWeights(controller->DT_SCIENCE);
    addDataWeights(controller->DT_SKY);
    addDataWeights(controller->DT_STANDARD);

    addGlobalWeights();

    on_datadirComboBox_currentIndexChanged(0);
}

void MemoryViewer::clearMemoryViewReceived()
{
    //    ui->datadirComboBox->clear();
    //    emit beginResetModel();

    ui->memoryTableView->clearSpans();
    ui->memoryTableView->setModel(emptyModel);
    ui->memoryTableView->resizeColumnsToContents();
    ui->memoryTableView->resizeRowsToContents();
    ui->memoryTableView->sortByColumn(0, Qt::AscendingOrder);
    ui->memoryTableView->horizontalHeader()->setStretchLastSection(true);
    ui->memoryTableView->show();

    for (auto &model : dataModelList) {
        model->deleteLater();
    }
    dataModelList.clear();

    ui->datadirComboBox->clear();
    ui->restoreComboBox->clear();
}

void MemoryViewer::populateMemoryViewReceived()
{
    populate();
    if (dataModelList.isEmpty()) return;

    //    emit endResetModel();
    ui->datadirComboBox->setCurrentIndex(0);
    ui->memoryTableView->clearSpans();
    ui->memoryTableView->setModel(dataModelList[0]);
    ui->memoryTableView->resizeColumnsToContents();
    ui->memoryTableView->resizeRowsToContents();
    ui->memoryTableView->sortByColumn(0, Qt::AscendingOrder);
    ui->memoryTableView->horizontalHeader()->setStretchLastSection(true);
    ui->memoryTableView->show();

    /*
      for (auto &model : dataModelList) {
        delete model;
        model = nullptr;
    }
*/
}

void MemoryViewer::addData(const QList<Data*> &DT_x, const QString &type)
{
    if (DT_x.isEmpty()) return;

    QStringList list;
    for (auto &data : DT_x) {
        if (data == nullptr
                || !data->dataInitialized
                || !data->dir.exists()
                || data->subDirName.isEmpty()) continue;
        list << type+":"+" "+data->subDirName;
        DataModel *dataModel = new DataModel(data);
        dataModel->setParent(this);
        dataModelList.append(dataModel);
        connect(this, &MemoryViewer::beginResetModel, dataModel, &DataModel::beginResetModelReceived, Qt::DirectConnection);
        connect(this, &MemoryViewer::endResetModel, dataModel, &DataModel::endResetModelReceived, Qt::DirectConnection);
    }
    if (list.isEmpty()) return;
    ui->datadirComboBox->addItems(list);
    ui->datadirComboBox->update();
}

void MemoryViewer::addDataWeights(const QList<Data*> &DT_x)
{
    if (DT_x.isEmpty()) return;
    QStringList list;
    QString weight = "WEIGHT:";
    for (auto &data : DT_x) {
        if (!data->dataInitialized) continue;
        QDir weightDir(data->mainDirName+"/WEIGHTS");
        if (!weightDir.exists()) continue;
        list << weight+" "+data->subDirName;
        DataModel *dataModel = new DataModel(data, "weight");
        dataModel->setParent(this);
        dataModelList.append(dataModel);
        connect(this, &MemoryViewer::beginResetModel, dataModel, &DataModel::beginResetModelReceived, Qt::DirectConnection);
        connect(this, &MemoryViewer::endResetModel, dataModel, &DataModel::endResetModelReceived, Qt::DirectConnection);
    }
    if (list.isEmpty()) return;
    ui->datadirComboBox->addItems(list);
    ui->datadirComboBox->update();
}

void MemoryViewer::addGlobalWeights()
{
    if (controller->GLOBALWEIGHTS == nullptr
            || !controller->GLOBALWEIGHTS->dataInitialized
            || !controller->GLOBALWEIGHTS->dir.exists()) return;
    DataModel *dataModel = new DataModel(controller->GLOBALWEIGHTS);
    dataModel->setParent(this);
    dataModelList.append(dataModel);
    connect(this, &MemoryViewer::beginResetModel, dataModel, &DataModel::beginResetModelReceived, Qt::DirectConnection);
    connect(this, &MemoryViewer::endResetModel, dataModel, &DataModel::endResetModelReceived, Qt::DirectConnection);
    ui->datadirComboBox->addItem("GLOBALWEIGHTS");
    ui->datadirComboBox->update();
}

void MemoryViewer::on_datadirComboBox_currentIndexChanged(int index)
{
    if (dataModelList.isEmpty() || index == -1) return;
    if (ui->datadirComboBox->count() == 0) return;

    if (mainGUI->doingInitialLaunch || mainGUI->readingSettings) return;

    ui->memoryTableView->clearSpans();
    ui->memoryTableView->setModel(dataModelList[index]);
    ui->memoryTableView->resizeColumnsToContents();
    ui->memoryTableView->resizeRowsToContents();
    ui->memoryTableView->sortByColumn(0, Qt::AscendingOrder);
    ui->memoryTableView->horizontalHeader()->setStretchLastSection(true);
    ui->memoryTableView->show();

    if (iViewOpen) {
        if (dataModelList[index]->modelType == "weight") iView->weightMode = true;
        else iView->weightMode = false;
        iView->myImageList = dataModelList[index]->imageList;
        iView->numImages = dataModelList[index]->imageList.length();
        if (ui->datadirComboBox->currentText() == "GLOBALWEIGHTS") {
            iView->dirName = controller->mainDirName + "/GLOBALWEIGHTS";
        }
        else {
            iView->dirName = controller->mainDirName + "/" + ui->datadirComboBox->currentText().split(":").at(1).simplified();
        }
    }

    showhideStatusCheckBoxes(dataModelList[index]->modelType);
    updateStatusCheckBoxes(dataModelList[index]->myData);

    // Populate the restore mode accordingly
    ui->restoreComboBox->clear();
    if (dataModelList[index]->modelType == "calib") {
        ui->restoreComboBox->insertItem(0, "RAWDATA");
    }
    if (dataModelList[index]->modelType == "science") {
        QString status1 = dataModelList[index]->imageList[0]->statusBackupL1;
        QString status2 = dataModelList[index]->imageList[0]->statusBackupL2;
        QString status3 = dataModelList[index]->imageList[0]->statusBackupL3;
        ui->restoreComboBox->addItem("RAWDATA");
        if (!status1.isEmpty()) ui->restoreComboBox->addItem(status1+"_IMAGES");
        if (!status2.isEmpty()) ui->restoreComboBox->addItem(status2+"_IMAGES");
        if (!status3.isEmpty()) ui->restoreComboBox->addItem(status3+"_IMAGES");
        addBackupDirs(dataModelList[index]->myData->dirName);
    }

    if (dataModelList[index]->modelType == "weight" || dataModelList[index]->modelType == "globalweight") {
        ui->restoreComboBox->setDisabled(true);
        ui->restorePushButton->setDisabled(true);
    }
    else {
        ui->restoreComboBox->setEnabled(true);
        ui->restorePushButton->setEnabled(true);
    }
    ui->restoreComboBox->update();

    updateStatusTipRestoreButton();
}

// triggered when a processing task has created a new backup dir. Only for the currently visible science data
void MemoryViewer::addBackupDirReceived(QString scienceDir, QString backupDirName)
{
    if (ui->datadirComboBox->currentText() == "SCIENCE: "+scienceDir) {
        repopulateRestoreComboBox();
    }
}

void MemoryViewer::addBackupDirs(const QString &dirName)
{
    QDir dir(dirName);
    QStringList dirList = dir.entryList(QDir::Dirs);
    for (auto &it : dirList) {
        // add the dir to the combobox if it is a true backup dir, and not yet contained in the combobox
        if (it.contains("_IMAGES")
                && ui->restoreComboBox->findText(it) == -1
                && !dir.isEmpty()) {
            ui->restoreComboBox->addItem(it);
        }
    }
    ui->restoreComboBox->update();
}

void MemoryViewer::writeCheckBoxClicked(const QModelIndex &index)
{
    workerThread = new QThread(this);
    worker = new MemoryWorker(this);
    workerInit = true;
    workerThreadInit = true;

    int cbindex = ui->datadirComboBox->currentIndex();
    if (cbindex == -1) return;

    DataModel *model = dataModelList[cbindex];
    if (model->imageList.isEmpty()) return;   // Not sure this is ever happening
    worker->myImage = model->imageList[index.row()];
    worker->moveToThread(workerThread);
    QObject::connect(workerThread, &QThread::started, worker, &MemoryWorker::MemoryViewerDumpImagesToDrive);
    QObject::connect(worker, &MemoryWorker::finished, worker, &QObject::deleteLater);
    QObject::connect(worker, &QObject::destroyed, workerThread, &QThread::quit);

    workerThread->start();
}

/*
void MemoryViewer::activeStateCheckBoxClicked(QModelIndex index)
{
    workerThread = new QThread(this);
    worker = new MemoryWorker(this);
    workerInit = true;
    workerThreadInit = true;

    DataModel *model = dataModelList[ui->datadirComboBox->currentIndex()];
    worker->myImage = model->imageList[index.row()];
    worker->path = controller->mainDirName + "/" + ui->datadirComboBox->currentText();
    worker->moveToThread(workerThread);
    QObject::connect(workerThread, &QThread::started, worker, &MemoryWorker::processActiveStatusChanged);
    QObject::connect(worker, &MemoryWorker::finished, worker, &QObject::deleteLater);
    QObject::connect(worker, &QObject::destroyed, workerThread, &QThread::quit);

    workerThread->start();
}
*/

/*
void MemoryViewer::on_L0ToDrivePushButton_clicked()
{
    ui->L0ToDrivePushButton->setText("Writing data ...");
    ui->L0ToDrivePushButton->setDisabled(true);
    QTest::qWait(50);

    int index = ui->datadirComboBox->currentIndex();

#pragma omp parallel for num_threads(controller->maxExternalThreads)
    for (int i=0; i<dataModelList[index]->imageList.length(); ++i) {
        // TODO: check if status string is properly set
        MyImage *it = dataModelList[index]->imageList[i];
        if (!it->imageOnDrive) {
            it->writeImage(it->path + "/" + it->baseName + controller->statusNew + ".fits");
            it->imageOnDrive = true;
            it->emitModelUpdateNeeded();
        }
    }
    ui->L0ToDrivePushButton->setText("Write L0 data to Drive");
    ui->L0ToDrivePushButton->setEnabled(true);
}
*/

void MemoryViewer::on_downloadToolButton_clicked()
{
    workerThread = new QThread(this);
    worker = new MemoryWorker(this);
    workerInit = true;
    workerThreadInit = true;
    worker->moveToThread(workerThread);
    QObject::connect(workerThread, &QThread::started, worker, &MemoryWorker::MemoryViewerDumpImagesToDrive);
    QObject::connect(worker, &MemoryWorker::finished, worker, &QObject::deleteLater);
    QObject::connect(worker, &QObject::destroyed, workerThread, &QThread::quit);

    workerThread->start();
}

void MemoryViewer::on_memoryTableView_clicked(const QModelIndex &index)
{
    int cbindex = ui->datadirComboBox->currentIndex();
    if (cbindex == -1) return;

    DataModel *model = dataModelList[cbindex];
    if (model->imageList.isEmpty()) return;   // Not sure this is ever happening

    // CHECK: uncommented on 2019-10-02; should have no impact since unused
    //    MyImage *myImage = model->imageList[index.row()];

    if (!iViewOpen) {
        QString dirName = "";
        if (ui->datadirComboBox->currentText() == "GLOBALWEIGHTS") {
            dirName = controller->mainDirName + "/GLOBALWEIGHTS";
        }
        else {
            dirName = controller->mainDirName + "/" + ui->datadirComboBox->currentText().split(":").at(1).simplified();
        }
        iView = new IView("MEMview", model->imageList, dirName, this);
        iViewOpen = true;
        if (model->modelType == "weight") iView->weightMode = true;
        else iView->weightMode = false;
        connect(iView, &IView::currentlyDisplayedIndex, this, &MemoryViewer::currentlyDisplayedIndex_received);
    }
    iView->scene->clear();
    iView->loadFromRAMlist(index);
    iView->currentId = index.row();
    // IView needs to know the directory name so that it can overlay catalogs
    iView->show();
    //    iView->raise();
}

void MemoryViewer::currentlyDisplayedIndex_received(int currentId)
{
    // Deselect current selection
    QModelIndex currentIndex = ui->memoryTableView->selectionModel()->currentIndex();
    ui->memoryTableView->selectionModel()->setCurrentIndex(currentIndex, QItemSelectionModel::Deselect);

    int cbindex = ui->datadirComboBox->currentIndex();
    if (cbindex == -1) return;

    // Make new selection
    QModelIndex index = dataModelList[cbindex]->index(currentId,0);
    ui->memoryTableView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
}

void MemoryViewer::on_restorePushButton_clicked()
{
    Data *data = getDataClassThisModel();
    if (data == nullptr) return;

    QString backupDir = ui->restoreComboBox->currentText();
    if (backupDir.isEmpty()) return;

    data->restoreBackupLevel(backupDir);

    // ui->restoreComboBox->removeItem(ui->restoreComboBox->currentIndex());

    repopulateRestoreComboBox();
}

void MemoryViewer::showhideStatusCheckBoxes(QString type)
{
    Data *data = getDataClassThisModel();
    if (data == nullptr) return;

    if (type == "science") {
        ui->procstatusHDUreformatCheckbox->setVisible(true);
        ui->procstatusProcessscienceCheckbox->setVisible(true);
        ui->procstatusCollapseCheckbox->setVisible(true);
        ui->procstatusSkysubCheckbox->setVisible(true);

        if (data->instData->type == "OPT") {
            ui->procstatusChopnodCheckbox->setVisible(false);
            ui->procstatusBackgroundCheckbox->setVisible(true);
        }
        else if (data->instData->type == "NIR") {
            ui->procstatusChopnodCheckbox->setVisible(false);
            ui->procstatusBackgroundCheckbox->setVisible(true);
        }
        else if (data->instData->type == "NIRMIR") {
            ui->procstatusChopnodCheckbox->setVisible(true);
            ui->procstatusBackgroundCheckbox->setVisible(true);
        }
        else if (data->instData->type == "MIR") {
            ui->procstatusChopnodCheckbox->setVisible(true);
            ui->procstatusBackgroundCheckbox->setVisible(false);
        }

        ui->procstatusStarflatCheckbox->setVisible(false);
    }
    else {
        ui->procstatusHDUreformatCheckbox->setVisible(true);
        ui->procstatusProcessscienceCheckbox->setVisible(false);
        ui->procstatusChopnodCheckbox->setVisible(false);
        ui->procstatusBackgroundCheckbox->setVisible(false);
        ui->procstatusCollapseCheckbox->setVisible(false);
        ui->procstatusStarflatCheckbox->setVisible(false);
        ui->procstatusSkysubCheckbox->setVisible(false);
    }
}

void MemoryViewer::updateStatusCheckBoxes(Data *data)
{
    ui->procstatusHDUreformatCheckbox->setChecked(data->processingStatus->HDUreformat);
    ui->procstatusProcessscienceCheckbox->setChecked(data->processingStatus->Processscience);
    ui->procstatusChopnodCheckbox->setChecked(data->processingStatus->Chopnod);
    ui->procstatusBackgroundCheckbox->setChecked(data->processingStatus->Background);
    ui->procstatusCollapseCheckbox->setChecked(data->processingStatus->Collapse);
    ui->procstatusStarflatCheckbox->setChecked(data->processingStatus->Starflat);
    ui->procstatusSkysubCheckbox->setChecked(data->processingStatus->Skysub);
}

// reflect a status update in the currently shown checkboxes (if they represent the Data class that sent the signal)
void MemoryViewer::updateStatusCheckBoxesReceived(QString statusstring)
{
    // The argument is ignored (used in other slots)

    int index = ui->datadirComboBox->currentIndex();
    if (dataModelList.isEmpty() || index == -1 || mainGUI->doingInitialLaunch) return;
    Data *data = qobject_cast<Data*>(sender());

    if (dataModelList[index]->myData == data) statusDataToCheckBox(data);
}

// Reflect the processingStatus of Data class in the checkboxes
void MemoryViewer::statusDataToCheckBox(Data *data)
{
    ui->procstatusHDUreformatCheckbox->setChecked(data->processingStatus->HDUreformat);
    ui->procstatusProcessscienceCheckbox->setChecked(data->processingStatus->Processscience);
    ui->procstatusChopnodCheckbox->setChecked(data->processingStatus->Chopnod);
    ui->procstatusBackgroundCheckbox->setChecked(data->processingStatus->Background);
    ui->procstatusCollapseCheckbox->setChecked(data->processingStatus->Collapse);
    ui->procstatusStarflatCheckbox->setChecked(data->processingStatus->Starflat);
    ui->procstatusSkysubCheckbox->setChecked(data->processingStatus->Skysub);
}

// Reflect the processingStatus of the checkboxes in the Data class
void MemoryViewer::statusCheckBoxToData()
{
    Data *data = getDataClassThisModel();
    if (data == nullptr) return;

    data->processingStatus->HDUreformat = ui->procstatusHDUreformatCheckbox->isChecked();
    data->processingStatus->Processscience = ui->procstatusProcessscienceCheckbox->isChecked();
    data->processingStatus->Chopnod = ui->procstatusChopnodCheckbox->isChecked();
    data->processingStatus->Background = ui->procstatusBackgroundCheckbox->isChecked();
    data->processingStatus->Collapse = ui->procstatusCollapseCheckbox->isChecked();
    data->processingStatus->Starflat = ui->procstatusStarflatCheckbox->isChecked();
    data->processingStatus->Skysub = ui->procstatusSkysubCheckbox->isChecked();
}

Data* MemoryViewer::getDataClassThisModel()
{
    int index = ui->datadirComboBox->currentIndex();
    if (dataModelList.isEmpty() || index == -1 || mainGUI->doingInitialLaunch) nullptr;
    return dataModelList[index]->myData;
}

void MemoryViewer::repopulateRestoreComboBox()
{
    QString dirName = controller->mainDirName + "/" + ui->datadirComboBox->currentText().split(":").at(1).simplified();
    QDir dir(dirName);
    if (!dir.exists()) return;  // should never happen

    QStringList filters;
    filters << "RAWDATA" << "*_IMAGES";
    QStringList backupdirList = dir.entryList(filters, QDir::Dirs);
    ui->restoreComboBox->clear();
    ui->restoreComboBox->addItems(backupdirList);
    ui->restoreComboBox->update();
}

void MemoryViewer::updateProcessingStatusOnDriveAndInData()
{
    Data *data = getDataClassThisModel();
    if (data == nullptr) return;

    statusCheckBoxToData();

    data->processingStatus->getStatusString();
    data->processingStatus->writeToDrive();
}

void MemoryViewer::updateStatusTipRestoreButton()
{
    if (ui->restoreComboBox->count() == 0 || ui->datadirComboBox->count() == 0) {
        ui->restorePushButton->setStatusTip("Restores certain processing stages");
        return;
    }

    QString backupDir = ui->restoreComboBox->currentText();
    QString dirName = ui->datadirComboBox->currentText().split(":").at(1).simplified();

    QString statusTip = "Restores "+backupDir+" in " + dirName + ". Images currently present in "+dirName + " will be deleted!";
    ui->restorePushButton->setStatusTip(statusTip);
}

void MemoryViewer::on_restoreComboBox_currentTextChanged(const QString &arg1)
{
    updateStatusTipRestoreButton();
}