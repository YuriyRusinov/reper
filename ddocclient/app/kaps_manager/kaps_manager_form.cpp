#include <QtGui>

#include "ui_kaps_manager_form.h"
#include "kaps_manager_form.h"

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

}

void KapsManagerForm::initKapsData()
{
    for(int i=0; i<6; i++){
        UI->tvKaps->topLevelItem(i)->setData(0, Qt::UserRole+1, i+1);
    }
}

void KapsManagerForm::slotKapsChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous)
{
    int idKaps = current->data(0, Qt::UserRole+1).toInt();
    if(idKaps <= 0)
        return;

    activateKaps(idKaps);
}

void KapsManagerForm::slotTaskChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous)
{
    int id = current->data(0, Qt::UserRole+1).toInt();
    if(id <= 0)
        return;

    activateTask(id);
}

void KapsManagerForm::slotTaskVarChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous)
{
    int id = current->data(0, Qt::UserRole+1).toInt();
    if(id <= 0)
        return;

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
    UI->tvFiles->clear();
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
        UI->tvTasks->setCurrentItem(0);
        int idTask = getCurrentTask();
        loadTaskVars(idTask);
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
        UI->tvTaskVars->setCurrentItem(0);
        int idTaskVar = getCurrentTaskVar();
        loadFiles(idTaskVar);
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
        strings << res->getCellAsString(i, 4) //name
                << res->getCellAsString(i, 6)  //KA type
                << res->getCellAsDateTime(i, 7).toString("dd.MM.yyyy hh:mm") //shooting_time
                << res->getCellAsString(i, 8)//region
                << res->getCellAsString(i, 5); //url
        
        QTreeWidgetItem * item = new QTreeWidgetItem(strings);
        int type = res->getCellAsInt(i, 1);
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

