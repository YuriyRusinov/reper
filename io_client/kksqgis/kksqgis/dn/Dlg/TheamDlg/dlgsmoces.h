#ifndef DLGSMOCES_H
#define DLGSMOCES_H

#include <QDialog>
#include <QSettings>
#include "Added/dnbdspectr.h"
#include "Added/First/dnmathadd.h"

namespace Ui {
class DlgSmoces;
}

class DlgSmoces : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgSmoces(QWidget *parent = 0);
    ~DlgSmoces();
    QString NameOrg,NameProg;
    float R600por,KofCorpor;
    float LamBegin,LamEnd;

signals:
    void ButtonOK();

protected:
    virtual void showEvent(QShowEvent *);

private slots:
    void on_ButtonOK_clicked();
    
private:
    Ui::DlgSmoces *ui;
    DNBDSpectr *BDSpec;
};

#endif // DLGSMOCES_H
