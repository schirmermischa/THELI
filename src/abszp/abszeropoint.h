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

#ifndef ABSZEROPOINT_H
#define ABSZEROPOINT_H

#include "../processingExternal/errordialog.h"
#include "../threading/abszpworker.h"
#include "../qcustomplot.h"
#include "../instrumentdata.h"
#include "../myimage/myimage.h"

#include <QMainWindow>
#include <QThread>
#include <QVector>
#include <QFileInfo>

#include "absphot.h"

class AbszpWorker;

namespace Ui {
class AbsZeroPoint;
}

class AbsZeroPoint : public QMainWindow
{
    Q_OBJECT

public:
    explicit AbsZeroPoint(QString image, instrumentDataType *instrumentData, QWidget *parent = nullptr);
    ~AbsZeroPoint();

    void plot();
    QString startImage = "";
    AbsPhot *absPhot = new AbsPhot();
    QThread *workerThread;
    AbszpWorker *abszpWorker;

    float autoMaxVal = 100.;       // determined by the coadd task on the coadded image

    int verbosity = 0;
    int maxCPU = 1;

    void taskInternalAbszeropoint();
    void updateSaturationValue(float value);

signals:
    void messageAvailable(QString message, QString type);
    void progressUpdate(float progress);
    void resetProgressBar();
    void finished();
    void readyForPlotting();
    void abszpClosed();

public slots:
    void processErrorOutput(QString errormessage, QString logname);
    void updateVerbosity(int verbosityLevel);

private slots:
    void on_actionClose_triggered();
    void on_zpImageLineEdit_textChanged(const QString &arg1);
    void on_zpRefcatComboBox_currentTextChanged(const QString &arg1);
    void on_zpLoadPushButton_clicked();
    void on_abortPushButton_clicked();
    void on_startPushButton_clicked();
    void on_zpClearPushButton_clicked();
    void on_zpExportPushButton_clicked();
    void on_zpFilterComboBox_currentTextChanged(const QString &arg1);
    void showData(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event);
    void on_zpFitOrderSpinBox_valueChanged(int arg1);
    void displayMessage(QString message, QString type);
    void finishedCalculations();
    void buildAbsPhot();
    void criticalReceived();
    void on_closePushButton_clicked();

private:
    void defaults_if_empty();
    void initGUI();
    void updateHeader();
    void validate();

    Ui::AbsZeroPoint *ui;

    QString taskBasename = "AbsZeropoint";
    QString thelidir;
    QString userdir;
    instrumentDataType *instData;
    ErrorDialog *errordialog = new ErrorDialog(this);
    QFileInfo fileInfo;
    QStringList totalCommandList;
    bool performingStartup = true;

    QVector<double> raRefCat;
    QVector<double> deRefCat;
    QVector<float> mag1RefCat;
    QVector<float> mag2RefCat;
    QVector<float> mag1errRefCat;
    QVector<float> mag2errRefCat;
    long numRefSources = 0;

    QVector<QVector<double>> matched;

    MyImage *myImage;

    bool workerInit = false;
    bool workerThreadInit = false;

    void clearText();
    bool doColortermFit();
    void updateCoaddHeader();
    void pushBeginMessage();
    void pushEndMessage();
    void queryRefCat();
    void loadPreferences();
    void closeEvent(QCloseEvent *event);
};

#endif // ABSZEROPOINT_H
