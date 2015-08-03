#include "azdialgeneral.h"
#include "ui_azdialgeneral.h"

AzDialGeneral::AzDialGeneral(QWidget *parent, int typeInterface) :
    QDialog(parent),
    ui(new Ui::AzDialGeneral)
{ 
    ui->setupUi(this);
    mComboBoxOne = ui->comboBoxOne;
    mTextLabelOne = ui->label;
    mDoubleSpinBox = ui->doubleSpinBox;

    switch (typeInterface)
    {
    case 0:
        ui->label->setText("");
    case 1:
        ui->label->setText("Выберете слой:");
        ui->label_2->setText("Размер ячейки дискретного поля:");
        ui->label_2->setStatusTip("Для Projected CS ~5000; для Geographic CS ~0.001");
        ui->doubleSpinBox->setMinimum(0.0);
        ui->doubleSpinBox->setMaximum(1000000.0);
        break;
    case 2:
        break;
    }
}

AzDialGeneral::~AzDialGeneral()
{
    delete ui;
}

void AzDialGeneral::on_buttonBox_accepted()
{
    mOkClick = true;
}

void AzDialGeneral::on_buttonBox_rejected()
{
    mOkClick = false;
}
