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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "readmes/multidirreadme.h"
#include "readmes/license.h"
#include "readmes/acknowledging.h"
#include "abszp/abszeropoint.h"
#include "instrumentdefinition.h"
#include "instrumentdata.h"
#include "colorpicture/colorpicture.h"
#include "imagestatistics/imagestatistics.h"
#include "threading/mainguiworker.h"
#include "functions.h"
#include "status.h"
#include "processingExternal/errordialog.h"
#include "processingInternal/controller.h"
#include "preferences.h"
#include "datadir.h"
#include "ui_mainwindow.h"
#include "dockwidgets/confdockwidget.h"
#include "dockwidgets/monitor.h"
#include "dockwidgets/memoryviewer.h"

#include <QMainWindow>
#include <QLineEdit>
#include <QProgressBar>
#include <QProcessEnvironment>
#include <QProcess>
#include <QSettings>
#include <QThread>
#include <QTimer>
#include <QFile>
#include <QDebug>
#include <QStringListModel>

namespace Ui {
class MainWindow;
}

// Forward declaration
class MyStringListModel;
class MyStringValidator;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString pid, QWidget *parent = nullptr);
    ~MainWindow();

    QString instrument_dir;
    QString instrument_userDir;

    QString GUIVERSION = "3.0.RC1";
    QString editorPreference;
    QString downloadServer;
    QString mainPID;
    IView *solutionViewer;

    Controller *controller;

    ConfDockWidget *cdw;
    Monitor *monitor;
    MemoryViewer *memoryViewer = nullptr;

    Ui::MainWindow *ui;

    Status status;
    QMap<QString,QString> taskCommentMap;

    QList<QThread*> threadList;

    QProgressBar *cpuProgressBar;
    QProgressBar *memoryProgressBar;
    QProgressBar *driveProgressBar;
    bool datadiskspace_warned;
    bool homediskspace_warned;
    int diskwarnPreference;
    bool doingInitialLaunch = false;
    bool readingSettings = false;
    bool checkPathsLineEdit(QLineEdit *lineEdit);

signals:
    QFont sendingDefaultFont(QFont);         //  implemented in designer
    void runningStatusChanged(bool running);
    void ControllerScanDataTree(QLineEdit *lineEdit);
    void newProjectLoaded();
    void rereadScienceDataDir();
    void messageAvailable(QString message, QString type);

public slots:
    void processMessage(QString text, QString type);
    void taskFinished();
    void launchViewer(QString dirname, QString filter, QString mode);
 //   void launchCoaddFluxcal(QString coaddImage);
    void appendOK();
//    void resumeWorkerThread(QString acceptanceState);
    void on_startPushButton_clicked();
    void showMessageBoxReceived(QString trigger, QString part1, QString part2);
    void progressUpdateReceived(float progress);
    void resetProgressBarReceived();
    void updateSwitchProcessMonitorPreference(bool switchToMonitor);
    void statusChangedReceived(QString newStatus);
    void updateExcludedDetectors(QString badDetectors);

protected:
    // Don't know yet what the 'override' means
    void closeEvent(QCloseEvent *event) override;

    QString thelidir;
    QString userdir;
    QString tmpdir;

    QFile instrument_file;
    QString instrument_name;
    QString instrument_type;
    QString instrument_bayer;
    int instrumentPreference;
    int numCPU = 1;
    int nframes = 1;
    int instrument_nchips = 1;
    long systemRAM;

    QString kernelType;
    QString productName;

private slots:
    void reset();
    void checkPaths();
    void connect_validators();
    void establish_connections();
    void initGUI();
    void link_ConfToolButtons_confStackedWidget();
    void link_taskCheckBoxes_confStackedWidget();
    void loadPreferences();
    void loadIView();
    void load_dialog_newinst();
    void load_dialog_imageStatistics();
    void load_dialog_abszeropoint();
    void load_dialog_colorpicture();
    void cdw_dockLocationChanged(const Qt::DockWidgetArea &area);
    void cdw_topLevelChanged(bool topLevel);
    void on_actionAdd_new_instrument_configuration_triggered();
    void on_actionEdit_preferences_triggered();
    void on_actionKill_triggered();
    void on_yieldToolButton_clicked();
    void on_stopToolButton_clicked();
    void on_processingTabWidget_currentChanged(int index);
    void on_setupInstrumentComboBox_currentTextChanged(const QString &arg1);
    void on_setupProjectLoadToolButton_clicked();
    void on_setupReadmePushButton_clicked();
    void scienceDataDirUpdatedReceived(QString allDirs);
    void startPushButton_clicked_dummy(QString string);
    void shutDown();
//    void undoToolButton_clicked();
    void updateFontSize(int index);
    void updateFont(QFont font);
    void updateDiskspaceWarning(int newLimit);
    void updateServer(QString server);
    void updateNumcpu(int cpu);
    void updateController();
    void updateControllerFunctors(QString text);
    void validate();
    int writePreferenceSettings();
    int writeGUISettings();

    // The following can also be under 'private', but then the declaration must be preceeded like this:
    // Q_INVOKABLE QString taskHDUreformat();
    QStringList taskHDUreformat(bool &stop, QString mode);
    QStringList taskProcessbias(bool &stop, QString mode);
    QStringList taskProcessdark(bool &stop, QString mode);
    QStringList taskProcessflatoff(bool &stop, QString mode);
    QStringList taskProcessflat(bool &stop, QString mode);
    QStringList taskProcessscience(bool &stop, QString mode);
    QStringList taskChopnod(bool &stop, QString mode);
    QStringList taskBackground(bool &stop, QString mode);
    QStringList taskCollapse(bool &stop, QString mode);
    QStringList taskBinnedpreview(bool &stop, QString mode);
    QStringList taskGlobalweight(bool &stop, QString mode);
    QStringList taskIndividualweight(bool &stop, QString mode);
    QStringList taskSeparate(bool &stop, QString mode);
    QStringList taskCreatesourcecat(bool &stop, QString mode);
    QStringList taskAstromphotom(bool &stop, QString mode);
    QStringList taskAbsphotindirect(bool &stop, QString mode);
    QStringList taskGetCatalogFromWEB(bool &stop, QString mode);
    QStringList taskGetCatalogFromIMAGE(bool &stop, QString mode);
    QStringList taskRestoreHeader(bool &stop, QString mode);
    QStringList taskSkysub(bool &stop, QString mode);
    QStringList taskCoaddition(bool &stop, QString mode);
    QStringList taskResolveTarget(bool &stop, QString mode);

    void check_taskHDUreformat(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskProcessbias(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskProcessdark(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskProcessflatoff(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskProcessflat(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskProcessscience(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskChopnod(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskBackground(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskCollapse(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskBinnedpreview(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskGlobalweight(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskIndividualweight(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskSeparate(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskCreatesourcecat(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskAstromphotom(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskAbsphotindirect(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskGetCatalogFromWEB(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskGetCatalogFromIMAGE(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskRestoreHeader(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskSkysub(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskCoaddition(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void check_taskResolveTarget(DataDir datadir, QString taskBasename, bool &stop, bool &skip, QString mode);
    void restoreOriginalData();
    void emitEditingFinished(const QString &arg1);
    void on_actionLicense_triggered();
    void on_actionAcknowledging_triggered();
    void loadCoaddAbsZP(QString coaddImage, instrumentDataType *instData, float maxVal);

private:
    // Variables we need to access frequently
    bool preventLoop_WriteSettings = false;

    QWidget *emptyWidget;
    MyStringListModel *instrument_model;
    Preferences *preferences;
    Instrument *instrument = new Instrument(this);
    instrumentDataType instData;
    MultidirReadme *multidirReadme;
    License *license;
    Acknowledging *acknowledging;
    ErrorDialog *errordialog = new ErrorDialog(this);
    QSettings *settingsp;
    QMap<QCheckBox*,QString> checkboxMap;
    QStringList totalCommandList;
    QStringList cleanCommandList;

    QThread *workerThread;
    MainGUIWorker *mainGUIWorker;

    bool processSkyImages = false;

    // Flags that tell whether an error message has been shown or not.
    bool GAP_DYNAMIC_FOUND_shown = false;
    bool WINDOWSIZE_TOO_LARGE_shown = false;
    bool INSUFFICIENT_BACKGROUND_NUMBER_shown = false;
    bool SKY_FILE_NOT_FOUND_shown = false;
    bool NO_OVERLAP_WITH_SKYAREA_shown = false;
    bool MASTER_BIAS_NOT_FOUND_shown = false;
    bool MASTER_FLAT_NOT_FOUND_shown = false;
    bool MASTER_FLATOFF_NOT_FOUND_shown = false;
    bool MASTER_BIAS_NOT_FOUND_GLOBW_shown = false;
    bool MASTER_FLAT_NOT_FOUND_GLOBW_shown = false;
    bool NO_MJDOBS_FOR_PM_shown = false;
    bool CANNOT_UPDATE_HEADER_WITH_PM_READ_shown = false;
    bool CANNOT_UPDATE_HEADER_WITH_PM_WRITE_shown = false;
    bool CANNOT_UPDATE_HEADER_WITH_PA_shown = false;
    bool CANNOT_WRITE_RESAMPLE_LIST_shown = false;
    bool CANNOT_OPEN_FILE_shown = false;
    bool CANNOT_READ_HEADER_KEYS_shown = false;
    bool DUPLICATE_MJDOBS_shown = false;
    bool IMAGES_NOT_FOUND_shown = false;
    bool IncompatibleSizeRAW_shown = false;
    bool INCONSISTENT_DATA_STATUS_shown = false;

    bool switchProcessMonitorPreference = true;

    bool areAllPathsValid();
    bool checkRightScampMode(QString coordsMode);
    bool checkMultipledirConsistency(QString mode);
    QStringList createCommandlistBlock(QString taskBasename, QStringList goodDirList, bool &stop, QString mode);
    QStringList displayCoaddFilterChoice(QString dirname, QString &filterChoice, QString mode);
    QString estimateStatusFromFilename(DataDir datadir);
    void fill_setupInstrumentComboBox();
    QString getStatusForSettings();
    void handleDataDirs(QStringList &goodDirList, QLineEdit *scienceLineEdit, QLineEdit *calib1LineEdit,
                        QLineEdit *calib2LineEdit, QString statusString, QString mode);
    void hasDirCurrentData(DataDir datadir, bool &stop);
    void initProcessingStatus();
    void initInstrumentData(QString instrumentNameFullPath);
    bool isRefcatRecent(QString dirname);
    void linkPrefInst_with_MainInst(int index);
    QString manualCoordsUpdate(QString science, QString coordsMode);
    QStringList matchCalibToScience(const QStringList scienceList, const QStringList calibList);
    bool maybeSave();
    void on_setupInstrumentComboBox_clicked();
    void populateTaskCommentMap();
    int readPreferenceSettings(QString &projectname);
    int readGUISettings(QString projectname);
    void repaintDataDirs();
    void returnIfRunning(QString message);
    QString sameRefCoords(QString coordsMode);
    void setStatusFromSettings(QString statusString);
    void testOverscan(QVector<int> &overscan);
    void toggleButtonsWhileRunning();
    void updatePreferences();
    void updateInstrumentComboBoxBackgroundColor();
    void updateCommandList(QStringList &commandList, QString taskBasename, QString arg1, QString arg2, int &count, QString scriptname);
    void updateProcessList(QStringList &commandList, QString taskBasename, QString arg1);
    void updateProcessList(QStringList &commandList, QString taskBasename, QString arg1, QString arg2);
    void deleteDummyLineEdits(QLineEdit *le1, QLineEdit *le2);
    void addDockWidgets();
    void wipeTmpDir();
    void resetProcessingErrorFlags();
    bool sufficientSpaceAvailable(long spaceNeeded);
    QString getInstDir(QString instname);
    bool OSPBC_addCommandBlock(const QString taskBasename, const QString mode, bool &stop);
    bool OSPBC_isTaskCurrentlyVisible(QCheckBox *cb);
    QString OSPBC_determineExecutionMode(QObject *sender);
    bool OSPBC_multipleDirConsistencyCheck();
    void displayMessage(QString messagestring, QString type);
    void checkMemoryConstraints();
    void addProgressBars();
    void resetInstrumentData();
    int estimateBinningFactor();
};

// Subclassing QStringListModel to allow certain entries being shown with different colors
class MyStringListModel : public QStringListModel
{
public:
    MyStringListModel()
    {
        ;
    }
    QString instrument_dir;
    QString instrument_userDir;
    QVariant data(const QModelIndex & index, int role) const
    {
        if(!index.isValid())
            return QVariant();

        int row = index.row();

        switch(role)
        {
        case Qt::DisplayRole:
            return this->stringList().at(row);
        case Qt::ForegroundRole:
            QFile file;
            QString name1 = instrument_dir+"/"+stringList().at(row)+".ini";
            QString name2 = instrument_userDir+"/"+stringList().at(row)+".ini";
            // Search until we found the instrument.ini file
            file.setFileName(name1);
            if (!file.exists()) file.setFileName(name2);
            QString type = get_fileparameter(&file, "TYPE");
            if (type == "OPT") return QBrush(QColor("#0000cc"));
            else if (type == "NIR") return QBrush(QColor("#009900"));
            else if (type == "NIRMIR") return QBrush(QColor("#cc5500"));
            else if (type == "MIR") return QBrush(QColor("#ff0000"));
            else return QBrush(QColor("#0000cc"));
        }

        return QVariant();
    }
};

#endif // MAINWINDOW_H
