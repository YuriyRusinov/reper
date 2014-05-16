#ifndef _RADIO_IMAGE_PLUGIN_H
#define _RADIO_IMAGE_PLUGIN_H

#include <QObject>

#include <kksbaseplugin.h>
#include <kksioplugin.h>
#include <kkssito.h>

class QAction;

class KKSObject;

class RadioImagePlugin : public QObject,
                         public KKSBasePlugin,
                         public KKSIOPlugin
{
public:
    RadioImagePlugin (void);
    ~RadioImagePlugin (void);

    QString getName() const {return tr("Radio Image Plugin");}
    QString getDesc() const {return tr("Radio Image Plugin");}
    QPixmap getPixmap() const {return QPixmap(":/pics/radio_antenna.png");}

    void setAction( QAction * _action);
    QAction * getAction() { return action; }

    void setFactoryManager(KKSSito * fManager);
    void setIO(KKSObject * io);
    KKSIOPluginType getIOPluginType();

public slots:
    void runPlug (void);

private:
    //
    // Variables
    //
    KKSSito *m_fManager;
    KKSObject * m_io;

private:
    Q_OBJECT
    Q_INTERFACES(KKSBasePlugin)
    Q_INTERFACES(KKSIOPlugin)
};

#endif
