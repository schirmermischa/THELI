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
#include "dockwidgets/ivwcsdockwidget.h"
#include "dockwidgets/ivstatisticsdockwidget.h"
#include "dockwidgets/ivfinderdockwidget.h"
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
    void autoContrast();
    void setImageList(QString filter);
    void redrawSkyCirclesAndCats();
    void clearItems();
    void updateCDmatrixFITS();
    void setCatalogOverlaysExternally(bool sourcecatShown, bool refcatShown);

    void xy2sky(double x, double y, QString button = "");

    bool weightMode = false;   // whether iview displays the image data or the weight

    bool refcatSourcesShown = false;
    bool sourcecatSourcesShown = false;

    QString G2referencePathName = "";
    QString AbsPhotReferencePathName = "";

    IvConfDockWidget *icdw;
    IvScampDockWidget *scampdw;
    IvColorDockWidget *colordw;
    IvStatisticsDockWidget *statdw = new IvStatisticsDockWidget(this);
    IvFinderDockWidget *finderdw = new IvFinderDockWidget(this);
    IvWCSDockWidget *wcsdw = new IvWCSDockWidget(this);

    MyGraphicsView *myGraphicsView;
//    MyGraphicsScene *scene = new MyGraphicsScene(this);
    MyGraphicsScene *scene = new MyGraphicsScene();
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

    int verbosity = 0;

    float globalMedian = 0.;
    float globalMedianR = 0.;
    float globalMedianG = 0.;
    float globalMedianB = 0.;
    float globalRMS = 0.;
    float globalMean = 0.;

    QList<MyImage*> myImageList;

    MyImage *currentMyImage = nullptr;
    QString currentFileName = "";

    int naxis1 = 0;
    int naxis2 = 0;

    QVector<float> fitsData;
    QVector<float> fitsDataR;
    QVector<float> fitsDataG;
    QVector<float> fitsDataB;

    QString displayMode = "CLEAR";

    void qimageToBinned(qreal qx, qreal qy, qreal &bx, qreal &by);
    QRect qimageToBinned(const QRectF qrect);
    QPointF qimageToBinned(const QPointF qpoint);
    void binnedToQimage(qreal bx, qreal by, qreal &qx, qreal &qy);
    QPointF binnedToQimage(const QPointF bpoint);

signals:
    void abortPlay();
    void colorFactorChanged(QString redFactor, QString blueFactor);
    void closed();
    void statisticsRequested(long x, long y);
    void solutionAcceptanceState(bool state);
    void middleMouseModeChanged(QString mode);
    void currentlyDisplayedIndex(int index);
    void updateNavigatorMagnified(QGraphicsPixmapItem *magnifiedPixmapItem, qreal scaleFactor, float dx, float dy);
    void updateNavigatorBinned(QGraphicsPixmapItem *binnedPixmapItem);
    void updateNavigatorBinnedViewport(QRect rect);
    void statisticsSampleAvailable(const QVector<float> &sample);
    void statisticsSampleColorAvailable(const QVector<float> &sampleR, const QVector<float> &sampleG, const QVector<float> &sampleB);
    void updateNavigatorBinnedWCS(wcsprm *cd, bool wcsinit);
    void clearMagnifiedScene();

private slots:
    void adjustBrightnessContrast(QPointF point);
    void appendSkyCircle();
    void backAction_triggered();
    void clearSeparationVector();
    void drawSeparationVector(QPointF pointStart, QPointF pointEnd);
    void drawSkyCircle(QPointF pointStart, QPointF pointEnd);
    void drawMaskingPolygon(QPointF pointStart, QPointF pointEnd);
    void drawSkyRectangle(QPointF pointStart, QPointF pointEnd);
    void endAction_triggered();
    void forwardAction_triggered();
    void initDynrangeDrag();
    void initSeparationVector(QPointF pointStart);
    void loadImage();
    void middlePressResetCRPIXreceived();
    void nextAction_triggered();
    void on_actionDragMode_triggered();
    void on_actionSkyMode_triggered();
    void on_actionWCSMode_triggered();
    void on_actionMaskingMode_triggered();
    void previousAction_triggered();
//    void startAction_triggered();        // public
    void showCurrentMousePos(QPointF point);
    void sendStatisticsCenter(QPointF point);
    void showSourceCat();
    void showReferenceCat();
    void updateSkyCircles();
    void updateCRPIX(QPointF pointStart, QPointF pointEnd);
    void updateCRPIXFITS();
    void updateCDmatrix(double cd11, double cd12, double cd21, double cd22);
    void updateNavigatorMagnifiedReceived(QPointF point);
//    void mouseEnteredViewReceived();
//    void mouseLeftViewReceived();
    void on_actionImage_statistics_triggered();
    void filterLineEdit_textChanged(const QString &arg1);
    void collectLocalStatisticsSample(QPointF point);
    void updateStatisticsButton();
    void updateFinderButton();
    void fovCenterChangedReceiver(QPointF newCenter);
    void on_actionFinder_triggered();
    void WCSupdatedReceived();

public slots:
    void autoContrastPushButton_toggled_receiver(bool checked);
    void clearAll();
    void colorFactorChanged_receiver(QString redFactor, QString blueFactor);
    void loadFITSexternal(QString fileName, QString filter);
    void loadFromRAM(MyImage *it, int level);
    void loadFromRAMlist(const QModelIndex &index);
    void loadFITSexternalRAM(int index);
    void mapFITS();
    void minmaxLineEdit_returnPressed_receiver(QString rangeMin, QString rangeMax);
    void showG2References(bool checked);
    void showAbsPhotReferences(bool checked);
    void solutionAcceptanceStateReceived(bool state);
    void startAction_triggered();
    void updateColorViewInternal(float redFactor, float blueFactor);
    void updateColorViewExternal(float redFactor, float blueFactor);
    void zoomFitPushButton_clicked_receiver(bool checked);
    void zoomInPushButton_clicked_receiver();
    void zoomOutPushButton_clicked_receiver();
    void zoomZeroPushButton_clicked_receiver();
    void viewportChangedReceived(QRect viewport_rect);
    void targetResolvedReceived(QString alphaStr, QString deltaStr);

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
    double crpix1_start = 0.;      // set when the middle mouse button is pressed in wcs mode
    double crpix2_start = 0.;      // set when the middle mouse button is pressed in wcs mode
    double plateScale = 0.;
    double skyRa = 0.;
    double skyDec = 0.;
    bool startLeftClickInsideItem = false;
    int timerId;
    QString ChannelR;
    QString ChannelG;
    QString ChannelB;
    bool allChannelsRead = false;
    unsigned char *dataInt;
    unsigned char *dataIntR;
    unsigned char *dataIntG;
    unsigned char *dataIntB;
    unsigned char *dataBinnedInt;
    unsigned char *dataBinnedIntR;
    unsigned char *dataBinnedIntG;
    unsigned char *dataBinnedIntB;
    QGraphicsPixmapItem *pixmapItem = nullptr;
    QGraphicsPixmapItem *magnifiedPixmapItem = nullptr;
    QGraphicsPixmapItem *binnedPixmapItem = nullptr;
    bool dataBinnedIntSet = false;
    bool dataBinnedIntRSet = false;
    bool dataBinnedIntGSet = false;
    bool dataBinnedIntBSet = false;
    bool dataIntSet = false;
    bool dataIntRSet = false;
    bool dataIntGSet = false;
    bool dataIntBSet = false;
    int magnify = 7;

    double rad = 3.1415926535 / 180.;
    bool fromMemory = false;

    struct wcsprm *wcs;
    bool wcsInit = false;
    char *fullheader = nullptr;

    QTimer *timer = new QTimer(this);

    QList<QGraphicsLineItem*> vectorLineItems;
    QList<QGraphicsTextItem*> vectorTextItems;
    QList<QGraphicsEllipseItem*> sourceCatItems;
    QList<QGraphicsRectItem*> refCatItems;
    QList<QGraphicsRectItem*> G2refCatItems;
    QList<QGraphicsRectItem*> AbsPhotRefCatItems;
    QList<QGraphicsRectItem*> skyRectItems;
    QList<QGraphicsTextItem*> skyTextItems;
    QList<QGraphicsPolygonItem*> maskPolygonItems;
    QList<QGraphicsRectItem*> maskRectItems;

    QLabel *speedLabel = new QLabel(this);
    QSpinBox *speedSpinBox = new QSpinBox(this);
    QLabel *filterLabel = new QLabel(this);
    QLineEdit *filterLineEdit = new QLineEdit(this);

    bool icdwDefined = false;
    bool scampdwDefined = false;

    bool binnedPixmapUptodate = false;

    void addDockWidgets();
    QRect adjustGeometry();
    void clearSkyCircleItems();
    void clearSkyRectItems();
    void clearMaskPolygonItems();
    void clearMaskRectItems();
    void clearVectorItems();
    void compressDynrange(const QVector<float> &fitsdata, unsigned char *intdata, float colorCorrectionFactor = 1.0);
    QString dec2hex(double angle);
    void dumpSkyCircleCoordinates();
    void getGlobalImageStatistics(QString colorMode = "");
    QString getVectorLabel(double separation);
    void getVectorOffsets(const qreal dx, const qreal dy, qreal &x_yoffset, qreal &y_xoffset, qreal &d_xoffset, qreal &d_yoffset);
    double haversine(double x1, double y1, double x2, double y2);
    void hideWCSdockWidget();
    void imageListToChipName();
    void initGUI();
    void initGUIstep2();
    void loadColorFITS(qreal scaleFactor);
    void loadFITS(QString filename, int currentId = 0, qreal scaleFactor = 1.0);
    bool loadFITSdata(QString filename, QVector<float> &data, QString colorMode = "");
    void makeConnections();
    void measureAngularSeparations(QPointF pointStart, QPointF pointEnd, double &sepX, double &sepY, double &sepD);
    void readPreferenceSettings();
    bool readRaDecCatalog(QString fileName, QList<QGraphicsRectItem *> &items, double size, int width, QColor color);
    void replotCatalogAfterZoom();
    void setCurrentId(QString filename);
    void setImageListFromMemory();
    void showWCSdockWidget();
    void sky2xyQImage(double ra, double dec, double &x, double &y);
    void sky2xyFITS(double ra, double dec, double &x, double &y);
    void writePreferenceSettings();

    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }
    void sendWCStoWCSdockWidget();
    void checkFinder();
    void checkFinderBypass();
};

#endif // IVIEW_H
