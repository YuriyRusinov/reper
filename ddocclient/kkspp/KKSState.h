/***********************************************************************
 * Module:  KKSState.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:09:41
 * Purpose: Declaration of the class KKSState
 * Comment: Состояние информационного объекта
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSState_h)
#define __KKSSITOOOM_KKSState_h

#include <KKSRecord.h>
#include "kkspp_config.h"

class _PP_EXPORT KKSState : public KKSRecord
{
public:
   KKSState();
   KKSState(const KKSState & s);
   KKSState(int id, const QString & name, const QString & desc = QString::null);
   virtual ~KKSState();

   static KKSState * defState1();
   static KKSState * defState2();

protected:
private:

};

#endif
