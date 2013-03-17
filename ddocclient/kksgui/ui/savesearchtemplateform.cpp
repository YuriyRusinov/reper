#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QtDebug>

#include <KKSSearchTemplate.h>
#include <KKSSearchTemplateType.h>
#include <defines.h>
#include "savesearchtemplateform.h"
#include "ui_save_search_template_form.h"

SaveSearchTemplateForm :: SaveSearchTemplateForm (KKSSearchTemplate * st,QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
    UI (new Ui::save_search_template_form),
    searchTemplate (st)
{
    UI->setupUi (this);
    if (searchTemplate)
        searchTemplate->addRef ();

    catChStateChanged (Qt::Unchecked);
    connect (UI->chCategory, SIGNAL (stateChanged (int)), this, SLOT (catChStateChanged (int)) );

    connect (UI->pbOk, SIGNAL (clicked()), this, SLOT (staccept()) );
    connect (UI->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

SaveSearchTemplateForm :: ~SaveSearchTemplateForm (void)
{
    delete UI;
    if (searchTemplate)
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
    QAbstractItemModel * oldMod = UI->tvSearchTemplateType->model ();
    UI->tvSearchTemplateType->setModel (typeMod);
    if (oldMod && oldMod != typeMod)
        delete oldMod;
    if (searchTemplate && searchTemplate->type())
    {
        QModelIndex sttInd = getCurrentType (searchTemplate->type()->id());
        if (sttInd.isValid())
        {
            QItemSelectionModel * selModel = UI->tvSearchTemplateType->selectionModel ();
            selModel->select (sttInd, QItemSelectionModel::ClearAndSelect);
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
    return wIndex.isValid() ? wIndex.data (Qt::UserRole).toInt() : -1;
}

void SaveSearchTemplateForm :: catChStateChanged (int state)
{
    bool isEn (state == Qt::Checked);
    UI->tvCategory->setEnabled (isEn);
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
    searchTemplate->setName (UI->lEName->text());
    QDialog::accept();
}

const QModelIndex& SaveSearchTemplateForm :: getCurrentType (int idType, const QModelIndex& pIndex) const
{
    if (!UI->tvSearchTemplateType->model())
        return QModelIndex();
    QAbstractItemModel * sTypeMod = UI->tvSearchTemplateType->model();
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
