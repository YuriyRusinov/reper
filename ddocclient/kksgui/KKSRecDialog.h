/* 
 * File:   KKSRecDialog.h
 * Author: yuriyrusinov
 *
 * Created on 17 ќкт€брь 2013 г., 13:06
 */

#ifndef KKSRECDIALOG_H
#define	KKSRECDIALOG_H

#include <QDialog>
#include <QIcon>

#include "kksgui_config.h"

class QGridLayout;
class QCloseEvent;

class KKSRecWidget;

class _GUI_EXPORT KKSRecDialog : public QDialog
{
public:
    KKSRecDialog(bool mode=false, Qt::Orientation orient=Qt::Vertical, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~KKSRecDialog();
    
    virtual qint64 getID (void) const;

    KKSRecWidget* getRecordsWidget (void) const;
    void setRecordsWidget (KKSRecWidget *rw);

    const QIcon & icon() const;

protected:
    virtual void closeEvent (QCloseEvent * event);

    QIcon m_icon; // иконка, котора€ отображаетс€ в меню и тулбаре главного окна, когда данное окно отображено как QMdiSubWindow

signals:
    void aboutToClose(KKSRecDialog *);

private:
    //
    // Variables
    //
    KKSRecWidget * recW;
//    QGridLayout * gLay;
private:
    Q_OBJECT

};

#endif	/* KKSRECDIALOG_H */

