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
