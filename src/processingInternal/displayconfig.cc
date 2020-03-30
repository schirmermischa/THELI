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

#include "controller.h"

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "../dockwidgets/confdockwidget.h"
#include "../dockwidgets/monitor.h"
#include "ui_confdockwidget.h"

#include <QString>

void Controller::populateCommentMap()
{
    commentMap.clear();
    commentMap.insert("HDUreformat", "HDU reformatting");
    commentMap.insert("Processbias", "Processing biases in");
    commentMap.insert("Processdark", "Processing darks in");
    commentMap.insert("Processflatoff", "Processing flat off/darks in");
    commentMap.insert("Processflat", "Processing flats in");
    commentMap.insert("Processscience", "Debiasing and flatfielding data in");
    commentMap.insert("Chopnod", "Doing chopnod background correction for");
    commentMap.insert("Background", "Applying background model to");
    commentMap.insert("Collapse", "Applying collapse correction to");
    commentMap.insert("Binnedpreview", "Creating binned previews for");
    commentMap.insert("Globalweight", "Creating global weights for");
    commentMap.insert("Individualweight", "Creating individual weights for");
    commentMap.insert("Separate", "Separating different targes in");
    commentMap.insert("Absphotindirect", "Performing indirect absolute photometry");
    commentMap.insert("Createsourcecat", "Creating source catalogs for");
    commentMap.insert("GetCatalogFromWEB", "Downloading astrometric reference catalog for");
    commentMap.insert("GetCatalogFromIMAGE", "Extracting astrometric reference catalog for");
    commentMap.insert("Astromphotom", "Calculating astrometric solution for");
    commentMap.insert("Resolvetarget", "Resolving target coordinates for");
    commentMap.insert("RestoreHeader", "Restoring original astrometry in images for");
    commentMap.insert("CopyZeroOrder", "Updating images with 0th order WCS solution for");
    commentMap.insert("ImageQuality", "Analyzing image quality for");
    commentMap.insert("SkysubModel", "Subtracting the sky for");
    commentMap.insert("SkysubConst", "Subtracting the sky for");
    commentMap.insert("SkysubPoly", "Subtracting the sky for");
    commentMap.insert("Coaddition", "Performing coaddition for");
    commentMap.insert("CoaddSmoothWeights", "Smoothing edges in weight maps for");
    commentMap.insert("CoaddPrepare", "Preparing coaddition for");
    commentMap.insert("CoaddResample", "Resampling images for");
    commentMap.insert("CoaddSwarpFilter", "Outlier filtering for");
    commentMap.insert("CoaddCoadd", "Coaddition images for");
    commentMap.insert("CoaddUpdate", "Updating header of coadd.fits for");
}

void Controller::pushConfigHDUreformat()
{
    QString config;
    config = "<tt>";
    config += "Overscan correction = "+ boolToString(cdw->ui->overscanCheckBox->isChecked()) + "<br>";
    config += "Overscan method = "+ cdw->ui->overscanMethodComboBox->currentText() + "<br>";
    config += "Nonlinearity correction = "+ boolToString(cdw->ui->nonlinearityCheckBox->isChecked()) + "<br>";
    config += "Point crosstalk correction = "+ boolToString(cdw->ui->normalxtalkCheckBox->isChecked()) + "<br>";
    config += "Point crosstalk amplitude = "+ cdw->ui->normalxtalkAmplitudeLineEdit->text() + "<br>";
    config += "Row crosstalk correction = "+ boolToString(cdw->ui->rowxtalkCheckBox->isChecked()) + "<br>";
    config += "Row crosstalk amplitude = "+ cdw->ui->rowxtalkAmplitudeLineEdit->text() + "<br>";
    emit messageAvailable(config, "config");
}

void Controller::pushConfigProcessbias()
{
    QString config;
    config = "<tt>";
    config += "Number of low rejected pixels / stack = " + cdw->ui->biasNlowLineEdit->text() + "<br>";
    config += "Number of high rejected pixels / stack = " + cdw->ui->biasNhighLineEdit->text() + "<br>";
    config += "Min mode allowed [e-] = " + cdw->ui->biasMinLineEdit->text() + "<br>";
    config += "Max mode allowed [e-] = " + cdw->ui->biasMaxLineEdit->text() + "<br>";
    emit messageAvailable(config, "config");
}

void Controller::pushConfigProcessdark()
{
    QString config;
    config = "<tt>";
    config += "Number of low rejected pixels / stack = " + cdw->ui->darkNlowLineEdit->text() + "<br>";
    config += "Number of high rejected pixels / stack = " + cdw->ui->darkNhighLineEdit->text() + "<br>";
    config += "Min mode allowed [e-] = " + cdw->ui->darkMinLineEdit->text() + "<br>";
    config += "Max mode allowed [e-] = " + cdw->ui->darkMaxLineEdit->text() + "<br>";
    emit messageAvailable(config, "config");
}

void Controller::pushConfigProcessflat()
{
    QString config;
    config = "<tt>";
    config += "Number of low rejected pixels / stack = " + cdw->ui->flatNlowLineEdit->text() + "<br>";
    config += "Number of high rejected pixels / stack = " + cdw->ui->flatNhighLineEdit->text() + "<br>";
    config += "Min mode allowed [e-] = " + cdw->ui->flatMinLineEdit->text() + "<br>";
    config += "Max mode allowed [e-] = " + cdw->ui->flatMaxLineEdit->text() + "<br>";
    emit messageAvailable(config, "config");
}

void Controller::pushConfigProcessflatoff()
{
    QString config;
    config = "<tt>";
    config += "Number of low rejected pixels / stack = " + cdw->ui->flatoffNlowLineEdit->text() + "<br>";
    config += "Number of high rejected pixels / stack = " + cdw->ui->flatoffNhighLineEdit->text() + "<br>";
    config += "Min mode allowed [e-] = " + cdw->ui->flatoffMinLineEdit->text() + "<br>";
    config += "Max mode allowed [e-] = " + cdw->ui->flatoffMaxLineEdit->text() + "<br>";
    emit messageAvailable(config, "config");
}

void Controller::pushConfigBackground()
{
    QString mode = " (STATIC)";
    if (cdw->ui->BACwindowLineEdit->text() != "0") mode = " (DYNAMIC)";
    QString config;
    config = "<tt>";
    config += "Detection threshold (DT) = " + cdw->ui->BACDTLineEdit->text() + "<br>";
    config += "Detection min area (DMIN) = " + cdw->ui->BACDMINLineEdit->text() + "<br>";
    config += "Two-pass background model = " + boolToString(cdw->ui->BAC2passCheckBox->isChecked()) + "<br>";
    config += "Convolve detections before masking = " + boolToString(cdw->ui->BACconvolutionCheckBox->isChecked()) + "<br>";
    config += "Mask expansion factor = " + cdw->ui->BACmefLineEdit->text() + "<br>";
    config += "Combination method = " + cdw->ui->BACmethodComboBox->currentText() + "<br>";
    config += "Number of low rejected pixels (1st pass) = " + cdw->ui->BAC1nlowLineEdit->text() + "<br>";
    config += "Number of high rejected pixels (1st pass) = " + cdw->ui->BAC1nhighLineEdit->text() + "<br>";
    config += "Number of low rejected pixels (2nd pass) = " + cdw->ui->BAC2nlowLineEdit->text() + "<br>";
    config += "Number of high rejected pixels (2nd pass) = " + cdw->ui->BAC2nhighLineEdit->text() + "<br>";
    config += "Reject detectors with stars brighter than [mag] = " + cdw->ui->BACmagLineEdit->text() + "<br>";
    config += "Minimum distance from detector edge ['] = " + cdw->ui->BACdistLineEdit->text() + "<br>";
    config += "Correction method = " + cdw->ui->BACapplyComboBox->currentText() + "<br>";
    config += "Model smoothing kernel width [pixel] = " + cdw->ui->BACbacksmoothscaleLineEdit->text() + "<br>";
    config += "Rescale model to individual exposure = " + boolToString(cdw->ui->BACrescaleCheckBox->isChecked()) + "<br>";
    config += "Window size = " + cdw->ui->BACwindowLineEdit->text() + mode + "<br>";
    config += "Min useful window size = " + cdw->ui->BACminWindowLineEdit->text() + "<br>";
    config += "Max gap size [hour] = " + cdw->ui->BACgapsizeLineEdit->text() + "<br>";
    emit messageAvailable(config, "config");
}

void Controller::pushConfigCollapse()
{
    QString config;
    config = "<tt>";
    config += "Detection threshold (DT) = " + cdw->ui->COCDTLineEdit->text() + "<br>";
    config += "Detection min area (DMIN) = " + cdw->ui->COCDMINLineEdit->text() + "<br>";
    config += "Mask expansion factor = " + cdw->ui->COCmefLineEdit->text() + "<br>";
    config += "Rejection threshold [sigma] = " + cdw->ui->COCrejectLineEdit->text() + "<br>";
    config += "Collapse direction = " + cdw->ui->COCdirectionComboBox->currentText() + "<br>";
    emit messageAvailable(config, "config");
}

void Controller::pushConfigBinnedpreview()
{
    QString config;
    config = "<tt>";
    config += "Binning factor = " + cdw->ui->BIPSpinBox->text() + "<br>";
    config += "Reject outliers = " + boolToString(cdw->ui->BIPrejectCheckBox->isChecked()) + "<br>";
    emit messageAvailable(config, "config");
}

void Controller::pushConfigGlobalweight()
{
    QString config;
    config = "<tt>";
    config += "Same weight for all pixels = " + boolToString(cdw->ui->CGWsameweightCheckBox->isChecked()) + "<br>";
    config += "Min value allowed for BIAS / DARK [e-] = " + cdw->ui->CGWdarkminLineEdit->text() + "<br>";
    config += "Max value allowed for BIAS / DARK [e-] = " + cdw->ui->CGWdarkmaxLineEdit->text() + "<br>";
    config += "Min value allowed for norm. FLAT [e-] = " + cdw->ui->CGWflatminLineEdit->text() + "<br>";
    config += "Max value allowed for norm. FLAT [e-] = " + cdw->ui->CGWflatmaxLineEdit->text() + "<br>";
    config += "Min value allowed for background model [e-] = " + cdw->ui->CGWbackminLineEdit->text() + "<br>";
    config += "Max value allowed for background model [e-] = " + cdw->ui->CGWbackmaxLineEdit->text() + "<br>";
    config += "Defect detection in FLAT: Kernel size = " + cdw->ui->CGWflatsmoothLineEdit->text() + "<br>";
    config += "Defect detection in FLAT: Row tolerance = " + cdw->ui->CGWflatrowtolLineEdit->text() + "<br>";
    config += "Defect detection in FLAT: Column tolerance = " + cdw->ui->CGWflatcoltolLineEdit->text() + "<br>";
    config += "Defect detection in FLAT: Cluster tolerance = " + cdw->ui->CGWflatclustolLineEdit->text() + "<br>";
    config += "Defect detection in BACK: Kernel size for = " + cdw->ui->CGWbacksmoothLineEdit->text() + "<br>";
    config += "Defect detection in BACK: Row tolerance = " + cdw->ui->CGWbackrowtolLineEdit->text() + "<br>";
    config += "Defect detection in BACK: Column tolerance = " + cdw->ui->CGWbackcoltolLineEdit->text() + "<br>";
    config += "Defect detection in BACK: Cluster tolerance = " + cdw->ui->CGWbackclustolLineEdit->text() + "<br>";
    emit messageAvailable(config, "config");
}

void Controller::pushConfigIndividualweight()
{
    QString config;
    config = "<tt>";
    config += "Min value allowed in image [e-] = " + cdw->ui->CIWminaduLineEdit->text() + "<br>";
    config += "Max value allowed in image [e-] = " + cdw->ui->CIWmaxaduLineEdit->text() + "<br>";
    config += "Detection aggressiveness for cosmics = " + cdw->ui->CIWaggressivenessLineEdit->text() + "<br>";
    config += "Mask blooming spikes = " + boolToString(cdw->ui->CIWmaskbloomingCheckBox->isChecked()) + "<br>";
    config += "Blooming spike min value [e-] = " + cdw->ui->CIWbloomMinaduLineEdit->text() + "<br>";
    config += "Blooming spike dynamic range [e-] = " + cdw->ui->CIWbloomRangeLineEdit->text() + "<br>";
    config += "Mask persistent pixels = " + boolToString(cdw->ui->CIWpersistenceCheckBox->isChecked()) + "<br>";
    config += "Maximum lookback time [min] = " + cdw->ui->CIWpersistenceTimescaleLineEdit->text() + "<br>";
    config += "Persistence trigger level [e-] = " + cdw->ui->CIWpersistenceMinaduLineEdit->text() + "<br>";
    emit messageAvailable(config, "config");
}

void Controller::pushConfigCreatesourcecat()
{
    QString config;
    config = "<tt>";
    config += "Detection threshold (DT) = " + cdw->ui->CSCDTLineEdit->text() + "<br>";
    config += "Detection min area (DMIN) = " + cdw->ui->CSCDMINLineEdit->text() + "<br>";
    config += "DEBLEND = " + cdw->ui->CSCmincontLineEdit->text() + "<br>";
    config += "Minimum FWHM [pixel] = " + cdw->ui->CSCFWHMLineEdit->text() + "<br>";
    config += "Convolve detections = " + boolToString(cdw->ui->CSCconvolutionCheckBox->isChecked()) + "<br>";
    config += "Max FLAG = " + cdw->ui->CSCmaxflagLineEdit->text() + "<br>";
    config += "Saturation [e-] = " + cdw->ui->CSCsaturationLineEdit->text() + "<br>";
    config += "Background level [e-] = " + cdw->ui->CSCbackgroundLineEdit->text() + "<br>";
    config += "Many hot pixels = " + boolToString(cdw->ui->CSCsamplingCheckBox->isChecked()) + "<br>";
    config += "Min # of objects per detector = " + cdw->ui->CSCrejectExposureLineEdit->text() + "<br>";
    emit messageAvailable(config, "config");
}

void Controller::pushConfigGetCatalogFromWeb()
{
    QString config;
    QString pm;
    QString ra = cdw->ui->ARCraLineEdit->text();
    QString dec = cdw->ui->ARCdecLineEdit->text();
    QString mag = cdw->ui->ARCminmagLineEdit->text();
    QString rad = cdw->ui->ARCradiusLineEdit->text();
    if (cdw->ui->ARCmaxpmLineEdit->text().isEmpty()) pm = "unconstrained";
    if (ra.isEmpty()) ra = "automatic";
    if (dec.isEmpty()) dec = "automatic";
    if (mag.isEmpty()) mag = "unconstrained";
    if (rad.isEmpty()) rad = "automatic";
    else rad.append("'");

    config = "<tt>";
    config += "Catalog = " + cdw->ui->ARCcatalogComboBox->currentText() + "<br>";
    config += "Max allowed pm = " + pm + "<br>";
    config += "Field center RA = " + ra + "<br>";
    config += "Field center DEC = " + dec + "<br>";
    config += "Target name = " + cdw->ui->ARCtargetresolverLineEdit->text() + "<br>";
    config += "Magnitude limit = " + mag + "<br>";
    config += "Radius = " + rad + "<br>";
    emit messageAvailable(config, "config");
}

void Controller::pushConfigGetCatalogFromImage()
{
    QString config;
    config = "<tt>";
    config += "FITS file = " + cdw->ui->ARCselectimageLineEdit->text() + "<br>";
    config += "DT = " + cdw->ui->ARCDTLineEdit->text() + "<br>";
    config += "DMIN = " + cdw->ui->ARCDMINLineEdit->text() + "<br>";
    emit messageAvailable(config, "config");
}

void Controller::pushConfigAstromphotom()
{
    QString config;
    config = "<tt>";
    config += "POSANGLE_MAXERR = " + cdw->ui->ASTposangleLineEdit->text() + "<br>";
    config += "POSITION_MAXERR = " + cdw->ui->ASTpositionLineEdit->text() + "<br>";
    config += "PIXSCALE_MAXERR = " + cdw->ui->ASTpixscaleLineEdit->text() + "<br>";
    config += "CROSSID_RADIUS = " + cdw->ui->ASTcrossidLineEdit->text() + "<br>";
    config += "ASTREF_WEIGHT = " + cdw->ui->ASTastrefweightLineEdit->text() + "<br>";
    config += "SN_THRESHOLDS = " + cdw->ui->ASTsnthreshLineEdit->text() + "<br>";
    config += "ASTR_INSTRUKEY = " + cdw->ui->ASTastrinstrukeyLineEdit->text() + "<br>";
    config += "PHOT_INSTRUKEY = " + cdw->ui->ASTphotinstrukeyLineEdit->text() + "<br>";
    config += "DISTORT_DEGREES = " + cdw->ui->ASTdistortLineEdit->text() + "<br>";
    config += "DISTORT_GROUPS = " + cdw->ui->ASTdistortgroupsLineEdit->text() + "<br>";
    config += "DISTORT_KEYS = " + cdw->ui->ASTdistortkeysLineEdit->text() + "<br>";
    config += "Astrometric solver = " + cdw->ui->ASTmethodComboBox->currentText() + "<br>";
    config += "Matching method = " + cdw->ui->ASTmatchMethodComboBox->currentText() + "<br>";
    config += "STABILITY_TYPE = " + cdw->ui->ASTstabilityComboBox->currentText() + "<br>";
    config += "MOSAIC_TYPE = " + cdw->ui->ASTmosaictypeComboBox->currentText() + "<br>";
    config += "FPA mode = " + cdw->ui->ASTfocalplaneComboBox->currentText() + "<br>";
    config += "Match flipped images = " + boolToString(cdw->ui->ASTmatchflippedCheckBox->isChecked()) + "<br>";
    emit messageAvailable(config, "config");
}

void Controller::pushConfigSkysubModel()
{
    QString config;
    config = "<tt>";
    config += "Detection threshold (DT) = " + cdw->ui->skyDTLineEdit->text() + "<br>";
    config += "Detection min area (DMIN) = " + cdw->ui->skyDMINLineEdit->text() + "<br>";
    config += "Kernel width [pixel] = " + cdw->ui->skyKernelLineEdit->text() + "<br>";
    config += "Mask expansion factor = " + cdw->ui->skyMefLineEdit->text() + "<br>";
    emit messageAvailable(config, "config");
}

void Controller::pushConfigSkysubConst()
{
    QString config;
    config = "<tt>";
    config += "Constant sky measurement area = " + cdw->ui->skyAreaComboBox->currentText() + "<br>";
    if (cdw->ui->skyAreaComboBox->currentIndex() != 0) {
        config += "Detection threshold (DT) = " + cdw->ui->skyDTLineEdit->text() + "<br>";
        config += "Detection min area (DMIN) = " + cdw->ui->skyDMINLineEdit->text() + "<br>";
        config += "Kernel width [pixel] = " + cdw->ui->skyKernelLineEdit->text() + "<br>";
        config += "Mask expansion factor = " + cdw->ui->skyMefLineEdit->text() + "<br>";
    }
    emit messageAvailable(config, "config");
}

void Controller::pushConfigSkysubPoly()
{
    QString config;
    config = "<tt>";
    emit messageAvailable(config, "config");
}

void Controller::pushConfigCoadd()
{
    QString config;
    config = "<tt>";
    config += "Reference RA    = " + cdw->ui->COAraLineEdit->text() + "<br>";
    config += "Reference DEC   = " + cdw->ui->COAdecLineEdit->text() + "<br>";
    config += "Plate scale     = " + cdw->ui->COApixscaleLineEdit->text() + "<br>";
    config += "Sky PA          = " + cdw->ui->COAskypaLineEdit->text() + "<br>";
    config += "COMBINETYPE     = " + cdw->ui->COAcombinetypeComboBox->currentText() + "<br>";
    config += "Resamp Kernel   = " + cdw->ui->COAkernelComboBox->currentText() + "<br>";
    config += "Sky projection  = " + cdw->ui->COAprojectionComboBox->currentText() + "<br>";
    config += "Celestial type  = " + cdw->ui->COAcelestialtypeComboBox->currentText() + "<br>";
    config += "Rescale weights = " + boolToString(cdw->ui->COArescaleweightsCheckBox->isChecked()) + "<br>";
    config += "Outlier thresh  = " + cdw->ui->COAoutthreshLineEdit->text() + "<br>";
    config += "Outlier numpix  = " + cdw->ui->COAoutsizeLineEdit->text() + "<br>";
    config += "Outlier border  = " + cdw->ui->COAoutborderLineEdit->text() + "<br>";
    config += "Identifier      = " + cdw->ui->COAuniqueidLineEdit->text() + "<br>";
    config += "Coadd chips     = " + cdw->ui->COAchipsLineEdit->text() + "<br>";
    config += "Edge smoothing  = " + cdw->ui->COAedgesmoothingLineEdit->text() + "<br>";;
    config += "NAXIS1          = " + cdw->ui->COAsizexLineEdit->text() + "<br>";;
    config += "NAXIS2          = " + cdw->ui->COAsizeyLineEdit->text() + "<br>";;
    config += "Nonsidereal RA  = " + cdw->ui->COApmraLineEdit->text() + " " + cdw->ui->COApmComboBox->currentText() + "<br>";
    config += "Nonsidereal DEC = " + cdw->ui->COApmdecLineEdit->text() + " " + cdw->ui->COApmComboBox->currentText() + "<br>";
    config += "Perform fluxcal = " + boolToString(cdw->ui->COAfluxcalibCheckBox->isChecked()) + "<br>";
    config += "</tt>";
    emit messageAvailable(config, "config");
}
