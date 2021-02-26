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

#include <QMap>
#include <QString>
#include <QStringList>

void Controller::populateHeaderDictionary()
{
    // The mandatory THELI header keywords, and possible corresponding key names in the raw data of other instruments
    //    headerDictionary.insert("CTYPE1", {"CTYPE1"});    // Forced, hence not read from headers
    //    headerDictionary.insert("CTYPE2", {"CTYPE2"});    // Forced, hence not read from headers
    headerDictionary.insert("CRPIX1", {"CRPIX1", "J_CRPIX1"});
    headerDictionary.insert("CRPIX2", {"CRPIX2", "J_CRPIX2"});
    headerDictionary.insert("CD1_1", {"CD1_1", "J_CD1_1"});
    headerDictionary.insert("CD1_2", {"CD1_2", "J_CD1_2"});
    headerDictionary.insert("CD2_1", {"CD2_1", "J_CD2_1"});
    headerDictionary.insert("CD2_2", {"CD2_2", "J_CD2_2"});
    headerDictionary.insert("CDELT1", {"CDELT1"});
    headerDictionary.insert("CDELT2", {"CDELT2"});
    headerDictionary.insert("CRVAL1", {"CRVAL1", "RA", "OBJCTRA", "MEANRA", "OBSRA", "CRVAL1A", "RA-D", "RA_DEG", "RA-HOURS", "RASTRNG"});
    headerDictionary.insert("CRVAL2", {"CRVAL2", "DEC", "OBJCTDEC", "MEANDEC", "OBSDEC", "CRVAL2A", "DEC-D", "DEC_DEG", "DECSTRNG"});
    headerDictionary.insert("RADESYS", {"RADESYS", "RADECSYS"});
    headerDictionary.insert("OBJECT", {"OBJECT", "QUEUEID", "TARGET", "TARGNAME"});
    headerDictionary.insert("AIRMASS", {"AIRMASS", "AMSTART", "HIERARCH ESO TEL AIRM START", "SECZ", "FZ_MP"});
    headerDictionary.insert("EXPTIME", {"EXPTIME", "EXPOSURE", "EXPOS", "EXPOSED", "EXP_TIME"});
    headerDictionary.insert("EQUINOX", {"EQUINOX", "EPOCH", "RADECEQ"});
    headerDictionary.insert("DATE-OBS", {"DATE-OBS", "DATEOBS", "DATE_OBS", "DATE", "UTSHUT", "TIME"});
    headerDictionary.insert("MJD-OBS", {"MJD-OBS", "MJD_OBS"});
    headerDictionary.insert("NAMPS", {"NAMPS"});
    headerDictionary.insert("GAIN", {"EGAIN", "GAIN", "HIERARCH ESO DET CHIP GAIN", "HIERARCH ESO DET OUT1 CONAD",
                                     "HIERARCH ESO DET OUT-1 ADU"});  // careful with gain and inverse gain!
    headerDictionary.insert("FILTER", {"FILTER", "FILTER1", "FILTER2", "FILTER3", "FILTER01", "FILTER02", "FILTER03", "FILTERID",
                                       "FILT1", "FILT2", "FILTERS", "FILTNAME", "SUBSET",
                                       "HIERARCH ESO INS FILT1 NAME", "HIERARCH ESO INS FILT2 NAME",
                                       "HIERARCH ESO INS FILT3 NAME", "HIERARCH ESO INS FILT4 NAME",
                                       "HIERARCH ESO INS OPTI-2 NAME",
                                       "AFT", "ALFLTNM", "FAFLTNM", "FBFLTNM", "FILTRE", "FLTRNAME", "PFMFNAME", "WFFBAND",
                                       "ACAMFSLI", "ACAMWH1", "ACAMWH2", "INGF1NAM", "INGF2NAM", "LIRF1NAM", "LIRF2NAM"});

    // Other keywords of interest (to calculate others, or preserve information)
    headerDictionary.insert("INSTRUME", {"INSTRUME", "DETECTOR"});
    headerDictionary.insert("DATE", {"DATE", "DATE-OBS", "DATEOBS", "UT-DATE"});
    headerDictionary.insert("TIME", {"EXPSTART", "TIME", "TIME-OBS", "UT", "UT-TIME", "UTSTART", "UT-STR"});
    headerDictionary.insert("LST", {"LST", "LST-OBS", "LSTHDR", "LST_OBS", "OBS-LST", "SD_TIME", "ST"});
    headerDictionary.insert("DIT", {"EXP1TIME", "EXPCOADD", "EXPTIME", "HIERARCH ESO DET DIT", "ITIME", "K_DETEP1", "TRUITIME", "DIT"});
    headerDictionary.insert("NDIT", {"COADD", "COADDONE", "COADDS", "COAVERAG", "HIERARCH ESO DET NDIT", "NCOADD", "NDIT"});
    //    headerDictionary.insert("IMAGEID", {});
    //    headerDictionary.insert("GABODSID", {});
    //    headerDictionary.insert("ZP", {});
    //    headerDictionary.insert("COEFF", {});

    /*
     * ABOUT GAIN:
     * FourStar has a GAIN keyword, but the correct one is EGAIN"
    */

}

// Used to replace long filter names by short ones
void Controller::populateFilterDictionary()
{
    // FLAMINGOS2
    filterDictionary.insert("DK_G0807+J_G0802", "DARK");
    filterDictionary.insert("DK_G0807", "DARK");
    filterDictionary.insert("J_G0802", "J");
    filterDictionary.insert("H_G0803", "H");
    filterDictionary.insert("Ks_G0804", "Ks");
    filterDictionary.insert("Y_G0811", "Y");
    filterDictionary.insert("J-lo_G0801", "Jlo");
    filterDictionary.insert("JH_G0809", "JH");
    filterDictionary.insert("HK_G0806_good", "HK");
    // GSAOI
    filterDictionary.insert("Blocked1_G1114", "DARK");
    filterDictionary.insert("Blocked2_G1128", "DARK");
    filterDictionary.insert("Z_G1101", "Z");
    filterDictionary.insert("J_G1102", "J");
    filterDictionary.insert("H_G1103", "H");
    filterDictionary.insert("Kprime_G1104", "Kp");
    filterDictionary.insert("Kshort_G1105", "Ks");
    filterDictionary.insert("K_G1106", "K");
    filterDictionary.insert("Jcont_G1107", "Jc");
    filterDictionary.insert("Hcont_G1108", "Hc");
    filterDictionary.insert("CH4short_G1109", "CH4s");
    filterDictionary.insert("CH4long_G1110", "CH4l");
    filterDictionary.insert("Kcntshrt_G1111", "Kcs");
    filterDictionary.insert("Kcntlong_G1112", "Kcl");
    filterDictionary.insert("HeI1083_G1115", "HeI");
    filterDictionary.insert("PaG_G1116", "PaG");
    filterDictionary.insert("PaB_G1117", "PaB");
    filterDictionary.insert("FeII_G1118", "FeII");
    filterDictionary.insert("H2(1-0)_G1121", "H210");
    filterDictionary.insert("BrG_G1122", "BrG");
    filterDictionary.insert("H2(2-1)_G1123", "H221");
    filterDictionary.insert("CO2360_G1124", "CO2360");
    // NIRI
    filterDictionary.insert("H_G0203", "H");
    filterDictionary.insert("J_G0202", "J");
    filterDictionary.insert("Kshort_G0205", "Ks");
    filterDictionary.insert("Y_G0241", "Y");
    filterDictionary.insert("Lprime_G0207", "Lp");
    // MOIRCS
    filterDictionary.insert("J_SUB", "J");
    // GROND_OPT
    filterDictionary.insert("grond_g", "g");
    filterDictionary.insert("grond_r", "r");
    filterDictionary.insert("grond_i", "i");
    filterDictionary.insert("grond_z", "z");
    // MOSCA@NOT
    filterDictionary.insert("U_Bes361_62", "U");
    filterDictionary.insert("B_Bes428_108", "B");
    filterDictionary.insert("V_Bes536_89", "V");
    filterDictionary.insert("R_Bes646_124", "R");
    filterDictionary.insert("I_int817_163", "I");
    filterDictionary.insert("u_SDSS353_55", "u");
    filterDictionary.insert("g_SDSS480_145", "g");
    filterDictionary.insert("r_SDSS618_148", "r");
    filterDictionary.insert("i_SDSS771_171", "i");
    filterDictionary.insert("z_SDSS832_LP", "z");
    // WFI@MPGESO
    filterDictionary.insert("BBB99_ESO842", "B");
    filterDictionary.insert("BBV89_ESO843", "V");
    filterDictionary.insert("BBRc162_ESO844", "Rc");
    filterDictionary.insert("BBB/123_ESO878", "Bnew");
    filterDictionary.insert("BBI/203_ESO879", "I");
    filterDictionary.insert("BBIclwp_ESO845", "Ic");
    filterDictionary.insert("BBIcIwp_ESO845", "Ic");
    filterDictionary.insert("BBU38_ESO841", "U38");
    filterDictionary.insert("BBU50_ESO877", "U50");
    //IMACS
    filterDictionary.insert("Sloan_u", "u");
    filterDictionary.insert("Sloan_g", "g");
    filterDictionary.insert("Sloan_r", "r");
    filterDictionary.insert("Sloan_i", "i");
    filterDictionary.insert("Sloan_z", "z");
    // MOSAIC-III
    filterDictionary.insert("Usparek1029", "U");
    filterDictionary.insert("Uk1001", "U");
    filterDictionary.insert("BHarrisk1002", "B");
    filterDictionary.insert("VHarrisk1003", "V");
    filterDictionary.insert("RHarrisk1004", "R");
    filterDictionary.insert("INearly-Mouldk1005", "I");
    filterDictionary.insert("CWashingtonk1006", "C");
    filterDictionary.insert("MWashingtonk1007", "M");
    filterDictionary.insert("haH-alphak1009", "Ha");
    filterDictionary.insert("ha4H-alpha+4nmk1010", "Ha+4");
    filterDictionary.insert("ha8H-alpha+8nmk1011", "Ha+8");
    filterDictionary.insert("ha12H-alpha+12nmk1012", "Ha+12");
    filterDictionary.insert("ha16H-alpha+16nmk1013", "SII");
    filterDictionary.insert("O3OIIIN2k1014", "OIII");
    filterDictionary.insert("OoffOIII+30nmk1015", "OIII+30");
    filterDictionary.insert("gSDSSk1017", "g");
    filterDictionary.insert("rSDSSk1018", "r");
    filterDictionary.insert("iSDSSk1019", "i");
    filterDictionary.insert("zSDSSk1020", "z");
    filterDictionary.insert("OoffOIII+30nmk1030", "OIII+30");
    filterDictionary.insert("OoffOIII+30nmk1031", "OIII+30");
    filterDictionary.insert("gdDECcamk1035", "gd");
    filterDictionary.insert("rdDECcamk1036", "rd");
    filterDictionary.insert("zdDECcamk1038", "zd");
    filterDictionary.insert("VRk1039", "VR");
    filterDictionary.insert("UnSteidelk1041", "U");
    filterDictionary.insert("GnSteidelk1042", "B");
    filterDictionary.insert("RsSteidelk1043", "R");
    filterDictionary.insert("UssolidUk1044", "U");
    filterDictionary.insert("UdDeyk1045", "U");
    filterDictionary.insert("815815_v2k1046", "820B");
    filterDictionary.insert("823823_v2k1047", "820R");
    filterDictionary.insert("337BATCk1051", "337_BATC");
    filterDictionary.insert("390BATCk1052", "390_BATC");
    filterDictionary.insert("420BATCk1053", "420_BATC");
    filterDictionary.insert("454BATCk1054", "454_BATC");
    filterDictionary.insert("493BATCk1055", "493_BATC");
    filterDictionary.insert("527BATCk1056", "527_BATC");
    filterDictionary.insert("579BATCk1057", "579_BATC");
    filterDictionary.insert("607BATCk1058", "607_BATC");
    filterDictionary.insert("666BATCk1059", "666_BATC");
    filterDictionary.insert("705BATCk1060", "705_BATC");
    filterDictionary.insert("755BATCk1061", "755_BATC");
    filterDictionary.insert("802BATCk1062", "802_BATC");
    filterDictionary.insert("848BATCk1063", "848_BATC");
    filterDictionary.insert("918BATCk1064", "918_BATC");
    filterDictionary.insert("973BATCk1065", "973_BATC");
    // SAMI
    filterDictionary.insert("s0012rSDSS+s0012", "r");
    // MEGACAM@LCO
    filterDictionary.insert("u,gblk", "u");
    filterDictionary.insert("g,gblk", "g");
    filterDictionary.insert("r,gblk", "r");
    filterDictionary.insert("i,gblk", "i");
    filterDictionary.insert("z,gblk", "z");
    filterDictionary.insert("gblk,u", "u");
    filterDictionary.insert("gblk,g", "g");
    filterDictionary.insert("gblk,r", "r");
    filterDictionary.insert("gblk,i", "i");
    filterDictionary.insert("gblk,z", "z");
    // GMOS
    filterDictionary.insert("u_G0308", "u");
    filterDictionary.insert("u_G0332", "u");
    filterDictionary.insert("g_G0301", "g");
    filterDictionary.insert("g_G0325", "g");
    filterDictionary.insert("r_G0303", "r");
    filterDictionary.insert("r_G0326", "r");
    filterDictionary.insert("i_G0302", "i");
    filterDictionary.insert("i_G0327", "i");
    filterDictionary.insert("z_G0304", "z");
    filterDictionary.insert("z_G0328", "z");
    filterDictionary.insert("Ha_G0336", "Ha");
    filterDictionary.insert("Ha_G0310", "Ha");
    filterDictionary.insert("HaC_G0337", "HaC");
    filterDictionary.insert("HaC_G0311", "HaC");
    filterDictionary.insert("SII_G0335", "SII");
    filterDictionary.insert("SII_G0317", "SII");
    filterDictionary.insert("OIII_G0338", "OIII");
    filterDictionary.insert("OIII_G0318", "OIII");
    filterDictionary.insert("OIIIC_G0339", "OIIIC");
    filterDictionary.insert("OIIIC_G0319", "OIIIC");
    filterDictionary.insert("OVI_G0345", "OVI");
    filterDictionary.insert("OVI_G0347", "OVI");
    filterDictionary.insert("OVIC_G0346", "OVIC");
    filterDictionary.insert("OVIC_G0348", "OVIC");
    filterDictionary.insert("HeII_G0320", "HeII");
    filterDictionary.insert("HeII_G0340", "HeII");
    filterDictionary.insert("HeIIC_G0321", "HeIIC");
    filterDictionary.insert("HeIIC_G0341", "HeIIC");
    // LDSS3
    filterDictionary.insert("g_Sloan", "g");
    filterDictionary.insert("r_Sloan", "r");
    filterDictionary.insert("i_Sloan", "i");
    filterDictionary.insert("z_Sloan", "z");
//    filterDictionary.insert(, );
}

void Controller::populateInstrumentDictionary()
{
    instrumentDictionary.insert("ACAM@WHT", "ACAM");
    instrumentDictionary.insert("ACS@HST", "");
    instrumentDictionary.insert("ALFOSC@NOT", "");
    instrumentDictionary.insert("ALTAU16M@VYSOS06", "");
    instrumentDictionary.insert("AltaU42_HIGHRES@ASV", "");
    instrumentDictionary.insert("AltaU42_LOWRES@ASV", "");
    instrumentDictionary.insert("ApogeeAlta@PROMPT4", "");
    instrumentDictionary.insert("ApogeeAlta@PROMPT5", "");
    instrumentDictionary.insert("CFH12K@CFHT", "");
    instrumentDictionary.insert("CFH12K@CFHT99", "");
    instrumentDictionary.insert("DECam@CTIO", "");
    instrumentDictionary.insert("Direct_4k_SWOPE@LCO", "Direct_4Kx4K-4");     // must replace a '/' in the INSTRUME keyword with '_'
    instrumentDictionary.insert("EFOSC2@ESO3.6m", "EFOSC");
    instrumentDictionary.insert("EFOSC2@NTT", "EFOSC");
    instrumentDictionary.insert("EFOSC2_2x2@ESO3.6m", "EFOSC");
    instrumentDictionary.insert("EFOSC2_2x2@NTT", "EFOSC");
    instrumentDictionary.insert("EMIR@GTC", "");
    instrumentDictionary.insert("EMMI_BIMG@NTT", "");
    instrumentDictionary.insert("EMMI_RILD@NTT", "");
    instrumentDictionary.insert("ENZIAN_CAS@HOLI_1M", "");
    instrumentDictionary.insert("FL03_LCOGT@CTIO", "");
    instrumentDictionary.insert("FL04_LCOGT@CTIO", "");
    instrumentDictionary.insert("FLAMINGOS2@GEMINI", "F2");
    instrumentDictionary.insert("FLI-PL16801@WISE", "");
    instrumentDictionary.insert("FORS1_199904-200703@VLT", "FORS1");
    instrumentDictionary.insert("FORS1_E2V_2x2@VLT", "FORS1");
    instrumentDictionary.insert("FORS2_200004-200203@VLT", "FORS2");
    instrumentDictionary.insert("FORS2_E2V_2x2@VLT", "FORS2");
    instrumentDictionary.insert("FORS2_MIT_1x1@VLT", "FORS2");
    instrumentDictionary.insert("FORS2_MIT_2x2@VLT", "FORS2");
    instrumentDictionary.insert("FourStar@LCO", "FourStar");
    instrumentDictionary.insert("FourStar_Chips1+4@LCO", "FourStar");
    instrumentDictionary.insert("GMOS-N-EEV-3ports@GEMINI", "GMOS-N");
    instrumentDictionary.insert("GMOS-N-EEV-6ports@GEMINI", "GMOS-N");
    instrumentDictionary.insert("GMOS-N-HAM@GEMINI", "GMOS-N");
    instrumentDictionary.insert("GMOS-N-HAM_1x1@GEMINI", "GMOS-N");
    instrumentDictionary.insert("GMOS-S-EEV@GEMINI", "GMOS-S");
    instrumentDictionary.insert("GMOS-S-HAM@GEMINI", "GMOS-S");
    instrumentDictionary.insert("GMOS-S-HAM_1x1@GEMINI", "GMOS-S");
    instrumentDictionary.insert("GMOS-S_EEV_GeMS@GEMINI", "GMOS-S");
    instrumentDictionary.insert("GOODMAN_1x1@SOAR", "");
    instrumentDictionary.insert("GOODMAN_2x2@SOAR", "");
    instrumentDictionary.insert("GPC1@PS1", "");
    instrumentDictionary.insert("GROND_NIR@MPGESO", "GROND");
    instrumentDictionary.insert("GROND_OPT@MPGESO", "GROND");
    instrumentDictionary.insert("GSAOI@GEMINI", "GSAOI");
    instrumentDictionary.insert("GSAOI_1k@GEMINI", "GSAOI");
    instrumentDictionary.insert("GSAOI_CHIP1@GEMINI", "GSAOI");
    instrumentDictionary.insert("GSAOI_CHIP2@GEMINI", "GSAOI");
    instrumentDictionary.insert("GSAOI_CHIP3@GEMINI", "GSAOI");
    instrumentDictionary.insert("GSAOI_CHIP4@GEMINI", "GSAOI");
    instrumentDictionary.insert("HAWKI@VLT", "HAWKI");
    instrumentDictionary.insert("HDI@KPNO_0.9m", "hdi");
    instrumentDictionary.insert("IMACS_F2_NEW@LCO", "IMACS Short-Camera");
    instrumentDictionary.insert("IMACS_F2_OLD@LCO", "");
    instrumentDictionary.insert("IMACS_F4_NEW@LCO", "IMACS Long-Camera");
    instrumentDictionary.insert("IMACS_F4_NEW_2x2@LCO", "IMACS Long-Camera");
    instrumentDictionary.insert("IMACS_F4_OLD@LCO", "");
    instrumentDictionary.insert("INGRID@WHT", "");
    instrumentDictionary.insert("IRCS_HIGHRES@SUBARU", "");
    instrumentDictionary.insert("IRCS_LOWRES@SUBARU", "");
    instrumentDictionary.insert("ISAAC@VLT", "");
    instrumentDictionary.insert("ISPI@CTIO", "");
    instrumentDictionary.insert("KTMC@CTIO", "");
    instrumentDictionary.insert("LAICA@CAHA", "");
    instrumentDictionary.insert("LAICA_2x2@CAHA", "");
    instrumentDictionary.insert("LBC_BLUE@LBT", "");
    instrumentDictionary.insert("LBC_RED@LBT", "");
    instrumentDictionary.insert("LDSS3@LCO", "");
    instrumentDictionary.insert("LIRIS@WHT", "LIRIS");
    instrumentDictionary.insert("LIRIS_POL@WHT", "LIRIS");
    instrumentDictionary.insert("LORRI@NewHorizons", "");
    instrumentDictionary.insert("MEGACAM_2x2@LCO", "megacam");
    instrumentDictionary.insert("MEGAPRIME@CFHT", "");
    instrumentDictionary.insert("MEGAPRIME_ELIXIR@CFHT", "");
    instrumentDictionary.insert("MEROPE@MERCATOR", "");
    instrumentDictionary.insert("MMIRS@LCO", "");
    instrumentDictionary.insert("MOIRCS_200406-200806@SUBARU", "MOIRCS");
    instrumentDictionary.insert("MOIRCS_200807-201505@SUBARU", "MOIRCS");
    instrumentDictionary.insert("MOIRCS_201512-today@SUBARU", "MOIRCS");
    instrumentDictionary.insert("MOSAIC-III_4@KPNO_4m", "Mosaic3");
    instrumentDictionary.insert("MOSAIC-III@KPNO_4m", "Mosaic3");
    instrumentDictionary.insert("MOSAIC-II_15@CTIO", "Mosaic2");
    instrumentDictionary.insert("MOSAIC-II_16@CTIO", "Mosaic2");
    instrumentDictionary.insert("MOSAIC-II_16_old@CTIO", "Mosaic2");
    instrumentDictionary.insert("MOSAIC-II_8@CTIO", "Mosaic2");
    instrumentDictionary.insert("MOSAIC-II_8_2x2@CTIO", "Mosaic2");
    instrumentDictionary.insert("MOSAIC-I_old@KPNO_0.9m", "");
    instrumentDictionary.insert("MOSAIC-I_old@KPNO_4.0m", "");
    instrumentDictionary.insert("MOSCA_2x2@NOT", "MOSAIC 4*2kx2k");
    instrumentDictionary.insert("MOSFIRE@KECK", "");
    instrumentDictionary.insert("NACO@VLT", "NAOS+CONICA");
    instrumentDictionary.insert("NACOSDI@VLT", "");
    instrumentDictionary.insert("NEWFIRM@CTIO", "NEWFIRM");
    instrumentDictionary.insert("NEWFIRM@KPNO_4m", "NEWFIRM");
    instrumentDictionary.insert("NICI@GEMINI", "");
    instrumentDictionary.insert("NICS@TNG", "");
    instrumentDictionary.insert("NIRC2@KECK", "");
    instrumentDictionary.insert("NIRI@GEMINI", "NIRI");
    instrumentDictionary.insert("NOTcam_highres@NOT", "");
    instrumentDictionary.insert("NOTcam_lowres@NOT", "");
    instrumentDictionary.insert("OASIS4x4@WHT", "");
    instrumentDictionary.insert("OASIS@WHT", "");
    instrumentDictionary.insert("OMEGACAM@VST", "OMEGACAM");
    instrumentDictionary.insert("OSIRIS@GTC", "");
    instrumentDictionary.insert("OSIRIS_F3@SOAR", "");
    instrumentDictionary.insert("OSIRIS_F7@SOAR", "");
    instrumentDictionary.insert("Omega2000@CAHA", "");
    instrumentDictionary.insert("PANIC@LCO", "");
    instrumentDictionary.insert("PANIC_OLD_4det@CAHA2.2", "");
    instrumentDictionary.insert("PAUCam@WHT", "");
    instrumentDictionary.insert("PFC_new@WHT", "");
    instrumentDictionary.insert("PFC_old@WHT", "Prime Imaging");
    instrumentDictionary.insert("PICCD@WISE", "");
    instrumentDictionary.insert("PISCES@LBT", "");
    instrumentDictionary.insert("SAMI_2x2@SOAR", "SAM");
    instrumentDictionary.insert("SDSS", "");
    instrumentDictionary.insert("SITe@TLS", "");
    instrumentDictionary.insert("SOFI@NTT", "");
    instrumentDictionary.insert("SOI@SOAR", "SOI");
    instrumentDictionary.insert("SPARTAN@SOAR", "");
    instrumentDictionary.insert("SUSI1@NTT", "SUSI");
    instrumentDictionary.insert("SUSI2_2x2@NTT", "");
    instrumentDictionary.insert("SUSI2old_2x2@NTT", "");
    instrumentDictionary.insert("SWOPE@LCO", "");
    instrumentDictionary.insert("SuprimeCam_200101-200104@SUBARU", "SuprimeCam");
    instrumentDictionary.insert("SuprimeCam_200105-200807@SUBARU", "SuprimeCam");
    instrumentDictionary.insert("SuprimeCam_200808-201705@SUBARU", "SuprimeCam");
    instrumentDictionary.insert("SuprimeCam_200808-201705_SDFRED@SUBARU", "SuprimeCam");
    instrumentDictionary.insert("TRECS@GEMINI", "TReCS");
    instrumentDictionary.insert("TRIPOL_1x1@SAAO", "");
    instrumentDictionary.insert("TRIPOL_2x2@SAAO", "");
    instrumentDictionary.insert("Tek2K@CTIO", "");
    instrumentDictionary.insert("VIMOS@VLT", "VIMOS");
    instrumentDictionary.insert("VIRCAM@VISTA", "");
    instrumentDictionary.insert("VISIR@VLT", "");
    instrumentDictionary.insert("WFC@INT", "WFC");
    instrumentDictionary.insert("WFC_2x2@INT", "WFC");
    instrumentDictionary.insert("WFC_IPHAS@INT", "WFC");
    instrumentDictionary.insert("WFI@AAT", "");
    instrumentDictionary.insert("WFI@MPGESO", "WFI");
    instrumentDictionary.insert("WFI@SSO_40inch", "");
    instrumentDictionary.insert("WFI_2x2_2006@MPGESO", "");
    instrumentDictionary.insert("WFI_2x2_2017@MPGESO", "");
    instrumentDictionary.insert("WHIRC@WIYN", "WHIRC");
    instrumentDictionary.insert("WIRC@Hale", "");
    instrumentDictionary.insert("WIRCam@CFHT", "");
    instrumentDictionary.insert("Y4KCam@CTIO", "");
    // instrumentDictionary.insert();
}
