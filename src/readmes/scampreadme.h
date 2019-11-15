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

#ifndef SCAMPREADME_H
#define SCAMPREADME_H

#include <QDialog>

namespace Ui {
class ScampReadme;
}

class ScampReadme : public QDialog
{
    Q_OBJECT

public:
    explicit ScampReadme(QWidget *parent = nullptr);
    ~ScampReadme();

private slots:

private:
    Ui::ScampReadme *ui;
};

#endif // SCAMPREADME_H
