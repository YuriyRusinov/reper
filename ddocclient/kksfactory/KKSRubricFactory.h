/***********************************************************************
 * Module:  KKSRubricFactory.h
 * Author:  yuriy
 * Modified: 3 ����� 2009 �. 14:30:40
 * Purpose: Declaration of the class KKSRubricFactory
 * Comment: ������ ����� �������� �������� ��������� ������.
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSRubricFactory_h)
#define __KKSSITOOOM_KKSRubricFactory_h

#include "kksfactory_config.h"

#include "KKSEntityFactory.h"

#include <KKSList.h>
#include <KKSFilter.h>

class QAbstractItemModel;
class QModelIndex;

class KKSLoader;
class KKSIncludesWidget;
class KKSFilterGroup;
class KKSPPFactory;
class KKSRubric;
class KKSObjEditorFactory;
class KKSObjEditor;
class KKSStuffFactory;
class RubricForm;

class _F_EXPORT KKSRubricFactory : public KKSEntityFactory
{
    public:
        KKSIncludesWidget * createRubricEditor (int mode, const KKSList<const KKSFilterGroup *>& filters, QWidget* parent=0);
        enum RubricMode
        {
            atRootRubric = 0,
            atMyDocsRubric = 1
        };

    signals:
        void rubricEditorCreated (KKSIncludesWidget *rEditor);
        void objEditorCreated (KKSObjEditor *oEditor);

    private slots:
        void saveRubric (KKSRubric * rootR, bool isMyDocs);
        void rubricItemUpload (void);
        void openRubricItem (int idObject);
        void loadRubricPrivilegies (RubricForm * rForm);
        void loadSearchTemplate (RubricForm * rForm);
        void loadCategory (RubricForm * rForm);
        void copyFromRubric (KKSRubric * rDest, QAbstractItemModel * attachModel, const QModelIndex& wIndex);
        void viewAttachments (QAbstractItemModel * attachModel, const KKSRubric * r);

    private:
        //
        // Functions
        //
        friend class KKSSito;
        KKSRubricFactory (KKSLoader *l, KKSPPFactory *_ppf, KKSObjEditorFactory *_oef, KKSStuffFactory * _stf, QObject* parent=0);
        ~KKSRubricFactory (void);

    private:
        //
        // Variables
        //
        KKSLoader *loader;
        KKSPPFactory *ppf;
        KKSObjEditorFactory *oef;
        KKSStuffFactory * stf;

    private:
        Q_OBJECT
};

#endif
