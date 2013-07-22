/***********************************************************************
 * Module:  KKSViewFactory.h
 * Author:  sergey
 * Modified: 27 ноября 2008 г. 13:28:59
 * Purpose: Declaration of the class KKSViewFactory
 * Comment: данный класс предназначен для загрузки из БД всего необходимого
 * для инициализации модели в QTreeview
 *    Он имеет статический метод createView() для этого
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSViewFactory_h)
#define __KKSSITOOOM_KKSViewFactory_h

#include "kksfactory_config.h"
#include <KKSFilter.h>
#include <KKSList.h>
#include <KKSMap.h>

class QTreeView;
class QAbstractItemModel;
class QModelIndex;
class QProgressDialog;

class KKSTemplate;
class KKSObjEditor;
class KKSObject;
class KKSLoader;
class KKSCategoryTemplateWidget;
class KKSRecWidget;
class KKSAttributesEditor;
class KKSCategory;
class KKSAttrGroup;
class KKSAGroup;
class KKSCategoryAttr;
class KKSEIOData;
class KKSObjectExemplar;

class _F_EXPORT KKSViewFactory
{
    public:
        static KKSRecWidget * createView (KKSTemplate* theTemplate, 
                                     KKSObjEditor *objEditor, 
                                     KKSObject* obj, 
                                     KKSLoader *l, 
                                     const KKSList<const KKSFilterGroup *> & filters,
                                     QWidget *parent=0,
                                     Qt::WindowFlags f=0);

        static void loadEIOEx (KKSObjEditor * editor, 
                               const KKSObject *pObj, 
                               KKSLoader *l, 
                               const KKSTemplate *t, 
                               QTreeView *tv,
                               const KKSList<const KKSFilterGroup *> & filters, 
                               bool resize=false,
                               const KKSCategory * cat = 0,
                               const QString& tableName = QString(),
                               QProgressDialog *pgDial=0,
                               bool isCheckable=false);

        static void loadEIOEx (QWidget *editor,
                               const KKSObject *pObj, 
                               KKSLoader *l, 
                               const KKSTemplate *t, 
                               QTreeView *tv,
                               const KKSList<const KKSFilterGroup *> & filters,
                               QProgressDialog *pgDial=0);

        static void updateEIOEx (KKSLoader *l,
                                 KKSObject *pObj,
                                 const KKSMap<qint64, KKSObjectExemplar *>& idObjEx,
                                 const KKSTemplate *t,
                                 QAbstractItemModel *sourceModel
                                 );

        static void loadCategories (KKSCategoryTemplateWidget* catTemplW,
                                    KKSLoader* l,
                                    const KKSList<const KKSFilterGroup *> & filters);

        static QAbstractItemModel* initCategoriesModel (KKSLoader* l, const KKSList<const KKSFilterGroup *>& catTypeFilters, const KKSList<const KKSFilterGroup *>& catFilters);

        static KKSRecWidget * createCategoryTemplates (int idCat,
                                                       KKSLoader *l,
                                                       QWidget *parent=0,
                                                       Qt::WindowFlags f=0);

        static void loadCategoryDbTemplates (const KKSList<KKSTemplate*>& lTempls,
                                             QAbstractItemModel *model,
                                             const QModelIndex& parent=QModelIndex());

        static void loadCategoryTemplates (int idCat,
                                           KKSLoader* l,
                                           QAbstractItemModel *model,
                                           const QModelIndex& parent=QModelIndex());

        static KKSRecWidget * createCategAttrsView (const KKSCategory *cat,
                                                    QWidget *parent=0,
                                                    Qt::WindowFlags f=0);

        static KKSRecWidget * createAttrAttrsView (const KKSAttribute *a,
                                                   bool mode,
                                                   QWidget *parent,
                                                   Qt::WindowFlags f);

        static KKSAttributesEditor * createAttrView (KKSLoader *l,
                                                     const KKSList<const KKSFilterGroup *> & filters,
                                                     bool mode,
                                                     QWidget *parent=0,
                                                     Qt::WindowFlags f=0);
        
        static void updateAttributesModel (KKSLoader *l, QAbstractItemModel * wModel);

        static void filterAttributesModel (KKSLoader *l, const KKSList<const KKSFilterGroup *> & filters, QAbstractItemModel * wModel);

        static void updateAttrModel (const KKSCategory *cat, QAbstractItemModel *model);
        static void updateAttrAttrsModel (const KKSAttribute *a, QAbstractItemModel *model);

        static void initTemplateGroups (KKSTemplate *t, QAbstractItemModel *tModel);
        static void insertTemplateGroup (KKSAttrGroup *tAGroup, const QModelIndex& parent, int ind, QAbstractItemModel *tModel);
        static void updateTemplateGroup (KKSAttrGroup *tAGroup, const QModelIndex& gIndex, QAbstractItemModel *tModel);

        static KKSAttributesEditor * createAvailAttrView (const KKSMap<int, KKSCategoryAttr *>& avAttrs,
                                                          QWidget *parent=0,
                                                          Qt::WindowFlags f=0);

        static QMap<int, QString> loadAttrRefs (KKSLoader *l, const KKSList<const KKSFilterGroup *> & filters);

        static KKSList<const KKSFilterGroup *> AttrRefsFilters (KKSLoader *l, int attrId, QString val_ref);

        static KKSRecWidget * createAdditionalView (KKSTemplate *t,
                                                    KKSObjEditor *objEditor,
                                                    const KKSObject * pObj,
                                                    const KKSCategory *cat,
                                                    KKSLoader *l,
                                                    QString tableName,
                                                    const KKSList<const KKSFilterGroup *> & filters=KKSList<const KKSFilterGroup *>(),
                                                    QWidget *parent=0,
                                                    Qt::WindowFlags f=0);

        static QVariant drawViewCells (const KKSCategory * cat, KKSEIOData * d, int type, KKSLoader *l, QAbstractItemModel * objModel, const QModelIndex& wIndex, int icolor=0);
        
        static QModelIndex searchModelIndex (QAbstractItemModel * sourceMod, qint64 iData, const QModelIndex& parent = QModelIndex(), int role=Qt::UserRole);
        static QModelIndex searchModelRowsIndex (QAbstractItemModel * sourceMod, qint64 iData, const QModelIndex& parent = QModelIndex(), int role=Qt::UserRole);
        static QModelIndex searchModelRowsIndexMultiType (QAbstractItemModel * sourceMod, qint64 iData, qint64 typeVal=0, const QModelIndex& parent = QModelIndex(), int role=Qt::UserRole);
        
        static KKSEIOData * getRecordData (const KKSObjectExemplar * rec);
        
        static void getSearchTemplates (KKSLoader * loader, QAbstractItemModel * searchTModel, const QModelIndex& pIndex=QModelIndex(), bool withSearchTemplates=true);

    protected:
    private:
        KKSViewFactory (void);
        ~KKSViewFactory (void);

        static void parseAttrGroup (KKSLoader *l, const KKSAGroup* aGroup, const KKSList<const KKSFilterGroup *> & filters, QAbstractItemModel * aModel, const QModelIndex& parent=QModelIndex());

};

#endif
