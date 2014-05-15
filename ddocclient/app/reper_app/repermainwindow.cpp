#include <QMdiArea>

#include "repermainwindow.h"
#include "ui_reper_main_window.h"

ReperMainWindow :: ReperMainWindow (QWidget * parent, Qt::WindowFlags flags)
    : QMainWindow (parent, flags),
    UI (new Ui::ReperMainWindowForm),
    m_mdiArea (new QMdiArea)
{
    UI->setupUi (this);
    UI->setCentralWidget (m_mdiArea);

    connect (UI->actConnect, SIGNAL (triggered()), this, SLOT (slotConnect()) );
    connect (UI->actDisconnect, SIGNAL (triggered()), this, SLOT (slotDisconnect()) );
    connect (UI->act3D_Models, SIGNAL (triggered()), this, SLOT (slot3DMod()) );
    connect (UI->actRLI, SIGNAL (triggered()), this, SLOT (slotRLI()) );
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
}

void ReperMainWindow :: slotDisconnect (void)
{
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

