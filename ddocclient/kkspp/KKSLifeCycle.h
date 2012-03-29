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
#include "KKSData.h"
#include "KKSStateCross.h"
#include "KKSList.h"



class _PP_EXPORT KKSLifeCycle : public KKSData
{
public:
    KKSLifeCycle();
    KKSLifeCycle(const KKSLifeCycle & lc);
    virtual ~KKSLifeCycle();

    const KKSList<KKSStateCross *> & stateCrosses() const;
    void setStateCrosses(const KKSList<KKSStateCross *> & sc);

    void addStateCross(KKSStateCross * s);
    void addStateCross(KKSState * s1, KKSState * s2);
    void removeStateCross(KKSStateCross * s);
    void removeStateCross(int index);
    KKSStateCross * stateCross(int index);
    const KKSStateCross * stateCross(int index) const;

    static KKSLifeCycle * defLifeCycle();

protected:
private:
    KKSList<KKSStateCross*> m_stateCrosses;


};

#endif
