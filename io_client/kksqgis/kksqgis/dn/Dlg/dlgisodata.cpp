 #include "dlgisodata.h"
#include "ui_dlgisodata.h"

DlgIsoData::DlgIsoData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgIsoData)
{
    ui->setupUi(this);
    this->ThisPoly=NULL;
    this->MaskCh=NULL;
    Lamda=NULL;
}

DlgIsoData::~DlgIsoData()
{
    delete ui;
}
void DlgIsoData::showEvent(QShowEvent *)
{
 QFile FileChan(this->PathData+'/'+this->FileNameChan);
 quint32 SizeFile;
 int *CheckChan;
 CheckChan=NULL;
 if(FileChan.exists())
 {
  FileChan.open(QIODevice::ReadOnly);
  SizeFile=FileChan.size()/sizeof(int);
  CheckChan=new int[SizeFile];
  FileChan.read((char*)CheckChan,SizeFile*sizeof(int));
  FileChan.close();
 }//if(FileChan.exists())


 QSettings Settings(this->NameOrg,this->NameProg);

 int KolvoKl=Settings.value("IsoData/KolvoKl",10).toInt();
 int KolvoIt=Settings.value("IsoData/KolvoIt",10).toInt();
 QString MinDist=Settings.value("IsoData/MinDist","150").toString();
 QString MaxSKO=Settings.value("IsoData/MaxSKO","0,5").toString();

 ui->EditKolvoKL->setText(QString().setNum(KolvoKl));
 ui->EditKolvoIt->setText(QString().setNum(KolvoIt));
 ui->EditMinDist->setText(MinDist);
 ui->EditMaxSKO->setText(MaxSKO);

 ui->ListChan->clear();
 if(this->ThisPoly!=NULL)
 {
  for(int i=0;i<this->ThisPoly->Ch;i++)
  {
   QListWidgetItem *item = new QListWidgetItem(ui->ListChan);
   item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
   if(i<ThisPoly->IshCh && IsChanSpecColibr)
    item->setData(Qt::DisplayRole, tr("Канал ")+QString().setNum(i+1)+" ("+QString().setNum(Lamda[i],'d',1)+")");
   else
    item->setData(Qt::DisplayRole, tr("Канал ")+QString().setNum(i+1));
   item->setData(Qt::CheckStateRole,Qt::Unchecked);
   if(CheckChan!=NULL && SizeFile>i && CheckChan[i]==1)
   {
    ui->ListChan->item(i)->setCheckState(Qt::Checked);
   }//if(CheckChan!=NULL && SizeFile<i && CheckChan[i]==1)
  }//for(int i=0;i<this->ThisPoly->Ch;i++)
 }//if(this->ThisPoly!=NULL)
 if(CheckChan!=NULL)
  delete[] CheckChan;
}

void DlgIsoData::on_ButtonOK_clicked()
{
 this->KolvoKl=ui->EditKolvoKL->text().toInt();
 this->KolvoIt=ui->EditKolvoIt->text().toInt();
 this->MaxSKO=ui->EditMaxSKO->text().replace(",",".").toFloat();
 this->MinDist=ui->EditMinDist->text().replace(",",".").toFloat();

 if(this->ThisPoly!=NULL)
 {
  if(this->MaskCh!=NULL)
   delete[] this->MaskCh;

  this->MaskCh=new bool[this->ThisPoly->Ch];

  QFile FileChan(this->PathData+'/'+this->FileNameChan);
  int ZnachCh;
  FileChan.open(QIODevice::Truncate|QIODevice::WriteOnly);
  for(int i=0;i<this->ThisPoly->Ch;i++)
  {
   if(ui->ListChan->item(i)->checkState()==Qt::Checked)
   {
    this->MaskCh[i]=TRUE;
    ZnachCh=1;
   }
   else
   {
    this->MaskCh[i]=FALSE;
    ZnachCh=-1;
   }
   FileChan.write((char*)&ZnachCh,sizeof(int));
  }
  FileChan.close();

  //Сохраняем параметры по умолчанию
  QSettings Settings(this->NameOrg,this->NameProg);
  Settings.setValue("IsoData/KolvoKl",this->KolvoKl);
  Settings.setValue("IsoData/KolvoIt",this->KolvoIt);
  Settings.setValue("IsoData/MinDist",ui->EditMinDist->text());
  Settings.setValue("IsoData/MaxSKO",ui->EditMaxSKO->text());

  emit ButtonOK();
 }//if(this->ThisPoly!=NULL)
}
