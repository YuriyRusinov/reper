#include <QtGui>
#include <QMessageBox>

#include <KKSObject.h>

#include "edsplugin.h"
#include "digitalsignatureform.h"

EDSPlugin::EDSPlugin(): initplugin (false), m_fManager (0)
{
    m_form = new DigitalSignatureForm();
    m_io = NULL;
}

EDSPlugin::~EDSPlugin()
{
    if(m_form)
        delete m_form;
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
    if(!initplugin)
    {
        initplugin = true;
    }

    m_form->exec();
}


void EDSPlugin::setIO(KKSObject * io)
{
    if(m_io)
        m_io->release();

    m_io = io;

    if(m_io)
        m_io->addRef();
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
