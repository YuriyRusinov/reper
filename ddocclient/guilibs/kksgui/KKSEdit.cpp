/***********************************************************************
 * Module:  KKSEdit.cpp
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:13:26
 * Purpose: Implementation of the class KKSEdit
 ***********************************************************************/

#include "KKSEdit.h"
#include "KKSAttrValue.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSEdit::KKSEdit()
// Purpose:    Implementation of KKSEdit::KKSEdit()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSEdit :: KKSEdit (const KKSAttrValue *attr, KKSIndAttrClass isSys, QWidget *parent)
    : QLineEdit (parent), KKSAttrWidget(attr, isSys),
    m_fixedSymCount(0)
{
    connect (this, SIGNAL (editingFinished()), this, SLOT (setVal()) );
}

KKSEdit :: KKSEdit (const KKSAttrValue *attr, KKSIndAttrClass isSys, const QString& contents, QWidget *parent)
    : QLineEdit (contents, parent), KKSAttrWidget(attr, isSys),
    m_fixedSymCount(0)
{
    
    //закомментированный вариант не будет работать в MDI-приложении kkssito_app,
    //поскольку кнопка "сохранить" вынесена на тулбар. 
    //В этом случае rditingFinished не происходит
    //вариант с textEdited работоспособен.
    //наверное возможны более "правильные" варианиты решения этой проблемы, но мы их оставим за кадром
    //connect (this, SIGNAL (editingFinished()), this, SLOT (setVal()) );
    connect (this, SIGNAL (textEdited(const QString&)), this, SLOT (setVal(const QString &)) );
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSEdit::~KKSEdit()
// Purpose:    Implementation of KKSEdit::~KKSEdit()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSEdit :: ~KKSEdit (void)
{
}

void KKSEdit :: setVal (const QString & newText)
{
    QVariant v (newText);
    emit valueChanged (m_av->id(), m_isSystem, v);
}

void KKSEdit::setFixedSymCount(int count)
{
    if(count < 0){
        m_fixedSymCount = 0;
        this->setInputMask("");
    }
    else{
        m_fixedSymCount = count;
        
        QString mask;
        for(int i=0; i<m_fixedSymCount; i++){
            mask += "x";
        }
        
        this->setInputMask(mask);
    }
}
