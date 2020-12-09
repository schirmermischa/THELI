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

#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <QDialog>
#include <QLineEdit>
#include <QButtonGroup>
#include <QTextStream>
#include <QStatusBar>

#include <QMainWindow>

namespace Ui {
class Instrument;
}

class Instrument : public QMainWindow
{
    Q_OBJECT

public:
    explicit Instrument(QWidget *parent = nullptr);
    ~Instrument();

private slots:
    void paintNumLineEdits(QString geometry);
    void on_numchipSpinBox_valueChanged(int arg1);
    void validate(QString arg1);
    void on_saveConfigPushButton_clicked();
    void on_instrumentTypeComboBox_currentIndexChanged(int index);
    void on_loadConfigPushButton_clicked();
    void toggle_bayer_ToolButtons();
    void timerConfigDone();
    void on_clearPushButton_clicked();
    void on_bayerCheckBox_clicked(bool checked);
    void on_actionClose_triggered();
    void toggleFormatPushButton();
    void on_readRAWgeometryPushButton_clicked();

private:
    Ui::Instrument *ui;

    QString thelidir;
    QString userdir;

    QList<QLineEdit*> geometryList;
    QList<QLineEdit*> geometryNumList;
    int numChips = 1;
    QString geometryToConfig(QString geometry);
    QString configToGeometry(QString config);
    void altStream(QTextStream &stream, QString keyword, QString altValue);
    QButtonGroup *bayerButtonGroup = new QButtonGroup(this);
    bool compareChipNumbers();
    void getKey(QTextStream &stream, QString bashkey, QString fitsKey, QString mode = "");
    void truncateFITSkey(QString &key);
    void applyStyleSheets();
};

#endif // INSTRUMENT_H
