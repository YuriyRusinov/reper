/***********************************************************************
 * Module:  KKSLoader.cpp
 * Author:  sergey
 * Modified: 16 декабря 2008 г. 12:33:15
 * Purpose: Implementation of the class KKSLoader
 ***********************************************************************/

#include <QtDebug>

#include "KKSLoader.h"
#include "KKSObjectExemplar.h"
#include "defines.h"
#include "kksdatabase.h"
#include "KKSObject.h"
#include "KKSEIOData.h"
#include "KKSCategory.h"
#include "KKSAttribute.h"
#include "KKSValue.h"
#include "KKSType.h"
#include "KKSSyncType.h"
#include "KKSAttrValue.h"
#include <KKSAttrView.h>
#include "KKSAttrType.h"
#include "KKSLifeCycle.h"
#include "KKSState.h"
#include "KKSDumper.h"
#include "KKSTemplate.h"
#include "KKSRubric.h"
#include "KKSSearchTemplate.h"
#include "KKSSearchTemplateType.h"
#include "KKSAclTemplate.h"
#include "KKSPrivilege.h"
#include <KKSAccessEntity.h>
#include "KKSIndicator.h"
#include "KKSIndicatorValue.h"


//////
#include "kksresult.h"////////
// Name:       KKSLoader::KKSLoader()
// Purpose:    Implementation of KKSLoader::KKSLoader()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSLoader::KKSLoader()
{
    db = NULL;
}

KKSLoader::~KKSLoader()
{
    clearCache(); 
}

void KKSLoader::clearCache()
{
    QList<KKSObject *> a = loadedObjects.values();
    for(int i=0; i<a.count(); i++){
        a.at(i)->release();
    }
    loadedObjects.clear();
}

void KKSLoader::setDb(KKSDatabase * _db)
{
    db = _db;
}

KKSDatabase * KKSLoader::getDb() const
{
    return db;
}

QMap<int, QString> KKSLoader::loadAttributeValues(const KKSAttribute * a,
                                                  QMap<int, QString> & refColumnValues,
                                                  bool isXml,
                                                  bool orderByValue,
                                                  QString tName,
                                                  const KKSList<const KKSFilterGroup*> filters) const
{
    QMap<int, QString> values;
    if(!a)
        return values;

    if(a->type()->attrType() != KKSAttrType::atList &&
        a->type()->attrType() != KKSAttrType::atParent &&
        a->type()->attrType() != KKSAttrType::atCheckList && 
        a->type()->attrType() != KKSAttrType::atCheckListEx &&
        a->type()->attrType() != KKSAttrType::atRecordColorRef &&
        a->type()->attrType() != KKSAttrType::atRecordTextColorRef)
    {
        qWarning("loadAttributeValues was invoked with KKSAttribute "
                 "that type is neither atList, atParent or atCheckList or atCheckListEx!");
        return values;
    }

    QString tableName = a->tableName();
    QString columnName = a->columnName();
    QString refColumnName = a->refColumnName();
    if(refColumnName.isEmpty())
        refColumnName = "id";


    if(a->type()->attrType() == KKSAttrType::atParent){
        tableName = tName;
    }

    if(tableName.isEmpty() || columnName.isEmpty()){
        qWarning("loadAttributeValues was invoked with corrupt KKSAttribute "
                 "(type is atList or atParent but tableName or columnName are empty)!");
        return values;
    }

    QString order;
    if(orderByValue)
        order = QString("order by \"%1\"").arg(columnName);

    KKSAttribute * refAttr = loadAttribute (columnName, tableName);
    if (!refAttr)
        return values;

    QString sql;
	//Т.е. проверяем ситуацию "ссылка на ссылку"
    if (refAttr->tableName().isEmpty()){
		sql = QString("select id, \"%1\", \"%2\" from %3 %4")
                        .arg(columnName)
                        .arg(refColumnName)
                        .arg(tableName)
                        .arg(order);
	}
    else
    {
        sql = QString ("select %4 %2.id, %2.\"%3\", %1.\"%5\" from %1 inner join %2 on (%1.%5 = %2.\"%3\")")
                    //.arg (refAttr->columnName())
                    .arg (refAttr->tableName())
                    .arg (tableName)
                    .arg (columnName)
                    .arg (isXml ? QString() : QString("distinct"))
                    .arg(refAttr->refColumnName());
                    //.arg (order);
        //qDebug () << __PRETTY_FUNCTION__ << sql;
    }

    KKSResult * res = db->execute(sql);
    if(!res)
        return values;

    int count = res->getRowCount();
    if(count == 0){
        delete res;
        return values;
    }

    refColumnValues.clear();

    for(int row=0; row<count; row++){
        int id = res->getCellAsInt(row, 0);
        QString value = res->getCellAsString(row, 1);
        values.insert(id, value);

        //if(columnName != refColumnName){
            QString ref = res->getCellAsString(row, 2);
            refColumnValues.insert(id, ref);
        //}
    }

    delete res;

    return values;
}

KKSList<KKSAttrValue *> KKSLoader::loadAttrValues(KKSObject * io) const
{
    KKSList<KKSAttrValue *> attrs;

    if(!io || !io->category())
        return attrs;
    
    QString sql = QString("select * from ioGetObjectAttrs(%1, true, NULL::timestamp, NULL::timestamp)").arg(io->id());
    KKSResult * res = db->execute(sql);
    if(!res)
        return attrs;

    int count = res->getRowCount();
    if(count == 0){
        //qWarning("The object does not contain any values of attributes");
        delete res;
        return attrs;
    }

    KKSCategory * c = io->category();

    for(int row=0; row<count; row++){
        //параметры самого атрибута
        int idAttr = res->getCellAsInt(row, 2);
        KKSCategoryAttr * a = c->attribute(idAttr);
        if(!a){
            qWarning("The object contain attr-value but category does not contain attribute! idAttr = %d, idObject = %d, idCategory = %d", idAttr, io->id(), c->id());
            continue;
        }

        KKSAttrValue * attr = new KKSAttrValue();
        attr->setAttribute(a);

        if (a->type()->attrType() == KKSAttrType::atPoints)
            qDebug () << __PRETTY_FUNCTION__ << a->id() << res->getCellAsString(row, 3);
        KKSValue v = constructValue(res->getCellAsString(row, 3), a, io->tableName());
        if(!v.isValid())
        {
            qWarning("Value for attribute is NOT valid! Value = %s, idCategory = %d, idAttribute = %d, idObject = %d",
                        res->getCellAsString(row, 3).toLocal8Bit().data(),
                        c->id(),
                        res->getCellAsInt(row, 2),
                        io->id());
        }

        attr->setValue(v);
        
        //устанавливаем дополнительные характеристики значения атрибута
        attr->setId(res->getCellAsInt(row, 7));
        attr->setStartDateTime(res->getCellAsDateTime(row, 8));
        attr->setStopDateTime(res->getCellAsDateTime(row, 9));
        //attr->setMeasDateTime(res->getCellAsDateTime(row, 10));
        attr->setInsertDateTime(res->getCellAsDateTime(row, 10));
        attr->setDesc(res->getCellAsString(row, 14));

        int idObjSrc = res->getCellAsInt(row, 11);
        if(idObjSrc > 0){
            KKSObject * o = loadIO(idObjSrc, true);//упрощенно
            attr->setIOSrc(o);
            if(o)
                o->release();
        }

        int idObjSrc1 = res->getCellAsInt(row, 12);
        if(idObjSrc1 > 0){
            KKSObject * o = loadIO(idObjSrc1, true);//упрощенно
            attr->setIOSrc1(o);
            if(o)
                o->release();
        }

        attr->setActual(res->getCellAsBool(row, 13));

        attrs.append(attr);
        attr->release();
    }
    
    delete res;

    return attrs;
}

KKSMap<qint64, KKSAttrValue *> KKSLoader::loadAttrAttrValues(KKSAttrValue * av, bool forRec) const
{
    KKSMap<qint64, KKSAttrValue *> aavList;

    if(!av || !av->attribute())
        return aavList;
    
    QString sql;
    if(!forRec)
        sql = QString("select * from aGetAttrsAttrsValues(%1, %2)").arg(av->id()).arg(av->attribute()->id());
    else
        sql = QString("select * from aGetRecAttrsAttrsValues(%1, %2)").arg(av->id()).arg(av->attribute()->id());

    KKSResult * res = db->execute(sql);
    if(!res)
        return aavList;

    int count = res->getRowCount();
    if(count == 0){
        delete res;
        return aavList;
    }

    KKSCategoryAttr * ca = av->attribute();

    for(int row=0; row<count; row++){
        //параметры самого атрибута
        int idAttrAttr = res->getCellAsInt(row, 3);
        KKSCategoryAttr * a = ca->attrAttr(idAttrAttr);
        if(!a){
            qWarning("The attrValue contain attr-value but categoryAttr does not contain attribute! idAttr = %d, idObject = %d, idCategory = %d", idAttrAttr, av->id(), ca->id());
            continue;
        }

        KKSAttrValue * attrAttrValue = new KKSAttrValue();
        attrAttrValue->setAttribute(a);

        if(!res->isEmpty(row, 2)){
            KKSValue v = constructValue(res->getCellAsString(row, 2), a, a->tableName()); //3-й параметр - parentTable. Для комплексных атрибутов не допускается в их составе иметь атрибут типа родитель-потомок
            if(!v.isValid())
            {
                //qWarning("Value for attribute is NOT valid! Value = %s, idCategoryAttr = %d, idAttrValue = %d, idAttrValue = %d",
                //            res->getCellAsString(row, 2).toLocal8Bit().data(),
                //            ca->id(),
                //            res->getCellAsInt(row, 1),
                //            av->id());
            }
            attrAttrValue->setValue(v);
        }

        
        
        //устанавливаем дополнительные характеристики значения атрибута
        if(!res->isEmpty(row, 0))
            attrAttrValue->setId(res->getCellAsInt64(row, 0));
        
        attrAttrValue->setActual(true);//для атрибутов, входящих в состав составного атрибута актуальность всегда устанавлдиваем в true, 
                                       //т.к. их значения связаны с актуальтым значением составного атрибута в целом.
                                       //темпоральность отдельных частей составных атрибюутов не поддерживается

        aavList.insert(attrAttrValue->id(), attrAttrValue);
        attrAttrValue->release();
    }
    
    delete res;

    return aavList;
}

QString KKSLoader::loadColumnValue(const KKSIndAttr * a,
                                   qint64 id, 
                                   const QString & parentTable) const
{
    QString tName = a->tableName();
    QString cName = a->columnName();
    
    QString value;

    if(tName.isEmpty() && cName.isEmpty())
        return value;
    
    QString tableName = tName.isEmpty() ? parentTable : tName;
    if(tableName.isEmpty())
        return value;


	if(tableName == "io_objects"){
		tableName = QString("f_sel_io_objects(%1)").arg(id);
	}

    QString pKeyName = a->refColumnName();
    if(pKeyName.isEmpty())
        pKeyName = "id";

	QString sql = QString("select \"%1\" from %2 where \"%4\" = %3")
                                        .arg(cName)
                                        .arg(tableName)
                                        .arg(id)
                                        .arg(pKeyName);

    KKSResult * res = db->execute(sql);
    if(!res)
        return value;

    int count = res->getRowCount();
    if(count != 1){
        qWarning("The column value does not present in table!");
        delete res;
        return value;
    }

    value = res->getCellAsString(0, 0);
    delete res;

    return value;
}

KKSMap<int, KKSCategoryAttr *> KKSLoader::loadCategoryAttrs(int idCategory) const
{
    KKSMap<int, KKSCategoryAttr *> attrs;
    
    QString sql = QString("select * from cGetCategoryAttrs(%1)").arg(idCategory);
    KKSResult * res = db->execute(sql);
    if(!res)
        return attrs;

    int count = res->getRowCount();
    if(count == 0){
        delete res;
        return attrs;
    }

    for(int row=0; row<count; row++){
        //параметры самого атрибута
        KKSCategoryAttr * attr = new KKSCategoryAttr();
        attr->setIdRow(res->getCellAsInt(row, 21));
        attr->setId(res->getCellAsInt(row, 0));
        attr->setCode(res->getCellAsString(row, 2));
        attr->setName(res->getCellAsString(row, 3));
        attr->setTitle(res->getCellAsString(row, 4));
        attr->setTableName(res->getCellAsString(row, 5));
        attr->setColumnName(res->getCellAsString(row, 6));
        attr->setDefWidth(res->getCellAsInt(row, 7));
        
        //тип атрибута
        KKSAttrType * type = new KKSAttrType();
        type->setId(res->getCellAsInt(row, 1));
        type->setName(res->getCellAsString(row, 8));
        type->setCode(res->getCellAsString(row, 9));

        if(!attr->columnName().isEmpty()){
            KKSAttrType * refType = new KKSAttrType();
            int idRefType = res->getCellAsInt(row, 13);
            if (idRefType != KKSAttrType::atList)//это когда атрибут является ссылкой на некоторое обычное значение
            {
                refType->setId (idRefType);
                refType->setName(res->getCellAsString(row, 14));
                refType->setCode(res->getCellAsString(row, 15));
            }
            else//это когда атрибут является ссылкой на атрибут типа ссылка (ссылка на ссылку)
            {
                bool isLow = false;
                QString aRefCode = attr->columnName();
                while (!isLow)
                {
                    KKSAttribute * refAttr = loadAttribute (aRefCode, attr->tableName());
                    if (!refAttr)
                        break;
                    aRefCode = refAttr->code ();
                    isLow = (refAttr->refType()->id() != KKSAttrType::atList);
                    if (isLow)
                    {
                        //qDebug () << __PRETTY_FUNCTION__ << isLow << refAttr->refType()->id();
                        refType->setId (refAttr->refType()->id());
                        refType->setName (refAttr->refType()->name());
                        refType->setCode (refAttr->refType()->code());
                    }
                    refAttr->release ();
                }
            }
            attr->setRefType(refType);
            refType->release();
        }
        /*if(!attr->columnName().isEmpty()){
            //тип ссылочного атрибута
            KKSAttrType * refType = new KKSAttrType();
            refType->setId(res->getCellAsInt(row, 13));
            refType->setName(res->getCellAsString(row, 14));
            refType->setCode(res->getCellAsString(row, 15));
            
            attr->setRefType(refType);
            refType->release();
        }*/

        attr->setType(type);
        type->release();

        //параметры атрибута в категории
        //дефолтное значение
        if (attr->type()->attrType() == KKSAttrType::atPoints)
            qDebug () << __PRETTY_FUNCTION__ << attr->id() << res->getCellAsString(row, 10);
        KKSValue defValue = constructValue(res->getCellAsString(row, 10), attr);
        if(!defValue.isValid())
            qWarning("defValue for attribute is NOT valid! defValue = %s, idCategory = %d, idAttribute = %d", 
                        res->getCellAsString(row, 10).toLocal8Bit().data(), 
                        idCategory, 
                        res->getCellAsInt(row, 0));

        attr->setDefValue(defValue);

        //обязательный атрибут
        attr->setMandatory(res->getCellAsBool(row, 11));
        //только для чтения атрибут
        attr->setReadOnly(res->getCellAsBool(row, 12));

        //поисковый запрос
        if(res->isEmpty(row, 17))
            attr->setSearchTemplate(NULL);
        else{
            KKSSearchTemplate * st = this->loadSearchTemplate(res->getCellAsInt(row, 17));
            if(st){
                attr->setSearchTemplate(st);
                st->release();
            }
        }

        //название ссылочной колонки
        if(!res->isEmpty(row, 18)){
            attr->setRefColumnName(res->getCellAsString(row, 18));
            attr->setRefColumnType(type);
        }

        //группа атрибута
        int idGroup = res->getCellAsInt(row, 19);
        QString groupName = res->getCellAsString(row, 20);
        KKSAGroup * g = new KKSAGroup(idGroup, groupName);
        attr->setGroup(g);
        g->release();

        attrs.insert(attr->id(), attr);
        attr->release();
    }
    
    delete res;

    return attrs;
}

KKSLifeCycleEx * KKSLoader::loadLifeCycle(int idLifeCycle) const
{
    KKSLifeCycleEx * lc = NULL;

    QString sql  = QString("select * from cGetLifeCycleInfo(%1)").arg(idLifeCycle);
    QString sql1 = QString("select * from cGetStateCrosses(%1);").arg(idLifeCycle);
    QString sql2 = QString("select * from lcGetStates(%1)").arg(idLifeCycle);


    KKSResult * res = db->execute(sql);
    if(!res)
        return lc;

    int count = res->getRowCount();
    if(count == 0){
        delete res;
        return lc;
    }
    
    lc = new KKSLifeCycleEx(res->getCellAsInt(0, 0), res->getCellAsString(0, 1), res->getCellAsString(0, 2));
    KKSState * startState = NULL;
    if(!res->isEmpty(0, 3)){
        startState = new KKSState(res->getCellAsInt(0, 3), res->getCellAsString(0, 4), res->getCellAsString(0, 5));
        startState->setIsSystem(res->getCellAsBool(0, 6));
    }
    lc->setStartState(startState);
    
    if(startState)
        startState->release();

    KKSState * autoStateAttr = NULL;
    if(!res->isEmpty(0, 7)){
        autoStateAttr = new KKSState(res->getCellAsInt(0, 7), res->getCellAsString(0, 8), res->getCellAsString(0, 9));
        autoStateAttr->setIsSystem(res->getCellAsBool(0, 10));
    }
    lc->setAutoStateAttr(autoStateAttr);

    if(autoStateAttr)
        autoStateAttr->release();

    KKSState * autoStateInd = NULL;
    if(!res->isEmpty(0, 11)){
        autoStateInd = new KKSState(res->getCellAsInt(0, 11), res->getCellAsString(0, 12), res->getCellAsString(0, 13));
        autoStateInd->setIsSystem(res->getCellAsBool(0, 14));
    }
    lc->setAutoStateInd(autoStateInd);

    if(autoStateInd)
        autoStateInd->release();

    delete res;

    res = db->execute(sql2);//states
    if(!res)
        return lc;

    count = res->getRowCount();
    if(count == 0){
        delete res;
        return lc;
    }

    for(int i=0; i<count; i++){
        KKSState * s = new KKSState(res->getCellAsInt(i, 0), res->getCellAsString(i, 1), res->getCellAsString(i, 2));
        s->setIsSystem(res->getCellAsBool(i, 3));
        lc->addState(s);
        s->release();
    }

    delete res;

    res = db->execute(sql1);//state_crosses
    if(!res)
        return lc;

    count = res->getRowCount();
    if(count == 0){
        delete res;
        return lc;
    }

    KKSState * s1 = NULL;
    KKSState * s2 = NULL;
    for(int row=0; row<count; row++){
        s1 = new KKSState();
        s1->setId(res->getCellAsInt(row, 2));
        s1->setName(res->getCellAsString(row, 3));
        s1->setDesc(res->getCellAsString(row, 4));
        s1->setIsSystem(res->getCellAsBool(row, 5));

        s2 = new KKSState();
        s2->setId(res->getCellAsInt(row, 6));
        s2->setName(res->getCellAsString(row, 7));
        s2->setDesc(res->getCellAsString(row, 8));
        s2->setIsSystem(res->getCellAsBool(row, 9));

        lc->addStateCross(s1, s2);
        s1->release();
        s2->release();
    }

    delete res;

    return lc;
}

KKSValue  KKSLoader::constructValue(const QString & value, 
                                    const KKSAttribute * a, 
                                    const QString & parentTable) const
{
    KKSValue v;
    if(!a || !a->type())
        return v;

    //qDebug () << __PRETTY_FUNCTION__ << a->id() << value;

    //в данном методе type предстает не как идентификатор строки в БД, а элемент enum KKSAttrType::KKSAttrTypes
    v.setValue(value, a->type()->attrType());
    if(!v.isValid())
        qWarning("Value for attribute is NOT valid! Value = %s", value.toLocal8Bit().data());

    if(v.value().isEmpty())
        return v;

    if( a->type()->attrType() == KKSAttrType::atList ||
        a->type()->attrType() == KKSAttrType::atParent
        )
    {
        //в случае атрибутов типа список необходимо загрузить в KKSValue реальное значение из соответствующей таблицы БД
        QString cv = loadColumnValue(a, 
                                     v.valueVariant().toInt(), 
                                     parentTable);
        if(!cv.isEmpty())
            v.setColumnValue(cv);
        else
            qWarning("The attribute KKSAttrType::atList points to empty or NULL value in corresponding table!");
    }

    return v;
}

KKSCategory * KKSLoader::loadCategory(int id, bool simplify) const
{
    
    KKSCategory * c = NULL;
    if(id <= 0)
        return c;

    c = NULL; //TODO!!! loadedCategories.value(id, NULL);
    if(c){
        c->addRef();
        return c;
    }

    QString sql = QString("select * from cGetCategory(%1)").arg(id);
    KKSResult * res = db->execute(sql);
    if(!res)
        return NULL;

    int count = res->getRowCount();
    if(count != 1){
        delete res;
        return NULL;
    }

    KKSType * type = new KKSType();
    type->setId(res->getCellAsInt(0, 1));
    type->setName(res->getCellAsString(0, 5));
    type->setDesc(res->getCellAsString(0, 6));
    type->setAsQualifier(res->getCellAsBool(0, 15));
    
    KKSState * state = new KKSState();
    state->setId(res->getCellAsInt(0, 11));
    state->setName(res->getCellAsString(0, 12));
    state->setDesc(res->getCellAsString(0, 13));

    c = new KKSCategory();
    //тип
    c->setType(type);
    type->release();
    //состояние
    c->setState(state);
    state->release();
    //ИД
    c->setId(res->getCellAsInt(0, 0));
    //название
    c->setName(res->getCellAsString(0, 3));
    //code
    c->setCode(res->getCellAsString(0, 8));
    //описание
    c->setDesc(res->getCellAsString(0, 4));
    
    //дочерняя категория (для контейнерных ИО)
    if(!res->isEmpty(0, 2)){
        KKSCategory * c1 = loadCategory(res->getCellAsInt(0, 2));
        c->setTableCategory(c1);
        if(c1)
            c1->release();
    }

    //дочерняя категория 2 (для контейнерных ИО)
    if(!res->isEmpty(0, 16)){
        KKSCategory * c2 = loadCategory(res->getCellAsInt(0, 16));
        c->setRecAttrCategory(c2);
        if(c2)
            c2->release();
    }

    //
    // is_main
    //
    c->setMain(res->getCellAsBool(0, 7));
    //
    // is_system
    //
    c->setAsSystem(res->getCellAsBool(0, 10));
    //
    // is_global
    //
    c->setAsGlobal(res->getCellAsBool(0, 14));

    //
    // атрибуты
    //
    KKSMap<int, KKSCategoryAttr *> attrs = loadCategoryAttrs(c->id());
    c->setAttributes(attrs);
    
    if(simplify){
        delete res;
        return c;
    }
    //
    // ЖЦ
    //
    if(!res->isEmpty(0, 17)){
        int idLifeCycle = res->getCellAsInt(0, 17); 
        KKSLifeCycleEx * lc = loadLifeCycle(idLifeCycle);
        c->setLifeCycle(lc);
        if(lc)
            lc->release();
    }

    delete res;

    //
    //рубрики
    //
    loadRubrics(c);
    //KKSAccessEntity * acl = 
    //
    // права доступа
    //
    this->loadAccessCat (c);
    //
    // шаблоны
    //
    loadCategoryTemplates (c);

    //TODO!!!
    //c->addRef();
    //loadedCategories.insert(c->id(), c);

    return c;
}

KKSObject * KKSLoader::loadIO_id(int id, bool simplify) const
{
    return loadIO(id, simplify);
}

KKSObject * KKSLoader::loadIO(int id, bool simplify) const
{
    KKSObject * io = NULL;
    
    //int idUser = getUserId();
    
    //if(getPrivilege(idUser, id, 2, true) == false){
    //    return io;
    //}
    
    if(simplify){
        io = loadedObjects.value(id, NULL);
        if(io){
            io->addRef();
            return io;
        }
    }

    //io = NULL;//TODO!!!! loadedObjects.value(id, NULL);
    //if(io){
    //    io->addRef();
    //    return io;
    //}

    QString sql = QString("select * from ioGetObject(%1)").arg(id);
    KKSResult * res = db->execute(sql);
    if(!res)
        return io;

    int count = res->getRowCount();
    if(count != 1){
        qWarning("The IO does not exist or you does not have permissions to load that!");
        delete res;
        return io;
    }

    io = new KKSObject();
    io->setId(id);

    KKSCategory * c = loadCategory(res->getCellAsInt(0, 1), simplify);
    if(!c){
        qWarning("The Error was occured while loading IO category! idObject = %d, idCategory = %d", id, res->getCellAsInt(0, 1));
        delete res;
        io->release();
        return NULL;
    }

    io->setCategory(c);
    c->release();

    io->setDesc(res->getCellAsString(0, 7));
    io->setName(res->getCellAsString(0, 4));
    io->setTableName(res->getCellAsString(0, 6));
    if(!res->isEmpty(0, 2))
        io->setAuthor(res->getCellAsInt(0, 2));
    io->setInfo(res->getCellAsString(0, 8));
    io->setInsertTime(res->getCellAsDateTime(0, 10));
    io->setIsSystem(res->getCellAsBool(0, 9));
    io->setIdMaclabel(res->getCellAsInt(0, 13));
    io->setOwnerOrg(res->getCellAsInt(0, 18));
    io->setIsGlobal(res->getCellAsBool(0, 19));
    QColor bkcolor = QColor();//res->getCell (0, 20).value<QColor>();//QColor::fromRgba ();
    if (res->getCell (0, 20).isValid())
    {
        unsigned int vlc = res->getCellAsInt64(0, 20);
        bkcolor = QColor::fromRgba (vlc);
        io->setRecordFillColor(bkcolor);
    }

    QColor fgcolor = QColor();//.value<QColor>();//QColor::;
    if (res->getCell (0, 21).isValid())
    {
        unsigned int vltc = res->getCellAsInt64(0, 21);
        fgcolor = QColor::fromRgba (vltc);
        io->setRecordTextColor(fgcolor);
    }
    //qDebug () << __PRETTY_FUNCTION__ << io->recordFillColor () << io->recordTextColor ();
    
    io->setIdSearchTemplate(res->getCellAsInt(0, 22));
    io->setRefTableName(res->getCellAsString(0, 23));
    io->setUuid(res->getCellAsString(0, 30));

    if(simplify){
        delete res;
        io->addRef();
        loadedObjects.insert(io->id(), io);

        return io;
    }

    //синхронизируемые организации
    sql = QString("( select getExValuesId(%1, 'io_objects_organization', 'id_io_objects', 'id_organization') )").arg(id);
    KKSResult * r = db->execute(sql);
    if(r){
        if(r->getRowCount() > 0){
            QString s = r->getCellAsString(0, 0);
            KKSValue v(s, KKSAttrType::atCheckListEx);
            io->setReplicatedOrgsV(v);
        }
        delete r;
    }

    KKSState * s = new KKSState();
    s->setId(res->getCellAsInt(0, 3));
    s->setName(res->getCellAsString(0, 11));
    s->setDesc(res->getCellAsString(0, 12));
    io->setState(s);
    s->release();

    io->setAttrValues(loadAttrValues(io));

    io->setIndicatorValues(loadIndicatorValues(io));

    KKSList<KKSFile *> files = loadFiles(io);
    io->setFiles(files);


    int id1 = res->getCellAsInt(0, 25);
    QString name = res->getCellAsString(0, 26);
    QString desc = res->getCellAsString(0, 27);

    KKSType * type = new KKSType(id1, name, desc);//здесть экземпляр данного класса является типом ИО, поэтому метод KKSType::isQualifier() смысла не имеет

    io->setType(type);
    type->release();


    KKSSyncType * st = new KKSSyncType();
    st->setId(res->getCellAsInt(0, 15));
    st->setName(res->getCellAsString(0, 16));
    st->setDesc(res->getCellAsString(0, 17));
    io->setSyncType(st);
    st->release();
    
    QString iconStr = res->getCellAsString(0, 29);
    io->setIcon(iconStr);

    delete res;

    loadUserTemplates(io);

    loadRubrics(io);

    loadPrivileges(io);

    //TODO!!!
    //io->addRef();
    //loadedObjects.insert(io->id(), io);

    return io;
}

KKSObject * KKSLoader::loadIO(const QString & tableName, bool simplify) const
{
    KKSObject * io = NULL;
    QString sql = QString("select ioGetObjectIDByTableName ('%1');").arg(tableName);
    //
    // io.id from io_objects io where table_name = '%1'
    //
    KKSResult * res = db->execute(sql);
    if(!res)
        return io;

    int count = res->getRowCount();
    if(count == 0){
        qWarning("The IO does not exist!");
        delete res;
        return io;
    }
    if(count > 1){
        qWarning() << __PRETTY_FUNCTION__ << "Corrupt data!";
        delete res;
        return io;
    }
    
    int id = res->getCellAsInt(0, 0);
    delete res;

    io = loadIO(id, simplify);
    return io;
}

KKSObjectExemplar * KKSLoader::loadEIO(qint64 id, 
									   const KKSObject * io, 
									   const KKSCategory *c0, 
									   const QString& table,
									   bool simplify) const
{
    KKSObjectExemplar * eio = NULL;
    
    if(!io || io->tableName().isEmpty()){
        qWarning("Given IO cannot contain any of EIO. It is simple (tableName is empty)");
        return eio;
    }

    QString fields;
    QString tableName = table.isEmpty() ? io->tableName() : table;
    
    const KKSCategory * c = 0;
    if (!c0 || table.isEmpty() || table == io->tableName())
    {
        c = io->category();
        if(!c)
            return eio;

        c = c->tableCategory();
    }
    else
        c = c0;

    KKSMap<int, KKSCategoryAttr *> attrs = c->attributes();
    int count = attrs.count();
    int i=0;
    KKSMap<int, KKSCategoryAttr *>::const_iterator pa;
    for (pa=attrs.constBegin(); pa != attrs.constEnd(); pa++)
    {
        KKSCategoryAttr * attr = pa.value();
        QString code;

        if(attr->type()->attrType() == KKSAttrType::atCheckListEx){
            //i++;
            //continue;
            QString rTable = tableName + "_" + attr->tableName();
            QString refTable;
            if(rTable == ORGANIZATION_WORK_MODE || 
               rTable == POSITION_WORK_MODE || 
               rTable == UNITS_WORK_MODE ||
               rTable == IO_OBJECTS_ORGANIZATION ||
               //rTable == IO_CATEGORIES_ORGANIZATION ||
               rTable == USER_CHAINS_ORGANIZATION ||
               rTable == REPORT_ORGANIZATION ||
               rTable == GUARD_OBJ_DEVICES_TSO ||
               rTable == ACCESS_CARDS_ACCESS_PLAN_TSO ||
               rTable == MAIL_LISTS_POSITION ||
               rTable == LIFE_CYCLE_IO_STATES ||
               rTable == IO_PROCESSING_ORDER_CHAINS ||
               rTable == SHU_DLS_POSITION
               )
            {
                refTable = rTable;
            }
            else
                refTable = rTable + "_ref_" + QString::number(attr->id());
            QString mainAttr = QString("id_%1").arg(tableName);
            QString childAttr = QString("id_%1").arg(attr->tableName());

            code = QString("( select getExValuesId(%1, '%2', '%3', '%4') )").arg(id).arg(refTable).arg(mainAttr).arg(childAttr);
        }
        else{
            code = attr->code(true);
        }
        
        fields += code;

        if(i<count-1){
            fields += ", ";
        }

        i++;
    }

    //обходной маневр для выборки записей ТСД
    //прямой доступ к данной таблице запрещен
    QString sql;
    if(tableName.toLower() == "tsd"){
        sql = QString("select * from jGetTsdRecord(%1);").arg(id);
    }
    else{
		if(tableName == "io_objects"){
			tableName = QString("f_sel_io_objects(%1)").arg(id);
		}
        if(io->id() <= _MAX_SYS_IO_ID_)
            sql = QString("select last_update, unique_id, %1 from %2 where id = %3").arg(fields).arg(tableName).arg(id);
        else
            sql = QString("select id_io_state, uuid_t, last_update, unique_id, r_icon, record_fill_color, record_text_color, %1 from %2 where id = %3").arg(fields).arg(tableName).arg(id);
    }

    qDebug() << __PRETTY_FUNCTION__ << sql;

    KKSResult * res = db->execute(sql);
    if(!res)
        return eio;

    count = res->getRowCount();
    if(count != 1){
        qWarning("The too many EIO with given ID! EIO.id = %d, IO.id = %d", id, io->id());
        delete res;
        return eio;
    }

    eio = new KKSObjectExemplar();
    eio->setIo(const_cast<KKSObject *> (io));

    KKSList<KKSAttrValue *> attrValues;
    
    
    KKSState * s = NULL;
    if(io->id() <= _MAX_SYS_IO_ID_){
        s = KKSState::defState1();
    }
    else{
        if(res->getCellAsInt(0, 0) <= 1)
            s = KKSState::defState1();
        else{
            s = loadState(res->getCellAsInt(0, 0));
        }
    }

    eio->setState(s);
    if(s)
        s->release();

    QString rIcon; 

    if(io->id() <= _MAX_SYS_IO_ID_){
        eio->setLastUpdate(res->getCellAsDateTime(0, 0));
        eio->setUniqueId(res->getCellAsString(0, 1));
        rIcon = KKSObjectExemplar::defIconAsString();
        eio->setIcon(rIcon);
        i = 2;
    }
    else{
        eio->setUuid(res->getCellAsString(0, 1));
        eio->setLastUpdate(res->getCellAsDateTime(0, 2));
        eio->setUniqueId(res->getCellAsString(0, 3));
        eio->setIcon(res->getCellAsString(0, 4));
        QColor bkcolor = QColor();//res->getCell (0, 20).value<QColor>();//QColor::fromRgba ();
        if (res->getCell (0, 5).isValid())
        {
            unsigned int vlc = res->getCellAsInt64(0, 5);
            bkcolor = QColor::fromRgba (vlc);
            eio->setRecordFillColor(bkcolor);
        }

        QColor fgcolor = QColor();//.value<QColor>();//QColor::;
        if (res->getCell (0, 6).isValid())
        {
            unsigned int vltc = res->getCellAsInt64(0, 6);
            fgcolor = QColor::fromRgba (vltc);
            eio->setRecordTextColor(fgcolor);
        }

        i = 7;
    }

    
    
    KKSMap<int, KKSCategoryAttr *>::const_iterator pa1;
    for (pa1=attrs.constBegin(); pa1 != attrs.constEnd(); pa1++)
    {
        KKSCategoryAttr *attr = pa1.value();

        KKSAttrValue * av = new KKSAttrValue();
        av->setAttribute(attr);

        KKSValue v;
        if(attr->type()->attrType() == KKSAttrType::atParent){
            v = constructValue(res->getCellAsString(0, i), attr, io->tableName());
        }
        else{
            v = constructValue(res->getCellAsString(0, i), attr);
        }
        av->setValue(v);
        if(attr->code() == "name")
            eio->setName(res->getCellAsString(0, i));
        if(attr->code() == "description")
            eio->setDesc(res->getCellAsString(0, i));
        if(attr->code() == "code")
            eio->setCode(res->getCellAsString(0, i));

        attrValues.append(av);
        av->release();
        i++;
    }

    eio->setAttrValues(attrValues);
    
    eio->setId(id);

	if(simplify)
		return eio;
    
    eio->setIndValues(loadIndValues(eio));
    
    loadRecRubrics (eio);
	
	eio->setFiles(loadFiles(eio));


    delete res;

    return eio;
}


KKSType * KKSLoader::loadType(int id) const
{
    QString sql = QString("select * from cGetCategoryType(%1);").arg(id);

    KKSResult * res = db->execute(sql);
    if(!res)
        return NULL;

    int count = res->getRowCount();
    if(count != 1){
        delete res;
        return NULL;
    }

    KKSType * type = new KKSType();
    type->setId(res->getCellAsInt(0, 0));
    type->setName(res->getCellAsString(0, 1));
    type->setRName(res->getCellAsString(0, 2));
    type->setDesc(res->getCellAsString(0, 3));
    type->setAsQualifier(res->getCellAsBool(0, 4));
    
    delete res;

    return type;
}

KKSType * KKSLoader::loadIOType(int id) const
{
    QString tableNames = QString(VIEW_PREFIX) + "io_types t ";

    QString sql = QString("select t.id, t.name, t.description "
                          "from %1 "
                          "where t.id = %2 ").arg(tableNames).arg(id);

    KKSResult * res = db->execute(sql);
    if(!res)
        return NULL;

    int count = res->getRowCount();
    if(count != 1){
        delete res;
        return NULL;
    }

    KKSType * type = new KKSType();
    type->setId(res->getCellAsInt(0, 0));
    type->setName(res->getCellAsString(0, 1));
    type->setDesc(res->getCellAsString(0, 2));
    
    delete res;

    return type;
}

KKSMap<int, KKSType *> KKSLoader::loadAvailableTypes (void) const
{
    KKSMap<int, KKSType*> typeList;
    
    QString sql = QString("select * from cGetCategoryTypes() where id not in (8, 9, 10);");

    KKSResult * res = db->execute(sql);
    if(!res)
        return typeList;

    int count = res->getRowCount();
    if(count <= 0){
        delete res;
        return typeList;
    }

    for (int row=0; row<count; row++){
        KKSType * type = new KKSType();
        int id = res->getCellAsInt(row, 0);
        
        type->setId(id);
        type->setName(res->getCellAsString(row, 1));
        type->setRName(res->getCellAsString(row, 2));
        type->setDesc(res->getCellAsString(row, 3));
        type->setAsQualifier(res->getCellAsBool(row, 4));
        
        typeList.insert(id, type);
        type->release();
    }
    
    delete res;
    
    return typeList;
}

KKSAttrType * KKSLoader::loadAttrType(int id) const
{
    QString sql = QString("select * from atGetAttrType(%1)").arg(id);

    KKSResult * res = db->execute(sql);
    if(!res)
        return NULL;

    int count = res->getRowCount();
    if(count == 0){
        delete res;
        return NULL;
    }

    //тип атрибута
    KKSAttrType * type = new KKSAttrType();
    type->setId(res->getCellAsInt(0, 0));
    type->setName(res->getCellAsString(0, 1));
    type->setCode(res->getCellAsString(0, 2));

    delete res;

    return type;
}

KKSAttrType * KKSLoader::loadAttrType(KKSAttrType::KKSAttrTypes type) const
{

    int id = 0;

    switch (type) 
    {
        case KKSAttrType::atBool: id = 1; break;                //логический
        case KKSAttrType::atList: id = 2; break;                //элемент справочника
        case KKSAttrType::atParent: id = 3;  break;             //родитель (ссылка на элемент того же справочника)
        case KKSAttrType::atDate: id = 4;  break;               //дата
        case KKSAttrType::atDateTime: id = 5;  break;           //дата-время
        case KKSAttrType::atDouble: id = 6; break;              //дробное значение
        case KKSAttrType::atObjRef: id = 7; break;              //ссылка на информационный объект
        case KKSAttrType::atInt: id = 8;  break;                //целочисленное
        case KKSAttrType::atString: id = 9;  break;             //строка
        case KKSAttrType::atInterval: id = 10; break;           //временной интервал (лет, месяцев, дней, часов, минут)
        case KKSAttrType::atUrl: id = 11;  break;               //гиперссылка
        case KKSAttrType::atCheckList: id = 12; break;          //набор значений справочника (старый)
        case KKSAttrType::atText: id = 13; break;               //Текст
        case KKSAttrType::atFixString: id = 14; break;          //Строка фиксированной длины
        case KKSAttrType::atJPG: id = 15; break;                //изображение JPG
        case KKSAttrType::atMaclabel: id = 16; break;           //Мандатная метка
        case KKSAttrType::atCheckListEx: id = 17; break;        //набор значений справочника
        case KKSAttrType::atRecordColor: id = 18; break;        //Цветовая гамма фона записи справочника (при отображении в списке)
        case KKSAttrType::atRecordColorRef: id = 19; break;     //Ссылка на цвет фона записи (при отображении в списке)
        case KKSAttrType::atXMLDoc: id = 20; break;             //XML-документ
        case KKSAttrType::atIntervalH: id = 21; break;          //временной интервал (часов, минут, секунд)
        case KKSAttrType::atPoints: id = 22; break;             //Набор пространственных точек
        case KKSAttrType::atSVG: id = 23; break;                //Объект SVG
        case KKSAttrType::atVideo: id = 24; break;              //Видео-ролик
        case KKSAttrType::atRecordTextColor: id = 25; break;    //Цветовая гамма текста записи справочника (при отображении в списке)
        case KKSAttrType::atRecordTextColorRef: id = 26; break; //Ссылка на цвет текста записи (при отображении в списке)
        case KKSAttrType::atTime: id = 27;  break;              //время
        //case KKSAttrType::atGeometry: id = 28; break;           //ГИС-объект (базовый)
        //case KKSAttrType::atGeometryPoly: id = 29; break;       //ГИС-объект (стандартный полигон)
        case KKSAttrType::atInt64: id = 30; break;              //целочисленное (64 бита)
        case KKSAttrType::atUUID: id = 31; break;               //уникальный идентификатор (UUID)
        case KKSAttrType::atComplex: id = 32; break;            //Составной атрибут
        case KKSAttrType::atHistogram: id = 33; break;          //Гистограмма
        case KKSAttrType::atVectorLayer: id = 34; break;        //ГИС-объект (векторный слой)
        case KKSAttrType::atRasterLayer: id = 35; break;        //ГИС-объект (растровый слой)
        case KKSAttrType::atGISMap: id = 36; break;             //ГИС-объект (карта)
    }

    KKSAttrType * aType = loadAttrType(id);

    return aType;
}


KKSAttribute * KKSLoader::loadAttribute(const QString & code, const QString & tableName) const
{
    QString sql = QString("select * from aGetAttribute('%1', '%2')").arg(code).arg(tableName);

    KKSResult * res = db->execute(sql);
    if(!res)
        return NULL;

    int count = res->getRowCount();
    if(count == 0){
        delete res;
        return NULL;
    }

    //параметры самого атрибута
    KKSAttribute * attr = new KKSAttribute();
    attr->setId(res->getCellAsInt(0, 0));
    attr->setCode(res->getCellAsString(0, 2));
    attr->setName(res->getCellAsString(0, 3));
    attr->setTitle(res->getCellAsString(0, 4));
    attr->setTableName(res->getCellAsString(0, 5));
    attr->setColumnName(res->getCellAsString(0, 6));
    attr->setDefWidth(res->getCellAsInt(0, 7));
    
    //тип атрибута
    KKSAttrType * type = new KKSAttrType();
    type->setId(res->getCellAsInt(0, 1));
    type->setName(res->getCellAsString(0, 8));
    type->setCode(res->getCellAsString(0, 9));

    //Тип ссылочного атрибута
    if(!attr->columnName().isEmpty()){
        KKSAttrType * refType = new KKSAttrType();
        int idRefType = res->getCellAsInt(0, 13);
        if (idRefType != KKSAttrType::atList)//это когда атрибут является ссылкой на некоторое обычное значение
        {
            refType->setId (idRefType);
            refType->setName(res->getCellAsString(0, 14));
            refType->setCode(res->getCellAsString(0, 15));
        }
        else //это когда атрибут является ссылкой на ссылку
        {
            bool isLow = false;
            QString aRefCode = attr->columnName();
            while (!isLow)
            {
                KKSAttribute * refAttr = loadAttribute (aRefCode, attr->tableName());
                if (!refAttr)
                    break;
                aRefCode = refAttr->code ();
                isLow = (refAttr->refType()->id() != KKSAttrType::atList);
                if (isLow)
                {
                    refType->setId (refAttr->refType()->id());
                    refType->setName (refAttr->refType()->name());
                    refType->setCode (refAttr->refType()->code());
                }
                refAttr->release ();
            }
        }
        attr->setRefType(refType);
        refType->release();
    }
    
    attr->setType(type);
    type->release();

    //поисковый запрос
    if(res->isEmpty(0, 17))
        attr->setSearchTemplate(NULL);
    else{
        KKSSearchTemplate * st = this->loadSearchTemplate(res->getCellAsInt(0, 17));
        if(st){
            attr->setSearchTemplate(st);
            st->release();
        }
    }

    //название ссылочной колонки
    if(!res->isEmpty(0, 18)){
        attr->setRefColumnName(res->getCellAsString(0, 18));
        attr->setRefColumnType(type);//внутри сделается addRef()
    }

    //группа атрибута
    int idGroup = res->getCellAsInt(0, 19);
    QString groupName = res->getCellAsString(0, 20);
    KKSAGroup * g = new KKSAGroup(idGroup, groupName);
    attr->setGroup(g);
    g->release();

    delete res;

    return attr;
}


KKSAttribute * KKSLoader::loadAttribute(int id) const
{    
    QString sql = QString("select * from aGetAttribute(%1)").arg(id);

    KKSResult * res = db->execute(sql);
    if(!res)
        return NULL;

    int count = res->getRowCount();
    if(count == 0){
        delete res;
        return NULL;
    }

    //параметры самого атрибута
    KKSAttribute * attr = new KKSAttribute();
    attr->setId(res->getCellAsInt(0, 0));
    attr->setCode(res->getCellAsString(0, 2));
    attr->setName(res->getCellAsString(0, 3));
    attr->setTitle(res->getCellAsString(0, 4));
    attr->setTableName(res->getCellAsString(0, 5));
    attr->setColumnName(res->getCellAsString(0, 6));
    attr->setDefWidth(res->getCellAsInt(0, 7));
    
    //тип атрибута
    KKSAttrType * type = new KKSAttrType();
    type->setId(res->getCellAsInt(0, 1));
    type->setName(res->getCellAsString(0, 8));
    type->setCode(res->getCellAsString(0, 9));

    //Тип ссылочного атрибута
    if(!attr->columnName().isEmpty()){
        KKSAttrType * refType = new KKSAttrType();
        int idRefType = res->getCellAsInt(0, 13);
        if (idRefType != KKSAttrType::atList) //это когда атрибут является ссылкой на некоторое обычное значение
        {
            refType->setId (idRefType);
            refType->setName(res->getCellAsString(0, 14));
            refType->setCode(res->getCellAsString(0, 15));
        }
        else //это когда атрибут является ссылкой на ссылку
        {
            bool isLow = false;
            QString aRefCode = attr->columnName();
            while (!isLow)
            {
                KKSAttribute * refAttr = loadAttribute (aRefCode, attr->tableName());
                if (!refAttr)
                    break;
                aRefCode = refAttr->code ();
                isLow = (refAttr->refType()->id() != KKSAttrType::atList);
                if (isLow)
                {
                    refType->setId (refAttr->refType()->id());
                    refType->setName (refAttr->refType()->name());
                    refType->setCode (refAttr->refType()->code());
                }
                refAttr->release ();
            }
        }
        attr->setRefType(refType);
        refType->release();
    }

    attr->setType(type);
    type->release();

    //поисковый запрос
    if(res->isEmpty(0, 17))
        attr->setSearchTemplate(NULL);
    else{
        KKSSearchTemplate * st = this->loadSearchTemplate(res->getCellAsInt(0, 17));
        if(st){
            attr->setSearchTemplate(st);
            st->release();
        }
    }

    //название ссылочной колонки
    if(!res->isEmpty(0, 18)){
        attr->setRefColumnName(res->getCellAsString(0, 18));
        attr->setRefColumnType(type);//внутри сделается addRef()
    }

    //группа атрибута
    int idGroup = res->getCellAsInt(0, 19);
    QString groupName = res->getCellAsString(0, 20);
    KKSAGroup * g = new KKSAGroup(idGroup, groupName);
    attr->setGroup(g);
    g->release();

    delete res;

    return attr;
}

KKSState * KKSLoader::loadState(int id) const
{
    QString sql = QString("select * from cGetState(%1);").arg(id);

    KKSResult * res = db->execute(sql);
    if(!res)
        return NULL;

    int count = res->getRowCount();
    if(count != 1){
        delete res;
        return NULL;
    }

    KKSState * s = new KKSState();
    s->setId(res->getCellAsInt(0, 0));
    s->setName(res->getCellAsString(0, 1));
    s->setDesc(res->getCellAsString(0, 2));
    s->setIsSystem(res->getCellAsBool(0, 3));
    
    delete res;

    return s;
}

KKSList<KKSState * > KKSLoader::loadStates() const
{
    KKSList<KKSState * > ss;

    QString sql = QString("select * from cGetStates();");

    KKSResult * res = db->execute(sql);
    if(!res)
        return ss;

    int count = res->getRowCount();
    if(count == 0){
        delete res;
        return ss;
    }
    
    for(int i=0; i<count; i++){
        KKSState * s = new KKSState();
        s->setId(res->getCellAsInt(i, 0));
        s->setName(res->getCellAsString(i, 1));
        s->setDesc(res->getCellAsString(i, 2));
        s->setIsSystem(res->getCellAsBool(i, 3));
        ss.append(s);
        s->release();
    }

    delete res;

    return ss;
}

KKSList<KKSState * > KKSLoader::loadStates(int idLifeCycle) const
{
    KKSList<KKSState * > ss;

    QString sql = QString("select * from lcGetStates(%1);").arg(idLifeCycle);

    KKSResult * res = db->execute(sql);
    if(!res)
        return ss;

    int count = res->getRowCount();
    if(count == 0){
        delete res;
        return ss;
    }
    
    for(int i=0; i<count; i++){
        KKSState * s = new KKSState();
        s->setId(res->getCellAsInt(i, 0));
        s->setName(res->getCellAsString(i, 1));
        s->setDesc(res->getCellAsString(i, 2));
        s->setIsSystem(res->getCellAsBool(i, 3));
        ss.append(s);
        s->release();
    }

    delete res;

    return ss;
}


KKSSyncType * KKSLoader::loadSyncType(int id) const
{
    QString tableNames = QString(VIEW_PREFIX) + "io_sync_types s ";

    QString sql = QString("select s.id, s.name, s.description "
                          "from %1 "
                          "where s.id = %2 ").arg(tableNames).arg(id);

    KKSResult * res = db->execute(sql);
    if(!res)
        return NULL;

    int count = res->getRowCount();
    if(count != 1){
        delete res;
        return NULL;
    }

    KKSSyncType * s = new KKSSyncType();
    s->setId(res->getCellAsInt(0, 0));
    s->setName(res->getCellAsString(0, 1));
    s->setDesc(res->getCellAsString(0, 2));
    
    delete res;

    return s;
}

KKSTemplate * KKSLoader::loadTemplate(int id) const
{
    KKSTemplate * t = NULL;

    QString tableNames = QString(VIEW_PREFIX) + "io_templates t";
    QString sql = QString("select t.id, t.id_io_category, t.name, t.code, t.description from %1 where t.id = %2").arg(tableNames).arg(id);

    KKSResult * res = db->execute(sql);
    if(!res)
        return t;

    int count = res->getRowCount();
    if(count != 1){
        delete res;
        return t;
    }
    
    t = new KKSTemplate();
    t->setId(res->getCellAsInt(0, 0));
    t->setName(res->getCellAsString(0, 2));
    t->setCode(res->getCellAsString(0, 3));
    t->setDesc(res->getCellAsString(0, 4));
    KKSMap<int, KKSAttrGroup *> g = loadTemplateAttrsGroups(res->getCellAsInt(0, 0));
    t->setGroups(g);
    KKSCategory * c = loadCategory(res->getCellAsInt(0, 1));
    if(!c){
        delete res;
        t->release();
        qWarning() << "Template is corrupt id_io_category is not present in table io_category!";
        return NULL;
    }

    t->setCategory(c);
    c->release();

    delete res;

    return t;
}

KKSList<KKSTemplate*> KKSLoader::loadCategoryTemplates(int idCategory, bool bWithAllMandatories) const
{
    KKSList<KKSTemplate*> tList;
    if(idCategory < 0)
        return tList;

    KKSTemplate * t = NULL;

    //QString tableNames = QString(VIEW_PREFIX) + "io_templates t";
    //QString sql = QString("select t.id, t.id_io_category, t.name, t.code, t.description from %1 where t.id_io_category = %2")
    //                            .arg(tableNames)
    //                            .arg(idCategory);
    QString sql = QString("select * from getCatTemplates(%1, %2);").arg(idCategory).arg(bWithAllMandatories? "TRUE" : "FALSE");

    KKSResult * res = db->execute(sql);
    if(!res)
        return tList;

    int count = res->getRowCount();
    if(count <= 0){
        delete res;
        return tList;
    }
    
    KKSCategory * c = loadCategory(idCategory);
    if(!c){
        delete res;
        qWarning() << "Template is corrupt. id_io_category is not present in table io_category!";
        return tList;
    }

    for(int i=0; i<count; i++)
    {
        t = new KKSTemplate();
        t->setId(res->getCellAsInt(i, 0));
        t->setName(res->getCellAsString(i, 2));
        t->setCode(res->getCellAsString(0, 3));
        t->setDesc(res->getCellAsString(0, 4));
        KKSMap<int, KKSAttrGroup *> g = loadTemplateAttrsGroups(res->getCellAsInt(i, 0));
        t->setGroups(g);

        t->setCategory(c);
        tList.append(t);
        t->release();
    }

    c->release();

    delete res;

    return tList;
}
 
KKSList<KKSTemplate*> KKSLoader::loadCategoryTemplates (KKSCategory * c, bool bWithAllMandatories) const
{
    KKSList<KKSTemplate*> tList;
    KKSTemplate * t = NULL;
    if(!c)
        return tList;

    QString sql = QString("select * from getCatTemplates(%1, %2);").arg(c->id()).arg(bWithAllMandatories? "TRUE" : "FALSE");

    KKSResult * res = db->execute(sql);
    if(!res)
        return tList;

    int count = res->getRowCount();
    if(count <= 0){
        delete res;
        return tList;
    }
    
    for(int i=0; i<count; i++){
        t = new KKSTemplate();
        t->setId(res->getCellAsInt(i, 0));
        t->setName(res->getCellAsString(i, 2));
        t->setCode(res->getCellAsString(0, 3));
        t->setDesc(res->getCellAsString(0, 4));
        KKSMap<int, KKSAttrGroup *> g = loadTemplateAttrsGroups(res->getCellAsInt(i, 0));
        t->setGroups(g);

        t->setCategory(c);
        tList.append(t);
        c->addTemplate (t);
        t->release();
    }

    delete res;

    return tList;
}

KKSMap<int, KKSAttrGroup *> KKSLoader::loadTemplateAttrsGroups(int idTemplate) const
{
    KKSMap<int, KKSAttrGroup *> groupsList;
    
    QString sql = QString("select * from tGetAttrGroups(%1)").arg(idTemplate);

    KKSResult * res = db->execute(sql);
    if(!res)
        return groupsList;

    int count = res->getRowCount();
    if(count == 0){
        delete res;
        return groupsList;
    }

    KKSAttrGroup * prev_group = NULL;
    KKSAttrGroup * last_group = NULL;
    //int oldGrId = -1;
    for(int row=0; row<count; row++)
    {
//        qDebug () << __PRETTY_FUNCTION__ << res->getCellAsInt (row, 18);
        int entity_type = res->getCellAsInt (row, 18);
        if (row==0 && entity_type == 0)
        {
            delete res;
            return groupsList;
        }

        int id = res->getCellAsInt (row, 0);
        if (entity_type == 1)
        {
            //
            // Это группа атрибутов
            //
            QString g_name = res->getCellAsString (row, 4);
            int g_order = res->getCellAsInt (row, 13);
            KKSAttrGroup * group = new KKSAttrGroup (id, g_name, g_order);
            if (res->isEmpty(row, 1) || res->getCellAsInt (row, 1) == 0)
            {
                //
                // Это группа верхнего уровня
                //
                groupsList.insert (res->getCellAsInt (row, 0), group);
                prev_group = group;
                last_group = group;
            }
            else //if (res->getCellAsInt (row, 1) == prev_group->id())
            {
                int idParent = res->getCellAsInt (row, 1);
//                qDebug () << __PRETTY_FUNCTION__ << idParent;
                KKSAttrGroup * parent_group = prev_group->childGroupForId (idParent);
                group->setParent (parent_group);
                parent_group->addChildGroup (id, group);
                last_group = group;
            }
        }
        else
        {
            //
            //атрибут
            //
            int idAttrGroup = res->getCellAsInt (row, 1);
            KKSAttrGroup * parent_group = 0;//prev_group->childGroupForId (idAttrGroup);
            for (KKSMap<int, KKSAttrGroup*>::const_iterator p=groupsList.constBegin(); \
                    p != groupsList.constEnd() && parent_group==0; p++)
                parent_group = p.value()->childGroupForId (idAttrGroup);
            if (!parent_group)
            {
                delete res;
                return groupsList;
            }
            //
            // параметры самого атрибута
            //
            KKSAttrView * attr = new KKSAttrView( );

            attr->setIdViewAttr(res->getCellAsInt(row, 22));
            attr->setId (id);
            attr->setCode (res->getCellAsString(row, 3));
            attr->setName (res->getCellAsString(row, 4));
            attr->setTitle(res->getCellAsString(row, 5));
            attr->setTableName (res->getCellAsString(row, 6));
            attr->setColumnName (res->getCellAsString(row, 7));
            attr->setDefWidth (res->getCellAsInt(row, 8));

            //
            // тип атрибута
            //
            KKSAttrType * type = new KKSAttrType();
            type->setId (res->getCellAsInt(row, 2));
            type->setName (res->getCellAsString(row, 9));
            type->setCode (res->getCellAsString(row, 10));

            //
            // Тип ссылочного атрибута
            //
            if(!attr->columnName().isEmpty()){
                KKSAttrType * refType = new KKSAttrType();
                int idRefType = res->getCellAsInt(row, 15);
                if (idRefType != KKSAttrType::atList) //это когда атрибут является ссылкой на некоторое обычное значение
                {
                    refType->setId (idRefType);
                    refType->setName(res->getCellAsString(row, 16));
                    refType->setCode(res->getCellAsString(row, 17));
                }
                else //это когда атрибут является ссылкой на ссылку
                {
                    bool isLow = false;
                    QString aRefCode = attr->columnName();
                    while (!isLow)
                    {
                        KKSAttribute * refAttr = loadAttribute (aRefCode, attr->tableName());
                        if (!refAttr)
                            break;
                        aRefCode = refAttr->code ();
                        isLow = (refAttr->refType()->id() != KKSAttrType::atList);
                        if (isLow)
                        {
                            qDebug () << __PRETTY_FUNCTION__ << isLow << refAttr->refType()->id();
                            refType->setId (refAttr->refType()->id());
                            refType->setName (refAttr->refType()->name());
                            refType->setCode (refAttr->refType()->code());
                        }
                        refAttr->release ();
                    }
                }
                attr->setRefType(refType);
                refType->release();
            }

            attr->setType (type);
            type->release();

            //название ссылочной колонки
            if(!res->isEmpty(row, 19)){
                attr->setRefColumnName(res->getCellAsString(row, 19));
                attr->setRefColumnType(type);
            }

            //группа атрибута
            int idGroup = res->getCellAsInt(row, 20);
            QString groupName = res->getCellAsString(row, 21);
            KKSAGroup * g = new KKSAGroup(idGroup, groupName);
            attr->setGroup(g);
            g->release();

            //
            // параметры атрибута в категории
            //
/*
            //дефолтное значение
            KKSValue defValue = constructValue(res->getCellAsString(row, 14), attr);
            if(!defValue.isValid())
                qWarning("defValue for attribute is NOT valid! defValue = %s, idTemplate = %d, idAttribute = %d", 
                            res->getCellAsString(row, 10).toLocal8Bit().data(), 
                            idTemplate, 
                            res->getCellAsInt(row, 0));
*/
            //
            // обязательный атрибут
            //
            attr->KKSCategoryAttr::setMandatory (res->getCellAsBool(row, 11));

            //
            // параметры атрибута в шаблоне
            //
            attr->KKSAttrView::setReadOnly (res->getCellAsBool(row, 12));
            attr->KKSAttrView::setOrder (res->getCellAsInt(row, 13));
            if (attr->type()->attrType() == KKSAttrType::atPoints)
                qDebug () << __PRETTY_FUNCTION__ << attr->id() << res->getCellAsString(row, 14);
            KKSValue defValue = constructValue (res->getCellAsString(row, 14), attr);
            if (!defValue.isValid())
                qWarning("defValue for attribute is NOT valid! defValue = %s, idTemplate = %d, idAttribute = %d", 
                            res->getCellAsString(row, 14).toLocal8Bit().data(), 
                            idTemplate, 
                            id);
            attr->KKSAttrView::setDefValue(defValue);
            parent_group->addAttrView (id, attr);

        }
/*
        //--KKSAttribute *a = loadAttribute (res->getCellAsInt(row, 0));
        

        //параметры атрибута в категории
        //дефолтное значение
        KKSValue defValue = constructValue(res->getCellAsString(row, 10), attr);
        if(!defValue.isValid())
            qWarning("defValue for attribute is NOT valid! defValue = %s, idTemplate = %d, idAttribute = %d", 
                        res->getCellAsString(row, 10).toLocal8Bit().data(), 
                        idTemplate, 
                        res->getCellAsInt(row, 0));

        attr->KKSCategoryAttr::setDefValue(defValue);
        //только для чтения атрибут
        attr->KKSCategoryAttr::setReadOnly(res->getCellAsBool(row, 12));

        //параметры атрибута в шаблоне
        
        //при необходимости создаем группу
        int newGrId = res->getCellAsInt(row, 13);
        if(!group || oldGrId != newGrId){
            if(group){
                group->release();
            }
            group = new KKSAttrGroup();
            group->setId(res->getCellAsInt(row, 13));
            group->setName(res->getCellAsString(row, 17));
            group->setOrder(res->getCellAsInt(row, 18));
            g.insert (group->id(), group);
            oldGrId = newGrId;
            if(!res->isEmpty(row, 19)){
                int idParent = res->getCellAsInt(row, 19);
                for (int i=0; i<g.count(); i++)
                {
                    KKSAttrGroup * gr = g[i];
                    if(gr->id() == idParent)
                    {
                        group->setParent(gr);
                        break;
                    }
                }
            }

        }

        //и добавляем в нее атрибут
        group->addAttrView(attr->id(), attr);
        attr->release();
*/
    }
    
    delete res;

    return groupsList;
}

QString KKSLoader::generateSelectEIOQuery(const KKSCategory * cat,
                                          const QString& table,
                                          const KKSList<const KKSFilterGroup *> & filters,
                                          bool isSys) const
{
    QString sql = QString::null;
//    if(!io)
//        return sql;
    
    QString tableName = /*table.isEmpty() ? io->tableName() : */table;
    if(tableName.isEmpty())
        return sql;

    QStringList usedTableNames;
    
    QString defParentFilter; //Если визуализируемая таблица является иерархической,
                             //необходимо указывать исходный фильтр для предложения WITH
                             //этот фильтр будет задаваться при разборе имеющихся фильтров.
                             //однако если фильтров с атрибутом родитель-потомок нет, то надо иметь
                             //исходный фильтр. таковым является id_parent isnull
                             //но нам при этом надо знать название ссылочной колонки
    //предусматриваем, что в таблице tableName может быть ссылка на саму себя
    //в этом случае пересекаем ее со своей копией, используя алиас
    //такой случай возможен при использовании атрибута типа atParent
    usedTableNames << tableName;
    
    QString withAttrPK; //название колонки, являющейся первичным ключом в атрибуте родитель-потомок (например id)
    QString withAttrFK; //название колонки, являющейся внешним ключом в атрибуте родитель-потомок (например id_parent)
    QString withTableName = tableName + "_rec"; //название подзапроса в предложении WITH
    //QString withTableName1 = withTableName + "_1"; //псевдоним таблицы в подзапросе рекурсивной части предложения WITH
    
    QString attrsWith;
    QString attrsWith1;
    if(isSys){
        attrsWith = "id, unique_id, last_update ";
        attrsWith1 = tableName + ".id, " + tableName + ".unique_id, " + tableName + ".last_update "; //колонки в подзапросе нерекурсивной части предложения WITH
    }
    else{
        attrsWith = "id, unique_id, last_update, uuid_t, id_io_state, r_icon, record_fill_color, record_text_color ";
        attrsWith1 = tableName + ".id, " + 
                     tableName + ".unique_id, " + 
                     tableName + ".last_update, " + 
                     tableName + ".uuid_t, " + 
                     tableName + ".id_io_state ," + 
                     tableName + ".r_icon, " + 
                     tableName + ".record_fill_color, " + 
                     tableName + ".record_text_color "; //колонки в подзапросе нерекурсивной части предложения WITH
    }
    
    
    
    QString withAttrs;//колонки в самой нижней части запроса с предложением WITH
    QString withJoinWord;//предложение left join в запросе с предложением WITH
    
    QString attrs;
    QString whereWord = " 1=1 ";
    QString joinWord;
    QString fromWord = tableName;
    const KKSCategory * tc = cat;//io->category()->tableCategory();
    bool isXml = cat->isAttrTypeContains (KKSAttrType::atXMLDoc) || cat->isAttrTypeContains (KKSAttrType::atSVG);
    KKSMap<int, KKSCategoryAttr *>::const_iterator pca;
    for (pca=tc->attributes().constBegin(); pca != tc->attributes().constEnd(); pca++)
    {
        KKSCategoryAttr *a = pca.value();
        if(a->code() == "id")
            continue;
        if(a->code() == "unique_id")
            continue;
        if(a->code() == "last_update")
            continue;
        if(a->code() == "id_io_state")
            continue;
        if(a->code() == "uuid_t"){
            if(tableName.toLower() != "io_objects" && tableName.toLower() != "tbl_io_objects")//в справочнике ИО есть атрибут uuit_t
                continue;
        }
        if(a->code() == "r_icon"){ 
            if(tableName.toLower() != "rubricator") //в справочнике "общесистемный рубрикатор" имеется колонка r_icon, но он при этом является системным, т.е. isSys = true
                continue;
        }
        if(a->code() == "record_fill_color")
            continue;
        if(a->code() == "record_text_color")
            continue;
        
        if(a->type()->attrType() == KKSAttrType::atCheckListEx)
            continue;

        QString refColumnName = a->refColumnName(true);
        if(refColumnName.isEmpty())
            refColumnName = "id";

        QString code = a->code(true); //quoted code
        QString codeFK = code.insert(code.size()-1, "_fk");
        code = a->code(true);
        QString codeUID = code.insert(code.size()-1, "_uid");
        code = a->code(true);

        if(a->type()->attrType() == KKSAttrType::atList ||
           a->type()->attrType() == KKSAttrType::atParent ||
           a->type()->attrType() == KKSAttrType::atRecordColorRef ||
           a->type()->attrType() == KKSAttrType::atRecordTextColorRef)
        {
            QString tName = a->tableName();
            QString cName = a->columnName(true); //quoted columnName
            
            if(a->type()->attrType() == KKSAttrType::atParent){
                tName = tableName;
                defParentFilter = QString("%1.%2 isnull").arg(tName).arg(code);
                withAttrPK = refColumnName;
                withAttrFK = code;
            }

            KKSObject * refObj = this->loadIO (tName, true);
            if (!refObj)
                continue;

            KKSCategory * cRef = refObj->category();
            if (!cRef)
            {
                refObj->release ();
                continue;
            }
            cRef = cRef->tableCategory();
            if (cRef)
                isXml = isXml || cRef->isAttrTypeContains (KKSAttrType::atXMLDoc) || cRef->isAttrTypeContains (KKSAttrType::atSVG);
            refObj->release ();
            
            if(tName.isEmpty() || cName.isEmpty()){
                qWarning() << "Corrupt attribute in IO! It MUST be reference to table and column but they are NULL!";
                return QString::null;
            }
            
            //если ссылка на одну и ту же таблицу используется в таблице tableName 
            //несколько раз, то пересечение надо делать несколько раз с этой таблицей
            //используя алиасы.
            int cnt = usedTableNames.count(tName);
            usedTableNames << tName;

            if(cnt > 0){
                fromWord += QString(", %1 %1%2").arg(tName).arg(cnt);
                if(!a->isMandatory()){
                    whereWord += QString(" and (%1.%2 isnull or %1.%2 = %3%4.%5) ").arg(tableName).arg(code).arg(tName).arg(cnt).arg(refColumnName);
                    joinWord += QString(" left join %1 %1%2 on %3.%4 = %1%2.%5").arg(tName).arg(cnt).arg(tableName).arg(code).arg(refColumnName);
                    withJoinWord += QString(" left join %1 %1%2 on %3.%4 = %1%2.%5").arg(tName).arg(cnt).arg(withTableName).arg(code).arg(refColumnName);
                    attrs += QString(", case when %1.%3 isnull then NULL else %4%5.%2 end as %3").arg(tableName).arg(cName).arg(code).arg(tName).arg(cnt);
                    attrs += QString(", %1.%2 as %3").arg(tableName).arg(code).arg(codeFK);//добавляем служебное поле ИД
                    attrs += QString(", case when %1.%3 isnull then NULL else %4%5.%2 end as %6").arg(tableName).arg("unique_id").arg(code).arg(tName).arg(cnt).arg(codeUID);//добавляем служебное поле unique_id
                    withAttrs += QString(", case when %1.%3 isnull then NULL else %4%5.%2 end as %3").arg(withTableName).arg(cName).arg(code).arg(tName).arg(cnt);
                    withAttrs += QString(", %1.%2 as %3").arg(withTableName).arg(code).arg(codeFK);//добавляем служебное поле ИД
                    withAttrs += QString(", case when %1.%3 isnull then NULL else %4%5.%2 end as %6").arg(withTableName).arg("unique_id").arg(code).arg(tName).arg(cnt).arg(codeUID);//добавляем служебное поле unique_id
                }
                else{
                    whereWord += QString(" and %1.%2 = %3%4.%5 ").arg(tableName).arg(code).arg(tName).arg(cnt).arg(refColumnName);
                    joinWord += QString(" left join %1 %1%2 on %3.%4 = %1%2.%5").arg(tName).arg(cnt).arg(tableName).arg(code).arg(refColumnName);
                    withJoinWord += QString(" left join %1 %1%2 on %3.%4 = %1%2.%5").arg(tName).arg(cnt).arg(withTableName).arg(code).arg(refColumnName);
                    attrs += QString(", %1%4.%2 as %3").arg(tName).arg(cName).arg(code).arg(cnt);
                    attrs += QString(", %1.%2 as %3").arg(tableName).arg(code).arg(codeFK);
                    attrs += QString(", %1%4.%2 as %3").arg(tName).arg("unique_id").arg(codeUID).arg(cnt);
                    withAttrs += QString(", %1%4.%2 as %3").arg(tName).arg(cName).arg(code).arg(cnt);
                    withAttrs += QString(", %1.%2 as %3").arg(withTableName).arg(code).arg(codeFK);
                    withAttrs += QString(", %1%4.%2 as %3").arg(tName).arg("unique_id").arg(codeUID).arg(cnt);
                }
            }
            else{
                fromWord += QString(", %1 ").arg(tName);
                if(!a->isMandatory()){
                    whereWord += QString(" and (%1.%2 isnull or %1.%2 = %3.%4) ").arg(tableName).arg(code).arg(tName).arg(refColumnName);
                    joinWord += QString(" left join %1 on %2.%3 = %1.%4").arg(tName).arg(tableName).arg(code).arg(refColumnName);
                    withJoinWord += QString(" left join %1 on %2.%3 = %1.%4").arg(tName).arg(withTableName).arg(code).arg(refColumnName);
                    attrs += QString(", case when %1.%3 isnull then NULL else %4.%2 end as %3").arg(tableName).arg(cName).arg(code).arg(tName);
                    attrs += QString(", %1.%2 as %3").arg(tableName).arg(code).arg(codeFK);
                    attrs += QString(", case when %1.%3 isnull then NULL else %4.%2 end as %5").arg(tableName).arg("unique_id").arg(code).arg(tName).arg(codeUID);
                    withAttrs += QString(", case when %1.%3 isnull then NULL else %4.%2 end as %3").arg(withTableName).arg(cName).arg(code).arg(tName);
                    withAttrs += QString(", %1.%2 as %3").arg(withTableName).arg(code).arg(codeFK);
                    withAttrs += QString(", case when %1.%3 isnull then NULL else %4.%2 end as %5").arg(withTableName).arg("unique_id").arg(code).arg(tName).arg(codeUID);
                }
                else{
                    whereWord += QString(" and %1.%2 = %3.%4 ").arg(tableName).arg(code).arg(tName).arg(refColumnName);
                    joinWord += QString(" left join %1 on %2.%3 = %1.%4").arg(tName).arg(tableName).arg(code).arg(refColumnName);
                    withJoinWord += QString(" left join %1 on %2.%3 = %1.%4").arg(tName).arg(withTableName).arg(code).arg(refColumnName);
                    attrs += QString(", %1.%2 as %3").arg(tName).arg(cName).arg(code);
                    attrs += QString(", %1.%2 as %3").arg(tableName).arg(code).arg(codeFK);
                    attrs += QString(", %1.%2 as %3").arg(tName).arg("unique_id").arg(codeUID);
                    withAttrs += QString(", %1.%2 as %3").arg(tName).arg(cName).arg(code);
                    withAttrs += QString(", %1.%2 as %3").arg(withTableName).arg(code).arg(codeFK);
                    withAttrs += QString(", %1.%2 as %3").arg(tName).arg("unique_id").arg(codeUID);
                }
            }
            attrsWith += QString(", %1").arg(code);
            attrsWith1 += QString(", %1.%2").arg(tableName).arg(code);

        }
        else if(a->type()->attrType() == KKSAttrType::atJPG || 
                a->type()->attrType() == KKSAttrType::atSVG ||
                a->type()->attrType() == KKSAttrType::atVideo ||
                a->type()->attrType() == KKSAttrType::atXMLDoc )
        {
            //необходимо опустить содержимое атрибутов типа JPG и SVG, поскольку они сильно тормозят систему
            //и к тому же в список ЭИО не выводятся
            if(tableName.toLower() != "io_objects" && tableName.toLower() != "rubricator"){
                attrs += QString(", 'pixmap/svg/video/xml data type' as %1").arg(code);
                withAttrs += QString(", 'pixmap/svg/video/xml data type' as %1").arg(code);
                attrsWith += QString(", xml_data_type_%1").arg(a->idRow());
                attrsWith1 += QString(", 'pixmap/svg/video/xml data type' as %1").arg(code);
            }
            else{
                if(a->type()->attrType() == KKSAttrType::atJPG){
                    attrs += QString(", %1.%2").arg(tableName).arg(code);
                    withAttrs += QString(", %1.%2").arg(withTableName).arg(code);
                    attrsWith += QString(", %1").arg(code);
                    attrsWith1 += QString(", %1.%2").arg(tableName).arg(code);
                }
            }

            
        }
        else{
            attrs += QString(", %1.%2").arg(tableName).arg(code);
            withAttrs += QString(", %1.%2").arg(withTableName).arg(code);
            attrsWith += QString(", %1").arg(code);
            attrsWith1 += QString(", %1.%2").arg(tableName).arg(code);
        }
    }
    
    //обходной маневр для ТСД если делаем выборку из таблицы tsd, то надо вызывать специальную функцию
    QStringList exTables;
    if(tableName.toLower() == "tsd"){
        QString filterWhere = generateFilterSQL(filters, tableName, exTables);
        if(!filterWhere.isEmpty())
            sql = QString("select * from jGetTsd('%1');").arg(filterWhere);
        else
            sql = QString("select * from jGetTsd(NULL);");
    }
    else{
        QString parentFilter;
        
        bool withRecursive = false;
        if(cat->id() == CAT_CATEGORY_ID || cat->id() == CAT_TABLE_CATEGORY_ID)
            withRecursive = false;
        else
            withRecursive = cat->isAttrTypeContains(KKSAttrType::atParent);
        
        //for()
        //if(!withRecursive){
        //
        //}

        QString filterWhere;
        if(withRecursive)
            filterWhere = generateFilterSQL(filters, withTableName, exTables);//exTables - таблицы с отношением M:N
        else
            filterWhere = generateFilterSQL(filters, tableName, exTables);//exTables - таблицы с отношением M:N

        QString exTablesStr;
        int cnt = exTables.count();
        if(cnt > 0){
            for(int i=0; i<cnt; i++){
                exTablesStr += ", " + exTables.at(i);
            }
        }
        
        if(!withRecursive){
            sql = QString("select %4 %1.id, %1.unique_id, %1.last_update %6 %2 from %1 %3 %5 where 1=1 ")
                                .arg(tableName)
                                .arg(attrs)
                                .arg(joinWord)
                                .arg (isXml ? QString() : QString("distinct"))
                                .arg(exTablesStr)
                                .arg(isSys ? QString("") : QString(", %1.uuid_t, %1.id_io_state, %1.r_icon, %1.record_fill_color, %1.record_text_color ").arg(tableName));//.arg(fromWord).arg(joinWord);//.arg(whereWord);
            
            if(!filterWhere.isEmpty())
                sql += filterWhere;
        }
        else{
            //QString attrPK;
            //QString attrFK;
            
            if(parentFilter.isEmpty())
                parentFilter = defParentFilter;

            sql = QString("with recursive %1 (%2, ii_rec_order) as (" 
                          "select %3 %4, 0 from %5 where %6 UNION ALL select %4, %1.ii_rec_order+1 from %1, %5 where %1.%7 = %5.%8"
                          ")"
                          "select %3 %1.id, %1.unique_id, %1.last_update %12 %9, %1.ii_rec_order from %1 %10 %11 where 1=1 ")
                              .arg(withTableName)
                              .arg(attrsWith)
                              .arg (isXml ? QString() : QString("distinct"))
                              .arg(attrsWith1)
                              .arg(tableName)
                              .arg(parentFilter)
                              .arg(withAttrPK)
                              .arg(withAttrFK)
                              .arg(withAttrs)
                              .arg(withJoinWord)
                              .arg(exTablesStr)
                              .arg(isSys ? QString("") : QString(", %1.uuid_t, %1.id_io_state, %1.r_icon, %1.record_fill_color, %1.record_text_color").arg(withTableName));
            if(!filterWhere.isEmpty())
                sql += filterWhere;
            
            sql += QString(" order by %1.ii_rec_order").arg(withTableName);
        }
    }

    qWarning()  << __PRETTY_FUNCTION__ << sql << "\n\n\n";

    return sql;
}

KKSMap<qint64, KKSEIOData *> KKSLoader::loadEIOList(const KKSObject * io, 
                                                 const KKSList<const KKSFilterGroup *>& filters) const
{
    KKSMap<qint64, KKSEIOData *> eioList;
    if(!io || 
       !io->id() || 
       !io->category() || 
       !io->category()->id() || 
       !io->category()->tableCategory() ||
       !io->category()->tableCategory()->id()
       )
    {
        return eioList;
    }
    eioList = loadEIOList (io->category()->tableCategory(), io->tableName(), filters, io->id() <= _MAX_SYS_IO_ID_ ? true : false);
    
    return eioList;
}

KKSList<KKSEIOData *> KKSLoader::loadEIOList1(const KKSObject * io, 
                                              const KKSList<const KKSFilterGroup *>& filters) const
{
    KKSList<KKSEIOData *> eioList;
    if(!io || 
       !io->id() || 
       !io->category() || 
       !io->category()->id() || 
       !io->category()->tableCategory() ||
       !io->category()->tableCategory()->id()
       )
    {
        return eioList;
    }
    eioList = loadEIOList1 (io->category()->tableCategory(), io->tableName(), filters, io->id() <= _MAX_SYS_IO_ID_ ? true : false);
    
    return eioList;
}

KKSEIOData * KKSLoader::loadEIOInfo (int idObject, int idRec) const
{
    KKSObject * io = loadIO (idObject, true);
    if (!io ||
        !io->id() || 
        !io->category() || 
        !io->category()->id() || 
        !io->category()->tableCategory() ||
        !io->category()->tableCategory()->id()
    )
    {
        return 0;
    }
    
    const KKSCategory * ct = io->category()->tableCategory();
    const KKSFilter * f = ct->createFilter(1, QString("select id from %1 where id=%2").arg(io->tableName()).arg(idRec), KKSFilter::foInSQL);
    KKSList<const KKSFilter*> filters;
    filters.append(f);
    f->release ();
    KKSFilterGroup * fg = new KKSFilterGroup(true);
    fg->setFilters (filters);
    KKSList<const KKSFilterGroup *> fList;
    fList.append(fg);
    fg->release ();
    KKSMap<qint64, KKSEIOData *> recInfoList = loadEIOList (io, fList);
    KKSEIOData * recInfo = recInfoList.value(idRec);
    if (recInfo)
        recInfo->addRef ();
    recInfoList.clear();
    io->release ();
    return recInfo;
}

KKSMap<qint64, KKSEIOData *> KKSLoader::loadEIOList(const KKSCategory * c0, 
                                                 const QString& tableName,
                                                 const KKSList<const KKSFilterGroup *>& filters,
                                                 bool isSys) const
{
    KKSMap<qint64, KKSEIOData *> eioList;

    QString sql = generateSelectEIOQuery(c0, tableName, filters, isSys);
    qDebug () << __PRETTY_FUNCTION__ << sql;
    if(sql.isEmpty())
        return eioList;

    KKSResult * res = db->execute(sql);
    int count = 0;
    if(!res || (count = res->getRowCount()) == 0){
        if(res)
            delete res;
        return eioList;
    }
    
    //проверяем, содержит ли категория ИО атрибут с типом интервал или интервал (часы, мин., сек.)
    bool hasInterval = false;
    KKSCategoryAttr * a = NULL;
    const KKSCategory * c = c0;//io->category()->tableCategory();
    KKSMap<int, KKSCategoryAttr *>::const_iterator pca;
    for (pca = c->attributes().constBegin(); pca != c->attributes().constEnd(); pca++)
    {
        a = pca.value();
        if(a && 
            (a->type()->attrType() == KKSAttrType::atInterval ||
             a->type()->attrType() == KKSAttrType::atIntervalH) 
           )
        {
            hasInterval = true;
            break;
        }
    }

    int columns = res->getColumnCount();

    for(int row=0; row<count; row++){
        
        KKSEIOData * eio = new KKSEIOData();
        
        for(int column=0; column<columns; column++){
            QString code = QString(res->getColumnName(column));//использование названия колонки для ключа QMap в классе KKSEIOData допустимо
            QString value = res->getCellAsString(row, column);
            
            if(code == "ii_rec_order" || 
               code == "id" || 
               code == "unique_id" || 
               code == "last_update" || 
               code == "id_io_state" || 
               code == "uuid_t" ||
               code == "r_icon" || 
               code == "record_fill_color" || 
               code == "record_text_color")
            {
                eio->addSysField(code, value);
            }
//            else if(code == "uuid_t" && tableName.toLower() != "io_objects" && tableName.toLower() != "tbl_io_objects"){
//                eio->addSysField(code, value);
//            }

            //в данном случае получение атрибута по его коду (хотя он и не является уникальным) 
            //допустимо, поскольку категория описывает таблицу, а в таблице не может быть 
            //двух колонок (атрибутов) с одинаковым названием                    
            const KKSCategoryAttr* a = c->attribute(code);
            if(!a)
                continue;
            

            //проверим на тип ИНТЕРВАЛ
            if(hasInterval){
                if(res->getColumnDataType(column) == KKSResult::dtInt4Array){
                    const KKSCategoryAttr* a = c->attribute(code);
                    if(a && a->type()->attrType() == KKSAttrType::atInterval){
                        KKSValue v;
                        v.setValue(value, KKSAttrType::atInterval);
                        value = v.value();
                    }
                    if(a && a->type()->attrType() == KKSAttrType::atIntervalH){
                        KKSValue v;
                        v.setValue(value, KKSAttrType::atIntervalH);
                        value = v.value();
                    }
                }
            }

            if(res->getColumnDataType(column) == KKSResult::dtBool){
                QString s = value.toLower();
                if(s == "t" || s == "true" || s == "1")
                    value = QObject::tr("Yes");
                else
                    value = QObject::tr("No");
            }

            if(res->getColumnDataType(column) == KKSResult::dtTimestamp){
                KKSValue v(value, KKSAttrType::atDateTime);
                QDateTime dt = v.valueVariant().toDateTime();
                value = dt.toString("dd.MM.yyyy hh:mm:ss");
            }

            int ier = 0;
            if(a->type()->attrType() == KKSAttrType::atList ||
                a->type()->attrType() == KKSAttrType::atParent
                )
            {
                QString sysValue = res->getCellAsString(row, ++column);
                QString sysValue1 = res->getCellAsString(row, ++column);
                ier = eio->addField(code, value);
                ier = eio->addSysField(code, sysValue);
                ier = eio->addSysField(code+"_uid", sysValue1);
            }
            else            
                ier = eio->addField(code, value);

            if (ier != OK_CODE)
            {
                qDebug () << __PRETTY_FUNCTION__ << QObject::tr ("Error");
                eio->release ();
                return eioList;
            }
        }

        //ID всегда идет первым в списке
        qint64 id = res->getCellAsInt64(row, 0);
        eioList.insert(id, eio);
        eio->release();
    }
    
    delete res;
    
    return eioList;
}

KKSList<KKSEIOData *> KKSLoader::loadEIOList1(const KKSCategory * c0, 
                                              const QString& tableName,
                                              const KKSList<const KKSFilterGroup *>& filters,
                                              bool isSys) const
{
    KKSList<KKSEIOData *> eioList;

    QString sql = generateSelectEIOQuery(c0, tableName, filters, isSys);
    qDebug () << __PRETTY_FUNCTION__ << sql;
    if(sql.isEmpty())
        return eioList;

    KKSResult * res = db->execute(sql);
    int count = 0;
    if(!res || (count = res->getRowCount()) == 0){
        if(res)
            delete res;
        return eioList;
    }
    
    //проверяем, содержит ли категория ИО атрибут с типом интервал или интервал (часы, мин., сек.)
    bool hasInterval = false;
    KKSCategoryAttr * a = NULL;
    const KKSCategory * c = c0;//io->category()->tableCategory();
    KKSMap<int, KKSCategoryAttr *>::const_iterator pca;
    for (pca = c->attributes().constBegin(); pca != c->attributes().constEnd(); pca++)
    {
        a = pca.value();
        if(a && 
            (a->type()->attrType() == KKSAttrType::atInterval ||
             a->type()->attrType() == KKSAttrType::atIntervalH) 
           )
        {
            hasInterval = true;
            break;
        }
    }

    int columns = res->getColumnCount();

    for(int row=0; row<count; row++){
        
        KKSEIOData * eio = new KKSEIOData();
        
        for(int column=0; column<columns; column++){
            QString code = QString(res->getColumnName(column));//использование названия колонки для ключа QMap в классе KKSEIOData допустимо
            QString value = res->getCellAsString(row, column);
            
            if(code == "ii_rec_order" || 
               code == "id" || 
               code == "unique_id" || 
               code == "last_update" || 
               code == "uuid_t" || 
               code == "id_io_state" || 
               code == "r_icon" || 
               code == "record_fill_color" || 
               code == "record_text_color")
            {
                eio->addSysField(code, value);
            }

            //в данном случае получение атрибута по его коду (хотя он и не является уникальным) 
            //допустимо, поскольку категория описывает таблицу, а в таблице не может быть 
            //двух колонок (атрибутов) с одинаковым названием                    
            const KKSCategoryAttr* a = c->attribute(code);
            if(!a)
                continue;
            

            //проверим на тип ИНТЕРВАЛ
            if(hasInterval){
                if(res->getColumnDataType(column) == KKSResult::dtInt4Array){
                    const KKSCategoryAttr* a = c->attribute(code);
                    if(a && a->type()->attrType() == KKSAttrType::atInterval){
                        KKSValue v;
                        v.setValue(value, KKSAttrType::atInterval);
                        value = v.value();
                    }
                    if(a && a->type()->attrType() == KKSAttrType::atIntervalH){
                        KKSValue v;
                        v.setValue(value, KKSAttrType::atIntervalH);
                        value = v.value();
                    }
                }
            }

            if(res->getColumnDataType(column) == KKSResult::dtBool){
                QString s = value.toLower();
                if(s == "t" || s == "true" || s == "1")
                    value = QObject::tr("Yes");
                else
                    value = QObject::tr("No");
            }

            if(res->getColumnDataType(column) == KKSResult::dtTimestamp){
                KKSValue v(value, KKSAttrType::atDateTime);
                QDateTime dt = v.valueVariant().toDateTime();
                value = dt.toString("dd.MM.yyyy hh:mm:ss");
            }

            int ier = 0;
            if(a->type()->attrType() == KKSAttrType::atList ||
                a->type()->attrType() == KKSAttrType::atParent
                )
            {
                QString sysValue = res->getCellAsString(row, ++column);
                QString sysValue1 = res->getCellAsString(row, ++column);
                ier = eio->addField(code, value);
                ier = eio->addSysField(code, sysValue);
                ier = eio->addSysField(code+"_uid", sysValue1);
            }
            else            
                ier = eio->addField(code, value);

            if (ier != OK_CODE)
            {
                qDebug () << __PRETTY_FUNCTION__ << QObject::tr ("Error");
                eio->release ();
                return eioList;
            }
        }

        //ID всегда идет первым в списке
        //qint64 id = res->getCellAsInt64(row, 0);
        eioList.append(eio);
        eio->release();
    }
    
    delete res;
    
    return eioList;
}

// /*
QString KKSLoader::generateFilterSQL(const KKSList<const KKSFilterGroup *> & filterGroups, 
                                     const QString & tableName, QStringList & exTables) const
{
    QString sql;
    QString tmp;

    if(filterGroups.empty())
        return sql;
    
    //QString tableName;
    
    //qWarning() << "\n\n\ngenerateFilterSQL: groupsCount = " << filterGroups.count() << "\n\n\n";
    
    for (int i=0; i<filterGroups.count(); i++)
    {
        const KKSFilterGroup * group = filterGroups[i];
        if(!group)
            continue;

        tmp = parseGroup(group, tableName, exTables);
        if(tmp.isEmpty())
            continue;

        if(i==0)
            sql += tmp;
        else if(!tmp.isEmpty())
            sql += " and " + tmp;
    }
    
    if(!sql.isEmpty())
        sql.prepend(" and ");

    //qWarning() << "generateFilterSQL: sql = " << sql;

    return sql; //ui->teSQLView->setText(sql);
}

QString KKSLoader::parseGroup(const KKSFilterGroup * g, const QString & tableName, QStringList & exTables) const
{
    QString sql;
    QString tmp;
    
    if(!g)
        return sql;

    QString groupOper;
    
    tmp += " ( ";
    if(g->AND()){
        groupOper = " AND ";
    }
    else{
        groupOper = " OR ";
    }

    int grCount=0;
    const KKSList<const KKSFilterGroup*> groups = g->groups();
    if(groups.count() > 0)
    {
        for (int i=0; i<groups.count(); i++)
        {
            const KKSFilterGroup * group = groups[i];
            if(!group)
                continue;

            //if(grCount == 0)
            tmp += parseGroup(group, tableName, exTables);
            //else 
            //    tmp += groupOper + " " + parseGroup(group, tableName);
            
            if(tmp.isEmpty())
                continue;

            if(grCount==0)
                sql = tmp;
            else
                sql += groupOper + tmp;
            
            grCount++;
            tmp.clear();
        }
    }
    
    tmp.clear();
    if(grCount==0){
        tmp = " ( ";
    }
    else{
        tmp = " " + groupOper + " ( ";
    }
    
    const KKSList<const KKSFilter*> filters = g->filters();
    if(filters.count() <= 0){
        if(grCount <= 0){
            return sql;
        }
        else{
            sql += " ) ";
        }
        return sql;
    }

    int fCount;
    for (fCount=0; fCount < filters.count(); fCount++)
    {
        const KKSFilter * f = filters[fCount];
        if(!f || !f->isCorrect()){
            qWarning() << "KKSFilter is not correct!";
            continue;
        }

        if(fCount==0)
            tmp += parseFilter(f, tableName, exTables);
        else 
            tmp += groupOper + parseFilter(f, tableName, exTables);

    }
    
    //  tmp += " ) ";
    
    if(fCount>0)
        sql += tmp + " ) ";
    else
        sql += " ) ";
    
    if(grCount > 0)
        sql += " ) ";
    
    return sql;
}

QString KKSLoader::parseFilter(const KKSFilter * f, const QString & tableName, QStringList & exTables) const
{
    QString sql;
    
    if(!f)
        return sql;

    const KKSAttribute * a = f->attribute();
    KKSList<const KKSValue *> values = f->values();
    KKSFilter::FilterOper oper = f->operation();
    bool cs = f->caseSensitive();
    
    //QString upper = QString(" upper(");
    //QString endUpper = QString(")");
    
    if(a->type()->attrType() != KKSAttrType::atString && 
        a->type()->attrType() != KKSAttrType::atFixString && 
        a->type()->attrType() != KKSAttrType::atText)
    {
        cs = true;
    }

    //Для случая если используется оператор LIKE
    QString sLike;
    QString aVal; //сюда потом будем добавлять символ %, если необходимо
    if(cs == true)
        sLike = "LIKE";
    else
        sLike = "ILIKE";

    QString code = a->code(true);

    if(tableName.isEmpty())
        sql += QString(" %1 ").arg(code);
    else if (a->type()->attrType() == KKSAttrType::atCheckListEx)
    {
        QString refTable = QString("%1_%2_ref_%3")                
                .arg (tableName)
                .arg (a->tableName())
                .arg (a->id());
        
        QString attrTable = a->tableName();

        if(!exTables.contains(refTable))
            exTables.append(refTable);

        if(!exTables.contains(attrTable))
            exTables.append(attrTable);

        QString sqlIn = values.at(0)->valueForInsert().mid (2, values.at(0)->valueForInsert().length()-4);
        if (!sqlIn.trimmed().isEmpty())
        {
            sql += QString(" %1.id=%1_%2_ref_%3.id_%1 and %1_%2_ref_%3.id_%2=%2.id and %2.id in (%4) ")
                    .arg (tableName)
                    .arg (a->tableName())
                    .arg (a->id())
                    .arg (sqlIn);
        }
        else
        {
            sql += QString(" %1.id=%1_%2_ref_%3.id_%1 and %1_%2_ref_%3.id_%2=%2.id  ")
                    .arg (tableName)
                    .arg (a->tableName())
                    .arg (a->id());
        }

/*
        sqlEx += QString (" inner join %1_%2_ref_%3 on (%1.id=%1_%2_ref_%3.id_%1) inner join %2 on (%1_%2_ref_%3.id_%2=%2.id and %2.id in(%4))")
                .arg (tableName)
                .arg (a->tableName())
                .arg (a->id())
                .arg (values.at(0)->valueForInsert().mid (2, values.at(0)->valueForInsert().length()-4));
*/
        /*        
        sql += QString (" %1 inner join %1_%2_ref_%3 on (%1.id=%1_%2_ref_%3.id_%1) inner join %2 on (%1_%2_ref_%3.id_%2=%2.id and %2.id in(%4))")
                .arg (tableName)
                .arg (a->tableName())
                .arg (a->id())
                .arg (values.at(0)->valueForInsert().mid (2, values.at(0)->valueForInsert().length()-4));
*/


        //sql += endUpper;
        qDebug () << __PRETTY_FUNCTION__ << sql;
        return sql;
    }
    else if (a->type()->attrType() == KKSAttrType::atCheckList)
    {
        if(oper == KKSFilter::foIn){
            sql += QString (" %1.%2 @> %3 ")
                                .arg(tableName)
                                .arg(code)
                                .arg (values.at(0)->valueForInsert());
        }
        else if(oper == KKSFilter::foNotIn){
            sql += QString (" not (%1.%2 @> %3) ")
                                .arg(tableName)
                                .arg(code)
                                .arg (values.at(0)->valueForInsert());
        }
        else if(oper == KKSFilter::foEq){
            sql += QString (" ( %1.%2 @> %3 and %3 @> %1.%2 ) ")
                                .arg(tableName)
                                .arg(code)
                                .arg (values.at(0)->valueForInsert());
        }
        else if(oper == KKSFilter::foNotEq){
            sql += QString (" not ( %1.%2 @> %3 and %3 @> %1.%2 ) ")
                                .arg(tableName)
                                .arg(code)
                                .arg (values.at(0)->valueForInsert());
        }
        else if(oper == KKSFilter::foIsNull){
            sql += QString (" %1.%2 is null or array_upper(%1.%2, 1) = 0 ")
                                .arg(tableName)
                                .arg(code);
        }
        else if(oper == KKSFilter::foIsNotNull){
            sql += QString (" %1.%2 is not null and array_upper(%1.%2, 1) > 0 ")
                                .arg(tableName)
                                .arg(code);
        }
        else{
            ;
        }

        //sql += endUpper;
        qDebug () << __PRETTY_FUNCTION__ << sql;
        return sql;
    }
    else
        sql += QString(" %1.%2 ").arg(tableName).arg(code);

    
    //если в качестве атрибута в фильтре используется "ссылка на элемент справочника",
    //необходимо определить, как называется поле, на которое ссылается внешний ключ.
    //это может быть поле, отличное от id
    //в этом случае нам надо из БД подгрузить значение этого поля, поскольку у нас в KKSFilter::values() содержится значение только поля id
    if(a->type()->attrType() == KKSAttrType::atList){
        if(!a->refColumnName().isEmpty() && a->refColumnName() != "id"){
            QString sql = QString("select \"%1\" from %2 where id = %3")
                                    .arg(a->refColumnName())
                                    .arg(a->tableName())
                                    .arg(values.at(0)->valueForInsert());

            KKSResult * res = db->execute(sql);
            if(!res)
                return QString::null;

            int count = res->getRowCount();
            if(count != 1){
                qWarning("The column value does not present in table!");
                delete res;
                return QString::null;
            }

            QString val = res->getCellAsString(0, 0);
            delete res;
            KKSValue * v = const_cast<KKSValue *>(values.at(0));
            v->setValue(val, KKSAttrType::atList);

        }
    }

    QString str;
    switch (oper){
        case KKSFilter::foEq:
            str += QString(" = %1").arg(values.at(0)->valueForInsert());
            break;
        case KKSFilter::foGr:
            str += QString(" > %1").arg(values.at(0)->valueForInsert());
            break;
        case KKSFilter::foLess:
            str += QString(" < %1").arg(values.at(0)->valueForInsert());
            break;
        case KKSFilter::foGrEq:
            str += QString(" >= %1").arg(values.at(0)->valueForInsert());
            break;
        case KKSFilter::foLessEq:
            str += QString(" <= %1").arg(values.at(0)->valueForInsert());
            break;
        case KKSFilter::foNotEq:
            str += QString(" <> %1").arg(values.at(0)->valueForInsert());
            break;
        case KKSFilter::foIsNull:
            str += QString(" is null");
            break;
        case KKSFilter::foIsNotNull:
            str += QString(" is not null");
            break;
        case KKSFilter::foIn:
            if (!f->constructForIn().trimmed().isEmpty())
                str += QString(" in (%1)").arg(f->constructForIn());
            break;
        case KKSFilter::foNotIn:
            if (!f->constructForIn().trimmed().isEmpty())
                str += QString(" not in (%1)").arg(f->constructForIn());
            break;
        case KKSFilter::foInSQL:
            if (!values.at(0)->value().trimmed().isEmpty())
                str += QString(" in (%1)").arg(values.at(0)->value());
            break;
        case KKSFilter::foBetween:
            str += QString(" between %1 and %2")
                        .arg(values.at(0)->valueForInsert())
                        .arg(values.at(1)->valueForInsert());
            break;
        case KKSFilter::foLike:
            str += QString(" %1 %2").arg(sLike).arg(values.at(0)->valueForInsert());
            break;
        case KKSFilter::foLikeIn:
            str += QString(" %1 %2").arg(sLike).arg("'%" + values.at(0)->value() + "%'");
            break;
        case KKSFilter::foLikeStart:
            str += QString(" %1 %2").arg(sLike).arg("'" + values.at(0)->value() + "%'");
            break;
        case KKSFilter::foLikeEnd:
            str += QString(" %1 %2").arg(sLike).arg("'%" + values.at(0)->value() + "'");
            break;
        default:
            break;
    }

    sql += str;

    return sql;
}


// */

/*--ksa
QString KKSLoader::generateFilterSQL(const KKSList<const KKSFilterGroup *> & filterGroups, 
                                     const QString & tableName) const
{
    QString sql;// = QString(" and 1=1 ");

    if(filterGroups.empty())
        return sql;

    
    for (int i=0; i<filterGroups.count(); i++)
    {
        const KKSFilterGroup * group = filterGroups[i];
        if(!group)
            continue;
        const KKSList<const KKSFilter*> filters = group->filters();
        if(filters.count() == 0)
            continue;

        QString groupOper;
        if(group->AND()){
            groupOper = "AND";
            sql += " and ( 1=1 ";
        }
        else{
            groupOper = "OR";
            sql += " and ( 1=0 ";
        }

        for (int i=0; i<filters.count(); i++)
        {
            const KKSFilter * f = filters[i];
            if(!f || !f->isCorrect())
            {
                qWarning() << "KKSFilter is not correct!";
                continue;
            }
            const KKSAttribute * a = f->attribute();
            KKSList<const KKSValue *> values = f->values();
            KKSFilter::FilterOper oper = f->operation();
            QString code = a->code();
            QString str = QString(" %3 %1.%2 ").arg(tableName).arg(code).arg(groupOper);
            switch (oper){
                case KKSFilter::foEq:
                    str += QString(" = %1").arg(values.at(0)->valueForInsert());
                    break;
                case KKSFilter::foGr:
                    str += QString(" > %1").arg(values.at(0)->valueForInsert());
                    break;
                case KKSFilter::foLess:
                    str += QString(" < %1").arg(values.at(0)->valueForInsert());
                    break;
                case KKSFilter::foGrEq:
                    str += QString(" >= %1").arg(values.at(0)->valueForInsert());
                    break;
                case KKSFilter::foLessEq:
                    str += QString(" <= %1").arg(values.at(0)->valueForInsert());
                    break;
                case KKSFilter::foNotEq:
                    str += QString(" <> %1").arg(values.at(0)->valueForInsert());
                    break;
                case KKSFilter::foIsNull:
                    str += QString(" is null");
                    break;
                case KKSFilter::foIsNotNull:
                    str += QString(" is not null");
                    break;
                case KKSFilter::foIn:
                    str += QString(" in (%1)").arg(f->constructForIn());
                    break;
                case KKSFilter::foInSQL:
                    str += QString(" in (%1)").arg(values.at(0)->value());
                    break;
                case KKSFilter::foBetween:
                    str += QString(" between %1 and %2")
                                .arg(values.at(0)->valueForInsert())
                                .arg(values.at(1)->valueForInsert());
                    break;
                case KKSFilter::foLike:
                    str += QString(" like %1").arg(values.at(0)->valueForInsert());
                    break;
                default:
                    break;
            }

            sql += str;
        }
        sql += " ) ";
    }

    return sql;
}
--ksa*/

int KKSLoader::getDlId() const
{
    if(!db)
        return ERROR_CODE;

    KKSResult * res = db->execute("select getCurrentDl();");
    if(!res || res->getRowCount() != 1){
        if(res)
            delete res;
        return ERROR_CODE;
    }

    int id = res->getCellAsInt(0, 0);
    delete res;

    return id;
}

int KKSLoader::getUserId(void) const
{
    if(!db)
        return ERROR_CODE;

    if(db->getCurrentUser() > 0)
        return db->getCurrentUser();
    
    KKSResult * res = db->execute("select getCurrentUser();");
    if(!res || res->getRowCount() != 1)
    {
        if(res)
            delete res;
        return ERROR_CODE;
    }

    int id = res->getCellAsInt (0, 0);
    delete res;

    db->setCurrentUser(id);

    return id;
}

QString KKSLoader::getDlName() const
{
    int id = getDlId();
    if(id == ERROR_CODE)
        return QString::null;

    QString sql = QString("select * from getDl(%1)").arg(id);

    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() != 1)
    {
        if(res)
            delete res;
        return QString::null;
    }

    QString name = res->getCellAsString (0, 5);
    delete res;

    return name;
}

QString KKSLoader::getUserName() const
{
    int id = getUserId();
    if(id == ERROR_CODE)
        return QString::null;

    QString sql = QString("select fio from users where id = %1").arg(id);

    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() != 1)
    {
        if(res)
            delete res;
        return QString::null;
    }

    QString name = res->getCellAsString (0, 0);
    delete res;

    return name;
}

QString KKSLoader::getCurrMacLabelName() const
{
    QString sql = QString("select getCurrentMacLabelName()");

    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() != 1)
    {
        if(res)
            delete res;
        return QObject::tr("Not defined");
    }

    QString name = res->getCellAsString (0, 0);
    delete res;

    return name;
}

int KKSLoader::getCurrMacLabelId() const
{
    QString sql = QString("select getCurrentMacLabelId()");

    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() != 1)
    {
        if(res)
            delete res;
        return -1;//в этом случае всегда возвращаем неизвестный
    }

    int id = res->getCellAsInt (0, 0);
    delete res;

    return id;
}

bool KKSLoader::canChangeMac(int idMaclabel) const
{
    QString sql = QString("select canChangeMac(%1)").arg(idMaclabel);

    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() != 1)
    {
        if(res)
            delete res;
        return false;
    }

    bool yes = res->getCellAsBool(0, 0);
    delete res;

    return yes;
}

int KKSLoader::setCurrentMaclabel(int idMaclabel) const
{
    QString sql = QString("select setCurrentMaclabel(%1)").arg(idMaclabel);

    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() != 1)
    {
        if(res)
            delete res;
        return 0;
    }

    int yes = res->getCellAsInt(0, 0);
    delete res;

    return yes;
}

KKSList<KKSFile *> KKSLoader::loadFiles(KKSObject * io) const
{
    KKSList<KKSFile *> files;

    if(!io || !io->id())
        return files;

    QString sql = QString("select * from ioGetFiles(%1);").arg(io->id());

    KKSResult * res = db->execute(sql);
    if(!res)
        return files;

    int count = res->getRowCount();
    if(count == 0){
        delete res;
        return files;
    }

    //KKSCategory * c = io->category();

    for(int row=0; row<count; row++){
        
        int idFile = res->getCellAsInt(row, 1);
        QString name = res->getCellAsString(row, 2);
        KKSFile * file = new KKSFile(idFile, name);
        file->setUploaded();
        
        int idFileType = res->getCellAsInt(row, 4);
        QString typeName = res->getCellAsString(row, 5);
        QString appWin = res->getCellAsString(row, 6);
        QString appLin = res->getCellAsString(row, 7);
        KKSFileType type(idFileType, typeName, appWin, appLin);

        file->setType(type);

        file->setSrcExt(res->getCellAsString(row, 8));

        files.append(file);
        file->release();
    }
    
    delete res;

    return files;
}

KKSList<KKSFile *> KKSLoader::loadFiles(const KKSObjectExemplar * eio) const
{
    KKSList<KKSFile *> files;

    if(!eio || !eio->id())
        return files;

    QString sql = QString("select * from recGetFiles(%1::int8);").arg(eio->id());

    KKSResult * res = db->execute(sql);
    if(!res)
        return files;

    int count = res->getRowCount();
    if(count == 0){
        delete res;
        return files;
    }

    for(int row=0; row<count; row++){
        
        int idFile = res->getCellAsInt(row, 1);
        QString name = res->getCellAsString(row, 2);
        KKSFile * file = new KKSFile(idFile, name);
        file->setUploaded();
        
        int idFileType = res->getCellAsInt(row, 4);
        QString typeName = res->getCellAsString(row, 5);
        QString appWin = res->getCellAsString(row, 6);
        QString appLin = res->getCellAsString(row, 7);
        KKSFileType type(idFileType, typeName, appWin, appLin);

        file->setType(type);

        file->setSrcExt(res->getCellAsString(row, 8));

        files.append(file);
        file->release();
    }
    
    delete res;

    return files;
}


KKSList<KKSFileExtention*> KKSLoader::loadFileExtentions(int idFileType) const
{
    KKSList<KKSFileExtention *> extentions;

    QString sql = QString("select id, name from fGetExtentions(%1);").arg(idFileType);

    KKSResult * res = db->execute(sql);
    if(!res)
        return extentions;

    int count = res->getRowCount();
    if(count == 0){
        delete res;
        return extentions;
    }

    for(int row=0; row<count; row++){
        
        int id = res->getCellAsInt(row, 0);
        QString name = res->getCellAsString(row, 1);
        
        KKSFileExtention * e = new KKSFileExtention(id, name);

        extentions.append(e);
        e->release();
    }
    
    delete res;

    return extentions;
}

KKSList<KKSFileType *> KKSLoader::loadFileTypes() const
{
    KKSList<KKSFileType *> fileTypes;

    QString sql = QString("select id, name, win_app, lin_app, extentions from rGetUrlTypes()");

    KKSResult * res = db->execute(sql);
    if(!res)
        return fileTypes;

    int count = res->getRowCount();
    if(count == 0){
        delete res;
        return fileTypes;
    }

    for(int row=0; row<count; row++){
        
        int idFileType = res->getCellAsInt(row, 0);
        QString typeName = res->getCellAsString(row, 1);
        QString appWin = res->getCellAsString(row, 2);
        QString appLin = res->getCellAsString(row, 3);
        
        KKSFileType * type = new KKSFileType(idFileType, typeName, appWin, appLin);

        KKSList<KKSFileExtention*> extentions = loadFileExtentions(idFileType);
        type->setExtentions(extentions);

        fileTypes.append(type);
        type->release();
    }
    
    delete res;

    return fileTypes;
}

void KKSLoader::loadUserTemplates(KKSObject * io) const
{
    if(!io)
        return;

    int idObject = io->id();

    if(idObject <= 0)
        return;


    int idUser = getUserId();
    if(idUser <= 0)
        return;

    QString sql = QString("select id_template, type from user_templates where id_io_object = %1 and id_user = %2").arg(idObject).arg(idUser);
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return;
    }

    for(int i=0; i<res->getRowCount(); i++){
        int idTemplate = res->getCellAsInt(i, 0);
        int type = res->getCellAsInt(i, 1);
        KKSTemplate * t = loadTemplate(idTemplate);
        if(!t)
            continue;

        if(type == 0){
            io->setTableTemplate(t);
        }
        else if(type == 1){
            io->setAttrTemplate(t);
        }

        t->release();
    }
}

void KKSLoader::loadRubrics(KKSObject * io) const
{
    if(!io || io->id() <= 0)
        return;

    //QString sql = QString("select * from ioGetRubrics(%1) order by 9,1").arg(io->id());
    QString sql = QString("select * from ioGetRubrics(%1)").arg(io->id());
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return;
    }
    
    KKSRubric * rootRubric = NULL;
    int cnt = res->getRowCount();
    for(int i=0; i<cnt; i++){
        int id = res->getCellAsInt(i, 0);
        
        int idParent = -1;
        if(!res->isEmpty(i, 1))
            idParent = res->getCellAsInt(i, 1);

        int idSearchTemplate = -1;
        KKSSearchTemplate * st = 0;
        if (!res->isEmpty (i, 3))
        {
            idSearchTemplate = res->getCellAsInt (i, 3);
            st = loadSearchTemplate (idSearchTemplate);
        }

        int idCategory = -1;
        KKSCategory * c = 0;
        if (!res->isEmpty (i, 4))
        {
            idCategory = res->getCellAsInt (i, 4);
            c = loadCategory(idCategory);
        }

        QString name = res->getCellAsString(i, 5);
        QString code = res->getCellAsString(i, 6);
        QString desc = res->getCellAsString(i, 7);
        QString icon = res->getCellAsString(i, 11);

        int type = res->getCellAsInt(i, 8);

        if(type == 0){//root rubric
            rootRubric = new KKSRubric(id, name);
            rootRubric->setCode(code);
            rootRubric->setDesc(desc);
            rootRubric->setSearchTemplate (st);
            rootRubric->setCategory(c);
            rootRubric->setIcon(icon);
            
            rootRubric->m_intId = id;//рубрика информационного объекта загружена из БД (используется в операции update класса KKSPPFactory)

            io->setRootRubric(rootRubric);
        }
        else if(type == 1){//subrubrics
            KKSRubric * subRubric = new KKSRubric(id, name);
            subRubric->setCode(code);
            subRubric->setDesc(desc);
            subRubric->setSearchTemplate (st);
            subRubric->setCategory(c);
            subRubric->setIcon(icon);
            subRubric->m_intId = id;

            if(idParent <= 0){
                qWarning() << "Bad subRubric!";
                subRubric->release();
                continue;
            }
            KKSRubric * parent = io->rootRubric()->rubricForId(idParent);
            if(!parent){
                qWarning() << "Bad subRubric! Parent is NULL!";
                subRubric->release();
                continue;
            }
            parent->addRubric(subRubric);
            subRubric->release();
        }
        else if(type == 2){//rubric items
            bool isAutomated = res->getCellAsBool(i, 10);
            KKSRubricItem * item = new KKSRubricItem(id, name, isAutomated);
            item->setIcon(icon);

            if(idParent <= 0){
                qWarning() << "Bad subRubric!";
                item->release();
                continue;
            }
            KKSRubric * parent = io->rootRubric()->rubricForId(idParent);
            if(!parent){
                qWarning() << "Bad subRubric! Parent is NULL!";
                item->release();
                continue;
            }
            parent->addItem(item);
            item->release();
        }

        if (st)
            st->release ();
    }

    if(rootRubric)
        rootRubric->release();
}

void KKSLoader::loadRubrics(KKSCategory * c) const
{
    if(!c || c->id() <= 0)
        return;

    //QString sql = QString("select * from cGetRubrics(%1) order by 5").arg(c->id());
    QString sql = QString("select * from cGetRubrics(%1)").arg(c->id());
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return;
    }
    
    KKSRubric * rootRubric = NULL;
    int cnt = res->getRowCount();
    for(int i=0; i<cnt; i++){
        int id = res->getCellAsInt(i, 0);
        
        int idParent = -1;
        if(!res->isEmpty(i, 1))
            idParent = res->getCellAsInt(i, 1);

        QString name = res->getCellAsString(i, 3);
        //QString code = res->getCellAsString(i, 4);
        //QString desc = res->getCellAsString(i, 5);

        int type = res->getCellAsInt(i, 4);

        if(type == 0){//root rubric
            rootRubric = new KKSRubric(id, name);
            //rootRubric->setCode(code);
            //rootRubric->setDesc(desc);

            c->setRootRubric(rootRubric);
        }
        else if(type == 1){//subrubrics
            KKSRubric * subRubric = new KKSRubric(id, name);
            //subRubric->setCode(code);
            //subRubric->setDesc(desc);
            if(idParent <= 0){
                qWarning() << "Bad subRubric!";
                subRubric->release();
                continue;
            }
            KKSRubric * parent = c->rootRubric() ? c->rootRubric()->rubricForId(idParent) : NULL;
            if(!parent){
                qWarning() << "Bad subRubric! Parent is NULL!";
                subRubric->release();
                continue;
            }
            parent->addRubric(subRubric);
            subRubric->release();
        }
        /*else if(type == 2){//rubric items
            KKSRubricItem * item = new KKSRubricItem(id, name);

            if(idParent <= 0){
                qWarning() << "Bad subRubric!";
                item->release();
                continue;
            }
            KKSRubric * parent = io->rootRubric()->rubricForId(idParent);
            if(!parent){
                qWarning() << "Bad subRubric! Parent is NULL!";
                item->release();
                continue;
            }
            parent->addItem(item);
            item->release();
        }
        */
    }

    if(rootRubric)
        rootRubric->release();
}

void KKSLoader::loadRecRubrics (KKSObjectExemplar * eio) const
{
    if(!eio || eio->id() <= 0)
        return;

    //QString sql = QString("select * from recGetRubrics(%1) order by 5,1").arg(eio->id());
    QString sql = QString("select * from recGetRubrics(%1)").arg(eio->id());
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return;
    }
    
    KKSRubric * rootRubric = NULL;
    int cnt = res->getRowCount();
    for(int i=0; i<cnt; i++){
        int id = res->getCellAsInt(i, 0);
        
        int idParent = -1;
        if(!res->isEmpty(i, 1))
            idParent = res->getCellAsInt(i, 1);

        KKSSearchTemplate * st = 0;
        KKSCategory * c = 0;

        QString name = res->getCellAsString(i, 3);
        QString code = QString();//res->getCellAsString(i, 6);
        QString desc = res->getCellAsString(i, 4);

        int type = res->getCellAsInt(i, 5);

        if(type == 0){//root rubric
            rootRubric = new KKSRubric(id, name);
            rootRubric->setCode(code);
            rootRubric->setDesc(desc);
            rootRubric->setSearchTemplate (st);
            rootRubric->setCategory(c);
            rootRubric->setIcon(res->getCellAsString(i, 6));
            
            rootRubric->m_intId = id;//рубрика информационного объекта загружена из БД (используется в операции update класса KKSPPFactory)

            eio->setRootRubric(rootRubric);
        }
        else if(type == 1){//subrubrics
            KKSRubric * subRubric = new KKSRubric(id, name);
            subRubric->setCode(code);
            subRubric->setDesc(desc);
            subRubric->setSearchTemplate (st);
            subRubric->setCategory(c);
            subRubric->setIcon(res->getCellAsString(i, 6));
            subRubric->m_intId = id;

            if(idParent <= 0){
                qWarning() << "Bad subRubric!";
                subRubric->release();
                continue;
            }
            KKSRubric * parent = eio->rootRubric()->rubricForId(idParent);
            if(!parent){
                qWarning() << "Bad subRubric! Parent is NULL!";
                subRubric->release();
                continue;
            }
            parent->addRubric(subRubric);
            subRubric->release();
        }
        else if(type == 2){//rubric items
            bool isAutomated = false;//res->getCellAsBool(i, 10);
            KKSRubricItem * item = new KKSRubricItem(id, name, isAutomated);
            item->setIcon(res->getCellAsString(i, 6));

            if(idParent <= 0){
                qWarning() << "Bad subRubric!";
                item->release();
                continue;
            }
            KKSRubric * parent = eio->rootRubric()->rubricForId(idParent);
            if(!parent){
                qWarning() << "Bad subRubric! Parent is NULL!";
                item->release();
                continue;
            }
            parent->addItem(item);
            item->release();
        }

    }

    if(rootRubric)
        rootRubric->release();

}

QString KKSLoader::getRecTable (int idRec) const
{
    QString sql = QString ("select getRecordTable(%1);").arg (idRec);
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() != 1)
    {
        if(res)
            delete res;
        return NULL;
    }
    QString tName = res->getCellAsString (0, 0);
    delete res;
    return tName;
}

KKSMap<qint64, KKSEIOData *> KKSLoader::loadRecList (const KKSRubric * r) const
{
    if (!r)
        return KKSMap<qint64, KKSEIOData *>();

    QString sql = QString("select * from recGetRubricItems(%1) order by 5,1").arg(r->id());
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() == 0)
    {
        if(res)
            delete res;
        return KKSMap<qint64, KKSEIOData *>();
    }

    int count = res->getRowCount();
    KKSMap<qint64, KKSEIOData *> results;
    for (int i=0; i<count; i++)
    {
        qint64 idr = res->getCellAsInt (i, 0);
        QString tableName = getRecTable (idr);
        KKSObject * io = this->loadIO (tableName, true);
        
        if (!io)
            continue;
        KKSEIOData * d = this->loadEIOInfo(io->id(), idr);
        results.insert(idr, d);
        d->release();
        io->release();
    }
    return results;
}

KKSRubric * KKSLoader::loadRubricators(bool bOnlyMyDocs) const
{

    int idUser = getUserId();
    //QString sql = QString("select * from getRubricators(%1, %2) order by id,type").arg(idUser).arg(bOnlyMyDocs? "TRUE" : "FALSE");
    QString sql = QString("select * from getRubricators(%1, %2)").arg(idUser).arg(bOnlyMyDocs? "TRUE" : "FALSE");
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return NULL;
    }
    
    KKSRubric * rootRubric = new KKSRubric(-1, "root rubric for all tree");

    int cnt = res->getRowCount();
    for(int i=0; i<cnt; i++){
        int id = res->getCellAsInt(i, 0);
        
        int idParent = -1;
        if(!res->isEmpty(i, 1))
            idParent = res->getCellAsInt(i, 1);
        
        int idSearchTemplate = -1;
        KKSSearchTemplate * st = 0;
        if (!res->isEmpty (i, 3))
        {
            idSearchTemplate = res->getCellAsInt (i, 3);
            st = loadSearchTemplate (idSearchTemplate);
        }
        
        int idCategory = -1;
        KKSCategory * c = 0;
        if (!res->isEmpty (i, 4))
        {
            idCategory = res->getCellAsInt (i, 4);
            c = loadCategory (idCategory);
        }

        QString name = res->getCellAsString(i, 5);
        QString code = res->getCellAsString(i, 6);
        QString desc = res->getCellAsString(i, 7);
        QString icon = res->getCellAsString(i, 11);

        int type = res->getCellAsInt(i, 8);

        if(type == 0){//rubricator
            KKSRubric * theRubric = new KKSRubric(id, name);
            theRubric->setCode(code);
            theRubric->setDesc(desc);
            theRubric->setSearchTemplate (st);
            theRubric->setCategory(c);
            theRubric->setIcon(icon);
            loadPrivileges (theRubric);
            theRubric->m_intId = id;

            theRubric->setParent(rootRubric);
            rootRubric->addRubric(theRubric);
            theRubric->release();
        }
        else if(type == 1){//rubrics
            KKSRubric * subRubric = new KKSRubric(id, name);
            subRubric->setCode(code);
            subRubric->setDesc(desc);
            subRubric->setSearchTemplate (st);
            subRubric->setCategory(c);
            subRubric->setIcon(icon);
            loadPrivileges (subRubric);
            subRubric->m_intId = id;

            if(idParent <= 0){
                qWarning() << "Bad rubric!";
                subRubric->release();
                continue;
            }
            KKSRubric * parent = rootRubric->rubricForId(idParent);
            if(!parent){
                qWarning() << "Bad rubric! Parent is NULL!";
                subRubric->release();
                continue;
            }
            subRubric->setParent (parent);
            parent->addRubric(subRubric);
            subRubric->release();
        }
        else if(type == 2){//rubric items
            bool b = res->getCellAsBool(i, 10);
            KKSRubricItem * item = new KKSRubricItem(id, name, b);
            item->setIcon(icon);

            if(idParent <= 0){
                qWarning() << "Bad subRubric!";
                item->release();
                continue;
            }
            KKSRubric * parent = rootRubric->rubricForId(idParent);
            if(!parent){
                qWarning() << "Bad subRubric! Parent is NULL!";
                item->release();
                continue;
            }
            item->setParent (parent);
            parent->addItem(item);
            item->release();
        }
    }
    
    return rootRubric;
}

KKSRubricBase * KKSLoader::loadCatRubricators(void) const
{
    QString sql = QString("select * from cgetcategoriesforrubricator() order by id;");
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return NULL;
    }
    
    KKSRubricBase * rootRubric = new KKSRubricOthers (-1, QObject::tr("Others"));//KKSRubric(-1, "Categories rubric for all tree");

    int cnt = res->getRowCount();
    for(int i=0; i<cnt; i++){
        int id = res->getCellAsInt(i, 0);
        
        //int idParent = -1;
        
        //KKSSearchTemplate * st = 0;
        
        KKSCategory * c = NULL;//this->loadCategory (id);

        QString name = res->getCellAsString(i, 3);
        QString code = res->getCellAsString(i, 8);
        QString desc = res->getCellAsString(i, 4);
        QString icon = QString();//res->getCellAsString(i, 11);

        //int type = 0;//res->getCellAsInt(i, 8);

        KKSRubric * theRubric = new KKSRubric(id, name);
        theRubric->setCategorized (true);
        theRubric->setCode(code);
        theRubric->setDesc(desc);
        //theRubric->setSearchTemplate (st);
        theRubric->setCategory(c);
        //theRubric->setIcon(icon);
        //loadPrivileges (theRubric);
        theRubric->m_intId = id;
/*        KKSList<const KKSRubricItem*> rItems = loadCatRubricItems (c);
        theRubric->setItems (rItems);
 */
        theRubric->setParent (rootRubric);

        rootRubric->addNode(theRubric);//>addRubric(theRubric);
        theRubric->release();
    }
    
    return rootRubric;
}

KKSList<const KKSRubricItem *> KKSLoader::loadCatRubricItems (const KKSCategory* cat) const
{
    KKSList<const KKSRubricItem *> rItems;
    if (!cat)
        return rItems;

    QString sql = QString("select id,name,r_icon from io_objects io where io.id_io_category=%1;").arg (cat->id());
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;
        return rItems;
    }

    int nr = res->getRowCount();
    for (int i=0; i<nr; i++)
    {
        QString rIconData = res->getCellAsString (i, 2);
        const KKSRubricItem * r = new KKSRubricItem (res->getCellAsInt (i, 0), res->getCellAsString(i, 1), true, rIconData);
//        if (!rIconData.isEmpty())
//            (const_cast<KKSRubricItem *>(r))->setIcon (rIconData);
        if (r)
            rItems.append (r);
    }
    delete res;
    return rItems;
}

KKSRubric * KKSLoader::loadRubric (int idRubr, bool withInherit) const
{
    //QString sql = QString("select * from getRubric(%1, %2) order by id, type").arg(idRubr).arg(withInherit ? QString("TRUE") : QString("FALSE"));
    QString sql = QString("select * from getRubric(%1, %2)").arg(idRubr).arg(withInherit ? QString("TRUE") : QString("FALSE"));

    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return NULL;
    }

    KKSRubric * rootRubric = new KKSRubric(res->getCellAsInt(0, 0), res->getCellAsString(0, 5));
    KKSSearchTemplate * st = 0;
    int idSearchTemplate (-1);
    if (!res->isEmpty (0, 3))
    {
        idSearchTemplate = res->getCellAsInt (0, 3);
        st = loadSearchTemplate (idSearchTemplate);
    }

    rootRubric->setSearchTemplate (st);
    int idCategory = -1;
    KKSCategory * c = 0;
    if (!res->isEmpty (0, 4))
    {
        idCategory = res->getCellAsInt (0, 4);
        c = loadCategory (idCategory);
    }
    rootRubric->setCategory (c);

    QString code = res->getCellAsString(0, 6);
    QString desc = res->getCellAsString(0, 7);
    QString icon = res->getCellAsString(0, 11);
    rootRubric->setCode (code);
    rootRubric->setDesc (desc);
    rootRubric->setIcon (icon);
    int cnt = res->getRowCount();
    
    for(int i=1; i<cnt; i++){
        int id = res->getCellAsInt(i, 0);

        int idParent = -1;
        if(!res->isEmpty(i, 1))
            idParent = res->getCellAsInt(i, 1);

        int idSearchTemplate = -1;
        KKSSearchTemplate * st = 0;
        if (!res->isEmpty (i, 3))
        {
            idSearchTemplate = res->getCellAsInt (i, 3);
            st = loadSearchTemplate (idSearchTemplate);
        }

        int idCategory = -1;
        KKSCategory * c = 0;
        if (!res->isEmpty (i, 4))
        {
            idCategory = res->getCellAsInt (i, 4);
            c = loadCategory (idCategory);
        }

        QString name = res->getCellAsString(i, 5);
        QString code = res->getCellAsString(i, 6);
        QString desc = res->getCellAsString(i, 7);
        QString icon = res->getCellAsString(i, 11);

        int type = res->getCellAsInt(i, 8);

        if(type == 0){//rubricator
            KKSRubric * theRubric = new KKSRubric(id, name);
            theRubric->setCode(code);
            theRubric->setDesc(desc);
            theRubric->setSearchTemplate (st);
            theRubric->setCategory(c);
            theRubric->setIcon(icon);
            loadPrivileges (theRubric);
            theRubric->m_intId = id;

            rootRubric->addRubric(theRubric);
            theRubric->release();
        }
        else if(type == 1){//rubrics
            KKSRubric * subRubric = new KKSRubric(id, name);
            subRubric->setCode(code);
            subRubric->setDesc(desc);
            subRubric->setSearchTemplate (st);
            subRubric->setCategory(c);
            subRubric->setIcon(icon);
            loadPrivileges (subRubric);
            subRubric->m_intId = id;

            if(idParent <= 0){
                qWarning() << "Bad rubric!";
                subRubric->release();
                continue;
            }
            KKSRubric * parent = rootRubric->rubricForId(idParent);
            if(!parent){
                qWarning() << "Bad rubric! Parent is NULL!";
                subRubric->release();
                continue;
            }
            parent->addRubric(subRubric);
            subRubric->release();
        }
        else if(type == 2){//rubric items
            bool b = res->getCellAsBool(i, 10);
            KKSRubricItem * item = new KKSRubricItem(id, name, b);
            item->setIcon(icon);

            if(idParent <= 0){
                qWarning() << "Bad subRubric!";
                item->release();
                continue;
            }
            KKSRubric * parent = rootRubric->rubricForId(idParent);
            if(!parent){
                qWarning() << "Bad subRubric! Parent is NULL!";
                item->release();
                continue;
            }
            parent->addItem(item);
            item->release();
        }
    }

    return rootRubric;
}

KKSMap<int, KKSAttribute*> KKSLoader::loadIOUsedAttrs() const
{
    KKSMap<int, KKSAttribute*> attrs;

    QString sql = QString("select id, name, code, title from ioGetUsedAttrs()");
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return attrs;
    }

    for(int i=0; i<res->getRowCount(); i++){
        int id = res->getCellAsInt(i, 0);
        QString name = res->getCellAsString(i, 1);
        QString code = res->getCellAsString(i, 2);
        QString title = res->getCellAsString(i, 3);

        KKSAttribute * attr = this->loadAttribute (id);
        //
        // new KKSAttribute();
        // в данном случае нигде не происходит установки типа.
        //
        attr->setCode(code);
        attr->setName(name);
        attr->setId(id);
        attr->setTitle(title);
        attrs.insert(attr->id(), attr);
        attr->release();
    }

    delete res;

    return attrs;
}

KKSList<KKSStatElement *> KKSLoader::loadIOStatistic(int id) const
{
    KKSList<KKSStatElement*> sl;
    
    if(id<=0)
        return sl;

    QString sql = QString("select * from ioGetLog(%1);").arg(id);
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return sl;
    }

    int cnt = res->getRowCount();
    for(int i=0; i<cnt; i++){
        int idUser = res->getCellAsInt(i, 1);
        QString userName = res->getCellAsString(i, 2);
        KKSStatElement::KKSStatOperation operType = (KKSStatElement::KKSStatOperation)res->getCellAsInt(i, 3);
        QDateTime operTime = res->getCellAsDateTime(i, 4);
        KKSStatElement * e = new KKSStatElement(idUser, userName, operType, operTime);
        sl.append(e);
        e->release();
    }

    delete res;

    return sl;
}

//метод загружает права (без наследования) ролей на данный ИО
//если встретится роль = getMyBoss() и getMyUnit() соответственно, 
//то задаются права для начальника и подразделения
//если встретится роль = OTHERS_ROLE, то задаются права для остальных ролей
void KKSLoader::loadPrivileges(KKSObject * io) const
{
    KKSMap<int, KKSPrivilege*> privs;
    if(!io || io->id() <= 0)
        return;

    QString sql = QString("select * from getIOPrivileges(%1)").arg(io->id());
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return;
    }

    QList<int> bossList = getBossList(io->author());
    QList<int> unitList = getUnitList(io->author());

    KKSAccessEntity * acl = new KKSAccessEntity ();
    for(int i=0; i<res->getRowCount(); i++){
        int idRole = res->getCellAsInt(i, 0);
        bool allowReadlist = res->getCellAsBool(i, 1);
        bool allowRead = res->getCellAsBool(i, 2);
        bool allowDelete = res->getCellAsBool(i, 3);
        bool allowUpdate = res->getCellAsBool(i, 4);
        bool allowSend = res->getCellAsBool (i, 5);

        KKSPrivilege * p = new KKSPrivilege(idRole, allowReadlist, allowRead, allowDelete, allowUpdate, allowSend);
        privs.insert(p->idRole(), p);

        if(bossList.contains(idRole) )
        {
        //if(idRole == getMyBoss()){
            acl->addBossPrivilege(idRole, p);
        }
        else if (idRole == 1)
        {
            //
            // если idRole == 1
            // то мы должны пройтись по всему списку начальников и задать им права,
            // кроме уже заданных
            //
            acl->addBossPrivilege(idRole, p);
            for (int ii=0; ii<bossList.count(); ii++)
            {
                if (!acl->getBossPrivilegies().contains (bossList[ii]))
                    acl->addBossPrivilege (bossList[ii], p);
            }
        }
        else if(unitList.contains(idRole) )
        {
        //else if(idRole == getMyUnit()){
            acl->addUnitPrivilege(idRole, p);
        }
        else if (idRole == 2)
        {
            //
            // если idRole == 2
            // то мы должны пройтись по всему списку подразделений и задать им права,
            // кроме уже заданных
            //
            acl->addUnitPrivilege(idRole, p);
            for (int ii=0; ii<unitList.count(); ii++)
            {
                if (!acl->getUnitPrivilegies().contains (unitList[ii]))
                    acl->addUnitPrivilege (unitList[ii], p);
            }
        }
        else if(idRole == OTHERS_ROLE){
            acl->setOthersPrivilege(p);
        }

        p->release();
    }
    acl->setPrivilegies (privs);
    io->setAccessRules (acl);

    delete res;
    
    //io->setPrivileges(privs);
}

qint64 KKSLoader::getIdByUID (const QString& tableName, const QString& uid) const
{
    qint64 id (-1);
    QString sql (QString("select getidbyuid ('%1', '%2');").arg (tableName).arg (uid));
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() != 1)
    {
        if(res)
            delete res;
        return id;
    }
    id = res->getCellAsInt64 (0, 0);
    delete res;
    return id;
}

void KKSLoader::loadPrivileges(KKSRubric * rubr) const
{
    KKSMap<int, KKSPrivilege*> privs;
    if (!rubr)// || io->id() <= 0)
        return;

    QString sql = QString("select * from getRubricPrivileges(%1)").arg(rubr->id());
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return;
    }

//    QList<int> bossList = getBossList(io->author());
//    QList<int> unitList = getUnitList(io->author());

    KKSAccessEntity * acl = new KKSAccessEntity();
    for(int i=0; i<res->getRowCount(); i++){
        int idRole = res->getCellAsInt(i, 0);
        bool allowReadlist = res->getCellAsBool(i, 1);
        bool allowRead = res->getCellAsBool(i, 2);
        bool allowDelete = res->getCellAsBool(i, 3);
        bool allowUpdate = res->getCellAsBool(i, 4);
        bool allowSend = res->getCellAsBool (i, 5);

        KKSPrivilege * p = new KKSPrivilege(idRole, allowReadlist, allowRead, allowDelete, allowUpdate, allowSend);
        privs.insert(p->idRole(), p);
/*
        if(bossList.contains(idRole)){
        //if(idRole == getMyBoss()){
            io->addBossPrivilege(idRole, p);
        }
        else if(unitList.contains(idRole)){
        //else if(idRole == getMyUnit()){
            io->addUnitPrivilege(idRole, p);
        }
        else if(idRole == OTHERS_ROLE){
            io->setOthersPrivilege(p);
        }
*/
        //rubr->setOthersPrivilege(p);

        p->release();
    }

    delete res;
    
    acl->setPrivilegies(privs);
    rubr->setAccessRules (acl);
    acl->release ();
}

//возвращает идентификатор роли-должностного лица, являющегося начальником текущего ДЛ
int KKSLoader::getMyBoss() const
{
   if(!db)
        return ERROR_CODE;

    KKSResult * res = db->execute("select id_dl from getMyBoss();");
    if(!res || res->getRowCount() != 1)
    {
        if(res)
            delete res;
        return ERROR_CODE;
    }

    int id = res->getCellAsInt (0, 0);
    delete res;

    return id;
}

//возвращает идентификатор роли-подразделения, в которое входит текущее ДЛ
int KKSLoader::getMyUnit() const
{
   if(!db)
        return ERROR_CODE;

    KKSResult * res = db->execute("select id_unit from getMyUnit();");
    if(!res || res->getRowCount() != 1)
    {
        if(res)
            delete res;
        return ERROR_CODE;
    }

    int id = res->getCellAsInt (0, 0);
    delete res;

    return id;
}
//возвращает идентификаторы ролей-должностных лиц, являющихся начальниками текущего ПОЛЬЗОВАТЕЛЯ
QList<int> KKSLoader::getBossList(int idUser) const
{
    QList<int> ids;
    if(!db || idUser <= 0)
        return ids;
    
    QString sql  = QString("select id_dl from getBossList(%1);").arg(idUser);
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() < 1)
    {
        if(res)
            delete res;
        return ids;
    }
    
    
    for(int i=0; i<res->getRowCount(); i++){
        int id = res->getCellAsInt (i, 0);
        ids.append(id);
    }
    delete res;

    return ids;
}

//возвращает идентификаторы ролей-подразделений, в которые входит текущий ПОЛЬЗОВАТЕЛЬ
QList<int> KKSLoader::getUnitList(int idUser) const
{
    QList<int> ids;
    if(!db || idUser <= 0)
        return ids;

    QString sql = QString("select id_unit from getUnitList(%1);").arg(idUser);
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() < 1)
    {
        if(res)
            delete res;
        return ids;
    }

    for(int i=0; i<res->getRowCount(); i++){
        int id = res->getCellAsInt (i, 0);
        ids.append(id);
    }

    delete res;

    return ids;
}

QMap<int, QString> KKSLoader::getBossNamesList (int idUser) const
{
    QMap<int, QString> bossMap;
    QString sql (QString ("select * from getBossList (%1);").arg (idUser));
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;
        return bossMap;
    }

    int n = res->getRowCount();
    for (int i=0; i<n; i++)
    {
        int id = res->getCellAsInt (i, 0);
        QString str = QString ("%1 %2").arg (res->getCellAsString (i, 2)).arg (res->getCellAsString (i, 3));
        bossMap.insert (id, str);
    }

    delete res;

    return bossMap;
}

QMap<int, QString> KKSLoader::getUnitNamesList (int idUser) const
{
    QMap<int, QString> unitMap;
    QString sql (QString ("select * from getUnitList (%1);").arg (idUser));
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;
        return unitMap;
    }

    int n = res->getRowCount();
    for (int i=0; i<n; i++)
    {
        int id = res->getCellAsInt (i, 0);
        QString str = res->getCellAsString (i, 1);
        unitMap.insert (id, str);
    }

    delete res;

    return unitMap;
}

QMap<int, QString> KKSLoader::getDls (void) const
{
    QMap<int, QString> posMap;
    QString sql ("select * from getdls();");

    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;
        return posMap;
    }

    int n = res->getRowCount();
    for (int i=0; i<n; i++)
    {
        int id = res->getCellAsInt (i, 0);
        QString str = res->getCellAsString (i, 2);
        posMap.insert (id, str);
    }

    delete res;

    return posMap;
}


bool KKSLoader::getPrivilege(int idRole, int idObject, int whatPrivilege, bool withInherit) const
{
    QString sql = QString("select * from getPrivilege(%1, %2, %3, %4)")
                            .arg(idRole)
                            .arg(idObject)
                            .arg(whatPrivilege)
                            .arg(withInherit ? "TRUE" : "FALSE");

    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return false;
    }

    bool ok = res->getCellAsBool(0, 0);
    delete res;
    
    return ok;
}

KKSMap<int, KKSSearchTemplateType *> KKSLoader::loadSearchTemplateTypes() const
{
    KKSMap<int, KKSSearchTemplateType *> sTypes;
    
    QString sql = QString("select * from ioGetSearchTemplateTypes()");//фирма гарантирует, что данные вернутся отсортированными по критерию "родители раньше потомков"
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() <= 0){
        if(res)
            delete res;

        return sTypes;
    }

    for (int i=0; i<res->getRowCount(); i++){
        KKSSearchTemplateType * t = new KKSSearchTemplateType(res->getCellAsInt(i, 0), res->getCellAsString(i, 1), res->getCellAsString(i, 2));
        if(!res->isEmpty(i, 3)){
            KKSSearchTemplateType * parent = sTypes.value(res->getCellAsInt(i, 3), NULL);
            if(parent)
                t->setParent(parent);
        }

        sTypes.insert(res->getCellAsInt(i, 0), t);
    }

    
    return sTypes;
}

KKSList<KKSSearchTemplate *> KKSLoader::loadSearchTemplates (void) const
{
    KKSMap<int, KKSSearchTemplateType *> sTypes = loadSearchTemplateTypes();//набор типов не будет очень большим, поэтому так можно делать

    KKSList<KKSSearchTemplate *> stList;
    QString sql ("select * from ioGetSearchTemplate();");
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;
        return stList;
    }

    for (int i=0; i<res->getRowCount(); i++)
    {

        KKSFilterGroup * gr = this->loadSearchGroup (res->getCellAsInt (i, 1));
        if (!gr)
            continue;

        KKSSearchTemplate * st = new KKSSearchTemplate (res->getCellAsInt (i, 0),  //id
                                                        gr, 
                                                        res->getCellAsString (i, 2),  //name
                                                        res->getCellAsInt (i, 3), //idAuthor
                                                        res->getCellAsString(i, 9), //description
                                                        res->getCellAsString(i, 6)); //authorName

        st->setCategory(res->getCellAsInt(i, 5), res->getCellAsString(i, 7));//idCategory, categoryName
        st->setCreationDatetime(res->getCellAsDateTime(i, 8));//creation_datetime
        KKSSearchTemplateType * t = sTypes.value(res->getCellAsInt(i, 4), NULL);
        st->setType(t);

        gr->release ();

        if (st)
            stList.append (st);
    }
    delete res;
    return stList;
}

KKSSearchTemplate * KKSLoader::loadSearchTemplate (int idSearchTemplate) const
{
    KKSMap<int, KKSSearchTemplateType *> sTypes = loadSearchTemplateTypes();//набор типов не будет очень большим, поэтому так можно делать

    QString sql (QString("select * from ioGetSearchTemplate(%1);").arg (idSearchTemplate));
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;
        return 0;
    }
    KKSFilterGroup * gr = this->loadSearchGroup (res->getCellAsInt (0, 1));
    if (!gr)
    {
        delete res;
        return 0;
    }
    KKSSearchTemplate * st = new KKSSearchTemplate (res->getCellAsInt (0, 0),  //id
                                                    gr, 
                                                    res->getCellAsString (0, 2),  //name
                                                    res->getCellAsInt (0, 3), //idAuthor
                                                    res->getCellAsString(0, 9), //description
                                                    res->getCellAsString(0, 6)); //authorName

    st->setCategory(res->getCellAsInt(0, 5), res->getCellAsString(0, 7));//idCategory, categoryName
    st->setCreationDatetime(res->getCellAsDateTime(0, 8));//creation_datetime
    KKSSearchTemplateType * t = sTypes.value(res->getCellAsInt(0, 4), NULL);
    st->setType(t);

    gr->release ();
    
    delete res;
    
    return st;
}

KKSMap<qint64, KKSSearchTemplate *> KKSLoader::loadSearchTemplatesByType (KKSSearchTemplateType * stt) const
{
    if (!stt || stt->id() <= 0)
        return KKSMap<qint64, KKSSearchTemplate *>();
    QString sql = QString ("select * from ioGetSearchTemplateByType(%1);").arg (stt->id());
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;
        return KKSMap<qint64, KKSSearchTemplate *>();
    }
    
    KKSMap<qint64, KKSSearchTemplate *> resTemplates;
    for (int i=0; i<res->getRowCount(); i++)
    {
        qint64 ids = res->getCellAsInt64(i, 0);
        KKSSearchTemplate * st = loadSearchTemplate (ids);
        if (!st)
            continue;
        resTemplates.insert (ids, st);
        st->release ();
    }
    delete res;
    
    return resTemplates;
}

KKSFilterGroup * KKSLoader::loadSearchGroup (int idGroup) const
{
    KKSFilterGroup * group = 0;

    QString sql = QString ("select * from getGroups(%1, true);").arg (idGroup);
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;
        return 0;
    }
    group = new KKSFilterGroup (res->getCellAsInt (0, 2));
    group->setId (idGroup);

    KKSList<const KKSFilterGroup *> subGroups;
    for (int i=1; i<res->getRowCount(); i++)
    {
        KKSFilterGroup * s_group = loadSearchGroup (res->getCellAsInt (i, 0));
        if (s_group)
            subGroups.append (s_group);
    }
    group->setGroups (subGroups);

    KKSList<const KKSFilter *> filters = loadCriteriaFromGroup (idGroup);
    group->setFilters (filters);

    return group;
}

KKSList<const KKSFilter *> KKSLoader::loadCriteriaFromGroup (int idGroup) const
{
    KKSList<const KKSFilter *> criteria;
    QString sql = QString ("select * from ioGetCriterionFromGroup (%1);").arg (idGroup);
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;
        return criteria;
    }

    for (int i=0; i<res->getRowCount(); i++)
    {
        KKSFilter * fc = this->loadCriterium (res->getCellAsInt (i, 0));
        if (fc)
            criteria.append (fc);
    }

    delete res;
    return criteria;
}

KKSFilter * KKSLoader::loadCriterium (int idCriteria) const
{
    KKSFilter * fc (0);
    QString sql = QString ("select * from ioGetCriterion(%1);").arg (idCriteria);
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;
        return fc;
    }

    KKSAttribute * attr = this->loadAttribute (res->getCellAsInt (0, 1));
    //qDebug () << __PRETTY_FUNCTION__ << attr->code () << attr->type()->attrType();
    if (!attr)
    {
        delete res;
        return fc;
    }
    KKSValue * val = new KKSValue (res->getCellAsString (0, 2), attr->type()->attrType());//res->getCellAsInt (0, 3));
    if (!val)
    {
        attr->release ();
        delete res;
        return fc;
    }
    KKSFilter::FilterOper op = (KKSFilter::FilterOper)res->getCellAsInt (0, 4);
    fc = new KKSFilter (attr, val, op);
    fc->setCaseSensitive (res->getCellAsBool(0, 5));

    val->release ();
    attr->release ();
    delete res;

    return fc;
}

KKSMap<int, KKSAddTable *> KKSLoader :: loadIOTables (KKSObject * io) const
{
    KKSMap<int, KKSAddTable *> resTables;
    if (!io)
        return resTables;

    QString sql = QString ("select * from ioGetObjectTables(%1);").arg (io->id());
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;
        return resTables;
    }

    for (int i=0; i<res->getRowCount(); i++)
    {
        int idCat = res->getCellAsInt (i, 3);
        KKSCategory * c = this->loadCategory (idCat);
        if (!c)
            continue;
        int idTable = res->getCellAsInt (i, 0);
        KKSAddTable * addTable = new KKSAddTable (io->id(),
                                                  idTable,
                                                  res->getCellAsString (i, 1),
                                                  c,
                                                  res->getCellAsString (i, 4),
                                                  res->getCellAsInt(i, 6),
                                                  res->getCellAsString(i, 7) );
        resTables.insert (idTable, addTable);
        addTable->release ();
        c->release ();
    }

    delete res;
    return resTables;
}

int KKSLoader :: getLocalOrgId() const
{
    QString sql = QString ("select * from getLocalOrgId();");
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() != 1)
    {
        if (res)
            delete res;
        return -1;
    }

    int id_org = res->getCellAsInt (0, 0);

    delete res;
    return id_org;
}

int KKSLoader :: getOrgId (void) const
{
    QString sql = QString ("select * from getmyorganization();");
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() != 1)
    {
        if (res)
            delete res;
        return -1;
    }

    int id_org = res->getCellAsInt (0, 0);

    delete res;
    return id_org;
}

QString KKSLoader :: getOrgName (void) const
{
    QString sql = QString ("select * from getmyorganization();");
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() != 1)
    {
        if (res)
            delete res;
        return QString();
    }

    QString org_name = res->getCellAsString (0, 1);

    delete res;
    return org_name;
}

QByteArray KKSLoader :: getOrgLogo (void) const
{
    QString sql = QString ("select tree_symbol from getMyOrganizationExt();");
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() != 1)
    {
        if (res)
            delete res;
        return QByteArray();
    }

    QByteArray org_logo = res->getCellAsByteArray (0, 0);

    delete res;
    return org_logo;
}

QString KKSLoader :: getOrgModeName (void) const
{
    QString sql = QString ("select * from getMyOrganizationExt();");
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() != 1)
    {
        if (res)
            delete res;
        return QString();
    }

    QString org_mode = res->getCellAsString (0, 6);

    delete res;
    return org_mode;
}

QMap<int, QString> KKSLoader :: getUrgencyLevels (void) const
{
    QString sql = QString ("select * from geturgencylevels();");
    KKSResult * res = db->execute (sql);
    if (!res)
        return QMap<int, QString>();

    QMap<int, QString> urgList;
    for (int i=0; i<res->getRowCount(); i++)
        urgList.insert (res->getCellAsInt (i, 0), res->getCellAsString (i, 1));

    delete res;
    return urgList;
}

bool KKSLoader :: isLocalDl (int idDl) const
{
    QString sql = QString ("select * from islocaldl(%1);").arg (idDl);
    KKSResult * res = db->execute (sql);
    if (!res)
        return false;


    bool result = res->getCellAsBool (0, 0);
    delete res;
    return result;
}

bool KKSLoader :: isPublicDl (int idDl) const
{
    QString sql = QString ("select * from ispublicdl (%1);").arg (idDl);
    KKSResult * res = db->execute (sql);
    if (!res)
        return false;


    bool result = res->getCellAsBool (0, 0);
    delete res;
    return result;
}

QMap<int, QStringList> KKSLoader :: getSyncOrg (int idObject) const
{
    if (idObject <= 0)
        return QMap<int, QStringList>();
    QString sql = QString ("select o.id, o.short_name, o.name from organization o inner join %1 io_org on (o.id=io_org.id_organization and io_org.id_io_objects=%2)").arg (IO_OBJECTS_ORGANIZATION).arg (idObject);
    KKSResult * res = db->execute (sql);
    if (!res)
        return QMap<int, QStringList>();

    QMap<int, QStringList> result;
    for (int i=0; i<res->getRowCount(); i++)
    {
        QStringList strL;
        strL << res->getCellAsString (i, 1) << res->getCellAsString (i, 2);
        result.insert (res->getCellAsInt (i, 0), strL);
    }
    delete res;
    return result;
}

KKSAclTemplate * KKSLoader :: loadAclTemplate (int idAclTemplate)
{
    KKSAclTemplate * aclTemplate (0);
    QString sql = QString ("select * from getAclTemplate (%1);").arg (idAclTemplate < 0 ? QString ("NULL") : QString::number (idAclTemplate));
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() != 1)
    {
        if (res)
            delete res;
        return aclTemplate;
    }

    aclTemplate = new KKSAclTemplate ();
    aclTemplate->setId (idAclTemplate);
    aclTemplate->setName (res->getCellAsString (0, 1));
    aclTemplate->setSystem (res->getCellAsBool (0, 2));
    delete res;
    QString privSql = QString ("select * from getPrivilegiesFromTemplate (%1);").arg (idAclTemplate < 0 ? QString ("NULL") : QString::number (idAclTemplate));
    KKSResult * privRes = db->execute(privSql);
    if (!privRes)
        return aclTemplate;
    KKSMap<int, KKSPrivilege*> bossPrivs;
    KKSMap<int, KKSPrivilege*> unitPrivs;
    KKSPrivilege * otherPriv (0);

    KKSMap<int, KKSPrivilege*> Privs;
    QList<int> bossList = this->getBossList (this->getUserId());
    QList<int> unitList = this->getUnitList (this->getUserId());
    for (int i=0; i<privRes->getRowCount(); i++)
    {
        int iType = privRes->getCellAsInt (i, 7);
        int id = privRes->getCellAsInt (i, 0);
        KKSPrivilege * p = new KKSPrivilege (id,
                                             privRes->getCellAsBool (i, 2),
                                             privRes->getCellAsBool (i, 3),
                                             privRes->getCellAsBool (i, 5),
                                             privRes->getCellAsBool (i, 4),
                                             privRes->getCellAsBool (i, 6)
                                            );
        if (iType == 2)
        {
            //
            // exceptions
            //
            Privs.insert (id, p);
        }
        else
        {
            //
            // boss, unit, others
            //
            switch (id)
            {
                case 0: case -10: case 3: default:
                {
                    otherPriv = p;
                    if (p)
                        otherPriv->addRef ();
                    break;
                }
                case 1:
                {
                    p->setRole (id);
                    bossPrivs.insert (id, p);
                    for (int ii=0; ii<bossList.count(); ii++)
                    {
                        int idBoss = bossList[ii];
                        p->setRole (idBoss);
                        bossPrivs.insert (idBoss, p);
                        Privs.insert (idBoss, p);
                    }
                    break;
                }
                case 2:
                {
                    p->setRole (id);
                    unitPrivs.insert (id, p);
                    for (int ii=0; ii<unitList.count(); ii++)
                    {
                        int idUnit =  unitList[ii];
                        p->setRole (idUnit);
                        unitPrivs.insert (idUnit, p);
                        Privs.insert (idUnit, p);
                    }
                    break;
                }
            }
        }
        p->release ();
    }

    aclTemplate->setBossPrivilegies (bossPrivs);
    aclTemplate->setUnitPrivilegies (unitPrivs);
    aclTemplate->setOthersPrivilege (otherPriv);
    aclTemplate->setPrivilegies (Privs);

    return aclTemplate;
}

KKSObjectExemplar * KKSLoader::getMessage(int idMsg) const
{
    KKSObjectExemplar * eio = NULL;
    
    KKSObject * io = loadIO (IO_MSGJ_ID);
    if (!io)
        return eio;

    QString fields;
    
    KKSCategory * c = 0;
    c = io->category()->tableCategory();
    if(!c)
        return eio;

    KKSMap<int, KKSCategoryAttr *> attrs = c->attributes();
    int count = attrs.count();
    int i=0;
    KKSMap<int, KKSCategoryAttr *>::const_iterator pa;
    for (pa=attrs.constBegin(); pa != attrs.constEnd(); pa++)
    {
        KKSCategoryAttr * attr = pa.value();
        QString code;
        code = attr->code(true);
        
        fields += code;

        if(i<count-1){
            fields += ", ";
        }

        i++;
    }

    QString sql = QString("select id, insert_time, message_body, id_io_object, id_dl_sender, id_dl_receiver, sent_datetime, receive_datetime, read_datetime, input_number, output_number, id_urgency_level from jGetMsg(%1)").arg(idMsg);

    KKSResult * res = db->execute(sql);
    if(!res)
        return eio;

    count = res->getRowCount();
    if(count != 1){
        qWarning("The too many EIO with given ID! EIO.id = %d, IO.id = %d", idMsg, io->id());
        delete res;
        return eio;
    }

    eio = new KKSObjectExemplar();
    eio->setIo(io);

    KKSList<KKSAttrValue *> attrValues;
    
    KKSCategoryAttr *attr = c->attribute(1);
    KKSAttrValue * av = new KKSAttrValue();
    av->setAttribute(attr);

    KKSValue v;
    v = constructValue(res->getCellAsString(0, 0), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(16);//insert_time
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 1), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(45);//message_body
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 2), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(48);//id_io_object
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 3), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(51);//id_dl_sender
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 4), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(52);//id_dl_receiver
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 5), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(54);//sent_datetime
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 6), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(55);//receive_datetime
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 7), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(56);//read_datetime
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 8), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(99);//input_number
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 9), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(100);//output_number
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 10), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(165);//id_urgency_level
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 11), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();

    eio->setAttrValues(attrValues);
    eio->setId(idMsg);
    
    delete res;

    return eio;
}

KKSObjectExemplar * KKSLoader::getCommand(int idCmd) const
{
    KKSObjectExemplar * eio = NULL;
    
    KKSObject * io = loadIO (IO_CMDJ_ID);
    if (!io)
        return eio;

    QString fields;
    
    KKSCategory * c = 0;
    c = io->category()->tableCategory();
    if(!c)
        return eio;

    KKSMap<int, KKSCategoryAttr *> attrs = c->attributes();
    int count = attrs.count();
    int i=0;
    KKSMap<int, KKSCategoryAttr *>::const_iterator pa;
    for (pa=attrs.constBegin(); pa != attrs.constEnd(); pa++)
    {
        KKSCategoryAttr * attr = pa.value();
        QString code;
        code = attr->code(true);
        
        fields += code;

        if(i<count-1){
            fields += ", ";
        }

        i++;
    }

    QString sql = QString("select id, id_io_category, insert_time, id_dl_to, id_dl_executor, exec_datetime, exec_period, is_archived, id_jr_state, id_dl_from, is_automated, message_body, receive_datetime, input_number, output_number, id_urgency_level, id_io_object_att, accepted_datetime from jGetCmd(%1)").arg(idCmd);

    KKSResult * res = db->execute(sql);
    if(!res)
        return eio;

    count = res->getRowCount();
    if(count != 1){
        qWarning("The too many EIO with given ID! EIO.id = %d, IO.id = %d", idCmd, io->id());
        delete res;
        return eio;
    }

    eio = new KKSObjectExemplar();
    eio->setIo(io);

    KKSList<KKSAttrValue *> attrValues;
    
    KKSCategoryAttr *attr = c->attribute(1);
    KKSAttrValue * av = new KKSAttrValue();
    av->setAttribute(attr);

    KKSValue v;
    v = constructValue(res->getCellAsString(0, 0), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(12);//id_io_category
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 1), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(16);//insert_time
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 2), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(34);//id_dl_to
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 3), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(35);//id_dl_executor
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 4), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(37);//exec_datetime
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 5), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(38);//exec_period
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 6), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(40);//is_archived
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 7), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(41);//id_jr_state
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 8), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(42);//id_dl_from
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 9), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(44);//is_automated
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 10), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(45);//message_body
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 11), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(55);//receive_datetime
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 12), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(99);//input_number
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 13), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(100);//output_number
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 14), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(165);//id_urgency_level
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 15), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(167);//id_io_object_att
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 16), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();
//--
    attr = c->attribute(188);//accepted_datetime
    av = new KKSAttrValue();
    av->setAttribute(attr);

    v = constructValue(res->getCellAsString(0, 17), attr);
    av->setValue(v);

    attrValues.append(av);
    av->release();

    eio->setAttrValues(attrValues);
    eio->setId(idCmd);
    
    delete res;

    return eio;
}

KKSAccessEntity * KKSLoader::loadAccessCat (KKSCategory * cat) const
{
    if (!cat || cat->id() <= 0)
        return 0;

    KKSAccessEntity * acl (0);

    QString sql = QString("select * from getCPrivileges(%1)").arg(cat->id());
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return acl;
    }

    //QList<int> bossList = getBossList(io->author());
    //QList<int> unitList = getUnitList(io->author());

    acl = new KKSAccessEntity ();
    KKSMap<int, KKSPrivilege*> privs;
    for(int i=0; i<res->getRowCount(); i++)
    {
        int idRole = res->getCellAsInt(i, 0);
        bool allowReadlist = res->getCellAsBool(i, 1);
        bool allowRead = res->getCellAsBool(i, 2);
        bool allowDelete = res->getCellAsBool(i, 3);
        bool allowUpdate = res->getCellAsBool(i, 4);
        bool allowSend = res->getCellAsBool (i, 5);

        KKSPrivilege * p = new KKSPrivilege(idRole, allowReadlist, allowRead, allowDelete, allowUpdate, allowSend);
        privs.insert(p->idRole(), p);

        if (idRole == OTHERS_ROLE)
        {
            acl->setOthersPrivilege(p);
        }

        p->release();
    }
    acl->setPrivilegies (privs);
    cat->setAccessRules (acl);
    return acl;
}

KKSMap<int, KKSAGroup *> KKSLoader :: loadAttrsGroups (void) const
{
    KKSMap<int, KKSAGroup*> results;

    QString sql ("select * from aGetAttrGroups(null::int4);");
    KKSResult * res = db->execute (sql);
    if (!res)
        return results;

    for (int i=0; i<res->getRowCount(); i++)
    {
        int aKey = res->getCellAsInt (i, 0);
        KKSAGroup * ag = new KKSAGroup (aKey, res->getCellAsString(i, 2));
        int aParent = res->getCellAsInt (i, 1);
        if (aParent <= 0)
            results.insert (aKey, ag);
        else if (results.contains (aParent))
        {
            KKSAGroup * ap = results.value (aParent);
            ap->addChildGroup (aKey, ag);
        }
        else
        {
            KKSAGroup * ap(0);
            for (KKSMap<int, KKSAGroup*>::const_iterator pg=results.constBegin();
                 pg != results.constEnd() && !ap;
                 pg++)
                ap = pg.value()->childGroupForId (aParent);
            if (ap)
                ap->addChildGroup (aKey, ag);
        }
        /*
        QString attrSql (QString("select * from aGetAttributesList(%1);").arg (aKey));
        KKSResult * aRes = db->execute (attrSql);
        if (aRes)
        {
            for (int ii=0; ii<aRes->getRowCount(); ii++)
            {
                int idAttr = aRes->getCellAsInt (ii, 0);
                KKSAttribute * attr = this->loadAttribute (idAttr);
                if (!attr)
                    continue;
                ag->addAttribute (idAttr, attr);

                attr->release ();
            }
            delete aRes;
        }
        */
        addAttributesToGroup(ag);
    }
    delete res;

    return results;
}

void KKSLoader::addAttributesToGroup(KKSAGroup * ag) const
{
    if(!ag)
        return;

    QString attrSql (QString("select * from aGetAttributesList(%1);").arg (ag->id()));
    KKSResult * res = db->execute (attrSql);
    if (!res || res->getRowCount() <= 0){
        if(res)
            delete res;
        return;
    }

    int cnt = res->getRowCount();
    for (int ii=0; ii<cnt; ii++)
    {
        int idAttr = res->getCellAsInt (ii, 0);

        //
        // параметры самого атрибута
        //
        KKSAttribute * attr = new KKSAttribute();
        attr->setId(res->getCellAsInt(ii, 0));
        attr->setCode(res->getCellAsString(ii, 2));
        attr->setName(res->getCellAsString(ii, 3));
        attr->setTitle(res->getCellAsString(ii, 4));
        attr->setTableName(res->getCellAsString(ii, 5));
        attr->setColumnName(res->getCellAsString(ii, 6));
        attr->setDefWidth(res->getCellAsInt(ii, 7));
        //
        //тип атрибута
        //
        KKSAttrType * type = new KKSAttrType();
        type->setId(res->getCellAsInt(ii, 1));
        type->setName(res->getCellAsString(ii, 8));
        type->setCode(res->getCellAsString(ii, 9));

        //
        //Тип ссылочного атрибута
        //
        if(!attr->columnName().isEmpty()){
            KKSAttrType * refType = new KKSAttrType();
            int idRefType = res->getCellAsInt(ii, 13);
            if (idRefType != KKSAttrType::atList)//это когда атрибут является ссылкой на некоторое обычное значение
            {
                refType->setId (idRefType);
                refType->setName(res->getCellAsString(ii, 14));
                refType->setCode(res->getCellAsString(ii, 15));
            }
            else //это когда атрибут является ссылкой на ссылку
            {
                bool isLow = false;
                QString aRefCode = attr->columnName();
                while (!isLow)
                {
                    KKSAttribute * refAttr = loadAttribute (aRefCode, attr->tableName());
                    if (!refAttr)
                        break;
                    aRefCode = refAttr->code ();
                    isLow = (refAttr->refType()->id() != KKSAttrType::atList);
                    if (isLow)
                    {
                        refType->setId (refAttr->refType()->id());
                        refType->setName (refAttr->refType()->name());
                        refType->setCode (refAttr->refType()->code());
                    }
                    refAttr->release ();
                }
            }
            attr->setRefType(refType);
            refType->release();
        }
    
        attr->setType(type);
        type->release();

        //
        //поисковый запрос
        //
        if(res->isEmpty(ii, 17))
            attr->setSearchTemplate(NULL);
        else{
            KKSSearchTemplate * st = this->loadSearchTemplate(res->getCellAsInt(ii, 17));
            if(st){
                attr->setSearchTemplate(st);
                st->release();
            }
        }

        //
        //название ссылочной колонки
        //
        if(!res->isEmpty(ii, 18)){
            attr->setRefColumnName(res->getCellAsString(ii, 18));
            attr->setRefColumnType(type);//внутри сделается addRef()
        }
        //
        //группа атрибута
        //
        int idGroup = res->getCellAsInt(ii, 19);
        QString groupName = res->getCellAsString(ii, 20);
        KKSAGroup * g = new KKSAGroup(idGroup, groupName);
        attr->setGroup(g);
        g->release();

        ag->addAttribute (idAttr, attr);

        attr->release ();
    }
    delete res;

}


KKSMap<int, KKSAGroup *> KKSLoader :: loadAvailAttrsGroups (void) const
{
    KKSMap<int, KKSAGroup*> results;

    QString sql ("select * from aGetAttrGroups(null::int4);");
    KKSResult * res = db->execute (sql);
    if (!res)
        return results;

    for (int i=0; i<res->getRowCount(); i++)
    {
        int aKey = res->getCellAsInt (i, 0);
        KKSAGroup * ag = new KKSAGroup (aKey, res->getCellAsString(i, 2));
        results.insert (aKey, ag);
        
        /*
        QString attrSql (QString("select * from aGetAttributesList(%1);").arg (aKey));
        KKSResult * aRes = db->execute (attrSql);
        if (aRes)
        {
            for (int ii=0; ii<aRes->getRowCount(); ii++)
            {
                int idAttr = aRes->getCellAsInt (ii, 0);
                KKSAttribute * attr = this->loadAttribute (idAttr);
                if (!attr)
                    continue;

                ag->addAttribute (idAttr, attr);

                attr->release ();
            }
            delete aRes;
        }
        */
        addAttributesToGroup(ag);
    }
    delete res;
    return results;
}

KKSAGroup * KKSLoader :: loadAttrGroup (int idAGr) const
{
    QString sql (QString("select * from aGetAttrGroup(%1);").arg (idAGr));
    KKSResult * res = db->execute (sql);
    if (!res)
        return 0;

    //int aKey = res->getCellAsInt (0, 0);
    KKSAGroup * ag = new KKSAGroup (idAGr, res->getCellAsString(0, 2));
    delete res;
    
    /*
    QString attrSql (QString("select * from aGetAttributesList(%1);").arg (aKey));
    KKSResult * aRes = db->execute (attrSql);
    if (aRes)
    {
        for (int ii=0; ii<aRes->getRowCount(); ii++)
        {
            int idAttr = aRes->getCellAsInt (ii, 0);
            KKSAttribute * attr = this->loadAttribute (idAttr);
            if (!attr)
                continue;

            ag->addAttribute (idAttr, attr);

            attr->release ();
        }
        delete aRes;
    }
    */
    addAttributesToGroup(ag);

    return ag;
}

KKSList<KKSIndicatorValue *> KKSLoader::loadIndicatorValues(KKSObject * io) const
{
    KKSList<KKSIndicatorValue *> inds;

    if(!io || io->id() <= 0)
        return inds;
    
    QString sql = QString("select * from ioGetIndicatorsValues(%1)").arg(io->id());
    KKSResult * res = db->execute(sql);
    if(!res)
        return inds;

    int count = res->getRowCount();
    if(count == 0){
        //qWarning("The object does not contain any values of attributes");
        delete res;
        return inds;
    }

    for(int row=0; row<count; row++){
        
        KKSAttrType * t = this->loadAttrType (res->getCellAsInt(row, 6));//new KKSAttrType();
        //t->setId(res->getCellAsInt(row, 6));//id_a_type
        
        KKSIndicatorType * iT = new KKSIndicatorType();
        iT->setId (res->getCellAsInt(row, 3));// (res->getCellAsInt(row, 6));
        iT->setName(res->getCellAsString(row, 4));
        
        KKSIndicator * ind = new KKSIndicator(res->getCellAsInt(row, 0),
                                              res->getCellAsString(row, 1),
                                              res->getCellAsString(row, 2)
                                              );

        ind->setType(t);
        t->release();

        ind->setIndType(iT);
        iT->release();

        ind->setTableName(res->getCellAsString(row, 20));
        ind->setColumnName(res->getCellAsString(row, 21));

        KKSAttrType * refType = NULL;
        if(!ind->columnName().isEmpty()){
            refType = new KKSAttrType();
            refType->setId(res->getCellAsInt(row, 22));
        }
        ind->setRefType(refType);

        int idParent = res->getCellAsInt(row, 5);
        if(idParent > 0){
            int cnt = inds.count()-1;
            for(int i=cnt; i>=0; i--){//как правило в данном цикле должен быть только один проход или их минимальное количество
                KKSIndicator * parent = inds.at(i)->indicator();
                if(parent && parent->id() == idParent){
                    ind->setParent(parent);
                    break;
                }
            }
        }

        KKSValue value = constructValue(res->getCellAsString(row, 15), ind, io->tableName());
        if(!value.isValid())
        {
            qWarning("Value for indicator is NOT valid! Value = %s, idIndicatorValue = %d, idObject = %d",
                        res->getCellAsString(row, 15).toLocal8Bit().data(),
                        res->getCellAsInt(row, 9),
                        io->id());
        }

        KKSIndicatorValue * iv = new KKSIndicatorValue(res->getCellAsInt(row, 9), ind, value);
        ind->release();

        KKSObject * usedIO = this->loadIO(res->getCellAsInt(row, 10), true);
        iv->setIO(usedIO);
        if(usedIO)
            usedIO->release();

        usedIO = this->loadIO(res->getCellAsInt(row, 11), true);
        iv->setIOSrc(usedIO);
        if(usedIO)
            usedIO->release();

        usedIO = this->loadIO(res->getCellAsInt(row, 12), true);
        iv->setIOSrc1(usedIO);
        if(usedIO)
            usedIO->release();

        //iv->setMeasDateTime(res->getCellAsDateTime(row, 13));
        iv->setInsertDateTime(res->getCellAsDateTime(row, 14));
        iv->setStartDateTime(res->getCellAsDateTime(row, 16));
        iv->setStopDateTime(res->getCellAsDateTime(row, 17));




        inds.append(iv);
        iv->release();
    }
    
    delete res;

    return inds;
}

KKSIndicatorValue * KKSLoader::loadIOIndicatorValue (KKSObject * io, int idIndicator) const
{
    if (!io || idIndicator <= 0)
        return 0;
    KKSList<KKSIndicatorValue *> ioIVals = this->loadIndicatorValues (io);
    KKSIndicatorValue * iVal (0);
    for (int i=0; i<ioIVals.count() && !iVal; i++)
        if (ioIVals[i] && idIndicator == ioIVals[i]->indicator()->id())
        {
            iVal = ioIVals[i];
            iVal->addRef ();
        }
    ioIVals.clear ();
    return iVal;
}

KKSMap<int, KKSIndicatorType *> KKSLoader::loadIndicatorTypes (void) const
{
    QString sql = QString ("select * from ioGetIndicatorTypes();");
    KKSResult * res = db->execute (sql);
    if (!res)
        return KKSMap<int, KKSIndicatorType*>();

    KKSMap<int, KKSIndicatorType*> itMap;
    for (int i=0; i<res->getRowCount(); i++)
    {
        KKSIndicatorType * iT = new KKSIndicatorType();
        if (!iT)
            continue;
        iT->setId (res->getCellAsInt(i, 0));
        iT->setName (res->getCellAsString (i, 1));
        itMap.insert (iT->id(), iT);
        iT->release ();
    }
    delete res;
    return itMap;
}

KKSIndicator * KKSLoader::loadIndicator (int idIndicator) const
{
    QString sql = QString ("select * from getindicator(%1)").arg (idIndicator);
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() != 1)
    {
        if (res)
            delete res;
        return 0;
    }
    
    KKSIndicator * ind = new KKSIndicator (res->getCellAsInt (0, 0),
                                           res->getCellAsString (0, 1),
                                           res->getCellAsString (0, 2));

    KKSIndicatorType * it = new KKSIndicatorType ();
    it->setId (res->getCellAsInt (0, 3));
    it->setName (res->getCellAsString (0, 4));
    ind->setIndType (it);
    KKSAttrType * atype = this->loadAttrType(res->getCellAsInt(0, 6));
    atype->setId (res->getCellAsInt (0, 6));
    ind->setType (atype);
    
    if (res->getCellAsInt (0, 5) > 0)
    {
        KKSIndicator * pInd = loadIndicator (res->getCellAsInt (0, 5));
        ind->setParent (pInd);
    }
    return ind;
}

KKSValue  KKSLoader::constructValue(const QString & value, 
                                    const KKSIndicator * i, 
                                    const QString & parentTable) const
{
    KKSValue v;
    if(!i || !i->type())
        return v;

    //в данном методе type предстает не как идентификатор строки в БД, а элемент enum KKSAttrType::KKSAttrTypes
    v.setValue(value, i->type()->attrType());
    if(!v.isValid())
        qWarning("Value for indicator is NOT valid! Value = %s", value.toLocal8Bit().data());

    if(v.value().isEmpty())
        return v;

    if( i->type()->attrType() == KKSAttrType::atList ||
        i->type()->attrType() == KKSAttrType::atParent
        )
    {
        //в случае атрибутов типа список необходимо загрузить в KKSValue реальное значение из соответствующей таблицы БД
        QString cv = loadColumnValue(i, 
                                     v.valueVariant().toInt(), 
                                     parentTable);
        if(!cv.isEmpty())
            v.setColumnValue(cv);
        else
            qWarning("The indicator KKSAttrType::atList points to empty or NULL value in corresponding table!");
    }

    return v;
}

KKSList<KKSAttrValue *> KKSLoader::loadIOAttrValueHistory(const KKSAttrValue * av, bool forRecords) const
{
    KKSList<KKSAttrValue *> avList;

    if(!av)
        return avList;

    QString sql;
    int idAVal;
    if (av->id() > 0)
        idAVal = av->id();
    else
        return avList;
//    {
//        const KKSAttrValue * avIO = av->ioSrc()->attrValueId(av->attribute()->id());
//        idAVal = avIO->id();
//    }
    
    if(forRecords)
        sql = QString ("select * from getRecAttrValueHistory(%1, NULL, NULL)").arg (idAVal);
    else
        sql = QString ("select * from getIOAttrValueHistory(%1, NULL, NULL)").arg (idAVal);

    KKSResult * res = db->execute (sql);
    int cnt = 0;
    if (!res || (cnt = res->getRowCount()) <= 0)
    {
        if (res)
            delete res;
        return avList;
    }

//

    for(int row=0; row<cnt; row++){
        //параметры самого атрибута
        //int idAttr = res->getCellAsInt(row, 2);
        KKSCategoryAttr * a = const_cast<KKSCategoryAttr*>(av->attribute());

        KKSAttrValue * attr = new KKSAttrValue();
        attr->setAttribute(a);

        KKSValue v = constructValue(res->getCellAsString(row, 3), a);
        if(!v.isValid())
        {
            qWarning("Value for attribute is NOT valid! Value = %s, idAttrValue= %d ",
                        res->getCellAsString(row, 3).toLocal8Bit().data(),
                        res->getCellAsInt(row, 7)
                    );
        }

        attr->setValue(v);
        
        //устанавливаем дополнительные характеристики значения атрибута
        attr->setId(res->getCellAsInt(row, 7));
        attr->setStartDateTime(res->getCellAsDateTime(row, 8));
        attr->setStopDateTime(res->getCellAsDateTime(row, 9));
        //attr->setMeasDateTime(res->getCellAsDateTime(row, 10));
        attr->setInsertDateTime(res->getCellAsDateTime(row, 11));
        attr->setDesc(res->getCellAsString(row, 15));

        int idObjSrc = res->getCellAsInt(row, 12);
        if(idObjSrc > 0){
            KKSObject * o = loadIO(idObjSrc, true);//упрощенно
            attr->setIOSrc(o);
            if(o)
                o->release();
        }

        int idObjSrc1 = res->getCellAsInt(row, 13);
        if(idObjSrc1 > 0){
            KKSObject * o = loadIO(idObjSrc1, true);//упрощенно
            attr->setIOSrc1(o);
            if(o)
                o->release();
        }

        attr->setActual(res->getCellAsBool(row, 14));

        avList.append(attr);
        attr->release();
    }
    delete res;
    return avList;

}

KKSAttrValue * KKSLoader::loadIOAttrValue(const KKSAttrValue * av, int idVal, bool forRecords) const
{
    KKSList<KKSAttrValue *> avList;

    if(!av || (av->id() <= 0 && idVal < 0))
        return 0;

    QString sql;
    
    if(forRecords)
        sql = QString ("select * from getRecAttrValue(%1)").arg (idVal);
    else
        sql = QString ("select * from getIOAttrValue(%1::int8)").arg (idVal);

    KKSResult * res = db->execute (sql);
    int cnt = res ? res->getRowCount() : 0;
    if (!res || cnt != 1)
    {
        if (res)
            delete res;
        return 0;
    }

    KKSCategoryAttr * a = const_cast<KKSCategoryAttr*>(av->attribute());

    KKSAttrValue * attr = new KKSAttrValue();
    attr->setAttribute(a);

    int row = 0;
    KKSValue v = constructValue(res->getCellAsString(row, 3), a);
    if(!v.isValid())
    {
        qWarning("Value for attribute is NOT valid! Value = %s, idAttrValue= %d ",
                    res->getCellAsString(row, 3).toLocal8Bit().data(),
                    res->getCellAsInt(row, 7)
                );
    }

    attr->setValue(v);

    //устанавливаем дополнительные характеристики значения атрибута
    attr->setId(res->getCellAsInt(row, 7));
    attr->setStartDateTime(res->getCellAsDateTime(row, 8));
    attr->setStopDateTime(res->getCellAsDateTime(row, 9));
    //attr->setMeasDateTime(res->getCellAsDateTime(row, 10));
    attr->setInsertDateTime(res->getCellAsDateTime(row, 11));
    attr->setDesc(res->getCellAsString(row, 15));

    int idObjSrc = res->getCellAsInt(row, 12);
    if(idObjSrc > 0){
        KKSObject * o = loadIO(idObjSrc, true);//упрощенно
        attr->setIOSrc(o);
        if(o)
            o->release();
    }

    int idObjSrc1 = res->getCellAsInt(row, 13);
    if(idObjSrc1 > 0){
        KKSObject * o = loadIO(idObjSrc1, true);//упрощенно
        attr->setIOSrc1(o);
        if(o)
            o->release();
    }

    attr->setActual(res->getCellAsBool(row, 14));
    delete res;
    return attr;
}

void KKSLoader::loadAttrAttrs(KKSAttribute * a) const
{
    if(!a || a->id() <= 0)
        return;

    KKSMap<int, KKSCategoryAttr *> aaList = loadAttrAttrs(a->id());
    
    a->setAttrs(aaList);
    
    return;
}

KKSMap<int, KKSCategoryAttr*> KKSLoader::loadAttrAttrs(int idAttr) const
{
    KKSMap<int, KKSCategoryAttr *> aaList;

    if(idAttr <= 0)
        return aaList;

    QString sql = QString("select * from aGetAttrAttrs(%1)").arg(idAttr);
    KKSResult * res = db->execute (sql);
    int cnt = 0;
    if (!res || (cnt = res->getRowCount()) <= 0)
    {
        if (res)
            delete res;
        return aaList;
    }

    for(int row=0; row<cnt; row++){
        KKSCategoryAttr * attr = new KKSCategoryAttr();
        int idAttrAttr = res->getCellAsInt(row, 21);
        
/**/
        attr->setId(res->getCellAsInt(row, 0));
        attr->setCode(res->getCellAsString(row, 2));
        attr->setName(res->getCellAsString(row, 3));
        attr->setTitle(res->getCellAsString(row, 4));
        attr->setTableName(res->getCellAsString(row, 5));
        attr->setColumnName(res->getCellAsString(row, 6));
        attr->setDefWidth(res->getCellAsInt(row, 7));

        attr->setIdRow(idAttrAttr);
        attr->setIdParent(idAttr);
        
        //тип атрибута
        KKSAttrType * type = new KKSAttrType();
        type->setId(res->getCellAsInt(row, 1));
        type->setName(res->getCellAsString(row, 8));
        type->setCode(res->getCellAsString(row, 9));

        if(!attr->columnName().isEmpty()){
            KKSAttrType * refType = new KKSAttrType();
            int idRefType = res->getCellAsInt(row, 13);
            if (idRefType != KKSAttrType::atList)//это когда атрибут является ссылкой на некоторое обычное значение
            {
                refType->setId (idRefType);
                refType->setName(res->getCellAsString(row, 14));
                refType->setCode(res->getCellAsString(row, 15));
            }
            else//это когда атрибут является ссылкой на атрибут типа ссылка (ссылка на ссылку)
            {
                bool isLow = false;
                QString aRefCode = attr->columnName();
                while (!isLow)
                {
                    KKSAttribute * refAttr = loadAttribute (aRefCode, attr->tableName());
                    if (!refAttr)
                        break;
                    aRefCode = refAttr->code ();
                    isLow = (refAttr->refType()->id() != KKSAttrType::atList);
                    if (isLow)
                    {
                        //qDebug () << __PRETTY_FUNCTION__ << isLow << refAttr->refType()->id();
                        refType->setId (refAttr->refType()->id());
                        refType->setName (refAttr->refType()->name());
                        refType->setCode (refAttr->refType()->code());
                    }
                    refAttr->release ();
                }
            }

            attr->setRefType(refType);
            refType->release();
        }


        attr->setType(type);
        type->release();

        //параметры атрибута в категории
        //дефолтное значение
        KKSValue defValue = constructValue(res->getCellAsString(row, 10), attr);
        if(!defValue.isValid())
            qWarning("defValue for attribute is NOT valid! defValue = %s, idAttrAttr = %d", 
                        res->getCellAsString(row, 10).toLocal8Bit().data(), 
                        idAttrAttr);

        attr->setDefValue(defValue);

        //обязательный атрибут
        attr->setMandatory(res->getCellAsBool(row, 11));
        //только для чтения атрибут
        attr->setReadOnly(res->getCellAsBool(row, 12));

        //поисковый запрос
        if(res->isEmpty(row, 17))
            attr->setSearchTemplate(NULL);
        else{
            KKSSearchTemplate * st = this->loadSearchTemplate(res->getCellAsInt(row, 17));
            if(st){
                attr->setSearchTemplate(st);
                st->release();
            }
        }

        //название ссылочной колонки
        if(!res->isEmpty(row, 18)){
            attr->setRefColumnName(res->getCellAsString(row, 18));
            attr->setRefColumnType(type);
        }

        //группа атрибута
        int idGroup = res->getCellAsInt(row, 19);
        QString groupName = res->getCellAsString(row, 20);
        KKSAGroup * g = new KKSAGroup(idGroup, groupName);
        attr->setGroup(g);
        g->release();
/**/
        aaList.insert(idAttrAttr, attr);
        attr->release();

    }
    
    delete res;

    return aaList;
}

QList<int> KKSLoader :: getForbiddenTypes (void) const
{
    QString sql = QString("select * from cGetForbiddenTypes()");
    KKSResult * res = db->execute (sql);
    if (!res)
        return QList<int>();
    QStringList fTypesStr = res->getCellAsStringList (0, 0);
    QList<int> forbiddenTypes;
    for (int i=0; i< fTypesStr.size(); i++)
        forbiddenTypes << fTypesStr[i].toInt();
    delete res;
    return forbiddenTypes;
}

KKSList<KKSAttrValue *> KKSLoader::loadIndValues(KKSObjectExemplar * eio) const
{
    KKSList<KKSAttrValue *> attrs;

    if(!eio || 
        eio->id() <= 0 || 
       !eio->io()->category() || 
       !eio->io()->category()->recAttrCategory()
       )
    {
        return attrs;
    }
    
    QString sql = QString("select * from eioGetIndicators(%1, true, NULL::timestamp, NULL::timestamp)").arg(eio->id());
    KKSResult * res = db->execute(sql);
    if(!res)
        return attrs;

    int count = res->getRowCount();
    if(count == 0){
        //qWarning("The object does not contain any values of attributes");
        delete res;
        return attrs;
    }

    KKSCategory * c = eio->io()->category()->recAttrCategory();

    for(int row=0; row<count; row++){
        //параметры самого атрибута
        int idAttr = res->getCellAsInt(row, 2);
        KKSCategoryAttr * a = c->attribute(idAttr);
        if(!a){
            qWarning("The object contain attr-value but category does not contain attribute! idAttr = %d, idObjectExemplar = %lld, idCategory = %d", idAttr, eio->id(), c->id());
            continue;
        }

        KKSAttrValue * attr = new KKSAttrValue();
        attr->setAttribute(a);

        KKSValue v = constructValue(res->getCellAsString(row, 3), a, eio->io()->tableName());
        if(!v.isValid())
        {
            qWarning("Value for attribute is NOT valid! Value = %s, idCategory = %d, idAttribute = %d, idObjectExemplar = %d",
                        res->getCellAsString(row, 3).toLocal8Bit().data(),
                        c->id(),
                        res->getCellAsInt(row, 2),
                        eio->id());
        }

        attr->setValue(v);
        
        //устанавливаем дополнительные характеристики значения атрибута
        attr->setId(res->getCellAsInt64(row, 7));
        attr->setStartDateTime(res->getCellAsDateTime(row, 8));
        attr->setStopDateTime(res->getCellAsDateTime(row, 9));
        //attr->setMeasDateTime(res->getCellAsDateTime(row, 10));
        attr->setInsertDateTime(res->getCellAsDateTime(row, 10));
        attr->setDesc(res->getCellAsString(row, 14));

        int idObjSrc = res->getCellAsInt(row, 11);
        if(idObjSrc > 0){
            KKSObject * o = loadIO(idObjSrc, true);//упрощенно
            attr->setIOSrc(o);
            if(o)
                o->release();
        }

        int idObjSrc1 = res->getCellAsInt(row, 12);
        if(idObjSrc1 > 0){
            KKSObject * o = loadIO(idObjSrc1, true);//упрощенно
            attr->setIOSrc1(o);
            if(o)
                o->release();
        }

        attr->setActual(res->getCellAsBool(row, 13));

        attrs.append(attr);
        attr->release();
    }
    
    delete res;

    return attrs;
}

int KKSLoader :: getRefIO (int idObjectE) const
{
    if (idObjectE < 0)
        return ERROR_CODE;
    QString sql = QString ("select * from getRecordTable(%1)").arg (idObjectE);
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount () != 1)
    {
        if (res)
            delete res;
        return ERROR_CODE;
    }
    QString tableName = res->getCellAsString (0, 0);
    delete res;
    sql = QString("select ioGetObjectIDByTableName ('%1');").arg(tableName);
    qDebug () << __PRETTY_FUNCTION__ << sql;
    KKSResult * iores = db->execute (sql);
    if (!iores || iores->getRowCount() != 1)
    {
        if (iores)
            delete iores;
        return ERROR_CODE;
    }
    int idObject = iores->getCellAsInt (0, 0);
    delete iores;
    return idObject;
}

bool KKSLoader :: isApplicable (KKSSearchTemplate * st, int idCategory) const
{
    if (!st || st->id() < 0)
        return true;
    QString sql = QString("select isApplicable (%1, %2)").arg (st->id()).arg (idCategory);
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() != 1)
    {
        if (res)
            delete res;
        return false;
    }
    bool isa = res->getCellAsBool (0, 0);
    delete res;
    return isa;
}

bool KKSLoader :: isIOCreated (const KKSCategory * cat) const
{
    if (!cat)
        return false;
    QString sql = QString("select isIOBycat(%1);").arg (cat->id());
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() != 1)
    {
        if (res)
            delete res;
        return false;
    }
    bool isc = res->getCellAsBool (0, 0);
    delete res;
    return isc;
}

bool KKSLoader :: isIOSynced (const KKSCategory * cat) const
{
    if (!cat)
        return false;
    QString sql = QString("select isIOSynced(%1);").arg (cat->id());
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() != 1)
    {
        if (res)
            delete res;
        return false;
    }
    bool isc = res->getCellAsBool (0, 0);
    delete res;
    return isc; 
}