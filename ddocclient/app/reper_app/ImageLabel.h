#ifndef _ImageLabel_H
#define _ImageLabel_H

#include <QLabel>
#include <QPoint>

class QMouseEvent;

class ImageLabel : public QLabel
{
public:
    ImageLabel (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~ImageLabel (void);

protected:
    virtual void mousePressEvent (QMouseEvent * ev);
    virtual void mouseReleaseEvent (QMouseEvent * ev);
    virtual void mouseMoveEvent (QMouseEvent * ev);

private:
    QPoint topL;
    QPoint botR;

private:
    Q_OBJECT
};

#endif
