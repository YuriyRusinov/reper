#ifndef SENDASXMLPLUGIN_H
#define SENDASXMLPLUGIN_H

#include <QObject>

#include <kksbaseplugin.h>
#include <kksioplugin.h>
#include <kkscoreapplication.h>

#include "sendasxmlform.h"

class QAction;
class KKSObject;
class KKSObjectExemplar;

class SendAsXMLPlugin : public QObject,
                        public KKSBasePlugin,
                        public KKSIOPlugin
{    
    Q_OBJECT
    Q_INTERFACES(KKSBasePlugin)
    Q_INTERFACES(KKSIOPlugin)

    public:
        SendAsXMLPlugin();
        ~SendAsXMLPlugin();
          
        QString getName() const {return tr("Send As XML Plugin");}
        QString getDesc() const {return tr("Send As XML Plugin");}
        QPixmap getPixmap() const {return QPixmap(":/pics/send_as_xml.xpm");}
          
        void setAction( QAction * _action);
        QAction * getAction() { return action; }

        void setIO(KKSObject * io);
        void setEIO(KKSObjectExemplar * eio);

        KKSIOPluginType getIOPluginType();
        void setFactoryManager(KKSCoreApplication* fManager);
  
    public slots:
        void openForm();

    signals:
        void getCurrentIO(KKSObject ** io);
        void getCurrentEIO(KKSObjectExemplar ** eio);
        void ioChanged(KKSObject * io);
        void eioChanged(KKSObjectExemplar * io);

    private:
        //KKSObject * m_io;
        KKSCoreApplication *m_fManager;
        
        //bool initplugin;

        //SendAsXMLForm *m_form;
};

#endif
