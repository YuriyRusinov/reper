#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QtDebug>

#include "savesearchtemplateform.h"
#include "ui_save_search_template_form.h"

SaveSearchTemplateForm :: SaveSearchTemplateForm (QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
    UI (new Ui::save_search_template_form)
{
    UI->setupUi (this);

    catChStateChanged (Qt::Unchecked);
    connect (UI->chCategory, SIGNAL (stateChanged (int)), this, SLOT (catChStateChanged (int)) );

    connect (UI->pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (UI->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

SaveSearchTemplateForm :: ~SaveSearchTemplateForm (void)
{
    delete UI;
}

void SaveSearchTemplateForm :: setCategoryModel (QAbstractItemModel * catMod)
{
    QAbstractItemModel * oldMod = UI->tvCategory->model ();
    UI->tvCategory->setModel (catMod);
    if (oldMod && oldMod != catMod)
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

void SaveSearchTemplateForm :: catChStateChanged (int state)
{
    bool isEn (state == Qt::Checked);
    UI->tvCategory->setEnabled (isEn);
}

QString SaveSearchTemplateForm :: getName (void) const
{
    return UI->lEName->text();
}
