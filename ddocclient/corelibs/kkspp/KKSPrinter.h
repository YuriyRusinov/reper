/***********************************************************************
 * Module:  KKSPrinter.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:09:41
 * Purpose: Declaration of the class KKSState
 * Comment: 
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSPrinter_h)
#define __KKSSITOOOM_KKSPrinter_h

#include <QFont>
#include <QFontMetrics>
#include <QPrinter>
#include <QPainter>
#include <QTextEdit>


#include "KKSList.h"
#include "KKSAttrValue.h"

#include "kkspp_config.h"

class KKSObject;
class KKSObjectExemplar;

class _PP_EXPORT KKSPrinter 
{
public:
   KKSPrinter();
   virtual ~KKSPrinter();

   void print(KKSObject * io, QString fileName = QString::null);
   void print(KKSObjectExemplar * eio, QString fileName = QString::null);

protected:
private:

    int wSpace;
    int wColon;
    int height;

    int pageWidth;
    int pageHeight;

    QFont font;
    QFontMetrics fm;
    QPainter painter;
    QPrinter * printer;
    QTextEdit * edit;
    
    int printTitles(const QStringList & titles);
    int printTitle(const QString & title, int tabH);

    int printAttributes(const KKSList<KKSAttrValue *> & avl, int tabH);

    int printSysAttributes(const KKSObject * io, int tabH);

    QPoint printAttribute(const QString & title,
                          const QString & value,
                          const QPoint & cPoint);

    int printValue(int tabW, 
                   int tabH, 
                   const QString & value, 
                   int flags);

    int printValuePart(int tabW, 
                       int tabH, 
                       const QString & value, 
                       int flags);
    int printValuePart1(int tabW, 
                        int tabH, 
                        const QString & value, 
                        int index, 
                        int flags);

    QRectF computeRealBound(int tabWidth, 
                            int tabHeight, 
                            const QString & value,
                            int flags);

    void initPrinter(const QString & fileName);
    void stopPrinter();
};

#endif
