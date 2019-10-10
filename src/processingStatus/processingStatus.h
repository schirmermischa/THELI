#ifndef PROCESSINGSTATUS_H
#define PROCESSINGSTATUS_H

#include <QObject>
#include <QDir>
#include <QString>

class ProcessingStatus : public QObject
{
    Q_OBJECT
public:
    explicit ProcessingStatus(QString dirName, QObject *parent = nullptr);

    bool HDUreformat = false;
    bool Processscience = false;
    bool Chopnod = false;
    bool Background = false;
    bool Collapse = false;
    bool Starflat = false;
    bool Skysub = false;

    QString statusString = "";

    void writeToDrive();       // Used by Data class only
    void readFromDrive();      // Used by Data class only
    void reset();
    void statusToBoolean(QString status);
    QString getStatusString();
    void inferStatusFromFilenames();

private:
    QDir dir;
    QString dirName = "";

    QString extractStatusFromFilename(QString &filename);

signals:
    void processingStatusChanged();
    void messageAvailable(QString messageString, QString code);
    void critical();

public slots:
};

#endif // PROCESSINGSTATUS_H
