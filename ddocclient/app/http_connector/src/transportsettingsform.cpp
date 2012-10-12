#include <QIntValidator>
#include <QSettings>

#include "ui_transport_settings_form.h"
#include "transportsettingsform.h"

TransportSettingsForm :: TransportSettingsForm (QSettings *s, QWidget *parent, Qt::WFlags f)
    : QDialog (parent, f),
    UI (new Ui::transport_settings_form),
    settings (s)
{
    UI->setupUi (this);

    QIntValidator *pDbVal = new QIntValidator (0, 100000, this);
    UI->lEPort->setValidator (pDbVal);
    QIntValidator *pHttpVal = new QIntValidator (0, 100000, this);
    UI->lEHttpPort->setValidator (pHttpVal);
    QIntValidator *pHttpTVal = new QIntValidator (1, 100000, this);
    UI->lEHttpTransport->setValidator (pHttpTVal);
    QIntValidator *pServerVal = new QIntValidator (0, 100000, this);
    UI->lEServerPort->setValidator (pServerVal);

    this->init ();

    connect (UI->pbQuit, SIGNAL (clicked()), this, SLOT (reject()) );
    connect (UI->pbStart, SIGNAL (clicked()), this, SLOT (setConnectionSettings()) );
}

TransportSettingsForm :: ~TransportSettingsForm (void)
{
    delete UI;
}

QSettings * TransportSettingsForm :: getSettings (void) const
{
    return settings;
}

void TransportSettingsForm :: setConnectionSettings (void)
{
    if (!settings)
    {
        reject ();
        return;
    }
    settings->beginGroup ("Database");
    settings->setValue ("host", UI->lEHost->text ());
    settings->setValue ("database", UI->lEDBName->text ());
    settings->setValue ("user", UI->lEUserName->text ());
    settings->setValue ("password", UI->lEPassword->text ());
    settings->setValue ("port", UI->lEPort->text ());
    settings->endGroup ();

    settings->beginGroup("Transport");
    settings->setValue ("transport", UI->lEHttpTransport->text ());
    settings->endGroup();


    settings->beginGroup ("Http");
    settings->setValue ("host", UI->lEHttpHost->text ());
    settings->setValue ("port", UI->lEHttpPort->text ());
   

    settings->setValue ("server_host", serverIp.toString());
    settings->setValue ("server_port", UI->lEServerPort->text ());
    settings->endGroup ();
    accept ();
}

void TransportSettingsForm :: init (void)
{
    settings->beginGroup ("Database");
    UI->lEHost->setText (settings->value ("host").toString());

    UI->lEHost->setToolTip(tr("Host where dabatabase runs"));
    UI->lEDBName->setToolTip(tr("Database name"));
    UI->lEUserName->setToolTip(tr("User database name"));
    UI->lEPassword->setToolTip(tr("Password user database"));
    UI->lEPort->setToolTip(tr("Port database"));

    UI->lEHttpHost->setToolTip(tr("Host where client must connect"));
    UI->lEHttpPort->setToolTip(tr("Port where client must connect"));
    
    UI->lEHttpTransport->setToolTip(tr("Identificator transport in system (for tcp/ip protocol has to be 1, for http-protocol has to be 2)"));

//    UI->lEServerHost->setToolTip(tr("Server where other client must connect"));
    UI->lEServerPort->setToolTip(tr("Server port number"));

    UI->lEDBName->setText (settings->value ("database").toString());
    UI->lEUserName->setText (settings->value ("user").toString());
    UI->lEPassword->setText (settings->value ("password").toString());
    UI->lEPort->setText (settings->value ("port").toString());
    settings->endGroup ();

    settings->beginGroup("Transport");
    UI->lEHttpTransport->setText (settings->value ("transport").toString ());
    settings->endGroup ();

    settings->beginGroup ("Http");
    UI->lEHttpHost->setText (settings->value ("host").toString());
    UI->lEHttpPort->setText (settings->value ("port").toString());
  
    //, UI->lEAddress->text ());

	//Получение адреса локального хоста
	QList<QHostAddress> serverIpList = QHostInfo::fromName(QHostInfo::localHostName()).addresses();
	serverIp = serverIpList.first();

	//QString string;
	//string = serverIp.toString();

    //UI->lEServerHost->setText (settings->value ("server_host").toString ());//, UI->lEServerHost->text ());
    UI->lEServerPort->setText (settings->value ("server_port").toString ());
    //, UI->lEServerPort->text ());
    settings->endGroup ();
}
