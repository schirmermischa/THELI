/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
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
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionCreate_new_instrument;
    QAction *actioniView;
    QAction *actionImage_statistics;
    QAction *actionColor_picture;
    QAction *actionNew_instrument;
    QAction *actionAbsZP;
    QAction *actionClose;
    QAction *actionWindowsConfiguration;
    QAction *actionWindowsMonitor;
    QAction *actionPreferences;
    QAction *actionEdit_preferences;
    QAction *actionAdd_new_instrument_configuration;
    QAction *actionKill;
    QAction *actionAbout;
    QAction *actionLicense;
    QAction *actionAcknowledging;
    QWidget *centralWidget;
    QGridLayout *gridLayout_4;
    QFrame *frame;
    QGridLayout *gridLayout_5;
    QToolButton *setupProjectResetDataToolButton;
    QLabel *setupProjectLabel;
    QToolButton *setupProjectLoadToolButton;
    QLabel *setupProjectLabel_2;
    QLineEdit *setupProjectLineEdit;
    QToolButton *setupProjectResetToolButton;
    QComboBox *setupInstrumentComboBox;
    QSpacerItem *verticalSpacer_24;
    QPlainTextEdit *plainTextEdit;
    QSpacerItem *verticalSpacer_22;
    QHBoxLayout *horizontalLayout;
    QPushButton *startPushButton;
    QToolButton *yieldToolButton;
    QToolButton *stopToolButton;
    QSpacerItem *horizontalSpacer_11;
    QProgressBar *processProgressBar;
    QTabWidget *processingTabWidget;
    QWidget *processingTabWidgetPage1;
    QGridLayout *gridLayout;
    QLabel *setupMainLabel;
    QPushButton *setupReadmePushButton;
    QLineEdit *setupMainLineEdit;
    QLabel *setupBiasLabel;
    QLineEdit *setupBiasLineEdit;
    QLabel *setupDarkLabel;
    QLineEdit *setupDarkLineEdit;
    QLabel *setupFlatoffLabel;
    QLineEdit *setupFlatoffLineEdit;
    QLabel *setupFlatLabel;
    QLineEdit *setupFlatLineEdit;
    QLabel *setupScienceLabel;
    QLineEdit *setupScienceLineEdit;
    QLabel *setupSkyLabel;
    QLineEdit *setupSkyLineEdit;
    QLabel *setupStandardLabel;
    QLineEdit *setupStandardLineEdit;
    QSpacerItem *verticalSpacer;
    QWidget *processingTabWidgetPage2;
    QGridLayout *gridLayout_3;
    QCheckBox *applyCollapseCheckBox;
    QToolButton *binnedPreviewConfigureToolButton;
    QCheckBox *applyBinnedpreviewCheckBox;
    QSpacerItem *verticalSpacer_2;
    QCheckBox *applyHDUreformatCheckBox;
    QCheckBox *applyProcessbiasCheckBox;
    QToolButton *calibratorConfigureToolButton;
    QToolButton *HDUreformatConfigureToolButton;
    QCheckBox *applyProcessdarkCheckBox;
    QCheckBox *applyProcessflatoffCheckBox;
    QCheckBox *applyProcessflatCheckBox;
    QCheckBox *applyProcessscienceCheckBox;
    QToolButton *chopnodConfigureToolButton;
    QCheckBox *applyChopnodCheckBox;
    QToolButton *BACconfigureToolButton;
    QCheckBox *applyBackgroundCheckBox;
    QToolButton *COCconfigureToolButton;
    QWidget *processingTabWidgetPage3;
    QGridLayout *gridLayout_2;
    QToolButton *globalweightConfigureToolButton;
    QCheckBox *applyGlobalweightCheckBox;
    QToolButton *individualweightConfigureToolButton;
    QCheckBox *applyIndividualweightCheckBox;
    QToolButton *separateConfigureToolButton;
    QCheckBox *applySeparateCheckBox;
    QToolButton *absphotomindirectConfigureToolButton;
    QCheckBox *applyAbsphotindirectCheckBox;
    QToolButton *createsourcecatConfigureToolButton;
    QCheckBox *applyCreatesourcecatCheckBox;
    QToolButton *astromphotomConfigureToolButton;
    QCheckBox *applyAstromphotomCheckBox;
    QToolButton *absphotomdirectConfigureToolButton;
    QCheckBox *applyStarflatCheckBox;
    QToolButton *skysubConfigureToolButton;
    QCheckBox *applySkysubCheckBox;
    QToolButton *coadditionConfigureToolButton;
    QCheckBox *applyCoadditionCheckBox;
    QSpacerItem *verticalSpacer_3;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuAbout;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(468, 736);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setDockOptions(QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks|QMainWindow::GroupedDragging);
        actionCreate_new_instrument = new QAction(MainWindow);
        actionCreate_new_instrument->setObjectName(QStringLiteral("actionCreate_new_instrument"));
        actioniView = new QAction(MainWindow);
        actioniView->setObjectName(QStringLiteral("actioniView"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/iview.png"), QSize(), QIcon::Normal, QIcon::Off);
        actioniView->setIcon(icon);
        actionImage_statistics = new QAction(MainWindow);
        actionImage_statistics->setObjectName(QStringLiteral("actionImage_statistics"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/sigma.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionImage_statistics->setIcon(icon1);
        actionColor_picture = new QAction(MainWindow);
        actionColor_picture->setObjectName(QStringLiteral("actionColor_picture"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/color.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionColor_picture->setIcon(icon2);
        actionNew_instrument = new QAction(MainWindow);
        actionNew_instrument->setObjectName(QStringLiteral("actionNew_instrument"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/addInst.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_instrument->setIcon(icon3);
        actionAbsZP = new QAction(MainWindow);
        actionAbsZP->setObjectName(QStringLiteral("actionAbsZP"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/abszp.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAbsZP->setIcon(icon4);
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        actionWindowsConfiguration = new QAction(MainWindow);
        actionWindowsConfiguration->setObjectName(QStringLiteral("actionWindowsConfiguration"));
        actionWindowsConfiguration->setCheckable(true);
        actionWindowsMonitor = new QAction(MainWindow);
        actionWindowsMonitor->setObjectName(QStringLiteral("actionWindowsMonitor"));
        actionWindowsMonitor->setCheckable(true);
        actionPreferences = new QAction(MainWindow);
        actionPreferences->setObjectName(QStringLiteral("actionPreferences"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icons/settings-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPreferences->setIcon(icon5);
        actionEdit_preferences = new QAction(MainWindow);
        actionEdit_preferences->setObjectName(QStringLiteral("actionEdit_preferences"));
        actionEdit_preferences->setIcon(icon5);
        actionAdd_new_instrument_configuration = new QAction(MainWindow);
        actionAdd_new_instrument_configuration->setObjectName(QStringLiteral("actionAdd_new_instrument_configuration"));
        actionAdd_new_instrument_configuration->setIcon(icon3);
        actionKill = new QAction(MainWindow);
        actionKill->setObjectName(QStringLiteral("actionKill"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/icons/bones_final.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionKill->setIcon(icon6);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionLicense = new QAction(MainWindow);
        actionLicense->setObjectName(QStringLiteral("actionLicense"));
        actionAcknowledging = new QAction(MainWindow);
        actionAcknowledging->setObjectName(QStringLiteral("actionAcknowledging"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        gridLayout_4 = new QGridLayout(centralWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy2);
        QPalette palette;
        QBrush brush(QColor(252, 252, 252, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(165, 205, 208, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        frame->setPalette(palette);
        frame->setAutoFillBackground(true);
        frame->setFrameShape(QFrame::Panel);
        frame->setFrameShadow(QFrame::Raised);
        frame->setLineWidth(1);
        gridLayout_5 = new QGridLayout(frame);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        setupProjectResetDataToolButton = new QToolButton(frame);
        setupProjectResetDataToolButton->setObjectName(QStringLiteral("setupProjectResetDataToolButton"));
        setupProjectResetDataToolButton->setMinimumSize(QSize(40, 40));
        setupProjectResetDataToolButton->setMaximumSize(QSize(40, 40));
        setupProjectResetDataToolButton->setFocusPolicy(Qt::ClickFocus);
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/icons/db-reset.png"), QSize(), QIcon::Normal, QIcon::Off);
        setupProjectResetDataToolButton->setIcon(icon7);
        setupProjectResetDataToolButton->setIconSize(QSize(48, 48));
        setupProjectResetDataToolButton->setAutoRaise(true);

        gridLayout_5->addWidget(setupProjectResetDataToolButton, 0, 5, 1, 1);

        setupProjectLabel = new QLabel(frame);
        setupProjectLabel->setObjectName(QStringLiteral("setupProjectLabel"));
        sizePolicy.setHeightForWidth(setupProjectLabel->sizePolicy().hasHeightForWidth());
        setupProjectLabel->setSizePolicy(sizePolicy);
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        setupProjectLabel->setFont(font);
        setupProjectLabel->setWordWrap(false);

        gridLayout_5->addWidget(setupProjectLabel, 0, 0, 1, 1);

        setupProjectLoadToolButton = new QToolButton(frame);
        setupProjectLoadToolButton->setObjectName(QStringLiteral("setupProjectLoadToolButton"));
        setupProjectLoadToolButton->setMinimumSize(QSize(40, 40));
        setupProjectLoadToolButton->setMaximumSize(QSize(40, 40));
        setupProjectLoadToolButton->setFocusPolicy(Qt::ClickFocus);
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/icons/open_project.png"), QSize(), QIcon::Normal, QIcon::Off);
        setupProjectLoadToolButton->setIcon(icon8);
        setupProjectLoadToolButton->setIconSize(QSize(48, 48));
        setupProjectLoadToolButton->setCheckable(false);
        setupProjectLoadToolButton->setAutoExclusive(true);
        setupProjectLoadToolButton->setAutoRaise(true);

        gridLayout_5->addWidget(setupProjectLoadToolButton, 0, 3, 1, 1);

        setupProjectLabel_2 = new QLabel(frame);
        setupProjectLabel_2->setObjectName(QStringLiteral("setupProjectLabel_2"));
        sizePolicy.setHeightForWidth(setupProjectLabel_2->sizePolicy().hasHeightForWidth());
        setupProjectLabel_2->setSizePolicy(sizePolicy);
        setupProjectLabel_2->setFont(font);
        setupProjectLabel_2->setWordWrap(false);

        gridLayout_5->addWidget(setupProjectLabel_2, 1, 0, 1, 1);

        setupProjectLineEdit = new QLineEdit(frame);
        setupProjectLineEdit->setObjectName(QStringLiteral("setupProjectLineEdit"));
        setupProjectLineEdit->setFocusPolicy(Qt::ClickFocus);

        gridLayout_5->addWidget(setupProjectLineEdit, 0, 1, 1, 1);

        setupProjectResetToolButton = new QToolButton(frame);
        setupProjectResetToolButton->setObjectName(QStringLiteral("setupProjectResetToolButton"));
        setupProjectResetToolButton->setMinimumSize(QSize(40, 40));
        setupProjectResetToolButton->setMaximumSize(QSize(40, 40));
        setupProjectResetToolButton->setFocusPolicy(Qt::ClickFocus);
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/icons/db-restart-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        setupProjectResetToolButton->setIcon(icon9);
        setupProjectResetToolButton->setIconSize(QSize(49, 49));
        setupProjectResetToolButton->setCheckable(false);
        setupProjectResetToolButton->setAutoExclusive(true);
        setupProjectResetToolButton->setAutoRaise(true);

        gridLayout_5->addWidget(setupProjectResetToolButton, 0, 6, 1, 1);

        setupInstrumentComboBox = new QComboBox(frame);
        setupInstrumentComboBox->setObjectName(QStringLiteral("setupInstrumentComboBox"));
        setupInstrumentComboBox->setFocusPolicy(Qt::NoFocus);
        setupInstrumentComboBox->setMaxVisibleItems(20);

        gridLayout_5->addWidget(setupInstrumentComboBox, 1, 1, 1, 6);


        gridLayout_4->addWidget(frame, 0, 0, 1, 3);

        verticalSpacer_24 = new QSpacerItem(17, 8, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_4->addItem(verticalSpacer_24, 2, 1, 1, 1);

        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(plainTextEdit->sizePolicy().hasHeightForWidth());
        plainTextEdit->setSizePolicy(sizePolicy3);
        plainTextEdit->setMinimumSize(QSize(450, 0));
        plainTextEdit->setFocusPolicy(Qt::ClickFocus);
        plainTextEdit->setFrameShape(QFrame::Panel);
        plainTextEdit->setLineWidth(2);
        plainTextEdit->setReadOnly(true);
        plainTextEdit->setBackgroundVisible(false);

        gridLayout_4->addWidget(plainTextEdit, 3, 0, 1, 3);

        verticalSpacer_22 = new QSpacerItem(17, 8, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_4->addItem(verticalSpacer_22, 4, 0, 1, 2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        startPushButton = new QPushButton(centralWidget);
        startPushButton->setObjectName(QStringLiteral("startPushButton"));
        startPushButton->setMinimumSize(QSize(80, 0));
        startPushButton->setMaximumSize(QSize(80, 16777215));
        QPalette palette1;
        QBrush brush2(QColor(0, 0, 0, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        QBrush brush3(QColor(68, 216, 204, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush3);
        QBrush brush4(QColor(149, 255, 247, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush4);
        QBrush brush5(QColor(108, 235, 225, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush5);
        QBrush brush6(QColor(34, 108, 102, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush6);
        QBrush brush7(QColor(45, 144, 136, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush7);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush2);
        QBrush brush8(QColor(255, 255, 255, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush8);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        QBrush brush9(QColor(161, 235, 229, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush9);
        QBrush brush10(QColor(255, 255, 220, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush10);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush10);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush10);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        startPushButton->setPalette(palette1);
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        startPushButton->setFont(font1);
        startPushButton->setFocusPolicy(Qt::ClickFocus);
        startPushButton->setAutoFillBackground(false);
        startPushButton->setCheckable(false);

        horizontalLayout->addWidget(startPushButton);

        yieldToolButton = new QToolButton(centralWidget);
        yieldToolButton->setObjectName(QStringLiteral("yieldToolButton"));
        yieldToolButton->setMinimumSize(QSize(40, 40));
        yieldToolButton->setMaximumSize(QSize(40, 40));
        yieldToolButton->setAutoFillBackground(true);
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/icons/Signal-yield-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        yieldToolButton->setIcon(icon10);
        yieldToolButton->setIconSize(QSize(40, 40));

        horizontalLayout->addWidget(yieldToolButton);

        stopToolButton = new QToolButton(centralWidget);
        stopToolButton->setObjectName(QStringLiteral("stopToolButton"));
        stopToolButton->setMinimumSize(QSize(40, 40));
        stopToolButton->setMaximumSize(QSize(40, 40));
        stopToolButton->setAutoFillBackground(true);
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/icons/Actions-process-stop-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        stopToolButton->setIcon(icon11);
        stopToolButton->setIconSize(QSize(40, 40));

        horizontalLayout->addWidget(stopToolButton);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_11);

        processProgressBar = new QProgressBar(centralWidget);
        processProgressBar->setObjectName(QStringLiteral("processProgressBar"));
        processProgressBar->setMinimumSize(QSize(200, 0));
        processProgressBar->setValue(0);
        processProgressBar->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(processProgressBar);


        gridLayout_4->addLayout(horizontalLayout, 5, 0, 1, 3);

        processingTabWidget = new QTabWidget(centralWidget);
        processingTabWidget->setObjectName(QStringLiteral("processingTabWidget"));
        sizePolicy2.setHeightForWidth(processingTabWidget->sizePolicy().hasHeightForWidth());
        processingTabWidget->setSizePolicy(sizePolicy2);
        processingTabWidget->setMinimumSize(QSize(450, 0));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        QBrush brush11(QColor(190, 222, 252, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette2.setBrush(QPalette::Active, QPalette::Light, brush8);
        QBrush brush12(QColor(222, 238, 253, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Midlight, brush12);
        QBrush brush13(QColor(95, 111, 126, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Dark, brush13);
        QBrush brush14(QColor(127, 148, 168, 255));
        brush14.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Mid, brush14);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette2.setBrush(QPalette::Active, QPalette::BrightText, brush8);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush11);
        palette2.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette2.setBrush(QPalette::Active, QPalette::AlternateBase, brush12);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipBase, brush10);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette2.setBrush(QPalette::Inactive, QPalette::Light, brush8);
        palette2.setBrush(QPalette::Inactive, QPalette::Midlight, brush12);
        palette2.setBrush(QPalette::Inactive, QPalette::Dark, brush13);
        palette2.setBrush(QPalette::Inactive, QPalette::Mid, brush14);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::BrightText, brush8);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush11);
        palette2.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush12);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush10);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush13);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette2.setBrush(QPalette::Disabled, QPalette::Light, brush8);
        palette2.setBrush(QPalette::Disabled, QPalette::Midlight, brush12);
        palette2.setBrush(QPalette::Disabled, QPalette::Dark, brush13);
        palette2.setBrush(QPalette::Disabled, QPalette::Mid, brush14);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush13);
        palette2.setBrush(QPalette::Disabled, QPalette::BrightText, brush8);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush13);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush11);
        palette2.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush11);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush10);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        processingTabWidget->setPalette(palette2);
        processingTabWidget->setFocusPolicy(Qt::ClickFocus);
        processingTabWidget->setAutoFillBackground(true);
        processingTabWidgetPage1 = new QWidget();
        processingTabWidgetPage1->setObjectName(QStringLiteral("processingTabWidgetPage1"));
        gridLayout = new QGridLayout(processingTabWidgetPage1);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        setupMainLabel = new QLabel(processingTabWidgetPage1);
        setupMainLabel->setObjectName(QStringLiteral("setupMainLabel"));

        gridLayout->addWidget(setupMainLabel, 0, 0, 1, 1);

        setupReadmePushButton = new QPushButton(processingTabWidgetPage1);
        setupReadmePushButton->setObjectName(QStringLiteral("setupReadmePushButton"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(setupReadmePushButton->sizePolicy().hasHeightForWidth());
        setupReadmePushButton->setSizePolicy(sizePolicy4);
        setupReadmePushButton->setMinimumSize(QSize(30, 0));
        setupReadmePushButton->setMaximumSize(QSize(30, 16777215));
        QPalette palette3;
        QBrush brush15(QColor(85, 170, 255, 255));
        brush15.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush15);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush15);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush15);
        setupReadmePushButton->setPalette(palette3);
        setupReadmePushButton->setFont(font);
        setupReadmePushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(setupReadmePushButton, 0, 1, 1, 1);

        setupMainLineEdit = new QLineEdit(processingTabWidgetPage1);
        setupMainLineEdit->setObjectName(QStringLiteral("setupMainLineEdit"));
        setupMainLineEdit->setClearButtonEnabled(true);

        gridLayout->addWidget(setupMainLineEdit, 0, 2, 1, 1);

        setupBiasLabel = new QLabel(processingTabWidgetPage1);
        setupBiasLabel->setObjectName(QStringLiteral("setupBiasLabel"));

        gridLayout->addWidget(setupBiasLabel, 1, 0, 1, 1);

        setupBiasLineEdit = new QLineEdit(processingTabWidgetPage1);
        setupBiasLineEdit->setObjectName(QStringLiteral("setupBiasLineEdit"));
        setupBiasLineEdit->setClearButtonEnabled(true);

        gridLayout->addWidget(setupBiasLineEdit, 1, 2, 1, 1);

        setupDarkLabel = new QLabel(processingTabWidgetPage1);
        setupDarkLabel->setObjectName(QStringLiteral("setupDarkLabel"));

        gridLayout->addWidget(setupDarkLabel, 2, 0, 1, 1);

        setupDarkLineEdit = new QLineEdit(processingTabWidgetPage1);
        setupDarkLineEdit->setObjectName(QStringLiteral("setupDarkLineEdit"));
        setupDarkLineEdit->setClearButtonEnabled(true);

        gridLayout->addWidget(setupDarkLineEdit, 2, 2, 1, 1);

        setupFlatoffLabel = new QLabel(processingTabWidgetPage1);
        setupFlatoffLabel->setObjectName(QStringLiteral("setupFlatoffLabel"));

        gridLayout->addWidget(setupFlatoffLabel, 3, 0, 1, 2);

        setupFlatoffLineEdit = new QLineEdit(processingTabWidgetPage1);
        setupFlatoffLineEdit->setObjectName(QStringLiteral("setupFlatoffLineEdit"));
        setupFlatoffLineEdit->setClearButtonEnabled(true);

        gridLayout->addWidget(setupFlatoffLineEdit, 3, 2, 1, 1);

        setupFlatLabel = new QLabel(processingTabWidgetPage1);
        setupFlatLabel->setObjectName(QStringLiteral("setupFlatLabel"));

        gridLayout->addWidget(setupFlatLabel, 4, 0, 1, 1);

        setupFlatLineEdit = new QLineEdit(processingTabWidgetPage1);
        setupFlatLineEdit->setObjectName(QStringLiteral("setupFlatLineEdit"));
        setupFlatLineEdit->setClearButtonEnabled(true);

        gridLayout->addWidget(setupFlatLineEdit, 4, 2, 1, 1);

        setupScienceLabel = new QLabel(processingTabWidgetPage1);
        setupScienceLabel->setObjectName(QStringLiteral("setupScienceLabel"));

        gridLayout->addWidget(setupScienceLabel, 5, 0, 1, 1);

        setupScienceLineEdit = new QLineEdit(processingTabWidgetPage1);
        setupScienceLineEdit->setObjectName(QStringLiteral("setupScienceLineEdit"));
        setupScienceLineEdit->setClearButtonEnabled(true);

        gridLayout->addWidget(setupScienceLineEdit, 5, 2, 1, 1);

        setupSkyLabel = new QLabel(processingTabWidgetPage1);
        setupSkyLabel->setObjectName(QStringLiteral("setupSkyLabel"));

        gridLayout->addWidget(setupSkyLabel, 6, 0, 1, 1);

        setupSkyLineEdit = new QLineEdit(processingTabWidgetPage1);
        setupSkyLineEdit->setObjectName(QStringLiteral("setupSkyLineEdit"));
        setupSkyLineEdit->setClearButtonEnabled(true);

        gridLayout->addWidget(setupSkyLineEdit, 6, 2, 1, 1);

        setupStandardLabel = new QLabel(processingTabWidgetPage1);
        setupStandardLabel->setObjectName(QStringLiteral("setupStandardLabel"));
        setupStandardLabel->setEnabled(false);

        gridLayout->addWidget(setupStandardLabel, 7, 0, 1, 1);

        setupStandardLineEdit = new QLineEdit(processingTabWidgetPage1);
        setupStandardLineEdit->setObjectName(QStringLiteral("setupStandardLineEdit"));
        setupStandardLineEdit->setEnabled(false);
        setupStandardLineEdit->setClearButtonEnabled(true);

        gridLayout->addWidget(setupStandardLineEdit, 7, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 8, 2, 1, 1);

        processingTabWidget->addTab(processingTabWidgetPage1, QString());
        processingTabWidgetPage2 = new QWidget();
        processingTabWidgetPage2->setObjectName(QStringLiteral("processingTabWidgetPage2"));
        gridLayout_3 = new QGridLayout(processingTabWidgetPage2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        applyCollapseCheckBox = new QCheckBox(processingTabWidgetPage2);
        applyCollapseCheckBox->setObjectName(QStringLiteral("applyCollapseCheckBox"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(applyCollapseCheckBox->sizePolicy().hasHeightForWidth());
        applyCollapseCheckBox->setSizePolicy(sizePolicy5);
        applyCollapseCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_3->addWidget(applyCollapseCheckBox, 8, 1, 1, 1);

        binnedPreviewConfigureToolButton = new QToolButton(processingTabWidgetPage2);
        binnedPreviewConfigureToolButton->setObjectName(QStringLiteral("binnedPreviewConfigureToolButton"));
        binnedPreviewConfigureToolButton->setMinimumSize(QSize(20, 20));
        binnedPreviewConfigureToolButton->setMaximumSize(QSize(20, 20));
        binnedPreviewConfigureToolButton->setFocusPolicy(Qt::ClickFocus);
        QIcon icon12;
        icon12.addFile(QStringLiteral("icons/key.png"), QSize(), QIcon::Normal, QIcon::Off);
        binnedPreviewConfigureToolButton->setIcon(icon12);
        binnedPreviewConfigureToolButton->setIconSize(QSize(40, 40));
        binnedPreviewConfigureToolButton->setAutoRaise(true);

        gridLayout_3->addWidget(binnedPreviewConfigureToolButton, 9, 0, 1, 1);

        applyBinnedpreviewCheckBox = new QCheckBox(processingTabWidgetPage2);
        applyBinnedpreviewCheckBox->setObjectName(QStringLiteral("applyBinnedpreviewCheckBox"));
        sizePolicy5.setHeightForWidth(applyBinnedpreviewCheckBox->sizePolicy().hasHeightForWidth());
        applyBinnedpreviewCheckBox->setSizePolicy(sizePolicy5);
        applyBinnedpreviewCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_3->addWidget(applyBinnedpreviewCheckBox, 9, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 2, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_2, 10, 1, 1, 1);

        applyHDUreformatCheckBox = new QCheckBox(processingTabWidgetPage2);
        applyHDUreformatCheckBox->setObjectName(QStringLiteral("applyHDUreformatCheckBox"));
        sizePolicy5.setHeightForWidth(applyHDUreformatCheckBox->sizePolicy().hasHeightForWidth());
        applyHDUreformatCheckBox->setSizePolicy(sizePolicy5);
        applyHDUreformatCheckBox->setFocusPolicy(Qt::ClickFocus);
        applyHDUreformatCheckBox->setAutoFillBackground(true);

        gridLayout_3->addWidget(applyHDUreformatCheckBox, 0, 1, 1, 1);

        applyProcessbiasCheckBox = new QCheckBox(processingTabWidgetPage2);
        applyProcessbiasCheckBox->setObjectName(QStringLiteral("applyProcessbiasCheckBox"));
        sizePolicy5.setHeightForWidth(applyProcessbiasCheckBox->sizePolicy().hasHeightForWidth());
        applyProcessbiasCheckBox->setSizePolicy(sizePolicy5);
        applyProcessbiasCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_3->addWidget(applyProcessbiasCheckBox, 1, 1, 1, 1);

        calibratorConfigureToolButton = new QToolButton(processingTabWidgetPage2);
        calibratorConfigureToolButton->setObjectName(QStringLiteral("calibratorConfigureToolButton"));
        calibratorConfigureToolButton->setMinimumSize(QSize(20, 20));
        calibratorConfigureToolButton->setMaximumSize(QSize(20, 20));
        calibratorConfigureToolButton->setFocusPolicy(Qt::ClickFocus);
        calibratorConfigureToolButton->setIcon(icon12);
        calibratorConfigureToolButton->setIconSize(QSize(40, 40));
        calibratorConfigureToolButton->setAutoRaise(true);

        gridLayout_3->addWidget(calibratorConfigureToolButton, 2, 0, 2, 1);

        HDUreformatConfigureToolButton = new QToolButton(processingTabWidgetPage2);
        HDUreformatConfigureToolButton->setObjectName(QStringLiteral("HDUreformatConfigureToolButton"));
        HDUreformatConfigureToolButton->setMinimumSize(QSize(20, 20));
        HDUreformatConfigureToolButton->setMaximumSize(QSize(20, 20));
        HDUreformatConfigureToolButton->setFocusPolicy(Qt::ClickFocus);
        HDUreformatConfigureToolButton->setIcon(icon12);
        HDUreformatConfigureToolButton->setIconSize(QSize(40, 40));
        HDUreformatConfigureToolButton->setAutoRaise(true);

        gridLayout_3->addWidget(HDUreformatConfigureToolButton, 0, 0, 1, 1);

        applyProcessdarkCheckBox = new QCheckBox(processingTabWidgetPage2);
        applyProcessdarkCheckBox->setObjectName(QStringLiteral("applyProcessdarkCheckBox"));
        sizePolicy5.setHeightForWidth(applyProcessdarkCheckBox->sizePolicy().hasHeightForWidth());
        applyProcessdarkCheckBox->setSizePolicy(sizePolicy5);
        applyProcessdarkCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_3->addWidget(applyProcessdarkCheckBox, 2, 1, 1, 1);

        applyProcessflatoffCheckBox = new QCheckBox(processingTabWidgetPage2);
        applyProcessflatoffCheckBox->setObjectName(QStringLiteral("applyProcessflatoffCheckBox"));
        sizePolicy5.setHeightForWidth(applyProcessflatoffCheckBox->sizePolicy().hasHeightForWidth());
        applyProcessflatoffCheckBox->setSizePolicy(sizePolicy5);
        applyProcessflatoffCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_3->addWidget(applyProcessflatoffCheckBox, 3, 1, 1, 1);

        applyProcessflatCheckBox = new QCheckBox(processingTabWidgetPage2);
        applyProcessflatCheckBox->setObjectName(QStringLiteral("applyProcessflatCheckBox"));
        sizePolicy5.setHeightForWidth(applyProcessflatCheckBox->sizePolicy().hasHeightForWidth());
        applyProcessflatCheckBox->setSizePolicy(sizePolicy5);
        applyProcessflatCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_3->addWidget(applyProcessflatCheckBox, 4, 1, 1, 1);

        applyProcessscienceCheckBox = new QCheckBox(processingTabWidgetPage2);
        applyProcessscienceCheckBox->setObjectName(QStringLiteral("applyProcessscienceCheckBox"));
        sizePolicy5.setHeightForWidth(applyProcessscienceCheckBox->sizePolicy().hasHeightForWidth());
        applyProcessscienceCheckBox->setSizePolicy(sizePolicy5);
        applyProcessscienceCheckBox->setFocusPolicy(Qt::ClickFocus);
        applyProcessscienceCheckBox->setAutoFillBackground(true);

        gridLayout_3->addWidget(applyProcessscienceCheckBox, 5, 1, 1, 1);

        chopnodConfigureToolButton = new QToolButton(processingTabWidgetPage2);
        chopnodConfigureToolButton->setObjectName(QStringLiteral("chopnodConfigureToolButton"));
        chopnodConfigureToolButton->setEnabled(false);
        chopnodConfigureToolButton->setMinimumSize(QSize(20, 20));
        chopnodConfigureToolButton->setMaximumSize(QSize(20, 20));
        chopnodConfigureToolButton->setFocusPolicy(Qt::ClickFocus);
        chopnodConfigureToolButton->setIcon(icon12);
        chopnodConfigureToolButton->setIconSize(QSize(40, 40));
        chopnodConfigureToolButton->setAutoRaise(true);

        gridLayout_3->addWidget(chopnodConfigureToolButton, 6, 0, 1, 1);

        applyChopnodCheckBox = new QCheckBox(processingTabWidgetPage2);
        applyChopnodCheckBox->setObjectName(QStringLiteral("applyChopnodCheckBox"));
        applyChopnodCheckBox->setEnabled(false);
        sizePolicy5.setHeightForWidth(applyChopnodCheckBox->sizePolicy().hasHeightForWidth());
        applyChopnodCheckBox->setSizePolicy(sizePolicy5);
        applyChopnodCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_3->addWidget(applyChopnodCheckBox, 6, 1, 1, 1);

        BACconfigureToolButton = new QToolButton(processingTabWidgetPage2);
        BACconfigureToolButton->setObjectName(QStringLiteral("BACconfigureToolButton"));
        BACconfigureToolButton->setMinimumSize(QSize(20, 20));
        BACconfigureToolButton->setMaximumSize(QSize(20, 20));
        BACconfigureToolButton->setFocusPolicy(Qt::ClickFocus);
        BACconfigureToolButton->setIcon(icon12);
        BACconfigureToolButton->setIconSize(QSize(40, 40));
        BACconfigureToolButton->setAutoRaise(true);

        gridLayout_3->addWidget(BACconfigureToolButton, 7, 0, 1, 1);

        applyBackgroundCheckBox = new QCheckBox(processingTabWidgetPage2);
        applyBackgroundCheckBox->setObjectName(QStringLiteral("applyBackgroundCheckBox"));
        sizePolicy5.setHeightForWidth(applyBackgroundCheckBox->sizePolicy().hasHeightForWidth());
        applyBackgroundCheckBox->setSizePolicy(sizePolicy5);
        applyBackgroundCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_3->addWidget(applyBackgroundCheckBox, 7, 1, 1, 1);

        COCconfigureToolButton = new QToolButton(processingTabWidgetPage2);
        COCconfigureToolButton->setObjectName(QStringLiteral("COCconfigureToolButton"));
        COCconfigureToolButton->setMinimumSize(QSize(20, 20));
        COCconfigureToolButton->setMaximumSize(QSize(20, 20));
        COCconfigureToolButton->setFocusPolicy(Qt::ClickFocus);
        COCconfigureToolButton->setIcon(icon12);
        COCconfigureToolButton->setIconSize(QSize(40, 40));
        COCconfigureToolButton->setAutoRaise(true);

        gridLayout_3->addWidget(COCconfigureToolButton, 8, 0, 1, 1);

        processingTabWidget->addTab(processingTabWidgetPage2, QString());
        processingTabWidgetPage3 = new QWidget();
        processingTabWidgetPage3->setObjectName(QStringLiteral("processingTabWidgetPage3"));
        gridLayout_2 = new QGridLayout(processingTabWidgetPage3);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        globalweightConfigureToolButton = new QToolButton(processingTabWidgetPage3);
        globalweightConfigureToolButton->setObjectName(QStringLiteral("globalweightConfigureToolButton"));
        globalweightConfigureToolButton->setMinimumSize(QSize(20, 20));
        globalweightConfigureToolButton->setMaximumSize(QSize(20, 20));
        globalweightConfigureToolButton->setFocusPolicy(Qt::ClickFocus);
        globalweightConfigureToolButton->setIcon(icon12);
        globalweightConfigureToolButton->setIconSize(QSize(40, 40));
        globalweightConfigureToolButton->setAutoRaise(true);

        gridLayout_2->addWidget(globalweightConfigureToolButton, 0, 0, 1, 1);

        applyGlobalweightCheckBox = new QCheckBox(processingTabWidgetPage3);
        applyGlobalweightCheckBox->setObjectName(QStringLiteral("applyGlobalweightCheckBox"));
        applyGlobalweightCheckBox->setFocusPolicy(Qt::ClickFocus);
        applyGlobalweightCheckBox->setAutoFillBackground(true);

        gridLayout_2->addWidget(applyGlobalweightCheckBox, 0, 1, 1, 1);

        individualweightConfigureToolButton = new QToolButton(processingTabWidgetPage3);
        individualweightConfigureToolButton->setObjectName(QStringLiteral("individualweightConfigureToolButton"));
        individualweightConfigureToolButton->setMinimumSize(QSize(20, 20));
        individualweightConfigureToolButton->setMaximumSize(QSize(20, 20));
        individualweightConfigureToolButton->setFocusPolicy(Qt::ClickFocus);
        individualweightConfigureToolButton->setIcon(icon12);
        individualweightConfigureToolButton->setIconSize(QSize(40, 40));
        individualweightConfigureToolButton->setAutoRaise(true);

        gridLayout_2->addWidget(individualweightConfigureToolButton, 1, 0, 1, 1);

        applyIndividualweightCheckBox = new QCheckBox(processingTabWidgetPage3);
        applyIndividualweightCheckBox->setObjectName(QStringLiteral("applyIndividualweightCheckBox"));
        applyIndividualweightCheckBox->setFocusPolicy(Qt::ClickFocus);
        applyIndividualweightCheckBox->setAutoFillBackground(true);

        gridLayout_2->addWidget(applyIndividualweightCheckBox, 1, 1, 1, 1);

        separateConfigureToolButton = new QToolButton(processingTabWidgetPage3);
        separateConfigureToolButton->setObjectName(QStringLiteral("separateConfigureToolButton"));
        separateConfigureToolButton->setEnabled(true);
        separateConfigureToolButton->setMinimumSize(QSize(20, 20));
        separateConfigureToolButton->setMaximumSize(QSize(20, 20));
        separateConfigureToolButton->setFocusPolicy(Qt::ClickFocus);
        separateConfigureToolButton->setIcon(icon12);
        separateConfigureToolButton->setIconSize(QSize(40, 40));
        separateConfigureToolButton->setAutoRaise(true);

        gridLayout_2->addWidget(separateConfigureToolButton, 2, 0, 1, 1);

        applySeparateCheckBox = new QCheckBox(processingTabWidgetPage3);
        applySeparateCheckBox->setObjectName(QStringLiteral("applySeparateCheckBox"));
        applySeparateCheckBox->setEnabled(true);
        applySeparateCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_2->addWidget(applySeparateCheckBox, 2, 1, 1, 1);

        absphotomindirectConfigureToolButton = new QToolButton(processingTabWidgetPage3);
        absphotomindirectConfigureToolButton->setObjectName(QStringLiteral("absphotomindirectConfigureToolButton"));
        absphotomindirectConfigureToolButton->setEnabled(false);
        absphotomindirectConfigureToolButton->setMinimumSize(QSize(20, 20));
        absphotomindirectConfigureToolButton->setMaximumSize(QSize(20, 20));
        absphotomindirectConfigureToolButton->setFocusPolicy(Qt::ClickFocus);
        absphotomindirectConfigureToolButton->setIcon(icon12);
        absphotomindirectConfigureToolButton->setIconSize(QSize(40, 40));
        absphotomindirectConfigureToolButton->setAutoRaise(true);

        gridLayout_2->addWidget(absphotomindirectConfigureToolButton, 3, 0, 1, 1);

        applyAbsphotindirectCheckBox = new QCheckBox(processingTabWidgetPage3);
        applyAbsphotindirectCheckBox->setObjectName(QStringLiteral("applyAbsphotindirectCheckBox"));
        applyAbsphotindirectCheckBox->setEnabled(false);
        applyAbsphotindirectCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_2->addWidget(applyAbsphotindirectCheckBox, 3, 1, 1, 1);

        createsourcecatConfigureToolButton = new QToolButton(processingTabWidgetPage3);
        createsourcecatConfigureToolButton->setObjectName(QStringLiteral("createsourcecatConfigureToolButton"));
        createsourcecatConfigureToolButton->setMinimumSize(QSize(20, 20));
        createsourcecatConfigureToolButton->setMaximumSize(QSize(20, 20));
        createsourcecatConfigureToolButton->setFocusPolicy(Qt::ClickFocus);
        createsourcecatConfigureToolButton->setIcon(icon12);
        createsourcecatConfigureToolButton->setIconSize(QSize(40, 40));
        createsourcecatConfigureToolButton->setAutoRaise(true);

        gridLayout_2->addWidget(createsourcecatConfigureToolButton, 4, 0, 1, 1);

        applyCreatesourcecatCheckBox = new QCheckBox(processingTabWidgetPage3);
        applyCreatesourcecatCheckBox->setObjectName(QStringLiteral("applyCreatesourcecatCheckBox"));
        applyCreatesourcecatCheckBox->setFocusPolicy(Qt::ClickFocus);
        applyCreatesourcecatCheckBox->setAutoFillBackground(true);

        gridLayout_2->addWidget(applyCreatesourcecatCheckBox, 4, 1, 1, 1);

        astromphotomConfigureToolButton = new QToolButton(processingTabWidgetPage3);
        astromphotomConfigureToolButton->setObjectName(QStringLiteral("astromphotomConfigureToolButton"));
        astromphotomConfigureToolButton->setMinimumSize(QSize(20, 20));
        astromphotomConfigureToolButton->setMaximumSize(QSize(20, 20));
        astromphotomConfigureToolButton->setFocusPolicy(Qt::ClickFocus);
        astromphotomConfigureToolButton->setIcon(icon12);
        astromphotomConfigureToolButton->setIconSize(QSize(40, 40));
        astromphotomConfigureToolButton->setAutoRaise(true);

        gridLayout_2->addWidget(astromphotomConfigureToolButton, 5, 0, 1, 1);

        applyAstromphotomCheckBox = new QCheckBox(processingTabWidgetPage3);
        applyAstromphotomCheckBox->setObjectName(QStringLiteral("applyAstromphotomCheckBox"));
        applyAstromphotomCheckBox->setFocusPolicy(Qt::ClickFocus);
        applyAstromphotomCheckBox->setAutoFillBackground(true);

        gridLayout_2->addWidget(applyAstromphotomCheckBox, 5, 1, 1, 1);

        absphotomdirectConfigureToolButton = new QToolButton(processingTabWidgetPage3);
        absphotomdirectConfigureToolButton->setObjectName(QStringLiteral("absphotomdirectConfigureToolButton"));
        absphotomdirectConfigureToolButton->setEnabled(false);
        absphotomdirectConfigureToolButton->setMinimumSize(QSize(20, 20));
        absphotomdirectConfigureToolButton->setMaximumSize(QSize(20, 20));
        absphotomdirectConfigureToolButton->setFocusPolicy(Qt::ClickFocus);
        absphotomdirectConfigureToolButton->setIcon(icon12);
        absphotomdirectConfigureToolButton->setIconSize(QSize(40, 40));
        absphotomdirectConfigureToolButton->setAutoRaise(true);

        gridLayout_2->addWidget(absphotomdirectConfigureToolButton, 6, 0, 1, 1);

        applyStarflatCheckBox = new QCheckBox(processingTabWidgetPage3);
        applyStarflatCheckBox->setObjectName(QStringLiteral("applyStarflatCheckBox"));
        applyStarflatCheckBox->setEnabled(false);
        applyStarflatCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_2->addWidget(applyStarflatCheckBox, 6, 1, 1, 1);

        skysubConfigureToolButton = new QToolButton(processingTabWidgetPage3);
        skysubConfigureToolButton->setObjectName(QStringLiteral("skysubConfigureToolButton"));
        skysubConfigureToolButton->setMinimumSize(QSize(20, 20));
        skysubConfigureToolButton->setMaximumSize(QSize(20, 20));
        skysubConfigureToolButton->setFocusPolicy(Qt::ClickFocus);
        skysubConfigureToolButton->setIcon(icon12);
        skysubConfigureToolButton->setIconSize(QSize(40, 40));
        skysubConfigureToolButton->setAutoRaise(true);

        gridLayout_2->addWidget(skysubConfigureToolButton, 7, 0, 1, 1);

        applySkysubCheckBox = new QCheckBox(processingTabWidgetPage3);
        applySkysubCheckBox->setObjectName(QStringLiteral("applySkysubCheckBox"));
        applySkysubCheckBox->setFocusPolicy(Qt::ClickFocus);
#ifndef QT_NO_SHORTCUT
        applySkysubCheckBox->setShortcut(QStringLiteral(""));
#endif // QT_NO_SHORTCUT

        gridLayout_2->addWidget(applySkysubCheckBox, 7, 1, 1, 1);

        coadditionConfigureToolButton = new QToolButton(processingTabWidgetPage3);
        coadditionConfigureToolButton->setObjectName(QStringLiteral("coadditionConfigureToolButton"));
        coadditionConfigureToolButton->setMinimumSize(QSize(20, 20));
        coadditionConfigureToolButton->setMaximumSize(QSize(20, 20));
        coadditionConfigureToolButton->setFocusPolicy(Qt::ClickFocus);
        coadditionConfigureToolButton->setIcon(icon12);
        coadditionConfigureToolButton->setIconSize(QSize(40, 40));
        coadditionConfigureToolButton->setAutoRaise(true);

        gridLayout_2->addWidget(coadditionConfigureToolButton, 8, 0, 1, 1);

        applyCoadditionCheckBox = new QCheckBox(processingTabWidgetPage3);
        applyCoadditionCheckBox->setObjectName(QStringLiteral("applyCoadditionCheckBox"));
        applyCoadditionCheckBox->setFont(font1);
        applyCoadditionCheckBox->setFocusPolicy(Qt::ClickFocus);
        applyCoadditionCheckBox->setAutoFillBackground(true);

        gridLayout_2->addWidget(applyCoadditionCheckBox, 8, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 34, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_3, 9, 1, 1, 1);

        processingTabWidget->addTab(processingTabWidgetPage3, QString());

        gridLayout_4->addWidget(processingTabWidget, 1, 0, 1, 3);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 468, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setIconSize(QSize(22, 22));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        QWidget::setTabOrder(setupProjectLineEdit, setupInstrumentComboBox);
        QWidget::setTabOrder(setupInstrumentComboBox, setupMainLineEdit);
        QWidget::setTabOrder(setupMainLineEdit, setupBiasLineEdit);
        QWidget::setTabOrder(setupBiasLineEdit, setupDarkLineEdit);
        QWidget::setTabOrder(setupDarkLineEdit, setupFlatoffLineEdit);
        QWidget::setTabOrder(setupFlatoffLineEdit, setupFlatLineEdit);
        QWidget::setTabOrder(setupFlatLineEdit, setupScienceLineEdit);
        QWidget::setTabOrder(setupScienceLineEdit, setupSkyLineEdit);
        QWidget::setTabOrder(setupSkyLineEdit, setupStandardLineEdit);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionEdit_preferences);
        menuFile->addAction(actionAdd_new_instrument_configuration);
        menuFile->addAction(actionKill);
        menuFile->addAction(actionClose);
        menuAbout->addAction(actionLicense);
        menuAbout->addAction(actionAcknowledging);
        toolBar->addAction(actionPreferences);
        toolBar->addSeparator();
        toolBar->addSeparator();
        toolBar->addSeparator();
        toolBar->addAction(actioniView);
        toolBar->addAction(actionImage_statistics);
        toolBar->addAction(actionAbsZP);
        toolBar->addAction(actionColor_picture);
        toolBar->addAction(actionNew_instrument);

        retranslateUi(MainWindow);

        processingTabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "THELI GUI", Q_NULLPTR));
        actionCreate_new_instrument->setText(QApplication::translate("MainWindow", "Create new instrument ...", Q_NULLPTR));
        actioniView->setText(QApplication::translate("MainWindow", "iView", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actioniView->setToolTip(QApplication::translate("MainWindow", "Launch iView", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionImage_statistics->setText(QApplication::translate("MainWindow", "Image statistics", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionImage_statistics->setToolTip(QApplication::translate("MainWindow", "Loads the image statistics module", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionColor_picture->setText(QApplication::translate("MainWindow", "Color picture", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionColor_picture->setToolTip(QApplication::translate("MainWindow", "Load the color picture module", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionNew_instrument->setText(QApplication::translate("MainWindow", "New instrument", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionNew_instrument->setToolTip(QApplication::translate("MainWindow", "Add a new instrument configuration", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionAbsZP->setText(QApplication::translate("MainWindow", "AbsZP", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionAbsZP->setToolTip(QApplication::translate("MainWindow", "Absolute photometric calibration", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionClose->setText(QApplication::translate("MainWindow", "&Close", Q_NULLPTR));
        actionWindowsConfiguration->setText(QApplication::translate("MainWindow", "Configuration", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionWindowsConfiguration->setToolTip(QApplication::translate("MainWindow", "Displays the task configuration options", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        actionWindowsConfiguration->setStatusTip(QApplication::translate("MainWindow", "Displays the task configuration options", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        actionWindowsMonitor->setText(QApplication::translate("MainWindow", "Monitor", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionWindowsMonitor->setToolTip(QApplication::translate("MainWindow", "Displays output from the current processing tasks", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        actionWindowsMonitor->setStatusTip(QApplication::translate("MainWindow", "Displays output from the current processing tasks", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        actionPreferences->setText(QApplication::translate("MainWindow", "Preferences", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionPreferences->setToolTip(QApplication::translate("MainWindow", "Edit preferences", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionEdit_preferences->setText(QApplication::translate("MainWindow", "Edit preferences", Q_NULLPTR));
        actionAdd_new_instrument_configuration->setText(QApplication::translate("MainWindow", "Add new instrument configuration", Q_NULLPTR));
        actionKill->setText(QApplication::translate("MainWindow", "Kill current task. Restart recommended.", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionKill->setToolTip(QApplication::translate("MainWindow", "Kills the current task immediately. Files on drive may need manual clean-up. Data model in memory possibly corrupted. Restart recommended.", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionAbout->setText(QApplication::translate("MainWindow", "About", Q_NULLPTR));
        actionLicense->setText(QApplication::translate("MainWindow", "GPL License", Q_NULLPTR));
        actionAcknowledging->setText(QApplication::translate("MainWindow", "Acknowledging", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        setupProjectResetDataToolButton->setStatusTip(QApplication::translate("MainWindow", "WARNING: Restores all raw data! All processed data in memory and on drive will be lost.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        setupProjectResetDataToolButton->setText(QApplication::translate("MainWindow", "...", Q_NULLPTR));
        setupProjectLabel->setText(QApplication::translate("MainWindow", "Project name", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        setupProjectLoadToolButton->setStatusTip(QApplication::translate("MainWindow", "Load an existing project", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        setupProjectLoadToolButton->setText(QApplication::translate("MainWindow", "...", Q_NULLPTR));
        setupProjectLabel_2->setText(QApplication::translate("MainWindow", "Instrument", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        setupProjectLineEdit->setStatusTip(QApplication::translate("MainWindow", "A meaningful name for this project, e.g. NGC1234_J-band", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        setupProjectResetToolButton->setStatusTip(QApplication::translate("MainWindow", "Reset all parameters for this project. Data kept in memory will optionally be written to the drive.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        setupProjectResetToolButton->setText(QApplication::translate("MainWindow", "...", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        setupInstrumentComboBox->setStatusTip(QApplication::translate("MainWindow", "Select instrument. Blue=optical, green = 1.0-2.5 microns, orange=1-5 microns, red=mid-IR", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        plainTextEdit->setStatusTip(QApplication::translate("MainWindow", "Displays top-level messages about the processing queue", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        startPushButton->setStatusTip(QApplication::translate("MainWindow", "Start the selected tasks", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        startPushButton->setText(QApplication::translate("MainWindow", "Start", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        yieldToolButton->setStatusTip(QApplication::translate("MainWindow", "Lets the current task finish for the current data set, then stops (soft abort).", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        yieldToolButton->setText(QApplication::translate("MainWindow", "...", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        stopToolButton->setStatusTip(QApplication::translate("MainWindow", "Stops the current task gracefully (task will not finish), files on drive may need manual clean-up (hard abort).", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        stopToolButton->setText(QApplication::translate("MainWindow", "...", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        processProgressBar->setStatusTip(QApplication::translate("MainWindow", "The progress of the currently ongoing task", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        processingTabWidget->setStatusTip(QApplication::translate("MainWindow", "The data tree and processing tasks", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        setupMainLabel->setText(QApplication::translate("MainWindow", "MAIN dir", Q_NULLPTR));
        setupReadmePushButton->setText(QApplication::translate("MainWindow", "?", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        setupMainLineEdit->setStatusTip(QApplication::translate("MainWindow", "The main directory containing all data. ABSOLUTE PATH required.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        setupBiasLabel->setText(QApplication::translate("MainWindow", "BIAS", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        setupBiasLineEdit->setStatusTip(QApplication::translate("MainWindow", "BIAS directories, applied to SCIENCE and SKY. Applied to FLAT if no FLAT_OFF data specified.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        setupDarkLabel->setText(QApplication::translate("MainWindow", "DARK", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        setupDarkLineEdit->setStatusTip(QApplication::translate("MainWindow", "DARK directories, applied to SCIENCE and SKY, only", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        setupFlatoffLabel->setText(QApplication::translate("MainWindow", "FLAT OFF/DARK", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        setupFlatoffLineEdit->setStatusTip(QApplication::translate("MainWindow", "Dark flats (NIR) or actual darks for flats", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        setupFlatLabel->setText(QApplication::translate("MainWindow", "FLAT", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        setupFlatLineEdit->setStatusTip(QApplication::translate("MainWindow", "FLAT directories", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        setupScienceLabel->setText(QApplication::translate("MainWindow", "SCIENCE", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        setupScienceLineEdit->setStatusTip(QApplication::translate("MainWindow", "Directories with SCIENCE exposures go here", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        setupSkyLabel->setText(QApplication::translate("MainWindow", "SKY", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        setupSkyLineEdit->setStatusTip(QApplication::translate("MainWindow", "Directories with blank fields for the science exposures", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        setupStandardLabel->setText(QApplication::translate("MainWindow", "STANDARD", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        setupStandardLineEdit->setStatusTip(QApplication::translate("MainWindow", "Directory with standard star exposures", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        processingTabWidget->setTabText(processingTabWidget->indexOf(processingTabWidgetPage1), QApplication::translate("MainWindow", "Data tree", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        applyCollapseCheckBox->setStatusTip(QApplication::translate("MainWindow", "Applies an average row/column correction to remove linear gradients.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        applyCollapseCheckBox->setText(QApplication::translate("MainWindow", "Collapse correction", Q_NULLPTR));
        binnedPreviewConfigureToolButton->setText(QString());
#ifndef QT_NO_STATUSTIP
        applyBinnedpreviewCheckBox->setStatusTip(QApplication::translate("MainWindow", "Displays binned previews of the fully preprocessed data.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        applyBinnedpreviewCheckBox->setText(QApplication::translate("MainWindow", "Binned preview", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        applyHDUreformatCheckBox->setStatusTip(QApplication::translate("MainWindow", "MANDATORY: Header reformatting, splitting of multi-chip data", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        applyHDUreformatCheckBox->setText(QApplication::translate("MainWindow", "HDU reformatting and detector-level corrections", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        applyProcessbiasCheckBox->setStatusTip(QApplication::translate("MainWindow", "Create a master BIAS", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        applyProcessbiasCheckBox->setText(QApplication::translate("MainWindow", "Process BIAS", Q_NULLPTR));
        calibratorConfigureToolButton->setText(QString());
        HDUreformatConfigureToolButton->setText(QString());
#ifndef QT_NO_STATUSTIP
        applyProcessdarkCheckBox->setStatusTip(QApplication::translate("MainWindow", "Create a master DARK", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        applyProcessdarkCheckBox->setText(QApplication::translate("MainWindow", "Process DARK", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        applyProcessflatoffCheckBox->setStatusTip(QApplication::translate("MainWindow", "Create a master dark flat or flat dark", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        applyProcessflatoffCheckBox->setText(QApplication::translate("MainWindow", "Process FLAT OFF / FLAT DARK", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        applyProcessflatCheckBox->setStatusTip(QApplication::translate("MainWindow", "Create the master FLAT", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        applyProcessflatCheckBox->setText(QApplication::translate("MainWindow", "Process FLAT", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        applyProcessscienceCheckBox->setStatusTip(QApplication::translate("MainWindow", "Apply master BIAS / DARK / FLAT", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        applyProcessscienceCheckBox->setText(QApplication::translate("MainWindow", "Process SCIENCE with BIAS / DARK and FLAT", Q_NULLPTR));
        chopnodConfigureToolButton->setText(QString());
#ifndef QT_NO_STATUSTIP
        applyChopnodCheckBox->setStatusTip(QApplication::translate("MainWindow", "Performs standard chop-nod sky subtraction for mid-infrared data", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        applyChopnodCheckBox->setText(QApplication::translate("MainWindow", "Chop/Nod skysub (MIR)", Q_NULLPTR));
        BACconfigureToolButton->setText(QString());
#ifndef QT_NO_STATUSTIP
        applyBackgroundCheckBox->setStatusTip(QApplication::translate("MainWindow", "Performs static or dynamic background modeling.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        applyBackgroundCheckBox->setText(QApplication::translate("MainWindow", "Background correction", Q_NULLPTR));
        COCconfigureToolButton->setText(QString());
        processingTabWidget->setTabText(processingTabWidget->indexOf(processingTabWidgetPage2), QApplication::translate("MainWindow", "Calibration tasks", Q_NULLPTR));
        globalweightConfigureToolButton->setText(QString());
#ifndef QT_NO_STATUSTIP
        applyGlobalweightCheckBox->setStatusTip(QApplication::translate("MainWindow", "MANDATORY: Prepares the global weight maps", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        applyGlobalweightCheckBox->setText(QApplication::translate("MainWindow", "Global weight maps", Q_NULLPTR));
        individualweightConfigureToolButton->setText(QString());
#ifndef QT_NO_STATUSTIP
        applyIndividualweightCheckBox->setStatusTip(QApplication::translate("MainWindow", "MANDATORY: Calculates individual weight maps", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        applyIndividualweightCheckBox->setText(QApplication::translate("MainWindow", "Individual weight maps", Q_NULLPTR));
        separateConfigureToolButton->setText(QString());
#ifndef QT_NO_STATUSTIP
        applySeparateCheckBox->setStatusTip(QApplication::translate("MainWindow", "Separates target groups if several targets have been combined so far in the same directory.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        applySeparateCheckBox->setText(QApplication::translate("MainWindow", "Separate target groups", Q_NULLPTR));
        absphotomindirectConfigureToolButton->setText(QString());
#ifndef QT_NO_STATUSTIP
        applyAbsphotindirectCheckBox->setStatusTip(QApplication::translate("MainWindow", "Performs absolute photometry using photometric standard star observations.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        applyAbsphotindirectCheckBox->setText(QApplication::translate("MainWindow", "Absolute photometry (using standard stars)", Q_NULLPTR));
        createsourcecatConfigureToolButton->setText(QString());
#ifndef QT_NO_STATUSTIP
        applyCreatesourcecatCheckBox->setStatusTip(QApplication::translate("MainWindow", "MANDATORY: Creates source catalogs for each image, used for photometry and astrometry.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        applyCreatesourcecatCheckBox->setText(QApplication::translate("MainWindow", "Create catalogs", Q_NULLPTR));
        astromphotomConfigureToolButton->setText(QString());
#ifndef QT_NO_STATUSTIP
        applyAstromphotomCheckBox->setStatusTip(QApplication::translate("MainWindow", "MANDATORY: Performs the astrometry", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        applyAstromphotomCheckBox->setText(QApplication::translate("MainWindow", "Astrometry / relative photometry", Q_NULLPTR));
        absphotomdirectConfigureToolButton->setText(QString());
        applyStarflatCheckBox->setText(QApplication::translate("MainWindow", "Star flat", Q_NULLPTR));
        skysubConfigureToolButton->setText(QString());
#ifndef QT_NO_STATUSTIP
        applySkysubCheckBox->setStatusTip(QApplication::translate("MainWindow", "Calculates a sky model, fit or constant sky. Mandatory for multi-chip data.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        applySkysubCheckBox->setText(QApplication::translate("MainWindow", "Sky subtraction", Q_NULLPTR));
        coadditionConfigureToolButton->setText(QString());
#ifndef QT_NO_STATUSTIP
        applyCoadditionCheckBox->setStatusTip(QApplication::translate("MainWindow", "MANDATORY: Final image coaddition.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        applyCoadditionCheckBox->setText(QApplication::translate("MainWindow", "Coaddition", Q_NULLPTR));
        processingTabWidget->setTabText(processingTabWidget->indexOf(processingTabWidgetPage3), QApplication::translate("MainWindow", "Coaddition tasks", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "THELI", Q_NULLPTR));
        menuAbout->setTitle(QApplication::translate("MainWindow", "About", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
