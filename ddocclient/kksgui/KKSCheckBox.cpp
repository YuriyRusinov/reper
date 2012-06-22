#include "KKSAttrValue.h"

#include "KKSCheckBox.h"

KKSCheckBox :: KKSCheckBox (const KKSAttrValue * attr, KKSIndAttr::KKSIndAttrClass isSys, QWidget *parent)
    : QCheckBox (parent), KKSAttrWidget(attr, isSys)
{

    setTristate(false);

    connect (this, SIGNAL (stateChanged(int)), this, SLOT (setState(int)) );
}

KKSCheckBox :: KKSCheckBox (const KKSAttrValue* attr, KKSIndAttr::KKSIndAttrClass isSys, const QString& text, QWidget *parent)
    : QCheckBox (text, parent), KKSAttrWidget(attr, isSys)
{
    setTristate(false);

    connect (this, SIGNAL (stateChanged(int)), this, SLOT (setState(int)) );
}

KKSCheckBox :: ~KKSCheckBox (void)
{
}

void KKSCheckBox :: setState (int state)
{
    QVariant v;
    if(state == Qt::Checked)
        v = QVariant(true);
    else
        v = QVariant(false);

    emit valueChanged (m_av->id(), m_isSystem, v);
}
