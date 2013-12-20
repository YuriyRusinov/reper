/****************************************************************************
** Meta object code from reading C++ file 'syncqueueviewerform.h'
**
** Created: Fri Dec 20 10:35:37 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../syncqueueviewerform.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'syncqueueviewerform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SyncQueueViewerForm[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      41,   20,   20,   20, 0x08,
      62,   20,   20,   20, 0x08,
      81,   20,   20,   20, 0x08,
      97,   20,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SyncQueueViewerForm[] = {
    "SyncQueueViewerForm\0\0signal_visionData()\0"
    "slot_filters_setup()\0slot_viewClicked()\0"
    "slot_accepted()\0slot_rejected()\0"
};

void SyncQueueViewerForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SyncQueueViewerForm *_t = static_cast<SyncQueueViewerForm *>(_o);
        switch (_id) {
        case 0: _t->signal_visionData(); break;
        case 1: _t->slot_filters_setup(); break;
        case 2: _t->slot_viewClicked(); break;
        case 3: _t->slot_accepted(); break;
        case 4: _t->slot_rejected(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData SyncQueueViewerForm::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SyncQueueViewerForm::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SyncQueueViewerForm,
      qt_meta_data_SyncQueueViewerForm, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SyncQueueViewerForm::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SyncQueueViewerForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SyncQueueViewerForm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SyncQueueViewerForm))
        return static_cast<void*>(const_cast< SyncQueueViewerForm*>(this));
    return QDialog::qt_metacast(_clname);
}

int SyncQueueViewerForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SyncQueueViewerForm::signal_visionData()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
