#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QItemSelection>
#include <QMenu>
#include <QAction>
#include <QTreeView>
#include <QLineEdit>
#include <QCheckBox>
#include <QListView>
#include <QToolBar>
#include <QIcon>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSize>
#include <QMessageBox>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QDialog>
#include <QPushButton>
#include <QEventLoop>
#include <QPointer>
#include <QSplitter>

#include <QtDebug>

#include <KKSObject.h>
#include <KKSPrivilege.h>

#include "kksstuffform.h"
#include "KKSCheckableModel.h"
#include "KKSItemDelegate.h"
#include "KKSHIntervalW.h"
#include <KKSData.h>
#include <KKSEventFilter.h>
#include <KKSAccessEntity.h>
#include <KKSAclTemplate.h>
#include <defines.h>

KKSStuffForm :: KKSStuffForm (KKSAccessEntity * acl, int idCUser, QWidget *parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    tvStuffStruct (new QTreeView (this)),
    lESearch (0),
    chRoles (0),//new QCheckBox(tr("View roles only with granted privilegies"), this)),
    tvAccessTemplates (new QTreeView (this)),
    lvPrivilegies (new QListView (this)),
    tbActions (new QToolBar (this)),
    tbTemplActions (new QToolBar (this)),
    stuffPopupMenu (0),
    stuffSortModel (0),
    //wObject (io),
    m_acl (acl),
    idUser (idCUser),
    detailLevel (-1),//level),
    pbOk (new QPushButton (tr("&OK"), this)),
    pbCancel (new QPushButton (tr ("&Cancel"), this)),
    pEventL (0)
    //aclTemplate (0)
{
    if (m_acl)
        m_acl->addRef ();

    this->initWidget ();
    this->initPopupMenu ();

    QItemSelectionModel *stuffSelModel = tvStuffStruct->selectionModel ();

    if (stuffSelModel)
    {
        connect (stuffSelModel, SIGNAL (currentChanged (const QModelIndex&, const QModelIndex&)),
                 this,
                 SLOT (currentEntityChanged(const QModelIndex&, const QModelIndex&))
                 );
        stuffSelModel->clear ();
        stuffSelModel->setCurrentIndex (QModelIndex(), QItemSelectionModel::Clear);
    }

    connect (tvStuffStruct, SIGNAL (customContextMenuRequested ( const QPoint &)), this, SLOT (ContextMenuRequest(const QPoint&)) );

    pbOk->setVisible (false);
    pbCancel->setVisible (false);
    connect (pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );

    this->initPrivilegiesModel ();
}

KKSStuffForm :: KKSStuffForm (int idCUser,
                              int level,
                              QWidget * parent,
                              Qt::WindowFlags flags)
    : QWidget (parent, flags),
    tvStuffStruct (new QTreeView (this)),
    lESearch (0),
    chRoles (0),//new QCheckBox(tr("View roles only with granted privilegies"), this)),
    tvAccessTemplates (new QTreeView (this)),
    lvPrivilegies (new QListView (this)),
    tbActions (new QToolBar (this)),
    tbTemplActions (new QToolBar (this)),
    stuffPopupMenu (new QMenu(this)),
    stuffSortModel (new QSortFilterProxyModel (this)),
    m_acl (0),
    idUser (idCUser),
    detailLevel (level),
    pbOk (new QPushButton (tr("&OK"), this)),
    pbCancel (new QPushButton (tr ("&Cancel"), this)),
    pEventL (0)
{
    setWindowTitle (tr ("Stuff"));
    KKSEventFilter * ef = new KKSEventFilter (this);
    tvStuffStruct->viewport()->installEventFilter (ef);

    QHeaderView * hStuff = tvStuffStruct->header ();
    hStuff->setClickable (true);
    hStuff->setSortIndicatorShown (true);
    hStuff->setStretchLastSection (true);
    hStuff->setSortIndicator (0, Qt::AscendingOrder);

    tvStuffStruct->setSortingEnabled (true);
    tvStuffStruct->setModel (stuffSortModel);

    tvStuffStruct->setContextMenuPolicy (Qt::CustomContextMenu);
    tvStuffStruct->setMouseTracking (true);
    this->initWidget ();
    this->initPopupMenu ();

    QItemSelectionModel *stuffSelModel = tvStuffStruct->selectionModel ();

    if (stuffSelModel)
    {
        connect (stuffSelModel, SIGNAL (currentChanged (const QModelIndex&, const QModelIndex&)),
                 this,
                 SLOT (currentEntityChanged(const QModelIndex&, const QModelIndex&))
                 );
        stuffSelModel->clear ();
        stuffSelModel->setCurrentIndex (QModelIndex(), QItemSelectionModel::Clear);
    }

    connect (tvStuffStruct, SIGNAL (customContextMenuRequested ( const QPoint &)), this, SLOT (ContextMenuRequest(const QPoint&)) );

    pbOk->setVisible (false);
    pbCancel->setVisible (false);
    connect (pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );

    this->initPrivilegiesModel ();
/*    if (m_acl && idCUser != io->author() && idCUser != 1 )
    {
        this->lvPrivilegies->setEnabled (false);
        this->tbTemplActions->setEnabled (false);
    }
 */
}

KKSStuffForm :: KKSStuffForm (KKSAclTemplate * at, int idCUser, QWidget *parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    tvStuffStruct (new QTreeView (this)),
    lESearch (new QLineEdit(this)),
    chRoles (new QCheckBox(tr("View roles only with granted privilegies"), this)),
    tvAccessTemplates (new QTreeView (this)),
    lvPrivilegies (new QListView (this)),
    tbActions (new QToolBar (this)),
    tbTemplActions (new QToolBar (this)),
    stuffPopupMenu (0),
    stuffSortModel (0),
    m_acl (at),
    idUser (idCUser),
    detailLevel (-1),
    pbOk (new QPushButton (tr("&OK"), this)),
    pbCancel (new QPushButton (tr ("&Cancel"), this)),
    pEventL (0)
{
    if (m_acl)
        m_acl->addRef ();
    setWindowTitle (tr("Access rules template"));
    setWindowModality (Qt::WindowModal);
    setAttribute (Qt::WA_ShowModal, true);
    KKSEventFilter * ef = new KKSEventFilter (this);
    tvStuffStruct->viewport()->installEventFilter (ef);

    QHeaderView * hStuff = tvStuffStruct->header ();
    hStuff->setStretchLastSection (true);

    QItemSelectionModel *stuffSelModel = tvStuffStruct->selectionModel ();

    if (stuffSelModel)
    {
        connect (stuffSelModel, SIGNAL (currentChanged (const QModelIndex&, const QModelIndex&)),
                 this,
                 SLOT (currentEntityChanged(const QModelIndex&, const QModelIndex&))
                 );
        stuffSelModel->clear ();
    }

    connect (tvStuffStruct, SIGNAL (customContextMenuRequested ( const QPoint &)), this, SLOT (ContextMenuRequest(const QPoint&)) );

    this->initModalWidget ();
    pbOk->setVisible (true);
    pbCancel->setVisible (true);
    QKeySequence keyCancel (Qt::Key_Escape);
    pbCancel->setShortcut (keyCancel);
    connect (pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );

    this->initPrivilegiesModel ();
}

KKSStuffForm :: ~KKSStuffForm (void)
{
    if (stuffPopupMenu)
    {
        stuffPopupMenu->clear ();
        delete stuffPopupMenu;
    }
    if (m_acl)
        m_acl->release ();
    //delete ui;
    if (pEventL && pEventL->isRunning())
        pEventL->exit (0);

    if (pEventL)
    {
        if (pEventL && pEventL->isRunning())
            pEventL->exit (0);
        delete pEventL;
        pEventL = 0;
    }

    //if (aclTemplate)
        //aclTemplate->release ();
}

void KKSStuffForm :: currentEntityChanged (const QModelIndex& current, const QModelIndex& previous)
{
    Q_UNUSED (previous);
    int id_entity = current.isValid() ? current.data (Qt::UserRole+USER_ENTITY).toInt() : -1;
    switch (id_entity)
    {
        case atOrganization:
        {
            actOrgAdd->setEnabled (true);
            actOrgEdit->setEnabled (true);
            actOrgDel->setEnabled (true);
            actOrgAddr->setEnabled (true);
            actDivAdd->setEnabled (true);
            actDivEdit->setEnabled (false);
            actDivDel->setEnabled (false);
            actPosAdd->setEnabled (false);
            actPosEdit->setEnabled (false);
            actPosDel->setEnabled (false);
            actUserAdd->setEnabled (true);
            actUserEdit->setEnabled (false);
            actUserDel->setEnabled (false);
            actUserToPos->setEnabled (false);
            actUserFromPos->setEnabled (false);
            break;
        }
        case atDivision:
        {
            actOrgAdd->setEnabled (true);
            actOrgEdit->setEnabled (true);
            actOrgDel->setEnabled (true);
            actOrgAddr->setEnabled (true);
            actDivAdd->setEnabled (true);
            actDivEdit->setEnabled (true);
            actDivDel->setEnabled (true);
            actPosAdd->setEnabled (true);
            actPosEdit->setEnabled (false);
            actPosDel->setEnabled (false);
            actUserAdd->setEnabled (true);
            actUserEdit->setEnabled (false);
            actUserDel->setEnabled (false);
            actUserToPos->setEnabled (false);
            actUserFromPos->setEnabled (false);
            break;
        }
        case atPosition:
        {
            actOrgAdd->setEnabled (true);
            actOrgEdit->setEnabled (true);
            actOrgDel->setEnabled (true);
            actOrgAddr->setEnabled (true);
            actDivAdd->setEnabled (true);
            actDivEdit->setEnabled (true);
            actDivDel->setEnabled (true);
            actPosAdd->setEnabled (true);
            actPosEdit->setEnabled (true);
            actPosDel->setEnabled (true);
            actUserAdd->setEnabled (true);
            actUserEdit->setEnabled (false);
            actUserDel->setEnabled (false);
            bool isSet = current.child (0, 0).isValid ();
            actUserToPos->setEnabled (!isSet);
            actUserFromPos->setEnabled (isSet);
            break;
        }
        case atUser:
        {
            actOrgAdd->setEnabled (true);
            actOrgEdit->setEnabled (true);
            actOrgDel->setEnabled (true);
            actOrgAddr->setEnabled (true);
            actDivAdd->setEnabled (true);
            actDivEdit->setEnabled (true);
            actDivDel->setEnabled (true);
            actPosAdd->setEnabled (true);
            actPosEdit->setEnabled (true);
            actPosDel->setEnabled (true);
            actUserAdd->setEnabled (true);
            actUserEdit->setEnabled (true);
            actUserDel->setEnabled (true);
            actUserToPos->setEnabled (false);
            actUserFromPos->setEnabled (true);
            break;
        }
        default:
        {
            actOrgAdd->setEnabled (true);
            actOrgEdit->setEnabled (false);
            actOrgDel->setEnabled (false);
            actOrgAddr->setEnabled (false);
            actDivAdd->setEnabled (false);
            actDivEdit->setEnabled (false);
            actDivDel->setEnabled (false);
            actPosAdd->setEnabled (false);
            actPosEdit->setEnabled (false);
            actPosDel->setEnabled (false);
            actUserAdd->setEnabled (true);
            actUserEdit->setEnabled (false);
            actUserDel->setEnabled (false);
            actUserToPos->setEnabled (false);
            actUserFromPos->setEnabled (false);
            break;
        }
    }
    if ( current.isValid() )
    {
        lvPrivilegies->setEnabled (true);
        int idEntity = current.data (Qt::UserRole+USER_ENTITY).toInt();
        //emit refreshPrivilegies (current, wObject, lvPrivilegies->model());
        int idRole = -1;
        KKSPrivilege * p = 0;
        switch (idEntity)
        {
            case atBoss:
            {
                idRole = 1;
                int idBoss = current.data(Qt::UserRole).toInt();
                if (m_acl && !m_acl->isNamed())
                {
                    lvPrivilegies->setEnabled (false);
                    //KKSAccessEntity * acl = wObject->getAccessRules();
                    KKSMap<int, KKSPrivilege*>::const_iterator pr = m_acl->getBossPrivilegies().constFind (idBoss);
                    if (pr == m_acl->getBossPrivilegies().constEnd())
                        pr = m_acl->getBossPrivilegies().constFind (idRole);
                    if (pr != m_acl->getBossPrivilegies().constEnd())
                        p = new KKSPrivilege (*pr.value());
                    else
                        p = new KKSPrivilege (idBoss, false, false, false, false, false);
                }
                else if (m_acl && m_acl->isNamed() && (m_acl->getBossPrivilegies().contains (idRole) || m_acl->getBossPrivilegies().contains (idBoss)))
                {
                    KKSMap<int, KKSPrivilege*>::const_iterator pr = m_acl->getBossPrivilegies().constFind (idBoss);
                    if (pr == m_acl->getBossPrivilegies().constEnd())
                        pr = m_acl->getBossPrivilegies().constFind (idRole);
                    p = new KKSPrivilege (*pr.value());
                }
                else
                    p = new KKSPrivilege (idRole, false, false, false, false, false);
                break;
            }
            case atGroup:
            {
                idRole = 2;
                int idUnit = current.data(Qt::UserRole).toInt();
                if (m_acl && !m_acl->isNamed())
                {
                    lvPrivilegies->setEnabled (false);
                    //KKSAccessEntity * acl = wObject->getAccessRules();
                    KKSMap<int, KKSPrivilege*>::const_iterator pr = m_acl->getUnitPrivilegies().constFind (idUnit);
                    if (pr != m_acl->getUnitPrivilegies().constEnd())
                        p = new KKSPrivilege (*pr.value());
                    else if (m_acl->getUnitPrivilegies().constFind (idRole) != m_acl->getUnitPrivilegies().constEnd())
                    {
                        pr = m_acl->getUnitPrivilegies().constFind (idRole);
                        p = new KKSPrivilege (*pr.value());
                    }
                    else
                        p = new KKSPrivilege (idUnit, false, false, false, false, false);
                }
                else if (m_acl && m_acl->isNamed() && m_acl->getUnitPrivilegies().contains (idRole))
                {
                    KKSMap<int, KKSPrivilege*>::const_iterator pr = m_acl->getUnitPrivilegies().constFind (idRole);
                    p = new KKSPrivilege (*pr.value());
                }
                else if (m_acl && m_acl->getUnitPrivilegies().contains (idUnit))
                {
                    KKSMap<int, KKSPrivilege*>::const_iterator pr = m_acl->getUnitPrivilegies().constFind (idUnit);
                    p = new KKSPrivilege (*pr.value());
                }
                else
                    p = new KKSPrivilege (idRole, false, false, false, false, false);
                break;
            }
            case atOthers:
            {
                idRole = 3;
                if (m_acl && !m_acl->isNamed())
                {
                    //KKSAccessEntity * acl = wObject->getAccessRules();
                    if (m_acl->getOthersPrivilege())
                        p = new KKSPrivilege (*m_acl->getOthersPrivilege());
                    else
                    {
                        p = new KKSPrivilege (idRole, false, false, false, false, false);
                        m_acl->setOthersPrivilege (p);
                    }
                }
                else if (m_acl && m_acl->isNamed() && m_acl->getOthersPrivilege())
                    p = new KKSPrivilege (*m_acl->getOthersPrivilege());
                else
                    p = new KKSPrivilege (idRole, false, false, false, false, false);
                break;
            }
            case atExceptions: default:
            {
                this->lvPrivilegies->setEnabled (false);
                idRole = current.data (Qt::UserRole).toInt();
                p = new KKSPrivilege (idRole, false, false, false, false, false);
                break;
            }
            case atDivision: 
            {
                idRole = current.data (Qt::UserRole).toInt();
                //KKSAccessEntity * acl = wObject ? wObject->getAccessRules() : 0;
                if (m_acl && !m_acl->isNamed() && (m_acl->getPrivilegies().contains (idRole) || m_acl->getUnitPrivilegies ().contains (idRole)))
                {
                    KKSMap<int, KKSPrivilege*>::const_iterator pr = m_acl->getPrivilegies().constFind (idRole);
                    if (pr == m_acl->getPrivilegies().constEnd())
                        pr = m_acl->getUnitPrivilegies().constFind (idRole);
                    p = new KKSPrivilege (*pr.value());
                }
                else if (m_acl && m_acl->isNamed() && m_acl->getPrivilegies().contains (idRole))
                {
                    KKSMap<int, KKSPrivilege*>::const_iterator pr = m_acl->getPrivilegies().constFind (idRole);
                    p = new KKSPrivilege (*pr.value());
                }
                else
                    p = new KKSPrivilege (idRole, false, false, false, false, false);
                break;
            }
            case atPosition:
            {
                idRole = current.data (Qt::UserRole).toInt();
                //KKSAccessEntity * acl = wObject ? wObject->getAccessRules() : 0;
                if (m_acl && !m_acl->isNamed() && (m_acl->getPrivilegies().contains (idRole) || m_acl->getBossPrivilegies().contains (idRole)) )
                {
                    KKSMap<int, KKSPrivilege*>::const_iterator pr = m_acl->getPrivilegies().constFind (idRole);
                    if (pr == m_acl->getPrivilegies().constEnd())
                        pr = m_acl->getBossPrivilegies().constFind (idRole);
                    p = new KKSPrivilege (*pr.value());
                }
                else if (m_acl && m_acl->isNamed() && m_acl->getPrivilegies().contains (idRole))
                {
                    KKSMap<int, KKSPrivilege*>::const_iterator pr = m_acl->getPrivilegies().constFind (idRole);
                    p = new KKSPrivilege (*pr.value());
                }
                else
                    p = new KKSPrivilege (idRole, false, false, false, false, false);
                break;
                
            }
            case atUser:
            {
                idRole = current.data (Qt::UserRole).toInt();
                //KKSAccessEntity * acl = wObject ? wObject->getAccessRules() : 0;
                if (/*(wObject && idRole == wObject->author()) ||*/ (m_acl && m_acl->isNamed() && m_acl->id() <= 0 && idRole == idUser))
                    p = new KKSPrivilege (idRole, true, true, true, true, true);
                else if (m_acl && !m_acl->isNamed() && m_acl->getPrivilegies().contains (idRole))
                {
                    KKSMap<int, KKSPrivilege*>::const_iterator pr = m_acl->getPrivilegies().constFind (idRole);
                    p = new KKSPrivilege (*pr.value());
                }
                else if (m_acl && m_acl->isNamed() && m_acl->getPrivilegies().contains (idRole))
                {
                    KKSMap<int, KKSPrivilege*>::const_iterator pr = m_acl->getPrivilegies().constFind (idRole);
                    p = new KKSPrivilege (*pr.value());
                }
                else
                    p = new KKSPrivilege (idRole, false, false, false, false, false);
                break;
            }
        }
        if (!p)
            return;
        QAbstractItemModel * privMod (lvPrivilegies->model());
        for (int i=0; i<5; i++)
        {
            QModelIndex wIndex = privMod->index (i, 0);
            switch (i)
            {
                case 0: privMod->setData (wIndex, p->allowReadList(), Qt::CheckStateRole); break;
                case 1: privMod->setData (wIndex, p->allowRead(), Qt::CheckStateRole); break;
                case 2: privMod->setData (wIndex, p->allowUpdate(), Qt::CheckStateRole); break;
                case 3: privMod->setData (wIndex, p->allowDelete(), Qt::CheckStateRole); break;
                case 4: privMod->setData (wIndex, p->allowSend(), Qt::CheckStateRole); break;
            }
        }
        p->release ();
    }
    else
    {
        QAbstractItemModel * privMod (lvPrivilegies->model());
        for (int i=0; i<5; i++)
        {
            QModelIndex wIndex = privMod->index (i, 0);
            privMod->setData (wIndex, true, Qt::CheckStateRole);
        }
        
    }
    //else if (current.isValid() && aclTemplate)
        //emit refreshAclPrivilegies (current, aclTemplate, lvPrivilegies->model());
}

void KKSStuffForm :: setPrivilegeForStuffEntity (const QModelIndex& ind)
{
    if (!ind.isValid () )
        return;

    if (!m_acl)
        return;

    if (isAclReadOnly ())//m_acl && (wObject->author() <= 0 || idUser != wObject->author()) && idUser != ADMIN_ROLE )
    {
        QMessageBox::warning (this, tr ("User Privilegies"), tr ("Only document author can change user privilegies"), QMessageBox::Ok);
        QAbstractItemModel* cModel = lvPrivilegies->model ();
        cModel->setData (ind, Qt::Checked-ind.data (Qt::CheckStateRole).toInt(), Qt::CheckStateRole);
        return;
    }
    qDebug () << __PRETTY_FUNCTION__ << ind.data (Qt::CheckStateRole);
    QList<bool> allowList;
    for (int i=0; i<ind.model()->rowCount(); i++)
    {
        QModelIndex cIndex = ind.model()->index (i, 0);
        allowList.append ((cIndex.data (Qt::CheckStateRole).toInt() == Qt::Checked));
    }

    QModelIndex stIndex = this->tvStuffStruct->selectionModel()->currentIndex ();
    int idRole = -1;//stuffSelModel->currentIndex().data (Qt::UserRole).toInt ();
    int idEntity = stIndex.data (Qt::UserRole+USER_ENTITY).toInt();
    KKSPrivilege * cPriv = 0;
    switch (idEntity)
    {
        case atBoss:
        {
            idRole = m_acl && !m_acl->isNamed() ? stIndex.data (Qt::UserRole).toInt() : 1;
            cPriv = new KKSPrivilege (idRole, allowList[0], allowList[1], allowList[3], allowList[2], allowList[4]);
            KKSMap<int, KKSPrivilege*> privs = m_acl->getBossPrivilegies();
            if (privs.contains (idRole))
                privs.remove (idRole);
            privs.insert (idRole, cPriv);
            m_acl->setBossPrivilegies (privs);
            break;
        }
        case atGroup:
        {
            idRole = m_acl && !m_acl->isNamed() ? stIndex.data (Qt::UserRole).toInt() : 2;
            cPriv = new KKSPrivilege (idRole, allowList[0], allowList[1], allowList[3], allowList[2], allowList[4]);
            KKSMap<int, KKSPrivilege*> privs = m_acl->getUnitPrivilegies();
            if (privs.contains (idRole))
                privs.remove (idRole);
            privs.insert (idRole, cPriv);
            m_acl->setUnitPrivilegies (privs);
            break;
        }
        case atOthers:
        {
            idRole = m_acl && !m_acl->isNamed() ? OTHERS_ROLE : 3;
            cPriv = new KKSPrivilege (idRole, allowList[0], allowList[1], allowList[3], allowList[2], allowList[4]);
            cPriv->setRole(idRole);
            m_acl->setOthersPrivilege(cPriv);
            break;
        }
        case atExceptions: default: break;
        case atDivision: case atPosition: case atUser:
        {
            idRole = stIndex.data (Qt::UserRole).toInt();
            cPriv = new KKSPrivilege (idRole, allowList[0], allowList[1], allowList[3], allowList[2], allowList[4]);
            KKSMap<int, KKSPrivilege*> privs = m_acl->getPrivilegies();
            if (privs.contains (idRole))
                privs.remove (idRole);
            privs.insert (idRole, cPriv);
            m_acl->setPrivilegies (privs);
            break;
       }
    }
    if (m_acl && !m_acl->isNamed() && cPriv)
    {
        //emit privilegiesChanged (cPriv, wObject);
        emit accessRulesChanged (m_acl);
        return;
    }

    switch (idEntity)
    {
        case atBoss:
        {
            idRole = 1;
            cPriv->setRole(idRole);
            KKSMap<int, KKSPrivilege*> privs = m_acl->getBossPrivilegies();
            if (privs.contains (idRole))
                privs.remove (idRole);
            privs.insert (idRole, cPriv);
            m_acl->setBossPrivilegies (privs);
            break;
        }
        case atGroup:
        {
            idRole = 2;
            cPriv->setRole(idRole);
            KKSMap<int, KKSPrivilege*> privs = m_acl->getUnitPrivilegies();
            if (privs.contains (idRole))
                privs.remove (idRole);
            privs.insert (idRole, cPriv);
            m_acl->setUnitPrivilegies (privs);
            break;
        }
        case atOthers:
        {
            idRole = 3;
            cPriv->setRole(idRole);
            m_acl->setOthersPrivilege(cPriv);
            break;
        }
        case atExceptions: default: break;
        case atDivision: case atPosition: case atUser:
        {
            idRole = stIndex.data (Qt::UserRole).toInt();
            cPriv->setRole(idRole);
            KKSMap<int, KKSPrivilege*> privs = m_acl->getPrivilegies();
            if (privs.contains (idRole))
                privs.remove (idRole);
            privs.insert (idRole, cPriv);
            m_acl->setPrivilegies (privs);
            break;
       }
    }
}

void KKSStuffForm :: currentTemplateChanged (const QModelIndex& current, const QModelIndex& previous)
{
    qDebug () << __PRETTY_FUNCTION__ << current << previous;
}

void KKSStuffForm :: initPrivilegiesModel (void)
{
    QStandardItemModel * privModel = new KKSCheckableModel (5, 1);
    QStringList privList;
    privList << tr ("Read of presense")
             << tr ("Read parameters")
             << tr ("Edit")
             << tr ("Remove")
             << tr ("Allow to syncronize");

    for (int i=0; i<privList.count(); i++)
    {
        QModelIndex wIndex = privModel->index (i, 0);
        privModel->setData (wIndex, privList[i], Qt::DisplayRole);
        privModel->setData (wIndex, Qt::Unchecked, Qt::CheckStateRole);
    }
    lvPrivilegies->setModel (privModel);
    lvPrivilegies->setEnabled (false);

    connect (lvPrivilegies, SIGNAL (clicked (const QModelIndex&)),
             this,
             SLOT (setPrivilegeForStuffEntity (const QModelIndex&))
             );
    connect (lvPrivilegies, SIGNAL (activated(const QModelIndex& )),
             this,
             SLOT (setPrivilegeForStuffEntity (const QModelIndex& ))
            );
}

void KKSStuffForm :: initPopupMenu (void)
{
    if (!stuffPopupMenu)
        return;

    this->stuffPopupMenu->clear ();
    this->stuffPopupMenu->addAction (actStuffRefresh);
    this->stuffPopupMenu->addSeparator ();

    this->stuffPopupMenu->addAction (actOrgAdd);
    this->stuffPopupMenu->addAction (actOrgEdit);
    this->stuffPopupMenu->addAction (actOrgDel);
    this->stuffPopupMenu->addAction (actOrgAddr);
    this->stuffPopupMenu->addSeparator ();

    this->stuffPopupMenu->addAction (actDivAdd);
    this->stuffPopupMenu->addAction (actDivEdit);
    this->stuffPopupMenu->addAction (actDivDel);
    this->stuffPopupMenu->addSeparator ();

    this->stuffPopupMenu->addAction (actPosAdd);
    this->stuffPopupMenu->addAction (actPosEdit);
    this->stuffPopupMenu->addAction (actPosDel);
    this->stuffPopupMenu->addSeparator ();

    this->stuffPopupMenu->addAction (actUserAdd);
    this->stuffPopupMenu->addAction (actUserEdit);
    this->stuffPopupMenu->addAction (actUserDel);
    this->stuffPopupMenu->addAction (actUserToPos);
    this->stuffPopupMenu->addAction (actUserFromPos);
}

void KKSStuffForm :: createOrganization (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (m_acl)
        return;
    if (stuffSortModel)
    {
        QItemSelectionModel * selModel = tvStuffStruct->selectionModel ();

        QItemSelection sel = selModel->selection();
        QModelIndex selIndex = sel.indexes().isEmpty() ? QModelIndex () : sel.indexes().at(0);
        QModelIndex wIndex = stuffSortModel->mapToSource (selIndex);
        QModelIndex pOrgIndex = wIndex;
        while (pOrgIndex.data (Qt::UserRole+USER_ENTITY).toInt() != atOrganization && pOrgIndex.isValid())
            pOrgIndex = pOrgIndex.parent();
        //qDebug () << __PRETTY_FUNCTION__ << pOrgIndex;
        emit addStuffEntity (pOrgIndex, atOrganization, stuffSortModel->sourceModel ());
    }
}

void KKSStuffForm :: editOrganization (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (m_acl)
        return;
    QItemSelectionModel * selModel = tvStuffStruct->selectionModel ();

    QModelIndex selIndex = selModel->currentIndex();
    QModelIndex wIndex = stuffSortModel->mapToSource (selIndex);
    QModelIndex pOrgIndex = wIndex;
    while (pOrgIndex.data (Qt::UserRole+USER_ENTITY).toInt() != atOrganization && pOrgIndex.isValid())
        pOrgIndex = pOrgIndex.parent();

    if (!pOrgIndex.isValid() || pOrgIndex.data (Qt::UserRole+USER_ENTITY).toInt() != atOrganization)
    {
        QMessageBox::warning (this, tr ("Edit organization"), tr ("Select organization to edit"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    emit editStuffEntity (pOrgIndex, atOrganization, stuffSortModel->sourceModel ());
}

void KKSStuffForm :: dropOrganization (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (m_acl)
        return;
    QItemSelectionModel * selModel = tvStuffStruct->selectionModel ();

    QModelIndex selIndex = selModel->currentIndex();
    QModelIndex wIndex = stuffSortModel->mapToSource (selIndex);
    QModelIndex pOrgIndex = wIndex;
    while (pOrgIndex.data (Qt::UserRole+USER_ENTITY).toInt() != atOrganization && pOrgIndex.isValid())
        pOrgIndex = pOrgIndex.parent();
    
    if (pOrgIndex.isValid() && pOrgIndex.data (Qt::UserRole+USER_ENTITY).toInt() == atOrganization)
    {
//        int res = QMessageBox::question (this, tr ("Delete organization"), tr ("Do you really want to delete ?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);
//        if (res == QMessageBox::Yes)
//        QModelIndex wIndex = stuffSortModel->mapToSource (selIndex);
        emit delStuffEntity (pOrgIndex, atOrganization, stuffSortModel->sourceModel ());
    }
}

void KKSStuffForm :: setOrganizationAddress (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (m_acl)
        return;
    QItemSelectionModel * selModel = tvStuffStruct->selectionModel ();

    QModelIndex selIndex = selModel->currentIndex();
    QModelIndex wIndex = stuffSortModel->mapToSource (selIndex);
    QModelIndex pOrgIndex = wIndex;
    while (pOrgIndex.data (Qt::UserRole+USER_ENTITY).toInt() != atOrganization && pOrgIndex.isValid())
        pOrgIndex = pOrgIndex.parent();

    if (!pOrgIndex.isValid() || pOrgIndex.data (Qt::UserRole+USER_ENTITY).toInt() != atOrganization)
    {
        QMessageBox::warning (this, tr ("Set address"), tr ("Select organization for address setting"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    emit setOrgAddr (pOrgIndex, stuffSortModel->sourceModel ());
}

void KKSStuffForm :: createDivision (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (m_acl)
        return;
    QItemSelectionModel * selModel = tvStuffStruct->selectionModel ();

    QModelIndex selIndex = selModel->currentIndex();
    if (!selIndex.isValid() ||
            (selIndex.data (Qt::UserRole+USER_ENTITY).toInt() != atOrganization &&
             selIndex.data (Qt::UserRole+USER_ENTITY).toInt() != atDivision)
            )
    {
        QMessageBox::warning (this, tr ("Create division"), tr ("Select parent organization or division"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    QModelIndex wIndex = stuffSortModel->mapToSource (selIndex);
    emit addStuffEntity (wIndex, atDivision, stuffSortModel->sourceModel ());
}

void KKSStuffForm :: editDivision (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (m_acl)
        return;
    QItemSelectionModel * selModel = tvStuffStruct->selectionModel ();

    QModelIndex selIndex = selModel->currentIndex();
    if (!selIndex.isValid() || selIndex.data (Qt::UserRole+USER_ENTITY).toInt() != atDivision)
    {
        QMessageBox::warning (this, tr ("Edit division"), tr ("Select division"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    QModelIndex wIndex = stuffSortModel->mapToSource (selIndex);
    emit editStuffEntity (wIndex, atDivision, stuffSortModel->sourceModel ());
}

void KKSStuffForm :: dropDivision (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (m_acl)
        return;
    QItemSelectionModel * selModel = tvStuffStruct->selectionModel ();

    QModelIndex selIndex = selModel->currentIndex();
    if (selIndex.isValid() && selIndex.data (Qt::UserRole+USER_ENTITY).toInt() == atDivision)
    {
//        int res = QMessageBox::question (this, tr ("Delete division"), tr ("Do you really want to delete ?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);
//        if (res == QMessageBox::Yes)
        QModelIndex wIndex = stuffSortModel->mapToSource (selIndex);
        emit delStuffEntity (wIndex, atDivision, stuffSortModel->sourceModel ());
    }
}

void KKSStuffForm :: createPosition (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (m_acl)
        return;
    QItemSelectionModel * selModel = tvStuffStruct->selectionModel ();

    QModelIndex selIndex = selModel->currentIndex();
    if (!selIndex.isValid() || \
        (selIndex.data (Qt::UserRole+USER_ENTITY).toInt() != atOrganization && selIndex.data (Qt::UserRole+USER_ENTITY).toInt() != atDivision ))
    {
        QMessageBox::warning (this, tr ("Create position"), tr ("Select parent organization or division"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    QModelIndex wIndex = stuffSortModel->mapToSource (selIndex);
    emit addStuffEntity (wIndex, atPosition, stuffSortModel->sourceModel ());
}

void KKSStuffForm :: editPosition (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (m_acl)
        return;
    QItemSelectionModel * selModel = tvStuffStruct->selectionModel ();

    QModelIndex selIndex = selModel->currentIndex();
    if (!selIndex.isValid() || selIndex.data (Qt::UserRole+USER_ENTITY).toInt() != atPosition)
    {
        QMessageBox::warning (this, tr ("Edit position"), tr ("Select position"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    QModelIndex wIndex = stuffSortModel->mapToSource (selIndex);
    emit editStuffEntity (wIndex, atPosition, stuffSortModel->sourceModel ());
}

void KKSStuffForm :: dropPosition (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (m_acl)
        return;
    QItemSelectionModel * selModel = tvStuffStruct->selectionModel ();

    QModelIndex selIndex = selModel->currentIndex();
    if (selIndex.isValid() && selIndex.data (Qt::UserRole+USER_ENTITY).toInt() == atPosition)
    {
//        int res = QMessageBox::question (this, tr ("Delete position"), tr ("Do you really want to delete ?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);
//        if (res == QMessageBox::Yes)
        QModelIndex wIndex = stuffSortModel->mapToSource (selIndex);
        emit delStuffEntity (wIndex, atPosition, stuffSortModel->sourceModel ());
    }
}

void KKSStuffForm :: createUser (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (m_acl)
        return;
    QItemSelectionModel * selModel = tvStuffStruct->selectionModel ();

    QModelIndex selIndex = selModel->currentIndex();
    if (!selIndex.isValid() )
    {
        QMessageBox::warning (this, tr ("Create position"), tr ("Select parent organization or division"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    QModelIndex wIndex = stuffSortModel->mapToSource (selIndex);
    emit addStuffEntity (wIndex, atUser, stuffSortModel->sourceModel ());
}

void KKSStuffForm :: editUser (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (m_acl)
        return;
    QItemSelectionModel * selModel = tvStuffStruct->selectionModel ();

    QModelIndex selIndex = selModel->currentIndex();
    if (!selIndex.isValid() || selIndex.data (Qt::UserRole+USER_ENTITY).toInt() != atUser)
    {
        QMessageBox::warning (this, tr ("Edit user"), tr ("Select user"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    QModelIndex wIndex = stuffSortModel->mapToSource (selIndex);
    emit editStuffEntity (wIndex, atUser, stuffSortModel->sourceModel ());
}

void KKSStuffForm :: dropUser (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QItemSelectionModel * selModel = tvStuffStruct->selectionModel ();

    QModelIndex selIndex = selModel->currentIndex();
    if (selIndex.isValid() && selIndex.data (Qt::UserRole+USER_ENTITY).toInt() == atUser)
    {
//        int res = QMessageBox::question (this, tr ("Delete user"), tr ("Do you really want to delete ?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);
//        if (res == QMessageBox::Yes)
        QModelIndex wIndex = stuffSortModel->mapToSource (selIndex);
        emit delStuffEntity (wIndex, atUser, stuffSortModel->sourceModel ());
    }
}

void KKSStuffForm :: setUserOntoPosition (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (m_acl)// || aclTemplate)
        return;
    QItemSelectionModel * selModel = tvStuffStruct->selectionModel ();

    QModelIndex selIndex = selModel->currentIndex();
    if (selIndex.isValid() && selIndex.data (Qt::UserRole+USER_ENTITY).toInt() == atPosition)
    {
        QModelIndex posIndex = stuffSortModel->mapToSource (selIndex);
        emit setUserToPos (posIndex, stuffSortModel->sourceModel ());
        actUserToPos->setEnabled (stuffSortModel->sourceModel ()->rowCount (posIndex) == 0 );
        actUserFromPos->setEnabled (stuffSortModel->sourceModel ()->rowCount (posIndex) > 0 );
    }
}

void KKSStuffForm :: dismissUserFromPosition (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (m_acl)//wObject || aclTemplate)
        return;
    QItemSelectionModel * selModel = tvStuffStruct->selectionModel ();

    QModelIndex selIndex = selModel->currentIndex();
    if (selIndex.isValid() && (selIndex.data (Qt::UserRole+USER_ENTITY).toInt() == atPosition ||selIndex.data (Qt::UserRole+USER_ENTITY).toInt() == atUser ) )
    {
        QModelIndex posIndex = stuffSortModel->mapToSource (selIndex.data (Qt::UserRole+USER_ENTITY).toInt() == atPosition ? selIndex : selIndex.parent());
        int res = QMessageBox::question (this, tr ("Dismiss user"), tr ("Do you really want to dismiss ?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (res == QMessageBox::Yes)
            emit dropUserFromPos (posIndex, stuffSortModel->sourceModel ());
        actUserToPos->setEnabled (stuffSortModel->sourceModel ()->rowCount (posIndex) == 0 );
        actUserFromPos->setEnabled (stuffSortModel->sourceModel ()->rowCount (posIndex) > 0 );
    }
}

void KKSStuffForm :: ContextMenuRequest(const QPoint& pos)
{
    this->stuffPopupMenu->exec (tvStuffStruct->mapToGlobal(pos));
}

void KKSStuffForm :: save (void)
{
    qDebug () << __PRETTY_FUNCTION__;
}
/*
KKSObject * KKSStuffForm :: getObj (void) const
{
    return wObject;
}
*/

void KKSStuffForm :: setStuffModel (QAbstractItemModel * stModel)
{
    this->initTvModel (tvStuffStruct, stModel, SLOT (currentEntityChanged (const QModelIndex&, const QModelIndex&)));
    if (!m_acl && stuffSortModel)
    {
        QHeaderView * hv = tvStuffStruct->header ();
        stuffSortModel->sort (0, hv->sortIndicatorOrder ());
    }
}

void KKSStuffForm :: setTemplateModel (QAbstractItemModel * tModel)
{
    this->initTvModel (tvAccessTemplates, tModel, SLOT (currentTemplateChanged (const QModelIndex&, const QModelIndex&)));
    KKSEventFilter *eFilter = new KKSEventFilter (this);
    tvAccessTemplates->viewport()->installEventFilter (eFilter);
}

void KKSStuffForm :: initTvModel (QTreeView * tv, QAbstractItemModel * nModel, const char* method)
{
    QAbstractItemModel * oldModel = tv->model ();
    while (qobject_cast<QAbstractProxyModel *>(oldModel))
        oldModel = qobject_cast<QAbstractProxyModel *>(oldModel)->sourceModel ();
    QItemSelectionModel *selModel = tv->selectionModel ();
    QModelIndex cSelIndex = selModel ? selModel->currentIndex () : QModelIndex();

    if (selModel)
        QObject::disconnect (selModel, SIGNAL (currentChanged (const QModelIndex&, const QModelIndex&)), this, method);

    QAbstractProxyModel * proxyModel = qobject_cast<QAbstractProxyModel *>(tv->model());
    if (proxyModel)
        proxyModel->setSourceModel (nModel);
    else
        tv->setModel (nModel);

    selModel = tv->selectionModel ();

    if (selModel)
        QObject::connect (selModel, SIGNAL (currentChanged (const QModelIndex&, const QModelIndex&)), this, method);

    if (oldModel && oldModel != nModel)
        delete oldModel;

    QAbstractItemDelegate * oldDeleg = tv->itemDelegate ();
    if (oldDeleg && qobject_cast <KKSItemDelegate *> (oldDeleg))
        return;

    QItemDelegate * iDeleg = new KKSItemDelegate (this);
    tv->setItemDelegate (iDeleg);

    if (oldDeleg)
        delete oldDeleg;
}

void KKSStuffForm :: addTemplate (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    emit addAclTemplate (tvAccessTemplates->model ());
}

void KKSStuffForm :: editTemplate (void)
{
    QItemSelectionModel *selModel = tvAccessTemplates->selectionModel ();
    if (!selModel || selModel->selectedIndexes ().isEmpty())
    {
        QMessageBox::warning (this, tr ("Access templates"), tr ("Please select template"), QMessageBox::Ok);
        return;
    }
    QItemSelection iSel = selModel->selection ();
    int idAclTemplate = iSel.indexes().at (0).data (Qt::UserRole).toInt();
    emit editAclTemplate (idAclTemplate, tvAccessTemplates->model ());
}

void KKSStuffForm :: applyTemplate (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QItemSelectionModel *selModel = tvAccessTemplates->selectionModel ();
    if (!selModel || selModel->selectedIndexes ().isEmpty())
    {
        QMessageBox::warning (this, tr ("Access templates"), tr ("Please select template"), QMessageBox::Ok);
        return;
    }
    QItemSelection iSel = selModel->selection ();
    int idAclTemplate = iSel.indexes().at (0).data (Qt::UserRole).toInt();
    if (idAclTemplate > 0)
    {
        emit applyAclTemplate (idAclTemplate, m_acl);
        //KKSPrivilege * cPriv = new KKSPrivilege (wObject->author(), true, true, true, true, true);
        emit accessRulesChanged (m_acl);
        //cPriv->release ();
    }
}

void KKSStuffForm :: saveAsTemplate (void)
{
    qDebug () << __PRETTY_FUNCTION__;
}

void KKSStuffForm :: refreshTemplates (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    emit refreshAclTemplates (tvAccessTemplates->model());
}

void KKSStuffForm :: dropTemplate (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QItemSelectionModel *selModel = tvAccessTemplates->selectionModel ();
    if (!selModel || selModel->selectedIndexes ().isEmpty())
    {
        QMessageBox::warning (this, tr ("Access templates"), tr ("Please select template"), QMessageBox::Ok);
        return;
    }
    QModelIndex tIndex = selModel->currentIndex();
    bool isSystem = tIndex.sibling (tIndex.row(), 0).data (Qt::UserRole+1).toBool ();

    if (isSystem)
    {
        QMessageBox::warning (this, tr ("Delete access template"), tr ("You cannot delete system templates"), QMessageBox::Ok);
        return;
    }

    QMessageBox::StandardButton isDel = QMessageBox::question (this, tr ("Delete access template"), tr ("Do you really want to delete ?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (isDel == QMessageBox::Yes)
        emit deleteAclTemplate (tIndex.sibling (tIndex.row(), 0).data (Qt::UserRole).toInt (), tvAccessTemplates->model(), tIndex.row());
}

QAbstractItemView * KKSStuffForm :: getPrivView (void) const
{
    return lvPrivilegies;
}

void KKSStuffForm :: initWidget (void)
{
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (tbActions, 0, 0, 1, 2);
    
    QSize iSize (24, 24);
    tbActions->setIconSize (iSize);

    if (m_acl)
        tbActions->setVisible (false);

    this->initActions();

    gLay->addWidget (tvStuffStruct, 1, 0, 1, (m_acl ? 1 : 2));

    QVBoxLayout * vLay = new QVBoxLayout ();
/*
    QGroupBox * gbFilters = new QGroupBox (tr ("Filters"), this);
    vLay->addWidget (gbFilters);

    QGridLayout * gFiltLay = new QGridLayout (gbFilters);
    QLabel * lSearch = new QLabel (tr("Search"), gbFilters);
    gFiltLay->addWidget (lSearch, 0, 0, 1, 1);
    gFiltLay->addWidget (lESearch, 0, 1, 1, 1);
    gFiltLay->addWidget (chRoles, 1, 0, 1, 2);
    //
    // Пока скрыть
    // TODO: сделать фильтр к древовидной структуре
    //
    gbFilters->setVisible (false);
*/
    QHBoxLayout * hTemplLay = new QHBoxLayout ();
    hTemplLay->addWidget (tvAccessTemplates);
    hTemplLay->addWidget (tbTemplActions);
    tbTemplActions->setOrientation (Qt::Vertical);
    tbTemplActions->setIconSize (iSize);
    vLay->addLayout (hTemplLay);

    actTemplAdd = new QAction (QIcon (":/ddoc/add.png"), tr ("Add"), this);
    actTemplAdd->setToolTip (tr ("Create new access template"));
    tbTemplActions->addAction (actTemplAdd);

    actTemplEdit = new QAction (QIcon (":/ddoc/edit.png"), tr ("Edit"), this);
    actTemplEdit->setToolTip (tr ("Edit access template"));
    tbTemplActions->addAction (actTemplEdit);

    actTemplApply = new QAction (QIcon(":/ddoc/apply.png"), tr("A&pply"), this);
    actTemplApply->setToolTip (tr ("Apply selected access template"));
    tbTemplActions->addAction (actTemplApply);

    actTemplSave = new QAction (QIcon(":/ddoc/accept.png"), tr ("Save &As..."), this);
    actTemplSave->setToolTip (tr ("Save access template"));
    tbTemplActions->addAction (actTemplSave);
    actTemplSave->setVisible (false);

    actTemplRefresh = new QAction (QIcon(":/ddoc/reload.png"), tr ("&Refresh"), this);
    actTemplRefresh->setToolTip (tr ("Refresh templates list"));
    tbTemplActions->addAction (actTemplRefresh);

    actTemplDel = new QAction (QIcon(":/ddoc/delete.png"), tr ("&Delete"), this);
    actTemplDel->setToolTip (tr ("Delete selected template"));
    tbTemplActions->insertAction (actTemplApply, actTemplDel);

    vLay->addWidget (lvPrivilegies);

    gLay->addLayout (vLay, 1, 1, 1, 1);
    if (!m_acl)
    {
//        gbFilters->setVisible (false);
        tvAccessTemplates->setVisible (false);
        tbTemplActions->setVisible (false);
        lvPrivilegies->setVisible (false);
    }

    connect (actTemplAdd, SIGNAL (triggered()), this, SLOT (addTemplate()) );
    connect (actTemplEdit, SIGNAL (triggered()), this, SLOT (editTemplate()) );
    connect (actTemplApply, SIGNAL (triggered()), this, SLOT (applyTemplate()) );
    connect (actTemplSave, SIGNAL (triggered()), this, SLOT (saveAsTemplate()) );
    connect (actTemplRefresh, SIGNAL (triggered()), this, SLOT (refreshTemplates()) );
    connect (actTemplDel, SIGNAL (triggered()), this, SLOT (dropTemplate()) );

}

void KKSStuffForm :: initActions (void)
{
    actOrgAdd = new QAction (QIcon(":/ddoc/organization_add.png"), tr("Add organization"), this);
    tbActions->addAction (actOrgAdd);
    connect (actOrgAdd, SIGNAL (triggered()), this, SLOT (createOrganization()) );

    actOrgEdit = new QAction (QIcon(":/ddoc/organization_edit.png"), tr("Edit organization"), this);
    tbActions->addAction (actOrgEdit);
    connect (actOrgEdit, SIGNAL (triggered()), this, SLOT (editOrganization()) );

    actOrgDel = new QAction (QIcon(":/ddoc/organization_remove.png"), tr("Delete organization"), this);
    tbActions->addAction (actOrgDel);
    connect (actOrgDel, SIGNAL (triggered()), this, SLOT (dropOrganization()) );

    actOrgAddr = new QAction (QIcon (":/ddoc/organization_address.png"), tr ("Set organization address(es)"), this);
    tbActions->addAction (actOrgAddr);
    connect (actOrgAddr, SIGNAL (triggered()), this, SLOT (setOrganizationAddress()) );
    tbActions->addSeparator ();

    actDivAdd = new QAction (QIcon(":/ddoc/unit_add.png"), tr("Add subdivision"), this);
    tbActions->addAction (actDivAdd);
    connect (actDivAdd, SIGNAL (triggered ()), this, SLOT (createDivision()) );

    actDivEdit = new QAction (QIcon(":/ddoc/unit_edit.png"), tr("Edit subdivision"), this);
    tbActions->addAction (actDivEdit);
    connect (actDivEdit, SIGNAL (triggered ()), this, SLOT (editDivision()) );

    actDivDel = new QAction (QIcon(":/ddoc/unit_delete.png"), tr("Delete subdivision"), this);
    tbActions->addAction (actDivDel);
    connect (actDivDel, SIGNAL (triggered()), this, SLOT (dropDivision()) );

    tbActions->addSeparator ();

    actPosAdd = new QAction (QIcon(":/ddoc/position_add.png"), tr("Add position"), this);
    tbActions->addAction (actPosAdd);
    connect (actPosAdd, SIGNAL (triggered()), this, SLOT (createPosition()) );

    actPosEdit = new QAction (QIcon(":/ddoc/position_edit.png"), tr("Edit position"), this);
    tbActions->addAction (actPosEdit);
    connect (actPosEdit, SIGNAL (triggered()), this, SLOT (editPosition()) );

    actPosDel = new QAction (QIcon(":/ddoc/position_delete.png"), tr("Delete position"), this);
    tbActions->addAction (actPosDel);
    connect (actPosDel, SIGNAL (triggered()), this, SLOT (dropPosition()) );

    tbActions->addSeparator ();

    actUserAdd = new QAction (QIcon(":/ddoc/user_add.png"), tr("Add user"), this);
    tbActions->addAction (actUserAdd);
    connect (actUserAdd, SIGNAL (triggered()), this, SLOT (createUser()) );

    actUserEdit = new QAction (QIcon(":/ddoc/user_edit.png"), tr("Edit user"), this);
    tbActions->addAction (actUserEdit);
    connect (actUserEdit, SIGNAL (triggered()), this, SLOT (editUser()) );

    actUserDel = new QAction (QIcon(":/ddoc/user_delete.png"), tr("Delete user"), this);
    tbActions->addAction (actUserDel);
    connect (actUserDel, SIGNAL (triggered()), this, SLOT (dropUser()) );

    tbActions->addSeparator ();
    actUserToPos = new QAction (QIcon (":/ddoc/user_to_pos.png"), tr ("Set user onto position"), this);
    tbActions->addAction (actUserToPos);
    connect (actUserToPos, SIGNAL (triggered()), this, SLOT (setUserOntoPosition()) );

    actUserFromPos = new QAction (QIcon (":/ddoc/user_from_pos.png"), tr ("Dismiss user from position"), this);
    tbActions->addAction (actUserFromPos);
    connect (actUserFromPos, SIGNAL (triggered()), this, SLOT (dismissUserFromPosition()) );
    tbActions->addSeparator ();

    actStuffRefresh = new QAction (QIcon(":/ddoc/reload.png"), tr ("&Refresh stuff structure"), this);
    tbActions->addAction (actStuffRefresh);
    connect (actStuffRefresh, SIGNAL (triggered()), this, SLOT (refreshStruct()) );

}

void KKSStuffForm :: initModalWidget (void)
{
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (lESearch, 0, 0, 1, 1);
    QSplitter * spViews = new QSplitter (Qt::Vertical, this);
    spViews->addWidget (tvStuffStruct);
    spViews->addWidget (lvPrivilegies);
    gLay->addWidget (spViews, 1, 0, 1, 1);
    //gLay->addWidget (lvPrivilegies, 2, 0, 1, 1);
    QHBoxLayout * hLay = new QHBoxLayout ();
    hLay->addStretch ();
    hLay->addWidget (pbOk);
    hLay->addWidget (pbCancel);
    gLay->addLayout (hLay, 3, 0, 1, 1);
    tvAccessTemplates->setVisible (false);
    lESearch->setVisible (true);
    if (m_acl && m_acl->isNamed())
        lESearch->setText (dynamic_cast<KKSAclTemplate *>(m_acl)->name());
    connect (lESearch, SIGNAL (textChanged ( const QString &)), this, SLOT (setTemplateName (const QString&)) );
    chRoles->setVisible (false);
    this->initActions();
    this->tbActions->setVisible (false);
}

int KKSStuffForm :: getDetailLevel (void) const
{
    return detailLevel;
}

void KKSStuffForm :: refreshStruct (void)
{
    QItemSelection sel = tvStuffStruct->selectionModel ()->selection();
    QItemSelection sourceSel;
    if (tvStuffStruct->selectionModel () && !m_acl)
    {
        sourceSel = stuffSortModel->mapSelectionToSource (sel);
        emit refreshStuff (m_acl, detailLevel, stuffSortModel->sourceModel (), sourceSel, tvStuffStruct);
        QHeaderView * hv = tvStuffStruct->header ();
        stuffSortModel->sort (0, hv->sortIndicatorOrder ());
    }
    else
        emit refreshStuff (m_acl, detailLevel, tvStuffStruct->model (), sel, tvStuffStruct);
}

void KKSStuffForm :: accept (void)
{
    done (QDialog::Accepted);
    close ();
}

void KKSStuffForm :: reject (void)
{
    done (QDialog::Rejected);
    close ();
}

int KKSStuffForm :: result (void) const
{
    return res;
}

int KKSStuffForm :: exec (void)
{
    if (pEventL)
    {
        qWarning("QDialog::exec: Recursive call detected");
        return -1;
    }
    bool deleteOnClose = testAttribute(Qt::WA_DeleteOnClose);
    setAttribute (Qt::WA_DeleteOnClose, false);

    bool wasShowModal = testAttribute(Qt::WA_ShowModal);
    setAttribute (Qt::WA_ShowModal, true);
    res = 0;

    this->show ();
    if (pEventL)
    {
        if (pEventL->isRunning())
            pEventL->exit (0);
        delete pEventL;
        pEventL = 0;
    }
    pEventL = new QEventLoop();//&eventLoop;
    QPointer<QWidget> guard = this;

    if (!guard.isNull () && pEventL)
        pEventL->exec();

    if (guard.isNull())
    {
        pEventL = 0;
        return QDialog::Rejected;
    }
    setAttribute(Qt::WA_ShowModal, wasShowModal);

//    int res = result();
    if (deleteOnClose)
        delete this;
    return res;

}

void KKSStuffForm :: done (int r)
{
    hide ();
    res = r;
    emit finished (r);
    if (pEventL && pEventL->isRunning())
        pEventL->exit (0);

    if (r == QDialog::Accepted)
        emit accepted();
    else if (r == QDialog::Rejected)
        emit rejected();
}
/*
KKSAclTemplate * KKSStuffForm :: getAclTemplate (void) const
{
    return aclTemplate;
}

void KKSStuffForm :: setAcltemplate (KKSAclTemplate * aclT)
{
    if (aclTemplate)
        aclTemplate->release();

    aclTemplate = aclT;

    if (aclTemplate)
        aclTemplate->addRef ();
}
*/
void KKSStuffForm :: setTemplateName (const QString& aclName)
{
    if (m_acl && m_acl->isNamed())
        dynamic_cast<KKSAclTemplate *>(m_acl)->setName (aclName);
}

void KKSStuffForm :: setAclReadOnly (bool isReadOnly)
{
 //   if (m_acl && idCUser != io->author() && idCUser != 1 )
//    {
    this->lvPrivilegies->setEnabled (!isReadOnly);
    this->tbTemplActions->setEnabled (!isReadOnly);
//    }
    
}

bool KKSStuffForm :: isAclReadOnly (void) const
{
    if (m_acl && m_acl->isNamed())
        return false;
    bool isLvPriv = this->lvPrivilegies->isEnabled ();
    bool isTemplA = this->tbTemplActions->isEnabled();
    return !isLvPriv || !isTemplA;
}

const KKSAccessEntity * KKSStuffForm :: getAccessEntity (void) const
{
    return m_acl;
}

KKSAccessEntity * KKSStuffForm :: getAccessEntity (void)
{
    return m_acl;
}

void KKSStuffForm :: setAccessEntity (KKSAccessEntity * acl)
{
    if (m_acl)
        m_acl->release ();

    m_acl = acl;

    if (m_acl)
        m_acl->addRef ();
}