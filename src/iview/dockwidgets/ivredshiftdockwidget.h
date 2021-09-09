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

#ifndef IVREDSHIFTDOCKWIDGET_H
#define IVREDSHIFTDOCKWIDGET_H

#include <QDockWidget>
#include <QCheckBox>
#include <QList>
#include "../myaxis.h"

namespace Ui {
class IvRedshiftDockWidget;
}

class IView;  // Forward declaration to access members

class IvRedshiftDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit IvRedshiftDockWidget(IView *parent = 0);
    ~IvRedshiftDockWidget();

    Ui::IvRedshiftDockWidget *ui;

    IView *iview;

    QList<QPair<QString,float>> HList;
    QList<QPair<QString,float>> HeList;
    QList<QPair<QString,float>> NeList;
    QList<QPair<QString,float>> NList;
    QList<QPair<QString,float>> CList;
    QList<QPair<QString,float>> OList;
    QList<QPair<QString,float>> ArList;
    QList<QPair<QString,float>> FeList;
    QList<QPair<QString,float>> MgList;
    QList<QPair<QString,float>> NaList;
    QList<QPair<QString,float>> CaList;
    QList<QPair<QString,float>> SList;

    void init();

public slots:
    void showWavelength(QString lobs, QString lrest);
    void redshiftUpdatedReceiver(float z_0);

private slots:
    void on_zLineEdit_textChanged(const QString &arg1);
    void validate();

private:
    void populateWavelengthMaps();
    void showAxes();

signals:
    void redshiftEdited(QString text);

};

#endif // IVREDSHIFTDOCKWIDGET_H
