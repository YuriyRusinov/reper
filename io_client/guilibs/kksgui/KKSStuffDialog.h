/* 
 * File:   KKSStuffDialog.h
 * Author: yrusinov
 *
 * Created on 5 Декабрь 2012 г., 12:25
 */

#ifndef KKSSTUFFDIALOG_H
#define	KKSSTUFFDIALOG_H

#include <QDialog>

#include "kksgui_config.h"

class QPushButton;

class KKSStuffForm;

class _GUI_EXPORT KKSStuffDialog : public QDialog
{
public:
    KKSStuffDialog(KKSStuffForm * stForm, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~KKSStuffDialog();
    
    KKSStuffForm * getStuffForm (void) const;

private:
    //
    // Variables
    //
    KKSStuffForm * stuffWidget;
    QPushButton * pbOk;
    QPushButton * pbCancel;

private:
    Q_OBJECT
};

#endif	/* KKSSTUFFDIALOG_H */

