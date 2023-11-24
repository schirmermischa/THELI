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
#include <QValidator>
#include <QRegExpValidator>

void ConfDockWidget::validate()
{
    QRegExp ri( "^[0-9]+" );
    QRegExp ricomma( "^[0-9,]+" );
    QRegExp ricommablank( "^[0-9,\\s]+" );
    QRegExp rint( "^[-]{0,1}[0-9]+" );
    QRegExp rx( "^\\S+$" );
    QRegExp RA( "[0-9.:]+" );
    QRegExp DEC( "^[-]{0,1}[0-9.:]+" );
    QRegExp rx3( "^[A-Z0-9a-z-+._,:]+$" );
    QRegExp rx4( "^[A-Z0-9a-z-+_]+$" );
    QRegExp rx5( "^[A-Z0-9a-z-+._, ]+$" );
    QRegExp rf( "^[-]{0,1}[0-9]*[.]{0,1}[0-9]+" );
    QRegExp rf2( "^[0-9]*[.]{0,1}[0-9]+" );

    QValidator* validator_int_pos = new QRegExpValidator( ri, this );
    QValidator* validator_int_pos_comma = new QRegExpValidator( ricomma, this );
    QValidator* validator_int_pos_comma_blank = new QRegExpValidator( ricommablank, this );
    QValidator* validator_int = new QRegExpValidator( rint, this );
    QValidator* validator_string = new QRegExpValidator( rx, this );
    QValidator* validator_ra = new QRegExpValidator( RA, this );
    QValidator* validator_dec = new QRegExpValidator( DEC, this );
    QValidator* validator_string3 = new QRegExpValidator( rx3, this );
    QValidator* validator_headerkeyword = new QRegExpValidator( rx4, this );
    QValidator* validator_stringdir = new QRegExpValidator( rx5, this );
    QValidator* validator_float = new QRegExpValidator(rf, this );
    QValidator* validator_float_pos = new QRegExpValidator(rf2, this );

    ui->APDmaxphoterrorLineEdit->setValidator( validator_float_pos );
    ui->APIWCSLineEdit->setValidator( validator_float_pos );
    ui->APIapertureLineEdit->setValidator( validator_int_pos );
    ui->APIcolortermLineEdit->setValidator( validator_float );
    ui->APIconvergenceLineEdit->setValidator( validator_float_pos );
    ui->APIextinctionLineEdit->setValidator( validator_float );
    ui->APIfilterkeywordLineEdit->setValidator( validator_string );
    ui->APIkappaLineEdit->setValidator( validator_float_pos );
    ui->APIminmagLineEdit->setValidator( validator_float_pos );
    ui->APIthresholdLineEdit->setValidator( validator_float_pos );
    ui->APIzpmaxLineEdit->setValidator( validator_float_pos );
    ui->APIzpminLineEdit->setValidator( validator_float_pos );
    ui->ARCdecLineEdit->setValidator( validator_dec );
    ui->ARCmaxpmLineEdit->setValidator( validator_float_pos );
    ui->ARCminmagLineEdit->setValidator( validator_float_pos );
    ui->ARCraLineEdit->setValidator( validator_ra );
    ui->ARCradiusLineEdit->setValidator( validator_float_pos );
    ui->ARCselectimageLineEdit->setValidator( validator_string );
    ui->ARCtargetresolverLineEdit->setValidator( validator_stringdir );
    ui->ASTastrefweightLineEdit->setValidator( validator_float_pos );
    ui->ASTastrinstrukeyLineEdit->setValidator( validator_headerkeyword );
    ui->ASTcrossidLineEdit->setValidator( validator_float_pos );
    ui->ASTdistortLineEdit->setValidator( validator_int_pos );
    ui->ASTdistortgroupsLineEdit->setValidator( validator_int_pos_comma );
    ui->ASTdistortkeysLineEdit->setValidator( validator_string3 );
    ui->ASTphotinstrukeyLineEdit->setValidator( validator_headerkeyword );
    ui->ASTpixscaleLineEdit->setValidator( validator_float_pos );
    ui->ASTposangleLineEdit->setValidator( validator_float_pos );
    ui->ASTpositionLineEdit->setValidator( validator_float_pos );
    ui->ASTresolutionLineEdit->setValidator( validator_int_pos );
    ui->ASTsnthreshLineEdit->setValidator( validator_int_pos_comma );
    ui->ASTxcorrDMINLineEdit->setValidator( validator_int_pos );
    ui->ASTxcorrDTLineEdit->setValidator( validator_float_pos );
    ui->BAC1nhighLineEdit->setValidator( validator_int_pos );
    ui->BAC1nlowLineEdit->setValidator( validator_int_pos);
    ui->BAC2nhighLineEdit->setValidator( validator_int_pos );
    ui->BAC2nlowLineEdit->setValidator( validator_int_pos );
    ui->BACDMINLineEdit->setValidator( validator_int_pos );
    ui->BACDTLineEdit->setValidator( validator_float_pos);
    ui->BACbacksmoothscaleLineEdit->setValidator( validator_int_pos );
    ui->BACdistLineEdit->setValidator( validator_float_pos );
    ui->BACgapsizeLineEdit->setValidator( validator_float_pos);
    ui->BACmagLineEdit->setValidator( validator_float_pos );
    ui->BACmefLineEdit->setValidator( validator_float_pos );
    ui->BACwindowLineEdit->setValidator( validator_int_pos );
    ui->BACminWindowLineEdit->setValidator( validator_int_pos );
    ui->CGWbackclustolLineEdit->setValidator( validator_float_pos);
    ui->CGWbackcoltolLineEdit->setValidator( validator_float_pos );
    ui->CGWbackrowtolLineEdit->setValidator( validator_float_pos);
    ui->CGWbacksmoothLineEdit->setValidator( validator_float_pos );
    ui->CGWbackmaxLineEdit->setValidator( validator_float_pos );
    ui->CGWbackminLineEdit->setValidator( validator_float_pos );
    ui->CGWdarkmaxLineEdit->setValidator( validator_int );
    ui->CGWdarkminLineEdit->setValidator( validator_int );
    ui->CGWflatclustolLineEdit->setValidator( validator_float_pos );
    ui->CGWflatcoltolLineEdit->setValidator( validator_float_pos );
    ui->CGWflatmaxLineEdit->setValidator( validator_float_pos );
    ui->CGWflatminLineEdit->setValidator( validator_float_pos );
    ui->CGWflatrowtolLineEdit->setValidator( validator_float_pos );
    ui->CGWflatsmoothLineEdit->setValidator( validator_float_pos );
    ui->CIWsaturationLineEdit->setValidator( validator_float_pos );
    ui->CIWbloomRangeLineEdit->setValidator( validator_int_pos );
    ui->CIWbloomMinaduLineEdit->setValidator( validator_int_pos );
    ui->CIWaggressivenessLineEdit->setValidator( validator_float_pos );
    ui->CIWmaxaduLineEdit->setValidator( validator_int );
    ui->CIWminaduLineEdit->setValidator( validator_int );
    ui->COAchipsLineEdit->setValidator( validator_int_pos_comma_blank );
    ui->COAdecLineEdit->setValidator( validator_dec );
    ui->COAedgesmoothingLineEdit->setValidator( validator_int_pos );
    ui->COAoutborderLineEdit->setValidator( validator_int_pos );
    ui->COAoutsizeLineEdit->setValidator( validator_int_pos );
    ui->COAoutthreshLineEdit->setValidator( validator_float_pos );
    ui->COApixscaleLineEdit->setValidator( validator_float_pos );
    ui->COApmdecLineEdit->setValidator( validator_float );
    ui->COApmraLineEdit->setValidator( validator_float );
    ui->COAraLineEdit->setValidator( validator_ra );
    ui->COAminMJDOBSLineEdit->setValidator( validator_float_pos );
    ui->COAmaxMJDOBSLineEdit->setValidator( validator_float_pos );
    ui->COAsizexLineEdit->setValidator( validator_int_pos );
    ui->COAsizeyLineEdit->setValidator( validator_int_pos );
    ui->COAskypaLineEdit->setValidator( validator_float );
    ui->COAuniqueidLineEdit->setValidator( validator_string );
    ui->COCDMINLineEdit->setValidator( validator_int_pos );
    ui->COCDTLineEdit->setValidator( validator_float_pos );
    ui->COCmefLineEdit->setValidator( validator_float_pos );
    ui->COCrejectLineEdit->setValidator( validator_float_pos );
    ui->COCxmaxLineEdit->setValidator( validator_int_pos );
    ui->COCxminLineEdit->setValidator( validator_int_pos );
    ui->COCymaxLineEdit->setValidator( validator_int_pos );
    ui->COCyminLineEdit->setValidator( validator_int_pos );
    ui->CSCDMINLineEdit->setValidator( validator_int_pos );
    ui->CSCDTLineEdit->setValidator( validator_float_pos );
    ui->CSCFWHMLineEdit->setValidator( validator_float_pos );
    ui->CSCbackgroundLineEdit->setValidator( validator_float );
    ui->CSCmaxflagLineEdit->setValidator( validator_int_pos );
    ui->CSCmaxellLineEdit->setValidator( validator_float_pos );
    ui->CSCmincontLineEdit->setValidator( validator_float_pos );
    ui->CSCrejectExposureLineEdit->setValidator( validator_int_pos );
    ui->CSCsaturationLineEdit->setValidator( validator_int_pos );
    ui->SPSlengthLineEdit->setValidator( validator_int_pos );
    ui->SPSexcludeLineEdit->setValidator( validator_int_pos );
    ui->SPSnumbergroupsLineEdit->setValidator( validator_int_pos );
    ui->biasMaxLineEdit->setValidator( validator_int_pos );
    ui->biasMinLineEdit->setValidator( validator_int_pos );
    ui->biasNhighLineEdit->setValidator( validator_int_pos );
    ui->biasNlowLineEdit->setValidator( validator_int_pos );
    ui->darkMaxLineEdit->setValidator( validator_int_pos );
    ui->darkMinLineEdit->setValidator( validator_int_pos );
    ui->darkNhighLineEdit->setValidator( validator_int_pos );
    ui->darkNlowLineEdit->setValidator( validator_int_pos );
    ui->excludeDetectorsLineEdit->setValidator( validator_int_pos_comma_blank );
    ui->flatoffMaxLineEdit->setValidator( validator_int_pos );
    ui->flatoffMinLineEdit->setValidator( validator_int_pos );
    ui->flatoffNhighLineEdit->setValidator( validator_int_pos );
    ui->flatoffNlowLineEdit->setValidator( validator_int_pos );
    ui->flatMaxLineEdit->setValidator( validator_int_pos );
    ui->flatMinLineEdit->setValidator( validator_int_pos );
    ui->flatNhighLineEdit->setValidator( validator_int_pos );
    ui->flatNlowLineEdit->setValidator( validator_int_pos );
    ui->normalxtalkAmplitudeLineEdit->setValidator( validator_float );
    ui->rowxtalkAmplitudeLineEdit->setValidator( validator_float );
    ui->saturationLineEdit->setValidator( validator_int_pos );
    ui->separateTargetLineEdit->setValidator( validator_float_pos );
    ui->skyDMINLineEdit->setValidator( validator_int_pos );
    ui->skyDTLineEdit->setValidator( validator_float_pos );
    ui->skyKernelLineEdit->setValidator( validator_int_pos );
    ui->skyMefLineEdit->setValidator( validator_float_pos );
}

void ConfDockWidget::connect_validators()
{
    connect(ui->APDmaxphoterrorLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->APIWCSLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->APIapertureLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->APIcolortermLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->APIconvergenceLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->APIextinctionLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->APIfilterkeywordLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->APIkappaLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->APIminmagLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->APIthresholdLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->APIzpmaxLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->APIzpminLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->ARCdecLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->ARCminmagLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->ARCraLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->ARCradiusLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->ARCselectimageLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->ARCtargetresolverLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->ASTastrefweightLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->ASTastrinstrukeyLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->ASTcrossidLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->ASTdistortLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->ASTdistortgroupsLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->ASTdistortkeysLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->ASTphotinstrukeyLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->ASTpixscaleLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->ASTposangleLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->ASTpositionLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->ASTresolutionLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->ASTsnthreshLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->BAC1nhighLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->BAC1nlowLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->BAC2nhighLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->BAC2nlowLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->BACDMINLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->BACDTLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->BACbacksmoothscaleLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->BACdistLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->BACgapsizeLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->BACmagLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->BACmefLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->BACwindowLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->BACminWindowLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CGWbackclustolLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CGWbackcoltolLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CGWbackrowtolLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CGWbacksmoothLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CGWbackmaxLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CGWbackminLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CGWdarkmaxLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CGWdarkminLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CGWflatclustolLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CGWflatcoltolLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CGWflatmaxLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CGWflatminLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CGWflatrowtolLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CGWflatsmoothLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CIWbloomRangeLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CIWsaturationLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CIWbloomMinaduLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CIWaggressivenessLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CIWmaxaduLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CIWminaduLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COAchipsLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COAdecLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COAedgesmoothingLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COAoutborderLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COAoutsizeLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COAoutthreshLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COApixscaleLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COApmdecLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COApmraLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COAraLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COAminMJDOBSLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COAmaxMJDOBSLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COAsizexLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COAsizeyLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COAskypaLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COAuniqueidLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COCDMINLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COCDTLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COCmefLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COCrejectLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COCxmaxLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COCxminLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COCymaxLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->COCyminLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CSCDMINLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CSCDTLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CSCFWHMLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CSCbackgroundLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CSCmaxflagLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CSCmaxellLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CSCmincontLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CSCrejectExposureLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->CSCsaturationLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->SPSlengthLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->SPSexcludeLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->SPSnumbergroupsLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->biasMaxLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->biasMinLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->biasNhighLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->biasNlowLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->darkMaxLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->darkMinLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->darkNhighLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->darkNlowLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->excludeDetectorsLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->flatoffMaxLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->flatoffMinLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->flatoffNhighLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->flatoffNlowLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->flatMaxLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->flatMinLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->flatNhighLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->flatNlowLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->normalxtalkAmplitudeLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->rowxtalkAmplitudeLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->saturationLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->separateTargetLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->skyDMINLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->skyDTLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->skyKernelLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
    connect(ui->skyMefLineEdit, &QLineEdit::textChanged, this, &ConfDockWidget::validate);
}
