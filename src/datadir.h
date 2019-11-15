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

#ifndef DATADIR_H
#define DATADIR_H

#include <QString>
#include <QDir>

class DataDir
{
public:
    DataDir(QString data, QString main = "");
    DataDir();

    QString name;  // the full path name
    QString subdirname;  // the data subdir name alone
    QDir dir;
    bool exists();
    long numFITS;
    long numCHIP1;
    bool hasMaster();
    bool isEmpty();
    bool hasType(QString type);
    long numEXT(QString type);
    void setPaths(QString data, QString main = "");
};

#endif // DATADIR_H
