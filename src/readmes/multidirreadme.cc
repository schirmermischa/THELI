#include "multidirreadme.h"
#include "ui_multidirreadme.h"

MultidirReadme::MultidirReadme(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultidirReadme)
{
    ui->setupUi(this);
}

MultidirReadme::~MultidirReadme()
{
    delete ui;
}
