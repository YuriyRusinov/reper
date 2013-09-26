/***********************************************************************
 * Module:  KKSAttributesFactory.h
 * Author:  yuriyrusinov
 * Modified: 03 сентября 2010 г. 12:00:00
 * Purpose: Declaration of the class KKSAttributesFactory
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttributesFactory_h)
#define __KKSSITOOOM_KKSAttributesFactory_h

#include "kksfactory_config.h"
#include <KKSList.h>
#include <KKSFilter.h>
#include "KKSEntityFactory.h"
#include "KKSIndAttr.h"

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
class KKSPPFactory;
class KKSAttributesEditor;
class KKSAttrEditor;
class KKSAttrValue;
class KKSObjEditor;
class KKSCategoryAttr;
class KKSAttrValue;
class KKSAttribute;
class KKSAttrType;
class KKSObject;
class KKSEIOData;

class _F_EXPORT KKSAttributesFactory : public KKSEntityFactory
{
public:
    KKSAttributesEditor * viewAttributes (const KKSList<const KKSFilterGroup *>& filters, bool mode, QWidget * parent=0, Qt::WFlags f=0);

    static KKSAttrValue* createAttrValue(const QString & xml);
    
    static QString toXML (KKSCategoryAttr* attr);
    
    void setValue (QWidget *aw, 
                   const KKSAttrValue * pattrValue, 
                   const KKSAttrType *pCatType, 
                   KKSIndAttr::KKSIndAttrClass isSystem, 
                   const QVariant& v, 
                   bool isObjExist, 
                   QString tableName, 
                   int idCat, 
                   QToolButton *tbRef, 
                   QCheckBox *ch, 
                   QWidget *wEditor, 
                   QLabel *lTitle, 
                   bool isRef=false);
    
    void setOEF(KKSObjEditorFactory * _oef);
public slots:
    void putAttrWidget (KKSAttrValue *attr, 
                        KKSObjEditor *objEditor, 
                        QGridLayout *gLayout, 
                        int n_str, 
                        KKSIndAttr::KKSIndAttrClass isSystem, 
                        QString tableName = QString::null, 
                        int idCat=-1);

private slots:
    void findAttributes (int idAttrs, QAbstractItemModel *aModel, KKSAttributesEditor * attrsEditor);
    void saveAttribute (const QModelIndex&, QAbstractItemModel * aModel, KKSAttributesEditor *aEditor);
    void loadAttribute (int idAttr, QAbstractItemModel * aModel, const QModelIndex& aIndex, KKSAttributesEditor *aEditor);
    void delAttribute (int idAttr, QAbstractItemModel * aModel, const QModelIndex& aIndex, KKSAttributesEditor *aEditor);
    void loadAttrsRefFields (KKSAttribute * attr, int idRef, KKSAttrEditor * aEditor);
    void loadSearchTemplates (KKSAttrEditor * aEditor);
    void loadAttrsRefs (KKSAttribute * attr, KKSAttrEditor * aEditor);

    void addAttrGroup (QAbstractItemModel *aModel, const QModelIndex& pIndex, KKSAttributesEditor *attrEditor);
    void editAttrGroup (int idAttrGroup, QAbstractItemModel *aModel, const QModelIndex& aIndex, KKSAttributesEditor *attrEditor);
    void delAttrGroup (int idAttrGroup, QAbstractItemModel *aModel, const QModelIndex& aIndex, KKSAttributesEditor *attrEditor);

    void slotLoadIOSrc  (KKSObject ** io, QWidget * parent);
    void viewIOSrc (KKSObject * io, QWidget * parent);
    //void slotLoadIOSrc1 (const KKSAttrValue * av, QLineEdit * lESrc1);
    void loadIOAttrValueHistory(const KKSAttrValue * av, bool forRecords = false);
    void loadIOAttrValueHistoryR(const KKSAttrValue * av, qint64 idObj, qint64 idRec, bool forRecords = false);
    
    void showAttrsWidget(KKSAttribute *, KKSAttrEditor *);//показать виджет с атрибутами для случая с описывающими атрибутами атрибута
    void addAttribute (KKSAttribute *a, QAbstractItemModel * attrModel, KKSAttrEditor *editor);
    void editAttribute (int id, KKSAttribute *a, QAbstractItemModel * attrModel, KKSAttrEditor *editor);
    void delAttribute (int id, KKSAttribute *a, QAbstractItemModel * attrModel, KKSAttrEditor *editor);

    void addComplexAttr (KKSAttribute *a, QAbstractItemModel * attrModel, KKSAttrEditor *editor);
    void editComplexAttr (int id, KKSAttribute *a, QAbstractItemModel * attrModel, const QModelIndex& attrInd, KKSAttrEditor *editor);
    void delComplexAttr (int id, KKSAttribute *a, QAbstractItemModel * attrModel, const QModelIndex& attrInd, KKSAttrEditor *editor);
    
    void updateAttrModel (QAbstractItemModel * attrModel);
    
    void viewAttrValue (const KKSAttrValue * av, int idAVal, int isSys, QWidget * pWidget);
    void refreshAttrValue (const KKSAttrValue * av, int idAVal);
    
    void updateAttrValueModel (const KKSAttrValue * pAttrValue, const QVariant& val, QAbstractItemModel * attrValModel);
    void initAttrValueModel (const KKSAttrValue * pAttrValue, const QVariant& val);

signals:
    void viewHistory(const KKSList<KKSAttrValue *> &);
    void expandIndex (const QModelIndex&);
    void aValRefresh (const KKSAttribute *, QVariant);
    
private:
    //
    // Functions
    //
    friend class KKSSito;
    KKSAttributesFactory (KKSLoader *l, KKSEIOFactory *_eiof, KKSObjEditorFactory * _oef, KKSPPFactory * _ppf);
    virtual ~KKSAttributesFactory (void);

#ifdef Q_CC_MSVC
    QLabel * createAttrTitle (KKSAttrValue * av, KKSIndAttr::KKSIndAttrClass isSystem = KKSIndAttr::KKSIndAttrClass::iacIOUserAttr, KKSObjEditor *objEditor = NULL);
#else
    QLabel * createAttrTitle (KKSAttrValue * av, KKSIndAttr::KKSIndAttrClass isSystem = KKSIndAttr::iacIOUserAttr, KKSObjEditor *objEditor = NULL);
#endif
    QCheckBox * createChDateTime (bool isMandatory, QGridLayout *gLayout, QLabel *lTitle, int n_str);
    QWidget * createAttrWidget ( KKSAttrValue * pAttrValue, 
                                 KKSObjEditor *objEditor,
                                 bool is_mandatory, 
                                 const KKSAttrType *pCatType,  
                                 KKSIndAttr::KKSIndAttrClass isSystem, 
                                 QGridLayout *gLayout, 
                                 int n_str, 
                                 const QVariant& V, 
                                 QLabel *lTitle, 
                                 QToolButton *&tbRef, 
                                 QCheckBox *&ch, 
                                 bool isRef=false);
    
    QWidget * createAttrValWidget (const KKSAttrValue * pAttrValue, int idAVal, int isSys, QWidget * parent=0, Qt::WindowFlags flags=0);

    QWidget * createAttrCheckWidget (const KKSAttrValue * pAttrValue, const KKSAttrType *pCatType,  KKSIndAttr::KKSIndAttrClass isSystem, QTabWidget * tabW, KKSObjEditor * objEditor);
    QAbstractItemModel * aValComplexModel (const KKSAttrValue * pAttrValue, const QVariant& av=QVariant());

    void connectToSlots (QObject *aw, QWidget* wEditor);

    KKSMap<qint64, KKSEIOData *> getAttrList (const KKSAttrValue * pAttrValue, const QVariant& val) const;
private:
    //
    // Variables
    //
    KKSLoader * loader;
    KKSEIOFactory *eiof;
    KKSObjEditorFactory * m_oef;
    KKSPPFactory * m_ppf;

private:
    Q_OBJECT
};

#endif
