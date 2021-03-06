#include <QGridLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QCheckBox>
#include <QRegExp>
#include <QRegExpValidator>
#include <QValidator>
#include <QtDebug>

#include "KKSAttrUUIDWidget.h"

KKSAttrUUIDWidget :: KKSAttrUUIDWidget (bool isRef, QWidget *parent, Qt::WindowFlags f)
    : QWidget (parent ,f),
    uuidWidget (new QLineEdit (this)),
    gAttrLay (new QGridLayout()),
    idAttrValue (-1),
    cVal (QVariant()),
    isSystem (iacIOUserAttr),
    m_isRef(isRef)
{
    this->setLayout (gAttrLay);
    gAttrLay->addWidget (uuidWidget, 0, 0, 1, 1);
    
    tbGen = NULL;
    chAuto = NULL;
    
    if(!isRef){
        tbGen = new QToolButton(this);
        chAuto = new QCheckBox (tr("Generate automatically"), this);

        gAttrLay->addWidget (tbGen, 0, 1, 1, 1);
        gAttrLay->addWidget (chAuto, 1, 0, 1, 2);
    }
    else{
        uuidWidget->setReadOnly(true);
    }

    gAttrLay->setContentsMargins (0, 0, 0, 0);
    gAttrLay->setAlignment (Qt::AlignRight | Qt::AlignVCenter);
    
    if(chAuto)
        chAuto->setChecked(true);
    if(tbGen){
        tbGen->setToolTip(tr("Generate automatically"));
        tbGen->setText ("...");
    }

    QRegExp uuidRegExp ("[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}");
    QValidator * uuidVal = new QRegExpValidator (uuidRegExp, this);
    uuidWidget->setValidator (uuidVal);

    connect (uuidWidget, SIGNAL (editingFinished()), this, SLOT (setVal()) );
    connect (uuidWidget, SIGNAL (textChanged(const QString &)), this, SLOT (setVal(const QString &)) );
    if(tbGen)
        connect (tbGen, SIGNAL (clicked()), this, SLOT (generate_uuid()) );
}

KKSAttrUUIDWidget :: ~KKSAttrUUIDWidget (void)
{
}

void KKSAttrUUIDWidget :: setVal()
{
    if (!uuidWidget)
        return;
    
    QString uuid = uuidWidget->text();
    
    cVal = QVariant (uuid);
    emit valueChanged (idAttrValue, isSystem, cVal);
}

void KKSAttrUUIDWidget :: setVal(const QString & t)
{
    if (!uuidWidget)
        return;

    if(!t.isEmpty())
        return;
    
    QString uuid = uuidWidget->text();
    
    cVal = QVariant (uuid);
    emit valueChanged (idAttrValue, isSystem, cVal);
}

void KKSAttrUUIDWidget :: setVal (qint64 id, KKSIndAttrClass sys, QVariant val)
{
    idAttrValue = id;
    isSystem = sys;
    
    uuidWidget->setText (val.toString());
    
    cVal = val;
    emit valueChanged (idAttrValue, sys, val);
}

qint64 KKSAttrUUIDWidget :: getIdAttrValue (void) const
{
    return idAttrValue;
}

KKSIndAttrClass KKSAttrUUIDWidget :: isSysAttr (void) const
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
    if(!chAuto)
        return Qt::Checked;

    return chAuto->checkState ();
}