#include "dlgcreateimgpoly.h"
#include "ui_dlgcreateimgpoly.h"

DlgCreateImgPoly::DlgCreateImgPoly(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCreateImgPoly)
{
    ui->setupUi(this);
}

DlgCreateImgPoly::~DlgCreateImgPoly()
{
    delete ui;
}

void DlgCreateImgPoly::showEvent(QShowEvent *)
{
 ui->ComboRCh->clear();
 ui->ComboGCh->clear();
 ui->ComboBCh->clear();
 for(int i=0;i<ThisPoly->Ch;i++)
 {
  ui->ComboRCh->addItem("Канал "+QString().setNum(i+1));
  ui->ComboGCh->addItem("Канал "+QString().setNum(i+1));
  ui->ComboBCh->addItem("Канал "+QString().setNum(i+1));
 }
 ui->EditRKof->setText("1");
 ui->EditGKof->setText("1");
 ui->EditBKof->setText("1");
}

void DlgCreateImgPoly::on_ChangeNumChan()
{
 ui->ComboRCh->clear();
 ui->ComboGCh->clear();
 ui->ComboBCh->clear();
 for(int i=0;i<ThisPoly->Ch;i++)
 {
  ui->ComboRCh->addItem("Канал "+QString().setNum(i+1));
  ui->ComboGCh->addItem("Канал "+QString().setNum(i+1));
  ui->ComboBCh->addItem("Канал "+QString().setNum(i+1));
 }
}

void DlgCreateImgPoly::on_ButtonOK_clicked()
{
 NumChR=ui->ComboRCh->currentIndex();
 NumChG=ui->ComboGCh->currentIndex();
 NumChB=ui->ComboBCh->currentIndex();

 ui->EditRKof->text().replace(',','.');
 ui->EditGKof->text().replace(',','.');
 ui->EditBKof->text().replace(',','.');

 KofR=ui->EditRKof->text().toFloat();
 KofG=ui->EditGKof->text().toFloat();
 KofB=ui->EditBKof->text().toFloat();

 emit ButtonOK();
}
