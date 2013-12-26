/*
 * File:   KKSHistWidget.cpp
 * Author: yuriyrusinov
 *
 * Created on 12 Декабрь 2013 г., 16:57
 */

#include <QPainter>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QValidator>
#include <QtDebug>

#include <KKSHistogram.h>
#include <KKSAttrValue.h>
#include "KKSCharts.h"
#include "KKSHistWidget.h"
#include "KKSHistDrawWidget.h"
#include "KKSValue.h"
#include <KKSCategory.h>
#include <KKSObject.h>

KKSHistWidget::KKSHistWidget(const KKSAttrValue *av, KKSIndAttrClass isSys, QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags), KKSAttrWidget(av, isSys),
    lEFrom (new QLineEdit),
    lETo (new QLineEdit),
    lECount (new QLineEdit),
    cbScenario (new QComboBox),
    cbVariant (new QComboBox),
    cbCategory (new QComboBox),
    cbSource (new QComboBox),
    cbReceiver (new QComboBox),
    wHistDrawW (new KKSHistDrawWidget)
{
    init ();
    
    connect (lEFrom, SIGNAL (editingFinished()), this, SLOT (fromChanged ()) );
    connect (lETo, SIGNAL (editingFinished ()), this, SLOT (toChanged ()) );
    connect (lECount, SIGNAL (editingFinished ()), this, SLOT (numChanged ()) );
    
    connect (cbCategory, SIGNAL (activated(int)), this, SLOT (catChanged (int)) );
    connect (cbSource, SIGNAL (activated(int)), this, SLOT (ioChanged(int)) );
}

KKSHistWidget::~KKSHistWidget()
{
    delete wHistDrawW;
    delete cbReceiver;
    delete cbSource;
    delete cbCategory;
    delete cbVariant;
    delete cbScenario;
    delete lECount;
    delete lETo;
    delete lEFrom;
}

void KKSHistWidget::paintEvent(QPaintEvent *event)
{
    QVariant val = m_av->value().valueVariant();
    KKSHistogram m_hist = val.value<KKSHistogram>();
    if (m_hist.isEmpty())
    {
        QWidget::paintEvent(event);
        return;
    }

    QPainter painter(this);
    int n = m_hist.size();
    qreal barWidth = width() / (qreal)n;
    int h = height ();
    QMap<int, double> m_h = m_hist.getVec();
    int i=0;
    for (QMap<int, double>::const_iterator p = m_h.constBegin(); p != m_h.constEnd(); p++)
    {
        qreal y = p.value() * h;
        // draw level
        painter.fillRect(barWidth * i, h - y, barWidth * (i + 1), h, Qt::red);
        // clear the rest of the control
        painter.fillRect(barWidth * i, 0, barWidth * (i + 1), h - y, Qt::black);
        i++;
    }
}

void KKSHistWidget::init (void)
{
    QGridLayout * gLay = new QGridLayout (this);
    QGroupBox * gbParams = new QGroupBox (tr("Parameters"), this);
    gLay->addWidget (gbParams, 0, 0, 1, 1);
    
    QGridLayout * gParLay = new QGridLayout (gbParams);
    QHBoxLayout * hFromLay = new QHBoxLayout;
    QLabel * lFrom = new QLabel (tr("From:"), this);
    hFromLay->addWidget (lFrom);
    hFromLay->addWidget (lEFrom);
    QValidator * dFromVal = new QDoubleValidator (this);
    lEFrom->setValidator (dFromVal);
    gParLay->addLayout (hFromLay, 0, 0, 1, 1);
    QLabel * lTo = new QLabel (tr("To:"), this);
    QHBoxLayout * hToLay = new QHBoxLayout;
    hToLay->addWidget (lTo);
    hToLay->addWidget (lETo);
    QValidator * dToVal = new QDoubleValidator (this);
    lETo->setValidator (dToVal);
    gParLay->addLayout (hToLay, 1, 0, 1, 1);
    QLabel * lNum = new QLabel (tr("Number of bins:"), this);
    QHBoxLayout * hNumLay = new QHBoxLayout;
    hNumLay->addWidget (lNum);
    QValidator * nCountVal = new QIntValidator (1, 1000000, this);
    lECount->setValidator (nCountVal);
    hNumLay->addWidget (lECount);
    gParLay->addLayout (hNumLay, 2, 0, 1, 1);
    
    QGroupBox * gbFilters = new QGroupBox (tr("Filters"), this);
    gLay->addWidget (gbFilters, 1, 0, 1, 1);
    QGridLayout * gFiltLay = new QGridLayout (gbFilters);
    QLabel * lScenario = new QLabel (tr("Scenario"), this);
    QHBoxLayout * hScenarioLay = new QHBoxLayout;
    hScenarioLay->addWidget (lScenario);
    hScenarioLay->addWidget (cbScenario);
    gFiltLay->addLayout(hScenarioLay, 0, 0, 1, 1);
    QLabel * lVariant = new QLabel (tr("Variant"), this);
    QHBoxLayout * hVariantLay = new QHBoxLayout;
    hVariantLay->addWidget (lVariant);
    hVariantLay->addWidget(cbVariant);
    gFiltLay->addLayout (hVariantLay, 1, 0, 1, 1);
    QLabel * lCategory = new QLabel (tr("Category"), this);
    QHBoxLayout * hCatLay = new QHBoxLayout;
    hCatLay->addWidget (lCategory);
    hCatLay->addWidget(cbCategory);
    gFiltLay->addLayout (hCatLay, 2, 0, 1, 1);
    
    QLabel * lSource = new QLabel (tr("Source"), this);
    QHBoxLayout * hSourceLay = new QHBoxLayout;
    hSourceLay->addWidget (lSource);
    hSourceLay->addWidget (cbSource);
    gFiltLay->addLayout (hSourceLay, 3, 0, 1, 1);

    QLabel * lReceiver = new QLabel (tr("Receiver"), this);
    QHBoxLayout * hRecLay = new QHBoxLayout;
    hRecLay->addWidget (lReceiver);
    hRecLay->addWidget (cbReceiver);
    gFiltLay->addLayout (hRecLay, 4, 0, 1, 1);
    gLay->addWidget (wHistDrawW, 5, 0, 1, 1);
//    gLay->addWidget (wCharts, 5, 0, 1, 1);
}

void KKSHistWidget::loadScenario (const QMap<int, QString>& scList)
{
    cbScenario->clear ();
    for (QMap<int, QString>::const_iterator p=scList.constBegin();
            p != scList.constEnd();
            ++p)
    {
        cbScenario->addItem (p.value(), p.key());
    }
}

void KKSHistWidget::loadVariants (const QMap<int, QString>& varList)
{
    cbVariant->clear ();
    for (QMap<int, QString>::const_iterator p=varList.constBegin();
            p != varList.constEnd();
            ++p)
    {
        cbVariant->addItem (p.value(), p.key());
    }
}

void KKSHistWidget::loadCategories (const KKSMap<int, KKSCategory *>& catList)
{
    this->cbCategory->clear();
    this->cbCategory->addItem (tr("No selected category"), -1);
    for (KKSMap<int, KKSCategory *>::const_iterator p=catList.constBegin();
            p != catList.constEnd();
            ++p)
    {
        cbCategory->addItem (p.value()->name(), p.key());
    }
}

void KKSHistWidget::loadIOList (const KKSMap<int, KKSObject *>& IOList)
{
    cbSource->clear ();
    for (KKSMap<int, KKSObject *>::const_iterator p = IOList.constBegin();
            p != IOList.constEnd();
            ++p)
    {
        cbSource->addItem (p.value()->name(), p.key());
    }
}

void KKSHistWidget::loadRecvList (const QMap<int, QString>& posList)
{
    cbReceiver->clear ();
    for (QMap<int, QString>::const_iterator p=posList.constBegin();
            p != posList.constEnd();
            ++p)
    {
        cbReceiver->addItem (p.value(), p.key());
    }
}

void KKSHistWidget::setHist (const KKSHistogram& hist)
{
    QVariant v = QVariant::fromValue<KKSHistogram>(hist);
    lEFrom->setText (QString::number (hist.getXMin()));
    lETo->setText (QString::number (hist.getXMax()));
    lECount->setText (QString::number (hist.getVec().count()));
    int idCat = hist.category() ? hist.category()->id() : -1;
    if (idCat > 0)
    {
        int cbInd = this->cbCategory->findData (idCat, Qt::UserRole);
        cbCategory->setCurrentIndex (cbInd);
    }
    emit valueChanged (m_av->id(), m_isSystem, v);
}

void KKSHistWidget::fromChanged ()
{
    QString text = lEFrom->text ();
    KKSHistogram hist = m_av->value().valueVariant().value<KKSHistogram>();
    double x = text.toDouble();
    double xMax = hist.getXMax();
    hist.setRange (x, xMax);
    QVariant v = QVariant::fromValue<KKSHistogram>(hist);
    qDebug () << __PRETTY_FUNCTION__ << v;
    if (!hist.category())
    {
        int idCat = cbCategory->itemData (cbCategory->currentIndex()).toInt ();
        emit loadCategory (idCat, v);
    }
    emit valueChanged (m_av->id(), m_isSystem, v);
}

void KKSHistWidget::toChanged ()
{
    QString text = lETo->text ();
    KKSHistogram hist = m_av->value().valueVariant().value<KKSHistogram>();
    hist.setRange (hist.getXMin(), text.toDouble());
    QVariant v = QVariant::fromValue<KKSHistogram>(hist);
    emit valueChanged (m_av->id(), m_isSystem, v);
}

void KKSHistWidget::numChanged ()
{
    QString text = lECount->text ();
    KKSHistogram hist = m_av->value().valueVariant().value<KKSHistogram>();
    hist.setSize(text.toInt());
    QVariant v = QVariant::fromValue<KKSHistogram>(hist);
    emit valueChanged (m_av->id(), m_isSystem, v);
}

void KKSHistWidget::catChanged (int cIndex)
{
    int idCat = cbCategory->itemData (cIndex).toInt ();
    KKSHistogram hist = m_av->value().valueVariant().value<KKSHistogram>();
    QVariant v = QVariant::fromValue<KKSHistogram>(hist);
    emit loadCategory (idCat, v);

    emit valueChanged (m_av->id(), m_isSystem, v);
}

void KKSHistWidget::ioChanged (int ioIndex)
{
    int ioId = this->cbSource->itemData (ioIndex).toInt ();
    KKSHistogram hist = m_av->value().valueVariant().value<KKSHistogram>();
    QVariant v = QVariant::fromValue<KKSHistogram>(hist);
    emit loadIO (ioId, v);
    emit valueChanged (m_av->id(), m_isSystem, v);
}

void KKSHistWidget::calcHist (void)
{
    
}
