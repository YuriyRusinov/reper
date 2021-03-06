/***********************************************************************
 * Module:  KKSRecWidget.h
 * Author:  yuriyrusinov
 * Modified: 26.01.2009 �. 16:00:00
 * Purpose: Declaration of the class KKSRecWidget
 * Comment: ������ ����� ����������� �� QWidget � ������������ ���
 *  ������������ ������ ����������� ��
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSRecWidget_h)
#define __KKSSITOOOM_KKSRecWidget_h

#include <QWidget>
#include <QTreeView>
#include <QModelIndex>
#include <QItemSelection>
#include <QAbstractItemView>

#include "kksgui_config.h"

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
class QLineEdit;
class QBoxLayout;

class KKSAttribute;

#define _ID_FILTER_GROUP 0
#define _ID_ADD_EDIT_DEL_GROUP 1
#define _ID_IMPORT_GROUP 2
#define _ID_VIEW_GROUP 3
#define _ID_REPORT_GROUP 4
#define _GROUP_COUNT 4

class KKSRecWidgetTreeView : public QTreeView
{
    Q_OBJECT
public:
    KKSRecWidgetTreeView(QWidget * parent = 0):QTreeView(parent){}
    virtual ~KKSRecWidgetTreeView(){}
protected:
    void currentChanged ( const QModelIndex & current, const QModelIndex & previous );
signals:
    void recViewCurrentChanged(const QModelIndex & current);
};

class _GUI_EXPORT KKSRecWidget : public QWidget
{
public:
    KKSRecWidget (bool mode=false, Qt::Orientation orient=Qt::Vertical, QWidget *parent=0, Qt::WindowFlags f=0);
    //KKSRecWidget (const QString& filterTitle, const QString& addTitle, const QString& editTitle, const QString& delTitle, const QString& importTitle, const QString& exportTitle, QTreeView *tView, bool mode=false, QWidget *parent=0, Qt::WindowFlags f=0);
    ~KKSRecWidget (void);

    qint64 getID (void) const;
    QList<qint64> getIDList (void) const;

    void setEIOModel (QAbstractItemModel *model);
    QAbstractItemModel *getModel (void) const;
    QAbstractItemModel *getSourceModel (void) const;
    QItemSelectionModel *getSelectionModel (void) const;
    QModelIndex getCurrentIndex (void) const;
    QModelIndex getSourceIndex (void) const;
    QModelIndexList getSourceIndexes (void) const;
    QItemSelection getSourceSelection (void) const;
    QTreeView * getView (void) const;
    QToolBar * getToolBar (void) const;
    QAbstractItemDelegate * getItemDelegate () const;
    void setItemDelegate (QAbstractItemDelegate * deleg);
    QAbstractItemView::SelectionMode selectionMode (void) const;
    void setSelectionMode ( QAbstractItemView::SelectionMode mode );

    void resizeSections (const QList<int>& hAttrWidths);
    void hideAllButtons (void);
    void showEditGroup (void);

    void hideActionGroup (int num_gr);
    void showActionGroup (int num_gr);

    void hideToolBar (void);
    void showToolBar (void);
    
    void hideFilter (void);
    void showFilter (void);

    void hideReportGroup();
    void showReportGroup();

    void addToolBarAction (QAction * act);
    QAction * addToolBarSeparator (void);
    void insertToolBarAction (QAction * before, QAction * action);
    void removeToolBarAction (QAction * action);
    
    void setToolBarActionEnabled (QAction * act, bool isEnabled);

    QSize sizeHint() const;
    void setRecContextMenuPolicy (Qt::ContextMenuPolicy policy);
    
    void enableGroupMenu (bool enable);
    void enableFilterMenu (bool enable);
    void clearGroupMenu (void);
    QAction * addGroupAttribute (KKSAttribute * attr);
    
    QBoxLayout * getButtonsLay (void) const;
    
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

    void showReportEditor(qint64 idReport);
    void showReportViewer(qint64 idReport);
    
    void editEntitiesList (QAbstractItemModel * sourceMod, const QItemSelection& sel);
    void delEntitiesList (QAbstractItemModel * sourceMod, const QItemSelection& sel);

private slots:
    void addRec (void);
    void editRec (void);
    void delRec (void);
    void refreshRec (void);
    void hideRecord (void);
    void viewAllRecs (void);
    void viewRecsFromHere (void);

    void tvDoubleClicked(const QModelIndex & index);
    void recViewCurrentChanged(const QModelIndex & current);
    void filterRecs (const QString& text);

    void slotReportEdit();
    void slotReportOpen();

private:
    //
    // Functions
    //
    void setTreeView (QTreeView *_tv);
    void init_widgets (bool mode, Qt::Orientation orient);
    void setVisibleFrom (const QModelIndex & sParent=QModelIndex(), bool v=true);

private:
    //
    // Variables
    //
    friend class KKSViewFactory;
    friend class KKSObjEditorFactory;

    KKSRecWidgetTreeView * tView;
    QToolBar * tBActions;
    QMenu * pMenu;
    QMenu * pGroupBy;
    QMenu * pFilter;
    QLineEdit * filterLE;
    QGroupBox * gbFilter;

    QBoxLayout * vButtonsLayout;

public:

    QAction * actFilter;
    QAction * actSearchT;
    QAction * actFilterSep;

    QAction * actAdd;
    QAction * actEdit;
    QAction * actDel;
    QAction * actEditSep;

    QAction * actReportEdit;//for openRPT
    QAction * actReportOpen;//for openRPT
    QAction * actReportSep;

    QAction * actImport;
    QAction * actExport;
    QAction * actImportExportSep;

    QAction * actGroupBy;
    QAction * actViewAll;
    QAction * actViewOnlyFromHere;
    QAction * actViewCurentRec;
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
