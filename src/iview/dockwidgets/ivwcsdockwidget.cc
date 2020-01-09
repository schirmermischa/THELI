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
    cd11 = translateCDmatrixSlider(position) * cd11_orig;
    emit CDmatrixChanged(cd11, cd12, cd21, cd22);

    ui->cd11LineEdit->setText(QString::number(cd11, 'e', 6));
}

void IvWCSDockWidget::on_cd12Slider_sliderMoved(int position)
{
    cd12 = translateCDmatrixSlider(position) * cd12_orig;
    emit CDmatrixChanged(cd11, cd12, cd21, cd22);

    ui->cd12LineEdit->setText(QString::number(cd12, 'e', 6));
}

void IvWCSDockWidget::on_cd21Slider_sliderMoved(int position)
{
    cd21 = translateCDmatrixSlider(position) * cd21_orig;
    emit CDmatrixChanged(cd11, cd12, cd21, cd22);

    ui->cd21LineEdit->setText(QString::number(cd21, 'e', 6));
}

void IvWCSDockWidget::on_cd22Slider_sliderMoved(int position)
{
    cd22 = translateCDmatrixSlider(position) * cd22_orig;
    emit CDmatrixChanged(cd11, cd12, cd21, cd22);

    ui->cd22LineEdit->setText(QString::number(cd22, 'e', 6));
}

// Translates the integer slider position to a relative change, non-linearly using a cosh dependence
double IvWCSDockWidget::translateCDmatrixSlider(int position)
{
    double pos = position;
    pos = 0.5*(pow(2., 3.*pos/100.) + pow(2., -3.*pos/100.));    // from -8 to 8
    if (position < 0) pos *= -1.;
    return pos;
}

// Translates the integer slider position to a relative change, non-linearly using a cosh dependence
double IvWCSDockWidget::translatePlateScaleSlider(int position)
{
    double pos = position;
    return 0.5*(pow(2., pos/100.) + pow(2., -pos/100.));          // from 0.5 to 2
}

// Translates the integer slider position to a relative change, non-linearly using a cosh dependence
double IvWCSDockWidget::translatePosAngleSlider(int position)
{
    double pos = position;
    return 0.5*(pow(2., 5.*pos/100.) - pow(2., -5.*pos/100.));    // from -32 to 32
}


void IvWCSDockWidget::on_plateScaleSlider_sliderMoved(int position)
{
    double rescaleFactor = translatePlateScaleSlider(position);

    cd11 *= rescaleFactor;
    cd12 *= rescaleFactor;
    cd21 *= rescaleFactor;
    cd22 *= rescaleFactor;
    emit CDmatrixChanged(cd11, cd12, cd21, cd22);
    updateLineEdits();
}

void IvWCSDockWidget::on_posAngleSlider_sliderMoved(int position)
{
    double oldPA = getPosAnglefromCD(cd11, cd12, cd21, cd22);
    double newPA = oldPA + translatePosAngleSlider(position);
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
    emit CDmatrixChanged(cd11, cd12, cd21, cd22);
    updateLineEdits();
    restoreSliders();
}

void IvWCSDockWidget::updateLineEdits()
{
    ui->cd11LineEdit->setText(QString::number(cd11, 'e', 6));
    ui->cd12LineEdit->setText(QString::number(cd12, 'e', 6));
    ui->cd21LineEdit->setText(QString::number(cd21, 'e', 6));
    ui->cd22LineEdit->setText(QString::number(cd22, 'e', 6));
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
