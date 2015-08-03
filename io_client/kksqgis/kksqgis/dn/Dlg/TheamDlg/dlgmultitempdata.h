#ifndef DLGMULTITEMPDATA_H
#define DLGMULTITEMPDATA_H

#include <QDialog>
#include <QSettings>
#include <QFileDialog>

namespace Ui {
class DlgMultiTempData;
}

class DlgMultiTempData : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgMultiTempData(QWidget *parent = 0);
    ~DlgMultiTempData();
    QString NameOrg,NameProg;
    QString FileName;
    float LeftPor,RigthPor;


signals:
    void ButtonOK();
protected:
    virtual void showEvent(QShowEvent *);
    
private slots:
    void on_ButtonOK_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::DlgMultiTempData *ui;
};

#endif // DLGMULTITEMPDATA_H
