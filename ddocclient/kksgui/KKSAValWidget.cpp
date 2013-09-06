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
#include <QGroupBox>
#include <QGridLayout>
#include <QPushButton>
#include <QStackedLayout>
#include <QToolButton>

#include <KKSAttrValue.h>
#include <KKSAttribute.h>
#include <KKSAttrType.h>
#include "KKSAValWidget.h"

KKSAValWidget::KKSAValWidget(KKSAttrValue * _av, QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
      pAttrValue (_av),
      valWidget (new QWidget()),
      stLay (new QStackedLayout (valWidget)),
      lEVal (new QLineEdit),
      dEVal (new QDateEdit),
      tEVal (new QTimeEdit),
      dtEVal (new QDateTimeEdit),
      tvVal (new QTreeView),
      chVal (new QCheckBox),
      gbVal (new QGroupBox)
{
    if (pAttrValue)
        pAttrValue->addRef();
    
    QGridLayout * avGLay = new QGridLayout;
    setLayout (avGLay);
    if (pAttrValue)
        chVal->setText (pAttrValue->attribute()->name());
    stLay->addWidget (lEVal);
    stLay->setAlignment(lEVal, Qt::AlignLeft | Qt::AlignVCenter);
    stLay->addWidget (dEVal);
    stLay->setAlignment(dEVal, Qt::AlignLeft | Qt::AlignVCenter);
    stLay->addWidget (tEVal);
    stLay->setAlignment(tEVal, Qt::AlignLeft | Qt::AlignVCenter);
    stLay->addWidget (dtEVal);
    stLay->setAlignment(dtEVal, Qt::AlignLeft | Qt::AlignVCenter);
    stLay->addWidget (tvVal);
    stLay->setAlignment(tvVal, Qt::AlignJustify | Qt::AlignVCenter);
    stLay->addWidget(chVal);
    stLay->setAlignment(chVal, Qt::AlignLeft | Qt::AlignVCenter);
    stLay->addWidget(gbVal);
    stLay->setAlignment(gbVal, Qt::AlignJustify | Qt::AlignVCenter);

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
            lEVal->setText(pAttrValue->value().valueVariant().toString());
            lEVal->setReadOnly (true);
            break;
        }
        case KKSAttrType::atMaclabel:
        {
/*            valWidget = new QWidget (this);
            QHBoxLayout * hLay = new QHBoxLayout (valWidget);
            for (int i=0; i<2; i++)
            {
                QLineEdit * lE = new QLineEdit (valWidget);
                hLay->addWidget (lE);
            }*/
            break;
        }
        case KKSAttrType::atDate:
        {
            dEVal->setDate(pAttrValue->value().valueVariant().toDate());
            dEVal->setReadOnly(true);
            break;
        }
        case KKSAttrType::atTime:
        {
            tEVal->setTime(pAttrValue->value().valueVariant().toTime());//QTime::fromString(pAttrValue->value().valueForInsert(), QString("hh.mm.ss")));
            tEVal->setReadOnly(true);
            break;
        }
        case KKSAttrType::atDateTime:
        {
            dtEVal->setDateTime(pAttrValue->value().valueVariant().toDateTime());//QDateTime::fromString(pAttrValue->value().valueVariant(), QString("dd.MM.yyyy hh.mm.ss")));
            dtEVal->setReadOnly(true);
            break;
        }
        default:break;
    }

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
    delete gbVal;
    delete chVal;
    delete tvVal;
    delete dtEVal;
    delete tEVal;
    delete dEVal;
    delete lEVal;
    delete valWidget;
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
    if (!a || !a->id() != pAttrValue->attribute()->id())
        return;
    const KKSAttribute * attr = pAttrValue->attribute();
    const KKSAttrType * aType = attr->type();
    KKSAttrType::KKSAttrTypes idAttrType = aType->attrType();
    switch (idAttrType)
    {
        case KKSAttrType::atBool: 
        {
            bool isCh = val.toBool();
            stLay->setCurrentIndex (5);
            //QCheckBox * ch = (qobject_cast<QCheckBox *>(stLay->widget(5)));
            chVal->setCheckState (isCh ? Qt::Checked : Qt::Unchecked);
            break;
        }
        case KKSAttrType::atCheckList:
        case KKSAttrType::atCheckListEx:
        //case KKSAttrType::atComplex:
        {
            QAbstractItemModel * avMod = tvVal->model();
            emit updateMod (pAttrValue, val, avMod);
            stLay->setCurrentIndex (4);
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
            //valWidget = new QLineEdit (this);
            stLay->setCurrentWidget (lEVal);
            lEVal->setText(val.toString());
            break;
        }
        case KKSAttrType::atMaclabel:
        {
/*            valWidget = new QWidget (this);
            QHBoxLayout * hLay = new QHBoxLayout (valWidget);
            for (int i=0; i<2; i++)
            {
                QLineEdit * lE = new QLineEdit (valWidget);
                hLay->addWidget (lE);
            }*/
            break;
        }
        case KKSAttrType::atDate:
        {
            //valWidget = new QDateEdit (this);
            //QDateEdit * dE = qobject_cast<QDateEdit *>(valWidget);
            dEVal->setDate(val.toDate());
            dEVal->setReadOnly(true);
            break;
        }
        case KKSAttrType::atTime:
        {
            //valWidget = new QTimeEdit (this);
            //QTimeEdit * tE = qobject_cast<QTimeEdit *>(valWidget);
            tEVal->setTime(val.toTime());//QTime::fromString(pAttrValue->value().valueForInsert(), QString("hh.mm.ss")));
            tEVal->setReadOnly(true);
            break;
        }
        case KKSAttrType::atDateTime:
        {
            dtEVal->setDateTime(val.toDateTime());//QDateTime::fromString(pAttrValue->value().valueVariant(), QString("dd.MM.yyyy hh.mm.ss")));
            dtEVal->setReadOnly(true);
            break;
        }
        default:break;
    }
}

void KKSAValWidget::upVal (void)
{
    emit prevVal();
}

void KKSAValWidget::downVal (void)
{
    emit nextVal();
}


