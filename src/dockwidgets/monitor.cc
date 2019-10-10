#include "monitor.h"
#include "ui_monitor.h"
#include "functions.h"

#include <QString>
#include <QDebug>

Monitor::Monitor(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::Monitor)
{
    ui->setupUi(this);
}

Monitor::~Monitor()
{
    delete ui;
}

void Monitor::displayMessage(QString text, QString type)
{
    mutex.lock();
    message(ui->monitorPlainTextEdit, text, type);
    mutex.unlock();
}

void Monitor::appendOK()
{
    ui->monitorPlainTextEdit->moveCursor(QTextCursor::End);
    ui->monitorPlainTextEdit->insertPlainText(" OK");
    ui->monitorPlainTextEdit->moveCursor(QTextCursor::End);
}
