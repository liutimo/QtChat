/****************************************************************************
** Meta object code from reading C++ file 'chatinput.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "BasicControls/chatinput.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatinput.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ChatInput_t {
    QByteArrayData data[13];
    char stringdata0[109];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChatInput_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChatInput_t qt_meta_stringdata_ChatInput = {
    {
QT_MOC_LITERAL(0, 0, 9), // "ChatInput"
QT_MOC_LITERAL(1, 10, 7), // "sendMsg"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 16), // "btn_send_clicked"
QT_MOC_LITERAL(4, 36, 9), // "showIcons"
QT_MOC_LITERAL(5, 46, 7), // "addIcon"
QT_MOC_LITERAL(6, 54, 4), // "path"
QT_MOC_LITERAL(7, 59, 4), // "hide"
QT_MOC_LITERAL(8, 64, 10), // "fontChange"
QT_MOC_LITERAL(9, 75, 5), // "index"
QT_MOC_LITERAL(10, 81, 11), // "colorChange"
QT_MOC_LITERAL(11, 93, 10), // "sizeChange"
QT_MOC_LITERAL(12, 104, 4) // "test"

    },
    "ChatInput\0sendMsg\0\0btn_send_clicked\0"
    "showIcons\0addIcon\0path\0hide\0fontChange\0"
    "index\0colorChange\0sizeChange\0test"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChatInput[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   62,    2, 0x0a /* Public */,
       4,    0,   63,    2, 0x08 /* Private */,
       5,    1,   64,    2, 0x08 /* Private */,
       7,    0,   67,    2, 0x08 /* Private */,
       8,    1,   68,    2, 0x08 /* Private */,
      10,    1,   71,    2, 0x08 /* Private */,
      11,    1,   74,    2, 0x08 /* Private */,
      12,    0,   77,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,

       0        // eod
};

void ChatInput::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ChatInput *_t = static_cast<ChatInput *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendMsg((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->btn_send_clicked(); break;
        case 2: _t->showIcons(); break;
        case 3: _t->addIcon((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->hide(); break;
        case 5: _t->fontChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->colorChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->sizeChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->test(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ChatInput::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ChatInput::sendMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ChatInput::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ChatInput.data,
      qt_meta_data_ChatInput,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ChatInput::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatInput::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ChatInput.stringdata0))
        return static_cast<void*>(const_cast< ChatInput*>(this));
    return QWidget::qt_metacast(_clname);
}

int ChatInput::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ChatInput::sendMsg(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
