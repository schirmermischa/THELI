#include "tutorials.h"
#include "ui_tutorials.h"

Tutorials::Tutorials(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tutorials)
{
    ui->setupUi(this);
}

Tutorials::~Tutorials()
{
    delete ui;
}

void Tutorials::on_pushButton_clicked()
{
    close();
}
