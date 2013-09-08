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
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIcon>
#include <QTextEdit>
#include <QSizePolicy>

#include <KKSAttrValue.h>
#include <KKSAttribute.h>
#include <KKSAttrType.h>
#include "KKSAValWidget.h"

KKSAValWidget::KKSAValWidget(KKSAttrValue * _av, QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
      pAttrValue (_av),
      valWidget (0),
      lEVal (0),
      dEVal (0),
      tEVal (0),
      dtEVal (0),
      tvVal (0),
      chVal (0),
      gbVal (0),
      macVal (0),
      textEVal (0),
      tbUp (new QToolButton),
      tbDown (new QToolButton)
{
    if (pAttrValue)
        pAttrValue->addRef();
    
    QGridLayout * avGLay = new QGridLayout;
    setLayout (avGLay);
    //
    // Конструируем только 1 виджет в зависимости от типа.
    //
    avGLay->setContentsMargins (0, 0, 0, 0);
//    QSizePolicy leSp (QSizePolicy::Expanding, QSizePolicy::Fixed);
//    lEVal->setSizePolicy (leSp);

    tbUp->setIcon (QIcon(":/ddoc/uparrow.png"));
    tbUp->setToolTip (tr("Previous value"));
    tbDown->setIcon (QIcon(":/ddoc/downarrow.png"));
    tbDown->setToolTip (tr("Next value"));

    const KKSAttribute * attr = pAttrValue->attribute();
    const KKSAttrType * aType = attr->type();
    KKSAttrType::KKSAttrTypes idAttrType = aType->attrType();
    QVariant V (pAttrValue->value().valueVariant());
    switch (idAttrType)
    {
        case KKSAttrType::atBool: 
        {
            chVal = new QCheckBox;
            chVal->setText (pAttrValue->attribute()->name());
            Qt::CheckState chState;
            if (V.toBool())
                chState = Qt::Checked;
            else
                chState = Qt::Unchecked;
            chVal->setCheckState (chState);
            valWidget = qobject_cast<QWidget *>(chVal);
            break;
        }
        case KKSAttrType::atCheckList:
        case KKSAttrType::atCheckListEx:
        {
            tvVal = new QTreeView;
            valWidget = qobject_cast<QWidget *>(tvVal);
            break;
        }
        case KKSAttrType::atComplex:
        {
            gbVal = new QGroupBox;
            valWidget = qobject_cast<QWidget *> (gbVal);
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
            lEVal = new QLineEdit;
            lEVal->setReadOnly (true);
            lEVal->setText(V.toString());
            valWidget = qobject_cast<QWidget *>(lEVal);
            break;
        }
        case KKSAttrType::atMaclabel:
        {
            macVal = new QWidget;
            QHBoxLayout * hLay = new QHBoxLayout (macVal);
            for (int i=0; i<2; i++)
            {
                lE[i] = new QLineEdit (macVal);
                lE[i]->setReadOnly(true);
                hLay->addWidget (lE[i]);
            }
            valWidget = macVal;
            break;
        }
        case KKSAttrType::atDate:
        {
            dEVal = new QDateEdit;
            dEVal->setReadOnly (true);
            dEVal->setDate(V.toDate());
            valWidget = qobject_cast<QWidget *>(dEVal);
            break;
        }
        case KKSAttrType::atTime:
        {
            tEVal = new QTimeEdit;
            tEVal->setReadOnly (true);
            tEVal->setTime(V.toTime());//QTime::fromString(pAttrValue->value().valueForInsert(), QString("hh.mm.ss")));
            valWidget = qobject_cast<QWidget *>(tEVal);
            break;
        }
        case KKSAttrType::atDateTime:
        {
            dtEVal = new QDateTimeEdit;
            dtEVal->setReadOnly (true);
            dtEVal->setDateTime(V.toDateTime());//QDateTime::fromString(pAttrValue->value().valueVariant(), QString("dd.MM.yyyy hh.mm.ss")));
            valWidget = qobject_cast<QWidget *> (dtEVal);
            break;
        }
        case KKSAttrType::atText:
        {
            textEVal = new QTextEdit;
            textEVal->setReadOnly (true);
            textEVal->setPlainText (V.toString());
            valWidget = qobject_cast<QWidget *>(textEVal);
            break;
        }
        default:break;
    }

//    QSizePolicy valSp (QSizePolicy::Expanding, QSizePolicy::Preferred);
//    valWidget->setSizePolicy (valSp);
    avGLay->addWidget (valWidget, 0, 0, 2, 1);//, Qt::AlignJustify | Qt::AlignVCenter);
    QVBoxLayout * vNavLay = new QVBoxLayout;
    vNavLay->addWidget (tbUp);
    vNavLay->addWidget (tbDown);
    avGLay->addLayout (vNavLay, 0, 1, 2, 1, Qt::AlignVCenter);
    QHBoxLayout * buttonsLay = new QHBoxLayout;
    buttonsLay->addStretch(1);
    QPushButton * pbClose = new QPushButton (tr("&Close"), this);
    connect (pbClose, SIGNAL (clicked()), this, SLOT (close()));
    buttonsLay->addWidget(pbClose);
    avGLay->addLayout (buttonsLay, , 0, 1, 2, Qt::AlignJustify | Qt::AlignBottom);//addWidget(pbClose, 1, 1, 1, 1);

}

KKSAValWidget::~KKSAValWidget()
{
    if (pAttrValue)
        pAttrValue->release();
    if (valWidget)
        delete valWidget;

    delete tbUp;
    delete tbDown;
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
//            stLay->setCurrentIndex (5);
            //QCheckBox * ch = (qobject_cast<QCheckBox *>(stLay->widget(5)));
            chVal->setCheckState (isCh ? Qt::Checked : Qt::Unchecked);
            break;
        }
        case KKSAttrType::atCheckList:
        case KKSAttrType::atCheckListEx:
        {
            QAbstractItemModel * avMod = tvVal->model();
            emit updateMod (pAttrValue, val, avMod);
//            stLay->setCurrentIndex (4);
            break;
        }
        case KKSAttrType::atComplex:
        {
            emit updateComplexAttr (pAttrValue, val, gbVal);
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
            //stLay->setCurrentWidget (lEVal);
            lEVal->setText(val.toString());
            break;
        }
        case KKSAttrType::atMaclabel:
        {
            QList<QVariant> macList = val.toList();
            if (macList.count() < 2)
                break;
            for (int i=0; i<2; i++)
            {
                lE[i]->setText (macList[i].toString());
            }
            break;
        }
        case KKSAttrType::atDate:
        {
            //valWidget = new QDateEdit (this);
            //QDateEdit * dE = qobject_cast<QDateEdit *>(valWidget);
            dEVal->setDate(val.toDate());
            break;
        }
        case KKSAttrType::atTime:
        {
            //valWidget = new QTimeEdit (this);
            //QTimeEdit * tE = qobject_cast<QTimeEdit *>(valWidget);
            tEVal->setTime(val.toTime());//QTime::fromString(pAttrValue->value().valueForInsert(), QString("hh.mm.ss")));
            break;
        }
        case KKSAttrType::atDateTime:
        {
            dtEVal->setDateTime(val.toDateTime());//QDateTime::fromString(pAttrValue->value().valueVariant(), QString("dd.MM.yyyy hh.mm.ss")));
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

void KKSAValWidget::setValModel (QAbstractItemModel * valMod)
{
    if (!tvVal)
        return;
    QAbstractItemModel * oldModel = tvVal->model ();
    tvVal->setModel (valMod);
    if (oldModel && oldModel != valMod)
        delete oldModel;
}
