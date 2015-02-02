#ifndef DLGCREATEIMGPOLY_H
#define DLGCREATEIMGPOLY_H

#include <QDialog>
#include "Added/dnpoly2.h"

namespace Ui {
class DlgCreateImgPoly;
}

class DlgCreateImgPoly : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgCreateImgPoly(QWidget *parent = 0);
    ~DlgCreateImgPoly();

    int NumChR,NumChG,NumChB;
    float KofR,KofG,KofB;

    DNPoly2 *ThisPoly;
signals:
    void ButtonOK();
protected:
    virtual void showEvent(QShowEvent *);

public slots:
    /*Слот изменения количества каналов в Combo, при изменении текущего полигона или добавления канала в текущий полигон*/
    void on_ChangeNumChan();
private slots:
    void on_ButtonOK_clicked();

private:
    Ui::DlgCreateImgPoly *ui;
};

#endif // DLGCREATEIMGPOLY_H
