/********************************************************************************
** Form generated from reading UI file 'ivcolordockwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IVCOLORDOCKWIDGET_H
#define UI_IVCOLORDOCKWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IvColorDockWidget
{
public:
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer_3;
    QFrame *redFrame;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_2;
    QFrame *blueFrame;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *G2referencesPushButton;
    QPushButton *redResetPushButton;
    QLineEdit *redFactorLineEdit;
    QSlider *redSlider;
    QPushButton *blueResetPushButton;
    QLineEdit *blueFactorLineEdit;
    QSlider *blueSlider;

    void setupUi(QDockWidget *IvColorDockWidget)
    {
        if (IvColorDockWidget->objectName().isEmpty())
            IvColorDockWidget->setObjectName(QStringLiteral("IvColorDockWidget"));
        IvColorDockWidget->resize(180, 307);
        IvColorDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        IvColorDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        gridLayout = new QGridLayout(dockWidgetContents);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalSpacer_3 = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_3, 1, 0, 1, 1);

        redFrame = new QFrame(dockWidgetContents);
        redFrame->setObjectName(QStringLiteral("redFrame"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(redFrame->sizePolicy().hasHeightForWidth());
        redFrame->setSizePolicy(sizePolicy);
        QPalette palette;
        QBrush brush(QColor(252, 252, 252, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(241, 37, 64, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        redFrame->setPalette(palette);
        redFrame->setAutoFillBackground(true);
        redFrame->setFrameShape(QFrame::Box);
        redFrame->setFrameShadow(QFrame::Plain);
        redFrame->setLineWidth(1);
        verticalLayout_2 = new QVBoxLayout(redFrame);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(3, 3, 3, 3);

        gridLayout->addWidget(redFrame, 2, 0, 3, 1);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_2, 5, 1, 1, 1);

        blueFrame = new QFrame(dockWidgetContents);
        blueFrame->setObjectName(QStringLiteral("blueFrame"));
        sizePolicy.setHeightForWidth(blueFrame->sizePolicy().hasHeightForWidth());
        blueFrame->setSizePolicy(sizePolicy);
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush2(QColor(25, 25, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        blueFrame->setPalette(palette1);
        blueFrame->setAutoFillBackground(true);
        blueFrame->setFrameShape(QFrame::Box);
        blueFrame->setFrameShadow(QFrame::Plain);
        blueFrame->setLineWidth(1);
        verticalLayout = new QVBoxLayout(blueFrame);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(3, 3, 3, 3);

        gridLayout->addWidget(blueFrame, 6, 0, 3, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 9, 2, 1, 1);

        G2referencesPushButton = new QPushButton(dockWidgetContents);
        G2referencesPushButton->setObjectName(QStringLiteral("G2referencesPushButton"));
        G2referencesPushButton->setCheckable(true);

        gridLayout->addWidget(G2referencesPushButton, 0, 0, 1, 4);

        redResetPushButton = new QPushButton(dockWidgetContents);
        redResetPushButton->setObjectName(QStringLiteral("redResetPushButton"));

        gridLayout->addWidget(redResetPushButton, 2, 1, 1, 3);

        redFactorLineEdit = new QLineEdit(dockWidgetContents);
        redFactorLineEdit->setObjectName(QStringLiteral("redFactorLineEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(redFactorLineEdit->sizePolicy().hasHeightForWidth());
        redFactorLineEdit->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(redFactorLineEdit, 3, 1, 1, 3);

        redSlider = new QSlider(dockWidgetContents);
        redSlider->setObjectName(QStringLiteral("redSlider"));
        redSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(redSlider, 4, 1, 1, 3);

        blueResetPushButton = new QPushButton(dockWidgetContents);
        blueResetPushButton->setObjectName(QStringLiteral("blueResetPushButton"));

        gridLayout->addWidget(blueResetPushButton, 6, 1, 1, 3);

        blueFactorLineEdit = new QLineEdit(dockWidgetContents);
        blueFactorLineEdit->setObjectName(QStringLiteral("blueFactorLineEdit"));
        sizePolicy1.setHeightForWidth(blueFactorLineEdit->sizePolicy().hasHeightForWidth());
        blueFactorLineEdit->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(blueFactorLineEdit, 7, 1, 1, 3);

        blueSlider = new QSlider(dockWidgetContents);
        blueSlider->setObjectName(QStringLiteral("blueSlider"));
        blueSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(blueSlider, 8, 1, 1, 3);

        IvColorDockWidget->setWidget(dockWidgetContents);

        retranslateUi(IvColorDockWidget);

        QMetaObject::connectSlotsByName(IvColorDockWidget);
    } // setupUi

    void retranslateUi(QDockWidget *IvColorDockWidget)
    {
        IvColorDockWidget->setWindowTitle(QApplication::translate("IvColorDockWidget", "Color balance", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        G2referencesPushButton->setStatusTip(QApplication::translate("IvColorDockWidget", "The reference sources used for color calibration (SDSS: red, PanSTARRS: cyan, APASS: green)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        G2referencesPushButton->setText(QApplication::translate("IvColorDockWidget", "Show G2 References", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        redResetPushButton->setStatusTip(QApplication::translate("IvColorDockWidget", "Resets the red factor to the determined value", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        redResetPushButton->setText(QApplication::translate("IvColorDockWidget", "Reset red factor", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        redFactorLineEdit->setStatusTip(QApplication::translate("IvColorDockWidget", "Red color balance factor", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        blueResetPushButton->setStatusTip(QApplication::translate("IvColorDockWidget", "Resets the blue factor to the determined value", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        blueResetPushButton->setText(QApplication::translate("IvColorDockWidget", "Reset blue factor", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        blueFactorLineEdit->setStatusTip(QApplication::translate("IvColorDockWidget", "Blue color balance factor", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
    } // retranslateUi

};

namespace Ui {
    class IvColorDockWidget: public Ui_IvColorDockWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IVCOLORDOCKWIDGET_H
