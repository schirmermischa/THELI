#ifndef SCAMPWORKER_H
#define SCAMPWORKER_H

#include "worker.h"

#include <QObject>
#include <QProcess>

class ScampWorker : public Worker
{
    Q_OBJECT

public:
    explicit ScampWorker(QString command, QString dir, QString shortname, QObject *parent = nullptr);

    QString scampCommand;
    QString scampDirName;
    QString shortName;
    QProcess *extProcess;

    void abort();

public slots:
    void runScamp();

private slots:
//    void processExternalStdout();
    void processExternalStderr();

signals:
    void errorFound();
    void fieldMatched();
    void finishedScamp();
    void messageAvailable(QString message, QString type);

};

#endif // SCAMPWORKER_H
