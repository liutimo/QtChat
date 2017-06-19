/****************************************************************************
** Meta object code from reading C++ file 'connecttoserver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "NetWork/connecttoserver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'connecttoserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ConnectToServer_t {
    QByteArrayData data[12];
    char stringdata0[160];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConnectToServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConnectToServer_t qt_meta_stringdata_ConnectToServer = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ConnectToServer"
QT_MOC_LITERAL(1, 16, 11), // "loginStatus"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 11), // "LoginStatus"
QT_MOC_LITERAL(4, 41, 1), // "s"
QT_MOC_LITERAL(5, 43, 17), // "responseHeartBeat"
QT_MOC_LITERAL(6, 61, 18), // "responseFriendList"
QT_MOC_LITERAL(7, 80, 15), // "receivedMessage"
QT_MOC_LITERAL(8, 96, 19), // "ReceivedMessageMsg*"
QT_MOC_LITERAL(9, 116, 3), // "msg"
QT_MOC_LITERAL(10, 120, 16), // "responseUserInfo"
QT_MOC_LITERAL(11, 137, 22) // "receivedOfflineMessage"

    },
    "ConnectToServer\0loginStatus\0\0LoginStatus\0"
    "s\0responseHeartBeat\0responseFriendList\0"
    "receivedMessage\0ReceivedMessageMsg*\0"
    "msg\0responseUserInfo\0receivedOfflineMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConnectToServer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       5,    0,   47,    2, 0x06 /* Public */,
       6,    1,   48,    2, 0x06 /* Public */,
       7,    1,   51,    2, 0x06 /* Public */,
      10,    1,   54,    2, 0x06 /* Public */,
      11,    1,   57,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,

       0        // eod
};

void ConnectToServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConnectToServer *_t = static_cast<ConnectToServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->loginStatus((*reinterpret_cast< LoginStatus(*)>(_a[1]))); break;
        case 1: _t->responseHeartBeat(); break;
        case 2: _t->responseFriendList((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 3: _t->receivedMessage((*reinterpret_cast< ReceivedMessageMsg*(*)>(_a[1]))); break;
        case 4: _t->responseUserInfo((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 5: _t->receivedOfflineMessage((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ConnectToServer::*_t)(LoginStatus );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConnectToServer::loginStatus)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ConnectToServer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConnectToServer::responseHeartBeat)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (ConnectToServer::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConnectToServer::responseFriendList)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (ConnectToServer::*_t)(ReceivedMessageMsg * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConnectToServer::receivedMessage)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (ConnectToServer::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConnectToServer::responseUserInfo)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (ConnectToServer::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConnectToServer::receivedOfflineMessage)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject ConnectToServer::staticMetaObject = {
    { &QTcpSocket::staticMetaObject, qt_meta_stringdata_ConnectToServer.data,
      qt_meta_data_ConnectToServer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ConnectToServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConnectToServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConnectToServer.stringdata0))
        return static_cast<void*>(const_cast< ConnectToServer*>(this));
    return QTcpSocket::qt_metacast(_clname);
}

int ConnectToServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ConnectToServer::loginStatus(LoginStatus _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ConnectToServer::responseHeartBeat()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ConnectToServer::responseFriendList(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ConnectToServer::receivedMessage(ReceivedMessageMsg * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ConnectToServer::responseUserInfo(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ConnectToServer::receivedOfflineMessage(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
