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

#include "iview.h"
#include "ui_iview.h"

#include "../functions.h"
#include "mygraphicsview.h"
#include "mygraphicsellipseitem.h"
#include "mygraphicsscene.h"
#include "ui_ivconfdockwidget.h"

#include <QDir>
#include <QSettings>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QGraphicsGridLayout>
#include <QDebug>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QToolBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPointF>
#include <QScrollBar>
#include <QTimer>

void IView::initSeparationVector(QPointF pointStart)
{
    if (displayMode.contains("SCAMP") || displayMode == "CLEAR") return;

    startLeftClickInsideItem = false;
    QList<QGraphicsItem*> listStart = scene->items(pointStart, Qt::IntersectsItemShape);
    for (auto & it : listStart) {
        if (it->type() == QGraphicsEllipseItem::Type) startLeftClickInsideItem = true;
    }
}

void IView::initDynrangeDrag()
{
    if (displayMode.contains("SCAMP") || displayMode == "CLEAR") return;

    dynRangeMinDragStart = dynRangeMin;
    dynRangeMaxDragStart = dynRangeMax;

    // Must clear all items on the scene, then redraw afterwards
    if (!skyCircleItems.isEmpty()) skyCircleItems.clear();
    if (!sourceCatItems.isEmpty()) sourceCatItems.clear();
    if (!refCatItems.isEmpty()) refCatItems.clear();
    if (!acceptedSkyCircleItems.isEmpty()) {
        for (auto &it : acceptedSkyCircleItems) {
            scene->removeItem(it);
        }
    }
}

void IView::showCurrentMousePos(QPointF point)
{
    // This function receives the image coords from a mouseMoveEvent
    // and translates them

    // Cursor coordinates:
    // Still have to mirror in y, and add a half pixel correction
    // to be conform with the FITS standard

    if (displayMode == "CLEAR" || displayMode.contains("SCAMP")) return;

    float x_cursor;
    float y_cursor;
    // constant offsets required to show same coordinates as in skycat / ds9
    x_cursor = point.x() + 0.5;
    y_cursor = naxis2 - point.y() + 0.5;

    // Display the information
    QString xpos = QString::number(x_cursor);
    QString ypos = QString::number(y_cursor);
    icdw->ui->xposLabel->setText("x = "+xpos);
    icdw->ui->yposLabel->setText("y = "+ypos);

    // Pixel index in the 2D image
    long i = x_cursor - 0.5;
    long j = y_cursor - 0.5;
    if (i<naxis1 && i>=0 && j<naxis2 && j>=0) {
        if (displayMode == "FITSmonochrome" || displayMode == "MEMview") {
            QString value = QString::number(fitsData[i+naxis1*j]);
            icdw->ui->valueLabel->setText("Value = "+value);
        }
        else {
            // Color FITS
            QString rval = QString::number(fitsDataR[i+naxis1*j]);
            QString gval = QString::number(fitsDataG[i+naxis1*j]);
            QString bval = QString::number(fitsDataB[i+naxis1*j]);
            icdw->ui->valueLabel->setText("Value R = "+rval);
            icdw->ui->valueGreenLabel->setText("Value G = "+gval);
            icdw->ui->valueBlueLabel->setText("Value B = "+bval);
        }
    }
    else {
        if (displayMode == "FITSmonochrome" || displayMode == "MEMview") {
            icdw->ui->valueLabel->setText("Value = ");
        }
        else {
            icdw->ui->valueLabel->setText("Value R = ");
            icdw->ui->valueGreenLabel->setText("Value G = ");
            icdw->ui->valueBlueLabel->setText("Value B = ");
        }
    }

    xy2sky(x_cursor, y_cursor);

    // Show the magnified area in the magnify window
    // Translate FITS pixel position in the large window to the magnify window
    icdw->magnifiedGraphicsView->mapFromScene(point.x(),point.y());
    icdw->magnifiedGraphicsView->setScene(icdw->magnifiedScene);
    icdw->magnifiedGraphicsView->show();
}

void IView::collectLocalStatisticsSample(QPointF point)
{
    if (!statdw->isVisible()) return;

    // Pixel index in the 2D image
    long x = point.x();
    long y = naxis2 - point.y();

    int s = statdw->statWidth;

    int imin = x-s;
    int imax = x+s;
    int jmin = y-s;
    int jmax = y+s;
    if (imin < 0) imin = 0;
    if (imax >= naxis1) imax = naxis1-1;
    if (jmin < 0) jmin = 0;
    if (jmax >= naxis2) jmax = naxis2-1;
    if (displayMode == "FITSmonochrome" || displayMode == "MEMview") {
        QVector<float> sample;
        for (int j = jmin; j<=jmax; ++j) {
            for (int i = imin; i<=imax; ++i) {
                sample.append(fitsData[i+naxis1*j]);
            }
        }
        emit statisticsSampleAvailable(sample);
    }
    else if (displayMode == "FITScolor") {
        QVector<float> sampleR;
        QVector<float> sampleG;
        QVector<float> sampleB;
        for (int j = jmin; j<=jmax; ++j) {
            for (int i = imin; i<=imax; ++i) {
                sampleR.append(fitsDataR[i+naxis1*j]);
                sampleG.append(fitsDataG[i+naxis1*j]);
                sampleB.append(fitsDataB[i+naxis1*j]);
            }
        }
        emit statisticsSampleColorAvailable(sampleR, sampleG, sampleB);
    }
}

void IView::adjustBrightnessContrast(QPointF point)
{
    if (displayMode.contains("SCAMP") || displayMode == "CLEAR") return;
    // Also leave if actively cycling through images
    // (will crash because the scene items become unavailable while dragging)
    // Or (NOT IMPLEMENTED): stop cycling through images
    if (ui->actionForward->isChecked()) return;
    if (ui->actionBack->isChecked()) return;

    icdw->ui->autocontrastPushButton->setChecked(false);
    // This slot receives the right-click drag vector
    int dx = point.x();
    int dy = point.y();

    // Adjust brightness level (shift lower bound of dynamic range)
    // Adjust contrast symmetrically around
    float range = dynRangeMaxDragStart - dynRangeMinDragStart;

    //    dynRangeMin = dynRangeMinDragStart * (1.1*(100.+dy)/100.+0.01) - 7*dx;
    //    dynRangeMax = dynRangeMaxDragStart / (1.1*(100.+dy)/100.+0.01);
    float cutoff = 0.95;
    float scaling = dy/100.*cutoff;
    if (scaling > cutoff) scaling = cutoff;
    if (scaling < -cutoff) scaling = -cutoff;
    float deltaRange = - 0.5 * range * scaling;
    dynRangeMax = dynRangeMaxDragStart - deltaRange;
    //    dynRangeMin = dynRangeMinDragStart + deltaRange + 7*dx;
    dynRangeMin = dynRangeMinDragStart + deltaRange + range*0.4*dx/100;
    //    qDebug() << dynRangeMinDragStart << deltaRange << 7*dx << scaling << dy;
    //    if (dynRangeMin <= 1.01*dynRangeMin) dynRangeMax = 1.01*dynRangeMin;
    int validDigits = 3-log(fabs(dynRangeMax))/log(10);
    if (validDigits<0) validDigits = 0;
    icdw->ui->minLineEdit->setText(QString::number(dynRangeMin,'f',validDigits));
    icdw->ui->maxLineEdit->setText(QString::number(dynRangeMax,'f',validDigits));
    mapFITS();
    myGraphicsView->setScene(scene);
    myGraphicsView->show();

    // Show the magnified area in the magnify window
    icdw->magnifiedGraphicsView->setScene(icdw->magnifiedScene);
    icdw->magnifiedGraphicsView->show();

    emit updateNavigatorBinned(binnedPixmapItem);
    emit updateNavigatorBinnedCDmatrix(wcs->cd);
}

double IView::haversine(double x1, double y1, double x2, double y2)
{
    if (!wcsInit) return 0.;

    double world1[2];
    double world2[2];
    double phi;
    double theta;
    double imgcrd[2];
    double pixcrd[2];
    int stat[1];

    // zero-indexing of C++ vectors can be ignored when measuring distances.
    pixcrd[0] = x1;
    pixcrd[1] = y1;
    wcsp2s(wcs, 1, 2, pixcrd, imgcrd, &phi, &theta, world1, stat);
    double alpha1 = world1[0] * rad;
    double delta1 = world1[1] * rad;

    // Get alpha / delta for the 2nd point
    pixcrd[0] = x2;
    pixcrd[1] = y2;
    wcsp2s(wcs, 1, 2, pixcrd, imgcrd, &phi, &theta, world2, stat);
    double alpha2 = world2[0] * rad;
    double delta2 = world2[1] * rad;

    double dDelta = delta2 - delta1;
    double dAlpha = alpha2 - alpha1;

    // Haversine formula to calculate angular distance between two points on a sphere in degrees, also works for very small separations
    return 2.*asin( sqrt( pow(sin(dDelta/2.),2) + cos(delta1)*cos(delta2)*pow(sin(dAlpha/2.),2))) / rad;
}

void IView::measureAngularSeparations(QPointF pointStart, QPointF pointEnd, double &sepX, double &sepY, double &sepD)
{
    qreal x1 = pointStart.x();
    qreal y1 = pointStart.y();
    qreal x2 = pointEnd.x();
    qreal y2 = pointEnd.y();

    // The "diagonal"
    sepD = haversine(x1, y1, x2, y2) * 3600.;

    // The "horizontal"
    sepX = haversine(x1, y1, x2, y1) * 3600.;

    // The "vertical"
    sepY = haversine(x1, y1, x1, y2) * 3600.;
}

void IView::drawSeparationVector(QPointF pointStart, QPointF pointEnd)
{
    if (displayMode.contains("SCAMP") || displayMode == "CLEAR") return;
    // (will crash because the scene items become unavailable while dragging)
    if (ui->actionForward->isChecked()) return;
    if (ui->actionBack->isChecked()) return;

    // don't draw the vector if there is an ellipse underneath
    if (startLeftClickInsideItem) return;

    QPen pen(QColor("#ffff00"));
    pen.setWidth(0);
    QPen penDashed(QColor("#ffff00"));
    penDashed.setWidth(0);
    QVector<qreal> dashes;
    dashes << 5 << 5;
    penDashed.setDashPattern(dashes);
    qreal x1 = pointStart.x();
    qreal y1 = pointStart.y();
    qreal x2 = pointEnd.x();
    qreal y2 = pointEnd.y();
    qreal dy = y2-y1;
    qreal dx = x2-x1;
    double sepX = 0.;
    double sepY = 0.;
    double sepD = 0.;
    measureAngularSeparations(pointStart, pointEnd, sepX, sepY, sepD);

    QString sepXString = getVectorLabel(sepX);
    QString sepYString = getVectorLabel(sepY);
    QString sepDString = getVectorLabel(sepD);
    clearVectorItems();
    QGraphicsTextItem *labelX = scene->addText(sepXString);
    QGraphicsTextItem *labelY = scene->addText(sepYString);
    QGraphicsTextItem *labelD = scene->addText(sepDString);
    QFont currentFont = this->font();
    qreal rescale;
    if (!icdw->ui->zoomFitPushButton->isChecked()) rescale = icdw->zoom2scale(zoomLevel);
    else rescale = myGraphicsView->transform().m11();
    currentFont.setPointSize(int(float(currentFont.pointSize())/rescale));
    labelX->setFont(currentFont);
    labelY->setFont(currentFont);
    labelD->setFont(currentFont);
    labelX->setDefaultTextColor(QColor("#ffff00"));
    labelY->setDefaultTextColor(QColor("#ffff00"));
    labelD->setDefaultTextColor(QColor("#ffff00"));
    qreal xpos = 0.5*(x1+x2);
    qreal ypos = 0.5*(y1+y2);
    qreal x_yoffset = 0.;
    qreal y_xoffset = 0.;
    qreal d_xoffset = 0;
    qreal d_yoffset = 0;
    getVectorOffsets(dx, dy, x_yoffset, y_xoffset, d_xoffset, d_yoffset);
    labelX->setPos(xpos-20/icdw->zoom2scale(zoomLevel), y1+x_yoffset);
    labelY->setPos(x2+y_xoffset, ypos-10/icdw->zoom2scale(zoomLevel));
    labelD->setPos(xpos+d_xoffset, ypos+d_yoffset);
    vectorLineItems.append(scene->addLine(x1, y1, x2, y2, pen));         // diagonal
    vectorLineItems.append(scene->addLine(x1, y1, x2, y1, penDashed));   // horizontal
    vectorLineItems.append(scene->addLine(x2, y1, x2, y2, penDashed));   // vertical
    vectorTextItems.append(labelX);
    vectorTextItems.append(labelY);
    vectorTextItems.append(labelD);
    myGraphicsView->setScene(scene);
    myGraphicsView->show();
}

// Used to draw separation vectors
void IView::getVectorOffsets(const qreal dx, const qreal dy, qreal &x_yoffset,
                             qreal &y_xoffset, qreal &d_xoffset, qreal &d_yoffset)
{
    qreal s;
    if (!icdw->ui->zoomFitPushButton->isChecked()) {
        s = icdw->zoom2scale(zoomLevel);
    }
    else {
        s = myGraphicsView->transform().m11();
    }

    if (dy < 0) x_yoffset = 10/s;
    else x_yoffset = -30/s;
    if (dx < 0) y_xoffset = -70/s;
    else y_xoffset = 10/s;
    if (dx>=0 && dy>=0) {
        d_xoffset = -50/s;
        d_yoffset = 10/s;
    }
    else if (dx<0 && dy>=0) {
        d_xoffset = 10/s;
        d_yoffset = 10/s;
    }
    else if (dx<0 && dy<0) {
        d_xoffset = 10/s;
        d_yoffset = -25/s;
    }
    else {
        // (dx>=0 && dy<0)
        d_xoffset = -50/s;
        d_yoffset = -50/s;
    }
}

void IView::clearVectorItems()
{
    if (!vectorLineItems.isEmpty()) {
        for (auto &it : vectorLineItems ) scene->removeItem(it);
        vectorLineItems.clear();
    }
    if (!vectorTextItems.isEmpty()) {
        for (auto &it : vectorTextItems ) scene->removeItem(it);
        vectorTextItems.clear();
    }
}

void IView::clearSkyRectItems()
{
    if (!skyRectItems.isEmpty()) {
        for (auto &it : skyRectItems ) scene->removeItem(it);
        skyRectItems.clear();
    }
    if (!skyTextItems.isEmpty()) {
        for (auto &it : skyTextItems ) scene->removeItem(it);
        skyTextItems.clear();
    }
}

void IView::clearSkyCircleItems()
{
    if (!skyCircleItems.isEmpty()) {
        for (auto &it : skyCircleItems ) scene->removeItem(it);
        skyCircleItems.clear();
    }
}

// currently unused
void IView::drawSkyRectangle(QPointF pointStart, QPointF pointEnd)
{
    if (displayMode.contains("SCAMP") || displayMode == "CLEAR") return;
    if (displayMode == "FITScolor") return;
    // (will crash because the scene items become unavailable while dragging)
    if (ui->actionForward->isChecked()) return;
    if (ui->actionBack->isChecked()) return;

    QPen pen(QColor("#00ff00"));
    pen.setWidth(0);
    qreal x1 = pointStart.x();
    qreal y1 = pointStart.y();
    qreal x2 = pointEnd.x();
    qreal y2 = pointEnd.y();
    qreal dx = fabs(x2-x1);
    qreal dy = fabs(y2-y1);

    // Collect alpha and delta of the four vertices
    QVector<double> alphaValues;
    QVector<double> deltaValues;
    // TODO if routine is activated: must translate x/y to FITS coordinate system before computing sky coordinates!
    xy2sky(x1,y1,"middleButton");
    alphaValues.append(skyRa);
    deltaValues.append(skyDec);
    xy2sky(x2,y1,"middleButton");
    alphaValues.append(skyRa);
    deltaValues.append(skyDec);
    xy2sky(x2,y2,"middleButton");
    alphaValues.append(skyRa);
    deltaValues.append(skyDec);
    xy2sky(x1,y2,"middleButton");
    alphaValues.append(skyRa);
    deltaValues.append(skyDec);
    double alphaMin = minVec_T(alphaValues);
    double alphaMax = maxVec_T(alphaValues);
    double deltaMin = minVec_T(deltaValues);
    double deltaMax = maxVec_T(deltaValues);

    QString raMin = QString::number(alphaMin,'f',6);
    QString raMax = QString::number(alphaMax,'f',6);
    QString decMin = QString::number(deltaMin,'f',6);
    QString decMax = QString::number(deltaMax,'f',6);
    QString skyBox = "R.A. min = "+raMin+"\n";
    skyBox.append("R.A. max = "+raMax+"\n");
    skyBox.append("DEC min = "+decMin+"\n");
    skyBox.append("DEC max = "+decMax);
    QGraphicsTextItem *labelSkyBox = scene->addText(skyBox);
    QFont currentFont = this->font();
    qreal rescale;
    if (!icdw->ui->zoomFitPushButton->isChecked()) rescale = icdw->zoom2scale(zoomLevel);
    else rescale = myGraphicsView->transform().m11();
    currentFont.setPointSize(int(float(currentFont.pointSize())/rescale));
    labelSkyBox->setFont(currentFont);
    labelSkyBox->setDefaultTextColor(QColor("#00ff00"));
    labelSkyBox->setPos(x1, y1-100/rescale);

    clearSkyRectItems();
    qreal xmin = pointStart.x();
    qreal ymin = pointStart.y();
    if (pointEnd.x() < xmin) xmin = pointEnd.x();
    if (pointEnd.y() < ymin) ymin = pointEnd.y();
    skyRectItems.append(scene->addRect(xmin, ymin, dx, dy, pen));
    skyTextItems.append(labelSkyBox);
    myGraphicsView->setScene(scene);
    myGraphicsView->show();
}

void IView::middlePressResetCRPIXreceived()
{
    if (!wcsInit) return;

    crpix1_start = wcs->crpix[0];
    crpix2_start = wcs->crpix[1];
}

/*
// Receiver for the event when the mouse enters the main graphics view
void IView::mouseEnteredViewReceived()
{
    //    icdw->ui->navigatorStackedWidget->setCurrentIndex(1);
    emit updateNavigatorBinned(binnedPixmapItem);
}

// Receiver for the event when the mouse leaves the main graphics view
void IView::mouseLeftViewReceived()
{
    //    icdw->ui->navigatorStackedWidget->setCurrentIndex(0);
    emit updateNavigatorBinned(binnedPixmapItem);
}
*/

void IView::viewportChangedReceived(QRect viewport_rect)
{
    // the currently shown image area, in QImage coordinates
    QRectF rect = myGraphicsView->mapToScene(viewport_rect).boundingRect();

    // the matching area, in QImage coordinates of the binned overview image
    QRect rectBinned = qimageToBinned(rect);

    // send the binned rect to the binned navigator window
    emit updateNavigatorBinnedViewport(rectBinned);
}

void IView::updateCRPIX(QPointF pointStart, QPointF pointEnd)
{
    if (!wcsInit) return;

    // Do nothing if no refcat items are displayed.
    if (refCatItems.isEmpty()) return;

    // Remove items from display
    for (auto &it: refCatItems) scene->removeItem(it);
    refCatItems.clear();
    myGraphicsView->setScene(scene);
    myGraphicsView->show();

    // Recalculate CRPIX offset
    qreal dx = pointEnd.x() - pointStart.x();
    qreal dy = -(pointEnd.y() - pointStart.y());   // Image y-axis flipped in graphics view

    wcs->crpix[0] = crpix1_start + dx;
    wcs->crpix[1] = crpix2_start + dy;
    wcs->flag = 0;    // force an update of internal wcs params.
    showReferenceCat();
}

void IView::updateCDmatrix(double cd11, double cd12, double cd21, double cd22)
{
    if (!wcsInit) return;

    // Do nothing if no refcat items are displayed.
    if (refCatItems.isEmpty()) return;

    // Remove items from display
    for (auto &it: refCatItems) scene->removeItem(it);
    refCatItems.clear();
    myGraphicsView->setScene(scene);
    myGraphicsView->show();

    // Update CD matrix
    wcs->cd[0] = cd11;
    wcs->cd[1] = cd12;
    wcs->cd[2] = cd21;
    wcs->cd[3] = cd22;
    wcs->flag = 0;    // force an update of internal wcs params.
    showReferenceCat();
}

// Called when middle mouse button is released in wcs mode
void IView::updateCRPIXFITS()
{
    if (!wcsInit) return;

    // Do nothing if no refcat items are displayed.
    if (refCatItems.isEmpty()) return;

    if (currentMyImage != nullptr) {
        int status = 0;
        if (currentFileName.isEmpty()) currentFileName = currentMyImage->baseName+".fits";
        // force drive dump if FITS file does not exist yet
        if (!currentMyImage->imageOnDrive) currentMyImage->writeImage();
        // Identical way to reconstruct filenames
        // TODO: uniformize
        //        qDebug() << currentFileName;
        //        qDebug() << currentMyImage->path+"/"+currentMyImage->chipName+currentMyImage->processingStatus->statusString+".fits";
        fitsfile *fptr = nullptr;
        fits_open_file(&fptr, (dirName+"/"+currentMyImage->pathExtension+"/"+currentFileName).toUtf8().data(), READWRITE, &status);
        fits_update_key_flt(fptr, "CRPIX1", wcs->crpix[0], -5, nullptr, &status);
        fits_update_key_flt(fptr, "CRPIX2", wcs->crpix[1], -5, nullptr, &status);
        fits_close_file(fptr, &status);
        if (status > 0) qDebug() << "IView::updateCRPIXFITS(): cfitsio error code = " << status << dirName+"/"+currentFileName;
    }
}

// Called when slider is released in wcs mode
void IView::updateCDmatrixFITS()
{
    if (!wcsInit) return;

    // Do nothing if no refcat items are displayed.
    if (refCatItems.isEmpty()) return;

    if (currentMyImage != nullptr) {
        int status = 0;
        if (currentFileName.isEmpty()) currentFileName = currentMyImage->baseName+".fits";
        fitsfile *fptr = nullptr;
        fits_open_file(&fptr, (dirName+"/"+currentMyImage->pathExtension+"/"+currentFileName).toUtf8().data(), READWRITE, &status);
        fits_update_key_dbl(fptr, "CD1_1", wcs->cd[0], 8, nullptr, &status);
        fits_update_key_dbl(fptr, "CD1_2", wcs->cd[1], 8, nullptr, &status);
        fits_update_key_dbl(fptr, "CD2_1", wcs->cd[2], 8, nullptr, &status);
        fits_update_key_dbl(fptr, "CD2_2", wcs->cd[3], 8, nullptr, &status);
        fits_close_file(fptr, &status);
        if (status > 0) qDebug() << "IView::updateCDmatrixFITS(): cfitsio error code = " << status << dirName+"/"+currentFileName;
    }
}

void IView::drawSkyCircle(QPointF pointStart, QPointF pointEnd)
{
    if (displayMode.contains("SCAMP") || displayMode == "CLEAR") return;
    if (displayMode == "FITScolor") return;

    QPen pen(QColor("#00ff00"));
    pen.setWidth(0);
    QVector<qreal> dashes;
    dashes << 3 << 3;
    pen.setDashPattern(dashes);
    qreal x1 = pointStart.x();
    qreal y1 = pointStart.y();
    qreal x2 = pointEnd.x();
    qreal y2 = pointEnd.y();
    qreal xcen = 0.5*(x1+x2);
    qreal ycen = 0.5*(y1+y2);
    qreal dx = fabs(x2-x1);
    qreal dy = fabs(y2-y1);
    qreal radius = sqrt(dx*dx+dy*dy);

    // Collect alpha and delta of the first click
    // Set 'skyRa' and 'skyDec'
    xy2sky(xcen,ycen,"middleButton");

    clearSkyCircleItems();
    QGraphicsEllipseItem *ellipse = scene->addEllipse(x1-radius, y1-radius, 2.*radius, 2.*radius, pen);
    ellipse->setFlags(QGraphicsEllipseItem::ItemIsSelectable | QGraphicsEllipseItem::ItemIsMovable);
    skyCircleItems.append(ellipse);
    myGraphicsView->setScene(scene);
    myGraphicsView->show();
}

void IView::appendSkyCircle()
{
    if (displayMode.contains("SCAMP") || displayMode == "CLEAR") return;
    if (displayMode == "FITScolor") return;

    // Fetch the last item drawn and add it to the permanent list
    QGraphicsItem *lastItem = scene->items(Qt::AscendingOrder).last();
    QPen pen(QColor("#00ff00"));
    pen.setWidth(0);
    if (lastItem->type() == QGraphicsEllipseItem::Type) {
        QGraphicsEllipseItem *ellipse = qgraphicsitem_cast<QGraphicsEllipseItem *>(lastItem);
        ellipse->setPen(pen);
        acceptedSkyCircleItems.append(ellipse);
    }
    // Clear the temporary display (ellipses shown while dragging)
    clearSkyCircleItems();
    // Add the last state of the drawn ellipse item to the scene
    if (!acceptedSkyCircleItems.isEmpty()) {
        scene->addItem(acceptedSkyCircleItems.last());
    }
    // Update the data record in the external file
    dumpSkyCircleCoordinates();
}

void IView::updateSkyCircles()
{
    if (displayMode.contains("SCAMP") || displayMode == "CLEAR") return;
    if (displayMode == "FITScolor") return;

    // A selected sky circle has been removed by a key press event, or moved by mouse drag
    // Update the underlying QList by fetching all visible sky circles
    acceptedSkyCircleItems.clear();
    QList<QGraphicsItem *> list = scene->items();
    for (auto &it : list) {
        if (it->type() == QGraphicsEllipseItem::Type) {
            acceptedSkyCircleItems.append(qgraphicsitem_cast<QGraphicsEllipseItem *>(it));
        }
    }
    // Update the data record in the external file
    dumpSkyCircleCoordinates();
}

void IView::redrawSkyCirclesAndCats()
{
    if (displayMode.contains("SCAMP") || displayMode == "CLEAR") return;
    if (displayMode == "FITScolor") return;

    if (!acceptedSkyCircleItems.isEmpty()) {
        for (auto &it: acceptedSkyCircleItems) {
            scene->addItem(it);
        }
    }

    showSourceCat();
    showReferenceCat();
}

void IView::dumpSkyCircleCoordinates()
{
    // Dump the centers and radii of the sky circles to an external file
    QFile skysamples(startDirName+"/skysamples.dat");
    if (skysamples.open(QIODevice::WriteOnly)) {
        QTextStream outputStream(&skysamples);
        for (auto &it : acceptedSkyCircleItems)
        {
            // Store the coordinates in decimal Ra Dec radius [arcsec]
            QRectF bound = it->sceneBoundingRect();
            double xcen = bound.center().x();
            double ycen = naxis2-bound.center().y();
            xy2sky(xcen, ycen, "middleButton");
            double radius = bound.height()/2.*plateScale;
            outputStream.setRealNumberPrecision(9);
            outputStream << skyRa << " " << skyDec << " " << radius << "\n";
        }
        skysamples.close();
    }

    // Create a copy in the parent folder (if we have several science directories of the same target)
    QFile copy(startDirName+"/../skysamples.dat");
    if (copy.exists()) copy.remove();
    skysamples.copy(startDirName+"/../skysamples.dat");
}

void IView::clearSeparationVector()
{
    if (displayMode.contains("SCAMP") || displayMode == "CLEAR") return;

    clearVectorItems();
    myGraphicsView->setScene(scene);
    myGraphicsView->show();
}

void IView::xy2sky(double x, double y, QString button)
{
    if (!wcsInit) return;

    double world[2];
    double phi;
    double theta;
    double imgcrd[2];
    double pixcrd[2];
    pixcrd[0] = x;
    pixcrd[1] = y;
    int  	stat[1];
    wcsp2s(wcs, 1, 2, pixcrd, imgcrd, &phi, &theta, world, stat);

    // not used by all calls to this function
    if (button == "middleButton") {
        skyRa = world[0];
        skyDec = world[1];
        return;
    }

    QString alphaDec = QString::number(world[0],'f',6);
    QString deltaDec = QString::number(world[1],'f',6);
    QString alphaHex = dec2hex(world[0]/15.).remove("+");
    QString deltaHex = dec2hex(world[1]);
    icdw->ui->alphaDecLabel->setText("R.A. = "+alphaDec);
    icdw->ui->alphaHexLabel->setText("R.A. = "+alphaHex);
    icdw->ui->deltaDecLabel->setText("Dec  = "+deltaDec);
    icdw->ui->deltaHexLabel->setText("Dec  = "+deltaHex);
}

// Unused
/*
void IView::xy2sky_linear(double x, double y, QString button)
{
    double pi = 3.14159265;
    double dx = x - crpix1;
    double dy = y - crpix2;
    double xt = cd1_1*dx + cd2_1*dy;
    double yt = cd1_2*dx + cd2_2*dy;
    double delta = crval2 + yt;
    double alpha = crval1 + xt / cos(delta * pi/180.);

    // not used by all calls to this function
    if (button == "middleButton") {
        skyRa = alpha;
        skyDec = delta;
        return;
    }

    QString alphaDec = QString::number(alpha,'f',6);
    QString deltaDec = QString::number(delta,'f',6);
    QString alphaHex = dec2hex(alpha/15.).remove("+");
    QString deltaHex = dec2hex(delta);
    icdw->ui->alphaDecLabel->setText("R.A. = "+alphaDec);
    icdw->ui->alphaHexLabel->setText("R.A. = "+alphaHex);
    icdw->ui->deltaDecLabel->setText("Dec  = "+deltaDec);
    icdw->ui->deltaHexLabel->setText("Dec  = "+deltaHex);
}
*/
