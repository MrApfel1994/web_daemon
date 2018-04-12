/****************************************************************************
** Meta object code from reading C++ file 'WebApp.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "WebApp.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WebApp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_WebApp_t {
    QByteArrayData data[17];
    char stringdata[156];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WebApp_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WebApp_t qt_meta_stringdata_WebApp = {
    {
QT_MOC_LITERAL(0, 0, 6), // "WebApp"
QT_MOC_LITERAL(1, 7, 13), // "OnLoadStarted"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 14), // "OnLoadProgress"
QT_MOC_LITERAL(4, 37, 8), // "progress"
QT_MOC_LITERAL(5, 46, 14), // "OnLoadFinished"
QT_MOC_LITERAL(6, 61, 6), // "result"
QT_MOC_LITERAL(7, 68, 14), // "OnTitleChanged"
QT_MOC_LITERAL(8, 83, 5), // "title"
QT_MOC_LITERAL(9, 89, 12), // "OnUrlChanged"
QT_MOC_LITERAL(10, 102, 3), // "url"
QT_MOC_LITERAL(11, 106, 11), // "OnJsConsole"
QT_MOC_LITERAL(12, 118, 2), // "id"
QT_MOC_LITERAL(13, 121, 4), // "line"
QT_MOC_LITERAL(14, 126, 3), // "msg"
QT_MOC_LITERAL(15, 130, 18), // "OnZoomFactorChange"
QT_MOC_LITERAL(16, 149, 6) // "factor"

    },
    "WebApp\0OnLoadStarted\0\0OnLoadProgress\0"
    "progress\0OnLoadFinished\0result\0"
    "OnTitleChanged\0title\0OnUrlChanged\0url\0"
    "OnJsConsole\0id\0line\0msg\0OnZoomFactorChange\0"
    "factor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WebApp[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    1,   50,    2, 0x08 /* Private */,
       5,    1,   53,    2, 0x08 /* Private */,
       7,    1,   56,    2, 0x08 /* Private */,
       9,    1,   59,    2, 0x08 /* Private */,
      11,    3,   62,    2, 0x08 /* Private */,
      15,    1,   69,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QUrl,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString,   12,   13,   14,
    QMetaType::Void, QMetaType::Float,   16,

       0        // eod
};

void WebApp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WebApp *_t = static_cast<WebApp *>(_o);
        switch (_id) {
        case 0: _t->OnLoadStarted(); break;
        case 1: _t->OnLoadProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->OnLoadFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->OnTitleChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->OnUrlChanged((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 5: _t->OnJsConsole((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 6: _t->OnZoomFactorChange((*reinterpret_cast< float(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject WebApp::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_WebApp.data,
      qt_meta_data_WebApp,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *WebApp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WebApp::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_WebApp.stringdata))
        return static_cast<void*>(const_cast< WebApp*>(this));
    return QObject::qt_metacast(_clname);
}

int WebApp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
