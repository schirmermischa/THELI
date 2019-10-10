#ifndef CPU_H
#define CPU_H

#include <QObject>
#include <QFile>
#include <QVector>
#include <QTextStream>

class CPU : public QObject
{
    Q_OBJECT
public:
    explicit CPU(QObject *parent = nullptr);
    ~CPU();

    int getCPUload();
    float getCurrentValue();

signals:

private:

    bool CPUbarDeactivated = false;
    double tot1 = 0.;
    double tot2 = 0.;
    double idle1 = 0.;
    double idle2 = 0.;

    void readStatsCPU(double &totval, double &idleval);

    QFile file;
    QTextStream instream;
    int maxCPU = 1;

    long long lastTotal = 0;
    long long lastTotalActive = 0;
    long long lastTotalIdle = 0;

    void init();
public slots:

};

#endif // CPU_H
