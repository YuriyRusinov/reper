/***********************************************************************
 * Module:  KKSEIOFactory.h
 * Author:  sergey
 * Modified: 16 декабря 2008 г. 12:45:46
 * Purpose: Declaration of the class KKSEIOFactory
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSEIOFactory_h)
#define __KKSSITOOOM_KKSEIOFactory_h

#include "kksfactory_config.h"
#include "KKSMap.h"
#include "KKSList.h"
#include "KKSAttrValue.h"
#include "KKSCategoryAttr.h"

class KKSDatabase;
class KKSObjectExemplar;
class KKSCategory;

class QProgressDialog;

class _F_EXPORT KKSEIOFactory
{
public:

    int insertEIO(KKSObjectExemplar* eio, const KKSCategory* cat=0, const QString& table=QString()) const;
    int updateEIO(const KKSObjectExemplar* eio, const KKSCategory* cat=0, const QString& table=QString()) const;
    int deleteEIO(KKSObjectExemplar* eio, const QString& table=QString()) const;
    int deleteRecord(int id, const QString & table) const;
    int deleteAllRecords(const QString & table) const;
    int insertEIOList(KKSList<KKSObjectExemplar*> eioList,  const KKSCategory* cat=0, const QString & table=QString(), QProgressDialog *pgDial=0) const;

    int insertCommand(KKSObjectExemplar* eio) const;
    int insertMessage(KKSObjectExemplar* eio) const;
    int insertTSDRecord(KKSObjectExemplar* eio) const;
    

    void setDb(KKSDatabase * db);
    KKSDatabase * getDb() const;

protected:
private:
    friend class KKSSito;
    friend class KKSSitoWeb;
    friend class KKSPPFactory;
    KKSEIOFactory();
    ~KKSEIOFactory();
    
    KKSDatabase * db;

    int insertRecord(KKSObjectExemplar* eio, 
                     const KKSCategory* cat=0, 
                     const QString& table=QString()) const;
    int updateRecord(const KKSObjectExemplar* eio, 
                     const KKSCategory* cat=0, 
                     const QString& table=QString()) const;
    int deleteRecord(KKSObjectExemplar* eio, const QString& table=QString()) const;

    int generateInsertQuery(const QString & tableName, 
                            const KKSMap<int, KKSCategoryAttr *> & attrs, 
                            const KKSList<KKSAttrValue *> & attrValues, 
                            QString & query,
                            QString & exQuery) const;
    int generateUpdateQuery(const QString & tableName, 
                            const KKSMap<int, KKSCategoryAttr *> & attrs, 
                            const KKSList<KKSAttrValue *> & attrValues, 
                            int id,
                            QString & query,
                            QString & exQuery) const;
    int getNextSeq(QString tableName, QString idColumn = QString("id")) const;

    
};

#endif
