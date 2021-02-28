#include "ivfinderdockwidget.h"
#include "ui_ivfinderdockwidget.h"

#include "../iview.h"
#include "../../query/query.h"

#include <QMessageBox>

IvFinderDockWidget::IvFinderDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::IvFinderDockWidget)
{
    ui->setupUi(this);

    QIcon star(":/icons/star_icon.png");
    ui->targetresolverToolButton->setIcon(star);
    QIcon asteroid(":/icons/asteroid_icon.png");
    ui->MPCresolverToolButton->setIcon(asteroid);

    connect(this, &IvFinderDockWidget::targetResolved, this, &IvFinderDockWidget::targetResolvedReceived);
    connect(ui->targetAlphaLineEdit, &QLineEdit::textChanged, this, &IvFinderDockWidget::validate);
    connect(ui->targetDeltaLineEdit, &QLineEdit::textChanged, this, &IvFinderDockWidget::validate);
}

IvFinderDockWidget::~IvFinderDockWidget()
{
    delete ui;
}

void IvFinderDockWidget::on_targetresolverToolButton_clicked()
{
    ui->targetAlphaLineEdit->clear();
    ui->targetDeltaLineEdit->clear();

    QString targetName = ui->targetNameSiderealLineEdit->text();
    if (targetName.isEmpty()) return;
    targetName = targetName.simplified().replace(" ", "_");

    int verbosity = 0;
    Query *query = new Query(&verbosity);
    QString check = query->resolveTargetSidereal(targetName);
    if (check == "Resolved") emit targetResolved(query->targetAlpha, query->targetDelta);
    else if (check == "Unresolved") {
        QMessageBox::information(this, "THELI: Target unresolved",
                                 "The target " + targetName + " could not be resolved.");
    }
    else {
        // nothing yet.
    }

    delete query;
    query = nullptr;
}

// Query updates RA/DEC fields if successful
void IvFinderDockWidget::targetResolvedReceived(QString alpha, QString delta)
{
    ui->targetAlphaLineEdit->setText(alpha);
    ui->targetDeltaLineEdit->setText(delta);
}

// Coords were entered manually, then user clicks the "locate" button
void IvFinderDockWidget::on_locatePushButton_clicked()
{
    QString alpha = ui->targetAlphaLineEdit->text();
    QString delta = ui->targetDeltaLineEdit->text();
    if (alpha.isEmpty() || delta.isEmpty()) return;

    emit targetResolved(alpha, delta);
}

void IvFinderDockWidget::validate()
{
    QRegExp RA( "[0-9.:]+" );
    QRegExp DEC( "^[-]{0,1}[0-9.:]+" );

    QValidator* validator_ra = new QRegExpValidator( RA, this );
    QValidator* validator_dec = new QRegExpValidator( DEC, this );

    ui->targetAlphaLineEdit->setValidator( validator_ra );
    ui->targetDeltaLineEdit->setValidator( validator_dec );
}

void IvFinderDockWidget::on_clearPushButton_clicked()
{
    ui->targetNameSiderealLineEdit->clear();
    ui->targetNameNonsiderealLineEdit->clear();
    ui->targetAlphaLineEdit->clear();
    ui->targetDeltaLineEdit->clear();
    emit clearTargetResolved();
}

void IvFinderDockWidget::on_MPCresolverToolButton_clicked()
{
    ui->targetAlphaLineEdit->clear();
    ui->targetDeltaLineEdit->clear();

    QString targetName = ui->targetNameNonsiderealLineEdit->text();
    if (targetName.isEmpty()) return;
    targetName = targetName.simplified().replace(" ", "_");

    int verbosity = 0;
    Query *query = new Query(&verbosity);
    QString check = query->resolveTargetNonsidereal(targetName, dateObs, geoLon, geoLat);
    if (check == "Resolved") emit targetResolved(query->targetAlpha, query->targetDelta);
    else if (check == "Unresolved") {
        QMessageBox::information(this, "THELI: Target unresolved",
                                 "The target " + targetName + " could not be resolved.");
    }
    else {
        // nothing yet.
    }

    delete query;
    query = nullptr;
}

void IvFinderDockWidget::updateDateObsAndGeo(QString dateobs, float geolon, float geolat)
{
    dateObs = dateobs;
    geoLon = geolon;
    geoLat = geolat;
}

// just show a crosshair at the coordinates, don't resolve anything
void IvFinderDockWidget::bypassResolver()
{
    emit targetResolved(ui->targetAlphaLineEdit->text(), ui->targetDeltaLineEdit->text());
}
