/***********************************************************************
 * Module:  KKSAttribute.h
 * Author:  sergey
 * Modified: 25 но€бр€ 2008 г. 17:39:56
 * Purpose: Declaration of the class KKSAttribute
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttribute_h)
#define __KKSSITOOOM_KKSAttribute_h

#include <QMetaType>
class KKSAttrType;

#include "kkspp_config.h"
#include <KKSRecord.h>
#include <KKSIndAttr.h>
//#include <KKSAttrAttr.h>
#include "KKSMap.h"
#include "KKSList.h"
#include "KKSValue.h"

class KKSSearchTemplate;
class KKSAttribute;
class KKSAttrAttr;

class _PP_EXPORT KKSAGroup : public KKSRecord
{
public:
    KKSAGroup();
    KKSAGroup(const KKSAGroup & g);
    KKSAGroup(int id, const QString & name);
    virtual ~KKSAGroup();

    void setParent(KKSAGroup * g);
    KKSAGroup * parent();
    const KKSAGroup * parent() const;

    void setChildGroups(const KKSMap<int, KKSAGroup *> & gl);
    const KKSMap<int, KKSAGroup *> & childGroups() const;
    int addChildGroup(int idGroup, KKSAGroup * g);
    int removeChildGroup(int idGroup);

    const KKSAGroup * childGroupForId (int id, bool recursive=true) const;
    KKSAGroup * childGroupForId (int id, bool recursive=true);
    QList<int> childKeys (void) const;

    void setAttributes (const KKSMap<int, KKSAttribute*>& attrs);
    const KKSMap<int, KKSAttribute*> & getAttributes (void) const;
    int addAttribute (int idAttr, KKSAttribute * a);
    int removeAttribute (int idAttr);
    
protected:
private:
    KKSAGroup * m_parent;

    KKSMap<int, KKSAGroup *> m_childGroups;
    KKSMap<int, KKSAttribute *> m_Attrs;
};

class _PP_EXPORT KKSAttribute : public KKSIndAttr//KKSRecord
{
public:
    KKSAttribute();
    KKSAttribute(const KKSAttribute & a);
    virtual ~KKSAttribute();
   
    const QString & title(void) const;
    void setTitle(const QString & newTitle);

    const KKSSearchTemplate * searchTemplate() const;
    KKSSearchTemplate * searchTemplate();
    void setSearchTemplate(KKSSearchTemplate * st);

    int defWidth() const;
    void setDefWidth(int w);

    const KKSAGroup * group() const;
    KKSAGroup * group();
    void setGroup(KKSAGroup * _group);

    virtual bool isSystem() const;
    void setAsSystem(bool yes = true);
    virtual bool isAttribute (void) const;

    void setAttrsAttrs(const KKSMap<int, KKSAttrAttr *> & attrs);
    const KKSMap<int, KKSAttrAttr*> & attrsAttrs() const;
    KKSMap<int, KKSAttrAttr*> & attrsAttrs();


protected:
private:
    QString m_title;

    int m_defWidth;
    KKSAGroup* m_group;

    KKSSearchTemplate * m_st;

    bool m_isSystem;

    KKSMap<int, KKSAttrAttr*> m_attrsAttrs;//список атрибутов, описывающих данный атрибут. 
                                           //ƒл€ ускорени€ работы системы загружаютс€ только по отдельной команде
                                           //имеетс€ флаг m_attrsLoaded, определ€ющий факт загруженности данных атрибутов
                                           //‘акт загруженности определ€етс€ тем, вызывалс€ ли метод setAttrsAttrs(). 
                                           //≈сли вызывалс€, то атрибуты были загружены
    bool m_attrsLoaded;
};


class _PP_EXPORT KKSAttrAttr : public KKSAttribute
{
public:
    bool isMandatory(void) const;
    void setMandatory(bool newIsMandatory);
    bool isReadOnly(void) const;
    void setReadOnly(bool newIsReadOnly);

    int idParentAttr();
    int idParentAttr() const;
    void setIdParentAttr(int id);

    int idAttrAttr();
    int idAttrAttr() const;
    void setIdAttrAttr(int id);

    const KKSValue & defValue() const;
    KKSValue  & defValue();
    void setDefValue(const KKSValue & _defValue);

    KKSAttrAttr();
    KKSAttrAttr(const KKSAttribute & a);
    KKSAttrAttr(const KKSAttrAttr & a);
    virtual ~KKSAttrAttr();

    //метод создает экземпл€р данного класса по заданным параметрам
    //если строковое представление значени€ по умолчанию в данном атрибуте
    //не удалось привести к типу атрибута, то bBadValue устанавливаетс€ в TRUE
    //в противном случае в FALSE
    static KKSAttrAttr * create(KKSAttribute * a, 
                                bool bMandatory, 
                                bool bReadOnly, 
                                const QString & defValue = QString::null,
                                bool * bBadValue = NULL);

    static void initDefIdParent();
    static int getDefIdParent();
    static void decDefIdParent();


protected:
private:

    int m_rowId;//id в таблице attrs_attrs
    int m_idParentAttr;//идентификуатор описываемого атрибута

    bool m_isMandatory;
    bool m_isReadOnly;
    KKSValue m_defValue;

};

Q_DECLARE_METATYPE (KKSAGroup*);
Q_DECLARE_METATYPE (KKSAttribute*);

#endif
