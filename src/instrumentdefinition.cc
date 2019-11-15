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

#include "instrumentdefinition.h"
#include "ui_instrumentdefinition.h"
#include "functions.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

Instrument::Instrument(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Instrument)
{
    ui->setupUi(this);

    geometryList.append(ui->overscanxMinLineEdit);
    geometryList.append(ui->overscanxMaxLineEdit);
    geometryList.append(ui->overscanyMinLineEdit);
    geometryList.append(ui->overscanyMaxLineEdit);
    geometryList.append(ui->startxLineEdit);
    geometryList.append(ui->startyLineEdit);
    geometryList.append(ui->sizexLineEdit);
    geometryList.append(ui->sizeyLineEdit);
    geometryList.append(ui->crpix1LineEdit);
    geometryList.append(ui->crpix2LineEdit);

    geometryNumList.append(ui->overscanxMinNumLineEdit);
    geometryNumList.append(ui->overscanxMaxNumLineEdit);
    geometryNumList.append(ui->overscanyMinNumLineEdit);
    geometryNumList.append(ui->overscanyMaxNumLineEdit);
    geometryNumList.append(ui->startxNumLineEdit);
    geometryNumList.append(ui->startyNumLineEdit);
    geometryNumList.append(ui->sizexNumLineEdit);
    geometryNumList.append(ui->sizeyNumLineEdit);
    geometryNumList.append(ui->crpix1NumLineEdit);
    geometryNumList.append(ui->crpix2NumLineEdit);

    for (auto &it: geometryList) {
        connect(it, &QLineEdit::textChanged, this, &Instrument::validate);
        connect(it, &QLineEdit::textChanged, this, &Instrument::paintNumLineEdits);
    }
    connect(ui->latitudeLineEdit, &QLineEdit::textChanged, this, &Instrument::validate);
    connect(ui->longitudeLineEdit, &QLineEdit::textChanged, this, &Instrument::validate);
    connect(ui->plateScaleLineEdit, &QLineEdit::textChanged, this, &Instrument::validate);
    connect(ui->bayerRGGBToolButton, &QToolButton::clicked, this, &Instrument::toggle_bayer_ToolButtons);
    connect(ui->bayerGBRGToolButton, &QToolButton::clicked, this, &Instrument::toggle_bayer_ToolButtons);
    connect(ui->bayerBGGRToolButton, &QToolButton::clicked, this, &Instrument::toggle_bayer_ToolButtons);
    connect(ui->bayerGRBGToolButton, &QToolButton::clicked, this, &Instrument::toggle_bayer_ToolButtons);

    initEnvironment(thelidir, userdir, tmpdir);

    ui->bayerCheckBox->setChecked(false);
    bayerButtonGroup->addButton(ui->bayerGBRGToolButton);
    bayerButtonGroup->addButton(ui->bayerRGGBToolButton);
    bayerButtonGroup->addButton(ui->bayerBGGRToolButton);
    bayerButtonGroup->addButton(ui->bayerGRBGToolButton);
    bayerButtonGroup->setExclusive(true);
    ui->bayerGBRGToolButton->setIcon(QIcon(":/images/bayer_gbrg.png"));
    ui->bayerRGGBToolButton->setIcon(QIcon(":/images/bayer_rggb.png"));
    ui->bayerBGGRToolButton->setIcon(QIcon(":/images/bayer_bggr.png"));
    ui->bayerGRBGToolButton->setIcon(QIcon(":/images/bayer_grbg.png"));
    ui->buttonFrame->hide();

    paintNumLineEdits("");

    setWindowIcon(QIcon(":/images/addInst.png"));

    applyStyleSheets();
}

Instrument::~Instrument()
{
    delete ui;
}

void Instrument::applyStyleSheets()
{
    /*
    QList<QLabel*> titleList;
    titleList.append(ui->title1Label);
    for (auto &it : titleList) {
        it->setStyleSheet("color: rgb(150, 240, 240); background-color: rgb(58, 78, 98);");
        it->setMargin(8);
    }
    */

    QList<QLabel*> subtitleList;
    subtitleList.append(ui->subtitle1Label);
    subtitleList.append(ui->subtitle2Label);
    subtitleList.append(ui->subtitle5Label);
    for (auto &it : subtitleList) {
        it->setStyleSheet("background-color: rgb(190,190,210);");
        it->setMargin(4);
    }

    QList<QFrame*> frameList;
    frameList.append(ui->inst1Frame);
    frameList.append(ui->inst2Frame);
    frameList.append(ui->inst6Frame);
    for (auto &it : frameList) {
        //        it->setStyleSheet("background-color: #ebebee;");
        // nothing yet
    }

    QList<QToolButton*> buttonList;
    buttonList.append(ui->bayerBGGRToolButton);
    buttonList.append(ui->bayerRGGBToolButton);
    buttonList.append(ui->bayerGBRGToolButton);
    buttonList.append(ui->bayerGRBGToolButton);
    for (auto &it : buttonList) {
        //
    }
}

void Instrument::toggle_bayer_ToolButtons()
{
    QToolButton *toolbutton = qobject_cast<QToolButton*>(sender());
    QString sender_name = toolbutton->objectName();

    if (toolbutton->isChecked()) this->setStyleSheet("QToolButton#"+sender_name+" { background-color: black }");
    else this->setStyleSheet("QToolButton#"+sender_name+" { background-color: #ddddddd }");
}

void Instrument::paintNumLineEdits(QString geometry)
{
    QPalette paletteMatch;
    QPalette paletteNoMatch;
    paletteMatch.setColor(QPalette::Base,QColor("#a9ffe6"));
    paletteNoMatch.setColor(QPalette::Base,QColor("#ff99aa"));
    int numEntries;
    geometry = geometry.simplified();

    // Triggered by LineEdit changed
    QLineEdit *le = qobject_cast<QLineEdit*>(sender());
    if (le != 0) {
        if (geometry.isEmpty()) numEntries = 0;
        else numEntries = geometry.split(' ').length();
        for (int i=0; i<geometryList.length(); ++i) {
            if (le->objectName() == geometryList.at(i)->objectName()) {
                if (numEntries == ui->numchipSpinBox->value()) geometryNumList.at(i)->setPalette(paletteMatch);
                else geometryNumList.at(i)->setPalette(paletteNoMatch);
                geometryNumList.at(i)->setText(QString::number(numEntries));
            }
        }
    }
    else {
        // Triggered by numchipSpinBox value changed
        for (int i=0; i<geometryList.length(); ++i) {
            if (geometryList.at(i)->text().isEmpty()) numEntries = 0;
            else numEntries = geometryList.at(i)->text().split(' ').length();
            if (numEntries == ui->numchipSpinBox->value()) geometryNumList.at(i)->setPalette(paletteMatch);
            else geometryNumList.at(i)->setPalette(paletteNoMatch);
            geometryNumList.at(i)->setText(QString::number(numEntries));
        }
    }
}

void Instrument::on_numchipSpinBox_valueChanged(int arg1)
{
    numChips = arg1;
    paintNumLineEdits("");
}

void Instrument::validate(QString arg1)
{
    if (arg1.isEmpty()) return;

    QRegExp rf1( "[-0-9.]+" );
    QValidator* validator_float = new QRegExpValidator(rf1, this );
    QRegExp re( "[-0-9e.]+" );
    QRegExp rf2( "[0-9.]+" );
    QValidator* validator_floatpos = new QRegExpValidator(rf2, this );
    QRegExp ri3( "[0-9\\s]+" );
    QValidator* validator_intposblank = new QRegExpValidator(ri3, this );
    QRegExp ri4( "[-0-9\\s]+" );
    QValidator* validator_intblank = new QRegExpValidator(ri4, this );
    ui->latitudeLineEdit->setValidator( validator_float );
    ui->longitudeLineEdit->setValidator( validator_float );
    ui->gainLineEdit->setValidator( validator_floatpos );
    ui->plateScaleLineEdit->setValidator( validator_floatpos );
    ui->overscanxMinLineEdit->setValidator( validator_intposblank );
    ui->overscanxMaxLineEdit->setValidator( validator_intposblank);
    ui->overscanyMinLineEdit->setValidator( validator_intposblank );
    ui->overscanyMaxLineEdit->setValidator( validator_intposblank);
    ui->startxLineEdit->setValidator( validator_intposblank );
    ui->startyLineEdit->setValidator( validator_intposblank );
    ui->sizexLineEdit->setValidator( validator_intposblank );
    ui->sizeyLineEdit->setValidator( validator_intposblank );
    ui->crpix1LineEdit->setValidator( validator_intblank );
    ui->crpix2LineEdit->setValidator( validator_intblank );

    // instrument name must not accept blank characters
    QRegExp rx2( "^\\S+$" );
    QValidator* validator_string2 = new QRegExpValidator( rx2, this );
    ui->instrumentNameLineEdit->setValidator( validator_string2 );
}

QString Instrument::geometryToConfig(QString geometry)
{
    // Transforms the entered blank-separated geometry to a bash array
    QStringList list = geometry.simplified().split(' ');
    QString bashArray = "";
    int i=1;
    for (auto &it : list) {
        bashArray.append("["+QString::number(i)+"]="+it+" ");
        ++i;
    }
    bashArray = bashArray.simplified();
    bashArray.prepend("(");
    bashArray.append(")");

    return bashArray;
}

QString Instrument::configToGeometry(QString config)
{
    // Transforms the bash vector to a blank-separated list
    config = config.remove("(");
    config = config.remove(")");
    config = config.remove("[");
    config = config.remove("]");
    config = config.replace('=',' ');
    QStringList list = config.simplified().split(' ');
    QString geometry = "";

    // Keep every second word, only
    int i=1;
    for (auto &it : list) {
        if (! (i%2)) geometry.append(it+" ");
        ++i;
    }
    return geometry.simplified();
}

void Instrument::on_saveConfigPushButton_clicked()
{
    QString instrumentName = ui->instrumentNameLineEdit->text();
    QString filePath = QDir::homePath()+"/.theli/instruments_user/"+instrumentName+".ini";
    QFile configfile(filePath);

    if (!compareChipNumbers()) {
        QMessageBox::warning( this, "Mismatch in detector numbers",
                              "<b>Warning:</b><br> The number of entries in one or more of the\n"
                              "detector geometries does not match the number of chips declared for the camera.<br>\n");
        return;
    }

    if (configfile.exists()) {
        QMessageBox msgBox;
        msgBox.setText("A configuration file for "+instrumentName+" already exists.");
        QAbstractButton* overwrite = msgBox.addButton(tr("Overwrite"), QMessageBox::YesRole);
        QAbstractButton* cancel = msgBox.addButton(tr("Cancel"), QMessageBox::NoRole);
        msgBox.exec();
        if (msgBox.clickedButton() == overwrite) {
            // do nothing
        }
        else if (msgBox.clickedButton() == cancel) return;
    }

    if (configfile.open(QIODevice::WriteOnly)) {
        QTextStream outputStream(&configfile);
        outputStream << "INSTRUMENT=" << instrumentName << "\n";
        outputStream << "NCHIPS=" << ui->numchipSpinBox->value() << "\n";
        outputStream << "# Geographic location\n";
        outputStream << "OBSLAT=" << ui->latitudeLineEdit->text() << "\n";
        outputStream << "OBSLONG=" << ui->longitudeLineEdit->text() << "\n\n";
        outputStream << "# Plate scale\n";
        outputStream << "PIXSCALE=" << ui->plateScaleLineEdit->text().toFloat() << "\n\n";
        outputStream << "# Detector gain (lowest gain for a multi-detector instrument, i.e. brightest image)\n";
        outputStream << "GAIN=" << ui->gainLineEdit->text() << "\n\n";
        outputStream << "# Detector geometries\n";
        outputStream << "OVSCANX1=" << geometryToConfig(ui->overscanxMinLineEdit->text()) << "\n";
        outputStream << "OVSCANX2=" << geometryToConfig(ui->overscanxMaxLineEdit->text()) << "\n";
        outputStream << "OVSCANY1=" << geometryToConfig(ui->overscanyMinLineEdit->text()) << "\n";
        outputStream << "OVSCANY2=" << geometryToConfig(ui->overscanyMaxLineEdit->text()) << "\n";
        outputStream << "CUTX=" << geometryToConfig(ui->startxLineEdit->text()) << "\n";
        outputStream << "CUTY=" << geometryToConfig(ui->startyLineEdit->text()) << "\n";
        outputStream << "SIZEX=" << geometryToConfig(ui->sizexLineEdit->text()) << "\n";
        outputStream << "SIZEY=" << geometryToConfig(ui->sizeyLineEdit->text()) << "\n";
        outputStream << "REFPIXX=" << geometryToConfig(ui->crpix1LineEdit->text()) << "\n";
        outputStream << "REFPIXY=" << geometryToConfig(ui->crpix2LineEdit->text()) << "\n\n";
        outputStream << "# Camera type\n";
        QString type;
        if (ui->instrumentTypeComboBox->currentIndex() == 0) type = "OPT";
        else if (ui->instrumentTypeComboBox->currentIndex() == 1) type = "NIR";
        else if (ui->instrumentTypeComboBox->currentIndex() == 2) type = "NIRMIR";
        else if (ui->instrumentTypeComboBox->currentIndex() == 3) type = "MIR";
        else type = "OPT";
        outputStream << "TYPE=" << type << "\n";
        if (ui->bayerCheckBox->isChecked()) {
            int checkedId = bayerButtonGroup->checkedId();
            if (checkedId == -1) checkedId = 1;
            QString bayerPattern = bayerButtonGroup->button(checkedId)->objectName().remove("bayer").remove("ToolButton");
            outputStream << "BAYER=" << bayerPattern << "\n";
        }
        configfile.close();

        // change the text label of the pushbutton for a short while
        ui->saveConfigPushButton->setText("Done");
        QTimer *timer = new QTimer();
        connect( timer, SIGNAL(timeout()), SLOT(timerConfigDone()) );
        timer->start(1000);
    }
    else {
        QMessageBox msgBox;
        msgBox.setText(filePath+" could not be opened for writing!");
        msgBox.exec();
        return;
    }

    QMessageBox::information( this, "Restart required",
                              "User-defined instrument configurations can be found at the bottom of the instrument selection menu after restarting THELI.");
    }

void Instrument::on_instrumentTypeComboBox_currentIndexChanged(int index)
{
    if (index == 0) {
        ui->bayerCheckBox->setEnabled(true);
        if (ui->bayerCheckBox->isChecked()) ui->buttonFrame->show();
        ui->overscanxMinLineEdit->setEnabled(true);
        ui->overscanxMaxLineEdit->setEnabled(true);
        ui->overscanyMinLineEdit->setEnabled(true);
        ui->overscanyMaxLineEdit->setEnabled(true);
        ui->overscanxMinNumLineEdit->setEnabled(true);
        ui->overscanxMaxNumLineEdit->setEnabled(true);
        ui->overscanyMinNumLineEdit->setEnabled(true);
        ui->overscanyMaxNumLineEdit->setEnabled(true);
    }
    else {
        ui->bayerCheckBox->setDisabled(true);
        ui->bayerCheckBox->setChecked(false);
        ui->buttonFrame->hide();
        ui->overscanxMinLineEdit->setEnabled(false);
        ui->overscanxMaxLineEdit->setEnabled(false);
        ui->overscanyMinLineEdit->setEnabled(false);
        ui->overscanyMaxLineEdit->setEnabled(false);
        ui->overscanxMinNumLineEdit->setEnabled(false);
        ui->overscanxMaxNumLineEdit->setEnabled(false);
        ui->overscanyMinNumLineEdit->setEnabled(false);
        ui->overscanyMaxNumLineEdit->setEnabled(false);
    }
}

void Instrument::on_loadConfigPushButton_clicked()
{
    QString instrumentDir;
    QMessageBox msgBox;
    msgBox.setText("Which instrument type do you want to load?");
    QAbstractButton* observatory = msgBox.addButton(tr("Observatory"), QMessageBox::YesRole);
    QAbstractButton* userdefined = msgBox.addButton(tr("User defined"), QMessageBox::YesRole);
    QAbstractButton* cancel = msgBox.addButton(tr("Cancel"), QMessageBox::NoRole);
    msgBox.exec();
    if (msgBox.clickedButton() == observatory) instrumentDir = thelidir+"/src/config/instruments/";
    else if (msgBox.clickedButton() == userdefined) instrumentDir = QDir::homePath()+"/.theli/instruments_user/";
    else if (msgBox.clickedButton() == cancel) return;

    QString instrumentFilename =
            QFileDialog::getOpenFileName(this, tr("Select instrument configuration file"), instrumentDir,
                                         tr("Instrument configuration file (*.ini)"));

    if ( instrumentFilename.isEmpty()) return;

    // Parse the configuration file
    QString line;
    QStringList list;
    QString keyword;
    QString keyvalue;
    QFile instrumentFile(instrumentFilename);
    if ( instrumentFile.open(QIODevice::ReadOnly)) {
        QTextStream stream( &instrumentFile );
        // For backwards compatibility (not all config files contain these keywords):
        ui->bayerCheckBox->setChecked(false);
        ui->gainLineEdit->setText("1.0");
        ui->longitudeLineEdit->setText("0");
        while ( !stream.atEnd() ) {
            line = stream.readLine().simplified();
            // skip comments
            if (line.contains("#") || line.isEmpty() || !line.contains("=")) continue;
            if (line.contains("=(")) {
                list = line.split("=(");
                keyword = list.at(0);
                keyvalue = list.at(1);
                keyvalue = configToGeometry(keyvalue);
            }
            else {
                list = line.split("=");
                keyword = list.at(0);
                keyvalue = list.at(1);
            }

            if (keyword == "INSTRUMENT") ui->instrumentNameLineEdit->setText(keyvalue);
            if (keyword == "NCHIPS") {
                ui->numchipSpinBox->setValue(keyvalue.toInt());
                numChips = keyvalue.toInt();
            }
            if (keyword == "OBSLAT") ui->latitudeLineEdit->setText(keyvalue);
            if (keyword == "OBSLONG") ui->longitudeLineEdit->setText(keyvalue);
            if (keyword == "PIXSCALE") ui->plateScaleLineEdit->setText(keyvalue);
            if (keyword == "GAIN") ui->gainLineEdit->setText(keyvalue);
            if (keyword == "OVSCANX1") ui->overscanxMinLineEdit->setText(keyvalue);
            if (keyword == "OVSCANX2") ui->overscanxMaxLineEdit->setText(keyvalue);
            if (keyword == "OVSCANY1") ui->overscanyMinLineEdit->setText(keyvalue);
            if (keyword == "OVSCANY2") ui->overscanyMaxLineEdit->setText(keyvalue);
            if (keyword == "CUTX") ui->startxLineEdit->setText(keyvalue);
            if (keyword == "CUTY") ui->startyLineEdit->setText(keyvalue);
            if (keyword == "SIZEX") ui->sizexLineEdit->setText(keyvalue);
            if (keyword == "SIZEY") ui->sizeyLineEdit->setText(keyvalue);
            if (keyword == "REFPIXX") ui->crpix1LineEdit->setText(keyvalue);
            if (keyword == "REFPIXY") ui->crpix2LineEdit->setText(keyvalue);
            if (keyword == "TYPE") {
                if (keyvalue == "OPT") ui->instrumentTypeComboBox->setCurrentIndex(0);
                else if (keyvalue == "NIR") ui->instrumentTypeComboBox->setCurrentIndex(1);
                else if (keyvalue == "NIRMIR") ui->instrumentTypeComboBox->setCurrentIndex(2);
                else if (keyvalue == "MIR") ui->instrumentTypeComboBox->setCurrentIndex(3);
                else ui->instrumentTypeComboBox->setCurrentIndex(0);

                if (keyvalue == "OPT") ui->bayerCheckBox->setEnabled(true);
                else ui->bayerCheckBox->setDisabled(true);
            }
            if (keyword == "BAYER") {
                if (keyvalue == "N") ui->bayerCheckBox->setChecked(false);
                else {
                    ui->bayerCheckBox->setChecked(true);
                    ui->buttonFrame->show();
                }
            }
        }
        instrumentFile.close();
    }
}

bool Instrument::compareChipNumbers()
{
    bool comparison = true;
    for (auto &it: geometryNumList) {
        if (numChips != it->text().toInt()) comparison = false;
    }
    return comparison;
}

void Instrument::altStream(QTextStream &stream, QString keyword, QString altValue)
{
    stream << "  if [ \"${"+keyword+"}\"_A = \"_A\" ]; then\n";
    stream << "      "+keyword+"="+altValue+"\n";
    stream << "  fi\n\n";
}

void Instrument::getKey(QTextStream &stream, QString bashKey, QString fitsKey, QString mode)
{
    if (mode.isEmpty()) {
        stream << "  "+bashKey+"=`get_key ${FILE} \""+fitsKey+"\"`\n";
    }
    else if (mode == "cleanstring") {
        stream << "  "+bashKey+"=`get_key ${FILE} \""+fitsKey+"\" | ${P_CLEANSTRING}`\n";
    }
    else if (mode == "cleancolon") {
        stream << "  "+bashKey+"=`get_key ${FILE} \""+fitsKey+"\" cleancolon`\n";
    }
    else if (mode == "equinox") {
        // Remove first char if not numeric
        stream << "  "+bashKey+"=`get_key ${FILE} \""+fitsKey+"\" | sed 's/[^0-9.]//g'`\n";
    }
}

void Instrument::truncateFITSkey(QString &key)
{
    int length = key.length();
    if (length == 8) return;

    if (length > 8) {
        key = key.left(8);
        return;
    }

    if (length < 8) {
        while (length < 8) {
            key = key.append(" ");
            length = key.length();
        }
    }
}


void Instrument::timerConfigDone()
{
    ui->saveConfigPushButton->setText("Save instrument config");
}

void Instrument::on_clearPushButton_clicked()
{
    QList<QLineEdit*> lineeditList;
    lineeditList.append(ui->crpix1LineEdit);
    lineeditList.append(ui->crpix1NumLineEdit);
    lineeditList.append(ui->crpix2LineEdit);
    lineeditList.append(ui->crpix2NumLineEdit);
    lineeditList.append(ui->gainLineEdit);
    lineeditList.append(ui->instrumentNameLineEdit);
    lineeditList.append(ui->latitudeLineEdit);
    lineeditList.append(ui->longitudeLineEdit);
    lineeditList.append(ui->optionalKeysLineEdit);
    lineeditList.append(ui->overscanxMaxLineEdit);
    lineeditList.append(ui->overscanxMaxNumLineEdit);
    lineeditList.append(ui->overscanxMinLineEdit);
    lineeditList.append(ui->overscanxMinNumLineEdit);
    lineeditList.append(ui->overscanyMaxLineEdit);
    lineeditList.append(ui->overscanyMaxNumLineEdit);
    lineeditList.append(ui->overscanyMinLineEdit);
    lineeditList.append(ui->overscanyMinNumLineEdit);
    lineeditList.append(ui->plateScaleLineEdit);
    lineeditList.append(ui->sizexLineEdit);
    lineeditList.append(ui->sizexNumLineEdit);
    lineeditList.append(ui->sizeyLineEdit);
    lineeditList.append(ui->sizeyNumLineEdit);
    lineeditList.append(ui->startxLineEdit);
    lineeditList.append(ui->startxNumLineEdit);
    lineeditList.append(ui->startyLineEdit);
    lineeditList.append(ui->startyNumLineEdit);

    // Clear line edits
    for (auto &it : lineeditList) {
        it->clear();
    }
}

void Instrument::on_bayerCheckBox_clicked(bool checked)
{
    if (checked) {
        ui->bayerRGGBToolButton->setDown(true);    // not effective?
        ui->buttonFrame->show();
    }
    else ui->buttonFrame->hide();
}

void Instrument::on_actionClose_triggered()
{
    this->close();
}
