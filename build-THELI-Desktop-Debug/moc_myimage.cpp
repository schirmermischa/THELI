/****************************************************************************
** Meta object code from reading C++ file 'myimage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/myimage/myimage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'myimage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyImage_t {
    QByteArrayData data[16];
    char stringdata0[192];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyImage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyImage_t qt_meta_stringdata_MyImage = {
    {
QT_MOC_LITERAL(0, 0, 7), // "MyImage"
QT_MOC_LITERAL(1, 8, 17), // "modelUpdateNeeded"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 8), // "chipName"
QT_MOC_LITERAL(4, 36, 16), // "messageAvailable"
QT_MOC_LITERAL(5, 53, 7), // "message"
QT_MOC_LITERAL(6, 61, 4), // "type"
QT_MOC_LITERAL(7, 66, 13), // "setMemoryLock"
QT_MOC_LITERAL(8, 80, 6), // "locked"
QT_MOC_LITERAL(9, 87, 10), // "setWCSLock"
QT_MOC_LITERAL(10, 98, 8), // "critical"
QT_MOC_LITERAL(11, 107, 7), // "warning"
QT_MOC_LITERAL(12, 115, 13), // "errorOccurred"
QT_MOC_LITERAL(13, 129, 24), // "messageAvailableReceived"
QT_MOC_LITERAL(14, 154, 18), // "anetOutputReceived"
QT_MOC_LITERAL(15, 173, 18) // "errorFoundReceived"

    },
    "MyImage\0modelUpdateNeeded\0\0chipName\0"
    "messageAvailable\0message\0type\0"
    "setMemoryLock\0locked\0setWCSLock\0"
    "critical\0warning\0errorOccurred\0"
    "messageAvailableReceived\0anetOutputReceived\0"
    "errorFoundReceived"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyImage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    2,   67,    2, 0x06 /* Public */,
       7,    1,   72,    2, 0x06 /* Public */,
       9,    1,   75,    2, 0x06 /* Public */,
      10,    0,   78,    2, 0x06 /* Public */,
      11,    0,   79,    2, 0x06 /* Public */,
      12,    0,   80,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    2,   81,    2, 0x0a /* Public */,
      14,    2,   86,    2, 0x0a /* Public */,
      15,    0,   91,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,    6,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,    6,
    QMetaType::Void,

       0        // eod
};

void MyImage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyImage *_t = static_cast<MyImage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->modelUpdateNeeded((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->messageAvailable((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->setMemoryLock((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->setWCSLock((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->critical(); break;
        case 5: _t->warning(); break;
        case 6: _t->errorOccurred(); break;
        case 7: _t->messageAvailableReceived((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 8: _t->anetOutputReceived((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 9: _t->errorFoundReceived(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (MyImage::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyImage::modelUpdateNeeded)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MyImage::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyImage::messageAvailable)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MyImage::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyImage::setMemoryLock)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MyImage::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyImage::setWCSLock)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (MyImage::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyImage::critical)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (MyImage::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyImage::warning)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (MyImage::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyImage::errorOccurred)) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject MyImage::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MyImage.data,
      qt_meta_data_MyImage,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MyImage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyImage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyImage.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MyImage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void MyImage::modelUpdateNeeded(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MyImage::messageAvailable(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MyImage::setMemoryLock(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MyImage::setWCSLock(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MyImage::critical()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void MyImage::warning()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void MyImage::errorOccurred()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
