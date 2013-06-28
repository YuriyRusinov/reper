/***********************************************************************
 * Module:  KKSEIOFactory.cpp
 * Author:  sergey
 * Modified: 16 декабря 2008 г. 12:45:46
 * Purpose: Implementation of the class KKSEIOFactory
 ***********************************************************************/

#include <QProgressDialog>
#include <QtDebug>

#include "KKSEIOFactory.h"
#include "kksdatabase.h"

#include "defines.h"
#include "KKSObjectExemplar.h"
#include "KKSObject.h"
#include "KKSCategory.h"
#include "KKSAttrType.h"
#include "KKSRubric.h"
#include "KKSFileLoader.h"
#include "KKSPPFactory.h"
#include "KKSFile.h"
#include "KKSState.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSEIOFactory::KKSEIOFactory()
// Purpose:    Implementation of KKSEIOFactory::KKSEIOFactory()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSEIOFactory::KKSEIOFactory()
{
    db = NULL;
    fileLoader = NULL;
    m_ppFactory = NULL;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSEIOFactory::~KKSEIOFactory()
// Purpose:    Implementation of KKSEIOFactory::~KKSEIOFactory()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSEIOFactory::~KKSEIOFactory()
{
   // TODO : implement
}

void KKSEIOFactory::setDb(KKSDatabase * _db)
{
    db = _db;
}


void KKSEIOFactory::setParams( 
                             KKSFileLoader * _fileLoader, 
                             KKSPPFactory * _ppF,
                             KKSDatabase * _db)
{
    db = _db;
    //loader = _loader;
    m_ppFactory = _ppF;
    fileLoader = _fileLoader;
    //eiof = _eiof;
}

KKSDatabase * KKSEIOFactory::getDb() const
{
    return db;
}


int KKSEIOFactory::insertEIO(KKSObjectExemplar* eio, 
                             const KKSCategory* cat, 
                             const QString& table, 
                             bool bImported,
                             QWidget * parent) const
{
    if(!eio || !eio->io())
        return ERROR_CODE;

    int res = insertRecord(eio, cat, table, bImported);
    if (res <= 0)
        return ERROR_CODE;

    int rres = insertIncludes (eio);
    if (rres <=0)
        return ERROR_CODE;
    return res;
}

int KKSEIOFactory::updateEIO(KKSObjectExemplar* eio, 
                             const KKSCategory* cat, 
                             const QString& table,
                             QWidget * parent) const
{
    if(!eio || !eio->io())
        return ERROR_CODE;

    int res = updateRecord(eio, cat, table, parent);
    if (res <= 0)
        return ERROR_CODE;
    int rres = updateIncludes (eio);
    if (rres <=0)
        return ERROR_CODE;
    commitRubrics (eio->rootRubric());
    return res;
}

int KKSEIOFactory::deleteEIO(KKSObjectExemplar* eio, const QString& table) const
{
    if(!eio || !eio->io())
        return ERROR_CODE;

    return deleteRecord(eio, table);
}


////////////////////////////////////////////////////////////////////////
// Name:       KKSRecordFactory::insertRecord(KKSObjectExemplar* eio)
// Purpose:    Implementation of KKSRecordFactory::insertRecord()
// Создаем новый ЭИО в соответствующей таблице
// При этом для тех атрибутов, которые указаны как NULL, если они имеют значения по умолчанию 
// и значение параметра isMandatory = true, то для таких атрибутов подставляется значение по умолчанию.
// Parameters:
// - eio
// Return:     int
////////////////////////////////////////////////////////////////////////

int KKSEIOFactory::insertRecord(KKSObjectExemplar* eio, 
                                const KKSCategory * cat, 
                                const QString & table, 
                                bool bImported,
                                QWidget * parent) const
{
    if(!db || !eio)
        return ERROR_CODE;

    KKSObject * io = eio->io();
    if(!io)
        return ERROR_CODE;

    qDebug () << __PRETTY_FUNCTION__ << table << io->tableName();
    //берем название таблицы, в которую необходимо будет сделать инсерт
    QString tableName = table.isEmpty() ? io->tableName() : table;

    /*if (cat)
    {
        for (KKSMap<int, KKSCategoryAttr*>::const_iterator pa = cat->attributes().constBegin(); \
             pa != cat->attributes().constEnd(); \
             pa++)
            qDebug () << __PRETTY_FUNCTION__ << pa.key() << pa.value()->code (false) << pa.value()->code (true);
    }
    */

    const KKSCategory * category = 0;
    if (cat)
        category = cat;
    else
    {
        category = io->category();
        if (!category)
            return ERROR_CODE;

        //берем подчиненную (дочернюю) категорию ИО, которая описывает таблицу, в которую делаем инсерт
        category = category->tableCategory();
        if (!category)
            return ERROR_CODE;
    }

    /*for (KKSMap<int, KKSCategoryAttr*>::const_iterator pa = category->attributes().constBegin(); \
         pa != category->attributes().constEnd(); \
         pa++)
        qDebug () << __PRETTY_FUNCTION__ << pa.key() << pa.value()->code (false) << pa.value()->code (true);*/

    //берем список атрибутов (колонок таблицы) дочерней категории ИО
    KKSMap<int, KKSCategoryAttr *> attrs = category->attributes();
    //также берем список значений соответствующих атрибутов у ЭИО
    KKSList<KKSAttrValue* > attrValues = eio->attrValues();

    QString query;
    QString exQuery;//если таблица содержит атрибуты типа atCheckListEx (отношение многие-ко-многим)

    //сохраняем в БД состояние ЭИО, если оно не создано в БД
    if(eio->state() && eio->state()->id() <= 0){
        m_ppFactory->insertState(eio->state());
    }
    
    //данный метод генерит реальный SQL-запрос в БД на инсерт в таблицу
    qint64 id = generateInsertQuery(eio, 
                                    tableName, 
                                    attrs, 
                                    attrValues, 
                                    query, 
                                    exQuery, 
                                    bImported, 
                                    io->id() <= _MAX_SYS_IO_ID_ ? true : false);
    if(id == ERROR_CODE || query.isEmpty())
        return ERROR_CODE;

    //qWarning() << __PRETTY_FUNCTION__ << query;
    KKSResult * res = db->execute(query);
    if(!res || (res->resultStatus() != KKSResult::CommandOk && res->resultStatus() != KKSResult::TuplesOk)){
        if(res){
            qWarning() << res->errorMessage();
            delete res;
        }
        return ERROR_CODE;
    }
    delete res;

    //выполняем запросы на инсерты в таблицы с отношениями многие-ко-многим
    if(!exQuery.isEmpty()){
        KKSResult * res = db->execute(exQuery);
        if(!res || (res->resultStatus() != KKSResult::CommandOk && res->resultStatus() != KKSResult::TuplesOk)){
            if(res){
                qWarning() << res->errorMessage();
                delete res;
            }
            return ERROR_CODE;
        }
        delete res;
    }

    //после того, как ЭИО сохранен в БД, присваиваем ему ИД
    eio->setId(id);


    //Сохраняем значения показателей ЕИО
    //добавляем значения атрибутов
    bool ok = insertIndValues(eio);
    if(ok != OK_CODE){
        eio->setId(-1);
        return ERROR_CODE;
    }

    //добавляем прикрепленные файлы
    //setInTransaction();
    ok = insertFiles(eio, parent);
    //restoreInTransaction();
    if(ok != OK_CODE){
        //if(!inTransaction())
            //db->rollback();
        eio->setId(-1);
        return ERROR_CODE;
    }


    return OK_CODE;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSRecordFactory::updateRecord(const KKSObjectExemplar* eio)
// Purpose:    Implementation of KKSRecordFactory::updateRecord()
// Parameters:
// - eio
// Return:     int
////////////////////////////////////////////////////////////////////////

int KKSEIOFactory::updateRecord(KKSObjectExemplar* eio, 
                                const KKSCategory* cat, 
                                const QString & table,
                                QWidget * parent) const
{
    if(!db || !eio)
        return ERROR_CODE;

    if(eio->id() <= 0)
        return ERROR_CODE;

    KKSObject * io = eio->io();
    if(!io)
        return ERROR_CODE;

    //берем название таблицы, в которую необходимо будет сделать апдейт
    QString tableName = table.isEmpty () ? io->tableName() : table;

    const KKSCategory * category = 0;
    if (cat)
        category = cat;
    else
    {
        category = io->category();
        if(!category)
            return ERROR_CODE;

        //берем подчиненную (дочернюю) категорию ИО, которая описывает таблицу, в которую делаем апдейт
        category = category->tableCategory();
        if(!category)
            return ERROR_CODE;
    }

    //берем список атрибутов (колонок таблицы) дочерней категории ИО
    KKSMap<int, KKSCategoryAttr *> attrs = category->attributes();
    //также берем список значений соответствующих атрибутов у ЭИО
    KKSList<KKSAttrValue* > attrValues = eio->attrValues();

    QString query;
    QString exQuery;
    
    
    //сохраняем в БД состояние ЭИО, если оно не создано в БД
    if(eio->state() && eio->state()->id() <= 0){
        m_ppFactory->insertState(eio->state());
    }

    //данный метод генерит реальный SQL-запрос в БД на апдейт в таблицу
    qint64 id = generateUpdateQuery(eio,
                                    tableName, 
                                    attrs, 
                                    attrValues, 
                                    eio->id(), 
                                    query, 
                                    exQuery, 
                                    io->id() <= _MAX_SYS_IO_ID_ ? true : false);
    if(id == ERROR_CODE || (query.isEmpty() && exQuery.isEmpty()))
        return ERROR_CODE;

    qWarning() << __PRETTY_FUNCTION__ << query;

    if(!query.isEmpty()){
        KKSResult * res = db->execute(query);
        if(!res || (res->resultStatus() != KKSResult::CommandOk && res->resultStatus() != KKSResult::TuplesOk)){
            if(res){
                qWarning() << res->errorMessage();
                delete res;
            }
            return ERROR_CODE;
        }
        delete res;
    }

    //выполняем запросы на апдейты в таблицы с отношениями многие-ко-многим
    //выполняется это путем удаления старых записей и создания новых
    
    if(!exQuery.isEmpty()){
        KKSResult * res = db->execute(exQuery);
        if(!res || (res->resultStatus() != KKSResult::CommandOk && res->resultStatus() != KKSResult::TuplesOk)){
            if(res){
                qWarning() << res->errorMessage();
                delete res;
            }
            return ERROR_CODE;
        }
        delete res;
    }

    //Сохраняем значения показателей ЕИО
    //добавляем значения атрибутов
    bool ok = updateIndValues(eio);
    if(ok != OK_CODE){
        return ERROR_CODE;
    }

    if(eio->m_filesModified){
        //изменяем прикрепленные файлы
        ok = updateFiles(eio, parent);
        if(ok != OK_CODE){
            //if(!inTransaction())
                //db->rollback();
            return ERROR_CODE;
        }
    }

    return OK_CODE;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSRecordFactory::deleteRecord(int id, const QString & table)
// Purpose:    Implementation of KKSRecordFactory::deleteRecord()
// Parameters:
// - id
// - table
// Return:     int

// метод удаляет запись из таблицы. 
// Если таблица имела атрибуты типа atCheckListEx (отношение многие-ко-кногим),
// то их удаление из промежуточной таблицы происходит автоматически, 
// поскольку они создаются с ограничением ON DELETE CASCADE
////////////////////////////////////////////////////////////////////////

int KKSEIOFactory::deleteRecord(qint64 id, const QString & table) const
{
    if(!db)
        return ERROR_CODE;

    QString sql = QString("delete from %1 where id = %2::int8").arg(table).arg(id);

    KKSResult * res = db->execute(sql);
    if(!res || (res->resultStatus() != KKSResult::CommandOk && res->resultStatus() != KKSResult::TuplesOk)){
        if(res){
            qWarning() << res->errorMessage();
            delete res;
        }
        return ERROR_CODE;
    }
    
    delete res;

    sql = QString("select eioDeleteIndicators(%1::int8);").arg(id);

    return OK_CODE;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSRecordFactory::deleteRecord(KKSObjectExemplar* eio)
// Purpose:    Implementation of KKSRecordFactory::deleteRecord()
// Данный метод удаляет из БД соответствующую запись, а также изменяет идентификатор текущего ЕИО на -1
// Это важно, поскольку данный ЕИО при удачном выполнении метода больше не соответствует ни одной записи в БД
// Parameters:
// - eio
// Return:     int
////////////////////////////////////////////////////////////////////////

int KKSEIOFactory::deleteRecord(KKSObjectExemplar* eio, const QString& table) const
{
    if(!eio || !eio->io())
        return ERROR_CODE;
    
    QString tableName = table.isEmpty () ? eio->io()->tableName() : table;
    qint64 id = eio->id();

    int res = deleteRecord(id, tableName);
    if(res != OK_CODE)
        return ERROR_CODE;

    eio->setId(-1);

    return OK_CODE;
}

//генерится SQL-запрос следующего вида:
//INSERT INTO <tableName> (id, <attributes>) values (idValue, <attr_values>);
//при этом делается запрос к БД с целью получения idValue
//метод возвращает идентификатор записи, который получит новый ЭИО при выполнении сгенерированного запроса
qint64 KKSEIOFactory::generateInsertQuery(const KKSRecord * rec,
                                          const QString & tableName, 
                                          const KKSMap<int, KKSCategoryAttr *> & attrs, 
                                          const KKSList<KKSAttrValue *> & attrValues, 
                                          QString & query,
                                          QString & exQuery, /*для обработки атрибутов типа atCheckListEx*/
                                          bool bImported,
                                          bool isSys) const
{
    if (tableName.isEmpty())
        return ERROR_CODE;
    
    query.clear();
    exQuery.clear();

    //TODO: необходимо учесть, что значение атрибута, который является ссылкой, 
    //должно быть равно значению идентификатора соответствующей записи
    //при этом необходима поддержка хранения значения атрибута 
    //с точки зрения не системы (id), а пользователя (column_name в KKSAttribute)

    int count = 0;
    
    //В настоящее время мы загружаем экземпляры информационных объектов таким образом, 
    //что количество атрибутов в их категориях равно количеству созданных значений этих атрибутов для ЕИО
    if((count = attrs.count()) != attrValues.count()){
        //тем не менее при импорте табличных данных из внешнего файла может сложиться ситуация, при которой поле id отсутствует
        //в этом случае мы это можем проигнорировать и продолжить работу
        //при этом мы полагаем, что поле id в списке attrs есть всегда
        if(count != attrValues.count()+1)
            return ERROR_CODE;

        bool hasID = false;
        count = attrValues.count();
        for(int i=0; i<count; i++){
            if(attrValues.at(i)->attribute()->code() == "id"){
                hasID = true;
                break;
            }
        }
        if(hasID)
            return ERROR_CODE;
    }

    qint64 idValue = 0;
    QString attrArray;
    QString valueArray;
    
    idValue = getNextSeq(tableName, "id");
    if(idValue == ERROR_CODE)
        return ERROR_CODE;

    for(int i=0; i<count; i++){

        KKSAttrValue * attrValue = attrValues.at(i);
        KKSCategoryAttr * attr = attrValue->attribute();

        //пропускаем идентификатор записи в таблице БД. Его бы зададим отдельно
        if(attr->code() == "id")
            continue;

        //Атрибут может описывать поле таблицы. 
        //В этом случае удобно разделить понятие "Название" и "Название Колонки Таблицы". 
        //Данное поле хранит "Название Колонки Таблицы"
        QString code = attr->code(true);

        KKSAttrType::KKSAttrTypes iType = attr->type()->attrType();

        //пропускаем атрибуты atCheckListEx
        //для них генерим отдельные запросы на инсерт 
        //в переходную таблицу
        if(iType == KKSAttrType::atCheckListEx){

            //коррекция для системных справочников, имеющих атрибут данного типа
            //такими справочниками являются organization, units, position
            //для них названия переходных таблиц являются жестко заданными при генерации БД
            QString rTable = tableName + "_" + attr->tableName();
            QString refTable;
            if(rTable == ORGANIZATION_WORK_MODE || 
               rTable == POSITION_WORK_MODE || 
               rTable == UNITS_WORK_MODE ||
               rTable == IO_OBJECTS_ORGANIZATION ||
               rTable == USER_CHAINS_ORGANIZATION ||
               rTable == REPORT_ORGANIZATION ||
               rTable == GUARD_OBJ_DEVICES_TSO ||
               rTable == ACCESS_CARDS_ACCESS_PLAN_TSO ||
               rTable == MAIL_LISTS_POSITION  ||
               rTable == LIFE_CYCLE_IO_STATES ||
               rTable == IO_PROCESSING_ORDER_CHAINS ||
               rTable == SHU_DLS_POSITION
               )
            {
                refTable = rTable;
            }
            else
                refTable = rTable + "_ref_" + QString::number(attr->id());

            QString ids;

            QString mainAttr = QString("id_%1").arg(tableName);
            QString childAttr = QString("id_%1").arg(attr->tableName());
            
            //Если записи импортируются из внешнего файла, то необходимо получить значения соответствующих идентификаторов
            if(bImported){
                
                KKSValue value = attrValue->value();
                
                QString cCast;
                const KKSAttrType * a = attr->refColumnType();
                if(a){
                    if(a->attrType() == KKSAttrType::atString || 
                       a->attrType() == KKSAttrType::atFixString ||
                       a->attrType() == KKSAttrType::atText)
                    {
                        cCast = "::varchar[]";
                    }

                }
                if(cCast.isEmpty())
                    cCast = "::int8[]";

                QString aQuery = QString ("select 'ARRAY[' || array_to_string(array_agg( '''' || %4 || ''''), ',') || ']%5' from %1 where %2 = ANY (ARRAY[%3]) limit 1")
                                                 .arg (attr->tableName ())
                                                 .arg (attr->columnName ())
                                                 .arg (value.value())
                                                 .arg (attr->refColumnName().isEmpty() ? "id" : attr->refColumnName())
                                                 .arg (cCast);
                
                KKSResult * aRes = db->execute (aQuery);
                
                if (!aRes || aRes->getRowCount () == 0){
                    if(attr->isMandatory()){
                        KKSValue defValue = attr->defValue();
                        ids = defValue.valueForInsert();
                    }
                    else{
                        ids = "NULL";
                    }
                }
                else{
                    ids = aRes->getCellAsString(0, 0);
                }

                if (aRes)
                    delete aRes;
            }
            else{
                ids = attrValue->value().valueForInsert();
            }
            
            if(ids.isEmpty())
                ids = "NULL";

            exQuery += QString("select aInsertExValues('%1', %2, %3, '%4', '%5');")
                                .arg(refTable)
                                .arg(idValue)
                                .arg(ids)
                                .arg(mainAttr)
                                .arg(childAttr);
            

            continue;
        }

        attrArray += code;
                
        //-----
        /*
        Выделяется 3 варианта:
        1 - атрибут не является ссылочным или имеет тип ссылка на элемент справочника и реально ссылается на поле id
        2 - атрибут имеет тип ссылка на элемент справочника и реально ссылается на поле, отличное от id
        3 - осуществляется импорт табличных данных (записей справочников) из внешнего файла в БД. В этом случае значение атрибута
            будет задано не как идентификатор записи в соответствующей таблице,
            а как значение поля (строка в общем случае), тогда мы ищем 
            (а при необходимости добавляем) нужную нам запись в БД.
        Нижеприведенный код рассматривает все три варианта
        */
        
        KKSValue value = attrValue->value();
        if (iType == KKSAttrType::atParent && bImported)
            qDebug () << __PRETTY_FUNCTION__ << value.value();
        
        if ((iType == KKSAttrType::atList || iType == KKSAttrType::atParent) && 
            bImported && //в этом случае надо обязательно делать запрос за значением идентификатора записи.
            value.value() != QString("NULL") &&
            !value.value().isEmpty ()
           )
        {
            QString aQuery = QString ("SELECT %4 from %1 where %2='%3' limit 1;")
                                             .arg ((iType == KKSAttrType::atParent ? tableName : attr->tableName ()))
                                             .arg (iType == KKSAttrType::atList && !bImported ? attr->columnName () : QString("id"))
                                             .arg (value.value())
                                             .arg(attr->refColumnName().isEmpty() ? "id" : attr->refColumnName());
            qDebug () << __PRETTY_FUNCTION__ << aQuery;
            KKSResult * aRes = db->execute (aQuery);

            if (!aRes || aRes->getRowCount () == 0)
            {
                QString atName = (iType == KKSAttrType::atParent ? tableName : attr->tableName ());
                qDebug () << __PRETTY_FUNCTION__ << atName << value.value ();
                qint64 idV = getNextSeq (atName, "id");
                aQuery = QString ("INSERT INTO %1 (id, %2) VALUES (%4, '%3');")
                                    .arg (atName)
                                    .arg (attr->columnName ())
                                    .arg (value.value ())
                                    .arg (idV);
                KKSResult * aResIns = db->execute (aQuery);
                if (aResIns)
                {
                    valueArray += QString::number (idV);
                    delete aResIns;
                }
                else
                    return ERROR_CODE;
            }
            else
                valueArray += QString::number (aRes->getCellAsInt (0, 0));

            if (aRes)
                delete aRes;
        }
        else if((iType == KKSAttrType::atCheckList) && 
                bImported && //в этом случае надо обязательно делать запрос за значением идентификаторов записей.
                value.value() != QString("NULL") &&
                !value.value().isEmpty ())
        {
            QString cCast;
            const KKSAttrType * a = attr->refColumnType();
            if(a){
                if(a->attrType() == KKSAttrType::atString || 
                   a->attrType() == KKSAttrType::atFixString ||
                   a->attrType() == KKSAttrType::atText)
                {
                    cCast = "::varchar[]";
                }

            }
            if(cCast.isEmpty())
                cCast = "::int8[]";

            QString aQuery = QString ("select 'ARRAY[' || array_to_string(array_agg( '''' || %4 || ''''), ',') || ']%5' from %1 where %2 = ANY (ARRAY[%3]) limit 1")
                                             .arg (attr->tableName ())
                                             .arg (attr->columnName ())
                                             .arg (value.value())
                                             .arg (attr->refColumnName().isEmpty() ? "id" : attr->refColumnName())
                                             .arg (cCast);
            
            KKSResult * aRes = db->execute (aQuery);
            
            if (!aRes || aRes->getRowCount () == 0){
                if(attr->isMandatory()){
                    KKSValue defValue = attr->defValue();
                    valueArray += defValue.valueForInsert();
                }
                else
                    valueArray += "NULL";                
                
                return ERROR_CODE;
            }
            else{
                valueArray += aRes->getCellAsString(0, 0);
            }

            if (aRes)
                delete aRes;
        }
        //-----
        else
        {
            //если значение атрибута отсутствует, то используем значение по умолчанию (при его наличии), 
            //но только в случае, когда у атрибута параметр isMandatory = true.
            if(value.isNull() ||
               ((iType == KKSAttrType::atList || iType == KKSAttrType::atParent) && value.valueForInsert() == "''")
              )
            {
                if(attr->isMandatory()){
                    KKSValue defValue = attr->defValue();
                    valueArray += defValue.valueForInsert();
                }
                else
                    valueArray += "NULL";
            }
            else
                valueArray += value.valueForInsert();
        }

        if(i<count-1){
            attrArray += ", ";
            valueArray += ", ";
        }
    }

    if(attrArray.endsWith(", ")){
        attrArray.remove(attrArray.length()-2, 2);
        valueArray.remove(valueArray.length()-2, 2);
    }

    //добавляем состояние, если надо
    int idState = 1;
    if(rec && rec->state()){
        idState = rec->state()->id();
    }
    
    QString rIcon = KKSRecord::defIconAsString();
    if(rec){
        rIcon = rec->iconAsString();
    }
    
    qint64 fillColor = -1;
    qint64 textColor = -1;
    if(rec){
        fillColor = rec->recordFillColor().rgba();
        textColor = rec->recordTextColor().rgba();
    }
    
    if(isSys){
        if(attrArray.trimmed().isEmpty())
            query = QString("INSERT INTO %1 (id) VALUES (%2);").arg(tableName).arg(idValue);
        else
            query = QString("INSERT INTO %1 (id, %2) VALUES (%3, %4);").arg(tableName).arg(attrArray).arg(idValue).arg(valueArray);
    }
    else{
        if(attrArray.trimmed().isEmpty()){
            query = QString("INSERT INTO %1 (id, id_io_state, r_icon, record_fill_color, record_text_color) VALUES (%2, %3, %4, %5::int8, %6::int8);")
                            .arg(tableName)
                            .arg(idValue)
                            .arg(idState)
                            .arg(rIcon.isEmpty() ? QString("NULL") : QString("'") + rIcon + QString("'"))
                            .arg(fillColor < 0 ? QString("NULL") : QString::number(fillColor))
                            .arg(textColor < 0 ? QString("NULL") : QString::number(textColor));
        }
        else{
            query = QString("INSERT INTO %1 (id, id_io_state, r_icon, record_fill_color, record_text_color, %2) VALUES (%3, %5, %6, %7::int8, %8::int8, %4);")
                            .arg(tableName)
                            .arg(attrArray)
                            .arg(idValue)
                            .arg(valueArray)
                            .arg(idState)
                            .arg(rIcon.isEmpty() ? QString("NULL") : QString("'") + rIcon + QString("'"))
                            .arg(fillColor < 0 ? QString("NULL") : QString::number(fillColor))
                            .arg(textColor < 0 ? QString("NULL") : QString::number(textColor));
        }
    }

    return idValue;
}

//генерится SQL-запрос следующего вида:
//UPDATE <tableName> SET (<attribute1=value1, ...>) where id = <id>;
//метод возвращает OK_CODE при успехе
qint64 KKSEIOFactory::generateUpdateQuery(const KKSRecord * rec,
                                          const QString & tableName, 
                                          const KKSMap<int, KKSCategoryAttr *> & attrs, 
                                          const KKSList<KKSAttrValue *> & attrValues, 
                                          qint64 idEIO,
                                          QString & query,
                                          QString & exQuery,
                                          bool isSys) const
{
    query.clear();
    exQuery.clear();

    //необходимо учесть, что значение атрибута, который является ссылкой, 
    //должно быть равно значению идентификатора соответствующей записи
    //при этом необходима поддержка хранения значения атрибута 
    //с точки зрения не системы (id), а пользователя (column_name в KKSAttribute)

    int count = 0;
    //В настоящее время мы загружаем экземпляры информационных объектов таким образом, 
    //что количество атрибутов в их категориях равно количеству созданных значений этих атрибутов для ЕИО
    if((count = attrs.count()) != attrValues.count())
        return ERROR_CODE;

    QString attrArray;
    
    for(int i=0; i<count; i++){
        KKSAttrValue * attrValue = attrValues.at(i);
        KKSCategoryAttr * attr = attrValue->attribute();

        //пропускаем идентификатор записи в таблице БД. Его бы зададим отдельно
        if(attr->code() == "id")
            continue;

        //Атрибут может описывать поле таблицы. 
        //В этом случае удобно разделить понятие "Название" и "Название Колонки Таблицы". 
        //Данное поле хранит "Название Колонки Таблицы"
        QString code = attr->code(true);


        //пропускаем атрибуты atCheckListEx
        //для них генерим отдельные запросы на инсерт 
        //в переходную таблицу
        if(attr->type()->attrType() == KKSAttrType::atCheckListEx){
            //коррекция для системных справочников, имеющих атрибут данного типа
            //такими справочниками являются organization, units, position, ряд других
            //для них названия переходных таблиц являются жестко заданными при генерации БД
            QString rTable = tableName + "_" + attr->tableName();
            QString refTable;
            if(rTable == ORGANIZATION_WORK_MODE || 
               rTable == POSITION_WORK_MODE || 
               rTable == UNITS_WORK_MODE ||
               //rTable == IO_OBJECTS_ORGANIZATION ||
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
            else if(rTable == IO_OBJECTS_ORGANIZATION){
                continue;//информация о синхронизируемых организациях обновляется отдельно! Здесь это делать нельзя
            }
            else
                refTable = rTable + "_ref_" + QString::number(attr->id());

            QString ids = attrValue->value().valueForInsert();
            QString mainAttr = QString("id_%1").arg(tableName);
            QString childAttr = QString("id_%1").arg(attr->tableName());
            exQuery += QString("select aUpdateExValues('%1', %2, %3, '%4', '%5');")
                                .arg(refTable)
                                .arg(idEIO)
                                .arg(ids)
                                .arg(mainAttr)
                                .arg(childAttr);
            continue;
        }

        attrArray += code;
        attrArray += " = ";
        KKSValue value = attrValue->value();
        //если значение атрибута отсутствует, то используем значение по умолчанию (при его наличии), 
        //но только в случае, когда у атрибута параметр isMandatory = true.
        if(value.isNull()){
            if(attr->isMandatory()){
                KKSValue defValue = attr->defValue();
                attrArray += defValue.valueForInsert();
            }
            else
                attrArray += "NULL";
        }
        else
            attrArray += value.valueForInsert();

        if(i<count-1){
            attrArray += ", ";
        }
    }

    if(attrArray.endsWith(", ")){
        attrArray.remove(attrArray.length()-2, 2);
    }

    
    //добавляем состояние, если надо
    int idState = 1;
    if(rec && rec->state()){
        idState = rec->state()->id();
    }
    
    QString rIcon = KKSRecord::defIconAsString();
    if(rec){
        rIcon = rec->iconAsString();
    }

    qint64 fillColor = -1;
    qint64 textColor = -1;
    if(rec){
        if(rec->recordFillColor().isValid())
            fillColor = rec->recordFillColor().rgba();
        if(rec->recordTextColor().isValid())
            textColor = rec->recordTextColor().rgba();
    }

    if(isSys){
        if(!attrArray.trimmed().isEmpty())
            query = QString("UPDATE %1 SET %2 WHERE id = %3::int8;")
                            .arg(tableName)
                            .arg(attrArray)
                            .arg(idEIO);
    }
    else{
        if(!attrArray.trimmed().isEmpty()){
            query = QString("UPDATE %1 SET id_io_state = %2, r_icon = %5, record_fill_color = %6::int8, record_text_color = %7::int8, %3 WHERE id = %4::int8;")
                            .arg(tableName)
                            .arg(idState)
                            .arg(attrArray)
                            .arg(idEIO)
                            .arg(rIcon.isEmpty() ? QString("NULL") : QString("'") + rIcon + QString("'"))
                            .arg(fillColor < 0 ? QString("NULL") : QString::number(fillColor))
                            .arg(textColor < 0 ? QString("NULL") : QString::number(textColor));
        }
    }

    return OK_CODE;
}

qint64 KKSEIOFactory::getNextSeq(QString tableName, QString idColumn) const
{
    QString tName;
    QString sql = QString("select ref_table_name from io_objects where table_name = '%1'").arg(tableName);
    KKSResult * res = db->execute(sql);
    if(!res)
        return ERROR_CODE;

    if(!res->isEmpty(0, 0)){
        tName = res->getCellAsString(0, 0);
    }
    else
        tName = tableName;

    delete res;
    
    QString query = QString("select getNextSeq('%1', '%2')").arg(tName).arg(idColumn);
    res = db->execute(query);
    if(!res)
        return ERROR_CODE;

    qint64 id = res->getCellAsInt64(0, 0);
    
    delete res;
    
    if(id < 0)
        return ERROR_CODE;

    return id;
}

int KKSEIOFactory::deleteAllRecords(const QString & table) const
{
    if(table.isEmpty())
        return ERROR_CODE;

    QString sql = QString("delete from %1").arg(table);
    int res = db->executeCmd(sql);
    if(res == ERROR_CODE)
        return ERROR_CODE;
    
    return OK_CODE;
}

int KKSEIOFactory::insertEIOList(KKSList<KKSObjectExemplar*> eioList,
                                 const QStringList& uidsSorted,
                                 QMap<QString, qint64>& uids,
                                 const KKSCategory* cat, 
                                 const QString & table, 
                                 QProgressDialog *pgDial, 
                                 bool bImported) const
{
    if(table.isEmpty() || eioList.count() != uids.count())
        return ERROR_CODE;

    int count = eioList.count();
    if(count == 0)
        return OK_CODE;
    
    if (pgDial)
    {
        pgDial->setMinimum (0);
        pgDial->setMaximum (count);
    }

    QMap<QString, qint64>::const_iterator pu = uids.constBegin();
    for(int i=0; i<count; i++)
    {
        if (pgDial)
            pgDial->setValue (i);
        
        db->begin();
        
        KKSObjectExemplar * eio = eioList.at(i);
        QString puid;
        for (int ii=0; ii<eio->attrValues().count(); ii++)
        {
            KKSAttrValue * av = eio->attrValueIndex(ii);
            if (av && 
                av->attribute() &&
                av->attribute()->type()->attrType() == KKSAttrType::atParent &&
                !av->value().value().isEmpty())
                //av->value().value().toInt() > 0 &&
                //av->value().value().toInt() <= i)
            {
                puid = uids.find (av->value().value()).key();
                //int pKey = av->value().value().toInt();
                int pId = uids.value (puid);//eioList.at (pKey-1)->id();
                qDebug () << __PRETTY_FUNCTION__ << puid << av->value().value() << pId;
                KKSValue val = KKSValue ((pId > 0 ? QString::number(pId) : QString()), KKSAttrType::atParent);
                av->setValue(val);
            }
        }
//        if(!eio->io() || eio->io()->tableName() != table){
//            db->rollback();
//            return ERROR_CODE;
//        }

        int res = insertEIO(eio, cat, table, bImported);
        if(res != OK_CODE){
            db->rollback();
            continue;
        }
        //pu.value() = eio->id();
        QString skey = uidsSorted[i];
        uids[skey] = eio->id();

        db->commit();
        qDebug () << __PRETTY_FUNCTION__ << uids;
        pu++;
    }
    
    
    return OK_CODE;
}

int KKSEIOFactory::insertCommand(KKSObjectExemplar* eio) const
{
    if(!eio)
        return ERROR_CODE;

    QString sql = QString("select cmdInsertOut(%1::int4, %2::int4, %3::int4, %4, %5::int4[2], %6::varchar, %7::int4, %8::int4)")
        .arg(eio->attrValue(ATTR_ID_DL_TO)->value().valueForInsert())
        .arg(eio->attrValue(ATTR_ID_DL_EXECUTOR)->value().valueForInsert())
        .arg(eio->attrValue(ATTR_ID_IO_CATEGORY)->value().valueForInsert())
        .arg(eio->attrValue(ATTR_EXEC_DATETIME)->value().valueForInsert())
        .arg(eio->attrValue(ATTR_EXEC_PERIOD)->value().valueForInsert())
        .arg(eio->attrValue(ATTR_MESSAGE_BODY)->value().valueForInsert())
        .arg(eio->attrValue(ATTR_ID_URGENCY_LEVEL)->value().valueForInsert())
        .arg(eio->attrValue(ATTR_ID_IO_OBJECT_ATT)->value().valueForInsert());
    
    
    KKSResult * res = db->execute(sql);
    if(!res)
        return ERROR_CODE;

    int id = res->getCellAsInt(0, 0);
    
    delete res;
    
    if(id < 0)
        return ERROR_CODE;


    return OK_CODE;
}

int KKSEIOFactory::insertMessage(KKSObjectExemplar* eio) const
{
    if(!eio)
        return ERROR_CODE;

    QString sql = QString("select msgInsertOut(%1::int4, %2, %3, %4)")
        .arg(eio->attrValue(ATTR_ID_IO_OBJECT)->value().valueForInsert())
        .arg(eio->attrValue(ATTR_MESSAGE_BODY)->value().valueForInsert())
        .arg(eio->attrValue(ATTR_ID_DL_RECEIVER)->value().valueForInsert())
        .arg(eio->attrValue(ATTR_ID_URGENCY_LEVEL)->value().valueForInsert());
    
    
    KKSResult * res = db->execute(sql);
    if(!res)
        return ERROR_CODE;

    int id = res->getCellAsInt(0, 0);
    
    delete res;
    
    if(id < 0)
        return ERROR_CODE;


    return OK_CODE;
}


int KKSEIOFactory::insertTSDRecord(KKSObjectExemplar* eio) const
{
    if(!eio)
        return ERROR_CODE;

    QString sql = QString("select tsdInsertOut(%1, %2, %3, %4, %5, %6, %7)")
        .arg(eio->attrValue(ATTR_ID_IO_CATEGORY)->value().valueForInsert())
        .arg(eio->attrValue(ATTR_ID_DL_TO)->value().valueForInsert())
        .arg(eio->attrValue(ATTR_ID_DL_EXECUTOR)->value().valueForInsert())
        .arg(eio->attrValue(ATTR_ID_DL_CONTROLLER)->value().valueForInsert())
        .arg(eio->attrValue(ATTR_EXEC_DATETIME)->value().valueForInsert())
        .arg(eio->attrValue(ATTR_EXEC_PERIOD)->value().valueForInsert())
        .arg(eio->attrValue(ATTR_OSNOVANIE)->value().valueForInsert());
    
    
    KKSResult * res = db->execute(sql);
    if(!res)
        return ERROR_CODE;

    int id = res->getCellAsInt(0, 0);
    
    delete res;
    
    if(id < 0)
        return ERROR_CODE;


    return OK_CODE;
}


/*
---------------------------------------------------------------------------------
---------------------------------------------------------------------------------
*/

int KKSEIOFactory::insertIndValues(const KKSObjectExemplar * eio) const
{
    if(!eio)
        return ERROR_CODE;

    KKSList<KKSAttrValue * > indValues = eio->indValues();

    QString sql;
    QString sqlEx;
    for (int i=0; i<indValues.count(); i++)
    {
        KKSAttrValue * av = indValues[i];
        if(!av)
            continue;

        KKSCategoryAttr * a = av->attribute();
        KKSValue v = av->value();
        
        if(av->value().isNull())
            continue;

        if(a->type()->attrType() == KKSAttrType::atCheckListEx){
            //коррекция для системных справочников, имеющих атрибут данного типа
            //такими справочниками являются organization, units, position
            //для них названия переходных таблиц являются жестко заданными при генерации БД
            QString rTable = "io_object_" + a->tableName();
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
                refTable = rTable + "_ref_" + QString::number(a->id());

            QString ids = av->value().valueForInsert();
            QString mainAttr = QString("id_io_object");
            QString childAttr = QString("id_%1").arg(a->tableName());
            sqlEx += QString("select aInsertExValues('%1', %2, %3, '%4', '%5');")
                                .arg(refTable)
                                .arg(eio->id())
                                .arg(ids)
                                .arg(mainAttr)
                                .arg(childAttr);
        }

        QDateTime dt = av->startDateTime();
        QString tVal; 
        QString dtStart;
        QString dtStop;
        QString dtMeas;
        
        if(dt.isValid()){
            tVal = dt.toString("dd.MM.yyyy hh:mm:ss");
            dtStart = QString("to_timestamp('%1', 'DD.MM.YYYY HH24:MI:SS')::timestamp").arg(tVal);
        }
        else{
            dtStart = QString("current_timestamp::timestamp");
        }
        
        dt = av->stopDateTime();
        if(dt.isValid()){
            tVal = dt.toString("dd.MM.yyyy hh:mm:ss");
            dtStop = QString("to_timestamp('%1', 'DD.MM.YYYY HH24:MI:SS')::timestamp").arg(tVal);
        }
        else
            dtStop = QString("NULL::timestamp");

        /*
        dt = av->measDateTime();
        if(dt.isValid()){
            tVal = dt.toString("dd.MM.yyyy hh:mm:ss");
            dtMeas = QString("to_timestamp('%1', 'DD.MM.YYYY HH24:MI:SS')::timestamp").arg(tVal);
        }
        else
            dtMeas = QString("current_timestamp::timestamp");
        */

        //eio->indValue(1)->attribute()->idCategoryAttr
        sql += QString("select eioInsertIndicator(%1, %2, %3::varchar, %4, %5, %6, %7, %8);")
                              .arg(eio->id())
                              .arg(a->idRow())
                              .arg(v.valueForInsert())
                              .arg(dtStart)
                              .arg(dtStop)
                              //.arg(dtMeas)
                              .arg(av->ioSrc() ? QString::number (av->ioSrc()->id()) : QString ("NULL::int4"))
                              .arg(av->ioSrc1() ? QString::number (av->ioSrc1()->id()) : QString ("NULL::int4"))
                              .arg(av->desc().isEmpty() ? QString("NULL") : QString("'") + av->desc() + QString("'"));

    }

    if(sql.isEmpty())
        return OK_CODE;

    KKSResult * res = db->execute(sql);
    if(!res)
        return ERROR_CODE;
    if(res->getRowCount() == 0){
        delete res;
        return ERROR_CODE;
    }

    int ok = res->getCellAsInt(0, 0);
    delete res;
    if(ok != 1)
        return ERROR_CODE;

    if(!sqlEx.isEmpty()){
        KKSResult * res = db->execute(sqlEx);
        if(!res)
            return ERROR_CODE;
        if(res->getRowCount() == 0){
            delete res;
            return ERROR_CODE;
        }

        int ok = res->getCellAsInt(0, 0);
        delete res;
        if(ok != 1)
            return ERROR_CODE;
    }

    return OK_CODE;
}


int KKSEIOFactory::updateIndValues(const KKSObjectExemplar * eio) const
{
    if(!eio)
        return ERROR_CODE;

    KKSList<KKSAttrValue * > indValues = eio->indValues();

    QString sql;
    QString sqlEx;
    for (int i=0; i<indValues.count(); i++)
    {
        KKSAttrValue * av = indValues[i];
        if(!av){
            qWarning() << "WARNING!! Some of indicators of EIO is NULL pointer! Check data integrity!";
            continue;
        }

        KKSCategoryAttr * a = av->attribute();
        KKSValue v = av->value();
        
        if(a->type()->attrType() == KKSAttrType::atCheckListEx){
            //коррекция для системных справочников, имеющих атрибут данного типа
            //такими справочниками являются organization, units, position
            //для них названия переходных таблиц являются жестко заданными при генерации БД
            QString rTable = "io_object_" + a->tableName();
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
                refTable = rTable + "_ref_" + QString::number(a->id());

            QString ids = av->value().valueForInsert();
            QString mainAttr = QString("id_io_object");
            QString childAttr = QString("id_%1").arg(a->tableName());
            sqlEx += QString("select aUpdateExValues('%1', %2, %3, '%4', '%5');")
                                .arg(refTable)
                                .arg(eio->id())
                                .arg(ids)
                                .arg(mainAttr)
                                .arg(childAttr);
        }

        QDateTime dt = av->startDateTime();
        QString tVal; 
        QString dtStart;
        QString dtStop;
        QString dtMeas;
        
        if(dt.isValid()){
            tVal = dt.toString("dd.MM.yyyy hh:mm:ss");
            dtStart = QString("to_timestamp('%1', 'DD.MM.YYYY HH24:MI:SS')::timestamp").arg(tVal);
        }
        else{
            dtStart = QString("current_timestamp::timestamp");
        }
        
        dt = av->stopDateTime();
        if(dt.isValid()){
            tVal = dt.toString("dd.MM.yyyy hh:mm:ss");
            dtStop = QString("to_timestamp('%1', 'DD.MM.YYYY HH24:MI:SS')::timestamp").arg(tVal);
        }
        else
            dtStop = QString("NULL::timestamp");

        /*
        dt = av->measDateTime();
        qDebug() << dt;
        if(dt.isValid()){
            tVal = dt.toString("dd.MM.yyyy hh:mm:ss");
            dtMeas = QString("to_timestamp('%1', 'DD.MM.YYYY HH24:MI:SS')::timestamp").arg(tVal);
        }
        else
            dtMeas = QString("current_timestamp::timestamp");
        */

        
        QString s = QString("select eioUpdateIndicator(%1, %2, %3::varchar, NULL, NULL, %4, %5, %6);")
                              .arg(eio->id())
                              .arg(a->idRow())
                              .arg(v.valueForInsert())
                              //.arg(dtStart)
                              //.arg(dtStop)
                              //.arg(dtMeas)
                              .arg(av->ioSrc() ? QString::number (av->ioSrc()->id()) : QString ("NULL"))
                              .arg(av->ioSrc1() ? QString::number (av->ioSrc1()->id()) : QString ("NULL"))
                              .arg(av->desc().isEmpty() ? QString("NULL") : QString("'") + av->desc() + QString("'"));
        qDebug() << s;

        sql += s;
    }

    if(sql.isEmpty())
        return OK_CODE;

    
    KKSResult * res = db->execute(sql);
    if(!res)
        return ERROR_CODE;
    if(res->getRowCount() == 0){
        delete res;
        return ERROR_CODE;
    }

    int ok = res->getCellAsInt(0, 0);
    delete res;
    if(ok != 1)
        return ERROR_CODE;

    if(!sqlEx.isEmpty()){
        KKSResult * res = db->execute(sqlEx);
        if(!res)
            return ERROR_CODE;
        if(res->getRowCount() == 0){
            delete res;
            return ERROR_CODE;
        }

        int ok = res->getCellAsInt(0, 0);
        delete res;
        if(ok != 1)
            return ERROR_CODE;
    }

    return OK_CODE;
}

int KKSEIOFactory::updateIncludes(const KKSObjectExemplar * eio) const
{
    if(!eio || eio->id() <= 0)
        return ERROR_CODE;

    KKSRubric * rootRubric = eio->rootRubric();
    if(!rootRubric)
        return OK_CODE;

    int cnt = rootRubric->rubrics().count();
    if(cnt  == 0)
        return OK_CODE;
    
    int ok;
    if(rootRubric->id() > 0)
        ok = updateRubric(rootRubric);
    else
        ok = insertRubric(rootRubric, -1, eio->id(), true);

    if (ok <= 0 ){
        rollbackRubrics(rootRubric, true);
        return ERROR_CODE;
    }

    ok = updateRubrics(rootRubric);
    if (ok < 0 )
    {
        rollbackRubrics(rootRubric, true);
        return ERROR_CODE;
    }

    //удаляем рубрики, которые не вошли в новый перечень
    QString ids = rootRubric->getFullTreeOfIdsString();
    QString sql;
    if(ids.isEmpty())
        sql = QString("select recDeleteIncludes(%1, ARRAY[-1])").arg(eio->id());
    else
        sql = QString("select recDeleteIncludes(%1, ARRAY[%2])").arg(eio->id()).arg(ids);
    qDebug () << __PRETTY_FUNCTION__ << ids << sql;
    KKSResult * res = db->execute(sql);
    ok = -1;
    if(res){
        ok = res->getCellAsInt(0, 0);
        delete res;
    }
    if(ok < 0)
        return ERROR_CODE;

    return OK_CODE;

}

int KKSEIOFactory::insertIncludes(const KKSObjectExemplar * eio) const
{
    if(!eio || eio->id() <= 0)
        return ERROR_CODE;

    KKSRubric * rootRubric = eio->rootRubric();
    if(!rootRubric)
        return OK_CODE;

    int cnt = rootRubric->rubrics().count();
    if(cnt  == 0)
        return OK_CODE;
    
    int ok = insertRubric(rootRubric, -1, eio->id(), true);
    //
    // Теперь функция insertRubric возвращает id добавленной рубрики
    //
    if (ok < 0 ){
        rollbackRubrics(rootRubric);
        return ERROR_CODE;
    }

    ok = insertRubrics(rootRubric);
    if (ok < 0 )// != OK_CODE){
    {
        rollbackRubrics(rootRubric);
        return ERROR_CODE;
    }

    return OK_CODE;

}

int KKSEIOFactory::deleteIncludes(int idRec) const
{
    QString sql = QString("select recDeleteRubrics(%1)").arg(idRec);
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return ERROR_CODE;
    }
    int ok = res->getCellAsInt(0, 0);
    delete res;

    if(ok != 1)
        return ERROR_CODE;

    return OK_CODE;
}

int KKSEIOFactory::updateRubrics(KKSRubric * parent, int idMyDocsRubricator) const
{
    if(!parent)
        return ERROR_CODE;

    int cnt = parent->rubrics().count();

    for(int i=0; i<cnt; i++){
        KKSRubric * r = parent->rubric(i);
        
        int ok;
        if(r->id() <= 0)
            ok = insertRubric(r, parent->id(), -1, false, idMyDocsRubricator);
        else
            ok = updateRubric(r);
        
        if (ok <0 )
            return ERROR_CODE;

        ok = updateRubrics(r, idMyDocsRubricator);
        if (ok < 0 )
            return ERROR_CODE;
    }
    
    //сначала удалим все вложения в рубрику
    QString sql = QString("delete from rubric_records where id_rubric = %1").arg(parent->id());
    int ok = db->executeCmd(sql);
    if(ok != OK_CODE)
        return ERROR_CODE;

    cnt = parent->items().count();
    for(int i=0; i<cnt; i++){
        const KKSRubricItem * item = parent->item(i);
        int ok = insertRubricItem(parent->id(), item->id(), item->isAutomated());
        if(ok != OK_CODE)
            return ERROR_CODE;
    }

    return OK_CODE;

}

int KKSEIOFactory::updateRubric(KKSRubric * r) const
{
    if (!r || r->id() <= 0)
        return ERROR_CODE;

    int idRubric = r->id();
    
    QString icon = r->iconAsString();
    QString sql = QString("select * from recUpdateRubricLocal (%1, '%2', '%3', %4);")
                          .arg(r->id())
                          .arg (r->name())
                          .arg (r->desc())
                          .arg (icon.isEmpty() ? QString("NULL") : QString("'%1'").arg(icon));

    KKSResult * res = db->execute (sql);
    if (!res)
    {
        return ERROR_CODE;
    }
    idRubric = res->getCellAsInt (0, 0);
    delete res;

    r->setId (idRubric);

    return idRubric;

}

int KKSEIOFactory::insertRubrics(KKSRubric * parent, int idMyDocsRubricator) const
{
    if(!parent)
        return ERROR_CODE;

    int cnt = parent->rubrics().count();

    for(int i=0; i<cnt; i++){
        KKSRubric * r = parent->rubric(i);
        int ok = insertRubric(r, parent->id(), -1, false, idMyDocsRubricator);
        if (ok <0 )//!= OK_CODE)
            return ERROR_CODE;

        ok = insertRubrics(r, idMyDocsRubricator);
        if (ok < 0 )// != OK_CODE)
            return ERROR_CODE;
    }

    cnt = parent->items().count();
    for(int i=0; i<cnt; i++){
        const KKSRubricItem * item = parent->item(i);
        int ok = insertRubricItem(parent->id(), item->id(), item->isAutomated());
        if(ok != OK_CODE)
            return ERROR_CODE;
    }

    return OK_CODE;// parent->id();

}

int KKSEIOFactory::insertRubric(KKSRubric * r, int idParent, int idRec, bool root, int idMyDocsRubricator) const
{
    if (!r)
        return ERROR_CODE;

    int idRubric = ERROR_CODE;//eiof->getNextSeq("rubricator", "id");
    
    QString icon = r->iconAsString();
    QString sql = QString("select * from recInsertRubric(%1, %2, '%3', '%4', %5);")
                          .arg ( (root || idParent <= 0) ? QString("NULL") : QString::number(idParent))
                          .arg (root ? QString::number(idRec) : QString("NULL"))
                          .arg (r->name())
                          .arg (r->desc())
                          .arg (icon.isEmpty() ? QString("NULL") : QString("'%1'").arg(icon));

    qDebug () << __PRETTY_FUNCTION__ << sql;

    KKSResult * res = db->execute (sql);
    if (!res)
    {
        return ERROR_CODE;
    }
    idRubric = res->getCellAsInt (0, 0);
    delete res;
/*
    if (idMyDocsRubricator > 0 && r->id() == idMyDocsRubricator)
    {
        int res = createMyDocsRubricator(idRubric);
        if (res != OK_CODE)
            return ERROR_CODE;
    }
*/
    Q_UNUSED (idMyDocsRubricator);
    r->setId (idRubric);

//    int ok = updateUserPrivileges (r, idRubric);
//    qDebug () << __PRETTY_FUNCTION__ << ok;

    return idRubric;

}

int KKSEIOFactory::insertRubricItem(int idRubric, int idRec, bool isAutomated) const
{
    Q_UNUSED(isAutomated);

    if(idRubric <= 0 || idRec <= 0)
        return OK_CODE;

    QString sql = QString("insert into rubric_records (id_record, id_rubric) values(%1, %2)")
		        .arg(idRec)
                        .arg(idRubric);
    
    int ok = db->executeCmd(sql);
    if(ok != OK_CODE){
        return ERROR_CODE;
    }

    return OK_CODE;

}

void KKSEIOFactory::rollbackRubrics(KKSRubric * parent, bool forUpdate) const
{
    if(!parent)
        return;

    rollbackRubric(parent, forUpdate);
    for(int i=0; i<parent->rubrics().count(); i++){
        KKSRubric * r = parent->rubric(i);
        rollbackRubrics(r, forUpdate);
    }
}

void KKSEIOFactory::rollbackRubric(KKSRubric * r, bool forUpdate) const
{
    if(!r)
        return;

    if(forUpdate && r->m_intId > 0) //если рубрика загружена из БД, то не надо ее помечать как несозданную
        return;
    
    r->setId(-1);
    
}

void KKSEIOFactory::commitRubrics(KKSRubric * parent) const
{
    if(!parent)
        return;

    commitRubric(parent);
    for(int i=0; i<parent->rubrics().count(); i++){
        KKSRubric * r = parent->rubric(i);
        commitRubrics(r);
    }
    
}

void KKSEIOFactory::commitRubric(KKSRubric * r) const
{
    if(!r)
        return;

    r->m_intId = r->id();
}

int KKSEIOFactory::insertRubricators(KKSRubric * rootRubric, int idMyDocsRubricator, bool bMyDocs) const
{
    Q_UNUSED(bMyDocs);
    Q_UNUSED(idMyDocsRubricator);
    return 0;
}

int KKSEIOFactory::deleteRubricators(bool bMyDocs) const
{
    Q_UNUSED(bMyDocs);
    return 0;
}

int KKSEIOFactory::deleteRubric(int idRubric) const
{
    Q_UNUSED(idRubric);
    return 0;
}

int KKSEIOFactory::insertFile(qint64 idRecord, KKSFile * f, QWidget * parent) const
{
    if(!f)
        return ERROR_CODE;

    int ok = uploadFile(f, parent);
    if(ok != OK_CODE)
        return ERROR_CODE;

    QString sql = QString("select recInsertUrl(%1::int8, %2, '%3')")
                          .arg(idRecord)
                          .arg(f->id())
                          .arg(f->name());

    KKSResult * res = db->execute(sql);
    ok = res ? res->getCellAsInt(0,0) : ERROR_CODE;
    if(res)
        delete res;

    if(ok != OK_CODE){
        return ERROR_CODE;
    }

    return OK_CODE;
}

int KKSEIOFactory::insertFiles(const KKSObjectExemplar * eio, QWidget * parent) const
{
    if(!eio)
        return ERROR_CODE;

    KKSList<KKSFile * > files = eio->files();
    int ok = OK_CODE;

    for (int i=0; i<files.count(); i++)
    {
        KKSFile * f = files[i];
        if(!f)
            continue;

        ok = insertFile(eio->id(), f, parent);

        if(ok != OK_CODE){
            return ERROR_CODE;
        }
    }

    return OK_CODE;
}

int KKSEIOFactory::updateFiles(const KKSObjectExemplar * eio, QWidget * parent) const
{
    if(!eio)
        return ERROR_CODE;

    qint64 idRecord = eio->id();
    if(idRecord <= 0)
        return ERROR_CODE;

    KKSList<KKSFile*> files = eio->files();
    for(int i=0; i<files.count(); i++){
        KKSFile * f = files.at(i);
        int idUrl = f->id();
        int ok;
        if(idUrl <= 0){
            ok = insertFile(eio->id(), f, parent);
        }
        else{
            ok = updateFileInfo(f);
        }
        if(ok != OK_CODE)
            return ERROR_CODE;
    }

    QString ids;
    if(files.count() > 0)
        ids = QString::number(files.at(0)->id());

    for(int i=1; i<files.count(); i++){
        const KKSFile * f = files.at(i); 
        if(f)
            ids += ", " + QString::number(f->id());
    }
    
    //удаляем все оставшиеся связки с файлами и при этом удаляем при необходимости сами файлы
    QString sql;
    if(ids.isEmpty())
        sql = QString("select rRemoveRecUrl(%1, TRUE, ARRAY[-1])").arg(idRecord);
    else
        sql = QString("select rRemoveRecUrl(%1, TRUE, ARRAY[%2])").arg(idRecord).arg(ids);
    
	KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return ERROR_CODE;
    }
    int ok = res->getCellAsInt(0, 0);
    delete res;
    if(ok != 1){
        return ERROR_CODE;
    }

    return OK_CODE;
}

int KKSEIOFactory::updateFileInfo(KKSFile * f) const
{
    if(!f || f->id() <= 0)
        return ERROR_CODE;

    QString sql = QString("select rUpdateRecUrl(%1, '%2', %3, '%4')")
                     .arg(f->id())
                     .arg(f->name())
                     .arg(f->type().id())
                     .arg(f->srcExt());

    KKSResult * res = db->execute(sql);
    int ok = ERROR_CODE;
    if(res)
        ok = res->getCellAsInt(0, 0);

    delete res;
    if(ok <= 0)
        return ERROR_CODE;

    return OK_CODE;
}

int KKSEIOFactory::uploadFile(KKSFile * f, QWidget * parent) const
{
    if(!f)
        return ERROR_CODE;

    if(!fileLoader)
        return ERROR_CODE;

    if(f->localUrl().isEmpty()){
        qWarning() << "upload file. URL is empty!";
        return ERROR_CODE;
    }

    if(f->id() <= 0){
        QString sql = QString("select rInsertUrl('%1', 'not assigned', %2, '%3')")
                                     .arg(f->name())
                                     .arg(f->type().id())
                                     .arg(f->srcExt());
        qDebug () << __PRETTY_FUNCTION__ << sql;
        KKSResult * res = db->execute(sql);
        if(!res)
            return ERROR_CODE;
        if(res->getRowCount() == 0){
            delete res;
            return ERROR_CODE;
        }

        int idUrl = res->getCellAsInt(0, 0);
        
        delete res;
        
        if(idUrl <= 0)
            return ERROR_CODE;
        
        f->setId(idUrl);
    }

    int ok = fileLoader->rWriteFile(f->id(), f->localUrl(), true, -1, parent);
    if(ok != OK_CODE){
        f->setId(-1);
        return ERROR_CODE;
    }

    f->setUploaded(true);
    f->setLoaded(true);

    return OK_CODE;
}
