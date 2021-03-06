/***********************************************************************
 * Module:  KKSRubricFactory.h
 * Author:  yuriy
 * Modified: 3 ����� 2009 �. 14:30:40
 * Purpose: Declaration of the class KKSRubricFactory
 * Comment: ������ ����� �������� �������� ��������� ������.
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSRubricFactory_h)
#define __KKSSITOOOM_KKSRubricFactory_h

#include <QHash>

#include "kksguifactory_config.h"

#include "KKSEntityFactory.h"
#include <kksincludeswidget.h>

#include <KKSList.h>
#include <KKSMap.h>
#include <KKSFilter.h>

class QAbstractItemModel;
class QModelIndex;
class QObject;

class KKSLoader;
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
class KKSEIOData;
class KKSObject;

/*!\ingroup FACTORY_GROUP
\brief ����� ��� ������������ ���������� ��������� ��� �������������� ������ � ������������
*/
class _GF_EXPORT KKSRubricFactory : public KKSEntityFactory
{
    public:
        
        //��� ���������� � ������������� �����������
        enum RubricMode
        {
            atRootRubric = 0, //������ ������������� ���������� (��� ���������)
            atMyDocsRubric = 1, //������ "��� ���������"
            atOthers = 2 //������������� ���������� � �����������
        };

        //����������, ���������� �� ������� � ����������, ��� ������ ���� ������� 
        //(� ��������� ������ ������ � ���������� ����� �����)
        enum RubricShowMode
        {
            smRubricsOnly = 0,
            smRubricsWithIncludes = 1
        };



        KKSIncludesWidget * createRubricEditor (RubricMode mode, 
                                                RubricShowMode showMode = smRubricsWithIncludes,
                                                bool bModal = false, //�������� ����������� ����� ������ ��������. �� ���� ����� ������ � ���, ��� �� ������, ����� �� �� ��������� �� ������ �������� ���� MDI-����������
                                                QWidget* parent=0,
                                                Qt::WindowFlags flags=0);
        

        //KKSIncludesWidget * createModalRubricEditor (RubricMode mode, 
        //                                             bool withCategories=true, 
        //                                             QWidget* parent=0);

        KKSIncludesWidget * createRubricRecEditor (KKSRubric * rootRubric,
                                                   KKSIncludesWidget::RubricatorSource rSource,
                                                   RubricShowMode showMode = smRubricsWithIncludes,
                                                   QWidget *parent=0,
                                                   Qt::WindowFlags flags=0);

        static KKSTemplate * rubrRecTemlate (void);

    signals:
        void rubricEditorCreated (KKSIncludesWidget *rEditor);
        void objEditorCreated (KKSObjEditor *oEditor);
        void rubricAttachments (QAbstractItemModel * rModel, KKSIncludesWidget * iW);

        void showReportEditor(qint64 idReport);
        void showReportViewer(qint64 idReport);


    private slots:
        void saveRubric (KKSRubric * rootR, bool isMyDocs);
        void rubricItemUpload (const KKSRubric * r, bool forRecords, QAbstractItemModel * itemModel);
        void recRubricItemUpload(const KKSRubric *r, QAbstractItemModel * itemModel, KKSObject * o, KKSIncludesWidget *editor);
        void rubricItemCreate (const KKSRubric * r, QAbstractItemModel * itemModel, const QModelIndex& parent);
        
        void openRubricItem (int idObject);
        void openRubricItem (int idObject, int idRecord);

        void loadRubricPrivilegies (RubricForm * rForm);
        void loadSearchTemplate (RubricForm * rForm);
        void loadCategory (RubricForm * rForm);
        void loadIO(RubricForm * rForm);

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
        friend class KKSApplication;
        KKSRubricFactory (KKSLoader *l, KKSPPFactory *_ppf, KKSEIOFactory * _eiof, KKSObjEditorFactory *_oef, KKSStuffFactory * _stf, QObject* parent=0);
        ~KKSRubricFactory (void);

        void initConnections(KKSIncludesWidget * iW) const; //���������� ������-���� ��� ������� �����������
        QString getEditorTitle(RubricMode mode) const; //���������� ��������� ���� � ������������
        int addCategoriesToRubricator(QAbstractItemModel * rubrMod) const;//��������� ��������� � ������ ������ (������ Others)
        KKSMap<qint64, KKSEIOData * > getRubricatorsData() const; //�������� ������ � ���� KKSEIOData ��� ������ ������

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
