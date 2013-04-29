// -*- C++ -*-
/* 
 * File:   KKSOrganizationAddrWidget.h
 * Author: yuriy
 *
 * Created on 2 Февраль 2012 г., 20:25
 */

#ifndef _KKSORGANIZATIONADDRFORM_H
#define	_KKSORGANIZATIONADDRFORM_H

#include <QDialog>
#include <QMap>
#include "kksgui_config.h"

class QTreeView;
class QAbstractItemDelegate;
class QAbstractItemModel;
class QPushButton;
class QModelIndex;

class _GUI_EXPORT KKSOrganizationAddrForm : public QDialog
{
public:
    KKSOrganizationAddrForm (int idOrg, 
                                 QString orgName, 
                                 const QMap<int, QString>& transporters, 
                                 const QMap<int, QString>& aList, 
                                 const QMap<int, bool>& actList, 
                                 const QMap<int, int>& pList, 
                                 QWidget * parent=0, 
                                 Qt::WindowFlags flags=0);
    ~KKSOrganizationAddrForm (void);

    const QMap<int, QString>& getAddrs (void) const;
    const QMap<int, bool>& getActiveList (void) const;
    const QMap<int, int>& getPortList (void) const;

    void setItemDelegate (QAbstractItemDelegate * deleg);
    void setModel (QAbstractItemModel * mod);

private slots:
    void addrChanged (const QModelIndex& topLeft, const QModelIndex& bottomRight);

private:
    //
    // Functions
    //
    void initWidget (void);
private:
    //
    // Variables
    //
    int idOrganization;
    QString organizationName;
    QMap<int, QString> transp;
    QMap<int, QString> addrs;
    QMap<int, bool> activeList;
    QMap<int, int> portList;

    QTreeView * tvTransporters;
    QPushButton * pbOk;
    QPushButton * pbCancel;
    
private:
    Q_OBJECT
};

#endif	/* _KKSORGANIZATIONADDRWIDGET_H */

