#include <QPainter>
#include "KKSImage.h"

KKSImage :: KKSImage (QWidget *parent, Qt::WFlags f)
    : QWidget (parent, f)
{
}

KKSImage :: ~KKSImage (void)
{
}

void KKSImage :: setImage (const QImage& im)
{
    wIm = im;
    this->setMinimumSize (wIm.size());
}

const QImage& KKSImage :: getImage (void) const
{
    return wIm;
}

void KKSImage :: paintEvent (QPaintEvent *event)
{
    if (wIm.isNull())
        return QWidget::paintEvent (event);
    QPainter p (this);//= pE->painter ();
    QPointF stPoint (0.0, 0.0);// this->width()/2
    p.drawImage (stPoint, wIm);
}
