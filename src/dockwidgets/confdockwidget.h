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

#ifndef CONFDOCKWIDGET_H
#define CONFDOCKWIDGET_H

#include "readmes/scampreadme.h"

#include <QDockWidget>
#include <QMap>
#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QButtonGroup>

class MainWindow;   // forward declaration to access GUI members

namespace Ui {
class ConfDockWidget;
}

class ConfDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit ConfDockWidget(MainWindow *parent = 0);
    ~ConfDockWidget();

    Ui::ConfDockWidget *ui;

    QMap<QString,QString> defaultMap;
    MainWindow *mainGUI;

    // The following are set externally by MainWindow::on_setupInstrumentComboBox_clicked()
    QFile instrument_file;
    QString instrument_dir;
    QString instrument_name;
    QString instrument_type;
    QString instrument_bayer;
    int instrument_nchips = 1;

    QButtonGroup *norxtalkButtonGroup = new QButtonGroup(this);
    QButtonGroup *rowxtalkButtonGroup = new QButtonGroup(this);

    void load_default_stdcat();
    bool checkRightScampMode(const QString &coordsMode);

private:
    QList<QPushButton*> APIstdPushButtonList;
    QList<QComboBox*> APIstdComboBoxList;
    ScampReadme *scampreadme;

    bool doingInitialLaunch = false;

    void populateDefaultMap();
    void populateAPIlists();
    void establish_connections();
    void launchViewer(const QString &dirname, const QString &filter, const QString &mode);
    void initConfDockWidget();
    void applyStyleSheets();
    void setupXtalkButtonGroups();
    void validate();
    void connect_validators();
    void setupAstrometry();

private slots:
    void on_APDrefcatComboBox_currentTextChanged(const QString &arg1);
    void on_APIrefcatComboBox_currentTextChanged(const QString &arg1);
    void on_ARCcatalogComboBox_currentTextChanged(const QString &arg1);
    void on_ARCselectimagePushButton_clicked();
    void on_ASTmethodComboBox_currentIndexChanged(int index);
    void on_ASTmatchMethodComboBox_currentIndexChanged(int index);
    void on_ASTreadmepushButton_clicked();
    void on_COAcelestialtypeComboBox_activated(const QString &arg1);
    void on_COAskypaPushButton_clicked();
    void on_COCdirectionComboBox_currentTextChanged(const QString &arg1);
    void on_confStackedWidget_currentChanged(int arg1);
    void on_confForwardPushButton_clicked();
    void on_confBackwardPushButton_clicked();
    void on_skyAreaComboBox_currentTextChanged(const QString &arg1);
    void on_skyAreaPushButton_clicked();
    void showAPIsolution();
    void switch_static_dynamic();
    void toggle_icons_xtalkToolButtons();
    void updateConfPageLabel();

    void on_nonlinearityCheckBox_clicked();

public slots:
//    void readGaiaBulkMotion();
    void loadDefaults();
    void updateGaiaBulkMotion(const QString &pmRA, const QString &pmDE);
    void updateAPIsolutions();
    void updateARCdisplay();
    void setupInstrumentComboBox_clicked();
    void toggle_skyparams();
    void toggle_skyparamThresholds(QString text);
    void targetResolvedReceived(const QString &alpha, const QString &delta);
};

#endif // CONFDOCKWIDGET_H
