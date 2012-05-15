/***********************************************************************
 * Module:  KKSTemplate.cpp
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:12:58
 * Purpose: Implementation of the class KKSTemplate
 ***********************************************************************/

#include "KKSCategory.h"
#include "KKSAttrGroup.h"
#include "KKSTemplate.h"
#include "defines.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSTemplate::KKSTemplate()
// Purpose:    Implementation of KKSTemplate::KKSTemplate()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSTemplate::KKSTemplate() : KKSRecord()
{
    m_category = NULL;
}

KKSTemplate::KKSTemplate(const KKSTemplate & t) : KKSRecord(t)
{
    m_category = NULL;
    setCategory(const_cast<KKSCategory*>(t.category()));
    m_groups = t.groups();
}

KKSTemplate & KKSTemplate::operator = (const KKSTemplate & t) 
{
    
    setId(t.id());
    setCode(t.code());
    setDesc(t.desc());
    setName(t.name());
    setParent(const_cast<KKSRecord*>(t.parent()));
    
    setCategory(const_cast<KKSCategory*>(t.category()));
    m_groups = t.groups();
   
    return *this;
}

KKSTemplate::KKSTemplate(int id, const QString & name, KKSCategory * c) : KKSRecord(id, name)
{
    m_category = NULL;
    setCategory(c);
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSTemplate::~KKSTemplate()
// Purpose:    Implementation of KKSTemplate::~KKSTemplate()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSTemplate::~KKSTemplate()
{
   if(m_category)
       m_category->release();

}

const KKSCategory * KKSTemplate::category() const
{
    return m_category;
}

KKSCategory * KKSTemplate::category()
{
    return m_category;
}

void KKSTemplate::setCategory(KKSCategory * c)
{
    if(m_category)
        m_category->release();

    m_category = c;
    if(m_category)
        m_category->addRef();
}

const KKSMap<int, KKSAttrGroup *> & KKSTemplate::groups() const
{
    return m_groups;
}

void KKSTemplate::setGroups(const KKSMap<int, KKSAttrGroup *> & g)
{
    m_groups = g;
}

void KKSTemplate::addGroup(KKSAttrGroup * g)
{
    if(!g)
        return;

    m_groups.insert (g->id(), g);
//    m_groups.append(g);
}

int KKSTemplate::removeGroup( KKSAttrGroup * g, bool recursive)
{
    if(!g)
        return ERROR_CODE;

    //static int steps = 0;
    //steps++;
    bool isUsed = false;
    KKSMap<int, KKSAttrGroup*>::const_iterator pg = m_groups.constBegin();
    for (; pg != m_groups.constEnd(); pg++)
    {
        KKSAttrGroup * gr = pg.value();
        if((KKSAttrGroup *)(gr->parent()) == g)
        {
            isUsed = true;
            break;
        }

    }
    if(isUsed && !recursive)
        return ERROR_CODE;

    if(!isUsed){
        m_groups.remove (g->id());//removeAll(g);
        return OK_CODE;
    }

    for (pg = m_groups.constBegin(); pg != m_groups.constEnd(); pg++)
    {
        KKSAttrGroup * gr = pg.value();//m_groups[i];
        if((KKSAttrGroup *)(gr->parent()) == g){
            int ok = removeGroup(gr, true);
            if(ok != OK_CODE)
                return ERROR_CODE;
        }
    }

    m_groups.remove (g->id());//removeAll(g);
    return OK_CODE;
}

int KKSTemplate::removeGroup(int index, bool recursive)
{
    //m_groups.removeAt(index);
    KKSMap<int, KKSAttrGroup *>::const_iterator pg = m_groups.constFind (index);
    if (pg == m_groups.constEnd())
        return ERROR_CODE;
    KKSAttrGroup * g = pg.value();
    if(!g)
        return ERROR_CODE;

    return removeGroup(g, recursive);;
}

KKSAttrGroup * KKSTemplate::group (int index)
{
    KKSMap<int, KKSAttrGroup *>::const_iterator pg = m_groups.constFind (index);
    if (pg == m_groups.constEnd())
        return 0;
    KKSAttrGroup * g = pg.value();
    return g;
}

const KKSAttrGroup * KKSTemplate::group(int index) const
{
    KKSMap<int, KKSAttrGroup *>::const_iterator pg = m_groups.constFind (index);
    if (pg == m_groups.constEnd())
        return 0;
    const KKSAttrGroup * g = pg.value();
    return g;
}

int KKSTemplate::attrsCount() const
{
    int cnt = 0;

    KKSMap<int, KKSAttrGroup *>::const_iterator pg = m_groups.constBegin();
    for (; pg != m_groups.constEnd(); pg++)
    {
        KKSAttrGroup * g = pg.value();//m_groups[i];
        cnt += g->attrViews().count();
    }

    return cnt;
}

const KKSList<KKSAttrView*> KKSTemplate::sortedAttrs() const
{
    KKSList<KKSAttrView *> aViews;
    
    QList<KKSAttrGroup*> groups = m_groups.values();
    qSort(groups.begin(), groups.end(), compareAttrGroups);
    
    for (int i=0; i<groups.count(); i++)
    {
        KKSAttrGroup * g = groups[i];
        KKSMap<int, KKSAttrView*> attrs = g->attrViews();
        QList<KKSAttrView *> attrs_list = attrs.values();
        qSort (attrs_list.begin(), attrs_list.end(), compareAttrViews);

        for (int ii=0; ii<attrs_list.count(); ii++)
        {
            KKSAttrView * a = attrs_list [ii];
            aViews.append(a);
        }
    }

    return aViews;
}

const KKSMap<int, KKSAttrView *> KKSTemplate::attributes() const
{
    KKSMap<int, KKSAttrView *> aViews;
    
    QList<KKSAttrGroup*> groups = m_groups.values();
    
    for (int i=0; i<groups.count(); i++)
    {
        KKSAttrGroup * g = groups[i];
        KKSMap<int, KKSAttrView*> attrs = g->attrViews();
        QList<KKSAttrView *> attrs_list = attrs.values();

        for (int ii=0; ii<attrs_list.count(); ii++)
        {
            KKSAttrView * a = attrs_list [ii];
            aViews.insert(a->id(), a);
        }
    }

    return aViews;

}


const KKSMap<int, KKSCategoryAttr *> KKSTemplate::availableAttrs() const
{
    KKSMap<int, KKSCategoryAttr *> aAttrs;
    KKSMap<int, KKSAttrView *> assignedAttrs;

    //KKSAttrGroup * g;
    KKSMap<int, KKSAttrGroup *>::const_iterator pg = m_groups.constBegin();
    //
    // Исправлено в связи с появлением подгрупп атрибутов
    //
    for (; pg != m_groups.constEnd(); pg++)
    {
        KKSAttrGroup * g = pg.value();//m_groups[i];
        KKSMap<int, KKSAttrView *> attrs = groupAttrs (g);//g->attrViews();
        KKSMap<int, KKSAttrView *>::const_iterator pa;
        for (pa = attrs.constBegin(); pa != attrs.constEnd(); pa++)
        {
            KKSAttrView *av = pa.value();
            assignedAttrs.insert(av->id(), av);
        }
    }

    KKSMap<int, KKSCategoryAttr*> cAttrs = category()->attributes();
    KKSMap<int, KKSCategoryAttr*>::const_iterator pca;
    for (pca = cAttrs.constBegin(); pca != cAttrs.constEnd(); pca++)
    {
        KKSCategoryAttr * ca = pca.value();
        int id = ca->id();
        int count = assignedAttrs.count(id);
        if(count == 0)
            aAttrs.insert(ca->id(), ca);
    }

    return aAttrs;
}

//
// Семейство методов поиска группы по id
// добавлено Ю.Л.Русиновым 16.11.2010
//
KKSAttrGroup * KKSTemplate::searchGroupById (int idGroup, bool recursive)
{
    KKSMap<int, KKSAttrGroup *>::const_iterator pg = m_groups.constFind (idGroup);
    if (pg != m_groups.constEnd())
        return pg.value();
    else if (recursive)
    {
        KKSAttrGroup * gr = 0;
        for (pg=m_groups.constBegin(); pg != m_groups.constEnd() && gr==0; pg++)
        {
            KKSAttrGroup * g = pg.value();
            gr = g->childGroupForId (idGroup, recursive);
        }
        return gr;
    }

    return 0;
}

const KKSAttrGroup * KKSTemplate::searchGroupById (int idGroup, bool recursive) const
{
    KKSMap<int, KKSAttrGroup *>::const_iterator pg = m_groups.constFind (idGroup);
    if (pg != m_groups.constEnd())
        return pg.value();
    else if (recursive)
    {
        KKSAttrGroup * gr = 0;
        for (pg=m_groups.constBegin(); pg != m_groups.constEnd() && gr==0; pg++)
        {
            KKSAttrGroup * g = pg.value();
            gr = g->childGroupForId (idGroup, recursive);
        }
        return gr;
    }

    return 0;
}

KKSMap<int, KKSAttrView *> KKSTemplate::groupAttrs (KKSAttrGroup * g) const
{
    KKSMap<int, KKSAttrView *> assignedAttrs;
    if (!g)
        return assignedAttrs;

    //KKSAttrGroup * g;
    KKSMap<int, KKSAttrView *> attrs = g->attrViews();
    KKSMap<int, KKSAttrView *>::const_iterator pa;
    for (pa = attrs.constBegin(); pa != attrs.constEnd(); pa++)
    {
        KKSAttrView *av = pa.value();
        assignedAttrs.insert(av->id(), av);
    }

    KKSMap<int, KKSAttrGroup*> childGroups = g->childGroups();
    for (KKSMap<int, KKSAttrGroup*>::const_iterator pg = childGroups.constBegin(); \
                                                    pg != childGroups.constEnd(); \
                                                    pg++)
    {
        KKSAttrGroup * gc = pg.value();
        if (!gc)
            continue;
        KKSMap<int, KKSAttrView *> g_attrs = groupAttrs (gc);
        KKSMap<int, KKSAttrView *>::const_iterator pag;
        for (pag = g_attrs.constBegin(); pag != g_attrs.constEnd(); pag++)
        {
            KKSAttrView *av = pag.value();
            assignedAttrs.insert(av->id(), av);
        }
    }

    return assignedAttrs;
}

QList<int> KKSTemplate :: groupKeys (void) const
{
    QList<int> gkeys = m_groups.keys();
//    KKSMap<int, KKSAttrGroup*> childGroups = g->childGroups();
    for (KKSMap<int, KKSAttrGroup*>::const_iterator pg = m_groups.constBegin(); \
                                                    pg != m_groups.constEnd(); \
                                                    pg++)
    {
        QList<int> gSubKeys = pg.value()->childKeys();
        gkeys += gSubKeys;
    }
    return gkeys;
}
