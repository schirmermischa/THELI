/****************************************************************************
** Meta object code from reading C++ file 'swarpworker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/threading/swarpworker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'swarpworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SwarpWorker_t {
    QByteArrayData data[12];
    char stringdata0[152];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SwarpWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SwarpWorker_t qt_meta_stringdata_SwarpWorker = {
    {
QT_MOC_LITERAL(0, 0, 11), // "SwarpWorker"
QT_MOC_LITERAL(1, 12, 19), // "finishedPreparation"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 10), // "errorFound"
QT_MOC_LITERAL(4, 44, 18), // "finishedResampling"
QT_MOC_LITERAL(5, 63, 8), // "threadID"
QT_MOC_LITERAL(6, 72, 18), // "finishedCoaddition"
QT_MOC_LITERAL(7, 91, 16), // "messageAvailable"
QT_MOC_LITERAL(8, 108, 7), // "message"
QT_MOC_LITERAL(9, 116, 4), // "type"
QT_MOC_LITERAL(10, 121, 8), // "runSwarp"
QT_MOC_LITERAL(11, 130, 21) // "processExternalStderr"

    },
    "SwarpWorker\0finishedPreparation\0\0"
    "errorFound\0finishedResampling\0threadID\0"
    "finishedCoaddition\0messageAvailable\0"
    "message\0type\0runSwarp\0processExternalStderr"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SwarpWorker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    0,   50,    2, 0x06 /* Public */,
       4,    1,   51,    2, 0x06 /* Public */,
       6,    0,   54,    2, 0x06 /* Public */,
       7,    2,   55,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   60,    2, 0x0a /* Public */,
      11,    0,   61,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    8,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SwarpWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SwarpWorker *_t = static_cast<SwarpWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finishedPreparation(); break;
        case 1: _t->errorFound(); break;
        case 2: _t->finishedResampling((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->finishedCoaddition(); break;
        case 4: _t->messageAvailable((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->runSwarp(); break;
        case 6: _t->processExternalStderr(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SwarpWorker::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SwarpWorker::finishedPreparation)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SwarpWorker::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SwarpWorker::errorFound)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (SwarpWorker::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SwarpWorker::finishedResampling)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (SwarpWorker::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SwarpWorker::finishedCoaddition)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (SwarpWorker::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SwarpWorker::messageAvailable)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject SwarpWorker::staticMetaObject = {
    { &Worker::staticMetaObject, qt_meta_stringdata_SwarpWorker.data,
      qt_meta_data_SwarpWorker,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SwarpWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SwarpWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SwarpWorker.stringdata0))
        return static_cast<void*>(this);
    return Worker::qt_metacast(_clname);
}

int SwarpWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Worker::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void SwarpWorker::finishedPreparation()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SwarpWorker::errorFound()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SwarpWorker::finishedResampling(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SwarpWorker::finishedCoaddition()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void SwarpWorker::messageAvailable(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
