#include "license.h"
#include "ui_license.h"

License::License(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::License)
{
    ui->setupUi(this);
}

License::~License()
{
    delete ui;
}
