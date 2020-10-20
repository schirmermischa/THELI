/********************************************************************************
** Form generated from reading UI file 'errordialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ERRORDIALOG_H
#define UI_ERRORDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_ErrorDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *errorFoundLabel;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *errorShowPushButton;
    QLineEdit *errorLogfileLineEdit;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *errorCodeLabel;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_2;
    QTextEdit *errorExplanationTextEdit;
    QSpacerItem *verticalSpacer;
    QLabel *label_3;
    QTextEdit *errorSuggestionsTextEdit;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *errorClosePushButton;

    void setupUi(QDialog *ErrorDialog)
    {
        if (ErrorDialog->objectName().isEmpty())
            ErrorDialog->setObjectName(QStringLiteral("ErrorDialog"));
        ErrorDialog->resize(740, 573);
        gridLayout = new QGridLayout(ErrorDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        errorFoundLabel = new QLabel(ErrorDialog);
        errorFoundLabel->setObjectName(QStringLiteral("errorFoundLabel"));

        gridLayout->addWidget(errorFoundLabel, 0, 0, 1, 3);

        horizontalSpacer_2 = new QSpacerItem(216, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 3, 1, 1);

        errorShowPushButton = new QPushButton(ErrorDialog);
        errorShowPushButton->setObjectName(QStringLiteral("errorShowPushButton"));
        errorShowPushButton->setFocusPolicy(Qt::NoFocus);

        gridLayout->addWidget(errorShowPushButton, 0, 4, 1, 1);

        errorLogfileLineEdit = new QLineEdit(ErrorDialog);
        errorLogfileLineEdit->setObjectName(QStringLiteral("errorLogfileLineEdit"));
        errorLogfileLineEdit->setFocusPolicy(Qt::NoFocus);
        errorLogfileLineEdit->setReadOnly(true);

        gridLayout->addWidget(errorLogfileLineEdit, 1, 0, 1, 5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(ErrorDialog);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        horizontalLayout->addWidget(label);

        errorCodeLabel = new QLabel(ErrorDialog);
        errorCodeLabel->setObjectName(QStringLiteral("errorCodeLabel"));
        QPalette palette;
        QBrush brush(QColor(170, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(119, 120, 120, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        errorCodeLabel->setPalette(palette);
        QFont font1;
        font1.setBold(true);
        font1.setItalic(true);
        font1.setUnderline(false);
        font1.setWeight(75);
        errorCodeLabel->setFont(font1);

        horizontalLayout->addWidget(errorCodeLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 4);

        verticalSpacer_2 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_2, 3, 2, 1, 1);

        label_2 = new QLabel(ErrorDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font);

        gridLayout->addWidget(label_2, 4, 0, 1, 1);

        errorExplanationTextEdit = new QTextEdit(ErrorDialog);
        errorExplanationTextEdit->setObjectName(QStringLiteral("errorExplanationTextEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(errorExplanationTextEdit->sizePolicy().hasHeightForWidth());
        errorExplanationTextEdit->setSizePolicy(sizePolicy);
        errorExplanationTextEdit->setMaximumSize(QSize(16777215, 100));
        errorExplanationTextEdit->setFocusPolicy(Qt::NoFocus);
        errorExplanationTextEdit->setFrameShape(QFrame::Panel);
        errorExplanationTextEdit->setReadOnly(true);

        gridLayout->addWidget(errorExplanationTextEdit, 5, 0, 1, 5);

        verticalSpacer = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer, 6, 2, 1, 1);

        label_3 = new QLabel(ErrorDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font);

        gridLayout->addWidget(label_3, 7, 0, 1, 3);

        errorSuggestionsTextEdit = new QTextEdit(ErrorDialog);
        errorSuggestionsTextEdit->setObjectName(QStringLiteral("errorSuggestionsTextEdit"));
        errorSuggestionsTextEdit->setFocusPolicy(Qt::NoFocus);
        errorSuggestionsTextEdit->setFrameShape(QFrame::Panel);
        errorSuggestionsTextEdit->setLineWidth(1);
        errorSuggestionsTextEdit->setReadOnly(true);

        gridLayout->addWidget(errorSuggestionsTextEdit, 8, 0, 1, 5);

        horizontalSpacer_3 = new QSpacerItem(217, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 9, 1, 1, 1);

        errorClosePushButton = new QPushButton(ErrorDialog);
        errorClosePushButton->setObjectName(QStringLiteral("errorClosePushButton"));

        gridLayout->addWidget(errorClosePushButton, 9, 4, 1, 1);


        retranslateUi(ErrorDialog);
        QObject::connect(errorClosePushButton, SIGNAL(clicked()), ErrorDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(ErrorDialog);
    } // setupUi

    void retranslateUi(QDialog *ErrorDialog)
    {
        ErrorDialog->setWindowTitle(QApplication::translate("ErrorDialog", "THELI Error translation", Q_NULLPTR));
        errorFoundLabel->setText(QApplication::translate("ErrorDialog", "An error was found in this logfile (line XXXX):", Q_NULLPTR));
        errorShowPushButton->setText(QApplication::translate("ErrorDialog", "Show logfile", Q_NULLPTR));
        label->setText(QApplication::translate("ErrorDialog", "Error code:", Q_NULLPTR));
        errorCodeLabel->setText(QApplication::translate("ErrorDialog", "Message", Q_NULLPTR));
        label_2->setText(QApplication::translate("ErrorDialog", "Explanation:", Q_NULLPTR));
        label_3->setText(QApplication::translate("ErrorDialog", "Suggestions how to solve this:", Q_NULLPTR));
        errorClosePushButton->setText(QApplication::translate("ErrorDialog", "Close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ErrorDialog: public Ui_ErrorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ERRORDIALOG_H
