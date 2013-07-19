#include <QSvgRenderer>
#include <QFileDialog>
#include <QMessageBox>
#include <QtDebug>
#include <QBuffer>
#include <QPicture>
#include <QPainter>
#include <QPaintEvent>
#include <QWheelEvent>
#include <QGraphicsSvgItem>

#include "KKSAttrValue.h"
#include "KKSSvgWidget.h"

KKSSvgWidget :: KKSSvgWidget (const KKSAttrValue *attr, KKSIndAttr::KKSIndAttrClass isSys, QWidget *parent, Qt::WindowFlags flags)
    : QWidget (parent ,flags), KKSAttrWidget(attr, isSys),
    SVGContents (QByteArray()),
    dRend (new QSvgRenderer())
{

    connect (dRend, SIGNAL (repaintNeeded()), this, SLOT (updateSVG()) );
}

KKSSvgWidget :: KKSSvgWidget (const KKSAttrValue *attr, KKSIndAttr::KKSIndAttrClass isSys, const QByteArray& contents, QWidget *parent, Qt::WindowFlags flags)
    : QWidget (parent, flags), KKSAttrWidget(attr, isSys),
    SVGContents (contents),
    dRend (new QSvgRenderer(contents))
{
//    qDebug () << __PRETTY_FUNCTION__ << dRend->defaultSize () << dRend->isValid() << dRend->viewBoxF ();

    connect (dRend, SIGNAL (repaintNeeded()), this, SLOT (updateSVG()) );
}

KKSSvgWidget :: ~KKSSvgWidget (void)
{
}

const QByteArray& KKSSvgWidget :: getContents (void) const
{
    return SVGContents;
}

void KKSSvgWidget :: setContents (const QByteArray& c)
{
    SVGContents = c;
    dRend->load (SVGContents);
}

void KKSSvgWidget :: openFile (void)
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Image File"), 
                                                    QDir::currentPath(),
                                                    tr("SVG files (*.svg *.svgz *.svg.gz)"));

    if (fileName.isEmpty())
        return;

    QPicture pic;
    pic.load (fileName);
    if (pic.isNull())
    {
        qCritical() << tr("Cannot load %1.").arg(fileName);
        QMessageBox::critical(this, tr("Error"),
                                 tr("Cannot load %1.").arg(fileName));
        return;
    }
    QBuffer buffer (&SVGContents);
    buffer.open (QIODevice::WriteOnly);
    pic.save (&buffer);
    bool res = dRend->load (SVGContents);
    if (!res)
        return;
    //qDebug () << __PRETTY_FUNCTION__ << res << dRend->defaultSize () << dRend->isValid() << dRend->viewBoxF ();

    QVariant val (SVGContents);
    emit valueChanged (m_av->id(), m_isSystem, val);
    updateGeometry ();
    updateSVG ();
}

void KKSSvgWidget :: updateSVG (void)
{
    if (!dRend->isValid())
        return;
    //qDebug () << __PRETTY_FUNCTION__ << dRend->defaultSize () << dRend->isValid() << dRend->viewBoxF ();
    return QWidget::update ();
}

void KKSSvgWidget :: setVal (const QByteArray & newSVG)
{
    setContents (newSVG);
    QVariant val (SVGContents);
    emit valueChanged (m_av->id(), m_isSystem, val);
}

QSize KKSSvgWidget :: sizeHint (void) const
{
    if (dRend)
        return dRend->defaultSize();
    return QWidget::sizeHint ();
}

void KKSSvgWidget :: paintEvent (QPaintEvent *event)
{
    if (event)
        qDebug () << __PRETTY_FUNCTION__ << event->rect();
    QPainter p (this);
    p.setViewport (0, 0, width(), height());
    dRend->render (&p);
//    QWidget::paintEvent (event);
}

void KKSSvgWidget :: wheelEvent (QWheelEvent *event)
{
//    qDebug () << __PRETTY_FUNCTION__;
    const double diff = 0.1;
    QSize size = dRend->defaultSize();
    int width  = size.width();
    int height = size.height();
    if (event->delta() > 0)
    {
        width = int (this->width()+this->width()*diff);
        height = int (this->height()+this->height()*diff);
    }
    else
    {
        width  = int (this->width()-this->width()*diff);
        height = int (this->height()-this->height()*diff);
    }
    resize(width, height);
}
