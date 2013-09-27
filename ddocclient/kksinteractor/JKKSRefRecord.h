/***********************************************************************
 * Module:  JKKSRefRecord.h
 * Author:  yuriyrusinov
 * Modified: 17 июня 2010 г. 17:30
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
 * Это класс записей справочников для передачи по транспорту
 */

class _I_EXPORT JKKSRefRecord : public JKKSMessage, public JKKSUID
{
public:
    JKKSRefRecord (int id_queue=-1, 
                   int id_rec=-1, 
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
                   int idState = 1, 
                   const QColor bkCol = QColor(),
                   const QColor fgCol = QColor(),
                   const QIcon rIcon = QIcon());
    
    JKKSRefRecord (const JKKSRefRecord& RR);
    
    ~JKKSRefRecord (void);

    //
    // virtual functions definition
    //
    QByteArray serialize (void) const;
    int unserialize (const QByteArray& mess);
    int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID);
    int id (void) const;
    JKKSMessageType getMessageType (void) const;

    void setId (int id);

    int getIDQueue (void) const;
    void setIDQueue (int idq);

    int getEntityType (void) const;
    void setEntityType (int id_entity_type);

    QString getTableName (void) const;
    void setTableName (const QString& tName);

    QString getTableUID (void) const;
    void setTableUID (const QString& t_uid);

    int getSyncType (void) const;
    void setSyncType (int sync_type);

    const QMap<int, JKKSCategory>& getCategory (void) const;
    void setCategory (const QMap<int, JKKSCategory>& aCats);

    const QStringList& attrsValues (void) const;
    void setAttrsValues (const QStringList& attrs_vals);

    const JKKSDocument& getIODoc (void) const;
    void setIODoc (const JKKSDocument& doc);

    const JKKSIOTable& getAddTable (void) const;
    void setAddTable (const JKKSIOTable& table);

    const JKKSAddress & getSenderAddr (void) const;
    void setSenderAddr (const JKKSAddress & addr);

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
    int idQueue;
    int idRec;
    int idEntityType;
    QString tableName;
    QString tableUID;
    int syncType;
    QMap<int, JKKSCategory> cat;
    QStringList aVals;

    JKKSDocument ioDoc;
    JKKSIOTable ioTable;
    JKKSAddress senderAddr;
};

class _I_EXPORT JKKSQueueResponse : public JKKSMessage 
{
public:
    JKKSQueueResponse (int local_id=-1, 
                       int id_queue=-1, 
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
    int id (void) const;
    JKKSMessageType getMessageType (void) const;

    void setId (int id);

    int getResult (void) const;
    void setResult (int sRes);

    int getExternalId (void) const;
    void setExternalId (int idq);

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
    int localId;
    int idQueue;
    int syncResult;
};

#endif
