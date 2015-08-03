#ifndef DLGROUTMOVE_H
#define DLGROUTMOVE_H

#include <QDialog>
#include <QSettings>
#include "Added/dnbdspectr.h"
#include "Added/First/dnmathadd.h"

namespace Ui {
class DlgRoutMove;
}

class DlgRoutMove : public QDialog
{
    Q_OBJECT

    struct NumAndNameLandScape
    {
     int NumLandScape;
     QString NameLandScape;
    };
public:


    explicit DlgRoutMove(QWidget *parent = 0);
    ~DlgRoutMove();
    QList <DlgRoutMove::NumAndNameLandScape> LandScapeList;
    int NumCl,I;
    float Qc;
    float MaxSKO;
    float NDVIPor;
    float LengthPor;
    float MultiCh;
    int Fenolog;
    QString NameOrg,NameProg;
    bool IsDlgShow;

signals:
    void ButtonOK();

protected:
    virtual void showEvent(QShowEvent *);
    
private slots:
    void on_ButtonOK_clicked();

    void on_ComboFen_currentIndexChanged(int index);

    void on_ComboLandScape_currentIndexChanged(int index);

private:
    Ui::DlgRoutMove *ui;
    DNBDSpectr *BDSpec;
    void FillFormFromBD(int NumLandScape);

};

#endif // DLGROUTMOVE_H
