/***********************************************************************
 * Module:  KKSExtFile.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:13:14
 * Purpose: Declaration of the class KKSExtFile
 * Comment: Это будет виджет типа QLineEdit.
 *    Но он должен кроме всего прочего иметь функционал, позволяющий через некоторый механизм записывать файл в БД
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
