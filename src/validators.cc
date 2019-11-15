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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QValidator>
#include <QRegExpValidator>

void MainWindow::validate()
{
    QRegExp rx1( "^\\S+$" );
    QRegExp rx2( "^[A-Z0-9a-z-+._, ]+$" );
    QValidator *validator_string = new QRegExpValidator( rx1, this );
    QValidator *validator_stringdir = new QRegExpValidator( rx2, this );

    ui->setupBiasLineEdit->setValidator( validator_stringdir );
    ui->setupDarkLineEdit->setValidator( validator_stringdir );
    ui->setupFlatLineEdit->setValidator( validator_stringdir );
    ui->setupFlatoffLineEdit->setValidator( validator_stringdir );
    ui->setupMainLineEdit->setValidator( validator_string );
    ui->setupProjectLineEdit->setValidator( validator_string );
    ui->setupScienceLineEdit->setValidator( validator_stringdir );
    ui->setupSkyLineEdit->setValidator( validator_stringdir );
    ui->setupStandardLineEdit->setValidator( validator_stringdir);
}

void MainWindow::connect_validators()
{
    connect(ui->setupBiasLineEdit, &QLineEdit::textChanged, this, &MainWindow::validate);
    connect(ui->setupDarkLineEdit, &QLineEdit::textChanged, this, &MainWindow::validate);
    connect(ui->setupFlatLineEdit, &QLineEdit::textChanged, this, &MainWindow::validate);
    connect(ui->setupFlatoffLineEdit, &QLineEdit::textChanged, this, &MainWindow::validate);
    connect(ui->setupMainLineEdit, &QLineEdit::textChanged, this, &MainWindow::validate);
    connect(ui->setupProjectLineEdit, &QLineEdit::textChanged, this, &MainWindow::validate);
    connect(ui->setupScienceLineEdit, &QLineEdit::textChanged, this, &MainWindow::validate);
    connect(ui->setupSkyLineEdit, &QLineEdit::textChanged, this, &MainWindow::validate);
    connect(ui->setupStandardLineEdit, &QLineEdit::textChanged, this, &MainWindow::validate);
}

