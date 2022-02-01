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

#ifndef TUTORIALS_H
#define TUTORIALS_H

#include <QDialog>

namespace Ui {
class Tutorials;
}

class Tutorials : public QDialog
{
    Q_OBJECT

public:
    explicit Tutorials(QWidget *parent = 0);
    ~Tutorials();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Tutorials *ui;
};

#endif // TUTORIALS_H
