#include "dlgsmoces.h"
#include "ui_dlgsmoces.h"

DlgSmoces::DlgSmoces(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSmoces)
{
 ui->setupUi(this);
 //Подключение БД
 BDSpec=new DNBDSpectr("QPSQL","REPER","repermaster","localhost","master");
}

DlgSmoces::~DlgSmoces()
{
 delete ui;
}

void DlgSmoces::showEvent(QShowEvent *)
{
 //Установка параметров по умолчанию
 QSettings Settings(this->NameOrg,this->NameProg);
 KofCorpor=Settings.value("DlgSmoces/KofCorPor",0.5).toFloat();
 ui->EditKofCorrPor->setText(QString().setNum(KofCorpor,'d',3));

 //Чтение данных из БД
 QList <DNBDSpectr::NameAndNum> NameClsIdSmoces;
 NameClsIdSmoces=BDSpec->ReadNamesSpectrs("Территории, маскируемые аэрозольно-дымовыми завесами");
 QList <float> R600;
 QList <QString> ClsBakgroId;
 float R600Min=10;

 ClsBakgroId<<"0";
 for(int i=0;i<NameClsIdSmoces.size();i++)
 {
  R600=BDSpec->GetBrightIntegral(LamBegin,LamEnd,NameClsIdSmoces[i].Num,ClsBakgroId);

//  QMessageBox msg;
//  msg.setText(QString().setNum(R600[0],'d',3));
//  msg.exec();

  for(int j=0;j<R600.size();j++)
  {
   if(R600[j]<R600Min && R600[j]!=-1)
    R600Min=R600[j];
  }
 }//for(int i=0;i<NameClsIdForests.size();i++)
 ui->EditR600Por->setText(QString().setNum(R600Min,'d',3));
}

void DlgSmoces::on_ButtonOK_clicked()
{
 KofCorpor=ui->EditKofCorrPor->text().toFloat();
 R600por=ui->EditR600Por->text().toFloat();


  //Сохраняем параметры по умолчанию
 QSettings Settings(this->NameOrg,this->NameProg);
 Settings.setValue("DlgSmoces/KofCorPor",KofCorpor);
 emit ButtonOK();
}
