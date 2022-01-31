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

#include "ivredshiftdockwidget.h"
#include "ui_ivredshiftdockwidget.h"

#include "../iview.h"
#include "../../functions.h"
#include <math.h>

#include <QString>
#include <QSpinBox>
#include <QList>
#include <QPair>

IvRedshiftDockWidget::IvRedshiftDockWidget(IView *parent) :
    QDockWidget(parent),
    ui(new Ui::IvRedshiftDockWidget)
{
    ui->setupUi(this);

    iview = parent;

    populateWavelengthMaps();

    init();

    QString lambda = QString(QChar(0xbb, 0x03));
    QString angstrom = QString(QChar(0x2b, 0x21));
    ui->lambdaObsLabel->setText(lambda+"<sub>observed</sub>"+" ["+angstrom+"]");
    ui->lambdaRestLabel->setText(lambda+"<sub>restframe</sub>"+" ["+angstrom+"]");
}

IvRedshiftDockWidget::~IvRedshiftDockWidget()
{
    delete ui;
}

void IvRedshiftDockWidget::populateWavelengthMaps()
{
    HList.append(qMakePair(QString("Lya"), 1215));
    HList.append(qMakePair(QString("Lyb"), 1025));
    HList.append(qMakePair(QString("Lyg"), 972));
    HList.append(qMakePair(QString("Hd"), 4103));
    HList.append(qMakePair(QString("Hg"), 4343));
    HList.append(qMakePair(QString("Hb"), 4864));
    HList.append(qMakePair(QString("Ha"), 6565));
    HList.append(qMakePair(QString("Paa"), 18746));
    HList.append(qMakePair(QString("Pab"), 12815));
    HList.append(qMakePair(QString("Pag"), 10935));
    HList.append(qMakePair(QString("Brg"), 21650));
    HList.append(qMakePair(QString("H2_1-0"), 21220));
    HList.append(qMakePair(QString("H2_2-1"), 22480));

    HeList.append(qMakePair(QString("HeII"), 1640));
    HeList.append(qMakePair(QString("HeII"), 2733));
    HeList.append(qMakePair(QString("HeI"), 2945));
    HeList.append(qMakePair(QString("HeII"), 3204));
    HeList.append(qMakePair(QString("HeI"), 3889));
    HeList.append(qMakePair(QString("HeI"), 4471));
    HeList.append(qMakePair(QString("HeII"), 4687));
    HeList.append(qMakePair(QString("HeII"), 5413));
    HeList.append(qMakePair(QString("HeI"), 5877));
    HeList.append(qMakePair(QString("HeI"), 6680));
    HeList.append(qMakePair(QString("HeI"), 10830));
    HeList.append(qMakePair(QString("HeI"), 20581));

    OList.append(qMakePair(QString("OVI"), 1032));
    OList.append(qMakePair(QString("OVI"), 1038));
    OList.append(qMakePair(QString("OVI]"), 1402));
    OList.append(qMakePair(QString("OIII]"), 1666));
    OList.append(qMakePair(QString("OIII"), 3133));
    OList.append(qMakePair(QString("[OII]"), 3727));
    OList.append(qMakePair(QString("[OIII]"), 4363));
    OList.append(qMakePair(QString("[OIII]"), 4960));
    OList.append(qMakePair(QString("[OIII]"), 5008));
    OList.append(qMakePair(QString("[OI]"), 6302));
    OList.append(qMakePair(QString("[OI]"), 6366));
    OList.append(qMakePair(QString("[OII]"), 7321));

    NList.append(qMakePair(QString("NV"), 1240));
    NList.append(qMakePair(QString("[NII]"), 6550));
    NList.append(qMakePair(QString("[NII]"), 6586));

    CList.append(qMakePair(QString("CIII"), 977));
    CList.append(qMakePair(QString("CIII]"), 1909));
    CList.append(qMakePair(QString("CIV"), 1549));
    CList.append(qMakePair(QString("CII"), 2326));

    ArList.append(qMakePair(QString("[ArIII]"), 7138));

    SList.append(qMakePair(QString("[SII]"), 1812));
    SList.append(qMakePair(QString("[SII]"), 1822));
    SList.append(qMakePair(QString("[SII]"), 4069));
    SList.append(qMakePair(QString("[SII]"), 4076));
    SList.append(qMakePair(QString("SII"), 5203));
    SList.append(qMakePair(QString("[SII]"), 6718));
    SList.append(qMakePair(QString("[SII]"), 6733));
    SList.append(qMakePair(QString("[SIII]"), 9071));
    SList.append(qMakePair(QString("[SIII]"), 9533));

    NaList.append(qMakePair(QString("NaD2"), 5890));
    NaList.append(qMakePair(QString("NaD1"), 5896));

    NeList.append(qMakePair(QString("[NeIV]"), 2424));
    NeList.append(qMakePair(QString("[NeV]"), 3347));
    NeList.append(qMakePair(QString("[NeV]"), 3427));
    NeList.append(qMakePair(QString("[NeIII]"), 3869));
    NeList.append(qMakePair(QString("[NeIII]"), 3968));

    CaList.append(qMakePair(QString("CaH"), 3934));
    CaList.append(qMakePair(QString("CaK"), 3969));
    CaList.append(qMakePair(QString("CaT1"), 8499));
    CaList.append(qMakePair(QString("CaT2"), 8542));
    CaList.append(qMakePair(QString("CaT3"), 8662));

    MgList.append(qMakePair(QString("Mg"), 2799));
    MgList.append(qMakePair(QString("Mg"), 5167));
    MgList.append(qMakePair(QString("Mg"), 5173));
    MgList.append(qMakePair(QString("Mg"), 5184));

    FeList.append(qMakePair(QString("FeI"), 2745));
    FeList.append(qMakePair(QString("FeI"), 2807));
    FeList.append(qMakePair(QString("FeI"), 2950));
    FeList.append(qMakePair(QString("FeI"), 5725));
    FeList.append(qMakePair(QString("[FeVII]"), 6089));
    FeList.append(qMakePair(QString("FeI"), 11887));
}

void IvRedshiftDockWidget::init()
{
    connect(ui->HCheckBox, &QCheckBox::clicked, iview, &IView::updateAxes);
    connect(ui->HeCheckBox, &QCheckBox::clicked, iview, &IView::updateAxes);
    connect(ui->NCheckBox, &QCheckBox::clicked, iview, &IView::updateAxes);
    connect(ui->CCheckBox, &QCheckBox::clicked, iview, &IView::updateAxes);
    connect(ui->OCheckBox, &QCheckBox::clicked, iview, &IView::updateAxes);
    connect(ui->SCheckBox, &QCheckBox::clicked, iview, &IView::updateAxes);
    connect(ui->NeCheckBox, &QCheckBox::clicked, iview, &IView::updateAxes);
    connect(ui->NaCheckBox, &QCheckBox::clicked, iview, &IView::updateAxes);
    connect(ui->CaCheckBox, &QCheckBox::clicked, iview, &IView::updateAxes);
    connect(ui->MgCheckBox, &QCheckBox::clicked, iview, &IView::updateAxes);
    connect(ui->ArCheckBox, &QCheckBox::clicked, iview, &IView::updateAxes);
    connect(ui->FeCheckBox, &QCheckBox::clicked, iview, &IView::updateAxes);
    connect(ui->finesseSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), iview, &IView::updateFinesse);          // QSpinBox has two valueChanged(), signals with int and string arguments; we need int
    connect(ui->zLineEdit, &QLineEdit::textEdited, iview, &IView::redshiftChanged);
    connect(ui->zLineEdit, &QLineEdit::textChanged, this, &IvRedshiftDockWidget::validate);
}

void IvRedshiftDockWidget::showWavelength(QString lobs, QString lrest)
{
    // Display the wavelengths
    ui->lambdaObsLineEdit->setText(lobs);
    ui->lambdaRestLineEdit->setText(lrest);
}

void IvRedshiftDockWidget::on_zLineEdit_textChanged(const QString &arg1)
{
    emit redshiftEdited(ui->zLineEdit->text());
}

void IvRedshiftDockWidget::redshiftUpdatedReceiver(float z_0)
{
    ui->zLineEdit->setText(QString::number(z_0, 'f', 4));
}

void IvRedshiftDockWidget::validate()
{
    QRegExp rf( "^[0-9]*[.]{0,1}[0-9]+" );
    QValidator* validator_float_pos = new QRegExpValidator(rf, this );

    ui->zLineEdit->setValidator(validator_float_pos);
}
