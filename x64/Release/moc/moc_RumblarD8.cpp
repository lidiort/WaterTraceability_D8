/****************************************************************************
** Meta object code from reading C++ file 'RumblarD8.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../RumblarD8.h"
#include <QtGui/qtextcursor.h>
#include <QScreen>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RumblarD8.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_RumblarD8_t {
    uint offsetsAndSizes[24];
    char stringdata0[10];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[14];
    char stringdata4[15];
    char stringdata5[15];
    char stringdata6[15];
    char stringdata7[15];
    char stringdata8[11];
    char stringdata9[8];
    char stringdata10[6];
    char stringdata11[2];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_RumblarD8_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_RumblarD8_t qt_meta_stringdata_RumblarD8 = {
    {
        QT_MOC_LITERAL(0, 9),  // "RumblarD8"
        QT_MOC_LITERAL(10, 12),  // "handleButton"
        QT_MOC_LITERAL(23, 0),  // ""
        QT_MOC_LITERAL(24, 13),  // "buscarFichero"
        QT_MOC_LITERAL(38, 14),  // "buscarFichero2"
        QT_MOC_LITERAL(53, 14),  // "buscarFichero3"
        QT_MOC_LITERAL(68, 14),  // "buscarFichero4"
        QT_MOC_LITERAL(83, 14),  // "buscarFichero7"
        QT_MOC_LITERAL(98, 10),  // "SizeCHange"
        QT_MOC_LITERAL(109, 7),  // "checked"
        QT_MOC_LITERAL(117, 5),  // "Grid*"
        QT_MOC_LITERAL(123, 1)   // "g"
    },
    "RumblarD8",
    "handleButton",
    "",
    "buscarFichero",
    "buscarFichero2",
    "buscarFichero3",
    "buscarFichero4",
    "buscarFichero7",
    "SizeCHange",
    "checked",
    "Grid*",
    "g"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_RumblarD8[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   56,    2, 0x08,    1 /* Private */,
       3,    0,   57,    2, 0x08,    2 /* Private */,
       4,    0,   58,    2, 0x08,    3 /* Private */,
       5,    0,   59,    2, 0x08,    4 /* Private */,
       6,    0,   60,    2, 0x08,    5 /* Private */,
       7,    0,   61,    2, 0x08,    6 /* Private */,
       8,    2,   62,    2, 0x08,    7 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 10,    9,   11,

       0        // eod
};

Q_CONSTINIT const QMetaObject RumblarD8::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_RumblarD8.offsetsAndSizes,
    qt_meta_data_RumblarD8,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_RumblarD8_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<RumblarD8, std::true_type>,
        // method 'handleButton'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'buscarFichero'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'buscarFichero2'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'buscarFichero3'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'buscarFichero4'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'buscarFichero7'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SizeCHange'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<Grid *, std::false_type>
    >,
    nullptr
} };

void RumblarD8::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RumblarD8 *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->handleButton(); break;
        case 1: _t->buscarFichero(); break;
        case 2: _t->buscarFichero2(); break;
        case 3: _t->buscarFichero3(); break;
        case 4: _t->buscarFichero4(); break;
        case 5: _t->buscarFichero7(); break;
        case 6: _t->SizeCHange((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Grid*>>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject *RumblarD8::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RumblarD8::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RumblarD8.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int RumblarD8::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
