#ifndef _KKSImage_H
#define _KKSImage_H

#include <QWidget>
#include <kksgui_config.h>

class _GUI_EXPORT KKSImage : public QWidget
{
public:
    KKSImage (QWidget *parent=0, Qt::WFlags f=0);
    ~KKSImage (void);

    void setImage (const QImage& im);
    const QImage& getImage (void) const;

protected:
    void paintEvent (QPaintEvent *event);

private:
    QImage wIm;

private:
    Q_OBJECT
};

#endif
