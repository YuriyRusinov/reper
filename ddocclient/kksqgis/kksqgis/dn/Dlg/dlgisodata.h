#ifndef DLGISODATA_H
#define DLGISODATA_H

#include <QSettings>
#include <QString>
#include <QDialog>
#include <QFile>
#include "Added/dnpoly2.h"

namespace Ui {
class DlgIsoData;
}

class DlgIsoData : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgIsoData(QWidget *parent = 0);
    ~DlgIsoData();

    int KolvoKl,KolvoIt;
    float MinDist;
    float MaxSKO;
    bool *MaskCh;
    DNPoly2 *ThisPoly;

    QString NameOrg,NameProg;
    QString PathData;
    QString FileNameChan;
    float *Lamda;
    bool IsChanSpecColibr;
signals:
    void ButtonOK();

protected:
    virtual void showEvent(QShowEvent *);

private slots:
    void on_ButtonOK_clicked();

private:
    Ui::DlgIsoData *ui;
};

#endif // DLGISODATA_H
