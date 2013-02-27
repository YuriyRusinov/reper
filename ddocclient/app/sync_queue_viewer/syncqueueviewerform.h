#ifndef _SYNCQUEUEVIEWRFORM_H
#define _SYNCQUEUEVIEWRFORM_H


#include <QDialog>
#include <QModelIndex>
#include <QProgressDialog>
#include <QHBoxLayout>
#include <QSplitter>

class KKSDatabase;
class SyncQueueView;
class SyncQueueViewForm;

#define _TABLE_COLUMN_COUNT_ 12

class SyncQueueViewerForm : public QDialog
{
    Q_OBJECT

    public:
        SyncQueueViewerForm(KKSDatabase * adb, QWidget * parent=NULL);
        ~SyncQueueViewerForm ();

		//����������� ������ ������� �������
        QString GetCursorString(){return sqlCursor;}
		//����������� ����� ������� � �������
        int countInCursor();

        int GetColumnCount(){return countColumn;}
        int GetRowCount(){return countRow;}
        SyncQueueView* GetTreeView(){ return syncQueueTreeWnd;}

    private:
		//������ ����������
		QPushButton* qpb_filters;
		QPushButton* qpb_delete;
		QPushButton* qpb_restart;
		QPushButton* qpb_cancel;
		QPushButton* qpb_save;
		QPushButton* qpb_exit;
		QPushButton* qpb_view;

        SyncQueueView *syncQueueTreeWnd ;// � ��� ����� ���?

        QString sqlCursor;

        KKSDatabase * db;

        bool cursor_open_file;
        bool cursor_open;

        int countColumn;
        int countRow;
        int count_colomn_logfile;
        QProgressDialog * progress;

    private slots:
		//���� ������ ������� ��� ��������� ��������
		void slot_filters_setup();
		//���� ��������� � ���� ������
        void on_pbView_clicked();
};

#endif
