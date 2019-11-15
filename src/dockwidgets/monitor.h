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

#ifndef MONITOR_H
#define MONITOR_H

#include <QDockWidget>
#include <QMutex>

namespace Ui {
class Monitor;
}

class Monitor : public QDockWidget
{
    Q_OBJECT

public:
    explicit Monitor(QWidget *parent = 0);
    ~Monitor();

    Ui::Monitor *ui;
    QMutex mutex;

public slots:
    void displayMessage(QString message, QString type);
    void appendOK();

private:

};

#endif // MONITOR_H
