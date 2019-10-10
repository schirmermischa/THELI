#ifndef COLORPICTUREWORKER_H
#define COLORPICTUREWORKER_H

#include "../colorpicture/colorpicture.h"
#include "worker.h"

#include <QObject>

// Forward declaration, because 'include colorpciture.h' includes this worker
class ColorPicture;

class ColorPictureWorker : public Worker
{
    Q_OBJECT

public:
    explicit ColorPictureWorker(ColorPicture *colorpicture, QString workmode, QObject *parent = nullptr);

    ColorPicture *colorPicture;
    QString workMode;
    bool yield = false;

public slots:
    void runTask();

signals:
    void tasknameReturned(QString workmode);

};

#endif // COLORPICTUREWORKER_H
