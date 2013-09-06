/* 
 * File:   KKSAValWidget.cpp
 * Author: yuriyrusinov
 * 
 * Created on 5 Сентябрь 2013 г., 10:52
 */

#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QAbstractItemModel>

#include <QLineEdit>
#include <QDateEdit>
#include <QDateTimeEdit>
#include <QTreeView>
#include <QCheckBox>
#include <QGridLayout>
#include <QPushButton>
#include <QToolButton>

#include <KKSAttrValue.h>
#include <KKSAttribute.h>
#include <KKSAttrType.h>
#include "KKSAValWidget.h"

KKSAValWidget::KKSAValWidget(KKSAttrValue * _av, QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
      pAttrValue (_av)
{
    if (pAttrValue)
        pAttrValue->addRef();
    
    QGridLayout * avGLay = new QGridLayout;
    setLayout (avGLay);

    const KKSAttribute * attr = pAttrValue->attribute();
    const KKSAttrType * aType = attr->type();
    KKSAttrType::KKSAttrTypes idAttrType = aType->attrType();
    switch (idAttrType)
    {
        case KKSAttrType::atBool: 
        {
            valWidget = new QCheckBox (this);
            break;
        }
        case KKSAttrType::atCheckList:
        case KKSAttrType::atCheckListEx:
        case KKSAttrType::atComplex:
        {
            valWidget = new QTreeView (this);
            break;
        }
        case KKSAttrType::atDouble:
        case KKSAttrType::atFixString:
        case KKSAttrType::atInt:
        case KKSAttrType::atInt64:
        case KKSAttrType::atString:
        case KKSAttrType::atList:
        case KKSAttrType::atParent:
        case KKSAttrType::atObjRef:
        case KKSAttrType::atUUID:
        case KKSAttrType::atUrl:
        case KKSAttrType::atRecordColor:
        case KKSAttrType::atRecordColorRef:
        case KKSAttrType::atRecordTextColor:
        case KKSAttrType::atRecordTextColorRef:
        {
            valWidget = new QLineEdit (this);
            QLineEdit * lE = (qobject_cast<QLineEdit *>(valWidget));
            lE->setReadOnly (true);
            lE->setText(pAttrValue->value().valueVariant().toString());
            break;
        }
        case KKSAttrType::atMaclabel:
        {
            valWidget = new QWidget (this);
            QHBoxLayout * hLay = new QHBoxLayout (valWidget);
            for (int i=0; i<2; i++)
            {
                QLineEdit * lE = new QLineEdit (valWidget);
                hLay->addWidget (lE);
            }
            break;
        }
        case KKSAttrType::atDate:
        {
            valWidget = new QDateEdit (this);
            QDateEdit * dE = qobject_cast<QDateEdit *>(valWidget);
            dE->setDate(pAttrValue->value().valueVariant().toDate());
            dE->setReadOnly(true);
            break;
        }
        case KKSAttrType::atTime:
        {
            valWidget = new QTimeEdit (this);
            QTimeEdit * tE = qobject_cast<QTimeEdit *>(valWidget);
            tE->setTime(pAttrValue->value().valueVariant().toTime());//QTime::fromString(pAttrValue->value().valueForInsert(), QString("hh.mm.ss")));
            tE->setReadOnly(true);
            break;
        }
        case KKSAttrType::atDateTime:
        {
            valWidget = new QDateTimeEdit (this);
            QDateTimeEdit * dtE = qobject_cast<QDateTimeEdit *>(valWidget);
            dtE->setDateTime(pAttrValue->value().valueVariant().toDateTime());//QDateTime::fromString(pAttrValue->value().valueVariant(), QString("dd.MM.yyyy hh.mm.ss")));
            dtE->setReadOnly(true);
            break;
        }
        default:break;
    }
    if (valWidget)
        avGLay->addWidget (valWidget, 0, 0, 1, 1, Qt::AlignJustify | Qt::AlignVCenter);
    QToolButton * tbUp = new QToolButton;
    tbUp->setIcon(QIcon(":/ddoc/uparrow.png"));
    QHBoxLayout * buttonsLay = new QHBoxLayout;
    //QSpacerItem * spItem = new QSpacerItem (20, 80, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    //avGLay->addItem(spItem, 1, 0, 1, 1);
    buttonsLay->addStretch(1);
    QPushButton * pbClose = new QPushButton (tr("&Close"), this);
    connect (pbClose, SIGNAL (clicked()), this, SLOT (close()));
    buttonsLay->addWidget(pbClose);
    avGLay->addLayout (buttonsLay, 1, 0, 1, 2, Qt::AlignJustify | Qt::AlignBottom);//addWidget(pbClose, 1, 1, 1, 1);

}

KKSAValWidget::~KKSAValWidget()
{
    if (pAttrValue)
        pAttrValue->release();
}

void KKSAValWidget::setDate (const KKSAttribute * a, const QDate& D)
{
    
}

void KKSAValWidget::setTime (const KKSAttribute * a, const QTime& T)
{
    
}

void KKSAValWidget::setDateTime (const KKSAttribute * a, const QDateTime& DT)
{
    
}

void KKSAValWidget::setModel (const KKSAttribute * a, QAbstractItemModel * mod)
{
    
}

void KKSAValWidget::setCheck (const KKSAttribute * a, bool ch)
{
    
}

void KKSAValWidget::setText (const KKSAttribute * a, QString text)
{
}

void KKSAValWidget::setValue (const KKSAttribute * a, QVariant val)
{
    
}

void KKSAValWidget::upVal (void)
{
    emit prevVal();
}

void KKSAValWidget::downVal (void)
{
    emit nextVal();
}


