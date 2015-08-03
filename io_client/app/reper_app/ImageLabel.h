#ifndef _ImageLabel_H
#define _ImageLabel_H

#include <QLabel>
#include <QRect>

class QMouseEvent;
class QPaintEvent;
class QResizeEvent;

class ImageLabel : public QLabel
{
public:
    ImageLabel (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~ImageLabel (void);

    const QRect& getSelection (void) const;
    QImage getSelectedImage (void) const;
protected:
    //
    // Overrides
    //
    virtual void mousePressEvent (QMouseEvent * ev);
    virtual void mouseReleaseEvent (QMouseEvent * ev);
    virtual void mouseMoveEvent (QMouseEvent * ev);
    virtual void paintEvent (QPaintEvent * ev);
    virtual void resizeEvent (QResizeEvent * ev);

private:
    bool selectionStarted;
    QRect selectionRect;

private:
    Q_OBJECT
};

#endif
