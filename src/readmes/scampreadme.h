#ifndef SCAMPREADME_H
#define SCAMPREADME_H

#include <QDialog>

namespace Ui {
class ScampReadme;
}

class ScampReadme : public QDialog
{
    Q_OBJECT

public:
    explicit ScampReadme(QWidget *parent = nullptr);
    ~ScampReadme();

private slots:

private:
    Ui::ScampReadme *ui;
};

#endif // SCAMPREADME_H
