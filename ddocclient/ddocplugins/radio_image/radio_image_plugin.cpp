#include <QMessageBox>
#include <QAction>
#include <QtCore/qplugin.h> 

#include <KKSObject.h>

#include "radio_image_plugin.h"

RadioImagePlugin :: RadioImagePlugin (void) 
    : m_fManager (0),
    m_io (0)
{
}

RadioImagePlugin :: ~RadioImagePlugin (void)
{
}

void RadioImagePlugin :: setAction( QAction * _action)
{
    if(_action)
    {
        action = _action;
        connect (action, SIGNAL (triggered()), this, SLOT (runPlug()));
    }
}

void RadioImagePlugin :: runPlug (void)
{
    QMessageBox::information (0, tr("Radio image"), tr ("Radio image test"));
    return;
}

void RadioImagePlugin :: setFactoryManager(KKSSito * fManager)
{
    m_fManager = fManager;
}

void RadioImagePlugin :: setIO(KKSObject * io)
{
    if(m_io)
        m_io->release();

    m_io = io;

    if(m_io)
        m_io->addRef();
}

KKSIOPlugin::KKSIOPluginType RadioImagePlugin :: getIOPluginType()
{
    return KKSIOPlugin::iotFiles;
}

Q_EXPORT_PLUGIN(RadioImagePlugin)
