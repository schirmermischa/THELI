/********************************************************************************
** Form generated from reading UI file 'multidirreadme.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MULTIDIRREADME_H
#define UI_MULTIDIRREADME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_MultidirReadme
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QTextEdit *textEdit;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *closePushButton;

    void setupUi(QDialog *MultidirReadme)
    {
        if (MultidirReadme->objectName().isEmpty())
            MultidirReadme->setObjectName(QStringLiteral("MultidirReadme"));
        MultidirReadme->resize(734, 661);
        gridLayout = new QGridLayout(MultidirReadme);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(MultidirReadme);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        textEdit = new QTextEdit(MultidirReadme);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        QFont font1;
        font1.setPointSize(10);
        textEdit->setFont(font1);

        gridLayout->addWidget(textEdit, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        closePushButton = new QPushButton(MultidirReadme);
        closePushButton->setObjectName(QStringLiteral("closePushButton"));

        horizontalLayout->addWidget(closePushButton);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);


        retranslateUi(MultidirReadme);
        QObject::connect(closePushButton, SIGNAL(clicked()), MultidirReadme, SLOT(close()));

        QMetaObject::connectSlotsByName(MultidirReadme);
    } // setupUi

    void retranslateUi(QDialog *MultidirReadme)
    {
        MultidirReadme->setWindowTitle(QApplication::translate("MultidirReadme", "The THELI data directory tree", Q_NULLPTR));
        label->setText(QApplication::translate("MultidirReadme", "Multiple science and calibration directories", Q_NULLPTR));
        textEdit->setHtml(QApplication::translate("MultidirReadme", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Noto Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">To facilitate multi-color processing, more than one directory can be specified for the various data types. Comma- or blank-separated lists are allowed. The entries must be relative to the absolute <span style=\" font-style:italic;\">MAIN </span>directory path. The following rules apply:</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-ind"
                        "ent:0px;\"><span style=\" font-weight:600; text-decoration: underline; color:#00007f;\">(1) HDU reformatting, processing of BIASes, DARKs and FLAT OFF/DARKs</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Any number of entries is allowed.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; text-decoration: underline; color:#00007f;\">(2) Processing of FLATs</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-weight:600; text-decoration: underline; color:#5500ff;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-"
                        "right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#00007f;\">(2.1) </span><span style=\" color:#ff007f;\">One CALIB dir</span> (can be BIAS or FLAT OFF/DARK). In this case, the same CALIB dir will be applied to all FLATs. Example:</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000; background-color:#ccffee;\">CALIB = BIAS</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000; background-color:#ccffee;\">FLAT = FLAT_B, FLAT_V, FLAT_R</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#000000;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-"
                        "weight:600; color:#00007f;\">(2.2)</span><span style=\" color:#000000;\"> </span><span style=\" color:#ff007f;\">Equal number of CALIB dirs</span><span style=\" color:#000000;\">. The first FLAT entry will be processed by the first CALIB entry, and so on. Example:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000; background-color:#ccffee;\">CALIB = FLATOFF_J, FLATOFF_H, FLATOFF_K</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000; background-color:#ccffee;\">FLAT = FLAT_J, FLAT_H, FLAT_K</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#000000;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-inde"
                        "nt:0px;\"><span style=\" font-weight:600; text-decoration: underline; color:#00007f;\">(3) Processing of SCIENCEs</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#000000;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#00007f;\">(3.1)</span><span style=\" color:#000000;\"> </span><span style=\" color:#ff007f;\">One FLAT and one BIAS</span><span style=\" color:#000000;\"> (assuming SCIENCEs and FLAT were taken in the same filter). Example:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000; background-color:#ccffee;\">BIAS = BIAS</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0"
                        "px;\"><span style=\" color:#000000; background-color:#ccffee;\">FLAT = FLAT</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000; background-color:#ccffee;\">SCIENCE = TARGET_1, TARGET_2, TARGET_3</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#000000;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#00007f;\">(3.2)</span><span style=\" color:#00007f;\"> </span><span style=\" color:#ff007f;\">Equal number of FLATs and SCIENCE, and one BIAS</span><span style=\" color:#000000;\">, e.g. when the same target was observed in three filters:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-inde"
                        "nt:0px;\"><span style=\" color:#000000; background-color:#ccffee;\">BIAS = BIAS</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000; background-color:#ccffee;\">FLAT = FLAT_g, FLAT_r, FLAT_i</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000; background-color:#ccffee;\">SCIENCE = TARGET_g, TARGET_r, TARGET_i</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#000000;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#00007f;\">(3.3)</span><span style=\" color:#00007f;\"> </span><span style=\" color:#ff007f;\">Equal number of CALIB dirs</span><span styl"
                        "e=\" color:#000000;\">, e.g. when a target was observed multiple times at different dates. First science entry is corrected by first calib entries, etc:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000; background-color:#ccffee;\">DARK = DARK_2016, DARK_2017, DARK_2018</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000; background-color:#ccffee;\">FLAT = FLAT_2016, FLAT_2017, FLAT_2018</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000; background-color:#ccffee;\">SCIENCE = TARGET_2016, TARGET_2017, TARGET_2018</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color"
                        ":#000000;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#00007f;\">(3.4)</span><span style=\" color:#00007f;\"> </span><span style=\" color:#ff007f;\">Equal number but different types of CALIB dirs</span><span style=\" color:#000000;\">. For example, a target was observed in J- and K-band. For J-band, you have a FLAT OFF (e.g. a dome flat with lamp off), but in K-band the dark flat is not possible because you are dominated by the dome's thermal emission; you want to subtract a real DARK. In this case, you can pretend the true DARK to be a FLAT OFF, like this:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000; background-color:#ccffee;\">FLATOFF = FLATOFF_J, DARK_K</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-"
                        "indent:0; text-indent:0px;\"><span style=\" color:#000000; background-color:#ccffee;\">FLAT = FLAT_J, FLAT_K</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000; background-color:#ccffee;\">SCIENCE = TARGET_J, TARGET_K</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#000000;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000;\">If neither of these combinations apply, the SCIENCEs and CALIBs must be processed one by one.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#000000;\"><br /></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bott"
                        "om:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#000000;\"><br /></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#000000;\"><br /></p></body></html>", Q_NULLPTR));
        closePushButton->setText(QApplication::translate("MultidirReadme", "Close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MultidirReadme: public Ui_MultidirReadme {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MULTIDIRREADME_H
