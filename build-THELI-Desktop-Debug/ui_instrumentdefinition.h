/********************************************************************************
** Form generated from reading UI file 'instrumentdefinition.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INSTRUMENTDEFINITION_H
#define UI_INSTRUMENTDEFINITION_H

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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Instrument
{
public:
    QAction *actionClose;
    QWidget *centralwidget;
    QGridLayout *gridLayout_9;
    QFrame *frame1;
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout_3;
    QFrame *inst1Frame;
    QGridLayout *gridLayout_4;
    QLabel *label;
    QLabel *label_5;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_7;
    QSpinBox *numchipSpinBox;
    QLabel *label_3;
    QLineEdit *latitudeLineEdit;
    QLabel *label_4;
    QLineEdit *plateScaleLineEdit;
    QLabel *label_14;
    QLineEdit *longitudeLineEdit;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *bayerCheckBox;
    QFrame *buttonFrame;
    QGridLayout *gridLayout_2;
    QToolButton *bayerRGGBToolButton;
    QToolButton *bayerGBRGToolButton;
    QToolButton *bayerGRBGToolButton;
    QToolButton *bayerBGGRToolButton;
    QLineEdit *instrumentNameLineEdit;
    QComboBox *instrumentTypeComboBox;
    QComboBox *flipComboBox;
    QSpacerItem *horizontalSpacer_9;
    QFrame *inst6Frame;
    QGridLayout *gridLayout;
    QLabel *label_34;
    QLineEdit *optionalKeysLineEdit;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_18;
    QRadioButton *formatFITSRadioButton;
    QRadioButton *formatRAWRadioButton;
    QPushButton *readRAWgeometryPushButton;
    QLabel *subtitle1Label;
    QVBoxLayout *verticalLayout_3;
    QLabel *subtitle2Label;
    QFrame *inst2Frame;
    QGridLayout *gridLayout_8;
    QLabel *label_6;
    QLineEdit *overscanxMinLineEdit;
    QLineEdit *overscanxMinNumLineEdit;
    QLabel *label_7;
    QLineEdit *overscanxMaxLineEdit;
    QLineEdit *overscanxMaxNumLineEdit;
    QLabel *label_16;
    QLineEdit *overscanyMinLineEdit;
    QLineEdit *overscanyMinNumLineEdit;
    QLabel *label_17;
    QLineEdit *overscanyMaxLineEdit;
    QLineEdit *overscanyMaxNumLineEdit;
    QLabel *label_8;
    QLineEdit *startxLineEdit;
    QLineEdit *startxNumLineEdit;
    QLabel *label_9;
    QLineEdit *startyLineEdit;
    QLineEdit *startyNumLineEdit;
    QLabel *label_10;
    QLineEdit *sizexLineEdit;
    QLineEdit *sizexNumLineEdit;
    QLabel *label_11;
    QLineEdit *sizeyLineEdit;
    QLineEdit *sizeyNumLineEdit;
    QLabel *label_12;
    QLineEdit *crpix1LineEdit;
    QLineEdit *crpix1NumLineEdit;
    QLabel *label_13;
    QLineEdit *crpix2LineEdit;
    QLineEdit *crpix2NumLineEdit;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *loadConfigPushButton;
    QPushButton *clearPushButton;
    QPushButton *saveConfigPushButton;
    QSpacerItem *verticalSpacer;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Instrument)
    {
        if (Instrument->objectName().isEmpty())
            Instrument->setObjectName(QStringLiteral("Instrument"));
        Instrument->resize(759, 789);
        actionClose = new QAction(Instrument);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        centralwidget = new QWidget(Instrument);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout_9 = new QGridLayout(centralwidget);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        frame1 = new QFrame(centralwidget);
        frame1->setObjectName(QStringLiteral("frame1"));
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
        frame1->setPalette(palette);
        frame1->setAutoFillBackground(true);
        frame1->setFrameShape(QFrame::Panel);
        frame1->setFrameShadow(QFrame::Sunken);
        frame1->setLineWidth(1);
        gridLayout_5 = new QGridLayout(frame1);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(0);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        inst1Frame = new QFrame(frame1);
        inst1Frame->setObjectName(QStringLiteral("inst1Frame"));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush7(QColor(235, 235, 238, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush8(QColor(245, 245, 246, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        QBrush brush9(QColor(117, 117, 119, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush9);
        QBrush brush10(QColor(157, 157, 159, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        inst1Frame->setPalette(palette1);
        inst1Frame->setAutoFillBackground(true);
        inst1Frame->setFrameShape(QFrame::NoFrame);
        inst1Frame->setLineWidth(0);
        gridLayout_4 = new QGridLayout(inst1Frame);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        label = new QLabel(inst1Frame);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        gridLayout_4->addWidget(label, 0, 0, 1, 1);

        label_5 = new QLabel(inst1Frame);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_4->addWidget(label_5, 1, 0, 1, 1);

        label_2 = new QLabel(inst1Frame);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_4->addWidget(label_2, 2, 0, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(186, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_7, 2, 1, 1, 3);

        numchipSpinBox = new QSpinBox(inst1Frame);
        numchipSpinBox->setObjectName(QStringLiteral("numchipSpinBox"));
        numchipSpinBox->setFocusPolicy(Qt::NoFocus);
        numchipSpinBox->setMinimum(1);
        numchipSpinBox->setMaximum(999);

        gridLayout_4->addWidget(numchipSpinBox, 2, 4, 1, 1);

        label_3 = new QLabel(inst1Frame);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_4->addWidget(label_3, 3, 0, 1, 1);

        latitudeLineEdit = new QLineEdit(inst1Frame);
        latitudeLineEdit->setObjectName(QStringLiteral("latitudeLineEdit"));
        latitudeLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_4->addWidget(latitudeLineEdit, 3, 1, 1, 1);

        label_4 = new QLabel(inst1Frame);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_4->addWidget(label_4, 3, 2, 1, 1);

        plateScaleLineEdit = new QLineEdit(inst1Frame);
        plateScaleLineEdit->setObjectName(QStringLiteral("plateScaleLineEdit"));
        plateScaleLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_4->addWidget(plateScaleLineEdit, 3, 3, 1, 2);

        label_14 = new QLabel(inst1Frame);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_4->addWidget(label_14, 4, 0, 1, 1);

        longitudeLineEdit = new QLineEdit(inst1Frame);
        longitudeLineEdit->setObjectName(QStringLiteral("longitudeLineEdit"));
        longitudeLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_4->addWidget(longitudeLineEdit, 4, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        bayerCheckBox = new QCheckBox(inst1Frame);
        bayerCheckBox->setObjectName(QStringLiteral("bayerCheckBox"));
        bayerCheckBox->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_4->addWidget(bayerCheckBox);

        buttonFrame = new QFrame(inst1Frame);
        buttonFrame->setObjectName(QStringLiteral("buttonFrame"));
        buttonFrame->setFrameShape(QFrame::NoFrame);
        buttonFrame->setFrameShadow(QFrame::Raised);
        buttonFrame->setLineWidth(0);
        gridLayout_2 = new QGridLayout(buttonFrame);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        bayerRGGBToolButton = new QToolButton(buttonFrame);
        bayerRGGBToolButton->setObjectName(QStringLiteral("bayerRGGBToolButton"));
        bayerRGGBToolButton->setMinimumSize(QSize(44, 44));
        bayerRGGBToolButton->setMaximumSize(QSize(44, 44));
        bayerRGGBToolButton->setFocusPolicy(Qt::NoFocus);
        bayerRGGBToolButton->setIconSize(QSize(40, 40));
        bayerRGGBToolButton->setCheckable(true);
        bayerRGGBToolButton->setChecked(true);
        bayerRGGBToolButton->setAutoExclusive(true);

        gridLayout_2->addWidget(bayerRGGBToolButton, 0, 0, 1, 1);

        bayerGBRGToolButton = new QToolButton(buttonFrame);
        bayerGBRGToolButton->setObjectName(QStringLiteral("bayerGBRGToolButton"));
        bayerGBRGToolButton->setMinimumSize(QSize(44, 44));
        bayerGBRGToolButton->setMaximumSize(QSize(44, 44));
        bayerGBRGToolButton->setFocusPolicy(Qt::NoFocus);
        bayerGBRGToolButton->setIconSize(QSize(40, 40));
        bayerGBRGToolButton->setCheckable(true);
        bayerGBRGToolButton->setAutoExclusive(true);

        gridLayout_2->addWidget(bayerGBRGToolButton, 0, 1, 1, 1);

        bayerGRBGToolButton = new QToolButton(buttonFrame);
        bayerGRBGToolButton->setObjectName(QStringLiteral("bayerGRBGToolButton"));
        bayerGRBGToolButton->setMinimumSize(QSize(44, 44));
        bayerGRBGToolButton->setMaximumSize(QSize(44, 44));
        bayerGRBGToolButton->setFocusPolicy(Qt::NoFocus);
        bayerGRBGToolButton->setIconSize(QSize(40, 40));
        bayerGRBGToolButton->setCheckable(true);
        bayerGRBGToolButton->setAutoExclusive(true);

        gridLayout_2->addWidget(bayerGRBGToolButton, 0, 2, 1, 1);

        bayerBGGRToolButton = new QToolButton(buttonFrame);
        bayerBGGRToolButton->setObjectName(QStringLiteral("bayerBGGRToolButton"));
        bayerBGGRToolButton->setMinimumSize(QSize(44, 44));
        bayerBGGRToolButton->setMaximumSize(QSize(44, 44));
        bayerBGGRToolButton->setFocusPolicy(Qt::NoFocus);
        bayerBGGRToolButton->setIconSize(QSize(40, 40));
        bayerBGGRToolButton->setCheckable(true);
        bayerBGGRToolButton->setAutoExclusive(true);

        gridLayout_2->addWidget(bayerBGGRToolButton, 0, 3, 1, 1);


        horizontalLayout_4->addWidget(buttonFrame);


        gridLayout_4->addLayout(horizontalLayout_4, 5, 0, 1, 5);

        instrumentNameLineEdit = new QLineEdit(inst1Frame);
        instrumentNameLineEdit->setObjectName(QStringLiteral("instrumentNameLineEdit"));
        instrumentNameLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_4->addWidget(instrumentNameLineEdit, 0, 1, 1, 4);

        instrumentTypeComboBox = new QComboBox(inst1Frame);
        instrumentTypeComboBox->setObjectName(QStringLiteral("instrumentTypeComboBox"));
        instrumentTypeComboBox->setFocusPolicy(Qt::NoFocus);

        gridLayout_4->addWidget(instrumentTypeComboBox, 1, 1, 1, 4);

        flipComboBox = new QComboBox(inst1Frame);
        flipComboBox->setObjectName(QStringLiteral("flipComboBox"));

        gridLayout_4->addWidget(flipComboBox, 4, 2, 1, 3);


        gridLayout_3->addWidget(inst1Frame, 1, 0, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(18, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_9, 1, 1, 1, 1);

        inst6Frame = new QFrame(frame1);
        inst6Frame->setObjectName(QStringLiteral("inst6Frame"));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette2.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette2.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette2.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush);
        palette2.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette2.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette2.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette2.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette2.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette2.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette2.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette2.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette2.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette2.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette2.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette2.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        inst6Frame->setPalette(palette2);
        inst6Frame->setAutoFillBackground(true);
        inst6Frame->setFrameShape(QFrame::NoFrame);
        inst6Frame->setFrameShadow(QFrame::Raised);
        inst6Frame->setLineWidth(0);
        gridLayout = new QGridLayout(inst6Frame);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_34 = new QLabel(inst6Frame);
        label_34->setObjectName(QStringLiteral("label_34"));

        gridLayout->addWidget(label_34, 0, 0, 1, 1);

        optionalKeysLineEdit = new QLineEdit(inst6Frame);
        optionalKeysLineEdit->setObjectName(QStringLiteral("optionalKeysLineEdit"));

        gridLayout->addWidget(optionalKeysLineEdit, 1, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_3, 2, 0, 1, 1);

        label_18 = new QLabel(inst6Frame);
        label_18->setObjectName(QStringLiteral("label_18"));

        gridLayout->addWidget(label_18, 3, 0, 1, 1);

        formatFITSRadioButton = new QRadioButton(inst6Frame);
        formatFITSRadioButton->setObjectName(QStringLiteral("formatFITSRadioButton"));
        formatFITSRadioButton->setChecked(true);

        gridLayout->addWidget(formatFITSRadioButton, 4, 0, 1, 1);

        formatRAWRadioButton = new QRadioButton(inst6Frame);
        formatRAWRadioButton->setObjectName(QStringLiteral("formatRAWRadioButton"));

        gridLayout->addWidget(formatRAWRadioButton, 5, 0, 1, 1);

        readRAWgeometryPushButton = new QPushButton(inst6Frame);
        readRAWgeometryPushButton->setObjectName(QStringLiteral("readRAWgeometryPushButton"));

        gridLayout->addWidget(readRAWgeometryPushButton, 6, 0, 1, 1);


        gridLayout_3->addWidget(inst6Frame, 1, 2, 1, 1);

        subtitle1Label = new QLabel(frame1);
        subtitle1Label->setObjectName(QStringLiteral("subtitle1Label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        subtitle1Label->setFont(font);

        gridLayout_3->addWidget(subtitle1Label, 0, 0, 1, 3);


        gridLayout_5->addLayout(gridLayout_3, 0, 0, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        subtitle2Label = new QLabel(frame1);
        subtitle2Label->setObjectName(QStringLiteral("subtitle2Label"));
        subtitle2Label->setFont(font);

        verticalLayout_3->addWidget(subtitle2Label);

        inst2Frame = new QFrame(frame1);
        inst2Frame->setObjectName(QStringLiteral("inst2Frame"));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette3.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette3.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette3.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush);
        palette3.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette3.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette3.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette3.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette3.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        inst2Frame->setPalette(palette3);
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        inst2Frame->setFont(font1);
        inst2Frame->setAutoFillBackground(true);
        inst2Frame->setFrameShape(QFrame::NoFrame);
        inst2Frame->setLineWidth(0);
        gridLayout_8 = new QGridLayout(inst2Frame);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        label_6 = new QLabel(inst2Frame);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_8->addWidget(label_6, 0, 0, 1, 1);

        overscanxMinLineEdit = new QLineEdit(inst2Frame);
        overscanxMinLineEdit->setObjectName(QStringLiteral("overscanxMinLineEdit"));

        gridLayout_8->addWidget(overscanxMinLineEdit, 0, 1, 1, 1);

        overscanxMinNumLineEdit = new QLineEdit(inst2Frame);
        overscanxMinNumLineEdit->setObjectName(QStringLiteral("overscanxMinNumLineEdit"));
        overscanxMinNumLineEdit->setMinimumSize(QSize(60, 0));
        overscanxMinNumLineEdit->setMaximumSize(QSize(60, 16777215));
        overscanxMinNumLineEdit->setFocusPolicy(Qt::NoFocus);
        overscanxMinNumLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        overscanxMinNumLineEdit->setReadOnly(true);

        gridLayout_8->addWidget(overscanxMinNumLineEdit, 0, 2, 1, 1);

        label_7 = new QLabel(inst2Frame);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_8->addWidget(label_7, 1, 0, 1, 1);

        overscanxMaxLineEdit = new QLineEdit(inst2Frame);
        overscanxMaxLineEdit->setObjectName(QStringLiteral("overscanxMaxLineEdit"));

        gridLayout_8->addWidget(overscanxMaxLineEdit, 1, 1, 1, 1);

        overscanxMaxNumLineEdit = new QLineEdit(inst2Frame);
        overscanxMaxNumLineEdit->setObjectName(QStringLiteral("overscanxMaxNumLineEdit"));
        overscanxMaxNumLineEdit->setMinimumSize(QSize(60, 0));
        overscanxMaxNumLineEdit->setMaximumSize(QSize(60, 16777215));
        overscanxMaxNumLineEdit->setFocusPolicy(Qt::NoFocus);
        overscanxMaxNumLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        overscanxMaxNumLineEdit->setReadOnly(true);

        gridLayout_8->addWidget(overscanxMaxNumLineEdit, 1, 2, 1, 1);

        label_16 = new QLabel(inst2Frame);
        label_16->setObjectName(QStringLiteral("label_16"));

        gridLayout_8->addWidget(label_16, 2, 0, 1, 1);

        overscanyMinLineEdit = new QLineEdit(inst2Frame);
        overscanyMinLineEdit->setObjectName(QStringLiteral("overscanyMinLineEdit"));

        gridLayout_8->addWidget(overscanyMinLineEdit, 2, 1, 1, 1);

        overscanyMinNumLineEdit = new QLineEdit(inst2Frame);
        overscanyMinNumLineEdit->setObjectName(QStringLiteral("overscanyMinNumLineEdit"));
        overscanyMinNumLineEdit->setMinimumSize(QSize(60, 0));
        overscanyMinNumLineEdit->setMaximumSize(QSize(60, 16777215));
        overscanyMinNumLineEdit->setFocusPolicy(Qt::NoFocus);
        overscanyMinNumLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        overscanyMinNumLineEdit->setReadOnly(true);

        gridLayout_8->addWidget(overscanyMinNumLineEdit, 2, 2, 1, 1);

        label_17 = new QLabel(inst2Frame);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout_8->addWidget(label_17, 3, 0, 1, 1);

        overscanyMaxLineEdit = new QLineEdit(inst2Frame);
        overscanyMaxLineEdit->setObjectName(QStringLiteral("overscanyMaxLineEdit"));

        gridLayout_8->addWidget(overscanyMaxLineEdit, 3, 1, 1, 1);

        overscanyMaxNumLineEdit = new QLineEdit(inst2Frame);
        overscanyMaxNumLineEdit->setObjectName(QStringLiteral("overscanyMaxNumLineEdit"));
        overscanyMaxNumLineEdit->setMinimumSize(QSize(60, 0));
        overscanyMaxNumLineEdit->setMaximumSize(QSize(60, 16777215));
        overscanyMaxNumLineEdit->setFocusPolicy(Qt::NoFocus);
        overscanyMaxNumLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        overscanyMaxNumLineEdit->setReadOnly(true);

        gridLayout_8->addWidget(overscanyMaxNumLineEdit, 3, 2, 1, 1);

        label_8 = new QLabel(inst2Frame);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_8->addWidget(label_8, 4, 0, 1, 1);

        startxLineEdit = new QLineEdit(inst2Frame);
        startxLineEdit->setObjectName(QStringLiteral("startxLineEdit"));

        gridLayout_8->addWidget(startxLineEdit, 4, 1, 1, 1);

        startxNumLineEdit = new QLineEdit(inst2Frame);
        startxNumLineEdit->setObjectName(QStringLiteral("startxNumLineEdit"));
        startxNumLineEdit->setMinimumSize(QSize(60, 0));
        startxNumLineEdit->setMaximumSize(QSize(60, 16777215));
        startxNumLineEdit->setFocusPolicy(Qt::NoFocus);
        startxNumLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        startxNumLineEdit->setReadOnly(true);

        gridLayout_8->addWidget(startxNumLineEdit, 4, 2, 1, 1);

        label_9 = new QLabel(inst2Frame);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_8->addWidget(label_9, 5, 0, 1, 1);

        startyLineEdit = new QLineEdit(inst2Frame);
        startyLineEdit->setObjectName(QStringLiteral("startyLineEdit"));

        gridLayout_8->addWidget(startyLineEdit, 5, 1, 1, 1);

        startyNumLineEdit = new QLineEdit(inst2Frame);
        startyNumLineEdit->setObjectName(QStringLiteral("startyNumLineEdit"));
        startyNumLineEdit->setMinimumSize(QSize(60, 0));
        startyNumLineEdit->setMaximumSize(QSize(60, 16777215));
        startyNumLineEdit->setFocusPolicy(Qt::NoFocus);
        startyNumLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        startyNumLineEdit->setReadOnly(true);

        gridLayout_8->addWidget(startyNumLineEdit, 5, 2, 1, 1);

        label_10 = new QLabel(inst2Frame);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_8->addWidget(label_10, 6, 0, 1, 1);

        sizexLineEdit = new QLineEdit(inst2Frame);
        sizexLineEdit->setObjectName(QStringLiteral("sizexLineEdit"));

        gridLayout_8->addWidget(sizexLineEdit, 6, 1, 1, 1);

        sizexNumLineEdit = new QLineEdit(inst2Frame);
        sizexNumLineEdit->setObjectName(QStringLiteral("sizexNumLineEdit"));
        sizexNumLineEdit->setMinimumSize(QSize(60, 0));
        sizexNumLineEdit->setMaximumSize(QSize(60, 16777215));
        sizexNumLineEdit->setFocusPolicy(Qt::NoFocus);
        sizexNumLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        sizexNumLineEdit->setReadOnly(true);

        gridLayout_8->addWidget(sizexNumLineEdit, 6, 2, 1, 1);

        label_11 = new QLabel(inst2Frame);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_8->addWidget(label_11, 7, 0, 1, 1);

        sizeyLineEdit = new QLineEdit(inst2Frame);
        sizeyLineEdit->setObjectName(QStringLiteral("sizeyLineEdit"));

        gridLayout_8->addWidget(sizeyLineEdit, 7, 1, 1, 1);

        sizeyNumLineEdit = new QLineEdit(inst2Frame);
        sizeyNumLineEdit->setObjectName(QStringLiteral("sizeyNumLineEdit"));
        sizeyNumLineEdit->setMinimumSize(QSize(60, 0));
        sizeyNumLineEdit->setMaximumSize(QSize(60, 16777215));
        sizeyNumLineEdit->setFocusPolicy(Qt::NoFocus);
        sizeyNumLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        sizeyNumLineEdit->setReadOnly(true);

        gridLayout_8->addWidget(sizeyNumLineEdit, 7, 2, 1, 1);

        label_12 = new QLabel(inst2Frame);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_8->addWidget(label_12, 8, 0, 1, 1);

        crpix1LineEdit = new QLineEdit(inst2Frame);
        crpix1LineEdit->setObjectName(QStringLiteral("crpix1LineEdit"));

        gridLayout_8->addWidget(crpix1LineEdit, 8, 1, 1, 1);

        crpix1NumLineEdit = new QLineEdit(inst2Frame);
        crpix1NumLineEdit->setObjectName(QStringLiteral("crpix1NumLineEdit"));
        crpix1NumLineEdit->setMinimumSize(QSize(60, 0));
        crpix1NumLineEdit->setMaximumSize(QSize(60, 16777215));
        crpix1NumLineEdit->setFocusPolicy(Qt::NoFocus);
        crpix1NumLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        crpix1NumLineEdit->setReadOnly(true);

        gridLayout_8->addWidget(crpix1NumLineEdit, 8, 2, 1, 1);

        label_13 = new QLabel(inst2Frame);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_8->addWidget(label_13, 9, 0, 1, 1);

        crpix2LineEdit = new QLineEdit(inst2Frame);
        crpix2LineEdit->setObjectName(QStringLiteral("crpix2LineEdit"));

        gridLayout_8->addWidget(crpix2LineEdit, 9, 1, 1, 1);

        crpix2NumLineEdit = new QLineEdit(inst2Frame);
        crpix2NumLineEdit->setObjectName(QStringLiteral("crpix2NumLineEdit"));
        crpix2NumLineEdit->setMinimumSize(QSize(60, 0));
        crpix2NumLineEdit->setMaximumSize(QSize(60, 16777215));
        crpix2NumLineEdit->setFocusPolicy(Qt::NoFocus);
        crpix2NumLineEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        crpix2NumLineEdit->setReadOnly(true);

        gridLayout_8->addWidget(crpix2NumLineEdit, 9, 2, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_8->addItem(verticalSpacer_4, 10, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_5 = new QSpacerItem(188, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        loadConfigPushButton = new QPushButton(inst2Frame);
        loadConfigPushButton->setObjectName(QStringLiteral("loadConfigPushButton"));
        loadConfigPushButton->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_2->addWidget(loadConfigPushButton);

        clearPushButton = new QPushButton(inst2Frame);
        clearPushButton->setObjectName(QStringLiteral("clearPushButton"));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette4.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette4.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette4.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush);
        palette4.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette4.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette4.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette4.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette4.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette4.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette4.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette4.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette4.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette4.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette4.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        clearPushButton->setPalette(palette4);
        clearPushButton->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_2->addWidget(clearPushButton);

        saveConfigPushButton = new QPushButton(inst2Frame);
        saveConfigPushButton->setObjectName(QStringLiteral("saveConfigPushButton"));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush11(QColor(68, 216, 204, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette5.setBrush(QPalette::Active, QPalette::Button, brush11);
        QBrush brush12(QColor(149, 255, 247, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette5.setBrush(QPalette::Active, QPalette::Light, brush12);
        QBrush brush13(QColor(108, 235, 225, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette5.setBrush(QPalette::Active, QPalette::Midlight, brush13);
        QBrush brush14(QColor(34, 108, 102, 255));
        brush14.setStyle(Qt::SolidPattern);
        palette5.setBrush(QPalette::Active, QPalette::Dark, brush14);
        QBrush brush15(QColor(45, 144, 136, 255));
        brush15.setStyle(Qt::SolidPattern);
        palette5.setBrush(QPalette::Active, QPalette::Mid, brush15);
        palette5.setBrush(QPalette::Active, QPalette::Text, brush);
        palette5.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette5.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush11);
        palette5.setBrush(QPalette::Active, QPalette::Shadow, brush);
        QBrush brush16(QColor(161, 235, 229, 255));
        brush16.setStyle(Qt::SolidPattern);
        palette5.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
        palette5.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette5.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette5.setBrush(QPalette::Inactive, QPalette::Light, brush12);
        palette5.setBrush(QPalette::Inactive, QPalette::Midlight, brush13);
        palette5.setBrush(QPalette::Inactive, QPalette::Dark, brush14);
        palette5.setBrush(QPalette::Inactive, QPalette::Mid, brush15);
        palette5.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush11);
        palette5.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush16);
        palette5.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette5.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush14);
        palette5.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette5.setBrush(QPalette::Disabled, QPalette::Light, brush12);
        palette5.setBrush(QPalette::Disabled, QPalette::Midlight, brush13);
        palette5.setBrush(QPalette::Disabled, QPalette::Dark, brush14);
        palette5.setBrush(QPalette::Disabled, QPalette::Mid, brush15);
        palette5.setBrush(QPalette::Disabled, QPalette::Text, brush14);
        palette5.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::ButtonText, brush14);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush11);
        palette5.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush11);
        palette5.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette5.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        saveConfigPushButton->setPalette(palette5);
        saveConfigPushButton->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_2->addWidget(saveConfigPushButton);


        gridLayout_8->addLayout(horizontalLayout_2, 11, 0, 1, 3);


        verticalLayout_3->addWidget(inst2Frame);


        gridLayout_5->addLayout(verticalLayout_3, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer, 2, 0, 1, 1);


        gridLayout_9->addWidget(frame1, 0, 0, 1, 1);

        Instrument->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Instrument);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 759, 25));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        Instrument->setMenuBar(menubar);
        statusbar = new QStatusBar(Instrument);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        Instrument->setStatusBar(statusbar);
        QWidget::setTabOrder(instrumentNameLineEdit, latitudeLineEdit);
        QWidget::setTabOrder(latitudeLineEdit, longitudeLineEdit);
        QWidget::setTabOrder(longitudeLineEdit, plateScaleLineEdit);
        QWidget::setTabOrder(plateScaleLineEdit, overscanxMinLineEdit);
        QWidget::setTabOrder(overscanxMinLineEdit, overscanxMaxLineEdit);
        QWidget::setTabOrder(overscanxMaxLineEdit, overscanyMinLineEdit);
        QWidget::setTabOrder(overscanyMinLineEdit, overscanyMaxLineEdit);
        QWidget::setTabOrder(overscanyMaxLineEdit, startxLineEdit);
        QWidget::setTabOrder(startxLineEdit, startyLineEdit);
        QWidget::setTabOrder(startyLineEdit, sizexLineEdit);
        QWidget::setTabOrder(sizexLineEdit, sizeyLineEdit);
        QWidget::setTabOrder(sizeyLineEdit, crpix1LineEdit);
        QWidget::setTabOrder(crpix1LineEdit, crpix2LineEdit);
        QWidget::setTabOrder(crpix2LineEdit, optionalKeysLineEdit);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionClose);

        retranslateUi(Instrument);

        QMetaObject::connectSlotsByName(Instrument);
    } // setupUi

    void retranslateUi(QMainWindow *Instrument)
    {
        Instrument->setWindowTitle(QApplication::translate("Instrument", "Setup for a new instrument configuration", Q_NULLPTR));
        actionClose->setText(QApplication::translate("Instrument", "Close", Q_NULLPTR));
        label->setText(QApplication::translate("Instrument", "Instrument name", Q_NULLPTR));
        label_5->setText(QApplication::translate("Instrument", "Type", Q_NULLPTR));
        label_2->setText(QApplication::translate("Instrument", "Number of chips", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        numchipSpinBox->setStatusTip(QApplication::translate("Instrument", "The number of detectors used in this instrument.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_3->setText(QApplication::translate("Instrument", "Geo. latitude", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        latitudeLineEdit->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        latitudeLineEdit->setStatusTip(QApplication::translate("Instrument", "Geographic latitude of the observatory", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_4->setText(QApplication::translate("Instrument", "Scale [ \" / pix ]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        plateScaleLineEdit->setStatusTip(QApplication::translate("Instrument", "The plate scale in arcsecond per pixel", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_14->setText(QApplication::translate("Instrument", "Geo. longitude", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        longitudeLineEdit->setStatusTip(QApplication::translate("Instrument", "Geographic longitude of the observatory. West counts negative.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        bayerCheckBox->setStatusTip(QApplication::translate("Instrument", "A colored pixel mask commonly found in digital cameras and some amateur CCD cameras.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        bayerCheckBox->setText(QApplication::translate("Instrument", "Detector has Bayer matrix", Q_NULLPTR));
        bayerRGGBToolButton->setText(QString());
        bayerGBRGToolButton->setText(QString());
        bayerGRBGToolButton->setText(QString());
        bayerBGGRToolButton->setText(QString());
#ifndef QT_NO_STATUSTIP
        instrumentNameLineEdit->setStatusTip(QApplication::translate("Instrument", "Suggested name structure: INSTRUMENT@TELESCOPE", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        instrumentTypeComboBox->clear();
        instrumentTypeComboBox->insertItems(0, QStringList()
         << QApplication::translate("Instrument", "OPTICAL", Q_NULLPTR)
         << QApplication::translate("Instrument", "Near-IR (1-2.5 micron)", Q_NULLPTR)
         << QApplication::translate("Instrument", "Near/mid-IR (1-5 micron)", Q_NULLPTR)
         << QApplication::translate("Instrument", "Mid-IR (>5 micron)", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        instrumentTypeComboBox->setToolTip(QApplication::translate("Instrument", "The type determines which elements in the interface will be shown or hidden.", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        instrumentTypeComboBox->setStatusTip(QApplication::translate("Instrument", "Operative wavelength range. THELI will hide and show unrelevant and relevant parts, respectively.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        flipComboBox->clear();
        flipComboBox->insertItems(0, QStringList()
         << QApplication::translate("Instrument", "Do not flip WCS", Q_NULLPTR)
         << QApplication::translate("Instrument", "Flip WCS horizontally", Q_NULLPTR)
         << QApplication::translate("Instrument", "Flip WCS vertically", Q_NULLPTR)
         << QApplication::translate("Instrument", "Rotate WCS by 180 deg", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        flipComboBox->setStatusTip(QApplication::translate("Instrument", "Flip the WCS matrix to simplify astrometric solutions. ONLY IF CD MATRIX IS ABSENT IN RAW DATA", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_34->setText(QApplication::translate("Instrument", "Propagate keywords to new FITS headers", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        optionalKeysLineEdit->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        optionalKeysLineEdit->setStatusTip(QApplication::translate("Instrument", "A blank- or comma-separated list of keywords that should be propagated to the new FITS header.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        optionalKeysLineEdit->setText(QString());
        label_18->setText(QApplication::translate("Instrument", "Raw data format", Q_NULLPTR));
        formatFITSRadioButton->setText(QApplication::translate("Instrument", "FITS", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        formatRAWRadioButton->setStatusTip(QApplication::translate("Instrument", "Typical data format of a commercial CMOS camera", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        formatRAWRadioButton->setText(QApplication::translate("Instrument", "RAW (*.CR2, *.ARW, *.DNG etc)", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        readRAWgeometryPushButton->setStatusTip(QApplication::translate("Instrument", "Load a CMOS RAW file and auto-fill the geometry", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        readRAWgeometryPushButton->setText(QApplication::translate("Instrument", "Load RAW geometry from file ...", Q_NULLPTR));
        subtitle1Label->setText(QApplication::translate("Instrument", "(1) General properties", Q_NULLPTR));
        subtitle2Label->setText(QApplication::translate("Instrument", "(2) Detector geometries (FITS only; geometry of RAW formats must be auto-loaded)", Q_NULLPTR));
        label_6->setText(QApplication::translate("Instrument", "Overscan xmin", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        overscanxMinLineEdit->setToolTip(QApplication::translate("Instrument", "Blank-separated list, as many entries as detectors", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        overscanxMinLineEdit->setStatusTip(QApplication::translate("Instrument", "The min and max overscan pixels. If the camera has no overscan, enter 0. ", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        overscanxMinNumLineEdit->setStatusTip(QApplication::translate("Instrument", "The number of entries made to the left. Must match the number of chips.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        overscanxMinNumLineEdit->setText(QApplication::translate("Instrument", "0", Q_NULLPTR));
        label_7->setText(QApplication::translate("Instrument", "Overscan xmax", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        overscanxMaxLineEdit->setToolTip(QApplication::translate("Instrument", "Blank-separated list, as many entries as detectors", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        overscanxMaxLineEdit->setStatusTip(QApplication::translate("Instrument", "The min and max overscan pixels. If the camera has no overscan, enter 0. ", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        overscanxMaxNumLineEdit->setStatusTip(QApplication::translate("Instrument", "The number of entries made to the left. Must match the number of chips.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        overscanxMaxNumLineEdit->setText(QApplication::translate("Instrument", "0", Q_NULLPTR));
        label_16->setText(QApplication::translate("Instrument", "Overscan ymin", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        overscanyMinLineEdit->setToolTip(QApplication::translate("Instrument", "Blank-separated list, as many entries as detectors", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        overscanyMinLineEdit->setStatusTip(QApplication::translate("Instrument", "The min and max overscan pixels. If the camera has no overscan, enter 0. ", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        overscanyMinNumLineEdit->setStatusTip(QApplication::translate("Instrument", "The number of entries made to the left. Must match the number of chips.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        overscanyMinNumLineEdit->setText(QApplication::translate("Instrument", "0", Q_NULLPTR));
        label_17->setText(QApplication::translate("Instrument", "Overscan ymax", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        overscanyMaxLineEdit->setToolTip(QApplication::translate("Instrument", "Blank-separated list, as many entries as detectors", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        overscanyMaxLineEdit->setStatusTip(QApplication::translate("Instrument", "The min and max overscan pixels. If the camera has no overscan, enter 0. ", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        overscanyMaxNumLineEdit->setStatusTip(QApplication::translate("Instrument", "The number of entries made to the left. Must match the number of chips.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        overscanyMaxNumLineEdit->setText(QApplication::translate("Instrument", "0", Q_NULLPTR));
        label_8->setText(QApplication::translate("Instrument", "Start x", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        startxLineEdit->setToolTip(QApplication::translate("Instrument", "Blank-separated list, as many entries as detectors", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        startxLineEdit->setStatusTip(QApplication::translate("Instrument", "The first illuminated pixel along the horizontal axis", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        startxNumLineEdit->setStatusTip(QApplication::translate("Instrument", "The number of entries made to the left. Must match the number of chips.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        startxNumLineEdit->setText(QApplication::translate("Instrument", "0", Q_NULLPTR));
        label_9->setText(QApplication::translate("Instrument", "Start y", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        startyLineEdit->setToolTip(QApplication::translate("Instrument", "Blank-separated list, as many entries as detectors", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        startyLineEdit->setStatusTip(QApplication::translate("Instrument", "The first illuminated pixel along the vertical axis", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        startyNumLineEdit->setStatusTip(QApplication::translate("Instrument", "The number of entries made to the left. Must match the number of chips.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        startyNumLineEdit->setText(QApplication::translate("Instrument", "0", Q_NULLPTR));
        label_10->setText(QApplication::translate("Instrument", "Size x", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        sizexLineEdit->setToolTip(QApplication::translate("Instrument", "Blank-separated list, as many entries as detectors", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        sizexLineEdit->setStatusTip(QApplication::translate("Instrument", "The number of illuminated pixels along the horizontal axis", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        sizexNumLineEdit->setStatusTip(QApplication::translate("Instrument", "The number of entries made to the left. Must match the number of chips.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        sizexNumLineEdit->setText(QApplication::translate("Instrument", "0", Q_NULLPTR));
        label_11->setText(QApplication::translate("Instrument", "Size y", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        sizeyLineEdit->setToolTip(QApplication::translate("Instrument", "Blank-separated list, as many entries as detectors", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        sizeyLineEdit->setStatusTip(QApplication::translate("Instrument", "The number of illuminated pixels along the vertical axis", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        sizeyNumLineEdit->setStatusTip(QApplication::translate("Instrument", "The number of entries made to the left. Must match the number of chips.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        sizeyNumLineEdit->setText(QApplication::translate("Instrument", "0", Q_NULLPTR));
        label_12->setText(QApplication::translate("Instrument", "CRPIX1", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        crpix1LineEdit->setToolTip(QApplication::translate("Instrument", "Blank-separated list, as many entries as detectors", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        crpix1LineEdit->setText(QString());
#ifndef QT_NO_STATUSTIP
        crpix1NumLineEdit->setStatusTip(QApplication::translate("Instrument", "The number of entries made to the left. Must match the number of chips.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        crpix1NumLineEdit->setText(QApplication::translate("Instrument", "0", Q_NULLPTR));
        label_13->setText(QApplication::translate("Instrument", "CRPIX2", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        crpix2LineEdit->setToolTip(QApplication::translate("Instrument", "Blank-separated list, as many entries as detectors", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        crpix2LineEdit->setText(QString());
#ifndef QT_NO_STATUSTIP
        crpix2NumLineEdit->setStatusTip(QApplication::translate("Instrument", "The number of entries made to the left. Must match the number of chips.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        crpix2NumLineEdit->setText(QApplication::translate("Instrument", "0", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        loadConfigPushButton->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        loadConfigPushButton->setStatusTip(QApplication::translate("Instrument", "Select any existing instrument configuration as an example.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        loadConfigPushButton->setText(QApplication::translate("Instrument", "Load config", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        clearPushButton->setStatusTip(QApplication::translate("Instrument", "Resets all fields in this dialog.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        clearPushButton->setText(QApplication::translate("Instrument", "Reset all fields", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        saveConfigPushButton->setStatusTip(QApplication::translate("Instrument", "Saves the configuration to $HOME/.theli/instrument_user/", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        saveConfigPushButton->setText(QApplication::translate("Instrument", "Save config", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("Instrument", "File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Instrument: public Ui_Instrument {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INSTRUMENTDEFINITION_H
