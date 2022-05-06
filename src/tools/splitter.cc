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

#include "splitter.h"
#include "../instrumentdata.h"
#include "../myimage/myimage.h"
#include "../functions.h"
#include "../processingInternal/mask.h"
#include "../processingInternal/data.h"
#include "cfitsioerrorcodes.h"
#include "../dockwidgets/confdockwidget.h"
#include "ui_confdockwidget.h"

#include "fitsio.h"
#include "libraw/libraw.h"

#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QVector>
#include <QFile>
#include <QDir>

Splitter::Splitter(const instrumentDataType &instrumentData, const Mask *detectorMask, const Mask *altDetectorMask,
                   Data *someData, QString datatype, const ConfDockWidget *confDockWidget, QString maindirname,
                   QString subdirname, QString filename, int *verbose,
                   QObject *parent) :
    QObject(parent),
    mask(detectorMask),
    altMask(altDetectorMask),
    instData(instrumentData),       // instData is modified locally during splitting, in the Splitter class only
    cdw(confDockWidget)
{
    fileName = filename;
    mainDirName = maindirname;
    subDirName = subdirname;
    path = mainDirName + "/" + subDirName;
    name = path+"/"+fileName;
    dataType = datatype;
    QFileInfo fi(name);
    baseName = fi.completeBaseName();
    verbosity = verbose;
    data = someData;

    QDir rawdata(path+"/RAWDATA");
    rawdata.mkpath(path+"/RAWDATA");

    // Bypassing a memory leak in cfitsio
    QFile file(name);
    if (!file.exists()) {
        successProcessing = false;
    }

    // The remainder of the processing is called externally from the controller
}

void Splitter::backupRawFile()
{
    if (!successProcessing) return;
    moveFile(fileName, path, path+"/RAWDATA/");
}

void Splitter::determineFileFormat()
{
    if (!successProcessing) return;

    alreadyProcessed = false;
    // Try opening as FITS
    fits_open_file(&rawFptr, name.toUtf8().data(), READONLY, &rawStatus);
    // CHECK: xtalk correction only works if we maintain the original detector geometry
    // correctXtalk();

    if (!rawStatus) {
        dataFormat = "FITS";

        // Already processed by THELI?
        long thelipro = 0;
        fits_read_key_lng(rawFptr, "THELIPRO", &thelipro, nullptr, &rawStatus);
        if (rawStatus != KEY_NO_EXIST) {
            // key exists, this file has been processed by the splitter alreay. Must skip.
            emit messageAvailable(fileName + " : Already processed, skipped.", "image");
            rawStatus = 0;
            alreadyProcessed = true;
            return;
        }
        else {
            // Key does not exist, meaning file has not been processed. reset raw status
            rawStatus = 0;
        }

        uncompress();
        consistencyChecks();
        getDetectorSections(); // Read overscan and data sections for the current instrument; resize accordingly, including gain vector
        getNumberOfAmplifiers();
    }
    else {
        // Try opening as RAW
        dataFormat = "RAW";
        int ret = rawProcessor.open_file((path+"/"+fileName).toUtf8().data());
        if (ret == LIBRAW_SUCCESS) {
            // Opened. Try unpacking
            bool unpack = rawProcessor.unpack();
            if (unpack != LIBRAW_SUCCESS) {
                emit messageAvailable(fileName + " : Could not unpack file: " + libraw_strerror(unpack), "warning");
                dataFormat = "UnknownFormat";
                return;
            }
            getNumberOfAmplifiers();
        }
        else dataFormat = "UnknownFormat";
    }

    if (dataFormat == "UnknownFormat") {
        // FITS opening error?
        //        printCfitsioError("determineFileFormat()", rawStatus);
        // successProcessing = false;    // Don not trigger an error, just skip the file

        QDir unknownFile(path+"/UnknownFormat");
        unknownFile.mkpath(path+"/UnknownFormat/");
        moveFile(fileName, path, path+"/UnknownFormat");
        emit messageAvailable("File "+fileName+" : Unknown format. Cfitsio error code: "+QString::number(rawStatus) + ". Moved to "+subDirName+"/UnknownFormat/", "warning");
    }
}

void Splitter::uncompress()
{
    if (!successProcessing) return;

    // Unpack tile-compressed image and write new fits file to disk
    if (fits_is_compressed_image(rawFptr, &rawStatus)) {
        if (*verbosity > 1) emit messageAvailable(baseName + " : Uncompressing ...", "image");
        fitsfile *outRawPtr;
        int outRawStatus = 0;
        QFileInfo fi(name);
        QString outName = "!" + path + fi.completeBaseName();
        if (!outName.contains(".fits") || !outName.contains(".fit") || !outName.contains(".fts")) outName.append(".fits");
        fits_create_file(&outRawPtr, outName.toUtf8().data(), &outRawStatus);
        fits_img_decompress(rawFptr, outRawPtr, &rawStatus);
        // delete compressed file if uncompression was successful, create new fits pointer to uncompressed file        // CHECK: xtalk correction only works if we maintain the original detector geometry
        // correctXtalk();
        // CHECK : No uncompressed FITS files appear after splitting. Where are they? The raw file does not get removed either.
        if (!rawStatus && !outRawStatus) {
            fits_close_file(rawFptr, &rawStatus);
            //QFile compressedFile(name);
            //compressedFile.remove();
            name = outName;
            name.remove('!');
            QFileInfo fi2(name);
            baseName = fi2.completeBaseName();
            fits_open_file(&rawFptr, name.toUtf8().data(), READONLY, &rawStatus);
            if (rawStatus) {
                emit messageAvailable(baseName + " : Could not open uncompressed FITS file!", "error");
                printCfitsioError(__func__, rawStatus);
            }
        }
        else {
            emit messageAvailable(fi.fileName() + " : Uncompression did not work as expected!", "error");
            printCfitsioError(__func__, rawStatus);
            printCfitsioError(__func__, outRawStatus);
            successProcessing = false;
        }
    }
}

void Splitter::consistencyChecks()
{
    if (!successProcessing) return;

    // TODO: Determine FITS type (SINGLE, MEF or CUBE)

    fits_get_num_hdus(rawFptr, &numExt, &rawStatus);
    if (fitsType == "SINGLE" || fitsType == "MEF") {
        if (numHDU < instData.numChips) {
            emit messageAvailable(fileName + " : "+QString::number(numHDU)+" HDUs found, "+QString::number(instData.numChips)
                                  +" expected.<br>File is moved to "+subDirName+"/INCONSISTENT/", "warning");
            emit warning();
            QDir inconsistentFile(path+"/INCONSISTENT");
            inconsistentFile.mkpath(path+"/INCONSISTENT");
            moveFile(name, path, path+"/INCONSISTENT");

            successProcessing = false;
        }
        // equal or more numHDUs than chips is fine (e.g. multi-channel cameras, or multiple readout ports per detector)
    }

    printCfitsioError(__func__, rawStatus);
}

void Splitter::compileNumericKeys()
{
    numericKeyNames.clear();
    numericKeyNames.append(headerDictionary.value("CRVAL1"));
    numericKeyNames.append(headerDictionary.value("CRVAL2"));
    numericKeyNames.append(headerDictionary.value("CRPIX1"));
    numericKeyNames.append(headerDictionary.value("CRPIX2"));
    numericKeyNames.append(headerDictionary.value("EXPTIME"));
    numericKeyNames.append(headerDictionary.value("CD1_1"));
    numericKeyNames.append(headerDictionary.value("CD1_2"));
    numericKeyNames.append(headerDictionary.value("CD2_1"));
    numericKeyNames.append(headerDictionary.value("CD2_2"));
    numericKeyNames.append(headerDictionary.value("CDELT1"));
    numericKeyNames.append(headerDictionary.value("CDELT2"));
    numericKeyNames.append(headerDictionary.value("AIRMASS"));
    numericKeyNames.append(headerDictionary.value("EQUINOX"));
    numericKeyNames.append(headerDictionary.value("MJD-OBS"));
    numericKeyNames.append(headerDictionary.value("GAIN"));
    numericKeyNames.append(headerDictionary.value("DATE"));
    numericKeyNames.append(headerDictionary.value("TIME"));
    numericKeyNames.append(headerDictionary.value("LST"));
}

void Splitter::extractImages()
{
    if (!successProcessing) return;
    if (alreadyProcessed) return;
    if (dataFormat == "UnknownFormat") return;

    emit messageAvailable(fileName + " : HDU reformatting, low-level pixel processing ...", "image");

    // adjust progress step size for multi-chip cameras whose detectors are stored in single extension FITS files
    // (i.e. raw data does not come as a MEF but as separate FITS files)
    QStringList instruments = {"Direct_4k_SWOPE@LCO", "FORS1_E2V_2x2@VLT", "FORS2_E2V_2x2@VLT", "FORS2_MIT_1x1@VLT", "FORS2_MIT_2x2@VLT", "FourStar@LCO",
                               "HSC@SUBARU", "LDSS3_2004-201401@LCO", "LDSS3_from201402@LCO",
                               "IMACS_F2_200404-2008@LCO", "IMACS_F2_2008-today@LCO", "IMACS_F4_200404-2012@LCO", "IMACS_F4_2012-today@LCO", "IMACS_F4_2012-today_2x2@LCO",
                               "MOIRCS_200406-201006@SUBARU", "MOIRCS_201007-201505@SUBARU", "MOIRCS_201512-today@SUBARU",
                               "SPARTAN@SOAR", "SuprimeCam_200101-200104@SUBARU", "SuprimeCam_200105-200807@SUBARU", "SuprimeCam_200808-201705@SUBARU",
                               "SuprimeCam_200808-201705_SDFRED@SUBARU", "VIMOS@VLT"};

    // multiple readout channels in different FITS extensions
    multiChannelMultiExt << "DEIMOS_2AMP@KECK"
                         << "GMOS-N-HAM@GEMINI" << "GMOS-N-HAM_1x1@GEMINI"
                         << "GMOS-S-HAM@GEMINI" << "GMOS-S-HAM_1x1@GEMINI" << "GMOS-S-HAM_4x4@GEMINI"
                         << "LRIS_BLUE@KECK" << "LRIS_RED@KECK"
                         << "MOSAIC-II_16@CTIO" << "MOSAIC-III_4@KPNO_4m"
                         << "PISCO@LCO" << "SAMI_2x2@SOAR" << "SOI@SOAR";
    if (multiChannelMultiExt.contains(instData.name)) ampInSeparateExt = true;

    if (instruments.contains(instData.name)) {
        progressStepSize *= instData.numChips;
    }

    if (dataFormat == "FITS") extractImagesFITS();
    else if (dataFormat == "RAW") extractImagesRAW();
    else {
        // nothing yet
    }

    // Lastly, store the raw file in the RAWDATA directory
    backupRawFile();
}

// SINGLE and MEF files can be treated the same way
void Splitter::extractImagesFITS()
{
    if (!successProcessing) return;

    int hduType;
    int chip = 0;  // Start counting at 0 (the output filename will have chip+1)

    // Step through HDUs, extract extensions if they are images, and process them
    // IMPORTANT: It is implied that the number of the extension, as encountered in the data,
    // is identical to the 'chip' number used throughout the rest of this source code
    readPrimaryHeader();

    // Check if the instrument matches the data
    // This does NOT catch all inconsistent matches at this point (e.g. we are not comparing detector identification strings),
    // and the INSTRUME keyword has not been determined for all pre-defined instruments yet. In the altter case, the test is skipped.
    // And this works only for uncompressed data (INSTRUME keyword found in extension header of compressed data, which is not yet read at this point
    QString instrument = "";
    searchKeyValue(headerDictionary.value("INSTRUME"), instrument);
    if (!checkInstrumentConsistency(instrument)) {
        if (rawStatus == END_OF_FILE) rawStatus = 0;
        fits_close_file(rawFptr, &rawStatus);
        printCfitsioError(__func__, rawStatus);
        return;
    }

    // FORCE a beginning with the absolute first HDU. If I use 'movrel' then I'm not sure it is going to skip
    // over the first HDU if there is one. To be tested with suitable data. Then we could remove the 'movabs'
    // and make 'movrel' the first command inside 'while () {}'
    fits_movabs_hdu(rawFptr, 1, &hduType, &rawStatus);
    while (rawStatus != END_OF_FILE && successProcessing) {

        if (hduType == IMAGE_HDU) {
            // for MMIRS and NOTcam, we only want to keep the first extension
            if (instData.name.contains("MMIRS")
                    || instData.name.contains("NOTcam")) {
                if (chip >= 1) break;
            }

            // some multi-chip cams (FORS, etc) come with separate FITS files. For them, 'chip' would always be zero,
            // and thus the correct overscan regions etc not identified correctly.
            // Others such as GROND image simultaneously in different bandpasses on multiple detectors, but they show the
            // same field of view and should be treated as single-chip cameras.
            // Hence this mapping
            int chipMapped = inferChipID(chip) - 1;   // same value as chip for normal 'MEF' files

            // For HSC@SUBARU, drop the 8 focus CCDs
            if (instData.name == "HSC@SUBARU" && chipMapped >= 104) break;
            // HSC chips too difficult for astrometry
            // "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,22,23,24,29,30,31,32,37,38,39,40,45,46,47,48,53,54,55,56,61,62,63,64,69,70,71,72,77,78,79,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104"

            // do we have an "image" (as compared to a data unit that is simply a nullptr)
            int naxis = -1;
            fits_get_img_dim(rawFptr, &naxis, &rawStatus);
            if (naxis == 0 || naxis == 1 || naxis >= 4) {
                // Empty or peculiar data units. Continue with the next one
                fits_movrel_hdu(rawFptr, 1, &hduType, &rawStatus);
                continue;
            }

            if (chipMapped == -1) {
                emit messageAvailable("Splitter::extractImagesFITS(): Could not infer chip number", "error");
                emit critical();
                successProcessing = false;
                continue;
            }

            // OK, we have either a 2D image or a cube.

            // Saturation handling
            if (instData.type == "OPT") saturationValue = pow(2,16)-1.;
            else saturationValue = pow(2,20)-1.;    // HAWAII-2RGs can on-chip coadd exposures, exceeding the usual 16-bit dynamic range
            if (userSaturationValue != 0) saturationValue = userSaturationValue;

            // Build the header. Must clear before processing new chip
            headerTHELI.clear();
            readExtHeader();
            if (!isDetectorAlive(chipMapped)) {
                fits_movrel_hdu(rawFptr, 1, &hduType, &rawStatus);
                ++chip;
                continue;
            }
            if (!isImageUsable(chipMapped)) {
                fits_movrel_hdu(rawFptr, 1, &hduType, &rawStatus);
                ++chip;
                continue;
            }
            testGROND();                     // GROND data are an oddball and need to be tested here
            updateMEFpastingStatus(chip);    // Some cameras have readout channels in individual FITS extensions, implying skipping e.g. writeImage()

            buildTheliHeaderFILTER(chip);
            buildTheliHeaderWCS(chipMapped);
            buildTheliHeaderEXPTIME();
            buildTheliHeaderDATEOBS();  // must be done before MJD-OBS
            buildTheliHeaderMJDOBS();
            buildTheliHeaderAIRMASS();

            //            buildTheliHeaderGAIN(chipMapped);
            //            buildTheliHeader();

            // Leave if this image is bad
            if (!successProcessing) continue;

            // 2D image
            if (naxis == 2) {
                getCurrentExtensionData();              // sets naxis1/2Raw, needed by everything below
                getMultiportInformation(chipMapped);    // sets naxis1/2. Updates overscan and data sections for nonstandard multiport readouts

                // overrideDetectorSections(chip);      // TESTING of a more autonomous procedure

                // gains must be built after multiport chips are assembled
                buildTheliHeaderGAIN(chipMapped);
                buildTheliHeader();

                if (instData.name.contains("LIRIS")) descrambleLiris();
                correctOverscan();
                // correctOverscan(combineOverscan_ptr, overscanX[chipMapped], overscanY[chipMapped]);
                //                cropDataSection(dataSection[chipMapped]);
                pasteMultiportIlluminatedSections(chipMapped);
                correctXtalk();             // Must maintain original detector geometry for x-talk correction, i.e. do before cropping. Must replace naxisi by naxisiRaw in xtalk methods.

                correctNonlinearity(chipMapped);
                convertToElectrons(chipMapped);
                applyMask(chipMapped);
                writeImage(chipMapped);
                //                initMyImage(chip);
            }

            // Cube
            if (naxis == 3) {
                getDataInCube();
                getMultiportInformation(chipMapped);    // Update overscan and data sections for nonstandard multiport readouts

                buildTheliHeaderGAIN(chipMapped);
                buildTheliHeader();

                // Test for invalid cube. Not sure such a FIS file can exist?
                if (naxis3Raw == 0) continue;    // Invalid cube. Not sure such a thing can exist?

                // For these instruments we want to stack (mean) the cube, not slice it
                QStringList instruments = {"TRECS@GEMINI"};
                if (instruments.contains(instData.name)) {
                    stackCube();
                    correctOverscan();
                    // correctOverscan(combineOverscan_ptr, overscanX[chipMapped], overscanY[chipMapped]);
                    //                    cropDataSection(dataSection[chipMapped]);
                    pasteMultiportIlluminatedSections(chipMapped);
                    correctXtalk();                 // TODO: how valid is that operation for the stack?
                    correctNonlinearity(chipMapped);      // TODO: how valid is that operation for the stack?
                    convertToElectrons(chipMapped);
                    applyMask(chipMapped);
                    writeImage(chipMapped);
                    //   initMyImage(chip);
                    // TODO: how is the exposure time defined for these data? Probably requires individual solution


                    // Hamamatsus: define temporary data array, larger than the individual FITS extension,
                    // and successively copy pixels as they become available from the individual extensions.
                    // Then introduce an individual exception in writeImage() and applymask() (once all extensions are available).
                }
                else {
                    // Loop over slices, extract each of them
                    for (long i=0; i<naxis3Raw; ++i) {
                        sliceCube(i);
                        correctOverscan();
                        //                        correctOverscan(combineOverscan_ptr, overscanX[chipMapped], overscanY[chipMapped]);
                        cropDataSection(dataSection[chipMapped]);
                        correctXtalk();
                        correctNonlinearity(chipMapped);
                        convertToElectrons(chipMapped);
                        applyMask(chipMapped);
                        writeImageSlice(chip, i);
                        //      initMyImage(chip);
                    }
                }
            }
#pragma omp atomic
            *progress += progressStepSize;
        }
        fits_movrel_hdu(rawFptr, 1, &hduType, &rawStatus);

        //#pragma omp atomic
        //        *progress += progressStepSize;
        ++chip;
    }

    // Reset status (if indicating we moved past end of file, as intended)
    if (rawStatus == END_OF_FILE) rawStatus = 0;
    fits_close_file(rawFptr, &rawStatus);

    printCfitsioError(__func__, rawStatus);
}

// skip over bad detectors
bool Splitter::isDetectorAlive(int &chipMapped)
{
    if (instData.name == "SuprimeCam_200101-200104@SUBARU") {
        if (chipMapped == 6) return false;
        else if (chipMapped > 6) --chipMapped;       // compensate chip number for lost chip
    }
    if (instData.name == "WFI_chips1-6@MPGESO") {
        if (chipMapped == 7) return false;
        if (chipMapped == 8) return false;
    }

    return true;
}

// For some cameras, the first (or other) images in a sequence might be unusable
bool Splitter::isImageUsable(int &chipMapped)
{
    if (instData.name == "NIRI@GEMINI") {
        QString value = "";
        searchKeyValue(QStringList() << "DATALAB", value);
        QStringList list = value.split('-');
        QString id = list.last();
        if (id == "001") {
            if (*verbosity > 1) emit messageAvailable(baseName + " : First NIRI image in sequence, excluding ...", "ignore");
            return false;
        }
    }

    return true;
}

int Splitter::inferChipID(int chip)
{
    // Data from some cameras, such as SuprimeCam and FORS, come in separate FITS files instead of a MEF file.
    // Multichannel imagers also need to be treated here
    // We need to identify the chip number correctly:
    int chipID = chip + 1;    // external counting starts with zero; 'chipID' is the number we return for most instruments

    // These need special treatment. The 'chip' variable is not necessarily used for all of them
    if (instData.name.contains("FORS1_E2V")                   // The FORSes with their newer 2-detector configuration
            || instData.name.contains("FORS2_E2V")
            || instData.name.contains("FORS2_MIT")) {
        QString value = "";
        searchKeyValue(QStringList() << "ORIGFILE", value);       // e.g. FORS2_IMG141.43.CHIP1.fits or FORS2_IMG141.43.CHIP2.fits
        QStringList valueList = value.split("CHIP");
        if (valueList.length() == 2) {
            value = valueList.at(1);
            chipID = value.remove(".fits").toInt();
            return chipID;
        }
        else {
            emit messageAvailable("Could not determine chip number for " + instData.name, "error");
            emit critical();
            return 1;
        }
    }

    else if (instData.name == "VIMOS@VLT") {
        int value = 0;
        searchKeyValue(QStringList() << "HIERARCH ESO OCS CON QUAD", value);    // running from 1 to 4
        chipID = value;
        return chipID;
    }

    else if (instData.name == "Direct_4k_SWOPE@LCO"
             || instData.name.contains("LDSS3")) {
        int value = 0;
        searchKeyValue(QStringList() << "OPAMP", value);    // running from 1-2 (LDSS3) and from 1-4 (Direct_4k)
        chipID = value;
        return chipID;
    }

    else if (instData.name == "SuprimeCam_200105-200807@SUBARU"
             || instData.name == "SuprimeCam_200101-200104@SUBARU"
             || instData.name == "SuprimeCam_200808-201705@SUBARU"
             || instData.name == "HSC@SUBARU") {
        int value = 0;
        searchKeyValue(QStringList() << "DET-ID", value);    // running from 0 to 9 (and 0 to 111 for HSC)
        chipID = value + 1;
        return chipID;
    }

    else if (instData.name.contains("MOIRCS")) {
        int value = 0;
        searchKeyValue(QStringList() << "DET-ID", value);    // running from 1 to 2
        chipID = value;
        return chipID;
    }

    else if (instData.name == "FourStar@LCO") {
        int value = 0;
        searchKeyValue(QStringList() << "CHIP", value);    // running from 1 to 4
        chipID = value;
        return chipID;
    }

    else if (instData.name.contains("IMACS")) {
        int value = 0;
        searchKeyValue(QStringList() << "CHIP", value);    // running from 1 to 8
        chipID = value;
        return chipID;
    }

    else if (instData.name == "SPARTAN@SOAR") {
        int value = 0;
        searchKeyValue(QStringList() << "DETSERNO", value);    // detector serial number
        if (value == 102) chipID = 1;
        else if (value == 108) chipID = 2;
        else if (value == 97) chipID = 3;
        else if (value == 66) chipID = 4;
        else {
            emit messageAvailable("Unknown SPARTAN detector serial number: " + QString::number(value), "error");
            return 0;
        }
        return chipID;
    }

    // Multichannel imagers: have just one (so far) detector number per channel, and the camera.ini specifies just 1 chip
    // Otherwise chipID runs to values larger than 1 which crashes queries of the Mask class
    else if (instData.name == "GROND_OPT@MPGESO"
             || instData.name == "GROND_NIR@MPGESO") {
        //             || instData.name == "PISCO@LCO") {            // must maintain chip id because of chip-dependent pasting
        // Simultaneous observations in multiple bands, but just a single detector per band
        return 1;   // (reduced by -1 in caller)
    }

    return chipID;
}

// Retrieve the raw data pixel values
void Splitter::getCurrentExtensionData()
{
    if (!successProcessing) return;

    long naxis[2];

    // Get image geometry
    fits_get_img_size(rawFptr, 2, naxis, &rawStatus);

    // Read the data block
    naxis1Raw = naxis[0];
    naxis2Raw = naxis[1];

    long nelements = naxis1Raw*naxis2Raw;
    float *buffer = new float[nelements];
    float nullval = 0.;
    int anynull;
    long fpixel = 1;
    fits_read_img(rawFptr, TFLOAT, fpixel, nelements, &nullval, buffer, &anynull, &rawStatus);

    if (!rawStatus) {
        dataRaw.clear();
        dataRaw.resize(nelements);
        dataRaw.squeeze();
        for (long i=0; i<nelements; ++i) {
            float val = buffer[i];
            if (std::isinf(val) || std::isnan(val)) val = 0.;      // set peculiar values to zero
            dataRaw[i] = val;
        }
    }
    else {
        successProcessing = false;
    }

    delete [] buffer;
    buffer = nullptr;

    printCfitsioError(__func__, rawStatus);
}

// UNUSED
void Splitter::getDataInFirstCubeSlice()
{
    if (!successProcessing) return;

    long naxis[3];

    // Get image geometry
    fits_get_img_size(rawFptr, 3, naxis, &rawStatus);

    // Read the data block
    naxis1Raw = naxis[0];
    naxis2Raw = naxis[1];
    long nelements = naxis1Raw*naxis2Raw;
    float *buffer = new float[nelements];
    float nullval = 0.;
    int anynull;
    long *fpixel = new long[3];
    fpixel[0] = 1;
    fpixel[1] = 1;
    fpixel[2] = 1;
    fits_read_pix(rawFptr, TFLOAT, fpixel, nelements, &nullval, buffer, &anynull, &rawStatus);

    if (!rawStatus) {
        dataRaw.clear();
        dataRaw.reserve(nelements);
        for (long i=0; i<nelements; ++i) {
            float val = buffer[i];
            if (std::isinf(val) || std::isnan(val)) val = 0.;
            dataRaw.append(val);
        }
        dataRaw.squeeze();
    }

    delete [] buffer;
    delete [] fpixel;
    buffer = nullptr;
    fpixel = nullptr;

    printCfitsioError(__func__, rawStatus);
}

void Splitter::getDataInCube()
{
    if (!successProcessing) return;

    long naxis[3];

    // Get image geometry
    fits_get_img_size(rawFptr, 3, naxis, &rawStatus);

    // Read the data block
    naxis1Raw = naxis[0];
    naxis2Raw = naxis[1];
    naxis3Raw = naxis[2];
    long nelementsAll = naxis1Raw*naxis2Raw*naxis3Raw;
    float *bufferAll = new float[nelementsAll];
    int anynull;
    fits_read_3d_flt(rawFptr, 0, TFLOAT, naxis1Raw, naxis2Raw, naxis1Raw, naxis2Raw, naxis3Raw, bufferAll, &anynull, &rawStatus);

    if (!rawStatus) {
        dataCubeRaw.clear();
        dataCubeRaw.reserve(nelementsAll);
        for (long i=0; i<nelementsAll; ++i) {
            float val = bufferAll[i];
            if (std::isinf(val) || std::isnan(val)) val = 0.;
            dataCubeRaw.append(val);
        }
    }

    delete [] bufferAll;
    bufferAll = nullptr;

    printCfitsioError(__func__, rawStatus);
}

void Splitter::sliceCube(long slice)
{
    if (!successProcessing) return;

    dataRaw.clear();
    dataRaw.resize(naxis1Raw*naxis2Raw);
    dataRaw.squeeze();

    long k = 0;
    long step = naxis1Raw*naxis2Raw;
    for (long i=slice*step; i<(slice+1)*step; ++i) {
        dataRaw[k] = dataCubeRaw[i];
        ++k;
    }
}

void Splitter::stackCube()
{
    if (!successProcessing) return;

    dataRaw.clear();
    dataRaw.resize(naxis1Raw*naxis2Raw);
    dataRaw.squeeze();

    long k = 0;
    long step = naxis1Raw * naxis2Raw;
    long dim = naxis1Raw * naxis2Raw * naxis3Raw;
    for (long i=0; i<dim; ++i) {
        if (k == step) k = 0;  // reset counter when we come back to the first pixel
        dataRaw[k] += dataCubeRaw[i];
        ++k;
    }
}

void Splitter::getDetectorSections()
{
    if (!successProcessing) return;

    // WARNING
    // This is for detectors only which have a single amplifier per detector, at least without injected overscan data between illuminated pixels
    // Other configurations are handled separately

    overscanX.clear();
    overscanY.clear();
    dataSection.clear();
    gain.clear();

    overscanX.resize(instData.numChips);
    overscanY.resize(instData.numChips);
    dataSection.resize(instData.numChips);
    gain.resize(instData.numChips);

    QVector<int> xmin = instData.overscan_xmin;
    QVector<int> xmax = instData.overscan_xmax;
    QVector<int> ymin = instData.overscan_ymin;
    QVector<int> ymax = instData.overscan_ymax;

    for (int chip=0; chip<instData.numChips; ++chip) {
        // Overscan X
        QVector<long> overscanxRegion;
        if (!xmin.isEmpty() && !xmax.isEmpty()) overscanxRegion << xmin[chip] << xmax[chip];
        overscanX[chip] << overscanxRegion;

        // Overscan Y
        QVector<long> overscanyRegion;
        if (!ymin.isEmpty() && !ymax.isEmpty()) overscanyRegion << ymin[chip] << ymax[chip];
        overscanY[chip] << overscanyRegion;

        // Data Section
        QVector<long> section;
        section << instData.cutx[chip];
        section << instData.cutx[chip] + instData.sizex[chip] - 1; // sizex is not a coordinate, but the number of pixels along this axis. Hence -1
        section << instData.cuty[chip];
        section << instData.cuty[chip] + instData.sizey[chip] - 1; // sizey is not a coordinate, but the number of pixels along this axis. Hence -1
        dataSection[chip] << section;
    }
}

// TESTING
void Splitter::overrideDetectorSections(int chip)
{
    if (instData.name == "MOSAIC-III@KPNO_4m"
            || instData.name == "WFC@INT") {
        multiportOverscanDirections << "vertical";
        QVector<long> tmp = extractVerticesFromKeyword("BIASSEC");
        overscanX[chip] << tmp.at(0) << tmp.at(1);
        dataSection[chip] = extractVerticesFromKeyword("DATASEC");
        dataSection[chip] = extractVerticesFromKeyword("TRIMSEC");
        naxis1 = dataSection[chip].at(1) - dataSection[chip].at(0);
        naxis2 = dataSection[chip].at(3) - dataSection[chip].at(2);
    }
}

void Splitter::getNumberOfAmplifiers()
{
    if (!successProcessing) return;

    numAmpPerChip = 1;     // The number of amplifiers forming data for a single detector. Always 1, unless stored in separate FITS extensions

    if (instData.name.contains("GMOS-N-HAM") || instData.name.contains("GMOS-S-HAM")) {
        // update numAmpPerChip with NAMPS keyword
        fits_read_key_lng(rawFptr, "NAMPS", &numAmpPerChip, nullptr, &rawStatus);
        if (rawStatus) {
            emit messageAvailable("Splitter::getNumberOfAmplifiers(): Could not read NAMPS keyword!", "error");
            numAmpPerChip = 1;
            rawStatus = 0;
        }
    }
    // 2 amps per detector
    if (instData.name == "SOI@SOAR"
            || instData.name == "DEIMOS_2AMP@KECK"
            || instData.name == "LRIS_BLUE@KECK"
            || instData.name == "LRIS_RED@KECK"
            || instData.name == "MOSAIC-II_16@CTIO"
            || instData.name == "PISCO@LCO") {
        numAmpPerChip = 2;
        rawStatus = 0;
    }
    // 4 amps per detector (geometric layout doesn't matter here)
    if (instData.name == "MOSAIC-III_4@KPNO_4m"
            || instData.name == "SAMI_2x2@SOAR") {
        numAmpPerChip = 4;
        rawStatus = 0;
    }

    // multiple readout channels in different FITS extensions
    multiChannelMultiExt << "DEIMOS_2AMP@KECK"
                         << "GMOS-N-HAM@GEMINI" <<  "GMOS-N-HAM_1x1@GEMINI"
                         << "GMOS-S-HAM@GEMINI" << "GMOS-S-HAM_1x1@GEMINI" << "GMOS-S-HAM_4x4@GEMINI"
                         << "LRIS_BLUE@KECK" << "LRIS_RED@KECK"
                         << "MOSAIC-II_16@CTIO" << "MOSAIC-III_4@KPNO_4m"
                         << "PISCO@LCO" << "SAMI_2x2@SOAR" << "SOI@SOAR";
    if (multiChannelMultiExt.contains(instData.name)) ampInSeparateExt = true;

    if (numAmpPerChip > 1 && ampInSeparateExt) {
        overscanX.clear();
        overscanY.clear();
        dataSection.clear();
        gain.clear();
        overscanX.resize(instData.numChips * numAmpPerChip);
        overscanY.resize(instData.numChips * numAmpPerChip);
        dataSection.resize(instData.numChips * numAmpPerChip);
        gain.resize(instData.numChips * numAmpPerChip);
    }
}

// Write the pixel-corrected extension as a separate FITS file to disk
void Splitter::writeImage(int chipMapped)
{
    if (!successProcessing) return;

    if (!MEFpastingFinished) return;

    // Do not write bad detectors
    // Easiest done here, and not further above, in case of detectors with multiple readout amps
    if (instData.badChips.contains(chipMapped)) return;

    // Exceptions. Return if successful.
    if (individualFixWriteImage(chipMapped)) return;

    // The new output file
    fitsfile *fptr;
    int status = 0;
    long fpixel = 1;
    long nelements = naxis1*naxis2;
    float *array = new float[nelements];
    for (long i=0; i<nelements; ++i) {
        array[i] = dataCurrent[i];
    }

    long naxis = 2;
    long naxes[2] = { naxis1, naxis2 };

    // Infer true chip number:
    //    int chipID = inferChipID(chip);

    int chipID = chipMapped + 1;

    // adjust chipID for data where multiple channels are in separate extensions
    if (multiChannelMultiExt.contains(instData.name)) {
        if (instData.name.contains("GMOS")) {
            if (chipMapped == 3) chipID = 1;
            if (chipMapped == 7) chipID = 2;
            if (chipMapped == 11) chipID = 3;
        }
        if (instData.name == "SOI@SOAR") {
            if (chipMapped == 1) chipID = 1;
            if (chipMapped == 3) chipID = 2;
        }
        if (instData.name == "DEIMOS_2AMP@KECK") {
            if (chipMapped == 1) chipID = 1;
            if (chipMapped == 3) chipID = 2;
            if (chipMapped == 5) chipID = 3;
            if (chipMapped == 7) chipID = 4;
        }
        if (instData.name == "LRIS_BLUE@KECK") {
            if (chipMapped == 1) chipID = 1;
            if (chipMapped == 3) chipID = 2;
        }
        if (instData.name == "LRIS_RED@KECK") {
            if (chipMapped == 1) chipID = 1;
            if (chipMapped == 3) chipID = 2;
        }
        if (instData.name == "MOSAIC-II_16@CTIO") {
            if (chipMapped == 1) chipID = 1;
            if (chipMapped == 3) chipID = 2;
            if (chipMapped == 5) chipID = 3;
            if (chipMapped == 7) chipID = 4;
            if (chipMapped == 9) chipID = 5;
            if (chipMapped == 11) chipID = 6;
            if (chipMapped == 13) chipID = 7;
            if (chipMapped == 15) chipID = 8;
        }
        if (instData.name == "PISCO@LCO") {        // multi-channel imager
            if (chipMapped == 1) chipID = 1;
            if (chipMapped == 3) chipID = 1;
            if (chipMapped == 5) chipID = 1;
            if (chipMapped == 7) chipID = 1;
        }
        if (instData.name == "MOSAIC-III_4@KPNO_4m") {
            if (chipMapped == 3) chipID = 1;
            if (chipMapped == 7) chipID = 2;
            if (chipMapped == 11) chipID = 3;
            if (chipMapped == 15) chipID = 4;
        }
        if (instData.name == "SAMI_2x2@SOAR") {
            if (chipMapped == 3) chipID = 1;
        }
        MEFpastingFinished = false;
    }

    // Replace blanks and other unwanted chars in file names
    baseName.replace(' ','_');
    baseName.replace('[','_');
    baseName.replace(']','_');
    baseName.remove('$');
    baseName.remove('&');
    baseName.remove('#');

    // Output file name
    splitFileName = "!"+path+"/"+baseName+"_"+QString::number(chipID)+"P.fits";
    // If renaming active, and dateobs was determined successfully
    if (cdw->ui->theliRenamingCheckBox->isChecked() && dateObsValue != "2020-01-01T00:00:00.000") {
        if (dataFormat == "RAW" || !instData.bayer.isEmpty()) {
            // No filter name for bayer matrix images
            splitFileName = "!"+path+"/"+instData.shortName+"."+dateObsValue+"_"+QString::number(chipID)+"P.fits";
        }
        else {
            // general case
            splitFileName = "!"+path+"/"+instData.shortName+"."+filter+"."+dateObsValue+"_"+QString::number(chipID)+"P.fits";
            // special cases (overrides outName)
            individualFixOutName(chipID);
        }
    }
    fits_create_file(&fptr, splitFileName.toUtf8().data(), &status);
    fits_create_img(fptr, FLOAT_IMG, naxis, naxes, &status);
    fits_write_img(fptr, TFLOAT, fpixel, nelements, array, &status);

    // Propagate header
    for (int i=0; i<headerTHELI.length(); ++i) {
        fits_write_record(fptr, headerTHELI[i].toUtf8().constData(), &status);
    }
    // Update the SATURATE keyword
    fits_update_key_flt(fptr, "SATURATE", saturationValue, 6, nullptr, &status);
    fits_close_file(fptr, &status);

    delete [] array;
    array = nullptr;

    printCfitsioError(__func__, status);
}

void Splitter::individualFixOutName(const int chipID)
{
    bool individualFixDone = false;
    bool test = true;
    if (instData.name == "VIMOS@VLT") {
        test = searchKeyValue(QStringList() << "HIERARCH ESO DET EXP NO", uniqueID);
        uniqueID = uniqueID.split('_').at(0);   //  " / char was already replaced by _ in searchKeyInHeaderValue()"
        uniqueID = uniqueID.simplified();
        individualFixDone = true;
    }
    //    if (instData.name == "Direct_4k_SWOPE@LCO") {
    //        test = searchKeyValue(QStringList() << "FILENAME", uniqueID);
    //        uniqueID = uniqueID.remove(7,2);
    //        individualFixDone = true;
    //    }
    else if (instData.name.contains("MOIRCS")) {
        test = searchKeyValue(QStringList() << "EXP-ID", uniqueID);    // e.g. MCSE00012193
        individualFixDone = true;
    }
    else if (instNameFromData == "GROND_OPT@MPGESO") {
        individualFixDone = true;
    }
    else if (instData.name == "PISCO@LCO") {
        individualFixDone = true;
    }

    if (individualFixDone) {
        if (!test) {
            emit messageAvailable(baseName + " : Could not determine unambiguous file name!", "error");
            emit critical();
            successProcessing = false;
        }
        else {
            if (instNameFromData == "GROND_OPT@MPGESO") {
                QString newPath = path+"_"+filter;
                QDir newDir(newPath);
                newDir.mkpath(newPath);
                splitFileName = "!"+newPath+"/"+instData.shortName+"."+filter+"."+dateObsValue+"_"+QString::number(chipID)+"P.fits";
            }
            else if (instData.name == "PISCO@LCO") {
                QString newPath = path+"_"+filter;
                QDir newDir(newPath);
                newDir.mkpath(newPath);
                splitFileName = "!"+newPath+"/"+instData.shortName+"."+filter+"."+dateObsValue+"_"+QString::number(chipID)+"P.fits";
            }
            else {
                splitFileName = "!"+path+"/"+instData.shortName+"."+filter+"."+uniqueID+"_"+QString::number(chipID)+"P.fits";
            }
        }
    }
}

// Some instrument modes need special treatment
bool Splitter::individualFixWriteImage(int chipMapped)
{
    bool individualFixDone = false;

    if (instData.name == "LIRIS_POL@WHT") {    // Write the four subregions with the different polarization angles as separate FITS files

        for (int channel=0; channel<=3; ++channel) {
            fitsfile *fptr;
            int status = 0;
            long fpixel = 1;
            // WARNING: If the SIZEx/y vectors in instrument.ini change, we must also change the ijminmax below!
            long nax1 = instData.sizex[0];
            long nax2 = instData.sizey[0];
            long nelements = nax1*nax2;
            long imin = 43;   // same for all channels
            long imax = 980;  // same for all channels
            long jmin = 0;
            long jmax = 0;
            QString channelID = "";
            if (channel == 0) {      // 0 deg
                jmin = 739;   // always 200 pixels high
                jmax = 938;
                channelID = "PA000";
            }
            else if (channel == 1) { // 90 deg
                jmin = 513;
                jmax = 712;
                channelID = "PA090";
            }
            else if (channel == 2) { // 135 deg
                jmin = 285;
                jmax = 484;
                channelID = "PA135";
            }
            else if (channel == 3) { // 45 deg
                jmin = 62;
                jmax = 261;
                channelID = "PA045";
            }
            float *array = new float[nelements];
            long k = 0;
            for (long j=0; j<naxis2Raw; ++j) {
                for (long i=0; i<naxis2Raw; ++i) {
                    if (i>=imin && i<=imax && j>=jmin && j<=jmax) {
                        array[k] = dataCurrent[i+naxis1Raw*j];
                        ++k;
                    }
                }
            }
            long naxis = 2;
            long naxes[2] = {nax1, nax2};

            // Replace blanks in file names
            baseName.replace(' ','_');

            // Output file goes to a separate directory to account for different detectors / filters
            QString newPath = path+"_"+channelID;
            QDir newDir(newPath);
            newDir.mkpath(newPath);
            QString outName = "!"+newPath+"/"+baseName+"_"+channelID+"_1P.fits";
            // If renaming active, and dateobs was determined successfully
            if (cdw->ui->theliRenamingCheckBox->isChecked() && dateObsValue != "2020-01-01T00:00:00.000") {
                outName = "!"+newPath+"/"+instData.shortName+"."+filter+"_"+channelID+"."+dateObsValue+"_1P.fits";
            }
            fits_create_file(&fptr, outName.toUtf8().data(), &status);
            fits_create_img(fptr, FLOAT_IMG, naxis, naxes, &status);
            fits_write_img(fptr, TFLOAT, fpixel, nelements, array, &status);

            // Propagate header
            for (int i=0; i<headerTHELI.length(); ++i) {
                fits_write_record(fptr, headerTHELI[i].toUtf8().constData(), &status);
            }
            // Update the filter keyword with the polarization angle
            QString newFilter = filter+"_"+channelID;
            fits_update_key_str(fptr, "FILTER", newFilter.toUtf8().data(), nullptr, &status);
            fits_close_file(fptr, &status);

            delete [] array;
            array = nullptr;

            printCfitsioError(__func__, status);
        }
        individualFixDone = true;
    }

    if (instNameFromData == "GROND_NIR@MPGESO") {       // Write the three channels as separate FITS files
        for (int channel=0; channel<=2; ++channel) {
            fitsfile *fptr;
            int status = 0;
            long fpixel = 1;
            long nax1 = 1024;
            long nax2 = 1024;
            long nelements = nax1*nax2;
            long imin = 0;
            long imax = 0;
            long jmin = 0;     // same for all channels
            long jmax = 1023;  // same for all channels
            float gain = 1.0;
            QString channelID = "";
            if (channel == 0) {
                imin = 0;
                imax = 1023;
                channelID = "J";
                searchKeyValue(QStringList() << "J_GAIN", gain);
            }
            else if (channel == 1) {
                imin = 1024;
                imax = 2047;
                channelID = "H";
                searchKeyValue(QStringList() << "H_GAIN", gain);
            }
            else if (channel == 2) {
                imin = 2048;
                imax = 3071;
                channelID = "K";
                searchKeyValue(QStringList() << "K_GAIN", gain);
            }
            float *array = new float[nelements];
            long k = 0;
            for (long j=0; j<naxis2Raw; ++j) {
                for (long i=0; i<naxis1Raw; ++i) {
                    if (i>=imin && i<=imax && j>=jmin && j<=jmax) {
                        array[k] = -1.0 * dataCurrent[i+naxis1Raw*j] * gain;     // ADUs counting negative?? I thought I have seen it all ...
                        ++k;
                    }
                }
            }
            long naxis = 2;
            long naxes[2] = {nax1, nax2};

            // Replace blanks in file names
            baseName.replace(' ','_');

            // Output file goes to a separate directory to account for different detectors / filters
            QString newPath = path+"_"+channelID;
            QDir newDir(newPath);
            newDir.mkpath(newPath);
            QString outName = "!"+newPath+"/"+baseName+"_"+channelID+"_1P.fits";
            // If renaming active, and dateobs was determined successfully
            if (cdw->ui->theliRenamingCheckBox->isChecked() && dateObsValue != "2020-01-01T00:00:00.000") {
                outName = "!"+newPath+"/"+instData.shortName+"."+channelID+"."+dateObsValue+"_1P.fits";
            }
            fits_create_file(&fptr, outName.toUtf8().data(), &status);
            fits_create_img(fptr, FLOAT_IMG, naxis, naxes, &status);
            fits_write_img(fptr, TFLOAT, fpixel, nelements, array, &status);

            // Propagate header
            for (int i=0; i<headerTHELI.length(); ++i) {
                fits_write_record(fptr, headerTHELI[i].toUtf8().constData(), &status);
            }
            // Update the filter keyword with the polarization angle
            QString newFilter = channelID;
            fits_update_key_str(fptr, "FILTER", newFilter.toUtf8().data(), nullptr, &status);
            // Update the gain
            fits_update_key_flt(fptr, "GAINORIG", gain, 6, nullptr, &status);

            fits_close_file(fptr, &status);

            delete [] array;
            array = nullptr;

            printCfitsioError(__func__, status);
        }
        individualFixDone = true;
    }

    return individualFixDone;
}

void Splitter::writeImageSlice(int chip, long slice)
{
    if (!successProcessing) return;

    // The new output file
    fitsfile *fptr;
    int status = 0;
    long fpixel = 1;
    long nelements = naxis1*naxis2;
    float *array = new float[nelements];
    for (long i=0; i<nelements; ++i) {
        array[i] = dataCurrent[i];
    }

    long naxis = 2;
    long naxes[2] = { naxis1, naxis2 };

    // Infer true chip number:
    int chipID = inferChipID(chip);

    // Output file name
    QString outName;
    if (naxis3Raw == 1) outName = "!"+path+"/"+baseName+"_"+QString::number(chipID)+"P.fits";
    else outName = "!"+path+"/"+baseName+"_sl"+QString::number(slice)+"_"+QString::number(chipID)+"P.fits";
    // If renaming active, and dateobs was determined successfully
    if (cdw->ui->theliRenamingCheckBox->isChecked() && dateObsValue != "2020-01-01T00:00:00.000") {
        if (dataFormat == "RAW") {
            // No filter name for bayer matrix images
            if (naxis3Raw == 1) outName = "!"+path+"/"+instData.shortName+"."+dateObsValue+"_"+QString::number(chipID)+"P.fits";
            else outName = "!"+path+"/"+instData.shortName+"."+dateObsValue+"_sl"+QString::number(slice)+"_"+QString::number(chipID)+"P.fits";
        }
        else {
            if (naxis3Raw == 1) outName = "!"+path+"/"+instData.shortName+"."+filter+"."+dateObsValue+"_"+QString::number(chipID)+"P.fits";
            else outName = "!"+path+"/"+instData.shortName+"."+filter+"."+dateObsValue+"_sl"+QString::number(slice)+"_"+QString::number(chipID)+"P.fits";
        }
    }
    fits_create_file(&fptr, outName.toUtf8().data(), &status);
    fits_create_img(fptr, FLOAT_IMG, naxis, naxes, &status);
    fits_write_img(fptr, TFLOAT, fpixel, nelements, array, &status);

    // Propagate header
    for (int i=0; i<headerTHELI.length(); ++i) {
        fits_write_record(fptr, headerTHELI[i].toUtf8().constData(), &status);
    }

    // Update the SATURATE keyword
    fits_update_key_flt(fptr, "SATURATE", saturationValue, 6, nullptr, &status);

    // WARNING: POOR TIMIMG! Increment MJD-OBS by 0.1 s per slice
    double mjdobs = 0.;
    fits_read_key_dbl(fptr, "MJD-OBS", &mjdobs, NULL, &status);
    mjdobs += slice*1.157e-6;
    fits_update_key_dbl(fptr, "MJD-OBS", mjdobs, 12, NULL, &status);
    fits_close_file(fptr, &status);

    delete [] array;
    array = nullptr;

    printCfitsioError(__func__, status);
}

// UNUSED. Not sure whether I need the MyImages sorted
// Crashes with an out-of-range error on mask->globalMask (or mask->isChipMasked) vector when restoring data from RAWDATA and rerunning.
// Works fine if done the first time.
void Splitter::initMyImage(int chip)
{
    if (!successProcessing) return;

    // TODO: must use chipID instead of "chip", e.g. for FORS2

    MyImage *myImage = new MyImage(path, baseName+"_"+QString::number(chip+1)+"P.fits", "P", chip+1,
                                   mask->globalMask[chip], verbosity);
    myImage->setParent(this);
    myImage->imageOnDrive = true;
    omp_set_lock(genericLock);
    data->myImageList[chip].append(myImage);
    if (!data->uniqueChips.contains(chip+1)) data->uniqueChips.push_back(chip+1);
    omp_unset_lock(genericLock);
    connect(myImage, &MyImage::modelUpdateNeeded, data, &Data::modelUpdateReceiver);
    connect(myImage, &MyImage::critical, data, &Data::pushCritical);
    connect(myImage, &MyImage::warning, data, &Data::pushWarning);
    connect(myImage, &MyImage::messageAvailable, data, &Data::pushMessageAvailable);
    connect(myImage, &MyImage::setMemoryLock, data, &Data::setMemoryLockReceived, Qt::DirectConnection);
    connect(myImage, &MyImage::setWCSLock, data, &Data::setWCSLockReceived, Qt::DirectConnection);
    myImage->emitModelUpdateNeeded();
    myImage->readImage(splitFileName.remove("!"));
    ++data->numImages;
}

void Splitter::printCfitsioError(QString funcName, int status)
{
    if (status) {
        CfitsioErrorCodes *errorCodes = new CfitsioErrorCodes(this);
        emit messageAvailable(baseName + " Splitter::"+funcName+":<br>" + errorCodes->errorKeyMap.value(status), "error");
        emit critical();
        successProcessing = false;
    }
}

void Splitter::testGROND()
{
    // we want to identify for each exposure whether it comes from the optical or the NIR camera
    // so that we can use the appropriate branch to deal with the data
    if (instData.name.contains("GROND")) {
        int nax1 = 0;
        searchKeyValue(QStringList() << "NAXIS1", nax1);
        if (nax1 == 3072) instNameFromData = "GROND_NIR@MPGESO";
        else instNameFromData = "GROND_OPT@MPGESO";
    }
}

void Splitter::readExtHeader()
{
    if (rawStatus) return;

    // Read the entire header. This should always work!
    char *fullheader = nullptr;
    int numHeaderKeys = 0;
    fits_hdr2str(rawFptr, TRUE, NULL, 0, &fullheader, &numHeaderKeys, &rawStatus);
    printCfitsioError(__func__, rawStatus);
    if (rawStatus) return;

    fullExtHeaderString = QString::fromUtf8(fullheader);
    fits_free_memory(fullheader, &rawStatus);

    // Map the header onto a QVector<QString>
    int cardLength = 80;
    long length = fullExtHeaderString.length();
    if (length<80) return;
    extHeader.clear();
    for (long i=0; i<=length-cardLength; i+=cardLength) {
        QStringRef subString(&fullExtHeaderString, i, cardLength);
        QString string = subString.toString();
        extHeader.push_back(string);
    }
}

void Splitter::readPrimaryHeader()
{
    if (rawStatus) return;
    if (!successProcessing) return;

    // Read the entire header. This should always work!
    char *fullheader = nullptr;
    int numHeaderKeys = 0;
    fits_hdr2str(rawFptr, TRUE, NULL, 0, &fullheader, &numHeaderKeys, &rawStatus);
    printCfitsioError(__func__, rawStatus);
    if (rawStatus) return;

    fullPrimaryHeaderString = QString::fromUtf8(fullheader);
    fits_free_memory(fullheader, &rawStatus);

    // Map the header onto a QVector<QString>
    int cardLength = 80;
    long length = fullPrimaryHeaderString.length();
    if (length<80) return;
    primaryHeader.clear();
    for (long i=0; i<=length-cardLength; i+=cardLength) {
        QStringRef subString(&fullPrimaryHeaderString, i, cardLength);
        QString string = subString.toString();
        primaryHeader.push_back(string);
    }
}

bool Splitter::checkCorrectMaskSize(const int chip)
{
    if (instData.name.contains("DUMMY")) return true;

    long n_mask = mask->globalMask[chip].length();
    long n_data = dataCurrent.length();
    // detectors with multi-amps (one amp per FITS extension): masked at a later stage (calibration)
    if (instNameFromData == "GROND_NIR@MPGESO" && instData.name == "GROND_NIR@MPGESO") return true;
    if (n_mask > 0 && n_data > 0 && n_mask != n_data) {
        if (!maskSizeWarningShown) {
            emit messageAvailable(fileName + ": Inconsistent image size detected between data and instrument configuration"
                                             " (overscan and / or data section) in\n"+instData.nameFullPath+
                                  " \n. The image will not be processed, please remove it manually.", "warning");
            emit warning();
            //            successProcessing = false;
            //            maskSizeWarningShown = true;
            return false;
        }
    }
    return true;
}

bool Splitter::checkInstrumentConsistency(QString foundInstrumentName)
{
    QString expectedInstrumentName = instrumentDictionary.value(instData.name);
    if (!expectedInstrumentName.isEmpty() && expectedInstrumentName != foundInstrumentName) {
        emit messageAvailable(fileName + ": Wrong instrument selected: Expected " + instData.name + ", but \"INSTRUME= " + foundInstrumentName+"\"", "error");
        emit critical();
        successProcessing = false;
        return false;
    }
    return true;
}
