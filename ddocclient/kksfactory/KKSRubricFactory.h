/***********************************************************************
 * Module:  KKSRubricFactory.h
 * Author:  yuriy
 * Modified: 3 марта 2009 г. 14:30:40
 * Purpose: Declaration of the class KKSRubricFactory
 * Comment: данный класс является фабрикой редактора рубрик.
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSRubricFactory_h)
#define __KKSSITOOOM_KKSRubricFactory_h

#include <QHash>

#include "kksfactory_config.h"

#include "KKSEntityFactory.h"

#include <KKSList.h>
#include <KKSFilter.h>

class QAbstractItemModel;
class QModelIndex;
class QObject;

class KKSLoader;
class KKSIncludesWidget;
class KKSFilterGroup;
class KKSPPFactory;
class KKSRubric;
class KKSRubricItem;
class KKSObjEditorFactory;
class KKSObjEditor;
class KKSStuffFactory;
class RubricForm;
class KKSObjectExemplar;
class KKSEIOFactory;
class KKSTemplate;

class _F_EXPORT KKSRubricFactory : public KKSEntityFactory
{
    public:
        KKSIncludesWidget * createRubricEditor (int mode, const KKSList<const KKSFilterGroup *>& filters, bool withCategories=true, QWidget* parent=0);
        enum RubricMode
        {
            atRootRubric = 0,
            atMyDocsRubric = 1,
            atOthers = 2
        };
        KKSIncludesWidget * createModalRubricEditor (int mode, const KKSList<const KKSFilterGroup *>& filters, bool withCategories=true, QWidget* parent=0);
        KKSIncludesWidget * createRubricRecEditor (KKSRubric * rootRubric,
                                     bool isAttach,
                                     bool isDocs,
                                     bool forCategory,
                                     bool forRecord,
                                     QWidget *parent=0,
                                     Qt::WindowFlags flags=0);
        static KKSTemplate * rubrRecTemlate (void);

    signals:
        void rubricEditorCreated (KKSIncludesWidget *rEditor);
        void objEditorCreated (KKSObjEditor *oEditor);
        void rubricAttachments (QAbstractItemModel * rModel, KKSIncludesWidget * iW);

    private slots:
        void saveRubric (KKSRubric * rootR, bool isMyDocs);
        void rubricItemUpload (const KKSRubric * r, bool forRecords, QAbstractItemModel * itemModel);
        void rubricItemCreate (const KKSRubric * r, QAbstractItemModel * itemModel, const QModelIndex& parent);
        void openRubricItem (int idObject);
        void loadRubricPrivilegies (RubricForm * rForm);
        void loadSearchTemplate (RubricForm * rForm);
        void loadCategory (RubricForm * rForm);
        void copyFromRubric (KKSRubric * rDest, QAbstractItemModel * attachModel, const QModelIndex& wIndex);
        void viewAttachments (QAbstractItemModel * attachModel, const KKSRubric * r);
        void initRubricAttachments (const KKSRubric * r, bool isRec);
        void appendRubricItem (QAbstractItemModel * attachModel, const KKSRubricItem * rItem);
        void rubricItemCreated(KKSObjectExemplar * rec);
        void objEditorClosed ();
        void appendRecord (int idObject, const KKSRubric* r, QAbstractItemModel * attachModel, const QModelIndex& parent);
        void setSyncSettings (const QList<int>& ioIDList);
        void putIntoRubr (const QList<int>& ioIDList, const KKSRubric * oldRubric);
        void sendDocs (const QList<int>& ioIDList);
        void setAccessDocs (const QList<int>& ioIDList);
        KKSRubric * loadRubric (KKSRubric * r, int idRubr, QAbstractItemModel * rubrModel, const QModelIndex& rubrIndex);
        void loadCategoryIntoRubric (KKSRubric * r);

    private:
        //
        // Functions
        //
        friend class KKSSito;
        KKSRubricFactory (KKSLoader *l, KKSPPFactory *_ppf, KKSEIOFactory * _eiof, KKSObjEditorFactory *_oef, KKSStuffFactory * _stf, QObject* parent=0);
        ~KKSRubricFactory (void);

    private:
        //
        // Variables
        //
        KKSLoader *loader;
        KKSPPFactory *ppf;
        KKSEIOFactory *eiof;
        KKSObjEditorFactory *oef;
        KKSStuffFactory * stf;
        
        QHash<KKSObjectExemplar *, KKSRubric *> ioRubrs;
        QHash<KKSObjectExemplar *, QAbstractItemModel *> ioModels;
        QHash<KKSObjectExemplar *, QModelIndex > ioParents;
        QHash<KKSObjectExemplar *, KKSIncludesWidget*> includesW;

    private:
        Q_OBJECT
};

#endif
