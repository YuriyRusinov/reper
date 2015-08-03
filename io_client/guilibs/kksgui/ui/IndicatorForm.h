/* 
 * File:   IndicatorForm.h
 * Author: yuriy
 *
 * Created on 28 Февраль 2012 г., 22:04
 */

#ifndef _INDICATORFORM_H
#define	_INDICATORFORM_H

#include <QDialog>
#include "kksgui_config.h"

class QLineEdit;

namespace Ui
{
    class indicator_form;
};

class KKSIndicatorValue;

class _GUI_EXPORT IndicatorForm : public QDialog
{
public:
    IndicatorForm (KKSIndicatorValue * av, bool mode=true, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~IndicatorForm();

    void setIndValue(KKSIndicatorValue * av);
    KKSIndicatorValue * indValue (void) const;

private slots:
    void loadIndType (void);
    void viewIOSource (void);
    void loadIOSource (void);
    void viewIOTransfer (void);
    void loadIOTransfer (void);

    void saveInd (void);
    void applyInd (void);
    
    void chToStateChanged (int state);

signals:
    void loadIndicatorType (KKSIndicatorValue *, QLineEdit *);
    void viewIOSrc (KKSIndicatorValue*);
    void viewIOSrc1 (KKSIndicatorValue*);
    void loadIOSrc (KKSIndicatorValue*, QLineEdit *);
    void loadIOSrc1 (KKSIndicatorValue*, QLineEdit *);
private:
    //
    // Functions
    //
    void initForm (bool mode);
private:
    //
    // Variables
    //
    Ui::indicator_form *UI;
    KKSIndicatorValue* indicatorVal;
private:
    Q_OBJECT
};

#endif	/* _INDICATORFORM_H */
