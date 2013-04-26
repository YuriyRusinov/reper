#include <QDateTime>
#include <QDateTimeEdit>
#include <QMessageBox>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QtDebug>
#include <QHeaderView>

#include <KKSAttrValue.h>
#include <KKSObject.h>

#include "attrhistory.h"
#include "ui_attr_history.h"
#include "defines.h"

#define c_id 0
#define c_start 1
#define c_stop 2
#define c_value 3
//#define c_measured 4
#define c_source 4
#define c_transfer 5
#define c_description 6

AttrHistory::AttrHistory (const KKSList<KKSAttrValue*> & histlist, QWidget *parent, Qt::WFlags f)
    : QDialog (parent, f),
    UI (new Ui::attr_history)
{

    UI->setupUi (this);

    {
        view (histlist);
    }

}

AttrHistory::~AttrHistory ()
{
    delete UI;
}

void AttrHistory::view(const KKSList<KKSAttrValue*> & histlist)
{

    UI->leGroup->setReadOnly(true);
    UI->leName->setReadOnly(true);
    UI->leType->setReadOnly(true);

    KKSAttrValue * av;
    KKSCategoryAttr * attr;
    QString tempStr;
    //QStandardItem *tree;

    QStandardItemModel *model =new QStandardItemModel;
    QStandardItem *item;

    //Получаем Имя Тип И Группу атрибута
    av = histlist.first();
    attr = av->attribute();
    qDebug()<<"Category ID "<<attr->id()<<" Category Name: "<<attr->name()<<" Category Group: "<<attr->group()->name()<<" Category Type: "<< attr->type()->name();

    UI->leGroup->setText(attr->group()->name());
    UI->leType->setText(attr->type()->name());
    UI->leName->setText(attr->name());

    QStringList listHeader;
    listHeader <<tr("Id")<<tr("Start")<<tr("Stop")<<tr("Value")<<tr("Source")<<tr("Transfer")<<tr("Description");
    model->setHorizontalHeaderLabels(listHeader);

    QHeaderView *headerView = UI->tabHistory->header();
    headerView->setDefaultAlignment(Qt::AlignCenter);

    //Обходим все значениея истории изменений атрибута и вносим в таблицу
    for (int i =0; i<histlist.count(); i++)
    {
        av = histlist.at(i);

            item = new QStandardItem(tempStr.setNum(av->id()));
            item->setTextAlignment(Qt::AlignCenter);
            model->setItem(i,c_id,item);
            tempStr.clear();

        if (av->value().valueForInsert() != NULL)
        {
            item = new QStandardItem(av->value().valueForInsert());
            item->setTextAlignment(Qt::AlignCenter);
            model->setItem(i,c_value,item);
        }


            item = new QStandardItem(av->startDateTime().toString());
            item->setTextAlignment(Qt::AlignCenter);
            model->setItem(i,c_start,item);

            item = new QStandardItem(av->stopDateTime().toString());
            item->setTextAlignment(Qt::AlignCenter);
            model->setItem(i,c_stop,item);


        if (av->ioSrc() != NULL)
        {
            item = new QStandardItem(av->ioSrc()->name());
            item->setTextAlignment(Qt::AlignCenter);
            model->setItem(i,c_source,item);
        }

        if (av->ioSrc1() != NULL)
        {
            item = new QStandardItem(av->ioSrc1()->name());
            item->setTextAlignment(Qt::AlignCenter);if (av->ioSrc1() != NULL)
            model->setItem(i,c_transfer,item);
        }

        //item = new QStandardItem(av->measDateTime().toString());
        //item->setTextAlignment(Qt::AlignCenter);
        //model->setItem(i,c_measured,item);


        if (av->desc() != NULL)
        {
            item = new QStandardItem(av->desc());
            item->setTextAlignment(Qt::AlignCenter);
            model->setItem(i,c_description,item);
        }

    }

       UI->tabHistory->setModel(model);


}
