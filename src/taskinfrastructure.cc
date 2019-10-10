#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_confdockwidget.h"
#include "functions.h"
#include "status.h"
#include "threading/mainguiworker.h"
#include "processingInternal/data.h"
#include "processingExternal/errordialog.h"
#include "datadir.h"
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QTest>
#include <QMetaObject>
#include <QGenericReturnArgument>

QStringList MainWindow::matchCalibToScience(const QStringList scienceList, const QStringList calibList)
{
    // This utility modifies the calib list so that it matches the science list in length.
    // Example:
    // science list contains S1, S2, S3
    // calib list contains F. Then, at the end it will contain F, F, F
    // If calib list is empty, then at the end it will contain "", "", ""

    int nscience = scienceList.length();
    int ncalib = calibList.length();

    // Initialize the newCalibList with blanks and the same dimension as the scienceList.
    QStringList newCalibList;
    for (int i=0; i<nscience; ++i) {
        newCalibList << "";
    }

    // if the calibList was empty
    if (ncalib == 0) {
        // empty on purpose;
    }
    else if (ncalib == 1) {
        for (int i=0; i<nscience; ++i) {
            newCalibList.operator[](i) = calibList.at(0);
        }
    }
    else if (ncalib == nscience && nscience > 1) {
        for (int i=0; i<nscience; ++i) {
            newCalibList.operator[](i) = calibList.at(i);
        }
    }
    else {
        // Invalid config
        // Error handled in the caller
        return QStringList();
    }
    return newCalibList;
}

void MainWindow::handleDataDirs(QStringList &goodDirList,
                                QLineEdit *scienceLineEdit, QLineEdit *calib1LineEdit,
                                QLineEdit *calib2LineEdit, QString statusString, QString mode)
{
    // This function assigns the correct calibration directories (if any)
    // and status strings (if any) to the science directories.
    // Note that "science" could be a flat, and "calib" could be a bias.

    if (scienceLineEdit->text().isEmpty()) return;

    // Convert data dir(s) to stringlists
    QStringList scienceList = datadir2StringList(scienceLineEdit);
    QStringList calib1List = datadir2StringList(calib1LineEdit);
    QStringList calib2List = datadir2StringList(calib2LineEdit);
    QStringList newCalib1List;
    QStringList newCalib2List;

    // Match the calib list to the science list (increase length if necessary)
    // If a LineEdit is not needed, an empty constructor is used in the caller, resulting in an empty stringList;
    newCalib1List = matchCalibToScience(scienceList, calib1List);
    newCalib2List = matchCalibToScience(scienceList, calib2List);

    // Last consistency check:
    //   if (mode == "execute") {
    if (scienceList.length() != newCalib1List.length()
            || scienceList.length() != newCalib2List.length()) {
        QMessageBox::critical(this, tr("Invalid data dir configuration"),
                              tr("You have defined an incompatible number of calibration and science directories.\n"
                                 "Allowed setups will be displayed next."),
                              QMessageBox::Ok);
        on_setupReadmePushButton_clicked();
        return;
    }
    //   }

    QString main = ui->setupMainLineEdit->text();
    QString scriptArg;
    QString scriptArg_blank;
    QDir scienceDir;

    // Loop over directories, prepend maindir if necessary;
    // The idea is to produce strings "main science calib" separated by blanks,
    // or "main science statusString".
    // We check elsewhere in areAllPathsValid() whether directories are good
    for (int i=0; i<scienceList.size(); ++i) {
        scriptArg = main + "/" + scienceList.at(i);
        scriptArg_blank = main + " " + scienceList.at(i) + " "
                + newCalib1List.at(i) + " "
                + newCalib2List.at(i) + " " +statusString;
        // Clean the strings
        scriptArg = scriptArg.simplified();
        scriptArg_blank = scriptArg_blank.simplified();

        if (QDir(scienceDir).exists()) goodDirList << scriptArg_blank;
    }
}

// Turn the entry/entries of a subdirectory in the data tree to a cleaned stringlist

// This function creates the list of commands for all checked checkboxes.
// It also runs a number of consistency checks.
// The final list of commands is processed if on_startPushButton_clicked().
void MainWindow::updateProcessList(QStringList &commandList, QString taskBasename, QString arg1)
{
    // This will show in the overview window which task is currently running
    QString message = taskCommentMap.value(taskBasename);
    QStringList list = arg1.split(" ");
    QString workingDir = "";
    if (list.length() >= 2) workingDir = list.at(1);
    else workingDir = list.at(0);
    commandList.append("MESSAGE::"+message+" "+workingDir);
    // The actual task
    commandList.append("RUN::"+taskBasename+":: "+arg1);
}

// Like above, but used for processscience, which may operate on science, sky or std
// Also used for create source cat
void MainWindow::updateProcessList(QStringList &commandList, QString taskBasename, QString arg1, QString arg2)
{
    // This will show in the overview window which task is currently running
    QString message = taskCommentMap.value(taskBasename);
    QStringList list = arg1.split(" ");
    QString workingDir = "";
    if (list.length() >= 2) workingDir = list.at(1);
    else workingDir = list.at(0);
    commandList.append("MESSAGE::"+message+" "+workingDir);
    // The actual task
    commandList.append("RUN::"+taskBasename+":: "+arg1+" "+arg2);
}

// This function collects the internal process calls
// NOTE: THIS IS CALLED FOR EACH TASK SEQUENTIALLY
QStringList MainWindow::createCommandlistBlock(QString taskBasename, QStringList goodDirList, bool &stop, QString mode)
{
    // Does the task exist in the checkbox map?
    QString taskName;
    if (checkboxMap.values().contains(taskBasename)) taskName = checkboxMap.key(taskBasename)->text();
    else taskName = "";

    if (goodDirList.isEmpty()) {
        message(ui->plainTextEdit, "STOP: No data were provided for '"+taskName+"' .");
        stop = true;
        return QStringList();
    }

    DataDir datadir;
    QStringList commandList;

    // Process science can act on DT_SCIENCE, SKY, STD and thus needs to distinguish between them
    QString scienceMode = "";
    if (!goodDirList.isEmpty()) {
        QString last = goodDirList.last();
        if (last == "theli_DT_SCIENCE" || last == "theli_DT_SKY" || last == "theli_DT_STANDARDD") {
            scienceMode = last;
            goodDirList.removeLast(); // do not interpret the last string as a data directory; in this case it's a flag
        }
    }

    // Loop over all directories found for the current task
    for (auto &it : goodDirList) {
        datadir.setPaths(it);
        // Now call a consistency check by the function's string representation.
        // The function name is "check_task<taskBasename>()"
        // and it simply updates the 'stop' and 'skip' flags
        bool skip = false;
        bool test = QMetaObject::invokeMethod(this, ("check_task"+taskBasename).toStdString().c_str(),
                                              Qt::DirectConnection,
                                              Q_ARG(DataDir, datadir),
                                              Q_ARG(QString, taskBasename),
                                              Q_ARG(bool &, stop),
                                              Q_ARG(bool &, skip),
                                              Q_ARG(QString, mode));
        if (!test) {
            qDebug() << "QDEBUG: createCommandListBlock: Could not evaluate QMetaObject.";
            stop = true;
            return QStringList();
        }

        // Omit the current task if either skip or stop are true;
        // Several commands can be skipped, but we stop everything (at a later point below) at the first stop.
        if (stop) continue;
        if (skip) continue;

        QStringList tmpdirlist = it.split(" ");
        QString main = tmpdirlist.at(0);
        // Few (1?) exceptions were 'science' is not needed
        QString science = "";

        if (tmpdirlist.length() >= 2) science = tmpdirlist.at(1);

        if (taskBasename == "HDUreformat"
                || taskBasename == "Processbias"
                || taskBasename == "Processdark"
                || taskBasename == "Processflatoff"
                || taskBasename == "Processflat"
                || taskBasename == "Binnedpreview"
                || taskBasename == "Background"
                || taskBasename == "Collapse"
                || taskBasename == "Globalweight"
                || taskBasename == "Individualweight"
                || taskBasename == "Skysub"
                || taskBasename == "ResolveTarget"
                || taskBasename == "GetCatalogFromWEB"
                || taskBasename == "GetCatalogFromIMAGE"
                || taskBasename == "RestoreHeader"
                || taskBasename == "Separate") {
            updateProcessList(commandList, taskBasename, it);
        }

        if (taskBasename == "Processscience") {
            updateProcessList(commandList, taskBasename, it, scienceMode);
        }

        if (taskBasename == "Createsourcecat") {
            QString coordsMode = manualCoordsUpdate(science, mode);
            updateProcessList(commandList, taskBasename, it, coordsMode);
        }

        if (taskBasename == "Astromphotom") {
            // do we need to update the reference catalog?
            if (!isRefcatRecent(main+"/"+science)) {
                if (cdw->ui->ASTmethodComboBox->currentText() == "Scamp") {
                    if (cdw->ui->ARCwebRadioButton->isChecked()) {
                        updateProcessList(commandList, "GetCatalogFromWEB", it);
                    }
                    else {
                        updateProcessList(commandList, "GetCatalogFromIMAGE", it);
                    }
                }
            }

            if (cdw->ui->ASTmethodComboBox->currentText() == "Scamp") {
                if (!cdw->checkRightScampMode(mode)) {
                    message(ui->plainTextEdit, "Aborted, please choose a different MOSAIC_TYPE for scamp.", "stop");
                    stop = true;
                    return QStringList();
                }
            }
            // TODO
            // Put other methods go here: a.net, x-corr, header-astrom

            updateProcessList(commandList, taskBasename, it);
        }

        if (taskBasename == "Coaddition") {
            // Check how many filters we have in the coadd dir
            QString filterChoice;
            QStringList filterlist = displayCoaddFilterChoice(main+"/"+science+"/", filterChoice, mode);
            // Suggest the user to add reference coordinates for a multi-color data set
            if (filterlist.length() > 1) {
                QString coordsMode = sameRefCoords(mode);
                if (coordsMode == "Cancel") {
                    stop = true;
                    return QStringList();
                }
            }

            QString flag = "";
            // Loop over all potential filters
            for (auto &filter : filterlist) {
                QString filterArg = "";
                if (filterChoice == "Separately") {  // for loop will run over each filter
                    filterArg = filter;
                }
                else if (filterChoice == "all") {    // for loop will run exactly once, because we break at the end
                    filterArg = "all";
                }
                else if (filterChoice == filter) {   // for loop will run exactly once, because we break at the end
                    filterArg = filter;
                    flag = "individual";
                }
                else if (filterChoice == "Abort") break;
                if (filterArg.isEmpty()) continue;

                updateProcessList(commandList, taskBasename, it+" "+filterArg);

                if (filterArg == "all" || flag == "individual") break;
            }
        }

        // TODO
        /*
        if (taskBasename == "Chopnod") {
            updateCommandList(commandList, taskBasename, science, it, count, "process_science_chopnod_para.sh");
        }

        if (taskBasename == "Absphotindirect") {
            QString extension = tmpdirlist.at(2);
            QString standard = ui->setupStandardLineEdit->text();
            updateCommandList(commandList, taskBasename, standard, main+" "+standard+" "+extension, count, "create_astromcats_phot_para.sh");
            if (cdw->ui->APIWCSCheckBox->isChecked()) {
                updateCommandList(commandList, taskBasename, standard, main+" "+standard+" "+extension, count, "create_scampcats.sh");
                updateCommandList(commandList, taskBasename, standard, main+" "+standard+" "+extension+" photom", count, "create_scamp.sh");
            }
            updateCommandList(commandList, taskBasename, standard, main+" "+standard+" "+extension, count, "create_stdphotom_prepare_para.sh");
            updateCommandList(commandList, taskBasename, science, main+" "+science+" "+standard+" "+extension, count, "create_abs_photo_info.sh");
            commandList.append("UPDATESOLUTIONS::"+main+"/"+standard);
            // must trigger cdw->updateAPIsolutions()
        }
        */
    }

    commandList.append("UPDATESTATUS::"+taskBasename);

    return commandList;
}

void MainWindow::on_yieldToolButton_clicked()
{
    // There is only sth to abort if the start button is disabled
    controller->userYield = true;
    emit messageAvailable("Yield request received ...", "stop");
    if (!ui->startPushButton->isEnabled()) {
        message(ui->plainTextEdit, "Will stop after current task has finished, please wait ...", "stop");
        mainGUIWorker->yield = true;
        if (!ui->startPushButton->isEnabled()) {
            ui->startPushButton->setEnabled(true);
            ui->startPushButton->setText("Start");
        }
    }
}

void MainWindow::on_stopToolButton_clicked()
{
    // There is only sth to abort if the start button is disabled
    controller->userStop = true;
    emit messageAvailable("Stop request received ...", "stop");
    if (!ui->startPushButton->isEnabled()) {
        message(ui->plainTextEdit, "Stopping current task, please wait ...", "stop");
        // Make sure we are not entering next task in the queue
        mainGUIWorker->yield = true;
        QTest::qWait(50);
        // Pause thread
        mainGUIWorker->pause();
        if (controller->currentData != nullptr
                && controller->taskBasename != "HDUreformat") {
            // reformatting falls through because of userAbort flag
            controller->currentData->setSuccess(false);
        }
        mainGUIWorker->resume();
        if (controller->taskBasename == "Astromphotom") {
            emit messageAvailable("Sending Scamp a kill signal ...", "stop");
            controller->scampWorker->abort();
            controller->workerThread->quit();
            controller->workerThread->wait();
            emit messageAvailable("Successfully detached from Scamp", "stop");
        }
        if (controller->taskBasename == "Coaddition") {
            emit messageAvailable("Sending Swarp a kill signal ...", "stop");
            controller->swarpWorker->abort();
            controller->workerThread->quit();
            controller->workerThread->wait();
            emit messageAvailable("Successfully detached from Swarp", "stop");
        }
        workerThread->quit();
        workerThread->wait();
        if (controller->taskBasename == "Processbias"
                || controller->taskBasename == "Processdark"
                || controller->taskBasename == "Processflatoff"
                || controller->taskBasename == "Processflat"
                || controller->taskBasename == "Binnedpreview"
                || controller->taskBasename == "Globalweight"
                || controller->taskBasename == "Individualweight"
                || controller->taskBasename == "Createsourcecat"
                || controller->taskBasename == "Astromphotom"
                || controller->taskBasename == "Coaddition") {
            emit messageAvailable("Hard abort: " +  controller->taskBasename, "stop");
            emit messageAvailable("Manual clean-up is not required, simply re-run the task for a clean state.", "note");
        }
        else {
            emit messageAvailable("<br>*****************************************************************", "stop");
            emit messageAvailable("Hard abort: " +  controller->taskBasename, "stop");
            emit messageAvailable("Files in " + controller->currentDirName + " likely need manual clean-up.", "stop");
            emit messageAvailable("*****************************************************************", "stop");
        }
        if (!ui->startPushButton->isEnabled()) {
            ui->startPushButton->setEnabled(true);
            ui->startPushButton->setText("Start");
        }
    }
}

void MainWindow::on_actionKill_triggered()
{
    // There is only sth to abort if the start button is disabled
    controller->userKill = true;
    emit messageAvailable("Abort request received ...", "stop");
    if (!ui->startPushButton->isEnabled()) {
        message(ui->plainTextEdit, "Kill signal sent, please wait ...", "stop");
        if (controller->taskBasename == "Astromphotom") {
            emit messageAvailable("Sending Scamp a kill signal ...", "stop");
            controller->scampWorker->abort();
            controller->workerThread->quit();
            controller->workerThread->wait();
            emit messageAvailable("Successfully detached from Scamp", "stop");
        }
        if (controller->taskBasename == "Coaddition") {
            emit messageAvailable("Sending Swarp a kill signal ...", "stop");
            controller->swarpWorker->abort();
            controller->workerThread->quit();
            controller->workerThread->wait();
            emit messageAvailable("Successfully detached from Swarp", "stop");
        }
        workerThread->terminate();
        workerThread->wait();
        message(ui->plainTextEdit, "Aborted.", "stop");
        if (!ui->startPushButton->isEnabled()) {
            ui->startPushButton->setEnabled(true);
            ui->startPushButton->setText("Start");
        }
        if (controller->taskBasename == "Processbias"
                || controller->taskBasename == "Processdark"
                || controller->taskBasename == "Processflatoff"
                || controller->taskBasename == "Processflat"
                || controller->taskBasename == "Binnedpreview"
                || controller->taskBasename == "Globalweight"
                || controller->taskBasename == "Individualweight"
                || controller->taskBasename == "Createsourcecat"
                || controller->taskBasename == "Astromphotom"
                || controller->taskBasename == "Coaddition") {
            emit messageAvailable("Task killed: " +  controller->taskBasename, "warning");
            emit messageAvailable("Manual clean-up is not required, simply re-run the task for a clean state.", "note");
        }
        emit messageAvailable("Task killed: " +  controller->taskBasename, "stop");
        emit messageAvailable("<br>*****************************************************************", "stop");
        emit messageAvailable("Files in " + controller->currentDirName + " need manual clean-up.", "stop");
        emit messageAvailable("Data model in RAM in undefined state, recommend THELI restart.", "stop");
        emit messageAvailable("*****************************************************************", "stop");
    }
}

void MainWindow::startPushButton_clicked_dummy(QString string)
{
    on_startPushButton_clicked();
}

// This top level function has two modes: a "simulator" mode, executed when task checkboxes are checked or
// unchecked, or data dirs edited successfully. It goes through all the tests and prints notifications in plainTextEdit,
// but it does not execute anything. Only when the actual start button is clicked, the function goes through with the execution
// but does not print any uncritical information anymore
void MainWindow::on_startPushButton_clicked()
{
    if (!doingInitialLaunch) {
        controller->progress = 0.;
    }
    QSettings settings("THELI", "PREFERENCES");
    processSkyImages = settings.value("prefProcessSkyCheckBox",false).toBool();

    // Do not do anything while tasks are running already
    if (!ui->startPushButton->isEnabled()) return;

    // Do not simulate during startup (excessive (?) parseing of the directory tree)
    //    if (doingInitialLaunch || readingSettings) return;

    // The current execution mode
    QString mode = OSPBC_determineExecutionMode(sender());

    // Reset error message box flags
    resetProcessingErrorFlags();

    // Check whether the directory tree is consistent
    if (!OSPBC_multipleDirConsistencyCheck()) return;

    // Check if all data Dirs are valid. If there is a single one that isn't, abort!
    // Exception: mode = ResolveTarget does not require any data directories
    if (! areAllPathsValid()
            && (mode != "ResolveTarget")
            && !doingInitialLaunch) {
        message(ui->plainTextEdit, "STOP: Nonexistent entries were found in the data directory tree. "
                                   "They must be fixed or removed.");
        ui->processingTabWidget->setCurrentIndex(0);
        return;
    }

    // Save the setup in case the GUI crashes
    writeGUISettings();

    // The following list contains all script commands that would be executed
    totalCommandList.clear();

    bool stop = false;
    QString taskBasename;
    // Process commands from other buttons than the "start" button
    if (mode == "GetCatalogFromWEB"
            || mode == "GetCatalogFromIMAGE"
            || mode == "ResolveTarget"
            || mode == "RestoreHeader") {
        taskBasename = mode;
        if (!OSPBC_addCommandBlock(taskBasename, mode, stop)) return;   // updates totalCommandList
    }
    else {
        // process commands from the start button (must loop over task checkboxes)
        for (auto &it : status.listCheckBox) {
            // Include only tasks visible on the currently visible stacked widget page
            //            if (it->isChecked() && isTaskCurrentlyVisible(it)) {
            if (it->isChecked()) {
                taskBasename = it->objectName().remove("apply").remove("CheckBox");
                if (!OSPBC_addCommandBlock(taskBasename, mode, stop)) return;   // updates totalCommandList
            }
        }
    }
    totalCommandList.append("END::"+taskBasename);

    // If we are not in simulator mode then do the real thing
    if (mode != "simulate") {
        bool anythingChecked = false;
        int numTasksChecked = 0;
        for (auto &it : status.listCheckBox) {
            if (it->isChecked()) {
                ++numTasksChecked;
                anythingChecked = true;
            }
        }
        // Leave if no normal task is checked 9apart from special buttons)
        if (mode != "GetCatalogFromWEB"
                && mode != "GetCatalogFromIMAGE"
                && mode != "ResolveTarget"
                && mode != "RestoreHeader"
                && !anythingChecked) {
            return;
        }
        // Reset the controller's processing status
        controller->successProcessing = true;
        controller->userYield = false;
        controller->userStop = false;
        controller->userKill = false;
        controller->abortProcess = false;
        controller->swapWarningShown = false;
        ui->startPushButton->setText("Running ...");
        ui->startPushButton->setDisabled(true);
        for (auto &it : status.listDataDirs) {
            it->setDisabled(true);
        }

        cdw->ui->confStackedWidget->setDisabled(true);

        emit runningStatusChanged(true);
        toggleButtonsWhileRunning();

        workerThread = new QThread();
        mainGUIWorker = new MainGUIWorker(controller, totalCommandList);
        //workerInit = true;
        //workerThreadInit = true;
        mainGUIWorker->moveToThread(workerThread);
        connect(workerThread, &QThread::started, mainGUIWorker, &MainGUIWorker::runTask);
        connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater, Qt::DirectConnection);
        connect(mainGUIWorker, &MainGUIWorker::finished, this, &MainWindow::taskFinished);
        connect(mainGUIWorker, &MainGUIWorker::updateStatus, &status, &Status::updateStatusReceived);
        connect(mainGUIWorker, &MainGUIWorker::messageAvailable, this, &MainWindow::displayMessage);
        connect(mainGUIWorker, &MainGUIWorker::finished, workerThread, &QThread::quit, Qt::DirectConnection);
        connect(mainGUIWorker, &MainGUIWorker::finished, mainGUIWorker, &QObject::deleteLater, Qt::DirectConnection);

        // if  an error is encountered during scanning, we must end the worker thread
        workerThread->start();

        // show the process monitor, for normal processing tasks, only
        if (taskBasename != "ResolveTarget"
                && taskBasename != "GetCatalogFromIMAGE"
                && taskBasename != "GetCatalogFromWEB") {
            if (switchProcessMonitorPreference) monitor->raise();
            ui->processProgressBar->setValue(0);
        }
        // Speed up the CPU timer
        controller->cpuTimer->setInterval(500);
    }
    else {
        bool anythingChecked = false;
        int numTasksChecked = 0;
        for (auto &it : status.listCheckBox) {
            if (it->isChecked()) {
                ++numTasksChecked;
                anythingChecked = true;
            }
        }
        if (!anythingChecked) {
            message(ui->plainTextEdit, "Currently no activated tasks.", "info");
        }
        else {
            if (! stop) {
                if (totalCommandList.length() == 1) message(ui->plainTextEdit, "Nothing to be done. Selected tasks already executed.", "info");
                else {
                    if (numTasksChecked == 1) message(ui->plainTextEdit, QString::number(numTasksChecked) + " task activated. Ready !", "info");
                    else message(ui->plainTextEdit, QString::number(numTasksChecked) + " tasks activated. Ready !", "info");
                }

            }
        }
    }
}

void MainWindow::displayMessage(QString messagestring, QString type)
{
    message(ui->plainTextEdit, messagestring, type);
}

bool MainWindow::OSPBC_addCommandBlock(const QString taskBasename, const QString mode, bool &stop)
{
    QStringList commandblock;
    // call the function by its string representation (needs a const char *)
    bool test = QMetaObject::invokeMethod(this, ("task"+taskBasename).toStdString().c_str(),
                                          Qt::DirectConnection,
                                          Q_RETURN_ARG(QStringList, commandblock),
                                          Q_ARG(bool &, stop),
                                          Q_ARG(QString, mode));
    if (!test) {
        qDebug() << "QDEBUG: on_startPushButton_clicked: Could not evaluate QMetaObject.";
        return false;
    }
    // Check if the task identified a condition (which it will print to ui->plainTextEdit)
    // that should prevent execution (e.g. insufficient number of exposures, etc).
    if (stop && mode != "simulate") {
        return false;
    }

    // Add the actual processing commands (script calls) to a string list
    totalCommandList.append(commandblock);

    return true;
}

// Check if a task is on the currently displayed stack widget page
bool MainWindow::OSPBC_isTaskCurrentlyVisible(QCheckBox *cb)
{
    if (ui->processingTabWidget->currentIndex() == 0) return false;
    else if (ui->processingTabWidget->currentIndex() == 1) {
        if (cb == ui->applyHDUreformatCheckBox
                || cb == ui->applyProcessbiasCheckBox
                || cb == ui->applyProcessdarkCheckBox
                || cb == ui->applyProcessflatoffCheckBox
                || cb == ui->applyProcessflatCheckBox
                || cb == ui->applyProcessscienceCheckBox
                || cb == ui->applyChopnodCheckBox
                || cb == ui->applyBackgroundCheckBox
                || cb == ui->applyCollapseCheckBox
                || cb == ui->applyBinnedpreviewCheckBox) return true;
        else return false;
    }
    else if (ui->processingTabWidget->currentIndex() == 2) {
        if (cb == ui->applyGlobalweightCheckBox
                || cb == ui->applyIndividualweightCheckBox
                || cb == ui->applySeparateCheckBox
                || cb == ui->applyAbsphotindirectCheckBox
                || cb == ui->applyCreatesourcecatCheckBox
                || cb == ui->applyAstromphotomCheckBox
                || cb == ui->applyStarflatCheckBox
                || cb == ui->applySkysubCheckBox
                || cb == ui->applyCoadditionCheckBox) return true;
        else return false;
    }
    else {
        // never evaluated
        return false;
    }
}

// Which mode are we in
QString MainWindow::OSPBC_determineExecutionMode(QObject *sender)
{
    QString mode;
    if (sender == ui->startPushButton) mode = "execute";
    else if (sender == cdw->ui->ARCgetcatalogPushButton) {
        if (cdw->ui->ARCwebRadioButton->isChecked()) mode = "GetCatalogFromWEB";
        if (cdw->ui->ARCimageRadioButton->isChecked()) mode = "GetCatalogFromIMAGE";
    }
    else if (sender == cdw->ui->restoreHeaderPushButton) mode = "RestoreHeader";
    else if (sender == cdw->ui->ARCtargetresolverToolButton) mode = "ResolveTarget";
    else {
        mode = "simulate";
        ui->plainTextEdit->clear();
    }

    return mode;
}

bool MainWindow::OSPBC_multipleDirConsistencyCheck()
{
    // CONSISTENCY CHECK concerning multiple directories
    // Loop over initial checkboxes (0-8, 6&7 don't matter)
    // Hardcoded, needs to be adjusted when more tasks are introduced before applyBackgroundCheckBox
    int i = 0;
    bool test = true;
    for (auto &it : status.listCheckBox) {
        if (it->isChecked()) {
            QString taskBasename = it->objectName().remove("apply").remove("CheckBox");
            test *= checkMultipledirConsistency(taskBasename);
        }
        ++i;
        // Don't have to evaluate more checkboxes.
        if (i >= 9) break;
    }
    // If this evaluates to false, then at least one part of the data directory tree is inconsistent.
    // In this case warning messages will have been printed to plainTextEdit already by
    // checkMultipledirConsistency()
    // We just need to show the dialog with the explanation.
    if (!test) {
        on_setupReadmePushButton_clicked();
        return false;
    }
    else return true;
}

//UNUSED
void MainWindow::appendOK()
{
    ui->plainTextEdit->moveCursor (QTextCursor::End);
    ui->plainTextEdit->insertPlainText("OK");
    ui->plainTextEdit->moveCursor (QTextCursor::End);
}

void MainWindow::taskFinished()
{
    cdw->ui->confStackedWidget->setEnabled(true);

    if (controller->userYield) {
        emit messageAvailable("Task '"+controller->taskBasename + "' finished.\nSoft abort. ", "stop");
        emit messageAvailable(controller->currentDirName + " in clean state, no manual clean-up required.", "note");
    }
    ui->startPushButton->setEnabled(true);
    ui->startPushButton->setText("Start");
    for (auto &it : status.listDataDirs) {
        it->setEnabled(true);
    }

    monitor->displayMessage("<br>     *** DONE ***", "note");

    emit runningStatusChanged(false);
    // enable various buttons again
    toggleButtonsWhileRunning();
    // Reset all abort flags to false in the current Data class if the task was aborted.
    // Not when we still do splitting, because Data classes have not been instantiated
    if (controller->currentData == nullptr) return;

    if (controller->userYield
            || controller->userStop
            || controller->userKill) {
        controller->currentData->resetUserAbort();
    }
    // Allow reprocessing after hard aborts
    if (controller->userStop
            || controller->userKill) {
        controller->currentData->resetSuccessProcessing();
    }

    // slow down CPU timer
    controller->cpuTimer->setInterval(2000);
}

bool MainWindow::isRefcatRecent(QString dirname)
{
    QFile file(dirname+"/cat/refcat/.refcatID");
    QFile catalog(dirname+"/cat/refcat/theli_mystd.scamp");
    if (!file.exists()) return false;
    if (!catalog.exists()) return false;

    QString id;
    QString currentId;

    if ( !file.open(QIODevice::ReadOnly)) {
        qDebug() << "QDEBUG: isRefcatRecent(): "+dirname+".refcatID could not be opened.";
        return false;
    }
    else {
        // extract the ID string
        QTextStream stream( &file );
        id = stream.readLine();
        id = id.simplified();
        file.close();
        if (cdw->ui->ARCwebRadioButton->isChecked()) {
            QString catalog = cdw->ui->ARCcatalogComboBox->currentText();
            QString ra = cdw->ui->ARCraLineEdit->text();
            QString dec = cdw->ui->ARCdecLineEdit->text();
            QString minmag = cdw->ui->ARCminmagLineEdit->text();
            if (minmag.isEmpty()) minmag = cdw->defaultMap["ARCminmagLineEdit"];
            QString radius = cdw->ui->ARCradiusLineEdit->text();
            currentId = catalog+"_"+ra+"_"+dec+"_"+minmag+"_"+radius;
            if (catalog.contains("GAIA")) currentId.append("_"+cdw->ui->ARCmaxpmLineEdit->text());
        }
        else {
            QString image = cdw->ui->ARCselectimageLineEdit->text();
            QString dt = cdw->ui->ARCDTLineEdit->text();
            QString dmin = cdw->ui->ARCDMINLineEdit->text();
            QString deblend = cdw->ui->ARCmincontLineEdit->text();
            currentId = image+"_"+dt+"_"+dmin+"_"+deblend;
        }
        if (id != currentId) return false;
        else return true;
    }
}

void MainWindow::processMessage(QString text, QString type)
{
    message(ui->plainTextEdit, text, type);
}

QString MainWindow::manualCoordsUpdate(QString science, QString mode)
{
    if (mode != "execute") return "empty";
    if (cdw->ui->ARCraLineEdit->text() != "" && cdw->ui->ARCdecLineEdit->text() != "") {
        QMessageBox msgBox;
        msgBox.setText(science+": Overwrite CRVAL1/2 and CDi_j keywords?");
        msgBox.setInformativeText("Manual coordinates were provided. "
                                  "Do you want to overwrite the CRVAL1/2 "
                                  "keywords in the FITS headers with the values for RA and DEC entered above?\n"
                                  "Optionally, you can also force the CD matrix to have North up and East left.\n\n");
        QAbstractButton *pButtonCrval = msgBox.addButton(tr("Update Ra/DEC"), QMessageBox::YesRole);
        QAbstractButton *pButtonCrvalCD = msgBox.addButton(tr("Update RA/DEC, reset CD matrix"), QMessageBox::YesRole);
        QAbstractButton *pButtonUnchanged = msgBox.addButton(tr("Leave header unchanged"), QMessageBox::YesRole);
        QAbstractButton *pButtonCancel = msgBox.addButton(tr("Cancel"), QMessageBox::YesRole);
        msgBox.exec();
        if (msgBox.clickedButton()==pButtonCrval) return "crval";
        else if (msgBox.clickedButton()==pButtonCrvalCD) return "crval+cd";
        else if (msgBox.clickedButton()==pButtonUnchanged) return "empty";
        else return "Cancel";
    }
    else return "empty";
}

QString MainWindow::sameRefCoords(QString coordsMode) {
    if (coordsMode != "execute") return "";
    if (cdw->ui->COAraLineEdit->text() == "" || cdw->ui->COAdecLineEdit->text() == "") {
        QMessageBox msgBox;
        msgBox.setText("No reference coordinates provided for multi-color data set");
        msgBox.setInformativeText("Several coadditions will be performed for a multi-color data set. "
                                  "It is highly recommended that you use identical RA/DEC reference coordinates for the "
                                  "projection of each coaddition. Images can then be automatically cropped such that "
                                  "the same object appears on the same pixel in all coadded images.\n\n");
        QAbstractButton* pButtonContinue = msgBox.addButton(tr("Continue as is"), QMessageBox::YesRole);
        QAbstractButton* pButtonCancel = msgBox.addButton(tr("Cancel"), QMessageBox::YesRole);
        msgBox.exec();
        if (msgBox.clickedButton()==pButtonContinue) return "Continue";
        else return "Cancel";
    }
    else return "";
}

QStringList MainWindow::displayCoaddFilterChoice(QString dirname, QString &filterChoice, QString mode)
{
    //    QStringList filterList = controller->getFilterList(dirname);

    // We look in normal images, then in skysub images for filter string
    QDir dir(dirname);
    QStringList filter("*_1*.fits");
    dir.setNameFilters(filter);
    dir.setSorting(QDir::Name);
    QStringList list = dir.entryList();
    if (list.isEmpty()) {
        emit messageAvailable("MainWindow::displayCoaddFilterChoice(): No files found for coaddition!", "warning");
        filterChoice = "all";
    }

    QStringList filterList;
    for ( auto &fits : list) {
        int status = 0;
        fitsfile *fptr = nullptr;
        char filter[80];
        fits_open_file(&fptr, (dirname+"/"+fits).toUtf8().data(), READONLY, &status);
        fits_read_key_str(fptr, "FILTER", filter, NULL, &status);
        fits_close_file(fptr, &status);
        QString filterString(filter);
        filterString = filterString.simplified();
        if (!filterList.contains(filterString)) filterList.append(filterString);
    }

    int nfilt = filterList.length();
    if (nfilt == 0) {
        emit messageAvailable("MainWindow::displayCoaddFilterChoice(): No filter keyword found in coadd image list!", "warning");
        filterChoice = "all";
    }
    else if (nfilt == 1) {
        filterChoice = filterList.at(0);
    }
    else {
        if (mode == "simulate") {
            // do not show dialog. use default:
            filterChoice = "Separately";
        }
        else {
            // nfilt > 1
            QMessageBox msgBox;
            msgBox.setText("Different filters are available for coaddition in\n "+dirname);
            msgBox.setInformativeText("Choose a method below to coadd these filters :\n");
            QAbstractButton *pAllSeparately = msgBox.addButton(tr("Separately"), QMessageBox::YesRole);
            pAllSeparately->setToolTip("Create separate coadded images for each filter");
            QAbstractButton *pAllTogether = msgBox.addButton(tr("Together"), QMessageBox::YesRole);
            pAllTogether->setToolTip("Combine images from all filters into a single coadded image");
            for (auto &it : filterList) {
                QAbstractButton *button = msgBox.addButton(it, QMessageBox::YesRole);
                button->setToolTip("Coadd this filter, only");
            }
            QAbstractButton *pFend = msgBox.addButton(tr("Abort"), QMessageBox::NoRole);
            msgBox.exec();
            // WARNING! This is broken in some KDE versions because KDE inserts the & char for the shortcut into the string returned by text().
            // HUGE screwup. Should never mess with GUI at this level
            filterChoice = msgBox.clickedButton()->text();
            if (filterChoice == "Together") filterChoice = "all";
        }
    }
    return filterList;
}
