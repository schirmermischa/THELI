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

#include "ram.h"

#include <QVector>
#include <QString>
#include <QTest>
#include <QFile>
#include <QThread>
#include <QSysInfo>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifdef __MACH__
#include <mach/mach.h>
#define MACOS
#elif __LINUX__
#define LINUX
#endif

RAM::RAM(QObject *parent) : QObject(parent)
{
    file.setFileName("/proc/meminfo");
    if ( !file.open(QIODevice::ReadOnly)) {
        RAMbarDeactivated = true;
        return;
    }
    instream.setDevice(&file);

    QSysInfo *sysInfo = new QSysInfo;
    kernelType = sysInfo->kernelType();
    delete sysInfo;
    sysInfo = nullptr;
}

RAM::~RAM()
{
    file.close();
}

long RAM::getRAMload()
{
    if (RAMbarDeactivated) return 0;

    if (kernelType == "linux") {
        readStatsRAM_Linux();
    }
    else if (kernelType == "darwin") {
        readStatsRAM_MAC();
    }

    int RAMload = (totalRAM - availableRAM) / 1024;

    return RAMload;   // in [MB]
}

void RAM::readStatsRAM_Linux()
{
    // Reset file to beginning
    instream.seek(0);

    // Files in /proc/ report their size as zero. Hence we can't do "while until EOF", but just read until nothing is read anymore.

    QString line = instream.readLine().simplified();
    while (!line.isNull()) {
        QStringList values = line.split(" ");
        if (values[0] == "MemAvailable:") {
            availableRAM = values[1].toLong();
            break;
        }
        line = instream.readLine().simplified();
    }
}

void RAM::readStatsRAM_MAC()
{
    unsigned long ulSystem;
    unsigned long ulUser;
    unsigned long ulNice;
    unsigned long ulIdle;
    getRAMInfo_MAC(&ulSystem, &ulUser, &ulNice, &ulIdle);
}

float RAM::getCurrentValue()
{
    if (kernelType == "linux") {
        readStatsRAM_Linux();
    }
    else if (kernelType == "darwin") {
        readStatsRAM_MAC();
    }

    return (totalRAM - availableRAM) / 1024.;
}

void RAM::getRAMInfo_MAC(unsigned long *pulSystem, unsigned long *pulUser, unsigned long *pulNice, unsigned long *pulIdle)
{
#ifdef __MACH__
    mach_msg_type_number_t unCpuMsgCount = 0;
    processor_flavor_t nCpuFlavor = PROCESSOR_CPU_LOAD_INFO;
    kern_return_t nErr = 0;
    natural_t unCPUNum = 0;
    processor_cpu_load_info_t structCpuData;
    host_t host = mach_host_self();
    *pulSystem = 0;
    *pulUser = 0;
    *pulNice = 0;
    *pulIdle = 0;
    nErr = host_processor_info(host, nCpuFlavor, &unCPUNum, (processor_info_array_t *)&structCpuData, &unCpuMsgCount);
    if(nErr != KERN_SUCCESS) {
        qDebug() << "Kernel error: " << mach_error_string(nErr);
    }
    else {
        for(int i=0; i<(int)unCPUNum; ++i) {
            *pulSystem += structCpuData[i].cpu_ticks[CPU_STATE_SYSTEM];
            *pulUser += structCpuData[i].cpu_ticks[CPU_STATE_USER];
            *pulNice += structCpuData[i].cpu_ticks[CPU_STATE_NICE];
            *pulIdle += structCpuData[i].cpu_ticks[CPU_STATE_IDLE];
        }
    }
#endif
}
