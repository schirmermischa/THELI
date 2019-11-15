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

#ifndef IVIEW_H
#define IVIEW_H

#include "mygraphicsview.h"
#include "mygraphicsscene.h"
#include "mygraphicsellipseitem.h"
#include "dockwidgets/ivconfdockwidget.h"
#include "dockwidgets/ivscampdockwidget.h"
#include "dockwidgets/ivcolordockwidget.h"
#include "../myimage/myimage.h"

#include "fitsio2.h"
#include "wcs.h"
#include "wcshdr.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QLabel>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QDebug>
#include <QLayout>
#include <QTimer>
#include <QSpinBox>
#include <QVector>
#include <QLineEdit>
#include <QPushButton>
#include <QActionGroup>

namespace Ui {
class IView;
}

class IView : public QMainWindow
{
    Q_OBJECT

public:
    explicit IView(QWidget *parent = nullptr);
    explicit IView(QString mode, QWidget *parent = nullptr);
    explicit IView(QString mode, QList<MyImage *> &list, QString dirname, QWidget *parent = nullptr);
    explicit IView(QString mode, QString name, QWidget *parent = nullptr);
    explicit IView(QString mode, QString dirname, QString filter, QWidget *parent = nullptr);
    explicit IView(QString mode, QString dirname, QString fileName, QString filter, QWidget *parent = nullptr);
    explicit IView(QString mode, QString dirname, QString rChannel, QString gChannel, QString bChannel,
                   float factorR, float factorB, QWidget *parent = nullptr);

    ~IView();
    QString dirName;
    QString startDirName = "";  // the dir name of the first image loaded (never changed after init)
    QString filterName;
    QString currentSuffix;
    QString middleMouseMode = "DragMode";
    bool startDirNameSet = false;
    bool scampInteractiveMode = false;
    bool scampCorrectlyClosed = false;
    void setMiddleMouseMode(QString mode);
    void switchMode(QString mode = "");
    void loadPNG(QString filename, int currentId = 0);
    void autoContrast(QString colorMode = "");
    void setImageList(QString filter);
    void redrawSkyCirclesAndCats();

    bool weightMode = false;   // whether iview displays the image data or the weight

    IvConfDockWidget *icdw;
    IvScampDockWidget *scampdw;
    IvColorDockWidget *colordw;

    MyGraphicsView *myGraphicsView;
    MyGraphicsScene *scene = new MyGraphicsScene(this);
    int numImages = 0;
    int zoomLevel = 0;
    float dynRangeMin;
    float dynRangeMax;
    QLabel *pageLabel = new QLabel(this);
    QStringList imageList;
    QStringList imageListChipName;
    QList<QGraphicsEllipseItem*> skyCircleItems;
    QList<QGraphicsEllipseItem*> acceptedSkyCircleItems;

    int currentId = 0;

    QList<MyImage*> myImageList;

signals:
    void abortPlay();
    void colorFactorChanged(QString redFactor, QString blueFactor);
    void closed();
    void statisticsRequested(long x, long y);
    void solutionAcceptanceState(bool state);
    void middleMouseModeChanged(QString mode);
    void currentlyDisplayedIndex(int index);

private slots:
    void adjustBrightnessContrast(QPointF point);
    void appendSkyCircle();
    void backAction_triggered();
//    void blueSliderMoved(const int &sliderValue);
//    void blueFactorEdited(const QString &value);
    void clearSeparationVector();
    void drawSeparationVector(QPointF pointStart, QPointF pointEnd);
    void drawSkyCircle(QPointF pointStart, QPointF pointEnd);
    void drawSkyRectangle(QPointF pointStart, QPointF pointEnd);
    void endAction_triggered();
    void forwardAction_triggered();
    void initDynrangeDrag();
    void initSeparationVector(QPointF pointStart);
    void loadImage();
    void nextAction_triggered();
    //    void on_zoomInPushButton_clicked();
    //    void on_zoomOutPushButton_clicked();
    //    void on_zoomZeroPushButton_clicked();
    //    void on_zoomFitPushButton_clicked();
    //    void on_autocontrastPushButton_clicked();
    //    void on_minLineEdit_returnPressed();
    //    void on_maxLineEdit_returnPressed();
    void on_actionDragMode_triggered();
    void on_actionSkyMode_triggered();
    //    void on_filterLineEdit_textChanged(const QString &arg1);
    void previousAction_triggered();
//    void redSliderMoved(const int &sliderValue);
//    void redFactorEdited(const QString &value);
//    void resetRedFactor();
//    void resetBlueFactor();
    void startAction_triggered();
    void showCurrentMousePos(QPointF point);
    void sendStatisticsCenter(QPointF point);
    void showSourceCat();
    void showReferenceCat();
    void updateSkyCircles();
//    void validate();
    //    void initPanning();
    //    void centerView(QPointF point);
    //    void endPanning();
    //  void on_filterLineEdit_editingFinished();    void drawSkyRectangle(QPointF pointStart, QPointF pointEnd);
    // void on_waveletPushButton_clicked();
public slots:
    void clearAll();
    void colorFactorChanged_receiver(QString redFactor, QString blueFactor);
    void loadFITSexternal(QString fileName, QString filter);
    void mapFITS();
    void showG2References(bool checked);
    void updateColorViewInternal(float redFactor, float blueFactor);
    void updateColorViewExternal(float redFactor, float blueFactor);
    void solutionAcceptanceStateReceived(bool state);
    void autoContrastPushButton_toggled_receiver(bool checked);
    void zoomFitPushButton_clicked_receiver(bool checked);
    void minmaxLineEdit_returnPressed_receiver(QString rangeMin, QString rangeMax);
    void zoomInPushButton_clicked_receiver();
    void zoomOutPushButton_clicked_receiver();
    void zoomZeroPushButton_clicked_receiver();
    void loadFromRAM(MyImage *it, int level);
    void loadFromRAMlist(const QModelIndex &index);
    void loadFITSexternalRAM(int index);
protected:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent *event);

private:
    Ui::IView *ui;

    QLabel *coordsLabel;
    QActionGroup *middleMouseActionGroup = new QActionGroup(this);
    int screenHeight;
    int screenWidth;
    float dynRangeMinDragStart = 0.;
    float dynRangeMaxDragStart = 0.;
    float medVal;
    float rmsVal;
    double crpix1;
    double crpix2;
    double crval1;
    double crval2;
    double cd1_1;
    double cd1_2;
    double cd2_1;
    double cd2_2;
    double plateScale = 0.;
    double skyRa;
    double skyDec;
    bool hasWCS = false;
    bool startLeftClickInsideItem = false;
    QString displayMode = "CLEAR";
    int timerId;
    QString ChannelR;
    QString ChannelG;
    QString ChannelB;
    bool allChannelsRead = false;
    unsigned char *dataInt;
    unsigned char *dataIntR;
    unsigned char *dataIntG;
    unsigned char *dataIntB;
    QGraphicsPixmapItem *pixmapItem;
    bool dataIntSet = false;
    bool dataIntRSet = false;
    bool dataIntGSet = false;
    bool dataIntBSet = false;
    int naxis1;
    int naxis2;

    struct wcsprm *wcs;
    bool wcsInit = false;
    char *fullheader = nullptr;

    QTimer *timer = new QTimer(this);

 //   QGraphicsScene *zoomScene = new QGraphicsScene();

    QList<QGraphicsLineItem*> vectorLineItems;
    QList<QGraphicsTextItem*> vectorTextItems;
    QList<QGraphicsEllipseItem*> sourceCatItems;
    QList<QGraphicsRectItem*> refCatItems;
    QList<QGraphicsRectItem*> G2refCatItems;
    QList<QGraphicsRectItem*> skyRectItems;
    QList<QGraphicsTextItem*> skyTextItems;

    QLabel *speedLabel = new QLabel(this);
    QSpinBox *speedSpinBox = new QSpinBox(this);

    bool icdwDefined = false;
    bool scampdwDefined = false;

    QVector<float> fitsData;
    QVector<float> fitsDataR;
    QVector<float> fitsDataG;
    QVector<float> fitsDataB;

    QRect adjustGeometry();
    void clearItems();
    void clearVectorItems();
    void clearSkyRectItems();
    void clearSkyCircleItems();
    void dumpSkyCircleCoordinates();
    QString dec2hex(double angle);
    void compressDynrange(const QVector<float> &fitsdata, unsigned char *intdata, float colorCorrectionFactor = 1.0);
    QString getVectorLabel(double separation);
    void getImageStatistics(QString colorMode = "");
    void getVectorOffsets(const qreal dx, const qreal dy, qreal &x_yoffset,
                          qreal &y_xoffset, qreal &d_xoffset, qreal &d_yoffset);
    void initGUI();
    void initGUIstep2();
    void loadFITS(QString filename, int currentId = 0, qreal scaleFactor = 1.0);
    bool loadFITSdata(QString filename, QVector<float> &data, QString colorMode = "");
    void loadColorFITS(qreal scaleFactor);
    void makeConnections();
    bool readRaDecCatalog(QString fileName, QList<QGraphicsRectItem *> &items, double size, int width, QColor color);
    void readPreferenceSettings();
    void sky2xy(double ra, double dec, double &x, double &y);
    void sky2xy_linear(double ra, double dec, double &x, double &y);
    void setCurrentId(QString filename);
//    void sliderToText(int sliderValue, QString channel);
//    void textToSlider(QString value, QString channel);
    void writePreferenceSettings();
    void xy2sky(double x, double y, QString button = "");
    void xy2sky_linear(double x, double y, QString button = "");
    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }
    void addDockWidgets();
    void setImageListFromMemory();
    void imageListToChipName();
    void replotCatalogAfterZoom();
};

#endif // IVIEW_H
