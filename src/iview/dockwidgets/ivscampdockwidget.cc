#include "ivscampdockwidget.h"
#include "ui_ivscampdockwidget.h"
#include "../iview.h"

#include <QObject>

IvScampDockWidget::IvScampDockWidget(IView *parent) :
    QDockWidget(parent),
    ui(new Ui::IvScampDockWidget)
{
    ui->setupUi(this);

    iview = parent;
}

IvScampDockWidget::~IvScampDockWidget()
{
    delete ui;
}

void IvScampDockWidget::on_acceptSolution_clicked()
{
    emit solutionAcceptanceState(true);
}

void IvScampDockWidget::on_rejectSolution_clicked()
{
    emit solutionAcceptanceState(false);
}
