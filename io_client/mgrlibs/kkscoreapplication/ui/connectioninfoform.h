#ifndef _CONNINFO_FORM_H
#define _CONNINFO_FORM_H

#include <QDialog>
#include <QString>

namespace Ui
{
    class connection_info_form;
};

class ConnectionInfoForm : public QDialog//, private Ui::connection_info_form
{
public:
    ConnectionInfoForm (QWidget * parent=NULL);
    virtual ~ConnectionInfoForm ( );
    
    void setDatabaseInfo(const QString & host, const QString & port, const QString & db);
    void setUserInfo(const QString & user, const QString & position, const QString & accLevel);

public slots:
    void pbOKClicked();

private:
    //
    // Variables
    //
    Ui::connection_info_form * UI;
private:

    Q_OBJECT
};

#endif
