#include <QDateTime>
#include <QMessageBox>
#include <QtDebug>

#include <KKSObject.h>
#include <KKSObjectExemplar.h>
#include <KKSAttrValue.h>
#include <KKSCategory.h>
#include <KKSAttrType.h>

#include "periodicreportform.h"
#include "ui_periodic_report_form.h"
#include "defines.h"

PeriodicReportForm :: PeriodicReportForm (bool isCreate, QWidget * parent, Qt::WFlags f)
    : QDialog (parent, f),
    UI (new Ui::periodic_report_form),
    periodicEIO (0),
    attrVals (KKSList<KKSAttrValue *>())
{
    UI->setupUi (this);

    UI->lInsertDB->setVisible (!isCreate);
    UI->dTEInserted->setVisible (!isCreate);
    UI->cbArchive->setVisible (!isCreate);
    UI->lState->setVisible (!isCreate);
    UI->lEState->setVisible (!isCreate);
    UI->tbState->setVisible (!isCreate);
    UI->dTEDeadline->setDateTime (QDateTime::currentDateTime ());
    this->periodCheckStateChanged (Qt::Unchecked);
    QIntValidator * pVal = new QIntValidator (1, 24, this);
    UI->lEPeriod->setValidator (pVal);
    QStringList units = KKSAttrType::getIntervalUnits();
    for (int i=0; i<units.count(); i++)
        UI->cbUnits->addItem (units[i], i+1);
    UI->cbUnits->setCurrentIndex (5);

    connect (UI->tbCategory, SIGNAL (clicked()), this, SLOT (loadCategory()) );
    connect (UI->tbAddressee, SIGNAL (clicked()), this, SLOT (loadAddressee()) );
    connect (UI->tbExecutor, SIGNAL (clicked()), this, SLOT (loadExecutor()) );
    connect (UI->tbController, SIGNAL (clicked()), this, SLOT (loadController()) );
    connect (UI->cbPeriod, SIGNAL (stateChanged (int)), this, SLOT (periodCheckStateChanged (int)) );
    connect (UI->dTEDeadline, SIGNAL (dateTimeChanged (const QDateTime &)), this, SLOT (setDeadLine(const QDateTime &)) );
    connect (UI->lEPeriod, SIGNAL (textEdited (const QString&)), this, SLOT (periodTimeChanged (const QString&)) );
    connect (UI->cbUnits, SIGNAL (activated(int)), this, SLOT (periodUnitChanged (int)) );
    connect (UI->tEReason, SIGNAL (textChanged()), this, SLOT (setReason()) );
    connect (UI->cbArchive, SIGNAL (stateChanged (int)), this, SLOT (archiveStateChanged (int)) );
    connect (UI->tbState, SIGNAL (clicked()), this, SLOT (loadState()) );

    connect (UI->pbOk, SIGNAL (clicked()), this, SLOT (constructReport()) );
    connect (UI->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

PeriodicReportForm :: ~PeriodicReportForm (void)
{
    if (periodicEIO)
        periodicEIO->release ();
    delete UI;
}

KKSObjectExemplar * PeriodicReportForm :: getReport (void) const
{
    return periodicEIO;
}

void PeriodicReportForm :: setReport (KKSObjectExemplar * peio)
{
    if (periodicEIO)
    {
        periodicEIO->release ();
        attrVals.clear ();
    }

    periodicEIO = peio;

    if (periodicEIO)
    {
        periodicEIO->addRef ();
        attrVals = periodicEIO->attrValues();
        //KKSCategory *tc = periodicEIO->io()->category()->tableCategory();
        
        /*for (KKSMap<int, KKSCategoryAttr*>::const_iterator pa=tc->attributes().constBegin(); \
                pa != tc->attributes().constEnd(); \
                pa++)
            qDebug () << __PRETTY_FUNCTION__ << pa.value ()->id () << pa.value()->code ();
        */
    }
    else
        attrVals.clear ();
}

const KKSList<KKSAttrValue *>& PeriodicReportForm :: attrValues() const
{
    return attrVals;
}

KKSList<KKSAttrValue *>& PeriodicReportForm :: attrValues (void)
{
    return attrVals;
}

void PeriodicReportForm :: setValues (const KKSList<KKSAttrValue *>& vals)
{
    attrVals = vals;
}

void PeriodicReportForm :: constructReport (void)
{
    if (!periodicEIO)
        return;

    if (UI->lECategory->text().isEmpty())
    {
        QMessageBox::warning (this, tr ("Periodic report"), tr ("Set IO category"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    if (UI->lEAddressee->text().isEmpty())
    {
        QMessageBox::warning (this, tr ("Periodic report"), tr ("Set Addresse"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    if (UI->lEController->text().isEmpty())
    {
        QMessageBox::warning (this, tr ("Periodic report"), tr ("Set controller"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    if (UI->lEExecutor->text().isEmpty())
    {
        QMessageBox::warning (this, tr ("Periodic report"), tr ("Set executor"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    if (UI->tEReason->toPlainText().isEmpty())
    {
        QMessageBox::warning (this, tr ("Periodic report"), tr ("Set report text"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    periodicEIO->setAttrValues (attrVals);
    this->accept ();
}

void PeriodicReportForm :: loadCategory (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QString tableName ("io_categories");
    int attrId (ATTR_ID_IO_CATEGORY);
    emit loadReference (tableName, UI->lECategory, attrId);
}

void PeriodicReportForm :: loadAddressee (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QString tableName ("position");
    int attrId (ATTR_ID_DL_TO);
    emit loadReference (tableName, UI->lEAddressee, attrId);
}

void PeriodicReportForm :: loadController (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QString tableName ("position");
    int attrId (ATTR_ID_DL_CONTROLLER);
    emit loadReference (tableName, UI->lEController, attrId);
}

void PeriodicReportForm :: loadExecutor (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QString tableName ("position");
    int attrId (ATTR_ID_DL_EXECUTOR);
    emit loadReference (tableName, UI->lEExecutor, attrId);
}

void PeriodicReportForm :: periodCheckStateChanged (int state)
{
    bool isEnable (state == Qt::Checked);
    UI->lPeriod->setEnabled (isEnable);
    UI->lEPeriod->setEnabled (isEnable);
    UI->cbUnits->setEnabled (isEnable);
    if (!periodicEIO)
        return;

    if (isEnable)
    {
        this->periodTimeChanged (UI->lEPeriod->text());
        this->periodUnitChanged (UI->cbUnits->currentIndex());
    }
    else
    {
        QString v_str = QString();
        int type = (KKSAttrType::atInterval);
        KKSValue pval (v_str, type);//KKSAttrType::atInterval);
        attrVals[7]->setValue (pval);
    }
}

void PeriodicReportForm :: setDeadLine (const QDateTime& datetime)
{
    KKSValue dtVal (QVariant (datetime).toString(), KKSAttrType::atDateTime);
    attrVals[6]->setValue (dtVal);
}

void PeriodicReportForm :: periodTimeChanged (const QString & text)
{
    int num = text.toInt();
    KKSValue pval (QString("%1 %2").arg (num).arg (UI->cbUnits->itemData (UI->cbUnits->currentIndex()).toInt()), KKSAttrType::atInterval);
    attrVals[7]->setValue (pval);
}

void PeriodicReportForm :: periodUnitChanged (int index)
{
    int num = UI->lEPeriod->text().toInt ();
    KKSValue pval (QString("%1 %2").arg (num).arg (UI->cbUnits->itemData (index).toInt()), KKSAttrType::atInterval);
    attrVals[7]->setValue (pval);
}

void PeriodicReportForm :: setReason (void)
{
    KKSValue body (UI->tEReason->toPlainText(), KKSAttrType::atText);
    attrVals[8]->setValue (body);
}

void PeriodicReportForm :: archiveStateChanged (int state)
{
    bool isArchive (state == Qt::Checked);
    QString isArchStr = (isArchive ? QString("true") : QString("false"));
    KKSValue pval (isArchStr, KKSAttrType::atBool);
    attrVals[9]->setValue (pval);
}

void PeriodicReportForm :: loadState (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QString tableName ("jr_states");
    int attrId(ATTR_ID_JR_STATE);
    emit loadReference (tableName, UI->lEExecutor, attrId);
}
