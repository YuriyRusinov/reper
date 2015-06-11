/***********************************************************************
 * Module:  KKSAttrGroup.cpp
 * Author:  sergey
 * Modified: 1 декабр€ 2008 г. 12:09:20
 * Purpose: Implementation of the class KKSAttrGroup
 * Comment: √руппа атрибутов в шаблоне с набором вход€щих в группу атрибутов
 ***********************************************************************/

#include "KKSAttrView.h"
#include "KKSAttrGroup.h"
#include "defines.h"
#include <QtDebug>

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttrGroup::KKSAttrGroup()
// Purpose:    Implementation of KKSAttrGroup::KKSAttrGroup()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSAttrGroup::KKSAttrGroup() : KKSRecord()
{
    m_order = 0;
    m_parent = 0;
    m_isExpanded = false;
}

KKSAttrGroup::KKSAttrGroup(const KKSAttrGroup & g) : KKSRecord(g)
{
    m_order = g.order();
    m_attributes = g.attrViews();
    setParent(const_cast<KKSAttrGroup*>(g.parent()));
    m_isExpanded = false;
}

KKSAttrGroup::KKSAttrGroup(int id, const QString & name, int _order) : KKSRecord(id, name)
{
    m_order = _order;
    m_parent = 0;
    m_isExpanded = false;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttrGroup::~KKSAttrGroup()
// Purpose:    Implementation of KKSAttrGroup::~KKSAttrGroup()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSAttrGroup::~KKSAttrGroup()
{
    if(m_parent)
        m_parent->release();
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSGroup::getOrder()
// Purpose:    Implementation of KKSGroup::getOrder()
// Return:     int
////////////////////////////////////////////////////////////////////////

int KKSAttrGroup::order(void) const
{
   return m_order;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSGroup::setOrder(int newOrder)
// Purpose:    Implementation of KKSGroup::setOrder()
// Parameters:
// - newOrder
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSAttrGroup::setOrder(int newOrder)
{
   m_order = newOrder;
}

void KKSAttrGroup::setAttrViews(const KKSMap<int, KKSAttrView *> & a)
{
    m_attributes = a;
}

const KKSMap<int, KKSAttrView *> & KKSAttrGroup::attrViews() const
{
    return m_attributes;
}

const KKSList<KKSAttrView*> KKSAttrGroup::sortedViews() const
{
    //KKSMap<int, KKSAttrView*> attrs = m_gr->attrViews();
    QList<KKSAttrView *> attrs_list = m_attributes.values();
    qSort (attrs_list.begin(), attrs_list.end(), compareAttrViews);
    KKSList<KKSAttrView *> aViews;
    for (int i=0; i<attrs_list.count(); i++)
    {
        KKSAttrView * a = attrs_list[i];
        aViews.append(a);
    }

    return aViews;
}

const KKSAttrView * KKSAttrGroup::attrView(int id) const
{
    KKSAttrView * a = NULL;

    QMap<int, KKSAttrView *>::const_iterator i = m_attributes.find(id);
    while (i != m_attributes.end() && i.key() == id) {
        a = i.value();
        //
        // берем всегда первый атрибут
        //
        break;
     }

    return a;
}

KKSAttrView * KKSAttrGroup::attrView(int id)
{
    KKSAttrView * a = NULL;

     QMap<int, KKSAttrView *>::iterator i = m_attributes.find(id);
     while (i != m_attributes.end() && i.key() == id) {
         a = i.value();
         //берем всегда первый атрибут
         break;
     }

    return a;
}

/*
KKSAttrView * KKSAttrGroup::attrView(const QString & code)
{
    KKSAttrView * a = NULL;

    bool bFound = false;
    KKSMap<int, KKSAttrView*>::const_iterator pa;
    for (pa = m_attributes.constBegin(); pa != m_attributes.constEnd(); pa++)
    {
        a = pa.value();
        if(a && a->code() == code){
            bFound = true;
            break;
        }
    }
    
    if(!bFound)
        return NULL;

    return a;
}

const KKSAttrView * KKSAttrGroup::attrView(const QString & code) const
{
    KKSAttrView * a = NULL;

    bool bFound = false;
    KKSMap<int, KKSAttrView*>::const_iterator pa;
    for (pa = m_attributes.constBegin(); pa != m_attributes.constEnd(); pa++)
    {
        a = pa.value();
        if(a && a->code() == code){
            bFound = true;
            break;
        }
    }
    
    if(!bFound)
        return NULL;

    return a;
}
*/

int KKSAttrGroup::addAttrView(int id, KKSAttrView * a)
{
    int cnt = m_attributes.insert(id, a);
    if(!cnt)
        return ERROR_CODE;

    return OK_CODE;
}

int KKSAttrGroup::removeAttrView(int id, bool withRecursive)
{
    m_attributes.remove(id);
    
    if(!withRecursive)
        return OK_CODE;

    KKSMap<int, KKSAttrGroup*>::iterator pa;
    for (pa = m_childGroups.begin(); pa != m_childGroups.end(); pa++){
        KKSAttrGroup * g = pa.value();
        g->removeAttrView(id, withRecursive);
    }
    
    return OK_CODE;
}

/*
int KKSAttrGroup::removeAttrView(const QString & code)
{
    KKSAttrView * a = attrView(code);
    if(!a)
        return ERROR_CODE;

    m_attributes.remove(a->id());

    return OK_CODE;
}
*/

int KKSAttrGroup::replaceAttrView(int id, KKSAttrView * a)
{
    //
    // если удал€емый атрибут и новое значение ссылаютс€ на одну и ту же область,
    // то происходит аварийное завершение
    //
    KKSAttrView * oldAttrView =  m_attributes.value (id, 0);
    if (oldAttrView && oldAttrView != a)
        removeAttrView(id);
    int cnt = addAttrView(id, a);
    return cnt == 0 ? ERROR_CODE : OK_CODE;
}

// child groups
void KKSAttrGroup::setChildGroups(const KKSMap<int, KKSAttrGroup *> & gl)
{
    m_childGroups = gl;
}

const KKSMap<int, KKSAttrGroup *> & KKSAttrGroup::childGroups() const
{
    return m_childGroups;
}

const KKSList<KKSAttrGroup *> KKSAttrGroup::sortedChildGroups() const
{
    QList<KKSAttrGroup*> ag = m_childGroups.values();
    qSort (ag.begin(), ag.end(), compareAttrGroups);
    KKSList<KKSAttrGroup*> aGr;
    for (int i=0; i<ag.count(); i++)
    {
        KKSAttrGroup * g = ag[i];
        aGr.append(g);
    }

    return aGr;
}

int KKSAttrGroup::addChildGroup(int idGroup, KKSAttrGroup * g)
{
    int cnt = m_childGroups.insert(idGroup, g);
    if(!cnt)
        return ERROR_CODE;

    //найдем группу с таким же номером по пор€дку
    //если такова€ присутствует, то увеличим пор€док на 1
    if(g->order() == -1){
        KKSList<KKSAttrGroup *> gList = sortedChildGroups();
        int cnt = gList.count();
        if(cnt == 1)
            g->setOrder(0);
        else{
            KKSAttrGroup * lastGroup = gList.at(cnt-1);
            int lastOrder = lastGroup->order();
            lastOrder++;
            g->setOrder(lastOrder);

        }

    }
    
    return OK_CODE;
}

int KKSAttrGroup::prependChildGroup(int idGroup, KKSAttrGroup * g)
{
    int cnt = m_childGroups.insert(idGroup, g);
    if(!cnt)
        return ERROR_CODE;

    //найдем группу с таким же номером по пор€дку
    //если такова€ присутствует, то уменьшим пор€док на 1
    if(g->order() == -1){
        KKSList<KKSAttrGroup *> gList = sortedChildGroups();
        int cnt = gList.count();
        if(cnt == 1)
            g->setOrder(0);
        else{
            KKSAttrGroup * firstGroup = gList.at(0);
            int firstOrder = firstGroup->order();
            firstOrder--;
            g->setOrder(firstOrder);

        }

    }
    
    return OK_CODE;
}

int KKSAttrGroup::removeChildGroup(int idGroup)
{
    m_childGroups.remove(idGroup);
    return OK_CODE;
}
// end of child groups

void KKSAttrGroup::setParent(KKSAttrGroup * g)
{
    if(m_parent)
        m_parent->release();

    m_parent = g;
    if(m_parent)
        m_parent->addRef();
}

KKSAttrGroup * KKSAttrGroup::parent()
{
    return m_parent;
}

const KKSAttrGroup * KKSAttrGroup::parent() const 
{
    return m_parent;
}

//
// ‘ункции поиска подгруппы по заданному id наподобие аналогичных методов
// дл€ рубрик
// добавил ё.Ћ.–усинов
//
const KKSAttrGroup * KKSAttrGroup :: childGroupForId (int id, bool recursive) const
{
    if (this->id() == id)
        return this;

    KKSMap<int, KKSAttrGroup *>::const_iterator pg = m_childGroups.constFind (id);
    if (pg != m_childGroups.constEnd())
        return pg.value();
    else if (recursive)
    {
        const KKSAttrGroup * gr=0;
        for (pg=m_childGroups.constBegin(); pg != m_childGroups.constEnd() && gr==0; pg++)
        {
            gr = pg.value()->childGroupForId (id, recursive);//pg.key());
        }
        return gr;
    }

    return 0;
}

KKSAttrGroup * KKSAttrGroup :: childGroupForId (int id, bool recursive)
{
    if (this->id() == id)
        return this;

    KKSMap<int, KKSAttrGroup *>::const_iterator pg = m_childGroups.constFind (id);
    if (pg != m_childGroups.constEnd())
        return pg.value();
    else if (recursive)
    {
        KKSAttrGroup * gr=0;
        for (pg=m_childGroups.constBegin(); pg != m_childGroups.constEnd() && gr==0; pg++)
        {
            gr = pg.value()->childGroupForId (id, recursive);
        }
        return gr;
    }

    return 0;
}
//
// добавлено 15.11.2010
//
QList<int> KKSAttrGroup :: childKeys (void) const
{
    QList<int> gKeys = m_childGroups.keys();
    for (KKSMap<int, KKSAttrGroup *>::const_iterator pg=m_childGroups.constBegin(); pg != m_childGroups.constEnd(); pg++)
        gKeys += pg.value()->childKeys();

    return gKeys;
}
