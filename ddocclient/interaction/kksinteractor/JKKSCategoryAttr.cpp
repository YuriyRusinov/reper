#include <QTextStream>
#include <QDataStream>

#include "JKKSCategoryAttr.h"

JKKSCategoryAttr :: JKKSCategoryAttr (qint64 idAttr, 
                                      qint64 idAttrType, 
                                      const QString& aCode, 
                                      const QString& aName, 
                                      const QString& aTitle, 
                                      const QString& aTable, 
                                      const QString& aTableUid,
                                      const QString& aColumn, 
                                      int aDefWidth, 
                                      const QString& aDefVal, 
                                      bool isMandatory, 
                                      bool isReadOnly,
                                      const QString & uid)
    : JKKSUID(uid, QString()),
    m_idAttribute (idAttr),
    m_idAttrType (idAttrType),
    m_aCode (aCode),
    m_aName (aName),
    m_aTitle (aTitle),
    m_aTable (aTable),
    m_aTableUid (aTableUid),
    m_aColumn (aColumn),
    m_aDefWidth (aDefWidth),
    m_aDefValue (aDefVal),
    m_isMandatory (isMandatory),
    m_isReadOnly (isReadOnly),
    m_transferrable(true)
{
}

JKKSCategoryAttr :: JKKSCategoryAttr (const JKKSCategoryAttr& cAttr)
    : JKKSUID(cAttr),//.m_uid),
    m_idAttribute (cAttr.m_idAttribute),
    m_idAttrType (cAttr.m_idAttrType),
    m_aCode (cAttr.m_aCode),
    m_aName (cAttr.m_aName),
    m_aTitle (cAttr.m_aTitle),
    m_aTable (cAttr.m_aTable),
    m_aTableUid (cAttr.m_aTableUid),
    m_aColumn (cAttr.m_aColumn),
    m_aDefWidth (cAttr.m_aDefWidth),
    m_aDefValue (cAttr.m_aDefValue),
    m_isMandatory (cAttr.m_isMandatory),
    m_isReadOnly (cAttr.m_isReadOnly),
    m_transferrable(cAttr.m_transferrable),
    m_attrs(cAttr.m_attrs),
    m_attrAttrUid(cAttr.m_attrAttrUid)
{
}

JKKSCategoryAttr :: ~JKKSCategoryAttr (void)
{
}

QDataStream& operator<< (QDataStream& out, const JKKSCategoryAttr& attr)
{
    out << attr.m_idAttribute;
    out << attr.m_idAttrType;
    out << attr.m_aCode;
    out << attr.m_aName;
    out << attr.m_aTitle;
    out << attr.m_aTableUid;
    out << attr.m_aTable;
    out << attr.m_aColumn;
    out << attr.m_aDefWidth;
    out << attr.m_aDefValue;
    out << attr.m_isMandatory;
    out << attr.m_isReadOnly;
    out << attr.uid();
    out << attr.attrs();
    out << attr.m_transferrable;
    out << attr.m_attrAttrUid;

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSCategoryAttr& attr)
{
    in >> attr.m_idAttribute;
    in >> attr.m_idAttrType;
    in >> attr.m_aCode;
    in >> attr.m_aName;
    in >> attr.m_aTitle;
    in >> attr.m_aTableUid;
    in >> attr.m_aTable;
    in >> attr.m_aColumn;
    in >> attr.m_aDefWidth;
    in >> attr.m_aDefValue;
    in >> attr.m_isMandatory;
    in >> attr.m_isReadOnly;
    QString uid;
    in >> uid;
    attr.setUid (uid);
    in >> attr.m_attrs;
    in >> attr.m_transferrable;
    in >> attr.m_attrAttrUid;

    return in;
}


qint64 JKKSCategoryAttr :: id (void) const
{
    return m_idAttribute;
}

void JKKSCategoryAttr :: setId (qint64 id)
{
    m_idAttribute = id;
}

qint64 JKKSCategoryAttr :: idAttrType (void) const
{
    return m_idAttrType;
}

void JKKSCategoryAttr :: setIdAttrType (qint64 id)
{
    m_idAttrType = id;
}

const QString & JKKSCategoryAttr :: code (void) const
{
    return m_aCode;
}

void JKKSCategoryAttr :: setCode (const QString& nCode)
{
    m_aCode = nCode;
}

const QString & JKKSCategoryAttr :: name (void) const
{
    return m_aName;
}

void JKKSCategoryAttr :: setName (const QString& nName)
{
    m_aName = nName;
}

const QString & JKKSCategoryAttr :: title (void) const
{
    return m_aTitle;
}

void JKKSCategoryAttr :: setTitle (const QString& nTitle)
{
    m_aTitle = nTitle;
}

const QString & JKKSCategoryAttr :: table (void) const
{
    return m_aTable;
}

void JKKSCategoryAttr :: setTable (const QString& nTable)
{
    m_aTable = nTable;
}

const QString& JKKSCategoryAttr :: getTableUID (void) const
{
    return m_aTableUid;
}

void JKKSCategoryAttr :: setTableUid (const QString& table_uid)
{
    m_aTableUid= table_uid;
}

const QString & JKKSCategoryAttr :: column (void) const
{
    return m_aColumn;
}

void JKKSCategoryAttr :: setColumn (const QString& col)
{
    m_aColumn = col;
}

int JKKSCategoryAttr :: defWidth (void) const
{
    return m_aDefWidth;
}

void JKKSCategoryAttr :: setDefWidth (int w)
{
    m_aDefWidth = w;
}

const QString & JKKSCategoryAttr :: defValue (void) const
{
    return m_aDefValue;
}

void JKKSCategoryAttr :: setDefValue (const QString& val)
{
    m_aDefValue = val;
}

bool JKKSCategoryAttr :: isMandatory (void) const
{
    return m_isMandatory;
}

void JKKSCategoryAttr :: setMandatory (bool v)
{
    m_isMandatory = v;
}

bool JKKSCategoryAttr :: isReadOnly (void) const
{
    return m_isReadOnly;
}

void JKKSCategoryAttr :: setReadOnly (bool v)
{
    m_isReadOnly = v;
}

void JKKSCategoryAttr::setAttrs(const QMap<qint64, JKKSCategoryAttr> & attrs)
{
    m_attrs = attrs;
}

const QMap<qint64, JKKSCategoryAttr> & JKKSCategoryAttr::attrs() const
{
    return m_attrs;
}

QMap<qint64, JKKSCategoryAttr> & JKKSCategoryAttr::attrs()
{
    return m_attrs;
}

bool JKKSCategoryAttr :: transferrable (void) const
{
    return m_transferrable;
}

void JKKSCategoryAttr :: setTransferrable (bool v)
{
    m_transferrable = v;
}

QTextStream& JKKSCategoryAttr :: loadFromBuffer (QTextStream& f, QString& str)
{
    f >> str;
    if (str.startsWith ("'") && !str.endsWith ("'"))
    {
        QString fstr;
        while (!fstr.endsWith ("'"))
        {
            f >> fstr;
            str += QString (" %1").arg (fstr);
        }
    }
    str = str.mid (1, str.length()-2);
    return f;
}

bool JKKSCategoryAttr :: operator< (const JKKSCategoryAttr& A) const
{
    return m_aCode < A.m_aCode;
}

const QString & JKKSCategoryAttr :: attrAttrUid() const
{
    return m_attrAttrUid;
}

void JKKSCategoryAttr :: setAttrAttrUid(const QString & uid)
{
    m_attrAttrUid = uid;
}
