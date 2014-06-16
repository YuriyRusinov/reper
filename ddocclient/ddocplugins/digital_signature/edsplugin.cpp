#include <QtGui>
#include <QMessageBox>

#include <KKSObject.h>

#include "edsplugin.h"
#include "digitalsignatureform.h"

EDSPlugin::EDSPlugin(): m_fManager (0)
{
}

EDSPlugin::~EDSPlugin()
{
}

void EDSPlugin::setAction( QAction * _act )
{
    if(_act)
    {
        action = _act;
        connect (action, SIGNAL (triggered()), this, SLOT (loadForm()));
    }
}

void EDSPlugin::loadForm()
{
    DigitalSignatureForm * m_form = new DigitalSignatureForm();
    m_form->exec();
    delete m_form;
}


void EDSPlugin::setIO(KKSObject * io)
{
    Q_UNUSED(io);
}

void EDSPlugin::setEIO(KKSObjectExemplar * eio)
{
    Q_UNUSED(eio);
}

void EDSPlugin :: setFactoryManager(KKSCoreApplication * fManager)
{
    m_fManager = fManager;
}


KKSIOPlugin::KKSIOPluginType EDSPlugin::getIOPluginType()
{
    return KKSIOPlugin::iotFiles;
}

Q_EXPORT_PLUGIN(EDSPlugin)
