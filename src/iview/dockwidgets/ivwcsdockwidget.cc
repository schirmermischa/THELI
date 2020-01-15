#include "ivwcsdockwidget.h"
#include "ui_ivwcsdockwidget.h"
#include "../../functions.h"

IvWCSDockWidget::IvWCSDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::IvWCSDockWidget)
{
    ui->setupUi(this);
}

IvWCSDockWidget::~IvWCSDockWidget()
{
    delete ui;
}

void IvWCSDockWidget::init()
{
    cd11 = cd11_orig;
    cd12 = cd12_orig;
    cd21 = cd21_orig;
    cd22 = cd22_orig;

    updateLineEdits();
}

void IvWCSDockWidget::on_cd11Slider_sliderMoved(int position)
{
    QSlider *slider = qobject_cast<QSlider*>(sender());
    float weight = calcCDweight();
    if (weight > 1.) weight = 1.;
    cd11 = translateCDmatrixSlider(position, slider->maximum() / weight) * cd11_orig;
    emit CDmatrixChanged(cd11, cd12, cd21, cd22);

    ui->cd11LineEdit->setText(QString::number(cd11, 'e', 6));
}

void IvWCSDockWidget::on_cd12Slider_sliderMoved(int position)
{
    QSlider *slider = qobject_cast<QSlider*>(sender());
    float weight = calcCDweight();
    if (weight < 1.) weight = 1.;
    weight = 5.;
    cd12 = translateCDmatrixSlider(position, slider->maximum() / weight) * cd12_orig;
    emit CDmatrixChanged(cd11, cd12, cd21, cd22);

    ui->cd12LineEdit->setText(QString::number(cd12, 'e', 6));
}

void IvWCSDockWidget::on_cd21Slider_sliderMoved(int position)
{
    QSlider *slider = qobject_cast<QSlider*>(sender());
    float weight = calcCDweight();
    if (weight < 1.) weight = 1.;
    weight = 5.;
    cd21 = translateCDmatrixSlider(position, slider->maximum() / weight) * cd21_orig;
    emit CDmatrixChanged(cd11, cd12, cd21, cd22);

    ui->cd21LineEdit->setText(QString::number(cd21, 'e', 6));
}

void IvWCSDockWidget::on_cd22Slider_sliderMoved(int position)
{
    QSlider *slider = qobject_cast<QSlider*>(sender());
    float weight = calcCDweight();
    if (weight > 1.) weight = 1.;
    cd22 = translateCDmatrixSlider(position, slider->maximum() / weight) * cd22_orig;
    emit CDmatrixChanged(cd11, cd12, cd21, cd22);

    ui->cd22LineEdit->setText(QString::number(cd22, 'e', 6));
}

// Calculate a weight that makes the WCS slider more sensitive to smaller CD values
float IvWCSDockWidget::calcCDweight()
{
    float ratio = 1.0;

    if (cd12_orig != 0. && cd11_orig != 0) ratio = fabs(cd11_orig / cd12_orig);
    else if (cd11_orig == 0. && cd12_orig != 0.) ratio = 0.001;   // effective
    else if (cd12_orig == 0. && cd11_orig != 0.) ratio = 1000.;   // effective
    else ratio = 1.;

    float weight = sqrt(ratio);

//    return 1.;   // deactivated

    return weight;
}

// Translates the integer slider position to a relative change, non-linearly
double IvWCSDockWidget::translateCDmatrixSlider(int position, int maxRange)
{
    double pos = position;
    double range = maxRange;
    return pow(2., 0.5*pos/range);    // from 0.7 to 1.4
}

// Translates the integer slider position to a relative change, non-linearly using a cosh dependence
double IvWCSDockWidget::translatePlateScaleSlider(int position, int maxRange)
{
    double pos = position;
    double range = maxRange;
    return pow(2., 0.5*pos/range);    // from 0.7 to 1.4
}

// Translates the integer slider position to a relative change, non-linearly using a cosh dependence
double IvWCSDockWidget::translatePosAngleSlider(int position, int maxRange)
{
    double pos = position;
    double range = maxRange;
    return 10.*(pos/range);    // from -10 to 10
}


void IvWCSDockWidget::on_plateScaleSlider_sliderMoved(int position)
{
    QSlider *slider = qobject_cast<QSlider*>(sender());
    double rescaleFactor = translatePlateScaleSlider(position, slider->maximum());

    cd11 = cd11_orig * rescaleFactor;
    cd12 = cd12_orig * rescaleFactor;
    cd21 = cd21_orig * rescaleFactor;
    cd22 = cd22_orig * rescaleFactor;
    emit CDmatrixChanged(cd11, cd12, cd21, cd22);
    updateLineEdits();
}

void IvWCSDockWidget::on_posAngleSlider_sliderMoved(int position)
{
    QSlider *slider = qobject_cast<QSlider*>(sender());
    double oldPA = getPosAnglefromCD(cd11_orig, cd12_orig, cd21_orig, cd22_orig, false);
    double newPA = oldPA + translatePosAngleSlider(position, slider->maximum());
    cd11 = cd11_orig;
    cd12 = cd12_orig;
    cd21 = cd21_orig;
    cd22 = cd22_orig;
    rotateCDmatrix(cd11, cd12, cd21, cd22, newPA);
    emit CDmatrixChanged(cd11, cd12, cd21, cd22);
    updateLineEdits();
}

void IvWCSDockWidget::on_resetPushButton_clicked()
{
    cd11 = cd11_orig;
    cd12 = cd12_orig;
    cd21 = cd21_orig;
    cd22 = cd22_orig;
    updateLineEdits();
    restoreSliders();
    emit CDmatrixChanged(cd11, cd12, cd21, cd22);
}

void IvWCSDockWidget::updateLineEdits()
{
    ui->cd11LineEdit->setText(QString::number(cd11, 'e', 6));
    ui->cd12LineEdit->setText(QString::number(cd12, 'e', 6));
    ui->cd21LineEdit->setText(QString::number(cd21, 'e', 6));
    ui->cd22LineEdit->setText(QString::number(cd22, 'e', 6));

    double pscale1 = sqrt(cd11 * cd11 + cd21 * cd21);
    double pscale2 = sqrt(cd12 * cd12 + cd22 * cd22);
    QString pscale = QString::number(0.5*(pscale1+pscale2)*3600., 'f', 6);
    ui->plateScaleLineEdit->setText(pscale);

    QString PA = QString::number(getPosAnglefromCD(cd11, cd12, cd21, cd22), 'f', 2);
    ui->posAngleLineEdit->setText(PA);
}

void IvWCSDockWidget::restoreSliders()
{
    ui->cd11Slider->setValue(0);
    ui->cd12Slider->setValue(0);
    ui->cd21Slider->setValue(0);
    ui->cd22Slider->setValue(0);
    ui->plateScaleSlider->setValue(0);
    ui->posAngleSlider->setValue(0);
}
