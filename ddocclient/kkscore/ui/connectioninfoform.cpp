#include <QtCore>
#include <QtGui>

#include "connectioninfoform.h"


ConnectionInfoForm :: ConnectionInfoForm (QWidget * parent)
    : QDialog (parent)
{
    setupUi (this);

    connect (pbOK, SIGNAL (clicked ()), this, SLOT (pbOKClicked ()) );
}

ConnectionInfoForm :: ~ConnectionInfoForm ( )
{
}

void ConnectionInfoForm :: pbOKClicked ( void )
{
    accept ();
}


void ConnectionInfoForm :: setUserInfo(const QString & user, const QString & position, const QString & accLevel) 
{
    leUser->setText(user);
    lePosition->setText(position);
    leAccLevel->setText(accLevel);
}

void ConnectionInfoForm :: setDatabaseInfo(const QString & host, const QString & port, const QString & db) 
{
    leDBHost->setText(host);
    leDBPort->setText(port);
    leDBName->setText(db);
}

