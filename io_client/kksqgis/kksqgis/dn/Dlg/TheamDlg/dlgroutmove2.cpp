#include "dlgroutmove2.h"
#include "ui_dlgroutmove2.h"

DlgRoutMove2::DlgRoutMove2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgRoutMove2)
{
    ui->setupUi(this);
    //Подключение БД
    BDSpec=new DNBDSpectr("QPSQL","REPER","repermaster","localhost","master");
}

DlgRoutMove2::~DlgRoutMove2()
{
    delete ui;
}

void DlgRoutMove2::showEvent(QShowEvent *)
{
 //Установка параметров по умолчанию
 QSettings Settings(this->NameOrg,this->NameProg);

 DeltaNDVI1=Settings.value("RoutMove2/DeltaNDVI1","0.05").toFloat();
 DeltaNDVI2=Settings.value("RoutMove2/DeltaNDVI2","0.05").toFloat();
 DeltaNDVI3=Settings.value("RoutMove2/DeltaNDVI3","0.01").toFloat();
 PorWBI=Settings.value("RoutMove2/PorWBI","0.01").toFloat();

 ui->EditDeltaNDVI1->setText(QString().setNum(DeltaNDVI1,'d',4));
 ui->EditDeltaNDVI2->setText(QString().setNum(DeltaNDVI2,'d',4));
 ui->EditDeltaNDVI3->setText(QString().setNum(DeltaNDVI3,'d',4));
 ui->EditPorWBI->setText(QString().setNum(PorWBI,'d',4));

 ui->ComboFen->clear();
 ui->ComboFen->addItem("Первая половина лета");
 ui->ComboFen->addItem("Вторая половина лета");
 ui->ComboFen->addItem("Начало осени");
 ui->ComboFen->addItem("Основной период осени");
 //   ui->ComboFen->view()->setFixedWidth(150);
 ui->ComboFen->setCurrentIndex(0);

 FillFormFromBD();
}

void DlgRoutMove2::FillFormFromBD()
{
 int Fenol=ui->ComboFen->currentIndex()+4;
 QList <DNBDSpectr::NameAndNum> NameClsIdSwamp;
 NameClsIdSwamp=BDSpec->ReadNamesSpectrs("Болота");

 QList <QString> ClsBackGroId;
 ClsBackGroId.clear();
 ClsBackGroId<<"0";

 QList <float> N750_860;
 QList <float> N630_690;
 QList <float> N690_720;
 QList <float> N800_900;
 QList <float> N720_800;

 QList <float> N900;
 QList <float> N970;

 float NDVI1,NDVI2,NDVI3,WBI;
 float NDVI1Midl=0.45,NDVI2Midl=0.25,NDVI3Midl=0.15,WBIMin=1.1;

 for(int i=0;i<NameClsIdSwamp.size();i++)
 {
  N750_860.clear();
  N630_690.clear();

  N690_720.clear();
  N800_900.clear();
  N720_800.clear();

  N630_690=BDSpec->GetBrightIntegral(Lam2NDVI1[0],Lam2NDVI1[1],NameClsIdSwamp[i].Num,ClsBackGroId,Fenol);
  N750_860=BDSpec->GetBrightIntegral(Lam1NDVI1[0],Lam1NDVI1[1],NameClsIdSwamp[i].Num,ClsBackGroId,Fenol);
  N690_720=BDSpec->GetBrightIntegral(Lam2NDVI2[0],Lam2NDVI2[1],NameClsIdSwamp[i].Num,ClsBackGroId,Fenol);
  N800_900=BDSpec->GetBrightIntegral(Lam1NDVI3[0],Lam1NDVI3[1],NameClsIdSwamp[i].Num,ClsBackGroId,Fenol);
  N720_800=BDSpec->GetBrightIntegral(Lam2NDVI3[0],Lam2NDVI3[1],NameClsIdSwamp[i].Num,ClsBackGroId,Fenol);

  N900=BDSpec->GetBrightLamda(Lam1WBI,NameClsIdSwamp[i].Num,ClsBackGroId,Fenol);
  N970=BDSpec->GetBrightLamda(Lam2WBI,NameClsIdSwamp[i].Num,ClsBackGroId,Fenol);

  NDVI1=1000;
  NDVI2=1000;
  NDVI3=1000;
  WBI=1000;

  NDVI1Midl=0;
  NDVI2Midl=0;
  NDVI3Midl=0;
  WBIMin=1000;
  bool isWBIValid=false;
  int KolvoIndex=0;
  for(int j=0;j<N750_860.size();j++)
  {

   if(N750_860[j]>0 && N630_690[j]>0 && N690_720[j]>0 && N800_900[j]>0 && N720_800[j]>0 && N800_900[j]>0 && N720_800[j]>0)
   {
    NDVI1=(N750_860[j]-N630_690[j])/(N750_860[j]+N630_690[j]);
    NDVI2=(N750_860[j]-N690_720[j])/(N750_860[j]+N690_720[j]);
    NDVI3=(N800_900[j]-N720_800[j])/(N800_900[j]+N720_800[j]);
    WBI=N900[j]/N970[j];

    NDVI1Midl+=NDVI1;
    NDVI2Midl+=NDVI2;
    NDVI3Midl+=NDVI3;

    if(!isWBIValid)
    {
     WBIMin=WBI;
     isWBIValid=true;
    }
    if(isWBIValid && WBIMin>WBI)
     WBIMin=WBI;
    KolvoIndex++;
   }
  }//for(int j=0;j<N750_860.size();j++)

  if(KolvoIndex>0)
  {
   NDVI1Midl=NDVI1Midl/KolvoIndex;
   NDVI2Midl=NDVI2Midl/KolvoIndex;
   NDVI3Midl=NDVI3Midl/KolvoIndex;
  }

  else
  {
   NDVI1Midl=0.45;
   NDVI2Midl=0.25;
   NDVI3Midl=0.15;
   WBIMin=1.1;
  }

 }//for(int i=0;i<NameClsIdForests.size();i++)
 ui->EditPorNDVI1->setText(QString().setNum(NDVI1Midl,'d',3));
 ui->EditPorNDVI2->setText(QString().setNum(NDVI2Midl,'d',3));
 ui->EditPorNDVI3->setText(QString().setNum(NDVI3Midl,'d',3));
 ui->EditPorWBI->setText(QString().setNum(WBIMin,'d',3));


}

void DlgRoutMove2::on_ButtonOK_clicked()
{
 PorNDVI1=ui->EditPorNDVI1->text().toFloat();
 PorNDVI2=ui->EditPorNDVI2->text().toFloat();
 PorNDVI3=ui->EditPorNDVI3->text().toFloat();
 DeltaNDVI1=ui->EditDeltaNDVI1->text().toFloat();
 DeltaNDVI2=ui->EditDeltaNDVI2->text().toFloat();
 DeltaNDVI3=ui->EditDeltaNDVI3->text().toFloat();
 PorWBI=ui->EditPorWBI->text().toFloat();
 emit ButtonOK();
}

void DlgRoutMove2::on_ComboFen_currentIndexChanged(int index)
{
 if(ui->ComboFen->count()==4 && !this->isHidden())
  FillFormFromBD();
}
