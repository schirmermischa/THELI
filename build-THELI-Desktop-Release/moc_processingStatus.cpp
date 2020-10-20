/****************************************************************************
** Meta object code from reading C++ file 'processingStatus.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/processingStatus/processingStatus.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'processingStatus.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ProcessingStatus_t {
    QByteArrayData data[7];
    char stringdata0[87];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ProcessingStatus_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ProcessingStatus_t qt_meta_stringdata_ProcessingStatus = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ProcessingStatus"
QT_MOC_LITERAL(1, 17, 23), // "processingStatusChanged"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 16), // "messageAvailable"
QT_MOC_LITERAL(4, 59, 13), // "messageString"
QT_MOC_LITERAL(5, 73, 4), // "code"
QT_MOC_LITERAL(6, 78, 8) // "critical"

    },
    "ProcessingStatus\0processingStatusChanged\0"
    "\0messageAvailable\0messageString\0code\0"
    "critical"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ProcessingStatus[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    2,   30,    2, 0x06 /* Public */,
       6,    0,   35,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    4,    5,
    QMetaType::Void,

       0        // eod
};

void ProcessingStatus::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ProcessingStatus *_t = static_cast<ProcessingStatus *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->processingStatusChanged(); break;
        case 1: _t->messageAvailable((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->critical(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (ProcessingStatus::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ProcessingStatus::processingStatusChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ProcessingStatus::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ProcessingStatus::messageAvailable)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (ProcessingStatus::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ProcessingStatus::critical)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject ProcessingStatus::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ProcessingStatus.data,
      qt_meta_data_ProcessingStatus,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ProcessingStatus::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProcessingStatus::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ProcessingStatus.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ProcessingStatus::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ProcessingStatus::processingStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ProcessingStatus::messageAvailable(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ProcessingStatus::critical()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
