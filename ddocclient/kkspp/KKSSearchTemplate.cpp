#include "KKSFilter.h"
#include "KKSSearchTemplate.h"
#include "defines.h"

KKSSearchTemplateType::KKSSearchTemplateType(int id, const QString & name, const QString & desc) 
: KKSRecord (id, name, desc), m_parent(NULL)
{

}

KKSSearchTemplateType :: KKSSearchTemplateType (const KKSSearchTemplateType & stt)
: KKSRecord (stt), m_parent(stt.m_parent)
{
    if(m_parent)
        m_parent->addRef();
}

KKSSearchTemplateType::~KKSSearchTemplateType (void)
{
    if(m_parent)
        m_parent->release();
}


const KKSSearchTemplateType * KKSSearchTemplateType::parent() const
{
    return m_parent;
}

KKSSearchTemplateType * KKSSearchTemplateType::parent()
{
    return m_parent;
}

void KKSSearchTemplateType::setParent(KKSSearchTemplateType * p)
{
    if(m_parent)
        m_parent->release();

    m_parent = p;

    if(m_parent)
        m_parent->addRef();
}

KKSSearchTemplateType * KKSSearchTemplateType::defaultType() 
{
    //KKSSearchTemplateType * t = new KKSSearchTemplateType(1, QObject::tr("Общие поисковые запросы")); //сделано в main.cpp QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    KKSSearchTemplateType * t = new KKSSearchTemplateType(-1, QObject::tr("child")); //сделано в main.cpp QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    KKSSearchTemplateType * t1 = new KKSSearchTemplateType(-1, QObject::tr("parent")); //сделано в main.cpp QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    t->setParent(t1);
    t1->release();
    return t;
}


/****************************************/

KKSSearchTemplate :: KKSSearchTemplate (int id, KKSFilterGroup * sg, const QString& name, int idAuth, const QString & desc, const QString & authName)
    : KKSRecord (id, name, desc),
    mainGroup (sg),
    author (idAuth),
    m_authorName(authName),
    m_idCategory(IO_TABLE_CATEGORY_ID),//
    m_type(KKSSearchTemplateType::defaultType())
{
    if (mainGroup)
        mainGroup->addRef ();

}

KKSSearchTemplate :: KKSSearchTemplate (const KKSSearchTemplate& st)
    : KKSRecord (st),
    mainGroup (st.mainGroup),
    author (st.author),
    m_authorName(st.m_authorName),
    m_idCategory(st.m_idCategory),
    m_cName(st.m_cName),
    m_type(st.m_type),
    m_creationDatetime(st.m_creationDatetime)
{
    if (mainGroup)
        mainGroup->addRef ();

    if(m_type)
        m_type->addRef();
}

KKSSearchTemplate :: ~KKSSearchTemplate (void)
{
    if (mainGroup)
        mainGroup->release ();

    if(m_type)
        m_type->release();
}

KKSFilterGroup *KKSSearchTemplate :: getMainGroup (void) const
{
    return mainGroup;
}

void KKSSearchTemplate :: setMainGroup (KKSFilterGroup *sg)
{
    if (mainGroup)
        mainGroup->release ();

    mainGroup = sg;

    if (mainGroup)
        mainGroup->addRef ();
}

int KKSSearchTemplate :: idAuthor (void) const
{
    return author;
}

void KKSSearchTemplate :: setAuthor (int id, const QString & name)
{
    author = id;
    m_authorName = name;
}

const QString & KKSSearchTemplate :: authorName() const
{
    return m_authorName;
}

int KKSSearchTemplate :: idCategory() const
{
    return m_idCategory;
}

const QString & KKSSearchTemplate :: categoryName() const
{
    return m_cName;
}

void KKSSearchTemplate :: setCategory(int id, const QString & name)
{
    m_idCategory = id;
    m_cName = name;
}

const KKSSearchTemplateType * KKSSearchTemplate :: type() const
{
    return m_type;
}

KKSSearchTemplateType * KKSSearchTemplate :: type()
{
    return m_type;
}

void KKSSearchTemplate :: setType(KKSSearchTemplateType * t)
{
    if(m_type)
        m_type->release();

    m_type = t;

    if(m_type)
        m_type->addRef();
}

const QDateTime & KKSSearchTemplate :: creationDatetime() const
{
    return m_creationDatetime;
}

void KKSSearchTemplate :: setCreationDatetime(const QDateTime & dt)
{
    m_creationDatetime = dt;
}
