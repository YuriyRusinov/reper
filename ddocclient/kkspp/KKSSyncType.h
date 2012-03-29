/***********************************************************************
 * Module:  KKSSyncType.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:09:41
 * Purpose: Declaration of the class KKSSyncType
 * Comment: Тип синхронизации информационного объекта
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSSyncType_h)
#define __KKSSITOOOM_KKSSyncType_h

#include <KKSRecord.h>
#include "kkspp_config.h"

class _PP_EXPORT KKSSyncType : public KKSRecord
{
public:
   KKSSyncType();
   KKSSyncType(const KKSSyncType & s);
   KKSSyncType(int id, const QString & name, const QString & desc = QString::null);
   virtual ~KKSSyncType();

   static KKSSyncType* defSyncType1();

protected:
private:

};

#endif
