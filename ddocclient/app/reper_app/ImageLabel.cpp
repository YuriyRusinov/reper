#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QCursor>
#include <QtDebug>
#include "ImageLabel.h"

ImageLabel :: ImageLabel (QWidget * parent, Qt::WindowFlags flags)
    : QLabel (parent, flags),
    selectionStarted (false)
{
}

ImageLabel :: ~ImageLabel (void)
{
}

void ImageLabel :: mousePressEvent (QMouseEvent * ev)
{
    if (ev->button () == Qt::LeftButton)
    {
        selectionStarted=true;
        selectionRect.setTopLeft (ev->pos());
        selectionRect.setBottomRight (ev->pos());
    }
    QLabel::mousePressEvent (ev);
}

void ImageLabel :: mouseReleaseEvent (QMouseEvent * ev)
{
    QLabel::mouseReleaseEvent (ev);
    selectionStarted=false;
    if (selectionRect.size ().width() <= 1 || selectionRect.size ().height() <= 1)
    {
        selectionRect.setSize (QSize());
        repaint ();
    }
    else
        qDebug () << __PRETTY_FUNCTION__ << selectionRect;
}

void ImageLabel :: mouseMoveEvent (QMouseEvent * ev)
{
    QLabel::mouseMoveEvent (ev);
    if (selectionStarted)
    {
        selectionRect.setBottomRight(ev->pos());
        repaint ();
    }
}

void ImageLabel :: paintEvent (QPaintEvent * ev)
{
    QLabel::paintEvent(ev);
    QPainter painter(this);
    
    //painter.begin (this);
    QBrush sBrush (QColor(0,0,0,180));
    QPen selPen = QPen (sBrush, 1, Qt::DashLine);
    painter.setPen (QPen(QBrush(QColor(0,0,0,180)),1,Qt::DashLine));
    painter.setBrush (QBrush(QColor(255,255,255,120)));
    painter.drawRect (selectionRect);
    //qDebug () << __PRETTY_FUNCTION__ << selectionRect;
    //painter.end();
}

void ImageLabel :: resizeEvent (QResizeEvent * ev)
{
    qDebug () << __PRETTY_FUNCTION__ << ev->size() << ev->oldSize() << selectionRect.size() << (pixmap() ? pixmap()->size() : QSize());
/*    if (!selectionRect.size().isEmpty())
    {
        //double scW = (double)(ev->size().width())/(double)(ev->oldSize().width());
        //double scH = (double)(ev->size().height())/(double)(ev->oldSize().height());
        int deltaW = ev->size().width() - ev->oldSize().width();
        int deltaH = ev->size().height() - ev->oldSize().height();
        QSize rectSize = selectionRect.size();
        rectSize.scale (rectSize.width() +deltaW, rectSize.height() + deltaH, Qt::IgnoreAspectRatio);
        //qDebug () << __PRETTY_FUNCTION__ << deltaW << deltaH << ev->size()
        //                                 << ev->oldSize() <<  selectionRect.size();
        selectionRect.setSize (rectSize);
    }
*/
    QLabel::resizeEvent (ev);
}

const QRect& ImageLabel :: getSelection (void) const
{
    return selectionRect;
}
