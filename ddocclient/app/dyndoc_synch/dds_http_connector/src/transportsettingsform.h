#ifndef _TRANSPORT_SETTINGS_FORM_H
#define _TRANSPORT_SETTINGS_FORM_H

#include "httpwindow.h"

#include <QDialog>
#include <QHostInfo>
class QSettings;
class QHostInfo;

namespace Ui
{
    class transport_settings_form;
};

class dds_TransportSettingsForm : public QDialog
{
public:
    dds_TransportSettingsForm (dyndoc_HTTPconnector::HTTPsettings& settings, QWidget *parent=0, Qt::WFlags f=0);
    virtual ~dds_TransportSettingsForm (void);

    dyndoc_HTTPconnector::HTTPsettings* getSettings (void) const;

private slots:
    void setConnectionSettings (void);

private:
    void init (void);

private:
    Ui::transport_settings_form *UI;
    QHostAddress serverIp;

    dyndoc_HTTPconnector::HTTPsettings* Csettings;
private:
    Q_OBJECT
};
#endif
