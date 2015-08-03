#include <QTreeView>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QItemSelection>
#include <QAction>
#include <QVariant>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMainWindow>
#include <QMessageBox>
#include <QFont>
#include <QtDebug>

#include "cmdjournalform.h"
#include "cmdjournalsettingsform.h"

#include "msgjournalform.h"
#include "msgjournalsettingsform.h"
#include "kksmsgjournallv.h"
#include "kksmsgjournalitem.h"
#include <KKSFile.h>
#include "KKSFilesForm.h"
#include <KKSValue.h>
#include "KKSItemDelegate.h"
#include <defines.h>

#include "KKSJournalWidget.h"
#include "kkscmdjournalitem.h"

KKSJournalWidget :: KKSJournalWidget (
                                      KKSCmdJSettings * cmdSettings,
                                      KKSMsgJSettings * msgSettings,
                                      const KKSMap<int, KKSEIOData*> & cmdData,
                                      const KKSMap<int, KKSEIOData*> & msgData,
                                      int idDl,
                                      QWidget *parent,
                                      Qt::WindowFlags flags)
    : QDockWidget (tr("Command/message journal"), parent, flags),
      m_cmdData (cmdData),
      m_msgData (msgData),
      m_idDl (idDl),
      tvJournal (new QTreeView (this)),
      m_cmdSettings (cmdSettings),
      m_msgSettings (msgSettings)
{
    this->setWidget (tvJournal);
    tvJournal->setContextMenuPolicy (Qt::DefaultContextMenu);
    QItemDelegate * itemDeleg = new KKSItemDelegate (this);
    tvJournal->setItemDelegate (itemDeleg);

    this->initModel ();
    this->initCmd ();
    this->initMsg();

    this->initSysMenu ();
    this->initMsgWidgetMenu ();
    this->initCmdWidgetMenu ();
}

KKSJournalWidget :: ~KKSJournalWidget (void)
{

}

void KKSJournalWidget :: initModel (void)
{
    QAbstractItemModel * cmdMsgModel = new QStandardItemModel (2, 1);
    cmdMsgModel->setHeaderData (0, Qt::Horizontal, tr ("Command/message journal"), Qt::DisplayRole);
    QFont tvFont = tvJournal->font ();
    tvFont.setBold (true);
    QModelIndex cmdIndex = cmdMsgModel->index (0, 0);
    cmdMsgModel->setData (cmdIndex, tvFont, Qt::FontRole);
    cmdMsgModel->setData (cmdIndex, tr("Commands"), Qt::DisplayRole);
    cmdMsgModel->setData (cmdIndex, 0, Qt::UserRole+USER_ENTITY);
    QModelIndex msgIndex = cmdMsgModel->index (1, 0);
    cmdMsgModel->setData (msgIndex, tvFont, Qt::FontRole);
    cmdMsgModel->setData (msgIndex, tr("Messages"), Qt::DisplayRole);
    cmdMsgModel->setData (msgIndex, 1, Qt::UserRole+USER_ENTITY);

    tvJournal->setModel (cmdMsgModel);
}

void KKSJournalWidget :: initCmd (void)
{
    int oldId = -1;
    if (!m_cmdData.isEmpty())
    {
        QItemSelectionModel * selModel = tvJournal->selectionModel ();
        QModelIndex wIndex = selModel->currentIndex();
        wIndex = wIndex.sibling (wIndex.row(), 0);
        QVariant cVal = wIndex.data (Qt::UserRole);
        int id_entity = wIndex.data (Qt::UserRole+USER_ENTITY).toInt();
        if (id_entity == 0 && cVal.canConvert<KKSCommand>())
        {
            KKSCommand cmd = cVal.value<KKSCommand>();
            if(cmd.id() > 0)
                oldId = cmd.id();
        }
    }

    //m_cmdJournalLv->clear();

    QAbstractItemModel * jModel = tvJournal->model ();
    QModelIndex cmdIndex = jModel->index (0, 0);
    //jModel->removeRows (0, jModel->rowCount(cmdIndex), cmdIndex);
    if (!jModel->rowCount (cmdIndex))
        jModel->insertRows (0, 3, cmdIndex);
    if (!jModel->columnCount(cmdIndex))
        jModel->insertColumns (0, 1, cmdIndex);
    int row = 0;
    jModel->setData (cmdIndex, 0, Qt::UserRole+2);
    QStringList commandTypes;
    commandTypes << tr ("Input command(s) (0)") << tr ("Command(s) under control (0)") << tr ("Output command(s) (0)");
    for (int i=0; i<3; i++)
    {
        QModelIndex wIndex = jModel->index (i, 0, cmdIndex);
        jModel->setData (wIndex, commandTypes[i], Qt::DisplayRole);
        jModel->setData (wIndex, 0, Qt::UserRole+2);
    }

    QStringList commandStates;
    commandStates << tr ("issued (0)") << tr ("accepted (0)") << tr ("performed (0)") << tr ("expired (0)") << tr ("cancelled (0)");
    for (int i=0; i<3; i++)
    {
        QModelIndex wIndex = jModel->index (i, 0, cmdIndex);
        if (jModel->rowCount (wIndex) == 0)
            jModel->insertRows (0, 5, wIndex);
        if (!jModel->columnCount (wIndex))
            jModel->insertColumns (0, 1, wIndex);
        for (int ii=0; ii<commandStates.count(); ii++)
        {
            QModelIndex wcIndex = jModel->index (ii, 0, wIndex);
            jModel->setData (wcIndex, commandStates[ii], Qt::DisplayRole);
            jModel->setData (wcIndex, 0, Qt::UserRole+2);
            if (jModel->rowCount (wcIndex) > 0)
                jModel->removeRows (0, jModel->rowCount(wcIndex), wcIndex);
        }
    }
    KKSMap<int, KKSEIOData*>::const_iterator pd;
    if(m_cmdData.isEmpty())
        return;

    for (pd = m_cmdData.constBegin(); pd != m_cmdData.constEnd(); pd++)
    {
        KKSEIOData * record = pd.value();
        if(!record)
            continue;

        QModelIndex parentIndex = cmdIndex;
        int nVal = parentIndex.data (Qt::UserRole+2).toInt();
        jModel->setData (parentIndex, ++nVal, Qt::UserRole+2);

        KKSCommand cmd;
        // є п/п
        cmd.setId(record->fieldValue("id").toInt());
        //message_body
        cmd.setMessageBody(record->fieldValue("message_body"));
        //category_name
        cmd.setCategoryName(record->fieldValue("category_name"));
        //отправ
        cmd.setDlFrom(record->fieldValue("dl_from_name"));
        //получатель результата
        cmd.setDlTo(record->fieldValue("dl_to_name"));
        //ответственный исполнитель
        cmd.setDlExecutor(record->fieldValue("dl_executor_name") + " (" + record->fieldValue("executor_name") + ")");
        //planned_exec
        int cmdTypeEx = record->fieldValue("cmd_type").toInt();
        int idDlExecutor = record->fieldValue("id_dl_executor").toInt();
        int idDlFrom = record->fieldValue("id_dl_from").toInt();

        KKSCommand::CmdTypeEx typeEx;
        Q_UNUSED (typeEx);
        cmd.setCmdTypeEx((KKSCommand::CmdTypeEx)cmdTypeEx);

        KKSCommand::CmdType type;
        if(m_idDl == idDlExecutor)
        {
            type = KKSCommand::ctIn;
            parentIndex = jModel->index (0, 0, parentIndex);
            int nVal = parentIndex.data (Qt::UserRole+2).toInt();
            jModel->setData (parentIndex, ++nVal, Qt::UserRole+2);
        }
        else if(m_idDl == idDlFrom)
        {
            type = KKSCommand::ctOut;
            parentIndex = jModel->index (2, 0, parentIndex);
            int nVal = parentIndex.data (Qt::UserRole+2).toInt();
            jModel->setData (parentIndex, ++nVal, Qt::UserRole+2);
        }
        else if(cmdTypeEx == KKSCommand::jtIn)
        {
            type = KKSCommand::ctAllIn;
            parentIndex = jModel->index (0, 0, parentIndex);
            int nVal = parentIndex.data (Qt::UserRole+2).toInt();
            jModel->setData (parentIndex, ++nVal, Qt::UserRole+2);
        }
        else if(cmdTypeEx == KKSCommand::cteControl)
        {
            type = KKSCommand::ctOut;
            parentIndex = jModel->index (1, 0, parentIndex);
            int nVal = parentIndex.data (Qt::UserRole+2).toInt();
            jModel->setData (parentIndex, ++nVal, Qt::UserRole+2);
        }
        else
        {
            type = KKSCommand::ctAllOut;
            parentIndex = jModel->index (0, 0, parentIndex);
            int nVal = parentIndex.data (Qt::UserRole+2).toInt();
            jModel->setData (parentIndex, ++nVal, Qt::UserRole+2);
        }

        cmd.setCmdType(type);
        QDateTime dt = KKSValue::stringToDateTime(record->fieldValue("exec_datetime"));//planned_exec
        cmd.setExecDateTime(dt);
        //insert_datetime
        dt = KKSValue::stringToDateTime(record->fieldValue("insert_time"));
        cmd.setInsertTime(dt);
        //periodical
        cmd.setExecPeriod(record->fieldValue("exec_period"));
        //receive_datetime
        dt = KKSValue::stringToDateTime(record->fieldValue("receive_datetime"));
        cmd.setReceiveDateTime(dt);
        //accepted_datetime
        dt = KKSValue::stringToDateTime(record->fieldValue("accepted_datetime"));
        cmd.setAcceptedDateTime(dt);
        //real_exec
        dt = KKSValue::stringToDateTime(record->fieldValue("real_exec"));
        cmd.setRealExec(dt);

        cmd.setCategoryId(record->fieldValue("id_io_category").toInt());
        cmd.setIoObjectId(record->fieldValue("id_io_object_att").toInt());
        cmd.setCmdState((KKSCommand::CmdState)record->fieldValue("id_jr_state").toInt());
        cmd.setCmdStateName(record->fieldValue("jr_state_name"));
        switch (cmd.cmdState())
        {
            case KKSCommand::csPlanned: parentIndex = jModel->index (0, 0, parentIndex); break;
            case KKSCommand::csOuted: parentIndex = jModel->index (0, 0, parentIndex); break;
            case KKSCommand::csExecuting: case KKSCommand::csSavedAsDraft: parentIndex = jModel->index (1, 0, parentIndex); break;
            case KKSCommand::csExecuted: parentIndex = jModel->index (2, 0, parentIndex); break;
            case KKSCommand::csTimeElapsed: parentIndex = jModel->index (3, 0, parentIndex); break;
        }
        int ncmd = parentIndex.data (Qt::UserRole+2).toInt();
        jModel->setData (parentIndex, ++ncmd, Qt::UserRole+2);

        bool b = false;
        if(record->fieldValue("is_archived") == QObject::tr("Yes"))
            b = true;
        cmd.setIsArchived(b);

        //read_datetime_e
        dt = KKSValue::stringToDateTime(record->fieldValue("read_datetime_e"));
        cmd.setReadDateTimeE(dt);

        //read_datetime_c
        dt = KKSValue::stringToDateTime(record->fieldValue("read_datetime_c"));
        cmd.setReadDateTimeC(dt);

//        m_cmdJournalLv->addCommand(cmd);
        QModelIndex cmdInd = QModelIndex ();
        for (int ii=0; ii<jModel->rowCount(parentIndex) && !cmdInd.isValid(); ii++)
        {
            QModelIndex cIndex = jModel->index (ii, 0, parentIndex);
            if (cmd.id() == cIndex.data (Qt::UserRole+1).toInt())
                cmdInd = cIndex;
        }

        if (!cmdInd.isValid())
        {
            bool is_inserted = jModel->insertRows (jModel->rowCount(parentIndex), 1, parentIndex);
            if (!is_inserted)
                continue;
            if (jModel->columnCount (parentIndex) == 0)
                jModel->insertColumns (0, 1, parentIndex);
            cmdInd = jModel->index (jModel->rowCount(parentIndex)-1, 0, parentIndex);
            //int ncount = parentIndex.data (Qt::UserRole+2).toInt ();
            //ncount++;
            //jModel->setData (parentIndex, ncount, Qt::UserRole+2);
        }
        if (cmdInd.parent().row() <= 1)
        {
            QColor cCol;
            if(cmd.cmdState() == KKSCommand::csExecuted)
            {
                cCol = Qt::white;
            }
            else
            {
                double percent = cmd.timePercentEstimated();
                if( percent >= 90.0 ){
                    cCol = Qt::white;
                }
                else if(percent >= 50.0 && percent < 90.0){
                    cCol = Qt::darkGreen;
                }
                else if(percent >= 20.0 && percent < 50.0){
                    cCol = Qt::darkYellow;
                }
                else{
                    cCol = Qt::darkRed;
                }
            }
            QBrush rbr (cCol);
            jModel->setData (cmdInd, rbr, Qt::BackgroundRole);
        }

        QString commandDisp = QString ("[%1](%2,%3)")
                                .arg (cmd.messageBody().simplified())
                                .arg (cmd.dlFrom())
                                .arg (record->fieldValue("executor_name"));

        jModel->setData (cmdInd, commandDisp, Qt::DisplayRole);
        QIcon urgencyIcon;
        switch (record->fieldValue ("id_urgency_level").toInt())
        {
            case 1: urgencyIcon = QIcon (":/ddoc/urgency_1.png"); break;
            case 2: urgencyIcon = QIcon (":/ddoc/urgency_2.png"); break;
            case 3: urgencyIcon = QIcon (":/ddoc/urgency_3.png"); break;
        }
        jModel->setData (cmdInd, urgencyIcon, Qt::DecorationRole);
        if (cmdInd.parent().row() == 3)
        {
            QBrush rbr (Qt::red);
            jModel->setData (cmdInd, rbr, Qt::BackgroundRole);
            jModel->setData (cmdInd.parent(), rbr, Qt::BackgroundRole);
        }
        jModel->setData (cmdInd, QVariant::fromValue (cmd), Qt::UserRole);
        jModel->setData (cmdInd, cmd.id (), Qt::UserRole+1);
        jModel->setData (cmdInd, 0, Qt::UserRole+USER_ENTITY);
        //int nSum = 0;
        //parentIndex = parentIndex.parent ();
        for (; parentIndex.isValid(); parentIndex = parentIndex.parent ())
        {
            QString val = parentIndex.data (Qt::DisplayRole).toString ();
            int nVal = parentIndex.data (Qt::UserRole+2).toInt();
            //nSum +=nVal;
            val = QString ("%1 (%2)").arg (val.mid (0, val.lastIndexOf("(")-1)).arg (nVal);
            jModel->setData (parentIndex, val, Qt::DisplayRole);
            jModel->setData (parentIndex, nVal, Qt::UserRole+2);
        }
        if(cmd.id() == oldId){
            tvJournal->selectionModel()->setCurrentIndex (cmdInd, QItemSelectionModel::Select);
            //QModelIndex index = m_cmdJournalLv->model()->index(row, 0);
            //m_cmdJournalLv->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Rows);
        }

        row ++;
    }
}

void KKSJournalWidget :: initMsg()
{
    int oldId = -1;
    if(!m_msgData.isEmpty()){
        QItemSelectionModel * selModel = tvJournal->selectionModel ();
        QModelIndex wIndex = selModel->currentIndex();
        wIndex = wIndex.sibling (wIndex.row(), 0);
        QVariant cVal = wIndex.data (Qt::UserRole);
        int id_entity = wIndex.data (Qt::UserRole+USER_ENTITY).toInt();
        if (id_entity == 1 && cVal.canConvert<KKSMessage>())
        {
            KKSMessage msg = cVal.value<KKSMessage>();//m_msgJournalLv->currentMessage();
            if (msg.id() > 0)
                oldId = msg.id();
        }
    }

    int row = 0;
    KKSMap<int, KKSEIOData*>::const_iterator pd;
    QAbstractItemModel * jModel = tvJournal->model ();
    QModelIndex msgIndex = jModel->index (1, 0);
    jModel->setData (msgIndex, 0, Qt::UserRole+2);
    if (!jModel->rowCount (msgIndex))
        jModel->insertRows (0, 2, msgIndex);
    if (!jModel->columnCount(msgIndex))
        jModel->insertColumns (0, 1, msgIndex);
    QModelIndex msgInIndex = jModel->index (0, 0, msgIndex);
    jModel->setData (msgInIndex, tr ("Inbox (0)"), Qt::DisplayRole);
    jModel->setData (msgInIndex, 0, Qt::UserRole+2);
    QModelIndex msgOutIndex = jModel->index (1, 0, msgIndex);
    jModel->setData (msgOutIndex, tr ("Sent (0)"), Qt::DisplayRole);
    jModel->setData (msgOutIndex, 0, Qt::UserRole+2);
    if (!jModel->rowCount (msgInIndex))
        jModel->insertRows (0, 2, msgInIndex);
    if (!jModel->rowCount (msgOutIndex))
        jModel->insertRows (0, 2, msgOutIndex);
    if (!jModel->columnCount (msgInIndex))
        jModel->insertColumns (0, 1, msgInIndex);
    if (!jModel->columnCount (msgOutIndex))
        jModel->insertColumns (0, 1, msgOutIndex);
    QModelIndex msgReadInIndex = jModel->index (1, 0, msgInIndex);
    jModel->setData (msgReadInIndex, tr ("Were read (0)"), Qt::DisplayRole);
    jModel->setData (msgReadInIndex, 0, Qt::UserRole+2);
    if (jModel->rowCount (msgReadInIndex) > 0)
        jModel->removeRows (0, jModel->rowCount (msgReadInIndex), msgReadInIndex);
    QModelIndex msgNotReadInIndex = jModel->index (0, 0, msgInIndex);
    if (jModel->rowCount (msgNotReadInIndex) > 0)
        jModel->removeRows (0, jModel->rowCount (msgNotReadInIndex), msgNotReadInIndex);
    jModel->setData (msgNotReadInIndex, tr ("Were not read (0)"), Qt::DisplayRole);
    jModel->setData (msgNotReadInIndex, 0, Qt::UserRole+2);
    QModelIndex msgReadOutIndex = jModel->index (1, 0, msgOutIndex);
    jModel->setData (msgReadOutIndex, tr ("Were read (0)"), Qt::DisplayRole);
    jModel->setData (msgReadOutIndex, 0, Qt::UserRole+2);
    if (jModel->rowCount (msgReadOutIndex) > 0)
        jModel->removeRows (0, jModel->rowCount (msgReadOutIndex), msgReadOutIndex);
    QModelIndex msgNotReadOutIndex = jModel->index (0, 0, msgOutIndex);
    jModel->setData (msgNotReadOutIndex, tr ("Were not read (0)"), Qt::DisplayRole);
    jModel->setData (msgNotReadOutIndex, 0, Qt::UserRole+2);
    if (jModel->rowCount (msgNotReadOutIndex) > 0)
        jModel->removeRows (0, jModel->rowCount (msgNotReadOutIndex), msgNotReadOutIndex);
    if(m_msgData.isEmpty())
        return;
    for (pd = m_msgData.constBegin(); pd != m_msgData.constEnd(); pd++)
    {
        KKSEIOData * record = pd.value();
        if(!record)
            continue;

        QModelIndex parentIndex = msgIndex;
        int nVal = parentIndex.data (Qt::UserRole+2).toInt();
        jModel->setData (parentIndex, ++nVal, Qt::UserRole+2);
        KKSMessage msg;
        //
        // Message number
        //
        msg.setId(record->fieldValue("id").toInt());

        msg.setObjectId(record->fieldValue("id_io_object").toInt());
        msg.setObjectName(record->fieldValue("object_name"));

        msg.setCategoryId(record->fieldValue("id_io_category").toInt());
        msg.setCategoryName(record->fieldValue("category_name"));


        int idDlReceiver = record->fieldValue("id_dl_receiver").toInt();
        int idDlSender = record->fieldValue("id_dl_sender").toInt();
        int jrType = record->fieldValue("msg_type").toInt();
        Q_UNUSED (jrType);
        KKSMessage::MsgType type;
        if(m_idDl == idDlReceiver)
        {
            type = KKSMessage::mtIn;
            parentIndex = msgInIndex;
            int nVal = parentIndex.data (Qt::UserRole+2).toInt();
            jModel->setData (parentIndex, ++nVal, Qt::UserRole+2);
        }
        else if(m_idDl == idDlSender)
        {
            type = KKSMessage::mtOut;
            parentIndex = msgOutIndex;
            int nVal = parentIndex.data (Qt::UserRole+2).toInt();
            jModel->setData (parentIndex, ++nVal, Qt::UserRole+2);
        }
        else
        {
            type = KKSMessage::mtIn;
            parentIndex = msgInIndex;
            int nVal = parentIndex.data (Qt::UserRole+2).toInt();
            jModel->setData (parentIndex, ++nVal, Qt::UserRole+2);
        }

        msg.setMsgType(type);

        msg.setDlTo(record->fieldValue("dl_receiver_name"));
        msg.setDlFrom(record->fieldValue("dl_sender_name"));

        QDateTime dt = KKSValue::stringToDateTime(record->fieldValue("sent_datetime"));
        msg.setSentDateTime(dt);

        dt = KKSValue::stringToDateTime(record->fieldValue("receive_datetime"));
        msg.setReceiveDateTime(dt);

        dt = KKSValue::stringToDateTime(record->fieldValue("read_datetime"));
        msg.setReadDateTime(dt);
        if (dt.isValid())
            parentIndex = jModel->index (1, 0, parentIndex);
        else
            parentIndex = jModel->index (0, 0, parentIndex);
        int ncount = parentIndex.data (Qt::UserRole+2).toInt ();
        jModel->setData (parentIndex, ++ncount, Qt::UserRole+2);

        dt = KKSValue::stringToDateTime(record->fieldValue("insert_time"));
        msg.setInsertTime(dt);

        msg.setMessageBody(record->fieldValue("message_body"));
        //jModel->insertRows (jModel->rowCount(parentIndex), 1, parentIndex);
        //if (jModel->columnCount (parentIndex) == 0)
        //    jModel->insertColumns (0, 1, parentIndex);

        QModelIndex msgInd = QModelIndex ();
        for (int ii=0; ii<jModel->rowCount(parentIndex) && !msgInd.isValid(); ii++)
        {
            QModelIndex mIndex = jModel->index (ii, 0, parentIndex);
            if (msg.id() == mIndex.data (Qt::UserRole+1).toInt())
                msgInd = mIndex;
        }

        if (!msgInd.isValid())
        {
            bool is_inserted = jModel->insertRows (jModel->rowCount(parentIndex), 1, parentIndex);
            if (!is_inserted)
                continue;
            if (jModel->columnCount (parentIndex) == 0)
                jModel->insertColumns (0, 1, parentIndex);
            msgInd = jModel->index (jModel->rowCount(parentIndex)-1, 0, parentIndex);
        }

        QString messageDisp = QString ("[%1](%2)")
                                .arg (msg.messageBody().simplified())
                                .arg (parentIndex == msgReadInIndex || parentIndex == msgNotReadInIndex ? record->fieldValue("dl_sender_name") : record->fieldValue("dl_receiver_name"));
                                //.arg (record->fieldValue("executor_name"));

        jModel->setData (msgInd, messageDisp, Qt::DisplayRole);
        QIcon urgencyIcon;
        switch (record->fieldValue ("id_urgency_level").toInt())
        {
            case 1: urgencyIcon = QIcon (":/ddoc/urgency_1.png"); break;
            case 2: urgencyIcon = QIcon (":/ddoc/urgency_2.png"); break;
            case 3: urgencyIcon = QIcon (":/ddoc/urgency_3.png"); break;
        }
        jModel->setData (msgInd, urgencyIcon, Qt::DecorationRole);
        jModel->setData (msgInd, QVariant::fromValue (msg), Qt::UserRole);
        jModel->setData (msgInd, msg.id (), Qt::UserRole+1);
        jModel->setData (msgInd, 1, Qt::UserRole+USER_ENTITY);
        jModel->setData (parentIndex, jModel->rowCount(parentIndex), Qt::UserRole+2);
/*        if (parentIndex == msgReadInIndex ||
            parentIndex == msgNotReadInIndex)
            jModel->setData (msgInd, tr ("Sent from %1").arg (record->fieldValue("dl_sender_name")), Qt::DisplayRole);
        else
            jModel->setData (msgInd, tr ("Sent to %1").arg (record->fieldValue("dl_receiver_name")), Qt::DisplayRole);
 */
        //m_msgJournalLv->addMessage(msg);
        //int nSum = 0;
        for (; parentIndex.isValid(); parentIndex = parentIndex.parent ())
        {
            QString val = parentIndex.data (Qt::DisplayRole).toString ();
            int nVal = parentIndex.data (Qt::UserRole+2).toInt();
            //nSum +=nVal;
            val = QString ("%1 (%2)").arg (val.mid (0, val.lastIndexOf("(")-1)).arg (nVal);
            jModel->setData (parentIndex, val, Qt::DisplayRole);
            jModel->setData (parentIndex, nVal, Qt::UserRole+2);
        }
        if(msg.id() == oldId){
            QModelIndex index = tvJournal->model()->index(row, 0);
            //tvJournal->model()->setData (index, )
            tvJournal->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Rows);
        }

        row ++;
    }

    //m_msgJournalLv->setSelectionBehavior(QAbstractItemView::SelectRows);
    
}

void KKSJournalWidget :: initSysMenu()
{
    floatingAction = new QAction(tr("Floating"), this);
    floatingAction->setCheckable(true);
    connect(floatingAction, SIGNAL(triggered(bool)), SLOT(changeFloating(bool)));

    areaActions = new QActionGroup(this);
    areaActions->setExclusive(true);

    leftAction = new QAction(tr("Place on Left") , this);
    leftAction->setCheckable(true);
    connect(leftAction, SIGNAL(triggered(bool)), SLOT(placeLeft(bool)));

    rightAction = new QAction(tr("Place on Right") , this);
    rightAction->setCheckable(true);
    connect(rightAction, SIGNAL(triggered(bool)), SLOT(placeRight(bool)));

    topAction = new QAction(tr("Place on Top") , this);
    topAction->setCheckable(true);
    connect(topAction, SIGNAL(triggered(bool)), SLOT(placeTop(bool)));

    bottomAction = new QAction(tr("Place on Bottom") , this);
    bottomAction->setCheckable(true);
    connect(bottomAction, SIGNAL(triggered(bool)), SLOT(placeBottom(bool)));

    areaActions->addAction(leftAction);
    areaActions->addAction(rightAction);
    areaActions->addAction(topAction);
    areaActions->addAction(bottomAction);

    m_pSysMenu = new QMenu(tr("Message Journal"), this);
    m_pSysMenu->addAction(toggleViewAction());
    m_pSysMenu->addSeparator();
    m_pSysMenu->addAction(floatingAction);
    m_pSysMenu->addSeparator();
    m_pSysMenu->addActions(areaActions->actions());

    connect (tvJournal, SIGNAL (doubleClicked (const QModelIndex &)), this, SLOT (viewEntity (const QModelIndex& )) );
    QItemSelectionModel * selModel = tvJournal->selectionModel ();
    connect (selModel,
             SIGNAL (selectionChanged (const QItemSelection&, const QItemSelection&)),
             this,
             SLOT (cmdMsgSelectionChanged (const QItemSelection&, const QItemSelection& ))
            );
}

void KKSJournalWidget :: initCmdWidgetMenu()
{
    aCmdRefresh = new QAction (tr ("&Refresh"), this);
    connect (aCmdRefresh, SIGNAL (triggered()), this, SLOT (cmdRefresh ()));
    aSettingsCmd = new QAction(tr("Settings..."), this);
    aSettingsCmd->setStatusTip(tr("Setings of command journal"));
    connect(aSettingsCmd, SIGNAL(triggered()),
            this, SLOT(commandSettingsChanged()));

    aCreateOutCmd = new QAction(tr("Create Command..."), this);
    aCreateOutCmd->setStatusTip(tr("Create output command"));
    connect(aCreateOutCmd, SIGNAL(triggered()),
            this, SLOT(createOutCommand()));

    aDeleteOutCmd = new QAction(tr("Delete Command..."), this);
    aDeleteOutCmd->setStatusTip(tr("Delete output command"));
    connect(aDeleteOutCmd, SIGNAL(triggered()),
            this, SLOT(deleteOutCommand()));

    aEditOutCmd = new QAction(tr("Edit Command..."), this);
    aEditOutCmd->setStatusTip(tr("Edit output command"));
    connect(aEditOutCmd, SIGNAL(triggered()),
            this, SLOT(editOutCommand()));

    aDropPeriodCmd = new QAction(tr("Drop period command..."), this);
    aDropPeriodCmd->setStatusTip(tr("Drop period command"));
    connect(aDropPeriodCmd, SIGNAL(triggered()),
            this, SLOT(dropPeriodCommand()));

    aViewCmd = new QAction(tr("View command..."), this);
    aViewCmd->setStatusTip(tr("View command"));
    connect(aViewCmd, SIGNAL(triggered()),
            this, SLOT(viewCommand()));

    aViewCmdDoc = new QAction(tr("View command attachement..."), this);
    aViewCmdDoc->setStatusTip(tr("View command attachement"));
    connect(aViewCmdDoc, SIGNAL(triggered()),
            this, SLOT(viewCommandDoc()));

    aViewOutCmdResult = new QAction(tr("View result..."), this);
    aViewOutCmdResult->setStatusTip(tr("View executed result of command"));
    connect(aViewOutCmdResult, SIGNAL(triggered()),
            this, SLOT(viewCommandResults()));

    aExecuteInCmd = new QAction(tr("Execute Command..."), this);
    aExecuteInCmd->setStatusTip(tr("Execute input command"));
    connect(aExecuteInCmd, SIGNAL(triggered()),
            this, SLOT(executeCommand()));
    aExecuteExInCmd = new QAction(tr("Choose document as command result..."), this);
    aExecuteExInCmd->setStatusTip(tr("Choose document as command result"));
    connect(aExecuteExInCmd, SIGNAL(triggered()),
            this, SLOT(setDocumentAsResult()));

    aSetExecuting = new QAction(tr("Set as executing"), this);
    aSetExecuting->setStatusTip(tr("Set Command As Executing"));
    connect(aSetExecuting, SIGNAL(triggered()),
            this, SLOT(setCommandOnExecute()));

    m_pWidgetCmdMenu = new QMenu(tr("Command Journal"), this);
    m_pWidgetCmdMenu->addAction (aCmdRefresh);
    m_pWidgetCmdMenu->addSeparator ();

    //QMenu * inCmdMenu = new QMenu(tr("Input Commands"),  this);
    m_pWidgetCmdMenu->addAction (aSetExecuting);
    m_pWidgetCmdMenu->addSeparator();
    m_pWidgetCmdMenu->addAction (aExecuteInCmd);
    m_pWidgetCmdMenu->addAction(aExecuteExInCmd);
    //inCmdMenu->addAction(aSetExecuted);
    //m_pWidgetCmdMenu->addMenu(inCmdMenu);

    //QMenu * outCmdMenu = new QMenu(tr("Output Commands"),  this);
    m_pWidgetCmdMenu->addAction(aCreateOutCmd);
    m_pWidgetCmdMenu->addSeparator();
    m_pWidgetCmdMenu->addAction(aEditOutCmd);
    m_pWidgetCmdMenu->addAction(aDeleteOutCmd);
    m_pWidgetCmdMenu->addAction(aDropPeriodCmd);
    //m_pWidgetCmdMenu->addMenu(outCmdMenu);

    m_pWidgetCmdMenu->addSeparator();
    m_pWidgetCmdMenu->addAction(aViewCmd);
    m_pWidgetCmdMenu->addAction(aViewCmdDoc);
    m_pWidgetCmdMenu->addAction(aViewOutCmdResult);
    m_pWidgetCmdMenu->addSeparator();

    QMenu * viewMenu = new QMenu(tr("View"), this);
    viewMenu->addAction(toggleViewAction());
    viewMenu->addSeparator();
    viewMenu->addAction(floatingAction);
    viewMenu->addSeparator();
    viewMenu->addActions(areaActions->actions());

    m_pWidgetCmdMenu->addMenu(viewMenu);

    m_pWidgetCmdMenu->addSeparator();
    m_pWidgetCmdMenu->addAction(aSettingsCmd);

}

void KKSJournalWidget :: initMsgWidgetMenu()
{
    aMsgRefresh = new QAction (tr ("&Refresh"), this);
    connect (aMsgRefresh, SIGNAL (triggered ()), this, SLOT (msgRefresh()));

    aSettingsMsg = new QAction(tr("Settings..."), this);
    aSettingsMsg->setStatusTip(tr("Setings of message journal"));
    connect(aSettingsMsg, SIGNAL(triggered()),
            this, SLOT(messageSettingsChanged ()) );

    aCreateOutMsg = new QAction(tr("Create Message..."), this);
    aCreateOutMsg->setStatusTip(tr("Create output message"));
    connect(aCreateOutMsg, SIGNAL(triggered()),
            this, SLOT(createOutMessage()));

    aSetRead = new QAction(tr("Mark as read"), this);
    aSetRead->setStatusTip(tr("Mark input message as read"));
    connect(aSetRead, SIGNAL(triggered()),
            this, SLOT(markAsRead()));

    aViewOutMsgResult = new QAction(tr("View message attachment..."), this);
    aViewOutMsgResult->setStatusTip(tr("View document in message attachment"));
    connect(aViewOutMsgResult, SIGNAL(triggered()),
            this, SLOT(viewMessageAttachments()));

    aViewMsg = new QAction(tr("View message..."), this);
    aViewMsg->setStatusTip(tr("View message"));
    connect(aViewMsg, SIGNAL(triggered()),
            this, SLOT(viewMessage()));

    aViewOutMsgFile = new QAction(tr("View message attachment files..."), this);
    aViewOutMsgFile->setStatusTip(tr("View message files"));
    connect(aViewOutMsgFile, SIGNAL(triggered()),
            this, SLOT(viewMsgFiles()));

    m_pWidgetMsgMenu = new QMenu(tr("Command/Message Journal"), this);
    m_pWidgetMsgMenu->addAction (aMsgRefresh);
    aMsgRefreshSeparator = m_pWidgetMsgMenu->addSeparator();

    m_pWidgetMsgMenu->addAction (aSetRead);

    m_pWidgetMsgMenu->addAction (aCreateOutMsg);
    aMsgCreateSeparator = m_pWidgetMsgMenu->addSeparator();

    m_pWidgetMsgMenu->addAction (aViewOutMsgResult);
    m_pWidgetMsgMenu->addAction (aViewOutMsgFile);
    m_pWidgetMsgMenu->addAction (aViewMsg);
    m_pWidgetMsgMenu->addSeparator();

    QMenu * viewMenu = new QMenu(tr("View"), this);
    viewMenu->addAction (toggleViewAction());
    viewMenu->addSeparator ();
    viewMenu->addAction (floatingAction);
    viewMenu->addSeparator ();
    viewMenu->addActions (areaActions->actions());

    m_pWidgetMsgMenu->addMenu (viewMenu);

    m_pWidgetMsgMenu->addSeparator();
    m_pWidgetMsgMenu->addAction (aSettingsMsg);
}

void KKSJournalWidget :: updateMsgMenuItems(const QModelIndex& wIndex)
{
    //QModelIndex wIndex = tvJournal->selectionModel()->currentIndex ();
    if (wIndex.parent().isValid() && wIndex.parent().parent().isValid() && wIndex.parent().parent().parent().isValid())
    {
        KKSMessage msg = wIndex.data (Qt::UserRole).value<KKSMessage>();//m_msgJournalLv->currentMessage();

        bool isCanViewAttachment = canViewAttachement (msg);
        aViewOutMsgResult->setVisible (isCanViewAttachment);
        if (isCanViewAttachment)
            emit loadMsgFiles (msg.objectId());
        else
            aViewOutMsgFile->setEnabled (false);
        bool isSetRead = canSetAsRead (msg);
        aSetRead->setVisible (isSetRead);
        bool isOutMessage = (msg.msgType() == KKSMessage::mtOut);
        aCreateOutMsg->setVisible (isOutMessage);
    }
    else if (wIndex.parent().isValid() && wIndex.parent().parent().isValid())
    {
        bool isSetRead = (wIndex.row() == 0);//canSetAsRead (msg);
        aSetRead->setVisible (isSetRead);
        bool isOutMessage = (wIndex.parent().row() > 0);
        aCreateOutMsg->setVisible (isOutMessage);
    }
    else if (wIndex.parent().isValid())
    {
        bool isOutMessage = true;//(wIndex.row() > 0);
        aCreateOutMsg->setVisible (isOutMessage);
        aSetRead->setVisible (false);
        aViewOutMsgResult->setVisible (false);
        aViewMsg->setVisible (false);
        aViewOutMsgFile->setVisible (false);
        aSetRead->setVisible (false);
    }
    else
    {
        aCreateOutMsg->setVisible (true);
        aSetRead->setVisible (false);
        aViewOutMsgResult->setVisible (false);
        aViewMsg->setVisible (false);
        aViewOutMsgFile->setVisible (false);
        aSetRead->setVisible (false);
    }
}

int KKSJournalWidget :: currentMsgJournalRecord() const
{
    QModelIndex wIndex = tvJournal->selectionModel()->currentIndex ();
    KKSMessage msg = wIndex.data (Qt::UserRole).value<KKSMessage>();
    int idJournal = msg.id();

    return idJournal;
}

void KKSJournalWidget :: slotShowMsgError (QString error) const
{
    qCritical() << error;
    QMessageBox::critical(this->widget(), tr("Error"), error, QMessageBox::Ok);
}

void KKSJournalWidget :: contextMenuEvent (QContextMenuEvent *e)
{
    e->accept ();
    QModelIndex wIndex = tvJournal->selectionModel()->currentIndex();//indexAt (e->globalPos());
    QModelIndex rootIndex (wIndex);
    int i = wIndex.row();
    int j = wIndex.column ();
    qDebug () << __PRETTY_FUNCTION__ << i << j;
    if (!wIndex.isValid())
        return;
    while (rootIndex.parent().isValid())
        rootIndex = rootIndex.parent();
    int id_entity = rootIndex.data (Qt::UserRole+USER_ENTITY).toInt();
    if (id_entity == 1)
    {
        //updateMsgMenuItems(wIndex);
        m_pWidgetMsgMenu->exec(e->globalPos());
    }
    else if (id_entity == 0)
    {
        //updateCmdMenuItems (wIndex);
        m_pWidgetCmdMenu->exec (e->globalPos());
    }
}

void KKSJournalWidget :: cmdMsgSelectionChanged (const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED (deselected);
    if(selected.indexes().count() == 0)
        return;

    QModelIndex wIndex = selected.indexes().at (0);
    wIndex = wIndex.sibling (wIndex.row(), 0);
    QModelIndex rootIndex (wIndex);
    while (rootIndex.parent().isValid())
        rootIndex = rootIndex.parent();
    int id_entity = rootIndex.data (Qt::UserRole+USER_ENTITY).toInt();
    if (id_entity == 1)
        updateMsgMenuItems(wIndex);
    else if (id_entity == 0)
        updateCmdMenuItems (wIndex);
}

void KKSJournalWidget :: updateCmdMenuItems (const QModelIndex& wIndex)
{
    //QModelIndex wIndex = tvJournal->selectionModel()->currentIndex ();
    //KKSCommand cmd = m_cmdJournalLv->currentCommand();

    if (wIndex.parent().isValid() && wIndex.parent().parent().isValid() && wIndex.parent().parent().parent().isValid())
    {
        KKSCommand cmd = wIndex.data (Qt::UserRole).value<KKSCommand>();//m_msgJournalLv->currentMessage();
        bool isVisible = canExecuteCmd(cmd);
        aExecuteInCmd->setVisible (isVisible);
        aExecuteExInCmd->setVisible (isVisible);
        isVisible = canSetAsExecuting(cmd) && wIndex.parent().row() != 3;
        aSetExecuting->setVisible (isVisible);
        isVisible = canEditOutCmd(cmd);
        aEditOutCmd->setVisible (isVisible);
        isVisible = canDeleteOutCmd(cmd);
        aDeleteOutCmd->setVisible (isVisible);
        aViewCmd->setVisible (true);
        aViewOutCmdResult->setVisible (canViewResultsCmd(cmd));
        aViewCmdDoc->setVisible (canViewCmdDoc(cmd));
        aDropPeriodCmd->setVisible (canDropPeriodCmd(cmd));
    }
    else if (wIndex.parent().isValid() && wIndex.parent().parent().isValid())
    {
        aExecuteInCmd->setVisible (false);
        aSetExecuting->setVisible (false);
        aExecuteExInCmd->setVisible (false);
        aEditOutCmd->setVisible (false);
        aDeleteOutCmd->setVisible (false);
        aViewCmd->setVisible (false);
        aViewOutCmdResult->setVisible (false);
        aViewCmdDoc->setVisible (false);
        aDropPeriodCmd->setVisible (false);
        aCreateOutCmd->setVisible (true);
    }
    else if (wIndex.parent().isValid())
    {
        //int iRow = wIndex.row();
        aExecuteInCmd->setVisible (false);
        aSetExecuting->setVisible (false);
        aEditOutCmd->setVisible (false);
        aDeleteOutCmd->setVisible (false);
        aViewOutCmdResult->setVisible (false);
        aViewCmdDoc->setVisible (false);
        aDropPeriodCmd->setVisible (false);
        aExecuteExInCmd->setVisible (false);
        aViewCmd->setVisible (false);
        aCreateOutCmd->setVisible (true);
    }
    else
    {
        aExecuteInCmd->setVisible (false);
        aSetExecuting->setVisible (false);
        aEditOutCmd->setVisible (false);
        aDeleteOutCmd->setVisible (false);
        aViewOutCmdResult->setVisible (false);
        aViewCmdDoc->setVisible (false);
        aDropPeriodCmd->setVisible (false);
        aExecuteExInCmd->setVisible (false);
        aViewCmd->setVisible (false);
        aCreateOutCmd->setVisible (true);
    }
}

bool KKSJournalWidget :: canViewResultsCmd(const KKSCommand & cmd)
{
    if(cmd.id() <= 0)
        return false;

    if(cmd.cmdState() == KKSCommand::csExecuted){
        return true;
    }

    if(cmd.cmdState() == KKSCommand::csSavedAsDraft &&
       cmd.cmdType() == KKSCommand::ctIn)//ctAllIn не подходит ибо только автор может просмотреть свой черновик
    {
        return true;
    }

    return false;
}

bool KKSJournalWidget :: canViewCmdDoc(const KKSCommand & cmd)
{
    if(cmd.id() <= 0)
        return false;

    if(cmd.ioObjectId() > 0){
        return true;
    }

    return false;
}

void KKSJournalWidget :: messageSettingsChanged ()
{
    KKSMsgJSettings settings (*m_msgSettings);

    MsgJournalSettingsForm * f = new MsgJournalSettingsForm(settings, this);
    if (f->exec() == QDialog::Accepted)
    {
        settings = f->settings();

        //m_settings->bAllInput = settings.bAllInput;
        //m_settings->bAllOutput = settings.bAllOutput;
        m_msgSettings->bMyInput = settings.bMyInput;
        //m_settings->bInputExecuted = settings.bInputExecuted;
        //m_settings->bOutputExecuted = settings.bOutputExecuted;
        //m_settings->bInputArchived = settings.bInputArchived;
        //m_settings->bOutputArchived = settings.bOutputArchived;
        m_msgSettings->bMyOutput = settings.bMyOutput;
        m_msgSettings->bUseInputDate = settings.bUseInputDate;
        m_msgSettings->bUseOutputDate = settings.bUseOutputDate;
        m_msgSettings->dInputDate = settings.dInputDate;
        m_msgSettings->dOutputDate = settings.dOutputDate;
        m_msgSettings->queryInterval = settings.queryInterval;

        emit msgSettingsChanged();
    }

    delete f;

}

bool KKSJournalWidget :: canViewAttachement(const KKSMessage & msg)
{
    if(msg.id() <= 0)
        return false;

    if(msg.objectId() <= 0)
        return false;

    return true;
}

bool KKSJournalWidget :: canSetAsRead(const KKSMessage & msg)
{
    if(msg.id() <= 0)
        return false;

    if(msg.msgType() != KKSMessage::mtIn)
        return false;

    QString text = msg.readDateTime().toString("dd.MM.yyyy");
    if(!text.isEmpty())
        return false;

    return true;
}

void KKSJournalWidget :: placeLeft(bool p)
{
    place(Qt::LeftDockWidgetArea, p);
}

void KKSJournalWidget :: placeRight(bool p)
{
    place(Qt::RightDockWidgetArea, p);
}

void KKSJournalWidget :: placeTop(bool p)
{
    place(Qt::TopDockWidgetArea, p);
}

void KKSJournalWidget :: placeBottom(bool p)
{
    place(Qt::BottomDockWidgetArea, p);
}

void KKSJournalWidget :: isFilesEnabled (const KKSList<KKSFile *>& files)
{
    int nFiles = files.count();
    aViewOutMsgFile->setEnabled (nFiles > 0);
    msgFiles = files;
}

void KKSJournalWidget :: changeFloating(bool on)
{
    setFloating (on);//floating);
}

void KKSJournalWidget :: createOutMessage()
{
    emit msgCreateOutMsg();
}

void KKSJournalWidget :: place(Qt::DockWidgetArea area, bool place)
{
    if (!place)
        return;

    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());
    if(mainWindow)
        mainWindow->addDockWidget(area, this);

}

void KKSJournalWidget :: commandSettingsChanged ()
{
    KKSCmdJSettings settings (*m_cmdSettings);
    CmdJournalSettingsForm * f = new CmdJournalSettingsForm(settings, this);
    if(f->exec() == QDialog::Accepted){
        settings = f->settings();

        m_cmdSettings->bAllInput = settings.bAllInput;
        m_cmdSettings->bAllOutput = settings.bAllOutput;
        m_cmdSettings->bMyInput = settings.bMyInput;
        m_cmdSettings->bInputExecuted = settings.bInputExecuted;
        m_cmdSettings->bOutputExecuted = settings.bOutputExecuted;
        m_cmdSettings->bInputArchived = settings.bInputArchived;
        m_cmdSettings->bOutputArchived = settings.bOutputArchived;
        m_cmdSettings->bMyOutput = settings.bMyOutput;
        m_cmdSettings->bUseInputDate = settings.bUseInputDate;
        m_cmdSettings->bUseOutputDate = settings.bUseOutputDate;
        m_cmdSettings->dInputDate = settings.dInputDate;
        m_cmdSettings->dOutputDate = settings.dOutputDate;
        m_cmdSettings->queryInterval = settings.queryInterval;

        emit cmdSettingsChanged();
    }

    delete f;

}

void KKSJournalWidget :: createOutCommand ()
{
    emit createOutCmd();
}

void KKSJournalWidget :: deleteOutCommand (void)
{
    QItemSelectionModel * selModel = tvJournal->selectionModel ();
    QModelIndex cmdIndex = selModel->currentIndex ();
    if (tvJournal->model()->hasChildren (cmdIndex))
    {
        qWarning() << tr ("Please select command for delete");
        QMessageBox::warning (this, tr ("Delete command"), tr ("Please select command for delete"), QMessageBox::Ok);
        return;
    }
    KKSCommand cmd = cmdIndex.data (Qt::UserRole).value <KKSCommand>();//m_cmdJournalLv->currentCommand();
    bool ok = canDeleteOutCmd(cmd);
    if(ok)
        emit setAsArchived(cmd.id(), true);

}

bool KKSJournalWidget :: canDeleteOutCmd(const KKSCommand & cmd)
{
    //пока запрещаем удал€ть распор€жени€, в том числе просто выданные
    return false;

    if(cmd.id() <= 0)
        return false;

    if(cmd.cmdType() != KKSCommand::ctOut &&
        cmd.cmdType() != KKSCommand::ctAllOut)
    {
        return false;
    }

    if(cmd.cmdState() != KKSCommand::csOuted ||
        cmd.cmdState() != KKSCommand::csPlanned)
    {
        return false;
    }

    return true;
}

void KKSJournalWidget :: editOutCommand (void)
{
    QItemSelectionModel * selModel = tvJournal->selectionModel ();
    QModelIndex cmdIndex = selModel->currentIndex ();
    if (tvJournal->model()->hasChildren (cmdIndex))
    {
        qWarning() << tr ("Please select command for edit");
        QMessageBox::warning (this, tr ("Edit command"), tr ("Please select command for edit"), QMessageBox::Ok);
        return;
    }
    KKSCommand cmd = cmdIndex.data (Qt::UserRole).value <KKSCommand>();
    //KKSCommand cmd = m_cmdJournalLv->currentCommand();
    bool ok = canEditOutCmd(cmd);
    if(ok)
        emit editOutCmd(cmd.id());
}

bool KKSJournalWidget :: canEditOutCmd(const KKSCommand & cmd)
{
    //пока запрещаем редактировать распор€жени€
    return false;

    if(cmd.id() <= 0)
        return false;

    if(cmd.cmdType() != KKSCommand::ctOut &&
        cmd.cmdType() != KKSCommand::ctAllOut)
    {
        return false;
    }

    if(cmd.cmdState() != KKSCommand::csOuted ||
        cmd.cmdState() != KKSCommand::csPlanned)
    {
        return false;
    }

    return true;
}

void KKSJournalWidget :: dropPeriodCommand (void)
{
    QItemSelectionModel * selModel = tvJournal->selectionModel ();
    QModelIndex cmdIndex = selModel->currentIndex ();
    if (tvJournal->model()->hasChildren (cmdIndex))
    {
        qWarning() << tr ("Please select command");
        QMessageBox::warning (this, tr ("Drop command period"), tr ("Please select command"), QMessageBox::Ok);
        return;
    }
    KKSCommand cmd = cmdIndex.data (Qt::UserRole).value <KKSCommand>();
//    KKSCommand cmd = m_cmdJournalLv->currentCommand();
    bool ok = canDropPeriodCmd(cmd);
    if(ok)
        emit dropPeriodCmd(cmd.id());
}

bool KKSJournalWidget :: canDropPeriodCmd (const KKSCommand & cmd)
{
    if(cmd.id() <= 0)
        return false;

    if(cmd.cmdType() != KKSCommand::ctOut &&
        cmd.cmdType() != KKSCommand::ctAllOut)
    {
        return false;
    }

    if(cmd.execPeriod().isEmpty())
        return false;

    return true;
}

void KKSJournalWidget :: viewCommand (void)
{
    QItemSelectionModel * selModel = tvJournal->selectionModel ();
    QModelIndex cmdIndex = selModel->currentIndex ();
    if (tvJournal->model()->hasChildren (cmdIndex))
    {
        qWarning() << tr ("Please select command");
        QMessageBox::warning (this, tr ("View command"), tr ("Please select command"), QMessageBox::Ok);
        return;
    }
    //KKSCommand cmd = cmdIndex.data (Qt::UserRole).value <KKSCommand>();
    int id =  cmdIndex.data (Qt::UserRole+1).toInt();//currentJournalRecord();
    if(id > 0)
        emit viewCmd(id);

}

void KKSJournalWidget :: viewCommandDoc (void)
{
    QItemSelectionModel * selModel = tvJournal->selectionModel ();
    QModelIndex cmdIndex = selModel->currentIndex ();
    if (tvJournal->model()->hasChildren (cmdIndex))
    {
        qWarning() << tr ("Please select command");
        QMessageBox::warning (this, tr ("View command"), tr ("Please select command"), QMessageBox::Ok);
        return;
    }
    KKSCommand cmd = cmdIndex.data (Qt::UserRole).value <KKSCommand>();
    //KKSCommand cmd = m_cmdJournalLv->currentCommand();
    int idObject = cmd.ioObjectId();
    //int id = currentJournalRecord();
    if(idObject > 0)
        emit viewCmdDoc(idObject);
    
}

void KKSJournalWidget :: viewCommandResults (void)
{
    QItemSelectionModel * selModel = tvJournal->selectionModel ();
    QModelIndex cmdIndex = selModel->currentIndex ();
    if (tvJournal->model()->hasChildren (cmdIndex))
    {
        qWarning() << tr ("Please select command");
        QMessageBox::warning (this, tr ("View command results"), tr ("Please select command"), QMessageBox::Ok);
        return;
    }
    KKSCommand cmd = cmdIndex.data (Qt::UserRole).value <KKSCommand>();
    int id = cmdIndex.data (Qt::UserRole+1).toInt();
    if (id > 0){
        bool bShowExecButton = false;
        if(cmd.cmdState() == KKSCommand::csSavedAsDraft)
            bShowExecButton = true;

        emit viewResultCmd(id, bShowExecButton);
    }
    
}

void KKSJournalWidget :: executeCommand (void)
{
    QItemSelectionModel * selModel = tvJournal->selectionModel ();
    QModelIndex cmdIndex = selModel->currentIndex ();
    if (tvJournal->model()->hasChildren (cmdIndex))
    {
        qWarning() << tr ("Please select command");
        QMessageBox::warning (this, tr ("Execute command"), tr ("Please select command"), QMessageBox::Ok);
        return;
    }
    KKSCommand cmd = cmdIndex.data (Qt::UserRole).value <KKSCommand>();
//    KKSCommand cmd = m_cmdJournalLv->currentCommand();
    bool ok = canExecuteCmd(cmd);
    if(!ok)
        return;

    int idCategory = cmd.categoryId();
    int idJournal = cmd.id();

    if(idCategory <= 0 || idJournal <= 0){
        qCritical() << tr("You should select one command!");
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("You should select one command!"),
                              QMessageBox::Ok);
        return;
    }

    emit executeClicked(idJournal, idCategory);
    
}

bool KKSJournalWidget :: canExecuteCmd(const KKSCommand & cmd)
{
    if(cmd.id() <= 0)
        return false;

    if(cmd.cmdType() != KKSCommand::ctIn &&
        cmd.cmdType() != KKSCommand::ctAllIn)
    {
        return false;
    }

    if(cmd.cmdState() == KKSCommand::csExecuting || cmd.cmdState() == KKSCommand::csTimeElapsed){
        return true;
    }

    return false;
}

void KKSJournalWidget :: setDocumentAsResult (void)
{
    QItemSelectionModel * selModel = tvJournal->selectionModel ();
    QModelIndex cmdIndex = selModel->currentIndex ();
    if (tvJournal->model()->hasChildren (cmdIndex))
    {
        qWarning() << tr ("Please select command");
        QMessageBox::warning (this, tr ("Execute command"), tr ("Please select command"), QMessageBox::Ok);
        return;
    }
    KKSCommand cmd = cmdIndex.data (Qt::UserRole).value <KKSCommand>();
//    KKSCommand cmd = m_cmdJournalLv->currentCommand();
    bool ok = canExecuteCmd(cmd);
    if(!ok)
        return;

    int idCategory = cmd.categoryId();
    int idJournal = cmd.id();

    if(idCategory <= 0 || idJournal <= 0){
        qCritical() << tr("You should select one command!");
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("You should select one command!"),
                              QMessageBox::Ok);
        return;
    }

    emit executeExClicked(idJournal, idCategory);

}

void KKSJournalWidget :: setCommandOnExecute (void)
{
    QItemSelectionModel * selModel = tvJournal->selectionModel ();
    QModelIndex cmdIndex = selModel->currentIndex ();
    if (tvJournal->model()->hasChildren (cmdIndex))
    {
        qWarning() << tr ("Please select command");
        QMessageBox::warning (this, tr ("Execute command"), tr ("Please select command"), QMessageBox::Ok);
        return;
    }
    KKSCommand cmd = cmdIndex.data (Qt::UserRole).value <KKSCommand>();
    bool ok = canSetAsExecuting(cmd);
    if(ok)
        emit setAsExecuting(cmd.id());
}

bool KKSJournalWidget :: canSetAsExecuting(const KKSCommand & cmd)
{
    int id = cmd.id();
    if(id <= 0)
        return false;

    if(cmd.cmdType() != KKSCommand::ctIn)
        return false;

    if(cmd.cmdState() == KKSCommand::csExecuted ||
        cmd.cmdState() == KKSCommand::csExecuting ||
        cmd.cmdState() == KKSCommand::csSavedAsDraft ||
        cmd.cmdState() == KKSCommand::csTimeElapsed)
    {
        return false;
    }

    return true;
}

void KKSJournalWidget :: markAsRead (void)
{
    QItemSelectionModel * selModel = tvJournal->selectionModel ();
    QModelIndex msgIndex = selModel->currentIndex ();
    if (tvJournal->model()->hasChildren (msgIndex))
    {
        qWarning() << tr ("Please select message");
        QMessageBox::warning (this, tr ("Mark message"), tr ("Please select message"), QMessageBox::Ok);
        return;
    }
    KKSMessage msg = msgIndex.data (Qt::UserRole).value <KKSMessage>();
    int id = msgIndex.data (Qt::UserRole+1).toInt();//currentJournalRecord();
    if(id > 0)
        emit msgMarkAsRead(id);
    
}

void KKSJournalWidget :: viewMessageAttachments (void)
{
    QItemSelectionModel * selModel = tvJournal->selectionModel ();
    QModelIndex msgIndex = selModel->currentIndex ();
    if (tvJournal->model()->hasChildren (msgIndex))
    {
        qWarning() << tr ("Please select message");
        QMessageBox::warning (this, tr ("View message attachments"), tr ("Please select message"), QMessageBox::Ok);
        return;
    }
    KKSMessage msg = msgIndex.data (Qt::UserRole).value <KKSMessage>();
//    KKSMessage msg = m_msgJournalLv->currentMessage();
    int idObject = msg.objectId();
    //int id = currentJournalRecord();
    if(idObject > 0)
        emit msgViewResultMsg (idObject);
}

void KKSJournalWidget :: viewMessage (void)
{
    QItemSelectionModel * selModel = tvJournal->selectionModel ();
    QModelIndex msgIndex = selModel->currentIndex ();
    if (tvJournal->model()->hasChildren (msgIndex))
    {
        qWarning() << tr ("Please select message");
        QMessageBox::warning (this, tr ("View message"), tr ("Please select message"), QMessageBox::Ok);
        return;
    }
    int id = msgIndex.data (Qt::UserRole+1).toInt();//currentJournalRecord();
    if(id > 0)
        emit viewMsg(id);
    
}

void KKSJournalWidget :: setCmdData (const KKSMap<int, KKSEIOData*> & cmdData)
{
    m_cmdData = cmdData;
    initCmd ();
}

void KKSJournalWidget :: setMsgData (const KKSMap<int, KKSEIOData*> & msgData)
{
    m_msgData = msgData;
    initMsg ();
}

void KKSJournalWidget :: msgRefresh (void)
{
    emit msgSettingsChanged ();
}

void KKSJournalWidget :: cmdRefresh (void)
{
    emit cmdSettingsChanged ();
}

void KKSJournalWidget :: viewEntity (const QModelIndex& wIndex)
{
    if (wIndex.parent().parent().parent().isValid())
    {
        int irow = wIndex.parent().parent().parent().row();
        if (irow == 0)
            this->viewCommand ();
        else
            this->viewMessage ();
    }
    else
    {
        bool isExp = tvJournal->isExpanded (wIndex);
        if (isExp)
            tvJournal->collapse (wIndex);
        else
            tvJournal->expand (wIndex);
    }
}