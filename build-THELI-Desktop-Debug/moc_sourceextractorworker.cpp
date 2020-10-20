/****************************************************************************
** Meta object code from reading C++ file 'sourceextractorworker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/threading/sourceextractorworker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sourceextractorworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SourceExtractorWorker_t {
    QByteArrayData data[9];
    char stringdata0[127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SourceExtractorWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SourceExtractorWorker_t qt_meta_stringdata_SourceExtractorWorker = {
    {
QT_MOC_LITERAL(0, 0, 21), // "SourceExtractorWorker"
QT_MOC_LITERAL(1, 22, 10), // "errorFound"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 16), // "messageAvailable"
QT_MOC_LITERAL(4, 51, 7), // "message"
QT_MOC_LITERAL(5, 59, 4), // "type"
QT_MOC_LITERAL(6, 64, 18), // "runSourceExtractor"
QT_MOC_LITERAL(7, 83, 21), // "processExternalStdout"
QT_MOC_LITERAL(8, 105, 21) // "processExternalStderr"

    },
    "SourceExtractorWorker\0errorFound\0\0"
    "messageAvailable\0message\0type\0"
    "runSourceExtractor\0processExternalStdout\0"
    "processExternalStderr"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SourceExtractorWorker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    2,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   45,    2, 0x0a /* Public */,
       7,    0,   46,    2, 0x08 /* Private */,
       8,    0,   47,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    4,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SourceExtractorWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SourceExtractorWorker *_t = static_cast<SourceExtractorWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->errorFound(); break;
        case 1: _t->messageAvailable((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->runSourceExtractor(); break;
        case 3: _t->processExternalStdout(); break;
        case 4: _t->processExternalStderr(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SourceExtractorWorker::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SourceExtractorWorker::errorFound)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SourceExtractorWorker::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SourceExtractorWorker::messageAvailable)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject SourceExtractorWorker::staticMetaObject = {
    { &Worker::staticMetaObject, qt_meta_stringdata_SourceExtractorWorker.data,
      qt_meta_data_SourceExtractorWorker,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SourceExtractorWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SourceExtractorWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SourceExtractorWorker.stringdata0))
        return static_cast<void*>(this);
    return Worker::qt_metacast(_clname);
}

int SourceExtractorWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Worker::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SourceExtractorWorker::errorFound()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SourceExtractorWorker::messageAvailable(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
