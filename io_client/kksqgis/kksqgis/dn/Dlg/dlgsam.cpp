#include "dlgsam.h"
#include "ui_dlgsam.h"

DlgSAM::DlgSAM(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSAM)
{
    ui->setupUi(this);
    this->ThisPoly=NULL;
    this->MaskCh=NULL;
}

DlgSAM::~DlgSAM()
{
    delete ui;
}

void DlgSAM::showEvent(QShowEvent *)
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

 ui->ListCh->clear();
 ui->ListEtalon->clear();
 if(this->ThisPoly!=NULL)
 {
  for(int i=0;i<this->ThisPoly->Ch;i++)
  {
   QListWidgetItem *item = new QListWidgetItem(ui->ListCh);
   item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
   item->setData(Qt::DisplayRole, tr("Канал ")+QString().setNum(i+1));
   item->setData(Qt::CheckStateRole,Qt::Unchecked);
   if(CheckChan!=NULL && SizeFile>i && CheckChan[i]==1)
   {
    ui->ListCh->item(i)->setCheckState(Qt::Checked);
   }//if(CheckChan!=NULL && SizeFile<i && CheckChan[i]==1)
  }//for(int i=0;i<this->ThisPoly->Ch;i++)
 }//if(this->ThisPoly!=NULL)
 if(CheckChan!=NULL)
  delete[] CheckChan;
}

void DlgSAM::on_ButtonAdd_clicked()
{
 QStringList FileNamesList;
 FileNamesList=QFileDialog::getOpenFileNames(this,"Открыть полигоны","","*.pol");

 for(int i=0;i<FileNamesList.size();i++)
 {
  ui->ListEtalon->addItem(FileNamesList[i]);
 }//for(int i=0;i<FileNamesList.size();i++)
}

void DlgSAM::on_ButtonDell_clicked()
{
 delete ui->ListEtalon->currentItem();
}

void DlgSAM::on_ButtonOK_clicked()
{
 PolyFilesNames.clear();
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
   if(ui->ListCh->item(i)->checkState()==Qt::Checked)
   {
    this->MaskCh[i]=TRUE;
    ZnachCh=1;
   }//if(ui->ListChan->item(i)->checkState()==Qt::Checked)
   else
   {
    this->MaskCh[i]=FALSE;
    ZnachCh=-1;
   }
   FileChan.write((char*)&ZnachCh,sizeof(int));
  }//for(int i=0;i<this->ThisPoly->Ch;i++)
  FileChan.close();
  //int den=ui->ListEtalon->size();

  for(int i=0;i<ui->ListEtalon->count();i++)
  {
   PolyFilesNames<<ui->ListEtalon->item(i)->text();
  }

  this->IsSKOEnable=TRUE;
  if(ui->RadioSKODisable->isChecked())
   this->IsSKOEnable=FALSE;

  this->LimitSKO=-1.;
  this->LimitSKO=ui->EditSKOLimit->text().replace(',','.').toFloat();

  emit ButtonOK();
 }//if(this->ThisPoly!=NULL)
}
