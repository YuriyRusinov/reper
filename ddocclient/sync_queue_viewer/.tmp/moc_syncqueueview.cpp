/****************************************************************************
** Meta object code from reading C++ file 'syncqueueview.h'
**
** Created: Fri Dec 20 10:35:38 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../syncqueueview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'syncqueueview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SyncQueueView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   15,   14,   14, 0x05,
      57,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     101,   14,   14,   14, 0x0a,
     123,   14,   14,   14, 0x0a,
     150,  144,   14,   14, 0x0a,
     179,   14,   14,   14, 0x0a,
     195,   15,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SyncQueueView[] = {
    "SyncQueueView\0\0rhs\0"
    "signal_visionScreen(sqv_data::window)\0"
    "signal_scrollScreen(sqv_data::scrollWindow)\0"
    "slot_sliderRealised()\0slot_sliderPressed()\0"
    "value\0slot_sliderValueChanged(int)\0"
    "slot_viewRows()\0slot_setNewScreen(sqv_data::window)\0"
};

void SyncQueueView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SyncQueueView *_t = static_cast<SyncQueueView *>(_o);
        switch (_id) {
        case 0: _t->signal_visionScreen((*reinterpret_cast< sqv_data::window(*)>(_a[1]))); break;
        case 1: _t->signal_scrollScreen((*reinterpret_cast< sqv_data::scrollWindow(*)>(_a[1]))); break;
        case 2: _t->slot_sliderRealised(); break;
        case 3: _t->slot_sliderPressed(); break;
        case 4: _t->slot_sliderValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->slot_viewRows(); break;
        case 6: _t->slot_setNewScreen((*reinterpret_cast< sqv_data::window(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SyncQueueView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SyncQueueView::staticMetaObject = {
    { &QTreeView::staticMetaObject, qt_meta_stringdata_SyncQueueView,
      qt_meta_data_SyncQueueView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SyncQueueView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SyncQueueView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SyncQueueView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SyncQueueView))
        return static_cast<void*>(const_cast< SyncQueueView*>(this));
    return QTreeView::qt_metacast(_clname);
}

int SyncQueueView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void SyncQueueView::signal_visionScreen(sqv_data::window _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SyncQueueView::signal_scrollScreen(sqv_data::scrollWindow _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
