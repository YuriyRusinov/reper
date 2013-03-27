#include <QAbstractItemModel>
#include <QItemSelectionModel>
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
}

void SaveSearchTemplateForm :: setTypesModel (QAbstractItemModel * typeMod)
{
    QAbstractItemModel * oldMod = sortTModel->sourceModel();//UI->tvSearchTemplateType->model ();
    sortTModel->setSourceModel (typeMod);
    if (oldMod && oldMod != typeMod)
        delete oldMod;
    if (searchTemplate && searchTemplate->type())
    {
        QModelIndex sttInd = getCurrentType (searchTemplate->type()->id());
        qDebug () << __PRETTY_FUNCTION__ << searchTemplate->type()->id() << searchTemplate->type()->name() << sttInd;
        if (sttInd.isValid())
        {
            QItemSelectionModel * selModel = UI->tvSearchTemplateType->selectionModel ();
            QModelIndex sIndex = sortTModel->mapFromSource (sttInd);
            selModel->select (sIndex, QItemSelectionModel::ClearAndSelect);
        }
    }
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
        QMessageBox::warning (this, tr ("Search template"), tr ("Set search template name"), QMessageBox::Ok);
        return;
    }
    searchTemplate->setName (UI->lEName->text());
    QDialog::accept();
}

QModelIndex SaveSearchTemplateForm :: getCurrentType (int idType, const QModelIndex& pIndex) const
{
    if (!sortTModel->sourceModel())
        return QModelIndex();
    QAbstractItemModel * sTypeMod = sortTModel->sourceModel();//UI->tvSearchTemplateType->model();
    int nr = sTypeMod->rowCount (pIndex);
    for (int i=0; i<nr; i++)
    {
        QModelIndex wIndex = sTypeMod->index (i, 0, pIndex);
        if (wIndex.isValid() && idType == wIndex.data (Qt::UserRole).toInt())
            return wIndex;
        else if (sTypeMod->rowCount (wIndex) > 0)
        {
            QModelIndex chIndex;
            chIndex = getCurrentType (idType, wIndex);
            if (chIndex.isValid())
                return chIndex;
        }
        else
            continue;
    }
    return QModelIndex ();
}
