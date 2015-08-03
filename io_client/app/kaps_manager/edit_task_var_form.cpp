#include <QtGui>

#include "ui_edit_task_var_form.h"
#include "edit_task_var_form.h"


EditTaskVarForm::EditTaskVarForm(QWidget *parent, Qt::WFlags f)
    : QDialog(parent, f),
    UI (new Ui::edit_task_var_form)
{
    UI->setupUi (this);


}

void EditTaskVarForm::on_pbOK_clicked()
{
    accept();
}

void EditTaskVarForm::on_pbCancel_clicked()
{
    reject();
}

void EditTaskVarForm::setName(const QString & name)
{
    UI->leName->setText(name);
}

QString  EditTaskVarForm::name()
{
    return UI->leName->text();
}

void EditTaskVarForm::setDesc(const QString & desc)
{
    UI->leDesc->setText(desc);
}

QString EditTaskVarForm::desc()
{
    return UI->leDesc->text();
}

void EditTaskVarForm::setFolder(const QString & folder)
{
    UI->leFolder->setText(folder);
}

QString EditTaskVarForm::folder()
{
    return UI->leFolder->text();
}

void EditTaskVarForm::setAuthor(const QString & author)
{
    UI->leAuthor->setText(author);
}

QString EditTaskVarForm::author()
{
    return UI->leAuthor->text();
}

void EditTaskVarForm::setRegion(const QString & region)
{
    UI->leRegion->setText(region);
}

QString EditTaskVarForm::region()
{
    return UI->leRegion->text();
}

void EditTaskVarForm::setCreationDate(const QDate & date)
{
    UI->deCreationDate->setDate(date);
}

QDate EditTaskVarForm::creationDate()
{
    return UI->deCreationDate->date();
}
