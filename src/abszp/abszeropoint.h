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

#include "../threading/abszpworker.h"
#include "../qcustomplot.h"
#include "../myimage/myimage.h"
#include "../iview/iview.h"

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
    explicit AbsZeroPoint(QString image, QWidget *parent = nullptr);
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
    void updateAbsPhotPlot(bool checked);

public slots:
    void updateVerbosity(int verbosityLevel);
    void iViewClosed();

private slots:
    void on_actionClose_triggered();
    void on_closePushButton_clicked();
    void on_showAbsphotPushButton_clicked();
    void on_zpImageLineEdit_textChanged(const QString &arg1);
    void on_zpRefcatComboBox_currentTextChanged(const QString &arg1);
    void on_zpLoadPushButton_clicked();
    void on_abortPushButton_clicked();
    void on_startPushButton_clicked();
    void on_zpClearPushButton_clicked();
    void on_zpExportPushButton_clicked();
    void on_zpFilterComboBox_currentTextChanged(const QString &arg1);
    void showData(QCPAbstractPlottable *plottable, int dataIndex);
    void on_zpFitOrderSpinBox_valueChanged(int arg1);
    void displayMessage(QString message, QString type);
    void finishedCalculations();
    void buildAbsPhot();
    void criticalReceived();

private:
    void defaults_if_empty();
    void initGUI();
    void updateHeader();
    void validate();
    bool doColortermFit();
    void updateCoaddHeader();
    void pushBeginMessage();
    void pushEndMessage();
    void queryRefCat();
    void loadPreferences();
    void closeEvent(QCloseEvent *event);
    void writeAbsPhotRefcat();
    void clearText();
    double getFirstZPestimate();

    Ui::AbsZeroPoint *ui;

    QString taskBasename = "AbsZeropoint";
    QString thelidir;
    QString userdir;
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

    double refCatUpperMagLimit = 40.;

    QVector<QVector<double>> refDat;
    QVector<QVector<double>> objDat;

    QVector<QVector<double>> matched;

    MyImage *myImage;

    bool workerInit = false;
    bool workerThreadInit = false;

    IView *iView;
    bool iViewOpen = false;

};

#endif // ABSZEROPOINT_H
