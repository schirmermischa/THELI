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

#ifndef IVSCAMPDOCKWIDGET_H
#define IVSCAMPDOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
class IvScampDockWidget;
}

class IView;  // Forward declaration to access members

class IvScampDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit IvScampDockWidget(IView *parent = 0);
    ~IvScampDockWidget();

    IView *iview;

private:
    Ui::IvScampDockWidget *ui;

private slots:
    void on_acceptSolution_clicked();
    void on_rejectSolution_clicked();

signals:
    void solutionAcceptanceState(bool state);

};

#endif // IVSCAMPDOCKWIDGET_H
