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
