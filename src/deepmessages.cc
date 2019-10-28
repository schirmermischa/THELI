#include "mainwindow.h"
#include <QDebug>
#include <QMessageBox>

// This source file handles signals emitted from within the processing threads

// Show error messages
void MainWindow::showMessageBoxReceived(QString trigger, QString part1, QString part2)
{
    if (trigger == "Controller::GAP_DYNAMIC_FOUND" && !GAP_DYNAMIC_FOUND_shown) {
        GAP_DYNAMIC_FOUND_shown = true;
        QMessageBox::warning(this, tr("Significant gap found in background image series"),
                             tr("A gap of ")+part1+tr(" hours was found ")+
                             tr("in the dynamic series of background images, exceeding the maximum ")+
                             tr("allowed gap size of ")+part2+
                             tr(" hours.\nUsually, a gap size does not need to be defined with dynamic background modeling."),
                             QMessageBox::Ok);
    }
    else if (trigger == "Controller::INSUFFICIENT_BACKGROUND_NUMBER" && !INSUFFICIENT_BACKGROUND_NUMBER_shown) {
        INSUFFICIENT_BACKGROUND_NUMBER_shown = true;
        QMessageBox::warning(this, tr("Insufficient number of suitable background images."),
                             part1+tr(" images were found to construct a background model. ")+
                             tr("This is less than the required minimum of ")+part2+tr(" images."),
                             QMessageBox::Ok);
    }
    else if (trigger == "Controller::WINDOWSIZE_TOO_LARGE" && !WINDOWSIZE_TOO_LARGE_shown) {
        WINDOWSIZE_TOO_LARGE_shown = true;
        QMessageBox::warning(this, tr("Window size too large"),
                             tr("The currently selected window size of ")+part1+tr(" images is larger than the number")+
                             tr("of images (")+part2+tr(") available for background correction. Aborting."),
                             QMessageBox::Ok);
    }
    else if (trigger == "Controller::SKY_FILE_NOT_FOUND" && !SKY_FILE_NOT_FOUND_shown) {
        SKY_FILE_NOT_FOUND_shown = true;
        QMessageBox::warning(this, tr("File not found / no sky positions selected"),
                             tr("The file with blank sky positions could not be found or is empty:")+"\n"+
                             part1+"/skysamples.dat\n"+
                             tr("Click on \"Select sky area(s)\" top open iView and select at least one empty sky area with the middle mouse button."),
                             QMessageBox::Ok);
    }
    else if (trigger == "Controller::NO_OVERLAP_WITH_SKYAREA" && !NO_OVERLAP_WITH_SKYAREA_shown) {
        NO_OVERLAP_WITH_SKYAREA_shown = true;
        QMessageBox::warning(this, tr("Insufficient sky data"),
                             tr("The following exposure did not overlap with any of the manually defined sky positions:")+"\n"+
                             part1+"\nAborting.",
                             QMessageBox::Ok);
    }
    else if (trigger == "Controller::MASTER_BIAS_NOT_FOUND" && !MASTER_BIAS_NOT_FOUND_shown) {
        MASTER_BIAS_NOT_FOUND_shown = true;
        QMessageBox::warning(this, tr("Master BIAS not found"),
                             tr("The following master BIAS or DARK was not found for calibrating science images:")+"\n"+
                             part1+
                             tr("\nYou must create it first."),
                             QMessageBox::Ok);
    }
    else if (trigger == "Controller::MASTER_FLAT_NOT_FOUND" && !MASTER_FLAT_NOT_FOUND_shown) {
        MASTER_FLAT_NOT_FOUND_shown = true;
        QMessageBox::warning(this, tr("Master FLAT not found"),
                             tr("The following master FLAT was not found for calibrating science images:")+"\n"+
                             part1+
                             tr("\nYou must create it first."),
                             QMessageBox::Ok);
    }
    else if (trigger == "Controller::MASTER_FLATOFF_NOT_FOUND" && !MASTER_FLATOFF_NOT_FOUND_shown) {
        MASTER_FLATOFF_NOT_FOUND_shown = true;
        QMessageBox::warning(this, tr("Master Calibrator not found"),
                             tr("Master BIAS or FLAT_OFF not found for creation of master FLAT!")+"\n"+
                             part1+
                             tr("You must create it first."),
                             QMessageBox::Ok);
    }
    else if (trigger == "Controller::MASTER_BIAS_NOT_FOUND_GLOBW" && !MASTER_BIAS_NOT_FOUND_GLOBW_shown) {
        MASTER_BIAS_NOT_FOUND_GLOBW_shown = true;
        QMessageBox::warning(this, tr("Master BIAS or FLATOFF not found"),
                             tr("Master BIAS or FLATOFF not found, needed to create the global weight.")+"\n"+
                             part1+
                             tr("You must create it first."),
                             QMessageBox::Ok);
    }
    else if (trigger == "Controller::MASTER_FLAT_NOT_FOUND_GLOBW" && !MASTER_FLAT_NOT_FOUND_GLOBW_shown) {
        MASTER_FLAT_NOT_FOUND_GLOBW_shown = true;
        QMessageBox::warning(this, tr("Master FLAT not found"),
                             tr("Master FLAT not found, needed to create the global weight.")+"\n"+
                             part1+
                             tr("You must create it first."),
                             QMessageBox::Ok);
    }
    else if (trigger == "Controller::NO_MJDOBS_FOR_PM" && !NO_MJDOBS_FOR_PM_shown) {
        NO_MJDOBS_FOR_PM_shown = true;
        QMessageBox::warning(this, tr("MJD-OBS not found"),
                             tr("The MJD-OBS keyword was not found or zero in at least one of the exposures.")+"\n"+
                             tr("It is required for the requested non-sidereal motion correction. Aborting."),
                             QMessageBox::Ok);
    }
    else if (trigger == "Controller::CANNOT_UPDATE_HEADER_WITH_PM_READ" && !CANNOT_UPDATE_HEADER_WITH_PM_READ_shown) {
        CANNOT_UPDATE_HEADER_WITH_PM_READ_shown = true;
        QMessageBox::warning(this, tr("Cannot read header file"),
                             tr("The header file") + part1 + tr("\ncould not be opened for reading. Needed to update with the non-sidereal proper motion.")+"\n"+
                             tr("Error is: ") + part2 + "\nAborting.",
                             QMessageBox::Ok);
    }
    else if (trigger == "Controller::CANNOT_UPDATE_HEADER_WITH_PM_WRITE" && !CANNOT_UPDATE_HEADER_WITH_PM_WRITE_shown) {
        CANNOT_UPDATE_HEADER_WITH_PM_WRITE_shown = true;
        QMessageBox::warning(this, tr("Cannot write header file"),
                             tr("The header file\n") + part1 + tr("\ncould not be opened for writing. Needs to be updated with the non-sidereal proper motion.")+"\n"+
                             tr("Error is: ") + part2 + "\nAborting.",
                             QMessageBox::Ok);
    }
    else if (trigger == "Controller::CANNOT_UPDATE_HEADER_WITH_PA" && !CANNOT_UPDATE_HEADER_WITH_PA_shown) {
        CANNOT_UPDATE_HEADER_WITH_PA_shown = true;
        QMessageBox::warning(this, tr("Cannot read header file"),
                             part1 + tr("\ncould not be opened to read the CD matrix. This is needed for the update it with the new position angle.")+"\n"+
                             tr("Error is: ") + part2 + "\nAborting.",
                             QMessageBox::Ok);
    }
    else if (trigger == "Controller::CANNOT_WRITE_RESAMPLE_LIST" && !CANNOT_WRITE_RESAMPLE_LIST_shown) {
        CANNOT_WRITE_RESAMPLE_LIST_shown = true;
        QMessageBox::warning(this, tr("Cannot write file"),
                             tr("Coaddition: Could not append image names to ")+part1+"<br>"+
                             tr("Error is: ") + part2 + "<br>Aborting.",
                             QMessageBox::Ok);
    }
    else if (trigger == "Controller::CANNOT_OPEN_FILE" && !CANNOT_OPEN_FILE_shown) {
        CANNOT_OPEN_FILE_shown = true;
        QMessageBox::warning(this, tr("Cannot open file for read/write"),
                             tr("The following file could not be opened: ")+part1+"<br>"+
                             tr("Error is: ") + part2 + "<br>Aborting.",
                             QMessageBox::Ok);
    }
    else if (trigger == "Data::CANNOT_READ_HEADER_KEYS" && !CANNOT_READ_HEADER_KEYS_shown) {
        CANNOT_READ_HEADER_KEYS_shown = true;
        QMessageBox::warning(this, tr("Could not read one or more of the following keywords:\nNAXIS1, NAXIS2, FILTER, MJD-OBS"),
                             tr("Add the keywords manually, or the remove the corrupted images.\n")+part1,
                             QMessageBox::Ok);
    }
    else if (trigger == "Data::INCONSISTENT_DATA_STATUS" && INCONSISTENT_DATA_STATUS_shown) {
        INCONSISTENT_DATA_STATUS_shown = true;
        QMessageBox::warning(this, tr("Inconsistent data status detected in ")+part1,
                             part1 + " : " + tr("The FITS files found do not match the recorded status (*") + part2 +".fits).\n"+
                             tr("Either restore the files manually, or use the 'Processing status' menu to reflect the current status. A restart is recommended."),
                QMessageBox::Ok);
    }
    else if (trigger == "Data::Duplicate_MJDOBS" && !DUPLICATE_MJDOBS_shown) {
        DUPLICATE_MJDOBS_shown = true;
        QMessageBox::warning(this, tr("One or more images have identical MJD-OBS header keywords."),
                             tr("THELI cannot build a list of contemporal images for the background model in")+part1+
                             tr("<br>You must first fix the MJD-OBS keywords to their correct values."),
                             QMessageBox::Ok);
    }
    else if (trigger == "Data::IMAGES_NOT_FOUND" && !IMAGES_NOT_FOUND_shown) {
        IMAGES_NOT_FOUND_shown = true;
        QMessageBox::warning(this, tr("ERROR: Images not found"),
                             tr("Could not find any images of type :<br>")+"*"+part1+".fits" + "<br>in "+part2 +
                             tr("<br>Check that the recorded processing status matches the currently present images."),
                             QMessageBox::Ok);
    }
    else if (trigger == "Controller::TARGET_UNRESOLVED") {
        QMessageBox::warning(this, tr("Target not found"),
                             tr("The coordinates of ") + part1 + tr(" could not be resolved.")+
                             tr("<br>Check for typos, or try a different name of the same target."),
                             QMessageBox::Ok);
    }
    else if (trigger == "Controller::UNKNOWN_HOST") {
        QMessageBox::warning(this, tr("Unknown host"),
                             tr("The host used by the 'sesame' service to resolve the coordinates for ")+
                             part1+tr(" is unknown, or offline."),
                             QMessageBox::Ok);
    }
    else if (trigger == "Colorpicture::EmptySelection") {
        QMessageBox::warning(this, tr("Empty selection"),
                             tr("At least two images must be chosen from the list of coadded images."),
                             QMessageBox::Ok);
    }
    else if (trigger == "Splitter::IncompatibleSizeRAW" && !IncompatibleSizeRAW_shown) {
        IncompatibleSizeRAW_shown = true;
        QMessageBox::warning(this, tr("Incompatible detector geometry in setup"),
                             tr("You must adjust the SIZEX and SIZEY parameters in ")+part1+
                             tr(" to match the geometry defined in the RAW file: ")+part2,
                             QMessageBox::Ok);
    }
    else if (trigger == "Data::WEIGHT_DATA_NOT_FOUND") {
        QMessageBox::warning(this, tr("Missing weight images"),
                             tr("The following ")+part1+tr(" images do not have a matching weight image (WEIGHTS/*weight.fits):\n")+part2,
                             QMessageBox::Ok);
    }
    else if (trigger == "Data::HEADER_DATA_NOT_FOUND") {
        QMessageBox::warning(this, tr("Missing astrometric headers"),
                             tr("The following ")+part1+tr(" images do not have a matching astrometric headers (headers/*.head):\n")+part2,
                             QMessageBox::Ok);
    }
}

void MainWindow::resetProcessingErrorFlags()
{
    GAP_DYNAMIC_FOUND_shown = false;
    WINDOWSIZE_TOO_LARGE_shown = false;
    INSUFFICIENT_BACKGROUND_NUMBER_shown = false;
    SKY_FILE_NOT_FOUND_shown = false;
    NO_OVERLAP_WITH_SKYAREA_shown = false;
}
