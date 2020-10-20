/********************************************************************************
** Form generated from reading UI file 'colorpicture.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLORPICTURE_H
#define UI_COLORPICTURE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ColorPicture
{
public:
    QAction *actionClose;
    QWidget *centralwidget;
    QGridLayout *gridLayout_7;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_2;
    QPushButton *selectDirPushButton;
    QLineEdit *dirLineEdit;
    QSpacerItem *verticalSpacer;
    QLabel *label;
    QLabel *titleLabel;
    QFrame *frame_4;
    QGridLayout *gridLayout;
    QLabel *label_19;
    QListView *coaddImageListView;
    QListView *coaddDirListView;
    QPushButton *clearCoaddsPushButton;
    QPushButton *getCoaddsPushButton;
    QSpacerItem *horizontalSpacer_2;
    QFrame *bbnb1Frame;
    QGridLayout *gridLayout_5;
    QLabel *subtitle2Label;
    QComboBox *narrowbandComboBox;
    QLabel *subtitle1Label;
    QComboBox *broadbandComboBox;
    QPushButton *getRatioPushButton;
    QLineEdit *filterRatioLineEdit;
    QLabel *label_6;
    QLineEdit *narrowbandWeightLineEdit;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_2;
    QTextEdit *textEdit;
    QPushButton *BBNBcombinePushButton;
    QWidget *tab_3;
    QLabel *label_2;
    QFrame *frame_3;
    QGridLayout *gridLayout_3;
    QLabel *redLabel;
    QLineEdit *blueErrorLineEdit;
    QPushButton *previewCalibPushButton;
    QLabel *label_16;
    QLabel *label_15;
    QPushButton *resultAVGWHITEPushButton;
    QSpacerItem *horizontalSpacer_10;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_18;
    QPushButton *resultSDSSPushButton;
    QLabel *numAPASSLabel;
    QLabel *numSDSSLabel;
    QLineEdit *redFactorLineEdit;
    QComboBox *blueComboBox;
    QLineEdit *DTLineEdit;
    QPushButton *resultPANSTARRSPushButton;
    QLabel *blueLabel;
    QLineEdit *greenErrorLineEdit;
    QLabel *numPANSTARRSLabel;
    QLineEdit *DMINLineEdit;
    QComboBox *greenComboBox;
    QLabel *label_4;
    QLabel *numAVGWHITELabel;
    QSpacerItem *horizontalSpacer_12;
    QLineEdit *blueFactorLineEdit;
    QPushButton *calibratePushButton;
    QComboBox *redComboBox;
    QPushButton *resultAPASSPushButton;
    QLineEdit *greenFactorLineEdit;
    QLineEdit *redErrorLineEdit;
    QLabel *greenLabel;
    QLabel *label_5;
    QLabel *label_10;
    QFrame *statisticsFrame;
    QGridLayout *gridLayout_8;
    QSpacerItem *horizontalSpacer_5;
    QTableWidget *statisticsTableWidget;
    QPushButton *createTiffPushButton;
    QLabel *label_9;
    QPushButton *getStatisticsPushButton;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label_3;
    QGridLayout *gridLayout_6;
    QPlainTextEdit *processingTextEdit;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *abortPushButton;
    QSpacerItem *horizontalSpacer;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ColorPicture)
    {
        if (ColorPicture->objectName().isEmpty())
            ColorPicture->setObjectName(QStringLiteral("ColorPicture"));
        ColorPicture->resize(989, 844);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ColorPicture->sizePolicy().hasHeightForWidth());
        ColorPicture->setSizePolicy(sizePolicy);
        actionClose = new QAction(ColorPicture);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        centralwidget = new QWidget(ColorPicture);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout_7 = new QGridLayout(centralwidget);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QPalette palette;
        QBrush brush(QColor(252, 252, 252, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(199, 199, 200, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        tabWidget->setPalette(palette);
        tabWidget->setFocusPolicy(Qt::ClickFocus);
        tabWidget->setAutoFillBackground(true);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_2 = new QGridLayout(tab);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        selectDirPushButton = new QPushButton(tab);
        selectDirPushButton->setObjectName(QStringLiteral("selectDirPushButton"));
        selectDirPushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_2->addWidget(selectDirPushButton, 0, 0, 1, 1);

        dirLineEdit = new QLineEdit(tab);
        dirLineEdit->setObjectName(QStringLiteral("dirLineEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(dirLineEdit->sizePolicy().hasHeightForWidth());
        dirLineEdit->setSizePolicy(sizePolicy1);
        dirLineEdit->setFocusPolicy(Qt::StrongFocus);
        dirLineEdit->setReadOnly(true);

        gridLayout_2->addWidget(dirLineEdit, 0, 1, 1, 3);

        verticalSpacer = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer, 1, 0, 1, 1);

        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        gridLayout_2->addWidget(label, 2, 0, 1, 1);

        titleLabel = new QLabel(tab);
        titleLabel->setObjectName(QStringLiteral("titleLabel"));
        titleLabel->setFont(font);

        gridLayout_2->addWidget(titleLabel, 2, 3, 1, 1);

        frame_4 = new QFrame(tab);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame_4->sizePolicy().hasHeightForWidth());
        frame_4->setSizePolicy(sizePolicy2);
        QPalette palette1;
        QBrush brush2(QColor(0, 0, 0, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        QBrush brush3(QColor(219, 219, 220, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush3);
        QBrush brush4(QColor(255, 255, 255, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush4);
        QBrush brush5(QColor(237, 237, 237, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush5);
        QBrush brush6(QColor(109, 109, 110, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush6);
        QBrush brush7(QColor(146, 146, 147, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush7);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush4);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush5);
        QBrush brush8(QColor(255, 255, 220, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush8);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        frame_4->setPalette(palette1);
        frame_4->setAutoFillBackground(true);
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Plain);
        gridLayout = new QGridLayout(frame_4);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_19 = new QLabel(frame_4);
        label_19->setObjectName(QStringLiteral("label_19"));

        gridLayout->addWidget(label_19, 2, 0, 1, 2);

        coaddImageListView = new QListView(frame_4);
        coaddImageListView->setObjectName(QStringLiteral("coaddImageListView"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(coaddImageListView->sizePolicy().hasHeightForWidth());
        coaddImageListView->setSizePolicy(sizePolicy3);
        coaddImageListView->setMinimumSize(QSize(400, 0));
        QPalette palette2;
        QBrush brush9(QColor(255, 255, 235, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush9);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush9);
        QBrush brush10(QColor(238, 255, 255, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush10);
        coaddImageListView->setPalette(palette2);
        coaddImageListView->setFocusPolicy(Qt::NoFocus);
        coaddImageListView->setFrameShape(QFrame::Panel);
        coaddImageListView->setLineWidth(2);
        coaddImageListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        coaddImageListView->setSelectionMode(QAbstractItemView::NoSelection);

        gridLayout->addWidget(coaddImageListView, 3, 0, 1, 2);

        coaddDirListView = new QListView(frame_4);
        coaddDirListView->setObjectName(QStringLiteral("coaddDirListView"));
        sizePolicy3.setHeightForWidth(coaddDirListView->sizePolicy().hasHeightForWidth());
        coaddDirListView->setSizePolicy(sizePolicy3);
        coaddDirListView->setMinimumSize(QSize(400, 0));
        coaddDirListView->setFocusPolicy(Qt::ClickFocus);
        coaddDirListView->setFrameShape(QFrame::Panel);
        coaddDirListView->setLineWidth(2);
        coaddDirListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        coaddDirListView->setProperty("showDropIndicator", QVariant(false));
        coaddDirListView->setAlternatingRowColors(false);
        coaddDirListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        coaddDirListView->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout->addWidget(coaddDirListView, 0, 0, 1, 2);

        clearCoaddsPushButton = new QPushButton(frame_4);
        clearCoaddsPushButton->setObjectName(QStringLiteral("clearCoaddsPushButton"));
        clearCoaddsPushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(clearCoaddsPushButton, 1, 1, 1, 1);

        getCoaddsPushButton = new QPushButton(frame_4);
        getCoaddsPushButton->setObjectName(QStringLiteral("getCoaddsPushButton"));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        QBrush brush11(QColor(68, 216, 204, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush11);
        QBrush brush12(QColor(149, 255, 247, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Light, brush12);
        QBrush brush13(QColor(108, 235, 225, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Midlight, brush13);
        QBrush brush14(QColor(34, 108, 102, 255));
        brush14.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Dark, brush14);
        QBrush brush15(QColor(45, 144, 136, 255));
        brush15.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Mid, brush15);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette3.setBrush(QPalette::Active, QPalette::BrightText, brush4);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush11);
        palette3.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        QBrush brush16(QColor(161, 235, 229, 255));
        brush16.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipBase, brush8);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette3.setBrush(QPalette::Inactive, QPalette::Light, brush12);
        palette3.setBrush(QPalette::Inactive, QPalette::Midlight, brush13);
        palette3.setBrush(QPalette::Inactive, QPalette::Dark, brush14);
        palette3.setBrush(QPalette::Inactive, QPalette::Mid, brush15);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::BrightText, brush4);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush11);
        palette3.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush16);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush14);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette3.setBrush(QPalette::Disabled, QPalette::Light, brush12);
        palette3.setBrush(QPalette::Disabled, QPalette::Midlight, brush13);
        palette3.setBrush(QPalette::Disabled, QPalette::Dark, brush14);
        palette3.setBrush(QPalette::Disabled, QPalette::Mid, brush15);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush14);
        palette3.setBrush(QPalette::Disabled, QPalette::BrightText, brush4);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush14);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush11);
        palette3.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush11);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        getCoaddsPushButton->setPalette(palette3);
        getCoaddsPushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(getCoaddsPushButton, 1, 0, 1, 1);


        gridLayout_2->addWidget(frame_4, 3, 0, 1, 2);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 3, 2, 1, 1);

        bbnb1Frame = new QFrame(tab);
        bbnb1Frame->setObjectName(QStringLiteral("bbnb1Frame"));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush3);
        palette4.setBrush(QPalette::Active, QPalette::Light, brush4);
        palette4.setBrush(QPalette::Active, QPalette::Midlight, brush5);
        palette4.setBrush(QPalette::Active, QPalette::Dark, brush6);
        palette4.setBrush(QPalette::Active, QPalette::Mid, brush7);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette4.setBrush(QPalette::Active, QPalette::BrightText, brush4);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush3);
        palette4.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette4.setBrush(QPalette::Active, QPalette::AlternateBase, brush5);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipBase, brush8);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush3);
        palette4.setBrush(QPalette::Inactive, QPalette::Light, brush4);
        palette4.setBrush(QPalette::Inactive, QPalette::Midlight, brush5);
        palette4.setBrush(QPalette::Inactive, QPalette::Dark, brush6);
        palette4.setBrush(QPalette::Inactive, QPalette::Mid, brush7);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::BrightText, brush4);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette4.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush5);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush8);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush6);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush3);
        palette4.setBrush(QPalette::Disabled, QPalette::Light, brush4);
        palette4.setBrush(QPalette::Disabled, QPalette::Midlight, brush5);
        palette4.setBrush(QPalette::Disabled, QPalette::Dark, brush6);
        palette4.setBrush(QPalette::Disabled, QPalette::Mid, brush7);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette4.setBrush(QPalette::Disabled, QPalette::BrightText, brush4);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush6);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        palette4.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush3);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush8);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        bbnb1Frame->setPalette(palette4);
        bbnb1Frame->setAutoFillBackground(true);
        bbnb1Frame->setFrameShape(QFrame::StyledPanel);
        bbnb1Frame->setFrameShadow(QFrame::Plain);
        bbnb1Frame->setLineWidth(0);
        gridLayout_5 = new QGridLayout(bbnb1Frame);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        subtitle2Label = new QLabel(bbnb1Frame);
        subtitle2Label->setObjectName(QStringLiteral("subtitle2Label"));

        gridLayout_5->addWidget(subtitle2Label, 0, 0, 1, 1);

        narrowbandComboBox = new QComboBox(bbnb1Frame);
        narrowbandComboBox->setObjectName(QStringLiteral("narrowbandComboBox"));
        sizePolicy1.setHeightForWidth(narrowbandComboBox->sizePolicy().hasHeightForWidth());
        narrowbandComboBox->setSizePolicy(sizePolicy1);
        narrowbandComboBox->setMinimumSize(QSize(300, 0));
        narrowbandComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_5->addWidget(narrowbandComboBox, 0, 1, 1, 3);

        subtitle1Label = new QLabel(bbnb1Frame);
        subtitle1Label->setObjectName(QStringLiteral("subtitle1Label"));

        gridLayout_5->addWidget(subtitle1Label, 1, 0, 1, 1);

        broadbandComboBox = new QComboBox(bbnb1Frame);
        broadbandComboBox->setObjectName(QStringLiteral("broadbandComboBox"));
        sizePolicy1.setHeightForWidth(broadbandComboBox->sizePolicy().hasHeightForWidth());
        broadbandComboBox->setSizePolicy(sizePolicy1);
        broadbandComboBox->setMinimumSize(QSize(300, 0));
        broadbandComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_5->addWidget(broadbandComboBox, 1, 1, 1, 3);

        getRatioPushButton = new QPushButton(bbnb1Frame);
        getRatioPushButton->setObjectName(QStringLiteral("getRatioPushButton"));
        sizePolicy1.setHeightForWidth(getRatioPushButton->sizePolicy().hasHeightForWidth());
        getRatioPushButton->setSizePolicy(sizePolicy1);
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette5.setBrush(QPalette::Active, QPalette::Light, brush12);
        palette5.setBrush(QPalette::Active, QPalette::Midlight, brush13);
        palette5.setBrush(QPalette::Active, QPalette::Dark, brush14);
        palette5.setBrush(QPalette::Active, QPalette::Mid, brush15);
        palette5.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette5.setBrush(QPalette::Active, QPalette::BrightText, brush4);
        palette5.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush11);
        palette5.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette5.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
        palette5.setBrush(QPalette::Active, QPalette::ToolTipBase, brush8);
        palette5.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette5.setBrush(QPalette::Inactive, QPalette::Light, brush12);
        palette5.setBrush(QPalette::Inactive, QPalette::Midlight, brush13);
        palette5.setBrush(QPalette::Inactive, QPalette::Dark, brush14);
        palette5.setBrush(QPalette::Inactive, QPalette::Mid, brush15);
        palette5.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::BrightText, brush4);
        palette5.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush11);
        palette5.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush16);
        palette5.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush8);
        palette5.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush14);
        palette5.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette5.setBrush(QPalette::Disabled, QPalette::Light, brush12);
        palette5.setBrush(QPalette::Disabled, QPalette::Midlight, brush13);
        palette5.setBrush(QPalette::Disabled, QPalette::Dark, brush14);
        palette5.setBrush(QPalette::Disabled, QPalette::Mid, brush15);
        palette5.setBrush(QPalette::Disabled, QPalette::Text, brush14);
        palette5.setBrush(QPalette::Disabled, QPalette::BrightText, brush4);
        palette5.setBrush(QPalette::Disabled, QPalette::ButtonText, brush14);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush11);
        palette5.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush11);
        palette5.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush8);
        palette5.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        getRatioPushButton->setPalette(palette5);
        getRatioPushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_5->addWidget(getRatioPushButton, 2, 1, 1, 1);

        filterRatioLineEdit = new QLineEdit(bbnb1Frame);
        filterRatioLineEdit->setObjectName(QStringLiteral("filterRatioLineEdit"));
        sizePolicy1.setHeightForWidth(filterRatioLineEdit->sizePolicy().hasHeightForWidth());
        filterRatioLineEdit->setSizePolicy(sizePolicy1);
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::Base, brush9);
        palette6.setBrush(QPalette::Inactive, QPalette::Base, brush9);
        palette6.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        filterRatioLineEdit->setPalette(palette6);
        filterRatioLineEdit->setFocusPolicy(Qt::ClickFocus);
        filterRatioLineEdit->setReadOnly(true);

        gridLayout_5->addWidget(filterRatioLineEdit, 2, 2, 1, 2);

        label_6 = new QLabel(bbnb1Frame);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_5->addWidget(label_6, 3, 1, 1, 2);

        narrowbandWeightLineEdit = new QLineEdit(bbnb1Frame);
        narrowbandWeightLineEdit->setObjectName(QStringLiteral("narrowbandWeightLineEdit"));
        sizePolicy1.setHeightForWidth(narrowbandWeightLineEdit->sizePolicy().hasHeightForWidth());
        narrowbandWeightLineEdit->setSizePolicy(sizePolicy1);
        narrowbandWeightLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_5->addWidget(narrowbandWeightLineEdit, 3, 3, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(329, 27, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_4, 4, 0, 1, 2);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_5->addItem(verticalSpacer_2, 5, 0, 1, 1);

        textEdit = new QTextEdit(bbnb1Frame);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        sizePolicy3.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
        textEdit->setSizePolicy(sizePolicy3);
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::Base, brush9);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush9);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        textEdit->setPalette(palette7);
        QFont font1;
        font1.setPointSize(10);
        textEdit->setFont(font1);
        textEdit->setFocusPolicy(Qt::ClickFocus);

        gridLayout_5->addWidget(textEdit, 6, 0, 1, 4);

        BBNBcombinePushButton = new QPushButton(bbnb1Frame);
        BBNBcombinePushButton->setObjectName(QStringLiteral("BBNBcombinePushButton"));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette8.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette8.setBrush(QPalette::Active, QPalette::Light, brush12);
        palette8.setBrush(QPalette::Active, QPalette::Midlight, brush13);
        palette8.setBrush(QPalette::Active, QPalette::Dark, brush14);
        palette8.setBrush(QPalette::Active, QPalette::Mid, brush15);
        palette8.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette8.setBrush(QPalette::Active, QPalette::BrightText, brush4);
        palette8.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette8.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette8.setBrush(QPalette::Active, QPalette::Window, brush11);
        palette8.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette8.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
        palette8.setBrush(QPalette::Active, QPalette::ToolTipBase, brush8);
        palette8.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette8.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette8.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette8.setBrush(QPalette::Inactive, QPalette::Light, brush12);
        palette8.setBrush(QPalette::Inactive, QPalette::Midlight, brush13);
        palette8.setBrush(QPalette::Inactive, QPalette::Dark, brush14);
        palette8.setBrush(QPalette::Inactive, QPalette::Mid, brush15);
        palette8.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette8.setBrush(QPalette::Inactive, QPalette::BrightText, brush4);
        palette8.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette8.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette8.setBrush(QPalette::Inactive, QPalette::Window, brush11);
        palette8.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette8.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush16);
        palette8.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush8);
        palette8.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette8.setBrush(QPalette::Disabled, QPalette::WindowText, brush14);
        palette8.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette8.setBrush(QPalette::Disabled, QPalette::Light, brush12);
        palette8.setBrush(QPalette::Disabled, QPalette::Midlight, brush13);
        palette8.setBrush(QPalette::Disabled, QPalette::Dark, brush14);
        palette8.setBrush(QPalette::Disabled, QPalette::Mid, brush15);
        palette8.setBrush(QPalette::Disabled, QPalette::Text, brush14);
        palette8.setBrush(QPalette::Disabled, QPalette::BrightText, brush4);
        palette8.setBrush(QPalette::Disabled, QPalette::ButtonText, brush14);
        palette8.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette8.setBrush(QPalette::Disabled, QPalette::Window, brush11);
        palette8.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette8.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush11);
        palette8.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush8);
        palette8.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        BBNBcombinePushButton->setPalette(palette8);
        BBNBcombinePushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_5->addWidget(BBNBcombinePushButton, 4, 2, 1, 2);


        gridLayout_2->addWidget(bbnb1Frame, 3, 3, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        label_2 = new QLabel(tab_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(9, 9, 561, 20));
        label_2->setFont(font);
        frame_3 = new QFrame(tab_3);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setGeometry(QRect(9, 35, 944, 176));
        QPalette palette9;
        palette9.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush17(QColor(247, 247, 247, 255));
        brush17.setStyle(Qt::SolidPattern);
        palette9.setBrush(QPalette::Active, QPalette::Window, brush17);
        palette9.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::Window, brush17);
        palette9.setBrush(QPalette::Disabled, QPalette::Base, brush17);
        palette9.setBrush(QPalette::Disabled, QPalette::Window, brush17);
        frame_3->setPalette(palette9);
        frame_3->setAutoFillBackground(true);
        frame_3->setFrameShape(QFrame::Panel);
        frame_3->setFrameShadow(QFrame::Sunken);
        frame_3->setLineWidth(1);
        gridLayout_3 = new QGridLayout(frame_3);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        redLabel = new QLabel(frame_3);
        redLabel->setObjectName(QStringLiteral("redLabel"));
        sizePolicy2.setHeightForWidth(redLabel->sizePolicy().hasHeightForWidth());
        redLabel->setSizePolicy(sizePolicy2);
        QPalette palette10;
        QBrush brush18(QColor(204, 0, 0, 255));
        brush18.setStyle(Qt::SolidPattern);
        palette10.setBrush(QPalette::Active, QPalette::WindowText, brush18);
        palette10.setBrush(QPalette::Inactive, QPalette::WindowText, brush18);
        QBrush brush19(QColor(119, 120, 120, 255));
        brush19.setStyle(Qt::SolidPattern);
        palette10.setBrush(QPalette::Disabled, QPalette::WindowText, brush19);
        redLabel->setPalette(palette10);

        gridLayout_3->addWidget(redLabel, 1, 0, 1, 1);

        blueErrorLineEdit = new QLineEdit(frame_3);
        blueErrorLineEdit->setObjectName(QStringLiteral("blueErrorLineEdit"));
        blueErrorLineEdit->setMinimumSize(QSize(90, 0));
        blueErrorLineEdit->setMaximumSize(QSize(90, 16777215));
        QPalette palette11;
        QBrush brush20(QColor(255, 255, 229, 255));
        brush20.setStyle(Qt::SolidPattern);
        palette11.setBrush(QPalette::Active, QPalette::Base, brush20);
        palette11.setBrush(QPalette::Inactive, QPalette::Base, brush20);
        QBrush brush21(QColor(239, 240, 241, 255));
        brush21.setStyle(Qt::SolidPattern);
        palette11.setBrush(QPalette::Disabled, QPalette::Base, brush21);
        blueErrorLineEdit->setPalette(palette11);
        blueErrorLineEdit->setFocusPolicy(Qt::NoFocus);
        blueErrorLineEdit->setAutoFillBackground(false);
        blueErrorLineEdit->setReadOnly(true);

        gridLayout_3->addWidget(blueErrorLineEdit, 3, 9, 1, 1);

        previewCalibPushButton = new QPushButton(frame_3);
        previewCalibPushButton->setObjectName(QStringLiteral("previewCalibPushButton"));
        QPalette palette12;
        palette12.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette12.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette12.setBrush(QPalette::Active, QPalette::Light, brush12);
        palette12.setBrush(QPalette::Active, QPalette::Midlight, brush13);
        palette12.setBrush(QPalette::Active, QPalette::Dark, brush14);
        palette12.setBrush(QPalette::Active, QPalette::Mid, brush15);
        palette12.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette12.setBrush(QPalette::Active, QPalette::BrightText, brush4);
        palette12.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette12.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette12.setBrush(QPalette::Active, QPalette::Window, brush11);
        palette12.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette12.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
        palette12.setBrush(QPalette::Active, QPalette::ToolTipBase, brush8);
        palette12.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette12.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette12.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette12.setBrush(QPalette::Inactive, QPalette::Light, brush12);
        palette12.setBrush(QPalette::Inactive, QPalette::Midlight, brush13);
        palette12.setBrush(QPalette::Inactive, QPalette::Dark, brush14);
        palette12.setBrush(QPalette::Inactive, QPalette::Mid, brush15);
        palette12.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette12.setBrush(QPalette::Inactive, QPalette::BrightText, brush4);
        palette12.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette12.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette12.setBrush(QPalette::Inactive, QPalette::Window, brush11);
        palette12.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette12.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush16);
        palette12.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush8);
        palette12.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette12.setBrush(QPalette::Disabled, QPalette::WindowText, brush14);
        palette12.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette12.setBrush(QPalette::Disabled, QPalette::Light, brush12);
        palette12.setBrush(QPalette::Disabled, QPalette::Midlight, brush13);
        palette12.setBrush(QPalette::Disabled, QPalette::Dark, brush14);
        palette12.setBrush(QPalette::Disabled, QPalette::Mid, brush15);
        palette12.setBrush(QPalette::Disabled, QPalette::Text, brush14);
        palette12.setBrush(QPalette::Disabled, QPalette::BrightText, brush4);
        palette12.setBrush(QPalette::Disabled, QPalette::ButtonText, brush14);
        palette12.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette12.setBrush(QPalette::Disabled, QPalette::Window, brush11);
        palette12.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette12.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush11);
        palette12.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush8);
        palette12.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        previewCalibPushButton->setPalette(palette12);
        previewCalibPushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_3->addWidget(previewCalibPushButton, 4, 8, 1, 2);

        label_16 = new QLabel(frame_3);
        label_16->setObjectName(QStringLiteral("label_16"));

        gridLayout_3->addWidget(label_16, 0, 9, 1, 1);

        label_15 = new QLabel(frame_3);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout_3->addWidget(label_15, 0, 8, 1, 1);

        resultAVGWHITEPushButton = new QPushButton(frame_3);
        resultAVGWHITEPushButton->setObjectName(QStringLiteral("resultAVGWHITEPushButton"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(resultAVGWHITEPushButton->sizePolicy().hasHeightForWidth());
        resultAVGWHITEPushButton->setSizePolicy(sizePolicy4);
        resultAVGWHITEPushButton->setMinimumSize(QSize(120, 0));
        resultAVGWHITEPushButton->setMaximumSize(QSize(120, 16777215));
        resultAVGWHITEPushButton->setFocusPolicy(Qt::ClickFocus);
        resultAVGWHITEPushButton->setCheckable(true);
        resultAVGWHITEPushButton->setAutoExclusive(false);

        gridLayout_3->addWidget(resultAVGWHITEPushButton, 4, 11, 1, 1);

        horizontalSpacer_10 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_10, 2, 7, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(350, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_3, 4, 0, 1, 2);

        label_18 = new QLabel(frame_3);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setFont(font);

        gridLayout_3->addWidget(label_18, 0, 11, 1, 2);

        resultSDSSPushButton = new QPushButton(frame_3);
        resultSDSSPushButton->setObjectName(QStringLiteral("resultSDSSPushButton"));
        sizePolicy4.setHeightForWidth(resultSDSSPushButton->sizePolicy().hasHeightForWidth());
        resultSDSSPushButton->setSizePolicy(sizePolicy4);
        resultSDSSPushButton->setMinimumSize(QSize(120, 0));
        resultSDSSPushButton->setMaximumSize(QSize(120, 16777215));
        resultSDSSPushButton->setFocusPolicy(Qt::ClickFocus);
        resultSDSSPushButton->setCheckable(true);
        resultSDSSPushButton->setAutoExclusive(false);

        gridLayout_3->addWidget(resultSDSSPushButton, 2, 11, 1, 1);

        numAPASSLabel = new QLabel(frame_3);
        numAPASSLabel->setObjectName(QStringLiteral("numAPASSLabel"));
        sizePolicy2.setHeightForWidth(numAPASSLabel->sizePolicy().hasHeightForWidth());
        numAPASSLabel->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(numAPASSLabel, 3, 12, 1, 1);

        numSDSSLabel = new QLabel(frame_3);
        numSDSSLabel->setObjectName(QStringLiteral("numSDSSLabel"));
        sizePolicy2.setHeightForWidth(numSDSSLabel->sizePolicy().hasHeightForWidth());
        numSDSSLabel->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(numSDSSLabel, 2, 12, 1, 1);

        redFactorLineEdit = new QLineEdit(frame_3);
        redFactorLineEdit->setObjectName(QStringLiteral("redFactorLineEdit"));
        redFactorLineEdit->setMinimumSize(QSize(90, 0));
        redFactorLineEdit->setMaximumSize(QSize(90, 16777215));
        redFactorLineEdit->setClearButtonEnabled(false);

        gridLayout_3->addWidget(redFactorLineEdit, 1, 8, 1, 1);

        blueComboBox = new QComboBox(frame_3);
        blueComboBox->setObjectName(QStringLiteral("blueComboBox"));
        QSizePolicy sizePolicy5(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(blueComboBox->sizePolicy().hasHeightForWidth());
        blueComboBox->setSizePolicy(sizePolicy5);
        blueComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_3->addWidget(blueComboBox, 3, 1, 1, 6);

        DTLineEdit = new QLineEdit(frame_3);
        DTLineEdit->setObjectName(QStringLiteral("DTLineEdit"));
        DTLineEdit->setMinimumSize(QSize(80, 0));
        DTLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_3->addWidget(DTLineEdit, 4, 3, 1, 1);

        resultPANSTARRSPushButton = new QPushButton(frame_3);
        resultPANSTARRSPushButton->setObjectName(QStringLiteral("resultPANSTARRSPushButton"));
        sizePolicy4.setHeightForWidth(resultPANSTARRSPushButton->sizePolicy().hasHeightForWidth());
        resultPANSTARRSPushButton->setSizePolicy(sizePolicy4);
        resultPANSTARRSPushButton->setMinimumSize(QSize(120, 0));
        resultPANSTARRSPushButton->setMaximumSize(QSize(120, 16777215));
        resultPANSTARRSPushButton->setFocusPolicy(Qt::ClickFocus);
        resultPANSTARRSPushButton->setCheckable(true);
        resultPANSTARRSPushButton->setAutoExclusive(false);

        gridLayout_3->addWidget(resultPANSTARRSPushButton, 1, 11, 1, 1);

        blueLabel = new QLabel(frame_3);
        blueLabel->setObjectName(QStringLiteral("blueLabel"));
        sizePolicy2.setHeightForWidth(blueLabel->sizePolicy().hasHeightForWidth());
        blueLabel->setSizePolicy(sizePolicy2);
        QPalette palette13;
        QBrush brush22(QColor(0, 0, 255, 255));
        brush22.setStyle(Qt::SolidPattern);
        palette13.setBrush(QPalette::Active, QPalette::WindowText, brush22);
        palette13.setBrush(QPalette::Inactive, QPalette::WindowText, brush22);
        palette13.setBrush(QPalette::Disabled, QPalette::WindowText, brush19);
        blueLabel->setPalette(palette13);

        gridLayout_3->addWidget(blueLabel, 3, 0, 1, 1);

        greenErrorLineEdit = new QLineEdit(frame_3);
        greenErrorLineEdit->setObjectName(QStringLiteral("greenErrorLineEdit"));
        greenErrorLineEdit->setMinimumSize(QSize(90, 0));
        greenErrorLineEdit->setMaximumSize(QSize(90, 16777215));
        QPalette palette14;
        palette14.setBrush(QPalette::Active, QPalette::Base, brush20);
        QBrush brush23(QColor(255, 255, 221, 255));
        brush23.setStyle(Qt::SolidPattern);
        palette14.setBrush(QPalette::Active, QPalette::Window, brush23);
        palette14.setBrush(QPalette::Inactive, QPalette::Base, brush20);
        palette14.setBrush(QPalette::Inactive, QPalette::Window, brush23);
        palette14.setBrush(QPalette::Disabled, QPalette::Base, brush21);
        palette14.setBrush(QPalette::Disabled, QPalette::Window, brush23);
        greenErrorLineEdit->setPalette(palette14);
        greenErrorLineEdit->setFocusPolicy(Qt::NoFocus);
        greenErrorLineEdit->setAutoFillBackground(false);
        greenErrorLineEdit->setReadOnly(true);

        gridLayout_3->addWidget(greenErrorLineEdit, 2, 9, 1, 1);

        numPANSTARRSLabel = new QLabel(frame_3);
        numPANSTARRSLabel->setObjectName(QStringLiteral("numPANSTARRSLabel"));
        sizePolicy2.setHeightForWidth(numPANSTARRSLabel->sizePolicy().hasHeightForWidth());
        numPANSTARRSLabel->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(numPANSTARRSLabel, 1, 12, 1, 1);

        DMINLineEdit = new QLineEdit(frame_3);
        DMINLineEdit->setObjectName(QStringLiteral("DMINLineEdit"));
        DMINLineEdit->setMinimumSize(QSize(80, 0));
        DMINLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_3->addWidget(DMINLineEdit, 4, 5, 1, 1);

        greenComboBox = new QComboBox(frame_3);
        greenComboBox->setObjectName(QStringLiteral("greenComboBox"));
        sizePolicy5.setHeightForWidth(greenComboBox->sizePolicy().hasHeightForWidth());
        greenComboBox->setSizePolicy(sizePolicy5);
        greenComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_3->addWidget(greenComboBox, 2, 1, 1, 6);

        label_4 = new QLabel(frame_3);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_3->addWidget(label_4, 4, 4, 1, 1);

        numAVGWHITELabel = new QLabel(frame_3);
        numAVGWHITELabel->setObjectName(QStringLiteral("numAVGWHITELabel"));
        sizePolicy2.setHeightForWidth(numAVGWHITELabel->sizePolicy().hasHeightForWidth());
        numAVGWHITELabel->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(numAVGWHITELabel, 4, 12, 1, 1);

        horizontalSpacer_12 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_12, 2, 10, 1, 1);

        blueFactorLineEdit = new QLineEdit(frame_3);
        blueFactorLineEdit->setObjectName(QStringLiteral("blueFactorLineEdit"));
        blueFactorLineEdit->setMinimumSize(QSize(90, 0));
        blueFactorLineEdit->setMaximumSize(QSize(90, 16777215));
        blueFactorLineEdit->setAutoFillBackground(false);
        blueFactorLineEdit->setReadOnly(false);

        gridLayout_3->addWidget(blueFactorLineEdit, 3, 8, 1, 1);

        calibratePushButton = new QPushButton(frame_3);
        calibratePushButton->setObjectName(QStringLiteral("calibratePushButton"));
        QSizePolicy sizePolicy6(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(calibratePushButton->sizePolicy().hasHeightForWidth());
        calibratePushButton->setSizePolicy(sizePolicy6);
        calibratePushButton->setMinimumSize(QSize(150, 0));
        QPalette palette15;
        palette15.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette15.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette15.setBrush(QPalette::Active, QPalette::Light, brush12);
        palette15.setBrush(QPalette::Active, QPalette::Midlight, brush13);
        palette15.setBrush(QPalette::Active, QPalette::Dark, brush14);
        palette15.setBrush(QPalette::Active, QPalette::Mid, brush15);
        palette15.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette15.setBrush(QPalette::Active, QPalette::BrightText, brush4);
        palette15.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette15.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette15.setBrush(QPalette::Active, QPalette::Window, brush11);
        palette15.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette15.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
        palette15.setBrush(QPalette::Active, QPalette::ToolTipBase, brush8);
        palette15.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette15.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette15.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette15.setBrush(QPalette::Inactive, QPalette::Light, brush12);
        palette15.setBrush(QPalette::Inactive, QPalette::Midlight, brush13);
        palette15.setBrush(QPalette::Inactive, QPalette::Dark, brush14);
        palette15.setBrush(QPalette::Inactive, QPalette::Mid, brush15);
        palette15.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette15.setBrush(QPalette::Inactive, QPalette::BrightText, brush4);
        palette15.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette15.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette15.setBrush(QPalette::Inactive, QPalette::Window, brush11);
        palette15.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette15.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush16);
        palette15.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush8);
        palette15.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette15.setBrush(QPalette::Disabled, QPalette::WindowText, brush14);
        palette15.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette15.setBrush(QPalette::Disabled, QPalette::Light, brush12);
        palette15.setBrush(QPalette::Disabled, QPalette::Midlight, brush13);
        palette15.setBrush(QPalette::Disabled, QPalette::Dark, brush14);
        palette15.setBrush(QPalette::Disabled, QPalette::Mid, brush15);
        palette15.setBrush(QPalette::Disabled, QPalette::Text, brush14);
        palette15.setBrush(QPalette::Disabled, QPalette::BrightText, brush4);
        palette15.setBrush(QPalette::Disabled, QPalette::ButtonText, brush14);
        palette15.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette15.setBrush(QPalette::Disabled, QPalette::Window, brush11);
        palette15.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette15.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush11);
        palette15.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush8);
        palette15.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        calibratePushButton->setPalette(palette15);
        calibratePushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_3->addWidget(calibratePushButton, 4, 6, 1, 1);

        redComboBox = new QComboBox(frame_3);
        redComboBox->setObjectName(QStringLiteral("redComboBox"));
        sizePolicy5.setHeightForWidth(redComboBox->sizePolicy().hasHeightForWidth());
        redComboBox->setSizePolicy(sizePolicy5);
        redComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_3->addWidget(redComboBox, 1, 1, 1, 6);

        resultAPASSPushButton = new QPushButton(frame_3);
        resultAPASSPushButton->setObjectName(QStringLiteral("resultAPASSPushButton"));
        sizePolicy4.setHeightForWidth(resultAPASSPushButton->sizePolicy().hasHeightForWidth());
        resultAPASSPushButton->setSizePolicy(sizePolicy4);
        resultAPASSPushButton->setMinimumSize(QSize(120, 0));
        resultAPASSPushButton->setMaximumSize(QSize(120, 16777215));
        resultAPASSPushButton->setFocusPolicy(Qt::ClickFocus);
        resultAPASSPushButton->setCheckable(true);
        resultAPASSPushButton->setAutoExclusive(false);

        gridLayout_3->addWidget(resultAPASSPushButton, 3, 11, 1, 1);

        greenFactorLineEdit = new QLineEdit(frame_3);
        greenFactorLineEdit->setObjectName(QStringLiteral("greenFactorLineEdit"));
        greenFactorLineEdit->setMinimumSize(QSize(90, 0));
        greenFactorLineEdit->setMaximumSize(QSize(90, 16777215));
        QPalette palette16;
        palette16.setBrush(QPalette::Active, QPalette::Base, brush20);
        palette16.setBrush(QPalette::Inactive, QPalette::Base, brush20);
        palette16.setBrush(QPalette::Disabled, QPalette::Base, brush21);
        greenFactorLineEdit->setPalette(palette16);
        greenFactorLineEdit->setFocusPolicy(Qt::NoFocus);
        greenFactorLineEdit->setReadOnly(true);

        gridLayout_3->addWidget(greenFactorLineEdit, 2, 8, 1, 1);

        redErrorLineEdit = new QLineEdit(frame_3);
        redErrorLineEdit->setObjectName(QStringLiteral("redErrorLineEdit"));
        redErrorLineEdit->setMinimumSize(QSize(90, 0));
        redErrorLineEdit->setMaximumSize(QSize(90, 16777215));
        QPalette palette17;
        palette17.setBrush(QPalette::Active, QPalette::Base, brush20);
        palette17.setBrush(QPalette::Active, QPalette::Window, brush23);
        palette17.setBrush(QPalette::Inactive, QPalette::Base, brush20);
        palette17.setBrush(QPalette::Inactive, QPalette::Window, brush23);
        palette17.setBrush(QPalette::Disabled, QPalette::Base, brush21);
        palette17.setBrush(QPalette::Disabled, QPalette::Window, brush23);
        redErrorLineEdit->setPalette(palette17);
        redErrorLineEdit->setFocusPolicy(Qt::NoFocus);
        redErrorLineEdit->setAutoFillBackground(false);
        redErrorLineEdit->setReadOnly(true);

        gridLayout_3->addWidget(redErrorLineEdit, 1, 9, 1, 1);

        greenLabel = new QLabel(frame_3);
        greenLabel->setObjectName(QStringLiteral("greenLabel"));
        sizePolicy2.setHeightForWidth(greenLabel->sizePolicy().hasHeightForWidth());
        greenLabel->setSizePolicy(sizePolicy2);
        QPalette palette18;
        QBrush brush24(QColor(0, 153, 0, 255));
        brush24.setStyle(Qt::SolidPattern);
        palette18.setBrush(QPalette::Active, QPalette::WindowText, brush24);
        palette18.setBrush(QPalette::Inactive, QPalette::WindowText, brush24);
        palette18.setBrush(QPalette::Disabled, QPalette::WindowText, brush19);
        greenLabel->setPalette(palette18);

        gridLayout_3->addWidget(greenLabel, 2, 0, 1, 1);

        label_5 = new QLabel(frame_3);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_3->addWidget(label_5, 4, 2, 1, 1);

        label_10 = new QLabel(tab_3);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(9, 241, 351, 20));
        label_10->setFont(font);
        statisticsFrame = new QFrame(tab_3);
        statisticsFrame->setObjectName(QStringLiteral("statisticsFrame"));
        statisticsFrame->setGeometry(QRect(9, 267, 881, 274));
        QSizePolicy sizePolicy7(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(statisticsFrame->sizePolicy().hasHeightForWidth());
        statisticsFrame->setSizePolicy(sizePolicy7);
        QPalette palette19;
        palette19.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush25(QColor(236, 236, 236, 255));
        brush25.setStyle(Qt::SolidPattern);
        palette19.setBrush(QPalette::Active, QPalette::Window, brush25);
        palette19.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette19.setBrush(QPalette::Inactive, QPalette::Window, brush25);
        palette19.setBrush(QPalette::Disabled, QPalette::Base, brush25);
        palette19.setBrush(QPalette::Disabled, QPalette::Window, brush25);
        statisticsFrame->setPalette(palette19);
        statisticsFrame->setAutoFillBackground(true);
        statisticsFrame->setFrameShape(QFrame::Panel);
        statisticsFrame->setFrameShadow(QFrame::Sunken);
        statisticsFrame->setLineWidth(2);
        gridLayout_8 = new QGridLayout(statisticsFrame);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        horizontalSpacer_5 = new QSpacerItem(536, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_5, 2, 0, 1, 2);

        statisticsTableWidget = new QTableWidget(statisticsFrame);
        statisticsTableWidget->setObjectName(QStringLiteral("statisticsTableWidget"));
        QSizePolicy sizePolicy8(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(statisticsTableWidget->sizePolicy().hasHeightForWidth());
        statisticsTableWidget->setSizePolicy(sizePolicy8);
        statisticsTableWidget->setFocusPolicy(Qt::ClickFocus);
        statisticsTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        statisticsTableWidget->verticalHeader()->setVisible(false);

        gridLayout_8->addWidget(statisticsTableWidget, 1, 0, 1, 4);

        createTiffPushButton = new QPushButton(statisticsFrame);
        createTiffPushButton->setObjectName(QStringLiteral("createTiffPushButton"));
        sizePolicy6.setHeightForWidth(createTiffPushButton->sizePolicy().hasHeightForWidth());
        createTiffPushButton->setSizePolicy(sizePolicy6);
        QPalette palette20;
        palette20.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette20.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette20.setBrush(QPalette::Active, QPalette::Light, brush12);
        palette20.setBrush(QPalette::Active, QPalette::Midlight, brush13);
        palette20.setBrush(QPalette::Active, QPalette::Dark, brush14);
        palette20.setBrush(QPalette::Active, QPalette::Mid, brush15);
        palette20.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette20.setBrush(QPalette::Active, QPalette::BrightText, brush4);
        palette20.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette20.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette20.setBrush(QPalette::Active, QPalette::Window, brush11);
        palette20.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette20.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
        palette20.setBrush(QPalette::Active, QPalette::ToolTipBase, brush8);
        palette20.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette20.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette20.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette20.setBrush(QPalette::Inactive, QPalette::Light, brush12);
        palette20.setBrush(QPalette::Inactive, QPalette::Midlight, brush13);
        palette20.setBrush(QPalette::Inactive, QPalette::Dark, brush14);
        palette20.setBrush(QPalette::Inactive, QPalette::Mid, brush15);
        palette20.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette20.setBrush(QPalette::Inactive, QPalette::BrightText, brush4);
        palette20.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette20.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette20.setBrush(QPalette::Inactive, QPalette::Window, brush11);
        palette20.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette20.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush16);
        palette20.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush8);
        palette20.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette20.setBrush(QPalette::Disabled, QPalette::WindowText, brush14);
        palette20.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette20.setBrush(QPalette::Disabled, QPalette::Light, brush12);
        palette20.setBrush(QPalette::Disabled, QPalette::Midlight, brush13);
        palette20.setBrush(QPalette::Disabled, QPalette::Dark, brush14);
        palette20.setBrush(QPalette::Disabled, QPalette::Mid, brush15);
        palette20.setBrush(QPalette::Disabled, QPalette::Text, brush14);
        palette20.setBrush(QPalette::Disabled, QPalette::BrightText, brush4);
        palette20.setBrush(QPalette::Disabled, QPalette::ButtonText, brush14);
        palette20.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette20.setBrush(QPalette::Disabled, QPalette::Window, brush11);
        palette20.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette20.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush11);
        palette20.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush8);
        palette20.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        createTiffPushButton->setPalette(palette20);
        createTiffPushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_8->addWidget(createTiffPushButton, 2, 3, 1, 1);

        label_9 = new QLabel(statisticsFrame);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setFont(font);

        gridLayout_8->addWidget(label_9, 0, 0, 1, 1);

        getStatisticsPushButton = new QPushButton(statisticsFrame);
        getStatisticsPushButton->setObjectName(QStringLiteral("getStatisticsPushButton"));
        sizePolicy6.setHeightForWidth(getStatisticsPushButton->sizePolicy().hasHeightForWidth());
        getStatisticsPushButton->setSizePolicy(sizePolicy6);
        QPalette palette21;
        palette21.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette21.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette21.setBrush(QPalette::Active, QPalette::Light, brush12);
        palette21.setBrush(QPalette::Active, QPalette::Midlight, brush13);
        palette21.setBrush(QPalette::Active, QPalette::Dark, brush14);
        palette21.setBrush(QPalette::Active, QPalette::Mid, brush15);
        palette21.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette21.setBrush(QPalette::Active, QPalette::BrightText, brush4);
        palette21.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette21.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette21.setBrush(QPalette::Active, QPalette::Window, brush11);
        palette21.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette21.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
        palette21.setBrush(QPalette::Active, QPalette::ToolTipBase, brush8);
        palette21.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette21.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette21.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette21.setBrush(QPalette::Inactive, QPalette::Light, brush12);
        palette21.setBrush(QPalette::Inactive, QPalette::Midlight, brush13);
        palette21.setBrush(QPalette::Inactive, QPalette::Dark, brush14);
        palette21.setBrush(QPalette::Inactive, QPalette::Mid, brush15);
        palette21.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette21.setBrush(QPalette::Inactive, QPalette::BrightText, brush4);
        palette21.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette21.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette21.setBrush(QPalette::Inactive, QPalette::Window, brush11);
        palette21.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette21.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush16);
        palette21.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush8);
        palette21.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette21.setBrush(QPalette::Disabled, QPalette::WindowText, brush14);
        palette21.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette21.setBrush(QPalette::Disabled, QPalette::Light, brush12);
        palette21.setBrush(QPalette::Disabled, QPalette::Midlight, brush13);
        palette21.setBrush(QPalette::Disabled, QPalette::Dark, brush14);
        palette21.setBrush(QPalette::Disabled, QPalette::Mid, brush15);
        palette21.setBrush(QPalette::Disabled, QPalette::Text, brush14);
        palette21.setBrush(QPalette::Disabled, QPalette::BrightText, brush4);
        palette21.setBrush(QPalette::Disabled, QPalette::ButtonText, brush14);
        palette21.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette21.setBrush(QPalette::Disabled, QPalette::Window, brush11);
        palette21.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette21.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush11);
        palette21.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush8);
        palette21.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        getStatisticsPushButton->setPalette(palette21);
        getStatisticsPushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_8->addWidget(getStatisticsPushButton, 2, 2, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(218, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_7, 0, 1, 1, 3);

        tabWidget->addTab(tab_3, QString());

        gridLayout_7->addWidget(tabWidget, 0, 0, 1, 1);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font);

        gridLayout_7->addWidget(label_3, 1, 0, 1, 1);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        processingTextEdit = new QPlainTextEdit(centralwidget);
        processingTextEdit->setObjectName(QStringLiteral("processingTextEdit"));
        QSizePolicy sizePolicy9(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(processingTextEdit->sizePolicy().hasHeightForWidth());
        processingTextEdit->setSizePolicy(sizePolicy9);
        processingTextEdit->setMinimumSize(QSize(500, 0));
        processingTextEdit->setMaximumSize(QSize(16777215, 200));
        processingTextEdit->setFrameShape(QFrame::Panel);
        processingTextEdit->setReadOnly(true);

        gridLayout_6->addWidget(processingTextEdit, 0, 0, 2, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_6, 0, 1, 1, 1);

        abortPushButton = new QPushButton(centralwidget);
        abortPushButton->setObjectName(QStringLiteral("abortPushButton"));
        abortPushButton->setMaximumSize(QSize(120, 16777215));
        QPalette palette22;
        palette22.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        QBrush brush26(QColor(223, 77, 123, 255));
        brush26.setStyle(Qt::SolidPattern);
        palette22.setBrush(QPalette::Active, QPalette::Button, brush26);
        QBrush brush27(QColor(255, 168, 195, 255));
        brush27.setStyle(Qt::SolidPattern);
        palette22.setBrush(QPalette::Active, QPalette::Light, brush27);
        QBrush brush28(QColor(239, 122, 159, 255));
        brush28.setStyle(Qt::SolidPattern);
        palette22.setBrush(QPalette::Active, QPalette::Midlight, brush28);
        QBrush brush29(QColor(111, 38, 61, 255));
        brush29.setStyle(Qt::SolidPattern);
        palette22.setBrush(QPalette::Active, QPalette::Dark, brush29);
        QBrush brush30(QColor(149, 51, 82, 255));
        brush30.setStyle(Qt::SolidPattern);
        palette22.setBrush(QPalette::Active, QPalette::Mid, brush30);
        palette22.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette22.setBrush(QPalette::Active, QPalette::BrightText, brush4);
        palette22.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette22.setBrush(QPalette::Active, QPalette::Base, brush4);
        palette22.setBrush(QPalette::Active, QPalette::Window, brush26);
        palette22.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        QBrush brush31(QColor(239, 166, 189, 255));
        brush31.setStyle(Qt::SolidPattern);
        palette22.setBrush(QPalette::Active, QPalette::AlternateBase, brush31);
        palette22.setBrush(QPalette::Active, QPalette::ToolTipBase, brush8);
        palette22.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette22.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette22.setBrush(QPalette::Inactive, QPalette::Button, brush26);
        palette22.setBrush(QPalette::Inactive, QPalette::Light, brush27);
        palette22.setBrush(QPalette::Inactive, QPalette::Midlight, brush28);
        palette22.setBrush(QPalette::Inactive, QPalette::Dark, brush29);
        palette22.setBrush(QPalette::Inactive, QPalette::Mid, brush30);
        palette22.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette22.setBrush(QPalette::Inactive, QPalette::BrightText, brush4);
        palette22.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette22.setBrush(QPalette::Inactive, QPalette::Base, brush4);
        palette22.setBrush(QPalette::Inactive, QPalette::Window, brush26);
        palette22.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette22.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush31);
        palette22.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush8);
        palette22.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette22.setBrush(QPalette::Disabled, QPalette::WindowText, brush29);
        palette22.setBrush(QPalette::Disabled, QPalette::Button, brush26);
        palette22.setBrush(QPalette::Disabled, QPalette::Light, brush27);
        palette22.setBrush(QPalette::Disabled, QPalette::Midlight, brush28);
        palette22.setBrush(QPalette::Disabled, QPalette::Dark, brush29);
        palette22.setBrush(QPalette::Disabled, QPalette::Mid, brush30);
        palette22.setBrush(QPalette::Disabled, QPalette::Text, brush29);
        palette22.setBrush(QPalette::Disabled, QPalette::BrightText, brush4);
        palette22.setBrush(QPalette::Disabled, QPalette::ButtonText, brush29);
        palette22.setBrush(QPalette::Disabled, QPalette::Base, brush26);
        palette22.setBrush(QPalette::Disabled, QPalette::Window, brush26);
        palette22.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette22.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush26);
        palette22.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush8);
        palette22.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        abortPushButton->setPalette(palette22);

        gridLayout_6->addWidget(abortPushButton, 0, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer, 0, 3, 1, 1);


        gridLayout_7->addLayout(gridLayout_6, 2, 0, 1, 1);

        ColorPicture->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ColorPicture);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 989, 23));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        ColorPicture->setMenuBar(menubar);
        statusbar = new QStatusBar(ColorPicture);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        ColorPicture->setStatusBar(statusbar);
        QWidget::setTabOrder(dirLineEdit, narrowbandWeightLineEdit);
        QWidget::setTabOrder(narrowbandWeightLineEdit, redFactorLineEdit);
        QWidget::setTabOrder(redFactorLineEdit, blueFactorLineEdit);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionClose);

        retranslateUi(ColorPicture);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(ColorPicture);
    } // setupUi

    void retranslateUi(QMainWindow *ColorPicture)
    {
        ColorPicture->setWindowTitle(QApplication::translate("ColorPicture", "Photometric color calibration of RGB images", Q_NULLPTR));
        actionClose->setText(QApplication::translate("ColorPicture", "Close", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        tabWidget->setStatusTip(QApplication::translate("ColorPicture", "Performs the photometric color calibration method", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        selectDirPushButton->setStatusTip(QApplication::translate("ColorPicture", "Coadditions will be searched for in all sub-directories (looking for coadd_* directories)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        selectDirPushButton->setText(QApplication::translate("ColorPicture", "Select parent directory ...", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        dirLineEdit->setStatusTip(QApplication::translate("ColorPicture", "Coadditions will be searched for in all sub-directories (looking for coadd_* directories)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label->setText(QApplication::translate("ColorPicture", "(1) Select coadds for cropping", Q_NULLPTR));
        titleLabel->setText(QApplication::translate("ColorPicture", "(2) Optional: Combine broad- and narrowband images", Q_NULLPTR));
        label_19->setText(QApplication::translate("ColorPicture", "Selected and cropped coadded images", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        coaddImageListView->setStatusTip(QApplication::translate("ColorPicture", "The cropped coadded images available for calibration (stored in the color_theli/ subdirectory)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        coaddDirListView->setStatusTip(QApplication::translate("ColorPicture", "Click, or ctrl-click to select all coadditions needed for the color picture.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        clearCoaddsPushButton->setStatusTip(QApplication::translate("ColorPicture", "Clears the list of directories.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        clearCoaddsPushButton->setText(QApplication::translate("ColorPicture", "Clear", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        getCoaddsPushButton->setStatusTip(QApplication::translate("ColorPicture", "Crops coadded images to the same size, and stores them in a color_theli/ sub-directory.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        getCoaddsPushButton->setText(QApplication::translate("ColorPicture", "Crop coadded images", Q_NULLPTR));
        subtitle2Label->setText(QApplication::translate("ColorPicture", "Narrowband", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        narrowbandComboBox->setStatusTip(QApplication::translate("ColorPicture", "Select the narrowband image", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        subtitle1Label->setText(QApplication::translate("ColorPicture", "Broadband ", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        broadbandComboBox->setStatusTip(QApplication::translate("ColorPicture", "Select the broadband image", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        getRatioPushButton->setStatusTip(QApplication::translate("ColorPicture", "Measures the median flux ratio of the narrow and broadband filters (NB / BB). Requires PHOTINSTRU_KEY = FILTER for scamp!", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        getRatioPushButton->setText(QApplication::translate("ColorPicture", "(1) Estimate effective ratio", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        filterRatioLineEdit->setStatusTip(QApplication::translate("ColorPicture", "The estimated ratio of the effective filter widths (narrow / broad)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        filterRatioLineEdit->setText(QApplication::translate("ColorPicture", "0", Q_NULLPTR));
        label_6->setText(QApplication::translate("ColorPicture", "Relative weight of narrowband image", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        narrowbandWeightLineEdit->setStatusTip(QApplication::translate("ColorPicture", "Default: 1.0. Larger values (1.5, 2, ...) increase the narrowband contribution while maintaining stellar fluxes.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        narrowbandWeightLineEdit->setText(QApplication::translate("ColorPicture", "1.0", Q_NULLPTR));
        textEdit->setHtml(QApplication::translate("ColorPicture", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Noto Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">Combines narrow- and broadband images to enhance nebular features, e.g. an H-alpha image combined with a red image. The combined images are added to the list shown at the left. </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">The ratio of the effective bandpass widths is essential to preserve the stars' colors. The coadditions must be based on a 'Scamp' run with PHOTINSTRU_KEY = FILTER, otherwise the relative "
                        "flux levels are not preserved.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">The calculation is as follows (w = weight, r = ratio)</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">Image</span><span style=\" font-size:12pt; vertical-align:sub;\">combined</span><span style=\" font-size:12pt;\"> = (1 - w * r) * Image</span><span style=\" font-size:12pt; vertical-align:sub;\">broad</span><span style=\" font-size:12pt;\"> + w * I"
                        "mage</span><span style=\" font-size:12pt; vertical-align:sub;\">narrow</span></p></body></html>", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        BBNBcombinePushButton->setStatusTip(QApplication::translate("ColorPicture", "Combines broad- and narrowband images and adds them to the list of coadded images.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        BBNBcombinePushButton->setText(QApplication::translate("ColorPicture", "(2) Combine images", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("ColorPicture", "Preparation", Q_NULLPTR));
        label_2->setText(QApplication::translate("ColorPicture", "(3) Calibrate images against photometric reference catalogs", Q_NULLPTR));
        redLabel->setText(QApplication::translate("ColorPicture", "Red image", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        blueErrorLineEdit->setStatusTip(QApplication::translate("ColorPicture", "The statistical uncertainty of the blue factor", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        previewCalibPushButton->setText(QApplication::translate("ColorPicture", "(2) Preview RGB image", Q_NULLPTR));
        label_16->setText(QApplication::translate("ColorPicture", "Uncertainty", Q_NULLPTR));
        label_15->setText(QApplication::translate("ColorPicture", "Factor", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        resultAVGWHITEPushButton->setStatusTip(QApplication::translate("ColorPicture", "Shows average white correction factors", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        resultAVGWHITEPushButton->setText(QApplication::translate("ColorPicture", "AVGWHITE", Q_NULLPTR));
        label_18->setText(QApplication::translate("ColorPicture", "Choose calibration result", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        resultSDSSPushButton->setStatusTip(QApplication::translate("ColorPicture", "Shows the SDSS result", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        resultSDSSPushButton->setText(QApplication::translate("ColorPicture", "SDSS", Q_NULLPTR));
        numAPASSLabel->setText(QApplication::translate("ColorPicture", "? stars", Q_NULLPTR));
        numSDSSLabel->setText(QApplication::translate("ColorPicture", "? stars", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        redFactorLineEdit->setStatusTip(QApplication::translate("ColorPicture", "Adjust the red factor (relative to green)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        redFactorLineEdit->setText(QString());
#ifndef QT_NO_STATUSTIP
        blueComboBox->setStatusTip(QApplication::translate("ColorPicture", "Select the image that will form the blue channel of the RGB image", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        DTLineEdit->setStatusTip(QApplication::translate("ColorPicture", "Detection threshold per pixel", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        DTLineEdit->setText(QApplication::translate("ColorPicture", "3.0", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        resultPANSTARRSPushButton->setStatusTip(QApplication::translate("ColorPicture", "Shows the PANSTARRS result", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        resultPANSTARRSPushButton->setText(QApplication::translate("ColorPicture", "PANSTARRS", Q_NULLPTR));
        blueLabel->setText(QApplication::translate("ColorPicture", "Blue image", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        greenErrorLineEdit->setStatusTip(QApplication::translate("ColorPicture", "The uncertainty of the green factor (fixed)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        greenErrorLineEdit->setText(QApplication::translate("ColorPicture", "0.0", Q_NULLPTR));
        numPANSTARRSLabel->setText(QApplication::translate("ColorPicture", "? stars", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        DMINLineEdit->setStatusTip(QApplication::translate("ColorPicture", "Minimum number of connected pixels above the detection threshold", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        DMINLineEdit->setText(QApplication::translate("ColorPicture", "5", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        greenComboBox->setStatusTip(QApplication::translate("ColorPicture", "Select the image that will form the green channel of the RGB image", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_4->setText(QApplication::translate("ColorPicture", "DMIN", Q_NULLPTR));
        numAVGWHITELabel->setText(QApplication::translate("ColorPicture", "? stars", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        blueFactorLineEdit->setStatusTip(QApplication::translate("ColorPicture", "Adjust the blue factor (relative to green)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        blueFactorLineEdit->setText(QString());
#ifndef QT_NO_TOOLTIP
        calibratePushButton->setToolTip(QApplication::translate("ColorPicture", "This may take several minutes", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        calibratePushButton->setStatusTip(QApplication::translate("ColorPicture", "Determine the correction factors so that solar-type stars are displayed white.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        calibratePushButton->setText(QApplication::translate("ColorPicture", "(1) Calibrate", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        redComboBox->setStatusTip(QApplication::translate("ColorPicture", "Select the image that will form the red channel of the RGB image", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        resultAPASSPushButton->setStatusTip(QApplication::translate("ColorPicture", "Shows the APASS result", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        resultAPASSPushButton->setText(QApplication::translate("ColorPicture", "APASS", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        greenFactorLineEdit->setStatusTip(QApplication::translate("ColorPicture", "The green factor is fixed", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        greenFactorLineEdit->setText(QApplication::translate("ColorPicture", "1.0", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        redErrorLineEdit->setStatusTip(QApplication::translate("ColorPicture", "The statistical uncertainty of the red factor", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        greenLabel->setText(QApplication::translate("ColorPicture", "Green image", Q_NULLPTR));
        label_5->setText(QApplication::translate("ColorPicture", "DT", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        label_10->setStatusTip(QApplication::translate("ColorPicture", "The min and max levels will form the black and white points, respectively", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_10->setText(QApplication::translate("ColorPicture", "(4) Adjust dynamic range for the TIFF images", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        statisticsTableWidget->setStatusTip(QApplication::translate("ColorPicture", "Adjust the thresholds according to your needs", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        createTiffPushButton->setStatusTip(QApplication::translate("ColorPicture", "Stores 16bit TIFF images for each channel under color_theli/", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        createTiffPushButton->setText(QApplication::translate("ColorPicture", "(4) Create TIFFs", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        label_9->setStatusTip(QApplication::translate("ColorPicture", "The min and max levels will form the black and white points, respectively", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_9->setText(QApplication::translate("ColorPicture", "Edit min and max as required", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        getStatisticsPushButton->setStatusTip(QApplication::translate("ColorPicture", "Select a region with empty sky (middle-click drag) in an image", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        getStatisticsPushButton->setText(QApplication::translate("ColorPicture", "(3) Estimate background level", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("ColorPicture", "Photometric color calibration", Q_NULLPTR));
        label_3->setText(QApplication::translate("ColorPicture", "System output", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        processingTextEdit->setStatusTip(QApplication::translate("ColorPicture", "Shows processing messages", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        abortPushButton->setStatusTip(QApplication::translate("ColorPicture", "Lets the current task finish gracefully, then stops before executing further tasks in the queue.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        abortPushButton->setText(QApplication::translate("ColorPicture", "Abort", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("ColorPicture", "File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ColorPicture: public Ui_ColorPicture {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLORPICTURE_H
