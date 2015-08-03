#include "dlggraph.h"
#include "ui_dlggraph.h"

DlgGraph::DlgGraph(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgGraph)
{
 ui->setupUi(this);
 this->DLeft=50;
 this->DBottom=30;

 this->XRec1=this->DLeft;
 this->YRec1=120;
 this->XRec2=this->width()-this->XRec1;
 this->YRec2=this->height()-this->DBottom;

 this->WGraph=this->XRec2-this->XRec1;
 this->HGraph=this->YRec2-this->YRec1;

 this->CurrentGraph=-1;
 MaxXVal=-1.;
 MaxYVal=-1.;

 this->Graphs.clear();
}

DlgGraph::~DlgGraph()
{
 delete ui;

 for(int i=0;i<Graphs.size();i++)
 {
  Graphs[i].XValue.clear();
  Graphs[i].YValue.clear();
 }//for(int i=0;i<Graphs.size();i++)
 Graphs.clear();
 MaxXVal=-1.;
 MaxYVal=-1.;
}

void DlgGraph::AddGraph(float *xv,float *yv, int n)
{
 DataGraph NewGraph;
 NewGraph.XValue.clear();
 NewGraph.YValue.clear();

 for(int i=0;i<n;i++)
 {
  NewGraph.XValue<<xv[i];
  NewGraph.YValue<<yv[i];
  if(xv[i]>=MaxXVal)
   MaxXVal=xv[i];
  if(yv[i]>=MaxYVal)
   MaxYVal=yv[i];
 }//for(int i=0;i<n;i++)
 Graphs<<NewGraph;

 this->RCol<<rand()%255;
 this->GCol<<rand()%(255-this->RCol[this->RCol.size()-1]-1);
 this->BCol<<rand()%255;
}

void DlgGraph::paintEvent(QPaintEvent *pe)
{
 QPainter Painter;

 if(!this->isHidden())
 {
  Painter.begin(this);
  Painter.setPen(QPen(QColor(0,0,0),3));
  Painter.drawLine(this->XRec1,this->YRec2,this->XRec2,this->YRec2);
  Painter.drawLine(this->XRec1,this->YRec1,this->XRec1,this->YRec2);

  if(this->Graphs.size()>0)
  {
   this->OsX=this->Okrugl(MaxXVal);
   this->OsY=this->Okrugl(MaxYVal);
   this->XKof=this->WGraph/this->OsX.MaxZnach;
   this->YKof=this->HGraph/this->OsY.MaxZnach;

   int i,j,x1,y1,x2,y2;
   QString DataString;
   Painter.setPen(QPen(QColor(0,0,0),1));
//Нанесение сетки
   for(i=0;i<this->OsX.KolvoDel;i++)
   {
    x1=(i+1)*this->OsX.CenaDel*this->XKof+this->XRec1;
    x2=(i+1)*this->OsX.CenaDel*this->XKof+this->XRec1;
    y1=this->YRec1;
    y2=this->YRec2;
    Painter.drawLine(x1,y1,x2,y2);
    DataString.setNum((float)(i+1)*this->OsX.CenaDel,'g',2);
    Painter.drawText(x1-5,y2+20,DataString);
   }

   for(i=0;i<this->OsY.KolvoDel;i++)
   {
    y1=i*this->OsY.CenaDel*this->YKof+this->YRec1;
    y2=i*this->OsY.CenaDel*this->YKof+this->YRec1;
    x1=this->XRec1;
    x2=this->XRec2;
    Painter.drawLine(x1,y1,x2,y2);
    DataString.setNum((float)(this->OsY.KolvoDel-i)*this->OsY.CenaDel,'g',2);
    Painter.drawText(x1-(DataString.length()*5)-15,y2+5,DataString);
   }

   int LineDepth;
   LineDepth=2;
   if(i==this->CurrentGraph)
    LineDepth=4;


  // QMessageBox

   for(i=0;i<this->Graphs.size();i++)
   {
    if(i!=this->CurrentGraph)
    {
     Painter.setPen(QPen(QColor(this->RCol[i],this->GCol[i],this->BCol[i]),2));
     for(j=0;j<this->Graphs[i].XValue.size()-1;j++)
     {
      x1=Graphs[i].XValue[j]*this->XKof+this->XRec1;
      x2=Graphs[i].XValue[j+1]*this->XKof+this->XRec1;
      y1=this->YRec2-(Graphs[i].YValue[j]*this->YKof);
      y2=this->YRec2-(Graphs[i].YValue[j+1]*this->YKof);
      Painter.drawLine(x1,y1,x2,y2);
     }//for(j=0;j<this->Ch;j++)
    }//if(i!=this->CurrentGraph)
   }//for(i=0;i<this->SpecGraphics.size();i++)

   if(this->CurrentGraph!=-1)
   {
    i=this->CurrentGraph;
    Painter.setPen(QPen(QColor(this->RCol[i],this->GCol[i],this->BCol[i]),4));
    for(j=0;j<this->Graphs[i].XValue.size()-1;j++)
    {
     x1=Graphs[i].XValue[j]*this->XKof+this->XRec1;
     x2=Graphs[i].XValue[j+1]*this->XKof+this->XRec1;
     y1=this->YRec2-(Graphs[i].YValue[j]*this->YKof);
     y2=this->YRec2-(Graphs[i].YValue[j+1]*this->YKof);
     Painter.drawLine(x1,y1,x2,y2);
    }//for(j=0;j<this->Ch;j++)
   }//if(this->CurrentGraph!=-1)
  }//if(this->SpecGraphics.size()>0 && this->Ch>0)
  Painter.end();
 }//if(!this->isHidden())
}
void DlgGraph::resizeEvent(QResizeEvent*)
{
 this->XRec2=this->width()-this->XRec1;
 this->YRec2=this->height()-this->DBottom;
 this->WGraph=this->XRec2-this->XRec1;
 this->HGraph=this->YRec2-this->YRec1;

 if(this->Graphs.size()>0)
 {
  this->XKof=(float)this->WGraph/this->OsX.MaxZnach;
  this->YKof=(float)this->HGraph/this->OsY.MaxZnach;
 }
}
OsParam DlgGraph::Okrugl(float var)
{
 float RVar;
 float varDec,Dec;
 int nDec,i,varDecInt;
 int CDelen[3],MinOst,Ost,nDelen;
 OsParam ThisOs;

 CDelen[0]=10;
 CDelen[1]=20;
 CDelen[2]=50;

 Dec=0;
 if(var>=1000)
  Dec=0.1;
 else
  Dec=10;

 varDec=var;
 nDec=0;
 while(varDec>=1000. || varDec<100.)
 {
  varDec=varDec*Dec;
  nDec++;
 }

 if(Dec==0.1)
  nDec=nDec*(-1);

 varDecInt=(int)varDec;

 MinOst=CDelen[0]-(varDecInt%CDelen[0]);
 nDelen=0;
 for(i=1;i<3;i++)
 {
  Ost=CDelen[i]-(varDecInt%CDelen[i]);
  if(Ost<=MinOst)
  {
   MinOst=Ost;
   nDelen=i;
  }//if(Ost<MinOst)
 }//for(i=1;i<3;i++)

 Ost=varDecInt/CDelen[nDelen];

 if(Ost>=14) CDelen[nDelen]=CDelen[nDelen]*10;

 Ost=varDecInt/CDelen[nDelen]+1;
 ThisOs.CenaDel=CDelen[nDelen];
 ThisOs.KolvoDel=Ost;

 RVar=Ost*CDelen[nDelen];

 for(i=0;i<nDec;i++)
 {
  RVar=RVar/Dec;
  ThisOs.CenaDel=ThisOs.CenaDel/Dec;
 }
 ThisOs.MaxZnach=RVar;
 return ThisOs;
}
