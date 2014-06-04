#include "KKSCategory.h"
#include "KKSAddTable.h"

KKSAddTable :: KKSAddTable (int _idObject, 
                            int idt, 
                            const QString& table_name, 
                            const KKSCategory * c, 
                            const QString& ttitle,
                            int idSearchTemplate,
                            const QString & refTableName)
    : KKSRecord(),
    idObject (_idObject),
    idTable (idt),
    tableName (table_name),
    cat (c),
    title (ttitle),
    m_idSearchTemplate(idSearchTemplate),
    m_refTableName(refTableName)
{
    if (c)
        cat->addRef ();
}

KKSAddTable :: KKSAddTable (const KKSAddTable& t)
    : KKSRecord (t),
    idObject (t.idObject),
    idTable (t.idTable),
    tableName (t.tableName),
    cat (t.cat),
    title (t.title),
    m_idSearchTemplate(t.m_idSearchTemplate),
    m_refTableName(t.m_refTableName)
{
    if (cat)
        cat->addRef ();
}

KKSAddTable :: ~KKSAddTable (void)
{
    if (cat)
        cat->release ();
}

int KKSAddTable :: idIO (void) const
{
    return idObject;
}

int KKSAddTable :: id (void) const
{
    return idTable;
}

void KKSAddTable :: setId (int idt)
{
    idTable = idt;
}

QString KKSAddTable :: getTableName (void) const
{
    return tableName;
}

void KKSAddTable :: setTableName (const QString& table_name)
{
    tableName = table_name;
}

const KKSCategory * KKSAddTable :: category (void) const
{
    return cat;
}

void KKSAddTable :: setCategory (const KKSCategory * c)
{
    if (cat)
        cat->release ();

    cat = c;

    if (cat)
        cat->addRef ();
}

QString KKSAddTable :: getTitle (void)
{
    return title;
}

void KKSAddTable :: setTitle (const QString& ttitle)
{
    title = ttitle;
}

QString KKSAddTable :: getRefTableName (void) const
{
    return m_refTableName;
}

void KKSAddTable :: setRefTableName (const QString& ref_table_name)
{
    m_refTableName = ref_table_name;
}

int KKSAddTable :: idSearchTemplate (void) const
{
    return m_idSearchTemplate;
}

void KKSAddTable :: setIdSearchTemplate (int idSt)
{
    m_idSearchTemplate = idSt;
}
