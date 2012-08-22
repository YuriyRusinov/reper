#ifndef SIU_CONNECT_FORM_H
#define SIU_CONNECT_FORM_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;


namespace Ui
{
    class siu_connect_form;
};


class SIUConnectForm : public QDialog
{
    Q_OBJECT

public:
    SIUConnectForm(QWidget *parent=0, Qt::WFlags f=0);

    QString getIP() const;
    QString getPort() const;
    QString getUser() const;
    QString getPasswd() const;
    QString getSOP() const;
	QString getConstraint() const;

private:
	Ui::siu_connect_form *UI;
};

#endif
