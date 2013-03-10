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
#include "defines.h"
#include <QObject>

////////////////////////////////////////////////////////////////////////
// Name:       KKSLifeCycle::KKSLifeCycle()
// Purpose:    Implementation of KKSLifeCycle::KKSLifeCycle()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSLifeCycleEx::KKSLifeCycleEx(int id, const QString & name, const QString & desc)
:KKSRecord(id, name, desc), m_startState(NULL)
{
}

KKSLifeCycleEx::KKSLifeCycleEx(const KKSLifeCycleEx & lc) : KKSRecord(lc), m_startState(NULL)
{
    m_stateCrosses = lc.stateCrosses();

    setStartState(const_cast<KKSState * > (lc.startState()));
    setStates(lc.states());
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSLifeCycle::~KKSLifeCycle()
// Purpose:    Implementation of KKSLifeCycle::~KKSLifeCycle()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSLifeCycleEx::~KKSLifeCycleEx()
{
    if(m_startState)
        m_startState->release();
}

const KKSList<KKSStateCross *> & KKSLifeCycleEx::stateCrosses() const
{
    return m_stateCrosses;
}

void KKSLifeCycleEx::setStateCrosses(const KKSList<KKSStateCross *> & sc)
{
    m_stateCrosses = sc;
}

void KKSLifeCycleEx::addStateCross(KKSStateCross * s)
{
    if(!s)
        return;

    m_stateCrosses.append(s);
}

void KKSLifeCycleEx::addStateCross(KKSState * s1, KKSState * s2)
{
    if(!s1 || !s2)
        return;

    KKSStateCross * sc = new KKSStateCross();
    sc->setStates(s1, s2);
    
    addStateCross(sc);
    
    sc->release();
}

void KKSLifeCycleEx::removeStateCross(KKSStateCross * s)
{
    if(!s)
        return;
    
    m_stateCrosses.removeAll(s);
}

void KKSLifeCycleEx::removeStateCross(int index)
{
    m_stateCrosses.removeAt(index);
}

KKSStateCross * KKSLifeCycleEx::stateCross(int index)
{
    KKSStateCross * sc = NULL;
    if(index >= m_stateCrosses.count())
        return sc;

    sc = m_stateCrosses[index];
    return sc;
}

const KKSStateCross * KKSLifeCycleEx::stateCross(int index) const
{
    const KKSStateCross * sc = NULL;
    if(index >= m_stateCrosses.count())
        return sc;

    sc = m_stateCrosses.at(index);
    return sc;
}

const KKSState * KKSLifeCycleEx::startState() const
{
    return m_startState;
}

KKSState * KKSLifeCycleEx::startState()
{
    return m_startState;
}

void KKSLifeCycleEx::setStartState(KKSState * s)
{
    if(m_startState)
        m_startState->release();

    m_startState = s;

    if(m_startState)
        m_startState->addRef();
}

const KKSMap<int, KKSState * > & KKSLifeCycleEx::states() const
{
    return m_states;
}

int KKSLifeCycleEx::addState(KKSState * s)
{
    int cnt = m_states.insert(s->id(), s);
    if(!cnt)
        return ERROR_CODE;

    return OK_CODE;
}

int KKSLifeCycleEx::removeState(int id)
{
    m_states.remove(id);
    return OK_CODE;
}

void KKSLifeCycleEx::setStates(const KKSMap<int, KKSState* > & ss)
{
    m_states = ss;
}

/*
KKSLifeCycleEx * KKSLifeCycleEx::defLifeCycle()
{
    KKSLifeCycleEx * lc = new KKSLifeCycleEx(1, QObject::tr("Default life cycle"));

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
*/
