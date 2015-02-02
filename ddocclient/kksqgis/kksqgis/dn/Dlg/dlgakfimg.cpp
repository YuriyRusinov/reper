#include "dlgakfimg.h"
#include "ui_dlgakfimg.h"

DlgAKFImg::DlgAKFImg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAKFImg)
{
    ui->setupUi(this);
}

DlgAKFImg::~DlgAKFImg()
{
    delete ui;
}

void DlgAKFImg::FillMouseCoord(int x, int y)
{
 ui->EditX->setText(QString().setNum(x));
 ui->EditY->setText(QString().setNum(y));
}

void DlgAKFImg::showEvent(QShowEvent *)
{
 ui->ComboNumCh->clear();
 ui->ComboSideSq->clear();
 for(int i=0;i<ThisPoly->Ch;i++)
  ui->ComboNumCh->addItem("Канал "+QString().setNum(i+1));

 for(int i=3;i<=25;i+=2)
  ui->ComboSideSq->addItem(QString().setNum(i));
}

void DlgAKFImg::on_ComboSideSq_currentIndexChanged(const QString &arg1)
{
 XMouse=ui->EditX->text().toInt();
 YMouse=ui->EditY->text().toInt();
 emit ComboSideSqClick(arg1.toInt());
}

void DlgAKFImg::on_ButtonOK_clicked()
{
 XMouse=ui->EditX->text().toInt();
 YMouse=ui->EditY->text().toInt();
 NumCh=ui->ComboNumCh->currentIndex();
 Side=ui->ComboSideSq->currentText().toInt();
 emit ButtonOK();
}
