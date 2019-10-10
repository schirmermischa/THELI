#include "ivconfdockwidget.h"
#include "ui_ivconfdockwidget.h"
#include "../iview.h"

IvConfDockWidget::IvConfDockWidget(IView *parent) :
    QDockWidget(parent),
    ui(new Ui::IvConfDockWidget)
{
    ui->setupUi(this);
    iview = parent;
}

IvConfDockWidget::~IvConfDockWidget()
{
    delete ui;
}

void IvConfDockWidget::switchMode(QString mode)
{
    ui->waveletPushButton->hide();
    if (mode == "FITSmonochrome" || mode == "MEMview") {
        ui->valueGreenLabel->hide();
        ui->valueBlueLabel->hide();
    }
    else if (mode == "FITScolor") {
        ui->valueGreenLabel->show();
        ui->valueBlueLabel->show();
    }
    else if (mode == "CLEAR") {
        ui->xposLabel->setText("x = ");
        ui->yposLabel->setText("y = ");
        ui->alphaDecLabel->setText("R.A. = ");
        ui->alphaHexLabel->setText("R.A. = ");
        ui->deltaDecLabel->setText("Dec  = ");
        ui->deltaHexLabel->setText("Dec  = ");
        ui->valueLabel->setText("Value = ");
        ui->zoomLabel->setText("Zoom level: ");
        ui->medianLabel->setText("Median = ");
        ui->rmsLabel->setText("stdev  = ");
        ui->valueGreenLabel->hide();
        ui->valueBlueLabel->hide();
    }
}

double IvConfDockWidget::zoom2scale(int zoomlevel)
{
    // Translates the integer zoom level to a scaling factor.
    // Update the zoom label
    if (zoomlevel > 0) {
        ui->zoomLabel->setText("Zoom level: "+QString::number(zoomlevel+1)+":1");
        return zoomlevel+1.;
    }
    else if (zoomlevel == 0) {
        ui->zoomLabel->setText("Zoom level: 1:1");
        return 1.;
    }
    else {
        ui->zoomLabel->setText("Zoom level: 1:"+QString::number(-zoomlevel+1));
        return 1./(-zoomlevel+1.);
    }
}

void IvConfDockWidget::on_zoomInPushButton_clicked()
{
    ui->zoomFitPushButton->setChecked(false);
    emit zoomInPushButton_clicked();
}

void IvConfDockWidget::on_zoomOutPushButton_clicked()
{
    ui->zoomFitPushButton->setChecked(false);
    emit zoomOutPushButton_clicked();
}

void IvConfDockWidget::on_zoomZeroPushButton_clicked()
{
    ui->zoomFitPushButton->setChecked(false);
    emit zoomZeroPushButton_clicked();
}

void IvConfDockWidget::on_zoomFitPushButton_clicked()
{
    emit zoomFitPushButton_clicked(ui->zoomFitPushButton->isChecked());
}

void IvConfDockWidget::on_minLineEdit_returnPressed()
{
    ui->autocontrastPushButton->setChecked(false);
    emit minmaxLineEdit_returnPressed(ui->minLineEdit->text(), ui->maxLineEdit->text());
}

void IvConfDockWidget::on_maxLineEdit_returnPressed()
{
    ui->autocontrastPushButton->setChecked(false);
    emit minmaxLineEdit_returnPressed(ui->minLineEdit->text(), ui->maxLineEdit->text());
}

void IvConfDockWidget::on_autocontrastPushButton_toggled(bool checked)
{
    emit autoContrastPushButton_toggled(checked);
}

void IvConfDockWidget::on_filterLineEdit_textChanged(const QString &arg1)
{
    QString filter = ui->filterLineEdit->text();

    if (filter.isEmpty()
            || !filter.contains(".fits")
            || !filter.contains("*")) {
        filter = "*.fits";
        ui->filterLineEdit->setText(filter);
    }

    iview->setImageList(filter);
    iview->numImages = iview->imageList.length();

    iview->pageLabel->setText(" Image ? / "+QString::number(iview->numImages));
}
