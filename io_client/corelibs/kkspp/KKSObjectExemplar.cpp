/***********************************************************************
 * Module:  KKSObjectExemplar.cpp
 * Author:  sergey
 * Modified: 1 ������� 2008 �. 12:12:00
 * Purpose: Implementation of the class KKSObjectExemplar
 * Comment: ���
 ***********************************************************************/

#include "KKSObject.h"
#include "KKSAttrValue.h"
#include "KKSObjectExemplar.h"
#include "KKSValue.h"
#include "KKSAttrType.h"
#include "KKSAttribute.h"
#include "KKSRubric.h"
#include "defines.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSObjectExemplar::KKSObjectExemplar()
// Purpose:    Implementation of KKSObjectExemplar::KKSObjectExemplar()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSObjectExemplar::KKSObjectExemplar() : KKSRecord(),
        m_rootRubric (0)
{
   m_io = NULL;
   m_filesModified = false;
}

KKSObjectExemplar::KKSObjectExemplar(const KKSObjectExemplar & eio) : KKSRecord(eio),
        m_rootRubric (eio.m_rootRubric)
{
    m_io = NULL;
    if(eio.io()){
        setIo(eio.io());
    }
    if (m_rootRubric)
        m_rootRubric->addRef ();

    m_attrValues = eio.attrValues();
    m_indValues = eio.indValues();
    m_files = eio.files();
	m_filesModified = eio.m_filesModified;

}

KKSObjectExemplar::KKSObjectExemplar(qint64 id, const QString & name, KKSObject * io) : KKSRecord(id, name),
        m_rootRubric (0)
{
    m_io = NULL;
    setIo(io);
    m_filesModified = false;
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
    if (m_rootRubric)
        m_rootRubric->release ();

}

KKSObject * KKSObjectExemplar::io() const
{
    return m_io;
}

void KKSObjectExemplar::setIo(KKSObject * _io)
{
    if(m_io)
        m_io->release();
    
    m_io = _io;
    if(m_io)
        m_io->addRef();

}


void KKSObjectExemplar::setName(const QString & newName)
{
    KKSRecord::setName(newName);
    KKSAttrValue * av = attrValue(ATTR_NAME);
    if(!av)
        qWarning("Parameter 'name' does not present in object exemplar attributes!");
    else
    {
        KKSValue v;
        v.setValue(newName, KKSAttrType::atString);
        av->setValue(v);
    }
}

void KKSObjectExemplar::setId(qint64 newId)
{
    KKSRecord::setId(newId);

    KKSAttrValue * av = attrValue(ATTR_ID);
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
    KKSAttrValue * av = attrValue(ATTR_DESCRIPTION);
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
    KKSAttrValue * av = attrValue(ATTR_CODE);
    if(!av)
        qWarning("Parameter 'code' does not present in object exemplar attributes!");
    else{
        KKSValue v;
        v.setValue(newCode, KKSAttrType::atString);
        av->setValue(v);
    }
}

//------------------------------------------------------------------------
////������ ��� ������ � ���������� ������� ������������ (��������� ������)
//------------------------------------------------------------------------
const KKSList<KKSAttrValue *> & KKSObjectExemplar::attrValues() const
{
    return m_attrValues;
}

KKSList<KKSAttrValue *> & KKSObjectExemplar::attrValues()
{
    return m_attrValues;
}

void KKSObjectExemplar::setAttrValues(const KKSList<KKSAttrValue*> & _attrValues)
{
    m_attrValues = _attrValues;
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

//�������� �������� �������� �� ���� �������� (�� KKSCategoryAttr::code())
KKSAttrValue * KKSObjectExemplar::attrValue(const QString & code)
{
    KKSAttrValue * av = NULL;
    
    for (int i=0; i<m_attrValues.count(); i++)
    {
        KKSAttrValue * av1 = m_attrValues [i];
        if(av1->attribute()->code() == code){
            av = av1;
            break;            
        }
    }

    return av;
}

const KKSAttrValue * KKSObjectExemplar::attrValue(const QString & code) const
{
    const KKSAttrValue * av = NULL;
    
    for (int i=0; i<m_attrValues.count(); i++)
    {
        KKSAttrValue * av1 = m_attrValues [i];
        if(av1->attribute()->code() == code){
            av = av1;
            break;            
        }
    }

    return av;
}

/*!\brief ��������� �������� ��������� ������ ����������� � ���������� �� ��������� (������ ��� ������ �� KKSCategoryAttr::defValue()

���� ��� ��������� ��������� ���� ������ ��������, ��� ���� �������� �� �������� �� ���������
*/
int KKSObjectExemplar::createDefaultAttrValues()
{
    if(!this->io())
        return ERROR_CODE;

    KKSCategory * c = io()->category();
    if(!c)
        return ERROR_CODE;

    c = c->tableCategory();
    if(!c)
        return ERROR_CODE;

    if(m_attrValues.count() > 0)
        m_attrValues.clear();

    KKSMap<int, KKSCategoryAttr *> attrs = c->attributes();
    KKSMap<int, KKSCategoryAttr *>::const_iterator pa;

    for (pa=attrs.constBegin(); pa != attrs.constEnd(); pa++)
    {
        KKSCategoryAttr * attr = pa.value();
        QString code;
        
        KKSAttrValue * av = new KKSAttrValue();
        av->setAttribute(attr);
        KKSValue v = attr->defValue();
        av->setValue(v);

        m_attrValues.append(av);
    }


    return OK_CODE;
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


//------------------------------------------------------------------------
////������ ��� ������ � ������������ ������� ������������ 
//------------------------------------------------------------------------
const KKSList<KKSAttrValue *> & KKSObjectExemplar::indValues() const
{
    return m_indValues;
}

KKSList<KKSAttrValue *> & KKSObjectExemplar::indValues()
{
    return m_indValues;
}

void KKSObjectExemplar::setIndValues(const KKSList<KKSAttrValue*> & _indValues)
{
    m_indValues = _indValues;
}


KKSAttrValue * KKSObjectExemplar::indValue(int id)
{
    KKSAttrValue * av = NULL;
    for (int i=0; i<m_indValues.count(); i++)
    {
        KKSAttrValue * av1 = m_indValues [i];
        if(av1->attribute()->id() == id){
            av = av1;
            break;            
        }
    }

    return av;
}

const KKSAttrValue * KKSObjectExemplar::indValue(int id) const
{
    const KKSAttrValue * av = NULL;
    
    for (int i=0; i<m_indValues.count(); i++)
    {
        KKSAttrValue * av1 = m_indValues [i];
        if(av1->attribute()->id() == id){
            av = av1;
            break;            
        }
    }

    return av;
}

void KKSObjectExemplar::addIndValue(const QString & value, KKSCategoryAttr * attr, bool * bBadValue)
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
    addIndValue(av);
    av->release();

    if(bBadValue)
        *bBadValue = badValue;
}

void KKSObjectExemplar::addIndValue(KKSAttrValue * av)
{
    if(!av)
        return;

    m_indValues.append(av);
}

void KKSObjectExemplar::removeIndValue(KKSAttrValue * av)
{
    if(!av)
        return;
    
    m_indValues.removeAll(av);
}

void KKSObjectExemplar::removeIndValue(int index)
{
    m_indValues.removeAt(index);
}

KKSAttrValue * KKSObjectExemplar::indValueIndex(int index)
{
    KKSAttrValue * av = NULL;
    if(index >= m_indValues.count())
        return av;

    av = m_indValues[index];
    return av;
}

const KKSAttrValue * KKSObjectExemplar::indValueIndex(int index) const
{
    const KKSAttrValue * av = NULL;
    if(index >= m_indValues.count())
        return av;

    av = m_indValues.at(index);
    return av;
}

void KKSObjectExemplar::setRootRubric(KKSRubric * r)
{
    if(m_rootRubric)
        m_rootRubric->release();

    m_rootRubric = r;

    if(m_rootRubric)
        m_rootRubric->addRef();
}

KKSRubric * KKSObjectExemplar::rootRubric() const
{
    return m_rootRubric;
}


const KKSList<KKSFile *> & KKSObjectExemplar::files() const
{
    return m_files;
}

KKSList<KKSFile *> & KKSObjectExemplar::files()
{
    return m_files;
}

void KKSObjectExemplar::setFiles(const KKSList<KKSFile *> & _files)
{
    m_files = _files;
    m_filesModified = true;
}

void KKSObjectExemplar::addFile(KKSFile * f)
{
    if(!f)
        return;

    m_filesModified = true;
    m_files.append(f);
}

//������� �� ������ ������ ��������� ����
int KKSObjectExemplar::removeFile(KKSFile * f)
{
    if(!f)
        return ERROR_CODE;
    
    int cnt = m_files.removeAll(f);
    
    if(cnt == 0)
        return ERROR_CODE;

    m_filesModified = true;
    
    return OK_CODE;
}

//������� �� ������ ������ ���� � ��������� ���������� ��������
int KKSObjectExemplar::removeFile(int index)
{
    int ok = m_files.removeAt(index);
    if(ok == 0)
        return ERROR_CODE;

    m_filesModified = true;
    return OK_CODE;
}

KKSFile * KKSObjectExemplar::file(int index)
{
    KKSFile * f = NULL;
    if(index >= m_files.count())
        return f;

    f = m_files[index];
    return f;
}

const KKSFile * KKSObjectExemplar::file(int index) const
{
    const KKSFile * f = NULL;
    if(index >= m_files.count())
        return f;

    f = m_files.at(index);

    return f;
}

