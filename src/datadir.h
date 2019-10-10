#ifndef DATADIR_H
#define DATADIR_H

#include <QString>
#include <QDir>

class DataDir
{
public:
    DataDir(QString data, QString main = "");
    DataDir();

    QString name;  // the full path name
    QString subdirname;  // the data subdir name alone
    QDir dir;
    bool exists();
    long numFITS;
    long numCHIP1;
    bool hasMaster();
    bool isEmpty();
    bool hasType(QString type);
    long numEXT(QString type);
    void setPaths(QString data, QString main = "");
};

#endif // DATADIR_H
