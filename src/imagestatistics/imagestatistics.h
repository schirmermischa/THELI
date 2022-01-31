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

#ifndef IMAGESTATISTICS_H
#define IMAGESTATISTICS_H

#include "../readmes/imstatsreadme.h"
#include "../iview/iview.h"
#include "../qcustomplot.h"
#include "../instrumentdata.h"
#include "../processingStatus/processingStatus.h"

#include <QMainWindow>
#include <myimage/myimage.h>
#include "../processingInternal/data.h"

#include <QVector>

namespace Ui {
class ImageStatistics;
}

class ImageStatistics : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor passing a list of MyImages
//    explicit ImageStatistics(QVector<QList<MyImage *> > &imlist, const QString main, const QString sciencedir,
//                             const instrumentDataType *instrumentData, QWidget *parent);
explicit ImageStatistics(QList<Data *> &datalist, const QString main, const QString sciencedir,
                         const instrumentDataType *instrumentData, QWidget *parent);
    ~ImageStatistics();

    void keyReleaseEvent(QKeyEvent *event);

    bool iViewOpen = false;

signals:
    void imageSelected(int index);

public slots:
    void currentlyDisplayedIndexReceived(int currentId);

private slots:
    void dataPointClicked(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event);
    void numericSelection();
    void on_statisticsPushButton_clicked();
    void on_exportPushButton_clicked();
    void on_showPushButton_clicked();
    void on_ClearPlotPushButton_clicked();
    void on_restoreDataPushButton_clicked();
    void on_clearSelectionPushButton_clicked();
    void on_invertSelectionPushButton_clicked();
    void on_readmePushButton_clicked();
    void on_connectDataPointsCheckBox_clicked();
    void uncheckIviewPushButton();
    void validate();
//  void currentlyDisplayedIndexReceived(int currentId);
    void on_fwhmunitsComboBox_currentIndexChanged(const QString &arg1);
    void on_actionClose_triggered();
    void on_scienceComboBox_activated(const QString &arg1);

private:
    QString mainDir;
    QString statusString;
    ProcessingStatus *processingStatus;
    const instrumentDataType *instData;
    QList<Data*> dataList;
    Ui::ImageStatistics *ui;
    QString thelidir;
    QString userdir;
    QVector<QList<MyImage*>> myExposureList;
    QList<MyImage*> allMyImages;
    QStringList dataName;
    QStringList badStatsList;
    QString scienceDirName;
    QDir scienceDir;
    QVector<double> dataImageNr = QVector<double>();
    QVector<double> dataSky = QVector<double>();
    QVector<double> dataAirmass = QVector<double>();
    QVector<double> dataFWHM = QVector<double>();
    QVector<double> dataRZP = QVector<double>();
    QVector<double> dataEllipticity = QVector<double>();
    bool skyData = true;
    bool seeingData = true;
    bool seeingFromGaia = false;            // A flag whether the seeing was estimated from medians of all sources, or from a match with GAIA point sources
    bool airmassData = true;
    bool rzpData = true;
    bool ellipticityData = true;
    bool imgSelected = false;
    bool statisticsDataDisplayed = false;
    QString imgSelectedName = "";
    int numObj = 0;
    int lastDataPointClicked = -1;
    enum Qt::MouseButton lastButtonClicked = Qt::LeftButton;
    enum Qt::Key lastKeyPressed = Qt::Key_Left;
    bool initKeySelection = false;
    QString directionSwitched = "noSwitch";

    IView *iView;
    QCPGraph *skyGraph;
    QCPGraph *seeingGraph;
    QCPGraph *airmassGraph;
    QCPGraph *rzpGraph;
    QCPGraph *ellipticityGraph;
    QCPDataSelection selection;          // data points selected by mouse clicks or key presses
    QCPDataSelection iviewSelection;    // data point selected for display in iView, marked with a special symbol
    QCPDataSelection numSelection;       // data points selected by manually entered numeric thresholds
    QList<QCPGraph*> graphList;
    QStringList filteredImageList;
    QList<QLineEdit*> numericThresholdList;
    ImstatsReadme *imstatsReadme;
    QCPGraph::LineStyle myLineStyle;

    void clearAll();
    void clearData();
    void clearSelection();
    void makeConnections();
    void plot();
    void plotSelection(int index);
    void readStatisticsData();
//    void showNoData(QCPGraph *graph, float xpos, float ypos);
    void makeListOfBadImages();
    Data *getData(QList<Data *> DT_x, QString dirName);
    void init();
    bool isImageSelected(MyImage *myImage, const QString &ra, const QString &dec, const QVector<int> &chipID);
    void activateImages();
};

#endif // IMAGESTATISTICS_H
