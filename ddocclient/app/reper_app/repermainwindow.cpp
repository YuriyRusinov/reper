#include <QMdiArea>

#include "kkssito.h"
#include <kksdatabase.h>
#include <kkspluginloader.h>
#include <kksbaseplugin.h>
#include <radio_image_plugin.h>
#include "repermainwindow.h"
#include "ui_reper_main_window.h"

ReperMainWindow :: ReperMainWindow (QWidget * parent, Qt::WindowFlags flags)
    : QMainWindow (parent, flags),
    UI (new Ui::ReperMainWindowForm),
    m_mdiArea (new QMdiArea)
{
    UI->setupUi (this);
    this->setCentralWidget (m_mdiArea);

    KKSPluginLoader * pLoader = kksSito->pluginLoader();
    QList<QObject*> * plugins = pLoader->getPlugins();
    if (plugins)
    {
        QMenu * plugMenu = new QMenu;
        int nPlug = plugins->count();
        //bool isRadIm = false;
        for (int i=0; i<nPlug ; i++)
        {
            QObject * plug = plugins->at(i);
            QAction * aPlug = new QAction (plug);
            KKSBasePlugin * kksBaseP = qobject_cast<KKSBasePlugin *>(plug);
            aPlug->setText (kksBaseP->getName());
            aPlug->setIcon (QIcon (kksBaseP->getPixmap()));
            plugMenu->addAction (aPlug);
            kksBaseP->setAction (aPlug);
            if (qobject_cast<RadioImagePlugin *>(plug))
            {
                //isRadIm = true;
                RadioImagePlugin * rImPlug = qobject_cast<RadioImagePlugin *>(plug);
                rImPlug->setFactoryManager (kksSito);
                //rImPlug->setAction (UI->actRLI);
            }

        }
        UI->actPlugins->setMenu (plugMenu);
    }

    connect (UI->actConnect, SIGNAL (triggered()), this, SLOT (slotConnect()) );
    connect (UI->actDisconnect, SIGNAL (triggered()), this, SLOT (slotDisconnect()) );
    connect (UI->act3D_Models, SIGNAL (triggered()), this, SLOT (slot3DMod()) );
    //connect (UI->actRLI, SIGNAL (triggered()), this, SLOT (slotRLI()) );
    connect (UI->actE_xit, SIGNAL (triggered()), this, SLOT (slotClose()) );
    connect (UI->actSearch_region, SIGNAL (triggered()), this, SLOT (slotSearch()) );
    connect (UI->actComparison, SIGNAL (triggered()), this, SLOT (slotCompare()) );
}

ReperMainWindow :: ~ReperMainWindow (void)
{
    delete m_mdiArea;
    delete UI;
}

void ReperMainWindow :: slotConnect (void)
{
    int res = kksSito->GUIConnect(this);
}

void ReperMainWindow :: slotDisconnect (void)
{
    kksSito->db()->disconnect();
}

void ReperMainWindow :: slot3DMod (void)
{
}

void ReperMainWindow :: slotRLI (void)
{
}

void ReperMainWindow :: slotClose (void)
{
    close();
}

void ReperMainWindow :: slotSearch (void)
{
}

void ReperMainWindow :: slotCompare (void)
{
}

