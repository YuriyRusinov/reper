/***********************************************************************
 * Module:  KKSRecordFactory.cpp
 * Author:  sergey
 * Modified: 2 декабря 2008 г. 13:22:57
 * Purpose: Implementation of the class KKSRecordFactory
 ***********************************************************************/
#include <QRegExp>
#include <QtDebug>
#include <QHash>
#include <QByteArray>
#include <QIcon>
#include <QPixmap>
#include <QBuffer>

#include "KKSPPFactory.h"
#include "KKSEIOFactory.h"
#include "KKSLoader.h"
#include "KKSFileLoader.h"
#include "KKSConverter.h"
#include "KKSFilter.h"

#include "KKSObjectExemplar.h"
#include "defines.h"
#include "kksdatabase.h"
#include "KKSObject.h"
#include "KKSCategory.h"
#include "KKSCategoryAttr.h"
#include "KKSValue.h"
#include "KKSType.h"
#include "KKSAttrValue.h"
#include "KKSAttrType.h"
#include "KKSLifeCycle.h"
#include "KKSState.h"
#include "KKSTemplate.h"
#include "KKSAttrView.h"
#include "KKSAttrGroup.h"
#include "KKSRubric.h"
#include "KKSSearchTemplate.h"
#include "KKSSearchTemplateType.h"
#include "KKSAclTemplate.h"
#include "KKSAccessEntity.h"
#include "kkssito.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSPPFactory::KKSPPFactory()
// Purpose:    Implementation of KKSPPFactory::KKSPPFactory()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSPPFactory::KKSPPFactory()
{
    db = NULL;
    loader = NULL;
    eiof = NULL;
    m_inTransaction.push(false);
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSPPFactory::~KKSPPFactory()
// Purpose:    Implementation of KKSPPFactory::~KKSPPFactory()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSPPFactory::~KKSPPFactory()
{
}



/************************************************************************
*************************************************************************
*************************************************************************
******************СОХРАНЕНИЕ, УДАЛЕНИЕ, ИЗМЕНЕНИЕ ЕО*********************
*************************************************************************
*************************************************************************
*************************************************************************/


void KKSPPFactory::setParams(KKSLoader * _loader, 
                             KKSFileLoader * _fileLoader, 
                             KKSEIOFactory * _eiof, 
                             KKSDatabase * _db)
{
    db = _db;
    loader = _loader;
    fileLoader = _fileLoader;
    eiof = _eiof;
}

KKSDatabase * KKSPPFactory::getDb() const
{
    return db;
}


/************************************************************************
*************************************************************************
*************************************************************************
******************СОХРАНЕНИЕ, УДАЛЕНИЕ, ИЗМЕНЕНИЕ ИО*********************
*************************************************************************
*************************************************************************
*************************************************************************/

/*
TODO: ВАЖНО!!!!
При работе с ИО и ЕИО необходимо учесть будущую подсистему разграничения доступа!
в частности, при создании ИО задавать на него дискреционные права
при удалении ЕИО - удалять информацию о них из таблицы access_table
В настоящее время это не делается

также надо удалять записи из журнала работы пользователей с ИО (таблица log)
*/


/*
при создании контейнерного объекта создается таблица, которая будет содержать его экземпляры.
Если таблица с заданным в KKSObject::tableName() названием существует, 
то можно воспользолваться возможностью создать таблицу с автоматическим названием.
В этом случае надо параметр useDefault установить в TRUE.
Когда это так, то в случае, когда таблица с исходным названием существует, 
то будет использовано автоназвание вида:
eio_table_<ID_IO_OBJECT>

У каждого ИО имеется уникальный буквенно-цифровой код. 
В БД не допускается иметь более одного ИО с одним кодом.

Для прозрачной поддержки этого существует возможность 
использовать автоматическое создание кода вида
IO_OBJ_<ID_IO_OBJECT>, где ID_IO_OBJECT - присвоенный идентификатор ИО
*/
int KKSPPFactory::insertIO(KKSObject * io, bool useDefaultTable, QWidget * parent) 
{
    Q_UNUSED(useDefaultTable);

    if(!db || !io || !io->category())
        return ERROR_CODE;
    
    if(!inTransaction())
        db->begin();
    
    //проверяем, сохранена ли категория ИО
    //если не сохранена - сохраняем
    if(io->category()->id() <= 0){
        setInTransaction();
        int ok = insertCategory(io->category());
        restoreInTransaction();
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    }

    //если тип ИО не задан, используем по умолчанию первый в БД
    if(!io->type()){
        io->setType(KKSType::defType1());
    }


    //если у ИО не сохранен тип - сохраним его
    if(io->type()->id() <= 0){
        setInTransaction();
        int ok = insertIOType(const_cast<KKSType*>(io->type()));
        restoreInTransaction();
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    }

    //разрешаем задавать свое название таблицы для справочника.
    //однако, поскольку таблица с таким названием в БД может присутствовать
    //то в процессе сохранения записи в io_objects 
    //название таблицы может поменяться на уникальное
    //поэтому позже загрузим из БД реально присвоенное
    QString ioTableName = io->tableName();
    if(!ioTableName.trimmed().isEmpty()){
        ;
    }
    else{
        io->setTableName(QString::null);
    }


    //создаем запись в таблице io_objects
    io->setId(-1);
    KKSObjectExemplar * eio = KKSConverter::objToExemplar(loader, io);
    //KKSAttrValue * avRec = eio->attrValue (ATTR_RECORD_FILL_COLOR);
    //KKSAttrValue * avRecT = eio->attrValue (ATTR_RECORD_TEXT_COLOR);
    
    int ok = eiof->insertEIO(eio);
    if(ok == ERROR_CODE){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }
    io->setId(eio->id());
    eio->release();
    
    
    QString s = QString("select table_name from io_objects where id = %1").arg(io->id());
    KKSResult * r = db->execute(s);
    if(!r || r->getRowCount() == 0){
        if(r)
            delete r;
        return ERROR_CODE;
    }
    QString tName = r->getCellAsString(0, 0);
    io->setTableName(tName);
    delete r;
        
    //делаем операции инсерта в таблицу attrs_values временно нежурналируемыми
    ok = setAsLogging(3, false);
    if(ok != OK_CODE){
        if(!inTransaction())
            db->rollback();
        io->setId(-1);
        return ERROR_CODE;
    }

    //добавляем значения атрибутов
    setInTransaction();
    ok = insertAttrValues(io);
    restoreInTransaction();
    if(ok != OK_CODE){
        if(!inTransaction())
            db->rollback();
        io->setId(-1);
        return ERROR_CODE;
    }

    //делаем операции инсерта в таблицу attrs_values снова журналируемыми
    ok = setAsLogging(3, true);
    if(ok != OK_CODE){
        if(!inTransaction())
            db->rollback();
        io->setId(-1);
        return ERROR_CODE;
    }

    //добавляем прикрепленные файлы
    setInTransaction();
    ok = insertFiles(io, parent);
    restoreInTransaction();
    if(ok != OK_CODE){
        if(!inTransaction())
            db->rollback();
        io->setId(-1);
        return ERROR_CODE;
    }

    //обновляем дискреционные права доступа
    //поскольку идет операция создания ИО, то текущий 
    //пользователь гарантированно является автором и имеет на это право.
    ok = updateUserPrivileges(io);
    if(ok != OK_CODE){
        if(!inTransaction())
            db->rollback();
        io->setId(-1);
        return ERROR_CODE;
    }
    
    //обновляем информацию о шаблонах, которые используются 
    //при визуализации ИО для ТЕКУЩЕГО пользователя
    //шаблоны, если они заданы, должны существовать в БД!
    ok = updateUserTemplates(io);
    if(ok != OK_CODE){
        if(!inTransaction())
            db->rollback();
        io->setId(-1);
        return ERROR_CODE;
    }

    //обновляем информацию о вложениях данного ИО
    ok = insertIncludes(io);
    if(ok != OK_CODE){
        if(!inTransaction())
            db->rollback();
        io->setId(-1);
        return ERROR_CODE;
    }

    //делаем операции апдейта в таблицу io_objects временно нежурналируемыми
    ok = setAsLogging(2, false);
    if(ok != OK_CODE){
        if(!inTransaction())
            db->rollback();
        io->setId(-1);
        return ERROR_CODE;
    }
    
    //помечаем информационный объект как завершенный  (при операции ИНСЕРТ)
    //(он создан и ему заданы значения всех атрибутов)
    QString sql = QString("select ioSetCompleted(%1, 1)").arg(io->id());
    KKSResult * res = db->execute(sql);
    if(!res || res->getCellAsInt(0, 0) != OK_CODE){
        if(!inTransaction())
            db->rollback();
        io->setId(-1);
        return ERROR_CODE;
    }

    delete res;

    //информация о синхронизируемых органихациях всегда добавляется самая последняя, 
    //чтобы не было дублирования записей в таблице out_sync_queue
    QString ids = io->replicatedOrgsV().valueForInsert();
    sql = QString("select ioInsertOrgs(%1, %2);")
                                .arg(io->id())
                                .arg(ids);
    res = db->execute(sql);
    if(!res || res->getCellAsInt(0, 0) != OK_CODE){
        if(!inTransaction())
            db->rollback();
        io->setId(-1);
        if(res) 
            delete res;
        return ERROR_CODE;
    }
    delete res;

    //делаем операции апдейта в таблицу io_objects снова журналируемыми
    ok = setAsLogging(2, true);
    if(ok != OK_CODE){
        if(!inTransaction())
            db->rollback();
        io->setId(-1);
        if(res) 
            delete res;

        return ERROR_CODE;
    }

    if(!inTransaction())
        db->commit();

    return OK_CODE;
}

/*
Категорию ИО менять запрещается!
*/
int KKSPPFactory::updateIO(KKSObject * io, QWidget * parent)
{
    if(!db || !io)
        return ERROR_CODE;

    int idUser = loader->getUserId();
    
    if(idUser != io->author() && loader->getPrivilege(idUser, io->id(), 4, true) == false){
        return PRIVILEGE_ERROR;
    }


    //обновляем дискреционные права доступа
    //но только в том случае, если текущий пользователь является автором ИО или администратором
    if(io->author() == idUser || idUser == 1){ //admin
        int ok = updateUserPrivileges(io);
        if(ok != OK_CODE){
            //if(!inTransaction())
            //    db->rollback();
            io->setId(-1);
            //rollbackRubrics(io->rootRubric(), true);
            return ERROR_CODE;
        }
    }

	//если ИО создан на внешнем объекте, то не позволяем даже администратору изменять его характеристики.
	//менять ему можно лишь права доступа, собственно поэтому этот блок вынесен выше.
	if(io->author() <= 0){
		qWarning() << "KKSPPFactory::updateIO(): ONLY OWNER ORGANIZATION CAN MODIFY THE OBJECT!";

		return PRIVILEGE_ERROR1;
	}

    if(!inTransaction())
        db->begin();
    
    //мы должны проверить, изменялись ли какие-либо критичные параметры ИО
    // - название подчиненной таблицы - в этом случае надо изменить название подчиненной таблицы
    // - категория - в этом случае надо завершиться с ошибкой
    KKSObject * tmp = loader->loadIO(io->id(), true);
    if(!tmp){
        qWarning() << "Object does not saved in database, so it cannot be updated, must be inserted";
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }
    
    if(tmp->category()->id() != io->category()->id()){
        qWarning() << "Changing category of existing in database objects does not supported!";
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }
    
    KKSCategory * tc1 = tmp->category()->tableCategory();
    KKSCategory * tc2 = io->category()->tableCategory();
    if( (tc1 && tc2 && tc1->id() == tc2->id()) || (!tc1 && !tc2) ) {
        ;//ok!
    }
    else{
        qWarning() << "Changing category of existing in database objects is unsupported!";
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    //нельзя изменить "тип" объекта (контейнерный-неконтейнерный и обратно)
    if((tmp->tableName().isEmpty() && !io->tableName().isEmpty()) ||
        (!tmp->tableName().isEmpty() && io->tableName().isEmpty()))
    {
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    //если тип ИО не задан, используем по умолчанию первый в БД
    if(!io->type()){
        io->setType(KKSType::defType1());
    }


    //если у ИО не сохранен тип - сохраним его
    if(io->type()->id() <= 0){
        setInTransaction();
        int ok = insertIOType(const_cast<KKSType*>(io->type()));
        restoreInTransaction();
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    }


    //помечаем информационный объект как незавершенный 
    //(он находится в процессе обновления, и этот процесс не закончен)
    QString sql = QString("select ioSetUpdating(%1)").arg(io->id());
    KKSResult * res = db->execute(sql);
    if(!res || res->getCellAsInt(0, 0) != OK_CODE){
        if(!inTransaction())
            db->rollback();
        //io->setId(-1);
        return ERROR_CODE;
    }
    delete res;

    //переименовываем таблицу ---делается в триггере
    /*
    if(tmp->tableName() != io->tableName()){
        QString sql = QString("alter table %1 rename to %1").arg(tmp->tableName()).arg(io->tableName());
        int ok = db->executeCmd(sql);
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    }
    */

    tmp->release();
    
    //изменяем запись в таблице io_objects
    KKSObjectExemplar * eio = KKSConverter::objToExemplar(loader, io);
    int ok = eiof->updateEIO(eio);
    if(ok == ERROR_CODE){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    if(io->m_attrsModified){
        
        //записываем в журнал информацию об изменении атрибутивного состава ИО
        //ok = addLogAttrs(io->id(), 5);
        //if(ok != OK_CODE){
        //    if(!inTransaction())
        //        db->rollback();
        //    io->setId(-1);
        //    return ERROR_CODE;
        //}

        //делаем операции записи и удаления в таблицу attrs_values временно нежурналируемыми
        ok = setAsLogging(3, false);
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            io->setId(-1);
            return ERROR_CODE;
        }
        ok = setAsLogging(4, false);
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            io->setId(-1);
            return ERROR_CODE;
        }
        
        //изменяем значения атрибутов
        //изменение производится путем вызова команды UPDATE для каждого из значений атрибутов 
        setInTransaction();
        ok = updateAttrValues(io);
        restoreInTransaction();
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
/*
        setInTransaction();
        ok = insertAttrValues(io);
        restoreInTransaction();
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
*/
//делаем операции записи и удаления в таблицу attrs_values снова журналируемыми
        ok = setAsLogging(3, true);
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            io->setId(-1);
            return ERROR_CODE;
        }
        ok = setAsLogging(4, true);
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            io->setId(-1);
            return ERROR_CODE;
        }

    }

    if(io->m_filesModified){
        //изменяем прикрепленные файлы
        ok = updateFiles(io, parent);
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    }

    if(io->m_userTemplatesModified){
        //обновляем информацию о шаблонах, которые используются 
        //при визуализации ИО для ТЕКУЩЕГО пользователя
        //шаблоны, если они заданы, должны существовать в БД!
        ok = updateUserTemplates(io);
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();

            return ERROR_CODE;
        }
    }

    if(io->m_includesModified){
        //обновляем информацию о вложениях данного ИО
        ok = updateIncludes(io);
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();

            return ERROR_CODE;
        }
    }

/*
//обновляем дискреционные права доступа
    //но только в том случае, если текущий пользователь является автором ИО
    if(io->author() == loader->getUserId()){
        ok = updateUserPrivileges(io);
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            io->setId(-1);
            rollbackRubrics(io->rootRubric(), true);
            return ERROR_CODE;
        }
    }
*/
    //помечаем информационный объект как завершенный (при операции АПДЕЙТ)
    //(он создан и ему заданы значения всех атрибутов)
    sql = QString("select ioSetCompleted(%1, 2)").arg(io->id());
    res = db->execute(sql);
    if(!res || res->getCellAsInt(0, 0) != OK_CODE){
        if(!inTransaction())
            db->rollback();
        //io->setId(-1);
        if(res) 
            delete res;
        rollbackRubrics(io->rootRubric(), true);
        return ERROR_CODE;
    }

    delete res;

    //информация о синхронизируемых организациях всегда добавляется самая последняя, 
    //чтобы не было дублирования записей в таблице out_sync_queue
    QString ids = io->replicatedOrgsV().valueForInsert();
    qDebug () << __PRETTY_FUNCTION__ << ids;
    sql = QString("select ioInsertOrgs(%1, %2);")
                                .arg(io->id())
                                .arg(ids);
    res = db->execute(sql);
    if(!res || res->getCellAsInt(0, 0) != OK_CODE){
        if(!inTransaction())
            db->rollback();
        io->setId(-1);
        if(res) 
            delete res;

        rollbackRubrics(io->rootRubric(), true);
        return ERROR_CODE;
    }

    delete res;

    if(!inTransaction())
        db->commit();


    commitRubrics(io->rootRubric());//помечаем рубрики ИО, как созданные а в БД

    KKSObject * io1 = loader->loadedObjects.value(io->id(), NULL);
    if(!io1)
        return OK_CODE;
    
    loader->loadedObjects.remove(io1->id());
    io1->release();
    io->addRef();
    loader->loadedObjects.insert(io->id(), io);
    
   // io->addRef();
    //    return io;
   // }

    return OK_CODE;
}

int KKSPPFactory::deleteIO(KKSObject * io) const
{
    if(!db || !io)
        return ERROR_CODE;

    if(io->id() <= 0)
        return ERROR_CODE;

    //нельзя удалять системные ИО
    if(io->isSystem())
        return ERROR_CODE;

    int idUser = loader->getUserId();
    
    if(idUser != io->author() && loader->getPrivilege(idUser, io->id(), 3, true) == false){
        return PRIVILEGE_ERROR;
    }
    

    //все выполняем в одной транзакции
    if(!inTransaction())
        db->begin();

    QString sql = QString("select ioDelete(%1)").arg(io->id());
    KKSResult * res = db->execute(sql);
    if(!res || res->getCellAsInt(0, 0) != 1){
        if(res)
            delete res;

        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    if(!inTransaction())
        db->commit();

    //объект удалили. Теперь необходимо ему установить ИД в -1 
    //(т.е. этим мы говорим, что он в БД не сохранен)
    io->setId(-1);

    return OK_CODE;
}

int KKSPPFactory::insertFile(int idObject, KKSFile * f, QWidget * parent) const
{
    if(!f)
        return ERROR_CODE;

    int ok = uploadFile(f, parent);
    if(ok != OK_CODE)
        return ERROR_CODE;

    QString sql = QString("select ioInsertUrl(%1, %2, '%3')")
                          .arg(idObject)
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

int KKSPPFactory::insertFiles(const KKSObject * io, QWidget * parent) const
{
    if(!io)
        return ERROR_CODE;

    KKSList<KKSFile * > files = io->files();
    int ok = OK_CODE;

    for (int i=0; i<files.count(); i++)
    {
        KKSFile * f = files[i];
        if(!f)
            continue;

        ok = insertFile(io->id(), f, parent);

        if(ok != OK_CODE){
            return ERROR_CODE;
        }
    }

    return OK_CODE;
}

/*

int KKSPPFactory::deleteFiles(const KKSObject * io, bool bRemoveFiles) const
{
    if(!io)
        return ERROR_CODE;

    int idObject = io->id();
    if(idObject <= 0)
        return ERROR_CODE;

    KKSList<KKSFile*> files = io->files();
    for(int i=0; i<files.count(); i++){
        KKSFile * f = files.at(i);
        int idUrl = f->id();
        if(idUrl <= 0){
            qWarning() << "deleteFiles: idUrl <= 0!";
            continue;
        }

        QString sql = QString("select rRemoveObjUrl(%1, %2, %3)")
                            .arg(idObject)
                            .arg(idUrl)
                            .arg(bRemoveFiles? "TRUE" : "FALSE");
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
        f->setId(-1);
        //f->setUploaded(false);
    }
    
    //т.е. если идет апдейт ИО (данный параметр выставляется в FALSE только в методе updateIO() )
    if(!bRemoveFiles){
        //удаляем все оставшиеся связки с файлами и при этом удаляем при необходимости сами файлы
        QString sql = QString("select rRemoveObjUrl(%1, TRUE)").arg(idObject);
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
    }

    return OK_CODE;
}
*/

int KKSPPFactory::updateFiles(const KKSObject * io, QWidget * parent) const
{
    if(!io)
        return ERROR_CODE;

    int idObject = io->id();
    if(idObject <= 0)
        return ERROR_CODE;

    KKSList<KKSFile*> files = io->files();
    for(int i=0; i<files.count(); i++){
        KKSFile * f = files.at(i);
        int idUrl = f->id();
        int ok;
        if(idUrl <= 0){
            ok = insertFile(io->id(), f, parent);
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
        sql = QString("select rRemoveObjUrl(%1, TRUE, ARRAY[-1])").arg(idObject);
    else
        sql = QString("select rRemoveObjUrl(%1, TRUE, ARRAY[%2])").arg(idObject).arg(ids);
    
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

int KKSPPFactory::updateFileInfo(KKSFile * f) const
{
    if(!f || f->id() <= 0)
        return ERROR_CODE;

    QString sql = QString("select rUpdateUrl(%1, '%2', %3, '%4')")
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

int KKSPPFactory::uploadFile(KKSFile * f, QWidget * parent) const
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

int KKSPPFactory::deleteAttrValues(const KKSObject * io) const
{
    if(!io)
        return ERROR_CODE;

    QString sql = QString("delete from attrs_values where id_io_object = %1").arg(io->id());
    int ok = db->executeCmd(sql);
    if(ok != OK_CODE){
        return ERROR_CODE;
    }

    return OK_CODE;
}

int KKSPPFactory::insertAttrValues(const KKSObject * io) const
{
    if(!io)
        return ERROR_CODE;

    KKSList<KKSAttrValue * > attrValues = io->attrValues();

    QString sql;
    QString sqlEx;
    for (int i=0; i<attrValues.count(); i++)
    {
        KKSAttrValue * av = attrValues[i];
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
                                .arg(io->id())
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

        sql += QString("select ioInsertAttr(%1, %2, %3::varchar, %4, %5, %6, %7, %8);")
                              .arg(io->id())
                              .arg(a->id())
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

int KKSPPFactory::updateAttrValues(const KKSObject * io) const
{
    if(!io)
        return ERROR_CODE;

    KKSList<KKSAttrValue * > attrValues = io->attrValues();

    QString sql;
    QString sqlEx;
    for (int i=0; i<attrValues.count(); i++)
    {
        KKSAttrValue * av = attrValues[i];
        if(!av){
            qWarning() << "WARNING!! Some of attributes of IO is NULL pointer! Check data integrity!";
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
                                .arg(io->id())
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

        
        QString s = QString("select ioUpdateAttr(%1, %2, %3::varchar, NULL, NULL, %4, %5, %6);")
                              .arg(io->id())
                              .arg(a->id())
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

int KKSPPFactory::insertGroup(int idTemplate, KKSAttrGroup * g) const
{
    if(!db || !g || idTemplate <= 0)
        return ERROR_CODE;

    if(!inTransaction())
        db->begin();

    if(g->parent() && g->parent()->id() <= 0){
        int ok = insertGroup(idTemplate, (KKSAttrGroup*)(g->parent()));
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    }
    
    int idGroup = eiof->getNextSeq("a_groups", "id");
    if(idGroup <= 0){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }
    
    QString parent_id = (g->parent() ? QString::number(g->parent()->id()) : QString("NULL"));
    QString sql = QString("insert into a_groups (id, id_io_template, id_parent, name, \"order\") "
                          "values(%1, %2, %3, '%4', %5)")
                                .arg(idGroup)
                                .arg(idTemplate)
                                .arg(parent_id)
                                .arg(g->name())
                                .arg(g->order());
    
    int ok = db->executeCmd(sql);
    if(ok != OK_CODE){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    if(!inTransaction())
        db->commit();

    g->setId(idGroup);

    return OK_CODE;
}

int KKSPPFactory::insertAttrView(int idCategory, int idTemplate, int idGroup, KKSAttrView * av) const
{
    if(!db || !av || idTemplate <= 0 || idCategory <= 0 || idGroup <= 0)
        return ERROR_CODE;

    if(!inTransaction())
        db->begin();

    int idCategoryAttr = av->idCategoryAttr();
    int idAttr = av->id();

    //атрибут не сохранен в БД
    if(idAttr <= 0 || idCategoryAttr <= 0){
        int ok = insertCategoryAttr(idCategory, av);
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
        idAttr = av->id();
        idCategoryAttr = av->idCategoryAttr();
    }


    QString defVal = av->defValue().valueForInsert();
    bool isReadOnly = av->isReadOnly();
    int order = av->order();

    QString sql = QString("select ivInsert(%1, %2, %3, %4, %5, %6)")
                            .arg(idCategoryAttr)
                            .arg(idTemplate)
                            .arg(idGroup)
                            .arg(isReadOnly ? "true" : "false")
                            .arg(order)
                            .arg(defVal.isEmpty() || av->defValue().isNull() ? QString("NULL") : QString ("asString(%1, false)").arg (defVal));
    //qDebug () << __PRETTY_FUNCTION__ << sql;

    
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() != 1)
    {
        if (res)
            delete res;

        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    int idViewAttr = res->getCellAsInt(0, 0);
    delete res;

    if(idViewAttr <= 0){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    av->setIdViewAttr(idViewAttr);

    if(!inTransaction())
        db->commit();

    return OK_CODE;
}

int KKSPPFactory::insertAttrGroup(int idCategory, int idTemplate, KKSAttrGroup * g) const
{
    if(!db || !g || idTemplate <= 0)
        return ERROR_CODE;

    if(!inTransaction())
        db->begin();

    //если группа не сохранена - сохраняем ее
    //важно помнить, что тройка (id_template, id_parent, order) является UNIQUE индексом!
    if(g->id() <= 0){
        int ok = insertGroup(idTemplate, g);
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    }

    //
    //добавим атрибуты в шаблон
    //
    KKSMap<int, KKSAttrView *>::const_iterator pa;
    for (pa=g->attrViews().constBegin(); pa != g->attrViews().constEnd(); pa++)
    {
        KKSAttrView * av = pa.value();
        int ok = insertAttrView(idCategory, idTemplate, g->id(), av);
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    }
    //
    // добавим подгруппы в группу
    //
    QList<KKSAttrGroup*> sGroups = g->childGroups().values();
    qSort (sGroups.begin(), sGroups.end(), compareAttrGroups);
    KKSMap<int, KKSAttrGroup*> cGroups;
    for (int i=0; i<sGroups.count(); i++)
    {
        KKSAttrGroup * gs = sGroups[i];
        gs->setId (-1);
        int ok = insertAttrGroup (idCategory, idTemplate, gs);
        if (ok != OK_CODE)
        {
            if (!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
        cGroups.insert (gs->id(), gs);
    }
    g->setChildGroups (cGroups);
    
    if(!inTransaction())
        db->commit();

    return OK_CODE;
}

int KKSPPFactory::insertTemplate(KKSTemplate * t) const
{
    if(!db || !t || !t->category())
        return ERROR_CODE;

    if(!inTransaction())
        db->begin();

    //если категория шаблона не сохранена, то сохраним сначала ее
    if(t->category()->id() <= 0){
        setInTransaction();
        int ok = insertCategory(t->category());
        restoreInTransaction();
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    }

    //сначала сохраним в БД саму запись о шаблоне в таблицу шаблонов
    //создаем запись в таблице io_templates
    t->setId(-1);
    KKSObjectExemplar * eio = KKSConverter::templateToExemplar(loader, t);
    int ok = eiof->insertEIO(eio);
    if(ok == ERROR_CODE){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }
    t->setId(eio->id());
    eio->release();

    //теперь сохраним группы и атрибуты шаблона
    //при этом надо учитывать, что группа  является атрибутом шаблона,
    //поэтому всем группам надо установить идентификатор в -1
    KKSMap<int, KKSAttrGroup *>::const_iterator pg = t->groups().constBegin();
    QList<KKSAttrGroup *> tGroups = t->groups().values();
    qSort (tGroups.begin(), tGroups.end(), compareAttrGroups);
    KKSMap<int, KKSAttrGroup *> tNGroups;
    for (int i=0; i<tGroups.count(); i++)
    {
        KKSAttrGroup * g = tGroups[i];//pg.value();//t->groups().at(i);
        g->setId(-1);
        setInTransaction();
        int ok = insertAttrGroup(t->category()->id(), t->id(), g);
        restoreInTransaction();
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            t->setId(-1);
            return ERROR_CODE;
        }
        tNGroups.insert (g->id(), g);
    }
    t->setGroups (tNGroups);

    if(!inTransaction())
        db->commit();

    return OK_CODE;
}

/*
Обновление шаблона производим по следующему алгоритму:
- изменяем название шаблона
- изменяем набор групп в шаблоне (удаляем все старые и создаем новые. При этом они поменяют идентификаторы!)
- изменяем набор атрибутов в шаблоне и их параметры (удаляем старые и создаем новые)

категорию, к которой относится шаблон, менять нельзя
*/
int KKSPPFactory::updateTemplate(const KKSTemplate * t) const
{
    if(!db || !t || t->id() <= 0 || !t->category() || t->category()->id() <= 0)
        return ERROR_CODE;

    if(!inTransaction())
        db->begin();

    //изменяем название шаблона
    QString sql = QString("update io_templates set name = '%1' where id = %2").arg(t->name()).arg(t->id());
    int ok = db->executeCmd(sql);
    if(ok != OK_CODE){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    //изменяем набор групп
    //для этого сначала удаляем старые атрибуты и сами группы
    sql = QString("delete from io_views where id_io_template = %1").arg(t->id());
    ok = db->executeCmd(sql);
    if(ok != OK_CODE){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    sql = QString("delete from a_groups where id_io_template = %1").arg(t->id());
    ok = db->executeCmd(sql);
    if(ok != OK_CODE){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    //теперь создаем группы
    
    //перед этим помечаем их как несохраненные (ставим ИД = -1)
    //и сохраняем группы и атрибуты шаблона
    //KKSMap<int, KKSAttrGroup *>::const_iterator pg = t->groups().constBegin();
    QList<KKSAttrGroup *> tGroups = t->groups().values();
    qSort (tGroups.begin(), tGroups.end(), compareAttrGroups);
    for (int i=0; i<tGroups.count(); i++)
    {
        KKSAttrGroup * g = tGroups[i];
        g->setId(-1);
        setInTransaction();
        int ok = insertAttrGroup(t->category()->id(), t->id(), g);
        restoreInTransaction();
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    }

    if(!inTransaction())
        db->commit();

    return OK_CODE;
}

int KKSPPFactory::deleteTemplate(int idTemplate) const
{
    if(!db || idTemplate <= 0)
        return ERROR_CODE;

    if(!inTransaction())
        db->begin();

    /*
    QString sql = QString("delete from io_views where id_io_template = %1").arg(idTemplate);
    int ok = db->executeCmd(sql);
    if(ok != OK_CODE){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    sql = QString("delete from a_groups where id_io_template = %1").arg(idTemplate);
    ok = db->executeCmd(sql);
    if(ok != OK_CODE){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    sql = QString("delete from io_templates where id = %1").arg(idTemplate);
    ok = db->executeCmd(sql);
    if(ok != OK_CODE){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }
    */

    QString sql = QString("select tDeleteTemplate(%1)").arg(idTemplate);
    KKSResult * res = db->execute(sql);
    if(!res || res->getCellAsInt(0, 0) != 1){
        if(res)
            delete res;
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }
    delete res;

    if(!inTransaction())
        db->commit();

    return OK_CODE;
}

int KKSPPFactory::deleteTemplate(KKSTemplate * t) const
{
    if(!db || !t || t->id() <= 0)
        return ERROR_CODE;

    if(!inTransaction())
        db->begin();

    setInTransaction();
    int ok = deleteTemplate(t->id());
    restoreInTransaction();
    if(ok != OK_CODE){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    if(!inTransaction())
        db->commit();

    t->setId(-1);

    return OK_CODE;
}


int KKSPPFactory::insertCategory(KKSCategory* c) const
{
    if(!db || !c || !c->type())
        return ERROR_CODE;

    if(!inTransaction())
        db->begin();

    //если категория имеет дочернюю категорию, то сохраним сначала ее
    //при этом важно отметить, что если дочерняя категория уже существует в БД, 
    //то никаких действий по ее сохранению предпринимать не надо.
    //Поскольку insertCategory() вызывается не для нее, а для другой категории
    //Здесь важно лишь, чтобы дочерняя существовала в БД.
    //если дочернюю категорию надо сохранить (изменить), то эта операция должна вызываться отдельно
    if(c->tableCategory()){
        if(c->tableCategory()->id() <= 0){
            setInTransaction();
            int ok = insertCategory(c->tableCategory());
            restoreInTransaction();
            if(ok != OK_CODE){
                if(!inTransaction())
                    db->rollback();
                return ERROR_CODE;
            }
        }
    }


    if(c->recAttrCategory()){
        if(c->recAttrCategory()->id() <= 0){
            setInTransaction();
            int ok = insertCategory(c->recAttrCategory());
            restoreInTransaction();
            if(ok != OK_CODE){
                if(!inTransaction())
                    db->rollback();
                return ERROR_CODE;
            }
        }
    }

    //
    //если у категории не сохранен тип - сохраним его
    //
    if(c->type()->id() <= 0){
        setInTransaction();
        int ok = insertType(c->type());
        restoreInTransaction();
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    }

    //
    // теперь сохраним жизненный цикл
    //

    if(c->lifeCycle() && c->lifeCycle()->id() <= 0){
        setInTransaction();
        int ok = insertLifeCycle(c->lifeCycle());
        restoreInTransaction();
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            c->setId(-1);
            return ERROR_CODE;
        }
    }



    
    //сначала сохраним в БД саму запись о категории в таблицу категорий
    //создаем запись в таблице io_categories
    c->setId(-1);
    
    //если категория была помечена как глобальная, то запомним это и поменяем ее на неглобальную.
    //это важно, поскольку сделать ее глобальной можно лишь после задания всех ее атрибутов 
    //и проверки, что они не ссылаются на неглобальные ИО
    bool isGlobal = false;
    if(c->isGlobal()){
        c->setAsGlobal(false);
        isGlobal = true;
    }

    KKSObjectExemplar * eio = KKSConverter::categoryToExemplar(loader, c);
    int ok = eiof->insertEIO(eio);
    if(ok == ERROR_CODE){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }
    c->setId(eio->id());
    eio->release();

    //
    //теперь сохраним атрибуты категории
    //
    KKSMap<int, KKSCategoryAttr *>::const_iterator pca;
    for (pca = c->attributes().constBegin(); pca != c->attributes().constEnd(); pca++)
    {
        KKSCategoryAttr * a = pca.value();
        setInTransaction();
        int ok = insertCategoryAttr(c->id(), a);
        restoreInTransaction();
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            c->setId(-1);
            return ERROR_CODE;
        }
    }


    if(c->rootRubric()){
        setInTransaction();
        int ok = cInsertRubrics(c);
        restoreInTransaction();
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            c->setId(-1);
            return ERROR_CODE;
        }
    }
    
    if (!c->getTemplates().isEmpty())
    {
        setInTransaction();
        int ok = this->saveCatTemplates (c);
        restoreInTransaction();
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            c->setId(-1);
            return ERROR_CODE;
        }
    }
    //помечаем категорию как завершенную
    //(она создана и ей назначены все атрибуты)
    QString sql = QString("select cSetCompleted(%1, 1)").arg(c->id());
    KKSResult * res = db->execute(sql);
    if(!res || res->getCellAsInt(0, 0) != OK_CODE){
        if(res)
            delete res;
        if(!inTransaction())
            db->rollback();
        c->setId(-1);
        return ERROR_CODE;
    }
    delete res;

    //если категория была помечена как глобальная, то самое время сделать ее таковой
    //(все атрибуты заданы и она помечена как завершенная)
    if(isGlobal){
        sql = QString("select cSetGlobal(%1)").arg(c->id());
        res = db->execute(sql);
        if(!res || res->getCellAsInt(0, 0) != OK_CODE){
            if(res)
                delete res;
            if(!inTransaction())
                db->rollback();
            c->setId(-1);
            return ERROR_CODE;
        }
        delete res;
    }

    int accOk = this->writeCatAccessRules (c->getAccessRules(), c->id());
    if (accOk < 0)
    {
        if(!inTransaction())
            db->rollback();
        c->setId(-1);
        return ERROR_CODE;
    }

    if(!inTransaction())
        db->commit();

    return OK_CODE;
}

int KKSPPFactory :: saveCatTemplates (KKSCategory * c) const
{
    if (!c)
        return ERROR_CODE;
    
    KKSMap<int, KKSTemplate *> tList = c->getTemplates();
    KKSList<KKSTemplate *> allTemplates = loader->loadCategoryTemplates (c);
    //
    // сначала удаляем из бд шаблоны, подлежащие удалению.
    //
    for (int i = 0; i < allTemplates.count(); i++)
    {
        if (allTemplates[i] && !tList.contains (allTemplates[i]->id()))
            this->deleteTemplate (allTemplates[i]->id());
    }
    for (KKSMap<int, KKSTemplate *>::const_iterator pt = tList.constBegin();
            pt != tList.constEnd();
            pt++)
    {
        KKSTemplate * t = pt.value();
        int res = pt.key() < 0 ? this->insertTemplate (t) : this->updateTemplate (t);
        if (res < 0)
        {
            if(!inTransaction())
                db->rollback();
            c->setId(-1);
            return ERROR_CODE;
        }
        if (pt.key() < 0)
        {
            c->removeTemplate (pt.key());
            c->addTemplate (t);
        }
    }
    return OK_CODE;
}

//если категория используется в ИО, то ее изменение запрещено
//соответственно, производится проверка на это.
//Если категория или ее дочерняя категория используются - изменение запрещено
int KKSPPFactory::updateCategory(const KKSCategory* c) const
{
    if(!db || !c || c->id() <= 0 ||!c->type())
        return ERROR_CODE;

    //в этом методе производится начало новой транзакции
    //или создается savepoint, если транзакция начата
    //производится удаление категории (вызов deleteCategory())
    //проверка, смогли ли удалить (если смогли, значит категория не используется)
    //и выполнение отката (транзакции или savepoint'а), т.к. реально мы ее здесь не удаляем
    
    //!!!TODO Пока данную проверку делвть не надо, поскольку мы не изменяем набор атрибутов
    //if(categoryInUse(const_cast<KKSCategory *>(c)))
    //    return ERROR_CODE;

    if(!inTransaction())
        db->begin();

    //если категория имеет дочернюю категорию, то сохраним сначала ее
    //при этом важно отметить, что если дочерняя категория уже существует в БД, 
    //то никаких действий по ее сохранению предпринимать не надо.
    //Поскольку updateCategory() вызывается не для нее, а для другой категории
    //Здесь важно лишь, чтобы дочерняя существовала в БД.
    //если дочернюю категорию надо сохранить (изменить), то эта операция должна вызываться отдельно
    if(c->tableCategory()){
        //если категория не сохранена - сохраним
        if(c->tableCategory()->id() <= 0){
            setInTransaction();
            int ok = insertCategory(const_cast<KKSCategory*>(c->tableCategory()));
            restoreInTransaction();
            if(ok != OK_CODE){
                if(!inTransaction())
                    db->rollback();
                return ERROR_CODE;
            }
        }
    }

    if(c->recAttrCategory()){
        //если категория не сохранена - сохраним
        if(c->recAttrCategory()->id() <= 0){
            setInTransaction();
            int ok = insertCategory(const_cast<KKSCategory*>(c->recAttrCategory()));
            restoreInTransaction();
            if(ok != OK_CODE){
                if(!inTransaction())
                    db->rollback();
                return ERROR_CODE;
            }
        }
    }

    //если у категории не сохранен тип - сохраним его
    if(c->type()->id() <= 0){
        setInTransaction();
        int ok = insertType(const_cast<KKSType*>(c->type()));
        restoreInTransaction();
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    }

    //изменяем запись в таблице io_category
    KKSObjectExemplar * eio = KKSConverter::categoryToExemplar(loader, c);
    int ok = eiof->updateEIO(eio);
    if(ok == ERROR_CODE){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }
    
    //теперь обновим атрибуты категории
    /*TODO На время запрещаем обновление атрибутов в категориях*/

    /*
    setInTransaction();
    ok = deleteCategoryAttrs(c->id());
    restoreInTransaction();
    if(ok != OK_CODE)
    {
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }
    */

    KKSMap<int, KKSCategoryAttr *>::const_iterator pca;
    for (pca = c->attributes().constBegin(); pca != c->attributes().constEnd(); pca++)
    {
        KKSCategoryAttr * a = pca.value();
        setInTransaction();
        int ok = updateCategoryAttr(c->id(), a);
        restoreInTransaction();
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    }
    
    //
    //рубрики
    //
    if(c->rootRubric()){
        setInTransaction();
        int ok = cUpdateRubrics(c);
        restoreInTransaction();
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    }
    if (!c->getTemplates().isEmpty())
    {
        setInTransaction();
        int ok = this->saveCatTemplates (const_cast<KKSCategory*>(c));
        restoreInTransaction();
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    }

    int accOk = this->writeCatAccessRules (c->getAccessRules(), c->id());
    if (accOk < 0)
    {
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    if(!inTransaction())
        db->commit();

    return OK_CODE;
}


int KKSPPFactory::deleteCategoryAttrs(int idCategory) const
{
    if(idCategory <= 0)
        return ERROR_CODE;

    QString sql = QString("delete from attrs_categories where id_io_category = %1").arg(idCategory);
    int ok = db->executeCmd(sql);
    if(ok != OK_CODE){
        return ERROR_CODE;
    }

    return OK_CODE;
}


int KKSPPFactory::deleteCategory(KKSCategory* c) const
{
    if(!db || !c || c->id() <= 0)
        return ERROR_CODE;

    if(!inTransaction())
        db->begin();

    QString sql = QString("select cDeleteCategory(%1)").arg(c->id());
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() != 1 || res->getCellAsInt(0, 0) != 1){
        if(!inTransaction())
            db->rollback();
        
        if(res)
            delete res;

        return ERROR_CODE;
    }
    
    if(!inTransaction())
        db->commit();

    return OK_CODE;
}

int KKSPPFactory::insertType(KKSType * t) const
{
    if(!db || !t)
        return ERROR_CODE;

    t->setId(-1);
    KKSObjectExemplar * eio = KKSConverter::typeToExemplar(loader, t);
    //QString str = KKSDumper::dump(eio);
    //qWarning("%s", str.toLocal8Bit().constData());
    int ok = eiof->insertEIO(eio);
    if(ok == ERROR_CODE){
        return ERROR_CODE;
    }
    t->setId(eio->id());
    eio->release();
    
    return OK_CODE;
}

int KKSPPFactory::insertIOType(KKSType * t) const
{
    if(!db || !t)
        return ERROR_CODE;

    t->setId(-1);
    KKSObjectExemplar * eio = KKSConverter::ioTypeToExemplar(loader, t);
    //QString str = KKSDumper::dump(eio);
    //qWarning("%s", str.toLocal8Bit().constData());
    int ok = eiof->insertEIO(eio);
    if(ok == ERROR_CODE){
        return ERROR_CODE;
    }
    t->setId(eio->id());
    eio->release();
    
    return OK_CODE;
}

int KKSPPFactory::insertCategoryAttr(int idCategory, KKSCategoryAttr * a) const
{
    if(!db || !a)
        return ERROR_CODE;

    if(a->id() <= 0){
        a->setId(-1);
        KKSObjectExemplar * eio = KKSConverter::attributeToExemplar(loader, a);
        int ok = eiof->insertEIO(eio);
        if(ok == ERROR_CODE){
            return ERROR_CODE;
        }
        a->setId(eio->id());
        eio->release();
    }

    int idAttr = a->id();
    QString defVal;
    if(!a->defValue().isNull())
        defVal = a->defValue().valueForInsert();
    bool isMandatory = a->isMandatory();
    bool isReadOnly = a->isReadOnly();

    QString sql = QString("select cAddAttr(%1, %2, %3, %4, %5)")
                            .arg(idCategory)
                            .arg(idAttr)
                            .arg(defVal.isEmpty() ? QString("NULL") : QString("asString(%1, false)").arg(defVal))
                            .arg(isMandatory ? "true" : "false")
                            .arg(isReadOnly ? "true" : "false");

    KKSResult * res = db->execute(sql);

    if(!res || res->getRowCount() != 1){
        if(res)
            delete res;

        return ERROR_CODE;
    }

    int idCategoryAttr = res->getCellAsInt(0, 0);
    delete res;

    if(idCategoryAttr <= 0)
        return ERROR_CODE;

    a->setIdCategoryAttr(idCategoryAttr);

    return OK_CODE;
}

int KKSPPFactory::updateCategoryAttr(int idCategory, KKSCategoryAttr * a) const
{
    if(!db || !a)
        return ERROR_CODE;

    if(a->idCategoryAttr() <= 0 || a->id() <= 0){
        return ERROR_CODE;
    }

    int idAttr = a->id();
    
    QString defVal;
    if(!a->defValue().isNull())
        defVal = a->defValue().valueForInsert();
    bool isMandatory = a->isMandatory();
    bool isReadOnly = a->isReadOnly();

    QString sql = QString("select cUpdateAttr(%1, %2, %3, %4, %5)")
                            .arg(idCategory)
                            .arg(idAttr)
                            .arg(defVal.isEmpty() ? QString("NULL") : QString("asString(%1, false)").arg(defVal))
                            .arg(isMandatory ? "true" : "false")
                            .arg(isReadOnly ? "true" : "false");

    KKSResult * res = db->execute(sql);

    if(!res || res->getRowCount() != 1){
        if(res)
            delete res;

        return ERROR_CODE;
    }

    int idCategoryAttr = res->getCellAsInt(0, 0);
    delete res;

    if(idCategoryAttr <= 0)
        return ERROR_CODE;

    return OK_CODE;
}

int KKSPPFactory::updateLifeCycle(KKSLifeCycleEx * lc) const
{
    if(!lc)
        return ERROR_CODE;

    if(!inTransaction())
        db->begin();
    
    //
    //считаем, что состояния в БД сохранены ранее тем или иным образом
    //

    QString sql = QString("select cUpdateLifeCycle(%1, '%2', %3, %4);")
                     .arg(lc->id())
                     .arg(lc->name())
                     .arg(lc->desc().isEmpty() ? QString("NULL") : QString("'") + lc->desc() + QString("'"))
                     .arg(lc->startState() ? QString::number(lc->startState()->id()) : QString("NULL"));

    KKSResult * res = db->execute(sql);
    if(!res){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    int ok = res->getCellAsInt(0, 0);
    delete res;
    if(ok <= 0){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    KKSMap<int, KKSState *> ss = lc->states();
    KKSMap<int, KKSState *>::const_iterator i;
    QString ids;
    if(ss.count() > 0){
        ids = QString::number(-1);//чтобы удобнее было потом обрабатывать запятую

        for (i=ss.constBegin(); i != ss.constEnd(); i++){
            const KKSState * s = i.value();
            if(s)
                ids += ", " + QString::number(s->id());

            QString sql = QString("select lcUpdateState(%1, %2);")
                               .arg(lc->id())
                               .arg(s->id());

            KKSResult * res = db->execute(sql);
            if(!res){
                if(!inTransaction())
                    db->rollback();
                return ERROR_CODE;
            }
            int ok = res->getCellAsInt(0, 0);
            delete res;
            if(ok != OK_CODE){
                if(!inTransaction())
                    db->rollback();
                return ERROR_CODE;
            }
        }
    }
    
    //удаляем все оставшиеся связки с состояниями
    if(ids.isEmpty())
        sql = QString("select lcRemoveStates(%1, ARRAY[-1])").arg(lc->id());
    else
        sql = QString("select lcRemoveStates(%1, ARRAY[%2])").arg(lc->id()).arg(ids);
    
	res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    ok = res->getCellAsInt(0, 0);
    delete res;
    if(ok != 1){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

/*****/

    ids = QString::number(-1);//чтобы удобнее было потом обрабатывать запятую
    for (int i=0; i<lc->stateCrosses().count(); i++)
    {
        KKSStateCross * sc = lc->stateCrosses().at (i);
        if(sc->stateSrc()->id() <= 0){
            if(!inTransaction())
                db->rollback();
            qWarning() << "Incorrect KKSState in KKSStateCross. Not present in states of lifeCycle.";
            return ERROR_CODE;
        }
        if(sc->stateDst()->id() <= 0){
            if(!inTransaction())
                db->rollback();
            qWarning() << "Incorrect KKSState in KKSStateCross. Not present in states of lifeCycle.";
            return ERROR_CODE;
        }
        
        int idSrc = sc->stateSrc()->id();
        int idDst = sc->stateDst()->id();


        QString sql = QString("select lcUpdateStateCross(%1, %2, %3, %4)")
                                .arg(QString("' '"))
                                .arg(lc->id())
                                .arg(idSrc)
                                .arg(idDst);

        KKSResult * res = db->execute(sql);
        if(!res){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
        int idCross = res->getCellAsInt(0, 0);
        ids += ", " + QString::number(idCross);//запоминаем идентификатор в справочнике переходов, который должен остаться в БД

        delete res;
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    
    }

    //удаляем все оставшиеся связки с переходами
    sql;
    if(ids.isEmpty())
        sql = QString("select lcRemoveStateCrosses(%1, ARRAY[-1])").arg(lc->id());
    else
        sql = QString("select lcRemoveStateCrosses(%1, ARRAY[%2])").arg(lc->id()).arg(ids);
    
	res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    ok = res->getCellAsInt(0, 0);
    delete res;
    if(ok != 1){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

/*****/

    if(!inTransaction())
        db->commit();

    return OK_CODE;
}

int KKSPPFactory::deleteLifeCycle(int idLifeCycle) const
{
    if(idLifeCycle <= 0)
        return ERROR_CODE;

    if(!inTransaction())
        db->begin();

    QString sql = QString("select cDeleteLifeCycle(%1);").arg(idLifeCycle);
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() != 1 || res->getCellAsInt(0, 0) != 1){
        if(!inTransaction())
            db->rollback();
        
        if(res)
            delete res;

        return ERROR_CODE;
    }

    delete res;
    
    if(!inTransaction())
        db->commit();

    return OK_CODE;
}


int KKSPPFactory::insertLifeCycle(KKSLifeCycleEx * lc) const
{
    
    if(!lc)
        return ERROR_CODE;

    if(!inTransaction())
        db->begin();

    if(lc->startState() && lc->startState()->id() <= 0){
        int ok = insertState(lc->startState());
        if(ok != OK_CODE){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    }

    QString sql = QString("select cInsertLifeCycle('%1', %2, %3)")
                     .arg(lc->name())
                     .arg(lc->desc().isEmpty() ? QString("NULL") : QString("'") + lc->desc() + QString("'"))
                     .arg(lc->startState() ? QString::number(lc->startState()->id()) : QString("NULL"));
    
    KKSResult * res = db->execute(sql);
    if(!res){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    int idLifeCycle = res->getCellAsInt(0, 0);
    delete res;

    if(idLifeCycle <= 0){
        if(!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    lc->setId(idLifeCycle);
        
    KKSMap<int, KKSState *> ss = lc->states();
    KKSMap<int, KKSState *>::iterator i;
    if(ss.count() > 0){
        KKSState * s=0;
        for (i=ss.begin(); i != ss.end(); i++)
        {
            s = i.value();

            if(s && s->id() <= 0){
                int ok = insertState(s);
                if(ok != OK_CODE){
                    if(!inTransaction())
                        db->rollback();
                    return ERROR_CODE;
                }
            }

            QString sql = QString("select lcInsertState(%1, %2);")
                               .arg(lc->id())
                               .arg(s->id());

            KKSResult * res = db->execute(sql);
            if(!res){
                if(!inTransaction())
                    db->rollback();
                return ERROR_CODE;
            }
            int ok = res->getCellAsInt(0, 0);
            delete res;
            if(ok != OK_CODE){
                if(!inTransaction())
                    db->rollback();
                return ERROR_CODE;
            }
        }
        
    }
    
    //считаем, что переходы между состояниями возможны только сренди состояний, находящихся в списке состояний ЖЦ
    //поэтому здесь все состояния должны уже быть сохранены в БД.
    //если встретится состояние с ИД <= 0,  то считаем это ошибкой
    //(кроме прочего, система триггеров в БД не позволит сохранить состояния, отсутствующие в списке)
    for (int i=0; i<lc->stateCrosses().count(); i++)
    {
        KKSStateCross * sc = lc->stateCrosses().at (i);
        if(sc->stateSrc()->id() <= 0){
            if(!inTransaction())
                db->rollback();
            qWarning() << "Incorrect KKSState in KKSStateCross. Not present in states of lifeCycle.";
            return ERROR_CODE;
        }
        if(sc->stateDst()->id() <= 0){
            if(!inTransaction())
                db->rollback();
            qWarning() << "Incorrect KKSState in KKSStateCross. Not present in states of lifeCycle.";
            return ERROR_CODE;
        }
        
        int idSrc = sc->stateSrc()->id();
        int idDst = sc->stateDst()->id();


        QString sql = QString("select cInsertStateCross(%1, %2, %3, %4)")
                                .arg(QString("' '"))
                                .arg(lc->id())
                                .arg(idSrc)
                                .arg(idDst);

        KKSResult * res = db->execute(sql);
        if(!res){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
        int ok = res->getCellAsInt(0, 0);
        delete res;
        if(ok <= 0){
            if(!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    
    }
    
    if(!inTransaction())
        db->commit();

    return OK_CODE;
}

bool KKSPPFactory::categoryInUse(KKSCategory * c) const
{
    if(!c)
        return false;

    bool bInUse = false;
    
    if(!inTransaction()){
        db->begin();
    }
    else
    {
        qWarning() << "KKSPPFactory::categoryInUse() method was invoked."
                      "A transaction is currently started. "
                      "Because of current version of " +kksSitoName+" Server "
                      "does not supported savepoints (subtransactions) "
                      "you cannot verify using of the category."
                      "So returning error!"
                      "Invoke this method not in transaction block!";
        return true;
    }
    //QString sql = QString("delete from io_category where id_category");
    
    QString sql = QString("update io_categories set id_child = NULL where id_child = %1").arg(c->id());
    int ok = db->executeCmd(sql);
    if(ok != OK_CODE){
        if(!inTransaction())
            db->rollback();
        return false;
    }

    setInTransaction();
    ok = deleteCategory(c);
    restoreInTransaction();
    if(ok != OK_CODE)
        bInUse = true;

    db->rollback();

    return bInUse;
}

int KKSPPFactory::insertState(KKSState * s) const
{
    if(!db || !s)
        return ERROR_CODE;

    s->setId(-1);
    QString sql = QString("select cInsertState('%1', %2)")
                     .arg(s->name())
                     .arg(s->desc().isEmpty() ? QString("NULL") : QString("'") + s->desc() + QString("'"));
    KKSResult * res = db->execute(sql);
    if(!res)
        return ERROR_CODE;

    int id = 0;
    id = res->getCellAsInt(0, 0);
    delete res;
    if(id<=0){
        return ERROR_CODE;
    }

    s->setId(id);
    
    return OK_CODE;
}

int KKSPPFactory::updateState(KKSState * s) const
{
    if(!db || !s)
        return ERROR_CODE;

    QString sql = QString("select cUpdateState(%1, '%2', %3)")
                     .arg(s->id())
                     .arg(s->name())
                     .arg(s->desc().isEmpty() ? QString("NULL") : QString("'") + s->desc() + QString("'"));
    KKSResult * res = db->execute(sql);
    if(!res)
        return ERROR_CODE;

    int ok = res->getCellAsInt(0, 0);
    delete res;
    if(ok <= 0){
        return ERROR_CODE;
    }
    
    return OK_CODE;
}

int KKSPPFactory::deleteState(int idState) const
{
    if(!db)
        return ERROR_CODE;

    QString sql = QString("select cDeleteState(%1)").arg(idState);
    KKSResult * res = db->execute(sql);
    if(!res)
        return ERROR_CODE;

    int ok = res->getCellAsInt(0, 0);
    delete res;
    if(ok <= 0){
        return ERROR_CODE;
    }
    
    return OK_CODE;
}


KKSAttribute * KKSPPFactory::createAttribute(int id, 
                                             const QString & code, 
                                             const QString & name, 
                                             KKSAttrType::KKSAttrTypes type, 
                                             bool * loaded)
{
    KKSAttribute * a = NULL;
    bool bLoaded = false;

    //пробуем загрузить атрибут из БД по ИД
    if(id > 0)
    {
        a = loader->loadAttribute(id);
        if(a)
            bLoaded = true;
    }
    /*
    if(!a && !code.isEmpty()){
        a = loader->loadAttribute(code);
        if(a)
            bLoaded = true;
    }
    */

    if(!a){
        bLoaded = false;

        a = new KKSAttribute();
        a->setId(id);
        a->setCode(code);
        a->setName(name);
        a->setTitle(name);
        KKSAttrType * t = loader->loadAttrType(type);
        if(t){
            a->setType(t);
            t->release();
        }
    }

    if(loaded)
        *loaded = bLoaded;

    if(!a->type()){
        a->release();
        return NULL;
    }
    
    return a;
}

bool KKSPPFactory::inTransaction() const
{
    return m_inTransaction.top();
}

void KKSPPFactory::setInTransaction() const
{
    m_inTransaction.push(true);
}

void KKSPPFactory::restoreInTransaction() const
{
    m_inTransaction.pop();
}

int KKSPPFactory::setAsExecutedCommand(int idObject, int idJournal, bool draft) const
{
    if(idJournal <= 0 || idObject <= 0)
        return ERROR_CODE;
    
    QString sql = QString("select cjInsert(%1, %2, %3)").arg(idObject).arg(idJournal).arg(draft ? "TRUE" : "FALSE");
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() != 1){
        if(res)
            delete res;

        return ERROR_CODE;
    }

    int ok = res->getCellAsInt(0, 0);
    delete res;

    if(ok <= 0)
        return ERROR_CODE;

    return OK_CODE;
}

int KKSPPFactory::setAsCommandResult(int idObject, int idJournal) const
{
    if(idJournal <= 0 || idObject <= 0)
        return ERROR_CODE;
    
    QString sql = QString("select cjSetAsResult(%1, %2)").arg(idObject).arg(idJournal);
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() != 1){
        if(res)
            delete res;

        return ERROR_CODE;
    }

    int ok = res->getCellAsInt(0, 0);
    delete res;

    if(ok <= 0)
        return ERROR_CODE;

    return OK_CODE;
}

int KKSPPFactory::updateUserTemplates(const KKSObject * io) const
{
    if(!io || io->id() <= 0)
        return ERROR_CODE;

    int idUser = loader->getUserId();
    int idObject = io->id();
    KKSTemplate * t = io->tableTemplate();
    if(t && t->id() > 0){
        QString sql = QString("select updateUserTemplate(%1, %2, %3, %4)")
                            .arg(idUser)
                            .arg(idObject)
                            .arg(t->id())
                            .arg(0);
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
    }
    else if (t && t->id() <= 0)
    {
        QString sql = QString("select clearUserTemplate(%1, %2, %3)")
                            .arg(idUser)
                            .arg(idObject)
                            .arg(0);
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
    }

    t = io->attrTemplate();

    if(t && t->id() > 0){
        QString sql = QString("select updateUserTemplate(%1, %2, %3, %4)")
                            .arg(idUser)
                            .arg(idObject)
                            .arg(t->id())
                            .arg(1);
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
    }
    else if (t && t->id() <= 0)
    {
        QString sql = QString("select clearUserTemplate(%1, %2, %3)")
                            .arg(idUser)
                            .arg(idObject)
                            .arg(1);
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
    }

    return OK_CODE;
}

/*
int KKSPPFactory::updateIncludes(const KKSObject * io) const
{
    if(!io || io->id() <= 0)
        return ERROR_CODE;

    int idObject = io->id();
    int ok = deleteIncludes(idObject);
    if(ok != OK_CODE){
        return ERROR_CODE;
    }
    
    ok = insertIncludes(io);
    if(ok != OK_CODE){
        return ERROR_CODE;
    }
    
    return OK_CODE;
}
*/

int KKSPPFactory::deleteIncludes(int idObject) const
{
    QString sql = QString("select ioDeleteIncludes(%1)").arg(idObject);
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

int KKSPPFactory::updateIncludes(const KKSObject * io) const
{
    if(!io || io->id() <= 0)
        return ERROR_CODE;

    KKSRubric * rootRubric = io->rootRubric();
    if(!rootRubric)
        return OK_CODE;

    int cnt = rootRubric->rubrics().count();
    if(cnt  == 0)
        return OK_CODE;
    
    int ok;
    if(rootRubric->id() > 0)
        ok = updateRubric(rootRubric);
    else
        ok = insertRubric(rootRubric, -1, io->id(), true);

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
        sql = QString("select ioDeleteIncludes(%1, ARRAY[-1])").arg(io->id());
    else
        sql = QString("select ioDeleteIncludes(%1, ARRAY[%2])").arg(io->id()).arg(ids);
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


int KKSPPFactory::insertIncludes(const KKSObject * io) const
{
    if(!io || io->id() <= 0)
        return ERROR_CODE;

    KKSRubric * rootRubric = io->rootRubric();
    if(!rootRubric)
        return OK_CODE;

    int cnt = rootRubric->rubrics().count();
    if(cnt  == 0)
        return OK_CODE;
    
    int ok = insertRubric(rootRubric, -1, io->id(), true);
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

int KKSPPFactory::cUpdateRubrics(const KKSCategory* c) const
{
    if(!c || c->id() <= 0)
        return ERROR_CODE;

    int idCategory = c->id();
    int ok = cDeleteRubrics(idCategory);
    if(ok != OK_CODE){
        return ERROR_CODE;
    }
    
    ok = cInsertRubrics(c);
    if(ok != OK_CODE){
        return ERROR_CODE;
    }
    
    return OK_CODE;
}

int KKSPPFactory::cDeleteRubrics(int idCategory) const
{
    QString sql = QString("select cDeleteRubrics(%1)").arg(idCategory);
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

int KKSPPFactory::cInsertRubrics(const KKSCategory * c) const
{
    if(!c || c->id() <= 0)
        return ERROR_CODE;

    KKSRubric * rootRubric = c->rootRubric();
    if(!rootRubric)
        return OK_CODE;

    int cnt = rootRubric->rubrics().count();
    if(cnt  == 0)
        return OK_CODE;
    
    int ok = cInsertRubric(rootRubric, -1, c->id(), true);
    if(ok <= 0)
        return ERROR_CODE;

    ok = cInsertRubrics(rootRubric);
    if(ok != OK_CODE)
        return ERROR_CODE;
    
    if(ok != OK_CODE){
        rollbackRubrics(rootRubric);
    }

    return OK_CODE;
}


void KKSPPFactory::rollbackRubrics(KKSRubric * parent, bool forUpdate) const
{
    if(!parent)
        return;

    rollbackRubric(parent, forUpdate);
    for(int i=0; i<parent->rubrics().count(); i++){
        KKSRubric * r = parent->rubric(i);
        rollbackRubrics(r, forUpdate);
    }

}

void KKSPPFactory::rollbackRubric(KKSRubric * r, bool forUpdate) const
{
    if(!r)
        return;

    if(forUpdate && r->m_intId > 0) //если рубрика загружена из БД, то не надо ее помечать как несозданную
        return;
    
    r->setId(-1);
}

void KKSPPFactory::commitRubrics(KKSRubric * parent) const
{
    if(!parent)
        return;

    commitRubric(parent);
    for(int i=0; i<parent->rubrics().count(); i++){
        KKSRubric * r = parent->rubric(i);
        commitRubrics(r);
    }

}

void KKSPPFactory::commitRubric(KKSRubric * r) const
{
    if(!r)
        return;

    r->m_intId = r->id();
}

int KKSPPFactory::cInsertRubrics(KKSRubric * parent) const
{
    if(!parent)
        return ERROR_CODE;

    int cnt = parent->rubrics().count();

    for(int i=0; i<cnt; i++){
        KKSRubric * r = parent->rubric(i);
        int ok = cInsertRubric(r, parent->id(), -1, false);
        if(ok <= 0)
            return ERROR_CODE;

        ok = cInsertRubrics(r);
        if(ok != OK_CODE)
            return ERROR_CODE;
    }

    return OK_CODE;
}

int KKSPPFactory::cInsertRubric(KKSRubric * r, int idParent, int idCategory, bool root) const
{
    if(!r)
        return ERROR_CODE;

    int idRubric = ERROR_CODE;

    QString sql = QString("select * from cinsertrubric (%1, %2, '%3')")
                          .arg( (root || idParent <= 0) ? QString("NULL") : QString::number(idParent))
                          .arg(root ? QString::number(idCategory) : QString("NULL"))
                          .arg(r->name());

    KKSResult * res = db->execute (sql);
    if (!res)
        return ERROR_CODE;

    idRubric = res->getCellAsInt (0, 0);
    delete res;

    r->setId (idRubric);

    return idRubric;
}

int KKSPPFactory::updateRubrics(KKSRubric * parent, int idMyDocsRubricator) const
{
    if(!parent)
        return ERROR_CODE;

    if (parent->isCategorized())
        return OK_CODE;
    int cnt = parent->rubrics().count();

    for(int i=0; i<cnt; i++){
        KKSRubric * r = parent->rubric(i);
        if (r->isCategorized())
            continue;
        
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
    QString sql = QString("delete from io_rubricator where id_rubric = %1").arg(parent->id());
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

int KKSPPFactory::insertRubrics(KKSRubric * parent, int idMyDocsRubricator) const
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

int KKSPPFactory::updateRubric(KKSRubric * r) const
{
    if (!r || r->id() <= 0)
        return ERROR_CODE;
    
    if (r->isCategorized())
        return OK_CODE;

    int idRubric = r->id();
    QByteArray rIconB;
    QBuffer rIconBuff (&rIconB);
    rIconBuff.open(QIODevice::WriteOnly);
    r->getIcon().pixmap(QSize(22, 22)).save(&rIconBuff, "XPM");

    QString sql = QString("select * from ioUpdateIncludeLocal (%1, '%2', '%3', '%4', %5, %6, %7);")
                          .arg(r->id())
                          .arg (r->name())
                          .arg (r->code())
                          .arg (r->desc())
                          .arg (r->getCategory() ? QString::number (r->getCategory()->id()) : QString ("NULL::int4"))
                          .arg (r->getSearchTemplate() ? QString::number (r->getSearchTemplate()->id()) : QString("NULL::int4"))
                          .arg (r->getIcon().isNull() ? QString("NULL") : QString("'%1'").arg (QString(rIconB)));

    KKSResult * res = db->execute (sql);
    if (!res)
    {
        return ERROR_CODE;
    }
    idRubric = res->getCellAsInt (0, 0);
    delete res;

    r->setId (idRubric);

    int ok = updateUserPrivileges (r, idRubric);
    if(ok <= 0){
        //!!!
    }

    return idRubric;
}

int KKSPPFactory::insertRubric(KKSRubric * r, int idParent, int idObject, bool root, int idMyDocsRubricator) const
{
    if (!r)
        return ERROR_CODE;
    
    if (r->isCategorized())
        return OK_CODE;

    int idRubric = ERROR_CODE;//eiof->getNextSeq("rubricator", "id");
    QByteArray rIconB;
    QBuffer rIconBuff (&rIconB);
    rIconBuff.open(QIODevice::WriteOnly);
    r->getIcon().pixmap(QSize(22, 22)).save(&rIconBuff, "XPM");
    
    QString sql = QString("select * from ioinsertinclude (%1, %2, '%3', '%4', '%5', %6, %7, NULL, %8);")
                          .arg ( (root || idParent <= 0) ? QString("NULL") : QString::number(idParent))
                          .arg (root ? QString::number(idObject) : QString("NULL"))
                          .arg (r->name())
                          .arg (r->code())
                          .arg (r->desc())
                          .arg (r->getSearchTemplate() ? QString::number (r->getSearchTemplate()->id()) : QString("NULL::int4"))
                          .arg (r->getCategory() ? QString::number (r->getCategory()->id()) : QString ("NULL::int4"))
                          .arg (r->getIcon().isNull() ? QString("NULL") : QString("'%1'").arg (QString(rIconB)));

    qDebug () << __PRETTY_FUNCTION__ << sql;

    KKSResult * res = db->execute (sql);
    if (!res)
    {
        return ERROR_CODE;
    }
    idRubric = res->getCellAsInt (0, 0);
    delete res;

    if (idMyDocsRubricator > 0 && r->id() == idMyDocsRubricator)
    {
        int res = createMyDocsRubricator(idRubric);
        if (res != OK_CODE)
            return ERROR_CODE;
    }

    r->setId (idRubric);

    int ok = updateUserPrivileges (r, idRubric);
    qDebug () << __PRETTY_FUNCTION__ << ok;

    return idRubric;
}

int KKSPPFactory::insertRubricItem(int idRubric, int idObject, bool isAutomated) const
{
    if(idRubric <= 0 || idObject <= 0)
        return OK_CODE;

    QString sql = QString("insert into io_rubricator (id_io_object, id_rubric, is_automated) values(%1, %2, %3)")
		                  .arg(idObject)
						  .arg(idRubric)
						  .arg(isAutomated ? QString("TRUE") : QString("FALSE"));
    
    int ok = db->executeCmd(sql);
    if(ok != OK_CODE){
        return ERROR_CODE;
    }

    return OK_CODE;
}

int KKSPPFactory::createMyDocsRubricator(int idRubricator) const
{
    if(idRubricator <= 0)
        return ERROR_CODE;

    int idUser = loader->getUserId();
    QString sql = QString("insert into user_rubricator (id_rubricator, id_user) values(%1, %2)").arg(idRubricator).arg(idUser);
    int ok = db->executeCmd(sql);
    if(ok != OK_CODE){
        return ERROR_CODE;
    }

    return OK_CODE;
}

int KKSPPFactory::updateRubricators(KKSRubric * rootRubric, bool bMyDocs) const
{
    Q_UNUSED(bMyDocs);
    if(!rootRubric)
        return ERROR_CODE;

    int cnt = rootRubric->rubrics().count();
    if(cnt  == 0)
        return OK_CODE;
    
    db->begin();

    int idMyDocsRubricator = getMyDocsRubricatorId();

    int ok;
    ok = updateRubrics(rootRubric, idMyDocsRubricator);
    if (ok < 0 )
    {
        rollbackRubrics(rootRubric, true);
        db->rollback();
        return ERROR_CODE;
    }

    //удаляем рубрики, которые не вошли в новый перечень
    QString ids = rootRubric->getFullTreeOfDeletedIds();

    QString sql;
    if(ids.isEmpty()){
        db->commit();
        return OK_CODE;
    }
   
    sql = QString("select deleteRubrics(ARRAY[%1])").arg(ids);
    KKSResult * res = db->execute(sql);
    ok = -1;
    if(res){
        ok = res->getCellAsInt(0, 0);
        delete res;
    }
    if(ok < 0){
        db->rollback();
        return ERROR_CODE;
    }

    db->commit();

    return OK_CODE;
}

int KKSPPFactory::deleteRubricators(bool bMyDocs) const
{
    int idUser = loader->getUserId();
    QString sql = QString("select deleteRubricators(%1, %2)").arg(idUser).arg(bMyDocs ? "TRUE" : "FALSE");
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

int KKSPPFactory::insertRubricators(KKSRubric * rootRubric, int idMyDocsRubricator, bool bMyDocs) const
{

    Q_UNUSED (bMyDocs);
    if(!rootRubric)
        return ERROR_CODE;

    int cnt = rootRubric->rubrics().count();
    if(cnt  == 0)
        return OK_CODE;
    
    int ok = insertRubrics(rootRubric, idMyDocsRubricator);
    if (ok < 0) //OK_CODE)
        return ERROR_CODE;
    
    if (ok < 0)// OK_CODE)
    {
        rollbackRubrics(rootRubric);
    }

    return OK_CODE;
}

int KKSPPFactory::getMyDocsRubricatorId() const
{
    int idUser = loader->getUserId();
    QString sql = QString("select id_rubricator from user_rubricator where id_user = %1").arg(idUser);
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return ERROR_CODE;
    }

    int idRubricator = res->getCellAsInt(0, 0);
    delete res;

    return idRubricator;
}

int KKSPPFactory::setAsLogging(int idOper, bool yes) const
{
    QString sql;
    if(yes)
        sql = QString("select setAsLogging(%1)").arg(idOper);
    else
        sql = QString("select setAsNotLogging(%1)").arg(idOper);

    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() == 0 || res->getCellAsInt(0, 0) != 1){
        if(res)
            delete res;

        return ERROR_CODE;
    }

    return OK_CODE;
}

int KKSPPFactory::addLogAttrs(int idObject, int idOper) const
{
    int idUser = loader->getUserId();
    QString sql = QString("insert into log (id_user, id_io_object, id_operation_type, datetime) "
                          "values (%1, %2, %3, current_timestamp)")
                                .arg(idUser)
                                .arg(idObject)
                                .arg(idOper);
    int ok = db->executeCmd(sql);
    if(ok != OK_CODE){
        return ERROR_CODE;
    }

    return OK_CODE;
}

int KKSPPFactory::updateUserPrivileges(const KKSObject * io) const
{
    if(!io || io->id() <= 0)
        return ERROR_CODE;

    int idObject = io->id();
    //
    //удаляем все существующие права
    //
    QString sql = QString("select dropPrivileges(%1)").arg(idObject);
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
    
    //
    //задаем новые дискреционные права на объект
    //права для ролей AuthorBoss и AuthorUnit отдельно не задаем
    //если они заданы, то должны существовать соответствующие записи 
    //в списке io->privileges() (т.е. права исключений)
    //отдельно задается только права роли OTHERS_ROLE

    const KKSAccessEntity * acl = io->getAccessRules();
    ok = writeAccessRules (acl, io->id());
    if(ok != 1)
        return ERROR_CODE;
/*    KKSMap<int, KKSPrivilege *> privs = acl->getPrivilegies();
    KKSMap<int, KKSPrivilege *>::const_iterator pa;
    for (pa=privs.constBegin(); pa!=privs.constEnd(); pa++)
    {
        KKSPrivilege * p = pa.value();
        int idRole = pa.key();
        
        //если все права отсутствуют, то и нет смысла добавлять такую запись
        if(!p->allowReadList() && !p->allowRead() && !p->allowDelete() && !p->allowUpdate() && !p->allowSend())
            continue;

        QString sql = QString("select setPrivileges(%1, %2, %3, %4, %5, %6, %7)")
                                .arg(idRole)
                                .arg(io->id())
                                .arg(p->allowReadList() ? "TRUE" : "FALSE")
                                .arg(p->allowRead() ? "TRUE" : "FALSE")
                                .arg(p->allowDelete() ? "TRUE" : "FALSE")
                                .arg(p->allowUpdate() ? "TRUE" : "FALSE")
                                .arg(p->allowSend() ? "TRUE" : "FALSE");

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

    }

    //применяем права для роли OTHERS_ROLE
    KKSPrivilege * p = acl->getOthersPrivilege();
    if(!p)
        return OK_CODE;
    if(!p->allowReadList() && !p->allowRead() && !p->allowDelete() && !p->allowUpdate() && !p->allowSend())
        return OK_CODE;

    sql = QString("select setPrivileges(%1, %2, %3, %4, %5, %6, %7)")
                            .arg(OTHERS_ROLE)
                            .arg(io->id())
                            .arg(p->allowReadList() ? "TRUE" : "FALSE")
                            .arg(p->allowRead() ? "TRUE" : "FALSE")
                            .arg(p->allowDelete() ? "TRUE" : "FALSE")
                            .arg(p->allowUpdate() ? "TRUE" : "FALSE")
                            .arg (p->allowSend() ? "TRUE" : "FALSE");

    res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return ERROR_CODE;
    }
    ok = res->getCellAsInt(0, 0);
    delete res;

    if(ok != 1)
        return ERROR_CODE;
*/
    return OK_CODE;
}

int KKSPPFactory::writeAccessRules (const KKSAccessEntity * acl, int idObject) const
{
    if (!acl || idObject <= 0)
        return ERROR_CODE;

    KKSMap<int, KKSPrivilege *> privs = acl->getPrivilegies();
    KKSMap<int, KKSPrivilege *>::const_iterator pa;
    for (pa=privs.constBegin(); pa!=privs.constEnd(); pa++)
    {
        KKSPrivilege * p = pa.value();
        int idRole = pa.key();

        //если все права отсутствуют, то и нет смысла добавлять такую запись
        if(!p->allowReadList() && !p->allowRead() && !p->allowDelete() && !p->allowUpdate() && !p->allowSend())
            continue;

        QString sql = QString("select setPrivileges(%1, %2, %3, %4, %5, %6, %7)")
                                .arg(idRole)
                                .arg(idObject)
                                .arg(p->allowReadList() ? "TRUE" : "FALSE")
                                .arg(p->allowRead() ? "TRUE" : "FALSE")
                                .arg(p->allowDelete() ? "TRUE" : "FALSE")
                                .arg(p->allowUpdate() ? "TRUE" : "FALSE")
                                .arg(p->allowSend() ? "TRUE" : "FALSE");

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

    }

    //применяем права для роли OTHERS_ROLE
    KKSPrivilege * p = acl->getOthersPrivilege();
    if(!p)
        return OK_CODE;
    if(!p->allowReadList() && !p->allowRead() && !p->allowDelete() && !p->allowUpdate() && !p->allowSend())
        return OK_CODE;

    QString sql = QString("select setPrivileges(%1, %2, %3, %4, %5, %6, %7)")
                            .arg(OTHERS_ROLE)
                            .arg(idObject)
                            .arg(p->allowReadList() ? "TRUE" : "FALSE")
                            .arg(p->allowRead() ? "TRUE" : "FALSE")
                            .arg(p->allowDelete() ? "TRUE" : "FALSE")
                            .arg(p->allowUpdate() ? "TRUE" : "FALSE")
                            .arg (p->allowSend() ? "TRUE" : "FALSE");

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

int KKSPPFactory::writeCatAccessRules (const KKSAccessEntity * acl, int idCategory) const
{
    if (!acl || idCategory <= 0)
        return ERROR_CODE;

    KKSMap<int, KKSPrivilege *> privs = acl->getPrivilegies();
    KKSMap<int, KKSPrivilege *>::const_iterator pa;
    for (pa=privs.constBegin(); pa!=privs.constEnd(); pa++)
    {
        KKSPrivilege * p = pa.value();
        int idRole = pa.key();

        //если все права отсутствуют, то и нет смысла добавлять такую запись
        if(!p->allowReadList() && !p->allowRead() && !p->allowDelete() && !p->allowUpdate() && !p->allowSend())
            continue;

        QString sql = QString("select cSetPrivileges(%1, %2, %3, %4, %5, %6, %7)")
                                .arg(idRole)
                                .arg(idCategory)
                                .arg(p->allowReadList() ? "TRUE" : "FALSE")
                                .arg(p->allowRead() ? "TRUE" : "FALSE")
                                .arg(p->allowDelete() ? "TRUE" : "FALSE")
                                .arg(p->allowUpdate() ? "TRUE" : "FALSE")
                                .arg(p->allowSend() ? "TRUE" : "FALSE");
        qDebug () << __PRETTY_FUNCTION__ << sql;

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

    }

    //применяем права для роли OTHERS_ROLE
    KKSPrivilege * p = acl->getOthersPrivilege();
    if(!p)
        return OK_CODE;
    if(!p->allowReadList() && !p->allowRead() && !p->allowDelete() && !p->allowUpdate() && !p->allowSend())
        return OK_CODE;

    QString sql = QString("select cSetPrivileges(%1, %2, %3, %4, %5, %6, %7)")
                            .arg(OTHERS_ROLE)
                            .arg(idCategory)
                            .arg(p->allowReadList() ? "TRUE" : "FALSE")
                            .arg(p->allowRead() ? "TRUE" : "FALSE")
                            .arg(p->allowDelete() ? "TRUE" : "FALSE")
                            .arg(p->allowUpdate() ? "TRUE" : "FALSE")
                            .arg (p->allowSend() ? "TRUE" : "FALSE");

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

int KKSPPFactory::updateUserPrivileges(const KKSRubric * rubr, int idRubric) const
{
    if (!rubr || idRubric <= 0)
        return ERROR_CODE;

    //
    //удаляем все существующие права
    //
    QString sql = QString("select dropRubricPrivileges(%1)").arg(idRubric);
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
    
    //
    //задаем новые дискреционные права на объект
    //права для ролей AuthorBoss и AuthorUnit отдельно не задаем
    //если они заданы, то должны существовать соответствующие записи 
    //в списке io->privileges() (т.е. права исключений)
    //отдельно задается только права роли OTHERS_ROLE
    KKSAccessEntity * acl = rubr->getAccessRules ();
    if (!acl)
        return OK_CODE;

    KKSMap<int, KKSPrivilege *> privs = acl->getPrivilegies();
    KKSMap<int, KKSPrivilege *>::const_iterator pa;
    for (pa=privs.constBegin(); pa!=privs.constEnd(); pa++)
    {
        KKSPrivilege * p = pa.value();
        int idRole = pa.key();
        
        //если все права отсутствуют, то и нет смысла добавлять такую запись
        if(!p->allowReadList() && !p->allowRead() && !p->allowDelete() && !p->allowUpdate())
            continue;

        QString sql = QString("select setRubricPrivileges(%1, %2, %3, %4, %5, %6)")
                                .arg(idRole)
                                .arg(idRubric)
                                .arg(p->allowReadList() ? "TRUE" : "FALSE")
                                .arg(p->allowRead() ? "TRUE" : "FALSE")
                                .arg(p->allowDelete() ? "TRUE" : "FALSE")
                                .arg(p->allowUpdate() ? "TRUE" : "FALSE");

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

    }

    //применяем права для роли OTHERS_ROLE
    KKSPrivilege * p = acl->getOthersPrivilege();
    qDebug () << __PRETTY_FUNCTION__ << rubr->name () << p;
    if(!p)
        return OK_CODE;
    if(!p->allowReadList() && !p->allowRead() && !p->allowDelete() && !p->allowUpdate())
        return OK_CODE;

    sql = QString("select setRubricPrivileges(%1, %2, %3, %4, %5, %6)")
                            .arg(OTHERS_ROLE)
                            .arg(idRubric)
                            .arg(p->allowReadList() ? "TRUE" : "FALSE")
                            .arg(p->allowRead() ? "TRUE" : "FALSE")
                            .arg(p->allowDelete() ? "TRUE" : "FALSE")
                            .arg(p->allowUpdate() ? "TRUE" : "FALSE");

    res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return ERROR_CODE;
    }
    ok = res->getCellAsInt(0, 0);
    delete res;

    if(ok != 1)
        return ERROR_CODE;
   
    return OK_CODE;
}

int KKSPPFactory :: insertSearchGroup (int idParent, KKSFilterGroup * group) const
{
    if (!group)
        return ERROR_CODE;

    if (!inTransaction())
        db->begin();

    int operation = group->AND() ? 1 : 0;
    bool isNot = false;
    QString sql = QString("select ioInsertSearchGroup (%1, %2, %3);")
                        .arg (idParent < 0 ? QString("NULL::int4") : QString::number (idParent))
                        .arg (operation)
                        .arg (isNot ? QString("TRUE") : QString("FALSE"));

    KKSResult * res = db->execute(sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;

        if (!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    int idGroup = res->getCellAsInt (0, 0);
    delete res;

    if (idGroup < 0)
    {
        if (!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    group->setId (idGroup);

    KKSList<const KKSFilterGroup*> subGroups = group->groups();
    for (int i=0; i<subGroups.count(); i++)
    {
        KKSFilterGroup * sGroup = const_cast<KKSFilterGroup *>(subGroups[i]);
        int resgr = this->insertSearchGroup (idGroup, sGroup);
        if (resgr < 0)
        {
            if (!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    }

    KKSList<const KKSFilter*> filters = group->filters();
    for (int i=0; i<filters.count(); i++)
    {
        KKSFilter * f = const_cast<KKSFilter *>(filters[i]);
        int idCriteria = this->insertSearchCriterion (f);
        if (idCriteria < 0)
        {
            if (!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
        int resgrc = this->insertSearchCriterionIntoGroup (idCriteria, idGroup);
        if (resgrc < 0)
        {
            if (!inTransaction())
                db->rollback();
            return ERROR_CODE;
        }
    }

    if (!inTransaction())
        db->commit();

    return idGroup;
}

int KKSPPFactory :: deleteSearchGroup (int idGroup) const
{
    QString sql = QString("select ioDeleteGroup (%1);")
                        .arg (idGroup < 0 ? QString("NULL::int4") : QString::number (idGroup));

    if (!inTransaction())
        db->begin();

    KKSResult * res = db->execute(sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;

        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }

    int ok = res->getCellAsInt (0, 0);
    delete res;

    if (ok < 0)
    {
        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }

    if (!inTransaction())
        db->commit();

    return ok;
}

int KKSPPFactory :: clearCriteriaGroup (int idGroup) const
{
    QString sql = QString("select ioClearCriteriaGroup (%1);")
                        .arg (idGroup < 0 ? QString("NULL::int4") : QString::number (idGroup));

    if (!inTransaction())
        db->begin();

    KKSResult * res = db->execute(sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;
 
        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }

    int ok = res->getCellAsInt (0, 0);
    delete res;

    if (ok < 0)
    {
        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }

    if (!inTransaction())
        db->commit();

    return ok;
}

int KKSPPFactory :: insertSearchTemplateType (KKSSearchTemplateType * t) const
{
    if(!t)
        return OK_CODE;

    if (!inTransaction())
        db->begin();

    KKSSearchTemplateType * parent = t->parent();
    if(parent){
        if(parent->id() <= 0){
            int ok = insertSearchTemplateType(parent);
            if(ok != OK_CODE){
                if(!inTransaction())
                    db->rollback();
                return ERROR_CODE;
            }
        }
    }

    QString sql = QString("select insertSearchTemplateType('%1', %2, %3)")
                           .arg(t->name())
                           .arg(t->desc().isEmpty() ? QString("NULL") : QString("'") + t->desc() + QString("'") )
                           .arg(t->parent() ? QString::number(t->parent()->id()) : QString("NULL"));
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() <= 0){
        if(res)
            delete res;
        
        if(!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }

    t->setId(res->getCellAsInt(0, 0));

    delete res;

    if (!inTransaction())
        db->commit();
    
    return OK_CODE;
}

int KKSPPFactory :: insertSearchTemplate (KKSSearchTemplate * st) const
{
    if (!st)
        return ERROR_CODE;

    if (!inTransaction())
        db->begin();

    KKSFilterGroup *mainGroup = st->getMainGroup ();
    int idMainGroup = this->insertSearchGroup (-1, mainGroup);
    if (idMainGroup < 0 || st->name().isEmpty())
    {
        if (!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    if(st->type()){
        if(st->type()->id() <= 0){
            setInTransaction();
            int ok = insertSearchTemplateType(st->type());//идентификатор сохраним в него прямо
            restoreInTransaction();
            if(ok != OK_CODE){
                if(!inTransaction())
                    db->rollback();
                return ERROR_CODE;
            }
        }
    }

    QString sql = QString("select ioInsertSearchTemplate('%1', %2, %3, %4, %5);")
                        .arg (st->name())
                        .arg (QString::number(idMainGroup))
                        .arg (st->idCategory() <= 0 ? QString::number( IO_TABLE_CATEGORY_ID) : QString::number(st->idCategory()))
                        .arg (st->type() ? QString::number(st->type()->id()) : QString::number(1))
                        .arg (st->desc().isEmpty() ? QString("NULL") : QString("'") + st->desc() + QString("'"));

    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;

        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }

    int ok = res->getCellAsInt (0, 0);
    delete res;

    if (ok < 0)
    {
        if (!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    if (!inTransaction())
        db->commit();
    return ok;
}

int KKSPPFactory :: updateSearchTemplate (KKSSearchTemplate * st) const
{
    if (!st || st->id() < 0 || !st->getMainGroup())
        return ERROR_CODE;

    if (!inTransaction())
        db->begin();


    if(st->type()){
        if(st->type()->id() <= 0){
            setInTransaction();
            int ok = insertSearchTemplateType(st->type());//идентификатор сохраним в него прямо
            restoreInTransaction();
            if(ok != OK_CODE){
                if(!inTransaction())
                    db->rollback();
                return ERROR_CODE;
            }
        }
    }

    int idSearchTemplate = st->id ();
    QString sql = QString("select ioUpdateSearchTemplate(%1, '%2', %3, %4, %5, %6);")
                        .arg (idSearchTemplate)
                        .arg (st->name())
                        .arg (st->getMainGroup()->id())
                        .arg (st->idCategory() <= 0 ? QString::number( IO_TABLE_CATEGORY_ID) : QString::number(st->idCategory()))
                        .arg (st->type() ? QString::number(st->type()->id()) : QString::number(1))
                        .arg (st->desc().isEmpty() ? QString("NULL") : QString("'") + st->desc() + QString("'"));

    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;

        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }

    int ok = res->getCellAsInt (0, 0);
    delete res;

    if (ok < 0)
    {
        if (!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    if (!inTransaction())
        db->commit();

    return ok;
}

int KKSPPFactory :: deleteSearchTemplate (int idSearchTemplate) const
{
    if (idSearchTemplate < 0)
        return ERROR_CODE;

    if (!inTransaction())
        db->begin();

    QString sql = QString("select ioDeleteSearchTemplate (%1);")
                        .arg (idSearchTemplate);

    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;

        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }

    int ok = res->getCellAsInt (0, 0);
    delete res;

    if (ok < 0)
    {
        if (!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    if (!inTransaction())
        db->commit();
    return ok;
}

int KKSPPFactory :: insertSearchCriterion (KKSFilter * f) const
{
    if (!f)
        return ERROR_CODE;

    const KKSAttribute * attr = f->attribute();
    if (!attr)
        return ERROR_CODE;

//    QString attrCode = attr->id ();
    QString value;

    if (f->values().isEmpty() && 
        f->operation() != KKSFilter::foIsNull && 
        f->operation() != KKSFilter::foIsNotNull)
    {
        return ERROR_CODE;
    }
    else if(f->operation() == KKSFilter::foIsNull || 
            f->operation() == KKSFilter::foIsNotNull)
    {
        value = QString::null;
    }
    else{
        const KKSValue * val = f->value (0);
        if (!val)
            return ERROR_CODE;
        
        value = val->value ();
        QRegExp reg ("(')");
        if (value.contains (reg))
            value.replace (reg, "\\'");
    }
    int idType = 1;//val->type ();
    int idOperation = f->operation();
    bool isNot = false;
    QString sql = QString ("select ioInsertSearchCriterion (%1, '%2', %3, %4, %5, %6);")
                        .arg (attr->id())
                        .arg (value.isEmpty() ? QString ("NULL::varchar") : value)
                        .arg (idType < 0 ? QString ("NULL::int4") : QString::number (idType))
                        .arg (idOperation < 0 ? QString ("NULL::int4") : QString::number (idOperation))
                        .arg (isNot ? QString ("TRUE") : QString ("FALSE"))
                        .arg (f->caseSensitive() ? QString ("TRUE") : QString ("FALSE"));

    if (!inTransaction())
        db->begin();

    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;

        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }

    int ok = res->getCellAsInt (0, 0);
    delete res;

    if (ok < 0)
    {
        if (!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    f->setId (ok);

    if (!inTransaction())
        db->commit();

    return ok;
}

int KKSPPFactory :: insertSearchCriterionIntoGroup (int idCriterion, int idGroup) const
{
    if (idCriterion < 0 || idGroup < 0)
        return ERROR_CODE;

    QString sql = QString ("select ioInsertCriterionIntoGroup (%1, %2);")
                        .arg (idCriterion)
                        .arg (idGroup);

    if (!inTransaction())
        db->begin();

    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;

        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }

    int ok = res->getCellAsInt (0, 0);
    delete res;

    if (ok < 0)
    {
        if (!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    if (!inTransaction())
        db->commit();

    return ok;
}

int KKSPPFactory :: insertAddTable (KKSObject * io, 
                                    const QString& tableName, 
                                    KKSCategory * c, 
                                    const QString& tname,
                                    int idSearchTemplate,
                                    const QString & refTableName) const
{
    if (!io || !c || !db || tableName.isEmpty())
        return ERROR_CODE;

    QString sql = QString ("select ioInsertTable('%1', %2, %3, '%4', NULL, %5, %6);")
                    .arg (tableName)
                    .arg (io->id())
                    .arg (c->id())
                    .arg (tname.isEmpty() ? tableName : tname)
                    .arg (idSearchTemplate <= 0 ? QString("NULL") : QString::number(idSearchTemplate))
                    .arg (refTableName.isEmpty() ? QString("NULL") : QString("'%1'").arg(refTableName));

    if (!inTransaction())
        db->begin();

    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;

        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }
    
    int ok = res->getCellAsInt (0, 0);
    delete res;

    if (ok < 0)
    {
        if (!inTransaction())
            db->rollback();
        return ERROR_CODE;
    }

    if (!inTransaction())
        db->commit();

    return ok;
}

int KKSPPFactory :: insertAclTemplate (KKSAccessEntity * at) const
{
    if (!at || !at->isNamed())
        return ERROR_CODE;
    QString sql = QString ("select setAclTemplate('%1')").arg (at->name());
    qDebug () << __PRETTY_FUNCTION__ << sql;
    if (!inTransaction())
        db->begin();
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;

        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }

    int ok = res->getCellAsInt (0, 0);
    if (ok < 0)
    {
        delete res;
        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }
    at->setId (ok);
    int accId = at->id();
    qDebug () << __PRETTY_FUNCTION__ << accId;
    delete res;
    KKSMap<int, KKSPrivilege*> bossPrivs = at->getBossPrivilegies ();
    int ier = writePrivilegies (at, bossPrivs);
    if (ier < 0)
    {
        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }
    KKSMap<int, KKSPrivilege*> unitPrivs = at->getUnitPrivilegies ();
    ier = writePrivilegies (at, unitPrivs);
    if (ier < 0)
    {
        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }
    KKSPrivilege * otherP = at->getOthersPrivilege();
    if (otherP)
    {
        KKSMap<int, KKSPrivilege*> op;
        op.insert (3, otherP);
        ier = writePrivilegies (at, op);
        if (ier < 0)
        {
            if (!inTransaction())
                db->rollback();

            return ERROR_CODE;
        }
    }
    KKSMap<int, KKSPrivilege*> privs = at->getPrivilegies ();
    ier = writePrivilegies (at, privs, false);
    if (ier < 0)
    {
        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }
    if (!inTransaction())
        db->commit();
    return at->id();
}

int KKSPPFactory :: updateAclTemplate (KKSAccessEntity * at) const
{
    if (!at || !at->isNamed())
        return ERROR_CODE;
    int accId = at->id();
    QString sql = QString ("select setAclTemplate(%1, '%2')").arg (accId).arg (at->name());
    if (!inTransaction())
        db->begin();
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;

        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }

    int ok = res->getCellAsInt (0, 0);
    if (ok < 0)
    {
        delete res;
        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }
    at->setId (ok);
    delete res;
    KKSMap<int, KKSPrivilege*> bossPrivs = at->getBossPrivilegies ();
    int ier = writePrivilegies (at, bossPrivs);
    if (ier < 0)
    {
        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }
    KKSMap<int, KKSPrivilege*> unitPrivs = at->getUnitPrivilegies ();
    ier = writePrivilegies (at, unitPrivs);
    if (ier < 0)
    {
        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }
    KKSPrivilege * otherP = at->getOthersPrivilege();
    if (otherP)
    {
        KKSMap<int, KKSPrivilege*> op;
        op.insert (3, otherP);
        ier = writePrivilegies (at, op);
        if (ier < 0)
        {
            if (!inTransaction())
                db->rollback();

            return ERROR_CODE;
        }
    }
    KKSMap<int, KKSPrivilege*> privs = at->getPrivilegies ();
    ier = writePrivilegies (at, privs, false);
    if (ier < 0)
    {
        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }
    if (!inTransaction())
        db->commit();
    return at->id();
}

int KKSPPFactory :: deleteAclTemplate (KKSAccessEntity * at) const
{
    if (!at || at->id() < 0 || !at->isNamed())
        return ERROR_CODE;

    return deleteAclTemplate (at->id());
}

int KKSPPFactory :: deleteAclTemplate (int idAclTemplate) const
{
    if (idAclTemplate <= 0)
        return ERROR_CODE;

    QString sql = QString ("select delAclTemplate (%1)").arg (idAclTemplate);
    if (!inTransaction())
        db->begin();
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() != 1)
    {
        if (res)
            delete res;

        if (!inTransaction())
            db->rollback();

        return ERROR_CODE;
    }
    delete res;
    return OK_CODE;
}

int KKSPPFactory :: writePrivilegies (KKSAccessEntity * at, const KKSMap<int, KKSPrivilege*>& privs, bool isSys) const
{
    if (!inTransaction())
        db->begin();
    for (KKSMap<int, KKSPrivilege*>::const_iterator p = privs.constBegin();
         p != privs.constEnd();
         p++)
    {
        if (!p.value())
            continue;
        QString privSql = QString ("select setAclTemplateParams (%1, %2, %3, %4, %5, %6, %7, %8);")
                                    .arg (at->id())
                                    .arg (p.key())
                                    .arg (p.value()->allowReadList() ? QString ("TRUE") : QString ("FALSE"))
                                    .arg (p.value()->allowRead() ? QString ("TRUE") : QString ("FALSE"))
                                    .arg (p.value()->allowUpdate() ? QString ("TRUE") : QString ("FALSE"))
                                    .arg (p.value()->allowDelete() ? QString ("TRUE") : QString ("FALSE"))
                                    .arg (p.value()->allowSend() ? QString ("TRUE") : QString ("FALSE"))
                                    .arg (isSys ? QString("TRUE") : QString ("FALSE"));
        //qDebug () << __PRETTY_FUNCTION__ << privSql;
        KKSResult * privRes = db->execute (privSql);
        if (!privRes || privRes->getRowCount() != 1)
        {
            if (privRes)
                delete privRes;

            if (!inTransaction())
                db->rollback();

            return ERROR_CODE;
        }
        delete privRes;
    }
    if (!inTransaction())
        db->commit();
    return OK_CODE;
}

int KKSPPFactory::insertAttrAttr(int idParentAttr, KKSAttrAttr * aa) const
{
    if(!db || !aa)
        return ERROR_CODE;

    if(aa->id() <= 0){
        aa->setId(-1);
        KKSObjectExemplar * eio = KKSConverter::attributeToExemplar(loader, aa);
        int ok = eiof->insertEIO(eio);
        if(ok == ERROR_CODE){
            return ERROR_CODE;
        }
        aa->setId(eio->id());
        eio->release();
    }

    int idAttr = aa->id();
    QString defVal = aa->defValue().valueForInsert();
    bool isMandatory = aa->isMandatory();
    bool isReadOnly = aa->isReadOnly();

    QString sql = QString("select aInsertAttrAttr(%1, %2, %3, %4, %5)")
                            .arg(idParentAttr)
                            .arg(idAttr)
                            .arg(defVal)
                            .arg(isMandatory ? "true" : "false")
                            .arg(isReadOnly ? "true" : "false");


    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() != 1)
    {
        if (res)
            delete res;

        return ERROR_CODE;
    }

    int idAttrAttr = res->getCellAsInt(0, 0);

    delete res;

    return idAttrAttr;
}

int KKSPPFactory::insertAttrAttrs(const KKSAttribute * a) const
{
    if(!db || !a || a->id() <= 0)
        return ERROR_CODE;

    QString sqlIn = "-1";

    KKSMap<int, KKSAttrAttr *>::const_iterator pca;
    for (pca = a->attrsAttrs().constBegin(); pca != a->attrsAttrs().constEnd(); pca++)
    {
        KKSAttrAttr * aa = pca.value();

        int idAttrAttr = insertAttrAttr(a->id(), aa);
        if(idAttrAttr == ERROR_CODE){
            return ERROR_CODE;
        }
        sqlIn += QString(", %1").arg(idAttrAttr);
    }

    QString sql = QString("select aDeleteAttrAttrs(%1, ARRAY[%2])").arg(a->id()).arg(sqlIn);
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() != 1)
    {
        if (res)
            delete res;

        return ERROR_CODE;
    }

    int ok = res->getCellAsInt(0, 0);
    delete res;

    if(ok <= 0)
        return ERROR_CODE;

    return OK_CODE;
}
