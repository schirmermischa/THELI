#ifndef IVCONFDOCKWIDGET_H
#define IVCONFDOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
class IvConfDockWidget;
}

class IView;  // Forward declaration to access members

class IvConfDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit IvConfDockWidget(IView *parent = 0);
    ~IvConfDockWidget();
    Ui::IvConfDockWidget *ui;

    IView *iview;

    void switchMode(QString mode);
    double zoom2scale(int zoomlevel);

private slots:
    void on_zoomOutPushButton_clicked();
    void on_zoomInPushButton_clicked();
    void on_minLineEdit_returnPressed();
    void on_maxLineEdit_returnPressed();
    void on_autocontrastPushButton_toggled(bool checked);

public slots:
    void on_zoomFitPushButton_clicked();
    void on_zoomZeroPushButton_clicked();
    void on_filterLineEdit_textChanged(const QString &arg1);

signals:
    void autoContrastPushButton_toggled(bool checked);
    void minmaxLineEdit_returnPressed(QString rangeMin, QString rangeMax);
    void zoomFitPushButton_clicked(bool checked);
    void zoomInPushButton_clicked();
    void zoomOutPushButton_clicked();
    void zoomZeroPushButton_clicked();
};

#endif // IVCONFDOCKWIDGET_H
