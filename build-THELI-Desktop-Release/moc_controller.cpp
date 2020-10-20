/****************************************************************************
** Meta object code from reading C++ file 'controller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/processingInternal/controller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Controller_t {
    QByteArrayData data[98];
    char stringdata0[1793];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Controller_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Controller_t qt_meta_stringdata_Controller = {
    {
QT_MOC_LITERAL(0, 0, 10), // "Controller"
QT_MOC_LITERAL(1, 11, 10), // "loadViewer"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 7), // "dirname"
QT_MOC_LITERAL(4, 31, 6), // "filter"
QT_MOC_LITERAL(5, 38, 4), // "mode"
QT_MOC_LITERAL(6, 43, 16), // "messageAvailable"
QT_MOC_LITERAL(7, 60, 7), // "message"
QT_MOC_LITERAL(8, 68, 4), // "type"
QT_MOC_LITERAL(9, 73, 8), // "appendOK"
QT_MOC_LITERAL(10, 82, 14), // "showMessageBox"
QT_MOC_LITERAL(11, 97, 7), // "trigger"
QT_MOC_LITERAL(12, 105, 5), // "part1"
QT_MOC_LITERAL(13, 111, 5), // "part2"
QT_MOC_LITERAL(14, 117, 14), // "progressUpdate"
QT_MOC_LITERAL(15, 132, 8), // "progress"
QT_MOC_LITERAL(16, 141, 14), // "targetResolved"
QT_MOC_LITERAL(17, 156, 5), // "alpha"
QT_MOC_LITERAL(18, 162, 5), // "delta"
QT_MOC_LITERAL(19, 168, 16), // "resetProgressBar"
QT_MOC_LITERAL(20, 185, 20), // "swarpStartResampling"
QT_MOC_LITERAL(21, 206, 20), // "swarpStartCoaddition"
QT_MOC_LITERAL(22, 227, 21), // "swarpStartSwarpfilter"
QT_MOC_LITERAL(23, 249, 16), // "swarpStartUpdate"
QT_MOC_LITERAL(24, 266, 19), // "showScampCheckPlots"
QT_MOC_LITERAL(25, 286, 21), // "scienceDataDirUpdated"
QT_MOC_LITERAL(26, 308, 6), // "allDir"
QT_MOC_LITERAL(27, 315, 15), // "clearMemoryView"
QT_MOC_LITERAL(28, 331, 18), // "populateMemoryView"
QT_MOC_LITERAL(29, 350, 21), // "stopFileProgressTimer"
QT_MOC_LITERAL(30, 372, 26), // "addBackupDirToMemoryviewer"
QT_MOC_LITERAL(31, 399, 10), // "scienceDir"
QT_MOC_LITERAL(32, 410, 9), // "loadAbsZP"
QT_MOC_LITERAL(33, 420, 14), // "coaddImagePath"
QT_MOC_LITERAL(34, 435, 6), // "maxVal"
QT_MOC_LITERAL(35, 442, 23), // "updateMemoryProgressBar"
QT_MOC_LITERAL(36, 466, 14), // "splitterMemory"
QT_MOC_LITERAL(37, 481, 11), // "forceFinish"
QT_MOC_LITERAL(38, 493, 15), // "displayProgress"
QT_MOC_LITERAL(39, 509, 23), // "taskInternalProcessbias"
QT_MOC_LITERAL(40, 533, 23), // "taskInternalProcessdark"
QT_MOC_LITERAL(41, 557, 26), // "taskInternalProcessflatoff"
QT_MOC_LITERAL(42, 584, 23), // "taskInternalProcessflat"
QT_MOC_LITERAL(43, 608, 26), // "taskInternalProcessscience"
QT_MOC_LITERAL(44, 635, 22), // "taskInternalBackground"
QT_MOC_LITERAL(45, 658, 20), // "taskInternalCollapse"
QT_MOC_LITERAL(46, 679, 25), // "taskInternalBinnedpreview"
QT_MOC_LITERAL(47, 705, 24), // "taskInternalGlobalweight"
QT_MOC_LITERAL(48, 730, 28), // "taskInternalIndividualweight"
QT_MOC_LITERAL(49, 759, 27), // "taskInternalCreatesourcecat"
QT_MOC_LITERAL(50, 787, 18), // "taskInternalSkysub"
QT_MOC_LITERAL(51, 806, 22), // "taskInternalCoaddition"
QT_MOC_LITERAL(52, 829, 29), // "taskInternalGetCatalogFromWEB"
QT_MOC_LITERAL(53, 859, 31), // "taskInternalGetCatalogFromIMAGE"
QT_MOC_LITERAL(54, 891, 23), // "taskInternalHDUreformat"
QT_MOC_LITERAL(55, 915, 25), // "taskInternalResolveTarget"
QT_MOC_LITERAL(56, 941, 25), // "taskInternalRestoreHeader"
QT_MOC_LITERAL(57, 967, 24), // "taskInternalAstromphotom"
QT_MOC_LITERAL(58, 992, 20), // "taskInternalSeparate"
QT_MOC_LITERAL(59, 1013, 27), // "finishedPreparationReceived"
QT_MOC_LITERAL(60, 1041, 24), // "waitForResamplingThreads"
QT_MOC_LITERAL(61, 1066, 8), // "threadID"
QT_MOC_LITERAL(62, 1075, 21), // "finishedScampReceived"
QT_MOC_LITERAL(63, 1097, 20), // "fieldMatchedReceived"
QT_MOC_LITERAL(64, 1118, 27), // "showScampCheckPlotsReceived"
QT_MOC_LITERAL(65, 1146, 31), // "registerScampSolutionAcceptance"
QT_MOC_LITERAL(66, 1178, 21), // "scampSolutionAccepted"
QT_MOC_LITERAL(67, 1200, 25), // "continueWithCopyZeroOrder"
QT_MOC_LITERAL(68, 1226, 13), // "copyZeroOrder"
QT_MOC_LITERAL(69, 1240, 18), // "errorFoundReceived"
QT_MOC_LITERAL(70, 1259, 24), // "addToProgressBarReceived"
QT_MOC_LITERAL(71, 1284, 12), // "differential"
QT_MOC_LITERAL(72, 1297, 9), // "updateAll"
QT_MOC_LITERAL(73, 1307, 15), // "loadPreferences"
QT_MOC_LITERAL(74, 1323, 22), // "updateMemoryPreference"
QT_MOC_LITERAL(75, 1346, 14), // "isRAMminimized"
QT_MOC_LITERAL(76, 1361, 32), // "updateIntermediateDataPreference"
QT_MOC_LITERAL(77, 1394, 26), // "intermediateDataPreference"
QT_MOC_LITERAL(78, 1421, 16), // "criticalReceived"
QT_MOC_LITERAL(79, 1438, 15), // "warningReceived"
QT_MOC_LITERAL(80, 1454, 24), // "messageAvailableReceived"
QT_MOC_LITERAL(81, 1479, 16), // "appendOKReceived"
QT_MOC_LITERAL(82, 1496, 22), // "showMessageBoxReceived"
QT_MOC_LITERAL(83, 1519, 15), // "updateVerbosity"
QT_MOC_LITERAL(84, 1535, 14), // "verbosityLevel"
QT_MOC_LITERAL(85, 1550, 22), // "dataTreeEditedReceived"
QT_MOC_LITERAL(86, 1573, 24), // "newProjectLoadedReceived"
QT_MOC_LITERAL(87, 1598, 13), // "coaddResample"
QT_MOC_LITERAL(88, 1612, 16), // "coaddSwarpfilter"
QT_MOC_LITERAL(89, 1629, 15), // "coaddCoaddition"
QT_MOC_LITERAL(90, 1645, 11), // "coaddUpdate"
QT_MOC_LITERAL(91, 1657, 28), // "rereadScienceDataDirReceived"
QT_MOC_LITERAL(92, 1686, 21), // "setMemoryLockReceived"
QT_MOC_LITERAL(93, 1708, 6), // "locked"
QT_MOC_LITERAL(94, 1715, 18), // "setWCSLockReceived"
QT_MOC_LITERAL(95, 1734, 25), // "absZeroPointCloseReceived"
QT_MOC_LITERAL(96, 1760, 24), // "resetErrorStatusReceived"
QT_MOC_LITERAL(97, 1785, 7) // "dirName"

    },
    "Controller\0loadViewer\0\0dirname\0filter\0"
    "mode\0messageAvailable\0message\0type\0"
    "appendOK\0showMessageBox\0trigger\0part1\0"
    "part2\0progressUpdate\0progress\0"
    "targetResolved\0alpha\0delta\0resetProgressBar\0"
    "swarpStartResampling\0swarpStartCoaddition\0"
    "swarpStartSwarpfilter\0swarpStartUpdate\0"
    "showScampCheckPlots\0scienceDataDirUpdated\0"
    "allDir\0clearMemoryView\0populateMemoryView\0"
    "stopFileProgressTimer\0addBackupDirToMemoryviewer\0"
    "scienceDir\0loadAbsZP\0coaddImagePath\0"
    "maxVal\0updateMemoryProgressBar\0"
    "splitterMemory\0forceFinish\0displayProgress\0"
    "taskInternalProcessbias\0taskInternalProcessdark\0"
    "taskInternalProcessflatoff\0"
    "taskInternalProcessflat\0"
    "taskInternalProcessscience\0"
    "taskInternalBackground\0taskInternalCollapse\0"
    "taskInternalBinnedpreview\0"
    "taskInternalGlobalweight\0"
    "taskInternalIndividualweight\0"
    "taskInternalCreatesourcecat\0"
    "taskInternalSkysub\0taskInternalCoaddition\0"
    "taskInternalGetCatalogFromWEB\0"
    "taskInternalGetCatalogFromIMAGE\0"
    "taskInternalHDUreformat\0"
    "taskInternalResolveTarget\0"
    "taskInternalRestoreHeader\0"
    "taskInternalAstromphotom\0taskInternalSeparate\0"
    "finishedPreparationReceived\0"
    "waitForResamplingThreads\0threadID\0"
    "finishedScampReceived\0fieldMatchedReceived\0"
    "showScampCheckPlotsReceived\0"
    "registerScampSolutionAcceptance\0"
    "scampSolutionAccepted\0continueWithCopyZeroOrder\0"
    "copyZeroOrder\0errorFoundReceived\0"
    "addToProgressBarReceived\0differential\0"
    "updateAll\0loadPreferences\0"
    "updateMemoryPreference\0isRAMminimized\0"
    "updateIntermediateDataPreference\0"
    "intermediateDataPreference\0criticalReceived\0"
    "warningReceived\0messageAvailableReceived\0"
    "appendOKReceived\0showMessageBoxReceived\0"
    "updateVerbosity\0verbosityLevel\0"
    "dataTreeEditedReceived\0newProjectLoadedReceived\0"
    "coaddResample\0coaddSwarpfilter\0"
    "coaddCoaddition\0coaddUpdate\0"
    "rereadScienceDataDirReceived\0"
    "setMemoryLockReceived\0locked\0"
    "setWCSLockReceived\0absZeroPointCloseReceived\0"
    "resetErrorStatusReceived\0dirName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Controller[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      72,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      20,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,  374,    2, 0x06 /* Public */,
       6,    2,  381,    2, 0x06 /* Public */,
       9,    0,  386,    2, 0x06 /* Public */,
      10,    3,  387,    2, 0x06 /* Public */,
      14,    1,  394,    2, 0x06 /* Public */,
      16,    2,  397,    2, 0x06 /* Public */,
      19,    0,  402,    2, 0x06 /* Public */,
      20,    0,  403,    2, 0x06 /* Public */,
      21,    0,  404,    2, 0x06 /* Public */,
      22,    0,  405,    2, 0x06 /* Public */,
      23,    0,  406,    2, 0x06 /* Public */,
      24,    0,  407,    2, 0x06 /* Public */,
      25,    1,  408,    2, 0x06 /* Public */,
      27,    0,  411,    2, 0x06 /* Public */,
      28,    0,  412,    2, 0x06 /* Public */,
      29,    0,  413,    2, 0x06 /* Public */,
      30,    1,  414,    2, 0x06 /* Public */,
      32,    2,  417,    2, 0x06 /* Public */,
      35,    1,  422,    2, 0x06 /* Public */,
      37,    0,  425,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      38,    0,  426,    2, 0x08 /* Private */,
      39,    0,  427,    2, 0x08 /* Private */,
      40,    0,  428,    2, 0x08 /* Private */,
      41,    0,  429,    2, 0x08 /* Private */,
      42,    0,  430,    2, 0x08 /* Private */,
      43,    0,  431,    2, 0x08 /* Private */,
      44,    0,  432,    2, 0x08 /* Private */,
      45,    0,  433,    2, 0x08 /* Private */,
      46,    0,  434,    2, 0x08 /* Private */,
      47,    0,  435,    2, 0x08 /* Private */,
      48,    0,  436,    2, 0x08 /* Private */,
      49,    0,  437,    2, 0x08 /* Private */,
      50,    0,  438,    2, 0x08 /* Private */,
      51,    0,  439,    2, 0x08 /* Private */,
      52,    0,  440,    2, 0x08 /* Private */,
      53,    0,  441,    2, 0x08 /* Private */,
      54,    0,  442,    2, 0x08 /* Private */,
      55,    0,  443,    2, 0x08 /* Private */,
      56,    0,  444,    2, 0x08 /* Private */,
      57,    0,  445,    2, 0x08 /* Private */,
      58,    0,  446,    2, 0x08 /* Private */,
      59,    0,  447,    2, 0x08 /* Private */,
      60,    1,  448,    2, 0x08 /* Private */,
      62,    0,  451,    2, 0x08 /* Private */,
      63,    0,  452,    2, 0x08 /* Private */,
      64,    0,  453,    2, 0x08 /* Private */,
      65,    1,  454,    2, 0x08 /* Private */,
      67,    0,  457,    2, 0x08 /* Private */,
      68,    0,  458,    2, 0x08 /* Private */,
      69,    0,  459,    2, 0x08 /* Private */,
      70,    1,  460,    2, 0x08 /* Private */,
      72,    0,  463,    2, 0x0a /* Public */,
      73,    0,  464,    2, 0x0a /* Public */,
      74,    1,  465,    2, 0x0a /* Public */,
      76,    1,  468,    2, 0x0a /* Public */,
      78,    0,  471,    2, 0x0a /* Public */,
      79,    0,  472,    2, 0x0a /* Public */,
      80,    2,  473,    2, 0x0a /* Public */,
      81,    0,  478,    2, 0x0a /* Public */,
      82,    3,  479,    2, 0x0a /* Public */,
      83,    1,  486,    2, 0x0a /* Public */,
      85,    0,  489,    2, 0x0a /* Public */,
      86,    0,  490,    2, 0x0a /* Public */,
      87,    0,  491,    2, 0x0a /* Public */,
      88,    0,  492,    2, 0x0a /* Public */,
      89,    0,  493,    2, 0x0a /* Public */,
      90,    0,  494,    2, 0x0a /* Public */,
      91,    0,  495,    2, 0x0a /* Public */,
      92,    1,  496,    2, 0x0a /* Public */,
      94,    1,  499,    2, 0x0a /* Public */,
      95,    0,  502,    2, 0x0a /* Public */,
      96,    1,  503,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    4,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    7,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   11,   12,   13,
    QMetaType::Void, QMetaType::Float,   15,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   17,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   26,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   31,
    QMetaType::Void, QMetaType::QString, QMetaType::Float,   33,   34,
    QMetaType::Void, QMetaType::Long,   36,
    QMetaType::Void,

 // slots: parameters
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
    QMetaType::Void, QMetaType::Int,   61,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   66,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,   71,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   75,
    QMetaType::Void, QMetaType::QString,   77,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    7,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   11,   12,   13,
    QMetaType::Void, QMetaType::Int,   84,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   93,
    QMetaType::Void, QMetaType::Bool,   93,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   97,

       0        // eod
};

void Controller::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Controller *_t = static_cast<Controller *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->loadViewer((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 1: _t->messageAvailable((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->appendOK(); break;
        case 3: _t->showMessageBox((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 4: _t->progressUpdate((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 5: _t->targetResolved((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: _t->resetProgressBar(); break;
        case 7: _t->swarpStartResampling(); break;
        case 8: _t->swarpStartCoaddition(); break;
        case 9: _t->swarpStartSwarpfilter(); break;
        case 10: _t->swarpStartUpdate(); break;
        case 11: _t->showScampCheckPlots(); break;
        case 12: _t->scienceDataDirUpdated((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->clearMemoryView(); break;
        case 14: _t->populateMemoryView(); break;
        case 15: _t->stopFileProgressTimer(); break;
        case 16: _t->addBackupDirToMemoryviewer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 17: _t->loadAbsZP((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 18: _t->updateMemoryProgressBar((*reinterpret_cast< long(*)>(_a[1]))); break;
        case 19: _t->forceFinish(); break;
        case 20: _t->displayProgress(); break;
        case 21: _t->taskInternalProcessbias(); break;
        case 22: _t->taskInternalProcessdark(); break;
        case 23: _t->taskInternalProcessflatoff(); break;
        case 24: _t->taskInternalProcessflat(); break;
        case 25: _t->taskInternalProcessscience(); break;
        case 26: _t->taskInternalBackground(); break;
        case 27: _t->taskInternalCollapse(); break;
        case 28: _t->taskInternalBinnedpreview(); break;
        case 29: _t->taskInternalGlobalweight(); break;
        case 30: _t->taskInternalIndividualweight(); break;
        case 31: _t->taskInternalCreatesourcecat(); break;
        case 32: _t->taskInternalSkysub(); break;
        case 33: _t->taskInternalCoaddition(); break;
        case 34: _t->taskInternalGetCatalogFromWEB(); break;
        case 35: _t->taskInternalGetCatalogFromIMAGE(); break;
        case 36: _t->taskInternalHDUreformat(); break;
        case 37: _t->taskInternalResolveTarget(); break;
        case 38: _t->taskInternalRestoreHeader(); break;
        case 39: _t->taskInternalAstromphotom(); break;
        case 40: _t->taskInternalSeparate(); break;
        case 41: _t->finishedPreparationReceived(); break;
        case 42: _t->waitForResamplingThreads((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 43: _t->finishedScampReceived(); break;
        case 44: _t->fieldMatchedReceived(); break;
        case 45: _t->showScampCheckPlotsReceived(); break;
        case 46: _t->registerScampSolutionAcceptance((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 47: _t->continueWithCopyZeroOrder(); break;
        case 48: _t->copyZeroOrder(); break;
        case 49: _t->errorFoundReceived(); break;
        case 50: _t->addToProgressBarReceived((*reinterpret_cast< const float(*)>(_a[1]))); break;
        case 51: _t->updateAll(); break;
        case 52: _t->loadPreferences(); break;
        case 53: _t->updateMemoryPreference((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 54: _t->updateIntermediateDataPreference((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 55: _t->criticalReceived(); break;
        case 56: _t->warningReceived(); break;
        case 57: _t->messageAvailableReceived((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 58: _t->appendOKReceived(); break;
        case 59: _t->showMessageBoxReceived((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 60: _t->updateVerbosity((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 61: _t->dataTreeEditedReceived(); break;
        case 62: _t->newProjectLoadedReceived(); break;
        case 63: _t->coaddResample(); break;
        case 64: _t->coaddSwarpfilter(); break;
        case 65: _t->coaddCoaddition(); break;
        case 66: _t->coaddUpdate(); break;
        case 67: _t->rereadScienceDataDirReceived(); break;
        case 68: _t->setMemoryLockReceived((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 69: _t->setWCSLockReceived((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 70: _t->absZeroPointCloseReceived(); break;
        case 71: _t->resetErrorStatusReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (Controller::*_t)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::loadViewer)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Controller::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::messageAvailable)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::appendOK)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (Controller::*_t)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::showMessageBox)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (Controller::*_t)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::progressUpdate)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (Controller::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::targetResolved)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::resetProgressBar)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::swarpStartResampling)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::swarpStartCoaddition)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::swarpStartSwarpfilter)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::swarpStartUpdate)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::showScampCheckPlots)) {
                *result = 11;
                return;
            }
        }
        {
            typedef void (Controller::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::scienceDataDirUpdated)) {
                *result = 12;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::clearMemoryView)) {
                *result = 13;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::populateMemoryView)) {
                *result = 14;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::stopFileProgressTimer)) {
                *result = 15;
                return;
            }
        }
        {
            typedef void (Controller::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::addBackupDirToMemoryviewer)) {
                *result = 16;
                return;
            }
        }
        {
            typedef void (Controller::*_t)(QString , float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::loadAbsZP)) {
                *result = 17;
                return;
            }
        }
        {
            typedef void (Controller::*_t)(long );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::updateMemoryProgressBar)) {
                *result = 18;
                return;
            }
        }
        {
            typedef void (Controller::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Controller::forceFinish)) {
                *result = 19;
                return;
            }
        }
    }
}

const QMetaObject Controller::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Controller.data,
      qt_meta_data_Controller,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Controller::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Controller::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Controller.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Controller::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 72)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 72;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 72)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 72;
    }
    return _id;
}

// SIGNAL 0
void Controller::loadViewer(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Controller::messageAvailable(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Controller::appendOK()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Controller::showMessageBox(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Controller::progressUpdate(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Controller::targetResolved(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Controller::resetProgressBar()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void Controller::swarpStartResampling()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void Controller::swarpStartCoaddition()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void Controller::swarpStartSwarpfilter()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void Controller::swarpStartUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void Controller::showScampCheckPlots()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void Controller::scienceDataDirUpdated(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void Controller::clearMemoryView()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void Controller::populateMemoryView()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void Controller::stopFileProgressTimer()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void Controller::addBackupDirToMemoryviewer(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void Controller::loadAbsZP(QString _t1, float _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void Controller::updateMemoryProgressBar(long _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void Controller::forceFinish()
{
    QMetaObject::activate(this, &staticMetaObject, 19, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
