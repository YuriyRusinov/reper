#ifndef _ImageLabel_H
#define _ImageLabel_H

#include <QLabel>
#include <QRect>

class QMouseEvent;

class ImageLabel : public QLabel
{
public:
    ImageLabel (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~ImageLabel (void);

    const QRect& getSelection (void) const;
protected:
    virtual void mousePressEvent (QMouseEvent * ev);
    virtual void mouseReleaseEvent (QMouseEvent * ev);
    virtual void mouseMoveEvent (QMouseEvent * ev);
    virtual void paintEvent (QPaintEvent * ev);

private:
    bool selectionStarted;
    QRect selectionRect;

private:
    Q_OBJECT
};

#endif
