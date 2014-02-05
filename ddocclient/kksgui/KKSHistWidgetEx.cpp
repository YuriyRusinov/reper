/*
 * File:   KKSHistWidgetEx.cpp
 * Author: ksa
 *
 * Created on 27.01.2014 ã., 10:00
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
#include <QPair>

#include <KKSHistogram.h>
#include <KKSAttrValue.h>
#include "ui_histogram_widget.h"
#include "KKSHistWidgetEx.h"
#include "KKSValue.h"
#include <KKSCategory.h>
#include "KKSCheckableModel.h"
#include <KKSObject.h>
#include <KKSQwtHistogram.h>

KKSHistWidgetEx::KKSHistWidgetEx(const KKSAttrValue *av, KKSIndAttrClass isSys, QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags), KKSAttrWidget(av, isSys),
    UI (new Ui::histogram_widget),
    m_hist (0),
    m_qwtHistogramWidget(0),
    m_paramsChanged(false),
    m_completed(false)
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
    

    init ();

    connect (UI->pbUpdate, SIGNAL (clicked()), this, SLOT (calcHist()) );
    connect (UI->leCount, SIGNAL(textEdited(const QString &)), this, SLOT(slotParamsChanged()));
}

KKSHistWidgetEx::~KKSHistWidgetEx()
{
    m_hist->release ();
    delete m_qwtHistogramWidget;
}


void KKSHistWidgetEx::init (void)
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

void KKSHistWidgetEx::calcHist (void)
{
    if(UI->leFrom->text().toDouble() >= UI->leTo->text().toDouble() || UI->leCount->text().toInt() <= 0){
        QMessageBox::warning(this, tr("Incorrect params"), tr("Please, input correct parameters!"));
        return;
    }

    if(m_paramsChanged){
        QMessageBox::warning(this, tr(""), tr(""));
        return;
    }

    //KKSValue v = getVal();//(hStr, KKSAttrType::atHistogram);
    //QString hStr = v.valueForInsert();
    
    //emit valueChanged (m_av->id(), m_isSystem, hStr);//v.valueVariant());

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
    else{
        m_qwtHistogramWidget->init(m_hist);
    }

    m_qwtHistogramWidget->resize(600,400);
    
    m_qwtHistogramWidget->show();
    if(m_qwtHistogramWidget->isMinimized())
        m_qwtHistogramWidget->showNormal();

    m_qwtHistogramWidget->repaint();
}

void KKSHistWidgetEx::loadScenario (const QMap<int, QString>& scList)
{
    QAbstractItemModel * scMod = UI->lVScenarios->model ();
    int nsc = scList.count();
    if (!scMod){
        scMod = new KKSCheckableModel (nsc, 1);
        //connect(scMod, SIGNAL(checkStateChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slotParamsChanged(const QModelIndex &, const QModelIndex &)));
    }
    else
    {
        int nr = scMod->rowCount();
        scMod->removeRows (0, nr);
        scMod->insertRows (0, nsc);
        if (scMod->columnCount() == 0)
            scMod->insertColumns (0, 1);
    }

    int i=0;
    for (QMap<int, QString>::const_iterator p=scList.constBegin();
            p != scList.constEnd();
            ++p)
    {
        QModelIndex scInd = scMod->index (i, 0);
        scMod->setData (scInd, p.key(), Qt::UserRole);
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

    if(m_hist)
        setupCheckedData(UI->lVScenarios, m_hist->getScenario());

}

void KKSHistWidgetEx::loadVariants (const QMap<int, QString>& varList)
{
    QAbstractItemModel * varModel = UI->lvVariants->model ();
    bool isSet (true);
    int nvr = varList.count();
    if (!varModel)
    {
        varModel = new KKSCheckableModel (nvr, 1);
        //connect(varModel, SIGNAL(checkStateChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slotParamsChanged(const QModelIndex &, const QModelIndex &)));
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

    int i=0;
    for (QMap<int, QString>::const_iterator p=varList.constBegin();
            p != varList.constEnd();
            ++p)
    {
        QModelIndex vIndex = varModel->index (i, 0);
        varModel->setData (vIndex, p.key(), Qt::UserRole);
        varModel->setData (vIndex, p.value(), Qt::DisplayRole);
        i++;
    }

    if (!isSet){
        UI->lvVariants->setModel (varModel);
        connect (varModel,
                 SIGNAL (dataChanged(const QModelIndex&, const QModelIndex&)),
                 this,
                 SLOT (variantSet(const QModelIndex&, const QModelIndex&))
                );
    }

    if(m_hist)
        setupCheckedData(UI->lvVariants, m_hist->getVariant());

}

void KKSHistWidgetEx::loadCategories (const QMap<int, QString>& cList)
{
    //UI->lwCategories->clear();
    QAbstractItemModel * catMod = UI->lvCategories->model();
    bool isSet (true);
    int ncr = cList.count();
    if (!catMod){
        catMod = new KKSCheckableModel (ncr, 1);
        //connect(catMod, SIGNAL(checkStateChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slotParamsChanged(const QModelIndex &, const QModelIndex &)));
        isSet = false;
    }
    else
    {
        int nr = catMod->rowCount();
        catMod->removeRows (0, nr);
        if (catMod->columnCount() == 0)
            catMod->insertColumns (0, 1);
    }

    int i=0;
    for (QMap<int, QString>::const_iterator p=cList.constBegin();
            p != cList.constEnd();
            ++p)
    {
        QModelIndex cIndex = catMod->index (i, 0);
        catMod->setData (cIndex, p.key(), Qt::UserRole);
        catMod->setData (cIndex, p.value(), Qt::DisplayRole);
        i++;
    }

    if (!isSet){
        UI->lvCategories->setModel (catMod);
        connect (catMod,
                 SIGNAL (dataChanged(const QModelIndex&, const QModelIndex&)),
                 this,
                 SLOT (catSet(const QModelIndex&, const QModelIndex&))
                );
    }

    if(m_hist)
        setupCheckedData(UI->lvCategories, m_hist->getCategories());
}

void KKSHistWidgetEx::loadIOList (const QMap<int, QString>& ioList)
{
    //UI->lwCategories->clear();
    QAbstractItemModel * ioMod = UI->lvIOs->model();
    bool isSet (true);
    int ncr = ioList.count();
    if (!ioMod){
        ioMod = new KKSCheckableModel (ncr, 1);
        //connect(ioMod, SIGNAL(checkStateChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slotParamsChanged(const QModelIndex &, const QModelIndex &)));
        isSet = false;
    }
    else
    {
        int nr = ioMod->rowCount();
        ioMod->removeRows (0, nr);
        if (ioMod->columnCount() == 0)
            ioMod->insertColumns (0, 1);
    }

    int i=0;
    for (QMap<int, QString>::const_iterator p=ioList.constBegin();
            p != ioList.constEnd();
            ++p)
    {
        QModelIndex ioIndex = ioMod->index (i, 0);
        ioMod->setData (ioIndex, p.key(), Qt::UserRole);
        ioMod->setData (ioIndex, p.value(), Qt::DisplayRole);
        i++;
    }

    if (!isSet){
        UI->lvIOs->setModel (ioMod);
        connect (ioMod,
                 SIGNAL (dataChanged(const QModelIndex&, const QModelIndex&)),
                 this,
                 SLOT (ioSet(const QModelIndex&, const QModelIndex&))
                );
    }

    if(m_hist)
        setupCheckedData(UI->lvIOs, m_hist->getIO());
}

void KKSHistWidgetEx::loadPartLows(const QMap<int, QString>& plList)
{
    //UI->lwCategories->clear();
    QAbstractItemModel * plMod = UI->lvLows->model();
    bool isSet (true);
    int ncr = plList.count();
    if (!plMod){
        plMod = new KKSCheckableModel (ncr, 1);
        //connect(plMod, SIGNAL(checkStateChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slotParamsChanged(const QModelIndex &, const QModelIndex &)));
        isSet = false;
    }
    else
    {
        int nr = plMod->rowCount();
        plMod->removeRows (0, nr);
        if (plMod->columnCount() == 0)
            plMod->insertColumns (0, 1);
    }

    int i=0;
    for (QMap<int, QString>::const_iterator p=plList.constBegin();
            p != plList.constEnd();
            ++p)
    {
        QModelIndex plIndex = plMod->index (i, 0);
        plMod->setData (plIndex, p.key(), Qt::UserRole);
        plMod->setData (plIndex, p.value(), Qt::DisplayRole);
        i++;
    }

    if (!isSet){
        UI->lvLows->setModel (plMod);
        connect (plMod,
                 SIGNAL (dataChanged(const QModelIndex&, const QModelIndex&)),
                 this,
                 SLOT (partLowSet(const QModelIndex&, const QModelIndex&))
                );
    }

    if(m_hist)
        setupCheckedData(UI->lvLows, m_hist->getPartLows());
}

void KKSHistWidgetEx::loadServices(const QMap<int, QString>& sList)
{
    //UI->lwCategories->clear();
    QAbstractItemModel * sMod = UI->lvServices->model();
    bool isSet (true);
    int ncr = sList.count();
    if (!sMod){
        sMod = new KKSCheckableModel (ncr, 1);
        //connect(sMod, SIGNAL(checkStateChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slotParamsChanged(const QModelIndex &, const QModelIndex &)));
        isSet = false;
    }
    else
    {
        int nr = sMod->rowCount();
        sMod->removeRows (0, nr);
        if (sMod->columnCount() == 0)
            sMod->insertColumns (0, 1);
    }

    int i=0;
    for (QMap<int, QString>::const_iterator p=sList.constBegin();
            p != sList.constEnd();
            ++p)
    {
        QModelIndex sIndex = sMod->index (i, 0);
        sMod->setData (sIndex, p.key(), Qt::UserRole);
        sMod->setData (sIndex, p.value(), Qt::DisplayRole);
        i++;
    }

    if (!isSet){
        UI->lvServices->setModel (sMod);
        connect (sMod,
                 SIGNAL (dataChanged(const QModelIndex&, const QModelIndex&)),
                 this,
                 SLOT (serviceSet(const QModelIndex&, const QModelIndex&))
                );
    }

    if(m_hist)
        setupCheckedData(UI->lvServices, m_hist->getServices());
}

void KKSHistWidgetEx::setupCheckedData(QListView * list, const QList<int> ids)
{
    if(!list)
        return;

    KKSCheckableModel * model = (KKSCheckableModel *)list->model();
    if(!model)
        return;

    int rowCount = model->rowCount();
    if(!rowCount)
        return;

    for(int i=0; i<rowCount; i++){
        QModelIndex index = model->index(i, 0);
        
        //bool checked = model->data(index, Qt::CheckStateRole).toBool();
        int id = model->data (index, Qt::UserRole).toInt();
        
        QVariant v(ids.contains (id));
        model->setData(index, v,  Qt::CheckStateRole);
        
        /*
        if (!ids.contains (id))
            model->setData(index, false,  Qt::CheckStateRole);
        else if (ids.contains(id))
            model->setData(index, true,  Qt::CheckStateRole);
        */
    }
}


void KKSHistWidgetEx::setHist (const KKSHistogram& shist)
{
    
    if (m_hist)
        m_hist->release ();
    
    m_hist = new KKSHistogram (shist);
    
    //QVariant v = QVariant::fromValue<KKSHistogram>(shist);
    
    UI->leFrom->setText (QString::number (m_hist->getXMin()));
    UI->leTo->setText (QString::number (m_hist->getXMax()));
    UI->leCount->setText (QString::number (m_hist->size()));

    
    QList<int> data = m_hist->getScenario();
    setupCheckedData(UI->lVScenarios, data);
    
    data = m_hist->getVariant();
    setupCheckedData(UI->lvVariants, data);

    data = m_hist->getCategories();
    setupCheckedData(UI->lvCategories, data);

    data = m_hist->getIO();
    setupCheckedData(UI->lvIOs, data);

    data = m_hist->getPartLows();
    setupCheckedData(UI->lvLows, data);

    data = m_hist->getServices();
    setupCheckedData(UI->lvServices, data);

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
    else{
        m_qwtHistogramWidget->init(m_hist);
    }
    
    //emit valueChanged (m_av->id(), m_isSystem, v);

}

void KKSHistWidgetEx::slotParamsChanged()
{
    if(!m_completed)
        return;

    m_paramsChanged = true;


    KKSValue v = getVal();//(hStr, KKSAttrType::atHistogram);
    QString hStr = v.valueForInsert();
    
    emit valueChanged (m_av->id(), m_isSystem, hStr);//v.valueVariant());
}

void KKSHistWidgetEx::saveHist (KKSValue & v)
{
    if(!m_av)
        return;

    if(m_hist->id() <= 0){
        qint64 id = 0;
        emit getIdForHistogramParams(m_av->attribute()->tableName(), &id);
        m_hist->setId(id);
    }

    v = getVal();
}


KKSValue KKSHistWidgetEx::getVal (void)
{
    double xmin = UI->leFrom->text().toDouble ();
    double xmax = UI->leTo->text().toDouble ();
    if (xmin >= xmax)
    {
        QMessageBox::warning (this, tr("Histogram"), tr("Minimum value has to be little than maximum"), QMessageBox::Ok);
        return KKSValue();
    }

    int num = UI->leCount->text().toInt ();
    
    if(!m_hist)
        m_hist = new KKSHistogram;

    m_hist->setRange (xmin, xmax);
    m_hist->setSize (num);

    QString hStr = m_hist->toString();
    qDebug () << __PRETTY_FUNCTION__ << hStr;
    KKSValue v (hStr, KKSAttrType::atHistogram);
    
    return v;
}

void KKSHistWidgetEx::scenarioSet (const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    int iRow0 = topLeft.row();
    int iRow1 = bottomRight.row();

    QList<int> scList = m_hist->getScenario();

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

    m_hist->setScenario(scList);
    slotParamsChanged();
}

void KKSHistWidgetEx::variantSet (const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    int iRow0 = topLeft.row();
    int iRow1 = bottomRight.row();

    QList<int> vList = m_hist->getVariant();
    
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

    m_hist->setVariant(vList);
    slotParamsChanged();
}

void KKSHistWidgetEx::catSet (const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    int iRow0 = topLeft.row();
    int iRow1 = bottomRight.row();

    QList<int> cList = m_hist->getCategories();
    
    QAbstractItemModel * vMod = qobject_cast<QAbstractItemModel *>(sender());
    for (int i=iRow0; i<=iRow1; i++)
    {
        QModelIndex wIndex = vMod->index (iRow0, 0, topLeft.parent());
        Qt::CheckState chState = (Qt::CheckState)(vMod->data(wIndex, Qt::CheckStateRole).toInt());
        int idV = vMod->data (wIndex, Qt::UserRole).toInt();
        if (chState == Qt::Checked && !cList.contains (idV))
            cList.append (idV);
        else if (chState == Qt::Unchecked && cList.contains(idV))
            cList.removeOne(idV);
    }

    m_hist->setCategories(cList);
    slotParamsChanged();
    
}

void KKSHistWidgetEx::ioSet (const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    int iRow0 = topLeft.row();
    int iRow1 = bottomRight.row();

    QList<int> ioList = m_hist->getIO();
    
    QAbstractItemModel * vMod = qobject_cast<QAbstractItemModel *>(sender());
    for (int i=iRow0; i<=iRow1; i++)
    {
        QModelIndex wIndex = vMod->index (iRow0, 0, topLeft.parent());
        Qt::CheckState chState = (Qt::CheckState)(vMod->data(wIndex, Qt::CheckStateRole).toInt());
        int idV = vMod->data (wIndex, Qt::UserRole).toInt();
        if (chState == Qt::Checked && !ioList.contains (idV))
            ioList.append (idV);
        else if (chState == Qt::Unchecked && ioList.contains(idV))
            ioList.removeOne(idV);
    }

    m_hist->setIO(ioList);
    slotParamsChanged();
}

void KKSHistWidgetEx::serviceSet (const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    int iRow0 = topLeft.row();
    int iRow1 = bottomRight.row();

    QList<int> sList = m_hist->getServices();
    
    QAbstractItemModel * vMod = qobject_cast<QAbstractItemModel *>(sender());
    for (int i=iRow0; i<=iRow1; i++)
    {
        QModelIndex wIndex = vMod->index (iRow0, 0, topLeft.parent());
        Qt::CheckState chState = (Qt::CheckState)(vMod->data(wIndex, Qt::CheckStateRole).toInt());
        int idV = vMod->data (wIndex, Qt::UserRole).toInt();
        if (chState == Qt::Checked && !sList.contains (idV))
            sList.append (idV);
        else if (chState == Qt::Unchecked && sList.contains(idV))
            sList.removeOne(idV);
    }

    m_hist->setServices(sList);
    slotParamsChanged();
}

void KKSHistWidgetEx::partLowSet (const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    int iRow0 = topLeft.row();
    int iRow1 = bottomRight.row();

    QList<int> plList = m_hist->getPartLows();
    
    QAbstractItemModel * vMod = qobject_cast<QAbstractItemModel *>(sender());
    for (int i=iRow0; i<=iRow1; i++)
    {
        QModelIndex wIndex = vMod->index (iRow0, 0, topLeft.parent());
        Qt::CheckState chState = (Qt::CheckState)(vMod->data(wIndex, Qt::CheckStateRole).toInt());
        int idV = vMod->data (wIndex, Qt::UserRole).toInt();
        if (chState == Qt::Checked && !plList.contains (idV))
            plList.append (idV);
        else if (chState == Qt::Unchecked && plList.contains(idV))
            plList.removeOne(idV);
    }

    m_hist->setPartLows(plList);
    slotParamsChanged();
}

void KKSHistWidgetEx::needToUpdateHistogramGraphic()
{

    QString tName = m_av->attribute()->tableName();
    emit getHistogramGraphic(*m_hist, tName);

    UI->leFrom->setText (QString::number (m_hist->getXMin()));
    UI->leTo->setText (QString::number (m_hist->getXMax()));
    UI->leCount->setText (QString::number (m_hist->size()));

    m_paramsChanged = false;

}

void KKSHistWidgetEx::completed(bool b)
{
    m_completed = b;
}
