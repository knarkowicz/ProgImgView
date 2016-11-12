/****************************************************************************
** Meta object code from reading C++ file 'mainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "mainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CMainWindow_t {
    QByteArrayData data[23];
    char stringdata0[235];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CMainWindow_t qt_meta_stringdata_CMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CMainWindow"
QT_MOC_LITERAL(1, 12, 4), // "Open"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 7), // "Compare"
QT_MOC_LITERAL(4, 26, 13), // "ReloadCurrent"
QT_MOC_LITERAL(5, 40, 14), // "OpenRecentFile"
QT_MOC_LITERAL(6, 55, 13), // "ChangeChannel"
QT_MOC_LITERAL(7, 69, 7), // "channel"
QT_MOC_LITERAL(8, 77, 10), // "ChangeFace"
QT_MOC_LITERAL(9, 88, 4), // "face"
QT_MOC_LITERAL(10, 93, 9), // "ChangeMip"
QT_MOC_LITERAL(11, 103, 3), // "mip"
QT_MOC_LITERAL(12, 107, 12), // "ChangeVisMin"
QT_MOC_LITERAL(13, 120, 6), // "visMin"
QT_MOC_LITERAL(14, 127, 12), // "ChangeVisMax"
QT_MOC_LITERAL(15, 140, 6), // "visMax"
QT_MOC_LITERAL(16, 147, 14), // "ChangeVisGamma"
QT_MOC_LITERAL(17, 162, 8), // "visGamma"
QT_MOC_LITERAL(18, 171, 17), // "ChangeVisDiffMult"
QT_MOC_LITERAL(19, 189, 4), // "mult"
QT_MOC_LITERAL(20, 194, 18), // "SubWindowActivated"
QT_MOC_LITERAL(21, 213, 14), // "QMdiSubWindow*"
QT_MOC_LITERAL(22, 228, 6) // "window"

    },
    "CMainWindow\0Open\0\0Compare\0ReloadCurrent\0"
    "OpenRecentFile\0ChangeChannel\0channel\0"
    "ChangeFace\0face\0ChangeMip\0mip\0"
    "ChangeVisMin\0visMin\0ChangeVisMax\0"
    "visMax\0ChangeVisGamma\0visGamma\0"
    "ChangeVisDiffMult\0mult\0SubWindowActivated\0"
    "QMdiSubWindow*\0window"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x08 /* Private */,
       3,    0,   75,    2, 0x08 /* Private */,
       4,    0,   76,    2, 0x08 /* Private */,
       5,    0,   77,    2, 0x08 /* Private */,
       6,    1,   78,    2, 0x08 /* Private */,
       8,    1,   81,    2, 0x08 /* Private */,
      10,    1,   84,    2, 0x08 /* Private */,
      12,    1,   87,    2, 0x08 /* Private */,
      14,    1,   90,    2, 0x08 /* Private */,
      16,    1,   93,    2, 0x08 /* Private */,
      18,    1,   96,    2, 0x08 /* Private */,
      20,    1,   99,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Float,   13,
    QMetaType::Void, QMetaType::Float,   15,
    QMetaType::Void, QMetaType::Float,   17,
    QMetaType::Void, QMetaType::Float,   19,
    QMetaType::Void, 0x80000000 | 21,   22,

       0        // eod
};

void CMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CMainWindow *_t = static_cast<CMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Open(); break;
        case 1: _t->Compare(); break;
        case 2: _t->ReloadCurrent(); break;
        case 3: _t->OpenRecentFile(); break;
        case 4: _t->ChangeChannel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->ChangeFace((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->ChangeMip((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->ChangeVisMin((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 8: _t->ChangeVisMax((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 9: _t->ChangeVisGamma((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 10: _t->ChangeVisDiffMult((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 11: _t->SubWindowActivated((*reinterpret_cast< QMdiSubWindow*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CMainWindow.data,
      qt_meta_data_CMainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CMainWindow.stringdata0))
        return static_cast<void*>(const_cast< CMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
