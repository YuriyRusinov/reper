/***********************************************************************
 * Module:  KKSCatAttrEditor.h
 * Author:  yuriyrusinov
 * Modified: 16.03.2009 г. 11:02:00
 * Purpose: Declaration of the class KKSCatAttrEditor
 * Comment: Данный класс унаследован от QDialog и предназначен для
 *  визуализации и редактирования атрибута внутри категории
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttrAttrEditor_h)
#define __KKSSITOOOM_KKSAttrAttrEditor_h

#include "kksgui_config.h"

#include <QDialog>

#include <KKSMap.h>

namespace Ui
{
    class kksattrattr_editor;
};

class KKSCategoryAttr;
class KKSAttrType;

class _GUI_EXPORT KKSAttrAttrEditor : public QDialog
{
public:
    KKSAttrAttrEditor (KKSCategoryAttr *attr, bool for_template=false, QWidget *parent=0);
    ~KKSAttrAttrEditor (void);

    KKSCategoryAttr * getAttrAttribute (void);
    const KKSCategoryAttr * getAttrAttribute (void) const;

    int getTypeID (void) const;

private slots:
    void accept ();

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
    Ui::kksattrattr_editor* ui;

private:
    Q_OBJECT
};

#endif
