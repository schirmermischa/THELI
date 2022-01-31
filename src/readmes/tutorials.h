#ifndef TUTORIALS_H
#define TUTORIALS_H

#include <QDialog>

namespace Ui {
class Tutorials;
}

class Tutorials : public QDialog
{
    Q_OBJECT

public:
    explicit Tutorials(QWidget *parent = 0);
    ~Tutorials();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Tutorials *ui;
};

#endif // TUTORIALS_H
