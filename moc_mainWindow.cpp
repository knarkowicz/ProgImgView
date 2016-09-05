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
    QByteArrayData data[20];
    char stringdata0[204];
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
QT_MOC_LITERAL(3, 18, 13), // "ReloadCurrent"
QT_MOC_LITERAL(4, 32, 14), // "OpenRecentFile"
QT_MOC_LITERAL(5, 47, 13), // "ChangeChannel"
QT_MOC_LITERAL(6, 61, 7), // "channel"
QT_MOC_LITERAL(7, 69, 10), // "ChangeFace"
QT_MOC_LITERAL(8, 80, 4), // "face"
QT_MOC_LITERAL(9, 85, 9), // "ChangeMip"
QT_MOC_LITERAL(10, 95, 3), // "mip"
QT_MOC_LITERAL(11, 99, 12), // "ChangeVisMin"
QT_MOC_LITERAL(12, 112, 6), // "visMin"
QT_MOC_LITERAL(13, 119, 12), // "ChangeVisMax"
QT_MOC_LITERAL(14, 132, 6), // "visMax"
QT_MOC_LITERAL(15, 139, 14), // "ChangeVisGamma"
QT_MOC_LITERAL(16, 154, 8), // "visGamma"
QT_MOC_LITERAL(17, 163, 18), // "SubWindowActivated"
QT_MOC_LITERAL(18, 182, 14), // "QMdiSubWindow*"
QT_MOC_LITERAL(19, 197, 6) // "window"

    },
    "CMainWindow\0Open\0\0ReloadCurrent\0"
    "OpenRecentFile\0ChangeChannel\0channel\0"
    "ChangeFace\0face\0ChangeMip\0mip\0"
    "ChangeVisMin\0visMin\0ChangeVisMax\0"
    "visMax\0ChangeVisGamma\0visGamma\0"
    "SubWindowActivated\0QMdiSubWindow*\0"
    "window"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    1,   67,    2, 0x08 /* Private */,
       7,    1,   70,    2, 0x08 /* Private */,
       9,    1,   73,    2, 0x08 /* Private */,
      11,    1,   76,    2, 0x08 /* Private */,
      13,    1,   79,    2, 0x08 /* Private */,
      15,    1,   82,    2, 0x08 /* Private */,
      17,    1,   85,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Float,   12,
    QMetaType::Void, QMetaType::Float,   14,
    QMetaType::Void, QMetaType::Float,   16,
    QMetaType::Void, 0x80000000 | 18,   19,

       0        // eod
};

void CMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CMainWindow *_t = static_cast<CMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Open(); break;
        case 1: _t->ReloadCurrent(); break;
        case 2: _t->OpenRecentFile(); break;
        case 3: _t->ChangeChannel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->ChangeFace((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->ChangeMip((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->ChangeVisMin((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 7: _t->ChangeVisMax((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 8: _t->ChangeVisGamma((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 9: _t->SubWindowActivated((*reinterpret_cast< QMdiSubWindow*(*)>(_a[1]))); break;
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
QT_END_MOC_NAMESPACE
