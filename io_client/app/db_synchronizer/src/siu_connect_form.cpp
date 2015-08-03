#include <QtGui>

#include "ui_siu_connect_form.h"
#include "siu_connect_form.h"


SIUConnectForm::SIUConnectForm(QWidget *parent, Qt::WFlags f)
    : QDialog(parent, f),
    UI (new Ui::siu_connect_form)
{
    UI->setupUi (this);

	connect(UI->pbOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(UI->pbCancel, SIGNAL(clicked()), this, SLOT(reject()));

}

QString SIUConnectForm::getIP() const
{
    return UI->leIP->text();
}
QString SIUConnectForm::getPort() const
{
    return UI->lePort->text();
}
QString SIUConnectForm::getUser() const
{
    return UI->leUser->text();
}
QString SIUConnectForm::getPasswd() const
{
    return UI->lePassword->text();
}
QString SIUConnectForm::getSOP() const
{
    return UI->leSOP->text();
}

QString SIUConnectForm::getConstraint() const
{
    return UI->leConstraint->text();
}
