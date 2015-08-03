#include <QtGui>
#include <QMessageBox>

#include <KKSObject.h>
#include <KKSObjectExemplar.h>

#include "sendasxmlplugin.h"
#include "sendasxmlform.h"

SendAsXMLPlugin::SendAsXMLPlugin(): m_fManager (0)
{
}

SendAsXMLPlugin::~SendAsXMLPlugin()
{

}

void SendAsXMLPlugin::setAction( QAction * _act )
{
    if(_act)
    {
        action = _act;
        connect (action, SIGNAL (triggered()), this, SLOT (openForm()));
    }
}

void SendAsXMLPlugin::openForm()
{
    KKSObject * io = NULL;
    KKSObjectExemplar * eio = NULL;
    emit getCurrentIO(&io);
    if(!io){
        emit getCurrentEIO(&eio);
        if(!eio)
            return;
    }

    SendAsXMLForm * m_form = new SendAsXMLForm();
    if(io)
        m_form->setIO(io);
    if(eio)
        m_form->setEIO(eio);
    
    m_form->exec();
    
    delete m_form;
}


void SendAsXMLPlugin::setIO(KKSObject * io)
{
    Q_UNUSED(io);
}

void SendAsXMLPlugin::setEIO(KKSObjectExemplar * eio)
{
    Q_UNUSED(eio);
}


void SendAsXMLPlugin :: setFactoryManager(KKSCoreApplication * fManager)
{
    m_fManager = fManager;
}


KKSIOPlugin::KKSIOPluginType SendAsXMLPlugin::getIOPluginType()
{
    return KKSIOPlugin::iotIO;
}

Q_EXPORT_PLUGIN(SendAsXMLPlugin)
