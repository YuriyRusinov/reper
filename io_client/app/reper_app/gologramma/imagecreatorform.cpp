#include <QComboBox>

#include "imagecreatorform.h"
#include "ui_image_creator_form.h"

imageCreatorForm::imageCreatorForm(QWidget *parent, Qt::WindowFlags flags) :
    QDialog(parent, flags),
    ui(new Ui::imageCreatorForm)
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
    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(build()));

    ui->startButton->setEnabled(false);
    ui->modelBox->setChecked(false);

    ui->loadModel->hide();
    ui->modelBox->hide();

    ui->shipLengthBox->setValue(300);
    ui->resolutionBox->setValue(10);
    ui->maxXYBox->setValue(120);
    ui->maxXZBox->setValue(40);
    ui->angleStepBox->setValue (60);
    ui->angleStepXZBox->setValue (20);
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

bool imageCreatorForm::forTests (void) const
{
    return (ui->cbTests->checkState() != Qt::Unchecked);
}

void imageCreatorForm::initShipTypes (const QMap<int, QString>& shipTypes)
{
    for (QMap<int, QString>::const_iterator p=shipTypes.constBegin();
             p != shipTypes.constEnd();
             p++)
        ui->cbShipType->addItem (p.value(), p.key());
}

int imageCreatorForm::getShipType (void) const
{
    return ui->cbShipType->itemData (ui->cbShipType->currentIndex()).toInt();
}
