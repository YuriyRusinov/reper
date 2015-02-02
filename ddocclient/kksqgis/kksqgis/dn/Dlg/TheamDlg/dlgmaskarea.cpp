#include "dlgmaskarea.h"
#include "ui_dlgmaskarea.h"

DlgMaskArea::DlgMaskArea(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMaskArea)
{
    ui->setupUi(this);
    ui->ComboTypeFone->clear();
//    ui->ComboTypeFone->addItem("Песок");
//    ui->ComboTypeFone->addItem("Снег");
//    ui->ComboTypeFone->addItem("Растительность");

    //Подключение БД
    BDSpec=new DNBDSpectr("QPSQL","REPER","repermaster","localhost","master");
}

DlgMaskArea::~DlgMaskArea()
{
 delete ui;
}

void DlgMaskArea::showEvent(QShowEvent *)
{
 QSettings Settings(this->NameOrg,this->NameProg);

 float DeltaMS=Settings.value("MaskArea/DeltaMS",0).toFloat();
 float DeltaGS=Settings.value("MaskArea/DeltaGS",0).toFloat();
 float DeltaSub=Settings.value("MaskArea/DeltaSub",0).toFloat();
 //float MaxDeriviant=Settings.value("MaskArea/MaxDeriviant",0).toFloat();
 MaxDeriviant=-1;

 ui->EditDeltaMS->setText(QString().setNum(DeltaMS,'d',3));
 ui->EditDeltaGS->setText(QString().setNum(DeltaGS,'d',3));
 ui->EditDeltaSub->setText(QString().setNum(DeltaSub,'d',3));
// ui->EditMaxDeriviant->setText(QString().setNum(MaxDeriviant,'d',3));

 ui->ListNameMask->clear();
 for(int i=0;i<ListNameMask.size();i++)
 {
  QListWidgetItem *WItem;
  WItem=new QListWidgetItem(/*ui->ListNameMask*/ListNameMask[i]);
  //ui->ListNameMask->setItemWidget(WItem,new QRadioButton(ListNameMask[i]));
  //WItem->setCheckState(Qt::Unchecked);
  //WItem->setFlags(Qt::ItemIsSelectable);
  ui->ListNameMask->addItem(WItem);
 }
 ui->ListNameMask->setCurrentRow(0);

 QList <DNBDSpectr::NameAndClsIdFones> Fones;

 Fones=BDSpec->ReadFones("Средства пассивной маскировки ВВСТ",ui->ListNameMask->currentItem()->text());

 ui->ComboTypeFone->clear();
 QList <QString> MainFonesCls;
 MainFonesCls.clear();
 for(int i=0;i<Fones.size();i++)
 {
  ui->ComboTypeFone->addItem(Fones[i].NameFone);
  for(int j=0;j<Fones[i].ClsIdsFone.size();j++)
  {
   MainFonesCls<<Fones[i].ClsIdsFone[j];
  }//for(int i=0;i<Fones[0].ClsIdsFone.size();i++)
 }//for(int i=0;i<Fones.size();i++)

 ui->ComboTypeFone->setCurrentIndex(0);

 QList <QString> MaskType;
 QString Condition="clsid="+QString().setNum(ListClsIdNameMask[0]);
 Condition+=" AND classpropid=10";
 MaskType=BDSpec->ReadData("classvaltab","classval",Condition);
 float MSIndex;
 float MSIndex2=-1000;
 float GSIndex;

 if(MaskType[0]=="травяной")
 {
  this->TypeFone=2;

  QList <float> BrNir1;
  QList <float> BrRed;
  QList <float> BrGren;
  QList <float> NDVI;
  QList <float> TCHVI;

  QList <float> NDVIre;
  QList <float> R750;
  QList <float> R705;

  R750=BDSpec->GetBrightLamda(750,QString().setNum(ListClsIdNameMask[0]),Fones[0].ClsIdsFone);
  R705=BDSpec->GetBrightLamda(705,QString().setNum(ListClsIdNameMask[0]),Fones[0].ClsIdsFone);

  for(int i=0;i<R750.size();i++)
   NDVIre<<(R750[i]-R705[i])/(R750[i]+R705[i]);

  GSIndex=0;
  for(int i=0;i<NDVIre.size();i++)
   GSIndex+=NDVIre[i]/NDVIre.size();

  BrNir1=BDSpec->GetBrightIntegral(LamNir1MS1,LamNir1MS2,QString().setNum(ListClsIdNameMask[0]),Fones[0].ClsIdsFone);
  BrRed=BDSpec->GetBrightIntegral(LamRedMS1,LamRedMS2,QString().setNum(ListClsIdNameMask[0]),Fones[0].ClsIdsFone);
  BrGren=BDSpec->GetBrightIntegral(LamGrMS1,LamGrMS2,QString().setNum(ListClsIdNameMask[0]),Fones[0].ClsIdsFone);

  for(int i=0;i<BrNir1.size();i++)
   NDVI<<(BrNir1[i]-BrRed[i])/(BrNir1[i]+BrRed[i]);

  MSIndex=0;
  for(int i=0;i<NDVI.size();i++)
   MSIndex+=NDVI[i]/NDVI.size();

  for(int i=0;i<BrNir1.size();i++)
   TCHVI<<((BrRed[i]-BrGren[i])-(BrNir1[i]-BrRed[i]))/((BrRed[i]-BrGren[i])+(BrNir1[i]-BrRed[i]));

  MSIndex2=0;
  for(int i=0;i<TCHVI.size();i++)
   MSIndex2+=TCHVI[i]/TCHVI.size();

  QList <SpecGraph> Graph;
  Graph=BDSpec->ReadPartSpectrsObj(QString().setNum(ListClsIdNameMask[0]),MainFonesCls,700,735);

  QList <float> BrDev1;
  QList <float> BrDev2;
  BrDev1=BDSpec->GetBrightLamda(535,QString().setNum(ListClsIdNameMask[0]),MainFonesCls);
  BrDev2=BDSpec->GetBrightLamda(585,QString().setNum(ListClsIdNameMask[0]),MainFonesCls);
  MaxDeriviant=-1;
  for(int i=0;i<Graph.size();i++)
  {
   if(BDSpec->GetNumMeasureMent(Graph[i],700,735)>2)
   {
    float PromMaxDer;
    float *Lam,*Brigth;


    Lam=new float[Graph[i].Lam.size()];
    Brigth=new float[Graph[i].Lam.size()];
    for(int j=0;j<Graph[i].Lam.size();j++)
    {
     Lam[j]=Graph[i].Lam[j];
     Brigth[j]=Graph[i].KSJ[j];
    }
    //DNMathAdd MathAdd;
    //float *BrigthDeriv;
    //BrigthDeriv=MathAdd.GetDeriviant(Graph[i].Lam.size(),Brigth,Lam);
    //PromMaxDer=MathAdd.GetMax(BrigthDeriv,Graph[i].Lam.size()-1);
    PromMaxDer=BrDev1[i]-BrDev2[i];
    if(MaxDeriviant<PromMaxDer);
     MaxDeriviant=PromMaxDer;
    delete[] Lam;
    delete[] Brigth;
    //delete[] BrigthDeriv;
   }//if(BDSpec->GetNumMeasureMent(Graph[i])>2)
  }//for(int i=0;i<Graph.size();i++)

 }//if(MaskType=="травяной")

 if(MaskType[0]=="песчаный")
 {
  this->TypeFone=0;

  QList <float> BrNir1;
  QList <float> BrRed;
  QList <float> BrGren;
  QList <float> TCHVI;

  QList <float> NDVIre;
  QList <float> R750;
  QList <float> R705;

  R750=BDSpec->GetBrightLamda(750,QString().setNum(ListClsIdNameMask[0]),Fones[0].ClsIdsFone);
  R705=BDSpec->GetBrightLamda(705,QString().setNum(ListClsIdNameMask[0]),Fones[0].ClsIdsFone);

  for(int i=0;i<R750.size();i++)
   NDVIre<<(R750[i]-R705[i])/(R750[i]+R705[i]);

  GSIndex=0;
  for(int i=0;i<NDVIre.size();i++)
   GSIndex+=NDVIre[i]/NDVIre.size();

  BrNir1=BDSpec->GetBrightIntegral(LamNir1MS1,LamNir1MS2,QString().setNum(ListClsIdNameMask[0]),Fones[0].ClsIdsFone);
  BrRed=BDSpec->GetBrightIntegral(LamRedMS1,LamRedMS2,QString().setNum(ListClsIdNameMask[0]),Fones[0].ClsIdsFone);
  BrGren=BDSpec->GetBrightIntegral(LamGrMS1,LamGrMS2,QString().setNum(ListClsIdNameMask[0]),Fones[0].ClsIdsFone);

  for(int i=0;i<BrNir1.size();i++)
   TCHVI<<((BrRed[i]-BrGren[i])-(BrNir1[i]-BrRed[i]))/((BrRed[i]-BrGren[i])+(BrNir1[i]-BrRed[i]));

  MSIndex=0;
  for(int i=0;i<TCHVI.size();i++)
   MSIndex+=TCHVI[i]/TCHVI.size();

  QList <SpecGraph> Graph;
  Graph=BDSpec->ReadPartSpectrsObj(QString().setNum(ListClsIdNameMask[0]),MainFonesCls,550,650);

  QList <float> BrDev1;
  QList <float> BrDev2;
  BrDev1=BDSpec->GetBrightLamda(550,QString().setNum(ListClsIdNameMask[0]),MainFonesCls);
  BrDev2=BDSpec->GetBrightLamda(650,QString().setNum(ListClsIdNameMask[0]),MainFonesCls);

  MaxDeriviant=-1;
  for(int i=0;i<Graph.size();i++)
  {
   if(BDSpec->GetNumMeasureMent(Graph[i],550,650)>2)
   {
    float PromMaxDer;
    float *Lam,*Brigth;
    Lam=new float[Graph[i].Lam.size()];
    Brigth=new float[Graph[i].Lam.size()];
    for(int j=0;j<Graph[i].Lam.size();j++)
    {
     Lam[j]=Graph[i].Lam[j];
     Brigth[j]=Graph[i].KSJ[j];
    }
    //DNMathAdd MathAdd;
    //float *BrigthDeriv;
    //BrigthDeriv=MathAdd.GetDeriviant(Graph[i].Lam.size(),Brigth,Lam);
    //PromMaxDer=MathAdd.GetMax(BrigthDeriv,Graph[i].Lam.size()-1);
    PromMaxDer=BrDev1[i]-BrDev2[i];
    if(MaxDeriviant<PromMaxDer);
     MaxDeriviant=PromMaxDer;
    delete[] Lam;
    delete[] Brigth;
    //delete[] BrigthDeriv;
   }//if(BDSpec->GetNumMeasureMent(Graph[i])>2)
  }//for(int i=0;i<Graph.size();i++)

 }//if(MaskType=="песчаный")

 if(MaskType[0]=="снежный")
 {
  this->TypeFone=1;

  QList <float> BrNir1;
  QList <float> BrNir2;
  QList <float> SN;

  QList <float> SNgs;
  QList <float> R750;
  QList <float> R1000;

  R750=BDSpec->GetBrightLamda(750,QString().setNum(ListClsIdNameMask[0]),Fones[0].ClsIdsFone);
  R1000=BDSpec->GetBrightLamda(1000,QString().setNum(ListClsIdNameMask[0]),Fones[0].ClsIdsFone);

  for(int i=0;i<R750.size();i++)
   SNgs<<(R1000[i]-R750[i])/(R1000[i]+R750[i]);

  GSIndex=0;
  for(int i=0;i<SNgs.size();i++)
   GSIndex+=SNgs[i]/SNgs.size();

  BrNir1=BDSpec->GetBrightIntegral(LamNir1MS1,LamNir1MS2,QString().setNum(ListClsIdNameMask[0]),Fones[0].ClsIdsFone);
  BrNir2=BDSpec->GetBrightIntegral(LamNir2MS1,LamNir2MS2,QString().setNum(ListClsIdNameMask[0]),Fones[0].ClsIdsFone);


  for(int i=0;i<BrNir1.size();i++)
  {
   SN<<(BrNir2[i]-BrNir1[i])/(BrNir2[i]+BrNir1[i]);
  }

  MSIndex=0;
  for(int i=0;i<SN.size();i++)
   MSIndex+=SN[i]/SN.size();
 }//if(MaskType=="снежный")

 ui->EditMSIndexPor->setText(QString().setNum(MSIndex,'d',3));
 ui->EditGSIndexPor->setText(QString().setNum(GSIndex,'d',3));
 ui->EditMaxDeriviant->setText(QString().setNum(MaxDeriviant,'d',3));

 ClsId=QString().setNum(ListClsIdNameMask[0]);
 ClsBackgroId.clear();
 for(int i=0;i<Fones[0].ClsIdsFone.size();i++)
  ClsBackgroId<<Fones[0].ClsIdsFone[i];
}

void DlgMaskArea::on_ButtonOK_clicked()
{
 GSIndexPor=ui->EditGSIndexPor->text().replace(",",".").toFloat();
 MSIndexPor=ui->EditMSIndexPor->text().replace(",",".").toFloat();
 DeltaGS=ui->EditDeltaGS->text().replace(",",".").toFloat();
 DeltaMS=ui->EditDeltaMS->text().replace(",",".").toFloat();
 DeltaSub=ui->EditDeltaSub->text().replace(",",".").toFloat();
 MaxDeriviant=ui->EditMaxDeriviant->text().replace(",",".").toFloat();

// QMessageBox msg;
// msg.setText(QString().setNum(DeltaSub,'d',3));
// msg.exec();


 //Сохраняем параметры по умолчанию
 QSettings Settings(this->NameOrg,this->NameProg);
 Settings.setValue("MaskArea/DeltaMS",DeltaMS);
 Settings.setValue("MaskArea/DeltaGS",DeltaGS);
 Settings.setValue("MaskArea/DeltaSub",DeltaSub);
 Settings.setValue("MaskArea/MaxDeriviant",DeltaSub);


 emit ButtonOK();
}

void DlgMaskArea::on_ListNameMask_itemClicked(QListWidgetItem *item)
{
 QList <DNBDSpectr::NameAndClsIdFones> Fones;
 Fones=BDSpec->ReadFones("Средства пассивной маскировки ВВСТ",item->text());

 ui->ComboTypeFone->clear();
 QList <QString> MainFonesCls;
 MainFonesCls.clear();
 for(int i=0;i<Fones.size();i++)
 {
  ui->ComboTypeFone->addItem(Fones[i].NameFone);
  for(int j=0;j<Fones[ui->ComboTypeFone->currentIndex()].ClsIdsFone.size();j++)
  {
   MainFonesCls<<Fones[i].ClsIdsFone[j];
  }//for(int i=0;i<Fones[0].ClsIdsFone.size();i++)
 }

 ui->ComboTypeFone->setCurrentIndex(0);

 QList <QString> MaskType;
 QString Condition="clsid="+QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]);
 Condition+=" AND classpropid=10";
 MaskType=BDSpec->ReadData("classvaltab","classval",Condition);
 float MSIndex;
 float GSIndex;
 MaxDeriviant=-1;

 if(MaskType[0]=="травяной")
 {
  this->TypeFone=2;

  QList <float> BrNir1;
  QList <float> BrRed;
  QList <float> NDVI;

  QList <float> NDVIre;
  QList <float> R750;
  QList <float> R705;

  R750=BDSpec->GetBrightLamda(750,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[ui->ComboTypeFone->currentIndex()].ClsIdsFone);
  R705=BDSpec->GetBrightLamda(705,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[ui->ComboTypeFone->currentIndex()].ClsIdsFone);

//  QMessageBox msg;
//  msg.setText("clsid="+QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()])+
//              "\nclsidFone="+Fones[ui->ComboTypeFone->currentIndex()].ClsIdsFone[0]+
//              "\nR750="+QString().setNum(R750[0],'d',3)+"\n"+"R705="+QString().setNum(R705[0],'d',3));
//  msg.exec();

  for(int i=0;i<R750.size();i++)
   NDVIre<<(R750[i]-R705[i])/(R750[i]+R705[i]);

  GSIndex=0;
  for(int i=0;i<NDVIre.size();i++)
   GSIndex+=NDVIre[i]/NDVIre.size();

  BrNir1=BDSpec->GetBrightIntegral(LamNir1MS1,LamNir1MS2,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[ui->ComboTypeFone->currentIndex()].ClsIdsFone);
  BrRed=BDSpec->GetBrightIntegral(LamRedMS1,LamRedMS2,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[ui->ComboTypeFone->currentIndex()].ClsIdsFone);

  for(int i=0;i<BrNir1.size();i++)
  {
   NDVI<<(BrNir1[i]-BrRed[i])/(BrNir1[i]+BrRed[i]);
  }

  MSIndex=0;
  for(int i=0;i<NDVI.size();i++)
   MSIndex+=NDVI[i]/NDVI.size();

  QList <SpecGraph> Graph;
  Graph=BDSpec->ReadPartSpectrsObj(QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),MainFonesCls,700,735);

  QList <float> BrDev1;
  QList <float> BrDev2;
  BrDev1=BDSpec->GetBrightLamda(535,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),MainFonesCls);
  BrDev2=BDSpec->GetBrightLamda(585,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),MainFonesCls);

  MaxDeriviant=-1;
  for(int i=0;i<Graph.size();i++)
  {
   if(BDSpec->GetNumMeasureMent(Graph[i],700,735)>2)
   {
    float PromMaxDer;
    float *Lam,*Brigth;
    Lam=new float[Graph[i].Lam.size()];
    Brigth=new float[Graph[i].Lam.size()];
    for(int j=0;j<Graph[i].Lam.size();j++)
    {
     Lam[j]=Graph[i].Lam[j];
     Brigth[j]=Graph[i].KSJ[j];
//     QMessageBox msg;
//     msg.setText(QString().setNum(Graph[i].KSJ[j],'d',3));
//     msg.exec();
    }
//    DNMathAdd MathAdd;
//    float *BrigthDeriv;
//    BrigthDeriv=MathAdd.GetDeriviant(Graph[i].Lam.size(),Brigth,Lam);
//    PromMaxDer=MathAdd.GetMax(BrigthDeriv,Graph[i].Lam.size()-1);
    PromMaxDer=BrDev1[i]-BrDev2[i];
    if(MaxDeriviant<PromMaxDer);
     MaxDeriviant=PromMaxDer;
    delete[] Lam;
    delete[] Brigth;
//    delete[] BrigthDeriv;
   }//if(BDSpec->GetNumMeasureMent(Graph[i],700,735)>2)
  }//for(int i=0;i<Graph.size();i++)
 }//if(MaskType=="травяной")

 if(MaskType[0]=="песчаный")
 {
  this->TypeFone=0;
  QList <float> BrNir1;
  QList <float> BrRed;
  QList <float> BrGren;
  QList <float> TCHVI;

  QList <float> NDVIre;
  QList <float> R750;
  QList <float> R705;

  R750=BDSpec->GetBrightLamda(750,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[ui->ComboTypeFone->currentIndex()].ClsIdsFone);
  R705=BDSpec->GetBrightLamda(705,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[ui->ComboTypeFone->currentIndex()].ClsIdsFone);

  for(int i=0;i<R750.size();i++)
   NDVIre<<(R750[i]-R705[i])/(R750[i]+R705[i]);

  GSIndex=0;
  for(int i=0;i<NDVIre.size();i++)
   GSIndex+=NDVIre[i]/NDVIre.size();

  BrNir1=BDSpec->GetBrightIntegral(LamNir1MS1,LamNir1MS2,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[ui->ComboTypeFone->currentIndex()].ClsIdsFone);
  BrRed=BDSpec->GetBrightIntegral(LamRedMS1,LamRedMS2,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[ui->ComboTypeFone->currentIndex()].ClsIdsFone);
  BrGren=BDSpec->GetBrightIntegral(LamGrMS1,LamGrMS2,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[ui->ComboTypeFone->currentIndex()].ClsIdsFone);

  for(int i=0;i<BrNir1.size();i++)
   TCHVI<<((BrRed[i]-BrGren[i])-(BrNir1[i]-BrRed[i]))/((BrRed[i]-BrGren[i])+(BrNir1[i]-BrRed[i]));

  MSIndex=0;
  for(int i=0;i<TCHVI.size();i++)
   MSIndex+=TCHVI[i]/TCHVI.size();

  QList <SpecGraph> Graph;
  Graph=BDSpec->ReadPartSpectrsObj(QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),MainFonesCls,550,650);
  QList <float> BrDev1;
  QList <float> BrDev2;
  BrDev1=BDSpec->GetBrightLamda(550,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),MainFonesCls);
  BrDev2=BDSpec->GetBrightLamda(650,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),MainFonesCls);

  MaxDeriviant=0;
  for(int i=0;i<Graph.size();i++)
  {
   if(BDSpec->GetNumMeasureMent(Graph[i],550,650)>2)
   {
    float PromMaxDer;
    float *Lam,*Brigth;
    Lam=new float[Graph[i].Lam.size()];
    Brigth=new float[Graph[i].Lam.size()];
    for(int j=0;j<Graph[i].Lam.size();j++)
   {
    Lam[j]=Graph[i].Lam[j];
    Brigth[j]=Graph[i].KSJ[j];
   }
//    DNMathAdd MathAdd;
//    float *BrigthDeriv;
//    BrigthDeriv=MathAdd.GetDeriviant(Graph[i].Lam.size(),Brigth,Lam);
//    PromMaxDer=MathAdd.GetMax(BrigthDeriv,Graph[i].Lam.size()-1);
    PromMaxDer=BrDev1[i]-BrDev2[i];
    if(MaxDeriviant<PromMaxDer);
     MaxDeriviant=PromMaxDer;
    delete[] Lam;
    delete[] Brigth;
    //delete[] BrigthDeriv;
   }//if(BDSpec->GetNumMeasureMent(Graph[i],550,650)>2)
  }//for(int i=0;i<Graph.size();i++)
 }//if(MaskType=="песчаный")

 if(MaskType[0]=="снежный")
 {
  this->TypeFone=1;
  QList <float> BrNir1;
  QList <float> BrNir2;
  QList <float> SN;

  QList <float> SNgs;
  QList <float> R750;
  QList <float> R1000;

  R750=BDSpec->GetBrightLamda(750,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[ui->ComboTypeFone->currentIndex()].ClsIdsFone);
  R1000=BDSpec->GetBrightLamda(1000,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[ui->ComboTypeFone->currentIndex()].ClsIdsFone);

  for(int i=0;i<R750.size();i++)
   SNgs<<(R1000[i]-R750[i])/(R1000[i]+R750[i]);

  GSIndex=0;
  for(int i=0;i<SNgs.size();i++)
   GSIndex+=SNgs[i]/SNgs.size();

  BrNir1=BDSpec->GetBrightIntegral(LamNir1MS1,LamNir1MS2,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[ui->ComboTypeFone->currentIndex()].ClsIdsFone);
  BrNir2=BDSpec->GetBrightIntegral(LamNir2MS1,LamNir2MS2,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[ui->ComboTypeFone->currentIndex()].ClsIdsFone);

  for(int i=0;i<BrNir1.size();i++)
  {
   //   QMessageBox msg;
   //   msg.setText(QString().setNum(LamNir2MS1,'d',3));
   //   msg.exec();
   SN<<(BrNir2[i]-BrNir1[i])/(BrNir2[i]+BrNir1[i]);
  }

  MSIndex=0;
  for(int i=0;i<SN.size();i++)
   MSIndex+=SN[i]/SN.size();
 }//if(MaskType=="снежный")
 ui->EditMSIndexPor->setText(QString().setNum(MSIndex,'d',3));
 ui->EditGSIndexPor->setText(QString().setNum(GSIndex,'d',3));
 ui->EditMaxDeriviant->setText(QString().setNum(MaxDeriviant,'d',6));

 ClsId=QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]);
 ClsBackgroId.clear();
 for(int i=0;i<Fones[ui->ComboTypeFone->currentIndex()].ClsIdsFone.size();i++)
  ClsBackgroId<<Fones[ui->ComboTypeFone->currentIndex()].ClsIdsFone[i];
}

void DlgMaskArea::on_ComboTypeFone_currentIndexChanged(int index)
{
 QList <DNBDSpectr::NameAndClsIdFones> Fones;

 if(index>=0)
 {
  Fones=BDSpec->ReadFones("Средства пассивной маскировки ВВСТ",ui->ListNameMask->currentItem()->text());

//  for(int i=0;i<Fones.size();i++)
//  {
//   QString str="";
//   for(int j=0;j<Fones[i].ClsIdsFone.size();j++)
//    str+=Fones[i].ClsIdsFone[j]+"    ";
//   QMessageBox msg;
//   msg.setText(Fones[i].NameFone+"   "+str);
//   msg.exec();
//  }



  QList <QString> MaskType;
  QString Condition="clsid="+QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]);
  Condition+=" AND classpropid=10";
  MaskType=BDSpec->ReadData("classvaltab","classval",Condition);
  float MSIndex;
  float GSIndex;

  if(MaskType[0]=="травяной")
  {
   this->TypeFone=2;
   QList <float> BrNir1;
   QList <float> BrRed;
   QList <float> NDVI;

   QList <float> NDVIre;
   QList <float> R750;
   QList <float> R705;

   R750=BDSpec->GetBrightLamda(750,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[index].ClsIdsFone);
   R705=BDSpec->GetBrightLamda(705,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[index].ClsIdsFone);

   for(int i=0;i<R750.size();i++)
    NDVIre<<(R750[i]-R705[i])/(R750[i]+R705[i]);

   GSIndex=0;
   for(int i=0;i<NDVIre.size();i++)
    GSIndex+=NDVIre[i]/NDVIre.size();

   BrNir1=BDSpec->GetBrightIntegral(LamNir1MS1,LamNir1MS2,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[index].ClsIdsFone);
   BrRed=BDSpec->GetBrightIntegral(LamRedMS1,LamRedMS2,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[index].ClsIdsFone);

//   if(index==1)
//   {
//    QMessageBox msg;
//    msg.setText(QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()])+"\n"+QString().setNum(Fones[index].ClsIdsFone));
//    msg.exec();
//   }

   for(int i=0;i<BrNir1.size();i++)
   {
    NDVI<<(BrNir1[i]-BrRed[i])/(BrNir1[i]+BrRed[i]);
   }

   MSIndex=0;
   for(int i=0;i<NDVI.size();i++)
    MSIndex+=NDVI[i]/NDVI.size();
  }//if(MaskType=="травяной")

  if(MaskType[0]=="песчаный")
  {
   this->TypeFone=0;
   QList <float> BrNir1;
   QList <float> BrRed;
   QList <float> BrGren;
   QList <float> TCHVI;

   QList <float> NDVIre;
   QList <float> R750;
   QList <float> R705;

   R750=BDSpec->GetBrightLamda(750,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[index].ClsIdsFone);
   R705=BDSpec->GetBrightLamda(705,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[index].ClsIdsFone);

   for(int i=0;i<R750.size();i++)
    NDVIre<<(R750[i]-R705[i])/(R750[i]+R705[i]);

   GSIndex=0;
   for(int i=0;i<NDVIre.size();i++)
    GSIndex+=NDVIre[i]/NDVIre.size();

   BrNir1=BDSpec->GetBrightIntegral(LamNir1MS1,LamNir1MS2,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[index].ClsIdsFone);
   BrRed=BDSpec->GetBrightIntegral(LamRedMS1,LamRedMS2,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[index].ClsIdsFone);
   BrGren=BDSpec->GetBrightIntegral(LamGrMS1,LamGrMS2,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[index].ClsIdsFone);

   for(int i=0;i<BrNir1.size();i++)
    TCHVI<<((BrRed[i]-BrGren[i])-(BrNir1[i]-BrRed[i]))/((BrRed[i]-BrGren[i])+(BrNir1[i]-BrRed[i]));

   MSIndex=0;
   for(int i=0;i<TCHVI.size();i++)
    MSIndex+=TCHVI[i]/TCHVI.size();
  }//if(MaskType=="песчаный")

  if(MaskType[0]=="снежный")
  {
   this->TypeFone=1;
   QList <float> BrNir1;
   QList <float> BrNir2;
   QList <float> SN;

   QList <float> SNgs;
   QList <float> R750;
   QList <float> R1000;

   R750=BDSpec->GetBrightLamda(750,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[index].ClsIdsFone);
   R1000=BDSpec->GetBrightLamda(1000,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[index].ClsIdsFone);

   for(int i=0;i<R750.size();i++)
    SNgs<<(R1000[i]-R750[i])/(R1000[i]+R750[i]);

   GSIndex=0;
   for(int i=0;i<SNgs.size();i++)
    GSIndex+=SNgs[i]/SNgs.size();

   BrNir1=BDSpec->GetBrightIntegral(LamNir1MS1,LamNir1MS2,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[index].ClsIdsFone);
   BrNir2=BDSpec->GetBrightIntegral(LamNir2MS1,LamNir2MS2,QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]),Fones[index].ClsIdsFone);

   for(int i=0;i<BrNir1.size();i++)
   {
    SN<<(BrNir2[i]-BrNir1[i])/(BrNir2[i]+BrNir1[i]);
   }

   MSIndex=0;
   for(int i=0;i<SN.size();i++)
    MSIndex+=SN[i]/SN.size();
  }//if(MaskType=="снежный")
  ui->EditMSIndexPor->setText(QString().setNum(MSIndex,'d',3));
  ui->EditGSIndexPor->setText(QString().setNum(GSIndex,'d',3));

  ClsId=QString().setNum(ListClsIdNameMask[ui->ListNameMask->currentRow()]);
  ClsBackgroId.clear();
  for(int i=0;i<Fones[index].ClsIdsFone.size();i++)
   ClsBackgroId<<Fones[index].ClsIdsFone[i];
 }//if(index>=0)
}
