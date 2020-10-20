/********************************************************************************
** Form generated from reading UI file 'memoryviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEMORYVIEWER_H
#define UI_MEMORYVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MemoryViewer
{
public:
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout_4;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_2;
    QTableView *memoryTableView;
    QToolButton *downloadToolButton;
    QToolButton *uploadToolButton;
    QGridLayout *gridLayout_3;
    QCheckBox *procstatusSkysubCheckbox;
    QCheckBox *procstatusCollapseCheckbox;
    QCheckBox *procstatusStarflatCheckbox;
    QSpacerItem *horizontalSpacer_3;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QComboBox *restoreComboBox;
    QComboBox *datadirComboBox;
    QPushButton *restorePushButton;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayout;
    QCheckBox *procstatusProcessscienceCheckbox;
    QCheckBox *procstatusChopnodCheckbox;
    QCheckBox *procstatusBackgroundCheckbox;
    QCheckBox *procstatusHDUreformatCheckbox;

    void setupUi(QDockWidget *MemoryViewer)
    {
        if (MemoryViewer->objectName().isEmpty())
            MemoryViewer->setObjectName(QStringLiteral("MemoryViewer"));
        MemoryViewer->resize(709, 574);
        MemoryViewer->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        MemoryViewer->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        gridLayout_4 = new QGridLayout(dockWidgetContents);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        verticalSpacer_2 = new QSpacerItem(101, 13, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_4->addItem(verticalSpacer_2, 2, 3, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(127, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_2, 0, 4, 1, 2);

        memoryTableView = new QTableView(dockWidgetContents);
        memoryTableView->setObjectName(QStringLiteral("memoryTableView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(memoryTableView->sizePolicy().hasHeightForWidth());
        memoryTableView->setSizePolicy(sizePolicy);

        gridLayout_4->addWidget(memoryTableView, 3, 0, 1, 6);

        downloadToolButton = new QToolButton(dockWidgetContents);
        downloadToolButton->setObjectName(QStringLiteral("downloadToolButton"));
        downloadToolButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon;
        icon.addFile(QStringLiteral("../icons/download.png"), QSize(), QIcon::Normal, QIcon::Off);
        downloadToolButton->setIcon(icon);
        downloadToolButton->setIconSize(QSize(32, 32));
        downloadToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_4->addWidget(downloadToolButton, 1, 4, 1, 1);

        uploadToolButton = new QToolButton(dockWidgetContents);
        uploadToolButton->setObjectName(QStringLiteral("uploadToolButton"));
        uploadToolButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon1;
        icon1.addFile(QStringLiteral("../icons/upload.png"), QSize(), QIcon::Normal, QIcon::Off);
        uploadToolButton->setIcon(icon1);
        uploadToolButton->setIconSize(QSize(32, 32));
        uploadToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_4->addWidget(uploadToolButton, 1, 5, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setVerticalSpacing(0);
        procstatusSkysubCheckbox = new QCheckBox(dockWidgetContents);
        procstatusSkysubCheckbox->setObjectName(QStringLiteral("procstatusSkysubCheckbox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(procstatusSkysubCheckbox->sizePolicy().hasHeightForWidth());
        procstatusSkysubCheckbox->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(procstatusSkysubCheckbox, 2, 0, 1, 1);

        procstatusCollapseCheckbox = new QCheckBox(dockWidgetContents);
        procstatusCollapseCheckbox->setObjectName(QStringLiteral("procstatusCollapseCheckbox"));
        sizePolicy1.setHeightForWidth(procstatusCollapseCheckbox->sizePolicy().hasHeightForWidth());
        procstatusCollapseCheckbox->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(procstatusCollapseCheckbox, 0, 0, 1, 1);

        procstatusStarflatCheckbox = new QCheckBox(dockWidgetContents);
        procstatusStarflatCheckbox->setObjectName(QStringLiteral("procstatusStarflatCheckbox"));
        sizePolicy1.setHeightForWidth(procstatusStarflatCheckbox->sizePolicy().hasHeightForWidth());
        procstatusStarflatCheckbox->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(procstatusStarflatCheckbox, 1, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_3, 0, 3, 2, 1);

        horizontalSpacer_3 = new QSpacerItem(13, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_3, 0, 1, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setVerticalSpacing(0);
        label = new QLabel(dockWidgetContents);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 1, 1, 1);

        restoreComboBox = new QComboBox(dockWidgetContents);
        restoreComboBox->setObjectName(QStringLiteral("restoreComboBox"));
        sizePolicy1.setHeightForWidth(restoreComboBox->sizePolicy().hasHeightForWidth());
        restoreComboBox->setSizePolicy(sizePolicy1);
        restoreComboBox->setMinimumSize(QSize(100, 0));
        restoreComboBox->setFocusPolicy(Qt::NoFocus);
        restoreComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_2->addWidget(restoreComboBox, 0, 2, 1, 1);

        datadirComboBox = new QComboBox(dockWidgetContents);
        datadirComboBox->setObjectName(QStringLiteral("datadirComboBox"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(datadirComboBox->sizePolicy().hasHeightForWidth());
        datadirComboBox->setSizePolicy(sizePolicy2);
        datadirComboBox->setMinimumSize(QSize(160, 0));
        datadirComboBox->setFocusPolicy(Qt::NoFocus);
        datadirComboBox->setMaxVisibleItems(25);
        datadirComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_2->addWidget(datadirComboBox, 1, 0, 1, 1);

        restorePushButton = new QPushButton(dockWidgetContents);
        restorePushButton->setObjectName(QStringLiteral("restorePushButton"));
        restorePushButton->setFocusPolicy(Qt::NoFocus);

        gridLayout_2->addWidget(restorePushButton, 1, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer, 2, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_2, 0, 0, 2, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setVerticalSpacing(0);
        procstatusProcessscienceCheckbox = new QCheckBox(dockWidgetContents);
        procstatusProcessscienceCheckbox->setObjectName(QStringLiteral("procstatusProcessscienceCheckbox"));
        sizePolicy1.setHeightForWidth(procstatusProcessscienceCheckbox->sizePolicy().hasHeightForWidth());
        procstatusProcessscienceCheckbox->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(procstatusProcessscienceCheckbox, 1, 0, 1, 1);

        procstatusChopnodCheckbox = new QCheckBox(dockWidgetContents);
        procstatusChopnodCheckbox->setObjectName(QStringLiteral("procstatusChopnodCheckbox"));
        sizePolicy1.setHeightForWidth(procstatusChopnodCheckbox->sizePolicy().hasHeightForWidth());
        procstatusChopnodCheckbox->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(procstatusChopnodCheckbox, 2, 0, 1, 1);

        procstatusBackgroundCheckbox = new QCheckBox(dockWidgetContents);
        procstatusBackgroundCheckbox->setObjectName(QStringLiteral("procstatusBackgroundCheckbox"));
        sizePolicy1.setHeightForWidth(procstatusBackgroundCheckbox->sizePolicy().hasHeightForWidth());
        procstatusBackgroundCheckbox->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(procstatusBackgroundCheckbox, 3, 0, 1, 1);

        procstatusHDUreformatCheckbox = new QCheckBox(dockWidgetContents);
        procstatusHDUreformatCheckbox->setObjectName(QStringLiteral("procstatusHDUreformatCheckbox"));
        sizePolicy1.setHeightForWidth(procstatusHDUreformatCheckbox->sizePolicy().hasHeightForWidth());
        procstatusHDUreformatCheckbox->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(procstatusHDUreformatCheckbox, 0, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout, 0, 2, 3, 1);

        MemoryViewer->setWidget(dockWidgetContents);

        retranslateUi(MemoryViewer);

        QMetaObject::connectSlotsByName(MemoryViewer);
    } // setupUi

    void retranslateUi(QDockWidget *MemoryViewer)
    {
        MemoryViewer->setWindowTitle(QApplication::translate("MemoryViewer", "Memory viewer", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        downloadToolButton->setStatusTip(QApplication::translate("MemoryViewer", "Writes out Level 0 data in memory (current processing status)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        downloadToolButton->setText(QApplication::translate("MemoryViewer", "Write", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        uploadToolButton->setStatusTip(QApplication::translate("MemoryViewer", "Updates the current state in memory with that on the drive.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        uploadToolButton->setText(QApplication::translate("MemoryViewer", "Sync", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        procstatusSkysubCheckbox->setStatusTip(QApplication::translate("MemoryViewer", "Indicates whether a task was done. Change manually if there is a conflict between FITS files on drive and status in RAM.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        procstatusSkysubCheckbox->setText(QApplication::translate("MemoryViewer", "Skysub (S)", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        procstatusCollapseCheckbox->setStatusTip(QApplication::translate("MemoryViewer", "Indicates whether a task was done. Change manually if there is a conflict between FITS files on drive and status in RAM.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        procstatusCollapseCheckbox->setText(QApplication::translate("MemoryViewer", "Collapse (C)", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        procstatusStarflatCheckbox->setStatusTip(QApplication::translate("MemoryViewer", "Indicates whether a task was done. Change manually if there is a conflict between FITS files on drive and status in RAM.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        procstatusStarflatCheckbox->setText(QApplication::translate("MemoryViewer", "Starflat (D)", Q_NULLPTR));
        label->setText(QApplication::translate("MemoryViewer", "Data shown:", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        restoreComboBox->setStatusTip(QApplication::translate("MemoryViewer", "The processing state you want to restore.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        datadirComboBox->setStatusTip(QApplication::translate("MemoryViewer", "The data type for which memory and processing status are displayed.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        restorePushButton->setStatusTip(QApplication::translate("MemoryViewer", "Restores the displayed data to the selected processing state.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        restorePushButton->setText(QApplication::translate("MemoryViewer", "Restore", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        procstatusProcessscienceCheckbox->setStatusTip(QApplication::translate("MemoryViewer", "Indicates whether a task was done. Change manually if there is a conflict between FITS files on drive and status in RAM.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        procstatusProcessscienceCheckbox->setText(QApplication::translate("MemoryViewer", "Process science (A)", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        procstatusChopnodCheckbox->setStatusTip(QApplication::translate("MemoryViewer", "Indicates whether a task was done. Change manually if there is a conflict between FITS files on drive and status in RAM.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        procstatusChopnodCheckbox->setText(QApplication::translate("MemoryViewer", "Chopnod (M)", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        procstatusBackgroundCheckbox->setStatusTip(QApplication::translate("MemoryViewer", "Indicates whether a task was done. Change manually if there is a conflict between FITS files on drive and status in RAM.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        procstatusBackgroundCheckbox->setText(QApplication::translate("MemoryViewer", "Background (B)", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        procstatusHDUreformatCheckbox->setStatusTip(QApplication::translate("MemoryViewer", "Indicates whether a task was done. Change manually if there is a conflict between FITS files on drive and status in RAM.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        procstatusHDUreformatCheckbox->setText(QApplication::translate("MemoryViewer", "HDUreformat (P)", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MemoryViewer: public Ui_MemoryViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEMORYVIEWER_H
