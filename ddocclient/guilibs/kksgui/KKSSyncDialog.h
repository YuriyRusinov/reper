#ifndef _KKSSYNC_DIALOG_H
#define _KKSSYNC_DIALOG_H

#include <QDialog>

#include "kksgui_config.h"

class QPushButton;
class QLineEdit;
class QAbstractItemModel;
class QModelIndex;

class KKSAttrValue;
class KKSSyncWidget;

class _GUI_EXPORT KKSSyncDialog : public QDialog
{
public:
    KKSSyncDialog (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~KKSSyncDialog (void);

    KKSSyncWidget * getSyncWidget (void) const;

private slots:
    void setIOGlobal (bool isSync);
    void setSyncType (QLineEdit * leType);
    void addSyncOrg (KKSAttrValue * av, QAbstractItemModel * sMod);
    void delSyncOrg (KKSAttrValue * av, const QModelIndex& oInd, QAbstractItemModel * sMod);
    
signals:
    void loadSyncRef (QString tableName, QWidget * attrW, qint64 attrId);
    void loadAddSyncOrg (KKSAttrValue * av, QAbstractItemModel * sMod);
    void loadDelSyncOrg (KKSAttrValue * av, const QModelIndex& oInd, QAbstractItemModel * sMod);

private:
    //
    // Variables
    //
    KKSSyncWidget * syncW;
    QPushButton * pbOk;
    QPushButton * pbCancel;
    bool m_sync;

private:
    Q_OBJECT
};

#endif
