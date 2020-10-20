/********************************************************************************
** Form generated from reading UI file 'imstatsreadme.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMSTATSREADME_H
#define UI_IMSTATSREADME_H

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

class Ui_ImstatsReadme
{
public:
    QGridLayout *gridLayout;
    QTextEdit *textEdit;
    QSpacerItem *horizontalSpacer;
    QPushButton *closePushButton;

    void setupUi(QDialog *ImstatsReadme)
    {
        if (ImstatsReadme->objectName().isEmpty())
            ImstatsReadme->setObjectName(QStringLiteral("ImstatsReadme"));
        ImstatsReadme->resize(938, 797);
        QPalette palette;
        QBrush brush(QColor(252, 252, 252, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(219, 219, 220, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        ImstatsReadme->setPalette(palette);
        ImstatsReadme->setAutoFillBackground(true);
        gridLayout = new QGridLayout(ImstatsReadme);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        textEdit = new QTextEdit(ImstatsReadme);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setReadOnly(true);

        gridLayout->addWidget(textEdit, 0, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(832, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        closePushButton = new QPushButton(ImstatsReadme);
        closePushButton->setObjectName(QStringLiteral("closePushButton"));

        gridLayout->addWidget(closePushButton, 1, 1, 1, 1);


        retranslateUi(ImstatsReadme);
        QObject::connect(closePushButton, SIGNAL(clicked()), ImstatsReadme, SLOT(close()));

        QMetaObject::connectSlotsByName(ImstatsReadme);
    } // setupUi

    void retranslateUi(QDialog *ImstatsReadme)
    {
        ImstatsReadme->setWindowTitle(QApplication::translate("ImstatsReadme", "Mouse and keyboard interactions in the image statistics dialog", Q_NULLPTR));
        textEdit->setHtml(QApplication::translate("ImstatsReadme", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Noto Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">The statistics module should be invoked after source catalogs and astrometry have been created. It displays the following parameters: </span><span style=\" font-size:12pt; color:#ff007f;\">Sky background</span><span style=\" font-size:12pt;\">, </span><span style=\" font-size:12pt; color:#ff007f;\">airmass</span><span style=\" font-size:12pt;\">, </span><span style=\" font-size:12pt; color:#ff007f;\">seeing</span><span style=\" font-size:12pt;\">, </span><span style=\" font-size:12pt; color:#ff007f;\">relative transparency</sp"
                        "an><span style=\" font-size:12pt;\"> (if astrometry was run), </span><span style=\" font-size:12pt; color:#ff007f;\">PSF ellipticity</span><span style=\" font-size:12pt;\">.</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600; color:#00007f;\">Selecting images using name filters</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">Use a filter such as </span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">*.fits</span><span style=\" font-size:12pt;\"> to obtain statistics for all images. Narrow down the selection as you would do on the command line, e.g. </span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">*_3AB.fits</span><span style=\" font-size:12pt;\"> would select the third chip of a multi-chip camera, only.</span><"
                        "/p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600; color:#00007f;\">Selecting images using sky coordinates</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">If both RA and DEC coordinates are provided, then only images containing these coordinates will be shown (if they also fulfil the filter criterion). Useful if you want to check the quality at the location of a particular source in multi-chip data (source appearing in different detectors with varying image quality).</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600; color:#00007f;\">Zooming and panning</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px"
                        "; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">Left-click-drag in a plot or on the axes to pan around. Scroll in a plot or on the axes to zoom.  </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600; color:#00007f;\">Browsing images in iView</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">Select an image by clicking on a data point. The image will be displayed in </span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">iView</span><span style=\" font-size:12pt;\"> if the </span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">Show image</span><span style=\" font-size:12pt;\"> button is selected</span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">. </span>"
                        "<span style=\" font-size:12pt;\">Use the </span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">&lt;left&gt; </span><span style=\" font-size:12pt;\">or </span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">&lt;right&gt; arrow keys</span><span style=\" font-size:12pt;\"> to step through the data. You can also use the yellow selector buttons in</span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\"> iView</span><span style=\" font-size:12pt;\">. </span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">iView</span><span style=\" font-size:12pt;\"> will display the same selection as the statistics module (unless an RA/DEC filter is used).</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600; color:#00007f;\">Interactively rejecting and restoring images</span></p>\n"
"<p style=\" margin-top:12px; margin-b"
                        "ottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">Left-click </span><span style=\" font-size:12pt;\">a data point to select / deselect a single image. Consecutively </span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">right-click</span><span style=\" font-size:12pt;\"> to select / deselect multiple images. If you </span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">right-clicked </span><span style=\" font-size:12pt;\">a data point, then subsequent </span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">&lt;left&gt; </span><span style=\" font-size:12pt;\">and</span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\"> &lt;right&gt; </span><span style=\" font-size:12pt;\">key strokes can be used to quickly include more data points in the vicinity. Following a left-click, the same keys will select single images only.<br />Press <"
                        "/span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">&lt;Delete&gt;</span><span style=\" font-size:12pt;\"> to </span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">park</span><span style=\" font-size:12pt;\"> the selected images (single chips) in a </span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">badStats/ </span><span style=\" font-size:12pt;\">subdirectory. Press the </span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">&lt;A&gt; key</span><span style=\" font-size:12pt;\"> (not &lt;ALT&gt;) to park </span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">all</span><span style=\" font-size:12pt;\"> selected images (all chips) from the associated multi-chip exposure. The graphics will update immediately.<br />Clicking on </span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">Restore data</span><span style=\" font-size:12pt;\"> will move the images back to their original place. Y"
                        "ou might have to replot the graphics to reflect the change after restoring the data.</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600; color:#00007f;\">Numerically rejecting images</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">In addition to mouse-clicks, you can provide lower and upper limits to all parameters (y-axis) and also to the number of images (x-axis). Make the selective active by hitting </span><span style=\" font-size:12pt; font-weight:600; font-style:italic;\">&lt;return&gt;</span><span style=\" font-size:12pt;\">.</span></p></body></html>", Q_NULLPTR));
        closePushButton->setText(QApplication::translate("ImstatsReadme", "Close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ImstatsReadme: public Ui_ImstatsReadme {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMSTATSREADME_H
