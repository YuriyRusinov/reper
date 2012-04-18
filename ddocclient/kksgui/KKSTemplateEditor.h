/***********************************************************************
 * Module:  KKSTemplateEditor.h
 * Author:  yuriyrusinov
 * Modified: 5 феврал€ 2009 г. 14:30:00
 * Purpose: Declaration of the class KKSTemplateEditor
 * Comment: ƒанный класс унаследован от KKSDialog и предназначен дл€
 * визуализации и редактировани€ шаблонов.
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSTemplateEditor_h)
#define __KKSSITOOOM_KKSTemplateEditor_h

#include "kksgui_config.h"

#include "KKSDialog.h"

#include <QModelIndex>

class QPushButton;
class QTreeView;
class QTableView;
class QLineEdit;
class QTabWidget;
class QAbstractItemModel;
class QAbstractItemDelegate;

class KKSTemplate;
class KKSAttrView;

class _GUI_EXPORT KKSTemplateEditor : public KKSDialog
{
    public:
        KKSTemplateEditor (KKSTemplate *t, bool mode=true, QWidget *parent=0, Qt::WindowFlags f=0);
        ~KKSTemplateEditor ();

        int getTemplateID (void) const;
        int getCategoryID (void) const;

        void setModel (QAbstractItemModel *model);
        void setItemDelegate (QAbstractItemDelegate *iDeleg);

    public slots:
        void save (void);
        void saveClose (void);

    private slots:
        void addTemplateGroup (void);
        void editTemplateGroup (void);
        void delTemplateGroup (void);

        void addAttribute (void);
        void editAttribute (void);
        void delAttribute (void);

        void moveUp (void);
        void moveDown (void);
        
        void saveErr (void);
        void dropErr (void);

    signals:
        void saveTemplate (KKSTemplate *, KKSTemplateEditor *);

        void updateAttrGroups (KKSTemplate *, KKSTemplateEditor *);

        void addAttrGroup (KKSTemplate *, const QModelIndex&, KKSTemplateEditor *);
        void editAttrGroup (int, const QModelIndex&, KKSTemplate *, KKSTemplateEditor *);
        void delAttrGroup (int, const QModelIndex&, KKSTemplate *, KKSTemplateEditor *);

        void addAttrIntoGroup (int, const QModelIndex&, KKSTemplate *, KKSTemplateEditor *);
        void appendAttrIntoGroup (int, int, const QModelIndex&, KKSTemplate *, KKSTemplateEditor *);
        void pushAttrIntoGroup (int, int, KKSTemplate *, KKSTemplateEditor *);
        void editAttrInGroup (int, int, const QModelIndex&, const QModelIndex&, KKSTemplate *, KKSTemplateEditor *);
        void delAttrFromGroup (int, int, const QModelIndex&, const QModelIndex&, KKSTemplate *, KKSTemplateEditor *);

    private:
        //
        // Functions
        //
        void init_widgets (bool mode);
        void init_par_widget (QTabWidget * tWidget);
        void init_group_widget (QTabWidget *tWidget);
        void init_attr_widget (QTabWidget *tWidget);

        QModelIndex getSelectedIndex (void) const;
        void swapAttrs (KKSAttrView *aView, KKSAttrView *aViewPrev, const QModelIndex& gIndex, const QModelIndex& gIndexP);

    private:
        //
        // Variables
        //
        friend class KKSTemplateEditorFactory;
        friend class KKSViewFactory;

        int idCat;
        int ID;
        QString name;
        KKSTemplate * tRef;

        QLineEdit *lEID;
        QLineEdit *lETitle;
        QLineEdit *lECat;

        QTreeView *tvGroups;

        QPushButton *pbAddGroup;
        QPushButton *pbEditGroup;
        QPushButton *pbDelGroup;

        QPushButton *pbAddAttr;
        QPushButton *pbEditAttr;
        QPushButton *pbDelAttr;

        QPushButton *pbUp;
        QPushButton *pbDown;

        QTableView *tableAttrs;

        QPushButton *pbOk;
        QPushButton *pbCancel;
        QPushButton *pbApply;
        
        bool dbErr;

    private:
        Q_OBJECT
};

#endif
