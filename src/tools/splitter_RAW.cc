#include "splitter.h"
#include "../instrumentdata.h"
#include "../myimage/myimage.h"
#include "../myfits/myfits.h"
#include "../functions.h"

#include "libraw/libraw.h"

#include <QString>
#include <QStringList>
#include <QVector>
#include <QFile>
#include <QDir>

// Handles RAW files from CMOS-type cameras

void Splitter::importRAW()
{
    if (!successProcessing) return;

    // The file has been opened successfully already by Splitter::determineFileType()
#define S rawProcessor.imgdata.sizes

    // TODO: check what that means. Do we have to return?
    if (!(rawProcessor.imgdata.idata.filters || rawProcessor.imgdata.idata.colors == 1)) {
        emit messageAvailable(fileName + " : Only Bayer-pattern RAW files supported.", "warning");
        emit warning();
        //   return;
    }

    // Extract pixels
    naxis1Raw = S.raw_width;
    naxis2Raw = S.raw_height;

    naxis1 = S.iwidth;
    naxis2 = S.iheight;

    bool sizeTest = checkChipGeometry();
    if (!sizeTest) {
        successProcessing = false;
        return;
    }

    long dim = naxis1Raw*naxis2Raw;
    dataRaw.clear();
    dataRaw.reserve(dim);
    for (int i=0; i<dim; ++i) {
        dataRaw.append(rawProcessor.imgdata.rawdata.raw_image[i]);
    }

    // Extract metadata
#define P1 rawProcessor.imgdata.idata
#define P2 rawProcessor.imgdata.other

    QString timeStamp = ctime(&(P2.timestamp));
    timeStamp = timeStamp.simplified();
    QStringList timeStampList = timeStamp.split(' ');
    if (timeStampList.length() < 5) {
        dateObsValue = "2020-01-01T00:00:00.0";
    }
    else {
        QString yy = timeStampList[4];
        QString mm = timeStampList[1];
        QString dd = timeStampList[2];
        QString hours = timeStampList[3];

        if (mm == "Jan") mm = "01";
        else if (mm == "Feb") mm = "02";
        else if (mm == "Mar") mm = "03";
        else if (mm == "Apr") mm = "04";
        else if (mm == "May") mm = "05";
        else if (mm == "Jun") mm = "06";
        else if (mm == "Jul") mm = "07";
        else if (mm == "Aug") mm = "08";
        else if (mm == "Sep") mm = "09";
        else if (mm == "Oct") mm = "10";
        else if (mm == "Nov") mm = "11";
        else if (mm == "Dec") mm = "12";
        dateObsValue = yy+"-"+mm+"-"+dd+"T"+hours;
        if (!checkFormatDATEOBS()) dateObsValue = "2020-01-01T00:00:00.0";
    }
    mjdobsValue = dateobsToMJD();
    exptimeValue = P2.shutter;
    sensorTemp = P2.SensorTemperature;
    cameraTemp = P2.CameraTemperature;
    isoSpeed = QString::number(int(P2.iso_speed));

    bayerPattern = "";
    if (P1.filters) {
        if (!P1.cdesc[3]) P1.cdesc[3] = 'G';
        for (int i=0; i<4; ++i) {
            bayerPattern.append(P1.cdesc[rawProcessor.fcol(i >> 1, i & 1)]);
        }
    }
}

// For RAW files we force the SIZE settings in the camera.ini
bool Splitter::checkChipGeometry()
{
    if (!successProcessing) return false;

    for (int chip=0; chip<instData->numChips; ++chip) {
        if (instData->sizex[chip] != naxis1 || instData->sizey[chip] != naxis2) {
            emit showMessageBox("Splitter::IncompatibleSizeRAW", instData->name, QString::number(naxis1)+" "+QString::number(naxis2));
            return false;
        }
    }
    return true;
}

void Splitter::buildHeaderRAW()
{
    if (!successProcessing) return;

    QStringList cards;
    cards.append("OBJECT  = 'Unknown'");
    cards.append("CTYPE1  = 'RA---TAN'");
    cards.append("CTYPE2  = 'DEC--TAN'");
    cards.append("CRVAL1  = 0.00001");
    cards.append("CRVAL2  = 0.0");
    cards.append("CRPIX1  = "+QString::number(naxis1/2));
    cards.append("CRPIX2  = "+QString::number(naxis2/2));
    cards.append("CD1_1   = "+QString::number(-1.*instData->pixscale/3600.));
    cards.append("CD1_2   = 0.0");
    cards.append("CD2_1   = 0.0");
    cards.append("CD2_2   = "+QString::number(instData->pixscale/3600.));
    cards.append("EQUINOX = 2000.0");
    cards.append("RADESYS = 'ICRS'");
    cards.append("EXPTIME = "+QString::number(exptimeValue));
    cards.append("DATE-OBS= "+dateObsValue);
    cards.append("MJD-OBS = "+QString::number(mjdobsValue, 'f', 12));
    cards.append("FILTER  = 'RGB'");
    cards.append("AIRMASS = 1.0");
    cards.append("BAYERPAT= '"+bayerPattern+"'");
    if (sensorTemp > -1000.) cards.append("DET_TEMP= "+QString::number(sensorTemp, 'f', 2));
    if (cameraTemp > -1000.) cards.append("CAM_TEMP= "+QString::number(cameraTemp, 'f', 2));
    if (!isoSpeed.isEmpty()) cards.append("ISOSPEED= "+isoSpeed);

    for (auto &card : cards) {
        card.resize(80, ' ');
    }

    headerTHELI.clear();
    headerTHELI.append(cards);
    headerTHELI.append(*dummyKeys);
}

void Splitter::extractImagesRAW()
{
    if (!successProcessing) return;

    // Keeping multi-chip code structure for consistency, even though we have just a single chip
    for (int chip=0; chip<instData->numChips; ++chip) {
        importRAW();
        overwriteCameraIniRAW();
        getDetectorSections();
        correctOverscan(combineOverscan_ptr, overscanX[chip], overscanY[chip]);
        cropDataSection(dataSection[chip]);
        correctXtalk();
        correctNonlinearity(chip);
        buildHeaderRAW();
        writeImage(chip);
#pragma omp atomic
        *progress += progressStepSize;
    }
}

void Splitter::overwriteCameraIniRAW()
{
    if (!successProcessing) return;

    // For the sake of generality I keep the multi-chip architecture.
    // RAW files can contain multiple extensions (which are not supported yet by THELI)

    // WARNING: we OVERWRITE the overscan and data section entries in camera.ini
    // Trusting that metadata in RAW files are correct

    // No overscan correction if less than 5 pixels wide
    for (int chip=0; chip<instData->numChips; ++chip) {
        // If overscan == 0 in camera.ini, the overscan_xmin etc vectors are set to empty to indicate no overscan shall be done for FITS files.
        // However, for RAW files, we do want an overscan if available.
        instData->overscan_xmin.resize(1);
        instData->overscan_xmax.resize(1);
        instData->overscan_ymin.resize(1);
        instData->overscan_ymax.resize(1);
        instData->overscan_xmin[chip] = 0;
        instData->overscan_xmax[chip] = S.left_margin < 5 ? 0 : S.left_margin - 1;
        instData->overscan_ymin[chip] = 0;
        instData->overscan_ymax[chip] = S.top_margin < 5 ? 0 : S.top_margin - 1;
        // Deactivate overscan if area is less than 5 pixels wide
        if (instData->overscan_xmin[chip] == 0 && instData->overscan_xmax[chip] == 0) {
            instData->overscan_xmin[chip] = -1;
            instData->overscan_xmax[chip] = -1;
        }
        if (instData->overscan_ymin[chip] == 0 && instData->overscan_ymax[chip] == 0) {
            instData->overscan_ymin[chip] = -1;
            instData->overscan_ymax[chip] = -1;
        }
        instData->cutx[chip] = S.left_margin;
        instData->cuty[chip] = S.top_margin;
        instData->sizex[chip] = S.iwidth;
        instData->sizey[chip] = S.iheight;
    }

    // Don't need RAW data and metadata anymore
    rawProcessor.recycle();
}
