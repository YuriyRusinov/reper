#ifndef DLGMAGICWAND_H
#define DLGMAGICWAND_H

#include <QDialog>
#include <QSettings>
#include "Added/dnpoly2.h"

namespace Ui {
class DlgMagicWand;
}

class DlgMagicWand : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgMagicWand(QWidget *parent = 0);
    ~DlgMagicWand();

    int x,y;
    float Contrast;
    int NumCh,mode;
    DNPoly2 *ThisPoly;

    QString NameOrg,NameProg;
signals:
    void ButtonOK();

public slots:
    void MouseClicked(int x,int y);
    
private slots:
    void on_ButtonOK_clicked();

protected:
    virtual void showEvent(QShowEvent *);

private:
    Ui::DlgMagicWand *ui;
};

#endif // DLGMAGICWAND_H
