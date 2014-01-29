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
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QtDebug>
#include <QPair>

#include <KKSHistogram.h>
#include <KKSAttrValue.h>
#include "ui_histogram_widget.h"
#include "KKSHistWidget.h"
#include "KKSHistDrawWidget.h"
#include "KKSValue.h"
#include <KKSCategory.h>
#include "KKSCheckableModel.h"
#include <KKSObject.h>
#include <KKSQwtHistogram.h>

KKSHistWidget::KKSHistWidget(const KKSAttrValue *av, KKSIndAttrClass isSys, QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags), KKSAttrWidget(av, isSys),
    m_hist (0),
    m_qwtHistogramWidget(NULL),
    UI (new Ui::histogram_widget)
{

    UI->setupUi (this);

    if (av && av->value().valueVariant().canConvert<KKSHistogram>())
    {
        KKSValue hval = av->value();
        QVariant hvalVar = hval.valueVariant();
        m_hist = new KKSHistogram (hvalVar.value<KKSHistogram>());
    }
    else
        m_hist = new KKSHistogram;
    

    //ksa connect (cbCategory, SIGNAL (activated(int)), this, SLOT (catChanged (int)) );
    //ksa connect (cbIORef, SIGNAL (activated(int)), this, SLOT (ioChanged(int)) );

    connect (UI->pbUpdate, SIGNAL (clicked()), this, SLOT (calcHist()) );
}

KKSHistWidget::~KKSHistWidget()
{
    m_hist->release ();
    delete m_qwtHistogramWidget;
}


void KKSHistWidget::init (void)
{
    if(!m_av)
        return;

    m_qwtHistogramWidget = new KKSQwtPlotWidget(m_av->attribute()->title(),
                                                m_hist,
                                                this,
                                                Qt::Window | 
                                                Qt::WindowTitleHint | 
                                                Qt::WindowSystemMenuHint | 
                                                Qt::WindowMinimizeButtonHint | 
                                                Qt::WindowMaximizeButtonHint);

    m_qwtHistogramWidget->setWindowTitle(tr("Histogram view"));    
    
    QValidator * dFromVal = new QDoubleValidator (this);
    UI->leFrom->setValidator (dFromVal);
    if (m_hist && !m_hist->isEmpty())
        UI->leFrom->setText (QString::number (m_hist->getXMin()));


    QValidator * dToVal = new QDoubleValidator (this);
    UI->leTo->setValidator (dToVal);
    if (m_hist && !m_hist->isEmpty())
        UI->leTo->setText (QString::number (m_hist->getXMax()));

    QValidator * nCountVal = new QIntValidator (1, 1000000, this);
    UI->leCount->setValidator (nCountVal);
    if (m_hist && !m_hist->isEmpty())
        UI->leCount->setText (QString::number (m_hist->size()));
}

void KKSHistWidget::calcHist (void)
{
    if(UI->leFrom->text().toInt() == UI->leTo->text().toInt() || UI->leCount->text().toInt() == 0){
        QMessageBox::warning(this, tr("Incorrect params"), tr("Please, input correct parameters!"));
        return;
    }

    KKSValue v = getVal();//(hStr, KKSAttrType::atHistogram);
    QString hStr = v.valueForInsert();
    emit valueChanged (m_av->id(), m_isSystem, hStr);//v.valueVariant());

    if(!m_qwtHistogramWidget){
        m_qwtHistogramWidget = new KKSQwtPlotWidget(m_av->attribute()->title(), 
                                                    m_hist,
                                                    this,
                                                    Qt::Window | 
                                                    Qt::WindowTitleHint | 
                                                    Qt::WindowSystemMenuHint | 
                                                    Qt::WindowMinimizeButtonHint | 
                                                    Qt::WindowMaximizeButtonHint);

        m_qwtHistogramWidget->setWindowTitle(tr("Histogram view"));    
    }

    m_qwtHistogramWidget->resize(600,400);
    
    m_qwtHistogramWidget->show();
    if(m_qwtHistogramWidget->isMinimized())
        m_qwtHistogramWidget->showNormal();

    m_qwtHistogramWidget->repaint();
}

void KKSHistWidget::loadScenario (const QMap<int, QString>& scList)
{
    QAbstractItemModel * scMod = UI->lVScenarios->model ();
    int nsc = scList.count();
    if (!scMod)
        scMod = new KKSCheckableModel (nsc, 1);
    else
    {
        int nr = scMod->rowCount();
        scMod->removeRows (0, nr);
        scMod->insertRows (0, nsc);
        if (scMod->columnCount() == 0)
            scMod->insertColumns (0, 1);
    }
    //UI->lwScenarios->clear ();
    int i=0;
    QList<int> scHList = hist->getScenario();
    for (QMap<int, QString>::const_iterator p=scList.constBegin();
            p != scList.constEnd();
            ++p)
    {
        QModelIndex scInd = scMod->index (i, 0);
        scMod->setData (scInd, p.key(), Qt::UserRole);
        if (scHList.contains(p.key()))
            scMod->setData (scInd, Qt::Checked, Qt::CheckStateRole);
        else
            scMod->setData (scInd, Qt::Unchecked, Qt::CheckStateRole);
        scMod->setData (scInd, p.value(), Qt::DisplayRole);
        i++;
    }
    if (!UI->lVScenarios->model ())
    {
        UI->lVScenarios->setModel (scMod);
        connect (scMod,
                 SIGNAL (dataChanged(const QModelIndex&, const QModelIndex&)),
                 this,
                 SLOT (scenarioSet(const QModelIndex&, const QModelIndex&))
                );
    }
/*    if (hist && hist->getScenario() > 0)
    {
        int indSc = hist->getScenario();
        int cind = cbScenario->findData (QVariant (indSc));
        cbScenario->setCurrentIndex (cind);
    }
 */
}

void KKSHistWidget::loadVariants (const QMap<int, QString>& varList)
{
    QAbstractItemModel * varModel = UI->lvVariants->model ();
    bool isSet (true);
    int nvr = varList.count();
    if (!varModel)
    {
        varModel = new KKSCheckableModel (nvr, 1);
        isSet = false;
    }
    else
    {
        int nr = varModel->rowCount();
        varModel->removeRows (0, nr);
        varModel->insertRows (0, nvr);
        if (varModel->columnCount () == 0)
            varModel->insertColumns (0, 1);
    }
    //UI->lwVariants->clear ();
    int i=0;
    QList<int> vHList = hist->getVariant();
    for (QMap<int, QString>::const_iterator p=varList.constBegin();
            p != varList.constEnd();
            ++p)
    {
        QModelIndex vIndex = varModel->index (i, 0);
        varModel->setData (vIndex, p.key(), Qt::UserRole);
        if (vHList.contains (p.key()))
            varModel->setData (vIndex, Qt::Checked, Qt::CheckStateRole);
        else
            varModel->setData (vIndex, Qt::Unchecked, Qt::CheckStateRole);
        varModel->setData (vIndex, p.value(), Qt::DisplayRole);
        i++;
    }
    if (!isSet)
    {
        UI->lvVariants->setModel (varModel);
        connect (varModel,
                 SIGNAL (dataChanged(const QModelIndex&, const QModelIndex&)),
                 this,
                 SLOT (variantSet(const QModelIndex&, const QModelIndex&))
                );
    }
/*    if (hist && hist->getVariant() > 0)
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
    QAbstractItemModel * catMod = UI->lvCategories->model();
    bool isSet (!catMod);
    int ncr = catList.count();
    if (!catMod)
        catMod = new KKSCheckableModel (ncr, 1);
    else
    {
        int nr = catMod->rowCount();
        catMod->removeRows (0, nr);
        if (catMod->columnCount() == 0)
            catMod->insertColumns (0, 1);
    }
    int i = 0;
    for (KKSMap<int, KKSCategory *>::const_iterator p=catList.constBegin();
            p != catList.constEnd();
            ++p)
    {
        QModelIndex catInd = catMod->index (i++, 0);
        catMod->setData (catInd, p.key(), Qt::UserRole);
        catMod->setData (catInd, p.value()->name(), Qt::DisplayRole);
    }
    if (!isSet)
        UI->lvCategories->setModel (catMod);
/*    if (hist && hist->category())
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
    QAbstractItemModel *ioMod = UI->lvIOs->model ();
    bool isSet (!ioMod);
    int ioc = IOList.count ();
    if (!ioMod)
        ioMod = new KKSCheckableModel (ioc, 1);
    else
    {
        int nr = ioMod->rowCount();
        ioMod->removeRows (0, nr);
        if (ioMod->columnCount() == 0)
            ioMod->insertColumns (0, 1);
    }
    int i = 0;
    for (KKSMap<int, KKSObject *>::const_iterator p=IOList.constBegin();
            p != IOList.constEnd();
            ++p)
    {
        QModelIndex ioInd = ioMod->index (i++, 0);
        ioMod->setData (ioInd, p.key(), Qt::UserRole);
        ioMod->setData (ioInd, p.value()->name(), Qt::DisplayRole);
    }
    if (!isSet)
        UI->lvIOs->setModel (ioMod);
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

void KKSHistWidget::setHist (const KKSHistogram& shist)
{
    
    if (m_hist)
        m_hist->release ();
    
    m_hist = new KKSHistogram (shist);
    
    QVariant v = QVariant::fromValue<KKSHistogram>(shist);
    

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

    
    //ksa (qobject_cast<KKSHistDrawWidget *>(wHistDrawW))->setData (hist->getVec());
    
    emit valueChanged (m_av->id(), m_isSystem, v);
}
/*
void KKSHistWidget::catChanged (int cIndex)
{
    int idCat = cbCategory->itemData (cIndex).toInt ();
    KKSHistogram hist = m_av->value().valueVariant().value<KKSHistogram>();
    QVariant v = QVariant::fromValue<KKSHistogram>(hist);
    emit loadCategory (idCat, &hist);

    //qDebug () << __PRETTY_FUNCTION__ << v << hist.toString();
    emit valueChanged (m_av->id(), m_isSystem, v);
}

void KKSHistWidget::ioChanged (int ioIndex)
{
    int ioId = this->cbIORef->itemData (ioIndex).toInt ();
    KKSHistogram hist = m_av->value().valueVariant().value<KKSHistogram>();
    QVariant v = QVariant::fromValue<KKSHistogram>(hist);
    emit loadIO (ioId, &hist);
    //qDebug () << __PRETTY_FUNCTION__ << v << hist.toString();
    emit valueChanged (m_av->id(), m_isSystem, v);
}
 */

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
        hist->setValue (i, QPair<double, double>(0.0, 0.0));
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
    
    if(m_hist)
        m_hist->release();
    m_hist = new KKSHistogram(*hist);
    
    hist->release ();
    return v;
}

void KKSHistWidget::scenarioSet (const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    //qDebug () << __PRETTY_FUNCTION__ << topLeft << bottomRight;
    int iRow0 = topLeft.row();
    int iRow1 = bottomRight.row();
    QList<int> scList = hist->getScenario();
    QAbstractItemModel * scMod = qobject_cast<QAbstractItemModel *>(sender());
    for (int i=iRow0; i<=iRow1; i++)
    {
        QModelIndex wIndex = scMod->index (iRow0, 0, topLeft.parent());
        Qt::CheckState chState = (Qt::CheckState)(scMod->data(wIndex, Qt::CheckStateRole).toInt());
        int idSc = scMod->data (wIndex, Qt::UserRole).toInt();
        if (chState == Qt::Checked && !scList.contains (idSc))
            scList.append (idSc);
        else if (chState == Qt::Unchecked && scList.contains(idSc))
            scList.removeOne(idSc);
    }
    //qDebug () << __PRETTY_FUNCTION__ << scList;
    hist->setScenario(scList);
}

void KKSHistWidget::variantSet (const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    //qDebug () << __PRETTY_FUNCTION__ << topLeft << bottomRight;
    int iRow0 = topLeft.row();
    int iRow1 = bottomRight.row();
    QList<int> vList = hist->getVariant();
    QAbstractItemModel * vMod = qobject_cast<QAbstractItemModel *>(sender());
    for (int i=iRow0; i<=iRow1; i++)
    {
        QModelIndex wIndex = vMod->index (iRow0, 0, topLeft.parent());
        Qt::CheckState chState = (Qt::CheckState)(vMod->data(wIndex, Qt::CheckStateRole).toInt());
        int idV = vMod->data (wIndex, Qt::UserRole).toInt();
        if (chState == Qt::Checked && !vList.contains (idV))
            vList.append (idV);
        else if (chState == Qt::Unchecked && vList.contains(idV))
            vList.removeOne(idV);
    }
    hist->setVariant(vList);
}

void KKSHistWidget::catSet (const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    qDebug () << __PRETTY_FUNCTION__ << topLeft << bottomRight;
    
}

void KKSHistWidget::ioSet (const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    qDebug () << __PRETTY_FUNCTION__ << topLeft << bottomRight;
    
}

void KKSHistWidget::servSet (const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    qDebug () << __PRETTY_FUNCTION__ << topLeft << bottomRight;
    
}
