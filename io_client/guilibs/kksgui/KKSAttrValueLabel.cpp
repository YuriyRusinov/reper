/***********************************************************************
 * Module:  KKSAttrValueLabel.cpp
 * Author:  sergey
 * Modified: 26 ����� 2012 �. 12:13:26
 * Purpose: Implementation of the class KKSAttrValueLabel
 ***********************************************************************/

#include "KKSAttrValueLabel.h"
#include "KKSAttrValue.h"
#include "KKSAttribute.h"
#include <QMessageBox>
#include <KKSAttrValuePropsForm.h>

KKSAttrValueLabel :: KKSAttrValueLabel (KKSAttrValue * av, KKSIndAttrClass isSystem, QWidget *parent)
    : QLabel (parent)
{
    m_av = NULL;
    m_isSystem = isSystem; //0 - ��������� ������� �� ��� ��������� ������� ���. �� ���� �� ���������������� ������� ����������� ������� � ������� ��������
                           //1 - ���������������� ������� ��� �� (�������� � attrs_values). �� ���� ���������������� �������� ����������� ������� � ������� ��������
                           //2 - ���������� (����������� �������) ��� ��� (�������� � rec_attrs_values). �� ���� ���������������� ������� ����������� ������� � ������� ��������
    setAttrValue(av);

    //connect( this, SIGNAL( clicked() ), this, SLOT( showAttrValueProps() ) );
}

KKSAttrValueLabel:: ~KKSAttrValueLabel (void)
{
    if (m_av)
        m_av->release ();
}

void KKSAttrValueLabel::mouseReleaseEvent ( QMouseEvent * event )
{
    QLabel::mouseReleaseEvent(event);
    showAttrValueProps();
}

/*
void KKSAttrValueLabel :: mouseMoveEvent( QMouseEvent * event )
{
    Q_UNUSED(event);
}
 */

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
    QString coloredText (text);
    bool isUnderline (false);

    if(m_isSystem != iacTableAttr &&
       m_isSystem != iacAttrAttr)
    {
        coloredText = tr("<font color='blue'>%2</font>").arg(text);
        isUnderline = true;
    }

    this->setText( coloredText );	

    QFont lFont = this->font ();
    lFont.setUnderline( isUnderline);

    if (isMandatory)
    {
        lFont.setBold (true);
    }

    this->setFont (lFont);

    if(isUnderline){
        setToolTip(tr("Click on label to show extended attribute properties"));
        setCursor(Qt::PointingHandCursor);
    }

}

void KKSAttrValueLabel :: showAttrValueProps()
{
    if(!m_av || m_isSystem == iacTableAttr || m_isSystem == iacAttrAttr)
        return;

    bool isSys = (m_isSystem == iacIOUserAttr);
    KKSAttrValuePropsForm * f = new KKSAttrValuePropsForm(m_av, 
                                                          true, 
                                                          !isSys,
                                                          this);

    connect (f, SIGNAL(loadIOSrc(KKSObject **, QWidget *)), this, SIGNAL(loadIOSrc(KKSObject **, QWidget *)));
    connect (f, SIGNAL(viewIOSrc(KKSObject *, QWidget *)), this, SIGNAL(viewIOSrc(KKSObject *, QWidget *)));
    connect (f, SIGNAL(loadHistory(const KKSAttrValue *, bool)), this, SLOT(loadHist(const KKSAttrValue *, bool)));
    connect (f, SIGNAL(viewAttrValue (const KKSAttrValue *, int, QWidget *)), this, SLOT (viewAVal(const KKSAttrValue *, int, QWidget *)));
    connect (f, SIGNAL(refreshAttrValue (const KKSAttrValue *, int)), this, SIGNAL(refreshDetailAttrVal (const KKSAttrValue *, int)) );
    connect(this, SIGNAL(viewHistory(const KKSList<KKSAttrValue *> &)), f, SLOT(viewHistory(const KKSList<KKSAttrValue *> &)));

    if(f->exec() == QDialog::Accepted)
        emit attrValueChanged();

    f->setParent (0);
    delete f;
}

void KKSAttrValueLabel :: loadHist (const KKSAttrValue * av, bool forRecs)
{
    if (!av)
        return;
    emit loadHistory (av, forRecs);
}

void KKSAttrValueLabel :: viewAVal (const KKSAttrValue * av, int idAVal, QWidget * pWidget)
{
    if (!av || idAVal < 0)
        return;
    emit viewDetailAttrVal (av, idAVal, m_isSystem, pWidget);
}

void KKSAttrValueLabel :: viewAHist (const KKSAttrValue * av, const KKSList<KKSAttrValue *> & vList)
{
    qDebug () << __PRETTY_FUNCTION__ << vList.count();
    if (av && av->id() > 0 && (!m_av || m_av->id() < 0) && m_av != av)
    {
        m_av->setId (av->id());
    }
    if (vList.count())
        emit viewHistory (vList);
}