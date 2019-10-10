#include "abszpworker.h"
#include "../abszp/abszeropoint.h"

#include <QProcess>
#include <QTest>

AbszpWorker::AbszpWorker(AbsZeroPoint *abszeropoint, QObject *parent) : Worker(parent)
{
//    abszpCommand = command;
//    abszpDirName = dir;
    absZeroPoint = abszeropoint;
}

void AbszpWorker::runTask()
{
    QTest::qWait(50);
    if (yield) {
        emit finished();
        return;
    }
    absZeroPoint->taskInternalAbszeropoint();

    emit finished();
    // stdout and stderr channels are slotted into the monitor's plainTextEdit
}

void AbszpWorker::processExternalStdout()
{
    QProcess *process = qobject_cast<QProcess*>(sender());
    QString stdout(process->readAllStandardOutput());
    //    QString stdout(externalProcess->readLine());
    stdout.remove(QRegExp("[^a-zA-Z.:-\\d\\s]"));
    emit messageAvailable(stdout.simplified(), "normal");
}

void AbszpWorker::processExternalStderr()
{
    QProcess *process = qobject_cast<QProcess*>(sender());
    QString stderr(process->readAllStandardError());
    stderr.remove(QRegExp("[^a-zA-Z\\d\\s]"));
    emit messageAvailable(stderr.simplified(), "normal");
}
