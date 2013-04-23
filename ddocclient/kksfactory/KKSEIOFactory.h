/***********************************************************************
 * Module:  KKSEIOFactory.h
 * Author:  sergey
 * Modified: 16 декабря 2008 г. 12:45:46
 * Purpose: Declaration of the class KKSEIOFactory
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSEIOFactory_h)
#define __KKSSITOOOM_KKSEIOFactory_h

#include <QHash>

#include "kksfactory_config.h"
#include "KKSMap.h"
#include "KKSList.h"
#include "KKSAttrValue.h"
#include "KKSCategoryAttr.h"

class KKSDatabase;
class KKSObjectExemplar;
class KKSCategory;
class KKSRubric;
class KKSFile;
class QWidget;
class KKSFileLoader;

class QProgressDialog;

class _F_EXPORT KKSEIOFactory
{
public:

    void setParams( 
                   KKSFileLoader * _fileLoader, 
                    
                   KKSDatabase * db);

    //bImported=true сигнализирует о том, что запись справочника импортируется из внешнего файла
    int insertEIO(KKSObjectExemplar* eio, 
                  const KKSCategory* cat=0, 
                  const QString& table=QString(), 
                  bool bImported = false,
                  QWidget * parent = NULL) const; 

    int updateEIO(const KKSObjectExemplar* eio, 
                  const KKSCategory* cat=0, 
                  const QString& table=QString(),
                  QWidget * parent = NULL) const;
    int deleteEIO(KKSObjectExemplar* eio, const QString& table=QString()) const;
    int deleteRecord(qint64 id, const QString & table) const;
    int deleteAllRecords(const QString & table) const;
    int insertEIOList(KKSList<KKSObjectExemplar*> eioList,
                      QHash<QString, qint64>& uids,
                      const KKSCategory* cat=0,
                      const QString & table=QString(),
                      QProgressDialog *pgDial=0,
                      bool bImported = false) const;

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
    KKSFileLoader * fileLoader;

    int insertRecord(KKSObjectExemplar* eio, 
                     const KKSCategory* cat=0, 
                     const QString& table=QString(), 
                     bool bImported = false,
                     QWidget * parent = NULL) const;
    int updateRecord(const KKSObjectExemplar* eio, 
                     const KKSCategory* cat=0, 
                     const QString& table=QString(),
                     QWidget * parent = NULL) const;
    int deleteRecord(KKSObjectExemplar* eio, const QString& table=QString()) const;

    qint64 generateInsertQuery(const QString & tableName, 
                            const KKSMap<int, KKSCategoryAttr *> & attrs, 
                            const KKSList<KKSAttrValue *> & attrValues, 
                            QString & query,
                            QString & exQuery, 
                            bool bImported = false) const;
    qint64 generateUpdateQuery(const QString & tableName, 
                            const KKSMap<int, KKSCategoryAttr *> & attrs, 
                            const KKSList<KKSAttrValue *> & attrValues, 
                            qint64 id,
                            QString & query,
                            QString & exQuery) const;
    qint64 getNextSeq(QString tableName, QString idColumn = QString("id")) const;

    int updateIndValues(const KKSObjectExemplar * eio) const;
    int insertIndValues(const KKSObjectExemplar * eio) const;

    int updateIncludes(const KKSObjectExemplar * eio) const;
    int insertIncludes(const KKSObjectExemplar * eio) const;
    int deleteIncludes(int idObject) const;
    int updateRubrics(KKSRubric * parent, int idMyDocsRubricator = -1) const;
    int updateRubric(KKSRubric * r) const;
    int insertRubrics(KKSRubric * parent, int idMyDocsRubricator = -1) const;
    int insertRubric(KKSRubric * r, int idParent, int idRec, bool root = false, int idMyDocsRubricator = -1) const;
    int insertRubricItem(int idRubric, int idRec, bool isAutomated) const;
    void rollbackRubrics(KKSRubric * r, bool forUpdate = false) const;
    void rollbackRubric(KKSRubric * r, bool forUpdate = false) const;
    void commitRubrics(KKSRubric * r) const;
    void commitRubric(KKSRubric * r) const;
    int insertRubricators(KKSRubric * rootRubric, int idMyDocsRubricator, bool bMyDocs = false) const;
    int deleteRubricators(bool bMyDocs = false) const;
    int deleteRubric(int idRubric) const;

    int insertFile(qint64 idRecord, KKSFile * f, QWidget * parent = NULL) const;
    int insertFiles(const KKSObjectExemplar * eio, QWidget * parent = NULL) const;
    int updateFiles(const KKSObjectExemplar * eio, QWidget * parent = NULL) const;
    int updateFileInfo(KKSFile * f) const;
    int uploadFile(KKSFile * f, QWidget * parent) const;

    
};

#endif
