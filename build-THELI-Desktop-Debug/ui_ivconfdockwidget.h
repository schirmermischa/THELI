/********************************************************************************
** Form generated from reading UI file 'ivconfdockwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IVCONFDOCKWIDGET_H
#define UI_IVCONFDOCKWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IvConfDockWidget
{
public:
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_4;
    QPushButton *zoomOutPushButton;
    QPushButton *zoomInPushButton;
    QPushButton *zoomZeroPushButton;
    QPushButton *zoomFitPushButton;
    QLabel *filterLabel;
    QLineEdit *filterLineEdit;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer;
    QFrame *indicatorFrame;
    QGridLayout *gridLayout_2;
    QLabel *yposLabel;
    QLabel *valueLabel;
    QLabel *valueGreenLabel;
    QLabel *valueBlueLabel;
    QLabel *zoomLabel;
    QLabel *alphaHexLabel;
    QLabel *deltaHexLabel;
    QLabel *rmsLabel;
    QLabel *xposLabel;
    QLabel *medianLabel;
    QLabel *alphaDecLabel;
    QLabel *deltaDecLabel;
    QFormLayout *formLayout;
    QLabel *minLabel;
    QLineEdit *minLineEdit;
    QLabel *maxLabel;
    QLineEdit *maxLineEdit;
    QPushButton *autocontrastPushButton;
    QPushButton *quitPushButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDockWidget *IvConfDockWidget)
    {
        if (IvConfDockWidget->objectName().isEmpty())
            IvConfDockWidget->setObjectName(QStringLiteral("IvConfDockWidget"));
        IvConfDockWidget->resize(183, 649);
        IvConfDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        IvConfDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        gridLayout = new QGridLayout(dockWidgetContents);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(4);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        zoomOutPushButton = new QPushButton(dockWidgetContents);
        zoomOutPushButton->setObjectName(QStringLiteral("zoomOutPushButton"));
        zoomOutPushButton->setMinimumSize(QSize(60, 0));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(162, 255, 229, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(208, 255, 242, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(81, 127, 114, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(108, 170, 153, 255));
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
        zoomOutPushButton->setPalette(palette);
        zoomOutPushButton->setFocusPolicy(Qt::NoFocus);

        gridLayout_4->addWidget(zoomOutPushButton, 0, 0, 1, 1);

        zoomInPushButton = new QPushButton(dockWidgetContents);
        zoomInPushButton->setObjectName(QStringLiteral("zoomInPushButton"));
        zoomInPushButton->setMinimumSize(QSize(60, 0));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        zoomInPushButton->setPalette(palette1);
        zoomInPushButton->setFocusPolicy(Qt::NoFocus);

        gridLayout_4->addWidget(zoomInPushButton, 0, 1, 1, 1);

        zoomZeroPushButton = new QPushButton(dockWidgetContents);
        zoomZeroPushButton->setObjectName(QStringLiteral("zoomZeroPushButton"));
        zoomZeroPushButton->setMinimumSize(QSize(60, 0));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette2.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette2.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush);
        palette2.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette2.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette2.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        zoomZeroPushButton->setPalette(palette2);
        zoomZeroPushButton->setFocusPolicy(Qt::NoFocus);

        gridLayout_4->addWidget(zoomZeroPushButton, 1, 0, 1, 1);

        zoomFitPushButton = new QPushButton(dockWidgetContents);
        zoomFitPushButton->setObjectName(QStringLiteral("zoomFitPushButton"));
        zoomFitPushButton->setMinimumSize(QSize(60, 0));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette3.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette3.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette3.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush);
        palette3.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette3.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette3.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette3.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette3.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette3.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette3.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        zoomFitPushButton->setPalette(palette3);
        zoomFitPushButton->setFocusPolicy(Qt::NoFocus);
        zoomFitPushButton->setCheckable(true);

        gridLayout_4->addWidget(zoomFitPushButton, 1, 1, 1, 1);


        gridLayout->addLayout(gridLayout_4, 2, 0, 1, 2);

        filterLabel = new QLabel(dockWidgetContents);
        filterLabel->setObjectName(QStringLiteral("filterLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(filterLabel->sizePolicy().hasHeightForWidth());
        filterLabel->setSizePolicy(sizePolicy);

        gridLayout->addWidget(filterLabel, 5, 0, 1, 2);

        filterLineEdit = new QLineEdit(dockWidgetContents);
        filterLineEdit->setObjectName(QStringLiteral("filterLineEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(filterLineEdit->sizePolicy().hasHeightForWidth());
        filterLineEdit->setSizePolicy(sizePolicy1);
        filterLineEdit->setFocusPolicy(Qt::ClickFocus);
        filterLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(filterLineEdit, 6, 0, 1, 2);

        verticalSpacer_3 = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_3, 1, 0, 1, 2);

        verticalSpacer_2 = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_2, 3, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 39, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        gridLayout->addItem(verticalSpacer, 7, 0, 1, 1);

        indicatorFrame = new QFrame(dockWidgetContents);
        indicatorFrame->setObjectName(QStringLiteral("indicatorFrame"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(indicatorFrame->sizePolicy().hasHeightForWidth());
        indicatorFrame->setSizePolicy(sizePolicy2);
        indicatorFrame->setMinimumSize(QSize(150, 0));
        QPalette palette4;
        QBrush brush7(QColor(252, 252, 252, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush7);
        QBrush brush8(QColor(198, 198, 199, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush8);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush7);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush8);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush8);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush8);
        indicatorFrame->setPalette(palette4);
        QFont font;
        font.setPointSize(10);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        indicatorFrame->setFont(font);
        indicatorFrame->setAutoFillBackground(true);
        indicatorFrame->setFrameShape(QFrame::StyledPanel);
        indicatorFrame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(indicatorFrame);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setVerticalSpacing(0);
        yposLabel = new QLabel(indicatorFrame);
        yposLabel->setObjectName(QStringLiteral("yposLabel"));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setWeight(50);
        yposLabel->setFont(font1);

        gridLayout_2->addWidget(yposLabel, 1, 0, 1, 2);

        valueLabel = new QLabel(indicatorFrame);
        valueLabel->setObjectName(QStringLiteral("valueLabel"));
        valueLabel->setFont(font1);

        gridLayout_2->addWidget(valueLabel, 2, 0, 1, 2);

        valueGreenLabel = new QLabel(indicatorFrame);
        valueGreenLabel->setObjectName(QStringLiteral("valueGreenLabel"));
        sizePolicy.setHeightForWidth(valueGreenLabel->sizePolicy().hasHeightForWidth());
        valueGreenLabel->setSizePolicy(sizePolicy);
        valueGreenLabel->setFont(font1);

        gridLayout_2->addWidget(valueGreenLabel, 3, 0, 1, 2);

        valueBlueLabel = new QLabel(indicatorFrame);
        valueBlueLabel->setObjectName(QStringLiteral("valueBlueLabel"));
        valueBlueLabel->setFont(font1);

        gridLayout_2->addWidget(valueBlueLabel, 4, 0, 1, 2);

        zoomLabel = new QLabel(indicatorFrame);
        zoomLabel->setObjectName(QStringLiteral("zoomLabel"));
        sizePolicy.setHeightForWidth(zoomLabel->sizePolicy().hasHeightForWidth());
        zoomLabel->setSizePolicy(sizePolicy);
        zoomLabel->setFont(font1);

        gridLayout_2->addWidget(zoomLabel, 11, 0, 1, 2);

        alphaHexLabel = new QLabel(indicatorFrame);
        alphaHexLabel->setObjectName(QStringLiteral("alphaHexLabel"));
        alphaHexLabel->setFont(font1);

        gridLayout_2->addWidget(alphaHexLabel, 6, 0, 1, 2);

        deltaHexLabel = new QLabel(indicatorFrame);
        deltaHexLabel->setObjectName(QStringLiteral("deltaHexLabel"));
        deltaHexLabel->setFont(font1);

        gridLayout_2->addWidget(deltaHexLabel, 8, 0, 1, 2);

        rmsLabel = new QLabel(indicatorFrame);
        rmsLabel->setObjectName(QStringLiteral("rmsLabel"));
        rmsLabel->setFont(font1);

        gridLayout_2->addWidget(rmsLabel, 10, 0, 1, 2);

        xposLabel = new QLabel(indicatorFrame);
        xposLabel->setObjectName(QStringLiteral("xposLabel"));
        xposLabel->setFont(font1);

        gridLayout_2->addWidget(xposLabel, 0, 0, 1, 2);

        medianLabel = new QLabel(indicatorFrame);
        medianLabel->setObjectName(QStringLiteral("medianLabel"));
        medianLabel->setFont(font1);

        gridLayout_2->addWidget(medianLabel, 9, 0, 1, 2);

        alphaDecLabel = new QLabel(indicatorFrame);
        alphaDecLabel->setObjectName(QStringLiteral("alphaDecLabel"));
        alphaDecLabel->setFont(font1);

        gridLayout_2->addWidget(alphaDecLabel, 5, 0, 1, 2);

        deltaDecLabel = new QLabel(indicatorFrame);
        deltaDecLabel->setObjectName(QStringLiteral("deltaDecLabel"));
        deltaDecLabel->setFont(font1);

        gridLayout_2->addWidget(deltaDecLabel, 7, 0, 1, 2);


        gridLayout->addWidget(indicatorFrame, 0, 0, 1, 2);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        minLabel = new QLabel(dockWidgetContents);
        minLabel->setObjectName(QStringLiteral("minLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, minLabel);

        minLineEdit = new QLineEdit(dockWidgetContents);
        minLineEdit->setObjectName(QStringLiteral("minLineEdit"));
        minLineEdit->setMinimumSize(QSize(90, 0));
        minLineEdit->setFocusPolicy(Qt::StrongFocus);

        formLayout->setWidget(0, QFormLayout::FieldRole, minLineEdit);

        maxLabel = new QLabel(dockWidgetContents);
        maxLabel->setObjectName(QStringLiteral("maxLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, maxLabel);

        maxLineEdit = new QLineEdit(dockWidgetContents);
        maxLineEdit->setObjectName(QStringLiteral("maxLineEdit"));
        maxLineEdit->setMinimumSize(QSize(90, 0));
        maxLineEdit->setFocusPolicy(Qt::StrongFocus);

        formLayout->setWidget(1, QFormLayout::FieldRole, maxLineEdit);

        autocontrastPushButton = new QPushButton(dockWidgetContents);
        autocontrastPushButton->setObjectName(QStringLiteral("autocontrastPushButton"));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette5.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette5.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette5.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette5.setBrush(QPalette::Active, QPalette::Text, brush);
        palette5.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette5.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette5.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette5.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        palette5.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette5.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette5.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette5.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette5.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette5.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette5.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        palette5.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette5.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette5.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette5.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette5.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette5.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette5.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette5.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette5.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette5.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette5.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        autocontrastPushButton->setPalette(palette5);
        QFont font2;
        font2.setBold(false);
        font2.setWeight(50);
        autocontrastPushButton->setFont(font2);
        autocontrastPushButton->setFocusPolicy(Qt::NoFocus);
        autocontrastPushButton->setAutoFillBackground(true);
        autocontrastPushButton->setCheckable(true);

        formLayout->setWidget(2, QFormLayout::SpanningRole, autocontrastPushButton);


        gridLayout->addLayout(formLayout, 4, 0, 1, 2);

        quitPushButton = new QPushButton(dockWidgetContents);
        quitPushButton->setObjectName(QStringLiteral("quitPushButton"));

        gridLayout->addWidget(quitPushButton, 9, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 9, 0, 1, 1);

        IvConfDockWidget->setWidget(dockWidgetContents);

        retranslateUi(IvConfDockWidget);

        QMetaObject::connectSlotsByName(IvConfDockWidget);
    } // setupUi

    void retranslateUi(QDockWidget *IvConfDockWidget)
    {
        IvConfDockWidget->setWindowTitle(QApplication::translate("IvConfDockWidget", "Configuration", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        zoomOutPushButton->setStatusTip(QApplication::translate("IvConfDockWidget", "Zoom out", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        zoomOutPushButton->setText(QApplication::translate("IvConfDockWidget", "-", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        zoomInPushButton->setStatusTip(QApplication::translate("IvConfDockWidget", "Zoom in", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        zoomInPushButton->setText(QApplication::translate("IvConfDockWidget", "+", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        zoomZeroPushButton->setStatusTip(QApplication::translate("IvConfDockWidget", "1:1 zoom", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        zoomZeroPushButton->setText(QApplication::translate("IvConfDockWidget", "o", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        zoomFitPushButton->setStatusTip(QApplication::translate("IvConfDockWidget", "Zoom to fit image into the window", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        zoomFitPushButton->setText(QApplication::translate("IvConfDockWidget", "Fit", Q_NULLPTR));
        filterLabel->setText(QApplication::translate("IvConfDockWidget", "Filter", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        filterLineEdit->setStatusTip(QApplication::translate("IvConfDockWidget", "Only these images will be selected when using the yellow navigation buttons at the top.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        filterLineEdit->setText(QApplication::translate("IvConfDockWidget", "*.fits", Q_NULLPTR));
        yposLabel->setText(QApplication::translate("IvConfDockWidget", "y", Q_NULLPTR));
        valueLabel->setText(QApplication::translate("IvConfDockWidget", "Value", Q_NULLPTR));
        valueGreenLabel->setText(QApplication::translate("IvConfDockWidget", "Value", Q_NULLPTR));
        valueBlueLabel->setText(QApplication::translate("IvConfDockWidget", "Value", Q_NULLPTR));
        zoomLabel->setText(QApplication::translate("IvConfDockWidget", "Zoom level:", Q_NULLPTR));
        alphaHexLabel->setText(QApplication::translate("IvConfDockWidget", "R.A.", Q_NULLPTR));
        deltaHexLabel->setText(QApplication::translate("IvConfDockWidget", "Dec", Q_NULLPTR));
        rmsLabel->setText(QApplication::translate("IvConfDockWidget", "stdev", Q_NULLPTR));
        xposLabel->setText(QApplication::translate("IvConfDockWidget", "x", Q_NULLPTR));
        medianLabel->setText(QApplication::translate("IvConfDockWidget", "Median", Q_NULLPTR));
        alphaDecLabel->setText(QApplication::translate("IvConfDockWidget", "R.A.", Q_NULLPTR));
        deltaDecLabel->setText(QApplication::translate("IvConfDockWidget", "Dec", Q_NULLPTR));
        minLabel->setText(QApplication::translate("IvConfDockWidget", "Min", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        minLineEdit->setStatusTip(QApplication::translate("IvConfDockWidget", "Pixels fainter than this value will be shown black", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        maxLabel->setText(QApplication::translate("IvConfDockWidget", "Max", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        maxLineEdit->setStatusTip(QApplication::translate("IvConfDockWidget", "Pixels brighter than this value will be shown white", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        autocontrastPushButton->setStatusTip(QApplication::translate("IvConfDockWidget", "Automatically determine the dynamic range for display", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        autocontrastPushButton->setText(QApplication::translate("IvConfDockWidget", "Auto contrast", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        quitPushButton->setStatusTip(QApplication::translate("IvConfDockWidget", "Cloes iView, all changes to images will be saved.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        quitPushButton->setText(QApplication::translate("IvConfDockWidget", "Close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class IvConfDockWidget: public Ui_IvConfDockWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IVCONFDOCKWIDGET_H
