#include <QtCore>
#include <QtGui>

#include "ui_connection_info_form.h"
#include "connectioninfoform.h"


ConnectionInfoForm :: ConnectionInfoForm (QWidget * parent)
    : QDialog (parent),
    UI (new Ui::connection_info_form)
{
    UI->setupUi (this);

    connect (UI->pbOK, SIGNAL (clicked ()), this, SLOT (pbOKClicked ()) );
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
    UI->leUser->setText(user);
    UI->lePosition->setText(position);
    UI->leAccLevel->setText(accLevel);
}

void ConnectionInfoForm :: setDatabaseInfo(const QString & host, const QString & port, const QString & db) 
{
    UI->leDBHost->setText(host);
    UI->leDBPort->setText(port);
    UI->leDBName->setText(db);
}

