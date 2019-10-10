#include "imstatsreadme.h"
#include "ui_imstatsreadme.h"

ImstatsReadme::ImstatsReadme(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImstatsReadme)
{
    ui->setupUi(this);
}

ImstatsReadme::~ImstatsReadme()
{
    delete ui;
}
