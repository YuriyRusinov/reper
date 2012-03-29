/* 
 * File:   KKSAttrValuePropsForm.h
 * Author: ksa
 *
 * Created on 27 марта 2012 г., 22:04
 */

#ifndef KKSAttrValuePropsFORM_H
#define	KKSAttrValuePropsFORM_H

#include <QDialog>
#include "kksgui_config.h"

class QLineEdit;

namespace Ui
{
    class kksattr_value_props_form;
};

class KKSAttrValue;
class KKSObject;

class _GUI_EXPORT KKSAttrValuePropsForm : public QDialog
{
public:
    KKSAttrValuePropsForm(KKSAttrValue * av, bool mode=true, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~KKSAttrValuePropsForm();

    void setAttrValue(KKSAttrValue * av);
    KKSAttrValue * attrValue (void) const;

private slots:
    void viewIOSource (void);
    void loadIOSource (void);
    void viewIOTransfer (void);
    void loadIOTransfer (void);

    void saveAttrValue (void);
    void apply (void);
    
    void chToStateChanged (int state);

signals:
    void viewIOSrc (KKSObject*, QWidget *);
    //void viewIOSrc1 (KKSObject*);
    void loadIOSrc (KKSObject **);
    //void loadIOSrc1 (const KKSAttrValue*, QLineEdit *);
private:
    //
    // Functions
    //
    void initForm (bool mode);
private:
    //
    // Variables
    //
    Ui::kksattr_value_props_form *UI;
    KKSAttrValue* m_av;

    KKSObject * ioSrc;
    KKSObject * ioSrc1;
private:
    Q_OBJECT
};

#endif	/* KKSAttrValuePropsFORM_H */
