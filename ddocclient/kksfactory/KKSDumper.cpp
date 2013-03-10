/***********************************************************************
 * Module:  KKSDumper.cpp
 * Author:  gakonya
 * Modified: 13 декабря 2008 г. 12:59:59
 * Purpose: Implementation of the class KKSDumper
 ***********************************************************************/

#include "KKSDumper.h"

#include <KKSObject.h>
#include <KKSCategory.h>
#include <KKSObjectExemplar.h>
#include <KKSType.h>

#include "KKSEIOData.h"
#include "KKSList.h"
#include "KKSAttrValue.h"
#include "KKSAttrType.h"
#include "KKSValue.h"
#include "KKSLifeCycle.h"
#include "KKSState.h"
#include "KKSTemplate.h"
#include "KKSAttrGroup.h"
#include "KKSAttrView.h"

#include "defines.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSDumper::KKSDumper()
// Purpose:    Implementation of KKSDumper::KKSDumper()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSDumper::KKSDumper()
{
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSDumper::~KKSDumper()
// Purpose:    Implementation of KKSDumper::~KKSDumper()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSDumper::~KKSDumper()
{
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSDumper::dump(const KKSObject* io)
// Purpose:    Implementation of KKSDumper::dump()
// Parameters:
// - io
// Return:     int
////////////////////////////////////////////////////////////////////////

QString KKSDumper::dump(const KKSObject* io, const QString & prefix)
{
    QString str;
    if(!io){
        str += prefix + "NULL pointer\n";
        return str;
    }

    str += prefix + QString("**** KKSObject object ****\n");
    str += prefix + "==== Main parameters ====\n";
    str += prefix + QString("ID = %1\n").arg(io->id());
    str += prefix + QString("NAME = %1\n").arg(io->name());
//	str += prefix + QString("CODE = %1\n").arg(io->code());
    str += prefix + QString("DESCRIPTION = %1\n").arg(io->desc());
    str += prefix + QString("AUTHOR = %1\n").arg(io->author());
    str += prefix + QString("INFO = %1\n").arg(io->info());
    str += prefix + QString("INSERT_TIME = %1\n").arg(io->insertTime().toString());
    str += prefix + QString("IS_SYSTEM = %1\n").arg(io->isSystem());
    str += prefix + QString("TABLE_NAME = %1\n").arg(io->tableName());
    str += prefix + QString("STATE:\n");
    str += dump(io->state(), prefix+"\t");
    str += prefix + QString("CATEGORY:\n");
    str += dump(io->category(), prefix + "\t");

    str += prefix + QString("ATTR VALUES:\n");
    str += prefix + QString("COUNT = %1\n").arg(io->attrValues().count());
    KKSList<KKSAttrValue *> avList = io->attrValues();
    for (int i=0; i<avList.count(); i++)
    {
        KKSAttrValue * av = avList[i];
        str += prefix + QString("Attr_value %1:\n").arg(i);
        str += dump(av, prefix+"\t");
    }

    return str;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSDumper::dump(const KKSCategory* c)
// Purpose:    Implementation of KKSDumper::dump()
// Parameters:
// - c
// Return:     int
////////////////////////////////////////////////////////////////////////

QString KKSDumper::dump(const KKSCategory* c, const QString & prefix)
{
    QString str;
    if(!c){
        str += prefix + "NULL pointer\n";
        //qWarning("%s", str.toLocal8Bit().constData());
        return str;
    }
    str += prefix + "**** KKSCategory object ****\n";
    str += prefix + "==== Main parameters ====\n";
    str += prefix + QString("ID = %1\n").arg(c->id());
    str += prefix + QString("TYPE:\n");
    str += prefix + QString("\tID = %1\n").arg(c->type()->id());
    str += prefix + QString("\tNAME = %1\n").arg(c->type()->name());
    str += prefix + QString("\tCODE = %1\n").arg(c->type()->code());
    str += prefix + QString("\tDESCRIPTION = %1\n").arg(c->type()->desc());
    str += prefix + QString("IS_MAIN = ==NOT IMPLEMENTED==\n");
    str += prefix + QString("NAME = %1\n").arg(c->name());
    str += prefix + QString("DESCRIPTION = %1\n").arg(c->desc());
    str += "\n";
    str += prefix + "==== Attributes ====\n";
    str += prefix + QString("COUNT = %1\n").arg(c->attributes().count());
    int i=0;
    KKSMap<int, KKSCategoryAttr*>::const_iterator pca;
    for (pca = c->attributes().constBegin(); pca != c->attributes().constEnd(); pca++)
    {
        KKSCategoryAttr * attr = pca.value();
        str += prefix + QString("Attribute %1:\n").arg(i);
        str += dump(attr, prefix+"\t");
        i++;
    }
    //str += "\n";
    //str += prefix + "==== Life Cycle ====\n";
    //str += dump(c->lifeCycle(), prefix + "\t");
    
    str += "\n";
    str += prefix + "====Child category====\n";
    if(c->tableCategory()){
        str += dump(c->tableCategory(), prefix+"\t");
    }
    else{
        str += prefix + "Has no child category\n";
    }
    
    return str;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSDumper::dump(const KKSObjectExemplar* eio)
// Purpose:    Implementation of KKSDumper::dump()
// Parameters:
// - eio
// Return:     int
////////////////////////////////////////////////////////////////////////

QString KKSDumper::dump(const KKSObjectExemplar* eio, const QString & prefix)
{
    QString str;
    if(!eio){
        str += prefix + "NULL pointer\n";
        return str;
    }

    str += prefix + "**** KKSObjectExemplar object ****\n";
    str += prefix + "==== Main parameters ====\n";
    str += prefix + QString("ID = %1\n").arg(eio->id());
    
    str += dump(eio->io(), prefix + "\t");
    
    str += prefix + QString("Attributes\n");
    KKSList<KKSAttrValue *> avList = eio->attrValues();
    str += prefix + QString("COUNT = %1\n").arg(avList.count());
    for (int i=0; i< avList.count(); i++)
    {
        KKSAttrValue * av = avList[i];
        str += prefix + QString("Attribute %1:\n").arg(i);
        str += dump(av, prefix + "\t");
    }

    return str;
}

QString KKSDumper::dump(const KKSValue & v, const QString & prefix)
{
    QString str;

    if(v.isNull()){
        str += prefix + "IS NULL\n";
        return str;
    }

    if(!v.isValid()){
        str += prefix + "WARNING! Value is NOT valid!\n";
    }
    
    str += prefix + QString("TYPE = %1\n").arg(v.type());
    str += prefix + QString("VALUE = %1\n").arg(v.value());
    str += prefix + QString("IS_LITERAL = %1\n").arg(v.isLiteral());
    str += prefix + QString("COLUMN_VALUE = %1\n").arg(v.columnValue());

    return str;
}

QString KKSDumper::dump(const KKSAttrValue * av, const QString & prefix)
{
    QString str;

    if(!av)
    {
        str += prefix + "NULL pointer!\n";
        return str;
    }
    
    str += prefix + "Attribute:\n";
    str += dump(av->attribute(), prefix + "\t");

    str += prefix + "Value:\n";
    str += dump(av->value(), prefix + "\t");

    return str;
}

QString KKSDumper::dump(const KKSCategoryAttr * attr, const QString & prefix)
{
    QString str;

    if(!attr)
    {
        str += prefix + "NULL pointer!\n";
        return str;
    }

    //str += prefix + "Attribute:\n";
    str += prefix + QString("ID: %1\n").arg(attr->id());
    str += prefix + QString("NAME: %1\n").arg(attr->name());
    str += prefix + QString("CODE: %1\n").arg(attr->code());
    
    str += prefix + "TYPE:\n";
    str += prefix + QString("\tID = %1\n").arg(attr->type()->id());
    str += prefix + QString("\tNAME = %1\n").arg(attr->type()->name());
    str += prefix + QString("\tCODE = %1\n").arg(attr->type()->code());
    
    str += prefix + QString("TITLE: %1\n").arg(attr->title());
    str += prefix + QString("TABLE_NAME: %1\n").arg(attr->tableName());
    str += prefix + QString("COLUMN_NAME: %1\n").arg(attr->columnName());
    str += prefix + QString("DEF_WIDTH: %1\n").arg(attr->defWidth());
    
    str += prefix + QString("IS_MANDATORY: %1\n").arg(attr->isMandatory());
    str += prefix + QString("IS_READ_ONLY: %1\n").arg(attr->isReadOnly());

    str += prefix + QString("DEF_VALUE:\n");
    str += dump(attr->defValue(), prefix+"\t");

    return str;
}
/*
QString KKSDumper::dump(const KKSLifeCycle * lc, const QString & prefix)
{
    QString str;

    if(!lc)
    {
        str += prefix + "NULL pointer!\n";
        return str;
    }

    return str;
}
*/
QString KKSDumper::dump(const KKSState * s, const QString & prefix)
{
    QString str;

    if(!s)
    {
        str += prefix + "NULL pointer!\n";
        return str;
    }

    return str;
}

QString KKSDumper::dump(const KKSEIOData * eio, const QString & prefix)
{
    QString str;

    if(!eio)
    {
        str += prefix + "NULL pointer!\n";
        return str;
    }

    str += prefix + QString("**** KKSEIOData object ****\n");
    
    str += prefix;
    QMap<QString, QString>::const_iterator p;
    for (p=eio->fields().constBegin(); p!=eio->fields().constEnd(); p++)
    {
        QString Val = p.value();
        str += Val;
        str += "\t|";
    }

    return str;
}

QString KKSDumper::dump(const KKSTemplate * t, const QString & prefix)
{
    QString str;
    if(!t){
        str += prefix + "NULL pointer\n";
        //qWarning("%s", str.toLocal8Bit().constData());
        return str;
    }
    str += prefix + "**** KKSTemplate object ****\n";
    str += prefix + "==== Main parameters ====\n";
    str += prefix + QString("ID = %1\n").arg(t->id());
    str += prefix + QString("NAME = %1\n").arg(t->name());
    str += prefix + QString("CODE = %1\n").arg(t->code());
    str += prefix + QString("DESCRIPTION = %1\n").arg(t->desc());
    str += "\n";
    str += prefix + "==== Category ====\n";
    str += dump(t->category(), prefix +"\t");
    str += "\n";
    str += prefix + "==== Groups ====\n";
    str += prefix + QString("COUNT = %1\n").arg(t->groups().count());
    for (int i=0; i<t->groups().count(); i++)
    {
        const KKSAttrGroup * g = t->group(i);// t->groups()[i];
        str += prefix + QString("Group %1:\n").arg(i);
        str += dump(g, prefix+"\t");
    }
    
    return str;
}

QString KKSDumper::dump(const KKSAttrGroup * g, const QString & prefix)
{
    QString str;
    if(!g){
        str += prefix + "NULL pointer\n";
        return str;
    }

    str += prefix + QString("ID = %1\n").arg(g->id());
    str += prefix + QString("NAME = %1\n").arg(g->name());
    str += prefix + QString("ORDER = %1\n").arg(g->order());
    str += "\n";
    str += prefix + "==== Attributes ====\n";
    str += prefix + QString("COUNT = %1\n").arg(g->attrViews().count());
    int i=0;
    KKSMap<int, KKSAttrView *>::const_iterator pa;
    for (pa=g->attrViews().constBegin(); pa != g->attrViews().constEnd(); pa++)
    {
        KKSAttrView * av = pa.value();
        str += prefix + QString("AttrView %1:\n").arg(i);
        str += dump(av, prefix+"\t");
        i++;
    }
    
    str += "\n";
    str += prefix + "==== Parent ====\n";
    if(g->parent()){
        str += dump((KKSAttrGroup *)(g->parent()), prefix + "\t");
    }
    else{
        str += prefix + "Has no parent!\n";
    }
    
    return str;
}

QString KKSDumper::dump(const KKSAttrView * av, const QString & prefix)
{
    QString str;

    if(!av)
    {
        str += prefix + "NULL pointer!\n";
        return str;
    }

    //str += prefix + "Attribute:\n";
    str += prefix + QString("ID: %1\n").arg(av->id());
    str += prefix + QString("NAME: %1\n").arg(av->name());
    str += prefix + QString("CODE: %1\n").arg(av->code());
    
    str += prefix + "TYPE:\n";
    str += prefix + QString("\tID = %1\n").arg(av->type()->id());
    str += prefix + QString("\tNAME = %1\n").arg(av->type()->name());
    str += prefix + QString("\tCODE = %1\n").arg(av->type()->code());
    
    str += prefix + QString("TITLE: %1\n").arg(av->title());
    str += prefix + QString("TABLE_NAME: %1\n").arg(av->tableName());
    str += prefix + QString("COLUMN_NAME: %1\n").arg(av->columnName());
    str += prefix + QString("DEF_WIDTH: %1\n").arg(av->defWidth());
    
    str += prefix + QString("Values in KKSCategoryAttr:\n");
    str += prefix + QString("IS_MANDATORY: %1\n").arg(av->KKSCategoryAttr::isMandatory());
    str += prefix + QString("IS_READ_ONLY: %1\n").arg(av->KKSCategoryAttr::isReadOnly());

    str += prefix + QString("DEF_VALUE:\n");
    str += dump(av->KKSCategoryAttr::defValue(), prefix+"\t");

    str += prefix + QString("Values in KKSAttrView:\n");
    str += prefix + QString("ORDER: %1\n").arg(av->order());    
    str += prefix + QString("IS_READ_ONLY: %1\n").arg(av->isReadOnly());

    str += prefix + QString("DEF_VALUE:\n");
    str += dump(av->defValue(), prefix+"\t");

    return str;
}
