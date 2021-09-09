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

#ifndef IVFINDERDOCKWIDGET_H
#define IVFINDERDOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
class IvFinderDockWidget;
}

class IvFinderDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit IvFinderDockWidget(QWidget *parent = 0);
    ~IvFinderDockWidget();
    Ui::IvFinderDockWidget *ui;

    QString dateObs = "2020-01-01T00:00:00";
    float geoLon = 0.0;
    float geoLat = 0.0;

    void bypassResolver();
public slots:
    void updateDateObsAndGeo(QString dateobs, float geolon, float geolat);

    void on_targetresolverToolButton_clicked();
    void on_MPCresolverToolButton_clicked();
    void on_locatePushButton_clicked();

private slots:
    void targetResolvedReceived(QString alpha, QString delta);
    void validate();
    void on_clearPushButton_clicked();

signals:
    void targetResolved(QString alpha, QString delta);
    void clearTargetResolved();

private:
};

#endif // IVFINDERDOCKWIDGET_H
