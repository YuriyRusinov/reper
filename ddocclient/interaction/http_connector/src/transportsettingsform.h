#ifndef _TRANSPORT_SETTINGS_FORM_H
#define _TRANSPORT_SETTINGS_FORM_H

#include <QDialog>
#include <QHostInfo>
class QSettings;
class QHostInfo;

namespace Ui
{
    class transport_settings_form;
};

class TransportSettingsForm : public QDialog
{
public:
    TransportSettingsForm (QSettings *s, QWidget *parent=0, Qt::WFlags f=0);
    virtual ~TransportSettingsForm (void);

    QSettings * getSettings (void) const;

private slots:
    void setConnectionSettings (void);

private:
    //
    // Functions
    //
    void init (void);

private:
    //
    // Variables
    //
    Ui::transport_settings_form *UI;
    QSettings * settings;
	QHostAddress   serverIp;//IP адрес сервера

private:
    Q_OBJECT
};
#endif
