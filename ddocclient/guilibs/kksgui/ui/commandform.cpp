#include <QDateTime>
#include <QDateTimeEdit>
#include <QIntValidator>
#include <QMessageBox>
#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QtDebug>
#include <QHeaderView>

#include "KKSCheckableModel.h"
#include <KKSCategory.h>
#include <KKSObjectExemplar.h>
#include <KKSObject.h>
#include <KKSAttribute.h>
#include <KKSAttrType.h>
#include <KKSAttrValue.h>
#include <KKSValue.h>
#include <KKSAttrType.h>

#include "commandform.h"
#include "ui_command_form.h"
#include "defines.h"

CommandForm :: CommandForm (const QMap<int, QString>& positions, const QMap<int, QString>& urgencyLevels, bool isCreate, QWidget *parent, Qt::WFlags f)
    : QDialog (parent, f),
    UI (new Ui::command_widget),
    commandCat (0),
    commandEIO (0)
{
    UI->setupUi (this);
    UI->tvExecutor->setSelectionMode (QAbstractItemView::ExtendedSelection);
    UI->tvExecutor->setSelectionBehavior (QAbstractItemView::SelectRows);
    UI->tvExecutor->header ()->setStretchLastSection (true);

    this->initExecutors (positions);
    for (QMap<int, QString>::const_iterator p = urgencyLevels.constBegin(); \
            p != urgencyLevels.constEnd(); \
            ++p)
        UI->cbUrgency->addItem (p.value(), p.key());

    UI->cbUrgency->setCurrentIndex (0);
    UI->lInsertDB->setVisible (!isCreate);
    UI->dTEInserted->setVisible (!isCreate);
    
    if (isCreate)
    {
        UI->leInputNumber->setText(tr("Will autogenerated on creation"));
        UI->leOutputNumber->setText(tr("Will autogenerated on creation"));
    }

    UI->dTEInserted->setDateTime (QDateTime::currentDateTime ());
    UI->dTEDeadline->setDateTime (QDateTime::currentDateTime ());
    UI->lStatus->setVisible (!isCreate);
    UI->lEStatus->setVisible (!isCreate);
    UI->lSender->setVisible (!isCreate);
    UI->lESender->setVisible (!isCreate);
    this->periodCheckStateChanged (Qt::Unchecked);
    QIntValidator * pVal = new QIntValidator (1, 24, this);
    UI->lEPeriod->setValidator (pVal);
    QStringList units = KKSAttrType::getIntervalUnits();
    for (int i=0; i<units.count(); i++)
        UI->cbUnits->addItem (units[i], i+1);
    UI->cbUnits->setCurrentIndex (5);
    UI->pbOk->setVisible (isCreate);
    UI->lReceiveDateTime->setVisible (!isCreate);
    UI->dTReceiveEdit->setVisible (!isCreate);
    UI->lAcceptDateTime->setVisible (!isCreate);
    UI->dTAcceptEdit->setVisible (!isCreate);

    connect (UI->tbCategory, SIGNAL (clicked()), this, SLOT (loadCategory()) );
    connect (UI->tbAddExecutor, SIGNAL (clicked()), this, SLOT (loadExecutor()) );    connect (UI->tbController, SIGNAL (clicked()), this, SLOT (loadController()) );
    connect (UI->cbPeriod, SIGNAL (stateChanged (int)), this, SLOT (periodCheckStateChanged (int)) );
    connect (UI->dTEDeadline, SIGNAL (dateTimeChanged (const QDateTime &)), this, SLOT (setDeadLine(const QDateTime &)) );
    connect (UI->lEPeriod, SIGNAL (textEdited (const QString&)), this, SLOT (periodTimeChanged (const QString&)) );
    connect (UI->cbUnits, SIGNAL (activated(int)), this, SLOT (periodUnitChanged (int)) );
    connect (UI->tECommandBody, SIGNAL (textChanged()), this, SLOT (setCommandMess()) );
    connect (UI->cbUrgency, SIGNAL (activated(int)), this, SLOT (urgencyChanged (int)) );
    connect (UI->tbDocument, SIGNAL (clicked()), this, SLOT (loadDocument()) );
    connect (UI->tbAddMailList, SIGNAL (clicked()), this, SLOT (addMailList()) );
    connect (UI->tbSetMailList, SIGNAL (clicked()), this, SLOT (setMailList()) );
    connect (UI->tbClearMailList, SIGNAL (clicked()), this, SLOT (clearMailList()) );
    connect (UI->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );

    if (isCreate)
        connect (UI->pbOk, SIGNAL (clicked()), this, SLOT (constructCommand()) );
    else
        UI->pbCancel->setText (tr("&Close"));
}

CommandForm :: ~CommandForm (void)
{
    if (commandCat)
        commandCat->release ();
    if (commandEIO)
        commandEIO->release ();
    attrVals.clear ();
    delete UI;
}

KKSCategory * CommandForm :: getCategory (void) const
{
    return commandCat;
}

void CommandForm :: setCategory (KKSCategory * cat)
{
    if (commandCat)
        commandCat->release ();

    commandCat = cat;

    if (commandCat)
    {
        commandCat->addRef ();
        UI->lECategory->setText (commandCat->name ());
    }
    else
        UI->lECategory->setText (QString());
}

KKSObjectExemplar * CommandForm :: getCommand (void) const
{
    return commandEIO;
}

void CommandForm :: setCommand (KKSObjectExemplar * ceio)
{
    if (commandEIO)
        commandEIO->release ();

    commandEIO = ceio;

    if (commandEIO)
    {
        commandEIO->addRef ();
        attrVals = commandEIO->attrValues();
    }
    else
        attrVals.clear ();
}

void CommandForm :: loadCategory (void)
{
    QString tableName ("io_categories");
    int attrId (ATTR_ID_IO_CATEGORY);

    emit loadReference (tableName, UI->lECategory, attrId);
}

void CommandForm :: loadController (void)
{
    QString tableName ("position");
    int attrId(ATTR_ID_DL_TO);

    emit loadReference (tableName, UI->lEController, attrId);
}

void CommandForm :: urgencyChanged (int index)
{
    //QString tableName ("urgency_levels");
    KKSValue val (QString::number (UI->cbUrgency->itemData (index).toInt()), KKSAttrType::atList);
    commandEIO->attrValue (ATTR_ID_URGENCY_LEVEL)->setValue (val);
}

void CommandForm :: loadDocument (void)
{
    QString tableName ("io_objects");
    int attrId (ATTR_ID_IO_OBJECT_ATT);

    emit loadReference (tableName, UI->lEDocument, attrId);
}

void CommandForm :: setAttachment (QString attName)
{
    UI->lEDocument->setText (attName);
}

void CommandForm :: periodCheckStateChanged (int state)
{
    bool isEnable (state == Qt::Checked);
    UI->lPeriod->setEnabled (isEnable);
    UI->lEPeriod->setEnabled (isEnable);
    UI->cbUnits->setEnabled (isEnable);
    if (!commandEIO)
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
        commandEIO->attrValue (ATTR_EXEC_PERIOD)->setValue (pval);
//        attrVals[11]->setValue (pval);//period
    }
}

void CommandForm :: constructCommand (void)
{
    if (!commandEIO)
        return;

    if (UI->lECategory->text().isEmpty())
    {
        qWarning() << tr ("Set IO category");
        QMessageBox::warning (this, tr ("Command"), tr ("Set IO category"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    if (UI->lEController->text().isEmpty())
    {
        qWarning() << tr ("Set controller");
        QMessageBox::warning (this, tr ("Command"), tr ("Set controller"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    if (getExecutors().isEmpty())
    {
        qWarning() << tr ("Set executor");
        QMessageBox::warning (this, tr ("Command"), tr ("Set executor"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    if (UI->tECommandBody->toPlainText().isEmpty())
    {
        qWarning() << tr ("Set command text");
        QMessageBox::warning (this, tr ("Command"), tr ("Set command text"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
//    commandEIO->setAttrValues (attrVals);
    
    this->accept ();
}

QDateTime CommandForm :: getInsertDateTime (void) const
{
    return UI->dTEInserted->dateTime ();
}

void CommandForm :: setInsertDateTime (const QDateTime& dt)
{
    UI->dTEInserted->setDateTime (dt);
}

KKSAttrValue * CommandForm :: getController (void) const
{
    if (!commandEIO || !commandEIO->attrValue (ATTR_ID_DL_TO))
        return 0;

    return commandEIO->attrValue (ATTR_ID_DL_TO);
}

void CommandForm :: setController (const QString& dl_name)
{
    UI->lEController->setText (dl_name);
}
/*
KKSAttrValue * CommandForm :: getExecutor (void) const
{
    if (!commandEIO || !commandEIO->attrValue ("id_dl_executor"))
        return 0;

    return commandEIO->attrValue ("id_dl_executor");
}

void CommandForm :: setExecutor (const QString& dl_executor)
{
//    UI->lEExecutor->setText (dl_executor);
}
*/

KKSAttrValue * CommandForm :: getInputNumber(void) const
{
    if (!commandEIO || !commandEIO->attrValue (ATTR_INPUT_NUMBER))
        return 0;

    return commandEIO->attrValue (ATTR_INPUT_NUMBER);
}

void CommandForm :: setInputNumber(const QString& inputNumber)
{
    UI->leInputNumber->setText (inputNumber);
}

KKSAttrValue * CommandForm :: getOutputNumber(void) const
{
    if (!commandEIO || !commandEIO->attrValue (ATTR_OUTPUT_NUMBER))
        return 0;

    return commandEIO->attrValue (ATTR_OUTPUT_NUMBER);
}

void CommandForm :: setOutputNumber(const QString& outputNumber)
{
    UI->leOutputNumber->setText (outputNumber);
}

QDateTime CommandForm :: getDeadline (void) const
{
    return UI->dTEDeadline->dateTime ();
}

void CommandForm :: setDeadLineT (const QDateTime& dt)
{
    UI->dTEDeadline->setDateTime (dt);
}

QString CommandForm :: getPeriod (void) const
{
    if (UI->cbPeriod->checkState () != Qt::Checked || UI->cbUnits->currentIndex () < 0)
        return QString ();

    QString res = QString ("%1 %2").arg (UI->lEPeriod->text().toInt()).arg (UI->cbUnits->currentText());
    return res;
}

void CommandForm :: setPeriod (int value, QString unitText)
{
    UI->cbPeriod->setCheckState (Qt::Checked);
    int index = UI->cbUnits->findText (unitText, Qt::MatchExactly);
    UI->lEPeriod->setText (QString::number (value));
    UI->cbUnits->setCurrentIndex (index);
}

KKSAttrValue * CommandForm :: getStatus (void) const
{
     if (!commandEIO || !commandEIO->attrValue (ATTR_ID_JR_STATE))
        return 0;

    return commandEIO->attrValue (ATTR_ID_JR_STATE);
}

void CommandForm :: setStatus (QString status)
{
    UI->lEStatus->setText (status);
}

KKSAttrValue * CommandForm :: getSender (void) const
{
    if (!commandEIO || !commandEIO->attrValue (ATTR_ID_DL_FROM))
        return 0;

    return commandEIO->attrValue (ATTR_ID_DL_FROM);
}

void CommandForm :: setSender (QString sender_name)
{
    UI->lESender->setText (sender_name);
}

QString CommandForm :: getCommandMessage (void) const
{
    return UI->tECommandBody->toPlainText ();
}

void CommandForm :: setCommandMessage (QString text)
{
    UI->tECommandBody->setPlainText (text);
}

const KKSList<KKSAttrValue *> & CommandForm :: attrValues() const
{
    return attrVals;
}

KKSList<KKSAttrValue *>& CommandForm :: attrValues (void)
{
    return attrVals;
}

void CommandForm :: setValues (const KKSList<KKSAttrValue *>& vals)
{
    attrVals = vals;
}

void CommandForm :: setDeadLine (const QDateTime& datetime)
{
    KKSValue dtVal (QVariant (datetime).toString(), KKSAttrType::atDateTime);
    commandEIO->attrValue (ATTR_EXEC_DATETIME)->setValue (dtVal);
    //attrVals[9]->setValue (dtVal);//exec_datetime
}

void CommandForm :: setCommandMess (void)
{
    KKSValue body (getCommandMessage(), KKSAttrType::atText);
    commandEIO->attrValue (ATTR_MESSAGE_BODY)->setValue (body);
//    attrVals[11]->setValue (body);//message_body
}

void CommandForm :: periodTimeChanged (const QString & text)
{
    int num = text.toInt();
    KKSValue pval (QString("%1 %2").arg (num).arg (UI->cbUnits->itemData (UI->cbUnits->currentIndex()).toInt()), KKSAttrType::atInterval);
    commandEIO->attrValue (ATTR_EXEC_PERIOD)->setValue (pval);
//    attrVals[10]->setValue (pval);//exec_period
}

void CommandForm :: periodUnitChanged (int index)
{
    int num = UI->lEPeriod->text().toInt ();
    KKSValue pval (QString("%1 %2").arg (num).arg (UI->cbUnits->itemData (index).toInt()), KKSAttrType::atInterval);
    commandEIO->attrValue (ATTR_EXEC_PERIOD)->setValue (pval);
//    attrVals[10]->setValue (pval);//exec_period
}

void CommandForm :: initExecutors (const QMap<int, QString>& pos)
{
    QAbstractItemModel *exModel = new KKSCheckableModel (pos.count(), 1);
    exModel->setHeaderData (0, Qt::Horizontal, tr ("Executors"), Qt::DisplayRole);
    int i=0;
    for (QMap<int, QString>::const_iterator pe = pos.constBegin(); pe != pos.constEnd(); ++pe)
    {
        QModelIndex wIndex = exModel->index (i, 0);
        exModel->setData (wIndex, pe.key(), Qt::UserRole);
        exModel->setData (wIndex, pe.value(), Qt::DisplayRole);
        exModel->setData (wIndex, false, Qt::CheckStateRole);
        i++;
    }
    UI->tvExecutor->setModel (exModel);
}

QList<int> CommandForm :: getExecutors (void) const
{
    QAbstractItemModel * exModel = UI->tvExecutor->model ();
    if (!exModel)
        return QList<int>();

    QList<int> results;
    for (int i=0; i<exModel->rowCount(); i++)
    {
        QModelIndex wIndex = exModel->index (i, 0);//selIndexes[i];
        if (wIndex.data (Qt::CheckStateRole).toBool())
            results.append (wIndex.data (Qt::UserRole).toInt());
    }

    return results;
}

void CommandForm :: selectExecutor (int idEx)
{
    QAbstractItemModel * exModel = UI->tvExecutor->model ();
    if (!exModel)
        return;

    bool isSelected (false);
    for (int i=0; i<exModel->rowCount() && !isSelected; i++)
    {
        QModelIndex wIndex = exModel->index (i, 0);
        if (wIndex.data (Qt::UserRole).toInt() == idEx)
        {
            exModel->setData (wIndex, true, Qt::CheckStateRole);
            isSelected = true;
        }
    }
}

void CommandForm :: setUrgency (int idUrgency)
{
    int index = UI->cbUrgency->findData (idUrgency);
    if (index >= 0)
        UI->cbUrgency->setCurrentIndex (index);
}

void CommandForm :: addMailList (void)
{
    QAbstractItemModel * exModel = UI->tvExecutor->model ();
    if (!exModel)
    {
        qWarning() << tr ("Set model into executors");
        QMessageBox::warning (this, tr ("Command form"), tr ("Set model into executors"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    emit loadExecutors (exModel);
}

void CommandForm :: setMailList (void)
{
    this->clearMailList ();
    this->addMailList ();
}

void CommandForm :: clearMailList (void)
{
    this->clearExecutors();
}

void CommandForm :: clearExecutors (void)
{
    QAbstractItemModel * exModel = UI->tvExecutor->model ();
    if (exModel && exModel->rowCount() > 0)
    {
        int nr = exModel->rowCount();
        exModel->removeRows (0, nr);
    }
}

void CommandForm :: setExecutors (const QMap<int, QString>& exList)
{
    this->clearExecutors();
    this->initExecutors (exList);
}

void CommandForm :: setReceiveDateTime (const QDateTime& dtRecv)
{
    if(!dtRecv.isValid()){
        UI->dTReceiveEdit->setVisible(false);
        UI->lReceiveDateTime->setVisible(false);
        return;
    }

    UI->dTReceiveEdit->setDateTime (dtRecv);
}

void CommandForm :: setAcceptDateTime (const QDateTime& dtAcc)
{
    if(!dtAcc.isValid()){
        UI->dTAcceptEdit->setVisible(false);
        UI->lAcceptDateTime->setVisible(false);
        return;
    }
    UI->dTAcceptEdit->setDateTime (dtAcc);
}

void CommandForm :: loadExecutor (void)
{
    QString tableName ("position");
    //QString attrCode ("id_dl_executor");
    QAbstractItemModel * exModel = UI->tvExecutor->model ();
    if (!exModel)
    {
        qWarning() << tr("Set model into executors");
        QMessageBox::warning (this, tr ("Command form"), tr ("Set model into executors"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    emit addExecutor (tableName, exModel, ATTR_ID_DL_EXECUTOR);
}