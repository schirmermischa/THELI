/********************************************************************************
** Form generated from reading UI file 'scampreadme.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCAMPREADME_H
#define UI_SCAMPREADME_H

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

class Ui_ScampReadme
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QTextEdit *textEdit;

    void setupUi(QDialog *ScampReadme)
    {
        if (ScampReadme->objectName().isEmpty())
            ScampReadme->setObjectName(QStringLiteral("ScampReadme"));
        ScampReadme->resize(794, 895);
        gridLayout = new QGridLayout(ScampReadme);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalSpacer = new QSpacerItem(626, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        pushButton = new QPushButton(ScampReadme);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);

        textEdit = new QTextEdit(ScampReadme);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setReadOnly(true);

        gridLayout->addWidget(textEdit, 0, 0, 1, 2);


        retranslateUi(ScampReadme);
        QObject::connect(pushButton, SIGNAL(clicked()), ScampReadme, SLOT(close()));

        QMetaObject::connectSlotsByName(ScampReadme);
    } // setupUi

    void retranslateUi(QDialog *ScampReadme)
    {
        ScampReadme->setWindowTitle(QApplication::translate("ScampReadme", "Explanation of Scamp MOSAIC_TYPEs", Q_NULLPTR));
        pushButton->setText(QApplication::translate("ScampReadme", "Close", Q_NULLPTR));
        textEdit->setHtml(QApplication::translate("ScampReadme", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Noto Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">From the SCAMP user manual: </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">SCAMP can manipulate mosaics in a number of ways to perform the matching of sources on the sky, and the astrometric calibration itself. </span><span style=\" font-size:12pt; font-weight:600;\">For single-chip cameras only the UNCHANGED mode makes sense!</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0p"
                        "x; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600; color:#00007f;\">UNCHANGED</span><span style=\" font-size:12pt;\">:<br />The relative positioning of detectors on the focal plane, as recorded in the WCS keywords of the FITS headers, is assumed to be correct and constant from exposure to exposure. Matching with the reference catalogue will be done for all the detectors at once.</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600; color:#00007f;\">SHARE PROJAXIS</span><span style=\" font-size:12pt;\">: <br />The relative positioning of detectors is assumed to be constant and correct, but the different extensions within the same catalogue file do not share the same projection axis (the CRVAL FITS WCS keywords are different): although this does not prevent SCAMP to derive an accurate solution, this is generally not an efficie"
                        "nt astrometric description of the focal plane. This option brings all extensions to the same centered projection axis while compensating with other WCS parameters.</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600; color:#00007f;\">SAME CRVAL</span><span style=\" font-size:12pt;\">: <br />Like SHARE PROJAXIS above, brings all extensions to the same centered projection axis (CRVAL parameters), but does not compensate by changing other WCS parameters. This option is useful when the CRPIX and CD WCS parameters are overridden by some focal plane model stored in a global .ahead file.</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600; color:#00007f;\">FIX FOCALPLANE</span><span style=\" font-size:12pt;\">: <br />Applies first a SHARE PROJAXIS correcti"
                        "on to the headers and then attempts to derive a common, median relative positioning of detectors within the focal plane. This mode is useful to fix the positions of detectors when these have been derived independently at each exposure in an earlier not-so-robust calibration. A minimum of 5 exposures per astrometric instrument is recommended.</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600; color:#00007f;\">LOOSE</span><span style=\" font-size:12pt;\">: <br />Makes all detector positions to be considered as independent between exposures. Contrary to other modes, matching with the reference catalogue will be conducted separately for each extension. The LOOSE mode is generally used for totally uncalibrated mosaics in a first SCAMP pass before doing a FIX FOCALPLANE.</span></p></body></html>", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ScampReadme: public Ui_ScampReadme {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCAMPREADME_H
