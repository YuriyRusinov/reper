/***********************************************************************
 * Module:  KKSPrinter.cpp
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:09:41
 * Purpose: Implementation of the class KKSPrinter
 * Comment: 
 ***********************************************************************/

#include "KKSPrinter.h"

#include "KKSState.h"
#include "KKSObject.h"
#include "KKSObjectExemplar.h"
#include "KKSAttribute.h"
#include "KKSCategoryAttr.h"
#include "KKSAttrValue.h"

#include <QtDebug>

KKSPrinter::KKSPrinter():
font(QFont("Times New Roman", 12)),
fm(QFontMetrics(font)),
printer(NULL)
{
    wSpace = fm.width(" ");
    wColon = fm.width(":");

    height = fm.height();
    pageWidth = 0;
    pageHeight = 0;
}

KKSPrinter::~KKSPrinter()
{
   // TODO : implement
}

void KKSPrinter::print(KKSObject * io, QString fileName)
{
    if(!io)
        return;

    printer = new QPrinter(QPrinter::ScreenResolution);
    
    initPrinter(fileName);

    QStringList titles;
    titles << io->name();
    
    int tabH = printTitles(titles);

    tabH = printTitle(QObject::tr("\nSystem attributes:"), tabH);
    
    tabH = printSysAttributes(io, tabH);

    tabH = printTitle(QObject::tr("\nAttributes:"), tabH);
    
    tabH = printAttributes(io->attrValues(), tabH);
    
    stopPrinter();
}

void KKSPrinter::print(KKSObjectExemplar * eio, QString fileName)
{
    if(!eio)
        return;

    printer = new QPrinter(QPrinter::ScreenResolution);
    
    initPrinter(fileName);

    QStringList titles;
    titles << QObject::tr("Record of qualifier:") 
           << (QString("\"") + eio->io()->name() + QString("\""));
    
    int tabH = printTitles(titles);

    printAttributes(eio->attrValues(), tabH);
    
    stopPrinter();
}

int KKSPrinter::printTitles(const QStringList & titles)
{
    int tabH = 0;
    for(int i=0; i<titles.count(); i++){
        tabH += printTitle(titles.at(i), tabH);
    }

    return tabH;
}

int KKSPrinter::printTitle(const QString& title, int tabH)
{
    QRectF realBound = computeRealBound(0, tabH, title, Qt::TextWordWrap | Qt::AlignCenter);
    painter.drawText(realBound, Qt::TextWordWrap | Qt::AlignCenter, title, &realBound);
    tabH += realBound.height();

    return tabH;
}

int KKSPrinter::printSysAttributes(const KKSObject * io, int tabH)
{
    if(!io)
        return tabH;
    
    QString value = io->info();
    QString title = QObject::tr("Information");
    if(!value.trimmed().isEmpty())
        tabH = printAttribute(title, value, QPoint(0, tabH)).y();

    value = io->desc();
    title = QObject::tr("Description");
    if(!value.trimmed().isEmpty())
        tabH = printAttribute(title, value, QPoint(0, tabH)).y();

    value = io->author();
    title = QObject::tr("Author");
    if(!value.trimmed().isEmpty())
        tabH = printAttribute(title, value, QPoint(0, tabH)).y();
    
    value = io->insertTime().toString("dd.MM.yyyy HH:mm");
    title = QObject::tr("Insert time");
    if(!value.trimmed().isEmpty())
        tabH = printAttribute(title, value, QPoint(0, tabH)).y();

    value = io->state()->name();
    title = QObject::tr("State");
    if(!value.trimmed().isEmpty())
        tabH = printAttribute(title, value, QPoint(0, tabH)).y();

    return tabH;
}

int KKSPrinter::printAttributes(const KKSList<KKSAttrValue *> & avl, int tabH)
{
    QPoint startPoint(0, tabH);
    QPoint currPoint = startPoint;

    for(int i=0; i<avl.size(); i++){
        KKSAttrValue * av = avl.at(i);
        QString title = av->attribute()->title();
        QString value = av->value().value();

        currPoint = printAttribute(title, value, currPoint);
    }
    tabH = currPoint.y();
    
    return tabH;
}

QPoint KKSPrinter::printAttribute(const QString & title,
                                  const QString & value,
                                  const QPoint & cPoint)
{
    QPoint nextPoint;
    int titleWidth = fm.width(title);
    
    painter.drawText(QRect(cPoint.x(), cPoint.y(), titleWidth, height), title);
    painter.drawText(QRect(cPoint.x()+titleWidth, cPoint.y(), wColon, height), ":");

    int tabH = printValue(titleWidth + wSpace + wColon, cPoint.y(), value, Qt::TextWordWrap);
    
    /*
    QRectF realBound = computeRealBound(titleWidth + wSpace + wColon, cPoint.y(), value, Qt::TextWordWrap);

    painter.drawText(realBound, Qt::TextWordWrap, value, &realBound);
    
    //nextPoint = QPoint(0, cPoint.y() + realBound.height());
    */
    nextPoint = QPoint(0, tabH);
    return nextPoint;
}

int KKSPrinter::printValue(int tabW, int tabH, const QString & value, int flags)
{
    
    QRectF realBound = computeRealBound(tabW, tabH, value, flags);
//--- 
    painter.drawText(realBound, flags, value, &realBound);
    
    if(realBound.height() >= pageHeight - tabH){
        printer->newPage();
        tabH = 0;
        return tabH;
    }

    tabH += realBound.height();
    return tabH;


//---

/*    if(realBound.height() <= pageHeight - tabH){
        painter.drawText(realBound, flags, value, &realBound);

        tabH += realBound.height();

        if(pageHeight - tabH == 0){
            printer->newPage();
            tabH = 0;
        }
            
        return tabH;
    }
    else{
        tabH = printValuePart(tabW, tabH, value, flags);
    }
*/
    return tabH;
}

int KKSPrinter::printValuePart(int tabW, int tabH, const QString & value, int flags)
{
    QString valueLeft = value.left(value.size()/2);
    QRectF realBound = computeRealBound(tabW, tabH, valueLeft, flags);

    int direction = (pageHeight - tabH) - realBound.height();
    //то, что осталось меньше, чем надо, тогда надо уменьшать valueLeft по одному символу
    if(direction < 0){
        tabH = printValuePart1(tabW, tabH, value, -1, flags);
    }
    else if(direction == 0){
        painter.drawText(realBound, flags, valueLeft, &realBound);
        
        printer->newPage();
        
        QString valueRight = value.right((value.size()/2));
        realBound = computeRealBound(tabW, 0, valueRight, flags);
        painter.drawText(realBound, flags, valueRight, &realBound);
        tabH = realBound.height();
        
        return tabH;
    }
    else{
        tabH = printValuePart1(tabW, tabH, value, 1, flags);
    }

    return tabH;
}

int KKSPrinter::printValuePart1(int tabW, int tabH, const QString & value, int index, int flags)
{
    QString valueLeft = value.left( (value.size()/2) + index);
    QRectF realBound = computeRealBound(tabW, tabH, valueLeft, flags);

    int direction = (pageHeight - tabH) - realBound.height();
    //то, что осталось, меньше, чем надо, тогда надо уменьшать valueLeft по одному символу
    if(direction != 0){
        if(index < 0)
            index--;
        else
            index++;

        tabH = printValuePart1(tabW, tabH, value, index, flags);
    }
    else{
        painter.drawText(realBound, flags, valueLeft, &realBound);
        
        printer->newPage();
        
        if(index > 0){
            QString valueRight = value.right(value.size() - valueLeft.size());
            realBound = computeRealBound(tabW, 0, valueRight, flags);
            painter.drawText(realBound, flags, valueRight, &realBound);
            tabH = realBound.height();
        
            return tabH;
        }
        else{
            QString newValue = value.right(value.size()/2 + index);
            tabH = printValuePart(tabW, tabH, newValue, flags);
        }
    }

    return tabH;
}

QRectF KKSPrinter::computeRealBound(int tabWidth, int tabHeight, const QString & value, int flags)
{
    int theWidth = pageWidth - tabWidth;

    //ширина выводимой строки в одном измерении 
    //(т.е. сколько бы она заняла будучи выведенной в одну строку)
    qreal valueStringWidth = fm.width(value);
    
    //вычисляемое количество необходимых строк, чтобы на странице увидеть весь текст
    qreal columnCountR = 1.0;
    int columnCount = 1;
    
    //если ширина выводимой строки больше ширины доступной части страницы
    //то высичляем количнство необходимых строк
    if(valueStringWidth > theWidth){
        columnCountR = (valueStringWidth / theWidth);
        columnCount = (int) columnCountR;
        if(columnCount < columnCountR)
            columnCount++;
    }
    
    qreal valueHeight = height * columnCount;

    QRectF rect(0 + tabWidth, tabHeight, theWidth, valueHeight);
    
    QRectF realBound = painter.boundingRect(rect, flags, value);
/*    
    if(realBound.height() + tabHeight > pageHeight){
        printer->newPage();
        
        rect = QRectF(0 + tabWidth, 0, theWidth, valueHeight);
        realBound = painter.boundingRect(rect, flags, value);
    }
*/
    return realBound;
}

void KKSPrinter::initPrinter(const QString & fileName)
{
    if(fileName.isEmpty())
        printer->setOutputFileName("outfile.pdf");
    else
        printer->setOutputFileName(fileName);

    painter.begin(printer);
    painter.setFont(font);

    QRectF pageRect = printer->pageRect();
    pageWidth = pageRect.width();
    pageHeight = pageRect.height();
}

void KKSPrinter::stopPrinter()
{
    painter.end();
    
    delete printer;
}
