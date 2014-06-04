/***********************************************************************
 * Module:  KKSAttrValue.cpp
 * Author:  sergey
 * Modified: 27 но€бр€ 2008 г. 14:01:33
 * Purpose: Implementation of the class KKSAttrValue
 ***********************************************************************/

#include "KKSValue.h"
#include "KKSAttribute.h"
#include "KKSAttrValue.h"
#include "KKSObject.h"

qint64 m_defId = -2;

void KKSAttrValue::initDefId(){ m_defId = -2;}
qint64  KKSAttrValue::getDefId(){ return m_defId;}
void KKSAttrValue::decDefId() {m_defId--;}



////////////////////////////////////////////////////////////////////////
// Name:       KKSAttrValue::KKSAttrValue()
// Purpose:    Implementation of KKSAttrValue::KKSAttrValue()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSAttrValue::KKSAttrValue() : KKSData(),
m_ioSrc(0),
m_ioSrc1(0),
m_isActual(true),
m_attrsValuesLoaded(false)
{
   m_attribute = NULL;
   
   m_id = KKSAttrValue::getDefId();
   KKSAttrValue::decDefId();
}

KKSAttrValue::KKSAttrValue(const KKSAttrValue & av) : KKSData(),
m_ioSrc(0),
m_ioSrc1(0),
m_isActual(true),
m_attrsValuesLoaded(false)
{
    m_attribute = NULL;

    setId(av.id());

    //setMeasDateTime(av.measDateTime());
    setInsertDateTime(av.insertDateTime());
    setStartDateTime(av.startDateTime());
    setStopDateTime(av.stopDateTime());

    m_attrsValues = av.attrsValues();

    setIOSrc(const_cast<KKSObject *>(av.ioSrc()));
    setIOSrc1(const_cast<KKSObject *>(av.ioSrc1()));

    m_value = av.value();

    m_desc = av.desc();

    if(av.attribute())
        setAttribute(const_cast<KKSCategoryAttr*>(av.attribute()));

    verify();
}

KKSAttrValue::KKSAttrValue(const KKSValue & v, KKSCategoryAttr * a) : KKSData(),
m_ioSrc(0),
m_ioSrc1(0),
m_isActual(true),
m_attrsValuesLoaded(false)
{
    m_id = KKSAttrValue::getDefId();
    KKSAttrValue::decDefId();
    
    m_attribute = NULL;
    setValue(v);
    setAttribute(a);
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttrValue::~KKSAttrValue()
// Purpose:    Implementation of KKSAttrValue::~KKSAttrValue()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSAttrValue::~KKSAttrValue()
{
    if(m_attribute)
        m_attribute->release();

    if(m_ioSrc)
        m_ioSrc->release();

    if(m_ioSrc1)
        m_ioSrc1->release();

}

const KKSValue & KKSAttrValue::value() const
{
    if(!attribute())
        return m_value;

    if(attribute()->type()->attrType() == KKSAttrType::atComplex){
        KKSMap<qint64, KKSAttrValue*> aaList = m_attrsValues;
        KKSMap<qint64, KKSAttrValue*>::const_iterator  i;
        QString V;
        for(i=aaList.constBegin(); i != aaList.constEnd(); i++){
            KKSAttrValue * av = i.value();
            if(!av || !av->attribute())
                continue;
            
            int idAttrAttr = av->attribute()->idRow();//фактически это attr_attr
            if(av->value().isNull() || !av->value().isValid())
                continue;

            QString v_str = av->value().valueForInsert();
            //if(v_str.startsWith("'"))
            //    v_str.remove(0, 1);
            //if(v_str.endsWith("'"))
            //    v_str.remove(v_str.length()-1, 1);
            
            QString v = QString("%1~~~%2").arg(QString::number(idAttrAttr)).arg(v_str);
            if(!V.isEmpty())
                V += QString("^~^~^");
            V += v;
        }

        KKSValue vv = KKSValue(V, KKSAttrType::atComplex);
        
        m_value = vv;
    }

    return m_value;
}

KKSValue & KKSAttrValue::value()
{
    return m_value;
}

void KKSAttrValue::setValue(const KKSValue & value)
{
    m_value = value;
}

const QString & KKSAttrValue::desc() const
{
    return m_desc;
}

QString & KKSAttrValue::desc()
{
    return m_desc;
}

void KKSAttrValue::setDesc(const QString & desc)
{
    m_desc = desc;
}

const KKSCategoryAttr * KKSAttrValue::attribute() const
{
    return m_attribute;
}

KKSCategoryAttr * KKSAttrValue::attribute()
{
    return m_attribute;
}

void KKSAttrValue::setAttribute(KKSCategoryAttr * attr)
{
    if(m_attribute)
        m_attribute->release();

    m_attribute = attr;
    if(m_attribute)
        m_attribute->addRef();
}

qint64 KKSAttrValue::id() const
{
    return m_id;
}


void KKSAttrValue::setId(qint64 id)
{
    m_id = id;
}

KKSObject * KKSAttrValue::ioSrc()
{
    return m_ioSrc;
}

const KKSObject * KKSAttrValue::ioSrc() const
{
    return m_ioSrc;
}

void KKSAttrValue::setIOSrc(KKSObject * io)
{
    if (m_ioSrc)
        m_ioSrc->release ();

    m_ioSrc = io;

    if (m_ioSrc)
        m_ioSrc->addRef ();
}

KKSObject * KKSAttrValue::ioSrc1()
{
    return m_ioSrc1;
}

const KKSObject * KKSAttrValue::ioSrc1() const
{
    return m_ioSrc1;
}

void KKSAttrValue::setIOSrc1(KKSObject * io)
{
    if (m_ioSrc1)
        m_ioSrc1->release ();

    m_ioSrc1 = io;

    if (m_ioSrc1)
        m_ioSrc1->addRef ();
}

/*
const QDateTime & KKSAttrValue::measDateTime() const
{
    return m_measDateTime;
}
*/

/*
void KKSAttrValue::setMeasDateTime(const QDateTime & dt)
{
    m_measDateTime = dt;

    verify();
}
*/

const QDateTime & KKSAttrValue::insertDateTime() const
{
    return m_insertDateTime;
}

void KKSAttrValue::setInsertDateTime(const QDateTime & dt)
{
    m_insertDateTime = dt;

    verify();
}

const QDateTime & KKSAttrValue::startDateTime() const
{
    return m_startDateTime;
}

void KKSAttrValue::setStartDateTime(const QDateTime & dt)
{
    m_startDateTime = dt;

    verify();
}

const QDateTime & KKSAttrValue::stopDateTime() const
{
    return m_stopDateTime;
}

void KKSAttrValue::setStopDateTime(const QDateTime & dt)
{
    m_stopDateTime = dt;
    
    verify();
}

void KKSAttrValue::verify()
{
    m_valid = true;
}

bool KKSAttrValue::isValid() const
{
    return m_valid;
}

bool KKSAttrValue::isActual() const
{
    return m_isActual;
}

void KKSAttrValue::setActual(bool a)
{
    m_isActual = a;
}

void KKSAttrValue::setAttrsValues(const KKSMap<qint64, KKSAttrValue *> & attrsValues)
{
    m_attrsValues = attrsValues;
    m_attrsValuesLoaded = true;
}

const KKSMap<qint64, KKSAttrValue*> & KKSAttrValue::attrsValues() const
{
    return m_attrsValues;
}

KKSMap<qint64, KKSAttrValue*> & KKSAttrValue::attrsValues()
{
    return m_attrsValues;
}

bool KKSAttrValue::attrsValuesLoaded() const
{
    return m_attrsValuesLoaded;
}
