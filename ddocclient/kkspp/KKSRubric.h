/***********************************************************************
 * Module:  KKSRubric.h
 * Author:  sergey
 * Modified: 25 декабря 2008 г. 17:22:35
 * Purpose: Declaration of the class KKSRubric
 * Comment: рубрики рубрикатора и элементы рубрик
 *    используется при работе с рубрикатором.
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSRubric_h)
#define __KKSSITOOOM_KKSRubric_h

#include <QMetaType>

#include "KKSList.h"
#include "KKSMap.h"
#include "KKSRecord.h"
#include "KKSPrivilege.h"
#include "kkspp_config.h"
#include <QIcon>

class KKSSearchTemplate;
//class KKSPrivilege;
class KKSCategory;
class KKSAccessEntity;

class _PP_EXPORT KKSRubricBase : public KKSRecord
{
public:
    KKSRubricBase (void);
    KKSRubricBase (qint64 id, const QString& name, const QString& desc=QString());
    KKSRubricBase (const KKSRubricBase& RB);
    virtual ~KKSRubricBase (void);

    KKSRubricBase& operator= (const KKSRubricBase& rb);

    virtual void setDefaultIcon (const QPixmap& px)=0;
    virtual QPixmap getDefaultIcon (void) const=0;

    QIcon getIcon (void) const;
    //void setIcon (const QIcon& icon);
    const QString& iconAsString() const;
    void setIcon (const QString & s);
    
    enum RubricBaseType
    {
        atRootRubric = 0,
        atRubric = 1,
        atRubricItem = 2,
        atRubricCategory = 3,
        atOthers = 4
    };
    
    virtual int rubricType (void) const=0;
    
    int childNumber (void) const;
protected:
    
    QIcon m_rubrIcon;
    QString m_iconData;
};

class _PP_EXPORT KKSRubricOthers : public KKSRubricBase
{
public:
    KKSRubricOthers (void);
    KKSRubricOthers (qint64 id, const QString& name, const QString& desc=QString());
    KKSRubricOthers (const KKSRubricOthers& RO);
    virtual ~KKSRubricOthers (void);
    
    KKSRubricOthers& operator= (const KKSRubricOthers& others);

    virtual void setDefaultIcon (const QPixmap& px);
    virtual QPixmap getDefaultIcon (void) const;
    virtual int rubricType (void) const;

    void addNode (const KKSRubricBase * node);
    void insertNode (int i, const KKSRubricBase * node);
    void clearNodes (void);
    
    const KKSList<const KKSRubricBase *>& subnodes (void) const;
    KKSList<const KKSRubricBase *>& subnodes (void);
    void setNodes (const KKSList<const KKSRubricBase *>& nodes);

private:
    KKSList<const KKSRubricBase *> m_subNodes;
};

class _PP_EXPORT KKSRubricItem : public KKSRubricBase//public KKSData
{
public:

    KKSRubricItem();
    KKSRubricItem(qint64 idItem, const QString & name, bool b = false);
    KKSRubricItem(const KKSRubricItem & other);
    ~KKSRubricItem();
    
    KKSRubricItem& operator= (const KKSRubricItem& other);
    
/*    void setId(int id);
    int id() const;

    void setName(const QString & name);
    const QString & name() const;
*/
    static void setIcon(const QPixmap & px);
    static QPixmap icon();
 
    bool isAutomated() const {return m_isAutomated;}
    void setAutomated(bool b) {m_isAutomated = b;}

    bool isUpdated() const {return m_isUpdated;}
    void setUpdated(bool b) {m_isUpdated = b;}

    virtual void setDefaultIcon (const QPixmap& px);
    virtual QPixmap getDefaultIcon (void) const;

/*    QIcon getIcon (void) const;
    //void setIcon (const QIcon& icon);
    const QString iconAsString() const;
    void setIcon (const QString & s);
 */
    virtual int rubricType (void) const;

private:
    
//    int m_idItem;
//    QString m_name;
    bool m_isAutomated;
    bool m_isUpdated;
    
    //QIcon m_rubrItemIcon;
    //QString m_iconData;
};


class _PP_EXPORT KKSRubric : public KKSRubricBase
{
public:
    KKSRubric();
    KKSRubric(int id, const QString & name, KKSSearchTemplate * st=0, KKSCategory * c=0, KKSAccessEntity * ac=0);
    KKSRubric(const KKSRubric & other);
    ~KKSRubric();
    
    KKSRubric & operator = (const KKSRubric & other);

    void addItem(const KKSRubricItem * item);
    void insertItem (int i, const KKSRubricItem * item);
    void addItems(const KKSList<const KKSRubricItem *> & items);
    void setItems(const KKSList<const KKSRubricItem *> & items);
    void removeItem(int index);

    void addRubric(const KKSRubric * r);
    void insertRubric (int i, const KKSRubric * r);
    void addRubrics(const KKSList<const KKSRubric *> & rubrics);
    void setRubrics(const KKSList<const KKSRubric *> & rubrics);
    void removeRubric(int index);

    void clear();
    void clearItems();
    void clearRubrics();

    const KKSRubricItem * item(int index) const;
    const KKSList<const KKSRubricItem *> & items() const;
    const KKSRubric * rubric(int index) const;
    const KKSList<const KKSRubric*> & rubrics() const;
    const KKSRubric * rubricForId(int id, bool recursivelly = true) const;
    const KKSRubricItem * itemForId(int id) const;

    KKSRubricItem * itemForId(int id);
    KKSRubric * rubricForId(int id, bool recursivelly = true);
    KKSRubric * rubric(int index);

    static void setIcon(const QPixmap & px);
    static QPixmap icon();

    KKSRubric * deepCopy(bool dropIds = false) const;

    KKSSearchTemplate * getSearchTemplate (void) const;
    void setSearchTemplate (KKSSearchTemplate * st);

    //
    // работа с категориями
    //
    KKSCategory * getCategory (void) const;
    void setCategory (KKSCategory *c);

    //
    // работа с дискреционными правами доступа
    //
    KKSAccessEntity * getAccessRules (void) const;
    void setAccessRules (KKSAccessEntity * _acl);

    bool isCategorized (void) const;
    void setCategorized (bool c);

    virtual void setDefaultIcon (const QPixmap& px);
    virtual QPixmap getDefaultIcon (void) const;
    virtual int rubricType (void) const;

private:

    KKSList<const KKSRubricItem *> m_items;
    KKSList<const KKSRubric *> m_rubrics;
    KKSList<const KKSRubric *> m_deletedRubrics;
    KKSSearchTemplate * m_searchTemplate;
    KKSCategory * m_category;

    bool m_isUpdated;
    
    //QIcon m_rubricIcon;
    //QString m_iconData;
    
    bool m_isCategorized;

    friend class KKSLoader;
    friend class KKSPPFactory;
    friend class KKSEIOFactory;
    //
    //дискреционные права доступа
    //
    //исключения (применяются всегда последними, поэтому если есть 
    //рассогласование между правами начальника, группы и соответствующими
    //исключениями, если таковые имеются, то применятся права, 
    //заданные для соответствующих исключений)
    //права для boss, unit специально не применяются
    //приложение должно отслеживать, что если пользователь изменил право
    //на boss или unit, то должны автоматически измениться права 
    //для соответствующих ролей в исключениях
    //Специально применяется только служебная роль OTHERS_ROLE
    KKSAccessEntity * m_acl;

    int m_intId;//служебное поле, используется для определения, 
              //является ли данная рубрика новой или она уже существует в БД
              // и осуществляется ее обновление
              //Данное поле будет  > 0, если рубрика загружена из БД

    QString getFullTreeOfIdsString() const;//возвращает список идентификаторов вложенных подрубрик (включая данную рурику) в виде id1, id2, id3
    QString getFullTreeOfDeletedIds() const;//возвращает список удаленных подрубрик
};

Q_DECLARE_METATYPE(const KKSRubricBase *);
Q_DECLARE_METATYPE(KKSRubricBase *);

#endif

