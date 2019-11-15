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

#ifndef IVCOLORDOCKWIDGET_H
#define IVCOLORDOCKWIDGET_H

#include <QDockWidget>
#include <QSlider>
#include <QString>
#include <QLineEdit>
#include <QPushButton>

namespace Ui {
class IvColorDockWidget;
}

class IView;  // Forward declaration to access members

class IvColorDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit IvColorDockWidget(IView *parent = 0);
    ~IvColorDockWidget();
    Ui::IvColorDockWidget *ui;
    void textToSlider(QString value, QString channel);
    QVector<float> colorFactorApplied = QVector<float>(3, 1.0); // stores current RGB factors
    QVector<float> colorFactorZeropoint = QVector<float>(3, 1.0); // stores fixed RGB factors as determined by calibration

    IView *iview;

private:
    void sliderToText(int sliderValue, QString channel);

    int sliderSteps = 100;

signals:
    void colorFactorChanged(QString redFactor, QString blueFactor);
    void showG2references(bool checked);

private slots:
    void blueSliderMoved(const int &sliderValue);
    void blueFactorEdited(const QString &value);
    void redSliderMoved(const int &sliderValue);
    void redFactorEdited(const QString &value);
    void validate();
    void on_blueResetPushButton_clicked();
    void on_redResetPushButton_clicked();
};

#endif // IVCOLORDOCKWIDGET_H
