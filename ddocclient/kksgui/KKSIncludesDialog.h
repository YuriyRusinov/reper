/* 
 * File:   KKSIncludesDialog.h
 * Author: yrusinov
 *
 * Created on 5 Декабрь 2012 г., 12:25
 */

#ifndef KKSINCLUDESDIALOG_H
#define	KKSINCLUDESDIALOG_H

#include <QDialog>

#include "kksgui_config.h"

class QPushButton;

class KKSIncludesWidget;

class _GUI_EXPORT KKSIncludesDialog : public QDialog
{
public:
    KKSIncludesDialog(KKSIncludesWidget * iW, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~KKSIncludesDialog();
    
    KKSIncludesWidget * getIncWidget (void) const;

private:
    //
    // Variables
    //
    KKSIncludesWidget * incWidget;
    QPushButton * pbOk;
    QPushButton * pbCancel;

private:
    Q_OBJECT
};

#endif	/* KKSINCLUDESDIALOG_H */

