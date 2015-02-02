#ifndef DLGEMBEDOBJ_H
#define DLGEMBEDOBJ_H

#include <QDialog>
#include "Added/dnbdspectr.h"
#include "Added/First/dnmathadd.h"

namespace Ui {
class DlgEmbedObj;
}

class DlgEmbedObj : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgEmbedObj(QWidget *parent = 0);
    ~DlgEmbedObj();
    QString NameOrg,NameProg;
    float DeltaMS,MSIndex;
    float Contrast630_690,Contrast800_1000;
    float DeltaGS,GSIndex;
    float WBI;
    float AKFMin;
    float KofCorelPor;
    float Brigth630_690,Brigth800_1000;

    float LamMSIndex1[2],LamMSIndex2[2];
    float LamGSIndex[2];
    float LamContr1[2],LamContr2[2];

signals:
    void ButtonOK();

protected:
    virtual void showEvent(QShowEvent *);
private slots:
    void on_ButtonOK_clicked();

    void on_ListNameEmbedObj_currentRowChanged(int currentRow);

private:
    Ui::DlgEmbedObj *ui;
    DNBDSpectr *BDSpec;
     QList <DNBDSpectr::NameAndNum> TypeEmbedObj;
    void FillForm();
};

#endif // DLGEMBEDOBJ_H
