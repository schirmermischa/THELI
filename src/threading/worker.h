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
