#ifndef DLGROUTMOVE2_H
#define DLGROUTMOVE2_H

#include <QDialog>
#include <QSettings>
#include "Added/dnbdspectr.h"
#include "Added/First/dnmathadd.h"

namespace Ui {
class DlgRoutMove2;
}

class DlgRoutMove2 : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgRoutMove2(QWidget *parent = 0);
    ~DlgRoutMove2();
    float PorNDVI1,PorNDVI2,PorNDVI3,DeltaNDVI1,DeltaNDVI2,DeltaNDVI3,PorWBI;
    int Fenolog;
    QString NameOrg,NameProg;

    float Lam1NDVI1[2],Lam2NDVI1[2],Lam1NDVI2[2],Lam2NDVI2[2],Lam1NDVI3[2],Lam2NDVI3[2],Lam1WBI,Lam2WBI;

signals:
    void ButtonOK();
protected:
    virtual void showEvent(QShowEvent *);
private slots:
    void on_ButtonOK_clicked();

    void on_ComboFen_currentIndexChanged(int index);

private:
    Ui::DlgRoutMove2 *ui;
    DNBDSpectr *BDSpec;
    void FillFormFromBD();
};

#endif // DLGROUTMOVE2_H
