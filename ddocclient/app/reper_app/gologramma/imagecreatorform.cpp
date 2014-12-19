#include "imagecreatorform.h"
#include "ui_image_creator_form.h"

imageCreatorForm::imageCreatorForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::image_creator_form)
{
    ui->setupUi(this);

    setLayout(ui->mainLayout);

    formInit();
}

imageCreatorForm::~imageCreatorForm()
{
    delete ui;
}

void imageCreatorForm::formInit()
{
    connect(ui->exitButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->selectFolder,SIGNAL(clicked()),this,SLOT(selectFileName()));
//    connect(ui->loadModel,SIGNAL(clicked()),this,SLOT(loadFileModel()));
//    connect(ui->resolutionBox,SIGNAL(valueChanged(int)),this,SLOT(controllSpinBox()));
//    connect(ui->resolutionBox,SIGNAL(valueChanged(QString)),this,SLOT(controllSpinBox()));
    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(build()));

    ui->startButton->setEnabled(false);
    ui->modelBox->setChecked(false);

    ui->loadModel->hide();
    ui->modelBox->hide();

    ui->shipLengthBox->setValue(300);
    ui->resolutionBox->setValue(10);
    ui->maxXYBox->setValue(360);
    ui->maxXZBox->setValue(80);
}

void imageCreatorForm::selectFileName()
{
    QFileDialog* fileDialog = new QFileDialog(this);
    QString buf;

    buf = fileDialog->getOpenFileName();

    delete fileDialog;

    ui->lineEdit->setText(buf);
    ui->modelBox->setEnabled(true);
    ui->modelBox->setChecked(false);
    ui->modelBox->setEnabled(false);
    ui->startButton->setEnabled(false);

    ui->startButton->setEnabled(true);
}

int imageCreatorForm::controllSpinBox()
{
    if(ui->maxXYBox->value() <= ui->minXYBox->value() ||
       ui->maxXZBox->value() <= ui->minXZBox->value())
    {
       QMessageBox msgBox;
       msgBox.setText("Incorrect max, min values!");
       return msgBox.exec();
    }

    if(((ui->maxXYBox->value() - ui->minXYBox->value())%ui->angleStepBox->value()) ||
       ((ui->maxXZBox->value() - ui->minXZBox->value())%ui->angleStepXZBox->value())
      )
    {
       QMessageBox msgBox;
       msgBox.setText("Incorrect step values!");
       return msgBox.exec();
    }

    return 0;
}

void imageCreatorForm::build()
{
    if(controllSpinBox())
        return;

    ui->selectFolder->setEnabled(false);
    ui->loadModel->setEnabled(false);
    ui->startButton->setEnabled(false);

    generatingDataPlus gData;
    gData.filename = ui->lineEdit->text();
    gData.data.lengthOfShip = static_cast<double>(ui->shipLengthBox->value());
    gData.data.numberOfUnit = (ui->resolutionBox->value());
    gData.data.XY_angleMax = (ui->maxXYBox->value());
    gData.data.XY_angleMin = (ui->minXYBox->value());
    gData.data.XY_angleStep = (ui->angleStepBox->value());
    gData.data.XZ_angleMax = (ui->maxXZBox->value());
    gData.data.XZ_angleMin = (ui->minXZBox->value());
    gData.data.XZ_angleStep = (ui->angleStepXZBox->value());

    emit imagesData(gData);

    ui->selectFolder->setEnabled(true);
    ui->loadModel->setEnabled(true);
    ui->startButton->setEnabled(true);
}
