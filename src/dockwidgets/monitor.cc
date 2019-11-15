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

#include "monitor.h"
#include "ui_monitor.h"
#include "functions.h"

#include <QString>
#include <QDebug>

Monitor::Monitor(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::Monitor)
{
    ui->setupUi(this);
}

Monitor::~Monitor()
{
    delete ui;
}

void Monitor::displayMessage(QString text, QString type)
{
    mutex.lock();
    message(ui->monitorPlainTextEdit, text, type);
    mutex.unlock();
}

void Monitor::appendOK()
{
    ui->monitorPlainTextEdit->moveCursor(QTextCursor::End);
    ui->monitorPlainTextEdit->insertPlainText(" OK");
    ui->monitorPlainTextEdit->moveCursor(QTextCursor::End);
}
