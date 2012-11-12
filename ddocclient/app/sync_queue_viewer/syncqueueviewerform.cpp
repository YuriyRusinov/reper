#include <QProgressDialog>
#include <QMessageBox>
#include <QFile>
#include <QWidget>

//#include "debug.h"
#include "kksdatabase.h"

#include "syncqueueviewerform.h"
#include "syncqueueview.h"
#include "syncqueueviewform.h"
//#include "logeditsettings.h"

#include "ui_sync_queue_viewer_form.h"

SyncQueueViewerForm :: SyncQueueViewerForm(KKSDatabase * adb, QWidget * parent) :
    QDialog (parent)
{
    syncQueueView = NULL;
    db = adb;

    ui = new Ui::sync_queue_viewer_form;
    ui->setupUi(this);

    syncQueueTreeWnd = new SyncQueueView(ui->grBoxFilters);
    syncQueueTreeWnd->setObjectName(QString::fromUtf8("syncQueueTreeWnd"));
    syncQueueTreeWnd->setGeometry(QRect(10, 280, 501, 231));

    ui->vboxLayout->addWidget(syncQueueTreeWnd);

    syncQueueTreeWnd->SetDB(adb);

    cursor_open = false;
    cursor_open_file = false;
    count_colomn_logfile = 8;

    progress = NULL;
    
    InitSyncQueueViewerForm();
}

SyncQueueViewerForm:: ~SyncQueueViewerForm(void)
{
    db->close("sync_cursor_file");
    db->close("sync_cursor");
    db->close("init_cursor");

    db->commit();

    if(syncQueueView)
        delete syncQueueView;
}

void SyncQueueViewerForm::InitSyncQueueViewerForm()
{
    //KKSResult *rzBd = NULL;
    KKSResult* res = NULL;
    int i = 0;
    QString str = ""; 
    bool getOk;
    int iCount = 0;

    //rzBd = db->getLoggingLevel();

    int userRole_GroupIndex = Qt::UserRole + 1;


    ui->dateTimeEditTimeStart->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEditTimeEnd->setDateTime(QDateTime::currentDateTime());

    connectSlots();

    syncQueueView = new SyncQueueViewForm(this);
}


int SyncQueueViewerForm::countInCursor()
{
    KKSResult * res = NULL;

    int cur = sqlCursor.indexOf(" from ");
    //int cur = sqlCursor.indexOf("select * from");

    if(cur == -1)
        return 0;

    //QString newSql = "select count(*) " + sqlCursor.right(sqlCursor.length() - cur);
    //QString newSql = "select count(*) " + sqlCursor.right(sqlCursor.length() - cur - 8);
    QString newSql = QString("select count(*) from out_sync_queue");
    res = db->execute(newSql);

    int i;

    if(res)
    {
        i = res->getCellAsInt(0);
        countRow = i;

        delete res;
        return i;
    }
    else
    {
        countRow = 0;
        return 0;
    }
}

void SyncQueueViewerForm::on_pbExit_clicked()
{
    close();
}

void SyncQueueViewerForm::on_pbView_clicked()
{
    QString curUser = ui->comboBoxUsers->currentText();
    QString groupUser = ui->comboBoxGroups->currentText();
    QString curGroupOperation = ui->comboBoxGroupOperations->currentText();
    QString curOperation = ui->comboBoxOperations->currentText();

    QApplication::setOverrideCursor(Qt::WaitCursor);

    if(cursor_open)
    {
        db->close("sync_cursor");
        db->close("sync_cursor_file");
        db->close("init_cursor");
    }

    QString start;
    QString end;
    sqlCursor.clear();

    start = ui->dateTimeEditTimeStart->dateTime().toString("yyyy-MM-dd:hh:mm:ss");
    end = ui->dateTimeEditTimeEnd->dateTime().toString("yyyy-MM-dd:hh:mm:ss");


    //!!!Здесь надо добавить код, который будет формировать запрос в соответствии с фильтрами!!!


    /*
    if(curUser == tr("All") && curOperation == tr("All") && groupUser == tr("All") && curGroupOperation == tr("All"))
    {
        sqlCursor = SelectAll();
    }

    if(curUser != tr("All") && curOperation == tr("All") && curGroupOperation == tr("All"))
    {
        sqlCursor = SelectUser();
    }

    if(curGroupOperation != tr("All") && curOperation == tr("All") && groupUser == tr("All") && curUser == tr("All"))
    {
        sqlCursor = SelectGroupOperation();
    }
    
    if(groupUser != tr("All") && curUser == tr("All") && curGroupOperation == tr("All") && curOperation == tr("All"))
    {
        sqlCursor = SelectGroupUser();
    }

    if(curOperation != tr("All") && groupUser != tr("All") && curUser == tr("All"))
    {
        sqlCursor = SelectOperation();
    }

    if(groupUser != tr("All") && curOperation != tr("All"))
        sqlCursor = SelectUserOperation();

    if(groupUser != tr("All") && curUser == tr("All") && curGroupOperation != tr("All") && curOperation == tr("All"))
        sqlCursor = SelectGroupUserOperation();

    */

    //sqlCursor += QString(" \'%1\', \'%2\')").arg(start).arg(end);

    //sqlCursor = QString("select o.id, o.last_update, org.name, o.id_entity, o.entity_table, o.entity_type, o.sync_type, o.sync_result, o.entity_uid, o.entity_io_uid, o.id_organization, o.unique_id from out_sync_queue o inner join organization org on (o.id_organization = org.id)");
    
    
    
    sqlCursor = QString("select * from out_sync_queue");

    cursor_open = true;



    db->begin();

    db->declare("sync_cursor", sqlCursor);
    db->declare("sync_cursor_file", sqlCursor);
    db->declare("init_cursor", sqlCursor);

    countColumn = _TABLE_COLUMN_COUNT_;
    countRow = countInCursor();

    syncQueueTreeWnd->SetCountCursor(countRow);
    syncQueueTreeWnd->SetSQLCursor(sqlCursor);
    syncQueueTreeWnd->InitSyncQueueView();

    QApplication::restoreOverrideCursor();
}

void SyncQueueViewerForm::on_pbClear_clicked()
{
    if(cursor_open)
    {
        db->close("sync_cursor");
        db->close("sync_cursor_file");
        db->close("init_cursor");
    }

    countRow = 0;

    syncQueueTreeWnd->SetCountCursor(countRow);
    syncQueueTreeWnd->SetSQLCursor(sqlCursor);
    syncQueueTreeWnd->InitSyncQueueView();

    countColumn = _TABLE_COLUMN_COUNT_;
    countRow = 0;
}

/*
void SyncQueueViewerForm::on_pbSaveToFile_clicked()
{
    XGisResult *res = NULL;

    QString v;
    QString str;

    int i=0;
    int j=0;

    cursor_open_file = true;

    QString sName;// = xGis->getSaveFileName(
                //this,
                //tr("Choose a filename to save under"));

    if(sName == tr("")) 
        return;

    QFile data(sName);

    if (data.open(QFile::WriteOnly)) 
    {
        progress = new QProgressDialog(tr("Wait for save logs..."), tr("Abort save"), 0, countRow -1, this);
        progress->setCancelButton(NULL);

        res = db->fetch("log_cursor_file", 2);
        data.write("0");

        if(res)
        {
            for(j=0;j<count_colomn_logfile;j++)        
            {
                data.write("||");

                switch(j)
                {
                    case 0:
                        v = res->getCellAsString(0, 2);
                        data.write(v.toLocal8Bit());
                        break;
                    case 1:
                        v = res->getCellAsString(0, 4);
                        data.write(v.toLocal8Bit());
                        break;
                    case 2:
                        v = res->getCellAsString(0, 6);
                        data.write(v.toLocal8Bit());
                        break;
                    case 3:
                        v = res->getCellAsString(0, 7);
                        data.write(v.toLocal8Bit());
                        break;
                    case 4:
                        v = res->getCellAsString(0, 8);
                        data.write(v.toLocal8Bit());
                        break;
                    case 5:
                        v = res->getCellAsString(0, 9);
                        data.write(v.toLocal8Bit());
                        break;
                    case 6:
                        v = res->getCellAsString(0, 10);
                        data.write(v.toLocal8Bit());
                        break;
                    case 7:
                        v = res->getCellAsString(0, 11);
                        data.write(v.toLocal8Bit());
                    default:
                        break;
                }
            }

            data.write("||");
            delete res;
        }

        data.write("\n");
        data.close();

        data.open(QFile::Append);
        for(i=1;i<countRow -1;i++)
        {
            progress->setValue(i);
            qApp->processEvents();

            str = QString("%1").arg(i);
            str.append("||");
            data.write(str.toLocal8Bit());

            res = db->fetch("log_cursor_file");
            if(res)
            {
                for(j=0;j<count_colomn_logfile;j++)        
                {
                    data.write("||");

                    switch(j)
                    {
                        case 0:
                            v = res->getCellAsString(0, 2);
                            data.write(v.toLocal8Bit());
                            break;
                        case 1:
                            v = res->getCellAsString(0, 4);
                            data.write(v.toLocal8Bit());
                            break;
                        case 2:
                            v = res->getCellAsString(0, 6);
                            data.write(v.toLocal8Bit());
                            break;
                        case 3:
                            v = res->getCellAsString(0, 7);
                            data.write(v.toLocal8Bit());
                            break;
                        case 4:
                            v = res->getCellAsString(0, 8);
                            data.write(v.toLocal8Bit());
                            break;
                        case 5:
                            v = res->getCellAsString(0, 9);
                            data.write(v.toLocal8Bit());
                            break;
                        case 6:
                            v = res->getCellAsString(0, 10);
                            data.write(v.toLocal8Bit());
                            break;
                        case 7:
                            v = res->getCellAsString(0, 11);
                            data.write(v.toLocal8Bit());
                        default:
                            break;
                        }
                    }
                data.write("\n");
                }

                delete res;
        }

        data.write("\n");
        data.close();

        progress->setValue(countRow -1);
        QMessageBox::information(this, tr("Operation complete"), tr("Save finished successfully"), QMessageBox::Ok);
    }

    cursor_open_file = false;
}
*/

/*
void LogManagerForm::on_pbEditLogSettings_clicked()
{
    LogEditSettingsForm dlg(db, this);
    dlg.exec();
}

*/

void SyncQueueViewerForm::doubleClicked(const QModelIndex & index)
{
    Q_UNUSED(index);

    QMessageBox::warning(this, tr("Double clicked"), tr("Double clicked"));
    //syncQueueView->InitView();
    //syncQueueView->exec();
}

void SyncQueueViewerForm::connectSlots()
{
    connect(syncQueueTreeWnd, SIGNAL(doubleClicked (const QModelIndex)), this, SLOT(doubleClicked(const QModelIndex)));
}

/*
QString SyncQueueViewerForm::SelectUserOperation()
{
    QString sqlCursorA;
    int id_user = -1;
    int id_operation = -1;
    int userRole_OperationIndex = Qt::UserRole + 1;
    int userRole_UserIndex = Qt::UserRole + 1;
    int index = -1;

    index = ui->comboBoxOperations->currentIndex();
    id_operation =  ui->comboBoxOperations->itemData(index, userRole_OperationIndex).toInt();

    index = ui->comboBoxUsers->currentIndex();
    id_user =  ui->comboBoxUsers->itemData(index, userRole_UserIndex).toInt();

    sqlCursorA = QString("select * from getLog(%1, %2, ").arg(id_user).arg(id_operation);

    return sqlCursorA;
}

QString LogManagerForm::SelectAll()
{
    QString sqlCursorA;

    sqlCursorA = QString("select * from getLog(");

    return sqlCursorA;
}

QString LogManagerForm::SelectUser()
{
    QString sqlCursorA;
    int id_user = -1;
    int userRole_UserIndex = Qt::UserRole + 1;
    int index = -1;

    index = ui->comboBoxUsers->currentIndex();
    id_user =  ui->comboBoxUsers->itemData(index, userRole_UserIndex).toInt();

    sqlCursorA = QString("select * from getLog(%1,").arg(id_user);

    return sqlCursorA;
}

QString LogManagerForm::SelectGroupOperation()
{
    QString sqlCursorA;
    int userRole_GroupIndex = Qt::UserRole + 1;

    int index = ui->comboBoxGroupOperations->currentIndex();    
    int id_oper_group =  ui->comboBoxGroupOperations->itemData(index, userRole_GroupIndex).toInt();

    sqlCursorA = QString("select * from getLogByOperGroup(%1,").arg(id_oper_group);

    return sqlCursorA;
}

QString LogManagerForm::SelectGroupUser()
{
    QString sqlCursorA;
    int userRole_GroupIndex = Qt::UserRole + 1;

    int index = ui->comboBoxGroups->currentIndex();    
    int id_user_group =  ui->comboBoxGroups->itemData(index, userRole_GroupIndex).toInt();

    sqlCursorA = QString("select * from getLogByUserGroup(%1,").arg(id_user_group);

    return sqlCursorA;
}

QString LogManagerForm::SelectOperation()
{
    QString sqlCursorA;
    int userRole_OperationIndex = Qt::UserRole + 1;

    int index = ui->comboBoxOperations->currentIndex();    
    int id_operation =  ui->comboBoxOperations->itemData(index, userRole_OperationIndex).toInt();

    sqlCursorA = QString("select * from getLogByType(%1,").arg(id_operation);

    return sqlCursorA;
}

QString LogManagerForm::SelectGroupUserOperation()
{
    QString sqlCursorA;
    int userRole_GroupIndex = Qt::UserRole + 1;

    int index = ui->comboBoxGroups->currentIndex();    
    int id_user_group =  ui->comboBoxGroups->itemData(index, userRole_GroupIndex).toInt();

    index = ui->comboBoxGroupOperations->currentIndex();    
    int id_oper_group =  ui->comboBoxGroupOperations->itemData(index, userRole_GroupIndex).toInt();

    sqlCursorA = QString("select * from getLog(%1, NULL, %2, NULL, ").arg(id_user_group).arg(id_oper_group);

    return sqlCursorA;
}
*/

