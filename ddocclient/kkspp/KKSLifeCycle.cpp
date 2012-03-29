/***********************************************************************
 * Module:  KKSLifeCycle.cpp
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:10:03
 * Purpose: Implementation of the class KKSLifeCycle
 * Comment: Жизненный цикл информационного объекта.
 *    Входит в состав категории.
 ***********************************************************************/
#include "KKSStateCross.h"
#include "KKSLifeCycle.h"
#include "KKSState.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSLifeCycle::KKSLifeCycle()
// Purpose:    Implementation of KKSLifeCycle::KKSLifeCycle()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSLifeCycle::KKSLifeCycle()
{
}

KKSLifeCycle::KKSLifeCycle(const KKSLifeCycle & lc)
{
    m_stateCrosses = lc.stateCrosses();
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSLifeCycle::~KKSLifeCycle()
// Purpose:    Implementation of KKSLifeCycle::~KKSLifeCycle()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSLifeCycle::~KKSLifeCycle()
{
}

const KKSList<KKSStateCross *> & KKSLifeCycle::stateCrosses() const
{
    return m_stateCrosses;
}

void KKSLifeCycle::setStateCrosses(const KKSList<KKSStateCross *> & sc)
{
    m_stateCrosses = sc;
}

void KKSLifeCycle::addStateCross(KKSStateCross * s)
{
    if(!s)
        return;

    m_stateCrosses.append(s);
}

void KKSLifeCycle::addStateCross(KKSState * s1, KKSState * s2)
{
    if(!s1 || !s2)
        return;

    KKSStateCross * sc = new KKSStateCross();
    sc->setStates(s1, s2);
    
    addStateCross(sc);
    
    sc->release();
}

void KKSLifeCycle::removeStateCross(KKSStateCross * s)
{
    if(!s)
        return;
    
    m_stateCrosses.removeAll(s);
}

void KKSLifeCycle::removeStateCross(int index)
{
    m_stateCrosses.removeAt(index);
}

KKSStateCross * KKSLifeCycle::stateCross(int index)
{
    KKSStateCross * sc = NULL;
    if(index >= m_stateCrosses.count())
        return sc;

    sc = m_stateCrosses[index];
    return sc;
}

const KKSStateCross * KKSLifeCycle::stateCross(int index) const
{
    const KKSStateCross * sc = NULL;
    if(index >= m_stateCrosses.count())
        return sc;

    sc = m_stateCrosses.at(index);
    return sc;
}

KKSLifeCycle * KKSLifeCycle::defLifeCycle()
{
    KKSLifeCycle * lc = new KKSLifeCycle();

    KKSStateCross * sc = new KKSStateCross();
    
    KKSState * src = KKSState::defState1();
    KKSState * dst = KKSState::defState2();
    
    sc->setStates(src, dst);
    lc->addStateCross(sc);
    lc->addStateCross(dst, src);

    src->release();
    dst->release();
    sc->release();
    
    return lc;
}
