#include "KKSAttrValue.h"
#include "KKSDateEdit.h"

KKSDateEdit :: KKSDateEdit (const KKSAttrValue *attr, KKSIndAttr::KKSIndAttrClass isSys, QWidget *parent)
    : QDateEdit (parent), KKSAttrWidget(attr, isSys)
{
    connect (this, SIGNAL (dateChanged (const QDate&)), this, SLOT (setDate (const QDate&)) );
}

KKSDateEdit :: KKSDateEdit (const KKSAttrValue *attr, KKSIndAttr::KKSIndAttrClass isSys, const QDate& date, QWidget *parent)
    : QDateEdit (parent), KKSAttrWidget(attr, isSys)
{
	
	if(date.isValid())
		QDateEdit::setDate(date);

    connect (this, SIGNAL (dateChanged (const QDate&)), this, SLOT (setDate (const QDate&)) );
}

KKSDateEdit :: ~KKSDateEdit (void)
{
}

void KKSDateEdit :: setDate (const QDate& d)
{
    QVariant v (d);
    emit valueChanged (m_av->id(), m_isSystem, v);
}
