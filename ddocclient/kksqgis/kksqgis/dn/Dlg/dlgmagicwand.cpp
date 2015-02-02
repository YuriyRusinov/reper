#include "dlgmagicwand.h"
#include "ui_dlgmagicwand.h"

DlgMagicWand::DlgMagicWand(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMagicWand)
{
    ui->setupUi(this);
}

DlgMagicWand::~DlgMagicWand()
{
    delete ui;
}

void DlgMagicWand::MouseClicked(int x,int y)
{
 if(!this->isHidden())
 {
  ui->EditMouseX->setText(QString().setNum(x));
  ui->EditMouseY->setText(QString().setNum(y));
 }//if(!this->isHidden())
}

void DlgMagicWand::on_ButtonOK_clicked()
{
 this->x=ui->EditMouseX->text().toInt();
 this->y=ui->EditMouseY->text().toInt();
 this->Contrast=ui->EditPorog->text().replace(",",".").toFloat();
 this->NumCh=ui->ComboCh->currentIndex();

 this->mode=0;
 if(ui->RadioContr->isChecked())
  this->mode=0;
 if(ui->RadioBrigth->isChecked())
  this->mode=1;
 if(ui->RadioContr2->isChecked())
  this->mode=2;

 //Сохраняем параметры по умолчанию
 QSettings Settings(this->NameOrg,this->NameProg);
 Settings.setValue("MagicWand/CurCh",this->NumCh);
 Settings.setValue("MagicWand/CurMode",this->mode);
 Settings.setValue("MagicWand/CurPorog",ui->EditPorog->text());

 emit ButtonOK();
// this->close();
//  this->ThisPoly->MagicWand(this->ThisPoly->CurrentPoly,x,y,Contrast,NumCh,mode,this->NoBlackPixels2);
}

void DlgMagicWand::showEvent(QShowEvent *)
{
 QSettings Settings(this->NameOrg,this->NameProg);

 int CurCh=Settings.value("MagicWand/CurCh",0).toInt();
 int CurMode=Settings.value("MagicWand/CurMode",0).toInt();
 QString CurPorog=Settings.value("MagicWand/CurPorog","0,5").toString();

 ui->ComboCh->clear();
 if(this->ThisPoly!=NULL)
 {
  for(int i=0;i<this->ThisPoly->Ch;i++)
  {
   ui->ComboCh->addItem(tr("Канал ")+QString().setNum(i+1));
  }//for(int i=0;i<this->ThisPoly->Ch;i++)
 }//if(this->ThisPoly!=NULL)

 //Устанавливаем изначальные значения
 ui->ComboCh->setCurrentIndex(CurCh);
 if(CurMode==0)
  ui->RadioContr->setChecked(TRUE);
 if(CurMode==1)
  ui->RadioBrigth->setChecked(TRUE);
 if(CurMode==2)
  ui->RadioContr2->setChecked(TRUE);
 ui->EditPorog->setText(CurPorog);
}
