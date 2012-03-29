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
#include "KKSMap.h"
#include "KKSList.h"

class KKSSearchTemplate;
class KKSAttribute;

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
   
/*    const QString & refColumnName(bool quoted = false) const;
    void setRefColumnName(const QString & newColumnName);

    const QString & columnName(bool quoted = false) const;
    void setColumnName(const QString & newColumnName);
    const QString & tableName(void) const;
    void setTableName(const QString & newTableName);
 */
    const QString & title(void) const;
    void setTitle(const QString & newTitle);

    const KKSSearchTemplate * searchTemplate() const;
    KKSSearchTemplate * searchTemplate();
    void setSearchTemplate(KKSSearchTemplate * st);

    /*
#ifdef Q_WS_WIN
    //for web 
    std::string columnName_w(void) const;
    void setColumnName_w(std::string & newColumnName);
    std::string tableName_w(void) const;
    void setTableName(std::string & newTableName);
    std::string title_w(void) const;
    void setTitle_w(std::string & newTitle);
#endif
*/
    int defWidth() const;
    void setDefWidth(int w);
/*
    const KKSAttrType * type() const;
    KKSAttrType * type();
    void setType(KKSAttrType * _type);
*/
    const KKSAGroup * group() const;
    KKSAGroup * group();
    void setGroup(KKSAGroup * _group);
/*
    const KKSAttrType * refType() const;
    KKSAttrType * refType();
    void setRefType(KKSAttrType * _type);

    const KKSAttrType * refColumnType() const;
    KKSAttrType * refColumnType();
    void setRefColumnType(KKSAttrType * _type);
*/
    virtual bool isSystem() const;
    void setAsSystem(bool yes = true);
    virtual bool isAttribute (void) const;
protected:
private:
    QString m_title;
/*    QString m_tableName;
    QString m_columnName;
    
    QString m_refColumnName;
    KKSAttrType * m_refColumnType;
    
    mutable QString m_quotedColumnName;
    mutable QString m_quotedRefColumnName;
*/
    int m_defWidth;
//    KKSAttrType* m_type;
    KKSAGroup* m_group;
//    KKSAttrType* m_refType;

    KKSSearchTemplate * m_st;

    bool m_isSystem;
};

Q_DECLARE_METATYPE (KKSAGroup*);
Q_DECLARE_METATYPE (KKSAttribute*);

#endif
