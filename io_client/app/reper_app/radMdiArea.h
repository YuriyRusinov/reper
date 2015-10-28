#ifndef _MDI_AREA_H
#define _MDI_AREA_H

#include <QMdiArea>
#include <QFont>
#include <QPen>
//#include "src_config.h"

class QResizeEvent;
class QPaintEvent;
class QShowEvent;
class QContextMenuEvent;
class QMenu;

class RadMdiArea : public QMdiArea
{
public:
    RadMdiArea (const QImage& im, QString title, QWidget * parent = 0);
    ~RadMdiArea (void);

private slots:
    void setTitleFont (void);
    void setTitleColor (void);

protected:
    //
    // Overrides
    //
    virtual void resizeEvent (QResizeEvent * resizeEvent);
    virtual void paintEvent (QPaintEvent * pEvent);
    virtual void showEvent (QShowEvent * event);
    virtual void contextMenuEvent (QContextMenuEvent * event);

private:
    //
    // Functions
    //
    void init (void);

private:
    //
    // Variables
    //
    QImage bkImage;
    QString rTitle;
    QPen tPen;
    QFont tFont;
    QMenu * pMenu;

private:
    Q_OBJECT
};
#endif
