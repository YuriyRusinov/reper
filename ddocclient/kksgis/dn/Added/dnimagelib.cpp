#include "dnimagelib.h"

DnImageLib::DnImageLib(QString FileName)
{
 RasterData=NULL;
 GDALAllRegister();
 GdalData=(GDALDataset *) GDALOpen(FileName.toAscii(),GA_ReadOnly);
 W=GdalData->GetRasterXSize();
 H=GdalData->GetRasterYSize();
 Ch=GdalData->GetRasterCount();

 QFileInfo OpenFile(FileName);
 this->PathImgFile=OpenFile.absolutePath();
 this->NameImgFile=OpenFile.completeBaseName();

 this->MinBrigth=new float[this->Ch];
 this->MaxBrigth=new float[this->Ch];
 this->MidleBrigth=new float[this->Ch];
 this->IsChanUsed=new bool[this->Ch];

 this->Lamda=NULL;

 for(int i=0;i<this->Ch;i++)
  this->IsChanUsed[i]=false;

 QFile HDRFile(this->PathImgFile+'/'+this->NameImgFile+".hdr");
 HDRFile.open(QIODevice::ReadOnly|QIODevice::Text);
 if(HDRFile.isOpen())
 {
  this->Lamda=new float[this->Ch];
  QString DataString;
  QStringList ChanList;
  do{
     DataString=HDRFile.readLine();
    }while(DataString.compare("wavelength")<0);
  do{
     DataString=HDRFile.readLine();
    }while(DataString.compare("wavelength")<0);
  int nCh=0;
  do{
     DataString=HDRFile.readLine();
     ChanList=DataString.split(", ");
     for(int i=0;i<ChanList.size();i++)
     {
      QString Str=ChanList[i];
      if(Str.indexOf('}')>=0)
       Str.remove('}');
      if(Str.indexOf(',')>=0)
       Str.remove(',');

      this->Lamda[nCh]=Str.toFloat();
      nCh++;
     }//for(int i=0;i<ChanList.size();i++)
    }while(DataString.indexOf('}')<0);


 }//if(HDRFile)

}
DnImageLib::~DnImageLib()
{
 delete[] this->MinBrigth;
 delete[] this->MaxBrigth;
 delete[] this->MidleBrigth;
 delete[] this->IsChanUsed;

 if(this->Lamda!=NULL)
  delete[] this->Lamda;
 delete[] RasterData;
}

/*Работа со спектральными данными*/
float* DnImageLib::GetBandZone(int NumBand,int xn,int yn,int xk,int yk)
{
 NumBand+=1;
 float *BrigthData=NULL;
 BrigthData=new float[(xk-xn+1)*(yk-yn+1)];
 //BrigthData=(float *)CPLMalloc(sizeof(float)*(xk-xn)*(yk-yn));
 if(NumBand<=GdalData->GetRasterCount() && xk<GdalData->GetRasterXSize() && yk<GdalData->GetRasterYSize())
 {
  GDALRasterBand  *poBand;
  poBand=GdalData->GetRasterBand(NumBand);
  poBand->RasterIO(GF_Read,xn,yn,
                   xk-xn+1,yk-yn+1,
                   BrigthData,xk-xn+1,yk-yn+1,GDT_Float32,
                   0,0);
 }
 return BrigthData;
}
float* DnImageLib::GetSpectrZone(int xn,int yn,int xk,int yk, bool *MaskCh)
{
 float *SpectrData=NULL;
 int n=this->GetQChanals(MaskCh);
 int nCh=0;
 SpectrData=new float[(xk-xn+1)*(yk-yn+1)*n];
 for(int i=0;i<this->Ch;i++)
 {
  if(MaskCh[i])
  {
   float *BandZone;
   BandZone=this->GetBandZone(i,xn,yn,xk,yk);
   for(quint64 j=0;j<(xk-xn+1)*(yk-yn+1);j++)
    SpectrData[nCh+j*n]=BandZone[j];
   delete[] BandZone;
   nCh++;
  }//if(MaskCh[i])
 }
 return SpectrData;
}
float* DnImageLib::GetSpectrString(int y,bool *MaskCh)
{
 int n=this->GetQChanals(MaskCh);
 int nCh=0;
 float *SpectrData=NULL;
 SpectrData=new float[this->W*n];
 for(int i=0;i<this->Ch;i++)
 {
  if(MaskCh[i])
  {
   float *BandString;
   BandString=this->GetBandZone(i,0,y,this->W-1,y);
   for(int j=0;j<this->W;j++)
   {
    SpectrData[nCh+j*n]=BandString[j];
   }//for(int j=0;j<this->W;j++)
   delete[] BandString;
   nCh++;
  }//if(MaskCh[i])
 }//for(int i=0;i<this->Ch;i++)

 return SpectrData;
}
float* DnImageLib::GetSpectrPoint(int xp,int yp,bool *MaskCh)
{
 int n=this->GetQChanals(MaskCh);
 int nCh=0;
 float *SpectrData=NULL;
 SpectrData=new float[n];
 for(int i=0;i<this->Ch;i++)
 {
  if(MaskCh[i])
  {
   float *BandP;
   BandP=this->GetBandZone(i,xp,yp,xp,yp);
   SpectrData[nCh]=BandP[0];
   delete[] BandP;
   nCh++;
  }//if(MaskCh[i])
 }//for(int i=0;i<this->Ch;i++)
 return SpectrData;
}

/*Работа с  географическими данными*/
/*Получить географические параметры*/
void DnImageLib::GetGeoData(GeoDataStruct *GD)
{
 double adfGeoTransform[6];
 this->GdalData->GetGeoTransform(adfGeoTransform);
 GD->XTopLeftPix=adfGeoTransform[0];
 GD->XD=adfGeoTransform[1];
 GD->XAngle=adfGeoTransform[2];
 GD->YTopLeftPix=adfGeoTransform[3];
 GD->YD=adfGeoTransform[5];
 GD->YAngle=adfGeoTransform[4];
}
/*Пересчитать координаты точки изображения в географические*/
void DnImageLib::DetermGeoCoord(int xp,int yp,GeoDataStruct GD,double &XGeo,double &YGeo)
{
 XGeo=GD.XTopLeftPix+xp*GD.XD*cos(GD.XAngle)-yp*GD.YD*sin(GD.XAngle);
 YGeo=GD.YTopLeftPix+yp*GD.YD*cos(GD.YAngle)-xp*GD.XD*sin(GD.YAngle);
}
/*Функции визуализации*/
QImage DnImageLib::GenerateImg(int Ch1,int Ch2,int Ch3,double B1,double B2,double B3,double Contrast)
{
 bool *MaskCh;
 QList <int> nCh;

 nCh<<Ch1<<Ch2<<Ch3;
 MaskCh=this->FillMaskCh(nCh);

 float BMax,BMin,Kof;
 for(int i=0;i<3;i++)
 {
  if(!this->IsChanUsed[nCh.at(i)])
  {
   this->GetMinMaxBrigthChan(nCh.at(i),&this->MinBrigth[nCh.at(i)],&this->MaxBrigth[nCh.at(i)],&this->MidleBrigth[nCh.at(i)]);
   this->IsChanUsed[nCh.at(i)]=true;
  }
  if(i==0)
  {
   BMax=this->MaxBrigth[nCh.at(i)];
   BMin=this->MinBrigth[nCh.at(i)];
  }
  else
  {
   if(this->MaxBrigth[nCh.at(i)]>BMax)
    BMax=this->MaxBrigth[nCh.at(i)];
   if(this->MinBrigth[nCh.at(i)]<BMin)
    BMin=this->MinBrigth[nCh.at(i)];
  }
 }//for(int i=0;i<3;i++)
 Kof=1;
 if((BMax-BMin)>255 && BMin>=0.)
  Kof=255/BMax;

 if((BMax-BMin)>255 && BMin<0.)
  Kof=255/(BMax-BMin);

/*Создание изображения*/
 const int n=3;
 const int nRead=(int)this->H/n;
 const int OstRead=this->H%n;
 if(this->RasterData==NULL)
 {
  this->RasterData=new unsigned int[this->W*this->H];
  memset(this->RasterData,0,this->W*this->H*sizeof(unsigned int));
 }

 int r,g,b;
 quint64 Di=0;

 for(int i=0;i<nRead;i++)
 {
  float *BrigthR,*BrigthG,*BrigthB;
  BrigthR=this->GetBandZone(Ch1,0,i*n,this->W-1,i*n+(n-1));
  BrigthG=this->GetBandZone(Ch2,0,i*n,this->W-1,i*n+(n-1));
  BrigthB=this->GetBandZone(Ch3,0,i*n,this->W-1,i*n+(n-1));
  for(quint32 j=0;j<this->W*n;j++)
  {
   if(BMin>=0)
   {
    r=BrigthR[j]*Kof;
    g=BrigthG[j]*Kof;
    b=BrigthB[j]*Kof;
   }
   else
   {
    r=(BrigthR[j]-BMin)*Kof;
    g=(BrigthG[j]-BMin)*Kof;
    b=(BrigthB[j]-BMin)*Kof;
    if(r<0)r=0;
    if(g<0)g=0;
    if(b<0)b=0;
   }
   r*=B1;
   g*=B2;
   b*=B3;
   if(r>255) r=255;
   if(g>255) g=255;
   if(b>255) b=255;
   this->RasterData[Di]=qRgb(r,g,b);
   Di++;
  }//for(quint32 j=0;j<this->W*n;j++)
  delete[] BrigthR;
  delete[] BrigthG;
  delete[] BrigthB;
 }//for(int i=0;i<nRead;i++)

 if(OstRead>0)
 {
  float *BrigthR,*BrigthG,*BrigthB;
  BrigthR=this->GetBandZone(Ch1,0,nRead*n,this->W-1,nRead*n+(OstRead-1));
  BrigthG=this->GetBandZone(Ch2,0,nRead*n,this->W-1,nRead*n+(OstRead-1));
  BrigthB=this->GetBandZone(Ch3,0,nRead*n,this->W-1,nRead*n+(OstRead-1));
  for(quint32 j=0;j<this->W*OstRead;j++)
  {
   if(BMin>=0)
   {
    r=BrigthR[j]*Kof;
    g=BrigthG[j]*Kof;
    b=BrigthB[j]*Kof;
   }
   else
   {
    r=(BrigthR[j]-BMin)*Kof;
    g=(BrigthG[j]-BMin)*Kof;
    b=(BrigthB[j]-BMin)*Kof;
    if(r<0)r=0;
    if(g<0)g=0;
    if(b<0)b=0;
   }
   r*=B1;
   g*=B2;
   b*=B3;
   if(r>255) r=255;
   if(g>255) g=255;
   if(b>255) b=255;
   this->RasterData[Di]=qRgb(r,g,b);
   Di++;
  }//for(quint32 j=0;j<this->W*OstRead;j++)
  delete[] BrigthR;
  delete[] BrigthG;
  delete[] BrigthB;
 }

 QImage img( (uchar*)&this->RasterData[0],this->W,this->H, QImage::Format_RGB32 );
 nCh.clear();
 return img;
}
/*Запись спектральных данных в файл*/
void DnImageLib::GenerateSpectrFile(int x,int y,int W,int H,QList <QPoint> pt, QString PolyName)
{
 int xc,yc;
 QDir Dir;
 PolygonProp ThisPolygon;
 bool *MaskChan;
 int *MassPoly;

 MassPoly=new int[W*H];

 for(int xp=0;xp<W;xp++)
 {
  for(int yp=0;yp<H;yp++)
  {
   MassPoly[xp+yp*W]=-1;
   if(this->IsPointInside(xp+x,yp+y,pt))
    MassPoly[xp+yp*W]=1;
  }//for(int yp=0;yp<H;yp++)
 }//for(int xp=0;xp<W;xp++)


 if(!Dir.exists(this->PathImgFile+'/'+this->NameImgFile))
  Dir.mkdir(this->PathImgFile+'/'+this->NameImgFile);

 QString FileName=this->PathImgFile+'/'+this->NameImgFile+'/'+PolyName+".pol";
 QFile SerFile(FileName);

 int ProgR,ProgCV;
 ProgR=this->Ch;
 ProgCV=0;

 SerFile.open(QIODevice::Truncate|QIODevice::WriteOnly);

 ThisPolygon.xn=x;
 ThisPolygon.yn=y;
 ThisPolygon.W=W;
 ThisPolygon.H=H;
 ThisPolygon.KCh=this->Ch;
 ThisPolygon.KPix=pt.size();

 SerFile.write((char*)&ThisPolygon,sizeof(PolygonProp));
 SerFile.write((char*)MassPoly,sizeof(int)*W*H);

 delete[] MassPoly;

 for(int i=0;i<pt.size();i++)
 {
  xc=pt[i].x();
  yc=pt[i].y();
  SerFile.write((char*)&xc,sizeof(int));
  SerFile.write((char*)&yc,sizeof(int));
 }


 QList <int> nCh;
 for(int i=0;i<this->Ch;i++)
  nCh<<i;

 MaskChan=this->FillMaskCh(nCh);
 float *SpectrZone;
 SpectrZone=new float[W*H*this->Ch];
 for(int i=0;i<this->Ch;i++)
 {
  float *PBand;
  PBand=this->GetBandZone(i,x,y,x+W-1,y+H-1);
  for(quint64 j=0;j<W*H;j++)
  {
   SpectrZone[i+j*this->Ch]=PBand[j];
  }//for(quint64 j=0;j<W*H;j++)
  delete[] PBand;
  ProgCV++;
  emit this->ChangeProgressVal(ProgR,ProgCV);
 }//for(int i=0;i<this->Ch;i++)
 SerFile.write((char*)SpectrZone,sizeof(float)*W*H*this->Ch);
 SerFile.close();
 delete[] MaskChan;
 delete[] SpectrZone;
}

/*Определение номеров спектральных каналов*/
int DnImageLib::DetermNCh(float Lam)
{
 int n=-1;
 if(this->Lamda!=NULL)
 {
  float MinD;
  MinD=fabs(Lam-this->Lamda[0]);
  n=0;
  for(int i=1;i<this->Ch;i++)
  {
   if(fabs(Lam-this->Lamda[i])<MinD)
   {
    MinD=fabs(Lam-this->Lamda[i]);
    n=i;
   }//if(fabs(Lam-this->Lamda[i])<MinD)
  }//for(int i=0;i<this->Ch;i++)
 }//if(this->Lamda!=NULL)
 return n;
}

/**********************************************************************************************************************/
/*Private*/
int DnImageLib::GetQChanals(bool *MaskCh)
{
 int n=0;
 for(int i=0;i<this->Ch;i++)
 {
  if(MaskCh[i])
   n++;
 }
 return n;
}
void DnImageLib::GetMinMaxBrigthChan(int nCh,float *BMin,float *BMax, float *BMidle)
{
 const int n=10;
 const int nRead=(int)this->H/n;
 const int OstRead=this->H%n;
 bool IsPixFined=FALSE;
 quint64 nPix=0;
 *BMidle=0;

 for(int i=0;i<nRead;i++)
 {
  float *Brigth;
  Brigth=this->GetBandZone(nCh,0,i*n,this->W-1,i*n+(n-1));
  if(i==0)
  {
   *BMax=Brigth[0];
  }//if(i==0)
  else
  {
   for(quint32 j=0;j<this->W*n;j++)
   {
    if(Brigth[j]>*BMax)
     *BMax=Brigth[j];

    if(Brigth[j]!=0)
    {
     *BMidle+=Brigth[j]/(this->W*this->H);
     nPix++;
     if(!IsPixFined)
     {
      *BMin=Brigth[j];
      IsPixFined=TRUE;
     }
     if(Brigth[j]<*BMin && IsPixFined)
      *BMin=Brigth[j];
    }//if(Brigth[j]!=0)
   }//for(quint32 j=0;j<this->W*(n+1);j++)
  }//else
  delete[] Brigth;
 }//for(int i=0;i<nRead;i++)

 if(OstRead>0)
 {
  float *Brigth;
  Brigth=this->GetBandZone(nCh,0,nRead*n,this->W-1,nRead*n+(OstRead-1));
  for(quint32 j=0;j<this->W*OstRead;j++)
  {
   if(Brigth[j]>*BMax)
    *BMax=Brigth[j];
   if(Brigth[j]!=0)
   {
    *BMidle+=Brigth[j]/(this->W*this->H);
    nPix++;
    if(Brigth[j]<*BMin && IsPixFined)
     *BMin=Brigth[j];
   }//if(Brigth[j]!=0)
  }//for(quint32 j=0;j<this->W*(n+1);j++)
  *BMidle=*BMidle*(this->W*this->H)/nPix;
  delete[] Brigth;
 }//if(OstRead>0)
}
bool* DnImageLib::FillMaskCh(QList <int> nCh)
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
bool DnImageLib::IsPointInside(int xp,int yp,QList <QPoint> pt)
{
 int intersections_num = 0;
 int prev =pt.size()-1;
 bool prev_under =pt[prev].y()<=yp;

 for (int i=0;i<pt.size(); ++i)
 {
  bool cur_under=pt[i].y()<=yp;
  int ax,ay,bx,by;
  ax=pt[prev].x()-xp;
  ay=pt[prev].y()-yp;

  bx=pt[i].x()-xp;
  by=pt[i].y()-yp;

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
