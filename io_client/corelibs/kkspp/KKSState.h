/***********************************************************************
 * Module:  KKSState.h
 * Author:  sergey
 * Modified: 1 ������� 2008 �. 12:09:41
 * Purpose: Declaration of the class KKSState
 * Comment: ��������� ��������������� �������
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSState_h)
#define __KKSSITOOOM_KKSState_h

#include <KKSRecord.h>
#include "KKSMap.h"
#include "kkspp_config.h"

class _PP_EXPORT KKSState : public KKSRecord
{
public:
   KKSState();
   KKSState(const KKSState & s);
   KKSState(int id, const QString & name, const QString & desc = QString::null);//, bool isKKSState = false);
   virtual ~KKSState();

   static KKSState * defState1();
   static KKSState * defState2();

   bool isSystem() const;
   void setIsSystem(bool yes = true);

protected:
private:

    bool m_isSystem;

};

#endif
