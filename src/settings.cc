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

#include <QSettings>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_confdockwidget.h"

int MainWindow::writePreferenceSettings()
{
    QSettings settings("THELI", "PREFERENCES");
    settings.setValue("setupProjectLineEdit", ui->setupProjectLineEdit->text());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.sync();
    return settings.status();
}

int MainWindow::readPreferenceSettings(QString &projectname)
{
    QSettings settings("THELI", "PREFERENCES");
    projectname = settings.value("setupProjectLineEdit","").toString();
    diskwarnPreference = settings.value("prefDiskspacewarnSpinBox",100).toInt();
    switchProcessMonitorPreference = settings.value("prefSwitchProcessMonitorCheckBox", true).toBool();
    numCPU = settings.value("prefCPUSpinBox", QThread::idealThreadCount()).toInt();
    restoreGeometry(settings.value("myWidget/geometry").toByteArray());
    restoreState(settings.value("myWidget/windowState").toByteArray());
    return settings.status();
}

QString MainWindow::getStatusForSettings()
{
    QString statusString = "";
    for (auto &it : status.listHistory) {
        if (it) statusString.append("1");
        else statusString.append("0");
    }
    return statusString;
}

void MainWindow::setStatusFromSettings(QString statusString)
{
    // Enforce that the status string is no longer than the maximum string
    // (this is in case I change the number of task checkboxes at some point)
    statusString.truncate(19);
    QStringList list = statusString.split("");
    // The call above results in empty strings at the first and last place of the list.
    // We need to remove them:
    list.removeFirst();
    list.removeLast();
    int i = 0;
    for (auto &it : list) {
        if (it == "0") status.listHistory.operator[](i) = false;
        else status.listHistory.operator[](i) = true;
        ++i;
    }
    status.updateStatus();
}

int MainWindow::writeGUISettings()
{
    // We have to suppress write GUI settings while GUI settings are loaded.
    // For example, this would be triggered by the updates of several setupDataDir LineEdits
    // below. A writeGUI() would e.g. overwrite the instrument in the settings file before
    // it can be read.

    if (preventLoop_WriteSettings) return QSettings::NoError;

    // Likewise, settings must not be written anytime during init
    if (doingInitialLaunch) return QSettings::NoError;

    // I don't like empty chars at the beginning. Not that they hurt so far, but nonetheless
    for (auto &it: status.listDataDirs) {
        QString text = it->text();
        if (!text.isEmpty() && text.operator[](0) == ' ') text.remove(0,1);
        it->setText(text);
    }
    QSettings settings("THELI", ui->setupProjectLineEdit->text());
    settings.beginGroup("MainWindow");
    QString statusString = getStatusForSettings();
    settings.setValue("statusString", statusString);

    // CONF DOCK WIDGET
    settings.setValue("APDfilterComboBox", cdw->ui->APDfilterComboBox->currentIndex());
    settings.setValue("APDmaxphoterrorLineEdit", cdw->ui->APDmaxphoterrorLineEdit->text());
    settings.setValue("APDrefcatComboBox", cdw->ui->APDrefcatComboBox->currentIndex());
    settings.setValue("APIWCSCheckBox", cdw->ui->APIWCSCheckBox->isChecked());
    settings.setValue("APIWCSLineEdit", cdw->ui->APIWCSLineEdit->text());
    settings.setValue("APIapertureLineEdit", cdw->ui->APIapertureLineEdit->text());
    settings.setValue("APIcalibrationmodeComboBox", cdw->ui->APIcalibrationmodeComboBox->currentIndex());
    settings.setValue("APIcolorComboBox", cdw->ui->APIcolorComboBox->currentIndex());
    settings.setValue("APIcolortermLineEdit", cdw->ui->APIcolortermLineEdit->text());
    settings.setValue("APIconvergenceLineEdit", cdw->ui->APIconvergenceLineEdit->text());
    settings.setValue("APIextinctionLineEdit", cdw->ui->APIextinctionLineEdit->text());
    settings.setValue("APIfilterComboBox", cdw->ui->APIfilterComboBox->currentIndex());
    settings.setValue("APIfilterkeywordLineEdit", cdw->ui->APIfilterkeywordLineEdit->text());
    settings.setValue("APIkappaLineEdit", cdw->ui->APIkappaLineEdit->text());
    settings.setValue("APIminmagLineEdit", cdw->ui->APIminmagLineEdit->text());
    settings.setValue("APInight1ComboBox", cdw->ui->APInight1ComboBox->currentIndex());
    settings.setValue("APInight2ComboBox", cdw->ui->APInight2ComboBox->currentIndex());
    settings.setValue("APInight3ComboBox", cdw->ui->APInight3ComboBox->currentIndex());
    settings.setValue("APInight4ComboBox", cdw->ui->APInight4ComboBox->currentIndex());
    settings.setValue("APInight5ComboBox", cdw->ui->APInight5ComboBox->currentIndex());
    settings.setValue("APInight6ComboBox", cdw->ui->APInight6ComboBox->currentIndex());
    settings.setValue("APInight7ComboBox", cdw->ui->APInight7ComboBox->currentIndex());
    settings.setValue("APInight8ComboBox", cdw->ui->APInight8ComboBox->currentIndex());
    settings.setValue("APInight9ComboBox", cdw->ui->APInight9ComboBox->currentIndex());
    settings.setValue("APIrefcatComboBox", cdw->ui->APIrefcatComboBox->currentIndex());
    settings.setValue("APIthresholdLineEdit", cdw->ui->APIthresholdLineEdit->text());
    settings.setValue("APIzpmaxLineEdit", cdw->ui->APIzpmaxLineEdit->text());
    settings.setValue("APIzpminLineEdit", cdw->ui->APIzpminLineEdit->text());
    settings.setValue("ARCDMINLineEdit", cdw->ui->ARCDMINLineEdit->text());
    settings.setValue("ARCDTLineEdit", cdw->ui->ARCDTLineEdit->text());
    settings.setValue("ARCcatalogComboBox", cdw->ui->ARCcatalogComboBox->currentIndex());
    settings.setValue("ARCdecLineEdit", cdw->ui->ARCdecLineEdit->text());
    settings.setValue("ARCimageRadioButton", cdw->ui->ARCimageRadioButton->isChecked());
    settings.setValue("ARCmaxpmLineEdit", cdw->ui->ARCmaxpmLineEdit->text());
    settings.setValue("ARCpmRALineEdit", cdw->ui->ARCpmRALineEdit->text());
    settings.setValue("ARCpmDECLineEdit", cdw->ui->ARCpmDECLineEdit->text());
    settings.setValue("ARCminmagLineEdit", cdw->ui->ARCminmagLineEdit->text());
    settings.setValue("ARCraLineEdit", cdw->ui->ARCraLineEdit->text());
    settings.setValue("ARCradiusLineEdit", cdw->ui->ARCradiusLineEdit->text());
    settings.setValue("ARCselectimageLineEdit", cdw->ui->ARCselectimageLineEdit->text());
    settings.setValue("ARCtargetresolverLineEdit", cdw->ui->ARCtargetresolverLineEdit->text());
    settings.setValue("ARCwebRadioButton", cdw->ui->ARCwebRadioButton->isChecked());
    settings.setValue("ASTastrefweightLineEdit", cdw->ui->ASTastrefweightLineEdit->text());
    settings.setValue("ASTastrinstrukeyLineEdit", cdw->ui->ASTastrinstrukeyLineEdit->text());
    settings.setValue("ASTcrossidLineEdit", cdw->ui->ASTcrossidLineEdit->text());
    settings.setValue("ASTdistortLineEdit", cdw->ui->ASTdistortLineEdit->text());
    settings.setValue("ASTdistortgroupsLineEdit", cdw->ui->ASTdistortgroupsLineEdit->text());
    settings.setValue("ASTdistortkeysLineEdit", cdw->ui->ASTdistortkeysLineEdit->text());
    settings.setValue("ASTfocalplaneComboBox", cdw->ui->ASTfocalplaneComboBox->currentIndex());
    settings.setValue("ASTmatchMethodComboBox", cdw->ui->ASTmatchMethodComboBox->currentIndex());
    settings.setValue("ASTmatchflippedCheckBox", cdw->ui->ASTmatchflippedCheckBox->isChecked());
    settings.setValue("ASTmethodComboBox", cdw->ui->ASTmethodComboBox->currentIndex());
    settings.setValue("ASTmosaictypeComboBox", cdw->ui->ASTmosaictypeComboBox->currentIndex());
    settings.setValue("ASTphotinstrukeyLineEdit", cdw->ui->ASTphotinstrukeyLineEdit->text());
    settings.setValue("ASTpixscaleLineEdit", cdw->ui->ASTpixscaleLineEdit->text());
    settings.setValue("ASTposangleLineEdit", cdw->ui->ASTposangleLineEdit->text());
    settings.setValue("ASTpositionLineEdit", cdw->ui->ASTpositionLineEdit->text());
    settings.setValue("ASTresolutionLineEdit", cdw->ui->ASTresolutionLineEdit->text());
    settings.setValue("ASTsnthreshLineEdit", cdw->ui->ASTsnthreshLineEdit->text());
    settings.setValue("ASTstabilityComboBox", cdw->ui->ASTstabilityComboBox->currentIndex());
    settings.setValue("ASTxcorrDTLineEdit", cdw->ui->ASTxcorrDTLineEdit->text());
    settings.setValue("ASTxcorrDMINLineEdit", cdw->ui->ASTxcorrDMINLineEdit->text());
    settings.setValue("BAC1nhighLineEdit", cdw->ui->BAC1nhighLineEdit->text());
    settings.setValue("BAC1nlowLineEdit", cdw->ui->BAC1nlowLineEdit->text());
    settings.setValue("BAC2nhighLineEdit", cdw->ui->BAC2nhighLineEdit->text());
    settings.setValue("BAC2nlowLineEdit", cdw->ui->BAC2nlowLineEdit->text());
    settings.setValue("BAC2passCheckBox", cdw->ui->BAC2passCheckBox->isChecked());
    settings.setValue("BACDMINLineEdit", cdw->ui->BACDMINLineEdit->text());
    settings.setValue("BACDTLineEdit", cdw->ui->BACDTLineEdit->text());
    settings.setValue("BACapplyComboBox", cdw->ui->BACapplyComboBox->currentIndex());
    settings.setValue("BACbacksmoothscaleLineEdit", cdw->ui->BACbacksmoothscaleLineEdit->text());
    settings.setValue("BACconvolutionCheckBox", cdw->ui->BACconvolutionCheckBox->isChecked());
    settings.setValue("BACdistLineEdit", cdw->ui->BACdistLineEdit->text());
    settings.setValue("BACgapsizeLineEdit", cdw->ui->BACgapsizeLineEdit->text());
    settings.setValue("BACmagLineEdit", cdw->ui->BACmagLineEdit->text());
    settings.setValue("BACmefLineEdit", cdw->ui->BACmefLineEdit->text());
    settings.setValue("BACmethodComboBox", cdw->ui->BACmethodComboBox->currentIndex());
    settings.setValue("BACrescaleCheckBox", cdw->ui->BACrescaleCheckBox->isChecked());
    settings.setValue("BACwindowLineEdit", cdw->ui->BACwindowLineEdit->text());
    settings.setValue("BACminWindowLineEdit", cdw->ui->BACminWindowLineEdit->text());
    settings.setValue("BIPSpinBox", cdw->ui->BIPSpinBox->value());
    settings.setValue("BIPrejectCheckBox", cdw->ui->BIPrejectCheckBox->isChecked());
    settings.setValue("CGWbackclustolLineEdit", cdw->ui->CGWbackclustolLineEdit->text());
    settings.setValue("CGWbackcoltolLineEdit", cdw->ui->CGWbackcoltolLineEdit->text());
    settings.setValue("CGWbackmaxLineEdit", cdw->ui->CGWbackmaxLineEdit->text());
    settings.setValue("CGWbackminLineEdit", cdw->ui->CGWbackminLineEdit->text());
    settings.setValue("CGWbackrowtolLineEdit", cdw->ui->CGWbackrowtolLineEdit->text());
    settings.setValue("CGWbacksmoothLineEdit", cdw->ui->CGWbacksmoothLineEdit->text());
    settings.setValue("CGWdarkmaxLineEdit", cdw->ui->CGWdarkmaxLineEdit->text());
    settings.setValue("CGWdarkminLineEdit", cdw->ui->CGWdarkminLineEdit->text());
    settings.setValue("CGWflatclustolLineEdit", cdw->ui->CGWflatclustolLineEdit->text());
    settings.setValue("CGWflatcoltolLineEdit", cdw->ui->CGWflatcoltolLineEdit->text());
    settings.setValue("CGWflatmaxLineEdit", cdw->ui->CGWflatmaxLineEdit->text());
    settings.setValue("CGWflatminLineEdit", cdw->ui->CGWflatminLineEdit->text());
    settings.setValue("CGWflatrowtolLineEdit", cdw->ui->CGWflatrowtolLineEdit->text());
    settings.setValue("CGWflatsmoothLineEdit", cdw->ui->CGWflatsmoothLineEdit->text());
    settings.setValue("CGWsameweightCheckBox", cdw->ui->CGWsameweightCheckBox->isChecked());
    settings.setValue("CIWsaturationLineEdit", cdw->ui->CIWsaturationLineEdit->text());
    settings.setValue("CIWbloomRangeLineEdit", cdw->ui->CIWbloomRangeLineEdit->text());
    settings.setValue("CIWbloomMinaduLineEdit", cdw->ui->CIWbloomMinaduLineEdit->text());
    settings.setValue("CIWaggressivenessLineEdit", cdw->ui->CIWaggressivenessLineEdit->text());
    settings.setValue("CIWmaskbloomingCheckBox", cdw->ui->CIWmaskbloomingCheckBox->isChecked());
    settings.setValue("CIWmasksaturationCheckBox", cdw->ui->CIWmasksaturationCheckBox->isChecked());
    settings.setValue("CIWmaxaduLineEdit", cdw->ui->CIWmaxaduLineEdit->text());
    settings.setValue("CIWminaduLineEdit", cdw->ui->CIWminaduLineEdit->text());
    settings.setValue("COAcelestialtypeComboBox", cdw->ui->COAcelestialtypeComboBox->currentIndex());
    settings.setValue("COAchipsLineEdit", cdw->ui->COAchipsLineEdit->text());
    settings.setValue("COAcombinetypeComboBox", cdw->ui->COAcombinetypeComboBox->currentIndex());
    settings.setValue("COAdecLineEdit", cdw->ui->COAdecLineEdit->text());
    settings.setValue("COAedgesmoothingLineEdit", cdw->ui->COAedgesmoothingLineEdit->text());
    settings.setValue("COAfluxcalibCheckBox", cdw->ui->COAfluxcalibCheckBox->isChecked());
    settings.setValue("COAkernelComboBox", cdw->ui->COAkernelComboBox->currentIndex());
    settings.setValue("COAoutborderLineEdit", cdw->ui->COAoutborderLineEdit->text());
    settings.setValue("COAoutsizeLineEdit", cdw->ui->COAoutsizeLineEdit->text());
    settings.setValue("COAoutthreshLineEdit", cdw->ui->COAoutthreshLineEdit->text());
    settings.setValue("COApixscaleLineEdit", cdw->ui->COApixscaleLineEdit->text());
    settings.setValue("COApmComboBox", cdw->ui->COApmComboBox->currentIndex());
    settings.setValue("COApmdecLineEdit", cdw->ui->COApmdecLineEdit->text());
    settings.setValue("COApmraLineEdit", cdw->ui->COApmraLineEdit->text());
    settings.setValue("COAprojectionComboBox", cdw->ui->COAprojectionComboBox->currentIndex());
    settings.setValue("COAraLineEdit", cdw->ui->COAraLineEdit->text());
    settings.setValue("COArescaleweightsCheckBox", cdw->ui->COArescaleweightsCheckBox->isChecked());
    settings.setValue("COAsizexLineEdit", cdw->ui->COAsizexLineEdit->text());
    settings.setValue("COAsizeyLineEdit", cdw->ui->COAsizeyLineEdit->text());
    settings.setValue("COAskypaLineEdit", cdw->ui->COAskypaLineEdit->text());
    settings.setValue("COAuniqueidLineEdit", cdw->ui->COAuniqueidLineEdit->text());
    settings.setValue("COCDMINLineEdit", cdw->ui->COCDMINLineEdit->text());
    settings.setValue("COCDTLineEdit", cdw->ui->COCDTLineEdit->text());
    settings.setValue("COCdirectionComboBox", cdw->ui->COCdirectionComboBox->currentIndex());
    settings.setValue("COCmefLineEdit", cdw->ui->COCmefLineEdit->text());
    settings.setValue("COCrejectLineEdit", cdw->ui->COCrejectLineEdit->text());
    settings.setValue("COCxmaxLineEdit", cdw->ui->COCxmaxLineEdit->text());
    settings.setValue("COCxminLineEdit", cdw->ui->COCxminLineEdit->text());
    settings.setValue("COCymaxLineEdit", cdw->ui->COCymaxLineEdit->text());
    settings.setValue("COCyminLineEdit", cdw->ui->COCyminLineEdit->text());
    settings.setValue("CSCDMINLineEdit", cdw->ui->CSCDMINLineEdit->text());
    settings.setValue("CSCDTLineEdit", cdw->ui->CSCDTLineEdit->text());
    settings.setValue("CSCFWHMLineEdit", cdw->ui->CSCFWHMLineEdit->text());
    settings.setValue("CSCbackgroundLineEdit", cdw->ui->CSCbackgroundLineEdit->text());
    settings.setValue("CSCmaxflagLineEdit", cdw->ui->CSCmaxflagLineEdit->text());
    settings.setValue("CSCmaxellLineEdit", cdw->ui->CSCmaxellLineEdit->text());
    settings.setValue("CSCmincontLineEdit", cdw->ui->CSCmincontLineEdit->text());
    settings.setValue("CSCrejectExposureLineEdit", cdw->ui->CSCrejectExposureLineEdit->text());
    settings.setValue("CSCconvolutionCheckBox", cdw->ui->CSCconvolutionCheckBox->isChecked());
    settings.setValue("CSCsamplingCheckBox", cdw->ui->CSCsamplingCheckBox->isChecked());
    settings.setValue("CSCsaturationLineEdit", cdw->ui->CSCsaturationLineEdit->text());
    settings.setValue("CSCMethodComboBox", cdw->ui->CSCMethodComboBox->currentIndex());
    settings.setValue("SPSlengthLineEdit", cdw->ui->SPSlengthLineEdit->text());
    settings.setValue("SPSexcludeLineEdit", cdw->ui->SPSexcludeLineEdit->text());
    settings.setValue("SPSnumbergroupsLineEdit", cdw->ui->SPSnumbergroupsLineEdit->text());
    settings.setValue("biasMaxLineEdit", cdw->ui->biasMaxLineEdit->text());
    settings.setValue("biasMinLineEdit", cdw->ui->biasMinLineEdit->text());
    settings.setValue("biasNhighLineEdit", cdw->ui->biasNhighLineEdit->text());
    settings.setValue("biasNlowLineEdit", cdw->ui->biasNlowLineEdit->text());
    settings.setValue("biasMethodComboBox", cdw->ui->biasMethodComboBox->currentIndex());
    settings.setValue("chopnodComboBox", cdw->ui->chopnodComboBox->currentIndex());
    settings.setValue("chopnodInvertCheckBox", cdw->ui->chopnodInvertCheckBox->isChecked());
    settings.setValue("confStackedWidget", cdw->ui->confStackedWidget->currentIndex());
    settings.setValue("darkMaxLineEdit", cdw->ui->darkMaxLineEdit->text());
    settings.setValue("darkMinLineEdit", cdw->ui->darkMinLineEdit->text());
    settings.setValue("darkNhighLineEdit", cdw->ui->darkNhighLineEdit->text());
    settings.setValue("darkNlowLineEdit", cdw->ui->darkNlowLineEdit->text());
    settings.setValue("darkMethodComboBox", cdw->ui->darkMethodComboBox->currentIndex());
    settings.setValue("excludeDetectorsLineEdit", cdw->ui->excludeDetectorsLineEdit->text());
    settings.setValue("flatMaxLineEdit", cdw->ui->flatMaxLineEdit->text());
    settings.setValue("flatMinLineEdit", cdw->ui->flatMinLineEdit->text());
    settings.setValue("flatNhighLineEdit", cdw->ui->flatNhighLineEdit->text());
    settings.setValue("flatNlowLineEdit", cdw->ui->flatNlowLineEdit->text());
    settings.setValue("flatMethodComboBox", cdw->ui->flatMethodComboBox->currentIndex());
    settings.setValue("flatoffMaxLineEdit", cdw->ui->flatoffMaxLineEdit->text());
    settings.setValue("flatoffMinLineEdit", cdw->ui->flatoffMinLineEdit->text());
    settings.setValue("flatoffNhighLineEdit", cdw->ui->flatoffNhighLineEdit->text());
    settings.setValue("flatoffNlowLineEdit", cdw->ui->flatoffNlowLineEdit->text());
    settings.setValue("flatoffMethodComboBox", cdw->ui->flatoffMethodComboBox->currentIndex());
    settings.setValue("overscanCheckBox", cdw->ui->overscanCheckBox->isChecked());
    settings.setValue("theliRenamingCheckBox", cdw->ui->theliRenamingCheckBox->isChecked());
    settings.setValue("nonlinearityCheckBox", cdw->ui->nonlinearityCheckBox->isChecked());
    settings.setValue("normalxtalkAmplitudeLineEdit", cdw->ui->normalxtalkAmplitudeLineEdit->text());
    settings.setValue("normalxtalkCheckBox", cdw->ui->normalxtalkCheckBox->isChecked());
    settings.setValue("overscanMethodComboBox", cdw->ui->overscanMethodComboBox->currentIndex());
    settings.setValue("rowxtalkAmplitudeLineEdit", cdw->ui->rowxtalkAmplitudeLineEdit->text());
    settings.setValue("rowxtalkCheckBox", cdw->ui->rowxtalkCheckBox->isChecked());
    settings.setValue("saturationLineEdit", cdw->ui->saturationLineEdit->text());
    settings.setValue("separateTargetLineEdit", cdw->ui->separateTargetLineEdit->text());
    settings.setValue("skyAreaComboBox", cdw->ui->skyAreaComboBox->currentIndex());
    settings.setValue("skyConstsubRadioButton", cdw->ui->skyConstsubRadioButton->isChecked());
    settings.setValue("skyDMINLineEdit", cdw->ui->skyDMINLineEdit->text());
    settings.setValue("skyDTLineEdit", cdw->ui->skyDTLineEdit->text());
    settings.setValue("skyKernelLineEdit", cdw->ui->skyKernelLineEdit->text());
    settings.setValue("skyMefLineEdit", cdw->ui->skyMefLineEdit->text());
    settings.setValue("skyModelRadioButton", cdw->ui->skyModelRadioButton->isChecked());
    settings.setValue("skyPolynomialRadioButton", cdw->ui->skyPolynomialRadioButton->isChecked());
    settings.setValue("skyPolynomialSpinBox", cdw->ui->skyPolynomialSpinBox->value());
    settings.setValue("skySavemodelCheckBox", cdw->ui->skySavemodelCheckBox->isChecked());
    settings.setValue("splitMIRcubeCheckBox", cdw->ui->splitMIRcubeCheckBox->isChecked());
    settings.setValue("xtalk_col_1x2ToolButton", cdw->ui->xtalk_col_1x2ToolButton->isChecked());
    settings.setValue("xtalk_col_2x1ToolButton", cdw->ui->xtalk_col_2x1ToolButton->isChecked());
    settings.setValue("xtalk_col_2x2ToolButton", cdw->ui->xtalk_col_2x2ToolButton->isChecked());
    settings.setValue("xtalk_nor_1x2ToolButton", cdw->ui->xtalk_nor_1x2ToolButton->isChecked());
    settings.setValue("xtalk_nor_2x1ToolButton", cdw->ui->xtalk_nor_2x1ToolButton->isChecked());
    settings.setValue("xtalk_nor_2x2ToolButton", cdw->ui->xtalk_nor_2x2ToolButton->isChecked());
    settings.setValue("xtalk_row_1x2ToolButton", cdw->ui->xtalk_row_1x2ToolButton->isChecked());
    settings.setValue("xtalk_row_2x1ToolButton", cdw->ui->xtalk_row_2x1ToolButton->isChecked());
    settings.setValue("xtalk_row_2x2ToolButton", cdw->ui->xtalk_row_2x2ToolButton->isChecked());

    // MAIN WINDOW
    settings.setValue("applyAbsphotindirectCheckBox", ui->applyAbsphotindirectCheckBox->isChecked());
    settings.setValue("applyAstromphotomCheckBox", ui->applyAstromphotomCheckBox->isChecked());
    settings.setValue("applyBackgroundCheckBox", ui->applyBackgroundCheckBox->isChecked());
    settings.setValue("applyBinnedpreviewCheckBox", ui->applyBinnedpreviewCheckBox->isChecked());
    settings.setValue("applyChopnodCheckBox", ui->applyChopnodCheckBox->isChecked());
    settings.setValue("applyCoadditionCheckBox", ui->applyCoadditionCheckBox->isChecked());
    settings.setValue("applyCollapseCheckBox", ui->applyCollapseCheckBox->isChecked());
    settings.setValue("applyCreatesourcecatCheckBox", ui->applyCreatesourcecatCheckBox->isChecked());
    settings.setValue("applyGlobalweightCheckBox", ui->applyGlobalweightCheckBox->isChecked());
    settings.setValue("applyHDUreformatCheckBox", ui->applyHDUreformatCheckBox->isChecked());
    settings.setValue("applyIndividualweightCheckBox", ui->applyIndividualweightCheckBox->isChecked());
    settings.setValue("applyProcessbiasCheckBox", ui->applyProcessbiasCheckBox->isChecked());
    settings.setValue("applyProcessdarkCheckBox", ui->applyProcessdarkCheckBox->isChecked());
    settings.setValue("applyProcessflatCheckBox", ui->applyProcessflatCheckBox->isChecked());
    settings.setValue("applyProcessflatoffCheckBox", ui->applyProcessflatoffCheckBox->isChecked());
    settings.setValue("applyProcessscienceCheckBox", ui->applyProcessscienceCheckBox->isChecked());
    settings.setValue("applySeparateCheckBox", ui->applySeparateCheckBox->isChecked());
    settings.setValue("applySkysubCheckBox", ui->applySkysubCheckBox->isChecked());
    settings.setValue("applyStarflatCheckBox", ui->applyStarflatCheckBox->isChecked());
    settings.setValue("processingTabWidget", ui->processingTabWidget->currentIndex());
    settings.setValue("setupBiasLineEdit", ui->setupBiasLineEdit->text());
    settings.setValue("setupDarkLineEdit", ui->setupDarkLineEdit->text());
    settings.setValue("setupFlatLineEdit", ui->setupFlatLineEdit->text());
    settings.setValue("setupFlatoffLineEdit", ui->setupFlatoffLineEdit->text());
    settings.setValue("setupInstrumentComboBox", ui->setupInstrumentComboBox->currentText());
    settings.setValue("setupMainLineEdit", ui->setupMainLineEdit->text());
    settings.setValue("setupProjectLineEdit", ui->setupProjectLineEdit->text());
    settings.setValue("setupScienceLineEdit", ui->setupScienceLineEdit->text());
    settings.setValue("setupSkyLineEdit", ui->setupSkyLineEdit->text());
    settings.setValue("setupStandardLineEdit", ui->setupStandardLineEdit->text());
    settings.endGroup();
    settings.sync();
    return settings.status();
}
 
int MainWindow::readGUISettings(QString projectname)
{
    preventLoop_WriteSettings = true;
    if (projectname.isEmpty()) {
        projectname = "MyProject";
        ui->setupProjectLineEdit->setText(projectname);
        cdw->loadDefaults();
        fill_setupInstrumentComboBox();
        preventLoop_WriteSettings = false;
        return 3; // not checked
    }

    QSettings settings("THELI", projectname);
    settings.beginGroup("MainWindow");

    readingSettings = true;

    setStatusFromSettings(settings.value("statusString").toString());

    // CONF DOCK WIDGET
    cdw->ui->APDfilterComboBox->setCurrentIndex(settings.value("APDfilterComboBox").toInt());
    cdw->ui->APDmaxphoterrorLineEdit->setText(settings.value("APDmaxphoterrorLineEdit").toString());
    cdw->ui->APDrefcatComboBox->setCurrentIndex(settings.value("APDrefcatComboBox").toInt());
    cdw->ui->APIWCSCheckBox->setChecked(settings.value("APIWCSCheckBox").toBool());
    cdw->ui->APIWCSLineEdit->setText(settings.value("APIWCSLineEdit").toString());
    cdw->ui->APIapertureLineEdit->setText(settings.value("APIapertureLineEdit").toString());
    cdw->ui->APIcalibrationmodeComboBox->setCurrentIndex(settings.value("APIcalibrationmodeComboBox").toInt());
    cdw->ui->APIcolorComboBox->setCurrentIndex(settings.value("APIcolorComboBox").toInt());
    cdw->ui->APIcolortermLineEdit->setText(settings.value("APIcolortermLineEdit").toString());
    cdw->ui->APIconvergenceLineEdit->setText(settings.value("APIconvergenceLineEdit").toString());
    cdw->ui->APIextinctionLineEdit->setText(settings.value("APIextinctionLineEdit").toString());
    cdw->ui->APIfilterComboBox->setCurrentIndex(settings.value("APIfilterComboBox").toInt());
    cdw->ui->APIfilterkeywordLineEdit->setText(settings.value("APIfilterkeywordLineEdit").toString());
    cdw->ui->APIkappaLineEdit->setText(settings.value("APIkappaLineEdit").toString());
    cdw->ui->APIminmagLineEdit->setText(settings.value("APIminmagLineEdit").toString());
    cdw->ui->APInight1ComboBox->setCurrentIndex(settings.value("APInight1ComboBox").toInt());
    cdw->ui->APInight2ComboBox->setCurrentIndex(settings.value("APInight2ComboBox").toInt());
    cdw->ui->APInight3ComboBox->setCurrentIndex(settings.value("APInight3ComboBox").toInt());
    cdw->ui->APInight4ComboBox->setCurrentIndex(settings.value("APInight4ComboBox").toInt());
    cdw->ui->APInight5ComboBox->setCurrentIndex(settings.value("APInight5ComboBox").toInt());
    cdw->ui->APInight6ComboBox->setCurrentIndex(settings.value("APInight6ComboBox").toInt());
    cdw->ui->APInight7ComboBox->setCurrentIndex(settings.value("APInight7ComboBox").toInt());
    cdw->ui->APInight8ComboBox->setCurrentIndex(settings.value("APInight8ComboBox").toInt());
    cdw->ui->APInight9ComboBox->setCurrentIndex(settings.value("APInight9ComboBox").toInt());
    cdw->ui->APIrefcatComboBox->setCurrentIndex(settings.value("APIrefcatComboBox").toInt());
    cdw->ui->APIthresholdLineEdit->setText(settings.value("APIthresholdLineEdit").toString());
    cdw->ui->APIzpmaxLineEdit->setText(settings.value("APIzpmaxLineEdit").toString());
    cdw->ui->APIzpminLineEdit->setText(settings.value("APIzpminLineEdit").toString());
    cdw->ui->ARCDMINLineEdit->setText(settings.value("ARCDMINLineEdit").toString());
    cdw->ui->ARCDTLineEdit->setText(settings.value("ARCDTLineEdit").toString());
    cdw->ui->ARCcatalogComboBox->setCurrentIndex(settings.value("ARCcatalogComboBox").toInt());
    cdw->ui->ARCdecLineEdit->setText(settings.value("ARCdecLineEdit").toString());
    cdw->ui->ARCimageRadioButton->setChecked(settings.value("ARCimageRadioButton").toBool());
    cdw->ui->ARCmaxpmLineEdit->setText(settings.value("ARCmaxpmLineEdit").toString());
    cdw->ui->ARCpmRALineEdit->setText(settings.value("ARCpmRALineEdit").toString());
    cdw->ui->ARCpmDECLineEdit->setText(settings.value("ARCpmDECLineEdit").toString());
    cdw->ui->ARCminmagLineEdit->setText(settings.value("ARCminmagLineEdit").toString());
    cdw->ui->ARCraLineEdit->setText(settings.value("ARCraLineEdit").toString());
    cdw->ui->ARCradiusLineEdit->setText(settings.value("ARCradiusLineEdit").toString());
    cdw->ui->ARCselectimageLineEdit->setText(settings.value("ARCselectimageLineEdit").toString());
    cdw->ui->ARCtargetresolverLineEdit->setText(settings.value("ARCtargetresolverLineEdit").toString());
    cdw->ui->ARCwebRadioButton->setChecked(settings.value("ARCwebRadioButton").toBool());
    cdw->ui->ASTastrefweightLineEdit->setText(settings.value("ASTastrefweightLineEdit").toString());
    cdw->ui->ASTastrinstrukeyLineEdit->setText(settings.value("ASTastrinstrukeyLineEdit").toString());
    cdw->ui->ASTcrossidLineEdit->setText(settings.value("ASTcrossidLineEdit").toString());
    cdw->ui->ASTdistortLineEdit->setText(settings.value("ASTdistortLineEdit").toString());
    cdw->ui->ASTdistortgroupsLineEdit->setText(settings.value("ASTdistortgroupsLineEdit").toString());
    cdw->ui->ASTdistortkeysLineEdit->setText(settings.value("ASTdistortkeysLineEdit").toString());
    cdw->ui->ASTfocalplaneComboBox->setCurrentIndex(settings.value("ASTfocalplaneComboBox").toInt());
    cdw->ui->ASTmatchMethodComboBox->setCurrentIndex(settings.value("ASTmatchMethodComboBox").toInt());
    cdw->ui->ASTmatchflippedCheckBox->setChecked(settings.value("ASTmatchflippedCheckBox").toBool());
    cdw->ui->ASTmethodComboBox->setCurrentIndex(settings.value("ASTmethodComboBox").toInt());
    cdw->ui->ASTmosaictypeComboBox->setCurrentIndex(settings.value("ASTmosaictypeComboBox").toInt());
    cdw->ui->ASTphotinstrukeyLineEdit->setText(settings.value("ASTphotinstrukeyLineEdit").toString());
    cdw->ui->ASTpixscaleLineEdit->setText(settings.value("ASTpixscaleLineEdit").toString());
    cdw->ui->ASTposangleLineEdit->setText(settings.value("ASTposangleLineEdit").toString());
    cdw->ui->ASTpositionLineEdit->setText(settings.value("ASTpositionLineEdit").toString());
    cdw->ui->ASTresolutionLineEdit->setText(settings.value("ASTresolutionLineEdit").toString());
    cdw->ui->ASTsnthreshLineEdit->setText(settings.value("ASTsnthreshLineEdit").toString());
    cdw->ui->ASTstabilityComboBox->setCurrentIndex(settings.value("ASTstabilityComboBox").toInt());
    cdw->ui->ASTxcorrDTLineEdit->setText(settings.value("ASTxcorrDTLineEdit").toString());
    cdw->ui->ASTxcorrDMINLineEdit->setText(settings.value("ASTxcorrDMINLineEdit").toString());
    cdw->ui->BAC1nhighLineEdit->setText(settings.value("BAC1nhighLineEdit").toString());
    cdw->ui->BAC1nlowLineEdit->setText(settings.value("BAC1nlowLineEdit").toString());
    cdw->ui->BAC2nhighLineEdit->setText(settings.value("BAC2nhighLineEdit").toString());
    cdw->ui->BAC2nlowLineEdit->setText(settings.value("BAC2nlowLineEdit").toString());
    cdw->ui->BAC2passCheckBox->setChecked(settings.value("BAC2passCheckBox").toBool());
    cdw->ui->BACDMINLineEdit->setText(settings.value("BACDMINLineEdit").toString());
    cdw->ui->BACDTLineEdit->setText(settings.value("BACDTLineEdit").toString());
    cdw->ui->BACapplyComboBox->setCurrentIndex(settings.value("BACapplyComboBox").toInt());
    cdw->ui->BACbacksmoothscaleLineEdit->setText(settings.value("BACbacksmoothscaleLineEdit").toString());
    cdw->ui->BACconvolutionCheckBox->setChecked(settings.value("BACconvolutionCheckBox").toBool());
    cdw->ui->BACdistLineEdit->setText(settings.value("BACdistLineEdit").toString());
    cdw->ui->BACgapsizeLineEdit->setText(settings.value("BACgapsizeLineEdit").toString());
    cdw->ui->BACmagLineEdit->setText(settings.value("BACmagLineEdit").toString());
    cdw->ui->BACmefLineEdit->setText(settings.value("BACmefLineEdit").toString());
    cdw->ui->BACmethodComboBox->setCurrentIndex(settings.value("BACmethodComboBox").toInt());
    cdw->ui->BACrescaleCheckBox->setChecked(settings.value("BACrescaleCheckBox").toBool());
    cdw->ui->BACwindowLineEdit->setText(settings.value("BACwindowLineEdit").toString());
    cdw->ui->BACminWindowLineEdit->setText(settings.value("BACminWindowLineEdit").toString());
    cdw->ui->BIPSpinBox->setValue(settings.value("BIPSpinBox").toInt());
    cdw->ui->BIPrejectCheckBox->setChecked(settings.value("BIPrejectCheckBox").toBool());
    cdw->ui->CGWbackclustolLineEdit->setText(settings.value("CGWbackclustolLineEdit").toString());
    cdw->ui->CGWbackcoltolLineEdit->setText(settings.value("CGWbackcoltolLineEdit").toString());
    cdw->ui->CGWbackmaxLineEdit->setText(settings.value("CGWbackmaxLineEdit").toString());
    cdw->ui->CGWbackminLineEdit->setText(settings.value("CGWbackminLineEdit").toString());
    cdw->ui->CGWbackrowtolLineEdit->setText(settings.value("CGWbackrowtolLineEdit").toString());
    cdw->ui->CGWbacksmoothLineEdit->setText(settings.value("CGWbacksmoothLineEdit").toString());
    cdw->ui->CGWdarkmaxLineEdit->setText(settings.value("CGWdarkmaxLineEdit").toString());
    cdw->ui->CGWdarkminLineEdit->setText(settings.value("CGWdarkminLineEdit").toString());
    cdw->ui->CGWflatclustolLineEdit->setText(settings.value("CGWflatclustolLineEdit").toString());
    cdw->ui->CGWflatcoltolLineEdit->setText(settings.value("CGWflatcoltolLineEdit").toString());
    cdw->ui->CGWflatmaxLineEdit->setText(settings.value("CGWflatmaxLineEdit").toString());
    cdw->ui->CGWflatminLineEdit->setText(settings.value("CGWflatminLineEdit").toString());
    cdw->ui->CGWflatrowtolLineEdit->setText(settings.value("CGWflatrowtolLineEdit").toString());
    cdw->ui->CGWflatsmoothLineEdit->setText(settings.value("CGWflatsmoothLineEdit").toString());
    cdw->ui->CGWsameweightCheckBox->setChecked(settings.value("CGWsameweightCheckBox").toBool());
    cdw->ui->CIWsaturationLineEdit->setText(settings.value("CIWsaturationLineEdit").toString());
    cdw->ui->CIWbloomRangeLineEdit->setText(settings.value("CIWbloomRangeLineEdit").toString());
    cdw->ui->CIWbloomMinaduLineEdit->setText(settings.value("CIWbloomMinaduLineEdit").toString());
    cdw->ui->CIWaggressivenessLineEdit->setText(settings.value("CIWaggressivenessLineEdit").toString());
    cdw->ui->CIWmaskbloomingCheckBox->setChecked(settings.value("CIWmaskbloomingCheckBox").toBool());
    cdw->ui->CIWmasksaturationCheckBox->setChecked(settings.value("CIWmasksaturationCheckBox").toBool());
    cdw->ui->CIWmaxaduLineEdit->setText(settings.value("CIWmaxaduLineEdit").toString());
    cdw->ui->CIWminaduLineEdit->setText(settings.value("CIWminaduLineEdit").toString());
    cdw->ui->COAcelestialtypeComboBox->setCurrentIndex(settings.value("COAcelestialtypeComboBox").toInt());
    cdw->ui->COAchipsLineEdit->setText(settings.value("COAchipsLineEdit").toString());
    cdw->ui->COAcombinetypeComboBox->setCurrentIndex(settings.value("COAcombinetypeComboBox").toInt());
    cdw->ui->COAdecLineEdit->setText(settings.value("COAdecLineEdit").toString());
    cdw->ui->COAedgesmoothingLineEdit->setText(settings.value("COAedgesmoothingLineEdit").toString());
    cdw->ui->COAfluxcalibCheckBox->setChecked(settings.value("COAfluxcalibCheckBox").toBool());
    cdw->ui->COAkernelComboBox->setCurrentIndex(settings.value("COAkernelComboBox").toInt());
    cdw->ui->COAoutborderLineEdit->setText(settings.value("COAoutborderLineEdit").toString());
    cdw->ui->COAoutsizeLineEdit->setText(settings.value("COAoutsizeLineEdit").toString());
    cdw->ui->COAoutthreshLineEdit->setText(settings.value("COAoutthreshLineEdit").toString());
    cdw->ui->COApixscaleLineEdit->setText(settings.value("COApixscaleLineEdit").toString());
    cdw->ui->COApmComboBox->setCurrentIndex(settings.value("COApmComboBox").toInt());
    cdw->ui->COApmdecLineEdit->setText(settings.value("COApmdecLineEdit").toString());
    cdw->ui->COApmraLineEdit->setText(settings.value("COApmraLineEdit").toString());
    cdw->ui->COAprojectionComboBox->setCurrentIndex(settings.value("COAprojectionComboBox").toInt());
    cdw->ui->COAraLineEdit->setText(settings.value("COAraLineEdit").toString());
    cdw->ui->COArescaleweightsCheckBox->setChecked(settings.value("COArescaleweightsCheckBox").toBool());
    cdw->ui->COAsizexLineEdit->setText(settings.value("COAsizexLineEdit").toString());
    cdw->ui->COAsizeyLineEdit->setText(settings.value("COAsizeyLineEdit").toString());
    cdw->ui->COAskypaLineEdit->setText(settings.value("COAskypaLineEdit").toString());
    cdw->ui->COAuniqueidLineEdit->setText(settings.value("COAuniqueidLineEdit").toString());
    cdw->ui->COCDMINLineEdit->setText(settings.value("COCDMINLineEdit").toString());
    cdw->ui->COCDTLineEdit->setText(settings.value("COCDTLineEdit").toString());
    cdw->ui->COCdirectionComboBox->setCurrentIndex(settings.value("COCdirectionComboBox").toInt());
    cdw->ui->COCmefLineEdit->setText(settings.value("COCmefLineEdit").toString());
    cdw->ui->COCrejectLineEdit->setText(settings.value("COCrejectLineEdit").toString());
    cdw->ui->COCxmaxLineEdit->setText(settings.value("COCxmaxLineEdit").toString());
    cdw->ui->COCxminLineEdit->setText(settings.value("COCxminLineEdit").toString());
    cdw->ui->COCymaxLineEdit->setText(settings.value("COCymaxLineEdit").toString());
    cdw->ui->COCyminLineEdit->setText(settings.value("COCyminLineEdit").toString());
    cdw->ui->CSCDMINLineEdit->setText(settings.value("CSCDMINLineEdit").toString());
    cdw->ui->CSCDTLineEdit->setText(settings.value("CSCDTLineEdit").toString());
    cdw->ui->CSCFWHMLineEdit->setText(settings.value("CSCFWHMLineEdit").toString());
    cdw->ui->CSCbackgroundLineEdit->setText(settings.value("CSCbackgroundLineEdit").toString());
    cdw->ui->CSCmaxflagLineEdit->setText(settings.value("CSCmaxflagLineEdit").toString());
    cdw->ui->CSCmaxellLineEdit->setText(settings.value("CSCmaxellLineEdit").toString());
    cdw->ui->CSCMethodComboBox->setCurrentIndex(settings.value("CSCMethodComboBox").toInt());
    cdw->ui->CSCmincontLineEdit->setText(settings.value("CSCmincontLineEdit").toString());
    cdw->ui->CSCrejectExposureLineEdit->setText(settings.value("CSCrejectExposureLineEdit").toString());
    cdw->ui->CSCconvolutionCheckBox->setChecked(settings.value("CSCconvolutionCheckBox").toBool());
    cdw->ui->CSCsamplingCheckBox->setChecked(settings.value("CSCsamplingCheckBox").toBool());
    cdw->ui->CSCsaturationLineEdit->setText(settings.value("CSCsaturationLineEdit").toString());
    cdw->ui->SPSlengthLineEdit->setText(settings.value("SPSlengthLineEdit").toString());
    cdw->ui->SPSnumbergroupsLineEdit->setText(settings.value("SPSnumbergroupsLineEdit").toString());
    cdw->ui->SPSexcludeLineEdit->setText(settings.value("SPSexcludeLineEdit").toString());
    cdw->ui->biasMaxLineEdit->setText(settings.value("biasMaxLineEdit").toString());
    cdw->ui->biasMinLineEdit->setText(settings.value("biasMinLineEdit").toString());
    cdw->ui->biasNhighLineEdit->setText(settings.value("biasNhighLineEdit").toString());
    cdw->ui->biasNlowLineEdit->setText(settings.value("biasNlowLineEdit").toString());
    cdw->ui->biasMethodComboBox->setCurrentIndex(settings.value("biasMethodComboBox").toInt());
    cdw->ui->chopnodComboBox->setCurrentIndex(settings.value("chopnodComboBox").toInt());
    cdw->ui->chopnodInvertCheckBox->setChecked(settings.value("chopnodInvertCheckBox").toBool());
    cdw->ui->darkMaxLineEdit->setText(settings.value("darkMaxLineEdit").toString());
    cdw->ui->darkMinLineEdit->setText(settings.value("darkMinLineEdit").toString());
    cdw->ui->darkNhighLineEdit->setText(settings.value("darkNhighLineEdit").toString());
    cdw->ui->darkNlowLineEdit->setText(settings.value("darkNlowLineEdit").toString());
    cdw->ui->darkMethodComboBox->setCurrentIndex(settings.value("darkMethodComboBox").toInt());
    cdw->ui->excludeDetectorsLineEdit->setText(settings.value("excludeDetectorsLineEdit").toString());
    cdw->ui->flatMaxLineEdit->setText(settings.value("flatMaxLineEdit").toString());
    cdw->ui->flatMinLineEdit->setText(settings.value("flatMinLineEdit").toString());
    cdw->ui->flatNhighLineEdit->setText(settings.value("flatNhighLineEdit").toString());
    cdw->ui->flatNlowLineEdit->setText(settings.value("flatNlowLineEdit").toString());
    cdw->ui->flatMethodComboBox->setCurrentIndex(settings.value("flatMethodComboBox").toInt());
    cdw->ui->flatoffMaxLineEdit->setText(settings.value("flatoffMaxLineEdit").toString());
    cdw->ui->flatoffMinLineEdit->setText(settings.value("flatoffMinLineEdit").toString());
    cdw->ui->flatoffNhighLineEdit->setText(settings.value("flatoffNhighLineEdit").toString());
    cdw->ui->flatoffNlowLineEdit->setText(settings.value("flatoffNlowLineEdit").toString());
    cdw->ui->flatoffMethodComboBox->setCurrentIndex(settings.value("flatoffMethodComboBox").toInt());
    // Instrument-dependent, see below
    // cdw->ui->skyAreaComboBox->setCurrentIndex(settings.value("skyAreaComboBox").toInt());
    cdw->ui->skyConstsubRadioButton->setChecked(settings.value("skyConstsubRadioButton").toBool());
    cdw->ui->skyDMINLineEdit->setText(settings.value("skyDMINLineEdit").toString());
    cdw->ui->skyDTLineEdit->setText(settings.value("skyDTLineEdit").toString());
    cdw->ui->skyKernelLineEdit->setText(settings.value("skyKernelLineEdit").toString());
    cdw->ui->skyMefLineEdit->setText(settings.value("skyMefLineEdit").toString());
    cdw->ui->skyModelRadioButton->setChecked(settings.value("skyModelRadioButton").toBool());
    cdw->ui->skyPolynomialRadioButton->setChecked(settings.value("skyPolynomialRadioButton").toBool());
    cdw->ui->skyPolynomialSpinBox->setValue(settings.value("skyPolynomialSpinBox").toInt());
    cdw->ui->skySavemodelCheckBox->setChecked(settings.value("skySavemodelCheckBox").toBool());
    cdw->ui->theliRenamingCheckBox->setChecked(settings.value("theliRenamingCheckBox").toBool());
    cdw->ui->overscanCheckBox->setChecked(settings.value("overscanCheckBox").toBool());
    cdw->ui->nonlinearityCheckBox->setChecked(settings.value("nonlinearityCheckBox").toBool());
    cdw->ui->normalxtalkAmplitudeLineEdit->setText(settings.value("normalxtalkAmplitudeLineEdit").toString());
    cdw->ui->normalxtalkCheckBox->setChecked(settings.value("normalxtalkCheckBox").toBool());
    cdw->ui->overscanMethodComboBox->setCurrentIndex(settings.value("overscanMethodComboBox").toInt());
    cdw->ui->splitMIRcubeCheckBox->setChecked(settings.value("splitMIRcubeCheckBox").toBool());
    cdw->ui->xtalk_col_1x2ToolButton->setChecked(settings.value("xtalk_col_1x2ToolButton").toBool());
    cdw->ui->xtalk_col_2x1ToolButton->setChecked(settings.value("xtalk_col_2x1ToolButton").toBool());
    cdw->ui->xtalk_col_2x2ToolButton->setChecked(settings.value("xtalk_col_2x2ToolButton").toBool());
    cdw->ui->xtalk_nor_1x2ToolButton->setChecked(settings.value("xtalk_nor_1x2ToolButton").toBool());
    cdw->ui->xtalk_nor_2x1ToolButton->setChecked(settings.value("xtalk_nor_2x1ToolButton").toBool());
    cdw->ui->xtalk_nor_2x2ToolButton->setChecked(settings.value("xtalk_nor_2x2ToolButton").toBool());
    cdw->ui->xtalk_row_1x2ToolButton->setChecked(settings.value("xtalk_row_1x2ToolButton").toBool());
    cdw->ui->xtalk_row_2x1ToolButton->setChecked(settings.value("xtalk_row_2x1ToolButton").toBool());
    cdw->ui->xtalk_row_2x2ToolButton->setChecked(settings.value("xtalk_row_2x2ToolButton").toBool());
    cdw->ui->confStackedWidget->setCurrentIndex(settings.value("confStackedWidget").toInt());
    cdw->ui->rowxtalkAmplitudeLineEdit->setText(settings.value("rowxtalkAmplitudeLineEdit").toString());
    cdw->ui->rowxtalkCheckBox->setChecked(settings.value("rowxtalkCheckBox").toBool());
    cdw->ui->saturationLineEdit->setText(settings.value("saturationLineEdit").toString());
    cdw->ui->separateTargetLineEdit->setText(settings.value("separateTargetLineEdit").toString());

    // MAIN WINDOW
    ui->applyAbsphotindirectCheckBox->setChecked(settings.value("applyAbsphotindirectCheckBox").toBool());
    ui->applyAstromphotomCheckBox->setChecked(settings.value("applyAstromphotomCheckBox").toBool());
    ui->applyBackgroundCheckBox->setChecked(settings.value("applyBackgroundCheckBox").toBool());
    ui->applyBinnedpreviewCheckBox->setChecked(settings.value("applyBinnedpreviewCheckBox").toBool());
    ui->applyChopnodCheckBox->setChecked(settings.value("applyChopnodCheckBox").toBool());
    ui->applyCoadditionCheckBox->setChecked(settings.value("applyCoadditionCheckBox").toBool());
    ui->applyCollapseCheckBox->setChecked(settings.value("applyCollapseCheckBox").toBool());
    ui->applyCreatesourcecatCheckBox->setChecked(settings.value("applyCreatesourcecatCheckBox").toBool());
    ui->applyGlobalweightCheckBox->setChecked(settings.value("applyGlobalweightCheckBox").toBool());
    ui->applyHDUreformatCheckBox->setChecked(settings.value("applyHDUreformatCheckBox").toBool());
    ui->applyIndividualweightCheckBox->setChecked(settings.value("applyIndividualweightCheckBox").toBool());
    ui->applyProcessbiasCheckBox->setChecked(settings.value("applyProcessbiasCheckBox").toBool());
    ui->applyProcessdarkCheckBox->setChecked(settings.value("applyProcessdarkCheckBox").toBool());
    ui->applyProcessflatCheckBox->setChecked(settings.value("applyProcessflatCheckBox").toBool());
    ui->applyProcessflatoffCheckBox->setChecked(settings.value("applyProcessflatoffCheckBox").toBool());
    ui->applyProcessscienceCheckBox->setChecked(settings.value("applyProcessscienceCheckBox").toBool());
    ui->applySeparateCheckBox->setChecked(settings.value("applySeparateCheckBox").toBool());
    ui->applySkysubCheckBox->setChecked(settings.value("applySkysubCheckBox").toBool());
    ui->applyStarflatCheckBox->setChecked(settings.value("applyStarflatCheckBox").toBool());
    ui->processingTabWidget->setCurrentIndex(settings.value("processingTabWidget").toInt());
    fill_setupInstrumentComboBox();   // Must be populated first so that the matching entry can be selected
    ui->setupInstrumentComboBox->setCurrentText(settings.value("setupInstrumentComboBox").toString());
    ui->setupBiasLineEdit->setText(settings.value("setupBiasLineEdit").toString());
    ui->setupDarkLineEdit->setText(settings.value("setupDarkLineEdit").toString());
    ui->setupFlatLineEdit->setText(settings.value("setupFlatLineEdit").toString());
    ui->setupFlatoffLineEdit->setText(settings.value("setupFlatoffLineEdit").toString());
    ui->setupMainLineEdit->setText(settings.value("setupMainLineEdit").toString());
    ui->setupProjectLineEdit->setText(settings.value("setupProjectLineEdit").toString());
    ui->setupScienceLineEdit->setText(settings.value("setupScienceLineEdit").toString());
    ui->setupSkyLineEdit->setText(settings.value("setupSkyLineEdit").toString());
    ui->setupStandardLineEdit->setText(settings.value("setupStandardLineEdit").toString());

    // Now that the instrument is known, we can update instrument-dependent comboboxes etc
    cdw->setupInstrumentComboBox_clicked();
    cdw->ui->skyAreaComboBox->setCurrentIndex(settings.value("skyAreaComboBox").toInt());

    settings.endGroup();
    preventLoop_WriteSettings = false;
    readingSettings = false;

    // Housekeeping for correct display
    cdw->updateARCdisplay();

    updatePreferences();

    updateInstrumentComboBoxBackgroundColor();

    return settings.status();
}
