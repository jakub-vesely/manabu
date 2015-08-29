/****************************************************************************
** Meta object code from reading C++ file 'SerialPort.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../manabu/program/manabu/SerialPort.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SerialPort.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SerialPort_t {
    QByteArrayData data[27];
    char stringdata[278];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialPort_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialPort_t qt_meta_stringdata_SerialPort = {
    {
QT_MOC_LITERAL(0, 0, 10), // "SerialPort"
QT_MOC_LITERAL(1, 11, 8), // "SetValue"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 5), // "value"
QT_MOC_LITERAL(4, 27, 15), // "GetFlashVersion"
QT_MOC_LITERAL(5, 43, 5), // "layer"
QT_MOC_LITERAL(6, 49, 15), // "SetFlashAddress"
QT_MOC_LITERAL(7, 65, 8), // "uint16_t"
QT_MOC_LITERAL(8, 74, 7), // "address"
QT_MOC_LITERAL(9, 82, 17), // "SetFlashWriteWord"
QT_MOC_LITERAL(10, 100, 4), // "word"
QT_MOC_LITERAL(11, 105, 17), // "SetFlashLatchWord"
QT_MOC_LITERAL(12, 123, 16), // "GetFlashCheckSum"
QT_MOC_LITERAL(13, 140, 11), // "SetFlashEnd"
QT_MOC_LITERAL(14, 152, 17), // "SetFlashLoadCheck"
QT_MOC_LITERAL(15, 170, 4), // "byte"
QT_MOC_LITERAL(16, 175, 7), // "SetMode"
QT_MOC_LITERAL(17, 183, 4), // "mode"
QT_MOC_LITERAL(18, 188, 14), // "FillModuleType"
QT_MOC_LITERAL(19, 203, 12), // "ModuleTypes&"
QT_MOC_LITERAL(20, 216, 10), // "moduleType"
QT_MOC_LITERAL(21, 227, 8), // "GetState"
QT_MOC_LITERAL(22, 236, 4), // "int&"
QT_MOC_LITERAL(23, 241, 7), // "GetMode"
QT_MOC_LITERAL(24, 249, 5), // "uint&"
QT_MOC_LITERAL(25, 255, 9), // "readyRead"
QT_MOC_LITERAL(26, 265, 12) // "timerTimeout"

    },
    "SerialPort\0SetValue\0\0value\0GetFlashVersion\0"
    "layer\0SetFlashAddress\0uint16_t\0address\0"
    "SetFlashWriteWord\0word\0SetFlashLatchWord\0"
    "GetFlashCheckSum\0SetFlashEnd\0"
    "SetFlashLoadCheck\0byte\0SetMode\0mode\0"
    "FillModuleType\0ModuleTypes&\0moduleType\0"
    "GetState\0int&\0GetMode\0uint&\0readyRead\0"
    "timerTimeout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialPort[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x0a /* Public */,
       4,    1,   87,    2, 0x0a /* Public */,
       6,    1,   90,    2, 0x0a /* Public */,
       9,    1,   93,    2, 0x0a /* Public */,
      11,    1,   96,    2, 0x0a /* Public */,
      12,    0,   99,    2, 0x0a /* Public */,
      13,    0,  100,    2, 0x0a /* Public */,
      14,    2,  101,    2, 0x0a /* Public */,
      16,    2,  106,    2, 0x0a /* Public */,
      18,    2,  111,    2, 0x0a /* Public */,
      21,    2,  116,    2, 0x0a /* Public */,
      23,    2,  121,    2, 0x0a /* Public */,
      25,    0,  126,    2, 0x09 /* Protected */,
      26,    0,  127,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Bool, QMetaType::Int,    3,
    QMetaType::Int, QMetaType::UInt,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 7,   10,
    QMetaType::Void, 0x80000000 | 7,   10,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt, QMetaType::UChar,    5,   15,
    QMetaType::Bool, QMetaType::UInt, QMetaType::UInt,    5,   17,
    QMetaType::Bool, QMetaType::UInt, 0x80000000 | 19,    5,   20,
    QMetaType::Bool, QMetaType::UInt, 0x80000000 | 22,    5,    3,
    QMetaType::Int, QMetaType::UInt, 0x80000000 | 24,    5,   17,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SerialPort::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SerialPort *_t = static_cast<SerialPort *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->SetValue((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { int _r = _t->GetFlashVersion((*reinterpret_cast< uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: _t->SetFlashAddress((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 3: _t->SetFlashWriteWord((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 4: _t->SetFlashLatchWord((*reinterpret_cast< uint16_t(*)>(_a[1]))); break;
        case 5: { int _r = _t->GetFlashCheckSum();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 6: _t->SetFlashEnd(); break;
        case 7: _t->SetFlashLoadCheck((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< unsigned char(*)>(_a[2]))); break;
        case 8: { bool _r = _t->SetMode((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: { bool _r = _t->FillModuleType((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ModuleTypes(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: { bool _r = _t->GetState((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: { int _r = _t->GetMode((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 12: _t->readyRead(); break;
        case 13: _t->timerTimeout(); break;
        default: ;
        }
    }
}

const QMetaObject SerialPort::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SerialPort.data,
      qt_meta_data_SerialPort,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SerialPort::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialPort::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SerialPort.stringdata))
        return static_cast<void*>(const_cast< SerialPort*>(this));
    return QObject::qt_metacast(_clname);
}

int SerialPort::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
