/********************************************************************************
** Form generated from reading UI file 'iview.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IVIEW_H
#define UI_IVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IView
{
public:
    QAction *actionLoadImageFromDrive;
    QAction *actionClose;
    QAction *actionStart;
    QAction *actionPrevious;
    QAction *actionBack;
    QAction *actionForward;
    QAction *actionNext;
    QAction *actionEnd;
    QAction *actionSourceCat;
    QAction *actionRefCat;
    QAction *actionDragMode;
    QAction *actionSkyMode;
    QAction *actionWCSMode;
    QWidget *centralwidget;
    QGridLayout *gridLayout_6;
    QFrame *frame;
    QGridLayout *gridLayout_5;
    QVBoxLayout *graphicsLayout;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *IView)
    {
        if (IView->objectName().isEmpty())
            IView->setObjectName(QStringLiteral("IView"));
        IView->resize(664, 711);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(IView->sizePolicy().hasHeightForWidth());
        IView->setSizePolicy(sizePolicy);
        actionLoadImageFromDrive = new QAction(IView);
        actionLoadImageFromDrive->setObjectName(QStringLiteral("actionLoadImageFromDrive"));
        actionClose = new QAction(IView);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        actionStart = new QAction(IView);
        actionStart->setObjectName(QStringLiteral("actionStart"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/icon_start.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStart->setIcon(icon);
        actionPrevious = new QAction(IView);
        actionPrevious->setObjectName(QStringLiteral("actionPrevious"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/icon_previous.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrevious->setIcon(icon1);
        actionBack = new QAction(IView);
        actionBack->setObjectName(QStringLiteral("actionBack"));
        actionBack->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/icon_back.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBack->setIcon(icon2);
        actionForward = new QAction(IView);
        actionForward->setObjectName(QStringLiteral("actionForward"));
        actionForward->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/icon_forward.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionForward->setIcon(icon3);
        actionNext = new QAction(IView);
        actionNext->setObjectName(QStringLiteral("actionNext"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/icon_next.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNext->setIcon(icon4);
        actionEnd = new QAction(IView);
        actionEnd->setObjectName(QStringLiteral("actionEnd"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icons/icon_end.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEnd->setIcon(icon5);
        actionSourceCat = new QAction(IView);
        actionSourceCat->setObjectName(QStringLiteral("actionSourceCat"));
        actionSourceCat->setCheckable(true);
        actionRefCat = new QAction(IView);
        actionRefCat->setObjectName(QStringLiteral("actionRefCat"));
        actionRefCat->setCheckable(true);
        actionDragMode = new QAction(IView);
        actionDragMode->setObjectName(QStringLiteral("actionDragMode"));
        actionDragMode->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/icons/hand.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDragMode->setIcon(icon6);
        actionSkyMode = new QAction(IView);
        actionSkyMode->setObjectName(QStringLiteral("actionSkyMode"));
        actionSkyMode->setCheckable(true);
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/icons/pipette.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSkyMode->setIcon(icon7);
        actionWCSMode = new QAction(IView);
        actionWCSMode->setObjectName(QStringLiteral("actionWCSMode"));
        actionWCSMode->setCheckable(true);
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/icons/wcs.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionWCSMode->setIcon(icon8);
        centralwidget = new QWidget(IView);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout_6 = new QGridLayout(centralwidget);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QStringLiteral("frame"));
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::Panel);
        frame->setFrameShadow(QFrame::Sunken);
        frame->setLineWidth(2);
        gridLayout_5 = new QGridLayout(frame);
        gridLayout_5->setSpacing(0);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        graphicsLayout = new QVBoxLayout();
        graphicsLayout->setSpacing(0);
        graphicsLayout->setObjectName(QStringLiteral("graphicsLayout"));

        gridLayout_5->addLayout(graphicsLayout, 0, 0, 1, 1);


        gridLayout_6->addWidget(frame, 0, 0, 1, 1);

        IView->setCentralWidget(centralwidget);
        menubar = new QMenuBar(IView);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 664, 25));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        IView->setMenuBar(menubar);
        statusbar = new QStatusBar(IView);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        IView->setStatusBar(statusbar);
        toolBar = new QToolBar(IView);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        IView->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionLoadImageFromDrive);
        menuFile->addAction(actionClose);
        toolBar->addAction(actionDragMode);
        toolBar->addAction(actionSkyMode);
        toolBar->addAction(actionWCSMode);
        toolBar->addSeparator();
        toolBar->addAction(actionSourceCat);
        toolBar->addAction(actionRefCat);
        toolBar->addSeparator();
        toolBar->addAction(actionPrevious);
        toolBar->addAction(actionNext);
        toolBar->addSeparator();
        toolBar->addAction(actionStart);
        toolBar->addAction(actionEnd);
        toolBar->addSeparator();
        toolBar->addAction(actionBack);
        toolBar->addAction(actionForward);

        retranslateUi(IView);

        QMetaObject::connectSlotsByName(IView);
    } // setupUi

    void retranslateUi(QMainWindow *IView)
    {
        IView->setWindowTitle(QApplication::translate("IView", "iView", Q_NULLPTR));
        actionLoadImageFromDrive->setText(QApplication::translate("IView", "&Load image from drive", Q_NULLPTR));
        actionClose->setText(QApplication::translate("IView", "&Close", Q_NULLPTR));
        actionStart->setText(QString());
#ifndef QT_NO_TOOLTIP
        actionStart->setToolTip(QApplication::translate("IView", "First in series", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionPrevious->setText(QString());
#ifndef QT_NO_TOOLTIP
        actionPrevious->setToolTip(QApplication::translate("IView", "Previous in series", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionBack->setText(QString());
#ifndef QT_NO_TOOLTIP
        actionBack->setToolTip(QApplication::translate("IView", "Play back series", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionForward->setText(QString());
#ifndef QT_NO_TOOLTIP
        actionForward->setToolTip(QApplication::translate("IView", "Play forward series", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionNext->setText(QString());
#ifndef QT_NO_TOOLTIP
        actionNext->setToolTip(QApplication::translate("IView", "Next in series", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionEnd->setText(QString());
#ifndef QT_NO_TOOLTIP
        actionEnd->setToolTip(QApplication::translate("IView", "Last in series", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionSourceCat->setText(QApplication::translate("IView", "SourceCat", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionSourceCat->setToolTip(QApplication::translate("IView", "Overlay source catalog", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionRefCat->setText(QApplication::translate("IView", "RefCat", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionRefCat->setToolTip(QApplication::translate("IView", "Overlay reference catalog", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionDragMode->setText(QApplication::translate("IView", "Drag Mode", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionDragMode->setToolTip(QApplication::translate("IView", "Middle mouse button drags image", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionSkyMode->setText(QApplication::translate("IView", "Sky Mode", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionSkyMode->setToolTip(QApplication::translate("IView", "Middle mouse button creates sky measurement areas", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionWCSMode->setText(QApplication::translate("IView", "WCS Mode", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionWCSMode->setToolTip(QApplication::translate("IView", "Middle mouse button changes CRPIX1/2", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        menuFile->setTitle(QApplication::translate("IView", "Fi&le", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("IView", "toolBar", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        toolBar->setToolTip(QApplication::translate("IView", "Middle mouse button defines circular areas for sky level determination", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class IView: public Ui_IView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IVIEW_H
