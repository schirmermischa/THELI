#ifndef MAINGUIWORKER_H
#define MAINGUIWORKER_H

#include "worker.h"
#include "../processingInternal/controller.h"

#include <QObject>

class MainGUIWorker : public Worker
{
    Q_OBJECT

public:
    explicit MainGUIWorker(Controller *mycontroller, QStringList mycommands, QObject *parent = nullptr);

    QStringList commands;
    Controller *controller;

    bool yield = false;

public slots:
    void runTask();

private slots:

signals:
    void updateStatus(QString taskBasename, bool success);
};

#endif // MAINGUIWORKER_H
