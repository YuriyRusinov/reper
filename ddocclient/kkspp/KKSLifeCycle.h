/***********************************************************************
 * Module:  KKSLifeCycle.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:10:03
 * Purpose: Declaration of the class KKSLifeCycle
 * Comment: Жизненный цикл информационного объекта.
 *    Входит в состав категории.
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSLifeCycle_h)
#define __KKSSITOOOM_KKSLifeCycle_h

#include <QList>
#include "kkspp_config.h"
#include "KKSRecord.h"
#include "KKSStateCross.h"
#include "KKSList.h"
#include "KKSMap.h"



class _PP_EXPORT KKSLifeCycleEx : public KKSRecord
{
public:
    KKSLifeCycleEx(int id, const QString & name, const QString & desc = QString::null);
    KKSLifeCycleEx(const KKSLifeCycleEx & lc);
    virtual ~KKSLifeCycleEx();

    const KKSList<KKSStateCross *> & stateCrosses() const;
    void setStateCrosses(const KKSList<KKSStateCross *> & sc);

    void addStateCross(KKSStateCross * s);
    void addStateCross(KKSState * s1, KKSState * s2);
    void removeStateCross(KKSStateCross * s);
    void removeStateCross(int index);
    KKSStateCross * stateCross(int index);
    const KKSStateCross * stateCross(int index) const;

    const KKSState * startState() const;
    KKSState * startState();
    void setStartState(KKSState * s);

    const KKSState * autoStateAttr() const;
    KKSState * autoStateAttr();
    void setAutoStateAttr(KKSState * s);

    const KKSState * autoStateInd() const;
    KKSState * autoStateInd();
    void setAutoStateInd(KKSState * s);

    const KKSMap<int, KKSState * > & states() const;
    int addState(KKSState * s);
    int removeState(int id);
    void setStates(const KKSMap<int, KKSState* > & ss);
    //static KKSLifeCycleEx * defLifeCycle();

protected:
private:
    KKSList<KKSStateCross*> m_stateCrosses;//?
    
    
    KKSMap<int, KKSState *> m_states;
    KKSState * m_startState;
    KKSState * m_autoStateAttr;
    KKSState * m_autoStateInd;


};

#endif
