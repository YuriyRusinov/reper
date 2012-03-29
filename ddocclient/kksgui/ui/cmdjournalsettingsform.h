#ifndef CMDJOURNALSETTINGSFORM_H
#define CMDJOURNALSETTINGSFORM_H

#include <QDialog>
#include <kksgui_config.h>

//#include <KKSList.h>
//#include <KKSTemplate.h>

#include <KKSList.h>
#include <KKSFilter.h>
#include <QDate>

namespace Ui
{
    class cmd_journal_settings_form;
}

class _GUI_EXPORT KKSCmdJSettings
{
public:
    bool bMyInput;
    bool bAllInput;
    bool bInputExecuted;
    bool bInputArchived;
    bool bUseInputDate;
    QDate dInputDate;

    bool bMyOutput;
    bool bAllOutput;
    bool bOutputExecuted;
    bool bOutputArchived;
    bool bUseOutputDate;
    QDate dOutputDate;

    int queryInterval;

    QString beepFile;

    bool usedOutput()
    {
        if(bMyOutput || bAllOutput)
            return true;
        return false;
    }

    bool usedInput()
    {
        if(bMyInput || bAllInput)
            return true;
        return false;
    }

    KKSCmdJSettings()
    {
        bMyInput = true;
        bAllInput = false;
        bInputExecuted = false;
        bInputArchived = false;
        bUseInputDate = false;
        dInputDate = QDate::currentDate();

        bMyOutput = false;
        bAllOutput = false;
        bOutputExecuted = false;
        bOutputArchived = false;
        bUseOutputDate = false;
        dOutputDate = QDate::currentDate();
        
        queryInterval = 1;
        beepFile = QString();
    }

    KKSCmdJSettings(const KKSCmdJSettings & other)
    {
        bMyInput = other.bMyInput;
        bAllInput = other.bAllInput;
        bInputExecuted = other.bInputExecuted;
        bInputArchived = other.bInputArchived;
        bUseInputDate = other.bUseInputDate;
        dInputDate = other.dInputDate;

        bMyOutput = other.bMyOutput;
        bAllOutput = other.bAllOutput;
        bOutputExecuted = other.bOutputExecuted;
        bOutputArchived = other.bOutputArchived;
        bUseOutputDate = other.bUseOutputDate;
        dOutputDate = other.dOutputDate;
        
        queryInterval = other.queryInterval;
        beepFile = other.beepFile;
    }

};

class _GUI_EXPORT CmdJournalSettingsForm : public QDialog
{
    Q_OBJECT

public:
    CmdJournalSettingsForm(const KKSCmdJSettings & settings, QWidget *parent = 0);
    ~CmdJournalSettingsForm();

    void setSettings(const KKSCmdJSettings & settings);
    const KKSCmdJSettings & settings();

private slots:
    void on_pbOk_clicked();
    void on_pbCancel_clicked();
    void on_cbOutputCmdDate_stateChanged(int state);
    void on_cbInputCmdDate_stateChanged(int state);

    void on_cbOutputCmd_stateChanged(int state);
    void on_cbOutputCmdAll_stateChanged(int state);
    void on_cbInputCmd_stateChanged(int state);
    void on_cbInputCmdAll_stateChanged(int state);

    void init();
    void initSettings();
    void initWidgets();
private:
    Ui::cmd_journal_settings_form * ui;

    KKSCmdJSettings m_settings;
};

#endif // CMDJOURNALSETTINGSFORM_H
