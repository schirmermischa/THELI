#include "mainguiworker.h"
#include "../processingInternal/controller.h"

#include <QProcess>
#include <QTest>
#include <QString>
#include <QStringList>

MainGUIWorker::MainGUIWorker(Controller *mycontroller, QStringList mycommands, QObject *parent) : Worker(parent)
{
    commands = mycommands;
    controller = mycontroller;
}

void MainGUIWorker::runTask()
{
    //  isWorkerRunning = true;

    // Grab the command list, read the keywords, and process it accordingly
    QString key;
    QString instructions;
    QStringList list;

    // The following loop handles 4 different types of 'key events'

    // yield can be triggered from outside
    for (auto &it : commands) {
        // We need a little wait here (50msec) to allow for the parent process to update "abort"
        // in case the process is aborted by the user.
        QTest::qWait(50);
        if (yield || !controller->successProcessing) {
            emit messageAvailable("Aborted.\n", "stop");
            emit finished();
            break;
        }
        list = it.split("::");
        key = list.at(0);
        // EVENT 1: Used to show which task is currently executed
        if (key == "MESSAGE") {
            int length = list.length();
            // Only emit non-empty messages
            if (length > 1) {
                instructions = list.at(1).simplified();
                emit messageAvailable(instructions+" ... ", "output");
            }
        }
        // EVENT 2: execute the task
        else if (key == "RUN") {
            QString taskBasename = list.at(1);
            instructions = list.at(2).simplified();
            // Set up the controller with the taskname, the arguments, and the parallelization, then run the task
            controller->taskBasename = taskBasename;
            controller->instructions = instructions;
            controller->loadPreferences();
            // update the data tree here?
            controller->runTask();
        }
        // EVENT 3: update the GUI after the task has successfully finished
        else if (key == "UPDATESTATUS") {
            // Needed to show in the GUI which task has just successfully finished;
            QString taskBasename = list.at(1);
            emit updateStatus(taskBasename, true);
        }
        // EVENT 4: show that everything is done.
        else if (key == "END") {
            QString taskBasename = list.at(1);
//            if (taskBasename != "Astromphotom"
//                    && taskBasename != "Coaddition") {
                emit messageAvailable("All tasks finished.\n", "info");
                emit finished();
//            }
/*
            if (taskBasename == "Astromphotom") {
                qDebug() << "MainGUIWorker::runTask(): FIXME";
                emit messageAvailable("All tasks finished.\n", "info");
                emit finished();
            }
*/
        }
    }
}
