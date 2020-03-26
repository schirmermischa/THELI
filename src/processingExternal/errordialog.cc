/*
Copyright (C) 2019 Mischa Schirmer

This file is part of THELI.

THELI is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program in the LICENSE file.
If not, see https://www.gnu.org/licenses/ .
*/

#include "errordialog.h"
#include "ui_errordialog.h"
#include "../functions.h"
#include "../preferences.h"
#include <QSysInfo>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>

ErrorDialog::ErrorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorDialog)
{
    ui->setupUi(this);
    //     connect(ui->errorShowPushButton, &QPushButton::clicked, this, &ErrorDialog::openLogfile);
    readSettings();
}

ErrorDialog::~ErrorDialog()
{
    delete ui;
}

void ErrorDialog::update(){
    ui->errorCodeLabel->setText(code);
    ui->errorFoundLabel->setText("An error was found in this logfile (line "+line+"):");
    ui->errorExplanationTextEdit->setText(explanation);
    ui->errorSuggestionsTextEdit->setText(suggestion);
    ui->errorLogfileLineEdit->setText(logname);
}

void ErrorDialog::getErrorLine(QString code)
{
    QStringList list = code.split(" ");
    if (list.length() >=3 ) {
        line = list.at(2);
        line.remove("):");
    }
    else {
        line = "<Unknown>";
    }
}

int ErrorDialog::readSettings()
{
    QSettings settings("THELI", "PREFERENCES");
    editorPreference = settings.value("prefEditorComboBox").toString();
    return settings.status();
}

void ErrorDialog::on_errorShowPushButton_clicked()
{
    showLogfile(logname, line);
}

void ErrorDialog::translate()
{
    // Typically, the errormessage string looks like this:
    // "Error (line 3743): performing run processing"

    // Split it by the first colon, then simplify the string thereafter
    // Remove everything up to and including the colon:
    int colonindex = code.indexOf(":");
    code = code.remove(0,colonindex+1);
    code = code.simplified();

    if (code == "Illegal instruction") {
        explanation = "This is an error that mostly occurs in 'scamp' during astrometry.";
        suggestion = "The cause for this is not known. "
                     "Try using a denser / less dense reference catalog, adjust the object detection thresholds, and/or the scamp parameters.";
    }
    // Uncomment the following for testing purposes
    /*
    else if (code == "GUIVERSION !") {
        explanation = "Dummy";
        suggestion = "Dummy";
    }
*/
    else if (code == "inaccuracies likely to occur") {
        explanation = "This warning occurs during astrometry or coaddition. It always implies a bad astrometric solution, "
                      "which may or may not be immediately visible in your coadded image.";
        suggestion = "You should rerun the astrometry, trying to achieve a better solution. "
                     "Review the checkplots in the \"plots/\" directory carefully. Were the images correctly "
                     "identified on the sky? (fgroups_1.png)";
    }
    else if (code == "WARNING: Not enough matched detections in ") {
        explanation = "This error occurs during astrometry with scamp. "
                      "It means that for or more detectors no matches were found with the reference catalog, "
                      "which should be immediately visible in plots/fgroups_1.png .\n"
                      "The three most common reasons are\n"
                      "(1) The reference catalog and the source catalog do not share sufficiently many sources\n"
                      "(2) The true astrometric solution is outside the search space defined by the scamp parameters\n"
                      "(3) The scamp parameters insufficiently constrain the solution space (too many false positives).\n";
        suggestion = "(1) Review the checkplots. Are most images off or just a single one (which?)\n"
                     "(2) If in a low density field, decrease the source detection thresholds for the object catalogs "
                     "    and increase the magnitude for the reference catalog to get more sources."
                     "(3) If still unsuccessful, load the reference catalog ($HOME/.theli/scripts/theli_mystd.[skycat | reg] "
                     "    over one of your images, and see if you can match it by eye. How large are the offsets / rotations? "
                     "    Adjust POSITION_MAXERR and POSANGLE_MAXERR accordingly.\n"
                     "(4) Reference catalog and matching parameters look fine, but still no success? "
                     "    Load the source detections (SCIENCE/cat/[ds9cat | skycat]/ over your image. "
                     "    Can you match them with the reference catalog?\n"
                     "(5) Try the astrometry.net implementation in THELI.\n"
                     "(1-4) should solve most problems. Think out of the box, e.g if you observe in K-band in a molecular "
                     "    cloud you'll have plenty of detetcions, but optical reference catalogs won't show anything. "
                     "    Use 2MASS instead. Or, all reference sources are badly saturated in your images and rejected "
                     "    from your source catalogs. Use deeper reference catalogs, or adjust the detection flags. "
                     "(5) There are other cases though which are harder to crack, e.g. wide-angle fish-eye exposures, "
                     "    MCAO, etc, and there is no off-the-shelf answer what to do.";
    }
    else if (code == " 0 astrometric references loaded from theli_mystd.scamp") {
        explanation = "scamp does not find reference sources in the downloaded online catalog.";
        suggestion = "Did you download the reference catalog for the correct sky position? "
                     "It could also be that you have one or more stray images from a different pointing in your data set, "
                     "and that results in the reference catalog being downloaded in an area somewhere 'in between' the two "
                     "pointings, resulting in no actual overlap with either position.";
    }
    else if (code == "WARNING: Significant inaccuracy likely to occur in projection") {
        explanation = "This warning occurs during coaddition. It always implies a bad astrometric solution, "
                      "which may or may not be immediately visible in your coadded image.";
        suggestion = "You must rerun the astrometry and try to achieve a better solution. "
                     "Review the checkplots in the \"plots/\" directory carefully. Were the images correctly "
                     "identified on the sky? (fgroups_1.png)";
    }
    else if (code == "WARNING: Null or negative global weighting factor") {
        explanation = "This occurs during coaddition and means that the (manually provided) "
                      "reference coordinates do not (by a large amount) overlap with the actual pointing of your data.";
        suggestion = "Check the manually provided reference coordinates.";
    }
    else if (code == "fatal: division by zero attempted") {
        explanation = "Happens during astrometry, in case one or more images have no or very few sources. "
                "It could also be that there is an error with one of the download servers for the reference catalog.";
        suggestion = "Lower your detection thresholds. Check your images for bad ones (clouds, focus, etc). "
                "Try a different reference catalog or download server (under Edit->Preferences) if the problem occurs at this step.";
    }
    else if (code == "no match with reference catalog !") {
        explanation = "scamp does not find matching sources in the reference catalog.";
        suggestion = "Did you download the reference catalog for the correct sky position? "
                     "It could also be that you have one or more stray images from a different pointing in your data set, "
                     "and that results in the reference catalog being downloaded in an area somewhere 'in between' the two "
                     "pointings, resulting in no actual overlap with either position.\n"
                     "Other things to check:\n"
                     "(1) The reference catalog and the source catalog do not share sufficiently many sources,\n"
                     "(2) The true astrometric solution is outside the search space defined by the scamp parameters,\n"
                     "(3) The scamp parameters insufficiently constrain the solution space (too many false positives).";
        suggestion = "(1) Review the checkplots. Are most images off or just a single one (which?)\n"
                     "(2) If in a low density field, decrease the source detection thresholds for the object catalogs "
                     "    and increase the magnitude for the reference catalog to get more sources.\n"
                     "(3) If still unsuccessful, load the reference catalog ($HOME/.theli/scripts/theli_mystd.[skycat | reg] "
                     "    over one of your images, and see if you can match it by eye. How large are the offsets / rotations? "
                     "    Adjust POSITION_MAXERR and POSANGLE_MAXERR accordingly.\n"
                     "(4) Reference catalog and matching parameters look fine, but still no success? "
                     "    Load the source detections (SCIENCE/cat/[ds9cat | skycat]/ over your image. "
                     "    Can you match them with the reference catalog?"
                     "(5) Try the astrometry.net implementation in THELI.\n"
                     "(1-4) should solve most problems. Think out of the box, e.g if you observe in K-band in a molecular "
                     "    cloud you'll have plenty of detetcions, but optical reference catalogs won't show anything. "
                     "    Use 2MASS instead. Or, all reference sources are badly saturated in your images and rejected "
                     "    from your source catalogs. Use deeper reference catalogs, or adjust the detection flags.\n"
                     "(5) There are other cases though which are harder to crack, e.g. wide-angle fish-eye exposures, "
                     "    MCAO, etc, and there is no off-the-shelf answer what to do.";
    }
    else if (code == "Not enough memory") {
        explanation = "This happens either during astrometry or coaddition.";
        suggestion = "If in astrometry, try reducing the number of CPUs by a factor of two. "
                     "Check the memory usage using 'top' on the command line. If doing a coaddition, are you "
                     "creating a huge mosaic? Perhaps you have entered the wrong plate scale?";
    }
    else if (code == "buffer overflow detected") {
        explanation = "This happens either during astrometry or coaddition.";
        suggestion = "If in astrometry, try reducing the number of CPUs by a factor of two. "
                     "Check the memory usage using 'top' on the command line. If doing a coaddition, are you "
                     "creating a huge mosaic? Perhaps you have entered the wrong plate scale?";
    }
    else if (code == "did not solve") {
        explanation = "This error occurs when astrometry.net cannot find a solution.";
        suggestion = "Try running the astrometry with scamp, where you have much more control.";
    }
    else if (code == "(core dumped)") {
        explanation = "This is a very uncommon error implying some memory corruption.";
        suggestion = "If it happens during coaddition, check plate scale "
                     "and manually provided reference coordinates.";
    }
    else if (code == "has flux scale = 0: I will take 1 instead") {
        explanation = "This happens during coaddition and implies a wrong / incomplete astrometric solution.";
        suggestion = "Check the logfile to see which image is causing the problem. Perhaps the image is bad "
                     "and could not be solved completely during astrometry.";
    }
    else if (code == "Could not allocate memory") {
        explanation = "This happens most likely during astrometry or coaddition.";
        suggestion = "If in astrometry, try reducing the number of CPUs by a factor of two.\n"
                     "Check the memory usage using 'top' on the command line. If doing a coaddition, are you "
                     "creating a huge mosaic? Perhaps you have entered the wrong plate scale?";
    }
    else if (code == "ERROR: No photometric reference sources retrieved!") {
        explanation = "This occurs when determining an absolute photometric zeropoint.";
        suggestion = "Is the WCS in your image good? Is the network connection working? "
                     "If you used a catalog that does not have full sky coverage (e.g. SDSS, PANSTARRS), then "
                     "this message indicates that your field is probably not covered by that catalog.";
    }
    else if (code == "ERROR: Could not find file with blank sky positions") {
        explanation = "The positions where the sky estimates are taken was not found.";
        suggestion = "If you requested a polynomial fit, then you must define blank sky positions. "
                     "This message will also be shown if you requested a constant sky subtraction, "
                     "with the empty sky regions set to 'Specific sky area(s)' instead of a whole chip. "
                     "If you want to use specific areas, use the THELI internal FITS viewer to create a file with empty sky positions. "
                     "Load an astrometrically calibrated image, and define areas using middle-click drags on the image. "
                     "The file will be automatically created and updated on the fly in the SCIENCE directory.";
    }
    else if (code == "ERROR: File with blank sky positions") {
        explanation = "The file with empty sky positions does not contain any entries.";
        suggestion = "Use the THELI internal FITS viewer to create a file with empty sky positions. "
                     "Load an astrometrically calibrated image, and define areas using middle-click drags on the image. "
                     "The file will be automatically created and updated on the fly in the SCIENCE directory.";
    }
    else if (code == "ERROR: Less than 3 sky positions were located in the images.") {
        explanation = "The sky polynomial fit is underconstrained. More sky positions are required.";
        suggestion = "Use the THELI internal FITS viewer to create a file with sufficiently many empty sky positions. "
                     "Load an astrometrically calibrated image, and define areas using middle-click drags on the image. "
                     "The file will be automatically created and updated on the fly in the SCIENCE directory.";
    }
    else if (code == "ERROR: Insufficient number of sky positions for a model of degree") {
        explanation = "The sky polynomial fit is underconstrained. More sky positions are required.";
        suggestion = "Use the THELI internal FITS viewer to create a file with sufficiently many empty sky positions. "
                     "Load an astrometrically calibrated image, and define areas using middle-click drags on the image. "
                     "The file will be automatically created and updated on the fly in the SCIENCE directory.";
    }
    else if (code == "Did not create any quads") {
        explanation = "astrometry.net did not create any quads, which are used for pattern matching between source and reference catalog";
        suggestion = "This can happen if the source density is very low. Try using a different reference catalog.";
    }
    else if (code == "solve-field: invalid option --") {
        explanation = "The syntax of astrometry.net has changed.";
        suggestion = "THELI is not using the correct syntax for astrometry.net. Please send the logfile to schirmer@mpia.de .";
    }
}

void ErrorDialog::on_errorLogfileLineEdit_textChanged(const QString &arg1)
{
    paintPathLineEdit(ui->errorLogfileLineEdit, arg1, "file");
}
