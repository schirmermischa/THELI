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

#include "ivstatisticsdockwidget.h"
#include "ui_ivstatisticsdockwidget.h"
#include "../iview.h"
#include "../../functions.h"
#include <math.h>

IvStatisticsDockWidget::IvStatisticsDockWidget(IView *parent) :
    QDockWidget(parent),
    ui(new Ui::IvStatisticsDockWidget)
{
    ui->setupUi(this);

    iview = parent;
}

IvStatisticsDockWidget::~IvStatisticsDockWidget()
{
    delete ui;
}

void IvStatisticsDockWidget::init()
{
    QString sigma = QChar(0xc3, 0x03);
    int validDigits = 4 - log( fabs(iview->globalMedian)) / log(10.);
    if (validDigits < 0) validDigits = 0;

    if (iview->displayMode == "FITSmonochrome" || iview->displayMode == "MEMview") {
        ui->globalSigmaLeftLabel->setText(sigma + " = ");
        ui->localSigmaLeftLabel->setText(sigma + " = ");
        ui->globalMedianLabel->setText(QString::number(iview->globalMedian, 'f', validDigits));
        ui->globalMeanLabel->setText(QString::number(iview->globalMean, 'f', validDigits));
        ui->globalSigmaLabel->setText(QString::number(iview->globalRMS, 'f', validDigits));
        ui->localMedianLabel->clear();
        ui->localMeanLabel->clear();
        ui->localSigmaLabel->clear();
    }
    if (iview->displayMode == "FITScolor") {
        ui->globalMedianLeftLabel->setText("Median R = ");
        ui->globalMeanLeftLabel->setText("Median G = ");
        ui->globalSigmaLeftLabel->setText("Median B = ");
        ui->globalMedianLabel->setText(QString::number(iview->globalMedianR, 'f', validDigits));
        ui->globalMeanLabel->setText(QString::number(iview->globalMedianG, 'f', validDigits));
        ui->globalSigmaLabel->setText(QString::number(iview->globalMedianB, 'f', validDigits));
        ui->localMedianLeftLabel->setText("Median R = ");
        ui->localMeanLeftLabel->setText("Median G = ");
        ui->localSigmaLeftLabel->setText("Median B = ");
        ui->localMedianLabel->clear();
        ui->localMeanLabel->clear();
        ui->localSigmaLabel->clear();
    }

    ui->localWindowComboBox->setCurrentText("9x9");
}

void IvStatisticsDockWidget::statisticsSampleReceiver(const QVector<float> &sample)
{
    if (!this->isVisible()) return;

    localMedian = straightMedian_T(sample);
    localMean = meanMask_T(sample, QVector<bool>());
    localSigma = 1.486*madMask_T(sample, QVector<bool>(), "ignoreZeroes");

    int validDigits = 4 - log( fabs(localMedian)) / log(10.);
    if (validDigits < 0) validDigits = 0;

    ui->localMedianLabel->setText(QString::number(localMedian, 'f', validDigits));
    ui->localMeanLabel->setText(QString::number(localMean, 'f', validDigits));
    ui->localSigmaLabel->setText(QString::number(localSigma, 'f', validDigits));
}

void IvStatisticsDockWidget::statisticsSampleColorReceiver(const QVector<float> &sampleR, const QVector<float> &sampleG, const QVector<float> &sampleB)
{
    if (!this->isVisible()) return;

    localMedianR = straightMedian_T(sampleR);
    localMedianG = straightMedian_T(sampleG);
    localMedianB = straightMedian_T(sampleB);
    /*
    float localMeanR = meanMask_T(sampleR, QVector<bool>());
    float localMeanG = meanMask_T(sampleG, QVector<bool>());
    float localMeanB = meanMask_T(sampleB, QVector<bool>());
    float localSigmaR = 1.486*madMask_T(sampleR, QVector<bool>(), "ignoreZeroes");
    float localSigmaG = 1.486*madMask_T(sampleG, QVector<bool>(), "ignoreZeroes");
    float localSigmaB = 1.486*madMask_T(sampleB, QVector<bool>(), "ignoreZeroes");
    */

    int validDigits = 4 - log( fabs(localMedianG)) / log(10.);
    if (validDigits < 0) validDigits = 0;

    // re-purposing default labels:
    ui->localMedianLabel->setText(QString::number(localMedianR, 'f', validDigits));
    ui->localMeanLabel->setText(QString::number(localMedianG, 'f', validDigits));
    ui->localSigmaLabel->setText(QString::number(localMedianB, 'f', validDigits));
}

void IvStatisticsDockWidget::on_localWindowComboBox_currentIndexChanged(const QString &arg1)
{
    statWidth = ui->localWindowComboBox->currentText().split('x').at(0).toInt();
}
