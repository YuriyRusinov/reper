/***********************************************************************
 * Module:  KKSText.cpp
 * Author:  sergey
 * Modified: 1 ������� 2008 �. 12:13:26
 * Purpose: Implementation of the class KKSEdit
 ***********************************************************************/

#include "KKSText.h"
#include "KKSAttrValue.h"


KKSText :: KKSText (const KKSAttrValue *attr, KKSIndAttrClass isSys, QWidget *parent)
    : QTextEdit (parent), KKSAttrWidget(attr, isSys),
    m_fixedSymCount(0)
{
    connect (this, SIGNAL (textChanged()), this, SLOT (setValPrivate()) );
}

KKSText :: KKSText (const KKSAttrValue *attr, KKSIndAttrClass isSys, const QString& contents, QWidget *parent)
    //: QTextEdit (contents, parent),
    : QTextEdit (parent), KKSAttrWidget(attr, isSys),
    m_fixedSymCount(0)
{
    
    this->setPlainText(contents);

    //������������������ ������� �� ����� �������� � MDI-���������� kkssito_app,
    //��������� ������ "���������" �������� �� ������. 
    //� ���� ������ editingFinished �� ����������
    //������� � textEdited ��������������.
    //�������� �������� ����� "����������" ��������� ������� ���� ��������, �� �� �� ������� �� ������
    //connect (this, SIGNAL (editingFinished()), this, SLOT (setVal()) );
    connect (this, SIGNAL (textChanged()), this, SLOT (setValPrivate()) );
}


KKSText :: ~KKSText (void)
{
}

void KKSText :: setVal (const QString & newText)
{
    QVariant v (newText);
    emit valueChanged (m_av->id(), m_isSystem, v);
}

void KKSText :: setValPrivate ()
{
    QString newText = this->toPlainText();
    QVariant v (newText);
    emit valueChanged (m_av->id(), m_isSystem, v);
}

void KKSText::setFixedSymCount(int count)
{
    if(count < 0){
        m_fixedSymCount = 0;
        this->setLineWrapMode(QTextEdit::WidgetWidth);
        this->setLineWrapColumnOrWidth(0);
    }
    else{
        m_fixedSymCount = count;
        this->setLineWrapMode(QTextEdit::FixedColumnWidth);
        this->setLineWrapColumnOrWidth(m_fixedSymCount);
    }
}
