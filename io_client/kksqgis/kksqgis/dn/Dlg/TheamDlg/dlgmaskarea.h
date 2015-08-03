#ifndef DLGMASKAREA_H
#define DLGMASKAREA_H

#include <QDialog>
#include <QSettings>
#include <QListWidgetItem>
#include <QRadioButton>
#include "Added/dnbdspectr.h"
#include "Added/First/dnmathadd.h"

namespace Ui {
class DlgMaskArea;
}

class DlgMaskArea : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgMaskArea(QWidget *parent = 0);
    ~DlgMaskArea();
    QString NameOrg,NameProg;
    float DeltaMS,MSIndexPor;
    float DeltaSub,GSIndexPor,DeltaGS,MaxDeriviant;
    float DerivLam1,DerivLam2;
    int TypeFone;
    QString ClsId;
    QList<QString> ClsBackgroId;
    QList <QString> ListNameMask;
    QList <int> ListClsIdNameMask;
    QList <int> ListBackgroIdNameMask;


    float LamNir1MS1,LamNir1MS2,LamNir2MS1,LamNir2MS2,LamRedMS1,LamRedMS2,LamGrMS1,LamGrMS2;

signals:
    void ButtonOK();
    
private slots:
    void on_ButtonOK_clicked();

    void on_ListNameMask_itemClicked(QListWidgetItem *item);



    void on_ComboTypeFone_currentIndexChanged(int index);

protected:
    virtual void showEvent(QShowEvent *);

private:
    Ui::DlgMaskArea *ui;
    DNBDSpectr *BDSpec;
};

#endif // DLGMASKAREA_H
