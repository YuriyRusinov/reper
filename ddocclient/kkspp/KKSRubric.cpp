/***********************************************************************
 * Module:  KKSRubric.cpp
 * Author:  sergey
 * Modified: 25 декабря 2008 г. 17:22:35
 * Purpose: Implementation of the classes KKSRubric
 * Comment: используется для рубрикации
 ***********************************************************************/

#include "KKSRubric.h"
#include "KKSSearchTemplate.h"
#include "KKSPrivilege.h"
#include "KKSCategory.h"
#include "KKSAccessEntity.h"
#include "defines.h"

QPixmap * pxRubric = NULL;
QPixmap * pxRubricItem = NULL;

KKSRubricBase :: KKSRubricBase (void) : KKSRecord (),
    m_rubrIcon (QIcon()),
    m_iconData (QString()),
    m_subNodes (KKSList<const KKSRubricBase *>()),
    m_isInitialized(true),
    m_isChanged(false)
{
}

KKSRubricBase :: KKSRubricBase (qint64 id, const QString& name, const QString& desc) : KKSRecord (id, name, desc),
    m_rubrIcon (QIcon()),
    m_iconData (QString()),
    m_subNodes (KKSList<const KKSRubricBase *>()),
    m_isInitialized(true),
    m_isChanged(false)
{
}

KKSRubricBase :: KKSRubricBase (const KKSRubricBase& RB)
    : KKSRecord (RB),
      m_rubrIcon (RB.m_rubrIcon),
      m_iconData (RB.m_iconData),
      m_subNodes (RB.m_subNodes),
      m_isInitialized(RB.m_isInitialized),
      m_isChanged(RB.m_isChanged)
{
}

KKSRubricBase :: ~KKSRubricBase (void)
{
}
/*
int KKSRubricBase :: id() const
{
    return 0;
}
*/

KKSRubricBase& KKSRubricBase :: operator= (const KKSRubricBase& rb)
{
    m_rubrIcon = rb.m_rubrIcon;
    m_iconData = rb.m_iconData;
    m_subNodes = rb.m_subNodes;

    m_isInitialized = rb.m_isInitialized;
    m_isChanged = rb.m_isChanged;

    return *this;
}

QIcon KKSRubricBase :: getIcon (void) const
{
    return m_rubrIcon;
}

const QString& KKSRubricBase :: iconAsString() const
{
    return m_iconData;
}

void KKSRubricBase :: setIcon (const QString & s)
{
    QPixmap px;
    bool isOk = px.loadFromData(s.toUtf8(), "XPM");
    if (!isOk)
        return;

    m_rubrIcon = QIcon(px);
    m_iconData = s;
    m_isChanged = true;
}

void KKSRubricBase :: setIcon (const QIcon& icon)
{
    m_rubrIcon = icon;
    m_isChanged = true;
}

int KKSRubricBase :: childNumber (void) const
{
    if (parent() && static_cast<const KKSRubric *>(parent()))
    {
        int idc = -1;
        const KKSRubric * prubr = static_cast<const KKSRubric *>(parent());
        int rCount;
        if (this->rubricType() == atRubricItem)
            rCount = prubr->items().count();
        else
            rCount = prubr->rubrics().count();
        for (int i=0; i<rCount && idc<0; i++)
        {
            const KKSRubricBase * rw (0);
            if (this->rubricType() == atRubricItem)
                rw = prubr->item(i);
            else
                rw = prubr->rubric(i);
            
            if (rw->id() == id())
                idc = i;
        }
        if (this->rubricType() == atRubricItem && idc > 0)
            idc += prubr->rubrics().count();
        return idc;
    }
    else
        return m_subNodes.count();
}

void KKSRubricBase :: addNode (const KKSRubricBase * node)
{
    if (!node)
        return;

    m_subNodes.append(node);
}

void KKSRubricBase :: insertNode (int i, const KKSRubricBase * node)
{
    if (!node)
        return;

    m_subNodes.insert(i, node);
}

void KKSRubricBase :: clearNodes (void)
{
    m_subNodes.clear();
}

const KKSList<const KKSRubricBase *>& KKSRubricBase :: subnodes (void) const
{
    return m_subNodes;
}

KKSList<const KKSRubricBase *>& KKSRubricBase :: subnodes (void)
{
    return m_subNodes;
}

void KKSRubricBase :: setNodes (const KKSList<const KKSRubricBase *>& nodes)
{
    m_subNodes = nodes;
}

void KKSRubricBase :: setInitialized(bool yes)
{
    m_isInitialized = yes;
}

bool KKSRubricBase :: isInitialized() const
{
    return m_isInitialized;
}


/*=============*/

KKSRubricOthers :: KKSRubricOthers (void) : KKSRubricBase ()
{
}

KKSRubricOthers :: KKSRubricOthers (qint64 id, const QString& name, const QString& desc) : KKSRubricBase (id, name, desc)
{
}

KKSRubricOthers :: KKSRubricOthers (const KKSRubricOthers& RO) : KKSRubricBase (RO)
{
}

KKSRubricOthers :: ~KKSRubricOthers (void)
{
}

KKSRubricOthers& KKSRubricOthers :: operator= (const KKSRubricOthers& /*others*/)
{
    return *this;
}

void KKSRubricOthers :: setDefaultIcon (const QPixmap& px)
{
    Q_UNUSED (px);
}

QPixmap KKSRubricOthers :: getDefaultIcon (void) const
{
    return QPixmap ();
}

int KKSRubricOthers :: rubricType (void) const
{
    return KKSRubricBase::atOthers;
}

/*============*/
KKSRubricItem::KKSRubricItem() : KKSRubricBase(),
    m_isAutomated (false),
    m_isUpdated (false)
{
}

KKSRubricItem::KKSRubricItem(qint64 idItem, const QString & name, bool b, const QString& rIconStr) : KKSRubricBase(idItem, name),
    m_isAutomated (b),
    m_isUpdated (false)
{
    setIcon (rIconStr);
}

KKSRubricItem::KKSRubricItem(const KKSRubricItem & other) : KKSRubricBase (other),
    m_isAutomated (other.m_isAutomated),
    m_isUpdated (other.m_isUpdated)
{
}

KKSRubricItem::~KKSRubricItem()
{

}

KKSRubricItem& KKSRubricItem :: operator= (const KKSRubricItem& other)
{
    m_isAutomated = other.m_isAutomated;
    m_isUpdated = other.m_isUpdated;
    return *this;
}
/*
void KKSRubricItem::setId(int id)
{
    m_idItem = id;
}

int KKSRubricItem::id() const
{
    return m_idItem;
}

void KKSRubricItem::setName(const QString & name)
{
    m_name = name;
}

const QString & KKSRubricItem::name() const
{
    return m_name;
}
*/

void KKSRubricItem::setGeneralIcon(const QPixmap& px)
{
    if(pxRubricItem)
        delete pxRubricItem;

    pxRubricItem = new QPixmap(px);
}

void KKSRubricItem::setDefaultIcon(const QPixmap & px)
{
    KKSRubricItem::setGeneralIcon(px);
/*    if(pxRubricItem)
        delete pxRubricItem;

    pxRubricItem = new QPixmap(px);*/
}

QPixmap KKSRubricItem::getDefaultIcon() const
{
/*    QPixmap px;
    
    if(pxRubricItem)
        px = QPixmap(*pxRubricItem);
*/
    return KKSRubricItem::icon();
}

QPixmap KKSRubricItem::icon()
{
    QPixmap px;
    
    if(pxRubricItem)
        px = QPixmap(*pxRubricItem);

    return px;
    
}
/*
QIcon KKSRubricItem::getIcon (void) const
{
    return m_rubrItemIcon;
}

void KKSRubricItem :: setIcon (const QString & s)
{
    QPixmap px;
    px.loadFromData(s.toUtf8());

    m_rubrItemIcon = QIcon(px);
    m_iconData = s;
}

const QString KKSRubricItem :: iconAsString () const
{
    return m_iconData;
}
*/

int KKSRubricItem::rubricType (void) const
{
    return KKSRubricBase::atRubricItem;
}

/*===========================================*/
KKSRubric::KKSRubric() : KKSRubricBase(),
    m_searchTemplate (0),
    m_category (0),
    m_isCategorized (false),
    m_acl (0),
    m_intId (-1)
{

}

KKSRubric::KKSRubric(const KKSRubric & other) : KKSRubricBase(other),
    m_searchTemplate (other.m_searchTemplate),
    m_category (other.m_category),
    m_isCategorized (other.m_isCategorized),
    m_acl (other.m_acl),
/*    m_privileges(other.m_privileges),
    m_bossPrivileges(other.m_bossPrivileges),
    m_unitPrivileges(other.m_unitPrivileges),
    m_othersPrivilege(NULL),*/
    m_intId(other.m_intId)
{
    m_items = other.items();
    m_rubrics = other.rubrics();

    m_deletedRubrics = other.m_deletedRubrics;
    m_deletedItems = other.m_deletedItems;

    if (m_searchTemplate)
        m_searchTemplate->addRef ();

    if (m_category)
        m_category->addRef ();

    if (m_acl)
        m_acl->addRef ();
    //setOthersPrivilege(other.othersPrivilege());
}

KKSRubric::KKSRubric(int id, const QString & name, KKSSearchTemplate * st, KKSCategory * c, KKSAccessEntity * ac) : KKSRubricBase(id, name),
    m_searchTemplate (st),
    m_category (c),
    m_isCategorized (false),
    m_acl (ac),
/*    m_privileges (KKSMap<int, KKSPrivilege *>()),
    m_bossPrivileges (KKSMap<int, KKSPrivilege *>()),
    m_unitPrivileges (KKSMap<int, KKSPrivilege *>()),
    m_othersPrivilege (0),*/
    m_intId (-1)
{
    if (m_searchTemplate)
        m_searchTemplate->addRef ();
    if (m_category)
        m_category->addRef ();
    if (m_acl)
        m_acl->addRef ();
}

KKSRubric::~KKSRubric()
{
    if (m_searchTemplate)
        m_searchTemplate->release ();
    if (m_category)
        m_category->release ();
    if (m_acl)
        m_acl->release ();
}

KKSRubric & KKSRubric::operator = (const KKSRubric & other) 
{
    setId(other.id());
    setName(other.name());
    setCode(other.code());
    setDesc(other.desc());
    setParent(const_cast<KKSRecord*>(other.parent()));

    m_intId = other.m_intId;

    setItems(other.items());
    setRubrics(other.rubrics());

    m_deletedRubrics = other.m_deletedRubrics;
    m_deletedItems = other.m_deletedItems;
    
    m_searchTemplate = other.m_searchTemplate;
    if (m_searchTemplate)
        m_searchTemplate->addRef ();
    
    m_category = other.m_category;
    if (m_category)
        m_category->addRef ();
    
    //setOthersPrivilege (other.m_othersPrivilege);
    m_acl = other.m_acl;
    if (m_acl)
        m_acl->addRef();
    
//    m_rubricIcon = other.m_rubricIcon;
//    m_iconData = other.m_iconData;
    m_isCategorized = other.m_isCategorized;
    
    return *this;
}

void KKSRubric::addItem(const KKSRubricItem * item)
{
    if(!item)
        return;
    
    m_items.append(item);
    addNode (item);
    
    m_isChanged = true;
}

void KKSRubric::insertItem (int i, const KKSRubricItem * item)
{
    if(!item || i<0 || i>m_items.size())
        return;

    m_items.insert (i, item);
    insertNode (i, item);

    m_isChanged = true;
}

void KKSRubric::addItems(const KKSList<const KKSRubricItem*> & items)
{
    for (int i=0; i<items.count(); i++)
    {
        const KKSRubricItem * item = items[i];
        m_items.append(item);
        addNode (item);
    }

    m_isChanged = true;
}

void KKSRubric::setItems(const KKSList<const KKSRubricItem *> & items)
{
    //?????????!!!!!!!!!
    //m_deletedItems.append(m_items);
    
    m_items.clear();
    m_items = items;
    KKSList<const KKSRubricBase *> subNodes = subnodes();
    for (int i=0; i<subNodes.count(); )
    {
        if (subNodes[i]->rubricType() == KKSRubricBase::atRubricItem)
            subNodes.removeAt(i);
        else
            i++;
    }
    setNodes (subNodes);

    m_isChanged = true;
}

void KKSRubric::removeItem(int index)
{
    const KKSRubricItem * ri = m_items.at(index);
    if(!ri)
        return;

    KKSList<const KKSRubricBase *> subNodes = subnodes();
    subNodes.removeAll (ri);
    setNodes (subNodes);
    m_deletedItems.append(ri);

    m_items.removeAt(index);

    m_isChanged = true;
}

void KKSRubric::clear()
{
    clearItems();
    clearRubrics();
    clearNodes ();

    m_isChanged = true;
}

void KKSRubric::clearItems()
{
    //?????????!!!!!!!!!
    //m_deletedItems.append(m_items);

    m_items.clear();
    KKSList<const KKSRubricBase *> subNodes = subnodes();
    for (int i=0; i<subNodes.count(); )
    {
        if (subNodes[i]->rubricType() == KKSRubricBase::atRubricItem)
            subNodes.removeAt(i);
        else
            i++;
    }
    setNodes (subNodes);

    m_isChanged = true;
}

void KKSRubric::clearRubrics()
{
    //?????????!!!!!!!!!
    //m_deletedRubrics.append(m_rubrics);

    m_rubrics.clear();
    KKSList<const KKSRubricBase *> subNodes = subnodes();
    for (int i=0; i<subNodes.count(); )
    {
        if (subNodes[i]->rubricType() != KKSRubricBase::atRubricItem)
            subNodes.removeAt(i);
        else
            i++;
    }
    setNodes (subNodes);

    m_isChanged = true;
}

const KKSRubricItem * KKSRubric::item(int index) const
{
    return m_items.at(index);
}

const KKSList<const KKSRubricItem *> & KKSRubric::items() const
{
    return m_items;
}


void KKSRubric::addRubric(const KKSRubric * r)
{
    if(!r)
        return;
    
    m_rubrics.append(r);
    addNode (r);

    m_isChanged = true;
}

void KKSRubric::insertRubric (int i, const KKSRubric * r)
{
    if(!r || i<0 || i>m_rubrics.size())
        return;

    m_rubrics.insert(i, r);
    insertNode (i, r);

    m_isChanged = true;
}

void KKSRubric::addRubrics(const KKSList<const KKSRubric *> & rubrics)
{
    for (int i=0; i<rubrics.count(); i++)
    {
        const KKSRubric * r = rubrics[i];
        m_rubrics.append(r);
        addNode (r);
    }

    m_isChanged = true;
}

void KKSRubric::setRubrics(const KKSList<const KKSRubric*> & rubrics)
{
    //?????????!!!!!!!!!
    //m_deletedRubrics.append(m_rubrics);

    m_rubrics.clear();
    KKSList<const KKSRubricBase *> subNodes = subnodes();
    for (int i=0; i<subNodes.count(); )
    {
        if (subNodes[i]->rubricType() == KKSRubricBase::atRootRubric ||
            subNodes[i]->rubricType() == KKSRubricBase::atRubric ||
            subNodes[i]->rubricType() == KKSRubricBase::atRubricCategory
            )
            subNodes.removeAt(i);
        else
            i++;
    }
    setNodes (subNodes);
    m_rubrics = rubrics;

    m_isChanged = true;
}

void KKSRubric::removeRubric(int index)
{
    const KKSRubric * rr = m_rubrics.at(index);
    if(!rr)
        return;

    KKSList<const KKSRubricBase *> subNodes = subnodes();
    subNodes.removeAll (rr);
    setNodes (subNodes);
    m_deletedRubrics.append(rr);
    m_rubrics.removeAt(index);

    m_isChanged = true;
}

const KKSRubric * KKSRubric::rubric(int index) const
{
    return m_rubrics.at(index);
}

KKSRubric * KKSRubric::rubric(int index)
{
    if(index<m_rubrics.count() && index >= 0)
        return const_cast<KKSRubric*>(m_rubrics[index]);
    
    return NULL;
}

const KKSList<const KKSRubric*> & KKSRubric::rubrics() const
{
    return m_rubrics;
}

const KKSRubric * KKSRubric::rubricForId(int id, bool recursivelly) const
{
    if(id <= 0)
        return NULL;

    if(this->id() == id)
        return this;

    int cnt = m_rubrics.count();
    for(int i=0; i<cnt; i++){
        const KKSRubric * r = m_rubrics.at(i);
        if(r->id() == id){
            return r;
        }
        else if(recursivelly){
            const KKSRubric * r1 = r->rubricForId(id, recursivelly);
            if(r1)
                return r1;
        }
    }

    return NULL;
}

//возвращается рубрика по ее идентификатору
KKSRubric * KKSRubric::rubricForId(int id, bool recursivelly)
{
    if(id <= 0)
        return NULL;

    if(this->id() == id)
        return this;

    int cnt = m_rubrics.count();
    for(int i=0; i<cnt; i++){
        KKSRubric * r = const_cast<KKSRubric*>(m_rubrics[i]);
        if(r->id() == id){
            return r;
        }
        else if(recursivelly){
            KKSRubric * r1 = r->rubricForId(id, recursivelly);
            if(r1)
                return r1;
        }
    }

    return NULL;
}

//возвращается item, находящийся в данной рубрике по его id
//нерекурсивно
const KKSRubricItem * KKSRubric::itemForId(int id) const
{
    if(id <= 0)
        return NULL;

    int cnt = m_items.count();
    for(int i=0; i<cnt; i++){
        const KKSRubricItem * item = m_items.at(i);
        if(item->id() == id){
            return item;
        }
    }

    return NULL;
}

KKSRubricItem * KKSRubric::itemForId(int id)
{
    if(id <= 0)
        return NULL;

    int cnt = m_items.count();
    for(int i=0; i<cnt; i++){
        KKSRubricItem * item = const_cast<KKSRubricItem*>(m_items[i]);
        if(item->id() == id){
            return item;
        }
    }

    return NULL;
}

void KKSRubric::setGeneralIcon(const QPixmap & px)
{
    if(pxRubric)
        delete pxRubric;

    pxRubric = new QPixmap(px);
}

QPixmap KKSRubric::icon()
{
    QPixmap px;
    
    if(pxRubric)
        px = QPixmap(*pxRubric);

    return px;
}

KKSRubric * KKSRubric::deepCopy(bool dropIds) const
{
    KKSRubric * copy = new KKSRubric(this->id(), this->name());
    copy->setIcon(this->icon());
    if(dropIds)
        copy->setId(-1);

    KKSList<const KKSRubric *> subRubrics = this->rubrics();
    for(int i=0; i<subRubrics.count(); i++){
        KKSRubric * r = subRubrics.at(i)->deepCopy(dropIds);//deep copy
        copy->addRubric(r);
        r->release();
    }

    KKSList<const KKSRubricItem *> items = this->items();
    for(int i=0; i<items.count(); i++){
        KKSRubricItem * item = new KKSRubricItem(*(items.at(i)));
        copy->addItem(item);
        item->release();
    }

    return copy;
}

KKSSearchTemplate * KKSRubric::getSearchTemplate (void) const
{
    return m_searchTemplate;
}

void KKSRubric::setSearchTemplate (KKSSearchTemplate * st)
{
    if (m_searchTemplate)
        m_searchTemplate->release ();

    m_searchTemplate = st;

    if (m_searchTemplate)
        m_searchTemplate->addRef ();

    m_isChanged = true;
}

KKSCategory * KKSRubric::getCategory (void) const
{
    return m_category;
}

void KKSRubric::setCategory (KKSCategory *c)
{
    if (m_category)
        m_category->release ();

    m_category = c;

    if (m_category)
        m_category->addRef ();

    m_isChanged = true;
}

KKSAccessEntity * KKSRubric::getAccessRules (void) const
{
    return m_acl;
}

void KKSRubric::setAccessRules (KKSAccessEntity * _acl)
{
    if (m_acl)
        m_acl->release ();

    m_acl = _acl;

    if (m_acl)
        m_acl->addRef ();

    m_isChanged = true;
}

QString KKSRubric::getFullTreeOfIdsString() const
{
    QString ids;
    ids = QString::number(this->id());

    for(int i=0; i<m_rubrics.count(); i++){
        const KKSRubric * r = m_rubrics.at(i); 
        if(r){
            ids += ", " + r->getFullTreeOfIdsString();
        }
    }

    return ids;
}

//рассматривать удаленные подрубрики удаленных рубрик нет необходимости, 
//поскольку серверная функция гарантирует удаление вложенных подрубрик и вложений удаляемой рубрики
QString KKSRubric::getFullTreeOfDeletedIds() const
{
    QString ids;
    //ids = QString::number(this->id());

    for(int i=0; i<m_deletedRubrics.count(); i++){
        const KKSRubric * r = m_deletedRubrics.at(i); 
        if(r){
            if(ids.isEmpty())
                ids += QString::number(r->id());
            else
                ids += ", " + QString::number(r->id());
        }
    }

    for(int i=0; i<m_rubrics.count(); i++){
        const KKSRubric * r = m_rubrics.at(i); 
        if(r){
            QString idsTmp = r->getFullTreeOfDeletedIds();
            if(idsTmp.isEmpty())
                continue;

            if(ids.isEmpty())
                ids += idsTmp;
            else
                ids += ", " + idsTmp;
        }
    }

    return ids;
}

/*
QIcon KKSRubric :: getIcon (void) const
{
    return m_rubricIcon;
}

void KKSRubric :: setIcon (const QString & s)
{
    QPixmap px;
    bool isOk = px.loadFromData(s.toUtf8(), "XPM");
    if (!isOk)
        return;

    m_rubricIcon = QIcon(px);
    m_iconData = s;
}

const QString KKSRubric :: iconAsString () const
{
    return m_iconData;
}
 */

void KKSRubric :: setDefaultIcon (const QPixmap& px)
{
    KKSRubric::setGeneralIcon (px);
/*    if(pxRubric)
        delete pxRubric;

    pxRubric = new QPixmap(px);
 */
}

QPixmap KKSRubric :: getDefaultIcon (void) const
{
    return KKSRubric::icon();
/*    QPixmap px;
    
    if(pxRubric)
        px = QPixmap(*pxRubric);

    return px;
 */
}

int KKSRubric :: rubricType (void) const
{
    if (m_isCategorized)
        return KKSRubricBase::atRubricCategory;
    else
        return KKSRubricBase::atRubric;

}

bool KKSRubric :: isCategorized (void) const
{
    return m_isCategorized;
}

void KKSRubric :: setCategorized (bool c)
{
    m_isCategorized = c;
}

const KKSList<const KKSRubricItem *> & KKSRubric :: deletedItems() const
{
    return m_deletedItems;
}

