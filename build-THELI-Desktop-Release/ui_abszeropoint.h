/********************************************************************************
** Form generated from reading UI file 'abszeropoint.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABSZEROPOINT_H
#define UI_ABSZEROPOINT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_AbsZeroPoint
{
public:
    QAction *actionClose;
    QWidget *centralwidget;
    QGridLayout *gridLayout_4;
    QPushButton *zpLoadPushButton;
    QSpacerItem *horizontalSpacer;
    QLineEdit *zpImageLineEdit;
    QLabel *label;
    QFrame *frame_2;
    QGridLayout *gridLayout_5;
    QCustomPlot *zpPlot;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents_2;
    QGridLayout *gridLayout_3;
    QLabel *label_3;
    QSpacerItem *verticalSpacer_2;
    QLabel *zpHeaderLabel;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QPlainTextEdit *zpPlainTextEdit;
    QHBoxLayout *horizontalLayout;
    QPushButton *startPushButton;
    QPushButton *abortPushButton;
    QPushButton *zpClearPushButton;
    QPushButton *zpExportPushButton;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QFrame *frame_3;
    QGridLayout *gridLayout;
    QLabel *label_35;
    QLabel *label_37;
    QSpinBox *zpFitOrderSpinBox;
    QComboBox *zpFilterComboBox;
    QLabel *label_39;
    QLineEdit *zpPhoterrorLineEdit;
    QLineEdit *zpApertureLineEdit;
    QLabel *label_36;
    QLineEdit *zpSaturationLineEdit;
    QComboBox *zpRefcatComboBox;
    QComboBox *zpColorComboBox;
    QLabel *label_38;
    QLabel *label_31;
    QLabel *label_33;
    QLabel *label_2;
    QLabel *label_4;
    QLineEdit *zpDTLineEdit;
    QLineEdit *zpDMINLineEdit;
    QPushButton *closePushButton;

    void setupUi(QMainWindow *AbsZeroPoint)
    {
        if (AbsZeroPoint->objectName().isEmpty())
            AbsZeroPoint->setObjectName(QStringLiteral("AbsZeroPoint"));
        AbsZeroPoint->resize(879, 784);
        AbsZeroPoint->setAutoFillBackground(true);
        actionClose = new QAction(AbsZeroPoint);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        centralwidget = new QWidget(AbsZeroPoint);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout_4 = new QGridLayout(centralwidget);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        zpLoadPushButton = new QPushButton(centralwidget);
        zpLoadPushButton->setObjectName(QStringLiteral("zpLoadPushButton"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(zpLoadPushButton->sizePolicy().hasHeightForWidth());
        zpLoadPushButton->setSizePolicy(sizePolicy);
        zpLoadPushButton->setFocusPolicy(Qt::NoFocus);

        gridLayout_4->addWidget(zpLoadPushButton, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer, 0, 1, 1, 1);

        zpImageLineEdit = new QLineEdit(centralwidget);
        zpImageLineEdit->setObjectName(QStringLiteral("zpImageLineEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(zpImageLineEdit->sizePolicy().hasHeightForWidth());
        zpImageLineEdit->setSizePolicy(sizePolicy1);

        gridLayout_4->addWidget(zpImageLineEdit, 1, 0, 1, 2);

        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        QPalette palette;
        QBrush brush(QColor(170, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(119, 120, 120, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label->setPalette(palette);
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        gridLayout_4->addWidget(label, 2, 0, 1, 2);

        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy2);
        QPalette palette1;
        QBrush brush2(QColor(252, 252, 252, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush2);
        QBrush brush3(QColor(195, 195, 195, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        frame_2->setPalette(palette1);
        frame_2->setAutoFillBackground(true);
        frame_2->setFrameShape(QFrame::Panel);
        frame_2->setFrameShadow(QFrame::Sunken);
        frame_2->setLineWidth(2);
        gridLayout_5 = new QGridLayout(frame_2);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        zpPlot = new QCustomPlot(frame_2);
        zpPlot->setObjectName(QStringLiteral("zpPlot"));
        sizePolicy2.setHeightForWidth(zpPlot->sizePolicy().hasHeightForWidth());
        zpPlot->setSizePolicy(sizePolicy2);
        zpPlot->setMinimumSize(QSize(400, 300));
        zpPlot->setAutoFillBackground(true);

        gridLayout_5->addWidget(zpPlot, 0, 0, 1, 1);


        gridLayout_4->addWidget(frame_2, 3, 0, 1, 2);

        AbsZeroPoint->setCentralWidget(centralwidget);
        menubar = new QMenuBar(AbsZeroPoint);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 879, 23));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        AbsZeroPoint->setMenuBar(menubar);
        statusbar = new QStatusBar(AbsZeroPoint);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        AbsZeroPoint->setStatusBar(statusbar);
        dockWidget = new QDockWidget(AbsZeroPoint);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::Base, brush2);
        QBrush brush4(QColor(219, 219, 220, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush4);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush4);
        dockWidget->setPalette(palette2);
        dockWidget->setAutoFillBackground(true);
        dockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QStringLiteral("dockWidgetContents_2"));
        gridLayout_3 = new QGridLayout(dockWidgetContents_2);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label_3 = new QLabel(dockWidgetContents_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font);

        gridLayout_3->addWidget(label_3, 0, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(17, 17, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_3->addItem(verticalSpacer_2, 2, 2, 1, 1);

        zpHeaderLabel = new QLabel(dockWidgetContents_2);
        zpHeaderLabel->setObjectName(QStringLiteral("zpHeaderLabel"));
        zpHeaderLabel->setFont(font);

        gridLayout_3->addWidget(zpHeaderLabel, 3, 0, 1, 3);

        frame = new QFrame(dockWidgetContents_2);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy3);
        frame->setAutoFillBackground(true);
        frame->setFrameShape(QFrame::Panel);
        frame->setFrameShadow(QFrame::Sunken);
        frame->setLineWidth(2);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        zpPlainTextEdit = new QPlainTextEdit(frame);
        zpPlainTextEdit->setObjectName(QStringLiteral("zpPlainTextEdit"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(zpPlainTextEdit->sizePolicy().hasHeightForWidth());
        zpPlainTextEdit->setSizePolicy(sizePolicy4);
        zpPlainTextEdit->setFocusPolicy(Qt::NoFocus);
        zpPlainTextEdit->setReadOnly(true);

        gridLayout_2->addWidget(zpPlainTextEdit, 0, 0, 1, 1);


        gridLayout_3->addWidget(frame, 4, 0, 1, 3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        startPushButton = new QPushButton(dockWidgetContents_2);
        startPushButton->setObjectName(QStringLiteral("startPushButton"));
        startPushButton->setMinimumSize(QSize(80, 0));
        QPalette palette3;
        QBrush brush5(QColor(0, 0, 0, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush5);
        QBrush brush6(QColor(68, 216, 204, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush6);
        QBrush brush7(QColor(149, 255, 247, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Light, brush7);
        QBrush brush8(QColor(108, 235, 225, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        QBrush brush9(QColor(34, 108, 102, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Dark, brush9);
        QBrush brush10(QColor(45, 144, 136, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush5);
        QBrush brush11(QColor(255, 255, 255, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::BrightText, brush11);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush5);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush11);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush6);
        palette3.setBrush(QPalette::Active, QPalette::Shadow, brush5);
        QBrush brush12(QColor(161, 235, 229, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::AlternateBase, brush12);
        QBrush brush13(QColor(255, 255, 220, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipBase, brush13);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipText, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::Light, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette3.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::BrightText, brush11);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush11);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::Shadow, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush12);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush13);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::Light, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette3.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette3.setBrush(QPalette::Disabled, QPalette::BrightText, brush11);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::Shadow, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush13);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush5);
        startPushButton->setPalette(palette3);
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        startPushButton->setFont(font1);
        startPushButton->setFocusPolicy(Qt::NoFocus);
        startPushButton->setAutoFillBackground(false);
        startPushButton->setCheckable(false);

        horizontalLayout->addWidget(startPushButton);

        abortPushButton = new QPushButton(dockWidgetContents_2);
        abortPushButton->setObjectName(QStringLiteral("abortPushButton"));
        abortPushButton->setMinimumSize(QSize(80, 0));
        abortPushButton->setMaximumSize(QSize(80, 16777215));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush5);
        QBrush brush14(QColor(223, 77, 123, 255));
        brush14.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush14);
        QBrush brush15(QColor(255, 168, 195, 255));
        brush15.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Light, brush15);
        QBrush brush16(QColor(239, 122, 159, 255));
        brush16.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Midlight, brush16);
        QBrush brush17(QColor(111, 38, 61, 255));
        brush17.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Dark, brush17);
        QBrush brush18(QColor(149, 51, 82, 255));
        brush18.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::Mid, brush18);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush5);
        palette4.setBrush(QPalette::Active, QPalette::BrightText, brush11);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush5);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush11);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush14);
        palette4.setBrush(QPalette::Active, QPalette::Shadow, brush5);
        QBrush brush19(QColor(239, 166, 189, 255));
        brush19.setStyle(Qt::SolidPattern);
        palette4.setBrush(QPalette::Active, QPalette::AlternateBase, brush19);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipBase, brush13);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipText, brush5);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush5);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush14);
        palette4.setBrush(QPalette::Inactive, QPalette::Light, brush15);
        palette4.setBrush(QPalette::Inactive, QPalette::Midlight, brush16);
        palette4.setBrush(QPalette::Inactive, QPalette::Dark, brush17);
        palette4.setBrush(QPalette::Inactive, QPalette::Mid, brush18);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush5);
        palette4.setBrush(QPalette::Inactive, QPalette::BrightText, brush11);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush5);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush11);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush14);
        palette4.setBrush(QPalette::Inactive, QPalette::Shadow, brush5);
        palette4.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush19);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush13);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush5);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush17);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush14);
        palette4.setBrush(QPalette::Disabled, QPalette::Light, brush15);
        palette4.setBrush(QPalette::Disabled, QPalette::Midlight, brush16);
        palette4.setBrush(QPalette::Disabled, QPalette::Dark, brush17);
        palette4.setBrush(QPalette::Disabled, QPalette::Mid, brush18);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush17);
        palette4.setBrush(QPalette::Disabled, QPalette::BrightText, brush11);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush17);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush14);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush14);
        palette4.setBrush(QPalette::Disabled, QPalette::Shadow, brush5);
        palette4.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush14);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush13);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush5);
        abortPushButton->setPalette(palette4);
        abortPushButton->setFont(font1);
        abortPushButton->setFocusPolicy(Qt::NoFocus);

        horizontalLayout->addWidget(abortPushButton);

        zpClearPushButton = new QPushButton(dockWidgetContents_2);
        zpClearPushButton->setObjectName(QStringLiteral("zpClearPushButton"));
        zpClearPushButton->setFocusPolicy(Qt::NoFocus);

        horizontalLayout->addWidget(zpClearPushButton);

        zpExportPushButton = new QPushButton(dockWidgetContents_2);
        zpExportPushButton->setObjectName(QStringLiteral("zpExportPushButton"));
        zpExportPushButton->setFocusPolicy(Qt::NoFocus);

        horizontalLayout->addWidget(zpExportPushButton);

        horizontalSpacer_2 = new QSpacerItem(17, 25, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout_3->addLayout(horizontalLayout, 5, 0, 1, 3);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_3->addItem(verticalSpacer, 6, 1, 1, 1);

        frame_3 = new QFrame(dockWidgetContents_2);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        QSizePolicy sizePolicy5(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(frame_3->sizePolicy().hasHeightForWidth());
        frame_3->setSizePolicy(sizePolicy5);
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::Base, brush2);
        QBrush brush20(QColor(199, 199, 200, 255));
        brush20.setStyle(Qt::SolidPattern);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush20);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush20);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush20);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush20);
        frame_3->setPalette(palette5);
        frame_3->setAutoFillBackground(true);
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame_3);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_35 = new QLabel(frame_3);
        label_35->setObjectName(QStringLiteral("label_35"));

        gridLayout->addWidget(label_35, 4, 1, 1, 1);

        label_37 = new QLabel(frame_3);
        label_37->setObjectName(QStringLiteral("label_37"));

        gridLayout->addWidget(label_37, 9, 1, 1, 1);

        zpFitOrderSpinBox = new QSpinBox(frame_3);
        zpFitOrderSpinBox->setObjectName(QStringLiteral("zpFitOrderSpinBox"));
        zpFitOrderSpinBox->setEnabled(false);
        zpFitOrderSpinBox->setMinimum(1);
        zpFitOrderSpinBox->setMaximum(3);

        gridLayout->addWidget(zpFitOrderSpinBox, 3, 2, 1, 2);

        zpFilterComboBox = new QComboBox(frame_3);
        zpFilterComboBox->setObjectName(QStringLiteral("zpFilterComboBox"));

        gridLayout->addWidget(zpFilterComboBox, 1, 2, 1, 2);

        label_39 = new QLabel(frame_3);
        label_39->setObjectName(QStringLiteral("label_39"));
        label_39->setEnabled(false);

        gridLayout->addWidget(label_39, 3, 1, 1, 1);

        zpPhoterrorLineEdit = new QLineEdit(frame_3);
        zpPhoterrorLineEdit->setObjectName(QStringLiteral("zpPhoterrorLineEdit"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(zpPhoterrorLineEdit->sizePolicy().hasHeightForWidth());
        zpPhoterrorLineEdit->setSizePolicy(sizePolicy6);
        zpPhoterrorLineEdit->setMinimumSize(QSize(0, 0));
        zpPhoterrorLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(zpPhoterrorLineEdit, 5, 2, 1, 2);

        zpApertureLineEdit = new QLineEdit(frame_3);
        zpApertureLineEdit->setObjectName(QStringLiteral("zpApertureLineEdit"));
        sizePolicy6.setHeightForWidth(zpApertureLineEdit->sizePolicy().hasHeightForWidth());
        zpApertureLineEdit->setSizePolicy(sizePolicy6);
        zpApertureLineEdit->setMinimumSize(QSize(0, 0));
        zpApertureLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(zpApertureLineEdit, 10, 1, 1, 3);

        label_36 = new QLabel(frame_3);
        label_36->setObjectName(QStringLiteral("label_36"));

        gridLayout->addWidget(label_36, 5, 1, 1, 1);

        zpSaturationLineEdit = new QLineEdit(frame_3);
        zpSaturationLineEdit->setObjectName(QStringLiteral("zpSaturationLineEdit"));
        sizePolicy6.setHeightForWidth(zpSaturationLineEdit->sizePolicy().hasHeightForWidth());
        zpSaturationLineEdit->setSizePolicy(sizePolicy6);
        zpSaturationLineEdit->setMinimumSize(QSize(0, 0));
        zpSaturationLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(zpSaturationLineEdit, 4, 2, 1, 2);

        zpRefcatComboBox = new QComboBox(frame_3);
        zpRefcatComboBox->setObjectName(QStringLiteral("zpRefcatComboBox"));

        gridLayout->addWidget(zpRefcatComboBox, 0, 2, 1, 2);

        zpColorComboBox = new QComboBox(frame_3);
        zpColorComboBox->setObjectName(QStringLiteral("zpColorComboBox"));

        gridLayout->addWidget(zpColorComboBox, 2, 2, 1, 2);

        label_38 = new QLabel(frame_3);
        label_38->setObjectName(QStringLiteral("label_38"));

        gridLayout->addWidget(label_38, 2, 1, 1, 1);

        label_31 = new QLabel(frame_3);
        label_31->setObjectName(QStringLiteral("label_31"));

        gridLayout->addWidget(label_31, 0, 1, 1, 1);

        label_33 = new QLabel(frame_3);
        label_33->setObjectName(QStringLiteral("label_33"));

        gridLayout->addWidget(label_33, 1, 1, 1, 1);

        label_2 = new QLabel(frame_3);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 6, 1, 1, 1);

        label_4 = new QLabel(frame_3);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 7, 1, 1, 1);

        zpDTLineEdit = new QLineEdit(frame_3);
        zpDTLineEdit->setObjectName(QStringLiteral("zpDTLineEdit"));
        sizePolicy6.setHeightForWidth(zpDTLineEdit->sizePolicy().hasHeightForWidth());
        zpDTLineEdit->setSizePolicy(sizePolicy6);
        zpDTLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(zpDTLineEdit, 6, 2, 1, 2);

        zpDMINLineEdit = new QLineEdit(frame_3);
        zpDMINLineEdit->setObjectName(QStringLiteral("zpDMINLineEdit"));
        sizePolicy6.setHeightForWidth(zpDMINLineEdit->sizePolicy().hasHeightForWidth());
        zpDMINLineEdit->setSizePolicy(sizePolicy6);
        zpDMINLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(zpDMINLineEdit, 7, 2, 1, 2);


        gridLayout_3->addWidget(frame_3, 1, 0, 1, 3);

        closePushButton = new QPushButton(dockWidgetContents_2);
        closePushButton->setObjectName(QStringLiteral("closePushButton"));
        closePushButton->setMinimumSize(QSize(80, 0));
        closePushButton->setMaximumSize(QSize(80, 16777215));

        gridLayout_3->addWidget(closePushButton, 6, 0, 1, 1);

        dockWidget->setWidget(dockWidgetContents_2);
        AbsZeroPoint->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionClose);

        retranslateUi(AbsZeroPoint);

        QMetaObject::connectSlotsByName(AbsZeroPoint);
    } // setupUi

    void retranslateUi(QMainWindow *AbsZeroPoint)
    {
        AbsZeroPoint->setWindowTitle(QApplication::translate("AbsZeroPoint", "Absolute photometric calibration", Q_NULLPTR));
        actionClose->setText(QApplication::translate("AbsZeroPoint", "Close", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        zpLoadPushButton->setStatusTip(QApplication::translate("AbsZeroPoint", "Select an image with valid WCS for photometric calibration.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        zpLoadPushButton->setText(QApplication::translate("AbsZeroPoint", "Select image ...", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        zpImageLineEdit->setStatusTip(QApplication::translate("AbsZeroPoint", "The full path to the image that is to be calibrated. Any image with valid WCS will work.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label->setText(QApplication::translate("AbsZeroPoint", "Click on a zeropoint to update the FITS header", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        zpPlot->setStatusTip(QApplication::translate("AbsZeroPoint", "Click on a zeropoint in the upper panel to update the FITS header", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        menuFile->setTitle(QApplication::translate("AbsZeroPoint", "File", Q_NULLPTR));
        dockWidget->setWindowTitle(QApplication::translate("AbsZeroPoint", "Configuration", Q_NULLPTR));
        label_3->setText(QApplication::translate("AbsZeroPoint", "Setup", Q_NULLPTR));
        zpHeaderLabel->setText(QApplication::translate("AbsZeroPoint", "System output", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        zpPlainTextEdit->setStatusTip(QApplication::translate("AbsZeroPoint", "This window displays the keywords with the photometric solution written to the FITS header.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        startPushButton->setText(QApplication::translate("AbsZeroPoint", "Start", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        abortPushButton->setStatusTip(QApplication::translate("AbsZeroPoint", "Lets the current task finish gracefully, then stops before executing further tasks in the queue.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        abortPushButton->setText(QApplication::translate("AbsZeroPoint", "Abort", Q_NULLPTR));
        zpClearPushButton->setText(QApplication::translate("AbsZeroPoint", "Clear plot", Q_NULLPTR));
        zpExportPushButton->setText(QApplication::translate("AbsZeroPoint", "Export plot", Q_NULLPTR));
        label_35->setText(QApplication::translate("AbsZeroPoint", "Saturation / nonlinearity limit", Q_NULLPTR));
        label_37->setText(QApplication::translate("AbsZeroPoint", "Aperture diameters [ pixel ]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        zpFitOrderSpinBox->setStatusTip(QApplication::translate("AbsZeroPoint", "The order of the polynomial fit for the ZP / color term dependence", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        zpFilterComboBox->setStatusTip(QApplication::translate("AbsZeroPoint", "The filter in which you observed.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_39->setText(QApplication::translate("AbsZeroPoint", "Polynomial fit order", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        zpPhoterrorLineEdit->setToolTip(QApplication::translate("AbsZeroPoint", "The maximum photometric error allowed for sources in the image.", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        zpPhoterrorLineEdit->setStatusTip(QApplication::translate("AbsZeroPoint", "The upper limit to the measured photometric error in your data.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        zpApertureLineEdit->setStatusTip(QApplication::translate("AbsZeroPoint", "A comma-separated list of aperture diameters used for the growth curve.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_36->setText(QApplication::translate("AbsZeroPoint", "Max phot error [ mag ]", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        zpSaturationLineEdit->setToolTip(QApplication::translate("AbsZeroPoint", "The level above which non-linearity (or saturation) occurs.", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        zpSaturationLineEdit->setStatusTip(QApplication::translate("AbsZeroPoint", "The level at which nonlinearity or saturation occurs.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        zpRefcatComboBox->clear();
        zpRefcatComboBox->insertItems(0, QStringList()
         << QApplication::translate("AbsZeroPoint", "PANSTARRS-DR1", Q_NULLPTR)
         << QApplication::translate("AbsZeroPoint", "SDSS-DR12", Q_NULLPTR)
         << QApplication::translate("AbsZeroPoint", "APASS-DR9", Q_NULLPTR)
         << QApplication::translate("AbsZeroPoint", "2MASS", Q_NULLPTR)
         << QApplication::translate("AbsZeroPoint", "VHS", Q_NULLPTR)
         << QApplication::translate("AbsZeroPoint", "UKIDSS", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        zpRefcatComboBox->setStatusTip(QApplication::translate("AbsZeroPoint", "A photometric reference catalog matching the filter of your observations. PANSTARRS queries can take several minutes.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        zpColorComboBox->setStatusTip(QApplication::translate("AbsZeroPoint", "The color term is determined from the reference catalog. Ideally, choose one that matches other filters you have for this target.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_38->setText(QApplication::translate("AbsZeroPoint", "Color term you want to use", Q_NULLPTR));
        label_31->setText(QApplication::translate("AbsZeroPoint", "Photometric reference catalog", Q_NULLPTR));
        label_33->setText(QApplication::translate("AbsZeroPoint", "Filter in which you observed", Q_NULLPTR));
        label_2->setText(QApplication::translate("AbsZeroPoint", "DT", Q_NULLPTR));
        label_4->setText(QApplication::translate("AbsZeroPoint", "DMIN", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        zpDTLineEdit->setStatusTip(QApplication::translate("AbsZeroPoint", "Detection threshold per pixel", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        zpDTLineEdit->setText(QApplication::translate("AbsZeroPoint", "10", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        zpDMINLineEdit->setStatusTip(QApplication::translate("AbsZeroPoint", "Minimum number of connected pixels above the detection threshold", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        zpDMINLineEdit->setText(QApplication::translate("AbsZeroPoint", "10", Q_NULLPTR));
        closePushButton->setText(QApplication::translate("AbsZeroPoint", "Close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AbsZeroPoint: public Ui_AbsZeroPoint {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABSZEROPOINT_H
