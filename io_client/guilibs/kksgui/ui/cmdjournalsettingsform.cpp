#include "cmdjournalsettingsform.h"
#include "ui_cmd_journal_settings_form.h"
#include <QMessageBox>
#include <QTreeWidget>

CmdJournalSettingsForm::CmdJournalSettingsForm(const KKSCmdJSettings & settings, 
                                               QWidget *parent)
    : QDialog(parent)
{
    ui = new Ui::cmd_journal_settings_form;
    ui->setupUi(this);
    
    m_settings = settings;

    init();
}

CmdJournalSettingsForm::~CmdJournalSettingsForm()
{

}

void CmdJournalSettingsForm::init()
{
    ui->cbInputCmdDate->setTristate(false);
    ui->cbOutputCmdDate->setTristate(false);


    ui->deInputCmd->setDate(QDate::currentDate());
    ui->deOutputCmd->setDate(QDate::currentDate());

    connect(ui->cbInputCmdDate, SIGNAL(stateChanged(int)), this, SLOT(on_cbInputCmdDate_stateChanged(int)));
    connect(ui->cbOutputCmdDate, SIGNAL(stateChanged(int)), this, SLOT(on_cbOutputCmdDate_stateChanged(int)));

    connect(ui->cbInputCmd, SIGNAL(stateChanged(int)), this, SLOT(on_cbInputCmd_stateChanged(int)));
    connect(ui->cbOutputCmd, SIGNAL(stateChanged(int)), this, SLOT(on_cbOutputCmd_stateChanged(int)));
    connect(ui->cbInputCmdAll, SIGNAL(stateChanged(int)), this, SLOT(on_cbInputCmdAll_stateChanged(int)));
    connect(ui->cbOutputCmdAll, SIGNAL(stateChanged(int)), this, SLOT(on_cbOutputCmdAll_stateChanged(int)));

    initWidgets();

    ui->deInputCmd->setEnabled(ui->cbInputCmdDate->isChecked());
    ui->deOutputCmd->setEnabled(ui->cbOutputCmdDate->isChecked());
}

void CmdJournalSettingsForm::on_pbCancel_clicked()
{
    reject();
}

void CmdJournalSettingsForm::on_pbOk_clicked()
{
    accept();
}

void CmdJournalSettingsForm::on_cbOutputCmdDate_stateChanged(int state)
{
    if(state == Qt::Checked)
        ui->deOutputCmd->setEnabled(true);
    else
        ui->deOutputCmd->setEnabled(false);
}

void CmdJournalSettingsForm::on_cbInputCmdDate_stateChanged(int state)
{
    if(state == Qt::Checked)
        ui->deInputCmd->setEnabled(true);
    else
        ui->deInputCmd->setEnabled(false);
}

void CmdJournalSettingsForm::on_cbOutputCmd_stateChanged(int state)
{
    if(state == Qt::Checked || ui->cbOutputCmdAll->isChecked()){
        ui->gbOutputEx->setEnabled(true);
    }
    else{
        ui->cbOutputExecuted->setChecked(false);
        ui->cbOutputArchived->setChecked(false);
        ui->cbOutputCmdDate->setChecked(false);
        ui->gbOutputEx->setEnabled(false);
    }
}

void CmdJournalSettingsForm::on_cbOutputCmdAll_stateChanged(int state)
{
    if(state == Qt::Checked || ui->cbOutputCmd->isChecked()){
        ui->gbOutputEx->setEnabled(true);
    }
    else{
        ui->cbOutputExecuted->setChecked(false);
        ui->cbOutputArchived->setChecked(false);
        ui->cbOutputCmdDate->setChecked(false);
        ui->gbOutputEx->setEnabled(false);
    }
}

void CmdJournalSettingsForm::on_cbInputCmd_stateChanged(int state)
{
    if(state == Qt::Checked || ui->cbInputCmdAll->isChecked()){
        ui->gbInputEx->setEnabled(true);
    }
    else{
        ui->cbInputExecuted->setChecked(false);
        ui->cbInputArchived->setChecked(false);
        ui->cbInputCmdDate->setChecked(false);
        ui->gbInputEx->setEnabled(false);
    }
}

void CmdJournalSettingsForm::on_cbInputCmdAll_stateChanged(int state)
{
    if(state == Qt::Checked || ui->cbInputCmd->isChecked()){
        ui->gbInputEx->setEnabled(true);
    }
    else{
        ui->cbInputExecuted->setChecked(false);
        ui->cbInputArchived->setChecked(false);
        ui->cbInputCmdDate->setChecked(false);
        ui->gbInputEx->setEnabled(false);
    }
}

void CmdJournalSettingsForm::initSettings()
{
    m_settings.bMyInput = ui->cbInputCmd->isChecked();
    m_settings.bAllInput = ui->cbInputCmdAll->isChecked();
    m_settings.bInputExecuted = ui->cbInputExecuted->isChecked();
    m_settings.bInputArchived = ui->cbInputArchived->isChecked();
    m_settings.bUseInputDate = ui->cbInputCmdDate->isChecked();
    m_settings.dInputDate = ui->deInputCmd->date();

    m_settings.bMyOutput = ui->cbOutputCmd->isChecked();
    m_settings.bAllOutput = ui->cbOutputCmdAll->isChecked();
    m_settings.bOutputExecuted = ui->cbOutputExecuted->isChecked();
    m_settings.bOutputArchived = ui->cbOutputArchived->isChecked();
    m_settings.bUseOutputDate = ui->cbOutputCmdDate->isChecked();
    m_settings.dOutputDate = ui->deOutputCmd->date();

    m_settings.queryInterval = ui->sbInterval->value();
}

void CmdJournalSettingsForm::initWidgets()
{
    ui->cbInputCmd->setChecked(m_settings.bMyInput);
    ui->cbInputCmdAll->setChecked(m_settings.bAllInput);
    ui->cbInputExecuted->setChecked(m_settings.bInputExecuted);
    ui->cbInputArchived->setChecked(m_settings.bInputArchived);
    ui->cbInputCmdDate->setChecked(m_settings.bUseInputDate);
    ui->deInputCmd->setDate(m_settings.dInputDate);

    ui->cbOutputCmd->setChecked(m_settings.bMyOutput);
    ui->cbOutputCmdAll->setChecked(m_settings.bAllOutput);
    ui->cbOutputExecuted->setChecked(m_settings.bOutputExecuted);
    ui->cbOutputArchived->setChecked(m_settings.bOutputArchived);
    ui->cbOutputCmdDate->setChecked(m_settings.bUseOutputDate);
    ui->deOutputCmd->setDate(m_settings.dOutputDate);

    ui->sbInterval->setValue(m_settings.queryInterval);
}

const KKSCmdJSettings & CmdJournalSettingsForm::settings()
{
    initSettings();
    return m_settings;
}

void CmdJournalSettingsForm::setSettings(const KKSCmdJSettings & settings)
{
    m_settings = settings;
    initWidgets();
}
