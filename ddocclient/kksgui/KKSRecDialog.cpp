/* 
 * File:   KKSRecDialog.cpp
 * Author: yuriyrusinov
 * 
 * Created on 17 ќкт€брь 2013 г., 13:06
 */

#include <QCloseEvent>
#include <QGridLayout>

#include "KKSRecWidget.h"
#include "KKSRecDialog.h"

KKSRecDialog::KKSRecDialog(bool mode, Qt::Orientation orient, QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
      m_icon (QIcon(":/ddoc/rubric_item.png")),
      recW (new KKSRecWidget (mode, orient, this))
      //gLay (new QGridLayout)
{
//    setLayout (gLay);
//    gLay->addWidget (recW);
}

KKSRecDialog::~KKSRecDialog()
{
}

qint64 KKSRecDialog::getID (void) const
{
    return recW->getID();
}

KKSRecWidget* KKSRecDialog::getRecordsWidget (void) const
{
    return recW;
}

void KKSRecDialog::setRecordsWidget (KKSRecWidget *rw)
{
    if (recW)
    {
        recW->setParent (0);
        delete recW;
        recW = 0;
    }
    recW = rw;
}

const QIcon & KKSRecDialog::icon() const
{
    return m_icon;
}

void KKSRecDialog::closeEvent (QCloseEvent * event)
{
    emit aboutToClose(this);

    QWidget::closeEvent (event);
    
}