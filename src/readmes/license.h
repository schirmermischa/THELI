#ifndef LICENSE_H
#define LICENSE_H

#include <QDialog>

namespace Ui {
class License;
}

class License : public QDialog
{
    Q_OBJECT

public:
    explicit License(QWidget *parent = 0);
    ~License();

private:
    Ui::License *ui;
};

#endif // LICENSE_H
