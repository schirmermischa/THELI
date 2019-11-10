#include "splitter.h"
#include "tools.h"
#include "../instrumentdata.h"
#include "../myimage/myimage.h"
#include "../dockwidgets/confdockwidget.h"
#include "ui_confdockwidget.h"
#include "../myfits/myfits.h"
#include "../functions.h"
#include "fitsio.h"
#include <QString>
#include <QVector>
#include <QFile>
#include <QDir>


// The functions below make a fork of the overscan / cropping part for detectors with multiple readout channels
// Ultimately, this should become generic enough to also work with "nomal" detectors

/*
void Splitter::pasteMultiPortDataSections(QVector<long> dataSect)
{
    if (!successProcessing) return;

    if (instData.name == "LIRIS_POL@WHT") {
        naxis1 = naxis1Raw;
        naxis2 = naxis2Raw;
        dataCurrent = dataRaw;
        return;
    }

    // The image that enters this function has the same geometry as the raw data, including all overscan areas.

    // New image geometry; here, naxis1/2 refers to the total size of the image (after pasting one or more readout channels)
    // This should match what is given in instrument.ini
    naxis1 = dataSect[1] - dataSect[0] + 1;
    naxis2 = dataSect[3] - dataSect[2] + 1;
    if (*verbosity > 1) emit messageAvailable(baseName + " : Cropping to data section, size = "
                                              + QString::number(naxis1)+"x"+QString::number(naxis2)+" pixel", "image");
    dataCurrent.resize(naxis1*naxis2);

    // Loop over readout channels
    int numChannels = multiportOverscanDirections.length();       // the number of readout channels per detector
    for (int channel=0; channel<numChannels; ++channel) {

    }

    long k = 0;
    for (long j=dataSect[2]; j<=dataSect[3]; ++j) {
        for (long i=dataSect[0]; i<=dataSect[1]; ++i) {
            dataCurrent[k] = dataRaw[i+naxis1Raw*j];
            ++k;
        }
    }
}
*/

/*
void Splitter::computeMultiportDataOffsets()
{
    for (int channel=0; channel<multiportGains.length(); ++channel) {
        long xoffset = multiportDataSections[channel][0] - multiportImageSections[channel][0];
        long yoffset = multiportDataSections[channel][2] - multiportImageSections[channel][2];
    }
}
*/

// paste a subarea 'sector' {xmin, xmax, ymin, ymax) from source image "dataS" to target image "dataT",
// with offsets dx and dy in the target image
void Splitter::pasteSubArea(QVector<float> &dataT, const QVector<float> &dataS, const QVector<long> &sector, const float corrFactor,
                            const long dx, const long dy, const long nT, const long mT, const long nS, const long mS)
{
    long iminS = sector[0];
    long imaxS = sector[1];
    long jminS = sector[2];
    long jmaxS = sector[3];

    for (long jS=jminS; jS<=jmaxS; ++jS) {
        for (long iS=iminS; iS<=imaxS; ++iS) {
            long iT = dx+iS-iminS;
            long jT = dy+jS-jminS;
            if (iT>=nT || iT<0 || jT>=mT || jT<0) continue;   // don't paste pixels falling outside target area
            dataT[iT+nT*jT] = dataS[iS+nS*jS] * corrFactor;   // correcting for small gain differences
        }
    }
}

void Splitter::getMultiportInformation(int chip)
{
    multiportOverscanSections.clear();
    multiportDataSections.clear();
    multiportGains.clear();
    multiportOverscanDirections.clear();
    bool individualFixDone = false;

    naxis1 = dataSection[chip][1] - dataSection[chip][0] + 1;
    naxis2 = dataSection[chip][3] - dataSection[chip][2] + 1;

    if (instData.name == "GROND_OPT@MPGESO") {
        multiportOverscanDirections << "vertical" << "vertical";
        multiportOverscanSections << extractVerticesFromKeyword("BSECA");
        multiportOverscanSections << extractVerticesFromKeyword("BSECB");
        multiportDataSections << extractVerticesFromKeyword("DSECA");
        multiportDataSections << extractVerticesFromKeyword("DSECB");
        multiportImageSections << extractVerticesFromKeyword("CSECA");
        multiportImageSections << extractVerticesFromKeyword("CSECB");

        float gainValue1 = 0.0;
        float gainValue2 = 0.0;
        searchKeyValue(QStringList() << "GAIN", gainValue1);
        // Nominally, the 2 channels in the optical GROND detectors have the same gain, but that's not exactly true
        QString detector = "";
        searchKeyValue(QStringList() << "EXTNAME", detector);
        if (detector == "CCDg") gainValue2 = gainValue1 * 1.06947;
        if (detector == "CCDr") gainValue2 = gainValue1 * 1.00766;
        if (detector == "CCDi") gainValue2 = gainValue1 * 1.00001;
        if (detector == "CCDz") gainValue2 = gainValue1 * 0.88904;
        multiportGains << gainValue1 << gainValue2;
        individualFixDone = true;
    }

    if (multiportGains.length() != multiportOverscanSections.length()
            || multiportGains.length() != multiportDataSections.length()) {
        emit messageAvailable("Splitter::getMultiportInformation : Inconsistent number of channels for gain, overscan and data section!"
                              + QString::number(channelGains.length()) + " "
                              + QString::number(multiportOverscanSections.length()) + " "
                              + QString::number(multiportDataSections.length()), "error");
        emit critical();
        successProcessing = false;
    }

    // What to do for detectors that are not split up by several readout channels and overscans
    if (!individualFixDone) {
        // Append the overscan strips from the instrument.ini files, and padd the missing dimension
        QVector<long> overscan;
        if (!overscanX[chip].isEmpty()) {
            overscan << overscanX[chip][0] << overscanX[chip][1] << 0 << 0;
            multiportOverscanDirections << "vertical";
        }
        if (!overscanY[chip].isEmpty()) {
            overscan << 0 << 0 << overscanY[chip][2] << overscanY[chip][3];
            multiportOverscanDirections << "horizontal";
        }
        multiportOverscanSections << overscan;

        QVector<long> dataSect;
        dataSect << 0 << naxis1Raw -1 << 0 << naxis2Raw - 1;
        multiportDataSections << dataSect;
    }
}

void Splitter::pasteMultiportDataSections(int chip)
{
    // Paste the data sections into a single image of dimensions naxis1, naxis2
    dataCurrent.resize(naxis1*naxis2);
    long k = 0;   // the running 1D index in the pasted image
    int channel = 0;
    // Loop over channels
    for (auto &section : multiportDataSections) {
        if (section.length() != 4) continue; // skip wrong vertices, for whatever reason they might be here
        long xPasteOffset = multiportImageSections[channel][0];
        long yPasteOffset = multiportImageSections[channel][2];
//        long n = multiportImageSections[channel][1] + 1;  // +1 to make it analogous to everywhere else in the code ('effective naxis1')
//        long m = multiportImageSections[channel][2] + 1;  // +1 to make it analogous to everywhere else in the code ('effective naxis2')

        // Here i+j run over the raw data pixel coordinate system
        /*
        long imin = multiportDataSections[channel][0];
        long imax = multiportDataSections[channel][1];  // inclusive
        long jmin = multiportDataSections[channel][2];
        long jmax = multiportDataSections[channel][3];  // inclusive
        for (long j=jmin; j<=jmax; ++j) {
            for (long i=imin; i<=imax; ++i) {
                k = i-imin+xPasteOffset + naxis1*(j-jmin+yPasteOffset);
                dataCurrent[k] = dataRaw[i+naxis1Raw*j] * channelGains[channel];
            }
        }
        */
        pasteSubArea(dataCurrent, dataRaw, multiportDataSections[channel], multiportGains[channel],
                     xPasteOffset, yPasteOffset, naxis1, naxis2, naxis1Raw, naxis2Raw);
        ++channel;
    }
}

// splitting "[xmin:xmax,ymin:ymax]"
QVector<long> Splitter::extractVertices(QString vertexString)
{
    vertexString = vertexString.replace(":"," ");
    vertexString = vertexString.replace(","," ");
    vertexString = vertexString.replace("[","");
    vertexString = vertexString.replace("]","");
    vertexString = vertexString.replace("'","");
    vertexString = vertexString.simplified();
    QStringList list = vertexString.split(" ");
    QVector<long> vertices;
    // for loop also works for one-dimensional vertices
    for (int i=0; i<list.length(); ++i) {
        vertices << list[i].toFloat() - 1;   // The -1 accounts for C++ indexing starting at zero
    }
    return vertices;
}

QVector<long> Splitter::extractVerticesFromKeyword(QString keyword)
{
    QString section = "";
    searchKeyValue(QStringList() << keyword, section);
    return extractVertices(section);
}
