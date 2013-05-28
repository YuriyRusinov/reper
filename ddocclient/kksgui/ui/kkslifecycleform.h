/* 
 * File:   kkslifecycleform.h
 * Author: yuriyrusinov
 *
 * Created on 28 Май 2013 г., 15:24
 */

#ifndef KKSLIFECYCLEFORM_H
#define	KKSLIFECYCLEFORM_H

#include <QDialog>

#include "kksgui_config.h"

namespace Ui
{
    class kks_life_cycle_form;
};

class KKSLifeCycleEx;

class _GUI_EXPORT kkslifecycleform : public QDialog
{
public:
    kkslifecycleform(KKSLifeCycleEx * lc, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~kkslifecycleform();

    KKSLifeCycleEx * getLC (void) const;

private slots:
    void lcAccept (void);
    void setState (void);
    void clearState (void);

private:
    //
    // Variables
    //
    Ui::kks_life_cycle_form * UI;
    KKSLifeCycleEx * lifeCycle;
private:
    Q_OBJECT

};

#endif	/* KKSLIFECYCLEFORM_H */

