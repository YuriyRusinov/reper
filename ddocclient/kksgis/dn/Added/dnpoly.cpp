/*
Класс для работы с файлом спектральных данных универсального размера
Формат файла:
1. структура, в которой определены основные параметры,
2. массив полигона -1 - точка не рассматривается, 1 - точка рассматривается
3. массив координат вершин многоугольника
4. спектральные данные

x и y задаются в координатах изображения
*/

#include "dnpoly.h"

DNPoly::DNPoly(QString SerFileName)
{
 unsigned long long i;
 PolygonProp HeaderStruct;

 QPoint Point;
 int xc,yc,jx,jy;
 this->SerFileName=QObject::tr(qPrintable(SerFileName));

 QFileInfo OpenFile(SerFileName);
 this->PathTempFile=OpenFile.absolutePath();

 QFile SerFile(SerFileName);
 SerFile.open(QIODevice::ReadOnly);
 SerFile.read((char*)&HeaderStruct,sizeof(PolygonProp));

 this->xn=HeaderStruct.xn;
 this->yn=HeaderStruct.yn;
 this->W=HeaderStruct.W;
 this->H=HeaderStruct.H;
 this->Ch=HeaderStruct.KCh;
 this->KolvoPix=HeaderStruct.KPix;

 this->MassPoly=new int[W*H];
 this->ClassifMass=new int[W*H];
 this->CurrentPoly=new bool[W*H];

 for(jy=0;jy<H;jy++)
 {
  for(jx=0;jx<W;jx++)
   this->CurrentPoly[jx+jy*W]=FALSE;
 }

 SerFile.read((char*)this->MassPoly,sizeof(int)*W*H);

 for(i=0;i<this->KolvoPix;i++)
 {
  SerFile.read((char*)&xc,sizeof(int));
  SerFile.read((char*)&yc,sizeof(int));
  Point.setX(xc);
  Point.setY(yc);
  this->pt<<Point;
 }
 this->OffsetData=SerFile.pos();
 SerFile.close();

//Заполнения массива классификации
 for(jy=0;jy<H;jy++)
 {
  for(jx=0;jx<W;jx++)
  {
   this->CurrentPoly[jx+jy*W]=FALSE;
   this->ClassifMass[jx+jy*W]=-1;
   if(this->MassPoly[jx+jy*W]==1)
   {
    this->CurrentPoly[jx+jy*W]=TRUE;
    this->ClassifMass[jx+jy*W]=0;
   }
  }//for(jx=0;jx<W;jx++)
 }//for(jy=0;jy<H;jy++)

 this->px=0;
 for(i=0;i<this->W*this->H;i++)
 {
  if(this->MassPoly>=0)
   this->px++;
 }

 //Установка начальных значений
 this->IsPolyClassif=FALSE;
}
DNPoly::~DNPoly()
{
 delete[] this->MassPoly;
 delete[] this->CurrentPoly;
 this->pt.clear();

 if(this->ClassifMass!=NULL)
  delete[] this->ClassifMass;
}

/*Функции определения спектральных характеристик*/
/*Взять спектр одной точки*/
void DNPoly::GetSpectrPoint(int x,int y,float *DataSpec,bool *MaskCh)
{
 QFile SerFile(this->SerFileName);
 qint64 Offset;
 int i,ThisCh,iCh;
 float *SpecPoint;

 x=x-this->xn;
 y=y-this->yn;

 // Узнаём количество каналов интереса
 ThisCh=0;
 for(i=0;i<this->Ch;i++)
 {
  if(MaskCh[i]==TRUE)
   ThisCh++;
 }
 SpecPoint=new float[this->Ch];

 SerFile.open(QIODevice::ReadOnly);

 Offset=(x+y*this->W)*this->Ch*sizeof(float);
 SerFile.seek(this->OffsetData+Offset);
 SerFile.read((char*)SpecPoint,sizeof(float)*this->Ch);
 SerFile.close();

 iCh=0;
 for(i=0;i<this->Ch;i++)
 {
  if(MaskCh[i]==TRUE)
  {
   DataSpec[iCh]=SpecPoint[i];
   iCh++;
  }//if(MaskChan[i]==TRUE)
 }//for(i=0;i<this->Ch;i++)
 delete[] SpecPoint;
}
/*Взять спектр строки*/
void DNPoly::GetSpectrString(int y,float *DataSpec,bool *MaskCh)
{
 QFile SerFile(this->SerFileName);
 qint64 Offset;
 int i,j,ThisCh,iCh;
 float *SpecStr;
 int proba;

 y=y-this->yn;

 if(y==132)
  proba=0;

 // Узнаём количество каналов интереса
 ThisCh=0;
 for(i=0;i<this->Ch;i++)
 {
  if(MaskCh[i]==TRUE)
   ThisCh++;
 }
 SpecStr=new float[this->Ch*this->W];

 SerFile.open(QIODevice::ReadOnly);


 Offset=y*this->W*this->Ch*sizeof(float);
 SerFile.seek(this->OffsetData+Offset);
 SerFile.read((char*)SpecStr,sizeof(float)*this->W*this->Ch);
 SerFile.close();

 for(j=0;j<this->W;j++)
 {
  iCh=0;
  for(i=0;i<this->Ch;i++)
  {
   if(MaskCh[i]==TRUE)
   {
    DataSpec[iCh+j*ThisCh]=SpecStr[i+j*this->Ch];
    proba=SpecStr[i+j*this->Ch];
    iCh++;
   }//if(MaskChan[i]==TRUE)
  }//for(i=0;i<this->Ch;i++)
 }//for(j=0;j<this->W;j++)
 delete[] SpecStr;
}
/*Взять спектр участка изображения, обозначенного в массиве CurentPoly*/
void DNPoly::GetSpectrZone(float *DataSpec,bool *MaskCh)
{
 int jx,jy,i,ThisCh,iCh;
 float *SpecString,proba;
 unsigned long long ip;

 // Узнаём количество каналов интереса
 ThisCh=0;
 for(i=0;i<this->Ch;i++)
 {
  if(MaskCh[i]==TRUE)
   ThisCh++;
 }//for(i=0;i<this->Ch;i++)
 SpecString=new float[this->W*ThisCh];

 ip=0;
 for(jy=0;jy<this->H;jy++)
 {
  this->GetSpectrString(jy+this->yn,SpecString,MaskCh);
  for(jx=0;jx<this->W;jx++)
  {
   if(this->CurrentPoly[jx+jy*this->W])
   {
    iCh=0;
    for(i=0;i<this->Ch;i++)
    {
     if(MaskCh[i])
     {
      DataSpec[iCh+ip*ThisCh]=SpecString[iCh+jx*ThisCh];
      proba=DataSpec[iCh+ip*ThisCh];
      iCh++;
     }//if(MaskCh[i])
    }//for(i=0;i<this->Ch;i++)
    ip++;
   }//if(PolyPix[jx+jy*this->W])
  }//for(jx=0;jx<this->W;jx++)
 }//for(jy=0;jy<this->H;jy++)

 delete[]SpecString;
}
/*Взять яркость всех точек полигона в определённом канале*/
void DNPoly::GetBrigthChanPoly(float *DataSpec,int nCh,bool *NoBlackPixels)
{
 int jx,jy,i,xn,yn,xk,yk;
 float *SpecString;
 unsigned long long ip;
 bool *MaskCh,ynOgr=FALSE,isXTrue;

 MaskCh=new bool[this->Ch];

 for(i=0;i<this->Ch;i++)
 {
  MaskCh[i]=FALSE;
  if(i==nCh)
   MaskCh[i]=TRUE;
 }//for(i=0;i<this->Ch;i++)

 SpecString=new float[this->W];
 ip=0;
 xn=0;
 yn=0;
 xk=this->W;
 yk=this->H;
 for(jy=0;jy<this->H;jy++)
 {
  isXTrue=FALSE;
  for(jx=0;jx<this->W;jx++)
  {
   if(NoBlackPixels[jx+jy*this->W])
   {
    isXTrue=TRUE;
    if(!ynOgr)
    {
     yn=jy;
     yk=jy;
     ynOgr=TRUE;
     break;
    }
    if(ynOgr && jy>yk)
    {
     yk=jy;
     break;
    }
   }//if(NoBlackPixels[jx+jy*this->W])
  }//for(jx=0;jx<this->W;jx++)
  if(!isXTrue && ynOgr)
   break;
 }//for(jy=0;jy<this->H;jy++)

 for(jy=/*0*/yn;jy<=/*this->H*/yk;jy++)
 {
  this->GetSpectrString(jy+this->yn,SpecString,MaskCh);
  for(jx=0;jx<this->W;jx++)
  {
   if(NoBlackPixels[jx+jy*this->W])
   {
    DataSpec[ip]=SpecString[jx];
    ip++;
   }//if(PolyPix[jx+jy*this->W])
  }//for(jx=0;jx<this->W;jx++)
 }//for(jy=0;jy<this->H;jy++)
 delete[]SpecString;
 delete[]MaskCh;
}
/*Взять яркость в канале пямоугольника в котором находится исходеый полигон*/
void DNPoly::GetBrigthSqare(float *DataSpec,int nCh)
{
 bool *MaskChan;
 float *SpecStr;
 float proba;

 MaskChan=new bool[this->Ch];
 SpecStr=new float[this->W];

 for(int i=0;i<this->Ch;i++)
 {
  MaskChan[i]=FALSE;
  if(i==nCh)
   MaskChan[i]=TRUE;
 }//for(int i=0;i<this->Ch;i++)

 for(int jy=0;jy<this->H;jy++)
 {
  this->GetSpectrString(jy+this->yn,SpecStr,MaskChan);
  for(int jx=0;jx<this->W;jx++)
  {
   DataSpec[jx+jy*this->W]=SpecStr[jx];
   if(DataSpec[jx+jy*this->W]!=255)
    proba=0;
  }//for(int jx=0;jx<this->W;jx++)
 }//for(jy=0;jy<this->H;jy++)
 delete[] SpecStr;
 delete[] MaskChan;
}
/*Умножить все значения яркости на множитель*/
void DNPoly::MultiCh(float Kof)
{
 PolygonProp HeaderStruct;
 bool *MaskCh;
 float *SpecString,*SpecStringNew;


 MaskCh=new bool[this->Ch];
 SpecString=new float[this->W*this->Ch];
 SpecStringNew=new float [this->W*this->Ch];

 for(int i=0;i<this->Ch;i++)
 {
  MaskCh[i]=TRUE;
 }

 HeaderStruct.xn=this->xn;
 HeaderStruct.yn=this->yn;
 HeaderStruct.W=this->W;
 HeaderStruct.H=this->H;
 HeaderStruct.KCh=this->Ch;
 HeaderStruct.KPix=this->KolvoPix;

 QFile SerFile(this->PathTempFile+"//Temp.pol");
 SerFile.open(QIODevice::Truncate|QIODevice::WriteOnly);

 SerFile.write((char*)&HeaderStruct,sizeof(PolygonProp));
 SerFile.write((char*)this->MassPoly,sizeof(int)*W*H);

 int i=0;
 int xc,yc;
 while(i<pt.size())
 {
  xc=this->pt[i].x();
  yc=this->pt[i].y();
  SerFile.write((char*)&xc,sizeof(int));
  SerFile.write((char*)&yc,sizeof(int));
  i++;
 }//while(i<this->KolvoPixPoly)

 for(int jy=0;jy<this->H;jy++)
 {
  this->GetSpectrString(jy+this->yn,SpecString,MaskCh);
  for(int jx=0;jx<this->W;jx++)
  {
   for(int iCh=0;iCh<this->Ch;iCh++)
   {
    SpecStringNew[iCh+jx*this->Ch]=SpecString[iCh+jx*this->Ch]*Kof;
    if(SpecStringNew[iCh+jx*this->Ch]<0)
     SpecStringNew[iCh+jx*this->Ch]=0;
   }//for(int iCh=0;iCh<this->Ch;iCh++)
  }//for(int jx=0;jx<this->W;jx++)
  SerFile.write((char*)SpecStringNew,sizeof(float)*W*(this->Ch));
 }//for(int jy=0;jy<this->H;jy++)

 SerFile.close();

 QFile::remove(this->SerFileName);
 QFile::rename(this->PathTempFile+"//Temp.pol",this->SerFileName);

 delete[] MaskCh;
 delete[] SpecString;
 delete[] SpecStringNew;
}
/*Функции определения статистических параметров окружения точки*/
/*Вычислить энтропию окружения точки*/
float DNPoly::GetEntropPoint(int x,int y,int nPixels,quint64 KDiskr,int NumCh)
{
 QList <QPoint> SelectPix;
 bool *NoBlackPixels,*MaskCh;
 float EntropPix[1];
 NoBlackPixels=new bool[this->W*this->H];
 MaskCh=new bool[this->Ch];
 memset(MaskCh,0,this->Ch*sizeof(bool));
 MaskCh[NumCh]=TRUE;
 EntropPix[0]=5;
 if(this->ClassifMass[x+y*this->W]>=0)
 {
  SelectPix=this->DefinePixels(nPixels,x,y);
  memset(NoBlackPixels,0,this->W*this->H*sizeof(bool));
  for(int i=0;i<SelectPix.size();i++)
  {
   NoBlackPixels[SelectPix[i].x()+SelectPix[i].y()*this->W]=TRUE;
  }//for(int i=0;i<SelectPix.size();i++)
  this->GetEntropPoly(NoBlackPixels,SelectPix.size(),EntropPix,KDiskr,MaskCh);
//  QMessageBox msg;
//  msg.setText(QString().setNum(EntropPix[0],'d',6));
//  msg.exec();
 }//if(this->ClassifMass[x+y*this->W]>=0)
 delete NoBlackPixels;
 delete MaskCh;
 SelectPix.clear();
 return EntropPix[0];
}
/*Вычислить СКО окружения точки*/
float DNPoly::GetSKOPoint(int x,int y,int nPixels,int NumCh)
{
 QList <QPoint> SelectPix;
 bool *NoBlackPixels,*MaskCh;
 float SKOPix[1];
 NoBlackPixels=new bool[this->W*this->H];
 MaskCh=new bool[this->Ch];
 memset(MaskCh,0,this->Ch*sizeof(bool));
 MaskCh[NumCh]=TRUE;
 SKOPix[0]=5;
 if(this->ClassifMass[x+y*this->W]>=0)
 {
  SelectPix=this->DefinePixels(nPixels,x,y);
  memset(NoBlackPixels,0,this->W*this->H*sizeof(bool));
  for(int i=0;i<SelectPix.size();i++)
  {
   NoBlackPixels[SelectPix[i].x()+SelectPix[i].y()*this->W]=TRUE;
  }//for(int i=0;i<SelectPix.size();i++)
  this->GetSKOPoly(NoBlackPixels,SelectPix.size(),SKOPix,MaskCh);
 }//if(this->ClassifMass[x+y*this->W]>=0)
 delete NoBlackPixels;
 delete MaskCh;
 SelectPix.clear();
 return SKOPix[0];
}
/*Функции работы с полигонами*/
/*Выделить полигон к которому принадлежит заданная точка (получить массив NoBlackPixels)*/
quint64 DNPoly::GetKolvoPointPoly(int x,int y,int *MassIsh,bool *NoBlackPixels)
{
 bool *Prover;
 QPoint Pix1,Pix2;
 int i,j_x,j_y;
 int FirstPix,AtherPix;
 int xP2,yP2,NullB;
 quint64 KolvoPix;
 QQueue <QPoint> PixSteck;


 x=x-this->xn;
 y=y-this->yn;

 for(j_y=0;j_y<this->H;j_y++)
 {
  for(j_x=0;j_x<this->W;j_x++)
   this->CurrentPoly[j_x+j_y*W]=FALSE;
 }

 Prover=new bool[this->W*this->H];
 KolvoPix=0;
 for(j_y=0;j_y<this->H;j_y++)
 {
  for(j_x=0;j_x<this->W;j_x++)
  {
   NoBlackPixels[j_x+j_y*this->W]=FALSE;
   Prover[j_x+j_y*this->W]=FALSE;
   //MassZone[j_x+j_y*this->W]=-1;
  }//for(j_x=0;j_x<this->W;j_x++)
 }//for(j_y=0;j_y<this->H;j_y++)

 FirstPix=MassIsh[x+y*this->W];
 Pix1.setX(x);
 Pix1.setY(y);
 NoBlackPixels[x+y*this->W]=TRUE;
 this->CurrentPoly[x+y*this->W]=TRUE;
 Prover[x+y*this->W]=TRUE;
 PixSteck<<Pix1;

 while(PixSteck.size()>0)
 {
  Pix1=PixSteck[PixSteck.size()-1];
  xP2=Pix1.x();
  yP2=Pix1.y();

  if(NoBlackPixels[xP2+this->W*yP2]==TRUE)
  {
   KolvoPix++;
   PixSteck.removeLast();
   for(i=0;i<4;i++)
   {
    NullB=0;
    if((i==0)&&(Pix1.x()+1<this->W)&&(NoBlackPixels[Pix1.x()+1+Pix1.y()*this->W]==FALSE)&&(Prover[Pix1.x()+1+Pix1.y()*this->W]==FALSE))
    {
     xP2=Pix1.x()+1;
     yP2=Pix1.y();
     NullB=1;
     Prover[Pix1.x()+1+Pix1.y()*this->W]=TRUE;
    }
    if((i==1)&&(Pix1.y()+1<this->H)&&(NoBlackPixels[Pix1.x()+(Pix1.y()+1)*this->W]==FALSE)&&(Prover[Pix1.x()+(Pix1.y()+1)*this->W]==FALSE))
    {
     xP2=Pix1.x();
     yP2=Pix1.y()+1;
     NullB=1;
     Prover[Pix1.x()+(Pix1.y()+1)*this->W]=TRUE;
    }
    if((i==2)&&(Pix1.x()-1>=0)&&(NoBlackPixels[Pix1.x()-1+Pix1.y()*this->W]==FALSE)&&(Prover[Pix1.x()-1+Pix1.y()*this->W]==FALSE))
    {
     xP2=Pix1.x()-1;
     yP2=Pix1.y();
     NullB=1;
     Prover[Pix1.x()-1+Pix1.y()*this->W]=TRUE;
    }
    if((i==3)&&(Pix1.y()-1>=0)&&(NoBlackPixels[Pix1.x()+(Pix1.y()-1)*this->W]==FALSE)&&(Prover[Pix1.x()+(Pix1.y()-1)*this->W]==FALSE))
    {
     xP2=Pix1.x();
     yP2=Pix1.y()-1;
     NullB=1;
     Prover[Pix1.x()+(Pix1.y()-1)*this->W]=TRUE;
    }
    if(NullB==1)
    {
     AtherPix=MassIsh[xP2+yP2*this->W];
     if(FirstPix==AtherPix)
     {
      Pix2.setX(xP2);
      Pix2.setY(yP2);
      PixSteck<<Pix2;
      NoBlackPixels[xP2+(this->W*yP2)]=TRUE;
      this->CurrentPoly[xP2+(this->W*yP2)]=TRUE;
     }//if(FirstPix==AtherPix)
    }//if(NullB==1)
   }//for(i=0;i<4;i++)
  }//if(NoBlackPixels[xP2+this->W*yP2]==TRUE)
  else
   PixSteck.removeLast();
 }//while(PixSteck.size()>0)
 delete[] Prover;
 return KolvoPix;
}
/*Статистические характеристики полигонов*/
void DNPoly::GetMidlSpecPoly(bool *NoBlackPixels,unsigned long long KolvoPixPoly,float *MidlSpecPoly,bool *MaskCh)
{
 int jx,jy,i,ThisCh,iCh;
 float *SpecString;
 unsigned long long ip;

 // Узнаём количество каналов интереса
 ThisCh=0;
 for(i=0;i<this->Ch;i++)
 {
  if(MaskCh[i]==TRUE)
  {
   MidlSpecPoly[ThisCh]=0;
   ThisCh++;
  }
 }//for(i=0;i<this->Ch;i++)
 SpecString=new float[this->W*ThisCh];

 ip=0;
 for(jy=0;jy<this->H;jy++)
 {
  this->GetSpectrString(jy+this->yn,SpecString,MaskCh);
  for(jx=0;jx<this->W;jx++)
  {
   if(NoBlackPixels[jx+jy*this->W])
   {
    iCh=0;
    for(i=0;i<this->Ch;i++)
    {
     if(MaskCh[i])
     {
      MidlSpecPoly[iCh]+=SpecString[iCh+jx*ThisCh]/KolvoPixPoly;
      iCh++;
     }//if(MaskCh[i])
    }//for(i=0;i<this->Ch;i++)
    ip++;
   }//if(PolyPix[jx+jy*this->W])
  }//for(jx=0;jx<this->W;jx++)
 }//for(jy=0;jy<this->H;jy++)

 delete[]SpecString;
}
void DNPoly::GetSKOPoly(bool *NoBlackPixels,unsigned long long KolvoPixPoly, float *SKOPoly, bool *MaskCh)
{
 int i,iCh;
 float *DataSpec;

 DataSpec=new float[KolvoPixPoly];

 iCh=0;
 for(i=0;i<this->Ch;i++)
 {
  if(MaskCh[i]==TRUE)
  {
   this->GetBrigthChanPoly(DataSpec,i,NoBlackPixels);
   SKOPoly[iCh]=this->Math.CalcSKO(DataSpec,KolvoPixPoly);
   iCh++;
  }//if(MaskCh[i]==TRUE)
 }//for(i=0;i<this->Ch;i++)

 delete[] DataSpec;

}
void DNPoly::GetEntropPoly(bool *NoBlackPixels,unsigned long long KolvoPixPoly, float *EntropPoly,quint64 KDistr,bool *MaskCh)
{
 int i,iCh;
 float *DataSpec;
 DataSpec=new float[KolvoPixPoly];

 iCh=0;
 for(i=0;i<this->Ch;i++)
 {
  if(MaskCh[i]==TRUE)
  {
   this->GetBrigthChanPoly(DataSpec,i,NoBlackPixels);
   EntropPoly[iCh]=this->Math.CalcEntropOtn(DataSpec,KDistr,KolvoPixPoly);
   iCh++;
  }//if(MaskCh[i]==TRUE)
 }//for(i=0;i<this->Ch;i++)

 delete[] DataSpec;
}
/*Функции работы с классами*/
quint64 DNPoly::GetKolvoPointClass(int NumKl,int *MassIsh,bool *NoBlackPixels)
{
 quint64 KolPoint=0;

 for(quint64 i=0;i<this->W*this->H;i++)
 {
  if(NoBlackPixels[i] && MassIsh[i]==NumKl)
  {
   KolPoint++;
  }//if(NoBlackPixels[i] && MassIsh[i]==NumKl)
 }//for(quint64 i=0;i<this->W*this->H;i++)
 return KolPoint;
}
QList <int> DNPoly::GetNumClassMass(int *MassIsh,bool *NoBlackPixels)
{
 QList <int> NumClassMass;
 int NumClass;
 bool IsNumClassNew;
 NumClassMass.clear();

 for(quint64 i=0;i<this->W*this->H;i++)
 {
  if(NoBlackPixels[i])
  {
   NumClass=MassIsh[i];
   IsNumClassNew=TRUE;

   for(int j=0;j<NumClassMass.size();j++)
   {
    if(NumClass==NumClassMass[j])
    {
     IsNumClassNew=FALSE;
     break;
    }//if(NumClass==NumClassMass[i])
   }//for(int j=0;j<NumClassMass.size();j++)
   if(IsNumClassNew)
    NumClassMass<<NumClass;
  }//if(NoBlackPixels[i] && MassIsh[i]==NumKl)
 }//for(quint64 i=0;i<this->W*this->H;i++)
 return NumClassMass;
}
void DNPoly::GetMidleSpectrClass(float *DataSpec,quint64 KPoint,int NumClass,int *MassIsh,bool *NoBlackPixels,bool *MaskCh)
{
 float *SpecString;
 //quint64 KPoint;
 // Узнаём количество каналов интереса
 int ThisCh=0;
 for(int i=0;i<this->Ch;i++)
 {
  if(MaskCh[i]==TRUE)
   ThisCh++;
 }
 for(int i=0;i<ThisCh;i++)
  DataSpec[i]=0;
 SpecString=new float[this->W*ThisCh];
 //KPoint=0;
 for(int jy=0;jy<this->H;jy++)
 {
  for(int jx=0;jx<this->W;jx++)
  {
   if(NoBlackPixels[jx+jy*this->W] && MassIsh[jx+jy*this->W]==NumClass)
   {
    for(int i=0;i<ThisCh;i++)
     DataSpec[i/*+KPoint*ThisCh*/]+=SpecString[i+jx*ThisCh]/KPoint;
   }//if(NoBlackPixels[jx+jy*this->W])
  }//for(int jx=0;jx<this->W;jx++)
 }//for(int jy=0;jy<this->H;jy++)

 delete[] SpecString;
}
/*Функции преобразования спектральных каналов*/
void DNPoly::AddChanal(QString Formula)
{
 PolygonProp HeaderStruct;
 bool *MaskCh;
 float *SpecString,*SpecStringNew,Value;
 float *SpecPoint;
 DNCalcString Cls;
 float *Res;
 QString DataStr;
 MaskCh=new bool[this->Ch];
 SpecString=new float[this->W*this->Ch];
 SpecStringNew=new float [this->W*(this->Ch+1)];
 SpecPoint=new float[this->Ch];
 for(int i=0;i<this->Ch;i++)
 {
  MaskCh[i]=TRUE;
 }

 HeaderStruct.xn=this->xn;
 HeaderStruct.yn=this->yn;
 HeaderStruct.W=this->W;
 HeaderStruct.H=this->H;
 HeaderStruct.KCh=this->Ch+1;
 HeaderStruct.KPix=this->KolvoPix;

 QFile SerFile(PathTempFile+"//Temp.pol");
 SerFile.open(QIODevice::Truncate|QIODevice::WriteOnly);

 SerFile.write((char*)&HeaderStruct,sizeof(PolygonProp));
 SerFile.write((char*)this->MassPoly,sizeof(int)*W*H);

 int i=0;
 int xc,yc;
 while(i<pt.size())
 {
  xc=this->pt[i].x();
  yc=this->pt[i].y();
  SerFile.write((char*)&xc,sizeof(int));
  SerFile.write((char*)&yc,sizeof(int));
  i++;
 }//while(i<this->KolvoPixPoly)

 int ColvoCh=Cls.FillNumChan(Formula);
 Res=new float[ColvoCh];

 for(int jy=0;jy<this->H;jy++)
 {
  this->GetSpectrString(jy+this->yn,SpecString,MaskCh);
  for(int jx=0;jx<this->W;jx++)
  {
   for(int iCh=0;iCh<ColvoCh;iCh++)
   {
    if(Cls.NumChan[iCh]>=0)
     Res[iCh]=SpecString[Cls.NumChan[iCh]+jx*this->Ch];
    if(Cls.NumChan[iCh]<0 && Cls.ChB[iCh]>=0 && Cls.ChE[iCh]>=0)
    {
        int Ch1,Ch2;
        float denf;
        Ch1=Cls.ChB[iCh];
        Ch2=Cls.ChE[iCh];
        denf=Res[iCh];
        Ch1=0;

     for(int i=0;i<this->Ch;i++)
      SpecPoint[i]=SpecString[i+jx*this->Ch];
     Res[iCh]=this->CalcMidleBrigth(Cls.ChB[iCh],Cls.ChE[iCh],SpecPoint);


    }
   }//for(int iCh=0;iCh<ColvoCh;iCh++)
   DataStr=Cls.FillStringChan(Formula,Res);
   Value=Cls.MainFunc(DataStr);
   for(int iCh=0;iCh<this->Ch;iCh++)
   {
    SpecStringNew[iCh+jx*(this->Ch+1)]=SpecString[iCh+jx*this->Ch];
   }//for(int iCh=0;iCh<this->Ch;iCh++)
   SpecStringNew[this->Ch+jx*(this->Ch+1)]=Value;
  }//for(int jx=0;jx<this->W;jx++)
  SerFile.write((char*)SpecStringNew,sizeof(float)*W*(this->Ch+1));
 }//for(int jy=0;jy<this->H;jy++)

 SerFile.close();

 QFile::remove(this->SerFileName);
 QFile::rename(PathTempFile+"//Temp.pol",this->SerFileName);

 this->Ch=this->Ch+1;

// this->SerFileName


 delete[] MaskCh;
 delete[] SpecString;
 delete[] SpecStringNew;
 delete[] SpecPoint;
 delete[] Res;
}
void DNPoly::AddChanal(float *Brigth)
{
 PolygonProp HeaderStruct;
 bool *MaskCh;
 float *SpecString,*SpecStringNew;

 MaskCh=new bool[this->Ch];
 SpecString=new float[this->W*this->Ch];
 SpecStringNew=new float [this->W*(this->Ch+1)];

  for(int i=0;i<this->Ch;i++)
  {
   MaskCh[i]=TRUE;
  }

  HeaderStruct.xn=this->xn;
  HeaderStruct.yn=this->yn;
  HeaderStruct.W=this->W;
  HeaderStruct.H=this->H;
  HeaderStruct.KCh=this->Ch+1;
  HeaderStruct.KPix=this->KolvoPix;

  QFile SerFile(this->PathTempFile+"//Temp.pol");
  SerFile.open(QIODevice::Truncate|QIODevice::WriteOnly);

  SerFile.write((char*)&HeaderStruct,sizeof(PolygonProp));
  SerFile.write((char*)this->MassPoly,sizeof(int)*W*H);

  int i=0;
  int xc,yc;
  while(i<pt.size())
  {
   xc=this->pt[i].x();
   yc=this->pt[i].y();
   SerFile.write((char*)&xc,sizeof(int));
   SerFile.write((char*)&yc,sizeof(int));
   i++;
  }//while(i<this->KolvoPixPoly)

  for(int jy=0;jy<this->H;jy++)
  {
   this->GetSpectrString(jy+this->yn,SpecString,MaskCh);
   for(int jx=0;jx<this->W;jx++)
   {
    for(int iCh=0;iCh<this->Ch;iCh++)
    {
     SpecStringNew[iCh+jx*(this->Ch+1)]=SpecString[iCh+jx*this->Ch];
    }//for(int iCh=0;iCh<this->Ch;iCh++)
    SpecStringNew[this->Ch+jx*(this->Ch+1)]=Brigth[jx+jy*this->W];
   }//for(int jx=0;jx<this->W;jx++)
   SerFile.write((char*)SpecStringNew,sizeof(float)*W*(this->Ch+1));
  }//for(int jy=0;jy<this->H;jy++)

  SerFile.close();

  QFile::remove(this->SerFileName);
  QFile::rename(this->PathTempFile+"//Temp.pol",this->SerFileName);

  this->Ch=this->Ch+1;

  // this->SerFileName


  delete[] MaskCh;
  delete[] SpecString;
  delete[] SpecStringNew;
}

void DNPoly::AddIntegralChanal(int nCh1,int nCh2)
{
 PolygonProp HeaderStruct;
 bool *MaskCh;
 float *SpecString,*SpecStringNew;
 float Brigth;

 MaskCh=new bool[this->Ch];
 SpecString=new float[this->W*this->Ch];
 SpecStringNew=new float [this->W*(this->Ch+1)];

 for(int i=0;i<this->Ch;i++)
  MaskCh[i]=TRUE;

 HeaderStruct.xn=this->xn;
 HeaderStruct.yn=this->yn;
 HeaderStruct.W=this->W;
 HeaderStruct.H=this->H;
 HeaderStruct.KCh=this->Ch+1;
 HeaderStruct.KPix=this->KolvoPix;

 QFile SerFile(this->PathTempFile+"//Temp.pol");
 SerFile.open(QIODevice::Truncate|QIODevice::WriteOnly);

 SerFile.write((char*)&HeaderStruct,sizeof(PolygonProp));
 SerFile.write((char*)this->MassPoly,sizeof(int)*W*H);

 int i=0;
 int xc,yc;
 while(i<pt.size())
 {
  xc=this->pt[i].x();
  yc=this->pt[i].y();
  SerFile.write((char*)&xc,sizeof(int));
  SerFile.write((char*)&yc,sizeof(int));
  i++;
 }//while(i<this->KolvoPixPoly)

 for(int jy=0;jy<this->H;jy++)
 {
  this->GetSpectrString(jy+this->yn,SpecString,MaskCh);
  for(int jx=0;jx<this->W;jx++)
  {
   Brigth=0;
   for(int iCh=nCh1;iCh<=nCh2;iCh++)
    Brigth+=SpecString[iCh+jx*this->Ch]/(nCh2-nCh1+1);

   for(int iCh=0;iCh<this->Ch;iCh++)
   {
    SpecStringNew[iCh+jx*(this->Ch+1)]=SpecString[iCh+jx*this->Ch];
   }//for(int iCh=0;iCh<this->Ch;iCh++)
   SpecStringNew[this->Ch+jx*(this->Ch+1)]=Brigth;
  }//for(int jx=0;jx<this->W;jx++)
  SerFile.write((char*)SpecStringNew,sizeof(float)*W*(this->Ch+1));
 }//for(int jy=0;jy<this->H;jy++)

 SerFile.close();

 QFile::remove(this->SerFileName);
 QFile::rename(this->PathTempFile+"//Temp.pol",this->SerFileName);

 this->Ch=this->Ch+1;

 // this->SerFileName


 delete[] MaskCh;
 delete[] SpecString;
 delete[] SpecStringNew;
}

void DNPoly::CreateIndexImg(QString BMPFileName,int NumCh)
{
    FILEHEADER bfhFile;
    INFOHEADER bihFile;
    RGBColor *OutBuf;
    quint16 SizeEnd;
    float KofMain;
    float MinBright,MaxBright;
    float *SerString;
    bool *MaskCh;
    quint8 *NullByte;
    quint16 BMPZnach[3];
    bool FlagZap;

//    int ProgR,ProgCV;

    SizeEnd=this->W%4;
    NullByte=new quint8[SizeEnd];
    MaskCh=new bool[this->Ch];

    for(int i=0;i<SizeEnd;i++)
     NullByte[i]=0;

    for(int i=0;i<this->Ch;i++)
    {
     MaskCh[i]=FALSE;
     if(i==NumCh)
      MaskCh[i]=TRUE;
    }

    SerString=new float[this->W];

//    ProgR=this->Heigth*2;
//    ProgCV=0;
    FlagZap=FALSE;
    for(int jy=0;jy<this->H;jy++)
    {
     this->GetSpectrString(jy+this->yn,SerString,MaskCh);
     for(int jx=0;jx<this->W;jx++)
     {
      /*Если точка первая то запоминаем её характеристики*/
      if(this->MassPoly[jx+jy*this->W]>=0 && FlagZap==FALSE)
      {
       MaxBright=SerString[jx];
       MinBright=SerString[jx];
       FlagZap=TRUE;
      }//if(jy==0 && jx==0)
      /*Если точка не первая*/
      if(this->MassPoly[jx+jy*this->W]>=0 && FlagZap==TRUE)
      {
       if(MaxBright<=SerString[jx])
        MaxBright=SerString[jx];

       if(MinBright>=SerString[jx])
        MinBright=SerString[jx];
      }//if(this->MassPoly[jx+jy*this->W]>=0 && FlagZap==TRUE)
     }//for(j=0;j<this->Width;j++)
//      ProgCV++;
//      emit ChangeProgressVal(ProgR,ProgCV);
    }//for(i=0;i<this->Height;i++)
    KofMain=255/(MaxBright-MinBright);


    /*Создание  BMP файла*/


    int den=0;
    den=sizeof(bfhFile);
    den=sizeof(bihFile);

    memset(&bfhFile,0,sizeof(bfhFile));
    memset(&bihFile,0,sizeof(bihFile));

    bfhFile.bfType=0x4D42;
    bfhFile.bfOffBits=/*sizeof(bfhFile)*/14+sizeof(bihFile);
    bfhFile.bfSize=bfhFile.bfOffBits+
                    4*(this->H*this->W)+
                    (this->H*(this->W%4));
    bihFile.biSize=sizeof(bihFile);
    bihFile.biBitCount=24;
    bihFile.biClrUsed=0;
    bihFile.biCompression=0;
    bihFile.biHeight=this->H;
    bihFile.biWidth=this->W;
    bihFile.biPlanes=1;

    QFile BMPFile(this->PathTempFile+'/'+BMPFileName);

    BMPFile.open(QIODevice::Truncate|QIODevice::WriteOnly);
    //BMPFile.write((char*)&bfhFile,/*sizeof(bfhFile)*/14);
    BMPFile.write((char*)&bfhFile.bfType,2);
    BMPFile.write((char*)&bfhFile.bfSize,4);
    BMPFile.write((char*)&bfhFile.bfReserved1,2);
    BMPFile.write((char*)&bfhFile.bfReserved2,2);
    BMPFile.write((char*)&bfhFile.bfOffBits,4);

    BMPFile.write((char*)&bihFile,sizeof(bihFile));

    OutBuf=new RGBColor[this->W];

     for(int jy=this->H-1;jy>=0;jy--)
     {
      this->GetSpectrString(jy+this->yn,SerString,MaskCh);
      for(int jx=0;jx<this->W;jx++)
      {
       if(this->MassPoly[jx+jy*this->W]>=0)
       {
        for(int iCh=0;iCh<3;iCh++)
        {
         BMPZnach[iCh]=(SerString[jx]-MinBright)*KofMain;;
         if(BMPZnach[iCh]>255)BMPZnach[iCh]=255;
        }//for(i2=0;i2<3;i2++)
       }//if(this->MassPoly[jx+jy*this->W]>=0)
       else
       {
        for(int iCh=0;iCh<3;iCh++)
         BMPZnach[iCh]=0;
       }
       OutBuf[jx].R=(quint8)BMPZnach[0];
       OutBuf[jx].G=(quint8)BMPZnach[1];
       OutBuf[jx].B=(quint8)BMPZnach[2];
      }//for(j=0;j<this->Width;j++)

      BMPFile.write((char*)OutBuf,sizeof(RGBColor)*this->W);
      BMPFile.write((char*)NullByte,sizeof(quint8)*SizeEnd);
//      ProgCV++;
//      emit ChangeProgressVal(ProgR,ProgCV);
     }//for(i=0;i<this->Height;i++)

     BMPFile.close();

     delete[] SerString;
     delete[] OutBuf;
     delete[] NullByte;
     delete[] MaskCh;
}
void DNPoly::AddEntropChanal(int nPixels,quint64 KDiskr,int NumCh)
{
 bool *MaskCh;
 float *SpecString,*SpecStringNew;
 float *SpecPoint;
 PolygonProp HeaderStruct;

 MaskCh=new bool[this->Ch];
 SpecString=new float[this->W*this->Ch];
 SpecStringNew=new float [this->W*(this->Ch+1)];
 SpecPoint=new float[this->Ch];
 for(int i=0;i<this->Ch;i++)
 {
  MaskCh[i]=TRUE;
 }
 HeaderStruct.xn=this->xn;
 HeaderStruct.yn=this->yn;
 HeaderStruct.W=this->W;
 HeaderStruct.H=this->H;
 HeaderStruct.KCh=this->Ch+1;
 HeaderStruct.KPix=this->KolvoPix;

 QFile SerFile(this->PathTempFile+"//Temp.pol");
 SerFile.open(QIODevice::Truncate|QIODevice::WriteOnly);

 SerFile.write((char*)&HeaderStruct,sizeof(PolygonProp));
 SerFile.write((char*)this->MassPoly,sizeof(int)*W*H);

 int i=0;
 int xc,yc;
 while(i<pt.size())
 {
  xc=this->pt[i].x();
  yc=this->pt[i].y();
  SerFile.write((char*)&xc,sizeof(int));
  SerFile.write((char*)&yc,sizeof(int));
  i++;
 }//while(i<this->KolvoPixPoly)

 for(int jy=0;jy<this->H;jy++)
 {
  this->GetSpectrString(jy+this->yn,SpecString,MaskCh);
  for(int jx=0;jx<this->W;jx++)
  {
   for(int iCh=0;iCh<this->Ch;iCh++)
   {
    SpecStringNew[iCh+jx*(this->Ch+1)]=SpecString[iCh+jx*this->Ch];
   }//for(int iCh=0;iCh<this->Ch;iCh++)
   SpecStringNew[this->Ch+jx*(this->Ch+1)]=this->GetEntropPoint(jx,jy,nPixels,KDiskr,NumCh)/*Value+(jx+jy)*/;
  }//for(int jx=0;jx<this->W;jx++)
  SerFile.write((char*)SpecStringNew,sizeof(float)*W*(this->Ch+1));
 }//for(int jy=0;jy<this->H;jy++)

 SerFile.close();

 QFile::remove(this->SerFileName);
 QFile::rename(this->PathTempFile+"//Temp.pol",this->SerFileName);

 this->Ch=this->Ch+1;


 delete[] SpecString;
 delete[] SpecStringNew;
 delete[] SpecPoint;
}
void DNPoly::AddSKOChanal(int nPixels, int NumCh)
{
 bool *MaskCh;
 float *SpecString,*SpecStringNew;
 float *SpecPoint;
 PolygonProp HeaderStruct;

 MaskCh=new bool[this->Ch];
 SpecString=new float[this->W*this->Ch];
 SpecStringNew=new float [this->W*(this->Ch+1)];
 SpecPoint=new float[this->Ch];
 for(int i=0;i<this->Ch;i++)
 {
  MaskCh[i]=TRUE;
 }
 HeaderStruct.xn=this->xn;
 HeaderStruct.yn=this->yn;
 HeaderStruct.W=this->W;
 HeaderStruct.H=this->H;
 HeaderStruct.KCh=this->Ch+1;
 HeaderStruct.KPix=this->KolvoPix;

 QFile SerFile(PathTempFile+"//Temp.ser");
 SerFile.open(QIODevice::Truncate|QIODevice::WriteOnly);

 SerFile.write((char*)&HeaderStruct,sizeof(PolygonProp));
 SerFile.write((char*)this->MassPoly,sizeof(int)*W*H);

 int i=0;
 int xc,yc;
 while(i<pt.size())
 {
  xc=this->pt[i].x();
  yc=this->pt[i].y();
  SerFile.write((char*)&xc,sizeof(int));
  SerFile.write((char*)&yc,sizeof(int));
  i++;
 }//while(i<this->KolvoPixPoly)

 for(int jy=0;jy<this->H;jy++)
 {
  this->GetSpectrString(jy+this->yn,SpecString,MaskCh);
  for(int jx=0;jx<this->W;jx++)
  {
   for(int iCh=0;iCh<this->Ch;iCh++)
   {
    SpecStringNew[iCh+jx*(this->Ch+1)]=SpecString[iCh+jx*this->Ch];
   }//for(int iCh=0;iCh<this->Ch;iCh++)
   SpecStringNew[this->Ch+jx*(this->Ch+1)]=this->GetSKOPoint(jx,jy,nPixels,NumCh)/*Value+(jx+jy)*/;
  }//for(int jx=0;jx<this->W;jx++)
  SerFile.write((char*)SpecStringNew,sizeof(float)*W*(this->Ch+1));
 }//for(int jy=0;jy<this->H;jy++)

 SerFile.close();

 QFile::remove(this->SerFileName);
 QFile::rename(PathTempFile+"//Temp.ser",this->SerFileName);

 this->Ch=this->Ch+1;


 delete[] SpecString;
 delete[] SpecStringNew;
 delete[] SpecPoint;
}
float DNPoly::CalcMidleBrigth(int NumCh1,int NumCh2,float *SpecData)
{
 int KolvCh=NumCh2-NumCh1+1;
 float MidleSpec;
 MidleSpec=0;
 for(int i=NumCh1;i<=NumCh2;i++)
 {
  MidleSpec+=SpecData[i]/KolvCh;
 }//for(int i=NumCh1;i<=NumCh2;i++)
 return MidleSpec;
}

/*Функции фильтрации каналов*/
/*Линейная фильтрация*/
float* DNPoly::LinearFilter(int side,float *Apert,float Kof,int NumCh)
{
 float *NewCh;
 float *Brith;
 NewCh=new float[this->W*this->H];
 Brith=new float[this->W*this->H];
 int n=side;

 for(quint64 i=0;i<this->W*this->H;i++)
  NewCh[i]=-1;

 this->GetBrigthSqare(Brith,NumCh);



 for(int y=(int)n/2;y<this->H-(int)n/2;y++)
 {
  for(int x=(int)n/2;x<this->W-(int)n/2;x++)
  {
   if(Brith[x+this->W*y]>=0)
   {
    bool BlackPix=FALSE;
    for(int xs=x-(int)n/2;xs<x+(int)n/2;xs++)
    {
     for(int ys=y-(int)n/2;ys<y+(int)n/2;ys++)
     {
      if(Brith[xs+this->W*ys]<0)
       BlackPix=TRUE;
     }//for(int ys=y-(int)n/2;ys<y+(int)n/2;y++)
    }//for(int xs=x-(int)n/2;xs<x+(int)n/2;x++)
    if(!BlackPix)
    {
     float Summ=0;
     int i=0;
     for(int xs=x-(int)n/2;xs<x+(int)n/2;xs++)
     {
      for(int ys=y-(int)n/2;ys<y+(int)n/2;ys++)
      {
       Summ+=Brith[xs+this->W*ys]*Apert[i];
       i++;
      }//for(int ys=y-(int)n/2;ys<y+(int)n/2;y++)
     }//for(int xs=x-(int)n/2;xs<x+(int)n/2;x++)
     Summ=Summ*Kof;
     NewCh[x+this->W*y]=Summ;

    }//if(!BlackPix)
   }//if(Brith[x+this->W*y]>=0)
  }//for(int x=0;x<this->W;x++)
 }//for(int y=0;y<this->H;y++)

 delete Brith;
 return NewCh;
}
/***********************************************************************************/

/*Методы селекции и выделения*/
/*Волшебная палочка*/
quint64 DNPoly::MagicWand(bool *NoBlackPixels,int x,int y,float Contrast,int NumCh, int mode,bool *NoBlackPixels2)
{
 x=x-this->xn;
 y=y-this->yn;
 quint64 KolvoPx;
 KolvoPx=-1;

/*Если заданная точка пренадлежит оцениваемому полигону*/
 if(NoBlackPixels[x+y*this->W])
 {
  float *SpectrData,CContrast;
  float FirstPix,AtherPix;
  int xP2,yP2,xc,yc;
  QQueue <QPoint> PixSteck;
  QPoint Pix1,Pix2;
  bool *Prover;

  int dx[4],dy[4];

  dx[0]=-1;
  dy[0]=0;

  dx[1]=1;
  dy[1]=0;

  dx[2]=0;
  dy[2]=-1;

  dx[3]=0;
  dy[3]=1;

  Prover=new bool[this->W*this->H];
//Установка начальных значений элементов выходного массива
  KolvoPx=0;
  for(quint64 i=0;i<this->W*this->H;i++)
  {
   NoBlackPixels2[i]=FALSE;
   Prover[i]=FALSE;
  }

//Получение яркости точек полигона в канале
  SpectrData=new float[this->W*this->H];
  this->GetBrigthSqare(SpectrData,NumCh);

//Яркость первой точки
  FirstPix=SpectrData[x+y*this->W];
  Pix1.setX(x);
  Pix1.setY(y);
  NoBlackPixels2[x+y*this->W]=TRUE;
  Prover[x+y*this->W]=TRUE;
  PixSteck<<Pix1;
  KolvoPx++;

  while(PixSteck.size()>0)
  {
   Pix1=PixSteck[PixSteck.size()-1];
   xP2=Pix1.x();
   yP2=Pix1.y();
   if(NoBlackPixels2[xP2+this->W*yP2]==TRUE)
   {
    PixSteck.removeLast();
    for(int i=0;i<4;i++)
    {
     xc=xP2+dx[i];
     yc=yP2+dy[i];
     if(xc>=0 && xc<this->W && yc>=0 && yc<this->H && Prover[xc+yc*this->W]==FALSE && NoBlackPixels2[xc+yc*this->W]==FALSE && NoBlackPixels[xc+yc*this->W]==TRUE)
     {
      do{
         AtherPix=SpectrData[xc+yc*this->W];
         Prover[xc+yc*this->W]=TRUE;
         //Если считаем по контрасту с первой точкой
         if(mode==0)
         {
          CContrast=(AtherPix-FirstPix)/(AtherPix+FirstPix);
         }//if(mode==0)
         //Если считаем по яркости
         if(mode==1)
         {
          CContrast=AtherPix;
         }//if(mode==1)
         //Если считаем по контрасту смежных точек
         if(mode==2)
         {
          CContrast=(AtherPix-FirstPix)/(AtherPix+FirstPix);
          FirstPix=AtherPix;
         }//if(mode==0)
         //Если условие выполняется записываем точку в стек
         if(fabs(CContrast)<=fabs(Contrast))
         {
          Pix2.setX(xc);
          Pix2.setY(yc);
          PixSteck<<Pix2;
          NoBlackPixels2[xc+(this->W*yc)]=TRUE;
          KolvoPx++;
         }
         xc+=dx[i];
         yc+=dy[i];
        }while((xc>=0 && xc<this->W && yc>=0 && yc<this->H) &&
               Prover[xc+yc*this->W]==FALSE &&
               NoBlackPixels2[xc+yc*this->W]==FALSE &&
               NoBlackPixels[xc+yc*this->W]==TRUE &&
               fabs(CContrast)<=fabs(Contrast));
     }//if(xc>=0 && xc<this->W && yc>=0 && yc<this->H && Prover[xc+yc*this->W]==FALSE)
    }//if(NoBlackPixels2[xP2+this->W*yP2]==TRUE)
   }//if(NoBlackPixels[xP2+this->W*yP2]==TRUE)
   else
    PixSteck.removeLast();
  }//while(PixSteck.size()>0)
  delete[] SpectrData;
  delete[] Prover;

 }//if(NoBlackPixels[x+y*this->W])

//Заполнение массива классификации
 if(KolvoPx>0)
 {
  this->IsPolyClassif=TRUE;

  if(this->ClassifMass!=NULL)
   delete[] this->ClassifMass;

  this->ClassifMass=new int[this->W*this->H];

  for(quint64 i=0;i<this->W*this->H;i++)
  {
   this->ClassifMass[i]=-1;

   if(NoBlackPixels2[i]==TRUE)
    this->ClassifMass[i]=1;
  }//for(quint64 i=0;i<this->W*this->H;i++)
 }
 return KolvoPx;
}
/*IsoData*/
int DNPoly::IsoData(bool *NoBlackPixels,int KolvoClass,float MaxSKO,float Qc,int I, bool *MaskCh)
{
 DNMathAdd ThisMath;
 quint64 KPix;
 int ThisCh,KolvoI;
 IsoDataClassProp ClassOne;
 IsoDataMetaClass MetaOne;
 QList <IsoDataClassProp> Classes;
 QList <IsoDataMetaClass> Metas;
 QMap <int,int> DeleteKlProp;
 float *SpectrString;
 float *Mass1,Dsr;
 bool IsClassDual;
 int k=0;

 int ProgR,ProgCV;

 //Определяем количество анализируемых точек
 KPix=0;
 ClassOne.Dsr=0;
 ClassOne.KPix=0;
 ClassOne.Objed=FALSE;
 for(quint64 i=0;i<this->W*this->H;i++)
 {
  this->ClassifMass[i]=-1;
  if(NoBlackPixels[i])
  {
   KPix++;
   this->ClassifMass[i]=0;
  }
 }//for(quint64 i=0;i<this->W*this->H;i++)

 //Определяем количество анализируемых каналов
 ThisCh=0;
 for(int i=0;i<this->Ch;i++)
 {
  if(MaskCh[i])
   ThisCh++;
 }//for(int i=0;i<this->Ch;i++)

 SpectrString=new float[this->W*ThisCh];
 Mass1=new float[ThisCh];

 //Задаём начальные условия
 Classes<<ClassOne;
 Classes[0].Zk=new float[ThisCh];
 Classes[0].SKO=new float[ThisCh];
 KolvoI=0; //Количество итераций

 ProgR=3*I+1;
 ProgCV=0;
 emit ChangeProgressVal(ProgR,ProgCV);
 do
 {
  do //}while(IsClassDual);
  {
   KolvoI++;
   if(Classes.size()==1)
   {
    ProgCV++;
    emit ChangeProgressVal(ProgR,ProgCV);
   }//if(Classes.size()==1)
   if(Classes.size()>1)
   {
    for(int i=0;i<Classes.size();i++)
     Classes[i].KPix=0;

//Шаг 2 Распределение точек согласно центрам кластеров
    for(int jy=0;jy<this->H;jy++)
    {
     this->GetSpectrString(jy+this->yn,SpectrString,MaskCh);
     for(int jx=0;jx<this->W;jx++)
     {
      if(NoBlackPixels[jx+jy*this->W])
      {
       for(int iCh=0;iCh<ThisCh;iCh++)
       {
        Mass1[iCh]=SpectrString[iCh+jx*ThisCh];
       }//for(int iCh=0;iCh<ThisCh;iCh++)
    //Вычисление расстояний между точкой и центром кластеров
       float Dmin,D;
       int NumKl=0;
       Dmin=ThisMath.CalcEvklid(Mass1,Classes[0].Zk,ThisCh);
       for(int i=1;i<Classes.size();i++)
       {
        D=ThisMath.CalcEvklid(Mass1,Classes[i].Zk,ThisCh);
        if(D<Dmin)
        {
         Dmin=D;
         NumKl=i;
        }//if(D<Dmin)
       }//for(int i=0;i<Classes.size();i++)
       this->ClassifMass[jx+jy*this->W]=NumKl;
       Classes[NumKl].KPix++;
      }//if(NoBlackPixels[jx+jy*this->W])
     }//for(int jx=0;jx<this->xn+this->W;jx++)
    }//for(int jy=this->yn;jy<this->yn+this->H;jy++)

    ProgCV++;
    emit ChangeProgressVal(ProgR,ProgCV);

//Шаг 3 удаляем кластеры с нулевыми пикселями
    k=0;
    int KolvoDelKl=0;
    int k2=0;
    DeleteKlProp.clear();
    while(k<Classes.size())
    {
     if(Classes[k].KPix>0)
      k++;
     else
     {
      Classes.removeAt(k);
      KolvoDelKl++;
     }
     DeleteKlProp[k2]=KolvoDelKl;
     k2++;
    }//while(k<Classes.size())
   }//if(Classes.size()>1)

    int KKlass;
    KKlass=Classes.size();

   //Обнуляем всю информацию о кластерах
   for(int i=0;i<Classes.size();i++)
   {
    Classes[i].KPix=0;
    Classes[i].Dsr=0;
    Classes[i].Objed=FALSE;
    for(int iCh=0;iCh<ThisCh;iCh++)
    {
     Classes[i].Zk[iCh]=0;
     Classes[i].SKO[iCh]=0;
    }//
   }//for(int i=0;i<Classes.size();i++)

//Шаг 4 Рассчёт центра кластеров
   //Начинаем рассчёт
   for(int jy=0;jy<this->H;jy++)
   {
    this->GetSpectrString(jy+this->yn,SpectrString,MaskCh);
    for(int jx=0;jx<this->W;jx++)
    {
     if(NoBlackPixels[jx+jy*this->W])
     {
      int NumKl;
      //Преобразование массива классификации в связи с удалёнными кластерами
      this->ClassifMass[jx+jy*this->W]=this->ClassifMass[jx+jy*this->W]-DeleteKlProp[this->ClassifMass[jx+jy*this->W]];
      NumKl=this->ClassifMass[jx+jy*this->W];
      Classes[NumKl].KPix++;
      for(int iCh=0;iCh<ThisCh;iCh++)
      {
       Classes[NumKl].Zk[iCh]+=SpectrString[iCh+jx*ThisCh];
      }//for(int iCh=0;iCh<ThisCh;iCh++)
     }//if(NoBlackPixels[jx+jy*this->W])
    }//for(int jx=0;jx<this->xn+this->W;jx++)
   }//for(int jy=0;jy<this->H;jy++)

   ProgCV++;
   emit ChangeProgressVal(ProgR,ProgCV);

   for(int i=0;i<Classes.size();i++)
   {
    for(int iCh=0;iCh<ThisCh;iCh++)
    {
     Classes[i].Zk[iCh]=Classes[i].Zk[iCh]/Classes[i].KPix;
    }//for(int iCh=0;iCh<ThisCh;iCh++)
   }//for(int i=0;i<Classes.size();i++)

//Шаг 5 Вычисляется среднее расстояние между точками класса и его центром
   for(int jy=0;jy<this->H;jy++)
   {
    this->GetSpectrString(jy+this->yn,SpectrString,MaskCh);
    for(int jx=0;jx<this->W;jx++)
    {
     if(NoBlackPixels[jx+jy*this->W])
     {
      int NumKl;
      NumKl=this->ClassifMass[jx+jy*this->W];
      for(int iCh=0;iCh<ThisCh;iCh++)
      {
       Mass1[iCh]=SpectrString[iCh+jx*ThisCh];
      }//for(int iCh=0;iCh<ThisCh;iCh++)
      Classes[NumKl].Dsr+=ThisMath.CalcEvklid(Classes[NumKl].Zk,Mass1,ThisCh)/Classes[NumKl].KPix;
     }//if(NoBlackPixels[jx+jy*this->W])
    }//for(int jx=0;jx<this->xn+this->W;jx++)
   }//for(int jy=0;jy<this->H;jy++)

   ProgCV++;
   emit ChangeProgressVal(ProgR,ProgCV);

//Шаг 6 Вычисляется общее среднее расстояние между центрами кластеров
   Dsr=0;
   for(int i=0;i<Classes.size();i++)
   {
    Dsr+=(Classes[i].Dsr*Classes[i].KPix)/KPix;
   }//for(int i=0;i<Classes.size();i++)

   if(KolvoI==I)
    Qc=0;
   IsClassDual=FALSE;

   if(KolvoI<I && Classes.size()<=KolvoClass/2 || ((KolvoI)%2==1 && Classes.size()<=KolvoClass/2))
   {
//Шаг 8 Вычисляем СКО для каждого кластера
    for(int jy=0;jy<this->H;jy++)
    {
     this->GetSpectrString(jy+this->yn,SpectrString,MaskCh);
     for(int jx=0;jx<this->W;jx++)
     {
      if(NoBlackPixels[jx+jy*this->W])
      {
       int NumKl;
       NumKl=this->ClassifMass[jx+jy*this->W];
       for(int iCh=0;iCh<ThisCh;iCh++)
       {
        Classes[NumKl].SKO[iCh]+=((SpectrString[iCh+jx*ThisCh]-Classes[NumKl].Zk[iCh])*(SpectrString[iCh+jx*ThisCh]-Classes[NumKl].Zk[iCh]))/ Classes[NumKl].KPix;
       }//for(int iCh=0;iCh<ThisCh;iCh++)
      }//if(NoBlackPixels[jx+jy*this->W])
     }//for(int jx=0;jx<this->xn+this->W;jx++)
    }//for(int jy=0;jy<this->H;jy++)

//Шаг 9 Максимальное значение компоненты СКО
    for(int i=0;i<Classes.size();i++)
    {
     Classes[i].SKOMax=Classes[i].SKO[0];
     Classes[i].NumChSKOMax=0;
     for(int iCh=0;iCh<ThisCh;iCh++)
     {
      if(Classes[i].SKO[iCh]>Classes[i].SKOMax)
      {
       Classes[i].SKOMax=Classes[i].SKO[iCh];
       Classes[i].NumChSKOMax=iCh;
      }//if(Classes[i].SKO[iCh]>Classes[i].SKOMax)
     }//for(int iCh=0;iCh<ThisCh;iCh++)
    }//for(int i=0;i<Classes.size();i++)

//Шаг 10 Расщепление кластеров
    k=Classes.size();
    for(int i=0;i<k;i++)
    {
     if(Classes[i].SKOMax>MaxSKO && (Classes[i].Dsr>Dsr || Classes.size()<=KolvoClass/2) && Classes[i].KPix>0)
     {
      //Корректировка координат центра старого кластера
      Classes[i].KPix=0;
      Classes[i].Dsr=0;
      Classes[i].SKOMax=0;
      Classes[i].Zk[Classes[i].NumChSKOMax]=Classes[i].Zk[Classes[i].NumChSKOMax]+0.5*Classes[i].Zk[Classes[i].NumChSKOMax];

      //Создание нового кластера
      Classes<<ClassOne;
      Classes[Classes.size()-1].Zk=new float[ThisCh];
      Classes[Classes.size()-1].SKO=new float[ThisCh];

      for(int iCh=0; iCh<ThisCh; iCh++)
      {
       Classes[Classes.size()-1].Zk[iCh]=Classes[i].Zk[iCh];
       Classes[Classes.size()-1].SKO[iCh]=0;
      }
      Classes[Classes.size()-1].Zk[Classes[i].NumChSKOMax]=Classes[i].Zk[Classes[i].NumChSKOMax]-0.5*Classes[i].Zk[Classes[i].NumChSKOMax];
      IsClassDual=TRUE;
     }//if(Classes[i].SKOMax>MaxSKO && (Classes[i].Dsr>Dsr || Classes.size()<=KolvoClass/2) && Classes[i].KPix>0)
    }//for(int i=0;i<Classes.size();i++)
   }//if(KolvoI+1<I && (Classes.size()<=KolvoClass/2 || (KolvoI+1)%2==1))
//   if(IsClassDual)
//    KolvoI++;

//   QMessageBox msg;
//   msg.setText(QObject::tr("Расщепление кластера: ")+QString().setNum(Classes.size()));
//   msg.exec();

  }while(IsClassDual);

//Шаг 11 Вычисляются расстояния между всеми парами центров кластеров
  Metas.clear();

  for(int i1=0;i1<Classes.size();i1++)
  {
   for(int i2=0;i2<Classes.size();i2++)
   {
    if(i2!=i1)
    {
     MetaOne.D=ThisMath.CalcEvklid(Classes[i1].Zk,Classes[i2].Zk,ThisCh);
     MetaOne.NumCl1=i1;
     MetaOne.NumCl2=i2;
     if(MetaOne.D<Qc)
      Metas<<MetaOne;
    }//if(i2!=i1)
   }//for(int i2=0;i2<Classes.size();i2++)
  }//for(int i1=0;i1<Classes.size();i1++)

//     QMessageBox msg;
//     msg.setText(QObject::tr("Объединяемые кластеры: ")+QString().setNum(Metas.size()));
//     msg.exec();

//Шаг 12 Сортировка расстояний по возрастанию
  for(int i1=0;i1<Metas.size();i1++)
  {
   for(int i2=0;i2<Metas.size()-1;i2++)
   {
    if(Metas[i2].D>Metas[i2+1].D)
    {
     MetaOne=Metas[i2];
     Metas[i2]=Metas[i2+1];
     Metas[i2+1]=MetaOne;
    }//if(Metas[i2].D>Metas[i2+1].D)
   }//for(int i2=0;i2<Metas.size()-1;i2++)
  }//for(int i1=0;i1<Metas.size();i1++)

//Шаг 13 Слияние кластеров
  for(int i=0;i<Metas.size();i++)
  {
   if(!Classes[Metas[i].NumCl1].Objed && !Classes[Metas[i].NumCl2].Objed)
   {
    Classes[Metas[i].NumCl1].Objed=TRUE;
    Classes[Metas[i].NumCl2].Objed=TRUE;

    ClassOne.Dsr=0;
    ClassOne.KPix=0;
    ClassOne.Objed=FALSE;
    Classes<<ClassOne;
    Classes[Classes.size()-1].Zk=new float[ThisCh];
    Classes[Classes.size()-1].SKO=new float[ThisCh];
    for(int iCh=0;iCh<ThisCh;iCh++)
    {
     Classes[Classes.size()-1].Zk[iCh]=(1/(Classes[Metas[i].NumCl1].KPix+Classes[Metas[i].NumCl2].KPix))*
                                       (Classes[Metas[i].NumCl1].KPix*Classes[Metas[i].NumCl1].Zk[iCh]+Classes[Metas[i].NumCl2].KPix*Classes[Metas[i].NumCl2].Zk[iCh]);
    }//for(int iCh=0;iCh<ThisCh;iCh++)
   }//if(!Classes[Metas[i].NumCl1].Objed && !Classes[Metas[i].NumCl2].Objed)
  }//for(int i=0;i<Metas.size();i++)

 //Удаление кластеров которые были объеденены в один
  k=0;
  int k3=0;
  while(k<Classes.size())
  {
   if(!Classes[k].Objed)
    k++;
   else
   {
    Classes.removeAt(k);
    k3++;
   }
  }
//  KolvoI++;

//  QMessageBox msg2;
//  msg2.setText(QObject::tr("Текущее число кластеров: ")+QString().setNum(Classes.size()));
//  msg2.exec();
  if(KolvoI/*+1*/==I)
  {
   ProgCV++;
   emit ChangeProgressVal(ProgR,ProgCV);
  }

 }while(KolvoI/*+1*/<I);
 delete[] Mass1;
 delete[] SpectrString;
 this->IsPolyClassif=TRUE;
 k=Classes.size();

 for(int i=0;i<Classes.size();i++)
 {
  //QMessageBox msg;
  //msg.setText(QString().setNum(Classes[i].KPix));
  //msg.exec();
  delete[] Classes[i].SKO;
  delete[] Classes[i].Zk;
 }
 Classes.clear();
 return k;
}
/*Метод спектрального угла*/
int DNPoly::SAM(bool *NoBlackPixels,QStringList PolyFileNames,bool IsSKOEnabled,float LimitSKO,bool *MaskCh)
{
 DNPoly *ThisPoly;

 QList <float*> MLam;
 QList <float> StandOAngel;
// QList <float> ModVector;
 int KolvoCh;
 quint64 KolPoint;
 float SKO;
 KolvoCh=0;

 int ProgR,ProgCV;

 //Определение количества каналов
 for(int i=0; i<this->Ch;i++)
 {
  if(MaskCh[i])
   KolvoCh++;
 }

 ProgR=PolyFileNames.size()+this->H;
 ProgCV=0;
 emit ChangeProgressVal(ProgR,ProgCV);

 //Открытие заданных полигонов и определение ключевых параметров
 for(int i=0;i<PolyFileNames.size();i++)
 {
  MLam<<new float[KolvoCh];
  ThisPoly=new DNPoly(PolyFileNames[i]);

  if(ThisPoly->Ch==this->Ch)
  {
   //Определение количества точек в полигоне
   KolPoint=0;
   for(quint64 ip=0;ip<ThisPoly->W*ThisPoly->H;ip++)
   {
    if(ThisPoly->CurrentPoly[ip])
     KolPoint++;
   }//for(quint64 ip=0;ip<ThisPoly->W*ThisPoly->H;ip++)

   //Получение спектральных характеристик полигона
   float *DataSpec;
   float *SpecPoint;
   float *PointsAngle;
   DataSpec=new float[KolPoint*KolvoCh];
   SpecPoint=new float[KolvoCh];
   PointsAngle=new float[KolPoint];
   ThisPoly->GetSpectrZone(DataSpec,MaskCh);

   //Расчёт средних значений яркостей точек полигона в каждом канале
   for(int iCh=0;iCh<KolvoCh;iCh++)
   {
    MLam[i][iCh]=0;
    for(quint64 ip=0;ip<KolPoint;ip++)
    {
     MLam[i][iCh]+=DataSpec[iCh+ip*KolvoCh]/KolPoint;
    }//for(quint64 ip=0;ip<KolPoint;ip++)
   }//for(int iCh=0;iCh<KolvoCh;iCh++)

  //Рассчёт углов в яркостном пространстве между вектором средних значений и каждой точкой
   if(IsSKOEnabled)
   {
    for(quint64 ip=0;ip<KolPoint;ip++)
    {
     for(int iCh=0;iCh<KolvoCh;iCh++)
     {
      SpecPoint[iCh]=DataSpec[iCh+ip*KolvoCh];
     }//for(int iCh=0;iCh<KolvoCh;iCh++)
     PointsAngle[ip]=Math.CalcSpecAngle(KolvoCh,SpecPoint,MLam[i]);
    }//for(quint64 ip=0;ip<KolPoint;ip++)

    //Расчёт СКО углов
    SKO=Math.CalcSKO(PointsAngle,KolPoint);
    StandOAngel<<(float)sqrt(SKO*KolPoint/(KolPoint-1));
    //proba=SKOAngel[i];
    if(StandOAngel[i]>3.14*LimitSKO/180 && LimitSKO>0) StandOAngel[i]=LimitSKO*3.14/180;
   }//if(IsSKOEnabled)
   delete[] DataSpec;
   delete[] SpecPoint;
   delete[] PointsAngle;
  }//if(ThisPoly->Ch==this->Ch)
  else
  {
   QMessageBox msg;
   msg.setText(tr("Количество каналов эталона\n")+PolyFileNames[i]+"\n"+tr("Несоответствует классифицируемому изображению"));
   msg.exec();
  }
  delete ThisPoly;
  ProgCV++;
  emit ChangeProgressVal(ProgR,ProgCV);
 }//for(int i=0;i<PolyFileNames.size();i++)

 //Классификация точек текущего полигона
 float *SpecString;
 float *Mass;
 float Angle,pi,MaxPi,MinAngle;
 SpecString=new float[this->W*KolvoCh];
 Mass=new float[KolvoCh];
 for(int jy=0;jy<this->H;jy++)
 {
  this->GetSpectrString(jy+this->yn,SpecString,MaskCh);
  for(int jx=0;jx<this->W;jx++)
  {
   this->ClassifMass[jx+jy*this->W]=-1;
   if(NoBlackPixels[jx+jy*this->W])
   {
    for(int iCh=0;iCh<KolvoCh;iCh++)
    {
     Mass[iCh]=SpecString[iCh+jx*KolvoCh];
    }//for(int iCh=0;iCh<KolvoCh;iCh++)
    MaxPi=-1;
    MinAngle=5.;
    for(int i=0;i<MLam.size();i++)
    {
     Angle=Math.CalcSpecAngle(KolvoCh,Mass,MLam[i]);
     if(IsSKOEnabled)
     {
      pi=(1/(StandOAngel[i]*sqrt(2*3.14)))*exp((-1)*Angle/(2*StandOAngel[i]*StandOAngel[i]));
      if(pi>MaxPi)
      {
       MaxPi=pi;
       this->ClassifMass[jx+jy*this->W]=i;
      }//if(pi>MaxPi)
     }//if(IsSKOEnabled)
     else
     {
      if(MinAngle>Angle)
      {
       MinAngle=Angle;
       this->ClassifMass[jx+jy*this->W]=i;
      }//if(MinAngle>Angle)
     }
    }//for(i=0;i<MLam.size();i++)
   }//if(NoBlackPixels[jx+jy*this->W])
  }//for(int jx=0;jx<this->W;jx++)
  ProgCV++;
  emit ChangeProgressVal(ProgR,ProgCV);
 }//for(int jy=0;jy<this->H;jy++)

 this->IsPolyClassif=TRUE;

 for(int i=0;i<MLam.size();i++)
  delete[] MLam[i];
 MLam.clear();
 StandOAngel.clear();
 delete[] SpecString;
 delete[] Mass;
 return PolyFileNames.size();
}

/*Фильтр*/
void DNPoly::FilterPix(bool *NoBlackPixels,QString Formula,int Usl, float Value2)
{
 QString DataStr;
 DNCalcString Cls;
 float *Res,Value;
 int ColvoCh;

 float *SpecString;
 bool *MaskCh;

 int ProgR,ProgCV;

 MaskCh=new bool[this->Ch];
 SpecString=new float[this->Ch*this->W];

 for(int i=0;i<this->Ch;i++)
  MaskCh[i]=TRUE;

 ColvoCh=Cls.FillNumChan(Formula);
 Res=new float[ColvoCh];

 ProgR=this->H;
 ProgCV=0;
 emit ChangeProgressVal(ProgR,ProgCV);

 for(int jy=0;jy<this->H;jy++)
 {
  this->GetSpectrString(jy+this->yn,SpecString,MaskCh);
  for(int jx=0;jx<this->W;jx++)
  {
   this->ClassifMass[jx+jy*this->W]=-1;
   if(NoBlackPixels[jx+jy*this->W])
   {
    for(int i=0;i<ColvoCh;i++)
    {
     Res[i]=SpecString[Cls.NumChan[i]+jx*this->Ch];
    }//for(int i=0;i<ColvoCh;i++)
    DataStr=Cls.FillStringChan(Formula,Res);
    Value=Cls.MainFunc(DataStr);
    if((Usl==0 && Value<Value2) ||(Usl==1 && Value>Value2))
     this->ClassifMass[jx+jy*this->W]=0;
    else
     this->ClassifMass[jx+jy*this->W]=-1;
   }//if(this->ThisPoly->ClassifMass[jx+jy*this->ThisPoly->W]>=0)
  }//for(int jx=0;jx<this->ThisPoly->W;jx++)
  ProgCV++;
  emit ChangeProgressVal(ProgR,ProgCV);
 }//for(int jy=0;jy<this->ThisPoly->H;jy++)
 this->IsPolyClassif=TRUE;

 delete[] Res;
 delete[] MaskCh;
 delete[] SpecString;
}
void DNPoly::FilterPoly(int *MassIsh,QString Formula,int Usl, float Value2)
{
 bool *ProverPix;
 bool *NoBlackPixels;
 bool *MaskCh;
 float *SpecString;
 float *Res;
 float Value;
 DNCalcString Cls;
 QString DataStr;
 QString Function,ParamStr;
 int param;
 int CommaPos;
 int ColvoCh;
 QList <float> VarsPoly;
 int ProgR,ProgCV;

 Function="";
 if(Formula.indexOf("Entrop(")>=0)
 {
  Function="Entrop";
  Formula.remove("Entrop(");
  CommaPos=Formula.indexOf(',');
  ParamStr=Formula.right(Formula.length()-CommaPos);
  ParamStr.remove(')');
  ParamStr.remove(',');
  param=ParamStr.toInt();
  Formula=Formula.left(CommaPos);
 }
 if(Formula.indexOf("SKO(")>=0)
 {
  Function="SKO";
  Formula.remove("SKO(");
  Formula.remove(')');
 }

 ProverPix=new bool[this->W*this->H];
 NoBlackPixels=new bool[this->W*this->H];
 MaskCh=new bool[this->Ch];
 SpecString=new float[this->Ch*this->W];

 ColvoCh=Cls.FillNumChan(Formula);
 Res=new float[ColvoCh];

 for(int i=0;i<this->Ch;i++)
  MaskCh[i]=TRUE;

 for(quint64 i=0;i<this->W*this->H;i++)
 {
  ProverPix[i]=FALSE;
  NoBlackPixels[i]=FALSE;
  this->ClassifMass[i]=-1;
 }

 ProgR=this->H;
 ProgCV=0;
 emit ChangeProgressVal(ProgR,ProgCV);

 for(int jy=0;jy<this->H;jy++)
 {
  for(int jx=0;jx<this->W;jx++)
  {
   if(!ProverPix[jx+jy*this->W] && MassIsh[jx+jy*this->W]>=0)
   {
    this->GetKolvoPointPoly(jx+this->xn,jy+this->yn,MassIsh,NoBlackPixels);
    int yMin,yMax;
    yMin=this->H;
    yMax=0;
    for(int jy2=0;jy2<this->H;jy2++)
    {
     for(int jx2=0;jx2<this->W;jx2++)
     {
      if(NoBlackPixels[jx2+jy2*this->W])
      {
       if(yMin>jy2)
        yMin=jy2;
       if(yMax<jy2)
        yMax=jy2;
      }
     }//for(int jx2=0;jx2<this->W;jx2++)
    }//for(int jy2=0;jy2<this->H;jy2++)

    VarsPoly.clear();
    for(int jy2=/*0*/yMin;jy2<=yMax;jy2++)
    {
     this->GetSpectrString(jy2+this->yn,SpecString,MaskCh);
     for(int jx2=0;jx2<this->W;jx2++)
     {
      if(!ProverPix[jx2+jy2*this->W] && NoBlackPixels[jx2+jy2*this->W] && MassIsh[jx2+jy2*this->W]>=0)
      {
       for(int i=0;i<ColvoCh;i++)
       {
        Res[i]=SpecString[Cls.NumChan[i]+jx2*this->Ch];
       }//for(int i=0;i<ColvoCh;i++)

       DataStr=Cls.FillStringChan(Formula,Res);
       VarsPoly<<Cls.MainFunc(DataStr);

       ProverPix[jx2+jy2*this->W]=NoBlackPixels[jx2+jy2*this->W];
      }//if(!ProverPix[i] && NoBlackPixels[i])
     }//for(int jx2=0;jx2<this->W;jx2++)
    }//for(int jy2=0;jy2<this->H;jy2++)
    Value=0;

    if(Function=="")
    {
     for(int i=0;i<VarsPoly.size();i++)
      Value+=VarsPoly[i]/VarsPoly.size();
    }
    if(Function=="Entrop" || Function=="SKO")
    {
     float *Mass;
     Mass=new float[VarsPoly.size()];

     for(int i=0;i<VarsPoly.size();i++)
      Mass[i]=VarsPoly[i];

     if(Function=="Entrop")
      Value=this->Math.CalcEntropOtn(Mass,param,VarsPoly.size());

     if(Function=="SKO")
      Value=this->Math.CalcSKO(Mass,VarsPoly.size());

     delete[] Mass;
    }


    if((Usl==0 && Value<Value2) ||(Usl==1 && Value>Value2))
    {
     for(quint64 i=0;i<this->W*this->H;i++)
     {
      if(NoBlackPixels[i])
       this->ClassifMass[i]=0;
     }//for(quint64 i=0;i<this->W*this->H;i++)
    }//if((Usl==0 && Value<Value2) ||(Usl==1 && Value>Value2))

   }//if(!ProverPix[jx+jy*this->W])
  }//for(int jy=0;jy<this->H;jy++)
  ProgCV++;
  emit ChangeProgressVal(ProgR,ProgCV);
 }//for(int jy=0;jy<this->H;jy++)

 delete[] ProverPix;
 delete[] NoBlackPixels;
 delete[] MaskCh;
 delete[] SpecString;
 delete[] Res;
}

/**********************************************************************************************************************************/
//Тематические задачи
void DNPoly::SelectWater()
{
 QList <int> NumKlasses;
 bool *NoBlackPixels,*PolyPixsels,*ClassPixels;
 bool MaskCh[3];
 float SpecClass[3];
 int *MassPolygons;
 quint64 KolvoPixClass;
 MaskCh[0]=TRUE;
 MaskCh[1]=TRUE;
 MaskCh[2]=TRUE;

 NoBlackPixels=new bool[this->H*this->W];
 PolyPixsels=new bool[this->H*this->W];
 ClassPixels=new bool[this->H*this->W];
 MassPolygons=new int[this->H*this->W];

 for(int x=0;x<this->W;x++)
 {
  for(int y=0;y<this->H;y++)
  {
   NoBlackPixels[x+y*this->W]=FALSE;
   ClassPixels[x+y*this->W]=FALSE;
   this->ClassifMass[x+y*this->W]=-1;
   MassPolygons[x+y*this->W]=-1;
   if(this->MassPoly[x+y*this->W]>=0)
    NoBlackPixels[x+y*this->W]=TRUE;
  }
 }

 this->IsoData(NoBlackPixels,20/*6*/,/*0.00001*/1,/*0.0001*/1,20,MaskCh);
 NumKlasses=this->GetNumClassMass(this->ClassifMass,NoBlackPixels);
 for(int i=0;i<NumKlasses.size();i++)
 {
  KolvoPixClass=this->GetKolvoPointClass(NumKlasses[i],this->ClassifMass,NoBlackPixels);
  this->GetMidleSpectrClass(SpecClass,KolvoPixClass,NumKlasses[i],this->ClassifMass,NoBlackPixels,MaskCh);
  if(SpecClass[1]/SpecClass[2]>/*1.65*/1.85/* && SpecClass[1]/SpecClass[2]</*1.7*//*1.9*/)
  {
   for(int x=0;x<this->W;x++)
   {
    for(int y=0;y<this->H;y++)
    {
//     NoBlackPixels[x+y*this->W]=FALSE;
     if(this->ClassifMass[x+y*this->W]==NumKlasses[i])
      ClassPixels[x+y*this->W]=TRUE;
    }//for(int y=0;y<this->H;y++)
   }//for(int y=0;y<this->H;y++)
  }//if(SpecClass[1]/SpecClass[2]>1.6 && SpecClass[1]/SpecClass[2]<1.7)
 }//for(int i=0;i<NumKlasses.size();i++)


 for(int x=0;x<this->W;x++)
 {
  for(int y=0;y<this->H;y++)
  {
   this->ClassifMass[x+y*this->W]=-1;
   if(ClassPixels[x+y*this->W])
    this->ClassifMass[x+y*this->W]=0;
  }//for(int y=0;y<this->H;y++)
 }//for(int y=0;y<this->H;y++)

/*
 KolvoPoly=0;
 int den=0;
 for(int y=0;y<this->H;y++)
 {
  for(int x=0;x<this->W;x++)
  {
   if(x==80)
    den=1;
   if(MassPolygons[x+y*this->W]<0 && ClassPixels[x+y*this->W])
   {
    KolvoPixPoly=this->GetKolvoPointPoly(x+this->xn,y+this->yn,this->ClassifMass,PolyPixsels);
    for(int y2=0;y2<this->H;y2++)
    {
     for(int x2=0;x2<this->W;x2++)
     {
      if(PolyPixsels[x2+y2*this->W])
      {
       MassPolygons[x2+y2*this->W]=KolvoPoly;
      }//if(PolyPixsels[x+y*this->W])
     }//for(int x2=0;x2<this->W;x2++)
    }//for(int y2=0;y2<this->H;y2++)

    if(KolvoPixPoly>100)
    {
     this->GetMidlSpecPoly(PolyPixsels,KolvoPixPoly,SpecPoly,MaskCh);
//     QMessageBox msg;
//     msg.setText(QString().setNum(KolvoPixPoly));
//     msg.exec();

     for(int y2=0;y2<this->H;y2++)
     {
      for(int x2=0;x2<this->W;x2++)
      {
       if(PolyPixsels[x2+y2*this->W])
       {
        if(fabs((SpecPoly[0]-SpecPoly[1])/(SpecPoly[0]+SpecPoly[1]))<0.05)
         this->ClassifMass[x2+y2*this->W]=0;
        else
         this->ClassifMass[x2+y2*this->W]=-1;
       }//if(PolyPixsels[x+y*this->W])
      }//for(int x2=0;x2<this->W;x2++)
     }//for(int y2=0;y2<this->H;y2++)
    }//if(KolvoPixPoly>20)
    else
    {
     for(int y2=0;y2<this->H;y2++)
     {
      for(int x2=0;x2<this->W;x2++)
      {
       if(PolyPixsels[x2+y2*this->W])
       {
        this->ClassifMass[x2+y2*this->W]=-1;
       }//if(PolyPixsels[x+y*this->W])
      }//for(int x2=0;x2<this->W;x2++)
     }//for(int y2=0;y2<this->H;y2++)
    }
    KolvoPoly++;
   }//if(MassPolygons[x+y*this->W]<0 && this->MassPoly[x+y*this->W]>=0)
  }//for(int x=0;x<this->W;x++)
 }//for(int y=0;y<this->H;y++)

/*
 for(int x=0;x<this->W;x++)
 {
  for(int y=0;y<this->H;y++)
  {
   this->ClassifMass[x+y*this->W]=-1;
   if(NoBlackPixels[x+y*this->W])
    this->ClassifMass[x+y*this->W]=0;
  }//for(int y=0;y<this->H;y++)
 }//for(int y=0;y<this->H;y++)
*/
 delete NoBlackPixels;
 delete PolyPixsels;
 delete ClassPixels;
 delete MassPolygons;
}
void DNPoly::Batinometr()
{
 if(this->Ch==this->IshCh)
 {
  this->AddChanal("sqrt([6]*[6]+[7]*[7]+[8]*[8])");
 }//if(this->Ch==this->IshCh)
 if(this->Ch-this->IshCh==1)
 {
  this->AddSKOChanal(10,this->Ch-1);
 }//if(this->Ch-this->IshCh==1)
 bool *MaskCh;
 QList <int> nCh;
 nCh<<3<<this->Ch-1;

 float *Brigth=new float[this->W*nCh.size()];
 MaskCh=this->FillMaskCh(nCh);

 for(int y=0;y<this->H;y++)
 {
  this->GetSpectrString(y+yn,Brigth,MaskCh);
  for(int x=0;x<this->W;x++)
  {
   this->ClassifMass[x+y*this->W]=-1;
   if(this->MassPoly[x+y*this->W]>=0)
   {
    this->ClassifMass[x+y*this->W]=1; //Всё остальное
    if(Brigth[1+x*nCh.size()]>0.01)
     this->ClassifMass[x+y*this->W]=0; //Мокрофиты
    if(Brigth[x*nCh.size()]>0.03)
     this->ClassifMass[x+y*this->W]=1; //Песок
   }//if(this->MassPoly>=0)
  }//for(quint64 i=0;i<this->W*this->H;i++)
 }//for(int y=0;y<this->H;y++)
 this->IsPolyClassif=TRUE;
 delete[] MaskCh;
 delete[] Brigth;
 nCh.clear();

 nCh<<0<<1<<2<<4<<5;
 MaskCh=this->FillMaskCh(nCh);
 Brigth=new float[this->W*nCh.size()];
 float *NewCh=new float[this->W*this->H];

 DNTheam Theam;

 for(int y=0;y<this->H;y++)
 {
  this->GetSpectrString(y+yn,Brigth,MaskCh);
  for(int x=0;x<this->W;x++)
  {
   if(this->MassPoly[x+y*this->W]>=0)
   {
    NewCh[x+y*this->W]=Theam.Batinometr(ClassifMass[x+y*this->W],
                                        Brigth[x*5+1],
                                        Brigth[x*5+2],
                                        Brigth[x*5+3],
                                        Brigth[x*5],
                                        Brigth[x*5+2],
                                        Brigth[x*5+4]);
   }//if(this->MassPoly>=0)
  }//for(quint64 i=0;i<this->W*this->H;i++)
 }//for(int y=0;y<this->H;y++)

 this->AddChanal(NewCh);
 delete[] NewCh;
 float *Apert,*NewCh2;
 int n=7;
 Apert=new float[n*n];

 for(int i=0;i<n*n;i++)
 {
  Apert[i]=1;
 }

 NewCh2=this->LinearFilter(n,Apert,(1./(n*n)),this->Ch-1);
 this->AddChanal(NewCh2);

 float HMin=15,HMax=0,Kof=1;

 for(int y=0;y<this->H;y++)
 {
  for(int x=0;x<this->W;x++)
  {
   if(NewCh2[x+y*this->W]>0 && HMin>NewCh2[x+y*this->W])
    HMin=NewCh2[x+y*this->W];

   if(HMax<NewCh2[x+y*this->W])
    HMax=NewCh2[x+y*this->W];
  }//for(int x=0;x<this->W;x++)
 }//for(int y=0;y<this->H;y++)

 if(HMax!=HMin)
  Kof=(HMax-HMin)/10;

 this->KofV=Kof;
 this->MinV=HMin;
 for(int y=0;y<this->H;y++)
 {
  for(int x=0;x<this->W;x++)
  {
   this->ClassifMass[x+y*this->W]=-5;
   if(this->MassPoly[x+y*this->W]>=0 && NewCh2[x+y*this->W]>0)
   {
    this->ClassifMass[x+y*this->W]=(NewCh2[x+y*this->W]-HMin)/Kof;
   }
  }//for(int x=0;x<this->W;x++)
 }//for(int y=0;y<this->H;y++)

 delete[] NewCh2;
// this->CreateIndexImg("proba_1.bmp",7);
 this->CreateIndexImg("Batinometr.bmp",this->Ch-1);
}
void DNPoly::Batinometr(int N590,int N830,int N900,
                        int N580,int N620,
                        int N485,int N560,int N660,
                        int N450,int N545,int N605)
{
 if(this->Ch==this->IshCh)
 {
  QString Str="sqrt(["+QString().setNum(N590)+
              "]*["+QString().setNum(N590)+
              "]+["+QString().setNum(N830)+
              "]*["+QString().setNum(N830)+
              "]+["+QString().setNum(N900)+
              "]*["+QString().setNum(N900)+"])";
  this->AddChanal(Str);
 }//if(this->Ch==this->IshCh)
 if(this->Ch-this->IshCh==1)
 {
  this->AddIntegralChanal(N580,N620);
 }//if(this->Ch-this->IshCh==1)
 if(this->Ch-this->IshCh==2)
 {
  this->AddSKOChanal(10,this->Ch-2);
  this->AddIntegralChanal(N580,N620);
 }//if(this->Ch-this->IshCh==2)

 bool *MaskCh;
 QList <int> nCh;
 nCh<<this->Ch-2<<this->Ch-1;

 float *Brigth=new float[this->W*nCh.size()];
 MaskCh=this->FillMaskCh(nCh);

 for(int y=0;y<this->H;y++)
 {
  this->GetSpectrString(y+yn,Brigth,MaskCh);
  for(int x=0;x<this->W;x++)
  {
   this->ClassifMass[x+y*this->W]=-1;
   if(this->MassPoly[x+y*this->W]>=0)
   {
    this->ClassifMass[x+y*this->W]=1; //Всё остальное
    if(Brigth[1+x*nCh.size()]>0.01)
     this->ClassifMass[x+y*this->W]=0; //Мокрофиты
    if(Brigth[x*nCh.size()]>0.03)
     this->ClassifMass[x+y*this->W]=1; //Песок
   }//if(this->MassPoly>=0)
  }//for(quint64 i=0;i<this->W*this->H;i++)
 }//for(int y=0;y<this->H;y++)
 this->IsPolyClassif=TRUE;
 delete[] MaskCh;
 delete[] Brigth;
 nCh.clear();

 nCh<<N485<<N560<<N660<<N450<<N545<<N605;
 MaskCh=this->FillMaskCh(nCh);
 Brigth=new float[this->W*nCh.size()];
 float *NewCh=new float[this->W*this->H];

 DNTheam Theam;
 for(int y=0;y<this->H;y++)
 {
  this->GetSpectrString(y+yn,Brigth,MaskCh);
  for(int x=0;x<this->W;x++)
  {
   if(this->MassPoly[x+y*this->W]>=0)
   {
    NewCh[x+y*this->W]=Theam.Batinometr(ClassifMass[x+y*this->W],
                                        Brigth[x*nCh.size()+1],
                                        Brigth[x*nCh.size()+3],
                                        Brigth[x*nCh.size()+5],
                                        Brigth[x*nCh.size()],
                                        Brigth[x*nCh.size()+2],
                                        Brigth[x*nCh.size()+4]);

   }//if(this->MassPoly>=0)
  }//for(quint64 i=0;i<this->W*this->H;i++)
 }//for(int y=0;y<this->H;y++)

 this->AddChanal(NewCh);
 delete[] NewCh;
 float *Apert,*NewCh2;
 int n=7;
 Apert=new float[n*n];

 for(int i=0;i<n*n;i++)
 {
  Apert[i]=1;
 }

 NewCh2=this->LinearFilter(n,Apert,(1./(n*n)),this->Ch-1);
 this->AddChanal(NewCh2);

 float HMin=15,HMax=0,Kof=1;

 for(int y=0;y<this->H;y++)
 {
  for(int x=0;x<this->W;x++)
  {
   if(NewCh2[x+y*this->W]>0 && HMin>NewCh2[x+y*this->W])
    HMin=NewCh2[x+y*this->W];

   if(HMax<NewCh2[x+y*this->W])
    HMax=NewCh2[x+y*this->W];
  }//for(int x=0;x<this->W;x++)
 }//for(int y=0;y<this->H;y++)

 if(HMax!=HMin)
  Kof=(HMax-HMin)/10;

 this->KofV=Kof;
 this->MinV=HMin;

 for(int y=0;y<this->H;y++)
 {
  for(int x=0;x<this->W;x++)
  {
   this->ClassifMass[x+y*this->W]=-5;
   if(this->MassPoly[x+y*this->W]>=0 && NewCh2[x+y*this->W]>0)
   {
    this->ClassifMass[x+y*this->W]=(NewCh2[x+y*this->W]-HMin)/Kof;
   }
  }//for(int x=0;x<this->W;x++)
 }//for(int y=0;y<this->H;y++)

 delete[] NewCh2;

 // this->CreateIndexImg("proba_1.bmp",7);
 this->CreateIndexImg("Batinometr.bmp",this->Ch-1);
}

/**********************************************************************************************************************************/

//Векторизация
void DNPoly::PixToGeo(int xp,int yp,double xTopLeft,double XD,double XAngle,double yTopLeft,double YD,double yAngle,double *xGeo,double *yGeo)
{
 *xGeo=xTopLeft+xp*XD*cos(XAngle)-yp*YD*sin(XAngle);
 *yGeo=yTopLeft+yp*YD*cos(yAngle)-xp*XD*sin(yAngle);
}
QList <DNVector> DNPoly::RastrToVector(double xTopLeft,double XD,double XAngle,double yTopLeft,double YD,double YAngle,float Kof,float MinV, QString FileName)
{
 using namespace std;
 struct PointsProperty
 {
  bool NoBlack;
  bool Prover;
 };
 struct PointPlace
 {
  int x;
  int y;
 };
 struct PixArea
 {
  int AreaNumber;
  int AreaNumber2[2];
  bool Prover;
  bool Krai;
  int NumPolygon;
  int Uglov;
 };
 struct PointPlaceMass
 {
  int *x;
  int *y;
  int *UglovPoint;
 };
 struct PointKoord
 {
  double x;
  double y;
 };

 PointPlace Pix,Pix2;
 PointPlaceMass *ObjectKoord,*ObjectKoordUporadoch;
 PixArea *Points;
 std::vector <PointPlace> PixSteck;

 int *KolvoKraiPoint,*EllMass;
 int ObodX[4],ObodY[4],FlagArea=0;
 bool *BlackAreaKrai;
 bool Obod[4];
 bool *PixProver;
 QList <DNVector> Polygons;
 DNVector Polygon;
// QString DataString[3]; /*Наименование слоя англ*/
// QString DataString2[3]; /*Наименование слоя рус*/
// QString DataString3[3]; /*Свойство 1*/
// QString DataString4[3]; /*Свойство 2*/
// QString DataNumString;
 //TCHAR *VectorFileName;
 //LPSTR VFileName;
 //char *CharString;
 //CharString=new char[200];
 bool PolygonZap=FALSE; /*Признак записи создания полигона*/
 int Old_j_pol=-10;/*Старый номер полигона*/

// bool PixAreaTrue;

 int i,j_x,j_y,m,j,k,j_pol;
 int KolvoPointProver; /*Количество точек, которое необходимо проверить в алгоритме волшебной палочки (проверять диагонали или нет: может равняться 4 или 8)*/
 int xP2,yP2,NullB;
 int IarkFirstPix,IarkPix,IarkPixMax=-1,IarkPixMin=255;
 int Number,SummObod;
 quint32 KolvoPix=0,EPSGCode;
 int proba,/*proba2,*/Napravl;
 double KoordX,KoordY;

// WORD Enter=2573;

// tif=XTIFFOpen(GTifFileName,"r");
// gtif=GTIFNew(tif);
// GTIFGetDefn(gtif,&psDefn);
// EPSGCode=psDefn.PCS;

 for(j_y=0;j_y<this->H;j_y++)
 {
  for(j_x=0;j_x<this->W;j_x++)
  {
   if(this->ClassifMass[j_x+this->W*j_y]>IarkPixMax)
    IarkPixMax=this->ClassifMass[j_x+this->W*j_y];
   if(this->ClassifMass[j_x+this->W*j_y]<IarkPixMin)
    IarkPixMin=this->ClassifMass[j_x+this->W*j_y];
  }
 }


//  DataString[0]=_T("SHFields;");
//  DataString2[0]=_T("Сельскохозяйственное поле;");
//  DataString3[0]=_T("Залежь;");

//  DataString[1]=_T("SHFields;");
//  DataString2[1]=_T("Сельскохозяйственное поле;");
//  DataString3[1]=_T("Пашня;");

//  DataString[2]=_T("SHFields;");
//  DataString2[2]=_T("Сельскохозяйственное поле;");
//  DataString3[2]=_T("Пар;");


// ::wcscpy(VectorFileName,(TCHAR*)Obj->MainPath);
// ::wcscat(VectorFileName,_T("FileForVektor.txt"));
// VFileName=Obj->UnicodeToAnsi(VectorFileName);

// FILE *VektorFile=fopen(VFileName,"wb");
// fseek(VektorFile,0,SEEK_SET);

 if(IarkPixMin<0)
  IarkPixMin=0;

 for(j_pol=IarkPixMin;j_pol<=IarkPixMax;j_pol++)
 {

  Points=new PixArea[(this->W+1)*(this->H+1)];

  for(j_y=0;j_y<this->H+1;j_y++)
  {
   for(j_x=0;j_x<this->W+1;j_x++)
   {
    Points[j_x+(this->W+1)*j_y].AreaNumber=0;
    Points[j_x+(this->W+1)*j_y].AreaNumber2[0]=-1;
    Points[j_x+(this->W+1)*j_y].AreaNumber2[1]=-1;
    Points[j_x+(this->W+1)*j_y].Prover=FALSE;
    Points[j_x+(this->W+1)*j_y].Krai=FALSE;
    Points[j_x+(this->W+1)*j_y].NumPolygon=-1;
    Points[j_x+(this->W+1)*j_y].Uglov=-1;
   }
  }

  Number=0;
  for(j_y=0;j_y<this->H;j_y++)
  {
   for(j_x=0;j_x<this->W;j_x++)
   {
    if(Points[j_x+(this->W+1)*j_y].Prover==FALSE)
    {
     if(this->ClassifMass[j_x+this->W*j_y]==j_pol)
      IarkFirstPix=this->ClassifMass[j_x+this->W*j_y];
     else
      IarkFirstPix=-5;

     Pix.x=j_x;
     Pix.y=j_y;

     Points[j_x+(this->W+1)*j_y].Prover=TRUE;
     Points[j_x+(this->W+1)*j_y].AreaNumber=Number;
     Points[j_x+(this->W+1)*j_y].NumPolygon=IarkFirstPix;

     KolvoPointProver=4;
     if(IarkFirstPix==-5)
      KolvoPointProver=8;

     PixSteck.push_back(Pix);

     KolvoPix=0;
     while(PixSteck.size()>0)
     {
      Pix=PixSteck[PixSteck.size()-1];
      xP2=(int)Pix.x;
      yP2=(int)Pix.y;
      if(Points[j_x+(this->W+1)*j_y].NumPolygon==IarkFirstPix)
      {
       KolvoPix++;
       PixSteck.pop_back();

       for(i=0;i<KolvoPointProver;i++)
       {
        NullB=0;
        if((i==0)&&(Pix.x+1<this->W)&&(Points[Pix.x+1+Pix.y*(this->W+1)].Prover==FALSE))
        {
         xP2=Pix.x+1;
         yP2=Pix.y;
         NullB=1;
        }
        if((i==1)&&(Pix.y+1<this->H)&&(Points[Pix.x+(Pix.y+1)*(this->W+1)].Prover==FALSE))
        {
         xP2=Pix.x;
         yP2=Pix.y+1;
         NullB=1;
        }
        if((i==2)&&(Pix.x-1>=0)&&(Points[Pix.x-1+Pix.y*(this->W+1)].Prover==FALSE))
        {
         xP2=Pix.x-1;
         yP2=Pix.y;
         NullB=1;
        }
        if((i==3)&&(Pix.y-1>=0)&&(Points[Pix.x+(Pix.y-1)*(this->W+1)].Prover==FALSE))
        {
         xP2=Pix.x;
         yP2=Pix.y-1;
         NullB=1;
        }
/*Проверка диагональных точек*/
        if((i==4)&&(Pix.y-1>=0)&&(Pix.x-1>=0)&&(Points[(Pix.x-1)+(Pix.y-1)*(this->W+1)].Prover==FALSE))
        {
         xP2=Pix.x-1;
         yP2=Pix.y-1;
         NullB=1;
        }
        if((i==5)&&(Pix.y-1>=0)&&(Pix.x+1<this->W)&&(Points[(Pix.x+1)+(Pix.y-1)*(this->W+1)].Prover==FALSE))
        {
         xP2=Pix.x+1;
         yP2=Pix.y-1;
         NullB=1;
        }
        if((i==6)&&(Pix.y+1<this->H)&&(Pix.x-1>=0)&&(Points[(Pix.x-1)+(Pix.y+1)*(this->W+1)].Prover==FALSE))
        {
         xP2=Pix.x-1;
         yP2=Pix.y+1;
         NullB=1;
        }
        if((i==7)&&(Pix.y+1<this->H)&&(Pix.x+1<this->W)&&(Points[(Pix.x+1)+(Pix.y+1)*(this->W+1)].Prover==FALSE))
        {
         xP2=Pix.x+1;
         yP2=Pix.y+1;
         NullB=1;
        }


        if(NullB==1)
        {
         if(this->ClassifMass[xP2+this->W*yP2]==j_pol)
          IarkPix=this->ClassifMass[xP2+this->W*yP2];
         else
          IarkPix=-5;
         if(IarkPix==IarkFirstPix)
         {
          Pix2.x=xP2;
          Pix2.y=yP2;
          PixSteck.push_back(Pix2);

          Points[xP2+(this->W+1)*yP2].NumPolygon=IarkPix;
          Points[xP2+(this->W+1)*yP2].Prover=TRUE;
          Points[xP2+(this->W+1)*yP2].AreaNumber=Number;
         }
        } /*if(NullB==1)*/
       } /*for*/
      } /*if*/
      else
       PixSteck.pop_back();
     } /*while*/
     Number++;
    } /*if(Points[j_x+(x2-x1+1)*j_y].Prover==FALSE)*/
   }/*(j_x=x1;j_x<x2;j_x++)*/
  }/*for(j_y=y1;j_y<y2;j_y++)*/

  KolvoKraiPoint=new int[Number];
  EllMass=new int[Number];
  BlackAreaKrai=new bool[Number];

  for(i=0;i<Number;i++)
  {
   KolvoKraiPoint[i]=0;
   BlackAreaKrai[i]=FALSE;
  }

 /*Запоминаем номера краевых чёрных зон*/

  for(j_x=0;j_x<this->W;j_x++)
  {
   if(Points[j_x].NumPolygon==-5)
    BlackAreaKrai[Points[j_x].AreaNumber]=TRUE;
   if((Points[j_x+(this->H-1)*(this->W+1)].NumPolygon==-5))
    BlackAreaKrai[Points[j_x+(this->H-1)*(this->W+1)].AreaNumber]=TRUE;
  }
  for(j_y=0;j_y<(this->H);j_y++)
  {
   if(Points[0+j_y*(this->W+1)].NumPolygon==-5)
    BlackAreaKrai[Points[0+j_y*(this->W+1)].AreaNumber]=TRUE;
   if((Points[(this->W-1)+j_y*(this->W+1)].NumPolygon==-5))
    BlackAreaKrai[Points[(this->W-1)+j_y*(this->W+1)].AreaNumber]=TRUE;
  }

  ObodX[0]=-1;
  ObodY[0]=-1;

  ObodX[1]=0;
  ObodY[1]=-1;

  ObodX[2]=-1;
  ObodY[2]=0;

  ObodX[3]=0;
  ObodY[3]=0;

  FlagArea=0;

  for(j_y=0;j_y<(this->H+1);j_y++)
  {
   for(j_x=0;j_x<(this->W+1);j_x++)
   {
    for(m=0;m<4;m++)
     Obod[m]=FALSE;

    if(j_x==0)
    {
     Obod[0]=TRUE;
     Obod[2]=TRUE;
    }
    if(j_y==0)
    {
     Obod[0]=TRUE;
     Obod[1]=TRUE;
    }
    if(j_x==(this->W))
    {
     Obod[1]=TRUE;
     Obod[3]=TRUE;
    }
    if(j_y==(this->H))
    {
     Obod[2]=TRUE;
     Obod[3]=TRUE;
    }
    SummObod=0;

    for(m=0;m<4;m++)
    {
     if(Obod[m]==FALSE)
     {
      if(Points[(j_x+ObodX[m])+(j_y+ObodY[m])*(this->W+1)].NumPolygon==-5)
       Obod[m]=BlackAreaKrai[Points[(j_x+ObodX[m])+(j_y+ObodY[m])*(this->W+1)].AreaNumber];
     }
     if(Obod[m]==FALSE)
     {
      Points[j_x+j_y*(this->W+1)].AreaNumber2[0]=Points[(j_x+ObodX[m])+(j_y+ObodY[m])*(this->W+1)].AreaNumber;
     }
     SummObod+=(int)Obod[m];
    }
    if(SummObod!=4&&SummObod!=0)
    {
     Points[j_x+j_y*(this->W+1)].Krai=TRUE;
     KolvoKraiPoint[(Points[j_x+j_y*(this->W+1)].AreaNumber2[0])]+=1;
     if(SummObod!=2)
     {
      Points[j_x+j_y*(this->W+1)].Uglov=0;
      for(m=0;m<4;m++)
       Points[j_x+j_y*(this->W+1)].Uglov+=Obod[m]<<m;
      FlagArea++;
     }
/**********************Диагональные точки*************************************/
     if(SummObod==2&&Obod[0]==Obod[3]&&Obod[1]==Obod[2])
     {
      Points[j_x+j_y*(this->W+1)].Uglov=0;
      for(m=0;m<4;m++)
      {
       Points[j_x+j_y*(this->W+1)].Uglov+=Obod[m]<<m;
       if(Obod[m]==FALSE&&
          Points[(j_x+ObodX[m])+(j_y+ObodY[m])*(this->W+1)].NumPolygon!=-5&&
          Points[j_x+j_y*(this->W+1)].AreaNumber2[0]!=Points[(j_x+ObodX[m])+(j_y+ObodY[m])*(this->W+1)].AreaNumber)
       {
        Points[j_x+j_y*(this->W+1)].AreaNumber2[1]=Points[(j_x+ObodX[m])+(j_y+ObodY[m])*(this->W+1)].AreaNumber;
        KolvoKraiPoint[(Points[j_x+j_y*(this->W+1)].AreaNumber2[1])]+=1;
       }

       if(Obod[m]==FALSE&&
          Points[(j_x+ObodX[m])+(j_y+ObodY[m])*(this->W+1)].NumPolygon!=-5&&
          Points[j_x+j_y*(this->W+1)].AreaNumber2[0]==Points[(j_x+ObodX[m])+(j_y+ObodY[m])*(this->W+1)].AreaNumber)//&&
       {
        if(ObodY[m]!=0)
        {
         Points[j_x+j_y*(this->W+1)].AreaNumber2[1]=Points[(j_x+ObodX[m])+(j_y+ObodY[m])*(this->W+1)].AreaNumber;
         KolvoKraiPoint[(Points[j_x+j_y*(this->W+1)].AreaNumber2[1])]+=1;
         Points[j_x+j_y*(this->W+1)].Uglov=-1;
        }
       }
      }
     }
    }/*if(SummObod!=4&&SummObod!=0)*/
   }
  }

  ObjectKoord=new PointPlaceMass[Number];
  ObjectKoordUporadoch=new PointPlaceMass[Number];
  for(i=0;i<Number;i++)
  {
   ObjectKoord[i].x=new int[KolvoKraiPoint[i]];
   ObjectKoord[i].y=new int[KolvoKraiPoint[i]];
   ObjectKoord[i].UglovPoint=new int[KolvoKraiPoint[i]];

   ObjectKoordUporadoch[i].x=new int[KolvoKraiPoint[i]];
   ObjectKoordUporadoch[i].y=new int[KolvoKraiPoint[i]];
   ObjectKoordUporadoch[i].UglovPoint=new int[KolvoKraiPoint[i]];

   EllMass[i]=0;
  }

  for(j_y=0;j_y<(this->H+1);j_y++)
  {
   for(j_x=0;j_x<(this->W+1);j_x++)
   {
    if(Points[j_x+(this->W+1)*j_y].Krai==TRUE)
    {
     ObjectKoord[Points[j_x+(this->W+1)*j_y].AreaNumber2[0]].x[EllMass[Points[j_x+(this->W+1)*j_y].AreaNumber2[0]]]=j_x;
     ObjectKoord[Points[j_x+(this->W+1)*j_y].AreaNumber2[0]].y[EllMass[Points[j_x+(this->W+1)*j_y].AreaNumber2[0]]]=j_y;
     ObjectKoord[Points[j_x+(this->W+1)*j_y].AreaNumber2[0]].UglovPoint[EllMass[Points[j_x+(this->W+1)*j_y].AreaNumber2[0]]]=Points[j_x+(this->W+1)*j_y].Uglov;
     if(Points[j_x+(this->W+1)*j_y].AreaNumber2[0]==Points[j_x+(this->W+1)*j_y].AreaNumber2[1] && Points[j_x+(this->W+1)*j_y].AreaNumber2[1]>=0)
      ObjectKoord[Points[j_x+(this->W+1)*j_y].AreaNumber2[0]].UglovPoint[EllMass[Points[j_x+(this->W+1)*j_y].AreaNumber2[0]]]=-1;
     if(Points[j_x+(this->W+1)*j_y].AreaNumber2[1]!=-1)
     {
      ObjectKoord[Points[j_x+(this->W+1)*j_y].AreaNumber2[1]].x[EllMass[Points[j_x+(this->W+1)*j_y].AreaNumber2[1]]]=j_x;
      ObjectKoord[Points[j_x+(this->W+1)*j_y].AreaNumber2[1]].y[EllMass[Points[j_x+(this->W+1)*j_y].AreaNumber2[1]]]=j_y;
      if(Points[j_x+(this->W+1)*j_y].Uglov==6)
      {
       ObjectKoord[Points[j_x+(this->W+1)*j_y].AreaNumber2[0]].UglovPoint[EllMass[Points[j_x+(this->W+1)*j_y].AreaNumber2[0]]]=7;
       ObjectKoord[Points[j_x+(this->W+1)*j_y].AreaNumber2[1]].UglovPoint[EllMass[Points[j_x+(this->W+1)*j_y].AreaNumber2[1]]]=14;
      }
      if(Points[j_x+(this->W+1)*j_y].Uglov==9)
      {
       ObjectKoord[Points[j_x+(this->W+1)*j_y].AreaNumber2[0]].UglovPoint[EllMass[Points[j_x+(this->W+1)*j_y].AreaNumber2[0]]]=11;
       ObjectKoord[Points[j_x+(this->W+1)*j_y].AreaNumber2[1]].UglovPoint[EllMass[Points[j_x+(this->W+1)*j_y].AreaNumber2[1]]]=13;
      }
      EllMass[Points[j_x+(this->W+1)*j_y].AreaNumber2[1]]++;
     }

     ObjectKoordUporadoch[Points[j_x+(this->W+1)*j_y].AreaNumber2[0]].x[EllMass[Points[j_x+(this->W+1)*j_y].AreaNumber2[0]]]=0;
     ObjectKoordUporadoch[Points[j_x+(this->W+1)*j_y].AreaNumber2[0]].y[EllMass[Points[j_x+(this->W+1)*j_y].AreaNumber2[0]]]=0;

     EllMass[Points[j_x+(this->W+1)*j_y].AreaNumber2[0]]++;
    }
   }
  }



  ObodX[0]=0;
  ObodY[0]=-1;

  ObodX[1]=1;
  ObodY[1]=0;

  ObodX[2]=0;
  ObodY[2]=1;

  ObodX[3]=-1;
  ObodY[3]=0;


  for(m=0;m<Number;m++)
  {
   PixProver=new bool[KolvoKraiPoint[m]];
   for(i=0;i<KolvoKraiPoint[m];i++)
    PixProver[i]=FALSE;
   if(KolvoKraiPoint[m]!=0)
   {
    Napravl=-1;
    ObjectKoordUporadoch[m].x[0]=ObjectKoord[m].x[0];
    ObjectKoordUporadoch[m].y[0]=ObjectKoord[m].y[0];
    PixProver[0]=TRUE;
    i=0;

    if(ObjectKoord[m].UglovPoint[0]==7||ObjectKoord[m].UglovPoint[0]==8)
     k=1;
    if(ObjectKoord[m].UglovPoint[0]==11||ObjectKoord[m].UglovPoint[0]==4)
     k=2;
    if(ObjectKoord[m].UglovPoint[0]==13||ObjectKoord[m].UglovPoint[0]==2)
     k=1;
    if(ObjectKoord[m].UglovPoint[0]==14||ObjectKoord[m].UglovPoint[0]==1)
     k=0;

    for(j=1;j<KolvoKraiPoint[m];j++)
    {
     do{
        do{
           i++;
          }while(PixProver[i]==TRUE);
       }while(ObjectKoordUporadoch[m].x[j-1]+ObodX[k]!=ObjectKoord[m].x[i]||
              ObjectKoordUporadoch[m].y[j-1]+ObodY[k]!=ObjectKoord[m].y[i]);

     Napravl=k;
     ObjectKoordUporadoch[m].x[j]=ObjectKoord[m].x[i];
     ObjectKoordUporadoch[m].y[j]=ObjectKoord[m].y[i];
     ObjectKoordUporadoch[m].UglovPoint[j]=ObjectKoord[m].UglovPoint[i];

     if(ObjectKoordUporadoch[m].UglovPoint[j]!=-1)
     {
      if(ObjectKoordUporadoch[m].UglovPoint[j]==7||ObjectKoordUporadoch[m].UglovPoint[j]==8)
      {
       if(Napravl==3)
        k=2;
       if(Napravl==0)
        k=1;
      }
      if(ObjectKoordUporadoch[m].UglovPoint[j]==11||ObjectKoordUporadoch[m].UglovPoint[j]==4)
      {
       if(Napravl==1)
        k=2;
       if(Napravl==0)
        k=3;
      }
      if(ObjectKoordUporadoch[m].UglovPoint[j]==13||ObjectKoordUporadoch[m].UglovPoint[j]==2)
      {
       if(Napravl==2)
        k=1;
       if(Napravl==3)
        k=0;
      }
      if(ObjectKoordUporadoch[m].UglovPoint[j]==14||ObjectKoordUporadoch[m].UglovPoint[j]==1)
      {
       if(Napravl==1)
        k=0;
       if(Napravl==2)
        k=3;
      }
     }
     PixProver[i]=TRUE;

     if(Points[ObjectKoord[m].x[i]+ObjectKoord[m].y[i]*(this->W+1)].AreaNumber2[1]!=-1&&
        Points[ObjectKoord[m].x[i]+ObjectKoord[m].y[i]*(this->W+1)].AreaNumber2[1]== Points[ObjectKoord[m].x[i]+ObjectKoord[m].y[i]*(this->W+1)].AreaNumber2[0])
     {
      PixProver[i]=FALSE;
      Points[ObjectKoord[m].x[i]+ObjectKoord[m].y[i]*(this->W+1)].AreaNumber2[1]=-1;
     }

     i=0;
    }
   }
   delete[] PixProver;
  }


/*Запись в файл и перевод координат в тиф*/
  if(Number>0)
  {
/*Проверка группы полигонов на чёрные зоны, если все зоны чёрные, запись в файл не осуществляется*/
   k=-1;
   for(i=0;i<Number;i++)
   {
    if(KolvoKraiPoint[i]>0)
     k=1;
   }
   if(k==1)
   {
    if(PolygonZap==TRUE && Old_j_pol!=j_pol)
    {
     PolygonZap=TRUE;
    }

    if(PolygonZap==FALSE)
    {


//     CharString=Obj->UnicodeToAnsi(_T("@"));
//     fwrite(&CharString,sizeof(char),1,VektorFile);

//     CharString=Obj->UnicodeToAnsi(DataString[j_pol]);
//     fwrite(&CharString,strlen(CharString),1,VektorFile);

//     CharString=Obj->UnicodeToAnsi(DataString2[j_pol]);
//     fwrite(&CharString,strlen(CharString),1,VektorFile);

/*Запись EPSG кода для слоя*/
//     itoa(EPSGCode,CharString,10);
//     fwrite(&CharString,strlen(CharString),1,VektorFile);

     //VectorFile->WriteString(DataString2[j_pol]);

//     CharString=Obj->UnicodeToAnsi(_T("poly;"));
//     fwrite(&CharString,strlen(CharString),1,VektorFile);



//     CharString=Obj->UnicodeToAnsi(_T("1;"));
//     fwrite(&CharString,strlen(CharString),1,VektorFile);

//     CharString=Obj->UnicodeToAnsi(_T("cellS;"));
//     fwrite(&CharString,strlen(CharString),1,VektorFile);

//     CharString=Obj->UnicodeToAnsi(_T("SHuse;"));
//     fwrite(&CharString,strlen(CharString),1,VektorFile);

//     CharString=Obj->UnicodeToAnsi(_T("Степень использования поля;"));
//     fwrite(&CharString,strlen(CharString),1,VektorFile);

//     fwrite(&Enter,sizeof(WORD),1,VektorFile);
    }
   }
  }

  double xGeo,yGeo;
  for(i=0;i<Number;i++)
  {
   if(KolvoKraiPoint[i]>0)
   {
    Old_j_pol=j_pol;
    PolygonZap=TRUE;
    Polygon.GPt.clear();
    Polygon.TypeLayer=0;
    Polygon.NameLayer="Dno";
    Polygon.EPSG=FileName;
    Polygon.Vol=j_pol*Kof+MinV;
//    CharString=Obj->UnicodeToAnsi(DataString3[j_pol]);
//    fwrite(&CharString,strlen(CharString),1,VektorFile);


    //VectorFile->WriteString(DataString3[j_pol]);
    for(j=0;j<KolvoKraiPoint[i];j++)
    {
     if(ObjectKoordUporadoch[i].x[j]>=this->W)
      ObjectKoordUporadoch[i].x[j]=this->W-1;
     if(ObjectKoordUporadoch[i].y[j]>=this->H)
      ObjectKoordUporadoch[i].y[j]=this->H-1;

     KoordX=ObjectKoordUporadoch[i].x[j]+this->xn;
     KoordY=ObjectKoordUporadoch[i].y[j]+this->yn;

     this->PixToGeo(KoordX,KoordY,xTopLeft,XD,XAngle,yTopLeft,YD,YAngle,&xGeo,&yGeo);
     GeoPoint Pt;
     Pt.x=xGeo;
     Pt.y=yGeo;
     Polygon.GPt<<Pt;

//     GTIFImageToPCS(gtif,&KoordX,&KoordY);

//	 DataNumString.Format(_T("%.02f/%.02f;"),KoordX,KoordY);
//	 DataNumString.Replace(_T("."),_T(","));
//     sprintf(CharString,"%.02f/%.02f;",KoordX,KoordY);

     //_gcvt_s(
//     fwrite(&CharString,strlen(CharString),1,VektorFile);
    }/*for(j=0;j<KolvoKraiPoint[i];j++)*/
//    if(j_pol==5)
//    {
//     QMessageBox msg;
//     msg.setText(QString().setNum(Number));
//     msg.exec();
//    }
//    fwrite(&Enter,sizeof(WORD),1,VektorFile);
    Polygons<<Polygon;
   }/*if(KolvoKraiPoint[i]>0)*/
  }/*for(i=0;i<Number;i++)*/



  for(i=0;i<Number;i++)
  {
   if(KolvoKraiPoint[i]>0)
   {
    delete[] ObjectKoord[i].x;
    delete[] ObjectKoord[i].y;
    //delete[] ObjectKoord[i].PixProver;
    delete[] ObjectKoord[i].UglovPoint;

    delete[] ObjectKoordUporadoch[i].x;
    delete[] ObjectKoordUporadoch[i].y;
    //delete[] ObjectKoordUporadoch[i].PixProver;
    delete[] ObjectKoordUporadoch[i].UglovPoint;
   }
  }

  delete[] Points;
  delete[] KolvoKraiPoint;
  delete[] EllMass;
  delete[] BlackAreaKrai;
  delete[] ObjectKoord;
  delete[] ObjectKoordUporadoch;
 //  delete[] DiagonalPointBlack;
 }
// fclose(VektorFile);
// GTIFFree(gtif);
// XTIFFClose(tif);
// delete[] CharString;
 return Polygons;
}

//private
//x и y в привате считаются от начала полигона
QList<QPoint> DNPoly::DefinePixels(int NumPixels,int x,int y)
{
 //QList<QPoint> DefPix;

 bool *Prover;
 bool *NoBlackPixels;

 QPoint Pix1,Pix2;
 int i,j_x,j_y;
 int xP2,yP2,NullB;
 quint64 KolvoPix;
 QList <QPoint> PixSteck,DefPix;
 int dx[4],dy[4];
 dx[0]=0;
 dx[1]=1;
 dx[2]=0;
 dx[3]=-1;

 dy[0]=-1;
 dy[1]=0;
 dy[2]=1;
 dy[3]=0;

 PixSteck.clear();
 DefPix.clear();

 Prover=new bool[this->W*this->H];
 NoBlackPixels=new bool[this->W*this->H];
 KolvoPix=0;
 for(j_y=0;j_y<this->H;j_y++)
 {
  for(j_x=0;j_x<this->W;j_x++)
  {
   Prover[j_x+j_y*this->W]=FALSE;
   NoBlackPixels[j_x+j_y*this->W]=FALSE;
  }//for(j_x=0;j_x<this->W;j_x++)
 }//for(j_y=0;j_y<this->H;j_y++)


 Pix1.setX(x);
 Pix1.setY(y);
 Prover[x+y*this->W]=TRUE;
 PixSteck<<Pix1;
 DefPix<<Pix1;
 NoBlackPixels[x+y*this->W]=TRUE;

 while(PixSteck.size()>0 &&DefPix.size()<NumPixels)
 {
  Pix1=PixSteck[0];
  xP2=Pix1.x();
  yP2=Pix1.y();

  if(NoBlackPixels[xP2+this->W*yP2]==TRUE)
  {
   KolvoPix++;
   PixSteck.removeFirst();
   for(i=0;i<4;i++)
   {
    NullB=0;
    if((Pix1.x()+dx[i]<this->W)&&(Pix1.x()+dx[i]>0)&&
       (Pix1.y()+dy[i]<this->H)&&(Pix1.y()+dy[i]>0)&&
       (NoBlackPixels[Pix1.x()+dx[i]+(Pix1.y()+dy[i])*this->W]==FALSE)&&
       (Prover[Pix1.x()+dx[i]+(Pix1.y()+dy[i])*this->W]==FALSE))
    {
     xP2=Pix1.x()+dx[i];
     yP2=Pix1.y()+dy[i];
     NullB=1;
     Prover[Pix1.x()+dx[i]+(Pix1.y()+dy[i])*this->W]=TRUE;
    }
    if(NullB==1)
    {
     if(this->ClassifMass[Pix1.x()+dx[i]+(Pix1.y()+dy[i])*this->W]>=0)
     {
      Pix2.setX(xP2);
      Pix2.setY(yP2);
      PixSteck<<Pix2;
      DefPix<<Pix2;
      NoBlackPixels[xP2+(this->W*yP2)]=TRUE;
     }//if(FirstPix==AtherPix)
    }//if(NullB==1)
   }//for(i=0;i<4;i++)
  }//if(NoBlackPixels[xP2+this->W*yP2]==TRUE)
  else
   PixSteck.removeFirst();
 }//while(PixSteck.size()>0)

 delete[] Prover;
 delete[] NoBlackPixels;
 PixSteck.clear();
 return DefPix;
}
bool* DNPoly::FillMaskCh(QList <int> nCh)
{
 bool *MaskCh=new bool[this->Ch];
 for(int i=0;i<this->Ch;i++)
 {
  MaskCh[i]=false;
  for(int j=0;j<nCh.size();j++)
  {
   if(i==nCh.at(j))
   {
    MaskCh[i]=true;
    break;
   }//if(i==nCh.at(j))
  }//for(int j=0;j<nCh.size();j++)
 }//for(int i=0;i<this->Ch;i++)
 return MaskCh;
}
