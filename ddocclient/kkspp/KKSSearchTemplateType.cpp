/* 
 * File:   KKSSearchTemplateType.cpp
 * Author: yrusinov
 * 
 * Created on 28 ������� 2012 �., 15:01
 */

#include "KKSSearchTemplateType.h"

KKSSearchTemplateType::KKSSearchTemplateType(int id, const QString & name, const QString & desc) 
: KKSRecord (id, name, desc), m_parent(NULL)
{

}

KKSSearchTemplateType :: KKSSearchTemplateType (const KKSSearchTemplateType & stt)
: KKSRecord (stt), m_parent(stt.m_parent)
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

