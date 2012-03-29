/***********************************************************************
 * Module:  KKSStateCross.cpp
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:09:49
 * Purpose: Implementation of the class KKSStateCross
 * Comment: переход между двумя состояниями
 ***********************************************************************/

#include "KKSState.h"
#include "KKSStateCross.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSStateCross::KKSStateCross()
// Purpose:    Implementation of KKSStateCross::KKSStateCross()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSStateCross::KKSStateCross()
{
   m_stateSrc = NULL;
   m_stateDst = NULL;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSStateCross::~KKSStateCross()
// Purpose:    Implementation of KKSStateCross::~KKSStateCross()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSStateCross::~KKSStateCross()
{
    if(m_stateSrc)
        m_stateSrc->release();

    if(m_stateDst)
        m_stateDst->release();
}

const KKSState * KKSStateCross::stateSrc() const
{
    return m_stateSrc;
}

const KKSState * KKSStateCross::stateDst() const
{
    return m_stateDst;
}

KKSState * KKSStateCross::stateSrc()
{
    return m_stateSrc;
}

KKSState * KKSStateCross::stateDst()
{
    return m_stateDst;
}

void KKSStateCross::setStates(KKSState * src, KKSState * dst)
{
    if(!src || !dst){
        qWarning("States cannot be NULL in KKSStateCross::setStates() method!");
        return;
    }

    if(m_stateSrc)
        m_stateSrc->release();

    if(m_stateDst)
        m_stateDst->release();
    
    m_stateSrc = src;
    m_stateDst = dst;

    if(m_stateSrc)
        m_stateSrc->addRef();

    if(m_stateDst)
        m_stateDst->addRef();
}
