#include <QMouseEvent>
#include "ImageLabel.h"

ImageLabel :: ImageLabel (QWidget * parent, Qt::WindowFlags flags)
    : QLabel (parent, flags)
{
}

ImageLabel :: ~ImageLabel (void)
{
}

void ImageLabel :: mousePressEvent (QMouseEvent * ev)
{
    QLabel::mousePressEvent (ev);
}

void ImageLabel :: mouseReleaseEvent (QMouseEvent * ev)
{
    QLabel::mouseReleaseEvent (ev);
}

void ImageLabel :: mouseMoveEvent (QMouseEvent * ev)
{
    QLabel::mouseMoveEvent (ev);
}
