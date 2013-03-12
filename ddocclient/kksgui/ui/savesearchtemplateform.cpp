#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QtDebug>

#include <KKSSearchTemplate.h>
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

    connect (UI->pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
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
}

int SaveSearchTemplateForm :: getIdCat (void) const
{
    if (!UI->tvCategory->isEnabled())
        return -1;

    QItemSelectionModel * selMod = UI->tvCategory->selectionModel ();
    QModelIndex wIndex = selMod->currentIndex ();
    wIndex = wIndex.sibling (wIndex.row(), 0);
    return wIndex.isValid() ? wIndex.data (Qt::UserRole).toInt() : -1;
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