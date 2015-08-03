/***********************************************************************
 * Module:  KKSFile.h
 * Author:  ksa
 * Modified: 3 февраля 2009 г. 16:44:11
 * Purpose: Declaration of the class KKSFile
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSFile_h)
#define __KKSSITOOOM_KKSFile_h

#include "kkspp_config.h"

#include <KKSRecord.h>
#include <KKSList.h>

class QProcess;

class _PP_EXPORT KKSFileExtention : public KKSRecord
{
public:
    KKSFileExtention(int id, const QString & name);
    KKSFileExtention();
    KKSFileExtention(const KKSFileExtention & other);
    ~KKSFileExtention();

};

class _PP_EXPORT KKSFileType : public KKSRecord
{
public:

    KKSFileType(int id, const QString & name);
    KKSFileType(int id, 
                const QString & name, 
                const QString & appWin,
                const QString & appLin);
    
    KKSFileType(const KKSFileType& other);
    KKSFileType();

    ~KKSFileType();

    void setWinApp(const QString & app);
    const QString & winApp() const;

    void setLinApp(const QString & app);
    const QString & linApp() const;

    static KKSFileType defType();
    static KKSFileType defGISType();

    void addExtention(KKSFileExtention * e);
    void setExtentions(const KKSList<KKSFileExtention*> & el);
    void addExtentions(const KKSList<KKSFileExtention*> & el);
    void removeExtention(int index);

    const KKSList<KKSFileExtention*> & extentions() const;
    KKSFileExtention * extention(int index) const;

    bool assotiated(const QString & extName) const;

private:
    friend class KKSFile;
    const QString & app() const;
    QString m_appWin;
    QString m_appLin;
    KKSList<KKSFileExtention*> m_extentions;
};

class _PP_EXPORT KKSFile : public KKSRecord
{
    friend class KKSPPFactory;
    friend class KKSLoader;
    friend class KKSEIOFactory;

public:
    KKSFile(int id, const QString & name);
    KKSFile(int id, const QString & name, const QString & localUrl);
    KKSFile(const KKSFile& other);

    ~KKSFile();
    
    bool open();

    void setType(const KKSFileType & type);
    const KKSFileType & type() const;

    void setLocalUrl(const QString & url);
    const QString & localUrl() const;

    void setSrcExt(const QString & srcExt);
    const QString & srcExt() const;

    bool loaded() const;
    bool uploaded() const;


protected:
    void setLoaded(bool l = true);
    void setUploaded(bool u = true);
private:

    QString m_localUrl;
    QString m_srcExt;

    mutable bool m_bLoaded;
    mutable bool m_bUploaded;

    KKSFileType m_type;

    QProcess * proc;

};

#endif
