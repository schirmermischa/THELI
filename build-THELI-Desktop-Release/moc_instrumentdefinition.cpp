/****************************************************************************
** Meta object code from reading C++ file 'instrumentdefinition.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/instrumentdefinition.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'instrumentdefinition.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Instrument_t {
    QByteArrayData data[19];
    char stringdata0[386];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Instrument_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Instrument_t qt_meta_stringdata_Instrument = {
    {
QT_MOC_LITERAL(0, 0, 10), // "Instrument"
QT_MOC_LITERAL(1, 11, 17), // "paintNumLineEdits"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 8), // "geometry"
QT_MOC_LITERAL(4, 39, 30), // "on_numchipSpinBox_valueChanged"
QT_MOC_LITERAL(5, 70, 4), // "arg1"
QT_MOC_LITERAL(6, 75, 8), // "validate"
QT_MOC_LITERAL(7, 84, 31), // "on_saveConfigPushButton_clicked"
QT_MOC_LITERAL(8, 116, 45), // "on_instrumentTypeComboBox_cur..."
QT_MOC_LITERAL(9, 162, 5), // "index"
QT_MOC_LITERAL(10, 168, 31), // "on_loadConfigPushButton_clicked"
QT_MOC_LITERAL(11, 200, 24), // "toggle_bayer_ToolButtons"
QT_MOC_LITERAL(12, 225, 15), // "timerConfigDone"
QT_MOC_LITERAL(13, 241, 26), // "on_clearPushButton_clicked"
QT_MOC_LITERAL(14, 268, 24), // "on_bayerCheckBox_clicked"
QT_MOC_LITERAL(15, 293, 7), // "checked"
QT_MOC_LITERAL(16, 301, 24), // "on_actionClose_triggered"
QT_MOC_LITERAL(17, 326, 22), // "toggleFormatPushButton"
QT_MOC_LITERAL(18, 349, 36) // "on_readRAWgeometryPushButton_..."

    },
    "Instrument\0paintNumLineEdits\0\0geometry\0"
    "on_numchipSpinBox_valueChanged\0arg1\0"
    "validate\0on_saveConfigPushButton_clicked\0"
    "on_instrumentTypeComboBox_currentIndexChanged\0"
    "index\0on_loadConfigPushButton_clicked\0"
    "toggle_bayer_ToolButtons\0timerConfigDone\0"
    "on_clearPushButton_clicked\0"
    "on_bayerCheckBox_clicked\0checked\0"
    "on_actionClose_triggered\0"
    "toggleFormatPushButton\0"
    "on_readRAWgeometryPushButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Instrument[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x08 /* Private */,
       4,    1,   82,    2, 0x08 /* Private */,
       6,    1,   85,    2, 0x08 /* Private */,
       7,    0,   88,    2, 0x08 /* Private */,
       8,    1,   89,    2, 0x08 /* Private */,
      10,    0,   92,    2, 0x08 /* Private */,
      11,    0,   93,    2, 0x08 /* Private */,
      12,    0,   94,    2, 0x08 /* Private */,
      13,    0,   95,    2, 0x08 /* Private */,
      14,    1,   96,    2, 0x08 /* Private */,
      16,    0,   99,    2, 0x08 /* Private */,
      17,    0,  100,    2, 0x08 /* Private */,
      18,    0,  101,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Instrument::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Instrument *_t = static_cast<Instrument *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->paintNumLineEdits((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->on_numchipSpinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->validate((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->on_saveConfigPushButton_clicked(); break;
        case 4: _t->on_instrumentTypeComboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_loadConfigPushButton_clicked(); break;
        case 6: _t->toggle_bayer_ToolButtons(); break;
        case 7: _t->timerConfigDone(); break;
        case 8: _t->on_clearPushButton_clicked(); break;
        case 9: _t->on_bayerCheckBox_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->on_actionClose_triggered(); break;
        case 11: _t->toggleFormatPushButton(); break;
        case 12: _t->on_readRAWgeometryPushButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject Instrument::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Instrument.data,
      qt_meta_data_Instrument,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Instrument::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Instrument::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Instrument.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Instrument::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
