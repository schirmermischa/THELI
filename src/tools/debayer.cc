#include "tools.h"
#include "../myimage/myimage.h"
#include <QDebug>

// For debayering
float hue_transit(float l1, float l2, float l3, float v1, float v3)
{
    //printf("hue_transit: l1 %5.1f l2 %5.1f l3 %5.1f v1 %5.1f v3 %5.1f\n",l1,l2,l3,v1,v3);
    if((l1<l2 && l2<l3) || (l1> l2 && l2 > l3))
        return(v1+(v3-v1) * (l2-l1)/(l3-l1));
    else
        return((v1+v3)/2.0+(l2*2.0-l1-l3)/4.0);
}

// For debayering
int direction(float N, float E, float W, float S)
{
    if (N < E && W < S) {
        if ( N < W) return 1;
        else return 3;
    }
    else {
        if (E < S) return 2;
        else return 4;
    }
}

// Thx @Carsten Moos for initial coding.
// PPG algorithm, https://sites.google.com/site/chklin/demosaic  (Chuan-Kai Lin)
// The input image becomes the R-band channel
void debayer(int chip, MyImage *image, MyImage *imageR, MyImage *imageG, MyImage *imageB)
{
    if (!image->successProcessing) return;

    QString pattern = image->getKeyword("BAYERPAT");
    if ( pattern != "RGGB"
         && pattern != "GRBG"
         && pattern != "GBRG"
         && pattern != "BGGR") {
        qDebug() << "Tools:: debayer(): Bayer pattern not recognised. Nothing will be done.\n";
        image->successProcessing = false;
        return;
    }

    // chop the last row / column of pixels if the image dimensions are uneven
    int n = image->naxis1;
    int m = image->naxis2;
    if ( n % 2 != 0) n = n - 1;
    if ( m % 2 != 0) m = m - 1;

    // Setup the debayered channels
    QList<MyImage*> list;
    list << imageR << imageG << imageB;
    double mjdOffset = 0.;
    for (auto &it: list) {
        it->naxis1 = n;
        it->naxis2 = m;
        it->dataCurrent.resize(n*m);
        it->path = image->path;
        it->baseName = image->rootName;
        it->chipName = image->rootName+"_"+QString::number(image->chipNumber);
        it->exptime = image->exptime;
        it->header = image->header;
        it->pathBackupL1 = image->pathBackupL1;
        it->baseNameBackupL1 = image->baseNameBackupL1;
        it->imageInMemory = true;
        // Data::populateExposureList() will group debayered images into one exposure, and then
        // mergeScampCatalogs() will create three extensions. But we need them individually.
        // Hence introducing a 0.1 s offset in MJD-OBS.
        // Fixing Data::populateExposureList() is non-trivial
        it->mjdobs = image->mjdobs + mjdOffset;
        mjdOffset += 1.e-6;
    }
    imageR->baseName.append("_R_"+QString::number(chip+1));  // status 'PA' will be appended externally
    imageG->baseName.append("_G_"+QString::number(chip+1));
    imageB->baseName.append("_B_"+QString::number(chip+1));
    imageR->rootName.append("_R");
    imageG->rootName.append("_G");
    imageB->rootName.append("_B");
    imageR->chipName = imageR->baseName;
    imageG->chipName = imageG->baseName;
    imageB->chipName = imageB->baseName;
    imageR->filter = "R";
    imageG->filter = "G";
    imageB->filter = "B";

    // cut all patterns to RGGB
    int xoffset;
    int yoffset;
    if (pattern == "RGGB") {
        xoffset = 0;
        yoffset = 0;
    }
    else if (pattern == "GRBG") {
        xoffset = 1;
        yoffset = 0;
    }
    else if (pattern == "GBRG") {
        xoffset = 0;
        yoffset = 1;
    }
    else {
        // pattern == "BGGR"
        xoffset = 1;
        yoffset = 1;
    }

    /* // The Bayer pattern looks like this
      RGRGRGRGR
      gBgBgBgBg
      RGRGRGRGR
      gBgBgBgBg
    */

    imageR->dataCurrent.resize(n*m);
    imageG->dataCurrent.resize(n*m);
    imageB->dataCurrent.resize(n*m);

    // correct Offset & correct colors
    // for bayerfilter RGBG colorbalance:
    const float R = 2.21;
    const float G = 1.00;
    const float B = 1.01;
    const float g = 1.51;
    // offset correction
    const float o = 256;

    // Subtract offset;
    // Color balance with sensor multipliers R,G,B,g
    long k1;
    long i;
    long j;

    // TODO: test if we can simplify the code below by using references to vectors (less writing);
    // QVector<float> &I = image->dataCurrent;

    if (R != 1.0 && G != 1.0 && B != 1.0 && o != 0.0) {
        for (j=0; j<m; ++j) {
            for (i=0; i<n; ++i) {
                if ( j == 0 || j >= m-1 || i == 0 || i >= n-1) {  // upper lower left right edge
                    // don't care here
                    ++k1;
                }
                else {
                    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // red pixel
                        image->dataCurrent[(i+n*j)] = R*(image->dataCurrent[(i+n*j)]-o);
                        k1++;
                    }
                    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // green pixel 1, red above
                        image->dataCurrent[(i+n*j)] = G*(image->dataCurrent[(i+n*j)]-o);
                        k1++;
                    }
                    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // green pixel 2, blue above
                        image->dataCurrent[(i+n*j)] = g*(image->dataCurrent[(i+n*j)]-o);
                        k1++;;
                    }
                    if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blue pixel
                        image->dataCurrent[(i+n*j)] = B*(image->dataCurrent[(i+n*j)]-o);
                        k1++;
                    }
                }
            }
        }
    }

    k1 = 0;  // red

    // Calculate the green values at red and blue pixels
    for (j=0; j<m; ++j) {
        for (i=0; i<n; ++i) {

            if( j<=2 || j>=m-3 || i<=2 || i>=n-3) { // 3 rows top bottom left right
                imageR->dataCurrent[k1] = image->dataCurrent[(i+n*j)];  // all the same with current color
                imageG->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                imageB->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                ++k1;
            }
            else{
                // gradient calculation for green values at red or blue pixels
                float DN = abs(image->dataCurrent[(i-2*n+n*j)]-image->dataCurrent[(i+n*j)])*2.0
                        + abs(image->dataCurrent[(i-n+n*j)]-image->dataCurrent[(i+n+n*j)]);
                float DE = abs(image->dataCurrent[(i+n*j)]-image->dataCurrent[(i+2+n*j)])*2.0
                        + abs(image->dataCurrent[(i-1+n*j)]-image->dataCurrent[(i+1+n*j)]);
                float DW = abs(image->dataCurrent[(i-2+n*j)]-image->dataCurrent[(i+n*j)])*2.0
                        + abs(image->dataCurrent[(i-1+n*j)]-image->dataCurrent[(i+1+n*j)]);
                float DS = abs(image->dataCurrent[(i+n*j)]-image->dataCurrent[(i+2*n+n*j)])*2.0
                        + abs(image->dataCurrent[(i-n+n*j)]-image->dataCurrent[(i+n+n*j)]);

                if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0) { // red pixel
                    imageR->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                    switch (direction(DN,DE,DW,DS)) {
                    case 1: imageG->dataCurrent[k1] = (image->dataCurrent[(i-n+n*j)]*3.0+ image->dataCurrent[(i+n*j)]
                                + image->dataCurrent[(i+n+n*j)] - image->dataCurrent[(i-2*n+n*j)]) / 4.0; break;
                    case 2: imageG->dataCurrent[k1] = (image->dataCurrent[(i+1+n*j)]*3.0+ image->dataCurrent[(i+n*j)]
                                + image->dataCurrent[(i-1+n*j)] - image->dataCurrent[(i+2+n*j)]) / 4.0; break;
                    case 3: imageG->dataCurrent[k1] = (image->dataCurrent[(i-1+n*j)]*3.0+ image->dataCurrent[(i+n*j)]
                                + image->dataCurrent[(i+1+n*j)] - image->dataCurrent[(i-2+n*j)]) / 4.0; break;
                    case 4: imageG->dataCurrent[k1] = (image->dataCurrent[(i+n+n*j)]*3.0+ image->dataCurrent[(i+n*j)]
                                + image->dataCurrent[(i-n+n*j)] - image->dataCurrent[(i+2*n+n*j)]) / 4.0; break;
                    }
                    ++k1;
                }
                else if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blue pixel
                    switch (direction(DN,DE,DW,DS)){
                    case 1: imageG->dataCurrent[k1] = (image->dataCurrent[(i-n+n*j)]*3.0+ image->dataCurrent[(i+n*j)]
                                + image->dataCurrent[(i+n+n*j)] - image->dataCurrent[(i-2*n+n*j)]) / 4.0; break;
                    case 2: imageG->dataCurrent[k1] = (image->dataCurrent[(i+1+n*j)]*3.0+ image->dataCurrent[(i+n*j)]
                                + image->dataCurrent[(i-1+n*j)] - image->dataCurrent[(i+2+n*j)]) / 4.0; break;
                    case 3: imageG->dataCurrent[k1] = (image->dataCurrent[(i-1+n*j)]*3.0+ image->dataCurrent[(i+n*j)]
                                + image->dataCurrent[(i+1+n*j)] - image->dataCurrent[(i-2+n*j)]) / 4.0; break;
                    case 4: imageG->dataCurrent[k1] = (image->dataCurrent[(i+n+n*j)]*3.0+ image->dataCurrent[(i+n*j)]
                                + image->dataCurrent[(i-n+n*j)] - image->dataCurrent[(i+2*n+n*j)]) / 4.0; break;
                    }
                    imageB->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                    ++k1;
                }
                else if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // green pixel, red above
                    imageG->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                    ++k1;
                }
                else if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // green pixel, blue above
                    imageG->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                    ++k1;
                }
            }
        }
    }

    k1 = 0;
    // Calculating blue and red at green pixels
    // Calculating blue or red at red or blue pixels
    for (j=0; j<m; ++j) {
        for (i=0; i<n; ++i) {

            if (j<=2 || j>=m-3 || i<=2 || i>=n-3) {  // 3 rows top bottom left right
                if (!xoffset && !yoffset) {  // RGGB
                    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // red pixel
                        imageR->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                        imageG->dataCurrent[k1] = image->dataCurrent[(i+1+n*j)];
                        imageB->dataCurrent[k1] = image->dataCurrent[(i+1+n+n*j)];
                    }
                    else if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // green pixel, red above
                        imageR->dataCurrent[k1] = image->dataCurrent[(i-n+n*j)];
                        imageG->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                        imageB->dataCurrent[k1] = image->dataCurrent[(i+1+n*j)];
                    }
                    else if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // green pixel, blue above
                        imageR->dataCurrent[k1] = image->dataCurrent[(i-1+n*j)];
                        imageG->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                        imageB->dataCurrent[k1] = image->dataCurrent[(i+n+n*j)];
                    }
                    else if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blue pixel
                        imageR->dataCurrent[k1] = image->dataCurrent[(i-1-n+n*j)];
                        imageG->dataCurrent[k1] = image->dataCurrent[(i-1+n*j)];
                        imageB->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                    }
                }
                else if (xoffset && !yoffset) { //GRBG
                    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // red pixel
                        imageR->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                        imageG->dataCurrent[k1] = image->dataCurrent[(i-1+n*j)];
                        imageB->dataCurrent[k1] = image->dataCurrent[(i-1+n+n*j)];
                    }
                    else if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // green pixel, red above
                        imageR->dataCurrent[k1] = image->dataCurrent[(i-n+n*j)];
                        imageG->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                        imageB->dataCurrent[k1] = image->dataCurrent[(i-1+n*j)];
                    }
                    else if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // green pixel, blue above
                        imageR->dataCurrent[k1] = image->dataCurrent[(i+1+n*j)];
                        imageG->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                        imageB->dataCurrent[k1] = image->dataCurrent[(i+n+n*j)];
                    }
                    else if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blue pixel
                        imageR->dataCurrent[k1] = image->dataCurrent[(i+1-n+n*j)];
                        imageG->dataCurrent[k1] = image->dataCurrent[(i+1+n*j)];
                        imageB->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                    }
                }
                else if (!xoffset && yoffset) { //GBRG
                    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // red pixel
                        imageR->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                        imageG->dataCurrent[k1] = image->dataCurrent[(i-n+n*j)];
                        imageB->dataCurrent[k1] = image->dataCurrent[(i+1-n+n*j)];
                    }
                    else if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // green pixel, red above
                        imageR->dataCurrent[k1] = image->dataCurrent[(i+n+n*j)];
                        imageG->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                        imageB->dataCurrent[k1] = image->dataCurrent[(i+1+n*j)];
                    }
                    else if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // green pixel, blue above
                        imageR->dataCurrent[k1] = image->dataCurrent[(i-1+n*j)];
                        imageG->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                        imageB->dataCurrent[k1] = image->dataCurrent[(i-n+n*j)];
                    }
                    else if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blue pixel
                        imageR->dataCurrent[k1] = image->dataCurrent[(i-1+n+n*j)];
                        imageG->dataCurrent[k1] = image->dataCurrent[(i-1+n*j)];
                        imageB->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                    }
                }
                else if (xoffset && yoffset) { //BGGR
                    if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0){ // red pixel
                        imageR->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                        imageG->dataCurrent[k1] = image->dataCurrent[(i-1+n*j)];
                        imageB->dataCurrent[k1] = image->dataCurrent[(i-1-n+n*j)];
                    }
                    else if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0){ // green pixel, red above
                        imageR->dataCurrent[k1] = image->dataCurrent[(i+n+n*j)];
                        imageG->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                        imageB->dataCurrent[k1] = image->dataCurrent[(i-1+n*j)];
                    }
                    else if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1){ // green pixel, blue above
                        imageR->dataCurrent[k1] = image->dataCurrent[(i+1+n*j)];
                        imageG->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                        imageB->dataCurrent[k1] = image->dataCurrent[(i-n+n*j)];
                    }
                    else if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1){ // blue pixel
                        imageR->dataCurrent[k1] = image->dataCurrent[(i+1+n+n*j)];
                        imageG->dataCurrent[k1] = image->dataCurrent[(i+1+n*j)];
                        imageB->dataCurrent[k1] = image->dataCurrent[(i+n*j)];
                    }
                }
                k1++;
            }
            else{
                // diagonal gradients
                float dne = abs(image->dataCurrent[(i-n+1+n*j)]-image->dataCurrent[(i+n-1+n*j)])
                        + abs(image->dataCurrent[(i-2*n+2+n*j)]-image->dataCurrent[(i+n*j)])
                        + abs(image->dataCurrent[(i+n*j)]-image->dataCurrent[(i+2*n-2+n*j)])
                        + abs(imageG->dataCurrent[(i-n+1+n*j)]-imageG->dataCurrent[(i+n*j)])
                        + abs(imageG->dataCurrent[(i+n*j)]-imageG->dataCurrent[(i+n-1+n*j)]);
                float dnw = abs(image->dataCurrent[(i-n-1+n*j)]-image->dataCurrent[(i+n+1+n*j)])
                        + abs(image->dataCurrent[(i-2-2*n+n*j)]-image->dataCurrent[(i+n*j)])
                        + abs(image->dataCurrent[(i+n*j)]-image->dataCurrent[(i+2+2*n+n*j)])
                        + abs(imageG->dataCurrent[(i-n-1+n*j)]-imageG->dataCurrent[(i+n*j)])
                        + abs(imageG->dataCurrent[(i+n*j)]-imageG->dataCurrent[(i+n+1+n*j)]);

                if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 0) { // red pixel
                    if (dne <= dnw)
                        imageB->dataCurrent[k1] = hue_transit(imageG->dataCurrent[i-n+1+n*j], imageG->dataCurrent[i+n*j], imageG->dataCurrent[i+n-1+n*j],
                                image->dataCurrent[(i-n+1+n*j)], image->dataCurrent[(i+n-1+n*j)]);
                    else
                        imageB->dataCurrent[k1] = hue_transit(imageG->dataCurrent[i-n-1+n*j], imageG->dataCurrent[i+n*j], imageG->dataCurrent[i+n+1+n*j],
                                image->dataCurrent[(i-n-1+n*j)], image->dataCurrent[(i+n+1+n*j)]);
                    ++k1;
                }
                else if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 0) { // green pixel, red above
                    imageR->dataCurrent[k1] = hue_transit(imageG->dataCurrent[(i-n+n*j)], image->dataCurrent[(i+n*j)], imageG->dataCurrent[(i+n+n*j)],
                            image->dataCurrent[(i-n+n*j)], image->dataCurrent[(i+n+n*j)]);
                    imageB->dataCurrent[k1] = hue_transit(imageG->dataCurrent[(i-1+n*j)], image->dataCurrent[(i+n*j)], imageG->dataCurrent[(i+1+n*j)],
                            image->dataCurrent[(i-1+n*j)], image->dataCurrent[(i+1+n*j)]);
                    ++k1;
                }
                else if ((j+yoffset)%2 == 0 && (i+xoffset)%2 == 1) { // green pixel, blue above
                    imageR->dataCurrent[k1] = hue_transit(imageG->dataCurrent[(i-1+n*j)], image->dataCurrent[(i+n*j)], imageG->dataCurrent[(i+1+n*j)],
                            image->dataCurrent[(i-1+n*j)], image->dataCurrent[(i+1+n*j)]);
                    imageB->dataCurrent[k1] = hue_transit(imageG->dataCurrent[(i-n+n*j)], image->dataCurrent[(i+n*j)], imageG->dataCurrent[(i+n+n*j)],
                            image->dataCurrent[(i-n+n*j)], image->dataCurrent[(i+n+n*j)]);
                    ++k1;
                }
                else if ((j+yoffset)%2 == 1 && (i+xoffset)%2 == 1) { // blue pixel
                    if (dne <= dnw)
                        imageR->dataCurrent[k1] = hue_transit(imageG->dataCurrent[(i-n+1+n*j)], imageG->dataCurrent[(i+n*j)], imageG->dataCurrent[(i+n-1+n*j)],
                                image->dataCurrent[(i-n+1+n*j)], image->dataCurrent[(i+n-1+n*j)]);
                    else
                        imageR->dataCurrent[k1] = hue_transit(imageG->dataCurrent[(i-n-1+n*j)], imageG->dataCurrent[(i+n*j)], imageG->dataCurrent[(i+n+1+n*j)],
                                image->dataCurrent[(i-n-1+n*j)], image->dataCurrent[(i+n+1+n*j)]);
                    ++k1;
                }
            }
        }
    }
}

void updateDebayerMemoryStatus(MyImage *image)
{
    if (!image->successProcessing) return;

    image->imageInMemory = true;
    image->dataBackupL1 = image->dataCurrent;
    image->backupL1InMemory = true;
}

// Remove the relative sensitivity pattern from the bayerflat
// (Calculate an average 2x2 superpixel and divide the flat by it)
void equalizeBayerFlat(MyImage *image)
{
    int n = image->naxis1;
    int m = image->naxis2;

    // calculate the average 2x2 pixel
    float ll = 0.;
    float lr = 0.;
    float ul = 0.;
    float ur = 0.;

    for (int j=0; j<m-1; j+=2) {
        for (int i=0; i<n-1; i+=2) {
            ll += image->dataCurrent[i+n*j];
            lr += image->dataCurrent[i+1+n*j];
            ul += image->dataCurrent[i+n*(j+1)];
            ur += image->dataCurrent[i+1+n*(j+1)];
        }
    }

    ll /= ( n*m / 4. );
    lr /= ( n*m / 4. );
    ul /= ( n*m / 4. );
    ur /= ( n*m / 4. );

    // The four values above have the average intensity of the flat.
    // We want to preserve it after the super pixel normalization
    float sum = (ll + lr + ul + ur) / 4.;
    ll /= sum;
    lr /= sum;
    ul /= sum;
    ur /= sum;

    // Normalize the flat
    for (int j=0; j<m-1; j+=2) {
        for (int i=0; i<n-1; i+=2) {
            image->dataCurrent[i+n*j] /= ll;
            image->dataCurrent[i+1+n*j] /= lr;
            image->dataCurrent[i+n*(j+1)] /= ul;
            image->dataCurrent[i+1+n*(j+1)] /= ur;
        }
    }

    // Update the mode
    image->updateMode();
}
