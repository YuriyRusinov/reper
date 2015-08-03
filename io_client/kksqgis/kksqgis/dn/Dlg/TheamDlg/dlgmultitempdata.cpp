#include "dlgmultitempdata.h"
#include "ui_dlgmultitempdata.h"

DlgMultiTempData::DlgMultiTempData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMultiTempData)
{
    ui->setupUi(this);
}

DlgMultiTempData::~DlgMultiTempData()
{
    delete ui;
}

void DlgMultiTempData::showEvent(QShowEvent *)
{
 QSettings Settings(this->NameOrg,this->NameProg);

 LeftPor=Settings.value("MultiTempData/LeftPor",0).toFloat();
 RigthPor=Settings.value("MultiTempData/RigthPor",0).toFloat();

 ui->EditLeftPor->setText(QString().setNum(LeftPor,'d',3));
 ui->EditRigthPor->setText(QString().setNum(RigthPor,'d',3));
}

void DlgMultiTempData::on_ButtonOK_clicked()
{
 LeftPor=ui->EditLeftPor->text().toFloat();
 RigthPor=ui->EditRigthPor->text().toFloat();
 FileName=ui->EditObzor->text();

 QSettings Settings(this->NameOrg,this->NameProg);
 Settings.setValue("MultiTempData/LeftPor",LeftPor);
 Settings.setValue("MultiTempData/RigthPor",RigthPor);
 emit ButtonOK();
}

void DlgMultiTempData::on_pushButton_3_clicked()
{
 QString FileNameOpen;
 FileNameOpen=QFileDialog::getOpenFileName(this,"Открыть файл проекта","","Raster files (*.img *.asc *.tif *tiff *.bmp *.jpg *.jpeg *.ser *.int);;Geotiff (*.tif *.tiff)");
 if(FileNameOpen.isEmpty())
 {
  ui->EditObzor->setText(FileNameOpen);
 }
}
