/* 
 * File:   KKSRecDialog.cpp
 * Author: yuriyrusinov
 * 
 * Created on 17 ќкт€брь 2013 г., 13:06
 */

#include <QCloseEvent>
#include <QGridLayout>
#include <QTreeView>

#include <KKSAttribute.h>
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
    if (recW == rw)
        return;
    if (recW)
    {
        recW->setParent (0);
        delete recW;
        recW = 0;
    }
    recW = rw;
    if (recW && recW->getSourceModel())
    {
        QAbstractItemModel * sMod = recW->getSourceModel();
        const KKSCategoryAttr * cAttrP = sMod->data(QModelIndex(), Qt::UserRole+3).value<const KKSCategoryAttr *>();
        if (cAttrP)
            recW->getView()->setRootIsDecorated(true);
    }
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