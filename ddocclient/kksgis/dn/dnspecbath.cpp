#include "dnspecbath.h"
#include "dn/dnwidgetimage.h"

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
 this->NameProg="SuperProga";
 this->NameOrg="VKA";
 this->AppDir=QDir::currentPath();
 this->IsTreePolygonClear=FALSE;
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
 wMW=wMW-200;
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

 this->FileNameOpen=QFileDialog::getOpenFileName(this,"Открыть файл проекта","","Raster files (*.img *.asc *.tif *tiff *.bmp *.jpg *.jpeg);;Geotiff (*.tif *.tiff)");
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

  ui->DNWPic->img=GdalImage->GenerateImg(nCh[0],nCh[1],nCh[2],BKof[0],BKof[1],BKof[2],1);
  ui->DNWPic->isImgLoad=true;

  this->FillMainForm();


    //Если существуют полигоны в проекте, то делаем первый полигон текущим
   if(this->NamesPoly.size()>0)
   {
    this->CurrentNamePoly=this->NamesPoly[0];
    this->CurrentNameClassif='/'+this->CurrentNamePoly;
    this->ChangeCurrentPoly();
    this->ReadFilePoly(this->NamePolyToFile(this->CurrentNamePoly));
   }

   this->FillStackPolygons();
   ui->DNWPic->repaint();
//     ui->WPic->ChangedBMP(this->ThisFileProj.FoolNameImageStart);
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
  ui->DNWPic->img=GdalImage->GenerateImg(nCh[0],nCh[1],nCh[2],BKof[0],BKof[1],BKof[2],1);
  ui->DNWPic->isImgLoad=true;
  this->FillMainForm();
  if(this->NamesPoly.size()>0)
  {
   this->CurrentNamePoly=this->NamesPoly[0];
   this->CurrentNameClassif='/'+this->CurrentNamePoly;
   this->ChangeCurrentPoly();
   this->ReadFilePoly(this->NamePolyToFile(this->CurrentNamePoly));
  }

  this->FillStackPolygons();
  ui->DNWPic->repaint();
 }//if(!this->FileNameOpen.isEmpty())
}

void DNSpecBath::on_CreatePoly_triggered(bool checked)
{
 if(checked)
 {
  ui->DNWPic->Polygon.pt.clear();
  ui->DNWPic->Polygon.IsPolyClassif=FALSE;
 }

 ui->DNWPic->IsCretaePolyOn=checked;

//Сохранение полигона в файл (если снята галочка с меню Создать полигон и создаваемый полигон имеет больше двух точек)
 if(!checked && ui->DNWPic->Polygon.pt.size()>2)
 {
  bool InputOk;
  QString NamePoly=QInputDialog::getText(this,"Создать полигон","Введите имя полигона",
                                         QLineEdit::Normal,"Polygon_"+QString().setNum(ui->DNWPic->Polygons.size()+1),
                                         &InputOk);
  if(InputOk && !NamePoly.isEmpty())
  {
   ui->DNWPic->Polygons<<ui->DNWPic->Polygon;
   QPoint nP,kP;
   nP=ui->DNWPic->Polygon.GetMinP();
   kP=ui->DNWPic->Polygon.GetMaxP();
   this->GdalImage->GenerateSpectrFile(nP.x(),nP.y(),
                                       kP.x()-nP.x(),kP.y()-nP.y(),
                                       ui->DNWPic->Polygon.pt,NamePoly);
   ui->DNWPic->Polygon.pt.clear();
   ui->DNWPic->Polygon.IsPolygonSelect=FALSE;

   this->CurrentNamePoly=NamePoly;
   this->ChangeCurrentPoly();
   this->ReadFilePoly(this->NamePolyToFile(this->CurrentNamePoly));
   this->FillMainForm();
  }//if(InputOk && !NamePoly.isEmpty())
 }//if(!checked && ui->DNWPic->Polygon.pt.size()>2)

 ui->DNWPic->repaint();
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

void DNSpecBath::on_treePolygons_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
 QString FileNamePoly;
 QTreeWidgetItem *itemP;
 itemP=current;
 if(!this->IsTreePolygonClear)
 {
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

  this->ReadFilePoly(FileNamePoly);
  this->FillStackPolygons();
  this->ChangeCurrentPoly();

  //Если текущая позиция является результатом классификации
  if(this->IsCurPolyClassif)
  {
   this->ReadFileClassif(this->GdalImage->PathImgFile+'/'+this->GdalImage->NameImgFile+this->CurrentNameClassif+".kls");
  }
  ui->DNWPic->repaint();
 }//if(!this->IsTreePolygonClear)
 else
 {
  this->IsTreePolygonClear=FALSE;
 }
}

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
  Polygons=this->SerPoly->RastrToVector(GDSt.XTopLeftPix,GDSt.XD,GDSt.XAngle,GDSt.YTopLeftPix,GDSt.YD,GDSt.YAngle,SerPoly->KofV,SerPoly->MinV, this->FileNameOpen);

  ui->pbVectorize->setEnabled(true);
  ui->statusBar->showMessage(tr("Расчет спектральной батинометрии завершен"));
  QApplication::restoreOverrideCursor();

  this->FillMainForm();

 }//if(this->SerPoly!=NULL)
}

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

 this->SerPoly= new DNPoly(PolyFileName);
 this->SerPoly->IshCh=this->GdalImage->Ch;

 if(ui->DNWPic->Polygon.pt.size()>0)
  ui->DNWPic->Polygon.pt.clear();

 ui->DNWPic->Polygon.pt=this->SerPoly->pt;
// QMessageBox msg;
// msg.setText(QString().setNum(ui->DNWPic->Polygon.pt.size()));
// msg.exec();
 ui->DNWPic->Polygon.IsPolyClassif=this->SerPoly->IsPolyClassif;
 ui->DNWPic->Polygon.ClassifMass=this->SerPoly->ClassifMass;
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
 ui->DNWPic->Polygon.IsPolyClassif=this->SerPoly->IsPolyClassif;
 ui->DNWPic->Polygon.ClassifMass=this->SerPoly->ClassifMass;
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
 DNPoly *Ser;
 for(ip=0;ip<this->NamesPoly.size();ip++)
 {
//  if(this->NamesPoly[ip]!=this->CurrentNamePoly)
//  {
  Ser=new DNPoly(this->NamePolyToFile(this->NamesPoly[ip]));
  Pol.pt=Ser->pt;
  ui->DNWPic->Polygons<<Pol;
  delete Ser;
//  }//if(this->NamesPoly[ip]!=this->CurrentNamePoly)
 }//for(ip=0;ip<this->NamesPoly.size();ip++)
}
/*Событие изменения текущего полигона*/
void DNSpecBath::ChangeCurrentPoly()
{
 if(this->SerPoly!=NULL)
  delete this->SerPoly;

 this->SerPoly=new DNPoly(this->NamePolyToFile(this->CurrentNamePoly));
 this->SerPoly->IshCh=this->GdalImage->Ch;
// this->DlgPP->ThisPoly=this->SerPoly;
// this->DlgSG->ThisPoly=this->SerPoly;
// this->DlgMW->ThisPoly=this->SerPoly;
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
     y>=this->SerPoly->yn && y<this->SerPoly->yn+this->SerPoly->H &&
     this->SerPoly->IsPolyClassif)
  {
   bool *MaskCh=new bool[this->SerPoly->Ch];
   for(int i=0;i<this->SerPoly->Ch;i++)
   {
    MaskCh[i]=FALSE;
    if(i==this->SerPoly->Ch-1)
     MaskCh[i]=TRUE;
   }//for(int i=0;i<this->SerPoly->Ch;i++)
   float Brigth;
   this->SerPoly->GetSpectrPoint(x,y,&Brigth,MaskCh);
   DlgShowDepth->EditDeptch->setText(QString().setNum(Brigth,'d',4));
   delete[] MaskCh;
  }
 }//if(DlgShowDepth!=NULL)
}

void DNSpecBath::on_DNSpecBath_destroyed()
{

}
