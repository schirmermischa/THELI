/*
Copyright (C) 2019 Mischa Schirmer

This file is part of THELI.

THELI is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program in the LICENSE file.
If not, see https://www.gnu.org/licenses/ .
*/

#include "abszeropoint.h"
#include "ui_abszeropoint.h"
#include "../functions.h"
#include "../instrumentdata.h"
#include "../qcustomplot.h"
#include "../query/query.h"
#include "../myimage/myimage.h"
#include "../threading/memoryworker.h"
#include "../tools/tools.h"
#include "../tools/cfitsioerrorcodes.h"

#include "fitsio2.h"

#include <QDir>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <QTest>

AbsZeroPoint::AbsZeroPoint(QString image, QWidget *parent) :
    QMainWindow(parent),
    startImage(image),
    ui(new Ui::AbsZeroPoint)
{
    ui->setupUi(this);

    initEnvironment(thelidir, userdir);
    initGUI();
    if (!startImage.isEmpty()) {
        ui->zpImageLineEdit->setText(startImage);
    }

    performingStartup = false;

    connect(this, &AbsZeroPoint::messageAvailable, this, &AbsZeroPoint::displayMessage);
    connect(this, &AbsZeroPoint::readyForPlotting, this, &AbsZeroPoint::buildAbsPhot);
    //    connect(this, &AbsZeroPoint::progressUpdate, this, &AbsZeroPoint::progressUpdateReceived);
    //    connect(this, &AbsZeroPoint::resetProgressBar, this, &AbsZeroPoint::resetProgressBarReceived);
}

void AbsZeroPoint::closeEvent(QCloseEvent *event)
{
    emit abszpClosed();
    event->accept();
}

AbsZeroPoint::~AbsZeroPoint()
{
    delete absPhot;
    absPhot = nullptr;

    delete ui;
}

void AbsZeroPoint::displayMessage(QString message, QString type)
{
    if (type == "error") ui->zpPlainTextEdit->appendHtml("<font color=\"#ee0000\">"+message+"</font>");
    else if (type == "info") ui->zpPlainTextEdit->appendHtml("<font color=\"#0033cc\">"+message+"</font>");
    else ui->zpPlainTextEdit->appendHtml(message);
}

void AbsZeroPoint::initGUI()
{
    clearText();

    // Fill in defaults
    on_zpRefcatComboBox_currentTextChanged("ATLAS-REFCAT2");
    on_zpFilterComboBox_currentTextChanged("g");
    defaults_if_empty();

    // Crashes the GUI upon launch
    //    ui->topDockWidget->titleBarWidget()->setFixedHeight(1);

    // Connections
    // For the GUI
    connect(ui->zpSaturationLineEdit, &QLineEdit::textChanged, this, &AbsZeroPoint::validate);
    connect(ui->zpPhoterrorLineEdit, &QLineEdit::textChanged, this, &AbsZeroPoint::validate);
    connect(ui->zpApertureLineEdit, &QLineEdit::textChanged, this, &AbsZeroPoint::validate);
    // For a mouse-click in the plot
    connect(ui->zpPlot, &QCustomPlot::plottableClick, this, &AbsZeroPoint::showData);

    // Deactivate UKIDSS for the time being
    // very inhomogeneous sky coverage in various epochs
    const QStandardItemModel* model = dynamic_cast< QStandardItemModel * >( ui->zpRefcatComboBox->model() );
    model->item(7,0)->setEnabled(false);

    // Provide a roughly log range of apertures
    ui->zpApertureLineEdit->setText("10,15,20,25,30,40,50,75,100");

    // Initialize the plot
    ui->zpPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->zpPlot->plotLayout()->clear(); // clear default axis rect so we can start from scratch

    setWindowIcon(QIcon(":/icons/abszp.png"));

    loadPreferences();
}

// UNUSED slot
void AbsZeroPoint::updateVerbosity(int verbosityLevel)
{
    verbosity = verbosityLevel;
}

void AbsZeroPoint::clearText()
{
    QFont notoFont("Noto Sans", 10, QFont::Normal);
    ui->zpHeaderLabel->setText("System output");
    ui->zpPlainTextEdit->clear();
    ui->zpPlainTextEdit->setFont(notoFont);
}

void AbsZeroPoint::validate()
{
    // Validators
    QRegExp rfloat( "[0-9]*[.]{0,1}[0-9]+" );
    QValidator* validator_float = new QRegExpValidator(rfloat, this);
    QRegExp rintcomma( "[0-9, ]+" );
    QValidator* validator_intcomma = new QRegExpValidator(rintcomma, this);
    QRegExp rint( "^[-]{0,1}[0-9]+" );
    QValidator* validator_int = new QRegExpValidator(rint, this);

    ui->zpSaturationLineEdit->setValidator(validator_float);
    ui->zpPhoterrorLineEdit->setValidator(validator_float);
    ui->zpApertureLineEdit->setValidator(validator_intcomma);
    ui->zpDTLineEdit->setValidator(validator_float);
    ui->zpDMINLineEdit->setValidator(validator_int);
}

void AbsZeroPoint::defaults_if_empty()
{
    if (ui->zpPhoterrorLineEdit->text().isEmpty()) ui->zpPhoterrorLineEdit->setText("0.05");
    if (ui->zpSaturationLineEdit->text().isEmpty()) ui->zpSaturationLineEdit->setText("100");
    if (ui->zpApertureLineEdit->text().isEmpty()) ui->zpApertureLineEdit->setText("20");
}

void AbsZeroPoint::loadPreferences()
{
    QSettings settings("THELI", "PREFERENCES");
    maxCPU = settings.value("prefCPUSpinBox").toInt();
    verbosity = settings.value("prefVerbosityComboBox").toInt();
}

void AbsZeroPoint::on_startPushButton_clicked()
{
    if (ui->zpImageLineEdit->text().isEmpty()) {
        QMessageBox::information(this, tr("Missing image."),
                                 tr("You have not specified an image to be calibrated."),
                                 QMessageBox::Ok);
        return;
    }

    on_zpClearPushButton_clicked();

    absPhot->clear();
    plot();
    QTest::qWait(50);

    // Launch the thread
    ui->startPushButton->setText("Running ...");
    ui->startPushButton->setDisabled(true);
    ui->zpColorComboBox->setDisabled(true);
    ui->zpFilterComboBox->setDisabled(true);
    ui->zpRefcatComboBox->setDisabled(true);

    workerThread = new QThread();
    abszpWorker = new AbszpWorker(this);
    //workerInit = true;
    //workerThreadInit = true;
    abszpWorker->moveToThread(workerThread);
    connect(workerThread, &QThread::started, abszpWorker, &AbszpWorker::runTask);
    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater, Qt::DirectConnection);
    connect(abszpWorker, &AbszpWorker::finished, this, &AbsZeroPoint::finishedCalculations);
    connect(abszpWorker, &AbszpWorker::finished, workerThread, &QThread::quit, Qt::DirectConnection);
    connect(abszpWorker, &AbszpWorker::finished, abszpWorker, &QObject::deleteLater, Qt::DirectConnection);
    connect(abszpWorker, &AbszpWorker::messageAvailable, this, &AbsZeroPoint::displayMessage);

    workerThread->start();
}

void AbsZeroPoint::criticalReceived()
{
    emit finished();
}

void AbsZeroPoint::updateSaturationValue(float value)
{
    // use 90% of the highest measured value as a limit
    ui->zpSaturationLineEdit->setText(QString::number(0.9*value, 'f', 2));
}

// Running in separate thread!
// Here is where the actual work happens
void AbsZeroPoint::taskInternalAbszeropoint()
{
    myImage = new MyImage(ui->zpImageLineEdit->text(), QVector<bool>(), &verbosity);

    connect(myImage, &MyImage::messageAvailable, this, &AbsZeroPoint::displayMessage);
    connect(myImage, &MyImage::critical, this, &AbsZeroPoint::criticalReceived);
    myImage->globalMaskAvailable = false;
    myImage->maxCPU = maxCPU;
    myImage->loadHeader();

    bool successQuery = true;
    bool successDetection = true;
    // executing catalog query and source detection in parallel
#pragma omp parallel sections
    {
#pragma omp section
        {
            emit messageAvailable("Querying " + ui->zpRefcatComboBox->currentText() + " ...", "ignore");
            queryRefCat();
            if (numRefSources == 0) {
                emit messageAvailable("AbsZP: Aborting, could not find any reference sources.", "error");
                emit finished();
                successQuery = false;
            }
        }
#pragma omp section
        {
            emit messageAvailable("Detecting objects in image ...", "ignore");
            QString DT = ui->zpDTLineEdit->text();
            QString DMIN = ui->zpDMINLineEdit->text();
            if (DT.isEmpty()) {
                DT = "10";
                ui->zpDTLineEdit->setText("10");
            }
            if (DMIN.isEmpty()) {
                DMIN = "10";
                ui->zpDMINLineEdit->setText("10");
            }

            // Collect apertures and pass them on
            QString ap = ui->zpApertureLineEdit->text();
            ap = ap.replace(","," ").simplified();
            QStringList apList = ap.split(" ");
            QVector<float> apertures;
            for (auto &val : apList) {
                apertures << val.toFloat();
            }
            myImage->apertures = apertures;

            // Detect objects
            myImage->readImage(ui->zpImageLineEdit->text());
            if (!myImage->successProcessing) successDetection = false;
            myImage->readWeight();
            myImage->successProcessing = true;
            myImage->apertures = apertures;
            myImage->backgroundModel(100, "interpolate");
            myImage->segmentImage(DT, DMIN, true, false);
            long ngood = 0;
            for (auto &it : myImage->objectList) if (it->FLAGS == 0) ++ngood;
            emit messageAvailable(QString::number(myImage->objectList.length()) + " sources detected, "+QString::number(ngood) + " selected", "info");
        }
    }

    if (!successDetection || !successQuery) return;

    emit messageAvailable("Matching objects with reference sources ...", "ignore");
    refDat.clear();
    objDat.clear();
    // DEC comes first in the catalogs, because the matching alg sorts the vectors for DEC

    for (auto &object : myImage->objectList) {
        QVector<double> objdata;
        if (object->FLAGS == 0) {
            objdata << object->DELTA_J2000 << object->ALPHA_J2000 << object->MAG_AUTO
                    << object->MAGERR_AUTO << object->MAG_APER << object->MAGERR_APER;
            objDat.append(objdata);
            //            qDebug() << qSetRealNumberPrecision(12) << object->ALPHA_J2000 << object->DELTA_J2000;
        }
    }

    // Estimate the matching tolerance
    myImage->estimateMatchingTolerance();
    //    qDebug() << "matching tolerance: " << myImage->matchingTolerance;
    // qDebug() << " " ;      // spacer between object and reference catalog output

    // Iterate the matching once to get a sensible upper limit for the refcat download
    // (and thus a cleaner process in extreme cases where the refcat is MUCH deeper than the image)
    refCatUpperMagLimit = 40.;
    int multiple1 = 0;
    int multiple2 = 0;
    int loop = 0;
    while (loop <= 1) {

        // Collect suitable reference sources
        refDat.clear();
        for (int i=0; i<raRefCat.length(); ++i) {
            QVector<double> refdata;
            // only propagate reference sources inside the image area and brighter than the upper mag limit
            if (myImage->containsRaDec(raRefCat[i], deRefCat[i])
                    && mag1RefCat[i] < refCatUpperMagLimit) {
                refdata << deRefCat[i] << raRefCat[i] << mag1RefCat[i] << mag2RefCat[i] << mag1errRefCat[i] << mag2errRefCat[i];
                refDat.append(refdata);
            }
            // qDebug() << qSetRealNumberPrecision(12) << raRefCat[i] << deRefCat[i];
        }

        // Now do the matching
        multiple1 = 0;
        multiple2 = 0;
        matched.clear();     // Otherwise, multiple runs will append to previous data
        if (ui->zpRefcatComboBox->currentText().contains("2MASS")) {
            // simple way to accomodate accumulated proper motions for 2MASS (adding 1" matching radius)
            match2D_refcoords(objDat, refDat, matched, myImage->matchingTolerance + 1./3600., multiple1, multiple2, maxCPU);
        }
        else {
            match2D_refcoords(objDat, refDat, matched, myImage->matchingTolerance, multiple1, multiple2, maxCPU);
        }

        // Update upper refcat mag limit and repeat once
        refCatUpperMagLimit = getFirstZPestimate();
//        qDebug() << "upper limit:" << refCatUpperMagLimit << refDat.length();
        ++loop;
    }

    // debugging: output catalogs
    QFile outcat_detected(myImage->path+"/detected.cat");
    QFile outcat_downloaded(myImage->path+"/downloaded.cat");
    QTextStream stream_detected(&outcat_detected);
    QTextStream stream_downloaded(&outcat_downloaded);
    outcat_detected.open(QIODevice::WriteOnly);
    outcat_downloaded.open(QIODevice::WriteOnly);
    long i = 0;
    for (auto &it : raRefCat) {
        stream_downloaded << QString::number(it, 'f', 9) << " " << QString::number(deRefCat[i], 'f', 9) << " " << QString::number(mag1RefCat[i], 'f', 3) << "\n";
        ++i;
    }
    outcat_downloaded.close();
    outcat_downloaded.setPermissions(QFile::ReadUser | QFile::WriteUser);

    for (auto &it : objDat) {
        stream_detected << QString::number(it[1], 'f', 9) << " " << QString::number(it[0], 'f', 9) << " " << QString::number(it[2], 'f', 3) << "\n";
    }
    outcat_detected.close();
    outcat_detected.setPermissions(QFile::ReadUser | QFile::WriteUser);

    if (matched.length() == 0) {
        emit messageAvailable("No matches found!", "error");
        emit finished();
        return;
    }
    else {
        emit messageAvailable(QString::number(matched.length()) + " clean matches found.", "ignore");
        //  for (auto &it : matched) qDebug() << qSetRealNumberPrecision(10) << it[0] << it[1];
    }
    if (multiple1 > 1) emit messageAvailable("Multiply matched objects (ignored): " + QString::number(multiple1), "ignore");
    if (multiple2 > 1) emit messageAvailable("Multiply matched reference sources (ignored): " + QString::number(multiple2), "ignore");

    // The plotting makes changes to the GUI, and therefore has been done from within the main thread!
    emit readyForPlotting();

    // update the catalog display if iview is open
    writeAbsPhotRefcat();
    if (iViewOpen) on_showAbsphotPushButton_clicked();

    getFirstZPestimate();
}

double AbsZeroPoint::getFirstZPestimate()
{
    // if the reference catalog is much deeper than the image, we should apply a magnitude cut before matching.
    // Hence, do a rough ZP estimate (without color term) and then rematch
    QVector<double> magdiff;
    QVector<double> magDetected;
    magdiff.resize(matched.length());
    magDetected.resize(matched.length());
    long i = 0;
    for (auto &it : matched) {
        magdiff[i] = it[2] - it[6];
        magDetected[i] = it[6];
        ++i;
    }
    double medianZP = straightMedian_T(magdiff);
    // double rms = madMask_T(magdiff);

    // return the faintest detected magnitude estimate, and add 0.3 mag as a margin
    return maxVec_T(magDetected) + medianZP + 0.3;
}

void AbsZeroPoint::queryRefCat()
{
    Query *query = new Query(&verbosity);
    query->photomDir = myImage->path;
    query->photomImageName = myImage->name;
    query->refcatName = ui->zpRefcatComboBox->currentText();
    QString filter1 = ui->zpFilterComboBox->currentText();
    QString filter2 = ui->zpColorComboBox->currentText().remove('-').remove(filter1);
    QString filter3 = "";
    // For Gaia we enforce to always use BP-RP as colour
    if (query->refcatName.contains("GAIA")) {
        filter1 = "G";
        filter2 = "BP";
        filter3 = "RP";
        query->refcatFilter1 = filter1;
        query->refcatFilter2 = filter2;
        query->refcatFilter3 = filter3;
    }
    else {
        query->refcatFilter1 = filter1;
        query->refcatFilter2 = filter2;
    }
    query->doPhotomQueryFromWeb();

    raRefCat.swap(query->ra_out);
    deRefCat.swap(query->de_out);
    mag1RefCat.swap(query->mag1_out);
    mag2RefCat.swap(query->mag2_out);
    mag1errRefCat.swap(query->mag1err_out);
    mag2errRefCat.swap(query->mag2err_out);
    if (query->refcatName.contains("GAIA")) {
        mag3RefCat.swap(query->mag3_out);
        mag3errRefCat.swap(query->mag3err_out);
    }

    numRefSources = query->numSources;

    // Conversion to AB mag if not yet done

    // Convert APASS VEGA mags to AB mags
    // http://www.astronomy.ohio-state.edu/~martini/usefuldata.html
    if (query->refcatName.contains("APASS")) {
        float corr1 = 0.;
        float corr2 = 0.;
        if (filter1 == "B") corr1 = -0.09;
        else if (filter1 == "V") corr1 = 0.02;
        if (filter2 == "B") corr2 = -0.09;
        else if (filter2 == "V") corr2 = 0.02;

        for (auto &mag : mag1RefCat) mag += corr1;
        for (auto &mag : mag2RefCat) mag += corr2;
    }
    // Convert Gaia mags to AB mags
    // https://gea.esac.esa.int/archive/documentation/GEDR3/Data_processing/chap_cu5pho/cu5pho_sec_photProc/cu5pho_ssec_photCal.html
    if (query->refcatName.contains("GAIA")) {
        float corr1 = 0.;
        float corr2 = 0.;
        float corr3 = 0.;
        if (filter1 == "G") corr1 = 0.1136;
        else if (filter1 == "BP") corr1 = 0.0155;
        else if (filter1 == "RP") corr1 = 0.3561;
        if (filter2 == "G") corr2 = 0.1136;
        else if (filter2 == "BP") corr2 = 0.0155;
        else if (filter2 == "RP") corr2 = 0.3561;
        if (filter3 == "G") corr3 = 0.1136;
        else if (filter3 == "BP") corr3 = 0.0155;
        else if (filter3 == "RP") corr3 = 0.3561;

        for (auto &mag : mag1RefCat) mag += corr1;
        for (auto &mag : mag2RefCat) mag += corr2;
        for (auto &mag : mag3RefCat) mag += corr3;
    }
    // Convert 2MASS VEGA mags to AB mags
    // http://www.astronomy.ohio-state.edu/~martini/usefuldata.html
    if (query->refcatName.contains("2MASS")) {
        float corr1 = 0.;
        float corr2 = 0.;
        if (filter1 == "J") corr1 = 0.91;
        else if (filter1 == "H") corr1 = 1.39;
        else if (filter1 == "Ks") corr1 = 1.85;
        if (filter2 == "J") corr2 = 0.91;
        else if (filter2 == "H") corr2 = 1.39;
        else if (filter2 == "Ks") corr2 = 1.85;

        for (auto &mag : mag1RefCat) mag += corr1;
        for (auto &mag : mag2RefCat) mag += corr2;
    }
    // Convert VHS VEGA mags to AB mags
    // Pons et al. 2019, MNRAS, 484, 5142: https://academic.oup.com/mnras/article/484/4/5142/5303744
    // http://www.astronomy.ohio-state.edu/~martini/usefuldata.html (for Y-band)
    if (query->refcatName.contains("VHS")) {
        float corr1 = 0.;
        float corr2 = 0.;
        if (filter1 == "Y") corr1 = 0.634;
        else if (filter1 == "J") corr1 = 0.937;
        else if (filter1 == "H") corr1 = 1.384;
        else if (filter1 == "Ks") corr1 = 1.839;
        if (filter2 == "Y") corr2 = 0.634;
        else if (filter2 == "J") corr2 = 0.937;
        else if (filter2 == "H") corr2 = 1.384;
        else if (filter2 == "Ks") corr2 = 1.839;

        for (auto &mag : mag1RefCat) mag += corr1;
        for (auto &mag : mag2RefCat) mag += corr2;
    }
    // Convert UKIDSS VEGA mags to AB mags
    // Hewett et al. 2006, MNRAS, 367, 454
    if (query->refcatName.contains("UKIDSS")) {
        float corr1 = 0.;
        float corr2 = 0.;
        if (filter1 == "J") corr1 = 0.938;
        else if (filter1 == "H") corr1 = 1.379;
        else if (filter1 == "Ks") corr1 = 1.900;
        if (filter2 == "J") corr2 = 0.938;
        else if (filter2 == "H") corr2 = 1.379;
        else if (filter2 == "Ks") corr2 = 1.900;

        for (auto &mag : mag1RefCat) mag += corr1;
        for (auto &mag : mag2RefCat) mag += corr2;
    }

    delete query;
    query = nullptr;

    // Count how many inside image
    long ngood = 0;
    for (int i=0; i<raRefCat.length(); ++i) {
        if (myImage->containsRaDec(raRefCat[i], deRefCat[i])) ++ngood;
    }

    emit messageAvailable(QString::number(numRefSources) + " reference sources found, "+QString::number(ngood)+" inside image", "info");
    emit messageAvailable("Please wait for object detection to finish ...", "ignore");
}

void AbsZeroPoint::writeAbsPhotRefcat()
{
    // The iView catalog (ASCII)
    QString outpath = myImage->path;
    QDir outdir(outpath);
    if (!outdir.exists()) {
        emit messageAvailable(QString(__func__) + " : " + myImage->path +" directory does not exist to write AbsPhot reference catalogs", "error");
        emit criticalReceived();
        return;
    }

    QFile outcat_iview_matched(outpath+"/ABSPHOT_sources_matched.iview");
    QTextStream stream_iview_matched(&outcat_iview_matched);
    if( !outcat_iview_matched.open(QIODevice::WriteOnly)) {
        emit messageAvailable(QString(__func__) + " : ERROR writing "+outpath+outcat_iview_matched.fileName()+" : "+outcat_iview_matched.errorString(), "error");
        emit criticalReceived();
        return;
    }

    QFile outcat_iview_down(outpath+"/ABSPHOT_sources_downloaded.iview");
    QTextStream stream_iview_down(&outcat_iview_down);
    if( !outcat_iview_down.open(QIODevice::WriteOnly)) {
        emit messageAvailable(QString(__func__) + " : ERROR writing "+outpath+outcat_iview_down.fileName()+" : "+outcat_iview_down.errorString(), "error");
        emit criticalReceived();
        return;
    }

    // Write downloaded iView catalog
    for (auto &it : refDat) {
        stream_iview_down << QString::number(it[1], 'f', 9) << " " << QString::number(it[0], 'f', 9) << " " << QString::number(it[2], 'f', 3) << "\n";
    }
    outcat_iview_down.close();
    outcat_iview_down.setPermissions(QFile::ReadUser | QFile::WriteUser);

    // Write matched iView catalog
    for (auto &it : matched) {
        stream_iview_matched << QString::number(it[1], 'f', 9) << " " << QString::number(it[0], 'f', 9) << " " << QString::number(it[2], 'f', 3) << "\n";
    }
    outcat_iview_matched.close();
    outcat_iview_matched.setPermissions(QFile::ReadUser | QFile::WriteUser);
}

// Not yet developed for Gaia
void AbsZeroPoint::buildAbsPhot()
{
    QString apertures = ui->zpApertureLineEdit->text();
    apertures = apertures.replace(",", " ");
    apertures = apertures.simplified();
    QStringList aperList = apertures.split(" ");
    int numAper = aperList.length();
    absPhot->numAper += numAper;
    for (int i=0; i<numAper; ++i) {
        QString ap = aperList.at(i);
        absPhot->qv_apertures.append(ap.toDouble());
    }

    // We need to know which color term was used so that we can
    // calculate the color correctly
    absPhot->color = ui->zpColorComboBox->currentText();
    absPhot->filter = ui->zpFilterComboBox->currentText();
    QString colorfilter = ui->zpColorComboBox->currentText();
    absPhot->colorfilter = colorfilter.remove("-").remove(ui->zpFilterComboBox->currentText());

    // Each matched entry contains
    // RA DEC Mag1REF Mag2REF Mag1ERR_REF Mag2ERR_REF MAG_AUTO MAGERR_AUTO MAG_APER(vector) MAGERR_APER(vector)

    // Object magnitudes are calculated for a fiducial ZP = 0, and need to be exposure time normalized
    // CHECK: this would go wrong if someone calibrates an image that is not normalized to one second!
    //    float normalization = 2.5*log10(myImage->exptime);
    float normalization = 0.;   // additive, in mag-space
    for (auto &match : matched) {
        absPhot->qv_RA.append(match[1]);
        absPhot->qv_DEC.append(match[0]);
        absPhot->qv_mag1ref.append(match[2]);
        absPhot->qv_mag2ref.append(match[3]);
        absPhot->qv_mag1errref.append(match[4]);
        absPhot->qv_mag2errref.append(match[5]);
        absPhot->qv_magauto.append(match[6] + normalization);
        absPhot->qv_magerrauto.append(match[7]);
        QVector<double> dummy1;
        QVector<double> dummy2;
        for (int i=0; i<numAper; ++i) {
            dummy1.append(match[8+i] + normalization);
            dummy2.append(match[8+numAper+i]);
        }
        absPhot->qv_magaper.append(dummy1);
        absPhot->qv_magerraper.append(dummy2);
        absPhot->qv_ManualMask.append(false);
        absPhot->numObj += 1;
    }

    // Calculations
    absPhot->initialized = true;
    // Individual color indices
    absPhot->getColor();
    // Calculate the ZP dependence on the color term
    // absPhot->regressionLinfit(absPhot->slope, absPhot->cutoff);
    if (!doColortermFit()) return;

    // Calculate the ZPs per aperture
    absPhot->getZP();
    plot();

    emit finished();
}

void AbsZeroPoint::finishedCalculations()
{
    ui->startPushButton->setEnabled(true);
    ui->startPushButton->setText("Start");
    ui->zpColorComboBox->setEnabled(true);
    ui->zpFilterComboBox->setEnabled(true);
    ui->zpRefcatComboBox->setEnabled(true);
}

void AbsZeroPoint::on_abortPushButton_clicked()
{
    if (!ui->startPushButton->isEnabled()) {
        if (workerThread) workerThread->quit();
        if (workerThread) workerThread->wait();
        finishedCalculations();
    }
}

void AbsZeroPoint::on_zpLoadPushButton_clicked()
{
    QString currentFileName =
            QFileDialog::getOpenFileName(this, tr("Select astrometrically calibrated image"), QDir::homePath(),
                                         tr("Images (*.fits *.fit *.fts *.FIT *.FITS *.FTS);; All (*.*)"));

    ui->zpImageLineEdit->setText(currentFileName);
    paintPathLineEdit(ui->zpImageLineEdit, currentFileName, "file");
}

void AbsZeroPoint::on_zpImageLineEdit_textChanged(const QString &arg1)
{
    paintPathLineEdit(ui->zpImageLineEdit, arg1, "file");
    fileInfo.setFile(arg1);
    // Clear the plot
    on_zpClearPushButton_clicked();
}

void AbsZeroPoint::on_zpRefcatComboBox_currentTextChanged(const QString &arg1)
{
    if (arg1.contains("ATLAS-REFCAT2")) {
        fill_combobox(ui->zpFilterComboBox, "g r i z");
        fill_combobox(ui->zpColorComboBox, "g-r g-i r-i i-z");
    }
    if (arg1.contains("GAIA")) {
        fill_combobox(ui->zpFilterComboBox, "G");
        fill_combobox(ui->zpColorComboBox, "BP-RP");
    }
    if (arg1.contains("PANSTARRS")) {
        fill_combobox(ui->zpFilterComboBox, "g r i z y");
        fill_combobox(ui->zpColorComboBox, "g-r g-i r-i i-z z-y");
    }
    else if (arg1.contains("SDSS")) {
        fill_combobox(ui->zpFilterComboBox, "u g r i z");
        fill_combobox(ui->zpColorComboBox, "u-g g-r g-i r-i i-z");
    }
    else if (arg1.contains("SKYMAPPER")) {
        fill_combobox(ui->zpFilterComboBox, "u v g r i z");
        fill_combobox(ui->zpColorComboBox, "u-g u-v g-r v-r g-i v-i r-i i-z");
    }
    else if (arg1.contains("APASS")) {
        fill_combobox(ui->zpFilterComboBox, "B V g r i");
        fill_combobox(ui->zpColorComboBox, "B-V g-r g-i r-i");
    }
    else if (arg1.contains("2MASS")) {
        fill_combobox(ui->zpFilterComboBox, "J H Ks");
        fill_combobox(ui->zpColorComboBox, "J-H J-Ks H-Ks");
    }
    else if (arg1.contains("VHS")) {
        fill_combobox(ui->zpFilterComboBox, "Y J H Ks");
        fill_combobox(ui->zpColorComboBox, "Y-J J-H J-Ks H-Ks");
    }
    // currently not implemented because of very patchy photometric coverage
    else if (arg1.contains("UKIDSS")) {
        fill_combobox(ui->zpFilterComboBox, "Y J H K");
        fill_combobox(ui->zpFilterComboBox, "Y-J J-H J-K H-K");
    }

    on_zpFilterComboBox_currentTextChanged(ui->zpFilterComboBox->currentText());
}

void AbsZeroPoint::on_zpClearPushButton_clicked()
{
    if (absPhot->numObj > 0) {
        clearText();
        absPhot->clear();
        ui->zpPlot->clearGraphs();
        ui->zpPlot->clearItems();
        ui->zpPlot->plotLayout()->clear();
        plot();
    }
}

void AbsZeroPoint::on_zpExportPushButton_clicked()
{
    if (absPhot->numObj == 0) return;
    QString path = fileInfo.absolutePath();
    QString base = fileInfo.completeBaseName();

    /* PDSF output is extremely slow, freezes the GUI for 1 minute;
     * deactivated for the time being
    QString saveFileName =
            QFileDialog::getSaveFileName(this, tr("Save Image"),
                                 base+".png",
                                 tr("Graphics file (*.png *.pdf)"));

    if ( saveFileName.isEmpty() ) return;
    QFileInfo graphicsInfo(saveFileName);
    if (! (graphicsInfo.suffix() == "png" || graphicsInfo.suffix() == "pdf")) {
        QMessageBox::information(this, tr("Invalid graphics type"),
                                 tr("Only graphics formats .png and .pdf are allowed."),
                                    QMessageBox::Ok);
        return;
    }

    if (graphicsInfo.suffix() == "pdf") ui->zpPlot->savePdf(saveFileName);
    else if (graphicsInfo.suffix() == "png") ui->zpPlot->savePng(saveFileName);
    */

    QString saveFileName =
            QFileDialog::getSaveFileName(this, tr("Save .png Image"),
                                         path+"/"+base+"_ZPcurve.png",
                                         tr("Graphics file (*.png)"));

    if ( saveFileName.isEmpty() ) return;
    QFileInfo graphicsInfo(saveFileName);
    if (graphicsInfo.suffix() != "png") {
        QMessageBox::information(this, tr("Invalid graphics type"),
                                 tr("Only .png format is currently allowed."),
                                 QMessageBox::Ok);
        return;
    }
    else {
        ui->zpPlot->savePng(saveFileName,0,0,1.25);
    }
}

void AbsZeroPoint::on_zpFilterComboBox_currentTextChanged(const QString &arg1)
{
    // Leave the colour combobox unchanged for Gaia. We always use BP-RP.
    if (ui->zpRefcatComboBox->currentText().contains("GAIA")) return;

    // Mask color terms that do not contain the primary filter:
    const QStandardItemModel* model = dynamic_cast< QStandardItemModel * >( ui->zpColorComboBox->model() );
    int length = ui->zpColorComboBox->count();
    bool updated = false;
    for (int i=0; i<length; ++i) {
        QString text = model->item(i,0)->text();
        if (!(model->item(i,0)->text()).contains(arg1)) {
            model->item(i,0)->setEnabled(false);
        }
        else {
            model->item(i,0)->setEnabled(true);
            if (!updated) {
                ui->zpColorComboBox->setCurrentIndex(i);
                updated = true;
            }
        }
    }

    on_zpClearPushButton_clicked();
}

/*
void AbsZeroPoint::endScript()
{
    // Process the result table
    QString image  = ui->zpImageLineEdit->text();
    QFileInfo fileInfo(image);
    QString catalogName = fileInfo.path()+"/"+fileInfo.completeBaseName()+".absphot.cat";
    // The file contains Ra Dec Mag1REF Mag2REF MagAUTO MagerrAUTO MagAPER(vector)

    QString apertures = ui->zpApertureLineEdit->text();
    apertures = apertures.replace(",", " ");
    apertures = apertures.simplified();
    QStringList aperList = apertures.split(" ");
    int numAper = aperList.length();
    absPhot->numAper += numAper;

    for (int i=0; i<numAper; ++i) {
        QString ap = aperList.at(i);
        absPhot->qv_apertures.append(ap.toDouble());
    }

    // We need to know which color term was used so that we can
    // calculate the color correctly
    absPhot->color = ui->zpColorComboBox->currentText();
    absPhot->filter = ui->zpFilterComboBox->currentText();
    QString colorfilter = ui->zpColorComboBox->currentText();
    absPhot->colorfilter = colorfilter.remove("-").remove(ui->zpFilterComboBox->currentText());

    QFile catalog(catalogName);
    QString line;
    QStringList lineList;

    // Read all the magnitudes into the absPhot class
    if ( catalog.open(QIODevice::ReadOnly)) {
        QTextStream stream( &catalog );
        while ( !stream.atEnd() ) {
            line = stream.readLine().simplified();
            // skip header lines
            if (line.contains("#")) continue;
            lineList = line.split(" ");
            absPhot->qv_RA.append(lineList.at(0).toDouble());
            absPhot->qv_DEC.append(lineList.at(1).toDouble());
            absPhot->qv_mag1ref.append(lineList.at(2).toDouble());
            absPhot->qv_mag1errref.append(lineList.at(3).toDouble());
            absPhot->qv_mag2ref.append(lineList.at(4).toDouble());
            absPhot->qv_mag2errref.append(lineList.at(5).toDouble());
            absPhot->qv_magauto.append(lineList.at(6).toDouble());
            absPhot->qv_magerrauto.append(lineList.at(7).toDouble());
            QVector<double> dummy = QVector<double>();
            for (int i=0; i<numAper; ++i) {
                dummy.append(lineList.at(8+i).toDouble());
            }
            absPhot->qv_magaper.append(dummy);
            absPhot->qv_ManualMask.append(false);
            absPhot->numObj += 1;
        }
        catalog.close();
    }

    // Calculations
    absPhot->initialized = true;
    // Individual color indices
    absPhot->getColor();
    // Calculate the ZP dependence on the color term
    // absPhot->regressionLinfit(absPhot->slope, absPhot->cutoff);
    if (!doColortermFit()) return;

    // Calculate the ZPs per aperture
    absPhot->getZP();

    plot();

    ui->startPushButton->setEnabled(true);
    ui->startPushButton->setText("Start");
    ui->zpColorComboBox->setEnabled(true);
    ui->zpFilterComboBox->setEnabled(true);
    ui->zpRefcatComboBox->setEnabled(true);
}
*/

// void AbsZeroPoint::showData(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event)
void AbsZeroPoint::showData(QCPAbstractPlottable *plottable, int dataIndex)
{
    if (plottable->parentLayerable()->objectName() == "aperRect") {
        absPhot->ZPSelected = QString::number(absPhot->qv_ZP[dataIndex], 'f', 3);
        absPhot->ZPerrSelected = QString::number(absPhot->qv_ZPerr[dataIndex], 'f', 3);
        absPhot->Color1Selected = QString::number(absPhot->fitParams[1], 'f', 4);
        absPhot->Color2Selected = QString::number(absPhot->fitParams[2], 'f', 4);
        absPhot->Color3Selected = QString::number(absPhot->fitParams[3], 'f', 4);
        absPhot->ColorErr1Selected = QString::number(absPhot->fitParamsErr[1], 'f', 4);
        absPhot->ColorErr2Selected = QString::number(absPhot->fitParamsErr[2], 'f', 4);
        absPhot->ColorErr3Selected = QString::number(absPhot->fitParamsErr[3], 'f', 4);
        updateCoaddHeader();
    }
    /*
     *
     * NOT WORKING! for some reason dataIndex does not select the object clicked, but some random other object.
     * It appears that data points plotted start with index 0 at left and have highest index at right.
    */
    else if (plottable->parentLayerable()->objectName() == "colorRect") {
        // Select only clicks on data points
//        if (QCPGraph *graph = qobject_cast<QCPGraph*>(plottable)) {
            // set or unset the flag for this data point
//            if (absPhot->qv_ManualFlag[dataIndex]) absPhot->qv_ManualFlag[dataIndex] = false;
//            else absPhot->qv_ManualFlag[dataIndex] = true;
//            qDebug() << qSetRealNumberPrecision(12) << dataIndex << "AA" << absPhot->qv_colorIndividual[dataIndex] << absPhot->qv_ZPIndividual[dataIndex] << absPhot->qv_RA[dataIndex] << absPhot->qv_DEC[dataIndex];
//            absPhot->regression(absPhot->slope, absPhot->cutoff);
//            absPhot->getZP();
            // plot();
//        }
    }
}

void AbsZeroPoint::plot()
{
    if (absPhot->numAper == 0) {
        ui->zpPlot->replot();
        ui->zpPlot->update();
        return;
    }

    ui->zpPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->zpPlot->plotLayout()->clear(); // clear default axis rect so we can start from scratch

    // The axes box for the ZP Growth curve
    QCPAxisRect *AxisRectAper = new QCPAxisRect(ui->zpPlot);
    AxisRectAper->setObjectName("aperRect");
    AxisRectAper->setupFullAxesBox(true);
    AxisRectAper->axis(QCPAxis::atBottom)->setLabel("Aperture diameter [pixel]");
    AxisRectAper->axis(QCPAxis::atLeft)->setLabel("ZP "+ui->zpFilterComboBox->currentText()+"  [ mag ]");
    AxisRectAper->axis(QCPAxis::atBottom)->setRange(0,100);
    AxisRectAper->axis(QCPAxis::atLeft)->setRange(24,28);

    // The axes box for the ZP color dependence
    QCPAxisRect *AxisRectColor = new QCPAxisRect(ui->zpPlot);
    AxisRectColor->setObjectName("colorRect");
    AxisRectColor->setupFullAxesBox(true);
    AxisRectColor->axis(QCPAxis::atBottom)->setLabel(ui->zpColorComboBox->currentText()+"  [ mag ]");
    AxisRectColor->axis(QCPAxis::atLeft)->setLabel("Ref - Inst  [ mag ]");
    AxisRectColor->axis(QCPAxis::atBottom)->setRange(-1,3);
    AxisRectColor->axis(QCPAxis::atLeft)->setRange(24,28);

    // Space for the title bar
    QString imagename = fileInfo.fileName();
    QCPTextElement *title = new QCPTextElement(ui->zpPlot, imagename, QFont("sans", 12));
    ui->zpPlot->plotLayout()->addElement(0, 0, title);
    ui->zpPlot->plotLayout()->addElement(1, 0, AxisRectAper);
    ui->zpPlot->plotLayout()->addElement(2, 0, AxisRectColor);

    /*
    // The Auto mag Rectangle in the growth curve. Must plot first, so other clickable data points end up on top
    QCPItemRect *magAuto = new QCPItemRect(ui->zpPlot);
    magAuto->topLeft->setType(QCPItemPosition::PositionType::ptPlotCoords);
    magAuto->bottomRight->setType(QCPItemPosition::PositionType::ptPlotCoords);
    magAuto->topLeft->setCoords(QPointF(minVec_T(absPhot->qv_apertures), absPhot->ZPauto + absPhot->ZPautoerr));
    magAuto->bottomRight->setCoords(QPointF(maxVec_T(absPhot->qv_apertures), absPhot->ZPauto - absPhot->ZPautoerr));
    magAuto->setClipToAxisRect(true);
    magAuto->setPen(QPen(Qt::red));
    magAuto->setSelectable(false);
    QBrush boxBrush(QColor(255, 0, 0, 100));
    magAuto->setBrush(boxBrush);

    double maxAp = maxVec_T(absPhot->qv_apertures);
    double minAp = minVec_T(absPhot->qv_apertures);

    // Add label for magAuto
    QCPItemText *magAutoText = new QCPItemText(ui->zpPlot);
    magAutoText->position->setType(QCPItemPosition::ptPlotCoords);
    magAutoText->setPositionAlignment(Qt::AlignLeft|Qt::AlignCenter);
    magAutoText->position->setCoords(0.5*(minAp+maxAp), absPhot->ZPauto);
    magAutoText->setSelectable(false);
    magAutoText->setText("MAG_AUTO");
    magAutoText->setTextAlignment(Qt::AlignLeft);
    magAutoText->setFont(QFont(font().family(), 9));
    magAutoText->setPadding(QMargins(4, 0, 4, 0));
    magAutoText->setPen(QPen(Qt::black));
    */

    // The graph for the ZP growth curve
    QCPGraph *aperZPGraph = ui->zpPlot->addGraph(AxisRectAper->axis(QCPAxis::atBottom), AxisRectAper->axis(QCPAxis::atLeft));
    aperZPGraph->setSelectable(QCP::stSingleData);
    aperZPGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, QPen(Qt::blue), QBrush(Qt::blue), 6));
    aperZPGraph->setLineStyle(QCPGraph::lsLine);
    aperZPGraph->setData(absPhot->qv_apertures,absPhot->qv_ZP);
    QCPSelectionDecorator *decorator = new QCPSelectionDecorator();
    QCPScatterStyle scatterstyle(QCPScatterStyle::ScatterShape::ssSquare, QColor("#009988"), QColor("#009988"), 12);
    decorator->setPen(QPen(QColor("#000000")));
    decorator->setScatterStyle(scatterstyle, QCPScatterStyle::spAll);
    aperZPGraph->setSelectionDecorator(decorator);

    double xmin = minVec_T(absPhot->qv_apertures);
    double xmax = maxVec_T(absPhot->qv_apertures);
    double ymin = minVec_T(absPhot->qv_ZP) - absPhot->qv_ZPerr[0];
    double ymax = maxVec_T(absPhot->qv_ZP) + absPhot->qv_ZPerr[absPhot->qv_ZPerr.size()-1];
    double dx = (xmax - xmin) * 0.05;
    double dy = (ymax - ymin) * 0.05;
    aperZPGraph->keyAxis()->setRange(xmin-dx, xmax+dx);
    aperZPGraph->valueAxis()->setRange(ymin-dy, ymax+dy);

    // Error bars
    QCPErrorBars *errorBarsZP = new QCPErrorBars(aperZPGraph->keyAxis(), aperZPGraph->valueAxis());
    errorBarsZP->setDataPlottable(aperZPGraph);
    errorBarsZP->setSelectable(QCP::SelectionType::stNone);
    errorBarsZP->setData(absPhot->qv_ZPerr);

    // The graph for the color dependence
    QCPGraph *colorZPGraph = ui->zpPlot->addGraph(AxisRectColor->axis(QCPAxis::atBottom), AxisRectColor->axis(QCPAxis::atLeft));
    colorZPGraph->setSelectable(QCP::stSingleData);
//    colorZPGraph->setSelectable(QCP::SelectionType::stNone);
    colorZPGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, QPen(QColor("#096169")), QBrush(QColor("#096169")), 6));
    colorZPGraph->setLineStyle(QCPGraph::lsNone);
    colorZPGraph->rescaleKeyAxis();
    colorZPGraph->rescaleValueAxis();
    colorZPGraph->setData(absPhot->qv_colorIndividual,absPhot->qv_ZPIndividual);

    // vertical error bars
    QCPErrorBars *errorValueBarsColor = new QCPErrorBars(colorZPGraph->keyAxis(), colorZPGraph->valueAxis());
    errorValueBarsColor->setDataPlottable(colorZPGraph);
    errorValueBarsColor->setSelectable(QCP::SelectionType::stNone);
    errorValueBarsColor->setData(absPhot->qv_ZPerrIndividual);
    errorValueBarsColor->setPen(QPen(QColor("#1eb7ce")));
    errorValueBarsColor->setErrorType(QCPErrorBars::etValueError);

    // horizontal error bars
    QCPErrorBars *errorKeyBarsColor = new QCPErrorBars(colorZPGraph->keyAxis(), colorZPGraph->valueAxis());
    errorKeyBarsColor->setDataPlottable(colorZPGraph);
    errorKeyBarsColor->setSelectable(QCP::SelectionType::stNone);
    errorKeyBarsColor->setData(absPhot->qv_colorErrIndividual);
    errorKeyBarsColor->setPen(QPen(QColor("#1eb7ce")));
    errorKeyBarsColor->setErrorType(QCPErrorBars::etKeyError);

    xmin = minVec_T(absPhot->qv_colorIndividual);
    xmax = maxVec_T(absPhot->qv_colorIndividual);
    ymin = minVec_T(absPhot->qv_ZPIndividual);
    ymax = maxVec_T(absPhot->qv_ZPIndividual);
    // we always want to see the origin:
    if (xmin > 0.) xmin = -0.1;
    dx = (xmax - xmin) * 0.08;
    dy = (ymax - ymin) * 0.08;
    colorZPGraph->keyAxis()->setRange(xmin-dx, xmax+dx);
    colorZPGraph->valueAxis()->setRange(ymin-dy, ymax+dy);

    // The graph for the color dependence outliers
    if (absPhot->num_outliers > 0) {
        QCPGraph *colorOutlierZPGraph = ui->zpPlot->addGraph(AxisRectColor->axis(QCPAxis::atBottom), AxisRectColor->axis(QCPAxis::atLeft));
        colorOutlierZPGraph->setSelectable(QCP::SelectionType::stNone);
        colorOutlierZPGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCrossCircle, 12));
        colorOutlierZPGraph->setPen(QPen(QColor(Qt::red)));
        colorOutlierZPGraph->setLineStyle(QCPGraph::lsNone);
        colorOutlierZPGraph->rescaleKeyAxis();
        colorOutlierZPGraph->rescaleValueAxis();
        colorOutlierZPGraph->setData(absPhot->qv_colorIndividualOutlier,absPhot->qv_ZPIndividualOutlier);
    }

    // Show LINEAR line fit to the color dependence
    /*
    QVector<QCPGraphData> dataFit(2);
    dataFit[0].key = xmin - 0.5;
    dataFit[1].key = xmax + 0.5;
    dataFit[0].value = absPhot->cutoff + absPhot->slope * dataFit[0].key;
    dataFit[1].value = absPhot->cutoff + absPhot->slope * dataFit[1].key;
    QCPGraph *fitGraph = ui->zpPlot->addGraph(AxisRectColor->axis(QCPAxis::atBottom), AxisRectColor->axis(QCPAxis::atLeft));
    fitGraph->data()->set(dataFit);
    fitGraph->setSelectable(QCP::stNone);
    fitGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black), QBrush(Qt::white), 6));
    fitGraph->setPen(QPen(QColor(Qt::black), 2));
    */

    // Show GSL fit to the color dependence
    QVector<QCPGraphData> dataFit;
    float step = (xmax - xmin + 2.*dx) / 100;  // plot at 100 intervals
    for (int i=0; i<=100; ++i) {
        double xval = xmin - dx + i*step;
        double yval = 0.;
        for (int k=0; k<4; ++k) {      // polynomial degree is hardcoded to three (higher order coeffs might be zero if a lower order was requested)
            yval += absPhot->fitParams[k]*pow(xval, double(k));
        }
        QCPGraphData dataPoint(xval, yval);
        dataFit.append(dataPoint);
    }
    QCPGraph *fitGraph = ui->zpPlot->addGraph(AxisRectColor->axis(QCPAxis::atBottom), AxisRectColor->axis(QCPAxis::atLeft));
    fitGraph->data()->set(dataFit);
    fitGraph->setSelectable(QCP::stNone);
    //    fitGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black), QBrush(Qt::white), 6));
    fitGraph->setPen(QPen(QColor(Qt::red), 2));

    // The graph for the color dependence (replot because of cluttering by the error bars)
    QCPGraph *colorZPGraph2 = ui->zpPlot->addGraph(AxisRectColor->axis(QCPAxis::atBottom), AxisRectColor->axis(QCPAxis::atLeft));
    colorZPGraph2->setSelectable(QCP::SelectionType::stMultipleDataRanges);
    colorZPGraph2->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, QPen(QColor("#096169")), QBrush(QColor("#096169")), 6));
    colorZPGraph2->setLineStyle(QCPGraph::lsNone);
    colorZPGraph2->rescaleKeyAxis();
    colorZPGraph2->rescaleValueAxis();
    colorZPGraph2->setData(absPhot->qv_colorIndividual,absPhot->qv_ZPIndividual);

    /*
    // The graph for manual outliers (if any)
    if (absPhot->num_ManualOutliers > 0) {
        QCPGraph *colorZPGraphManualOutlier = ui->zpPlot->addGraph(AxisRectColor->axis(QCPAxis::atBottom), AxisRectColor->axis(QCPAxis::atLeft));
        colorZPGraphManualOutlier->setSelectable(QCP::SelectionType::stMultipleDataRanges);
        colorZPGraphManualOutlier->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, QPen(QColor("#ff0000")), QBrush(QColor("#ff0000")), 9));
        colorZPGraphManualOutlier->setLineStyle(QCPGraph::lsNone);
        colorZPGraphManualOutlier->rescaleKeyAxis();
        colorZPGraphManualOutlier->rescaleValueAxis();
        colorZPGraphManualOutlier->setData(absPhot->qv_colorManualOutlier,absPhot->qv_ZPManualOutlier);

        // void AbsZeroPoint::showData(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event)

    }
    */


    // Place data points in the topmost layer
    //    QCPLayer *zpLayer = aperZPGraph->layer();
    //    ui->zpPlot->moveLayer(zpLayer,ui->zpPlot->layer("main"));
    //    QCPLayer *colorLayer = colorZPGraph->layer();
    //    colorLayer->setMode(QCPLayer::lmBuffered);

    ui->zpPlot->replot();
    ui->zpPlot->update();
}

void AbsZeroPoint::on_actionClose_triggered()
{
    emit abszpClosed();
    this->close();
}

bool AbsZeroPoint::doColortermFit()
{
    /*
    int fitOrder = ui->zpFitOrderSpinBox->value();
    if (!absPhot->regression(fitOrder)) {
        QMessageBox::information( this, "Ill-constrainend fit",
                                  "There are not enough photometric reference stars to perform a fit of order "+QString::number(fitOrder),
                                  QMessageBox::Ok);
        return false;
    }
    */

    absPhot->regressionLinfit();

    return true;
}

void AbsZeroPoint::on_zpFitOrderSpinBox_valueChanged(int arg1)
{
    if (performingStartup) return;   // This slot is triggered by the GUI setp when data are still unavailable
    if (!doColortermFit()) return;
    ui->zpPlot->clearGraphs();
    ui->zpPlot->clearItems();
    ui->zpPlot->plotLayout()->clear();
    plot();
    updateCoaddHeader();
}

void AbsZeroPoint::updateCoaddHeader()
{
    absPhot->Color1Selected = QString::number(absPhot->fitParams[1], 'f', 4);
    absPhot->Color2Selected = QString::number(absPhot->fitParams[2], 'f', 4);
    absPhot->Color3Selected = QString::number(absPhot->fitParams[3], 'f', 4);
    absPhot->ColorErr1Selected = QString::number(absPhot->fitParamsErr[1], 'f', 4);
    absPhot->ColorErr2Selected = QString::number(absPhot->fitParamsErr[2], 'f', 4);
    absPhot->ColorErr3Selected = QString::number(absPhot->fitParamsErr[3], 'f', 4);

    float fluxConv = 0.0;
    if (!absPhot->ZPSelected.isEmpty()) {
        fluxConv = 1.e6 * pow(10, -0.4 * (absPhot->ZPSelected.toFloat() - 8.90));    // Converting ZP to microJy
    }

    // The coadded FITS file
    fitsfile *fptr;
    int status = 0;
    QString name = ui->zpImageLineEdit->text();
    fits_open_file(&fptr, name.toUtf8().data(), READWRITE, &status);
    fits_update_key_flt(fptr, "ZPD", absPhot->ZPSelected.toFloat(), 6, "Photometric ZP in ZPD_FILT", &status);
    fits_update_key_flt(fptr, "ZPD_ERR", absPhot->ZPerrSelected.toFloat(), 6, "Error of ZPD", &status);
    fits_update_key_str(fptr, "ZPD_FILT", ui->zpFilterComboBox->currentText().toUtf8().data(), "Filter for ZPD", &status);
    fits_update_key_flt(fptr, "ZPD_C1", absPhot->Color1Selected.toFloat(), 6, "Linear color term in ZPD_SURV", &status);
    fits_update_key_flt(fptr, "ZPD_C2", absPhot->Color2Selected.toFloat(), 6, "Quadratic color term in ZPD_SURV", &status);
    fits_update_key_flt(fptr, "ZPD_C3", absPhot->Color3Selected.toFloat(), 6, "Cubic color term in ZPD_SURV", &status);
    fits_update_key_flt(fptr, "ZPD_CER1", absPhot->ColorErr1Selected.toFloat(), 6, "Error linear color term in ZPD_SURV", &status);
    fits_update_key_flt(fptr, "ZPD_CER2", absPhot->ColorErr2Selected.toFloat(), 6, "Error quadratic color term in ZPD_SURV", &status);
    fits_update_key_flt(fptr, "ZPD_CER3", absPhot->ColorErr3Selected.toFloat(), 6, "Error cubic color term in ZPD_SURV", &status);
    fits_update_key_str(fptr, "ZPD_INDX", ui->zpColorComboBox->currentText().toUtf8().data(), "Color index in ZPD_SURV", &status);
    if (ui->zpRefcatComboBox->currentText().contains("2MASS")) {
        fits_update_key_str(fptr, "ZPD_SYST", "ABmag", "Mag. system (converted from 2MASS VEGA mags)", &status);
    }
    else if (ui->zpRefcatComboBox->currentText().contains("UKIDSS")) {
        fits_update_key_str(fptr, "ZPD_SYST", "ABmag", "Mag. system (converted from 2MASS UKIDSS mags)", &status);
    }
    else if (ui->zpRefcatComboBox->currentText().contains("APASS")) {
        fits_update_key_str(fptr, "ZPD_SYST", "ABmag", "Mag. system (B and V VEGA mags converted to AB)", &status);
    }
    else if (ui->zpRefcatComboBox->currentText().contains("GAIA")) {
        fits_update_key_str(fptr, "ZPD_SYST", "ABmag", "Mag. system (Gaia mags converted to AB)", &status);
    }
    else {
        fits_update_key_str(fptr, "ZPD_SYST", "ABmag", "Magnitude system", &status);
    }
    fits_update_key_str(fptr, "ZPD_SURV", ui->zpRefcatComboBox->currentText().toUtf8().data(), "Survey and filter system", &status);
    fits_update_key_flt(fptr, "FLUXCONV", fluxConv, 6, "Factor to convert image to microJansky", &status);
    fits_close_file(fptr, &status);

    ui->zpHeaderLabel->setText("FITS header entries were updated:");
    ui->zpPlainTextEdit->clear();

    if (status) {
        CfitsioErrorCodes *errorCodes = new CfitsioErrorCodes(this);
        QString code = errorCodes->errorKeyMap.value(status);
        ui->zpPlainTextEdit->appendHtml("<font color=\"#ee0000\">Cfitsio error while updating header: " + code + "</font>");
    }

    QFont notoFontMono("Noto Sans Mono CJK JP", 8, QFont::Normal);
    ui->zpPlainTextEdit->setFont(notoFontMono);

    ui->zpPlainTextEdit->appendHtml("<tt>ZPD&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;= "+absPhot->ZPSelected+"</tt>");
    ui->zpPlainTextEdit->appendHtml("<tt>ZPD_ERR&nbsp;= "+absPhot->ZPerrSelected+"</tt>");
    ui->zpPlainTextEdit->appendHtml("<tt>ZPD_FILT= '"+ui->zpFilterComboBox->currentText()+"'</tt>");
    ui->zpPlainTextEdit->appendHtml("<tt>ZPD_C1&nbsp;&nbsp;= "+absPhot->Color1Selected+"</tt>");
    ui->zpPlainTextEdit->appendHtml("<tt>ZPD_C2&nbsp;&nbsp;= "+absPhot->Color2Selected+"</tt>");
    ui->zpPlainTextEdit->appendHtml("<tt>ZPD_C3&nbsp;&nbsp;= "+absPhot->Color3Selected+"</tt>");
    ui->zpPlainTextEdit->appendHtml("<tt>ZPD_CER1= "+absPhot->ColorErr1Selected+"</tt>");
    ui->zpPlainTextEdit->appendHtml("<tt>ZPD_CER2= "+absPhot->ColorErr2Selected+"</tt>");
    ui->zpPlainTextEdit->appendHtml("<tt>ZPD_CER3= "+absPhot->ColorErr3Selected+"</tt>");
    ui->zpPlainTextEdit->appendHtml("<tt>ZPD_INDX= '"+ui->zpColorComboBox->currentText()+"'</tt>");

    // CHECK: everytime we add new reference catalogs
    if (ui->zpRefcatComboBox->currentText().contains("2MASS")) {
        ui->zpPlainTextEdit->appendHtml("<tt>ZPD_SYST= 'ABmag' (converted from 2MASS VEGA mags)</tt>");
    }
    else if (ui->zpRefcatComboBox->currentText().contains("UKIDSS")) {
        ui->zpPlainTextEdit->appendHtml("<tt>ZPD_SYST= 'ABmag' (converted from UKIDSS VEGA mags)</tt>");
    }
    else if (ui->zpRefcatComboBox->currentText().contains("APASS")) {
        ui->zpPlainTextEdit->appendHtml("<tt>ZPD_SYST= 'ABmag' (B and V VEGA mags converted to AB mag)</tt>");
    }
    else if (ui->zpRefcatComboBox->currentText().contains("GAIA")) {
        ui->zpPlainTextEdit->appendHtml("<tt>ZPD_SYST= 'ABmag' (Gaia mags converted to AB mag)</tt>");
    }
    else if (ui->zpRefcatComboBox->currentText().contains("VHS")) {
        ui->zpPlainTextEdit->appendHtml("<tt>ZPD_SYST= 'ABmag' (converted from VEGA mags)</tt>");
    }
    else {
        ui->zpPlainTextEdit->appendHtml("<tt>ZPD_SYST= 'ABmag'</tt>");
    }
    ui->zpPlainTextEdit->appendHtml("<tt>ZPD_SURV= '"+ui->zpRefcatComboBox->currentText()+"'</tt>");
    ui->zpPlainTextEdit->appendHtml("<tt>FLUXCONV= "+QString::number(fluxConv, 'f', 4)+" (conversion factor to microJy)</tt>");
}

void AbsZeroPoint::on_closePushButton_clicked()
{
    emit abszpClosed();
    this->close();
}

void AbsZeroPoint::on_showAbsphotPushButton_clicked()
{
    // TODO: show sources that are kept after the fit in yet another color
    if (!iViewOpen) {
        iView = new IView("FITSmonochrome", ui->zpImageLineEdit->text(), this);
        connect(iView, &IView::closed, this, &AbsZeroPoint::iViewClosed);
        connect(this, &AbsZeroPoint::updateAbsPhotPlot, iView, &IView::showAbsPhotReferences);
        iView->show();
        iView->AbsPhotReferencePathName = myImage->path;
        iView->showAbsPhotReferences(true);
        iViewOpen = true;
    }
    else {
        iView->show();
        iView->AbsPhotReferencePathName = myImage->path;
        emit updateAbsPhotPlot(true);
    }
}

void AbsZeroPoint::iViewClosed()
{
    iViewOpen = false;
}
