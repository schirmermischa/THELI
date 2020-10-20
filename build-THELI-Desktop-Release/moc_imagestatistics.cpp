/****************************************************************************
** Meta object code from reading C++ file 'imagestatistics.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/imagestatistics/imagestatistics.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imagestatistics.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ImageStatistics_t {
    QByteArrayData data[26];
    char stringdata0[552];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImageStatistics_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImageStatistics_t qt_meta_stringdata_ImageStatistics = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ImageStatistics"
QT_MOC_LITERAL(1, 16, 13), // "imageSelected"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 5), // "index"
QT_MOC_LITERAL(4, 37, 16), // "dataPointClicked"
QT_MOC_LITERAL(5, 54, 21), // "QCPAbstractPlottable*"
QT_MOC_LITERAL(6, 76, 9), // "plottable"
QT_MOC_LITERAL(7, 86, 9), // "dataIndex"
QT_MOC_LITERAL(8, 96, 12), // "QMouseEvent*"
QT_MOC_LITERAL(9, 109, 5), // "event"
QT_MOC_LITERAL(10, 115, 16), // "numericSelection"
QT_MOC_LITERAL(11, 132, 31), // "on_statisticsPushButton_clicked"
QT_MOC_LITERAL(12, 164, 27), // "on_exportPushButton_clicked"
QT_MOC_LITERAL(13, 192, 25), // "on_showPushButton_clicked"
QT_MOC_LITERAL(14, 218, 30), // "on_ClearPlotPushButton_clicked"
QT_MOC_LITERAL(15, 249, 32), // "on_restoreDataPushButton_clicked"
QT_MOC_LITERAL(16, 282, 35), // "on_clearSelectionPushButton_c..."
QT_MOC_LITERAL(17, 318, 36), // "on_invertSelectionPushButton_..."
QT_MOC_LITERAL(18, 355, 27), // "on_readmePushButton_clicked"
QT_MOC_LITERAL(19, 383, 36), // "on_connectDataPointsCheckBox_..."
QT_MOC_LITERAL(20, 420, 22), // "uncheckIviewPushButton"
QT_MOC_LITERAL(21, 443, 8), // "validate"
QT_MOC_LITERAL(22, 452, 40), // "on_fwhmunitsComboBox_currentI..."
QT_MOC_LITERAL(23, 493, 4), // "arg1"
QT_MOC_LITERAL(24, 498, 24), // "on_actionClose_triggered"
QT_MOC_LITERAL(25, 523, 28) // "on_scienceComboBox_activated"

    },
    "ImageStatistics\0imageSelected\0\0index\0"
    "dataPointClicked\0QCPAbstractPlottable*\0"
    "plottable\0dataIndex\0QMouseEvent*\0event\0"
    "numericSelection\0on_statisticsPushButton_clicked\0"
    "on_exportPushButton_clicked\0"
    "on_showPushButton_clicked\0"
    "on_ClearPlotPushButton_clicked\0"
    "on_restoreDataPushButton_clicked\0"
    "on_clearSelectionPushButton_clicked\0"
    "on_invertSelectionPushButton_clicked\0"
    "on_readmePushButton_clicked\0"
    "on_connectDataPointsCheckBox_clicked\0"
    "uncheckIviewPushButton\0validate\0"
    "on_fwhmunitsComboBox_currentIndexChanged\0"
    "arg1\0on_actionClose_triggered\0"
    "on_scienceComboBox_activated"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImageStatistics[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   99,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    3,  102,    2, 0x08 /* Private */,
      10,    0,  109,    2, 0x08 /* Private */,
      11,    0,  110,    2, 0x08 /* Private */,
      12,    0,  111,    2, 0x08 /* Private */,
      13,    0,  112,    2, 0x08 /* Private */,
      14,    0,  113,    2, 0x08 /* Private */,
      15,    0,  114,    2, 0x08 /* Private */,
      16,    0,  115,    2, 0x08 /* Private */,
      17,    0,  116,    2, 0x08 /* Private */,
      18,    0,  117,    2, 0x08 /* Private */,
      19,    0,  118,    2, 0x08 /* Private */,
      20,    0,  119,    2, 0x08 /* Private */,
      21,    0,  120,    2, 0x08 /* Private */,
      22,    1,  121,    2, 0x08 /* Private */,
      24,    0,  124,    2, 0x08 /* Private */,
      25,    1,  125,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5, QMetaType::Int, 0x80000000 | 8,    6,    7,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   23,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   23,

       0        // eod
};

void ImageStatistics::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ImageStatistics *_t = static_cast<ImageStatistics *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->imageSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->dataPointClicked((*reinterpret_cast< QCPAbstractPlottable*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QMouseEvent*(*)>(_a[3]))); break;
        case 2: _t->numericSelection(); break;
        case 3: _t->on_statisticsPushButton_clicked(); break;
        case 4: _t->on_exportPushButton_clicked(); break;
        case 5: _t->on_showPushButton_clicked(); break;
        case 6: _t->on_ClearPlotPushButton_clicked(); break;
        case 7: _t->on_restoreDataPushButton_clicked(); break;
        case 8: _t->on_clearSelectionPushButton_clicked(); break;
        case 9: _t->on_invertSelectionPushButton_clicked(); break;
        case 10: _t->on_readmePushButton_clicked(); break;
        case 11: _t->on_connectDataPointsCheckBox_clicked(); break;
        case 12: _t->uncheckIviewPushButton(); break;
        case 13: _t->validate(); break;
        case 14: _t->on_fwhmunitsComboBox_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: _t->on_actionClose_triggered(); break;
        case 16: _t->on_scienceComboBox_activated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCPAbstractPlottable* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (ImageStatistics::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageStatistics::imageSelected)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ImageStatistics::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ImageStatistics.data,
      qt_meta_data_ImageStatistics,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ImageStatistics::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImageStatistics::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ImageStatistics.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int ImageStatistics::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void ImageStatistics::imageSelected(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
