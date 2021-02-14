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

// qimage: the coordinate system of the displayed QImage
// myimage: the coordinate system of the FITS image

void IView::qimageToBinned(qreal qx, qreal qy, qreal &bx, qreal &by)
{
    qreal norm = naxis2 >= naxis1 ? naxis2 : naxis1;
    bx = qx * qreal(icdw->navigator_binned_nx) / norm;
    by = qy * qreal(icdw->navigator_binned_ny) / norm;
}

QPointF IView::qimageToBinned(const QPointF qpoint)
{
    qreal norm = naxis2 >= naxis1 ? naxis2 : naxis1;
    qreal qx = qpoint.x();
    qreal qy = qpoint.y();
    qreal bx = qx * qreal(icdw->navigator_binned_nx) / norm;
    qreal by = qy * qreal(icdw->navigator_binned_ny) / norm;
    QPointF bpoint;
    bpoint.setX(bx);
    bpoint.setY(by);

    return bpoint;
}

QRect IView::qimageToBinned(const QRectF qrect)
{
    qreal qx1 = 0.;
    qreal qy1 = 0.;
    qreal qx2 = 0.;
    qreal qy2 = 0.;
    qrect.getCoords(&qx1, &qy1, &qx2, &qy2);
    qreal bx1;
    qreal bx2;
    qreal by1;
    qreal by2;
    qimageToBinned(qx1, qy1, bx1, by1);
    qimageToBinned(qx2, qy2, bx2, by2);
    QRect brect;
    brect.setCoords(bx1, by1, bx2, by2);

    return brect;
}

void IView::binnedToQimage(qreal bx, qreal by, qreal &qx, qreal &qy)
{
    qreal norm = naxis2 >= naxis1 ? naxis2 : naxis1;
    qx = bx * norm / qreal(icdw->navigator_binned_nx);
    qy = by * norm / qreal(icdw->navigator_binned_ny);
}

QPointF IView::binnedToQimage(const QPointF bpoint)
{
    qreal norm = naxis2 >= naxis1 ? naxis2 : naxis1;
    qreal bx = bpoint.x();
    qreal by = bpoint.y();
    qreal qx = bx * norm / qreal(icdw->navigator_binned_nx);
    qreal qy = by * norm / qreal(icdw->navigator_binned_ny);
    QPointF qpoint;
    qpoint.setX(qx);
    qpoint.setY(qy);

    return qpoint;
}
