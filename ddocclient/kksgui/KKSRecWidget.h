/***********************************************************************
 * Module:  KKSRecWidget.h
 * Author:  yuriyrusinov
 * Modified: 26.01.2009 г. 16:00:00
 * Purpose: Declaration of the class KKSRecWidget
 * Comment: Данный класс унаследован от QWidget и предназначен для
 *  визуализации списка экземпляров ИО
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSRecWidget_h)
#define __KKSSITOOOM_KKSRecWidget_h

#include <QWidget>
#include <QModelIndex>

#include "kksgui_config.h"

class QTreeView;
class QToolButton;
class QPushButton;
class QAbstractItemModel;
class QItemSelectionModel;
class QAbstractItemDelegate;
class QGridLayout;
class QGroupBox;
class QAction;
class QToolBar;
class QMenu;
class QContextMenuEvent;

class _GUI_EXPORT KKSRecWidget : public QWidget
{
public:
    KKSRecWidget (QTreeView *tView, bool mode=false, QWidget *parent=0, Qt::WindowFlags f=0);
    //KKSRecWidget (const QString& filterTitle, const QString& addTitle, const QString& editTitle, const QString& delTitle, const QString& importTitle, const QString& exportTitle, QTreeView *tView, bool mode=false, QWidget *parent=0, Qt::WindowFlags f=0);
    ~KKSRecWidget (void);

    qint64 getID (void) const;

    void setEIOModel (QAbstractItemModel *model);
    QAbstractItemModel *getModel (void) const;
    QAbstractItemModel *getSourceModel (void) const;
    QItemSelectionModel *getSelectionModel (void) const;
    QModelIndex getCurrentIndex (void) const;
    QModelIndex getSourceIndex (void) const;
    QModelIndexList getSourceIndexes (void) const;
    QTreeView * getView (void) const;
    QAbstractItemDelegate * getItemDelegate () const;
    void setItemDelegate (QAbstractItemDelegate * deleg);

    void resizeSections (const QList<int>& hAttrWidths);
    void hideAllButtons (void);
    void showEditGroup (void);

    void hideGroup (int num_gr);
    void showGroup (int num_gr);

    void hideToolBar (void);
    void showToolBar (void);

    void addToolBarAction (QAction * act);
    QAction * addToolBarSeparator (void);
    void insertToolBarAction (QAction * before, QAction * action);
    void removeToolBarAction (QAction * action);

    QSize sizeHint() const;
    void setRecContextMenuPolicy (Qt::ContextMenuPolicy policy);
    
protected:
    //
    // Widget reimplementation
    //
    virtual void contextMenuEvent (QContextMenuEvent * event);

signals:
    void addEntity (QAbstractItemModel * sourceMod, const QModelIndex& pIndex);
    void editEntity (QAbstractItemModel * sourceMod, const QModelIndex& wIndex);
    void delEntity (QAbstractItemModel * sourceMod, const QModelIndex& wIndex);
    void refreshMod (QAbstractItemModel * sourceMod);
    
    void entityDoubleClicked();

private slots:
    void addRec (void);
    void editRec (void);
    void delRec (void);
    void refreshRec (void);
    void hideRecord (void);

    void tvDoubleClicked(const QModelIndex & index);

private:
    //
    // Functions
    //
    void setTreeView (QTreeView *_tv);
    void init_widgets (bool mode);

private:
    //
    // Variables
    //
    friend class KKSViewFactory;
    friend class KKSObjEditorFactory;

    QTreeView * tv;
    QToolBar * tBActions;
    QMenu * pMenu;

public:

    QAction * actFilter;
    QAction * actSearchT;
    QAction * actFilterSep;

    QAction * actAdd;
    QAction * actEdit;
    QAction * actDel;
    QAction * actEditSep;

    QAction * actImport;
    QAction * actExport;
    QAction * actImportExportSep;

    QAction * actGroupBy;
    QAction * actHideRec;
    QAction * actRefresh;

    QPushButton * pbOk;
    QPushButton * pbCancel;
    QPushButton * pbApply;

    QAction * actSetView;

private:
    QGridLayout * gLay;

private:
    Q_OBJECT
};

#endif
