#if !defined(__KKSSITOOOM_KKSSyncWidget_h)
#define __KKSSITOOOM_KKSSyncWidget_h

#include "kksgui_config.h"

#include <QWidget>
#include <QMap>

class QAbstractItemModel;
class QCheckBox;
class QLineEdit;
class QToolButton;
class QGroupBox;
class QTreeView;
class QGridLayout;
class QModelIndex;
class QToolBar;
class QAction;
class QLabel;

class KKSAttribute;
class KKSSortFilterProxyModel;
class KKSAttrValue;

class _GUI_EXPORT KKSSyncWidget : public QWidget
{
public:
    KKSSyncWidget (QWidget * parent=0, bool accessOk = true, Qt::WFlags f=0);//accessOk = false, если у пользователя нету права синхронизировать данный ИО
    virtual ~KKSSyncWidget (void);

    bool isCheckGlobal (void) const;
    void setCheckGlobal (bool global);

    void setSyncType (const QString& syncName);

    void setSyncOrgModel (QAbstractItemModel * sMod);
    void setSyncAttrVal (KKSAttrValue * attrVal);

    KKSSortFilterProxyModel * getSortModel (void) const;

private slots:
    void setGlobal (int state);
    void setSyncType (void);
    void addSyncOrg (void);
    void delSyncOrg (void);

signals:
    void setIsGlobal (bool isSync);
    void setSyncType (QLineEdit * leType);
    void addSyncOrganization (KKSAttrValue * av, QAbstractItemModel * sMod);
    void delSyncOrganization (KKSAttrValue * av, const QModelIndex& oInd, QAbstractItemModel * sMod);

private:
    //
    // Functions
    //
    void setupWidget (void);

private:
    //
    // Variables
    //
    QCheckBox * chGlobal;
    QLabel * lSyncType;
    QLineEdit * lESyncType;
    QToolButton * tbSyncType;
    QAction * actSyncType;
    QGroupBox * gbSyncOrg;
    QToolBar * tbActions;
    QAction * actAdd;
    QAction * actDel;
    QTreeView * tvSync;
    QGridLayout * gLay;
    
    bool m_accessOk;

    KKSAttrValue * syncAttrVal;
    KKSSortFilterProxyModel * sortModel;

private:
    Q_OBJECT
};

#endif
