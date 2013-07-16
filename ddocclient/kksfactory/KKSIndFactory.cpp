#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QTreeView>
#include <QModelIndex>
#include <QAction>
#include <QIcon>
#include <QGridLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QCheckBox>
#include <QSizePolicy>
#include <QLineEdit>
#include <QToolButton>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QGridLayout>
#include <QLayout>
#include <QLayoutItem>
#include <QMessageBox>
#include <QtDebug>

#include <KKSIndicator.h>
#include <KKSIndicatorValue.h>
#include <KKSMap.h>
#include <KKSObjectExemplar.h>
#include <KKSObject.h>
#include <KKSCategory.h>
#include <KKSFilter.h>
#include <KKSEIOData.h>

#include <KKSRecWidget.h>
#include <KKSObjEditor.h>
#include <KKSObject.h>
#include <KKSCheckBox.h>
#include <KKSAttrCheckListWidget.h>
#include <KKSColorWidget.h>
#include <KKSTimeEdit.h>
#include <KKSDateEdit.h>
#include <KKSDateTimeEdit.h>
#include <KKSHIntervalW.h>
#include <KKSAttrRefWidget.h>
#include <KKSEdit.h>
#include <KKSText.h>
#include <KKSVideoPlayer.h>
#include <KKSListWidget.h>
#include <KKSIntervalWidget.h>
#include <KKSPixmap.h>
#include <KKSPointTable.h>
#include <KKSSvgWidget.h>
#include <IndicatorForm.h>
#include "KKSLoader.h"
#include "KKSIndFactory.h"
#include "KKSObjEditorFactory.h"
#include "defines.h"

KKSIndFactory :: KKSIndFactory (KKSDatabase * _db, KKSLoader *l, QObject *parent)
    : KKSEntityFactory (parent),
    db (_db),
    loader (l),
    oef (0)
{
}

KKSIndFactory :: ~KKSIndFactory (void)
{
}

void KKSIndFactory ::setOEF (KKSObjEditorFactory * _oef)
{
    oef = _oef;
}

KKSRecWidget * KKSIndFactory :: getIOIndices (KKSObject * io, QWidget * parent, Qt::WindowFlags flags)
{
    KKSRecWidget * recW = new KKSRecWidget (false, parent, flags);
    QTreeView * tv = recW->getView();//new QTreeView;
    QAction * actNewInd =  new QAction (QIcon(":/ddoc/add_index.png"), tr ("Add new index"), recW);
    recW->insertToolBarAction (recW->actEdit, actNewInd);
    connect (actNewInd, SIGNAL (triggered()), this, SLOT (addNewIndex()) );
    QAbstractItemModel * indModel = getIndicesModel (io);
    recW->setEIOModel (indModel);
    recW->hideGroup (2);
    recW->hideGroup (3);
    return recW;
}

QAbstractItemModel * KKSIndFactory :: getIndicesModel (KKSObject * io)
{
    Q_UNUSED (io);
    QAbstractItemModel * iModel = new QStandardItemModel (0, 1);
    iModel->setHeaderData (0, Qt::Horizontal, tr ("Indices"), Qt::DisplayRole);
    return iModel;
}

void KKSIndFactory :: addIOIndex (KKSObject * io, const QModelIndex& pIndex, QAbstractItemModel * mod)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (!io || !mod)
        return;
    Q_UNUSED (pIndex);
    QWidget * parentWidget = qobject_cast<QWidget *>(this->sender());
    KKSIndicator * ind = new KKSIndicator (-1, QObject::tr("New indicator"));
    KKSMap<int, KKSIndicator*> indices;
    KKSIndicatorValue * indV = new KKSIndicatorValue (-1, ind, KKSValue());
    IndicatorForm * indForm = new IndicatorForm (indV, parentWidget);
//    IndexForm * indForm = new IndexForm(ind, indices, parentWidget);
    if (indForm->exec() == QDialog::Accepted)
    {
    }
}

void KKSIndFactory :: editIOIndex (KKSObject * io, const QModelIndex& wIndex, QAbstractItemModel * mod)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (!io || !mod || !wIndex.isValid())
        return;
}

void KKSIndFactory :: delIOIndex (KKSObject * io, const QModelIndex& wIndex, QAbstractItemModel * mod)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (!io || !mod || !wIndex.isValid())
        return;
}

void KKSIndFactory :: refreshIndices (KKSObject * io, QAbstractItemModel * mod)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (!io || !mod )
        return;
}

void KKSIndFactory :: addNewIndex (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    
}

void KKSIndFactory :: putIndWidget (KKSIndicatorValue *indicVal, KKSObjEditor *objEditor, QGridLayout *gLayout, int n_str, bool isSystem, QString tableName, int idCat)
{
    const KKSValue pVal = indicVal->value();
    const KKSIndicator * pCategAttr = indicVal->indicator();
    QVariant V = pVal.valueVariant();
    QString ss = pVal.value();
    bool isExist (objEditor->getObj()->id()>0);
//    resWidget->setCategoryAttr (pCategAttr);
    if (!pCategAttr)
    {
        qDebug () << __PRETTY_FUNCTION__ << "Category attribute is null";
        return;
    }

    const KKSAttrType *pCatType = pCategAttr->type ();
    QToolButton *tbRef = 0;
    QLabel *lTitle = 0;
    QCheckBox *ch = 0;
    QWidget *aw = 0;
    qDebug () << __PRETTY_FUNCTION__ << gLayout->columnCount();
    if (pCatType->attrType() != KKSAttrType::atCheckListEx)
        aw = this->createIndWidget (pCategAttr, pCatType, isSystem, gLayout, n_str, V, lTitle, tbRef, ch, objEditor);
    else
    {
        QTabWidget * tabEnc = objEditor->getRecTab();
        //Q_UNUSED (tabEnc);
        aw = this->createAttrCheckWidget (pCategAttr, pCatType, tabEnc);
    }
    this->setValue (aw, pCategAttr, pCatType, V, isExist, tableName, idCat, tbRef, ch, objEditor, lTitle);

    return;
}

QLabel * KKSIndFactory :: createAttrTitle (const QString& title)
{
    QLabel *lTitle = new QLabel (QString ("%1:").arg (title));
    lTitle->setMinimumHeight (20);
    QSizePolicy spLabel (QSizePolicy::Expanding, QSizePolicy::Fixed);
    lTitle->setSizePolicy (spLabel);
    return lTitle;
}

QCheckBox * KKSIndFactory :: createChDateTime (QGridLayout *gLayout, QLabel *lTitle, int n_str)
{
    QCheckBox * ch = 0;
    ch = new QCheckBox ();
    QSizePolicy hPol (QSizePolicy::Fixed, QSizePolicy::Fixed);
    hPol.setHorizontalStretch (0);
    gLayout->addWidget (ch, n_str, 0, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    gLayout->addWidget (lTitle, n_str, 1, 1, 1, Qt::AlignRight | Qt::AlignTop);

    return ch;
}

QWidget * KKSIndFactory :: createIndWidget (const KKSIndicator * pCategAttr, 
                                            const KKSAttrType *pCatType,  
                                            bool isSystem, 
                                            QGridLayout *gLayout, 
                                            int n_str, 
                                            const QVariant& V, 
                                            QLabel *lTitle, 
                                            QToolButton *&tbRef, 
                                            QCheckBox *&ch, 
                                            KKSObjEditor * objEditor, 
                                            bool isRef)
{
    Q_UNUSED(isRef);
    Q_UNUSED(objEditor);
    Q_UNUSED(ch);
    Q_UNUSED(tbRef);
    Q_UNUSED(lTitle);
    Q_UNUSED(V);
    Q_UNUSED(n_str);
    Q_UNUSED(gLayout);
    Q_UNUSED(isSystem);
    Q_UNUSED(pCatType);
    Q_UNUSED(pCategAttr);
    QWidget * attrWidget = 0;
    /*
    if (!pCatType)
        return 0;

    QSizePolicy hPw (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    if (isRef)
        qDebug () << __PRETTY_FUNCTION__ << pCatType->attrType();

    QToolButton * tbQuestion = new QToolButton ();
    tbQuestion->setText (QString("?"));
    objEditor->addTbQList (tbQuestion, pCategAttr->id());
    QToolButton * tbEdit = new QToolButton ();
    tbEdit->setText (tr("Edit"));
    tbEdit->setIcon (QIcon(":/ddoc/edit.png"));
    objEditor->addTbEditList (tbEdit, pCategAttr->id());
    QHBoxLayout * buttonsLay = new QHBoxLayout;
    gLayout->addLayout (buttonsLay, n_str, 3, 1, 1, Qt::AlignTop);//, Qt::AlignJustify | Qt::AlignTop);
    QToolButton * tbDel = new QToolButton ();
    tbDel->setText (tr("Delete"));
    tbDel->setIcon (QIcon(":/ddoc/delete.png"));
    objEditor->addTbDelList (tbDel, pCategAttr->id());
    buttonsLay->addWidget (tbEdit);//, n_str, 3, 1, 1);
    buttonsLay->addWidget (tbDel);
    buttonsLay->addWidget (tbQuestion);//, n_str, 4, 1, 1);
    switch (pCatType->attrType())
    {
        case KKSAttrType::atUndef: default:
            break;
        case KKSAttrType::atBool:
            {
                attrWidget = new KKSCheckBox (pCategAttr, isSystem, QString("%1").arg (isRef ? QString() : pCategAttr->name ()));
                attrWidget->setMinimumHeight (20);
                attrWidget->setLayoutDirection (Qt::RightToLeft);
                attrWidget->setSizePolicy (hPw);
                gLayout->addWidget (attrWidget, n_str, 1, 1, 3, Qt::AlignLeft | Qt::AlignTop);
            }
            break;
        case KKSAttrType::atList:
        case KKSAttrType::atParent:
        case KKSAttrType::atRecordColorRef:
        case KKSAttrType::atRecordTextColorRef:
            {
                if (!isRef)
                {
                    qDebug () << __PRETTY_FUNCTION__ << "Reference" << pCategAttr->refType()->attrType();
                    lTitle = this->createAttrTitle (pCategAttr->name());
                    qDebug () << __PRETTY_FUNCTION__ << "Title is created" << lTitle->text ();
                    Qt::Alignment align=Qt::AlignRight;
                    if (pCategAttr->refType()->attrType() == KKSAttrType::atText )
                        align |= Qt::AlignTop;
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2, align);//Qt::AlignRight);
                }
                hPw.setHorizontalStretch (10);
                attrWidget = new KKSAttrRefWidget ();
                QLabel *l=0;
                QToolButton *tb = 0;
                QCheckBox *chr = 0;
                QVariant vr = QVariant ();
                bool isWRef (pCategAttr->refType()->attrType() != KKSAttrType::atList &&
                             pCategAttr->refType()->attrType() != KKSAttrType::atParent);
                qDebug () << __PRETTY_FUNCTION__ << "Widget creation" << pCategAttr->refType()->attrType() << isWRef << pCategAttr->id();
                const KKSIndicator * refAttr = 0;
                if (pCategAttr->refType()->attrType () == KKSAttrType::atList ||
                    pCategAttr->refType()->attrType () == KKSAttrType::atParent)
                {
                    QString aCode = pCategAttr->columnName ();
                    //
                    // TODO: make load indicator
                    //
                    refAttr = 0;//loader->loadAttribute (aCode, pCategAttr->tableName());
                }
                else
                {
                    refAttr = pCategAttr;
                    refAttr->addRef ();
                }
                KKSIndicator * cAttr = 0;
                QWidget *arw = createIndWidget (refAttr, cAttr ? cAttr->refType() : pCategAttr->refType(), false, qobject_cast<QGridLayout *>(attrWidget->layout ()), 0, vr, l, tb, chr, objEditor, true);//isWRef);
                qDebug () << __PRETTY_FUNCTION__ << "Widget has created" << pCategAttr->refType()->attrType();
                if (!arw)
                    break;
                if (cAttr)
                    cAttr->release ();
                qobject_cast<KKSAttrRefWidget *>(attrWidget)->setAttrWidget (arw);
                attrWidget->setMinimumHeight (20);
                if (!isRef)
                {
                    tbRef = new QToolButton ();
                    tbRef->setMinimumHeight (20);
                    tbRef->setText ("...");
                    QGridLayout *gLay = new QGridLayout ();
                    int ng = 1;
                    if ( pCategAttr->refType()->attrType() == KKSAttrType::atJPG )
                        gLay->addWidget (attrWidget, 0, 0, ng, 1, Qt::AlignCenter);
                    else
                        gLay->addWidget (attrWidget, 0, 0, ng, 1);
                    gLay->addWidget (tbRef, 0, 1, ng, 1, Qt::AlignTop);
                    if (pCategAttr->refType()->attrType() == KKSAttrType::atText )
                        gLayout->addLayout (gLay, n_str, 2, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
                    else
                        gLayout->addLayout (gLay, n_str, 2, 1, 1);
                }
                attrWidget->setSizePolicy (hPw);
            }
            break;
        case KKSAttrType::atCheckList:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (pCategAttr->name());
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2, Qt::AlignRight | Qt::AlignTop);
                }
                attrWidget = new KKSListWidget (pCategAttr, isSystem);
                attrWidget->setMinimumHeight(40);
                attrWidget->setSizePolicy(hPw);
                if (isRef)
                    break;
                QHBoxLayout *hLay = new QHBoxLayout ();
                hLay->addWidget (attrWidget);
                gLayout->addLayout (hLay, n_str, 2, 1, 1, Qt::AlignTop);
            }
            break;
        case KKSAttrType::atCheckListEx:
            {
                break;
            }
            break;
        case KKSAttrType::atObjRef:
            break;
        case KKSAttrType::atDate:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (pCategAttr->name());
                    ch = this->createChDateTime (gLayout, lTitle, n_str);
                }
                QSizePolicy hPw (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                hPw.setHorizontalStretch (10);

                QDate v;
                if (V.isNull() || V.toString() == "current_timestamp")
                {
                    v = QDate::currentDate();
                    QVariant var(v);
                }
                else
                    v = V.toDate ();

                attrWidget = new KKSDateEdit (pCategAttr, isSystem, v);
                qobject_cast<QDateEdit *>(attrWidget)->setReadOnly (isRef);
                attrWidget->setMinimumHeight (20);
                attrWidget->setSizePolicy (hPw);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1, Qt::AlignTop);
                if (ch)
                {
                    if (V.isNull() || !V.isValid())
                        ch->setCheckState (Qt::Unchecked);
                    else
                        ch->setCheckState (Qt::Checked);
                }
            }
            break;
        case KKSAttrType::atDateTime:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (pCategAttr->name());
                    ch = this->createChDateTime (gLayout, lTitle, n_str);
                }
                QSizePolicy hPw (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                hPw.setHorizontalStretch (10);

                QDateTime v;
                if (V.isNull() || V.toString() == "current_timestamp")
                {
                    v = QDateTime::currentDateTime();
                    QVariant var(v);
                }
                else
                    v = V.toDateTime ();

                attrWidget = new KKSDateTimeEdit (pCategAttr, isSystem, v);
                qobject_cast<QDateTimeEdit *>(attrWidget)->setReadOnly (isRef);
                attrWidget->setMinimumHeight (20);
                attrWidget->setSizePolicy (hPw);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1, Qt::AlignTop);
                if (ch)
                {
                    if (V.isNull() || !V.isValid())
                        ch->setCheckState (Qt::Unchecked);
                    else
                        ch->setCheckState (Qt::Checked);
                }
            }
            break;
        case KKSAttrType::atTime:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (pCategAttr->name());
                    ch = this->createChDateTime (gLayout, lTitle, n_str);
                }
                QSizePolicy hPw (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                hPw.setHorizontalStretch (10);

                QTime v;
                if (V.isNull() || V.toString() == "current_timestamp")
                {
                    v = QTime::currentTime();
                    QVariant var(v);
                }
                else
                    v = V.toTime ();

                attrWidget = new KKSTimeEdit (pCategAttr, isSystem, v);
                qobject_cast<QTimeEdit *>(attrWidget)->setReadOnly (isRef);
                attrWidget->setMinimumHeight (20);
                attrWidget->setSizePolicy (hPw);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1, Qt::AlignTop);
                if (ch)
                {
                    if (V.isNull() || !V.isValid())
                        ch->setCheckState (Qt::Unchecked);
                    else
                        ch->setCheckState (Qt::Checked);
                }
            }
            break;
        case KKSAttrType::atDouble:
        case KKSAttrType::atInt:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (pCategAttr->name());
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2, Qt::AlignRight | Qt::AlignTop);
                }
                double v = V.toDouble ();
                int vi = V.toInt ();
                attrWidget = new KKSEdit (pCategAttr, isSystem, (pCatType->attrType() == KKSAttrType::atDouble ? QString::number (v, 'g', 20) : QString::number (vi)));
                qobject_cast<QLineEdit *>(attrWidget)->setReadOnly (isRef);
                QValidator *dval = 0;
                if (pCatType->attrType() == KKSAttrType::atDouble)
                {
                    dval = new QDoubleValidator (0);
                    (qobject_cast<QDoubleValidator *>(dval))->setDecimals (16);
                }
                else
                    dval = new QIntValidator (0);
                qobject_cast <QLineEdit *>(attrWidget)->setValidator (dval);
                attrWidget->setMinimumHeight (20);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1, Qt::AlignTop);
                attrWidget->setSizePolicy (hPw);
            }
            break;
        case KKSAttrType::atString:
        case KKSAttrType::atFixString:
        case KKSAttrType::atUrl:
        case KKSAttrType::atMaclabel:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (pCategAttr->name());
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2, Qt::AlignRight | Qt::AlignTop);
                }
                QString v = V.toString ();
                if (v.isNull())
                    v = QString("");
                attrWidget = new KKSEdit (pCategAttr, isSystem, v);
                QLineEdit * lE = qobject_cast<QLineEdit *>(attrWidget);
                if (QString::compare (pCategAttr->code(false), QString("email_prefix"), Qt::CaseInsensitive) == 0)
                {
                    QRegExp rVal (QString("^([A-Za-z0-9_])+"));
                    QRegExpValidator * regVal = new QRegExpValidator (rVal, lE);
                    lE->setValidator (regVal);
                }
                lE->setReadOnly (isRef);
//                if (pCatType->attrType() == KKSAttrType::atFixString)
//                    qobject_cast<KKSEdit*>(attrWidget)->setFixedSymCount(pCategAttr->defWidth());
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1, Qt::AlignTop);
                attrWidget->setSizePolicy (hPw);
                attrWidget->setMinimumHeight (20);
            }
            break;
        case KKSAttrType::atInterval:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (pCategAttr->name());
                    ch = this->createChDateTime (gLayout, lTitle, n_str);
                }
                QSizePolicy hPw (QSizePolicy::Expanding, QSizePolicy::Fixed);
                hPw.setHorizontalStretch (10);

                attrWidget = new KKSIntervalWidget (pCategAttr, isSystem);
                attrWidget->setSizePolicy (hPw);
                attrWidget->setMinimumHeight (20);
                QString v = V.toStringList().join(" ");
                QStringList vl = v.split (" ");
                int vi = vl[0].toInt();
                QIntValidator *dVal = new QIntValidator( 1, 24, 0);
                QLineEdit *lEdit = new QLineEdit (QString::number (vi));
                lEdit->setMinimumHeight (20);
                lEdit->setValidator (dVal);
                QSizePolicy lPw (QSizePolicy::Expanding, QSizePolicy::Fixed, QSizePolicy::LineEdit);
                lPw.setHorizontalStretch (3);
                lEdit->setSizePolicy (lPw);
                qobject_cast<KKSIntervalWidget*>(attrWidget)->setLineEdit (lEdit);

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
                cbUnit->setSizeAdjustPolicy (QComboBox::AdjustToContentsOnFirstShow);
                qobject_cast<KKSIntervalWidget*>(attrWidget)->setComboUnits (cbUnit);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1, Qt::AlignTop);
                if (ch)
                {
                    if (V.isNull() || !V.isValid())
                        ch->setCheckState (Qt::Unchecked);
                    else
                        ch->setCheckState (Qt::Checked);
                }
            }
            break;
        case KKSAttrType::atIntervalH:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (pCategAttr->name());
                    ch = this->createChDateTime (gLayout, lTitle, n_str);
                }
                QSizePolicy hPw (QSizePolicy::Expanding, QSizePolicy::Fixed);
                hPw.setHorizontalStretch (10);

                attrWidget = new KKSHIntervalW (pCategAttr, isSystem);
                attrWidget->setSizePolicy (hPw);
                attrWidget->setMinimumHeight (20);
                QString v = V.toStringList().join(" ");
                QStringList vl = v.split (" ");
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1, Qt::AlignTop);
                if (ch)
                {
                    if (V.isNull() || !V.isValid())
                        ch->setCheckState (Qt::Unchecked);
                    else
                        ch->setCheckState (Qt::Checked);
                }
            }
            break;
        case KKSAttrType::atText:
        case KKSAttrType::atXMLDoc:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (pCategAttr->name());
                    gLayout->addWidget (lTitle, n_str, 0, 2, 2, Qt::AlignRight | Qt::AlignTop);
                }
                QString v = V.toString ();
                if (v.isNull())
                    v = QString("");
                attrWidget = new KKSText (pCategAttr, isSystem, v);
                qobject_cast<KKSText *>(attrWidget)->setFixedSymCount (100);
                
                // TODO; default width
                //pCategAttr->defWidth());
                //
                qobject_cast<QTextEdit *>(attrWidget)->setReadOnly (isRef);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);
                attrWidget->setSizePolicy (hPw);
                attrWidget->setMinimumHeight (20);
            }
            break;
        case KKSAttrType::atJPG:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (pCategAttr->name());
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2, Qt::AlignRight | Qt::AlignTop);
                }

                QString v = V.toString ();
                if (v.isNull())
                    v = QString("");

                attrWidget = new KKSPixmap (pCategAttr, isSystem, v);
                QToolButton *tbRef = new QToolButton ();
                tbRef->setMinimumHeight (20);
                tbRef->setText ("...");
                //--px->setSizePolicy (hPw);
                attrWidget->setMinimumHeight (20);
                //px->setSizeAdjustPolicy (QComboBox::AdjustToMinimumContentsLength);
                //--QHBoxLayout *hLay = new QHBoxLayout ();
                //--hLay->addWidget (px);
                //--hLay->addWidget (tbRef);

                //--gLayout->addLayout (hLay, n_str, 2, 1, 1);
                if (!isRef)
                {
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1, Qt::AlignCenter);
                    gLayout->addWidget (tbRef, n_str, 3, 1, 1);

                    QObject::connect (tbRef, SIGNAL(pressed()), attrWidget, SLOT(openFile()));
                }
            }
            break;
        case KKSAttrType::atSVG:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (pCategAttr->name());
                    gLayout->addWidget (lTitle, n_str, 0, 1, 1, Qt::AlignRight | Qt::AlignTop);
                }

                QByteArray v = V.toByteArray ();
                if (v.isNull())
                {
                    v = QByteArray();
                    attrWidget = new KKSSvgWidget (pCategAttr, isSystem);
                }
                else
                    attrWidget = new KKSSvgWidget (pCategAttr, isSystem, v);//, gLayout->parentWidget());
                QToolButton *tbRef = new QToolButton ();
                tbRef->setMinimumHeight (20);
                tbRef->setText ("...");
                attrWidget->setMinimumHeight (20);
                if (!isRef)
                {
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 2, Qt::AlignCenter | Qt::AlignTop);
                    gLayout->addWidget (tbRef, n_str, 4, 1, 1);

                    QObject::connect (tbRef, SIGNAL(pressed()), attrWidget, SLOT(openFile()));
                }
            }
            break;
        case KKSAttrType::atPoints:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (pCategAttr->name());
                    gLayout->addWidget (lTitle, n_str, 0, 2, 2, Qt::AlignRight | Qt::AlignTop);
                }
                QList<QVariant> v = V.toList();
                attrWidget = new KKSPointTable (pCategAttr, isSystem, v);
                QSizePolicy hPwt (QSizePolicy::Expanding, QSizePolicy::Expanding);
                attrWidget->setSizePolicy (hPwt);
                attrWidget->setMinimumHeight (20);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);//, Qt::AlignJustify);//Qt::AlignCenter);
            }
            break;
        case KKSAttrType::atRecordColor:
        case KKSAttrType::atRecordTextColor:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (pCategAttr->name());
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2);//, Qt::AlignRight | Qt::AlignTop);
                }
                unsigned int vlc = V.toUInt ();
                QRgb rgb_col (vlc);// = V.value<QColor>();//toUInt ();
                QColor rgb_color = QColor::fromRgba (rgb_col);//V.value<QColor>();//toInt ();
                if (V.toString().isEmpty())
                    rgb_color =  QColor ();
                qDebug () << __PRETTY_FUNCTION__ << rgb_color << vlc << V;
                attrWidget = new KKSColorWidget (pCategAttr, isSystem, rgb_color);
                if (isRef)
                    qobject_cast<KKSColorWidget *>(attrWidget)->hideToolButton ();
                QSizePolicy hPwt (QSizePolicy::Expanding, QSizePolicy::Fixed);//Expanding);
                attrWidget->setSizePolicy (hPwt);
                attrWidget->setMinimumHeight (20);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1, Qt::AlignTop);//, Qt::AlignJustify);//Qt::AlignCenter);
            }
            break;
        case KKSAttrType::atVideo:
            {
                if (!isRef)
                {
                    lTitle = this->createAttrTitle (pCategAttr->name());
                    gLayout->addWidget (lTitle, n_str, 0, 1, 2);//, Qt::AlignRight | Qt::AlignTop);
                }
                attrWidget = new KKSVideoPlayer (pCategAttr, isSystem);
                if (!isRef)
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 1);//, Qt::AlignJustify);//Qt::AlignCenter);
                QToolButton *tbRef = new QToolButton ();
                tbRef->setMinimumHeight (20);
                tbRef->setText ("...");
                attrWidget->setMinimumHeight (20);
                if (!isRef)
                {
                    gLayout->addWidget (attrWidget, n_str, 2, 1, 2, Qt::AlignCenter | Qt::AlignTop);
                    gLayout->addWidget (tbRef, n_str, 4, 1, 1);

                    QObject::connect (tbRef, SIGNAL(pressed()), attrWidget, SLOT(openFile()));
                }
            }
            break;
    }
    objEditor->addIndRow(pCategAttr->id(), attrWidget, lTitle);
    */
    return attrWidget;
}

QWidget * KKSIndFactory :: createAttrCheckWidget (const KKSIndicator * pCategAttr, const KKSAttrType *pCatType, QTabWidget * tabW)
{
    QWidget * attrWidget = 0;
    /*if (!pCategAttr || !pCatType || !tabW)
        return 0;

    if (pCatType->attrType() != KKSAttrType::atCheckListEx)
        return 0;
    attrWidget = new KKSAttrCheckWidget (pCategAttr, false);//KKSListWidget (pCategAttr, isSystem);
    attrWidget->setMinimumHeight(40);
    QSizePolicy hPw (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    attrWidget->setSizePolicy(hPw);
    tabW->addTab (attrWidget, pCategAttr->name());
    */
    return attrWidget;
}

void KKSIndFactory :: setValue (QWidget *aw, const KKSIndicator * pCategAttr, const KKSAttrType *pCatType, const QVariant& V, bool isObjExist, QString tableName, int idCat, QToolButton *tbRef, QCheckBox *ch, QWidget *wEditor, QLabel *lTitle, bool isRef)
{
    /*
    Q_UNUSED (isObjExist);
    Q_UNUSED (tableName);
    Q_UNUSED (idCat);
    Q_UNUSED (lTitle);
    KKSObjEditor * objEditor = qobject_cast<KKSObjEditor *>(wEditor);
    switch (pCatType->attrType())
    {
        case KKSAttrType::atUndef: default:
            break;
        case KKSAttrType::atBool:
            {
                bool v = V.toBool ();
                qobject_cast<QCheckBox *>(aw)->setCheckState ( (v ? Qt::Checked : Qt::Unchecked) );
                qobject_cast<KKSCheckBox *>(aw)->setState ( (v ? Qt::Checked : Qt::Unchecked) );
                connectToSlots (aw, wEditor);
            }
            break;
        case KKSAttrType::atList:
        case KKSAttrType::atParent:
        case KKSAttrType::atRecordColorRef:
        case KKSAttrType::atRecordTextColorRef:
            {
                //
                // TODO: make reference indicator
                //
#if 0
                KKSAttrRefWidget *aRefW = qobject_cast<KKSAttrRefWidget *>(aw);
                if (!aRefW)
                    break;
                if (objEditor && !isRef)
                    objEditor->addListAttrWidget (tbRef, aRefW, pCategAttr);

                int idUser = loader->getUserId ();
                int idMaclabel = loader->getCurrMacLabelId();
                int idOwnerOrg = loader->getOrgId();
                QVariant cV (V);

                if ((tableName == "io_objects" && pCategAttr->tableName() == "users" ) && V.toInt() <= 0)
                    cV = QString::number (idUser);
                else if ((tableName == "io_objects" && pCategAttr->tableName() == "io_categories") && V.toInt() <= 0)
                    cV = QString::number (idCat);
                else if ((tableName == "io_objects" && pCategAttr->tableName() == "organization") && V.toInt() <= 0)
                    cV = QString::number (idOwnerOrg);
                else if ((tableName == "maclabels" || pCategAttr->tableName() == "maclabels") && V.toInt() <= 0)
                    cV = QString::number (idMaclabel);

                QMap<int, QString> values;
                QMap<int, QString> refColumnValues;

                if (pCatType->attrType() != KKSAttrType::atParent )
                {
                    QString tName = pCategAttr->tableName ();
                    KKSObject * refObj = loader->loadIO (tName, true);
                    if (!refObj)
                        break;

                    KKSCategory * cRef = refObj->category();
                    if (!cRef)
                    {
                        refObj->release ();
                        break;
                    }
                    bool isXml = false;
                    cRef = cRef->tableCategory();
                    if (cRef)
                        isXml = isXml || cRef->isAttrTypeContains(KKSAttrType::atXMLDoc) || cRef->isAttrTypeContains (KKSAttrType::atSVG);
                    refObj->release ();
#if 0
                    values = loader->loadAttributeValues (pCategAttr,
                                                          refColumnValues,
                                                          isXml,
                                                          !isXml,
                                                          QString(),
                                                          KKSList<const KKSFilterGroup*>());
#endif
                }
                else{
#if 0
                    values = loader->loadAttributeValues (pCategAttr,
                                                          refColumnValues,
                                                          true,
                                                          true,
                                                          (tableName.isEmpty() ? pCategAttr->tableName() : tableName),
                                                          KKSList<const KKSFilterGroup*>());
#endif
                }

                QString v_str;


                int iVal = 0;
                if(pCategAttr->refColumnName().isEmpty() || pCategAttr->refColumnName() == "id"){
                    bool ok = false;
                    iVal = cV.toString().toInt(&ok);
                    if(!ok)
                    {
                        int id = refColumnValues.key(cV.toString());
                        if (id > 0)
                            iVal = id;
                    }
                }
                else{
                    int id = refColumnValues.key(cV.toString());
                    if (id > 0)
                        iVal = id;
                }

                QMap<int, QString>::const_iterator pv = values.constFind (iVal);
                if (pv != values.constEnd())
                    v_str = pv.value();

                if (!isRef)
                {
                    connectToSlots (aRefW, wEditor);
                    aRefW->setValue (pCategAttr->id(), isSystem, cV);
                }

                QVariant vRef;
                if (pCatType->attrType() == KKSAttrType::atParent || pCatType->attrType() == KKSAttrType::atList)
                    vRef = QVariant (v_str);
                else if (!v_str.isEmpty())
                    vRef = QVariant (QColor::fromRgba (v_str.toUInt()));
                else
                    vRef = QVariant (QColor());

                setValue (aRefW->getAttrWidget(),
                          pCategAttr,
                          pCategAttr->refType(),
                          isSystem,
                          vRef,
                          isObjExist,
                          tableName,
                          idCat,
                          tbRef,
                          ch,
                          wEditor,
                          lTitle,
                          true);

                if (!isRef)
                {
                    if (objEditor && tbRef )
                        QObject::connect (tbRef, SIGNAL (clicked()), objEditor, SLOT (setList()) );
                    if (tbRef)
                        tbRef->setEnabled (!isObjExist || !pCategAttr->isReadOnly());
                }
#endif
            }
            break;
        case KKSAttrType::atCheckList:
            {
                QMap<int, QString> refValues;
                QMap<int, QString> values;// = loader->loadAttributeValues (pCategAttr, refValues);

                QStringList vArray = V.toStringList();
                for (QMap<int, QString>::const_iterator pv = values.constBegin(); pv!=values.constEnd(); pv++)
                {
                    QString v = pv.value();
                    int key = pv.key();
                    QVariant var = QVariant (key);
                    QListWidgetItem * item = new QListWidgetItem (v, qobject_cast<QListWidget *>(aw));

                    if (vArray.contains(QString::number(key)))
                        item->setCheckState(Qt::Checked);
                    else
                        item->setCheckState(Qt::Unchecked);

                    item->setData (Qt::UserRole, var);
                }
                aw->setEnabled (true);
                if (!isRef)
                    connectToSlots (aw, wEditor);
            }
            break;
        case KKSAttrType::atCheckListEx:
            {
                QString tableName = pCategAttr->tableName ();
                KKSAttrCheckWidget * arw = qobject_cast<KKSAttrCheckWidget *>(aw);
                if (!arw)
                    break;

                QStringList vArray = V.toStringList();
                KKSObject * refIO = loader->loadIO (tableName, true);
                if (!refIO)
                    break;
                KKSCategory * c = refIO->category ();
                if (!c)
                {
                    refIO->release ();
                    break;
                }
                KKSCategory * ct = c->tableCategory ();
                if (!ct)
                {
                    refIO->release ();
                    break;
                }
                KKSList<const KKSFilterGroup*> filters;
                QString vals;
                for (int i=0; i<vArray.count(); i++)
                {
                    vals += vArray[i];
                    if (i < vArray.count()-1)
                        vals += ",";
                }
                if (!vals.isEmpty())
                {
                    QString value = QString ("select id from %1 where id in (%2) ").arg (tableName).arg (vals);
                    //const KKSFilter * filter = ct->createFilter ("id", value, KKSFilter::foInSQL);
                    const KKSFilter * filter = ct->createFilter (1, value, KKSFilter::foInSQL);
                    KKSList <const KKSFilter *> fl;
                    fl.append (filter);
                    filter->release ();
                    KKSFilterGroup * fg = new KKSFilterGroup(true);
                    fg->setFilters(fl);
                    filters.append(fg);
                    fg->release();
                }
                //QMap<int, QString> values = loader->loadAttributeValues (pCategAttr, true, tableName, filters);
                //qDebug () << __PRETTY_FUNCTION__ << values;
                KKSMap<int, KKSCategoryAttr *> attrs = ct->attributes ();
                QStandardItemModel * sAttrModel = new QStandardItemModel (0, attrs.count());
                int ic = 0;
                for (KKSMap<int, KKSCategoryAttr *>::const_iterator pa = attrs.constBegin(); \
                                                                    pa != attrs.constEnd(); \
                                                                    pa++)
                {
                    sAttrModel->setHeaderData (ic, Qt::Horizontal, pa.value()->title(), Qt::DisplayRole);
                    ic++;
                }
                int ii=0;
                if (!vals.isEmpty())
                {
                    KKSMap<int, KKSEIOData *> eioList = loader->loadEIOList (refIO, filters);
                    for (KKSMap<int, KKSEIOData *>::const_iterator pv = eioList.constBegin(); pv!=eioList.constEnd(); pv++)
                    {
                        sAttrModel->insertRows (ii, 1);
                        QModelIndex saInd = sAttrModel->index (ii, 0);
                        //QString v = pv.value();
                        int key = pv.key();
                        ic = 0;
                        KKSObjectExemplar * wObjE = loader->loadEIO (pv.key(), refIO);

                        for (KKSMap<int, KKSCategoryAttr *>::const_iterator pa = attrs.constBegin(); \
                                                                            pa != attrs.constEnd(); \
                                                                            pa++)
                        {
                            QModelIndex saInd = sAttrModel->index (ii, ic);
                            KKSAttrValue * av = wObjE->attrValue (pa.value()->id());
                            QVariant val = av ? av->value().valueVariant () : QVariant();
                            if (av->attribute()->type()->attrType() == KKSAttrType::atJPG)
                                val = QObject::tr("<Image data %1>").arg (ii);
                            sAttrModel->setData (saInd, key, Qt::UserRole);
                            sAttrModel->setData (saInd, val, Qt::DisplayRole);
                            ic++;
                        }
                        wObjE->release ();
                        ii++;
                    }
                }
                //c->release ();
                refIO->release ();
                QObject :: connect (arw, \
                         SIGNAL (addAttrRef (const KKSIndAttr*,  bool, QAbstractItemModel*)), \
                         wEditor, \
                         SLOT (addAttributeCheckReference (const KKSIndAttr*, bool, QAbstractItemModel *)) \
                        );
                QObject :: connect (arw, \
                         SIGNAL (delAttrRef (const KKSIndAttr*, bool, QAbstractItemModel*, const QModelIndex&)), \
                         wEditor, \
                         SLOT (delAttributeCheckReference (const KKSIndAttr*, bool, QAbstractItemModel*, const QModelIndex&)) \
                        );
                QObject :: connect (arw, \
                         SIGNAL (refIOOpen (QString)), \
                         wEditor, \
                         SLOT (openReferenceIO (QString))
                        );
                arw->setModel (sAttrModel);
                aw->setEnabled (true);//!isObjExist || !pCategAttr->isReadOnly());
                if (!isRef)
                    connectToSlots (aw, wEditor);
            }
            break;
        case KKSAttrType::atObjRef:
            break;
        case KKSAttrType::atDate:
            {
                QDate v;
                if (V.isNull() || V.toString() == "current_timestamp")
                {
                    v = QDate::currentDate();
                    QVariant var(v);
                    if (objEditor && !isRef)
                        objEditor->setIndValue (pCategAttr->id(), false, var);
                }
                else if (V.isNull())
                {
                    v = QDate();
                    QVariant var(v);
                    if (objEditor && !isRef)
                        objEditor->setIndValue (pCategAttr->id(), false, var);
                }
                else
                {
                    v = V.toDate ();
                    if (objEditor && !isRef)
                        objEditor->setIndValue (pCategAttr->id(), false, V);
                }

                if (ch && objEditor)
                {
//                    objEditor->setOpt (pCategAttr->id(), isSystem, ch);
//                    objEditor->addOptWidget (pCategAttr->id(), isSystem, aw);
                }
                bool isEnable = (!ch || ch->checkState () == Qt::Checked);
                if (!isRef)
                {
                    connectToSlots (aw, wEditor);
                    qobject_cast <QDateEdit*>(aw)->setDate (v);
                }
                else
                {
                    KKSAttrRefWidget * rdw = qobject_cast<KKSAttrRefWidget *>(aw);
                    QDateEdit *dEdit = rdw ? qobject_cast<QDateEdit *>(rdw->getAttrWidget ()) : qobject_cast<QDateEdit *> (aw);
                    if (dEdit)
                        dEdit->setDate (v);
                }
                aw->setEnabled (isEnable);
            }
            break;
        case KKSAttrType::atDateTime:
            {
                QDateTime v;
                if (V.isNull())
                {
                    v = QDateTime();
                    QVariant var(v);
                    if (objEditor && !isRef)
                        objEditor->setIndValue (pCategAttr->id(), false, var);
                }
                else
                {
                    v = V.toDateTime ();
                    if (objEditor && !isRef)
                        objEditor->setIndValue (pCategAttr->id(), false, V);
                }

                if (ch && objEditor)
                {
//                    objEditor->setOpt (pCategAttr->id(), isSystem, ch);
//                    objEditor->addOptWidget (pCategAttr->id(), isSystem, aw);
                }
                bool isEnable = (!ch || ch->checkState () == Qt::Checked);
                if (!isRef)
                {
                    connectToSlots (aw, wEditor);
                    qobject_cast<QDateTimeEdit *>(aw)->setDateTime (v);
                }
                else
                {
                    KKSAttrRefWidget * rdw = qobject_cast<KKSAttrRefWidget *>(aw);
                    QDateTimeEdit *dtEdit = rdw ? qobject_cast<QDateTimeEdit *>(rdw->getAttrWidget ()) : qobject_cast<QDateTimeEdit *> (aw);
                    if (dtEdit)
                        dtEdit->setDateTime (v);
                }
                aw->setEnabled (isEnable);
            }
            break;
        case KKSAttrType::atTime:
            {
                QTime v;
                if (V.isNull() || V.toString() == "current_timestamp")
                {
                    v = QTime::currentTime();
                    QVariant var(v);
                    if (objEditor && !isRef)
                        objEditor->setIndValue (pCategAttr->id(), false, var);
                }
                else if (V.isNull())
                {
                    v = QTime();
                    QVariant var(v);
                    if (objEditor && !isRef)
                        objEditor->setIndValue (pCategAttr->id(), false, var);
                }
                else
                {
                    v = V.toTime ();
                    if (objEditor && !isRef)
                        objEditor->setIndValue (pCategAttr->id(), false, V);
                }

                if (ch && objEditor)
                {
//                    objEditor->setOpt (pCategAttr->id(), isSystem, ch);
//                    objEditor->addOptWidget (pCategAttr->id(), isSystem, aw);
                }
                bool isEnable = (!ch || ch->checkState () == Qt::Checked);
                if (!isRef)
                {
                    connectToSlots (aw, wEditor);
                    qobject_cast<QDateTimeEdit *>(aw)->setTime (v);
                }
                else
                {
                    KKSAttrRefWidget * rdw = qobject_cast<KKSAttrRefWidget *>(aw);
                    QTimeEdit *dtEdit = rdw ? qobject_cast<QTimeEdit *>(rdw->getAttrWidget ()) : qobject_cast<QTimeEdit *> (aw);
                    if (dtEdit)
                        dtEdit->setTime (v);
                }
                aw->setEnabled (isEnable);
            }
            break;
        case KKSAttrType::atDouble:
        case KKSAttrType::atInt:
            {
                double v = V.toDouble ();
                int vi = V.toInt ();
                KKSEdit *lEdit = qobject_cast<KKSEdit *>(aw);
                if (pCatType->attrType() == KKSAttrType::atDouble)
                    lEdit->setVal (QString::number (v));
                else
                    lEdit->setVal (QString::number (vi));

                if (!isRef)
                    connectToSlots (aw, wEditor);
                else
                {
                    if (pCatType->attrType() == KKSAttrType::atDouble)
                        lEdit->setText (QString::number (v, 'f', 16));
                    else
                        lEdit->setText (QString::number (vi));
                }
                lEdit->setVal (lEdit->text());
                lEdit->setEnabled (true);
            }
            break;
        case KKSAttrType::atString:
        case KKSAttrType::atFixString:
        case KKSAttrType::atUrl:
        case KKSAttrType::atMaclabel:
            {
                QString v = V.toString ();
                if (v.isNull())
                    v = QString("");
                qDebug () << __PRETTY_FUNCTION__ << v;
                KKSEdit *lEdit = qobject_cast<KKSEdit *>(aw);
                lEdit->setVal (v);
                if (!isRef)
                {
                    connectToSlots (aw, wEditor);
                    lEdit->setVal (lEdit->text());
                    lEdit->setEnabled (true);
                }
                else
                    lEdit->setText (v);
            }
            break;
        case KKSAttrType::atInterval:
            {
                KKSIntervalWidget *intervalW = qobject_cast<KKSIntervalWidget *>(aw);
                if (ch && objEditor)
                {
//                    objEditor->setOpt (pCategAttr->id(), isSystem, ch);
//                    objEditor->addOptWidget (pCategAttr->id(), isSystem, lTitle);
//                    objEditor->addOptWidget (pCategAttr->id(), isSystem, intervalW);
                    if (V.isNull() || !V.isValid())
                        ch->setCheckState (Qt::Unchecked);
                    else
                        ch->setCheckState (Qt::Checked);
                }
                bool isEnable = (!ch || ch->checkState () == Qt::Checked);
                if (!isRef)
                    connectToSlots (aw, wEditor);
                intervalW->setValue ();
                intervalW->setEnabled (isEnable);//!isExist || !pCategAttr->isReadOnly());
            }
            break;
        case KKSAttrType::atIntervalH:
            {
                KKSHIntervalW * intervalHW = qobject_cast<KKSHIntervalW *>(aw);
                if (ch && objEditor)
                {
//                    objEditor->setOpt (pCategAttr->id(), isSystem, ch);
//                    objEditor->addOptWidget (pCategAttr->id(), isSystem, lTitle);
//                    objEditor->addOptWidget (pCategAttr->id(), isSystem, intervalHW);
                    if (V.isNull() || !V.isValid())
                        ch->setCheckState (Qt::Unchecked);
                    else
                        ch->setCheckState (Qt::Checked);
                }
                bool isEnable = (!ch || ch->checkState () == Qt::Checked);
                intervalHW->setEnabled (isEnable);
                if (!isRef)
                    connectToSlots (aw, wEditor);

                QString v = V.toStringList().join(" ");
                QStringList vl = v.split (" ");
                if (vl.size() < 3)
                    intervalHW->setValue();
                else
                {
                    int h = vl[0].toInt();
                    int m = vl[1].toInt();
                    int s = vl[2].toInt();
                    intervalHW->setValue (h, m, s);
                }
            }
            break;
        case KKSAttrType::atText:
        case KKSAttrType::atXMLDoc:
            {
                QString v = V.toString ();
                if (v.isNull())
                    v = QString("");
                KKSText *lText = qobject_cast<KKSText*>(aw);
                if (!isRef)
                    connectToSlots (aw, wEditor);
                else
                    lText->setText (v);
                lText->setVal (v);
                lText->setEnabled (true);
            }
            break;
        case KKSAttrType::atJPG:
            {
                QString v = V.toString ();
                if (v.isNull())
                    v = QString("");

                KKSPixmap * px = qobject_cast<KKSPixmap *>(aw);
                if (!isRef)
                    connectToSlots (aw, wEditor);
                if (isRef)
                    px->setPixmap (v);
                px->setVal (v);

                px->setEnabled (true);
                if (tbRef)
                    tbRef->setEnabled (true);
            }
            break;
        case KKSAttrType::atSVG:
            {
                QByteArray v = V.toByteArray ();
                //qDebug () << __PRETTY_FUNCTION__ << v;
                if (v.isNull())
                    v = QByteArray();

                KKSSvgWidget * px = qobject_cast<KKSSvgWidget *>(aw);
                if (!isRef)
                    connectToSlots (aw, wEditor);
                if (isRef)
                    px->setContents (v);
                px->setVal (v);

                px->setEnabled (true);
                if (tbRef)
                    tbRef->setEnabled (true);
            }
            break;
        case KKSAttrType::atRecordColor:
        case KKSAttrType::atRecordTextColor:
            {
                //unsigned int vlc = V.toUInt ();
                qDebug () << __PRETTY_FUNCTION__ << V;// << vlc;
                //QRgb rgb_col (vlc);// = V.value<QColor>();//toUInt ();
                QColor rgb_color = V.value<QColor>();//QColor::fromRgba (rgb_col);//V.value<QColor>();//toInt ();
                if (V.toString().isEmpty())
                    rgb_color =  QColor ();
                KKSColorWidget * cw = qobject_cast<KKSColorWidget *>(aw);
                if (!isRef)
                    connectToSlots (aw, wEditor);
                //rgb_color = V.value<QColor>();//QColor::fromRgba (av->value().value().toUInt());
                qDebug () << __PRETTY_FUNCTION__ << rgb_color;
                cw->setColor (rgb_color);
                cw->setEnabled (true);//!isObjExist || !pCategAttr->isReadOnly());
                if (tbRef)
                    tbRef->setEnabled (true);//!isObjExist || !pCategAttr->isReadOnly());
            }
            break;
        case KKSAttrType::atVideo:
            {
                KKSVideoPlayer * vw = qobject_cast< KKSVideoPlayer *>(aw);
                if (!isRef)
                    connectToSlots (aw, wEditor);
                QByteArray v = V.toByteArray ();
                //qDebug () << __PRETTY_FUNCTION__ << v;
                if (v.isNull())
                    v = QByteArray();
                vw->setMovie (v);
            }
            break;
    }
    */
}

void KKSIndFactory :: connectToSlots (QObject *aw, QWidget* wEditor)
{
    QObject::connect (aw, 
                      SIGNAL (valueChanged(int64, KKSIndAttr::KKSIndAttrClass, QVariant)), 
                      wEditor, 
                      SLOT (setIndValue (qint64, KKSIndAttr::KKSIndAttrClass, QVariant)) );
}

void KKSIndFactory :: viewIOIndicator (KKSObject * io, int idIndicator)
{
    qDebug () << __PRETTY_FUNCTION__ << io << idIndicator;
    KKSIndicatorValue * indVal = loader->loadIOIndicatorValue (io, idIndicator);
    if (!indVal)
    {
        KKSIndicator * ind = loader->loadIndicator (idIndicator);
        for (int i=0; i<io->indicatorValues().count() && !indVal; i++)
        {
            if (idIndicator == io->indicatorValues().at(i)->indicator()->id())
                indVal = new KKSIndicatorValue (io->indicatorValues().at(i)->id(), ind, io->indicatorValues().at(i)->value());
        }
//        indVal = new KKSIndicatorValue(-1, ind, KKSValue());
//        return;
    }
    QWidget * parentW = qobject_cast<QWidget *>(this->sender());
    IndicatorForm * iForm = new IndicatorForm (indVal, false, parentW);
    iForm->setWindowModality (Qt::WindowModal);//setModal ()
    iForm->setAttribute (Qt::WA_DeleteOnClose);

    connect (iForm,
             SIGNAL (loadIndicatorType (KKSIndicatorValue *, QLineEdit *)),
             this,
             SLOT (loadIndType (KKSIndicatorValue *, QLineEdit *))
            );
    connect (iForm,
            SIGNAL (loadIOSrc (KKSIndicatorValue *, QLineEdit *)),
            this,
            SLOT (loadIndIOSRC (KKSIndicatorValue *, QLineEdit *))
            );
    connect (iForm,
             SIGNAL (viewIOSrc (KKSIndicatorValue *)),
             this,
             SLOT (viewIndIOSRC (KKSIndicatorValue *))
            );
    connect (iForm,
             SIGNAL (viewIOSrc1 (KKSIndicatorValue *)),
             this,
             SLOT (viewIndIOSRC1 (KKSIndicatorValue *))
            );
    iForm->show ();
}

void KKSIndFactory :: editIOIndicator (KKSObject * io, int idIndicator, QWidget * indW)
{
    qDebug () << __PRETTY_FUNCTION__ << io << idIndicator;
    KKSIndicatorValue * indVal = loader->loadIOIndicatorValue (io, idIndicator);
    if (!indVal)
    {
        KKSIndicator * ind = loader->loadIndicator (idIndicator);
        for (int i=0; i<io->indicatorValues().count() && !indVal; i++)
        {
            if (idIndicator == io->indicatorValues().at(i)->indicator()->id())
                indVal = new KKSIndicatorValue (io->indicatorValues().at(i)->id(), ind, io->indicatorValues().at(i)->value());
        }
//        indVal = new KKSIndicatorValue(-1, ind, KKSValue());
//        return;
    }
//    KKSIndicatorValue * indVal = loader->loadIOIndicatorValue (io, idIndicator);
//    if (!indVal)
//        return;
    QWidget * parentW = qobject_cast<QWidget *>(this->sender());
    IndicatorForm * iForm = new IndicatorForm (indVal, true, parentW);
    iForm->setWindowModality (Qt::WindowModal);//setModal ()
    iForm->setAttribute (Qt::WA_DeleteOnClose);

    connect (iForm,
             SIGNAL (loadIndicatorType (KKSIndicatorValue *, QLineEdit *)),
             this,
             SLOT (loadIndType (KKSIndicatorValue *, QLineEdit *))
            );
    connect (iForm,
            SIGNAL (loadIOSrc (KKSIndicatorValue *, QLineEdit *)),
            this,
            SLOT (loadIndIOSRC (KKSIndicatorValue *, QLineEdit *))
            );
    connect (iForm,
             SIGNAL (viewIOSrc (KKSIndicatorValue *)),
             this,
             SLOT (viewIndIOSRC (KKSIndicatorValue *))
            );
    connect (iForm,
             SIGNAL (viewIOSrc1 (KKSIndicatorValue *)),
             this,
             SLOT (viewIndIOSRC1 (KKSIndicatorValue *))
            );
    if (!iForm->isModal())
    {
        emit viewIndicatorWidget (iForm);
        return;
    }
    if (iForm->exec () == QDialog::Accepted)
    {
        KKSIndicatorValue * iVal = iForm->indValue();
        KKSList<KKSIndicatorValue *> iVals = io->indicatorValues();
        int i_ind(-1);
        for (int i=0; i<iVals.count() && i_ind < 0; i++)
            if (iVals[i] && iVals[i]->id() == idIndicator)
            {
                iVals[i] = iVal;
                i_ind = i;
            }
        io->setIndicatorValues (iVals);
        qDebug () << __PRETTY_FUNCTION__ << iVal->value().value() << io->indicatorValueId(idIndicator)->value().value();
        if (indW && qobject_cast<KKSEdit *>(indW))
        {
            qobject_cast<KKSEdit *>(indW)->setVal (iVal->value().value());
            qobject_cast<KKSEdit *>(indW)->setText (iVal->value().value());
        }
//        io->indicatorValue(idIndicator) = iVal;
    }
}

void KKSIndFactory :: loadIndType (KKSIndicatorValue * indVal, QLineEdit * lEIndType)
{
    if (!indVal || !lEIndType)
        return;
    KKSList<const KKSFilterGroup *> filters;
    QWidget * parentW = qobject_cast<QWidget *>(this->sender());
    KKSObjEditor* intTypeRec = oef->createObjRecEditor(IO_IO_ID, 86, filters, tr ("Select indicator type"), 0, true, false, Qt::WindowModal, parentW);
    if (intTypeRec->exec () == QDialog::Accepted)
    {
        int idType = intTypeRec->getID();
        KKSMap<int, KKSIndicatorType*> iTypes = loader->loadIndicatorTypes();
        KKSIndicatorType * it = iTypes.value (idType);
        indVal->indicator()->setIndType (it);
        lEIndType->setText (it->name());
        iTypes.clear ();
    }
}

void KKSIndFactory :: loadIndIOSRC (KKSIndicatorValue * indVal, QLineEdit * lEIndSrc)
{
    if (!indVal || !lEIndSrc)
        return;
    KKSList<const KKSFilterGroup *> filters;
    QWidget * parentW = qobject_cast<QWidget *>(this->sender());
    KKSObjEditor* indSrc = oef->createObjRecEditor(IO_IO_ID, IO_IO_ID, filters, tr ("Select indicator type"), 0, true, false, Qt::WindowModal, parentW);
    if (indSrc->exec () == QDialog::Accepted)
    {
        int idObject = indSrc->getID();
        KKSObject * wObj = loader->loadIO (idObject);
        indVal->setIOSrc (wObj);
        lEIndSrc->setText (wObj->name());
        if (wObj)
            wObj->release ();
    }
}

void KKSIndFactory :: loadIndIOSRC1 (KKSIndicatorValue * indVal, QLineEdit * lEIndSrc1)
{
    if (!indVal || !lEIndSrc1)
        return;
    KKSList<const KKSFilterGroup *> filters;
    QWidget * parentW = qobject_cast<QWidget *>(this->sender());
    KKSObjEditor* indSrc = oef->createObjRecEditor(IO_IO_ID, IO_IO_ID, filters, tr ("Select indicator type"), 0, true, false, Qt::WindowModal, parentW);
    if (indSrc->exec () == QDialog::Accepted)
    {
        int idObject = indSrc->getID();
        KKSObject * wObj = loader->loadIO (idObject);
        indVal->setIOSrc1 (wObj);
        lEIndSrc1->setText (wObj->name());
        if (wObj)
            wObj->release ();
    }
}

void KKSIndFactory :: viewIndIOSRC (KKSIndicatorValue * indVal)
{
    if (!indVal || !indVal->ioSrc())
        return;
    KKSObject * wObj = indVal->ioSrc();//loader->loadIO (, true);
    int idObj = wObj->id ();
    bool isModal = false;//editor->isModal ();
    QString s;
    this->oef->editExistOE (0, IO_IO_ID, idObj, wObj->category(), s, 0, isModal);
}

void KKSIndFactory :: viewIndIOSRC1 (KKSIndicatorValue * indVal)
{
    if (!indVal || !indVal->ioSrc1())
        return;
    KKSObject * wObj = indVal->ioSrc1();//loader->loadIO (, true);
    int idObj = wObj->id ();
    bool isModal = false;//editor->isModal ();
    QString s;
    this->oef->editExistOE (0, IO_IO_ID, idObj, wObj->category(), s, 0, isModal);
}

void KKSIndFactory :: addNewIOIndicator (KKSObject * io, QWidget * indWidget)
{
    if (!io || !indWidget)
        return;
    QGridLayout * gIndLay = qobject_cast<QGridLayout *> (indWidget->layout());
    KKSObject * refInds = loader->loadIO (IO_INDICATOR_ID);
    if (!refInds)
    {
        QWidget * parentW = qobject_cast <QWidget *>(this->sender());
        QMessageBox::warning (parentW, tr ("Add new indicator value"), tr("Cannot load indicator reference"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    KKSCategory * c = refInds->category ();
    if (!c)
    {
        QWidget * parentW = qobject_cast <QWidget *>(this->sender());
        QMessageBox::warning (parentW, tr ("Add new indicator value"), tr("Invalid category"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    
    c = c->tableCategory();
    if (!c)
    {
        QWidget * parentW = qobject_cast <QWidget *>(this->sender());
        QMessageBox::warning (parentW, tr ("Add new indicator value"), tr("Invalid category"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    KKSList<const KKSFilterGroup *> filters;
    if (io->indicatorValues().count())
    {
        QString values = QString ("select id from indicator where id not in (");
        QList<int> indIOVals;
        for (int i=0; i<io->indicatorValues().count(); i++)
        {
            values += QString::number (io->indicatorValue(i)->indicator()->id());
            if (i < io->indicatorValues().count()-1)
                values += QString(",");
        }
        values += QString (")");
        KKSFilter * f = c->createFilter (ATTR_ID, values, KKSFilter::foInSQL);
        KKSFilterGroup * fg = new KKSFilterGroup (false);
        fg->addFilter (f);
        filters.append (fg);
        fg->release ();
    }
    QWidget * parentW = qobject_cast <QWidget *>(this->sender());
    KKSObjEditor* indSrcW = oef->createObjRecEditor(IO_IO_ID, IO_INDICATOR_ID, filters, tr ("Select indicator"), 0, true, false, Qt::WindowModal, parentW);
    if (indSrcW->exec () == QDialog::Accepted)
    {
        int idIndicator = indSrcW->getID();
        KKSIndicator * ind = loader->loadIndicator(idIndicator);
        KKSIndicatorValue * indVal = new KKSIndicatorValue ();
        if (!indVal)
            return;
        indVal->setIndicator(ind);
        QWidget * parentW = 0;//qobject_cast<QWidget *>(this->sender());
        IndicatorForm * iForm = new IndicatorForm (indVal, true, parentW);
        iForm->setWindowModality (Qt::WindowModal);//setModal ()

        connect (iForm,
                 SIGNAL (loadIndicatorType (KKSIndicatorValue *, QLineEdit *)),
                 this,
                 SLOT (loadIndType (KKSIndicatorValue *, QLineEdit *))
                );
        connect (iForm,
                SIGNAL (loadIOSrc (KKSIndicatorValue *, QLineEdit *)),
                this,
                SLOT (loadIndIOSRC (KKSIndicatorValue *, QLineEdit *))
                );
        connect (iForm,
                 SIGNAL (viewIOSrc (KKSIndicatorValue *)),
                 this,
                 SLOT (viewIndIOSRC (KKSIndicatorValue *))
                );
        connect (iForm,
                 SIGNAL (viewIOSrc1 (KKSIndicatorValue *)),
                 this,
                 SLOT (viewIndIOSRC1 (KKSIndicatorValue *))
                );

        if (iForm->exec () == QDialog::Accepted)
        {
            KKSIndicatorValue * iVal = iForm->indValue();
            KKSList<KKSIndicatorValue *> iVals = io->indicatorValues();
            int i_ind(-1);
            for (int i=0; i<iVals.count() && i_ind < 0; i++)
                if (iVals[i] && iVals[i]->id() == idIndicator)
                {
                    iVals[i] = iVal;
                    i_ind = i;
                }
            io->setIndicatorValues (iVals);
            io->addIndicatorValue(iVal);
            int nr = gIndLay ? gIndLay->rowCount() : -1;
            int nc = gIndLay ? gIndLay->columnCount() : -1;
            qDebug () << __PRETTY_FUNCTION__ << nr << nc;
            if (nr > 0 && qobject_cast <KKSObjEditor *>(this->sender()))
            {
                KKSObjEditor * editor = qobject_cast <KKSObjEditor *>(this->sender());
                editor->setObjChanged (true);
                if (nr < 2)
                    this->putIndWidget(iVal, editor, gIndLay, nr);
                else
                {
                    QLayoutItem * lItem = gIndLay->itemAtPosition (nr-1, 0);
                    QSpacerItem * sp = lItem->spacerItem();
                    gIndLay->removeItem (sp);
                    this->putIndWidget(iVal, editor, gIndLay, nr-1);
                    gIndLay->addItem (sp, nr, 0, 1, 4);//, Qt::AlignLeft | Qt::AlignBottom);
                }
                qDebug () << __PRETTY_FUNCTION__ << gIndLay->rowCount();
            }
        }
        delete iForm;
    }
}