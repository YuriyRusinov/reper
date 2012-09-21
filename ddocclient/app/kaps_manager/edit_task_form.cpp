#include <QtGui>

#include "ui_edit_task_form.h"
#include "edit_task_form.h"


EditTaskForm::EditTaskForm(QWidget *parent, Qt::WFlags f)
    : QDialog(parent, f),
    UI (new Ui::edit_task_form)
{
    UI->setupUi (this);


}

void EditTaskForm::on_pbOK_clicked()
{
    accept();
}

void EditTaskForm::on_pbCancel_clicked()
{
    reject();
}

void EditTaskForm::setName(const QString & name)
{
    UI->leName->setText(name);
}

QString  EditTaskForm::name()
{
    return UI->leName->text();
}

void EditTaskForm::setDesc(const QString & desc)
{
    UI->leDesc->setText(desc);
}

QString EditTaskForm::desc()
{
    return UI->leDesc->text();
}

void EditTaskForm::setFolder(const QString & folder)
{
    UI->leFolder->setText(folder);
}

QString EditTaskForm::folder()
{
    return UI->leFolder->text();
}
