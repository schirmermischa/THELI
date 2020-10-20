/****************************************************************************
** Meta object code from reading C++ file 'mygraphicsview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/iview/mygraphicsview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mygraphicsview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyGraphicsView_t {
    QByteArrayData data[20];
    char stringdata0[294];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyGraphicsView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyGraphicsView_t qt_meta_stringdata_MyGraphicsView = {
    {
QT_MOC_LITERAL(0, 0, 14), // "MyGraphicsView"
QT_MOC_LITERAL(1, 15, 15), // "currentMousePos"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 18), // "rightDragTravelled"
QT_MOC_LITERAL(4, 51, 19), // "middleDragTravelled"
QT_MOC_LITERAL(5, 71, 10), // "pointStart"
QT_MOC_LITERAL(6, 82, 8), // "pointEnd"
QT_MOC_LITERAL(7, 91, 18), // "middleWCSTravelled"
QT_MOC_LITERAL(8, 110, 17), // "middleWCSreleased"
QT_MOC_LITERAL(9, 128, 17), // "leftDragTravelled"
QT_MOC_LITERAL(10, 146, 10), // "rightPress"
QT_MOC_LITERAL(11, 157, 9), // "leftPress"
QT_MOC_LITERAL(12, 167, 11), // "middlePress"
QT_MOC_LITERAL(13, 179, 5), // "point"
QT_MOC_LITERAL(14, 185, 18), // "leftButtonReleased"
QT_MOC_LITERAL(15, 204, 19), // "rightButtonReleased"
QT_MOC_LITERAL(16, 224, 20), // "middleButtonReleased"
QT_MOC_LITERAL(17, 245, 21), // "middlePressResetCRPIX"
QT_MOC_LITERAL(18, 267, 21), // "updateMiddleMouseMode"
QT_MOC_LITERAL(19, 289, 4) // "mode"

    },
    "MyGraphicsView\0currentMousePos\0\0"
    "rightDragTravelled\0middleDragTravelled\0"
    "pointStart\0pointEnd\0middleWCSTravelled\0"
    "middleWCSreleased\0leftDragTravelled\0"
    "rightPress\0leftPress\0middlePress\0point\0"
    "leftButtonReleased\0rightButtonReleased\0"
    "middleButtonReleased\0middlePressResetCRPIX\0"
    "updateMiddleMouseMode\0mode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyGraphicsView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       3,    1,   87,    2, 0x06 /* Public */,
       4,    2,   90,    2, 0x06 /* Public */,
       7,    2,   95,    2, 0x06 /* Public */,
       8,    0,  100,    2, 0x06 /* Public */,
       9,    2,  101,    2, 0x06 /* Public */,
      10,    0,  106,    2, 0x06 /* Public */,
      11,    1,  107,    2, 0x06 /* Public */,
      12,    1,  110,    2, 0x06 /* Public */,
      14,    0,  113,    2, 0x06 /* Public */,
      15,    0,  114,    2, 0x06 /* Public */,
      16,    0,  115,    2, 0x06 /* Public */,
      17,    0,  116,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      18,    1,  117,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPointF,    2,
    QMetaType::Void, QMetaType::QPointF,    2,
    QMetaType::Void, QMetaType::QPointF, QMetaType::QPointF,    5,    6,
    QMetaType::Void, QMetaType::QPointF, QMetaType::QPointF,    5,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPointF, QMetaType::QPointF,    5,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPointF,    5,
    QMetaType::Void, QMetaType::QPointF,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   19,

       0        // eod
};

void MyGraphicsView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyGraphicsView *_t = static_cast<MyGraphicsView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->currentMousePos((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 1: _t->rightDragTravelled((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 2: _t->middleDragTravelled((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        case 3: _t->middleWCSTravelled((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        case 4: _t->middleWCSreleased(); break;
        case 5: _t->leftDragTravelled((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        case 6: _t->rightPress(); break;
        case 7: _t->leftPress((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 8: _t->middlePress((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 9: _t->leftButtonReleased(); break;
        case 10: _t->rightButtonReleased(); break;
        case 11: _t->middleButtonReleased(); break;
        case 12: _t->middlePressResetCRPIX(); break;
        case 13: _t->updateMiddleMouseMode((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (MyGraphicsView::*_t)(QPointF );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyGraphicsView::currentMousePos)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MyGraphicsView::*_t)(QPointF );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyGraphicsView::rightDragTravelled)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MyGraphicsView::*_t)(QPointF , QPointF );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyGraphicsView::middleDragTravelled)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MyGraphicsView::*_t)(QPointF , QPointF );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyGraphicsView::middleWCSTravelled)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (MyGraphicsView::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyGraphicsView::middleWCSreleased)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (MyGraphicsView::*_t)(QPointF , QPointF );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyGraphicsView::leftDragTravelled)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (MyGraphicsView::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyGraphicsView::rightPress)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (MyGraphicsView::*_t)(QPointF );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyGraphicsView::leftPress)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (MyGraphicsView::*_t)(QPointF );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyGraphicsView::middlePress)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (MyGraphicsView::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyGraphicsView::leftButtonReleased)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (MyGraphicsView::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyGraphicsView::rightButtonReleased)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (MyGraphicsView::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyGraphicsView::middleButtonReleased)) {
                *result = 11;
                return;
            }
        }
        {
            typedef void (MyGraphicsView::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyGraphicsView::middlePressResetCRPIX)) {
                *result = 12;
                return;
            }
        }
    }
}

const QMetaObject MyGraphicsView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_MyGraphicsView.data,
      qt_meta_data_MyGraphicsView,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MyGraphicsView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyGraphicsView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyGraphicsView.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int MyGraphicsView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void MyGraphicsView::currentMousePos(QPointF _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MyGraphicsView::rightDragTravelled(QPointF _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MyGraphicsView::middleDragTravelled(QPointF _t1, QPointF _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MyGraphicsView::middleWCSTravelled(QPointF _t1, QPointF _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MyGraphicsView::middleWCSreleased()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void MyGraphicsView::leftDragTravelled(QPointF _t1, QPointF _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MyGraphicsView::rightPress()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void MyGraphicsView::leftPress(QPointF _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void MyGraphicsView::middlePress(QPointF _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void MyGraphicsView::leftButtonReleased()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void MyGraphicsView::rightButtonReleased()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void MyGraphicsView::middleButtonReleased()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void MyGraphicsView::middlePressResetCRPIX()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
