/* 
 * File:   KKSRecDialog.h
 * Author: yuriyrusinov
 *
 * Created on 17 ������� 2013 �., 13:06
 */

#ifndef KKSRECDIALOG_H
#define	KKSRECDIALOG_H

#include <QDialog>
#include <QIcon>
#include <QAbstractItemView>

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
    virtual QList<qint64> getIDList (void) const;

    KKSRecWidget* getRecordsWidget (void) const;
    void setRecordsWidget (KKSRecWidget *rw);

    QAbstractItemView::SelectionMode selectionMode (void) const;
    void setSelectionMode ( QAbstractItemView::SelectionMode mode );

    const QIcon & icon() const;

protected:
    //
    // override functions
    //
    virtual void closeEvent (QCloseEvent * event);

    QIcon m_icon; // ������, ������� ������������ � ���� � ������� �������� ����, ����� ������ ���� ���������� ��� QMdiSubWindow

signals:
    void aboutToClose(QWidget *);

protected:
    //
    // Variables
    //
    KKSRecWidget * recW;
//    QGridLayout * gLay;
private:
    Q_OBJECT

};

#endif	/* KKSRECDIALOG_H */

