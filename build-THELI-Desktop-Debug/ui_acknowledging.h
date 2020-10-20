/********************************************************************************
** Form generated from reading UI file 'acknowledging.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACKNOWLEDGING_H
#define UI_ACKNOWLEDGING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_Acknowledging
{
public:
    QGridLayout *gridLayout;
    QTextEdit *textEdit;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QDialog *Acknowledging)
    {
        if (Acknowledging->objectName().isEmpty())
            Acknowledging->setObjectName(QStringLiteral("Acknowledging"));
        Acknowledging->resize(598, 683);
        gridLayout = new QGridLayout(Acknowledging);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        textEdit = new QTextEdit(Acknowledging);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        gridLayout->addWidget(textEdit, 0, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(491, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        pushButton = new QPushButton(Acknowledging);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);


        retranslateUi(Acknowledging);
        QObject::connect(pushButton, SIGNAL(clicked()), Acknowledging, SLOT(close()));

        QMetaObject::connectSlotsByName(Acknowledging);
    } // setupUi

    void retranslateUi(QDialog *Acknowledging)
    {
        Acknowledging->setWindowTitle(QApplication::translate("Acknowledging", "Acknowledging THELI", Q_NULLPTR));
        textEdit->setHtml(QApplication::translate("Acknowledging", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Noto Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600;\">Acknowledgements</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-weight:600;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">THELI v3 is written in C++, making extensive use of the Qt library, available from <a href=\"https://www.qt.io/\"><span style=\" text-decoration: underline; color:#2980b"
                        "9;\">https://www.qt.io/</span></a> .</p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/icons/qt5_logo.png\" /></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600;\">Acknowledging THELI in scientific publications</span></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt; font-weight:600;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">In scientific publications, you must "
                        "mention the exact version number of THELI used to process your data, together with a reference to this paper: </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"https://iopscience.iop.org/article/10.1088/0067-0049/209/2/21\"><span style=\" text-decoration: underline; color:#2980b9;\">Schirmer, M. 2013, ApJS, 209, 21</span></a>   <a href=\"https://ui.adsabs.harvard.edu/abs/2013ApJS..209...21S/abstract\"><span style=\" text-decoration: underline; color:#2980b9;\">(ADS)</span></a></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; text-decoration: underline; color:#2980b9;\"><br /></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600;\">Acknowledging 3rd party tools used by THELI</span></p"
                        ">\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:12pt; font-weight:600;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">THELI v3 makes use of third-party tools and data products, which must be acknowledged accordingly:</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"https://ui.adsabs.harvard.edu/abs/2002ASPC..281..228B/abstract\"><span style=\" text-decoration: underline; color:#2980b9;\">Bertin et al. 2002, ASPC, 281, 228</span></a>, (Swarp)</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0"
                        "; text-indent:0px;\"><a href=\"https://ui.adsabs.harvard.edu/abs/2006ASPC..351..112B/abstract\"><span style=\" text-decoration: underline; color:#2980b9;\">Bertin 2006, ASPC, 351, 112</span></a> (Scamp)</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"https://ui.adsabs.harvard.edu/abs/1996A%26AS..117..393B/abstract\"><span style=\" text-decoration: underline; color:#2980b9;\">Bertin &amp; Arnouts 1996, A&amp;AS, 117, 393</span></a> (Source Extractor; optional)</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">The use of Source Extractor is optional; by default, THELI uses its own detection method. You must also include astrometric and photometric reference catalogs and their online retr"
                        "ieval tool.  If not discussed in the main text, the minimum text snippet for your acknowledgements is this (adjust as needed):</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-style:italic;\">&quot;THELI made use of the following tools and data products: Source Extractor (Bertin &amp; Arnouts, 1996), Scamp (Bertin 2006), Swarp (Bertin et al. 2002), the XXX catalog (&lt;author, year&gt;) and the VizieR catalogue access tool, CDS, Strasbourg, France.\342\200\235</span></p></body></html>", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Acknowledging", "OK", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Acknowledging: public Ui_Acknowledging {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACKNOWLEDGING_H
