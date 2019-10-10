#ifndef CFITSIOERRORCODES_H
#define CFITSIOERRORCODES_H

#include <QObject>
#include <QMap>

class CfitsioErrorCodes : public QObject
{
    Q_OBJECT
    void populateErrorKeyMap();

public:
    explicit CfitsioErrorCodes(QObject *parent = nullptr);

    QMap<int, QString> errorKeyMap;

signals:

public slots:
};

#endif // CFITSIOERRORCODES_H
