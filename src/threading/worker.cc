/*
Copyright (C) 2019 Mischa Schirmer

This file is part of THELI.

THELI is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program in the LICENSE file.
If not, see https://www.gnu.org/licenses/ .
*/

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
