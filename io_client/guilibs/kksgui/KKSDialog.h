/*******************************************************************************************************
 * Module:  KKSDialog.h
 * Author:  yuriyrusinov
 * Modified: 23.01.2009 �. 11:32:00
 * Purpose: Declaration of the class KKSDialog
 * Comment: ������ ����� ����������� �� QWidget � ������������ ��� ����������
 * ������ ���������� �������� �������������� ��, ���, ��������� etc.
 *******************************************************************************************************/

#if !defined(__KKSSITOOOM_KKSDialog_h)
#define __KKSSITOOOM_KKSDialog_h

#include "kksgui_config.h"

#include <QDialog>
#include <QIcon>

class QEventLoop;

class _GUI_EXPORT KKSDialog : public QDialog
{
    public:
        KKSDialog (QWidget *parent=0, Qt::WindowFlags f=0);
        ~KKSDialog (void);

        int result (void) const;

        QSize sizeHint() const;

        const QIcon & icon() const;

    public slots:
        //int exec (void);

        void accept (void);
        void reject (void);
        void done (int r);

    signals:
        void accepted (void);
        void rejected (void);
        void finished (int r);

        void aboutToClose(QWidget *);

   protected:
        virtual void closeEvent (QCloseEvent * event);

        QIcon m_icon; //������, ������� ������������ � ���� � ������� �������� ����, ����� ������ ���� ���������� ��� QMdiSubWindow

   private:
        int res;
        QEventLoop *pEventL;

    private:
        Q_OBJECT
};

#endif
