/********************************************************************************
** Form generated from reading UI file 'preferences.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCES_H
#define UI_PREFERENCES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Preferences
{
public:
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *generalLabel;
    QFrame *generalFrame;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QLabel *label_9;
    QLabel *confBrightstarServerLabel_2;
    QComboBox *prefVerbosityComboBox;
    QSpinBox *prefDiskspacewarnSpinBox;
    QCheckBox *prefProcessSkyCheckBox;
    QLabel *confBrightstarServerLabel;
    QComboBox *prefIntermediateDataComboBox;
    QComboBox *prefServerComboBox;
    QSpacerItem *verticalSpacer_5;
    QLabel *parallelizationLabel;
    QFrame *parallelizationFrame;
    QGridLayout *gridLayout_4;
    QLabel *label_7;
    QSpinBox *prefCPUSpinBox;
    QLabel *label_8;
    QSpinBox *prefIOthreadsSpinBox;
    QCheckBox *prefGPUCheckBox;
    QSpacerItem *verticalSpacer_6;
    QLabel *memoryLabel;
    QFrame *memoryFrame;
    QGridLayout *gridLayout_3;
    QLabel *prefMemoryLabel;
    QSpinBox *prefMemorySpinBox;
    QCheckBox *prefMemoryCheckBox;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout;
    QLabel *userInterfaceLabel;
    QFrame *userInterfaceFrame;
    QGridLayout *gridLayout_6;
    QCheckBox *prefSwitchProcessMonitorCheckBox;
    QSpacerItem *verticalSpacer_4;
    QLabel *fontLabel;
    QFrame *fontFrame;
    QGridLayout *gridLayout_5;
    QPushButton *prefDefaultFontPushButton;
    QSpinBox *prefFontsizeSpinBox;
    QPushButton *prefFontdialogPushButton;
    QLabel *label_6;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *prefCloseButton;
    QPushButton *prefCancelButton;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QDialog *Preferences)
    {
        if (Preferences->objectName().isEmpty())
            Preferences->setObjectName(QStringLiteral("Preferences"));
        Preferences->resize(675, 513);
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(170, 170, 170, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(212, 212, 212, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(85, 85, 85, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(113, 113, 113, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush2);
        QBrush brush6(QColor(200, 200, 201, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush6);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        QBrush brush7(QColor(255, 255, 220, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
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
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
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
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        Preferences->setPalette(palette);
        Preferences->setModal(true);
        gridLayout_2 = new QGridLayout(Preferences);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        generalLabel = new QLabel(Preferences);
        generalLabel->setObjectName(QStringLiteral("generalLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(generalLabel->sizePolicy().hasHeightForWidth());
        generalLabel->setSizePolicy(sizePolicy);
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        generalLabel->setFont(font);
        generalLabel->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        generalLabel->setWordWrap(true);

        verticalLayout_2->addWidget(generalLabel);

        generalFrame = new QFrame(Preferences);
        generalFrame->setObjectName(QStringLiteral("generalFrame"));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush8(QColor(235, 235, 238, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush8);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush9(QColor(245, 245, 246, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush9);
        QBrush brush10(QColor(117, 117, 119, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush10);
        QBrush brush11(QColor(157, 157, 159, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush11);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush8);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush9);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush10);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush11);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush10);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush10);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush11);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush10);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush10);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        generalFrame->setPalette(palette1);
        generalFrame->setAutoFillBackground(true);
        generalFrame->setFrameShape(QFrame::StyledPanel);
        generalFrame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(generalFrame);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_4 = new QLabel(generalFrame);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label_9 = new QLabel(generalFrame);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout->addWidget(label_9, 1, 0, 1, 1);

        confBrightstarServerLabel_2 = new QLabel(generalFrame);
        confBrightstarServerLabel_2->setObjectName(QStringLiteral("confBrightstarServerLabel_2"));

        gridLayout->addWidget(confBrightstarServerLabel_2, 2, 0, 1, 1);

        prefVerbosityComboBox = new QComboBox(generalFrame);
        prefVerbosityComboBox->setObjectName(QStringLiteral("prefVerbosityComboBox"));

        gridLayout->addWidget(prefVerbosityComboBox, 2, 1, 1, 1);

        prefDiskspacewarnSpinBox = new QSpinBox(generalFrame);
        prefDiskspacewarnSpinBox->setObjectName(QStringLiteral("prefDiskspacewarnSpinBox"));
        prefDiskspacewarnSpinBox->setMinimum(256);
        prefDiskspacewarnSpinBox->setMaximum(9999);
        prefDiskspacewarnSpinBox->setSingleStep(256);

        gridLayout->addWidget(prefDiskspacewarnSpinBox, 3, 1, 1, 1);

        prefProcessSkyCheckBox = new QCheckBox(generalFrame);
        prefProcessSkyCheckBox->setObjectName(QStringLiteral("prefProcessSkyCheckBox"));
        prefProcessSkyCheckBox->setEnabled(false);

        gridLayout->addWidget(prefProcessSkyCheckBox, 4, 0, 1, 2);

        confBrightstarServerLabel = new QLabel(generalFrame);
        confBrightstarServerLabel->setObjectName(QStringLiteral("confBrightstarServerLabel"));
        confBrightstarServerLabel->setEnabled(false);

        gridLayout->addWidget(confBrightstarServerLabel, 0, 0, 1, 1);

        prefIntermediateDataComboBox = new QComboBox(generalFrame);
        prefIntermediateDataComboBox->setObjectName(QStringLiteral("prefIntermediateDataComboBox"));

        gridLayout->addWidget(prefIntermediateDataComboBox, 1, 1, 1, 1);

        prefServerComboBox = new QComboBox(generalFrame);
        prefServerComboBox->setObjectName(QStringLiteral("prefServerComboBox"));
        prefServerComboBox->setEnabled(false);
        prefServerComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(prefServerComboBox, 0, 1, 1, 1);


        verticalLayout_2->addWidget(generalFrame);

        verticalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_5);

        parallelizationLabel = new QLabel(Preferences);
        parallelizationLabel->setObjectName(QStringLiteral("parallelizationLabel"));
        sizePolicy.setHeightForWidth(parallelizationLabel->sizePolicy().hasHeightForWidth());
        parallelizationLabel->setSizePolicy(sizePolicy);
        parallelizationLabel->setFont(font);
        parallelizationLabel->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        parallelizationLabel->setWordWrap(true);

        verticalLayout_2->addWidget(parallelizationLabel);

        parallelizationFrame = new QFrame(Preferences);
        parallelizationFrame->setObjectName(QStringLiteral("parallelizationFrame"));
        QPalette palette2;
        QBrush brush12(QColor(252, 252, 252, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush12);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush8);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush12);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush8);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush8);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush8);
        parallelizationFrame->setPalette(palette2);
        parallelizationFrame->setAutoFillBackground(true);
        parallelizationFrame->setFrameShape(QFrame::StyledPanel);
        parallelizationFrame->setFrameShadow(QFrame::Raised);
        gridLayout_4 = new QGridLayout(parallelizationFrame);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        label_7 = new QLabel(parallelizationFrame);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_4->addWidget(label_7, 0, 0, 1, 1);

        prefCPUSpinBox = new QSpinBox(parallelizationFrame);
        prefCPUSpinBox->setObjectName(QStringLiteral("prefCPUSpinBox"));
        prefCPUSpinBox->setMinimum(1);

        gridLayout_4->addWidget(prefCPUSpinBox, 0, 1, 1, 1);

        label_8 = new QLabel(parallelizationFrame);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setEnabled(false);

        gridLayout_4->addWidget(label_8, 1, 0, 1, 1);

        prefIOthreadsSpinBox = new QSpinBox(parallelizationFrame);
        prefIOthreadsSpinBox->setObjectName(QStringLiteral("prefIOthreadsSpinBox"));
        prefIOthreadsSpinBox->setEnabled(false);
        prefIOthreadsSpinBox->setMinimum(1);

        gridLayout_4->addWidget(prefIOthreadsSpinBox, 1, 1, 1, 1);

        prefGPUCheckBox = new QCheckBox(parallelizationFrame);
        prefGPUCheckBox->setObjectName(QStringLiteral("prefGPUCheckBox"));
        prefGPUCheckBox->setEnabled(false);

        gridLayout_4->addWidget(prefGPUCheckBox, 2, 0, 1, 2);


        verticalLayout_2->addWidget(parallelizationFrame);

        verticalSpacer_6 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_6);

        memoryLabel = new QLabel(Preferences);
        memoryLabel->setObjectName(QStringLiteral("memoryLabel"));
        sizePolicy.setHeightForWidth(memoryLabel->sizePolicy().hasHeightForWidth());
        memoryLabel->setSizePolicy(sizePolicy);
        memoryLabel->setFont(font);
        memoryLabel->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        memoryLabel->setWordWrap(true);

        verticalLayout_2->addWidget(memoryLabel);

        memoryFrame = new QFrame(Preferences);
        memoryFrame->setObjectName(QStringLiteral("memoryFrame"));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush8);
        palette3.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Midlight, brush9);
        palette3.setBrush(QPalette::Active, QPalette::Dark, brush10);
        palette3.setBrush(QPalette::Active, QPalette::Mid, brush11);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush);
        palette3.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush8);
        palette3.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Active, QPalette::AlternateBase, brush9);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Midlight, brush9);
        palette3.setBrush(QPalette::Inactive, QPalette::Dark, brush10);
        palette3.setBrush(QPalette::Inactive, QPalette::Mid, brush11);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush9);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Midlight, brush9);
        palette3.setBrush(QPalette::Disabled, QPalette::Dark, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::Mid, brush11);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        memoryFrame->setPalette(palette3);
        memoryFrame->setAutoFillBackground(true);
        memoryFrame->setFrameShape(QFrame::StyledPanel);
        memoryFrame->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(memoryFrame);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        prefMemoryLabel = new QLabel(memoryFrame);
        prefMemoryLabel->setObjectName(QStringLiteral("prefMemoryLabel"));

        gridLayout_3->addWidget(prefMemoryLabel, 0, 0, 1, 1);

        prefMemorySpinBox = new QSpinBox(memoryFrame);
        prefMemorySpinBox->setObjectName(QStringLiteral("prefMemorySpinBox"));
        prefMemorySpinBox->setMinimum(1024);
        prefMemorySpinBox->setMaximum(1000000);
        prefMemorySpinBox->setSingleStep(512);

        gridLayout_3->addWidget(prefMemorySpinBox, 0, 1, 1, 1);

        prefMemoryCheckBox = new QCheckBox(memoryFrame);
        prefMemoryCheckBox->setObjectName(QStringLiteral("prefMemoryCheckBox"));

        gridLayout_3->addWidget(prefMemoryCheckBox, 1, 0, 1, 2);


        verticalLayout_2->addWidget(memoryFrame);

        verticalSpacer_3 = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout_2->addItem(verticalSpacer_3);


        gridLayout_2->addLayout(verticalLayout_2, 0, 0, 2, 1);

        horizontalSpacer_2 = new QSpacerItem(30, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 0, 1, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        userInterfaceLabel = new QLabel(Preferences);
        userInterfaceLabel->setObjectName(QStringLiteral("userInterfaceLabel"));
        sizePolicy.setHeightForWidth(userInterfaceLabel->sizePolicy().hasHeightForWidth());
        userInterfaceLabel->setSizePolicy(sizePolicy);
        userInterfaceLabel->setFont(font);
        userInterfaceLabel->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        userInterfaceLabel->setWordWrap(true);

        verticalLayout->addWidget(userInterfaceLabel);

        userInterfaceFrame = new QFrame(Preferences);
        userInterfaceFrame->setObjectName(QStringLiteral("userInterfaceFrame"));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush8);
        palette4.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Midlight, brush9);
        palette4.setBrush(QPalette::Active, QPalette::Dark, brush10);
        palette4.setBrush(QPalette::Active, QPalette::Mid, brush11);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush);
        palette4.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush8);
        palette4.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Active, QPalette::AlternateBase, brush9);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush8);
        palette4.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Midlight, brush9);
        palette4.setBrush(QPalette::Inactive, QPalette::Dark, brush10);
        palette4.setBrush(QPalette::Inactive, QPalette::Mid, brush11);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush8);
        palette4.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush9);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush10);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush8);
        palette4.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Midlight, brush9);
        palette4.setBrush(QPalette::Disabled, QPalette::Dark, brush10);
        palette4.setBrush(QPalette::Disabled, QPalette::Mid, brush11);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush10);
        palette4.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush10);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush8);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush8);
        palette4.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush8);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        userInterfaceFrame->setPalette(palette4);
        userInterfaceFrame->setAutoFillBackground(true);
        userInterfaceFrame->setFrameShape(QFrame::StyledPanel);
        userInterfaceFrame->setFrameShadow(QFrame::Raised);
        gridLayout_6 = new QGridLayout(userInterfaceFrame);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        prefSwitchProcessMonitorCheckBox = new QCheckBox(userInterfaceFrame);
        prefSwitchProcessMonitorCheckBox->setObjectName(QStringLiteral("prefSwitchProcessMonitorCheckBox"));
        prefSwitchProcessMonitorCheckBox->setChecked(true);

        gridLayout_6->addWidget(prefSwitchProcessMonitorCheckBox, 0, 0, 1, 2);


        verticalLayout->addWidget(userInterfaceFrame);

        verticalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_4);

        fontLabel = new QLabel(Preferences);
        fontLabel->setObjectName(QStringLiteral("fontLabel"));
        sizePolicy.setHeightForWidth(fontLabel->sizePolicy().hasHeightForWidth());
        fontLabel->setSizePolicy(sizePolicy);
        fontLabel->setFont(font);
        fontLabel->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        fontLabel->setWordWrap(true);

        verticalLayout->addWidget(fontLabel);

        fontFrame = new QFrame(Preferences);
        fontFrame->setObjectName(QStringLiteral("fontFrame"));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Active, QPalette::Button, brush8);
        palette5.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Midlight, brush9);
        palette5.setBrush(QPalette::Active, QPalette::Dark, brush10);
        palette5.setBrush(QPalette::Active, QPalette::Mid, brush11);
        palette5.setBrush(QPalette::Active, QPalette::Text, brush);
        palette5.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette5.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush8);
        palette5.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette5.setBrush(QPalette::Active, QPalette::AlternateBase, brush9);
        palette5.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette5.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Button, brush8);
        palette5.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Midlight, brush9);
        palette5.setBrush(QPalette::Inactive, QPalette::Dark, brush10);
        palette5.setBrush(QPalette::Inactive, QPalette::Mid, brush11);
        palette5.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush8);
        palette5.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush9);
        palette5.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette5.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush10);
        palette5.setBrush(QPalette::Disabled, QPalette::Button, brush8);
        palette5.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Midlight, brush9);
        palette5.setBrush(QPalette::Disabled, QPalette::Dark, brush10);
        palette5.setBrush(QPalette::Disabled, QPalette::Mid, brush11);
        palette5.setBrush(QPalette::Disabled, QPalette::Text, brush10);
        palette5.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::ButtonText, brush10);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush8);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush8);
        palette5.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush8);
        palette5.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette5.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        fontFrame->setPalette(palette5);
        fontFrame->setAutoFillBackground(true);
        fontFrame->setFrameShape(QFrame::StyledPanel);
        fontFrame->setFrameShadow(QFrame::Raised);
        gridLayout_5 = new QGridLayout(fontFrame);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        prefDefaultFontPushButton = new QPushButton(fontFrame);
        prefDefaultFontPushButton->setObjectName(QStringLiteral("prefDefaultFontPushButton"));

        gridLayout_5->addWidget(prefDefaultFontPushButton, 0, 1, 1, 1);

        prefFontsizeSpinBox = new QSpinBox(fontFrame);
        prefFontsizeSpinBox->setObjectName(QStringLiteral("prefFontsizeSpinBox"));
        prefFontsizeSpinBox->setMinimum(8);

        gridLayout_5->addWidget(prefFontsizeSpinBox, 1, 1, 1, 1);

        prefFontdialogPushButton = new QPushButton(fontFrame);
        prefFontdialogPushButton->setObjectName(QStringLiteral("prefFontdialogPushButton"));

        gridLayout_5->addWidget(prefFontdialogPushButton, 0, 0, 1, 1);

        label_6 = new QLabel(fontFrame);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_5->addWidget(label_6, 1, 0, 1, 1);


        verticalLayout->addWidget(fontFrame);

        verticalSpacer = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        prefCloseButton = new QPushButton(Preferences);
        prefCloseButton->setObjectName(QStringLiteral("prefCloseButton"));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Active, QPalette::Button, brush8);
        palette6.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette6.setBrush(QPalette::Active, QPalette::Midlight, brush9);
        palette6.setBrush(QPalette::Active, QPalette::Dark, brush10);
        palette6.setBrush(QPalette::Active, QPalette::Mid, brush11);
        palette6.setBrush(QPalette::Active, QPalette::Text, brush);
        palette6.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette6.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette6.setBrush(QPalette::Active, QPalette::Window, brush8);
        palette6.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette6.setBrush(QPalette::Active, QPalette::AlternateBase, brush9);
        palette6.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette6.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Button, brush8);
        palette6.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::Midlight, brush9);
        palette6.setBrush(QPalette::Inactive, QPalette::Dark, brush10);
        palette6.setBrush(QPalette::Inactive, QPalette::Mid, brush11);
        palette6.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::Window, brush8);
        palette6.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush9);
        palette6.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette6.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush10);
        palette6.setBrush(QPalette::Disabled, QPalette::Button, brush8);
        palette6.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::Midlight, brush9);
        palette6.setBrush(QPalette::Disabled, QPalette::Dark, brush10);
        palette6.setBrush(QPalette::Disabled, QPalette::Mid, brush11);
        palette6.setBrush(QPalette::Disabled, QPalette::Text, brush10);
        palette6.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::ButtonText, brush10);
        palette6.setBrush(QPalette::Disabled, QPalette::Base, brush8);
        palette6.setBrush(QPalette::Disabled, QPalette::Window, brush8);
        palette6.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush8);
        palette6.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette6.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        prefCloseButton->setPalette(palette6);

        horizontalLayout->addWidget(prefCloseButton);

        prefCancelButton = new QPushButton(Preferences);
        prefCancelButton->setObjectName(QStringLiteral("prefCancelButton"));
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Active, QPalette::Button, brush8);
        palette7.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette7.setBrush(QPalette::Active, QPalette::Midlight, brush9);
        palette7.setBrush(QPalette::Active, QPalette::Dark, brush10);
        palette7.setBrush(QPalette::Active, QPalette::Mid, brush11);
        palette7.setBrush(QPalette::Active, QPalette::Text, brush);
        palette7.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette7.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette7.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette7.setBrush(QPalette::Active, QPalette::Window, brush8);
        palette7.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette7.setBrush(QPalette::Active, QPalette::AlternateBase, brush9);
        palette7.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette7.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Button, brush8);
        palette7.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::Midlight, brush9);
        palette7.setBrush(QPalette::Inactive, QPalette::Dark, brush10);
        palette7.setBrush(QPalette::Inactive, QPalette::Mid, brush11);
        palette7.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::Window, brush8);
        palette7.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush9);
        palette7.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette7.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush10);
        palette7.setBrush(QPalette::Disabled, QPalette::Button, brush8);
        palette7.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::Midlight, brush9);
        palette7.setBrush(QPalette::Disabled, QPalette::Dark, brush10);
        palette7.setBrush(QPalette::Disabled, QPalette::Mid, brush11);
        palette7.setBrush(QPalette::Disabled, QPalette::Text, brush10);
        palette7.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::ButtonText, brush10);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush8);
        palette7.setBrush(QPalette::Disabled, QPalette::Window, brush8);
        palette7.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush8);
        palette7.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette7.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        prefCancelButton->setPalette(palette7);

        horizontalLayout->addWidget(prefCancelButton);

        horizontalSpacer = new QSpacerItem(17, 27, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        gridLayout_2->addLayout(verticalLayout, 0, 2, 2, 1);

        horizontalSpacer_3 = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 1, 3, 1, 1);

#ifndef QT_NO_SHORTCUT
        confBrightstarServerLabel_2->setBuddy(prefServerComboBox);
        confBrightstarServerLabel->setBuddy(prefServerComboBox);
#endif // QT_NO_SHORTCUT

        retranslateUi(Preferences);

        QMetaObject::connectSlotsByName(Preferences);
    } // setupUi

    void retranslateUi(QDialog *Preferences)
    {
        Preferences->setWindowTitle(QApplication::translate("Preferences", "Preferences", Q_NULLPTR));
        generalLabel->setText(QApplication::translate("Preferences", "General setup", Q_NULLPTR));
        label_4->setText(QApplication::translate("Preferences", "Disk space warning [ MB ]", Q_NULLPTR));
        label_9->setText(QApplication::translate("Preferences", "Store intermediate data", Q_NULLPTR));
        confBrightstarServerLabel_2->setText(QApplication::translate("Preferences", "Verbosity", Q_NULLPTR));
        prefVerbosityComboBox->clear();
        prefVerbosityComboBox->insertItems(0, QStringList()
         << QApplication::translate("Preferences", "Minimal", Q_NULLPTR)
         << QApplication::translate("Preferences", "Normal", Q_NULLPTR)
         << QApplication::translate("Preferences", "Full", Q_NULLPTR)
         << QApplication::translate("Preferences", "Debug", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        prefVerbosityComboBox->setToolTip(QApplication::translate("Preferences", "The amount of information shown in the monitor window", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        prefVerbosityComboBox->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_TOOLTIP
        prefDiskspacewarnSpinBox->setToolTip(QApplication::translate("Preferences", "When to show a warning that you are running out of disk space", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        prefProcessSkyCheckBox->setStatusTip(QApplication::translate("Preferences", "When checked, SKY images are treated like science images and fully processed.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        prefProcessSkyCheckBox->setText(QApplication::translate("Preferences", "Fully process SKY images", Q_NULLPTR));
        confBrightstarServerLabel->setText(QApplication::translate("Preferences", "Data server", Q_NULLPTR));
        prefIntermediateDataComboBox->clear();
        prefIntermediateDataComboBox->insertItems(0, QStringList()
         << QApplication::translate("Preferences", "If necessary", Q_NULLPTR)
         << QApplication::translate("Preferences", "Always", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        prefIntermediateDataComboBox->setToolTip(QApplication::translate("Preferences", "Default: Store intermediate data only if necessary (reduced I/O)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        prefServerComboBox->clear();
        prefServerComboBox->insertItems(0, QStringList()
         << QApplication::translate("Preferences", "Canada", Q_NULLPTR)
         << QApplication::translate("Preferences", "China", Q_NULLPTR)
         << QApplication::translate("Preferences", "France", Q_NULLPTR)
         << QApplication::translate("Preferences", "India", Q_NULLPTR)
         << QApplication::translate("Preferences", "Japan", Q_NULLPTR)
         << QApplication::translate("Preferences", "South Africa", Q_NULLPTR)
         << QApplication::translate("Preferences", "UK", Q_NULLPTR)
         << QApplication::translate("Preferences", "USA", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        prefServerComboBox->setToolTip(QApplication::translate("Preferences", "The server from which to download astrometric and photometric reference catalogs", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        parallelizationLabel->setText(QApplication::translate("Preferences", "Parallelization", Q_NULLPTR));
        label_7->setText(QApplication::translate("Preferences", "Max number of CPUs", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        prefCPUSpinBox->setToolTip(QApplication::translate("Preferences", "The maximum number of CPUs THELI may use. This requires that your C++ compiler supports OpenMP v3.0 or later (e.g. GCC v4.4)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        prefCPUSpinBox->setStatusTip(QApplication::translate("Preferences", "Maximum number of CPUs", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        prefCPUSpinBox->setSuffix(QString());
        label_8->setText(QApplication::translate("Preferences", "Max number of I/O threads", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        prefIOthreadsSpinBox->setToolTip(QApplication::translate("Preferences", "The maximum number of simultaneous read / write processes. For modern SSDs there is no penalty. For hard disks choose 2-4, RAIDs can tolerate higher settings. Maximum useful number is the number of CPUs permitted<br>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        prefIOthreadsSpinBox->setStatusTip(QApplication::translate("Preferences", "Maximum number of simultaneous read/write operations", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_TOOLTIP
        prefGPUCheckBox->setToolTip(QApplication::translate("Preferences", "A few tasks profit from GPU parallelization. This requires that you have GCC v6.1 or later, or another compiler that supports the OpenMP 4.5 standard.", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        prefGPUCheckBox->setStatusTip(QApplication::translate("Preferences", "Offload calculations to the graphics card (some tasks, only).", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        prefGPUCheckBox->setText(QApplication::translate("Preferences", "Include GPU in parallelization", Q_NULLPTR));
        memoryLabel->setText(QApplication::translate("Preferences", "Memory usage", Q_NULLPTR));
        prefMemoryLabel->setText(QApplication::translate("Preferences", "Max memory used", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        prefMemorySpinBox->setToolTip(QApplication::translate("Preferences", "The maxmimum amount of RAM THELI may use (time-averaged soft limit)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        prefMemorySpinBox->setSuffix(QApplication::translate("Preferences", " MB", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        prefMemoryCheckBox->setToolTip(QApplication::translate("Preferences", "By default, THELI keeps as much data as possible in memory. Minimizing memory usage results in increased I/O and slower execution.<br>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        prefMemoryCheckBox->setText(QApplication::translate("Preferences", "Minimize memory usage", Q_NULLPTR));
        userInterfaceLabel->setText(QApplication::translate("Preferences", "User interface behaviour", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        prefSwitchProcessMonitorCheckBox->setToolTip(QApplication::translate("Preferences", "When clicking the Start button, THELI will switch to the process monitor to display details of the reduction process.", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        prefSwitchProcessMonitorCheckBox->setText(QApplication::translate("Preferences", "Switch to process monitor after \"Start\"", Q_NULLPTR));
        fontLabel->setText(QApplication::translate("Preferences", "Font settings", Q_NULLPTR));
        prefDefaultFontPushButton->setText(QApplication::translate("Preferences", "Default font", Q_NULLPTR));
        prefFontdialogPushButton->setText(QApplication::translate("Preferences", "Choose font  [...]", Q_NULLPTR));
        label_6->setText(QApplication::translate("Preferences", "Font size", Q_NULLPTR));
        prefCloseButton->setText(QApplication::translate("Preferences", "OK", Q_NULLPTR));
        prefCancelButton->setText(QApplication::translate("Preferences", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Preferences: public Ui_Preferences {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCES_H
