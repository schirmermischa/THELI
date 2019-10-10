#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "../processingInternal/data.h"
#include "../myimage/myimage.h"

#include <QAbstractTableModel>
#include <QColor>
#include <QList>

class Data;

class DataModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit DataModel(Data *someData, QObject *parent = nullptr);
    explicit DataModel(Data *someData, QString weight, QObject *parent = nullptr);
    explicit DataModel(QObject *parent = nullptr);

    // Header:
    Q_INVOKABLE QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Data *myData;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QList<MyImage*> imageList;
    QList<MyImage*> exposureList;

    QString modelType = "science";   // can be "calib", "weight", and "globalweight";

    // Add and remove data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    void updateImageList();

private slots:
    void modelUpdateReceiver(QString basename, QString chipName);
    void updateHeaderLine(long row);
    void updateHeaderLineGlobal();

public slots:
    void beginResetModelReceived();
    void endResetModelReceived();

private:
    int numImages = 0;
    QColor orange = QColor("#f39c12");
    QColor blue = QColor("#3498d8");
    QColor green = QColor("#16a085");
    QColor red = QColor("#e74c3c");
    QColor turquois = QColor("#49ebc5");
    QColor grey = QColor("#dddddd");
    QColor white = QColor("#ffffff");

    QColor getColorDrive(bool flag);
    QColor getColorRAM(bool flag);
    QColor getColorState(MyImage::active_type state);
    QColor getColorImage(MyImage::active_type state);
    QString getIndicator(bool flag);
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

signals:
//    void writeImageToDisk(MyImage *myImage) const;
};

#endif // DATAMODEL_H
