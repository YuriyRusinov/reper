#include "dnwidgetimage.h"

DNWidgetImage::DNWidgetImage(QWidget *parent) :
    QWidget(parent)
{
 this->isImgLoad=FALSE;
 this->IsCretaePolyOn=FALSE;
 this->IsSelPoly=FALSE;
 this->NewKof=1.;
 this->MouseX=0;
 this->MouseY=0;
 this->SliderX=0;
 this->SliderY=0;
 //this->KolvoPixPoly=0;
 //this->setMouseTracking(TRUE);
 for(int i=0;i<50;i++)
 {
  this->ColorClassR[i]=rand()%255;
  this->ColorClassG[i]=rand()%255;
  this->ColorClassB[i]=rand()%255;
 }//for(int i=0;i<50;i++)
}


void DNWidgetImage::paintEvent(QPaintEvent *pe)
{
 QPainter Painter;
 int ww,hw;
 int i,ip,xc,yc,xl1,xl2,yl1,yl2;
 //int x,y;
 Qt::GlobalColor Color;

// if(!this->isImgLoad && this->Polygon.pt.size()>0)
// {
//  ww=0;
//  hw=0;
//  for(i=0;i<this->Polygon.pt.size();i++)
//  {
//   if(ww<this->Polygon.pt[i].x())
//    ww=this->Polygon.pt[i].x();
//   if(hw<this->Polygon.pt[i].y())
//    hw=this->Polygon.pt[i].y();
//  }//for(ip=0;ip<this->Polygon.size();i++)
//  ww=ww*this->NewKof;
//  hw=hw*this->NewKof;
//  this->resize(ww+10,hw+10);
// }//if(!this->LoadImg && this->Polygon.size()>0)

 Color=Qt::magenta;
 Painter.begin(this);

 //Если картинка загружена
 if(this->isImgLoad)
 {
  ww=(int)(this->img.width()/**this->NewKof*/);
  hw=(int)(this->img.height()/**this->NewKof*/);
  this->resize(ww,hw);
  Painter.drawLine(0,0,ww,hw);
  Painter.drawImage(0,0,this->img/*Scale*/,0,0);
 }//if(this->LoadImg)

//Отрисовка текущего полигона

 if(this->Polygon.pt.size()>0)
 {
//     QMessageBox msg;
//     msg.setText(QString().setNum(this->Polygon.pt.size()));
//     msg.exec();

  xl1=this->Polygon.pt[0].x();
  yl1=this->Polygon.pt[0].y();
  xl2=this->MouseX/this->NewKof;
  yl2=this->MouseY/this->NewKof;

  i=0;
  while(i<this->Polygon.pt.size())
  {
   xc=this->Polygon.pt[i].x();
   yc=this->Polygon.pt[i].y();
   xc=xc*this->NewKof;
   yc=yc*this->NewKof;

   Painter.setPen(QPen(Color,4));
   Painter.drawPoint(xc,yc);
   i++;
  }//while(i<this->KolvoPixPoly)
  if(this->Polygon.pt.size()==1)
  {
   Painter.setPen(QPen(Color,4));
   Painter.drawLine(xl1,yl1,xl2,yl2);
  }
  if(this->Polygon.pt.size()>1)
  {
   Painter.setPen(QPen(Color,4));
   QPolygon Polygon;
   i=0;
   while(i<this->Polygon.pt.size())
   {
    xc=this->Polygon.pt[i].x();
    yc=this->Polygon.pt[i].y();
    xc=xc*this->NewKof;
    yc=yc*this->NewKof;
    Polygon<<QPoint(xc,yc);
    i++;
   }//while(i<this->KolvoPixPoly)

   if(this->IsCretaePolyOn)
    Polygon<<QPoint(this->MouseX,this->MouseY);

   Painter.drawPolygon(Polygon);
  }

//Если к текущему полигону применялись методы классификации то отрисовываем на полигоне результаты классификации
  if(this->Polygon.IsPolyClassif)
  {
   this->Polygon.maxY=this->Polygon.GetMaxP().y();
   this->Polygon.minY=this->Polygon.GetMinP().y();
   this->Polygon.maxX=this->Polygon.GetMaxP().x();
   this->Polygon.minX=this->Polygon.GetMinP().x();

//Размер отрисовки классифицированной области с учётом текущего масштаба изображения
   int ThisWK=(this->Polygon.maxX-this->Polygon.minX)*this->NewKof;
   int ThisHK=(this->Polygon.maxY-this->Polygon.minY)*this->NewKof;

   int ThisW=(this->Polygon.maxX-this->Polygon.minX);
   int ThisH=(this->Polygon.maxY-this->Polygon.minY);

//Переменные перевода из масштабированного изображения в нормальное
   int xp,yp;

   for(int jy=0;jy<ThisHK;jy++)
   {
    for(int jx=0;jx<ThisWK;jx++)
    {
     xp=jx/this->NewKof;
     yp=jy/this->NewKof;
     if(this->Polygon.ClassifMass[xp+yp*ThisW]>=0)
     {
      int num=this->Polygon.ClassifMass[xp+yp*ThisW];
      Painter.setPen(QPen(QColor(this->ColorClassR[num],this->ColorClassG[num],this->ColorClassB[num],255/*this->ProzrPol*/),1));
      Painter.drawPoint(jx+this->Polygon.minX*this->NewKof,jy+this->Polygon.minY*this->NewKof);
     }
    }//for(int jx=0;jx<ThisWK;jx++)
   }//for(int jy=0;jy<ThisHK;jy++)
  }//if(this->Polygon.IsPolyClassif)
 }//if(this->KolvoPixPoly>0)

 //Отрисовка полигонов

 if(this->Polygons.size()>0)
 {
  QPolygon *Polygon;
  Polygon=new QPolygon[this->Polygons.size()];
  for(ip=0;ip<this->Polygons.size();ip++)
  {
   i=0;
   while(i<this->Polygons[ip].pt.size())
   {
    xc=this->Polygons[ip].pt[i].x();
    yc=this->Polygons[ip].pt[i].y();
    xc=xc*this->NewKof;
    yc=yc*this->NewKof;



    Polygon[ip]<<QPoint(xc,yc);

    Painter.setPen(QPen(Color,4));
    Painter.drawPoint(xc,yc);
    i++;
//    QMessageBox msg;
//    msg.setText(QString().setNum(xc));
//    msg.exec();
   }//while(i<this->KolvoPixPoly)

   Painter.setPen(QPen(Color,2));
   Painter.drawPolygon(Polygon[ip]);

  }//for(ip=0;ip<this->Polygons.size();ip++)
  delete[] Polygon;
 }//if(this->Polygons.size()>0)

 Painter.end();
}
void DNWidgetImage::mouseMoveEvent(QMouseEvent* mEvent)
{
 this->MouseX=mEvent->x();
 this->MouseY=mEvent->y();

 emit MouseMove(this->MouseX/this->NewKof,this->MouseY/this->NewKof);

 if(this->Polygon.pt.size()>0)
  this->repaint();
}
void DNWidgetImage::mousePressEvent(QMouseEvent *mEvent)
{
 int x,y;
 QPoint Point;
 x=this->MouseX/this->NewKof;
 y=this->MouseY/this->NewKof;
 if(this->IsCretaePolyOn) //Если выбран пункт меню Выделить полигон
 {
  Point.setX(x);
  Point.setY(y);
  this->Polygon.pt<<Point;
  //this->Polygon.KolvoPix++;
  this->repaint();
 }
// emit this->MouseClicked(x,y);
}

/*Private*/
int DNWidgetImage::GetNumCurPoly()
{
 int CurPoly=-1;
 for(int i=0;i<this->Polygons.size();i++)
 {
  if(this->Polygons[i].IsPolygonSelect)
  {
   CurPoly=i;
   break;
  }//if(this->Polygons[i].IsPolygonSelect)
 }//for(int i=0;i<this->Polygons.size();i++)
 return CurPoly;
}

void DNWidgetImage::DeselectPoly()
{
 for(int i=0;i<this->Polygons.size();i++)
 {
  this->Polygons[i].IsPolygonSelect=false;
 }//for(int i=0;i<this->Polygons.size();i++)
}

/************************DNImgPoly************************************************************/

DNImgPoly::DNImgPoly()
{
 this->IsPolygonSelect=FALSE;
 //this->KolvoPix=0;
}

QPoint DNImgPoly::GetMinP()
{
 int i,x,y;
 QPoint Point;
 Point.setX(-1);
 Point.setY(-1);
 if(this->pt.size()>0)
 {
  i=0;
  while(i<this->pt.size())
  {
   if(i==0)
   {
    x=this->pt[i].x();
    y=this->pt[i].y();
   }//if(i==0)
   if(i!=0)
   {
    if(this->pt[i].x()<x)
     x=this->pt[i].x();
    if(this->pt[i].y()<y)
     y=this->pt[i].y();
   }//if(i!=0)
   i++;
  }//while(i<this->pt.size())
  Point.setX(x);
  Point.setY(y);
 }//if(this->pt.size()>0)
 this->minX=Point.x();
 this->minY=Point.y();
 return Point;
}

QPoint DNImgPoly::GetMaxP()
{
 int i,x,y;
 QPoint Point;
 Point.setX(-1);
 Point.setY(-1);
 if(this->pt.size()>0)
 {
  i=0;
  while(i<this->pt.size())
  {
   if(i==0)
   {
    x=this->pt[i].x();
    y=this->pt[i].y();
   }//if(i==0)
   if(i!=0)
   {
    if(this->pt[i].x()>x)
     x=this->pt[i].x();
    if(this->pt[i].y()>y)
     y=this->pt[i].y();
   }//if(i!=0)
   i++;
  }//while(i<this->pt.size())
  Point.setX(x);
  Point.setY(y);
 }//if(this->pt.size()>0)
 this->maxX=Point.x();
 this->maxY=Point.y();
 return Point;
}

/*Выделение полигона*/
void DNImgPoly::SelectPolygon()
{
 int MassW,MassH;
 int jx,jy;
 if(this->pt.size()>2)
 {
  MassW=this->GetMaxP().x()-this->GetMinP().x();
  MassH=this->GetMaxP().y()-this->GetMinP().y();

  if(this->IsPolygonSelect==TRUE)
   delete[] this->SelPoly;

  this->SelPoly=new int[MassW*MassH];
  this->IsPolygonSelect=TRUE;

  for(jx=0;jx<MassW;jx++)
  {
   for(jy=0;jy<MassH;jy++)
   {
    this->SelPoly[jx+jy*MassW]=-1;
   }//for(jy=0;jy<MassH;jy++)
  }//for(jx=0;jx<MassW;jx++)

  for(jy=0;jy<MassH;jy++)
  {
   for(jx=0;jx<MassW;jx++)
   {
    if(this->IsPointInside(jx+this->minX,jy+this->minY))
     this->SelPoly[jx+jy*MassW]=1;
   }//for(jx=0;jx<MassW;jx++)
  }//for(jy=0;jy<MassH;jy++)
 }//if(this->pt.size()>2)
}

/*Проверка попадает ли точка в полигон*/
bool DNImgPoly::IsPointInside(int xp,int yp)
{
 int intersections_num = 0;
 int prev =this->pt.size()-1;
 bool prev_under =this->pt[prev].y()<=yp;

 for (int i=0;i<this->pt.size(); ++i)
 {
  bool cur_under=this->pt[i].y()<=yp;
  int ax,ay,bx,by;
  ax=this->pt[prev].x()-xp;
  ay=this->pt[prev].y()-yp;

  bx=this->pt[i].x()-xp;
  by=this->pt[i].y()-yp;

  float t = (ax*(by - ay) - ay*(bx - ax));
  if(cur_under&&!prev_under)
  {
   if (t>0)
    intersections_num += 1;
  }
  if (!cur_under && prev_under)
  {
   if (t<0)
    intersections_num += 1;
  }
  prev = i;
  prev_under = cur_under;
 }
 return (intersections_num&1) != 0;
}
