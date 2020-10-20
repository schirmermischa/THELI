/****************************************************************************
** Meta object code from reading C++ file 'datamodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/datamodel/datamodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'datamodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DataModel_t {
    QByteArrayData data[14];
    char stringdata0[182];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DataModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DataModel_t qt_meta_stringdata_DataModel = {
    {
QT_MOC_LITERAL(0, 0, 9), // "DataModel"
QT_MOC_LITERAL(1, 10, 19), // "modelUpdateReceiver"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 8), // "chipName"
QT_MOC_LITERAL(4, 40, 16), // "updateHeaderLine"
QT_MOC_LITERAL(5, 57, 3), // "row"
QT_MOC_LITERAL(6, 61, 22), // "updateHeaderLineGlobal"
QT_MOC_LITERAL(7, 84, 23), // "beginResetModelReceived"
QT_MOC_LITERAL(8, 108, 21), // "endResetModelReceived"
QT_MOC_LITERAL(9, 130, 10), // "headerData"
QT_MOC_LITERAL(10, 141, 7), // "section"
QT_MOC_LITERAL(11, 149, 15), // "Qt::Orientation"
QT_MOC_LITERAL(12, 165, 11), // "orientation"
QT_MOC_LITERAL(13, 177, 4) // "role"

    },
    "DataModel\0modelUpdateReceiver\0\0chipName\0"
    "updateHeaderLine\0row\0updateHeaderLineGlobal\0"
    "beginResetModelReceived\0endResetModelReceived\0"
    "headerData\0section\0Qt::Orientation\0"
    "orientation\0role"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x08 /* Private */,
       4,    1,   52,    2, 0x08 /* Private */,
       6,    0,   55,    2, 0x08 /* Private */,
       7,    0,   56,    2, 0x0a /* Public */,
       8,    0,   57,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
       9,    3,   58,    2, 0x02 /* Public */,
       9,    2,   65,    2, 0x22 /* Public | MethodCloned */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Long,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::QVariant, QMetaType::Int, 0x80000000 | 11, QMetaType::Int,   10,   12,   13,
    QMetaType::QVariant, QMetaType::Int, 0x80000000 | 11,   10,   12,

       0        // eod
};

void DataModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DataModel *_t = static_cast<DataModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->modelUpdateReceiver((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->updateHeaderLine((*reinterpret_cast< long(*)>(_a[1]))); break;
        case 2: _t->updateHeaderLineGlobal(); break;
        case 3: _t->beginResetModelReceived(); break;
        case 4: _t->endResetModelReceived(); break;
        case 5: { QVariant _r = _t->headerData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Qt::Orientation(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = std::move(_r); }  break;
        case 6: { QVariant _r = _t->headerData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Qt::Orientation(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject DataModel::staticMetaObject = {
    { &QAbstractTableModel::staticMetaObject, qt_meta_stringdata_DataModel.data,
      qt_meta_data_DataModel,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DataModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DataModel.stringdata0))
        return static_cast<void*>(this);
    return QAbstractTableModel::qt_metacast(_clname);
}

int DataModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
