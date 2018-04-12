/****************************************************************************
** Meta object code from reading C++ file 'WebView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "WebView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WebView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_WebView_t {
    QByteArrayData data[17];
    char stringdata[148];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WebView_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WebView_t qt_meta_stringdata_WebView = {
    {
QT_MOC_LITERAL(0, 0, 7), // "WebView"
QT_MOC_LITERAL(1, 8, 12), // "ProcessEvent"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 7), // "QEvent*"
QT_MOC_LITERAL(4, 30, 2), // "ev"
QT_MOC_LITERAL(5, 33, 6), // "Resize"
QT_MOC_LITERAL(6, 40, 1), // "w"
QT_MOC_LITERAL(7, 42, 1), // "h"
QT_MOC_LITERAL(8, 44, 12), // "new_framebuf"
QT_MOC_LITERAL(9, 57, 7), // "LoadUrl"
QT_MOC_LITERAL(10, 65, 3), // "url"
QT_MOC_LITERAL(11, 69, 12), // "PageBackward"
QT_MOC_LITERAL(12, 82, 11), // "PageForward"
QT_MOC_LITERAL(13, 94, 10), // "PageReload"
QT_MOC_LITERAL(14, 105, 16), // "ChangeZoomFactor"
QT_MOC_LITERAL(15, 122, 3), // "fac"
QT_MOC_LITERAL(16, 126, 21) // "ProcessDelayedRegions"

    },
    "WebView\0ProcessEvent\0\0QEvent*\0ev\0"
    "Resize\0w\0h\0new_framebuf\0LoadUrl\0url\0"
    "PageBackward\0PageForward\0PageReload\0"
    "ChangeZoomFactor\0fac\0ProcessDelayedRegions"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WebView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x02 /* Public */,
       5,    3,   57,    2, 0x02 /* Public */,
       9,    1,   64,    2, 0x02 /* Public */,
      11,    0,   67,    2, 0x02 /* Public */,
      12,    0,   68,    2, 0x02 /* Public */,
      13,    0,   69,    2, 0x02 /* Public */,
      14,    1,   70,    2, 0x02 /* Public */,
      16,    0,   73,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Bool, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::VoidStar,    6,    7,    8,
    QMetaType::Void, QMetaType::QUrl,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,   15,
    QMetaType::Void,

       0        // eod
};

void WebView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WebView *_t = static_cast<WebView *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->ProcessEvent((*reinterpret_cast< QEvent*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: _t->Resize((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< void*(*)>(_a[3]))); break;
        case 2: _t->LoadUrl((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 3: _t->PageBackward(); break;
        case 4: _t->PageForward(); break;
        case 5: _t->PageReload(); break;
        case 6: _t->ChangeZoomFactor((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 7: _t->ProcessDelayedRegions(); break;
        default: ;
        }
    }
}

const QMetaObject WebView::staticMetaObject = {
    { &QWebView::staticMetaObject, qt_meta_stringdata_WebView.data,
      qt_meta_data_WebView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *WebView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WebView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_WebView.stringdata))
        return static_cast<void*>(const_cast< WebView*>(this));
    return QWebView::qt_metacast(_clname);
}

int WebView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
