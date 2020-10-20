/****************************************************************************
** Meta object code from reading C++ file 'memoryviewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/dockwidgets/memoryviewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'memoryviewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MemoryViewer_t {
    QByteArrayData data[25];
    char stringdata0[528];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MemoryViewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MemoryViewer_t qt_meta_stringdata_MemoryViewer = {
    {
QT_MOC_LITERAL(0, 0, 12), // "MemoryViewer"
QT_MOC_LITERAL(1, 13, 15), // "beginResetModel"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 13), // "endResetModel"
QT_MOC_LITERAL(4, 44, 8), // "populate"
QT_MOC_LITERAL(5, 53, 23), // "clearMemoryViewReceived"
QT_MOC_LITERAL(6, 77, 26), // "populateMemoryViewReceived"
QT_MOC_LITERAL(7, 104, 20), // "addBackupDirReceived"
QT_MOC_LITERAL(8, 125, 10), // "scienceDir"
QT_MOC_LITERAL(9, 136, 30), // "updateStatusCheckBoxesReceived"
QT_MOC_LITERAL(10, 167, 12), // "statusString"
QT_MOC_LITERAL(11, 180, 20), // "projectResetReceived"
QT_MOC_LITERAL(12, 201, 38), // "on_datadirComboBox_currentInd..."
QT_MOC_LITERAL(13, 240, 5), // "index"
QT_MOC_LITERAL(14, 246, 29), // "on_downloadToolButton_clicked"
QT_MOC_LITERAL(15, 276, 20), // "writeCheckBoxClicked"
QT_MOC_LITERAL(16, 297, 26), // "on_memoryTableView_clicked"
QT_MOC_LITERAL(17, 324, 32), // "currentlyDisplayedIndex_received"
QT_MOC_LITERAL(18, 357, 9), // "currentId"
QT_MOC_LITERAL(19, 367, 28), // "on_restorePushButton_clicked"
QT_MOC_LITERAL(20, 396, 38), // "updateProcessingStatusOnDrive..."
QT_MOC_LITERAL(21, 435, 28), // "updateStatusTipRestoreButton"
QT_MOC_LITERAL(22, 464, 37), // "on_restoreComboBox_currentTex..."
QT_MOC_LITERAL(23, 502, 4), // "arg1"
QT_MOC_LITERAL(24, 507, 20) // "iViewClosed_received"

    },
    "MemoryViewer\0beginResetModel\0\0"
    "endResetModel\0populate\0clearMemoryViewReceived\0"
    "populateMemoryViewReceived\0"
    "addBackupDirReceived\0scienceDir\0"
    "updateStatusCheckBoxesReceived\0"
    "statusString\0projectResetReceived\0"
    "on_datadirComboBox_currentIndexChanged\0"
    "index\0on_downloadToolButton_clicked\0"
    "writeCheckBoxClicked\0on_memoryTableView_clicked\0"
    "currentlyDisplayedIndex_received\0"
    "currentId\0on_restorePushButton_clicked\0"
    "updateProcessingStatusOnDriveAndInData\0"
    "updateStatusTipRestoreButton\0"
    "on_restoreComboBox_currentTextChanged\0"
    "arg1\0iViewClosed_received"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MemoryViewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  104,    2, 0x06 /* Public */,
       3,    0,  105,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,  106,    2, 0x0a /* Public */,
       5,    0,  107,    2, 0x0a /* Public */,
       6,    0,  108,    2, 0x0a /* Public */,
       7,    1,  109,    2, 0x0a /* Public */,
       9,    1,  112,    2, 0x0a /* Public */,
      11,    0,  115,    2, 0x0a /* Public */,
      12,    1,  116,    2, 0x08 /* Private */,
      14,    0,  119,    2, 0x08 /* Private */,
      15,    1,  120,    2, 0x08 /* Private */,
      16,    1,  123,    2, 0x08 /* Private */,
      17,    1,  126,    2, 0x08 /* Private */,
      19,    0,  129,    2, 0x08 /* Private */,
      20,    0,  130,    2, 0x08 /* Private */,
      21,    0,  131,    2, 0x08 /* Private */,
      22,    1,  132,    2, 0x08 /* Private */,
      24,    0,  135,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,   13,
    QMetaType::Void, QMetaType::QModelIndex,   13,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   23,
    QMetaType::Void,

       0        // eod
};

void MemoryViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MemoryViewer *_t = static_cast<MemoryViewer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->beginResetModel(); break;
        case 1: _t->endResetModel(); break;
        case 2: _t->populate(); break;
        case 3: _t->clearMemoryViewReceived(); break;
        case 4: _t->populateMemoryViewReceived(); break;
        case 5: _t->addBackupDirReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->updateStatusCheckBoxesReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->projectResetReceived(); break;
        case 8: _t->on_datadirComboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_downloadToolButton_clicked(); break;
        case 10: _t->writeCheckBoxClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 11: _t->on_memoryTableView_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 12: _t->currentlyDisplayedIndex_received((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->on_restorePushButton_clicked(); break;
        case 14: _t->updateProcessingStatusOnDriveAndInData(); break;
        case 15: _t->updateStatusTipRestoreButton(); break;
        case 16: _t->on_restoreComboBox_currentTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->iViewClosed_received(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (MemoryViewer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MemoryViewer::beginResetModel)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MemoryViewer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MemoryViewer::endResetModel)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject MemoryViewer::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_MemoryViewer.data,
      qt_meta_data_MemoryViewer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MemoryViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MemoryViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MemoryViewer.stringdata0))
        return static_cast<void*>(this);
    return QDockWidget::qt_metacast(_clname);
}

int MemoryViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void MemoryViewer::beginResetModel()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MemoryViewer::endResetModel()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
