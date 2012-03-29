/***********************************************************************
 * Module:  KKSAttrEditor.h
 * Author:  yuriyrusinov
 * Modified: 24.01.2009 г. 11:32:00
 * Purpose: Declaration of the class KKSAttrEditor
 * Comment: ƒанный класс унаследован от KKSDialog и предназначен дл€
 *  визуализации и редактировани€ атрибута категории
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttrEditor_h)
#define __KKSSITOOOM_KKSAttrEditor_h

#include "kksgui_config.h"

#include <QDialog>
#include <KKSMap.h>

namespace Ui
{
    class kksattr_editor;
};

class KKSAttribute;
class KKSAttrType;
class KKSAGroup;
class KKSObject;
class KKSSearchTemplate;

class _GUI_EXPORT KKSAttrEditor : public QDialog
{
    public:
        KKSAttrEditor (KKSAttribute *attr, const KKSMap<int, KKSAttrType *>& aTypes, const KKSMap<int, KKSAGroup*>& aGroups, const QMap<int, QString>& Refs, QWidget *parent=0);
        ~KKSAttrEditor (void);

        KKSAttribute * getAttribute (void);
        const KKSAttribute * getAttribute (void) const;

        int getTypeID (void) const;
        int getGroupId (void) const;
        void setGroupId (int idGroup);

        void uploadReferenceFields (const QMap<QString, QString>& fields);
        void setIO (KKSObject *o);
        void setCurrentRef (int index);
        void setCurrentRef (const QString& text);
        void setSearchTemplate(KKSSearchTemplate * st);

    signals:
        void getReferences (KKSAttribute * attr, KKSAttrEditor * aEditor);
        void getReferenceFields (KKSAttribute * attr, int idRef, KKSAttrEditor * aEditor);
        void getSearchTemplate(KKSAttrEditor * aEditor);

    private slots:
        void accept ();
        void currentAttrTypeChanged (int index);
        void currentReferenceChanged (int index);
        void setCodeEnabled (int state);
        void slotAddFilterClicked();
        void slotDelFilterClicked();
        void setTitleText (const QString& text);

    private:
        //
        // Functions
        //
        void init_widgets (void);
        void set_types (void);
        void set_references (void);
        void set_groups (void);

    private:
        //
        // Variables
        //
        KKSAttribute *attribute;
        KKSMap<int, KKSAttrType *> attrTypes;
        KKSMap<int, KKSAGroup *> attrsGroups;
        QMap<int, QString> ioRefs;
        Ui::kksattr_editor* ui;
        KKSObject *io;
        QList<int> cRefTypes;

        KKSSearchTemplate * searchTemplate;//текущий фильтр (если не задан, то < 0)

    private:
        Q_OBJECT
};

#endif
