#include "swarpworker.h"

#include <QProcess>
#include <QTest>

SwarpWorker::SwarpWorker(QString command, QString dir, QString type, QObject *parent) : Worker(parent)
{
    swarpCommand = command;
    coaddDirName = dir;
    swarpType = type;
//    qDebug() << "SW1" << coaddDirName;
}

void SwarpWorker::runSwarp()
{
    extProcess = new QProcess();
    //    connect(&extProcess, &QProcess::readyReadStandardOutput, this, &Controller::processExternalStdout);
    connect(extProcess, &QProcess::readyReadStandardError, this, &SwarpWorker::processExternalStderr);
    QTest::qWait(300);   // If I don't do this, the GUI crashes. It seems the process produces an output faster than the connection can be made ...
    extProcess->setWorkingDirectory(coaddDirName);
    extProcess->start("/bin/sh -c \""+swarpCommand+"\"");
    extProcess->waitForFinished(-1);

    if (swarpType == "swarpPreparation") emit finishedPreparation();
    if (swarpType == "swarpResampling") emit finishedResampling(threadID);
    if (swarpType == "swarpCoaddition") emit finishedCoaddition();

    emit finished();
    // stdout and stderr channels are slotted into the monitor's plainTextEdit
}

void SwarpWorker::abort()
{
    extProcess->kill();
    emit finished();
}

void SwarpWorker::processExternalStderr()
{
    QProcess *process = qobject_cast<QProcess*>(sender());

    // have to remove the '\x1B[1A' and '\x1B[1M' escape sequences.
    QString stderr(process->readAllStandardError());
    stderr.remove(QRegExp("[^a-zA-Z\\d\\s\\.\\:\\-\\_/]"));
    stderr.remove("1A1M ");
    stderr.remove("1A ");
    stderr.remove("1M ");
    stderr = stderr.simplified();
    stderr.replace(" Center:", "<br>Center:");
    stderr.replace(" Gain:", "<br>Gain:");
    stderr.replace(" Scale:", "<br>Scale:");
    stderr.replace(" Background:", "<br>Background:");
    stderr.replace(" RMS:", "<br>RMS:");
    stderr.replace(" Weight scale:", "<br>Weight scale:");
    stderr.replace("... Looking for ", "<br>Looking for ");
    stderr.replace(" Examining input data ...", "<br>Examining input data");
    stderr.replace(" Creating NEW output image ", "<br>Creating NEW output image ");
    stderr.replace(" Creating NEW weight-map ", "<br>Creating NEW weight-map ");
    stderr.replace(" ----- SWarp ", "<br>----- SWarp ");
    stderr.replace(" ------- Output File coadd.fits:", "<br>------- Output File coadd.fits:");
    stderr.replace(" Flux scaling astrom/photom:", "<br>Flux scaling astrom/photom:");
    stderr.replace(" -------------- File", "<br>-------------- File");
    stderr.remove(QRegExp("^1A"));
    stderr.remove(" 0m ");
    stderr.remove(" 7m ");

    QStringList warnings;
    warnings << "WARNING: Astrometric approximation too inaccurate for this re-projection";
    QStringList errors;
    errors << "Error:";

    QString type = "normal";
    for (auto &warning : warnings) {
        if (stderr.contains(warning)) {
            type = "warning";
            break;
        }
    }
    for (auto &error : errors) {
        if (stderr.contains(error)) {
            type = "error";
            emit errorFound();
            break;
        }
    }
    emit messageAvailable(stderr.simplified(), type);
}
