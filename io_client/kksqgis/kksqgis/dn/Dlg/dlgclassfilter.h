#ifndef DLGCLASSFILTER_H
#define DLGCLASSFILTER_H

#include <QDialog>
#include <QMap>
#include <QListWidgetItem>
#include <DNWidgetimage.h>
#include "Added/dnpoly2.h"

namespace Ui {
class DlgClassFilter;
}

class DlgClassFilter : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgClassFilter(QWidget *parent = 0);
    ~DlgClassFilter();

    int *NewClassif;
    DNPoly2 *ThisPoly;
    DNWidgetImage *WidgetChange;
    QMap <int,quint64> ClassPix;


protected:
    virtual void showEvent(QShowEvent *);

private slots:
    void on_ListClassif_itemClicked(QListWidgetItem *item);
    void on_ButtonOK_clicked();

signals:
    void ButtonOK();

private:
    Ui::DlgClassFilter *ui;
};

#endif // DLGCLASSFILTER_H
