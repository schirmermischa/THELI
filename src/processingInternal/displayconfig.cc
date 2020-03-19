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

void Controller::pushConfigCreatesourcecat()
{
    QString config;
    config = "<tt>";
    config += "DT = " + cdw->ui->CSCDTLineEdit->text() + "<br>";
    config += "DMIN = " + cdw->ui->CSCDMINLineEdit->text() + "<br>";
    config += "DEBLEND = " + cdw->ui->CSCmincontLineEdit->text() + "<br>";
    config += "Min FWHM = " + cdw->ui->CSCFWHMLineEdit->text() + "<br>";
    config += "Convolution = " + boolToString(cdw->ui->CSCconvolutionCheckBox->isChecked()) + "<br>";
    config += "Max FLAG = " + cdw->ui->CSCmaxflagLineEdit->text() + "<br>";
    config += "Saturation = " + cdw->ui->CSCsaturationLineEdit->text() + "<br>";
    config += "Background level = " + cdw->ui->CSCbackgroundLineEdit->text() + "<br>";
    config += "Min # of objects = " + cdw->ui->CSCminobjectsLineEdit->text() + "<br>";
    config += "Many hot pixels = " + boolToString(cdw->ui->CSCsamplingCheckBox->isChecked()) + "<br>";
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
