#include "acknowledging.h"
#include "ui_acknowledging.h"

Acknowledging::Acknowledging(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Acknowledging)
{
    ui->setupUi(this);
}

Acknowledging::~Acknowledging()
{
    delete ui;
}
