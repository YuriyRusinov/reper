/***********************************************************************
 * Module:  JKKSRefRecord.h
 * Author:  yuriyrusinov
 * Modified: 17 июн€ 2010 г. 17:30
 * Purpose: Declaration of the class JKKSRefRecord
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSRefRecord_h)
#define __KKSSITOOOM_JKKSRefRecord_h

#include "kksinteractor_config.h"
#include "jkksuid.h"
#include "JKKSCategory.h"
#include "JKKSMessage.h"
#include "JKKSDocument.h"
#include "JKKSIOTable.h"

#include <QString>
#include <QStringList>
#include <QByteArray>

class QDataStream;

class JKKSLoader;
/*
 * Ёто класс записей справочников дл€ передачи по транспорту
 */

class _I_EXPORT JKKSRefRecord : public JKKSMessage, public JKKSUID
{
public:
    JKKSRefRecord (qint64 id_queue=-1, 
                   qint64 id_rec=-1, 
                   int id_entity_type=-1, 
                   const QString& tName=QString(), 
                   int sync_type=-1, 
                   const JKKSCategory& aCat=JKKSCategory(), 
                   const QStringList& attrsVals=QStringList(), 

                   const JKKSAddress & addr = JKKSAddress (),
                   const QString& mess_code=QString(), 

                   const QString & t_uid = QString(), 
                   const QString & uid = QString(),
                   const QString & uuid = QString(),
                   qint64 idState = 1, 
                   const QColor bkCol = QColor(),
                   const QColor fgCol = QColor(),
                   const QString & rIcon = QString());
    
    JKKSRefRecord (const JKKSRefRecord& RR);
    
    ~JKKSRefRecord (void);

    //
    // virtual functions definition
    //
    QByteArray serialize (void) const;
    int unserialize (const QByteArray& mess);
    int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID);
    qint64 id (void) const;
    JKKSMessageType getMessageType (void) const;

    void setId (qint64 id);

    qint64 getIDQueue (void) const;
    void setIDQueue (qint64 idq);

    int getEntityType (void) const;
    void setEntityType (int id_entity_type);

    QString getTableName (void) const;
    void setTableName (const QString& tName);

    QString getTableUID (void) const;
    void setTableUID (const QString& t_uid);

    int getSyncType (void) const;
    void setSyncType (int sync_type);

    const QMap<qint64, JKKSCategory>& getCategory (void) const;
    void setCategory (const QMap<qint64, JKKSCategory>& aCats);

    const QStringList& attrsValues (void) const;
    void setAttrsValues (const QStringList& attrs_vals);

    const QMap<QString, QString> & indValues(void) const;
    void setIndValues (const QMap<QString, QString> & ind_vals);

    const JKKSDocument& getIODoc (void) const;
    void setIODoc (const JKKSDocument& doc);

    const JKKSIOTable& getAddTable (void) const;
    void setAddTable (const JKKSIOTable& table);

    const QMap<qint64, JKKSGlobalRubric>& rubrics (void) const;
    QMap<qint64, JKKSGlobalRubric>& rubrics (void);
    void setRubrics (const QMap<qint64, JKKSGlobalRubric>& rubrs);

    const QMap<qint64, JKKSIOUrl>& urls (void) const;
    QMap<qint64, JKKSIOUrl>& urls (void);
    void setUrls (const QMap<qint64, JKKSIOUrl>& urls);

private:
    //
    // Functions
    //
    friend QDataStream& operator<< (QDataStream& out, const JKKSRefRecord& RR);
    friend QDataStream& operator>> (QDataStream& in, JKKSRefRecord& RR);

private:
    //
    // Variables
    //
    qint64 idQueue;
    qint64 idRec;
    int idEntityType;
    QString tableName;
    QString tableUID;//фактически это unique_id информационного объекта, содержащего Ё»ќ
    int syncType;

    QMap<qint64, JKKSCategory> cat;
    QStringList aVals;//список значений табличных атрибутов. ≈сли атрибут ссылочный, то дл€ такого атрибута в качестве значени€ используетс€ его unique_id
    QMap<QString, QString> indVals; //список значений показателей записей справочников. ≈сли показатель ссылочный, то дл€ него в качестве значени€ используетс€ его unique_id

    QMap<qint64, JKKSGlobalRubric> m_rubrics; //рубрики
    QMap<qint64, JKKSIOUrl> m_urls;//прикрепленные файлы

    JKKSDocument ioDoc;
    JKKSIOTable ioTable;
};

class _I_EXPORT JKKSQueueResponse : public JKKSMessage 
{
public:
    JKKSQueueResponse (qint64 local_id=-1, 
                       qint64 id_queue=-1, 
                       int sync_result=-1, 
                       const JKKSAddress & addr = JKKSAddress(), 
                       const QString& type=QString());
    JKKSQueueResponse (const JKKSQueueResponse& QR);
    ~JKKSQueueResponse (void);

    //
    // virtual functions definition
    //
    QByteArray serialize (void) const;
    int unserialize (const QByteArray& mess);
    int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID);
    qint64 id (void) const;
    JKKSMessageType getMessageType (void) const;

    void setOrgUid(const QString & orgUid);
    const QString & orgUid() const;

    void setId (qint64 id);

    int getResult (void) const;
    void setResult (int sRes);

    qint64 getExternalId (void) const;
    void setExternalId (qint64 idq);

private:
    //
    // Functions
    //
    friend QDataStream& operator<< (QDataStream& out, const JKKSQueueResponse& RR);
    friend QDataStream& operator>> (QDataStream& in, JKKSQueueResponse& RR);
    
private:
    //
    // Variables
    //
    qint64 localId;
    qint64 idQueue;
    int syncResult;
    QString m_orgUid;
};

#endif
