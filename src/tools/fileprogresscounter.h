#ifndef FILEPROGRESSCOUNTER_H
#define FILEPROGRESSCOUNTER_H

#include <QObject>
#include <QDir>
#include <QTimer>
#include <QFile>

class FileProgressCounter : public QObject
{
    Q_OBJECT
public:
    explicit FileProgressCounter(QString dirname, QString filter, int numtotimages, float *progressDep,
                                 QObject *parent = nullptr);
    explicit FileProgressCounter(QString dirname, long finalsize, float *progressDep, QObject *parent = nullptr);

    QDir dir;
    QString dirName;
    QStringList filterList;
    int numTotImages;
    QTimer *timer;
    QFileInfo fi;
    long finalSize;
    float *progress;

signals:
    void progressUpdate(float progress);

public slots:
    void numberFileProgress();
    void sizeFileProgress();
    void stopFileProgressTimerReceived();

};

#endif // FILEPROGRESSCOUNTER_H
