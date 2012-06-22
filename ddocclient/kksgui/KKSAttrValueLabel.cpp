/***********************************************************************
 * Module:  KKSAttrValueLabel.cpp
 * Author:  sergey
 * Modified: 26 ����� 2012 �. 12:13:26
 * Purpose: Implementation of the class KKSAttrValueLabel
 ***********************************************************************/

#include "KKSAttrValueLabel.h"
#include "KKSAttrValue.h"
#include "KKSCategoryAttr.h"
#include <QMessageBox>
#include <KKSAttrValuePropsForm.h>

KKSAttrValueLabel :: KKSAttrValueLabel (KKSAttrValue * av, KKSIndAttr::KKSIndAttrClass isSystem, QWidget *parent)
    : QLabel (parent)
{
    m_av = NULL;
    m_isSystem = isSystem; //0 - ��������� ������� �� ��� ��������� ������� ���. �� ���� �� ���������������� ������� ����������� ������� � ������� ��������
                           //1 - ���������������� ������� ��� �� (�������� � attrs_values). �� ���� ���������������� �������� ����������� ������� � ������� ��������
                           //2 - ���������� (����������� �������) ��� ��� (�������� � rec_attrs_values). �� ���� ���������������� ������� ����������� ������� � ������� ��������
    setAttrValue(av);

    connect( this, SIGNAL( clicked() ), this, SLOT( showAttrValueProps() ) );
}

KKSAttrValueLabel:: ~KKSAttrValueLabel (void)
{
    if (m_av)
        m_av->release ();
}

void KKSAttrValueLabel::mouseReleaseEvent ( QMouseEvent * event )
{
    Q_UNUSED(event);
    emit clicked();
}

void KKSAttrValueLabel :: mouseMoveEvent( QMouseEvent * event )
{
    Q_UNUSED(event);
}

void KKSAttrValueLabel :: setAttrValue(KKSAttrValue * av) 
{
    if(m_av)
        m_av->release();

    m_av = av;

    if (m_av)
        m_av->addRef ();

    setLabelProps();
}

KKSAttrValue * KKSAttrValueLabel :: attrValue() const
{
    return m_av;
}

void KKSAttrValueLabel :: setLabelProps() 
{
    if(!m_av)
        return;

	bool isMandatory = m_av->attribute()->isMandatory();
	QString aTitle = m_av->attribute()->title();
    //bool isSystem = m_av->attribute()->isSystem();
	
	QString text = QString ("%1%2:")
		               .arg (aTitle)
					   .arg(isMandatory ? QString("*"): QString());
		
	QColor colour(Qt::darkBlue); 
    QString coloredText;
   
#ifdef Q_CC_MSVC
    if(m_isSystem != KKSIndAttr::KKSIndAttrClass::iacTableAttr)
#else
    if(m_isSystem != KKSIndAttr::iacTableAttr)
#endif
        coloredText = tr("<font color='blue'>%2</font>").arg(text);
    else
        coloredText = text;

    this->setText( coloredText );	
	
    QFont lFont = this->font ();
#ifdef Q_CC_MSVC
    lFont.setUnderline( m_isSystem != KKSIndAttr::KKSIndAttrClass::iacTableAttr ? true : false);
#else
    lFont.setUnderline( m_isSystem != KKSIndAttr::iacTableAttr ? true : false);
#endif
	if (isMandatory)
    {
        lFont.setBold (true);
    }

	this->setFont (lFont);

#ifdef Q_CC_MSVC
    if(m_isSystem != KKSIndAttr::KKSIndAttrClass::iacTableAttr){
#else
    if(m_isSystem != KKSIndAttr::iacTableAttr){
#endif
        setToolTip(tr("Click on label to show extended attribute properties"));
        setCursor(Qt::PointingHandCursor);
    }


}

void KKSAttrValueLabel :: showAttrValueProps()
{
#ifdef Q_CC_MSVC
    if(!m_av || m_isSystem == KKSIndAttr::KKSIndAttrClass::iacTableAttr)
#else
    if(!m_av || m_isSystem == KKSIndAttr::iacTableAttr)
#endif
        return;
#ifdef Q_CC_MSVC
    KKSAttrValuePropsForm * f = new KKSAttrValuePropsForm(m_av, 
                                                          true, 
                                                          m_isSystem == KKSIndAttr::KKSIndAttrClass::iacIOUserAttr ? false : true,
                                                          this);
#else
    KKSAttrValuePropsForm * f = new KKSAttrValuePropsForm(m_av, 
                                                          true, 
                                                          m_isSystem == KKSIndAttr::iacIOUserAttr ? false : true,
                                                          this);
#endif

    connect(f, SIGNAL(loadIOSrc(KKSObject **, QWidget *)), this, SIGNAL(loadIOSrc(KKSObject **, QWidget *)));
    connect(f, SIGNAL(viewIOSrc(KKSObject *, QWidget *)), this, SIGNAL(viewIOSrc(KKSObject *, QWidget *)));
    connect(f, SIGNAL(loadHistory(const KKSAttrValue *, bool)), this, SIGNAL(loadHistory(const KKSAttrValue *, bool)));
    connect(this, SIGNAL(viewHistory(const KKSList<KKSAttrValue *> &)), f, SLOT(viewHistory(const KKSList<KKSAttrValue *> &)));

    if(f->exec() == QDialog::Accepted)
        emit attrValueChanged();

    delete f;
}
