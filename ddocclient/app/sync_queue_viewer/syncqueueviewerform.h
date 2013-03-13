#ifndef _SYNCQUEUEVIEWRFORM_H
#define _SYNCQUEUEVIEWRFORM_H

#include<QDialog>

#include "syncqueueitemmodel.h"

class KKSDatabase;
class SyncQueueView;

namespace TableView
{
	const int TABLE_COLUMN_COUNT = 16; //���������� �������� � �������
}

class SyncQueueViewerForm : public QDialog
{
    Q_OBJECT

    public:
        SyncQueueViewerForm(KKSDatabase * adb, QWidget * parent=NULL);
        ~SyncQueueViewerForm ();

		//
		//����������� ������ ������� �������
        //
		QString GetCursorString(){return sqlCursor;}
		//
		//����������� ����� ������� � �������
        //
		int countInCursor();

        int GetColumnCount(){return countColumn;}
        int GetRowCount(){return countRow;}

        SyncQueueView* GetTreeView(){ return syncQueueTreeWnd;}
    private:
		//
		//������ ����������
		//
		QPushButton* qpb_filters;
		QPushButton* qpb_delete;
		QPushButton* qpb_restart;
		QPushButton* qpb_cancel;
		QPushButton* qpb_save;
		QPushButton* qpb_exit;
		QPushButton* qpb_view;

		//
		//������ ����������� ������
		//
        SyncQueueView *syncQueueTreeWnd;
		//
		//������ ����������� ������
		//
		SyncQueueItemModel * model;

        QString sqlCursor;	  //���������� ��� �������� �������

        KKSDatabase * db;	  //��������� �� ���� ������

        bool cursor_open_file;//���� �������� ����� �������
        bool cursor_open;     //���� �������� �������

        int countColumn;	  //���������� ��������
        int countRow;		  //���������� �����
        int count_colomn_logfile;

    private slots:
		//
		//���� ������ ������� ��� ��������� ��������
		//
		void slot_filters_setup();
		//
		//���� ��������� � ���� ������
        //
		void on_pbView_clicked();
		//
		//���� ���������� ������ ������
		//
		void slot_updateModelData(int input_topRow,int input_bottomRow);

	private:
		//
		//������� ��������� ������ �� ���� ������
		//
		void DBdata(int input_topRow,int input_bottomRow);
};

#endif
