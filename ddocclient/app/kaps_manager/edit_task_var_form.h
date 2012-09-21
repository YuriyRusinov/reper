#ifndef EDIT_TASK_VAR_FORM_H
#define EDIT_TASK_VAR_FORM_H

#include <QDialog>
class QLabel;
class QDate;
class QLineEdit;
class QPushButton;

namespace Ui
{
    class edit_task_var_form;
};


class EditTaskVarForm: public QDialog
{
    Q_OBJECT

public:
    EditTaskVarForm(QWidget *parent=0, Qt::WFlags f=0);

    void setName(const QString & name);
    QString name();

    void setDesc(const QString & desc);
    QString desc();

    void setFolder(const QString & folder);
    QString folder();

    void setCreationDate(const QDate & date);
    QDate creationDate();

    void setAuthor(const QString & author);
    QString author();

    void setRegion(const QString & region);
    QString region();

private slots:

    void on_pbOK_clicked();
    void on_pbCancel_clicked();

public:
	
    
    Ui::edit_task_var_form *UI;

};

#endif
