/********************************************************************************
** Form generated from reading UI file 'imagestatistics.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGESTATISTICS_H
#define UI_IMAGESTATISTICS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_ImageStatistics
{
public:
    QAction *actionClose;
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QCustomPlot *statPlot;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout_6;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *readmePushButton;
    QSpacerItem *verticalSpacer;
    QPushButton *showPushButton;
    QLabel *label_10;
    QFrame *frame_2;
    QGridLayout *gridLayout_3;
    QLineEdit *airmassMaxLineEdit;
    QLabel *label_9;
    QLineEdit *seeingMinLineEdit;
    QLineEdit *seeingMaxLineEdit;
    QLineEdit *rzpMinLineEdit;
    QLineEdit *airmassMinLineEdit;
    QLineEdit *skyMinLineEdit;
    QLineEdit *skyMaxLineEdit;
    QLabel *label_11;
    QLineEdit *ellMinLineEdit;
    QLabel *label_8;
    QLabel *label_12;
    QLineEdit *rzpMaxLineEdit;
    QLabel *label_13;
    QLineEdit *ellMaxLineEdit;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_14;
    QLineEdit *imageMinLineEdit;
    QLineEdit *imageMaxLineEdit;
    QLabel *label_17;
    QFrame *frame_3;
    QGridLayout *gridLayout_5;
    QLabel *label_15;
    QComboBox *fwhmunitsComboBox;
    QGridLayout *gridLayout_4;
    QPushButton *clearSelectionPushButton;
    QPushButton *invertSelectionPushButton;
    QPushButton *ClearPlotPushButton;
    QPushButton *exportPushButton;
    QPushButton *restoreDataPushButton;
    QCheckBox *connectDataPointsCheckBox;
    QSpacerItem *verticalSpacer_4;
    QFrame *frame;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *decLineEdit;
    QLineEdit *raLineEdit;
    QLabel *label_16;
    QLabel *label;
    QComboBox *scienceComboBox;
    QPushButton *statisticsPushButton;
    QLineEdit *chipsLineEdit;

    void setupUi(QMainWindow *ImageStatistics)
    {
        if (ImageStatistics->objectName().isEmpty())
            ImageStatistics->setObjectName(QStringLiteral("ImageStatistics"));
        ImageStatistics->resize(1290, 832);
        actionClose = new QAction(ImageStatistics);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        centralwidget = new QWidget(ImageStatistics);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        statPlot = new QCustomPlot(centralwidget);
        statPlot->setObjectName(QStringLiteral("statPlot"));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(170, 170, 171, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(212, 212, 213, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(85, 85, 85, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(113, 113, 114, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        QBrush brush6(QColor(255, 255, 220, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        statPlot->setPalette(palette);
        statPlot->setFocusPolicy(Qt::StrongFocus);
        statPlot->setAutoFillBackground(true);

        gridLayout_2->addWidget(statPlot, 0, 0, 1, 3);

        ImageStatistics->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ImageStatistics);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1290, 23));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        ImageStatistics->setMenuBar(menubar);
        statusbar = new QStatusBar(ImageStatistics);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        ImageStatistics->setStatusBar(statusbar);
        dockWidget = new QDockWidget(ImageStatistics);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        QPalette palette1;
        QBrush brush7(QColor(252, 252, 252, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush7);
        QBrush brush8(QColor(220, 220, 221, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush8);
        dockWidget->setPalette(palette1);
        dockWidget->setAutoFillBackground(true);
        dockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        gridLayout_6 = new QGridLayout(dockWidgetContents);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        label_5 = new QLabel(dockWidgetContents);
        label_5->setObjectName(QStringLiteral("label_5"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_5->setFont(font);

        gridLayout_6->addWidget(label_5, 0, 0, 1, 2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_2, 0, 2, 1, 1);

        readmePushButton = new QPushButton(dockWidgetContents);
        readmePushButton->setObjectName(QStringLiteral("readmePushButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(readmePushButton->sizePolicy().hasHeightForWidth());
        readmePushButton->setSizePolicy(sizePolicy);
        readmePushButton->setMinimumSize(QSize(30, 0));
        readmePushButton->setMaximumSize(QSize(30, 16777215));
        QPalette palette2;
        QBrush brush9(QColor(85, 170, 255, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush9);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush9);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush9);
        readmePushButton->setPalette(palette2);
        readmePushButton->setFont(font);
        readmePushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_6->addWidget(readmePushButton, 0, 3, 1, 1);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_6->addItem(verticalSpacer, 2, 1, 1, 2);

        showPushButton = new QPushButton(dockWidgetContents);
        showPushButton->setObjectName(QStringLiteral("showPushButton"));
        showPushButton->setFocusPolicy(Qt::ClickFocus);
        showPushButton->setCheckable(true);

        gridLayout_6->addWidget(showPushButton, 3, 0, 1, 3);

        label_10 = new QLabel(dockWidgetContents);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setFont(font);

        gridLayout_6->addWidget(label_10, 4, 0, 1, 1);

        frame_2 = new QFrame(dockWidgetContents);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::Base, brush7);
        QBrush brush10(QColor(198, 198, 199, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush10);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush10);
        frame_2->setPalette(palette3);
        frame_2->setAutoFillBackground(true);
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame_2);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        airmassMaxLineEdit = new QLineEdit(frame_2);
        airmassMaxLineEdit->setObjectName(QStringLiteral("airmassMaxLineEdit"));
        airmassMaxLineEdit->setMaximumSize(QSize(70, 16777215));
        airmassMaxLineEdit->setFocusPolicy(Qt::StrongFocus);
        airmassMaxLineEdit->setClearButtonEnabled(false);

        gridLayout_3->addWidget(airmassMaxLineEdit, 2, 2, 1, 1);

        label_9 = new QLabel(frame_2);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_3->addWidget(label_9, 3, 0, 1, 1);

        seeingMinLineEdit = new QLineEdit(frame_2);
        seeingMinLineEdit->setObjectName(QStringLiteral("seeingMinLineEdit"));
        seeingMinLineEdit->setMaximumSize(QSize(70, 16777215));
        seeingMinLineEdit->setFocusPolicy(Qt::StrongFocus);
        seeingMinLineEdit->setClearButtonEnabled(false);

        gridLayout_3->addWidget(seeingMinLineEdit, 3, 1, 1, 1);

        seeingMaxLineEdit = new QLineEdit(frame_2);
        seeingMaxLineEdit->setObjectName(QStringLiteral("seeingMaxLineEdit"));
        seeingMaxLineEdit->setMaximumSize(QSize(70, 16777215));
        seeingMaxLineEdit->setFocusPolicy(Qt::StrongFocus);
        seeingMaxLineEdit->setClearButtonEnabled(false);

        gridLayout_3->addWidget(seeingMaxLineEdit, 3, 2, 1, 1);

        rzpMinLineEdit = new QLineEdit(frame_2);
        rzpMinLineEdit->setObjectName(QStringLiteral("rzpMinLineEdit"));
        rzpMinLineEdit->setMaximumSize(QSize(70, 16777215));
        rzpMinLineEdit->setFocusPolicy(Qt::StrongFocus);
        rzpMinLineEdit->setClearButtonEnabled(false);

        gridLayout_3->addWidget(rzpMinLineEdit, 4, 1, 1, 1);

        airmassMinLineEdit = new QLineEdit(frame_2);
        airmassMinLineEdit->setObjectName(QStringLiteral("airmassMinLineEdit"));
        airmassMinLineEdit->setMaximumSize(QSize(70, 16777215));
        airmassMinLineEdit->setFocusPolicy(Qt::StrongFocus);
        airmassMinLineEdit->setClearButtonEnabled(false);

        gridLayout_3->addWidget(airmassMinLineEdit, 2, 1, 1, 1);

        skyMinLineEdit = new QLineEdit(frame_2);
        skyMinLineEdit->setObjectName(QStringLiteral("skyMinLineEdit"));
        skyMinLineEdit->setMaximumSize(QSize(70, 16777215));
        skyMinLineEdit->setFocusPolicy(Qt::StrongFocus);
        skyMinLineEdit->setClearButtonEnabled(false);

        gridLayout_3->addWidget(skyMinLineEdit, 1, 1, 1, 1);

        skyMaxLineEdit = new QLineEdit(frame_2);
        skyMaxLineEdit->setObjectName(QStringLiteral("skyMaxLineEdit"));
        skyMaxLineEdit->setMaximumSize(QSize(70, 16777215));
        skyMaxLineEdit->setFocusPolicy(Qt::StrongFocus);
        skyMaxLineEdit->setClearButtonEnabled(false);

        gridLayout_3->addWidget(skyMaxLineEdit, 1, 2, 1, 1);

        label_11 = new QLabel(frame_2);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_3->addWidget(label_11, 2, 0, 1, 1);

        ellMinLineEdit = new QLineEdit(frame_2);
        ellMinLineEdit->setObjectName(QStringLiteral("ellMinLineEdit"));
        ellMinLineEdit->setMaximumSize(QSize(70, 16777215));
        ellMinLineEdit->setFocusPolicy(Qt::StrongFocus);
        ellMinLineEdit->setClearButtonEnabled(false);

        gridLayout_3->addWidget(ellMinLineEdit, 5, 1, 1, 1);

        label_8 = new QLabel(frame_2);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_3->addWidget(label_8, 1, 0, 1, 1);

        label_12 = new QLabel(frame_2);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_3->addWidget(label_12, 4, 0, 1, 1);

        rzpMaxLineEdit = new QLineEdit(frame_2);
        rzpMaxLineEdit->setObjectName(QStringLiteral("rzpMaxLineEdit"));
        rzpMaxLineEdit->setMaximumSize(QSize(70, 16777215));
        rzpMaxLineEdit->setFocusPolicy(Qt::StrongFocus);
        rzpMaxLineEdit->setClearButtonEnabled(false);

        gridLayout_3->addWidget(rzpMaxLineEdit, 4, 2, 1, 1);

        label_13 = new QLabel(frame_2);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_3->addWidget(label_13, 5, 0, 1, 1);

        ellMaxLineEdit = new QLineEdit(frame_2);
        ellMaxLineEdit->setObjectName(QStringLiteral("ellMaxLineEdit"));
        ellMaxLineEdit->setMaximumSize(QSize(70, 16777215));
        ellMaxLineEdit->setFocusPolicy(Qt::StrongFocus);
        ellMaxLineEdit->setClearButtonEnabled(false);

        gridLayout_3->addWidget(ellMaxLineEdit, 5, 2, 1, 1);

        label_6 = new QLabel(frame_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_3->addWidget(label_6, 0, 1, 1, 1);

        label_7 = new QLabel(frame_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_3->addWidget(label_7, 0, 2, 1, 1);

        label_14 = new QLabel(frame_2);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_3->addWidget(label_14, 6, 0, 1, 1);

        imageMinLineEdit = new QLineEdit(frame_2);
        imageMinLineEdit->setObjectName(QStringLiteral("imageMinLineEdit"));
        imageMinLineEdit->setMaximumSize(QSize(70, 16777215));
        imageMinLineEdit->setFocusPolicy(Qt::StrongFocus);
        imageMinLineEdit->setClearButtonEnabled(false);

        gridLayout_3->addWidget(imageMinLineEdit, 6, 1, 1, 1);

        imageMaxLineEdit = new QLineEdit(frame_2);
        imageMaxLineEdit->setObjectName(QStringLiteral("imageMaxLineEdit"));
        imageMaxLineEdit->setMaximumSize(QSize(70, 16777215));
        imageMaxLineEdit->setFocusPolicy(Qt::StrongFocus);
        imageMaxLineEdit->setClearButtonEnabled(false);

        gridLayout_3->addWidget(imageMaxLineEdit, 6, 2, 1, 1);


        gridLayout_6->addWidget(frame_2, 5, 0, 1, 4);

        label_17 = new QLabel(dockWidgetContents);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setFont(font);

        gridLayout_6->addWidget(label_17, 6, 0, 1, 1);

        frame_3 = new QFrame(dockWidgetContents);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::Base, brush7);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush10);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush7);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush10);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush10);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush10);
        frame_3->setPalette(palette4);
        frame_3->setAutoFillBackground(true);
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        gridLayout_5 = new QGridLayout(frame_3);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        label_15 = new QLabel(frame_3);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout_5->addWidget(label_15, 1, 0, 1, 1);

        fwhmunitsComboBox = new QComboBox(frame_3);
        fwhmunitsComboBox->setObjectName(QStringLiteral("fwhmunitsComboBox"));

        gridLayout_5->addWidget(fwhmunitsComboBox, 1, 1, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        clearSelectionPushButton = new QPushButton(frame_3);
        clearSelectionPushButton->setObjectName(QStringLiteral("clearSelectionPushButton"));
        clearSelectionPushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_4->addWidget(clearSelectionPushButton, 2, 0, 1, 1);

        invertSelectionPushButton = new QPushButton(frame_3);
        invertSelectionPushButton->setObjectName(QStringLiteral("invertSelectionPushButton"));
        invertSelectionPushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_4->addWidget(invertSelectionPushButton, 2, 1, 1, 1);

        ClearPlotPushButton = new QPushButton(frame_3);
        ClearPlotPushButton->setObjectName(QStringLiteral("ClearPlotPushButton"));
        ClearPlotPushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_4->addWidget(ClearPlotPushButton, 3, 0, 1, 1);

        exportPushButton = new QPushButton(frame_3);
        exportPushButton->setObjectName(QStringLiteral("exportPushButton"));
        exportPushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_4->addWidget(exportPushButton, 3, 1, 1, 1);

        restoreDataPushButton = new QPushButton(frame_3);
        restoreDataPushButton->setObjectName(QStringLiteral("restoreDataPushButton"));
        restoreDataPushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_4->addWidget(restoreDataPushButton, 1, 0, 1, 1);


        gridLayout_5->addLayout(gridLayout_4, 2, 0, 1, 2);

        connectDataPointsCheckBox = new QCheckBox(frame_3);
        connectDataPointsCheckBox->setObjectName(QStringLiteral("connectDataPointsCheckBox"));
        connectDataPointsCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_5->addWidget(connectDataPointsCheckBox, 0, 0, 1, 2);


        gridLayout_6->addWidget(frame_3, 7, 0, 1, 4);

        verticalSpacer_4 = new QSpacerItem(20, 254, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer_4, 8, 0, 1, 1);

        frame = new QFrame(dockWidgetContents);
        frame->setObjectName(QStringLiteral("frame"));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::Base, brush7);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush10);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush7);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush10);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush10);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush10);
        frame->setPalette(palette5);
        frame->setAutoFillBackground(true);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 5, 0, 1, 3);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 6, 0, 1, 1);

        label_4 = new QLabel(frame);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 7, 0, 1, 1);

        decLineEdit = new QLineEdit(frame);
        decLineEdit->setObjectName(QStringLiteral("decLineEdit"));
        decLineEdit->setFocusPolicy(Qt::StrongFocus);
        decLineEdit->setClearButtonEnabled(false);

        gridLayout->addWidget(decLineEdit, 7, 1, 1, 2);

        raLineEdit = new QLineEdit(frame);
        raLineEdit->setObjectName(QStringLiteral("raLineEdit"));
        raLineEdit->setFocusPolicy(Qt::StrongFocus);
        raLineEdit->setClearButtonEnabled(false);

        gridLayout->addWidget(raLineEdit, 6, 1, 1, 2);

        label_16 = new QLabel(frame);
        label_16->setObjectName(QStringLiteral("label_16"));

        gridLayout->addWidget(label_16, 0, 0, 1, 1);

        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        scienceComboBox = new QComboBox(frame);
        scienceComboBox->setObjectName(QStringLiteral("scienceComboBox"));

        gridLayout->addWidget(scienceComboBox, 0, 1, 1, 2);

        statisticsPushButton = new QPushButton(frame);
        statisticsPushButton->setObjectName(QStringLiteral("statisticsPushButton"));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush11(QColor(68, 216, 204, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette6.setBrush(QPalette::Active, QPalette::Button, brush11);
        QBrush brush12(QColor(149, 255, 247, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette6.setBrush(QPalette::Active, QPalette::Light, brush12);
        QBrush brush13(QColor(108, 235, 225, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette6.setBrush(QPalette::Active, QPalette::Midlight, brush13);
        QBrush brush14(QColor(34, 108, 102, 255));
        brush14.setStyle(Qt::SolidPattern);
        palette6.setBrush(QPalette::Active, QPalette::Dark, brush14);
        QBrush brush15(QColor(45, 144, 136, 255));
        brush15.setStyle(Qt::SolidPattern);
        palette6.setBrush(QPalette::Active, QPalette::Mid, brush15);
        palette6.setBrush(QPalette::Active, QPalette::Text, brush);
        palette6.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette6.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette6.setBrush(QPalette::Active, QPalette::Window, brush11);
        palette6.setBrush(QPalette::Active, QPalette::Shadow, brush);
        QBrush brush16(QColor(161, 235, 229, 255));
        brush16.setStyle(Qt::SolidPattern);
        palette6.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
        palette6.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette6.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette6.setBrush(QPalette::Inactive, QPalette::Light, brush12);
        palette6.setBrush(QPalette::Inactive, QPalette::Midlight, brush13);
        palette6.setBrush(QPalette::Inactive, QPalette::Dark, brush14);
        palette6.setBrush(QPalette::Inactive, QPalette::Mid, brush15);
        palette6.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::Window, brush11);
        palette6.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush16);
        palette6.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette6.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush14);
        palette6.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette6.setBrush(QPalette::Disabled, QPalette::Light, brush12);
        palette6.setBrush(QPalette::Disabled, QPalette::Midlight, brush13);
        palette6.setBrush(QPalette::Disabled, QPalette::Dark, brush14);
        palette6.setBrush(QPalette::Disabled, QPalette::Mid, brush15);
        palette6.setBrush(QPalette::Disabled, QPalette::Text, brush14);
        palette6.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::ButtonText, brush14);
        palette6.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette6.setBrush(QPalette::Disabled, QPalette::Window, brush11);
        palette6.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush11);
        palette6.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette6.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        statisticsPushButton->setPalette(palette6);
        statisticsPushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(statisticsPushButton, 8, 1, 1, 2);

        chipsLineEdit = new QLineEdit(frame);
        chipsLineEdit->setObjectName(QStringLiteral("chipsLineEdit"));
        chipsLineEdit->setFocusPolicy(Qt::StrongFocus);
        chipsLineEdit->setClearButtonEnabled(false);

        gridLayout->addWidget(chipsLineEdit, 1, 1, 1, 2);


        gridLayout_6->addWidget(frame, 1, 0, 1, 4);

        dockWidget->setWidget(dockWidgetContents);
        ImageStatistics->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);
        QWidget::setTabOrder(raLineEdit, decLineEdit);
        QWidget::setTabOrder(decLineEdit, skyMinLineEdit);
        QWidget::setTabOrder(skyMinLineEdit, skyMaxLineEdit);
        QWidget::setTabOrder(skyMaxLineEdit, airmassMinLineEdit);
        QWidget::setTabOrder(airmassMinLineEdit, airmassMaxLineEdit);
        QWidget::setTabOrder(airmassMaxLineEdit, seeingMinLineEdit);
        QWidget::setTabOrder(seeingMinLineEdit, seeingMaxLineEdit);
        QWidget::setTabOrder(seeingMaxLineEdit, rzpMinLineEdit);
        QWidget::setTabOrder(rzpMinLineEdit, rzpMaxLineEdit);
        QWidget::setTabOrder(rzpMaxLineEdit, ellMinLineEdit);
        QWidget::setTabOrder(ellMinLineEdit, ellMaxLineEdit);
        QWidget::setTabOrder(ellMaxLineEdit, imageMinLineEdit);
        QWidget::setTabOrder(imageMinLineEdit, imageMaxLineEdit);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionClose);

        retranslateUi(ImageStatistics);

        QMetaObject::connectSlotsByName(ImageStatistics);
    } // setupUi

    void retranslateUi(QMainWindow *ImageStatistics)
    {
        ImageStatistics->setWindowTitle(QApplication::translate("ImageStatistics", "Image statistics", Q_NULLPTR));
        actionClose->setText(QApplication::translate("ImageStatistics", "Close", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        statPlot->setStatusTip(QApplication::translate("ImageStatistics", "Click to select / deselect data points. Right-click to select multiple points. Click-drag to shift plot range. Scroll on plot or axes to zoom.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        menuFile->setTitle(QApplication::translate("ImageStatistics", "File", Q_NULLPTR));
        dockWidget->setWindowTitle(QApplication::translate("ImageStatistics", "Configuration", Q_NULLPTR));
        label_5->setText(QApplication::translate("ImageStatistics", "Image selection", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        readmePushButton->setStatusTip(QApplication::translate("ImageStatistics", "Displays a dialog explaining the various mouse and keyboard interactions with the data.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        readmePushButton->setText(QApplication::translate("ImageStatistics", "?", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        showPushButton->setStatusTip(QApplication::translate("ImageStatistics", "Shows the image associated with a selected data point.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        showPushButton->setText(QApplication::translate("ImageStatistics", "Display selected image", Q_NULLPTR));
        label_10->setText(QApplication::translate("ImageStatistics", "Cut-offs", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        airmassMaxLineEdit->setStatusTip(QApplication::translate("ImageStatistics", "Maximum airmass allowed", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_9->setText(QApplication::translate("ImageStatistics", "Seeing", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        seeingMinLineEdit->setStatusTip(QApplication::translate("ImageStatistics", "Minimum seeing allowed (usually unconstrained as lower seeing is better)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        seeingMaxLineEdit->setStatusTip(QApplication::translate("ImageStatistics", "Maximum seeing allowed", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        rzpMinLineEdit->setStatusTip(QApplication::translate("ImageStatistics", "Minimum relative zeropoint allowed", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        airmassMinLineEdit->setStatusTip(QApplication::translate("ImageStatistics", "Minimum airmass allowed (usually unconstrained as lower airmass is better)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        skyMinLineEdit->setStatusTip(QApplication::translate("ImageStatistics", "Minimum sky background allowed", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        skyMaxLineEdit->setStatusTip(QApplication::translate("ImageStatistics", "Maximum sky background allowed", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_11->setText(QApplication::translate("ImageStatistics", "Airmass", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ellMinLineEdit->setStatusTip(QApplication::translate("ImageStatistics", "Minimum PSF ellipticity allowed  (usually unconstrained as lower ellipticity is better)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_8->setText(QApplication::translate("ImageStatistics", "Sky", Q_NULLPTR));
        label_12->setText(QApplication::translate("ImageStatistics", "RZP", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        rzpMaxLineEdit->setStatusTip(QApplication::translate("ImageStatistics", "Maximum relative zeropoint allowed (usually unconstrained as higher zeropoints mean better transparency)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_13->setText(QApplication::translate("ImageStatistics", "Ellip.", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ellMaxLineEdit->setStatusTip(QApplication::translate("ImageStatistics", "Maximum PSF ellipticity allowed", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_6->setText(QApplication::translate("ImageStatistics", "min", Q_NULLPTR));
        label_7->setText(QApplication::translate("ImageStatistics", "max", Q_NULLPTR));
        label_14->setText(QApplication::translate("ImageStatistics", "Image #", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        imageMinLineEdit->setStatusTip(QApplication::translate("ImageStatistics", "The lowest image number in the consecutive series (constraint on the x-axis)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        imageMaxLineEdit->setStatusTip(QApplication::translate("ImageStatistics", "The highest image number in the consecutive series (constraint on the x-axis)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_17->setText(QApplication::translate("ImageStatistics", "Settings", Q_NULLPTR));
        label_15->setText(QApplication::translate("ImageStatistics", "FWHM units", Q_NULLPTR));
        fwhmunitsComboBox->clear();
        fwhmunitsComboBox->insertItems(0, QStringList()
         << QApplication::translate("ImageStatistics", "arcsec", Q_NULLPTR)
         << QApplication::translate("ImageStatistics", "pixel", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        fwhmunitsComboBox->setStatusTip(QApplication::translate("ImageStatistics", "The units used for the seeing plot", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        clearSelectionPushButton->setStatusTip(QApplication::translate("ImageStatistics", "Clears a selection of data points.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        clearSelectionPushButton->setText(QApplication::translate("ImageStatistics", "Clear selection", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        invertSelectionPushButton->setStatusTip(QApplication::translate("ImageStatistics", "Invert the selection", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        invertSelectionPushButton->setText(QApplication::translate("ImageStatistics", "Invert selection", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ClearPlotPushButton->setStatusTip(QApplication::translate("ImageStatistics", "Clears and resets the plot panels to an empty state.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ClearPlotPushButton->setText(QApplication::translate("ImageStatistics", "Clear plot", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        exportPushButton->setStatusTip(QApplication::translate("ImageStatistics", "Exports the graphics to PNG format.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        exportPushButton->setText(QApplication::translate("ImageStatistics", "Export plot", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        restoreDataPushButton->setStatusTip(QApplication::translate("ImageStatistics", "Restores manually removed data points. Also puts the corresponding bad images back into their original place.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        restoreDataPushButton->setText(QApplication::translate("ImageStatistics", "Restore data", Q_NULLPTR));
        connectDataPointsCheckBox->setText(QApplication::translate("ImageStatistics", "Connect data points", Q_NULLPTR));
        label_2->setText(QApplication::translate("ImageStatistics", "Image shall contain:", Q_NULLPTR));
        label_3->setText(QApplication::translate("ImageStatistics", "R.A.", Q_NULLPTR));
        label_4->setText(QApplication::translate("ImageStatistics", "DEC", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        decLineEdit->setStatusTip(QApplication::translate("ImageStatistics", "Optional. The DEC coordinate that must be contained in the selection. Decimal or DD:MM:SS format. This might take a while to execute.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        raLineEdit->setStatusTip(QApplication::translate("ImageStatistics", "Optional. The R.A. coordinate that must be contained in the selection. Decimal or HH:MM:SS format. This might take a while to execute.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_16->setText(QApplication::translate("ImageStatistics", "Data:", Q_NULLPTR));
        label->setText(QApplication::translate("ImageStatistics", "Chips", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        scienceComboBox->setStatusTip(QApplication::translate("ImageStatistics", "Select for which SCIENCE directory the statistics should be loaded", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        statisticsPushButton->setStatusTip(QApplication::translate("ImageStatistics", "Obtain the statistics measurements", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        statisticsPushButton->setText(QApplication::translate("ImageStatistics", "Update plots", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        chipsLineEdit->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        chipsLineEdit->setStatusTip(QApplication::translate("ImageStatistics", "Blank- or comma-separated list of detector numbers to be shown", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        chipsLineEdit->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ImageStatistics: public Ui_ImageStatistics {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGESTATISTICS_H
