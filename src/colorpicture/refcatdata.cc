#include "refcatdata.h"

RefCatData::RefCatData(QString refcatname, QObject *parent) : QObject(parent)
{
    name = refcatname;
}

void RefCatData::clear()
{
    ra.clear();
    de.clear();
    mag1.clear();
    mag2.clear();
    mag3.clear();
    mag4.clear();
    mag5.clear();
}
