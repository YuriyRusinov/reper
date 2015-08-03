#include "msgjournalsettingsform.h"
#include "ui_msg_journal_settings_form.h"
#include <QMessageBox>
#include <QTreeWidget>

MsgJournalSettingsForm::MsgJournalSettingsForm(const KKSMsgJSettings & settings, 
                                               QWidget *parent)
    : QDialog(parent)
{
    ui = new Ui::msg_journal_settings_form;
    ui->setupUi(this);
    
    m_settings = settings;

    init();
}

MsgJournalSettingsForm::~MsgJournalSettingsForm()
{

}

void MsgJournalSettingsForm::init()
{
    ui->cbInputMsgDate->setTristate(false);
    ui->cbOutputMsgDate->setTristate(false);


    ui->deInputMsg->setDate(QDate::currentDate());
    ui->deOutputMsg->setDate(QDate::currentDate());

    connect(ui->cbInputMsgDate, SIGNAL(stateChanged(int)), this, SLOT(on_cbInputMsgDate_stateChanged(int)));
    connect(ui->cbOutputMsgDate, SIGNAL(stateChanged(int)), this, SLOT(on_cbOutputMsgDate_stateChanged(int)));

    connect(ui->cbInputMsg, SIGNAL(stateChanged(int)), this, SLOT(on_cbInputMsg_stateChanged(int)));
    connect(ui->cbOutputMsg, SIGNAL(stateChanged(int)), this, SLOT(on_cbOutputMsg_stateChanged(int)));
    //connect(ui->cbInputCmdAll, SIGNAL(stateChanged(int)), this, SLOT(on_cbInputCmdAll_stateChanged(int)));
    //connect(ui->cbOutputCmdAll, SIGNAL(stateChanged(int)), this, SLOT(on_cbOutputCmdAll_stateChanged(int)));

    initWidgets();

    ui->deInputMsg->setEnabled(ui->cbInputMsgDate->isChecked());
    ui->deOutputMsg->setEnabled(ui->cbOutputMsgDate->isChecked());
}

void MsgJournalSettingsForm::on_pbCancel_clicked()
{
    reject();
}

void MsgJournalSettingsForm::on_pbOk_clicked()
{
    accept();
}

void MsgJournalSettingsForm::on_cbOutputMsgDate_stateChanged(int state)
{
    if(state == Qt::Checked)
        ui->deOutputMsg->setEnabled(true);
    else
        ui->deOutputMsg->setEnabled(false);
}

void MsgJournalSettingsForm::on_cbInputMsgDate_stateChanged(int state)
{
    if(state == Qt::Checked)
        ui->deInputMsg->setEnabled(true);
    else
        ui->deInputMsg->setEnabled(false);
}

void MsgJournalSettingsForm::on_cbOutputMsg_stateChanged(int state)
{
    if(state == Qt::Checked){// || ui->cbOutputCmdAll->isChecked()){
        ui->gbOutputEx->setEnabled(true);
    }
    else{
        //ui->cbOutputExecuted->setChecked(false);
        //ui->cbOutputArchived->setChecked(false);
        ui->cbOutputMsgDate->setChecked(false);
        ui->gbOutputEx->setEnabled(false);
    }
}
/*
void MsgJournalSettingsForm::on_cbOutputCmdAll_stateChanged(int state)
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
*/
void MsgJournalSettingsForm::on_cbInputMsg_stateChanged(int state)
{
    if(state == Qt::Checked){// || ui->cbInputCmdAll->isChecked()){
        ui->gbInputEx->setEnabled(true);
    }
    else{
        //ui->cbInputExecuted->setChecked(false);
        //ui->cbInputArchived->setChecked(false);
        ui->cbInputMsgDate->setChecked(false);
        ui->gbInputEx->setEnabled(false);
    }
}
/*
void MsgJournalSettingsForm::on_cbInputCmdAll_stateChanged(int state)
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
*/
void MsgJournalSettingsForm::initSettings()
{
    m_settings.bMyInput = ui->cbInputMsg->isChecked();
    //m_settings.bAllInput = ui->cbInputCmdAll->isChecked();
    //m_settings.bInputExecuted = ui->cbInputExecuted->isChecked();
    //m_settings.bInputArchived = ui->cbInputArchived->isChecked();
    m_settings.bUseInputDate = ui->cbInputMsgDate->isChecked();
    m_settings.dInputDate = ui->deInputMsg->date();

    m_settings.bMyOutput = ui->cbOutputMsg->isChecked();
    //m_settings.bAllOutput = ui->cbOutputCmdAll->isChecked();
    //m_settings.bOutputExecuted = ui->cbOutputExecuted->isChecked();
    //m_settings.bOutputArchived = ui->cbOutputArchived->isChecked();
    m_settings.bUseOutputDate = ui->cbOutputMsgDate->isChecked();
    m_settings.dOutputDate = ui->deOutputMsg->date();

    m_settings.queryInterval = ui->sbInterval->value();
}

void MsgJournalSettingsForm::initWidgets()
{
    ui->cbInputMsg->setChecked(m_settings.bMyInput);
    //ui->cbInputCmdAll->setChecked(m_settings.bAllInput);
    //ui->cbInputExecuted->setChecked(m_settings.bInputExecuted);
    //ui->cbInputArchived->setChecked(m_settings.bInputArchived);
    ui->cbInputMsgDate->setChecked(m_settings.bUseInputDate);
    ui->deInputMsg->setDate(m_settings.dInputDate);

    ui->cbOutputMsg->setChecked(m_settings.bMyOutput);
    //ui->cbOutputCmdAll->setChecked(m_settings.bAllOutput);
    //ui->cbOutputExecuted->setChecked(m_settings.bOutputExecuted);
    //ui->cbOutputArchived->setChecked(m_settings.bOutputArchived);
    ui->cbOutputMsgDate->setChecked(m_settings.bUseOutputDate);
    ui->deOutputMsg->setDate(m_settings.dOutputDate);

    ui->sbInterval->setValue(m_settings.queryInterval);
}

const KKSMsgJSettings & MsgJournalSettingsForm::settings()
{
    initSettings();
    return m_settings;
}

void MsgJournalSettingsForm::setSettings(const KKSMsgJSettings & settings)
{
    m_settings = settings;
    initWidgets();
}
