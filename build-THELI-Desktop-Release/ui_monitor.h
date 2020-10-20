/********************************************************************************
** Form generated from reading UI file 'monitor.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MONITOR_H
#define UI_MONITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Monitor
{
public:
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout;
    QPlainTextEdit *monitorPlainTextEdit;

    void setupUi(QDockWidget *Monitor)
    {
        if (Monitor->objectName().isEmpty())
            Monitor->setObjectName(QStringLiteral("Monitor"));
        Monitor->resize(659, 335);
        Monitor->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        gridLayout = new QGridLayout(dockWidgetContents);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        monitorPlainTextEdit = new QPlainTextEdit(dockWidgetContents);
        monitorPlainTextEdit->setObjectName(QStringLiteral("monitorPlainTextEdit"));
        monitorPlainTextEdit->setFocusPolicy(Qt::NoFocus);
        monitorPlainTextEdit->setLayoutDirection(Qt::LeftToRight);
        monitorPlainTextEdit->setFrameShape(QFrame::Panel);
        monitorPlainTextEdit->setUndoRedoEnabled(false);
        monitorPlainTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        monitorPlainTextEdit->setReadOnly(true);
        monitorPlainTextEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        gridLayout->addWidget(monitorPlainTextEdit, 0, 0, 1, 1);

        Monitor->setWidget(dockWidgetContents);

        retranslateUi(Monitor);

        QMetaObject::connectSlotsByName(Monitor);
    } // setupUi

    void retranslateUi(QDockWidget *Monitor)
    {
        Monitor->setWindowTitle(QApplication::translate("Monitor", "Monitor", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        monitorPlainTextEdit->setStatusTip(QApplication::translate("Monitor", "Displays output of currently ongoing processing tasks", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
    } // retranslateUi

};

namespace Ui {
    class Monitor: public Ui_Monitor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MONITOR_H
