#ifndef EDIT_FILE_FORM_H
#define EDIT_FILE_FORM_H

#include <QDialog>
class QLabel;
class QLineEdit;
class QPushButton;
class QDateTime;

namespace Ui
{
    class edit_file_form;
};


class EditFileForm: public QDialog
{
    Q_OBJECT

public:
    EditFileForm(const QString & dir, QWidget *parent=0, Qt::WFlags f=0);

    void setName(const QString & name);
    QString name();

    void setDesc(const QString & desc);
    QString desc();

    void setFilePath(const QString & path);
    QString filePath();

    void setTimeShoot(const QDateTime & shoot);
    QDateTime timeShoot();

    void setKA(const QString & ka);
    QString KA();

    void setRegion(const QString & region);
    QString region();

private slots:

    void on_pbOK_clicked();
    void on_pbCancel_clicked();

    void on_tbAddFile_clicked();


public:
	
    
    Ui::edit_file_form *UI;
    QString projectDir;

};

#endif
