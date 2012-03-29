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

////////////////////////////////////////////////////////////////////////
// Name:       KKSEIOFactory::KKSEIOFactory()
// Purpose:    Implementation of KKSEIOFactory::KKSEIOFactory()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSEIOFactory::KKSEIOFactory()
{
    db = NULL;
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

KKSDatabase * KKSEIOFactory::getDb() const
{
    return db;
}


int KKSEIOFactory::insertEIO(KKSObjectExemplar* eio, const KKSCategory* cat, const QString& table) const
{
    if(!eio || !eio->io())
        return ERROR_CODE;

    return insertRecord(eio, cat, table);
}

int KKSEIOFactory::updateEIO(const KKSObjectExemplar* eio, const KKSCategory* cat, const QString& table) const
{
    if(!eio || !eio->io())
        return ERROR_CODE;

    return updateRecord(eio, cat, table);
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
                                const QString & table) const
{
    if(!db || !eio)
        return ERROR_CODE;

    KKSObject * io = eio->io();
    if(!io)
        return ERROR_CODE;

    qDebug () << __PRETTY_FUNCTION__ << table << io->tableName();
    //берем название таблицы, в которую необходимо будет сделать инсерт
    QString tableName = table.isEmpty() ? io->tableName() : table;

    if (cat)
    {
        for (KKSMap<int, KKSCategoryAttr*>::const_iterator pa = cat->attributes().constBegin(); \
             pa != cat->attributes().constEnd(); \
             pa++)
            qDebug () << __PRETTY_FUNCTION__ << pa.key() << pa.value()->code (false) << pa.value()->code (true);
    }
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

    //данный метод генерит реальный SQL-запрос в БД на инсерт в таблицу
    int id = generateInsertQuery(tableName, attrs, attrValues, query, exQuery);
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


    return OK_CODE;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSRecordFactory::updateRecord(const KKSObjectExemplar* eio)
// Purpose:    Implementation of KKSRecordFactory::updateRecord()
// Parameters:
// - eio
// Return:     int
////////////////////////////////////////////////////////////////////////

int KKSEIOFactory::updateRecord(const KKSObjectExemplar* eio, 
                                const KKSCategory* cat, 
                                const QString & table) const
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
    //данный метод генерит реальный SQL-запрос в БД на апдейт в таблицу
    int id = generateUpdateQuery(tableName, attrs, attrValues, eio->id(), query, exQuery);
    if(id == ERROR_CODE || (query.isEmpty() && exQuery.isEmpty()))
        return ERROR_CODE;

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

int KKSEIOFactory::deleteRecord(int id, const QString & table) const
{
    if(!db)
        return ERROR_CODE;

    QString sql = QString("delete from %1 where id = %2").arg(table).arg(id);

    KKSResult * res = db->execute(sql);
    if(!res || (res->resultStatus() != KKSResult::CommandOk && res->resultStatus() != KKSResult::TuplesOk)){
        if(res){
            qWarning() << res->errorMessage();
            delete res;
        }
        return ERROR_CODE;
    }
    
    delete res;

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
    int id = eio->id();

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
int KKSEIOFactory::generateInsertQuery(const QString & tableName, 
                                       const KKSMap<int, KKSCategoryAttr *> & attrs, 
                                       const KKSList<KKSAttrValue *> & attrValues, 
                                       QString & query,
                                       QString & exQuery /*для обработки атрибутов типа atCheckListEx*/) const
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
    if((count = attrs.count()) != attrValues.count())
        return ERROR_CODE;

    int idValue = 0;
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
               //rTable == IO_CATEGORIES_ORGANIZATION ||
               rTable == USER_CHAINS_ORGANIZATION ||
               rTable == REPORT_ORGANIZATION ||
               rTable == GUARD_OBJ_DEVICES_TSO ||
               rTable == ACCESS_CARDS_ACCESS_PLAN_TSO ||
			   rTable == MAIL_LISTS_POSITION  ||
               rTable == SHU_DLS_POSITION
               )
            {
                refTable = rTable;
            }
            else
                refTable = rTable + "_ref_" + QString::number(attr->id());

            QString ids = attrValue->value().valueForInsert();
            QString mainAttr = QString("id_%1").arg(tableName);
            QString childAttr = QString("id_%1").arg(attr->tableName());
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
        
        bool useID = false;
        QString refColumn = attr->refColumnName();
        if(refColumn.isEmpty() || refColumn == "id"){
            useID = true;//вариант 1, иначе - вариант 2
        }
        
        if(useID == true){
            
            
            bool ok (true);
            int nv = value.value().toInt (&ok);
            Q_UNUSED (nv);

            if ((iType == KKSAttrType::atList || iType == KKSAttrType::atParent) && 
                !ok && 
                value.value() != QString("NULL") &&
                !value.value().isEmpty ()
            )
            {
                QString aQuery = QString ("SELECT id from %1 where %2='%3' limit 1;")
                                                 .arg ((iType == KKSAttrType::atParent ? tableName : attr->tableName ()))
                                                 .arg (attr->columnName ())
                                                 .arg (value.value());
                KKSResult * aRes = db->execute (aQuery);

                if (!aRes || aRes->getRowCount () == 0)
                {
                    QString atName = (iType == KKSAttrType::atParent ? tableName : attr->tableName ());
                    qDebug () << __PRETTY_FUNCTION__ << atName << value.value ();
                    int idV = getNextSeq (atName, "id");
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
            //-----
            else
            {
                //если значение атрибута отсутствует, то используем значение по умолчанию (при его наличии), 
                //но только в случае, когда у атрибута параметр isMandatory = true.
                if(value.isNull()){
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
        }
        else{
            if(value.isNull()){
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

    if(attrArray.trimmed().isEmpty())
        query = QString("INSERT INTO %1 (id) VALUES (%2);").arg(tableName).arg(idValue);
    else
        query = QString("INSERT INTO %1 (id, %2) VALUES (%3, %4);").arg(tableName).arg(attrArray).arg(idValue).arg(valueArray);

    return idValue;
}

//генерится SQL-запрос следующего вида:
//UPDATE <tableName> SET (<attribute1=value1, ...>) where id = <id>;
//метод возвращает OK_CODE при успехе
int KKSEIOFactory::generateUpdateQuery(const QString & tableName, 
                                       const KKSMap<int, KKSCategoryAttr *> & attrs, 
                                       const KKSList<KKSAttrValue *> & attrValues, 
                                       int idEIO,
                                       QString & query,
                                       QString & exQuery) const
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
            exQuery += QString("select aInsertExValues('%1', %2, %3, '%4', '%5');")
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

    if(!attrArray.trimmed().isEmpty())
        query = QString("UPDATE %1 SET %2 WHERE id = %3;").arg(tableName).arg(attrArray).arg(idEIO);

    return OK_CODE;
}

int KKSEIOFactory::getNextSeq(QString tableName, QString idColumn) const
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

    int id = res->getCellAsInt(0, 0);
    
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

int KKSEIOFactory::insertEIOList(KKSList<KKSObjectExemplar*> eioList, const KKSCategory* cat, const QString & table, QProgressDialog *pgDial) const
{
    if(table.isEmpty())
        return ERROR_CODE;

    int count = eioList.count();
    if(count == 0)
        return OK_CODE;
    
    db->begin();

    if (pgDial)
    {
        pgDial->setMinimum (0);
        pgDial->setMaximum (count);
    }

    for(int i=0; i<count; i++)
    {
        if (pgDial)
            pgDial->setValue (i);
        KKSObjectExemplar * eio = eioList.at(i);
//        if(!eio->io() || eio->io()->tableName() != table){
//            db->rollback();
//            return ERROR_CODE;
//        }

        int res = insertEIO(eio, cat, table);
        if(res != OK_CODE){
            db->rollback();
            return ERROR_CODE;
        }
    }
    
    db->commit();
    
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

    QString sql = QString("select msgInsertOut(%1, %2, %3, %4)")
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

