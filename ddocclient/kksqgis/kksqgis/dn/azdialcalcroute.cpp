#include "azdialcalcroute.h"
#include "ui_azdialcalcroute.h"

AzDialCalcRoute::AzDialCalcRoute(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AzDialCalcRoute)
{
    ui->setupUi(this);
    ui->comboLayerAoI->addItem("route_army_area_19-07-43_a");
    ui->comboLayerForest->addItem("roadmovie_a");
    ui->comboLayerRelief->addItem("triang_2");
    ui->comboColDirectionSlope->addItem("grad");
    ui->comboColSlope->addItem("slope");



}

AzDialCalcRoute::~AzDialCalcRoute()
{
    delete ui;
}
