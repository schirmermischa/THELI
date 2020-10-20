/********************************************************************************
** Form generated from reading UI file 'confdockwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFDOCKWIDGET_H
#define UI_CONFDOCKWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfDockWidget
{
public:
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout;
    QStackedWidget *confStackedWidget;
    QWidget *HDUpage;
    QGridLayout *gridLayout_31;
    QVBoxLayout *verticalLayout_3;
    QLabel *conftitleHDUreformatLabel;
    QLabel *confsubtitleHDUreformat1Label;
    QFrame *confHDUreformat1Frame;
    QGridLayout *gridLayout_17;
    QCheckBox *splitMIRcubeCheckBox;
    QCheckBox *theliRenamingCheckBox;
    QLabel *confsubtitleHDUreformat2Label;
    QFrame *confHDUreformat2Frame;
    QGridLayout *gridLayout_3;
    QCheckBox *overscanCheckBox;
    QSpacerItem *horizontalSpacer_15;
    QCheckBox *nonlinearityCheckBox;
    QCheckBox *normalxtalkCheckBox;
    QSpacerItem *horizontalSpacer_11;
    QLabel *xtalkRowLabel;
    QLabel *xtalkNorLabel;
    QLineEdit *normalxtalkAmplitudeLineEdit;
    QCheckBox *rowxtalkCheckBox;
    QSpacerItem *horizontalSpacer_9;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *xtalk_nor_2x2ToolButton;
    QToolButton *xtalk_nor_1x2ToolButton;
    QToolButton *xtalk_nor_2x1ToolButton;
    QGridLayout *gridLayout_2;
    QToolButton *xtalk_row_2x2ToolButton;
    QToolButton *xtalk_row_1x2ToolButton;
    QToolButton *xtalk_row_2x1ToolButton;
    QToolButton *xtalk_col_2x2ToolButton;
    QToolButton *xtalk_col_1x2ToolButton;
    QToolButton *xtalk_col_2x1ToolButton;
    QLineEdit *rowxtalkAmplitudeLineEdit;
    QComboBox *overscanMethodComboBox;
    QLineEdit *saturationLineEdit;
    QLabel *label_21;
    QSpacerItem *verticalSpacer_33;
    QVBoxLayout *verticalLayout_4;
    QLabel *conftitleExcludeDetectorsLabel;
    QFrame *confExcludeDetectorsFrame;
    QGridLayout *gridLayout_22;
    QLineEdit *excludeDetectorsLineEdit;
    QLabel *label_2;
    QSpacerItem *verticalSpacer_5;
    QLabel *conftitleProcessbiasLabel;
    QLabel *confsubtitleCalibrators1Label;
    QFrame *confCalibrators1Frame;
    QGridLayout *gridLayout_14;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_22;
    QLabel *label_3;
    QLineEdit *biasNlowLineEdit;
    QLineEdit *biasNhighLineEdit;
    QComboBox *biasMethodComboBox;
    QLabel *label_4;
    QLineEdit *darkNlowLineEdit;
    QLineEdit *darkNhighLineEdit;
    QComboBox *darkMethodComboBox;
    QLabel *label_15;
    QLineEdit *flatoffNlowLineEdit;
    QLineEdit *flatoffNhighLineEdit;
    QComboBox *flatoffMethodComboBox;
    QLabel *label_5;
    QLineEdit *flatNlowLineEdit;
    QLineEdit *flatNhighLineEdit;
    QComboBox *flatMethodComboBox;
    QLabel *confsubtitleCalibrators2Label;
    QFrame *confCalibrators2Frame;
    QGridLayout *gridLayout_15;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_11;
    QLineEdit *biasMinLineEdit;
    QLineEdit *biasMaxLineEdit;
    QLabel *label_12;
    QLineEdit *darkMinLineEdit;
    QLineEdit *darkMaxLineEdit;
    QLabel *label_16;
    QLineEdit *flatoffMinLineEdit;
    QLineEdit *flatoffMaxLineEdit;
    QLineEdit *flatMinLineEdit;
    QLineEdit *flatMaxLineEdit;
    QLabel *label_13;
    QSpacerItem *verticalSpacer_34;
    QSpacerItem *horizontalSpacer_7;
    QWidget *SPSpage;
    QGridLayout *gridLayout_32;
    QVBoxLayout *verticalLayout_14;
    QLabel *conftitleChopnodLabel;
    QFrame *confChopnodFrame;
    QGridLayout *gridLayout_28;
    QLabel *label_59;
    QComboBox *chopnodComboBox;
    QCheckBox *chopnodInvertCheckBox;
    QSpacerItem *confChopnodSpacer;
    QLabel *conftitleBackground1Label;
    QLabel *confsubtitleBackground1Label;
    QFrame *confBackground1Frame;
    QGridLayout *gridLayout_21;
    QLabel *label;
    QLineEdit *SPSnumbergroupsLineEdit;
    QLabel *label_10;
    QLineEdit *SPSlengthLineEdit;
    QLabel *label_24;
    QLineEdit *SPSexcludeLineEdit;
    QLabel *confsubtitleBackground2Label;
    QFrame *confBackground2Frame;
    QGridLayout *gridLayout_8;
    QFormLayout *formLayout_10;
    QLabel *BACmefLabel;
    QLineEdit *BACmefLineEdit;
    QLabel *BACmethodLabel;
    QComboBox *BACmethodComboBox;
    QCheckBox *BAC2passCheckBox;
    QCheckBox *BACconvolutionCheckBox;
    QFormLayout *formLayout_9;
    QLabel *BACDTLabel;
    QLineEdit *BACDTLineEdit;
    QLabel *BACDMINLabel;
    QLineEdit *BACDMINLineEdit;
    QLabel *confsubtitleBackground3Label;
    QFrame *confBackground3Frame;
    QGridLayout *gridLayout_9;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *conf1passLabel;
    QLineEdit *BAC1nlowLineEdit;
    QLineEdit *BAC1nhighLineEdit;
    QLabel *conf2passLabel;
    QLineEdit *BAC2nlowLineEdit;
    QLineEdit *BAC2nhighLineEdit;
    QSpacerItem *verticalSpacer_35;
    QVBoxLayout *verticalLayout_15;
    QLabel *conftitleBackground2Label;
    QLabel *confsubtitleBackground4Label;
    QFrame *confBackground4Frame;
    QGridLayout *gridLayout_11;
    QLabel *confBrightstarMagLabel;
    QLineEdit *BACmagLineEdit;
    QLabel *confBrightstarDistanceLabel;
    QLineEdit *BACdistLineEdit;
    QLabel *confsubtitleBackground5Label;
    QFrame *confBackground5Frame;
    QGridLayout *gridLayout_41;
    QComboBox *BACapplyComboBox;
    QLabel *label_14;
    QLineEdit *BACbacksmoothscaleLineEdit;
    QCheckBox *BACrescaleCheckBox;
    QLabel *confsubtitleBackground6Label;
    QFrame *confBackground6Frame;
    QGridLayout *gridLayout_42;
    QLabel *BACcurrentmodeLabel;
    QLabel *BACgapsizeLabel;
    QLineEdit *BACgapsizeLineEdit;
    QLabel *BACwindowLabel;
    QLineEdit *BACwindowLineEdit;
    QLineEdit *BACminWindowLineEdit;
    QLabel *label_23;
    QSpacerItem *verticalSpacer_12;
    QSpacerItem *verticalSpacer_36;
    QSpacerItem *horizontalSpacer_8;
    QWidget *COCpage;
    QGridLayout *gridLayout_59;
    QVBoxLayout *verticalLayout_5;
    QLabel *conftitleCollapseLabel;
    QLabel *confsubtitleCollapse1Label;
    QFrame *confCollapse1Frame;
    QGridLayout *gridLayout_20;
    QLabel *confCollapseDTLabel;
    QLineEdit *COCDTLineEdit;
    QLabel *confCollapseDMINLabel;
    QLineEdit *COCDMINLineEdit;
    QLabel *confCollapseMEFLabel;
    QLineEdit *COCmefLineEdit;
    QLabel *confsubtitleCollapse2Label;
    QFrame *confCollapse2Frame;
    QGridLayout *gridLayout_19;
    QLabel *label_26;
    QLineEdit *COCrejectLineEdit;
    QLabel *label_27;
    QLabel *COCdirectionLabel;
    QComboBox *COCdirectionComboBox;
    QLabel *confsubtitleCollapse3Label;
    QFrame *confCollapse3Frame;
    QGridLayout *gridLayout_18;
    QLabel *confCollapseExcludeXLabel_3;
    QLabel *confCollapseExcludeXLabel_2;
    QLabel *confCollapseExcludeXLabel;
    QLineEdit *COCxminLineEdit;
    QLineEdit *COCxmaxLineEdit;
    QSpacerItem *horizontalSpacer_6;
    QLabel *confCollapseExcludeYLabel;
    QLineEdit *COCyminLineEdit;
    QLineEdit *COCymaxLineEdit;
    QSpacerItem *verticalSpacer_3;
    QVBoxLayout *verticalLayout_13;
    QLabel *conftitleBinnedpreviewLabel;
    QFrame *confBinnedpreviewFrame;
    QGridLayout *gridLayout_24;
    QLabel *confBinningLabel;
    QSpinBox *BIPSpinBox;
    QCheckBox *BIPrejectCheckBox;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *horizontalSpacer_14;
    QWidget *WEIGHTpage;
    QGridLayout *gridLayout_23;
    QVBoxLayout *verticalLayout;
    QLabel *conftitleGlobalweightLabel;
    QLabel *confsubtitleGlobalweight1Label;
    QFrame *confGlobalweight1Frame;
    QGridLayout *gridLayout_66;
    QCheckBox *CGWsameweightCheckBox;
    QLabel *confsubtitleGlobalweight2Label;
    QFrame *confGlobalweight2Frame;
    QGridLayout *gridLayout_53;
    QLabel *label_29;
    QLabel *label_30;
    QLabel *label_32;
    QLineEdit *CGWdarkminLineEdit;
    QLineEdit *CGWdarkmaxLineEdit;
    QLabel *label_33;
    QLineEdit *CGWflatminLineEdit;
    QLineEdit *CGWflatmaxLineEdit;
    QLabel *label_40;
    QLineEdit *CGWbackminLineEdit;
    QLineEdit *CGWbackmaxLineEdit;
    QLabel *confsubtitleGlobalweight3Label;
    QFrame *confGlobalweight3Frame;
    QGridLayout *gridLayout_56;
    QLabel *label_34;
    QLabel *label_35;
    QLabel *label_36;
    QLineEdit *CGWflatsmoothLineEdit;
    QLineEdit *CGWbacksmoothLineEdit;
    QLabel *label_37;
    QLineEdit *CGWflatrowtolLineEdit;
    QLineEdit *CGWbackrowtolLineEdit;
    QLabel *label_38;
    QLineEdit *CGWflatcoltolLineEdit;
    QLineEdit *CGWbackcoltolLineEdit;
    QLabel *label_39;
    QLineEdit *CGWflatclustolLineEdit;
    QLineEdit *CGWbackclustolLineEdit;
    QSpacerItem *verticalSpacer_44;
    QVBoxLayout *verticalLayout_2;
    QLabel *conftitleIndividualweightLabel;
    QLabel *confsubtitleIndividualweight1Label;
    QFrame *confIndividualweight1Frame;
    QGridLayout *gridLayout_4;
    QLabel *confIndWMinLabel;
    QLineEdit *CIWminaduLineEdit;
    QLabel *confIndWMaxLabel;
    QLineEdit *CIWmaxaduLineEdit;
    QLabel *confsubtitleIndividualweight2Label;
    QFrame *confIndividualweight2Frame;
    QGridLayout *gridLayout_6;
    QLabel *confIndWBloomMarginLabel_3;
    QLineEdit *CIWbloomRangeLineEdit;
    QSpacerItem *horizontalSpacer_17;
    QLabel *confIndWBloomMarginLabel;
    QLabel *confIndWBloomMinLabel;
    QSpacerItem *horizontalSpacer_22;
    QLineEdit *CIWaggressivenessLineEdit;
    QSpacerItem *horizontalSpacer_20;
    QLineEdit *CIWbloomMinaduLineEdit;
    QLabel *confIndWBloomMarginLabel_2;
    QSpacerItem *horizontalSpacer_21;
    QLabel *confIndWThresholdLabel;
    QCheckBox *CIWmaskbloomingCheckBox;
    QLineEdit *CIWpersistenceMinaduLineEdit;
    QLabel *confIndWBloomMinLabel_2;
    QLineEdit *CIWpersistenceTimescaleLineEdit;
    QCheckBox *CIWpersistenceCheckBox;
    QCheckBox *CIWmasksaturationCheckBox;
    QLineEdit *CIWsaturationLineEdit;
    QSpacerItem *verticalSpacer_45;
    QLabel *conftitleSeparateLabel;
    QFrame *confSeparateFrame;
    QGridLayout *gridLayout_49;
    QLabel *separationLabel;
    QLineEdit *separateTargetLineEdit;
    QSpacerItem *verticalSpacer_46;
    QSpacerItem *horizontalSpacer_3;
    QWidget *APIpage;
    QGridLayout *gridLayout_62;
    QVBoxLayout *verticalLayout_6;
    QLabel *conftitleAbsphotindirect1Label;
    QLabel *confsubtitleAbsphotindirect1Label;
    QFrame *confAbsphotindirect1Frame;
    QGridLayout *gridLayout_54;
    QGridLayout *gridLayout_51;
    QLabel *label_41;
    QComboBox *APIrefcatComboBox;
    QLabel *confAPILabel_3;
    QComboBox *APIfilterComboBox;
    QLabel *confAPILabel_13;
    QComboBox *APIcolorComboBox;
    QLabel *confAPILabel_11;
    QComboBox *APIcalibrationmodeComboBox;
    QLabel *confAPILabel;
    QLineEdit *APIapertureLineEdit;
    QLabel *confAPILabel_2;
    QLineEdit *APIfilterkeywordLineEdit;
    QLabel *confsubtitleAbsphotindirect2Label;
    QFrame *confAbsphotindirect2Frame;
    QGridLayout *gridLayout_13;
    QGridLayout *gridLayout_7;
    QLabel *confAPILabel_4;
    QLineEdit *APIcolortermLineEdit;
    QLineEdit *APIextinctionLineEdit;
    QLabel *confAPILabel_5;
    QLabel *confAPILabel_7;
    QLineEdit *APIminmagLineEdit;
    QLineEdit *APIkappaLineEdit;
    QLabel *confAPILabel_15;
    QLabel *confAPILabel_10;
    QLineEdit *APIthresholdLineEdit;
    QLineEdit *APIzpminLineEdit;
    QLabel *confAPILabel_14;
    QLineEdit *APIzpmaxLineEdit;
    QLabel *confAPILabel_6;
    QLineEdit *APIconvergenceLineEdit;
    QLabel *confAPILabel_8;
    QSpacerItem *verticalSpacer_47;
    QGridLayout *gridLayout_33;
    QLabel *conftitleAbsphotindirect2Label;
    QLabel *confsubtitleAbsphotindirect3Label;
    QFrame *confAbsphotindirect3Frame;
    QGridLayout *gridLayout_74;
    QLabel *confAPILabel_12;
    QLineEdit *APIWCSLineEdit;
    QCheckBox *APIWCSCheckBox;
    QPushButton *APIreadmePushButton;
    QLabel *confsubtitleAbsphotindirect4Label;
    QFrame *confAbsphotindirect4Frame;
    QGridLayout *gridLayout_58;
    QGridLayout *gridLayout_57;
    QPushButton *APInight7PushButton;
    QPushButton *APInight1PushButton;
    QComboBox *APInight1ComboBox;
    QPushButton *APInight2PushButton;
    QComboBox *APInight2ComboBox;
    QPushButton *APInight3PushButton;
    QComboBox *APInight3ComboBox;
    QPushButton *APInight4PushButton;
    QComboBox *APInight4ComboBox;
    QPushButton *APInight5PushButton;
    QComboBox *APInight5ComboBox;
    QPushButton *APInight6PushButton;
    QComboBox *APInight6ComboBox;
    QComboBox *APInight7ComboBox;
    QPushButton *APInight8PushButton;
    QComboBox *APInight8ComboBox;
    QPushButton *APInight9PushButton;
    QComboBox *APInight9ComboBox;
    QLabel *APInodataLabel;
    QSpacerItem *verticalSpacer_48;
    QSpacerItem *horizontalSpacer_2;
    QWidget *APDpage;
    QGridLayout *gridLayout_34;
    QVBoxLayout *verticalLayout_10;
    QLabel *conftitleCreatesourcecatLabel;
    QLabel *confsubtitleCreatesourcecat1Label;
    QFrame *confCreatesourcecat1Frame;
    QGridLayout *gridLayout_16;
    QLabel *label_20;
    QComboBox *CSCMethodComboBox;
    QLabel *confSourcecatDTLabel;
    QSpacerItem *horizontalSpacer_23;
    QLineEdit *CSCDTLineEdit;
    QLabel *confSourcecatDMINLabel;
    QSpacerItem *horizontalSpacer_18;
    QLineEdit *CSCDMINLineEdit;
    QLabel *confSourcecatDeblendLabel;
    QSpacerItem *horizontalSpacer_24;
    QLineEdit *CSCmincontLineEdit;
    QLabel *confSourcecatFWHMLabel;
    QSpacerItem *horizontalSpacer_25;
    QLineEdit *CSCFWHMLineEdit;
    QCheckBox *CSCconvolutionCheckBox;
    QLabel *confsubtitleCreatesourcecat2Label;
    QFrame *confCreatesourcecat2Frame;
    QGridLayout *gridLayout_44;
    QLabel *confSourcecatSaturationLabel;
    QLineEdit *CSCmaxflagLineEdit;
    QLineEdit *CSCbackgroundLineEdit;
    QLabel *confSourcecatBackgroundLabel;
    QLineEdit *CSCsaturationLineEdit;
    QLabel *confSourcecatFLAGLabel;
    QCheckBox *CSCsamplingCheckBox;
    QLabel *confsubtitleCreatesourcecat3Label;
    QFrame *confCreatesourcecat3Frame;
    QGridLayout *gridLayout_46;
    QLabel *confSourcecatFLAGLabel_2;
    QLineEdit *CSCrejectExposureLineEdit;
    QSpacerItem *verticalSpacer_52;
    QVBoxLayout *verticalLayout_9;
    QLabel *conftitleGetCatalogLabel;
    QLabel *confsubtitleGetCatalog1Label;
    QFrame *confGetCatalog1Frame;
    QGridLayout *gridLayout_50;
    QRadioButton *ARCwebRadioButton;
    QRadioButton *ARCimageRadioButton;
    QLabel *confsubtitleGetCatalog2Label;
    QFrame *confGetCatalog2Frame;
    QGridLayout *gridLayout_61;
    QSpacerItem *horizontalSpacer_19;
    QPushButton *ARCgetcatalogPushButton;
    QStackedWidget *ARCstackedWidget;
    QWidget *page;
    QGridLayout *gridLayout_36;
    QLabel *ARCserverLabel_2;
    QFrame *gaiaFrame;
    QGridLayout *gridLayout_29;
    QLabel *ARCgaiaLabel;
    QLabel *ARCmaxpmLabel;
    QSpacerItem *horizontalSpacer;
    QLineEdit *ARCmaxpmLineEdit;
    QLabel *ARCpmRALabel;
    QLineEdit *ARCpmRALineEdit;
    QLabel *ARCpmDECLabel;
    QLineEdit *ARCpmDECLineEdit;
    QLabel *ARCraLabel;
    QLineEdit *ARCraLineEdit;
    QLabel *ARCdecLabel;
    QLineEdit *ARCdecLineEdit;
    QLabel *ARCtargetresolverLabel;
    QToolButton *ARCtargetresolverToolButton;
    QLineEdit *ARCtargetresolverLineEdit;
    QLabel *ARCminmagLabel;
    QLineEdit *ARCminmagLineEdit;
    QLabel *ARCradiusLabel;
    QLineEdit *ARCradiusLineEdit;
    QSpacerItem *verticalSpacer_10;
    QComboBox *ARCcatalogComboBox;
    QWidget *page_2;
    QGridLayout *gridLayout_45;
    QGridLayout *gridLayout_26;
    QLabel *confSourcecatDTLabel_3;
    QLineEdit *ARCDMINLineEdit;
    QLineEdit *ARCselectimageLineEdit;
    QLabel *confSourcecatDTLabel_4;
    QPushButton *ARCselectimagePushButton;
    QLineEdit *ARCDTLineEdit;
    QSpacerItem *verticalSpacer_9;
    QSpacerItem *verticalSpacer_7;
    QSpacerItem *horizontalSpacer_13;
    QWidget *ARCpage;
    QGridLayout *gridLayout_35;
    QVBoxLayout *verticalLayout_8;
    QLabel *conftitleAstromphotom1Label;
    QLabel *confsubtitleAstromphotom1Label;
    QFrame *confAstromphotom1Frame;
    QGridLayout *gridLayout_60;
    QLineEdit *ASTxcorrDTLineEdit;
    QLabel *ASTxcorrDMINLabel;
    QLineEdit *ASTxcorrDMINLineEdit;
    QLabel *ASTxcorrDTLabel;
    QComboBox *ASTmethodComboBox;
    QPushButton *restoreHeaderPushButton;
    QLabel *confsubtitleAstromphotom2Label;
    QFrame *confAstromphotom2Frame;
    QGridLayout *gridLayout_27;
    QLabel *ASTLabel_16;
    QComboBox *ASTmatchMethodComboBox;
    QLabel *ASTposangleLabel;
    QLineEdit *ASTposangleLineEdit;
    QLabel *ASTpositionLabel;
    QLineEdit *ASTpositionLineEdit;
    QLabel *ASTLabel_3;
    QLineEdit *ASTpixscaleLineEdit;
    QLabel *ASTLabel_6;
    QLineEdit *ASTcrossidLineEdit;
    QCheckBox *ASTmatchflippedCheckBox;
    QLabel *confsubtitleAstromphotom3Label;
    QFrame *confAstromphotom3Frame;
    QGridLayout *gridLayout_63;
    QLineEdit *ASTastrefweightLineEdit;
    QLabel *ASTLabel_10;
    QLabel *ASTLabel_15;
    QLineEdit *ASTphotinstrukeyLineEdit;
    QLineEdit *ASTsnthreshLineEdit;
    QLabel *ASTLabel_7;
    QLineEdit *ASTastrinstrukeyLineEdit;
    QLabel *ASTLabel_14;
    QSpacerItem *verticalSpacer_8;
    QVBoxLayout *verticalLayout_16;
    QLabel *conftitleAstromphotom2Label;
    QLabel *confsubtitleAstromphotom4Label;
    QFrame *confAstromphotom4Frame;
    QGridLayout *gridLayout_55;
    QLineEdit *ASTdistortkeysLineEdit;
    QLineEdit *ASTdistortLineEdit;
    QLabel *ASTLabel_8;
    QLabel *ASTLabel_9;
    QLineEdit *ASTdistortgroupsLineEdit;
    QLabel *ASTLabel_4;
    QLabel *confsubtitleAstromphotom5Label;
    QFrame *confAstromphotom5Frame;
    QGridLayout *gridLayout_10;
    QLabel *ASTLabel_11;
    QComboBox *ASTstabilityComboBox;
    QLabel *ASTLabel_12;
    QComboBox *ASTmosaictypeComboBox;
    QLabel *ASTLabel_13;
    QComboBox *ASTfocalplaneComboBox;
    QPushButton *ASTreadmepushButton;
    QLabel *confsubtitleAstromphotom6Label;
    QFrame *confAstromphotom6Frame;
    QGridLayout *gridLayout_64;
    QLabel *ASTLabel_5;
    QLineEdit *ASTresolutionLineEdit;
    QPushButton *ASTviewCheckPlotsPushButton;
    QSpacerItem *verticalSpacer_2;
    QLabel *conftitleStarflatLabel;
    QFrame *confStarflatFrame;
    QGridLayout *gridLayout_12;
    QLabel *confAPDRefcatLabel;
    QComboBox *APDrefcatComboBox;
    QLabel *confAPDFilterLabel;
    QComboBox *APDfilterComboBox;
    QLabel *confAPDMaxphoterrorLabel;
    QLineEdit *APDmaxphoterrorLineEdit;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer_5;
    QWidget *SKYpage;
    QGridLayout *gridLayout_65;
    QVBoxLayout *verticalLayout_7;
    QLabel *conftitleSkysubLabel;
    QLabel *confsubtitleSkysub1Label;
    QFrame *confSkysub1Frame;
    QGridLayout *gridLayout_85;
    QRadioButton *skyModelRadioButton;
    QPushButton *skyReadmePushButton;
    QRadioButton *skyPolynomialRadioButton;
    QFrame *line;
    QRadioButton *skyConstsubRadioButton;
    QCheckBox *skySavemodelCheckBox;
    QSpinBox *skyPolynomialSpinBox;
    QLabel *confsubtitleSkysub2Label;
    QFrame *confSkysub2Frame;
    QGridLayout *gridLayout_75;
    QLabel *confCollapseDMINLabel_2;
    QLabel *confCollapseDTLabel_2;
    QLineEdit *skyDTLineEdit;
    QLineEdit *skyMefLineEdit;
    QLineEdit *skyKernelLineEdit;
    QLabel *confCollapseDMINLabel_3;
    QLabel *confCollapseMEFLabel_2;
    QLineEdit *skyDMINLineEdit;
    QLabel *confsubtitleSkysub3Label;
    QFrame *confSkysub3Frame;
    QGridLayout *gridLayout_25;
    QLabel *label_17;
    QComboBox *skyAreaComboBox;
    QPushButton *skyAreaPushButton;
    QSpacerItem *verticalSpacer_6;
    QSpacerItem *horizontalSpacer_10;
    QWidget *COCpage_2;
    QGridLayout *gridLayout_67;
    QVBoxLayout *verticalLayout_11;
    QLabel *conftitleCoadd1Label;
    QLabel *confsubtitleCoadd1Label;
    QFrame *confCoadd1Frame;
    QGridLayout *gridLayout_52;
    QLabel *COAtype1Label;
    QLineEdit *COAraLineEdit;
    QLabel *COAtype2Label;
    QLineEdit *COAdecLineEdit;
    QLabel *confsubtitleCoadd2Label;
    QFrame *confCoadd2Frame;
    QGridLayout *gridLayout_38;
    QLineEdit *COApixscaleLineEdit;
    QLineEdit *COAskypaLineEdit;
    QLabel *COCkernelLabel;
    QComboBox *COAcombinetypeComboBox;
    QLabel *COCkernelLabel_2;
    QComboBox *COAkernelComboBox;
    QComboBox *COAprojectionComboBox;
    QComboBox *COAcelestialtypeComboBox;
    QCheckBox *COArescaleweightsCheckBox;
    QLabel *label_53;
    QLabel *COCkernelLabel_3;
    QLabel *COCkernelLabel_4;
    QLabel *COCkernelLabel_5;
    QPushButton *COAskypaPushButton;
    QSpacerItem *horizontalSpacer_4;
    QLabel *confsubtitleCoadd3Label;
    QFrame *confCoadd3Frame;
    QGridLayout *gridLayout_83;
    QFormLayout *formLayout_16;
    QLabel *label_56;
    QLineEdit *COAoutthreshLineEdit;
    QLabel *label_58;
    QLineEdit *COAoutsizeLineEdit;
    QLabel *label_57;
    QLineEdit *COAoutborderLineEdit;
    QSpacerItem *verticalSpacer_54;
    QVBoxLayout *verticalLayout_12;
    QLabel *conftitleCoadd2Label;
    QLabel *confsubtitleCoadd4Label;
    QFrame *confCoadd4Frame;
    QGridLayout *gridLayout_5;
    QLineEdit *COAuniqueidLineEdit;
    QLineEdit *COAchipsLineEdit;
    QLabel *label_54;
    QLineEdit *COAedgesmoothingLineEdit;
    QLineEdit *COAsizexLineEdit;
    QLabel *label_64;
    QLineEdit *COAsizeyLineEdit;
    QLabel *label_65;
    QLabel *label_46;
    QLabel *label_47;
    QLabel *confsubtitleCoadd5Label;
    QFrame *confCoadd5Frame;
    QGridLayout *gridLayout_30;
    QLabel *label_82;
    QComboBox *COApmComboBox;
    QLabel *label_62;
    QLineEdit *COApmraLineEdit;
    QLabel *label_63;
    QLineEdit *COApmdecLineEdit;
    QLabel *confsubtitleCoadd6Label;
    QFrame *confCoadd6Frame;
    QGridLayout *gridLayout_40;
    QCheckBox *COAfluxcalibCheckBox;
    QSpacerItem *verticalSpacer_53;
    QSpacerItem *horizontalSpacer_16;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *parametersDefaultPushButton;
    QPushButton *confBackwardPushButton;
    QPushButton *confForwardPushButton;
    QLabel *confPageLabel;
    QSpacerItem *horizontalSpacer_12;

    void setupUi(QDockWidget *ConfDockWidget)
    {
        if (ConfDockWidget->objectName().isEmpty())
            ConfDockWidget->setObjectName(QStringLiteral("ConfDockWidget"));
        ConfDockWidget->resize(618, 670);
        ConfDockWidget->setFloating(false);
        ConfDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        gridLayout = new QGridLayout(dockWidgetContents);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        confStackedWidget = new QStackedWidget(dockWidgetContents);
        confStackedWidget->setObjectName(QStringLiteral("confStackedWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(confStackedWidget->sizePolicy().hasHeightForWidth());
        confStackedWidget->setSizePolicy(sizePolicy);
        confStackedWidget->setMinimumSize(QSize(600, 0));
        confStackedWidget->setMaximumSize(QSize(600, 16777215));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(170, 170, 170, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(212, 212, 212, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(85, 85, 85, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(113, 113, 113, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        QBrush brush6(QColor(255, 255, 220, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confStackedWidget->setPalette(palette);
        confStackedWidget->setFocusPolicy(Qt::ClickFocus);
        confStackedWidget->setAutoFillBackground(true);
        confStackedWidget->setFrameShape(QFrame::Panel);
        confStackedWidget->setFrameShadow(QFrame::Sunken);
        confStackedWidget->setLineWidth(1);
        HDUpage = new QWidget();
        HDUpage->setObjectName(QStringLiteral("HDUpage"));
        gridLayout_31 = new QGridLayout(HDUpage);
        gridLayout_31->setObjectName(QStringLiteral("gridLayout_31"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        conftitleHDUreformatLabel = new QLabel(HDUpage);
        conftitleHDUreformatLabel->setObjectName(QStringLiteral("conftitleHDUreformatLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(conftitleHDUreformatLabel->sizePolicy().hasHeightForWidth());
        conftitleHDUreformatLabel->setSizePolicy(sizePolicy1);
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        conftitleHDUreformatLabel->setFont(font);
        conftitleHDUreformatLabel->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleHDUreformatLabel->setWordWrap(true);

        verticalLayout_3->addWidget(conftitleHDUreformatLabel);

        confsubtitleHDUreformat1Label = new QLabel(HDUpage);
        confsubtitleHDUreformat1Label->setObjectName(QStringLiteral("confsubtitleHDUreformat1Label"));
        confsubtitleHDUreformat1Label->setFont(font);
        confsubtitleHDUreformat1Label->setFrameShape(QFrame::NoFrame);

        verticalLayout_3->addWidget(confsubtitleHDUreformat1Label);

        confHDUreformat1Frame = new QFrame(HDUpage);
        confHDUreformat1Frame->setObjectName(QStringLiteral("confHDUreformat1Frame"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(confHDUreformat1Frame->sizePolicy().hasHeightForWidth());
        confHDUreformat1Frame->setSizePolicy(sizePolicy2);
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush7(QColor(235, 235, 238, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush8(QColor(245, 245, 246, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        QBrush brush9(QColor(117, 117, 119, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush9);
        QBrush brush10(QColor(157, 157, 159, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confHDUreformat1Frame->setPalette(palette1);
        confHDUreformat1Frame->setAutoFillBackground(true);
        confHDUreformat1Frame->setFrameShape(QFrame::NoFrame);
        gridLayout_17 = new QGridLayout(confHDUreformat1Frame);
        gridLayout_17->setObjectName(QStringLiteral("gridLayout_17"));
        splitMIRcubeCheckBox = new QCheckBox(confHDUreformat1Frame);
        splitMIRcubeCheckBox->setObjectName(QStringLiteral("splitMIRcubeCheckBox"));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        splitMIRcubeCheckBox->setFont(font1);
        splitMIRcubeCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_17->addWidget(splitMIRcubeCheckBox, 1, 0, 1, 2);

        theliRenamingCheckBox = new QCheckBox(confHDUreformat1Frame);
        theliRenamingCheckBox->setObjectName(QStringLiteral("theliRenamingCheckBox"));
        theliRenamingCheckBox->setFont(font1);
        theliRenamingCheckBox->setFocusPolicy(Qt::ClickFocus);
        theliRenamingCheckBox->setChecked(true);

        gridLayout_17->addWidget(theliRenamingCheckBox, 0, 0, 1, 2);


        verticalLayout_3->addWidget(confHDUreformat1Frame);

        confsubtitleHDUreformat2Label = new QLabel(HDUpage);
        confsubtitleHDUreformat2Label->setObjectName(QStringLiteral("confsubtitleHDUreformat2Label"));
        confsubtitleHDUreformat2Label->setFont(font);
        confsubtitleHDUreformat2Label->setFrameShape(QFrame::NoFrame);

        verticalLayout_3->addWidget(confsubtitleHDUreformat2Label);

        confHDUreformat2Frame = new QFrame(HDUpage);
        confHDUreformat2Frame->setObjectName(QStringLiteral("confHDUreformat2Frame"));
        sizePolicy2.setHeightForWidth(confHDUreformat2Frame->sizePolicy().hasHeightForWidth());
        confHDUreformat2Frame->setSizePolicy(sizePolicy2);
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette2.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette2.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette2.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush);
        palette2.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette2.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette2.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette2.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette2.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette2.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette2.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette2.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette2.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette2.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette2.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette2.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confHDUreformat2Frame->setPalette(palette2);
        confHDUreformat2Frame->setAutoFillBackground(true);
        confHDUreformat2Frame->setFrameShape(QFrame::NoFrame);
        gridLayout_3 = new QGridLayout(confHDUreformat2Frame);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        overscanCheckBox = new QCheckBox(confHDUreformat2Frame);
        overscanCheckBox->setObjectName(QStringLiteral("overscanCheckBox"));
        overscanCheckBox->setFont(font1);
        overscanCheckBox->setFocusPolicy(Qt::ClickFocus);
        overscanCheckBox->setChecked(true);

        gridLayout_3->addWidget(overscanCheckBox, 1, 0, 1, 4);

        horizontalSpacer_15 = new QSpacerItem(18, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_15, 2, 0, 1, 1);

        nonlinearityCheckBox = new QCheckBox(confHDUreformat2Frame);
        nonlinearityCheckBox->setObjectName(QStringLiteral("nonlinearityCheckBox"));
        nonlinearityCheckBox->setFont(font1);
        nonlinearityCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_3->addWidget(nonlinearityCheckBox, 3, 0, 1, 4);

        normalxtalkCheckBox = new QCheckBox(confHDUreformat2Frame);
        normalxtalkCheckBox->setObjectName(QStringLiteral("normalxtalkCheckBox"));
        normalxtalkCheckBox->setFont(font1);
        normalxtalkCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_3->addWidget(normalxtalkCheckBox, 4, 0, 1, 4);

        horizontalSpacer_11 = new QSpacerItem(8, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_11, 5, 0, 1, 1);

        xtalkRowLabel = new QLabel(confHDUreformat2Frame);
        xtalkRowLabel->setObjectName(QStringLiteral("xtalkRowLabel"));

        gridLayout_3->addWidget(xtalkRowLabel, 9, 1, 1, 2);

        xtalkNorLabel = new QLabel(confHDUreformat2Frame);
        xtalkNorLabel->setObjectName(QStringLiteral("xtalkNorLabel"));

        gridLayout_3->addWidget(xtalkNorLabel, 6, 1, 1, 1);

        normalxtalkAmplitudeLineEdit = new QLineEdit(confHDUreformat2Frame);
        normalxtalkAmplitudeLineEdit->setObjectName(QStringLiteral("normalxtalkAmplitudeLineEdit"));
        normalxtalkAmplitudeLineEdit->setMinimumSize(QSize(80, 0));
        normalxtalkAmplitudeLineEdit->setMaximumSize(QSize(100, 16777215));

        gridLayout_3->addWidget(normalxtalkAmplitudeLineEdit, 6, 3, 1, 1);

        rowxtalkCheckBox = new QCheckBox(confHDUreformat2Frame);
        rowxtalkCheckBox->setObjectName(QStringLiteral("rowxtalkCheckBox"));
        rowxtalkCheckBox->setFont(font1);
        rowxtalkCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_3->addWidget(rowxtalkCheckBox, 7, 0, 1, 4);

        horizontalSpacer_9 = new QSpacerItem(8, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_9, 8, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        xtalk_nor_2x2ToolButton = new QToolButton(confHDUreformat2Frame);
        xtalk_nor_2x2ToolButton->setObjectName(QStringLiteral("xtalk_nor_2x2ToolButton"));
        xtalk_nor_2x2ToolButton->setFocusPolicy(Qt::ClickFocus);
        QIcon icon;
        icon.addFile(QStringLiteral("icons/xtalk_nor_2x2p.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QStringLiteral("icons/xtalk_nor_2x2a.png"), QSize(), QIcon::Normal, QIcon::On);
        xtalk_nor_2x2ToolButton->setIcon(icon);
        xtalk_nor_2x2ToolButton->setIconSize(QSize(49, 49));
        xtalk_nor_2x2ToolButton->setCheckable(true);
        xtalk_nor_2x2ToolButton->setAutoExclusive(true);

        horizontalLayout_3->addWidget(xtalk_nor_2x2ToolButton);

        xtalk_nor_1x2ToolButton = new QToolButton(confHDUreformat2Frame);
        xtalk_nor_1x2ToolButton->setObjectName(QStringLiteral("xtalk_nor_1x2ToolButton"));
        xtalk_nor_1x2ToolButton->setFocusPolicy(Qt::ClickFocus);
        QIcon icon1;
        icon1.addFile(QStringLiteral("icons/xtalk_nor_1x2p.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon1.addFile(QStringLiteral("icons/xtalk_nor_1x2a.png"), QSize(), QIcon::Normal, QIcon::On);
        xtalk_nor_1x2ToolButton->setIcon(icon1);
        xtalk_nor_1x2ToolButton->setIconSize(QSize(49, 49));
        xtalk_nor_1x2ToolButton->setCheckable(true);
        xtalk_nor_1x2ToolButton->setAutoExclusive(true);

        horizontalLayout_3->addWidget(xtalk_nor_1x2ToolButton);

        xtalk_nor_2x1ToolButton = new QToolButton(confHDUreformat2Frame);
        xtalk_nor_2x1ToolButton->setObjectName(QStringLiteral("xtalk_nor_2x1ToolButton"));
        xtalk_nor_2x1ToolButton->setFocusPolicy(Qt::ClickFocus);
        QIcon icon2;
        icon2.addFile(QStringLiteral("icons/xtalk_nor_2x1p.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon2.addFile(QStringLiteral("icons/xtalk_nor_2x1a.png"), QSize(), QIcon::Normal, QIcon::On);
        xtalk_nor_2x1ToolButton->setIcon(icon2);
        xtalk_nor_2x1ToolButton->setIconSize(QSize(49, 49));
        xtalk_nor_2x1ToolButton->setCheckable(true);
        xtalk_nor_2x1ToolButton->setAutoExclusive(true);

        horizontalLayout_3->addWidget(xtalk_nor_2x1ToolButton);


        gridLayout_3->addLayout(horizontalLayout_3, 5, 1, 1, 3);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        xtalk_row_2x2ToolButton = new QToolButton(confHDUreformat2Frame);
        xtalk_row_2x2ToolButton->setObjectName(QStringLiteral("xtalk_row_2x2ToolButton"));
        xtalk_row_2x2ToolButton->setFocusPolicy(Qt::ClickFocus);
        QIcon icon3;
        icon3.addFile(QStringLiteral("icons/xtalk_row_2x2p.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon3.addFile(QStringLiteral("icons/xtalk_row_2x2a.png"), QSize(), QIcon::Normal, QIcon::On);
        xtalk_row_2x2ToolButton->setIcon(icon3);
        xtalk_row_2x2ToolButton->setIconSize(QSize(49, 49));
        xtalk_row_2x2ToolButton->setCheckable(true);
        xtalk_row_2x2ToolButton->setAutoExclusive(true);

        gridLayout_2->addWidget(xtalk_row_2x2ToolButton, 0, 0, 1, 1);

        xtalk_row_1x2ToolButton = new QToolButton(confHDUreformat2Frame);
        xtalk_row_1x2ToolButton->setObjectName(QStringLiteral("xtalk_row_1x2ToolButton"));
        xtalk_row_1x2ToolButton->setFocusPolicy(Qt::ClickFocus);
        QIcon icon4;
        icon4.addFile(QStringLiteral("icons/xtalk_row_2x1p.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon4.addFile(QStringLiteral("icons/xtalk_row_2x1a.png"), QSize(), QIcon::Normal, QIcon::On);
        xtalk_row_1x2ToolButton->setIcon(icon4);
        xtalk_row_1x2ToolButton->setIconSize(QSize(49, 49));
        xtalk_row_1x2ToolButton->setCheckable(true);
        xtalk_row_1x2ToolButton->setAutoExclusive(true);

        gridLayout_2->addWidget(xtalk_row_1x2ToolButton, 0, 1, 1, 1);

        xtalk_row_2x1ToolButton = new QToolButton(confHDUreformat2Frame);
        xtalk_row_2x1ToolButton->setObjectName(QStringLiteral("xtalk_row_2x1ToolButton"));
        xtalk_row_2x1ToolButton->setFocusPolicy(Qt::ClickFocus);
        QIcon icon5;
        icon5.addFile(QStringLiteral("icons/xtalk_row_1x2p.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon5.addFile(QStringLiteral("icons/xtalk_row_1x2a.png"), QSize(), QIcon::Normal, QIcon::On);
        xtalk_row_2x1ToolButton->setIcon(icon5);
        xtalk_row_2x1ToolButton->setIconSize(QSize(49, 49));
        xtalk_row_2x1ToolButton->setCheckable(true);
        xtalk_row_2x1ToolButton->setAutoExclusive(true);

        gridLayout_2->addWidget(xtalk_row_2x1ToolButton, 0, 2, 1, 1);

        xtalk_col_2x2ToolButton = new QToolButton(confHDUreformat2Frame);
        xtalk_col_2x2ToolButton->setObjectName(QStringLiteral("xtalk_col_2x2ToolButton"));
        xtalk_col_2x2ToolButton->setFocusPolicy(Qt::ClickFocus);
        QIcon icon6;
        icon6.addFile(QStringLiteral("icons/xtalk_col_2x2p.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon6.addFile(QStringLiteral("icons/xtalk_col_2x2a.png"), QSize(), QIcon::Normal, QIcon::On);
        xtalk_col_2x2ToolButton->setIcon(icon6);
        xtalk_col_2x2ToolButton->setIconSize(QSize(49, 49));
        xtalk_col_2x2ToolButton->setCheckable(true);
        xtalk_col_2x2ToolButton->setAutoExclusive(true);

        gridLayout_2->addWidget(xtalk_col_2x2ToolButton, 1, 0, 1, 1);

        xtalk_col_1x2ToolButton = new QToolButton(confHDUreformat2Frame);
        xtalk_col_1x2ToolButton->setObjectName(QStringLiteral("xtalk_col_1x2ToolButton"));
        xtalk_col_1x2ToolButton->setFocusPolicy(Qt::ClickFocus);
        QIcon icon7;
        icon7.addFile(QStringLiteral("icons/xtalk_col_2x1p.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon7.addFile(QStringLiteral("icons/xtalk_col_2x1a.png"), QSize(), QIcon::Normal, QIcon::On);
        xtalk_col_1x2ToolButton->setIcon(icon7);
        xtalk_col_1x2ToolButton->setIconSize(QSize(49, 49));
        xtalk_col_1x2ToolButton->setCheckable(true);
        xtalk_col_1x2ToolButton->setAutoExclusive(true);

        gridLayout_2->addWidget(xtalk_col_1x2ToolButton, 1, 1, 1, 1);

        xtalk_col_2x1ToolButton = new QToolButton(confHDUreformat2Frame);
        xtalk_col_2x1ToolButton->setObjectName(QStringLiteral("xtalk_col_2x1ToolButton"));
        xtalk_col_2x1ToolButton->setFocusPolicy(Qt::ClickFocus);
        QIcon icon8;
        icon8.addFile(QStringLiteral("icons/xtalk_col_1x2p.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon8.addFile(QStringLiteral("icons/xtalk_col_1x2a.png"), QSize(), QIcon::Normal, QIcon::On);
        xtalk_col_2x1ToolButton->setIcon(icon8);
        xtalk_col_2x1ToolButton->setIconSize(QSize(49, 49));
        xtalk_col_2x1ToolButton->setCheckable(true);
        xtalk_col_2x1ToolButton->setAutoExclusive(true);

        gridLayout_2->addWidget(xtalk_col_2x1ToolButton, 1, 2, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 8, 1, 1, 3);

        rowxtalkAmplitudeLineEdit = new QLineEdit(confHDUreformat2Frame);
        rowxtalkAmplitudeLineEdit->setObjectName(QStringLiteral("rowxtalkAmplitudeLineEdit"));
        rowxtalkAmplitudeLineEdit->setMinimumSize(QSize(80, 0));
        rowxtalkAmplitudeLineEdit->setMaximumSize(QSize(100, 16777215));

        gridLayout_3->addWidget(rowxtalkAmplitudeLineEdit, 9, 3, 1, 1);

        overscanMethodComboBox = new QComboBox(confHDUreformat2Frame);
        overscanMethodComboBox->setObjectName(QStringLiteral("overscanMethodComboBox"));
        overscanMethodComboBox->setFocusPolicy(Qt::NoFocus);

        gridLayout_3->addWidget(overscanMethodComboBox, 2, 1, 1, 3);

        saturationLineEdit = new QLineEdit(confHDUreformat2Frame);
        saturationLineEdit->setObjectName(QStringLiteral("saturationLineEdit"));

        gridLayout_3->addWidget(saturationLineEdit, 0, 3, 1, 1);

        label_21 = new QLabel(confHDUreformat2Frame);
        label_21->setObjectName(QStringLiteral("label_21"));

        gridLayout_3->addWidget(label_21, 0, 1, 1, 1);


        verticalLayout_3->addWidget(confHDUreformat2Frame);

        verticalSpacer_33 = new QSpacerItem(207, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_33);


        gridLayout_31->addLayout(verticalLayout_3, 0, 0, 1, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        conftitleExcludeDetectorsLabel = new QLabel(HDUpage);
        conftitleExcludeDetectorsLabel->setObjectName(QStringLiteral("conftitleExcludeDetectorsLabel"));
        sizePolicy1.setHeightForWidth(conftitleExcludeDetectorsLabel->sizePolicy().hasHeightForWidth());
        conftitleExcludeDetectorsLabel->setSizePolicy(sizePolicy1);
        conftitleExcludeDetectorsLabel->setFont(font);
        conftitleExcludeDetectorsLabel->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleExcludeDetectorsLabel->setWordWrap(true);

        verticalLayout_4->addWidget(conftitleExcludeDetectorsLabel);

        confExcludeDetectorsFrame = new QFrame(HDUpage);
        confExcludeDetectorsFrame->setObjectName(QStringLiteral("confExcludeDetectorsFrame"));
        sizePolicy2.setHeightForWidth(confExcludeDetectorsFrame->sizePolicy().hasHeightForWidth());
        confExcludeDetectorsFrame->setSizePolicy(sizePolicy2);
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette3.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette3.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette3.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush);
        palette3.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette3.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette3.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette3.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette3.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confExcludeDetectorsFrame->setPalette(palette3);
        confExcludeDetectorsFrame->setAutoFillBackground(true);
        confExcludeDetectorsFrame->setFrameShape(QFrame::NoFrame);
        gridLayout_22 = new QGridLayout(confExcludeDetectorsFrame);
        gridLayout_22->setObjectName(QStringLiteral("gridLayout_22"));
        excludeDetectorsLineEdit = new QLineEdit(confExcludeDetectorsFrame);
        excludeDetectorsLineEdit->setObjectName(QStringLiteral("excludeDetectorsLineEdit"));

        gridLayout_22->addWidget(excludeDetectorsLineEdit, 1, 0, 1, 1);

        label_2 = new QLabel(confExcludeDetectorsFrame);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_22->addWidget(label_2, 0, 0, 1, 1);


        verticalLayout_4->addWidget(confExcludeDetectorsFrame);

        verticalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_4->addItem(verticalSpacer_5);

        conftitleProcessbiasLabel = new QLabel(HDUpage);
        conftitleProcessbiasLabel->setObjectName(QStringLiteral("conftitleProcessbiasLabel"));
        sizePolicy1.setHeightForWidth(conftitleProcessbiasLabel->sizePolicy().hasHeightForWidth());
        conftitleProcessbiasLabel->setSizePolicy(sizePolicy1);
        conftitleProcessbiasLabel->setFont(font);
        conftitleProcessbiasLabel->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleProcessbiasLabel->setWordWrap(true);

        verticalLayout_4->addWidget(conftitleProcessbiasLabel);

        confsubtitleCalibrators1Label = new QLabel(HDUpage);
        confsubtitleCalibrators1Label->setObjectName(QStringLiteral("confsubtitleCalibrators1Label"));
        confsubtitleCalibrators1Label->setFont(font);
        confsubtitleCalibrators1Label->setFrameShape(QFrame::NoFrame);

        verticalLayout_4->addWidget(confsubtitleCalibrators1Label);

        confCalibrators1Frame = new QFrame(HDUpage);
        confCalibrators1Frame->setObjectName(QStringLiteral("confCalibrators1Frame"));
        sizePolicy2.setHeightForWidth(confCalibrators1Frame->sizePolicy().hasHeightForWidth());
        confCalibrators1Frame->setSizePolicy(sizePolicy2);
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette4.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette4.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette4.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush);
        palette4.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette4.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette4.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette4.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette4.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette4.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette4.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette4.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette4.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette4.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette4.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confCalibrators1Frame->setPalette(palette4);
        confCalibrators1Frame->setAutoFillBackground(true);
        confCalibrators1Frame->setFrameShape(QFrame::NoFrame);
        confCalibrators1Frame->setFrameShadow(QFrame::Plain);
        gridLayout_14 = new QGridLayout(confCalibrators1Frame);
        gridLayout_14->setObjectName(QStringLiteral("gridLayout_14"));
        label_6 = new QLabel(confCalibrators1Frame);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_14->addWidget(label_6, 0, 1, 1, 1);

        label_7 = new QLabel(confCalibrators1Frame);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_14->addWidget(label_7, 0, 2, 1, 1);

        label_22 = new QLabel(confCalibrators1Frame);
        label_22->setObjectName(QStringLiteral("label_22"));

        gridLayout_14->addWidget(label_22, 0, 3, 1, 1);

        label_3 = new QLabel(confCalibrators1Frame);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_14->addWidget(label_3, 2, 0, 1, 1);

        biasNlowLineEdit = new QLineEdit(confCalibrators1Frame);
        biasNlowLineEdit->setObjectName(QStringLiteral("biasNlowLineEdit"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(biasNlowLineEdit->sizePolicy().hasHeightForWidth());
        biasNlowLineEdit->setSizePolicy(sizePolicy3);
        biasNlowLineEdit->setMinimumSize(QSize(60, 0));
        biasNlowLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_14->addWidget(biasNlowLineEdit, 2, 1, 1, 1);

        biasNhighLineEdit = new QLineEdit(confCalibrators1Frame);
        biasNhighLineEdit->setObjectName(QStringLiteral("biasNhighLineEdit"));
        sizePolicy3.setHeightForWidth(biasNhighLineEdit->sizePolicy().hasHeightForWidth());
        biasNhighLineEdit->setSizePolicy(sizePolicy3);
        biasNhighLineEdit->setMinimumSize(QSize(60, 0));
        biasNhighLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_14->addWidget(biasNhighLineEdit, 2, 2, 1, 1);

        biasMethodComboBox = new QComboBox(confCalibrators1Frame);
        biasMethodComboBox->setObjectName(QStringLiteral("biasMethodComboBox"));
        biasMethodComboBox->setFocusPolicy(Qt::NoFocus);

        gridLayout_14->addWidget(biasMethodComboBox, 2, 3, 1, 1);

        label_4 = new QLabel(confCalibrators1Frame);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_14->addWidget(label_4, 3, 0, 1, 1);

        darkNlowLineEdit = new QLineEdit(confCalibrators1Frame);
        darkNlowLineEdit->setObjectName(QStringLiteral("darkNlowLineEdit"));
        sizePolicy3.setHeightForWidth(darkNlowLineEdit->sizePolicy().hasHeightForWidth());
        darkNlowLineEdit->setSizePolicy(sizePolicy3);
        darkNlowLineEdit->setMinimumSize(QSize(60, 0));
        darkNlowLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_14->addWidget(darkNlowLineEdit, 3, 1, 1, 1);

        darkNhighLineEdit = new QLineEdit(confCalibrators1Frame);
        darkNhighLineEdit->setObjectName(QStringLiteral("darkNhighLineEdit"));
        sizePolicy3.setHeightForWidth(darkNhighLineEdit->sizePolicy().hasHeightForWidth());
        darkNhighLineEdit->setSizePolicy(sizePolicy3);
        darkNhighLineEdit->setMinimumSize(QSize(60, 0));
        darkNhighLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_14->addWidget(darkNhighLineEdit, 3, 2, 1, 1);

        darkMethodComboBox = new QComboBox(confCalibrators1Frame);
        darkMethodComboBox->setObjectName(QStringLiteral("darkMethodComboBox"));
        darkMethodComboBox->setFocusPolicy(Qt::NoFocus);

        gridLayout_14->addWidget(darkMethodComboBox, 3, 3, 1, 1);

        label_15 = new QLabel(confCalibrators1Frame);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout_14->addWidget(label_15, 4, 0, 1, 1);

        flatoffNlowLineEdit = new QLineEdit(confCalibrators1Frame);
        flatoffNlowLineEdit->setObjectName(QStringLiteral("flatoffNlowLineEdit"));
        sizePolicy3.setHeightForWidth(flatoffNlowLineEdit->sizePolicy().hasHeightForWidth());
        flatoffNlowLineEdit->setSizePolicy(sizePolicy3);
        flatoffNlowLineEdit->setMinimumSize(QSize(60, 0));
        flatoffNlowLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_14->addWidget(flatoffNlowLineEdit, 4, 1, 1, 1);

        flatoffNhighLineEdit = new QLineEdit(confCalibrators1Frame);
        flatoffNhighLineEdit->setObjectName(QStringLiteral("flatoffNhighLineEdit"));
        sizePolicy3.setHeightForWidth(flatoffNhighLineEdit->sizePolicy().hasHeightForWidth());
        flatoffNhighLineEdit->setSizePolicy(sizePolicy3);
        flatoffNhighLineEdit->setMinimumSize(QSize(60, 0));
        flatoffNhighLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_14->addWidget(flatoffNhighLineEdit, 4, 2, 1, 1);

        flatoffMethodComboBox = new QComboBox(confCalibrators1Frame);
        flatoffMethodComboBox->setObjectName(QStringLiteral("flatoffMethodComboBox"));
        flatoffMethodComboBox->setFocusPolicy(Qt::NoFocus);

        gridLayout_14->addWidget(flatoffMethodComboBox, 4, 3, 1, 1);

        label_5 = new QLabel(confCalibrators1Frame);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_14->addWidget(label_5, 5, 0, 1, 1);

        flatNlowLineEdit = new QLineEdit(confCalibrators1Frame);
        flatNlowLineEdit->setObjectName(QStringLiteral("flatNlowLineEdit"));
        sizePolicy3.setHeightForWidth(flatNlowLineEdit->sizePolicy().hasHeightForWidth());
        flatNlowLineEdit->setSizePolicy(sizePolicy3);
        flatNlowLineEdit->setMinimumSize(QSize(60, 0));
        flatNlowLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_14->addWidget(flatNlowLineEdit, 5, 1, 1, 1);

        flatNhighLineEdit = new QLineEdit(confCalibrators1Frame);
        flatNhighLineEdit->setObjectName(QStringLiteral("flatNhighLineEdit"));
        sizePolicy3.setHeightForWidth(flatNhighLineEdit->sizePolicy().hasHeightForWidth());
        flatNhighLineEdit->setSizePolicy(sizePolicy3);
        flatNhighLineEdit->setMinimumSize(QSize(60, 0));
        flatNhighLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_14->addWidget(flatNhighLineEdit, 5, 2, 1, 1);

        flatMethodComboBox = new QComboBox(confCalibrators1Frame);
        flatMethodComboBox->setObjectName(QStringLiteral("flatMethodComboBox"));
        flatMethodComboBox->setFocusPolicy(Qt::NoFocus);

        gridLayout_14->addWidget(flatMethodComboBox, 5, 3, 1, 1);


        verticalLayout_4->addWidget(confCalibrators1Frame);

        confsubtitleCalibrators2Label = new QLabel(HDUpage);
        confsubtitleCalibrators2Label->setObjectName(QStringLiteral("confsubtitleCalibrators2Label"));
        confsubtitleCalibrators2Label->setFont(font);
        confsubtitleCalibrators2Label->setFrameShape(QFrame::NoFrame);

        verticalLayout_4->addWidget(confsubtitleCalibrators2Label);

        confCalibrators2Frame = new QFrame(HDUpage);
        confCalibrators2Frame->setObjectName(QStringLiteral("confCalibrators2Frame"));
        sizePolicy2.setHeightForWidth(confCalibrators2Frame->sizePolicy().hasHeightForWidth());
        confCalibrators2Frame->setSizePolicy(sizePolicy2);
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette5.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette5.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette5.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette5.setBrush(QPalette::Active, QPalette::Text, brush);
        palette5.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette5.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette5.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette5.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette5.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette5.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette5.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette5.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette5.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette5.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette5.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette5.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette5.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette5.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette5.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette5.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette5.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette5.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette5.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette5.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette5.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette5.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confCalibrators2Frame->setPalette(palette5);
        confCalibrators2Frame->setAutoFillBackground(true);
        confCalibrators2Frame->setFrameShape(QFrame::NoFrame);
        gridLayout_15 = new QGridLayout(confCalibrators2Frame);
        gridLayout_15->setObjectName(QStringLiteral("gridLayout_15"));
        label_8 = new QLabel(confCalibrators2Frame);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_15->addWidget(label_8, 0, 1, 1, 1);

        label_9 = new QLabel(confCalibrators2Frame);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_15->addWidget(label_9, 0, 2, 1, 1);

        label_11 = new QLabel(confCalibrators2Frame);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_15->addWidget(label_11, 1, 0, 1, 1);

        biasMinLineEdit = new QLineEdit(confCalibrators2Frame);
        biasMinLineEdit->setObjectName(QStringLiteral("biasMinLineEdit"));
        biasMinLineEdit->setMinimumSize(QSize(0, 0));
        biasMinLineEdit->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_15->addWidget(biasMinLineEdit, 1, 1, 1, 1);

        biasMaxLineEdit = new QLineEdit(confCalibrators2Frame);
        biasMaxLineEdit->setObjectName(QStringLiteral("biasMaxLineEdit"));
        biasMaxLineEdit->setMinimumSize(QSize(0, 0));
        biasMaxLineEdit->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_15->addWidget(biasMaxLineEdit, 1, 2, 1, 1);

        label_12 = new QLabel(confCalibrators2Frame);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_15->addWidget(label_12, 2, 0, 1, 1);

        darkMinLineEdit = new QLineEdit(confCalibrators2Frame);
        darkMinLineEdit->setObjectName(QStringLiteral("darkMinLineEdit"));
        darkMinLineEdit->setMinimumSize(QSize(0, 0));
        darkMinLineEdit->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_15->addWidget(darkMinLineEdit, 2, 1, 1, 1);

        darkMaxLineEdit = new QLineEdit(confCalibrators2Frame);
        darkMaxLineEdit->setObjectName(QStringLiteral("darkMaxLineEdit"));
        darkMaxLineEdit->setMinimumSize(QSize(0, 0));
        darkMaxLineEdit->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_15->addWidget(darkMaxLineEdit, 2, 2, 1, 1);

        label_16 = new QLabel(confCalibrators2Frame);
        label_16->setObjectName(QStringLiteral("label_16"));

        gridLayout_15->addWidget(label_16, 3, 0, 1, 1);

        flatoffMinLineEdit = new QLineEdit(confCalibrators2Frame);
        flatoffMinLineEdit->setObjectName(QStringLiteral("flatoffMinLineEdit"));
        flatoffMinLineEdit->setMinimumSize(QSize(0, 0));
        flatoffMinLineEdit->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_15->addWidget(flatoffMinLineEdit, 3, 1, 1, 1);

        flatoffMaxLineEdit = new QLineEdit(confCalibrators2Frame);
        flatoffMaxLineEdit->setObjectName(QStringLiteral("flatoffMaxLineEdit"));
        flatoffMaxLineEdit->setMinimumSize(QSize(0, 0));
        flatoffMaxLineEdit->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_15->addWidget(flatoffMaxLineEdit, 3, 2, 1, 1);

        flatMinLineEdit = new QLineEdit(confCalibrators2Frame);
        flatMinLineEdit->setObjectName(QStringLiteral("flatMinLineEdit"));
        flatMinLineEdit->setMinimumSize(QSize(0, 0));
        flatMinLineEdit->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_15->addWidget(flatMinLineEdit, 4, 1, 1, 1);

        flatMaxLineEdit = new QLineEdit(confCalibrators2Frame);
        flatMaxLineEdit->setObjectName(QStringLiteral("flatMaxLineEdit"));
        flatMaxLineEdit->setMinimumSize(QSize(0, 0));
        flatMaxLineEdit->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_15->addWidget(flatMaxLineEdit, 4, 2, 1, 1);

        label_13 = new QLabel(confCalibrators2Frame);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_15->addWidget(label_13, 4, 0, 1, 1);


        verticalLayout_4->addWidget(confCalibrators2Frame);

        verticalSpacer_34 = new QSpacerItem(20, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_34);


        gridLayout_31->addLayout(verticalLayout_4, 0, 1, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(182, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_31->addItem(horizontalSpacer_7, 0, 2, 1, 1);

        confStackedWidget->addWidget(HDUpage);
        SPSpage = new QWidget();
        SPSpage->setObjectName(QStringLiteral("SPSpage"));
        gridLayout_32 = new QGridLayout(SPSpage);
        gridLayout_32->setObjectName(QStringLiteral("gridLayout_32"));
        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setSpacing(0);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        conftitleChopnodLabel = new QLabel(SPSpage);
        conftitleChopnodLabel->setObjectName(QStringLiteral("conftitleChopnodLabel"));
        sizePolicy1.setHeightForWidth(conftitleChopnodLabel->sizePolicy().hasHeightForWidth());
        conftitleChopnodLabel->setSizePolicy(sizePolicy1);
        conftitleChopnodLabel->setFont(font);
        conftitleChopnodLabel->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
"color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleChopnodLabel->setWordWrap(true);

        verticalLayout_14->addWidget(conftitleChopnodLabel);

        confChopnodFrame = new QFrame(SPSpage);
        confChopnodFrame->setObjectName(QStringLiteral("confChopnodFrame"));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette6.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette6.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette6.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette6.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette6.setBrush(QPalette::Active, QPalette::Text, brush);
        palette6.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette6.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette6.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette6.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette6.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette6.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette6.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette6.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette6.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette6.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette6.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette6.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette6.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette6.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette6.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette6.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette6.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette6.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette6.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette6.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette6.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette6.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette6.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette6.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette6.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confChopnodFrame->setPalette(palette6);
        confChopnodFrame->setAutoFillBackground(true);
        confChopnodFrame->setFrameShape(QFrame::NoFrame);
        confChopnodFrame->setFrameShadow(QFrame::Plain);
        gridLayout_28 = new QGridLayout(confChopnodFrame);
        gridLayout_28->setObjectName(QStringLiteral("gridLayout_28"));
        label_59 = new QLabel(confChopnodFrame);
        label_59->setObjectName(QStringLiteral("label_59"));

        gridLayout_28->addWidget(label_59, 0, 0, 1, 1);

        chopnodComboBox = new QComboBox(confChopnodFrame);
        chopnodComboBox->setObjectName(QStringLiteral("chopnodComboBox"));
        chopnodComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_28->addWidget(chopnodComboBox, 0, 1, 1, 1);

        chopnodInvertCheckBox = new QCheckBox(confChopnodFrame);
        chopnodInvertCheckBox->setObjectName(QStringLiteral("chopnodInvertCheckBox"));
        chopnodInvertCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_28->addWidget(chopnodInvertCheckBox, 1, 0, 1, 2);


        verticalLayout_14->addWidget(confChopnodFrame);

        confChopnodSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_14->addItem(confChopnodSpacer);

        conftitleBackground1Label = new QLabel(SPSpage);
        conftitleBackground1Label->setObjectName(QStringLiteral("conftitleBackground1Label"));
        sizePolicy1.setHeightForWidth(conftitleBackground1Label->sizePolicy().hasHeightForWidth());
        conftitleBackground1Label->setSizePolicy(sizePolicy1);
        conftitleBackground1Label->setFont(font);
        conftitleBackground1Label->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
"color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleBackground1Label->setWordWrap(true);

        verticalLayout_14->addWidget(conftitleBackground1Label);

        confsubtitleBackground1Label = new QLabel(SPSpage);
        confsubtitleBackground1Label->setObjectName(QStringLiteral("confsubtitleBackground1Label"));
        confsubtitleBackground1Label->setFont(font);

        verticalLayout_14->addWidget(confsubtitleBackground1Label);

        confBackground1Frame = new QFrame(SPSpage);
        confBackground1Frame->setObjectName(QStringLiteral("confBackground1Frame"));
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette7.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette7.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette7.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette7.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette7.setBrush(QPalette::Active, QPalette::Text, brush);
        palette7.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette7.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette7.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette7.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette7.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette7.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette7.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette7.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette7.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette7.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette7.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette7.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette7.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette7.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette7.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette7.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette7.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette7.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette7.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette7.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette7.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette7.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette7.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette7.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette7.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confBackground1Frame->setPalette(palette7);
        confBackground1Frame->setAutoFillBackground(true);
        confBackground1Frame->setFrameShape(QFrame::NoFrame);
        confBackground1Frame->setFrameShadow(QFrame::Plain);
        gridLayout_21 = new QGridLayout(confBackground1Frame);
        gridLayout_21->setObjectName(QStringLiteral("gridLayout_21"));
        label = new QLabel(confBackground1Frame);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_21->addWidget(label, 0, 0, 1, 1);

        SPSnumbergroupsLineEdit = new QLineEdit(confBackground1Frame);
        SPSnumbergroupsLineEdit->setObjectName(QStringLiteral("SPSnumbergroupsLineEdit"));
        SPSnumbergroupsLineEdit->setMinimumSize(QSize(60, 0));
        SPSnumbergroupsLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_21->addWidget(SPSnumbergroupsLineEdit, 0, 1, 1, 1);

        label_10 = new QLabel(confBackground1Frame);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_21->addWidget(label_10, 1, 0, 1, 1);

        SPSlengthLineEdit = new QLineEdit(confBackground1Frame);
        SPSlengthLineEdit->setObjectName(QStringLiteral("SPSlengthLineEdit"));
        SPSlengthLineEdit->setMinimumSize(QSize(60, 0));
        SPSlengthLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_21->addWidget(SPSlengthLineEdit, 1, 1, 1, 1);

        label_24 = new QLabel(confBackground1Frame);
        label_24->setObjectName(QStringLiteral("label_24"));

        gridLayout_21->addWidget(label_24, 2, 0, 1, 1);

        SPSexcludeLineEdit = new QLineEdit(confBackground1Frame);
        SPSexcludeLineEdit->setObjectName(QStringLiteral("SPSexcludeLineEdit"));
        SPSexcludeLineEdit->setMinimumSize(QSize(60, 0));
        SPSexcludeLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_21->addWidget(SPSexcludeLineEdit, 2, 1, 1, 1);


        verticalLayout_14->addWidget(confBackground1Frame);

        confsubtitleBackground2Label = new QLabel(SPSpage);
        confsubtitleBackground2Label->setObjectName(QStringLiteral("confsubtitleBackground2Label"));
        confsubtitleBackground2Label->setFont(font);

        verticalLayout_14->addWidget(confsubtitleBackground2Label);

        confBackground2Frame = new QFrame(SPSpage);
        confBackground2Frame->setObjectName(QStringLiteral("confBackground2Frame"));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette8.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette8.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette8.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette8.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette8.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette8.setBrush(QPalette::Active, QPalette::Text, brush);
        palette8.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette8.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette8.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette8.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette8.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette8.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette8.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette8.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette8.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette8.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette8.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette8.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette8.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette8.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette8.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette8.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette8.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette8.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette8.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette8.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette8.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette8.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette8.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette8.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette8.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette8.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette8.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette8.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette8.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette8.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette8.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confBackground2Frame->setPalette(palette8);
        confBackground2Frame->setAutoFillBackground(true);
        confBackground2Frame->setFrameShape(QFrame::NoFrame);
        confBackground2Frame->setFrameShadow(QFrame::Plain);
        gridLayout_8 = new QGridLayout(confBackground2Frame);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        formLayout_10 = new QFormLayout();
        formLayout_10->setObjectName(QStringLiteral("formLayout_10"));
        BACmefLabel = new QLabel(confBackground2Frame);
        BACmefLabel->setObjectName(QStringLiteral("BACmefLabel"));

        formLayout_10->setWidget(0, QFormLayout::LabelRole, BACmefLabel);

        BACmefLineEdit = new QLineEdit(confBackground2Frame);
        BACmefLineEdit->setObjectName(QStringLiteral("BACmefLineEdit"));
        BACmefLineEdit->setMinimumSize(QSize(80, 0));
        BACmefLineEdit->setMaximumSize(QSize(80, 16777215));

        formLayout_10->setWidget(0, QFormLayout::FieldRole, BACmefLineEdit);

        BACmethodLabel = new QLabel(confBackground2Frame);
        BACmethodLabel->setObjectName(QStringLiteral("BACmethodLabel"));

        formLayout_10->setWidget(1, QFormLayout::LabelRole, BACmethodLabel);

        BACmethodComboBox = new QComboBox(confBackground2Frame);
        BACmethodComboBox->setObjectName(QStringLiteral("BACmethodComboBox"));
        BACmethodComboBox->setFocusPolicy(Qt::ClickFocus);

        formLayout_10->setWidget(1, QFormLayout::FieldRole, BACmethodComboBox);


        gridLayout_8->addLayout(formLayout_10, 2, 0, 1, 2);

        BAC2passCheckBox = new QCheckBox(confBackground2Frame);
        BAC2passCheckBox->setObjectName(QStringLiteral("BAC2passCheckBox"));
        BAC2passCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_8->addWidget(BAC2passCheckBox, 0, 1, 1, 1);

        BACconvolutionCheckBox = new QCheckBox(confBackground2Frame);
        BACconvolutionCheckBox->setObjectName(QStringLiteral("BACconvolutionCheckBox"));
        BACconvolutionCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_8->addWidget(BACconvolutionCheckBox, 1, 1, 1, 1);

        formLayout_9 = new QFormLayout();
        formLayout_9->setObjectName(QStringLiteral("formLayout_9"));
        BACDTLabel = new QLabel(confBackground2Frame);
        BACDTLabel->setObjectName(QStringLiteral("BACDTLabel"));

        formLayout_9->setWidget(0, QFormLayout::LabelRole, BACDTLabel);

        BACDTLineEdit = new QLineEdit(confBackground2Frame);
        BACDTLineEdit->setObjectName(QStringLiteral("BACDTLineEdit"));
        BACDTLineEdit->setMinimumSize(QSize(80, 0));
        BACDTLineEdit->setMaximumSize(QSize(80, 16777215));

        formLayout_9->setWidget(0, QFormLayout::FieldRole, BACDTLineEdit);

        BACDMINLabel = new QLabel(confBackground2Frame);
        BACDMINLabel->setObjectName(QStringLiteral("BACDMINLabel"));

        formLayout_9->setWidget(1, QFormLayout::LabelRole, BACDMINLabel);

        BACDMINLineEdit = new QLineEdit(confBackground2Frame);
        BACDMINLineEdit->setObjectName(QStringLiteral("BACDMINLineEdit"));
        BACDMINLineEdit->setMinimumSize(QSize(80, 0));
        BACDMINLineEdit->setMaximumSize(QSize(80, 16777215));

        formLayout_9->setWidget(1, QFormLayout::FieldRole, BACDMINLineEdit);


        gridLayout_8->addLayout(formLayout_9, 0, 0, 2, 1);


        verticalLayout_14->addWidget(confBackground2Frame);

        confsubtitleBackground3Label = new QLabel(SPSpage);
        confsubtitleBackground3Label->setObjectName(QStringLiteral("confsubtitleBackground3Label"));
        confsubtitleBackground3Label->setFont(font);

        verticalLayout_14->addWidget(confsubtitleBackground3Label);

        confBackground3Frame = new QFrame(SPSpage);
        confBackground3Frame->setObjectName(QStringLiteral("confBackground3Frame"));
        QPalette palette9;
        palette9.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette9.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette9.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette9.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette9.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette9.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette9.setBrush(QPalette::Active, QPalette::Text, brush);
        palette9.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette9.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette9.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette9.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette9.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette9.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette9.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette9.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette9.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette9.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette9.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette9.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette9.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette9.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette9.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette9.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette9.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette9.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette9.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette9.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette9.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette9.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette9.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette9.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette9.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette9.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette9.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette9.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette9.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette9.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette9.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette9.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette9.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette9.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confBackground3Frame->setPalette(palette9);
        confBackground3Frame->setAutoFillBackground(true);
        confBackground3Frame->setFrameShape(QFrame::NoFrame);
        confBackground3Frame->setFrameShadow(QFrame::Plain);
        gridLayout_9 = new QGridLayout(confBackground3Frame);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        label_18 = new QLabel(confBackground3Frame);
        label_18->setObjectName(QStringLiteral("label_18"));

        gridLayout_9->addWidget(label_18, 0, 1, 1, 1);

        label_19 = new QLabel(confBackground3Frame);
        label_19->setObjectName(QStringLiteral("label_19"));

        gridLayout_9->addWidget(label_19, 0, 2, 1, 1);

        conf1passLabel = new QLabel(confBackground3Frame);
        conf1passLabel->setObjectName(QStringLiteral("conf1passLabel"));
        sizePolicy.setHeightForWidth(conf1passLabel->sizePolicy().hasHeightForWidth());
        conf1passLabel->setSizePolicy(sizePolicy);

        gridLayout_9->addWidget(conf1passLabel, 1, 0, 1, 1);

        BAC1nlowLineEdit = new QLineEdit(confBackground3Frame);
        BAC1nlowLineEdit->setObjectName(QStringLiteral("BAC1nlowLineEdit"));
        BAC1nlowLineEdit->setMinimumSize(QSize(80, 0));
        BAC1nlowLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_9->addWidget(BAC1nlowLineEdit, 1, 1, 1, 1);

        BAC1nhighLineEdit = new QLineEdit(confBackground3Frame);
        BAC1nhighLineEdit->setObjectName(QStringLiteral("BAC1nhighLineEdit"));
        BAC1nhighLineEdit->setMinimumSize(QSize(80, 0));
        BAC1nhighLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_9->addWidget(BAC1nhighLineEdit, 1, 2, 1, 1);

        conf2passLabel = new QLabel(confBackground3Frame);
        conf2passLabel->setObjectName(QStringLiteral("conf2passLabel"));
        sizePolicy.setHeightForWidth(conf2passLabel->sizePolicy().hasHeightForWidth());
        conf2passLabel->setSizePolicy(sizePolicy);

        gridLayout_9->addWidget(conf2passLabel, 2, 0, 1, 1);

        BAC2nlowLineEdit = new QLineEdit(confBackground3Frame);
        BAC2nlowLineEdit->setObjectName(QStringLiteral("BAC2nlowLineEdit"));
        BAC2nlowLineEdit->setMinimumSize(QSize(80, 0));
        BAC2nlowLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_9->addWidget(BAC2nlowLineEdit, 2, 1, 1, 1);

        BAC2nhighLineEdit = new QLineEdit(confBackground3Frame);
        BAC2nhighLineEdit->setObjectName(QStringLiteral("BAC2nhighLineEdit"));
        BAC2nhighLineEdit->setMinimumSize(QSize(80, 0));
        BAC2nhighLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_9->addWidget(BAC2nhighLineEdit, 2, 2, 1, 1);


        verticalLayout_14->addWidget(confBackground3Frame);

        verticalSpacer_35 = new QSpacerItem(20, 22, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_14->addItem(verticalSpacer_35);


        gridLayout_32->addLayout(verticalLayout_14, 0, 0, 1, 1);

        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setSpacing(0);
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        conftitleBackground2Label = new QLabel(SPSpage);
        conftitleBackground2Label->setObjectName(QStringLiteral("conftitleBackground2Label"));
        sizePolicy1.setHeightForWidth(conftitleBackground2Label->sizePolicy().hasHeightForWidth());
        conftitleBackground2Label->setSizePolicy(sizePolicy1);
        conftitleBackground2Label->setFont(font);
        conftitleBackground2Label->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
"color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleBackground2Label->setWordWrap(true);

        verticalLayout_15->addWidget(conftitleBackground2Label);

        confsubtitleBackground4Label = new QLabel(SPSpage);
        confsubtitleBackground4Label->setObjectName(QStringLiteral("confsubtitleBackground4Label"));
        confsubtitleBackground4Label->setFont(font);

        verticalLayout_15->addWidget(confsubtitleBackground4Label);

        confBackground4Frame = new QFrame(SPSpage);
        confBackground4Frame->setObjectName(QStringLiteral("confBackground4Frame"));
        QPalette palette10;
        palette10.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette10.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette10.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette10.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette10.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette10.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette10.setBrush(QPalette::Active, QPalette::Text, brush);
        palette10.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette10.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette10.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette10.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette10.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette10.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette10.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette10.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette10.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette10.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette10.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette10.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette10.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette10.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette10.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette10.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette10.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette10.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette10.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette10.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette10.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette10.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette10.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette10.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette10.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette10.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette10.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette10.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette10.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette10.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette10.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette10.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette10.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette10.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confBackground4Frame->setPalette(palette10);
        confBackground4Frame->setAutoFillBackground(true);
        confBackground4Frame->setFrameShape(QFrame::NoFrame);
        confBackground4Frame->setFrameShadow(QFrame::Plain);
        gridLayout_11 = new QGridLayout(confBackground4Frame);
        gridLayout_11->setObjectName(QStringLiteral("gridLayout_11"));
        confBrightstarMagLabel = new QLabel(confBackground4Frame);
        confBrightstarMagLabel->setObjectName(QStringLiteral("confBrightstarMagLabel"));

        gridLayout_11->addWidget(confBrightstarMagLabel, 0, 0, 1, 1);

        BACmagLineEdit = new QLineEdit(confBackground4Frame);
        BACmagLineEdit->setObjectName(QStringLiteral("BACmagLineEdit"));
        BACmagLineEdit->setMinimumSize(QSize(80, 0));
        BACmagLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_11->addWidget(BACmagLineEdit, 0, 1, 1, 1);

        confBrightstarDistanceLabel = new QLabel(confBackground4Frame);
        confBrightstarDistanceLabel->setObjectName(QStringLiteral("confBrightstarDistanceLabel"));

        gridLayout_11->addWidget(confBrightstarDistanceLabel, 1, 0, 1, 1);

        BACdistLineEdit = new QLineEdit(confBackground4Frame);
        BACdistLineEdit->setObjectName(QStringLiteral("BACdistLineEdit"));
        BACdistLineEdit->setMinimumSize(QSize(80, 0));
        BACdistLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_11->addWidget(BACdistLineEdit, 1, 1, 1, 1);


        verticalLayout_15->addWidget(confBackground4Frame);

        confsubtitleBackground5Label = new QLabel(SPSpage);
        confsubtitleBackground5Label->setObjectName(QStringLiteral("confsubtitleBackground5Label"));
        confsubtitleBackground5Label->setFont(font);

        verticalLayout_15->addWidget(confsubtitleBackground5Label);

        confBackground5Frame = new QFrame(SPSpage);
        confBackground5Frame->setObjectName(QStringLiteral("confBackground5Frame"));
        QPalette palette11;
        palette11.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette11.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette11.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette11.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette11.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette11.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette11.setBrush(QPalette::Active, QPalette::Text, brush);
        palette11.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette11.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette11.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette11.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette11.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette11.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette11.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette11.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette11.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette11.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette11.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette11.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette11.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette11.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette11.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette11.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette11.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette11.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette11.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette11.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette11.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette11.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette11.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette11.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette11.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette11.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette11.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette11.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette11.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette11.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette11.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette11.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette11.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette11.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confBackground5Frame->setPalette(palette11);
        confBackground5Frame->setAutoFillBackground(true);
        confBackground5Frame->setFrameShape(QFrame::NoFrame);
        confBackground5Frame->setFrameShadow(QFrame::Plain);
        gridLayout_41 = new QGridLayout(confBackground5Frame);
        gridLayout_41->setObjectName(QStringLiteral("gridLayout_41"));
        BACapplyComboBox = new QComboBox(confBackground5Frame);
        BACapplyComboBox->setObjectName(QStringLiteral("BACapplyComboBox"));
        QSizePolicy sizePolicy4(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(BACapplyComboBox->sizePolicy().hasHeightForWidth());
        BACapplyComboBox->setSizePolicy(sizePolicy4);
        BACapplyComboBox->setFocusPolicy(Qt::ClickFocus);
        BACapplyComboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);

        gridLayout_41->addWidget(BACapplyComboBox, 0, 0, 1, 1);

        label_14 = new QLabel(confBackground5Frame);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_41->addWidget(label_14, 1, 0, 1, 1);

        BACbacksmoothscaleLineEdit = new QLineEdit(confBackground5Frame);
        BACbacksmoothscaleLineEdit->setObjectName(QStringLiteral("BACbacksmoothscaleLineEdit"));
        BACbacksmoothscaleLineEdit->setMinimumSize(QSize(80, 0));
        BACbacksmoothscaleLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_41->addWidget(BACbacksmoothscaleLineEdit, 1, 1, 1, 1);

        BACrescaleCheckBox = new QCheckBox(confBackground5Frame);
        BACrescaleCheckBox->setObjectName(QStringLiteral("BACrescaleCheckBox"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(BACrescaleCheckBox->sizePolicy().hasHeightForWidth());
        BACrescaleCheckBox->setSizePolicy(sizePolicy5);
        BACrescaleCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_41->addWidget(BACrescaleCheckBox, 2, 0, 1, 1);


        verticalLayout_15->addWidget(confBackground5Frame);

        confsubtitleBackground6Label = new QLabel(SPSpage);
        confsubtitleBackground6Label->setObjectName(QStringLiteral("confsubtitleBackground6Label"));
        confsubtitleBackground6Label->setFont(font);

        verticalLayout_15->addWidget(confsubtitleBackground6Label);

        confBackground6Frame = new QFrame(SPSpage);
        confBackground6Frame->setObjectName(QStringLiteral("confBackground6Frame"));
        QPalette palette12;
        palette12.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette12.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette12.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette12.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette12.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette12.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette12.setBrush(QPalette::Active, QPalette::Text, brush);
        palette12.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette12.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette12.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette12.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette12.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette12.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette12.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette12.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette12.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette12.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette12.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette12.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette12.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette12.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette12.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette12.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette12.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette12.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette12.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette12.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette12.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette12.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette12.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette12.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette12.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette12.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette12.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette12.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette12.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette12.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette12.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette12.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette12.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette12.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confBackground6Frame->setPalette(palette12);
        confBackground6Frame->setAutoFillBackground(true);
        confBackground6Frame->setFrameShape(QFrame::NoFrame);
        confBackground6Frame->setFrameShadow(QFrame::Plain);
        gridLayout_42 = new QGridLayout(confBackground6Frame);
        gridLayout_42->setObjectName(QStringLiteral("gridLayout_42"));
        BACcurrentmodeLabel = new QLabel(confBackground6Frame);
        BACcurrentmodeLabel->setObjectName(QStringLiteral("BACcurrentmodeLabel"));

        gridLayout_42->addWidget(BACcurrentmodeLabel, 3, 0, 1, 2);

        BACgapsizeLabel = new QLabel(confBackground6Frame);
        BACgapsizeLabel->setObjectName(QStringLiteral("BACgapsizeLabel"));

        gridLayout_42->addWidget(BACgapsizeLabel, 2, 0, 1, 1);

        BACgapsizeLineEdit = new QLineEdit(confBackground6Frame);
        BACgapsizeLineEdit->setObjectName(QStringLiteral("BACgapsizeLineEdit"));
        BACgapsizeLineEdit->setMinimumSize(QSize(80, 0));
        BACgapsizeLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_42->addWidget(BACgapsizeLineEdit, 2, 1, 1, 1);

        BACwindowLabel = new QLabel(confBackground6Frame);
        BACwindowLabel->setObjectName(QStringLiteral("BACwindowLabel"));

        gridLayout_42->addWidget(BACwindowLabel, 0, 0, 1, 1);

        BACwindowLineEdit = new QLineEdit(confBackground6Frame);
        BACwindowLineEdit->setObjectName(QStringLiteral("BACwindowLineEdit"));
        BACwindowLineEdit->setMinimumSize(QSize(80, 0));
        BACwindowLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_42->addWidget(BACwindowLineEdit, 0, 1, 1, 1);

        BACminWindowLineEdit = new QLineEdit(confBackground6Frame);
        BACminWindowLineEdit->setObjectName(QStringLiteral("BACminWindowLineEdit"));
        BACminWindowLineEdit->setMinimumSize(QSize(80, 0));
        BACminWindowLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_42->addWidget(BACminWindowLineEdit, 1, 1, 1, 1);

        label_23 = new QLabel(confBackground6Frame);
        label_23->setObjectName(QStringLiteral("label_23"));

        gridLayout_42->addWidget(label_23, 1, 0, 1, 1);


        verticalLayout_15->addWidget(confBackground6Frame);

        verticalSpacer_12 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_15->addItem(verticalSpacer_12);

        verticalSpacer_36 = new QSpacerItem(17, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_15->addItem(verticalSpacer_36);


        gridLayout_32->addLayout(verticalLayout_15, 0, 1, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(399, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_32->addItem(horizontalSpacer_8, 0, 2, 1, 1);

        confStackedWidget->addWidget(SPSpage);
        COCpage = new QWidget();
        COCpage->setObjectName(QStringLiteral("COCpage"));
        gridLayout_59 = new QGridLayout(COCpage);
        gridLayout_59->setObjectName(QStringLiteral("gridLayout_59"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        conftitleCollapseLabel = new QLabel(COCpage);
        conftitleCollapseLabel->setObjectName(QStringLiteral("conftitleCollapseLabel"));
        sizePolicy1.setHeightForWidth(conftitleCollapseLabel->sizePolicy().hasHeightForWidth());
        conftitleCollapseLabel->setSizePolicy(sizePolicy1);
        conftitleCollapseLabel->setFont(font);
        conftitleCollapseLabel->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleCollapseLabel->setWordWrap(true);

        verticalLayout_5->addWidget(conftitleCollapseLabel);

        confsubtitleCollapse1Label = new QLabel(COCpage);
        confsubtitleCollapse1Label->setObjectName(QStringLiteral("confsubtitleCollapse1Label"));
        confsubtitleCollapse1Label->setFont(font);
        confsubtitleCollapse1Label->setWordWrap(true);

        verticalLayout_5->addWidget(confsubtitleCollapse1Label);

        confCollapse1Frame = new QFrame(COCpage);
        confCollapse1Frame->setObjectName(QStringLiteral("confCollapse1Frame"));
        QPalette palette13;
        palette13.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette13.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette13.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette13.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette13.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette13.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette13.setBrush(QPalette::Active, QPalette::Text, brush);
        palette13.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette13.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette13.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette13.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette13.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette13.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette13.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette13.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette13.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette13.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette13.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette13.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette13.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette13.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette13.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette13.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette13.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette13.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette13.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette13.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette13.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette13.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette13.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette13.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette13.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette13.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette13.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette13.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette13.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette13.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette13.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette13.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette13.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confCollapse1Frame->setPalette(palette13);
        confCollapse1Frame->setAutoFillBackground(true);
        confCollapse1Frame->setFrameShape(QFrame::NoFrame);
        confCollapse1Frame->setFrameShadow(QFrame::Plain);
        gridLayout_20 = new QGridLayout(confCollapse1Frame);
        gridLayout_20->setObjectName(QStringLiteral("gridLayout_20"));
        confCollapseDTLabel = new QLabel(confCollapse1Frame);
        confCollapseDTLabel->setObjectName(QStringLiteral("confCollapseDTLabel"));
        sizePolicy1.setHeightForWidth(confCollapseDTLabel->sizePolicy().hasHeightForWidth());
        confCollapseDTLabel->setSizePolicy(sizePolicy1);

        gridLayout_20->addWidget(confCollapseDTLabel, 0, 0, 1, 1);

        COCDTLineEdit = new QLineEdit(confCollapse1Frame);
        COCDTLineEdit->setObjectName(QStringLiteral("COCDTLineEdit"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(COCDTLineEdit->sizePolicy().hasHeightForWidth());
        COCDTLineEdit->setSizePolicy(sizePolicy6);
        COCDTLineEdit->setMinimumSize(QSize(80, 0));
        COCDTLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_20->addWidget(COCDTLineEdit, 0, 1, 1, 1);

        confCollapseDMINLabel = new QLabel(confCollapse1Frame);
        confCollapseDMINLabel->setObjectName(QStringLiteral("confCollapseDMINLabel"));
        sizePolicy1.setHeightForWidth(confCollapseDMINLabel->sizePolicy().hasHeightForWidth());
        confCollapseDMINLabel->setSizePolicy(sizePolicy1);

        gridLayout_20->addWidget(confCollapseDMINLabel, 1, 0, 1, 1);

        COCDMINLineEdit = new QLineEdit(confCollapse1Frame);
        COCDMINLineEdit->setObjectName(QStringLiteral("COCDMINLineEdit"));
        sizePolicy6.setHeightForWidth(COCDMINLineEdit->sizePolicy().hasHeightForWidth());
        COCDMINLineEdit->setSizePolicy(sizePolicy6);
        COCDMINLineEdit->setMinimumSize(QSize(80, 0));
        COCDMINLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_20->addWidget(COCDMINLineEdit, 1, 1, 1, 1);

        confCollapseMEFLabel = new QLabel(confCollapse1Frame);
        confCollapseMEFLabel->setObjectName(QStringLiteral("confCollapseMEFLabel"));
        confCollapseMEFLabel->setLayoutDirection(Qt::LeftToRight);
        confCollapseMEFLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_20->addWidget(confCollapseMEFLabel, 2, 0, 1, 1);

        COCmefLineEdit = new QLineEdit(confCollapse1Frame);
        COCmefLineEdit->setObjectName(QStringLiteral("COCmefLineEdit"));
        sizePolicy6.setHeightForWidth(COCmefLineEdit->sizePolicy().hasHeightForWidth());
        COCmefLineEdit->setSizePolicy(sizePolicy6);
        COCmefLineEdit->setMinimumSize(QSize(80, 0));
        COCmefLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_20->addWidget(COCmefLineEdit, 2, 1, 1, 1);


        verticalLayout_5->addWidget(confCollapse1Frame);

        confsubtitleCollapse2Label = new QLabel(COCpage);
        confsubtitleCollapse2Label->setObjectName(QStringLiteral("confsubtitleCollapse2Label"));
        confsubtitleCollapse2Label->setFont(font);

        verticalLayout_5->addWidget(confsubtitleCollapse2Label);

        confCollapse2Frame = new QFrame(COCpage);
        confCollapse2Frame->setObjectName(QStringLiteral("confCollapse2Frame"));
        QPalette palette14;
        palette14.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette14.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette14.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette14.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette14.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette14.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette14.setBrush(QPalette::Active, QPalette::Text, brush);
        palette14.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette14.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette14.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette14.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette14.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette14.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette14.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette14.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette14.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette14.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette14.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette14.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette14.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette14.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette14.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette14.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette14.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette14.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette14.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette14.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette14.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette14.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette14.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette14.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette14.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette14.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette14.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette14.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette14.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette14.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette14.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette14.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette14.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confCollapse2Frame->setPalette(palette14);
        confCollapse2Frame->setAutoFillBackground(true);
        confCollapse2Frame->setFrameShape(QFrame::NoFrame);
        confCollapse2Frame->setFrameShadow(QFrame::Plain);
        gridLayout_19 = new QGridLayout(confCollapse2Frame);
        gridLayout_19->setObjectName(QStringLiteral("gridLayout_19"));
        label_26 = new QLabel(confCollapse2Frame);
        label_26->setObjectName(QStringLiteral("label_26"));
        sizePolicy.setHeightForWidth(label_26->sizePolicy().hasHeightForWidth());
        label_26->setSizePolicy(sizePolicy);

        gridLayout_19->addWidget(label_26, 0, 0, 1, 2);

        COCrejectLineEdit = new QLineEdit(confCollapse2Frame);
        COCrejectLineEdit->setObjectName(QStringLiteral("COCrejectLineEdit"));
        COCrejectLineEdit->setMinimumSize(QSize(80, 0));
        COCrejectLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_19->addWidget(COCrejectLineEdit, 0, 2, 1, 1);

        label_27 = new QLabel(confCollapse2Frame);
        label_27->setObjectName(QStringLiteral("label_27"));
        sizePolicy.setHeightForWidth(label_27->sizePolicy().hasHeightForWidth());
        label_27->setSizePolicy(sizePolicy);

        gridLayout_19->addWidget(label_27, 1, 0, 1, 1);

        COCdirectionLabel = new QLabel(confCollapse2Frame);
        COCdirectionLabel->setObjectName(QStringLiteral("COCdirectionLabel"));
        sizePolicy3.setHeightForWidth(COCdirectionLabel->sizePolicy().hasHeightForWidth());
        COCdirectionLabel->setSizePolicy(sizePolicy3);
        COCdirectionLabel->setMinimumSize(QSize(44, 44));
        COCdirectionLabel->setMaximumSize(QSize(44, 44));

        gridLayout_19->addWidget(COCdirectionLabel, 1, 1, 1, 1);

        COCdirectionComboBox = new QComboBox(confCollapse2Frame);
        COCdirectionComboBox->setObjectName(QStringLiteral("COCdirectionComboBox"));
        COCdirectionComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_19->addWidget(COCdirectionComboBox, 1, 2, 1, 1);


        verticalLayout_5->addWidget(confCollapse2Frame);

        confsubtitleCollapse3Label = new QLabel(COCpage);
        confsubtitleCollapse3Label->setObjectName(QStringLiteral("confsubtitleCollapse3Label"));
        confsubtitleCollapse3Label->setFont(font);

        verticalLayout_5->addWidget(confsubtitleCollapse3Label);

        confCollapse3Frame = new QFrame(COCpage);
        confCollapse3Frame->setObjectName(QStringLiteral("confCollapse3Frame"));
        QPalette palette15;
        palette15.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette15.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette15.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette15.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette15.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette15.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette15.setBrush(QPalette::Active, QPalette::Text, brush);
        palette15.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette15.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette15.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette15.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette15.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette15.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette15.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette15.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette15.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette15.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette15.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette15.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette15.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette15.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette15.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette15.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette15.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette15.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette15.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette15.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette15.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette15.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette15.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette15.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette15.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette15.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette15.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette15.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette15.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette15.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette15.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette15.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette15.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette15.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette15.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette15.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette15.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette15.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confCollapse3Frame->setPalette(palette15);
        confCollapse3Frame->setAutoFillBackground(true);
        confCollapse3Frame->setFrameShape(QFrame::NoFrame);
        confCollapse3Frame->setFrameShadow(QFrame::Plain);
        gridLayout_18 = new QGridLayout(confCollapse3Frame);
        gridLayout_18->setObjectName(QStringLiteral("gridLayout_18"));
        confCollapseExcludeXLabel_3 = new QLabel(confCollapse3Frame);
        confCollapseExcludeXLabel_3->setObjectName(QStringLiteral("confCollapseExcludeXLabel_3"));
        sizePolicy.setHeightForWidth(confCollapseExcludeXLabel_3->sizePolicy().hasHeightForWidth());
        confCollapseExcludeXLabel_3->setSizePolicy(sizePolicy);

        gridLayout_18->addWidget(confCollapseExcludeXLabel_3, 0, 1, 1, 1);

        confCollapseExcludeXLabel_2 = new QLabel(confCollapse3Frame);
        confCollapseExcludeXLabel_2->setObjectName(QStringLiteral("confCollapseExcludeXLabel_2"));
        sizePolicy.setHeightForWidth(confCollapseExcludeXLabel_2->sizePolicy().hasHeightForWidth());
        confCollapseExcludeXLabel_2->setSizePolicy(sizePolicy);

        gridLayout_18->addWidget(confCollapseExcludeXLabel_2, 0, 2, 1, 1);

        confCollapseExcludeXLabel = new QLabel(confCollapse3Frame);
        confCollapseExcludeXLabel->setObjectName(QStringLiteral("confCollapseExcludeXLabel"));
        sizePolicy.setHeightForWidth(confCollapseExcludeXLabel->sizePolicy().hasHeightForWidth());
        confCollapseExcludeXLabel->setSizePolicy(sizePolicy);

        gridLayout_18->addWidget(confCollapseExcludeXLabel, 1, 0, 1, 1);

        COCxminLineEdit = new QLineEdit(confCollapse3Frame);
        COCxminLineEdit->setObjectName(QStringLiteral("COCxminLineEdit"));
        COCxminLineEdit->setMinimumSize(QSize(80, 0));
        COCxminLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_18->addWidget(COCxminLineEdit, 1, 1, 1, 1);

        COCxmaxLineEdit = new QLineEdit(confCollapse3Frame);
        COCxmaxLineEdit->setObjectName(QStringLiteral("COCxmaxLineEdit"));
        COCxmaxLineEdit->setMinimumSize(QSize(80, 0));
        COCxmaxLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_18->addWidget(COCxmaxLineEdit, 1, 2, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_18->addItem(horizontalSpacer_6, 1, 3, 1, 1);

        confCollapseExcludeYLabel = new QLabel(confCollapse3Frame);
        confCollapseExcludeYLabel->setObjectName(QStringLiteral("confCollapseExcludeYLabel"));
        sizePolicy.setHeightForWidth(confCollapseExcludeYLabel->sizePolicy().hasHeightForWidth());
        confCollapseExcludeYLabel->setSizePolicy(sizePolicy);

        gridLayout_18->addWidget(confCollapseExcludeYLabel, 2, 0, 1, 1);

        COCyminLineEdit = new QLineEdit(confCollapse3Frame);
        COCyminLineEdit->setObjectName(QStringLiteral("COCyminLineEdit"));
        COCyminLineEdit->setMinimumSize(QSize(80, 0));
        COCyminLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_18->addWidget(COCyminLineEdit, 2, 1, 1, 1);

        COCymaxLineEdit = new QLineEdit(confCollapse3Frame);
        COCymaxLineEdit->setObjectName(QStringLiteral("COCymaxLineEdit"));
        COCymaxLineEdit->setMinimumSize(QSize(80, 0));
        COCymaxLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_18->addWidget(COCymaxLineEdit, 2, 2, 1, 1);


        verticalLayout_5->addWidget(confCollapse3Frame);

        verticalSpacer_3 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_3);


        gridLayout_59->addLayout(verticalLayout_5, 0, 0, 1, 1);

        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setSpacing(0);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        conftitleBinnedpreviewLabel = new QLabel(COCpage);
        conftitleBinnedpreviewLabel->setObjectName(QStringLiteral("conftitleBinnedpreviewLabel"));
        sizePolicy1.setHeightForWidth(conftitleBinnedpreviewLabel->sizePolicy().hasHeightForWidth());
        conftitleBinnedpreviewLabel->setSizePolicy(sizePolicy1);
        conftitleBinnedpreviewLabel->setFont(font);
        conftitleBinnedpreviewLabel->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleBinnedpreviewLabel->setWordWrap(true);

        verticalLayout_13->addWidget(conftitleBinnedpreviewLabel);

        confBinnedpreviewFrame = new QFrame(COCpage);
        confBinnedpreviewFrame->setObjectName(QStringLiteral("confBinnedpreviewFrame"));
        QPalette palette16;
        palette16.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette16.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette16.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette16.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette16.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette16.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette16.setBrush(QPalette::Active, QPalette::Text, brush);
        palette16.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette16.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette16.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette16.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette16.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette16.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette16.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette16.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette16.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette16.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette16.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette16.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette16.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette16.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette16.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette16.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette16.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette16.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette16.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette16.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette16.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette16.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette16.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette16.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette16.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette16.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette16.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette16.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette16.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette16.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette16.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette16.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette16.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confBinnedpreviewFrame->setPalette(palette16);
        confBinnedpreviewFrame->setAutoFillBackground(true);
        confBinnedpreviewFrame->setFrameShape(QFrame::NoFrame);
        confBinnedpreviewFrame->setFrameShadow(QFrame::Plain);
        gridLayout_24 = new QGridLayout(confBinnedpreviewFrame);
        gridLayout_24->setObjectName(QStringLiteral("gridLayout_24"));
        confBinningLabel = new QLabel(confBinnedpreviewFrame);
        confBinningLabel->setObjectName(QStringLiteral("confBinningLabel"));

        gridLayout_24->addWidget(confBinningLabel, 0, 0, 1, 1);

        BIPSpinBox = new QSpinBox(confBinnedpreviewFrame);
        BIPSpinBox->setObjectName(QStringLiteral("BIPSpinBox"));
        BIPSpinBox->setMinimumSize(QSize(100, 0));
        BIPSpinBox->setMaximumSize(QSize(100, 16777215));
        BIPSpinBox->setFocusPolicy(Qt::ClickFocus);
        BIPSpinBox->setMinimum(1);

        gridLayout_24->addWidget(BIPSpinBox, 0, 1, 1, 1);

        BIPrejectCheckBox = new QCheckBox(confBinnedpreviewFrame);
        BIPrejectCheckBox->setObjectName(QStringLiteral("BIPrejectCheckBox"));
        BIPrejectCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_24->addWidget(BIPrejectCheckBox, 1, 0, 1, 1);


        verticalLayout_13->addWidget(confBinnedpreviewFrame);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_13->addItem(verticalSpacer_4);


        gridLayout_59->addLayout(verticalLayout_13, 0, 1, 1, 1);

        horizontalSpacer_14 = new QSpacerItem(37, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_59->addItem(horizontalSpacer_14, 0, 2, 1, 1);

        confStackedWidget->addWidget(COCpage);
        WEIGHTpage = new QWidget();
        WEIGHTpage->setObjectName(QStringLiteral("WEIGHTpage"));
        gridLayout_23 = new QGridLayout(WEIGHTpage);
        gridLayout_23->setObjectName(QStringLiteral("gridLayout_23"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        conftitleGlobalweightLabel = new QLabel(WEIGHTpage);
        conftitleGlobalweightLabel->setObjectName(QStringLiteral("conftitleGlobalweightLabel"));
        sizePolicy1.setHeightForWidth(conftitleGlobalweightLabel->sizePolicy().hasHeightForWidth());
        conftitleGlobalweightLabel->setSizePolicy(sizePolicy1);
        conftitleGlobalweightLabel->setFont(font);
        conftitleGlobalweightLabel->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleGlobalweightLabel->setWordWrap(true);

        verticalLayout->addWidget(conftitleGlobalweightLabel);

        confsubtitleGlobalweight1Label = new QLabel(WEIGHTpage);
        confsubtitleGlobalweight1Label->setObjectName(QStringLiteral("confsubtitleGlobalweight1Label"));
        confsubtitleGlobalweight1Label->setFont(font);

        verticalLayout->addWidget(confsubtitleGlobalweight1Label);

        confGlobalweight1Frame = new QFrame(WEIGHTpage);
        confGlobalweight1Frame->setObjectName(QStringLiteral("confGlobalweight1Frame"));
        QPalette palette17;
        palette17.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette17.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette17.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette17.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette17.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette17.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette17.setBrush(QPalette::Active, QPalette::Text, brush);
        palette17.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette17.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette17.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette17.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette17.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette17.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette17.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette17.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette17.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette17.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette17.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette17.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette17.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette17.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette17.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette17.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette17.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette17.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette17.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette17.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette17.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette17.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette17.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette17.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette17.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette17.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette17.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette17.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette17.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette17.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette17.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette17.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette17.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette17.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette17.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette17.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette17.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette17.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confGlobalweight1Frame->setPalette(palette17);
        confGlobalweight1Frame->setAutoFillBackground(true);
        confGlobalweight1Frame->setFrameShape(QFrame::NoFrame);
        confGlobalweight1Frame->setFrameShadow(QFrame::Plain);
        gridLayout_66 = new QGridLayout(confGlobalweight1Frame);
        gridLayout_66->setObjectName(QStringLiteral("gridLayout_66"));
        CGWsameweightCheckBox = new QCheckBox(confGlobalweight1Frame);
        CGWsameweightCheckBox->setObjectName(QStringLiteral("CGWsameweightCheckBox"));
        CGWsameweightCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_66->addWidget(CGWsameweightCheckBox, 0, 0, 1, 1);


        verticalLayout->addWidget(confGlobalweight1Frame);

        confsubtitleGlobalweight2Label = new QLabel(WEIGHTpage);
        confsubtitleGlobalweight2Label->setObjectName(QStringLiteral("confsubtitleGlobalweight2Label"));
        confsubtitleGlobalweight2Label->setFont(font);

        verticalLayout->addWidget(confsubtitleGlobalweight2Label);

        confGlobalweight2Frame = new QFrame(WEIGHTpage);
        confGlobalweight2Frame->setObjectName(QStringLiteral("confGlobalweight2Frame"));
        QPalette palette18;
        palette18.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette18.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette18.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette18.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette18.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette18.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette18.setBrush(QPalette::Active, QPalette::Text, brush);
        palette18.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette18.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette18.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette18.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette18.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette18.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette18.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette18.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette18.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette18.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette18.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette18.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette18.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette18.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette18.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette18.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette18.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette18.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette18.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette18.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette18.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette18.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette18.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette18.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette18.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette18.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette18.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette18.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette18.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette18.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette18.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette18.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette18.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette18.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette18.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette18.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette18.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette18.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confGlobalweight2Frame->setPalette(palette18);
        confGlobalweight2Frame->setAutoFillBackground(true);
        confGlobalweight2Frame->setFrameShape(QFrame::NoFrame);
        confGlobalweight2Frame->setFrameShadow(QFrame::Plain);
        gridLayout_53 = new QGridLayout(confGlobalweight2Frame);
        gridLayout_53->setObjectName(QStringLiteral("gridLayout_53"));
        label_29 = new QLabel(confGlobalweight2Frame);
        label_29->setObjectName(QStringLiteral("label_29"));

        gridLayout_53->addWidget(label_29, 0, 1, 1, 1);

        label_30 = new QLabel(confGlobalweight2Frame);
        label_30->setObjectName(QStringLiteral("label_30"));

        gridLayout_53->addWidget(label_30, 0, 2, 1, 1);

        label_32 = new QLabel(confGlobalweight2Frame);
        label_32->setObjectName(QStringLiteral("label_32"));

        gridLayout_53->addWidget(label_32, 1, 0, 1, 1);

        CGWdarkminLineEdit = new QLineEdit(confGlobalweight2Frame);
        CGWdarkminLineEdit->setObjectName(QStringLiteral("CGWdarkminLineEdit"));
        CGWdarkminLineEdit->setMinimumSize(QSize(60, 0));
        CGWdarkminLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_53->addWidget(CGWdarkminLineEdit, 1, 1, 1, 1);

        CGWdarkmaxLineEdit = new QLineEdit(confGlobalweight2Frame);
        CGWdarkmaxLineEdit->setObjectName(QStringLiteral("CGWdarkmaxLineEdit"));
        CGWdarkmaxLineEdit->setMinimumSize(QSize(60, 0));
        CGWdarkmaxLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_53->addWidget(CGWdarkmaxLineEdit, 1, 2, 1, 1);

        label_33 = new QLabel(confGlobalweight2Frame);
        label_33->setObjectName(QStringLiteral("label_33"));

        gridLayout_53->addWidget(label_33, 2, 0, 1, 1);

        CGWflatminLineEdit = new QLineEdit(confGlobalweight2Frame);
        CGWflatminLineEdit->setObjectName(QStringLiteral("CGWflatminLineEdit"));
        CGWflatminLineEdit->setMinimumSize(QSize(60, 0));
        CGWflatminLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_53->addWidget(CGWflatminLineEdit, 2, 1, 1, 1);

        CGWflatmaxLineEdit = new QLineEdit(confGlobalweight2Frame);
        CGWflatmaxLineEdit->setObjectName(QStringLiteral("CGWflatmaxLineEdit"));
        CGWflatmaxLineEdit->setMinimumSize(QSize(60, 0));
        CGWflatmaxLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_53->addWidget(CGWflatmaxLineEdit, 2, 2, 1, 1);

        label_40 = new QLabel(confGlobalweight2Frame);
        label_40->setObjectName(QStringLiteral("label_40"));

        gridLayout_53->addWidget(label_40, 3, 0, 1, 1);

        CGWbackminLineEdit = new QLineEdit(confGlobalweight2Frame);
        CGWbackminLineEdit->setObjectName(QStringLiteral("CGWbackminLineEdit"));
        CGWbackminLineEdit->setMinimumSize(QSize(60, 0));
        CGWbackminLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_53->addWidget(CGWbackminLineEdit, 3, 1, 1, 1);

        CGWbackmaxLineEdit = new QLineEdit(confGlobalweight2Frame);
        CGWbackmaxLineEdit->setObjectName(QStringLiteral("CGWbackmaxLineEdit"));
        CGWbackmaxLineEdit->setMinimumSize(QSize(60, 0));
        CGWbackmaxLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_53->addWidget(CGWbackmaxLineEdit, 3, 2, 1, 1);


        verticalLayout->addWidget(confGlobalweight2Frame);

        confsubtitleGlobalweight3Label = new QLabel(WEIGHTpage);
        confsubtitleGlobalweight3Label->setObjectName(QStringLiteral("confsubtitleGlobalweight3Label"));
        confsubtitleGlobalweight3Label->setFont(font);

        verticalLayout->addWidget(confsubtitleGlobalweight3Label);

        confGlobalweight3Frame = new QFrame(WEIGHTpage);
        confGlobalweight3Frame->setObjectName(QStringLiteral("confGlobalweight3Frame"));
        QPalette palette19;
        palette19.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette19.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette19.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette19.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette19.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette19.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette19.setBrush(QPalette::Active, QPalette::Text, brush);
        palette19.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette19.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette19.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette19.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette19.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette19.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette19.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette19.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette19.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette19.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette19.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette19.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette19.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette19.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette19.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette19.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette19.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette19.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette19.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette19.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette19.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette19.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette19.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette19.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette19.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette19.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette19.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette19.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette19.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette19.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette19.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette19.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette19.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette19.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette19.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette19.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette19.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette19.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confGlobalweight3Frame->setPalette(palette19);
        confGlobalweight3Frame->setAutoFillBackground(true);
        confGlobalweight3Frame->setFrameShape(QFrame::NoFrame);
        confGlobalweight3Frame->setFrameShadow(QFrame::Plain);
        gridLayout_56 = new QGridLayout(confGlobalweight3Frame);
        gridLayout_56->setObjectName(QStringLiteral("gridLayout_56"));
        label_34 = new QLabel(confGlobalweight3Frame);
        label_34->setObjectName(QStringLiteral("label_34"));

        gridLayout_56->addWidget(label_34, 0, 1, 1, 1);

        label_35 = new QLabel(confGlobalweight3Frame);
        label_35->setObjectName(QStringLiteral("label_35"));

        gridLayout_56->addWidget(label_35, 0, 2, 1, 1);

        label_36 = new QLabel(confGlobalweight3Frame);
        label_36->setObjectName(QStringLiteral("label_36"));

        gridLayout_56->addWidget(label_36, 1, 0, 1, 1);

        CGWflatsmoothLineEdit = new QLineEdit(confGlobalweight3Frame);
        CGWflatsmoothLineEdit->setObjectName(QStringLiteral("CGWflatsmoothLineEdit"));
        CGWflatsmoothLineEdit->setMinimumSize(QSize(60, 0));
        CGWflatsmoothLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_56->addWidget(CGWflatsmoothLineEdit, 1, 1, 1, 1);

        CGWbacksmoothLineEdit = new QLineEdit(confGlobalweight3Frame);
        CGWbacksmoothLineEdit->setObjectName(QStringLiteral("CGWbacksmoothLineEdit"));
        CGWbacksmoothLineEdit->setMinimumSize(QSize(60, 0));
        CGWbacksmoothLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_56->addWidget(CGWbacksmoothLineEdit, 1, 2, 1, 1);

        label_37 = new QLabel(confGlobalweight3Frame);
        label_37->setObjectName(QStringLiteral("label_37"));

        gridLayout_56->addWidget(label_37, 2, 0, 1, 1);

        CGWflatrowtolLineEdit = new QLineEdit(confGlobalweight3Frame);
        CGWflatrowtolLineEdit->setObjectName(QStringLiteral("CGWflatrowtolLineEdit"));
        CGWflatrowtolLineEdit->setMinimumSize(QSize(60, 0));
        CGWflatrowtolLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_56->addWidget(CGWflatrowtolLineEdit, 2, 1, 1, 1);

        CGWbackrowtolLineEdit = new QLineEdit(confGlobalweight3Frame);
        CGWbackrowtolLineEdit->setObjectName(QStringLiteral("CGWbackrowtolLineEdit"));
        CGWbackrowtolLineEdit->setMinimumSize(QSize(60, 0));
        CGWbackrowtolLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_56->addWidget(CGWbackrowtolLineEdit, 2, 2, 1, 1);

        label_38 = new QLabel(confGlobalweight3Frame);
        label_38->setObjectName(QStringLiteral("label_38"));

        gridLayout_56->addWidget(label_38, 3, 0, 1, 1);

        CGWflatcoltolLineEdit = new QLineEdit(confGlobalweight3Frame);
        CGWflatcoltolLineEdit->setObjectName(QStringLiteral("CGWflatcoltolLineEdit"));
        CGWflatcoltolLineEdit->setMinimumSize(QSize(60, 0));
        CGWflatcoltolLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_56->addWidget(CGWflatcoltolLineEdit, 3, 1, 1, 1);

        CGWbackcoltolLineEdit = new QLineEdit(confGlobalweight3Frame);
        CGWbackcoltolLineEdit->setObjectName(QStringLiteral("CGWbackcoltolLineEdit"));
        CGWbackcoltolLineEdit->setMinimumSize(QSize(60, 0));
        CGWbackcoltolLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_56->addWidget(CGWbackcoltolLineEdit, 3, 2, 1, 1);

        label_39 = new QLabel(confGlobalweight3Frame);
        label_39->setObjectName(QStringLiteral("label_39"));

        gridLayout_56->addWidget(label_39, 4, 0, 1, 1);

        CGWflatclustolLineEdit = new QLineEdit(confGlobalweight3Frame);
        CGWflatclustolLineEdit->setObjectName(QStringLiteral("CGWflatclustolLineEdit"));
        CGWflatclustolLineEdit->setMinimumSize(QSize(60, 0));
        CGWflatclustolLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_56->addWidget(CGWflatclustolLineEdit, 4, 1, 1, 1);

        CGWbackclustolLineEdit = new QLineEdit(confGlobalweight3Frame);
        CGWbackclustolLineEdit->setObjectName(QStringLiteral("CGWbackclustolLineEdit"));
        CGWbackclustolLineEdit->setMinimumSize(QSize(60, 0));
        CGWbackclustolLineEdit->setMaximumSize(QSize(60, 16777215));

        gridLayout_56->addWidget(CGWbackclustolLineEdit, 4, 2, 1, 1);


        verticalLayout->addWidget(confGlobalweight3Frame);

        verticalSpacer_44 = new QSpacerItem(20, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_44);


        gridLayout_23->addLayout(verticalLayout, 0, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        conftitleIndividualweightLabel = new QLabel(WEIGHTpage);
        conftitleIndividualweightLabel->setObjectName(QStringLiteral("conftitleIndividualweightLabel"));
        sizePolicy1.setHeightForWidth(conftitleIndividualweightLabel->sizePolicy().hasHeightForWidth());
        conftitleIndividualweightLabel->setSizePolicy(sizePolicy1);
        conftitleIndividualweightLabel->setFont(font);
        conftitleIndividualweightLabel->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleIndividualweightLabel->setWordWrap(true);

        verticalLayout_2->addWidget(conftitleIndividualweightLabel);

        confsubtitleIndividualweight1Label = new QLabel(WEIGHTpage);
        confsubtitleIndividualweight1Label->setObjectName(QStringLiteral("confsubtitleIndividualweight1Label"));
        confsubtitleIndividualweight1Label->setFont(font);

        verticalLayout_2->addWidget(confsubtitleIndividualweight1Label);

        confIndividualweight1Frame = new QFrame(WEIGHTpage);
        confIndividualweight1Frame->setObjectName(QStringLiteral("confIndividualweight1Frame"));
        QPalette palette20;
        palette20.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette20.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette20.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette20.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette20.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette20.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette20.setBrush(QPalette::Active, QPalette::Text, brush);
        palette20.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette20.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette20.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette20.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette20.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette20.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette20.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette20.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette20.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette20.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette20.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette20.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette20.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette20.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette20.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette20.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette20.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette20.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette20.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette20.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette20.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette20.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette20.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette20.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette20.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette20.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette20.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette20.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette20.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette20.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette20.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette20.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette20.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette20.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette20.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette20.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette20.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette20.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confIndividualweight1Frame->setPalette(palette20);
        confIndividualweight1Frame->setAutoFillBackground(true);
        confIndividualweight1Frame->setFrameShape(QFrame::NoFrame);
        confIndividualweight1Frame->setFrameShadow(QFrame::Plain);
        gridLayout_4 = new QGridLayout(confIndividualweight1Frame);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        confIndWMinLabel = new QLabel(confIndividualweight1Frame);
        confIndWMinLabel->setObjectName(QStringLiteral("confIndWMinLabel"));

        gridLayout_4->addWidget(confIndWMinLabel, 0, 0, 1, 1);

        CIWminaduLineEdit = new QLineEdit(confIndividualweight1Frame);
        CIWminaduLineEdit->setObjectName(QStringLiteral("CIWminaduLineEdit"));
        CIWminaduLineEdit->setMinimumSize(QSize(80, 0));
        CIWminaduLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_4->addWidget(CIWminaduLineEdit, 0, 1, 1, 1);

        confIndWMaxLabel = new QLabel(confIndividualweight1Frame);
        confIndWMaxLabel->setObjectName(QStringLiteral("confIndWMaxLabel"));

        gridLayout_4->addWidget(confIndWMaxLabel, 1, 0, 1, 1);

        CIWmaxaduLineEdit = new QLineEdit(confIndividualweight1Frame);
        CIWmaxaduLineEdit->setObjectName(QStringLiteral("CIWmaxaduLineEdit"));
        CIWmaxaduLineEdit->setMinimumSize(QSize(80, 0));
        CIWmaxaduLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_4->addWidget(CIWmaxaduLineEdit, 1, 1, 1, 1);


        verticalLayout_2->addWidget(confIndividualweight1Frame);

        confsubtitleIndividualweight2Label = new QLabel(WEIGHTpage);
        confsubtitleIndividualweight2Label->setObjectName(QStringLiteral("confsubtitleIndividualweight2Label"));
        confsubtitleIndividualweight2Label->setFont(font);

        verticalLayout_2->addWidget(confsubtitleIndividualweight2Label);

        confIndividualweight2Frame = new QFrame(WEIGHTpage);
        confIndividualweight2Frame->setObjectName(QStringLiteral("confIndividualweight2Frame"));
        QPalette palette21;
        palette21.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette21.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette21.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette21.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette21.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette21.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette21.setBrush(QPalette::Active, QPalette::Text, brush);
        palette21.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette21.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette21.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette21.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette21.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette21.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette21.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette21.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette21.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette21.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette21.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette21.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette21.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette21.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette21.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette21.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette21.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette21.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette21.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette21.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette21.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette21.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette21.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette21.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette21.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette21.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette21.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette21.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette21.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette21.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette21.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette21.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette21.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette21.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette21.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette21.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette21.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette21.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confIndividualweight2Frame->setPalette(palette21);
        confIndividualweight2Frame->setAutoFillBackground(true);
        confIndividualweight2Frame->setFrameShape(QFrame::NoFrame);
        confIndividualweight2Frame->setFrameShadow(QFrame::Plain);
        gridLayout_6 = new QGridLayout(confIndividualweight2Frame);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        confIndWBloomMarginLabel_3 = new QLabel(confIndividualweight2Frame);
        confIndWBloomMarginLabel_3->setObjectName(QStringLiteral("confIndWBloomMarginLabel_3"));
        confIndWBloomMarginLabel_3->setEnabled(false);

        gridLayout_6->addWidget(confIndWBloomMarginLabel_3, 8, 1, 1, 2);

        CIWbloomRangeLineEdit = new QLineEdit(confIndividualweight2Frame);
        CIWbloomRangeLineEdit->setObjectName(QStringLiteral("CIWbloomRangeLineEdit"));
        CIWbloomRangeLineEdit->setMinimumSize(QSize(80, 0));
        CIWbloomRangeLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_6->addWidget(CIWbloomRangeLineEdit, 5, 2, 1, 2);

        horizontalSpacer_17 = new QSpacerItem(35, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_17, 4, 0, 1, 1);

        confIndWBloomMarginLabel = new QLabel(confIndividualweight2Frame);
        confIndWBloomMarginLabel->setObjectName(QStringLiteral("confIndWBloomMarginLabel"));

        gridLayout_6->addWidget(confIndWBloomMarginLabel, 5, 1, 1, 1);

        confIndWBloomMinLabel = new QLabel(confIndividualweight2Frame);
        confIndWBloomMinLabel->setObjectName(QStringLiteral("confIndWBloomMinLabel"));

        gridLayout_6->addWidget(confIndWBloomMinLabel, 4, 1, 1, 1);

        horizontalSpacer_22 = new QSpacerItem(35, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_22, 8, 0, 1, 1);

        CIWaggressivenessLineEdit = new QLineEdit(confIndividualweight2Frame);
        CIWaggressivenessLineEdit->setObjectName(QStringLiteral("CIWaggressivenessLineEdit"));
        CIWaggressivenessLineEdit->setMinimumSize(QSize(80, 0));
        CIWaggressivenessLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_6->addWidget(CIWaggressivenessLineEdit, 1, 2, 1, 2);

        horizontalSpacer_20 = new QSpacerItem(35, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_20, 5, 0, 1, 1);

        CIWbloomMinaduLineEdit = new QLineEdit(confIndividualweight2Frame);
        CIWbloomMinaduLineEdit->setObjectName(QStringLiteral("CIWbloomMinaduLineEdit"));
        CIWbloomMinaduLineEdit->setMinimumSize(QSize(80, 0));
        CIWbloomMinaduLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_6->addWidget(CIWbloomMinaduLineEdit, 4, 2, 1, 2);

        confIndWBloomMarginLabel_2 = new QLabel(confIndividualweight2Frame);
        confIndWBloomMarginLabel_2->setObjectName(QStringLiteral("confIndWBloomMarginLabel_2"));
        confIndWBloomMarginLabel_2->setEnabled(false);

        gridLayout_6->addWidget(confIndWBloomMarginLabel_2, 7, 1, 1, 2);

        horizontalSpacer_21 = new QSpacerItem(35, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_21, 7, 0, 1, 1);

        confIndWThresholdLabel = new QLabel(confIndividualweight2Frame);
        confIndWThresholdLabel->setObjectName(QStringLiteral("confIndWThresholdLabel"));

        gridLayout_6->addWidget(confIndWThresholdLabel, 1, 0, 1, 2);

        CIWmaskbloomingCheckBox = new QCheckBox(confIndividualweight2Frame);
        CIWmaskbloomingCheckBox->setObjectName(QStringLiteral("CIWmaskbloomingCheckBox"));
        QPalette palette22;
        palette22.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush11(QColor(153, 153, 153, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette22.setBrush(QPalette::Active, QPalette::Button, brush11);
        QBrush brush12(QColor(230, 230, 230, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette22.setBrush(QPalette::Active, QPalette::Light, brush12);
        QBrush brush13(QColor(191, 191, 191, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette22.setBrush(QPalette::Active, QPalette::Midlight, brush13);
        QBrush brush14(QColor(76, 76, 76, 255));
        brush14.setStyle(Qt::SolidPattern);
        palette22.setBrush(QPalette::Active, QPalette::Dark, brush14);
        QBrush brush15(QColor(102, 102, 102, 255));
        brush15.setStyle(Qt::SolidPattern);
        palette22.setBrush(QPalette::Active, QPalette::Mid, brush15);
        palette22.setBrush(QPalette::Active, QPalette::Text, brush);
        palette22.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette22.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette22.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette22.setBrush(QPalette::Active, QPalette::Window, brush11);
        palette22.setBrush(QPalette::Active, QPalette::Shadow, brush);
        QBrush brush16(QColor(204, 204, 204, 255));
        brush16.setStyle(Qt::SolidPattern);
        palette22.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
        palette22.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette22.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette22.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette22.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette22.setBrush(QPalette::Inactive, QPalette::Light, brush12);
        palette22.setBrush(QPalette::Inactive, QPalette::Midlight, brush13);
        palette22.setBrush(QPalette::Inactive, QPalette::Dark, brush14);
        palette22.setBrush(QPalette::Inactive, QPalette::Mid, brush15);
        palette22.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette22.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette22.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette22.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette22.setBrush(QPalette::Inactive, QPalette::Window, brush11);
        palette22.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette22.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush16);
        palette22.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette22.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette22.setBrush(QPalette::Disabled, QPalette::WindowText, brush14);
        palette22.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette22.setBrush(QPalette::Disabled, QPalette::Light, brush12);
        palette22.setBrush(QPalette::Disabled, QPalette::Midlight, brush13);
        palette22.setBrush(QPalette::Disabled, QPalette::Dark, brush14);
        palette22.setBrush(QPalette::Disabled, QPalette::Mid, brush15);
        palette22.setBrush(QPalette::Disabled, QPalette::Text, brush14);
        palette22.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette22.setBrush(QPalette::Disabled, QPalette::ButtonText, brush14);
        palette22.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette22.setBrush(QPalette::Disabled, QPalette::Window, brush11);
        palette22.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette22.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush11);
        palette22.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette22.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        CIWmaskbloomingCheckBox->setPalette(palette22);
        CIWmaskbloomingCheckBox->setFont(font1);
        CIWmaskbloomingCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_6->addWidget(CIWmaskbloomingCheckBox, 3, 0, 1, 4);

        CIWpersistenceMinaduLineEdit = new QLineEdit(confIndividualweight2Frame);
        CIWpersistenceMinaduLineEdit->setObjectName(QStringLiteral("CIWpersistenceMinaduLineEdit"));
        CIWpersistenceMinaduLineEdit->setEnabled(false);
        CIWpersistenceMinaduLineEdit->setMinimumSize(QSize(40, 0));
        CIWpersistenceMinaduLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_6->addWidget(CIWpersistenceMinaduLineEdit, 8, 3, 1, 1);

        confIndWBloomMinLabel_2 = new QLabel(confIndividualweight2Frame);
        confIndWBloomMinLabel_2->setObjectName(QStringLiteral("confIndWBloomMinLabel_2"));

        gridLayout_6->addWidget(confIndWBloomMinLabel_2, 0, 0, 1, 4);

        CIWpersistenceTimescaleLineEdit = new QLineEdit(confIndividualweight2Frame);
        CIWpersistenceTimescaleLineEdit->setObjectName(QStringLiteral("CIWpersistenceTimescaleLineEdit"));
        CIWpersistenceTimescaleLineEdit->setEnabled(false);
        CIWpersistenceTimescaleLineEdit->setMinimumSize(QSize(40, 0));
        CIWpersistenceTimescaleLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_6->addWidget(CIWpersistenceTimescaleLineEdit, 7, 3, 1, 1);

        CIWpersistenceCheckBox = new QCheckBox(confIndividualweight2Frame);
        CIWpersistenceCheckBox->setObjectName(QStringLiteral("CIWpersistenceCheckBox"));
        CIWpersistenceCheckBox->setEnabled(false);
        QPalette palette23;
        palette23.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette23.setBrush(QPalette::Active, QPalette::Button, brush11);
        palette23.setBrush(QPalette::Active, QPalette::Light, brush12);
        palette23.setBrush(QPalette::Active, QPalette::Midlight, brush13);
        palette23.setBrush(QPalette::Active, QPalette::Dark, brush14);
        palette23.setBrush(QPalette::Active, QPalette::Mid, brush15);
        palette23.setBrush(QPalette::Active, QPalette::Text, brush);
        palette23.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette23.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette23.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette23.setBrush(QPalette::Active, QPalette::Window, brush11);
        palette23.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette23.setBrush(QPalette::Active, QPalette::AlternateBase, brush16);
        palette23.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette23.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette23.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette23.setBrush(QPalette::Inactive, QPalette::Button, brush11);
        palette23.setBrush(QPalette::Inactive, QPalette::Light, brush12);
        palette23.setBrush(QPalette::Inactive, QPalette::Midlight, brush13);
        palette23.setBrush(QPalette::Inactive, QPalette::Dark, brush14);
        palette23.setBrush(QPalette::Inactive, QPalette::Mid, brush15);
        palette23.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette23.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette23.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette23.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette23.setBrush(QPalette::Inactive, QPalette::Window, brush11);
        palette23.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette23.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush16);
        palette23.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette23.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette23.setBrush(QPalette::Disabled, QPalette::WindowText, brush14);
        palette23.setBrush(QPalette::Disabled, QPalette::Button, brush11);
        palette23.setBrush(QPalette::Disabled, QPalette::Light, brush12);
        palette23.setBrush(QPalette::Disabled, QPalette::Midlight, brush13);
        palette23.setBrush(QPalette::Disabled, QPalette::Dark, brush14);
        palette23.setBrush(QPalette::Disabled, QPalette::Mid, brush15);
        palette23.setBrush(QPalette::Disabled, QPalette::Text, brush14);
        palette23.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette23.setBrush(QPalette::Disabled, QPalette::ButtonText, brush14);
        palette23.setBrush(QPalette::Disabled, QPalette::Base, brush11);
        palette23.setBrush(QPalette::Disabled, QPalette::Window, brush11);
        palette23.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette23.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush11);
        palette23.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette23.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        CIWpersistenceCheckBox->setPalette(palette23);
        CIWpersistenceCheckBox->setFont(font1);
        CIWpersistenceCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_6->addWidget(CIWpersistenceCheckBox, 6, 0, 1, 4);

        CIWmasksaturationCheckBox = new QCheckBox(confIndividualweight2Frame);
        CIWmasksaturationCheckBox->setObjectName(QStringLiteral("CIWmasksaturationCheckBox"));

        gridLayout_6->addWidget(CIWmasksaturationCheckBox, 2, 0, 1, 2);

        CIWsaturationLineEdit = new QLineEdit(confIndividualweight2Frame);
        CIWsaturationLineEdit->setObjectName(QStringLiteral("CIWsaturationLineEdit"));
        CIWsaturationLineEdit->setMinimumSize(QSize(80, 0));
        CIWsaturationLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_6->addWidget(CIWsaturationLineEdit, 2, 2, 1, 2);


        verticalLayout_2->addWidget(confIndividualweight2Frame);

        verticalSpacer_45 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_45);

        conftitleSeparateLabel = new QLabel(WEIGHTpage);
        conftitleSeparateLabel->setObjectName(QStringLiteral("conftitleSeparateLabel"));
        sizePolicy1.setHeightForWidth(conftitleSeparateLabel->sizePolicy().hasHeightForWidth());
        conftitleSeparateLabel->setSizePolicy(sizePolicy1);
        conftitleSeparateLabel->setFont(font);
        conftitleSeparateLabel->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleSeparateLabel->setWordWrap(true);

        verticalLayout_2->addWidget(conftitleSeparateLabel);

        confSeparateFrame = new QFrame(WEIGHTpage);
        confSeparateFrame->setObjectName(QStringLiteral("confSeparateFrame"));
        confSeparateFrame->setEnabled(true);
        QPalette palette24;
        palette24.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette24.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette24.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette24.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette24.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette24.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette24.setBrush(QPalette::Active, QPalette::Text, brush);
        palette24.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette24.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette24.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette24.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette24.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette24.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette24.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette24.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette24.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette24.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette24.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette24.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette24.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette24.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette24.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette24.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette24.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette24.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette24.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette24.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette24.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette24.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette24.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette24.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette24.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette24.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette24.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette24.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette24.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette24.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette24.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette24.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette24.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette24.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette24.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette24.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette24.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette24.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confSeparateFrame->setPalette(palette24);
        confSeparateFrame->setAutoFillBackground(true);
        confSeparateFrame->setFrameShape(QFrame::NoFrame);
        confSeparateFrame->setFrameShadow(QFrame::Plain);
        gridLayout_49 = new QGridLayout(confSeparateFrame);
        gridLayout_49->setObjectName(QStringLiteral("gridLayout_49"));
        separationLabel = new QLabel(confSeparateFrame);
        separationLabel->setObjectName(QStringLiteral("separationLabel"));
        separationLabel->setEnabled(true);

        gridLayout_49->addWidget(separationLabel, 0, 0, 1, 1);

        separateTargetLineEdit = new QLineEdit(confSeparateFrame);
        separateTargetLineEdit->setObjectName(QStringLiteral("separateTargetLineEdit"));
        separateTargetLineEdit->setEnabled(true);
        separateTargetLineEdit->setMinimumSize(QSize(80, 0));
        separateTargetLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_49->addWidget(separateTargetLineEdit, 0, 1, 1, 1);


        verticalLayout_2->addWidget(confSeparateFrame);

        verticalSpacer_46 = new QSpacerItem(17, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_46);


        gridLayout_23->addLayout(verticalLayout_2, 0, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(59, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_23->addItem(horizontalSpacer_3, 0, 2, 1, 1);

        confStackedWidget->addWidget(WEIGHTpage);
        APIpage = new QWidget();
        APIpage->setObjectName(QStringLiteral("APIpage"));
        gridLayout_62 = new QGridLayout(APIpage);
        gridLayout_62->setObjectName(QStringLiteral("gridLayout_62"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        conftitleAbsphotindirect1Label = new QLabel(APIpage);
        conftitleAbsphotindirect1Label->setObjectName(QStringLiteral("conftitleAbsphotindirect1Label"));
        conftitleAbsphotindirect1Label->setEnabled(false);
        sizePolicy1.setHeightForWidth(conftitleAbsphotindirect1Label->sizePolicy().hasHeightForWidth());
        conftitleAbsphotindirect1Label->setSizePolicy(sizePolicy1);
        conftitleAbsphotindirect1Label->setFont(font);
        conftitleAbsphotindirect1Label->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleAbsphotindirect1Label->setWordWrap(true);

        verticalLayout_6->addWidget(conftitleAbsphotindirect1Label);

        confsubtitleAbsphotindirect1Label = new QLabel(APIpage);
        confsubtitleAbsphotindirect1Label->setObjectName(QStringLiteral("confsubtitleAbsphotindirect1Label"));
        confsubtitleAbsphotindirect1Label->setFont(font);
        confsubtitleAbsphotindirect1Label->setWordWrap(true);

        verticalLayout_6->addWidget(confsubtitleAbsphotindirect1Label);

        confAbsphotindirect1Frame = new QFrame(APIpage);
        confAbsphotindirect1Frame->setObjectName(QStringLiteral("confAbsphotindirect1Frame"));
        confAbsphotindirect1Frame->setEnabled(false);
        QPalette palette25;
        palette25.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette25.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette25.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette25.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette25.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette25.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette25.setBrush(QPalette::Active, QPalette::Text, brush);
        palette25.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette25.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette25.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette25.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette25.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette25.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette25.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette25.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette25.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette25.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette25.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette25.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette25.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette25.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette25.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette25.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette25.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette25.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette25.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette25.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette25.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette25.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette25.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette25.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette25.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette25.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette25.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette25.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette25.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette25.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette25.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette25.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette25.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette25.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette25.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette25.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette25.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette25.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confAbsphotindirect1Frame->setPalette(palette25);
        confAbsphotindirect1Frame->setAutoFillBackground(true);
        confAbsphotindirect1Frame->setFrameShape(QFrame::NoFrame);
        confAbsphotindirect1Frame->setFrameShadow(QFrame::Plain);
        gridLayout_54 = new QGridLayout(confAbsphotindirect1Frame);
        gridLayout_54->setObjectName(QStringLiteral("gridLayout_54"));
        gridLayout_51 = new QGridLayout();
        gridLayout_51->setObjectName(QStringLiteral("gridLayout_51"));
        label_41 = new QLabel(confAbsphotindirect1Frame);
        label_41->setObjectName(QStringLiteral("label_41"));

        gridLayout_51->addWidget(label_41, 0, 0, 1, 1);

        APIrefcatComboBox = new QComboBox(confAbsphotindirect1Frame);
        APIrefcatComboBox->setObjectName(QStringLiteral("APIrefcatComboBox"));
        APIrefcatComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_51->addWidget(APIrefcatComboBox, 0, 1, 1, 3);

        confAPILabel_3 = new QLabel(confAbsphotindirect1Frame);
        confAPILabel_3->setObjectName(QStringLiteral("confAPILabel_3"));

        gridLayout_51->addWidget(confAPILabel_3, 1, 0, 1, 1);

        APIfilterComboBox = new QComboBox(confAbsphotindirect1Frame);
        APIfilterComboBox->setObjectName(QStringLiteral("APIfilterComboBox"));
        APIfilterComboBox->setMinimumSize(QSize(0, 0));
        APIfilterComboBox->setMaximumSize(QSize(80, 16777215));
        APIfilterComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_51->addWidget(APIfilterComboBox, 1, 3, 1, 1);

        confAPILabel_13 = new QLabel(confAbsphotindirect1Frame);
        confAPILabel_13->setObjectName(QStringLiteral("confAPILabel_13"));

        gridLayout_51->addWidget(confAPILabel_13, 2, 0, 1, 1);

        APIcolorComboBox = new QComboBox(confAbsphotindirect1Frame);
        APIcolorComboBox->setObjectName(QStringLiteral("APIcolorComboBox"));
        APIcolorComboBox->setMinimumSize(QSize(0, 0));
        APIcolorComboBox->setMaximumSize(QSize(80, 16777215));
        APIcolorComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_51->addWidget(APIcolorComboBox, 2, 3, 1, 1);

        confAPILabel_11 = new QLabel(confAbsphotindirect1Frame);
        confAPILabel_11->setObjectName(QStringLiteral("confAPILabel_11"));

        gridLayout_51->addWidget(confAPILabel_11, 3, 0, 1, 2);

        APIcalibrationmodeComboBox = new QComboBox(confAbsphotindirect1Frame);
        APIcalibrationmodeComboBox->setObjectName(QStringLiteral("APIcalibrationmodeComboBox"));
        APIcalibrationmodeComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_51->addWidget(APIcalibrationmodeComboBox, 3, 2, 1, 2);

        confAPILabel = new QLabel(confAbsphotindirect1Frame);
        confAPILabel->setObjectName(QStringLiteral("confAPILabel"));

        gridLayout_51->addWidget(confAPILabel, 4, 0, 1, 3);

        APIapertureLineEdit = new QLineEdit(confAbsphotindirect1Frame);
        APIapertureLineEdit->setObjectName(QStringLiteral("APIapertureLineEdit"));
        APIapertureLineEdit->setMinimumSize(QSize(80, 0));
        APIapertureLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_51->addWidget(APIapertureLineEdit, 4, 3, 1, 1);

        confAPILabel_2 = new QLabel(confAbsphotindirect1Frame);
        confAPILabel_2->setObjectName(QStringLiteral("confAPILabel_2"));

        gridLayout_51->addWidget(confAPILabel_2, 5, 0, 1, 2);

        APIfilterkeywordLineEdit = new QLineEdit(confAbsphotindirect1Frame);
        APIfilterkeywordLineEdit->setObjectName(QStringLiteral("APIfilterkeywordLineEdit"));
        APIfilterkeywordLineEdit->setMinimumSize(QSize(80, 0));
        APIfilterkeywordLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_51->addWidget(APIfilterkeywordLineEdit, 5, 3, 1, 1);


        gridLayout_54->addLayout(gridLayout_51, 0, 0, 1, 1);


        verticalLayout_6->addWidget(confAbsphotindirect1Frame);

        confsubtitleAbsphotindirect2Label = new QLabel(APIpage);
        confsubtitleAbsphotindirect2Label->setObjectName(QStringLiteral("confsubtitleAbsphotindirect2Label"));
        confsubtitleAbsphotindirect2Label->setFont(font);
        confsubtitleAbsphotindirect2Label->setWordWrap(true);

        verticalLayout_6->addWidget(confsubtitleAbsphotindirect2Label);

        confAbsphotindirect2Frame = new QFrame(APIpage);
        confAbsphotindirect2Frame->setObjectName(QStringLiteral("confAbsphotindirect2Frame"));
        confAbsphotindirect2Frame->setEnabled(false);
        QPalette palette26;
        palette26.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette26.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette26.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette26.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette26.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette26.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette26.setBrush(QPalette::Active, QPalette::Text, brush);
        palette26.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette26.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette26.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette26.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette26.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette26.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette26.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette26.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette26.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette26.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette26.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette26.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette26.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette26.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette26.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette26.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette26.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette26.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette26.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette26.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette26.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette26.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette26.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette26.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette26.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette26.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette26.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette26.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette26.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette26.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette26.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette26.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette26.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette26.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette26.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette26.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette26.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette26.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confAbsphotindirect2Frame->setPalette(palette26);
        confAbsphotindirect2Frame->setAutoFillBackground(true);
        confAbsphotindirect2Frame->setFrameShape(QFrame::NoFrame);
        confAbsphotindirect2Frame->setFrameShadow(QFrame::Plain);
        gridLayout_13 = new QGridLayout(confAbsphotindirect2Frame);
        gridLayout_13->setObjectName(QStringLiteral("gridLayout_13"));
        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        confAPILabel_4 = new QLabel(confAbsphotindirect2Frame);
        confAPILabel_4->setObjectName(QStringLiteral("confAPILabel_4"));

        gridLayout_7->addWidget(confAPILabel_4, 0, 0, 1, 1);

        APIcolortermLineEdit = new QLineEdit(confAbsphotindirect2Frame);
        APIcolortermLineEdit->setObjectName(QStringLiteral("APIcolortermLineEdit"));
        APIcolortermLineEdit->setMinimumSize(QSize(80, 0));
        APIcolortermLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_7->addWidget(APIcolortermLineEdit, 1, 1, 1, 1);

        APIextinctionLineEdit = new QLineEdit(confAbsphotindirect2Frame);
        APIextinctionLineEdit->setObjectName(QStringLiteral("APIextinctionLineEdit"));
        APIextinctionLineEdit->setMinimumSize(QSize(80, 0));
        APIextinctionLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_7->addWidget(APIextinctionLineEdit, 0, 1, 1, 1);

        confAPILabel_5 = new QLabel(confAbsphotindirect2Frame);
        confAPILabel_5->setObjectName(QStringLiteral("confAPILabel_5"));

        gridLayout_7->addWidget(confAPILabel_5, 1, 0, 1, 1);

        confAPILabel_7 = new QLabel(confAbsphotindirect2Frame);
        confAPILabel_7->setObjectName(QStringLiteral("confAPILabel_7"));

        gridLayout_7->addWidget(confAPILabel_7, 2, 0, 1, 1);

        APIminmagLineEdit = new QLineEdit(confAbsphotindirect2Frame);
        APIminmagLineEdit->setObjectName(QStringLiteral("APIminmagLineEdit"));
        APIminmagLineEdit->setMinimumSize(QSize(80, 0));
        APIminmagLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_7->addWidget(APIminmagLineEdit, 6, 1, 1, 1);

        APIkappaLineEdit = new QLineEdit(confAbsphotindirect2Frame);
        APIkappaLineEdit->setObjectName(QStringLiteral("APIkappaLineEdit"));
        APIkappaLineEdit->setMinimumSize(QSize(80, 0));
        APIkappaLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_7->addWidget(APIkappaLineEdit, 4, 1, 1, 1);

        confAPILabel_15 = new QLabel(confAbsphotindirect2Frame);
        confAPILabel_15->setObjectName(QStringLiteral("confAPILabel_15"));

        gridLayout_7->addWidget(confAPILabel_15, 5, 0, 1, 1);

        confAPILabel_10 = new QLabel(confAbsphotindirect2Frame);
        confAPILabel_10->setObjectName(QStringLiteral("confAPILabel_10"));

        gridLayout_7->addWidget(confAPILabel_10, 7, 0, 1, 1);

        APIthresholdLineEdit = new QLineEdit(confAbsphotindirect2Frame);
        APIthresholdLineEdit->setObjectName(QStringLiteral("APIthresholdLineEdit"));
        APIthresholdLineEdit->setMinimumSize(QSize(80, 0));
        APIthresholdLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_7->addWidget(APIthresholdLineEdit, 5, 1, 1, 1);

        APIzpminLineEdit = new QLineEdit(confAbsphotindirect2Frame);
        APIzpminLineEdit->setObjectName(QStringLiteral("APIzpminLineEdit"));
        APIzpminLineEdit->setMinimumSize(QSize(80, 0));
        APIzpminLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_7->addWidget(APIzpminLineEdit, 2, 1, 1, 1);

        confAPILabel_14 = new QLabel(confAbsphotindirect2Frame);
        confAPILabel_14->setObjectName(QStringLiteral("confAPILabel_14"));

        gridLayout_7->addWidget(confAPILabel_14, 3, 0, 1, 1);

        APIzpmaxLineEdit = new QLineEdit(confAbsphotindirect2Frame);
        APIzpmaxLineEdit->setObjectName(QStringLiteral("APIzpmaxLineEdit"));
        APIzpmaxLineEdit->setMinimumSize(QSize(80, 0));
        APIzpmaxLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_7->addWidget(APIzpmaxLineEdit, 3, 1, 1, 1);

        confAPILabel_6 = new QLabel(confAbsphotindirect2Frame);
        confAPILabel_6->setObjectName(QStringLiteral("confAPILabel_6"));

        gridLayout_7->addWidget(confAPILabel_6, 4, 0, 1, 1);

        APIconvergenceLineEdit = new QLineEdit(confAbsphotindirect2Frame);
        APIconvergenceLineEdit->setObjectName(QStringLiteral("APIconvergenceLineEdit"));
        APIconvergenceLineEdit->setMinimumSize(QSize(80, 0));
        APIconvergenceLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_7->addWidget(APIconvergenceLineEdit, 7, 1, 1, 1);

        confAPILabel_8 = new QLabel(confAbsphotindirect2Frame);
        confAPILabel_8->setObjectName(QStringLiteral("confAPILabel_8"));

        gridLayout_7->addWidget(confAPILabel_8, 6, 0, 1, 1);


        gridLayout_13->addLayout(gridLayout_7, 0, 0, 1, 1);


        verticalLayout_6->addWidget(confAbsphotindirect2Frame);

        verticalSpacer_47 = new QSpacerItem(288, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_47);


        gridLayout_62->addLayout(verticalLayout_6, 0, 0, 1, 1);

        gridLayout_33 = new QGridLayout();
        gridLayout_33->setObjectName(QStringLiteral("gridLayout_33"));
        gridLayout_33->setVerticalSpacing(0);
        conftitleAbsphotindirect2Label = new QLabel(APIpage);
        conftitleAbsphotindirect2Label->setObjectName(QStringLiteral("conftitleAbsphotindirect2Label"));
        sizePolicy1.setHeightForWidth(conftitleAbsphotindirect2Label->sizePolicy().hasHeightForWidth());
        conftitleAbsphotindirect2Label->setSizePolicy(sizePolicy1);
        conftitleAbsphotindirect2Label->setFont(font);
        conftitleAbsphotindirect2Label->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleAbsphotindirect2Label->setWordWrap(true);

        gridLayout_33->addWidget(conftitleAbsphotindirect2Label, 0, 0, 1, 1);

        confsubtitleAbsphotindirect3Label = new QLabel(APIpage);
        confsubtitleAbsphotindirect3Label->setObjectName(QStringLiteral("confsubtitleAbsphotindirect3Label"));
        confsubtitleAbsphotindirect3Label->setFont(font);
        confsubtitleAbsphotindirect3Label->setWordWrap(true);

        gridLayout_33->addWidget(confsubtitleAbsphotindirect3Label, 1, 0, 1, 1);

        confAbsphotindirect3Frame = new QFrame(APIpage);
        confAbsphotindirect3Frame->setObjectName(QStringLiteral("confAbsphotindirect3Frame"));
        confAbsphotindirect3Frame->setEnabled(false);
        QPalette palette27;
        palette27.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette27.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette27.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette27.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette27.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette27.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette27.setBrush(QPalette::Active, QPalette::Text, brush);
        palette27.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette27.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette27.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette27.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette27.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette27.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette27.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette27.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette27.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette27.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette27.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette27.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette27.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette27.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette27.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette27.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette27.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette27.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette27.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette27.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette27.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette27.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette27.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette27.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette27.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette27.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette27.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette27.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette27.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette27.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette27.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette27.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette27.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette27.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette27.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette27.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette27.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette27.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confAbsphotindirect3Frame->setPalette(palette27);
        confAbsphotindirect3Frame->setAutoFillBackground(true);
        confAbsphotindirect3Frame->setFrameShape(QFrame::NoFrame);
        confAbsphotindirect3Frame->setFrameShadow(QFrame::Plain);
        gridLayout_74 = new QGridLayout(confAbsphotindirect3Frame);
        gridLayout_74->setObjectName(QStringLiteral("gridLayout_74"));
        confAPILabel_12 = new QLabel(confAbsphotindirect3Frame);
        confAPILabel_12->setObjectName(QStringLiteral("confAPILabel_12"));

        gridLayout_74->addWidget(confAPILabel_12, 2, 0, 1, 1);

        APIWCSLineEdit = new QLineEdit(confAbsphotindirect3Frame);
        APIWCSLineEdit->setObjectName(QStringLiteral("APIWCSLineEdit"));
        APIWCSLineEdit->setMinimumSize(QSize(80, 0));
        APIWCSLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_74->addWidget(APIWCSLineEdit, 2, 1, 1, 1);

        APIWCSCheckBox = new QCheckBox(confAbsphotindirect3Frame);
        APIWCSCheckBox->setObjectName(QStringLiteral("APIWCSCheckBox"));
        APIWCSCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_74->addWidget(APIWCSCheckBox, 1, 0, 1, 1);

        APIreadmePushButton = new QPushButton(confAbsphotindirect3Frame);
        APIreadmePushButton->setObjectName(QStringLiteral("APIreadmePushButton"));
        APIreadmePushButton->setMinimumSize(QSize(30, 30));
        APIreadmePushButton->setMaximumSize(QSize(30, 30));
        QPalette palette28;
        palette28.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush17(QColor(85, 170, 255, 255));
        brush17.setStyle(Qt::SolidPattern);
        palette28.setBrush(QPalette::Active, QPalette::Button, brush17);
        QBrush brush18(QColor(213, 234, 255, 255));
        brush18.setStyle(Qt::SolidPattern);
        palette28.setBrush(QPalette::Active, QPalette::Light, brush18);
        QBrush brush19(QColor(149, 202, 255, 255));
        brush19.setStyle(Qt::SolidPattern);
        palette28.setBrush(QPalette::Active, QPalette::Midlight, brush19);
        QBrush brush20(QColor(42, 85, 127, 255));
        brush20.setStyle(Qt::SolidPattern);
        palette28.setBrush(QPalette::Active, QPalette::Dark, brush20);
        QBrush brush21(QColor(56, 113, 170, 255));
        brush21.setStyle(Qt::SolidPattern);
        palette28.setBrush(QPalette::Active, QPalette::Mid, brush21);
        palette28.setBrush(QPalette::Active, QPalette::Text, brush);
        palette28.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette28.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette28.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette28.setBrush(QPalette::Active, QPalette::Window, brush17);
        palette28.setBrush(QPalette::Active, QPalette::Shadow, brush);
        QBrush brush22(QColor(170, 212, 255, 255));
        brush22.setStyle(Qt::SolidPattern);
        palette28.setBrush(QPalette::Active, QPalette::AlternateBase, brush22);
        palette28.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette28.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette28.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette28.setBrush(QPalette::Inactive, QPalette::Button, brush17);
        palette28.setBrush(QPalette::Inactive, QPalette::Light, brush18);
        palette28.setBrush(QPalette::Inactive, QPalette::Midlight, brush19);
        palette28.setBrush(QPalette::Inactive, QPalette::Dark, brush20);
        palette28.setBrush(QPalette::Inactive, QPalette::Mid, brush21);
        palette28.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette28.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette28.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette28.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette28.setBrush(QPalette::Inactive, QPalette::Window, brush17);
        palette28.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette28.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush22);
        palette28.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette28.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette28.setBrush(QPalette::Disabled, QPalette::WindowText, brush20);
        palette28.setBrush(QPalette::Disabled, QPalette::Button, brush17);
        palette28.setBrush(QPalette::Disabled, QPalette::Light, brush18);
        palette28.setBrush(QPalette::Disabled, QPalette::Midlight, brush19);
        palette28.setBrush(QPalette::Disabled, QPalette::Dark, brush20);
        palette28.setBrush(QPalette::Disabled, QPalette::Mid, brush21);
        palette28.setBrush(QPalette::Disabled, QPalette::Text, brush20);
        palette28.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette28.setBrush(QPalette::Disabled, QPalette::ButtonText, brush20);
        palette28.setBrush(QPalette::Disabled, QPalette::Base, brush17);
        palette28.setBrush(QPalette::Disabled, QPalette::Window, brush17);
        palette28.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette28.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush17);
        palette28.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette28.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        APIreadmePushButton->setPalette(palette28);
        APIreadmePushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_74->addWidget(APIreadmePushButton, 1, 1, 1, 1);


        gridLayout_33->addWidget(confAbsphotindirect3Frame, 2, 0, 1, 1);

        confsubtitleAbsphotindirect4Label = new QLabel(APIpage);
        confsubtitleAbsphotindirect4Label->setObjectName(QStringLiteral("confsubtitleAbsphotindirect4Label"));
        confsubtitleAbsphotindirect4Label->setFont(font);
        confsubtitleAbsphotindirect4Label->setWordWrap(true);

        gridLayout_33->addWidget(confsubtitleAbsphotindirect4Label, 3, 0, 1, 1);

        confAbsphotindirect4Frame = new QFrame(APIpage);
        confAbsphotindirect4Frame->setObjectName(QStringLiteral("confAbsphotindirect4Frame"));
        confAbsphotindirect4Frame->setEnabled(false);
        QPalette palette29;
        palette29.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette29.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette29.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette29.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette29.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette29.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette29.setBrush(QPalette::Active, QPalette::Text, brush);
        palette29.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette29.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette29.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette29.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette29.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette29.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette29.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette29.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette29.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette29.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette29.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette29.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette29.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette29.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette29.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette29.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette29.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette29.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette29.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette29.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette29.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette29.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette29.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette29.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette29.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette29.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette29.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette29.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette29.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette29.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette29.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette29.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette29.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette29.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette29.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette29.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette29.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette29.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confAbsphotindirect4Frame->setPalette(palette29);
        confAbsphotindirect4Frame->setAutoFillBackground(true);
        confAbsphotindirect4Frame->setFrameShape(QFrame::NoFrame);
        confAbsphotindirect4Frame->setFrameShadow(QFrame::Plain);
        gridLayout_58 = new QGridLayout(confAbsphotindirect4Frame);
        gridLayout_58->setObjectName(QStringLiteral("gridLayout_58"));
        gridLayout_57 = new QGridLayout();
        gridLayout_57->setObjectName(QStringLiteral("gridLayout_57"));
        APInight7PushButton = new QPushButton(confAbsphotindirect4Frame);
        APInight7PushButton->setObjectName(QStringLiteral("APInight7PushButton"));
        QSizePolicy sizePolicy7(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(APInight7PushButton->sizePolicy().hasHeightForWidth());
        APInight7PushButton->setSizePolicy(sizePolicy7);
        APInight7PushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_57->addWidget(APInight7PushButton, 6, 0, 1, 1);

        APInight1PushButton = new QPushButton(confAbsphotindirect4Frame);
        APInight1PushButton->setObjectName(QStringLiteral("APInight1PushButton"));
        sizePolicy7.setHeightForWidth(APInight1PushButton->sizePolicy().hasHeightForWidth());
        APInight1PushButton->setSizePolicy(sizePolicy7);
        APInight1PushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_57->addWidget(APInight1PushButton, 0, 0, 1, 1);

        APInight1ComboBox = new QComboBox(confAbsphotindirect4Frame);
        APInight1ComboBox->setObjectName(QStringLiteral("APInight1ComboBox"));
        sizePolicy7.setHeightForWidth(APInight1ComboBox->sizePolicy().hasHeightForWidth());
        APInight1ComboBox->setSizePolicy(sizePolicy7);
        APInight1ComboBox->setMinimumSize(QSize(140, 0));
        APInight1ComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_57->addWidget(APInight1ComboBox, 0, 1, 1, 1);

        APInight2PushButton = new QPushButton(confAbsphotindirect4Frame);
        APInight2PushButton->setObjectName(QStringLiteral("APInight2PushButton"));
        sizePolicy7.setHeightForWidth(APInight2PushButton->sizePolicy().hasHeightForWidth());
        APInight2PushButton->setSizePolicy(sizePolicy7);
        APInight2PushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_57->addWidget(APInight2PushButton, 1, 0, 1, 1);

        APInight2ComboBox = new QComboBox(confAbsphotindirect4Frame);
        APInight2ComboBox->setObjectName(QStringLiteral("APInight2ComboBox"));
        sizePolicy7.setHeightForWidth(APInight2ComboBox->sizePolicy().hasHeightForWidth());
        APInight2ComboBox->setSizePolicy(sizePolicy7);
        APInight2ComboBox->setMinimumSize(QSize(140, 0));
        APInight2ComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_57->addWidget(APInight2ComboBox, 1, 1, 1, 1);

        APInight3PushButton = new QPushButton(confAbsphotindirect4Frame);
        APInight3PushButton->setObjectName(QStringLiteral("APInight3PushButton"));
        sizePolicy7.setHeightForWidth(APInight3PushButton->sizePolicy().hasHeightForWidth());
        APInight3PushButton->setSizePolicy(sizePolicy7);
        APInight3PushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_57->addWidget(APInight3PushButton, 2, 0, 1, 1);

        APInight3ComboBox = new QComboBox(confAbsphotindirect4Frame);
        APInight3ComboBox->setObjectName(QStringLiteral("APInight3ComboBox"));
        sizePolicy7.setHeightForWidth(APInight3ComboBox->sizePolicy().hasHeightForWidth());
        APInight3ComboBox->setSizePolicy(sizePolicy7);
        APInight3ComboBox->setMinimumSize(QSize(140, 0));
        APInight3ComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_57->addWidget(APInight3ComboBox, 2, 1, 1, 1);

        APInight4PushButton = new QPushButton(confAbsphotindirect4Frame);
        APInight4PushButton->setObjectName(QStringLiteral("APInight4PushButton"));
        sizePolicy7.setHeightForWidth(APInight4PushButton->sizePolicy().hasHeightForWidth());
        APInight4PushButton->setSizePolicy(sizePolicy7);
        APInight4PushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_57->addWidget(APInight4PushButton, 3, 0, 1, 1);

        APInight4ComboBox = new QComboBox(confAbsphotindirect4Frame);
        APInight4ComboBox->setObjectName(QStringLiteral("APInight4ComboBox"));
        sizePolicy7.setHeightForWidth(APInight4ComboBox->sizePolicy().hasHeightForWidth());
        APInight4ComboBox->setSizePolicy(sizePolicy7);
        APInight4ComboBox->setMinimumSize(QSize(140, 0));
        APInight4ComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_57->addWidget(APInight4ComboBox, 3, 1, 1, 1);

        APInight5PushButton = new QPushButton(confAbsphotindirect4Frame);
        APInight5PushButton->setObjectName(QStringLiteral("APInight5PushButton"));
        sizePolicy7.setHeightForWidth(APInight5PushButton->sizePolicy().hasHeightForWidth());
        APInight5PushButton->setSizePolicy(sizePolicy7);
        APInight5PushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_57->addWidget(APInight5PushButton, 4, 0, 1, 1);

        APInight5ComboBox = new QComboBox(confAbsphotindirect4Frame);
        APInight5ComboBox->setObjectName(QStringLiteral("APInight5ComboBox"));
        sizePolicy7.setHeightForWidth(APInight5ComboBox->sizePolicy().hasHeightForWidth());
        APInight5ComboBox->setSizePolicy(sizePolicy7);
        APInight5ComboBox->setMinimumSize(QSize(140, 0));
        APInight5ComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_57->addWidget(APInight5ComboBox, 4, 1, 1, 1);

        APInight6PushButton = new QPushButton(confAbsphotindirect4Frame);
        APInight6PushButton->setObjectName(QStringLiteral("APInight6PushButton"));
        sizePolicy7.setHeightForWidth(APInight6PushButton->sizePolicy().hasHeightForWidth());
        APInight6PushButton->setSizePolicy(sizePolicy7);
        APInight6PushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_57->addWidget(APInight6PushButton, 5, 0, 1, 1);

        APInight6ComboBox = new QComboBox(confAbsphotindirect4Frame);
        APInight6ComboBox->setObjectName(QStringLiteral("APInight6ComboBox"));
        sizePolicy7.setHeightForWidth(APInight6ComboBox->sizePolicy().hasHeightForWidth());
        APInight6ComboBox->setSizePolicy(sizePolicy7);
        APInight6ComboBox->setMinimumSize(QSize(140, 0));
        APInight6ComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_57->addWidget(APInight6ComboBox, 5, 1, 1, 1);

        APInight7ComboBox = new QComboBox(confAbsphotindirect4Frame);
        APInight7ComboBox->setObjectName(QStringLiteral("APInight7ComboBox"));
        sizePolicy7.setHeightForWidth(APInight7ComboBox->sizePolicy().hasHeightForWidth());
        APInight7ComboBox->setSizePolicy(sizePolicy7);
        APInight7ComboBox->setMinimumSize(QSize(140, 0));
        APInight7ComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_57->addWidget(APInight7ComboBox, 6, 1, 1, 1);

        APInight8PushButton = new QPushButton(confAbsphotindirect4Frame);
        APInight8PushButton->setObjectName(QStringLiteral("APInight8PushButton"));
        sizePolicy7.setHeightForWidth(APInight8PushButton->sizePolicy().hasHeightForWidth());
        APInight8PushButton->setSizePolicy(sizePolicy7);
        APInight8PushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_57->addWidget(APInight8PushButton, 7, 0, 1, 1);

        APInight8ComboBox = new QComboBox(confAbsphotindirect4Frame);
        APInight8ComboBox->setObjectName(QStringLiteral("APInight8ComboBox"));
        sizePolicy7.setHeightForWidth(APInight8ComboBox->sizePolicy().hasHeightForWidth());
        APInight8ComboBox->setSizePolicy(sizePolicy7);
        APInight8ComboBox->setMinimumSize(QSize(140, 0));
        APInight8ComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_57->addWidget(APInight8ComboBox, 7, 1, 1, 1);

        APInight9PushButton = new QPushButton(confAbsphotindirect4Frame);
        APInight9PushButton->setObjectName(QStringLiteral("APInight9PushButton"));
        sizePolicy7.setHeightForWidth(APInight9PushButton->sizePolicy().hasHeightForWidth());
        APInight9PushButton->setSizePolicy(sizePolicy7);
        APInight9PushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_57->addWidget(APInight9PushButton, 8, 0, 1, 1);

        APInight9ComboBox = new QComboBox(confAbsphotindirect4Frame);
        APInight9ComboBox->setObjectName(QStringLiteral("APInight9ComboBox"));
        sizePolicy7.setHeightForWidth(APInight9ComboBox->sizePolicy().hasHeightForWidth());
        APInight9ComboBox->setSizePolicy(sizePolicy7);
        APInight9ComboBox->setMinimumSize(QSize(140, 0));
        APInight9ComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_57->addWidget(APInight9ComboBox, 8, 1, 1, 1);


        gridLayout_58->addLayout(gridLayout_57, 1, 0, 1, 1);

        APInodataLabel = new QLabel(confAbsphotindirect4Frame);
        APInodataLabel->setObjectName(QStringLiteral("APInodataLabel"));

        gridLayout_58->addWidget(APInodataLabel, 0, 0, 1, 1);


        gridLayout_33->addWidget(confAbsphotindirect4Frame, 4, 0, 1, 1);

        verticalSpacer_48 = new QSpacerItem(20, 68, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_33->addItem(verticalSpacer_48, 5, 0, 1, 1);


        gridLayout_62->addLayout(gridLayout_33, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(46, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_62->addItem(horizontalSpacer_2, 0, 2, 1, 1);

        confStackedWidget->addWidget(APIpage);
        APDpage = new QWidget();
        APDpage->setObjectName(QStringLiteral("APDpage"));
        gridLayout_34 = new QGridLayout(APDpage);
        gridLayout_34->setObjectName(QStringLiteral("gridLayout_34"));
        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(0);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        conftitleCreatesourcecatLabel = new QLabel(APDpage);
        conftitleCreatesourcecatLabel->setObjectName(QStringLiteral("conftitleCreatesourcecatLabel"));
        sizePolicy1.setHeightForWidth(conftitleCreatesourcecatLabel->sizePolicy().hasHeightForWidth());
        conftitleCreatesourcecatLabel->setSizePolicy(sizePolicy1);
        conftitleCreatesourcecatLabel->setFont(font);
        conftitleCreatesourcecatLabel->setAutoFillBackground(false);
        conftitleCreatesourcecatLabel->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleCreatesourcecatLabel->setWordWrap(true);

        verticalLayout_10->addWidget(conftitleCreatesourcecatLabel);

        confsubtitleCreatesourcecat1Label = new QLabel(APDpage);
        confsubtitleCreatesourcecat1Label->setObjectName(QStringLiteral("confsubtitleCreatesourcecat1Label"));
        confsubtitleCreatesourcecat1Label->setWordWrap(true);

        verticalLayout_10->addWidget(confsubtitleCreatesourcecat1Label);

        confCreatesourcecat1Frame = new QFrame(APDpage);
        confCreatesourcecat1Frame->setObjectName(QStringLiteral("confCreatesourcecat1Frame"));
        QPalette palette30;
        palette30.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette30.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette30.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette30.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette30.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette30.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette30.setBrush(QPalette::Active, QPalette::Text, brush);
        palette30.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette30.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette30.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette30.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette30.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette30.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette30.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette30.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette30.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette30.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette30.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette30.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette30.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette30.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette30.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette30.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette30.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette30.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette30.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette30.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette30.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette30.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette30.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette30.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette30.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette30.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette30.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette30.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette30.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette30.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette30.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette30.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette30.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette30.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette30.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette30.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette30.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette30.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confCreatesourcecat1Frame->setPalette(palette30);
        confCreatesourcecat1Frame->setAutoFillBackground(true);
        confCreatesourcecat1Frame->setFrameShape(QFrame::NoFrame);
        confCreatesourcecat1Frame->setFrameShadow(QFrame::Plain);
        gridLayout_16 = new QGridLayout(confCreatesourcecat1Frame);
        gridLayout_16->setObjectName(QStringLiteral("gridLayout_16"));
        label_20 = new QLabel(confCreatesourcecat1Frame);
        label_20->setObjectName(QStringLiteral("label_20"));

        gridLayout_16->addWidget(label_20, 0, 0, 1, 1);

        CSCMethodComboBox = new QComboBox(confCreatesourcecat1Frame);
        CSCMethodComboBox->setObjectName(QStringLiteral("CSCMethodComboBox"));
        sizePolicy6.setHeightForWidth(CSCMethodComboBox->sizePolicy().hasHeightForWidth());
        CSCMethodComboBox->setSizePolicy(sizePolicy6);
        CSCMethodComboBox->setFocusPolicy(Qt::NoFocus);

        gridLayout_16->addWidget(CSCMethodComboBox, 0, 1, 1, 2);

        confSourcecatDTLabel = new QLabel(confCreatesourcecat1Frame);
        confSourcecatDTLabel->setObjectName(QStringLiteral("confSourcecatDTLabel"));

        gridLayout_16->addWidget(confSourcecatDTLabel, 1, 0, 1, 1);

        horizontalSpacer_23 = new QSpacerItem(49, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_16->addItem(horizontalSpacer_23, 1, 1, 1, 1);

        CSCDTLineEdit = new QLineEdit(confCreatesourcecat1Frame);
        CSCDTLineEdit->setObjectName(QStringLiteral("CSCDTLineEdit"));
        CSCDTLineEdit->setMinimumSize(QSize(80, 0));
        CSCDTLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_16->addWidget(CSCDTLineEdit, 1, 2, 1, 1);

        confSourcecatDMINLabel = new QLabel(confCreatesourcecat1Frame);
        confSourcecatDMINLabel->setObjectName(QStringLiteral("confSourcecatDMINLabel"));

        gridLayout_16->addWidget(confSourcecatDMINLabel, 2, 0, 1, 1);

        horizontalSpacer_18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_16->addItem(horizontalSpacer_18, 2, 1, 1, 1);

        CSCDMINLineEdit = new QLineEdit(confCreatesourcecat1Frame);
        CSCDMINLineEdit->setObjectName(QStringLiteral("CSCDMINLineEdit"));
        CSCDMINLineEdit->setMinimumSize(QSize(80, 0));
        CSCDMINLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_16->addWidget(CSCDMINLineEdit, 2, 2, 1, 1);

        confSourcecatDeblendLabel = new QLabel(confCreatesourcecat1Frame);
        confSourcecatDeblendLabel->setObjectName(QStringLiteral("confSourcecatDeblendLabel"));

        gridLayout_16->addWidget(confSourcecatDeblendLabel, 3, 0, 1, 1);

        horizontalSpacer_24 = new QSpacerItem(49, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_16->addItem(horizontalSpacer_24, 3, 1, 1, 1);

        CSCmincontLineEdit = new QLineEdit(confCreatesourcecat1Frame);
        CSCmincontLineEdit->setObjectName(QStringLiteral("CSCmincontLineEdit"));
        CSCmincontLineEdit->setMinimumSize(QSize(80, 0));
        CSCmincontLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_16->addWidget(CSCmincontLineEdit, 3, 2, 1, 1);

        confSourcecatFWHMLabel = new QLabel(confCreatesourcecat1Frame);
        confSourcecatFWHMLabel->setObjectName(QStringLiteral("confSourcecatFWHMLabel"));

        gridLayout_16->addWidget(confSourcecatFWHMLabel, 4, 0, 1, 1);

        horizontalSpacer_25 = new QSpacerItem(49, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_16->addItem(horizontalSpacer_25, 4, 1, 1, 1);

        CSCFWHMLineEdit = new QLineEdit(confCreatesourcecat1Frame);
        CSCFWHMLineEdit->setObjectName(QStringLiteral("CSCFWHMLineEdit"));
        CSCFWHMLineEdit->setMinimumSize(QSize(80, 0));
        CSCFWHMLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_16->addWidget(CSCFWHMLineEdit, 4, 2, 1, 1);

        CSCconvolutionCheckBox = new QCheckBox(confCreatesourcecat1Frame);
        CSCconvolutionCheckBox->setObjectName(QStringLiteral("CSCconvolutionCheckBox"));
        CSCconvolutionCheckBox->setFocusPolicy(Qt::ClickFocus);
        CSCconvolutionCheckBox->setChecked(true);

        gridLayout_16->addWidget(CSCconvolutionCheckBox, 5, 0, 1, 1);


        verticalLayout_10->addWidget(confCreatesourcecat1Frame);

        confsubtitleCreatesourcecat2Label = new QLabel(APDpage);
        confsubtitleCreatesourcecat2Label->setObjectName(QStringLiteral("confsubtitleCreatesourcecat2Label"));
        confsubtitleCreatesourcecat2Label->setWordWrap(true);

        verticalLayout_10->addWidget(confsubtitleCreatesourcecat2Label);

        confCreatesourcecat2Frame = new QFrame(APDpage);
        confCreatesourcecat2Frame->setObjectName(QStringLiteral("confCreatesourcecat2Frame"));
        QPalette palette31;
        palette31.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette31.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette31.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette31.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette31.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette31.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette31.setBrush(QPalette::Active, QPalette::Text, brush);
        palette31.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette31.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette31.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette31.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette31.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette31.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette31.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette31.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette31.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette31.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette31.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette31.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette31.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette31.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette31.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette31.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette31.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette31.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette31.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette31.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette31.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette31.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette31.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette31.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette31.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette31.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette31.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette31.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette31.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette31.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette31.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette31.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette31.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette31.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette31.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette31.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette31.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette31.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confCreatesourcecat2Frame->setPalette(palette31);
        confCreatesourcecat2Frame->setAutoFillBackground(true);
        confCreatesourcecat2Frame->setFrameShape(QFrame::NoFrame);
        confCreatesourcecat2Frame->setFrameShadow(QFrame::Plain);
        gridLayout_44 = new QGridLayout(confCreatesourcecat2Frame);
        gridLayout_44->setObjectName(QStringLiteral("gridLayout_44"));
        confSourcecatSaturationLabel = new QLabel(confCreatesourcecat2Frame);
        confSourcecatSaturationLabel->setObjectName(QStringLiteral("confSourcecatSaturationLabel"));

        gridLayout_44->addWidget(confSourcecatSaturationLabel, 2, 0, 1, 1);

        CSCmaxflagLineEdit = new QLineEdit(confCreatesourcecat2Frame);
        CSCmaxflagLineEdit->setObjectName(QStringLiteral("CSCmaxflagLineEdit"));
        CSCmaxflagLineEdit->setMinimumSize(QSize(80, 0));
        CSCmaxflagLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_44->addWidget(CSCmaxflagLineEdit, 1, 1, 1, 1);

        CSCbackgroundLineEdit = new QLineEdit(confCreatesourcecat2Frame);
        CSCbackgroundLineEdit->setObjectName(QStringLiteral("CSCbackgroundLineEdit"));
        CSCbackgroundLineEdit->setMinimumSize(QSize(80, 0));
        CSCbackgroundLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_44->addWidget(CSCbackgroundLineEdit, 3, 1, 1, 1);

        confSourcecatBackgroundLabel = new QLabel(confCreatesourcecat2Frame);
        confSourcecatBackgroundLabel->setObjectName(QStringLiteral("confSourcecatBackgroundLabel"));

        gridLayout_44->addWidget(confSourcecatBackgroundLabel, 3, 0, 1, 1);

        CSCsaturationLineEdit = new QLineEdit(confCreatesourcecat2Frame);
        CSCsaturationLineEdit->setObjectName(QStringLiteral("CSCsaturationLineEdit"));
        CSCsaturationLineEdit->setMinimumSize(QSize(80, 0));
        CSCsaturationLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_44->addWidget(CSCsaturationLineEdit, 2, 1, 1, 1);

        confSourcecatFLAGLabel = new QLabel(confCreatesourcecat2Frame);
        confSourcecatFLAGLabel->setObjectName(QStringLiteral("confSourcecatFLAGLabel"));

        gridLayout_44->addWidget(confSourcecatFLAGLabel, 1, 0, 1, 1);

        CSCsamplingCheckBox = new QCheckBox(confCreatesourcecat2Frame);
        CSCsamplingCheckBox->setObjectName(QStringLiteral("CSCsamplingCheckBox"));
        CSCsamplingCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_44->addWidget(CSCsamplingCheckBox, 4, 0, 1, 1);


        verticalLayout_10->addWidget(confCreatesourcecat2Frame);

        confsubtitleCreatesourcecat3Label = new QLabel(APDpage);
        confsubtitleCreatesourcecat3Label->setObjectName(QStringLiteral("confsubtitleCreatesourcecat3Label"));
        confsubtitleCreatesourcecat3Label->setWordWrap(true);

        verticalLayout_10->addWidget(confsubtitleCreatesourcecat3Label);

        confCreatesourcecat3Frame = new QFrame(APDpage);
        confCreatesourcecat3Frame->setObjectName(QStringLiteral("confCreatesourcecat3Frame"));
        QPalette palette32;
        palette32.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette32.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette32.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette32.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette32.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette32.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette32.setBrush(QPalette::Active, QPalette::Text, brush);
        palette32.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette32.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette32.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette32.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette32.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette32.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette32.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette32.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette32.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette32.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette32.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette32.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette32.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette32.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette32.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette32.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette32.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette32.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette32.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette32.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette32.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette32.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette32.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette32.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette32.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette32.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette32.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette32.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette32.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette32.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette32.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette32.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette32.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette32.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette32.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette32.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette32.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette32.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confCreatesourcecat3Frame->setPalette(palette32);
        confCreatesourcecat3Frame->setAutoFillBackground(true);
        confCreatesourcecat3Frame->setFrameShape(QFrame::NoFrame);
        confCreatesourcecat3Frame->setFrameShadow(QFrame::Plain);
        gridLayout_46 = new QGridLayout(confCreatesourcecat3Frame);
        gridLayout_46->setObjectName(QStringLiteral("gridLayout_46"));
        confSourcecatFLAGLabel_2 = new QLabel(confCreatesourcecat3Frame);
        confSourcecatFLAGLabel_2->setObjectName(QStringLiteral("confSourcecatFLAGLabel_2"));
        confSourcecatFLAGLabel_2->setWordWrap(true);

        gridLayout_46->addWidget(confSourcecatFLAGLabel_2, 0, 0, 2, 1);

        CSCrejectExposureLineEdit = new QLineEdit(confCreatesourcecat3Frame);
        CSCrejectExposureLineEdit->setObjectName(QStringLiteral("CSCrejectExposureLineEdit"));
        CSCrejectExposureLineEdit->setMinimumSize(QSize(80, 0));
        CSCrejectExposureLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_46->addWidget(CSCrejectExposureLineEdit, 0, 1, 1, 1);


        verticalLayout_10->addWidget(confCreatesourcecat3Frame);

        verticalSpacer_52 = new QSpacerItem(267, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_10->addItem(verticalSpacer_52);


        gridLayout_34->addLayout(verticalLayout_10, 0, 0, 1, 1);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(0);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        conftitleGetCatalogLabel = new QLabel(APDpage);
        conftitleGetCatalogLabel->setObjectName(QStringLiteral("conftitleGetCatalogLabel"));
        sizePolicy1.setHeightForWidth(conftitleGetCatalogLabel->sizePolicy().hasHeightForWidth());
        conftitleGetCatalogLabel->setSizePolicy(sizePolicy1);
        conftitleGetCatalogLabel->setFont(font);
        conftitleGetCatalogLabel->setAutoFillBackground(false);
        conftitleGetCatalogLabel->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleGetCatalogLabel->setWordWrap(true);

        verticalLayout_9->addWidget(conftitleGetCatalogLabel);

        confsubtitleGetCatalog1Label = new QLabel(APDpage);
        confsubtitleGetCatalog1Label->setObjectName(QStringLiteral("confsubtitleGetCatalog1Label"));
        confsubtitleGetCatalog1Label->setWordWrap(true);

        verticalLayout_9->addWidget(confsubtitleGetCatalog1Label);

        confGetCatalog1Frame = new QFrame(APDpage);
        confGetCatalog1Frame->setObjectName(QStringLiteral("confGetCatalog1Frame"));
        QPalette palette33;
        palette33.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette33.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette33.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette33.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette33.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette33.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette33.setBrush(QPalette::Active, QPalette::Text, brush);
        palette33.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette33.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette33.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette33.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette33.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette33.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette33.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette33.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette33.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette33.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette33.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette33.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette33.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette33.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette33.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette33.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette33.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette33.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette33.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette33.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette33.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette33.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette33.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette33.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette33.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette33.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette33.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette33.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette33.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette33.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette33.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette33.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette33.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette33.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette33.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette33.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette33.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette33.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confGetCatalog1Frame->setPalette(palette33);
        confGetCatalog1Frame->setAutoFillBackground(true);
        confGetCatalog1Frame->setFrameShape(QFrame::StyledPanel);
        confGetCatalog1Frame->setFrameShadow(QFrame::Raised);
        gridLayout_50 = new QGridLayout(confGetCatalog1Frame);
        gridLayout_50->setObjectName(QStringLiteral("gridLayout_50"));
        ARCwebRadioButton = new QRadioButton(confGetCatalog1Frame);
        ARCwebRadioButton->setObjectName(QStringLiteral("ARCwebRadioButton"));
        ARCwebRadioButton->setFocusPolicy(Qt::ClickFocus);
        ARCwebRadioButton->setChecked(true);

        gridLayout_50->addWidget(ARCwebRadioButton, 0, 0, 1, 1);

        ARCimageRadioButton = new QRadioButton(confGetCatalog1Frame);
        ARCimageRadioButton->setObjectName(QStringLiteral("ARCimageRadioButton"));
        ARCimageRadioButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_50->addWidget(ARCimageRadioButton, 1, 0, 1, 1);


        verticalLayout_9->addWidget(confGetCatalog1Frame);

        confsubtitleGetCatalog2Label = new QLabel(APDpage);
        confsubtitleGetCatalog2Label->setObjectName(QStringLiteral("confsubtitleGetCatalog2Label"));
        confsubtitleGetCatalog2Label->setWordWrap(true);

        verticalLayout_9->addWidget(confsubtitleGetCatalog2Label);

        confGetCatalog2Frame = new QFrame(APDpage);
        confGetCatalog2Frame->setObjectName(QStringLiteral("confGetCatalog2Frame"));
        QPalette palette34;
        palette34.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette34.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette34.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette34.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette34.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette34.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette34.setBrush(QPalette::Active, QPalette::Text, brush);
        palette34.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette34.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette34.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette34.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette34.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette34.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette34.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette34.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette34.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette34.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette34.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette34.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette34.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette34.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette34.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette34.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette34.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette34.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette34.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette34.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette34.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette34.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette34.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette34.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette34.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette34.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette34.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette34.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette34.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette34.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette34.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette34.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette34.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette34.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette34.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette34.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette34.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette34.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confGetCatalog2Frame->setPalette(palette34);
        confGetCatalog2Frame->setAutoFillBackground(true);
        confGetCatalog2Frame->setFrameShape(QFrame::NoFrame);
        confGetCatalog2Frame->setFrameShadow(QFrame::Plain);
        gridLayout_61 = new QGridLayout(confGetCatalog2Frame);
        gridLayout_61->setObjectName(QStringLiteral("gridLayout_61"));
        horizontalSpacer_19 = new QSpacerItem(52, 13, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        gridLayout_61->addItem(horizontalSpacer_19, 1, 0, 1, 1);

        ARCgetcatalogPushButton = new QPushButton(confGetCatalog2Frame);
        ARCgetcatalogPushButton->setObjectName(QStringLiteral("ARCgetcatalogPushButton"));
        sizePolicy6.setHeightForWidth(ARCgetcatalogPushButton->sizePolicy().hasHeightForWidth());
        ARCgetcatalogPushButton->setSizePolicy(sizePolicy6);
        ARCgetcatalogPushButton->setMinimumSize(QSize(150, 0));
        QPalette palette35;
        palette35.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush23(QColor(68, 216, 204, 255));
        brush23.setStyle(Qt::SolidPattern);
        palette35.setBrush(QPalette::Active, QPalette::Button, brush23);
        QBrush brush24(QColor(149, 255, 247, 255));
        brush24.setStyle(Qt::SolidPattern);
        palette35.setBrush(QPalette::Active, QPalette::Light, brush24);
        QBrush brush25(QColor(108, 235, 225, 255));
        brush25.setStyle(Qt::SolidPattern);
        palette35.setBrush(QPalette::Active, QPalette::Midlight, brush25);
        QBrush brush26(QColor(34, 108, 102, 255));
        brush26.setStyle(Qt::SolidPattern);
        palette35.setBrush(QPalette::Active, QPalette::Dark, brush26);
        QBrush brush27(QColor(45, 144, 136, 255));
        brush27.setStyle(Qt::SolidPattern);
        palette35.setBrush(QPalette::Active, QPalette::Mid, brush27);
        palette35.setBrush(QPalette::Active, QPalette::Text, brush);
        palette35.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette35.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette35.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette35.setBrush(QPalette::Active, QPalette::Window, brush23);
        palette35.setBrush(QPalette::Active, QPalette::Shadow, brush);
        QBrush brush28(QColor(161, 235, 229, 255));
        brush28.setStyle(Qt::SolidPattern);
        palette35.setBrush(QPalette::Active, QPalette::AlternateBase, brush28);
        palette35.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette35.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette35.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette35.setBrush(QPalette::Inactive, QPalette::Button, brush23);
        palette35.setBrush(QPalette::Inactive, QPalette::Light, brush24);
        palette35.setBrush(QPalette::Inactive, QPalette::Midlight, brush25);
        palette35.setBrush(QPalette::Inactive, QPalette::Dark, brush26);
        palette35.setBrush(QPalette::Inactive, QPalette::Mid, brush27);
        palette35.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette35.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette35.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette35.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette35.setBrush(QPalette::Inactive, QPalette::Window, brush23);
        palette35.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette35.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush28);
        palette35.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette35.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette35.setBrush(QPalette::Disabled, QPalette::WindowText, brush26);
        palette35.setBrush(QPalette::Disabled, QPalette::Button, brush23);
        palette35.setBrush(QPalette::Disabled, QPalette::Light, brush24);
        palette35.setBrush(QPalette::Disabled, QPalette::Midlight, brush25);
        palette35.setBrush(QPalette::Disabled, QPalette::Dark, brush26);
        palette35.setBrush(QPalette::Disabled, QPalette::Mid, brush27);
        palette35.setBrush(QPalette::Disabled, QPalette::Text, brush26);
        palette35.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette35.setBrush(QPalette::Disabled, QPalette::ButtonText, brush26);
        palette35.setBrush(QPalette::Disabled, QPalette::Base, brush23);
        palette35.setBrush(QPalette::Disabled, QPalette::Window, brush23);
        palette35.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette35.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush23);
        palette35.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette35.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        ARCgetcatalogPushButton->setPalette(palette35);
        ARCgetcatalogPushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_61->addWidget(ARCgetcatalogPushButton, 1, 1, 1, 1);

        ARCstackedWidget = new QStackedWidget(confGetCatalog2Frame);
        ARCstackedWidget->setObjectName(QStringLiteral("ARCstackedWidget"));
        QPalette palette36;
        palette36.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette36.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette36.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette36.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette36.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette36.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette36.setBrush(QPalette::Active, QPalette::Text, brush);
        palette36.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette36.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette36.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette36.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette36.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette36.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette36.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette36.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette36.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette36.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette36.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette36.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette36.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette36.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette36.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette36.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette36.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette36.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette36.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette36.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette36.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette36.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette36.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette36.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette36.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette36.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette36.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette36.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette36.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette36.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette36.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette36.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette36.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette36.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette36.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette36.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette36.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette36.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        ARCstackedWidget->setPalette(palette36);
        ARCstackedWidget->setAutoFillBackground(true);
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        gridLayout_36 = new QGridLayout(page);
        gridLayout_36->setObjectName(QStringLiteral("gridLayout_36"));
        ARCserverLabel_2 = new QLabel(page);
        ARCserverLabel_2->setObjectName(QStringLiteral("ARCserverLabel_2"));

        gridLayout_36->addWidget(ARCserverLabel_2, 0, 0, 1, 1);

        gaiaFrame = new QFrame(page);
        gaiaFrame->setObjectName(QStringLiteral("gaiaFrame"));
        QPalette palette37;
        palette37.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush29(QColor(219, 219, 220, 255));
        brush29.setStyle(Qt::SolidPattern);
        palette37.setBrush(QPalette::Active, QPalette::Button, brush29);
        palette37.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush30(QColor(237, 237, 237, 255));
        brush30.setStyle(Qt::SolidPattern);
        palette37.setBrush(QPalette::Active, QPalette::Midlight, brush30);
        QBrush brush31(QColor(109, 109, 110, 255));
        brush31.setStyle(Qt::SolidPattern);
        palette37.setBrush(QPalette::Active, QPalette::Dark, brush31);
        QBrush brush32(QColor(146, 146, 147, 255));
        brush32.setStyle(Qt::SolidPattern);
        palette37.setBrush(QPalette::Active, QPalette::Mid, brush32);
        palette37.setBrush(QPalette::Active, QPalette::Text, brush);
        palette37.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette37.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette37.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette37.setBrush(QPalette::Active, QPalette::Window, brush29);
        palette37.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette37.setBrush(QPalette::Active, QPalette::AlternateBase, brush30);
        palette37.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette37.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette37.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette37.setBrush(QPalette::Inactive, QPalette::Button, brush29);
        palette37.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette37.setBrush(QPalette::Inactive, QPalette::Midlight, brush30);
        palette37.setBrush(QPalette::Inactive, QPalette::Dark, brush31);
        palette37.setBrush(QPalette::Inactive, QPalette::Mid, brush32);
        palette37.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette37.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette37.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette37.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette37.setBrush(QPalette::Inactive, QPalette::Window, brush29);
        palette37.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette37.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush30);
        palette37.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette37.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette37.setBrush(QPalette::Disabled, QPalette::WindowText, brush31);
        palette37.setBrush(QPalette::Disabled, QPalette::Button, brush29);
        palette37.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette37.setBrush(QPalette::Disabled, QPalette::Midlight, brush30);
        palette37.setBrush(QPalette::Disabled, QPalette::Dark, brush31);
        palette37.setBrush(QPalette::Disabled, QPalette::Mid, brush32);
        palette37.setBrush(QPalette::Disabled, QPalette::Text, brush31);
        palette37.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette37.setBrush(QPalette::Disabled, QPalette::ButtonText, brush31);
        palette37.setBrush(QPalette::Disabled, QPalette::Base, brush29);
        palette37.setBrush(QPalette::Disabled, QPalette::Window, brush29);
        palette37.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette37.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush29);
        palette37.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette37.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        gaiaFrame->setPalette(palette37);
        gaiaFrame->setAutoFillBackground(true);
        gaiaFrame->setFrameShape(QFrame::StyledPanel);
        gaiaFrame->setFrameShadow(QFrame::Raised);
        gridLayout_29 = new QGridLayout(gaiaFrame);
        gridLayout_29->setObjectName(QStringLiteral("gridLayout_29"));
        ARCgaiaLabel = new QLabel(gaiaFrame);
        ARCgaiaLabel->setObjectName(QStringLiteral("ARCgaiaLabel"));
        ARCgaiaLabel->setWordWrap(true);

        gridLayout_29->addWidget(ARCgaiaLabel, 0, 0, 1, 3);

        ARCmaxpmLabel = new QLabel(gaiaFrame);
        ARCmaxpmLabel->setObjectName(QStringLiteral("ARCmaxpmLabel"));

        gridLayout_29->addWidget(ARCmaxpmLabel, 1, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(28, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_29->addItem(horizontalSpacer, 1, 1, 1, 1);

        ARCmaxpmLineEdit = new QLineEdit(gaiaFrame);
        ARCmaxpmLineEdit->setObjectName(QStringLiteral("ARCmaxpmLineEdit"));

        gridLayout_29->addWidget(ARCmaxpmLineEdit, 1, 2, 1, 1);

        ARCpmRALabel = new QLabel(gaiaFrame);
        ARCpmRALabel->setObjectName(QStringLiteral("ARCpmRALabel"));

        gridLayout_29->addWidget(ARCpmRALabel, 2, 0, 1, 1);

        ARCpmRALineEdit = new QLineEdit(gaiaFrame);
        ARCpmRALineEdit->setObjectName(QStringLiteral("ARCpmRALineEdit"));
        QPalette palette38;
        QBrush brush33(QColor(255, 255, 238, 255));
        brush33.setStyle(Qt::SolidPattern);
        palette38.setBrush(QPalette::Active, QPalette::Base, brush33);
        palette38.setBrush(QPalette::Inactive, QPalette::Base, brush33);
        palette38.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        ARCpmRALineEdit->setPalette(palette38);
        ARCpmRALineEdit->setReadOnly(true);

        gridLayout_29->addWidget(ARCpmRALineEdit, 2, 1, 1, 2);

        ARCpmDECLabel = new QLabel(gaiaFrame);
        ARCpmDECLabel->setObjectName(QStringLiteral("ARCpmDECLabel"));

        gridLayout_29->addWidget(ARCpmDECLabel, 3, 0, 1, 1);

        ARCpmDECLineEdit = new QLineEdit(gaiaFrame);
        ARCpmDECLineEdit->setObjectName(QStringLiteral("ARCpmDECLineEdit"));
        QPalette palette39;
        palette39.setBrush(QPalette::Active, QPalette::Base, brush33);
        palette39.setBrush(QPalette::Inactive, QPalette::Base, brush33);
        palette39.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        ARCpmDECLineEdit->setPalette(palette39);
        ARCpmDECLineEdit->setReadOnly(true);

        gridLayout_29->addWidget(ARCpmDECLineEdit, 3, 1, 1, 2);


        gridLayout_36->addWidget(gaiaFrame, 1, 0, 1, 3);

        ARCraLabel = new QLabel(page);
        ARCraLabel->setObjectName(QStringLiteral("ARCraLabel"));

        gridLayout_36->addWidget(ARCraLabel, 2, 0, 1, 1);

        ARCraLineEdit = new QLineEdit(page);
        ARCraLineEdit->setObjectName(QStringLiteral("ARCraLineEdit"));

        gridLayout_36->addWidget(ARCraLineEdit, 2, 2, 1, 1);

        ARCdecLabel = new QLabel(page);
        ARCdecLabel->setObjectName(QStringLiteral("ARCdecLabel"));

        gridLayout_36->addWidget(ARCdecLabel, 3, 0, 1, 1);

        ARCdecLineEdit = new QLineEdit(page);
        ARCdecLineEdit->setObjectName(QStringLiteral("ARCdecLineEdit"));

        gridLayout_36->addWidget(ARCdecLineEdit, 3, 2, 1, 1);

        ARCtargetresolverLabel = new QLabel(page);
        ARCtargetresolverLabel->setObjectName(QStringLiteral("ARCtargetresolverLabel"));

        gridLayout_36->addWidget(ARCtargetresolverLabel, 4, 0, 1, 1);

        ARCtargetresolverToolButton = new QToolButton(page);
        ARCtargetresolverToolButton->setObjectName(QStringLiteral("ARCtargetresolverToolButton"));
        ARCtargetresolverToolButton->setMinimumSize(QSize(30, 30));
        ARCtargetresolverToolButton->setMaximumSize(QSize(30, 30));
        ARCtargetresolverToolButton->setFocusPolicy(Qt::ClickFocus);
        QIcon icon9;
        icon9.addFile(QStringLiteral("icons/magnifyer.png"), QSize(), QIcon::Normal, QIcon::Off);
        ARCtargetresolverToolButton->setIcon(icon9);
        ARCtargetresolverToolButton->setIconSize(QSize(49, 49));
        ARCtargetresolverToolButton->setCheckable(false);
        ARCtargetresolverToolButton->setAutoExclusive(true);
        ARCtargetresolverToolButton->setAutoRaise(true);

        gridLayout_36->addWidget(ARCtargetresolverToolButton, 4, 1, 1, 1);

        ARCtargetresolverLineEdit = new QLineEdit(page);
        ARCtargetresolverLineEdit->setObjectName(QStringLiteral("ARCtargetresolverLineEdit"));

        gridLayout_36->addWidget(ARCtargetresolverLineEdit, 4, 2, 1, 1);

        ARCminmagLabel = new QLabel(page);
        ARCminmagLabel->setObjectName(QStringLiteral("ARCminmagLabel"));

        gridLayout_36->addWidget(ARCminmagLabel, 5, 0, 1, 1);

        ARCminmagLineEdit = new QLineEdit(page);
        ARCminmagLineEdit->setObjectName(QStringLiteral("ARCminmagLineEdit"));

        gridLayout_36->addWidget(ARCminmagLineEdit, 5, 2, 1, 1);

        ARCradiusLabel = new QLabel(page);
        ARCradiusLabel->setObjectName(QStringLiteral("ARCradiusLabel"));

        gridLayout_36->addWidget(ARCradiusLabel, 6, 0, 1, 1);

        ARCradiusLineEdit = new QLineEdit(page);
        ARCradiusLineEdit->setObjectName(QStringLiteral("ARCradiusLineEdit"));

        gridLayout_36->addWidget(ARCradiusLineEdit, 6, 2, 1, 1);

        verticalSpacer_10 = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_36->addItem(verticalSpacer_10, 7, 2, 1, 1);

        ARCcatalogComboBox = new QComboBox(page);
        ARCcatalogComboBox->setObjectName(QStringLiteral("ARCcatalogComboBox"));
        ARCcatalogComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_36->addWidget(ARCcatalogComboBox, 0, 2, 1, 1);

        ARCstackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        gridLayout_45 = new QGridLayout(page_2);
        gridLayout_45->setObjectName(QStringLiteral("gridLayout_45"));
        gridLayout_26 = new QGridLayout();
        gridLayout_26->setObjectName(QStringLiteral("gridLayout_26"));
        confSourcecatDTLabel_3 = new QLabel(page_2);
        confSourcecatDTLabel_3->setObjectName(QStringLiteral("confSourcecatDTLabel_3"));

        gridLayout_26->addWidget(confSourcecatDTLabel_3, 2, 0, 1, 1);

        ARCDMINLineEdit = new QLineEdit(page_2);
        ARCDMINLineEdit->setObjectName(QStringLiteral("ARCDMINLineEdit"));
        ARCDMINLineEdit->setMinimumSize(QSize(80, 0));
        ARCDMINLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_26->addWidget(ARCDMINLineEdit, 3, 1, 1, 1);

        ARCselectimageLineEdit = new QLineEdit(page_2);
        ARCselectimageLineEdit->setObjectName(QStringLiteral("ARCselectimageLineEdit"));
        ARCselectimageLineEdit->setMinimumSize(QSize(200, 0));

        gridLayout_26->addWidget(ARCselectimageLineEdit, 1, 0, 1, 3);

        confSourcecatDTLabel_4 = new QLabel(page_2);
        confSourcecatDTLabel_4->setObjectName(QStringLiteral("confSourcecatDTLabel_4"));

        gridLayout_26->addWidget(confSourcecatDTLabel_4, 2, 1, 1, 1);

        ARCselectimagePushButton = new QPushButton(page_2);
        ARCselectimagePushButton->setObjectName(QStringLiteral("ARCselectimagePushButton"));
        ARCselectimagePushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_26->addWidget(ARCselectimagePushButton, 0, 0, 1, 3);

        ARCDTLineEdit = new QLineEdit(page_2);
        ARCDTLineEdit->setObjectName(QStringLiteral("ARCDTLineEdit"));
        ARCDTLineEdit->setMinimumSize(QSize(80, 0));
        ARCDTLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_26->addWidget(ARCDTLineEdit, 3, 0, 1, 1);


        gridLayout_45->addLayout(gridLayout_26, 0, 0, 1, 1);

        verticalSpacer_9 = new QSpacerItem(20, 247, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_45->addItem(verticalSpacer_9, 1, 0, 1, 1);

        ARCstackedWidget->addWidget(page_2);

        gridLayout_61->addWidget(ARCstackedWidget, 0, 0, 1, 2);


        verticalLayout_9->addWidget(confGetCatalog2Frame);

        verticalSpacer_7 = new QSpacerItem(326, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_7);


        gridLayout_34->addLayout(verticalLayout_9, 0, 1, 1, 1);

        horizontalSpacer_13 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_34->addItem(horizontalSpacer_13, 0, 2, 1, 1);

        confStackedWidget->addWidget(APDpage);
        ARCpage = new QWidget();
        ARCpage->setObjectName(QStringLiteral("ARCpage"));
        gridLayout_35 = new QGridLayout(ARCpage);
        gridLayout_35->setObjectName(QStringLiteral("gridLayout_35"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(0);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        conftitleAstromphotom1Label = new QLabel(ARCpage);
        conftitleAstromphotom1Label->setObjectName(QStringLiteral("conftitleAstromphotom1Label"));
        sizePolicy1.setHeightForWidth(conftitleAstromphotom1Label->sizePolicy().hasHeightForWidth());
        conftitleAstromphotom1Label->setSizePolicy(sizePolicy1);
        QPalette palette40;
        QBrush brush34(QColor(100, 230, 230, 255));
        brush34.setStyle(Qt::SolidPattern);
        palette40.setBrush(QPalette::Active, QPalette::WindowText, brush34);
        QBrush brush35(QColor(58, 78, 98, 255));
        brush35.setStyle(Qt::SolidPattern);
        palette40.setBrush(QPalette::Active, QPalette::Button, brush35);
        palette40.setBrush(QPalette::Active, QPalette::Text, brush34);
        palette40.setBrush(QPalette::Active, QPalette::ButtonText, brush34);
        palette40.setBrush(QPalette::Active, QPalette::Base, brush35);
        palette40.setBrush(QPalette::Active, QPalette::Window, brush35);
        palette40.setBrush(QPalette::Inactive, QPalette::WindowText, brush34);
        palette40.setBrush(QPalette::Inactive, QPalette::Button, brush35);
        palette40.setBrush(QPalette::Inactive, QPalette::Text, brush34);
        palette40.setBrush(QPalette::Inactive, QPalette::ButtonText, brush34);
        palette40.setBrush(QPalette::Inactive, QPalette::Base, brush35);
        palette40.setBrush(QPalette::Inactive, QPalette::Window, brush35);
        palette40.setBrush(QPalette::Disabled, QPalette::WindowText, brush34);
        palette40.setBrush(QPalette::Disabled, QPalette::Button, brush35);
        palette40.setBrush(QPalette::Disabled, QPalette::Text, brush34);
        palette40.setBrush(QPalette::Disabled, QPalette::ButtonText, brush34);
        palette40.setBrush(QPalette::Disabled, QPalette::Base, brush35);
        palette40.setBrush(QPalette::Disabled, QPalette::Window, brush35);
        conftitleAstromphotom1Label->setPalette(palette40);
        conftitleAstromphotom1Label->setFont(font);
        conftitleAstromphotom1Label->setAutoFillBackground(false);
        conftitleAstromphotom1Label->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleAstromphotom1Label->setWordWrap(true);

        verticalLayout_8->addWidget(conftitleAstromphotom1Label);

        confsubtitleAstromphotom1Label = new QLabel(ARCpage);
        confsubtitleAstromphotom1Label->setObjectName(QStringLiteral("confsubtitleAstromphotom1Label"));
        confsubtitleAstromphotom1Label->setFont(font);
        confsubtitleAstromphotom1Label->setWordWrap(true);

        verticalLayout_8->addWidget(confsubtitleAstromphotom1Label);

        confAstromphotom1Frame = new QFrame(ARCpage);
        confAstromphotom1Frame->setObjectName(QStringLiteral("confAstromphotom1Frame"));
        QPalette palette41;
        palette41.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette41.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette41.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette41.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette41.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette41.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette41.setBrush(QPalette::Active, QPalette::Text, brush);
        palette41.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette41.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette41.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette41.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette41.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette41.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette41.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette41.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette41.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette41.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette41.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette41.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette41.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette41.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette41.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette41.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette41.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette41.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette41.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette41.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette41.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette41.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette41.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette41.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette41.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette41.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette41.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette41.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette41.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette41.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette41.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette41.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette41.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette41.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette41.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette41.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette41.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette41.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confAstromphotom1Frame->setPalette(palette41);
        confAstromphotom1Frame->setAutoFillBackground(true);
        confAstromphotom1Frame->setFrameShape(QFrame::NoFrame);
        confAstromphotom1Frame->setFrameShadow(QFrame::Plain);
        gridLayout_60 = new QGridLayout(confAstromphotom1Frame);
        gridLayout_60->setObjectName(QStringLiteral("gridLayout_60"));
        ASTxcorrDTLineEdit = new QLineEdit(confAstromphotom1Frame);
        ASTxcorrDTLineEdit->setObjectName(QStringLiteral("ASTxcorrDTLineEdit"));
        ASTxcorrDTLineEdit->setMinimumSize(QSize(80, 0));
        ASTxcorrDTLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_60->addWidget(ASTxcorrDTLineEdit, 1, 1, 1, 1);

        ASTxcorrDMINLabel = new QLabel(confAstromphotom1Frame);
        ASTxcorrDMINLabel->setObjectName(QStringLiteral("ASTxcorrDMINLabel"));

        gridLayout_60->addWidget(ASTxcorrDMINLabel, 1, 2, 1, 1);

        ASTxcorrDMINLineEdit = new QLineEdit(confAstromphotom1Frame);
        ASTxcorrDMINLineEdit->setObjectName(QStringLiteral("ASTxcorrDMINLineEdit"));
        ASTxcorrDMINLineEdit->setMinimumSize(QSize(80, 0));
        ASTxcorrDMINLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_60->addWidget(ASTxcorrDMINLineEdit, 1, 3, 1, 1);

        ASTxcorrDTLabel = new QLabel(confAstromphotom1Frame);
        ASTxcorrDTLabel->setObjectName(QStringLiteral("ASTxcorrDTLabel"));

        gridLayout_60->addWidget(ASTxcorrDTLabel, 1, 0, 1, 1);

        ASTmethodComboBox = new QComboBox(confAstromphotom1Frame);
        ASTmethodComboBox->setObjectName(QStringLiteral("ASTmethodComboBox"));
        ASTmethodComboBox->setEnabled(false);
        sizePolicy7.setHeightForWidth(ASTmethodComboBox->sizePolicy().hasHeightForWidth());
        ASTmethodComboBox->setSizePolicy(sizePolicy7);
        QPalette palette42;
        palette42.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette42.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette42.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette42.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette42.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette42.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette42.setBrush(QPalette::Active, QPalette::Text, brush);
        palette42.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette42.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette42.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette42.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette42.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette42.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette42.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette42.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette42.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette42.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette42.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette42.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette42.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette42.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette42.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette42.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette42.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette42.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette42.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette42.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette42.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette42.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette42.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette42.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette42.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette42.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette42.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette42.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette42.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette42.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette42.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette42.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette42.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette42.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette42.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette42.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette42.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette42.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        ASTmethodComboBox->setPalette(palette42);
        ASTmethodComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_60->addWidget(ASTmethodComboBox, 0, 0, 1, 2);

        restoreHeaderPushButton = new QPushButton(confAstromphotom1Frame);
        restoreHeaderPushButton->setObjectName(QStringLiteral("restoreHeaderPushButton"));
        sizePolicy6.setHeightForWidth(restoreHeaderPushButton->sizePolicy().hasHeightForWidth());
        restoreHeaderPushButton->setSizePolicy(sizePolicy6);
        QPalette palette43;
        palette43.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette43.setBrush(QPalette::Active, QPalette::Button, brush23);
        palette43.setBrush(QPalette::Active, QPalette::Light, brush24);
        palette43.setBrush(QPalette::Active, QPalette::Midlight, brush25);
        palette43.setBrush(QPalette::Active, QPalette::Dark, brush26);
        palette43.setBrush(QPalette::Active, QPalette::Mid, brush27);
        palette43.setBrush(QPalette::Active, QPalette::Text, brush);
        palette43.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette43.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette43.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette43.setBrush(QPalette::Active, QPalette::Window, brush23);
        palette43.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette43.setBrush(QPalette::Active, QPalette::AlternateBase, brush28);
        palette43.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette43.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette43.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette43.setBrush(QPalette::Inactive, QPalette::Button, brush23);
        palette43.setBrush(QPalette::Inactive, QPalette::Light, brush24);
        palette43.setBrush(QPalette::Inactive, QPalette::Midlight, brush25);
        palette43.setBrush(QPalette::Inactive, QPalette::Dark, brush26);
        palette43.setBrush(QPalette::Inactive, QPalette::Mid, brush27);
        palette43.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette43.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette43.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette43.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette43.setBrush(QPalette::Inactive, QPalette::Window, brush23);
        palette43.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette43.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush28);
        palette43.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette43.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette43.setBrush(QPalette::Disabled, QPalette::WindowText, brush26);
        palette43.setBrush(QPalette::Disabled, QPalette::Button, brush23);
        palette43.setBrush(QPalette::Disabled, QPalette::Light, brush24);
        palette43.setBrush(QPalette::Disabled, QPalette::Midlight, brush25);
        palette43.setBrush(QPalette::Disabled, QPalette::Dark, brush26);
        palette43.setBrush(QPalette::Disabled, QPalette::Mid, brush27);
        palette43.setBrush(QPalette::Disabled, QPalette::Text, brush26);
        palette43.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette43.setBrush(QPalette::Disabled, QPalette::ButtonText, brush26);
        palette43.setBrush(QPalette::Disabled, QPalette::Base, brush23);
        palette43.setBrush(QPalette::Disabled, QPalette::Window, brush23);
        palette43.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette43.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush23);
        palette43.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette43.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        restoreHeaderPushButton->setPalette(palette43);
        restoreHeaderPushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_60->addWidget(restoreHeaderPushButton, 0, 2, 1, 2);


        verticalLayout_8->addWidget(confAstromphotom1Frame);

        confsubtitleAstromphotom2Label = new QLabel(ARCpage);
        confsubtitleAstromphotom2Label->setObjectName(QStringLiteral("confsubtitleAstromphotom2Label"));
        confsubtitleAstromphotom2Label->setFont(font);
        confsubtitleAstromphotom2Label->setWordWrap(true);

        verticalLayout_8->addWidget(confsubtitleAstromphotom2Label);

        confAstromphotom2Frame = new QFrame(ARCpage);
        confAstromphotom2Frame->setObjectName(QStringLiteral("confAstromphotom2Frame"));
        QPalette palette44;
        palette44.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette44.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette44.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette44.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette44.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette44.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette44.setBrush(QPalette::Active, QPalette::Text, brush);
        palette44.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette44.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette44.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette44.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette44.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette44.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette44.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette44.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette44.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette44.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette44.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette44.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette44.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette44.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette44.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette44.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette44.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette44.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette44.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette44.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette44.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette44.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette44.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette44.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette44.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette44.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette44.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette44.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette44.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette44.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette44.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette44.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette44.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette44.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette44.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette44.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette44.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette44.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confAstromphotom2Frame->setPalette(palette44);
        confAstromphotom2Frame->setAutoFillBackground(true);
        confAstromphotom2Frame->setFrameShape(QFrame::NoFrame);
        confAstromphotom2Frame->setFrameShadow(QFrame::Plain);
        gridLayout_27 = new QGridLayout(confAstromphotom2Frame);
        gridLayout_27->setObjectName(QStringLiteral("gridLayout_27"));
        ASTLabel_16 = new QLabel(confAstromphotom2Frame);
        ASTLabel_16->setObjectName(QStringLiteral("ASTLabel_16"));

        gridLayout_27->addWidget(ASTLabel_16, 0, 0, 1, 1);

        ASTmatchMethodComboBox = new QComboBox(confAstromphotom2Frame);
        ASTmatchMethodComboBox->setObjectName(QStringLiteral("ASTmatchMethodComboBox"));
        ASTmatchMethodComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_27->addWidget(ASTmatchMethodComboBox, 0, 1, 1, 2);

        ASTposangleLabel = new QLabel(confAstromphotom2Frame);
        ASTposangleLabel->setObjectName(QStringLiteral("ASTposangleLabel"));

        gridLayout_27->addWidget(ASTposangleLabel, 1, 0, 1, 2);

        ASTposangleLineEdit = new QLineEdit(confAstromphotom2Frame);
        ASTposangleLineEdit->setObjectName(QStringLiteral("ASTposangleLineEdit"));
        ASTposangleLineEdit->setMinimumSize(QSize(80, 0));
        ASTposangleLineEdit->setMaximumSize(QSize(100, 16777215));

        gridLayout_27->addWidget(ASTposangleLineEdit, 1, 2, 1, 1);

        ASTpositionLabel = new QLabel(confAstromphotom2Frame);
        ASTpositionLabel->setObjectName(QStringLiteral("ASTpositionLabel"));

        gridLayout_27->addWidget(ASTpositionLabel, 2, 0, 1, 2);

        ASTpositionLineEdit = new QLineEdit(confAstromphotom2Frame);
        ASTpositionLineEdit->setObjectName(QStringLiteral("ASTpositionLineEdit"));
        ASTpositionLineEdit->setMinimumSize(QSize(80, 0));
        ASTpositionLineEdit->setMaximumSize(QSize(100, 16777215));

        gridLayout_27->addWidget(ASTpositionLineEdit, 2, 2, 1, 1);

        ASTLabel_3 = new QLabel(confAstromphotom2Frame);
        ASTLabel_3->setObjectName(QStringLiteral("ASTLabel_3"));

        gridLayout_27->addWidget(ASTLabel_3, 3, 0, 1, 2);

        ASTpixscaleLineEdit = new QLineEdit(confAstromphotom2Frame);
        ASTpixscaleLineEdit->setObjectName(QStringLiteral("ASTpixscaleLineEdit"));
        ASTpixscaleLineEdit->setMinimumSize(QSize(80, 0));
        ASTpixscaleLineEdit->setMaximumSize(QSize(100, 16777215));

        gridLayout_27->addWidget(ASTpixscaleLineEdit, 3, 2, 1, 1);

        ASTLabel_6 = new QLabel(confAstromphotom2Frame);
        ASTLabel_6->setObjectName(QStringLiteral("ASTLabel_6"));

        gridLayout_27->addWidget(ASTLabel_6, 4, 0, 1, 2);

        ASTcrossidLineEdit = new QLineEdit(confAstromphotom2Frame);
        ASTcrossidLineEdit->setObjectName(QStringLiteral("ASTcrossidLineEdit"));
        ASTcrossidLineEdit->setMinimumSize(QSize(80, 0));
        ASTcrossidLineEdit->setMaximumSize(QSize(100, 16777215));

        gridLayout_27->addWidget(ASTcrossidLineEdit, 4, 2, 1, 1);

        ASTmatchflippedCheckBox = new QCheckBox(confAstromphotom2Frame);
        ASTmatchflippedCheckBox->setObjectName(QStringLiteral("ASTmatchflippedCheckBox"));
        ASTmatchflippedCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_27->addWidget(ASTmatchflippedCheckBox, 5, 0, 1, 3);


        verticalLayout_8->addWidget(confAstromphotom2Frame);

        confsubtitleAstromphotom3Label = new QLabel(ARCpage);
        confsubtitleAstromphotom3Label->setObjectName(QStringLiteral("confsubtitleAstromphotom3Label"));
        confsubtitleAstromphotom3Label->setFont(font);
        confsubtitleAstromphotom3Label->setWordWrap(true);

        verticalLayout_8->addWidget(confsubtitleAstromphotom3Label);

        confAstromphotom3Frame = new QFrame(ARCpage);
        confAstromphotom3Frame->setObjectName(QStringLiteral("confAstromphotom3Frame"));
        QPalette palette45;
        palette45.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette45.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette45.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette45.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette45.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette45.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette45.setBrush(QPalette::Active, QPalette::Text, brush);
        palette45.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette45.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette45.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette45.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette45.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette45.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette45.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette45.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette45.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette45.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette45.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette45.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette45.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette45.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette45.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette45.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette45.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette45.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette45.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette45.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette45.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette45.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette45.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette45.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette45.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette45.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette45.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette45.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette45.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette45.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette45.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette45.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette45.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette45.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette45.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette45.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette45.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette45.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confAstromphotom3Frame->setPalette(palette45);
        confAstromphotom3Frame->setAutoFillBackground(true);
        confAstromphotom3Frame->setFrameShape(QFrame::NoFrame);
        confAstromphotom3Frame->setFrameShadow(QFrame::Raised);
        gridLayout_63 = new QGridLayout(confAstromphotom3Frame);
        gridLayout_63->setObjectName(QStringLiteral("gridLayout_63"));
        ASTastrefweightLineEdit = new QLineEdit(confAstromphotom3Frame);
        ASTastrefweightLineEdit->setObjectName(QStringLiteral("ASTastrefweightLineEdit"));
        ASTastrefweightLineEdit->setMinimumSize(QSize(80, 0));
        ASTastrefweightLineEdit->setMaximumSize(QSize(100, 16777215));

        gridLayout_63->addWidget(ASTastrefweightLineEdit, 0, 2, 1, 1);

        ASTLabel_10 = new QLabel(confAstromphotom3Frame);
        ASTLabel_10->setObjectName(QStringLiteral("ASTLabel_10"));

        gridLayout_63->addWidget(ASTLabel_10, 2, 0, 1, 1);

        ASTLabel_15 = new QLabel(confAstromphotom3Frame);
        ASTLabel_15->setObjectName(QStringLiteral("ASTLabel_15"));

        gridLayout_63->addWidget(ASTLabel_15, 0, 0, 1, 1);

        ASTphotinstrukeyLineEdit = new QLineEdit(confAstromphotom3Frame);
        ASTphotinstrukeyLineEdit->setObjectName(QStringLiteral("ASTphotinstrukeyLineEdit"));
        ASTphotinstrukeyLineEdit->setMinimumSize(QSize(80, 0));

        gridLayout_63->addWidget(ASTphotinstrukeyLineEdit, 3, 1, 1, 2);

        ASTsnthreshLineEdit = new QLineEdit(confAstromphotom3Frame);
        ASTsnthreshLineEdit->setObjectName(QStringLiteral("ASTsnthreshLineEdit"));
        ASTsnthreshLineEdit->setMinimumSize(QSize(80, 0));
        ASTsnthreshLineEdit->setMaximumSize(QSize(100, 16777215));

        gridLayout_63->addWidget(ASTsnthreshLineEdit, 1, 2, 1, 1);

        ASTLabel_7 = new QLabel(confAstromphotom3Frame);
        ASTLabel_7->setObjectName(QStringLiteral("ASTLabel_7"));

        gridLayout_63->addWidget(ASTLabel_7, 1, 0, 1, 1);

        ASTastrinstrukeyLineEdit = new QLineEdit(confAstromphotom3Frame);
        ASTastrinstrukeyLineEdit->setObjectName(QStringLiteral("ASTastrinstrukeyLineEdit"));
        ASTastrinstrukeyLineEdit->setMinimumSize(QSize(80, 0));

        gridLayout_63->addWidget(ASTastrinstrukeyLineEdit, 2, 1, 1, 2);

        ASTLabel_14 = new QLabel(confAstromphotom3Frame);
        ASTLabel_14->setObjectName(QStringLiteral("ASTLabel_14"));

        gridLayout_63->addWidget(ASTLabel_14, 3, 0, 1, 1);


        verticalLayout_8->addWidget(confAstromphotom3Frame);

        verticalSpacer_8 = new QSpacerItem(17, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_8);


        gridLayout_35->addLayout(verticalLayout_8, 0, 0, 1, 1);

        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setSpacing(0);
        verticalLayout_16->setObjectName(QStringLiteral("verticalLayout_16"));
        conftitleAstromphotom2Label = new QLabel(ARCpage);
        conftitleAstromphotom2Label->setObjectName(QStringLiteral("conftitleAstromphotom2Label"));
        sizePolicy1.setHeightForWidth(conftitleAstromphotom2Label->sizePolicy().hasHeightForWidth());
        conftitleAstromphotom2Label->setSizePolicy(sizePolicy1);
        QPalette palette46;
        palette46.setBrush(QPalette::Active, QPalette::WindowText, brush34);
        palette46.setBrush(QPalette::Active, QPalette::Button, brush35);
        palette46.setBrush(QPalette::Active, QPalette::Text, brush34);
        palette46.setBrush(QPalette::Active, QPalette::ButtonText, brush34);
        palette46.setBrush(QPalette::Active, QPalette::Base, brush35);
        palette46.setBrush(QPalette::Active, QPalette::Window, brush35);
        palette46.setBrush(QPalette::Inactive, QPalette::WindowText, brush34);
        palette46.setBrush(QPalette::Inactive, QPalette::Button, brush35);
        palette46.setBrush(QPalette::Inactive, QPalette::Text, brush34);
        palette46.setBrush(QPalette::Inactive, QPalette::ButtonText, brush34);
        palette46.setBrush(QPalette::Inactive, QPalette::Base, brush35);
        palette46.setBrush(QPalette::Inactive, QPalette::Window, brush35);
        palette46.setBrush(QPalette::Disabled, QPalette::WindowText, brush34);
        palette46.setBrush(QPalette::Disabled, QPalette::Button, brush35);
        palette46.setBrush(QPalette::Disabled, QPalette::Text, brush34);
        palette46.setBrush(QPalette::Disabled, QPalette::ButtonText, brush34);
        palette46.setBrush(QPalette::Disabled, QPalette::Base, brush35);
        palette46.setBrush(QPalette::Disabled, QPalette::Window, brush35);
        conftitleAstromphotom2Label->setPalette(palette46);
        conftitleAstromphotom2Label->setFont(font);
        conftitleAstromphotom2Label->setAutoFillBackground(false);
        conftitleAstromphotom2Label->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleAstromphotom2Label->setWordWrap(true);

        verticalLayout_16->addWidget(conftitleAstromphotom2Label);

        confsubtitleAstromphotom4Label = new QLabel(ARCpage);
        confsubtitleAstromphotom4Label->setObjectName(QStringLiteral("confsubtitleAstromphotom4Label"));
        confsubtitleAstromphotom4Label->setFont(font);
        confsubtitleAstromphotom4Label->setWordWrap(true);

        verticalLayout_16->addWidget(confsubtitleAstromphotom4Label);

        confAstromphotom4Frame = new QFrame(ARCpage);
        confAstromphotom4Frame->setObjectName(QStringLiteral("confAstromphotom4Frame"));
        QPalette palette47;
        palette47.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette47.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette47.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette47.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette47.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette47.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette47.setBrush(QPalette::Active, QPalette::Text, brush);
        palette47.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette47.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette47.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette47.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette47.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette47.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette47.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette47.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette47.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette47.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette47.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette47.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette47.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette47.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette47.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette47.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette47.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette47.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette47.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette47.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette47.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette47.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette47.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette47.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette47.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette47.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette47.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette47.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette47.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette47.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette47.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette47.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette47.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette47.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette47.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette47.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette47.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette47.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confAstromphotom4Frame->setPalette(palette47);
        confAstromphotom4Frame->setAutoFillBackground(true);
        confAstromphotom4Frame->setFrameShape(QFrame::NoFrame);
        confAstromphotom4Frame->setFrameShadow(QFrame::Plain);
        gridLayout_55 = new QGridLayout(confAstromphotom4Frame);
        gridLayout_55->setObjectName(QStringLiteral("gridLayout_55"));
        ASTdistortkeysLineEdit = new QLineEdit(confAstromphotom4Frame);
        ASTdistortkeysLineEdit->setObjectName(QStringLiteral("ASTdistortkeysLineEdit"));
        ASTdistortkeysLineEdit->setMinimumSize(QSize(80, 0));
        ASTdistortkeysLineEdit->setMaximumSize(QSize(100, 16777215));

        gridLayout_55->addWidget(ASTdistortkeysLineEdit, 2, 1, 1, 1);

        ASTdistortLineEdit = new QLineEdit(confAstromphotom4Frame);
        ASTdistortLineEdit->setObjectName(QStringLiteral("ASTdistortLineEdit"));
        ASTdistortLineEdit->setMinimumSize(QSize(80, 0));
        ASTdistortLineEdit->setMaximumSize(QSize(100, 16777215));

        gridLayout_55->addWidget(ASTdistortLineEdit, 0, 1, 1, 1);

        ASTLabel_8 = new QLabel(confAstromphotom4Frame);
        ASTLabel_8->setObjectName(QStringLiteral("ASTLabel_8"));

        gridLayout_55->addWidget(ASTLabel_8, 1, 0, 1, 1);

        ASTLabel_9 = new QLabel(confAstromphotom4Frame);
        ASTLabel_9->setObjectName(QStringLiteral("ASTLabel_9"));

        gridLayout_55->addWidget(ASTLabel_9, 2, 0, 1, 1);

        ASTdistortgroupsLineEdit = new QLineEdit(confAstromphotom4Frame);
        ASTdistortgroupsLineEdit->setObjectName(QStringLiteral("ASTdistortgroupsLineEdit"));
        ASTdistortgroupsLineEdit->setMinimumSize(QSize(80, 0));
        ASTdistortgroupsLineEdit->setMaximumSize(QSize(100, 16777215));

        gridLayout_55->addWidget(ASTdistortgroupsLineEdit, 1, 1, 1, 1);

        ASTLabel_4 = new QLabel(confAstromphotom4Frame);
        ASTLabel_4->setObjectName(QStringLiteral("ASTLabel_4"));

        gridLayout_55->addWidget(ASTLabel_4, 0, 0, 1, 1);


        verticalLayout_16->addWidget(confAstromphotom4Frame);

        confsubtitleAstromphotom5Label = new QLabel(ARCpage);
        confsubtitleAstromphotom5Label->setObjectName(QStringLiteral("confsubtitleAstromphotom5Label"));
        confsubtitleAstromphotom5Label->setFont(font);
        confsubtitleAstromphotom5Label->setWordWrap(true);

        verticalLayout_16->addWidget(confsubtitleAstromphotom5Label);

        confAstromphotom5Frame = new QFrame(ARCpage);
        confAstromphotom5Frame->setObjectName(QStringLiteral("confAstromphotom5Frame"));
        QPalette palette48;
        palette48.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette48.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette48.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette48.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette48.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette48.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette48.setBrush(QPalette::Active, QPalette::Text, brush);
        palette48.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette48.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette48.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette48.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette48.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette48.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette48.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette48.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette48.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette48.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette48.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette48.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette48.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette48.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette48.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette48.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette48.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette48.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette48.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette48.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette48.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette48.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette48.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette48.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette48.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette48.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette48.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette48.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette48.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette48.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette48.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette48.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette48.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette48.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette48.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette48.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette48.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette48.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confAstromphotom5Frame->setPalette(palette48);
        confAstromphotom5Frame->setAutoFillBackground(true);
        confAstromphotom5Frame->setFrameShape(QFrame::NoFrame);
        confAstromphotom5Frame->setFrameShadow(QFrame::Plain);
        gridLayout_10 = new QGridLayout(confAstromphotom5Frame);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        ASTLabel_11 = new QLabel(confAstromphotom5Frame);
        ASTLabel_11->setObjectName(QStringLiteral("ASTLabel_11"));

        gridLayout_10->addWidget(ASTLabel_11, 0, 0, 1, 1);

        ASTstabilityComboBox = new QComboBox(confAstromphotom5Frame);
        ASTstabilityComboBox->setObjectName(QStringLiteral("ASTstabilityComboBox"));
        ASTstabilityComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_10->addWidget(ASTstabilityComboBox, 0, 1, 1, 1);

        ASTLabel_12 = new QLabel(confAstromphotom5Frame);
        ASTLabel_12->setObjectName(QStringLiteral("ASTLabel_12"));

        gridLayout_10->addWidget(ASTLabel_12, 1, 0, 1, 1);

        ASTmosaictypeComboBox = new QComboBox(confAstromphotom5Frame);
        ASTmosaictypeComboBox->setObjectName(QStringLiteral("ASTmosaictypeComboBox"));
        ASTmosaictypeComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_10->addWidget(ASTmosaictypeComboBox, 1, 1, 1, 1);

        ASTLabel_13 = new QLabel(confAstromphotom5Frame);
        ASTLabel_13->setObjectName(QStringLiteral("ASTLabel_13"));

        gridLayout_10->addWidget(ASTLabel_13, 2, 0, 1, 1);

        ASTfocalplaneComboBox = new QComboBox(confAstromphotom5Frame);
        ASTfocalplaneComboBox->setObjectName(QStringLiteral("ASTfocalplaneComboBox"));
        ASTfocalplaneComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_10->addWidget(ASTfocalplaneComboBox, 2, 1, 1, 1);

        ASTreadmepushButton = new QPushButton(confAstromphotom5Frame);
        ASTreadmepushButton->setObjectName(QStringLiteral("ASTreadmepushButton"));
        sizePolicy3.setHeightForWidth(ASTreadmepushButton->sizePolicy().hasHeightForWidth());
        ASTreadmepushButton->setSizePolicy(sizePolicy3);
        ASTreadmepushButton->setMinimumSize(QSize(30, 0));
        ASTreadmepushButton->setMaximumSize(QSize(30, 16777215));
        QPalette palette49;
        palette49.setBrush(QPalette::Active, QPalette::Button, brush17);
        palette49.setBrush(QPalette::Inactive, QPalette::Button, brush17);
        palette49.setBrush(QPalette::Disabled, QPalette::Button, brush17);
        ASTreadmepushButton->setPalette(palette49);
        ASTreadmepushButton->setFont(font);
        ASTreadmepushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_10->addWidget(ASTreadmepushButton, 3, 0, 1, 1);


        verticalLayout_16->addWidget(confAstromphotom5Frame);

        confsubtitleAstromphotom6Label = new QLabel(ARCpage);
        confsubtitleAstromphotom6Label->setObjectName(QStringLiteral("confsubtitleAstromphotom6Label"));
        confsubtitleAstromphotom6Label->setFont(font);
        confsubtitleAstromphotom6Label->setWordWrap(true);

        verticalLayout_16->addWidget(confsubtitleAstromphotom6Label);

        confAstromphotom6Frame = new QFrame(ARCpage);
        confAstromphotom6Frame->setObjectName(QStringLiteral("confAstromphotom6Frame"));
        QPalette palette50;
        palette50.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette50.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette50.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette50.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette50.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette50.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette50.setBrush(QPalette::Active, QPalette::Text, brush);
        palette50.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette50.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette50.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette50.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette50.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette50.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette50.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette50.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette50.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette50.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette50.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette50.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette50.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette50.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette50.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette50.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette50.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette50.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette50.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette50.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette50.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette50.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette50.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette50.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette50.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette50.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette50.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette50.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette50.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette50.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette50.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette50.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette50.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette50.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette50.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette50.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette50.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette50.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confAstromphotom6Frame->setPalette(palette50);
        confAstromphotom6Frame->setAutoFillBackground(true);
        confAstromphotom6Frame->setFrameShape(QFrame::NoFrame);
        confAstromphotom6Frame->setFrameShadow(QFrame::Plain);
        gridLayout_64 = new QGridLayout(confAstromphotom6Frame);
        gridLayout_64->setObjectName(QStringLiteral("gridLayout_64"));
        ASTLabel_5 = new QLabel(confAstromphotom6Frame);
        ASTLabel_5->setObjectName(QStringLiteral("ASTLabel_5"));

        gridLayout_64->addWidget(ASTLabel_5, 0, 0, 1, 1);

        ASTresolutionLineEdit = new QLineEdit(confAstromphotom6Frame);
        ASTresolutionLineEdit->setObjectName(QStringLiteral("ASTresolutionLineEdit"));
        ASTresolutionLineEdit->setMinimumSize(QSize(80, 0));
        ASTresolutionLineEdit->setMaximumSize(QSize(100, 16777215));

        gridLayout_64->addWidget(ASTresolutionLineEdit, 0, 1, 1, 1);

        ASTviewCheckPlotsPushButton = new QPushButton(confAstromphotom6Frame);
        ASTviewCheckPlotsPushButton->setObjectName(QStringLiteral("ASTviewCheckPlotsPushButton"));

        gridLayout_64->addWidget(ASTviewCheckPlotsPushButton, 0, 2, 1, 1);


        verticalLayout_16->addWidget(confAstromphotom6Frame);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_16->addItem(verticalSpacer_2);

        conftitleStarflatLabel = new QLabel(ARCpage);
        conftitleStarflatLabel->setObjectName(QStringLiteral("conftitleStarflatLabel"));
        conftitleStarflatLabel->setEnabled(false);
        sizePolicy1.setHeightForWidth(conftitleStarflatLabel->sizePolicy().hasHeightForWidth());
        conftitleStarflatLabel->setSizePolicy(sizePolicy1);
        QPalette palette51;
        palette51.setBrush(QPalette::Active, QPalette::WindowText, brush34);
        palette51.setBrush(QPalette::Active, QPalette::Button, brush35);
        palette51.setBrush(QPalette::Active, QPalette::Text, brush34);
        palette51.setBrush(QPalette::Active, QPalette::ButtonText, brush34);
        palette51.setBrush(QPalette::Active, QPalette::Base, brush35);
        palette51.setBrush(QPalette::Active, QPalette::Window, brush35);
        palette51.setBrush(QPalette::Inactive, QPalette::WindowText, brush34);
        palette51.setBrush(QPalette::Inactive, QPalette::Button, brush35);
        palette51.setBrush(QPalette::Inactive, QPalette::Text, brush34);
        palette51.setBrush(QPalette::Inactive, QPalette::ButtonText, brush34);
        palette51.setBrush(QPalette::Inactive, QPalette::Base, brush35);
        palette51.setBrush(QPalette::Inactive, QPalette::Window, brush35);
        palette51.setBrush(QPalette::Disabled, QPalette::WindowText, brush34);
        palette51.setBrush(QPalette::Disabled, QPalette::Button, brush35);
        palette51.setBrush(QPalette::Disabled, QPalette::Text, brush34);
        palette51.setBrush(QPalette::Disabled, QPalette::ButtonText, brush34);
        palette51.setBrush(QPalette::Disabled, QPalette::Base, brush35);
        palette51.setBrush(QPalette::Disabled, QPalette::Window, brush35);
        conftitleStarflatLabel->setPalette(palette51);
        conftitleStarflatLabel->setFont(font);
        conftitleStarflatLabel->setAutoFillBackground(false);
        conftitleStarflatLabel->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleStarflatLabel->setWordWrap(true);

        verticalLayout_16->addWidget(conftitleStarflatLabel);

        confStarflatFrame = new QFrame(ARCpage);
        confStarflatFrame->setObjectName(QStringLiteral("confStarflatFrame"));
        confStarflatFrame->setEnabled(false);
        QPalette palette52;
        palette52.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette52.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette52.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette52.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette52.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette52.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette52.setBrush(QPalette::Active, QPalette::Text, brush);
        palette52.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette52.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette52.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette52.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette52.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette52.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette52.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette52.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette52.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette52.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette52.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette52.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette52.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette52.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette52.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette52.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette52.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette52.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette52.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette52.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette52.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette52.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette52.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette52.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette52.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette52.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette52.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette52.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette52.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette52.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette52.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette52.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette52.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette52.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette52.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette52.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette52.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette52.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confStarflatFrame->setPalette(palette52);
        confStarflatFrame->setAutoFillBackground(true);
        confStarflatFrame->setFrameShape(QFrame::NoFrame);
        confStarflatFrame->setFrameShadow(QFrame::Plain);
        gridLayout_12 = new QGridLayout(confStarflatFrame);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        confAPDRefcatLabel = new QLabel(confStarflatFrame);
        confAPDRefcatLabel->setObjectName(QStringLiteral("confAPDRefcatLabel"));

        gridLayout_12->addWidget(confAPDRefcatLabel, 0, 0, 1, 1);

        APDrefcatComboBox = new QComboBox(confStarflatFrame);
        APDrefcatComboBox->setObjectName(QStringLiteral("APDrefcatComboBox"));
        APDrefcatComboBox->setMinimumSize(QSize(100, 0));
        APDrefcatComboBox->setMaximumSize(QSize(100, 16777215));
        APDrefcatComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_12->addWidget(APDrefcatComboBox, 0, 1, 1, 1);

        confAPDFilterLabel = new QLabel(confStarflatFrame);
        confAPDFilterLabel->setObjectName(QStringLiteral("confAPDFilterLabel"));
        confAPDFilterLabel->setWordWrap(true);

        gridLayout_12->addWidget(confAPDFilterLabel, 1, 0, 1, 1);

        APDfilterComboBox = new QComboBox(confStarflatFrame);
        APDfilterComboBox->setObjectName(QStringLiteral("APDfilterComboBox"));
        APDfilterComboBox->setMinimumSize(QSize(100, 0));
        APDfilterComboBox->setMaximumSize(QSize(100, 16777215));
        APDfilterComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_12->addWidget(APDfilterComboBox, 1, 1, 1, 1);

        confAPDMaxphoterrorLabel = new QLabel(confStarflatFrame);
        confAPDMaxphoterrorLabel->setObjectName(QStringLiteral("confAPDMaxphoterrorLabel"));

        gridLayout_12->addWidget(confAPDMaxphoterrorLabel, 2, 0, 1, 1);

        APDmaxphoterrorLineEdit = new QLineEdit(confStarflatFrame);
        APDmaxphoterrorLineEdit->setObjectName(QStringLiteral("APDmaxphoterrorLineEdit"));
        APDmaxphoterrorLineEdit->setMinimumSize(QSize(100, 0));
        APDmaxphoterrorLineEdit->setMaximumSize(QSize(100, 16777215));

        gridLayout_12->addWidget(APDmaxphoterrorLineEdit, 2, 1, 1, 1);


        verticalLayout_16->addWidget(confStarflatFrame);

        verticalSpacer = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_16->addItem(verticalSpacer);


        gridLayout_35->addLayout(verticalLayout_16, 0, 1, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(56, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_35->addItem(horizontalSpacer_5, 0, 2, 1, 1);

        confStackedWidget->addWidget(ARCpage);
        SKYpage = new QWidget();
        SKYpage->setObjectName(QStringLiteral("SKYpage"));
        gridLayout_65 = new QGridLayout(SKYpage);
        gridLayout_65->setObjectName(QStringLiteral("gridLayout_65"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        conftitleSkysubLabel = new QLabel(SKYpage);
        conftitleSkysubLabel->setObjectName(QStringLiteral("conftitleSkysubLabel"));
        sizePolicy1.setHeightForWidth(conftitleSkysubLabel->sizePolicy().hasHeightForWidth());
        conftitleSkysubLabel->setSizePolicy(sizePolicy1);
        QPalette palette53;
        palette53.setBrush(QPalette::Active, QPalette::WindowText, brush34);
        palette53.setBrush(QPalette::Active, QPalette::Button, brush35);
        palette53.setBrush(QPalette::Active, QPalette::Text, brush34);
        palette53.setBrush(QPalette::Active, QPalette::ButtonText, brush34);
        palette53.setBrush(QPalette::Active, QPalette::Base, brush35);
        palette53.setBrush(QPalette::Active, QPalette::Window, brush35);
        palette53.setBrush(QPalette::Inactive, QPalette::WindowText, brush34);
        palette53.setBrush(QPalette::Inactive, QPalette::Button, brush35);
        palette53.setBrush(QPalette::Inactive, QPalette::Text, brush34);
        palette53.setBrush(QPalette::Inactive, QPalette::ButtonText, brush34);
        palette53.setBrush(QPalette::Inactive, QPalette::Base, brush35);
        palette53.setBrush(QPalette::Inactive, QPalette::Window, brush35);
        palette53.setBrush(QPalette::Disabled, QPalette::WindowText, brush34);
        palette53.setBrush(QPalette::Disabled, QPalette::Button, brush35);
        palette53.setBrush(QPalette::Disabled, QPalette::Text, brush34);
        palette53.setBrush(QPalette::Disabled, QPalette::ButtonText, brush34);
        palette53.setBrush(QPalette::Disabled, QPalette::Base, brush35);
        palette53.setBrush(QPalette::Disabled, QPalette::Window, brush35);
        conftitleSkysubLabel->setPalette(palette53);
        conftitleSkysubLabel->setFont(font);
        conftitleSkysubLabel->setAutoFillBackground(false);
        conftitleSkysubLabel->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleSkysubLabel->setWordWrap(true);

        verticalLayout_7->addWidget(conftitleSkysubLabel);

        confsubtitleSkysub1Label = new QLabel(SKYpage);
        confsubtitleSkysub1Label->setObjectName(QStringLiteral("confsubtitleSkysub1Label"));
        confsubtitleSkysub1Label->setFont(font);
        confsubtitleSkysub1Label->setAutoFillBackground(true);
        confsubtitleSkysub1Label->setWordWrap(true);

        verticalLayout_7->addWidget(confsubtitleSkysub1Label);

        confSkysub1Frame = new QFrame(SKYpage);
        confSkysub1Frame->setObjectName(QStringLiteral("confSkysub1Frame"));
        QPalette palette54;
        palette54.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette54.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette54.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette54.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette54.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette54.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette54.setBrush(QPalette::Active, QPalette::Text, brush);
        palette54.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette54.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette54.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette54.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette54.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette54.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette54.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette54.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette54.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette54.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette54.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette54.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette54.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette54.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette54.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette54.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette54.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette54.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette54.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette54.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette54.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette54.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette54.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette54.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette54.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette54.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette54.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette54.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette54.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette54.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette54.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette54.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette54.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette54.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette54.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette54.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette54.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette54.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confSkysub1Frame->setPalette(palette54);
        confSkysub1Frame->setAutoFillBackground(true);
        confSkysub1Frame->setFrameShape(QFrame::NoFrame);
        confSkysub1Frame->setFrameShadow(QFrame::Plain);
        gridLayout_85 = new QGridLayout(confSkysub1Frame);
        gridLayout_85->setObjectName(QStringLiteral("gridLayout_85"));
        skyModelRadioButton = new QRadioButton(confSkysub1Frame);
        skyModelRadioButton->setObjectName(QStringLiteral("skyModelRadioButton"));
        skyModelRadioButton->setFocusPolicy(Qt::ClickFocus);
        skyModelRadioButton->setChecked(true);

        gridLayout_85->addWidget(skyModelRadioButton, 0, 0, 1, 1);

        skyReadmePushButton = new QPushButton(confSkysub1Frame);
        skyReadmePushButton->setObjectName(QStringLiteral("skyReadmePushButton"));
        sizePolicy3.setHeightForWidth(skyReadmePushButton->sizePolicy().hasHeightForWidth());
        skyReadmePushButton->setSizePolicy(sizePolicy3);
        skyReadmePushButton->setMinimumSize(QSize(30, 0));
        skyReadmePushButton->setMaximumSize(QSize(30, 16777215));
        QPalette palette55;
        palette55.setBrush(QPalette::Active, QPalette::Button, brush17);
        palette55.setBrush(QPalette::Inactive, QPalette::Button, brush17);
        palette55.setBrush(QPalette::Disabled, QPalette::Button, brush17);
        skyReadmePushButton->setPalette(palette55);
        skyReadmePushButton->setFont(font);
        skyReadmePushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_85->addWidget(skyReadmePushButton, 0, 1, 1, 1);

        skyPolynomialRadioButton = new QRadioButton(confSkysub1Frame);
        skyPolynomialRadioButton->setObjectName(QStringLiteral("skyPolynomialRadioButton"));
        skyPolynomialRadioButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_85->addWidget(skyPolynomialRadioButton, 1, 0, 1, 1);

        line = new QFrame(confSkysub1Frame);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_85->addWidget(line, 3, 0, 1, 2);

        skyConstsubRadioButton = new QRadioButton(confSkysub1Frame);
        skyConstsubRadioButton->setObjectName(QStringLiteral("skyConstsubRadioButton"));
        skyConstsubRadioButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_85->addWidget(skyConstsubRadioButton, 2, 0, 1, 1);

        skySavemodelCheckBox = new QCheckBox(confSkysub1Frame);
        skySavemodelCheckBox->setObjectName(QStringLiteral("skySavemodelCheckBox"));
        skySavemodelCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_85->addWidget(skySavemodelCheckBox, 4, 0, 1, 2);

        skyPolynomialSpinBox = new QSpinBox(confSkysub1Frame);
        skyPolynomialSpinBox->setObjectName(QStringLiteral("skyPolynomialSpinBox"));
        sizePolicy6.setHeightForWidth(skyPolynomialSpinBox->sizePolicy().hasHeightForWidth());
        skyPolynomialSpinBox->setSizePolicy(sizePolicy6);
        skyPolynomialSpinBox->setMinimumSize(QSize(60, 0));
        skyPolynomialSpinBox->setMaximumSize(QSize(60, 16777215));
        skyPolynomialSpinBox->setFocusPolicy(Qt::ClickFocus);
        skyPolynomialSpinBox->setMinimum(2);
        skyPolynomialSpinBox->setMaximum(4);

        gridLayout_85->addWidget(skyPolynomialSpinBox, 1, 1, 1, 1);


        verticalLayout_7->addWidget(confSkysub1Frame);

        confsubtitleSkysub2Label = new QLabel(SKYpage);
        confsubtitleSkysub2Label->setObjectName(QStringLiteral("confsubtitleSkysub2Label"));
        confsubtitleSkysub2Label->setFont(font);
        confsubtitleSkysub2Label->setAutoFillBackground(true);
        confsubtitleSkysub2Label->setWordWrap(true);

        verticalLayout_7->addWidget(confsubtitleSkysub2Label);

        confSkysub2Frame = new QFrame(SKYpage);
        confSkysub2Frame->setObjectName(QStringLiteral("confSkysub2Frame"));
        QPalette palette56;
        palette56.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette56.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette56.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette56.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette56.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette56.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette56.setBrush(QPalette::Active, QPalette::Text, brush);
        palette56.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette56.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette56.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette56.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette56.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette56.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette56.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette56.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette56.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette56.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette56.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette56.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette56.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette56.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette56.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette56.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette56.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette56.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette56.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette56.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette56.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette56.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette56.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette56.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette56.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette56.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette56.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette56.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette56.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette56.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette56.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette56.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette56.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette56.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette56.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette56.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette56.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette56.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confSkysub2Frame->setPalette(palette56);
        confSkysub2Frame->setAutoFillBackground(true);
        confSkysub2Frame->setFrameShape(QFrame::NoFrame);
        confSkysub2Frame->setFrameShadow(QFrame::Plain);
        gridLayout_75 = new QGridLayout(confSkysub2Frame);
        gridLayout_75->setObjectName(QStringLiteral("gridLayout_75"));
        confCollapseDMINLabel_2 = new QLabel(confSkysub2Frame);
        confCollapseDMINLabel_2->setObjectName(QStringLiteral("confCollapseDMINLabel_2"));
        sizePolicy1.setHeightForWidth(confCollapseDMINLabel_2->sizePolicy().hasHeightForWidth());
        confCollapseDMINLabel_2->setSizePolicy(sizePolicy1);

        gridLayout_75->addWidget(confCollapseDMINLabel_2, 0, 2, 1, 1);

        confCollapseDTLabel_2 = new QLabel(confSkysub2Frame);
        confCollapseDTLabel_2->setObjectName(QStringLiteral("confCollapseDTLabel_2"));
        sizePolicy1.setHeightForWidth(confCollapseDTLabel_2->sizePolicy().hasHeightForWidth());
        confCollapseDTLabel_2->setSizePolicy(sizePolicy1);

        gridLayout_75->addWidget(confCollapseDTLabel_2, 0, 0, 1, 1);

        skyDTLineEdit = new QLineEdit(confSkysub2Frame);
        skyDTLineEdit->setObjectName(QStringLiteral("skyDTLineEdit"));
        sizePolicy6.setHeightForWidth(skyDTLineEdit->sizePolicy().hasHeightForWidth());
        skyDTLineEdit->setSizePolicy(sizePolicy6);
        skyDTLineEdit->setMinimumSize(QSize(80, 0));
        skyDTLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_75->addWidget(skyDTLineEdit, 0, 1, 1, 1);

        skyMefLineEdit = new QLineEdit(confSkysub2Frame);
        skyMefLineEdit->setObjectName(QStringLiteral("skyMefLineEdit"));
        sizePolicy6.setHeightForWidth(skyMefLineEdit->sizePolicy().hasHeightForWidth());
        skyMefLineEdit->setSizePolicy(sizePolicy6);
        skyMefLineEdit->setMinimumSize(QSize(80, 0));
        skyMefLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_75->addWidget(skyMefLineEdit, 2, 3, 1, 1);

        skyKernelLineEdit = new QLineEdit(confSkysub2Frame);
        skyKernelLineEdit->setObjectName(QStringLiteral("skyKernelLineEdit"));
        sizePolicy6.setHeightForWidth(skyKernelLineEdit->sizePolicy().hasHeightForWidth());
        skyKernelLineEdit->setSizePolicy(sizePolicy6);
        skyKernelLineEdit->setMinimumSize(QSize(80, 0));
        skyKernelLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_75->addWidget(skyKernelLineEdit, 1, 3, 1, 1);

        confCollapseDMINLabel_3 = new QLabel(confSkysub2Frame);
        confCollapseDMINLabel_3->setObjectName(QStringLiteral("confCollapseDMINLabel_3"));
        sizePolicy1.setHeightForWidth(confCollapseDMINLabel_3->sizePolicy().hasHeightForWidth());
        confCollapseDMINLabel_3->setSizePolicy(sizePolicy1);
        confCollapseDMINLabel_3->setWordWrap(false);

        gridLayout_75->addWidget(confCollapseDMINLabel_3, 1, 0, 1, 3);

        confCollapseMEFLabel_2 = new QLabel(confSkysub2Frame);
        confCollapseMEFLabel_2->setObjectName(QStringLiteral("confCollapseMEFLabel_2"));
        confCollapseMEFLabel_2->setLayoutDirection(Qt::LeftToRight);
        confCollapseMEFLabel_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_75->addWidget(confCollapseMEFLabel_2, 2, 0, 1, 3);

        skyDMINLineEdit = new QLineEdit(confSkysub2Frame);
        skyDMINLineEdit->setObjectName(QStringLiteral("skyDMINLineEdit"));
        sizePolicy6.setHeightForWidth(skyDMINLineEdit->sizePolicy().hasHeightForWidth());
        skyDMINLineEdit->setSizePolicy(sizePolicy6);
        skyDMINLineEdit->setMinimumSize(QSize(80, 0));
        skyDMINLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_75->addWidget(skyDMINLineEdit, 0, 3, 1, 1);


        verticalLayout_7->addWidget(confSkysub2Frame);

        confsubtitleSkysub3Label = new QLabel(SKYpage);
        confsubtitleSkysub3Label->setObjectName(QStringLiteral("confsubtitleSkysub3Label"));
        confsubtitleSkysub3Label->setFont(font);
        confsubtitleSkysub3Label->setAutoFillBackground(true);
        confsubtitleSkysub3Label->setWordWrap(true);

        verticalLayout_7->addWidget(confsubtitleSkysub3Label);

        confSkysub3Frame = new QFrame(SKYpage);
        confSkysub3Frame->setObjectName(QStringLiteral("confSkysub3Frame"));
        QPalette palette57;
        palette57.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette57.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette57.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette57.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette57.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette57.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette57.setBrush(QPalette::Active, QPalette::Text, brush);
        palette57.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette57.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette57.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette57.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette57.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette57.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette57.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette57.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette57.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette57.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette57.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette57.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette57.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette57.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette57.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette57.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette57.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette57.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette57.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette57.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette57.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette57.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette57.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette57.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette57.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette57.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette57.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette57.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette57.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette57.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette57.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette57.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette57.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette57.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette57.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette57.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette57.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette57.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confSkysub3Frame->setPalette(palette57);
        confSkysub3Frame->setAutoFillBackground(true);
        confSkysub3Frame->setFrameShape(QFrame::NoFrame);
        confSkysub3Frame->setFrameShadow(QFrame::Plain);
        gridLayout_25 = new QGridLayout(confSkysub3Frame);
        gridLayout_25->setObjectName(QStringLiteral("gridLayout_25"));
        label_17 = new QLabel(confSkysub3Frame);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout_25->addWidget(label_17, 0, 0, 1, 1);

        skyAreaComboBox = new QComboBox(confSkysub3Frame);
        skyAreaComboBox->setObjectName(QStringLiteral("skyAreaComboBox"));
        sizePolicy4.setHeightForWidth(skyAreaComboBox->sizePolicy().hasHeightForWidth());
        skyAreaComboBox->setSizePolicy(sizePolicy4);
        skyAreaComboBox->setFocusPolicy(Qt::ClickFocus);
        skyAreaComboBox->setMaxVisibleItems(18);

        gridLayout_25->addWidget(skyAreaComboBox, 0, 1, 1, 1);

        skyAreaPushButton = new QPushButton(confSkysub3Frame);
        skyAreaPushButton->setObjectName(QStringLiteral("skyAreaPushButton"));
        QPalette palette58;
        palette58.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette58.setBrush(QPalette::Active, QPalette::Button, brush23);
        palette58.setBrush(QPalette::Active, QPalette::Light, brush24);
        palette58.setBrush(QPalette::Active, QPalette::Midlight, brush25);
        palette58.setBrush(QPalette::Active, QPalette::Dark, brush26);
        palette58.setBrush(QPalette::Active, QPalette::Mid, brush27);
        palette58.setBrush(QPalette::Active, QPalette::Text, brush);
        palette58.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette58.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette58.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette58.setBrush(QPalette::Active, QPalette::Window, brush23);
        palette58.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette58.setBrush(QPalette::Active, QPalette::AlternateBase, brush28);
        palette58.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette58.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette58.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette58.setBrush(QPalette::Inactive, QPalette::Button, brush23);
        palette58.setBrush(QPalette::Inactive, QPalette::Light, brush24);
        palette58.setBrush(QPalette::Inactive, QPalette::Midlight, brush25);
        palette58.setBrush(QPalette::Inactive, QPalette::Dark, brush26);
        palette58.setBrush(QPalette::Inactive, QPalette::Mid, brush27);
        palette58.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette58.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette58.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette58.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette58.setBrush(QPalette::Inactive, QPalette::Window, brush23);
        palette58.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette58.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush28);
        palette58.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette58.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette58.setBrush(QPalette::Disabled, QPalette::WindowText, brush26);
        palette58.setBrush(QPalette::Disabled, QPalette::Button, brush23);
        palette58.setBrush(QPalette::Disabled, QPalette::Light, brush24);
        palette58.setBrush(QPalette::Disabled, QPalette::Midlight, brush25);
        palette58.setBrush(QPalette::Disabled, QPalette::Dark, brush26);
        palette58.setBrush(QPalette::Disabled, QPalette::Mid, brush27);
        palette58.setBrush(QPalette::Disabled, QPalette::Text, brush26);
        palette58.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette58.setBrush(QPalette::Disabled, QPalette::ButtonText, brush26);
        palette58.setBrush(QPalette::Disabled, QPalette::Base, brush23);
        palette58.setBrush(QPalette::Disabled, QPalette::Window, brush23);
        palette58.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette58.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush23);
        palette58.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette58.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        skyAreaPushButton->setPalette(palette58);
        skyAreaPushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_25->addWidget(skyAreaPushButton, 1, 1, 1, 1);


        verticalLayout_7->addWidget(confSkysub3Frame);

        verticalSpacer_6 = new QSpacerItem(20, 90, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_6);


        gridLayout_65->addLayout(verticalLayout_7, 0, 0, 1, 1);

        horizontalSpacer_10 = new QSpacerItem(333, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_65->addItem(horizontalSpacer_10, 0, 1, 1, 1);

        confStackedWidget->addWidget(SKYpage);
        COCpage_2 = new QWidget();
        COCpage_2->setObjectName(QStringLiteral("COCpage_2"));
        gridLayout_67 = new QGridLayout(COCpage_2);
        gridLayout_67->setObjectName(QStringLiteral("gridLayout_67"));
        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(0);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        conftitleCoadd1Label = new QLabel(COCpage_2);
        conftitleCoadd1Label->setObjectName(QStringLiteral("conftitleCoadd1Label"));
        sizePolicy1.setHeightForWidth(conftitleCoadd1Label->sizePolicy().hasHeightForWidth());
        conftitleCoadd1Label->setSizePolicy(sizePolicy1);
        QPalette palette59;
        palette59.setBrush(QPalette::Active, QPalette::WindowText, brush34);
        palette59.setBrush(QPalette::Active, QPalette::Button, brush35);
        palette59.setBrush(QPalette::Active, QPalette::Text, brush34);
        palette59.setBrush(QPalette::Active, QPalette::ButtonText, brush34);
        palette59.setBrush(QPalette::Active, QPalette::Base, brush35);
        palette59.setBrush(QPalette::Active, QPalette::Window, brush35);
        palette59.setBrush(QPalette::Inactive, QPalette::WindowText, brush34);
        palette59.setBrush(QPalette::Inactive, QPalette::Button, brush35);
        palette59.setBrush(QPalette::Inactive, QPalette::Text, brush34);
        palette59.setBrush(QPalette::Inactive, QPalette::ButtonText, brush34);
        palette59.setBrush(QPalette::Inactive, QPalette::Base, brush35);
        palette59.setBrush(QPalette::Inactive, QPalette::Window, brush35);
        palette59.setBrush(QPalette::Disabled, QPalette::WindowText, brush34);
        palette59.setBrush(QPalette::Disabled, QPalette::Button, brush35);
        palette59.setBrush(QPalette::Disabled, QPalette::Text, brush34);
        palette59.setBrush(QPalette::Disabled, QPalette::ButtonText, brush34);
        palette59.setBrush(QPalette::Disabled, QPalette::Base, brush35);
        palette59.setBrush(QPalette::Disabled, QPalette::Window, brush35);
        conftitleCoadd1Label->setPalette(palette59);
        conftitleCoadd1Label->setFont(font);
        conftitleCoadd1Label->setAutoFillBackground(false);
        conftitleCoadd1Label->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleCoadd1Label->setWordWrap(true);

        verticalLayout_11->addWidget(conftitleCoadd1Label);

        confsubtitleCoadd1Label = new QLabel(COCpage_2);
        confsubtitleCoadd1Label->setObjectName(QStringLiteral("confsubtitleCoadd1Label"));
        confsubtitleCoadd1Label->setFont(font);
        confsubtitleCoadd1Label->setAutoFillBackground(true);

        verticalLayout_11->addWidget(confsubtitleCoadd1Label);

        confCoadd1Frame = new QFrame(COCpage_2);
        confCoadd1Frame->setObjectName(QStringLiteral("confCoadd1Frame"));
        QPalette palette60;
        palette60.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette60.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette60.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette60.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette60.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette60.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette60.setBrush(QPalette::Active, QPalette::Text, brush);
        palette60.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette60.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette60.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette60.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette60.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette60.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette60.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette60.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette60.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette60.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette60.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette60.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette60.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette60.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette60.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette60.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette60.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette60.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette60.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette60.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette60.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette60.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette60.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette60.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette60.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette60.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette60.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette60.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette60.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette60.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette60.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette60.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette60.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette60.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette60.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette60.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette60.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette60.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confCoadd1Frame->setPalette(palette60);
        confCoadd1Frame->setAutoFillBackground(true);
        confCoadd1Frame->setFrameShape(QFrame::NoFrame);
        confCoadd1Frame->setFrameShadow(QFrame::Plain);
        gridLayout_52 = new QGridLayout(confCoadd1Frame);
        gridLayout_52->setObjectName(QStringLiteral("gridLayout_52"));
        COAtype1Label = new QLabel(confCoadd1Frame);
        COAtype1Label->setObjectName(QStringLiteral("COAtype1Label"));

        gridLayout_52->addWidget(COAtype1Label, 0, 0, 1, 1);

        COAraLineEdit = new QLineEdit(confCoadd1Frame);
        COAraLineEdit->setObjectName(QStringLiteral("COAraLineEdit"));
        COAraLineEdit->setMinimumSize(QSize(200, 0));
        COAraLineEdit->setMaximumSize(QSize(200, 16777215));

        gridLayout_52->addWidget(COAraLineEdit, 0, 1, 1, 1);

        COAtype2Label = new QLabel(confCoadd1Frame);
        COAtype2Label->setObjectName(QStringLiteral("COAtype2Label"));

        gridLayout_52->addWidget(COAtype2Label, 1, 0, 1, 1);

        COAdecLineEdit = new QLineEdit(confCoadd1Frame);
        COAdecLineEdit->setObjectName(QStringLiteral("COAdecLineEdit"));
        COAdecLineEdit->setMinimumSize(QSize(200, 0));
        COAdecLineEdit->setMaximumSize(QSize(200, 16777215));

        gridLayout_52->addWidget(COAdecLineEdit, 1, 1, 1, 1);


        verticalLayout_11->addWidget(confCoadd1Frame);

        confsubtitleCoadd2Label = new QLabel(COCpage_2);
        confsubtitleCoadd2Label->setObjectName(QStringLiteral("confsubtitleCoadd2Label"));
        confsubtitleCoadd2Label->setFont(font);
        confsubtitleCoadd2Label->setAutoFillBackground(true);

        verticalLayout_11->addWidget(confsubtitleCoadd2Label);

        confCoadd2Frame = new QFrame(COCpage_2);
        confCoadd2Frame->setObjectName(QStringLiteral("confCoadd2Frame"));
        QPalette palette61;
        palette61.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette61.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette61.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette61.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette61.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette61.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette61.setBrush(QPalette::Active, QPalette::Text, brush);
        palette61.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette61.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette61.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette61.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette61.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette61.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette61.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette61.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette61.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette61.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette61.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette61.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette61.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette61.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette61.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette61.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette61.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette61.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette61.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette61.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette61.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette61.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette61.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette61.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette61.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette61.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette61.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette61.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette61.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette61.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette61.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette61.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette61.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette61.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette61.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette61.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette61.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette61.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confCoadd2Frame->setPalette(palette61);
        confCoadd2Frame->setAutoFillBackground(true);
        confCoadd2Frame->setFrameShape(QFrame::NoFrame);
        confCoadd2Frame->setFrameShadow(QFrame::Plain);
        gridLayout_38 = new QGridLayout(confCoadd2Frame);
        gridLayout_38->setObjectName(QStringLiteral("gridLayout_38"));
        COApixscaleLineEdit = new QLineEdit(confCoadd2Frame);
        COApixscaleLineEdit->setObjectName(QStringLiteral("COApixscaleLineEdit"));
        COApixscaleLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_38->addWidget(COApixscaleLineEdit, 0, 4, 1, 1);

        COAskypaLineEdit = new QLineEdit(confCoadd2Frame);
        COAskypaLineEdit->setObjectName(QStringLiteral("COAskypaLineEdit"));
        COAskypaLineEdit->setMinimumSize(QSize(80, 0));
        COAskypaLineEdit->setMaximumSize(QSize(80, 16777215));

        gridLayout_38->addWidget(COAskypaLineEdit, 1, 4, 1, 1);

        COCkernelLabel = new QLabel(confCoadd2Frame);
        COCkernelLabel->setObjectName(QStringLiteral("COCkernelLabel"));

        gridLayout_38->addWidget(COCkernelLabel, 2, 0, 1, 3);

        COAcombinetypeComboBox = new QComboBox(confCoadd2Frame);
        COAcombinetypeComboBox->setObjectName(QStringLiteral("COAcombinetypeComboBox"));
        COAcombinetypeComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_38->addWidget(COAcombinetypeComboBox, 2, 3, 1, 2);

        COCkernelLabel_2 = new QLabel(confCoadd2Frame);
        COCkernelLabel_2->setObjectName(QStringLiteral("COCkernelLabel_2"));

        gridLayout_38->addWidget(COCkernelLabel_2, 3, 0, 1, 3);

        COAkernelComboBox = new QComboBox(confCoadd2Frame);
        COAkernelComboBox->setObjectName(QStringLiteral("COAkernelComboBox"));
        COAkernelComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_38->addWidget(COAkernelComboBox, 3, 3, 1, 2);

        COAprojectionComboBox = new QComboBox(confCoadd2Frame);
        COAprojectionComboBox->setObjectName(QStringLiteral("COAprojectionComboBox"));
        COAprojectionComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_38->addWidget(COAprojectionComboBox, 4, 3, 1, 1);

        COAcelestialtypeComboBox = new QComboBox(confCoadd2Frame);
        COAcelestialtypeComboBox->setObjectName(QStringLiteral("COAcelestialtypeComboBox"));
        COAcelestialtypeComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_38->addWidget(COAcelestialtypeComboBox, 5, 3, 1, 2);

        COArescaleweightsCheckBox = new QCheckBox(confCoadd2Frame);
        COArescaleweightsCheckBox->setObjectName(QStringLiteral("COArescaleweightsCheckBox"));
        COArescaleweightsCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_38->addWidget(COArescaleweightsCheckBox, 6, 0, 1, 3);

        label_53 = new QLabel(confCoadd2Frame);
        label_53->setObjectName(QStringLiteral("label_53"));

        gridLayout_38->addWidget(label_53, 0, 0, 1, 4);

        COCkernelLabel_3 = new QLabel(confCoadd2Frame);
        COCkernelLabel_3->setObjectName(QStringLiteral("COCkernelLabel_3"));

        gridLayout_38->addWidget(COCkernelLabel_3, 4, 0, 1, 3);

        COCkernelLabel_4 = new QLabel(confCoadd2Frame);
        COCkernelLabel_4->setObjectName(QStringLiteral("COCkernelLabel_4"));

        gridLayout_38->addWidget(COCkernelLabel_4, 5, 0, 1, 3);

        COCkernelLabel_5 = new QLabel(confCoadd2Frame);
        COCkernelLabel_5->setObjectName(QStringLiteral("COCkernelLabel_5"));

        gridLayout_38->addWidget(COCkernelLabel_5, 1, 0, 1, 1);

        COAskypaPushButton = new QPushButton(confCoadd2Frame);
        COAskypaPushButton->setObjectName(QStringLiteral("COAskypaPushButton"));
        COAskypaPushButton->setFocusPolicy(Qt::ClickFocus);

        gridLayout_38->addWidget(COAskypaPushButton, 1, 2, 1, 2);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Ignored, QSizePolicy::Minimum);

        gridLayout_38->addItem(horizontalSpacer_4, 1, 1, 1, 1);


        verticalLayout_11->addWidget(confCoadd2Frame);

        confsubtitleCoadd3Label = new QLabel(COCpage_2);
        confsubtitleCoadd3Label->setObjectName(QStringLiteral("confsubtitleCoadd3Label"));
        confsubtitleCoadd3Label->setFont(font);
        confsubtitleCoadd3Label->setAutoFillBackground(true);

        verticalLayout_11->addWidget(confsubtitleCoadd3Label);

        confCoadd3Frame = new QFrame(COCpage_2);
        confCoadd3Frame->setObjectName(QStringLiteral("confCoadd3Frame"));
        QPalette palette62;
        palette62.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette62.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette62.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette62.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette62.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette62.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette62.setBrush(QPalette::Active, QPalette::Text, brush);
        palette62.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette62.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette62.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette62.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette62.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette62.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette62.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette62.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette62.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette62.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette62.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette62.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette62.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette62.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette62.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette62.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette62.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette62.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette62.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette62.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette62.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette62.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette62.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette62.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette62.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette62.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette62.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette62.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette62.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette62.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette62.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette62.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette62.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette62.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette62.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette62.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette62.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette62.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confCoadd3Frame->setPalette(palette62);
        confCoadd3Frame->setAutoFillBackground(true);
        confCoadd3Frame->setFrameShape(QFrame::NoFrame);
        confCoadd3Frame->setFrameShadow(QFrame::Plain);
        gridLayout_83 = new QGridLayout(confCoadd3Frame);
        gridLayout_83->setObjectName(QStringLiteral("gridLayout_83"));
        formLayout_16 = new QFormLayout();
        formLayout_16->setObjectName(QStringLiteral("formLayout_16"));
        label_56 = new QLabel(confCoadd3Frame);
        label_56->setObjectName(QStringLiteral("label_56"));

        formLayout_16->setWidget(0, QFormLayout::LabelRole, label_56);

        COAoutthreshLineEdit = new QLineEdit(confCoadd3Frame);
        COAoutthreshLineEdit->setObjectName(QStringLiteral("COAoutthreshLineEdit"));
        COAoutthreshLineEdit->setMinimumSize(QSize(80, 0));
        COAoutthreshLineEdit->setMaximumSize(QSize(80, 16777215));

        formLayout_16->setWidget(0, QFormLayout::FieldRole, COAoutthreshLineEdit);

        label_58 = new QLabel(confCoadd3Frame);
        label_58->setObjectName(QStringLiteral("label_58"));

        formLayout_16->setWidget(1, QFormLayout::LabelRole, label_58);

        COAoutsizeLineEdit = new QLineEdit(confCoadd3Frame);
        COAoutsizeLineEdit->setObjectName(QStringLiteral("COAoutsizeLineEdit"));
        COAoutsizeLineEdit->setMinimumSize(QSize(80, 0));
        COAoutsizeLineEdit->setMaximumSize(QSize(80, 16777215));

        formLayout_16->setWidget(1, QFormLayout::FieldRole, COAoutsizeLineEdit);

        label_57 = new QLabel(confCoadd3Frame);
        label_57->setObjectName(QStringLiteral("label_57"));

        formLayout_16->setWidget(2, QFormLayout::LabelRole, label_57);

        COAoutborderLineEdit = new QLineEdit(confCoadd3Frame);
        COAoutborderLineEdit->setObjectName(QStringLiteral("COAoutborderLineEdit"));
        COAoutborderLineEdit->setMinimumSize(QSize(80, 0));
        COAoutborderLineEdit->setMaximumSize(QSize(80, 16777215));

        formLayout_16->setWidget(2, QFormLayout::FieldRole, COAoutborderLineEdit);


        gridLayout_83->addLayout(formLayout_16, 0, 0, 1, 1);


        verticalLayout_11->addWidget(confCoadd3Frame);

        verticalSpacer_54 = new QSpacerItem(20, 48, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_11->addItem(verticalSpacer_54);


        gridLayout_67->addLayout(verticalLayout_11, 0, 0, 1, 1);

        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setSpacing(0);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        conftitleCoadd2Label = new QLabel(COCpage_2);
        conftitleCoadd2Label->setObjectName(QStringLiteral("conftitleCoadd2Label"));
        sizePolicy1.setHeightForWidth(conftitleCoadd2Label->sizePolicy().hasHeightForWidth());
        conftitleCoadd2Label->setSizePolicy(sizePolicy1);
        QPalette palette63;
        palette63.setBrush(QPalette::Active, QPalette::WindowText, brush34);
        palette63.setBrush(QPalette::Active, QPalette::Button, brush35);
        palette63.setBrush(QPalette::Active, QPalette::Text, brush34);
        palette63.setBrush(QPalette::Active, QPalette::ButtonText, brush34);
        palette63.setBrush(QPalette::Active, QPalette::Base, brush35);
        palette63.setBrush(QPalette::Active, QPalette::Window, brush35);
        palette63.setBrush(QPalette::Inactive, QPalette::WindowText, brush34);
        palette63.setBrush(QPalette::Inactive, QPalette::Button, brush35);
        palette63.setBrush(QPalette::Inactive, QPalette::Text, brush34);
        palette63.setBrush(QPalette::Inactive, QPalette::ButtonText, brush34);
        palette63.setBrush(QPalette::Inactive, QPalette::Base, brush35);
        palette63.setBrush(QPalette::Inactive, QPalette::Window, brush35);
        palette63.setBrush(QPalette::Disabled, QPalette::WindowText, brush34);
        palette63.setBrush(QPalette::Disabled, QPalette::Button, brush35);
        palette63.setBrush(QPalette::Disabled, QPalette::Text, brush34);
        palette63.setBrush(QPalette::Disabled, QPalette::ButtonText, brush34);
        palette63.setBrush(QPalette::Disabled, QPalette::Base, brush35);
        palette63.setBrush(QPalette::Disabled, QPalette::Window, brush35);
        conftitleCoadd2Label->setPalette(palette63);
        conftitleCoadd2Label->setFont(font);
        conftitleCoadd2Label->setAutoFillBackground(false);
        conftitleCoadd2Label->setStyleSheet(QLatin1String("color: rgb(100, 230, 230);background-color: rgb(58, 78, 98);\n"
""));
        conftitleCoadd2Label->setWordWrap(true);

        verticalLayout_12->addWidget(conftitleCoadd2Label);

        confsubtitleCoadd4Label = new QLabel(COCpage_2);
        confsubtitleCoadd4Label->setObjectName(QStringLiteral("confsubtitleCoadd4Label"));
        confsubtitleCoadd4Label->setFont(font);
        confsubtitleCoadd4Label->setAutoFillBackground(true);

        verticalLayout_12->addWidget(confsubtitleCoadd4Label);

        confCoadd4Frame = new QFrame(COCpage_2);
        confCoadd4Frame->setObjectName(QStringLiteral("confCoadd4Frame"));
        QPalette palette64;
        palette64.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette64.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette64.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette64.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette64.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette64.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette64.setBrush(QPalette::Active, QPalette::Text, brush);
        palette64.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette64.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette64.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette64.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette64.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette64.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette64.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette64.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette64.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette64.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette64.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette64.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette64.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette64.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette64.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette64.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette64.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette64.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette64.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette64.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette64.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette64.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette64.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette64.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette64.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette64.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette64.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette64.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette64.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette64.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette64.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette64.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette64.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette64.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette64.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette64.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette64.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette64.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confCoadd4Frame->setPalette(palette64);
        confCoadd4Frame->setAutoFillBackground(true);
        confCoadd4Frame->setFrameShape(QFrame::NoFrame);
        confCoadd4Frame->setFrameShadow(QFrame::Plain);
        gridLayout_5 = new QGridLayout(confCoadd4Frame);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        COAuniqueidLineEdit = new QLineEdit(confCoadd4Frame);
        COAuniqueidLineEdit->setObjectName(QStringLiteral("COAuniqueidLineEdit"));

        gridLayout_5->addWidget(COAuniqueidLineEdit, 1, 0, 1, 2);

        COAchipsLineEdit = new QLineEdit(confCoadd4Frame);
        COAchipsLineEdit->setObjectName(QStringLiteral("COAchipsLineEdit"));
        QSizePolicy sizePolicy8(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(COAchipsLineEdit->sizePolicy().hasHeightForWidth());
        COAchipsLineEdit->setSizePolicy(sizePolicy8);

        gridLayout_5->addWidget(COAchipsLineEdit, 3, 0, 1, 2);

        label_54 = new QLabel(confCoadd4Frame);
        label_54->setObjectName(QStringLiteral("label_54"));
        label_54->setWordWrap(true);

        gridLayout_5->addWidget(label_54, 4, 0, 1, 1);

        COAedgesmoothingLineEdit = new QLineEdit(confCoadd4Frame);
        COAedgesmoothingLineEdit->setObjectName(QStringLiteral("COAedgesmoothingLineEdit"));
        COAedgesmoothingLineEdit->setMinimumSize(QSize(100, 0));
        COAedgesmoothingLineEdit->setMaximumSize(QSize(100, 16777215));

        gridLayout_5->addWidget(COAedgesmoothingLineEdit, 4, 1, 1, 1);

        COAsizexLineEdit = new QLineEdit(confCoadd4Frame);
        COAsizexLineEdit->setObjectName(QStringLiteral("COAsizexLineEdit"));
        COAsizexLineEdit->setMinimumSize(QSize(100, 0));
        COAsizexLineEdit->setMaximumSize(QSize(100, 16777215));

        gridLayout_5->addWidget(COAsizexLineEdit, 5, 1, 1, 1);

        label_64 = new QLabel(confCoadd4Frame);
        label_64->setObjectName(QStringLiteral("label_64"));

        gridLayout_5->addWidget(label_64, 5, 0, 1, 1);

        COAsizeyLineEdit = new QLineEdit(confCoadd4Frame);
        COAsizeyLineEdit->setObjectName(QStringLiteral("COAsizeyLineEdit"));
        COAsizeyLineEdit->setMinimumSize(QSize(100, 0));
        COAsizeyLineEdit->setMaximumSize(QSize(100, 16777215));

        gridLayout_5->addWidget(COAsizeyLineEdit, 6, 1, 1, 1);

        label_65 = new QLabel(confCoadd4Frame);
        label_65->setObjectName(QStringLiteral("label_65"));

        gridLayout_5->addWidget(label_65, 6, 0, 1, 1);

        label_46 = new QLabel(confCoadd4Frame);
        label_46->setObjectName(QStringLiteral("label_46"));

        gridLayout_5->addWidget(label_46, 0, 0, 1, 2);

        label_47 = new QLabel(confCoadd4Frame);
        label_47->setObjectName(QStringLiteral("label_47"));

        gridLayout_5->addWidget(label_47, 2, 0, 1, 2);


        verticalLayout_12->addWidget(confCoadd4Frame);

        confsubtitleCoadd5Label = new QLabel(COCpage_2);
        confsubtitleCoadd5Label->setObjectName(QStringLiteral("confsubtitleCoadd5Label"));
        confsubtitleCoadd5Label->setFont(font);
        confsubtitleCoadd5Label->setWordWrap(true);

        verticalLayout_12->addWidget(confsubtitleCoadd5Label);

        confCoadd5Frame = new QFrame(COCpage_2);
        confCoadd5Frame->setObjectName(QStringLiteral("confCoadd5Frame"));
        QPalette palette65;
        palette65.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette65.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette65.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette65.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette65.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette65.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette65.setBrush(QPalette::Active, QPalette::Text, brush);
        palette65.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette65.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette65.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette65.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette65.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette65.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette65.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette65.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette65.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette65.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette65.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette65.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette65.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette65.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette65.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette65.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette65.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette65.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette65.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette65.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette65.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette65.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette65.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette65.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette65.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette65.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette65.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette65.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette65.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette65.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette65.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette65.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette65.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette65.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette65.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette65.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette65.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette65.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confCoadd5Frame->setPalette(palette65);
        confCoadd5Frame->setAutoFillBackground(true);
        confCoadd5Frame->setFrameShape(QFrame::NoFrame);
        confCoadd5Frame->setFrameShadow(QFrame::Plain);
        gridLayout_30 = new QGridLayout(confCoadd5Frame);
        gridLayout_30->setObjectName(QStringLiteral("gridLayout_30"));
        label_82 = new QLabel(confCoadd5Frame);
        label_82->setObjectName(QStringLiteral("label_82"));

        gridLayout_30->addWidget(label_82, 0, 0, 1, 1);

        COApmComboBox = new QComboBox(confCoadd5Frame);
        COApmComboBox->setObjectName(QStringLiteral("COApmComboBox"));
        sizePolicy8.setHeightForWidth(COApmComboBox->sizePolicy().hasHeightForWidth());
        COApmComboBox->setSizePolicy(sizePolicy8);
        COApmComboBox->setMinimumSize(QSize(100, 0));
        COApmComboBox->setMaximumSize(QSize(100, 16777215));
        COApmComboBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_30->addWidget(COApmComboBox, 0, 1, 1, 1);

        label_62 = new QLabel(confCoadd5Frame);
        label_62->setObjectName(QStringLiteral("label_62"));

        gridLayout_30->addWidget(label_62, 1, 0, 1, 1);

        COApmraLineEdit = new QLineEdit(confCoadd5Frame);
        COApmraLineEdit->setObjectName(QStringLiteral("COApmraLineEdit"));
        COApmraLineEdit->setMinimumSize(QSize(100, 0));
        COApmraLineEdit->setMaximumSize(QSize(100, 16777215));

        gridLayout_30->addWidget(COApmraLineEdit, 1, 1, 1, 1);

        label_63 = new QLabel(confCoadd5Frame);
        label_63->setObjectName(QStringLiteral("label_63"));

        gridLayout_30->addWidget(label_63, 2, 0, 1, 1);

        COApmdecLineEdit = new QLineEdit(confCoadd5Frame);
        COApmdecLineEdit->setObjectName(QStringLiteral("COApmdecLineEdit"));
        COApmdecLineEdit->setMinimumSize(QSize(100, 0));
        COApmdecLineEdit->setMaximumSize(QSize(100, 16777215));

        gridLayout_30->addWidget(COApmdecLineEdit, 2, 1, 1, 1);


        verticalLayout_12->addWidget(confCoadd5Frame);

        confsubtitleCoadd6Label = new QLabel(COCpage_2);
        confsubtitleCoadd6Label->setObjectName(QStringLiteral("confsubtitleCoadd6Label"));
        confsubtitleCoadd6Label->setFont(font);
        confsubtitleCoadd6Label->setWordWrap(true);

        verticalLayout_12->addWidget(confsubtitleCoadd6Label);

        confCoadd6Frame = new QFrame(COCpage_2);
        confCoadd6Frame->setObjectName(QStringLiteral("confCoadd6Frame"));
        QPalette palette66;
        palette66.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette66.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette66.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette66.setBrush(QPalette::Active, QPalette::Midlight, brush8);
        palette66.setBrush(QPalette::Active, QPalette::Dark, brush9);
        palette66.setBrush(QPalette::Active, QPalette::Mid, brush10);
        palette66.setBrush(QPalette::Active, QPalette::Text, brush);
        palette66.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette66.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette66.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette66.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette66.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette66.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        palette66.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette66.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette66.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette66.setBrush(QPalette::Inactive, QPalette::Button, brush7);
        palette66.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette66.setBrush(QPalette::Inactive, QPalette::Midlight, brush8);
        palette66.setBrush(QPalette::Inactive, QPalette::Dark, brush9);
        palette66.setBrush(QPalette::Inactive, QPalette::Mid, brush10);
        palette66.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette66.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette66.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette66.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette66.setBrush(QPalette::Inactive, QPalette::Window, brush7);
        palette66.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette66.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette66.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette66.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette66.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        palette66.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        palette66.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette66.setBrush(QPalette::Disabled, QPalette::Midlight, brush8);
        palette66.setBrush(QPalette::Disabled, QPalette::Dark, brush9);
        palette66.setBrush(QPalette::Disabled, QPalette::Mid, brush10);
        palette66.setBrush(QPalette::Disabled, QPalette::Text, brush9);
        palette66.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette66.setBrush(QPalette::Disabled, QPalette::ButtonText, brush9);
        palette66.setBrush(QPalette::Disabled, QPalette::Base, brush7);
        palette66.setBrush(QPalette::Disabled, QPalette::Window, brush7);
        palette66.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette66.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette66.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette66.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confCoadd6Frame->setPalette(palette66);
        confCoadd6Frame->setAutoFillBackground(true);
        confCoadd6Frame->setFrameShape(QFrame::NoFrame);
        confCoadd6Frame->setFrameShadow(QFrame::Plain);
        gridLayout_40 = new QGridLayout(confCoadd6Frame);
        gridLayout_40->setObjectName(QStringLiteral("gridLayout_40"));
        COAfluxcalibCheckBox = new QCheckBox(confCoadd6Frame);
        COAfluxcalibCheckBox->setObjectName(QStringLiteral("COAfluxcalibCheckBox"));
        COAfluxcalibCheckBox->setEnabled(true);
        COAfluxcalibCheckBox->setFocusPolicy(Qt::ClickFocus);

        gridLayout_40->addWidget(COAfluxcalibCheckBox, 0, 0, 1, 1);


        verticalLayout_12->addWidget(confCoadd6Frame);

        verticalSpacer_53 = new QSpacerItem(17, 124, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_12->addItem(verticalSpacer_53);


        gridLayout_67->addLayout(verticalLayout_12, 0, 1, 1, 1);

        horizontalSpacer_16 = new QSpacerItem(12, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_67->addItem(horizontalSpacer_16, 0, 2, 1, 1);

        confStackedWidget->addWidget(COCpage_2);

        gridLayout->addWidget(confStackedWidget, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        parametersDefaultPushButton = new QPushButton(dockWidgetContents);
        parametersDefaultPushButton->setObjectName(QStringLiteral("parametersDefaultPushButton"));
        parametersDefaultPushButton->setFocusPolicy(Qt::ClickFocus);

        horizontalLayout_2->addWidget(parametersDefaultPushButton);

        confBackwardPushButton = new QPushButton(dockWidgetContents);
        confBackwardPushButton->setObjectName(QStringLiteral("confBackwardPushButton"));
        confBackwardPushButton->setMinimumSize(QSize(60, 0));
        confBackwardPushButton->setMaximumSize(QSize(60, 16777215));
        QPalette palette67;
        palette67.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush36(QColor(255, 255, 170, 255));
        brush36.setStyle(Qt::SolidPattern);
        palette67.setBrush(QPalette::Active, QPalette::Button, brush36);
        palette67.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush37(QColor(255, 255, 212, 255));
        brush37.setStyle(Qt::SolidPattern);
        palette67.setBrush(QPalette::Active, QPalette::Midlight, brush37);
        QBrush brush38(QColor(127, 127, 85, 255));
        brush38.setStyle(Qt::SolidPattern);
        palette67.setBrush(QPalette::Active, QPalette::Dark, brush38);
        QBrush brush39(QColor(170, 170, 113, 255));
        brush39.setStyle(Qt::SolidPattern);
        palette67.setBrush(QPalette::Active, QPalette::Mid, brush39);
        palette67.setBrush(QPalette::Active, QPalette::Text, brush);
        palette67.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette67.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette67.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette67.setBrush(QPalette::Active, QPalette::Window, brush36);
        palette67.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette67.setBrush(QPalette::Active, QPalette::AlternateBase, brush37);
        palette67.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette67.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette67.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette67.setBrush(QPalette::Inactive, QPalette::Button, brush36);
        palette67.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette67.setBrush(QPalette::Inactive, QPalette::Midlight, brush37);
        palette67.setBrush(QPalette::Inactive, QPalette::Dark, brush38);
        palette67.setBrush(QPalette::Inactive, QPalette::Mid, brush39);
        palette67.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette67.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette67.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette67.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette67.setBrush(QPalette::Inactive, QPalette::Window, brush36);
        palette67.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette67.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush37);
        palette67.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette67.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette67.setBrush(QPalette::Disabled, QPalette::WindowText, brush38);
        palette67.setBrush(QPalette::Disabled, QPalette::Button, brush36);
        palette67.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette67.setBrush(QPalette::Disabled, QPalette::Midlight, brush37);
        palette67.setBrush(QPalette::Disabled, QPalette::Dark, brush38);
        palette67.setBrush(QPalette::Disabled, QPalette::Mid, brush39);
        palette67.setBrush(QPalette::Disabled, QPalette::Text, brush38);
        palette67.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette67.setBrush(QPalette::Disabled, QPalette::ButtonText, brush38);
        palette67.setBrush(QPalette::Disabled, QPalette::Base, brush36);
        palette67.setBrush(QPalette::Disabled, QPalette::Window, brush36);
        palette67.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette67.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush36);
        palette67.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette67.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confBackwardPushButton->setPalette(palette67);
        confBackwardPushButton->setFocusPolicy(Qt::ClickFocus);

        horizontalLayout_2->addWidget(confBackwardPushButton);

        confForwardPushButton = new QPushButton(dockWidgetContents);
        confForwardPushButton->setObjectName(QStringLiteral("confForwardPushButton"));
        confForwardPushButton->setMinimumSize(QSize(60, 0));
        confForwardPushButton->setMaximumSize(QSize(60, 16777215));
        QPalette palette68;
        palette68.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette68.setBrush(QPalette::Active, QPalette::Button, brush36);
        palette68.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette68.setBrush(QPalette::Active, QPalette::Midlight, brush37);
        palette68.setBrush(QPalette::Active, QPalette::Dark, brush38);
        palette68.setBrush(QPalette::Active, QPalette::Mid, brush39);
        palette68.setBrush(QPalette::Active, QPalette::Text, brush);
        palette68.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette68.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette68.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette68.setBrush(QPalette::Active, QPalette::Window, brush36);
        palette68.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette68.setBrush(QPalette::Active, QPalette::AlternateBase, brush37);
        palette68.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette68.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette68.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette68.setBrush(QPalette::Inactive, QPalette::Button, brush36);
        palette68.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette68.setBrush(QPalette::Inactive, QPalette::Midlight, brush37);
        palette68.setBrush(QPalette::Inactive, QPalette::Dark, brush38);
        palette68.setBrush(QPalette::Inactive, QPalette::Mid, brush39);
        palette68.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette68.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette68.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette68.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette68.setBrush(QPalette::Inactive, QPalette::Window, brush36);
        palette68.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette68.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush37);
        palette68.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette68.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette68.setBrush(QPalette::Disabled, QPalette::WindowText, brush38);
        palette68.setBrush(QPalette::Disabled, QPalette::Button, brush36);
        palette68.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette68.setBrush(QPalette::Disabled, QPalette::Midlight, brush37);
        palette68.setBrush(QPalette::Disabled, QPalette::Dark, brush38);
        palette68.setBrush(QPalette::Disabled, QPalette::Mid, brush39);
        palette68.setBrush(QPalette::Disabled, QPalette::Text, brush38);
        palette68.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette68.setBrush(QPalette::Disabled, QPalette::ButtonText, brush38);
        palette68.setBrush(QPalette::Disabled, QPalette::Base, brush36);
        palette68.setBrush(QPalette::Disabled, QPalette::Window, brush36);
        palette68.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette68.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush36);
        palette68.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette68.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        confForwardPushButton->setPalette(palette68);
        confForwardPushButton->setFocusPolicy(Qt::ClickFocus);

        horizontalLayout_2->addWidget(confForwardPushButton);

        confPageLabel = new QLabel(dockWidgetContents);
        confPageLabel->setObjectName(QStringLiteral("confPageLabel"));

        horizontalLayout_2->addWidget(confPageLabel);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_12);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        ConfDockWidget->setWidget(dockWidgetContents);
#ifndef QT_NO_SHORTCUT
        xtalkRowLabel->setBuddy(rowxtalkAmplitudeLineEdit);
        xtalkNorLabel->setBuddy(normalxtalkAmplitudeLineEdit);
        label_3->setBuddy(biasNlowLineEdit);
        label_4->setBuddy(darkNlowLineEdit);
        label_15->setBuddy(flatoffNlowLineEdit);
        label_5->setBuddy(flatNlowLineEdit);
        label_11->setBuddy(biasMinLineEdit);
        label_12->setBuddy(darkMinLineEdit);
        label_16->setBuddy(flatoffMinLineEdit);
        label_13->setBuddy(flatMinLineEdit);
        label_59->setBuddy(chopnodComboBox);
        label->setBuddy(SPSnumbergroupsLineEdit);
        label_10->setBuddy(SPSlengthLineEdit);
        label_24->setBuddy(SPSlengthLineEdit);
        BACmefLabel->setBuddy(BACmefLineEdit);
        BACmethodLabel->setBuddy(BACmethodComboBox);
        BACDTLabel->setBuddy(BACDTLineEdit);
        BACDMINLabel->setBuddy(BACDMINLineEdit);
        conf1passLabel->setBuddy(BAC1nlowLineEdit);
        conf2passLabel->setBuddy(BAC2nlowLineEdit);
        confBrightstarMagLabel->setBuddy(BACmagLineEdit);
        confBrightstarDistanceLabel->setBuddy(BACdistLineEdit);
        BACgapsizeLabel->setBuddy(BACgapsizeLineEdit);
        BACwindowLabel->setBuddy(BACwindowLineEdit);
        confCollapseDTLabel->setBuddy(COCDTLineEdit);
        confCollapseDMINLabel->setBuddy(COCDMINLineEdit);
        confCollapseMEFLabel->setBuddy(COCmefLineEdit);
        label_26->setBuddy(COCrejectLineEdit);
        label_27->setBuddy(COCdirectionComboBox);
        confCollapseExcludeXLabel_3->setBuddy(COCxminLineEdit);
        confCollapseExcludeXLabel_2->setBuddy(COCxminLineEdit);
        confCollapseExcludeXLabel->setBuddy(COCxminLineEdit);
        confCollapseExcludeYLabel->setBuddy(COCyminLineEdit);
        confBinningLabel->setBuddy(BIPSpinBox);
        label_32->setBuddy(CGWdarkminLineEdit);
        label_33->setBuddy(CGWflatminLineEdit);
        label_40->setBuddy(CGWbackminLineEdit);
        label_36->setBuddy(CGWflatsmoothLineEdit);
        label_37->setBuddy(CGWflatrowtolLineEdit);
        label_38->setBuddy(CGWflatcoltolLineEdit);
        label_39->setBuddy(CGWflatclustolLineEdit);
        confIndWMinLabel->setBuddy(CIWminaduLineEdit);
        confIndWMaxLabel->setBuddy(CIWmaxaduLineEdit);
        confIndWBloomMarginLabel_3->setBuddy(CIWbloomRangeLineEdit);
        confIndWBloomMarginLabel->setBuddy(CIWbloomRangeLineEdit);
        confIndWBloomMinLabel->setBuddy(CIWbloomMinaduLineEdit);
        confIndWBloomMarginLabel_2->setBuddy(CIWbloomRangeLineEdit);
        confIndWThresholdLabel->setBuddy(CIWaggressivenessLineEdit);
        confIndWBloomMinLabel_2->setBuddy(CIWbloomMinaduLineEdit);
        separationLabel->setBuddy(separateTargetLineEdit);
        label_41->setBuddy(APIrefcatComboBox);
        confAPILabel_3->setBuddy(APIfilterComboBox);
        confAPILabel_13->setBuddy(APIcolorComboBox);
        confAPILabel_11->setBuddy(APIcalibrationmodeComboBox);
        confAPILabel->setBuddy(APIapertureLineEdit);
        confAPILabel_2->setBuddy(APIfilterkeywordLineEdit);
        confAPILabel_4->setBuddy(APIextinctionLineEdit);
        confAPILabel_5->setBuddy(APIcolortermLineEdit);
        confAPILabel_7->setBuddy(APIzpmaxLineEdit);
        confAPILabel_15->setBuddy(APIthresholdLineEdit);
        confAPILabel_10->setBuddy(APIconvergenceLineEdit);
        confAPILabel_14->setBuddy(APIzpmaxLineEdit);
        confAPILabel_6->setBuddy(APIkappaLineEdit);
        confAPILabel_8->setBuddy(APIminmagLineEdit);
        confAPILabel_12->setBuddy(APIWCSLineEdit);
        confSourcecatDTLabel->setBuddy(CSCDTLineEdit);
        confSourcecatDMINLabel->setBuddy(CSCDTLineEdit);
        confSourcecatDeblendLabel->setBuddy(CSCmincontLineEdit);
        confSourcecatFWHMLabel->setBuddy(CSCFWHMLineEdit);
        ARCserverLabel_2->setBuddy(ARCcatalogComboBox);
        ARCmaxpmLabel->setBuddy(ARCraLineEdit);
        ARCpmRALabel->setBuddy(ARCraLineEdit);
        ARCpmDECLabel->setBuddy(ARCdecLineEdit);
        ARCraLabel->setBuddy(ARCraLineEdit);
        ARCdecLabel->setBuddy(ARCdecLineEdit);
        ARCtargetresolverLabel->setBuddy(ARCtargetresolverLineEdit);
        ARCminmagLabel->setBuddy(ARCminmagLineEdit);
        ARCradiusLabel->setBuddy(ARCradiusLineEdit);
        confSourcecatDTLabel_3->setBuddy(CSCDTLineEdit);
        confSourcecatDTLabel_4->setBuddy(CSCDTLineEdit);
        ASTLabel_16->setBuddy(ASTmosaictypeComboBox);
        ASTposangleLabel->setBuddy(ASTposangleLineEdit);
        ASTpositionLabel->setBuddy(ASTpositionLineEdit);
        ASTLabel_3->setBuddy(ASTpixscaleLineEdit);
        ASTLabel_6->setBuddy(ASTcrossidLineEdit);
        ASTLabel_10->setBuddy(ASTastrinstrukeyLineEdit);
        ASTLabel_15->setBuddy(ASTastrefweightLineEdit);
        ASTLabel_7->setBuddy(ASTsnthreshLineEdit);
        ASTLabel_14->setBuddy(ASTphotinstrukeyLineEdit);
        ASTLabel_8->setBuddy(ASTdistortgroupsLineEdit);
        ASTLabel_9->setBuddy(ASTdistortkeysLineEdit);
        ASTLabel_4->setBuddy(ASTdistortLineEdit);
        ASTLabel_11->setBuddy(ASTstabilityComboBox);
        ASTLabel_12->setBuddy(ASTmosaictypeComboBox);
        ASTLabel_13->setBuddy(ASTfocalplaneComboBox);
        ASTLabel_5->setBuddy(ASTdistortLineEdit);
        confAPDRefcatLabel->setBuddy(APDrefcatComboBox);
        confAPDFilterLabel->setBuddy(APDfilterComboBox);
        confAPDMaxphoterrorLabel->setBuddy(APDmaxphoterrorLineEdit);
        confCollapseDMINLabel_2->setBuddy(skyDMINLineEdit);
        confCollapseDTLabel_2->setBuddy(skyDTLineEdit);
        confCollapseDMINLabel_3->setBuddy(skyKernelLineEdit);
        confCollapseMEFLabel_2->setBuddy(skyMefLineEdit);
        COAtype1Label->setBuddy(COAraLineEdit);
        COAtype2Label->setBuddy(COAdecLineEdit);
        COCkernelLabel->setBuddy(COAcombinetypeComboBox);
        COCkernelLabel_2->setBuddy(COAkernelComboBox);
        label_53->setBuddy(COApixscaleLineEdit);
        COCkernelLabel_3->setBuddy(COAprojectionComboBox);
        COCkernelLabel_4->setBuddy(COAcelestialtypeComboBox);
        COCkernelLabel_5->setBuddy(COAskypaLineEdit);
        label_56->setBuddy(COAoutthreshLineEdit);
        label_58->setBuddy(COAoutsizeLineEdit);
        label_57->setBuddy(COAoutborderLineEdit);
        label_54->setBuddy(COAedgesmoothingLineEdit);
        label_64->setBuddy(COAsizexLineEdit);
        label_65->setBuddy(COAsizeyLineEdit);
        label_46->setBuddy(COAuniqueidLineEdit);
        label_47->setBuddy(COAchipsLineEdit);
        label_82->setBuddy(COApmComboBox);
        label_62->setBuddy(COApmraLineEdit);
        label_63->setBuddy(COApmdecLineEdit);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(normalxtalkAmplitudeLineEdit, rowxtalkAmplitudeLineEdit);
        QWidget::setTabOrder(rowxtalkAmplitudeLineEdit, biasNlowLineEdit);
        QWidget::setTabOrder(biasNlowLineEdit, biasNhighLineEdit);
        QWidget::setTabOrder(biasNhighLineEdit, darkNlowLineEdit);
        QWidget::setTabOrder(darkNlowLineEdit, darkNhighLineEdit);
        QWidget::setTabOrder(darkNhighLineEdit, flatoffNlowLineEdit);
        QWidget::setTabOrder(flatoffNlowLineEdit, flatoffNhighLineEdit);
        QWidget::setTabOrder(flatoffNhighLineEdit, flatNlowLineEdit);
        QWidget::setTabOrder(flatNlowLineEdit, flatNhighLineEdit);
        QWidget::setTabOrder(flatNhighLineEdit, biasMinLineEdit);
        QWidget::setTabOrder(biasMinLineEdit, biasMaxLineEdit);
        QWidget::setTabOrder(biasMaxLineEdit, darkMinLineEdit);
        QWidget::setTabOrder(darkMinLineEdit, darkMaxLineEdit);
        QWidget::setTabOrder(darkMaxLineEdit, flatoffMinLineEdit);
        QWidget::setTabOrder(flatoffMinLineEdit, flatoffMaxLineEdit);
        QWidget::setTabOrder(flatoffMaxLineEdit, flatMinLineEdit);
        QWidget::setTabOrder(flatMinLineEdit, flatMaxLineEdit);
        QWidget::setTabOrder(flatMaxLineEdit, SPSnumbergroupsLineEdit);
        QWidget::setTabOrder(SPSnumbergroupsLineEdit, SPSlengthLineEdit);
        QWidget::setTabOrder(SPSlengthLineEdit, SPSexcludeLineEdit);
        QWidget::setTabOrder(SPSexcludeLineEdit, BACDTLineEdit);
        QWidget::setTabOrder(BACDTLineEdit, BACDMINLineEdit);
        QWidget::setTabOrder(BACDMINLineEdit, BACmefLineEdit);
        QWidget::setTabOrder(BACmefLineEdit, BAC1nlowLineEdit);
        QWidget::setTabOrder(BAC1nlowLineEdit, BAC1nhighLineEdit);
        QWidget::setTabOrder(BAC1nhighLineEdit, BAC2nlowLineEdit);
        QWidget::setTabOrder(BAC2nlowLineEdit, BAC2nhighLineEdit);
        QWidget::setTabOrder(BAC2nhighLineEdit, BACmagLineEdit);
        QWidget::setTabOrder(BACmagLineEdit, BACdistLineEdit);
        QWidget::setTabOrder(BACdistLineEdit, BACbacksmoothscaleLineEdit);
        QWidget::setTabOrder(BACbacksmoothscaleLineEdit, BACwindowLineEdit);
        QWidget::setTabOrder(BACwindowLineEdit, BACminWindowLineEdit);
        QWidget::setTabOrder(BACminWindowLineEdit, BACgapsizeLineEdit);
        QWidget::setTabOrder(BACgapsizeLineEdit, COCDTLineEdit);
        QWidget::setTabOrder(COCDTLineEdit, COCDMINLineEdit);
        QWidget::setTabOrder(COCDMINLineEdit, COCmefLineEdit);
        QWidget::setTabOrder(COCmefLineEdit, COCrejectLineEdit);
        QWidget::setTabOrder(COCrejectLineEdit, COCxminLineEdit);
        QWidget::setTabOrder(COCxminLineEdit, COCxmaxLineEdit);
        QWidget::setTabOrder(COCxmaxLineEdit, COCyminLineEdit);
        QWidget::setTabOrder(COCyminLineEdit, COCymaxLineEdit);
        QWidget::setTabOrder(COCymaxLineEdit, CGWdarkminLineEdit);
        QWidget::setTabOrder(CGWdarkminLineEdit, CGWdarkmaxLineEdit);
        QWidget::setTabOrder(CGWdarkmaxLineEdit, CGWflatminLineEdit);
        QWidget::setTabOrder(CGWflatminLineEdit, CGWflatmaxLineEdit);
        QWidget::setTabOrder(CGWflatmaxLineEdit, CGWbackminLineEdit);
        QWidget::setTabOrder(CGWbackminLineEdit, CGWbackmaxLineEdit);
        QWidget::setTabOrder(CGWbackmaxLineEdit, CGWflatsmoothLineEdit);
        QWidget::setTabOrder(CGWflatsmoothLineEdit, CGWbacksmoothLineEdit);
        QWidget::setTabOrder(CGWbacksmoothLineEdit, CGWflatrowtolLineEdit);
        QWidget::setTabOrder(CGWflatrowtolLineEdit, CGWbackrowtolLineEdit);
        QWidget::setTabOrder(CGWbackrowtolLineEdit, CGWflatcoltolLineEdit);
        QWidget::setTabOrder(CGWflatcoltolLineEdit, CGWbackcoltolLineEdit);
        QWidget::setTabOrder(CGWbackcoltolLineEdit, CGWflatclustolLineEdit);
        QWidget::setTabOrder(CGWflatclustolLineEdit, CGWbackclustolLineEdit);
        QWidget::setTabOrder(CGWbackclustolLineEdit, CIWminaduLineEdit);
        QWidget::setTabOrder(CIWminaduLineEdit, CIWmaxaduLineEdit);
        QWidget::setTabOrder(CIWmaxaduLineEdit, CIWaggressivenessLineEdit);
        QWidget::setTabOrder(CIWaggressivenessLineEdit, CIWbloomMinaduLineEdit);
        QWidget::setTabOrder(CIWbloomMinaduLineEdit, CIWbloomRangeLineEdit);
        QWidget::setTabOrder(CIWbloomRangeLineEdit, CIWpersistenceTimescaleLineEdit);
        QWidget::setTabOrder(CIWpersistenceTimescaleLineEdit, CIWpersistenceMinaduLineEdit);
        QWidget::setTabOrder(CIWpersistenceMinaduLineEdit, separateTargetLineEdit);
        QWidget::setTabOrder(separateTargetLineEdit, APIapertureLineEdit);
        QWidget::setTabOrder(APIapertureLineEdit, APIfilterkeywordLineEdit);
        QWidget::setTabOrder(APIfilterkeywordLineEdit, APIextinctionLineEdit);
        QWidget::setTabOrder(APIextinctionLineEdit, APIcolortermLineEdit);
        QWidget::setTabOrder(APIcolortermLineEdit, APIzpminLineEdit);
        QWidget::setTabOrder(APIzpminLineEdit, APIzpmaxLineEdit);
        QWidget::setTabOrder(APIzpmaxLineEdit, APIkappaLineEdit);
        QWidget::setTabOrder(APIkappaLineEdit, APIthresholdLineEdit);
        QWidget::setTabOrder(APIthresholdLineEdit, APIminmagLineEdit);
        QWidget::setTabOrder(APIminmagLineEdit, APIconvergenceLineEdit);
        QWidget::setTabOrder(APIconvergenceLineEdit, APIWCSLineEdit);
        QWidget::setTabOrder(APIWCSLineEdit, CSCDTLineEdit);
        QWidget::setTabOrder(CSCDTLineEdit, CSCDMINLineEdit);
        QWidget::setTabOrder(CSCDMINLineEdit, CSCmincontLineEdit);
        QWidget::setTabOrder(CSCmincontLineEdit, CSCFWHMLineEdit);
        QWidget::setTabOrder(CSCFWHMLineEdit, CSCmaxflagLineEdit);
        QWidget::setTabOrder(CSCmaxflagLineEdit, CSCsaturationLineEdit);
        QWidget::setTabOrder(CSCsaturationLineEdit, CSCbackgroundLineEdit);
        QWidget::setTabOrder(CSCbackgroundLineEdit, ARCmaxpmLineEdit);
        QWidget::setTabOrder(ARCmaxpmLineEdit, ARCpmRALineEdit);
        QWidget::setTabOrder(ARCpmRALineEdit, ARCpmDECLineEdit);
        QWidget::setTabOrder(ARCpmDECLineEdit, ARCraLineEdit);
        QWidget::setTabOrder(ARCraLineEdit, ARCdecLineEdit);
        QWidget::setTabOrder(ARCdecLineEdit, ARCtargetresolverLineEdit);
        QWidget::setTabOrder(ARCtargetresolverLineEdit, ARCminmagLineEdit);
        QWidget::setTabOrder(ARCminmagLineEdit, ARCradiusLineEdit);
        QWidget::setTabOrder(ARCradiusLineEdit, ARCselectimageLineEdit);
        QWidget::setTabOrder(ARCselectimageLineEdit, ARCDTLineEdit);
        QWidget::setTabOrder(ARCDTLineEdit, ARCDMINLineEdit);
        QWidget::setTabOrder(ARCDMINLineEdit, ASTxcorrDTLineEdit);
        QWidget::setTabOrder(ASTxcorrDTLineEdit, ASTxcorrDMINLineEdit);
        QWidget::setTabOrder(ASTxcorrDMINLineEdit, ASTposangleLineEdit);
        QWidget::setTabOrder(ASTposangleLineEdit, ASTpositionLineEdit);
        QWidget::setTabOrder(ASTpositionLineEdit, ASTpixscaleLineEdit);
        QWidget::setTabOrder(ASTpixscaleLineEdit, ASTcrossidLineEdit);
        QWidget::setTabOrder(ASTcrossidLineEdit, ASTastrefweightLineEdit);
        QWidget::setTabOrder(ASTastrefweightLineEdit, ASTsnthreshLineEdit);
        QWidget::setTabOrder(ASTsnthreshLineEdit, ASTastrinstrukeyLineEdit);
        QWidget::setTabOrder(ASTastrinstrukeyLineEdit, ASTphotinstrukeyLineEdit);
        QWidget::setTabOrder(ASTphotinstrukeyLineEdit, ASTdistortLineEdit);
        QWidget::setTabOrder(ASTdistortLineEdit, ASTdistortgroupsLineEdit);
        QWidget::setTabOrder(ASTdistortgroupsLineEdit, ASTdistortkeysLineEdit);
        QWidget::setTabOrder(ASTdistortkeysLineEdit, APDmaxphoterrorLineEdit);
        QWidget::setTabOrder(APDmaxphoterrorLineEdit, skyDTLineEdit);
        QWidget::setTabOrder(skyDTLineEdit, skyDMINLineEdit);
        QWidget::setTabOrder(skyDMINLineEdit, skyKernelLineEdit);
        QWidget::setTabOrder(skyKernelLineEdit, skyMefLineEdit);
        QWidget::setTabOrder(skyMefLineEdit, COAraLineEdit);
        QWidget::setTabOrder(COAraLineEdit, COAdecLineEdit);
        QWidget::setTabOrder(COAdecLineEdit, COApixscaleLineEdit);
        QWidget::setTabOrder(COApixscaleLineEdit, COAskypaLineEdit);
        QWidget::setTabOrder(COAskypaLineEdit, COAoutthreshLineEdit);
        QWidget::setTabOrder(COAoutthreshLineEdit, COAoutsizeLineEdit);
        QWidget::setTabOrder(COAoutsizeLineEdit, COAoutborderLineEdit);
        QWidget::setTabOrder(COAoutborderLineEdit, COAuniqueidLineEdit);
        QWidget::setTabOrder(COAuniqueidLineEdit, COAchipsLineEdit);
        QWidget::setTabOrder(COAchipsLineEdit, COAedgesmoothingLineEdit);
        QWidget::setTabOrder(COAedgesmoothingLineEdit, COAsizexLineEdit);
        QWidget::setTabOrder(COAsizexLineEdit, COAsizeyLineEdit);
        QWidget::setTabOrder(COAsizeyLineEdit, COApmraLineEdit);
        QWidget::setTabOrder(COApmraLineEdit, COApmdecLineEdit);

        retranslateUi(ConfDockWidget);

        confStackedWidget->setCurrentIndex(3);
        ARCstackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ConfDockWidget);
    } // setupUi

    void retranslateUi(QDockWidget *ConfDockWidget)
    {
        ConfDockWidget->setWindowTitle(QApplication::translate("ConfDockWidget", "Configuration", Q_NULLPTR));
        conftitleHDUreformatLabel->setText(QApplication::translate("ConfDockWidget", "<html><head/><body><p>HDU / Detector corrections</p></body></html>", Q_NULLPTR));
        confsubtitleHDUreformat1Label->setText(QApplication::translate("ConfDockWidget", "(1) Renaming", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        splitMIRcubeCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Split MIR cubes. Otherwise, the cubes are stacked right away.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        splitMIRcubeCheckBox->setText(QApplication::translate("ConfDockWidget", "Split MIR cubes", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        theliRenamingCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Renames the files to INST.FILTER.DATEOBS_CHIP.fits", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        theliRenamingCheckBox->setText(QApplication::translate("ConfDockWidget", "Use THELI naming scheme", Q_NULLPTR));
        confsubtitleHDUreformat2Label->setText(QApplication::translate("ConfDockWidget", "(2) Low-level corrections", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        overscanCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Applies overscan correction. Note: some instruments will always be corrected.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        overscanCheckBox->setText(QApplication::translate("ConfDockWidget", "Overscan correction :", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        nonlinearityCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Applies a nonlinearity correction for flats and science images (if the coefficients are known for the current instrument)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        nonlinearityCheckBox->setText(QApplication::translate("ConfDockWidget", "Nonlinearity correction", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        normalxtalkCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Check if you want to correct normal crosstalk (ghost images)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        normalxtalkCheckBox->setText(QApplication::translate("ConfDockWidget", "Point crosstalk correction", Q_NULLPTR));
        xtalkRowLabel->setText(QApplication::translate("ConfDockWidget", "Amplitude", Q_NULLPTR));
        xtalkNorLabel->setText(QApplication::translate("ConfDockWidget", "Amplitude", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        normalxtalkAmplitudeLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The crosstalk amplitude factor (can be negative)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        rowxtalkCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Check if you want to correct row crosstalk (ghost lines)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        rowxtalkCheckBox->setText(QApplication::translate("ConfDockWidget", "Row crosstalk correction", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        xtalk_nor_2x2ToolButton->setStatusTip(QApplication::translate("ConfDockWidget", "Select a crosstalk pattern", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        xtalk_nor_2x2ToolButton->setText(QApplication::translate("ConfDockWidget", "...", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        xtalk_nor_1x2ToolButton->setStatusTip(QApplication::translate("ConfDockWidget", "Select a crosstalk pattern", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        xtalk_nor_1x2ToolButton->setText(QApplication::translate("ConfDockWidget", "...", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        xtalk_nor_2x1ToolButton->setStatusTip(QApplication::translate("ConfDockWidget", "Select a crosstalk pattern", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        xtalk_nor_2x1ToolButton->setText(QApplication::translate("ConfDockWidget", "...", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        xtalk_row_2x2ToolButton->setStatusTip(QApplication::translate("ConfDockWidget", "Select a crosstalk pattern", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        xtalk_row_2x2ToolButton->setText(QApplication::translate("ConfDockWidget", "...", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        xtalk_row_1x2ToolButton->setStatusTip(QApplication::translate("ConfDockWidget", "Select a crosstalk pattern", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        xtalk_row_1x2ToolButton->setText(QApplication::translate("ConfDockWidget", "...", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        xtalk_row_2x1ToolButton->setStatusTip(QApplication::translate("ConfDockWidget", "Select a crosstalk pattern", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        xtalk_row_2x1ToolButton->setText(QApplication::translate("ConfDockWidget", "...", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        xtalk_col_2x2ToolButton->setStatusTip(QApplication::translate("ConfDockWidget", "Select a crosstalk pattern", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        xtalk_col_2x2ToolButton->setText(QApplication::translate("ConfDockWidget", "...", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        xtalk_col_1x2ToolButton->setStatusTip(QApplication::translate("ConfDockWidget", "Select a crosstalk pattern", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        xtalk_col_1x2ToolButton->setText(QApplication::translate("ConfDockWidget", "...", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        xtalk_col_2x1ToolButton->setStatusTip(QApplication::translate("ConfDockWidget", "Select a crosstalk pattern", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        xtalk_col_2x1ToolButton->setText(QApplication::translate("ConfDockWidget", "...", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        rowxtalkAmplitudeLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The crosstalk amplitude factor (can be negative)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        overscanMethodComboBox->clear();
        overscanMethodComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "Line median", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Line mean", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Global median", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        overscanMethodComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "Method used to combine images in the stack", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        saturationLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Saturation level in the raw data (in ADU)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_21->setText(QApplication::translate("ConfDockWidget", "Saturation [ ADU ]", Q_NULLPTR));
        conftitleExcludeDetectorsLabel->setText(QApplication::translate("ConfDockWidget", "Exclude unusable detectors", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        excludeDetectorsLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "A comma- or blank-separated list of the bad detector numbers", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_2->setText(QApplication::translate("ConfDockWidget", "The chip numbers of bad detectors", Q_NULLPTR));
        conftitleProcessbiasLabel->setText(QApplication::translate("ConfDockWidget", "Process BIAS / DARK / FLATOFF / FLAT", Q_NULLPTR));
        confsubtitleCalibrators1Label->setText(QApplication::translate("ConfDockWidget", "(1) Reject pixels / combine method", Q_NULLPTR));
        label_6->setText(QApplication::translate("ConfDockWidget", "nlow", Q_NULLPTR));
        label_7->setText(QApplication::translate("ConfDockWidget", "nhigh", Q_NULLPTR));
        label_22->setText(QApplication::translate("ConfDockWidget", "Method", Q_NULLPTR));
        label_3->setText(QApplication::translate("ConfDockWidget", "BIAS", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        biasNlowLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The number of low pixels to be rejected from the stack", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        biasNhighLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The number of high pixels to be rejected from the stack", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        biasMethodComboBox->clear();
        biasMethodComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "Median", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Mean", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        biasMethodComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "Method used to combine images in the stack", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_4->setText(QApplication::translate("ConfDockWidget", "DARK", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        darkNlowLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The number of low pixels to be rejected from the stack", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        darkNhighLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The number of high pixels to be rejected from the stack", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        darkMethodComboBox->clear();
        darkMethodComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "Median", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Mean", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        darkMethodComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "Method used to combine images in the stack", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_15->setText(QApplication::translate("ConfDockWidget", "FLAT OFF", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        flatoffNlowLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The number of low pixels to be rejected from the stack", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        flatoffNhighLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The number of high pixels to be rejected from the stack", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        flatoffMethodComboBox->clear();
        flatoffMethodComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "Median", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Mean", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        flatoffMethodComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "Method used to combine images in the stack", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_5->setText(QApplication::translate("ConfDockWidget", "FLAT", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        flatNlowLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The number of low pixels to be rejected from the stack", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        flatNhighLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The number of high pixels to be rejected from the stack", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        flatMethodComboBox->clear();
        flatMethodComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "Median", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Mean", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        flatMethodComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "Method used to combine images in the stack", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confsubtitleCalibrators2Label->setText(QApplication::translate("ConfDockWidget", "(2) Min / max counts [e-] allowed", Q_NULLPTR));
        label_8->setText(QApplication::translate("ConfDockWidget", "min", Q_NULLPTR));
        label_9->setText(QApplication::translate("ConfDockWidget", "max", Q_NULLPTR));
        label_11->setText(QApplication::translate("ConfDockWidget", "BIAS", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        biasMinLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The minimum mode to be accepted for an image to enter a stack. Per chip, not per exposure.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        biasMaxLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The maximum mode to be accepted for an image to enter a stack. Per chip, not per exposure.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        biasMaxLineEdit->setText(QString());
        label_12->setText(QApplication::translate("ConfDockWidget", "DARK", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        darkMinLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The minimum mode to be accepted for an image to enter a stack. Per chip, not per exposure.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        darkMaxLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The maximum mode to be accepted for an image to enter a stack. Per chip, not per exposure.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_16->setText(QApplication::translate("ConfDockWidget", "FLAT OFF", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        flatoffMinLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The minimum mode to be accepted for an image to enter a stack. Per chip, not per exposure.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        flatoffMaxLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The maximum mode to be accepted for an image to enter a stack. Per chip, not per exposure.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        flatMinLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The minimum mode to be accepted for an image to enter a stack. Per chip, not per exposure.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        flatMaxLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The maximum mode to be accepted for an image to enter a stack. Per chip, not per exposure.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_13->setText(QApplication::translate("ConfDockWidget", "FLAT", Q_NULLPTR));
        conftitleChopnodLabel->setText(QApplication::translate("ConfDockWidget", "Chop / nod skysub (MIR)", Q_NULLPTR));
        label_59->setText(QApplication::translate("ConfDockWidget", "Chopnod pattern", Q_NULLPTR));
        chopnodComboBox->clear();
        chopnodComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "0110", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "1001", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "0101", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "1010", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "pairwise", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        chopnodComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "The chopnod pattern", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        chopnodInvertCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Invert the pattern every 2nd time, e.g. 0110-1001-0110-1001 ...", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        chopnodInvertCheckBox->setText(QApplication::translate("ConfDockWidget", "Invert chopnod pattern", Q_NULLPTR));
        conftitleBackground1Label->setText(QApplication::translate("ConfDockWidget", "Background correction", Q_NULLPTR));
        confsubtitleBackground1Label->setText(QApplication::translate("ConfDockWidget", "(0) Grouping / exclude images", Q_NULLPTR));
        label->setText(QApplication::translate("ConfDockWidget", "Number of groups", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        SPSnumbergroupsLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The number of groups in a sequence with different background characteristics", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_10->setText(QApplication::translate("ConfDockWidget", "Se&quence length", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        SPSlengthLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The number of exposures in a sequence.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_24->setText(QApplication::translate("ConfDockWidget", "E&xclude 'n' first exposures", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        SPSexcludeLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "This many exposures are rejected at the beginning of a sequence of images at a given dither position", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        SPSexcludeLineEdit->setText(QString());
        confsubtitleBackground2Label->setText(QApplication::translate("ConfDockWidget", "(1) Object masking (optional)", Q_NULLPTR));
        BACmefLabel->setText(QApplication::translate("ConfDockWidget", "&Mask expansion factor", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        BACmefLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Increases the default object mask by this factor", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        BACmethodLabel->setText(QApplication::translate("ConfDockWidget", "Combine method", Q_NULLPTR));
        BACmethodComboBox->clear();
        BACmethodComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "Median", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Mean", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        BACmethodComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "Median is robust against outliers, but has higher noise.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        BAC2passCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Remove rough background model before object masking (recommended for NIR data)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        BAC2passCheckBox->setText(QApplication::translate("ConfDockWidget", "2-pass model", Q_NULLPTR));
        BACconvolutionCheckBox->setText(QApplication::translate("ConfDockWidget", "Convolution", Q_NULLPTR));
        BACDTLabel->setText(QApplication::translate("ConfDockWidget", "DT", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        BACDTLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "SExtractor detection threshold per pixel", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        BACDMINLabel->setText(QApplication::translate("ConfDockWidget", "DMIN", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        BACDMINLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "SExtractor minimum number of connected pixels above the detection threshold", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confsubtitleBackground3Label->setText(QApplication::translate("ConfDockWidget", "(2) Reject pixels during combination", Q_NULLPTR));
        label_18->setText(QApplication::translate("ConfDockWidget", "nlow", Q_NULLPTR));
        label_19->setText(QApplication::translate("ConfDockWidget", "nhigh", Q_NULLPTR));
        conf1passLabel->setText(QApplication::translate("ConfDockWidget", "&1st pass", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        BAC1nlowLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The number of low pixels removed from the stack", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        BAC1nhighLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The number of high pixels removed from the stack", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        conf2passLabel->setText(QApplication::translate("ConfDockWidget", "&2nd pass", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        BAC2nlowLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The number of low pixels removed from the stack", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        BAC2nhighLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The number of high pixels removed from the stack", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        conftitleBackground2Label->setText(QString());
        confsubtitleBackground4Label->setText(QApplication::translate("ConfDockWidget", "(3) Reject chips with bright stars", Q_NULLPTR));
        confBrightstarMagLabel->setText(QApplication::translate("ConfDockWidget", "Brighter than [ mag ]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        BACmagLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Chips containing stars brighter than this magnitude are excluded from the model", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        BACmagLineEdit->setText(QString());
        confBrightstarDistanceLabel->setText(QApplication::translate("ConfDockWidget", "Edge distance [ ' ]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        BACdistLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Chips are rejected if stars are located outside the chip less than this distance.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        BACdistLineEdit->setText(QString());
        confsubtitleBackground5Label->setText(QApplication::translate("ConfDockWidget", "(4) How to apply the model", Q_NULLPTR));
        BACapplyComboBox->clear();
        BACapplyComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "Subtract model", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Divide model", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        BACapplyComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "How to apply the background model", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_14->setText(QApplication::translate("ConfDockWidget", "Kernel width [ pixel ]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        BACbacksmoothscaleLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The smoothing scale for the background model (unsmoothed if left empty)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        BACrescaleCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Rescale the model to each exposure. DO NOT do this for bright / extended sources.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        BACrescaleCheckBox->setText(QApplication::translate("ConfDockWidget", "Rescale model", Q_NULLPTR));
        confsubtitleBackground6Label->setText(QApplication::translate("ConfDockWidget", "(5) Static or dynamic mode", Q_NULLPTR));
        BACcurrentmodeLabel->setText(QApplication::translate("ConfDockWidget", "The mode is: STATIC", Q_NULLPTR));
        BACgapsizeLabel->setText(QApplication::translate("ConfDockWidget", "Max gap size [hours]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        BACgapsizeLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "A pause in the sequence longer than this marks a break point. Models do not extend over breakpoints.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        BACgapsizeLineEdit->setText(QString());
        BACwindowLabel->setText(QApplication::translate("ConfDockWidget", "Window si&ze", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        BACwindowLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "How many contemporal images should be used to calculate the model. If empty or 0, all exposures will be used.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        BACwindowLineEdit->setText(QString());
#ifndef QT_NO_STATUSTIP
        BACminWindowLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The minimum required window size. Equal to 'window size' if left empty.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_23->setText(QApplication::translate("ConfDockWidget", "Min useful window size", Q_NULLPTR));
        conftitleCollapseLabel->setText(QApplication::translate("ConfDockWidget", "Collapse correction", Q_NULLPTR));
        confsubtitleCollapse1Label->setText(QApplication::translate("ConfDockWidget", "(1) Object masking", Q_NULLPTR));
        confCollapseDTLabel->setText(QApplication::translate("ConfDockWidget", "DT", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COCDTLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Detection threshold per pixel", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confCollapseDMINLabel->setText(QApplication::translate("ConfDockWidget", "DMIN", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COCDMINLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Minimum number of connected pixels above the detection threshold", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confCollapseMEFLabel->setText(QApplication::translate("ConfDockWidget", "&Mask expansion factor", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COCmefLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Increases the default object mask by this factor", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confsubtitleCollapse2Label->setText(QApplication::translate("ConfDockWidget", "(2) Collapse parameters", Q_NULLPTR));
        label_26->setText(QApplication::translate("ConfDockWidget", "Re&jection threshold", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COCrejectLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Sigma cutoff for pixels in a line/column. Default: 1.5", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_27->setText(QApplication::translate("ConfDockWidget", "Collapse direction", Q_NULLPTR));
        COCdirectionLabel->setText(QString());
        COCdirectionComboBox->clear();
        COCdirectionComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "x", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "y", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "xy", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "xyyx", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "yxxy", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "xxxx", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "yyyy", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        COCdirectionComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "Choose the gradient pattern that needs to be corrected.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confsubtitleCollapse3Label->setText(QApplication::translate("ConfDockWidget", "(3) Exclude this region", Q_NULLPTR));
        confCollapseExcludeXLabel_3->setText(QApplication::translate("ConfDockWidget", "min", Q_NULLPTR));
        confCollapseExcludeXLabel_2->setText(QApplication::translate("ConfDockWidget", "max", Q_NULLPTR));
        confCollapseExcludeXLabel->setText(QApplication::translate("ConfDockWidget", "&x", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COCxminLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "A rectangular region that should be excluded from the correction. Makes sense only for single-chip cameras.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        COCxmaxLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "A rectangular region that should be excluded from the correction. Makes sense only for single-chip cameras.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confCollapseExcludeYLabel->setText(QApplication::translate("ConfDockWidget", "y", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COCyminLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "A rectangular region that should be excluded from the correction. Makes sense only for single-chip cameras.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        COCymaxLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "A rectangular region that should be excluded from the correction. Makes sense only for single-chip cameras.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        conftitleBinnedpreviewLabel->setText(QApplication::translate("ConfDockWidget", "Binned preview", Q_NULLPTR));
        confBinningLabel->setText(QApplication::translate("ConfDockWidget", "Binning", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        BIPSpinBox->setStatusTip(QApplication::translate("ConfDockWidget", "The binning factor for the mosaiced image.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        BIPrejectCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Rejects the lowest and highest 10% of pixels from the binned image.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        BIPrejectCheckBox->setText(QApplication::translate("ConfDockWidget", "Reject outliers", Q_NULLPTR));
        conftitleGlobalweightLabel->setText(QApplication::translate("ConfDockWidget", "Global weight maps", Q_NULLPTR));
        confsubtitleGlobalweight1Label->setText(QApplication::translate("ConfDockWidget", "(1) Do not use the flat (?)", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CGWsameweightCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Uses a constant weight of 1 for all pixels if checked.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CGWsameweightCheckBox->setText(QApplication::translate("ConfDockWidget", "Same weight for all pixels", Q_NULLPTR));
        confsubtitleGlobalweight2Label->setText(QApplication::translate("ConfDockWidget", "(2) Allowed range for calibrators", Q_NULLPTR));
        label_29->setText(QApplication::translate("ConfDockWidget", "min", Q_NULLPTR));
        label_30->setText(QApplication::translate("ConfDockWidget", "max", Q_NULLPTR));
        label_32->setText(QApplication::translate("ConfDockWidget", "BIAS / DARK [e-]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CGWdarkminLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Pixels will be zero in the global weight if below this value in this reference image", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        CGWdarkmaxLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Pixels will be zero in the global weight if above this value in this reference image", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_33->setText(QApplication::translate("ConfDockWidget", "Norm. FLAT [e-]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CGWflatminLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Pixels will be zero in the global weight if below this value in this reference image", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CGWflatminLineEdit->setText(QApplication::translate("ConfDockWidget", "0.5", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CGWflatmaxLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Pixels will be zero in the global weight if above this value in this reference image", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CGWflatmaxLineEdit->setText(QApplication::translate("ConfDockWidget", "1.5", Q_NULLPTR));
        label_40->setText(QApplication::translate("ConfDockWidget", "Background [e-]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CGWbackminLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Pixels will be zero in the global weight if below this value in this reference image", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CGWbackminLineEdit->setText(QString());
#ifndef QT_NO_STATUSTIP
        CGWbackmaxLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Pixels will be zero in the global weight if above this value in this reference image", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CGWbackmaxLineEdit->setText(QString());
        confsubtitleGlobalweight3Label->setText(QApplication::translate("ConfDockWidget", "(3) Defect detection", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        confGlobalweight3Frame->setStatusTip(QApplication::translate("ConfDockWidget", "Will smooth a flat / background map with some kernel, and then detect bad rows / columns / clusters in the difference image", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_34->setText(QApplication::translate("ConfDockWidget", "FLAT", Q_NULLPTR));
        label_35->setText(QApplication::translate("ConfDockWidget", "BACK", Q_NULLPTR));
        label_36->setText(QApplication::translate("ConfDockWidget", "Kernel size [pix]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CGWflatsmoothLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Kernel size (suggest: 100)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        CGWbacksmoothLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Kernel size (suggest: 30)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_37->setText(QApplication::translate("ConfDockWidget", "Row tolerance", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CGWflatrowtolLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Pixels above this relative contrast in the difference image will be masked. Suggested value: 0.02", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        CGWbackrowtolLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Pixels above this relative contrast in the difference image will be masked. Suggested value: 0.02", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_38->setText(QApplication::translate("ConfDockWidget", "Column tolerance", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CGWflatcoltolLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Pixels above this relative contrast in the difference image will be masked. Suggested value: 0.02", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CGWflatcoltolLineEdit->setText(QString());
#ifndef QT_NO_STATUSTIP
        CGWbackcoltolLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Pixels above this relative contrast in the difference image will be masked. Suggested value: 0.02", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CGWbackcoltolLineEdit->setText(QString());
        label_39->setText(QApplication::translate("ConfDockWidget", "Cluster tolerance", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CGWflatclustolLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Pixels above this relative contrast in the difference image will be masked. Suggested value: 0.1", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CGWflatclustolLineEdit->setText(QString());
#ifndef QT_NO_STATUSTIP
        CGWbackclustolLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Pixels above this relative contrast in the difference image will be masked. Suggested value: 0.1", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CGWbackclustolLineEdit->setText(QString());
        conftitleIndividualweightLabel->setText(QApplication::translate("ConfDockWidget", "Individual weight maps", Q_NULLPTR));
        confsubtitleIndividualweight1Label->setText(QApplication::translate("ConfDockWidget", "(1) Dynamic range thresholding", Q_NULLPTR));
        confIndWMinLabel->setText(QApplication::translate("ConfDockWidget", "Image min value [e-]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CIWminaduLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Pixels below this value will be masked", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confIndWMaxLabel->setText(QApplication::translate("ConfDockWidget", "Image max value [e-]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CIWmaxaduLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Pixels above this value will be masked", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confsubtitleIndividualweight2Label->setText(QApplication::translate("ConfDockWidget", "(2) Cosmetics", Q_NULLPTR));
        confIndWBloomMarginLabel_3->setText(QApplication::translate("ConfDockWidget", "Trigger level [e-]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CIWbloomRangeLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The saturation value of blooming spikes may vary by this much across the CCD", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CIWbloomRangeLineEdit->setText(QString());
        confIndWBloomMarginLabel->setText(QApplication::translate("ConfDockWidget", "Range [e-]", Q_NULLPTR));
        confIndWBloomMinLabel->setText(QApplication::translate("ConfDockWidget", "Min value [e-]", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        CIWaggressivenessLineEdit->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        CIWaggressivenessLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Values larger / smaller than 1.0 will mask more / less cosmics (SLOW). If zero or empty, no cosmics will be detected.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CIWaggressivenessLineEdit->setText(QApplication::translate("ConfDockWidget", "1.0", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CIWbloomMinaduLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The min value to cut-off the background (10 sigma above mean background if empty)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CIWbloomMinaduLineEdit->setInputMask(QString());
        confIndWBloomMarginLabel_2->setText(QApplication::translate("ConfDockWidget", "Time scale [ min]", Q_NULLPTR));
        confIndWThresholdLabel->setText(QApplication::translate("ConfDockWidget", "Aggressiveness", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CIWmaskbloomingCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Check this if blooming spikes should be masked.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CIWmaskbloomingCheckBox->setText(QApplication::translate("ConfDockWidget", "Mask blooming spikes", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CIWpersistenceMinaduLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Minimum pixel level that triggers persistence.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CIWpersistenceMinaduLineEdit->setText(QString());
        confIndWBloomMinLabel_2->setText(QApplication::translate("ConfDockWidget", "Cosmics and spurious pixels:", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CIWpersistenceTimescaleLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Images taken this many minutes before the current image will be considered.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CIWpersistenceTimescaleLineEdit->setText(QString());
#ifndef QT_NO_STATUSTIP
        CIWpersistenceCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Check this to mask pixels which suffer from image persistence", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CIWpersistenceCheckBox->setText(QApplication::translate("ConfDockWidget", "Mask persistent pixels", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CIWmasksaturationCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Masks saturated pixels.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CIWmasksaturationCheckBox->setText(QApplication::translate("ConfDockWidget", "Mask saturated pixels", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CIWsaturationLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Tolerance. A vakue of 0.9 would mask pixels with values 0.9 or more of the saturation estimate.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        conftitleSeparateLabel->setText(QApplication::translate("ConfDockWidget", "Separate target groups", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        separationLabel->setStatusTip(QApplication::translate("ConfDockWidget", "Images with a linear pixel overlap larger than this value belong to the same target.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        separationLabel->setText(QApplication::translate("ConfDockWidget", "Separation [ ' ]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        separateTargetLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Image associations separated by at least this many arcminutes will form separate groups.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        separateTargetLineEdit->setInputMask(QString());
        conftitleAbsphotindirect1Label->setText(QApplication::translate("ConfDockWidget", "Absolute photometry (indirect)", Q_NULLPTR));
        confsubtitleAbsphotindirect1Label->setText(QApplication::translate("ConfDockWidget", "(1) General settings", Q_NULLPTR));
        label_41->setText(QApplication::translate("ConfDockWidget", "STD CATALOG", Q_NULLPTR));
        APIrefcatComboBox->clear();
        APIrefcatComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "LANDOLT_UBVRI", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "STETSON_UBVRI", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "STRIPE82_ugriz", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "STRIPE82_u'g'r'i'z'", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "SMITH_u'g'r'i'z'", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "WASHINGTON", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "MKO_JHK", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "HUNT_JHK", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "2MASSfaint_JHK", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "PERSSON_JHKKs", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "JAC_YJHKLM", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "MKO_LM", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        APIrefcatComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "The reference catalog. Must contain your standard stars.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confAPILabel_3->setText(QApplication::translate("ConfDockWidget", "STD FILTER", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        APIfilterComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "The reference filter you want to use to calibrate your images", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confAPILabel_13->setText(QApplication::translate("ConfDockWidget", "STD COLOR", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        APIcolorComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "The reference color index you want.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confAPILabel_11->setText(QApplication::translate("ConfDockWidget", "Calibration mode", Q_NULLPTR));
        APIcalibrationmodeComboBox->clear();
        APIcalibrationmodeComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "NIGHTCALIB", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "RUNCALIB", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        APIcalibrationmodeComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "Calculate one solution per night (NIGHTCALIB) or for all data (RUNCALIB)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confAPILabel->setText(QApplication::translate("ConfDockWidget", "Aperture diameter [ pixel ]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        APIapertureLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The aperture diameter for the photometric measurement in your data.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confAPILabel_2->setText(QApplication::translate("ConfDockWidget", "Your FILTER keyword", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        APIfilterkeywordLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The filter keyword of your observations (just for the plots)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confsubtitleAbsphotindirect2Label->setText(QApplication::translate("ConfDockWidget", "(2) Fit parameters", Q_NULLPTR));
        confAPILabel_4->setText(QApplication::translate("ConfDockWidget", "EXTINCTION (fixed)", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        APIcolortermLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "A fixed value for the color term of your instrument", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        APIextinctionLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "A fixed value for the extinction (must be negative)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confAPILabel_5->setText(QApplication::translate("ConfDockWidget", "COLOR TERM (fixed)", Q_NULLPTR));
        confAPILabel_7->setText(QApplication::translate("ConfDockWidget", "&ZP min [ mag ]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        APIminmagLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Stars brighter than this magnitude in the reference catalog will be excluded from the fit.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        APIkappaLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The sigma threshold used to reject sources during the fit.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confAPILabel_15->setText(QApplication::translate("ConfDockWidget", "Non-phot. cut-off [ mag ]", Q_NULLPTR));
        confAPILabel_10->setText(QApplication::translate("ConfDockWidget", "Con&vergence criterion", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        APIthresholdLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Sources this many mag below the fit are rejected (suggest: 0.15)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        APIzpminLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Min value of the zeropoint (to reject clearly false data points)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confAPILabel_14->setText(QApplication::translate("ConfDockWidget", "ZP max [ mag ]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        APIzpmaxLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Max value of the zeropoint (to reject clearly false data points)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confAPILabel_6->setText(QApplication::translate("ConfDockWidget", "Threshold [ sigma ]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        APIconvergenceLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "ZP, extinction and color term change by less than this amount in one iteration step (suggest: 0.01)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confAPILabel_8->setText(QApplication::translate("ConfDockWidget", "Min mag for STD catalog", Q_NULLPTR));
        conftitleAbsphotindirect2Label->setText(QString());
        confsubtitleAbsphotindirect3Label->setText(QApplication::translate("ConfDockWidget", "(3) Standard star astrometry", Q_NULLPTR));
        confAPILabel_12->setText(QApplication::translate("ConfDockWidget", "WCS uncertainty [\"]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        APIWCSLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Match observation and reference within this many arcsec if no astrometric solution is performed.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        APIWCSCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Find astrometric solution of standard star data. ", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        APIWCSCheckBox->setText(QApplication::translate("ConfDockWidget", "DO ASTROMETRY", Q_NULLPTR));
        APIreadmePushButton->setText(QApplication::translate("ConfDockWidget", "?", Q_NULLPTR));
        confsubtitleAbsphotindirect4Label->setText(QApplication::translate("ConfDockWidget", "(4) Select photometric solution(s)", Q_NULLPTR));
        APInight7PushButton->setText(QApplication::translate("ConfDockWidget", "PushButton", Q_NULLPTR));
        APInight1PushButton->setText(QApplication::translate("ConfDockWidget", "PushButton", Q_NULLPTR));
        APInight1ComboBox->clear();
        APInight1ComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "Reject night", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 1 ", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 2", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 3", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 4", Q_NULLPTR)
        );
        APInight2PushButton->setText(QApplication::translate("ConfDockWidget", "PushButton", Q_NULLPTR));
        APInight2ComboBox->clear();
        APInight2ComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "Reject night", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 1 ", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 2", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 3", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 4", Q_NULLPTR)
        );
        APInight3PushButton->setText(QApplication::translate("ConfDockWidget", "PushButton", Q_NULLPTR));
        APInight3ComboBox->clear();
        APInight3ComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "Reject night", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 1 ", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 2", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 3", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 4", Q_NULLPTR)
        );
        APInight4PushButton->setText(QApplication::translate("ConfDockWidget", "PushButton", Q_NULLPTR));
        APInight4ComboBox->clear();
        APInight4ComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "Reject night", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 1 ", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 2", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 3", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 4", Q_NULLPTR)
        );
        APInight5PushButton->setText(QApplication::translate("ConfDockWidget", "PushButton", Q_NULLPTR));
        APInight5ComboBox->clear();
        APInight5ComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "Reject night", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 1 ", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 2", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 3", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 4", Q_NULLPTR)
        );
        APInight6PushButton->setText(QApplication::translate("ConfDockWidget", "PushButton", Q_NULLPTR));
        APInight6ComboBox->clear();
        APInight6ComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "Reject night", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 1 ", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 2", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 3", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 4", Q_NULLPTR)
        );
        APInight7ComboBox->clear();
        APInight7ComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "Reject night", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 1 ", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 2", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 3", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 4", Q_NULLPTR)
        );
        APInight8PushButton->setText(QApplication::translate("ConfDockWidget", "PushButton", Q_NULLPTR));
        APInight8ComboBox->clear();
        APInight8ComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "Reject night", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 1 ", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 2", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 3", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 4", Q_NULLPTR)
        );
        APInight9PushButton->setText(QApplication::translate("ConfDockWidget", "PushButton", Q_NULLPTR));
        APInight9ComboBox->clear();
        APInight9ComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "Reject night", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 1 ", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 2", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 3", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Use fit # 4", Q_NULLPTR)
        );
        APInodataLabel->setText(QApplication::translate("ConfDockWidget", "No photometric solutions available.", Q_NULLPTR));
        conftitleCreatesourcecatLabel->setText(QApplication::translate("ConfDockWidget", "Source catalogs", Q_NULLPTR));
        confsubtitleCreatesourcecat1Label->setText(QApplication::translate("ConfDockWidget", "(1) Detection", Q_NULLPTR));
        label_20->setText(QApplication::translate("ConfDockWidget", "Detection with", Q_NULLPTR));
        CSCMethodComboBox->clear();
        CSCMethodComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "THELI", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Source Extractor", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        CSCMethodComboBox->setToolTip(QApplication::translate("ConfDockWidget", "THELI internal detection method or Source Extractor", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        confSourcecatDTLabel->setText(QApplication::translate("ConfDockWidget", "DT", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CSCDTLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Detection threshold per pixel", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confSourcecatDMINLabel->setText(QApplication::translate("ConfDockWidget", "DMIN", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CSCDMINLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Minimum number of connected pixels above the detection threshold", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confSourcecatDeblendLabel->setText(QApplication::translate("ConfDockWidget", "DEBLEND", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CSCmincontLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Deblending contrast.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confSourcecatFWHMLabel->setText(QApplication::translate("ConfDockWidget", "Min FWHM [pix]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CSCFWHMLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Min FWHM (pixel) for a source to be kept", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        CSCconvolutionCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Turn off if the field is highly crowded, or if the PSF is undersampled AND lots of hot pixels are present.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CSCconvolutionCheckBox->setText(QApplication::translate("ConfDockWidget", "Convolution", Q_NULLPTR));
        confsubtitleCreatesourcecat2Label->setText(QApplication::translate("ConfDockWidget", "(2) Filtering", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        confSourcecatSaturationLabel->setStatusTip(QApplication::translate("ConfDockWidget", "The image value above which saturation occurs", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confSourcecatSaturationLabel->setText(QApplication::translate("ConfDockWidget", "Saturation value [e-]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CSCmaxflagLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Max FLAG of a source to be kept", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        CSCbackgroundLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Manually fixed background level (in case the automatic value is very wrong)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confSourcecatBackgroundLabel->setText(QApplication::translate("ConfDockWidget", "Background level", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CSCsaturationLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Saturation (or non-linearity, as you wish) starts above this level", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confSourcecatFLAGLabel->setText(QApplication::translate("ConfDockWidget", "Max FLAG", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CSCsamplingCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Check if image is well-sampled  but has lots of hot pixels. Also consider turning off SExtractor filtering.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        CSCsamplingCheckBox->setText(QApplication::translate("ConfDockWidget", "Many hot pixels", Q_NULLPTR));
        confsubtitleCreatesourcecat3Label->setText(QApplication::translate("ConfDockWidget", "(3) Exposure rejection", Q_NULLPTR));
        confSourcecatFLAGLabel_2->setText(QApplication::translate("ConfDockWidget", "Min # of sources in any detector for good exposure", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        CSCrejectExposureLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Deactivate exposure if any chip has less than this many sources (e.g. due to clouds)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        conftitleGetCatalogLabel->setText(QApplication::translate("ConfDockWidget", "Astrometric reference catalog", Q_NULLPTR));
        confsubtitleGetCatalog1Label->setText(QApplication::translate("ConfDockWidget", "(1) Source", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ARCwebRadioButton->setStatusTip(QApplication::translate("ConfDockWidget", "Retrieve the reference catalog from the Web", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ARCwebRadioButton->setText(QApplication::translate("ConfDockWidget", "Catalog from WEB", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ARCimageRadioButton->setStatusTip(QApplication::translate("ConfDockWidget", "Retrieve the reference catalog from an astrometrically calibrated image", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ARCimageRadioButton->setText(QApplication::translate("ConfDockWidget", "Catalog from IMAGE", Q_NULLPTR));
        confsubtitleGetCatalog2Label->setText(QApplication::translate("ConfDockWidget", "(2) Settings", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ARCgetcatalogPushButton->setStatusTip(QApplication::translate("ConfDockWidget", "Retrieve the astrometric reference catalog", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ARCgetcatalogPushButton->setText(QApplication::translate("ConfDockWidget", "Get catalog", Q_NULLPTR));
        ARCserverLabel_2->setText(QApplication::translate("ConfDockWidget", "Catalog", Q_NULLPTR));
        ARCgaiaLabel->setText(QApplication::translate("ConfDockWidget", "GAIA proper motions  [ mas/yr ]", Q_NULLPTR));
        ARCmaxpmLabel->setText(QApplication::translate("ConfDockWidget", "Max allowed pm", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        ARCmaxpmLineEdit->setToolTip(QApplication::translate("ConfDockWidget", "GAIA proper motions are corrected to the median epoch of the observations. Hence this parameter can stay empty. However, to measure proper motions across epochs it might be necessary to restrict proper motions in the reference catalog, OR you correct for the bulk proper motions for your field listed below.<br>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        ARCmaxpmLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The maximum proper motion allowed for a reference star", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ARCpmRALabel->setText(QApplication::translate("ConfDockWidget", "Bulk pm in RA", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        ARCpmRALineEdit->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        ARCpmRALineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The median proper motion in RA along this line of sight  (before filtering for max allowed pm)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ARCpmRALineEdit->setText(QString());
        ARCpmDECLabel->setText(QApplication::translate("ConfDockWidget", "Bulk pm in DEC", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        ARCpmDECLineEdit->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        ARCpmDECLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The median proper motion in DEC along this line of sight (before filtering for max allowed pm)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ARCraLabel->setText(QApplication::translate("ConfDockWidget", "R.A.", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ARCraLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Right ascension of your field (only if absent in FITS header)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ARCraLineEdit->setText(QString());
        ARCdecLabel->setText(QApplication::translate("ConfDockWidget", "DEC", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ARCdecLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Declination of your field (only if absent in FITS header)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ARCtargetresolverLabel->setText(QApplication::translate("ConfDockWidget", "Target", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ARCtargetresolverToolButton->setStatusTip(QApplication::translate("ConfDockWidget", "Lookup the sky coordinates for the given target name (e.g. NGC 1234; only if absent in FITS header)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ARCtargetresolverToolButton->setText(QApplication::translate("ConfDockWidget", "...", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ARCtargetresolverLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Use target lookup to retrieve sky coordinates (e.g. NGC 1234; only if absent in FITS header)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ARCtargetresolverLineEdit->setText(QString());
        ARCminmagLabel->setText(QApplication::translate("ConfDockWidget", "Mag limit", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ARCminmagLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Faint magnitude limit of the reference catalog. No limit is applied if left empty.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ARCminmagLineEdit->setText(QString());
        ARCradiusLabel->setText(QApplication::translate("ConfDockWidget", "Radius [ ' ]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ARCradiusLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Search radius for the reference catalog (determined automatically if left empty)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ARCcatalogComboBox->clear();
        ARCcatalogComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "GAIA-DR2", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "PANSTARRS-DR1", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "SDSS-DR12 ", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "2MASS", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "UCAC5", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "ASCC", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "TYC", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        ARCcatalogComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "The astrometric reference catalog", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confSourcecatDTLabel_3->setText(QApplication::translate("ConfDockWidget", "DT", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ARCDMINLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "SExtractor minimum number of connected pixels above the detection threshold", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        ARCselectimageLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The full absolute path to an astrometrically calibrated reference image", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confSourcecatDTLabel_4->setText(QApplication::translate("ConfDockWidget", "DMIN", Q_NULLPTR));
        ARCselectimagePushButton->setText(QApplication::translate("ConfDockWidget", "Select image with WCS [...]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ARCDTLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "SExtractor detection threshold per pixel", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        conftitleAstromphotom1Label->setText(QApplication::translate("ConfDockWidget", "Astrometry / relative photometry", Q_NULLPTR));
        confsubtitleAstromphotom1Label->setText(QApplication::translate("ConfDockWidget", "(1) Method", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ASTxcorrDTLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Detection threshold per pixel", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ASTxcorrDMINLabel->setText(QApplication::translate("ConfDockWidget", "DMIN", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ASTxcorrDMINLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Minimum number of connected pixels above the detection threshold", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ASTxcorrDTLabel->setText(QApplication::translate("ConfDockWidget", "DT", Q_NULLPTR));
        ASTmethodComboBox->clear();
        ASTmethodComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "Scamp", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Cross-correlation", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Header", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        ASTmethodComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "Choose the astrometric solver", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        restoreHeaderPushButton->setStatusTip(QApplication::translate("ConfDockWidget", "FITS headers are updated with the 0th order WCS solution. This button restores the original header if necessary.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        restoreHeaderPushButton->setText(QApplication::translate("ConfDockWidget", "Restore header", Q_NULLPTR));
        confsubtitleAstromphotom2Label->setText(QApplication::translate("ConfDockWidget", "(2) Matching", Q_NULLPTR));
        ASTLabel_16->setText(QApplication::translate("ConfDockWidget", "Method", Q_NULLPTR));
        ASTmatchMethodComboBox->clear();
        ASTmatchMethodComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "Scamp", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Astrometry.net", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Do not match", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        ASTmatchMethodComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "How the matching with the reference catalog is done.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ASTposangleLabel->setText(QApplication::translate("ConfDockWidget", "POSANGLE_MAXERR", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ASTposangleLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Upper limit of the position angle uncertainty", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ASTpositionLabel->setText(QApplication::translate("ConfDockWidget", "POSITION_MAXERR [ ' ]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ASTpositionLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Upper limit of the sky position uncertainty", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ASTLabel_3->setText(QApplication::translate("ConfDockWidget", "PIXSCALE_MAXERR", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ASTpixscaleLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Upper limit of the plate scale uncertainty", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ASTLabel_6->setText(QApplication::translate("ConfDockWidget", "CROSSID_RADIUS [ \" ]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ASTcrossidLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Sources are matched with the reference catalog within this radius after matching has finished.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        ASTmatchflippedCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Allow for mirrored images when doing the matching.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ASTmatchflippedCheckBox->setText(QApplication::translate("ConfDockWidget", "Match flipped images", Q_NULLPTR));
        confsubtitleAstromphotom3Label->setText(QApplication::translate("ConfDockWidget", "(3) Solution context", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ASTastrefweightLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Relative weight of reference-source pairs compared to source-source pairs in the solution.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ASTastrefweightLineEdit->setText(QString());
        ASTLabel_10->setText(QApplication::translate("ConfDockWidget", "ASTR_INSTRUKEY", Q_NULLPTR));
        ASTLabel_15->setText(QApplication::translate("ConfDockWidget", "ASTREF_WEIGHT", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ASTphotinstrukeyLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Separate photometric solutions for images having the same FITS keyword(s) value. Leave empty for a single solution.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        ASTsnthreshLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "S/N thresholds for the statistics. Two numbers, comma-separated", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ASTsnthreshLineEdit->setText(QString());
        ASTLabel_7->setText(QApplication::translate("ConfDockWidget", "SN_THRESHOLDS", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ASTastrinstrukeyLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Separate astrometric solutions for images having the same FITS keyword(s) value. Leave empty for a single solution.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ASTLabel_14->setText(QApplication::translate("ConfDockWidget", "PHOT_INSTRUKEY", Q_NULLPTR));
        conftitleAstromphotom2Label->setText(QString());
        confsubtitleAstromphotom4Label->setText(QApplication::translate("ConfDockWidget", "(4) Distortion", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ASTdistortkeysLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "SExtractor keywords and FITS keywords. The latter must be preceded by a colon, e.g. \":AIRMASS\"", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        ASTdistortLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Polynomial distortion degree(s) (comma-separated)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ASTLabel_8->setText(QApplication::translate("ConfDockWidget", "DISTORT_GROUPS", Q_NULLPTR));
        ASTLabel_9->setText(QApplication::translate("ConfDockWidget", "DISTORT_KEYS", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ASTdistortgroupsLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The number of distortion groups", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ASTLabel_4->setText(QApplication::translate("ConfDockWidget", "DISTORT_DEGREES", Q_NULLPTR));
        confsubtitleAstromphotom5Label->setText(QApplication::translate("ConfDockWidget", "(5) Focal plane array handling", Q_NULLPTR));
        ASTLabel_11->setText(QApplication::translate("ConfDockWidget", "STABILITY_TYPE", Q_NULLPTR));
        ASTstabilityComboBox->clear();
        ASTstabilityComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "INSTRUMENT", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "EXPOSURE", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        ASTstabilityComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "Stability of the optical distortion", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ASTLabel_12->setText(QApplication::translate("ConfDockWidget", "MOSAIC_TYPE", Q_NULLPTR));
        ASTmosaictypeComboBox->clear();
        ASTmosaictypeComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "UNCHANGED", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "SAME_CRVAL", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "SHARE_PROJAXIS", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "FIX_FOCALPLANE", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "LOOSE", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        ASTmosaictypeComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "Defines how multi-chip cameras are handled. ", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ASTLabel_13->setText(QApplication::translate("ConfDockWidget", "FPA mode", Q_NULLPTR));
        ASTfocalplaneComboBox->clear();
        ASTfocalplaneComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "IGNORE_PRIOR", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "USE_PRIOR", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "NEW", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        ASTfocalplaneComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "Whether to use or recreate prior information of the relative chip positioning", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ASTreadmepushButton->setText(QApplication::translate("ConfDockWidget", "?", Q_NULLPTR));
        confsubtitleAstromphotom6Label->setText(QApplication::translate("ConfDockWidget", "(6) Check plots", Q_NULLPTR));
        ASTLabel_5->setText(QApplication::translate("ConfDockWidget", "Resolution", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        ASTresolutionLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The resolution of the scamp check plots", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ASTresolutionLineEdit->setText(QString());
#ifndef QT_NO_STATUSTIP
        ASTviewCheckPlotsPushButton->setStatusTip(QApplication::translate("ConfDockWidget", "Displays the astrometry check plots (if existing)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        ASTviewCheckPlotsPushButton->setText(QApplication::translate("ConfDockWidget", "View", Q_NULLPTR));
        conftitleStarflatLabel->setText(QApplication::translate("ConfDockWidget", "Star flat", Q_NULLPTR));
        confAPDRefcatLabel->setText(QApplication::translate("ConfDockWidget", "CATALOG", Q_NULLPTR));
        APDrefcatComboBox->clear();
        APDrefcatComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "SDSS", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "PANSTARRS", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "2MASS", Q_NULLPTR)
        );
        confAPDFilterLabel->setText(QApplication::translate("ConfDockWidget", "FILTER", Q_NULLPTR));
        confAPDMaxphoterrorLabel->setText(QApplication::translate("ConfDockWidget", "Error [ mag ]", Q_NULLPTR));
        conftitleSkysubLabel->setText(QApplication::translate("ConfDockWidget", "Sky subtraction", Q_NULLPTR));
        confsubtitleSkysub1Label->setText(QApplication::translate("ConfDockWidget", "(1) Method", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        skyModelRadioButton->setStatusTip(QApplication::translate("ConfDockWidget", "The sky is modeled after object masking", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        skyModelRadioButton->setText(QApplication::translate("ConfDockWidget", "&Model the sky", Q_NULLPTR));
        skyReadmePushButton->setText(QApplication::translate("ConfDockWidget", "?", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        skyPolynomialRadioButton->setStatusTip(QApplication::translate("ConfDockWidget", "Fit a polynomial to empty sky areas selected by the user.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        skyPolynomialRadioButton->setText(QApplication::translate("ConfDockWidget", "Polynomial fit with degree", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        skyConstsubRadioButton->setStatusTip(QApplication::translate("ConfDockWidget", "Subtracts a constant sky. Under (3) you must select either specific sky areas or entire chips.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        skyConstsubRadioButton->setText(QApplication::translate("ConfDockWidget", "S&ubstract a constant sky", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        skySavemodelCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Saves the sky models in a SKY_IMAGES/ subdirectory.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        skySavemodelCheckBox->setText(QApplication::translate("ConfDockWidget", "Save sky model in SKY_IMAGES/", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        skyPolynomialSpinBox->setStatusTip(QApplication::translate("ConfDockWidget", "The degree of the polynomial fit", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confsubtitleSkysub2Label->setText(QApplication::translate("ConfDockWidget", "(2) Object masking", Q_NULLPTR));
        confCollapseDMINLabel_2->setText(QApplication::translate("ConfDockWidget", "DMIN", Q_NULLPTR));
        confCollapseDTLabel_2->setText(QApplication::translate("ConfDockWidget", "DT", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        skyDTLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Detection threshold per pixel", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        skyMefLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Increases the default object mask by this factor", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        skyKernelLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The width of the Gaussian smoothing kernel when modeling the sky", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        skyKernelLineEdit->setInputMask(QString());
        confCollapseDMINLabel_3->setText(QApplication::translate("ConfDockWidget", "Kernel width [ pixel ]", Q_NULLPTR));
        confCollapseMEFLabel_2->setText(QApplication::translate("ConfDockWidget", "Mask e&xpansion factor", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        skyDMINLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Minimum number of connected pixels above the detection threshold", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        skyDMINLineEdit->setInputMask(QString());
        confsubtitleSkysub3Label->setText(QApplication::translate("ConfDockWidget", "(3) Empty sky area(s)", Q_NULLPTR));
        label_17->setText(QApplication::translate("ConfDockWidget", "Choose ", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        skyAreaComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "The empty sky areas from which background estimates should be obtained", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_TOOLTIP
        skyAreaPushButton->setToolTip(QApplication::translate("ConfDockWidget", "Using middle-click drag, select area(s) of empty sky regions in an astrometrically calibrated image.", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        skyAreaPushButton->setStatusTip(QApplication::translate("ConfDockWidget", "Launches iView. Select empty sky areas using the middle mouse button.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        skyAreaPushButton->setText(QApplication::translate("ConfDockWidget", "Select sky area(s)", Q_NULLPTR));
        conftitleCoadd1Label->setText(QApplication::translate("ConfDockWidget", "Coaddition", Q_NULLPTR));
        confsubtitleCoadd1Label->setText(QApplication::translate("ConfDockWidget", "(1) Reference coordinates", Q_NULLPTR));
        COAtype1Label->setText(QApplication::translate("ConfDockWidget", "R.A.", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COAraLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Reference RA (should be identical if stacks are created for different filters)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        COAtype2Label->setText(QApplication::translate("ConfDockWidget", "DEC", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COAdecLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Reference DEC (should be identical if stacks are created for different filters)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confsubtitleCoadd2Label->setText(QApplication::translate("ConfDockWidget", "(2) Resampling, stacking, zeropoint", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COApixscaleLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The plate scale to which the images should be resampled", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        COApixscaleLineEdit->setText(QString());
#ifndef QT_NO_STATUSTIP
        COAskypaLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The sky position angle. North Up and East left for 0 degrees (or empty)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        COCkernelLabel->setText(QApplication::translate("ConfDockWidget", "Combine method", Q_NULLPTR));
        COAcombinetypeComboBox->clear();
        COAcombinetypeComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "WEIGHTED", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "MEDIAN", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "AVERAGE", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "MIN", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "MAX", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "CHI-MEAN", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "CHI-MODE", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "CHI-OLD", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "SUM", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "WEIGHTED_WEIGHT", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "MEDIAN_WEIGHT", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        COAcombinetypeComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "The method by which the resampled images should be combined", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        COCkernelLabel_2->setText(QApplication::translate("ConfDockWidget", "Resampling kernel", Q_NULLPTR));
        COAkernelComboBox->clear();
        COAkernelComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "NEAREST", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "BILINEAR", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "LANCZOS2", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "LANCZOS3", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "LANCZOS4", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        COAkernelComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "Resampling kernel. Use Lanczos3 for properly sampled images, and Lanczos2 for undersampled data.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        COAprojectionComboBox->clear();
        COAprojectionComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "TAN", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "Azp", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "STG", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "SIN", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "ARC", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "zpN", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "ZEA", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "AIR", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "CYP", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "CEA", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "CAR", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "MER", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "COP", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "COE", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "COD", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "COO", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "BON", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "PCO", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "GLS", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "PAR", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "MOL", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "AIT", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "TSC", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "CSC", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "QSC", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "NONE", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        COAprojectionComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "The sky projection type.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        COAcelestialtypeComboBox->clear();
        COAcelestialtypeComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "EQUATORIAL", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "GALACTIC", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "ECLIPTIC", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "SUPERGALACTIC", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "PIXEL", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        COAcelestialtypeComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "The celestial type.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        COArescaleweightsCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Switch on for empty fields, only. Large objects will bias the internal rms estimate.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        COArescaleweightsCheckBox->setText(QApplication::translate("ConfDockWidget", "Rescale WEIGHTS", Q_NULLPTR));
        label_53->setText(QApplication::translate("ConfDockWidget", "Plate scale [ \" / pixel ]", Q_NULLPTR));
        COCkernelLabel_3->setText(QApplication::translate("ConfDockWidget", "Sky projection", Q_NULLPTR));
        COCkernelLabel_4->setText(QApplication::translate("ConfDockWidget", "Celestial type", Q_NULLPTR));
        COCkernelLabel_5->setText(QApplication::translate("ConfDockWidget", "PA [ deg ]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COAskypaPushButton->setStatusTip(QApplication::translate("ConfDockWidget", "Set the sky position angle from the images", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        COAskypaPushButton->setText(QApplication::translate("ConfDockWidget", "From image", Q_NULLPTR));
        confsubtitleCoadd3Label->setText(QApplication::translate("ConfDockWidget", "(3) Outlier rejection", Q_NULLPTR));
        label_56->setText(QApplication::translate("ConfDockWidget", "Threshold per pixel [ sigma ]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COAoutthreshLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "The sigma threshold for an individual pixel to be considered an outlier.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_58->setText(QApplication::translate("ConfDockWidget", "# of connected bad pixels", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COAoutsizeLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Minimum number of connected pixels above the threshold required for an outlier", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_57->setText(QApplication::translate("ConfDockWidget", "Additional border  [ pixel ]", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COAoutborderLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Add a border of this width around clusters of bad pixels", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        conftitleCoadd2Label->setText(QString());
        confsubtitleCoadd4Label->setText(QApplication::translate("ConfDockWidget", "(4) Setup", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COAuniqueidLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "An identifier for the current coaddition", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        COAchipsLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Coadd these chips, only (blank- or comma-separated list)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_54->setText(QApplication::translate("ConfDockWidget", "Edge smoothing", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COAedgesmoothingLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Edge smoothing length in pixels. Should correspond to the max dither step size in pixels.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        COAsizexLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Image geometry (NAXIS1) of the coadded image", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_64->setText(QApplication::translate("ConfDockWidget", "NAXIS1", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COAsizeyLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Image geometry (NAXIS2) of the coadded image", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_65->setText(QApplication::translate("ConfDockWidget", "NAXIS2", Q_NULLPTR));
        label_46->setText(QApplication::translate("ConfDockWidget", "Unique identifier", Q_NULLPTR));
        label_47->setText(QApplication::translate("ConfDockWidget", "Coadd chips", Q_NULLPTR));
        confsubtitleCoadd5Label->setText(QApplication::translate("ConfDockWidget", "(5) Non-sidereal targets", Q_NULLPTR));
        label_82->setText(QApplication::translate("ConfDockWidget", "PM units", Q_NULLPTR));
        COApmComboBox->clear();
        COApmComboBox->insertItems(0, QStringList()
         << QApplication::translate("ConfDockWidget", "[ \" / sec ]", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "[ \" / min ]", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "[ \" / hour ]", Q_NULLPTR)
         << QApplication::translate("ConfDockWidget", "[ \" / day ]", Q_NULLPTR)
        );
#ifndef QT_NO_STATUSTIP
        COApmComboBox->setStatusTip(QApplication::translate("ConfDockWidget", "Units of the proper motion vector", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_62->setText(QApplication::translate("ConfDockWidget", "PM in R.A.", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COApmraLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Proper motion vector in RA (coordinate motion). Image coaddition will be with respect to the moving target.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label_63->setText(QApplication::translate("ConfDockWidget", "PM in DEC", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COApmdecLineEdit->setStatusTip(QApplication::translate("ConfDockWidget", "Proper motion vector in DEC (coordinate motion). Image coaddition will be with respect to the moving target.", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        confsubtitleCoadd6Label->setText(QApplication::translate("ConfDockWidget", "(6) Absolute photometry", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        COAfluxcalibCheckBox->setStatusTip(QApplication::translate("ConfDockWidget", "Loads the flux calibration module after the coaddition. (Currently deactivated. Load by clicking on the top cloud symbol)", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        COAfluxcalibCheckBox->setText(QApplication::translate("ConfDockWidget", "Perform flux calibration", Q_NULLPTR));
        parametersDefaultPushButton->setText(QApplication::translate("ConfDockWidget", "Defaults for this page", Q_NULLPTR));
        confBackwardPushButton->setText(QApplication::translate("ConfDockWidget", "<<", Q_NULLPTR));
        confForwardPushButton->setText(QApplication::translate("ConfDockWidget", ">>", Q_NULLPTR));
        confPageLabel->setText(QApplication::translate("ConfDockWidget", "Page", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ConfDockWidget: public Ui_ConfDockWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFDOCKWIDGET_H
