#include <QCheckBox>
#include <QComboBox>
#include <QtDebug>

#include <KKSIndicator.h>

#include "indexform.h"
#include "ui_index_form.h"

IndexForm :: IndexForm (KKSIndicator * ind, const KKSMap<int, KKSIndicator*>& existIndices, QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
    UI (new Ui::index_form),
    Index (ind),
    eIndices (existIndices)
{
    UI->setupUi (this);
    this->initForm ();

    connect (UI->chManual, SIGNAL (stateChanged(int)), this, SLOT (manualStateChanged (int)) );
    connect (UI->cbIndexParent, SIGNAL (activated(int)), this, SLOT (parentChanged(int)) );
    connect (UI->pbOk, SIGNAL (clicked()), this, SLOT (apply()) );
    connect (UI->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

IndexForm :: ~IndexForm (void)
{
    delete UI;
}

KKSIndicator * IndexForm :: getIndex (void) const
{
    return Index;
}

void IndexForm :: parentChanged (int index)
{
    qDebug () << __PRETTY_FUNCTION__ << index;
    if (index)
        Index->setParent(eIndices.value (UI->cbIndexParent->itemData (index).toInt()));
    else
        Index->setParent (0);
}

void IndexForm :: manualStateChanged (int state)
{
    bool isManual (state == Qt::Checked);
    UI->lETableName->setEnabled (isManual);
}

void IndexForm :: apply (void)
{
    /*if (UI->chManual->checkState () == Qt::Checked)
        Index->setTableName (UI->lETableName->text());
    else
        Index->setTableName (QString());
    Index->setName (UI->lEIndexName->text());*/
}

void IndexForm :: initForm (void)
{
    this->manualStateChanged (Qt::Unchecked);
    int i=1;
    UI->cbIndexParent->addItem (QString(), QVariant(-1));
    for (KKSMap<int, KKSIndicator*>::const_iterator p = eIndices.constBegin();
            p != eIndices.constEnd();
            ++p)
    {
        if (p.value())
        {
            UI->cbIndexParent->addItem (p.value()->name(), p.key());
            i++;
        }
    }
}
