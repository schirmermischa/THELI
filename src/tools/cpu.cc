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

#include "cpu.h"

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

#include "fitsio.h"

#ifdef __MACH__
#include <mach/mach.h>
#define MACOS
#elif __LINUX__
#define LINUX
#endif

CPU::CPU(QObject *parent) : QObject(parent)
{
    file.setFileName("/proc/stat");
    if ( !file.open(QIODevice::ReadOnly)) {
        CPUbarDeactivated = true;
        return;
    }
    instream.setDevice(&file);

    maxCPU = QThread::idealThreadCount();

    if (!fits_is_reentrant()) maxCPU = 1;

    // new method
    init();
}

CPU::~CPU()
{
    file.close();
}

int CPU::getCPUload()
{
    if (CPUbarDeactivated) return 0;

    // Take two snapshots 250 ms apart
    if (kernelType == "linux") {
        readStatsCPU_Linux(tot1, idle1);
        QTest::qWait(250);
        readStatsCPU_Linux(tot2, idle2);
    }
    else if (kernelType == "darwin") {
        readStatsCPU_MAC(tot1, idle1);
        QTest::qWait(250);
        readStatsCPU_MAC(tot2, idle2);
    }

    double dTot = tot2-tot1;
    double dIdle = idle2-idle1;

    int CPUload = 100. * maxCPU * (dTot - dIdle) / dTot;

    return CPUload;
}

void CPU::readStatsCPU_Linux(double &totval, double &idleval)
{
    // Reset file to beginning
    instream.seek(0);

    double tot = 0.;
    double idle = 0.;

    QString line = instream.readLine().simplified();
    QStringList values = line.split(" ");
    if (values[0] == "cpu") {
        for (int i=1; i<values.length(); ++i) {
            tot += values[i].toDouble();
            if (i==4 || i==5) idle += values[i].toDouble();
        }
    }

    totval = tot;
    idleval = idle;
}

void CPU::readStatsCPU_MAC(double &totval, double &idleval)
{
    unsigned long ulSystem;
    unsigned long ulUser;
    unsigned long ulNice;
    unsigned long ulIdle;
    getCPUInfo_MAC(&ulSystem, &ulUser, &ulNice, &ulIdle);

    totval = (double)(ulSystem + ulUser + ulNice + ulIdle);
    idleval = (double) ulIdle;
}

void CPU::init()
{
    QSysInfo *sysInfo = new QSysInfo;
    kernelType = sysInfo->kernelType();

    if (kernelType == "linux") {
        // Reset file to beginning
        lastTotal = 0;
        lastTotalActive = 0;
        lastTotalIdle = 0;
        instream.seek(0);
        QStringList values = instream.readLine().simplified().split(" ");
        for (int i=1; i<values.length(); ++i) {
            if (i!=4 && i!=5) lastTotalActive += values[i].toLongLong();
            else lastTotalIdle += values[i].toLongLong();
        }
        lastTotal = lastTotalIdle + lastTotalActive;
    }

    if (kernelType == "darwin") {
        // nothing to be done
    }

    delete sysInfo;
}

float CPU::getCurrentValue()
{
    long long total = 0;
    long long totalActive = 0;
    long long totalIdle = 0;

    // Reset file to beginning
    // We assume the first line contains the total of all CPUs, and starts with string "cpu".
    // Columns 5 and 6 conmtain the idle times (idle, iowait)
    instream.seek(0);
    QStringList values = instream.readLine().simplified().split(" ");
    for (int i=1; i<values.length(); ++i) {
        if (i!=4 && i!=5) totalActive += values[i].toLongLong();
        else totalIdle += values[i].toLongLong();
    }
    total = totalActive + totalIdle;

    long diffTotalActive = totalActive - lastTotalActive;
    //    long diffTotalIdle = totalIdle - lastTotalIdle;
    long diffTotal = total - lastTotal;
    float CPUload = 100. * maxCPU * diffTotalActive / diffTotal;

    lastTotal = total;
    lastTotalActive = totalActive;
    lastTotalIdle = totalIdle;

    return CPUload;
}

void CPU::getCPUInfo_MAC(unsigned long *pulSystem, unsigned long *pulUser, unsigned long *pulNice, unsigned long *pulIdle)
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
