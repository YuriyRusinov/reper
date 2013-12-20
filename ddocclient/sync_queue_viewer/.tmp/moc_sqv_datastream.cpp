/****************************************************************************
** Meta object code from reading C++ file 'sqv_datastream.h'
**
** Created: Fri Dec 20 10:35:40 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../sqv_datastream.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sqv_datastream.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SQV_DataStream[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   16,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      59,   16,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SQV_DataStream[] = {
    "SQV_DataStream\0\0rhs\0"
    "signal_data(sqv_data::modifyModelData)\0"
    "slot_getData(sqv_data::scrollWindow)\0"
};

void SQV_DataStream::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SQV_DataStream *_t = static_cast<SQV_DataStream *>(_o);
        switch (_id) {
        case 0: _t->signal_data((*reinterpret_cast< sqv_data::modifyModelData(*)>(_a[1]))); break;
        case 1: _t->slot_getData((*reinterpret_cast< sqv_data::scrollWindow(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SQV_DataStream::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SQV_DataStream::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SQV_DataStream,
      qt_meta_data_SQV_DataStream, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SQV_DataStream::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SQV_DataStream::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SQV_DataStream::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SQV_DataStream))
        return static_cast<void*>(const_cast< SQV_DataStream*>(this));
    return QObject::qt_metacast(_clname);
}

int SQV_DataStream::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void SQV_DataStream::signal_data(sqv_data::modifyModelData _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
