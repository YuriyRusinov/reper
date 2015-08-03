#ifndef DLGSAM_H
#define DLGSAM_H

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QStringList>
#include "Added/dnpoly2.h"

namespace Ui {
class DlgSAM;
}

class DlgSAM : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgSAM(QWidget *parent = 0);
    ~DlgSAM();

    bool *MaskCh;
    QStringList PolyFilesNames;
    DNPoly2 *ThisPoly;
    bool IsSKOEnable;
    float LimitSKO;

    QString NameOrg,NameProg;
    QString PathData;
    QString FileNameChan;
    
private:
    Ui::DlgSAM *ui;
signals:
    void ButtonOK();

protected:
    virtual void showEvent(QShowEvent *);
private slots:
    void on_ButtonAdd_clicked();
    void on_ButtonDell_clicked();
    void on_ButtonOK_clicked();
};

#endif // DLGSAM_H
