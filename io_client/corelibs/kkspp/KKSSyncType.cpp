/***********************************************************************
 * Module:  KKSSyncType.cpp
 * Author:  sergey
 * Modified: 1 ������� 2008 �. 12:09:41
 * Purpose: Implementation of the class KKSSyncType
 * Comment: ��� ������������� ��������������� �������
 ***********************************************************************/

#include "KKSSyncType.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSSyncType::KKSSyncType()
// Purpose:    Implementation of KKSSyncType::KKSSyncType()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSSyncType::KKSSyncType() : KKSRecord()
{
}

KKSSyncType::KKSSyncType(const KKSSyncType& s) : KKSRecord(s)
{

}

KKSSyncType::KKSSyncType(int id, const QString & name, const QString & desc) : KKSRecord(id, name, desc)
{

}

////////////////////////////////////////////////////////////////////////
// Name:       KKSSyncType::~KKSSyncType()
// Purpose:    Implementation of KKSSyncType::~KKSSyncType()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSSyncType::~KKSSyncType()
{
   // TODO : implement
}

KKSSyncType* KKSSyncType::defSyncType1()
{
    KKSSyncType* syncType = new KKSSyncType(1, QString::fromLocal8Bit("�� ����������������"));

    return syncType;
}
