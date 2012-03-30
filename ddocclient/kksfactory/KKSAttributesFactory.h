/***********************************************************************
 * Module:  KKSAttributesFactory.h
 * Author:  yuriyrusinov
 * Modified: 03 сент€бр€ 2010 г. 12:00:00
 * Purpose: Declaration of the class KKSAttributesFactory
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttributesFactory_h)
#define __KKSSITOOOM_KKSAttributesFactory_h

#include "kksfactory_config.h"
#include <KKSList.h>
#include <KKSFilter.h>
#include "KKSEntityFactory.h"

class QGridLayout;
class QLabel;
class QCheckBox;
class QToolButton;
class QTabWidget;
class QAbstractItemModel;
class QModelIndex;

class KKSLoader;
class KKSEIOFactory;
class KKSObjEditorFactory;
class KKSAttributesEditor;
class KKSAttrEditor;
class KKSAttrValue;
class KKSObjEditor;
class KKSCategoryAttr;
class KKSAttrValue;
class KKSAttribute;
class KKSAttrType;
class KKSObject;

class _F_EXPORT KKSAttributesFactory : public KKSEntityFactory
{
public:
    KKSAttributesEditor * viewAttributes (const KKSList<const KKSFilterGroup *>& filters, bool mode, QWidget * parent=0, Qt::WFlags f=0);

    static KKSAttrValue* createAttrValue(const QString & xml);
    void putAttrWidget (KKSAttrValue *attr, KKSObjEditor *objEditor, QGridLayout *gLayout, int n_str, bool isSystem, QString tableName = QString::null, int idCat=-1);
    static QString toXML (KKSCategoryAttr* attr);
    void setValue (QWidget *aw, const KKSAttrValue * pattrValue, const KKSAttrType *pCatType, bool isSystem, const QVariant& v, bool isObjExist, QString tableName, int idCat, QToolButton *tbRef, QCheckBox *ch, QWidget *wEditor, QLabel *lTitle, bool isRef=false);
    
    void setOEF(KKSObjEditorFactory * _oef);

private slots:
    void findAttributes (int idAttrs, QAbstractItemModel *aModel, KKSAttributesEditor * attrsEditor);
    void saveAttribute (KKSAttribute * cAttr, int idType, QAbstractItemModel * aModel, int idAttrGroup, KKSAttributesEditor *aEditor);
    void loadAttribute (int idAttr, QAbstractItemModel * aModel, const QModelIndex& aIndex, KKSAttributesEditor *aEditor);
    void delAttribute (int idAttr, QAbstractItemModel * aModel, const QModelIndex& aIndex, KKSAttributesEditor *aEditor);
    void loadAttrsRefFields (KKSAttribute * attr, int idRef, KKSAttrEditor * aEditor);
    void loadSearchTemplates (KKSAttrEditor * aEditor);
    void loadAttrsRefs (KKSAttribute * attr, KKSAttrEditor * aEditor);

    void addAttrGroup (QAbstractItemModel *aModel, KKSAttributesEditor *attrEditor);
    void editAttrGroup (int idAttrGroup, QAbstractItemModel *aModel, const QModelIndex& aIndex, KKSAttributesEditor *attrEditor);
    void delAttrGroup (int idAttrGroup, QAbstractItemModel *aModel, const QModelIndex& aIndex, KKSAttributesEditor *attrEditor);

    void slotLoadIOSrc  (KKSObject ** io);
    void viewIOSrc (KKSObject * io, QWidget * parent);
    //void slotLoadIOSrc1 (const KKSAttrValue * av, QLineEdit * lESrc1);
    void loadIOAttrValueHistory(const KKSAttrValue * av);

signals:
    void viewHistory(const KKSList<KKSAttrValue *> &);

private:
    //
    // Functions
    //
    friend class KKSSito;
    KKSAttributesFactory (KKSLoader *l, KKSEIOFactory *_eiof, KKSObjEditorFactory * _oef);
    virtual ~KKSAttributesFactory (void);

    QLabel * createAttrTitle (KKSAttrValue * av, bool isSystem = false, KKSObjEditor *objEditor = NULL);
    QCheckBox * createChDateTime (bool isMandatory, QGridLayout *gLayout, QLabel *lTitle, int n_str);
    QWidget * createAttrWidget ( KKSAttrValue * pAttrValue, 
                                 KKSObjEditor *objEditor,
                                 bool is_mandatory, 
                                 const KKSAttrType *pCatType,  
                                 bool isSystem, 
                                 QGridLayout *gLayout, 
                                 int n_str, 
                                 const QVariant& V, 
                                 QLabel *lTitle, 
                                 QToolButton *&tbRef, 
                                 QCheckBox *&ch, 
                                 bool isRef=false);

    QWidget * createAttrCheckWidget (const KKSAttrValue * pAttrValue, const KKSAttrType *pCatType,  bool isSystem, QTabWidget * tabW);

    void connectToSlots (QObject *aw, QWidget* wEditor);

private:
    //
    // Variables
    //
    KKSLoader * loader;
    KKSEIOFactory *eiof;
    KKSObjEditorFactory * m_oef;

private:
    Q_OBJECT
};

#endif
