#ifndef IVWCSDOCKWIDGET_H
#define IVWCSDOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
class IvWCSDockWidget;
}

class IvWCSDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit IvWCSDockWidget(QWidget *parent = 0);
    ~IvWCSDockWidget();

    double cd11_orig = 0.0;
    double cd12_orig = 0.0;
    double cd21_orig = 0.0;
    double cd22_orig = 0.0;

    void init();

private slots:
    void on_cd11Slider_sliderMoved(int position);
    void on_cd12Slider_sliderMoved(int position);
    void on_cd21Slider_sliderMoved(int position);
    void on_cd22Slider_sliderMoved(int position);
    void on_plateScaleSlider_sliderMoved(int position);
    void on_posAngleSlider_sliderMoved(int position);
    void on_resetPushButton_clicked();

private:
    Ui::IvWCSDockWidget *ui;

    double cd11 = 0.0;
    double cd12 = 0.0;
    double cd21 = 0.0;
    double cd22 = 0.0;

    double translateCDmatrixSlider(int position);
    double translatePlateScaleSlider(int position);
    double translatePosAngleSlider(int position);
    void updateLineEdits();
    void restoreSliders();

signals:
    void CDmatrixChanged(double cd11, double cd12, double cd21, double cd22);
};

#endif // IVWCSDOCKWIDGET_H
