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
#include <QMessageBox>
#include <QPushButton>
#include <QtDebug>

#include <KKSHistogram.h>
#include <KKSAttrValue.h>
#include "KKSCharts.h"
#include "ui_histogram_widget.h"
#include "KKSHistWidget.h"
#include "KKSHistDrawWidget.h"
#include "KKSValue.h"
#include <KKSCategory.h>
#include <KKSObject.h>

KKSHistWidget::KKSHistWidget(const KKSAttrValue *av, KKSIndAttrClass isSys, QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags), KKSAttrWidget(av, isSys),
/*
    lEFrom (new QLineEdit),
    lETo (new QLineEdit),
    lECount (new QLineEdit),
    cbScenario (new QComboBox),
    cbVariant (new QComboBox),
    cbCategory (new QComboBox),
    cbIORef (new QComboBox),
    cbSource (new QComboBox),
    cbReceiver (new QComboBox),
*/
    hist (0),
    UI (new Ui::histogram_widget)
    //pbCalc (new QPushButton (tr("&Update")))
{

    UI->setupUi (this);

    if (av && av->value().valueVariant().canConvert<KKSHistogram>())
    {
        KKSValue hval = av->value();
        QVariant hvalVar = hval.valueVariant();
        hist = new KKSHistogram (hvalVar.value<KKSHistogram>());
    }
    else
        hist = new KKSHistogram;
    

    init ();
    
    //ksa connect (cbCategory, SIGNAL (activated(int)), this, SLOT (catChanged (int)) );
    //ksa connect (cbIORef, SIGNAL (activated(int)), this, SLOT (ioChanged(int)) );

    connect (UI->pbUpdate, SIGNAL (clicked()), this, SLOT (calcHist()) );
}

KKSHistWidget::~KKSHistWidget()
{
    hist->release ();
    delete wHistDrawW;
/*
    delete pbCalc;
    delete cbReceiver;
    delete cbSource;
    delete cbIORef;
    delete cbCategory;
    delete cbVariant;
    delete cbScenario;
    delete lECount;
    delete lETo;
    delete lEFrom;
*/
}

/*
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
 */

void KKSHistWidget::init (void)
{
    
    UI->wHistogramParent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QHBoxLayout * hHistLay = new QHBoxLayout(UI->wHistogramParent);
    wHistDrawW = new KKSHistDrawWidget (UI->wHistogramParent);//,
                                           //Qt::Window | 
                                           //Qt::WindowTitleHint | 
                                           //Qt::WindowSystemMenuHint | 
                                           //Qt::WindowMinimizeButtonHint | 
                                           //Qt::WindowMaximizeButtonHint | 
                                           //Qt::WindowSoftkeysRespondHint);
    wHistDrawW->setMinimumSize(600, 400);

    hHistLay->addWidget(wHistDrawW);


    QValidator * dFromVal = new QDoubleValidator (this);
    UI->leFrom->setValidator (dFromVal);
    if (hist && !hist->isEmpty())
        UI->leFrom->setText (QString::number (hist->getXMin()));


    QValidator * dToVal = new QDoubleValidator (this);
    UI->leTo->setValidator (dToVal);
    if (hist && !hist->isEmpty())
        UI->leTo->setText (QString::number (hist->getXMax()));

    QValidator * nCountVal = new QIntValidator (1, 1000000, this);
    UI->leCount->setValidator (nCountVal);
    if (hist && !hist->isEmpty())
        UI->leCount->setText (QString::number (hist->size()));

    //wHistDrawW->setParent(UI->wHistogramParent);
    //wHistDrawW->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    /*
    QGridLayout * gLay = new QGridLayout (this);
    QGroupBox * gbParams = new QGroupBox (tr("Parameters"), this);
    gLay->addWidget (gbParams, 0, 0, 1, 1);
    
    QGridLayout * gParLay = new QGridLayout (gbParams);
    QGroupBox * gbHist = new QGroupBox (tr("Histogram"), this);
    QGridLayout * gHistLay = new QGridLayout (gbHist);
    QHBoxLayout * hFromLay = new QHBoxLayout;
    QLabel * lFrom = new QLabel (tr("From:"), this);
    hFromLay->addWidget (lFrom);
    hFromLay->addWidget (lEFrom);
    QValidator * dFromVal = new QDoubleValidator (this);
    lEFrom->setValidator (dFromVal);
    if (hist && !hist->isEmpty())
        lEFrom->setText (QString::number (hist->getXMin()));
    gHistLay->addLayout (hFromLay, 0, 0, 1, 1);
    QLabel * lTo = new QLabel (tr("To:"), this);
    QHBoxLayout * hToLay = new QHBoxLayout;
    hToLay->addWidget (lTo);
    hToLay->addWidget (lETo);
    QValidator * dToVal = new QDoubleValidator (this);
    lETo->setValidator (dToVal);
    if (hist && !hist->isEmpty())
        lETo->setText (QString::number (hist->getXMax()));
    gHistLay->addLayout (hToLay, 1, 0, 1, 1);
    QLabel * lNum = new QLabel (tr("Number of bins:"), this);
    QHBoxLayout * hNumLay = new QHBoxLayout;
    hNumLay->addWidget (lNum);
    QValidator * nCountVal = new QIntValidator (1, 1000000, this);
    lECount->setValidator (nCountVal);
    if (hist && !hist->isEmpty())
        lECount->setText (QString::number (hist->size()));
    hNumLay->addWidget (lECount);
    gHistLay->addLayout (hNumLay, 2, 0, 1, 1);
    gParLay->addWidget (gbHist, 0, 0, 2, 1);
    
    QGroupBox * gbFilters = new QGroupBox (tr("Filters"), this);
    QString tName = this->m_av->attribute()->tableName();
    //qDebug () << __PRETTY_FUNCTION__ << tName;
    bool isFiltVisible = (tName.compare(QString("message_series"), Qt::CaseInsensitive) == 0);
    gbFilters->setVisible(isFiltVisible);
    gParLay->addWidget (gbFilters, 0, 1, 1, 1);
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
    
    QLabel * lIO = new QLabel (tr("Information object"), this);
    QHBoxLayout * hIOLay = new QHBoxLayout;
    hIOLay->addWidget (lIO);
    hIOLay->addWidget (cbIORef);
    gFiltLay->addLayout (hIOLay, 3, 0, 1, 1);

    QLabel * lSource = new QLabel (tr("Source"), this);
    QHBoxLayout * hSourceLay = new QHBoxLayout;
    hSourceLay->addWidget (lSource);
    hSourceLay->addWidget (cbSource);
    gFiltLay->addLayout (hSourceLay, 4, 0, 1, 1);
    
    QLabel * lReceiver = new QLabel (tr("Receiver"), this);
    QHBoxLayout * hRecLay = new QHBoxLayout;
    hRecLay->addWidget (lReceiver);
    hRecLay->addWidget (cbReceiver);
    gFiltLay->addLayout (hRecLay, 5, 0, 1, 1);
    //gLay->addWidget (wHistDrawW, 5, 0, 1, 1);

    //сама гистограмма
    QGroupBox * gbPic = new QGroupBox (tr("View"), this);
    QGridLayout * gPicLay = new QGridLayout (gbPic);
    gPicLay->addWidget (wHistDrawW, 0, 0, 1, 1);
    wHistDrawW->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    gLay->addWidget (gbPic, 5, 0, 1, 1);


    gParLay->addWidget (pbCalc, 2 - (isFiltVisible ? 1 : 0), 0, 1, 2, Qt::AlignRight);
//    gLay->addWidget (wCharts, 5, 0, 1, 1);
*/
}

void KKSHistWidget::loadScenario (const QMap<int, QString>& scList)
{
    
    //UI->lwScenarios->clear ();
    /*
    for (QMap<int, QString>::const_iterator p=scList.constBegin();
            p != scList.constEnd();
            ++p)
    {
        cbScenario->addItem (p.value(), p.key());
    }
    if (hist && hist->getScenario() > 0)
    {
        int indSc = hist->getScenario();
        int cind = cbScenario->findData (QVariant (indSc));
        cbScenario->setCurrentIndex (cind);
    }
    */
}

void KKSHistWidget::loadVariants (const QMap<int, QString>& varList)
{
    //UI->lwVariants->clear ();
    /*
    for (QMap<int, QString>::const_iterator p=varList.constBegin();
            p != varList.constEnd();
            ++p)
    {
        cbVariant->addItem (p.value(), p.key());
    }
    if (hist && hist->getVariant() > 0)
    {
        int indV = hist->getVariant ();
        int vInd = cbVariant->findData (QVariant (indV));
        cbVariant->setCurrentIndex (vInd);
    }
    */
}

void KKSHistWidget::loadCategories (const KKSMap<int, KKSCategory *>& catList)
{
    //UI->lwCategories->clear();
    /*
    this->cbCategory->addItem (tr("No selected category"), -1);
    for (KKSMap<int, KKSCategory *>::const_iterator p=catList.constBegin();
            p != catList.constEnd();
            ++p)
    {
        cbCategory->addItem (p.value()->name(), p.key());
    }
    if (hist && hist->category())
    {
        qint64 idc = hist->category()->id();
        int catInd = cbCategory->findData (QVariant (idc));
        cbCategory->setCurrentIndex (catInd);
    }
    */
}

void KKSHistWidget::loadIOList (const KKSMap<int, KKSObject *>& IOList)
{
    //UI->lwIO->clear ();
    /*
    for (KKSMap<int, KKSObject *>::const_iterator p = IOList.constBegin();
            p != IOList.constEnd();
            ++p)
    {
        cbIORef->addItem (p.value()->name(), p.key());
    }
    if (hist && hist->srcObject())
    {
        qint64 idIO = hist->srcObject()->id();
        int ioInd = this->cbIORef->findData (QVariant (idIO));
        cbIORef->setCurrentIndex (ioInd);
    }
    */
}

void KKSHistWidget::loadRecvList (const QMap<int, QString>& posList)
{
    /*
    cbReceiver->clear ();
    for (QMap<int, QString>::const_iterator p=posList.constBegin();
            p != posList.constEnd();
            ++p)
    {
        cbReceiver->addItem (p.value(), p.key());
    }
    if (hist && hist->getReceiver() > 0)
    {
        qint64 idr = hist->getReceiver ();
        int indr = cbReceiver->findData (QVariant (idr));
        cbReceiver->setCurrentIndex (indr);
    }
    */
}

void KKSHistWidget::setHist (const KKSHistogram& shist)
{
    if (hist)
        hist->release ();
    
    hist = new KKSHistogram (shist);
    
    QVariant v = QVariant::fromValue<KKSHistogram>(shist);
    
    UI->leFrom->setText (QString::number (hist->getXMin()));
    UI->leTo->setText (QString::number (hist->getXMax()));
    UI->leCount->setText (QString::number (hist->getVec().count()));
    
    /*
    qint64 idCat = hist->category() ? hist->category()->id() : -1;
    if (idCat > 0)
    {
        int cbInd = this->cbCategory->findData (idCat, Qt::UserRole);
        cbCategory->setCurrentIndex (cbInd);
    }
    
    qint64 idIO = hist->srcObject () ? hist->srcObject()->id() : -1;
    if (idIO > 0)
    {
        int ioInd = cbIORef->findData (idIO);
        cbIORef->setCurrentIndex (ioInd);
    }
    qint64 idSc = hist->getScenario();
    if (idSc > 0)
    {
        int scInd = cbScenario->findData (idSc);
        cbScenario->setCurrentIndex (scInd);
    }
    qint64 idVar = hist->getVariant();
    if (idVar > 0)
    {
        int vind = cbVariant->findData (idVar);
        cbVariant->setCurrentIndex (vind);
    }
    qint64 idRecv = hist->getReceiver ();
    if (idRecv > 0)
    {
        int rind = cbReceiver->findData (idRecv);
        cbReceiver->setCurrentIndex (rind);
    }
    */

    
    (qobject_cast<KKSHistDrawWidget *>(wHistDrawW))->setData (hist->getVec());
    
    wHistDrawW->repaint();

    //emit valueChanged (m_av->id(), m_isSystem, v);
}

void KKSHistWidget::catChanged (int cIndex)
{
    /*
    int idCat = cbCategory->itemData (cIndex).toInt ();
    KKSHistogram hist = m_av->value().valueVariant().value<KKSHistogram>();
    QVariant v = QVariant::fromValue<KKSHistogram>(hist);
    emit loadCategory (idCat, &hist);

    //qDebug () << __PRETTY_FUNCTION__ << v << hist.toString();
    emit valueChanged (m_av->id(), m_isSystem, v);
    */
}

void KKSHistWidget::ioChanged (int ioIndex)
{
    /*
    int ioId = this->cbIORef->itemData (ioIndex).toInt ();
    KKSHistogram hist = m_av->value().valueVariant().value<KKSHistogram>();
    QVariant v = QVariant::fromValue<KKSHistogram>(hist);
    emit loadIO (ioId, &hist);
    //qDebug () << __PRETTY_FUNCTION__ << v << hist.toString();
    emit valueChanged (m_av->id(), m_isSystem, v);
    */
}

void KKSHistWidget::calcHist (void)
{
    KKSValue v = getVal();//(hStr, KKSAttrType::atHistogram);
    QString hStr = v.valueForInsert();
    emit valueChanged (m_av->id(), m_isSystem, hStr);//v.valueVariant());
    wHistDrawW->repaint();
}

void KKSHistWidget::saveHist (KKSValue & v)
{
    v = getVal();//KKSValue (hStr, KKSAttrType::atHistogram);
}

void KKSHistWidget::clearIO (void)
{
    /*
    this->cbIORef->clear();
    */
}

KKSValue KKSHistWidget::getVal (void)
{
    double xmin = UI->leFrom->text().toDouble ();
    double xmax = UI->leTo->text().toDouble ();
    if (xmin >= xmax)
    {
        QMessageBox::warning (this, tr("Histogram"), tr("Minimum value has to be little than maximum"), QMessageBox::Ok);
        return KKSValue();
    }

    int num = UI->leCount->text().toInt ();
    
    KKSHistogram * hist = new KKSHistogram;
    hist->setRange (xmin, xmax);
    hist->setSize (num);
    //double dx = (xmax-xmin)/num;
    hist->clear ();
    for (int i=0; i<num; i++)
    {
        //double x = xmin + i*dx;
        hist->setValue (i, 0.0);
    }
    
    /*
    int idScenario = cbScenario->itemData (cbScenario->currentIndex()).toInt();
    int idVariant = cbVariant->itemData (cbVariant->currentIndex()).toInt ();
    int idCategory = cbCategory->itemData (cbCategory->currentIndex()).toInt ();
    int idIOObject = cbIORef->itemData (cbIORef->currentIndex()).toInt ();
    int idReceiver = cbReceiver->itemData (cbReceiver->currentIndex()).toInt ();
    hist->setScenario(idScenario);
    hist->setVariant(idVariant);
    hist->setReceiver(idReceiver);
    emit loadCategory (idCategory, hist);
    emit loadIO (idIOObject, hist);
    */

    QString hStr = hist->toString();
    qDebug () << __PRETTY_FUNCTION__ << hStr;
    KKSValue v (hStr, KKSAttrType::atHistogram);
    hist->release ();
    return v;
}
