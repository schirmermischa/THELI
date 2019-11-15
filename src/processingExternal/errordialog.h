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

#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QDialog>

namespace Ui {
class ErrorDialog;
}

class ErrorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorDialog(QWidget *parent = nullptr);
    ~ErrorDialog();
    QString code;
    QString title;
    QString explanation;
    QString suggestion;
    QString logname;
    QString line;
    void translate();
    void getErrorLine(QString code);
    void update();

private slots:
    void on_errorShowPushButton_clicked();

    void on_errorLogfileLineEdit_textChanged(const QString &arg1);

private:
    Ui::ErrorDialog *ui;
    QString editorPreference;
    int readSettings();
};

#endif // ERRORDIALOG_H
