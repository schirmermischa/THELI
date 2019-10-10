#include "cpu.h"

#include <QVector>
#include <QString>
#include <QTest>
#include <QFile>
#include <QThread>

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

CPU::CPU(QObject *parent) : QObject(parent)
{
    file.setFileName("/proc/stat");
    if ( !file.open(QIODevice::ReadOnly)) {
        CPUbarDeactivated = true;
        return;
    }
    instream.setDevice(&file);

    maxCPU = QThread::idealThreadCount();


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

    // snapshot 1
    readStatsCPU(tot1, idle1);

    // 250ms pause
    QTest::qWait(250);

    // snapshot 2
    readStatsCPU(tot2, idle2);

    double dTot = tot2-tot1;
    double dIdle = idle2-idle1;

    int CPUload = 100. * maxCPU * (dTot - dIdle) / dTot;

    return CPUload;
}

void CPU::readStatsCPU(double &totval, double &idleval)
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

void CPU::init()
{
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
