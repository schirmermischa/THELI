#include "worker.h"

Worker:: Worker(QObject *parent)
    : QObject(parent)
{
}

void Worker::pause()
{
    auto const dispatcher = QThread::currentThread()->eventDispatcher();
    if (!dispatcher) {
//        qDebug() << "thread with no dispatcher";
        return;
    }

    if (state != RUNNING)
        return;

    state = PAUSED;
//    qDebug() << "paused";
    do {
        dispatcher->processEvents(QEventLoop::WaitForMoreEvents);
    } while (state == PAUSED);
}

void Worker::resume()
{
    if (state == PAUSED) {
        state = RUNNING;
//        qDebug() << "resumed";
    }
}

void Worker::cancel()
{
    if (state != IDLE) {
        state = IDLE;
//        qDebug() << "cancelled";
    }
}

bool Worker::isCancelled()
{
    auto const dispatcher = QThread::currentThread()->eventDispatcher();
    if (!dispatcher) {
//        qDebug() << "thread with no dispatcher";
        return false;
    }
    dispatcher->processEvents(QEventLoop::AllEvents);
    return state == IDLE;
}
