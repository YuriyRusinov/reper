/***********************************************************************
 * Module:  KKSCatEditorFactory.h
 * Author:  yuriy
 * Modified: 13 ����� 2009 �. 16:15:50
 * Purpose: Declaration of the class KKSCatEditorFactory
 * Comment: ������ ����� �������� �������� ���������� ���������
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSCatEditorFactory_h)
#define __KKSSITOOOM_KKSCatEditorFactory_h

//#include <KKSObjEditor.h>
//#include <KKSTemplate.h>
#include "kksfactory_config.h"

#include "KKSEntityFactory.h"
#include <KKSList.h>

class QModelIndex;
class QAbstractItemModel;

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

class _F_EXPORT KKSCatEditorFactory : public KKSEntityFactory
{
    public:
        KKSTemplateEditorFactory * getTemplateEditorFactory (void) const;
        KKSCategoryTemplateWidget* viewCategories (const KKSList<const KKSFilterGroup *> & filters,
                                                   bool mode=false,
                                                   Qt::WindowModality windowModality=Qt::NonModal,
                                                   QWidget *parent=0,
                                                   Qt::WindowFlags f=0);

        KKSCatEditor* createCategoryEditor (int idCategory, // ������������� ���������
                                            const KKSList<const KKSFilterGroup *> & filters,
                                            bool isChildCat, // ��������� ��� ������������� �������� ���������
                                            int idCatType, // ��� ��������� �� ���������
                                            bool mode=false,
                                            Qt::WindowModality windowModality=Qt::NonModal,
                                            QWidget *parent=0,
                                            Qt::WindowFlags f=0);

        KKSCatEditor* createCategoryEditor (KKSCategory *cat, // ���������
                                            const KKSList<const KKSFilterGroup *> & filters,
                                            bool isChildCat, // ��������� ��� ������������� �������� ���������
                                            int idCatType, // ��� ��������� �� ���������
                                            bool mode=false,
                                            Qt::WindowModality windowModality=Qt::NonModal,
                                            QWidget *parent=0,
                                            Qt::WindowFlags f=0);

    signals:
        void categoryEditorCreated (KKSCatEditor *cEditor);
        void categoryEditorCreatedModal (KKSCatEditor *cEditor);
        void categoryAdded (KKSCategory *cat);
        void attributesListLoaded (KKSAttributesEditor *aEditor);
        void categoryDbError ();

    private slots:
        void loadCatTemplates (KKSCategoryTemplateWidget *ctw, int idCat, const QModelIndex& ind);
        void addCategory (QWidget *ctw, int idCatType, bool isChild);
        void addCopyCategory (QWidget *ctw, int idCat, bool isChild);
        void addCloneCategory (QWidget *ctw, int idCat, bool isChild);
        void editCategory (QWidget* ctw, int idCat, bool isChild);
        void delCategory (QWidget * ctw, int idCat);
        //void loadAttributes (KKSCatEditor *editor);
        void loadCatAttribute (int id, KKSCategory *c, QAbstractItemModel * acModel, KKSCatEditor *editor);
        void loadCatAttribute (KKSCategoryAttr *cAttr, KKSCategory *c, QAbstractItemModel * acModel, KKSCatEditor *editor);
        void delCatAttribute (int id, KKSCategory *c, QAbstractItemModel * attrModel, KKSCatEditor *editor);

        void saveCategory (KKSCategory *cat, int idTableCat, int idType, KKSCatEditor *cEditor);
        void addAttributeIntoCategory (KKSCategory *c, QAbstractItemModel * attrModel, KKSCatEditor *editor);
        void copyAttributesIntoCategory (KKSCategory *c, QAbstractItemModel * attrModel, KKSCatEditor *cEditor);
//        void saveAttribute (KKSAttribute * cAttr, int idType, KKSAttributesEditor *aEditor);
//        void loadAttribute (int idAttr, KKSAttributesEditor *aEditor);
//        void delAttribute (int idAttr, KKSAttributesEditor *aEditor);

//        void loadAttrsRefs (KKSAttribute * attr, KKSAttrEditor * aEditor);
//        void loadAttrsRefFields (KKSAttribute * attr, int idRef, KKSAttrEditor * aEditor);

//        void findAttributes (int idAttrs, KKSAttributesEditor * attrsEditor);

    private:
        //
        // Functions
        //
        friend class KKSSito;
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
