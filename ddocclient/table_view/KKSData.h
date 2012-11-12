/***********************************************************************
 * Module:  KKSData.h
 * Author:  sergey
 * Modified: 8 ������� 2008 �. 16:53:33
 * Purpose: Declaration of the class KKSData
 * Comment: ������� ����� ��� ���� ������� �������
 *    �������� ��� ���������� - ������� ���������� ������ �� ������� ������ � �� �������������� �����������, ����� ���������� ������ ����� ������ ����.
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSData_h)
#define __KKSSITOOOM_KKSData_h

#include "kkspp_config.h"
#include "KKSPtr.h"

class _PP_EXPORT KKSData
{
public:
   void addRef(void) const;
   void release(void) const;
   int refCount(void) const;

   KKSData();
   virtual ~KKSData();

protected:
private:
   mutable int m_refCount;
};

typedef KKSPtr<KKSData> KKSDataPtr;

#endif
