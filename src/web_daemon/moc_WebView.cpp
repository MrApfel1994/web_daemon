/****************************************************************************
** Meta object code from reading C++ file 'WebView.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "WebView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WebView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WebView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: signature, parameters, type, tag, flags
      17,   14,    9,    8, 0x02,
      56,   39,    8,    8, 0x02,
      82,   78,    8,    8, 0x02,
      96,    8,    8,    8, 0x02,
     111,    8,    8,    8, 0x02,
     125,    8,    8,    8, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_WebView[] = {
    "WebView\0\0bool\0ev\0ProcessEvent(QEvent*)\0"
    "w,h,new_framebuf\0Resize(int,int,void*)\0"
    "url\0LoadUrl(QUrl)\0PageBackward()\0"
    "PageForward()\0PageReload()\0"
};

void WebView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WebView *_t = static_cast<WebView *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->ProcessEvent((*reinterpret_cast< QEvent*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: _t->Resize((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< void*(*)>(_a[3]))); break;
        case 2: _t->LoadUrl((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 3: _t->PageBackward(); break;
        case 4: _t->PageForward(); break;
        case 5: _t->PageReload(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData WebView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WebView::staticMetaObject = {
    { &QWebView::staticMetaObject, qt_meta_stringdata_WebView,
      qt_meta_data_WebView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WebView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WebView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WebView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WebView))
        return static_cast<void*>(const_cast< WebView*>(this));
    return QWebView::qt_metacast(_clname);
}

int WebView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
