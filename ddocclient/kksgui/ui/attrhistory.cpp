#include <QDateTime>
#include <QDateTimeEdit>
#include <QMessageBox>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QItemSelectionRange>
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

#include "attrhistory.h"
#include "ui_attr_history.h"
#include "defines.h"
#include "KKSAttrHistoryProxyModel.h"
#include "KKSItemDelegate.h"
#include "KKSAttrHistModel.h"

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
    QItemSelectionModel * selModel = UI->tvHistory->selectionModel ();
    connect (selModel,
             SIGNAL (selectionChanged (const QItemSelection&, const QItemSelection&)),
             this,
             SLOT (histSelectionChanged (const QItemSelection&, const QItemSelection&))
            );
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
    QAbstractItemModel *model = new KKSAttrHistModel (histlist);//QStandardItemModel (n, c_description+1);
    //QStandardItem *item;

    //Получаем Имя Тип И Группу атрибута
    av = histlist.first();
    attr = av->attribute();
    qDebug()<< __PRETTY_FUNCTION__ << "Category ID "<<attr->id()<<" Category Name: "<<attr->name()<<" Category Group: "<<attr->group()->name()<<" Category Type: "<< attr->type()->name();

    UI->leGroup->setText(attr->group()->name());
    UI->leType->setText(attr->type()->name());
    UI->leName->setText(attr->name());

    QHeaderView *headerView = UI->tvHistory->header();
    headerView->setDefaultAlignment(Qt::AlignCenter);

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
    emit viewAttrValue (idAttrVal, this);
}

void AttrHistory::upClicked (void)
{
    QAbstractItemModel * histMod = UI->tvHistory->model();
    if (!histMod)
        return;
    QItemSelectionModel * selModel = UI->tvHistory->selectionModel();
    QItemSelection iSel = selModel->selection();
    if (iSel.isEmpty())
        return;
    QModelIndex topWIndex = iSel.indexes().at(0);
    int iRow = topWIndex.row();
    if (iRow==0)
        return;
    QItemSelectionRange selRange = iSel.at(0);
    QModelIndex wLeftIndex = histMod->index(iRow-1, 0, topWIndex.parent());
    QModelIndex wRightIndex = histMod->index(iRow-1, selRange.width()-1, topWIndex.parent());
    QItemSelection newSel (wLeftIndex, wRightIndex);
    //qDebug () << __PRETTY_FUNCTION__ << newSel << wLeftIndex << wRightIndex;
    selModel->select(newSel, QItemSelectionModel::ClearAndSelect);
}

void AttrHistory::downClicked (void)
{
    QAbstractItemModel * histMod = UI->tvHistory->model();
    if (!histMod)
        return;
    QItemSelectionModel * selModel = UI->tvHistory->selectionModel();
    QItemSelection iSel = selModel->selection();
    if (iSel.isEmpty())
        return;
    QModelIndex topWIndex = iSel.indexes().at(0);
    int iRow = topWIndex.row();
    if (iRow==histMod->rowCount(topWIndex.parent())-1)
        return;
    QItemSelectionRange selRange = iSel.at(0);
    QModelIndex wLeftIndex = histMod->index(iRow+1, 0, topWIndex.parent());
    QModelIndex wRightIndex = histMod->index(iRow+1, selRange.width()-1, topWIndex.parent());
    QItemSelection newSel (wLeftIndex, wRightIndex);
    //qDebug () << __PRETTY_FUNCTION__ << newSel << wLeftIndex << wRightIndex;
    selModel->select(newSel, QItemSelectionModel::ClearAndSelect);
}

void AttrHistory::histSelectionChanged (const QItemSelection& selected, const QItemSelection& deselected)
{
    qDebug () << __PRETTY_FUNCTION__ << selected << deselected;
}
