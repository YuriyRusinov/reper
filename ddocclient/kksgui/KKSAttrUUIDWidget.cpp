#include <QGridLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QCheckBox>
#include <QRegExp>
#include <QRegExpValidator>
#include <QValidator>
#include <QtDebug>

#include "KKSAttrUUIDWidget.h"

KKSAttrUUIDWidget :: KKSAttrUUIDWidget (QWidget *parent, Qt::WindowFlags f)
    : QWidget (parent ,f),
    uuidWidget (new QLineEdit (this)),
    tbGen (new QToolButton(this)),
    chAuto (new QCheckBox (tr("Generate automatically"), this)),
    gAttrLay (new QGridLayout()),
    idAttrValue (-1),
    cVal (QVariant()),
#ifdef Q_CC_MSVC
    isSystem (KKSIndAttr::KKSIndAttrClass::iacIOUserAttr)
#else
    isSystem (KKSIndAttr::iacIOUserAttr)
#endif
{
    this->setLayout (gAttrLay);
    gAttrLay->addWidget (uuidWidget, 0, 0, 1, 1);
    gAttrLay->addWidget (tbGen, 0, 1, 1, 1);
    gAttrLay->addWidget (chAuto, 1, 0, 1, 2);
    gAttrLay->setContentsMargins (0, 0, 0, 0);
    gAttrLay->setAlignment (Qt::AlignRight | Qt::AlignVCenter);
    
    tbGen->setToolTip(tr("Generate automatically"));
    tbGen->setText ("...");
    QRegExp uuidRegExp ("[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}");
    QValidator * uuidVal = new QRegExpValidator (uuidRegExp, this);
    uuidWidget->setValidator (uuidVal);

    connect (uuidWidget, SIGNAL (editingFinished()), this, SLOT (setVal()) );
    connect (tbGen, SIGNAL (clicked()), this, SLOT (generate_uuid()) );
}

KKSAttrUUIDWidget :: ~KKSAttrUUIDWidget (void)
{
}

void KKSAttrUUIDWidget :: setVal (int id, KKSIndAttr::KKSIndAttrClass sys, QVariant val)
{
    idAttrValue = id;
    isSystem = sys;
    cVal = val;
    emit valueChanged (idAttrValue, sys, val);
}

int KKSAttrUUIDWidget :: getIdAttrValue (void) const
{
    return idAttrValue;
}

KKSIndAttr::KKSIndAttrClass KKSAttrUUIDWidget :: isSysAttr (void) const
{
    return isSystem;
}

const QVariant& KKSAttrUUIDWidget :: getValue (void) const
{
    return cVal;
}

void KKSAttrUUIDWidget :: setUUID (QString uuid)
{
    if (!uuidWidget)
        return;
    uuidWidget->setText (uuid);
    cVal = QVariant (uuid);
    emit valueChanged (idAttrValue, isSystem, cVal);
}

void KKSAttrUUIDWidget :: generate_uuid (void)
{
    emit generateUUID (idAttrValue);
}

QToolButton * KKSAttrUUIDWidget :: getButton (void) const
{
    return tbGen;
}

Qt::CheckState KKSAttrUUIDWidget :: checkState (void) const
{
    return chAuto->checkState ();
}