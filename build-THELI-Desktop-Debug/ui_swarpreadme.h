/********************************************************************************
** Form generated from reading UI file 'swarpreadme.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SWARPREADME_H
#define UI_SWARPREADME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_SwarpReadme
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *maxOpenFileLineEdit;
    QLabel *label_3;
    QLineEdit *openFileLineEdit;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QTextEdit *textEdit;

    void setupUi(QDialog *SwarpReadme)
    {
        if (SwarpReadme->objectName().isEmpty())
            SwarpReadme->setObjectName(QStringLiteral("SwarpReadme"));
        SwarpReadme->resize(442, 550);
        gridLayout = new QGridLayout(SwarpReadme);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(SwarpReadme);
        label->setObjectName(QStringLiteral("label"));
        QPalette palette;
        QBrush brush(QColor(221, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush1(QColor(255, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        QBrush brush2(QColor(168, 169, 169, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        QBrush brush3(QColor(241, 241, 241, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        label->setPalette(palette);
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setAutoFillBackground(true);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(SwarpReadme);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        maxOpenFileLineEdit = new QLineEdit(SwarpReadme);
        maxOpenFileLineEdit->setObjectName(QStringLiteral("maxOpenFileLineEdit"));
        maxOpenFileLineEdit->setReadOnly(true);

        gridLayout->addWidget(maxOpenFileLineEdit, 1, 1, 1, 2);

        label_3 = new QLabel(SwarpReadme);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        openFileLineEdit = new QLineEdit(SwarpReadme);
        openFileLineEdit->setObjectName(QStringLiteral("openFileLineEdit"));
        openFileLineEdit->setReadOnly(true);

        gridLayout->addWidget(openFileLineEdit, 2, 1, 1, 2);

        verticalSpacer = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer, 3, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(335, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 5, 0, 1, 2);

        pushButton = new QPushButton(SwarpReadme);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 5, 2, 1, 1);

        textEdit = new QTextEdit(SwarpReadme);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
        textEdit->setSizePolicy(sizePolicy);
        textEdit->setUndoRedoEnabled(false);
        textEdit->setReadOnly(true);

        gridLayout->addWidget(textEdit, 4, 0, 1, 3);


        retranslateUi(SwarpReadme);
        QObject::connect(pushButton, SIGNAL(clicked()), SwarpReadme, SLOT(close()));

        QMetaObject::connectSlotsByName(SwarpReadme);
    } // setupUi

    void retranslateUi(QDialog *SwarpReadme)
    {
        SwarpReadme->setWindowTitle(QApplication::translate("SwarpReadme", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("SwarpReadme", "<html><head/><body><p><span style=\" color:#dd0000;\">Critical: Incompatible system configuration</span></p></body></html>", Q_NULLPTR));
        label_2->setText(QApplication::translate("SwarpReadme", "Max allowed number of open files on your system:", Q_NULLPTR));
        label_3->setText(QApplication::translate("SwarpReadme", "Number of files that will be opened by Swarp:", Q_NULLPTR));
        pushButton->setText(QApplication::translate("SwarpReadme", "OK", Q_NULLPTR));
        textEdit->setHtml(QApplication::translate("SwarpReadme", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Noto Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">The following steps are needed to permanently change the upper limit of simultaneously opened files on a <span style=\" font-weight:600;\">Debian/Ubuntu</span> system. Other Linuxes are hopefully similar. On Mac you have to figure it out yourself.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">In <span style=\" font-weight"
                        ":600;\">/etc/security/limits.conf</span>, add/set the following two lines to increase the number of open files to 100000:</p>\n"
"<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Hack';\">* soft nofile 100000</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Hack';\">* hard nofile 100000</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">In <span style=\" font-weight:600;\">/etc/pam.d/su</span> and <span style=\" font-weight:600;\">/etc/pam.d/common-session</span>, add (or uncomment) the following line:</p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Hack';\">session required pam_limits.so</span></p>\n"
""
                        "<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Afterwards, logout and login again, and verify in a console that the <span style=\" font-style:italic;\">open files</span> parameter is set to 100000:</p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">$ ulimit -a</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SwarpReadme: public Ui_SwarpReadme {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SWARPREADME_H
