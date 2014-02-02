#include "optionsdialog.h"
#include "ui_options_dialog.h"

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);

    connect(ui->Ok,SIGNAL(clicked()),this,SLOT(okClicked()));
    connect(ui->Cancel,SIGNAL(clicked()),this,SLOT(cancelClicked()));
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::okClicked()
{
    emit okClic();
}

void OptionsDialog::cancelClicked()
{
    emit cancelClic();
}

QString OptionsDialog::getDBhost()
{
    return ui->lineEdit_hostDB->text();
}

QString OptionsDialog::getDBport()
{
    return ui->lineEdit_portDB->text();
}

QString OptionsDialog::getDBuser()
{
    return ui->lineEdit_user->text();
}

QString OptionsDialog::getDBpass()
{
    return ui->lineEdit_password->text();
}

QString OptionsDialog::getDBname()
{
    return ui->lineEdit_DBname->text();
}

QString OptionsDialog::getTransport()
{
    return ui->lineEdit_transport->text();
}

QString OptionsDialog::getTimer()
{
    return ui->lineEdit_delay->text();
}

QString OptionsDialog::getTimerUnits()
{
    return ui->lineEdit_timerUnits->text();
}

bool OptionsDialog::getTimerMode()
{
    return ui->checkBox->isChecked();
}

QString OptionsDialog::getGatewayHost()
{
    return ui->lineEdit_hostGateway->text();
}

QString OptionsDialog::getGatewayPort()
{
    return ui->lineEdit_portGateway->text();
}

QString OptionsDialog::getServerHost()
{
    return QString("192.168.17.118");//ui->lineEdit_hostServer->text();
}

QString OptionsDialog::getServerPort()
{
    return ui->lineEdit_portServer->text();
}

void OptionsDialog::setDBhost(QString rhs)
{
    ui->lineEdit_hostDB->setText(rhs);
}

void OptionsDialog::setDBport(QString rhs)
{
    ui->lineEdit_portDB->setText(rhs);
}

void OptionsDialog::setDBuser(QString rhs)
{
    ui->lineEdit_user->setText(rhs);
}

void OptionsDialog::setDBpass(QString rhs)
{
    ui->lineEdit_password->setText(rhs);
}

void OptionsDialog::setDBname(QString rhs)
{
    ui->lineEdit_DBname->setText(rhs);
}

void OptionsDialog::setTransport(QString rhs)
{
    ui->lineEdit_transport->setText(rhs);
}

void OptionsDialog::setTimer(QString rhs)
{
    ui->lineEdit_delay->setText(rhs);
}

void OptionsDialog::setTimerUnits(QString rhs)
{
    ui->lineEdit_timerUnits->setText(rhs);
}

void OptionsDialog::setTimerMode(bool rhs)
{
    ui->checkBox->setChecked(rhs);
}

void OptionsDialog::setGatewayHost(QString rhs)
{
    ui->lineEdit_hostGateway->setText(rhs);
}

void OptionsDialog::setGatewayPort(QString rhs)
{
    ui->lineEdit_portGateway->setText(rhs);
}

void OptionsDialog::setServerHost(QString rhs)
{
    //ui->lineEdit_hostServer->setText(rhs);
}

void OptionsDialog::setServerPort(QString rhs)
{
    ui->lineEdit_portServer->setText(rhs);
}
