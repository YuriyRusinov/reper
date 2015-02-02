#include "dnspecbath.h"
#include "dnwidgetimage.h"
#include "ui_dnspecbath.h"

DNSpecBath::DNSpecBath(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DNSpecBath)
{
 QTextCodec::setCodecForTr(QTextCodec::codecForName("system"));
 QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));
 QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));

 ui->setupUi(this);
 ui->scrollArea->setWidget(ui->DNWPic);

 this->GdalImage=NULL;
 this->SerPoly=NULL;
 this->DlgShowDepth=NULL;
 this->DlgSKOGraph=NULL;
 this->DlgMouseCoord=NULL;
 this->DlgSG=NULL;
 this->DlgG=NULL;

 this->DlgID=NULL;
 this->DlgSM=NULL;
 this->DlgMW=NULL;
 this->DlgCIP=NULL;
 this->DlgCF=NULL;
 this->DlgAI=NULL;

 /*Тематические диалоговые окна*/
 DlgMask=NULL;
 DlgRMove=NULL;
 DlgRMove2=NULL;
 DlgSmoce=NULL;
 DlgEO=NULL;
 DlgMTD=NULL;

 this->NameProg="SuperProga";
 this->NameOrg="VKA";
 this->FileNameChan="ChanProp.ch";
 PolygonsName="NewPolyCl";
 this->AppDir=QDir::currentPath();
 this->IsTreePolygonClear=FALSE;
 this->IsPolygonCreateMode=FALSE;
 connect(ui->DNWPic,SIGNAL(OnRightButton(bool)),ui->CreatePoly,SIGNAL(triggered(bool)));
 connect(ui->treePolygons,SIGNAL(clicked(QModelIndex)),ui->treePolygons,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
// ui->pbVectorize->setEnabled(true);
}

DNSpecBath::~DNSpecBath()
{
 if(this->GdalImage!=NULL)
  delete this->GdalImage;
 delete ui;
}

void DNSpecBath::resizeEvent(QResizeEvent*)
{
 int wMW,hMW;
 wMW=this->width();
 hMW=this->height();
 wMW=wMW-230;
 hMW=hMW-70;
 if(wMW<0) wMW=0;
 if(hMW<0) hMW=0;
 ui->scrollArea->resize(wMW,hMW);
 ui->DNWPic->ViewAreaX=wMW;
 ui->DNWPic->ViewAreaY=hMW;
 ui->WidgetViewParam->setGeometry(wMW+10,10,ui->WidgetViewParam->width(),ui->WidgetViewParam->height());
}

void DNSpecBath::on_FileOpen_triggered()
{

 this->FileNameOpen=QFileDialog::getOpenFileName(this,"Открыть файл проекта","","Raster files (*.img *.asc *.tif *tiff *.bmp *.jpg *.jpeg *.ser *.int);;Geotiff (*.tif *.tiff)");
 if(!this->FileNameOpen.isEmpty())
 {
  if(this->GdalImage!=NULL)
   delete this->GdalImage;

  this->FileNameLast=this->FileNameOpen;

  this->GdalImage=new DnImageLib(this->FileNameOpen);

  QSettings Settings(this->NameOrg,this->NameProg);
  this->BKof[0]=Settings.value("ImgParam/BKofR",1).toFloat();
  this->BKof[1]=Settings.value("ImgParam/BKofG",1).toFloat();
  this->BKof[2]=Settings.value("ImgParam/BKofB",1).toFloat();

  this->nCh[0]=Settings.value("ImgParam/nChR",2).toInt();
  this->nCh[1]=Settings.value("ImgParam/nChG",1).toInt();
  this->nCh[2]=Settings.value("ImgParam/nChB",0).toInt();

  Settings.setValue("ImgParam/LastImg",this->FileNameLast);

  for(int i=0;i<3;i++)
  {
   if(this->nCh[i]>this->GdalImage->Ch-1)
   {
    this->nCh[0]=2;
    this->nCh[1]=1;
    this->nCh[2]=0;
    break;
   }
  }

  if(this->GdalImage->Ch==1)
  {
   this->nCh[0]=0;
   this->nCh[1]=0;
   this->nCh[2]=0;
  }
  ui->DNWPic->img=GdalImage->GenerateImg(nCh[0],nCh[1],nCh[2],BKof[0],BKof[1],BKof[2],1);
  ui->DNWPic->isImgLoad=true;

  this->FillMainForm();


  //Если существуют полигоны в проекте, то делаем первый полигон текущим

  if(this->NamesPoly.size()>0)
  {
   this->CurrentNamePoly=this->NamesPoly[0];
   this->CurrentNameClassif='/'+this->CurrentNamePoly;
   this->FillStackPolygons();
   this->ChangeCurrentPoly();
   this->ReadFilePoly(this->NamePolyToFile(this->CurrentNamePoly));
  }
   ui->DNWPic->repaint();
 }//if(FileName!="")

}

void DNSpecBath::on_OpenLastImg_triggered()
{
 QSettings Settings(this->NameOrg,this->NameProg);
 this->FileNameLast=Settings.value("ImgParam/LastImg",1).toString();
 this->FileNameOpen=this->FileNameLast;
 if(!this->FileNameOpen.isEmpty())
 {
  if(this->GdalImage!=NULL)
   delete this->GdalImage;

  this->FileNameLast=this->FileNameOpen;

  this->GdalImage=new DnImageLib(this->FileNameOpen);

  this->BKof[0]=Settings.value("ImgParam/BKofR",1).toFloat();
  this->BKof[1]=Settings.value("ImgParam/BKofG",1).toFloat();
  this->BKof[2]=Settings.value("ImgParam/BKofB",1).toFloat();

  this->nCh[0]=Settings.value("ImgParam/nChR",2).toInt();
  this->nCh[1]=Settings.value("ImgParam/nChG",1).toInt();
  this->nCh[2]=Settings.value("ImgParam/nChB",0).toInt();

  Settings.setValue("ImgParam/LastImg",this->FileNameLast);

  for(int i=0;i<3;i++)
  {
   if(this->nCh[i]>this->GdalImage->Ch-1)
   {
    this->nCh[0]=2;
    this->nCh[1]=1;
    this->nCh[2]=0;
    break;
   }
  }

  if(this->GdalImage->Ch==1)
  {
   this->nCh[0]=0;
   this->nCh[1]=0;
   this->nCh[2]=0;
  }

  ui->DNWPic->img=GdalImage->GenerateImg(nCh[0],nCh[1],nCh[2],BKof[0],BKof[1],BKof[2],1);
  ui->DNWPic->isImgLoad=true;
  this->FillMainForm();
  if(this->NamesPoly.size()>0)
  {
   this->CurrentNamePoly=this->NamesPoly[0];
   this->CurrentNameClassif='/'+this->CurrentNamePoly;
   this->FillStackPolygons();
   this->ChangeCurrentPoly();
   this->ReadFilePoly(this->NamePolyToFile(this->CurrentNamePoly));
  }
  ui->DNWPic->repaint();
 }//if(!this->FileNameOpen.isEmpty())
}

void DNSpecBath::on_CreatePoly_triggered(bool checked)
{
 if(checked)
 {
  this->IsPolygonCreateMode=TRUE;
  ui->DNWPic->pt.clear();
 }

 ui->DNWPic->IsCretaePolyOn=checked;

 //Сохранение полигона в файл (если снята галочка с меню Создать полигон и создаваемый полигон имеет больше двух точек)
 if(!checked && ui->DNWPic->pt.size()>2 && this->IsPolygonCreateMode)
 {
  ui->CreatePoly->setChecked(false);
  bool InputOk;
  QString NamePoly=QInputDialog::getText(this,"Создать полигон","Введите имя полигона",
                                         QLineEdit::Normal,"Polygon_"+QString().setNum(ui->DNWPic->Polygons.size()+1),
                                         &InputOk);
  if(InputOk && !NamePoly.isEmpty())
  {
   int nPoly;
   nPoly=ui->DNWPic->NewPolygon();

   nPoly-=1;
   QPoint nP,kP;
   nP=ui->DNWPic->Polygons[nPoly].GetMinP();
   kP=ui->DNWPic->Polygons[nPoly].GetMaxP();
   this->GdalImage->GenerateSpectrFile(nP.x(),nP.y(),
                                       kP.x()-nP.x(),kP.y()-nP.y(),
                                       ui->DNWPic->Polygons[nPoly].pt,NamePoly);

   this->CurrentNamePoly=NamePoly;

   this->FillMainForm();
   this->FillStackPolygons();
   this->ChangeCurrentPoly();
   this->ReadFilePoly(this->NamePolyToFile(this->CurrentNamePoly));


  }//if(InputOk && !NamePoly.isEmpty())
  this->IsPolygonCreateMode=FALSE;
  ui->DNWPic->pt.clear();
 }//if(!checked && ui->DNWPic->Polygon.pt.size()>2)
 ui->DNWPic->repaint();
}

void DNSpecBath::on_ClassResultToPoly_triggered()
{
 QList <DNPoly2::ClassToPoly> NewPolygons;
 NewPolygons=SerPoly->ClassResultToPoly(5);

 int *IshClassif,IshXn,IshYn,WIsh;
 IshClassif=new int[SerPoly->W*SerPoly->H];

 for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)
 {
  IshClassif[i]=SerPoly->ClassifMass[i];
 }
 IshXn=SerPoly->xn;
 IshYn=SerPoly->yn;
 WIsh=SerPoly->W;

 for(quint64 i=0;i<NewPolygons.size();i++)
 {
  int nPoly;
  ui->DNWPic->pt=NewPolygons[i].Pix;
  nPoly=ui->DNWPic->NewPolygon();

  nPoly-=1;
  QPoint nP,kP;
  nP=ui->DNWPic->Polygons[nPoly].GetMinP();
  kP=ui->DNWPic->Polygons[nPoly].GetMaxP();
  this->GdalImage->GenerateSpectrFile(nP.x(),nP.y(),
                                      kP.x()-nP.x(),kP.y()-nP.y(),
                                      ui->DNWPic->Polygons[nPoly].pt,PolygonsName+QString().setNum(i+1));

  this->CurrentNamePoly=PolygonsName+QString().setNum(i+1);
  this->ChangeCurrentPoly();

  this->FillMainForm();
  this->FillStackPolygons();
  this->ReadFilePoly(this->NamePolyToFile(this->CurrentNamePoly));
  int Dx,Dy;
  Dx=SerPoly->xn-IshXn;
  Dy=SerPoly->yn-IshYn;
  for(int x=0;x<SerPoly->W;x++)
  {
   for(int y=0;y<SerPoly->H;y++)
   {
    if(IshClassif[(x+Dx)+(y+Dy)*WIsh]!=NewPolygons[i].ClassNum)
     SerPoly->ClassifMass[x+y*SerPoly->W]=-1;
   }
  }//for(int i=0;i<SerPoly->W;i++)
  ui->DNWPic->pt.clear();
 }
 delete[] IshClassif;
 NewPolygons.clear();
 ui->DNWPic->repaint();
}

void DNSpecBath::on_PolyExp_triggered()
{
 double xg,yg,xp,yp;
 QString FileName;
 FileName=QFileDialog::getSaveFileName(this,"Сохранить файл геопривязки полигона","","GeoPolygon (*.gp)");
 GeoDataStruct GDSt;
 GdalImage->GetGeoData(&GDSt);

 if(this->IsCurPolyClassif)
 {
  QString ClassifName=CurrentNameClassif.right(CurrentNameClassif.size()-CurrentNameClassif.lastIndexOf('/')-1);
  ClassifName+="\0";
  this->SerPoly->PolygonExpGeo(FileName,GDSt.XTopLeftPix,GDSt.XD,GDSt.XAngle,GDSt.YTopLeftPix,GDSt.YD,GDSt.YAngle,ClassifName);
 }
 else
  this->SerPoly->PolygonExpGeo(FileName,GDSt.XTopLeftPix,GDSt.XD,GDSt.XAngle,GDSt.YTopLeftPix,GDSt.YD,GDSt.YAngle);
}

void DNSpecBath::on_PolyImp_triggered()
{
 QString FileName;
 FileName=QFileDialog::getOpenFileName(this,"Открыть файл геопривязки полигона","","GeoPolygon (*.gp)");
 ui->DNWPic->pt.clear();

 /*Имя полигона*/
 QString NamePoly;
 QFileInfo FileInfo(FileName);
 NamePoly=FileInfo.baseName();

 QList <QPoint> pt;
 GeoDataStruct GDSt;
 GdalImage->GetGeoData(&GDSt);
 QString ClassName;
 bool *ClMass;
 pt=DNPoly2::PolygonImpGeo(FileName,GDSt.XTopLeftPix,GDSt.XD,GDSt.XAngle,GDSt.YTopLeftPix,GDSt.YD,GDSt.YAngle,&ClMass,&ClassName);

 /*Если некоторые точки выходят за рамки изображения*/
 int Xmin,Xmax,Ymin,Ymax;
 for(int i=0;i<pt.size();i++)
 {
  if(i==0)
  {
   Xmin=pt[i].x();
   Ymin=pt[i].y();
   Xmax=pt[i].x();
   Ymax=pt[i].y();
  }
  else
  {
   if(Xmin>pt[i].x())
    Xmin=pt[i].x();
   if(Ymin>pt[i].y())
    Ymin=pt[i].y();
   if(Xmax<pt[i].x())
    Xmax=pt[i].x();
   if(Ymax<pt[i].y())
    Ymax=pt[i].y();
  }
  if(pt[i].x()<0)
   pt[i].setX(0);
  if(pt[i].x()>=this->GdalImage->W)
   pt[i].setX(this->GdalImage->W-1);
  if(pt[i].y()<0)
   pt[i].setY(0);
  if(pt[i].y()>=this->GdalImage->H)
   pt[i].setY(this->GdalImage->H-1);
 }//for(i=0;i<pt.size();i++)

 int nPoly;
 ui->DNWPic->pt=pt;
 nPoly=ui->DNWPic->NewPolygon();

 nPoly-=1;
 QPoint nP,kP;
 nP=ui->DNWPic->Polygons[nPoly].GetMinP();
 kP=ui->DNWPic->Polygons[nPoly].GetMaxP();
 this->GdalImage->GenerateSpectrFile(nP.x(),nP.y(),
                                     kP.x()-nP.x(),kP.y()-nP.y(),
                                     ui->DNWPic->Polygons[nPoly].pt,NamePoly);

 this->CurrentNamePoly=NamePoly;
 this->ChangeCurrentPoly();

 for(int x=this->SerPoly->xn;x<(this->SerPoly->xn+this->SerPoly->W);x++)
 {
  for(int y=this->SerPoly->yn;y<(this->SerPoly->yn+this->SerPoly->H);y++)
  {
   this->SerPoly->ClassifMass[(x-this->SerPoly->xn)+(y-this->SerPoly->yn)*this->SerPoly->W]=-1;
   if(ClMass[(x-Xmin)+(y-Ymin)*(Xmax-Xmin)])
    this->SerPoly->ClassifMass[(x-this->SerPoly->xn)+(y-this->SerPoly->yn)*this->SerPoly->W]=0;
  }
 }

 QDir Dir;
 if(!Dir.exists(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly))
  Dir.mkdir(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly);
 QFile FileClassMass(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+'/'+ClassName+".kls");
 FileClassMass.open(QIODevice::Truncate|QIODevice::WriteOnly);
 FileClassMass.write((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
 FileClassMass.close();

 this->FillMainForm();
 this->FillStackPolygons();
 this->ReadFilePoly(this->NamePolyToFile(this->CurrentNamePoly));

 ui->DNWPic->pt.clear();
 ui->DNWPic->repaint();
 //delete[] ClMass;
}

void DNSpecBath::on_ButtonReCalcImg_clicked()
{
// bool IsOk[3];
// QString DataString;

 this->nCh[0]=ui->ComboR->currentIndex();
 this->nCh[1]=ui->ComboG->currentIndex();
 this->nCh[2]=ui->ComboB->currentIndex();

 this->BKof[0]=ui->EditKR->text().replace(",",".").toFloat();
 this->BKof[1]=ui->EditKG->text().replace(",",".").toFloat();
 this->BKof[2]=ui->EditKB->text().replace(",",".").toFloat();

 QSettings Settings(this->NameOrg,this->NameProg);
 Settings.setValue("ImgParam/BKofR",this->BKof[0]);
 Settings.setValue("ImgParam/BKofG",this->BKof[1]);
 Settings.setValue("ImgParam/BKofB",this->BKof[2]);

 Settings.setValue("ImgParam/nChR",this->nCh[0]);
 Settings.setValue("ImgParam/nChG",this->nCh[1]);
 Settings.setValue("ImgParam/nChB",this->nCh[2]);


 ui->DNWPic->img=GdalImage->GenerateImg(nCh[0],nCh[1],nCh[2],BKof[0],BKof[1],BKof[2],1);
 ui->DNWPic->repaint();
}

void DNSpecBath::on_treePolygons_itemClicked(QTreeWidgetItem *item, int column)
{
 QString FileNamePoly;
 QTreeWidgetItem *itemP;
 itemP=/*current*/item;
// if(!this->IsTreePolygonClear)
// {
  this->CurrentNameClassif="";
  this->IsCurPolyClassif=FALSE;

  while(ui->treePolygons->indexOfTopLevelItem(itemP)<0)
  {
   this->CurrentNameClassif='/'+itemP->text(0)+this->CurrentNameClassif;
   itemP=itemP->parent();
   this->IsCurPolyClassif=TRUE;
  }

  this->CurrentNamePoly=itemP->text(0);
  this->CurrentNameClassif='/'+itemP->text(0)+this->CurrentNameClassif;
  FileNamePoly=this->NamePolyToFile(itemP->text(0));


  this->ChangeCurrentPoly();
  this->ReadFilePoly(FileNamePoly);


  //Если текущая позиция является результатом классификации
  if(this->IsCurPolyClassif)
  {
   this->ReadFileClassif(this->GdalImage->PathImgFile+'/'+this->GdalImage->NameImgFile+this->CurrentNameClassif+".kls");
  }
  ui->DNWPic->repaint();
}

/*Тематические задачи*/

void DNSpecBath::on_Batinometriy_triggered()
{
 if(this->SerPoly!=NULL)
 {
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  if(this->GdalImage->Ch==8)
   this->SerPoly->Batinometr();
  if(this->GdalImage->Ch>8)
  {
   QList <int> nCh;
   nCh<<GdalImage->DetermNCh(590)<<GdalImage->DetermNCh(830)<<GdalImage->DetermNCh(900)<<GdalImage->DetermNCh(580)<<
        GdalImage->DetermNCh(620)<<GdalImage->DetermNCh(485)<<GdalImage->DetermNCh(560)<<GdalImage->DetermNCh(660)<<
        GdalImage->DetermNCh(450)<<GdalImage->DetermNCh(545)<<GdalImage->DetermNCh(605);

   this->SerPoly->Batinometr(nCh[0],nCh[1],nCh[2],nCh[3],nCh[4],nCh[5],nCh[6],nCh[7],nCh[8],nCh[9],nCh[10]);
  }

  QDir Dir;
  if(!Dir.exists(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly))
   Dir.mkdir(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly);
  QFile FileClassMass(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+'/'+"Batimetr"+".kls");
  FileClassMass.open(QIODevice::Truncate|QIODevice::WriteOnly);
  FileClassMass.write((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
  FileClassMass.close();

  GeoDataStruct GDSt;
  GdalImage->GetGeoData(&GDSt);
  QList <QString> FieldsName;
  QList <QString> FieldsType;
  FieldsName<<"Deep";
  FieldsType<<"double";
  Polygons=this->SerPoly->RastrToVector(GDSt.XTopLeftPix,GDSt.XD,GDSt.XAngle,GDSt.YTopLeftPix,GDSt.YD,GDSt.YAngle,SerPoly->KofV,SerPoly->MinV,this->FileNameOpen,
                                        FieldsName,FieldsType,"Batimetr");

  ui->pbVectorize->setEnabled(true);
  ui->statusBar->showMessage(tr("Расчет спектральной батинометрии завершен"));
  QApplication::restoreOverrideCursor();

  this->FillMainForm();

 }//if(this->SerPoly!=NULL)
}

void DNSpecBath::on_MaskPokr_triggered()
{
 if(this->SerPoly!=NULL)
 {
  if(DlgMask!=NULL)
   delete DlgMask;

  DlgMask=new DlgMaskArea();
  DlgMask->NameOrg=this->NameOrg;
  DlgMask->NameProg=this->NameProg;

  DlgMask->LamGrMS1=520;
  DlgMask->LamGrMS2=590;

  DlgMask->LamRedMS1=630;
  DlgMask->LamRedMS2=690;

  DlgMask->LamNir1MS1=760;
  DlgMask->LamNir1MS2=900;

  DlgMask->LamNir2MS1=900;
  DlgMask->LamNir2MS2=1000;

  DNBDSpectr BDSpec("QPSQL","REPER","repermaster","localhost","master");

  QList <DNBDSpectr::NameAndNum> NameAndClsMask;
  NameAndClsMask=BDSpec.ReadNamesSpectrs("Средства пассивной маскировки ВВСТ");

  DlgMask->ListNameMask.clear();
  for(int i=0;i<NameAndClsMask.size();i++)
  {
   DlgMask->ListNameMask<<NameAndClsMask[i].Name;//=BDSpec.ReadNamesSpectrs("Средства пассивной маскировки ВВСТ");
   DlgMask->ListClsIdNameMask<<NameAndClsMask[i].Num.toInt();
  }


  connect(DlgMask,SIGNAL(ButtonOK()),this,SLOT(on_DlgMask_OK()));
  DlgMask->show();
 }//if(this->SerPoly!=NULL)
// GeoDataStruct GDSt;
// GdalImage->GetGeoData(&GDSt);
// double xGeo,yGeo,xp,yp;
// SerPoly->PixToGeo(200,300,GDSt.XTopLeftPix,GDSt.XD,GDSt.XAngle,GDSt.YTopLeftPix,GDSt.YD,GDSt.YAngle,&xGeo,&yGeo);
// SerPoly->GeoToPix(&xp,&yp,GDSt.XTopLeftPix,GDSt.XD,GDSt.XAngle,GDSt.YTopLeftPix,GDSt.YD,GDSt.YAngle,xGeo,yGeo);
// QMessageBox msg;
// msg.setText(QString().setNum(yp,'d',6));
// msg.exec();
}

void DNSpecBath::on_DlgMask_OK()
{
 float DeltaGS=DlgMask->DeltaGS;
 float DeltaSub=DlgMask->DeltaSub;
 float GSIndexPor=DlgMask->GSIndexPor;
 float MaxDeriviant=DlgMask->MaxDeriviant;
 int TypeFone=DlgMask->TypeFone;

// QMessageBox msg;
// msg.setText(QString().setNum(TypeFone));
// msg.exec();

 QString ClsId=DlgMask->ClsId;
 QList <QString> ClsBackgroId;
 ClsBackgroId.clear();
 for(int i=0;i<DlgMask->ClsBackgroId.size();i++)
  ClsBackgroId<<DlgMask->ClsBackgroId[i];

 int NNir1=3,NNir2=2,NRed=1,NGreen=0;

// if(this->GdalImage->Lamda!=NULL)
// {
//  NNir1=this->GdalImage->DetermNCh(750);
//  NNir2=this->GdalImage->DetermNCh(850);
//  NRed=this->GdalImage->DetermNCh(650);
//  NGreen=this->GdalImage->DetermNCh(550);
// }

 int NCh1[3],NCh2[3];

 NCh1[0]=this->GdalImage->DetermNCh(DlgMask->LamGrMS1);
 NCh2[0]=this->GdalImage->DetermNCh(DlgMask->LamGrMS2);

 NCh1[1]=this->GdalImage->DetermNCh(DlgMask->LamRedMS1);
 NCh2[1]=this->GdalImage->DetermNCh(DlgMask->LamRedMS2);

 NCh1[2]=this->GdalImage->DetermNCh(DlgMask->LamNir1MS1);
 NCh2[2]=this->GdalImage->DetermNCh(DlgMask->LamNir1MS2);


 for(int i=0;i<3;i++)
 {
  float *LamMass;
  LamMass=new float[NCh2[i]-NCh1[i]+1];
  for(int j=NCh1[i];j<=NCh2[i];j++)
  {
   LamMass[j-NCh1[i]]=GdalImage->Lamda[j];
//   QMessageBox msg;
//   msg.setText(QString().setNum(j-NCh1[2])+"\n"+QString().setNum(LamMass[j-NCh1[2]],'d',3));
//   msg.exec();
  }//for(int j=NumCh1;j<=NumCh2;j++)

  SerPoly->AddIntegralChanal(NCh1[i],NCh2[i],LamMass);
  delete[] LamMass;
 }//for(int i=0;i<3;i++)

 NNir1=SerPoly->Ch-1;
 NRed=SerPoly->Ch-2;
 NGreen=SerPoly->Ch-3;

 SerPoly->MaskAreaMS(DlgMask->DeltaMS,DlgMask->TypeFone,
                     NNir1,NNir2,NRed,NGreen,
                     DlgMask->MSIndexPor,1.85,3
                     /*0.7,0.8,0.9,1.85,500*/);

 DlgMask->close();
 {
  QDir Dir;
  if(!Dir.exists(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly))
   Dir.mkdir(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly);
  QFile FileClassMass(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+'/'+"MaskAreaTap1"+".kls");
  FileClassMass.open(QIODevice::Truncate|QIODevice::WriteOnly);
  FileClassMass.write((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
  FileClassMass.close();
  this->FillMainForm();
 }

 /*Перенос результата в гиперспектральные данные*/
// GeoDataStruct GDSt;
// GdalImage->GetGeoData(&GDSt);
// SerPoly->PolygonExpGeo(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+".gp",GDSt.XTopLeftPix,GDSt.XD,GDSt.XAngle,GDSt.YTopLeftPix,GDSt.YD,GDSt.YAngle,"MaskArea");
// QString PathToMSPoly=this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+".gp";

// on_FileOpen_triggered();

// /*Имя полигона*/
// QString NamePoly;
// QFileInfo FileInfo(PathToMSPoly);
// NamePoly=FileInfo.baseName();

// QList <QPoint> pt;
// GdalImage->GetGeoData(&GDSt);
// QString ClassName;
// bool *ClMass;
// pt=DNPoly::PolygonImpGeo(PathToMSPoly,GDSt.XTopLeftPix,GDSt.XD,GDSt.XAngle,GDSt.YTopLeftPix,GDSt.YD,GDSt.YAngle,&ClMass,&ClassName);

// /*Если некоторые точки выходят за рамки изображения*/
// int Xmin,Xmax,Ymin,Ymax;
// for(int i=0;i<pt.size();i++)
// {
//  if(i==0)
//  {
//   Xmin=pt[i].x();
//   Ymin=pt[i].y();
//   Xmax=pt[i].x();
//   Ymax=pt[i].y();
//  }
//  else
//  {
//   if(Xmin>pt[i].x())
//    Xmin=pt[i].x();
//   if(Ymin>pt[i].y())
//    Ymin=pt[i].y();
//   if(Xmax<pt[i].x())
//    Xmax=pt[i].x();
//   if(Ymax<pt[i].y())
//    Ymax=pt[i].y();
//  }
//  if(pt[i].x()<0)
//   pt[i].setX(0);
//  if(pt[i].x()>=this->GdalImage->W)
//   pt[i].setX(this->GdalImage->W-1);
//  if(pt[i].y()<0)
//   pt[i].setY(0);
//  if(pt[i].y()>=this->GdalImage->H)
//   pt[i].setY(this->GdalImage->H-1);
// }//for(i=0;i<pt.size();i++)

// int nPoly;
// ui->DNWPic->pt=pt;
// nPoly=ui->DNWPic->NewPolygon();

// nPoly-=1;
// QPoint nP,kP;
// nP=ui->DNWPic->Polygons[nPoly].GetMinP();
// kP=ui->DNWPic->Polygons[nPoly].GetMaxP();
// this->GdalImage->GenerateSpectrFile(nP.x(),nP.y(),
//                                     kP.x()-nP.x(),kP.y()-nP.y(),
//                                     ui->DNWPic->Polygons[nPoly].pt,NamePoly);

// this->CurrentNamePoly=NamePoly;
// this->ChangeCurrentPoly();

// on_MultiChan_triggered();

// for(int x=this->SerPoly->xn;x<(this->SerPoly->xn+this->SerPoly->W);x++)
// {
//  for(int y=this->SerPoly->yn;y<(this->SerPoly->yn+this->SerPoly->H);y++)
//  {
//   this->SerPoly->ClassifMass[(x-this->SerPoly->xn)+(y-this->SerPoly->yn)*this->SerPoly->W]=-1;
//   if(ClMass[(x-Xmin)+(y-Ymin)*(Xmax-Xmin)])
//    this->SerPoly->ClassifMass[(x-this->SerPoly->xn)+(y-this->SerPoly->yn)*this->SerPoly->W]=0;
//  }
// }
//*************************************************************************************************************
 int N705,N750,N1000,NDif1,NDif2;
 float *LamMass;
 if(TypeFone==0)
 {
  NDif1=this->GdalImage->DetermNCh(550);
  NDif2=this->GdalImage->DetermNCh(650);
 }
 if(TypeFone==1)
 {
  NDif1=-1;
  NDif2=-1;
 }

 if(TypeFone==2)
 {
  NDif1=this->GdalImage->DetermNCh(535);
  NDif2=this->GdalImage->DetermNCh(585);
 }
 N705=this->GdalImage->DetermNCh(705);
 N750=this->GdalImage->DetermNCh(750);
 N1000=this->GdalImage->DetermNCh(1000);

 LamMass=new float[NDif2-NDif1+1];

 for(int i=NDif1;i<=NDif2;i++)
 {
  LamMass[i-NDif1]=this->GdalImage->Lamda[i];
 }//for(int i=NDif1;i<=NDif2;i++)


 QList <int> NumChSubPix;

 for(int i=500;i<=800;i+=5)
 {
  NumChSubPix<<this->GdalImage->DetermNCh(i);
 }


 float *LamMassSub;;
 LamMassSub=new float[NumChSubPix.size()];
 for(int i=0;i<NumChSubPix.size();i++)
 {
  LamMassSub[i]=this->GdalImage->Lamda[NumChSubPix[i]];
 }

////// QMessageBox msg;
////// msg.setText(QString().setNum(LamMassSub[0],'d',3)+"\n"+QString().setNum(LamMassSub[SerPoly->IshCh-1],'d',3));
////// msg.exec();

 SerPoly->MaskAreaGS(ClsId,ClsBackgroId,DeltaGS,TypeFone,N705,N750,N1000,GSIndexPor,DeltaSub,LamMass,MaxDeriviant,NDif1,NDif2,LamMassSub,NumChSubPix);

 delete[] LamMassSub;
 delete[] LamMass;

 {
  QDir Dir;
  if(!Dir.exists(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly))
   Dir.mkdir(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly);
  QFile FileClassMass(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+'/'+/*ClassName*/"MaskAreaTap2"+".kls");
  FileClassMass.open(QIODevice::Truncate|QIODevice::WriteOnly);
  FileClassMass.write((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
  FileClassMass.close();
 }

 GeoDataStruct GDSt;
 GdalImage->GetGeoData(&GDSt);
 QList <QString> FieldsName;
 QList <QString> FieldsType;
 FieldsName<<"Comment";
 FieldsType<<"String";
 Polygons=this->SerPoly->RastrToVector(GDSt.XTopLeftPix,GDSt.XD,GDSt.XAngle,GDSt.YTopLeftPix,GDSt.YD,GDSt.YAngle,/*SerPoly->KofV*/1.,/*SerPoly->MinV*/1.,this->FileNameOpen,
                                       FieldsName,FieldsType,"MaskAreaTap2");

 ui->pbVectorize->setEnabled(true);
 ui->statusBar->showMessage(tr("Расчет спектральной батинометрии завершен"));
 QApplication::restoreOverrideCursor();

 this->FillMainForm();
// this->FillStackPolygons();
// this->ReadFilePoly(this->NamePolyToFile(this->CurrentNamePoly));

// ui->DNWPic->pt.clear();
// ui->DNWPic->repaint();
}

void DNSpecBath::on_RoutMove_triggered()
{
 if(this->SerPoly!=NULL)
 {
  if(DlgRMove!=NULL)
   delete DlgRMove;

  DlgRMove=new DlgRoutMove();
  DlgRMove->NameOrg=this->NameOrg;
  DlgRMove->NameProg=this->NameProg;

  connect(DlgRMove,SIGNAL(ButtonOK()),this,SLOT(on_DlgRMove_OK()));
  DlgRMove->show();
 }//if(this->SerPoly!=NULL)
}

void DNSpecBath::on_DlgRMove_OK()
{
 QList <QString> proba;
 DNBDSpectr BDSpec("QPSQL","REPER","repermaster","localhost","master");

 int Ngreen=2,Nred=4,Nnir=6,NredEdge=5;

 float NDVIpor=DlgRMove->NDVIPor;
 int NumCl=DlgRMove->NumCl;
 float MaxSKO=DlgRMove->MaxSKO;
 float Qc=DlgRMove->Qc;
 int I=DlgRMove->I;
 float LengthPor=DlgRMove->LengthPor;
 float MultiCh=DlgRMove->MultiCh;
 int Fenolog=DlgRMove->Fenolog;
 DlgRMove->close();

 QString PolyNameIsh=CurrentNamePoly;

 int N630,N690,N700,N800,N520,N600,N1000;

 N630=this->GdalImage->DetermNCh(630);
 N690=this->GdalImage->DetermNCh(690);
 N700=this->GdalImage->DetermNCh(700);
 N800=this->GdalImage->DetermNCh(800);
 N520=this->GdalImage->DetermNCh(520);
 N600=this->GdalImage->DetermNCh(600);
 N1000=this->GdalImage->DetermNCh(1000);


 SerPoly->AddIntegralChanal(N630,N690);
 Nred=SerPoly->Ch-1;

 SerPoly->AddIntegralChanal(N800,N1000);
 Nnir=SerPoly->Ch-1;

 SerPoly->AddIntegralChanal(N700,N800);
 NredEdge=SerPoly->Ch-1;

 SerPoly->AddIntegralChanal(N520,N600);
 Ngreen=SerPoly->Ch-1;

 SerPoly->RoutMoveTap1(Ngreen,Nred,Nnir,NredEdge,NDVIpor,NumCl,MaxSKO,Qc,I);

 {
  QDir Dir;
  if(!Dir.exists(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly))
   Dir.mkdir(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly);
  QFile FileClassMass(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+'/'+"RoutMoveIsoData"+".kls");
  FileClassMass.open(QIODevice::Truncate|QIODevice::WriteOnly);
  FileClassMass.write((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
  FileClassMass.close();
  this->FillMainForm();
 }
 /*Результаты классификации в новые полигоны*/
 PolygonsName="RoutMoveCl";
 on_ClassResultToPoly_triggered();
 PolygonsName="NewPolyCl";

 /*Чтение новых полигонов и получение спектральных данных*/
 QStringList Files;
 QString DataString;
 QDir CurDir(this->GdalImage->PathImgFile+'/'+this->GdalImage->NameImgFile);
 QList <float*> MidlSpec;
 int nChObr=4;

 Files=CurDir.entryList(QStringList()<<"RoutMoveCl*.pol",QDir::Files,QDir::Name);

 foreach(DataString,Files)
 {
  ReadFilePoly(NamePolyToFile(DataString.remove(".pol")));

  if(SerPoly->px>5)
  {
   bool *MaskCh;
   MaskCh=new bool[SerPoly->Ch];
   MidlSpec<<new float[nChObr];

   for(int i=0;i<SerPoly->Ch;i++)
   {
    MaskCh[i]=false;
    if(i==Ngreen || i==Nred || i==Nnir || i==NredEdge)
     MaskCh[i]=true;
   }
   SerPoly->MultiCh(MultiCh);
   SerPoly->GetMidlSpecPoly(SerPoly->CurrentPoly,SerPoly->px,MidlSpec[MidlSpec.size()-1],MaskCh);
   delete[] MaskCh;
  }//if(SerPoly->px>5)
 }//foreach(DataString1,PolygonsFiles)

 /*Вытаскивание данных из БД*/
 float LamStart[4],LamFinish[4];
 QList <float*> SpecEtolon;
 LamStart[0]=540;
 LamStart[1]=630;
 LamStart[2]=690;
 LamStart[3]=750;

 LamFinish[0]=600;
 LamFinish[1]=690;
 LamFinish[2]=720;
 LamFinish[3]=840;

 QList <DNBDSpectr::NameAndNum> NameClsIdForests;
 NameClsIdForests=BDSpec.ReadNamesSpectrs("Леса");
 QList <QString> ClsBackGroId;
 ClsBackGroId.clear();
 ClsBackGroId<<"0";

 QList <float> Green;
 QList <float> Red;
 QList <float> RedEdge;
 QList <float> Nir;
 QList <QString> ClsId;

 for(int i=0;i<NameClsIdForests.size();i++)
 {
  Green.clear();
  RedEdge.clear();
  Red.clear();
  Nir.clear();

  Green=BDSpec.GetBrightIntegral(LamStart[0],LamFinish[0],NameClsIdForests[i].Num,ClsBackGroId,Fenolog);
  RedEdge=BDSpec.GetBrightIntegral(LamStart[1],LamFinish[1],NameClsIdForests[i].Num,ClsBackGroId,Fenolog);
  Red=BDSpec.GetBrightIntegral(LamStart[2],LamFinish[2],NameClsIdForests[i].Num,ClsBackGroId,Fenolog);
  Nir=BDSpec.GetBrightIntegral(LamStart[3],LamFinish[3],NameClsIdForests[i].Num,ClsBackGroId,Fenolog);

  for(int j=0;j<Green.size();j++)
  {
   if(Nir[j]>0 && Red[j]>0 && Green[j]>0 && RedEdge[j]>0)
   {
    SpecEtolon<<new float[4];
    SpecEtolon[SpecEtolon.size()-1][0]=Green[j];
    SpecEtolon[SpecEtolon.size()-1][1]=Red[j];
    SpecEtolon[SpecEtolon.size()-1][2]=RedEdge[j];
    SpecEtolon[SpecEtolon.size()-1][3]=Nir[j];
    ClsId<<NameClsIdForests[i].Num;
   }//if(Nir[j]>0 && Red[j]>0)
  }//for(int j=0;j<Green.size();j++)
 }//for(int i=0;i<NameClsIdForests.size();i++)

 //Определение эталонов для реклассификации по Байесу
 DNMathAdd MathAdd;
 struct PolygonsEtolonProp
 {
  QString ClsId;
  float MinEvkl;
  QString NamePoly;
  quint64 NumGraph;
 };

 PolygonsEtolonProp *PolEtol;
 PolEtol=new PolygonsEtolonProp[MidlSpec.size()]; //Каждому полигону назначается свой эталон из бд по минимуму эвклида

 //Отнесение каждого полигона к какому-либо классу по минимуму эвклида
 for(quint64 i=0;i<MidlSpec.size();i++)
 {
  float MinEvkl,Evkl;
  for(quint64 j=0; j<ClsId.size();j++)
  {
   Evkl=MathAdd.CalcEvklid(SpecEtolon[j],MidlSpec[i],nChObr);
   if(j==0)
   {
    MinEvkl=Evkl;
    PolEtol[i].ClsId=ClsId[0];
    PolEtol[i].MinEvkl=MinEvkl;
    PolEtol[i].NamePoly=Files[0];
    PolEtol[i].NumGraph=i;
   }//if(j==0)
   if(j!=0 && Evkl<MinEvkl)
   {
    MinEvkl=Evkl;
    PolEtol[i].ClsId=ClsId[j];
    PolEtol[i].MinEvkl=MinEvkl;
    PolEtol[i].NamePoly=Files[j];
   }//if(j!=0 && Evkl<MinEvkl)
  }//for(quint64 j=0; j<ClsId.size();j++)
//  QMessageBox msg;
//  msg.setText(PolEtol[i].ClsId);
//  msg.exec();
 }//for(quint64 i=0;i<MidlSpec.size();i++)

 //Выделение из полигонов с одинаковыми классами (ClsId) эталонных (по минимуму расстояния)
 QList <PolygonsEtolonProp> Etolons; //Из всех полигонов выбираются этолонные для каждого ClsId по минимуму эвклида внутри полигонов с одинаковым ClsId
 PolygonsEtolonProp EtolMinEvkl;
 for(quint64 i=0; i<MidlSpec.size();i++)
 {
  bool IsClsIdValid=false;
  for(quint64 j=0; j<Etolons.size(); j++)
  {
   if(Etolons[j].ClsId==PolEtol[i].ClsId)
   {
    IsClsIdValid=true;
    break;
   }//if(Etolons[j].ClsId==PolEtol[i].ClsId)
  }//for(quint64 j=0; j<Etolons.size(); j++)
  if(!IsClsIdValid)
  {
   EtolMinEvkl=PolEtol[i];
   for(quint64 j=0;j<MidlSpec.size();j++)
   {
    if(j!=i && PolEtol[i].ClsId==PolEtol[j].ClsId && PolEtol[i].MinEvkl>PolEtol[j].MinEvkl)
    {
     EtolMinEvkl=PolEtol[j];
    }
   }//for(quint64 j=0;j<MidlSpec.size();j++)
   Etolons<<EtolMinEvkl;
  }//if(!IsClsIdValid)
 }//for(quint64 i=0; i<MidlSpec.size();i++)

 delete[] PolEtol;

 if(Etolons.size()>1)
 {
  ReadFilePoly(NamePolyToFile(PolyNameIsh));
  ReadFileClassif(this->GdalImage->PathImgFile+'/'+this->GdalImage->NameImgFile+'/'+PolyNameIsh+'/'+"RoutMoveIsoData"+".kls");
  bool *NoBlackPix;
  NoBlackPix=new bool[SerPoly->W*SerPoly->H];
  for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)
  {
   NoBlackPix[i]=false;
   if(SerPoly->ClassifMass[i]>=0)
   {
    NoBlackPix[i]=true;
   }
  }
  QStringList PolyFileNames;

  for(int i=0;i<Etolons.size();i++)
  {
   PolyFileNames<<NamePolyToFile(Etolons[i].NamePoly.remove(".pol"));
  }
  bool *MaskCh;
  MaskCh=new bool[SerPoly->Ch];
  for(int i=0;i<SerPoly->Ch;i++)
  {
   MaskCh[i]=false;
   if(i==Ngreen || i==Nred || i==Nnir || i==NredEdge)
    MaskCh[i]=true;
  }
  SerPoly->MaxLike(NoBlackPix,PolyFileNames,MaskCh);
  for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)
  {
   if(SerPoly->ClassifMass[i]>=0)
    SerPoly->ClassifMass[i]=Etolons[SerPoly->ClassifMass[i]].ClsId.toInt();
  }
  {
   QDir Dir;
   if(!Dir.exists(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly))
    Dir.mkdir(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly);
   QFile FileClassMass(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+'/'+"RoutMove"+".kls");
   FileClassMass.open(QIODevice::Truncate|QIODevice::WriteOnly);
   FileClassMass.write((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
   FileClassMass.close();
   this->FillMainForm();
  }
  delete[] MaskCh;

  GeoDataStruct GDSt;
  GdalImage->GetGeoData(&GDSt);
  QList <QString> FieldsName;
  QList <QString> FieldsType;
  FieldsName<<"Comment";
  FieldsType<<"String";
  Polygons=this->SerPoly->RastrToVector(GDSt.XTopLeftPix,GDSt.XD,GDSt.XAngle,GDSt.YTopLeftPix,GDSt.YD,GDSt.YAngle,/*SerPoly->KofV*/1.,/*SerPoly->MinV*/1.,this->FileNameOpen,
                                        FieldsName,FieldsType,"RoutMove");

  ui->pbVectorize->setEnabled(true);
  ui->statusBar->showMessage(tr("Расчет спектральной батинометрии завершен"));
  QApplication::restoreOverrideCursor();


 }//if(Etolons.size()>1)

 else
 {
  ReadFilePoly(NamePolyToFile(PolyNameIsh));
  ReadFileClassif(this->GdalImage->PathImgFile+'/'+this->GdalImage->NameImgFile+'/'+PolyNameIsh+'/'+"RoutMoveIsoData"+".kls");
//  QMessageBox msg;
//  msg.setText(this->GdalImage->PathImgFile+'/'+this->GdalImage->NameImgFile+'/'+PolyNameIsh+'/'+"RoutMoveIsoData"+".kls");
//  msg.exec();
  for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)
  {
   if(SerPoly->ClassifMass[i]>=0)
   {
    SerPoly->ClassifMass[i]=Etolons[0].ClsId.toInt();
   }
  }
  {
   QDir Dir;
   if(!Dir.exists(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly))
    Dir.mkdir(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly);
   QFile FileClassMass(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+'/'+"RoutMove"+".kls");
   FileClassMass.open(QIODevice::Truncate|QIODevice::WriteOnly);
   FileClassMass.write((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
   FileClassMass.close();
   this->FillMainForm();
  }
 }

// QMessageBox msg;
// msg.setText(Etolons[0].ClsId);//QString().setNum(Etolons.size()));
// msg.exec();

}

void DNSpecBath::on_RoutMove2_triggered()
{
 if(this->SerPoly!=NULL)
 {
  if(DlgRMove2!=NULL)
   delete DlgRMove2;

  DlgRMove2=new DlgRoutMove2();
  DlgRMove2->NameOrg=this->NameOrg;
  DlgRMove2->NameProg=this->NameProg;

  DlgRMove2->Lam1NDVI1[0]=750;
  DlgRMove2->Lam1NDVI1[1]=860;

  DlgRMove2->Lam2NDVI1[0]=630;
  DlgRMove2->Lam2NDVI1[1]=690;

  DlgRMove2->Lam1NDVI2[0]=750;
  DlgRMove2->Lam1NDVI2[1]=860;

  DlgRMove2->Lam2NDVI2[0]=690;
  DlgRMove2->Lam2NDVI2[1]=720;

  DlgRMove2->Lam1NDVI3[0]=800;
  DlgRMove2->Lam1NDVI3[1]=900;

  DlgRMove2->Lam2NDVI3[0]=720;
  DlgRMove2->Lam2NDVI3[1]=800;

  DlgRMove2->Lam1WBI=900;
  DlgRMove2->Lam2WBI=970;

  connect(DlgRMove2,SIGNAL(ButtonOK()),this,SLOT(on_DlgRMove2_OK()));
  DlgRMove2->show();
    }//if(this->SerPoly!=NULL)
}

void DNSpecBath::on_DlgRMove2_OK()
{
 int N630,N690,N750,N860,N720,N800,N900,N970;
 float NDVIPor1,DeltaNDVIPor1,NDVIPor2,DeltaNDVIPor2,NDVIPor3,DeltaNDVIPor3,WBIPor;

 N630=this->GdalImage->DetermNCh(DlgRMove2->Lam2NDVI1[0]);
 N690=this->GdalImage->DetermNCh(DlgRMove2->Lam2NDVI1[1]);
 N750=this->GdalImage->DetermNCh(DlgRMove2->Lam1NDVI2[0]);
 N860=this->GdalImage->DetermNCh(DlgRMove2->Lam1NDVI1[1]);
 N720=this->GdalImage->DetermNCh(DlgRMove2->Lam2NDVI3[0]);
 N800=this->GdalImage->DetermNCh(DlgRMove2->Lam2NDVI3[1]);
 N900=this->GdalImage->DetermNCh(DlgRMove2->Lam1WBI);
 N970=this->GdalImage->DetermNCh(DlgRMove2->Lam2WBI);

 NDVIPor1=DlgRMove2->PorNDVI1;
 NDVIPor2=DlgRMove2->PorNDVI2;
 NDVIPor3=DlgRMove2->PorNDVI3;
 WBIPor=DlgRMove2->PorWBI;

 DeltaNDVIPor1=DlgRMove2->DeltaNDVI1;
 DeltaNDVIPor2=DlgRMove2->DeltaNDVI2;
 DeltaNDVIPor3=DlgRMove2->DeltaNDVI3;


 SerPoly->RoutMoveTap2(N630,N690,N750,N860,N720,N800,N900,N970,
                       NDVIPor1,DeltaNDVIPor1,NDVIPor2,DeltaNDVIPor2,NDVIPor3,DeltaNDVIPor3,WBIPor);

 DlgRMove2->close();

 {
  QDir Dir;
  if(!Dir.exists(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly))
   Dir.mkdir(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly);
  QFile FileClassMass(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+'/'+"RoutMoveTap2"+".kls");
  FileClassMass.open(QIODevice::Truncate|QIODevice::WriteOnly);
  FileClassMass.write((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
  FileClassMass.close();
  this->FillMainForm();
 }
 GeoDataStruct GDSt;
 GdalImage->GetGeoData(&GDSt);
 QList <QString> FieldsName;
 QList <QString> FieldsType;
 FieldsName<<"Comment";
 FieldsType<<"String";
 Polygons=this->SerPoly->RastrToVector(GDSt.XTopLeftPix,GDSt.XD,GDSt.XAngle,GDSt.YTopLeftPix,GDSt.YD,GDSt.YAngle,/*SerPoly->KofV*/1.,/*SerPoly->MinV*/1.,this->FileNameOpen,
                                       FieldsName,FieldsType,"RoutMoveTap2");

 ui->pbVectorize->setEnabled(true);
 ui->statusBar->showMessage(tr("Расчет спектральной батинометрии завершен"));
 QApplication::restoreOverrideCursor();

}

void DNSpecBath::on_Smoces_triggered()
{
 if(this->SerPoly!=NULL)
 {
  if(DlgSmoce!=NULL)
   delete DlgSmoce;

  DlgSmoce=new DlgSmoces();
  DlgSmoce->NameOrg=this->NameOrg;
  DlgSmoce->NameProg=this->NameProg;
  DlgSmoce->LamBegin=585;
  DlgSmoce->LamEnd=625;

  connect(DlgSmoce,SIGNAL(ButtonOK()),this,SLOT(on_DlgSmoces_OK()));
  DlgSmoce->show();
 }//if(this->SerPoly!=NULL)
}

void DNSpecBath::on_DlgSmoces_OK()
{
 float R600Por=DlgSmoce->R600por;
 float KofCorrPor=DlgSmoce->KofCorpor;
 int NRead=4;
 int NNir=6;
 int N600=3;
 int N500=2;
 SerPoly->SmocesTap1(NRead,NNir,0.3);

 {
  QDir Dir;
  if(!Dir.exists(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly))
   Dir.mkdir(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly);
  QFile FileClassMass(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+'/'+"SmocesTap1"+".kls");
  FileClassMass.open(QIODevice::Truncate|QIODevice::WriteOnly);
  FileClassMass.write((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
  FileClassMass.close();
  this->FillMainForm();
 }

 //Этап 2 Отсеиваем класс с высоким NDVI
// quint64 KPoint1=0,KPoint2=0;
// float *DataSpec1,*DataSpec2;
// bool *NoBlackPix;
// bool *MaskCh;
// NoBlackPix=new bool[SerPoly->W*SerPoly->H];
// MaskCh=new bool[SerPoly->Ch];
// DataSpec1=new float[1];
// DataSpec2=new float[1];

// for(int i=0;i<SerPoly->Ch;i++)
// {
//  MaskCh=false;
// }

// MaskCh[SerPoly->Ch-1]=true;
// for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)
// {
//  NoBlackPix[i]=false;
//  if(SerPoly->ClassifMass>=0)
//   NoBlackPix[i]=true;
//  if(SerPoly->ClassifMass[i]==0)
//   KPoint1++;
//  if(SerPoly->ClassifMass[i]==1)
//   KPoint2++;
// }

// SerPoly->GetMidleSpectrClass(DataSpec1,KPoint1,0,SerPoly->ClassifMass,NoBlackPix,MaskCh);
// SerPoly->GetMidleSpectrClass(DataSpec2,KPoint1,1,SerPoly->ClassifMass,NoBlackPix,MaskCh);

// if(DataSpec1>DataSpec2)
// {
//  for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)
//  {
//   if(SerPoly->ClassifMass[i]==0)
//    SerPoly->ClassifMass[i]=-1;
//  }
// }
// else
// {
//  for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)
//  {
//   if(SerPoly->ClassifMass[i]==1)
//    SerPoly->ClassifMass[i]=-1;
//  }
// }
// delete[] DataSpec1;
// delete[] DataSpec2;
// delete[] MaskCh;
// delete[] NoBlackPix;

// {
//  QDir Dir;
//  if(!Dir.exists(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly))
//   Dir.mkdir(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly);
//  QFile FileClassMass(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+'/'+"SmocesTap2"+".kls");
//  FileClassMass.open(QIODevice::Truncate|QIODevice::WriteOnly);
//  FileClassMass.write((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
//  FileClassMass.close();
//  this->FillMainForm();
// }

 //Этап 3 Отсеиваем полигоны по пороговому значению R600 и коэффициенту корреляции
 SerPoly->SmocesTap2(R600Por,KofCorrPor,N600,N500);

 {
  QDir Dir;
  if(!Dir.exists(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly))
   Dir.mkdir(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly);
  QFile FileClassMass(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+'/'+"SmocesTapFinal"+".kls");
  FileClassMass.open(QIODevice::Truncate|QIODevice::WriteOnly);
  FileClassMass.write((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
  FileClassMass.close();
  this->FillMainForm();
 }
 GeoDataStruct GDSt;
 GdalImage->GetGeoData(&GDSt);
 QList <QString> FieldsName;
 QList <QString> FieldsType;
 FieldsName<<"Comment";
 FieldsType<<"String";
 Polygons=this->SerPoly->RastrToVector(GDSt.XTopLeftPix,GDSt.XD,GDSt.XAngle,GDSt.YTopLeftPix,GDSt.YD,GDSt.YAngle,/*SerPoly->KofV*/1.,/*SerPoly->MinV*/1.,this->FileNameOpen,
                                       FieldsName,FieldsType,"SmocesTapFinal");

 ui->pbVectorize->setEnabled(true);
 ui->statusBar->showMessage(tr("Расчет спектральной батинометрии завершен"));
 QApplication::restoreOverrideCursor();

}

void DNSpecBath::on_EmbedObj_triggered()
{
 if(DlgEO!=NULL)
  delete DlgEO;

 DlgEO=new DlgEmbedObj();
 DlgEO->NameOrg=this->NameOrg;
 DlgEO->NameProg=this->NameProg;
// DlgEO->LamBegin=585;
// DlgEO->LamEnd=625;

 DlgEO->LamMSIndex1[0]=700;
 DlgEO->LamMSIndex1[1]=800;

 DlgEO->LamMSIndex2[0]=630;
 DlgEO->LamMSIndex2[1]=690;

 DlgEO->LamGSIndex[0]=750;
 DlgEO->LamGSIndex[1]=705;

 DlgEO->LamContr1[0]=630;
 DlgEO->LamContr1[1]=690;

 DlgEO->LamContr2[0]=800;
 DlgEO->LamContr2[1]=900;

 connect(DlgEO,SIGNAL(ButtonOK()),this,SLOT(on_DlgEmbedObj_OK()));
 DlgEO->show();
}

void DNSpecBath::on_DlgEmbedObj_OK()
{
 DlgEO->hide();

 float DeltaGS=DlgEO->DeltaGS;
 float DeltaMS=DlgEO->DeltaMS;
 float MSIndex=DlgEO->MSIndex;
 float Contrast630_690=DlgEO->Contrast630_690;
 float Contrast800_1000=DlgEO->Contrast800_1000;
 float GSIndex=DlgEO->GSIndex;
 float WBI=DlgEO->WBI;
 float AKFMin=DlgEO->AKFMin;
 float KofCorelPor=DlgEO->KofCorelPor;
 float Brigth630_690=DlgEO->Brigth630_690;
 float Brigth800_1000=DlgEO->Brigth630_690;

 int NCh1[4],NCh2[4];

 NCh1[0]=this->GdalImage->DetermNCh(DlgEO->LamMSIndex1[0]);
 NCh2[0]=this->GdalImage->DetermNCh(DlgEO->LamMSIndex1[1]);

 NCh1[1]=this->GdalImage->DetermNCh(DlgEO->LamMSIndex2[0]);
 NCh2[1]=this->GdalImage->DetermNCh(DlgEO->LamMSIndex2[1]);

 NCh1[2]=this->GdalImage->DetermNCh(DlgEO->LamContr1[0]);
 NCh2[2]=this->GdalImage->DetermNCh(DlgEO->LamContr1[1]);

 NCh1[3]=this->GdalImage->DetermNCh(DlgEO->LamContr2[0]);
 NCh2[3]=this->GdalImage->DetermNCh(DlgEO->LamContr2[1]);


 for(int i=0;i<4;i++)
 {
  float *LamMass;
  LamMass=new float[NCh2[i]-NCh1[i]+1];
  for(int j=NCh1[i];j<=NCh2[i];j++)
  {
//   if(i==3)
//   {
//    QMessageBox msg;
//    msg.setText(QString().setNum(GdalImage->Lamda[j],'d',3));
//    msg.exec();
//   }
   LamMass[j-NCh1[i]]=GdalImage->Lamda[j];
  }//for(int j=NumCh1;j<=NumCh2;j++)

  SerPoly->AddIntegralChanal(NCh1[i],NCh2[i],LamMass);

  delete[] LamMass;
 }//for(int i=0;i<3;i++)

 int NNir1=SerPoly->Ch-4;
 int NRed=SerPoly->Ch-3;
 int NContr1=SerPoly->Ch-2;
 int NContr2=SerPoly->Ch-1;

 QString Formula="(["+QString().setNum(NNir1+1)+"]-["+QString().setNum(NRed+1)+"])/"+
         "(["+QString().setNum(NNir1+1)+"]+["+QString().setNum(NRed+1)+"])";

 SerPoly->AddChanal(Formula);

 bool *NoBlackPix;
 NoBlackPix=new bool[SerPoly->W*SerPoly->H];

 for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)
 {
  NoBlackPix[i]=false;
  if(SerPoly->ClassifMass[i]>=0)
   NoBlackPix[i]=true;
 }

 Formula='['+QString().setNum(SerPoly->Ch)+']';

 SerPoly->FilterPix(NoBlackPix,Formula,0,MSIndex+DeltaMS);
 for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)
 {
  NoBlackPix[i]=false;
  if(SerPoly->ClassifMass[i]>=0)
   NoBlackPix[i]=true;
 }
 SerPoly->FilterPix(NoBlackPix,Formula,1,MSIndex-DeltaMS);

 for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)
 {
  NoBlackPix[i]=false;
  if(SerPoly->ClassifMass[i]>=0)
   NoBlackPix[i]=true;
 }


 Formula="(["+QString().setNum(NContr1+1)+"]-"+QString().setNum(Brigth630_690,'d',3)+")/"+
         "(["+QString().setNum(NContr1+1)+"]+"+QString().setNum(Brigth630_690,'d',3)+")";
 SerPoly->FilterPix(NoBlackPix,Formula,1 ,Contrast630_690);
 for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)
 {
  NoBlackPix[i]=false;
  if(SerPoly->ClassifMass[i]>=0)
   NoBlackPix[i]=true;
 }

 Formula="(["+QString().setNum(NContr2+1)+"]-"+QString().setNum(Brigth800_1000,'d',3)+")/"+
         "(["+QString().setNum(NContr2+1)+"]+"+QString().setNum(Brigth800_1000,'d',3)+")";
 SerPoly->AddChanal(Formula);

 SerPoly->FilterPix(NoBlackPix,Formula,1,Contrast800_1000);
// bool *MaskCh;
// MaskCh=new bool[SerPoly->Ch];
// for(int i=0;i<SerPoly->Ch;i++)
// {
//  MaskCh[i]=false;
// }
// MaskCh[SerPoly->Ch-1]=true;
// float *DataSpec;
// DataSpec=new float[1];
 for(quint64 x=0;x<SerPoly->W;x++)
 {
  for(quint64 y=0;y<SerPoly->H;y++)
  {
   NoBlackPix[x+y*SerPoly->W]=false;
   if(SerPoly->ClassifMass[x+y*SerPoly->W]>=0)
   {
    NoBlackPix[x+y*SerPoly->W]=true;
//    SerPoly->GetSpectrPoint(x,y,DataSpec,MaskCh);
//    QMessageBox msg;
//    msg.setText(QString().setNum(DataSpec[0],'d',3));
//    msg.exec();
   }
  }
 }




 int NChGS1=this->GdalImage->DetermNCh(DlgEO->LamGSIndex[0]);
 int NChGS2=this->GdalImage->DetermNCh(DlgEO->LamGSIndex[1]);

 Formula="(["+QString().setNum(NChGS1+1)+"]-["+QString().setNum(NChGS2+1)+"])/"+
         "(["+QString().setNum(NChGS1+1)+"]+["+QString().setNum(NChGS2+1)+"])";


 SerPoly->AddChanal(Formula);
 Formula='['+QString().setNum(SerPoly->Ch)+']';

 SerPoly->FilterPix(NoBlackPix,Formula,0,GSIndex+DeltaGS);
 for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)
 {
  NoBlackPix[i]=false;
  if(SerPoly->ClassifMass[i]>=0)
   NoBlackPix[i]=true;
 }
 SerPoly->FilterPix(NoBlackPix,Formula,1,GSIndex-DeltaGS);

 for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)
 {
  NoBlackPix[i]=false;
  if(SerPoly->ClassifMass[i]>=0)
   NoBlackPix[i]=true;
 }


 NChGS1=this->GdalImage->DetermNCh(900);
 NChGS2=this->GdalImage->DetermNCh(970);

 Formula="(["+QString().setNum(NChGS1+1)+"]/["+QString().setNum(NChGS2+1)+"])";


 SerPoly->AddChanal(Formula);
 Formula='['+QString().setNum(SerPoly->Ch)+']';

 SerPoly->FilterPix(NoBlackPix,Formula,1,WBI);
 for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)
 {
  NoBlackPix[i]=false;
  if(SerPoly->ClassifMass[i]>=0)
   NoBlackPix[i]=true;
 }

 bool *MaskCh;
 float *DataSpec;
 int NChAKF=0;

 int NAKF1=this->GdalImage->DetermNCh(625);
 int NAKF2=this->GdalImage->DetermNCh(750);
 int NAKFStop=this->GdalImage->DetermNCh(670);

 MaskCh=new bool[SerPoly->Ch];

 for(int i=0;i<SerPoly->Ch;i++)
 {
  MaskCh[i]=false;
  if(i>=NAKF1 && i<=NAKF2)
  {
   MaskCh[i]=true;
   NChAKF++;
  }
 }

 DataSpec=new float[NChAKF];
 DNMathAdd MathAdd;
 float AKFVol,MinAkfVol;


 for(int x=0;x<SerPoly->W;x++)
 {
  for(int y=0;y<SerPoly->H;y++)
  {
   if(SerPoly->ClassifMass[x+y*SerPoly->W]>=0)
   {
    SerPoly->GetSpectrPoint(x+SerPoly->xn,y+SerPoly->yn,DataSpec,MaskCh);
    for(int i=1;i<NAKF2-NAKFStop;i++)
    {
     float *AKFfunk1=new float[NChAKF-i];
     float *AKFfunk2=new float[NChAKF-i];

     for(int j=0;j<NChAKF-i;j++)
     {
      AKFfunk1[j]=DataSpec[j];
      AKFfunk2[j]=DataSpec[i+j];
     }//for(int j=0;j<NChAKF-i;j++)
     AKFVol=MathAdd.CalcCorel(NChAKF-i,AKFfunk1,AKFfunk2);
     if(i==1 || (i!=1 && AKFVol<MinAkfVol))
     {
      MinAkfVol=AKFVol;
     }
     delete[] AKFfunk1;
     delete[] AKFfunk2;
    }
//    QMessageBox msg;
//    msg.setText(QString().setNum(MinAkfVol,'d',5));
//    msg.exec();
    if(MinAkfVol>AKFMin)
     SerPoly->ClassifMass[x+y*SerPoly->W]=-1;
   }//if(SerPoly->ClassifMass[x+y*SerPoly->W]>=0)
  }//for(y=0;y<SerPoly->H;y++)
 }//for(x=0;x<SerPoly->W;x++)
 delete[] DataSpec;
 delete[] MaskCh;

 {
  QDir Dir;
  if(!Dir.exists(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly))
   Dir.mkdir(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly);
  QFile FileClassMass(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+'/'+"EmbedObj"+".kls");
  FileClassMass.open(QIODevice::Truncate|QIODevice::WriteOnly);
  FileClassMass.write((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
  FileClassMass.close();
  this->FillMainForm();
 }
 GeoDataStruct GDSt;
 GdalImage->GetGeoData(&GDSt);
 QList <QString> FieldsName;
 QList <QString> FieldsType;
 FieldsName<<"Comment";
 FieldsType<<"String";
 Polygons=this->SerPoly->RastrToVector(GDSt.XTopLeftPix,GDSt.XD,GDSt.XAngle,GDSt.YTopLeftPix,GDSt.YD,GDSt.YAngle,/*SerPoly->KofV*/1.,/*SerPoly->MinV*/1.,this->FileNameOpen,
                                       FieldsName,FieldsType,"EmbedObj");

 ui->pbVectorize->setEnabled(true);
 ui->statusBar->showMessage(tr("Расчет спектральной батинометрии завершен"));
 QApplication::restoreOverrideCursor();

}

void DNSpecBath::on_MultiTempDatal_triggered()
{
 if(DlgMTD!=NULL)
  delete DlgMTD;

 DlgMTD=new DlgMultiTempData();
 DlgMTD->NameOrg=this->NameOrg;
 DlgMTD->NameProg=this->NameProg;

 connect(DlgMTD,SIGNAL(ButtonOK()),this,SLOT(on_DlgMultiTempDatal_OK()));
 DlgMTD->show();
}

void DNSpecBath::on_DlgMultiTempDatal_OK()
{
 float LeftPor,RigthPor;
 QString FileNameSravn;

 LeftPor=DlgMTD->LeftPor;
 RigthPor=DlgMTD->RigthPor;
 FileNameSravn=DlgMTD->FileName;

 DlgMTD->hide();

 QList <float*> Brigth;


 for(int i=0;i<SerPoly->Ch;i++)
 {
  Brigth<<new float[SerPoly->W*SerPoly->H];
  SerPoly->GetBrigthSqare(Brigth[Brigth.size()-1],i);
 }//for(int i=0;i<SerPoly->Ch;i++)

 QList <QPoint> pt;

 for(int i=0;i<SerPoly->pt.size();i++)
  pt<<SerPoly->pt[i];

 bool *ClMass;
 ClMass=new bool[SerPoly->W*SerPoly->H];

 for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)
 {
  ClMass[i]=false;
  if(SerPoly->ClassifMass[i]>=0)
   ClMass[i]=true;
 }//for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)

 on_FileOpen_triggered();

 /*Имя полигона*/
 QString NamePoly="Poly";


 int nPoly;
 ui->DNWPic->pt=pt;
 nPoly=ui->DNWPic->NewPolygon();

 nPoly-=1;
 QPoint nP,kP;
 nP=ui->DNWPic->Polygons[nPoly].GetMinP();
 kP=ui->DNWPic->Polygons[nPoly].GetMaxP();
 this->GdalImage->GenerateSpectrFile(nP.x(),nP.y(),
                                     kP.x()-nP.x(),kP.y()-nP.y(),
                                     ui->DNWPic->Polygons[nPoly].pt,NamePoly);
  this->CurrentNamePoly=NamePoly;
  this->ChangeCurrentPoly();

  for(int x=0;x<SerPoly->W;x++)
  {
   for(int y=0;y<SerPoly->H;y++)
   {
    this->SerPoly->ClassifMass[x+y*SerPoly->W]=-1;
    if(ClMass[x+y*SerPoly->W])
     this->SerPoly->ClassifMass[x+y*SerPoly->W]=0;
   }
  }

 delete[] ClMass;
 this->FillMainForm();

 int ACh=0;
 for(int i=0;i<Brigth.size();i++)
 {
  SerPoly->AddChanal(Brigth[i]);
  delete[] Brigth[i];
  ACh++;
 }//for(int i=0;i<Brigth.size();i++)

 QList <int> NumCh;
 for(int i=0;i<ACh;i++)
 {
  QString Formula;
  Formula='['+QString().setNum(i+1)+"]-["+QString().setNum(SerPoly->IshCh+i+1)+']';
//  QMessageBox msg;
//  msg.setText(Formula);
//  msg.exec();
  SerPoly->AddChanal(Formula);
  NumCh<<SerPoly->Ch-1;
 }

 DNMathAdd Math;
 QList <float*> Chanals;
 bool *NoBlackPix;
 NoBlackPix=new bool[SerPoly->W*SerPoly->H];
 quint64 KPoint=0;

 for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)
 {
  NoBlackPix[i]=false;
  if(SerPoly->ClassifMass[i]>=0)
  {
   NoBlackPix[i]=true;
   KPoint++;
  }
 }//for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)

 for(int i=0;i<ACh;i++)
 {
  Chanals<<new float[KPoint];
  SerPoly->GetBrigthChanPoly(Chanals[Chanals.size()-1],SerPoly->Ch-i-1,NoBlackPix);
 }


 float *Mass,*CovMatr;
 Mass=new float[KPoint*ACh];
 CovMatr=new float[ACh*ACh];

 for(quint64 i=0;i<KPoint;i++)
 {
  for(int j=0;j<ACh;j++)
  {
   Mass[i+j*KPoint]=Chanals[j][i];
  }//for(j=0;j<ACh;j++)
 }//for(quint64 i=0;i<KPoint;i++)

 for(int i=0;i<ACh;i++)
 {
  delete[] Chanals[i];
 }

 float *MatrVector;
 float *ValueSel;
 MatrVector=new float[ACh*ACh];
 ValueSel=new float[ACh];
 Math.CalcCovMatr(KPoint,ACh,Mass,CovMatr);

// for(int i=0;i<ACh*ACh+5;i++)
// {
//  QMessageBox msg;
//  msg.setText(QString().setNum(CovMatr[i],'d',4));
//  msg.exec();
// }


//// CovMatr[0]=73.12651923292241;
//// CovMatr[1]=53.09202812674408;
//// CovMatr[2]=95.28200849420223;
//// CovMatr[3]=57.48517321533324;
//// CovMatr[4]=92.64024134912705;
//// CovMatr[5]=2.779079206257377;
//// CovMatr[6]=70.55040417607542;

//// CovMatr[7]=53.09202812674408;
//// CovMatr[8]=43.24882524911537;
//// CovMatr[9]=77.48043447722121;
//// CovMatr[10]=49.86737361965988;
//// CovMatr[11]=78.86381999009156;
//// CovMatr[12]=2.373301989149871;
//// CovMatr[13]=59.23946370431676;

//// CovMatr[14]=95.28200849420223;
//// CovMatr[15]=77.48043447722121;
//// CovMatr[16]=148.6147198405307;
//// CovMatr[17]=91.59652447535429;
//// CovMatr[18]=153.6836922820907;
//// CovMatr[19]=5.576073469509119;
//// CovMatr[20]=114.6625317259;

//// CovMatr[21]=57.48517321533324;
//// CovMatr[22]=49.86737361965988;
//// CovMatr[23]=91.59652447535429;
//// CovMatr[24]=105.4769567495374;
//// CovMatr[25]=94.0738351643233;
//// CovMatr[26]=-4.877646487902462;
//// CovMatr[27]=65.82033625428947;

//// CovMatr[28]=92.64024134912705;
//// CovMatr[29]=78.86381999009156;
//// CovMatr[30]=153.6836922820907;
//// CovMatr[31]=94.0738351643233;
//// CovMatr[32]=346.8931604914206;
//// CovMatr[33]=33.76494495600135;
//// CovMatr[34]=206.8380426279584;

//// CovMatr[35]=2.779079206257377;
//// CovMatr[36]=2.373301989149871;
//// CovMatr[37]=5.576073469509119;
//// CovMatr[38]=-4.877646487902462;
//// CovMatr[39]=33.76494495600135;
//// CovMatr[40]=19.64934622922803;
//// CovMatr[41]=17.82070379602213;

//// CovMatr[42]=70.55040417607542;
//// CovMatr[43]=59.23946370431676;
//// CovMatr[44]=114.6625317259;
//// CovMatr[45]=65.82033625428947;
//// CovMatr[46]=206.8380426279584;
//// CovMatr[47]=17.82070379602213;
//// CovMatr[48]=142.9272854661795;

//// float *ProbMatr;
//// ProbMatr=new float[9];
//// ProbMatr[0]=4;
//// ProbMatr[1]=2;
//// ProbMatr[2]=1;
//// ProbMatr[3]=2;
//// ProbMatr[4]=5;
//// ProbMatr[5]=3;
//// ProbMatr[6]=1;
//// ProbMatr[7]=3;
//// ProbMatr[8]=6;



 Math.MGK(CovMatr,ACh,MatrVector,ValueSel);
 Math.MatrTranspon(MatrVector,ACh,ACh);

 float *DataSpec,*DataRes;
 DataSpec=new float[ACh*KPoint];
 bool *MaskCh;
 MaskCh=new bool[SerPoly->Ch];

 for(int i=0;i<SerPoly->Ch;i++)
 {
  MaskCh[i]=false;
 }

 for(int i=0;i<NumCh.size();i++)
  MaskCh[NumCh[i]]=true;

 SerPoly->GetSpectrZone(DataSpec,MaskCh);
 DataRes=Math.MatrMulti(DataSpec,ACh,KPoint,MatrVector,ACh,ACh);



 float *Chan;
 float SKO[3],Middle[3],Max[3],Min[3];
 Chan=new float[KPoint];

 for(int i=0;i<3;i++)
 {
  Middle[i]=0;
  for(quint64 j=0;j<KPoint;j++)
  {
   Chan[j]=DataRes[i+j*ACh];
   Middle[i]+=Chan[j];
  }
  Middle[i]=Middle[i]/KPoint;
  SKO[i]=Math.CalcSKO(Chan,KPoint);
  Max[i]=Middle[i]+SKO[i]*RigthPor;
  Min[i]=Middle[i]-SKO[i]*LeftPor;
 }
 delete[] Chan;

 float *DopChan;
 DopChan=new float[SerPoly->W*SerPoly->H];
 quint64 ip=0;
 for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)
 {
  DopChan[i]=0.;
  if(NoBlackPix[i])
  {
   DopChan[i]=DataRes[ip*ACh];
   ip++;
  }
 }
 SerPoly->AddChanal(DopChan);

 ip=0;
 for(quint64 i=0;i<SerPoly->W*SerPoly->H;i++)
 {
  SerPoly->ClassifMass[i]=-1;
  if(NoBlackPix[i])
  {
   for(int j=0;j<3;j++)
   if(DataRes[j+ip*ACh]>Max[j] || DataRes[j+ip*ACh]<Min[j])
    SerPoly->ClassifMass[i]=0;
   ip++;
  }
 }





 delete[] NoBlackPix;
 delete[] DataRes;

 {
  QDir Dir;
  if(!Dir.exists(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly))
   Dir.mkdir(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly);
  QFile FileClassMass(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+'/'+"MultiTempData"+".kls");
  FileClassMass.open(QIODevice::Truncate|QIODevice::WriteOnly);
  FileClassMass.write((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
  FileClassMass.close();
  this->FillMainForm();
 }


 GeoDataStruct GDSt;
 GdalImage->GetGeoData(&GDSt);
 QList <QString> FieldsName;
 QList <QString> FieldsType;
 FieldsName<<"Comment";
 FieldsType<<"String";
 Polygons=this->SerPoly->RastrToVector(GDSt.XTopLeftPix,GDSt.XD,GDSt.XAngle,GDSt.YTopLeftPix,GDSt.YD,GDSt.YAngle,1.,1.,this->FileNameOpen,
                                       FieldsName,FieldsType,"MultiTempData");

// for(int i=0;i<Polygons.size();i++)

 ui->pbVectorize->setEnabled(true);
 ui->statusBar->showMessage(tr("Расчет завершен"));
 QApplication::restoreOverrideCursor();
}

/*Конец тематических задач*/

void DNSpecBath::on_MultiChan_triggered()
{
 if(this->SerPoly!=NULL)
 {
  bool InputOk;
  QString MultiCh=QInputDialog::getText(this,tr("Множитель каналов"),tr("Введите множитель каналов"),QLineEdit::Normal,"0.0001",&InputOk);
  if(InputOk)
  {
   float Kof=MultiCh.replace(",",".").toFloat();
   this->SerPoly->MultiCh(Kof);
  }//if(InputOk)
 }//if(this->SerPoly!=NULL)
}

void DNSpecBath::on_pbVectorize_clicked()
{
 emit SIGNALcreateVector(); // посылаем сигнал для выполнения векторизации
}

void DNSpecBath::on_ShowDeptch_triggered()
{
 if(this->DlgShowDepth!=NULL)
  delete this->DlgShowDepth;

 QDialog *Dlg;
 Dlg=new QDialog(this);
 this->DlgShowDepth=new Ui_DlgShowDeptch();
 this->DlgShowDepth->setupUi(Dlg);
 DlgShowDepth->ComboCh->clear();

 for(int i=0;i<SerPoly->Ch;i++)
 {
  DlgShowDepth->ComboCh->addItem("Канал"+QString().setNum(i+1));
 }

 Dlg->show();

 connect(ui->DNWPic,SIGNAL(MouseMove(int,int)),this,SLOT(MouseMove(int,int)));
}

void DNSpecBath::MouseMove(int x, int y)
{
 if(DlgShowDepth!=NULL)
 {
  DlgShowDepth->EditX->setText(QString().setNum(x));
  DlgShowDepth->EditY->setText(QString().setNum(y));

  if(this->SerPoly!=NULL &&
     x>=this->SerPoly->xn && x<this->SerPoly->xn+this->SerPoly->W &&
     y>=this->SerPoly->yn && y<this->SerPoly->yn+this->SerPoly->H /*&&
     this->SerPoly->IsPolyClassif*/)
  {
   bool *MaskCh=new bool[this->SerPoly->Ch];
   for(int i=0;i<this->SerPoly->Ch;i++)
   {
    MaskCh[i]=FALSE;
    if(i==DlgShowDepth->ComboCh->currentIndex())
     MaskCh[i]=TRUE;
   }//for(int i=0;i<this->SerPoly->Ch;i++)
   float Brigth;
   this->SerPoly->GetSpectrPoint(x,y,&Brigth,MaskCh);
   DlgShowDepth->EditBrigth->setText(QString().setNum(Brigth,'d',4));
   delete[] MaskCh;
  }
 }//if(DlgShowDepth!=NULL)
}

void DNSpecBath::MouseLClicked(int x, int y)
{
 if(this->DlgSKOGraph!=NULL)
 {
  DlgSKOGraph->EditX->setText(QString().setNum(x));
  DlgSKOGraph->EditY->setText(QString().setNum(y));
 }//if(this->DlgSKOGraph!=NULL)
 if(this->DlgMouseCoord!=NULL)
 {
  DlgMouseCoord->EditX->setText(QString().setNum(x));
  DlgMouseCoord->EditY->setText(QString().setNum(y));
 }
 if(DlgAI!=NULL)
 {
  DlgAI->XMouse=x;
  DlgAI->YMouse=y;
  DlgAI->FillMouseCoord(x,y);
  ui->DNWPic->IsDlgAIHidden=DlgAI->isHidden();
  ui->DNWPic->repaint();
 }

 this->XMouse=x;
 this->YMouse=y;
}

void DNSpecBath::on_CreateSpecGraph_triggered(bool checked)
{
 if(this->DlgSG==NULL)
 {
  DlgSG=new DlgSpecGraph(this);
  connect(ui->DNWPic,SIGNAL(MouseDoubleCliced(int,int)),this->DlgSG,SLOT(MouseDoubleCliced(int,int)));
 }
 if(this->DlgSG->isHidden())
 {
  this->DlgSG->show();
  this->DlgSG->ThisPoly=this->SerPoly;
 }//if(this->DlgSG->isHidden())
}

void DNSpecBath::on_CreateSKOGraph_triggered()
{
 if(SerPoly!=NULL)
 {
  if(this->DlgSKOGraph!=NULL)
   delete this->DlgSKOGraph;

  QDialog *Dlg;
  Dlg=new QDialog(this);
  this->DlgSKOGraph=new Ui_DlgSKOGraph();
  this->DlgSKOGraph->setupUi(Dlg);

  DlgSKOGraph->ComboCh->clear();
  for(int i=0;i<SerPoly->Ch;i++)
  {
   DlgSKOGraph->ComboCh->addItem("Канал "+QString().setNum(i+1));
  }//for(int i=0;i<SerPoly->Ch;i++)
  DlgSKOGraph->EditFrom->setText("5");
  DlgSKOGraph->EditTo->setText("50");
  Dlg->show();
  connect(ui->DNWPic,SIGNAL(MouseLClicked(int,int)),this,SLOT(MouseLClicked(int,int)));
  connect(this->DlgSKOGraph->ButtonOK,SIGNAL(clicked()),this,SLOT(on_DlgSKOGraph_OK()));
 }//if(SerPoly!=NULL)
}

void DNSpecBath::on_DlgSKOGraph_OK()
{
// if(this->DlgG!=NULL)
//  delete this->DlgG;
 int xp,yp,nFrom,nTo,nCh;
 xp=DlgSKOGraph->EditX->text().toInt();
 yp=DlgSKOGraph->EditY->text().toInt();
 nFrom=DlgSKOGraph->EditFrom->text().toInt();
 nTo=DlgSKOGraph->EditTo->text().toInt();
 nCh=DlgSKOGraph->ComboCh->currentIndex();

 xp=xp-this->SerPoly->xn;
 yp=yp-this->SerPoly->yn;

 float *x,*y;
 x=new float[nTo-nFrom];
 y=new float[nTo-nFrom];

 for(int i=nFrom;i<nTo;i++)
 {
  x[i-nFrom]=i;
  y[i-nFrom]=this->SerPoly->GetSKOPoint(xp,yp,i,nCh);
 }
 if(this->DlgG==NULL)
 {
  this->DlgG=new DlgGraph(this);
     connect(this->DlgG,SIGNAL(rejected()),this,SLOT(DlgGraphDestroy()));
 }


 this->DlgG->AddGraph(x,y,nTo-nFrom);

 if(this->DlgG->isHidden())
  this->DlgG->show();
 this->DlgG->repaint();
}

void DNSpecBath::DlgGraphDestroy()
{
 delete this->DlgG;
 this->DlgG=NULL;
}

void DNSpecBath::on_OilWater_triggered()
{
 if(this->DlgMouseCoord!=NULL)
  delete this->DlgMouseCoord;

 QDialog *Dlg;
 Dlg=new QDialog(this);
 this->DlgMouseCoord=new Ui_DlgMouseCoord();
 this->DlgMouseCoord->setupUi(Dlg);

 connect(ui->DNWPic,SIGNAL(MouseLClicked(int,int)),this,SLOT(MouseLClicked(int,int)));
 connect(this->DlgMouseCoord->ButtonOK,SIGNAL(clicked()),this,SLOT(on_DlgMouseCoord_OK()));
}

void DNSpecBath::on_DlgMouseCoord_OK()
{
 int xp,yp;
 QList <QPoint> Pt;

 xp=DlgMouseCoord->EditX->text().toInt();
 yp=DlgMouseCoord->EditY->text().toInt();

 xp=xp-this->SerPoly->xn;
 yp=yp-this->SerPoly->yn;

 if(this->GdalImage->Lamda!=NULL)
 {
  float *SpectrPt,*SpectrZone;
  bool *MaskCh;

  int n510,n555;
  int n600,n1000;
  int n730,n980;
  int n590,n710;
  int n400,n405;
  int n550,n580;

  n600=GdalImage->DetermNCh(600);
  n1000=GdalImage->DetermNCh(1000);

//  if(SerPoly->Ch==SerPoly->IshCh)
//  {
//   QString Str;
//   Str='['+QString().setNum(n600)+'-'+QString().setNum(n1000)+']';
//   SerPoly->AddChanal();
//  }//if(SerPoly->Ch==SerPoly->IshCh)

  Pt=this->SerPoly->DefinePixels(5,xp,yp);
  MaskCh=new bool[SerPoly->Ch];
  for(int i=0;i<SerPoly->Ch;i++)
  {
   MaskCh[i]=true;
  }



  SpectrPt=new float[SerPoly->Ch];
  SpectrZone=new float[SerPoly->Ch];
  memset(SpectrZone,0,SerPoly->Ch*sizeof(float));

  for(int i=0;i<Pt.size();i++)
  {
   int x,y;
   x=Pt[i].x()+SerPoly->xn;
   y=Pt[i].y()+SerPoly->yn;
   SerPoly->GetSpectrPoint(x,y,SpectrPt,MaskCh);
   for(int j=0;j<SerPoly->Ch;j++)
   {
    SpectrZone[j]+=SpectrPt[j]/Pt.size();
   }//for(int j=0;j<SerPoly->Ch;j++)
  }//for(int i=0;i<Pt.size();i++)
  delete[] SpectrPt;
  delete[] MaskCh;
 }//if(this->GdalImage->Lamda!=NULL)
}

void DNSpecBath::on_IsoData_triggered()
{
 if(DlgID!=NULL)
  delete DlgID;

 this->DlgID=new DlgIsoData(this);
 this->DlgID->NameOrg=this->NameOrg;
 this->DlgID->NameProg=this->NameProg;

 this->DlgID->ThisPoly=this->SerPoly;
 this->DlgID->PathData=this->GdalImage->PathImgFile+'/'+this->GdalImage->NameImgFile;
 this->DlgID->FileNameChan=this->FileNameChan;

 this->DlgID->Lamda=GdalImage->Lamda;
 this->DlgID->IsChanSpecColibr=GdalImage->Lamda;

 DlgID->show();
 connect(this->DlgID,SIGNAL(ButtonOK()),this,SLOT(on_DlgIsoData_OK()));
}

void DNSpecBath::on_DlgIsoData_OK()
{
 this->SerPoly->IsoData(this->SerPoly->CurrentPoly,
                        DlgID->KolvoKl,
                        DlgID->MaxSKO,
                        DlgID->MinDist,
                        DlgID->KolvoIt,
                        DlgID->MaskCh);
 QDir Dir;
 if(!Dir.exists(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly))
  Dir.mkdir(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly);
 QFile FileClassMass(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+'/'+"IsoData"+".kls");
 FileClassMass.open(QIODevice::Truncate|QIODevice::WriteOnly);
 FileClassMass.write((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
 FileClassMass.close();

 this->FillMainForm();
}

void DNSpecBath::on_SAM_triggered()
{
 if(DlgSM!=NULL)
  delete DlgSM;

 this->DlgSM=new DlgSAM(this);

 this->DlgSM->ThisPoly=this->SerPoly;
 this->DlgSM->PathData=this->GdalImage->PathImgFile+'/'+this->GdalImage->NameImgFile;
 this->DlgSM->FileNameChan=this->FileNameChan;
 this->DlgSM->show();
 connect(this->DlgSM,SIGNAL(ButtonOK()),this,SLOT(on_DlgSAM_OK()));
}

void DNSpecBath::on_DlgSAM_OK()
{
 SerPoly->SAM(SerPoly->CurrentPoly,
              DlgSM->PolyFilesNames,
              DlgSM->IsSKOEnable,
              DlgSM->LimitSKO,DlgSM->MaskCh);
 disconnect(this->DlgSM,SIGNAL(ButtonOK()),this,SLOT(on_DlgSAM_OK()));

 DlgSM->close();
 QDir Dir;
 if(!Dir.exists(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly))
  Dir.mkdir(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly);
 QFile FileClassMass(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+'/'+"SAM"+".kls");
 FileClassMass.open(QIODevice::Truncate|QIODevice::WriteOnly);
 FileClassMass.write((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
 FileClassMass.close();

 this->FillMainForm();
}

void DNSpecBath::on_BayesClassif_triggered()
{
 if(DlgSM!=NULL)
  delete DlgSM;

 this->DlgSM=new DlgSAM(this);

 this->DlgSM->ThisPoly=this->SerPoly;
 this->DlgSM->PathData=this->GdalImage->PathImgFile+'/'+this->GdalImage->NameImgFile;
 this->DlgSM->FileNameChan=this->FileNameChan;
 this->DlgSM->show();
 connect(this->DlgSM,SIGNAL(ButtonOK()),this,SLOT(on_BayesClassif_OK()));
}

void DNSpecBath::on_BayesClassif_OK()
{
 SerPoly->MaxLike(SerPoly->CurrentPoly,
                  DlgSM->PolyFilesNames,
                  DlgSM->MaskCh);
 disconnect(this->DlgSM,SIGNAL(ButtonOK()),this,SLOT(on_DlgSAM_OK()));

 DlgSM->close();
 QDir Dir;
 if(!Dir.exists(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly))
  Dir.mkdir(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly);
 QFile FileClassMass(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+'/'+"MaxLike"+".kls");
 FileClassMass.open(QIODevice::Truncate|QIODevice::WriteOnly);
 FileClassMass.write((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
 FileClassMass.close();

 this->FillMainForm();
}

void DNSpecBath::on_MagicWand_triggered()
{
 if(DlgMW!=NULL)
  delete DlgMW;

 this->DlgMW=new DlgMagicWand(this);
 this->DlgMW->NameOrg=this->NameOrg;
 this->DlgMW->NameProg=this->NameProg;

 this->DlgMW->ThisPoly=this->SerPoly;
 this->DlgMW->show();
 connect(this->DlgMW,SIGNAL(ButtonOK()),this,SLOT(on_DlgMagicWand_OK()));
 connect(ui->DNWPic,SIGNAL(MouseLClicked(int,int)),this->DlgMW,SLOT(MouseClicked(int,int)));
}

void DNSpecBath::on_DlgMagicWand_OK()
{
 bool *NoBlackPixels2;
 NoBlackPixels2=new bool[this->SerPoly->W*this->SerPoly->H];
 this->SerPoly->MagicWand(this->SerPoly->CurrentPoly,
                          this->DlgMW->x,this->DlgMW->y,
                          this->DlgMW->Contrast,this->DlgMW->NumCh,
                          this->DlgMW->mode,NoBlackPixels2);

 QDir Dir;
 if(!Dir.exists(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly))
  Dir.mkdir(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly);
 QFile FileClassMass(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+'/'+"MagikWand"+".kls");
 FileClassMass.open(QIODevice::Truncate|QIODevice::WriteOnly);
 FileClassMass.write((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
 FileClassMass.close();

 this->FillMainForm();
 delete[] NoBlackPixels2;
}

void DNSpecBath::on_AddChanal_triggered()
{
 bool InputOk;
 QString Formula=QInputDialog::getText(this,"Добавить новый канал","Введите формулу",
                                       QLineEdit::Normal,"",&InputOk);
 if(!Formula.isEmpty())
 {
  this->SerPoly->AddChanal(Formula);
 }//if(InputOk && !NamePoly.isEmpty())
}

void DNSpecBath::on_AddIntegralChanal_triggered()
{
 bool InputOk;
 QString Formula=QInputDialog::getText(this,"Добавить Интегрированный канал","Введите формулу",
                                          QLineEdit::Normal,"",&InputOk);
 if(!Formula.isEmpty())
 {
  QStringList LamChan;
  LamChan=Formula.split("-");
  int NumCh1,NumCh2;
  float Lam1,Lam2, *LamMass;
  Lam1=LamChan[0].toFloat();
  Lam2=LamChan[1].toFloat();

  NumCh1=this->GdalImage->DetermNCh(Lam1);
  NumCh2=this->GdalImage->DetermNCh(Lam2);
  LamMass=new float[NumCh2-NumCh1+1];

  for(int i=NumCh1;i<=NumCh2;i++)
  {
   LamMass[i-NumCh1]=GdalImage->Lamda[i];
  }

  this->SerPoly->AddIntegralChanal(NumCh1,NumCh2,LamMass);

//  QMessageBox msg;
//  msg.setText(LamChan[1]);
//  msg.exec();
  //this->SerPoly->AddChanal(Formula);
 }//if(InputOk && !NamePoly.isEmpty())
}

void DNSpecBath::on_CreateImgPoly_triggered()
{
 if(DlgCIP!=NULL)
  delete DlgCIP;
 this->DlgCIP=new DlgCreateImgPoly(this);
 this->DlgCIP->ThisPoly=this->SerPoly;
 this->DlgCIP->show();
 connect(this->DlgCIP,SIGNAL(ButtonOK()),this,SLOT(on_DlgCreateImgPoly_OK()));
}

void DNSpecBath::on_DlgCreateImgPoly_OK()
{
 float *ImgPolyR,MinR,MaxR;
 float *ImgPolyG,MinG,MaxG;
 float *ImgPolyB,MinB,MaxB;

 ImgPolyR=new float[SerPoly->px];
 ImgPolyG=new float[SerPoly->px];
 ImgPolyB=new float[SerPoly->px];


 SerPoly->GetBrigthChanPoly(ImgPolyR,DlgCIP->NumChR,SerPoly->CurrentPoly);
 MinR=ImgPolyR[0];
 MaxR=ImgPolyR[0];

 SerPoly->GetBrigthChanPoly(ImgPolyG,DlgCIP->NumChG,SerPoly->CurrentPoly);
 MinG=ImgPolyG[0];
 MaxG=ImgPolyG[0];

 SerPoly->GetBrigthChanPoly(ImgPolyB,DlgCIP->NumChB,SerPoly->CurrentPoly);
 MinB=ImgPolyB[0];
 MaxB=ImgPolyB[0];



 for(quint64 i=1;i<SerPoly->px;i++)
 {
  if(MinR>=ImgPolyR[i])
   MinR=ImgPolyR[i];
  if(MinG>=ImgPolyG[i])
   MinG=ImgPolyG[i];
  if(MinB>=ImgPolyB[i])
   MinB=ImgPolyB[i];

  if(MaxR<=ImgPolyR[i])
   MaxR=ImgPolyR[i];
  if(MaxG<=ImgPolyG[i])
   MaxG=ImgPolyG[i];
  if(MaxB<=ImgPolyB[i])
   MaxB=ImgPolyB[i];
 }//for(quint64 i=1;i<SerPoly->px;i++)


 float KofR=1;
 float KofG=1;
 float KofB=1;

 if(MinR>=0.)
  KofR=255/MaxR;
 if(/*(MaxR-MinR)>255 &&*/ MinR<0.)
  KofR=255/(MaxR-MinR);

 if(MinG>=0.)
  KofG=255/MaxG;
 if(/*(MaxG-MinG)>255 &&*/ MinG<0.)
  KofG=255/(MaxG-MinG);

 if(MinB>=0.)
  KofB=255/MaxB;
 if(/*(MaxB-MinB)>255 && */MinB<0.)
  KofB=255/(MaxB-MinB);



 for(quint64 i=0;i<SerPoly->px;i++)
 {

  if(MinR>=0)
   ImgPolyR[i]*=KofR;
   //ui->DNWPic->Polygon.PImgR[i]=ImgPolyR[i]*KofR;
  else
  {
   ImgPolyR[i]=(ImgPolyR[i]-MinR)*KofR;
   if(ImgPolyR[i]<0)
    ImgPolyR[i]=0;
  }

  if(MinG>=0)
   ImgPolyG[i]*=KofG;
  else
  {
   ImgPolyG[i]=(ImgPolyG[i]-MinG)*KofG;
   if(ImgPolyG[i]<0)
    ImgPolyG[i]=0;
  }

  if(MinB>=0)
   ImgPolyB[i]*=KofB;
  else
  {
   ImgPolyB[i]=(ImgPolyB[i]-MinB)*KofB;
   if(ImgPolyB[i]<0)
    ImgPolyB[i]=0;
  }


  ImgPolyR[i]*=DlgCIP->KofR;
  ImgPolyG[i]*=DlgCIP->KofG;
  ImgPolyB[i]*=DlgCIP->KofB;
  if(ImgPolyR[i]>255) ImgPolyR[i]=255;
  if(ImgPolyG[i]>255) ImgPolyG[i]=255;
  if(ImgPolyB[i]>255) ImgPolyB[i]=255;

 }//for(quint64 i=1;i<SerPoly->px;i++)

 ui->DNWPic->Polygons[NumCurPoly()].CreateImg(ImgPolyR,ImgPolyG,ImgPolyB,SerPoly->MassPoly,SerPoly->px);

 delete[] ImgPolyR;
 delete[] ImgPolyG;
 delete[] ImgPolyB;
 ui->DNWPic->repaint();
}

void DNSpecBath::on_ClassFilter_triggered()
{
 if(DlgCF!=NULL)
  delete DlgCF;

 this->DlgCF=new DlgClassFilter(this);
 this->DlgCF->ThisPoly=this->SerPoly;
 this->DlgCF->WidgetChange=ui->DNWPic;
 this->DlgCF->show();
 connect(this->DlgCF,SIGNAL(ButtonOK()),this,SLOT(on_DlgClassFilter_OK()));
}

void DNSpecBath::on_DlgClassFilter_OK()
{
 ui->DNWPic->Polygons[NumCurPoly()].IsPolyClassif=this->SerPoly->IsPolyClassif;
 ui->DNWPic->Polygons[NumCurPoly()].ClassifMass=this->SerPoly->ClassifMass;

 QDir Dir;
 if(!Dir.exists(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly))
  Dir.mkdir(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly);
 QFile FileClassMass(this->SerPoly->PathTempFile+'/'+this->CurrentNamePoly+'/'+"ClassFilter"+".kls");
 FileClassMass.open(QIODevice::Truncate|QIODevice::WriteOnly);
 FileClassMass.write((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
 FileClassMass.close();

 this->FillMainForm();
 ui->DNWPic->update();
}

void DNSpecBath::on_SaveSerFileShift_triggered()
{
 QStringList TagsName;
 TagsName<<"SensivityCorrection"<<"DarkCorrection";

 GdalImage->GetXMLRadiometricCorr(TagsName,"RadiometricCorr");
 GdalImage->ShiftChanal(0,60,1,186,GdalImage->PathImgFile+'/'+"proba.ser");
}

void DNSpecBath::on_CreateAKFImg_triggered()
{
 if(DlgAI!=NULL)
  delete DlgAI;

 this->DlgAI=new DlgAKFImg(this);
 this->DlgAI->ThisPoly=this->SerPoly;
 this->DlgAI->show();
 connect(ui->DNWPic,SIGNAL(MouseLClicked(int,int)),this,SLOT(MouseLClicked(int,int)));
 connect(this->DlgAI,SIGNAL(ButtonOK()),this,SLOT(on_DlgCreateAKFImg_OK()));
 connect(this->DlgAI,SIGNAL(rejected()),this,SLOT(on_DlgCreateAKFImg_Rejected()));
 connect(this->DlgAI,SIGNAL(ComboSideSqClick(int)),this,SLOT(on_DlgCreateAKFImgCombo(int)));
}

void DNSpecBath::on_DlgCreateAKFImg_OK()
{
 SerPoly->AddAKFChanal(DlgAI->Side,DlgAI->NumCh,DlgAI->XMouse,DlgAI->YMouse);

 ui->DNWPic->IsDlgAIHidden=DlgAI->isHidden();
 ui->DNWPic->repaint();
}

void DNSpecBath::on_DlgCreateAKFImg_Rejected()
{
 ui->DNWPic->IsDlgAIHidden=DlgAI->isHidden();
 ui->DNWPic->repaint();
}

void DNSpecBath::on_DlgCreateAKFImgCombo(int SideSq)
{
 ui->DNWPic->DlgAiSizeSide=SideSq;
 ui->DNWPic->repaint();
}

/********************************************************************************************/
void DNSpecBath::FillMainForm()
{
 this->IsTreePolygonClear=TRUE;
 int i;
 QString DataString1,DataString2;

 QStringList PolygonsFiles;
 QDir DirPolygons(this->GdalImage->PathImgFile+'/'+this->GdalImage->NameImgFile);

 QFileInfo *InfoPolygonsName;
 ui->ComboR->clear();
 ui->ComboG->clear();
 ui->ComboB->clear();

//Заполнение параметров изображения
 for(i=0;i<this->GdalImage->Ch;i++)
 {
  DataString2.setNum((float)i+1,'d',0);
  DataString1=tr("Канал ")+DataString2;

  if(GdalImage->IsChanSpecColibr)
   DataString1+=" ("+QString().setNum(GdalImage->Lamda[i],'d',1)+")";


  ui->ComboR->addItem(DataString1);
  ui->ComboG->addItem(DataString1);
  ui->ComboB->addItem(DataString1);
 }
 ui->ComboR->setCurrentIndex(this->nCh[0]);
 ui->ComboG->setCurrentIndex(this->nCh[1]);
 ui->ComboB->setCurrentIndex(this->nCh[2]);

 DataString1.setNum((float)this->BKof[0],'d',1);
 ui->EditKR->setText(DataString1);
 DataString1.setNum((float)this->BKof[1],'d',1);
 ui->EditKG->setText(DataString1);
 DataString1.setNum((float)this->BKof[2],'d',1);
 ui->EditKB->setText(DataString1);

//Заполнение дерева полигонов и сохранение имён полигонов
// DataString1="*.ser";
 PolygonsFiles=DirPolygons.entryList(QStringList()<<"*.pol",QDir::Files,QDir::Name);
 ui->treePolygons->setHeaderLabel(tr("Полигоны"));
 ui->treePolygons->clear();
 this->NamesPoly.clear();

 foreach(DataString1,PolygonsFiles)
 {
//  QTreeWidgetItem *TreePolygons=new QTreeWidgetItem(ui->treePolygons);
  InfoPolygonsName=new QFileInfo(DataString1);
  DataString2=InfoPolygonsName->baseName();
  delete InfoPolygonsName;
//  TreePolygons->setText(0,DataString2);
  this->NamesPoly<<DataString2;
 }//foreach(DataString1,PolygonsFiles)
 this->FillTreePolygons(this->GdalImage->PathImgFile+'/'+this->GdalImage->NameImgFile,ui->treePolygons);
}

/*Заполнить дерево полигонов*/
void DNSpecBath::FillTreePolygons(QString StartPath,QTreeWidgetItem *ItemParent)
{
 QDir CurDir(StartPath),Dir;
 QStringList Files;
 QString DataString,DataString2;
 QFileInfo *InfoPolygonsName;

 Files=CurDir.entryList(QStringList()<<"*.pol"<<"*.kls",QDir::Files,QDir::Name);
 foreach(DataString,Files)
 {
  QTreeWidgetItem *TreePolygons=new QTreeWidgetItem(ItemParent);
  InfoPolygonsName=new QFileInfo(DataString);
  DataString2=InfoPolygonsName->baseName();
  delete InfoPolygonsName;
  TreePolygons->setText(0,DataString2);
  if(Dir.exists(StartPath+'/'+DataString2))
  {
   this->FillTreePolygons(StartPath+'/'+DataString2,TreePolygons);
  }
 }//foreach(DataString1,PolygonsFiles)

}
void DNSpecBath::FillTreePolygons(QString StartPath,QTreeWidget *ItemParent)
{
 QDir CurDir(StartPath),Dir;
 QStringList Files;
 QString DataString,DataString2;
 QFileInfo *InfoPolygonsName;

 Files=CurDir.entryList(QStringList()<<"*.pol"<<"*.kls",QDir::Files,QDir::Name);
 foreach(DataString,Files)
 {
  QTreeWidgetItem *TreePolygons=new QTreeWidgetItem(ItemParent);
  InfoPolygonsName=new QFileInfo(DataString);
  DataString2=InfoPolygonsName->baseName();
  delete InfoPolygonsName;
  TreePolygons->setText(0,DataString2);
  if(Dir.exists(StartPath+'/'+DataString2))
  {
   this->FillTreePolygons(StartPath+'/'+DataString2,TreePolygons);
  }
 }//foreach(DataString1,PolygonsFiles)
}
/*Прочесть файл полигона и отобразить информацию в окне*/
void DNSpecBath::ReadFilePoly(QString PolyFileName)
{
 QFileInfo *fInfo;
 fInfo=new QFileInfo(PolyFileName);
 this->CurrentNamePoly=fInfo->baseName();
 delete fInfo;
 if(this->SerPoly!=NULL)
  delete this->SerPoly;

 this->SerPoly= new DNPoly2(PolyFileName);
 this->SerPoly->IshCh=this->GdalImage->Ch;



// QMessageBox msg;
// msg.setText(QString().setNum(NumCurPoly()));
// msg.exec();

 ui->DNWPic->Polygons[NumCurPoly()].IsPolyClassif=this->SerPoly->IsPolyClassif;
 ui->DNWPic->Polygons[NumCurPoly()].ClassifMass=this->SerPoly->ClassifMass;
}

void DNSpecBath::ReadFileClassif(QString PolyFileClassif)
{
 QFile FileClassif(PolyFileClassif);

 if(this->SerPoly->ClassifMass!=NULL)
  delete[] this->SerPoly->ClassifMass;

 this->SerPoly->ClassifMass=new int[this->SerPoly->W*this->SerPoly->H];
 FileClassif.open(QIODevice::ReadOnly);
 FileClassif.read((char*)this->SerPoly->ClassifMass,sizeof(int)*this->SerPoly->W*this->SerPoly->H);
 FileClassif.close();

 this->SerPoly->IsPolyClassif=TRUE;
 for(quint64 i=0;i<this->SerPoly->W*this->SerPoly->H;i++)
 {
  this->SerPoly->CurrentPoly[i]=FALSE;
  if(this->SerPoly->ClassifMass[i]>=0)
   this->SerPoly->CurrentPoly[i]=TRUE;
 }
 ui->DNWPic->Polygons[NumCurPoly()].IsPolyClassif=this->SerPoly->IsPolyClassif;
 ui->DNWPic->Polygons[NumCurPoly()].ClassifMass=this->SerPoly->ClassifMass;
}

/*Перевод из имени полигона в полное имя файла полигона*/
QString DNSpecBath::NamePolyToFile(QString NamePoly)
{
 QString DataString,DataString2;
 DataString=this->GdalImage->PathImgFile+'/'+this->GdalImage->NameImgFile+'/'+NamePoly+".pol";
 DataString2=QObject::tr(qPrintable(DataString));
 return DataString2;
}
/*Заполнить стэк полигонов*/
void DNSpecBath::FillStackPolygons()
{
 int ip;
 DNImgPoly Pol;
 DNPoly2 *Ser;
 ui->DNWPic->Polygons.clear();
 for(ip=0;ip<this->NamesPoly.size();ip++)
 {
//  if(this->NamesPoly[ip]!=this->CurrentNamePoly)
//  {
  Ser=new DNPoly2(this->NamePolyToFile(this->NamesPoly[ip]));
  Pol.pt=Ser->pt;
  ui->DNWPic->Polygons<<Pol;
  delete Ser;
//  }//if(this->NamesPoly[ip]!=this->CurrentNamePoly)
 }//for(ip=0;ip<this->NamesPoly.size();ip++)
}
/*Событие изменения текущего полигона*/
void DNSpecBath::ChangeCurrentPoly()
{
 if(NumCurPoly()>=0)
  ui->DNWPic->ChangeCurPoly(NumCurPoly());

 if(this->SerPoly!=NULL)
  delete this->SerPoly;

 this->SerPoly=new DNPoly2(this->NamePolyToFile(this->CurrentNamePoly));
 this->SerPoly->IshCh=this->GdalImage->Ch;

 if(this->DlgSG!=NULL)
  this->DlgSG->ThisPoly=this->SerPoly;
 if(this->DlgMW!=NULL)
  this->DlgMW->ThisPoly=this->SerPoly;
 if(this->DlgID!=NULL)
  this->DlgID->ThisPoly=this->SerPoly;
 if(this->DlgSM!=NULL)
  this->DlgSM->ThisPoly=this->SerPoly;
 if(this->DlgCIP!=NULL)
  this->DlgCIP->ThisPoly=this->SerPoly;
}
int DNSpecBath::NumCurPoly()
{
 int nCurPoly=-1;
 if(NamesPoly.size()>0)
  nCurPoly=0;
 for(int i=0;i<NamesPoly.size();i++)
 {
  if(NamesPoly[i]==CurrentNamePoly)
   break;
  nCurPoly++;
 }
 return nCurPoly;
}

void DNSpecBath::on_ForOlga_triggered()
{
 float *DataSpec;
 bool *MaskCh;
 MaskCh=new bool[SerPoly->Ch];
 for(int i=0; i<SerPoly->Ch; i++)
 {
  MaskCh[i]=true;
 }

 int KolfoPoint=0;
// for(int x=0;x<SerPoly->W;x++)
// {
//  for(int y=0;y<SerPoly->H;y++)
//  {
//   if(SerPoly->ClassifMass[x+y*SerPoly->W]>=0)
//   {
    DataSpec=new float[SerPoly->Ch];
    SerPoly->GetSpectrPoint(85,752,DataSpec,MaskCh);
    KolfoPoint++;
//   }
//   if(KolfoPoint>=20)
//    break;
//  }
//  if(KolfoPoint>=20)
//   break;
// }

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


   for(int i=0;i<SerPoly->Ch;i++)
   {
    out<<QString().setNum(i);
    out<<"\t"<<QString().setNum(DataSpec[i],'d',4).replace(".",",");
    out<<endl;
   }//for(int i=0;i<this->Ch;i++)
   FileTxt.close();
  }//if(!FileName.isEmpty())
}

