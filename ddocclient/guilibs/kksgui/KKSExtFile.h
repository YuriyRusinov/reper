/***********************************************************************
 * Module:  KKSExtFile.h
 * Author:  sergey
 * Modified: 1 ������� 2008 �. 12:13:14
 * Purpose: Declaration of the class KKSExtFile
 * Comment: ��� ����� ������ ���� QLineEdit.
 *    �� �� ������ ����� ����� ������� ����� ����������, ����������� ����� ��������� �������� ���������� ���� � ��
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSExtFile_h)
#define __KKSSITOOOM_KKSExtFile_h

//#include <KKSWidget.h>
#include "kksgui_config.h"

#include <QLineEdit>

class _GUI_EXPORT KKSExtFile : public QLineEdit//KKSWidget
{
    public:
        KKSExtFile();
        ~KKSExtFile();

    protected:
    private:
        Q_OBJECT

};

#endif
