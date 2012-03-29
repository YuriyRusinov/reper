/***********************************************************************
 * Module:  KKSState.cpp
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:09:41
 * Purpose: Implementation of the class KKSState
 * Comment: Состояние информационного объекта
 ***********************************************************************/

#include "KKSState.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSState::KKSState()
// Purpose:    Implementation of KKSState::KKSState()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSState::KKSState() : KKSRecord()
{
}

KKSState::KKSState(const KKSState & s) : KKSRecord(s)
{

}

KKSState::KKSState(int id, const QString & name, const QString & desc) : KKSRecord(id, name, desc)
{

}

////////////////////////////////////////////////////////////////////////
// Name:       KKSState::~KKSState()
// Purpose:    Implementation of KKSState::~KKSState()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSState::~KKSState()
{
   // TODO : implement
}

KKSState * KKSState::defState1()
{
    KKSState * state = new KKSState(1, QString::fromLocal8Bit("Активный"));

    return state;
}

KKSState * KKSState::defState2()
{
    KKSState * state = new KKSState(2, QString::fromLocal8Bit("Архивный"));

    return state;
}
