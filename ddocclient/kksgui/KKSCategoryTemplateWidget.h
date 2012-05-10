/***********************************************************************
 * Module:  KKSCategoryTemplateWidget.h
 * Author:  yuriyrusinov
 * Modified: 17.01.2009
 * Purpose: Declaration of the class KKSCategoryTemplateWidget
 * Comment: Данный класс унаследован от QWidget и предназначен для управлениякатегориями и их шаблонами.
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSCategoryTemplateWidget_h)
#define __KKSSITOOOM_KKSCategoryTemplateWidget_h

#include "kksgui_config.h"

#include <QWidget>
#include <QMap>
#include <QSize>

#include "KKSDialog.h"

class QEventLoop;
class QTreeView;
class QPushButton;
class QGridLayout;
class QItemSelectionModel;
class QModelIndex;
class QAbstractItemModel;
class QAction;
class QMenu;
class QToolBar;

class KKSCategory;
class KKSTemplate;

class _GUI_EXPORT KKSCategoryTemplateWidget : public KKSDialog
{
    public:
        KKSCategoryTemplateWidget (bool mode, const QList<int>& fTypes, bool asAdmin = false, QWidget *parent = 0, Qt::WindowFlags f=0);
        ~KKSCategoryTemplateWidget (void);

        int getCatID (void) const;
        int getTemplateID (void) const;

        QAbstractItemModel * getModel (void) const;
        QModelIndex getSelectedIndex (void) const;

        //
        // Functions overrides
        //
        virtual QSize sizeHint (void) const;

    private slots:

        void accept (void);
        void apply (void);

        void addCat (void);
        void addCopyCat (void);
        void addCloneCat (void);
        void addTemplate (void);
        void editCat (void);
        void editTemplate (void);
        void delCat (void);
        void delTemplate (void);

        void currIndexChanged (const QModelIndex& current, const QModelIndex& previous);
        void addCatRec (KKSCategory * c);
        void uploadCatT (KKSTemplate *t);

    signals:
        void addNewCategory (QWidget *ctw, int idCatType, bool isChild);
        void addCopyCategory (QWidget *ctw, int idCat, bool isChild);
        void addNewCategoryE (QWidget *ctw, int idCatType, bool isChild);
        void addNewTemplate (QWidget *ctw, int idCat);
        void editCategory (QWidget *ctw, int idCat, bool isChild);
        void editTempl (QWidget *ctw, int idTempl);
        void deleteCat (QWidget *ctw, int idCat);
        void deleteT (QWidget *ctw, int idTempl);
        void uploadTemplatesIntoCategory (KKSCategoryTemplateWidget *ctw, int idCat, const QModelIndex& ind);

    private:
        //
        // Functions
        //
//        void done (int r);

        void init_widgets (void);

        int getCurrentCategoryId (void) const;
        int getCurrentTemplateId (void) const;

        QModelIndex getMainCatIndex (void) const;

        void uploadModel (QAbstractItemModel *mod);

    private:
        friend class KKSObjEditorFactory;
        friend class KKSViewFactory;
        friend class KKSAttrWidgetFactory;
        //
        // Variables
        //
        QGridLayout *catTemplLayout;
        QTreeView *tvCatTemplate;

        QPushButton *pbOk;
        QPushButton *pbCancel;

        QToolBar * tbCatTempl;
        QAction * actAddC;
        QAction * actAddCopyC;
        QAction * actAddConBase;
        QAction *actAddT;

        QAction *actEditC;
        QAction *actEditT;

        QAction *actDelC;
        QAction *actDelT;

        QItemSelectionModel *sModel;
        QMap <int, QModelIndex> pCatInd;

        //определяет, что АРМ запущен от лица администратора 
        //(надо показать кнопки редактирования категорий и шаблонов)
        bool m_asAdmin;
        QList<int> forbiddenTypes;

    private:
        Q_OBJECT
};

#endif
