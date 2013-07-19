#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QAbstractItemDelegate>
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QMessageBox>
#include <QtDebug>

#include <KKSSearchTemplate.h>
#include <KKSSearchTemplateType.h>
#include <KKSEventFilter.h>
#include <KKSItemDelegate.h>
#include "KKSSearchTemplateFilterProxyModel.h"
#include <defines.h>
#include "savesearchtemplateform.h"
#include "ui_save_search_template_form.h"

SaveSearchTemplateForm :: SaveSearchTemplateForm (KKSSearchTemplate * st, bool mode, QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
    UI (new Ui::save_search_template_form),
    searchTemplate (st),
    sortTModel (new KKSSearchTemplateFilterProxyModel (this))
{
    UI->setupUi (this);
    if (searchTemplate)
    {
        if (mode)
            searchTemplate->addRef ();
        UI->lEName->setText (st->name());
    }
    
    QAbstractItemDelegate * sttDeleg = new KKSItemDelegate (this);
    UI->tvSearchTemplateType->setItemDelegate (sttDeleg);
    UI->tvSearchTemplateType->setModel (sortTModel);
    KKSEventFilter * sttEf = new KKSEventFilter (this);
    UI->tvSearchTemplateType->viewport()->installEventFilter (sttEf);
    UI->tvSearchTemplateType->setSortingEnabled (true);
    QHeaderView * hv = UI->tvSearchTemplateType->header();
    hv->setClickable (true);
    hv->setSortIndicatorShown (true);
    hv->setStretchLastSection (true);
    sortTModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    sortTModel->sort(0);

    QAbstractItemDelegate * catDeleg = new KKSItemDelegate (this);
    UI->tvCategory->setItemDelegate (catDeleg);
    KKSEventFilter * catEf = new KKSEventFilter (this);
    UI->tvCategory->viewport()->installEventFilter (catEf);
    UI->tvCategory->setSelectionBehavior(QAbstractItemView::SelectItems);
    
    UI->pbOk->setVisible (mode);
    UI->pbCancel->setVisible (mode);

    connect (UI->pbOk, SIGNAL (clicked()), this, SLOT (staccept()) );
    connect (UI->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

SaveSearchTemplateForm :: ~SaveSearchTemplateForm (void)
{
    delete UI;
    if (searchTemplate && UI->pbOk->isVisible())
        searchTemplate->release ();
}

void SaveSearchTemplateForm :: setCategoryModel (QAbstractItemModel * catMod)
{
    QAbstractItemModel * oldMod = UI->tvCategory->model ();
    UI->tvCategory->setModel (catMod);
    if (oldMod && oldMod != catMod)
        delete oldMod;
    QItemSelectionModel * selCModel = UI->tvCategory->selectionModel();
    connect (selCModel, 
             SIGNAL (selectionChanged (const QItemSelection&, const QItemSelection&)),
             this,
             SLOT (selCatChanged (const QItemSelection&, const QItemSelection&))
            );
}

void SaveSearchTemplateForm :: setTypesModel (QAbstractItemModel * typeMod)
{
    QAbstractItemModel * oldMod = sortTModel->sourceModel();//UI->tvSearchTemplateType->model ();
    sortTModel->setSourceModel (typeMod);
    if (oldMod && oldMod != typeMod)
        delete oldMod;
}

void SaveSearchTemplateForm :: selectType (const QModelIndex& tInd) const
{
    QItemSelectionModel * selModel = UI->tvSearchTemplateType->selectionModel ();
    QModelIndex sIndex = sortTModel->mapFromSource (tInd);
    selModel->select (sIndex, QItemSelectionModel::ClearAndSelect);
    selModel->setCurrentIndex (sIndex, QItemSelectionModel::Select);
    for (;sIndex.parent().isValid(); sIndex=sIndex.parent())
        UI->tvSearchTemplateType->expand(sIndex.parent());
}

void SaveSearchTemplateForm :: selectCategory (const QModelIndex& cInd) const
{
    QItemSelectionModel *cSelModel = UI->tvCategory->selectionModel ();
    cSelModel->select (cInd, QItemSelectionModel::ClearAndSelect);
    cSelModel->setCurrentIndex (cInd, QItemSelectionModel::Select);
    QModelIndex catInd(cInd);
    for (;catInd.parent().isValid(); catInd=catInd.parent())
        UI->tvCategory->expand (catInd.parent());
}

int SaveSearchTemplateForm :: getIdCat (void) const
{
    if (!UI->tvCategory->isEnabled())
        return -1;

    QItemSelectionModel * selMod = UI->tvCategory->selectionModel ();
    QModelIndex wIndex = selMod->currentIndex ();
    wIndex = wIndex.sibling (wIndex.row(), 0);
    if (!wIndex.isValid() || wIndex.data (Qt::UserRole+USER_ENTITY).toInt() != 1)
        return -1;
    return wIndex.data (Qt::UserRole).toInt();
}

int SaveSearchTemplateForm :: getIdType (void) const
{
    QItemSelectionModel * selMod = UI->tvSearchTemplateType->selectionModel ();
    QModelIndex wIndex = selMod->currentIndex ();
    wIndex = wIndex.sibling (wIndex.row(), 0);
    QModelIndex wsIndex = sortTModel->mapToSource (wIndex);
    return wsIndex.isValid() ? wsIndex.data (Qt::UserRole).toInt() : -1;
}

QString SaveSearchTemplateForm :: getName (void) const
{
    return UI->lEName->text();
}

void SaveSearchTemplateForm :: init (void)
{
    if (!searchTemplate)
        return;
    UI->lEName->setText (searchTemplate->name());
}

KKSSearchTemplate * SaveSearchTemplateForm :: getSearchTemplate (void) const
{
    return searchTemplate;
}

void SaveSearchTemplateForm :: staccept (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (UI->lEName->text().isEmpty())
    {
        qWarning() << tr ("Set search template name");
        QMessageBox::warning (this, tr ("Search template"), tr ("Set search template name"), QMessageBox::Ok);
        return;
    }
    searchTemplate->setName (UI->lEName->text());
    QDialog::accept();
}

void SaveSearchTemplateForm :: catChanged (const QModelIndex& current, const QModelIndex& previous)
{
    QItemSelectionModel * sMod = qobject_cast<QItemSelectionModel*>(sender());
    qDebug () << __PRETTY_FUNCTION__ << current << previous;
    if (current.isValid() && current.data(Qt::UserRole+USER_ENTITY).toInt() != 1)
    {
        qWarning() << tr("You have to select category not type");
        QMessageBox::warning(this, tr("Select category"), tr("You have to select category not type"), QMessageBox::Ok);
        if (previous.isValid())
        {
            sMod->setCurrentIndex (previous, QItemSelectionModel::Select);
            sMod->select(previous, QItemSelectionModel::Select);
        }
        else
        {
            sMod->setCurrentIndex (previous, QItemSelectionModel::Clear);
            sMod->reset();
        }
        return;
    }
    else if (current.isValid() && previous.isValid() && current.data(Qt::UserRole+USER_ENTITY).toInt() == 1)
    {
        emit categoryChanged (this->searchTemplate, current.data(Qt::UserRole).toInt());
    }
    else if (!current.isValid() && previous.isValid() && previous.data(Qt::UserRole+USER_ENTITY) == 1)
    {
        qWarning() << tr("You have to select category not type");
        QMessageBox::warning(this, tr("Select category"), tr("You have to select category not type"), QMessageBox::Ok);
        sMod->select(previous, QItemSelectionModel::ClearAndSelect);
        sMod->setCurrentIndex (previous, QItemSelectionModel::ClearAndSelect);
        return;
    }
    else
        sMod->reset();
}

void SaveSearchTemplateForm :: selCatChanged (const QItemSelection& selected, const QItemSelection& deselected)
{
    qDebug () << __PRETTY_FUNCTION__ << selected << deselected;
    QItemSelectionModel * sMod = qobject_cast<QItemSelectionModel*>(sender());
    if (selected.indexes().isEmpty())
        return;
    QModelIndex current = selected.indexes().at(0);
    QModelIndex previous = deselected.isEmpty() || deselected.indexes().isEmpty() ? QModelIndex() : deselected.indexes().at(0);
    if (current.isValid() && current.data(Qt::UserRole+USER_ENTITY).toInt() != 1 ||
        !current.isValid() && previous.isValid() && previous.data(Qt::UserRole+USER_ENTITY) == 1
       )
    {
        qWarning() << tr("You have to select category not type");
        QMessageBox::warning(this, tr("Select category"), tr("You have to select category not type"), QMessageBox::Ok);
        return;
    }
    else if (current.isValid() && current.data(Qt::UserRole+USER_ENTITY).toInt() == 1)
    {
        emit categoryChanged (this->searchTemplate, current.data(Qt::UserRole).toInt());
    }
    else
        sMod->reset();
}
