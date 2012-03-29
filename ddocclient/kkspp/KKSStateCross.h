/***********************************************************************
 * Module:  KKSStateCross.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:09:49
 * Purpose: Declaration of the class KKSStateCross
 * Comment: переход между двумя состояниями
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSStateCross_h)
#define __KKSSITOOOM_KKSStateCross_h

class KKSState;

#include "kkspp_config.h"
#include "KKSData.h"

class _PP_EXPORT KKSStateCross : public KKSData
{
public:
   KKSStateCross();
   ~KKSStateCross();

   const KKSState * stateSrc() const;
   const KKSState * stateDst() const;
   KKSState * stateSrc();
   KKSState * stateDst();

   void setStates(KKSState * src, KKSState * dst);

protected:
private:
   KKSState* m_stateSrc;
   KKSState* m_stateDst;


};

#endif
