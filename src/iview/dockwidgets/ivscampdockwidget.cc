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

#include "ivscampdockwidget.h"
#include "ui_ivscampdockwidget.h"
#include "../iview.h"

#include <QObject>

IvScampDockWidget::IvScampDockWidget(IView *parent) :
    QDockWidget(parent),
    ui(new Ui::IvScampDockWidget)
{
    ui->setupUi(this);

    iview = parent;
}

IvScampDockWidget::~IvScampDockWidget()
{
    delete ui;
}

void IvScampDockWidget::on_acceptSolution_clicked()
{
    emit solutionAcceptanceState(true);
}

void IvScampDockWidget::on_rejectSolution_clicked()
{
    emit solutionAcceptanceState(false);
}
