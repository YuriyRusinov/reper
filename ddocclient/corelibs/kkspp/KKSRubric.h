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
class KKSObject;

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
    void setIcon (const QIcon& icon);
    const QString& iconAsString() const;
    void setIcon (const QString & s);
    
    enum RubricBaseType
    {
        btRootRubric = 0, //виртуальная корневая рубрика
        btRubric = 1, //рубрика для ИО или общесистемного рубрикатора
        btRubricItem = 2, //элемент рубрики (ИО или ЭИО)
        btRubricAsCategory = 3, //категория в виде рубрики, отображенная в дереве рубрикатора
        btOthers = 4, //запись в рубрикаторе "Others"
        btRecordRubric = 5 //рубрика в записи справочника
    };
    
    virtual int rubricType (void) const=0;
    
    virtual int childNumber (void) const;

    void addNode (const KKSRubricBase * node);
    void insertNode (int i, const KKSRubricBase * node);
    void clearNodes (void);
    
    const KKSList<const KKSRubricBase *>& subnodes (void) const;
    KKSList<const KKSRubricBase *>& subnodes (void);
    void setNodes (const KKSList<const KKSRubricBase *>& nodes);

    void setInitialized(bool yes = true);
    bool isInitialized() const;

protected:
    
    QIcon m_rubrIcon;
    QString m_iconData;

    bool m_isChanged;
    bool m_isInitialized; //загружена ли рубрика или ее вложение полностью из БД. 
                          //По умолчанию предполагаем, что да. 
                          //Если рубрика не инициализирована, то считаем, что ее не надо сохранять в БД. Т.к. скорее всего в ней содержится не полная информация.
                          //В случае визуализации рубрикатора через loadEIOList() необходимо выставить это поле в false
                          //соответственно далее, если в визуализаторе рубрикатора рубрика выбрана, то необходимо выставить это поле в true (после загрузки данных в из БД в рубрику)

private:
    KKSList<const KKSRubricBase *> m_subNodes;
    
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

};

class _PP_EXPORT KKSRubricItem : public KKSRubricBase//public KKSData
{
public:

    KKSRubricItem();
    KKSRubricItem(qint64 idItem, const QString & name, bool b = false, const QString& rIconStr=QString());
    KKSRubricItem(const KKSRubricItem & other);
    ~KKSRubricItem();
    
    KKSRubricItem& operator= (const KKSRubricItem& other);
    
/*    void setId(int id);
    int id() const;

    void setName(const QString & name);
    const QString & name() const;
*/
    static void setGeneralIcon(const QPixmap & px);
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
    KKSRubric(qint64 id, const QString & name, KKSSearchTemplate * st=0, KKSCategory * c=0, KKSAccessEntity * ac=0);
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
    const KKSList<const KKSRubricItem *> & deletedItems() const;//список удаленных из рубрики итемов


    const KKSRubric * rubric(int index) const;
    const KKSList<const KKSRubric*> & rubrics() const;
    const KKSRubric * rubricForId(qint64 id, bool recursivelly = true) const;
    const KKSRubricItem * itemForId(qint64 id) const;

    KKSRubricItem * itemForId(qint64 id);
    KKSRubric * rubricForId(qint64 id, bool recursivelly = true);
    KKSRubric * rubric(int index);

    static void setGeneralIcon(const QPixmap & px);
    static QPixmap icon();

    KKSRubric * deepCopy(bool dropIds = false) const;

    //поисковый запрос на рубрику (только для рубрик в общесистемном рубрикаторе, в информационных объектах)
    KKSSearchTemplate * getSearchTemplate (void) const;
    void setSearchTemplate (KKSSearchTemplate * st);

    //информационный объект (справочник) на рубрику (только для рубрик в записях справочников)
    KKSObject * getIO(void) const;
    void setIO(KKSObject * io);

    //категория на рубрику (в итоге только записи справочников данной категории и информационные объекты данной категории попадают в рубрику)
    KKSCategory * getCategory (void) const;
    void setCategory (KKSCategory *c);

    //  дискреционные права доступа к рубрике (только для рубрик в общесистемном рубрикаторе, в информационных объектах)
    KKSAccessEntity * getAccessRules (void) const;
    void setAccessRules (KKSAccessEntity * _acl);

    //категория в дереве рубрик
    void setCategorized();
    //рубрика относится к записи справочника
    void setForRecord();
    //рубрика относится к ИО
    void  setForIO();

    virtual void setDefaultIcon (const QPixmap& px);
    virtual QPixmap getDefaultIcon (void) const;
    
    virtual int rubricType (void) const;

private:

    KKSList<const KKSRubricItem *> m_items;
    KKSList<const KKSRubricItem *> m_deletedItems;

    KKSList<const KKSRubric *> m_rubrics;
    KKSList<const KKSRubric *> m_deletedRubrics;
    
    KKSSearchTemplate * m_searchTemplate;
    KKSCategory * m_category;
    KKSObject * m_io;

    bool m_isUpdated;
        
    RubricBaseType m_baseType;//тип рубрики. Здесь возможны 3 варианта btRubric, btRubricAsCategory, btRecordRubric

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

    qint64 m_intId;//служебное поле, используется для определения, 
              //является ли данная рубрика новой или она уже существует в БД
              // и осуществляется ее обновление
              //Данное поле будет  > 0, если рубрика загружена из БД

    QString getFullTreeOfIdsString() const;//возвращает список идентификаторов вложенных подрубрик (включая данную рурику) в виде id1, id2, id3
    QString getFullTreeOfDeletedIds() const;//возвращает список удаленных подрубрик

};

Q_DECLARE_METATYPE(const KKSRubricBase *);
Q_DECLARE_METATYPE(KKSRubricBase *);

#endif

