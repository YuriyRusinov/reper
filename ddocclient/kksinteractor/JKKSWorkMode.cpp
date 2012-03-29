#include <QDataStream>
#include "JKKSWorkMode.h"

JKKSWorkMode :: JKKSWorkMode (int id, const QString& wm_name, const QString& desc, const QString& symbol, const JKKSWorkModeType& type, const QString& uid)
    : JKKSUID (uid),
    idWM (id),
    wmName (wm_name),
    wmDesc (desc),
    wmSymbol (symbol),
    wmType (type)
{
}

JKKSWorkMode :: JKKSWorkMode (const JKKSWorkMode& WM)
    : JKKSUID (WM),
    idWM (WM.idWM),
    wmName (WM.wmName),
    wmDesc (WM.wmDesc),
    wmSymbol (WM.wmSymbol),
    wmType (WM.wmType)
{
}

JKKSWorkMode :: ~JKKSWorkMode (void)
{
}

int JKKSWorkMode :: id (void) const
{
    return idWM;
}

void JKKSWorkMode :: setId (int _id)
{
    idWM = _id;
}

const QString& JKKSWorkMode :: getName (void) const
{
    return wmName;
}

void JKKSWorkMode :: setName (const QString& name)
{
    wmName = name;
}

const QString& JKKSWorkMode :: getDesc (void) const
{
    return wmDesc;
}

void JKKSWorkMode :: setDesc (const QString& desc)
{
    wmDesc = desc;
}

const QString& JKKSWorkMode :: getSymbol (void) const
{
    return wmSymbol;
}

void JKKSWorkMode :: setSymbol (const QString& symbol)
{
    wmSymbol = symbol;
}

const JKKSWorkModeType& JKKSWorkMode :: getType (void) const
{
    return wmType;
}

void JKKSWorkMode :: setType (const JKKSWorkModeType& type)
{
    wmType = type;
}

QDataStream& operator<< (QDataStream& out, const JKKSWorkMode& wm)
{
    out << wm.idWM;
    out << wm.wmName;
    out << wm.wmDesc;
    out << wm.wmSymbol;
    out << wm.wmType;
    out << JKKSUID (wm).uid();

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSWorkMode& wm)
{
    in >> wm.idWM;
    in >> wm.wmName;
    in >> wm.wmDesc;
    in >> wm.wmSymbol;
    in >> wm.wmType;
    QString uid;
    in >> uid;
    wm.setUid (uid);

    return in;
}
