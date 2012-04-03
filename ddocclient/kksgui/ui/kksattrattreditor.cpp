#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QSpacerItem>
#include <QtDebug>

#include <KKSValue.h>
#include <KKSAttrType.h>
#include "KKSAttrAttr.h"
#include "kksattrattreditor.h"
#include "ui_kksattrattr_editor.h"

KKSAttrAttrEditor :: KKSAttrAttrEditor (KKSAttrAttr *attr, bool for_template, QWidget *parent)
    : QDialog (parent),
    attribute (attr),
    ui (new Ui::kksattrattr_editor)
{
    ui->setupUi (this);

    if (attribute && attribute->id() >= 0)
    {
        //
        // Существующий атрибут
        //
        ui->lETitle->setText (attribute->title());
        ui->lETitle->setReadOnly (true);
        ui->chMandatory->setCheckState ((attribute->isMandatory() ? Qt::Checked : Qt::Unchecked));
        ui->chReadOnly->setCheckState ((attribute->isReadOnly() ? Qt::Checked : Qt::Unchecked));
        ui->lEName->setText (attribute->name());
        ui->lECode->setText (attribute->code());
        ui->lECode->setReadOnly (true);
        ui->lEName->setReadOnly (true);
        ui->lEDefVal->setText (attr->defValue().value());
    }

    ui->cbType->setEnabled (false);
    this->init_widgets (for_template);

    ui->pbOK->setDefault (true);
    ui->pbOK->setAutoDefault (true);
    QKeySequence enter_key (Qt::Key_Enter);
    ui->pbOK->setShortcut (enter_key);
    QKeySequence cancel_key (Qt::Key_Escape);
    ui->pbCancel->setShortcut (cancel_key);

    connect (ui->pbOK, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (ui->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

KKSAttrAttrEditor :: ~KKSAttrAttrEditor (void)
{
}

void KKSAttrAttrEditor :: accept ()
{
    if (!attribute)
        return;

    attribute->setMandatory ((ui->chMandatory->checkState()==Qt::Checked));
    attribute->setReadOnly ((ui->chReadOnly->checkState()==Qt::Checked));
    KKSValue val (ui->lEDefVal->text(), attribute->type()->attrType());
    attribute->setDefValue (val);

    QDialog::accept ();
}

void KKSAttrAttrEditor :: init_widgets (bool for_template)
{
    ui->chMandatory->setEnabled (!for_template);
    ui->chReadOnly->setEnabled (!for_template);
    this->set_types ();
}

KKSAttrAttr * KKSAttrAttrEditor :: getAttrAttribute (void)
{
    return attribute;
}

const KKSAttrAttr * KKSAttrAttrEditor :: getAttrAttribute (void) const
{
    return attribute;
}

void KKSAttrAttrEditor :: set_types (void)
{
    ui->cbType->clear ();
    int ind=-1;
    KKSMap<int, KKSAttrType *>::const_iterator p;
    int i=0;
    int cType = ( attribute && attribute->id() >= 0 ? attribute->type ()->attrType() : KKSAttrType::atString);

    if (cType == KKSAttrType::atInt)
    {
        QIntValidator *iVal = new QIntValidator (this);
        ui->lEDefVal->setValidator (iVal);
    }
    else if (cType == KKSAttrType::atDouble)
    {
        QDoubleValidator *dVal = new QDoubleValidator (this);
        ui->lEDefVal->setValidator (dVal);
    }

    ui->cbType->addItem (attribute->type()->name(), cType);
    ui->cbType->setCurrentIndex (0);
}

int KKSAttrAttrEditor :: getTypeID (void) const
{
    return ui->cbType->itemData (ui->cbType->currentIndex ()).toInt();
}
