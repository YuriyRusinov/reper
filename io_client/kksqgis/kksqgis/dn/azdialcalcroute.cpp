#include "azdialcalcroute.h"
#include "ui_azdialcalcroute.h"

AzDialCalcRoute::AzDialCalcRoute(QSettings *linkSettings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AzDialCalcRoute)
{
    ui->setupUi(this);
    QObject::connect(ui->buttonOk, SIGNAL(clicked()), this, SLOT(SLOTbuttonOkClick()));
    QObject::connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(SLOTcloseDialog()));
    this->linkSettings = linkSettings;
    mComboLayerAoI = ui->comboLayerAoI;
    mComboLayerForest = ui->comboLayerForest;
    mComboLayerWater = ui->comboLayerWater;
    mComboLayerRelief = ui->comboLayerRelief;
    mComboColDirectionSlope = ui->comboColDirectionSlope;
    mComboColSlope = ui->comboColSlope;
    mProgressBar = ui->progressBar;
    mProgressBarText = ui->labelProgress;
    ui->textStart->setText(linkSettings->value("ShortestPath/StartXY", "1;1").toString());
    ui->textEnd->setText(linkSettings->value("ShortestPath/FinishXY", "2;2").toString());
    mStrStart = ui->textStart->text();
    mStrFinish = ui->textEnd->text();
    QObject::connect(ui->textStart, SIGNAL(textChanged(QString)), this, SLOT(SLOTchangeTextStart(QString)));
    QObject::connect(ui->textEnd, SIGNAL(textChanged(QString)), this, SLOT(SLOTchangeTextFinish(QString)));
}

AzDialCalcRoute::~AzDialCalcRoute()
{
    delete ui;
}

void AzDialCalcRoute::setFinish(bool bReturn)
{
    if (bReturn)
    {
        ui->buttonOk->setEnabled(true);
        ui->buttonCancel->setEnabled(true);
    }
    else
    {
        if (!ui->buttonOk->isEnabled())
        {
            mProgressBar->setMinimum(0);
            mProgressBar->setMaximum(1);
            mProgressBar->setValue(1);
            ui->buttonCancel->setEnabled(true);
            ui->buttonCancel->setText("Закрыть");
            ui->buttonOk->setVisible(false);
        }
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


