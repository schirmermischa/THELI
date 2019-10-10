#include "swarpreadme.h"
#include "ui_swarpreadme.h"

SwarpReadme::SwarpReadme(long openfiles, long maxopenfiles, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SwarpReadme)
{
    ui->setupUi(this);

    openFiles = openfiles;
    maxOpenFiles = maxopenfiles;

    ui->maxOpenFileLineEdit->setText(QString::number(maxOpenFiles));
    ui->openFileLineEdit->setText(QString::number(openFiles));
}

SwarpReadme::~SwarpReadme()
{
    delete ui;
}
