#include <QGridLayout>
#include <QtDebug>

#include "KKSAttrRefWidget.h"

KKSAttrRefWidget :: KKSAttrRefWidget (QWidget *parent, Qt::WindowFlags f)
    : QWidget (parent ,f),
    refWidget (0),
    gAttrLay (new QGridLayout()),
    idAttrValue (-1),
    cVal (QVariant()),
    isSystem (KKSIndAttr::KKSIndAttrClass::iacIOUserAttr)
{
    this->setLayout (gAttrLay);
    gAttrLay->setContentsMargins (0, 0, 0, 0);
    gAttrLay->setAlignment (Qt::AlignRight | Qt::AlignVCenter);
}

KKSAttrRefWidget :: ~KKSAttrRefWidget (void)
{
}

void KKSAttrRefWidget :: setAttrWidget (QWidget *aw)
{
    if (refWidget)
    {
        gAttrLay->removeWidget (refWidget);
        refWidget->setParent (0);
        delete refWidget;
    }

    refWidget = aw;
    if (refWidget)
    {
        refWidget->setParent (this);
        gAttrLay->addWidget (refWidget, 0, 0, 1, 1);
    }
}

QWidget * KKSAttrRefWidget :: getAttrWidget (void) const
{
    return refWidget;
}

QGridLayout * KKSAttrRefWidget :: getLayout (void) const
{
    return gAttrLay;
}

void KKSAttrRefWidget :: setValue (int id, KKSIndAttr::KKSIndAttrClass sys, QVariant val)
{
    idAttrValue = id;
    isSystem = sys;
    cVal = val;
    emit valueChanged (idAttrValue, sys, val);
}

int KKSAttrRefWidget :: getIdAttrValue (void) const
{
    return idAttrValue;
}

KKSIndAttr::KKSIndAttrClass KKSAttrRefWidget :: isSysAttr (void) const
{
    return isSystem;
}

const QVariant& KKSAttrRefWidget :: getValue (void) const
{
    return cVal;
}
