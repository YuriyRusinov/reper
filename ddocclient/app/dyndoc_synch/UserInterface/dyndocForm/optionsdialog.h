#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT
    
signals:
    void okClic();
    void cancelClic();

public slots:
    void okClicked();
    void cancelClicked();

public:
    explicit OptionsDialog(QWidget *parent = 0);
    ~OptionsDialog();

    QString getDBhost();
    QString getDBport();
    QString getDBuser();
    QString getDBpass();
    QString getDBname();

    QString getTransport();
    QString getTimer();
    QString getTimerUnits();
    bool getTimerMode();

    QString getGatewayHost();
    QString getGatewayPort();

    QString getServerHost();
    QString getServerPort();
    
    void setDBhost(QString rhs);
    void setDBport(QString rhs);
    void setDBuser(QString rhs);
    void setDBpass(QString rhs);
    void setDBname(QString rhs);

    void setTransport(QString rhs);
    void setTimer(QString rhs);
    void setTimerUnits(QString rhs);
    void setTimerMode(bool rhs);

    void setGatewayHost(QString rhs);
    void setGatewayPort(QString rhs);

    void setServerHost(QString rhs);
    void setServerPort(QString rhs);

private:
    Ui::OptionsDialog *ui;
};

#endif // OPTIONSDIALOG_H
