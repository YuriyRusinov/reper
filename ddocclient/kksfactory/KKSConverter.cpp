/***********************************************************************
 * Module:  KKSConverter.cpp
 * Author:  sergey
 * Modified: 16 декабря 2008 г. 12:09:29
 * Purpose: Implementation of the class KKSConverter
 ***********************************************************************/

#include "KKSConverter.h"

#include "KKSObjectExemplar.h"
#include "defines.h"
#include "KKSLoader.h"
#include "KKSObject.h"
#include "KKSCategory.h"
#include "KKSAttribute.h"
#include "KKSType.h"
#include "KKSSyncType.h"
#include "KKSTemplate.h"
#include "KKSAttrValue.h"
#include "KKSAttrType.h"
#include "KKSState.h"
#include "KKSSearchTemplate.h"
#include "KKSRubric.h"
#include "KKSLifeCycle.h"
#include <KKSEIOData.h>



////////////////////////////////////////////////////////////////////////
// Name:       KKSConverter::KKSConverter()
// Purpose:    Implementation of KKSConverter::KKSConverter()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSConverter::KKSConverter()
{
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSConverter::~KKSConverter()
// Purpose:    Implementation of KKSConverter::~KKSConverter()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSConverter::~KKSConverter()
{
   // TODO : implement
}

KKSObjectExemplar * KKSConverter::attributeToExemplar(const KKSLoader * loader, const KKSAttribute * a)
{
    KKSObjectExemplar * eio = NULL;
    if(!a || !loader || !a->type())
        return eio;

    //загружаем информационный объект, который описывает справочник атрибутов
    KKSObject * io_io = loader->loadIO(IO_ATTR_ID, true);
    if(!io_io || !io_io->category() ||!io_io->category()->tableCategory())
        return eio;

    KKSCategory * tc = io_io->category()->tableCategory();

    eio = new KKSObjectExemplar();
    
    eio->setIo(io_io);
    io_io->release();
    
    KKSList<KKSAttrValue *> attrValues;
    
    //ID
    appendAttrValue(tc, attrValues, 1, QString::number(a->id()), KKSAttrType::atInt);
    //id_a_type
    appendAttrValue(tc, attrValues, 5, QString::number(a->type()->id()), KKSAttrType::atInt);//id_a_type
    //code
    appendAttrValue(tc, attrValues, 6, a->code(), KKSAttrType::atString);//code
    //name
    appendAttrValue(tc, attrValues, 2, a->name(), KKSAttrType::atString);//name
    //title
    appendAttrValue(tc, attrValues, 7, a->title(), KKSAttrType::atString);//title
    //table_name
    appendAttrValue(tc, attrValues, 8, a->tableName(), KKSAttrType::atString);//table_name
    //column_name
    appendAttrValue(tc, attrValues, 9, a->columnName(), KKSAttrType::atString);//column_name
    //def_width
    appendAttrValue(tc, attrValues, 57, QString::number(a->defWidth()), KKSAttrType::atInt);//def_width
    //is_system
    appendAttrValue(tc, attrValues, 15,  a->isSystem() ? "true" : "false", KKSAttrType::atBool);//is_system
    //id_attr_group
    if(a->group())
        appendAttrValue(tc, attrValues, 284,  QString::number(a->group()->id()), KKSAttrType::atInt);//id_attr_group
    else
        appendAttrValue(tc, attrValues, 284,  QString::number(2), KKSAttrType::atInt);//"others attributes" group

    //id_search_template
    if(a->searchTemplate())
        appendAttrValue(tc, attrValues, 110, QString::number(a->searchTemplate()->id()), KKSAttrType::atInt);//id_search_template
    else
        appendAttrValue(tc, attrValues, 110, QString::null, KKSAttrType::atInt);
    
    //ref_column_name
    appendAttrValue(tc, attrValues, 29, a->refColumnName(), KKSAttrType::atString);//ref_column_name

    eio->setAttrValues(attrValues);

    eio->setId(a->id());
    //eio->setDesc(a->desc());
    eio->setName(a->name());
    eio->setCode(a->code());

    return eio;
}

KKSObjectExemplar * KKSConverter::stateToExemplar(const KKSLoader * loader, const KKSState * s)
{
    KKSObjectExemplar * eio = NULL;
    if(!s || !loader)
        return eio;

    //загружаем информационный объект, который описывает справочник состояний ИО
    KKSObject * io_io = loader->loadIO(IO_STATE_ID, true);
    if(!io_io || !io_io->category() ||!io_io->category()->tableCategory())
        return eio;

    KKSCategory * tc = io_io->category()->tableCategory();

    eio = new KKSObjectExemplar();
    
    eio->setIo(io_io);
    io_io->release();
    
    KKSList<KKSAttrValue *> attrValues;
    
    //ID
    appendAttrValue(tc, attrValues, 1, QString::number(s->id()), KKSAttrType::atInt);
    //name
    appendAttrValue(tc, attrValues, 2, s->name(), KKSAttrType::atString);
    //description
    appendAttrValue(tc, attrValues, 3, s->desc(), KKSAttrType::atString);
    //is_system
    appendAttrValue(tc, attrValues, 15, s->isSystem() ? "TRUE" : "FALSE", KKSAttrType::atBool);

    eio->setAttrValues(attrValues);

    eio->setId(s->id());
    eio->setDesc(s->desc());
    eio->setName(s->name());

    return eio;
}

KKSObjectExemplar * KKSConverter::typeToExemplar(const KKSLoader * loader, const KKSType * t)
{
    KKSObjectExemplar * eio = NULL;
    if(!t || !loader)
        return eio;

    //загружаем информационный объект, который описывает справочник типов категорий ИО
    KKSObject * io_io = loader->loadIO(IO_CAT_TYPE_ID, true);
    if(!io_io || !io_io->category() ||!io_io->category()->tableCategory())
        return eio;

    KKSCategory * tc = io_io->category()->tableCategory();

    eio = new KKSObjectExemplar();
    
    eio->setIo(io_io);
    io_io->release();
    
    KKSList<KKSAttrValue *> attrValues;
    
    //ID
    appendAttrValue(tc, attrValues, 1, QString::number(t->id()), KKSAttrType::atInt);
    //name
    appendAttrValue(tc, attrValues, 2, t->name(), KKSAttrType::atString);
    //r_name
    appendAttrValue(tc, attrValues, 187, t->rName(), KKSAttrType::atString);
    //description
    appendAttrValue(tc, attrValues, 3, t->desc(), KKSAttrType::atString);
    //is_qualifier
    appendAttrValue(tc, attrValues, 310, t->isQualifier() ? "TRUE" : "FALSE", KKSAttrType::atBool);

    eio->setAttrValues(attrValues);

    eio->setId(t->id());
    eio->setDesc(t->desc());
    eio->setName(t->name());

    return eio;
}

KKSObjectExemplar * KKSConverter::ioTypeToExemplar(const KKSLoader * loader, const KKSType * t)
{
    KKSObjectExemplar * eio = NULL;
    if(!t || !loader)
        return eio;

    //загружаем информационный объект, который описывает справочник типов ИО
    KKSObject * io_io = loader->loadIO(IO_TYPE_ID, true);
    if(!io_io || !io_io->category() ||!io_io->category()->tableCategory())
        return eio;

    KKSCategory * tc = io_io->category()->tableCategory();

    eio = new KKSObjectExemplar();
    
    eio->setIo(io_io);
    io_io->release();
    
    KKSList<KKSAttrValue *> attrValues;
    
    //ID
    appendAttrValue(tc, attrValues, 1, QString::number(t->id()), KKSAttrType::atInt);
    //name
    appendAttrValue(tc, attrValues, 2, t->name(), KKSAttrType::atString);
    //description
    appendAttrValue(tc, attrValues, 3, t->desc(), KKSAttrType::atString);

    eio->setAttrValues(attrValues);

    eio->setId(t->id());
    eio->setDesc(t->desc());
    eio->setName(t->name());

    return eio;
}

KKSObjectExemplar * KKSConverter::syncTypeToExemplar(const KKSLoader * loader, const KKSSyncType * t)
{
    KKSObjectExemplar * eio = NULL;
    if(!t || !loader)
        return eio;

    //загружаем информационный объект, который описывает справочник типов ИО
    KKSObject * io_io = loader->loadIO(IO_SYNCTYPE_ID, true);
    if(!io_io || !io_io->category() ||!io_io->category()->tableCategory())
        return eio;

    KKSCategory * tc = io_io->category()->tableCategory();

    eio = new KKSObjectExemplar();
    
    eio->setIo(io_io);
    io_io->release();
    
    KKSList<KKSAttrValue *> attrValues;
    
    //ID
    appendAttrValue(tc, attrValues, 1, QString::number(t->id()), KKSAttrType::atInt);
    //name
    appendAttrValue(tc, attrValues, 2, t->name(), KKSAttrType::atString);
    //description
    appendAttrValue(tc, attrValues, 3, t->desc(), KKSAttrType::atString);

    eio->setAttrValues(attrValues);

    eio->setId(t->id());
    eio->setDesc(t->desc());
    eio->setName(t->name());

    return eio;
}

KKSObjectExemplar * KKSConverter::categoryToExemplar(const KKSLoader * loader, const KKSCategory * c)
{
    KKSObjectExemplar * eio = NULL;
    if(!c || !loader)
        return eio;

    //загружаем информационный объект, который описывает справочник категорий
    KKSObject * io_io = loader->loadIO(IO_CAT_ID, true);
    if(!io_io || !io_io->category() ||!io_io->category()->tableCategory())
        return eio;

    KKSCategory * tc = io_io->category()->tableCategory();

    eio = new KKSObjectExemplar();
    
    eio->setIo(io_io);
    io_io->release();
    
    KKSList<KKSAttrValue *> attrValues;
    
    //ID
    appendAttrValue(tc, attrValues, 1, QString::number(c->id()), KKSAttrType::atInt);
    //id_io_category_type
    appendAttrValue(tc, attrValues, 10, QString::number(c->type()->id()), KKSAttrType::atInt);//id_io_category_type
    
    //id_child
    if(c->tableCategory())
        appendAttrValue(tc, attrValues, 11, QString::number(c->tableCategory()->id()), KKSAttrType::atInt);//id_child
    else
        appendAttrValue(tc, attrValues, 11, QString::null, KKSAttrType::atInt);
    
    //id_child2
    if(c->recAttrCategory())
        appendAttrValue(tc, attrValues, 311, QString::number(c->recAttrCategory()->id()), KKSAttrType::atInt);//id_child2
    else
        appendAttrValue(tc, attrValues, 311, QString::null, KKSAttrType::atInt);

    //is_main
    appendAttrValue(tc, attrValues, 17, c->isMain() ? "true" : "false", KKSAttrType::atBool);//is_main
    //name
    appendAttrValue(tc, attrValues, 2, c->name(), KKSAttrType::atString);//name
    //code
    appendAttrValue(tc, attrValues, 6, c->code(), KKSAttrType::atString);//code
    //description
    appendAttrValue(tc, attrValues, 3, c->desc(), KKSAttrType::atString);
    //is_archived
    appendAttrValue(tc, attrValues, ATTR_IS_ARCHIVED,  "false", KKSAttrType::atBool);
    //io_objects_organization
    //appendAttrValue(tc, attrValues, "io_categories_organization",  c->replicatedOrgsV());
    //id_io_state
    appendAttrValue(tc, attrValues, 18, QString::number(c->state()->id()), KKSAttrType::atInt);//id_io_state
    //is_global
    appendAttrValue(tc, attrValues, 111,  c->isGlobal() ? "true" : "false", KKSAttrType::atBool);//is_global
    //id_life_cycle
    if(c->lifeCycle())
        appendAttrValue(tc, attrValues, 323, QString::number(c->lifeCycle()->id()), KKSAttrType::atInt);//id_life_cycle
    else
        appendAttrValue(tc, attrValues, 323, QString::null, KKSAttrType::atInt);//id_life_cycle

    eio->setAttrValues(attrValues);

    eio->setId(c->id());
    eio->setDesc(c->desc());
    eio->setName(c->name());
    eio->setCode(c->code());

    return eio;
}

KKSObjectExemplar * KKSConverter::objToExemplar(const KKSLoader * loader, const KKSObject * io)
{
    KKSObjectExemplar * eio = NULL;
    if(!io || !loader)
        return eio;

    //загружаем информационный объект, который описывает справочник информационных объектов
    KKSObject * io_io = loader->loadIO(IO_IO_ID, true);
    if(!io_io || !io_io->category() ||!io_io->category()->tableCategory())
        return eio;

    KKSCategory * tc = io_io->category()->tableCategory();

    eio = new KKSObjectExemplar();
    
    eio->setIo(io_io);
    io_io->release();
    
    KKSList<KKSAttrValue *> attrValues;
    
    //ID
    appendAttrValue(tc, attrValues, 1, QString::number(io->id()), KKSAttrType::atInt);
    //id_io_category
    appendAttrValue(tc, attrValues, 12, QString::number(io->category()->id()), KKSAttrType::atInt);//id_io_category
    //author
    appendAttrValue(tc, attrValues, 13, QString::number(io->author()), KKSAttrType::atInt);//author
    //id_io_state
    appendAttrValue(tc, attrValues, 18, QString::number(io->state()->id()), KKSAttrType::atInt);//id_io_state
    //id_io_type
    appendAttrValue(tc, attrValues, 43, QString::number(io->type()->id()), KKSAttrType::atInt);//id_io_type
    //id_maclabel
    appendAttrValue(tc, attrValues, 63, QString::number(io->idMaclabel()), KKSAttrType::atInt);//id_maclabel
    //name
    appendAttrValue(tc, attrValues, 2, io->name(), KKSAttrType::atString);
    //table_name
    appendAttrValue(tc, attrValues, 8, io->tableName(), KKSAttrType::atString);//table_name
    //description
    appendAttrValue(tc, attrValues, 3, io->desc(), KKSAttrType::atString);
    //information
    appendAttrValue(tc, attrValues, 14, io->info(), KKSAttrType::atString);
    //is_system
    appendAttrValue(tc, attrValues, 15, io->isSystem() ? "true" : "false", KKSAttrType::atBool);//is_system
    //insert_time
    appendAttrValue(tc, attrValues, 16, io->insertTime().toString("dd.MM.yyyy hh:mm:ss"), KKSAttrType::atDateTime);//insert_time
    
    //io_objects_organization
    //appendAttrValue(tc, attrValues, "io_objects_organization",  io->replicatedOrgsV());
    //организации всегда добавляются отдельно в методе KKSPPFActory::insertIO() и updateIO()
    //обрабатывать их в методе insertEIO и updateEIO не надо. 
    //Поэтому здесь всегда список синхронизируемых организаций игнорируется
    appendAttrValue(tc, attrValues, 97,  "{}", KKSAttrType::atCheckListEx);//io_objects_organization
    
    //is_global
    appendAttrValue(tc, attrValues, 111,  io->isGlobal() ? "true" : "false", KKSAttrType::atBool);//is_global
    //id_sync_type
    appendAttrValue(tc, attrValues, 112, QString::number(io->syncType()->id()), KKSAttrType::atInt);//id_sync_type
    //id_owner_org
    if(io->ownerOrg() > 0 )
        appendAttrValue(tc, attrValues, 117, QString::number(io->ownerOrg()), KKSAttrType::atInt);//id_owner_org
    else
        appendAttrValue(tc, attrValues, 117, QString::null, KKSAttrType::atInt);

    //record_fill_color
    if(io->recordFillColor().isValid())
        appendAttrValue(tc, attrValues, 206, QString::number(io->recordFillColor().rgba()), KKSAttrType::atRecordColor);//record_fill_color
    else
        appendAttrValue(tc, attrValues, 206, QString()/*::number(-1)*/, KKSAttrType::atRecordColor);
    //record_text_color
    if(io->recordTextColor().isValid())
        appendAttrValue(tc, attrValues, 207, QString::number(io->recordTextColor().rgba()), KKSAttrType::atRecordTextColor);//record_text_color
    else
        appendAttrValue(tc, attrValues, 207, QString()/*::number(-1)*/, KKSAttrType::atRecordTextColor);

    //id_search_template
    if(io->idSearchTemplate() > 0 )
        appendAttrValue(tc, attrValues, 110, QString::number(io->idSearchTemplate()), KKSAttrType::atInt);//id_search_template
    else
        appendAttrValue(tc, attrValues, 110, QString::null, KKSAttrType::atInt);

    //ref_table_name
    appendAttrValue(tc, attrValues, 216, io->refTableName(), KKSAttrType::atString);//ref_table_name

    //r_icon
    if(io->iconAsString().isEmpty())
        appendAttrValue(tc, attrValues, 312, QString::null/*::number(-1)*/, KKSAttrType::atJPG);
        
    else
        appendAttrValue(tc, attrValues, 312, io->iconAsString(), KKSAttrType::atJPG);//r_icon


    eio->setAttrValues(attrValues);

    eio->setId(io->id());
    eio->setDesc(io->desc());
    eio->setName(io->name());

    return eio;
}

KKSObjectExemplar * KKSConverter::templateToExemplar(const KKSLoader * loader, const KKSTemplate * t)
{
    KKSObjectExemplar * eio = NULL;
    if(!t || !loader || !t->category())
        return eio;

    //загружаем информационный объект, который описывает справочник категорий
    KKSObject * io_io = loader->loadIO(IO_TEMPLATE_ID, true);
    if(!io_io || !io_io->category() ||!io_io->category()->tableCategory())
        return eio;

    KKSCategory * tc = io_io->category()->tableCategory();

    eio = new KKSObjectExemplar();
    
    eio->setIo(io_io);
    io_io->release();
    
    KKSList<KKSAttrValue *> attrValues;
    
    //ID
    appendAttrValue(tc, attrValues, 1, QString::number(t->id()), KKSAttrType::atInt);
    //id_io_category
    appendAttrValue(tc, attrValues, 12, QString::number(t->category()->id()), KKSAttrType::atInt);//id_io_category
    //name
    appendAttrValue(tc, attrValues, 2, t->name(), KKSAttrType::atString);//name
    //code
    appendAttrValue(tc, attrValues, 6, t->code(), KKSAttrType::atString);//code
    //description
    appendAttrValue(tc, attrValues, 3, t->desc(), KKSAttrType::atString);//description

    eio->setAttrValues(attrValues);

    eio->setId(t->id());
    eio->setName(t->name());
    eio->setCode(t->code());
    eio->setDesc(t->desc());

    return eio;
}

int KKSConverter::appendAttrValue(     KKSCategory * tc, 
                                       KKSList<KKSAttrValue *> &attrValues, 
                                       int attrId, 
                                       const QString & attrValue, 
                                       KKSAttrType::KKSAttrTypes attrType)
{
    KKSValue v;
    v.setValue(attrValue, attrType);
    if(!v.isValid()){
        return ERROR_CODE;
    }

    KKSAttrValue * av = new KKSAttrValue();
    KKSCategoryAttr * attr = tc->attribute(attrId);
    if(!attr){
        av->release();
        return ERROR_CODE;
    }
    
    av->setAttribute(attr);
    av->setValue(v);
    
    attrValues.append(av);

    av->release();

    return OK_CODE;
}

int KKSConverter::appendAttrValue(     KKSCategory * tc, 
                                       KKSList<KKSAttrValue *> &attrValues, 
                                       int attrId, 
                                       const KKSValue & attrValue)
{
    KKSAttrValue * av = new KKSAttrValue();
    KKSCategoryAttr * attr = tc->attribute(attrId);
    if(!attr){
        av->release();
        return ERROR_CODE;
    }
    
    av->setAttribute(attr);
    av->setValue(attrValue);
    
    attrValues.append(av);

    av->release();

    return OK_CODE;
}

bool KKSConverter::objectFromExemplar(const KKSLoader * loader, KKSObject * io, const KKSObjectExemplar * eio)
{
    if(!io || !eio)
        return false;
       
    const KKSAttrValue * av = eio->attrValue(ATTR_ID_IO_CATEGORY);
    if(!av)
        return false;

    int idCategory = av->value().value().toInt();
    KKSCategory * c = loader->loadCategory(idCategory);
    if(!c)
        return false;

    io->setCategory(c);
    c->release();
    
    av = eio->attrValue(ATTR_AUTHOR);
    if(!av)
        return false;

    io->setAuthor(av->value().value().toInt());

    av = eio->attrValue(ATTR_ID_OWNER_ORG);
    if(!av)
        return false;

    io->setOwnerOrg(av->value().value().toInt());

    av = eio->attrValue(ATTR_ID_MACLABEL);
    if(!av)
        return false;

    io->setIdMaclabel(av->value().value().toInt());

    av = eio->attrValue(ATTR_ID_IO_STATE);
    if(!av)
        return false;
    
    KKSState * s = loader->loadState(av->value().value().toInt());
    if(!s)
        return false;
    
    io->setState(s);
    s->release();
    
    av = eio->attrValue(ATTR_ID_SYNC_TYPE);
    if(!av)
        return false;
    
    KKSSyncType * st = loader->loadSyncType(av->value().value().toInt());
    if(!st)
        return false;
    
    io->setSyncType(st);
    st->release();

    av = eio->attrValue(ATTR_ID_IO_TYPE);
    if(!av)
        return false;
    
    KKSType * t = loader->loadIOType(av->value().value().toInt());
    if(!t)
        return false;
    
    io->setType(t);
    t->release();

    av = eio->attrValue(ATTR_NAME);
    if(!av)
        return false;
    
    io->setName(av->value().value());

    av = eio->attrValue(ATTR_TABLE_NAME);
    if(!av)
        return false;
    
    io->setTableName(av->value().value());

    av = eio->attrValue(ATTR_DESCRIPTION);
    if(!av)
        return false;
    
    io->setDesc(av->value().value());

    av = eio->attrValue(ATTR_INFORMATION);
    if(!av)
        return false;
    
    io->setInfo(av->value().value());

    av = eio->attrValue(ATTR_IS_SYSTEM);
    if(!av)
        return false;
    
    bool sys = av->value().valueVariant().toBool();
    io->setIsSystem(sys);

    av = eio->attrValue(ATTR_IS_GLOBAL);
    if(!av)
        return false;
    
    bool gl = av->value().valueVariant().toBool();
    io->setIsGlobal(gl);

    av = eio->attrValue(ATTR_INSERT_TIME);
    if(!av)
        return false;
    
    QDateTime dt = av->value().valueVariant().toDateTime();
    if(!dt.isValid())
        return false;

    io->setInsertTime(dt);
    
    av = eio->attrValue(ATTR_IO_OBJECTS_ORGANIZATION);
    if(!av)
        return false;
    
    
    KKSValue v = av->value();//.valueVariant().toDateTime();
    if(!v.isValid())
        return false;

    io->setReplicatedOrgsV(v);

    av = eio->attrValue(ATTR_RECORD_FILL_COLOR);
    if(!av)
        return false;

    io->setRecordFillColor (av->value().valueVariant().value<QColor>());//av->value().value().toUInt());

    av = eio->attrValue(ATTR_RECORD_TEXT_COLOR);
    if(!av)
        return false;

    io->setRecordTextColor (av->value().valueVariant().value<QColor>());

    av = eio->attrValue(ATTR_R_ICON);
    if(!av)
        return false;
    io->setIcon(av->value().value());
    //
    // Серега неправ
    //(av->value().value().toUInt());
    //

    av = eio->attrValue(ATTR_ID_SEARCH_TEMPLATE);
    if(!av)
        return false;

    io->setIdSearchTemplate(av->value().value().toInt());

    av = eio->attrValue(ATTR_REF_TABLE_NAME);
    if(!av)
        return false;

    io->setRefTableName(av->value().value());

    return true;
}

KKSMap<qint64, KKSEIOData *> KKSConverter :: rubricEntityToData (const KKSLoader * loader, const KKSRubric * rubricB)
{
    KKSMap<qint64, KKSEIOData *> rubricData;
    KKSObject * refIO = loader->loadIO(IO_IO_ID);
    const KKSCategory * cat = /*rubricB->getCategory() && rubricB->getCategory()->attributes().count() > 0 ? rubricB->getCategory() :*/ refIO->category()->tableCategory();
    KKSObject * refRubr = loader->loadIO(IO_RUBR_ID);
    const KKSCategory * ct = refRubr->category();
    
    if (!ct || !ct->tableCategory())
    {
        refIO->release ();
        refRubr->release ();
        return rubricData;
    }
    ct = ct->tableCategory();
    KKSAttribute * aid = loader->loadAttribute (1);
    if (!aid)
    {
        refIO->release ();
        refRubr->release ();
        return rubricData;
    }
    QString sql;
    if (rubricB->rubricType () != KKSRubricBase::atRubricCategory)
        sql = QString ("select id_io_object from io_rubricator ior where ior.id_rubric=%1").arg (rubricB->id());
    else
        sql = QString ("select id from io_objects io where io.id_io_category=%1").arg (rubricB->id());
    KKSValue * val = new KKSValue (sql, KKSAttrType::atInt64);
    const KKSFilter * f = new KKSFilter (aid, val, KKSFilter::foInSQL);
    KKSFilterGroup * fg = new KKSFilterGroup (false);
    fg->addFilter (f);
    KKSList<const KKSFilterGroup *> filters;
    filters.append (fg);
    fg->release ();
    f->release ();
    rubricData = loader->loadEIOList (cat, refIO->tableName(), filters);
    qDebug () << __PRETTY_FUNCTION__ << rubricData.count();
    refRubr->release();
    refIO->release();

    return rubricData;
}
