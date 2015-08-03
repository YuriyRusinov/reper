#ifndef EDSPLUGIN_H
#define EDSPLUGIN_H

#include <QObject>

#include <kksbaseplugin.h>
#include <kksioplugin.h>
#include <kkscoreapplication.h>

#include "digitalsignatureform.h"

class QAction;
class KKSObject;

class EDSPlugin : public QObject,
                  public KKSBasePlugin,
                  public KKSIOPlugin
{    
    Q_OBJECT
    Q_INTERFACES(KKSBasePlugin)
    Q_INTERFACES(KKSIOPlugin)

    public:
        EDSPlugin();
        ~EDSPlugin();
          
        QString getName() const {return tr("Digital Signature Plugin");}
        QString getDesc() const {return tr("Digital Signature Plugin");}
        QPixmap getPixmap() const {return QPixmap(":/pics/eds.xpm");}
          
        void setAction( QAction * _action);
        QAction * getAction() { return action; }

        void setIO(KKSObject * io);
        void setEIO(KKSObjectExemplar * eio);

        KKSIOPluginType getIOPluginType();
        void setFactoryManager(KKSCoreApplication* fManager);
  
    public slots:
        void loadForm();

    signals:
        void getCurrentIO(KKSObject ** io);
        void getCurrentEIO(KKSObjectExemplar ** eio);
        void ioChanged(KKSObject * io);
        void eioChanged(KKSObjectExemplar * io);

private:
        KKSCoreApplication *m_fManager;
        
        DigitalSignatureForm *m_form;
};

#endif
