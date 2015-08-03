/***********************************************************************
 * Module:  KKSCatEditorFactory.h
 * Author:  yuriy
 * Modified: 13 марта 2009 г. 16:15:50
 * Purpose: Declaration of the class KKSCatEditorFactory
 * Comment: данный класс является фабрикой редакторов категорий
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSCatEditorFactory_h)
#define __KKSSITOOOM_KKSCatEditorFactory_h

//#include <KKSObjEditor.h>
//#include <KKSTemplate.h>
#include "kksguifactory_config.h"

#include "KKSEntityFactory.h"
#include <KKSList.h>

class QModelIndex;
class QAbstractItemModel;
class QLineEdit;
class QTreeView;

class KKSLoader;
class KKSObjEditorFactory;
class KKSPPFactory;
class KKSEIOFactory;
class KKSTemplateEditorFactory;
class KKSAttributesFactory;

class KKSFilterGroup;
class KKSCategoryTemplateWidget;
class KKSCatEditor;
class KKSCategory;
class KKSAttributesEditor;
class KKSCategoryAttr;
class KKSAttribute;
class KKSAttrEditor;
class KKSAttributesFactory;
class KKSStuffFactory;
class KKSRecWidget;
class KKSObjEditor;
class KKSRecDialog;
class kkslifecycleform;
class KKSLifeCycleEx;

/*!\ingroup FACTORY_GROUP

\brief Класс для формирования визуальных элементов для редактора атрибутов и категорий
*/
class _GF_EXPORT KKSCatEditorFactory : public KKSEntityFactory
{
    public:
        KKSTemplateEditorFactory * getTemplateEditorFactory (void) const;
        KKSCategoryTemplateWidget* viewCategories (const KKSList<const KKSFilterGroup *> & filters,
                                                   bool mode=false,
                                                   Qt::WindowModality windowModality=Qt::NonModal,
                                                   QWidget *parent=0,
                                                   Qt::WindowFlags f=0);

        KKSCatEditor* createCategoryEditor (int idCategory, // идентификатор категории
                                            const KKSList<const KKSFilterGroup *> & filters,
                                            bool isChildCat, // создается или редактируется дочерняя категория
                                            int idCatType, // тип категории по умолчанию
                                            bool mode=false,
                                            Qt::WindowModality windowModality=Qt::NonModal,
                                            QWidget *parent=0,
                                            Qt::WindowFlags f=0);

        KKSCatEditor* createCategoryEditor (KKSCategory *cat, // категория
                                            const KKSList<const KKSFilterGroup *> & filters,
                                            bool isChildCat, // создается или редактируется дочерняя категория
                                            int idCatType, // тип категории по умолчанию
                                            bool mode=false,
                                            Qt::WindowModality windowModality=Qt::NonModal,
                                            QWidget *parent=0,
                                            Qt::WindowFlags f=0);

    signals:
        void categoryEditorCreated (KKSCatEditor *cEditor);
        void categoryEditorCreatedModal (KKSCatEditor *cEditor);
        void categoryAdded (KKSCategory *cat);
        void attributesListLoaded (KKSAttributesEditor *aEditor);
        
        void lifeCycleEditorCreated (kkslifecycleform * lcEditor);
        void categoryDbError ();

    private slots:
        void loadCatTemplates (KKSCategoryTemplateWidget *ctw, int idCat, const QModelIndex& ind);
        void addCategory (QWidget *ctw, int idCatType, bool isChild);
        void addCopyCategory (QWidget *ctw, int idCat, bool isChild);
        void addCloneCategory (QWidget *ctw, int idCat, bool isChild);
        void editCategory (QWidget* ctw, int idCat, bool isChild);
        void delCategory (QWidget * ctw, int idCat);
        //void loadAttributes (KKSCatEditor *editor);
        void loadCatAttribute (int id, KKSCategory *c, QTreeView *tv, KKSCatEditor *editor);
        void loadCatAttribute (KKSCategoryAttr *cAttr, KKSCategory *c, QTreeView * tv, KKSCatEditor *editor);
        void delCatAttribute (int id, KKSCategory *c, QAbstractItemModel * attrModel, KKSCatEditor *editor);

        void saveCategory (KKSCategory *cat, int idTableCat, int idType, KKSCatEditor *cEditor);
        void addAttributeIntoCategory (KKSCategory *c, QTreeView * tv, KKSCatEditor *editor);
        void copyAttributesIntoCategory (KKSCategory *c, QAbstractItemModel * attrModel, KKSCatEditor *cEditor);
        void refreshCategoryTemplates (KKSCategory * c, QAbstractItemModel * templModel);

        void setLifeCycleIntoCategory (KKSCategory * c, QLineEdit * lE);
        void saveLifeCycleToDb (KKSLifeCycleEx * lc);
        
    public slots:
        KKSRecDialog * openLifeCycle ();
        kkslifecycleform * createLifeCycle ();

    private slots:
        void addLifeCycle (QWidget * editor, int idObject, const KKSCategory * c, QString tableName, int nTab, bool isModal, QAbstractItemModel * sRecMod);
        void editLifeCycle (QWidget * editor, int idObject, qint64 idObjE, const KKSCategory * c, QString tableName, int nTab, bool isModal, QAbstractItemModel * sRecMod, const QModelIndex& recIndex);
        void delLifeCycle (QWidget * editor, int idObject, qint64 idObjE, QString tableName, QAbstractItemModel * recModel, const QModelIndex& recIndex);

    private:
        //
        // Functions
        //
        friend class KKSApplication;
        KKSCatEditorFactory (KKSLoader *l, KKSObjEditorFactory * _objf, KKSPPFactory *_ppf, KKSEIOFactory *_eiof, KKSAttributesFactory * _attrf, KKSStuffFactory * _sf, QObject *parent=0);
        ~KKSCatEditorFactory (void);

        void setTemplateEditorFactory (KKSTemplateEditorFactory *_tf);
        KKSList<const KKSFilterGroup *> viewMainCategories (void);

        KKSRecWidget * getAttrsWidget (const KKSCategory *cat, bool mode=false, QWidget *parent=0) const;
        KKSRecWidget * getTemplateWidget (const KKSCategory * cat, bool mode=false, QWidget *parent=0) const;

    private:
        //
        // Variables
        //
        KKSLoader *loader;
        KKSObjEditorFactory * objf;
        KKSPPFactory *ppf;
        KKSEIOFactory *eiof;
        KKSTemplateEditorFactory *tf;
        KKSAttributesFactory * attrf;
        KKSStuffFactory * stuffF;

    private:
        Q_OBJECT
};

#endif
