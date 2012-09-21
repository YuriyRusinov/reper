#include <QtGui>

#include "ui_kaps_manager_form.h"
#include "kaps_manager_form.h"
#include "edit_task_form.h"
#include "edit_task_var_form.h"
#include "edit_file_form.h"

#include <kksresult.h>
#include <kksdatabase.h>
#include <defines.h>
#include <KKSList.h>
#include <kkssito.h>
#include <KKSLoader.h>
#include <KKSFileLoader.h>
#include <QFile>



KapsManagerForm::KapsManagerForm(KKSDatabase * db, QWidget *parent, Qt::WFlags f)
    : QDialog(parent, f),
    UI (new Ui::kaps_manager_form)
{
    UI->setupUi (this);

    this->db = db;

    initConnections();
    initKapsData();
	
}

void KapsManagerForm::initConnections()
{
    connect(UI->tvKaps, 
            SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), 
            this, 
            SLOT(slotKapsChanged(QTreeWidgetItem *, QTreeWidgetItem *)));

    connect(UI->tvTasks, 
            SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), 
            this, 
            SLOT(slotTaskChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
    
    connect(UI->tvTaskVars, 
            SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), 
            this, 
            SLOT(slotTaskVarChanged(QTreeWidgetItem *, QTreeWidgetItem *)));

    connect(UI->tvFiles, 
            SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), 
            this, 
            SLOT(slotFileChanged(QTreeWidgetItem *, QTreeWidgetItem *)));

    connect(UI->tbAddTask, SIGNAL(clicked()), this, SLOT(addTask()));
    connect(UI->tbEditTask, SIGNAL(clicked()), this, SLOT(editTask()));
    connect(UI->tbDelTask, SIGNAL(clicked()), this, SLOT(delTask()));

    connect(UI->tbAddTaskVar, SIGNAL(clicked()), this, SLOT(addTaskVar()));
    connect(UI->tbEditTaskVar, SIGNAL(clicked()), this, SLOT(editTaskVar()));
    connect(UI->tbDelTaskVar, SIGNAL(clicked()), this, SLOT(delTaskVar()));

    connect(UI->tbAddFile, SIGNAL(clicked()), this, SLOT(addFile()));
    connect(UI->tbEditFile, SIGNAL(clicked()), this, SLOT(editFile()));
    connect(UI->tbDelFile, SIGNAL(clicked()), this, SLOT(delFile()));
}

void KapsManagerForm::initKapsData()
{
    for(int i=0; i<6; i++){
        UI->tvKaps->topLevelItem(i)->setData(0, Qt::UserRole+1, i+1);
    }
}

void KapsManagerForm::slotKapsChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous)
{
    if(!current){
        clearTasks();
        return;
    }

    int idKaps = current->data(0, Qt::UserRole+1).toInt();
    if(idKaps <= 0){
        clearTasks();
        return;
    }

    activateKaps(idKaps);
}

void KapsManagerForm::slotTaskChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous)
{
    if(!current){
        clearTaskVars();
        return;
    }

    int id = current->data(0, Qt::UserRole+1).toInt();
    if(id <= 0){
        clearTaskVars();
        return;
    }

    activateTask(id);
}

void KapsManagerForm::slotTaskVarChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous)
{
    if(!current){
        clearFiles();
        return;
    }

    int id = current->data(0, Qt::UserRole+1).toInt();
    if(id <= 0){
        clearFiles();
        return;
    }

    activateTaskVar(id);
}

void KapsManagerForm::slotFileChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous)
{

}

void KapsManagerForm::activateKaps(int idKaps)
{
    clearTasks();
    loadTasks(idKaps);
}

void KapsManagerForm::activateTask(int idTask)
{
    clearTaskVars();
    loadTaskVars(idTask);
}

void KapsManagerForm::activateTaskVar(int idTaskVar)
{
    clearFiles();
    loadFiles(idTaskVar);
}

void KapsManagerForm::clearTasks()
{
    UI->tvTasks->clear();
    clearTaskVars();
}

void KapsManagerForm::clearTaskVars()
{
    UI->tvTaskVars->clear();
    clearFiles();
}

void KapsManagerForm::clearFiles()
{
    //UI->tvFiles->clear();
    
    for(int index = 0; index < 4; index++){
        QList<QTreeWidgetItem *> childs = UI->tvFiles->topLevelItem(index)->takeChildren();
        for(int i=0; i<childs.count(); i++)
            delete childs.at(i);
        childs.clear();
    }
}


void KapsManagerForm::loadTasks(int idKaps)
{
    //
    QString sql = QString("select * from tasks where id_kaps = %1 order by 3").arg(QString::number(idKaps));
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() <= 0){
        if(res)
            delete res;
        return;
    }

    int cnt = res->getRowCount();
    
    for(int i=0; i<cnt; i++){
        QStringList strings;
        strings << res->getCellAsString(i, 2) //name
                << res->getCellAsString(i, 4)  //descr
                << res->getCellAsString(i, 3);//folder_name

        
        QTreeWidgetItem * item = new QTreeWidgetItem(strings);
        item->setData(0, Qt::UserRole+1, res->getCell(i, 0));
        UI->tvTasks->addTopLevelItem(item);
    }

    if(cnt > 0){
        UI->tvTasks->setCurrentItem(UI->tvTasks->topLevelItem(0));
        int idTask = getCurrentTask();
        //loadTaskVars(idTask); //вызовется само, поскольку сработает сигнал tvTasks->currentItemChanged()
    }
}

void KapsManagerForm::loadTaskVars(int idTask)
{
    //
    QString sql = QString("select * from task_vars where id_task = %1 order by 3").arg(QString::number(idTask));
    KKSResult * res = db->execute(sql);
    if(!res || res->getRowCount() <= 0){
        if(res)
            delete res;
        return;
    }

    int cnt = res->getRowCount();
    
    for(int i=0; i<cnt; i++){
        QStringList strings;
        strings << res->getCellAsString(i, 2) //name
                << res->getCellAsString(i, 4) //descr
                << res->getCellAsDateTime(i, 5).toString("dd.MM.yyyy") //creation_date
                << res->getCellAsString(i, 6)  //region
                << res->getCellAsString(i, 7) //author
                << res->getCellAsString(i, 3); //folder_name
                

        
        QTreeWidgetItem * item = new QTreeWidgetItem(strings);
        item->setData(0, Qt::UserRole+1, res->getCell(i, 0));
        UI->tvTaskVars->addTopLevelItem(item);
    }

    if(cnt > 0){
        UI->tvTaskVars->setCurrentItem(UI->tvTaskVars->topLevelItem(0));
        int idTaskVar = getCurrentTaskVar();
        //loadFiles(idTaskVar); //вызовется само, поскольку сработает сигнал tvTaskVars->currentItemChanged()
    }

}


void KapsManagerForm::loadFiles(int idTaskVar)
{
    //
    QString sql = QString("select * from files where id_task_var = %1 order by 4").arg(QString::number(idTaskVar));
    KKSResult * res = db->execute(sql);
   
    if(!res || res->getRowCount() <= 0){
        if(res)
            delete res;
        return;
    }

    int cnt = res->getRowCount();
    
    for(int i=0; i<cnt; i++){
        QStringList strings;
        strings << res->getCellAsString(i, 3) //name
                << res->getCellAsString(i, 5)  //KA type
                << res->getCellAsDateTime(i, 6).toString("dd.MM.yyyy hh:mm") //shooting_time
                << res->getCellAsString(i, 7)//region
                << res->getCellAsString(i, 4); //url
        
        QTreeWidgetItem * item = new QTreeWidgetItem(strings);
        int type = res->getCellAsInt(i, 1);
        
        item->setData(0, Qt::UserRole+1, res->getCellAsInt(i, 0));
        item->setData(0, Qt::UserRole+2, type);
        
        QTreeWidgetItem *parent;
        switch(type){
            case 1: 
                parent = UI->tvFiles->topLevelItem(0);
                break;
            case 2: 
                parent = UI->tvFiles->topLevelItem(1);
                break;
            case 3: 
                parent = UI->tvFiles->topLevelItem(2);
                break;
            case 4: 
                parent = UI->tvFiles->topLevelItem(3);
                break;
            default: continue;
        }

        if(!parent)
            continue;

        parent->addChild(item);
    }

}


int KapsManagerForm::getCurrentKaps()
{
    QTreeWidgetItem * item = UI->tvKaps->currentItem();
    if(!item)
        return -1;

    return item->data(0, Qt::UserRole+1).toInt();
}

int KapsManagerForm::getCurrentTask()
{
    QTreeWidgetItem * item = UI->tvTasks->currentItem();
    if(!item)
        return -1;

    return item->data(0, Qt::UserRole+1).toInt();
}

int KapsManagerForm::getCurrentTaskVar()
{
    QTreeWidgetItem * item = UI->tvTaskVars->currentItem();
    if(!item)
        return -1;

    return item->data(0, Qt::UserRole+1).toInt();
}

int KapsManagerForm::getCurrentFile()
{
    QTreeWidgetItem * item = UI->tvFiles->currentItem();
    if(!item)
        return -1;

    return item->data(0, Qt::UserRole+1).toInt();
}


void KapsManagerForm::addTask()
{
    int idKaps = UI->tvKaps->currentItem()->data(0, Qt::UserRole+1).toInt();
    if(idKaps <= 0)
        return;

    EditTaskForm * f = new EditTaskForm(this);
    if(f->exec() != QDialog::Accepted){
        delete f;
        return;
    }
    QString name = f->name();
    QString desc = f->desc();
    QString folder = f->folder();
    
    int id = updateTaskInDB(-1, idKaps, name, desc, folder);
    if(id <= 0){
        QMessageBox::critical(this, tr("Ошибка"), tr("Произошла ошибка при создании новой тематической задачи в БД!"));
        return;
    }
    
    QStringList strings;
    strings << name
            << desc
            << folder;
    
    QTreeWidgetItem * item = new QTreeWidgetItem(strings);
    item->setData(0, Qt::UserRole+1, id);
    UI->tvTasks->addTopLevelItem(item);

    delete f;
}

void KapsManagerForm::editTask()
{
    int idKaps = UI->tvKaps->currentItem()->data(0, Qt::UserRole+1).toInt();
    if(idKaps <= 0)
        return;

    QTreeWidgetItem * item = UI->tvTasks->currentItem();
    if(!item){
        return;
    }

    QString name = item->data(0, Qt::DisplayRole).toString();
    QString desc = item->data(1, Qt::DisplayRole).toString();
    QString folder = item->data(2, Qt::DisplayRole).toString();

    EditTaskForm * f = new EditTaskForm(this);
    f->setName(name);
    f->setDesc(desc);
    f->setFolder(folder);

    if(f->exec() != QDialog::Accepted){
        delete f;
        return;
    }

    name = f->name();
    desc = f->desc();
    folder = f->folder();
    int id = item->data(0, Qt::UserRole+1).toInt();
    
    id = updateTaskInDB(id, idKaps, name, desc, folder);
    if(id <= 0){
        QMessageBox::critical(this, tr("Ошибка"), tr("Произошла ошибка при создании новой тематической задачи в БД!"));
        return;
    }

    item->setData(0, Qt::DisplayRole, name);
    item->setData(1, Qt::DisplayRole, desc);
    item->setData(2, Qt::DisplayRole, folder);

    delete f;
}

void KapsManagerForm::delTask()
{
    QTreeWidgetItem * item = UI->tvTasks->currentItem();
    if(!item)
        return;

    int idTask = item->data(0, Qt::UserRole+1).toInt();
    if(idTask <= 0)
        return;

    int yes = QMessageBox::question(this, 
                                    tr("Удаление тематической задачи"), 
                                    tr("Вы действительно хотите удалить тематическую задачу и все ее проекты?"), 
                                    QMessageBox::Yes | QMessageBox::No);
    if(yes != QMessageBox::Yes)
        return;

    int ok = deleteTaskInDB(idTask);
    if(ok != 1){
        return;
    }

    int index = UI->tvTasks->indexOfTopLevelItem(UI->tvTasks->currentItem());
    delete UI->tvTasks->takeTopLevelItem(index);
}

void KapsManagerForm::addTaskVar()
{
    int idTask = UI->tvTasks->currentItem()->data(0, Qt::UserRole+1).toInt();
    if(idTask <= 0)
        return;

    EditTaskVarForm * f = new EditTaskVarForm(this);
    if(f->exec() != QDialog::Accepted){
        delete f;
        return;
    }
    QString name = f->name();
    QString desc = f->desc();
    QString folder = f->folder();
    QString author = f->author();
    QString region = f->region();
    QDate date = f->creationDate();
    
    int id = updateTaskVarInDB(-1, idTask, name, desc, folder, author, region, date);
    if(id <= 0){
        QMessageBox::critical(this, tr("Ошибка"), tr("Произошла ошибка при создании нового проекта в БД!"));
        return;
    }
    
    QStringList strings;
    strings << name
            << desc
            << date.toString("dd.MM.yyyy")
            << region
            << author
            << folder;
    
    QTreeWidgetItem * item = new QTreeWidgetItem(strings);
    item->setData(0, Qt::UserRole+1, id);
    UI->tvTaskVars->addTopLevelItem(item);

    delete f;
}

void KapsManagerForm::editTaskVar()
{
    int idTask = UI->tvTasks->currentItem()->data(0, Qt::UserRole+1).toInt();
    if(idTask <= 0)
        return;

    QTreeWidgetItem * item = UI->tvTaskVars->currentItem();
    if(!item){
        return;
    }

    QString name = item->data(0, Qt::DisplayRole).toString();
    QString desc = item->data(1, Qt::DisplayRole).toString();
    QDate date = QDate::fromString(item->data(2, Qt::DisplayRole).toString(), QString("dd.MM.yyyy"));
    QString region = item->data(3, Qt::DisplayRole).toString();
    QString author = item->data(4, Qt::DisplayRole).toString();
    QString folder = item->data(5, Qt::DisplayRole).toString();

    EditTaskVarForm* f = new EditTaskVarForm(this);
    f->setName(name);
    f->setDesc(desc);
    f->setFolder(folder);
    f->setCreationDate(date);
    f->setRegion(region);
    f->setAuthor(author);

    if(f->exec() != QDialog::Accepted){
        delete f;
        return;
    }

    name = f->name();
    desc = f->desc();
    folder = f->folder();
    author = f->author();
    region = f->region();
    date = f->creationDate();

    int id = item->data(0, Qt::UserRole+1).toInt();
    
    id = updateTaskVarInDB(id, idTask, name, desc, folder, author, region, date);
    if(id <= 0){
        QMessageBox::critical(this, tr("Ошибка"), tr("Произошла ошибка при изменении нового проекта в БД!"));
        return;
    }
    
    item->setData(0, Qt::DisplayRole, name);
    item->setData(1, Qt::DisplayRole, desc);
    item->setData(2, Qt::DisplayRole, date.toString("dd.MM.yyyy"));
    item->setData(3, Qt::DisplayRole, region);
    item->setData(4, Qt::DisplayRole, author);
    item->setData(5, Qt::DisplayRole, folder);

    delete f;
}

void KapsManagerForm::delTaskVar()
{
    QTreeWidgetItem * item = UI->tvTaskVars->currentItem();
    if(!item)
        return;

    int idTaskVar = item->data(0, Qt::UserRole+1).toInt();
    if(idTaskVar <= 0)
        return;

    int yes = QMessageBox::question(this, 
                                    tr("Удаление проекта"), 
                                    tr("Вы действительно хотите удалить проект и все его файлы?"), 
                                    QMessageBox::Yes | QMessageBox::No);
    if(yes != QMessageBox::Yes)
        return;

    int ok = deleteTaskVarInDB(idTaskVar);
    if(ok != 1){
        return;
    }

    int index = UI->tvTaskVars->indexOfTopLevelItem(UI->tvTaskVars->currentItem());
    delete UI->tvTaskVars->takeTopLevelItem(index);
}

void KapsManagerForm::addFile()
{
    QString projectDir;
    int idTaskVar = UI->tvTaskVars->currentItem()->data(0, Qt::UserRole+1).toInt();
    if(idTaskVar <= 0)
        return;

    QTreeWidgetItem * item = UI->tvFiles->currentItem();
    if(!item)
        return;

    int index=0;
    if(item->parent() == NULL){ //toplevelitem
        index = UI->tvFiles->indexOfTopLevelItem(item);
        if(index < 0)
            return;
    }
    else{
        index = UI->tvFiles->indexOfTopLevelItem(item->parent());
        if(index < 0)
            return;
    }
    int idType = index + 1;

    EditFileForm * f = new EditFileForm(projectDir, this);
    if(f->exec() != QDialog::Accepted){
        delete f;
        return;
    }
    QString name = f->name();
    QString desc = f->desc();
    QString url = f->filePath();
    QString ka = f->KA();
    QString region = f->region();
    QDateTime timeShoot = f->timeShoot();
    
    int id = updateFileInDB(-1, idTaskVar, idType, name, url, ka, region, timeShoot);
    if(id <= 0){
        QMessageBox::critical(this, tr("Ошибка"), tr("Произошла ошибка при создании нового файла в БД!"));
        return;
    }
    
    QStringList strings;
    strings << name
            << ka
            << timeShoot.toString("dd.MM.yyyy hh:mm")
            << region
            << url;
    
    item = new QTreeWidgetItem(strings);
    item->setData(0, Qt::UserRole+1, id);
    item->setData(0, Qt::UserRole+2, idType);
    
    QTreeWidgetItem * parent = UI->tvFiles->topLevelItem(index);
    parent->addChild(item);

    delete f;
}

void KapsManagerForm::editFile()
{
    QString projectDir;
    int idTaskVar = UI->tvTaskVars->currentItem()->data(0, Qt::UserRole+1).toInt();
    if(idTaskVar <= 0)
        return;

    QTreeWidgetItem * item = UI->tvFiles->currentItem();
    if(!item)
        return;

    int index=0;
    if(item->parent() == NULL){ //toplevelitem
        return;
    }
    /*
    else{
        index = UI->tvFiles->indexOfTopLevelItem(item->parent());
        if(index < 0)
            return;
    }
    int idType = index + 1;
    */

    QString name = item->data(0, Qt::DisplayRole).toString();
    QString ka = item->data(1, Qt::DisplayRole).toString();
    QDateTime timeShoot = QDateTime::fromString(item->data(2, Qt::DisplayRole).toString(), QString("dd.MM.yyyy hh:mm"));
    QString region = item->data(3, Qt::DisplayRole).toString();
    QString url = item->data(4, Qt::DisplayRole).toString();
    int idType = item->data(0, Qt::UserRole+2).toInt();
    

    EditFileForm* f = new EditFileForm(projectDir, this);
    f->setName(name);
    f->setKA(ka);
    f->setFilePath(url);
    f->setTimeShoot(timeShoot);
    f->setRegion(region);
    //f->setAuthor(author);

    if(f->exec() != QDialog::Accepted){
        delete f;
        return;
    }

    name = f->name();
    //desc = f->desc();
    url = f->filePath();
    ka = f->KA();
    region = f->region();
    timeShoot = f->timeShoot();

    int id = item->data(0, Qt::UserRole+1).toInt();
    
    id = updateFileInDB(id, idTaskVar, idType, name, url, ka, region, timeShoot);
    if(id <= 0){
        QMessageBox::critical(this, tr("Ошибка"), tr("Произошла ошибка при изменении нового проекта в БД!"));
        return;
    }
    
    item->setData(0, Qt::DisplayRole, name);
    item->setData(1, Qt::DisplayRole, ka);
    item->setData(2, Qt::DisplayRole, timeShoot.toString("dd.MM.yyyy hh:mm"));
    item->setData(3, Qt::DisplayRole, region);
    item->setData(4, Qt::DisplayRole, url);

    delete f;
}

void KapsManagerForm::delFile()
{
    QTreeWidgetItem * item = UI->tvFiles->currentItem();
    if(!item)
        return;

    int idFile = item->data(0, Qt::UserRole+1).toInt();
    if(idFile <= 0)
        return;

    int index=0;
    if(item->parent() == NULL){ //toplevelitem
        return;
    }

    int yes = QMessageBox::question(this, 
                                    tr("Удаление файла"), 
                                    tr("Вы действительно хотите удалить файл из проекта?"), 
                                    QMessageBox::Yes | QMessageBox::No);
    if(yes != QMessageBox::Yes)
        return;

    int ok = deleteFileInDB(idFile);
    if(ok != 1){
        return;
    }

    index = item->parent()->indexOfChild(item);
    item->parent()->takeChild(index);
    delete item;

}

int KapsManagerForm::updateTaskInDB(int id, int idKaps, const QString & name, const QString & desc, const QString & folder)
{
    int idTask = id;

    QString sql;
    
    if(idTask < 0){
        sql = QString("select nextval('tasks_id_seq')");
        KKSResult * res = db->execute(sql);
        if(!res || res->getRowCount() != 1){
            if(res)
                delete res;
            return -1;
        }
        idTask = res->getCellAsInt(0, 0);
        sql = QString("insert into tasks (id, id_kaps, name, description, filder_name) values (%5, %1, '%2', '%3', '%4')").arg(QString::number(idKaps)).arg(name).arg(desc).arg(folder).arg(QString::number(idTask));
    }
    else
        sql = QString("update tasks set name = '%1', description = '%2', filder_name = '%3' where id = %4").arg(name).arg(desc).arg(folder).arg(QString::number(idTask));

    KKSResult * res = db->execute(sql);
    if(!res || res->resultStatus() != KKSResult::CommandOk){
        if(res)
            delete res;

        return -1;
    }

    return idTask;
}

int KapsManagerForm::updateTaskVarInDB(int id, int idTask, const QString & name, const QString & desc, const QString & folder, const QString & author, const QString & region, const QDate & date)
{
    int idTaskVar = id;

    QString sql;
    
    if(idTaskVar < 0){
        sql = QString("select nextval('task_vars_id_seq')");
        KKSResult * res = db->execute(sql);
        if(!res || res->getRowCount() != 1){
            if(res)
                delete res;
            return -1;
        }
        idTaskVar = res->getCellAsInt(0, 0);
        sql = QString("insert into task_vars (id, id_task, name, description, folder_name, creation_date, region, author) values (%1, %2, '%3', '%4', '%5', '%6'::date, '%7', '%8')")
                            .arg(QString::number(idTaskVar))
                            .arg(QString::number(idTask))
                            .arg(name)
                            .arg(desc)
                            .arg(folder)
                            .arg(date.toString("dd.MM.yyyy"))
                            .arg(region)
                            .arg(author);
    }
    else
        sql = QString("update task_vars set name = '%1', description = '%2', folder_name = '%3', creation_date = '%5'::date, region = '%6', author = '%7' where id = %4")
                            .arg(name)
                            .arg(desc)
                            .arg(folder)
                            .arg(QString::number(idTask))
                            .arg(date.toString("dd.MM.yyyy"))
                            .arg(region)
                            .arg(author);

    KKSResult * res = db->execute(sql);
    if(!res || res->resultStatus() != KKSResult::CommandOk){
        if(res)
            delete res;

        return -1;
    }

    return idTaskVar;
}

int KapsManagerForm::updateFileInDB(int id, int idTaskVar, int idType, const QString & name, const QString & url, const QString & ka, const QString & region, const QDateTime & timeShoot)
{
    int idFile = id;

    QString sql;
    
    if(idFile < 0){
        sql = QString("select nextval('files_id_seq')");
        KKSResult * res = db->execute(sql);
        if(!res || res->getRowCount() != 1){
            if(res)
                delete res;
            return -1;
        }
        idFile = res->getCellAsInt(0, 0);
        sql = QString("insert into files (id, id_task_var, id_type, name, url, ka_type, shooting_time, region) values (%1, %2, %3, '%4', '%5', '%6', '%7'::timestamp, '%8')")
                            .arg(QString::number(idFile))
                            .arg(QString::number(idTaskVar))
                            .arg(idType)
                            .arg(name)
                            .arg(url)
                            .arg(ka)
                            .arg(timeShoot.toString("dd.MM.yyyy hh:mm"))
                            .arg(region);
    }
    else
        sql = QString("update files set name = '%1', url = '%2', ka_type = '%3', shooting_time = '%5'::timestamp, region = '%6' where id = %4")
                            .arg(name)
                            .arg(url)
                            .arg(ka)
                            .arg(QString::number(idFile))
                            .arg(timeShoot.toString("dd.MM.yyyy hh:mm"))
                            .arg(region);

    KKSResult * res = db->execute(sql);
    if(!res || res->resultStatus() != KKSResult::CommandOk){
        if(res)
            delete res;

        return -1;
    }

    return idTaskVar;
}

int KapsManagerForm::deleteTaskInDB(int idTask)
{
    QString sql = QString("delete from files where id_task_var in (select id from task_vars where id_task = %1); delete from task_vars where id_task = %1; delete from tasks where id = %1").arg(QString::number(idTask));
    KKSResult * res = db->execute(sql);
    
    if(!res || res->resultStatus() != KKSResult::CommandOk){
        if(res)
            delete res;

        return -1;
    }

    return 1;
}

int KapsManagerForm::deleteTaskVarInDB(int idTaskVar)
{
    QString sql = QString("delete from files where id_task_var = %1; delete from task_vars where id = %1").arg(QString::number(idTaskVar));
    KKSResult * res = db->execute(sql);
    
    if(!res || res->resultStatus() != KKSResult::CommandOk){
        if(res)
            delete res;

        return -1;
    }

    return 1;
}

int KapsManagerForm::deleteFileInDB(int idFile)
{
    QString sql = QString("delete from files where id = %1").arg(QString::number(idFile));
    KKSResult * res = db->execute(sql);
    
    if(!res || res->resultStatus() != KKSResult::CommandOk){
        if(res)
            delete res;

        return -1;
    }

    return 1;
}
