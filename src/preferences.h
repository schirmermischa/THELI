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

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QFont>

namespace Ui {
class Preferences;
}

class Preferences : public QDialog
{
    Q_OBJECT

signals:
    int diskspacewarnChanged(int);
    int fontSizeChanged(int);
    int fontChanged(QFont);
    int instPreferenceChanged(int);
    int numcpuChanged(int);
    int serverChanged(QString);
    int memoryUsageChanged(bool);
    int intermediateDataChanged(QString);
    void verbosityLevelChanged(int index);
    void preferencesUpdated();
    int switchProcessMonitorChanged(bool);
    void messageAvailable(QString message, QString type);
    void warning();

public:
    explicit Preferences(bool running, QWidget *parent = nullptr);
    ~Preferences();

private slots:
    void on_prefFontdialogPushButton_clicked();
    void on_prefFontsizeSpinBox_valueChanged(int arg1);
    void on_prefDefaultFontPushButton_clicked();
    void on_prefDiskspacewarnSpinBox_valueChanged(int arg1);
    void on_prefServerComboBox_activated(const QString &arg1);
    void on_prefCPUSpinBox_valueChanged(int arg1);
    void on_prefIOthreadsSpinBox_valueChanged(int arg1);
    void on_prefMemoryCheckBox_clicked();
    void on_prefIntermediateDataComboBox_currentTextChanged(const QString &arg1);
    void on_prefVerbosityComboBox_currentIndexChanged(int index);
    void on_prefSwitchProcessMonitorCheckBox_clicked();
    void on_prefCancelButton_clicked();
    void on_prefCloseButton_clicked();

public slots:
    void receiveDefaultFont(QFont);
    void updateParallelization(bool running);

private:
    Ui::Preferences *ui;
    QFont defaultFont;
    float totalMemory;
    int maxMemoryUsed;
    void applyStyleSheets();
    void configureMemory();
    int readSettings();
    int writeSettings();
};

#endif // PREFERENCES_H
