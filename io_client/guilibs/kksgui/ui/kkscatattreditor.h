/***********************************************************************
 * Module:  KKSCatAttrEditor.h
 * Author:  yuriyrusinov
 * Modified: 16.03.2009 г. 11:02:00
 * Purpose: Declaration of the class KKSCatAttrEditor
 * Comment: Данный класс унаследован от QDialog и предназначен для
 *  визуализации и редактирования атрибута внутри категории
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSCatAttrEditor_h)
#define __KKSSITOOOM_KKSCatAttrEditor_h

#include "kksgui_config.h"

#include <QDialog>

#include <KKSMap.h>

namespace Ui
{
    class kkscatattr_editor;
};

class KKSCategoryAttr;
class KKSAttrType;

class _GUI_EXPORT KKSCatAttrEditor : public QDialog
{
public:
    KKSCatAttrEditor (KKSCategoryAttr *attr, const KKSMap<int, KKSAttrType *>& aTypes, bool for_template=false, QWidget *parent=0);
    virtual ~KKSCatAttrEditor (void);

    KKSCategoryAttr * getCatAttribute (void);
    const KKSCategoryAttr * getCatAttribute (void) const;

    int getTypeID (void) const;

private slots:
    void accept ();
    void uploadAttributes ();
    void currentAttrTypeChanged (int index);

private:
    //
    // Functions
    //
    void init_widgets (bool isCat);
    void set_types (void);

private:
    //
    // Variables
    //
    KKSCategoryAttr *attribute;
    KKSMap<int, KKSAttrType *> attrTypes;
    Ui::kkscatattr_editor* ui;

private:
    Q_OBJECT
};

#endif
