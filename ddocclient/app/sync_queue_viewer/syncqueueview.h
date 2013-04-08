#ifndef _SYNCQUEUEVIEW_H
#define _SYNCQUEUEVIEW_H

#include <QTreeView>
#include <QScrollBar>
#include <QHeaderView>
#include <QKeyEvent>

#include "sqv_delegate.h"

class SyncQueueViewerForm;

class SyncQueueView : public QTreeView
{
	Q_OBJECT

    public:
		//*****�������� � ����������� ���������� ������*****
        SyncQueueView(QWidget * parent);
        ~SyncQueueView();
		//**********

		//*****������� ��������� ������*****
		//
		//������� �������� �������� ����� ������� ������
		//
		bool getClearFlag() const;
		//
		//������� �������� ���������� ����� � �������
		//
		int getTotalRowCounts() const;
		//
		//������� �������� ���������� ������� �����
		//
		int getViewRowCounts() const;
		//**********

		//*****������� ��������� ������*****
		//
		//������� ��������� ������� ���������� ����� � �������
		//���� �������� �������, �� ������� false
		//
		bool setRowCount(int i_rowCount);
		//
		//��������� ����� ������ ������� �������
		//
		void setClearFlag(bool i_flag);
		//
		//��������� �������� ������ ���������
		//
		void setScrollValue(int i_value);
		//
		//��������� ���������� ������� �����
		//
		void setViewRows(int i_value);
		//**********

		//*****��������������� ������� *****
		//
		//������� ���������� ������ � ���������� �������
		//
        void updateData();
		//
		//������� ������� �������
		//
		void clear();
		//**********

    signals:
		void signal_viewRows(int top,int bottom);

    public slots:
		//
		//���� ��� ������� ���������� ������ ���������
		//
		void slot_sliderRealised();
		//
		//���� ��� ������� ������� �� ������ ���������
		//
        void slot_sliderPressed();
		//
		//���� ����������� ������� �����
		//� ���������� ������ ������������ ������ � �������� ������� � ������ ������
		//
		void slot_viewRows();
		//
		//���� ��������� ������� ������ ���������
		//� ���������� ������ ������������ ������ � �������� ������� � ������ ������
		//
		void slot_sliderValueChanged(int value);

	protected:
		virtual void keyPressEvent(QKeyEvent * i_event);//void keyPressEvent(QKeyEvent * i_event) virtual;
		virtual void resizeEvent (QResizeEvent * i_event);

    private:
		QScrollBar *scroll_view;//��������:)
		bool flag_scroll;       //���� ������� ������ ���������

		int totalRowCount;      //���������� ����� � �������
		int viewRowCount;       //���������� ������������ �����

		bool flag_clearAll;     //���� ������ �������
};

#endif
