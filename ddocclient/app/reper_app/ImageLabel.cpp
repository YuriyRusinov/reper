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
    //qDebug () << __PRETTY_FUNCTION__ << ev->size() << ev->oldSize() << selectionRect.size();
    if (!selectionRect.size().isEmpty())
    {
        double scW = (double)(ev->size().width())/(double)(ev->oldSize().width());
        double scH = (double)(ev->size().height())/(double)(ev->oldSize().height());
        QSize rectSize = selectionRect.size();
//        rectSize.scale (rectSize.width() *scW, rectSize.height() * scH, Qt::IgnoreAspectRatio);
        qDebug () << __PRETTY_FUNCTION__ << scW << scH << ev->size()
                                         << ev->oldSize() <<  selectionRect.size();
        selectionRect.setSize (rectSize);
    }
    QLabel::resizeEvent (ev);
}

const QRect& ImageLabel :: getSelection (void) const
{
    return selectionRect;
}
