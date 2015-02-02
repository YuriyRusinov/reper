#include "dlgembedobj.h"
#include "ui_dlgembedobj.h"

DlgEmbedObj::DlgEmbedObj(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgEmbedObj)
{
    ui->setupUi(this);
    //Подключение БД
    BDSpec=new DNBDSpectr("QPSQL","REPER","repermaster","localhost","master");
}

DlgEmbedObj::~DlgEmbedObj()
{
    delete ui;
}

void DlgEmbedObj::showEvent(QShowEvent *)
{
 QSettings Settings(this->NameOrg,this->NameProg);

 DeltaMS=Settings.value("EmbedObj/DeltaMS",0).toFloat();
 DeltaGS=Settings.value("EmbedObj/DeltaGS",0).toFloat();
// MSIndex=Settings.value("EmbedObj/MSIndex",0).toFloat();
// GSIndex=Settings.value("EmbedObj/GSIndex",0).toFloat();
 Contrast630_690=Settings.value("EmbedObj/Contrast630_690",0).toFloat();
 Contrast800_1000=Settings.value("EmbedObj/Contrast800_1000",0).toFloat();
// WBI=Settings.value("EmbedObj/WBI",0).toFloat();
 AKFMin=Settings.value("EmbedObj/AKFMin",0).toFloat();
 KofCorelPor=Settings.value("EmbedObj/KofCorelPor",0).toFloat();

 ui->ListNameEmbedObj->clear();
 TypeEmbedObj=BDSpec->ReadNamesSpectrs("скрытые и заглубленные объекты");

 for(int i=0;i<TypeEmbedObj.size();i++)
  ui->ListNameEmbedObj->addItem(TypeEmbedObj[i].Name);
 ui->ListNameEmbedObj->setCurrentRow(0);
 FillForm();
}

void DlgEmbedObj::on_ButtonOK_clicked()
{
 MSIndex=ui->EditMSIndex->text().toFloat();
 GSIndex=ui->EditGSIndex->text().toFloat();
 WBI=ui->EditWBIIndex->text().toFloat();
 DeltaGS=ui->EditGSIndexDelta->text().toFloat();
 DeltaMS=ui->EditMSIndexDelta->text().toFloat();
 AKFMin=ui->EditAKF->text().toFloat();
 KofCorelPor=ui->EditKofCor->text().toFloat();
 Contrast630_690=ui->EditCntrast630_690->text().toFloat();
 Contrast800_1000=ui->EditCntrast800_1000->text().toFloat();

 //Сохраняем параметры по умолчанию
 QSettings Settings(this->NameOrg,this->NameProg);
 Settings.setValue("EmbedObj/DeltaMS",DeltaMS);
 Settings.setValue("EmbedObj/DeltaGS",DeltaGS);
 Settings.setValue("EmbedObj/Contrast630_690",Contrast630_690);
 Settings.setValue("EmbedObj/Contrast800_1000",Contrast800_1000);
 Settings.setValue("EmbedObj/AKFMin",AKFMin);
 Settings.setValue("EmbedObj/KofCorelPor",KofCorelPor);
 emit ButtonOK();
}

void DlgEmbedObj::on_ListNameEmbedObj_currentRowChanged(int currentRow)
{
 FillForm();
}

void DlgEmbedObj::FillForm()
{
 int CurType=ui->ListNameEmbedObj->currentRow();

 QList <float> Nir;
 QList <float> Red;
 QList <QString> BackgroId;
 BackgroId<<"0";

 Nir=BDSpec->GetBrightIntegral(LamMSIndex1[0],LamMSIndex1[1],TypeEmbedObj[CurType].Num,BackgroId);
 Red=BDSpec->GetBrightIntegral(LamMSIndex2[0],LamMSIndex2[1],TypeEmbedObj[CurType].Num,BackgroId);

 int KolvoIzm=0;
 MSIndex=0;
 for(int j=0;j<Nir.size();j++)
 {
  if(Nir[j]>0 && Red[j]>0)
  {
   MSIndex+=(Nir[j]-Red[j])/(Nir[j]+Red[j]);
   KolvoIzm++;
  }//if(Nir[j]>0 && Red[j]>0)
 }
 MSIndex=MSIndex/KolvoIzm;

 QList <float> Br1,Br2;
 Br1=BDSpec->GetBrightIntegral(LamContr1[0],LamContr1[1],TypeEmbedObj[CurType].Num,BackgroId);
 Br2=BDSpec->GetBrightIntegral(LamContr2[0],LamContr2[1],TypeEmbedObj[CurType].Num,BackgroId);

 KolvoIzm=0;
 Brigth630_690=0;
 Brigth800_1000=0;
 for(int j=0;j<Br1.size();j++)
 {
  if(Br1[j]>0 && Br2[j]>0)
  {
   Brigth630_690+=Br1[j];
   Brigth800_1000+=Br2[j];
   KolvoIzm++;
  }//if(Nir[j]>0 && Red[j]>0)
 }
 Brigth630_690=Brigth630_690/KolvoIzm;
 Brigth800_1000=Brigth800_1000/KolvoIzm;

 QList <float> BrLam750;
 QList <float> BrLam705;

 BrLam750=BDSpec->GetBrightLamda(LamGSIndex[0],TypeEmbedObj[CurType].Num,BackgroId);
 BrLam705=BDSpec->GetBrightLamda(LamGSIndex[1],TypeEmbedObj[CurType].Num,BackgroId);

 KolvoIzm=0;
 GSIndex=0;
 for(int j=0;j<BrLam750.size();j++)
 {
  if(BrLam750[j]>0 && BrLam705[j]>0)
  {
   GSIndex+=(BrLam750[j]-BrLam705[j])/(BrLam750[j]+BrLam705[j]);
   KolvoIzm++;
  }//if(BrLam750[j]>0 && BrLam705[j]>0)
 }
 GSIndex=GSIndex/KolvoIzm;

 QList <float> B900;
 QList <float> B970;

 B900=BDSpec->GetBrightLamda(900,TypeEmbedObj[CurType].Num,BackgroId);
 B970=BDSpec->GetBrightLamda(970,TypeEmbedObj[CurType].Num,BackgroId);

 KolvoIzm=0;
 WBI=0;
 for(int j=0;j<B900.size();j++)
 {
  if(B900[j]>0 && B970[j]>0)
  {
   WBI+=B900[j]/B970[j];
   KolvoIzm++;
  }//if(BrLam750[j]>0 && BrLam705[j]>0)
 }
 WBI=WBI/KolvoIzm;

 ui->EditMSIndex->setText(QString().setNum(MSIndex,'d',3));
 ui->EditGSIndex->setText(QString().setNum(GSIndex,'d',3));
 ui->EditWBIIndex->setText(QString().setNum(WBI,'d',3));
 ui->EditGSIndexDelta->setText(QString().setNum(DeltaGS,'d',3));
 ui->EditMSIndexDelta->setText(QString().setNum(DeltaMS,'d',3));
 ui->EditAKF->setText(QString().setNum(AKFMin,'d',3));
 ui->EditKofCor->setText(QString().setNum(KofCorelPor,'d',3));
 ui->EditCntrast630_690->setText(QString().setNum(Contrast630_690,'d',3));
 ui->EditCntrast800_1000->setText(QString().setNum(Contrast800_1000,'d',3));
}

