#include <QtGui>
#include <QMessageBox>

#include <kksobject.h>

#include "edsplugin.h"
#include "digitalsignatureform.h"

EDSPlugin::EDSPlugin(): initplugin (false)
{
    m_form = new DigitalSignatureForm();
    m_io = NULL;
    m_fManager = NULL;
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

void EDSPlugin::setFactoryManager(KKSSito * fManager)
{
    m_fManager = fManager;
}

void EDSPlugin::setIO(KKSObject * io)
{
    if(m_io)
        m_io->release();

    m_io = io;

    if(m_io)
        m_io->addRef();
}

KKSIOPlugin::KKSIOPluginType EDSPlugin::getIOPluginType()
{
    return KKSIOPlugin::iotFiles;
}

Q_EXPORT_PLUGIN(EDSPlugin)
