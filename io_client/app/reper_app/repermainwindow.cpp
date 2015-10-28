#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QBuffer>
#include <QFileDialog>
#include <QDir>
#include <QVector>
#include <QProgressBar>
#include <QProgressDialog>
#include <QTranslator>
#include <QToolBar>
#include <QtDebug>

#include "assistant.h"
#include <kkssettings.h>
#include "kksapplication.h"
#include <kksdatabase.h>
#include <kkspluginloader.h>
#include <kksbaseplugin.h>
#include <KKSFilter.h>
#include <KKSLoader.h>
#include <KKSObjEditorFactory.h>
#include <KKSEIOFactory.h>
#include <KKSObjEditor.h>
#include <KKSObject.h>
#include <KKSObjectExemplar.h>
#include <KKSRecWidget.h>
#include <KKSAttribute.h>
#include <radio_image_plugin.h>
#include <defines.h>
#include "repermainwindow.h"
#include "searchradioform.h"
#include "searchradioimagefragmentform.h"
#include "searchradioimagecalc.h"
#include "objloader.h"
#include "gologramma.h"
#include "imagewidget.h"
#include "searchresultsform.h"
#include "radMdiArea.h"
#include <window.h>
#include "ui_reper_main_window.h"

using mslLoader::OBJloader;

const int id_attr_type_ship = 1001;

ReperMainWindow :: ReperMainWindow (QWidget * parent, Qt::WindowFlags flags)
    : QMainWindow (parent, flags),
    UI (new Ui::ReperMainWindowForm),
    m_mdiArea (new RadMdiArea(QImage(":/ddoc/m31.jpg"), tr("Software Reper"))),
    ass (new Assistant),
    tbActions (new QToolBar (this)),
    tbCalc (new QToolBar (this)),
    tbOthers (new QToolBar (this))
{
    UI->setupUi (this);
    this->setCentralWidget (m_mdiArea);
    tbActions->setIconSize (QSize(32, 32));
    tbCalc->setIconSize (QSize(32, 32));
    tbOthers->setIconSize (QSize(32, 32));
    setActionsEnabled (false);
    this->initToolBars ();

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
//    connect (UI->act3D_Models, SIGNAL (triggered()), this, SLOT (slot3DMod()) );
    connect (UI->actRLI, SIGNAL (triggered()), this, SLOT (slotRLI()) );
    connect (UI->actE_xit, SIGNAL (triggered()), this, SLOT (slotClose()) );
    connect (UI->actSearchImage, SIGNAL (triggered()), this, SLOT (slotSearchByImage()) );
    connect (UI->actComparison, SIGNAL (triggered()), this, SLOT (slotCompare()) );
    connect (UI->actGenerateGol, SIGNAL (triggered()), this, SLOT (slotGologram()) );
    connect (UI->actBy_Image_fragment, SIGNAL (triggered()), this, SLOT (slotViewImage()) );
    connect (UI->actUser_Manual, SIGNAL (triggered()), this, SLOT (slotHelp()));
    connect (UI->actSettings, SIGNAL (triggered()), this, SLOT (slotSettings()) );
    connect (UI->actView3DMod, SIGNAL (triggered()), this, SLOT (slot3DView()) );
}

ReperMainWindow :: ~ReperMainWindow (void)
{
    delete ass;
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
    kksCoreApp->db()->disconnect(false);
    setActionsEnabled(false);
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

    KKSFilter * fUserFilter = c->createFilter (1, QString::number (300), KKSFilter::foGr);

    filters.append(filter);
    filter->release();

    if (fUserFilter)
    {
        filters.append (fUserFilter);
        fUserFilter->release ();
    }
    QStringList tNames;
    tNames << QString ("type_ship") << QString ("radio_image") << QString("rli_image_raws") << QString("object_passports");
    KKSFilter * fTableFilter = c->createFilter (ATTR_TABLE_NAME, tNames, KKSFilter::foIn);
    if (fTableFilter)
    {
        filters.append (fTableFilter);
        fTableFilter->release ();
    }
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
    searchIm (QImage());
}

void ReperMainWindow :: searchIm (const QImage& sIm0)
{
    qDebug () << __PRETTY_FUNCTION__;
    SearchRadioImageCalc * imCalc = new SearchRadioImageCalc ();
    SearchRadioImageFragmentForm * srForm = imCalc->GUIImageView (sIm0);//new SearchRadioImageFragmentForm (sIm0);
    //srForm->setImage (sIm0);
    QImage sIm (sIm0);
    double az (-1.0);
    if (srForm->exec() == QDialog::Accepted)
    {
        sIm = srForm->getSourceImage();
        az = srForm->getAzimuth ();
        if (sIm.isNull())
        {
            delete imCalc;
            return;
        }
    }
    else
    {
        delete imCalc;//srForm;
        return;
    }
    KKSLoader * loader = kksApp->loader();
    QString tName = QString ("rli_image_raws");
    KKSObject * io = loader->loadIO (tName, true);
    if (!io)
    {
        QMessageBox::warning (0, tr("Select reference"),
                                 tr ("Not available suitable reference"),
                                 QMessageBox::Ok);
        delete imCalc;
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
    KKSCategoryAttr * aIm = 0;//loader->loadAttribute ("image_jpg"
    KKSCategoryAttr * aAz = 0;
    for (KKSMap<int, KKSCategoryAttr *>::const_iterator p = ct->attributes().constBegin();
            p != ct->attributes().constEnd() ;//&& aIm == 0 && aAz==0;
            ++p)
    {
        if (QString::compare (p.value()->code(), QString("image_jpg"), Qt::CaseInsensitive) == 0)
            aIm = p.value ();
        if (QString::compare (p.value()->code(), QString("azimuth"), Qt::CaseInsensitive) == 0)
            aAz = p.value ();
    }
    if (!aAz)
    {
        delete imCalc;
        return;
    }

    KKSFilter * filter = ct->createFilter (aIm->id(), bytes, KKSFilter::foEq);
    KKSFilter * fAzMin = ct->createFilter (aAz->id(), QString::number ((int)az-3), KKSFilter::foGrEq);
    KKSFilter * fAzMax = ct->createFilter (aAz->id(), QString::number ((int)az+3), KKSFilter::foLessEq);
    KKSList<const KKSFilter*> filters;
    filters.append (filter);
    filter->release ();
    KKSFilterGroup * group = new KKSFilterGroup(false);
    KKSFilterGroup * azGroup = new KKSFilterGroup (true);
    azGroup->addFilter (fAzMin);
    fAzMin->release ();
    azGroup->addFilter (fAzMax);
    fAzMax->release ();
    group->setFilters(filters);
    group->addGroup (azGroup);
    azGroup->release ();
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
    //slotCreateNewObjEditor(objEditor);
    SearchResultsForm *sresForm = new SearchResultsForm (sIm);
    QMdiSubWindow * m_ResW = m_mdiArea->addSubWindow (sresForm);
    m_ResW->setAttribute (Qt::WA_DeleteOnClose);
    sresForm->show ();
    KKSRecWidget * rw = objEditor->getRecordsWidget ();
    QAbstractItemModel * sMod = rw->getModel ();
    sresForm->setResultsModel (sMod);
    //connect (objEditor, SIGNAL (closeEditor()), m_objEditorW, SLOT (close()) );

    delete imCalc;//srForm;
}

void ReperMainWindow :: slotCompare (void)
{
}

void ReperMainWindow::slotCreateNewObjEditor(KKSObjEditor * objEditor)
{
    if(!objEditor)
        return;

/*    QTabWidget * tObj = objEditor->getTabWidget();
    qDebug () << __PRETTY_FUNCTION__ << objEditor->getObj()->id() << objEditor->getObjectEx()->id();
    if ((objEditor->getObj() && objEditor->getObj()->id() == IO_IO_ID) ||
        (objEditor->getObjectEx() && ((objEditor->getObjectEx()->id() == IO_IO_ID) || objEditor->getObjectEx()->io()->id() == IO_IO_ID )))
    {
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
    else if (tObj->count() > 1)
    {
        int n = tObj->count();
        for (int i=1; i<n; i++)
            tObj->removeTab (1);
    }
*/
    QMdiSubWindow * m_objEditorW = m_mdiArea->addSubWindow (objEditor);
    m_objEditorW->setAttribute (Qt::WA_DeleteOnClose);
    connect (objEditor, SIGNAL (closeEditor()), m_objEditorW, SLOT (close()) );
    //
    // помечаем, что редактор ИО является MDI-окном. В этом случае при определении активного окна необходимо будет слать сигнал isActiveSubWindow()
    //
    objEditor->setAsSubWindow(true);
    connect(objEditor, SIGNAL(isActiveSubWindow(const KKSObjEditor *, bool *)), this, SLOT(isActiveSubWindow(const KKSObjEditor *, bool *)));

    objEditor->setWindowState (objEditor->windowState() | Qt::WindowActive);
    m_objEditorW->setWindowState (m_objEditorW->windowState() | Qt::WindowActive);
    objEditor->show();
    objEditor->setNumCopies (1);//m_masscreateW->num());

//    connect (this->m_masscreateW, SIGNAL (setNum(int)), objEditor, SLOT (setNumCopies (int)) );
}

void ReperMainWindow::setActionsEnabled(bool enabled)
{
    UI->actRLI->setEnabled (enabled);
    UI->menu_Search->setEnabled (enabled);
    UI->actSearchImage->setEnabled (enabled);
//    UI->menuGologram->setEnabled (enabled);
    UI->actBy_Image_fragment->setEnabled (enabled);
    UI->actGenerateGol->setEnabled (enabled);
    UI->actView3DMod->setEnabled (enabled);
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

void ReperMainWindow::slotGologram (void)
{
    qDebug () << __PRETTY_FUNCTION__ ;
    //QTranslator golTr;
    //golTr.load ("./transl/gologram_ru.qm", ".");
    //QCoreApplication::installTranslator (&golTr);
    imageCreatorForm * icf = new imageCreatorForm (this);
    //
    // Загрузка из БД возможных типов кораблей
    //
    KKSLoader * loader = kksApp->loader ();
    KKSAttribute * a = loader->loadAttribute (id_attr_type_ship);//QString("id_type_ship"), QString("type_ship"));
    if (!a)
    {
        QMessageBox::warning (this, tr("Available ship types"), tr ("Cannot load ship types list"), QMessageBox::Ok);
        return;
    }
    //KKSObject * ioShipType = loader->loadIO (QString("type_ship"), true);
    QMap<int, QString> refColumnValues;
    QMap<int, QString> aVals = loader->loadAttributeValues (a, refColumnValues);
    icf->initShipTypes (aVals);
    //qDebug () << __PRETTY_FUNCTION__ << aVals << refColumnValues;
    a->release ();

    connect (icf, SIGNAL (imagesData(generatingDataPlus)), this, SLOT (slotGologramCalc(generatingDataPlus)) );
    icf->exec();
    qDebug () << __PRETTY_FUNCTION__ ;
}

void ReperMainWindow::slotGologramCalc (generatingDataPlus gdp)
{
    imageCreatorForm * iGW = qobject_cast<imageCreatorForm *>(this->sender());
    bool fTests (false);
    int type_ship (-1);
    if (iGW)
    {
        iGW->setVisible (false);
        fTests = iGW->forTests();
        type_ship = iGW->getShipType ();
    }
    qDebug () << __PRETTY_FUNCTION__ << type_ship;
    ImageGenerator* generator = new ImageGenerator(gdp,this);
    
    generator->loadModel();
    QVector<returningData> resD = generator->generateImages();
    delete generator;

    int nd = resD.count();
    qDebug () << __PRETTY_FUNCTION__ << nd;
//    KKSLoader * dbL = kksCoreApp->loader();
    KKSEIOFactory * eiof = kksCoreApp->eiof();
//    Q_UNUSED (eiof);
    KKSLoader * loader = kksApp->loader();
    QString tName = "rli_image_raws";
    KKSObject * io = loader->loadIO (tName, true);
    const KKSCategory * c = io->category();
    const KKSCategory * ct = c->tableCategory();
    KKSList<KKSAttrValue*> aVals;
    QString imDir = QString();
    if (fTests)
    {
        imDir = QFileDialog::getExistingDirectory (this, tr("Save test images"));
        if (imDir.isEmpty())
            return;
    }
    for (int i=0; i<nd; i++)
    {
        KKSObjectExemplar * cef = new KKSObjectExemplar (-1, tr("Record %1").arg (i), io);
//        int icol
        aVals.clear ();
        for (KKSMap<int, KKSCategoryAttr *>::const_iterator p = ct->attributes().constBegin();
                p != ct->attributes().constEnd();
                ++p)
        {
            KKSValue v;// = resD[i]
            if (QString::compare (p.value()->code(), QString("id_type_ship"), Qt::CaseInsensitive) == 0)
                v = KKSValue (QString::number (type_ship), KKSAttrType::atList);
            else if (QString::compare (p.value()->code(), QString("azimuth"), Qt::CaseInsensitive) == 0)
                v = KKSValue (QString :: number (resD[i].XY_angle), KKSAttrType::atDouble);
            else if (QString::compare (p.value()->code(), QString("elevation_angle"), Qt::CaseInsensitive) == 0)
                v = KKSValue (QString :: number (resD[i].XZ_angle), KKSAttrType::atDouble);
            else if (QString::compare (p.value()->code(), QString("depth"), Qt::CaseInsensitive) == 0)
                v = KKSValue (QString :: number (resD[i].rowNumber), KKSAttrType::atInt);
            else if (QString::compare (p.value()->code(), QString("image_raw"), Qt::CaseInsensitive) == 0)
            {
                QByteArray bData;
                bData.clear ();
                bData.append (QByteArray :: number (resD[i].XY_angle));
                bData.append (" ");
                bData.append (QByteArray :: number (resD[i].XZ_angle));
                bData.append (" ");
                bData.append (QByteArray :: number (resD[i].rowNumber));
                bData.append (" ");
                bData.append (QByteArray :: number (resD[i].columnNumber));
                bData.append (" ");
                bData.append (QByteArray :: number (resD[i].numberOfUnit));
                bData.append (" ");
                int nWidth = resD[i].rowNumber;
                int nHeight = resD[i].columnNumber;
                QImage pIm (nWidth, nHeight, QImage::Format_ARGB32);
                QByteArray bIm;// (imData);
                QFile imFile (this);
                if (fTests)
                    imFile.setFileName (imDir+QDir::separator()+QString("image_%1_%2.jpg").arg (resD[i].XY_angle).arg(resD[i].XZ_angle));
                //QFile debIm ("ddd.dat");
                //QDataStream debImage (&bIm, QIODevice::WriteOnly);
                //debIm.open (QIODevice::WriteOnly);
                int ncount (0);
                for (int ii=0; ii<nWidth; ii++)
                {
                    for (int iii=0; iii<nHeight; iii++)
                    {
                        uint c = (uchar)resD[i].data[ncount++];
                        bIm += QByteArray::number (c);//(uchar)resD[i].data[ncount++]);
                        c *= 255;
                        pIm.setPixel(ii, iii, qRgb(c,c,c));
                    }
                    //debImage << QString("\r\n");
                }
                if (fTests)
                    pIm.save (&imFile, "JPG");
                bData.append (bIm);
                v = KKSValue (QString(bData), KKSAttrType::atBinary);
            }

            KKSAttrValue * av = new KKSAttrValue (v, p.value());
            aVals.append (av);
            av->release ();
        }
        cef->setAttrValues (aVals);
        if (!fTests)
        {
            qint64 id = eiof->insertEIO (cef, ct, QString(), true);
            qDebug () << __PRETTY_FUNCTION__ << id;
        }
        cef->release ();
    }
    io->release ();
//    delete pProcD;
}

void ReperMainWindow::slotViewImage (void)
{
    ImageWidget * imW = new ImageWidget;
    QMdiSubWindow * m_imW = m_mdiArea->addSubWindow (imW);
    m_imW->setAttribute (Qt::WA_DeleteOnClose);
    m_imW->setWindowState (m_imW->windowState() | Qt::WindowActive);
    imW->show();

    connect (imW, SIGNAL (searchByIm (const QImage&)), this, SLOT (searchIm (const QImage&)) );
}

void ReperMainWindow::slotHelp (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    ass->showDocumentation ("index.html");
}

void ReperMainWindow::slotSettings (void)
{
    KKSSettings * s = kksApp->getKKSSettings();
    if(s)
        s->editSettings(this);
}

void ReperMainWindow :: initToolBars (void)
{
    tbActions->addAction (UI->actConnect);
    UI->actConnect->setIcon (QIcon(":/ddoc/connect.png"));
    tbActions->addAction (UI->actDisconnect);
    UI->actDisconnect->setIcon (QIcon(":/ddoc/disconnect.png"));
    tbActions->addAction (UI->actRLI);
    UI->actRLI->setIcon(QIcon(":/ddoc/find_doc.png"));
    tbActions->addAction (UI->actE_xit);
    UI->actE_xit->setIcon (QIcon(":/ddoc/quit.png"));
    this->addToolBar (tbActions);
    UI->actBy_Image_fragment->setIcon (QIcon (":/ddoc/search.png"));
    tbCalc->addAction (UI->actBy_Image_fragment);
    UI->actGenerateGol->setIcon (QIcon (":/ddoc/antenna_128.png"));
    tbCalc->addAction (UI->actGenerateGol);
    UI->actView3DMod->setIcon (QIcon (":/ddoc/galaxy3d.png"));
    tbCalc->addAction (UI->actView3DMod);
    this->addToolBar (tbCalc);
    UI->actSettings->setIcon (QIcon (":/ddoc/settings.png"));
    tbOthers->addAction (UI->actSettings);
    UI->actUser_Manual->setIcon (QIcon (":/ddoc/help.png"));
    tbOthers->addAction (UI->actUser_Manual);
    this->addToolBar (tbOthers);
}

void ReperMainWindow :: slot3DView (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QString filter (tr("Object files (*.obj);;All files(*);"));
    QString objFileName = QFileDialog::getOpenFileName (this, tr("Select source file"), QDir::currentPath(), filter);
    if (objFileName.isEmpty())
        return;

    QWidget * w = new Window (objFileName);
    QMdiSubWindow * m_subW = m_mdiArea->addSubWindow (w, Qt::Window);
    w->show();
    m_subW->setAttribute (Qt::WA_DeleteOnClose);
}

