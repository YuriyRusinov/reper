#include <QHBoxLayout>
#include <QtDebug>

#include "KKSIntervalWidget.h"
#include "KKSAttrValue.h"

KKSIntervalWidget :: KKSIntervalWidget (const KKSAttrValue *attr, KKSIndAttr::KKSIndAttrClass isSys, QWidget *parent)
    : QWidget (parent), KKSAttrWidget(attr, isSys),
    lEValue (0),
    cbUnit (0)
{
    hBLay = new QHBoxLayout ();
    this->setLayout (hBLay);
    int x0(0), y0(0), x(0), y(0);
    hBLay->setContentsMargins (x0, y0, x, y);
}

KKSIntervalWidget :: ~KKSIntervalWidget (void)
{
}

void KKSIntervalWidget :: setLineEdit (QLineEdit *le)
{
    if (lEValue)
    {
        lEValue->setParent (0);
        delete lEValue;
        lEValue = 0;
    }

    if (le)
    {
        lEValue = le;
        hBLay->insertWidget (0, lEValue);//, 0, Qt::AlignLeft);
        lEValue->setParent (this);
        connect (lEValue, SIGNAL (editingFinished()), this, SLOT (setValue()) );
    }
}

void KKSIntervalWidget :: setComboUnits (QComboBox *cb)
{
    if (cbUnit)
    {
        cbUnit->setParent (0);
        delete cbUnit;
        cbUnit = 0;
    }

    if (cb)
    {
        cbUnit = cb;
        hBLay->insertWidget (1, cbUnit);//, 0, Qt::AlignLeft);
        cbUnit->setParent (this);
        connect (cbUnit, SIGNAL (activated(int)), this, SLOT (setUnit (int)) );
    }
}

void KKSIntervalWidget :: setValue (void)
{
    QString str = QString ("%1 %2").arg (lEValue->text ()).arg (cbUnit->currentText ());
    QVariant val (str);
    emit valueChanged(m_av->id(), m_isSystem, val);
}

void KKSIntervalWidget :: setUnit (int index)
{
    QString str = QString ("%1 %2").arg (lEValue->text ()).arg (cbUnit->itemText (index));
    QVariant val (str);
    emit valueChanged(m_av->id(), m_isSystem, val);
}

void KKSIntervalWidget :: setValue(const IntervalValue & v)
{
    
    int a;
    QString w;
    int iW;

    v.value(&a, w, &iW);
    lEValue->setText(QString::number(a));
    cbUnit->setCurrentIndex(cbUnit->findText(w));

    setValue();
}

IntervalValue KKSIntervalWidget :: value()
{
    IntervalValue v;
    v.setValue(lEValue->text().toInt(), cbUnit->currentText());

    return v;
}
