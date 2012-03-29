/***********************************************************************
 * Module:  KKSStuffFactory.h
 * Author:  yuriyrusinov
 * Modified: 20 мая 2009 г. 12:40:51
 * Purpose: Declaration of the class KKSStuffFactory
 * Comment: данный класс является фабрикой управления ОШС
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSStuffFactory_h)
#define __KKSSITOOOM_KKSStuffFactory_h

#include "kksfactory_config.h"

#include "KKSEntityFactory.h"

#include <QModelIndex>
#include <QMap>

class QAbstractItemModel;
class QModelIndex;
class QTreeView;
class QItemSelection;
class QItemSelectionModel;
class QWidget;

class KKSLoader;
class KKSStuffForm;
class KKSObjEditorFactory;
class KKSPPFactory;

class KKSObject;
class KKSDatabase;
class KKSPrivilege;
class KKSObjectExemplar;
class KKSAclTemplate;
class KKSAccessEntity;

class _F_EXPORT KKSStuffFactory : public KKSEntityFactory
{
    public:
        KKSStuffForm * createStuffEditorForm (int level=-1, QWidget * parent=0, Qt::WindowFlags f=0);
        KKSStuffForm * createStuffEditorForm (KKSAccessEntity * acl, int idAuthor, QWidget * parent=0, Qt::WindowFlags f=0);

        enum Roles
        {
            atUnknown = -1,
            atBoss = 1,
            atGroup = 2,
            atOthers = 3,
            atExceptions = 4
        };

        enum StuffStructure
        {
            atOrganization = 5,
            atUnit = 6,
            atPosition = 7,
            atUser = 8
        };

        QAbstractItemModel * getStuffModel (const KKSAccessEntity * acl=0, int idAuthor=-1, int level=-1) const;

        KKSDatabase * getDb (void) const;

    private slots:
        void createAclTemplate (QAbstractItemModel *mod);
        void editAclTemplate (int idAclTemplate, QAbstractItemModel *mod);
        void dropAclTemplate (int id, QAbstractItemModel *mod, int row);
        void loadTemplates (QAbstractItemModel * mod);
        //void updatePriv (const QModelIndex& stIndex, KKSObject * obj, QAbstractItemModel * privMod);
        //void updateAclPriv (const QModelIndex& stIndex, KKSAclTemplate * aclT, QAbstractItemModel * privMod);
        void setIOPrivilege (KKSPrivilege *p, KKSObject * obj);

        void addStuffElement (const QModelIndex& parent, int entity, QAbstractItemModel * stuffModel);
        void editStuffElement (const QModelIndex& index, int entity, QAbstractItemModel * stuffModel);
        void delStuffElement (const QModelIndex& index, int entity, QAbstractItemModel * stuffModel);

        void setOrgAddress (const QModelIndex& orgIndex, QAbstractItemModel * stuffModel);
        void setUserToPosition (const QModelIndex& posIndex, QAbstractItemModel * stuffModel);
        void dismissUserFromPosition (const QModelIndex& posIndex, QAbstractItemModel * stuffModel);

        void refreshStuffStruct (KKSAccessEntity * acl, int level, QAbstractItemModel * stuffModel, const QItemSelection& sel, QTreeView * tv);
        void refreshEntity (KKSObjectExemplar * wObjE);
        void applyAclTemplateIO (int idAclTemplate, KKSAccessEntity * acl);

    private:
        //
        // Functions
        //
        friend class KKSSito;
        KKSStuffFactory (KKSLoader *l=0, KKSPPFactory * _ppf=0, KKSObjEditorFactory * _oef=0, KKSDatabase *_db=0);
        ~KKSStuffFactory (void);

        void initStuffForm (KKSStuffForm * sForm, int idAuthor, int level=-1);
        void initTemplatesModel (KKSStuffForm * sForm);
        int loadModelChilds (QAbstractItemModel * mod, int id, int id_parent, StuffStructure role, int nRow, QModelIndex& ind, int max_role=-1, const QModelIndex& parent=QModelIndex(), QItemSelectionModel * selModel=0, int userdata=-1, int entdata=-1) const;
        QModelIndex initStuffModel (QAbstractItemModel *stuffModel, int idAuthor=-1, const KKSAccessEntity * acl=0, int level=-1, QItemSelectionModel * selModel=0, int userdata=-1, int entdata=-1) const;

    private:
        //
        // Variables
        //
        KKSLoader * loader;
        KKSPPFactory * ppf;
        KKSObjEditorFactory * oef;
        KKSDatabase *db;

        QMap<QWidget *, QAbstractItemModel *> wModels;

    private:
        Q_OBJECT
};
#endif
