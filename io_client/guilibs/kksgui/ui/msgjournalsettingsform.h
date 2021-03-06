#ifndef MSGJOURNALSETTINGSFORM_H
#define MSGJOURNALSETTINGSFORM_H

#include <QDialog>
#include <kksgui_config.h>

//#include <KKSList.h>
//#include <KKSTemplate.h>

#include <KKSList.h>
#include <KKSFilter.h>
#include <QDate>

namespace Ui
{
    class msg_journal_settings_form;
}

class _GUI_EXPORT KKSMsgJSettings
{
public:
    bool bMyInput;
    //bool bAllInput;
    //bool bInputExecuted;
    //bool bInputArchived;
    bool bUseInputDate;
    QDate dInputDate;

    bool bMyOutput;
    //bool bAllOutput;
    //bool bOutputExecuted;
    //bool bOutputArchived;
    bool bUseOutputDate;
    QDate dOutputDate;

    int queryInterval;

    QString beepFile;

    bool usedOutput()
    {
        if(bMyOutput)
            return true;
        return false;
    }

    bool usedInput()
    {
        if(bMyInput)
            return true;
        return false;
    }

    KKSMsgJSettings()
    {
        bMyInput = true;
        //bAllInput = false;
        //bInputExecuted = false;
        //bInputArchived = false;
        bUseInputDate = false;
        dInputDate = QDate::currentDate();

        bMyOutput = false;
        //bAllOutput = false;
        //bOutputExecuted = false;
        //bOutputArchived = false;
        bUseOutputDate = false;
        dOutputDate = QDate::currentDate();
        
        queryInterval = 1;
        beepFile = QString();
    }

    KKSMsgJSettings(const KKSMsgJSettings & other)
    {
        bMyInput = other.bMyInput;
        //bAllInput = other.bAllInput;
        //bInputExecuted = other.bInputExecuted;
        //bInputArchived = other.bInputArchived;
        bUseInputDate = other.bUseInputDate;
        dInputDate = other.dInputDate;

        bMyOutput = other.bMyOutput;
        //bAllOutput = other.bAllOutput;
        //bOutputExecuted = other.bOutputExecuted;
        //bOutputArchived = other.bOutputArchived;
        bUseOutputDate = other.bUseOutputDate;
        dOutputDate = other.dOutputDate;
        
        queryInterval = other.queryInterval;
        beepFile = other.beepFile;
    }

};

class _GUI_EXPORT MsgJournalSettingsForm : public QDialog
{
    Q_OBJECT

public:
    MsgJournalSettingsForm(const KKSMsgJSettings & settings, QWidget *parent = 0);
    virtual ~MsgJournalSettingsForm();

    void setSettings(const KKSMsgJSettings & settings);
    const KKSMsgJSettings & settings();

private slots:
    void on_pbOk_clicked();
    void on_pbCancel_clicked();
    void on_cbOutputMsgDate_stateChanged(int state);
    void on_cbInputMsgDate_stateChanged(int state);

    void on_cbOutputMsg_stateChanged(int state);
    //void on_cbOutputCmdAll_stateChanged(int state);
    void on_cbInputMsg_stateChanged(int state);
    //void on_cbInputCmdAll_stateChanged(int state);

    void init();
    void initSettings();
    void initWidgets();
private:
    Ui::msg_journal_settings_form * ui;

    KKSMsgJSettings m_settings;
};

#endif // CMDJOURNALSETTINGSFORM_H
