/***********************************************************************
 * Module:  KKSAttributesEditor.h
 * Author:  yuriyrusinov
 * Modified: 27.01.2009 г. 11:32:00
 * Purpose: Declaration of the class KKSAttributesEditor
 * Comment: Данный класс унаследован от KKSDialog и предназначен для
 *  визуализации списка существующих атрибутов.
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttributesEditor_h)
#define __KKSSITOOOM_KKSAttributesEditor_h

#include "kksgui_config.h"

#include "KKSDialog.h"

#include <KKSMap.h>

class QAbstractItemModel;
class QItemSelectionModel;
class QModelIndex;
class QTreeView;
class QMouseEvent;
class QDragEnterEvent;
class QAction;

class KKSRecWidget;
class KKSCategoryAttr;
class KKSAttribute;
class KKSAttrType;
class KKSAttrEditor;
class KKSAGroup;

class _GUI_EXPORT KKSAttributesEditor : public KKSDialog
{
public:
    KKSAttributesEditor (const KKSMap<int, KKSAttrType *>& aTypes, const KKSMap<int, KKSAGroup *>& aGroups, const QMap<int, QString>& io_refs, QWidget *parent=0, Qt::WindowFlags f=0);
    KKSAttributesEditor (QWidget *parent=0, Qt::WindowFlags f=0);
    ~KKSAttributesEditor (void);

    int getAttrId (void) const;
    QList<int> getAttributesId (void) const;
    QAbstractItemModel * getModel (void) const;
    QItemSelectionModel * getSelectionModel (void) const;
    QModelIndex getCurrentIndex (void) const;
    KKSMap<int, KKSAttrType *> getTypes (void) const;
    QMap<int, QString> getReferences (void) const;
    QTreeView * getView (void) const;
    KKSMap<int, KKSAGroup *> getAvailableGroups (void) const;

private slots:
    void addAttribute (void);
    void editAttribute (void);
    void delAttribute (void);

    void addAGroup (void);
    void editAGroup (void);
    void delAGroup (void);

    void uploadAttribute (KKSAttribute *attr);

    void getRefFields (KKSAttribute * attr, int idRef, KKSAttrEditor * aEditor);
    void slotGetSearchTemplate(KKSAttrEditor * aEditor);

    void searchAttrs (void);
    void setAttr (const QModelIndex & index);

signals:
    void insertAttr (const QModelIndex& parent, QAbstractItemModel *aModel, KKSAttributesEditor *attrEditor);
    void updateAttr (int idAttr, QAbstractItemModel *aModel, const QModelIndex& parent, KKSAttributesEditor *attrEditor);
    void deleteAttr (int idAttr, QAbstractItemModel *aModel, const QModelIndex& parent, KKSAttributesEditor *attrEditor);
    //void showAttrsWidget(KKSAttribute *, KKSAttrEditor *);//показать виджет с атрибутами для случая с описывающими атрибутами атрибута

    void showAttrsWidget(KKSAttribute *, KKSAttrEditor *);//показать виджет с атрибутами для случая с описывающими атрибутами атрибута
    void addAttribute(KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *);
    void editAttribute(int, KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *);
    void delAttribute(int, KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *);


//    void requestAttr (int idAttr, KKSAttributesEditor *);

    void insertAttrGroup (QAbstractItemModel *aModel, const QModelIndex& parent, KKSAttributesEditor *attrEditor);
    void updateAttrGroup (int idAttrGroup, QAbstractItemModel *aModel, const QModelIndex& aIndex, KKSAttributesEditor *attrEditor);
    void deleteAttrGroup (int idAttrGroup, QAbstractItemModel *aModel, const QModelIndex& aIndex, KKSAttributesEditor *attrEditor);

    void getFieldsOfReference (KKSAttribute * attr, int idRef, KKSAttrEditor * aEditor);
    void getSearchTemplate( KKSAttrEditor * aEditor );

    void findInAttributes (int idAttrRef, QAbstractItemModel *aModel, KKSAttributesEditor * attrsEditor);

protected:
    //
    // For drag override
    //
    void mousePressEvent (QMouseEvent *event);
    void dragEnterEvent (QDragEnterEvent *event);

private:
    //
    // Functions
    //
    void init_widgets (void);
    void setRecordsWidget (KKSRecWidget * rw);

private:
    //
    // Variables
    //
    friend class KKSObjEditorFactory;
    friend class KKSViewFactory;

    KKSRecWidget *recW;
    KKSMap<int, KKSAttrType *> attrTypes;
    KKSMap<int, KKSAGroup *> attrsGroups;
    QMap<int, QString> ioRefs;

    QAction * aGroupAdd;
    QAction * aGroupEdit;
    QAction * aGroupDel;

private:
    Q_OBJECT
};

#endif
