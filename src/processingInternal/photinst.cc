#include "photinst.h"
#include "../functions.h"

#include <QVector>

PhotInst::PhotInst(QObject *parent) : QObject(parent)
{
    fluxScale.clear();
    expTime.clear();
    RZP.clear();
    indexMap.clear();
    baseName.clear();
}

void PhotInst::getRZP()
{
    numExp = fluxScale.length();

    for (int j=0; j<numExp; ++j) {
        RZP << -2.5*log10(fluxScale[j] * expTime[j]);
    }

    double meanRZP = meanMask_T(RZP);

    // Mean relative ZP
    for (auto &it : RZP) {
        it -= meanRZP;
    }

    // THELI FLXSCALE
    for (int j=0; j<numExp; ++j) {
        fluxScale[j] = pow(10., -0.4*RZP[j]) / expTime[j];
    }
}
