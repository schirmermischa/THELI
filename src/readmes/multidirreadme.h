#ifndef MULTIDIRREADME_H
#define MULTIDIRREADME_H

#include <QDialog>

namespace Ui {
class MultidirReadme;
}

class MultidirReadme : public QDialog
{
    Q_OBJECT

public:
    explicit MultidirReadme(QWidget *parent = nullptr);
    ~MultidirReadme();

private:
    Ui::MultidirReadme *ui;
};

#endif // MULTIDIRREADME_H
