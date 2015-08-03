/* 
 * File:   kksstateform.h
 * Author: yuriyrusinov
 *
 * Created on 28 Май 2013 г., 15:24
 */

#ifndef KKSSTATEFORM_H
#define	KKSSTATEFORM_H

#include <QDialog>

#include "kksgui_config.h"

namespace Ui
{
    class kks_state_form;
};

class QLineEdit;
class KKSState;

class _GUI_EXPORT kksstateform : public QDialog
{
public:
    kksstateform(KKSState * st, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~kksstateform();

    KKSState * getState (void) const;
    
private slots:
    void stAccept (void);
    void setParent (void);
    void clearParent (void);

private:
    //
    // Variables
    //
    Ui::kks_state_form * UI;
    KKSState * state;
private:
    Q_OBJECT

};

#endif	/* KKSLIFECYCLEFORM_H */

