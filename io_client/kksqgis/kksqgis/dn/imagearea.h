#ifndef IMAGEAREA_H
#define IMAGEAREA_H

#include <QScrollArea>
#include <QPainter>

class ImageArea : public QWidget
{
    Q_OBJECT
public:
    explicit ImageArea(QWidget *parent = 0);
    void ChangedImg(QString PathToImg);

    bool LoadImage;
private:
    QImage img;
protected:
    virtual void paintEvent(QPaintEvent *pe);
signals:
    
public slots:
    
};

#endif // IMAGEAREA_H
