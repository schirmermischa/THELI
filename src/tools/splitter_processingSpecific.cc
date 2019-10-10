#include "splitter.h"
#include "tools.h"
#include "../instrumentdata.h"
#include "../myimage/myimage.h"
#include "../myfits/myfits.h"
#include "../functions.h"

#include "fitsio.h"
#include <QString>
#include <QVector>
#include <QFile>
#include <QDir>

// This source file contains instrument-specific processing routines.
// Mainly to deal with peculiarities of the data

void Splitter::descrambleLiris()
{
    if (!successProcessing) return;

    int n = naxis1;
    int m = naxis2;
    QVector<QVector<float>> image2D(n);
    QVector<QVector<float>> image2Dmod(n);

    int i = 0;
    int j = 0;

    for(i=0; i<n; ++i) {
        image2D[i].fill(0,m);
        image2Dmod[i].fill(0,m);
    }

    // Put the image data into the 2D array
    for (j=0; j<m; ++j) {
        for (i=0; i<n; ++i) {
            image2D[i][j] = dataCurrent[i+n*j];
        }
    }

    // correct 1st quadrant
    // shift lower left corner to the upper right
    image2Dmod[510][511] = image2D[0][0];
    image2Dmod[511][511] = image2D[1][0];
    // shift main block two columns to the left
    for (i=0; i<=509; ++i)  {
        for (j=0; j<=511; ++j)  {
            image2Dmod[i][j] = image2D[i+2][j];
        }
    }
    // shift left two columns to the right and one row down
    for (j=1; j<=511; ++j)  {
        image2Dmod[510][j-1] = image2D[0][j];
        image2Dmod[511][j-1] = image2D[1][j];
    }

    // correct 2nd quadrant
    // shift lower left corner to the upper right
    image2Dmod[1023][511] = image2D[512][0];
    // shift main block one column to the left
    for (i=512; i<=1022; ++i)  {
        for (j=0; j<=511; ++j)  {
            image2Dmod[i][j] = image2D[i+1][j];
        }
    }
    // shift left column to the right and one row down
    for (j=1; j<=511; ++j)  {
        image2Dmod[1023][j-1] = image2D[512][j];
    }

    // correct 3rd quadrant
    // shift lower left corner to the upper right
    image2Dmod[511][1023] = image2D[0][512];
    // shift main block one column to the left
    for (i=0; i<=510; ++i)  {
        for (j=512; j<=1023; ++j)  {
            image2Dmod[i][j] = image2D[i+1][j];
        }
    }
    // shift left column to the right and one row down
    for (j=513; j<=1023; ++j)  {
        image2Dmod[511][j-1] = image2D[0][j];
    }

    // correct 4th quadrant
    // shift lower left corner to the upper right
    image2Dmod[1023][1023] = image2D[512][512];
    // shift main block one column to the left
    for (i=512; i<=1022; ++i)  {
        for (j=513; j<=1023; ++j)  {
            image2Dmod[i][j] = image2D[i+1][j];
        }
    }
    // shift left column to the right and one row down
    for (j=513; j<=1023; ++j)  {
        image2Dmod[1023][j-1] = image2D[512][j];
    }

    // Write the result
    for (j=0; j<m; ++j) {
        for (i=0; i<n; ++i) {
            dataCurrent[i+n*j] = image2Dmod[i][j];
        }
    }
}
