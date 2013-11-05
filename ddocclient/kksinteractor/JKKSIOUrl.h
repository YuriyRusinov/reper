/***********************************************************************
 * Module: JKKSIOUrl.h
 * Author: yuriyrusinov
 * Modified: 03 сентября 2009 г. 14:36
 * Purpose: Declaration of the class JKKSIOUrl
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSURL_H)
#define __KKSSITOOOM_JKKSURL_H

#include "kksinteractor_config.h"
#include <QString>
#include "jkksuid.h"
#include "JKKSMessage.h"

class _I_EXPORT JKKSFilePart : public JKKSMessage, public JKKSUID
{
public:
    JKKSFilePart();
    JKKSFilePart(const JKKSFilePart & part);
    JKKSFilePart(QString uid, bool isLast, bool isFirst);
    ~JKKSFilePart();

    void setData(const QByteArray & data);
    const QByteArray & getData() const;

    //void setUid(const QString & uid);
    //const QString & getUid() const;

    void setAbsUrl(const QString & url);
    const QString & getAbsUrl() const;

    void setIdUrl(qint64 idUrl);
    qint64 getIdUrl() const;

    qint64 id() const;//id in out_sync_queue
    void setId(qint64 _id);

    void setIsLast(bool isLast = true);
    bool isLast() const;

    void setIsFirst(bool isFirst = true);
    bool isFirst() const;

    const JKKSAddress & getSenderAddr (void) const;
    void setSenderAddr (const JKKSAddress & addr);

    const QString & receiverUID() const;
    const QString & senderUID() const;
    void setReceiverUID(const QString & uid);
    void setSenderUID(const QString & uid);


    virtual QByteArray serialize (void) const;
    virtual int unserialize (const QByteArray& mess);

    virtual int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID);


private:
    friend QDataStream& operator<< (QDataStream& out, const JKKSFilePart& part);
    friend QDataStream& operator>> (QDataStream& in, JKKSFilePart& part);

    QByteArray m_data;
    //QString m_uid;
    JKKSAddress m_senderAddr;
    qint64 m_idUrl;
    bool m_isLast;//обозначает, что содержатся данные, представляющие собой самый конец файла (последняя из передаваемых частей)
    bool m_isFirst;//обозначает, что содержатся данные, представляющие собой самое начало файла (первая из передаваемых частей)
    QString m_absUrl;
    qint64 m_idQueue;

    QString m_receiverUID;
    QString m_senderUID;
};

class _I_EXPORT JKKSIOUrl : public JKKSUID
{
public:
    JKKSIOUrl (qint64 id=-1, 
               qint64 id_url=-1, 
               const QString& name1=QString(), 
               const QString& name2=QString(), 
               int type=-1, 
               const QString& Src_ext=QString(), 
               int l=0, 
               const char * d=0);
    JKKSIOUrl (const JKKSIOUrl& url);
    ~JKKSIOUrl (void);

    qint64 getIOId (void) const;
    void setIOId (qint64 id);

    qint64 getURLId (void) const;
    void setURLId (qint64 idUrl);

    const QString& getIOURL (void) const;
    void setIOURL (const QString& io_url);

    const QString& getURL (void) const;
    void setURL (const QString& url);

    int getType (void) const;
    void setType (int type);

    const QString& getSrcExt (void) const;
    void setSrcExt (const QString& s);

    int getDataLength (void) const;
    void setDataLength (int l);

    const QByteArray& getData (void) const;
    void setData (const QByteArray& d);

private:
    //
    // Functions
    //
    friend QDataStream& operator<< (QDataStream& out, const JKKSIOUrl& URL);
    friend QDataStream& operator>> (QDataStream& in, JKKSIOUrl& URL);

private:
    qint64 idObject;
    qint64 idURL;
    QString IO_URL;
    QString URL;
    int urlType;
    QString src_ext;
    int length;

    QByteArray URLdata;
};

#endif
