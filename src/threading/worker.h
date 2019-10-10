#ifndef WORKER_H
#define WORKER_H

#include <QAbstractEventDispatcher>
#include <QObject>
#include <QThread>
#include <QDebug>

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject *parent = 0);

signals:
    void started();
    void finished();
    void messageAvailable(QString message, QString type);

public slots:
    void pause();
    void resume();
    void cancel();

protected:

    enum State { IDLE, RUNNING, PAUSED };
    State state = IDLE;

    bool isCancelled();
};

#endif // WORKER_H
