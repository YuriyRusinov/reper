#include <QWidget>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QAbstractProxyModel>
#include <QAbstractItemView>
#include <QMessageBox>
#include <QTreeView>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QItemSelection>
#include <QtDebug>

#include "KKSObjEditorFactory.h"
#include "KKSPPFactory.h"
#include "KKSStuffFactory.h"
#include <KKSObjEditor.h>
#include <KKSCheckableModel.h>
#include "KKSLoader.h"
#include <KKSObject.h>
#include <KKSPrivilege.h>
#include <kksresult.h>
#include <kksstuffform.h>
#include <KKSObjectExemplar.h>
#include "kksdatabase.h"
#include "KKSAttribute.h"
#include <KKSAclTemplate.h>
#include <KKSAccessEntity.h>
#include <KKSOrganizationAddrForm.h>
#include <KKSOrgModel.h>
#include <KKSOrgItemDelegate.h>
#include <defines.h>

#include <QMessageBox>


KKSStuffFactory :: KKSStuffFactory (KKSLoader *l, KKSPPFactory * _ppf, KKSObjEditorFactory * _oef, KKSDatabase *_db)
    : loader (l),
    ppf (_ppf),
    oef (_oef),
    db (_db)
{
    connect (oef, SIGNAL (cioSaved (KKSObjectExemplar *)), \
             this, \
             SLOT (refreshEntity (KKSObjectExemplar *))
            );
}

KKSStuffFactory :: ~KKSStuffFactory (void)
{
    wModels.clear ();
}

KKSDatabase * KKSStuffFactory :: getDb (void) const
{
    return db;
}

KKSStuffForm * KKSStuffFactory :: createStuffEditorForm (int level, QWidget * parent, Qt::WindowFlags f)
{
    KKSStuffForm * sForm = new KKSStuffForm (-1, level, parent, f);
    this->initStuffForm (sForm, 1, level);
    this->initTemplatesModel (sForm);
    return sForm;
}

KKSStuffForm * KKSStuffFactory :: createStuffEditorForm (KKSAccessEntity * acl, int idAuthor, QWidget * parent, Qt::WindowFlags f)
{
    KKSStuffForm * sForm = new KKSStuffForm (acl, loader->getUserId (), parent, f);
    this->initStuffForm (sForm, idAuthor);
    this->initTemplatesModel (sForm);
    bool isRW = (acl && loader->getUserId () == idAuthor) || (acl && loader->getUserId ()==1);
    sForm->setAclReadOnly (!isRW);
    return sForm;
}

QAbstractItemModel * KKSStuffFactory :: getStuffModel (const KKSAccessEntity * acl, int idAuthor, int level) const
{
    QStandardItemModel *stModel = new QStandardItemModel ((level < 0 ? 4 : 0), 1);
    QString header = (acl ? tr("Stuff privilegies") : tr("Organization(s)/division(s)/position(s)/user(s)"));
    stModel->setHeaderData (0, Qt::Horizontal, header, Qt::DisplayRole);
    this->initStuffModel (stModel, idAuthor, acl, level);
    return stModel;
}

QModelIndex KKSStuffFactory :: initStuffModel (QAbstractItemModel * stModel, int idAuthor, const KKSAccessEntity * acl, int level, QItemSelectionModel * selModel, int userdata, int entdata) const
{
    if (stModel->rowCount () > 0 && level > 0)
        stModel->removeRows (0, stModel->rowCount());
    else if (level < 0)
    {
        for (int i=0; i<4; i++)
        {
            QModelIndex wIndex = stModel->index (i, 0);
            int nrows = stModel->rowCount (wIndex);
            stModel->removeRows (0, nrows, wIndex);
        }
    }
    QModelIndex sourceIndex;
    if (level < 0)
    {
        QStringList sRoles;
        sRoles << tr ("Boss") << tr ("Unit") << tr ("Others") << tr ("Exceptions");
        QList<int> lRoles;
        lRoles << atBoss << atGroup << atOthers << atExceptions;
        for (int i=0; i<4; i++)
        {
            QModelIndex wIndex = stModel->index (i, 0);
            QMap<int, QString> entList;
            if (acl)
            {
                //qDebug () << __PRETTY_FUNCTION__ << io->author();
                switch (lRoles[i])
                {
                    case atBoss:
                        {
                            entList = loader->getBossNamesList (idAuthor);
                            stModel->setData (wIndex, loader->getMyBoss(), Qt::UserRole);
                            stModel->setData (wIndex, QIcon(":/ddoc/position.png"), Qt::DecorationRole);
                        }
                        break;
                    case atGroup:
                        {
                            entList = loader->getUnitNamesList (idAuthor);
                            stModel->setData (wIndex, loader->getMyUnit(), Qt::UserRole);
                            stModel->setData (wIndex, QIcon(":/ddoc/unit.png"), Qt::DecorationRole);
                        }
                        break;
                    case atOthers: stModel->setData (wIndex, OTHERS_ROLE, Qt::UserRole); break;
                    default: break;
                }
            }
            else
            {
                switch (lRoles[i])
                {
                    case atBoss:
                        {
                            stModel->setData (wIndex, loader->getMyBoss(), Qt::UserRole);
                            stModel->setData (wIndex, QIcon(":/ddoc/position.png"), Qt::DecorationRole);
                        }
                        break;
                    case atGroup:
                        {
                            stModel->setData (wIndex, loader->getMyUnit(), Qt::UserRole);
                            stModel->setData (wIndex, QIcon(":/ddoc/unit.png"), Qt::DecorationRole);
                        }
                        break;
                    case atOthers: stModel->setData (wIndex, OTHERS_ROLE, Qt::UserRole); break;
                    default: break;
                }
                
            }

            if (!entList.isEmpty ())
            {
                stModel->insertRows (0, entList.count(), wIndex);
                stModel->insertColumns (0, 1, wIndex);
                int ii=0;
                for (QMap<int, QString>::const_iterator p=entList.constBegin(); p != entList.constEnd(); p++)
                {
                    QModelIndex eIndex = stModel->index (ii, 0, wIndex);
                    stModel->setData (eIndex, p.value(), Qt::DisplayRole);
                    stModel->setData (eIndex, p.key(), Qt::UserRole);
                    stModel->setData (eIndex, lRoles[i], Qt::UserRole+USER_ENTITY);
                    if (lRoles[i] == atBoss)
                        stModel->setData (eIndex, QIcon(":/ddoc/position.png"), Qt::DecorationRole);
                    else if (lRoles[i] == atGroup)
                        stModel->setData (eIndex, QIcon(":/ddoc/unit.png"), Qt::DecorationRole);
                    ii++;
                }
            }
            stModel->setData (wIndex, sRoles[i], Qt::DisplayRole);
            stModel->setData (wIndex, lRoles[i], Qt::UserRole+USER_ENTITY);
        }
        QModelIndex pIndex = stModel->index (stModel->rowCount()-1, 0);
        int norg = this->loadModelChilds (stModel, -1, -1, atOrganization, 0, sourceIndex, -1, pIndex, selModel, userdata, entdata );
        qDebug () << __PRETTY_FUNCTION__ << norg;
    }
    else
    {
        QModelIndex pIndex = QModelIndex();
        int nent = -1;
        nent=this->loadModelChilds (stModel, -1, -1, atOrganization, 0, sourceIndex, level+5, pIndex, selModel, userdata, entdata);
        qDebug () << __PRETTY_FUNCTION__ << nent;
    }
    return sourceIndex;

}

void KKSStuffFactory :: initStuffForm (KKSStuffForm * sForm, int idAuthor, int level)
{
    QAbstractItemModel *stModel = getStuffModel (sForm->getAccessEntity(), idAuthor, level);
    sForm->setStuffModel (stModel);

/*    connect (sForm, SIGNAL (refreshPrivilegies (const QModelIndex&, KKSObject *, QAbstractItemModel *)),
             this,
             SLOT (updatePriv (const QModelIndex&, KKSObject *, QAbstractItemModel *))
             );


    connect (sForm, SIGNAL (privilegiesChanged (KKSPrivilege *, KKSObject *)),
             this,
             SLOT (setIOPrivilege(KKSPrivilege *, KKSObject *))
             );
*/
    connect (sForm, SIGNAL (addStuffEntity (const QModelIndex&, int, QAbstractItemModel *)),
             this,
             SLOT (addStuffElement (const QModelIndex&, int, QAbstractItemModel *))
            );

    connect (sForm, SIGNAL (editStuffEntity (const QModelIndex&, int, QAbstractItemModel *)),
             this,
             SLOT (editStuffElement (const QModelIndex&, int, QAbstractItemModel *))
             );

    connect (sForm, SIGNAL (delStuffEntity (const QModelIndex&, int, QAbstractItemModel *)),
             this,
             SLOT (delStuffElement (const QModelIndex&, int, QAbstractItemModel *))
             );

    connect (sForm, SIGNAL (setOrgAddr (const QModelIndex&, QAbstractItemModel *)),
             this,
             SLOT (setOrgAddress (const QModelIndex&, QAbstractItemModel *))
            );

    connect (sForm, SIGNAL (refreshStuff (KKSAccessEntity *, int, QAbstractItemModel *, const QItemSelection&, QTreeView *)),
             this,
             SLOT (refreshStuffStruct (KKSAccessEntity *, int, QAbstractItemModel *, const QItemSelection&, QTreeView *))
            );

    connect (sForm, SIGNAL (applyAclTemplate (int, KKSAccessEntity *)),
             this,
             SLOT (applyAclTemplateIO (int, KKSAccessEntity *))
            );

    connect (sForm, SIGNAL (setUserToPos (const QModelIndex&, QAbstractItemModel *)),
             this,
             SLOT (setUserToPosition (const QModelIndex&, QAbstractItemModel *))
            );

    connect (sForm, SIGNAL (dropUserFromPos (const QModelIndex&, QAbstractItemModel *)),
             this,
             SLOT (dismissUserFromPosition (const QModelIndex&, QAbstractItemModel *))
            );
}

void KKSStuffFactory :: initTemplatesModel (KKSStuffForm * sForm)
{
    QStandardItemModel *tModel = new QStandardItemModel (0, 1);
    tModel->setHeaderData (0, Qt::Horizontal, tr ("Privilegies templates"), Qt::DisplayRole);
    this->loadTemplates (tModel);
    sForm->setTemplateModel (tModel);

    connect (sForm, SIGNAL (addAclTemplate (QAbstractItemModel *)), this, SLOT (createAclTemplate (QAbstractItemModel *)) );
    connect (sForm, SIGNAL (editAclTemplate (int, QAbstractItemModel *)), this, SLOT (editAclTemplate (int, QAbstractItemModel *)) );
    connect (sForm, SIGNAL (deleteAclTemplate(int, QAbstractItemModel*, int)), this, SLOT (dropAclTemplate (int, QAbstractItemModel*, int)) );
    connect (sForm, SIGNAL (refreshAclTemplates(QAbstractItemModel*)), this, SLOT (loadTemplates (QAbstractItemModel*)) );
}

void KKSStuffFactory :: loadTemplates (QAbstractItemModel * tModel)
{
    if (!tModel)
        return;

    if (tModel->rowCount ())
        tModel->removeRows (0, tModel->rowCount ());

    QString templSql ("select * from getacltemplates();");
    KKSResult *tRes = db->execute (templSql);
    if (!tRes)
        return;

    int n = tRes->getRowCount();
    tModel->insertRows (0, n);
    for (int i=0; i<n; i++)
    {
        QModelIndex wIndex = tModel->index (i, 0);
        bool isSystem = tRes->getCellAsBool (i, 2);
        QFont f = wIndex.data (Qt::FontRole).value <QFont>();
        f.setBold (isSystem);
        tModel->setData (wIndex, tRes->getCellAsString (i, 1), Qt::DisplayRole);
        tModel->setData (wIndex, tRes->getCellAsInt (i, 0), Qt::UserRole);
        tModel->setData (wIndex, isSystem, Qt::UserRole+1);
        tModel->setData (wIndex, f, Qt::FontRole);
    }

}

int KKSStuffFactory :: loadModelChilds (QAbstractItemModel * mod, int id, int id_parent, StuffStructure role, int nRow, QModelIndex& ind, int max_role, const QModelIndex& parent, QItemSelectionModel * selModel, int userdata, int entdata) const
{
    if (!mod || (max_role>0 && role>max_role))
        return 0;

    QString sqlQuery;
    switch (role)
    {
        case atOrganization: sqlQuery = QString ("select * from getOrganizations (%1) where type=0 and id_parent %2;")
                                               .arg (id<0 ? QString("null::int4") : QString::number (id))
                                               .arg (id_parent<0 ? QString("is null") : QString("=%1").arg (id_parent));
                             break;
        case atUnit: 
                     {
                         QModelIndex pOrgIndex = parent;
                         if (parent.data (Qt::UserRole+USER_ENTITY).toInt() == (int)atUnit && parent.parent().isValid())
                         {
                             while (pOrgIndex.data(Qt::UserRole+USER_ENTITY).toInt() == (int)atUnit)
                                 pOrgIndex = pOrgIndex.parent();
                             sqlQuery = QString ("select * from getUnits (%1, %2) where type=1 and id_parent %3;")
                                                .arg (pOrgIndex.data (Qt::UserRole).toInt())
                                                .arg (id)
                                                .arg (id_parent<0 ? QString("is null") : QString("=%1").arg (id_parent));
                         }
                         else
                         {
                             sqlQuery = QString ("select * from getUnits (%1, %2) where type=1 and id_parent %3;")
                                                .arg (id)
                                                .arg (id_parent<0 ? QString("null::int4") : QString::number (id_parent))
                                                .arg (id_parent<0 ? QString("is null") : QString("=%1").arg (id_parent));
                         }
                     }
                     break;
        case atPosition:
                     {
                         sqlQuery = QString ("select * from getDls (%1) where type=2;")
                                            .arg (id);
                     }
                     break;
        case atUser:
                     {
                         sqlQuery = QString ("select * from getUserDl (%1) where type=3;")
                                            .arg (id);
                     }
                     break;
        default: return OTHERS_ROLE; break;

    }
    qDebug () << __PRETTY_FUNCTION__ << sqlQuery;
    KKSResult * res = db->execute (sqlQuery);

    if (!res)
        return 0;

    int nres = res->getRowCount();
    if (!mod->insertRows (nRow, nres, parent) || (mod->columnCount (parent)==0 && !mod->insertColumns (0, 1, parent)))
    {
        delete res;
        return 0;
    }

    for (int i=nRow; i<nRow+nres; i++)
    {
        QModelIndex wIndex = mod->index (i, 0, parent);
        int id = res->getCellAsInt (i-nRow, 0);
        mod->setData (wIndex, res->getCellAsString (i-nRow, 2), Qt::DisplayRole);
        mod->setData (wIndex, id, Qt::UserRole);
        mod->setData (wIndex, role, Qt::UserRole+USER_ENTITY);
        if (role == atOrganization)
            mod->setData (wIndex, QIcon(":/ddoc/organization.png"), Qt::DecorationRole);
        else if (role == atUnit)
            mod->setData (wIndex, QIcon(":/ddoc/unit.png"), Qt::DecorationRole);
        else if (role == atUser)
            mod->setData (wIndex, QIcon(":/ddoc/user.png"), Qt::DecorationRole);
        else if (role == atPosition)
            mod->setData (wIndex, QIcon(":/ddoc/position.png"), Qt::DecorationRole);
        if (selModel && id == userdata && role==entdata)
            ind = wIndex;//selModel->select (wIndex, QItemSelectionModel::Select);
        
        int nsuborg = loadModelChilds (mod, id, id, role, 0, ind, max_role, wIndex, selModel, userdata, entdata);
        
        int id_parent = -1;//wIndex.parent().isValid() ? wIndex.parent().data (Qt::UserRole).toInt() : -1;
        qDebug () << __PRETTY_FUNCTION__ << wIndex << wIndex.parent();
        int nunits = loadModelChilds (mod, id, id_parent, (StuffStructure)(role+1), nsuborg, ind, max_role, wIndex, selModel, userdata, entdata);
        qDebug () << __PRETTY_FUNCTION__ << nunits;
//        if (role == atUnit)
//            loadModelChilds (mod, id, -1, atPosition, nunits, wIndex);
    }

    delete res;
    return nres;
}

void KKSStuffFactory :: createAclTemplate (QAbstractItemModel *mod)
{
    if (!mod)
        return;
    KKSAclTemplate * aclT = new KKSAclTemplate ();
    QWidget * parentWidget = qobject_cast <QWidget *>(this->sender());
    KKSStuffForm * sTForm = new KKSStuffForm (aclT, loader->getUserId (), parentWidget, Qt::Dialog);
    aclT->release();
/*    connect (sTForm, SIGNAL (refreshAclPrivilegies (const QModelIndex&, KKSAclTemplate *, QAbstractItemModel *)),
             this,
             SLOT (updateAclPriv (const QModelIndex&, KKSAclTemplate *, QAbstractItemModel *))
             );

    connect (sTForm, SIGNAL (privilegiesChanged (KKSPrivilege *, KKSObject *)),
             this,
             SLOT (setIOPrivilege(KKSPrivilege *, KKSObject *))
             );
*/
    QAbstractItemModel *stModel = getStuffModel (0, -1);
    sTForm->setStuffModel (stModel);
    int res = sTForm->exec();
    if (res == QDialog::Accepted)
    {
        qDebug () << __PRETTY_FUNCTION__;
        KKSAccessEntity * aclTR = sTForm->getAccessEntity();//getAclTemplate();
        int ier = ppf->insertAclTemplate (aclTR);
        if (ier < 0)
        {
            QMessageBox::warning (sTForm, tr ("Insert access template"), tr ("Cannot save access template db error"), QMessageBox::Ok, QMessageBox::NoButton);
            return;
        }
        this->loadTemplates (mod);
    }
    delete sTForm;
}

void KKSStuffFactory :: editAclTemplate (int idAclTemplate, QAbstractItemModel *mod)
{
     if (!mod)
        return;
    KKSAclTemplate * aclT = loader->loadAclTemplate (idAclTemplate);
    QWidget * parentWidget = qobject_cast <QWidget *>(this->sender());
    if (!aclT)
    {
        QMessageBox::warning (parentWidget, tr("Edit access template"), tr("Cannot load access template"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    KKSStuffForm * sTForm = new KKSStuffForm (aclT, loader->getUserId (), parentWidget, Qt::Dialog);
    aclT->release();
/*    connect (sTForm, SIGNAL (refreshAclPrivilegies (const QModelIndex&, KKSAclTemplate *, QAbstractItemModel *)),
             this,
             SLOT (updateAclPriv (const QModelIndex&, KKSAclTemplate *, QAbstractItemModel *))
             );

    connect (sTForm, SIGNAL (privilegiesChanged (KKSPrivilege *, KKSObject *)),
             this,
             SLOT (setIOPrivilege(KKSPrivilege *, KKSObject *))
             );
*/
    QAbstractItemModel *stModel = getStuffModel (0, -1);
    sTForm->setStuffModel (stModel);
    int res = sTForm->exec();
    if (res == QDialog::Accepted)
    {
        qDebug () << __PRETTY_FUNCTION__;
        KKSAccessEntity * aclTR = sTForm->getAccessEntity();
        int ier = ppf->updateAclTemplate (aclTR);
        if (ier < 0)
        {
            QMessageBox::warning (sTForm, tr ("Insert access template"), tr ("Cannot save access template db error"), QMessageBox::Ok, QMessageBox::NoButton);
            return;
        }
        this->loadTemplates (mod);
    }
    delete sTForm;
}

void KKSStuffFactory :: dropAclTemplate (int id, QAbstractItemModel *mod, int row)
{
    //int idUser = loader->getUserId();
    QString sql = QString ("select delAclTemplate (%1);").arg (id);

    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;

        return;
    }

    int ok = res->getCellAsInt (0, 0);
    
    delete res;
    
    if (ok != 1)
        return;

    if (mod)
        mod->removeRows (row, 1);
}

/*
void KKSStuffFactory :: updatePriv (const QModelIndex& stIndex, KKSObject * obj, QAbstractItemModel * privMod)
{
    if (!stIndex.isValid () || privMod->rowCount() < 5 || privMod->columnCount () < 1)
        return;

    int id = stIndex.data (Qt::UserRole).toInt();
    bool isAuthor = obj && ((id == obj->author ()) || (stIndex.child(0, 0).data (Qt::UserRole).toInt() == obj->author () ));
    bool isBoss = obj && (loader->getBossList (obj->author ()).contains (id) || (stIndex.parent().isValid() && loader->getBossList (obj->author ()).contains (stIndex.parent().data (Qt::UserRole).toInt())) );
    int idEntity = stIndex.parent().isValid () ? -1 : stIndex.data (Qt::UserRole+USER_ENTITY).toInt ();
    const KKSPrivilege * priv = obj ? obj->privilege (id) : 0;
    if (!priv)
    {
        switch (idEntity)
        {
            case atBoss: priv = obj ? obj->bossPrivilege (id) : 0; break;
            case atGroup: priv = obj ? obj->unitPrivilege (id) : 0; break;
            case atOthers: priv = obj ? obj->privilege (OTHERS_ROLE) : 0; break;
            default:
                   {
                       if (!obj)
                           break;
                       QList<int> bossList = loader->getBossList (obj->author ());
                       QList<int> unitList = loader->getUnitList (obj->author ());
                       if (isAuthor)
                           priv = new KKSPrivilege (obj->author(), true, true, true, true, true);
                       else if (isBoss)
                           priv = obj->bossPrivilege (bossList.contains(id) ? id : stIndex.parent().data (Qt::UserRole).toInt()) ;
                       else if (unitList.contains (id))
                           priv = obj->unitPrivilege (id);
                       else if (stIndex.parent().isValid () && stIndex.data (Qt::UserRole+USER_ENTITY).toInt () == atUser)
                           priv = obj->privilege (stIndex.parent().data (Qt::UserRole).toInt());
                       else if (stIndex.data (Qt::UserRole+USER_ENTITY) == atOthers)
                           priv = obj->privilege (OTHERS_ROLE);
                       else
                           priv = obj->privilege (id);
                       break;
                   }

        }
    }

    QList<bool> prL;
    if (priv)
    {
        prL << priv->allowReadList () << priv->allowRead () << priv->allowUpdate () << priv->allowDelete ();
        qDebug () << __PRETTY_FUNCTION__ << prL;
        if (isAuthor)
            priv->release ();
    }
    else if (obj && obj->privilege (OTHERS_ROLE))
        prL << obj->privilege (OTHERS_ROLE)->allowReadList () << obj->privilege (OTHERS_ROLE)->allowRead () << obj->privilege (OTHERS_ROLE)->allowUpdate () << obj->privilege (OTHERS_ROLE)->allowDelete ();
    else
        prL << true << true << false << false;

    for (int i=0; i<5; i++)
    {
        bool rule = prL[i];

        QModelIndex pIndex = privMod->index (i, 0);
        privMod->setData (pIndex, (rule ? Qt::Checked : Qt::Unchecked), Qt::CheckStateRole);
    }
}

void KKSStuffFactory :: updateAclPriv (const QModelIndex& stIndex, KKSAclTemplate * aclT, QAbstractItemModel * privMod)
{
    if (!stIndex.isValid() || !aclT || !privMod || privMod->rowCount() < 5 || privMod->columnCount () < 1)
        return;

    int idUser = loader->getUserId();
    if (idUser <= 0)
        return;
    int idEntity = stIndex.data (Qt::UserRole+USER_ENTITY).toInt();
    Q_UNUSED (idEntity);
    KKSPrivilege * cPriv = 0;
    bool privpars[5];
    for (int i=0; i<5; i++)
    {
        QModelIndex pIndex = privMod->index (0, 0);
        privpars[i] = (pIndex.data (Qt::CheckStateRole).toInt() == Qt::Checked);
    }

    int idRole = -1;
    cPriv = new KKSPrivilege (idRole, privpars[0], privpars[1], privpars[2], privpars[3], privpars[4]);
}
 */

void KKSStuffFactory :: setIOPrivilege (KKSPrivilege *p, KKSObject * obj)
{
    if (!p || !obj || !obj->getAccessRules())
        return;

    KKSAccessEntity * acl = obj->getAccessRules();
    QList<int> bossList (loader->getBossList (obj->author ()));
    qDebug () << __PRETTY_FUNCTION__ << bossList << p->idRole();
    if (acl->getPrivilegies().contains (p->idRole()))
        acl->replacePrivilege (p->idRole(), p);
    else
        acl->addPrivilege (p->idRole(), p);
    if (bossList.contains (p->idRole()))
        acl->setBossPrivilege (p->idRole(), p);
    else
    {
        QList<int> unitList (loader->getUnitList (obj->author()));
        if (unitList.contains (p->idRole()))
            acl->setUnitPrivilege (p->idRole(), p);
        else if (p->idRole() == OTHERS_ROLE)
            acl->setOthersPrivilege (p);
//        else
//            obj->addPrivilege (p->idRole(), p);
    }
}

void KKSStuffFactory :: addStuffElement (const QModelIndex& parent, int entity, QAbstractItemModel * stuffModel)
{
    qDebug () << __PRETTY_FUNCTION__;
    QWidget * editor = qobject_cast<QWidget *>(this->sender());
    KKSObject * io = 0;
    KKSCategory * c = 0;
    KKSMap<qint64, KKSAttrValue *> ioVals;
    KKSMap<qint64, KKSAttrValue *> aVals;
    int id = -1;
    int parent_entity = -1;
    if (parent.isValid())
    {
        id = parent.data (Qt::UserRole).toInt ();
        parent_entity = parent.data (Qt::UserRole+USER_ENTITY).toInt ();
    }
    switch (entity)
    {
        case atOrganization: {
            io = loader->loadIO (IO_ORG_ID, true);
            c = io->category();
            if (parent_entity < 0)
                break;
            KKSAttribute * orgParentAttr = loader->loadAttribute(ATTR_ID_PARENT);
            if (!orgParentAttr)
                break;
            KKSCategoryAttr * cOrgParentAttr = KKSCategoryAttr::create(orgParentAttr, true, true);
            if (!cOrgParentAttr)
                break;
            KKSValue val (QString::number (id), KKSAttrType::atParent);
            KKSAttrValue * av = new KKSAttrValue (val, cOrgParentAttr);
            if (av)
            {
                aVals.insert (orgParentAttr->id(), av);
                av->release ();
            }
            cOrgParentAttr->release ();
            orgParentAttr->release ();
            //io->release();
            break;
        }
        case atUnit: {
            io = loader->loadIO (IO_UNITS_ID, true);
            c = io->category();
            if (parent_entity == atOrganization)
            {
                KKSAttribute * orgAttr = loader->loadAttribute(ATTR_ID_ORGANIZATION);
                if (!orgAttr)
                    break;
                KKSCategoryAttr * cOrgAttr = KKSCategoryAttr::create(orgAttr, true, true);
                if (!cOrgAttr)
                    break;
                KKSValue val (QString::number (id), KKSAttrType::atList);
                KKSAttrValue * av = new KKSAttrValue (val, cOrgAttr);
                if (av)
                {
                    aVals.insert (orgAttr->id(), av);
                    av->release ();
                }
                cOrgAttr->release ();
                orgAttr->release ();
            }
            else if (parent_entity == atUnit)
            {
                //
                // Когда мы создаем подразделение --- часть другого подразделения,
                // то необходимы и организация и подразделение
                //
                KKSAttribute * orgAttr = loader->loadAttribute(ATTR_ID_ORGANIZATION);
                if (!orgAttr)
                    break;
                KKSCategoryAttr * cOrgAttr = KKSCategoryAttr::create(orgAttr, true, true);
                if (!cOrgAttr)
                    break;
                QModelIndex pIndex (parent);
                do {
                    pIndex = pIndex.parent();
                } while (pIndex.parent().isValid());
                int idOrg = pIndex.data (Qt::UserRole).toInt ();
                KKSValue orgVal (QString::number (idOrg), KKSAttrType::atList);
                KKSAttrValue * avOrg = new KKSAttrValue (orgVal, cOrgAttr);
                if (avOrg)
                {
                    aVals.insert (orgAttr->id(), avOrg);
                    avOrg->release ();
                }
                cOrgAttr->release ();
                orgAttr->release ();

                KKSAttribute * divParentAttr = loader->loadAttribute(ATTR_ID_PARENT);
                if (!divParentAttr)
                    break;
                KKSCategoryAttr * cDivParentAttr = KKSCategoryAttr::create(divParentAttr, true, true);
                if (!cDivParentAttr)
                    break;
                KKSValue val (QString::number (id), KKSAttrType::atParent);
                KKSAttrValue * av = new KKSAttrValue (val, cDivParentAttr);
                if (av)
                {
                    aVals.insert (divParentAttr->id(), av);
                    av->release ();
                }
                cDivParentAttr->release ();
                divParentAttr->release ();
            }
            break;
        }
        case atPosition: {
            io = loader->loadIO (IO_POS_ID, true);
            c = io->category();
            if (parent_entity == atUnit)
            {
                KKSAttribute * orgAttr = loader->loadAttribute(ATTR_ID_UNIT);
                if (!orgAttr)
                    break;
                KKSCategoryAttr * cOrgAttr = KKSCategoryAttr::create(orgAttr, true, true);
                if (!cOrgAttr)
                    break;
                KKSValue val (QString::number (id), KKSAttrType::atParent);
                KKSAttrValue * av = new KKSAttrValue (val, cOrgAttr);
                if (av)
                {
                    aVals.insert (orgAttr->id(), av);
                    av->release ();
                }
                cOrgAttr->release ();
                orgAttr->release ();
            }
            break;
        }
        case atUser: {
            io = loader->loadIO (IO_USERS_ID, true);
            c = io->category();
            break;
        }
    }
    if (!io)
        return;
    oef->createNewEditorParam (editor, io->id(), c, io->tableName(), 3, false, ioVals, aVals);
    if (editor && !wModels.contains (editor))
        wModels.insert (editor, stuffModel);
    io->release ();
}

void KKSStuffFactory :: editStuffElement (const QModelIndex& index, int entity, QAbstractItemModel * stuffModel)
{
    qDebug () << __PRETTY_FUNCTION__;
    QWidget * editor = qobject_cast<QWidget *>(this->sender());
    KKSObject * io = 0;
    KKSCategory * c = 0;
    switch (entity)
    {
        case atOrganization: {
            io = loader->loadIO (IO_ORG_ID, true);
            c = io->category();
            break;
        }
        case atUnit: {
            io = loader->loadIO (IO_UNITS_ID, true);
            c = io->category();
            break;
        }
        case atPosition: {
            io = loader->loadIO (IO_POS_ID, true);
            c = io->category();
            break;
        }
        case atUser: {
            io = loader->loadIO (IO_USERS_ID, true);
            c = io->category();
            break;
        }
    }
    if (!io)
        return;
    oef->editExistOE (editor, io->id(), index.data (Qt::UserRole).toInt(), c, io->tableName(), 3, false);
    if (editor && !wModels.contains (editor))
        wModels.insert (editor, stuffModel);
    io->release ();
}

void KKSStuffFactory :: delStuffElement (const QModelIndex& index, int entity, QAbstractItemModel * stuffModel)
{
    qDebug () << __PRETTY_FUNCTION__;
    QWidget * editor = qobject_cast<QWidget *>(this->sender());
    KKSObject * io = 0;
    switch (entity)
    {
        case atOrganization: {
            io = loader->loadIO (IO_ORG_ID, true);
            break;
        }
        case atUnit: {
            io = loader->loadIO (IO_UNITS_ID, true);
            break;
        }
        case atPosition: {
            io = loader->loadIO (IO_POS_ID, true);
            break;
        }
        case atUser: {
            io = loader->loadIO (IO_USERS_ID, true);
            break;
        }
    }
    qint64 idStuffEnt = index.data (Qt::UserRole).toInt();
    int ier = oef->deleteOE (editor, io->id(), idStuffEnt, io->tableName(), stuffModel, index);
/*    if (ier == OK_CODE && stuffModel)
    {
        int irow = index.row ();
        stuffModel->removeRows (irow, 1, index.parent());
    }
*/
    io->release();

}

void KKSStuffFactory :: setOrgAddress (const QModelIndex& orgIndex, QAbstractItemModel * stuffModel)
{
    if (!stuffModel || !orgIndex.isValid())
        return;
    qDebug () << __PRETTY_FUNCTION__;
    int idOrg = orgIndex.data (Qt::UserRole).toInt ();
    QString sql = QString ("select * from getOrgs(%1);").arg (idOrg);
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount() != 1)
    {
        if (res)
            delete res;
        return;
    }
    QString orgName = res->getCellAsString(0, 8);
    QString recvUID = res->getCellAsString(0, 11);
    
    QString trSql = QString ("select * from gettransportaddresses ('%1');").arg (recvUID);
    KKSResult * tRes = db->execute (trSql);
    
    if (!tRes)
    {
        delete res;
        return;
    }

    QMap<int, QString> transps;
    QMap<int, QString> addrs;
    QMap<int, bool> actives;
    QMap<int, int> ports;

    KKSOrgModel * orgTrModel = new KKSOrgModel (tRes->getRowCount(), 5);
    QStringList headers;
    headers << tr ("Transport set is enabled")
            << tr ("Transport name")
            << tr ("Transport address")
            << tr ("Transport port")
            << tr ("Transport is active");
    
    for (int i=0; i<5; i++)
        orgTrModel->setHeaderData (i, Qt::Horizontal, headers[i], Qt::DisplayRole);
    
    for (int i=0; i<tRes->getRowCount(); i++)
    {
        int idTr = tRes->getCellAsInt (i, 1);
        transps.insert (idTr, tRes->getCellAsString(i, 2));
        addrs.insert (idTr, tRes->getCellAsString(i, 3));
        actives.insert (idTr, tRes->getCellAsBool (i, 4));
        ports.insert (idTr, tRes->getCellAsInt (i, 5));

        QModelIndex orgIndex = orgTrModel->index (i, 0);
        orgTrModel->setData (orgIndex, idTr, Qt::UserRole);
        orgTrModel->setData (orgIndex, true, Qt::CheckStateRole);
        orgIndex = orgTrModel->index (i, 1);
        orgTrModel->setData (orgIndex, tRes->getCellAsString(i, 2), Qt::DisplayRole);
        orgIndex = orgTrModel->index (i, 2);
        orgTrModel->setData (orgIndex, tRes->getCellAsString(i, 3), Qt::DisplayRole);
        orgIndex = orgTrModel->index (i, 3);
        orgTrModel->setData (orgIndex, tRes->getCellAsInt (i, 5), Qt::DisplayRole);
        orgIndex = orgTrModel->index (i, 4);
        orgTrModel->setData (orgIndex, tRes->getCellAsBool (i, 4), Qt::CheckStateRole);
    }


    delete tRes;
    delete res;

    KKSOrganizationAddrForm * orgAForm = new KKSOrganizationAddrForm (idOrg, orgName, transps, addrs, actives, ports);
    
    QAbstractItemDelegate * orgIDeleg = new KKSOrgItemDelegate;
    orgAForm->setModel (orgTrModel);
    orgAForm->setItemDelegate (orgIDeleg);
    
    if (orgAForm->exec () == QDialog::Accepted)
    {
        QMap<int, QString> addresses = orgAForm->getAddrs();
        QMap<int, bool> actives = orgAForm->getActiveList();
        QMap<int, int> ports = orgAForm->getPortList();
        
        QString transpStr = QString ("ARRAY[");
        QString addrs = QString ("ARRAY[");
        QString actStr = QString ("ARRAY[");
        QString portStr = QString ("ARRAY[");
        
        int i=0;
        QMap<int, bool>::const_iterator pact = actives.constBegin();
        QMap<int, int>::const_iterator pport = ports.constBegin();
        
        for (QMap<int, QString>::const_iterator pa = addresses.constBegin();
                pa != addresses.constEnd();
                pa++)
        {

            transpStr += QString::number (pa.key());

            addrs += QString ("'%1'").arg (pa.value());
            actStr += QString ("%1").arg (pact != actives.constEnd() && pact.value() ? QString("true") : QString ("false"));
            portStr += QString ("%1").arg ((pport != ports.constEnd() && pport.value() <= 0) ? QString("NULL::int4") : QString::number (pport.value()));

            if (i < addresses.size()-1)
            {
                transpStr += QString(",");
                addrs += QString(",");
                actStr += QString (",");
                portStr += QString (",");
            }
            i++;
            pact++;
            pport++;
        }

        transpStr += QString("]");
        addrs += QString("]");
        actStr += QString("]");
        portStr += QString("]");

        QString orgAddrSql = QString ("select setOrganizationAddress (%1, %2, %3, %4, %5)")
                .arg (idOrg)
                .arg (transpStr)
                .arg (addrs)
                .arg (portStr)
                .arg (actStr);

        KKSResult * orgAddrRes = db->execute (orgAddrSql);
        if (!orgAddrRes || orgAddrRes->getRowCount() != 1)
        {
            if (orgAddrRes)
                delete orgAddrRes;
            QMessageBox::warning (orgAForm, tr ("Set organization address"), tr ("Error, cannot set address"), QMessageBox::Ok);
            delete orgAForm;
            return;
        }
        delete orgAddrRes;
    }
    delete orgAForm;
}

void KKSStuffFactory :: setUserToPosition (const QModelIndex& posIndex, QAbstractItemModel * stuffModel)
{
    qDebug () << __PRETTY_FUNCTION__ << posIndex << stuffModel;
    if (!posIndex.isValid() || !stuffModel || stuffModel->rowCount (posIndex) > 0)
        return;
    
    const KKSList<const KKSFilterGroup *> filters;
    KKSObject * wObj = loader->loadIO (IO_USERS_ID, true);
    KKSCategory * wCat = wObj->category()->tableCategory();
    QWidget * parentWidget = qobject_cast<QWidget *>(this->sender());
    KKSObjEditor * usersEditor = oef->createObjRecEditor(IO_IO_ID, IO_USERS_ID, filters, tr ("Set user onto position"), wCat, true, false, Qt::WindowModal, parentWidget, Qt::Dialog);
    if (usersEditor->exec() == QDialog::Accepted)
    {
        int idUser = usersEditor->getID();
        int idPos = posIndex.data (Qt::UserRole).toInt();
        QString sql = QString ("select setUserToPos (%1, %2);").arg (idPos).arg (idUser);
        KKSResult * res = db->execute (sql);
        if (!res || res->getRowCount() != 1 || res->getCellAsInt(0, 0) < 0)
        {
            if (res)
                delete res;
            QMessageBox::warning (parentWidget, tr ("Set user onto position"), tr ("Cannot set user %1 onto %2").arg (idUser).arg (idPos), QMessageBox::Ok);
            return;
        }
        delete res;
        KKSObjectExemplar * userCIO = loader->loadEIO(idUser, wObj, wCat, QString("users"));
        if (!userCIO)
            return;
        stuffModel->insertRows (0, 1, posIndex);
        if (stuffModel->columnCount (posIndex) == 0)
            stuffModel->insertColumns (0, 1, posIndex);
        QModelIndex wIndex = stuffModel->index (0, 0, posIndex);
        stuffModel->setData (wIndex, idUser, Qt::UserRole);
        stuffModel->setData (wIndex, userCIO->attrValue(ATTR_FIO)->value().value(), Qt::DisplayRole);
        stuffModel->setData (wIndex, atUser, Qt::UserRole+USER_ENTITY);
        userCIO->release ();
    }
    wObj->release ();
}

void KKSStuffFactory :: dismissUserFromPosition (const QModelIndex& posIndex, QAbstractItemModel * stuffModel)
{
    qDebug () << __PRETTY_FUNCTION__ << posIndex << stuffModel;
    if (!posIndex.isValid() || !stuffModel || stuffModel->rowCount (posIndex) == 0)
        return;

    int idPos = posIndex.data (Qt::UserRole).toInt();
    QString sql = QString ("select setUserToPos (%1, NULL);").arg (idPos);
    KKSResult * res = db->execute (sql);
    QWidget * parentWidget = qobject_cast<QWidget *>(this->sender());
    if (!res || res->getRowCount() != 1 || res->getCellAsInt(0, 0) < 0)
    {
        if (res)
            delete res;
        QMessageBox::warning (parentWidget, tr ("Dismiss user from position"), tr ("Cannot dismiss user from %1").arg (idPos), QMessageBox::Ok);
        return;
    }
    delete res;
    stuffModel->removeRows (0, stuffModel->rowCount (posIndex), posIndex);
}

void KKSStuffFactory :: refreshStuffStruct (KKSAccessEntity * acl, int level, QAbstractItemModel * stuffModel, const QItemSelection& sel, QTreeView * tv)
{
    if (!stuffModel || !tv)
        return;
    int userdata = -1;
    int entdata = -1;
    if (!sel.isEmpty())
    {
        userdata = sel.indexes().at(0).data (Qt::UserRole).toInt ();
        entdata = sel.indexes().at(0).data (Qt::UserRole+USER_ENTITY).toInt ();
    }

    QItemSelectionModel * selModel = tv->selectionModel();
    int idUser = loader->getUserId();
    QModelIndex sourceIndex = initStuffModel (stuffModel, idUser/*io->author()*/, acl/*io->getAccessRules()*/, level, selModel, userdata, entdata);
    if (qobject_cast<QAbstractProxyModel *>(tv->model()))
    {
        QModelIndex proxyInd = qobject_cast<QAbstractProxyModel *>(tv->model())->mapFromSource (sourceIndex);
        selModel->setCurrentIndex (proxyInd, QItemSelectionModel::ClearAndSelect);
    }
    QModelIndexList selIndexes = selModel->selectedIndexes ();
 
    for (int i=0; i<selIndexes.count(); i++)
    {
        QModelIndex pInd = selIndexes[i];
        while (pInd.parent().isValid())
        {
            tv->expand (pInd.parent());
            pInd = pInd.parent();
        }
    }
}

void KKSStuffFactory :: refreshEntity (KKSObjectExemplar * wObjE)
{
    int level = -1;
    switch (wObjE->io()->id ())
    {
        case IO_ORG_ID: level = 1; break;
        case IO_UNITS_ID: level = 2; break;
        case IO_POS_ID: level = 3; break;
        case IO_USERS_ID: level = 4; break;
        default: break;
    }
    for (QMap<QWidget *, QAbstractItemModel *>::const_iterator p = wModels.constBegin(); \
         p != wModels.constEnd(); \
         p++)
    {
        if (!p.key())
            continue;

        try{
            KKSStuffForm * sForm = qobject_cast <KKSStuffForm *>(p.key());
            if (sForm && level <= sForm->getDetailLevel())
                sForm->refreshStruct ();
        }
        catch(...){
            QMessageBox::critical(0, tr("Error"), tr("An internal error was occured during form refresh"));
        }
    }
}

void KKSStuffFactory :: applyAclTemplateIO (int idAclTemplate, KKSAccessEntity * acl)
{
    if (idAclTemplate <= 0)
        return;

    KKSAclTemplate * aclT = loader->loadAclTemplate(idAclTemplate);
    if (!aclT)
        return;

    KKSStuffForm * sForm = qobject_cast<KKSStuffForm*> (this->sender());
    if (sForm)
        sForm->setAccessEntity(aclT);
    //wObject->setAccessRules (aclT);
    Q_UNUSED (acl);

    aclT->release ();
}
