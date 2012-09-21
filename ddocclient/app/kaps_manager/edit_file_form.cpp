#include <QtGui>

#include "ui_edit_file_form.h"
#include "edit_file_form.h"


EditFileForm::EditFileForm(const QString & dir, QWidget *parent, Qt::WFlags f)
    : QDialog(parent, f),
    UI (new Ui::edit_file_form)
{
    UI->setupUi (this);
    this->projectDir = dir;


}

void EditFileForm::on_pbOK_clicked()
{
    accept();
}

void EditFileForm::on_pbCancel_clicked()
{
    reject();
}

void EditFileForm::setName(const QString & name)
{
    UI->leName->setText(name);
}

QString  EditFileForm::name()
{
    return UI->leName->text();
}

void EditFileForm::setDesc(const QString & desc)
{
    UI->leDesc->setText(desc);
}

QString EditFileForm::desc()
{
    return UI->leDesc->text();
}

void EditFileForm::setFilePath(const QString & path)
{
    UI->leFilePath->setText(path);
}

QString EditFileForm::filePath()
{
    return UI->leFilePath->text();
}

void EditFileForm::setKA(const QString & ka)
{
    UI->leKA->setText(ka);
}

QString EditFileForm::KA()
{
    return UI->leKA->text();
}

void EditFileForm::setRegion(const QString & region)
{
    UI->leRegion->setText(region);
}

QString EditFileForm::region()
{
    return UI->leRegion->text();
}

void EditFileForm::setTimeShoot(const QDateTime & shoot)
{
    UI->dteShoot->setDateTime(shoot);
}

QDateTime EditFileForm::timeShoot()
{
    return UI->dteShoot->dateTime();
}

void EditFileForm::on_tbAddFile_clicked()
{
    QString url = QFileDialog::getOpenFileName(this, tr("Добавление файла в проект"), projectDir);
    UI->leFilePath->setText(url);

}
