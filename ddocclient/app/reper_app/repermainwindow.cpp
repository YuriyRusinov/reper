#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QBuffer>

#include "kksapplication.h"
#include <kksdatabase.h>
#include <kkspluginloader.h>
#include <kksbaseplugin.h>
#include <KKSFilter.h>
#include <KKSLoader.h>
#include <KKSObjEditorFactory.h>
#include <KKSObjEditor.h>
#include <radio_image_plugin.h>
#include <defines.h>
#include "repermainwindow.h"
#include "searchradioform.h"
#include "ui_reper_main_window.h"

ReperMainWindow :: ReperMainWindow (QWidget * parent, Qt::WindowFlags flags)
    : QMainWindow (parent, flags),
    UI (new Ui::ReperMainWindowForm),
    m_mdiArea (new QMdiArea)
{
    UI->setupUi (this);
    this->setCentralWidget (m_mdiArea);
    setActionsEnabled (false);

    KKSPluginLoader * pLoader = kksCoreApp->pluginLoader();
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
            kksBaseP->setFactoryManager (kksCoreApp);
            /*
            if (qobject_cast<RadioImagePlugin *>(plug))
            {
                //isRadIm = true;
                RadioImagePlugin * rImPlug = qobject_cast<RadioImagePlugin *>(plug);
                rImPlug->setFactoryManager (kksCoreApp);
                //rImPlug->setAction (UI->actRLI);
            }
            */

        }
        UI->actPlugins->setMenu (plugMenu);
    }
    KKSObjEditorFactory * oef = kksApp->oef();
    connect (oef, 
             SIGNAL(editorCreated(KKSObjEditor *)), 
             this, 
             SLOT(slotCreateNewObjEditor(KKSObjEditor*)));

    connect (UI->actConnect, SIGNAL (triggered()), this, SLOT (slotConnect()) );
    connect (UI->actDisconnect, SIGNAL (triggered()), this, SLOT (slotDisconnect()) );
    connect (UI->act3D_Models, SIGNAL (triggered()), this, SLOT (slot3DMod()) );
    connect (UI->actRLI, SIGNAL (triggered()), this, SLOT (slotRLI()) );
    connect (UI->actE_xit, SIGNAL (triggered()), this, SLOT (slotClose()) );
    connect (UI->actSearchImage, SIGNAL (triggered()), this, SLOT (slotSearchByImage()) );
    connect (UI->actComparison, SIGNAL (triggered()), this, SLOT (slotCompare()) );
}

ReperMainWindow :: ~ReperMainWindow (void)
{
    delete m_mdiArea;
    delete UI;
}

void ReperMainWindow :: slotConnect (void)
{
    int res = kksCoreApp->GUIConnect(this);
    if(res == ERROR_CODE){
        setActionsEnabled(false);
    }
    else{
        setActionsEnabled(true);
//        m_reportHandler->dbConnect(kksApp->db()->getName(), 
//                                   kksApp->db()->getHost(), 
//                                   kksApp->db()->getPort(), 
//                                   kksApp->db()->getUser(), 
//                                   kksApp->db()->getPass());
    }
}

void ReperMainWindow :: slotDisconnect (void)
{
    kksCoreApp->db()->disconnect();
}

void ReperMainWindow :: slot3DMod (void)
{
}

void ReperMainWindow :: slotRLI (void)
{
    KKSList<const KKSFilter*> filters;
    KKSObject * o = kksApp->loader()->loadIO(IO_IO_ID, true);
    if(!o || !o->category())
        return;
    KKSCategory * c = o->category();//->tableCategory();
    if(!c)
        return;
    if (!c->tableCategory())
        return;

    c = c->tableCategory();

    KKSObjEditorFactory * oef = kksApp->oef();

    int idUser = kksApp->loader()->getUserId();
    KKSFilter * filter = c->createFilter(ATTR_AUTHOR, QString::number(idUser), KKSFilter::foEq);
    if(!filter)
        return;

    filters.append(filter);
    filter->release();
    KKSList<const KKSFilterGroup *> filterGroups;
    KKSFilterGroup * group = new KKSFilterGroup(true);
    group->setFilters(filters);
    filterGroups.append(group);
    group->release();
    
    KKSObjEditor *objEditor = oef->createObjEditor(IO_IO_ID, 
                                                   IO_IO_ID, 
                                                   filterGroups, 
                                                   "",
                                                   o->category(),
                                                   false,
                                                   QString(),
                                                   false,
                                                   Qt::NonModal,
                                                   0);
    slotCreateNewObjEditor(objEditor);
    
    o->release();
}

void ReperMainWindow :: slotClose (void)
{
    close();
}

void ReperMainWindow :: slotSearchByImage (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    SearchRadioForm * srForm = new SearchRadioForm();
    QImage sIm ;
    if (srForm->exec() == QDialog::Accepted)
    {
        sIm = srForm->getImage();
        if (sIm.isNull())
        {
            delete srForm;
            return;
        }
    }
    else
    {
        delete srForm;
        return;
    }
    KKSLoader * loader = kksApp->loader();
    QString tName = "rli_image_raws";
    KKSObject * io = loader->loadIO (tName, true);
    if (!io)
    {
        QMessageBox::warning (0, tr("Select reference"),
                                 tr ("Not available suitable reference"),
                                 QMessageBox::Ok);
        delete srForm;
        return;
    }
    KKSObjEditorFactory * oef = kksApp->oef();
    KKSList<const KKSFilterGroup *> filterGroups;
    const KKSCategory * c = io->category();
    const KKSCategory * ct = c->tableCategory();
    QByteArray bytes;
    QBuffer buffer(&bytes);

    buffer.open(QIODevice::WriteOnly);
    sIm.save (&buffer, "XPM");
    buffer.close ();
    KKSCategoryAttr * a = 0;//loader->loadAttribute ("image_jpg"
    for (KKSMap<int, KKSCategoryAttr *>::const_iterator p = ct->attributes().constBegin();
            p != ct->attributes().constEnd() && a == 0;
            ++p)
        if (QString::compare (p.value()->code(), QString("image_jpg"), Qt::CaseInsensitive) == 0)
                a = p.value();

    KKSFilter * filter = ct->createFilter (a->id(), bytes, KKSFilter::foEq); 
    KKSList<const KKSFilter*> filters;
    filters.append (filter);
    filter->release ();
    KKSFilterGroup * group = new KKSFilterGroup(true);
    group->setFilters(filters);
    filterGroups.append(group);
    group->release();
    KKSObjEditor *objEditor = oef->createObjEditor(IO_IO_ID, 
                                                   io->id(), 
                                                   filterGroups, 
                                                   "",
                                                   c,
                                                   false,
                                                   QString(),
                                                   false,
                                                   Qt::NonModal,
                                                   0);
    io->release ();
    slotCreateNewObjEditor(objEditor);

    delete srForm;
}

void ReperMainWindow :: slotCompare (void)
{
}

void ReperMainWindow::slotCreateNewObjEditor(KKSObjEditor * objEditor)
{
    if(!objEditor)
        return;

    if (objEditor->getObj() && objEditor->getObj()->id() == IO_IO_ID)
    {
        QTabWidget * tObj = objEditor->getTabWidget();
        for (int i=0; i<3; i++)
        {
            //QWidget * w = tObj->widget (0);
            tObj->removeTab(0);//setTabEnabled (i, false);
            //w->setParent (0);
            //delete w;
            //QWidget * w = tObj->widget (i);
            //w->setEnabled (false);//setVisible (false);
        }
        //QWidget * w = tObj->widget (1);
        tObj->removeTab (1);//setTabEnabled (4, false);
        //w->setParent (0);
        //delete w;
    }
    QMdiSubWindow * m_objEditorW = m_mdiArea->addSubWindow (objEditor);
    m_objEditorW->setAttribute (Qt::WA_DeleteOnClose);
    connect (objEditor, SIGNAL (closeEditor()), m_objEditorW, SLOT (close()) );
    
    //помечаем, что редактор ИО является MDI-окном. В этом случае при определении активного окна необходимо будет слать сигнал isActiveSubWindow()
    objEditor->setAsSubWindow(true);
    connect(objEditor, SIGNAL(isActiveSubWindow(const KKSObjEditor *, bool *)), this, SLOT(isActiveSubWindow(const KKSObjEditor *, bool *)));


    objEditor->setWindowState (objEditor->windowState() | Qt::WindowMaximized | Qt::WindowActive);
    m_objEditorW->setWindowState (m_objEditorW->windowState() | Qt::WindowMaximized | Qt::WindowActive);
    objEditor->show();
    objEditor->setNumCopies (1);//m_masscreateW->num());

//    connect (this->m_masscreateW, SIGNAL (setNum(int)), objEditor, SLOT (setNumCopies (int)) );
}

void ReperMainWindow::setActionsEnabled(bool enabled)
{
    UI->actRLI->setEnabled (enabled);
    UI->menu_Search->setEnabled (enabled);
    UI->actSearchImage->setEnabled (enabled);
}

void ReperMainWindow::isActiveSubWindow(const KKSObjEditor * editor, bool * yes)
{
    if(!editor)
        return;
    if(!yes)
        return;

    QWidget * w = activeKKSSubWindow();
    if(!w)
        return;

    if(w == editor){
        *yes = true;
        return;
    }

    *yes = false;
}

QWidget * ReperMainWindow::activeKKSSubWindow()
{
    if (QMdiSubWindow *activeSubWindow = m_mdiArea->activeSubWindow())
        return qobject_cast<QWidget *>(activeSubWindow->widget());
    return 0;
}

