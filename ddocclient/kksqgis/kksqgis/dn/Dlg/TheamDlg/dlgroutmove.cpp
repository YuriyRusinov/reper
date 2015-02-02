#include "dlgroutmove.h"
#include "ui_dlgroutmove.h"

DlgRoutMove::DlgRoutMove(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgRoutMove)
{
 ui->setupUi(this);

 //Подключение БД
 BDSpec=new DNBDSpectr("QPSQL","REPER","repermaster","localhost","master");
}

DlgRoutMove::~DlgRoutMove()
{
 delete ui;
}

void DlgRoutMove::showEvent(QShowEvent *)
{
 IsDlgShow=false;
 //Установка параметров по умолчанию
 QSettings Settings(this->NameOrg,this->NameProg);

 int KolvoKl=Settings.value("RoutMove/KolvoKl",10).toInt();
 int KolvoIt=Settings.value("RoutMove/KolvoIt",10).toInt();
 float MinDist=Settings.value("RoutMove/MinDist","150").toFloat();
 float MaxSKO=Settings.value("RoutMove/MaxSKO","0,5").toFloat();
 float LenPor=Settings.value("RoutMove/LengthPor","0,5").toFloat();
 float MultiCh=Settings.value("RoutMove/MultiCh","0,0001").toFloat();

 ui->EditKolvoKL->setText(QString().setNum(KolvoKl));
 ui->EditKolvoIt->setText(QString().setNum(KolvoIt));
 ui->EditMinDist->setText(QString().setNum(MinDist,'d',3));
 ui->EditMaxSKO->setText(QString().setNum(MaxSKO,'d',3));
 ui->EditLengthPor->setText(QString().setNum(LenPor,'d',3));
 ui->EditMultiCh->setText(QString().setNum(MultiCh,'d',5));

 ui->ComboFen->clear();
 ui->ComboLandScape->clear();
 ui->ComboFen->addItem("Первая половина лета");
 ui->ComboFen->addItem("Вторая половина лета");
 ui->ComboFen->addItem("Начало осени");
 ui->ComboFen->addItem("Основной период осени");
 ui->ComboFen->view()->setFixedWidth(150);
 ui->ComboFen->setCurrentIndex(0);


// QList <DNBDSpectr::NameAndNum> NameClsIdForests;
// NameClsIdForests=BDSpec->ReadNamesSpectrs("Леса");

// QList <int> NumLandScape;
// NumLandScape=BDSpec->ReadLandScape(NameClsIdForests);
// DlgRoutMove::NumAndNameLandScape LandScape;

// LandScapeList.clear();

// for(int i=0;i<NumLandScape.size();i++)
// {
//  LandScape.NameLandScape=BDSpec->GetNameLandScape(NumLandScape[i]);
//  LandScape.NumLandScape=NumLandScape[i];
//  ui->ComboLandScape->addItem(BDSpec->GetNameLandScape(NumLandScape[i]));
//  LandScapeList<<LandScape;
// }
// ui->ComboLandScape->view()->setFixedWidth(150);
// ui->ComboLandScape->setCurrentIndex(0);
 FillFormFromBD(0);
 IsDlgShow=true;
}

void DlgRoutMove::on_ButtonOK_clicked()
{
 this->NumCl=ui->EditKolvoKL->text().toInt();
 this->I=ui->EditKolvoIt->text().toInt();
 this->MaxSKO=ui->EditMaxSKO->text().replace(",",".").toFloat();
 this->Qc=ui->EditMinDist->text().replace(",",".").toFloat();
 this->NDVIPor=ui->EditNDVIPor->text().replace(",",".").toFloat();
 this->LengthPor=ui->EditLengthPor->text().replace(",",".").toFloat();
 this->MultiCh=ui->EditMultiCh->text().replace(",",".").toFloat();
 this->Fenolog=ui->ComboFen->currentIndex()+4;

  //Сохраняем параметры по умолчанию
 QSettings Settings(this->NameOrg,this->NameProg);
 Settings.setValue("RoutMove/KolvoKl",this->NumCl);
 Settings.setValue("RoutMove/KolvoIt",this->I);
 Settings.setValue("RoutMove/MinDist",this->Qc);
 Settings.setValue("RoutMove/MaxSKO",this->MaxSKO);
 Settings.setValue("RoutMove/LengthPor",this->LengthPor);
 Settings.setValue("RoutMove/MultiCh",this->MultiCh);

 emit ButtonOK();
}

void DlgRoutMove::FillFormFromBD(int NumLandScape)
{
 IsDlgShow=false;
 int Fenol=ui->ComboFen->currentIndex();
 Fenol+=4;

 QList <DNBDSpectr::NameAndNum> NameClsIdForests;
 NameClsIdForests=BDSpec->ReadNamesSpectrs("Леса"/*,LandScapeList[NumLandScape].NumLandScape*/);

 QList <int> NameLandScape;
 NameLandScape=BDSpec->ReadLandScape(NameClsIdForests,Fenol);
 DlgRoutMove::NumAndNameLandScape LandScape;

 LandScapeList.clear();
 ui->ComboLandScape->clear();
 for(int i=0;i<NameLandScape.size();i++)
 {
   LandScape.NameLandScape=BDSpec->GetNameLandScape(NameLandScape[i]);
   LandScape.NumLandScape=NameLandScape[i];
   ui->ComboLandScape->addItem(BDSpec->GetNameLandScape(NameLandScape[i]));

//     QMessageBox msg;
//     msg.setText(LandScape.NameLandScape+"\n"+QString().setNum(LandScape.NumLandScape));
//     msg.exec();

   LandScapeList<<LandScape;
  }

  ui->ComboLandScape->view()->setFixedWidth(150);

  if(ui->ComboLandScape->count()>NumLandScape)
   ui->ComboLandScape->setCurrentIndex(NumLandScape);

   NameClsIdForests.clear();
   NameClsIdForests=BDSpec->ReadNamesSpectrs("Леса",Fenol,LandScapeList[NumLandScape].NumLandScape);


// BDSpec->ReadLandScape(NameClsIdForests);

 QList <QString> ClsBackGroId;
 ClsBackGroId.clear();
 ClsBackGroId<<"0";
 QList <float> Red;
 QList <float> Nir;
 float NDVI;
 QString NDVIClsId;
 NDVIPor=10;
 for(int i=0;i<NameClsIdForests.size();i++)
 {
  Red.clear();
  Nir.clear();
  Red=BDSpec->GetBrightIntegral(640,680,NameClsIdForests[i].Num,ClsBackGroId,Fenol,LandScapeList[NumLandScape].NumLandScape);
  Nir=BDSpec->GetBrightIntegral(720,800,NameClsIdForests[i].Num,ClsBackGroId,Fenol,LandScapeList[NumLandScape].NumLandScape);
  for(int j=0;j<Nir.size();j++)
  {
   if(Nir[j]>0 && Red[j]>0)
   {
    NDVI=(Nir[j]-Red[j])/(Nir[j]+Red[j]);
    if(NDVIPor>NDVI && NDVI>0.3)
    {
     NDVIPor=NDVI;
     NDVIClsId=NameClsIdForests[i].Num;
    }//if(NDVIPor>NDVI)
//    QMessageBox msg;
//    msg.setText("Fenol="+QString().setNum(Fenol)+"\nClsId="+NameClsIdForests[i].Num+"\nNDVI="+QString().setNum(NDVI[NDVI.size()-1])+"\nNir="+QString().setNum(Nir[j])+"\nRed="+QString().setNum(Red[j]));
//    msg.exec();
   }//if(Nir[j]>0 && Red[j]>0)
  }
 }//for(int i=0;i<NameClsIdForests.size();i++)
 ui->EditNDVIPor->setText(QString().setNum(this->NDVIPor,'d',3));
 IsDlgShow=true;
}

void DlgRoutMove::on_ComboFen_currentIndexChanged(int index)
{
 if(IsDlgShow)
  FillFormFromBD(0);
}

void DlgRoutMove::on_ComboLandScape_currentIndexChanged(int index)
{
 if(IsDlgShow)
  FillFormFromBD(index);
}
