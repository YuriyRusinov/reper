#include "kksstatusbar.h"
#include <QLabel>
#include <QHBoxLayout>

KKSStatusBar::KKSStatusBar(const QString & userName, 
                           const QString & dlName,
                           QWidget * parent)
:QStatusBar(parent)
{
    cwHellow = NULL;

    m_dlName = dlName;
    m_userName = userName;
}

KKSStatusBar::~KKSStatusBar()
{

}

void KKSStatusBar::init()
{
    if(cwHellow)
        addPermanentWidget(cwHellow, 1);
}


void KKSStatusBar::setHellow(const QPixmap & px, 
                             const QString & userName, 
                             const QString & dlName)
{
    m_userName = userName;
    m_dlName = dlName;
    
    QString hellow;
    if(m_userName.isEmpty())
        hellow = tr("Please, connect to database!");
    else
        hellow = tr("Welcome, %1! (%2)").arg(m_userName).arg(m_dlName);
    
    cwHellow = new CellWidget(px, hellow, QString::null, this);
    QLabel * l = cwHellow->label();
    QPalette p;
    p.setColor(QPalette::WindowText, Qt::red);
    l->setPalette(p);
    l->setAlignment(Qt::AlignLeft);
}

void KKSStatusBar::showStatusBar()
{
    init();
}

void KKSStatusBar::updateHellow(const QString & userName, 
                                const QString & dlName)
{
    if(!cwHellow)
        return;
    
    m_dlName = dlName;
    m_userName = userName;

    QString hellow;
    if(m_userName.isEmpty())
        hellow = tr("Please, connect to database!");
    else
        hellow = tr("Welcome, %1 (%2)").arg(m_userName).arg(m_dlName);

    cwHellow->setPersistentText(hellow);
}

//=======================================
//=======================================
CellWidget::CellWidget(const QPixmap & px, 
                       const QString & pText, 
                       const QString & text, 
                       QWidget * parent)
:QWidget(parent)
{
    setPixmap(px);
    
    QLabel * pxLabel = new QLabel(this);
    pxLabel->setPixmap(px);

    m_pText = pText;
    m_text = text;

    m_label = new QLabel(this);
    if(m_text.isEmpty())
        m_label->setText(m_pText);
    else
        m_label->setText(m_pText + ": " + m_text);
    
    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(pxLabel);
    layout->addWidget(m_label);
    
    setLayout(layout);
}

CellWidget::~CellWidget()
{
    
}

void CellWidget::setPixmap(const QPixmap & _px)
{
    px = _px;
}

void CellWidget::setPersistentText(const QString & text)
{
    m_pText = text;
    if(m_text.isEmpty())
        m_label->setText(m_pText);
    else
        m_label->setText(m_pText + ": " + m_text);
}

void CellWidget::setText(const QString & text)
{
    m_text = text;
    if(m_text.isEmpty())
        m_label->setText(m_pText);
    else
        m_label->setText(m_pText + ": " + m_text);
}

const QPixmap & CellWidget::pixmap() const
{
    return px;
}

const QString & CellWidget::text() const
{
    return m_text;
}

const QString & CellWidget::persistentText() const
{
    return m_pText;
}

QLabel * CellWidget::label() const
{
    return m_label;
}
