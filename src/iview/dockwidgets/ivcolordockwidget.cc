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

#include "ivcolordockwidget.h"
#include "ui_ivcolordockwidget.h"
#include "../iview.h"

#include <cmath>

#include <QString>
#include <QPushButton>

IvColorDockWidget::IvColorDockWidget(IView *parent) :
    QDockWidget(parent),
    ui(new Ui::IvColorDockWidget)
{
    ui->setupUi(this);
    iview = parent;

    ui->redSlider->setMinimum(0);
    ui->redSlider->setMaximum(sliderSteps);
    ui->redSlider->setValue(sliderSteps/2);
    ui->redSlider->setMinimumWidth(150);
    ui->redSlider->setMaximumWidth(250);
    ui->redFactorLineEdit->setMinimumWidth(120);
    ui->redFactorLineEdit->setMaximumWidth(120);
    ui->redResetPushButton->setText("Reset red factor");

    ui->blueSlider->setMinimum(0);
    ui->blueSlider->setMaximum(sliderSteps);
    ui->blueSlider->setValue(sliderSteps/2);
    ui->blueSlider->setMinimumWidth(150);
    ui->blueSlider->setMaximumWidth(250);
    ui->blueFactorLineEdit->setMinimumWidth(120);
    ui->blueFactorLineEdit->setMaximumWidth(120);
    ui->blueResetPushButton->setText("Reset blue factor");

    connect(ui->blueFactorLineEdit, &QLineEdit::textChanged, this, &IvColorDockWidget::validate);
    connect(ui->redFactorLineEdit, &QLineEdit::textChanged, this, &IvColorDockWidget::validate);
    connect(ui->redFactorLineEdit, &QLineEdit::textEdited, this, &IvColorDockWidget::redFactorEdited);
    connect(ui->blueFactorLineEdit, &QLineEdit::textEdited, this, &IvColorDockWidget::blueFactorEdited);
    connect(ui->redSlider, &QSlider::sliderMoved, this, &IvColorDockWidget::redSliderMoved);
    connect(ui->blueSlider, &QSlider::sliderMoved, this, &IvColorDockWidget::blueSliderMoved);
    connect(this, &IvColorDockWidget::colorFactorChanged, iview, &IView::colorFactorChanged_receiver);
    connect(ui->G2referencesPushButton, &QPushButton::toggled, iview, &IView::showG2References);
}

IvColorDockWidget::~IvColorDockWidget()
{
    delete ui;
}

void IvColorDockWidget::validate()
{
    // Floating point validator
    QRegExp rf( "^[0-9]+[.]{0,1}[0-9]+" );
    QValidator *validator = new QRegExpValidator( rf, this );
    ui->blueFactorLineEdit->setValidator( validator );
    ui->redFactorLineEdit->setValidator( validator );
}

void IvColorDockWidget::sliderToText(int sliderValue, QString channel)
{
    float sh = sliderSteps / 2.0;
    if (channel == "red") {
        colorFactorApplied[0] = colorFactorZeropoint[0] * pow(2., float(sliderValue-sh) / sh);
        ui->redFactorLineEdit->setText(QString::number(colorFactorApplied[0], 'f', 3));
    }

    if (channel == "blue") {
        colorFactorApplied[2] = colorFactorZeropoint[2] * pow(2., float(sliderValue-sh) / sh);
        ui->blueFactorLineEdit->setText(QString::number(colorFactorApplied[2], 'f', 3));
    }
}

void IvColorDockWidget::textToSlider(QString value, QString channel)
{
    float sh = sliderSteps / 2.0;
    if (channel == "red") {
        int sliderValue = sh * ( log(value.toFloat() / colorFactorZeropoint[0]) / log(2.) + 1.);
        ui->redSlider->setValue(sliderValue);
    }

    if (channel == "blue") {
        int sliderValue = sh * ( log(value.toFloat() / colorFactorZeropoint[2]) / log(2.) + 1.);
        ui->blueSlider->setValue(sliderValue);
    }
}

void IvColorDockWidget::redSliderMoved(const int &sliderValue)
{
    sliderToText(sliderValue, "red");
    emit colorFactorChanged(ui->redFactorLineEdit->text(), ui->blueFactorLineEdit->text());
}

void IvColorDockWidget::blueSliderMoved(const int &sliderValue)
{
    sliderToText(sliderValue, "blue");
    emit colorFactorChanged(ui->redFactorLineEdit->text(), ui->blueFactorLineEdit->text());
}

void IvColorDockWidget::redFactorEdited(const QString &value)
{
    textToSlider(value, "red");
    colorFactorApplied[0] = value.toFloat();
    emit colorFactorChanged(ui->redFactorLineEdit->text(), ui->blueFactorLineEdit->text());
}

void IvColorDockWidget::blueFactorEdited(const QString &value)
{
    textToSlider(value, "blue");
    colorFactorApplied[2] = value.toFloat();
    emit colorFactorChanged(ui->redFactorLineEdit->text(), ui->blueFactorLineEdit->text());
}

void IvColorDockWidget::on_redResetPushButton_clicked()
{
    ui->redSlider->setValue(sliderSteps/2.);
    ui->redFactorLineEdit->setText(QString::number(colorFactorZeropoint[0], 'f', 3));
    colorFactorApplied[0] = colorFactorZeropoint[0];
    emit colorFactorChanged(ui->redFactorLineEdit->text(), ui->blueFactorLineEdit->text());
}

void IvColorDockWidget::on_blueResetPushButton_clicked()
{
    ui->blueSlider->setValue(sliderSteps/2.);
    ui->blueFactorLineEdit->setText(QString::number(colorFactorZeropoint[2], 'f', 3));
    colorFactorApplied[2] = colorFactorZeropoint[2];
    emit colorFactorChanged(ui->redFactorLineEdit->text(), ui->blueFactorLineEdit->text());
}
