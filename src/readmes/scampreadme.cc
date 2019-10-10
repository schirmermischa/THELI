#include "scampreadme.h"
#include "ui_scampreadme.h"

ScampReadme::ScampReadme(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScampReadme)
{
    ui->setupUi(this);
}

ScampReadme::~ScampReadme()
{
    delete ui;
}
