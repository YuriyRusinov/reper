#ifndef _KKSSTUFF_FORM_H
#define _KKSSTUFF_FORM_H

#include <QWidget>
#include <kksgui_config.h>

class QModelIndex;
class QMenu;
class QAbstractItemModel;
class QTreeView;
class QAbstractItemView;
class QToolBar;
class QLineEdit;
class QCheckBox;
class QListView;
class QItemSelection;
class QSortFilterProxyModel;
class QAction;
class QPushButton;
class QEventLoop;

class KKSObject;
class KKSPrivilege;
class KKSAccessEntity;
class KKSAclTemplate;

class _GUI_EXPORT KKSStuffForm : public QWidget
{
    public:
        KKSStuffForm (KKSAccessEntity * acl,
                      int idCUser,
                      QWidget *parent=0,
                      Qt::WindowFlags flags=0);

	KKSStuffForm (int idCUser,
	              int level=-1,
	              QWidget * parent=0,
	              Qt::WindowFlags flags=0);

        KKSStuffForm (KKSAclTemplate * at,
                      int idCUser,
                      QWidget *parent=0,
                      Qt::WindowFlags flags=0);

        virtual ~KKSStuffForm (void);

        enum StuffEntity
        {
            atUnknownEntity = -1,
            atBoss = 1,
            atGroup = 2,
            atOthers = 3,
            atExceptions = 4,
            atOrganization = 5,
            atDivision = 6,
            atPosition = 7,
            atUser = 8
        };

        void save (void);
        //KKSObject * getObj (void) const;
        const KKSAccessEntity * getAccessEntity (void) const;
        KKSAccessEntity * getAccessEntity (void);
        void setAccessEntity (KKSAccessEntity * acl);

        void setStuffModel (QAbstractItemModel * stModel);
        void setTemplateModel (QAbstractItemModel * tModel);
        QAbstractItemView * getPrivView (void) const;

        void setAclReadOnly (bool isReadOnly);
        bool isAclReadOnly (void) const;

        //KKSAclTemplate * getAclTemplate (void) const;
        //void setAcltemplate (KKSAclTemplate * aclT);
        int getDetailLevel (void) const;
        int result (void) const;

    public slots:
        void refreshStruct (void);
        void accept (void);
        void reject (void);
        int exec (void);
        void done (int r);

    private slots:
        void currentEntityChanged (const QModelIndex& current, const QModelIndex& previous);
        //void currentPrivilegiesChanged (const QModelIndex& index);
        void currentTemplateChanged (const QModelIndex& current, const QModelIndex& previous);
        void ContextMenuRequest(const QPoint& pos);

        void createOrganization (void);
        void editOrganization (void);
        void dropOrganization (void);
        void setOrganizationAddress (void);

        void createDivision (void);
        void editDivision (void);
        void dropDivision (void);

        void createPosition (void);
        void editPosition (void);
        void dropPosition (void);

        void createUser (void);
        void editUser (void);
        void dropUser (void);
        void setUserOntoPosition (void);
        void dismissUserFromPosition (void);

        void addTemplate (void);
        void editTemplate (void);
        void applyTemplate (void);
        void saveAsTemplate (void);
        void refreshTemplates (void);
        void dropTemplate (void);

        void setTemplateName (const QString& aclName);
        void setPrivilegeForStuffEntity (const QModelIndex& ind);

    signals:
        //void privilegiesChanged (KKSPrivilege *, KKSObject *);
        void accessRulesChanged (KKSAccessEntity *);

        void deleteAclTemplate (int id, QAbstractItemModel *mod, int row);
        void refreshAclTemplates (QAbstractItemModel *mod);
        //void refreshPrivilegies (const QModelIndex& index, KKSObject * obj, QAbstractItemModel * privModel);
        //void refreshAclPrivilegies (const QModelIndex& index, KKSAclTemplate * aclT, QAbstractItemModel * privModel);
        void addAclTemplate (QAbstractItemModel *aclTemplMod);
        void editAclTemplate (int idAclTemplate, QAbstractItemModel *aclTemplMod);
        void applyAclTemplate (int idAclTemplate, KKSAccessEntity * wAcl);

        void addStuffEntity (const QModelIndex& parent, int entity, QAbstractItemModel * stuffModel);
        void editStuffEntity (const QModelIndex& index, int entity, QAbstractItemModel * stuffModel);
        void delStuffEntity (const QModelIndex& index, int entity, QAbstractItemModel * stuffModel);
        void setOrgAddr (const QModelIndex& orgIndex, QAbstractItemModel * stuffModel);
        void setUserToPos (const QModelIndex& posIndex, QAbstractItemModel * stuffModel);
        void dropUserFromPos (const QModelIndex& posIndex, QAbstractItemModel * stuffModel);

        void refreshStuff (KKSAccessEntity * acl, int level, QAbstractItemModel * stuffModel, const QItemSelection& sel, QTreeView * tv);

        void accepted ();
        void rejected ();
        void finished (int r);
        void aboutToClose();

    private:
        //
        // Functions
        //
        void initPrivilegiesModel (void);
        void initPopupMenu (void);
        void initTvModel (QTreeView * tv, QAbstractItemModel * nModel, const char* method);
        void initWidget (void);
        void initModalWidget (void);
        void initActions (void);

    private:
        //
        // Variables
        //
        //Ui::kksstuff_form *ui;
        QTreeView * tvStuffStruct;
        QLineEdit * lESearch;
        QCheckBox * chRoles;
        QTreeView * tvAccessTemplates;
        QListView * lvPrivilegies;
        QToolBar * tbActions;
        QToolBar * tbTemplActions;
        QMenu *stuffPopupMenu;
        QSortFilterProxyModel * stuffSortModel;
        KKSAccessEntity * m_acl;
        //KKSObject * wObject;
        int idUser;
        int detailLevel;

        QAction * actStuffRefresh;

        QAction * actOrgAdd;
        QAction * actOrgEdit;
        QAction * actOrgDel;
        QAction * actOrgAddr;

        QAction * actDivAdd;
        QAction * actDivEdit;
        QAction * actDivDel;

        QAction * actPosAdd;
        QAction * actPosEdit;
        QAction * actPosDel;

        QAction * actUserAdd;
        QAction * actUserEdit;
        QAction * actUserDel;

        QAction * actUserToPos;
        QAction * actUserFromPos;

        QAction * actTemplAdd;
        QAction * actTemplApply;
        QAction * actTemplSave;
        QAction * actTemplRefresh;
        QAction * actTemplDel;
        QAction * actTemplEdit;

        QPushButton * pbOk;
        QPushButton * pbCancel;
        int res;
        QEventLoop *pEventL;

        //KKSAclTemplate * aclTemplate;
    private:
        Q_OBJECT
};

#endif
