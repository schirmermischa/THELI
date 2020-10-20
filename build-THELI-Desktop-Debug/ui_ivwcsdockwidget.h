/********************************************************************************
** Form generated from reading UI file 'ivwcsdockwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IVWCSDOCKWIDGET_H
#define UI_IVWCSDOCKWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IvWCSDockWidget
{
public:
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout;
    QLineEdit *cd11LineEdit;
    QSlider *cd11Slider;
    QLabel *label_2;
    QLineEdit *cd12LineEdit;
    QLineEdit *posAngleLineEdit;
    QSlider *posAngleSlider;
    QPushButton *resetPushButton;
    QSlider *cd12Slider;
    QLabel *label_3;
    QLineEdit *cd21LineEdit;
    QSlider *cd21Slider;
    QLabel *label_4;
    QLineEdit *cd22LineEdit;
    QSlider *cd22Slider;
    QLabel *label_5;
    QLineEdit *plateScaleLineEdit;
    QSlider *plateScaleSlider;
    QLabel *label_6;
    QLabel *label;
    QSpacerItem *verticalSpacer;

    void setupUi(QDockWidget *IvWCSDockWidget)
    {
        if (IvWCSDockWidget->objectName().isEmpty())
            IvWCSDockWidget->setObjectName(QStringLiteral("IvWCSDockWidget"));
        IvWCSDockWidget->resize(186, 404);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        gridLayout = new QGridLayout(dockWidgetContents);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        cd11LineEdit = new QLineEdit(dockWidgetContents);
        cd11LineEdit->setObjectName(QStringLiteral("cd11LineEdit"));

        gridLayout->addWidget(cd11LineEdit, 0, 1, 1, 2);

        cd11Slider = new QSlider(dockWidgetContents);
        cd11Slider->setObjectName(QStringLiteral("cd11Slider"));
        cd11Slider->setMinimum(-100);
        cd11Slider->setMaximum(100);
        cd11Slider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(cd11Slider, 1, 0, 1, 3);

        label_2 = new QLabel(dockWidgetContents);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        cd12LineEdit = new QLineEdit(dockWidgetContents);
        cd12LineEdit->setObjectName(QStringLiteral("cd12LineEdit"));

        gridLayout->addWidget(cd12LineEdit, 2, 1, 1, 2);

        posAngleLineEdit = new QLineEdit(dockWidgetContents);
        posAngleLineEdit->setObjectName(QStringLiteral("posAngleLineEdit"));

        gridLayout->addWidget(posAngleLineEdit, 10, 2, 1, 1);

        posAngleSlider = new QSlider(dockWidgetContents);
        posAngleSlider->setObjectName(QStringLiteral("posAngleSlider"));
        posAngleSlider->setMinimum(-100);
        posAngleSlider->setMaximum(100);
        posAngleSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(posAngleSlider, 11, 0, 1, 3);

        resetPushButton = new QPushButton(dockWidgetContents);
        resetPushButton->setObjectName(QStringLiteral("resetPushButton"));

        gridLayout->addWidget(resetPushButton, 12, 0, 1, 3);

        cd12Slider = new QSlider(dockWidgetContents);
        cd12Slider->setObjectName(QStringLiteral("cd12Slider"));
        cd12Slider->setMinimum(-100);
        cd12Slider->setMaximum(100);
        cd12Slider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(cd12Slider, 3, 0, 1, 3);

        label_3 = new QLabel(dockWidgetContents);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        cd21LineEdit = new QLineEdit(dockWidgetContents);
        cd21LineEdit->setObjectName(QStringLiteral("cd21LineEdit"));

        gridLayout->addWidget(cd21LineEdit, 4, 1, 1, 2);

        cd21Slider = new QSlider(dockWidgetContents);
        cd21Slider->setObjectName(QStringLiteral("cd21Slider"));
        cd21Slider->setMinimum(-100);
        cd21Slider->setMaximum(100);
        cd21Slider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(cd21Slider, 5, 0, 1, 3);

        label_4 = new QLabel(dockWidgetContents);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 6, 0, 1, 1);

        cd22LineEdit = new QLineEdit(dockWidgetContents);
        cd22LineEdit->setObjectName(QStringLiteral("cd22LineEdit"));

        gridLayout->addWidget(cd22LineEdit, 6, 1, 1, 2);

        cd22Slider = new QSlider(dockWidgetContents);
        cd22Slider->setObjectName(QStringLiteral("cd22Slider"));
        cd22Slider->setMinimum(-100);
        cd22Slider->setMaximum(100);
        cd22Slider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(cd22Slider, 7, 0, 1, 3);

        label_5 = new QLabel(dockWidgetContents);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 8, 0, 1, 2);

        plateScaleLineEdit = new QLineEdit(dockWidgetContents);
        plateScaleLineEdit->setObjectName(QStringLiteral("plateScaleLineEdit"));

        gridLayout->addWidget(plateScaleLineEdit, 8, 2, 1, 1);

        plateScaleSlider = new QSlider(dockWidgetContents);
        plateScaleSlider->setObjectName(QStringLiteral("plateScaleSlider"));
        plateScaleSlider->setMinimum(-100);
        plateScaleSlider->setMaximum(100);
        plateScaleSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(plateScaleSlider, 9, 0, 1, 3);

        label_6 = new QLabel(dockWidgetContents);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 10, 0, 1, 2);

        label = new QLabel(dockWidgetContents);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 13, 0, 1, 1);

        IvWCSDockWidget->setWidget(dockWidgetContents);

        retranslateUi(IvWCSDockWidget);

        QMetaObject::connectSlotsByName(IvWCSDockWidget);
    } // setupUi

    void retranslateUi(QDockWidget *IvWCSDockWidget)
    {
        IvWCSDockWidget->setWindowTitle(QApplication::translate("IvWCSDockWidget", "DockWidget", Q_NULLPTR));
        label_2->setText(QApplication::translate("IvWCSDockWidget", "CD1_2", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        posAngleLineEdit->setStatusTip(QApplication::translate("IvWCSDockWidget", "Position angle, counting North over East", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        resetPushButton->setText(QApplication::translate("IvWCSDockWidget", "Restore original WCS", Q_NULLPTR));
        label_3->setText(QApplication::translate("IvWCSDockWidget", "CD2_1", Q_NULLPTR));
        label_4->setText(QApplication::translate("IvWCSDockWidget", "CD2_2", Q_NULLPTR));
        label_5->setText(QApplication::translate("IvWCSDockWidget", "Plate scale", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        plateScaleLineEdit->setStatusTip(QApplication::translate("IvWCSDockWidget", "The plate scale in arcsec / pixel", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_6->setText(QApplication::translate("IvWCSDockWidget", "Pos. angle", Q_NULLPTR));
        label->setText(QApplication::translate("IvWCSDockWidget", "CD1_1", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class IvWCSDockWidget: public Ui_IvWCSDockWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IVWCSDOCKWIDGET_H
