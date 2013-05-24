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
,m_isSystem(false)
{
}

KKSState::KKSState(const KKSState & s) : KKSRecord(s)
{
    m_isSystem = s.m_isSystem;
}

KKSState::KKSState(int id, const QString & name, const QString & desc, bool isKKSState) : KKSRecord(id, name, desc, QString::null, isKKSState)
, m_isSystem(false)
{

}

bool KKSState::isSystem() const
{
    return m_isSystem;
}

void KKSState::setIsSystem(bool yes)
{
    m_isSystem = yes;
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
    KKSState * state = new KKSState(1, QObject::tr("Active"), QString::null, true);
    state->m_isSystem = true;

    return state;
}

KKSState * KKSState::defState2()
{
    KKSState * state = new KKSState(2, QObject::tr("Archive"), QString::null, true);
    state->m_isSystem = true;

    return state;
}
