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
#include "KKSAttribute.h"
#include "kkscatattreditor.h"
#include "ui_kkscatattr_editor.h"

KKSCatAttrEditor :: KKSCatAttrEditor (KKSCategoryAttr *attr, const KKSMap<int, KKSAttrType *>& aTypes, bool for_template, QWidget *parent)
    : QDialog (parent),
    attribute (attr),
    attrTypes (aTypes),
    ui (new Ui::kkscatattr_editor)
{
    ui->setupUi (this);

    if (attribute && attribute->id() >= 0)
    {
        //
        // Существующий атрибут
        //
        qDebug () << __PRETTY_FUNCTION__ << attribute->id ();
        ui->lETitle->setText (attribute->title());
        ui->lETitle->setReadOnly (true);
        ui->chMandatory->setCheckState ((attribute->isMandatory() ? Qt::Checked : Qt::Unchecked));
        ui->chReadOnly->setCheckState ((attribute->isReadOnly() ? Qt::Checked : Qt::Unchecked));
        ui->cbType->setEnabled (false);
        ui->lEName->setText (attribute->name());
        ui->lECode->setText (attribute->code());
        ui->lECode->setReadOnly (true);
        ui->lEName->setReadOnly (true);
        ui->lEDefVal->setText (attr->defValue().value());
        ui->sbOrder->setValue(attr->order());
        ui->teDirectives->setText(attr->directives());
    }

    this->init_widgets (for_template);

    connect (ui->cbType, SIGNAL (activated(int)), this, SLOT (currentAttrTypeChanged(int)) );

    ui->pbOK->setDefault (true);
    ui->pbOK->setAutoDefault (true);
    QKeySequence enter_key (Qt::Key_Enter);
    ui->pbOK->setShortcut (enter_key);
    QKeySequence cancel_key (Qt::Key_Escape);
    ui->pbCancel->setShortcut (cancel_key);

    connect (ui->pbOK, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (ui->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

KKSCatAttrEditor :: ~KKSCatAttrEditor (void)
{
}

void KKSCatAttrEditor :: accept ()
{
    QDialog::accept ();
    if (!attribute)
        return;

    attribute->setMandatory ((ui->chMandatory->checkState()==Qt::Checked));
    attribute->setReadOnly ((ui->chReadOnly->checkState()==Qt::Checked));
    KKSValue val (ui->lEDefVal->text(), attribute->type()->attrType());
    attribute->setDefValue (val);
    attribute->setOrder(ui->sbOrder->value());
    attribute->setDirectives(ui->teDirectives->toPlainText());
//    attribute->setDesc (lEDesc->text());
}

void KKSCatAttrEditor :: uploadAttributes ()
{
}

void KKSCatAttrEditor :: currentAttrTypeChanged (int index)
{
    qDebug () << __PRETTY_FUNCTION__ << index;
}

void KKSCatAttrEditor :: init_widgets (bool for_template)
{
    ui->chMandatory->setEnabled (!for_template);
    ui->chReadOnly->setEnabled (!for_template);
    ui->tabDirectives->setEnabled(!for_template);
    //ui->sbOrder
    this->set_types ();
}

KKSCategoryAttr * KKSCatAttrEditor :: getCatAttribute (void)
{
    return attribute;
}

const KKSCategoryAttr * KKSCatAttrEditor :: getCatAttribute (void) const
{
    return attribute;
}

void KKSCatAttrEditor :: set_types (void)
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

    for (p = attrTypes.constBegin(); p != attrTypes.constEnd(); p++)
    {
        ui->cbType->addItem (p.value()->name (), p.key());
        if (p.key() == cType && ind < 0)
            ind = i;
        i++;
    }
    ui->cbType->setCurrentIndex (ind);
}

int KKSCatAttrEditor :: getTypeID (void) const
{
    return ui->cbType->itemData (ui->cbType->currentIndex ()).toInt();
}
