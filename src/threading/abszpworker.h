#ifndef ABSZPWORKER_H
#define ABSZPWORKER_H

#include "../abszp/abszeropoint.h"
#include "worker.h"

#include <QObject>

class AbsZeroPoint;

class AbszpWorker : public Worker
{
    Q_OBJECT

public:
    explicit AbszpWorker(AbsZeroPoint *abszeropoint, QObject *parent = nullptr);

    QString abszpCommand;
    QString abszpDirName;
    AbsZeroPoint *absZeroPoint;

    bool yield = false;

public slots:
    void runTask();

private slots:
    void processExternalStdout();
    void processExternalStderr();

signals:

};

#endif // ABSZPWORKER_H
