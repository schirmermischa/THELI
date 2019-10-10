#ifndef SWARPWORKER_H
#define SWARPWORKER_H

#include "worker.h"

#include <QObject>
#include <QProcess>

class SwarpWorker : public Worker
{
    Q_OBJECT

public:
    explicit SwarpWorker(QString command, QString dir, QString type, QObject *parent = nullptr);
    QString swarpCommand;
    QString coaddDirName;
    QString swarpType;
    int threadID;
    QProcess *extProcess;

    void abort();

public slots:
    void runSwarp();

signals:
    void finishedPreparation();
    void errorFound();
    void finishedResampling(int threadID);
    void finishedCoaddition();
    void messageAvailable(QString message, QString type);

private slots:
    void processExternalStderr();
};

#endif // SWARPWORKER_H
