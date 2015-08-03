/* 
 * File:   KKSSearchTemplateType.cpp
 * Author: yrusinov
 * 
 * Created on 28 ������� 2012 �., 15:01
 */

#include "KKSSearchTemplate.h"
#include "KKSSearchTemplateType.h"

KKSSearchTemplateType::KKSSearchTemplateType(int id, const QString & name, const QString & desc) 
: KKSRecord (id, name, desc), m_parent(NULL)
{

}

KKSSearchTemplateType :: KKSSearchTemplateType (const KKSSearchTemplateType & stt)
: KKSRecord (stt), 
  m_parent(stt.m_parent),
  childTypes (stt.childTypes),
  childTemplates (stt.childTemplates)
{
    if(m_parent)
        m_parent->addRef();
}

KKSSearchTemplateType::~KKSSearchTemplateType (void)
{
    if(m_parent)
        m_parent->release();
}


const KKSSearchTemplateType * KKSSearchTemplateType::parent() const
{
    return m_parent;
}

KKSSearchTemplateType * KKSSearchTemplateType::parent()
{
    return m_parent;
}

void KKSSearchTemplateType::setParent(KKSSearchTemplateType * p)
{
    if(m_parent)
        m_parent->release();

    m_parent = p;

    if(m_parent)
        m_parent->addRef();
}

KKSSearchTemplateType * KKSSearchTemplateType::defaultType() 
{
    KKSSearchTemplateType * t = new KKSSearchTemplateType(1, QObject::tr("¿¿¿¿¿ ¿¿¿¿¿¿¿¿¿ ¿¿¿¿¿¿¿")); //¿¿¿¿¿¿¿ ¿ main.cpp QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    return t;
}

void KKSSearchTemplateType::clearSubTypes (void)
{
    childTypes.clear ();
}

void KKSSearchTemplateType::addSubType (KKSSearchTemplateType* childType)
{
    if (!childType)
        return;
    childTypes.insert (childType->id(), childType);
}

const KKSMap<qint64, KKSSearchTemplateType *>& KKSSearchTemplateType::getSubTypes (void) const
{
    return childTypes;
}

KKSMap<qint64, KKSSearchTemplateType *>& KKSSearchTemplateType::getSubTypes (void)
{
    return childTypes;
}

void KKSSearchTemplateType::setSubTypes (const KKSMap<qint64, KKSSearchTemplateType *>& sTypes)
{
    childTypes = sTypes;
}

void KKSSearchTemplateType::clearTemplates (void)
{
    childTemplates.clear ();
}

void KKSSearchTemplateType::addTemplate (KKSSearchTemplate * childTempl)
{
    if (!childTempl)
        return;
    childTemplates.insert (childTempl->id(), childTempl);
}

const KKSMap<qint64, KKSSearchTemplate *>& KKSSearchTemplateType::getTemplates (void) const
{
    return childTemplates;
}

KKSMap<qint64, KKSSearchTemplate *>& KKSSearchTemplateType::getTemplates (void)
{
    return childTemplates;
}

void KKSSearchTemplateType::setChildTemplates (const KKSMap<qint64, KKSSearchTemplate *>& sTemplates)
{
    childTemplates = sTemplates;
}
