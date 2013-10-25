#include <QIntValidator>
#include <QSettings>

#include "ui_transport_settings_form.h"
#include "transportsettingsform.h"

dds_TransportSettingsForm :: dds_TransportSettingsForm (dyndoc_HTTPconnector::HTTPsettings& settings, QWidget *parent, Qt::WFlags f)
    : QDialog (parent, f),
    UI (new Ui::transport_settings_form)
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

    Csettings = &settings;

    this->init ();

    connect (UI->pbQuit, SIGNAL (clicked()), this, SLOT (reject()) );
    connect (UI->pbStart, SIGNAL (clicked()), this, SLOT (setConnectionSettings()) );
}

dds_TransportSettingsForm :: ~dds_TransportSettingsForm (void)
{
    delete UI;
}

dyndoc_HTTPconnector::HTTPsettings* dds_TransportSettingsForm :: getSettings (void) const
{
    return Csettings;
}

void dds_TransportSettingsForm :: setConnectionSettings (void)
{
    Csettings->host = UI->lEHost->text();
    Csettings->dbName = UI->lEDBName->text();
    Csettings->user = UI->lEUserName->text();
    Csettings->password = UI->lEPassword->text();
    Csettings->port = UI->lEPort->text().toInt();

    Csettings->transport = UI->lEHttpTransport->text().toInt();

    Csettings->http_host = UI->lEHttpHost->text();
    Csettings->http_port = UI->lEHttpPort->text().toInt();
   

    Csettings->addr = serverIp.toString();
    Csettings->server_port = UI->lEServerPort->text().toInt();
    accept ();
}

void dds_TransportSettingsForm :: init (void)
{
    QSettings* settings = new QSettings (QCoreApplication::applicationDirPath ()+"/http.ini", QSettings::IniFormat);

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
  
	//Получение адреса локального хоста
	QList<QHostAddress> serverIpList = QHostInfo::fromName(QHostInfo::localHostName()).addresses();
	serverIp = serverIpList.first();
    UI->lEServerPort->setText (settings->value ("server_port").toString ());

    settings->endGroup ();

    delete settings;
}
