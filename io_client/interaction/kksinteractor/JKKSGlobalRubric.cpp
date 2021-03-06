#include <QDataStream>
#include "JKKSGlobalRubric.h"

JKKSGlobalRubric :: JKKSGlobalRubric (qint64 id,
                                      const QString & uid,
                                      qint64 idp,
                                      qint64 idc,
                                      const QString& name,
                                      const QString& code,
                                      const QString& desc)
    : JKKSRubric (id, uid, idp, idc, name, code, desc),
    category (QMap<qint64, JKKSCategory>()),
    searchTemplate (JKKSSearchTemplate())
{
}

JKKSGlobalRubric :: JKKSGlobalRubric (const JKKSGlobalRubric& R)
    : JKKSRubric (R),
    category (R.category),
    searchTemplate (R.searchTemplate)
{
}

JKKSGlobalRubric :: ~JKKSGlobalRubric (void)
{
}

JKKSGlobalRubric& JKKSGlobalRubric :: operator= (const JKKSGlobalRubric& R)
{
    this->JKKSRubric::operator=(R);
    category = R.category;
    searchTemplate = R.searchTemplate;
    return *this;
}

const QMap<qint64, JKKSCategory>& JKKSGlobalRubric :: getCategory (void) const
{
    return category;
}

QMap<qint64, JKKSCategory>& JKKSGlobalRubric :: getCategory (void)
{
    return category;
}

void JKKSGlobalRubric :: setCategory (const QMap<qint64, JKKSCategory>& c)
{
    category = c;
}

const JKKSSearchTemplate& JKKSGlobalRubric :: getSearchTemplate (void) const
{
    return searchTemplate;
}

void JKKSGlobalRubric :: setSearchTemplate (const JKKSSearchTemplate& st)
{
    searchTemplate = st;
}

QDataStream& operator<< (QDataStream& out, const JKKSGlobalRubric& R)
{
    out << R.idRubric;
    out << R.idParent;
    out << R.idEntity;
    out << R.uidRubric;
    out << R.rubricName;
    out << R.rubricCode;
    out << R.rubricDesc;

    out << R.category;
    out << R.searchTemplate;

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSGlobalRubric& R)
{
    in >> R.idRubric;
    in >> R.idParent;
    in >> R.idEntity;
    in >> R.uidRubric;
    in >> R.rubricName;
    in >> R.rubricCode;
    in >> R.rubricDesc;

    in >> R.category;
    in >> R.searchTemplate;

    return in;
}
