/****************************************************************************
** Meta object code from reading C++ file 'WebApp.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "WebApp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WebApp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WebApp[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x08,
      33,   24,    7,    7, 0x08,
      60,   53,    7,    7, 0x08,
      87,   81,    7,    7, 0x08,
     115,  111,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_WebApp[] = {
    "WebApp\0\0OnLoadStarted()\0progress\0"
    "OnLoadProgress(int)\0result\0"
    "OnLoadFinished(bool)\0title\0"
    "OnTitleChanged(QString)\0url\0"
    "OnUrlChanged(QUrl)\0"
};

void WebApp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WebApp *_t = static_cast<WebApp *>(_o);
        switch (_id) {
        case 0: _t->OnLoadStarted(); break;
        case 1: _t->OnLoadProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->OnLoadFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->OnTitleChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->OnUrlChanged((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData WebApp::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WebApp::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_WebApp,
      qt_meta_data_WebApp, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WebApp::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WebApp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WebApp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WebApp))
        return static_cast<void*>(const_cast< WebApp*>(this));
    return QObject::qt_metacast(_clname);
}

int WebApp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
