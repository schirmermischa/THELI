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

#ifndef IVWCSDOCKWIDGET_H
#define IVWCSDOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
class IvWCSDockWidget;
}

class IvWCSDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit IvWCSDockWidget(QWidget *parent = 0);
    ~IvWCSDockWidget();

    double cd11_orig = 0.0;
    double cd12_orig = 0.0;
    double cd21_orig = 0.0;
    double cd22_orig = 0.0;

    void init();

private slots:
    void on_cd11Slider_sliderMoved(int position);
    void on_cd12Slider_sliderMoved(int position);
    void on_cd21Slider_sliderMoved(int position);
    void on_cd22Slider_sliderMoved(int position);
    void on_plateScaleSlider_sliderMoved(int position);
    void on_posAngleSlider_sliderMoved(int position);
    void on_resetPushButton_clicked();
    void on_cd11Slider_sliderReleased();
    void on_cd12Slider_sliderReleased();
    void on_cd21Slider_sliderReleased();
    void on_cd22Slider_sliderReleased();

private:
    Ui::IvWCSDockWidget *ui;

    double cd11 = 0.0;
    double cd12 = 0.0;
    double cd21 = 0.0;
    double cd22 = 0.0;

    double translateCDmatrixSlider(int position, int maxRange);
    double translatePlateScaleSlider(int position, int maxRange);
    double translatePosAngleSlider(int position, int maxRange);
    void updateLineEdits();
    void restoreSliders();
    float calcCDweight();

signals:
    void CDmatrixChanged(double cd11, double cd12, double cd21, double cd22);
    void CDmatrixChangedFITS(double cd11, double cd12, double cd21, double cd22);
    void CRPIXchanged(double crpix);
};

#endif // IVWCSDOCKWIDGET_H
