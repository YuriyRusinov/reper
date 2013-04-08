#ifndef _SYNCQUEUEVIEWRFORM_H
#define _SYNCQUEUEVIEWRFORM_H

#include <QMessageBox>
#include <QApplication>
#include <QDialog>

#include "kksdatabase.h"
#include "syncqueueitemmodel.h"
#include "syncqueueview.h"
#include "filtersform.h"

namespace TableView
{
	const int TABLE_COLUMN_COUNT      = 17; //���������� �������� � ������� ����������� � ���� ������
	const int TABLE_COLUMN_COUNT_VIEW = 8;  //���������� �������� � ������������ �������
}

class SyncQueueViewerForm : public QDialog
{
    Q_OBJECT

    public:
		//*****�������� � ����������� ���������� ������*****
        SyncQueueViewerForm(KKSDatabase * adb, QWidget * parent=NULL);
        ~SyncQueueViewerForm ();
		//**********

	protected:
		//
		//���������� ������� ��������� �������
		//
		virtual void resizeEvent(QResizeEvent * pe);
		//virtual void mousePressEvent ( QMouseEvent * i_event );
		//virtual void mouseReleaseEvent ( QMouseEvent * i_event );
		//virtual void leaveEvent(QEvent * i_event);
		//virtual void enterEvent (QEvent * i_event);

    private slots:
		//
		//���� ������ ������� ��� ��������� ��������
		//
		void slot_filters_setup();
		//
		//���� ��������� � ���� ������
        //
		void slot_viewClicked();
		//
		//���� ���������� ������ ������
		//
		void slot_updateModelData(int i_topRow,int i_bottomRow);

		void slot_accepted();
		void slot_rejected();

	private:
		//
		//����������� ����� ������� � �������
        //
		int countInCursor();
		//
		//������� �������� �������
		//
		bool openCursor();
		//
		//������� ��������� ������ �� ���� ������
		//
		int DBdata(int i_topRow,int i_bottomRow);
		//
		//��������� �������� ������ ���������
		//
		void setScrollValue(int i_value);
		//
		//������� �������� �������
		//
		void closeCursor();
		//
		//������� ������������� �������
		//
		void initWidget();
		//
		//������� ���������� ������ � �������
		//
		void refreshData();

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
		//
		//���������� ��� �������� �������
		//
        QString sqlCursorColumns;
		QString sqlCursorTF;
		QString sqlCursorFilters;
		//
		//��������� �� ���� ������
        //
		KKSDatabase * db;	  

		//*****�����*****
        bool cursor_open; //���� �������� �������

		bool flag_clicked;//���� ������� ������
		bool flag_error;  //���� ������
		//**********

        int countColumn;  //����� ���������� ��������
        int countRow;	  //����� ���������� �����

		int t_dataRow;
		int b_dataRow;

		FiltersForm* filtersDialog;

		QStringList org_list;
		QStringList obj_list;
		QStringList res_list;

		QString dateFrom;
		QString dateTo;
};

#endif
