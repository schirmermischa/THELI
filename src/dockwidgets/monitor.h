#ifndef MONITOR_H
#define MONITOR_H

#include <QDockWidget>
#include <QMutex>

namespace Ui {
class Monitor;
}

class Monitor : public QDockWidget
{
    Q_OBJECT

public:
    explicit Monitor(QWidget *parent = 0);
    ~Monitor();

    Ui::Monitor *ui;
    QMutex mutex;

public slots:
    void displayMessage(QString message, QString type);
    void appendOK();

private:

};

#endif // MONITOR_H
