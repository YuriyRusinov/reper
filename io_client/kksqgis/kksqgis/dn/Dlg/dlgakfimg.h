#ifndef DLGAKFIMG_H
#define DLGAKFIMG_H

#include <QDialog>
#include "Added/dnpoly2.h"

namespace Ui {
class DlgAKFImg;
}

class DlgAKFImg : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgAKFImg(QWidget *parent = 0);
    ~DlgAKFImg();

    DNPoly2 *ThisPoly;
    int XMouse,YMouse;
    int Side,NumCh;

    void FillMouseCoord(int x,int y);

signals:
    void ButtonOK();
    void ComboSideSqClick(int SizeSideSq);

protected:
    virtual void showEvent(QShowEvent *);
private slots:
    void on_ComboSideSq_currentIndexChanged(const QString &arg1);

    void on_ButtonOK_clicked();

private:
    Ui::DlgAKFImg *ui;
};

#endif // DLGAKFIMG_H
