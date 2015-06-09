/***********************************************************************
 * Module:  KKSAttribute.h
 * Author:  sergey
 * Modified: 25 ноября 2008 г. 17:39:56
 * Purpose: Declaration of the class KKSAttribute
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttribute_h)
#define __KKSSITOOOM_KKSAttribute_h

#include <QMetaType>
class KKSAttrType;

#include "kkspp_config.h"
#include <KKSRecord.h>
#include <KKSIndAttr.h>
#include "KKSMap.h"
#include "KKSList.h"
#include "KKSValue.h"

class KKSSearchTemplate;
class KKSAttribute;
class KKSCategoryAttr;


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

    virtual KKSIndAttrClass isSystem() const;
    void setAsSystem(KKSIndAttrClass yes = iacTableAttr);
    virtual bool isAttribute (void) const;

    void setAttrs(const KKSMap<int, KKSCategoryAttr *> & attrs);
    const KKSMap<int, KKSCategoryAttr*> & attrs() const;
    KKSMap<int, KKSCategoryAttr*> & attrs();

    const KKSCategoryAttr * attrAttr(int id) const;
    KKSCategoryAttr * attrAttr(int id);

protected:
private:
    QString m_title;

    int m_defWidth;
    KKSAGroup* m_group;

    KKSSearchTemplate * m_st;

    KKSIndAttrClass m_isSystem;

    KKSMap<int, KKSCategoryAttr*> m_attrs; //список атрибутов, описывающих данный атрибут. 
                                           //Для ускорения работы системы загружаются только по отдельной команде
                                           //имеется флаг m_attrsLoaded, определяющий факт загруженности данных атрибутов
                                           //Факт загруженности определяется тем, вызывался ли метод setAttrsAttrs(). 
                                           //Если вызывался, то атрибуты были загружены
    bool m_attrsLoaded;
};

Q_DECLARE_METATYPE (const KKSAttribute *);

class _PP_EXPORT KKSCategoryAttr : public KKSAttribute
{
public:
   bool isMandatory(void) const;
   void setMandatory(bool newIsMandatory);
   bool isReadOnly(void) const;
   void setReadOnly(bool newIsReadOnly);

   int order() const;
   void setOrder(int o);

   const QString & directives() const;
   void setDirectives(const QString & d);

   const KKSValue & defValue() const;
   KKSValue  & defValue();
   void setDefValue(const KKSValue & _defValue);

   int idRow() const;
   int idRow();
   void setIdRow(int id);

    int idParent();
    int idParent() const;
    void setIdParent(int id);
    
    KKSCategoryAttr();
    KKSCategoryAttr(const KKSAttribute & a);
    KKSCategoryAttr(const KKSCategoryAttr & a);
    virtual ~KKSCategoryAttr();

    //метод создает экземпляр данного класса по заданным параметрам
    //если строковое представление значения по умолчанию в данном атрибуте
    //не удалось привести к типу атрибута, то bBadValue устанавливается в TRUE
    //в противном случае в FALSE
    static KKSCategoryAttr * create(KKSAttribute * a, 
                                    bool bMandatory, 
                                    bool bReadOnly, 
                                    const QString & defValue = QString::null,
                                    int order = 1,
                                    const QString & directives = QString::null,
                                    bool * bBadValue = NULL);


    static void initDefIdRow();
    static int getDefIdRow();
    static void decDefIdRow();

protected:
private:

   bool m_isMandatory;
   bool m_isReadOnly;
   int m_order;//порядок отображения атрибута в категории
   QString m_directives; //Формализованные параметры по управлению отображением и поведением атрибута в категории (в специализированном формате)
   int m_idRow; //id в таблице attrs_categories или в таблице attrs_attrs

   int m_idParent;//идентификатор описываемого атрибута (который содержит данный экземпляр класса) или категории (для KKSCategoryAttr)
   
   KKSValue m_defValue;


};

inline bool compareCategoryAttrs(KKSCategoryAttr * item1, KKSCategoryAttr * item2)
{
    if(item1->order() < item2->order()) 
        return true; 
    
    return false;
}


Q_DECLARE_METATYPE (KKSCategoryAttr *);
Q_DECLARE_METATYPE (const KKSCategoryAttr *);

Q_DECLARE_METATYPE (KKSAGroup*);
Q_DECLARE_METATYPE (KKSAttribute*);

#endif
