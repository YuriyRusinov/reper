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
}

AzDialCalcRoute::~AzDialCalcRoute()
{
    delete ui;
}

void AzDialCalcRoute::SLOTbuttonOkClick()
{
    ui->buttonOk->setEnabled(false);
    ui->buttonCancel->setEnabled(false);
    emit SIGNALbuttonsClick(true);
}

void AzDialCalcRoute::SLOTcloseDialog()
{
    emit SIGNALbuttonsClick(false);
}


