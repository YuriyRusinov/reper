/***********************************************************************
 * Module: JKKSIOUrl.h
 * Author: yuriyrusinov
 * Modified: 03 �������� 2009 �. 14:36
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
    JKKSFilePart(QString uid, bool isLast = false);
    ~JKKSFilePart();

    void setData(const QByteArray & data);
    const QByteArray & getData() const;

    //void setUid(const QString & uid);
    //const QString & getUid() const;

    void setAbsUrl(const QString & url);
    const QString & getAbsUrl() const;

    void setIdUrl(int idUrl);
    int getIdUrl() const;

    int id() const;//id in out_sync_queue
    void setId(int _id);

    void setIsLast(bool isLast = true);
    bool isLast() const;

    const JKKSAddress & getSenderAddr (void) const;
    void setSenderAddr (const JKKSAddress & addr);

    virtual QByteArray serialize (void) const;
    virtual int unserialize (const QByteArray& mess);

    virtual int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID);


private:
    friend QDataStream& operator<< (QDataStream& out, const JKKSFilePart& part);
    friend QDataStream& operator>> (QDataStream& in, JKKSFilePart& part);

    QByteArray m_data;
    //QString m_uid;
    JKKSAddress m_senderAddr;
    int m_idUrl;
    bool m_isLast;
    QString m_absUrl;
    qint64 m_idQueue;
};

class _I_EXPORT JKKSIOUrl : public JKKSUID
{
public:
    JKKSIOUrl (int id=-1, 
               int id_url=-1, 
               const QString& name1=QString(), 
               const QString& name2=QString(), 
               int type=-1, 
               const QString& Src_ext=QString(), 
               int l=0, 
               const char * d=0);
    JKKSIOUrl (const JKKSIOUrl& url);
    ~JKKSIOUrl (void);

    int getIOId (void) const;
    void setIOId (int id);

    int getURLId (void) const;
    void setURLId (int idUrl);

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
    int idObject;
    int idURL;
    QString IO_URL;
    QString URL;
    int urlType;
    QString src_ext;
    int length;

    QByteArray URLdata;
};

#endif
