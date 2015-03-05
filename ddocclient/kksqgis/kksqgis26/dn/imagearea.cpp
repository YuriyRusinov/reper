#include "imagearea.h"

ImageArea::ImageArea(QWidget *parent) :
    QWidget(parent)
{
    this->LoadImage=FALSE;
    this->setFixedSize(100,100);
}

void ImageArea::paintEvent(QPaintEvent *pe)
{
 if (!LoadImage) return;

 QPainter Painter;
 int ww,hw;
 ww=0;
 hw=0;
 Painter.begin(this);
 ww=(int)this->img.width();
 hw=(int)this->img.height();
 this->setFixedSize(ww,hw);
 Painter.drawImage(0,0,this->img,0,0);
 Painter.end();
}

void ImageArea::ChangedImg(QString PathToImg)
{
 this->LoadImage=TRUE;
 this->img.load("e:/Beatls/DSC_2745.JPG");
 this->repaint();
}
