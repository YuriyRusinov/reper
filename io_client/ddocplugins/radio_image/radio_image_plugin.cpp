#include <QMessageBox>
#include <QAction>
#include <QtPlugin>

#include <KKSObject.h>
#include <KKSObjectExemplar.h>
#include <kksdatabase.h>

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
    if (!m_fManager)
    {
        QMessageBox::information (0, tr("Radio image"), tr ("Radio image test"));
        return;
    }

    KKSDatabase * db = m_fManager->db();
    if (!db->connected())
    {
        int res=m_fManager->GUIConnect(0);
        if (res < 0)
        {
            QMessageBox::warning (0, tr("Connect to db"), tr ("Cannot connect to database"), QMessageBox::Ok);
            return;
        }
    }
    return;
}

void RadioImagePlugin :: setFactoryManager(KKSCoreApplication * fManager)
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

void RadioImagePlugin :: setEIO(KKSObjectExemplar * rio)
{
    Q_UNUSED (rio);
}

KKSIOPlugin::KKSIOPluginType RadioImagePlugin :: getIOPluginType()
{
    return KKSIOPlugin::iotFiles;
}

Q_EXPORT_PLUGIN(RadioImagePlugin)
