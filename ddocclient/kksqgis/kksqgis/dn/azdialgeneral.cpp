#include "azdialgeneral.h"
#include "ui_azdialgeneral.h"

AzDialGeneral::AzDialGeneral(QWidget *parent, int typeInterface) :
    QDialog(parent),
    ui(new Ui::AzDialGeneral)
{ 
    ui->setupUi(this);
    mComboBoxOne = ui->comboBoxOne;
    mTextLabelOne = ui->label;

    switch (typeInterface)
    {
    case 0:
        ui->label->setText("");
    case 1:
        ui->label->setText("Выберете слой:");
        break;
    case 2:
        break;
    }
}

AzDialGeneral::~AzDialGeneral()
{
    delete ui;
}
