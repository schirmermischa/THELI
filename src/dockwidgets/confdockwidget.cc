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

#include "confdockwidget.h"
#include "ui_confdockwidget.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"
#include "../iview/iview.h"

#include <QPushButton>
#include <QToolButton>
#include <QRadioButton>
#include <QComboBox>

ConfDockWidget::ConfDockWidget(MainWindow *parent) :
    QDockWidget(parent),
    ui(new Ui::ConfDockWidget)
{
    mainGUI = parent;
    ui->setupUi(this);

    doingInitialLaunch = true;

    initConfDockWidget();

    setupAstrometry();

    populateDefaultMap();

    establish_connections();

    populateAPIlists();

    updateConfPageLabel();

    ui->skyAreaComboBox->setStyleSheet("combobox-popup: 0;");   // enforcing max number of 18 entries (set in designer)

    doingInitialLaunch = false;
}

ConfDockWidget::~ConfDockWidget()
{
    delete ui;
}

void ConfDockWidget::initConfDockWidget()
{
    QIcon magnifyer(":/icons/magnifyer.png");
    ui->ARCtargetresolverToolButton->setIcon(magnifyer);

    ui->xtalk_col_1x2ToolButton->setIcon(QIcon(":/icons/xtalk_col_1x2.png"));
    ui->xtalk_col_2x1ToolButton->setIcon(QIcon(":/icons/xtalk_col_2x1.png"));
    ui->xtalk_col_2x2ToolButton->setIcon(QIcon(":/icons/xtalk_col_2x2.png"));
    ui->xtalk_nor_1x2ToolButton->setIcon(QIcon(":/icons/xtalk_nor_1x2.png"));
    ui->xtalk_nor_2x1ToolButton->setIcon(QIcon(":/icons/xtalk_nor_2x1.png"));
    ui->xtalk_nor_2x2ToolButton->setIcon(QIcon(":/icons/xtalk_nor_2x2.png"));
    ui->xtalk_row_1x2ToolButton->setIcon(QIcon(":/icons/xtalk_row_1x2.png"));
    ui->xtalk_row_2x1ToolButton->setIcon(QIcon(":/icons/xtalk_row_2x1.png"));
    ui->xtalk_row_2x2ToolButton->setIcon(QIcon(":/icons/xtalk_row_2x2.png"));

    ui->skyReadmePushButton->hide();

    // update the collapse direction icon
    QString colldir = ui->COCdirectionComboBox->currentText();
    ui->COCdirectionLabel->setPixmap(QPixmap(":/icons/collapse_"+colldir));

    // These comboboxes do not get initialized or filled properly unless one clicks on it:
    on_APDrefcatComboBox_currentTextChanged(ui->APDrefcatComboBox->currentText());
    on_ASTmethodComboBox_currentIndexChanged(ui->ASTmethodComboBox->currentIndex());
    // why commented / uncommented above?
    //    on_APIrefcatComboBox_currentTextChanged(ui->APIrefcatComboBox->currentText());

    updateARCdisplay();

    toggle_skyparams();

    toggle_skyparamThresholds("");

    applyStyleSheets();

    setupXtalkButtonGroups();

    updateAPIsolutions();
}

void ConfDockWidget::establish_connections()
{
    // Connect crosstalk ToolButtons with icon updater
    connect(ui->xtalk_col_1x2ToolButton, &QToolButton::toggled, this, &ConfDockWidget::toggle_icons_xtalkToolButtons);
    connect(ui->xtalk_col_2x1ToolButton, &QToolButton::toggled, this, &ConfDockWidget::toggle_icons_xtalkToolButtons);
    connect(ui->xtalk_col_2x2ToolButton, &QToolButton::toggled, this, &ConfDockWidget::toggle_icons_xtalkToolButtons);
    connect(ui->xtalk_row_1x2ToolButton, &QToolButton::toggled, this, &ConfDockWidget::toggle_icons_xtalkToolButtons);
    connect(ui->xtalk_row_2x1ToolButton, &QToolButton::toggled, this, &ConfDockWidget::toggle_icons_xtalkToolButtons);
    connect(ui->xtalk_row_2x2ToolButton, &QToolButton::toggled, this, &ConfDockWidget::toggle_icons_xtalkToolButtons);
    connect(ui->xtalk_nor_1x2ToolButton, &QToolButton::toggled, this, &ConfDockWidget::toggle_icons_xtalkToolButtons);
    connect(ui->xtalk_nor_2x1ToolButton, &QToolButton::toggled, this, &ConfDockWidget::toggle_icons_xtalkToolButtons);
    connect(ui->xtalk_nor_2x2ToolButton, &QToolButton::toggled, this, &ConfDockWidget::toggle_icons_xtalkToolButtons);

    // Connect skysub Radio Buttons with a function that dis/enables frames
    connect(ui->skyModelRadioButton, &QRadioButton::clicked, this, &ConfDockWidget::toggle_skyparams);
    connect(ui->skyPolynomialRadioButton, &QRadioButton::clicked, this, &ConfDockWidget::toggle_skyparams);
    connect(ui->skyConstsubRadioButton, &QRadioButton::clicked, this, &ConfDockWidget::toggle_skyparams);
    connect(ui->skyAreaComboBox, &QComboBox::currentTextChanged, this, &ConfDockWidget::toggle_skyparamThresholds);

    connect(ui->BACwindowLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::switch_static_dynamic);
    connect(ui->confStackedWidget, &QStackedWidget::currentChanged, this, &ConfDockWidget::updateConfPageLabel);

    connect(ui->parametersDefaultPushButton, &QPushButton::clicked, this, &ConfDockWidget::loadDefaults);

    // TODO: check if needed
    // connect(ui->skyModelRadioButton, &QRadioButton::clicked, this, &MainWindow::on_startPushButton_clicked);
    // connect(ui->skyPolynomialRadioButton, &QRadioButton::clicked, this, &MainWindow::on_startPushButton_clicked);
    // connect(ui->skyConstsubRadioButton, &QRadioButton::clicked, this, &MainWindow::on_startPushButton_clicked);

    // Update command list in real time when the following parameters are changed
    // (may or may not add further scripts)
    // TODO: check if needed
    //    connect(ui->BACdistLineEdit, &QLineEdit::textChanged, this, &MainWindow::on_startPushButton_clicked);
    //    connect(ui->BACmagLineEdit, &QLineEdit::textChanged, this, &MainWindow::on_startPushButton_clicked);
    //    connect(ui->ARCwebRadioButton, &QRadioButton::clicked, this, &MainWindow::on_startPushButton_clicked);
    //    connect(ui->ARCimageRadioButton, &QRadioButton::clicked, this, &MainWindow::on_startPushButton_clicked);
    //    connect(ui->ASTmethodComboBox, &QComboBox::currentTextChanged, this, &MainWindow::startPushButton_clicked_dummy);
    //    connect(ui->APIWCSCheckBox, &QCheckBox::clicked, this, &MainWindow::on_startPushButton_clicked);

    // TODO: must activate
    connect(ui->ARCgetcatalogPushButton, &QPushButton::clicked, mainGUI, &MainWindow::on_startPushButton_clicked);
    connect(ui->restoreHeaderPushButton, &QPushButton::clicked, mainGUI, &MainWindow::on_startPushButton_clicked);
    connect(ui->ARCtargetresolverToolButton, &QToolButton::clicked, mainGUI, &MainWindow::on_startPushButton_clicked);

    connect(ui->ARCwebRadioButton, &QRadioButton::clicked, this, &ConfDockWidget::updateARCdisplay);
    connect(ui->ARCimageRadioButton, &QRadioButton::clicked, this, &ConfDockWidget::updateARCdisplay);

    // TODO: must activate
    // Connect StandardStar solution PushButtons with iView
    for (auto &it : APIstdPushButtonList) {
        connect(it, &QPushButton::clicked, this, &ConfDockWidget::showAPIsolution);
    }

    connect(ui->excludeDetectorsLineEdit, &QLineEdit::textChanged, mainGUI, &MainWindow::updateExcludedDetectors);

    connect(mainGUI->ui->setupInstrumentComboBox, &QComboBox::currentTextChanged, this, &ConfDockWidget::setupInstrumentComboBox_clicked);

    connect_validators();
}

void ConfDockWidget::applyStyleSheets()
{
    QList<QLabel*> conftitleLabelList;
    conftitleLabelList.append(ui->conftitleHDUreformatLabel);
    conftitleLabelList.append(ui->conftitleExcludeDetectorsLabel);
    conftitleLabelList.append(ui->conftitleProcessbiasLabel);
    conftitleLabelList.append(ui->conftitleChopnodLabel);
    conftitleLabelList.append(ui->conftitleBackground1Label);
    conftitleLabelList.append(ui->conftitleBackground2Label);
    conftitleLabelList.append(ui->conftitleCollapseLabel);
    conftitleLabelList.append(ui->conftitleBinnedpreviewLabel);
    conftitleLabelList.append(ui->conftitleGlobalweightLabel);
    conftitleLabelList.append(ui->conftitleIndividualweightLabel);
    conftitleLabelList.append(ui->conftitleSeparateLabel);
    conftitleLabelList.append(ui->conftitleAbsphotindirect1Label);
    conftitleLabelList.append(ui->conftitleAbsphotindirect2Label);
    conftitleLabelList.append(ui->conftitleCreatesourcecatLabel);
    conftitleLabelList.append(ui->conftitleGetCatalogLabel);
    conftitleLabelList.append(ui->conftitleAstromphotom1Label);
    conftitleLabelList.append(ui->conftitleAstromphotom2Label);
    conftitleLabelList.append(ui->conftitleStarflatLabel);
    conftitleLabelList.append(ui->conftitleSkysubLabel);
    conftitleLabelList.append(ui->conftitleCoadd1Label);
    conftitleLabelList.append(ui->conftitleCoadd2Label);
    for (auto &it : conftitleLabelList) {
        it->setStyleSheet("color: rgb(150, 240, 240); background-color: rgb(58, 78, 98);");
        it->setMargin(8);
    }

    QList<QLabel*> confsubtitleLabelList;
    confsubtitleLabelList.append(ui->confsubtitleHDUreformat1Label);
    confsubtitleLabelList.append(ui->confsubtitleHDUreformat2Label);
    confsubtitleLabelList.append(ui->confsubtitleCalibrators1Label);
    confsubtitleLabelList.append(ui->confsubtitleCalibrators2Label);
    confsubtitleLabelList.append(ui->confsubtitleBackground1Label);
    confsubtitleLabelList.append(ui->confsubtitleBackground2Label);
    confsubtitleLabelList.append(ui->confsubtitleBackground3Label);
    confsubtitleLabelList.append(ui->confsubtitleBackground4Label);
    confsubtitleLabelList.append(ui->confsubtitleBackground5Label);
    confsubtitleLabelList.append(ui->confsubtitleBackground6Label);
    confsubtitleLabelList.append(ui->confsubtitleCollapse1Label);
    confsubtitleLabelList.append(ui->confsubtitleCollapse2Label);
    confsubtitleLabelList.append(ui->confsubtitleCollapse3Label);
    confsubtitleLabelList.append(ui->confsubtitleGlobalweight1Label);
    confsubtitleLabelList.append(ui->confsubtitleGlobalweight2Label);
    confsubtitleLabelList.append(ui->confsubtitleGlobalweight3Label);
    confsubtitleLabelList.append(ui->confsubtitleIndividualweight1Label);
    confsubtitleLabelList.append(ui->confsubtitleIndividualweight2Label);
    confsubtitleLabelList.append(ui->confsubtitleAbsphotindirect1Label);
    confsubtitleLabelList.append(ui->confsubtitleAbsphotindirect2Label);
    confsubtitleLabelList.append(ui->confsubtitleAbsphotindirect3Label);
    confsubtitleLabelList.append(ui->confsubtitleAbsphotindirect4Label);
    confsubtitleLabelList.append(ui->confsubtitleCreatesourcecat1Label);
    confsubtitleLabelList.append(ui->confsubtitleCreatesourcecat2Label);
    confsubtitleLabelList.append(ui->confsubtitleCreatesourcecat3Label);
    confsubtitleLabelList.append(ui->confsubtitleGetCatalog1Label);
    confsubtitleLabelList.append(ui->confsubtitleGetCatalog2Label);
    confsubtitleLabelList.append(ui->confsubtitleAstromphotom1Label);
    confsubtitleLabelList.append(ui->confsubtitleAstromphotom2Label);
    confsubtitleLabelList.append(ui->confsubtitleAstromphotom3Label);
    confsubtitleLabelList.append(ui->confsubtitleAstromphotom4Label);
    confsubtitleLabelList.append(ui->confsubtitleAstromphotom5Label);
    confsubtitleLabelList.append(ui->confsubtitleAstromphotom6Label);
    confsubtitleLabelList.append(ui->confsubtitleSkysub1Label);
    confsubtitleLabelList.append(ui->confsubtitleSkysub2Label);
    confsubtitleLabelList.append(ui->confsubtitleSkysub3Label);
    confsubtitleLabelList.append(ui->confsubtitleCoadd1Label);
    confsubtitleLabelList.append(ui->confsubtitleCoadd2Label);
    confsubtitleLabelList.append(ui->confsubtitleCoadd3Label);
    confsubtitleLabelList.append(ui->confsubtitleCoadd4Label);
    confsubtitleLabelList.append(ui->confsubtitleCoadd5Label);
    confsubtitleLabelList.append(ui->confsubtitleCoadd6Label);
    for (auto &it : confsubtitleLabelList) {
        it->setStyleSheet("background-color: rgb(190,190,210);");
        it->setMargin(4);
    }

    QList<QFrame*> confFrameList;
    confFrameList.append(ui->confHDUreformat1Frame);
    confFrameList.append(ui->confHDUreformat2Frame);
    confFrameList.append(ui->confExcludeDetectorsFrame);
    confFrameList.append(ui->confCalibrators1Frame);
    confFrameList.append(ui->confCalibrators2Frame);
    confFrameList.append(ui->confChopnodFrame);
    confFrameList.append(ui->confBackground1Frame);
    confFrameList.append(ui->confBackground2Frame);
    confFrameList.append(ui->confBackground3Frame);
    confFrameList.append(ui->confBackground4Frame);
    confFrameList.append(ui->confBackground5Frame);
    confFrameList.append(ui->confBackground6Frame);
    confFrameList.append(ui->confCollapse1Frame);
    confFrameList.append(ui->confCollapse2Frame);
    confFrameList.append(ui->confCollapse3Frame);
    confFrameList.append(ui->confBinnedpreviewFrame);
    confFrameList.append(ui->confGlobalweight1Frame);
    confFrameList.append(ui->confGlobalweight2Frame);
    confFrameList.append(ui->confGlobalweight3Frame);
    confFrameList.append(ui->confIndividualweight1Frame);
    confFrameList.append(ui->confIndividualweight2Frame);
    confFrameList.append(ui->confSeparateFrame);
    confFrameList.append(ui->confAbsphotindirect1Frame);
    confFrameList.append(ui->confAbsphotindirect2Frame);
    confFrameList.append(ui->confAbsphotindirect3Frame);
    confFrameList.append(ui->confAbsphotindirect4Frame);
    confFrameList.append(ui->confAstromphotom1Frame);
    confFrameList.append(ui->confAstromphotom2Frame);
    confFrameList.append(ui->confAstromphotom3Frame);
    confFrameList.append(ui->confAstromphotom4Frame);
    confFrameList.append(ui->confAstromphotom5Frame);
    confFrameList.append(ui->confAstromphotom6Frame);
    confFrameList.append(ui->confCreatesourcecat1Frame);
    confFrameList.append(ui->confCreatesourcecat2Frame);
    confFrameList.append(ui->confCreatesourcecat3Frame);
    confFrameList.append(ui->confGetCatalog1Frame);
    confFrameList.append(ui->confGetCatalog2Frame);
    confFrameList.append(ui->confStarflatFrame);
    confFrameList.append(ui->confSkysub1Frame);
    confFrameList.append(ui->confSkysub2Frame);
    confFrameList.append(ui->confSkysub3Frame);
    confFrameList.append(ui->confCoadd1Frame);
    confFrameList.append(ui->confCoadd2Frame);
    confFrameList.append(ui->confCoadd3Frame);
    confFrameList.append(ui->confCoadd4Frame);
    confFrameList.append(ui->confCoadd5Frame);
    /*
    for (auto &it : confFrameList) {
    // specify large style sheet?
    }
    */

    /*
    // Checkboxes with differently colored background
    QList<QCheckBox*> confCheckboxList;
    confCheckboxList.append(ui->CIWmaskbloomingCheckBox);
    for (auto &it : confCheckboxList) {
        //        it->setStyleSheet("color: rgb(255, 255, 255); background-color: rgb(160,160,160);");
        it->setStyleSheet("background-color: rgb(190,190,210);");
    }
    */
}

void ConfDockWidget::targetResolvedReceived(const QString &alpha, const QString &delta)
{
    ui->ARCraLineEdit->setText(alpha);
    ui->ARCdecLineEdit->setText(delta);
}

void ConfDockWidget::toggle_icons_xtalkToolButtons()
{
    QToolButton *toolbutton = qobject_cast<QToolButton*>(sender());
    QString sender_name = toolbutton->objectName().remove("ToolButton");

    // Override checked/unchecked ToolButton background color
    // The "checked" color will actually be a bit darker than requested here (shade overriden)
    QPalette palette_checked, palette_unchecked;
    palette_checked.setColor(QPalette::Button,QColor("#00ffff"));
    palette_unchecked.setColor(QPalette::Button,QColor("#ffffff"));

    if (toolbutton->isChecked()) {
        if (sender()) {
            QToolButton *toolbutton = qobject_cast<QToolButton*>(sender());
            if (toolbutton->isChecked()) toolbutton->setPalette(palette_checked);
            else toolbutton->setPalette(palette_unchecked);
        }
        toolbutton->setPalette(palette_checked);
        // also activate the checkbox if a xtalk mode is selected
        if (sender_name.contains("nor")) ui->normalxtalkCheckBox->setChecked(true);
        else ui->rowxtalkCheckBox->setChecked(true);
    }
    else {
        toolbutton->setPalette(palette_unchecked);
    }
}

// This is a slot that receives a signal with a QString argument, but does not use it
void ConfDockWidget::toggle_skyparamThresholds(QString text)
{
    if (ui->skyAreaComboBox->currentIndex() == 0 && ui->confSkysub3Frame->isEnabled()) ui->confSkysub2Frame->setDisabled(true);
    else ui->confSkysub2Frame->setEnabled(true);
}

void ConfDockWidget::load_default_stdcat()
{
    if (instrument_type == "OPT") {
        ui->APIrefcatComboBox->setCurrentIndex(4);
        ui->APDfilterComboBox->setCurrentIndex(0);
    }
    else if (instrument_type == "NIR") {
        ui->APIrefcatComboBox->setCurrentIndex(6);
        ui->APDfilterComboBox->setCurrentIndex(1);
    }
    else if (instrument_type == "NIRMIR") {
        ui->APIrefcatComboBox->setCurrentIndex(10);
        ui->APDfilterComboBox->setCurrentIndex(1);
    }
    else {
        // There isn't really a solution for MIR...
        ui->APIrefcatComboBox->setCurrentIndex(10);
        ui->APDfilterComboBox->setCurrentIndex(1);
    }
}

void ConfDockWidget::populateAPIlists()
{
    APIstdPushButtonList.append(ui->APInight1PushButton);
    APIstdPushButtonList.append(ui->APInight2PushButton);
    APIstdPushButtonList.append(ui->APInight3PushButton);
    APIstdPushButtonList.append(ui->APInight4PushButton);
    APIstdPushButtonList.append(ui->APInight5PushButton);
    APIstdPushButtonList.append(ui->APInight6PushButton);
    APIstdPushButtonList.append(ui->APInight7PushButton);
    APIstdPushButtonList.append(ui->APInight8PushButton);
    APIstdPushButtonList.append(ui->APInight9PushButton);
    APIstdComboBoxList.append(ui->APInight1ComboBox);
    APIstdComboBoxList.append(ui->APInight2ComboBox);
    APIstdComboBoxList.append(ui->APInight3ComboBox);
    APIstdComboBoxList.append(ui->APInight4ComboBox);
    APIstdComboBoxList.append(ui->APInight5ComboBox);
    APIstdComboBoxList.append(ui->APInight6ComboBox);
    APIstdComboBoxList.append(ui->APInight7ComboBox);
    APIstdComboBoxList.append(ui->APInight8ComboBox);
    APIstdComboBoxList.append(ui->APInight9ComboBox);
}

void ConfDockWidget::toggle_skyparams()
{
    const QStandardItemModel* skyAreaModel = dynamic_cast< QStandardItemModel * >( ui->skyAreaComboBox->model() );
    if (ui->skyModelRadioButton->isChecked()) {
        ui->confSkysub2Frame->setEnabled(true);
        ui->confSkysub3Frame->setDisabled(true);
        ui->skyAreaPushButton->setDisabled(true);
    }
    else if (ui->skyPolynomialRadioButton->isChecked()) {
        ui->confSkysub2Frame->setDisabled(true);
        ui->confSkysub3Frame->setEnabled(true);
        for (int i=1; i<ui->skyAreaComboBox->count(); ++i) {
            skyAreaModel->item(i,0)->setEnabled(false);
        }
        // Must use specific areas
        ui->skyAreaComboBox->setCurrentIndex(0);
        ui->skyAreaPushButton->setEnabled(true);
    }
    else {
        if (ui->skyAreaComboBox->currentIndex() == 0) ui->confSkysub2Frame->setDisabled(true);
        else ui->confSkysub2Frame->setEnabled(true);

        ui->confSkysub3Frame->setEnabled(true);
        for (int i=1; i<ui->skyAreaComboBox->count(); ++i) {
            skyAreaModel->item(i,0)->setEnabled(true);
        }
        ui->skyAreaPushButton->setEnabled(true);
    }
}

void ConfDockWidget::on_COAcelestialtypeComboBox_activated(const QString &arg1)
{
    if (arg1 == "EQUATORIAL") {
        ui->COAtype1Label->setText("R.A.");
        ui->COAtype2Label->setText("DEC");
    }
    else if (arg1 == "GALACTIC") {
        ui->COAtype1Label->setText("GLON");
        ui->COAtype2Label->setText("GLAT");
    }
    else if (arg1 == "ECLIPTIC") {
        ui->COAtype1Label->setText("ELON");
        ui->COAtype2Label->setText("ELAT");
    }
    else if (arg1 == "SUPERGALACTIC") {
        ui->COAtype1Label->setText("SLON");
        ui->COAtype2Label->setText("SLAT");
    }
    else {
        ui->COAtype1Label->setText("R.A.");
        ui->COAtype2Label->setText("DEC");
    }
}

void ConfDockWidget::switch_static_dynamic()
{
    if (ui->BACwindowLineEdit->text().toInt() == 0) ui->BACcurrentmodeLabel->setText("The mode is: STATIC");
    else ui->BACcurrentmodeLabel->setText("The mode is: DYNAMIC");
}

void ConfDockWidget::on_COCdirectionComboBox_currentTextChanged(const QString &arg1)
{
    ui->COCdirectionLabel->setPixmap(QPixmap(":/icons/collapse_"+arg1));
}

void ConfDockWidget::on_APDrefcatComboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == "SDSS") fill_combobox(ui->APDfilterComboBox, "u g r i z");
    else if (arg1 == "2MASS") fill_combobox(ui->APDfilterComboBox, "J H Ks");
}

void ConfDockWidget::on_APIrefcatComboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == "LANDOLT_UBVRI") {
        fill_combobox(ui->APIfilterComboBox, "U B V R I");
        fill_combobox(ui->APIcolorComboBox, "U-B B-V V-R R-I");
    }
    else if (arg1 == "STETSON_UBVRI") {
        fill_combobox(ui->APIfilterComboBox, "U B V R I");
        fill_combobox(ui->APIcolorComboBox, "U-B B-V V-R R-I");
    }
    else if (arg1 == "STRIPE82_ugriz") {
        fill_combobox(ui->APIfilterComboBox, "u g r i z");
        fill_combobox(ui->APIcolorComboBox, "u-g g-r r-i i-z g-i");
    }
    else if (arg1 == "STRIPE82_u'g'r'i'z'") {
        fill_combobox(ui->APIfilterComboBox, "u' g' r' i' z'");
        fill_combobox(ui->APIcolorComboBox, "u'-g' g'-r' r'-i' i'-z' g'-i'");
    }
    else if (arg1 == "SMITH_u'g'r'i'z'") {
        fill_combobox(ui->APIfilterComboBox, "u' g' r' i' z'");
        fill_combobox(ui->APIcolorComboBox, "u'-g' g'-r' r'-i' i'-z' g'-i'");
    }
    else if (arg1 == "WASHINGTON") {
        fill_combobox(ui->APIfilterComboBox, "C M T1 T2");
        fill_combobox(ui->APIcolorComboBox, "C-M M-T1 T1-T2");
    }
    else if (arg1 == "MKO_JHK") {
        fill_combobox(ui->APIfilterComboBox, "J H K");
        fill_combobox(ui->APIcolorComboBox, "J-H H-K J-K");
    }
    else if (arg1 == "HUNT_JHK") {
        fill_combobox(ui->APIfilterComboBox, "J H K");
        fill_combobox(ui->APIcolorComboBox, "J-H H-K J-K");
    }
    else if (arg1 == "2MASSfaint_JHK") {
        fill_combobox(ui->APIfilterComboBox, "J H K");
        fill_combobox(ui->APIcolorComboBox, "J-H H-K J-K");
    }
    else if (arg1 == "PERSSON_JHKKs") {
        fill_combobox(ui->APIfilterComboBox, "J H K Ks");
        fill_combobox(ui->APIcolorComboBox, "J-H H-K H-Ks J-K J-Ks");
    }
    else if (arg1 == "JAC_YJHKLM") {
        fill_combobox(ui->APIfilterComboBox, "Y J H K L M");
        fill_combobox(ui->APIcolorComboBox, "Y-J J-H H-K J-K K-L L-M");
    }
    else if (arg1 == "MKO_LM") {
        fill_combobox(ui->APIfilterComboBox, "L M");
        fill_combobox(ui->APIcolorComboBox, "L-M");
    }
}

void ConfDockWidget::on_confStackedWidget_currentChanged(int arg1)
{
    if (this->isFloating()) {
        ui->confStackedWidget->setGeometry(100,100,300,300);
    }
}

void ConfDockWidget::updateARCdisplay()
{
    if (ui->ARCwebRadioButton->isChecked()) ui->ARCstackedWidget->setCurrentIndex(0);
    else ui->ARCstackedWidget->setCurrentIndex(1);
}

void ConfDockWidget::on_ASTmatchMethodComboBox_currentIndexChanged(int index)
{
    // TODO: check if needed (probably not, because we don't invoke scripts anymore)
    // on_startPushButton_clicked();
    if (index >= 1) {
        ui->ASTposangleLineEdit->setDisabled(true);
        ui->ASTpositionLineEdit->setDisabled(true);
        ui->ASTmatchflippedCheckBox->setDisabled(true);
        ui->ASTposangleLabel->setDisabled(true);
        ui->ASTpositionLabel->setDisabled(true);
    }
    else {
        ui->ASTposangleLineEdit->setEnabled(true);
        ui->ASTpositionLineEdit->setEnabled(true);
        ui->ASTmatchflippedCheckBox->setEnabled(true);
        ui->ASTposangleLabel->setEnabled(true);
        ui->ASTpositionLabel->setEnabled(true);
    }
}

void ConfDockWidget::on_ASTmethodComboBox_currentIndexChanged(int index)
{
    if (index == 0) {
        //        ui->confAstromphotom1Frame->setDisabled(true);
        ui->confAstromphotom2Frame->setEnabled(true);
        ui->confAstromphotom3Frame->setEnabled(true);
        ui->confAstromphotom4Frame->setEnabled(true);
        ui->confAstromphotom5Frame->setEnabled(true);
        ui->confAstromphotom6Frame->setEnabled(true);
        ui->ASTxcorrDTLineEdit->hide();
        ui->ASTxcorrDMINLineEdit->hide();
        ui->ASTxcorrDTLabel->hide();
        ui->ASTxcorrDMINLabel->hide();
    }
    else if (index == 1) {
        //        ui->confAstromphotom1Frame->setDisabled(true);
        ui->confAstromphotom2Frame->setDisabled(true);
        ui->confAstromphotom3Frame->setDisabled(true);
        ui->confAstromphotom4Frame->setDisabled(true);
        ui->confAstromphotom5Frame->setDisabled(true);
        ui->confAstromphotom6Frame->setDisabled(true);
        ui->ASTxcorrDTLineEdit->show();
        ui->ASTxcorrDMINLineEdit->show();
        ui->ASTxcorrDTLabel->show();
        ui->ASTxcorrDMINLabel->show();
    }
    else {
        //        ui->confAstromphotom1Frame->setEnabled(true);
        ui->confAstromphotom2Frame->setDisabled(true);
        ui->confAstromphotom3Frame->setDisabled(true);
        ui->confAstromphotom4Frame->setDisabled(true);
        ui->confAstromphotom5Frame->setDisabled(true);
        ui->confAstromphotom6Frame->setDisabled(true);
        ui->ASTxcorrDTLineEdit->hide();
        ui->ASTxcorrDMINLineEdit->hide();
        ui->ASTxcorrDTLabel->hide();
        ui->ASTxcorrDMINLabel->hide();
    }
}

void ConfDockWidget::on_COAskypaPushButton_clicked()
{
    QString maindir = mainGUI->ui->setupMainLineEdit->text();
    // Grab the first entry of the science dir
    QString science;
    if (!mainGUI->ui->setupScienceLineEdit->text().isEmpty()) {
        science = datadir2StringList(mainGUI->ui->setupScienceLineEdit).at(0);
    }
    else {
        return;
    }
    QString sciencedir = maindir+"/"+science;
    QDir dir(sciencedir);
    if (!dir.exists()) return;

    QString currentStatus = mainGUI->status.getStatusFromHistory();
    QStringList filter1("*_1"+currentStatus+".fits");
    QStringList filter2("*_1"+currentStatus+".sub.fits");
    dir.setNameFilters(filter1);
    dir.setSorting(QDir::Name);
    QStringList list = dir.entryList();
    QString sub = "";

    if (list.isEmpty()) {
        // Probably normal images were parked or removed, try skysub images
        dir.setNameFilters(filter2);
        dir.setSorting(QDir::Name);
        list = dir.entryList();
        if (list.isEmpty()) return;
        sub = ".sub";
    }

    if (list.length() == 0) {
        QMessageBox::warning( this, "Data not found!",
                              "No "+currentStatus+"-images were found in "+science+"\n");
        return;
    }

    // Images must have astrometric headers
    QString headerdir = sciencedir+"/headers/";
    bool check = hasMatchingPartnerFiles(sciencedir, currentStatus+sub+".fits", headerdir, ".head",
                                         "This task requires that all science images have astrometric headers.");
    if (!check) return;

    // Grab the first header file
    QString baseName = list.at(0);
    baseName.replace(currentStatus+sub+".fits","");
    QString headerName = headerdir+baseName+".head";
    QFile header(headerName);
    QString cd11 = "";
    QString cd12 = "";
    QString cd21 = "";
    QString cd22 = "";

    QString line;
    QString key;
    QString value;
    if ( header.open(QIODevice::ReadOnly)) {
        QTextStream stream( &header );
        while ( !stream.atEnd() ) {
            line = stream.readLine().simplified();
            if (!line.isEmpty()) {
                key = line.section(' ',0,0);
                value = line.section(' ',2,2);
                if (key == "CD1_1") cd11 = value;
                if (key == "CD1_2") cd12 = value;
                if (key == "CD2_1") cd21 = value;
                if (key == "CD2_2") cd22 = value;
            }
        }
        header.close();
    }

    if (!cd11.isEmpty()
            && !cd12.isEmpty()
            && !cd21.isEmpty()
            && !cd22.isEmpty()) {
        double skyPA = getPosAnglefromCD(cd11.toDouble(), cd12.toDouble(), cd21.toDouble(), cd22.toDouble());   // in [deg]
        // Truncate the result to one digit
        QString skyPAstring = QString::number(skyPA, 'f', 1);
        ui->COAskypaLineEdit->setText(skyPAstring);
    }
    else
        QMessageBox::warning( this, "Corrupted astrometric header",
                              "The CD matrix in "+headerName+" is incomplete, meaning the astrometry was not successful.");
}

/*
                void ConfDockWidget::on_viewerToolButton_clicked()
                {
                    QString main = ui->setupMainLineEdit->text();
                    QString science = ui->setupScienceLineEdit->text().split(" ").at(0);
                    QString dirname = main+"/"+science;
                    if (!QDir(dirname).exists()) dirname = QDir::homePath();

                    IView *iView = new IView(dirname, "*.fits", this);
                    iView->show();
                }
                */

void ConfDockWidget::on_skyAreaComboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 != "Specific sky area(s)") ui->skyAreaPushButton->setDisabled(true);
    else ui->skyAreaPushButton->setEnabled(true);
}

void ConfDockWidget::on_skyAreaPushButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Use the middle mouse button (click and drag) to select one or more empty sky areas. "
                   "You can use the forward / backward buttons to select other images. Close the viewer when finished.\n\n"
                   "Alternatively, you can open any astrometrically calibrated image in iView, e.g. a previous coaddition of the same data. "
                   "Select the dropper tool at the upper left and then use the middle mouse button as explained above.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    QString maindir = mainGUI->ui->setupMainLineEdit->text();
    // Grab the first entry of the science dir
    QString science;
    if (!mainGUI->ui->setupScienceLineEdit->text().isEmpty()) {
        science = datadir2StringList(mainGUI->ui->setupScienceLineEdit).at(0);
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("The science directory is empty.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
    QString sciencedir = maindir+"/"+science;
    if (!QDir(sciencedir).exists()) {
        QMessageBox msgBox;
        msgBox.setText("The science directory"+sciencedir+"does not exist.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
    launchViewer("SkyMode");
}

//void ConfDockWidget::launchViewer(const QString &dirname, const QString &filter, const QString &mode)
void ConfDockWidget::launchViewer(const QString &mode)
{
    // Load the plot viewer
    //    IView *iView = new IView(dirname, filter, false);

    // IView *iView = new IView("FITSmonochrome", dirname, filter, this);
    //    IView *iView = new IView(dirname, filter);
    //    iView->show();
    //    iView->setMiddleMouseMode(mode);

    QString mainDirName = mainGUI->controller->mainDirName;
    QString scienceDirName = mainGUI->controller->DT_SCIENCE[0]->subDirName;
    QString dirName = mainDirName + "/"+scienceDirName;
    IView *iView = new IView("MEMview", mainGUI->controller->DT_SCIENCE[0]->myImageList[0], dirName, this);
    iView->scene->clear();
    MyImage *it = mainGUI->controller->DT_SCIENCE[0]->myImageList[0][0];
    iView->loadFromRAM(it, 3);
    iView->currentId = 0;
    // IView needs to know the directory name so that it can overlay catalogs
    iView->show();
    iView->setMiddleMouseMode(mode);
    //    iView->raise();
}

void ConfDockWidget::on_ARCcatalogComboBox_currentTextChanged(const QString &arg1)
{
    if (arg1.contains("GAIA") || arg1.contains("UCAC5")) ui->gaiaFrame->show();
    else ui->gaiaFrame->hide();
}

void ConfDockWidget::on_ARCselectimagePushButton_clicked()
{
    QString currentFileName =
            QFileDialog::getOpenFileName(this, tr("Select astrometrically calibrated image"), QDir::homePath(),
                                         tr("Images (*.fits *.fit *.fts *.FIT *.FITS *.FTS);; All (*.*)"));

    if ( currentFileName.isEmpty() ) return;

    ui->ARCselectimageLineEdit->setText(currentFileName);
}

void ConfDockWidget::updateGaiaBulkMotion(const QString &pmRA, const QString &pmDE)
{
    ui->ARCpmRALineEdit->setText(pmRA);
    ui->ARCpmDECLineEdit->setText(pmDE);
}

void ConfDockWidget::on_confForwardPushButton_clicked()
{
    int count = ui->confStackedWidget->count();
    int current = ui->confStackedWidget->currentIndex();
    ui->confBackwardPushButton->setDisabled(false);
    if (current >= count-2) {
        ui->confForwardPushButton->setDisabled(true);
        ui->confBackwardPushButton->setFocus();
    }
    if (current < count-1) ui->confStackedWidget->setCurrentIndex(current+1);
    updateConfPageLabel();
}

void ConfDockWidget::on_confBackwardPushButton_clicked()
{
    int current = ui->confStackedWidget->currentIndex();
    ui->confForwardPushButton->setDisabled(false);
    if (current <= 1) {
        ui->confBackwardPushButton->setDisabled(true);
        ui->confForwardPushButton->setFocus();
    }
    if (current > 0) ui->confStackedWidget->setCurrentIndex(current-1);
    updateConfPageLabel();
}

void ConfDockWidget::updateConfPageLabel()
{
    int current = ui->confStackedWidget->currentIndex();
    ui->confPageLabel->setText("Page "+QString::number(current+1)+"/9");
    ui->parametersDefaultPushButton->setText("Defaults for page "+QString::number(current+1));
}

void ConfDockWidget::setupAstrometry()
{
    const QStandardItemModel* astrometryMatchModel = dynamic_cast< QStandardItemModel * >( ui->ASTmatchMethodComboBox->model() );

    // Enable / disable matching methods
    QString anetSolver = findExecutableName("solve-field");
    QString anetIndex = findExecutableName("build-astrometry-index");
    bool anet = true;
    if (anetSolver.isEmpty() || anetIndex.isEmpty()) anet = false;
    astrometryMatchModel->item( 0,0 )->setEnabled( true );
    astrometryMatchModel->item( 1,0 )->setEnabled( anet );
    astrometryMatchModel->item( 2,0 )->setEnabled( true );
    if (!doingInitialLaunch) {
        ui->ASTmatchMethodComboBox->setCurrentIndex(0);
    }
}

void ConfDockWidget::setupXtalkButtonGroups()
{
    norxtalkButtonGroup->addButton(ui->xtalk_nor_2x2ToolButton);
    norxtalkButtonGroup->addButton(ui->xtalk_nor_1x2ToolButton);
    norxtalkButtonGroup->addButton(ui->xtalk_nor_2x1ToolButton);
    norxtalkButtonGroup->setId(ui->xtalk_nor_2x2ToolButton, 0);
    norxtalkButtonGroup->setId(ui->xtalk_nor_1x2ToolButton, 1);
    norxtalkButtonGroup->setId(ui->xtalk_nor_2x1ToolButton, 2);
    ui->xtalk_nor_2x2ToolButton->setChecked(true);

    rowxtalkButtonGroup->addButton(ui->xtalk_row_2x2ToolButton);
    rowxtalkButtonGroup->addButton(ui->xtalk_row_1x2ToolButton);
    rowxtalkButtonGroup->addButton(ui->xtalk_row_2x1ToolButton);
    rowxtalkButtonGroup->addButton(ui->xtalk_col_2x2ToolButton);
    rowxtalkButtonGroup->addButton(ui->xtalk_col_1x2ToolButton);
    rowxtalkButtonGroup->addButton(ui->xtalk_col_2x1ToolButton);
    rowxtalkButtonGroup->setId(ui->xtalk_row_2x2ToolButton, 0);
    rowxtalkButtonGroup->setId(ui->xtalk_row_1x2ToolButton, 1);
    rowxtalkButtonGroup->setId(ui->xtalk_row_2x1ToolButton, 2);
    rowxtalkButtonGroup->setId(ui->xtalk_col_2x2ToolButton, 3);
    rowxtalkButtonGroup->setId(ui->xtalk_col_1x2ToolButton, 4);
    rowxtalkButtonGroup->setId(ui->xtalk_col_2x1ToolButton, 5);
    ui->xtalk_row_2x2ToolButton->setChecked(true);
}

void ConfDockWidget::setupInstrumentComboBox_clicked()
{
    // The structure holding the content of the instrument file
    // TODO: check if actually needed
    //  initInstrumentData(instrument_dir+instrument_name+".ini");

    //if (!readingSettings) {
    //    ui->COApixscaleLineEdit->setText(get_fileparameter(&instrument_file, "PIXSCALE"));
    //}

    // Update some comboboxes
    ui->skyAreaComboBox->clear();
    ui->skyAreaComboBox->addItem("Specific sky area(s)");
    if (instrument_nchips == 1) {
        ui->skyAreaComboBox->addItem("From entire chip");
    }
    else {
        ui->skyAreaComboBox->addItem("From each chip");
        for (int i=1; i<=instrument_nchips; ++i) {
            QString entry = "From chip "+QString::number(i);
            ui->skyAreaComboBox->addItem(entry);
        }
    }

    int currentASTmethod = ui->ASTmethodComboBox->currentIndex();
    // (de)activate some GUI elements depending on wavelength regime;
    // the following two lines are needed to (de)activate some elements in a ComboBox
    const QStandardItemModel* astrom_model = dynamic_cast< QStandardItemModel * >( ui->ASTmethodComboBox->model() );
    const QStandardItemModel* mosaictype_model = dynamic_cast< QStandardItemModel * >( ui->ASTmosaictypeComboBox->model() );
    const QStandardItemModel* focalplane_model = dynamic_cast< QStandardItemModel * >( ui->ASTfocalplaneComboBox->model() );
    const QStandardItemModel* std_model = dynamic_cast< QStandardItemModel * >( ui->APIrefcatComboBox->model() );

    if (instrument_nchips == 1) {
        mosaictype_model->item( 1,0 )->setEnabled( false );
        mosaictype_model->item( 2,0 )->setEnabled( false );
        mosaictype_model->item( 3,0 )->setEnabled( false );
        mosaictype_model->item( 4,0 )->setEnabled( false );
        focalplane_model->item( 1,0 )->setEnabled( false );
        focalplane_model->item( 2,0 )->setEnabled( false );
        if (!doingInitialLaunch) {
            ui->ASTmosaictypeComboBox->setCurrentIndex(0);
            ui->ASTfocalplaneComboBox->setCurrentIndex(0);
        }
        // Show the option of masking a rectangular region for collapse correction
        ui->confsubtitleCollapse3Label->show();
        ui->confCollapse3Frame->show();
    }
    else {
        mosaictype_model->item( 1,0 )->setEnabled( true );
        mosaictype_model->item( 2,0 )->setEnabled( true );
        mosaictype_model->item( 3,0 )->setEnabled( true );
        mosaictype_model->item( 4,0 )->setEnabled( true );
        focalplane_model->item( 1,0 )->setEnabled( true );
        focalplane_model->item( 2,0 )->setEnabled( true );
        if (ui->ASTstabilityComboBox->currentText() == "INSTRUMENT") {
            ui->ASTmosaictypeComboBox->setCurrentIndex(3);
            ui->ASTfocalplaneComboBox->setCurrentIndex(0);
        }
        else if (ui->ASTstabilityComboBox->currentText() == "EXPOSURE") {
            ui->ASTmosaictypeComboBox->setCurrentIndex(4);
            ui->ASTfocalplaneComboBox->setCurrentIndex(0);
        }
        // Hide the option of masking a rectangular region for collapse correction
        ui->confsubtitleCollapse3Label->hide();
        ui->confCollapse3Frame->hide();
    }

    if (instrument_type == "MIR") {
        ui->splitMIRcubeCheckBox->show();
        ui->confChopnodSpacer->changeSize(1,30);
        ui->confChopnodFrame->show();
        ui->conftitleChopnodLabel->show();
        ui->confsubtitleBackground1Label->setDisabled(true);
        ui->confsubtitleBackground2Label->setDisabled(true);
        ui->confsubtitleBackground3Label->setDisabled(true);
        ui->confsubtitleBackground4Label->setDisabled(true);
        ui->confsubtitleBackground5Label->setDisabled(true);
        ui->confsubtitleBackground6Label->setDisabled(true);
        ui->confBackground1Frame->setDisabled(true);
        ui->confBackground2Frame->setDisabled(true);
        ui->confBackground3Frame->setDisabled(true);
        ui->confBackground4Frame->setDisabled(true);
        ui->confBackground5Frame->setDisabled(true);
        ui->confBackground6Frame->setDisabled(true);
        ui->ASTmethodComboBox->setCurrentIndex(2);
        ui->confAstromphotom1Frame->setDisabled(true);
        ui->confAstromphotom2Frame->setDisabled(true);
        ui->confAstromphotom3Frame->setDisabled(true);
        ui->confAstromphotom4Frame->setDisabled(true);
        ui->confAstromphotom5Frame->setDisabled(true);
        ui->CIWmaskbloomingCheckBox->setDisabled(true);
        ui->confIndividualweight2Frame->setDisabled(true);
        astrom_model->item( 0,0 )->setEnabled( false );
    }
    else if (instrument_type == "NIR") {
        ui->splitMIRcubeCheckBox->hide();
        ui->confChopnodFrame->hide();
        ui->conftitleChopnodLabel->hide();
        ui->confChopnodSpacer->changeSize(1,0);
        ui->confsubtitleBackground1Label->setEnabled(true);
        ui->confsubtitleBackground1Label->show();
        ui->confsubtitleBackground2Label->setEnabled(true);
        ui->confsubtitleBackground3Label->setEnabled(true);
        ui->confsubtitleBackground4Label->setEnabled(true);
        ui->confsubtitleBackground5Label->setEnabled(true);
        ui->confsubtitleBackground6Label->setEnabled(true);
        ui->confBackground1Frame->setEnabled(true);
        ui->confBackground1Frame->show();
        ui->confBackground2Frame->setEnabled(true);
        ui->confBackground3Frame->setEnabled(true);
        ui->confBackground4Frame->setEnabled(true);
        ui->confBackground5Frame->setEnabled(true);
        ui->confBackground6Frame->setEnabled(true);
        if (currentASTmethod < 2) ui->ASTmethodComboBox->setCurrentIndex(currentASTmethod);
        else ui->ASTmethodComboBox->setCurrentIndex(0);
        ui->confAstromphotom1Frame->setEnabled(true);
        ui->confAstromphotom2Frame->setEnabled(true);
        ui->confAstromphotom3Frame->setEnabled(true);
        ui->confAstromphotom4Frame->setEnabled(true);
        ui->confAstromphotom5Frame->setEnabled(true);
        ui->CIWmaskbloomingCheckBox->setDisabled(true);
        //        ui->confIndividualweight2Frame->setDisabled(true);
        astrom_model->item( 0,0 )->setEnabled( true );
        std_model->item( 0,0 )->setEnabled( false );
        std_model->item( 1,0 )->setEnabled( false );
        std_model->item( 2,0 )->setEnabled( false );
        std_model->item( 3,0 )->setEnabled( false );
        std_model->item( 4,0 )->setEnabled( false );
        std_model->item( 5,0 )->setEnabled( false );
        std_model->item( 6,0 )->setEnabled( true );
        std_model->item( 7,0 )->setEnabled( true );
        std_model->item( 8,0 )->setEnabled( true );
        std_model->item( 9,0 )->setEnabled( true );
        std_model->item( 10,0 )->setEnabled( true );
        std_model->item( 11,0 )->setEnabled( true );
    }
    else if (instrument_type == "NIRMIR") {
        ui->splitMIRcubeCheckBox->show();
        ui->confChopnodFrame->show();
        ui->conftitleChopnodLabel->show();
        ui->confChopnodSpacer->changeSize(1,30);
        ui->confsubtitleBackground1Label->setEnabled(true);
        ui->confsubtitleBackground2Label->setEnabled(true);
        ui->confsubtitleBackground3Label->setEnabled(true);
        ui->confsubtitleBackground4Label->setEnabled(true);
        ui->confsubtitleBackground5Label->setEnabled(true);
        ui->confsubtitleBackground6Label->setEnabled(true);
        ui->confBackground1Frame->setEnabled(true);
        ui->confBackground2Frame->setEnabled(true);
        ui->confBackground3Frame->setEnabled(true);
        ui->confBackground4Frame->setEnabled(true);
        ui->confBackground5Frame->setEnabled(true);
        ui->confBackground6Frame->setEnabled(true);
        if (currentASTmethod < 2) ui->ASTmethodComboBox->setCurrentIndex(currentASTmethod);
        else ui->ASTmethodComboBox->setCurrentIndex(0);
        ui->confAstromphotom1Frame->setEnabled(true);
        ui->confAstromphotom2Frame->setEnabled(true);
        ui->confAstromphotom3Frame->setEnabled(true);
        ui->confAstromphotom4Frame->setEnabled(true);
        ui->confAstromphotom5Frame->setEnabled(true);
        ui->CIWmaskbloomingCheckBox->setDisabled(true);
        //        ui->confIndividualweight2Frame->setDisabled(true);
        astrom_model->item( 0,0 )->setEnabled( true );
        std_model->item( 0,0 )->setEnabled( false );
        std_model->item( 1,0 )->setEnabled( false );
        std_model->item( 2,0 )->setEnabled( false );
        std_model->item( 3,0 )->setEnabled( false );
        std_model->item( 4,0 )->setEnabled( false );
        std_model->item( 5,0 )->setEnabled( false );
        std_model->item( 6,0 )->setEnabled( true );
        std_model->item( 7,0 )->setEnabled( true );
        std_model->item( 8,0 )->setEnabled( true );
        std_model->item( 9,0 )->setEnabled( true );
        std_model->item( 10,0 )->setEnabled( true );
        std_model->item( 11,0 )->setEnabled( true );
    }
    else if (instrument_type == "OPT" || instrument_type == "") {
        ui->splitMIRcubeCheckBox->hide();
        ui->confChopnodSpacer->changeSize(1,0);
        ui->confChopnodFrame->hide();
        ui->conftitleChopnodLabel->hide();
        ui->confsubtitleBackground1Label->hide();
        ui->confsubtitleBackground2Label->setEnabled(true);
        ui->confsubtitleBackground3Label->setEnabled(true);
        ui->confsubtitleBackground4Label->setEnabled(true);
        ui->confsubtitleBackground5Label->setEnabled(true);
        ui->confsubtitleBackground6Label->setEnabled(true);
        ui->confBackground1Frame->hide();
        ui->confBackground2Frame->setEnabled(true);
        ui->confBackground3Frame->setEnabled(true);
        ui->confBackground4Frame->setEnabled(true);
        ui->confBackground5Frame->setEnabled(true);
        ui->confBackground6Frame->setEnabled(true);
        if (currentASTmethod < 2) ui->ASTmethodComboBox->setCurrentIndex(currentASTmethod);
        else ui->ASTmethodComboBox->setCurrentIndex(0);
        ui->confAstromphotom1Frame->setEnabled(true);
        ui->confAstromphotom2Frame->setEnabled(true);
        ui->confAstromphotom3Frame->setEnabled(true);
        ui->confAstromphotom4Frame->setEnabled(true);
        ui->confAstromphotom5Frame->setEnabled(true);
        ui->CIWmaskbloomingCheckBox->setEnabled(true);
        ui->confIndividualweight2Frame->setEnabled(true);
        // The following line triggers repaintDataDirs. Why?
        astrom_model->item( 0,0 )->setEnabled( true );
        std_model->item( 0,0 )->setEnabled( true );
        std_model->item( 1,0 )->setEnabled( true );
        std_model->item( 2,0 )->setEnabled( true );
        std_model->item( 3,0 )->setEnabled( true );
        std_model->item( 4,0 )->setEnabled( true );
        std_model->item( 5,0 )->setEnabled( true );
        std_model->item( 6,0 )->setEnabled( false );
        std_model->item( 7,0 )->setEnabled( false );
        std_model->item( 8,0 )->setEnabled( false );
        std_model->item( 9,0 )->setEnabled( false );
        std_model->item( 10,0 )->setEnabled( false );
        std_model->item( 11,0 )->setEnabled( false );
    }

    load_default_stdcat();

    // Update settings
    // TODO: make sure this does not happen in parallel to the same task in MainWindow
    // MUST UNCOMMENT
    // writeGUISettings();
}

void ConfDockWidget::updateAPIsolutions()
{
    QString standard = getNthWord(mainGUI->ui->setupStandardLineEdit->text(),1);
    QDir standardDir = QDir(mainGUI->ui->setupMainLineEdit->text()+"/"+standard+"/calib/");
    QStringList filter("night*.dat");
    QStringList solutionList = standardDir.entryList(filter);

    if (solutionList.isEmpty()) ui->APInodataLabel->show();
    else ui->APInodataLabel->hide();

    int numNights = solutionList.length();
    int i=0;
    for (auto &it : APIstdPushButtonList) {
        if (i<numNights) {
            QString buttonText = solutionList.at(i);
            buttonText = buttonText.remove(".dat");
            it->setText("Show "+buttonText);
            it->show();
        }
        else {
            it->hide();
        }
        ++i;
    }
    i=0;
    for (auto &it : APIstdComboBoxList) {
        if (i<numNights) it->show();
        else it->hide();
        ++i;
    }
}

// Not yet implemented
void ConfDockWidget::showAPIsolution()
{
    //    QString standard = getNthWord(mainGUI->ui->setupStandardLineEdit->text(),1);
    //    QPushButton* pushbutton = qobject_cast<QPushButton*>(sender());
    //    QString solutionPlotPath = mainGUI->ui->setupMainLineEdit->text()+"/"+standard+"/calib/";
    //    launchViewer(solutionPlotPath, pushbutton->text()+"_result.png", "");  // probably have to define a new IView mode for this
}

void ConfDockWidget::on_ASTreadmepushButton_clicked()
{
    scampreadme = new ScampReadme(this);
    scampreadme->show();
}

bool ConfDockWidget::checkRightScampMode(const QString &coordsMode)
{
    if (coordsMode != "execute") return true;
    if (instrument_nchips == 1) return true;
    if (ui->ASTmosaictypeComboBox->currentText() == "UNCHANGED"
            || (ui->ASTmosaictypeComboBox->currentText() == "FIX_FOCALPLANE" &&
                ui->ASTfocalplaneComboBox->currentText() != "IGNORE_PRIOR")) {
        QMessageBox msgBox;
        msgBox.setText("Likely inconsistent Scamp setup:\n'MOSAIC_TYPE = UNCHANGED' or\n"
                       "'MOSAIC_TYPE = FIX_FOCALPLANE' and 'FPA mode = IGNORE_PRIOR'");
        msgBox.setInformativeText("Relative chip orientations as encoded by the reference pixels and CD matrix "
                                  "may vary over short or long time scales. The best way is to re-measure the "
                                  "relative orientations from the data themselves. Five well-dithered exposures "
                                  "are usually sufficient, in particular if calibrated against GAIA. To do this, use\n\n"
                                  "'MOSAIC_TYPE = FIX_FOCALPLANE' and 'FPA mode = IGNORE_PRIOR'. \n\n"
                                  "If that doesn't work, try \n\n"
                                  "'MOSAIC_TYPE = LOOSE' and 'FPA mode = IGNORE_PRIOR'.\n\n"
                                  "If that does not work, try using the pre-determined focal plane array (FPA):\n\n "
                                  "'MOSAIC_TYPE = SAME_CRVAL' and 'FPA mode = USE_PRIOR'.\n\n");
        QAbstractButton* pButtonContinue = msgBox.addButton(tr("Continue as is"), QMessageBox::YesRole);
        QAbstractButton* pButtonDetails = msgBox.addButton(tr("Details [...]"), QMessageBox::YesRole);
        QAbstractButton* pButtonCancel = msgBox.addButton(tr("Cancel"), QMessageBox::YesRole);
        msgBox.exec();
        if (msgBox.clickedButton()==pButtonContinue) return true;
        else if (msgBox.clickedButton()==pButtonDetails) {
            scampreadme = new ScampReadme(this);
            scampreadme->show();
            return false;
        }
        else if (msgBox.clickedButton()==pButtonCancel) {
            return false;
        }
    }
    else return true;

    return true;
}

void ConfDockWidget::on_nonlinearityCheckBox_clicked()
{
    if (!ui->nonlinearityCheckBox->isChecked()) return;

    QString coeffsFileName = instrument_dir+"/"+instrument_name+".nldat";
    QFile coeffsFile(coeffsFileName);
    if (!coeffsFile.exists() || !coeffsFile.open(QIODevice::ReadOnly)) {
        QMessageBox msgBox;
        msgBox.setText("Nonlinearity correction for "+instrument_name+" is not implemented.\n"
                       +"Contact the author if you have the polynomial correction coefficients available.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        ui->nonlinearityCheckBox->setChecked(false);
    }
}

void ConfDockWidget::on_ASTviewCheckPlotsPushButton_clicked()
{
    Data *scienceData = nullptr;

    if (mainGUI->controller->DT_SCIENCE.length() == 1) {
        scienceData = mainGUI->controller->DT_SCIENCE.at(0);
    }
    else if (mainGUI->controller->DT_SCIENCE.length() == 0) {
        QMessageBox::information(this, tr("Missing data"),
                                 tr("No SCIENCE data were specified in the data tree.\n"),
                                 QMessageBox::Ok);
        return;
    }

    // See if any checkplots exist
    bool checkPlotsExist = false;
    for (auto &data : mainGUI->controller->DT_SCIENCE) {
        QDir plotsDir = data->dirName+"/plots/";
        if (plotsDir.exists()) checkPlotsExist = true;
    }

    if (checkPlotsExist) {
        if (mainGUI->controller->DT_SCIENCE.length() > 1) {
            QMessageBox msgBox;
            msgBox.setInformativeText(tr("The current SCIENCE data tree contains several entries for which astrometry check plots exist.\n") +
                                      tr("Display the plots for:\n\n"));
            for (auto &data : mainGUI->controller->DT_SCIENCE) {
                QDir plotsDir = data->dirName+"/plots/";
                if (plotsDir.exists()) msgBox.addButton(data->subDirName, QMessageBox::YesRole);
            }
            QAbstractButton *pCancel = msgBox.addButton(tr("Cancel"), QMessageBox::NoRole);
            msgBox.exec();
            QString choice = msgBox.clickedButton()->text().remove('&');  // remove & is a KDE fix
            if (msgBox.clickedButton()== pCancel) return;

            for (auto &data : mainGUI->controller->DT_SCIENCE) {
                if (data->subDirName == choice) {
                    scienceData = data;
                    break;
                }
            }
        }
        IView *checkplotViewer = new IView("SCAMP_VIEWONLY", scienceData->dirName+"/plots/", this);
        checkplotViewer->show();
    }
    else {
        QMessageBox msgBox;
        msgBox.setInformativeText(tr("Could not find astrometric check plots in any of the science directories.\n") +
                                  tr("You must run the astrometry first.\n"));
        msgBox.addButton(tr("OK"), QMessageBox::YesRole);
        msgBox.exec();
    }
}
