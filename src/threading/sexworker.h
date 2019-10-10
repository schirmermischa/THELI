#ifndef SEXWORKER_H
#define SEXWORKER_H

#include "worker.h"

#include <QObject>

class SexWorker : public Worker
{
    Q_OBJECT

public:
    explicit SexWorker(QString command, QString dir, QObject *parent = nullptr);

    QString sexCommand;
    QString sexDirName;

public slots:
    void runSex();

private slots:
    void processExternalStdout();
    void processExternalStderr();

signals:
    void errorFound();
    void messageAvailable(QString message, QString type);

};

#endif // SEXWORKER_H
