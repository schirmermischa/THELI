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

#ifndef COLORPICTURE_H
#define COLORPICTURE_H

#include "../processingExternal/errordialog.h"
#include "../threading/colorpictureworker.h"
#include "../iview/iview.h"
#include "../query/query.h"
#include "refcatdata.h"

#include <QMainWindow>
#include <QStringListModel>
#include <QComboBox>
#include <QString>
#include <QButtonGroup>
#include <QTableView>

class ColorPictureWorker;

namespace Ui {
class ColorPicture;
}

class ColorPicture : public QMainWindow
{
    Q_OBJECT

public:
    explicit ColorPicture(instrumentDataType *instrumentData, QString main = "", QWidget *parent = nullptr);
    ~ColorPicture();

    bool iViewOpen = false;

    int verbosity = 0;
    int maxCPU = 1;

    void taskInternalColorCalib();
    void taskInternalCropCoadds();
    void taskInternalFits2Tiff();
    void taskInternalBBNBratio();
    void taskInternalBBNBcombine();

signals:
    void colorFactorChanged(float redFactor, float blueFactor);
    void messageAvailable(QString message, QString type);
    void progressUpdate(float progress);
    void resetProgressBar();
    void finished();
    void showMessageBox(QString trigger, QString part1, QString part2);
    void updateNrefStars(QString name, long number);
    void addCombinedImage(MyImage *myImage);

public slots:
    void taskFinished(QString taskname);
    void updateVerbosity(int verbosityLevel);

private slots:
    void measureStatistics(long x, long y);
    void on_actionClose_triggered();
    void on_createTiffPushButton_clicked();
    void on_redComboBox_currentIndexChanged(int index);
    void on_greenComboBox_currentIndexChanged(int index);
    void on_blueComboBox_currentIndexChanged(int index);
    void on_narrowbandComboBox_currentTextChanged(const QString &arg1);
    void on_broadbandComboBox_currentTextChanged(const QString &arg1);
    void on_getRatioPushButton_clicked();
    void on_previewCalibPushButton_clicked();
    void on_calibratePushButton_clicked();
    void on_selectDirPushButton_clicked();
    void on_getCoaddsPushButton_clicked();
    void on_clearCoaddsPushButton_clicked();
    void on_BBNBcombinePushButton_clicked();
    void sendColorFactors();
    void updateCalibFactors();
    void updateImageListView();
    void updateFilterRatio();
    void updateIviewStatus();
    void updateTiff();
    void updateColorFactorsExternal(QString redFactor, QString blueFactor);
    void validate();
    void updateNrefStarsReceived(QString name, long number);
    void displayMessage(QString message, QString type);
    void criticalReceived();
    void on_abortPushButton_clicked();
    void addCombinedImageReceived(MyImage *combinedImage);

private:
    QString mainDir;
    Ui::ColorPicture *ui;

    QString thelidir;
    QString userdir;
    instrumentDataType *instData;
    QStringListModel *coaddImageModel;
    QStringListModel *coaddDirModel;
    ErrorDialog *errordialog = new ErrorDialog(this);
    IView *iView;
    QButtonGroup *resultButtonGroup = new QButtonGroup(this);
    QPalette buttonPalette;
    bool statisticsRetrieved = false;

    QThread *workerThread;
    ColorPictureWorker *colorpictureWorker;

    omp_lock_t wcslock;

    float bbnbFluxRatio = 1.0;
    float bbnbFluxRatioError = 0.0;

    QList<MyImage*> coaddList;     // all coadds, including weights
    QList<MyImage*> croppedList;   // only the cropped coadded images, without the weights

    QStringList refCatList = {"SDSS", "APASS", "PANSTARRS"};

    Query *SDSSquery = new Query(&verbosity);
    Query *APASSquery = new Query(&verbosity);
    Query *PANSTARRSquery = new Query(&verbosity);

    RefCatData *SDSS = new RefCatData("SDSS", this);
    RefCatData *APASS = new RefCatData("APASS", this);
    RefCatData *PANSTARRS = new RefCatData("PANSTARRS", this);

    typedef struct {
      QString catname;
      QString nstars;
      QString bfac;
      QString gfac;
      QString rfac;
      QString bfacerr;
      QString gfacerr;
      QString rfacerr;
    } _photcatresult_;

    _photcatresult_ photcatresult[4];

    void addDirectories();
    void checkCalibrationFactor(QLineEdit *le);
    void findRecursion(const QString &path, QStringList *result);
    void readFilterRatioResults(QString filename);
    void resetResultButtonGroup(QString resetLabels = "");
    void refreshComboBoxes();
    void showDone(QPushButton *pushButton, QString text);
    void toggleCalibResult();
    void loadPreferences();
    void filterSolarTypeStars(QList<Query *> queryList);
    void colorCalibSegmentImages();
    void colorCalibRetrieveCatalogs(QList<Query *> queryList);
    void colorCalibMatchReferenceCatalog(const QVector<QVector<double> > &matchedRGB, RefCatData *REFCAT, float tolerance);
    void colorCalibMatchCatalogs();
    MyImage *identifyCroppedMyImage(QString name);
    QVector<QVector<double> > getObjectData(MyImage *myImage);
    void updateBBNBcombine();
    void writeG2refcat(const QString refcatName, const QVector<QVector<double> > matchedREFCAT);
    void writeRGBTIFF(QVector<float> &R, QVector<float> &G, QVector<float> &B, long n, long m, float min, float max, QString path);
};


#endif // COLORPICTURE_H
