#include "splitter.h"
#include "../instrumentdata.h"
#include "../myimage/myimage.h"
#include "../myfits/myfits.h"
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
#include <QVector>
#include <QFile>
#include <QDir>

Splitter::Splitter(instrumentDataType &instrumentData, Mask *detectorMask, Data *someData, QString datatype,
                   ConfDockWidget *confDockWidget, QString maindirname,
                   QString subdirname, QString filename, int *verbose,
                   QObject *parent) : QObject(parent)
{
    instData = instrumentData;
    fileName = filename;
    cdw = confDockWidget;
    mainDirName = maindirname;
    subDirName = subdirname;
    path = mainDirName + "/" + subDirName;
    name = path+"/"+fileName;
    dataType = datatype;
    QFileInfo fi(name);
    baseName = fi.completeBaseName();
    verbosity = verbose;
    mask = detectorMask;
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
    moveFile(fileName, path, path+"/RAWDATA/");
}

void Splitter::determineFileFormat()
{
    if (!successProcessing) return;

    // Try opening as FITS
    fits_open_file(&rawFptr, name.toUtf8().data(), READONLY, &rawStatus);

    if (!rawStatus) {
        dataFormat = "FITS";
        uncompress();
        consistencyChecks();
        getDetectorSections(); // Read overscan and data sections for the current instrument
        gain.resize(instData.numChips);
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
                dataFormat = "Unknown";
                return;
            }
        }
        else dataFormat = "Unknown";
    }

    if (dataFormat == "Unknown") {
        // FITS opening error?
        printCfitsioError("determineFileFormat()", rawStatus);
        successProcessing = false;
        QDir unknownFile(path+"/UnknownFormat");
        unknownFile.mkpath(path+"/UnknownFormat/");
        moveFile(name, path, path+"/UnknownFormat");
        emit messageAvailable(fileName+" : Unknown format. Moved to "+subDirName+"/UnknownFormat/", "ignore");
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
        // delete compressed file if uncompression was successful, create new fits pointer to uncompressed file
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
                printCfitsioError("uncompress()", rawStatus);
            }
        }
        else {
            emit messageAvailable(fi.fileName() + " : Uncompression did not work as expected!", "error");
            printCfitsioError("uncompress()", rawStatus);
            printCfitsioError("uncompress()", outRawStatus);
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
    }

    printCfitsioError("consistencyChecks()", rawStatus);
}

void Splitter::extractImages()
{
    if (!successProcessing) return;

    // adjust progress step size for multi-chip cameras whose detectors are stored in single extension FITS files
    QStringList instruments = {"FORS1_E2V_2x2@VLT", "FORS2_E2V_2x2@VLT", "FORS2_MIT_1x1@VLT", "FORS2_MIT_2x2@VLT",
                               "FourStar@LCO",
                               "MOIRCS_200406-201006@SUBARU", "MOIRCS_201007-201505@SUBARU", "MOIRCS_201512-today@SUBARU",
                               "SuprimeCam_200101-200104@SUBARU", "SuprimeCam_200105-200807@SUBARU", "SuprimeCam_200808@SUBARU",
                               "SuprimeCam_200808_SDFRED@SUBARU", "VIMOS@VLT"};
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

    // FORCE a beginning with the absolute first HDU. If I use 'movrel' then I'm not sure it is going to skip
    // over the first HDU if there is one. To be tested with suitable data. Then we could remove the 'movabs'
    // and make 'movrel' the first command inside 'while () {}'
    fits_movabs_hdu(rawFptr, 1, &hduType, &rawStatus);
    while (rawStatus != END_OF_FILE && successProcessing) {
        if (hduType == IMAGE_HDU) {

            // some multi-chip cams (FORS, etc) come with separate FITS files. For them, 'chip' would always be zero,
            // and thus the correct overscan regions etc not identified correctly.
            // hence this mapping
            int chipMapped = inferChipID(chip) - 1;   // same value as chip for normal 'MEF' files

            // do we have an "image" (as compared to a data unit that is simply a nullptr)
            int naxis = -1;
            fits_get_img_dim(rawFptr, &naxis, &rawStatus);
            if (naxis == 0 || naxis == 1 || naxis >= 4) {
                // Empty or peculiar data units. Continue with the next one
                fits_movrel_hdu(rawFptr, 1, &hduType, &rawStatus);
                continue;
            }

            // OK, we have either a 2D image or a cube.

            // Build the header. Must clear before processing new chip
            headerTHELI.clear();
            readExtHeader();
            if (!isDetectorAlive(chipMapped)) {
                fits_movrel_hdu(rawFptr, 1, &hduType, &rawStatus);
                ++chip;
                continue;
            }
            buildTheliHeaderFILTER();
            buildTheliHeaderWCS(chipMapped);
            buildTheliHeaderEXPTIME();
            buildTheliHeaderDATEOBS();  // must be done before MJD-OBS
            buildTheliHeaderMJDOBS();
            buildTheliHeaderAIRMASS();
            buildTheliHeaderGAIN(chipMapped);
            buildTheliHeader();

            // 2D image
            if (naxis == 2) {
                getCurrentExtensionData();
                correctOverscan(combineOverscan_ptr, overscanX[chipMapped], overscanY[chipMapped]);
                cropDataSection(dataSection[chipMapped]);
                if (instData.name.contains("LIRIS")) descrambleLiris();
                // CHECK: xtalk correction only works if we maintain the original detector geometry
                correctXtalk();
                correctNonlinearity(chipMapped);
                convertToElectrons(chipMapped);
                applyMask(chipMapped);
                writeImage(chipMapped);
                //       initMyImage(chip);
            }

            // Cube
            if (naxis == 3) {
                getDataInCube();
                // Test for invalid cube. Not sure such a thing can exist?
                if (naxis3Raw == 0) continue;    // Invalid cube. Not sure such a thing can exist?

                // For these instruments we want to stack (mean) the cube, not slice it
                QStringList instruments = {"TRECS@GEMINI"};
                if (instruments.contains(instData.name)) {
                    stackCube();
                    correctOverscan(combineOverscan_ptr, overscanX[chipMapped], overscanY[chipMapped]);
                    cropDataSection(dataSection[chipMapped]);
                    correctXtalk();                 // TODO: how valid is that operation for the stack?
                    correctNonlinearity(chipMapped);      // TODO: how valid is that operation for the stack?
                    convertToElectrons(chipMapped);
                    applyMask(chipMapped);
                    writeImage(chipMapped);
                    //   initMyImage(chip);
                    // TODO: how is the exposure time defined for these data? Probably requires individual solution
                }
                else {
                    // Loop over slices, extract each of them
                    for (long i=0; i<naxis3Raw; ++i) {
                        sliceCube(i);
                        correctOverscan(combineOverscan_ptr, overscanX[chipMapped], overscanY[chipMapped]);
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

    printCfitsioError("extractImagesFITS()", rawStatus);
}

// skip over bad detectors
bool Splitter::isDetectorAlive(int &chipMapped)
{
    if (instData.name == "SuprimeCam_200101-200104@SUBARU") {
        if (chipMapped == 6) return false;
        else if (chipMapped > 6) --chipMapped;       // compensate chip number for lost chip
    }

    return true;
}

int Splitter::inferChipID(int chip)
{
    // Data from some cameras, such as SuprimeCam and FORS, come in separate FITS files instead of a MEF file.
    // We need to identify the chip number correctly:
    int chipID = chip + 1;    // external counting starts with zero; 'chipID' is the number we return for most instruments

    // These need special treatment. The 'chip' variable is not necessarily used for all of them
    if (instData.name.contains("FORS1_E2V")                   // The FORSes with their newer 2-detector configuration
            || instData.name.contains("FORS2_E2V")
            || instData.name.contains("FORS2_MIT")) {
        QString value = "";
        searchKeyValue(QStringList() << "ORIGFILE", value);       // FORS2_IMG141.43.CHIP1.fits or FORS2_IMG141.43.CHIP2.fits
        QStringList valueList = value.split("CHIP");
        if (valueList.length() == 2) {
            value = valueList.at(1);
            chipID = value.remove(".fits").toInt();
            return chipID;
        }
        else {
            emit messageAvailable("Could not determine chip number for " + instData.name, "error");
            emit critical();
            return 0;
        }
    }

    else if (instData.name == "VIMOS@VLT") {
        int value = 0;
        searchKeyValue(QStringList() << "HIERARCH ESO OCS CON QUAD", value);    // running from 1 to 4
        chipID = value;
        return chipID;
    }

    else if (instData.name == "SuprimeCam_200105-200807@SUBARU" || instData.name == "SuprimeCam_200101-200104@SUBARU") {
        int value = 0;
        searchKeyValue(QStringList() << "DET-ID", value);    // running from 0 to 9
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

    return chipID;
}

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
        dataRaw.reserve(nelements);
        for (long i=0; i<nelements; ++i) {
            float val = buffer[i];
            if (isinf(val) || isnan(val)) val = 0.;
            dataRaw.append(val);
        }
    }

    delete [] buffer;

    printCfitsioError("getCurrentExtensionData()", rawStatus);
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
            if (isinf(val) || isnan(val)) val = 0.;
            dataRaw.append(val);
        }
    }

    delete [] buffer;
    delete [] fpixel;

    printCfitsioError("getDataInFirstCubeSlice()", rawStatus);
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
            if (isinf(val) || isnan(val)) val = 0.;
            dataCubeRaw.append(val);
        }
    }

    delete [] bufferAll;

    printCfitsioError("getDataInCube()", rawStatus);
}

void Splitter::sliceCube(long slice)
{
    if (!successProcessing) return;

    dataRaw.clear();
    dataRaw.resize(naxis1Raw*naxis2Raw);

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

    overscanX.clear();
    overscanY.clear();
    dataSection.clear();

    overscanX.resize(instData.numChips);
    overscanY.resize(instData.numChips);
    dataSection.resize(instData.numChips);

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

// Write the pixel-corrected extension as a separate FITS file to disk
// Condensed version of the same function as in MyFITS
void Splitter::writeImage(int chipMapped)
{
    if (!successProcessing) return;

    // Exceptions. Return if successful.
    if (individualFixCDmatrix(chipMapped)) return;

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

    // Replace blanks in file names
    baseName.replace(' ','_');

    // Output file name
    QString outName = "!"+path+"/"+baseName+"_"+QString::number(chipID)+"P.fits";
    // If renaming active, and dateobs was determined successfully
    if (cdw->ui->theliRenamingCheckBox->isChecked() && dateObsValue != "2020-01-01T00:00:00.000") {
        if (dataFormat == "RAW") {
            // No filter name for bayer matrix images
            outName = "!"+path+"/"+instData.shortName+"."+dateObsValue+"_"+QString::number(chipID)+"P.fits";
        }
        else {
            // general case
            outName = "!"+path+"/"+instData.shortName+"."+filter+"."+dateObsValue+"_"+QString::number(chipID)+"P.fits";
            // special cases (overrides outName)
            individualFixOutName(outName, chipID);
        }
    }
    fits_create_file(&fptr, outName.toUtf8().data(), &status);
    fits_create_img(fptr, FLOAT_IMG, naxis, naxes, &status);
    fits_write_img(fptr, TFLOAT, fpixel, nelements, array, &status);

    // Propagate header
    for (int i=0; i<headerTHELI.length(); ++i) {
        fits_write_record(fptr, headerTHELI[i].toUtf8().constData(), &status);
    }
    fits_close_file(fptr, &status);

    delete [] array;

    printCfitsioError("writeImage()", status);
}

void Splitter::individualFixOutName(QString &outname, const int chipID)
{
    bool individualFixDone = false;
    bool test = true;
    if (instData.name == "VIMOS@VLT") {
        test = searchKeyValue(QStringList() << "HIERARCH ESO DET EXP NO", uniqueID);
        uniqueID = uniqueID.split('_').at(0);   //  " / char was already replaced by _ in searchKeyInHeaderValue()"
        uniqueID = uniqueID.simplified();
        individualFixDone = true;
    }
    else if (instData.name.contains("MOIRCS")) {
        test = searchKeyValue(QStringList() << "EXP-ID", uniqueID);    // e.g. MCSE00012193
        individualFixDone = true;
    }

    if (individualFixDone) {
        if (!test) {
            emit messageAvailable(baseName + " : Could not determine unambiguous file name!", "error");
            emit critical();
            successProcessing = false;
        }
        else {
            outname = "!"+path+"/"+instData.shortName+"."+filter+"."+uniqueID+"_"+QString::number(chipID)+"P.fits";
        }
    }
}

// Some instrument modes need special treatment
bool Splitter::individualFixWriteImage(int chipMapped)
{
    if (!successProcessing) return;

    bool individualFixDone = false;

    if (instData.name == "LIRIS_POL@WHT") {    // Write the four subregions with the different polarization angles as separate FITS files

        for (int channel=0; channel<=3; ++channel) {
            fitsfile *fptr;
            int status = 0;
            long fpixel = 1;
            // WARNING: If the SIZEx/y vectors in instrument.ini change, we must also change the ijminmax below!
            long nax1 = instData.sizex[channel];
            long nax2 = instData.sizey[channel];
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
            for (long j=0; j<naxis2; ++j) {
                for (long i=0; i<naxis2; ++i) {
                    if (i>=imin && i<=imax && j>=jmin && j<=jmax) {
                        array[k] = dataCurrent[i+naxis1*j];
                        ++k;
                    }
                }
            }

            long naxis = 2;
            long naxes[2] = {nax1, nax2};

            // Replace blanks in file names
            baseName.replace(' ','_');

            // Output file name
            QString outName = "!"+path+"/"+baseName+"_"+channelID+"_1P.fits";
            // If renaming active, and dateobs was determined successfully
            if (cdw->ui->theliRenamingCheckBox->isChecked() && dateObsValue != "2020-01-01T00:00:00.000") {
                outName = "!"+path+"/"+instData.shortName+"."+filter+"_"+channelID+"."+dateObsValue+"_1P.fits";
            }
            fits_create_file(&fptr, outName.toUtf8().data(), &status);
            fits_create_img(fptr, FLOAT_IMG, naxis, naxes, &status);
            fits_write_img(fptr, TFLOAT, fpixel, nelements, array, &status);

            // Propagate header
            for (int i=0; i<headerTHELI.length(); ++i) {
                fits_write_record(fptr, headerTHELI[i].toUtf8().constData(), &status);
            }
            fits_close_file(fptr, &status);

            delete [] array;

            printCfitsioError("writeImage()", status);
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
    // WARNING: POOR TIMIMG! Increment MJD-OBS by 0.1 s per slice
    double mjdobs = 0.;
    fits_read_key_dbl(fptr, "MJD-OBS", &mjdobs, NULL, &status);
    mjdobs += slice*1.157e-6;
    fits_update_key_dbl(fptr, "MJD-OBS", mjdobs, 12, NULL, &status);
    fits_close_file(fptr, &status);

    delete [] array;

    printCfitsioError("writeImageSlice()", status);
}

// UNUSED. Not sure whether I need the MyImages sorted
// Crashes with an out-of-range error on mask->globalMask (or mask->isChipMasked) vector when restoring data from RAWDATA and rerunning.
// Works fine if done the first time.
void Splitter::initMyImage(int chip)
{
    if (!successProcessing) return;

    // TODO: must use chipID instead of "chip", e.g. for FORS2

    MyImage *myImage = new MyImage(path, baseName+"_"+QString::number(chip+1)+"P.fits", "P", chip+1,
                                   mask->globalMask[chip], mask->isChipMasked[chip], verbosity);
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
    myImage->emitModelUpdateNeeded();
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

// Condensed from MyFITS
void Splitter::readExtHeader()
{
    if (rawStatus) return;

    // Read the entire header. This should always work!
    char *fullheader = nullptr;
    int numHeaderKeys = 0;
    fits_hdr2str(rawFptr, TRUE, NULL, 0, &fullheader, &numHeaderKeys, &rawStatus);
    printCfitsioError("readextheader()", rawStatus);
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
    printCfitsioError("readPrimaryHeader()", rawStatus);
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
