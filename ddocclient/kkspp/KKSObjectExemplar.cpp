/***********************************************************************
 * Module:  KKSObjectExemplar.cpp
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:12:00
 * Purpose: Implementation of the class KKSObjectExemplar
 * Comment: ЭИО
 ***********************************************************************/

#include "KKSObject.h"
#include "KKSAttrValue.h"
#include "KKSObjectExemplar.h"
#include "KKSValue.h"
#include "KKSAttrType.h"
#include "KKSCategoryAttr.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSObjectExemplar::KKSObjectExemplar()
// Purpose:    Implementation of KKSObjectExemplar::KKSObjectExemplar()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSObjectExemplar::KKSObjectExemplar() : KKSRecord()
{
   m_io = NULL;
}

KKSObjectExemplar::KKSObjectExemplar(const KKSObjectExemplar & eio) : KKSRecord(eio)
{
    m_io = NULL;
    if(eio.io()){
        setIo(eio.io());
    }

    m_attrValues = eio.attrValues();
}

KKSObjectExemplar::KKSObjectExemplar(int id, const QString & name, KKSObject * io) : KKSRecord(id, name)
{
    m_io = NULL;
    setIo(io);
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSObjectExemplar::~KKSObjectExemplar()
// Purpose:    Implementation of KKSObjectExemplar::~KKSObjectExemplar()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSObjectExemplar::~KKSObjectExemplar()
{
    if(m_io)
        m_io->release();

}

KKSObject * KKSObjectExemplar::io() const
{
    return m_io;
}

const KKSList<KKSAttrValue *> & KKSObjectExemplar::attrValues() const
{
    return m_attrValues;
}

KKSList<KKSAttrValue *> & KKSObjectExemplar::attrValues()
{
    return m_attrValues;
}

void KKSObjectExemplar::setIo(KKSObject * _io)
{
    if(m_io)
        m_io->release();
    
    m_io = _io;
    if(m_io)
        m_io->addRef();

}

void KKSObjectExemplar::setAttrValues(const KKSList<KKSAttrValue*> & _attrValues)
{
    m_attrValues = _attrValues;
}

void KKSObjectExemplar::setName(const QString & newName)
{
    KKSRecord::setName(newName);
    KKSAttrValue * av = attrValue(2);
    if(!av)
        qWarning("Parameter 'name' does not present in object exemplar attributes!");
    else
    {
        KKSValue v;
        v.setValue(newName, KKSAttrType::atString);
        av->setValue(v);
    }
}

void KKSObjectExemplar::setId(int newId)
{
    KKSRecord::setId(newId);

    KKSAttrValue * av = attrValue(1);
    if(!av)
        qWarning("Parameter 'id' does not present in object exemplar attributes!");
    else
    {
        KKSValue v;
        v.setValue(QString::number(newId), KKSAttrType::atInt);
        av->setValue(v);
    }
}

void KKSObjectExemplar::setDesc(const QString & newDesc)
{
    KKSRecord::setDesc(newDesc);
    KKSAttrValue * av = attrValue(3);
    if(!av)
        qWarning("Parameter 'description' does not present in object exemplar attributes!");
    else
    {
        KKSValue v;
        v.setValue(newDesc, KKSAttrType::atString);
        av->setValue(v);
    }
}

void KKSObjectExemplar::setCode(const QString & newCode)
{
    KKSRecord::setCode(newCode);
    KKSAttrValue * av = attrValue(6);
    if(!av)
        qWarning("Parameter 'code' does not present in object exemplar attributes!");
    else{
        KKSValue v;
        v.setValue(newCode, KKSAttrType::atString);
        av->setValue(v);
    }
}

KKSAttrValue * KKSObjectExemplar::attrValue(int id)
{
    KKSAttrValue * av = NULL;
    for (int i=0; i<m_attrValues.count(); i++)
    {
        KKSAttrValue * av1 = m_attrValues [i];
        if(av1->attribute()->id() == id){
            av = av1;
            break;            
        }
    }

    return av;
}

const KKSAttrValue * KKSObjectExemplar::attrValue(int id) const
{
    const KKSAttrValue * av = NULL;
    
    for (int i=0; i<m_attrValues.count(); i++)
    {
        KKSAttrValue * av1 = m_attrValues [i];
        if(av1->attribute()->id() == id){
            av = av1;
            break;            
        }
    }

    return av;
}

void KKSObjectExemplar::addAttrValue(const QString & value, KKSCategoryAttr * attr, bool * bBadValue)
{
    if(!attr){
        if(bBadValue)
            *bBadValue = true;
        return;
    }

    bool badValue = false;
    KKSValue v;
    v.setValue(value, attr->type()->attrType());
    if(!v.isValid())
        badValue = true;
    
    KKSAttrValue * av = new KKSAttrValue(v, attr);
    addAttrValue(av);
    av->release();

    if(bBadValue)
        *bBadValue = badValue;
}

void KKSObjectExemplar::addAttrValue(KKSAttrValue * av)
{
    if(!av)
        return;

    m_attrValues.append(av);
}

void KKSObjectExemplar::removeAttrValue(KKSAttrValue * av)
{
    if(!av)
        return;
    
    m_attrValues.removeAll(av);
}

void KKSObjectExemplar::removeAttrValue(int index)
{
    m_attrValues.removeAt(index);
}

KKSAttrValue * KKSObjectExemplar::attrValueIndex(int index)
{
    KKSAttrValue * av = NULL;
    if(index >= m_attrValues.count())
        return av;

    av = m_attrValues[index];
    return av;
}

const KKSAttrValue * KKSObjectExemplar::attrValueIndex(int index) const
{
    const KKSAttrValue * av = NULL;
    if(index >= m_attrValues.count())
        return av;

    av = m_attrValues.at(index);
    return av;
}
