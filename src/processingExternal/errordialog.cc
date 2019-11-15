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
    else if (code == "not found !") {
        explanation = "A file (image, catalog, header) was not found.";
        suggestion = "This may mean that an unrecognized error occurred previously in the task. "
                     "It could also indicate an issue with your data (e.g. images with no sources detected). "
                     "Check the logfile to see the context.";
    }
    else if (code == ": not found") {
        explanation = "A file (image, catalog, header) was not found.";
        suggestion = "This may mean that an unrecognized error occurred previously in the task. "
                     "It could also indicate an issue with your data (e.g. images with no sources detected). "
                     "Check the logfile to see the context.";
    }
    else if (code == "cholsolve()") {
        explanation = "This is a rare error occuring in 'scamp' during astrometry.";
        suggestion = "The cause for this is not known. "
                     "Try using a denser / less dense reference catalog, "
                     "adjust the object detection thresholds, and/or the scamp parameters.";
    }
    else if (code == "inaccuracies likely to occur") {
        explanation = "This warning occurs during astrometry or coaddition. It always implies a bad astrometric solution, "
                      "which may or may not be immediately visible in your coadded image.";
        suggestion = "You must rerun the astrometry and try to achieve a better solution. "
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
    else if (code == "too long to connect ") {
        explanation = "An online reference catalog takes too long to download.";
        suggestion = "The server might be down, THELI lets you choose from difference ones. "
                     "Also, check your internet connection.";
    }
    else if (code == "Segmentation fault") {
        explanation = "This is uncommon, and probably happens during astrometry or coaddition.";
        suggestion = "If it happens during coaddition, most likely there is a problem with astrometry, "
                     "or you have specified reference coordinates that do not (by a large amount) overlap with your data.";
    }
    else if (code == "segmentation fault") {
        explanation = "This is uncommon, and probably happens during astrometry or coaddition.";
        suggestion = "If it happens during coaddition, most likely there is a problem with astrometry, "
                     "or you have specified reference coordinates that do not (by a large amount) overlap with your data.";
    }
    else if (code == "Permission denied") {
        explanation = "THELI could not access one or more of your data files, or its own source files.";
        suggestion = "Check the context in the logfile to see which file causes the problem. "
                     "This could also be a setup error, e.g. if you installed THELI as root (which you should never do, "
                     "for that reason).";
    }
    else if (code == "command not found") {
        explanation = "THELI did not find a crucial component.";
        suggestion = "This could hint at an unresolved dependency issue during installation, "
                     "and not all sources were compiled correctly. Or some library is missing. Also, make sure that "
                     "THELI/theli/bin/[Linux | Linux_64 | Darwin] is included in your PATH environment variable.";
    }
    else if (code == "Cannot map ") {
        explanation = "This is a very uncommon error, most likely due to a faulty astrometric "
                      "that drives the coaddition routines into their limits.";
        suggestion = "Check the astrometry, and your coaddition parameters. Are you doing anything unusual?";
    }
    else if (code == " 0 astrometric references loaded from theli_mystd.scamp") {
        explanation = "scamp does not find reference sources in the downloaded online catalog.";
        suggestion = "Did you download the reference catalog for the correct sky position? "
                     "It could also be that you have one or more stray images from a different pointing in your data set, "
                     "and that results in the reference catalog being downloaded in an area somewhere 'in between' the two "
                     "pointings, resulting in no actual overlap with either position.";
    }
    else if (code == "fatal: cannot open file") {
        explanation = "One more files THELI needs for processing is not available, or cannot be opened.";
        suggestion = "Check the context of the logfile to see which file is causing problems. If this "
                     "happens during coaddition and the file is actually present, then it could mean that you are processing "
                     "a very large data set (thousands of images) and the number of files opened exceeds the max number of "
                     "open file descriptors on your system. Check 'ulimit -a'.";
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
    else if (code == "list has not enough members") {
        explanation = "This probably happened during catalog creation, cause unknown.";
        suggestion = "Try different detection thresholds. Check your images, perhaps there is "
                     "one that is badly trailed, defocused, or shows no stars because of clouds.";
    }
    else if (code == "Network is unreachable") {
        explanation = "";
        suggestion = "Check your network connection.";
    }
    else if (code == "fatal: division by zero attempted") {
        explanation = "Happens during astrometry, in case one or more images have no or very few sources. "
                "It could also be that there is an error with one of the download servers for the reference catalog.";
        suggestion = "Lower your detection thresholds. Check your images for bad ones (clouds, focus, etc). "
                "Try a different reference catalog or download server (under Edit->Preferences) if the problem occurs at this step.";
    }
    else if (code == "head not present) { Aborting") {
        explanation = "Probably occurs during coaddition, meaning the astrometry was not successful for all images.";
        suggestion = "Check the astrometry, and if all images have .head files under 'headers/'";
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
    else if (code == "integer expression expected") {
        explanation = "This is likely triggered by a syntax error in one of the bash scripts "
                      "but it could also be caused by corrupted data.";
        suggestion = "Send the logfile to Mischa (schirmer@mpia.de) for inspection.";
    }
    else if (code == "syntax error near unexpected token") {
        explanation = "This is likely triggered by a syntax error in one of the bash scripts "
                      "but it could also be caused by corrupted data.";
        suggestion = "Send the logfile to Mischa (schirmer@mpia.de) for inspection.";
    }
    else if (code == "keyword out of range") {
        explanation = "This likely occurs during HDU reformatting at the very beginning.";
        suggestion = "One of the FITS keywords is beyond limits, e.g. extremely high airmasses or exposure times. "
                     "Most of these have been caught by now, but if you are using data from an instrument which has few THELI users "
                     "you might still encounter this.\n"
                     "Alternatively, this could be caused by e.g. SExtractor, SWarp or Scamp if an empty value is assigned"
                     "to a parameter in the command syntax.\n"
                     "In either case this is likely a bug and you should send the logfile to Mischa (schirmer@mpia.de).";
    }
//    else if (code == "keyword unknown") {
//        explanation = "This is a very uncommon error that has not been seen in a long time.";
//        suggestion = "Send the logfile to Mischa (schirmer@mpia.de) for inspection.";
//    }
    else if (code == "too many arguments") {
        explanation = "There is currently no clear or consistent explanation.";
        suggestion = "Send the logfile to Mischa (schirmer@mpia.de) for inspection.";
    }
    else if (code == "cannot execute binary file") {
        explanation = "";
        suggestion = "Check the file permission flags in THELI/theli/bin/[Linux | Linux_64 | Darwin]. "
                     "Did you perhaps copy executables from one machine to another?";
    }
    else if (code == "Cannot decode file") {
        explanation = "This is a very uncommon error that has not been seen in a long time.";
        suggestion = "Send the logfile to Mischa (schirmer@mpia.de) for inspection.";
    }
    else if (code == "Not enough memory") {
        explanation = "This happens either during astrometry or coaddition.";
        suggestion = "If in astrometry, try reducing the number of CPUs by a factor of two. "
                     "Check the memory usage using 'top' on the command line. If doing a coaddition, are you "
                     "creating a huge mosaic? Perhaps you have entered the wrong plate scale?";
    }
    else if (code == "Operation not supported") {
        explanation = "This is a very uncommon error that has not been seen in a long time.";
        suggestion = "Send the logfile to Mischa (schirmer@mpia.de) for inspection.";
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
    else if (code == "CCfits::FITS::CantOpen") {
        explanation = "A FITS file cannot be opened for processing.";
        suggestion = "Check the context of the logfile to see which file is causing the problem. "
                     "This could occur early during processing because of a bad setup of the data directory tree or else.";
    }
    else if (code == "has flux scale = 0: I will take 1 instead") {
        explanation = "This happens during coaddition and implies a wrong / incomplete astrometric solution.";
        suggestion = "Check the logfile to see which image is causing the problem. Perhaps the image is bad "
                     "and could not be solved completely during astrometry.";
    }
    else if (code == "unbound variable") {
        explanation = "This is a very uncommon error that has not been seen in a long time.";
        suggestion = "Send the logfile to Mischa (schirmer@mpia.de) for inspection.";
    }
    else if (code == "incompatible plain sizes in list") {
        explanation = "This means that one or more of the exposures had a different readout geometry.";
        suggestion = "Check the file sizes of the original raw data to easily identify all incomptible images.";
    }
    else if (code == "Could not allocate memory") {
        explanation = "This happens most likely during astrometry or coaddition.";
        suggestion = "If in astrometry, try reducing the number of CPUs by a factor of two.\n"
                     "Check the memory usage using 'top' on the command line. If doing a coaddition, are you "
                     "creating a huge mosaic? Perhaps you have entered the wrong plate scale?";
    }
    else if (code == ".maxlevel.dat: No such file or directory") {
        explanation = "This should never happen and implies a bug.\n"
                      "The file should be reconstructed if absent (for whatever reason).";
        suggestion = "Send the logfile to Mischa (schirmer@mpia.de) for inspection.";
    }
    else if (code == "ERROR: No photometric reference sources retrieved!") {
        explanation = "This occurs when determining an absolute photometric zeropoint.";
        suggestion = "Is the WCS in your image good? Is the network connection working? "
                     "If you used a catalog that does not have full sky coverage (e.g. SDSS, PANSTARRS), then "
                     "this message indicates that your field is probably not covered by that catalog.";
    }
    else if (code == "cannot determine data size in bytes in ext") {
        explanation = "Usually happens during HDU reformatting when FITS images do not have the expected number of FITS extensions.";
        suggestion = "Check the number of extensions in your data, it should match the number of detectors.\n"
                     "If not, then either something went wrong when the raw file was recorded, or the observatory "
                     "has changed the format, e.g. appended a binary table with telemetry or other metadata. "
                     "In the latter case likely all exposures will have the problem. The extension will be disregarded by THELI "
                     "and with a little luck the expected number of detectors was extracted. In that case, you can just ignore "
                     "the error and proceed.";
    }
    else if (code == "etc. is inconsistent with nb. of extensions expected") {
        explanation = "This happens during HDU reformatting when there is a mix of (multi-chip) raw data "
                      "and images that ran through this step previously.";
        suggestion = "Only raw data must be present at this point, remove all other images.";
    }
    else if (code == "ERROR: No reference sources retrieved!") {
        explanation = "No astrometric reference sources were downloaded from the internet.";
        suggestion = "Either the reference catalog does not cover your field, "
                     "there was a problem with the internet connection, "
                     "or your settings were too tight.\nTry a different catalog, choose a different server "
                     "under 'Edit -> Preferences', check your internet connection, or adjust the magnitude limit and search radius.";
    }
    else if (code == "ERROR: No reference sources extracted!") {
        explanation = "No astrometric reference sources were extracted from the reference image.";
        suggestion = "This means that SExtractor did not detect any sources in the reference image that you provided. "
                     "Check that the image is OK, and perhaps your detection thresholds were too high?";
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
    else if (code == "ERROR: DATE-OBS keyword required by GAIA not found or invalid.") {
        explanation = "The DATE-OBS keyword is required to process the GAIA reference catalog. It could not be found in the FITS headers or is invalid.";
        suggestion = "Use a different reference catalog, or recreate the correct DATE-OBS keyword in the FITS headers.";
    }
    else if (code == "which is below the threshold 0.100000: aborting") {
        explanation = "The median level of the flatfield/bias/dark was outside some allowed (hardcoded) internal thresholds.";
        suggestion = "Check your data for faulty images. Note that this message could be erroneously triggered for a flat if "
                     "the image values are above 60,000 ADU (an internal threshold), hence they are not considered "
                     "for the median calculation and a zero value is returned. In this case, simply divide your flat field "
                     "by a number that brings it down to below the threshold.";
    }
    else if (code == "ERROR: Could not find the wget binary.") {
        explanation = "The 'wget' program for making an online query could not be found on your system.";
        suggestion = "Install 'wget'.";
    }
    else if (code == "ERROR: Invalid standard star catalog") {
        explanation = "The standard star catalog used could not be found in the internal data base.";
        suggestion = "This is most likely a bug. Send the logfile to Mischa (schirmer@mpia.de) for inspection.";
    }
    else if (code == "ERROR: Your standard star field does not overlap") {
        explanation = "The standard star catalog used does not overlap with your standard star observations,"
                      "or the standard stars could not be matched or identified.";
        suggestion = "Use a different catalog. If the catalog does overlap with your field, check whether the"
                     "standard star has high proper motion (quite often the case) and could therefore not be identified.";
    }
    else if (code == "solve-field: invalid option --") {
        explanation = "The syntax of astrometry.net has changed.";
        suggestion = "THELI is not using the correct syntax for astrometry.net. Please send the logfile to schirmer@mpia.de .";
    }
    else if (code == "ERROR: The flatfield does not contain the FILTER keyword.") {
        explanation = "The filter keyword is needed to name the global weight maps accordingly, "
                "so that several global weights can coexist in the WEIGHTS directory for multi-color data sets.";
        suggestion = "This should not have happened. Recreate the flat fields, using a single CPU, and inspect the logfile. "
                "If this has happened with a predefined instrument, please also send the logfile to Mischa (schirmer@mpia.de)";
    }
    else if (code == "ERROR: FILTER keyword not found in science exposure") {
        explanation = "The filter keyword is needed to identify the correct weight map.";
        suggestion = "This should not have happened. The FILTER keyword must be present in all science exposures. "
                "If this has happened with a predefined instrument, please send the logfile created during HDU "
                "reformatting to Mischa (schirmer@mpia.de)";
    }
    else if (code == "error reading input" || code == "does not exist, cannot be read, or is no FITS file") {
        explanation = "Probably a FITS file could not be read, or a file that isn't a FITS file was present during HDU reformatting.";
        suggestion = "If this happens during HDU reformatting, then other files than FITS files were present in this directory. "
                     "At this very first step THELI tries to process all files it can find in that directory, and fails if a file is"
                     "not a FITS image. This error is harmless, because all real FITS files were processed correctly. Check the RAWDATA/"
                     "directory whether this was actually the case, and try avoiding it in the future. All subsequent steps in THELI do not suffer from"
                     "this problem.";
    }
    else if (code == "ERROR: Could not find MJD-OBS keyword for nonsidereal motion correction.") {
        explanation = "The MJD-OBS keyword is required to calculate the correct offsets for non-sidereal targets.";
        suggestion = "The MJD-OBS keyword was not (or not correctly) propagated from the raw data during HDU reformatting."
                     "If you work with a pre-defined instrument in THELI then this is a bug and you should notify Mischa (schirmer@mpia.de) about it."
                     "Otherwise fix your HDF reformatting script (process_split_<xxx>.sh) such that it correctly propagates MJD-OBS.";
    }
    else if (code == "ERROR: Could not create TIFF from RAW") {
        explanation = "dcraw could not convert the RAW format to TIFF.";
        suggestion = "Check your installation of the libtiff library.";
    }
    else if (code == "*Error*: cannot open") {
        explanation = "A file was not created or could not be opened.";
        suggestion = "Check the context of the logfile to see which file is causing the problem.";
    }
    else if (code == "Error: Target resolver: Unknown host") {
        explanation = "The server for target coordinate resolution could not be looked up.";
        suggestion = "Check your internet connection.";
    }
    else if (code == "ERROR: Target could not be resolved.") {
        explanation = "The coordinates for the target could not be resolved.";
        suggestion = "The target could not be found in the Simbad, NED and VizieR databases. "
                     "Check the name for spelling errors, or provide the coordinates manually.";
    }
    else if (code == "ERROR: PanSTARRS queries over radii larger than 0.5 deg are not permitted.") {
        explanation = "PanSTARRS queries cannot be done from the command line over a search radius larger than 0.5 degrees.";
        suggestion = "Either you provided a manual search radius larger than half a degree, "
                     "or THELI determined internally that such a large search radius is required. "
                     "In either case, provide a manual search radius smaller than 0.5 degrees.";
    }
    else if (code == "error") {
        explanation = "";
        suggestion = "Check the logfile to see the context.\nMost likely there is a problem at an earlier stage.";
    }
    else if (code == "ERROR") {
        explanation = "";
        suggestion = "Check the logfile to see the context.\nMost likely there is a problem at an earlier stage.";
    }
}

void ErrorDialog::on_errorLogfileLineEdit_textChanged(const QString &arg1)
{
    paintPathLineEdit(ui->errorLogfileLineEdit, arg1, "file");
}
