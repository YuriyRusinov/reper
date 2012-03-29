
#include "KKSFile.h"

#include <QtCore/QProcess>

KKSFile::KKSFile(int id, const QString & name) : KKSRecord(id, name),
m_type(KKSFileType::defType())
{
    m_bLoaded = false;
    m_bUploaded = false;

    proc = NULL;
}

KKSFile::KKSFile(const KKSFile& other) : KKSRecord(other)
{
    m_bLoaded = other.loaded();
    m_bUploaded = other.uploaded();
    m_localUrl = other.localUrl();
    m_srcExt = other.srcExt();

    m_type = other.type();

    proc = NULL;
}

KKSFile::KKSFile(int id, const QString & name, const QString & localUrl) : KKSRecord(id, name),
m_type(KKSFileType::defType())
{
    setLocalUrl(localUrl);

    proc = NULL;
}

KKSFile::~KKSFile()
{
    if (proc && proc->state() == QProcess::Running) {
        proc->terminate();
        proc->waitForFinished(3000);
    }
    
    if(proc)
        delete proc;
}

const QString & KKSFile::localUrl() const
{
    return m_localUrl;
}

void KKSFile::setLocalUrl(const QString & url)
{
    m_localUrl = url;
    QString ext;

    m_bUploaded = false;

    if(m_localUrl.isEmpty()){
        m_bLoaded = false;
        return;
    }
    else
        m_bLoaded = true;

#ifdef WIN32
    m_localUrl.replace("/", "\\");
    ext = m_localUrl.section('\\', -1); 
#else
    ext = m_localUrl.section('/', -1); 
#endif    
    
    ext = ext.section('.', -1);
    setSrcExt(ext);
}

const QString & KKSFile::srcExt() const
{
    return m_srcExt;
}

void KKSFile::setSrcExt(const QString & ext)
{
    m_srcExt = ext;
}

bool KKSFile::loaded() const
{
    return m_bLoaded;
}

bool KKSFile::uploaded() const
{
    return m_bUploaded;
}

void KKSFile::setLoaded(bool l) 
{
    m_bLoaded = l;
}

void KKSFile::setUploaded(bool u) 
{
    m_bUploaded = u;
}

void KKSFile::setType(const KKSFileType & type)
{
    m_type = type;
}

const KKSFileType & KKSFile::type() const
{
    return m_type;
}


bool KKSFile::open()
{
    if (!proc)
        proc = new QProcess();

    if (proc->state() != QProcess::Running) {
        QString app = m_type.app();

        QStringList args;
        
        args << m_localUrl;

        proc->start(app, args);

        if (!proc->waitForStarted()) {
            return false;
        }    
    }

    return true;
}

/*================================*/
KKSFileType::KKSFileType(int id, const QString & name) : KKSRecord(id, name)
{

}

KKSFileType::KKSFileType(int id, 
                         const QString & name, 
                         const QString & appWin,
                         const QString & appLin) : KKSRecord(id, name)
{
    m_appLin = appLin;
    m_appWin = appWin;
}

KKSFileType::KKSFileType(const KKSFileType& other) : KKSRecord(other)
{
    m_appLin = other.linApp();
    m_appWin = other.winApp();
}

KKSFileType::KKSFileType() : KKSRecord(-1, "")
{
}

KKSFileType::~KKSFileType()
{

}

void KKSFileType::setWinApp(const QString & app)
{
    m_appWin = app;
}

const QString & KKSFileType::winApp() const
{
    return m_appWin;
}

void KKSFileType::setLinApp(const QString & app)
{
    m_appLin = app;
}

const QString & KKSFileType::linApp() const
{
    return m_appLin;
}

KKSFileType KKSFileType::defType()
{
    KKSFileType type;
    
    type.setId(1);
    type.setName(QObject::tr("Simple text editor"));
    type.setLinApp("elk-textedit");
    type.setWinApp("notepad");

    return type;
}

const QString & KKSFileType::app() const
{
#ifdef WIN32
    return m_appWin;
#else
    return m_appLin;
#endif
}

void KKSFileType::addExtention(KKSFileExtention * e)
{
    if(!e)
        return;

    m_extentions.append(e);
}

void KKSFileType::setExtentions(const KKSList<KKSFileExtention*> & el)
{
    m_extentions = el;
}

void KKSFileType::addExtentions(const KKSList<KKSFileExtention*> & el)
{
    for(int i=0; i<el.count(); i++){
        KKSFileExtention * e = el.at(i);
        if(e)
            m_extentions.append(e);
    }
}

void KKSFileType::removeExtention(int index)
{
    if(index >=0 && index <m_extentions.count())
        m_extentions.removeAt(index);
}

const KKSList<KKSFileExtention*> & KKSFileType::extentions() const
{
    return m_extentions;
}

KKSFileExtention * KKSFileType::extention(int index) const
{
    if(index >=0 && index <m_extentions.count())
        return m_extentions.at(index);

    return NULL;
}

bool KKSFileType::assotiated(const QString & extName) const
{
    bool bYes = false;
    
    for(int i=0; i<m_extentions.count(); i++){
        KKSFileExtention * e = m_extentions.at(i);
        if(e->name() == extName){
            bYes = true;
            break;
        }
    }

    return bYes;
}

/*========================*/

KKSFileExtention::KKSFileExtention(int id, const QString & name) : KKSRecord(id, name)
{

}

KKSFileExtention::KKSFileExtention() : KKSRecord()
{
    setId(1);
    setName("*");
}

KKSFileExtention::KKSFileExtention(const KKSFileExtention & other) : KKSRecord(other)
{

}

KKSFileExtention::~KKSFileExtention()
{

}
