#ifndef IVSCAMPDOCKWIDGET_H
#define IVSCAMPDOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
class IvScampDockWidget;
}

class IView;  // Forward declaration to access members

class IvScampDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit IvScampDockWidget(IView *parent = 0);
    ~IvScampDockWidget();

    IView *iview;

private:
    Ui::IvScampDockWidget *ui;

private slots:
    void on_acceptSolution_clicked();
    void on_rejectSolution_clicked();

signals:
    void solutionAcceptanceState(bool state);

};

#endif // IVSCAMPDOCKWIDGET_H
