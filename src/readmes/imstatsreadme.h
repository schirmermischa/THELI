#ifndef IMSTATSREADME_H
#define IMSTATSREADME_H

#include <QDialog>

namespace Ui {
class ImstatsReadme;
}

class ImstatsReadme : public QDialog
{
    Q_OBJECT

public:
    explicit ImstatsReadme(QWidget *parent = nullptr);
    ~ImstatsReadme();

private slots:

private:
    Ui::ImstatsReadme *ui;
};

#endif // IMSTATSREADME_H
