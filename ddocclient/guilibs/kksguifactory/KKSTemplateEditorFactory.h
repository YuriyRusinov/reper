/***********************************************************************
 * Module:  KKSTemplateEditorFactory.h
 * Author:  yuriy
 * Modified: 13 марта 2009 г. 16:15:50
 * Purpose: Declaration of the class KKSTemplateEditorFactory
 * Comment: данный класс является фабрикой редакторов шаблонов
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSTemplateEditorFactory_h)
#define __KKSSITOOOM_KKSTemplateEditorFactory_h

//#include <KKSObjEditor.h>
//#include <KKSTemplate.h>
#include "kksguifactory_config.h"

#include "KKSEntityFactory.h"

class QAbstractItemModel;
class QModelIndex;

class KKSLoader;
class KKSPPFactory;
class KKSCatEditorFactory;
class KKSCategoryTemplateWidget;
class KKSTemplateEditor;
class KKSTemplate;
class KKSCategory;
class KKSAttrGroup;

/*!\ingroup FACTORY_GROUP

\brief Класс для формирования визуальных элементов для редактора шаблонов
*/
class _GF_EXPORT KKSTemplateEditorFactory : public KKSEntityFactory
{
    public:
        KKSCatEditorFactory *getCatEditorFactory (void) const;
        KKSTemplateEditor *createTemplateEditor (KKSTemplate *t,
                                                 bool mode=false,
                                                 Qt::WindowModality windowModality=Qt::NonModal,
                                                 QWidget *parent=0,
                                                 Qt::WindowFlags f=0);

    signals:
        void templateEditorCreated (KKSTemplateEditor *tEditor);
        void templateEditorCreatedModal (KKSTemplateEditor *tEditor);
        void templateAdded (KKSTemplate *templ);
        void templateDbError ();
        void templateDeleted (KKSTemplate * t);
        void saveSuccess ();

    private slots:
        void addTemplate (QWidget *ctw, KKSCategory *c, QAbstractItemModel * templMod);
        void editTemplate (QWidget *ctw, int idTempl, QAbstractItemModel * templMod, const QModelIndex& tIndex);
        void editCatTemplate (QWidget *ctw, KKSTemplate *t, QAbstractItemModel * templMod, const QModelIndex& tIndex);
        void delTemplate (QWidget *ctw, int idTempl, QAbstractItemModel * templMod, const QModelIndex& tIndex);

        void addAGroupToTemplate (KKSTemplate *t, const QModelIndex& parent, KKSTemplateEditor *tEditor);
        void editAGroupInTemplate (int idAttrGroup, const QModelIndex& gIndex, KKSTemplate *t, KKSTemplateEditor *tEditor);
        void delAGroupInTemplate (int idAttrGroup, const QModelIndex& gIndex, KKSTemplate *t, KKSTemplateEditor *tEditor);

        void addAttrToTemplate (int idAttrGroup, const QModelIndex& gIndex, KKSTemplate *t, KKSTemplateEditor *tEditor);
        void appendAttrToTemplate (int idAttr, int idAttrGroup, const QModelIndex& gIndex, KKSTemplate *t, KKSTemplateEditor *tEditor);
        //void pushAttrToTemplate (int idAttr, int idAttrGroup, KKSTemplate *t, KKSTemplateEditor *tEditor);
        void editAttrInTemplate (int idAttr, int idAttrGroup, const QModelIndex& aIndex, const QModelIndex& gIndex, KKSTemplate *t, KKSTemplateEditor *tEditor);
        void deleteAttrFromTemplate (int idAttr, int idAttrGroup, const QModelIndex& aIndex, const QModelIndex& gIndex, KKSTemplate *t, KKSTemplateEditor *tEditor);
        void saveTemplate (KKSTemplate *t, KKSTemplateEditor *tEditor);
        void updateTemplate (KKSTemplate *t, KKSTemplateEditor *tEditor);

    private:
        //
        // Functions
        //
        friend class KKSApplication;
        KKSTemplateEditorFactory (KKSLoader *l, KKSPPFactory *_ppf, QObject *parent=0);
        ~KKSTemplateEditorFactory (void);

        void setCatEditorFactory (KKSCatEditorFactory *cf);

        void initTemplateConnections (KKSTemplateEditor *tEditor);
        KKSTemplate * initTemplate (int idTemplate, QString name=QString(), KKSCategory *c=0);

    private:
        //
        // Variables
        //
        KKSLoader *loader;
        KKSPPFactory *ppf;
        KKSCatEditorFactory *catf;

    private:
        Q_OBJECT
};

#endif
