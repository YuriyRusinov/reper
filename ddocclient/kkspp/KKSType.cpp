/***********************************************************************
 * Module:  KKSType.cpp
 * Author:  sergey
 * Modified: 25 но€бр€ 2008 г. 17:46:06
 * Purpose: Implementation of the class KKSType
 ***********************************************************************/

#include "KKSType.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSType::KKSType()
// Purpose:    Implementation of KKSType::KKSType()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSType::KKSType() : KKSRecord()
{
}

KKSType::KKSType(const KKSType & t) : KKSRecord(t)
{

}

KKSType::KKSType(int id, const QString & name, const QString & desc) : KKSRecord(id, name, desc)
{
    
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSType::~KKSType()
// Purpose:    Implementation of KKSType::~KKSType()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSType::~KKSType()
{
   // TODO : implement
}

KKSType * KKSType::defType1()
{
    KKSType * type = new KKSType(1, QString::fromLocal8Bit("ќбщие документы"));

    return type;
}
