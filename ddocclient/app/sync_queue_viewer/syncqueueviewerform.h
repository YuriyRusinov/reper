#ifndef _SYNCQUEUEVIEWRFORM_H
#define _SYNCQUEUEVIEWRFORM_H


#include <QDialog>
#include <QModelIndex>
#include <QProgressDialog>

class KKSDatabase;
class SyncQueueView;
class SyncQueueViewForm;

#define _TABLE_COLUMN_COUNT_ 12

namespace Ui 
{
    class sync_queue_viewer_form;
}

class SyncQueueViewerForm : public QDialog
{
    Q_OBJECT

    public:
        SyncQueueViewerForm(KKSDatabase * adb, QWidget * parent=NULL);
        ~SyncQueueViewerForm ();

        void InitSyncQueueViewerForm();
        //void InsertOperationAll(); 
        QString GetCursorString(){return sqlCursor;}
        int countInCursor();

        int GetColumnCount(){return countColumn;}
        int GetRowCount(){return countRow;}
        SyncQueueView* GetTreeView(){ return syncQueueTreeWnd;}
            
        SyncQueueViewForm *syncQueueView;
    private:
        Ui::sync_queue_viewer_form *ui;

        SyncQueueView *syncQueueTreeWnd ;

        QString sqlCursor;

        KKSDatabase * db;

        bool cursor_open_file;
        bool cursor_open;

        int countColumn;
        int countRow;
        int count_colomn_logfile;
        QProgressDialog * progress;

        //bool changeUser;
        //bool changeOperation;

        void connectSlots();

    private slots:
        //void on_pbEditLogSettings_clicked();
        //void on_pbSaveToFile_clicked();
        void on_pbClear_clicked();
        void on_pbView_clicked();
        void on_pbExit_clicked();
        void doubleClicked ( const QModelIndex & index );
        //QString SelectUserOperation();
        //QString SelectAll();
        //QString SelectUser();
        //QString SelectGroupOperation();
        //QString SelectGroupUser();
        //QString SelectOperation();
        //QString SelectGroupUserOperation();
};

#endif
