/***********************************************************************
 * Module:  KKSComboBox.cpp
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:13:21
 * Purpose: Implementation of the class KKSComboBox
 ***********************************************************************/

#include "KKSListWidget.h"
#include "KKSAttrValue.h"
#include "defines.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSComboBox::KKSComboBox()
// Purpose:    Implementation of KKSComboBox::KKSComboBox()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSListWidget::KKSListWidget(const KKSAttrValue* attr, bool isSys, QWidget *parent)
    : QListWidget(parent), KKSAttrWidget(attr, isSys)
{
    
    connect (this, SIGNAL (itemChanged(QListWidgetItem*)), this, SLOT (changeValue(QListWidgetItem *)));
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSComboBox::~KKSComboBox()
// Purpose:    Implementation of KKSComboBox::~KKSComboBox()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSListWidget::~KKSListWidget()
{
}

void KKSListWidget::changeValue(QListWidgetItem * it)
{
    Q_UNUSED(it);

    QListWidgetItem * item = NULL;
    int cnt = this->count();

    QVariant val;
    QStringList sl;

    for(int i=0; i<cnt; i++){
        item = this->item(i);
        if(!item)
            break;

        if(item->checkState() == Qt::Checked){
            QVariant v = item->data(Qt::UserRole);
            int id = v.toInt();
            sl.append(QString::number(id));
        }
    }
    
    if(sl.count() > 0){
        val = QVariant(sl);
    }
    
    emit valueChanged (m_av->id(), m_isSystem, val);
}
