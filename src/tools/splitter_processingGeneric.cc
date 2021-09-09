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
#include "tools.h"
#include "../instrumentdata.h"
#include "../myimage/myimage.h"
#include "../dockwidgets/confdockwidget.h"
#include "ui_confdockwidget.h"
#include "../functions.h"
#include "fitsio.h"
#include <QString>
#include <QVector>
#include <QFile>
#include <QDir>

void Splitter::correctOverscan()
{
    if (!cdw->ui->overscanCheckBox->isChecked()) return;

    if (multiportOverscanDirections.isEmpty()) return;

    int numOverscans = multiportOverscanSections.length();       // the number of overscan sections per detector
    int numDataSections = multiportChannelSections.length();     // the number of data sections per detector. A data section might have a horizontal and a vertical overscan

    // One overscan per data section

    // We keep track of the estimate of the overscan value per detector
    float overscanEffectiveVertical = 0.;
    float overscanEffectiveHorizontal = 0.;
    float overscanEffective = 0.;
    if (numOverscans == numDataSections) {
        for (int i=0; i<numOverscans; ++i) {
            if (multiportOverscanDirections[i] == "vertical") {
                doOverscanVertical(combineOverscan_ptr, multiportOverscanSections[i], multiportChannelSections[i], overscanEffectiveVertical);
            }
            if (multiportOverscanDirections[i] == "horizontal") {
                doOverscanHorizontal(combineOverscan_ptr, multiportOverscanSections[i], multiportChannelSections[i], overscanEffectiveHorizontal);
            }
            overscanEffective += overscanEffectiveVertical + overscanEffectiveHorizontal;
        }
        overscanEffective /= numOverscans;
    }
    // Two overscans per data section, e.g. vertical and horizontal
    else {
        for (int i=0; i<numOverscans; ++i) {
            if (multiportOverscanDirections[i] == "vertical") {
                doOverscanVertical(combineOverscan_ptr, multiportOverscanSections[i], multiportChannelSections[i/2], overscanEffectiveVertical);
            }
            if (multiportOverscanDirections[i] == "horizontal") {
                doOverscanHorizontal(combineOverscan_ptr, multiportOverscanSections[i], multiportChannelSections[i/2], overscanEffectiveHorizontal);
            }
            overscanEffective += overscanEffectiveVertical + overscanEffectiveHorizontal;
        }
        overscanEffective /= numOverscans;
    }

    // Update the saturation value
    //    qDebug() << saturationValue << overscanEffective;
    saturationValue -= overscanEffective;
}

/*
void Splitter::doOverscan(float (*combineFunction_ptr) (const QVector<float> &, const QVector<bool> &, long),
                          const QVector<long> &overscanXArea, const QVector<long> &overscanYArea, const QVector<long> vertices)
{
    if (!successProcessing) return;

    if (!cdw->ui->overscanCheckBox->isChecked()) return;

    // Overscan correction is done only for optical CCDs
    if (instData.type != "OPT") return;

    long i, j, k;
    long n = naxis1Raw;
    long m = naxis2Raw;

    // Overscan correction is applied to a subregion of the detector (including the overscan itself!).
    // If the detector has a single readout port only (or we can ignore them), then the overscan applies to the full array.
    // Otherwise, to the subarray defined by 'vertices'
    long imin;
    long imax;
    long jmin;
    long jmax;
    if (vertices.isEmpty()) {
        imin = 0;
        imax = naxis1Raw;
        jmin = 0;
        jmax = naxis2Raw;
    }
    else {
        imin = vertices[0];
        imax = vertices[1];
        jmin = vertices[2];
        jmax = vertices[3];
    }

    // PART 1: Correct vertical overscan
    if (!overscanXArea.isEmpty()) {
        QVector<float> spectral(overscanXArea[1]-overscanXArea[0]+1,0);
        QVector<float> spatial(m,0);
        // extract representative line
        for (j=0; j<m; ++j) {
            k = 0;
            for (i=overscanXArea[0]; i<=overscanXArea[1]; ++i) {
                spectral[k] = dataRaw[i+n*j];
                ++k;
            }
            spatial[j] = combineFunction_ptr(spectral, QVector<bool>(), 0);
            //            spatialv[j] = straightMedianInline(spectralv);
        }
        if (*verbosity > 1) emit messageAvailable(baseName + " : Mean vertical overscan = " + QString::number(meanMask(spatial), 'f', 3), "image");
        // subtract overscan
        for (i=imin; i<imax; ++i) {
            for (j=jmin; j<jmax; ++j) {
                dataRaw[i+n*j] -= spatial[j];
            }
        }
    }

    // PART 2: Correct horizontal overscan
    if (!overscanYArea.isEmpty()) {
        QVector<float> spectral(overscanYArea[1]-overscanYArea[0]+1,0);
        QVector<float> spatial(n,0);
        // extract representative line
        for (i=0; i<n; ++i) {
            k = 0;
            for (j=overscanYArea[0]; j<=overscanYArea[1]; ++j) {
                spectral[k] = dataRaw[i+n*j];
                ++k;
            }
            spatial[i] = combineFunction_ptr(spectral, QVector<bool>(), 0);
        }
        // subtract overscan
        if (*verbosity > 1) emit messageAvailable(baseName + " : Mean horizontal overscan = " + QString::number(meanMask(spatial), 'f', 3), "image");
        for (j=jmin; j<jmax; ++j) {
            for (i=imin; i<imax; ++i) {
                dataRaw[i+n*j] -= spatial[i];
            }
        }
    }
}
*/

// Correct vertical overscan
void Splitter::doOverscanVertical(float (*combineFunction_ptr) (const QVector<float> &, const QVector<bool> &, long),
                                  const QVector<long> &overscanArea, const QVector<long> dataVertices, float &overscanEstimate)
{
    if (!successProcessing) return;
    if (overscanArea.isEmpty()) return;

    long n = naxis1Raw;
    long m = naxis2Raw;

    overscanEstimate = 0.;

    QVector<float> spectral(overscanArea[1]-overscanArea[0]+1,0);   // select (xmin, xmax) from [xmin:xmax, ymin:ymax]
    QVector<float> spatial(m,0);

    // Line by line correction
    if (combineFunction_ptr != nullptr) {
        // extract representative line
        for (long j=0; j<m; ++j) {
            long k = 0;
            for (long i=overscanArea[0]; i<=overscanArea[1]; ++i) {
                spectral[k] = dataRaw[i+n*j];
                ++k;
            }
            spatial[j] = combineFunction_ptr(spectral, QVector<bool>(), 0);
            //            spatialv[j] = straightMedianInline(spectralv);
        }
        overscanEstimate = meanMask(spatial);
        if (*verbosity > 1) emit messageAvailable(baseName + " : Mean vertical overscan = " + QString::number(overscanEstimate, 'f', 3), "image");
    }
    // Global correction with constant value
    else {
        QVector<float> tmp;
        tmp.reserve((overscanArea[1]-overscanArea[0]+1)*m);
        for (long j=0; j<m; ++j) {
            for (long i=overscanArea[0]; i<=overscanArea[1]; ++i) {
                tmp << dataRaw[i+n*j];
            }
        }
        float constOverscan = straightMedianInline(tmp);
        overscanEstimate = constOverscan;
        if (*verbosity > 1) emit messageAvailable(baseName + " : Global median overscan = " + QString::number(constOverscan, 'f', 1), "image");
        for (long j=0; j<m; ++j) {
            spatial[j] = constOverscan;
        }
    }

    // Overscan correction is applied to a subregion of the detector (including the overscan itself!).
    // If the detector has a single readout port only (or we can ignore the ports), then the overscan applies to the full array.
    long imin = dataVertices[0];
    long imax = dataVertices[1];
    long jmin = dataVertices[2];
    long jmax = dataVertices[3];
    for (long i=imin; i<=imax; ++i) {
        for (long j=jmin; j<=jmax; ++j) {
            dataRaw[i+n*j] -= spatial[j];
        }
    }
}

// Correct horizontal overscan
void Splitter::doOverscanHorizontal(float (*combineFunction_ptr) (const QVector<float> &, const QVector<bool> &, long),
                                    const QVector<long> &overscanArea, const QVector<long> dataVertices, float &overscanEstimate)
{
    if (!successProcessing) return;
    if (overscanArea.isEmpty()) return;

    long n = naxis1Raw;

    overscanEstimate = 0.;

    QVector<float> spectral(overscanArea[3]-overscanArea[2]+1,0);   // select (ymin, ymax) from [xmin:xmax, ymin:ymax]
    QVector<float> spatial(n,0);

    // Line by line correction
    if (combineFunction_ptr != nullptr) {
        // extract representative line
        for (long i=0; i<n; ++i) {
            long k = 0;
            for (long j=overscanArea[2]; j<=overscanArea[3]; ++j) {
                spectral[k] = dataRaw[i+n*j];
                ++k;
            }
            spatial[i] = combineFunction_ptr(spectral, QVector<bool>(), 0);
        }
        overscanEstimate = meanMask(spatial);
        if (*verbosity > 1) emit messageAvailable(baseName + " : Mean horizontal overscan = " + QString::number(meanMask(spatial), 'f', 3), "image");
    }
    // Global correction with constant value
    else {
        QVector<float> tmp;
        tmp.reserve((overscanArea[3]-overscanArea[2]+1)*n);
        for (long i=0; i<n; ++i) {
            for (long j=overscanArea[2]; j<=overscanArea[3]; ++j) {
                tmp << dataRaw[i+n*j];
            }
        }
        float constOverscan = straightMedianInline(tmp);
        overscanEstimate = constOverscan;
        if (*verbosity > 1) emit messageAvailable(baseName + " : Global median overscan = " + QString::number(constOverscan, 'f', 1), "image");
        for (long i=0; i<n; ++i) {
            spatial[i] = constOverscan;
        }
    }

    // Overscan correction is applied to a subregion of the detector (including the overscan itself!).
    // If the detector has a single readout port only (or we can ignore the ports), then the overscan applies to the full array.
    long imin = dataVertices[0];
    long imax = dataVertices[1];
    long jmin = dataVertices[2];
    long jmax = dataVertices[3];
    for (long j=jmin; j<=jmax; ++j) {
        for (long i=imin; i<=imax; ++i) {
            dataRaw[i+n*j] -= spatial[i];
        }
    }
}

void Splitter::applyMask(int chip)
{
    if (!successProcessing) return;

    if (instData.name.contains("DUMMY")) return;

    if (!MEFpastingFinished) return;
    else {
        // Must reset 'chip' such that it counts true CCDs, not the extensions with the individual amplifiers (e.g., for GMOS and SOI)
        chip = chip / numAmpPerChip;
    }

    // Override: PISCO chip can be different than zero (other than grond) because we had to maintain its chip id for the chip-dependent pasting
    if (instData.name == "PISCO@LCO") chip = 0;       // (single detector, effectively)
    if (!checkCorrectMaskSize(chip)) {
        //        QDir unknownFile(path+"/InconsistentGeometry");
        //        unknownFile.mkpath(path+"/InconsistentGeometry/");
        //        moveFile(fileName, path, path+"/InconsistentGeometry/");
        return;
    }

    if (instData.name == "LIRIS_POL@WHT") return;           // Further cut-outs happen in writeImage(); mask geometry in camera.ini applies only once these have happened.
    //    if (instData.name == "GROND_NIR@MPGESO") return;  // Further cut-outs happen in writeImage(); mask geometry in camera.ini applies only once these have happened.
    if (instNameFromData == "GROND_NIR@MPGESO") return;     // Further cut-outs happen in writeImage(); mask geometry in camera.ini applies only once these have happened.

    if (*verbosity > 1) emit messageAvailable(baseName + " : Applying mask ...", "image");

    long i = 0;
    if (!instData.name.contains("GROND")) {
        for (auto &pixel : dataCurrent) {
            if (mask->globalMask[chip].at(i)) pixel = 0.;
            ++i;
        }
    }
    // Resolve a mask conflict (two different masks, but only one instrument can be selected at a time)
    else if (instNameFromData == "GROND_OPT@MPGESO" && instData.name == "GROND_NIR@MPGESO" && altMask != nullptr) {
        for (auto &pixel : dataCurrent) {
            if (altMask->globalMask[chip].at(i)) pixel = 0.;
            ++i;
        }
    }
}

// dataSect contains the exposed (desired) illuminated pixels, [min:max], starting counting at zero,
// correctly converted from the camera.ini CUTON_X and SIZE_X parameterization
void Splitter::cropDataSection(QVector<long> dataSect)
{
    if (!successProcessing) return;

    if (instData.name == "LIRIS_POL@WHT") {
        naxis1 = naxis1Raw;
        naxis2 = naxis2Raw;
        dataCurrent = dataRaw;
        return;
    }

    // new image geometry
    naxis1 = dataSect[1] - dataSect[0] + 1;
    naxis2 = dataSect[3] - dataSect[2] + 1;
    if (*verbosity > 1) emit messageAvailable(baseName + " : Cropping to data section, size = "
                                              + QString::number(naxis1)+"x"+QString::number(naxis2)+" pixel", "image");
    dataCurrent.resize(naxis1*naxis2);
    dataCurrent.squeeze();
    long k = 0;
    for (long j=dataSect[2]; j<=dataSect[3]; ++j) {
        for (long i=dataSect[0]; i<=dataSect[1]; ++i) {
            dataCurrent[k] = dataRaw[i+naxis1Raw*j];
            ++k;
        }
    }

    // TODO
    /*
    int crpix1 = getKeyword("CRPIX1").toFloat() - dataSection[0];
    int crpix2 = getKeyword("CRPIX2").toFloat() - dataSection[2];
    updateKeywordInHeader("CRPIX1", QString::number(crpix1));
    updateKeywordInHeader("CRPIX2", QString::number(crpix2));
    */

    //   successProcessing = true;
}

void Splitter::correctNonlinearity(int chip)
{
    if (!successProcessing) return;

    if (!MEFpastingFinished) return;

    if (!cdw->ui->nonlinearityCheckBox->isChecked()) return;

    // Leave if no correction polynomial is known
    if (nonlinearityCoefficients.isEmpty()) return;

    if (*verbosity > 1) emit messageAvailable(baseName + " : Nonlinearity correction ...", "image");

    // Extract coefficients for the current chip
    QVector<float> coeffs = nonlinearityCoefficients[chip];
    // Apply nonlinearity correction
    for (auto &pixel : dataCurrent) {
        pixel = polynomialSum(pixel, coeffs);
    }
    // Update saturation value
    saturationValue = polynomialSum(saturationValue, coeffs);

    // Individual implementations

    // Average nonlinearity correction
    // http://instrumentation.obs.carnegiescience.edu/FourStar/Documents/FourStar_Documentation.pdf
    if (instData.name == "FourStar@LCO") {
        float A = 0.;
        // High gain
        if (channelGains[0] > 2.) {
            if (chip == 0) A = 1.203e-8;
            if (chip == 1) A = 1.337e-8;
            if (chip == 2) A = 1.157e-8;
            if (chip == 3) A = 1.195e-8;
        }
        // Low gain
        else {
            if (chip == 0) A = 5.344e-9;
            if (chip == 1) A = 7.514e-9;
            if (chip == 2) A = 3.645e-9;
            if (chip == 3) A = 5.923e-9;
        }
        for (auto &pixel : dataCurrent) {
            pixel = pixel * (1.+A*pow(pixel,1.5));
        }
        saturationValue *= (1.+A*pow(saturationValue,1.5));
    }

    // http://csp2.lco.cl/manuals/dup_dc_linearity.php
    if (instData.name == "Direct_2k_DUPONT@LCO") {
        QString speed = "";
        searchKeyValue(headerDictionary.value("SPEED"), speed);

        float c1 = 1.0;
        float c2 = 0.0;
        float c3 = 0.0;
        float c4 = 0.0;
        if (speed == "Turbo") {
            c2 = 0.09907;
            c3 = -0.06151;
            c4 = 0.01522;
        }
        if (speed == "Fast") {
            c2 = 0.05022;
            c3 = -0.02814;
            c4 = 0.00733;
        }
        for (auto &pixel : dataCurrent) {
            float x = c1 + c2*(pixel/32000.0) + c3*pow(pixel/32000.0,2) + c4*pow(pixel/32000.0,3);
            pixel /= x;
        }
        float x = c1 + c2*(saturationValue/32000.0) + c3*pow(saturationValue/32000.0,2) + c4*pow(saturationValue/32000.0,3);
        saturationValue /= x;
    }

    // http://csp2.lco.cl/manuals/swo_nc_linearity.php (using mean values)
    // NOTE: linearity test page does not specify for which readout mode it applies
    if (instData.name == "Direct_4k_SWOPE@LCO") {
        float c1 = 1.0;
        float c2 = 0.0;
        float c3 = 0.0;
        if (chip == 0) {
            c2 = -0.03443;
            c3 = 0.006657;
        }
        if (chip == 1) {
            c2 = -0.01557;
            c3 = 0.003386;
        }
        if (chip == 2) {
            c2 = -0.01329;
            c3 = 0.003314;
        }
        if (chip == 3) {
            c2 = -0.01971;
            c3 = 0.004700;
        }
        for (auto &pixel : dataCurrent) {
            float x = c1 + c2*(pixel/32000.0) + c3*pow(pixel/32000.0,2);
            pixel /= x;
        }
        float x = c1 + c2*(saturationValue/32000.0) + c3*pow(saturationValue/32000.0,2);
        saturationValue /= x;
    }
}

void Splitter::convertToElectrons(int chip)
{
    if (!successProcessing) return;

    if (!MEFpastingFinished) return;

    // Update saturation value          // done in buildTheliHeaderGAIN(int chip)
    // saturationValue *= gain[chip];

    // cameras with multiple readout channels AND different gain per channel (but no overscan pasted into the data section)
    // WARNING: ASSUMPTIONS: applies to the size defined in camera.ini
    if (numReadoutChannels > 1) {
        //        qDebug() << channelGains;
        if (instData.name == "FORS1_199904-200703@VLT" || instData.name == "FORS2_200004-200203@VLT") {
            for (long j=0; j<naxis2; ++j) {
                for (long i=0; i<naxis1; ++i) {
                    auto &pixel = dataCurrent[i+naxis1*j];
                    if (i<=1022 && j<=1023) pixel *= channelGains[0];   // lower left quadrant
                    if (i>=1023 && j<=1023) pixel *= channelGains[1];   // lower right quadrant
                    if (i<=1022 && j>=1024) pixel *= channelGains[2];   // upper left quadrant
                    if (i>=1023 && j>=1024) pixel *= channelGains[3];   // upper right quadrant
                }
            }
            gainForSaturation = minVec_T(channelGains);    // SATURATE keyword shall reflect lowest number at which saturation occurs
        }
    }

    //    if (instData.name == "GROND_NIR@MPGESO") {
    if (instNameFromData == "GROND_NIR@MPGESO") {
        gain[chip] = 1.0;   // Gain is fixed in writeImageIndividual();
        saturationValue *= gainForSaturation;
        return;
    }
    if (instData.name == "SuprimeCam_200808-201705@SUBARU") {
        saturationValue *= gainForSaturation;
        return;
    }
    if (multiChannelMultiExt.contains(instData.name)) {
        // GMOS, SOI, MOSAIC-II_16@CTIO, etc
        saturationValue *= gainForSaturation;
        return;
    }

    saturationValue *= gainForSaturation;

    for (auto &pixel : dataCurrent) {
        pixel *= gain[chip];
    }
}

// Calculate the effective gain for a detector with different readout channels.
// When we divide the data by the flat field, the effective gain is the harmonic mean
float Splitter::harmonicGain(QVector<float> detectorGains)
{
    float harmGain = 0.;
    for (auto &gain : detectorGains) {
        harmGain += 1./gain;
    }
    harmGain = float(detectorGains.length()) / harmGain;

    return harmGain;
}

float Splitter::polynomialSum(float x, QVector<float> coefficients)
{
    // Coefficients contains the coeffs of a polynomial, starting with the lowest term
    // e.g., for p(x) = c0 + c1*x + c2*x*x  we have coefficients = [c0, c1, c2]
    float sum = 0.;
    int k = 0;
    for (auto & it : coefficients) {
        sum += it * pow(x, k);
        ++k;
    }
    return sum;
}

// IMPORTANT: xtalk correction assumes NO inter-chip crosstalk, and that the amplitudes are the same for all intra-chip crosstalk

int Splitter::getNorXtalkMethod()
{
    int xtalkMethod = -1;
    if (cdw->ui->normalxtalkCheckBox->isChecked()) {
        if (cdw->ui->xtalk_nor_2x2ToolButton->isChecked()) xtalkMethod = 1;
        if (cdw->ui->xtalk_nor_1x2ToolButton->isChecked()) xtalkMethod = 2;
        if (cdw->ui->xtalk_nor_2x1ToolButton->isChecked()) xtalkMethod = 3;
    }
    return xtalkMethod;
}

int Splitter::getRowXtalkMethod()
{
    int xtalkMethod = -1.;
    if (cdw->ui->rowxtalkCheckBox->isChecked()) {
        if (cdw->ui->xtalk_row_2x2ToolButton->isChecked()) xtalkMethod = 4;
        if (cdw->ui->xtalk_row_1x2ToolButton->isChecked()) xtalkMethod = 5;
        if (cdw->ui->xtalk_row_2x1ToolButton->isChecked()) xtalkMethod = 6;
        if (cdw->ui->xtalk_col_2x2ToolButton->isChecked()) xtalkMethod = 7;
        if (cdw->ui->xtalk_col_1x2ToolButton->isChecked()) xtalkMethod = 8;
        if (cdw->ui->xtalk_col_2x1ToolButton->isChecked()) xtalkMethod = 9;
    }
    return xtalkMethod;
}

void Splitter::correctXtalk()
{
    if (!successProcessing) return;

    if (!MEFpastingFinished) return;

    // Xtalk type
    bool doXtalkNor = cdw->ui->normalxtalkCheckBox->isChecked();
    bool doXtalkRow = cdw->ui->rowxtalkCheckBox->isChecked();
    if (!doXtalkNor && !doXtalkRow) return;

    // Xtalk Amplitudes
    xtalkNorAmpString = cdw->ui->normalxtalkAmplitudeLineEdit->text();
    xtalkRowAmpString = cdw->ui->rowxtalkAmplitudeLineEdit->text();
    if (xtalkNorAmpString.isEmpty() && xtalkRowAmpString.isEmpty()) return;

    if ( (doXtalkNor && xtalkNorAmpString.isEmpty())
         || ((doXtalkRow && xtalkRowAmpString.isEmpty()))) {
        emit messageAvailable("Incomplete cross-talk input. The cross-talk type must be checked as well as an amplitude given.", "warning");
        emit messageAvailable("Cross-talk correction will not take place", "warning");
        emit warning();
        return;
    }

    // Ok, proceed with xtalk correction
    xtalkNorAmp = xtalkNorAmpString.toFloat();
    xtalkRowAmp = xtalkRowAmpString.toFloat();
    xtalkKappa = 1e6;   // No rejection in x-talk correction when doing collapse correction
    // Holds the pixel corrected data
    dataXcorr.fill(0, naxis1*naxis2);

    // Correct point xtalk
    xtalkNorMethod = getNorXtalkMethod();
    if (xtalkNorMethod != -1 && *verbosity > 1) emit messageAvailable(baseName + " : Normal crosstalk correction ...", "image");
    if (xtalkNorMethod == 1) xtalk_method1();        // 2x2 quadrants, point xtalk
    else if (xtalkNorMethod == 2) xtalk_method2();   // 1x2 quadrants, point xtalk, top half bottom half
    else if (xtalkNorMethod == 3) xtalk_method3();   // 2x1 quadrants, point xtalk, left half right half
    // Make the corrected data the current data
    if (xtalkNorMethod != -1) dataCurrent.swap(dataXcorr);

    // Correct row xtalk
    xtalkRowMethod = getRowXtalkMethod();
    if (xtalkRowMethod != -1 && *verbosity > 1) emit messageAvailable(baseName + " : Row crosstalk correction ...", "image");
    if (xtalkRowMethod == 4) xtalk_method4();        // 2x2 quadrants, row xtalk
    else if (xtalkRowMethod == 5) xtalk_method5();   // 1x2 quadrants, row xtalk, top half bottom half
    else if (xtalkRowMethod == 6) xtalk_method6();   // 2x1 quadrants, row xtalk, left half right half
    else if (xtalkRowMethod == 7) xtalk_method7();   // 2x2 quadrants, col xtalk
    else if (xtalkRowMethod == 8) xtalk_method8();   // 1x2 quadrants, col xtalk, top half bottom half
    else if (xtalkRowMethod == 9) xtalk_method9();   // 2x1 quadrants, col xtalk, left half right half
    // Make the corrected data the current data
    if (xtalkRowMethod != -1) dataCurrent.swap(dataXcorr);

    dataXcorr.clear();
    dataXcorr.squeeze();

    // if (method == 10) xtalk_method10();    // Multi-xtalk, HAWAII-2RGs (rudimentary implementation, not supported by the GUI)
}

// The crosstalk images. We subtract from each quadrant the other quadrants, scaled by the xtalk amplitude.
// The amplitude is assumed to be the same for all quadrants
void Splitter::xtalk_method1()
{
    int n = naxis1;
    int m = naxis2;
    long i, j, ih, jh, il, jl;

    int xlow1  = 0;       // 1      for a 1024x1024 chip
    int xlow2  = n/2-1;   // 512
    int xhigh1 = n/2;     // 513
    int xhigh2 = n-1;     // 1024
    int ylow1  = 0;       // 1
    int ylow2  = m/2-1;   // 512
    int yhigh1 = m/2;     // 513
    int yhigh2 = m-1;     // 1024

    for (j=0; j<m; ++j) {
        for (i=0; i<n; ++i) {
            ih = i + xhigh1;
            jh = j + yhigh1;
            il = i - xhigh1;
            jl = j - yhigh1;

            // ll = lr + ul + ur
            if (i>=xlow1 && i<=xlow2 && j>=ylow1 && j<=ylow2)
                dataXcorr[i+n*j] = dataCurrent[i+n*j] - xtalkNorAmp * (dataCurrent[ih+n*j] + dataCurrent[i+n*jh] + dataCurrent[ih+n*jh]);
            // lr = ll + ul + ur
            if (i>=xhigh1 && i<=xhigh2 && j>=ylow1 && j<=ylow2)
                dataXcorr[i+n*j] = dataCurrent[i+n*j] - xtalkNorAmp * (dataCurrent[il+n*j] + dataCurrent[il+n*jh] + dataCurrent[i+n*jh]);
            // ul = lr + ll + ur
            if (i>=xlow1 && i<=xlow2 && j>=yhigh1 && j<=yhigh2)
                dataXcorr[i+n*j] = dataCurrent[i+n*j] - xtalkNorAmp * (dataCurrent[ih+n*jl] + dataCurrent[i+n*jl] + dataCurrent[ih+n*j]);
            // ur = lr + ul + ll
            if (i>=xhigh1 && i<=xhigh2 && j>=yhigh1 && j<=yhigh2)
                dataXcorr[i+n*j] = dataCurrent[i+n*j] - xtalkNorAmp * (dataCurrent[i+n*jl] + dataCurrent[il+n*j] + dataCurrent[il+n*jl]);
        }
    }
}

void Splitter::xtalk_method2()
{
    int n = naxis1;
    int m = naxis2;
    long i, j, jh, jl;

    int xlow1  = 0;       // 1
    int xhigh1 = n-1;     // 1024
    int ylow1  = 0;       // 1
    int ylow2  = m/2;     // 513
    int yhigh1 = m/2-1;   // 512
    int yhigh2 = m-1;     // 1024

    for (j=0; j<m; ++j) {
        for (i=0; i<n; ++i) {
            jh = j + ylow2;
            jl = j - ylow2;
            // fix lower quadrant
            if (i>=xlow1 && i<=xhigh1 && j>=ylow1 && j<=yhigh1)
                dataXcorr[i+n*j] = dataCurrent[i+n*j] - xtalkNorAmp * dataCurrent[i+n*jh];
            // fix upper quadrant
            if (i>=xlow1 && i<=xhigh1 && j>=ylow2 && j<=yhigh2)
                dataXcorr[i+n*j] = dataCurrent[i+n*j] - xtalkNorAmp * dataCurrent[i+n*jl];
        }
    }
}

void Splitter::xtalk_method3()
{
    int n = naxis1;
    int m = naxis2;
    long i, j, ih, il;
    int xlow1  = 0;       // 1
    int xlow2  = n/2;     // 513
    int xhigh1 = n/2-1;   // 512
    int xhigh2 = n-1;     // 1024
    int ylow1  = 0;       // 1
    int yhigh1 = m-1;     // 1024

    for (j=0; j<m; ++j) {
        for (i=0; i<n; ++i) {
            ih = i + xlow2;
            il = i - xlow2;
            // fix left quadrant
            if (i>=xlow1 && i<=xhigh1 && j>=ylow1 && j<=yhigh1)
                dataXcorr[i+n*j] = dataCurrent[i+n*j] - xtalkNorAmp * dataCurrent[ih+n*j];
            // fix right quadrant
            if (i>=xlow2 && i<=xhigh2 && j>=ylow1 && j<=yhigh1)
                dataXcorr[i+n*j] = dataCurrent[i+n*j] - xtalkNorAmp * dataCurrent[il+n*j];
        }
    }
}

void Splitter::xtalk_method4()
{
    int n = naxis1;
    int m = naxis2;
    long i, j, ih, jh, t;

    int xlow2 = n/2;     // 513
    int ylow2 = m/2;     // 513

    QVector<float> ll_input(n/2*m/2,0);
    QVector<float> lr_input(n/2*m/2,0);
    QVector<float> ul_input(n/2*m/2,0);
    QVector<float> ur_input(n/2*m/2,0);

    t = 0;
    for (j=0; j<m/2; ++j) {
        for (i=0; i<n/2; ++i) {
            ll_input[t++] = dataCurrent[i+n*j];
        }
    }
    t = 0;
    for (j=0; j<m/2; ++j) {
        for (i=n/2; i<n; ++i) {
            lr_input[t++] = dataCurrent[i+n*j];
        }
    }
    t = 0;
    for (j=m/2; j<m; ++j) {
        for (i=0; i<n/2; ++i) {
            ul_input[t++] = dataCurrent[i+n*j];
        }
    }
    t = 0;
    for (j=m/2; j<m; ++j) {
        for (i=n/2; i<n; ++i) {
            ur_input[t++] = dataCurrent[i+n*j];
        }
    }

    QVector<bool> mask(n/2*m/2,false);
    QVector<float> ll_mean = collapse_x(ll_input, mask, mask, xtalkKappa, n/2, m/2, "2Dmodel");
    QVector<float> lr_mean = collapse_x(lr_input, mask, mask, xtalkKappa, n/2, m/2, "2Dmodel");
    QVector<float> ul_mean = collapse_x(ul_input, mask, mask, xtalkKappa, n/2, m/2, "2Dmodel");
    QVector<float> ur_mean = collapse_x(ur_input, mask, mask, xtalkKappa, n/2, m/2, "2Dmodel");

    // ll = ll - xtalkRowAmp * mean(ll+lr+ul+ur)
    for (j=0; j<m/2; ++j) {
        for (i=0; i<n/2; ++i) {
            dataXcorr[i+n*j] = dataCurrent[i+n*j] -
                    xtalkRowAmp * (ll_mean[i+n/2*j] + lr_mean[i+n/2*j] + ul_mean[i+n/2*j] + ur_mean[i+n/2*j]);
        }
    }

    // lr = lr - xtalkRowAmp * mean(lr+ll+ul+ur)
    for (j=0; j<m/2; ++j) {
        for (i=0; i<n/2; ++i) {
            ih = i + xlow2;
            dataXcorr[ih+n*j] = dataCurrent[ih+n*j] -
                    xtalkRowAmp * (ll_mean[i+n/2*j] + lr_mean[i+n/2*j] + ul_mean[i+n/2*j] + ur_mean[i+n/2*j]);
        }
    }

    // ul = ul - xtalkRowAmp * mean(ul+lr+ll+ur)
    for (j=0; j<m/2; ++j) {
        for (i=0; i<n/2; ++i) {
            jh = j + ylow2;
            dataXcorr[i+n*jh] = dataCurrent[i+n*jh] -
                    xtalkRowAmp * (ll_mean[i+n/2*j] + lr_mean[i+n/2*j] + ul_mean[i+n/2*j] + ur_mean[i+n/2*j]);
        }
    }

    // ur = ur - xtalkRowAmp * mean(ur+lr+ul+ll)
    for (j=0; j<m/2; ++j) {
        for (i=0; i<n/2; ++i) {
            ih = i + xlow2;
            jh = j + ylow2;
            dataXcorr[ih+n*jh] = dataCurrent[ih+n*jh] -
                    xtalkRowAmp * (ll_mean[i+n/2*j] + lr_mean[i+n/2*j] + ul_mean[i+n/2*j] + ur_mean[i+n/2*j]);
        }
    }

}

void Splitter::xtalk_method5()
{
    int n = naxis1;
    int m = naxis2;
    long i, j, jh, t;

    int ylow2 = m/2;     // 513

    QVector<float> bo_input(n*m/2,0);
    QVector<float> up_input(n*m/2,0);

    t = 0;
    for (j=0; j<m/2; ++j) {
        for (i=0; i<n; ++i) {
            bo_input[t++] = dataCurrent[i+n*j];
        }
    }
    t = 0;
    for (j=m/2; j<m; ++j) {
        for (i=0; i<n; ++i) {
            up_input[t++] = dataCurrent[i+n*j];
        }
    }

    QVector<bool> maskLocal(n*m/2,false);
    QVector<float> bo_mean = collapse_x(bo_input, maskLocal, maskLocal, xtalkKappa, n, m/2, "2Dmodel");
    QVector<float> up_mean = collapse_x(up_input, maskLocal, maskLocal, xtalkKappa, n, m/2, "2Dmodel");

    // bo = bo - xtalkRowAmp * mean(bo+up)
    for (j=0; j<m/2; ++j) {
        for (i=0; i<n; ++i) {
            dataXcorr[i+n*j] = dataCurrent[i+n*j] - xtalkRowAmp * (bo_mean[i+n*j] + up_mean[i+n*j]);
        }
    }

    // up = up - xtalkRowAmp * mean(bo+up)
    for (j=0; j<m/2; ++j) {
        for (i=0; i<n; ++i) {
            jh = j + ylow2;
            dataXcorr[i+n*jh] = dataCurrent[i+n*jh] - xtalkRowAmp * (bo_mean[i+n*j] + up_mean[i+n*j]);
        }
    }
}

void Splitter::xtalk_method6()
{
    int n = naxis1;
    int m = naxis2;
    long i, j, ih, t;

    int xlow2 = n/2;     // 513

    QVector<float> le_input(n/2*m,0);
    QVector<float> ri_input(n/2*m,0);

    t = 0;
    for (j=0; j<m; ++j) {
        for (i=0; i<n/2; ++i) {
            le_input[t++] = dataCurrent[i+n*j];
        }
    }
    t = 0;
    for (j=0; j<m; ++j) {
        for (i=n/2; i<n; ++i) {
            ri_input[t++] = dataCurrent[i+n*j];
        }
    }

    QVector<bool> maskLocal(n/2*m,false);
    QVector<float> le_mean = collapse_x(le_input, maskLocal, maskLocal, xtalkKappa, n/2, m, "2Dmodel");
    QVector<float> ri_mean = collapse_x(ri_input, maskLocal, maskLocal, xtalkKappa, n/2, m, "2Dmodel");

    // le = le - xtalkRowAmp * mean(le+ri)
    for (j=0; j<m; ++j) {
        for (i=0; i<n/2; ++i) {
            dataXcorr[i+n*j] = dataCurrent[i+n*j] - xtalkRowAmp * (le_mean[i+n/2*j] + ri_mean[i+n/2*j]);
        }
    }

    // ri = ri - xtalkRowAmp * mean(le+ri)
    for (j=0; j<m; ++j) {
        for (i=0; i<n/2; ++i) {
            ih = i + xlow2;
            dataXcorr[ih+n*j] = dataCurrent[ih+n*j] - xtalkRowAmp * (le_mean[i+n/2*j] + ri_mean[i+n/2*j]);
        }
    }
}

void Splitter::xtalk_method7()
{
    int n = naxis1;
    int m = naxis2;
    long i, j, ih, jh, t;

    int xlow2 = n/2;     // 513
    int ylow2 = m/2;     // 513

    QVector<float> ll_input(n/2*m/2,0);
    QVector<float> lr_input(n/2*m/2,0);
    QVector<float> ul_input(n/2*m/2,0);
    QVector<float> ur_input(n/2*m/2,0);

    t = 0;
    for (j=0; j<m/2; ++j) {
        for (i=0; i<n/2; ++i) {
            ll_input[t++] = dataCurrent[i+n*j];
        }
    }
    t = 0;
    for (j=0; j<m/2; ++j) {
        for (i=n/2; i<n; ++i) {
            lr_input[t++] = dataCurrent[i+n*j];
        }
    }
    t = 0;
    for (j=m/2; j<m; ++j) {
        for (i=0; i<n/2; ++i) {
            ul_input[t++] = dataCurrent[i+n*j];
        }
    }
    t = 0;
    for (j=m/2; j<m; ++j) {
        for (i=n/2; i<n; ++i) {
            ur_input[t++] = dataCurrent[i+n*j];
        }
    }

    QVector<bool> maskLocal(n/2*m/2,false);
    QVector<float> ll_mean = collapse_y(ll_input, maskLocal, maskLocal, xtalkKappa, n/2, m/2, "2Dmodel");
    QVector<float> lr_mean = collapse_y(lr_input, maskLocal, maskLocal, xtalkKappa, n/2, m/2, "2Dmodel");
    QVector<float> ul_mean = collapse_y(ul_input, maskLocal, maskLocal, xtalkKappa, n/2, m/2, "2Dmodel");
    QVector<float> ur_mean = collapse_y(ur_input, maskLocal, maskLocal, xtalkKappa, n/2, m/2, "2Dmodel");

    // ll = ll - xtalkRowAmp * mean(ll+lr+ul+ur)
    for (j=0; j<m/2; ++j) {
        for (i=0; i<n/2; ++i) {
            dataXcorr[i+n*j] = dataCurrent[i+n*j] -
                    xtalkRowAmp * (ll_mean[i+n/2*j] + lr_mean[i+n/2*j] + ul_mean[i+n/2*j] + ur_mean[i+n/2*j]);
        }
    }

    // lr = lr - xtalkRowAmp * mean(lr+ll+ul+ur)
    for (j=0; j<m/2; ++j) {
        for (i=0; i<n/2; ++i) {
            ih = i + xlow2;
            dataXcorr[ih+n*j] = dataCurrent[ih+n*j] -
                    xtalkRowAmp * (ll_mean[i+n/2*j] + lr_mean[i+n/2*j] + ul_mean[i+n/2*j] + ur_mean[i+n/2*j]);
        }
    }

    // ul = ul - xtalkRowAmp * mean(ul+lr+ll+ur)
    for (j=0; j<m/2; ++j) {
        for (i=0; i<n/2; ++i) {
            jh = j + ylow2;
            dataXcorr[i+n*jh] = dataCurrent[i+n*jh] -
                    xtalkRowAmp * (ll_mean[i+n/2*j] + lr_mean[i+n/2*j] + ul_mean[i+n/2*j] + ur_mean[i+n/2*j]);
        }
    }

    // ur = ur - xtalkRowAmp * mean(ur+lr+ul+ll)
    for (j=0; j<m/2; ++j) {
        for (i=0; i<n/2; ++i) {
            ih = i + xlow2;
            jh = j + ylow2;
            dataXcorr[ih+n*jh] = dataCurrent[ih+n*jh] -
                    xtalkRowAmp * (ll_mean[i+n/2*j] + lr_mean[i+n/2*j] + ul_mean[i+n/2*j] + ur_mean[i+n/2*j]);
        }
    }
}

void Splitter::xtalk_method8()
{
    int n = naxis1;
    int m = naxis2;
    long i, j, jh, t;

    int ylow2 = m/2;     // 513

    QVector<float> bo_input(n*m/2,0);
    QVector<float> up_input(n*m/2,0);

    t = 0;
    for (j=0; j<m/2; ++j) {
        for (i=0; i<n; ++i) {
            bo_input[t++] = dataCurrent[i+n*j];
        }
    }
    t = 0;
    for (j=m/2; j<m; ++j) {
        for (i=0; i<n; ++i) {
            up_input[t++] = dataCurrent[i+n*j];
        }
    }

    QVector<bool> maskLocal(n*m/2,false);
    QVector<float> bo_mean = collapse_y(bo_input, maskLocal, maskLocal, xtalkKappa, n, m/2, "2Dmodel");
    QVector<float> up_mean = collapse_y(up_input, maskLocal, maskLocal, xtalkKappa, n, m/2, "2Dmodel");

    // bo = bo - xtalkRowAmp * mean(bo+up)
    for (j=0; j<m/2; ++j) {
        for (i=0; i<n; ++i) {
            dataXcorr[i+n*j] = dataCurrent[i+n*j] - xtalkRowAmp * (bo_mean[i+n*j] + up_mean[i+n*j]);
        }
    }

    // up = up - xtalkRowAmp * mean(bo+up)
    for (j=0; j<m/2; ++j) {
        for (i=0; i<n; ++i) {
            jh = j + ylow2;
            dataXcorr[i+n*jh] = dataCurrent[i+n*jh] - xtalkRowAmp * (bo_mean[i+n*j] + up_mean[i+n*j]);
        }
    }
}

void Splitter::xtalk_method9()
{
    int n = naxis1;
    int m = naxis2;
    long i, j, ih, t;

    int xlow2  = n/2;     // 513

    QVector<float> le_input(n/2*m,0);
    QVector<float> ri_input(n/2*m,0);

    t = 0;
    for (j=0; j<m; ++j) {
        for (i=0; i<n/2; ++i) {
            le_input[t++] = dataCurrent[i+n*j];
        }
    }
    t = 0;
    for (j=0; j<m; ++j) {
        for (i=n/2; i<n; ++i) {
            ri_input[t++] = dataCurrent[i+n*j];
        }
    }

    QVector<bool> maskLocal(n/2*m,false);
    QVector<float> le_mean = collapse_y(le_input, maskLocal, maskLocal, xtalkKappa, n/2, m, "2Dmodel");
    QVector<float> ri_mean = collapse_y(ri_input, maskLocal, maskLocal, xtalkKappa, n/2, m, "2Dmodel");

    // le = le - xtalkRowAmp * mean(le+ri)
    for (j=0; j<m; ++j) {
        for (i=0; i<n/2; ++i) {
            dataXcorr[i+n*j] = dataCurrent[i+n*j] - xtalkRowAmp * (le_mean[i+n/2*j] + ri_mean[i+n/2*j]);
        }
    }

    // ri = ri - xtalkRowAmp * mean(le+ri)
    for (j=0; j<m; ++j) {
        for (i=0; i<n/2; ++i) {
            ih = i + xlow2;
            dataXcorr[ih+n*j] = dataCurrent[ih+n*j] - xtalkRowAmp * (le_mean[i+n/2*j] + ri_mean[i+n/2*j]);
        }
    }

}

// For the typical point cross talk in HAWAII-2RGs (difficult to correct, not offered in the GUI)
void Splitter::xtalk_method10()
{
    if (xtalkDirection.isEmpty() || xtalkNsection < 0) {
        emit messageAvailable("For crosstalk mode 10 you must specify the readout direction and the number of readout sections!", "error");
        emit critical();
        return;
    }

    if (xtalkNsection < 2) {
        emit messageAvailable("xtalk(): The number of readout sections must be larger than 1.\n", "error");
        emit critical();
        return;
    }

    int n = naxis1;
    int m = naxis2;
    long i, j, k, l;
    int width;

    float medianval;
    // allocate memory
    QVector<float> h2_median(n*m,0);
    QVector<float> h2_sample(xtalkNsection,0);

    if (xtalkDirection.compare("x") == 0) {
        width = m/xtalkNsection;
        for (j=0; j<width; ++j) {
            for (i=0; i<n; ++i) {
                l = 0;
                for (k=0; k<xtalkNsection; k++) {
                    h2_sample[l++] = dataCurrent[i+n*(j+width*k)];
                }
                medianval = straightMedian_T(h2_sample);
                for (k=0; k<xtalkNsection; k++) {
                    h2_median[i+n*(j+width*k)] = medianval;
                }
            }
        }
    }

    if (xtalkDirection.compare("y") == 0) {
        width = n/xtalkNsection;
        for (j=0; j<m; ++j) {
            for (i=0; i<width; ++i) {
                l = 0;
                for (k=0; k<xtalkNsection; k++) {
                    h2_sample[l++] = dataCurrent[i+width*k+n*j];
                }
                medianval = straightMedian_T(h2_sample);
                for (k=0; k<xtalkNsection; k++) {
                    h2_median[i+width*k+n*j] = medianval;
                }
            }
        }
    }

    for (k=0; k<n*m; k++) {
        dataXcorr[k] = dataCurrent[k] - h2_median[k];
    }
}
