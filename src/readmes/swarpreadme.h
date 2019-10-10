#ifndef SWARPREADME_H
#define SWARPREADME_H

#include <QDialog>

namespace Ui {
class SwarpReadme;
}

class SwarpReadme : public QDialog
{
    Q_OBJECT

public:
    explicit SwarpReadme(long openfiles, long maxopenfiles, QWidget *parent = 0);
    ~SwarpReadme();

private:
    Ui::SwarpReadme *ui;

    long openFiles = 0;
    long maxOpenFiles = 0;
};

#endif // SWARPREADME_H
