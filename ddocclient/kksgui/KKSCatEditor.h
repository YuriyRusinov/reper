/***********************************************************************
 * Module:  KKSCatEditor.h
 * Author:  yuriyrusinov
 * Modified: 19.01.2009 г. 11:32:00
 * Purpose: Declaration of the class KKSCatEditor
 * Comment: ƒанный класс унаследован от QWidget и предназначен дл€ визуализации и редактировани€ категории
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSCatEditor_h)
#define __KKSSITOOOM_KKSCatEditor_h

#include "kksgui_config.h"

#include <QWidget>
#include <QMap>
#include <QString>

#include <KKSMap.h>

#include "KKSDialog.h"

class QPushButton;
class QEventLoop;
class QGridLayout;
class QTabWidget;
class QScrollArea;
class QLineEdit;
class QCheckBox;
class QComboBox;
class QToolButton;
class QTreeView;
class QAbstractItemModel;
class QModelIndex;

class KKSCategory;
class KKSRecWidget;
class KKSAttributesEditor;
class KKSCategoryAttr;
class KKSAttrType;
class KKSIncludesWidget;
class KKSAccessEntity;
class KKSStuffForm;
class KKSType;

class _GUI_EXPORT KKSCatEditor : public KKSDialog
{
    public:
        KKSCatEditor (KKSCategory *c, KKSRecWidget * rw, KKSRecWidget * rtw, KKSRecWidget * rTemplatesW, KKSRecWidget * rTableTemplatesW, const KKSMap<int, KKSType *>& catTypesList, int idCatType0 = -1, bool mode=true, QWidget *parent=0, Qt::WindowFlags f=0);
        ~KKSCatEditor (void);

        void save (void);
        void setIsChildCat (bool isChild);
        KKSCategory *category (void);
        const KKSCategory * category (void) const;
        void setTableType (KKSType * tType);

        void appendChildCat (int idCat, QString catName);
        void updateChildCat (int idCat, QString catName);
        QAbstractItemModel *getTemplateModel (void);
        QModelIndex getSelectedTemplateIndex (void) const;
        QTreeView * getTemplView (void) const;

        KKSStuffForm * getAccessWidget (void) const;
        void setAccessWidget (KKSStuffForm * sForm);

    private slots:

        void accept (void);
        void apply (void);

        void createChildCat (void);

        void addAttribute (void);
        void editAttribute (void);
        void delAttribute (void);

        void addTableAttribute (void);
        void editTableAttribute (void);
        void delTableAttribute (void);
        void copyAttributesFrom (void);

        void addTemplate (void);
        void editTemplate (void);
        void delTemplate (void);

        void addTableTemplate (void);
        void editTableTemplate (void);
        void delTableTemplate (void);

        void setCurrentType (int index);
        void setAccessRules (KKSAccessEntity * acl);

        void setTableCategoryName (const QString& tName);

    signals:
        void addAttrsIntoCat (KKSCategory *c, QAbstractItemModel * attrModel, KKSCatEditor *editor);
        void setAttribute (int id, KKSCategory *c, QAbstractItemModel * attrModel, KKSCatEditor *editor);
        void setAttribute (KKSCategoryAttr *cAttr, KKSCategory *c, QAbstractItemModel * attrModel, KKSCatEditor *editor);
        void copyAttrsFromAnotherCat (KKSCategory *c, QAbstractItemModel * attrModel, KKSCatEditor *editor);
        void delAttrFromCategory (int id, KKSCategory *c, QAbstractItemModel * attrModel, KKSCatEditor *editor);
        void saveCategory (KKSCategory *cat, int idTableCat, int idType, KKSCatEditor *editor);
        void addChildCat (QWidget *catW, int idCatType, bool isChild);
        void editChildCat (QWidget *catW, int idCat, bool isChild);
        void addNewCategoryTemplate (QWidget *catW, int idCat, QAbstractItemModel * templModel);
        void editCategoryTemplate (QWidget *catW, int idTempl, QAbstractItemModel * templModel, const QModelIndex& tIndex);
        void delCategoryTemplate (QWidget *catW, int idTempl, QAbstractItemModel * templModel, const QModelIndex& tIndex);

    private:
        //
        // Functions
        //
        void init_widgets (void);
        void setChildCat (const QMap<int, QString> chItems);
        void init_parameters (void);
        void init_attributes (void);
        void initTableAttributes (void);
        void init_templates (void);
        void init_rubrics (void);
//        void setAttrs (KKSRecWidget *rw);
//        void setTableAttrs (KKSRecWidget * rtw);
        void setAttrTypes (const KKSMap<int, KKSAttrType*>& aTypes);
        void setTemplates (KKSRecWidget *rtw);

    private:
        //
        // Variables
        //
//        friend class KKSObjEditorFactory;
        friend class KKSCatEditorFactory;
        friend class KKSViewFactory;

        KKSCategory *pCategory;
        KKSCategory *pTableCategory;
        KKSMap<int, KKSAttrType *> attrTypes;
        KKSMap<int, KKSType *> catTypes;
        KKSType * tableType;

        QGridLayout *gCatLayout;
        QTabWidget *tabCat;

        QPushButton *pbOk;
        QPushButton *pbCancel;
        QPushButton *pbApply;

        QLineEdit *lEID;
        QLineEdit *lEName;
        QComboBox *cbChildCat;
        QToolButton *tbChildCat;
        QComboBox *cbTypes;
        QCheckBox *cbGlobal;
//        QToolButton *tbType;

        KKSRecWidget *recWidget;
        KKSRecWidget *recTableW;
        KKSRecWidget *recCatTemplatesW;
        KKSRecWidget *recTableCatTemplatesW;
        KKSIncludesWidget * rubrW;
        KKSStuffForm * sForm;

        bool isCloseIgnored;

    private:
        Q_OBJECT
};

#endif
