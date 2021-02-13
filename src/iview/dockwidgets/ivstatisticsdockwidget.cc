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
    int validDigits = 4 - log( fabs(iview->globalMedian)) / log(10.);
    if (validDigits < 0) validDigits = 0;
    QString sigma = QChar(0xc3, 0x03);

    ui->globalMedianLabel->setText(QString::number(iview->globalMedian, 'f', validDigits));
    ui->globalMeanLabel->setText(QString::number(iview->globalMean, 'f', validDigits));
    ui->globalRMSLabel->setText(QString::number(iview->globalRMS, 'f', validDigits));
    ui->globalSigmaLabel->setText(sigma + " = ");

    ui->localMedianLabel->setText(QString::number(localMedian, 'f', validDigits));
    ui->localMeanLabel->setText(QString::number(localMean, 'f', validDigits));
    ui->localRMSLabel->setText(QString::number(localSigma, 'f', validDigits));
    ui->localSigmaLabel->setText(sigma + " = ");

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

//    if (iview->displayMode == "FITSmonochrome" || iview->displayMode == "MEMview") {
        ui->localMedianLabel->setText(QString::number(localMedian, 'f', validDigits));
        ui->localMeanLabel->setText(QString::number(localMean, 'f', validDigits));
        ui->localRMSLabel->setText(QString::number(localSigma, 'f', validDigits));
//    }
}

void IvStatisticsDockWidget::on_localWindowComboBox_currentIndexChanged(const QString &arg1)
{
    statWidth = ui->localWindowComboBox->currentText().split('x').at(0).toInt();
}
