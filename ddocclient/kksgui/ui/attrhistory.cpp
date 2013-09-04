#include <QDateTime>
#include <QDateTimeEdit>
#include <QMessageBox>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QMenu>
#include <QHeaderView>
#include <QContextMenuEvent>
#include <QKeyEvent>
#include <QPoint>
#include <QAction>
#include <QKeySequence>
#include <QtDebug>

#include <KKSAttrValue.h>
#include <KKSObject.h>
#include <KKSAttrHistModel.h>

#include "attrhistory.h"
#include "ui_attr_history.h"
#include "defines.h"
#include "KKSAttrHistoryProxyModel.h"
#include "KKSItemDelegate.h"

const int c_id = 0;
const int c_start = 1;
const int c_stop = 2;
const int c_value = 3;
//#define c_measured 4
const int c_source = 4;
const int c_transfer = 5;
const int c_description = 6;

AttrHistory::AttrHistory (const KKSList<KKSAttrValue*> & histlist, QWidget *parent, Qt::WFlags f)
    : QDialog (parent, f),
    UI (new Ui::attr_history),
    pHistMenu (new QMenu),
    aViewVals (new QAction(tr("View &values"), this))
{

    UI->setupUi (this);

    {
        view (histlist);
    }
    
    pHistMenu->addAction(aViewVals);
    QAbstractItemDelegate * iDeleg = new KKSItemDelegate();
    UI->tvHistory->setItemDelegate(iDeleg);
    connect (aViewVals, SIGNAL(triggered()), this, SLOT (viewVal()) );
    connect (UI->pbUp, SIGNAL(clicked()), this, SLOT (upClicked()) );
    connect (UI->pbDown, SIGNAL (clicked()), this, SLOT (downClicked()) );
    connect (UI->pbClose, SIGNAL(clicked()), this, SLOT (reject()) );
    connect (UI->tvHistory, SIGNAL (doubleClicked(const QModelIndex&)), this, SLOT (viewDblVal (const QModelIndex&)) );
}

AttrHistory::~AttrHistory ()
{
    aViewVals->setParent(0);
    delete aViewVals;
    delete pHistMenu;
    delete UI;
}

void AttrHistory::view(const KKSList<KKSAttrValue*> & histlist)
{

    UI->leGroup->setReadOnly(true);
    UI->leName->setReadOnly(true);
    UI->leType->setReadOnly(true);

    KKSAttrValue * av;
    KKSCategoryAttr * attr;
    //QStandardItem *tree;

    //int n = histlist.count();
    QAbstractItemModel *model =new KKSAttrHistModel (histlist);//QStandardItemModel (n, c_description+1);
    //QStandardItem *item;

    //�������� ��� ��� � ������ ��������
    av = histlist.first();
    attr = av->attribute();
    qDebug()<< __PRETTY_FUNCTION__ << "Category ID "<<attr->id()<<" Category Name: "<<attr->name()<<" Category Group: "<<attr->group()->name()<<" Category Type: "<< attr->type()->name();

    UI->leGroup->setText(attr->group()->name());
    UI->leType->setText(attr->type()->name());
    UI->leName->setText(attr->name());

    QHeaderView *headerView = UI->tvHistory->header();
    headerView->setDefaultAlignment(Qt::AlignCenter);

/*
    QStringList listHeader;
    listHeader <<tr("Id")<<tr("Start")<<tr("Stop")<<tr("Value")<<tr("Source")<<tr("Transfer")<<tr("Description");
    model->setHorizontalHeaderLabels(listHeader);

    //
    // ������� ��� �������� ������� ��������� �������� � ������ � �������
    //
    for (int i =0; i<n; i++)
    {
        av = histlist.at(i);
        int idAv = av->id();

        item = new QStandardItem(QString::number(idAv));
        item->setTextAlignment(Qt::AlignCenter);
        item->setData(idAv, Qt::UserRole);
        item->setData(QVariant::fromValue<KKSAttrValue *>(av), Qt::UserRole+1);
        model->setItem(i,c_id,item);

        if (av->value().valueForInsert() != NULL)
        {
            KKSAttribute * a = av->attribute();
            if (!a)
                continue;
            const KKSAttrType * t = a->type();
            const KKSAttrType * rt = a->refType();
            int tId = t->attrType();
            if (tId == KKSAttrType::atJPG ||
                (rt && rt->attrType() == KKSAttrType::atJPG)
               )
                item = new QStandardItem(tr("<Image data %1>").arg(i));
            else if (tId == KKSAttrType::atSVG ||
                     (rt && rt->attrType() == KKSAttrType::atSVG)
                    )
                item = new QStandardItem(tr("<SVG data %1>").arg(i));
            else if( tId == KKSAttrType::atXMLDoc || 
                    (rt && rt->attrType() == KKSAttrType::atXMLDoc)
                   )
                item = new QStandardItem(tr("<XML document %1>").arg(i));
            else if( tId == KKSAttrType::atVideo || 
                    (rt && rt->attrType() == KKSAttrType::atVideo)
                   )
                item = new QStandardItem(tr("<Video data %1>").arg(i));
            else if (tId == KKSAttrType::atRecordColor || 
                     tId == KKSAttrType::atRecordColorRef)
            {
                item = new QStandardItem(tr("Text example"));
                bool ok;
                quint64 vl = av->value().valueForInsert().toLongLong(&ok);
                if (!ok)
                    continue;
                item->setData(QColor::fromRgba(vl), Qt::BackgroundRole);
            }
            else if (tId == KKSAttrType::atRecordTextColor || 
                     tId == KKSAttrType::atRecordTextColorRef)
            {
                item = new QStandardItem(tr("Text example"));
                bool ok;
                quint64 vl = av->value().valueForInsert().toLongLong(&ok);
                if (!ok)
                    continue;
                item->setData(QColor::fromRgba(vl), Qt::ForegroundRole);
            }
            else
                item = new QStandardItem(av->value().valueForInsert());
            item->setTextAlignment(Qt::AlignCenter);
            item->setData(idAv, Qt::UserRole);
            model->setItem(i,c_value,item);
        }


        item = new QStandardItem(av->startDateTime().toString("dd.MM.yyyy hh:mm:ss"));
        item->setTextAlignment(Qt::AlignCenter);
        item->setData(idAv, Qt::UserRole);
        model->setItem(i,c_start,item);

        item = new QStandardItem(av->stopDateTime().toString("dd.MM.yyyy hh:mm:ss"));
        item->setTextAlignment(Qt::AlignCenter);
        item->setData(idAv, Qt::UserRole);
        model->setItem(i,c_stop,item);


        if (av->ioSrc() != NULL)
        {
            item = new QStandardItem(av->ioSrc()->name());
            item->setTextAlignment(Qt::AlignCenter);
            item->setData(idAv, Qt::UserRole);
            model->setItem(i,c_source,item);
        }

        if (av->ioSrc1() != NULL)
        {
            item = new QStandardItem(av->ioSrc1()->name());
            item->setTextAlignment(Qt::AlignCenter);
            item->setData(idAv, Qt::UserRole);
            model->setItem(i,c_transfer,item);
        }

        //item = new QStandardItem(av->measDateTime().toString());
        //item->setTextAlignment(Qt::AlignCenter);
        //model->setItem(i,c_measured,item);


        if (av->desc() != NULL)
        {
            item = new QStandardItem(av->desc());
            item->setTextAlignment(Qt::AlignCenter);
            item->setData(idAv, Qt::UserRole);
            model->setItem(i,c_description,item);
        }

    }
*/
    QSortFilterProxyModel * attrHistModel = new KKSAttrHistoryProxyModel(this);
    attrHistModel->setSourceModel (model);
    QHeaderView * hv = UI->tvHistory->header();
    hv->setClickable(true);
    attrHistModel->setDynamicSortFilter(true);
    hv->setSortIndicator(0,Qt::AscendingOrder);
    hv->setSortIndicatorShown(true);
    attrHistModel->sort(0,Qt::AscendingOrder);

    UI->tvHistory->setModel(attrHistModel);
    UI->tvHistory->setSortingEnabled(true);


}

void AttrHistory::contextMenuEvent (QContextMenuEvent * event)
{
    QPoint p = event->pos();
    QPoint gp = event->globalPos();
    QWidget * tCh = childAt (p);
    //qDebug () << __PRETTY_FUNCTION__ << p << tCh << UI->tvHistory->viewport() ;
    if (tCh == UI->tvHistory->viewport())
    {
        pHistMenu->popup(gp);
        event->accept();
        return;
    }
    QWidget::contextMenuEvent(event);
}

void AttrHistory::viewVal (void)
{
    QItemSelectionModel * selModel = UI->tvHistory->selectionModel();
    QModelIndex wIndex = selModel->currentIndex();
    wIndex = wIndex.sibling(wIndex.row(), 0);
    viewDblVal (wIndex);
}

void AttrHistory::viewDblVal (const QModelIndex& wIndex)
{
    QModelIndex vInd = wIndex.sibling(wIndex.row(), 0);
    int idAttrVal = vInd.data(Qt::UserRole).toInt();
    emit viewAttrValue (idAttrVal, UI->tvHistory->viewport());
}

void AttrHistory::upClicked (void)
{
    QKeyEvent * kUpEvent = new QKeyEvent (QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
    (qobject_cast<QObject *>(UI->tvHistory))->event (kUpEvent);
    //QCoreApplication::sendEvent (UI->tvHistory, kUpEvent);
    //this->keyPressEvent(kUpEvent);
    delete kUpEvent;
}

void AttrHistory::downClicked (void)
{
    QKeyEvent * kDownEvent = new QKeyEvent (QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
    (qobject_cast<QObject *>(UI->tvHistory))->event (kDownEvent);
    //QCoreApplication::sendEvent (UI->tvHistory, kDownEvent);
    //this->keyPressEvent(kUpEvent);
    delete kDownEvent;
}