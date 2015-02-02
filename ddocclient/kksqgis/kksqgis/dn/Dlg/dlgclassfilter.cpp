#include "dlgclassfilter.h"
#include "ui_dlgclassfilter.h"

DlgClassFilter::DlgClassFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgClassFilter)
{
    ui->setupUi(this);
    NewClassif=NULL;
    this->ThisPoly=NULL;
}

DlgClassFilter::~DlgClassFilter()
{
 if(this->NewClassif!=NULL)
  delete[] this->NewClassif;

 delete ui;
}

void DlgClassFilter::showEvent(QShowEvent *)
{
 if(this->ThisPoly!=NULL)
 {
  if(this->NewClassif!=NULL)
   delete[] this->NewClassif;

  this->NewClassif=new int[this->ThisPoly->W*this->ThisPoly->H];
  ClassPix.clear();
  ui->ListClassif->clear();

  for(quint64 i=0;i<this->ThisPoly->W*this->ThisPoly->H;i++)
  {
   ClassPix[this->ThisPoly->ClassifMass[i]]++;
   this->NewClassif[i]=this->ThisPoly->ClassifMass[i];
  }//for(quint64 i=0;i<this->ThisPoly->W*this->ThisPoly->H;i++)
  QMap<int,quint64>::iterator it=ClassPix.begin();
  for(;it!=ClassPix.end();it++)
  {
   if(it.key()>=0)
   {
    QListWidgetItem *item = new QListWidgetItem(ui->ListClassif);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
    item->setData(Qt::DisplayRole, "Класс "+QString().setNum(it.key())+" ("+QString().setNum(it.value())+" пикселей)");
    item->setData(Qt::CheckStateRole,Qt::Checked);
   }//if(it.key()>=0)
  }//for(;it!=ClassPix.end();it++)
 }//if(this->ThisPoly!=NULL)
}

void DlgClassFilter::on_ListClassif_itemClicked(QListWidgetItem *item)
{
 QString NumClass;
 int NumCl;
 bool *MaskPoly;

 MaskPoly=new bool[this->ClassPix.size()-1];
 NumClass=item->text();
 NumClass.remove("Класс ");
 NumClass.remove(NumClass.indexOf(' '),NumClass.length()-NumClass.indexOf(' '));

 NumCl=NumClass.toInt();
 int Kl=this->ClassPix.size();
 for(int i=0;i<this->ClassPix.size()-1;i++)
 {
  if(ui->ListClassif->item(i)->checkState()==Qt::Checked)
   MaskPoly[i]=TRUE;
  else
   MaskPoly[i]=FALSE;
 }//for(int i=0;i<this->ClassPix.size();i++)

 for(quint64 i=0;i<this->ThisPoly->W*this->ThisPoly->H;i++)
 {
  if(this->ThisPoly->ClassifMass[i]!=-1 && !MaskPoly[this->NewClassif[i]])
   this->ThisPoly->ClassifMass[i]=-1;

  if(this->NewClassif[i]!=-1 && this->ThisPoly->ClassifMass[i]==-1 && MaskPoly[this->NewClassif[i]])
   this->ThisPoly->ClassifMass[i]=this->NewClassif[i];
 }//for(quint64 i=0;i<this->ThisPoly->W*this->ThisPoly->H;i++)
 this->WidgetChange->update();
 delete[] MaskPoly;
}

void DlgClassFilter::on_ButtonOK_clicked()
{
 for(quint64 i=0;i<this->ThisPoly->W*this->ThisPoly->H;i++)
 {
  if(this->ThisPoly->ClassifMass[i]>=0)
   this->ThisPoly->ClassifMass[i]=0;
 }//for(quint64 i=0;i<this->ThisPoly->W*this->ThisPoly->H;i++)
 this->ThisPoly->IsPolyClassif=TRUE;
 emit ButtonOK();
 delete[] this->NewClassif;
 this->close();
}
