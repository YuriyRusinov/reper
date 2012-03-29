#ifndef _CONNINFO_FORM_H
#define _CONNINFO_FORM_H

#include <QDialog>
#include <QString>

#include "ui_connection_info_form.h"


class ConnectionInfoForm : public QDialog, private Ui::connection_info_form
{
    public:
        ConnectionInfoForm (QWidget * parent=NULL);
        ~ConnectionInfoForm ( );
        
        void setDatabaseInfo(const QString & host, const QString & port, const QString & db);
        void setUserInfo(const QString & user, const QString & position, const QString & accLevel);

    public slots:
        void pbOKClicked();


    private:

        Q_OBJECT
};

#endif
