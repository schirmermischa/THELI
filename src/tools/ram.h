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

#ifndef RAM_H
#define RAM_H

#include <QObject>
#include <QFile>
#include <QVector>
#include <QTextStream>

class RAM : public QObject
{
    Q_OBJECT
public:
    explicit RAM(QObject *parent = nullptr);
    ~RAM();

    long getRAMload();
    void getRAMInfo_MAC(unsigned long *pulSystem, unsigned long *pulUser, unsigned long *pulNice, unsigned long *pulIdle);
    float getCurrentValue();
    long totalRAM = 0;

signals:

private:

    bool RAMbarDeactivated = false;
    long availableRAM = 0;

    void readStatsRAM_Linux();
    void readStatsRAM_MAC();

    QFile file;
    QTextStream instream;

    QString kernelType;

//    void init();

public slots:

};

#endif // RAM_H
