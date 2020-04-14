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

#include "confdockwidget.h"
#include "ui_confdockwidget.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

void ConfDockWidget::populateDefaultMap()
{
    // These are the defaults that will be used if an empty parameter field
    // would be passed onto the scripts
    // Only LineEdits
    defaultMap.insert("APDmaxphoterrorLineEdit", "0.05");
    defaultMap.insert("APIWCSLineEdit", "10");
    defaultMap.insert("APIapertureLineEdit", "10");
    defaultMap.insert("APIcolortermLineEdit", "0.0");
    defaultMap.insert("APIconvergenceLineEdit", "0.01");
    defaultMap.insert("APIextinctionLineEdit", "-0.1");
    defaultMap.insert("APIfilterkeywordLineEdit", "");
    defaultMap.insert("APIkappaLineEdit", "2.0");
    defaultMap.insert("APIminmagLineEdit", "0");
    defaultMap.insert("APIthresholdLineEdit", "0.15");
    defaultMap.insert("APIzpmaxLineEdit", "");
    defaultMap.insert("APIzpminLineEdit", "");
    defaultMap.insert("ARCdecLineEdit", "");
    defaultMap.insert("ARCmaxpmLineEdit", "");
    defaultMap.insert("ARCminmagLineEdit", "25");
    defaultMap.insert("ARCraLineEdit", "");
    defaultMap.insert("ARCradiusLineEdit", "");
    defaultMap.insert("ARCmaxpmLineEdit", "");
    defaultMap.insert("ASTanetpixscaleLineEdit", "1.05");
    defaultMap.insert("ASTastrefweightLineEdit", "1.0");
    defaultMap.insert("ASTastrinstrukeyLineEdit", "FILTER");
    defaultMap.insert("ASTcrossidLineEdit", "1.0");
    defaultMap.insert("ASTdistortLineEdit", "3");
    defaultMap.insert("ASTdistortgroupsLineEdit", "");
    defaultMap.insert("ASTdistortkeysLineEdit", "");
    defaultMap.insert("ASTfgroupLineEdit", "1.0");
    defaultMap.insert("ASTphotinstrukeyLineEdit", "FILTER");
    defaultMap.insert("ASTpixscaleLineEdit", "1.05");
    defaultMap.insert("ASTposangleLineEdit", "2");
    defaultMap.insert("ASTpositionLineEdit", "2.0");
    defaultMap.insert("ASTresolutionLineEdit", "800");
    defaultMap.insert("ASTsnthreshLineEdit", "5,20");
    defaultMap.insert("BAC1nhighLineEdit", "0");
    defaultMap.insert("BAC1nlowLineEdit", "0");
    defaultMap.insert("BAC2nhighLineEdit", "0");
    defaultMap.insert("BAC2nlowLineEdit", "0");
    defaultMap.insert("BACDMINLineEdit", "");
    defaultMap.insert("BACDTLineEdit", "");
    defaultMap.insert("BACbacksmoothscaleLineEdit", "");
    defaultMap.insert("BACdistLineEdit", "");
    defaultMap.insert("BACfringesmoothscaleLineEdit", "");
    defaultMap.insert("BACgapsizeLineEdit", "");
    defaultMap.insert("BACmagLineEdit", "");
    defaultMap.insert("BACmefLineEdit", "");
    defaultMap.insert("BACwindowLineEdit", "0");
    defaultMap.insert("BACminWindowLineEdit", "");
    defaultMap.insert("CGWbackclustolLineEdit", "");
    defaultMap.insert("CGWbackcoltolLineEdit", "");
    defaultMap.insert("CGWbackmaxLineEdit", "");
    defaultMap.insert("CGWbackminLineEdit", "");
    defaultMap.insert("CGWbackrowtolLineEdit", "");
    defaultMap.insert("CGWbacksmoothLineEdit", "");
    defaultMap.insert("CGWdarkmaxLineEdit", "");
    defaultMap.insert("CGWdarkminLineEdit", "");
    defaultMap.insert("CGWflatclustolLineEdit", "");
    defaultMap.insert("CGWflatcoltolLineEdit", "");
    defaultMap.insert("CGWflatmaxLineEdit", "1.5");
    defaultMap.insert("CGWflatminLineEdit", "0.5");
    defaultMap.insert("CGWflatrowtolLineEdit", "");
    defaultMap.insert("CGWflatsmoothLineEdit", "");
    defaultMap.insert("CIWbloomRangeLineEdit", "");
    defaultMap.insert("CIWbloomMinaduLineEdit", "");
    defaultMap.insert("CIWbloomMinareaLineEdit", "");
    defaultMap.insert("CIWaggressivenessLineEdit", "");
    defaultMap.insert("CIWmaxaduLineEdit", "");
    defaultMap.insert("CIWminaduLineEdit", "");
    defaultMap.insert("COAchipsLineEdit", "");
    defaultMap.insert("COAdecLineEdit", "");
    defaultMap.insert("COAedgesmoothingLineEdit", "");
    defaultMap.insert("COAmaxseeingLineEdit", "");
    defaultMap.insert("COAminrzpLineEdit", "");
    defaultMap.insert("COAoutborderLineEdit", "");
    defaultMap.insert("COAoutsizeLineEdit", "4");
    defaultMap.insert("COAoutthreshLineEdit", "3");
    defaultMap.insert("COApmdecLineEdit", "");
    defaultMap.insert("COApmraLineEdit", "");
    defaultMap.insert("COAraLineEdit", "");
    defaultMap.insert("COAsizexLineEdit", "");
    defaultMap.insert("COAsizeyLineEdit", "");
    defaultMap.insert("COAskypaLineEdit", "");
    defaultMap.insert("COAuniqueidLineEdit", "");
    defaultMap.insert("COCDMINLineEdit", "5");
    defaultMap.insert("COCDTLineEdit", "1.5");
    defaultMap.insert("COCmefLineEdit", "");
    defaultMap.insert("COCrejectLineEdit", "1.5");
    defaultMap.insert("COCxmaxLineEdit", "");
    defaultMap.insert("COCxminLineEdit", "");
    defaultMap.insert("COCymaxLineEdit", "");
    defaultMap.insert("COCyminLineEdit", "");
    defaultMap.insert("CSCDMINLineEdit", "5");
    defaultMap.insert("CSCDTLineEdit", "5");
    defaultMap.insert("CSCFWHMLineEdit", "1.5");
    defaultMap.insert("CSCbackgroundLineEdit", "");
    defaultMap.insert("CSCmaxflagLineEdit", "8");
    defaultMap.insert("CSCmincontLineEdit", "0.0005");
    defaultMap.insert("CSCminobjectsLineEdit", "");
    defaultMap.insert("CSCsaturationLineEdit", "1.e8");
    defaultMap.insert("CSCrejectExposureLineEdit", "");
    defaultMap.insert("SPSlengthLineEdit", "10");
    defaultMap.insert("SPSnumbergroupsLineEdit", "3");
    defaultMap.insert("biasMaxLineEdit", "");
    defaultMap.insert("biasMinLineEdit", "");
    defaultMap.insert("biasNhighLineEdit", "");
    defaultMap.insert("biasNlowLineEdit", "");
    defaultMap.insert("darkMaxLineEdit", "");
    defaultMap.insert("darkMinLineEdit", "");
    defaultMap.insert("darkNhighLineEdit", "");
    defaultMap.insert("darkNlowLineEdit", "");
    defaultMap.insert("flatMaxLineEdit", "");
    defaultMap.insert("flatMinLineEdit", "");
    defaultMap.insert("flatNhighLineEdit", "");
    defaultMap.insert("flatNlowLineEdit", "");
    defaultMap.insert("flatoffMaxLineEdit", "");
    defaultMap.insert("flatoffMinLineEdit", "");
    defaultMap.insert("flatoffNhighLineEdit", "");
    defaultMap.insert("flatoffNlowLineEdit", "");
    defaultMap.insert("normalxtalkAmplitudeLineEdit", "0.0");
    defaultMap.insert("overscanNhighLineEdit", "");
    defaultMap.insert("overscanNlowLineEdit", "");
    defaultMap.insert("rowxtalkAmplitudeLineEdit", "0.0");
    defaultMap.insert("separateTargetLineEdit", "");
    defaultMap.insert("skyDMINLineEdit", "10");
    defaultMap.insert("skyDTLineEdit", "1.5");
    defaultMap.insert("skyKernelLineEdit", "256");
    defaultMap.insert("skyMefLineEdit", "");
    defaultMap.insert("skyOverrideLineEdit", "");
    defaultMap.insert("skyVertex1LineEdit", "");
    defaultMap.insert("skyVertex2LineEdit", "");
    defaultMap.insert("skyVertex3LineEdit", "");
    defaultMap.insert("skyVertex4LineEdit", "");
}

void ConfDockWidget::loadDefaults()
{
    //    qDebug() << "loading" << sender();
    if (sender() == mainGUI->ui->setupProjectResetToolButton || sender() == 0) {
        /*
        ui->setupMainLineEdit->clear();
        ui->setupBiasLineEdit->clear();
        ui->setupDarkLineEdit->clear();
        ui->setupFlatLineEdit->clear();
        ui->setupFlatoffLineEdit->clear();
        ui->setupScienceLineEdit->clear();
        ui->setupSkyLineEdit->clear();
        ui->setupStandardLineEdit->clear();
    */
        // TODO: why do we need this again?
        for (auto &it: mainGUI->status.listCheckBox) {
            it->setChecked(false);
        }

        mainGUI->ui->plainTextEdit->clear();
    }

    // TODO: use default map!
    if (sender() == mainGUI->ui->setupProjectResetToolButton || sender() == 0 ||
            (sender() == ui->parametersDefaultPushButton && ui->confStackedWidget->currentIndex() == 0)) {
        ui->xtalk_nor_2x2ToolButton->setChecked(true);
        ui->xtalk_row_2x2ToolButton->setChecked(true);
        ui->normalxtalkCheckBox->setChecked(false);
        ui->rowxtalkCheckBox->setChecked(false);
        ui->normalxtalkAmplitudeLineEdit->setText("");
        ui->rowxtalkAmplitudeLineEdit->setText("");
        ui->overscanCheckBox->setChecked(true);
        ui->theliRenamingCheckBox->setChecked(true);
        ui->nonlinearityCheckBox->setChecked(false);
        ui->nonlinearityCheckBox->setChecked(false);
        ui->splitMIRcubeCheckBox->setChecked(false);
        ui->biasNlowLineEdit->setText("");
        ui->biasNhighLineEdit->setText("");
        ui->darkNlowLineEdit->setText("");
        ui->darkNhighLineEdit->setText("");
        ui->excludeDetectorsLineEdit->clear();
        ui->flatoffNlowLineEdit->setText("");
        ui->flatoffNhighLineEdit->setText("");
        ui->flatNlowLineEdit->setText("");
        ui->flatNhighLineEdit->setText("");
        ui->biasMinLineEdit->clear();
        ui->biasMaxLineEdit->clear();
        ui->darkMinLineEdit->clear();
        ui->darkMaxLineEdit->clear();
        ui->flatoffMinLineEdit->clear();
        ui->flatoffMaxLineEdit->clear();
        ui->flatMinLineEdit->clear();
        ui->flatMaxLineEdit->clear();
    }

    if (sender() == mainGUI->ui->setupProjectResetToolButton || sender() == 0 ||
            (sender() == ui->parametersDefaultPushButton && ui->confStackedWidget->currentIndex() == 1)) {
        //        qDebug() << "background";
        ui->SPSnumbergroupsLineEdit->clear();
        ui->SPSlengthLineEdit->clear();
        ui->chopnodComboBox->setCurrentIndex(0);
        ui->chopnodInvertCheckBox->setChecked(false);
        ui->BAC1nhighLineEdit->setText("0");
        ui->BAC1nlowLineEdit->setText("0");
        ui->BAC2nhighLineEdit->setText("0");
        ui->BAC2nlowLineEdit->setText("0");
        ui->BAC2passCheckBox->setChecked(true);
        ui->BACDMINLineEdit->setText("5");
        ui->BACDTLineEdit->setText("1.5");
        ui->BACapplyComboBox->setCurrentIndex(0);
        ui->BACbacksmoothscaleLineEdit->clear();
        ui->BACconvolutionCheckBox->setChecked(false);
        ui->BACdistLineEdit->clear();
        ui->BACgapsizeLineEdit->clear();
        ui->BACmagLineEdit->clear();
        ui->BACmefLineEdit->clear();
        ui->BACmethodComboBox->setCurrentIndex(0);
        ui->BACrescaleCheckBox->setChecked(true);
        ui->BACwindowLineEdit->setText("0");
        ui->BACminWindowLineEdit->clear();
    }

    if (sender() == mainGUI->ui->setupProjectResetToolButton || sender() == 0 ||
            (sender() == ui->parametersDefaultPushButton && ui->confStackedWidget->currentIndex() == 2)) {
        ui->COCDMINLineEdit->setText("5");
        ui->COCDTLineEdit->setText("1.5");
        ui->COCdirectionComboBox->setCurrentIndex(0);
        ui->COCmefLineEdit->clear();
        ui->COCrejectLineEdit->setText("1.5");
        ui->COCxmaxLineEdit->clear();
        ui->COCxminLineEdit->clear();
        ui->COCymaxLineEdit->clear();
        ui->COCyminLineEdit->clear();
        ui->BIPSpinBox->setValue(1);
        ui->BIPrejectCheckBox->setChecked(false);
    }

    if (sender() == mainGUI->ui->setupProjectResetToolButton || sender() == 0 ||
            (sender() == ui->parametersDefaultPushButton && ui->confStackedWidget->currentIndex() == 3)) {
        ui->CGWbackclustolLineEdit->clear();
        ui->CGWbackcoltolLineEdit->clear();
        ui->CGWbackrowtolLineEdit->clear();
        ui->CGWbacksmoothLineEdit->clear();
        ui->CGWbackmaxLineEdit->clear();
        ui->CGWbackminLineEdit->clear();
        ui->CGWdarkmaxLineEdit->clear();
        ui->CGWdarkminLineEdit->clear();
        ui->CGWflatclustolLineEdit->clear();
        ui->CGWflatcoltolLineEdit->clear();
        ui->CGWflatmaxLineEdit->setText("1.5");
        ui->CGWflatminLineEdit->setText("0.5");
        ui->CGWflatrowtolLineEdit->clear();
        ui->CGWflatsmoothLineEdit->clear();
        ui->CGWsameweightCheckBox->setChecked(false);
        ui->CIWbloomRangeLineEdit->clear();
        ui->CIWbloomMinaduLineEdit->clear();
        ui->CIWaggressivenessLineEdit->clear();
        ui->CIWmaskbloomingCheckBox->setChecked(false);
        ui->CIWmaxaduLineEdit->clear();
        ui->CIWminaduLineEdit->clear();
        ui->separateTargetLineEdit->setText("");
    }

    if (sender() == mainGUI->ui->setupProjectResetToolButton || sender() == 0 ||
            (sender() == ui->parametersDefaultPushButton && ui->confStackedWidget->currentIndex() == 4)) {
        load_default_stdcat();
        if (instrument_type == "NIR") {
            ui->APIrefcatComboBox->setCurrentText("MKO_JHK");
        }
        else {
            ui->APIrefcatComboBox->setCurrentText("SMITH_u'g'r'i'z'");
        }
        ui->APIWCSCheckBox->setChecked(true);
        ui->APIWCSLineEdit->setText("10");
        ui->APIzpmaxLineEdit->clear();
        ui->APIzpminLineEdit->clear();
        ui->APIapertureLineEdit->setText("10");
        ui->APIcalibrationmodeComboBox->setCurrentIndex(0);
        ui->APIcolorComboBox->setCurrentIndex(0);
        ui->APIcolortermLineEdit->setText("0.0");
        ui->APIconvergenceLineEdit->setText("0.01");
        ui->APIextinctionLineEdit->setText("-0.1");
        ui->APIfilterComboBox->setCurrentIndex(0);
        ui->APIfilterkeywordLineEdit->clear();
        ui->APIkappaLineEdit->setText("2.0");
        ui->APIminmagLineEdit->clear();
        ui->APIthresholdLineEdit->setText("0.15");
        ui->APInight1ComboBox->setCurrentIndex(0);
        ui->APInight1PushButton->setChecked(false);
        ui->APInight2ComboBox->setCurrentIndex(0);
        ui->APInight2PushButton->setChecked(false);
        ui->APInight3ComboBox->setCurrentIndex(0);
        ui->APInight3PushButton->setChecked(false);
        ui->APInight4ComboBox->setCurrentIndex(0);
        ui->APInight4PushButton->setChecked(false);
        ui->APInight5ComboBox->setCurrentIndex(0);
        ui->APInight5PushButton->setChecked(false);
        ui->APInight6ComboBox->setCurrentIndex(0);
        ui->APInight6PushButton->setChecked(false);
        ui->APInight7ComboBox->setCurrentIndex(0);
        ui->APInight7PushButton->setChecked(false);
        ui->APInight8ComboBox->setCurrentIndex(0);
        ui->APInight8PushButton->setChecked(false);
        ui->APInight9ComboBox->setCurrentIndex(0);
        ui->APInight9PushButton->setChecked(false);
    }

    if (sender() == mainGUI->ui->setupProjectResetToolButton || sender() == 0 ||
            (sender() == ui->parametersDefaultPushButton && ui->confStackedWidget->currentIndex() == 5)) {
        load_default_stdcat();
        ui->ARCcatalogComboBox->setCurrentIndex(0);
        ui->ARCdecLineEdit->clear();
        ui->ARCminmagLineEdit->clear();
        ui->ARCmaxpmLineEdit->clear();
        ui->ARCDTLineEdit->setText("5");
        ui->ARCDMINLineEdit->setText("5");
        ui->ARCraLineEdit->clear();
        ui->ARCradiusLineEdit->clear();
        ui->ARCselectimageLineEdit->clear();
        ui->ARCtargetresolverLineEdit->clear();
        ui->ARCmaxpmLineEdit->clear();
        ui->ARCpmRALineEdit->clear();
        ui->ARCpmDECLineEdit->clear();
        ui->CSCDMINLineEdit->setText("5");
        ui->CSCDTLineEdit->setText("5");
        ui->CSCFWHMLineEdit->setText("1.5");
        ui->CSCbackgroundLineEdit->clear();
        ui->CSCmaxflagLineEdit->setText("8");
        ui->CSCmincontLineEdit->setText("0.0005");
        ui->CSCrejectExposureLineEdit->clear();
        ui->CSCsamplingCheckBox->setChecked(false);
        ui->CSCconvolutionCheckBox->setChecked(true);
        ui->CSCsaturationLineEdit->clear();
    }

    if (sender() == mainGUI->ui->setupProjectResetToolButton
            || sender() == 0
            || (sender() == ui->parametersDefaultPushButton && ui->confStackedWidget->currentIndex() == 6)) {
        if (instrument_type == "NIR") {
            ui->APDrefcatComboBox->setCurrentText("SDSS");
        }
        else {
            ui->APDrefcatComboBox->setCurrentText("2MASS");
        }
        ui->APDfilterComboBox->setCurrentIndex(0);
        ui->APDfilterComboBox->setCurrentIndex(0);
        ui->APDmaxphoterrorLineEdit->setText("0.05");
        ui->ASTastrefweightLineEdit->setText("1.0");
        ui->ASTastrinstrukeyLineEdit->setText("FILTER");
        ui->ASTcrossidLineEdit->setText("1.0");
        ui->ASTdistortLineEdit->setText("3");
        ui->ASTdistortgroupsLineEdit->clear();
        ui->ASTdistortkeysLineEdit->clear();
        ui->ASTmatchMethodComboBox->setCurrentIndex(0);
        ui->ASTmatchflippedCheckBox->setChecked(false);
        ui->ASTmethodComboBox->setCurrentIndex(0);
        if (instrument_nchips == 1) {
            ui->ASTmosaictypeComboBox->setCurrentIndex(0);
            ui->ASTfocalplaneComboBox->setCurrentIndex(0);
        }
        else {
            ui->ASTmosaictypeComboBox->setCurrentIndex(3);
            ui->ASTfocalplaneComboBox->setCurrentIndex(0);
        }
        ui->ASTphotinstrukeyLineEdit->setText("FILTER");
        ui->ASTpixscaleLineEdit->setText("1.05");
        ui->ASTposangleLineEdit->setText("2");
        ui->ASTpositionLineEdit->setText("2.0");
        ui->ASTresolutionLineEdit->setText("800");
        ui->ASTsnthreshLineEdit->setText("5,20");
        ui->ASTstabilityComboBox->setCurrentIndex(0);
    }

    if (sender() == mainGUI->ui->setupProjectResetToolButton || sender() == 0 ||
            (sender() == ui->parametersDefaultPushButton && ui->confStackedWidget->currentIndex() == 7)) {
        ui->skyAreaComboBox->setCurrentIndex(0);
        ui->skyConstsubRadioButton->setChecked(false);
        ui->skyDMINLineEdit->setText("10");
        ui->skyDTLineEdit->setText("1.5");
        ui->skyKernelLineEdit->setText("256");
        ui->skyMefLineEdit->clear();
        ui->skyModelRadioButton->setChecked(true);
        ui->skyPolynomialRadioButton->setChecked(false);
        ui->skyPolynomialSpinBox->setValue(1);
        ui->skySavemodelCheckBox->setChecked(false);
    }

    if (sender() == mainGUI->ui->setupProjectResetToolButton || sender() == 0 ||
            (sender() == ui->parametersDefaultPushButton && ui->confStackedWidget->currentIndex() == 8)) {
        ui->COAcelestialtypeComboBox->setCurrentIndex(0);
        ui->COAchipsLineEdit->clear();
        ui->COAcombinetypeComboBox->setCurrentIndex(0);
        ui->COAdecLineEdit->clear();
        ui->COAedgesmoothingLineEdit->clear();
        ui->COAfluxcalibCheckBox->setChecked(false);
        ui->COAkernelComboBox->setCurrentIndex(3);
        ui->COAoutsizeLineEdit->setText("4");
        ui->COAoutthreshLineEdit->setText("3");
        ui->COAoutborderLineEdit->clear();
        ui->COApixscaleLineEdit->setText(get_fileparameter(&instrument_file, "PIXSCALE"));
        ui->COApmComboBox->setCurrentIndex(0);
        ui->COApmdecLineEdit->clear();
        ui->COApmraLineEdit->clear();
        ui->COAprojectionComboBox->setCurrentIndex(0);
        ui->COAraLineEdit->clear();
        ui->COArescaleweightsCheckBox->setChecked(false);
        ui->COAsizexLineEdit->clear();
        ui->COAsizeyLineEdit->clear();
        ui->COAskypaLineEdit->clear();
        ui->COAuniqueidLineEdit->clear();
    }
}
