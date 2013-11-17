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
#include <QAbstractItemView>
#include <QFrame>

#include <QLineEdit>
#include <QDateEdit>
#include <QDateTimeEdit>
#include <QTreeView>
#include <QListView>
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
#include <QLabel>
#include <QColor>

#include <KKSAttrValue.h>
#include <KKSAttribute.h>
#include <KKSAttrType.h>
#include "KKSAValWidget.h"
#include "KKSSvgWidget.h"
#include "KKSPointTable.h"
#include "KKSPixmap.h"
#include "KKSVideoPlayer.h"
#include "KKSComplexAttrWidget.h"
#include "KKSIntervalWidget.h"
#include "KKSHIntervalW.h"
#include "KKSColorWidget.h"

KKSAValWidget::KKSAValWidget(KKSAttrValue * _av, QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
      pAttrValue (_av),
      valWidget (0),
      valFrame (new QFrame),
      lEVal (0),
      dEVal (0),
      tEVal (0),
      dtEVal (0),
      tvVal (0),
      chVal (0),
      gbVal (0),
      macVal (0),
      textEVal (0),
      svgVal (0),
      pointsVal (0),
      pixVal (0),
      videoVal (0),
      iValW (0),
      iValWH (0),
      iColW (0),
      tbUp (new QToolButton),
      tbDown (new QToolButton)
{
    if (pAttrValue)
    {
        pAttrValue->addRef();
        this->setWindowTitle(_av->attribute()->title());
    }

    QGridLayout * mainGLay = new QGridLayout (this);
    QGridLayout * avGLay = new QGridLayout;
    valFrame->setLayout (avGLay);
    //
    // Конструируем только 1 виджет в зависимости от типа.
    //
    //avGLay->setContentsMargins (0, 0, 0, 0);
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
    int numR (1);
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
            numR++;
            break;
        }
        case KKSAttrType::atComplex:
        {
            gbVal = new KKSComplexAttrWidget (pAttrValue, attr->isSystem());
//            (qobject_cast<KKSComplexAttrWidget*>(gbVal))->init();
//            (qobject_cast<KKSComplexAttrWidget*>(gbVal))->setVal(V.toString());
            valWidget = qobject_cast<QWidget *> (gbVal);
            QGridLayout * gLay = (qobject_cast<KKSComplexAttrWidget*>(gbVal))->getLay();
            initComplexWidget (pAttrValue, gLay, gbVal);
            numR++;
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
        case KKSAttrType::atRecordColorRef:
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
        case KKSAttrType::atGeometry:
        case KKSAttrType::atGeometryPoly:
        {
            textEVal = new QTextEdit;
            textEVal->setReadOnly (true);
            textEVal->setPlainText (V.toString());
            valWidget = qobject_cast<QWidget *>(textEVal);
            numR++;
            break;
        }
        case KKSAttrType::atSVG:
        {
            svgVal = new KKSSvgWidget (pAttrValue, iacIOUserAttr, V.toByteArray());
            valWidget = qobject_cast<QWidget *>(svgVal);
            break;
        }
        case KKSAttrType::atPoints:
        {
            pointsVal = new KKSPointTable (pAttrValue, iacIOUserAttr, V.toList());
            valWidget = qobject_cast<QWidget *>(pointsVal);
            break;
        }
        case KKSAttrType::atJPG:
        {
            pixVal = new KKSPixmap (pAttrValue, iacIOUserAttr, V.toString());
            valWidget = qobject_cast<QWidget *>(pixVal);
            break;
        }
        case KKSAttrType::atVideo:
        {
            videoVal = new KKSVideoPlayer (pAttrValue, iacIOUserAttr);
            videoVal->setMovie(V.toByteArray());
            valWidget = qobject_cast<QWidget *>(videoVal);
            break;
        }
        case KKSAttrType::atInterval:
        {
            iValW = new KKSIntervalWidget (pAttrValue, iacIOUserAttr);
            QString v = V.toStringList().join(" ");
            QStringList vl = v.split (" ");
            int vi = vl[0].toInt();
            QLineEdit *lEdit = new QLineEdit (QString::number (vi));
            lEdit->setReadOnly (true);
            iValW->setLineEdit (lEdit);
            QComboBox *cbUnit = new QComboBox ();
            QSizePolicy cPw (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed, QSizePolicy::ComboBox);
            cPw.setHorizontalStretch (1);
            cbUnit->setSizePolicy (cPw);
            cbUnit->setMinimumHeight (20);

            cbUnit->addItem (QString(), QVariant());

            QStringList units = KKSAttrType::getIntervalUnits();
            for (int i=0; i<units.count(); i++)
                cbUnit->addItem (units[i], i);
            if (vl.size()>1)
            {
                int ind = cbUnit->findText (vl[1], Qt::MatchContains);
                if (ind>0)
                    cbUnit->setCurrentIndex (ind);
            }
            cbUnit->setDisabled(true);
            cbUnit->setSizeAdjustPolicy (QComboBox::AdjustToContentsOnFirstShow);
            iValW->setComboUnits (cbUnit);
            valWidget = qobject_cast<QWidget *>(iValW);
            break;
        }
        case KKSAttrType::atIntervalH:
        {
            iValWH = new KKSHIntervalW (pAttrValue, iacIOUserAttr);
            iValWH->setReadOnly (true);
            valWidget = qobject_cast<QWidget *> (iValWH);
            QString v = V.toStringList().join(" ");
            QStringList vl = v.split (" ");
            int h = vl.size() >= 3 ? vl[0].toInt() : -1;
            int m = vl.size() >= 3 ? vl[1].toInt() : -1;
            int s = vl.size() >= 3 ? vl[2].toInt() : -1;
            IntervalHValue ihv (h, m, s);
            iValWH->setValue(ihv);
            break;
        }
        case KKSAttrType::atRecordColor:
        case KKSAttrType::atRecordTextColor:
        {
            //qDebug () << __PRETTY_FUNCTION__ << V;
            //unsigned int vlc = V.toUInt ();
            //QRgb rgb_col (V.value<QColor>().toRgb());//toUInt ();
            QColor rgb_color = V.value<QColor>();//QColor::fromRgba (rgb_col);//V.value<QColor>();//toInt ();
            
            iColW = new KKSColorWidget (pAttrValue, iacIOUserAttr, rgb_color, pAttrValue->attribute()->type()->attrType());
            iColW->hideToolButton();
            valWidget = qobject_cast<QWidget *>(iColW);
            break;
        }
        default:break;
    }

//    QSizePolicy valSp (QSizePolicy::Expanding, QSizePolicy::Preferred);
//    valWidget->setSizePolicy (valSp);
    avGLay->addWidget (valWidget, 0, 0, 1, 1);//, Qt::AlignJustify | Qt::AlignVCenter);
    mainGLay->addWidget (valFrame, 0, 0, 1, 1);
    QVBoxLayout * vNavLay = new QVBoxLayout;
    vNavLay->addWidget (tbUp);
    vNavLay->addWidget (tbDown);
    mainGLay->addLayout (vNavLay, 0, 1, 1, 1, Qt::AlignVCenter);
    QHBoxLayout * buttonsLay = new QHBoxLayout;
    buttonsLay->addStretch(1);
    QPushButton * pbClose = new QPushButton (tr("&Close"), this);
    connect (pbClose, SIGNAL (clicked()), this, SLOT (reject()));
    buttonsLay->addWidget(pbClose);
    mainGLay->addLayout (buttonsLay, 1, 0, 1, 2, Qt::AlignJustify | Qt::AlignBottom);//addWidget(pbClose, 1, 1, 1, 1);

    connect (tbUp, SIGNAL (clicked()), this, SLOT (upVal()) );
    connect (tbDown, SIGNAL (clicked()), this, SLOT (downVal()) );
}

void KKSAValWidget::initComplexWidget (KKSAttrValue * av, QGridLayout * gLay, QWidget * parent, Qt::WindowFlags flags)
{
    if (!av || !gLay)
        return;
    KKSMap<qint64, KKSAttrValue*> aVals = av->attrsValues();
    KKSMap<qint64, KKSAttrValue*>::const_iterator p = aVals.constBegin();

    int i=0;
    aWVals.clear();
    for (;p != aVals.constEnd(); p++)
    {
        KKSAttrValue * aV = p.value();
        if (!aV || !av->attribute())
            continue;
        const KKSAttribute * attr = aV->attribute();
        const KKSAttrType * aType = attr->type();
        int idAType = aType->attrType();
        QVariant val (aV->value().valueVariant());
        QWidget * aW (0);
        switch (idAType)
        {
            case KKSAttrType::atBool:
            {
                aW = new QCheckBox;
                break;
            }
            case KKSAttrType::atCheckList:
            {
                aW = new QListView;
                break;
            }
            case KKSAttrType::atDouble:
            case KKSAttrType::atFixString:
            case KKSAttrType::atInt:
            case KKSAttrType::atInt64:
            case KKSAttrType::atString:
            case KKSAttrType::atList:
            case KKSAttrType::atObjRef:
            case KKSAttrType::atUUID:
            case KKSAttrType::atUrl:
            {
                aW = new QLineEdit (parent);
                QLineEdit * lE = qobject_cast<QLineEdit *>(aW);
                lE->setText (val.toString());
                lE->setReadOnly (true);
                break;
            }
            case KKSAttrType::atMaclabel:
            {
                aW = new QWidget (parent);
                QHBoxLayout * hLay = new QHBoxLayout (aW);
                QList<QVariant> macList = val.toList();
                if (macList.count() < 2)
                    break;
                for (int i=0; i<2; i++)
                {
                    QLineEdit * lEMac = new QLineEdit (aW);
                    lEMac->setText (macList[i].toString());
                    lEMac->setReadOnly (true);
                    hLay->addWidget (lEMac);
                }
                break;
            }
            case KKSAttrType::atDate:
            {
                aW = new QDateEdit (parent);
                QDateEdit * dE = qobject_cast<QDateEdit *>(aW);
                dE->setDate (val.toDate());
                dE->setReadOnly (true);
                break;
            }
            case KKSAttrType::atTime:
            {
                aW = new QTimeEdit (parent);
                QTimeEdit * tE = qobject_cast<QTimeEdit *>(aW);
                tE->setTime (val.toTime());
                tE->setReadOnly (true);
                break;
            }
            case KKSAttrType::atDateTime:
            {
                aW = new QDateTimeEdit (parent);
                QDateTimeEdit * dtE = qobject_cast<QDateTimeEdit *>(aW);
                dtE->setDateTime(val.toDateTime());
                dtE->setReadOnly (true);
                break;
            }
            case KKSAttrType::atText:
            case KKSAttrType::atGeometry:
            case KKSAttrType::atGeometryPoly:
            {
                aW = new QTextEdit (parent);
                QTextEdit * textE = qobject_cast<QTextEdit *>(aW);
                textE->setText (val.toString());
                textE->setReadOnly (true);
                break;
            }
            case KKSAttrType::atSVG:
            {
                aW = new KKSSvgWidget (aV, iacIOUserAttr, val.toByteArray(), parent);
                break;
            }
            case KKSAttrType::atPoints:
            {
                aW = new KKSPointTable (aV, iacIOUserAttr, val.toList());
                break;
            }
            case KKSAttrType::atJPG:
            {
                aW = new KKSPixmap (pAttrValue, iacIOUserAttr, val.toString());
                break;
            }
            case KKSAttrType::atVideo:
            {
                aW = new KKSVideoPlayer (pAttrValue, iacIOUserAttr);
                (qobject_cast<KKSVideoPlayer *>(aW))->setMovie(val.toByteArray());
                break;
            }
            case KKSAttrType::atInterval:
            {
                aW = new KKSIntervalWidget (pAttrValue, iacIOUserAttr);
                KKSIntervalWidget * iW = qobject_cast<KKSIntervalWidget *>(aW);
                QString v = val.toStringList().join(" ");
                QStringList vl = v.split (" ");
                int vi = vl[0].toInt();
                QString unit = vl[1];
                IntervalValue iv (vi, unit);
                iW->setValue (iv);
                break;
            }
            case KKSAttrType::atIntervalH:
            {
                aW = new KKSHIntervalW (pAttrValue, iacIOUserAttr);
                KKSHIntervalW * iHW = qobject_cast<KKSHIntervalW *>(aW);
                iHW->setReadOnly(true);
                QString v = val.toStringList().join(" ");
                QStringList vl = v.split (" ");
                int h = vl.size() >= 3 ? vl[0].toInt() : -1;
                int m = vl.size() >= 3 ? vl[1].toInt() : -1;
                int s = vl.size() >= 3 ? vl[2].toInt() : -1;
                IntervalHValue ihv (h, m, s);
                iHW->setValue(ihv);
                break;
            }
            default:
            {
                aW = 0;
                aWVals.append (aW);
                continue;
            }
            
        }
        QLabel * lTitle = new QLabel (attr->name(), parent, flags);
        aWVals.append (aW);
        gLay->addWidget (lTitle, i, 0, 1, 1);
        gLay->addWidget (aW, i, 1, 1, 1);
        i++;
    }
}

KKSAValWidget::~KKSAValWidget()
{
    if (pAttrValue)
        pAttrValue->release();
    
    delete valFrame;
    //if (valWidget)
    //    delete valWidget;

    delete tbUp;
    delete tbDown;
}

void KKSAValWidget::setValue (const KKSAttribute * a, QVariant val)
{
    if (!a || a->id() != pAttrValue->attribute()->id())
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
            (qobject_cast<KKSComplexAttrWidget*>(gbVal))->setVal(val.toString());
            emit updateComplexAttr (pAttrValue, val, gbVal);
            if (aWVals.isEmpty())//qobject_cast<QWidget *>(sender()))
                initComplexWidget(pAttrValue,(qobject_cast<KKSComplexAttrWidget*>(gbVal))->getLay(), gbVal);
            else
                setComplexVals (val);
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
        case KKSAttrType::atRecordColorRef:
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
        case KKSAttrType::atText:
        case KKSAttrType::atGeometry:
        case KKSAttrType::atGeometryPoly:
        {
            textEVal->setPlainText(val.toString());
            break;
        }
        case KKSAttrType::atSVG:
        {
            svgVal->setContents(val.toByteArray());
            break;
        }
        case KKSAttrType::atPoints:
        {
            pointsVal->setData(val.toList());
            break;
        }
        case KKSAttrType::atJPG:
        {
            pixVal->setPixmap(val.toString());
            break;
        }
        case KKSAttrType::atVideo:
        {
            videoVal->setMovie(val.toByteArray());
            break;
        }
        case KKSAttrType::atInterval:
        {
            QString v = val.toStringList().join(" ");
            QStringList vl = v.split (" ");
            int vi = vl[0].toInt();
            QString unit = (vl.size() >= 2 ? vl[1] : tr("seconds"));
            IntervalValue iv (vi, unit);
            iValW->setValue (iv);
            break;
        }
        case KKSAttrType::atIntervalH:
        {
            QString v = val.toStringList().join(" ");
            QStringList vl = v.split (" ");
            int h = vl.size() >= 3 ? vl[0].toInt() : -1;
            int m = vl.size() >= 3 ? vl[1].toInt() : -1;
            int s = vl.size() >= 3 ? vl[2].toInt() : -1;
            IntervalHValue ihv (h, m, s);
            iValWH->setValue(ihv);
            break;
        }
        case KKSAttrType::atRecordColor:
        case KKSAttrType::atRecordTextColor:
        {
            //unsigned int vlc = val.toUInt ();
            //QRgb rgb_col (vlc);// = V.value<QColor>();//toUInt ();
            QColor rgb_color = val.value<QColor>();//::fromRgba (rgb_col);//V.value<QColor>();//toInt ();
            
            iColW->setColor(rgb_color);// = new KKSColorWidget (pAttrValue, KKSIndAttr::iacIOUserAttr, rgb_color, pAttrValue->attribute()->type()->attrType());
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

void KKSAValWidget::setComplexVals (const QVariant& val)
{
    QMap<QString, QVariant> valArr = val.toMap();
    
    //int nw = aWVals.count();
    KKSMap<qint64, KKSAttrValue*> aVals = pAttrValue->attrsValues();
    //qDebug () << __PRETTY_FUNCTION__ << valArr << aVals.keys();
    KKSMap<qint64, KKSAttrValue*>::const_iterator p = aVals.constBegin();
    int i=0;
    for (; p != aVals.constEnd(); p++)
    {
        QWidget * aW = aWVals[i];
        //qDebug () << __PRETTY_FUNCTION__ << aW << i;
        if (!aW)
        {
            i++;
            continue;
        }
        KKSAttrValue * av = p.value();
        if (!av || !av->attribute() || !av->attribute()->type())
            continue;
        int idAttrType = av->attribute()->type()->attrType();
        QMap<QString, QVariant>::const_iterator pv = valArr.constBegin()+i;
        QVariant valW = pv.value();//valArr.value(QString::number(p.key()));
        //qDebug () << __PRETTY_FUNCTION__ << valW << idAttrType;
        switch (idAttrType)
        {
            case KKSAttrType::atBool:
            {
                QCheckBox * ch = qobject_cast<QCheckBox *>(aW);
                bool isCh = valW.toBool();
                ch->setChecked(isCh);
                break;
            }
            case KKSAttrType::atCheckList:
            case KKSAttrType::atCheckListEx:
            {
                QAbstractItemModel * aMod = (qobject_cast<QAbstractItemView *>(aW))->model();
                emit updateMod (p.value(), valW, aMod);
                break;
            }
            case KKSAttrType::atDateTime:
            {
                QDateTimeEdit * dtE = qobject_cast<QDateTimeEdit *>(aW);
                if (valW.isValid())
                    dtE->setDateTime(valW.toDateTime());
                else
                    dtE->setDateTime(QDateTime());
                break;
            }
            case KKSAttrType::atDate:
            {
                QDateEdit * dE = qobject_cast<QDateEdit *>(aW);
                if (valW.isValid())
                    dE->setDate(valW.toDate());
                else
                    dE->setDate(QDate());
                break;
            }
            case KKSAttrType::atTime:
            {
                QTimeEdit * tE = qobject_cast<QTimeEdit *>(aW);
                if (valW.isValid())
                    tE->setTime(valW.toTime());
                else
                    tE->setTime(QTime());
                break;
            }
            case KKSAttrType::atDouble:
            case KKSAttrType::atFixString:
            case KKSAttrType::atInt:
            case KKSAttrType::atInt64:
            case KKSAttrType::atString:
            case KKSAttrType::atObjRef:
            case KKSAttrType::atUUID:
            case KKSAttrType::atUrl:
            {
                QLineEdit * lE = qobject_cast<QLineEdit *>(aW);
                lE->setText(valW.toString());
                break;
            }
            case KKSAttrType::atText:
            case KKSAttrType::atGeometry:
            case KKSAttrType::atGeometryPoly:
            {
                QTextEdit * textE = qobject_cast<QTextEdit *>(aW);
                textE->setPlainText(valW.toString());
                break;
            }
            case KKSAttrType::atSVG:
            {
                KKSSvgWidget * svgW = qobject_cast<KKSSvgWidget *>(aW);
                svgW->setContents(valW.toByteArray());
                break;
            }
            case KKSAttrType::atPoints:
            {
                KKSPointTable * pointW = qobject_cast<KKSPointTable *>(aW);
                pointW->setData(valW.toList());
                break;
            }
            case KKSAttrType::atJPG:
            {
                KKSPixmap * pixW = qobject_cast<KKSPixmap *>(aW);
                pixW->setPixmap (valW.toString());
                break;
            }
            case KKSAttrType::atVideo:
            {
                KKSVideoPlayer * vpW = qobject_cast<KKSVideoPlayer *>(aW);
                vpW->setMovie(valW.toByteArray());
                break;
            }
            case KKSAttrType::atInterval:
            {
                KKSIntervalWidget * iW = qobject_cast<KKSIntervalWidget *>(aW);
                QString v = valW.toStringList().join(" ");
                QStringList vl = v.split (" ");
                int vi = vl[0].toInt();
                QString unit = (vl.size() >= 2 ? vl[1] : tr("seconds"));
                IntervalValue iv (vi, unit);
                iW->setValue(iv);
                break;
            }
            case KKSAttrType::atIntervalH:
            {
                KKSHIntervalW * iHW = qobject_cast<KKSHIntervalW *>(aW);
                QString v = val.toStringList().join(" ");
                QStringList vl = v.split (" ");
                int h = vl.size() >= 3 ? vl[0].toInt() : -1;
                int m = vl.size() >= 3 ? vl[1].toInt() : -1;
                int s = vl.size() >= 3 ? vl[2].toInt() : -1;
                IntervalHValue ihv (h, m, s);
                iHW->setValue(ihv);
                break;
            }
        }
        i++;
    }
}
