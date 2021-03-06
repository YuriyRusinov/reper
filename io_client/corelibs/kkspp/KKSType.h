/***********************************************************************
 * Module:  KKSType.h
 * Author:  sergey
 * Modified: 25 ������ 2008 �. 17:46:06
 * Purpose: Declaration of the class KKSType
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSType_h)
#define __KKSSITOOOM_KKSType_h

#include <KKSRecord.h>
#include "kkspp_config.h"

class _PP_EXPORT KKSType : public KKSRecord
{
public:
    KKSType();
    KKSType(const KKSType & t);
    KKSType(int id, const QString & name, const QString & desc = QString::null);
    virtual ~KKSType();

    static KKSType * defType1();//������������ ������ ��� ����� �� (�� ��� ����� ��������� ��) !!!

    static KKSType * createType10();//����������� ���������
    
    bool isQualifier() const; //������������ ������ ��� ����� ���������
    void setAsQualifier(bool yes);

    void setRName(const QString & rName);
    const QString & rName() const;

protected:
private:

    bool m_isQualifier;//���������� �������� �� ��������� ������� ���� ������������ (�.�. ����� �� ��������� ����������� �������)
    QString m_rName;

};

#endif
