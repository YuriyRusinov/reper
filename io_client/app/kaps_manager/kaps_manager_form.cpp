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
#include <QSettings>


KapsManagerForm::KapsManagerForm(KKSDatabase * db, QWidget *parent, Qt::WFlags f)
    : QDialog(parent, f),
    UI (new Ui::kaps_manager_form)
{
    UI->setupUi (this);

    this->db = db;

    currentAppPath = QDir::currentPath();

    initSettingsFile();
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

    //connect(UI->tbAddTaskVarFromFolder, SIGNAL(clicked()), this, SLOT(addTaskVarFromFolder()));
    connect(UI->tbAddTaskVar, SIGNAL(clicked()), this, SLOT(addTaskVar()));
    connect(UI->tbEditTaskVar, SIGNAL(clicked()), this, SLOT(editTaskVar()));
    connect(UI->tbDelTaskVar, SIGNAL(clicked()), this, SLOT(delTaskVar()));

    connect(UI->tbAddFile, SIGNAL(clicked()), this, SLOT(addFile()));
    connect(UI->tbEditFile, SIGNAL(clicked()), this, SLOT(editFile()));
    connect(UI->tbDelFile, SIGNAL(clicked()), this, SLOT(delFile()));
    connect(UI->tbDownloadFile, SIGNAL(clicked()), this, SLOT(getFileFromDD()));
    connect(UI->tbDownloadAll, SIGNAL(clicked()), this, SLOT(downloadAllFiles()));
    connect(UI->tbUploadAll, SIGNAL(clicked()), this, SLOT(uploadAllFiles()));
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
        //loadTaskVars(idTask); //��������� ����, ��������� ��������� ������ tvTasks->currentItemChanged()
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
        //loadFiles(idTaskVar); //��������� ����, ��������� ��������� ������ tvTaskVars->currentItemChanged()
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
        item->setData(0, Qt::UserRole+3, res->getCellAsInt(i, 8));
        
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

QString KapsManagerForm::getCurrentKapsFolder()
{
    QTreeWidgetItem * item = UI->tvKaps->currentItem();
    if(!item)
        return QString::null;

    return item->data(2, Qt::DisplayRole).toString();
}


int KapsManagerForm::getCurrentTask()
{
    QTreeWidgetItem * item = UI->tvTasks->currentItem();
    if(!item)
        return -1;

    return item->data(0, Qt::UserRole+1).toInt();
}

QString KapsManagerForm::getCurrentTaskFolder()
{
    QTreeWidgetItem * item = UI->tvTasks->currentItem();
    if(!item)
        return QString::null;

    return item->data(2, Qt::DisplayRole).toString();
}

int KapsManagerForm::getCurrentTaskVar()
{
    QTreeWidgetItem * item = UI->tvTaskVars->currentItem();
    if(!item)
        return -1;

    return item->data(0, Qt::UserRole+1).toInt();
}

QString KapsManagerForm::getCurrentTaskVarFolder()
{
    QTreeWidgetItem * item = UI->tvTaskVars->currentItem();
    if(!item)
        return QString::null;

    return item->data(5, Qt::DisplayRole).toString();
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
    if(!UI->tvKaps->currentItem())
        return;

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
        QMessageBox::critical(this, tr("������"), tr("��������� ������ ��� �������� ����� ������������ ������ � ��!"));
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
    if(!UI->tvKaps->currentItem())
        return;

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
        QMessageBox::critical(this, tr("������"), tr("��������� ������ ��� �������� ����� ������������ ������ � ��!"));
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
                                    tr("�������� ������������ ������"), 
                                    tr("�� ������������� ������ ������� ������������ ������ � ��� �� �������?"), 
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
    if(!UI->tvTasks->currentItem())
        return;

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
        QMessageBox::critical(this, tr("������"), tr("��������� ������ ��� �������� ������ ������� � ��!"));
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

void KapsManagerForm::addTaskVarFromFolder()
{
    if(!UI->tvTasks->currentItem())
        return;

    int idTask = UI->tvTasks->currentItem()->data(0, Qt::UserRole+1).toInt();
    if(idTask <= 0)
        return;

    QString rootFolder = getRootFolder();
    QString kapsFolder = getCurrentKapsFolder();
    QString taskFolder = getCurrentTaskFolder();
    QString dir = rootFolder + "/" + kapsFolder + "/" + taskFolder;

    QString s = QFileDialog::getExistingDirectory(this, tr("�������� ������������ ������� ������ �������� ���. ������"), dir);
    QDir d(dir);
    dir = d.absolutePath();
    d = QDir(s);
    s = d.absolutePath();

    if(!s.contains(dir)){
        QMessageBox::critical(this, tr("������"), tr("��������� ������� �� �������� ������������ ��� ������������ ������!"));
        return;
    }

    QString name;
    QString desc;
    QString folder = d.dirName();
    QString author;
    QString region;
    QDate date;

    QString mdF = s + "/.metadata";
    if(QFile::exists(mdF)){
        QSettings s(mdF, QSettings::IniFormat, this);
        s.setIniCodec(QTextCodec::codecForLocale());
        
        name = s.value("name", "�� ������").toString();
        desc = s.value("description", "�� ������").toString();;
        date = QDate::fromString(s.value("creation_date", QDate::currentDate()).toString(), "dd.MM.yyyy");
        region = s.value("region", tr("�� ������")).toString();
        author = s.value("author", tr("�� ������")).toString();
    }
    else{
        QMessageBox::critical(this, tr("������"), tr("��������� ������� �� �������� ����� � ����������� ��� �������!"));
        return;
    }



    /*
    EditTaskVarForm * f = new EditTaskVarForm(this);
    if(f->exec() != QDialog::Accepted){
        delete f;
        return;
    }
    */
    
    int id = updateTaskVarInDB(-1, idTask, name, desc, folder, author, region, date);
    if(id <= 0){
        QMessageBox::critical(this, tr("������"), tr("��������� ������ ��� �������� ������ ������� � ��!"));
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
        QMessageBox::critical(this, tr("������"), tr("��������� ������ ��� ��������� ������ ������� � ��!"));
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
                                    tr("�������� �������"), 
                                    tr("�� ������������� ������ ������� ������ � ��� ��� �����?"), 
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
    if(!UI->tvTaskVars->currentItem())
        return;

    //QString projectDir;
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

    QString rootFolder = getRootFolder();
    QString kapsFolder = getCurrentKapsFolder();
    QString taskFolder = getCurrentTaskFolder();
    QString taskVarFolder = getCurrentTaskVarFolder();
    QString dir = rootFolder + "/" + kapsFolder + "/" + taskFolder + "/" + taskVarFolder;

    EditFileForm * f = new EditFileForm(dir, this);
    if(f->exec() != QDialog::Accepted){
        delete f;
        return;
    }
    QString name = f->name();
    QString desc = f->desc();
    QString fileName = f->filePath();
    QString url = f->fileUrl();
    QString ka = f->KA();
    QString region = f->region();
    QDateTime timeShoot = f->timeShoot();
    
    int idF = 0;
    int id = updateFileInDB(-1, idTaskVar, idType, name, fileName, url, ka, region, timeShoot, &idF);
    if(id <= 0){
        if(id != -100)//������ ���, ������ ������������ ����� �� �������� ������������� � �� ���� �� �����
            QMessageBox::critical(this, tr("������"), tr("��������� ������ ��� �������� ������ ����� � ��!"));
        delete f;
        return;
    }
    
    QStringList strings;
    strings << name
            << ka
            << timeShoot.toString("dd.MM.yyyy hh:mm")
            << region
            << fileName;
    
    item = new QTreeWidgetItem(strings);
    item->setData(0, Qt::UserRole+1, id);
    item->setData(0, Qt::UserRole+2, idType);
    item->setData(0, Qt::UserRole+3, idF);
    
    QTreeWidgetItem * parent = UI->tvFiles->topLevelItem(index);
    parent->addChild(item);

    delete f;
}

void KapsManagerForm::editFile()
{
    if(!UI->tvTaskVars->currentItem())
        return;

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
    QString fileName = item->data(4, Qt::DisplayRole).toString();
    int idType = item->data(0, Qt::UserRole+2).toInt();
    int idFile = item->data(0, Qt::UserRole+3).toInt();
    

    QString rootFolder = getRootFolder();
    QString kapsFolder = getCurrentKapsFolder();
    QString taskFolder = getCurrentTaskFolder();
    QString taskVarFolder = getCurrentTaskVarFolder();
    QString dir = rootFolder + "/" + kapsFolder + "/" + taskFolder + "/" + taskVarFolder;

    EditFileForm* f = new EditFileForm(dir, this);
    f->setName(name);
    f->setKA(ka);
    f->setFilePath(fileName);
    f->setTimeShoot(timeShoot);
    f->setRegion(region);
    //f->setAuthor(author);

    if(f->exec() != QDialog::Accepted){
        delete f;
        return;
    }

    name = f->name();
    //desc = f->desc();
    fileName = f->filePath();
    ka = f->KA();
    region = f->region();
    timeShoot = f->timeShoot();
    QString url = f->fileUrl();

    int id = item->data(0, Qt::UserRole+1).toInt();
    
    int idF = item->data(0, Qt::UserRole+3).toInt();
    id = updateFileInDB(id, idTaskVar, idType, name, fileName, url, ka, region, timeShoot, &idF);
    if(id <= 0){
        if(id != -100)//������ ���, ������ ������������ ����� �� �������� ������������� � �� ���� �� �����
            QMessageBox::critical(this, tr("������"), tr("��������� ������ ��� ��������� ������ ������� � ��!"));
        delete f;
        return;
    }
    
    item->setData(0, Qt::DisplayRole, name);
    item->setData(1, Qt::DisplayRole, ka);
    item->setData(2, Qt::DisplayRole, timeShoot.toString("dd.MM.yyyy hh:mm"));
    item->setData(3, Qt::DisplayRole, region);
    item->setData(4, Qt::DisplayRole, fileName);

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
                                    tr("�������� �����"), 
                                    tr("�� ������������� ������ ������� ���� �� �������?"), 
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

int KapsManagerForm::updateFileInDB(int id, 
                                    int idTaskVar, 
                                    int idType, 
                                    const QString & name,
                                    const QString & fileName,
                                    const QString & url, 
                                    const QString & ka, 
                                    const QString & region, 
                                    const QDateTime & timeShoot,
                                    int * idF)
{
    int idFile = id;

    QString sql;
    bool bFileExist = true;//������������ ��� ���������� �����. ���� ������������ ������ �� ����� � ����������� ����� ����� ����, �� �� ����� �������, �������� �� ������������ �� ������� �� ������
    
    if(idFile < 0){
        sql = QString("select id, id_file from files where id_type = %1 and id_task_var = %2 and url = '%3' limit 1")
                                     .arg(QString::number(idType))
                                     .arg(QString::number(idTaskVar))
                                     .arg(fileName);
        KKSResult * r = db->execute(sql);
        if(!r)
            return -1;

        bool bUpdate = false;//���� ������ ���� ����� ��������� � TRUE, �� ������������ ������ �������� ���������� ������������� �����

        if(r->getRowCount() != 0){
            int idFile = r->getCellAsInt(0, 0);
            int idF = r->getCellAsInt(0, 1);

            int ok = QMessageBox::warning(this, tr("��������!"), tr("� ������� ������� ��� ������������ ���� � ���������:\n%1\n����� ������������ ������ ��� �����. �������� ��� ���� ���������� � �� ����������?\n����� �� - ����� ����������� ������ ����� � ����������\n����� ��� - ����� ����������� ������ ������ �����\n����� ������ - ���� � ��� ���������� � �� ��������� ��� ���������.").arg(fileName), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if(ok == QMessageBox::Yes){
                int yes = deleteFileInDB(idFile);
                if(yes <= 0){
                    //QMessageBox::critical(this, tr("������"), tr("��������� ������ ��� ������ ����� �� �����. �������� � ������ ������ ��������"), QMessageBox::Ok);
                    return -1;
                }
            }
            else if(ok == QMessageBox::Cancel){
                return -100;//������ ���, ������ ������������ ����� �� �������� ������������� � �� ���� �� �����
            }
            else{
                bUpdate = true;
                //������ ���� �� �� �������
                QString s = QString("select rDeleteFile(id_file) from files where id = %1 and id_file is not null").arg(QString::number(idFile));
                KKSResult * res = db->execute(s);
                if(!res || res->getRowCount() != 1){
                    if(res)
                        delete res;

                    return -1;
                }
                delete res;

                //������ ������ �� ������� io_urls
                s = QString("update files set id_file = NULL where id = %1; delete from io_urls where id = %2")
                                    .arg(QString::number(idFile))
                                    .arg(QString::number(idF));
                res = db->execute(s);
                if(!res || res->resultStatus() != KKSResult::CommandOk){
                    if(res)
                        delete res;

                    return -1;
                }
                delete res;

                sql = QString("update files set name = '%1', url = '%2' where id = %3")
                                    .arg(name)
                                    .arg(fileName)
                                    .arg(QString::number(idFile));


                
            }
        }

        delete r;

        if(!bUpdate){
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
                                .arg(fileName)
                                .arg(ka)
                                .arg(timeShoot.toString("dd.MM.yyyy hh:mm"))
                                .arg(region);
        }
    }
    else{
        sql = QString("update files set name = '%1', url = '%2', ka_type = '%3', shooting_time = '%5'::timestamp, region = '%6' where id = %4")
                            .arg(name)
                            .arg(fileName)
                            .arg(ka)
                            .arg(QString::number(idFile))
                            .arg(timeShoot.toString("dd.MM.yyyy hh:mm"))
                            .arg(region);

        if(QFile::exists(url)){
            int ok = QMessageBox::question(this, tr("��������!"), tr("�������� ����� ���� �����?"), QMessageBox::Yes | QMessageBox::No);
            if(ok == QMessageBox::No)
                bFileExist = false;//������ �������� ���� �� �����
            else{
                QString s = QString("select id_file from files where id = %1")
                                             .arg(QString::number(idFile));
                KKSResult * r = db->execute(s);
                if(!r || r->getRowCount() == 0){
                    if(r)
                        delete r;
                    return -1;
                }
                int idF = r->getCellAsInt(0, 0);
                //������ ���� �� �� �������
                s = QString("select rDeleteFile(id_file) from files where id = %1 and id_file is not null").arg(QString::number(idFile));
                KKSResult * res = db->execute(s);
                if(!res || res->getRowCount() != 1){
                    if(res)
                        delete res;

                    return -1;
                }
                delete res;

                //������ ������ �� ������� io_urls
                s = QString("update files set id_file = NULL where id = %1; delete from io_urls where id = %2")
                                    .arg(QString::number(idFile))
                                    .arg(QString::number(idF));
                res = db->execute(s);
                if(!res || res->resultStatus() != KKSResult::CommandOk){
                    if(res)
                        delete res;

                    return -1;
                }
                delete res;            }
        }
        else{
            bFileExist = false;
        }

    }

    KKSResult * res = db->execute(sql);
    if(!res || res->resultStatus() != KKSResult::CommandOk){
        if(res)
            delete res;

        return -1;
    }

    if(bFileExist){
        int idF1 = addFileToDD(url, name, idFile);
        if(idF)
            *idF = idF1;
    }

    return idFile;
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
    QString sql = QString("select rDeleteFile(id_file) from files where id = %1 and id_file is not null").arg(QString::number(idFile));
    KKSResult * res = db->execute(sql);

    if(!res || res->getRowCount() != 1){
        if(res)
            delete res;

        return -1;
    }
    delete res;

    sql = QString("select id_file from files where id = %1 and id_file is not null").arg(QString::number(idFile));
    res = db->execute(sql);
    if(!res || res->getRowCount() == 0){
        if(res)
            delete res;
        return -1;
    }
    int idF = res->getCellAsInt(0, 0);
    delete res;


    sql = QString("delete from files where id = %1; delete from io_urls where id = %2")
                      .arg(QString::number(idFile))
                      .arg(QString::number(idF));
    res = db->execute(sql);
    
    if(!res || res->resultStatus() != KKSResult::CommandOk){
        if(res)
            delete res;

        return -1;
    }
    delete res;

    return 1;
}


int KapsManagerForm::rInsertUrl(const QString & fileName, int fileType, const QString & fileExt)
{
	QString sql = QString("select rInsertUrl('%1', 'not assigned', %2, '%3')")
                                     .arg(fileName)
                                     .arg(fileType)
                                     .arg(fileExt);
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
        
    return idUrl;
}

int KapsManagerForm::createFileInDD(int idUrl, const QString & localUrl)
{
	int ok = kksSito->fileLoader()->rWriteFile(idUrl, localUrl, true, -1, this);

	return ok;
}

int KapsManagerForm::addFileToDD(const QString & url, const QString & name, int idFile)
{
    int idF = rInsertUrl(name, 1, "qq");
    if(idF <= 0)
        return idF;

    int r = createFileInDD(idF, url);
    if(r <= 0){
        QMessageBox::critical(this, tr("������"), tr("��������� ������ ��� �������� ����� �� ������!"));
        return -1;
    }

    QString sql = QString("update files set id_file = %1 where id = %2")
                   .arg(QString::number(idF))
                   .arg(QString::number(idFile));


    KKSResult * res = db->execute(sql);
    
    if(!res || res->resultStatus() != KKSResult::CommandOk){
        if(res)
            delete res;

        return -1;
    }
    delete res;

    return idF;


}

int KapsManagerForm::downloadFile(int idF, const QString & localUrl)
{
	int ok = kksSito->fileLoader()->rGetFile(idF, localUrl, -1, this);

	return ok;
}

int KapsManagerForm::getFileForItem(QTreeWidgetItem * item)
{
    if(!item)
        return -1;

    int idFile = item->data(0, Qt::UserRole+1).toInt();
    if(idFile <= 0)
        return -1;

    int index=0;
    if(item->parent() == NULL){ //toplevelitem
        return -1;
    }

    int idF = item->data(0, Qt::UserRole+3).toInt();
    if(idF <= 0)
        return -1;

    QString localUrl;
    QString fileName = item->data(4, Qt::DisplayRole).toString();
    int idType = item->data(0, Qt::UserRole+2).toInt();
    localUrl = getFileFolder(idType) + "/" + fileName;
        
    int ok = downloadFile(idF, localUrl);

    return ok;
}

void KapsManagerForm::getFileFromDD()
{
    QTreeWidgetItem * item = UI->tvFiles->currentItem();
    if(!item)
        return;

    int res = getFileForItem(item);

}

void KapsManagerForm::initSettingsFile()
{
    if(!QFile::exists(currentAppPath + "./settings.ini")){
        QFile f(currentAppPath + "./settings.ini");
        f.open(QIODevice::WriteOnly);
        f.close();
        QSettings s(currentAppPath + "./settings.ini", QSettings::IniFormat, this);
        s.setValue("root_folder", "C:\\R_KT");
        s.setValue("snimki", "snimki");
        s.setValue("src_data", "src_data");
        s.setValue("med_data", "med_data");
        s.setValue("result_data", "result_data");
    }
}

QString KapsManagerForm::getFileFolder(int idType)
{
    initSettingsFile();

    QSettings s(currentAppPath + "./settings.ini", QSettings::IniFormat, this);
    QString snimki = s.value("snimki", "snimki").toString();
    QString src_data = s.value("src_data", "src_data").toString();
    QString med_data = s.value("med_data", "med_data").toString();
    QString result_data = s.value("result_data", "result_data").toString();


    QString rootFolder = getRootFolder();
    QString kapsFolder = getCurrentKapsFolder();
    QString taskFolder = getCurrentTaskFolder();
    QString taskVarFolder = getCurrentTaskVarFolder();
    QString fFolder;
    switch(idType){
        case 1: fFolder = snimki; break;
        case 2: fFolder = src_data; break;
        case 3: fFolder = med_data; break;
        case 4: fFolder = result_data; break;
        default: return QString::null; break;
    }

    QString dir = rootFolder + "/" + kapsFolder + "/" + taskFolder + "/" + taskVarFolder + "/" + fFolder;

    QDir d;
    if(!d.exists(dir))
        d.mkpath(dir);
    
    return dir;
}

QString KapsManagerForm::getRootFolder()
{
    initSettingsFile();

    QSettings s(currentAppPath + "./settings.ini", QSettings::IniFormat, this);
    QString rootFolder = s.value("root_folder", "C:\\R_KT2").toString();
    return rootFolder;
}

void KapsManagerForm::downloadAllFiles()
{
    if(!UI->tvTaskVars->currentItem())
        return;

    int cnt = UI->tvFiles->topLevelItemCount();
    for(int i=0; i<cnt; i++){
        QTreeWidgetItem * item = UI->tvFiles->topLevelItem(i);
        if(!item)
            continue;

        int childsCount = item->childCount();
        if(childsCount <= 0)
            continue;
        for(int j=0; j<childsCount; j++){
            QTreeWidgetItem *child = item->child(j);
            int res = getFileForItem(child);
            if(res <= 0){
                qWarning() << "ERROR was occured while downloading file to local file system!";
            }
        }
    }
    
}

void KapsManagerForm::uploadAllFiles()
{
    if(!UI->tvTaskVars->currentItem())
        return;

    QString rootFolder = getRootFolder();
    QString kapsFolder = getCurrentKapsFolder();
    QString taskFolder = getCurrentTaskFolder();
    QString taskVarFolder = getCurrentTaskVarFolder();
    QString dir = rootFolder + "/" + kapsFolder + "/" + taskFolder + "/" + taskVarFolder;

    QSettings s(currentAppPath + "./settings.ini", QSettings::IniFormat, this);
    //������
    QString snimki = s.value("snimki", "snimki").toString();
    QDir d(dir + "/" + snimki);
    QStringList files = d.entryList(QDir::Files | QDir::NoDotAndDotDot);
    for(int i=0; i<files.count(); i++){
        QString file = files.at(i);
        if(file.endsWith(".metadata"))
            continue;
        uploadFile(file, dir + "/" + snimki, 1);
    }
    
    //�������� ������
    QString src_data = s.value("src_data", "src_data").toString();
    d = QDir(dir + "/" + src_data);
    files = d.entryList(QDir::Files | QDir::NoDotAndDotDot);
    for(int i=0; i<files.count(); i++){
        QString file = files.at(i);
        if(file.endsWith(".metadata"))
            continue;
        uploadFile(file, dir + "/" + src_data, 2);
    }
    
    //������������� ����������
    QString med_data = s.value("med_data", "med_data").toString();
    d = QDir(dir + "/" + med_data);
    files = d.entryList(QDir::Files | QDir::NoDotAndDotDot);
    for(int i=0; i<files.count(); i++){
        QString file = files.at(i);
        if(file.endsWith(".metadata"))
            continue;
        uploadFile(file, dir + "/" + med_data, 3);
    }

    //�������� ����������
    QString result_data = s.value("result_data", "result_data").toString();
    d = QDir(dir + "/" + result_data);
    files = d.entryList(QDir::Files | QDir::NoDotAndDotDot);
    for(int i=0; i<files.count(); i++){
        QString file = files.at(i);
        if(file.endsWith(".metadata"))
            continue;
        uploadFile(file, dir + "/" + result_data, 4);
    }
}

void KapsManagerForm::uploadFile(const QString & file, const QString & fUrl, int idType)
{
    if(!UI->tvTaskVars->currentItem())
        return;

    int idTaskVar = getCurrentTaskVar();
    if(idTaskVar <= 0)
        return;
    
    QString name;
    QString desc;
    QString fileName;
    QString url = fUrl + "/" + file;
    QString ka;
    QString region;
    QDateTime timeShoot;
 
    QString mdF = fUrl + "/" + file + ".metadata";
    if(QFile::exists(mdF)){
        QSettings s(mdF, QSettings::IniFormat, this);
        s.setIniCodec(QTextCodec::codecForLocale());
        name = s.value("name", file).toString();
        //desc = desc;
        fileName = file;
        ka = s.value("ka", tr("�� ������")).toString();
        region = s.value("region", tr("�� ������")).toString();
        timeShoot = QDateTime::fromString(s.value("timeShoot", QDateTime::currentDateTime()).toString(), "dd.MM.yyyy hh:mm");
    }
    else{
        name = file;
        //desc = desc;
        fileName = file;
        ka = tr("�� ������");
        region = tr("�� ������");
        timeShoot = QDateTime::currentDateTime();
    }
    
    int idF = 0;
    int id = updateFileInDB(-1, idTaskVar, idType, name, fileName, url, ka, region, timeShoot, &idF);
    if(id <= 0){
        if(id != -100)//������ ���, ������ ������������ ����� �� �������� ������������� � �� ���� �� �����
            QMessageBox::critical(this, tr("������"), tr("��������� ������ ��� �������� ������ ����� � ��!"));
        return;
    }
    
    QStringList strings;
    strings << name
            << ka
            << timeShoot.toString("dd.MM.yyyy hh:mm")
            << region
            << fileName;
    
    QTreeWidgetItem * item = new QTreeWidgetItem(strings);
    item->setData(0, Qt::UserRole+1, id);
    item->setData(0, Qt::UserRole+2, idType);
    item->setData(0, Qt::UserRole+3, idF);
    
    QTreeWidgetItem * parent = UI->tvFiles->topLevelItem(idType-1);//idType-1 - ��� ���������� ������ ��������
    parent->addChild(item);
}
