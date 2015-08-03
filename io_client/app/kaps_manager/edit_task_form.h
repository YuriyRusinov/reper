#ifndef EDIT_TASK_FORM_H
#define EDIT_TASK_FORM_H

#include <QDialog>
class QLabel;
class QLineEdit;
class QPushButton;

namespace Ui
{
    class edit_task_form;
};


class EditTaskForm: public QDialog
{
    Q_OBJECT

public:
    EditTaskForm(QWidget *parent=0, Qt::WFlags f=0);

    void setName(const QString & name);
    QString name();

    void setDesc(const QString & desc);
    QString desc();

    void setFolder(const QString & folder);
    QString folder();

    private slots:

    void on_pbOK_clicked();
    void on_pbCancel_clicked();

public:
    
    Ui::edit_task_form *UI;

};

#endif
