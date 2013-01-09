#include "KKSFilter.h"
#include "KKSSearchTemplate.h"
#include "KKSSearchTemplateType.h"
#include "defines.h"

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
