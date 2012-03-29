/***********************************************************************
 * Module: JKKSIOUrl.h
 * Author: yuriyrusinov
 * Modified: 03 сент€бр€ 2009 г. 14:36
 * Purpose: Declaration of the class JKKSIOUrl
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSURL_H)
#define __KKSSITOOOM_JKKSURL_H

#include "kksinteractor_config.h"
#include <QString>

class _I_EXPORT JKKSIOUrl
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
