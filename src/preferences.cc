#include "preferences.h"
#include "ui_preferences.h"
#include "functions.h"
#include <QSettings>
#include <QMessageBox>
#include <QDebug>
#include <QCloseEvent>
#include <QFontDialog>
#include <QThread>

Preferences::Preferences(bool running, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);

    int maxCPU = QThread::idealThreadCount();
    if (maxCPU == -1) {
        qDebug() << "QDEBUG: Could not determine the number of CPUs for this machine!";
        qDebug() << "         Max number of CPUs set to 1.";
        maxCPU = 1;
    }
    ui->prefCPUSpinBox->setMaximum(maxCPU);
    ui->prefIOthreadsSpinBox->setMaximum(maxCPU);

    applyStyleSheets();

    configureMemory(); // Must do before reading settings

    readSettings();

    QFont currentFont = this->font();
    int fontsize = currentFont.pointSize();
    ui->prefFontsizeSpinBox->setValue(fontsize);

    // Prevent changes to important parameters while running
    if (running) {
        ui->parallelizationFrame->setDisabled(true);
        ui->memoryFrame->setDisabled(true);
        ui->prefIntermediateDataComboBox->setDisabled(true);
    }
    else {
        ui->parallelizationFrame->setEnabled(true);
        ui->memoryFrame->setEnabled(true);
        ui->prefIntermediateDataComboBox->setEnabled(true);
    }
}

void Preferences::configureMemory()
{
    // Memory setup. If we have less than 10 GB then we use MB, otherwise GB.
    // We always leave 1 GB for the operating system and other tasks
    /*
    totalMemory = get_memory() / 1024;   // [ MB ]
    if (totalMemory < 10000) {
        maxMemoryUsed = int(totalMemory) - 1024;
        ui->prefMemoryLabel->setText("Max usable memory [ MB ]");
        ui->prefMemorySpinBox->setMinimum(500);
        ui->prefMemorySpinBox->setMaximum(maxMemoryUsed);
    }
    else {
        maxMemoryUsed = int(totalMemory / 1024) - 1;
        ui->prefMemoryLabel->setText("Max usable memory [ GB ]");
        ui->prefMemorySpinBox->setMinimum(1);
        ui->prefMemorySpinBox->setMaximum(maxMemoryUsed);
    }
    */

    // MB only; Leave 512 MB min for OS
    totalMemory = get_memory() / 1024;   // [ MB ]
    maxMemoryUsed = int(totalMemory) - 512;
    ui->prefMemoryLabel->setText("Max usable memory [ MB ]");
    ui->prefMemorySpinBox->setMinimum(512);
    ui->prefMemorySpinBox->setMaximum(maxMemoryUsed);
}

void Preferences::applyStyleSheets()
{
    QList<QLabel*> labelList;
    labelList.append(ui->generalLabel);
    labelList.append(ui->parallelizationLabel);
    labelList.append(ui->memoryLabel);
    labelList.append(ui->fontLabel);
    labelList.append(ui->userInterfaceLabel);
    for (auto &it : labelList) {
        it->setStyleSheet("color: rgb(150, 240, 240); background-color: rgb(58, 78, 98);");
        it->setMargin(8);
    }

    QList<QFrame*> frameList;
    frameList.append(ui->generalFrame);
    frameList.append(ui->parallelizationFrame);
    frameList.append(ui->memoryFrame);
    frameList.append(ui->fontFrame);
    frameList.append(ui->userInterfaceFrame);
    /*
    for (auto &it : confFrameList) {
    // specify large style sheet?
    }
    */

    QList<QCheckBox*> checkboxList;
    checkboxList.append(ui->prefGPUCheckBox);
    checkboxList.append(ui->prefMemoryCheckBox);
    checkboxList.append(ui->prefSwitchProcessMonitorCheckBox);
    //    for (auto &it : checkboxList) {
    //        it->setStyleSheet("background-color: rgb(190,190,210);");
    //    }
}

void Preferences::updateParallelization(bool running)
{
    if (running) ui->parallelizationFrame->setDisabled(true);
    else ui->parallelizationFrame->setEnabled(true);
}

void Preferences::closeEvent(QCloseEvent *event)
{
    if (writeSettings() == QSettings::AccessError) {
        qDebug() << "QDEBUG: Preferences: Could not store preferences (access error).";
        event->accept();
    }
    else if (writeSettings() == QSettings::FormatError) {
        qDebug() << "QDEBUG: Preferences: A formatting error occurred while writing the preferences.";
        event->accept();
    }
    else {
        // no error
        event->accept();
    }
    emit preferencesUpdated();
}

Preferences::~Preferences()
{
    delete ui;
}

int Preferences::writeSettings()
{
    QSettings settings_lastproject("THELI", "PREFERENCES");
    settings_lastproject.setValue("prefServerComboBox", ui->prefServerComboBox->currentText());
    settings_lastproject.setValue("prefIntermediateDataComboBox", ui->prefIntermediateDataComboBox->currentText());
    settings_lastproject.setValue("prefVerbosityComboBox", ui->prefVerbosityComboBox->currentIndex());
    settings_lastproject.setValue("prefDiskspacewarnSpinBox", ui->prefDiskspacewarnSpinBox->value());
    settings_lastproject.setValue("prefCPUSpinBox", ui->prefCPUSpinBox->value());
    settings_lastproject.setValue("prefIOthreadsSpinBox", ui->prefIOthreadsSpinBox->value());
    settings_lastproject.setValue("prefGPUCheckBox", ui->prefGPUCheckBox->isChecked());
    settings_lastproject.setValue("prefProcessSkyCheckBox", ui->prefProcessSkyCheckBox->isChecked());
    settings_lastproject.setValue("prefMemorySpinBox", ui->prefMemorySpinBox->value());
    settings_lastproject.setValue("prefMemoryCheckBox", ui->prefMemoryCheckBox->isChecked());
    settings_lastproject.setValue("prefFontsizeSpinBox", ui->prefFontsizeSpinBox->value());
    settings_lastproject.setValue("prefSwitchProcessMonitorCheckBox", ui->prefSwitchProcessMonitorCheckBox->isChecked());
    settings_lastproject.setValue("prefFont", this->font());
    return settings_lastproject.status();
}

int Preferences::readSettings()
{
    QSettings settings("THELI", "PREFERENCES");
    ui->prefDiskspacewarnSpinBox->setValue(settings.value("prefDiskspacewarnSpinBox").toInt());
    ui->prefIntermediateDataComboBox->setCurrentText(settings.value("prefIntermediateDataComboBox").toString());
    ui->prefVerbosityComboBox->setCurrentIndex(settings.value("prefVerbosityComboBox").toInt());
    ui->prefServerComboBox->setCurrentText(settings.value("prefServerComboBox").toString());
    ui->prefCPUSpinBox->setValue(settings.value("prefCPUSpinBox").toInt());
    ui->prefIOthreadsSpinBox->setValue(settings.value("prefIOthreadsSpinBox").toInt());
    ui->prefGPUCheckBox->setChecked(settings.value("prefGPUCheckBox").toBool());
    ui->prefProcessSkyCheckBox->setChecked(settings.value("prefProcessSkyCheckBox").toBool());
    ui->prefFontsizeSpinBox->setValue(settings.value("prefFontsizeSpinBox").toInt());
    ui->prefMemorySpinBox->setValue(settings.value("prefMemorySpinBox", maxMemoryUsed).toInt());
    ui->prefMemoryCheckBox->setChecked(settings.value("prefMemoryCheckBox").toBool());
    ui->prefSwitchProcessMonitorCheckBox->setChecked(settings.value("prefSwitchProcessMonitorCheckBox").toBool());
    this->setFont(settings.value("prefFont").value<QFont>());
    return settings.status();
}

void Preferences::on_prefFontdialogPushButton_clicked()
{
    bool ok;
    QFont currentFont = this->font();
    QFont font = QFontDialog::getFont(&ok, QFont(currentFont), this);
    if (ok) {
        emit fontChanged(font);
        this->setFont(font);
    }
    int fontsize = font.pointSize();
    ui->prefFontsizeSpinBox->setValue(fontsize);
}

void Preferences::on_prefFontsizeSpinBox_valueChanged(int arg1)
{
    emit fontSizeChanged(arg1);
}

void Preferences::receiveDefaultFont(QFont font)
{
    defaultFont = font;
}

void Preferences::on_prefDefaultFontPushButton_clicked()
{
    emit fontChanged(defaultFont);
    this->setFont(defaultFont);
    int fontsize = defaultFont.pointSize();
    ui->prefFontsizeSpinBox->setValue(fontsize);
}

void Preferences::on_prefDiskspacewarnSpinBox_valueChanged(int arg1)
{
    emit diskspacewarnChanged(arg1);
}

void Preferences::on_prefServerComboBox_activated(const QString &arg1)
{
    emit serverChanged(arg1);
}

void Preferences::on_prefCPUSpinBox_valueChanged(int arg1)
{
    emit numcpuChanged(arg1);
}

void Preferences::on_prefIOthreadsSpinBox_valueChanged(int arg1)
{
    if (arg1 > 1) ui->prefIOthreadsSpinBox->setSuffix(" I/O threads");
    else ui->prefIOthreadsSpinBox->setSuffix(" I/O thread");
}

void Preferences::on_prefMemoryCheckBox_clicked()
{
    if (ui->prefMemoryCheckBox->isChecked()) {
        ui->prefIntermediateDataComboBox->setCurrentIndex(1);
        ui->prefIntermediateDataComboBox->setDisabled(true);
    }
    else {
        ui->prefIntermediateDataComboBox->setCurrentIndex(1);
        ui->prefIntermediateDataComboBox->setDisabled(false);
    }

    emit memoryUsageChanged(ui->prefMemoryCheckBox->isChecked());
}

void Preferences::on_prefIntermediateDataComboBox_currentTextChanged(const QString &arg1)
{
    emit intermediateDataChanged(arg1);
}

void Preferences::on_prefVerbosityComboBox_currentIndexChanged(int index)
{
    emit verbosityLevelChanged(index);
}


void Preferences::on_prefSwitchProcessMonitorCheckBox_clicked()
{
    emit switchProcessMonitorChanged(ui->prefSwitchProcessMonitorCheckBox->isChecked());
}
