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
    headerDictionary.insert("CRVAL1", {"CRVAL1", "RA", "OBJCTRA", "MEANRA", "OBSRA", "CRVAL1A", "RA-D", "RA_DEG", "RA-HOURS"});
    headerDictionary.insert("CRVAL2", {"CRVAL2", "DEC", "OBJCTDEC", "MEANDEC", "OBSDEC", "CRVAL2A", "DEC-D", "DEC_DEG"});
    headerDictionary.insert("RADESYS", {"RADESYS", "RADECSYS"});
    headerDictionary.insert("OBJECT", {"OBJECT", "QUEUEID", "TARGET", "TARGNAME"});
    headerDictionary.insert("AIRMASS", {"AIRMASS", "AMSTART", "HIERARCH ESO TEL AIRM START", "SECZ", "FZ_MP"});
    headerDictionary.insert("EXPTIME", {"EXPTIME", "EXPOSURE", "EXPOS", "EXPOSED", "EXP_TIME"});
    headerDictionary.insert("EQUINOX", {"EQUINOX", "EPOCH", "RADECEQ"});
    headerDictionary.insert("DATE-OBS", {"DATE-OBS", "DATEOBS", "DATE", "UTSHUT"});
    headerDictionary.insert("MJD-OBS", {"MJD-OBS"});
    headerDictionary.insert("GAIN", {"EGAIN", "GAIN", "HIERARCH ESO DET CHIP GAIN", "HIERARCH ESO DET OUT1 CONAD"});  // careful with gain and inverse gain!
    headerDictionary.insert("FILTER", {"FILTER", "FILTER1", "FILTER2", "FILTER3", "FILTER01", "FILTER02", "FILTER03", "FILTERID",
                                       "FILT1", "FILT2", "FILTERS", "FILTNAME", "SUBSET",
                                       "HIERARCH ESO INS FILT1 NAME", "HIERARCH ESO INS FILT2 NAME",
                                       "HIERARCH ESO INS FILT3 NAME", "HIERARCH ESO INS FILT4 NAME",
                                       "AFT", "ALFLTNM", "FAFLTNM", "FBFLTNM", "FILTRE", "FLTRNAME", "PFMFNAME", "WFFBAND",
                                       "ACAMFSLI", "ACAMWH1", "ACAMWH2", "INGF1NAM", "INGF2NAM", "LIRF1NAM", "LIRF2NAM"});

    // Other keywords of interest (to calculate others, or preserve information)
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
//    filterDictionary.insert(, );
}
