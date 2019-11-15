#ifndef ACKNOWLEDGING_H
#define ACKNOWLEDGING_H

#include <QDialog>

namespace Ui {
class Acknowledging;
}

class Acknowledging : public QDialog
{
    Q_OBJECT

public:
    explicit Acknowledging(QWidget *parent = 0);
    ~Acknowledging();

private:
    Ui::Acknowledging *ui;
};

#endif // ACKNOWLEDGING_H
