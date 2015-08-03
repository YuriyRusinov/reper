#include "dlgspecgraph.h"
#include "ui_dlgspecgraph.h"

DlgSpecGraph::DlgSpecGraph(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSpecGraph)
{
 QTextCodec::setCodecForTr(QTextCodec::codecForName("system"));

 ui->setupUi(this);

 this->DLeft=50;
 this->DBottom=30;
 this->Ch=0;

 this->XRec1=this->DLeft;
 this->YRec1=120;
 this->XRec2=this->width()-this->XRec1;
 this->YRec2=this->height()-this->DBottom;

 this->WGraph=this->XRec2-this->XRec1;
 this->HGraph=this->YRec2-this->YRec1;

 ui->comboTypeValue->addItem(tr("Спектральная яркость"));
 ui->comboTypeValue->addItem(tr("Среднее квадратичное отклонение"));
 ui->comboTypeValue->addItem(tr("Энтропия"));
 ui->comboTypeValue->setCurrentIndex(0);
 this->CurrentVal=0;
 this->CurrentGraph=-1;
 QRegExp rx("[0-9]{0,10}");
 ui->EditKDiskr->setValidator(new QRegExpValidator(rx,this));
 ui->EditKDiskr->setText("100");


}

DlgSpecGraph::~DlgSpecGraph()
{
    delete ui;
}

/***События***/
void DlgSpecGraph::paintEvent(QPaintEvent *pe)
{
 QPainter Painter;

 if(!this->isHidden())
 {
  Painter.begin(this);
  Painter.setPen(QPen(QColor(0,0,0),3));
  Painter.drawLine(this->XRec1,this->YRec2,this->XRec2,this->YRec2);
  Painter.drawLine(this->XRec1,this->YRec1,this->XRec1,this->YRec2);

  if(this->SpecGraphics.size()>0 && this->Ch>0)
  {
   this->MaxSpec=this->MaxThisSpec();
   this->OsX=this->Okrugl((float)this->Ch-1);
   this->OsY=this->Okrugl((float)this->MaxSpec);
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

   for(i=0;i<this->SpecGraphics.size();i++)
   {
    if(i!=this->CurrentGraph)
    {
     Painter.setPen(QPen(QColor(this->RCol[i],this->GCol[i],this->BCol[i]),2));
     for(j=0;j<this->Ch-1;j++)
     {
      x1=j*this->XKof+this->XRec1;
      x2=(j+1)*this->XKof+this->XRec1;
      y1=this->YRec2-(this->SpecGraphics[i][j]*this->YKof);
      y2=this->YRec2-(this->SpecGraphics[i][j+1]*this->YKof);
      Painter.drawLine(x1,y1,x2,y2);
     }//for(j=0;j<this->Ch;j++)
    }//if(i!=this->CurrentGraph)
   }//for(i=0;i<this->SpecGraphics.size();i++)

   if(this->CurrentGraph!=-1)
   {
    i=this->CurrentGraph;
    Painter.setPen(QPen(QColor(this->RCol[i],this->GCol[i],this->BCol[i]),4));
    for(j=0;j<this->Ch-1;j++)
    {
     x1=j*this->XKof+this->XRec1;
     x2=(j+1)*this->XKof+this->XRec1;
     y1=this->YRec2-(this->SpecGraphics[i][j]*this->YKof);
     y2=this->YRec2-(this->SpecGraphics[i][j+1]*this->YKof);
     Painter.drawLine(x1,y1,x2,y2);
    }//for(j=0;j<this->Ch;j++)
   }//if(this->CurrentGraph!=-1)

  }//if(this->SpecGraphics.size()>0 && this->Ch>0)

  Painter.end();
 }//if(!this->isHidden())
}

void DlgSpecGraph::resizeEvent(QResizeEvent*)
{
 this->XRec2=this->width()-this->XRec1;
 this->YRec2=this->height()-this->DBottom;
 this->WGraph=this->XRec2-this->XRec1;
 this->HGraph=this->YRec2-this->YRec1;

 if(this->SpecGraphics.size()>0)
 {
  this->XKof=(float)this->WGraph/this->OsX.MaxZnach;
  this->YKof=(float)this->HGraph/this->OsY.MaxZnach;
 }
}

void DlgSpecGraph::closeEvent(QCloseEvent *)
{
 if(this->SpecGraphics.size()>0)
 {
  for(int i=0;i<this->SpecGraphics.size();i++)
  {
   delete[] this->GraphBrigth[i];
   delete[] this->GraphSKO[i];
   delete[] this->GraphEntrop[i];
  }//for(int i=0;i<this->SpecGraphics.size();i++)
  this->SpecGraphics.clear();
  this->GraphBrigth.clear();
  this->GraphSKO.clear();
  this->GraphEntrop.clear();
  this->ptPoly.clear();
  this->RCol.clear();
  this->GCol.clear();
  this->BCol.clear();
  ui->ListPolygon->clear();
 }//if(this->SpecGraphics.size()>0)
}

/***Слоты***/
void DlgSpecGraph::MouseDoubleCliced(int x,int y)
{
 if(!this->isHidden())
 {
  if(x>=this->ThisPoly->xn && y>=this->ThisPoly->yn && x<this->ThisPoly->xn+this->ThisPoly->W && y<this->ThisPoly->yn+this->ThisPoly->H)
  {
   QPoint pt;

   pt.setX(x);
   pt.setY(y);
   this->ptPoly<<pt;
   this->AddStackGraph(x,y);

   this->RCol<<rand()%255;
   this->GCol<<rand()%(255-this->RCol[this->RCol.size()-1]-1);
   this->BCol<<rand()%255;

   QListWidgetItem *item = new QListWidgetItem(ui->ListPolygon);
   item->setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled);
   item->setData(Qt::EditRole, "Polygon_"+QString().setNum((ulong)this->ptPoly.size()));
   item->setData(Qt::DecorationRole,QColor(this->RCol[this->RCol.size()-1],this->GCol[this->GCol.size()-1],this->BCol[this->BCol.size()-1]));

   this->update();
  }//if(x>=this->SerPoly->xn && y>=this->SerPoly->yn && x<this->SerPoly->xn+this->SerPoly->W && y<this->SerPoly->yn+this->SerPoly->H)
 }//if(!this->DlgSG->isHidden())
}

/*Поменялся тип выводимых графиков*/
void DlgSpecGraph::on_comboTypeValue_currentIndexChanged(int index)
{
 this->CurrentVal=index;

 if(this->ptPoly.size()>0)
  this->FillStackGraph();
 this->update();
}

/*Поменялся текущий выделенный график*/
void DlgSpecGraph::on_ListPolygon_currentRowChanged(int currentRow)
{
 this->CurrentGraph=currentRow;
 this->update();
}

/*Сохранить графики в файл*/
void DlgSpecGraph::on_pushButton_clicked()
{
 QTextCodec::setCodecForTr(QTextCodec::codecForName("system"));
 QString FileName;
 FileName=QFileDialog::getSaveFileName();
 int txt;
 if(!FileName.isEmpty())
 {
  txt=FileName.indexOf(".txt");
  txt=FileName.length()-txt;
  if(txt!=4)
   FileName=FileName+".txt";

  QFile FileTxt(FileName);
  QString DataString;
  FileTxt.open(QIODevice::Truncate|QIODevice::WriteOnly|QIODevice::Text);
  QTextStream out(&FileTxt);

  if(this->CurrentVal==0)
   DataString=tr("Спектральная яркость");
  if(this->CurrentVal==1)
   DataString=tr("Среднее квадратичное отклонение");
  if(this->CurrentVal==2)
   DataString=tr("Энтропия");

  out<<tr("Величина: ")<<DataString<<endl;
  out<<tr("Номер канала");

  for(int i=0;i<this->ptPoly.size();i++)
   out<<"\t"<<tr(qPrintable(ui->ListPolygon->item(i)->text()));
  out<<endl;

  for(int i=0;i<this->Ch;i++)
  {
   out<<QString().setNum(i);
   for(int j=0;j<this->ptPoly.size();j++)
   {
    out<<"\t"<<QString().setNum(this->SpecGraphics[j][i],'d',4).replace(".",",");
   }//for(int j=0;j<this->ptPoly.size();j++)
   out<<endl;
  }//for(int i=0;i<this->Ch;i++)

  FileTxt.close();

 }//if(!FileName.isEmpty())
}
/**********************************************************************************************/
ParamOs DlgSpecGraph::Okrugl(float var)
{
 float RVar;
 float varDec,Dec;
 int nDec,i,varDecInt;
 int CDelen[3],MinOst,Ost,nDelen;
 ParamOs ThisOs;

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

float DlgSpecGraph::MaxThisSpec()
{
 int i,j;
 float Max=0;
 for(i=0;i<this->SpecGraphics.size();i++)
 {
  for(j=0;j<this->Ch;j++)
  {
   if(this->SpecGraphics[i][j]>=Max)
    Max=this->SpecGraphics[i][j];
  }//for(j=0;j<this->Ch;j++)
 }//for(i=0;i<this->SpecGraphics.size();i++)
 return Max;
}

void DlgSpecGraph::FillStackGraph()
{
 int i;

 this->SpecGraphics.clear();
 for(i=0;i<this->ptPoly.size();i++)
 {
  if(this->CurrentVal==0)
   this->SpecGraphics<<this->GraphBrigth[i];

  if(this->CurrentVal==1)
   this->SpecGraphics<<this->GraphSKO[i];

  if(this->CurrentVal==2)
   this->SpecGraphics<<this->GraphEntrop[i];

 }//for(i=0;i<this->ptPoly.size();i++)

 this->Ch=this->ThisPoly->Ch;
}

void DlgSpecGraph::AddStackGraph(int x,int y)
{
 bool *MaskCh,*PolyPix;
 unsigned long long KolvoPix;

 MaskCh=new bool[this->ThisPoly->Ch];
 PolyPix=new bool[this->ThisPoly->W*this->ThisPoly->H];

 for(int i=0;i<this->ThisPoly->Ch;i++)
  MaskCh[i]=TRUE;

 if(x>=this->ThisPoly->xn && y>=this->ThisPoly->yn && x<this->ThisPoly->xn+this->ThisPoly->W && y<this->ThisPoly->yn+this->ThisPoly->H)
 {
  KolvoPix=this->ThisPoly->GetKolvoPointPoly(x,y,this->ThisPoly->ClassifMass,PolyPix);

//  this->SpecGraphics<<new float[this->ThisPoly->Ch];

  this->GraphBrigth<<new float[this->ThisPoly->Ch];
  this->GraphSKO<<new float[this->ThisPoly->Ch];
  this->GraphEntrop<<new float[this->ThisPoly->Ch];


 //Расчёт спектральных характеристик полигона
   this->ThisPoly->GetMidlSpecPoly(PolyPix,KolvoPix,this->GraphBrigth[this->GraphBrigth.size()-1],MaskCh);
   this->ThisPoly->GetSKOPoly(PolyPix,KolvoPix,this->GraphSKO[this->GraphSKO.size()-1],MaskCh);
   this->ThisPoly->GetEntropPoly(PolyPix,KolvoPix,this->GraphEntrop[this->GraphEntrop.size()-1],ui->EditKDiskr->text().toInt(),MaskCh);

  if(this->CurrentVal==0)
   this->SpecGraphics/*[this->SpecGraphics.size()-1]*/<<this->GraphBrigth[this->GraphBrigth.size()-1];

  if(this->CurrentVal==1)
   this->SpecGraphics/*[this->SpecGraphics.size()-1]*/<<this->GraphSKO[this->GraphSKO.size()-1];

  if(this->CurrentVal==2)
   this->SpecGraphics/*[this->SpecGraphics.size()-1]*/<<this->GraphEntrop[this->GraphEntrop.size()-1];


  this->Ch=this->ThisPoly->Ch;
 }//if(x>=this->ThisPoly->xn && y>=this->ThisPoly->yn && x<this->ThisPoly->xn+this->ThisPoly->W && y<this->ThisPoly->yn+this->ThisPoly->H)
 delete[] MaskCh;
 delete[] PolyPix;
}




