/***********************************************************************
 * Module:  KKSCatAttrEditor.h
 * Author:  yuriyrusinov
 * Modified: 16.03.2009 �. 11:02:00
 * Purpose: Declaration of the class KKSCatAttrEditor
 * Comment: ������ ����� ����������� �� QDialog � ������������ ���
 *  ������������ � �������������� �������� ������ ���������
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

class KKSAttrAttr;
class KKSAttrType;

class _GUI_EXPORT KKSAttrAttrEditor : public QDialog
{
    public:
        KKSAttrAttrEditor (KKSAttrAttr *attr, bool for_template=false, QWidget *parent=0);
        ~KKSAttrAttrEditor (void);

        KKSAttrAttr * getAttrAttribute (void);
        const KKSAttrAttr * getAttrAttribute (void) const;

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
        KKSAttrAttr *attribute;
        Ui::kksattrattr_editor* ui;

    private:
        Q_OBJECT
};

#endif
