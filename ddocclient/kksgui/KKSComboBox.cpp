/***********************************************************************
 * Module:  KKSComboBox.cpp
 * Author:  sergey
 * Modified: 1 ������� 2008 �. 12:13:21
 * Purpose: Implementation of the class KKSComboBox
 ***********************************************************************/

#include "KKSComboBox.h"
#include "KKSAttrValue.h"
#include "defines.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSComboBox::loadRecords()
// Purpose:    Implementation of KKSComboBox::loadRecords()
// Comment:    ��� �������� ���� "������ �������� �� �����������" ���������� ��������� �� �� ������ �� �������� ������� � �������. �� ��� ��������� � �������, ������ ������������ � ����������, �� ��� ���� ������ ��������� ������������� ������ ������.
// Return:     int
////////////////////////////////////////////////////////////////////////
/*
int KKSComboBox::loadRecords(void)
{
    KKSComboBox * b = new KKSComboBox(0);
    b->show();
    delete b;

    return OK_CODE;
   // TODO : implement
}
*/
////////////////////////////////////////////////////////////////////////
// Name:       KKSComboBox::KKSComboBox()
// Purpose:    Implementation of KKSComboBox::KKSComboBox()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSComboBox::KKSComboBox (const KKSAttrValue* attr, KKSIndAttrClass isSys, QWidget *parent)
    : QComboBox (parent), KKSAttrWidget(attr, isSys)
{
    connect (this, SIGNAL (currentIndexChanged(int)), this, SLOT (setCurrentData(int)) );
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSComboBox::~KKSComboBox()
// Purpose:    Implementation of KKSComboBox::~KKSComboBox()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSComboBox::~KKSComboBox()
{
}

/*void KKSComboBox::addItem(const QString & v, int itemData)
{
    QComboBox::addItem(v);
    itemsData.append(itemData);
}
*/

void KKSComboBox :: setCurrentData (int index)
{
    QVariant val = this->itemData (index);
    emit valueChanged (m_av->id(), m_isSystem, val);
}
