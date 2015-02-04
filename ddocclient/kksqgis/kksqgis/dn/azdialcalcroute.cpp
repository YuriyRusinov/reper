#include "azdialcalcroute.h"
#include "ui_azdialcalcroute.h"

AzDialCalcRoute::AzDialCalcRoute(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AzDialCalcRoute)
{
    ui->setupUi(this);
    QObject::connect(ui->buttonOk, SIGNAL(clicked()), this, SLOT(SLOTbuttonOkClick()));
    QObject::connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(SLOTcloseDialog()));
    mComboLayerAoI = ui->comboLayerAoI;
    mComboLayerForest = ui->comboLayerForest;
    mComboLayerWater = ui->comboLayerWater;
    mComboLayerRelief = ui->comboLayerRelief;
    mComboColDirectionSlope = ui->comboColDirectionSlope;
    mComboColSlope = ui->comboColSlope;
    mProgressBar = ui->progressBar;
    mProgressBarText = ui->labelProgress;
    ui->textStart->setText("8;65");
    ui->textEnd->setText("150;61");
    mStrStart = ui->textStart->text();
    mStrFinish = ui->textEnd->text();
    QObject::connect(ui->textStart, SIGNAL(textChanged(QString)), this, SLOT(SLOTchangeTextStart(QString)));
    QObject::connect(ui->textEnd, SIGNAL(textChanged(QString)), this, SLOT(SLOTchangeTextFinish(QString)));
}

AzDialCalcRoute::~AzDialCalcRoute()
{
    delete ui;
}

void AzDialCalcRoute::setFinish()
{
    if (!ui->buttonOk->isEnabled())
    {
        ui->buttonCancel->setEnabled(true);
        ui->buttonCancel->setText("Закрыть");
        ui->buttonOk->setVisible(false);
    }
}

void AzDialCalcRoute::SLOTbuttonOkClick()
{
    if (mStrFinish.size() < 3 || mStrStart.size() < 3)
    {
        QMessageBox::information(this, "Error", "Не указаны или указаны некорректно стартовая/конечная ячейки", QMessageBox::Ok);
        return;
    }
    ui->buttonOk->setEnabled(false);
    ui->buttonCancel->setEnabled(false);
    emit SIGNALbuttonsClick(true);
}

void AzDialCalcRoute::SLOTcloseDialog()
{
    emit SIGNALbuttonsClick(false);
}

void AzDialCalcRoute::SLOTchangeTextStart(QString pStart)
{
    mStrStart = pStart;
}

void AzDialCalcRoute::SLOTchangeTextFinish(QString pFinish)
{
    mStrFinish = pFinish;
}


