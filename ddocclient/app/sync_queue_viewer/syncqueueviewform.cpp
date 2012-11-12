#include "syncqueueviewform.h"

#include <QMessageBox>
#include <QModelIndex>
#include <QScrollBar>
#include <QDialog>
#include <QWidget>
#include <QTreeWidget>

#include "syncqueueview.h"
#include "ui_sync_queue_view_form.h"

#include "syncqueueitemmodel.h"

SyncQueueViewForm::SyncQueueViewForm(QWidget * parent):
    QDialog (parent)
{
    ui = new Ui::sync_queue_view_form;
    ui->setupUi (this);
    
    syncQueueViewerForm = (SyncQueueViewerForm *)(this->parent());
}

SyncQueueViewForm :: ~SyncQueueViewForm (void)
{
}

void SyncQueueViewForm::on_pbOk_clicked()
{
    close();
}

void SyncQueueViewForm::on_pbCancel_clicked()
{
    close();
}

void SyncQueueViewForm::InitView()
{
    FillView();
}

void SyncQueueViewForm::FillView()
{
    QString strTemp = "";
    QDateTime dtTemp;

    SyncQueueView *syncQueueTreeWnd = syncQueueViewerForm->GetTreeView();
    QItemSelectionModel *selModel = syncQueueTreeWnd->selectionModel();
    SyncQueueItemModel *model = syncQueueTreeWnd->GetModel();

    QModelIndexList list = selModel->selectedIndexes();
    QModelIndex index = list.at(0);
 
    /*   
    strTemp = model->data(index, Qt::UserRole + 100).toString();
    ui->leUserName->setText(strTemp);
    
    strTemp = model->data(index, Qt::UserRole + 101).toString();
    ui->leTypeOperation->setText(strTemp);

    strTemp = model->data(index, Qt::UserRole + 102).toString();
    ui->leGroupOperation->setText(strTemp);

    strTemp = model->data(index, Qt::UserRole + 103).toString();
    dtTemp = dtTemp.fromString(strTemp, Qt::ISODate);
    ui->dteTimeOperation->setDateTime(dtTemp);

    strTemp = model->data(index, Qt::UserRole + 104).toString();
    ui->leId1->setText(strTemp);

    strTemp = model->data(index, Qt::UserRole + 105).toString();
    ui->leId2->setText(strTemp);

    strTemp = model->data(index, Qt::UserRole + 106).toString();
    ParserQString(ui->twOld, strTemp);
    
    strTemp = model->data(index, Qt::UserRole + 107).toString();
    ParserQString(ui->twNew, strTemp);
*/
}

void SyncQueueViewForm::on_pbUp_clicked()
{
    SyncQueueView *syncQueueTreeWnd = syncQueueViewerForm->GetTreeView();
    QItemSelectionModel *selModel = syncQueueTreeWnd->selectionModel();
    SyncQueueItemModel *model = syncQueueTreeWnd->GetModel();

    QModelIndexList list = selModel->selectedIndexes();
    QModelIndex index = list.at(0);

    if(index.row() == 0)
        return;

    QItemSelection selection(
            model->index(index.row() - 1, 0, syncQueueTreeWnd->rootIndex()),
            model->index(index.row() - 1, 2, syncQueueTreeWnd->rootIndex()));

    QScrollBar *scroll = syncQueueTreeWnd->verticalScrollBar();

    scroll->setValue(scroll->value() - scroll->singleStep());

    selModel->select(selection, QItemSelectionModel::ClearAndSelect);

    FillView();
}

void SyncQueueViewForm::on_pbDown_clicked()
{
    SyncQueueView *syncQueueTreeWnd = syncQueueViewerForm->GetTreeView();
    QItemSelectionModel *selModel = syncQueueTreeWnd->selectionModel();
    SyncQueueItemModel *model = syncQueueTreeWnd->GetModel();

    QModelIndexList list = selModel->selectedIndexes();
    QModelIndex index = list.at(0);

    int i = index.row() + 1;
    int j = model->rowCount(); 
    if( i == j)
        return;

    QItemSelection selection(
            model->index(index.row() + 1 , 0, syncQueueTreeWnd->rootIndex()),
            model->index(index.row() + 1, 2, syncQueueTreeWnd->rootIndex()));

    i = index.row();

    QScrollBar *scroll = syncQueueTreeWnd->verticalScrollBar();

    scroll->setValue(scroll->value() + scroll->singleStep());

    selModel->select(selection, QItemSelectionModel::ClearAndSelect);
    
    FillView();
}
/*
void LogViewForm::ParserQString(QTreeWidget *treewidget, QString fullString)
{
    QStringList splitStringList;
    treewidget->clear();

    QTreeWidgetItem *hdItem = NULL;
    hdItem = new QTreeWidgetItem();
    hdItem->setText(0, tr("characteristic"));
    hdItem->setText(1, tr("value"));
    treewidget->setHeaderItem(hdItem);

    if(fullString.count() == 0)
        return;

    treewidget->setColumnCount(2);

    QString reg("\", \"");

    splitStringList = fullString.split(reg);   //"id_layer" : "787", "id_user" : "18566", "user_acc" : "1111"
    
    int countSplitStringList = -1;
    countSplitStringList = splitStringList.count();
    
    QPair<QString,QString> pair;
    QList< QPair<QString,QString> > listPair;

    if(countSplitStringList == 1)
    {
        QString temp;
        QStringList temp_pair_split;
        QString temp_left, temp_right;

        temp_pair_split = fullString.split(":");

        temp_left = temp_pair_split.at(0).simplified();
        pair.first = temp_left;

        temp_right = temp_pair_split.at(1).simplified();
        pair.second = temp_right;

        listPair.append(pair);
    }
    
    if(countSplitStringList > 1)
    {
        int i = -1;

        QString temp;
        QStringList temp_pair_split;
        QString temp_left, temp_right;

        for(i = 0;i < countSplitStringList;i++)
        {
            temp = splitStringList.at(i);
            temp_pair_split = temp.split("\" : \"");
            
            temp_left = temp_pair_split.at(0).simplified();
            if(i==0)
                temp_left.remove("\"");
            pair.first = temp_left;

            temp_right = temp_pair_split.at(1).simplified();
            if(i==countSplitStringList-1)
                temp_right.remove("\"");
            pair.second = temp_right;

            listPair.append(pair);
        }
        
    }

    AddToTreeview(treewidget, listPair);
}
 */

/*
void LogViewForm::AddToTreeview(QTreeWidget *treewidget, QList< QPair<QString,QString> > list)
{
    int count = -1;
    int i = -1;
    QTreeWidgetItem *twItem = NULL;
    QTreeWidgetItem *hdItem = NULL;
    
    count = list.count();
    QString tmp = list.at(0).first;
    
    for(i=0;i<count;i++)
    {
        twItem = new QTreeWidgetItem(treewidget);
        twItem->setText(0, list.at(i).first);
        twItem->setText(1, list.at(i).second);
    }
}
 */
