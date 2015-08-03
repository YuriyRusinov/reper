/***********************************************************************
 * Module:  KKSGroup.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:11:53
 * Purpose: Declaration of the class KKSGroup
 * Comment: группа атрибутов
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSGroup_h)
#define __KKSSITOOOM_KKSGroup_h

#include "kkspp_config.h"

class _PP_EXPORT KKSGroup
{
public:
   KKSGroup();
   virtual ~KKSGroup();

   virtual int order(void) const = 0;
   virtual void setOrder(int newOrder) = 0;


};

#endif
