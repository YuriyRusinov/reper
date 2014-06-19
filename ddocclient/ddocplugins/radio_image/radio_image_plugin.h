#ifndef _RADIO_IMAGE_PLUGIN_H
#define _RADIO_IMAGE_PLUGIN_H

#include <QObject>

#include <kksbaseplugin.h>
#include <kksioplugin.h>
#include <kkscoreapplication.h>

class QAction;

class KKSObject;
class KKSObjectExemplar;

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

    void setFactoryManager(KKSCoreApplication* fManager);
    void setIO(KKSObject * io);
    void setEIO(KKSObjectExemplar * rio);
    KKSIOPluginType getIOPluginType();

public slots:
    void runPlug (void);

signals:

    void getCurrentIO(KKSObject ** io);
    void getCurrentEIO(KKSObjectExemplar ** eio);
    void ioChanged(KKSObject * io);
    void eioChanged(KKSObjectExemplar * io);

private:
    //
    // Variables
    //
    KKSCoreApplication *m_fManager;
    KKSObject * m_io;

private:
    Q_OBJECT
    Q_INTERFACES(KKSBasePlugin)
    Q_INTERFACES(KKSIOPlugin)
};

#endif
