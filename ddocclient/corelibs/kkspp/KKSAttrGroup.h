/***********************************************************************
 * Module:  KKSAttrGroup.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:09:20
 * Purpose: Declaration of the class KKSAttrGroup
 * Comment: Группа атрибутов в шаблоне с набором входящих в группу атрибутов
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttrGroup_h)
#define __KKSSITOOOM_KKSAttrGroup_h

#include <QList>
#include <QString>
#include "kkspp_config.h"

#include "KKSAttrView.h"
#include "KKSRecord.h"
#include "KKSGroup.h"
#include "KKSMap.h"
#include "KKSList.h"

class _PP_EXPORT KKSAttrGroup : public KKSRecord, public KKSGroup
{
public:
    KKSAttrGroup();
    KKSAttrGroup(const KKSAttrGroup & g);
    KKSAttrGroup(int id, const QString & name, int _order);
    virtual ~KKSAttrGroup();

    void setParent(KKSAttrGroup * g);
    KKSAttrGroup * parent();
    const KKSAttrGroup * parent() const;

    void setChildGroups(const KKSMap<int, KKSAttrGroup *> & gl);
    const KKSMap<int, KKSAttrGroup *> & childGroups() const;
    const KKSList<KKSAttrGroup *> sortedChildGroups() const;
    int addChildGroup(int idGroup, KKSAttrGroup * g);
    int removeChildGroup(int idGroup);

    int order(void) const;
    void setOrder(int newOrder);

    //В качестве ключа используется idAttribute! //НЕ idAttrView или idCategoryAttr !!
    void setAttrViews(const KKSMap<int, KKSAttrView *> & a);
    const KKSMap<int, KKSAttrView *> & attrViews() const;
    const KKSList<KKSAttrView*> sortedViews() const;

    int addAttrView(int idAttribute, KKSAttrView * a);//НЕ idAttrView или idCategoryAttr !!
    int removeAttrView(int idAttribute, bool withRecursive = false); //НЕ idAttrView или idCategoryAttr !! Второй параметр - удаление рекурсивное, просматриваем также все вложенные подгруппы
    //int removeAttrView(const QString & code);
    int replaceAttrView(int idAttribute, KKSAttrView * a); //НЕ idAttrView или idCategoryAttr !!

    const KKSAttrView * attrView(int idAttribute) const; //НЕ idAttrView или idCategoryAttr !!
    KKSAttrView * attrView(int idAttribute); //НЕ idAttrView или idCategoryAttr !!
    //KKSAttrView * attrView(const QString & code);
    //const KKSAttrView * attrView(const QString & code) const;

    //
    // Функции поиска подгруппы по заданному id наподобие аналогичных методов
    // для рубрик
    // добавил Ю.Л.Русинов
    //
    const KKSAttrGroup * childGroupForId (int id, bool recursive=true) const;
    KKSAttrGroup * childGroupForId (int id, bool recursive=true);
    QList<int> childKeys (void) const;

protected:
private:
    KKSMap<int, KKSAttrView*> m_attributes;
    int m_order;
    KKSAttrGroup * m_parent;

    KKSMap<int, KKSAttrGroup *> m_childGroups;
};

inline bool compareAttrGroups(KKSAttrGroup * item1, KKSAttrGroup * item2)
{
    if(item1->order() < item2->order()) 
        return true; 
    
    return false;
}


#endif

